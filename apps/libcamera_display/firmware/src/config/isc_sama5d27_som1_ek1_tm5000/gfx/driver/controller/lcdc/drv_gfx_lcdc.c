/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  MPLAB Harmony LCDC Generated Driver Implementation File

  File Name:
    drv_gfx_lcdc.c

  Summary:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/





// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/driver/gfx_driver.h"
#include "gfx/driver/controller/lcdc/drv_gfx_lcdc.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define BUFFER_PER_LAYER    1

#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 480

#define PIXEL_CLOCK_DIV 6
#define LCDC_OUTPUT_COLOR_MODE LCDC_OUTPUT_COLOR_MODE_24BPP
#define LCDC_DISPLAY_GUARD_NUM_FRAMES 30
#define LCDC_SYNC_EDGE LCDC_SYNC_EDGE_FIRST
#define LCDC_PWM_POLARITY LCDC_POLARITY_POSITIVE
#define GFX_LCDC_BACKGROUND_COLOR 0xffffffff
#define GFX_LCDC_LAYERS 2
#define LCDC_DEFAULT_BRIGHTNESS_PCT 100

#define SYNC_RECT_COUNT 200

#define XPHIDEF 0
#define YPHIDEF 0

#define LCDC_VSYNC_POLARITY LCDC_POLARITY_NEGATIVE

#define LCDC_HSYNC_POLARITY LCDC_POLARITY_NEGATIVE

#define LCDC_ENABLE_GLOBAL_HW_ALPHA 1

#define LCDC_PWM_CLOCK_SOURCE LCDC_PWM_SOURCE_SYSTEM
#define LCDC_PWM_PRESCALER 5

#define LCDC_DEFAULT_GFX_COLOR_MODE LCDC_INPUT_COLOR_MODE_RGB_565
#define FRAMEBUFFER_PTR_TYPE    uint16_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint16_t

typedef enum
{
    LAYER_UNLOCKED,
    LAYER_LOCKED,
    LAYER_LOCKED_PENDING,
} LAYER_LOCK_STATUS;

/**** Hardware Abstraction Interfaces ****/
typedef enum
{
    INIT = 0,
    DRAW,
    SWAP,
    SYNC,
} DRV_STATE;

const char* DRIVER_NAME = "LCDC";


typedef struct
{
  uint32_t addr;
  uint32_t ctrl;
  uint32_t next;
} LCDC_DMA_DESC;

LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc0;
LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc1;

static volatile DRV_STATE state[GFX_LCDC_LAYERS];
static unsigned int vsyncCount = 0;
static unsigned int activeLayer = 0;

static volatile int32_t waitForAlphaSetting[GFX_LCDC_LAYERS] = {0};

//Layer Parameters
//--------------------------------------------------------------------------
typedef struct __display_layer {
    LCDC_LAYER_ID hwLayerID;
    FRAMEBUFFER_PIXEL_TYPE  *baseaddr[BUFFER_PER_LAYER];
    int        draw;
    int        frame;
    uint32_t   resx;
    uint32_t   resy;
    uint32_t   buscfg;
    uint32_t   format;
    int32_t    startx;
    int32_t    starty;
    uint32_t   sizex;
    uint32_t   sizey;
    uint32_t   alpha;
    uint32_t   colorspace;
    uint16_t   color;
    LCDC_DMA_DESC * desc;
    uint32_t frameOffset;
    bool enabled;
    volatile LAYER_LOCK_STATUS updateLock;
    uint32_t   updateBaseAddr;
    gfxPixelBuffer pixelBuffer[BUFFER_PER_LAYER];
    volatile unsigned int frontBufferIdx;
    volatile unsigned int backBufferIdx;
    volatile gfxIRQCallBack irqCallback;
} DISPLAY_LAYER;
static DISPLAY_LAYER drvLayer[GFX_LCDC_LAYERS];

//This array defines the z-order of the hw layers in the LCDC
//This must be dynamically configured for the LCDC periperal on the part
static LCDC_LAYER_ID lcdcLayerZOrder[GFX_LCDC_LAYERS] =
{
    LCDC_LAYER_BASE,
    LCDC_LAYER_HEO,
};



void _IntHandlerLayerReadComplete(uintptr_t context);

