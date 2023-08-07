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
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8_val", "CSI2_DATA_FORMAT_RAW8")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10_Val", "CSI2_DATA_FORMAT_RAW10")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888_Val", "CSI2_DATA_FORMAT_RGB888")
    csi_data_format.setOutputMode("Value")
    csi_data_format.setDisplayMode("Description")
    csi_data_format.setDefaultValue(1)
    csi_data_format.setVisible(True)
    
    csi_bit_rate = component.createKeyValueSetSymbol("CSIBitRate", None)
    csi_bit_rate.setLabel("CSI Bit Rate")
    csi_bit_rate.setDescription("Set CSI Lanes Bit Rate")
    csi_bit_rate.setDefaultValue(22)
    csi_bit_rate.setReadOnly(True)
    
    PLIB_CSI_C = component.createFileSymbol("PLIB_CSI_C", None)
    PLIB_CSI_C.setDestPath("vision/peripheral/csi/")
    PLIB_CSI_C.setOutputName("plib_csi.c")
    PLIB_CSI_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/csi")
    PLIB_CSI_C.setType("SOURCE")
    PLIB_CSI_C.setMarkup(True)
    PLIB_CSI_C.setSourcePath("src/plib_csi.c.ftl")
    
    PLIB_CSI_H = component.createFileSymbol("PLIB_CSI_H", None)
    PLIB_CSI_H.setDestPath("vision/peripheral/csi/")
    PLIB_CSI_H.setOutputName("plib_csi.h")
    PLIB_CSI_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/csi")
    PLIB_CSI_H.setType("HEADER")
    PLIB_CSI_H.setMarkup(True)
    PLIB_CSI_H.setSourcePath("inc/plib_csi.h.ftl")