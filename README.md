# OpenPower: Your Fully Open Source Portable Power Solution
Hello and welcome to the OpenPower GitHub repository! I'm Shaurya, a 17-year-old high school student passionate about electronics and innovation. ThePoweredProject began as a humble endeavor, driven by my dream of creating and selling a product. It all started with the idea of building a simple power bank for personal use, and through countless hours of hard work and dedication, it has evolved into something much bigger. ThePoweredProject will release more designs, but for now, OpenPower is it's only design. OpenPower is not just a power bank; it's a versatile, high-capacity portable power solution with a focus on safety, convenience, and customization, all built on a fully open-source foundation.

The USB-C PD negotiations are all handled by a cheap USB-C controller board. A separate logic board is in charge of all the Battery Monitoring, LED Strip control, Temperature checks, and inputs and outputs. This aforementioned logic board is powered by an Arduino Pro Mini and all the code for it is written in C++ in the Arduino IDE 2.2.1. A TPS259631DDAR power management IC protects the device against Over-Charge, Over-Discharge, Over-Current, and Short-Circuits. For a detailed look at the components, check out the schematic and PCB diagram!

# Features

# Safety First
OpenPower prioritizes your safety above all else. OpenPower contains advanced safety features that continuously monitor critical parameters to ensure a secure charging experience. These include:
Temperature Monitoring: OpenPower keeps a close eye on the battery pack, internal components, and ambient temperature to prevent overheating.
Voltage Control: The power bank monitors battery pack voltage to ensure it stays within the safe range.
Automatic Shutdown: If any of these parameters fall outside the normal operating range, OpenPower automatically shuts down to protect you and your devices.

# High-Capacity Battery
Boasting a high-capacity battery, OpenPower ensures you have ample power on the go to charge your devices multiple times without worry. Say goodbye to the anxiety of running out of battery when you need it most.

# USB-C Power Delivery
With USB-C Power Delivery technology, OpenPower can rapidly charge your devices, delivering quick and efficient power when you need it most. You can count on OpenPower to provide a speedy and reliable charging experience.

# MagSafe Compatibility
Designed to work seamlessly with MagSafe-enabled devices, OpenPower provides a secure and reliable charging experience for your Apple products. Effortless, hassle-free charging.

# Call for Help (SOS Mode)
In emergency situations, OpenPower has your back. By pressing the function button five times, you can activate an SOS mode. The LED strip will flash "SOS" in Morse code, providing you with a potentially life-saving communication method.

# Intuitive Multi-Purpose LED Strip
OpenPower features an intuitive LED strip that serves as a communication method, making it easy to check battery status and usage. It's user-friendly and simple to master, ensuring that you always know how much power you have left.

# Open Source
Embrace your inner hacker and customize OpenPower to suit your unique needs. Its open-source design allows for hacking and modification, giving tech enthusiasts the freedom to tailor the device to their preferences. Whether you're a tinkerer or a developer, OpenPower is your canvas for creativity.

# Vibrant Color Options
Express your personal style with OpenPower. It's available in a wide array of vibrant colors, so you can choose the one that matches your personality and stands out from the crowd. Make a statement with your power bank!

# Contributions
All contributors and enthusiasts are welcome. If you have ideas, suggestions, or improvements, please feel free to open issues and submit pull requests. Together, we can make OpenPower even better.

# 
Thank you for visiting the OpenPower repository and being a part of this journey. I hope you find OpenPower as exciting and useful as I do. Let's power up lives, one charge at a time!
