# RC Car Project

This project aims to create an Arduino-controlled RC car.

## Description

The setup includes a Plusivo Arduino Uno, paired with an Arduino WiFi board, and a quad H-bridge motor controller. To control the car, the WiFi board initiates a server, a WiFi access point and starts a RC-car-control web page. The car can be controlled from any device using the site's graphical user interface or by connecting any controller supported by the browser.

Two Arduino boards are used because the WiFi board lacks access to the OCR2A/B and OCR0A/B timers, while the Arduino Uno lacks built-in WiFi capability.

The car's chassis was designed in Fusion360 and will be 3D printed using PLA filament, while the wheels, suspensions, motors, battery, and wires are purchased separately. The car can work in two modes: one where the servo is not used to steer the car, only the wheels, and one where the servo is connected to the two steering axles, thus turning the wheels. In the former working mode, two stoppers have been designed to prevent the wheels from spinning on their own.

## Components
- 1x Arduino UNO R3 board: [datasheet](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf)
- 1x WEMOS ESP8266 D1 R2 board: [details](https://wiki.geekworm.com/WEMOS_ESP8266_D1_R2_V2.1)
- 1x L293D quad H-bridge motor controller shield: [manual](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-motor-shield.pdf)
- 4x 7.5cm diameter wheels.
- 4x 6cm hole-to-hole suspensions.
- 2x 6.5cm hole-to-hole steering axels.
- 1x Servo motor for the steering arrangement.
- 1x 9V battery (which can be replaced with any rechargeable battery).
- A bunch of wires.
- Acces to a 3D-printer for the chassis.

## Notes
- The joystick in the rc-car-control web page was made with the help of this tutorial: [link](https://www.instructables.com/Making-a-Joystick-With-HTML-pure-JavaScript/)
- The 'ArduinoWifiDriver' directory was included because of a recurring error when trying to write the program on the D1 R2 board.
- The website url to control the car is: http://192.168.4.1
- The website will load only if you are connected to the propper wifi network named '**RC-CAR_ESP8266**' with the password '**password**'.

## Version 1.3
- Updated chassis models with two available extensions: one specific to Fusion360 and stl.
- The total amount of filament used is 123 meters.

## Version 1.2
- Rechecked the Arduino Uno code to ensure it works
- Updated the README to include the schematic and part names

## Version 1.1
- Remade website to be more responsive
- Remade ArduinoWifi code to be more readable
- Removed the ability of the Arduino to connect to other wifi networks for simplicity

## Version 1.0
- Added project to GitHub

## Images
- Bought parts
![Img](https://github.com/AndreiBertescu/RC-car/assets/126001291/dbc102bd-0316-42f4-a8c7-17943650daa4)

- L293D motor controller
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/840d33b0-4b84-4b96-a6c2-4b503885e220)

- WEMOS ESP8266 D1 R2
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/7cb64203-c317-41ef-842d-d271aaec2f35)

- Arduino UNO R3
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/827fd8d9-7ad5-4c6d-983e-48532b179a33)


