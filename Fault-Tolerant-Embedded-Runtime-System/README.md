# Fault-Tolerant-Embedded-Runtime-System

## Overview

This project implements a Fault-Tolerant Embedded Runtime System for a battery monitoring and protection application using ESP32. The system autonomously detects hardware and software failures, transitions between runtime operating modes, isolates faulty modules, maintains fault logs, and performs structured recovery operations.

The project was developed and simulated using Wokwi, VS Code, and PlatformIO.

---

## Features

* Runtime Mode Management
* Invalid Reading Detection
* Frozen ADC Detection
* Sensor Disconnection Detection
* Relay Mismatch Detection
* Fault Logging System
* Timestamp-Based Fault Recording
* Structured Recovery Mechanism
* Fault Isolation Support
* Automatic Runtime State Transitions

---

## Runtime Modes

### NORMAL

System operates under healthy conditions.

### DEGRADED

Non-critical faults are detected while healthy modules continue operating.

### FAILSAFE

Critical faults are detected and protection mechanisms are activated.

### SHUTDOWN

Severe fault conditions trigger complete system shutdown for safety.

---

## Components Used

### Inputs

* 4 Potentiometers (Battery Cell Simulation)

### Outputs

* LCD 16x2 I2C Display
* Relay Module
* Active Buzzer

### Controller

* ESP32 Dev Board

---

## Software Used

* VS Code
* PlatformIO
* Wokwi Simulator

---

## Circuit Connections

| Component | ESP32 GPIO |
| --------- | ---------- |
| Cell 1    | GPIO34     |
| Cell 2    | GPIO35     |
| Cell 3    | GPIO32     |
| Cell 4    | GPIO33     |
| Relay     | GPIO18     |
| Buzzer    | GPIO19     |
| LCD SDA   | GPIO21     |
| LCD SCL   | GPIO22     |

---

## Fault Detection Mechanisms

### Invalid Reading Detection

Detects battery voltage readings outside valid operating limits.

### Frozen ADC Detection

Detects ADC values that remain unchanged for a prolonged period, indicating possible sensor or software faults.

### Sensor Disconnection Detection

Detects disconnected or failed sensors based on abnormal voltage conditions.

### Relay Mismatch Detection

Compares commanded relay state with feedback state and detects inconsistencies.

---

## Fault Logging System

The system maintains timestamped fault records containing:

* Fault Type
* Fault Occurrence Time
* Runtime Mode

The fault logger stores recent fault history for diagnostic purposes.

---

## Structured Recovery

When fault conditions are removed, the system enters a recovery state and waits for a stabilization period before returning to NORMAL operation.

This prevents unstable state transitions and improves runtime reliability.

---

## Fault Isolation

Faulty modules are isolated while healthy modules continue operating whenever possible. This improves system availability and prevents unnecessary shutdowns.

---

## Simulation

The project was simulated using Wokwi simulator integrated with VS Code and PlatformIO.

---

## Project Output

The system displays:

* Runtime Mode Status
* Fault Messages
* Recovery Information
* Diagnostic Information

The Serial Monitor displays:

* Fault Logs
* Runtime State Transitions
* Diagnostic Events

---

## Files Included

* Source Code
* Wokwi Simulation Files
* Fault Management Modules
* Fault Logging Modules
* Project Screenshots

---

## Conclusion

The Fault-Tolerant Embedded Runtime System successfully demonstrates autonomous fault detection, runtime mode management, structured recovery, fault isolation, and diagnostic logging. The project provides a reliable embedded software architecture suitable for safety-critical battery monitoring applications.
