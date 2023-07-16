# Perkys_XBOX_A4950_Version
Perky Robots EPS32 controlled by XBOX over wifi.


Code for Perky's robot head's transportation with a two motor 3 servo wheeled vehicle.

Perky consists of a StereoPi camera mounted on a Meccano chassis. The head can be turned left, right, up and down.

Bluetooth control of a A4950 motor driver equipped vehicle. 

Wiring can deduced from the pin assignments.

Reguires a handheld XBOX game controller. Button presses can be identified on hardwaretester.com

This software creates a webpage on your local wifi network to control the vehicle using the joystick and buttons on the XBOX controller.

The XBOX controller is linked to an android phone in the usual bluetooth manner and then the webpage from the vehicle is loaded in your browser. I use an Android app called Fing to find the ip addresses of the Stereopi and vehicle. One cell phone is required for the bluetooth connection another is required for the stereopi headset viewer.  The web page of the vehicle will be blank or ask for a BT controller to be connected.

The Stereopi sends a 3d view of the surroundings and the servo on the head allows for scanning around.  Android phones work. Apple phones, shrug, not so much.

Flash to your ESP32 with the Arduino IDE.

Have fun.
