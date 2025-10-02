![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Vision Release v3.2.1

### New Features

* Added Defective Pixel Correction.
* Enable Histogram and implement Auto White Balance Algorithm.
* Moved Vision Apllications to new vision_apps repository.
* Add RGB parallel inferace support for SAM9x75

### Fixed Known Issues

* Fix image freeze issue.

### Development Tools

* [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator Plug-In v5.5.2](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
    * [MPLAB® Code Configurator Core v5.7.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

### Dependent Components

* [csp v3.23.0](https://github.com/Microchip-MPLAB-Harmony/csp/releases/tag/v3.23.0)
* [core v3.15.5](https://github.com/Microchip-MPLAB-Harmony/core/tree/v3.15.5)

## Vision Release v3.2.0

### New Features

* Introduce support for the SAM9x75 Curiosity Board.
* Implement a demonstration application for the SAM9x75 Curiosity Board utilizing the IMX219 and NewVision LVDS Display.
* Implement a demonstration application for the SAM9x75 Curiosity Board utilizing the IMX219 and NewVision AC69T88A Display.
* Integrate the OV5647 MIPI CSI Image Sensor with support for VGA, 720p, and 1080p resolutions.
* Enable 720p and 1080p resolution support in the IMX219 Image Sensor.
* Add image scaling support in the ISC.

### Fixed Known Issues

* Provide options in the MCC to enable or disable log messages.
* Manage the FPS count based on the VSync detect interrupt.
* Fix duplicate MCK clock divider entries in the MCC.
* Configure CAMERA_RESET and CAMERA_PWD peripheral I/O pin settings within the MCC instead of managing them through libcamera.
* Merge the CSI and CS2DC driver MCC menus, as well as the CSI and CSI2DC PLIBs MCC menus, into a single MCC menu under CSI.
* Combine the ISC driver menu and ISC PLIB menu into one consolidated MCC menu under ISC.
* Relocate all PLIB source and header files to their respective modules under drivers.
* Perform code cleanup and remove any unused code.

### Development Tools

* [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator Plug-In v5.5.2](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
    * [MPLAB® Code Configurator Core v5.7.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

### Dependent Components

* [csp v3.21.0](https://github.com/Microchip-MPLAB-Harmony/csp/releases/tag/v3.18.2)
* [core v3.14.2](https://github.com/Microchip-MPLAB-Harmony/core/tree/v3.13.2)
* [bsp v3.22.0](https://github.com/Microchip-MPLAB-Harmony/bsp/releases/tag/v3.17.0)
* [Gfx v3.16.0](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.14.0)
* [usb v3.15.0](https://github.com/Microchip-MPLAB-Harmony/usb/releases/tag/v3.12.0)

## Vision Release v3.1.0

### New Features

* Add SAMA5D2 MPU support
* Add SAMA7G54 MPU support
* Add an application demo to support SOM1EK board
* Add an application demo to support WL SOM1EK board
* Add ov2640 and ov5640 Image sensor
* Add 1080p support in IMX219 Image sensor

### Fixed Known Issues

* dynamically configure CSI N lanes & Bit rate
* Add Applications demo for sama7G5-ek board
* Replace Width and Height Image sensor MCC configuration with Resolution configuration
* Add an vision application demo to support SOM1EK board
* set register values using same macro defined in all MPU headers.
* source code indent & formatting.
* fix configuring Image sensor's
* Sony IMX219 reset not working
* indent & format source code.
* regenerate libcamera_display vision application code.
* SAMA7G5: saving frames to MSD is very slow.
* add new api to start and stop ISC capture
* add a api to get FPS info.
* Fix IMX219 to output 30fps for vga resolution
* add bin file to git attributes.
* fix build error's observed due to changes in ISC.h header file
* use printf to print debug messages on console.
* add WVGA resolution support
* add image sensor i2c mode support
* update ISC default values based on the MPU type in MCC configurations.
* configure ISC Hsync and Vsync polorities values
* configure ISC white balance values
* Hue and Saturation is not supported for sama5d2
* Fix image sensor detect
* configure camera pwd gpio pin.
* configure ISC Gamma values
* fix build errors for SAMA5D2 som1 ek1 board
* fix compilation error's based on ISC interface type.
* code cleanup
* reduce DMA_MAX_BUFFERS size.
* Add an vision application demo to support WL SOM1EK board
* generate offline docs for vision 3.1.0 release
* Configure Gamma correction only if input format type is Bayer Pattern

### Development Tools

* [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator Plug-In v5.5.0](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
    * [MPLAB® Code Configurator Core v5.7.0](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

### Dependent Components

* [dev_packs v3.18.1](https://github.com/Microchip-MPLAB-Harmony/dev_packs/releases/tag/v3.18.1)
* [csp v3.18.2](https://github.com/Microchip-MPLAB-Harmony/csp/releases/tag/v3.18.2)
* [core v3.13.2](https://github.com/Microchip-MPLAB-Harmony/core/tree/v3.13.2)
* [bsp v3.17.0](https://github.com/Microchip-MPLAB-Harmony/bsp/releases/tag/v3.17.0)
* [Gfx v3.14.0](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.14.0)
* [usb v3.12.0](https://github.com/Microchip-MPLAB-Harmony/usb/releases/tag/v3.12.0)


## Vision Release v3.0.0

### New Features

* Adds Harmony 3 component to support MIPI-C-PHY peripheral
* Adds Harmony 3 component to support Image Sensor peripheral
* Adds Harmony 3 component to support Sony IMX219 Camera Module
* Adds libCamera Demo for SAM9X75-DDR3-EB Early Access Evaluation Board using the MIPI CSI interface to capture video frames from the Sony IMX219 Camera Module and display it on to the AC69T88A LVDS display

### Known Issues

* None

### Development Tools

* [MPLAB® X IDE v6.10](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.30](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator Plug-In v5.3.7](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
    * [MPLAB® Code Configurator Core v5.5.7](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

### Dependent Components


* [dev_packs v3.17.0](https://github.com/Microchip-MPLAB-Harmony/dev_packs/releases/tag/v3.17.0)
* [csp v3.17.0](https://github.com/Microchip-MPLAB-Harmony/csp/releases/tag/v3.17.0)
* [core v3.13.1](https://github.com/Microchip-MPLAB-Harmony/core/tree/v3.13.1)
* [bsp v3.16.1](https://github.com/Microchip-MPLAB-Harmony/bsp/releases/tag/v3.16.1)
* [Gfx v3.13.0 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.0)
* [usb v3.11.0-E2](https://github.com/Microchip-MPLAB-Harmony/usb/releases/tag/v3.11.0-E2)




