
#ifndef ISC_DRV_H    /* Guard against multiple inclusion */
#define ISC_DRV_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "system/system_module.h"
#include "vision/drivers/image_sensor/drv_image_sensor.h"
#include "vision/drivers/isc/drv_isc.h"
#include "vision/peripheral/csi/plib_csi.h"

#define DMA_MAX_BUFFERS		(10)
#define COUNTER_FREQ		(1)

/** Video frame buffer size calculation */
#define FRAME_BUFFER_SIZE(W,H,BPP)	((W)*(H)* (BPP/8))

#define DEFAULT_FRAME_WIDTH    (640)
#define DEFAULT_FRAME_HEIGHT   (480)

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CAMERA_CALLBACK) (uintptr_t context);

// *****************************************************************************
/* CAMERA IRQ Callback Object

	Summary:
		Struct for ISC IRQ handler

	Description:
		This structure defines the ISC IRQ handler object, used to store the IRQ
		callback function registered from the ISC driver

	Remarks:
		None.
*/

typedef struct
{
	CAMERA_CALLBACK callback_fn;
	uintptr_t context;
}CAMERA_CALLBACK_OBJECT;


// *****************************************************************************
/*Structure
	CAMERA_INIT

	Summary:
		Defines the data required to initialize or reinitialize the CAMERA driver

	Description:
		This data type defines the data required to initialize or reinitialize the
		CAMERA driver.

	Remarks:
		None.
*/
typedef struct
{
	/* System module initialization */
	SYS_MODULE_INIT moduleInit;
	char imageSensorName[32];
	uint8_t imageSensorResolution;
    uint8_t imageSensorOutputFormat;
    uint8_t imageSensorOutputBitWidth;
	uint8_t iscInputFormat;
	uint8_t iscInputBits;
	uint8_t iscBayerPattern;
	uint8_t csiDataFormat;
	uint8_t iscOutputFormat;
	uint8_t iscOutputLayout;
	uint32_t imageWidth;
	uint32_t imageHeight;
	uint32_t drvI2CIndex;
	bool iscEnableGamma;
	bool iscEnableMIPI;
	bool iscEnableWhiteBalance;
	bool iscEnableHistogram; 
	bool iscEnableVideoMode;
	bool iscEnableBightnessAndContrast;
	bool iscEnableProgressiveMode;
} CAMERA_INIT;

SYS_MODULE_OBJ CAMERA_Initialize(const SYS_MODULE_INIT * const init);

bool CAMERA_Open(SYS_MODULE_OBJ object);

void CAMERA_Register_CallBack(const CAMERA_CALLBACK eventHandler,
								const uintptr_t contextHandle);

bool CAMERA_Start_Capture(SYS_MODULE_OBJ object);

bool CAMERA_Stop_Capture(SYS_MODULE_OBJ object);

bool CAMERA_Get_Frame(SYS_MODULE_OBJ object,
						uint32_t* buffer,
						uint32_t* width,
						uint32_t* height);

uint32_t CAMERA_Status(void);

#ifdef __cplusplus
}
#endif

#endif /* ISC_DRV_H */

/* *****************************************************************************
 End of File
 */
