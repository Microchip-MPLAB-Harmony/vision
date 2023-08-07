/*******************************************************************************
	Camera 

	Company:
		Microchip Technology Inc.

	File Name:
		camera.c

	Summary:
	camera File

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

#include "device.h"
#include "camera.h"
#include "peripheral/pio/plib_pio.h"
#include "vision/peripheral/isc/plib_isc.h"
#include "vision/peripheral/csi2dc/plib_csi2dc.h"
#include "system/debug/sys_debug.h"
#include "system/int/sys_int.h"
#include "vision/drivers/csi2dc/drv_csi2dc.h"
#include "vision/drivers/csi/drv_csi.h"

// ToDO Instead of fixed memory, Implement a memory pool 
__attribute__ ((__section__(".region_cache"))) __attribute__ ((__aligned__(32))) static uint8_t GBuffer[1920 * 1080 * 4 * DMA_MAX_BUFFERS];

/* Color space matrix setting */
static PLIB_ISC_COLOR_SPACE ref_cs = {
	0x42, 0x81, 0x19, 0x10, 0xFDA, 0xFB6, 0x70, 0x80, 0x70, 0xFA2, 0xFEE, 0x80};
	
static PLIB_ISC_COLOR_CORRECT ref_cc = {
	0, 0, 0, 0x100, 0, 0, 0x100, 0, 0, 0, 0, 0x100};

/* Gamma table with gamma 1/2.2 */
static uint32_t gamma_22_table[ISC_GAMMA_ENTRIES] = 
{
	0xa001c,  0x3f0012,  0x50000e,  0x5d000c,  0x68000c,  0x72000a,  0x7c0008,  0x840008,
	0x8c0006,  0x930006,  0x9a0006,  0xa00006,  0xa60006,  0xac0006,  0xb20006,  0xb80004,
	0xbd0006,  0xc20006,  0xc70004,  0xcc0004,  0xd10004,  0xd50004,  0xda0004,  0xde0004,
	0xe20004,  0xe60004,  0xea0004,  0xee0004,  0xf20004,  0xf60004,  0xfa0004,  0xfe0004,
	0x1020084, 0x1440068, 0x178005a, 0x1a50050, 0x1cd0048, 0x1f10044, 0x213003e, 0x232003a,
	0x24f0036, 0x26a0034, 0x2840032, 0x29d002e, 0x2b4002e, 0x2cb002c, 0x2e1002a, 0x2f6002a,
	0x30b0028, 0x31f0026, 0x3320026, 0x3450024, 0x3570024, 0x3690022, 0x37a0022, 0x38b0022,
	0x39c0020, 0x3ac0020, 0x3bc0020, 0x3cc001e, 0x3db001e, 0x3ea001e, 0x3f9001c, 0x3ff001e
};

// defines an Camera driver object instance
typedef struct
{
	char imageSensorName[32];
	uint8_t imageSensorResolution;
    uint8_t imageSensorOutputFormat;
    uint8_t imageSensorOutputBitWidth;
	uint8_t iscInputFormat;
	uint32_t imageWidth;
	uint32_t imageHeight;
	uint32_t drvI2CIndex;
	uint32_t bufferSize;
	DRV_ISC_OBJ* iscObj;
	DRV_CSI2DC_OBJ* csi2dcObj;
	DRV_CSI_OBJ* csiObj;
	DRV_IMAGE_SENSOR_OBJ* sensor;
} DEVICE_OBJECT;

/* Camera Driver instance object */
DEVICE_OBJECT DrvCameraInstances;  
CAMERA_CALLBACK_OBJECT CameraCallbackObj;

static void CAMERA_ISC_Callback(uintptr_t context)
{
	if (CameraCallbackObj.callback_fn != NULL)
	{
		CameraCallbackObj.callback_fn(CameraCallbackObj.context);
	}
}

void CAMERA_Register_CallBack(const CAMERA_CALLBACK eventHandler,
								const uintptr_t contextHandle)
{
	CameraCallbackObj.callback_fn = eventHandler;
	CameraCallbackObj.context = contextHandle;
}

