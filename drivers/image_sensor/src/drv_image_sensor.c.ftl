#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "peripheral/pio/plib_pio.h"
#include "device.h"
#include "drv_image_sensor.h"
#include "system/debug/sys_debug.h"

#define debug_print(args ...) if (CAMERA_ENABLE_DEBUG) fprintf(stderr, args)

static const DRV_IMAGE_SENSOR_OBJ* Devices[DRV_IMAGE_SENSOR_MAX_DEVICES] ={
    &imx219_device,
    &ov5647_device,
    &ov2640_device,
    &ov5640_device,
};

static void DRV_ImageSensor_I2C_EventHandler(DRV_I2C_TRANSFER_EVENT evt,
        DRV_I2C_TRANSFER_HANDLE hdl,
        uintptr_t context) {
    DRV_IMAGE_SENSOR_OBJ* obj = (DRV_IMAGE_SENSOR_OBJ*) context;

    if (obj == NULL) {
        return;
    }

    /* Checks for valid buffer handle */
    if (hdl == DRV_I2C_TRANSFER_HANDLE_INVALID) {
        return;
    }

    obj->drvI2cLock = false;
    if ((hdl == obj->objReadHandle) || (hdl == obj->objWriteHandle)) {
        if (evt == DRV_I2C_TRANSFER_EVENT_ERROR) {
            return;
        } else if (evt == DRV_I2C_TRANSFER_EVENT_COMPLETE) {
            obj->drvI2cSuccess = true;
        }
    }
}

bool DRV_ImageSensor_I2C_ReadReg(DRV_IMAGE_SENSOR_OBJ* isensor,
        const uint16_t addr,
        uint8_t* readBuf,
        const uint32_t sz) {
    uint8_t addrBuf[2];
    uint32_t timeout = 0;

    if (isensor == NULL) {
        return false;
    }

    isensor->drvI2cLock = true;

    //TOdo Do it only once.
    DRV_I2C_TransferEventHandlerSet(isensor->drvI2CHandle,
            DRV_ImageSensor_I2C_EventHandler,
            (uintptr_t) isensor);

    if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_BYTE) {
        DRV_I2C_WriteReadTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                (uint8_t*) & addr,
                1,
                readBuf,
                sz,
                &isensor->objReadHandle);
    } else if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE) {
        addrBuf[0] = (addr & 0x7F00) >> 8;
        addrBuf[1] = addr & 0xFF;
        DRV_I2C_WriteReadTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                addrBuf,
                2,
                readBuf,
                sz,
                &isensor->objReadHandle);

    } else if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_2BYTE) {
        DRV_I2C_WriteReadTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                (uint8_t*) & addr,
                1,
                readBuf,
                sz,
                &isensor->objReadHandle);
    } else {
        debug_print("\r\n DRV_ImageSensor_I2C_ReadReg: Invalid I2C interface mode \r\n");
        return false;
    }

    while (isensor->drvI2cLock == true) {
        timeout++;
        if (timeout == 100000)
            break;
    }

    return isensor->drvI2cSuccess;

}

bool DRV_ImageSensor_I2C_WriteReg(DRV_IMAGE_SENSOR_OBJ* isensor,
        const uint16_t addr,
        const uint32_t val) {
    uint8_t buf[4];
    uint32_t timeout = 0;

    if (isensor == NULL) {
        return false;
    }

    isensor->drvI2cLock = true;

    DRV_I2C_TransferEventHandlerSet(isensor->drvI2CHandle,
            DRV_ImageSensor_I2C_EventHandler,
            (uintptr_t) isensor);

    if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_BYTE) {
        buf[0] = (addr & 0xFF);
        buf[1] = val & 0xFF;
        DRV_I2C_WriteTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                buf,
                2,
                &isensor->objWriteHandle);
    } else if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE) {
        buf[0] = (addr & 0x7F00) >> 8;
        buf[1] = (addr & 0xFF);
        buf[2] = val & 0xFF;
        DRV_I2C_WriteTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                buf,
                3,
                &isensor->objWriteHandle);
    } else if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_2BYTE) {
        buf[0] = (addr & 0xFF);
        buf[1] = val & 0xFF;
        buf[2] = (val >> 8) & 0xFF;
        DRV_I2C_WriteTransferAdd(isensor->drvI2CHandle,
                isensor->devAddr,
                buf,
                3,
                &isensor->objWriteHandle);
    } else {
        debug_print("\r\n DRV_ImageSensor_I2C_WriteReg: Invalid I2C interface mode \r\n");
        return false;
    }

    while (isensor->drvI2cLock == true) {
        timeout++;
        if (timeout == 100000)
            break;
    }

    return isensor->drvI2cSuccess;
}

