#include <Adafruit_NeoPixel.h>
#include "OneButton.h"
#include "LowPower.h"
#define pushButton 2        //I-O
#define ledStrip 4          //LedStrip
#define wireless 5          //WirelessCharger
#define rgbPower 6          //RGBPower
#define wirOcto 7           //MagnetDetection
#define chargeOcto 8        //CheckIfTheDeviceIsGettingCharged
#define chrgLed 9           //CHRG LED
#define wirLed 10           //WIR LED
#define mainBatt A0         //BatteryMonitor
#define battTempPin A1      //BatteryTemperature
#define boostTempPin A2     //BoostConverterTemperature
#define wirelessTempPin A3  //WirelessChargerTemperature
#define lightSensor A6      //AmbientLightSensor
#define boostTempPin2 A7    //BoostConverter2Temperature
#define vref 4.85
Adafruit_NeoPixel strip(5, ledStrip, NEO_GRB + NEO_KHZ800);  //LedCount, LedPin, ColorStructure, CommunicationFrequency
OneButton button(2, true);
//Timing variables
unsigned long previousMillis;
unsigned long previousCheckMillis;
const unsigned long sleepThreshold = 30000;
const unsigned long checkThreshold = 5000;
//BatteryVariables
float minBattVolt = 2.7;
//LED variables
bool display = true;
//Wireless variables
bool wirelessState = false;
bool wirelessLed = true;
unsigned long previousWirelessTimeout;
const unsigned long wirelessTimeout = 15000;
//Mode variables
bool torch = false;
bool sosState = false;
bool sosStrideState = false;
//Temperature variables
const int thermistorPins[4] = { battTempPin, boostTempPin, wirelessTempPin, boostTempPin2 };
float temperatures[4];
bool startThresh = false;
#define thermistorNumber 4
#define thermistorRes 100000.0
#define betaValue 3950.0
//Environment sens variables
#define maxEnvADC 1023
#define minEnvADC 0
#define minBrightness 10
//Charge variables
bool chargeStatus = false;
bool previousChargeStatus = false;
void setup() {
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(ledStrip, OUTPUT);
  pinMode(wireless, OUTPUT);
  pinMode(rgbPower, OUTPUT);
  pinMode(wirOcto, INPUT);
  pinMode(chargeOcto, INPUT);
  pinMode(chrgLed, OUTPUT);
  pinMode(wirLed, OUTPUT);
  pinMode(mainBatt, INPUT);
  pinMode(battTempPin, INPUT);
  pinMode(boostTempPin, INPUT);
  pinMode(wirelessTempPin, INPUT);
  pinMode(lightSensor, INPUT);
  pinMode(boostTempPin2, INPUT);
  digitalWrite(wireless, !wirelessState);
  digitalWrite(rgbPower, LOW);
  strip.begin();
  strip.show();
  strip.setBrightness(255);
  digitalWrite(rgbPower, HIGH);
  button.attachClick(oneTap);
  button.attachDoubleClick(doubleTap);
  button.attachLongPressStop(longPress);
  button.attachMultiClick(SOS);
  Serial.begin(9600);
  detachInterrupt(digitalPinToInterrupt(2));
  oneTap();
}
void loop() {
  button.tick();
  chargeStatus = !digitalRead(chargeOcto);
  wirelessLed = !digitalRead(wirOcto);
  if (chargeStatus != previousChargeStatus) {
    digitalWrite(chrgLed, chargeStatus);
    stride(2, 255, 165, 0, 50, 100, true);
  }
  if (sosState) {
    sosStrideState = !sosStrideState;
    stride(1, 255 * sosStrideState, 0, 255 - 255 * sosStrideState, 25, 100, false);
  }
  if (wirelessState or torch or sosState or chargeStatus) {
    previousMillis = millis();
  }
  button.tick();
  if (millis() - previousMillis > sleepThreshold) {
    previousMillis = millis();
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
    startThresh = true;
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  }
  if (millis() - previousCheckMillis > checkThreshold) {
    display = false;
    oneTap();
    previousCheckMillis = millis();
  }
  if (wirelessState) {
    if (wirelessState and wirelessLed) {
      previousWirelessTimeout = millis();
    }
    if (millis() - previousWirelessTimeout > wirelessTimeout) {
      doubleTap();
      previousWirelessTimeout = millis();
    }
  }
  button.tick();
  previousChargeStatus = chargeStatus;
}
void oneTap() {
  int battAdc;
  int ledNum;
  float battVolt;
  float battPerCent;
  battAdc = analogRead(mainBatt);
  battVolt = (vref * battAdc) / 1023;
  battPerCent = ((battVolt - minBattVolt) / 1.5) * 100;
  Serial.print("Batt percent: ");
  Serial.println(battPerCent);
  for (int i = 0; i < thermistorNumber; i++) {
    float temperatureThreshold[4] = { 30.0 + 20 * !startThresh, 50 + 20 * !startThresh, 50 + 20 * !startThresh, 50 + 20 * !startThresh };
    temperatures[i] = analogRead(thermistorPins[i]);
    temperatures[i] = 1.0 / (log(((1024.0 / temperatures[i] - 1) * thermistorRes) / thermistorRes) / betaValue + 1.0 / (25.0 + 273.15)) - 273.15;
    Serial.print("Temperature ");
    Serial.print(i);
    Serial.print(" :");
    Serial.print(temperatures[i]);
    Serial.print("  ");
    if (temperatures[i] > temperatureThreshold[i]) {
      stride(5, 255, 0, 0, 50, 100, true);
      startThresh = true;
      attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
      digitalWrite(wireless, HIGH);
      digitalWrite(rgbPower, HIGH);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

    } else {
      Serial.println("PASSED");
    }
  }
  if (battPerCent < 5) {
    stride(3, 255, 0, 0, 50, 100, true);
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
    startThresh = true;
    digitalWrite(wireless, HIGH);
    digitalWrite(rgbPower, HIGH);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  } else {
    Serial.println("PASSED");
  }
  if (display) {
    ledNum = (battPerCent / 100) * strip.numPixels();
    digitalWrite(rgbPower, LOW);
    strip.setBrightness(envSens());
    for (int i = 0; i < ledNum; i++) {
      strip.setPixelColor(i, strip.Color(((strip.numPixels() - i) * 255) / 4, (i * 255) / 4, 0));
      strip.show();
      delay(50);
    }
    delay(2000 / ledNum);
    for (int i = 0; i < ledNum; i++) {
      strip.setPixelColor(i, strip.Color(255 * torch, 255 * torch, 255 * torch));
      strip.show();
      delay(50);
    }
    strip.setBrightness(255 * torch);
    digitalWrite(rgbPower, !torch);
  }
  display = true;
  startThresh = false;
}
void doubleTap() {
  wirelessState = !wirelessState;
  digitalWrite(wireless, !wirelessState);
  digitalWrite(wirLed, wirelessState);
  stride(2, 0, 255, 255, 50, 100, true);
}
void longPress() {
  Serial.println("Torch activated/deactivated");
  torch = !torch;
  digitalWrite(rgbPower, !(torch + sosState));
  strip.setBrightness(255);
  strip.fill(strip.Color(255 * torch, 255 * torch, 255 * torch));
  strip.show();
}
void SOS() {
  Serial.println("SOS activated/deactivated");
  sosState = !sosState;
  digitalWrite(rgbPower, !(torch + sosState));
  strip.clear();
  strip.setBrightness(255);
  strip.fill(strip.Color(255 * torch, 255 * torch, 255 * torch));
  strip.show();
}
void stride(int rep, int r, int g, int b, int gap, int gap2, bool empty) {
  digitalWrite(rgbPower, LOW);
  delay(100);
  for (int n = 0; n < rep; n++) {
    strip.setBrightness(envSens());
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
      strip.show();
      delay(gap);
    }
    if (empty) {
      delay(gap2);
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
        delay(gap);
      }
    }
  }
  digitalWrite(rgbPower, !(torch + sosState));
}
int envSens() {
  int envBrightness;
  envBrightness = map(analogRead(lightSensor), minEnvADC, maxEnvADC, minBrightness, 255);
  Serial.print("Env brightness: ");
  Serial.println(envBrightness);
  return envBrightness;
}
void wakeUp() {
  detachInterrupt(digitalPinToInterrupt(2));
}