# Intelligent-Embedded-HMI-Diagnostic-Interface

## Overview

This project implements a professional Human-Machine Interface (HMI) for an embedded battery monitoring and protection system using ESP32 and a 16x2 LCD display.

The interface automatically rotates between multiple diagnostic screens displaying live battery data, analytics, protection status, and fault diagnostics while supporting fault-priority screen overrides.

The project was developed and simulated using Wokwi, VS Code, and PlatformIO.

---

## Features

* Multi-Screen LCD Interface
* Automatic Screen Rotation
* Live Battery Data Display
* Battery Analytics Display
* Protection Status Display
* Diagnostic Information Display
* Fault Priority Screen Override
* Recovery Status Display
* Embedded HMI Architecture
* Real-Time Data Visualization

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

## Working Principle

The LCD automatically rotates through multiple screens:

### Battery Screen

Displays:

* Cell Voltages

### Analytics Screen

Displays:

* Average Voltage
* Imbalance Percentage

### Protection Status Screen

Displays:

* Relay Status
* System State

### Diagnostic Screen

Displays:

* Weak Cell Status
* Overvoltage Status

During critical battery conditions, normal screen rotation is suspended and fault information is displayed immediately.

---

## Simulation

The project was simulated using Wokwi simulator integrated with VS Code and PlatformIO.

---

## Project Output

The LCD continuously rotates through multiple diagnostic screens and automatically switches to fault-priority screens during abnormal operating conditions.

---

## Files Included

* Source Code
* Wokwi Simulation Files
* Project Screenshots

---
