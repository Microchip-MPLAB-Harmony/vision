/*******************************************************************************
	MIPI CSI2 BUS Driver

	Company:
		Microchip Technology Inc.

	File Name:
		drv_csi.c

	Summary:
		MIPI CSI Driver File

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
#include "vision/drivers/csi/drv_csi.h"
#include "vision/peripheral/csi/plib_csi.h"

static DRV_CSI_OBJ csiObj;


//Todo handle interrupt callbacks.
//Todo add more apis.

// Below is BPS calculations.
//Total Pixel size [bit] = 1920*1080*3*8 = 49766400 bits
//        (there are three colors (RGB) per pixel and each color is has 8bit resolution)
//Total raw video throughput [bit / sec] without compression = 
//        Total Pixel Size*Frame Rate =  49766400 bits*60 Hz/Sec  = 2985984000  bits/sec
//Total DSI link throughput [bit / sec] 1:3 video compression = 
//        Total raw video throughput * compression ratio = 2985984000  bits/sec * 1/3 = 995328000 bits/sec
//Total DSI per lane throughputc [bit /sec] = 995328000 / 4 = 248832000 [bits/sec]
//Bandwidth (i.e data clock frequency)[Hz] = 248832000/2
//        ( i.e 2 bits per one clock frequency such as DDR) = 124416000 Hz = 124 MHz

bool DRV_CSI_Configure(DRV_CSI_OBJ* devObj)
{
	if (devObj == NULL)
	{
		return false;
	}
	
	// release CSI2 reset
	CSI_Reset();
	// Enter to shutdown mode
	CSI_Shutdown();
	// Reset CSI D-Phy
	CSI_Reset_DPhy();
	// Initialize DWC_mipi_csi2_host sequence
	CSI_Analog_Init(devObj->csiBitRate); //0x16
	// exit shutdown state
	CSI_Exit_Shutdown();
	// clear PHY from reset
	CSI_Exit_Reset_DPhy();
	// Lanes
	CSI_Configure_Lane(devObj->numLanes);
	// configure CSI2 ID interface mode
	CSI_Configure_DataId(devObj->csiDataId, devObj->csiVirtualChannel, devObj->csiDataType);
	// clear CSI from reset
	CSI_Exit_Reset();
	
	return true;
}

DRV_CSI_OBJ* DRV_CSI_Initalize(void)
{
	csiObj.numLanes = CSI_NUM_LANES;
	csiObj.csiDataType = CSI_DATA_FORMAT_TYPE;
	csiObj.csiBitRate = 0x16;
	csiObj.csiFrameWidth = 640;    
	csiObj.csiFrameHeight = 480;
	csiObj.csiFps = 30;

	csiObj.csiDataId = 0;
	csiObj.csiVirtualChannel = 0;

	return &csiObj;
}

