# coding: utf-8
##############################################################################
# Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

def loadModule():
    if any(x in Variables.get("__PROCESSOR") for x in ["SAM9X7", "SAMA7"]):
        print("ISC module loaded to support " + str(Variables.get("__PROCESSOR")))
        component = Module.CreateComponent("vision_driver_isc", "Image Sensor Controller(ISC)", "/Vision/Drivers", "isc.py")
        component.setDisplayType("ISC Driver")
        component.addCapability("vision_driver_isc", "DRV_ISC", False)
        component.addDependency("vision_driver_csi", "DRV_CSI", False)
        component.addDependency("vision_image_sensor_data_bus", "ImageSensor Data Bus", False)
    elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2"]):
        print("ISC module loaded to support " + str(Variables.get("__PROCESSOR")))
        component = Module.CreateComponent("vision_driver_isc", "Image Sensor Controller(ISC)", "/Vision/Drivers", "isc_sama5d2.py")
        component.setDisplayType("ISC Driver")
        component.addCapability("vision_driver_isc", "DRV_ISC", False)
        component.addDependency("vision_image_sensor_data_bus", "ImageSensor Data Bus", False)
    else:
        print("ISC module not loaded.  No support for " + str(Variables.get("__PROCESSOR")))
