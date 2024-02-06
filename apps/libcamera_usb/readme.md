---
parent: Vision Example Applications
title: Libcamera USB
nav_order: 1
---

# Camera Demo Applications

These application capture video frames from image sensor module either using MIPI CSI2 or Parallel interface interface and save the captured frame in a bmp/jpeg file and copied to a USB drive using below listed development board.

|MPLABX Configuration|Board Configuration|
|:-------------------|:------------------|
|[libcamera_usb_sama7g54_ek.X](./firmware/libcamera_usb_sama7g54_ek.X/readme.md)| [SAMA7G54-EK Board](https://www.microchip.com/en-us/development-tool/ev21h18a) using the MIPI CSI interface to capture video frames from the [Sony IMX219 Camera Module](https://www.raspberrypi.com/products/camera-module-v2/) and USB Flash drive of FAT filesystem |
|[libcamera_usb_sama5d27_wlsom1_ek1.X](./firmware/libcamera_usb_sama5d27_wlsom1_ek1.X/readme.md)| [SAMA5 WLSOM1 EK1 Board](https://www.microchip.com/en-us/development-tool/dm320117) using Parallel interface to capture video frames from either [OV5640 Camera Board](https://www.waveshare.com/ov5640-camera-board-a.htm) or [OV2640 Camera Board](https://www.waveshare.com/ov2640-camera-board.htm) and USB Flash drive of FAT filesystem |


