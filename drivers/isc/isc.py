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
    isc_input_format = component.createKeyValueSetSymbol("ISCInputFormatType", None)
    isc_input_format.setLabel("Input Format")
    isc_input_format.setDescription("Must match the Output format selected in Sensor Device.")
    isc_input_format.addKey("RAW_BAYER", "DRV_IMAGE_SENSOR_RAW_BAYER", "RAW_BAYER")
    isc_input_format.addKey("YUV_422", "DRV_IMAGE_SENSOR_YUV_422", "YUV_422")
    isc_input_format.addKey("RGB", "DRV_IMAGE_SENSOR_RGB", "RGB")
    isc_input_format.addKey("CCIR656", "DRV_IMAGE_SENSOR_CCIR656", "CCIR656")
    isc_input_format.addKey("MONO", "DRV_IMAGE_SENSOR_MONO", "MONO")
    isc_input_format.setOutputMode("Value")
    isc_input_format.setDisplayMode("Description")
    isc_input_format.setDefaultValue(0)
    isc_input_format.setVisible(True)

    isc_input_bits = component.createKeyValueSetSymbol("ISCInputBitWidth", None)
    isc_input_bits.setLabel("Bit Width")
    isc_input_bits.setDescription("Must match the outpur bus width selected in Sensor Device")
    isc_input_bits.addKey("BIT_8", "DRV_IMAGE_SENSOR_8_BIT", "BIT_8")
    isc_input_bits.addKey("BIT_9", "DRV_IMAGE_SENSOR_9_BIT", "BIT_9")
    isc_input_bits.addKey("BIT_10", "DRV_IMAGE_SENSOR_10_BIT", "BIT_10")
    isc_input_bits.addKey("BIT_11", "DRV_IMAGE_SENSOR_11_BIT", "BIT_11")
    isc_input_bits.addKey("BIT_12", "DRV_IMAGE_SENSOR_12_BIT", "BIT_12")
    isc_input_bits.addKey("BIT_40", "DRV_IMAGE_SENSOR_40_BIT", "BIT_40")
    isc_input_bits.setOutputMode("Value")
    isc_input_bits.setDisplayMode("Description")
    isc_input_bits.setDefaultValue(2)
    isc_input_bits.setVisible(True)

    isc_output_format = component.createKeyValueSetSymbol("ISCOutputFormatType", None)
    isc_output_format.setLabel("Output Format")
    isc_output_format.setDescription("Output Format")
    isc_output_format.addKey("RLP_MODE_DAT8", "ISC_RLP_CFG_MODE_DAT8", "RLP_MODE_DAT8")
    isc_output_format.addKey("RLP_MODE_DAT9", "ISC_RLP_CFG_MODE_DAT9", "RLP_MODE_DAT9")
    isc_output_format.addKey("RLP_MODE_DAT10", "ISC_RLP_CFG_MODE_DAT10", "RLP_MODE_DAT10")
    isc_output_format.addKey("RLP_MODE_DAT11", "ISC_RLP_CFG_MODE_DAT11", "RLP_MODE_DAT11")
    isc_output_format.addKey("RLP_MODE_DAT12", "ISC_RLP_CFG_MODE_DAT12", "RLP_MODE_DAT12")
    isc_output_format.addKey("RLP_MODE_DATY8", "ISC_RLP_CFG_MODE_DATY8", "RLP_MODE_DATY8")
    isc_output_format.addKey("RLP_MODE_DATY10", "ISC_RLP_CFG_MODE_DATY10", "RLP_MODE_DATY10")
    isc_output_format.addKey("RLP_MODE_ARGB444", "ISC_RLP_CFG_MODE_ARGB444", "RLP_MODE_ARGB444")
    isc_output_format.addKey("RLP_MODE_ARGB555", "ISC_RLP_CFG_MODE_ARGB555", "RLP_MODE_ARGB555")
    isc_output_format.addKey("RLP_MODE_RGB565", "ISC_RLP_CFG_MODE_RGB565", "RLP_MODE_RGB565")
    isc_output_format.addKey("RLP_MODE_ARGB32", "ISC_RLP_CFG_MODE_ARGB32", "RLP_MODE_ARGB32")
    isc_output_format.addKey("RLP_MODE_YYCC", "ISC_RLP_CFG_MODE_YYCC", "RLP_MODE_YYCC")
    isc_output_format.addKey("RLP_MODE_YYCC_LIMITED", "ISC_RLP_CFG_MODE_YYCC_LIMITED", "RLP_MODE_YYCC_LIMITED")
    isc_output_format.addKey("RLP_MODE_YCYC", "ISC_RLP_CFG_MODE_YCYC", "RLP_MODE_YCYC")
    isc_output_format.addKey("RLP_MODE_YCYC_LIMITED", "ISC_RLP_CFG_MODE_YCYC_LIMITED", "RLP_MODE_YCYC_LIMITED")
    isc_output_format.addKey("RLP_MODE_BYPASS", "ISC_RLP_CFG_MODE_BYPASS", "RLP_MODE_BYPASS")
    isc_output_format.setOutputMode("Value")
    isc_output_format.setDisplayMode("Description")
    isc_output_format.setDefaultValue(10)
    isc_output_format.setVisible(True)    

    isc_output_layout = component.createKeyValueSetSymbol("ISCOutputLayoutType", None)
    isc_output_layout.setLabel("Outpur Layout Format")
    isc_output_layout.setDescription("Outpur Layout Format")
    isc_output_layout.addKey("LAYOUT_PACKED8", "ISC_LAYOUT_PACKED8", "LAYOUT_PACKED8")
    isc_output_layout.addKey("LAYOUT_PACKED16", "ISC_LAYOUT_PACKED16", "LAYOUT_PACKED16")
    isc_output_layout.addKey("LAYOUT_PACKED32", "ISC_LAYOUT_PACKED32", "LAYOUT_PACKED32")
    isc_output_layout.addKey("LAYOUT_YC422SP", "ISC_LAYOUT_YC422SP", "LAYOUT_YC422SP")
    isc_output_layout.addKey("LAYOUT_YC422P", "ISC_LAYOUT_YC422P", "LAYOUT_YC422P")
    isc_output_layout.addKey("LAYOUT_YC420SP", "ISC_LAYOUT_YC420SP", "LAYOUT_YC420SP")
    isc_output_layout.addKey("LAYOUT_YC420P", "ISC_LAYOUT_YC420P", "LAYOUT_YC420P")
    isc_output_layout.addKey("LAYOUT_YUY2", "ISC_LAYOUT_YUY2", "LAYOUT_YUY2")
    isc_output_layout.setOutputMode("Value")
    isc_output_layout.setDisplayMode("Description")
    isc_output_layout.setDefaultValue(2)
    isc_output_layout.setVisible(True)    

    isc_bayer_pattern = component.createKeyValueSetSymbol("ISCBayerPatternType", None)
    isc_bayer_pattern.setLabel("Bayer Pattern Type")
    isc_bayer_pattern.setDescription("Bayer Mode")
    isc_bayer_pattern.addKey("BayerPattern_GRGR", "ISC_CFA_CFG_BAYCFG_GRGR_Val", "BayerPattern_GRGR")
    isc_bayer_pattern.addKey("BayerPattern_RGRG", "ISC_CFA_CFG_BAYCFG_RGRG_Val", "BayerPattern_RGRG")
    isc_bayer_pattern.addKey("BayerPattern_GBGB", "ISC_CFA_CFG_BAYCFG_GBGB_Val", "BayerPattern_GBGB")
    isc_bayer_pattern.addKey("BayerPattern_BGBG", "ISC_CFA_CFG_BAYCFG_BGBG_Val", "BayerPattern_BGBG")
    isc_bayer_pattern.setOutputMode("Value")
    isc_bayer_pattern.setDisplayMode("Description")
    isc_bayer_pattern.setDefaultValue(1)
    isc_bayer_pattern.setVisible(True)    

    isc_gamma = component.createBooleanSymbol("ISCEnableGamma", None)
    isc_gamma.setLabel("Enable Gamma")
    isc_gamma.setDefaultValue(True)

    isc_wb = component.createBooleanSymbol("ISCEnableWhiteBalance", None)
    isc_wb.setLabel("Enable WhiteBalance")
    isc_wb.setDefaultValue(True)

    isc_hg = component.createBooleanSymbol("ISCEnableHistogram", None)
    isc_hg.setLabel("Enable Histogram")
    isc_hg.setDefaultValue(False)

    isc_mipi = component.createBooleanSymbol("ISCEnableMIPI", None)
    isc_mipi.setLabel("Enable MIPI Interface")
    isc_mipi.setDefaultValue(True)

    isc_vm = component.createBooleanSymbol("ISCEnableVideoMode", None)
    isc_vm.setLabel("Continuous Video Mode")
    isc_vm.setDefaultValue(True)

    isc_cbhs = component.createBooleanSymbol("ISCEnableBightnessAndContrast", None)
    isc_cbhs.setLabel("Enable Bightness And Contrast")
    isc_cbhs.setDefaultValue(True)

    isc_pm = component.createBooleanSymbol("ISCEnableProgressiveMode", None)
    isc_pm.setLabel("Enable Progressive Mode")
    isc_pm.setDefaultValue(True)

    DRV_ISC_C = component.createFileSymbol("DRV_ISC_C", None)
    DRV_ISC_C.setDestPath("vision/drivers/isc/")
    DRV_ISC_C.setOutputName("drv_isc.c")
    DRV_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    DRV_ISC_C.setType("SOURCE")
    DRV_ISC_C.setMarkup(True)
    DRV_ISC_C.setSourcePath("src/drv_isc.c.ftl")

    DRV_ISC_H = component.createFileSymbol("DRV_ISC_H", None)
    DRV_ISC_H.setDestPath("vision/drivers/isc/")
    DRV_ISC_H.setOutputName("drv_isc.h")
    DRV_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    DRV_ISC_H.setType("HEADER")
    DRV_ISC_H.setMarkup(True)
    DRV_ISC_H.setSourcePath("inc/drv_isc.h.ftl")

    DRV_ISC_CONFIG = component.createFileSymbol("DRV_ISC_CONFIG_H", None)
    DRV_ISC_CONFIG.setType("STRING")
    DRV_ISC_CONFIG.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_ISC_CONFIG.setSourcePath("templates/isc_defines.ftl")
    DRV_ISC_CONFIG.setMarkup(True)
