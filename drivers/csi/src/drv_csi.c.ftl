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

typedef struct
{
    uint16_t MinValue;
    uint16_t MaxValue;
    uint16_t BitRateValue;
} CSI_BitRate;

static const CSI_BitRate CSI_BitRateLookUp[] =
{
    {80, 89,   0x00},
    {90, 99,   0x10},
    {100, 109, 0x20}, //100000
    {110, 129, 0x01}, //000001
    {130, 139, 0x11}, //010001
    {140, 149, 0x21}, //100001
    {150, 169, 0x02}, //000010
    {170, 179, 0x22}, //010010
    {180, 199, 0x12}, //100010
    {200, 219, 0x03}, //000011
    {220, 239, 0x13}, //010011
    {240, 249, 0x23}, //100011
    {250, 269, 0x04}, //000100
    {270, 299, 0x14}, //010100
    {300, 329, 0x05}, //000101
    {330, 359, 0x15}, //010101
    {360, 399, 0x25}, //100101
    {400, 449, 0x06}, //000110
    {450, 499, 0x16}, //010110
    {500, 549, 0x07}, //000111
    {550, 599, 0x17}, //010111
    {600, 649, 0x08}, //001000
    {650, 699, 0x18}, //011000
    {700, 749, 0x09}, //001001
    {750, 799, 0x19}, //011001
    {800, 849, 0x29}, //101001
    {850, 899, 0x39}, //111001
    {900, 949, 0x0A}, //001010
    {950, 999, 0x1A},  //011010
    {1000, 1049, 0x2A},  //101010
    {1050, 1099, 0x3A},  //111010
    {1100, 1149, 0x0B}, //001011
    {1150, 1199, 0x01}, //011011
    {1200, 1249, 0x2B}, //101011
    {1250, 1299, 0x3B}, //111011
    {1300, 1349, 0x0C}, //001100
    {1350, 1399, 0x1C}, //011100
    {1400, 1449, 0x2C}, //101100
    {1450, 1500, 0x3C}, //111100
};

static uint32_t DRV_CSI_GetBitRate(DRV_CSI_OBJ* devObj)
{
    uint8_t bpp = 0;
    uint32_t bitrate = 0;
    uint32_t size = sizeof(CSI_BitRateLookUp) / sizeof(CSI_BitRateLookUp[0]);
    uint32_t i = 0;

    if (devObj == NULL)
    {
        return 0;
    }

    if (devObj->csiDataType == CSI2_DATA_FORMAT_RAW10)
        bpp = 10;
    else if (devObj->csiDataType == CSI2_DATA_FORMAT_RAW8)
        bpp = 8;

    bitrate = 	csiObj.csiFrameWidth * csiObj.csiFrameHeight  * csiObj.csiFps * bpp;
    bitrate /= 1000000;

    printf("\r\n size = %ld bitrate = %ld \r\n", size, bitrate);

    if (bitrate < 90)
    {
        return 0;
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            if ((bitrate >= CSI_BitRateLookUp[i].MinValue) &&
                (bitrate <= CSI_BitRateLookUp[i].MaxValue))
            {
                return CSI_BitRateLookUp[i].BitRateValue;
            }
        }
    }
    return 0;
}

bool DRV_CSI_Configure(DRV_CSI_OBJ* devObj)
{
    if (devObj == NULL)
    {
        return false;
    }

    devObj->csiBitRate = DRV_CSI_GetBitRate(devObj);

    printf("\r\n\t csiBitRate = 0x%x \r\n", devObj->csiBitRate);

    // release CSI2 reset
    CSI_Reset();
    // Enter to shutdown mode
    CSI_Shutdown();
    // Reset CSI D-Phy
    CSI_Reset_DPhy();
    // Initialize DWC_mipi_csi2_host sequence
    CSI_Analog_Init(devObj->csiBitRate, devObj->numLanes); //0x16
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
    csiObj.csiFps = 150;

    csiObj.csiDataId = 0;
    csiObj.csiVirtualChannel = 0;

    return &csiObj;
}

