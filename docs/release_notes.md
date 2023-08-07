---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes


## Vision Release v3.0.0

### New Features

* MH3-43851 Fix num of commands and params for external controller when set to maximum

### Known Issues

* Applications running on SAM E70 in combination with LCC will observe visual rendering artifacts on display during SD card R/W access. There is no loss in SD Card data.
* For applications on SAM E54 + CPRO with the 24-bit passthrough board, Pin 7 of the EXT1 connector should drive the backlight. However, on rev1.0 of the board, it is not connected to any pin on the MCU. As a workaround, it needs to be connected to a v3.3 pin.
* MISRA 2012 Rule 9.1 Compliance Deviation at legato_error.c line 74

For a list of post release issues that affect this release, refer to MPLAB Harmony [GFX Issues and Errata](https://github.com/Microchip-MPLAB-Harmony/gfx/wiki/Issues-and-Errata).

### Development Tools


* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator (MCC) v5.1.17](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
	
	OR
    * [MPLAB® Harmony Configurator (MHC) v3.8.3](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)


### Dependent Components


* [Core v3.11.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.11.1)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)




