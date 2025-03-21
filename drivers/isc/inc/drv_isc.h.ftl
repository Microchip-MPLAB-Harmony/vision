#ifndef DRV_ISC_H
#define DRV_ISC_H

#include "vision/drivers/isc/plib_isc.h"
#include <stdint.h>
#include <stdbool.h>

#define ISC_MAX_DMA_VIEW_SIZE (sizeof(DRV_ISC_DMA_VIEW2)/sizeof(uint32_t))
#define ISC_MAX_DMA_DESC (10)

#define ISC_GAMMA_ENTRIES (64)

#define ISC_HISTOGRAM_GR (0)
#define ISC_HISTOGRAM_R  (1)
#define ISC_HISTOGRAM_GB (2)
#define ISC_HISTOGRAM_B  (3)

#define ISC_BAYER_COUNT (ISC_HISTOGRAM_B + 1)

#define ISC_HIST_ENTRIES (512)

#define ISC_SUCCESS      (0)
#define ISC_ERROR_LOCK   (1)
#define ISC_ERROR_CONFIG (2)

typedef void (*iscd_callback_t)(uintptr_t context);

typedef enum
{
    ISC_LAYOUT_PACKED8 = 0,
    ISC_LAYOUT_PACKED16,
    ISC_LAYOUT_PACKED32,
    ISC_LAYOUT_YC422SP,
    ISC_LAYOUT_YC422P,
    ISC_LAYOUT_YC420SP,
    ISC_LAYOUT_YC420P,
    ISC_LAYOUT_YUY2
} DRV_ISC_LAYOUT;

typedef struct
{
    bool enableWB;
    uint32_t redOffset;
    uint32_t greenRedOffset;
    uint32_t blueOffset;
    uint32_t greenBlueOffset;
    uint32_t redGain;
    uint32_t greenRedGain;
    uint32_t blueGain;
    uint32_t greenBlueGain;
} DRV_ISC_WB;

typedef struct
{
    bool enableGamma;
    bool enableBlue;
    bool enableGreen;
    bool enableRed;
    bool enableBiPart;
    uint32_t* blueEntries;
    uint32_t* greenEntries;
    uint32_t* redEntries;
} DRV_ISC_GAMMA;

typedef struct
{
    bool enableCBC;
    uint32_t bright;
    uint32_t contrast;
    uint32_t hue;
    uint32_t saturation;
} DRV_ISC_CBC;

typedef struct
{
    uint32_t address0;
    uint32_t address1;
    uint32_t address2;
    uint32_t size;
    iscd_callback_t callback;
} DRV_ISC_DMA;

typedef struct
{
    uint32_t ctrl;
    uint32_t nextDesc;
    uint32_t addr;
    uint32_t stride;
} DRV_ISC_DMA_VIEW0;

typedef struct
{
    uint32_t ctrl;
    uint32_t nextDesc;
    uint32_t addr0;
    uint32_t stride0;
    uint32_t addr1;
    uint32_t stride1;
} DRV_ISC_DMA_VIEW1;

typedef struct
{
    uint32_t ctrl;
    uint32_t nextDesc;
    uint32_t addr0;
    uint32_t stride0;
    uint32_t addr1;
    uint32_t stride1;
    uint32_t addr2;
    uint32_t stride2;
} DRV_ISC_DMA_VIEW2;

typedef struct
{
    uint8_t inputFormat;
    uint8_t inputBits;
    uint8_t dmaDescSize;
    uint8_t bayerColorFilter;
    uint32_t bayerPattern;
    DRV_ISC_LAYOUT layout;
    PLIB_ISC_COLOR_SPACE* colorSpace;
    DRV_ISC_WB whiteBalance;
    DRV_ISC_GAMMA gamma;
    DRV_ISC_CBC cbc;
    PLIB_ISC_COLOR_CORRECT* colorCorrection;
    bool enableHistogram;
    uint32_t* histogramBuffer;
    uint32_t rlpMode;
    uint8_t frameIndex;
    bool enableVideoMode;
    bool enableMIPI;
    bool enableProgressiveMode;
    bool enableScaling;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint16_t outputWidth;
    uint16_t outputHeight;
    volatile uint32_t frameCount;
    DRV_ISC_DMA dma;
} DRV_ISC_OBJ;

DRV_ISC_OBJ* DRV_ISC_Initialize(void);

uint8_t DRV_ISC_Configure(DRV_ISC_OBJ* iscObj);

uint8_t DRV_ISC_Configure_DMA(DRV_ISC_OBJ* iscObj);

void DRV_ISC_Configure_Scaler(DRV_ISC_OBJ* iscObj);

bool DRV_ISC_Start_Capture(DRV_ISC_OBJ* iscObj);

void DRV_ISC_Stop_Capture();


#endif  //DRV_ISC_H 
