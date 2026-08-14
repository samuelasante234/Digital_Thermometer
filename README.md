# MSP430 Digital Thermometer

A bare-metal digital thermometer built around the Texas Instruments **MSP430G2553** microcontroller. This project reads temperature data from an LM35 temperature sensor and displays it on an **ST7789** TFT display via hardware SPI. 

This repository avoids bloated IDE auto-generated code. Instead, it relies on a custom-written hardware abstraction layer (HAL), register bit-field mappings, and a manually configured `Makefile` utilizing `msp430-gcc`.

## Features
* **Bare-Metal C**: Direct register manipulation and hardware-level control.
* **Hardware SPI**: Fast communication with the ST7789 display using the USCI_A0 peripheral.
* **Data Transfer Controller (DTC)**: Uses the MSP430's DTC to offload ADC sampling from the CPU, transferring 5 continuous samples directly into memory before triggering an interrupt.
* **Custom Bit-Field Mappings**: Clean, struct-union based register definitions for peripheral configuration.
* **Modular Architecture**: Strictly separated concerns for drivers, HAL, and fonts.

## Hardware Requirements
* MSP430G2553 (e.g., TI LaunchPad)
* ST7789 TFT Display Module (SPI)
* LM35 Temperature Sensor
* Jumper wires & Breadboard

## Building and Flashing
This project requires the `msp430-gcc` toolchain and `mspdebug`.

To compile the firmware:
```bash
make
```

To flash the firmware to the microcontroller:
```bash
make flash
```
