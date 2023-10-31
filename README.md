# RC Car Project

This project aims to create an Arduino-controlled RC car.

## Description

The setup includes a Plusivo Arduino Uno, paired with an Arduino WiFi board, and a quad H-bridge motor controller. To control the car, the WiFi board initiates a server on a generated WiFi network, allowing access to a control interface through a web page. It can be managed from any device using the site's graphical user interface or by connecting any controller supported by the browser.

Two Arduino boards are used because the WiFi board lacks sufficient pins for the motor controller, and the Arduino Uno lacks built-in WiFi capability.

The car's chassis will be 3D printed using PLA filament, while the wheels, suspensions, motors, battery, and wires are purchased separately.

## Components

- 1x Arduino Uno board
- 1x Arduino WiFi board
- 1x Quad H-bridge motor controller shield
- 4x Wheels
- 4x Suspensions
- 1x 9V battery (which can be replaced with a rechargeable battery)
- 1x Servo motor for the steering arrangement
- 1-2 meters of wires

## Version 1.1
- Remade website to be more responsive
- Remade ArduinoWifi code to be more readable
- Removed the ability of the Arduino to connect to other wifi networks for simplicity

## Version 1.0
- Added project to GitHub