static void LCDCUpdateDMADescriptor(LCDC_DMA_DESC * desc, uint32_t addr, uint32_t ctrl, uint32_t next)
{
    desc->addr = addr;
    desc->ctrl = ctrl;
    desc->next = next;
}

void DRV_LCDC_Update()
{
    uint32_t i;

    for (i = 0; i < GFX_LCDC_LAYERS; i++)
    {
        switch(state[i])
        {
            case INIT:
            {
                state[i] = DRAW;
                break;
            }
            case DRAW:
            case SWAP:
            default:
                break;
        }
    }
}

static uint32_t getLCDCStrideFromColorMode(LCDC_INPUT_COLOR_MODE mode)
{
    switch(mode)
    {
        case LCDC_INPUT_COLOR_MODE_LUT8:
            return 1;
            break;
        case LCDC_INPUT_COLOR_MODE_RGB_565:
            return 2;
            break;
        case LCDC_INPUT_COLOR_MODE_RGB_888:
            return 4;
            break;
        case LCDC_INPUT_COLOR_MODE_RGBA_8888:
        case LCDC_INPUT_COLOR_MODE_ARGB_8888:
        default:
            return 4;
            break;
    }    
}

static LCDC_INPUT_COLOR_MODE getLCDCColorModeFromGFXColorMode(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return LCDC_INPUT_COLOR_MODE_LUT8;
            break;
        case GFX_COLOR_MODE_RGB_565:
            return LCDC_INPUT_COLOR_MODE_RGB_565;
            break;
        case GFX_COLOR_MODE_RGB_888:
            return LCDC_INPUT_COLOR_MODE_RGB_888;
            break;
        case GFX_COLOR_MODE_RGBA_8888:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
        case GFX_COLOR_MODE_ARGB_8888:
            return LCDC_INPUT_COLOR_MODE_ARGB_8888;
            break;
        case GFX_COLOR_MODE_INDEX_1:
        case GFX_COLOR_MODE_INDEX_4:
        case GFX_COLOR_MODE_INDEX_8:
        case GFX_COLOR_MODE_RGB_332:
            return LCDC_INPUT_COLOR_MODE_UNSUPPORTED;
            break;
        default:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
    }
}

static void layerHEOGetScalingFactors(uint16_t xmemsize,
                                     uint16_t ymemsize,
                                     uint16_t xsize,
                                     uint16_t ysize,
                                     uint16_t* xfactor,
                                     uint16_t* yfactor)
{
    uint16_t xfactor1st, yfactor1st;

    xmemsize--;
    ymemsize--;
    xsize--;
    ysize--;
            
    xfactor1st = ((2048 * xmemsize - 256 * XPHIDEF)/ xsize) + 1;
    yfactor1st = ((2048 * ymemsize - 256 * XPHIDEF)/ ysize) + 1;

    if ((xfactor1st * xsize / 2048) > xmemsize)
        *xfactor = xfactor1st - 1;
    else
        *xfactor = xfactor1st;

    if ((yfactor1st * ysize / 2048) > ymemsize)
        *yfactor = yfactor1st - 1;
    else
        *yfactor = yfactor1st;
}

static void layerHEOSetSize(uint16_t s_width, uint16_t s_height, uint16_t w_width, uint16_t w_height)
{
    LCDC_SetWindowSize(LCDC_LAYER_HEO, w_width, w_height);
    LCDC_SetHEOImageMemSize(s_width, s_height);    
    
    //Source and window size are not the same, use scaler
    if (s_width != w_width || s_height != w_height)
    {
        uint16_t scale_w, scale_h;
        
        layerHEOGetScalingFactors(s_width,
                                s_height,
                                w_width,
                                w_height,
                                &scale_w,
                                &scale_h);
          
        LCDC_SetHEOScaler(scale_h, scale_w, true);
    }
    else
    {
        LCDC_SetHEOScaler(0, 0, false);
    }
}

static gfxColorMode getGFXColorModeFromLCDC(LCDC_INPUT_COLOR_MODE mode)
{
    switch(mode)
    {
        case LCDC_INPUT_COLOR_MODE_LUT8:
            return GFX_COLOR_MODE_GS_8;
        case LCDC_INPUT_COLOR_MODE_RGB_565:
            return GFX_COLOR_MODE_RGB_565;
        case LCDC_INPUT_COLOR_MODE_RGB_888:
            return GFX_COLOR_MODE_RGB_888;
        case LCDC_INPUT_COLOR_MODE_ARGB_8888:
            return GFX_COLOR_MODE_ARGB_8888;
        case LCDC_INPUT_COLOR_MODE_RGBA_8888:
            return GFX_COLOR_MODE_RGBA_8888;
        default:
            return GFX_COLOR_MODE_RGBA_8888;
    }
}