uint32_t DRV_ImageSensor_Read_ChipId(DRV_IMAGE_SENSOR_OBJ* isensor) {
    uint8_t buf[2] = {0, 0};
    uint8_t chipIdH = 0;
    uint8_t chipIdL = 0;
    uint16_t id = 0;
    uint32_t rsize = 0;

    if (isensor == NULL) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    if ((isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_BYTE) ||
            (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE)) {
        rsize = 1;
    } else if (isensor->i2cInfMode == DRV_IMAGE_SENSOR_I2C_REG_BYTE_DATA_2BYTE) {
        rsize = 2;
    } else {
        debug_print("\r\n DRV_ImageSensor_Read_ChipId: Invalid I2C interface mode \r\n");
        return DRV_IMAGE_SENSOR_ERROR;
    }

    DRV_ImageSensor_I2C_ReadReg(isensor, isensor->chipIdAddrHigh, buf, rsize);

    chipIdH = buf[0];

    DRV_ImageSensor_I2C_ReadReg(isensor, isensor->chipIdAddrLow, buf, rsize);

    chipIdL |= buf[0];

    id = (chipIdH << 8) | chipIdL;

    debug_print("\r\n chipIdH = 0x%x  chipIdL = 0x%x id = 0x%x\r\n", chipIdH, chipIdL, id);

    if ((id & isensor->chipIdMask) == (((isensor->chipIdHigh << 8) | isensor->chipIdLow) & isensor->chipIdMask))
        return DRV_IMAGE_SENSOR_SUCCESS;

    return DRV_IMAGE_SENSOR_ERROR;
}

static uint32_t DRV_ImageSensor_I2C_WriteRegs(DRV_IMAGE_SENSOR_OBJ* isensor,
        const DRV_IMAGE_SENSOR_REG* reglist) {
    int status;

    if ((isensor == NULL) || (reglist == NULL)) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    const DRV_IMAGE_SENSOR_REG* next = reglist;

    while (!((next->regAddr == 0xFF) && (next->regValue == 0xFF))) {
        status = DRV_ImageSensor_I2C_WriteReg(isensor,
                next->regAddr,
                next->regValue);
        if (status == false)
            return DRV_IMAGE_SENSOR_I2C_ERROR;

        next++;
    }

    return DRV_IMAGE_SENSOR_SUCCESS;
}

