/*******************************************************************************
  MPLAB Harmony XLCDC Generated Driver Implementation File

  File Name:
    drv_gfx_xlcdc.c

  Summary:
    Build-time generated implementation for the XLCDC Driver for SAM9X7/SAMA7D MPUs.

  Description:
    Contains function definitions and the data types that make up the interface to the XLCDC
    Graphics Controller for SAM9X7/SAMA7D MPUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#include "toolchain_specifics.h"
#include "gfx/driver/gfx_driver.h"
#include "gfx/driver/controller/xlcdc/drv_gfx_xlcdc.h"
#include "gfx/driver/controller/xlcdc/plib/plib_xlcdc.h"

/* Utility Macros */
/* Math */
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
/* Alignment Check */
#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) & ((align) - 1)) == 0)
/* Frame Buffer Macros */
/* Cached, Cache Aligned */
#define FB_CACHE_CA             CACHE_ALIGN
/* Not Cached */
#define FB_CACHE_NC             __attribute__ ((section(".region_nocache"), aligned (32)))
/* Frame Buffer Pointer Type */
#define FB_COL_MODE             XLCDC_RGB_COLOR_MODE_RGB_565
#define FB_BPP_TYPE             uint16_t
#define FB_PTR_TYPE             FB_BPP_TYPE *
#define FB_TYPE_SZ              sizeof(FB_BPP_TYPE)

/* Driver Settings */
#define XLCDC_HOR_RES           800
#define XLCDC_VER_RES           480
#define XLCDC_TOT_LAYERS        2
#define XLCDC_BUF_PER_LAYER     1

/* Local Data */
/* Driver */
typedef enum
{
    INIT = 0,
    DRAW
} DRV_STATE;

/* Generated Layer Order */
static const char layerOrder[XLCDC_TOT_LAYERS] = {
    XLCDC_LAYER_BASE,
    XLCDC_LAYER_HEO,
};

const char *DRIVER_NAME = "XLCDC";
static volatile DRV_STATE state;
static volatile uint32_t vsyncCount = 0;
static volatile bool vblankSync = false;

/* Layer */
typedef enum
{
    LAYER_LOCK_UNLOCKED,
    LAYER_LOCK_LOCKED,
    LAYER_LOCK_PENDING,
} LAYER_LOCK_STATUS;

typedef struct
{
    bool enabled;
    int pixelformat;
    uint32_t resx;
    uint32_t resy;
    uint32_t startx;
    uint32_t starty;
    uint32_t sizex;
    uint32_t sizey;
    uint32_t alpha;
    FB_PTR_TYPE baseaddr[XLCDC_BUF_PER_LAYER];
    gfxPixelBuffer pixelBuffer[XLCDC_BUF_PER_LAYER];
    volatile uint32_t frontBufferIdx;
    volatile LAYER_LOCK_STATUS updateLock;
} LAYER_ATTRIBUTES;

static uint32_t activeLayer = 0;
static gfxRect srcRect, destRect;
static LAYER_ATTRIBUTES drvLayer[XLCDC_TOT_LAYERS];

/* Convert GFX Color Mode to XLCDC Color Mode */
static XLCDC_RGB_COLOR_MODE DRV_XLCDC_ColorModeXLCDCFromGFX(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return XLCDC_RGB_COLOR_MODE_CLUT;
        case GFX_COLOR_MODE_RGB_565:
            return XLCDC_RGB_COLOR_MODE_RGB_565;
        case GFX_COLOR_MODE_RGB_888:
            return XLCDC_RGB_COLOR_MODE_RGB_888;
        case GFX_COLOR_MODE_ARGB_8888:
            return XLCDC_RGB_COLOR_MODE_ARGB_8888;
        case GFX_COLOR_MODE_RGBA_8888:
            return XLCDC_RGB_COLOR_MODE_RGBA_8888;
        default:
            return XLCDC_RGB_COLOR_MODE_RGBA_8888;
    }
}

