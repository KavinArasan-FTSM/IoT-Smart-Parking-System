# Smart Parking System using ESP32 & AWS IoT

This project implements a **Smart Parking System** using an **ESP32**, sensors, and AWS IoT for real-time monitoring and gate control. The system can detect vehicle presence, open/close the gate automatically, and provide live parking status on an LCD display.

---

## Features

- Automatic gate control using **IR sensor** and **ultrasonic distance sensor**.
- Manual override using a **push button**.
- Visual feedback via **LEDs** and **buzzer** for gate status.
- Realtime parking availability display on **16x2 LCD**.
- **AWS IoT integration** for remote monitoring and control.
- Secure Wi-Fi and MQTT connection using **TLS certificates**.

---

## Hardware Components

- ESP32 microcontroller  
- IR Sensor  
- Ultrasonic Sensor (HC-SR04)  
- Servo Motor (for gate)  
- 16x2 I2C LCD Display  
- LEDs (Red & Green)  
- Buzzer  
- Push Button  

---

## Software / Libraries

- **Arduino IDE** or PlatformIO  
- **ESP32Servo** (Servo control)  
- **LiquidCrystal_I2C** (LCD display)  
- **PubSubClient** (MQTT client)  
- **ArduinoJson** (JSON parsing)  
- **WiFiClientSecure** (TLS Wi-Fi connection)  

---
1. Clone the repository:
```bash
git clone https://github.com/yourusername/Smart-Parking-System.git
cd Smart-Parking-System

2. Configure secrets:
Copy secrets_template.h to secrets.h.
Fill in your Wi-Fi credentials, AWS IoT endpoint, and certificates.

3. Upload code to ESP32:
Open main.ino in Arduino IDE.
Select the correct board and COM port.
Upload the code.

4. Power the system:
Ensure all sensors, servo, and LCD are connected properly.
The system will connect to Wi-Fi and AWS IoT automatically.

Usage :
Automatic Gate: Opens when a vehicle is detected within range (IR + ultrasonic sensor).
Manual Gate: Press the push button to open the gate.
Parking Status: LCD displays the number of available spots in real-time.
AWS IoT Control: Remote control via MQTT messages (topic: Parking_System).

Notes : Keep secrets.h private and do not commit it to GitHub.
        Ensure the servo has sufficient power to operate the gate.
        Adjust sensor pins in main.ino if your hardware wiring differs.
