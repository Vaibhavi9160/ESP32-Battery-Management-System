# Adaptive-Multi-Cell-Battery-Intelligence-Engine

## Overview

This project implements an Adaptive Multi-Cell Battery Intelligence Engine using ESP32. The system continuously monitors a simulated 4-cell lithium battery pack, performs battery analytics, detects imbalance conditions, and classifies overall battery health.

The project was developed and simulated using Wokwi, VS Code, and PlatformIO.

---

## Features

* Real-time monitoring of 4 battery cell voltages
* Individual cell voltage calculation
* Average pack voltage calculation
* Strongest cell identification
* Weakest cell identification
* Battery imbalance percentage calculation
* Battery health classification
* Serial Monitor based diagnostics

---

## Components Used

### Inputs

* 4 Potentiometers (Battery Cell Simulation)

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

---

## Working Principle

The ESP32 continuously reads the voltage values of four simulated battery cells.

The system performs:

* Average voltage calculation
* Strongest cell identification
* Weakest cell identification
* Imbalance percentage calculation

Based on the calculated imbalance percentage, battery health is classified into:

* HEALTHY
* MINOR IMBALANCE
* CRITICAL IMBALANCE
* PACK FAILURE

---

## Simulation

The project was simulated using Wokwi simulator integrated with VS Code and PlatformIO.

---

## Project Output

The Serial Monitor displays:

* Cell Voltages
* Average Voltage
* Strongest Cell
* Weakest Cell
* Imbalance Percentage
* Battery Health Status

---

## Files Included

* Source Code
* Wokwi Simulation Files
* Project Screenshots

---
