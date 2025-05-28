# Automated Pergola with Arduino UNO
-----------------------------------

**Project developed by:** Ilie Ioan-Călin  
**Year:** 2025  
**Course:** Microcontroller Design Project  

## Description
This project implements the automation of a pergola that extends or retracts based on weather conditions detected by rain, temperature, and humidity sensors.

The microcontroller used is an Arduino UNO (ATmega328P). Commands to the pergola motors are transmitted through an L293D motor driver, while the user receives visual feedback via a 16x2 LCD (I2C) and LEDs.

## Project Structure

- `main_pergola.ino` – Main Arduino source code  
- `schema_electrica.png` – Exported wiring diagram (created in KiCad)  
- `schema.kicad_sch` – Native KiCad schematic file  
- `diagrama_semnal.png` – Signal diagram generated in WaveDrom  
- `diagrama_semnal.json` – WaveDrom source code  
- `demo_pergola_video.mp4` – System demonstration video  
- `README.txt` – This documentation file  
- `ChangeLog.txt` – Change history and implementation steps  

## Usage Instructions

1. Open the `main_pergola.ino` file in the Arduino IDE.
2. Select the Arduino UNO board and the correct port.
3. Upload the project to the board.
4. Ensure proper power supply for both the Arduino and the motors.
5. The system will start reading sensor data and automatically control the pergola.

## Extension Conditions

- Rain detected (analog value below 700)
- Humidity > 60%
- Temperature < 15°C

## User Feedback

- 16x2 LCD display that alternates between pergola status and sensor values  
- Red LED ON → Pergola is **extended**  
- Green LED ON → Pergola is **retracted**

## External Resources

**Libraries used:**

- `LCD_I2C.h`
- `DHT.h`
- `Wire.h`

## Compatibility

Tested on Arduino UNO (ATmega328P). A separate power supply for the motors is recommended.