/* Perform a CPU based Blit */
static gfxResult DRV_XLCDC_CPU_Blit(const gfxPixelBuffer* restrict source,
                                   const gfxRect* restrict rectSrc,
                                   const gfxPixelBuffer* restrict dest,
                                   const gfxRect* restrict rectDest)
{
    if (!source || !rectSrc || !dest || !rectDest)
        return GFX_FAILURE;

    // Calculate dimensions
    const uint32_t width = MIN(rectSrc->width, rectDest->width);
    const uint32_t height = MIN(rectSrc->height, rectDest->height);

    if (width == 0 || height == 0)
        return GFX_FAILURE;

    // Calculate row size in bytes
    const uint32_t pixelSize = gfxColorInfoTable[dest->mode].size;
    const uint32_t rowSize = width * pixelSize;

    // Calculate source and destination strides based on buffer widths
    const uint32_t srcStride = source->size.width * pixelSize;
    const uint32_t destStride = dest->size.width * pixelSize;

    uint8_t* restrict srcBase = (uint8_t*)gfxPixelBufferOffsetGet(source, rectSrc->x, rectSrc->y);
    uint8_t* restrict destBase = (uint8_t*)gfxPixelBufferOffsetGet(dest, rectDest->x, rectDest->y);

    // Check if we can do a single large transfer i.e. we have contiguous data
    if (width == source->size.width && width == dest->size.width)
    {
        const uint32_t totalSize = rowSize * height;
        memcpy(destBase, srcBase, totalSize);

        return GFX_SUCCESS;
    }

    // Row by row processing for non-contiguous data
    for (uint32_t row = 0; row < height; row++)
    {
        uint8_t* restrict src = srcBase + row * srcStride;
        uint8_t* restrict dst = destBase + row * destStride;
        memcpy(dst, src, rowSize);
    }

    return GFX_SUCCESS;
}

/* Process the Layer IOCTL Subset */
static gfxDriverIOCTLResponse DRV_XLCDC_LayerConfig(gfxDriverIOCTLRequest request,
                                                  gfxIOCTLArg_LayerArg *arg)
{
    gfxIOCTLArg_LayerValue *layerVal;
    gfxIOCTLArg_LayerPosition *layerPos;
    gfxIOCTLArg_LayerSize *layerSize;

    if (arg->id >= XLCDC_TOT_LAYERS)
    {
        return GFX_IOCTL_ERROR_UNKNOWN;

    }

    if (request == GFX_IOCTL_SET_LAYER_LOCK)
    {
        drvLayer[arg->id].updateLock = LAYER_LOCK_LOCKED;

        return GFX_IOCTL_OK;
    }

    if (drvLayer[arg->id].updateLock != LAYER_LOCK_LOCKED)
    {
        return GFX_IOCTL_ERROR_UNKNOWN;
    }

    if (request == GFX_IOCTL_SET_LAYER_UNLOCK)
    {
        XLCDC_SetLayerAddress(layerOrder[arg->id], (uint32_t)drvLayer[arg->id].baseaddr[0], false);
        XLCDC_SetLayerOpts(layerOrder[arg->id], drvLayer[arg->id].alpha, true, false);
        XLCDC_SetLayerWindowXYPos(layerOrder[arg->id], drvLayer[arg->id].startx, drvLayer[arg->id].starty, false);
        XLCDC_SetLayerWindowXYSize(layerOrder[arg->id], drvLayer[arg->id].sizex, drvLayer[arg->id].sizey, false);
        XLCDC_SetLayerXStride(layerOrder[arg->id], FB_TYPE_SZ * (drvLayer[arg->id].resx - drvLayer[arg->id].sizex), false);
        XLCDC_SetLayerEnable(layerOrder[arg->id], drvLayer[arg->id].enabled, true);

        drvLayer[arg->id].updateLock = LAYER_LOCK_UNLOCKED;

        return GFX_IOCTL_OK;
    }

    switch (request)
    {
        case GFX_IOCTL_SET_LAYER_ALPHA:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].alpha = layerVal->value.v_uint;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_SIZE:
        {
            layerSize = (gfxIOCTLArg_LayerSize *)arg;

            drvLayer[arg->id].resx = layerSize->width;
            drvLayer[arg->id].resy = layerSize->height;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_POSITION:
        {
            layerPos = (gfxIOCTLArg_LayerPosition *)arg;

            drvLayer[arg->id].startx = layerPos->x;
            drvLayer[arg->id].starty = layerPos->y;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_WINDOW_SIZE:
        {
            layerSize = (gfxIOCTLArg_LayerSize *)arg;

            drvLayer[arg->id].sizex = layerSize->width;
            drvLayer[arg->id].sizey = layerSize->height;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_BASE_ADDRESS:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].baseaddr[0] = layerVal->value.v_pointer;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_COLOR_MODE:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].pixelformat = DRV_XLCDC_ColorModeXLCDCFromGFX(layerVal->value.v_colormode);

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_LAYER_ENABLED:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            layerVal->value.v_bool = drvLayer[arg->id].enabled;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_ENABLED:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].enabled = layerVal->value.v_bool;

            return GFX_IOCTL_OK;
        }

        default:
            break;
    }

    return GFX_IOCTL_UNSUPPORTED;
}

void DRV_XLCDC_Update(void)
{
    switch(state)
    {
        case INIT:
        {
            state = DRAW;
            break;
        }
        case DRAW:
        default:
            break;
    }
}

