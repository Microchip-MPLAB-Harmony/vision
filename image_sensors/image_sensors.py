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
    image_sensor_type = component.createKeyValueSetSymbol("ImageSensorName", None)
    image_sensor_type.setLabel("Image Sensor")
    image_sensor_type.setDescription("Setect image sensor or Select Auto Probe to detect automatically connect sensor to the board")
    image_sensor_type.addKey("AutoDectect", "AutoDectect", "AutoDectect")
    image_sensor_type.addKey("OV2640", "OV2640", "OV2640")
    image_sensor_type.addKey("OV5640", "OV5640", "OV5640")
    image_sensor_type.addKey("IMX219", "IMX219", "IMX219")
    image_sensor_type.setOutputMode("Value")
    image_sensor_type.setDisplayMode("Description")
    image_sensor_type.setDefaultValue(0)
    image_sensor_type.setVisible(True)
    
    # Configuration Menu
    config_menu = component.createMenuSymbol("ImageSensorConfig", None)
    config_menu.setLabel("Configuration")
    config_menu.setDescription("ImageSensor Settings.")
    config_menu.setVisible(True)
    
    image_sensor_output_resolution = component.createKeyValueSetSymbol("ImageSensorResolution", config_menu)
    image_sensor_output_resolution.setLabel("Resolution")
    image_sensor_output_resolution.setDescription("Setect output resolution of image sensor")
    image_sensor_output_resolution.addKey("QVGA", "DRV_IMAGE_SENSOR_QVGA", "QVGA")
    image_sensor_output_resolution.addKey("VGA", "DRV_IMAGE_SENSOR_VGA", "VGA")
    image_sensor_output_resolution.addKey("WVGA", "DRV_IMAGE_SENSOR_WVGA", "WVGA")
    image_sensor_output_resolution.addKey("720p", "DRV_IMAGE_SENSOR_720P", "720p")
    image_sensor_output_resolution.addKey("1080p", "DRV_IMAGE_SENSOR_1080P", "1080p")
    image_sensor_output_resolution.addKey("5MP", "DRV_IMAGE_SENSOR_5MP", "5MP")
    image_sensor_output_resolution.addKey("8MP", "DRV_IMAGE_SENSOR_8MP", "8MP")
    image_sensor_output_resolution.setOutputMode("Value")
    image_sensor_output_resolution.setDisplayMode("Description")
    image_sensor_output_resolution.setDefaultValue(1)
    image_sensor_output_resolution.setVisible(True)
    
    image_sensor_output_format = component.createKeyValueSetSymbol("ImageSensorOutputFormat", config_menu)
    image_sensor_output_format.setLabel("Output Format")
    image_sensor_output_format.setDescription("Must match the Output format selected in Sensor Driver.")
    image_sensor_output_format.addKey("RAW_BAYER", "DRV_IMAGE_SENSOR_RAW_BAYER", "RAW_BAYER")
    image_sensor_output_format.addKey("YUV_422", "DRV_IMAGE_SENSOR_YUV_422", "YUV_422")
    image_sensor_output_format.addKey("RGB", "DRV_IMAGE_SENSOR_RGB", "RGB")
    image_sensor_output_format.addKey("CCIR656", "DRV_IMAGE_SENSOR_CCIR656", "CCIR656")
    image_sensor_output_format.addKey("MONO", "DRV_IMAGE_SENSOR_MONO", "MONO")
    image_sensor_output_format.addKey("JPEG", "DRV_IMAGE_SENSOR_JPEG", "JPEG")
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
    
    OV2640_C = component.createFileSymbol("OV2640_C", None)
    OV2640_C.setDestPath("vision/image_sensors/ov2640/")
    OV2640_C.setOutputName("ov2640.c")
    OV2640_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/image_sensors/ov2640")
    OV2640_C.setType("SOURCE")
    OV2640_C.setMarkup(True)
    OV2640_C.setSourcePath("ov2640/src/ov2640.c.ftl")
    
    OV5640_C = component.createFileSymbol("OV5640_C", None)
    OV5640_C.setDestPath("vision/image_sensors/ov5640/")
    OV5640_C.setOutputName("ov5640.c")
    OV5640_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/image_sensors/ov5640")
    OV5640_C.setType("SOURCE")
    OV5640_C.setMarkup(True)
    OV5640_C.setSourcePath("ov5640/src/ov5640.c.ftl")
    
    IMX219_C = component.createFileSymbol("IMX219_C", None)
    IMX219_C.setDestPath("vision/image_sensors/imx219/")
    IMX219_C.setOutputName("imx219.c")
    IMX219_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/image_sensors/imx219")
    IMX219_C.setType("SOURCE")
    IMX219_C.setMarkup(True)
    IMX219_C.setSourcePath("imx219/src/imx219.c.ftl")
    
    IMAGE_SENSOR_CONFIG_H = component.createFileSymbol("IMAGE_SENSOR_CONFIG_H", None)
    IMAGE_SENSOR_CONFIG_H.setType("STRING")
    IMAGE_SENSOR_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    IMAGE_SENSOR_CONFIG_H.setSourcePath("templates/image_sensors_defines.ftl")
    IMAGE_SENSOR_CONFIG_H.setMarkup(True)
