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
#include "vision/drivers/csi/plib_csi.h"
#include "vision/drivers/image_sensor/drv_image_sensor.h"

#define debug_print(args ...) if (CAMERA_ENABLE_DEBUG) fprintf(stderr, args)

static DRV_CSI_OBJ csiObj;

typedef struct {
    uint16_t MinValue;
    uint16_t MaxValue;
    uint16_t BitRateValue;
} CSI_BitRate;

static const CSI_BitRate CSI_BitRateLookUp[] = {
    {80, 89, 0x00},
    {90, 99, 0x10},
    {100, 109, 0x20},
    {110, 129, 0x01},
    {130, 139, 0x11},
    {140, 149, 0x21},
    {150, 169, 0x02},
    {170, 179, 0x22},
    {180, 199, 0x12},
    {200, 219, 0x03},
    {220, 239, 0x13},
    {240, 249, 0x23},
    {250, 269, 0x04},
    {270, 299, 0x14},
    {300, 329, 0x05},
    {330, 359, 0x15},
    {360, 399, 0x25},
    {400, 449, 0x06},
    {450, 499, 0x16},
    {500, 549, 0x07},
    {550, 599, 0x17},
    {600, 649, 0x08},
    {650, 699, 0x18},
    {700, 749, 0x09},
    {750, 799, 0x19},
    {800, 849, 0x29},
    {850, 899, 0x39},
    {900, 949, 0x0A},
    {950, 999, 0x1A},
    {1000, 1049, 0x2A},
    {1050, 1099, 0x3A},
    {1100, 1149, 0x0B},
    {1150, 1199, 0x1B},
    {1200, 1249, 0x2B},
    {1250, 1299, 0x3B},
    {1300, 1349, 0x0C},
    {1350, 1399, 0x1C},
    {1400, 1449, 0x2C},
    {1450, 1500, 0x3C},
};

static uint8_t DRV_CSI_LookupBitRateRegister(uint32_t bitrateMbps) {
    uint32_t size = sizeof(CSI_BitRateLookUp) / sizeof(CSI_BitRateLookUp[0]);
    uint32_t i;

    if (bitrateMbps < CSI_BitRateLookUp[0].MinValue) {
        debug_print("\r\n Bitrate below minimum, using lowest value\r\n");
        return CSI_BitRateLookUp[0].BitRateValue;
    }

    if (bitrateMbps > CSI_BitRateLookUp[size - 1].MaxValue) {
        debug_print("\r\n Bitrate above maximum, using highest value\r\n");
        return CSI_BitRateLookUp[size - 1].BitRateValue;
    }

    for (i = 0; i < size; i++) {
        if ((bitrateMbps >= CSI_BitRateLookUp[i].MinValue) &&
            (bitrateMbps <= CSI_BitRateLookUp[i].MaxValue)) {
            debug_print("\r\n Found bitrate register value: 0x%02X\r\n",
                        CSI_BitRateLookUp[i].BitRateValue);
            return (uint8_t)CSI_BitRateLookUp[i].BitRateValue;
        }
    }

    return 0;
}

static uint8_t DRV_CSI_GetBppFromDataType(uint8_t dataType) {
    switch (dataType) {
        case CSI2_DATA_FORMAT_RAW6:
            return 6;
        case CSI2_DATA_FORMAT_RAW7:
            return 7;
        case CSI2_DATA_FORMAT_RAW8:
        case CSI2_DATA_FORMAT_YUV420_8:
        case CSI2_DATA_FORMAT_YUV420_8L:
        case CSI2_DATA_FORMAT_YUV420_8C:
        case CSI2_DATA_FORMAT_YUV422_8:
            return 8;
        case CSI2_DATA_FORMAT_RAW10:
        case CSI2_DATA_FORMAT_YUV420_10:
        case CSI2_DATA_FORMAT_YUV420_10C:
        case CSI2_DATA_FORMAT_YUV422_10:
            return 10;
        case CSI2_DATA_FORMAT_RAW12:
        case CSI2_DATA_FORMAT_RGB444:
            return 12;
        case CSI2_DATA_FORMAT_RAW14:
            return 14;
        case CSI2_DATA_FORMAT_RGB555:
        case CSI2_DATA_FORMAT_RGB565:
            return 16;
        case CSI2_DATA_FORMAT_RGB666:
            return 18;
        case CSI2_DATA_FORMAT_RGB888:
            return 24;
        default:
            return 10;
    }
}

static uint8_t DRV_CSI_CalculateBitRate(uint64_t linkFreqMhz, uint8_t numLanes) {
    uint32_t bitrateMbps;

    bitrateMbps = DRV_CSI_LookupBitRateRegister(linkFreqMhz);

    debug_print("\r\n DRV_CSI_CalculateBitRate: link_freq=%llu MHz, lanes=%u, bitrate=%lu Mbps\r\n",
                (unsigned long long)linkFreqMhz,
                numLanes + 1,
                (unsigned long)bitrateMbps);

    return bitrateMbps;
}

bool DRV_CSI_Configure(DRV_CSI_OBJ* devObj) {
    uint8_t csiBitRate;

    if (devObj == NULL) {
        return false;
    }

    csiBitRate = DRV_CSI_LookupBitRateRegister(devObj->linkFreqMHz);

    debug_print("\r\n DRV_CSI_Configure: linkFreq=%lu MHz, lanes=%u, csiBitRate=0x%02X\r\n",
                (unsigned long)devObj->linkFreqMHz,
                devObj->numLanes + 1,
                csiBitRate);

    // release CSI2 reset
    CSI_Reset();
    // Enter to shutdown mode
    CSI_Shutdown();
    // Reset CSI D-Phy
    CSI_Reset_DPhy();
    // Initialize DWC_mipi_csi2_host sequence
    CSI_Analog_Init(csiBitRate, devObj->numLanes);
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

DRV_CSI_OBJ* DRV_CSI_Initalize(DRV_IMAGE_SENSOR_OBJ* iSensor) {
    if (iSensor == NULL) {
        debug_print("\r\n DRV_CSI_Initalize: ERROR - sensor is NULL\r\n");
        return NULL;
    }

    csiObj.numLanes = CSI_NUM_LANES;
    csiObj.csiDataId = 0;
    csiObj.csiVirtualChannel = 0;
    csiObj.csiDataType = CSI_DATA_FORMAT_TYPE;

    if (DRV_ImageSensor_GetLinkFrequency(iSensor, &csiObj.linkFreqMHz) != DRV_IMAGE_SENSOR_SUCCESS) {
        debug_print("\r\n DRV_CSI_Initalize: Failed to get link frequency from %s\r\n",
                    iSensor->name);
        return NULL;
    }

    debug_print("\r\n DRV_CSI_Initalize for %s linkFreq=%lu MHz\r\n",
                iSensor->name,
                (unsigned long)csiObj.linkFreqMHz);

    return &csiObj;
}

