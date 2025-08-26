
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "device.h"
#include "peripheral/xdmac/plib_xdmac.h"
#include "system/cache/sys_cache.h"
#include "system/debug/sys_debug.h"
#include "system/int/sys_int.h"
#include "system/time/sys_time.h"

#include "vision/drivers/image_sensor/drv_image_sensor.h"
#include "vision/drivers/isc/drv_isc.h"

#define debug_print(args ...) if (CAMERA_ENABLE_DEBUG) fprintf(stderr, args)

typedef union {
    DRV_ISC_DMA_VIEW0 view0[ISC_MAX_DMA_DESC];
    DRV_ISC_DMA_VIEW1 view1[ISC_MAX_DMA_DESC];
    DRV_ISC_DMA_VIEW2 view2[ISC_MAX_DMA_DESC];
} ISC_DEVICE_DMA_Pool;

__attribute__((__section__(".region_cache_aligned"))) __attribute__((__aligned__(32))) ISC_DEVICE_DMA_Pool isc_dma_pool;

static DRV_ISC_OBJ DrvISCObj;

static void DelayUS(uint32_t us) {
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;
    if (SYS_TIME_DelayUS(us, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

void ISC_Handler(void) {
    volatile const uint32_t status = ISC_Interrupt_Status();
    if ((status & ISC_INTSR_HISDONE_Msk) == ISC_INTSR_HISDONE_Msk) {
        ISC_Disable_Interrupt(ISC_INTSR_HISDONE_Msk);
        DrvISCObj.awb.hist_dma.dma_histo_ready = true;
        ISC_Enable_Interrupt(ISC_INTSR_HISDONE_Msk);
    }

    if ((status & ISC_INTSR_DDONE_Msk) == ISC_INTSR_DDONE_Msk) {
        ISC_Disable_Interrupt(ISC_INTEN_DDONE_Msk);
        if (DrvISCObj.frameIndex == (DrvISCObj.dmaDescSize - 1))
            DrvISCObj.frameIndex = 0;
        else
            DrvISCObj.frameIndex++;

        if (DrvISCObj.dma.callback)
            DrvISCObj.dma.callback((uintptr_t) & DrvISCObj);
        ISC_Enable_Interrupt(ISC_INTEN_DDONE_Msk);
    }
    if ((status & ISC_INTSR_VD_Msk) == ISC_INTSR_VD_Msk) {
        DrvISCObj.frameCount++;
    }
    // ToDo Handle other interrupts.
}

void DRV_ISC_Stop_Capture() {
    ISC_Stop_Capture();
}

bool DRV_ISC_Start_Capture(DRV_ISC_OBJ* iscObj) {
    int count = 1000;

    if (iscObj == NULL)
        return false;

    ISC_Start_Capture();

    while (count--) {
        if ((ISC_Interrupt_Status() & ISC_INTSR_VD_Msk) == ISC_INTSR_VD(1)) {
            ISC_Start_Capture();
            break;
        }
        DelayUS(1000);
    }

    if (count <= 0) {
        debug_print("\n\r ISC_Start_Capture timeout \n\r");
        return false;
    }

    return true;
}

uint8_t DRV_ISC_Configure_DMA(DRV_ISC_OBJ* iscObj) {
    uint32_t i;
    DRV_ISC_DMA_VIEW0* dma_view0;
    DRV_ISC_DMA_VIEW1* dma_view1;
    DRV_ISC_DMA_VIEW2* dma_view2;

    switch (iscObj->layout) {
        case ISC_LAYOUT_PACKED8:
        case ISC_LAYOUT_PACKED16:
        case ISC_LAYOUT_PACKED32:
        case ISC_LAYOUT_YUY2:
        {
            dma_view0 = isc_dma_pool.view0;
            for (i = 0; i < iscObj->dmaDescSize; i++) {
                dma_view0[i].ctrl = ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE(1);
                dma_view0[i].nextDesc = (uint32_t) & dma_view0[i + 1];
                dma_view0[i].addr = (uint32_t) iscObj->dma.address0 + (i * iscObj->dma.size);
                dma_view0[i].stride = 0;
            }
            dma_view0[i - 1].nextDesc = (uint32_t) & dma_view0[0];
            SYS_CACHE_CleanDCache_by_Addr((uint32_t*) dma_view0, sizeof (DRV_ISC_DMA_VIEW0) * iscObj->dmaDescSize);
            ISC_DMA_Configure_Desc_Entry((uint32_t) & dma_view0[0]);
            if (iscObj->layout == ISC_LAYOUT_YUY2)
#ifdef ISC_DCFG_YMBSIZE_BEATS32
                ISC_DMA_Configure_Input_Mode(ISC_DCFG_IMODE_PACKED32 | ISC_DCFG_YMBSIZE_BEATS32);
#else
                ISC_DMA_Configure_Input_Mode(ISC_DCFG_IMODE_PACKED32 | ISC_DCFG_YMBSIZE_BEATS16);
#endif
            else
                ISC_DMA_Configure_Input_Mode(ISC_DCFG_IMODE(iscObj->layout) | ISC_DCFG_YMBSIZE_BEATS16);
            ISC_DMA_Enable(ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE(1));
            break;
        }
        case ISC_LAYOUT_YC420SP:
        case ISC_LAYOUT_YC422SP:
        {
            /* Set DAM for 16-bit YC422SP/YC420SP with stream descriptor view 1
                for YCbCr planar pixel stream */
            dma_view1 = isc_dma_pool.view1;
            for (i = 0; i < iscObj->dmaDescSize; i++) {
                dma_view1[i].ctrl = ISC_DCTRL_DVIEW_SEMIPLANAR | ISC_DCTRL_DE(1);
                dma_view1[i].nextDesc = (uint32_t) & dma_view1[i + 1];
                dma_view1[i].addr0 = (uint32_t) iscObj->dma.address0 + i * iscObj->dma.size;
                dma_view1[i].stride0 = 0;
                dma_view1[i].addr1 = (uint32_t) iscObj->dma.address1 + i * iscObj->dma.size;
                ;
                dma_view1[i].stride1 = 0;
            }
            dma_view1[i - 1].nextDesc = (uint32_t) & dma_view1[0];
            SYS_CACHE_CleanDCache_by_Addr((uint32_t*) dma_view1, sizeof (DRV_ISC_DMA_VIEW1) * iscObj->dmaDescSize);
            ISC_DMA_Configure_Desc_Entry((uint32_t) & dma_view1[0]);
            ISC_DMA_Configure_Input_Mode(ISC_DCFG_IMODE(iscObj->layout) |
                    ISC_DCFG_YMBSIZE_BEATS8 | ISC_DCFG_CMBSIZE_BEATS8);
            ISC_DMA_Enable(ISC_DCTRL_DVIEW_SEMIPLANAR | ISC_DCTRL_DE(1));
            break;
        }
        case ISC_LAYOUT_YC422P:
        case ISC_LAYOUT_YC420P:
        {
            /* Set DAM for 16-bit YC422P/YC420P with stream descriptor view 2
                for YCbCr planar pixel stream */
            dma_view2 = isc_dma_pool.view2;
            for (i = 0; i < iscObj->dmaDescSize; i++) {
                dma_view2[i].ctrl = ISC_DCTRL_DVIEW_PLANAR | ISC_DCTRL_DE(1);
                dma_view2[i].nextDesc = (uint32_t) & dma_view2[i + 1];
                dma_view2[i].addr0 = (uint32_t) iscObj->dma.address0 + i * iscObj->dma.size;
                ;
                dma_view2[i].stride0 = 0;
                dma_view2[i].addr1 = (uint32_t) iscObj->dma.address1 + i * iscObj->dma.size;
                ;
                dma_view2[i].stride1 = 0;
                dma_view2[i].addr2 = (uint32_t) iscObj->dma.address2 + i * iscObj->dma.size;
                ;
                dma_view2[i].stride2 = 0;
            }
            dma_view2[i - 1].nextDesc = (uint32_t) & dma_view2[0];
            SYS_CACHE_CleanDCache_by_Addr((uint32_t*) dma_view2, sizeof (DRV_ISC_DMA_VIEW2) * iscObj->dmaDescSize);
            ISC_DMA_Configure_Desc_Entry((uint32_t) & dma_view2[0]);
            ISC_DMA_Configure_Input_Mode(ISC_DCFG_IMODE(iscObj->layout) |
                    ISC_DCFG_YMBSIZE_BEATS8 | ISC_DCFG_CMBSIZE_BEATS8);
            ISC_DMA_Enable(ISC_DCTRL_DVIEW_PLANAR | ISC_DCTRL_DE(1));
            break;
        }
        default:
            return ISC_ERROR_CONFIG;
    }

    return ISC_SUCCESS;
}

void DRV_ISC_Configure_Scaler(DRV_ISC_OBJ* iscObj) {
    if (iscObj->enableScaling) {
        ISC_Scaler_Enable(1, 1);
        ISC_Scaler_Source_Size(iscObj->imageWidth - 1, iscObj->imageHeight - 1);
        ISC_Scaler_Destination_Size(iscObj->outputWidth - 1, iscObj->outputHeight - 1);
        uint32_t vfactor = (uint32_t) ((uint64_t) (iscObj->imageHeight << 20) / iscObj->outputHeight);
        ISC_Scaler_Vertical_Scaling_Factor(vfactor);
        uint32_t hfactor = (uint32_t) ((uint64_t) (iscObj->imageWidth << 20) / iscObj->outputWidth);
        ISC_Scaler_Horizontal_Scaling_Factor(hfactor);
        ISC_Scaler_Configure_Vertical_Scaling();
        ISC_Scaler_Configure_Horizontal_Scaling();
    }
}

void hist_dma_callback(XDMAC_TRANSFER_EVENT status, uintptr_t context) {
    if (status == XDMAC_TRANSFER_COMPLETE) {
        DRV_ISC_OBJ* iscobj = (DRV_ISC_OBJ*) context;
        XDMAC_ChannelDisable(iscobj->awb.hist_dma.channel);
        SYS_CACHE_InvalidateDCache_by_Addr((uint32_t*) iscobj->histogramBuffer,
                ISC_HIST_ENTRIES * sizeof (uint32_t));
        iscobj->awb.hist_dma.dma_histo_done = true;
    } else {
        printf("\t\r dma error\n\r");
    }
}

void DRV_ISC_Configure_Histogram(DRV_ISC_OBJ* iscObj) {
    if (iscObj->enableHistogram) {
        iscObj->awb.hist_dma.channel = XDMAC_CHANNEL_0;
        XDMAC_ChannelCallbackRegister(iscObj->awb.hist_dma.channel, hist_dma_callback, (uintptr_t) iscObj);
        ISC_Histogram_Enable(1);
        ISC_Clear_Histogram_Table();
        iscObj->awb.state = AWB_INIT;
        iscObj->awb.op_mode = 0;
    }
}

uint8_t DRV_ISC_Configure(DRV_ISC_OBJ* iscObj) {
    uint32_t i;
    uint32_t* ge;
    uint32_t* re;
    uint32_t* be;

    if (iscObj == NULL)
        return ISC_ERROR_CONFIG;

    DrvISCObj = *iscObj;

    SYS_INT_SourceDisable(ID_ISC);
    ISC_Software_Reset();

    /* Set Continuous Acquisition mode */
    ISC_PFE_Enable_Video_Mode(iscObj->enableVideoMode);
    ISC_CFA_Enable(0);
    ISC_WB_Enable(0);
    ISC_Gamma_Enable(0, 0, 0);
    ISC_CSC_Enable(0);
    ISC_Sub422_Enable(0);
    ISC_Sub420_Configure(0, 0);

    /* ToDo else condition to handle interlaced setting */
    if (iscObj->enableProgressiveMode) {
        ISC_PFE_Set_Video_Mode(ISC_PFE_CFG0_MODE_PROGRESSIVE);
    }

    ISC_PFE_Set_BPS(ISC_PFE_CFG0_BPS(
            (iscObj->inputBits == DRV_IMAGE_SENSOR_40_BIT)
            ? 5 : (4 - iscObj->inputBits)));

#ifdef ISC_PFE_CFG0_MIPI_Msk
    ISC_PFE_MIPI_Enable(iscObj->enableMIPI);
#endif

    ISC_PFE_Set_Sync_Polarity(ISC_PFE_CFG0_HPOL(ISC_HSYNC_POLARITY_VAL),
            ISC_PFE_CFG0_VPOL(ISC_VSYNC_POLARITY_VAL));

    switch (iscObj->inputFormat) {
        case DRV_IMAGE_SENSOR_RAW_BAYER:

            if (iscObj->dpc.enableBLC) {
                ISC_Enable_Black_Level(iscObj->dpc.enableBLC, iscObj->dpc.blofstVal);
            }

            if (iscObj->dpc.enableGDC) {
                ISC_Enable_Green_Correction(iscObj->dpc.enableGDC, iscObj->dpc.gdcclpVal);
            }

            if (iscObj->dpc.enableDPC) {
                ISC_Enable_Defective_Pixel_Correction(iscObj->dpc.enableDPC);
                ISC_DPC_Configure(iscObj->dpc.bayerPattern, iscObj->dpc.enableEITPOL,
                        iscObj->dpc.enableTM, iscObj->dpc.enableTA, iscObj->dpc.enableTC,
                        iscObj->dpc.reModeVal, iscObj->dpc.ndModeVal, iscObj->dpc.ThreshMVal,
                        iscObj->dpc.ThreshAVal, iscObj->dpc.ThreshCVal);
            }

            if (iscObj->gamma.enableGamma) {
                if ((!iscObj->gamma.greenEntries) ||
                        (!iscObj->gamma.blueEntries) ||
                        (!iscObj->gamma.redEntries)) {
                    ISC_Gamma_Enable(1, 0, 0);
                } else {
                    // Todo move to plib
                    ISC_Gamma_Enable(1, ISC_GAM_CTRL_RENABLE(1) |
                            ISC_GAM_CTRL_GENABLE(1) |
                            ISC_GAM_CTRL_BENABLE(1),
                            iscObj->gamma.enableBiPart);

                    ge = iscObj->gamma.greenEntries;
                    be = iscObj->gamma.blueEntries;
                    re = iscObj->gamma.redEntries;
                    for (i = 0; i < ISC_GAMMA_ENTRIES; i++) {
                        ISC_REGS->ISC_GAM_RENTRY[i] = *re++;
                        ISC_REGS->ISC_GAM_GENTRY[i] = *ge++;
                        ISC_REGS->ISC_GAM_BENTRY[i] = *be++;
                    }
                }
            } else {
                ISC_Gamma_Enable(0, 0, 0);
            }

            /* In a single-sensor system, each cell on the sensor
             * has a specific color filter and microlens
             * positioned above it. The raw data obtained from the
             * sensor do not have the full R/G/B information at
             * each cell position. Color interpolation is required
             * to retrieve the missing components. */
            ISC_CFA_Enable(1);
            ISC_CFA_Configure(iscObj->bayerPattern, 1);

            /* The White Balance (WB) module captures the data bus
             * from the PFE module, each Bayer color component (R,
             * Gr, B, Gb) can be manually adjusted using an offset
             * and a gain. */
            if (iscObj->whiteBalance.enableWB) {
                ISC_WB_Enable(1);
                ISC_WB_Set_Bayer_Pattern(iscObj->bayerPattern);
                ISC_WB_Set_Bayer_Color(iscObj->whiteBalance.redOffset, \
                                   iscObj->whiteBalance.greenRedOffset, \
                                   iscObj->whiteBalance.blueOffset, \
                                   iscObj->whiteBalance.greenBlueOffset, \
                                   iscObj->whiteBalance.redGain, \
                                   iscObj->whiteBalance.greenRedGain, \
                                   iscObj->whiteBalance.blueGain, \
                                   iscObj->whiteBalance.greenBlueGain);
            }

            DRV_ISC_Configure_Scaler(iscObj);

            if (iscObj->cbc.enableCBC) {
                ISC_CBC_Enable(1);
                ISC_CBC_Configure(0, 0, iscObj->cbc.bright, iscObj->cbc.contrast);
#ifdef ISC_CBHS_HUE_Msk
                ISC_CBHS_Configure(iscObj->cbc.hue, iscObj->cbc.saturation);
#endif
            }

            if (iscObj->colorCorrection) {
                ISC_CC_Enable(1);
                ISC_CC_Configure(iscObj->colorCorrection);
            }

            if (iscObj->layout == ISC_LAYOUT_YC422SP ||
                    iscObj->layout == ISC_LAYOUT_YC420SP ||
                    iscObj->layout == ISC_LAYOUT_YC422P ||
                    iscObj->layout == ISC_LAYOUT_YC420P ||
                    iscObj->layout == ISC_LAYOUT_YUY2) {
                /* By converting an image from RGB to YCbCr
                 * color space, it is possible to separate Y,
                 * Cb and Cr information. */
                ISC_CSC_Enable(1);
                ISC_CSC_Configure(iscObj->colorSpace);
                /* The color space conversion output stream is
                 * a full-bandwidth YCbCr 4:4:4 signal. The
                 * chrominance subsampling divides the
                 * horizontal chrominance sampling rate by
                 * two */
                ISC_Sub422_Enable(1);
                if (iscObj->layout == ISC_LAYOUT_YC420SP ||
                        iscObj->layout == ISC_LAYOUT_YC420P)
                    ISC_Sub420_Configure(1, 0);
            }
            break;

        case DRV_IMAGE_SENSOR_YUV_422:
        case DRV_IMAGE_SENSOR_RGB:
        case DRV_IMAGE_SENSOR_JPEG:
            ISC_CFA_Enable(0);
            ISC_WB_Enable(0);
            ISC_Gamma_Enable(0, 0, 0);
            ISC_CSC_Enable(0);
            ISC_CSC_Enable(0);
            ISC_Sub422_Enable(0);
            ISC_Sub420_Configure(0, 0);
            break;
        default:
            /* TODO */
            break;
    }

    ISC_RLP_Configure(iscObj->rlpMode, 0);

    DRV_ISC_Configure_DMA(iscObj);

    DRV_ISC_Configure_Histogram(iscObj);

    iscObj->frameIndex = 0;

    ISC_Update_Profile();

    if (iscObj->enableHistogram) {
        ISC_Enable_Interrupt(ISC_INTEN_VD(1) | ISC_INTEN_DDONE(1) | ISC_INTEN_HISDONE(1));
    } else {
        ISC_Enable_Interrupt(ISC_INTEN_VD(1) | ISC_INTEN_DDONE(1));
    }

    ISC_Interrupt_Status();

    return ISC_SUCCESS;
}

static void DRV_ISC_AWB_CountUp(void) {
    uint32_t i;
    uint32_t hist_min = 0, hist_max = 0;
    uint32_t* buf = (uint32_t*) DrvISCObj.histogramBuffer;

    if (buf) {
        DrvISCObj.awb.count[DrvISCObj.awb.op_mode] = 0;
        for (i = 0; i < ISC_HIST_ENTRIES; i++) {
            if (*buf) {
                if (!hist_min)
                    hist_min = i;
                DrvISCObj.awb.white_count[DrvISCObj.awb.op_mode] = 0;
                hist_max = i;
            }
            DrvISCObj.awb.count[DrvISCObj.awb.op_mode] += (*buf) * i;
            DrvISCObj.awb.white_count[DrvISCObj.awb.op_mode] += (*buf++) * i;
        }
        if (!hist_min)
            hist_min = 1;

        DrvISCObj.awb.hist_min[DrvISCObj.awb.op_mode] = hist_min;
        DrvISCObj.awb.hist_max[DrvISCObj.awb.op_mode] = hist_max;
    }
}

static void DRV_ISC_AWB_Update(void) {
    int32_t wb_offset[ISC_BAYER_COUNT] = {0};
    uint32_t gain[ISC_BAYER_COUNT] = {0};
    uint32_t a_max = 0;
    uint64_t hist_avg = 0;
    uint32_t k[ISC_BAYER_COUNT] = {0};
    uint32_t x[ISC_BAYER_COUNT] = {0};

    a_max = DrvISCObj.awb.hist_max[0];
    for (uint8_t b = 1; b < ISC_BAYER_COUNT; b++) {
        if (DrvISCObj.awb.hist_max[b] > a_max)
            a_max = DrvISCObj.awb.hist_max[b];
    }
    hist_avg = (uint64_t) DrvISCObj.awb.count[ISC_HISTOGRAM_GR] + (uint64_t) DrvISCObj.awb.count[ISC_HISTOGRAM_GB];
    hist_avg >>= 1;
    for (uint8_t b = 0; b < ISC_BAYER_COUNT; b++) {
        if (DrvISCObj.awb.hist_min[b] != 0) {
            /* Convert 12 bits pipline to 9 bits. */
            wb_offset[b] = -((DrvISCObj.awb.hist_min[b] - 1) << 3);
        }
        k[b] = (hist_avg << 9) / DrvISCObj.awb.count[b];

        /* the a_max is the maximum value of Rhigh,Ghigh,Bhigh. We set the maximum values of the original image to be the thresholds, avoiding the over stretching. */
        x[b] = (a_max << 9) / (DrvISCObj.awb.hist_max[b] - DrvISCObj.awb.hist_min[b] + 1);
        x[b] = (x[b] > (3 << 9)) ? (3 << 9) : x[b];
        gain[b] = x[b] * k[b];
        gain[b] >>= 9;
    }

    ISC_WB_Set_Bayer_Color(wb_offset[ISC_HISTOGRAM_R],
            wb_offset[ISC_HISTOGRAM_GR],
            wb_offset[ISC_HISTOGRAM_B],
            wb_offset[ISC_HISTOGRAM_GB],
            gain[ISC_HISTOGRAM_R],
            gain[ISC_HISTOGRAM_GR],
            gain[ISC_HISTOGRAM_B],
            gain[ISC_HISTOGRAM_GB]);

    ISC_Update_Profile();
}

static void DRV_ISC_XDMA_Read_Histogram(void) {

    if (DrvISCObj.histogramBuffer)
        XDMAC_ChannelTransfer(DrvISCObj.awb.hist_dma.channel,
            (void *) &ISC_REGS->ISC_HIS_ENTRY[0], (void *)
            DrvISCObj.histogramBuffer, ISC_HIST_ENTRIES * 4);
}

void DRV_ISC_AWB_Algo(void) {
    switch (DrvISCObj.awb.state) {
        case AWB_INIT:
            ISC_Histogram_Configure(DrvISCObj.awb.op_mode, ISC_WB_Get_Bayer_Pattern(), 0);
            ISC_Update_Profile();
            if (DrvISCObj.awb.op_mode != (ISC_REGS->ISC_HIS_CFG & ISC_HIS_CFG_MODE_Msk))
                break;

            ISC_Clear_Histogram_Table();
            DrvISCObj.awb.hist_dma.dma_histo_done = false;
            ISC_Update_Histogram_Table();
            DrvISCObj.awb.state = AWB_WAIT_HIS_READY;
            break;
        case AWB_WAIT_HIS_READY:
            if (!DrvISCObj.awb.hist_dma.dma_histo_ready)
                break;
            DrvISCObj.awb.hist_dma.dma_histo_ready = false;
            DRV_ISC_XDMA_Read_Histogram();
            DrvISCObj.awb.state = AWB_WAIT_DMA_READY;
            break;
        case AWB_WAIT_DMA_READY:
            if (!DrvISCObj.awb.hist_dma.dma_histo_done)
                break;
            DrvISCObj.awb.hist_dma.dma_histo_done = false;
            DRV_ISC_AWB_CountUp();
            DrvISCObj.awb.op_mode++;
            if (DrvISCObj.awb.op_mode < ISC_BAYER_COUNT) {
                DrvISCObj.awb.state = AWB_INIT;
            } else {
                DrvISCObj.awb.state = AWB_WAIT_ISC_PERFORMED;
            }
            break;
        case AWB_WAIT_ISC_PERFORMED:
            DRV_ISC_AWB_Update();
            DrvISCObj.awb.op_mode = 0;
            DrvISCObj.awb.state = AWB_INIT;
            break;
    }
}

DRV_ISC_OBJ* DRV_ISC_Initialize(void) {
    ISC_Initialize();

    return &DrvISCObj;
}

