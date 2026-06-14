# ESP32 Battery Management System

## Project Overview

This project implements a Battery Management System (BMS) using the ESP32 microcontroller. The system monitors battery cell voltages, detects abnormal operating conditions, provides local Human Machine Interface (HMI) functionality, performs fault diagnostics, and enables cloud-based monitoring through the Blynk IoT platform.

The project was developed as part of an Embedded Systems and IoT Internship Program and is organized into six progressive tasks covering monitoring, safety, diagnostics, cloud telemetry, and dashboard analytics.

---

## Features

* Real-Time Battery Cell Voltage Monitoring
* Battery Health Analysis
* Weak Cell Detection
* Overvoltage Detection
* Voltage Imbalance Monitoring
* Rapid Voltage Fluctuation Detection
* Sensor Disconnection Detection
* Frozen ADC Detection
* Relay Mismatch Detection
* LCD-Based Human Machine Interface (HMI)
* Relay-Based Protection System
* Buzzer-Based Fault Alerts
* WiFi Connectivity using ESP32
* Cloud Telemetry using Blynk
* Executive Battery Intelligence Dashboard
* Historical Voltage Trend Visualization
* Risk Assessment and Diagnostics
* Intelligent Operator Recommendations

---

## Hardware Used

* ESP32 Development Board
* 16x2 I2C LCD Display
* Relay Module
* Buzzer
* Potentiometers (Battery Cell Simulation)
* WiFi Connectivity

---

## Software and Tools

* Visual Studio Code
* PlatformIO
* Wokwi Simulator
* Blynk IoT Platform
* GitHub

---

## Project Structure

```text
ESP32-Battery-Management-System
│
├── Task 1
│   ├── Battery Voltage Monitoring
│   └── Battery Health Analytics
│
├── Task 2
│   ├── Battery Safety Monitoring
│   └── LCD Status Display
│
├── Task 3
│   ├── Human Machine Interface (HMI)
│   └── Multi-Screen LCD Interface
│
├── Task 4
│   ├── Advanced Fault Detection
│   ├── Recovery Management
│   └── Fault Logging
│
├── Task 5
│   ├── Cloud Telemetry Architecture
│   ├── WiFi Connectivity
│   └── Blynk Integration
│
├── Task 6
│   ├── Executive Battery Intelligence Dashboard
│   ├── Risk Analysis
│   ├── Diagnostics
│   └── Operator Recommendations
│
└── INTERNSHIP PROJECT REPORT.docx
```

---

## Task Summary

### Task 1 – Battery Voltage Monitoring

* Battery Cell Voltage Measurement
* Battery Health Evaluation
* Average Voltage Calculation
* Strongest and Weakest Cell Identification
* Voltage Imbalance Analysis

### Task 2 – Battery Safety Monitoring

* Weak Cell Detection
* Overvoltage Detection
* Safety Status Monitoring
* LCD Status Display

### Task 3 – Human Machine Interface (HMI)

* Multi-Screen LCD Interface
* Battery Information Display
* Analytics Display
* Status Monitoring
* Diagnostic Information

### Task 4 – Advanced Fault Detection

* Rapid Voltage Fluctuation Detection
* Recovery State Management
* Fault Logging
* Relay and Buzzer Control

### Task 5 – Cloud Telemetry Architecture

* WiFi Connectivity
* Cloud Event Logging
* Blynk Integration
* Remote Monitoring
* RSSI Monitoring

### Task 6 – Executive Battery Intelligence Dashboard

* Live Battery Monitoring
* Historical Voltage Trends
* Runtime Mode Monitoring
* Risk Assessment
* Diagnostics Reporting
* Intelligent Operator Recommendations

---

## Cloud Telemetry Events

The system transmits the following telemetry events to the Blynk cloud platform:

* SYSTEM_START
* MODE_CHANGED
* INVALID_READING
* SENSOR_DISCONNECTED
* FROZEN_ADC
* RELAY_MISMATCH

---

## Dashboard Features

The Executive Battery Intelligence Dashboard provides:

* Live Cell Voltage Monitoring
* Historical Voltage Trend Charts
* Runtime Status Monitoring
* Risk Level Analysis
* Diagnostic Information
* Operator Recommendations
* WiFi RSSI Monitoring

---

## Results

The project successfully demonstrates:

* Battery Monitoring and Protection
* Embedded System Development
* Fault Detection and Diagnostics
* Human Machine Interface Design
* IoT Cloud Integration
* Real-Time Dashboard Visualization
* Cloud-Based Telemetry and Analytics

---

## Learning Outcomes

Through this project, the following skills were developed:

* ESP32 Programming
* Embedded C/C++
* PlatformIO Development
* IoT System Design
* Cloud Telemetry Integration
* Fault Detection Algorithms
* LCD Interfacing
* Git and GitHub Version Control
* Debugging and Troubleshooting
* Technical Documentation

---

## Author

**Vaibhavi B H**

Embedded Systems Internship Project

2026
