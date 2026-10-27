#ifndef DRV_IMAGE_SENSOR_H
#define DRV_IMAGE_SENSOR_H

#include "driver/i2c/drv_i2c.h"
#include <stdint.h>
#include <stdbool.h>

#define DRV_IMAGE_SENSOR_MAX_DEVICES    7
#define DRV_IMAGE_SENSOR_MAX_CONFIGS    10

enum {
    DRV_IMAGE_SENSOR_SUCCESS = 0,
    DRV_IMAGE_SENSOR_I2C_ERROR,
    DRV_IMAGE_SENSOR_ERROR
};

enum {
    DRV_IMAGE_SENSOR_RAW_BAYER = 0,
    DRV_IMAGE_SENSOR_YUV_422,
    DRV_IMAGE_SENSOR_RGB,
    DRV_IMAGE_SENSOR_CCIR656,
    DRV_IMAGE_SENSOR_MONO,
    DRV_IMAGE_SENSOR_JPEG,
};

enum {
    DRV_IMAGE_SENSOR_8_BIT = 0,
    DRV_IMAGE_SENSOR_9_BIT,
    DRV_IMAGE_SENSOR_10_BIT,
    DRV_IMAGE_SENSOR_11_BIT,
    DRV_IMAGE_SENSOR_12_BIT,
    DRV_IMAGE_SENSOR_40_BIT
};

enum {
    DRV_IMAGE_SENSOR_QVGA = 0,
    DRV_IMAGE_SENSOR_VGA,
    DRV_IMAGE_SENSOR_WVGA,
    DRV_IMAGE_SENSOR_SVGA,
    DRV_IMAGE_SENSOR_XGA,
    DRV_IMAGE_SENSOR_WXGA,
    DRV_IMAGE_SENSOR_UVGA,
    DRV_IMAGE_SENSOR_720P,
    DRV_IMAGE_SENSOR_1080P,
    DRV_IMAGE_SENSOR_5MP,
    DRV_IMAGE_SENSOR_8MP,
};

/** Image Sensor I2C mode */
enum {
    DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_BYTE = 0,
    DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE,
    DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_2BYTE
};

/** Image Sensor Flip Mode */
typedef enum {
    DRV_IMAGE_SENSOR_FLIP_NONE = 0x00,
    DRV_IMAGE_SENSOR_FLIP_HORIZONTAL = 0x01,
    DRV_IMAGE_SENSOR_FLIP_VERTICAL = 0x02,
    DRV_IMAGE_SENSOR_FLIP_BOTH = 0x03
} DRV_IMAGE_SENSOR_FLIP_MODE;

typedef struct {
    uint16_t regAddr;
    uint16_t regValue;
} DRV_IMAGE_SENSOR_REG;

typedef struct {
    uint8_t sensorType;
    uint8_t sensorResolution;
    uint8_t sensorFormat;
    uint8_t sensorBitWidth;
    uint8_t sensorSupported;
    uint32_t sensorWidth;
    uint32_t sensorHeight;
    const DRV_IMAGE_SENSOR_REG* iSensorRegSetting;
} DRV_IMAGE_SENSOR_CONFIGS;

typedef struct DRV_IMAGE_SENSOR_OBJ_T DRV_IMAGE_SENSOR_OBJ;

typedef uint32_t (*DRV_IMAGE_SENSOR_FLIP_FUNC)(DRV_IMAGE_SENSOR_OBJ* isensor, DRV_IMAGE_SENSOR_FLIP_MODE mode);
typedef uint32_t (*DRV_IMAGE_SENSOR_GET_FORMAT_FUNC)(DRV_IMAGE_SENSOR_OBJ* isensor, uint8_t* format, uint8_t* bitWidth);

struct DRV_IMAGE_SENSOR_OBJ_T {
    const char* name;
    uint8_t devAddr;
    uint8_t i2cInfMode; /* I2C interface mode  */
    uint16_t chipIdAddrHigh;
    uint16_t chipIdAddrLow;
    uint16_t chipIdHigh;
    uint16_t chipIdLow;
    uint16_t chipIdMask; /** Chip Id mask */
    uint16_t streamStartAddr;
    uint8_t streamStartValue;
    uint8_t streamStopValue;
    uint16_t flipRegAddr;
    uint32_t linkFreqMHz;
    DRV_HANDLE drvI2CHandle;
    DRV_I2C_TRANSFER_HANDLE objWriteHandle;
    DRV_I2C_TRANSFER_HANDLE objReadHandle;
    volatile bool drvI2cLock;
    volatile bool drvI2cSuccess;
    const DRV_IMAGE_SENSOR_CONFIGS* iSensorConfigs[DRV_IMAGE_SENSOR_MAX_CONFIGS];
    DRV_IMAGE_SENSOR_FLIP_FUNC flipFunc;
    DRV_IMAGE_SENSOR_GET_FORMAT_FUNC getFormatFunc;
};

extern DRV_IMAGE_SENSOR_OBJ ov2640_device;
extern DRV_IMAGE_SENSOR_OBJ ov5640_device;
extern DRV_IMAGE_SENSOR_OBJ ov5647_device;
extern DRV_IMAGE_SENSOR_OBJ imx219_device;
extern DRV_IMAGE_SENSOR_OBJ imx462_device;
extern DRV_IMAGE_SENSOR_OBJ imx519_device;
extern DRV_IMAGE_SENSOR_OBJ imx708_device;

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Probe(DRV_HANDLE drvI2CHandle, bool detect_auto, uint8_t id);

uint32_t DRV_ImageSensor_Read_ChipId(DRV_IMAGE_SENSOR_OBJ* isensor);

uint32_t DRV_ImageSensor_Configure(DRV_IMAGE_SENSOR_OBJ* iSensor,
        uint8_t resolution,
        uint8_t format);

uint32_t DRV_ImageSensor_GetConfig(DRV_IMAGE_SENSOR_OBJ* isensor,
        uint8_t resolution,
        uint8_t format,
        uint8_t* bits,
        uint32_t* width,
        uint32_t* height);

uint32_t DRV_ImageSensor_Start(DRV_IMAGE_SENSOR_OBJ* iSensor);

uint32_t DRV_ImageSensor_Stop(DRV_IMAGE_SENSOR_OBJ* isensor);

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Init(uint32_t drvI2CIndex, const char* devname);

bool DRV_ImageSensor_I2C_WriteReg(DRV_IMAGE_SENSOR_OBJ* isensor,
        const uint16_t addr,
        const uint32_t val);

bool DRV_ImageSensor_I2C_ReadReg(DRV_IMAGE_SENSOR_OBJ* isensor,
        const uint16_t addr,
        uint8_t* readBuf,
        const uint32_t sz);

uint32_t DRV_ImageSensor_SetFlip(DRV_IMAGE_SENSOR_OBJ* isensor,
        DRV_IMAGE_SENSOR_FLIP_MODE mode);

uint32_t DRV_ImageSensor_GetFlip(DRV_IMAGE_SENSOR_OBJ* isensor,
        DRV_IMAGE_SENSOR_FLIP_MODE* mode);

uint32_t DRV_ImageSensor_GetLinkFrequency(DRV_IMAGE_SENSOR_OBJ* isensor, uint32_t* linkFreqMHz);

uint32_t DRV_ImageSensor_GetFormat(DRV_IMAGE_SENSOR_OBJ* isensor, uint8_t* format, uint8_t* bitWidth);

const char* DRV_ImageSensor_GetFormatString(uint8_t format);

#endif /* ! DRV_IMAGE_SENSOR_H */