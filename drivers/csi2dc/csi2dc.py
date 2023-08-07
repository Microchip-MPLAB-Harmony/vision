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
    csi2dc_bus_type = component.createKeyValueSetSymbol("CSI2DCBusType", None)
    csi2dc_bus_type.setLabel("CSI Bus Mode")
    csi2dc_bus_type.setDescription("Csi2Dc Bus Type ")
    csi2dc_bus_type.addKey("MIPI CSI-2 serial interface ", "CSI2DC_BUS_CSI2_DPHY", "MIPI CSI-2 serial interface")
    csi2dc_bus_type.addKey("GPIO parallel interface", "CSI2DC_BUS_PARALLEL", "GPIO parallel interface")
    csi2dc_bus_type.setOutputMode("Value")
    csi2dc_bus_type.setDisplayMode("Description")
    csi2dc_bus_type.setDefaultValue(0)
    csi2dc_bus_type.setVisible(True)
    
    csi2dc_mipi_clock = component.createBooleanSymbol("CSI2DCMIPIClock", None)
    csi2dc_mipi_clock.setLabel("MIPI clock is free-running")
    csi2dc_mipi_clock.setDescription("Indicates the sensor MIPI clock is free-running or clock is gated.")
    csi2dc_mipi_clock.setDefaultValue(True)
    csi2dc_mipi_clock.setVisible(True)
    
    csi2dc_pa = component.createBooleanSymbol("CSI2DCPostAligned", None)
    csi2dc_pa.setLabel("ISC Post Adjustment")
    csi2dc_pa.setDescription("LSB aligned or MSB aligned")
    csi2dc_pa.setDefaultValue(True)
    
    csi2dc_vp_df = component.createKeyValueSetSymbol("CSI2DCVideoPipeFormatType", None)
    csi2dc_vp_df.setLabel("CSI2DC Video Pipe Format Type")
    csi2dc_vp_df.setDescription("Must match the Output Bit Width selected in csi2dc Driver.")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8", "CSI2DC_DATA_FORMAT_RAW8")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10", "CSI2DC_DATA_FORMAT_RAW10")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888", "CSI2DC_DATA_FORMAT_RGB888")
    csi2dc_vp_df.setOutputMode("Value")
    csi2dc_vp_df.setDisplayMode("Description")
    csi2dc_vp_df.setDefaultValue(1)
    csi2dc_vp_df.setVisible(True)
    
    csi2dc_vp_CId = component.createIntegerSymbol("CSI2DCVideoPipeChannelId", None)
    csi2dc_vp_CId.setLabel("Video Pipe Channel Id")
    csi2dc_vp_CId.setDescription("Video Pipe Channel Id")
    csi2dc_vp_CId.setDefaultValue(0)
    csi2dc_vp_CId.setReadOnly(True)
    
    dp_menu = component.createMenuSymbol("CSI2DCDataPipeMenu", None)
    dp_menu.setLabel("Data Pipe Attributes")
    dp_menu.setDescription("Contains Csi2dc Data Pipe parameters.")
    
    csi2dc_data_pipe = component.createBooleanSymbol("CSI2DCEnableDataPipe", dp_menu)
    csi2dc_data_pipe.setLabel("Enable Data Pipe")
    csi2dc_data_pipe.setDescription("Enable Data Pipe")
    csi2dc_data_pipe.setDefaultValue(False)
    
    csi2dc_dp_CId = component.createIntegerSymbol("CSI2DCDataPipeChannelId", dp_menu)
    csi2dc_dp_CId.setLabel("Data Pipe Channel Id")
    csi2dc_dp_CId.setDescription("Data Pipe Channel Id")
    csi2dc_dp_CId.setDefaultValue(0)
    csi2dc_dp_CId.setReadOnly(True)
    
    csi2dc_dp_df = component.createKeyValueSetSymbol("CSI2DCDataPipeFormatType", dp_menu)
    csi2dc_dp_df.setLabel("CSI2DC Data Pipe Format Type")
    csi2dc_dp_df.setDescription("Must match the Output Bit Width selected in csi2dc Driver.")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8", "CSI2DC_DATA_FORMAT_RAW8")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10", "CSI2DC_DATA_FORMAT_RAW10")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888", "CSI2DC_DATA_FORMAT_RGB888")
    csi2dc_dp_df.setOutputMode("Value")
    csi2dc_dp_df.setDisplayMode("Description")
    csi2dc_dp_df.setDefaultValue(1)
    csi2dc_dp_df.setVisible(True)
    
    dma_menu = component.createMenuSymbol("CSI2DCDMAMenu", dp_menu)
    dma_menu.setLabel("DMA Attributes")
    dma_menu.setDescription("Contains Csi2dc DMA parameters.")
    
    csi2dc_vp_CId = component.createBooleanSymbol("CSI2DCDataPipeEnableDMA", dma_menu)
    csi2dc_vp_CId.setLabel("Data Pipe Enable DMA")
    csi2dc_vp_CId.setDescription("Enable DMA for Data PIPE")
    csi2dc_vp_CId.setDefaultValue(False)
    
    csi2dc_dma_csize = component.createKeyValueSetSymbol("CSI2DCDataPipeDMAChuckSize", dma_menu)
    csi2dc_dma_csize.setLabel("CSI2DC Data Pipe DMA chuck Size")
    csi2dc_dma_csize.setDescription("CSI2DC Data Pipe DMA chuck Size")
    csi2dc_dma_csize.addKey("CSI2DC_DMA_CHUCK_SIZE_1", "CSI2DC_DMA_CHUCK_SIZE_1", "CSI2DC_DMA_CHUCK_SIZE_1")
    csi2dc_dma_csize.addKey("CSI2DC_DMA_CHUCK_SIZE_2", "CSI2DC_DMA_CHUCK_SIZE_2", "CSI2DC_DMA_CHUCK_SIZE_2")
    csi2dc_dma_csize.addKey("CSI2DC_DMA_CHUCK_SIZE_4", "CSI2DC_DMA_CHUCK_SIZE_4", "CSI2DC_DMA_CHUCK_SIZE_4")
    csi2dc_dma_csize.addKey("CSI2DC_DMA_CHUCK_SIZE_8", "CSI2DC_DMA_CHUCK_SIZE_8", "CSI2DC_DMA_CHUCK_SIZE_8")
    csi2dc_dma_csize.addKey("CSI2DC_DMA_CHUCK_SIZE_16", "CSI2DC_DMA_CHUCK_SIZE_16", "CSI2DC_DMA_CHUCK_SIZE_16")
    csi2dc_dma_csize.setOutputMode("Value")
    csi2dc_dma_csize.setDisplayMode("Description")
    csi2dc_dma_csize.setDefaultValue(4)
    csi2dc_dma_csize.setVisible(True)
    
    csi2dc_dma_count = component.createIntegerSymbol("CSI2DCDMACount", dma_menu)
    csi2dc_dma_count.setLabel("Data Pipe DMA Count")
    csi2dc_dma_count.setDescription("Data Pipe DMA Count")
    csi2dc_dma_count.setDefaultValue(320)
    csi2dc_dma_count.setReadOnly(True)
    
    DRV_CSI2DC_C = component.createFileSymbol("DRV_CSI2DC_C", None)
    DRV_CSI2DC_C.setDestPath("vision/drivers/csi2dc/")
    DRV_CSI2DC_C.setOutputName("drv_csi2dc.c")
    DRV_CSI2DC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi2dc")
    DRV_CSI2DC_C.setType("SOURCE")
    DRV_CSI2DC_C.setMarkup(True)
    DRV_CSI2DC_C.setSourcePath("src/drv_csi2dc.c.ftl")
    
    DRV_CSI2DC_H = component.createFileSymbol("DRV_CSI2DC_H", None)
    DRV_CSI2DC_H.setDestPath("vision/drivers/csi2dc/")
    DRV_CSI2DC_H.setOutputName("drv_csi2dc.h")
    DRV_CSI2DC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi2dc")
    DRV_CSI2DC_H.setType("HEADER")
    DRV_CSI2DC_H.setMarkup(True)
    DRV_CSI2DC_H.setSourcePath("inc/drv_csi2dc.h.ftl")
    
    DRV_CSI2DC_CONFIG_H = component.createFileSymbol("DRV_CSI2DC_CONFIG_H", None)
    DRV_CSI2DC_CONFIG_H.setType("STRING")
    DRV_CSI2DC_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_CSI2DC_CONFIG_H.setSourcePath("templates/csi2dc_defines.ftl")
    DRV_CSI2DC_CONFIG_H.setMarkup(True)
