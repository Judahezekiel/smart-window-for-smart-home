Smart Window System – Dependency & Special Requirements Documentation

Project by: Team Zudos
Developer: S Judah 


---

1. Hardware Dependencies

The smart window system relies on the following hardware components:

1.1 Microcontroller

ESP32 (Primary controller for automation and communication)


1.2 Sensors

LDR Sensor (Controls the blinds based on sunlight intensity)

Flame Sensor (Triggers the emergency fire exit mechanism)

Raindrop Sensor (Detects rain to trigger window and hanger movement)


1.3 Actuators

7 x Servo Motors (For window movement, blinds, hanger, and fire exit)


1.4 Power Requirements

5V & 3.3V Power Supply (ESP32 and servo motors require regulated power)

Backup Power (Optional) (To ensure functionality during power outages)


1.5 Communication Interfaces

GPIO Pins (For sensor and servo motor connections)

PWM Signals (For servo motor control)



---

2. Software Dependencies

The system operates using firmware developed for ESP32.

2.1 Development Environment

Arduino IDE (Primary programming tool)

PlatformIO (Optional) (For advanced development)


2.2 Programming Languages & Libraries

C++ (Arduino Framework)

ESP32 Servo Library (To control multiple servos)

Wi-Fi Library (if using IoT features)


2.3 Control Logic & Automation

Automatic Mode:

Window opens/closes based on rain detection.

Blinds adjust based on sunlight intensity.

Fire exit activates if flame sensor detects fire.

Clothes hanger moves when rain is detected.


Manual Mode:

User-controlled via buttons or an app (if Wi-Fi enabled).




---

3. Special Requirements

3.1 Mechanical Considerations

Window Structure should support servo-based movement.

Hanger Mechanism should allow smooth movement without obstruction.


3.2 Safety Requirements

Failsafe for Fire Exit: Fire exit servo should have a manual override.

Overcurrent Protection: Servo motors should not exceed their rated current to prevent overheating.


3.3 Environmental Considerations

Weatherproofing: Sensors exposed to rain should be waterproof or placed in protective casings.

Sunlight Sensor Calibration: LDR values should be tested for different lighting conditions.



---

4. Future Enhancements

Integration with a Smart Home System (Alexa, Google Assistant)

Remote Monitoring via IoT

Solar Power Integration

video link https://drive.google.com/file/d/1CnEbNAV0YJ7_VhwcbSZ7AHwRzRZWlk4t/view?usp=drivesdk