gfxResult DRV_XLCDC_Initialize(void)
{
    /* Clear the Layer Attributes */
    memset(drvLayer, 0, sizeof(drvLayer));

    /* Initialize Layer Attributes */
    for (uint32_t layerCount = 0; layerCount < XLCDC_TOT_LAYERS; layerCount++)
    {
        drvLayer[layerCount].pixelformat = FB_COL_MODE;
        drvLayer[layerCount].resx = XLCDC_HOR_RES;
        drvLayer[layerCount].resy = XLCDC_VER_RES;
        drvLayer[layerCount].startx = 0;
        drvLayer[layerCount].starty = 0;
        drvLayer[layerCount].sizex = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha = 255;
        drvLayer[layerCount].enabled = false;
        drvLayer[layerCount].updateLock = LAYER_LOCK_UNLOCKED;
        drvLayer[layerCount].frontBufferIdx = 0;

        XLCDC_SetLayerEnable(layerOrder[layerCount], false, true);
        XLCDC_SetLayerAddress(layerOrder[layerCount], (uint32_t) drvLayer[layerCount].baseaddr[drvLayer[layerCount].frontBufferIdx], false);
        XLCDC_SetLayerOpts(layerOrder[layerCount], 255, true, false);
        XLCDC_SetLayerWindowXYPos(layerOrder[layerCount], 0, 0, false);
        XLCDC_SetLayerWindowXYSize(layerOrder[layerCount], XLCDC_HOR_RES, XLCDC_VER_RES, false);
        XLCDC_SetLayerEnable(layerOrder[layerCount], drvLayer[layerCount].enabled, true);
    }

    return GFX_SUCCESS;
}

gfxResult DRV_XLCDC_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf)
{
    gfxResult result = GFX_FAILURE;

    if (state != DRAW)
        return result;

    gfxPixelBuffer_SetLocked(buf, GFX_TRUE);

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.height = buf->size.height;
    srcRect.width = buf->size.width;

    destRect.x = x;
    destRect.y = y;
    destRect.height = buf->size.height;
    destRect.width = buf->size.width;

    result = DRV_XLCDC_CPU_Blit(buf,
                                &srcRect,
                                &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx],
                                &destRect);

    gfxPixelBuffer_SetLocked(buf, GFX_FALSE);

    return result;
}

gfxDriverIOCTLResponse DRV_XLCDC_IOCTL(gfxDriverIOCTLRequest request, void* arg)
{
    gfxIOCTLArg_Value *val;
    gfxIOCTLArg_DisplaySize *disp;
    gfxIOCTLArg_LayerRect *rect;

    switch (request)
    {
        case GFX_IOCTL_LAYER_SWAP:
        {
            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_FRAME_END:
        {
            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_BUFFER_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = XLCDC_BUF_PER_LAYER;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize *)arg;

            disp->width = XLCDC_HOR_RES;
            disp->height = XLCDC_VER_RES;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = XLCDC_TOT_LAYERS;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = activeLayer;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_ACTIVE_LAYER:
        {
            gfxDriverIOCTLResponse response = GFX_IOCTL_OK;

            val = (gfxIOCTLArg_Value *)arg;

            if (val->value.v_uint >= XLCDC_TOT_LAYERS)
            {
                response =  GFX_IOCTL_ERROR_UNKNOWN;
            }
            else
            {
                activeLayer = val->value.v_uint;
            }

            return response;
        }

        case GFX_IOCTL_GET_LAYER_RECT:
        {
            rect = (gfxIOCTLArg_LayerRect *)arg;

            if (rect->layer.id >= XLCDC_TOT_LAYERS)
            {
                return GFX_IOCTL_ERROR_UNKNOWN;
            }

            rect->x = drvLayer[rect->layer.id].startx;
            rect->y = drvLayer[rect->layer.id].starty;
            rect->width = drvLayer[rect->layer.id].sizex;
            rect->height = drvLayer[rect->layer.id].sizey;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = vsyncCount;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_FRAMEBUFFER:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx];

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_PALETTE:
        {
            return GFX_IOCTL_UNSUPPORTED;
        }

        case GFX_IOCTL_SET_VBLANK_SYNC:
        {
            vblankSync = ((gfxIOCTLArg_Value *)arg)->value.v_bool;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_STATUS:
        {
            val = (gfxIOCTLArg_Value *)arg;
            val->value.v_uint = 0;
            unsigned int i;

            for (i = 0; i < XLCDC_TOT_LAYERS; i++)
            {
                if (drvLayer[i].updateLock != LAYER_LOCK_UNLOCKED)
                {
                    val->value.v_uint = 1;

                    break;
                }
            }

            return GFX_IOCTL_OK;
        }

        default:
        {
            if (request >= GFX_IOCTL_LAYER_REQ_START &&
                request <= GFX_IOCTL_LAYER_REQ_END)
            {
                return DRV_XLCDC_LayerConfig(request, (gfxIOCTLArg_LayerArg *)arg);
            }
                break;
        }
    }

    return GFX_IOCTL_UNSUPPORTED;
}
