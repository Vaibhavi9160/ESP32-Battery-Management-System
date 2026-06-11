# Task 5: Intelligent Cloud Telemetry Architecture

## Overview

This project implements an Intelligent Cloud Telemetry Architecture for a Battery Management System (BMS) using ESP32 and the Blynk IoT platform. The system transmits telemetry data only during important events such as fault detection, runtime mode transitions, and abnormal operating conditions instead of continuously sending data at fixed intervals.

The architecture is designed to ensure reliable cloud communication while allowing the embedded system to continue operating independently during network interruptions.

---

## Objectives

* Implement cloud communication using Blynk IoT.
* Transmit data only during state changes and anomalies.
* Monitor battery health and fault conditions.
* Support automatic cloud reconnection.
* Monitor wireless signal quality.
* Ensure non-blocking operation during communication failures.
* Maintain reliable telemetry delivery.

---

## Hardware Used

* ESP32 Development Board
* Potentiometers (Battery Cell Voltage Simulation)
* Relay Module
* Buzzer
* 16x2 I2C LCD Display

---

## Software Tools

* PlatformIO
* Visual Studio Code
* Wokwi Simulator
* Blynk IoT Cloud
* Arduino Framework

---

## Implemented Features

### 1. Blynk Cloud Integration

The ESP32 connects to the Blynk Cloud platform using WiFi and periodically maintains the cloud connection.

Features:

* Cloud authentication using Blynk Auth Token
* Online device monitoring
* Remote telemetry transmission

---

### 2. Event Driven Telemetry

Unlike traditional fixed interval telemetry systems, data is transmitted only when significant events occur.

Examples:

* Invalid battery readings
* Sensor disconnection detection
* Frozen ADC detection
* Relay mismatch detection
* Runtime mode changes

Benefits:

* Reduced bandwidth usage
* Lower cloud traffic
* Improved efficiency

---

### 3. Runtime Mode Monitoring

The system supports multiple runtime operating modes:

* NORMAL
* DEGRADED
* FAILSAFE
* SHUTDOWN

Mode transitions are reported to the cloud whenever a state change occurs.

---

### 4. Fault Reporting

The following faults are monitored and reported:

#### Invalid Reading Detection

Detects battery voltages outside valid operating limits.

#### Sensor Disconnection Detection

Detects missing or disconnected sensor inputs.

#### Frozen ADC Detection

Detects ADC channels that remain unchanged for an extended period.

#### Relay Mismatch Detection

Detects discrepancies between commanded and actual relay states.

---

### 5. WiFi Reconnection Handling

The system continuously monitors cloud connectivity.

If the connection is lost:

* Automatic reconnection attempts are initiated.
* Embedded monitoring continues without interruption.
* Cloud services resume automatically after reconnection.

---

### 6. Signal Quality Monitoring

Wireless signal strength is monitored using RSSI values.

Functions:

* Network quality assessment
* Communication diagnostics
* Cloud monitoring support

RSSI values are transmitted to Blynk for remote observation.

---

### 7. Non-Blocking Architecture

The telemetry system is designed so that cloud communication failures do not affect battery monitoring functionality.

The following subsystems continue operating normally:

* Fault Detection
* LCD Interface
* Relay Control
* Safety Monitoring
* Runtime State Management

---

## Blynk Dashboard Configuration

| Virtual Pin | Function       |
| ----------- | -------------- |
| V0          | Event Messages |
| V1          | Cell 1 Voltage |
| V2          | Cell 2 Voltage |
| V3          | Cell 3 Voltage |
| V4          | Cell 4 Voltage |
| V5          | WiFi RSSI      |

---

## Sample Telemetry Events

SYSTEM_START

INVALID_READING

SENSOR_DISCONNECTED

FROZEN_ADC

RELAY_MISMATCH

MODE_NORMAL

MODE_DEGRADED

MODE_FAILSAFE

MODE_SHUTDOWN

---

## Results

Successfully demonstrated:

* Real-time Blynk cloud connectivity
* Event-based telemetry transmission
* Fault detection and reporting
* Runtime mode monitoring
* WiFi reconnection handling
* RSSI signal monitoring
* Continuous embedded operation during network disruptions

---

## Conclusion

The Intelligent Cloud Telemetry Architecture successfully integrates ESP32-based battery monitoring with Blynk cloud services. By transmitting only significant events and runtime transitions, the system reduces communication overhead while maintaining reliable remote monitoring and fault reporting capabilities.
