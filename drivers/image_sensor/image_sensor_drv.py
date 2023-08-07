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


def instantiateComponent(component):          
	DRV_IMAGE_SENSOR_C = component.createFileSymbol("DRV_IMAGE_SENSOR_C", None)
	DRV_IMAGE_SENSOR_C.setDestPath("vision/drivers/image_sensor/")
	DRV_IMAGE_SENSOR_C.setOutputName("drv_image_sensor.c")
	DRV_IMAGE_SENSOR_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/image_sensor")
	DRV_IMAGE_SENSOR_C.setType("SOURCE")
	DRV_IMAGE_SENSOR_C.setMarkup(True)
	DRV_IMAGE_SENSOR_C.setSourcePath("src/drv_image_sensor.c.ftl")
	
	DRV_IMAGE_SENSOR_H = component.createFileSymbol("DRV_IMAGE_SENSOR_H", None)
	DRV_IMAGE_SENSOR_H.setDestPath("vision/drivers/image_sensor")
	DRV_IMAGE_SENSOR_H.setOutputName("drv_image_sensor.h")
	DRV_IMAGE_SENSOR_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/image_sensor")
	DRV_IMAGE_SENSOR_H.setType("HEADER")
	DRV_IMAGE_SENSOR_H.setMarkup(True)
	DRV_IMAGE_SENSOR_H.setSourcePath("inc/drv_image_sensor.h.ftl")
	
	I2CIndex = component.createIntegerSymbol("IMAGE_SENSOR_I2C_INDEX", None)
	I2CIndex.setLabel("I2C Driver Index")
	I2CIndex.setDefaultValue(0)
	I2CIndex.setMin(0)
	
	DRV_IMAGE_SENSOR_CONFIG_H = component.createFileSymbol("DRV_IMAGE_SENSOR_CONFIG_H", None)
	DRV_IMAGE_SENSOR_CONFIG_H.setType("STRING")
	DRV_IMAGE_SENSOR_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
	DRV_IMAGE_SENSOR_CONFIG_H.setSourcePath("templates/image_sensor_defines.ftl")
	DRV_IMAGE_SENSOR_CONFIG_H.setMarkup(True)
	
def onAttachmentConnected(source, target):
	print(component)
	
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("IMAGE_SENSOR_I2C_INDEX")
		I2CIndex.setValue(int(target["component"].getID()[-1]), 1)
		I2CIndex.setReadOnly(True)
	
def onAttachmentDisconnected(source, target):
	if source["id"] == "i2c":
		I2CIndex = source["component"].getSymbolByID("IMAGE_SENSOR_I2C_INDEX")
		I2CIndex.setValue(int(-1))
		I2CIndex.setReadOnly(False)
	
