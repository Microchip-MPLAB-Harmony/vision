#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "peripheral/pio/plib_pio.h"
#include "device.h"
#include "drv_image_sensor.h"
#include "system/debug/sys_debug.h"

static const DRV_IMAGE_SENSOR_OBJ* Devices[DRV_IMAGE_SENSOR_MAX_DEVICES] = {
	&imx219_device,
};


static void DRV_ImageSensor_I2C_EventHandler(DRV_I2C_TRANSFER_EVENT evt,
												DRV_I2C_TRANSFER_HANDLE hdl,
												uintptr_t context)
{
	DRV_IMAGE_SENSOR_OBJ* obj = (DRV_IMAGE_SENSOR_OBJ*)context;

	if (obj == NULL)
	{
		return;
	}

	/* Checks for valid buffer handle */
	if(hdl == DRV_I2C_TRANSFER_HANDLE_INVALID)
	{
		return;
	}

	if(evt == DRV_I2C_TRANSFER_EVENT_ERROR)
	{
		return;
	}
	else if((hdl == obj->objReadHandle || hdl == obj->objWriteHandle) &&
				evt == DRV_I2C_TRANSFER_EVENT_COMPLETE)
	{
		obj->drvI2cSuccess = true;        
	}
	obj->drvI2cLock = false;
}

bool DRV_ImageSensor_I2C_ReadReg(DRV_IMAGE_SENSOR_OBJ* isensor,
									const uint16_t addr,
									uint8_t* readBuf,
									const uint32_t sz)
{
	uint8_t addrBuf[2];
	uint32_t timeout = 0;

	if (isensor == NULL)
	{
		return false;
	}

	addrBuf[0] = (addr & 0x7F00) >> 8;
	addrBuf[1] = addr & 0xFF;

	isensor->drvI2cLock = true;

	DRV_I2C_TransferEventHandlerSet(isensor->drvI2CHandle,
										DRV_ImageSensor_I2C_EventHandler,
										(uintptr_t)isensor);

	DRV_I2C_WriteReadTransferAdd(isensor->drvI2CHandle, 
									isensor->devAddr,
									addrBuf,
									2,
									readBuf,
									sz,
									&isensor->objReadHandle);

	while(isensor->drvI2cLock == true)
	{
		timeout++;        
		if(timeout == 100000)
			break;
	}

	return isensor->drvI2cSuccess;

}

bool DRV_ImageSensor_I2C_WriteReg(DRV_IMAGE_SENSOR_OBJ* isensor,
										const uint16_t addr,
										const uint32_t val)
{
	uint8_t buf[4];
	uint32_t timeout = 0;

	if (isensor == NULL)
	{
		return false;
	}

	buf[0] = (addr & 0x7F00) >> 8;
	buf[1] = (addr & 0xFF);
	buf[2] = val & 0xFF;
	buf[3] = (val>>8) & 0xFF;

	isensor->drvI2cLock = true;

	DRV_I2C_TransferEventHandlerSet(isensor->drvI2CHandle, 
										DRV_ImageSensor_I2C_EventHandler,
										(uintptr_t)isensor);
	if(buf[3] != 0)
	{
		DRV_I2C_WriteTransferAdd(isensor->drvI2CHandle, 
									isensor->devAddr,
									buf,
									4,
									&isensor->objWriteHandle);
	}
	else 
	{
		DRV_I2C_WriteTransferAdd(isensor->drvI2CHandle, 
									isensor->devAddr,
									buf,
									3,
									&isensor->objWriteHandle);   
	}

	while(isensor->drvI2cLock == true)
	{
		timeout++;        
		if(timeout == 100000)
			break;
	}

	return isensor->drvI2cSuccess;
}

uint32_t DRV_ImageSensor_Read_ChipId(DRV_IMAGE_SENSOR_OBJ* isensor)
{
	uint8_t buf[2] = {0, 0};
	uint16_t id;

	if (isensor == NULL)
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	DRV_ImageSensor_I2C_ReadReg(isensor, isensor->chipIdAddr, buf, 2);

	id = buf[1];
	id |= buf[0] << 8;
	
	if(id != isensor->chipId)
		return DRV_IMAGE_SENSOR_ERROR;
	
	return DRV_IMAGE_SENSOR_SUCCESS;
}