/* Configure ISC */
static bool CAMERA_configure(DEVICE_OBJECT* pDrvObject)
{
	uint8_t bpp = 0;

	if(pDrvObject == NULL)
	{
		return false;
	}

	if (pDrvObject->iscObj->gamma.enableGamma)
	{
		pDrvObject->iscObj->gamma.enableBiPart = true;
		pDrvObject->iscObj->gamma.enableRed = true;
		pDrvObject->iscObj->gamma.enableGreen = true;
		pDrvObject->iscObj->gamma.enableBlue = true;
		pDrvObject->iscObj->gamma.redEntries = gamma_22_table;
		pDrvObject->iscObj->gamma.greenEntries = gamma_22_table;
		pDrvObject->iscObj->gamma.blueEntries = gamma_22_table;  
	}

	pDrvObject->iscObj->colorSpace = &ref_cs;
	pDrvObject->iscObj->colorCorrection = &ref_cc;
	
	if (pDrvObject->iscObj->whiteBalance.enableWB)
	{
		pDrvObject->iscObj->whiteBalance.redOffset = 0x1ef8;
		pDrvObject->iscObj->whiteBalance.greenRedOffset = 0x1ef8;
		pDrvObject->iscObj->whiteBalance.blueOffset = 0x1f00;
		pDrvObject->iscObj->whiteBalance.greenBlueOffset = 0x1ef0;
		pDrvObject->iscObj->whiteBalance.redGain = 0x798;
		pDrvObject->iscObj->whiteBalance.greenRedGain = 0x44f;
		pDrvObject->iscObj->whiteBalance.blueGain = 0xd4b;
		pDrvObject->iscObj->whiteBalance.greenBlueGain = 0x653;
	}

	if (pDrvObject->iscObj->cbc.enableCBC)
	{
		pDrvObject->iscObj->cbc.bright = 0x01;
		pDrvObject->iscObj->cbc.contrast = 0x12;
		pDrvObject->iscObj->cbc.hue = 0;
		pDrvObject->iscObj->cbc.saturation = 0x20;
	}

#if 0
	if (pDrvObject->iscObj->enableHistogram)
	{
		pDrvObject->iscObj->histo_buf = buffer_histogram;        
	}
	else
#endif
	{
		pDrvObject->iscObj->enableHistogram = false;
		pDrvObject->iscObj->histogramBuffer = NULL;        
	}

	switch(pDrvObject->iscObj->layout)
	{
		case ISC_LAYOUT_PACKED8:
		{
			bpp = 8;
			break;
		}
		case ISC_LAYOUT_PACKED16:
		case ISC_LAYOUT_YC422SP:
		case ISC_LAYOUT_YC422P:
		case ISC_LAYOUT_YC420SP:
		case ISC_LAYOUT_YC420P:
		case ISC_LAYOUT_YUY2:
		{
			bpp = 16;
			break;
		}
		case ISC_LAYOUT_PACKED32:
		{
			bpp = 32;
			break;
		}
		default:
		{
			bpp = 32;
			break;
		}
	}

	pDrvObject->bufferSize = FRAME_BUFFER_SIZE(pDrvObject->imageWidth, pDrvObject->imageHeight, bpp);
	pDrvObject->iscObj->dma.address0 = (uint32_t) GBuffer;
	pDrvObject->iscObj->dma.size = pDrvObject->bufferSize; 
	pDrvObject->iscObj->dma.callback = CAMERA_ISC_Callback;
	pDrvObject->iscObj->dmaDescSize = DMA_MAX_BUFFERS;

	DRV_ISC_Configure(pDrvObject->iscObj);

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\n\r CAMERA_configure: Done \n\r");

	return true;
}

bool CAMERA_Get_Frame(SYS_MODULE_OBJ object,
							uint32_t* buffer,
							uint32_t* width,
							uint32_t* height)
{
	uint8_t index = 0;
	DEVICE_OBJECT* pDrvObject = (DEVICE_OBJECT *)object;

	if(object == SYS_MODULE_OBJ_INVALID)
	{
		return false;
	}

	index = (pDrvObject->iscObj->frameIndex == 0) ? (DMA_MAX_BUFFERS - 1) : (pDrvObject->iscObj->frameIndex - 1);
	*buffer = (uint32_t)GBuffer + (index * pDrvObject->bufferSize);

	*width = pDrvObject->imageWidth;
	*height = pDrvObject->imageHeight;

	return true;
}

