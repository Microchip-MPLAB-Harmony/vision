/*******************************************************************************
	CSI2DC Driver

	Company:
		Microchip Technology Inc.

	File Name:
		drv_csi2dc.c

	Summary:
		CSI2DC Driver File

	Description:
	None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "configuration.h"
#include "vision/drivers/csi2dc/drv_csi2dc.h"
#include "vision/peripheral/csi2dc/plib_csi2dc.h"
#include "vision/peripheral/csi/plib_csi.h"
#include "system/int/sys_int.h"

static DRV_CSI2DC_OBJ csi2dcObj;

void CSI2DC_Handler(void)
{
    CSI2DC_Interrupt_Status();
    //Todo handle register callbacks.
}

static void DRV_CSI2DC_GlobalConfig(DRV_CSI2DC_OBJ* devObj)
{
    uint32_t reg = 0;

    if (devObj == NULL)
    {
        return;
    }

    if (devObj->enableMIPIFreeRun)
        reg &= (~CSI2DC_GCFGR_MIPIFRN_1);
    else
        reg |= CSI2DC_GCFGR_MIPIFRN_1;

    if (devObj->busMode == CSI2DC_BUS_PARALLEL)
        reg |= CSI2DC_GCFGR_GPIOSEL_1;

    CSI2DC_Global_Config(reg);
}

static void DRV_CSI2DC_Configure_VideoPipe(DRV_CSI2DC_OBJ* devObj)
{
    if (devObj == NULL)
    {
        return;
    }

    // Video Pipe configuration
    CSI2DC_Configure_VideoPipe(csi2dcObj.videoPipeDataType,
                               csi2dcObj.videoPipeChannelId,
                               csi2dcObj.videoPipeAlign);
    // enable video Pipe
    CSI2DC_Enable_VideoPipe();

    //Interrupt Capture Enable
    CSI2DC_Enable_VideoPipe_Interrupt(CSI2DC_VPIER_CAPTURE_Msk);

    //Clear Interrupt Status Register
    CSI2DC_VideoPipe_Interrupt_Status();
}

static void DRV_CSI2DC_Configure_DataPipe(DRV_CSI2DC_OBJ* devObj)
{
    if (devObj == NULL)
    {
        return;
    }

    if (devObj->enableDataPipe)
    {
        CSI2DC_Configure_DataPipe(csi2dcObj.dataPipeDataType,
                                  csi2dcObj.dataPipeChannelId,
                                  0x400);
        CSI2DC_Enable_DataPipe();

        CSI2DC_Configure_DataPipe_DMA(csi2dcObj.csi2dcDma.count,
                                      csi2dcObj.csi2dcDma.chuckSize,
                                      csi2dcObj.csi2dcDma.enableDMA);
    }
}

bool DRV_CSI2DC_Configure(DRV_CSI2DC_OBJ* devObj)
{
    if (devObj == NULL)
    {
        return false;
    }

    //Disable the CSI2DC interrupt
    SYS_INT_SourceDisable(ID_CSI2DC);

    CSI2DC_Reset();

    // configure global config register
    DRV_CSI2DC_GlobalConfig(devObj);

    DRV_CSI2DC_Configure_VideoPipe(devObj);

    DRV_CSI2DC_Configure_DataPipe(devObj);

    if (devObj->enableDataPipe)
    {
        CSI2DC_Update_Pipe(CSI2DC_PUR_VP_1 | CSI2DC_PUR_DP_1);
    }
    else
    {
        CSI2DC_Update_Pipe(CSI2DC_PUR_VP_1);
    }

    SYS_INT_SourceEnable(ID_CSI2DC);

    return true;
}

DRV_CSI2DC_OBJ* DRV_CSI2DC_Initalize(void)
{
    //ToDo Fill this using MCC configuration variables.
    csi2dcObj.busMode = CSI2DC_BUS_TYPE;
    csi2dcObj.enableMIPIFreeRun = CSI2DC_ENABLE_MIPI_CLOCK_FREE_RUN;
    csi2dcObj.videoPipeDataType = CSI2DC_VIDEO_PIPE_FORMAT_TYPE;
    csi2dcObj.dataPipeDataType = CSI2DC_DATA_PIPE_FORMAT_TYPE;
    csi2dcObj.enableDataPipe = CSI2DC_ENABLE_DATA_PIPE;
    csi2dcObj.videoPipeAlign = CSI2DC_POST_ALIGNED;

    csi2dcObj.videoPipeChannelId = CSI2DC_VIDEO_PIPE_CHANNEL_ID;
    csi2dcObj.dataPipeChannelId = CSI2DC_DATA_PIPE_CHANNEL_ID;

    csi2dcObj.csi2dcDma.enableDMA = CSI2DC_DATA_PIPE_ENABLE_DMA;
    csi2dcObj.csi2dcDma.chuckSize = CSI2DC_DATA_PIPE_DMA_CHUCK_SIZE;
    csi2dcObj.csi2dcDma.count = CSI2DC_DATA_PIPE_DMA_COUNT;

    return &csi2dcObj;
}

