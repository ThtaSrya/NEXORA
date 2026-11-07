# NEXORA – Smart Room System

**NEXORA (Next-Generation Smart Room Automation)** is an ESP32-based smart room monitoring and control system designed to monitor room conditions and control electrical lighting through a web-based interface.

The system uses a DHT11 sensor to measure room temperature and humidity, an LCD 16×2 with I2C interface to display sensor readings and device status, and a relay module to control a lamp remotely through a web dashboard.

## Features

- **Real-Time Monitoring:** Measures room temperature and humidity using the DHT11 sensor.
- **Web-Based Control:** Turns the room lamp ON or OFF through a web browser.
- **Wi-Fi Connectivity:** Connects to a local Wi-Fi network using the ESP32.
- **LCD Display:** Displays temperature, humidity, and lamp status.
- **Relay Control:** Controls an electrical lamp using a 5V active-LOW relay module.
- **Responsive Dashboard:** Provides a modern and interactive web interface accessible from devices connected to the same network.

## Hardware Components

| Component | Function |
|---|---|
| ESP32 DevKit V1 | Main microcontroller |
| DHT11 | Temperature and humidity sensor |
| LCD 16×2 I2C | Information display |
| 1-Channel Relay 5V | Lamp switching control |
| LM2596 Buck Converter | Voltage regulation from 12V to 5V |
| 12V 2A DC Adapter | Main power supply |

## Pin Configuration

| ESP32 GPIO | Connected Component |
|---|---|
| GPIO 4 | DHT11 Data |
| GPIO 23 | Relay IN |
| GPIO 2 | Internal LED |
| GPIO 21 | LCD SDA |
| GPIO 22 | LCD SCL |

## Software & Libraries

- Arduino IDE
- ESP32 Arduino Core
- WiFi.h
- WebServer.h
- Wire.h
- LiquidCrystal_I2C
- DHT Sensor Library by Adafruit
- Adafruit Unified Sensor

## How It Works

1. The ESP32 connects to the configured Wi-Fi network.
2. The device obtains an IP address and displays it on the LCD.
3. The DHT11 sensor measures room temperature and humidity.
4. Sensor readings and lamp status are displayed on the LCD.
5. Users access the web dashboard through the ESP32 IP address.
6. Users can control the lamp using the ON and OFF buttons.
7. The relay switches the lamp according to the received command.

## Project Objectives

NEXORA aims to demonstrate the implementation of the Internet of Things (IoT) in a smart room environment by integrating sensors, microcontrollers, wireless communication, and web-based device control.

## Project Status

**Development in Progress**

Future improvements may include additional sensors, enhanced automation features, and a more comprehensive monitoring system.

---

**NEXORA – Monitor. Connect. Control.**

*Powered by ESP32.*
