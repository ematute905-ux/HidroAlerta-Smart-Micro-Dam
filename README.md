# HidroAlerta: Automated Smart Micro-Dam

A functional scale prototype of an automated micro-dam designed to address water channel waste management and flood prevention. The system integrates two ultrasonic sensors to monitor lateral waste collection containers, servo-controlled flow gates operated via a custom MIT App Inventor Bluetooth application, and a real-time voltage monitoring system for its integrated hydraulic generators.

## Project Context, Awards & My Role
This prototype was developed as a team project for our school Science Fair, where it was awarded **2nd Place overall**. 

**My specific role:** I acted as the Lead Electronics & Robotics Developer. I was solely responsible for designing the circuit, programming the Arduino logic, configuring the Bluetooth communication, and developing the mobile application in MIT App Inventor, while my teammates focused on the physical construction and theoretical research.

## Scalability & Institutional Outreach
Beyond the school Science Fair, this prototype was envisioned as a scalable solution for national water management. We actively initiated contact with the National Autonomous University of Honduras (UNAH) Research Center and the National Electric Energy Enterprise (ENEE) to explore the viability of implementing this automated system in real-world local channels. While currently remaining a proof-of-concept prototype, the system's architecture was deliberately designed to demonstrate how low-cost microcontrollers can solve large-scale infrastructural and environmental challenges.

## Hardware setup
- Brain: Arduino Uno
- Sensors: 2x Ultrasonic Sensors (Waste container fill levels), FZ0430 Voltage Sensor
- Actuators: 2x MG996R Servomotors (Flow gates control)
- Indicators: Red LED (Capacity Alert), White LED (Power status)
- Display: 16x2 LCD Display with I2C Module (Address 0x27)
- Connectivity: HC-06 Bluetooth Module (SoftwareSerial)
- App: Custom Android application built in MIT App Inventor

## Pinout
| Component | Arduino Pin | Function |
| :--- | :--- | :--- |
| Ultrasonic 1 (Trig / Echo) | Digital Pins 2 / 3 | Waste container 1 fill level |
| Ultrasonic 2 (Trig / Echo) | Digital Pins 6 / 7 | Waste container 2 fill level |
| Servomotor 1 | Digital Pin 9 | Left flow gate control |
| Servomotor 2 | Digital Pin 8 | Right flow gate control |
| FZ0430 Voltage Sensor | Analog Pin A0 | Reads voltage from hydraulic generators |
| Bluetooth RX / TX | Digital Pins 10 / 11 | SoftwareSerial communication |
| Red LED | Digital Pin 12 | High-capacity alert (>= 80%) |
| White LED | Digital Pin 13 | System active indicator |
| I2C LCD | SDA / SCL (A4 / A5) | Real-time telemetry display |

## How the code works
1. Bluetooth Telemetry & Control: The system continuously sends telemetry data (container fill percentages, generated voltage, and gate status) to the mobile app every second. It simultaneously listens for incoming wireless commands ('1' to open gates, '0' to close).
2. Dynamic Fill Mapping: The ultrasonic sensors measure the distance to the solid waste inside the containers. The code dynamically maps this physical distance to a 0-100% capacity range using constraints to prevent overflow errors.
3. Automated Hardware Alerts: If either waste container reaches 80% capacity or higher, the system triggers a Red LED hardware alert, signaling the need for physical maintenance.

## Core logic snippet

```cpp
// Container percentage calculation and Alert System
int dist1 = medirDistancia(trig1, echo1);
int porc1 = constrain(map(dist1, distanciaVacio, distanciaLleno, 0, 100), 0, 100);

int dist2 = medirDistancia(trig2, echo2);
int porc2 = constrain(map(dist2, distanciaVacio, distanciaLleno, 0, 100), 0, 100);

// Hardware alert if any container is >= 80% full
if (porc1 >= 80 || porc2 >= 80) {
  digitalWrite(ledRojo, HIGH);
} else {
  digitalWrite(ledRojo, LOW);
}
```

## Project photos & media

**1. Full prototype aerial view**
<img width="1080" height="1224" alt="IMG-20260528-WA0023" src="https://github.com/user-attachments/assets/89f7d1c6-8f25-46eb-a964-b8e704d5c2bd" />


**2. Waste collection & dam zones**
<img width="1053" height="1600" alt="IMG-20260528-WA0020" src="https://github.com/user-attachments/assets/4b1f32ff-b6c3-4aa8-b1cd-943a834be1fa" />


**3. 3D gear & hydraulic wheel design**
<img width="992" height="984" alt="IMG-20260528-WA0021" src="https://github.com/user-attachments/assets/41e853ac-b1b7-4541-a061-4c16396d3995" />


**4. Gear & generator motor integration**
<img width="1600" height="1200" alt="IMG-20260524-WA0012" src="https://github.com/user-attachments/assets/6890b9c0-bc89-4142-b7d0-7d964b42bfa4" />


**5. Custom Mobile App Interface**
<img width="405" height="474" alt="Captura de pantalla 2026-10-25 195824" src="https://github.com/user-attachments/assets/5bcf8671-9b4e-4621-aa78-4977ab0a6063" />


[📥 Download MIT App Inventor source code (.aia)](AQUI_PONES_EL_LINK_AL_ARCHIVO_AIA)

**▶ Project demonstration video**
[ARRASTRA_AQUI_EL_VIDEO_DE_9MB]
