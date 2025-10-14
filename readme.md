![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Microchip Technology](./images/mh.png)

# MPLAB® Harmony 3 Vision Package

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating
embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller
and microprocessor devices.  Refer to the following links for more information:
 - [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
 - [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
 - [Microchip MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide)
 - [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
 - [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)
 - [MPLAB® Discover](https://mplab-discover.microchip.com/v1/itemtype/com.microchip.ide.project?s0=Legato)

This repository contains MPLAB® Harmony 3 Vision package. The package supports a free fast to market,
camera software development environment for Microchip MPLAB® 32-bit SAM microprocessor devices. Refer 
following links for release notes, License and vision application examples.
 - [Release Notes](./release_notes.md)
 - [MPLAB® Harmony License](./mplab_harmony_license.md)
 - [MPLAB® Harmony 3 Vision Applications]()
 
# Features

The key features of the MPLAB® Harmony Vision are the following:

- Hardware optimized for use with Microchip 32-bit SAM MicroProcessor's.
- Compatible component for use with MPLAB Code Configurator (MCC)
- Written in C with MISRA C (Mandatory) compliancy
- currebtly supported Image Sensor are Sony IMX219, OV2640 and OV5640 (with buitin Jpeg)
- MIPI-CSI2 and Parallel Interface support

# Contents Summary

| Category | Item | Description | Release Type |
| --- | --- | ---- |---- |
| drivers|  ISC | Driver for the Image Sensor Controller (ISC) peripheral | ![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      |   CSI2DC | Driver for the CSI-2 Demultiplexer Controller (CSI2DC) |![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      |   CSI |  Driver for the Camera Serial Interface (CSI) | ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
|      |   image_sensor | Control interface Driver for the Image sensors | ![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
| library    | libcamera | Vision library | ![app-beta](https://img.shields.io/badge/library-beta-orange?style=plastic) |
| image_sensors | driver | different Image sensor configurations| ![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      | IMX219 | MIPI-CSI2 Sony IMX219 MIPI CSI Image Sensor |![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      | OV5647 | OminiVision OV5647 MIPI CSI mage Sensor |![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      | OV5640 | OminiVision OV5640 Parallel interface Image Sensor |![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
|      | OV2640 | OminiVision OV2640 Parallel interface Image Sensor |![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |
| apps | examples | Vision Example applications | ![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/blob/master/mplab_harmony_license.md)

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()

