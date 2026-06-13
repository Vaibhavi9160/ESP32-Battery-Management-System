# Event-Driven-Safety-Protection-Kernel

## Overview

This project implements an Event-Driven Safety Protection Kernel for a simulated lithium battery pack using ESP32. The system continuously monitors battery conditions and performs protection actions during abnormal operating conditions.

The design follows a non-blocking event-driven architecture using millis() and implements stable state management, recovery logic, and anti-relay chatter protection.

---

## Features

* Weak Cell Voltage Detection
* Overvoltage Detection
* Rapid Voltage Fluctuation Detection
* Relay Cutoff Protection
* Buzzer Fault Alerts
* LCD Fault Indication
* Recovery Logic
* Anti-Relay Chatter Protection
* State Machine Based Design
* Non-Blocking millis() Scheduling

---

## Components Used

### Inputs

* 4 Potentiometers (Battery Cell Simulation)

### Outputs

* Relay Module
* Active Buzzer
* LCD 16x2 I2C Display

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

The ESP32 continuously monitors all battery cells.

When abnormal conditions are detected:

* Weak Cell Voltage
* Overvoltage
* Voltage Instability

the system:

* Disconnects the relay
* Activates the buzzer
* Displays fault messages on LCD

A state machine is implemented with:

* NORMAL
* FAULT
* RECOVERY

states to ensure stable operation.

---

## Simulation

The project was simulated using Wokwi simulator integrated with VS Code and PlatformIO.

---

## Project Output

The LCD displays:

* System Status
* Fault Messages
* Recovery Status

The system automatically performs protection actions based on battery conditions.

---

## Files Included

* Source Code
* Wokwi Simulation Files
* Project Screenshots

---