bool CAMERA_Stop_Capture(SYS_MODULE_OBJ object)
{
	DEVICE_OBJECT* pDrvObject = (DEVICE_OBJECT *)object;

	if(object == SYS_MODULE_OBJ_INVALID)
	{
		return false;
	}

	DRV_ImageSensor_Stop(pDrvObject->sensor);

	ISC_Stop_Capture();

	SYS_INT_SourceDisable(ID_ISC);

	return true;
}

bool CAMERA_Start_Capture(SYS_MODULE_OBJ object)
{
	DEVICE_OBJECT* pDrvObject = (DEVICE_OBJECT *)object;

	if(object == SYS_MODULE_OBJ_INVALID)
	{
		return false;
	}

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r ISC_Drv_Start_Capture : Start \n\r");

	DRV_ImageSensor_Start(pDrvObject->sensor);

	ISC_Start_Capture();
	while(1)
	{
		volatile const uint32_t status = ISC_Interrupt_Status(); 
		if ((status & ISC_INTSR_VD_Msk) == ISC_INTSR_VD_1)
		{
			ISC_Start_Capture();
			break;
		}
	}
	SYS_INT_SourceEnable(ID_ISC);

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r ISC_Drv_Start_Capture : Done \n\r");

	return true;
}

uint32_t CAMERA_Status()
{
	return ISC_Ctrl_Status();
}

bool CAMERA_Open(SYS_MODULE_OBJ object)
{
	DEVICE_OBJECT* pDrvObject = (DEVICE_OBJECT *)object;
    
	if(object == SYS_MODULE_OBJ_INVALID)
	{
		return false;
	}

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n CAMERA_Open: Start\r\n");

	if (pDrvObject->sensor)
	{
		if(DRV_ImageSensor_Configure(pDrvObject->sensor, 
			pDrvObject->imageSensorResolution,
			pDrvObject->imageSensorOutputFormat) != DRV_IMAGE_SENSOR_SUCCESS)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Sensor setup failed.\r\n");
		}

		if (DRV_ImageSensor_GetConfig(pDrvObject->sensor,
				pDrvObject->imageSensorResolution,
				pDrvObject->imageSensorOutputFormat,
				&pDrvObject->imageSensorOutputBitWidth,
				&pDrvObject->imageWidth,
				&pDrvObject->imageHeight) != DRV_IMAGE_SENSOR_SUCCESS)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Image Sensor Configurations: success \r\n");
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t OutputBitWidth = 0x%x \r\n",pDrvObject->imageSensorOutputBitWidth);
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t imageWidth = %d \r\n", pDrvObject->imageWidth);
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t imageHeight = %d \r\n", pDrvObject->imageHeight);
		}
	}

	if (pDrvObject->iscObj->inputBits != pDrvObject->imageSensorOutputBitWidth)
    {
        pDrvObject->iscObj->inputBits = pDrvObject->imageSensorOutputBitWidth;
    }
    
    if (pDrvObject->imageWidth <= 0)
    {
        pDrvObject->imageWidth = DEFAULT_FRAME_WIDTH;
    }
    
    if (pDrvObject->imageHeight <= 0)
    {
        pDrvObject->imageHeight = DEFAULT_FRAME_HEIGHT;
    }
    
	if (pDrvObject->csi2dcObj)
	{
		DRV_CSI2DC_Configure(pDrvObject->csi2dcObj);
	}

	if (pDrvObject->csiObj)
	{
		pDrvObject->csiObj->csiFrameWidth = pDrvObject->imageWidth;        
		pDrvObject->csiObj->csiFrameHeight = pDrvObject->imageHeight;
		// ToDo Add fps, nlanes, channelid from Sensor configuration. 
		DRV_CSI_Configure(pDrvObject->csiObj);
	}

	CAMERA_configure(pDrvObject);

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n CAMERA_Open: End\r\n");

	return true;
}

SYS_MODULE_OBJ CAMERA_Initialize(const SYS_MODULE_INIT * const init)
{
	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n CAMERA_Initialize : START\r\n");

	if(init == NULL)
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: Init is Null\r\n");
		return SYS_MODULE_OBJ_INVALID;
	}

	const CAMERA_INIT *pInit = (const CAMERA_INIT* const)init;
	DEVICE_OBJECT *pDrvInstance = (DEVICE_OBJECT*)&DrvCameraInstances;