uint32_t DRV_ImageSensor_Configure(DRV_IMAGE_SENSOR_OBJ* isensor,
        uint8_t resolution,
        uint8_t format) {
    uint8_t i;
    uint8_t found = 0;

    if (isensor == NULL) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    for (i = 0; i < DRV_IMAGE_SENSOR_MAX_CONFIGS; i++) {
        if (isensor->iSensorConfigs[i]->sensorSupported) {
            if (isensor->iSensorConfigs[i]->sensorResolution == resolution) {
                if (isensor->iSensorConfigs[i]->sensorFormat == format) {
                    found = 1;
                    break;
                }
            }
        }
    }
    if (found == 0) {
        debug_print("\r\n DRV_IMAGE_SENSOR_CONFIGS Not found \r\n");
        return DRV_IMAGE_SENSOR_ERROR;
    }

    return DRV_ImageSensor_I2C_WriteRegs(isensor, isensor->iSensorConfigs[i]->iSensorRegSetting);
}

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Probe(const DRV_HANDLE I2CHandle, bool detect_auto, uint8_t id) {
    uint8_t i;
    DRV_IMAGE_SENSOR_OBJ* isensor;

    if (!detect_auto) {
        if (id >= DRV_IMAGE_SENSOR_MAX_DEVICES) {
            return NULL;
        }

        isensor = (DRV_IMAGE_SENSOR_OBJ*) Devices[id];
        isensor->drvI2CHandle = I2CHandle;
        if (DRV_ImageSensor_Read_ChipId(isensor) == DRV_IMAGE_SENSOR_SUCCESS) {
            return isensor;
        } else {
            return NULL;
        }
    } else {
        for (i = 0; i < DRV_IMAGE_SENSOR_MAX_DEVICES; i++) {
            isensor = (DRV_IMAGE_SENSOR_OBJ*) Devices[i];
            isensor->drvI2CHandle = I2CHandle;
            if (DRV_ImageSensor_Read_ChipId(isensor) == DRV_IMAGE_SENSOR_SUCCESS)
                break;
            else
                isensor = NULL;
        }
        return isensor;
    }
    return NULL;
}

uint32_t DRV_ImageSensor_GetConfig(DRV_IMAGE_SENSOR_OBJ* isensor,
        uint8_t resolution,
        uint8_t format,
        uint8_t* bits,
        uint32_t* width,
        uint32_t* height) {
    uint8_t i;

    if (isensor == NULL) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    for (i = 0; i < DRV_IMAGE_SENSOR_MAX_CONFIGS; i++) {
        if (isensor->iSensorConfigs[i]->sensorSupported &&
                (isensor->iSensorConfigs[i]->sensorResolution == resolution) &&
                (isensor->iSensorConfigs[i]->sensorFormat == format)) {
            *bits = isensor->iSensorConfigs[i]->sensorBitWidth;
            *width = isensor->iSensorConfigs[i]->sensorWidth;
            *height = isensor->iSensorConfigs[i]->sensorHeight;
            return DRV_IMAGE_SENSOR_SUCCESS;
        }
    }
    return DRV_IMAGE_SENSOR_ERROR;
}

uint32_t DRV_ImageSensor_Stop(DRV_IMAGE_SENSOR_OBJ* isensor) {
    bool status;

    if (isensor == NULL) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    status = DRV_ImageSensor_I2C_WriteReg(isensor,
            isensor->streamStartAddr,
            isensor->streamStopValue);
    if (status == false) {
        return DRV_IMAGE_SENSOR_I2C_ERROR;
    }

    return DRV_IMAGE_SENSOR_SUCCESS;
}

uint32_t DRV_ImageSensor_Start(DRV_IMAGE_SENSOR_OBJ* isensor) {
    bool status;

    if (isensor == NULL) {
        return DRV_IMAGE_SENSOR_ERROR;
    }

    status = DRV_ImageSensor_I2C_WriteReg(isensor,
            isensor->streamStartAddr,
            isensor->streamStartValue);
    if (status == false) {
        return DRV_IMAGE_SENSOR_I2C_ERROR;
    }

    return DRV_IMAGE_SENSOR_SUCCESS;
}

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Init(uint32_t drvI2CIndex, const char* devname) {
    DRV_HANDLE I2CHandle;
    DRV_IMAGE_SENSOR_OBJ* sensor;
    if (drvI2CIndex >= 0) {
        I2CHandle = DRV_I2C_Open(drvI2CIndex, DRV_IO_INTENT_READWRITE);
        if (I2CHandle == DRV_HANDLE_INVALID) {
            debug_print("\r\n Camera: I2C Driver Open Failed\r\n");
            return NULL;
        }
    } else {
        debug_print("\r\n Camera: Invalid I2C Driver Index\r\n");
        return NULL;
    }

    sensor = DRV_ImageSensor_Probe(I2CHandle, true, 0);
    if (sensor != NULL) {
        debug_print("\r\n Found %s Image Sensor \r\n", sensor->name);
        return sensor;
    }
    debug_print("\r\n Image Sensor probe failed.\r\n");
    return NULL;
}
