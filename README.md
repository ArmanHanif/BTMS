Battery Thermal Management System (BTMS) for Electric Vehicles
===============================================================

Project Overview:
-----------------
The Battery Thermal Management System (BTMS) is designed to monitor the temperature and smoke levels in an electric vehicle’s battery area to ensure safety and prevent overheating or fire hazards. The system uses sensors to gather data, which is processed by the ESP32 microcontroller. When the temperature or smoke exceeds a set threshold, an alert is triggered, and data is sent to a web dashboard for real-time monitoring.

Key Components:
---------------
- ESP32 microcontroller (Wi-Fi enabled)
- Temperature sensor
- Smoke sensor
- Buzzer or LED for alerts
- IoT dashboard for data visualization

Objectives:
-----------
- Detect and monitor battery temperature and smoke.
- Provide alerts when readings exceed safe limits.
- Display real-time data on an IoT dashboard.

Working Principle:
------------------
The sensors continuously monitor temperature and smoke levels. The ESP32 reads the data, checks for abnormalities, and:
1. Activates a buzzer or LED if a threshold is crossed.
2. Sends the data to an IoT dashboard for live tracking.

Project Outcomes:
-----------------
- Real-time monitoring of environmental conditions near the battery.
- Effective alert system to warn about overheating or smoke.
- Proof of concept for improving EV battery safety.

Future Enhancements:
--------------------
- Add cloud storage for long-term data logging.
- Implement fan or cooling system control.
- Use machine learning for predictive fault detection.