#if !defined(CAMERA_RESET_PIN)
#error "Configure CAMERA_RESET_PIN is not configured in pin configuration"
#endif

	/* Reset ImageSensor*/
	if (CAMERA_RESET_PIN >= 0)
		CAMERA_RESET_Clear();

	if(pInit->drvI2CIndex >= 0)
	{
		pDrvInstance->sensor =  DRV_ImageSensor_Init(pInit->drvI2CIndex, pInit->imageSensorName);
		if (pDrvInstance->sensor == NULL)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: DRV_ImageSensor_Init Failed \r\n");
			return SYS_MODULE_OBJ_INVALID;
		}
        
        if (pDrvInstance->sensor->name != NULL)
        {
            strcpy(pDrvInstance->imageSensorName, pDrvInstance->sensor->name);            
        }
        else if (pInit->imageSensorName != NULL)
        {
            strcpy(pDrvInstance->imageSensorName, pInit->imageSensorName); 
        }

        if(pInit->imageSensorResolution >= 0)
            pDrvInstance->imageSensorResolution = pInit->imageSensorResolution;
        else
            pDrvInstance->imageSensorResolution = DRV_IMAGE_SENSOR_VGA;

        if(pInit->imageSensorOutputFormat >= 0)
            pDrvInstance->imageSensorOutputFormat = pInit->imageSensorOutputFormat;
        else
            pDrvInstance->imageSensorOutputFormat = DRV_IMAGE_SENSOR_RAW_BAYER;

        if(pInit->imageSensorOutputBitWidth >= 0)
            pDrvInstance->imageSensorOutputBitWidth = pInit->imageSensorOutputBitWidth;
        else
            pDrvInstance->imageSensorOutputBitWidth = DRV_IMAGE_SENSOR_10_BIT;
        
        if(DRV_ImageSensor_Configure(pDrvInstance->sensor, 
			pDrvInstance->imageSensorResolution,
			pDrvInstance->imageSensorOutputFormat) != DRV_IMAGE_SENSOR_SUCCESS)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Sensor setup failed.\r\n");
		}

		if (DRV_ImageSensor_GetConfig(pDrvInstance->sensor,
				pDrvInstance->imageSensorResolution,
				pDrvInstance->imageSensorOutputFormat,
				&pDrvInstance->imageSensorOutputBitWidth,
				&pDrvInstance->imageWidth,
				&pDrvInstance->imageHeight) == DRV_IMAGE_SENSOR_SUCCESS)
		{
			SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n Image Sensor Configurations: success \r\n");
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t OutputBitWidth = 0x%x \r\n",pDrvInstance->imageSensorOutputBitWidth);
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t imageWidth = %d \r\n", pDrvInstance->imageWidth);
			SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\n\t imageHeight = %d \r\n", pDrvInstance->imageHeight);
		}
	}
	else
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: Invalid I2C Driver Index\r\n");
		return SYS_MODULE_OBJ_INVALID;
	}

	if(pInit->imageSensorName != NULL)
		strcpy(pDrvInstance->imageSensorName, pInit->imageSensorName);

	if(pInit->imageSensorResolution >= 0)
		pDrvInstance->imageSensorResolution = pInit->imageSensorResolution;
	else
		pDrvInstance->imageSensorResolution = DRV_IMAGE_SENSOR_VGA;
    
    if(pInit->imageSensorOutputFormat >= 0)
		pDrvInstance->imageSensorOutputFormat = pInit->imageSensorOutputFormat;
	else
		pDrvInstance->imageSensorOutputFormat = DRV_IMAGE_SENSOR_RAW_BAYER;
    
    if(pInit->imageSensorOutputBitWidth >= 0)
		pDrvInstance->imageSensorOutputBitWidth = pInit->imageSensorOutputBitWidth;
	else
		pDrvInstance->imageSensorOutputBitWidth = DRV_IMAGE_SENSOR_10_BIT;

	pDrvInstance->csi2dcObj = DRV_CSI2DC_Initalize();
	if (pDrvInstance->csi2dcObj == NULL)
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: DRV_CSI2DC_Initalize Failed \r\n");
		return SYS_MODULE_OBJ_INVALID;
	}

	pDrvInstance->csiObj = DRV_CSI_Initalize();
	if (pDrvInstance->csiObj == NULL)
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: DRV_CSI_Initalize Failed \r\n");
		return SYS_MODULE_OBJ_INVALID;
	}

	if(pInit->csiDataFormat >= 0)
	{
		pDrvInstance->csi2dcObj->videoPipeDataType = pInit->csiDataFormat;
		pDrvInstance->csi2dcObj->dataPipeDataType = pInit->csiDataFormat;
		pDrvInstance->csiObj->csiDataType = pInit->csiDataFormat;
	}
	else
	{
		pDrvInstance->csi2dcObj->videoPipeDataType = CSI2_DATA_FORMAT_RAW10;
		pDrvInstance->csi2dcObj->dataPipeDataType = CSI2_DATA_FORMAT_RAW10;
		pDrvInstance->csiObj->csiDataType = CSI2_DATA_FORMAT_RAW10;
	}


	if(pInit->csiDataFormat >= 0)
	{
		pDrvInstance->csi2dcObj->videoPipeDataType = pInit->csiDataFormat;
		pDrvInstance->csi2dcObj->dataPipeDataType = pInit->csiDataFormat;
	}
	else
	{
		pDrvInstance->csi2dcObj->videoPipeDataType = CSI2_DATA_FORMAT_RAW10;
		pDrvInstance->csi2dcObj->dataPipeDataType = CSI2_DATA_FORMAT_RAW10;
	}

	pDrvInstance->iscObj = DRV_ISC_Initialize();
	if (pDrvInstance->iscObj == NULL)
	{
		SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nCamera: DRV_ISC_Initialize Failed \r\n");
		return SYS_MODULE_OBJ_INVALID;
	}

	if(pInit->iscInputFormat >= 0)
	{
		pDrvInstance->iscInputFormat = pInit->iscInputFormat;
		pDrvInstance->iscObj->inputFormat = pInit->iscInputFormat;
	}
	else
	{
		pDrvInstance->iscObj->inputFormat = DRV_IMAGE_SENSOR_RAW_BAYER;
		pDrvInstance->iscInputFormat = DRV_IMAGE_SENSOR_RAW_BAYER;
	}

	if(pInit->iscOutputFormat >= 0)
		pDrvInstance->iscObj->rlpMode = pInit->iscOutputFormat;
	else
		pDrvInstance->iscObj->rlpMode =  ISC_RLP_CFG_MODE_ARGB32;

	if(pInit->iscBayerPattern >= 0)
		pDrvInstance->iscObj->bayerPattern = pInit->iscBayerPattern;
	else
		pDrvInstance->iscObj->bayerPattern = ISC_CFA_CFG_BAYCFG_RGRG_Val;

	if(pInit->iscOutputLayout >= 0)
		pDrvInstance->iscObj->layout = pInit->iscOutputLayout;
	else
		pDrvInstance->iscObj->layout = ISC_LAYOUT_PACKED32;

	if(pInit->iscInputBits >= 0)
	{
		pDrvInstance->iscObj->inputBits = pInit->iscInputBits;
	}
	else
	{
		pDrvInstance->iscObj->inputBits = DRV_IMAGE_SENSOR_10_BIT;
	}

	pDrvInstance->iscObj->enableVideoMode = pInit->iscEnableVideoMode;

	pDrvInstance->iscObj->enableMIPI = pInit->iscEnableMIPI;

	pDrvInstance->iscObj->cbc.enableCBC = pInit->iscEnableBightnessAndContrast;

	pDrvInstance->iscObj->enableProgressiveMode = pInit->iscEnableProgressiveMode;

	pDrvInstance->iscObj->whiteBalance.enableWB = pInit->iscEnableWhiteBalance;
	pDrvInstance->iscObj->gamma.enableGamma =  pInit->iscEnableGamma;
	pDrvInstance->iscObj->enableHistogram = pInit->iscEnableHistogram;

    if (pDrvInstance->imageWidth <= 0)
        pDrvInstance->imageWidth = DEFAULT_FRAME_WIDTH;
    
    if (pDrvInstance->imageHeight <= 0)
        pDrvInstance->imageHeight = DEFAULT_FRAME_HEIGHT;

	SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\n CAMERA_Initialize : END\r\n");
	
	return (SYS_MODULE_OBJ)pDrvInstance;
}