gfxResult LCDC_BrightnessRangeGet(uint32_t *low, uint32_t *high)
{
    *low = 0;
    *high = 100;

    return GFX_SUCCESS;
}

gfxResult LCDC_BrightnessSet(uint32_t brightness)
{

    LCDC_SetPWMCompareValue(brightness * 0xff / 100);

    return GFX_SUCCESS;
}

gfxResult DRV_LCDC_Initialize()
{
    uint32_t      xResolution;
    uint32_t      yResolution;
    uint32_t      rightMargin;
    uint32_t      lowerMargin;
    uint32_t      hsyncLength;
    uint32_t      vsyncLength;
    uint32_t      leftMargin;
    uint32_t      upperMargin;
    uint32_t      layerCount;
    
    //Clear the descriptor and structures
    memset(drvLayer, 0, sizeof(drvLayer));

    /* set temporary information */
    xResolution     = 800;
    yResolution     = 480;
    rightMargin     = 40;
    leftMargin      = 40;
    hsyncLength     = 48;
    vsyncLength     = 3;
    upperMargin     = 29;
    lowerMargin     = 3;


    /* LCDC initialization */
    //1. Configure the LCD timing parameters
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMClockSourceSelection(LCDC_PWM_CLOCK_SOURCE);
    LCDC_SetClockDivider(PIXEL_CLOCK_DIV);

    //Disable all layers for now
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_BASE, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR1, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR2, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_HEO, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_PP, false);

    LCDC_WaitForSyncInProgress();
    LCDC_SetHSYNCPulseWidth(hsyncLength); //Set the pulse widths
    LCDC_SetVSYNCPulseWidth(vsyncLength);

    LCDC_WaitForSyncInProgress();
    LCDC_SetVerticalFrontPorchWidth(lowerMargin); //Set the vertical porches
    LCDC_SetVerticalBackPorchWidth(upperMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetHorizontalFrontPorchWidth(rightMargin); //Set the horizontal porches
    LCDC_SetHorizontalBackPorchWidth(leftMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetNumActiveRows(yResolution);
    LCDC_SetNumPixelsPerLine(xResolution);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetDisplayGuardTime(LCDC_DISPLAY_GUARD_NUM_FRAMES);
    LCDC_SetOutputMode(LCDC_OUTPUT_COLOR_MODE);
    LCDC_SetDisplaySignalSynchronization(true);
    LCDC_SetVSYNCPulseStart(LCDC_SYNC_EDGE);
    LCDC_SetVSYNCPolarity(LCDC_VSYNC_POLARITY);
    LCDC_SetHSYNCPolarity(LCDC_HSYNC_POLARITY);

    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMCompareValue(LCDC_DEFAULT_BRIGHTNESS_PCT * 0xFF / 100);
    LCDC_SetPWMSignalPolarity(LCDC_PWM_POLARITY);
    LCDC_SetPWMPrescaler(LCDC_PWM_PRESCALER);

    //2. Enable the Pixel Clock
    LCDC_WaitForSyncInProgress();
    LCDC_SetPixelClockEnable(true);
    
    //3. Poll CLKSTS field of the LCDC_LCDSR register to check that the clock is running.
    LCDC_WaitForClockRunning();
    
    // 4. Enable Horizontal and Vertical Synchronization by writing one to the
    // SYNCEN field of the LCDC_LCDEN register.
    LCDC_WaitForSyncInProgress();
    LCDC_SetSYNCEnable(true);
    
    // 5. Check that synchronization is up. */    
    LCDC_WaitForSynchronization();
    
    //6. Enable the display power signal 
    LCDC_WaitForSyncInProgress();
    LCDC_SetDISPSignalEnable(true);
    
    //7. Wait for power signal to be activated
    LCDC_WaitForDISPSignal();
    
    //8. Enable the backlight
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMEnable(true);

    drvLayer[0].desc = &channelDesc0;
    drvLayer[1].desc = &channelDesc1;
    
    for (layerCount = 0; layerCount < GFX_LCDC_LAYERS; layerCount++)
    {
        drvLayer[layerCount].hwLayerID = lcdcLayerZOrder[layerCount];
        drvLayer[layerCount].resx       = xResolution;
        drvLayer[layerCount].resy       = yResolution;
        drvLayer[layerCount].startx     = 0;
        drvLayer[layerCount].starty     = 0;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = 255;
        drvLayer[layerCount].colorspace = LCDC_DEFAULT_GFX_COLOR_MODE;
        drvLayer[layerCount].frameOffset = 0;
        drvLayer[layerCount].enabled    = true;
        LCDCUpdateDMADescriptor(drvLayer[layerCount].desc, 
                                (uint32_t) drvLayer[layerCount].baseaddr[0],
                                0x01,
                                (uint32_t) drvLayer[layerCount].desc);
        
       
        LCDC_SetLayerClockGatingDisable(drvLayer[layerCount].hwLayerID, false);
        LCDC_SetWindowPosition(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].startx, drvLayer[layerCount].starty);
        if (drvLayer[layerCount].hwLayerID == LCDC_LAYER_HEO)
            layerHEOSetSize(drvLayer[layerCount].resx, drvLayer[layerCount].resy, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
        else
            LCDC_SetWindowSize(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
        LCDC_SetUseDMAPathEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetRGBModeInput(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].colorspace);
        LCDC_SetDMAAddressRegister(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].desc->addr);
        LCDC_SetDMADescriptorNextAddress(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetDMAHeadPointer(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetTransferDescriptorFetchEnable(drvLayer[layerCount].hwLayerID, true);

        //Note: Blender APIs don't do anything to the base layer
        LCDC_SetBlenderOverlayLayerEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetBlenderDMALayerEnable(drvLayer[layerCount].hwLayerID, true); //Enable blender DMA
		//Using Global Alpha
        LCDC_SetBlenderGlobalAlphaEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetBlenderGlobalAlpha(drvLayer[layerCount].hwLayerID, 0xFF);
        LCDC_UpdateOverlayAttributesEnable(drvLayer[layerCount].hwLayerID);
        LCDC_UpdateAttribute(drvLayer[layerCount].hwLayerID); //Apply the attributes
        
        LCDC_SetSytemBusDMABurstEnable(drvLayer[layerCount].hwLayerID, true); // Set DLBO in configuration reg 0
        LCDC_SetSytemBusDMABurstLength(drvLayer[layerCount].hwLayerID, LCDC_BASECFG0_BLEN_AHB_INCR16_Val); // Set burst length
       
        LCDC_SetChannelEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_IRQ_Enable(LCDC_INTERRUPT_BASE + drvLayer[layerCount].hwLayerID);
        
        drvLayer[layerCount].frontBufferIdx = 0;
        drvLayer[layerCount].backBufferIdx = 0;
        drvLayer[layerCount].updateLock = LAYER_UNLOCKED;

        gfxPixelBufferCreate(xResolution,
                            yResolution,
                            getGFXColorModeFromLCDC(LCDC_DEFAULT_GFX_COLOR_MODE),
                            drvLayer[layerCount].baseaddr[0],
                            &drvLayer[layerCount].pixelBuffer[drvLayer[layerCount].frontBufferIdx]);

    }

    //Set HEO layer on top of OVL1
    LCDC_SetHEOVideoPriority(true);

    //Register the interrupt handler
    LCDC_IRQ_CallbackRegister(_IntHandlerLayerReadComplete, (uintptr_t) NULL);
    
    //Enable layer interrupts
    for (layerCount = 0; layerCount < GFX_LCDC_LAYERS; layerCount++)
    {
        LCDC_LAYER_IRQ_Enable(drvLayer[layerCount].hwLayerID, LCDC_LAYER_INTERRUPT_DMA);       
    }
    
    return GFX_SUCCESS;
}

void _IntHandlerLayerReadComplete(uintptr_t context)
{
    uint32_t i, status;
	//Check to see which layer this interrupt is from
    for (i = 0; i < GFX_LCDC_LAYERS; i++)
    {
        status = LCDC_LAYER_IRQ_Status(drvLayer[i].hwLayerID);

        if (status)
        {
	        LCDC_LAYER_IRQ_Disable(drvLayer[i].hwLayerID, LCDC_LAYER_INTERRUPT_DMA); 
            if (drvLayer[i].updateLock == LAYER_LOCKED_PENDING)
            {
                if (drvLayer[i].irqCallback != NULL)
                {
                    drvLayer[i].irqCallback(i);
                }
                drvLayer[i].updateLock = LAYER_UNLOCKED;
            }
        }
    }
    

    if (i < SYNC && state[i] == SWAP)
    {
        state[i] = SYNC;
    }
}

gfxResult DRV_LCDC_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf)
{
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;

    if (state[activeLayer] != DRAW)
	{
        return GFX_FAILURE;
	}


    rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx], x, y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }

    return GFX_SUCCESS;
}

