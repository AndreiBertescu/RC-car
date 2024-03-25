# RC Car Project

This project aims to create an Arduino-controlled RC car.

<p align="center">
  <img src="https://github.com/AndreiBertescu/RC-car/assets/126001291/d989602e-664d-4bb8-bf9f-a01735db04b3" width="400" height="300" alt="Image 1">
  <img src="https://github.com/AndreiBertescu/RC-car/assets/126001291/b597887e-b9cc-4e5d-bb37-bd1ae5ddd54c" width="400" height="300" alt="Image 2">
  <img src="https://github.com/AndreiBertescu/RC-car/assets/126001291/c48d39e5-de0f-464f-ba90-510cc3caec6a" width="400" height="300" alt="Image 3">
  <img src="https://github.com/AndreiBertescu/RC-car/assets/126001291/d3102435-70fd-461a-8e1a-1cc156964d81" width="400" height="300" alt="Image 4">
</p>

## Description

The setup includes a Plusivo Arduino Uno, paired with an Arduino WiFi board, and a quad H-bridge motor controller. To control the car, the WiFi board initiates a server, a WiFi access point and starts a RC-car-control web page. The car can be controlled from any device using the site's graphical user interface or by connecting any controller supported by the browser.

Two Arduino boards are used because the WiFi board lacks access to the OCR2A/B and OCR0A/B timers, while the Arduino Uno lacks built-in WiFi capability.

The car's chassis was designed in Fusion360 and will be 3D printed using PLA filament, while the wheels, suspensions, motors, battery, and wires are purchased separately. The car can work in two modes: one where the servo is not used to steer the car, only the wheels, and one where the servo is connected to the two steering axles, thus turning the wheels. In the former working mode, two stoppers have been designed to prevent the wheels from spinning on their own.

## Components
- 1x Arduino UNO R3 board: [datasheet](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf)
- 1x WEMOS ESP8266 D1 R2 board: [details](https://wiki.geekworm.com/WEMOS_ESP8266_D1_R2_V2.1)
- 1x L293D quad H-bridge motor controller shield: [manual](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-motor-shield.pdf)
- 1x passive buzzer
- 4x 7.5cm diameter wheels.
- 4x 6cm hole-to-hole suspensions.
- 2x 6.5cm hole-to-hole steering axels (which have been extended).
- 1x MG996R 10Kg/cm servo motor for the steering arrangement.
- 2x 9V battery (which can be replaced with any rechargeable battery) - one for the motors and one for the control boards.
- A bunch of wires.
- Acces to a 3D-printer for the chassis.
- 
### Screws
- 8 x M3x20
- 20 x M4x15
- 12 x M4x40
- 16 x M5x30
- 16 x M5x40

## Controls for an Xbox controller
- Right joystick for steering
- Light joystick for speed
- Y button for START/STOP button
- B button for honk
- A button for steering lock

## Notes
- The joystick in the rc-car-control web page was made with the help of this tutorial: [link](https://www.instructables.com/Making-a-Joystick-With-HTML-pure-JavaScript/)
- The 'ArduinoWifiDriver' directory was included because of a recurring error when trying to write the program on the D1 R2 board.
- The website url to control the car is: http://192.168.4.1
- The website will load only if you are connected to the propper wifi network named '**RC-CAR_ESP8266**' with the password '**password**'.
- The Wheel-Connector model was not utilized. Instead, 4x M4x15 screws with a 2mm hole drilled in their center were used to connect the motors with the wheels.

## Version 1.4
- Added schematic
- Added passive buzzer with honk feature
- Added custom paintjob

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
- Car model
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/6e7a9fec-dae6-4ae7-8f0a-dee15875384b)

- Bought parts
![Img](https://github.com/AndreiBertescu/RC-car/assets/126001291/dbc102bd-0316-42f4-a8c7-17943650daa4)

- L293D motor controller
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/840d33b0-4b84-4b96-a6c2-4b503885e220)

- WEMOS ESP8266 D1 R2
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/7cb64203-c317-41ef-842d-d271aaec2f35)

- Arduino UNO R3
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/827fd8d9-7ad5-4c6d-983e-48532b179a33)

- Front wheels assembly
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/63b8bf6c-6c23-4376-a6e5-4054f689306f)

- Small car components prepared for 3d-printing
![image](https://github.com/AndreiBertescu/RC-car/assets/126001291/1e3a0aad-15d6-4f66-97d3-aacb0889842f)



