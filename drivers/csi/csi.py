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
    csi_drv_menu = component.createMenuSymbol("CSIDriver", None)
    csi_drv_menu.setLabel("CSI Driver Settings")
    csi_drv_menu.setDescription("CSI Driver Settings.")
    
    csi_data_format = component.createKeyValueSetSymbol("CSIDataFormatType", csi_drv_menu)
    csi_data_format.setLabel("CSI Data Format")
    csi_data_format.setDescription("Must match the Output Bit Width selected in csi Driver.")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW8","CSI2_DATA_FORMAT_RAW8","CSI2_DATA_FORMAT_RAW8")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RAW10","CSI2_DATA_FORMAT_RAW10","CSI2_DATA_FORMAT_RAW10")
    csi_data_format.addKey("CSI2_DATA_FORMAT_RGB888","CSI2_DATA_FORMAT_RGB888","CSI2_DATA_FORMAT_RGB888")
    csi_data_format.setOutputMode("Value")
    csi_data_format.setDisplayMode("Description")
    csi_data_format.setDefaultValue(1)
    csi_data_format.setVisible(True)

    csi_n_lanes = component.createKeyValueSetSymbol("CSINumLanes", csi_drv_menu)
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
    
    csi2dc_drv_menu = component.createMenuSymbol("CSI2DriverMenu", None)
    csi2dc_drv_menu.setLabel("CSI2DC Driver Settings")
    csi2dc_drv_menu.setDescription("CSI2DC Driver Settings.")    
    
    csi2dc_bus_type = component.createKeyValueSetSymbol("CSI2DCBusType", csi2dc_drv_menu)
    csi2dc_bus_type.setLabel("CSI Bus Mode")
    csi2dc_bus_type.setDescription("Csi2Dc Bus Type ")
    csi2dc_bus_type.addKey("MIPI CSI-2 serial interface ", "CSI2DC_BUS_CSI2_DPHY", "MIPI CSI-2 serial interface")
    csi2dc_bus_type.addKey("GPIO parallel interface", "CSI2DC_BUS_PARALLEL", "GPIO parallel interface")
    csi2dc_bus_type.setOutputMode("Value")
    csi2dc_bus_type.setDisplayMode("Description")
    csi2dc_bus_type.setDefaultValue(0)
    csi2dc_bus_type.setVisible(True)
    
    csi2dc_mipi_clock = component.createBooleanSymbol("CSI2DCMIPIClock", csi2dc_drv_menu)
    csi2dc_mipi_clock.setLabel("MIPI clock is free-running")
    csi2dc_mipi_clock.setDescription("Indicates the sensor MIPI clock is free-running or clock is gated.")
    csi2dc_mipi_clock.setDefaultValue(False)
    csi2dc_mipi_clock.setVisible(True)
    
    csi2dc_pa = component.createBooleanSymbol("CSI2DCPostAligned", csi2dc_drv_menu)
    csi2dc_pa.setLabel("ISC Post Adjustment")
    csi2dc_pa.setDescription("LSB aligned or MSB aligned")
    csi2dc_pa.setDefaultValue(True)
    
    csi2dc_vp_df = component.createKeyValueSetSymbol("CSI2DCVideoPipeFormatType", csi2dc_drv_menu)
    csi2dc_vp_df.setLabel("CSI2DC Video Pipe Format Type")
    csi2dc_vp_df.setDescription("Must match the Output Bit Width selected in csi2dc Driver.")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8", "CSI2DC_DATA_FORMAT_RAW8")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10", "CSI2DC_DATA_FORMAT_RAW10")
    csi2dc_vp_df.addKey("CSI2DC_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888", "CSI2DC_DATA_FORMAT_RGB888")
    csi2dc_vp_df.setOutputMode("Value")
    csi2dc_vp_df.setDisplayMode("Description")
    csi2dc_vp_df.setDefaultValue(1)
    csi2dc_vp_df.setVisible(True)
    
    csi2dc_vp_CId = component.createIntegerSymbol("CSI2DCVideoPipeChannelId", csi2dc_drv_menu)
    csi2dc_vp_CId.setLabel("Video Pipe Channel Id")
    csi2dc_vp_CId.setDescription("Video Pipe Channel Id")
    csi2dc_vp_CId.setDefaultValue(0)
    csi2dc_vp_CId.setReadOnly(True)
    
    csi2dc_dp_menu = component.createMenuSymbol("CSI2DCDataPipeMenu", csi2dc_drv_menu)
    csi2dc_dp_menu.setLabel("Data Pipe Attributes")
    csi2dc_dp_menu.setDescription("Contains Csi2dc Data Pipe parameters.")
    
    csi2dc_data_pipe = component.createBooleanSymbol("CSI2DCEnableDataPipe", csi2dc_dp_menu)
    csi2dc_data_pipe.setLabel("Enable Data Pipe")
    csi2dc_data_pipe.setDescription("Enable Data Pipe")
    csi2dc_data_pipe.setDefaultValue(False)
    
    csi2dc_dp_CId = component.createIntegerSymbol("CSI2DCDataPipeChannelId", csi2dc_dp_menu)
    csi2dc_dp_CId.setLabel("Data Pipe Channel Id")
    csi2dc_dp_CId.setDescription("Data Pipe Channel Id")
    csi2dc_dp_CId.setDefaultValue(0)
    csi2dc_dp_CId.setReadOnly(True)
    
    csi2dc_dp_df = component.createKeyValueSetSymbol("CSI2DCDataPipeFormatType", csi2dc_dp_menu)
    csi2dc_dp_df.setLabel("CSI2DC Data Pipe Format Type")
    csi2dc_dp_df.setDescription("Must match the Output Bit Width selected in csi2dc Driver.")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8", "CSI2DC_DATA_FORMAT_RAW8")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10", "CSI2DC_DATA_FORMAT_RAW10")
    csi2dc_dp_df.addKey("CSI2DC_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888", "CSI2DC_DATA_FORMAT_RGB888")
    csi2dc_dp_df.setOutputMode("Value")
    csi2dc_dp_df.setDisplayMode("Description")
    csi2dc_dp_df.setDefaultValue(1)
    csi2dc_dp_df.setVisible(True)
    
    csi2dc_dma_menu = component.createMenuSymbol("CSI2DCDMAMenu", csi2dc_dp_menu)
    csi2dc_dma_menu.setLabel("DMA Attributes")
    csi2dc_dma_menu.setDescription("Contains Csi2dc DMA parameters.")
    
    csi2dc_vp_CId = component.createBooleanSymbol("CSI2DCDataPipeEnableDMA", csi2dc_dma_menu)
    csi2dc_vp_CId.setLabel("Data Pipe Enable DMA")
    csi2dc_vp_CId.setDescription("Enable DMA for Data PIPE")
    csi2dc_vp_CId.setDefaultValue(False)
    
    csi2dc_dma_csize = component.createKeyValueSetSymbol("CSI2DCDataPipeDMAChuckSize", csi2dc_dma_menu)
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
    
    csi2dc_dma_count = component.createIntegerSymbol("CSI2DCDMACount", csi2dc_dma_menu)
    csi2dc_dma_count.setLabel("Data Pipe DMA Count")
    csi2dc_dma_count.setDescription("Data Pipe DMA Count")
    csi2dc_dma_count.setDefaultValue(320)
    csi2dc_dma_count.setReadOnly(True)
    
    csi2dc_plib_menu = component.createMenuSymbol("CSI2DCPeripheralLibrary", None)
    csi2dc_plib_menu.setLabel("CSI2DC Peripheral Library Settings")
    csi2dc_plib_menu.setDescription("CSI2DC Peripheral Library Settings.")    
       
    csi2dc_data_format = component.createKeyValueSetSymbol("CSI2DCDataFormatType", csi2dc_plib_menu)
    csi2dc_data_format.setLabel("CSI Data Format")
    csi2dc_data_format.setDescription("Must match the Output Bit Width selected in csi2dc Driver.")
    csi2dc_data_format.addKey("CSI2_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8_val", "CSI2_DATA_FORMAT_RAW8")
    csi2dc_data_format.addKey("CSI2_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10_Val", "CSI2_DATA_FORMAT_RAW10")
    csi2dc_data_format.addKey("CSI2_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888_Val", "CSI2_DATA_FORMAT_RGB888")
    csi2dc_data_format.setOutputMode("Value")
    csi2dc_data_format.setDisplayMode("Description")
    csi2dc_data_format.setDefaultValue(1)
    csi2dc_data_format.setVisible(True)
    
    csi_plib_menu = component.createMenuSymbol("CSIPeripheralLibrary", None)
    csi_plib_menu.setLabel("CSI Peripheral Library Settings")
    csi_plib_menu.setDescription("CSI Peripheral Library Settings.")    
       
    csi_plib_data_format = component.createKeyValueSetSymbol("CSIPlibDataFormatType", csi_plib_menu)
    csi_plib_data_format.setLabel("CSI Data Format")
    csi_plib_data_format.setDescription("Must match the Output Bit Width selected in csi Driver.")
    csi_plib_data_format.addKey("CSI2_DATA_FORMAT_RAW8", "CSI2_DATA_FORMAT_RAW8_val", "CSI2_DATA_FORMAT_RAW8")
    csi_plib_data_format.addKey("CSI2_DATA_FORMAT_RAW10", "CSI2_DATA_FORMAT_RAW10_Val", "CSI2_DATA_FORMAT_RAW10")
    csi_plib_data_format.addKey("CSI2_DATA_FORMAT_RGB888", "CSI2_DATA_FORMAT_RGB888_Val", "CSI2_DATA_FORMAT_RGB888")
    csi_plib_data_format.setOutputMode("Value")
    csi_plib_data_format.setDisplayMode("Description")
    csi_plib_data_format.setDefaultValue(1)
    csi_plib_data_format.setVisible(True)
    
    csi_bit_rate = component.createKeyValueSetSymbol("CSIBitRate", csi_plib_menu)
    csi_bit_rate.setLabel("CSI Bit Rate")
    csi_bit_rate.setDescription("Set CSI Lanes Bit Rate")
    csi_bit_rate.setDefaultValue(22)
    csi_bit_rate.setReadOnly(True)

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
    
    DRV_CSI2DC_C = component.createFileSymbol("DRV_CSI2DC_C", None)
    DRV_CSI2DC_C.setDestPath("vision/drivers/csi/")
    DRV_CSI2DC_C.setOutputName("drv_csi2dc.c")
    DRV_CSI2DC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    DRV_CSI2DC_C.setType("SOURCE")
    DRV_CSI2DC_C.setMarkup(True)
    DRV_CSI2DC_C.setSourcePath("src/drv_csi2dc.c.ftl")
    
    DRV_CSI2DC_H = component.createFileSymbol("DRV_CSI2DC_H", None)
    DRV_CSI2DC_H.setDestPath("vision/drivers/csi/")
    DRV_CSI2DC_H.setOutputName("drv_csi2dc.h")
    DRV_CSI2DC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    DRV_CSI2DC_H.setType("HEADER")
    DRV_CSI2DC_H.setMarkup(True)
    DRV_CSI2DC_H.setSourcePath("inc/drv_csi2dc.h.ftl")
    
    PLIB_CSI_C = component.createFileSymbol("PLIB_CSI_C", None)
    PLIB_CSI_C.setDestPath("vision/drivers/csi/")
    PLIB_CSI_C.setOutputName("plib_csi.c")
    PLIB_CSI_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    PLIB_CSI_C.setType("SOURCE")
    PLIB_CSI_C.setMarkup(True)
    PLIB_CSI_C.setSourcePath("src/plib_csi.c.ftl")
    
    PLIB_CSI_H = component.createFileSymbol("PLIB_CSI_H", None)
    PLIB_CSI_H.setDestPath("vision/drivers/csi/")
    PLIB_CSI_H.setOutputName("plib_csi.h")
    PLIB_CSI_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    PLIB_CSI_H.setType("HEADER")
    PLIB_CSI_H.setMarkup(True)
    PLIB_CSI_H.setSourcePath("inc/plib_csi.h.ftl")
        
    PLIB_CSI2DC_C = component.createFileSymbol("PLIB_CSI2DC_C", None)
    PLIB_CSI2DC_C.setDestPath("vision/drivers/csi/")
    PLIB_CSI2DC_C.setOutputName("plib_csi2dc.c")
    PLIB_CSI2DC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    PLIB_CSI2DC_C.setType("SOURCE")
    PLIB_CSI2DC_C.setMarkup(True)
    PLIB_CSI2DC_C.setSourcePath("src/plib_csi2dc.c.ftl")
    
    PLIB_CSI2DC_H = component.createFileSymbol("PLIB_CSI2DC_H", None)
    PLIB_CSI2DC_H.setDestPath("vision/drivers/csi/")
    PLIB_CSI2DC_H.setOutputName("plib_csi2dc.h")
    PLIB_CSI2DC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/drivers/csi")
    PLIB_CSI2DC_H.setType("HEADER")
    PLIB_CSI2DC_H.setMarkup(True)
    PLIB_CSI2DC_H.setSourcePath("inc/plib_csi2dc.h.ftl")

    DRV_CSI2_CONFIG_H = component.createFileSymbol("DRV_CSI2_CONFIG_H", None)
    DRV_CSI2_CONFIG_H.setType("STRING")
    DRV_CSI2_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_CSI2_CONFIG_H.setSourcePath("templates/csi_defines.ftl")
    DRV_CSI2_CONFIG_H.setMarkup(True)
    
    DRV_CSI2DC_CONFIG_H = component.createFileSymbol("DRV_CSI2DC_CONFIG_H", None)
    DRV_CSI2DC_CONFIG_H.setType("STRING")
    DRV_CSI2DC_CONFIG_H.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    DRV_CSI2DC_CONFIG_H.setSourcePath("templates/csi2dc_defines.ftl")
    DRV_CSI2DC_CONFIG_H.setMarkup(True)
