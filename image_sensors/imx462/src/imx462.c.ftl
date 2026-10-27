
#include <stdio.h>
#include "vision/drivers/image_sensor/drv_image_sensor.h"

#define IMX462_SLAVE_ADDRESS      0x1A
#define IMX462_CHIPIDH_ADDRESS    0x3009
#define IMX462_CHIPIDL_ADDRESS    0x300A
#define IMX462_CHIPIDH            0x02
#define IMX462_CHIPIDL            0xF0
#define IMX462_CHIPID_MASK        0xFFFF
#define IMX462_IMAGE_ORIENTATION_REG  0x3007

static const DRV_IMAGE_SENSOR_REG imx462_raw_1080p[] = {
    {0x3003, 0x01},
    {0x3000, 0x01},
    {0x3001, 0x00},
    {0x3002, 0x01},

    {0x3005, 0x00},
    {0x3007, 0x00},
    {0x3009, 0x02},
    {0x300A, 0xF0},
    {0x300F, 0x00},
    {0x3010, 0x21},
    {0x3012, 0x64},
    {0x3013, 0x00},

    {0x3016, 0x09},

    {0x3018, 0x65},
    {0x3019, 0x04},

    {0x301C, 0x30},
    {0x301D, 0x11},

    {0x3046, 0x00},
    {0x304B, 0x0A},

    {0x305C, 0x18},
    {0x305D, 0x03},
    {0x305E, 0x20},
    {0x305F, 0x01},

    {0x3070, 0x02},
    {0x3071, 0x11},

    {0x309B, 0x10},
    {0x309C, 0x22},

    {0x30A2, 0x02},
    {0x30A6, 0x20},
    {0x30A8, 0x20},
    {0x30AA, 0x20},
    {0x30AC, 0x20},
    {0x30B0, 0x43},

    {0x3119, 0x9E},
    {0x311C, 0x1E},
    {0x311E, 0x08},

    {0x3128, 0x05},
    {0x3129, 0x00},
    {0x313D, 0x83},
    {0x3150, 0x03},

    {0x315E, 0x1A},
    {0x3164, 0x1A},

    {0x317C, 0x00},
    {0x317E, 0x00},

    {0x31EC, 0x0E},

    {0x32B8, 0x50},
    {0x32B9, 0x10},
    {0x32BA, 0x00},
    {0x32BB, 0x04},
    {0x32C8, 0x50},
    {0x32C9, 0x10},
    {0x32CA, 0x00},
    {0x32CB, 0x04},

    {0x332C, 0xD3},
    {0x332D, 0x10},
    {0x332E, 0x0D},

    {0x3358, 0x06},
    {0x3359, 0xE1},
    {0x335A, 0x11},

    {0x3360, 0x1E},
    {0x3361, 0x61},
    {0x3362, 0x10},

    {0x33B0, 0x50},
    {0x33B2, 0x1A},
    {0x33B3, 0x04},

    {0x3414, 0x0A},
    {0x3418, 0x49},
    {0x3419, 0x04},

    {0x3444, 0x20},
    {0x3445, 0x25},

    {0x3480, 0x49},

    {0x3000, 0x00},

    {0xFF, 0xFF}
};

static const DRV_IMAGE_SENSOR_REG imx462_raw_720p[] = {
    {0x3003, 0x01},
    {0x3000, 0x01},
    {0x3001, 0x00},
    {0x3002, 0x01},

    {0x3005, 0x00},
    {0x3007, 0x10},
    {0x3009, 0x02},
    {0x300A, 0xF0},
    {0x300F, 0x00},
    {0x3010, 0x21},
    {0x3012, 0x64},
    {0x3013, 0x00},

    {0x3016, 0x09},

    {0x3018, 0xEE},
    {0x3019, 0x02},

    {0x301C, 0xE4},
    {0x301D, 0x0C},

    {0x3046, 0x00},
    {0x304B, 0x0A},

    {0x305C, 0x18},
    {0x305D, 0x03},
    {0x305E, 0x20},
    {0x305F, 0x01},

    {0x3070, 0x02},
    {0x3071, 0x11},

    {0x309B, 0x10},
    {0x309C, 0x22},

    {0x30A2, 0x02},
    {0x30A6, 0x20},
    {0x30A8, 0x20},
    {0x30AA, 0x20},
    {0x30AC, 0x20},
    {0x30B0, 0x43},

    {0x3119, 0x9E},
    {0x311C, 0x1E},
    {0x311E, 0x08},

    {0x3128, 0x05},
    {0x3129, 0x00},
    {0x313D, 0x83},
    {0x3150, 0x03},

    {0x315E, 0x1A},
    {0x3164, 0x1A},

    {0x317C, 0x00},
    {0x317E, 0x00},

    {0x31EC, 0x0E},

    {0x32B8, 0x50},
    {0x32B9, 0x10},
    {0x32BA, 0x00},
    {0x32BB, 0x04},
    {0x32C8, 0x50},
    {0x32C9, 0x10},
    {0x32CA, 0x00},
    {0x32CB, 0x04},

    {0x332C, 0xD3},
    {0x332D, 0x10},
    {0x332E, 0x0D},

    {0x3358, 0x06},
    {0x3359, 0xE1},
    {0x335A, 0x11},

    {0x3360, 0x1E},
    {0x3361, 0x61},
    {0x3362, 0x10},

    {0x33B0, 0x50},
    {0x33B2, 0x1A},
    {0x33B3, 0x04},

    {0x3414, 0x0A},
    {0x3418, 0xD9},
    {0x3419, 0x02},

    {0x3444, 0x20},
    {0x3445, 0x25},

    {0x3480, 0x49},

    {0x3000, 0x00},

    {0xFF, 0xFF}
};

