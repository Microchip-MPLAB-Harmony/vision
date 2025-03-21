
#ifndef DRV_CSI2DC_H    /* Guard against multiple inclusion */
#define DRV_CSI2DC_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

typedef enum
{
    CSI2DC_BUS_CSI2_DPHY = 0,
    CSI2DC_BUS_PARALLEL
} CSI2DC_BUS_MODE;

typedef enum
{
    CSI2DC_DMA_CHUCK_SIZE_1 = 0,
    CSI2DC_DMA_CHUCK_SIZE_2,
    CSI2DC_DMA_CHUCK_SIZE_4,
    CSI2DC_DMA_CHUCK_SIZE_8,
    CSI2DC_DMA_CHUCK_SIZE_16
} CSI2DC_DMA_CHUCK_SIZE;

typedef struct
{
    uint16_t count;
    CSI2DC_DMA_CHUCK_SIZE chuckSize;
    bool enableDMA;
} CSI2DC_DMA;

typedef struct
{
    uint8_t videoPipeChannelId;
    uint8_t dataPipeChannelId;
    uint8_t videoPipeDataType;
    uint8_t dataPipeDataType;
    CSI2DC_BUS_MODE busMode;
    CSI2DC_DMA csi2dcDma;
    bool enableMIPIFreeRun;
    bool videoPipeAlign;
    bool enableDataPipe;
} DRV_CSI2DC_OBJ;

DRV_CSI2DC_OBJ* DRV_CSI2DC_Initalize(void);

bool DRV_CSI2DC_Configure(DRV_CSI2DC_OBJ* Obj);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif
