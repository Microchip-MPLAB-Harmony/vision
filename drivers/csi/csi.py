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
    csi_data_format = component.createKeyValueSetSymbol("CSIDataFormatType", None)
    csi_data_format.setLabel("CSI Data Format")
    csi_data_format.setDescription("Must match the Output Bit Width selected in csi Driver.")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW8","CSI2_DATA_FORMAT_RAW8","CSI2_DATA_FORMAT_RAW8")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW10","CSI2_DATA_FORMAT_RAW10","CSI2_DATA_FORMAT_RAW10")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RGB888","CSI2_DATA_FORMAT_RGB888","CSI2_DATA_FORMAT_RGB888")
    csi_data_format.setOutputMode("Value")
    csi_data_format.setDisplayMode("Description")
    csi_data_format.setDefaultValue(1)
    csi_data_format.setVisible(True)

    csi_n_lanes = component.createKeyValueSetSymbol("CSINumLanes", None)
    csi_n_lanes.setLabel("CSI N Lanes")
    csi_n_lanes.setDescription("CSI Number of data Lines")
    csi_n_lanes.addKey("CSI_DATA_LANES_1","CSI_DATA_LANES_1","CSI_DATA_LANES_1")
    csi_n_lanes.addKey("CSI_DATA_LANES_2","CSI_DATA_LANES_2","CSI_DATA_LANES_2")
    csi_n_lanes.addKey("CSI_DATA_LANES_3","CSI_DATA_LANES_3","CSI_DATA_LANES_3")
    csi_n_lanes.addKey("CSI_DATA_LANES_4","CSI_DATA_LANES_4","CSI_DATA_LANES_4")
    csi_n_lanes.setOutputMode("Value")
    csi_n_lanes.setDisplayMode("Description")
    csi_n_lanes.setDefaultValue(1)
    csi_n_lanes.setVisible(True)

    DRV_CSI2_C = component.createFileSymbol("DRV_CSI2_C", None)
    DRV_CSI2_C.setDestPath("vision/drivers/csi/")
    DRV_CSI2_C.setOutputName("drv_csi.c")
    DRV_CSI2_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    DRV_CSI2_C.setType("SOURCE")
    DRV_CSI2_C.setMarkup(True)
    DRV_CSI2_C.setSourcePath("src/drv_csi.c.ftl")

    DRV_CSI2_H = component.createFileSymbol("DRV_CSI2_H", None)
    DRV_CSI2_H.setDestPath("vision/drivers/csi/")
    DRV_CSI2_H.setOutputName("drv_csi.h")
    DRV_CSI2_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    DRV_CSI2_H.setType("HEADER")
    DRV_CSI2_H.setMarkup(True)
    DRV_CSI2_H.setSourcePath("inc/drv_csi.h.ftl")

    DRV_CSI2_CONFIG_H = component.createFileSymbol("DRV_CSI2_CONFIG_H", None)
    DRV_CSI2_CONFIG_H.setType("STRING")
    DRV_CSI2_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_CSI2_CONFIG_H.setSourcePath("templates/csi_defines.ftl")
    DRV_CSI2_CONFIG_H.setMarkup(True)