static gfxDriverIOCTLResponse DRV_LCDC_layerConfig(gfxDriverIOCTLRequest request,
                                           gfxIOCTLArg_LayerArg* arg)
{
    gfxIOCTLArg_LayerValue* val;
    gfxIOCTLArg_LayerPosition* pos;
    gfxIOCTLArg_LayerSize* sz;
    uint32_t stride = 0;
    int32_t x,y = 0;
    uint32_t width, height = 0;

    //Make sure layer is locked before accepting changes
    if (arg->id >= GFX_LCDC_LAYERS)
        return GFX_IOCTL_ERROR_UNKNOWN;
    
    // attempt to lock
    if (request == GFX_IOCTL_SET_LAYER_LOCK)
    {
        LCDC_LAYER_IRQ_Disable(drvLayer[arg->id].hwLayerID, LCDC_LAYER_INTERRUPT_DMA);

        drvLayer[arg->id].updateLock = LAYER_LOCKED;
        
        return GFX_IOCTL_OK;
    }
    
    //Layer should be locked 
    if (drvLayer[arg->id].updateLock != LAYER_LOCKED)
        return GFX_IOCTL_ERROR_UNKNOWN;
    
    if (request == GFX_IOCTL_SET_LAYER_UNLOCK)
    {
        drvLayer[arg->id].updateBaseAddr = (uint32_t)drvLayer[arg->id].baseaddr[drvLayer[arg->id].frontBufferIdx];

        width = drvLayer[arg->id].resx;
        height = drvLayer[arg->id].resy;
        x = drvLayer[arg->id].startx;
        y = drvLayer[arg->id].starty;

        if (x < 0)
        {
            drvLayer[arg->id].updateBaseAddr += abs(x) *
                    getLCDCStrideFromColorMode(drvLayer[arg->id].colorspace);
            width += x;
            x = 0;
        }

        if (y < 0)
        {
            drvLayer[arg->id].updateBaseAddr += abs(y) * drvLayer[arg->id].sizex *
                    getLCDCStrideFromColorMode(drvLayer[arg->id].colorspace);
            height += y;
            y = 0;
        }

        if (x + width > DISPLAY_WIDTH)
            width = (x < DISPLAY_WIDTH) ? DISPLAY_WIDTH - x : 0;

        if (y + height > DISPLAY_HEIGHT)
            height = (y < DISPLAY_HEIGHT) ? DISPLAY_HEIGHT - y : 0;

        stride = abs(drvLayer[arg->id].sizex - width) * 
            getLCDCStrideFromColorMode(drvLayer[arg->id].colorspace);

        LCDCUpdateDMADescriptor(drvLayer[arg->id].desc, 
                                drvLayer[arg->id].updateBaseAddr,
                                0x01,
                                (uint32_t) drvLayer[arg->id].desc);

        LCDC_SetWindowPosition(drvLayer[arg->id].hwLayerID, x, y);
        LCDC_SetHorizStride(drvLayer[arg->id].hwLayerID, stride);

        LCDC_SetWindowSize(drvLayer[arg->id].hwLayerID, width, height);

        LCDC_SetRGBModeInput(drvLayer[arg->id].hwLayerID, drvLayer[arg->id].colorspace);

        LCDC_SetBlenderGlobalAlpha(drvLayer[arg->id].hwLayerID, drvLayer[arg->id].alpha);  
		
        LCDC_SetChannelEnable(drvLayer[arg->id].hwLayerID, drvLayer[arg->id].enabled);

        //Update overlay attributes before the start of the next frame
        LCDC_UpdateOverlayAttributesEnable(drvLayer[arg->id].hwLayerID);

        LCDC_SetUseDMAPathEnable(drvLayer[arg->id].hwLayerID, drvLayer[arg->id].enabled);

        LCDC_LAYER_IRQ_Enable(drvLayer[arg->id].hwLayerID, LCDC_LAYER_INTERRUPT_DMA);
        
        drvLayer[arg->id].updateLock = LAYER_LOCKED_PENDING;
        
        return GFX_IOCTL_OK;
    }
    
    switch(request)
    {
        case GFX_IOCTL_SET_LAYER_ALPHA:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].alpha = val->value.v_uint;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_SIZE:
        {
            sz = (gfxIOCTLArg_LayerSize*)arg;
            
            drvLayer[arg->id].sizex = sz->width;
            drvLayer[arg->id].sizey = sz->height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_POSITION:
        {
            pos = (gfxIOCTLArg_LayerPosition*)arg;
            
            drvLayer[arg->id].startx = pos->x;
            drvLayer[arg->id].starty = pos->y;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_WINDOW_SIZE:
        {
            sz = (gfxIOCTLArg_LayerSize*)arg;
            
            drvLayer[arg->id].resx = sz->width;
            drvLayer[arg->id].resy = sz->height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_BASE_ADDRESS:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].baseaddr[drvLayer[arg->id].frontBufferIdx] = val->value.v_pointer;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_COLOR_MODE:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].colorspace = getLCDCColorModeFromGFXColorMode(val->value.v_colormode);
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_ENABLED:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            val->value.v_bool = drvLayer[arg->id].enabled;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_ENABLED:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].enabled = val->value.v_bool;
            
            return GFX_IOCTL_OK;
        }
        default:
            break;
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