static uint32_t DRV_ImageSensor_I2C_WriteRegs(DRV_IMAGE_SENSOR_OBJ* isensor,
									  const DRV_IMAGE_SENSOR_REG* reglist)
{
	int status;

	if((isensor == NULL) || (reglist == NULL))
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	const DRV_IMAGE_SENSOR_REG *next = reglist;

	while (!((next->regAddr == 0xFF) && (next->regValue == 0xFF)))
	{
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
						uint8_t format)
{
	uint8_t i;
	uint8_t found = 0;

	if (isensor == NULL)
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	for (i = 0; i < DRV_IMAGE_SENSOR_MAX_CONFIGS; i++) {
		if (isensor->iSensorConfigs[i]->sensorSupported){
			if (isensor->iSensorConfigs[i]->sensorResolution == resolution) {
				if (isensor->iSensorConfigs[i]->sensorFormat == format) {
					found = 1;
					break;
				}
			}
		}
	}
	if (found == 0)
	{
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\n DRV_IMAGE_SENSOR_CONFIGS Not found \r\n"); 
		return DRV_IMAGE_SENSOR_ERROR;
	}

	return DRV_ImageSensor_I2C_WriteRegs(isensor, isensor->iSensorConfigs[i]->iSensorRegSetting);
}

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Probe(const DRV_HANDLE I2CHandle, bool detect_auto, uint8_t id)
{
	uint8_t i;
	DRV_IMAGE_SENSOR_OBJ *isensor;  

	if (!detect_auto)
	{
		if (id >=DRV_IMAGE_SENSOR_MAX_DEVICES)
		{
			return NULL;
		}

		isensor = (DRV_IMAGE_SENSOR_OBJ*)Devices[id];
		isensor->drvI2CHandle = I2CHandle;
		if (DRV_ImageSensor_Read_ChipId(isensor) == DRV_IMAGE_SENSOR_SUCCESS)
		{
			return isensor;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		for (i = 0; i < DRV_IMAGE_SENSOR_MAX_DEVICES; i++)
		{
			isensor = (DRV_IMAGE_SENSOR_OBJ*)Devices[i];
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

uint32_t DRV_ImageSensor_GetConfig(DRV_IMAGE_SENSOR_OBJ *isensor,
									uint8_t resolution,
									uint8_t format,
									uint8_t *bits,
									uint32_t *width,
									uint32_t *height)
{
	uint8_t i;

	if (isensor == NULL)
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	for (i = 0; i < DRV_IMAGE_SENSOR_MAX_CONFIGS; i++)
	{
		if (isensor->iSensorConfigs[i]->sensorSupported && 
			(isensor->iSensorConfigs[i]->sensorResolution == resolution) &&
			(isensor->iSensorConfigs[i]->sensorFormat == format))
		{
			*bits = isensor->iSensorConfigs[i]->sensorBitWidth;
			*width = isensor->iSensorConfigs[i]->sensorWidth;
			*height = isensor->iSensorConfigs[i]->sensorHeight;
			return DRV_IMAGE_SENSOR_SUCCESS;
		}
	}
	return DRV_IMAGE_SENSOR_ERROR;
}

uint32_t DRV_ImageSensor_Stop(DRV_IMAGE_SENSOR_OBJ* isensor)
{
	bool status;

	if (isensor == NULL)
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	status = DRV_ImageSensor_I2C_WriteReg(isensor,
											isensor->streamStartAddr,
											isensor->streamStopValue);
	if (status == false)
	{
		return DRV_IMAGE_SENSOR_I2C_ERROR;
	}

	return DRV_IMAGE_SENSOR_SUCCESS;
}

uint32_t DRV_ImageSensor_Start(DRV_IMAGE_SENSOR_OBJ* isensor)
{
	bool status;
	
    if (isensor == NULL)
	{
		return DRV_IMAGE_SENSOR_ERROR;
	}

	status = DRV_ImageSensor_I2C_WriteReg(isensor,
											isensor->streamStartAddr,
											isensor->streamStartValue);
	if (status == false)
	{
		return DRV_IMAGE_SENSOR_I2C_ERROR;
	}

	return DRV_IMAGE_SENSOR_SUCCESS;
}

DRV_IMAGE_SENSOR_OBJ* DRV_ImageSensor_Init(uint32_t drvI2CIndex, const char* devname)
{
	DRV_HANDLE I2CHandle;
	DRV_IMAGE_SENSOR_OBJ* sensor;
	if(drvI2CIndex >= 0)
	{
		I2CHandle = DRV_I2C_Open(drvI2CIndex, DRV_IO_INTENT_READWRITE);
		if (I2CHandle == DRV_HANDLE_INVALID)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\n Camera: I2C Driver Open Failed\r\n");        
			return NULL;            
		}
	}
	else
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\n Camera: Invalid I2C Driver Index\r\n");        
		return NULL;
	}

	sensor = DRV_ImageSensor_Probe(I2CHandle, true, 0);
	if (sensor != NULL)
	{
		SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n Found %s Image Sensor \r\n",sensor->name);
//		if (strcmp(sensor->name, devname) == 0)
//		{
//			if (DRV_ImageSensor_Configure(sensor, DRV_IMAGE_SENSOR_VGA, DRV_IMAGE_SENSOR_RAW_BAYER) != DRV_IMAGE_SENSOR_SUCCESS)
//			{
//				SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Sensor setup failed.\r\n");
//			}
//		}
	}
	else
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Image Sensor probe failed.\r\n");
		return NULL;
	}
	return sensor;
}