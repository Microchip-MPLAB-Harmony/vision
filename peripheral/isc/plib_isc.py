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
    PLIB_ISC_C = component.createFileSymbol("PLIB_ISC_C", None)
    PLIB_ISC_C.setDestPath("vision/peripheral/isc/")
    PLIB_ISC_C.setOutputName("plib_isc.c")
    PLIB_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/isc")
    PLIB_ISC_C.setType("SOURCE")
    PLIB_ISC_C.setMarkup(True)
    PLIB_ISC_C.setSourcePath("src/plib_isc.c.ftl")
    
    PLIB_ISC_H = component.createFileSymbol("PLIB_ISC_H", None)
    PLIB_ISC_H.setDestPath("vision/peripheral/isc/")
    PLIB_ISC_H.setOutputName("plib_isc.h")
    PLIB_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/isc")
    PLIB_ISC_H.setType("HEADER")
    PLIB_ISC_H.setMarkup(True)
    PLIB_ISC_H.setSourcePath("inc/plib_isc.h.ftl")

    isc_clk_menu = component.createMenuSymbol("PLIB_ISC_ClockMenu", None)
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
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_mck_sel.setVisible(False)
    else:
        isc_mck_sel.setVisible(True)

    isc_mck_div = component.createIntegerSymbol("PLIB_ISC_MCK_DIV", isc_clk_menu)
    isc_mck_div.setLabel("MCK clock divider")
    isc_mck_div.setDescription("ISC MClock divider value")
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_mck_div.setDefaultValue(0)
    else:
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
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_isp_clk_sel.setVisible(False)
    else:
        isc_isp_clk_sel.setVisible(True)

    isc_isp_clk_div = component.createIntegerSymbol("PLIB_ISC_ISP_CLK_DIV", isc_clk_menu)
    isc_isp_clk_div.setLabel("ISP Clock divider")
    isc_isp_clk_div.setDescription("ISP Clock divider value")
    isc_isp_clk_div.setDefaultValue(2)
    isc_isp_clk_div.setMin(0)
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        isc_isp_clk_div.setVisible(False)
    else:
        isc_isp_clk_div.setVisible(True)

    isc_polarity_menu = component.createMenuSymbol("PLIB_ISC_PolarityMenu", None)
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

    PLIB_ISC_CONFIG_H = component.createFileSymbol("PLIB_ISC_CONFIG_H", None)
    PLIB_ISC_CONFIG_H.setType("STRING")
    PLIB_ISC_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    PLIB_ISC_CONFIG_H.setSourcePath("templates/plib_isc_defines.ftl")
    PLIB_ISC_CONFIG_H.setMarkup(True)