gfxDriverIOCTLResponse DRV_LCDC_IOCTL(gfxDriverIOCTLRequest request,
                                      void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;
    gfxIOCTLArg_LayerIRQCallback* callback;
    
    switch(request)
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
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = BUFFER_PER_LAYER;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize*)arg;            
            
            disp->width = DISPLAY_WIDTH;
            disp->height = DISPLAY_HEIGHT;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = GFX_LCDC_LAYERS;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = activeLayer;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            if(val->value.v_uint >= GFX_LCDC_LAYERS)
            {
                return GFX_IOCTL_ERROR_UNKNOWN;
            }
            else
            {
                activeLayer = val->value.v_uint;
                
                return GFX_IOCTL_OK;
            }
        }
        case GFX_IOCTL_GET_LAYER_RECT:
        {
            rect = (gfxIOCTLArg_LayerRect*)arg;
            
            if(rect->base.id >= GFX_LCDC_LAYERS)        
                return GFX_IOCTL_ERROR_UNKNOWN;
            
            rect->x = drvLayer[rect->base.id].startx;
            rect->y = drvLayer[rect->base.id].starty;
            rect->width = drvLayer[rect->base.id].sizex;
            rect->height = drvLayer[rect->base.id].sizey;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = vsyncCount;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_FRAMEBUFFER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx];

            return GFX_IOCTL_OK;
        }
		case GFX_IOCTL_SET_BRIGHTNESS:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            LCDC_BrightnessSet(val->value.v_int);
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_STATUS:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = 0;
            
            unsigned int i;
            for (i = 0; i < GFX_LCDC_LAYERS; i++)
            {
                if (drvLayer[i].updateLock != LAYER_UNLOCKED)
                {
                    val->value.v_uint = 1;

                    break;
                }
            }
            return GFX_IOCTL_OK;
        }		
        case GFX_IOCTL_SET_IRQ_CALLBACK:
        {
            callback = (gfxIOCTLArg_LayerIRQCallback*)arg;
            
            if(callback->base.id >= GFX_LCDC_LAYERS)        
                return GFX_IOCTL_ERROR_UNKNOWN;
            
            drvLayer[callback->base.id].irqCallback = callback->callback;                    
            
            return GFX_IOCTL_OK;            
        }
        default:
        {
            if (request >= GFX_IOCTL_LAYER_REQ_START && 
                request <= GFX_IOCTL_LAYER_REQ_END)
            {
                return DRV_LCDC_layerConfig(request, (gfxIOCTLArg_LayerArg*)arg);
            }
            break;
        }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

/*******************************************************************************
 End of File
*/
