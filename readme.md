![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Microchip Technology](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# MPLAB® Harmony 3 Vision Package

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for developing embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.

This repository contains the MPLAB® Harmony 3 Vision package, which provides a free, fast-to-market camera software development environment for Microchip 32-bit SAM microprocessor devices.

## Useful Links

For more information, refer to the following resources:

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)
- [MPLAB® Discover](https://mplab-discover.microchip.com/v1/itemtype/com.microchip.ide.project?s0=Legato)

Package-specific resources:

- [Release Notes](./release_notes.md)
- [MPLAB® Harmony License](./mplab_harmony_license.md)
- [MPLAB® Harmony 3 Vision Applications]()

## Features

Key features of the MPLAB® Harmony 3 Vision package include:

- Hardware-optimized support for Microchip 32-bit SAM microprocessors
- MPLAB Code Configurator support through compatible components
- C-based implementation with MISRA C Mandatory compliance
- Support for image sensors including:
  - Sony IMX219
  - OmniVision OV2640
  - OmniVision OV5640 with built-in JPEG support
  - OmniVision OV5647
- MIPI CSI-2 and parallel camera interface support
- Vision libraries and example applications for camera-based development

## Contents Summary

| Category | Item | Description | Release Type |
| --- | --- | --- | --- |
| drivers | ISC | Driver for the Image Sensor Controller peripheral | ![driver-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| drivers | CSI2DC | Driver for the CSI-2 Demultiplexer Controller | ![driver-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| drivers | CSI | Driver for the Camera Serial Interface | ![driver-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| drivers | image_sensor | Control interface driver for image sensors | ![driver-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| library | libcamera | Vision library | ![library-beta](https://img.shields.io/badge/library-beta-orange?style=plastic) |
| image_sensors | driver | Image sensor configuration drivers | ![driver-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| image_sensors | IMX219 | Sony IMX219 MIPI CSI image sensor | ![sensor-beta](https://img.shields.io/badge/sensor-beta-orange?style=plastic) |
| image_sensors | OV5647 | OmniVision OV5647 MIPI CSI image sensor | ![sensor-beta](https://img.shields.io/badge/sensor-beta-orange?style=plastic) |
| image_sensors | OV5640 | OmniVision OV5640 parallel interface image sensor | ![sensor-beta](https://img.shields.io/badge/sensor-beta-orange?style=plastic) |
| image_sensors | OV2640 | OmniVision OV2640 parallel interface image sensor | ![sensor-beta](https://img.shields.io/badge/sensor-beta-orange?style=plastic) |
| apps | examples | Vision example applications | ![app-beta](https://img.shields.io/badge/app-beta-orange?style=plastic) |

## License

This package is distributed under the MPLAB® Harmony License.

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/blob/master/mplab_harmony_license.md)

## Connect with Microchip

[![Follow us on YouTube](https://img.shields.io/badge/YouTube-Follow%20us-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

## Repository Activity

[![GitHub stars](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()
[![GitHub watchers](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()