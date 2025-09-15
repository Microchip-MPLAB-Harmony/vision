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
    isc_drv_menu = component.createMenuSymbol("ISCDriver", None)
    isc_drv_menu.setLabel("ISC Driver Settings")
    isc_drv_menu.setDescription("ISC Driver Settings.")
    
    isc_input_format = component.createKeyValueSetSymbol("ISCInputFormatType", isc_drv_menu)
    isc_input_format.setLabel("Input Format")
    isc_input_format.setDescription("Must match the Output format selected in Sensor Device.")
    isc_input_format.addKey("RAW_BAYER", "DRV_IMAGE_SENSOR_RAW_BAYER", "RAW_BAYER")
    isc_input_format.addKey("YUV_422", "DRV_IMAGE_SENSOR_YUV_422", "YUV_422")
    isc_input_format.addKey("RGB", "DRV_IMAGE_SENSOR_RGB", "RGB")
    isc_input_format.addKey("CCIR656", "DRV_IMAGE_SENSOR_CCIR656", "CCIR656")
    isc_input_format.addKey("MONO", "DRV_IMAGE_SENSOR_MONO", "MONO")
    isc_input_format.addKey("JPEG", "DRV_IMAGE_SENSOR_JPEG", "JPEG")
    isc_input_format.setOutputMode("Value")
    isc_input_format.setDisplayMode("Description")
    isc_input_format.setDefaultValue(0)
    isc_input_format.setVisible(True)

    isc_input_bits = component.createKeyValueSetSymbol("ISCInputBitWidth", isc_drv_menu)
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
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_input_bits.setDefaultValue(2)
    else:
        isc_input_bits.setDefaultValue(0)
    isc_input_bits.setVisible(True)

    isc_output_format = component.createKeyValueSetSymbol("ISCOutputFormatType", isc_drv_menu)
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
    isc_output_format.setDefaultValue(9)
    isc_output_format.setVisible(True)

    isc_output_layout = component.createKeyValueSetSymbol("ISCOutputLayoutType", isc_drv_menu)
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
    isc_output_layout.setDefaultValue(1)
    isc_output_layout.setVisible(True)

    isc_bayer_pattern = component.createKeyValueSetSymbol("ISCBayerPatternType", isc_drv_menu)
    isc_bayer_pattern.setLabel("Bayer Pattern Type")
    isc_bayer_pattern.setDescription("Bayer Mode")
    isc_bayer_pattern.addKey("BayerPattern_GRGR", "ISC_CFA_CFG_BAYCFG_GRGR_Val", "BayerPattern_GRGR")
    isc_bayer_pattern.addKey("BayerPattern_RGRG", "ISC_CFA_CFG_BAYCFG_RGRG_Val", "BayerPattern_RGRG")
    isc_bayer_pattern.addKey("BayerPattern_GBGB", "ISC_CFA_CFG_BAYCFG_GBGB_Val", "BayerPattern_GBGB")
    isc_bayer_pattern.addKey("BayerPattern_BGBG", "ISC_CFA_CFG_BAYCFG_BGBG_Val", "BayerPattern_BGBG")
    isc_bayer_pattern.setOutputMode("Value")
    isc_bayer_pattern.setDisplayMode("Description")
    isc_bayer_pattern.setDefaultValue(3)
    isc_bayer_pattern.setVisible(True)

    isc_gamma_menu = component.createMenuSymbol("ISCGammaMenu", isc_drv_menu)
    isc_gamma_menu.setLabel("Gamma Settings")
    isc_gamma_menu.setDescription("ISC Gamma Settings.")

    isc_gamma = component.createBooleanSymbol("ISCEnableGamma", isc_gamma_menu)
    isc_gamma.setLabel("Enable Gamma Correction")
    isc_gamma.setDefaultValue(True)

    isc_gamma_red = component.createBooleanSymbol("ISCGammaRedEntries", isc_gamma_menu)
    isc_gamma_red.setLabel("Enable Gamma Correction for R Channel")
    isc_gamma_red.setDefaultValue(False)

    isc_gamma_green = component.createBooleanSymbol("ISCGammaGreenEntries", isc_gamma_menu)
    isc_gamma_green.setLabel("Enable Gamma Correction for G Channel")
    isc_gamma_green.setDefaultValue(False)

    isc_gamma_blue = component.createBooleanSymbol("ISCGammaBlueEntries", isc_gamma_menu)
    isc_gamma_blue.setLabel("Enable Gamma Correction for B Channel")
    isc_gamma_blue.setDefaultValue(False)

    isc_wb_menu = component.createMenuSymbol("ISCWhiteBalanceMenu", isc_drv_menu)
    isc_wb_menu.setLabel("WhiteBalance Settings")
    isc_wb_menu.setDescription("Contains the ISC White Balance Settings.")

    isc_wb = component.createBooleanSymbol("ISCEnableWhiteBalance", isc_wb_menu)
    isc_wb.setLabel("Enable WhiteBalance")
    isc_wb.setDefaultValue(True)

    isc_wb_R_Offset = component.createIntegerSymbol("ISCWhiteBalance_R_Offset", isc_wb_menu)
    isc_wb_R_Offset.setLabel("ISC White Balance Offset for R")
    isc_wb_R_Offset.setDescription("ISC White Balance R Offset value ")
    isc_wb_R_Offset.setDefaultValue(0)
    isc_wb_R_Offset.setMin(0)

    isc_wb_GR_Offset = component.createIntegerSymbol("ISCWhiteBalance_GR_Offset", isc_wb_menu)
    isc_wb_GR_Offset.setLabel("ISC White Balance Offset for GR")
    isc_wb_GR_Offset.setDescription("ISC White Balance GR Offset value ")
    isc_wb_GR_Offset.setDefaultValue(0)
    isc_wb_GR_Offset.setMin(0)

    isc_wb_B_Offset = component.createIntegerSymbol("ISCWhiteBalance_B_Offset", isc_wb_menu)
    isc_wb_B_Offset.setLabel("ISC White Balance Offset for B")
    isc_wb_B_Offset.setDescription("ISC White Balance B Offset value ")
    isc_wb_B_Offset.setDefaultValue(0)
    isc_wb_B_Offset.setMin(0)

    isc_wb_GB_Offset = component.createIntegerSymbol("ISCWhiteBalance_GB_Offset", isc_wb_menu)
    isc_wb_GB_Offset.setLabel("ISC White Balance Offset for GB")
    isc_wb_GB_Offset.setDescription("ISC White Balance GB Offset value ")
    isc_wb_GB_Offset.setDefaultValue(0)
    isc_wb_GB_Offset.setMin(0)

    isc_wb_R_Gain = component.createIntegerSymbol("ISCWhiteBalance_R_Gain", isc_wb_menu)
    isc_wb_R_Gain.setLabel("ISC White Balance R Gain")
    isc_wb_R_Gain.setDescription("ISC White Balance R Gain value ")
    isc_wb_R_Gain.setDefaultValue(512)
    isc_wb_R_Gain.setMin(0)

    isc_wb_GR_Gain = component.createIntegerSymbol("ISCWhiteBalance_GR_Gain", isc_wb_menu)
    isc_wb_GR_Gain.setLabel("ISC White Balance GR Gain")
    isc_wb_GR_Gain.setDescription("ISC White Balance GR Gain value ")
    isc_wb_GR_Gain.setDefaultValue(512)
    isc_wb_GR_Gain.setMin(0)

    isc_wb_B_Gain = component.createIntegerSymbol("ISCWhiteBalance_B_Gain", isc_wb_menu)
    isc_wb_B_Gain.setLabel("ISC White Balance Offset for B")
    isc_wb_B_Gain.setDescription("ISC White Balance B Offset value ")
    isc_wb_B_Gain.setDefaultValue(512)
    isc_wb_B_Gain.setMin(0)

    isc_wb_GB_Gain = component.createIntegerSymbol("ISCWhiteBalance_GB_Gain", isc_wb_menu)
    isc_wb_GB_Gain.setLabel("ISC White Balance GB Gain")
    isc_wb_GB_Gain.setDescription("ISC White Balance GB Gain value ")
    isc_wb_GB_Gain.setDefaultValue(512)
    isc_wb_GB_Gain.setMin(0)

    isc_hg = component.createBooleanSymbol("ISCEnableHistogram", isc_drv_menu)
    isc_hg.setLabel("Enable Histogram")
    isc_hg.setDefaultValue(False)

    isc_vm = component.createBooleanSymbol("ISCEnableVideoMode", isc_drv_menu)
    isc_vm.setLabel("Continuous Video Mode")
    isc_vm.setDefaultValue(True)

    isc_cbhs_menu = component.createMenuSymbol("ISCBightnessAndContrastMenu", isc_drv_menu)
    isc_cbhs_menu.setLabel("Bightness And Contrast Settings")
    isc_cbhs_menu.setDescription("Contains the ISC White Balance Settings.")

    isc_cbhs = component.createBooleanSymbol("ISCEnableBightnessAndContrast", isc_cbhs_menu)
    isc_cbhs.setLabel("Enable Bightness And Contrast")
    isc_cbhs.setDefaultValue(True)

    isc_brightness = component.createIntegerSymbol("ISCBightness", isc_cbhs_menu)
    isc_brightness.setLabel("Brightness value")
    isc_brightness.setHelp("Image Brightness value (signed 11 bits 1:10:0)")
    isc_brightness.setDefaultValue(5)
    isc_brightness.setVisible(True)

    isc_contrast = component.createIntegerSymbol("ISCContrast", isc_cbhs_menu)
    isc_contrast.setLabel("Contrast value")
    isc_contrast.setHelp("Image Contrast value (unsigned 12 bits 0:4:8)")
    # isc_contrast.setMax(12.0)
    # isc_contrast.setMin(0.0)
    isc_contrast.setDefaultValue(18)
    isc_contrast.setVisible(True)

    isc_pm = component.createBooleanSymbol("ISCEnableProgressiveMode", isc_drv_menu)
    isc_pm.setLabel("Enable Progressive Mode")
    isc_pm.setDefaultValue(True)
    
    isc_plib_menu = component.createMenuSymbol("ISCPeripheralLibrary", None)
    isc_plib_menu.setLabel("ISC Peripheral Library Settings")
    isc_plib_menu.setDescription("ISC Peripheral Library Settings.")
    
    isc_clk_menu = component.createMenuSymbol("PLIB_ISC_ClockMenu", isc_plib_menu)
    isc_clk_menu.setLabel("ISC Clock Configuration")
    isc_clk_menu.setDescription("Contains ISC Clock Configuration Settings.")

    isc_mck_sel = component.createKeyValueSetSymbol("PLIB_ISC_MCK_SEL", isc_clk_menu)
    isc_mck_sel.setLabel("MCK Source Selection")
    isc_mck_sel.setDescription("ISC_MCK selectable clock sources")
    isc_mck_sel.addKey("hclock", "0", "hclock")
    isc_mck_sel.addKey("iscclk", "1", "iscclk")
    isc_mck_sel.addKey("gclk", "2", "gclk")
    isc_mck_sel.setOutputMode("Value")
    isc_mck_sel.setDisplayMode("Description")
    isc_mck_sel.setDefaultValue(0)
    isc_mck_sel.setVisible(True)

    isc_mck_div = component.createIntegerSymbol("PLIB_ISC_MCK_DIV", isc_clk_menu)
    isc_mck_div.setLabel("MCK clock divider")
    isc_mck_div.setDescription("ISC MClock divider value")
    isc_mck_div.setDefaultValue(7)

    isc_isp_clk_sel = component.createKeyValueSetSymbol("PLIB_ISC_ISP_CLK_SEL", isc_clk_menu)
    isc_isp_clk_sel.setLabel("ISP Clock Source Selection")
    isc_isp_clk_sel.setDescription("ISP Clock selectable clock sources")
    isc_isp_clk_sel.addKey("hclock", "0", "hclock")
    isc_isp_clk_sel.addKey("iscclk", "1", "iscclk")
    isc_isp_clk_sel.addKey("gclk", "2", "gclk")
    isc_isp_clk_sel.setOutputMode("Value")
    isc_isp_clk_sel.setDisplayMode("Description")
    isc_isp_clk_sel.setDefaultValue(0)
    isc_isp_clk_sel.setVisible(True)

    isc_isp_clk_div = component.createIntegerSymbol("PLIB_ISC_ISP_CLK_DIV", isc_clk_menu)
    isc_isp_clk_div.setLabel("ISP Clock divider")
    isc_isp_clk_div.setDescription("ISP Clock divider value")
    isc_isp_clk_div.setDefaultValue(2)
    isc_isp_clk_div.setMin(0)
    isc_isp_clk_div.setVisible(True)

    isc_polarity_menu = component.createMenuSymbol("PLIB_ISC_PolarityMenu", isc_plib_menu)
    isc_polarity_menu.setLabel("Polarity Settings")
    isc_polarity_menu.setDescription("Contains the ISC polarity values.")

    isc_hsync_polarity = component.createIntegerSymbol("PLIB_ISC_HSYNC_POLARITY", isc_polarity_menu)
    isc_hsync_polarity.setLabel("HSYNC Polarity")
    isc_hsync_polarity.setDescription("HSYNC signal is active high, i.e. valid pixels are sampled when HSYNC is asserted" 
                             "HSYNC signal is active low, i.e. valid pixels are sampled when HSYNC is deasserted")
    isc_hsync_polarity.setDefaultValue(0)
    isc_hsync_polarity.setMax(1)

    isc_vsync_polarity = component.createIntegerSymbol("PLIB_ISC_VSYNC_POLARITY", isc_polarity_menu)
    isc_vsync_polarity.setLabel("VSYNC Polarity")
    isc_vsync_polarity.setDescription("VSYNC signal is active high, i.e. valid pixels are sampled when VSYNC is asserted" 
                             "VSYNC signal is active low, i.e. valid pixels are sampled when VSYNC is deasserted")
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_vsync_polarity.setDefaultValue(0)
    else:
        isc_vsync_polarity.setDefaultValue(1)
    isc_vsync_polarity.setMin(0)
    isc_vsync_polarity.setMax(1)

    DRV_ISC_C = component.createFileSymbol("DRV_ISC_C", None)
    DRV_ISC_C.setDestPath("vision/drivers/isc/")
    DRV_ISC_C.setOutputName("drv_isc.c")
    DRV_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    DRV_ISC_C.setType("SOURCE")
    DRV_ISC_C.setMarkup(True)
    DRV_ISC_C.setSourcePath("src/drv_isc_sama5d2.c.ftl")

    DRV_ISC_H = component.createFileSymbol("DRV_ISC_H", None)
    DRV_ISC_H.setDestPath("vision/drivers/isc/")
    DRV_ISC_H.setOutputName("drv_isc.h")
    DRV_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    DRV_ISC_H.setType("HEADER")
    DRV_ISC_H.setMarkup(True)
    DRV_ISC_H.setSourcePath("inc/drv_isc_sama5d2.h.ftl")

    PLIB_ISC_C = component.createFileSymbol("PLIB_ISC_C", None)
    PLIB_ISC_C.setDestPath("vision/drivers/isc/")
    PLIB_ISC_C.setOutputName("plib_isc.c")
    PLIB_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    PLIB_ISC_C.setType("SOURCE")
    PLIB_ISC_C.setMarkup(True)
    PLIB_ISC_C.setSourcePath("src/plib_isc.c.ftl")

    PLIB_ISC_H = component.createFileSymbol("PLIB_ISC_H", None)
    PLIB_ISC_H.setDestPath("vision/drivers/isc/")
    PLIB_ISC_H.setOutputName("plib_isc.h")
    PLIB_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/isc")
    PLIB_ISC_H.setType("HEADER")
    PLIB_ISC_H.setMarkup(True)
    PLIB_ISC_H.setSourcePath("inc/plib_isc.h.ftl")

    DRV_ISC_CONFIG = component.createFileSymbol("DRV_ISC_CONFIG_H", None)
    DRV_ISC_CONFIG.setType("STRING")
    DRV_ISC_CONFIG.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_ISC_CONFIG.setSourcePath("templates/isc_defines_sama5d2.ftl")
    DRV_ISC_CONFIG.setMarkup(True)
    
    PLIB_ISC_CONFIG_H = component.createFileSymbol("PLIB_ISC_CONFIG_H", None)
    PLIB_ISC_CONFIG_H.setType("STRING")
    PLIB_ISC_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    PLIB_ISC_CONFIG_H.setSourcePath("templates/plib_isc_defines_sama5d2.ftl")
    PLIB_ISC_CONFIG_H.setMarkup(True)
