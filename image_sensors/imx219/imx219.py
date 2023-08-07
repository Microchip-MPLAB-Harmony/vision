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
    name = component.createStringSymbol("ImageSensorName", None)
    name.setLabel("Image Sensor Name")
    name.setDescription("The Image Sensor name.")
    name.setDefaultValue("IMX219")
    name.setReadOnly(True)
    
    # Configuration Menu
    config_menu = component.createMenuSymbol("imx219Config", None)
    config_menu.setLabel("Configuration")
    config_menu.setDescription("IMX219 Settings.")
    config_menu.setVisible(True)
    
    image_sensor_width = component.createIntegerSymbol("ImageSensorWidth", config_menu)
    image_sensor_width.setLabel("Image Sensor Width")
    image_sensor_width.setDescription("Set the image Capture width in pixels.")
    image_sensor_width.setDefaultValue(640)
    
    image_sensor_height = component.createIntegerSymbol("ImageSensorHeight", config_menu)
    image_sensor_height.setLabel("Image Sensor Height")
    image_sensor_height.setDescription("Set the image capture height in pixels.")
    image_sensor_height.setDefaultValue(480)
    
    image_sensor_output_format = component.createKeyValueSetSymbol("ImageSensorOutputFormat", config_menu)
    image_sensor_output_format.setLabel("Output Format")
    image_sensor_output_format.setDescription("Must match the Output format selected in Sensor Driver.")
    image_sensor_output_format.addKey("RAW_BAYER", "DRV_IMAGE_SENSOR_RAW_BAYER", "RAW_BAYER")
    image_sensor_output_format.addKey("YUV_422", "DRV_IMAGE_SENSOR_YUV_422", "YUV_422")
    image_sensor_output_format.addKey("RGB", "DRV_IMAGE_SENSOR_RGB", "RGB")
    image_sensor_output_format.addKey("CCIR656", "DRV_IMAGE_SENSOR_CCIR656", "CCIR656")
    image_sensor_output_format.addKey("MONO", "DRV_IMAGE_SENSOR_MONO", "MONO")
    image_sensor_output_format.setOutputMode("Value")
    image_sensor_output_format.setDisplayMode("Description")
    image_sensor_output_format.setDefaultValue(0)
    image_sensor_output_format.setVisible(True)
    
    image_sensor_bit_width = component.createKeyValueSetSymbol("ImageSensorBitWidth", config_menu)
    image_sensor_bit_width.setLabel("Bit Width")
    image_sensor_bit_width.setDescription("Must match the Output Bit Width selected in Sensor Driver.")
    image_sensor_bit_width.addKey("BIT_8", "DRV_IMAGE_SENSOR_8_BIT", "BIT_8")
    image_sensor_bit_width.addKey("BIT_9", "DRV_IMAGE_SENSOR_9_BIT", "BIT_9")
    image_sensor_bit_width.addKey("BIT_10", "DRV_IMAGE_SENSOR_10_BIT", "BIT_10")
    image_sensor_bit_width.addKey("BIT_11", "DRV_IMAGE_SENSOR_11_BIT", "BIT_11")
    image_sensor_bit_width.addKey("BIT_12", "DRV_IMAGE_SENSOR_12_BIT", "BIT_12")
    image_sensor_bit_width.addKey("BIT_40", "DRV_IMAGE_SENSOR_40_BIT", "BIT_40")
    image_sensor_bit_width.setOutputMode("Value")
    image_sensor_bit_width.setDisplayMode("Description")
    image_sensor_bit_width.setDefaultValue(2)
    image_sensor_bit_width.setVisible(True)
    
    IMX219_C = component.createFileSymbol("IMX219_C", None)
    IMX219_C.setDestPath("vision/image_sensors/imx219/")
    IMX219_C.setOutputName("imx219.c")
    IMX219_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/image_sensors/imx219")
    IMX219_C.setType("SOURCE")
    IMX219_C.setMarkup(True)
    IMX219_C.setSourcePath("src/imx219.c.ftl")
    
    IMX219_CONFIG_H = component.createFileSymbol("IMX219_CONFIG_H", None)
    IMX219_CONFIG_H.setType("STRING")
    IMX219_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    IMX219_CONFIG_H.setSourcePath("templates/imx219_defines.ftl")
    IMX219_CONFIG_H.setMarkup(True)