static const DRV_IMAGE_SENSOR_CONFIGS imx462_raw_1080p_config = {
    0,
    DRV_IMAGE_SENSOR_1080P,
    DRV_IMAGE_SENSOR_RAW_BAYER,
    DRV_IMAGE_SENSOR_10_BIT,
    1,
    1920,
    1080,
    imx462_raw_1080p
};

static const DRV_IMAGE_SENSOR_CONFIGS imx462_raw_720p_config = {
    0,
    DRV_IMAGE_SENSOR_720P,
    DRV_IMAGE_SENSOR_RAW_BAYER,
    DRV_IMAGE_SENSOR_10_BIT,
    1,
    1280,
    720,
    imx462_raw_720p
};

#define IMX462_LINK_FREQ_MHZ    445

static uint32_t IMX462_SetFlip(DRV_IMAGE_SENSOR_OBJ* isensor, DRV_IMAGE_SENSOR_FLIP_MODE mode);

static uint32_t IMX462_SetFlip(DRV_IMAGE_SENSOR_OBJ* isensor, DRV_IMAGE_SENSOR_FLIP_MODE mode) {
    uint8_t regVal = 0;
    bool i2cResult = false;

    printf("\r\n IMX462_SetFlip: Enter, mode=%d\r\n", mode);

    if (isensor == NULL) {
        printf("\r\n IMX462_SetFlip: ERROR - isensor is NULL\r\n");
        return DRV_IMAGE_SENSOR_ERROR;
    }

    printf("\r\n IMX462_SetFlip: sensor=%s, devAddr=0x%x, I2CHandle=0x%x\r\n",
           isensor->name, isensor->devAddr, (unsigned int)isensor->drvI2CHandle);

    switch (mode) {
        case DRV_IMAGE_SENSOR_FLIP_NONE:
            regVal = 0x00;
            break;
        case DRV_IMAGE_SENSOR_FLIP_HORIZONTAL:
            regVal = 0x01;
            break;
        case DRV_IMAGE_SENSOR_FLIP_VERTICAL:
            regVal = 0x02;
            break;
        case DRV_IMAGE_SENSOR_FLIP_BOTH:
            regVal = 0x03;
            break;
        default:
            printf("\r\n IMX462_SetFlip: ERROR - invalid mode\r\n");
            return DRV_IMAGE_SENSOR_ERROR;
    }

    printf("\r\n IMX462_SetFlip: Writing reg=0x%04X, val=0x%02X\r\n",
           IMX462_IMAGE_ORIENTATION_REG, regVal);

    i2cResult = DRV_ImageSensor_I2C_WriteReg(isensor, IMX462_IMAGE_ORIENTATION_REG, regVal);

    if (i2cResult == false) {
        printf("\r\n IMX462_SetFlip: ERROR - I2C write failed\r\n");
        return DRV_IMAGE_SENSOR_I2C_ERROR;
    }

    printf("\r\n IMX462_SetFlip: SUCCESS\r\n");
    return DRV_IMAGE_SENSOR_SUCCESS;
}

DRV_IMAGE_SENSOR_OBJ imx462_device = {
    .name = "IMX462",
    .devAddr = IMX462_SLAVE_ADDRESS,
    .i2cInfMode = DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE,
    .chipIdAddrHigh = IMX462_CHIPIDH_ADDRESS,
    .chipIdAddrLow = IMX462_CHIPIDL_ADDRESS,
    .chipIdHigh = IMX462_CHIPIDH,
    .chipIdLow = IMX462_CHIPIDL,
    .chipIdMask = IMX462_CHIPID_MASK,
    .streamStartAddr = 0x3000,
    .streamStartValue = 0x00,
    .streamStopValue = 0x01,
    .flipRegAddr = IMX462_IMAGE_ORIENTATION_REG,
    .flipFunc = IMX462_SetFlip,
    .getFormatFunc = NULL,
    .linkFreqMHz = IMX462_LINK_FREQ_MHZ,
    .iSensorConfigs = {
        &imx462_raw_720p_config,
        &imx462_raw_1080p_config
    },
    .drvI2CHandle = (DRV_HANDLE) NULL
};
