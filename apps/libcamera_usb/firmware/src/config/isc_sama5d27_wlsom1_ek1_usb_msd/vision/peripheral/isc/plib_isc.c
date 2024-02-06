#include "device.h"
#include "plib_isc.h"
#include "system/time/sys_time.h"
#include "system/debug/sys_debug.h"

#include <stddef.h>
#include <stdbool.h>


static void DelayUS(int us)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayUS(us, &timer) != SYS_TIME_SUCCESS)
        return;

    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/*  Writing to the ISC_CTRLEN or ISC_CTRLDIS register requires a double domain
  * synchronization, avoid writing these registers when the ISC_CTRLSR.SIP
  * bit is asserted.
*/
static bool ISC_Sync_InProgress(void)
{
    return (ISC_REGS->ISC_CTRLSR & ISC_CTRLSR_SIP(1)) == ISC_CTRLSR_SIP(1);
}

/**
 * Start Capture to start a single shot capture or continuous shot capture.
 */
void ISC_Start_Capture(void)
{
    while (ISC_Sync_InProgress());
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_CAPTURE_Msk;
}

/**
 *  Stop capture at next Vertical Synchronization Detection.
 */
void ISC_Stop_Capture(void)
{
    while (ISC_Sync_InProgress());
    ISC_REGS->ISC_CTRLDIS = ISC_CTRLDIS_DISABLE_Msk;
}

/**
 *  Get ISC Control Status.
 */
uint32_t ISC_Ctrl_Status(void)
{
    return ISC_REGS->ISC_CTRLSR;
}

/**
 *  update profile.
 */
int ISC_Update_Profile(void)
{
    uint32_t sr;
    int counter = 1000;

    while (ISC_Sync_InProgress());
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_UPPRO_Msk;
    sr = ISC_REGS->ISC_CTRLSR;
    while ((sr & ISC_CTRLEN_UPPRO_Msk) && counter--)
    {
        DelayUS(2000);
        sr = ISC_REGS->ISC_CTRLSR;
    }
    if (counter < 0)
    {
        return -1;
    }
    return 0;
}

/**
 *  Perform software reset.
 */
void ISC_Software_Reset(void)
{
    while (ISC_Sync_InProgress());
    ISC_REGS->ISC_CTRLDIS = ISC_CTRLDIS_SWRST_Msk;
}


#ifdef ISC_PFE_CFG0_MIPI_Msk
/**
 *  Enable MIPI.
 * vmode: enable
 */
void ISC_PFE_MIPI_Enable(uint8_t en)
{
    if (en)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_MIPI(1);
    else
        ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_MIPI(1);
}
#endif

/**
 * configure PFE(Parallel Front End) video mode.
 * vmode: Parallel Front End Mode
 */
void ISC_PFE_Set_Video_Mode(uint32_t vmode)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_MODE_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= vmode;
}

/**
 *Set PFE(Parallel Front End) H/V synchronization polarity.
 * hpol: Horizontal Synchronization Polarity
 * vpol: Vertical Synchronization Polarity
 */
void ISC_PFE_Set_Sync_Polarity(uint32_t hpol, uint32_t vpol)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_HPOL(1);
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_VPOL(1);
    ISC_REGS->ISC_PFE_CFG0 |= hpol | vpol;
}

/**
 * Set PFE(Parallel Front End) pixel Clock polarity.
 * ppol: pixel Clock Polarity, The pixel stream is sampled on the
 *  rising or falling edge of the pixel Clock
 */
void ISC_PFE_Pixel_Polarity(uint32_t ppol)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_PPOL(1);
    ISC_REGS->ISC_PFE_CFG0 |= ppol ;
}

/**
 * Set PFE(Parallel Front End) field polarity.
 * fpol: Top/bottom field polarity configuration.
 */
void ISC_PFE_Field_Polarity(uint32_t fpol)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_FPOL(1);
    ISC_REGS->ISC_PFE_CFG0 |= fpol ;
}

/**
 * Enables/disable PFE(Parallel Front End) cropping
 * enable_column: Column Cropping enable/disable(1/0)
 * enable_row: Row Cropping enable/disable(1/0)
 */
void ISC_PFE_Enable_Crop(uint8_t enable_column, uint8_t enable_row)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_COLEN(1);
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_ROWEN(1);
    if (enable_column)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_COLEN(1);
    if (enable_row)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_ROWEN(1);
}

/**
 * Set PFE(Parallel Front End) Bits Per Sample.
 * bps: Bits Per Sample.
 */
void ISC_PFE_Set_BPS(uint32_t bps)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_BPS_Msk;
    ISC_REGS->ISC_PFE_CFG0 |= bps ;
}

/**
 * Set PFE(Parallel Front End)in continuous mode
 */
void ISC_PFE_Enable_Video_Mode(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_CONT(1);
    else
        ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_CONT(1);
}

/**
 * Set PFE(Parallel Front End) gated Clock.
 * enable: enable/disable gated Clock.
 */
void ISC_PFE_Gated_Clock(uint8_t enable)
{
    ISC_REGS->ISC_PFE_CFG0 &= ~ISC_PFE_CFG0_GATED(1);
    if (enable)
        ISC_REGS->ISC_PFE_CFG0 |= ISC_PFE_CFG0_GATED(1);
}
/**
 * Configure PFE(Parallel Front End) cropping area.
 * Hstart: Horizontal starting position of the cropping area
 * Hend: Horizontal ending position of the cropping area
 * Vstart: Vertical starting position of the cropping area
 * Hend: Vertical ending position of the cropping area
 */
void ISC_PFE_Crop_Area(uint32_t hstart, uint32_t hend,
                       uint32_t vstart, uint32_t vend)
{
    ISC_REGS->ISC_PFE_CFG1 = ISC_PFE_CFG1_COLMIN(hstart) | ISC_PFE_CFG1_COLMAX(hend);
    ISC_REGS->ISC_PFE_CFG2 = ISC_PFE_CFG2_ROWMIN(vstart) | ISC_PFE_CFG2_ROWMAX(vend);
}


#ifdef ISC_DPC_CTRL_Msk
/**
 *  Black Level Correction.
 */
void ISC_Enable_Black_Level(uint8_t enable, uint16_t level)
{
    if (enable)
    {
        ISC_REGS->ISC_DPC_CTRL |= ISC_DPC_CTRL_BLCEN(1);
        ISC_REGS->ISC_DPC_CFG |= ISC_DPC_CFG_BLOFST(level);
    }
    else
    {
        ISC_REGS->ISC_DPC_CTRL &= ~ISC_DPC_CTRL_BLCEN(1);
    }
}

/**
 *  Green Correction.
 */
void ISC_Enable_Green_Correction(uint8_t enable, uint8_t clip)
{
    if (enable)
    {
        ISC_REGS->ISC_DPC_CTRL |= ISC_DPC_CTRL_GDCEN(1);
        ISC_REGS->ISC_DPC_CFG |= ISC_DPC_CFG_GDCCLP(clip);
    }
    else
    {
        ISC_REGS->ISC_DPC_CTRL &= ~ISC_DPC_CTRL_GDCEN(1);
    }
}
#endif

/**
 *  Enables ISP Clock.
 */
void ISC_Enable_ISP_Clock(void)
{
    ISC_REGS->ISC_CLKEN = ISC_CLKEN_ICEN(1);
}

/**
 *  Disables ISP Clock.
 */
void ISC_Disable_ISP_Clock(void)
{
    ISC_REGS->ISC_CLKDIS = ISC_CLKDIS_ICDIS_Msk;
}

/**
 *  Software reset ISP Clock.
 */
void ISC_Reset_ISP_Clock(void)
{
    ISC_REGS->ISC_CLKDIS = ISC_CLKDIS_ICSWRST_Msk;
}


#if defined(ISC_CLKCFG_ICDIV_Msk) && defined(ISC_CLKCFG_ICSEL_Msk)
/**
 * Configure the ISP clock.
 * ispClockDiv ISP Clock Divider.
 *  ispClockSelection ISP Clock Selection.
			0: HCLOCK is selected.
			1: GCK is selected.
 */
void ISC_Configure_ISP_Clock(uint32_t ispClockDiv, uint32_t ispClockSelection)
{
    uint32_t clkcfg = ISC_REGS->ISC_CLKCFG & ~(ISC_CLKCFG_ICDIV_Msk | ISC_CLKCFG_ICSEL_Msk);

    ISC_REGS->ISC_CLKCFG = clkcfg |
                           ISC_CLKCFG_ICDIV(ispClockDiv) |
                           ISC_CLKCFG_ICSEL(ispClockSelection);

    while (ISC_Sync_InProgress());
}
#endif

/**
 * Configure the Master Clock.
 * masterClockDiv Master Clock Divider.
 * masterClockSelection Master Clock Selection.
			0: HCLOCK is selected.
			1: GCK is selected.
			2: 480-MHz system Clock is selected.
 */
void ISC_Configure_Master_Clock(uint32_t masterClockDiv, uint32_t masterClockSelection)
{
#ifdef ISC_CLKCFG_MCSEL_Msk
    uint32_t clkcfg = ISC_REGS->ISC_CLKCFG & ~(ISC_CLKCFG_MCDIV_Msk | ISC_CLKCFG_MCSEL_Msk);

    ISC_REGS->ISC_CLKCFG = clkcfg |
                           ISC_CLKCFG_MCDIV(masterClockDiv) |
                           ISC_CLKCFG_MCSEL(masterClockSelection);

    while (ISC_Sync_InProgress());
#else
    ISC_REGS->ISC_CLKCFG = ISC_CLKCFG_MCDIV(masterClockDiv);
#endif
}

/**
 *  Enables master Clock.
 */
void ISC_Enable_Master_Clock(void)
{
    ISC_REGS->ISC_CLKEN = ISC_CLKEN_MCEN(1);
}

/**
 *  Disables master Clock.
 */
void ISC_Disable_Master_Clock(void)
{
    ISC_REGS->ISC_CLKDIS = ISC_CLKDIS_MCDIS_Msk;
}

/**
 *  Software reset master Clock.
 */
void ISC_Reset_Master_Clock(void)
{
    ISC_REGS->ISC_CLKDIS = ISC_CLKDIS_MCSWRST_Msk;
}

/**
 *  Return ISC Clock Status.
 */
uint32_t ISC_Clock_status(void)
{
    return ISC_REGS->ISC_CLKSR;
}

/*------------------------------------------
 *         Interrupt functions
 *----------------------------------------*/
/**
 *  Enable ISC interrupt
 *  flag of interrupt to Enable
 */
void ISC_Enable_Interrupt(uint32_t flag)
{
    ISC_REGS->ISC_INTEN = flag;
}

/**
 *  Disable ISC interrupt
 *  flag of interrupt to disable
 */
void ISC_Disable_Interrupt(uint32_t flag)
{
    ISC_REGS->ISC_INTDIS = flag;
}

/**
 *  Return ISC status register
 */
uint32_t ISC_Interrupt_Status(void)
{
    return ISC_REGS->ISC_INTSR;
}

/*------------------------------------------
 *         White Balance functions
 *----------------------------------------*/
/**
 *  Enables/disable White Balance.
 */
void ISC_WB_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_WB_CTRL = ISC_WB_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_WB_CTRL = 0;
}

/**
 *  White Balance Bayer Configuration (Pixel Color Pattern).
 */
void ISC_WB_Set_Bayer_Pattern(uint8_t pattern)
{
    ISC_REGS->ISC_WB_CFG = pattern;
}

/**
 *  Get White Balance Bayer Configuration (Pixel Color Pattern).
 */
uint8_t ISC_WB_Get_Bayer_Pattern(void)
{
    return ISC_REGS->ISC_WB_CFG;
}
/**
 * Adjust White Balance with color component.
 * r_offset  Offset Red Component (signed 13 bits 1:12:0)
 * gr_offset Offset Green Component for Red Row (signed 13 bits 1:12:0)
 * b_offset  Offset Blue Component (signed 13 bits, 1:12:0)
 * gb_offset Offset Green Component for Blue Row (signed 13 bits, 1:12:0)
 * r_gain    Red Component Gain (unsigned 13 bits, 0:4:9)
 * gr_gain   Green Component (Red row) Gain (unsigned 13 bits, 0:4:9)
 * b_gain    Blue Component Gain (unsigned 13 bits, 0:4:9)
 * gb_gain   Green Component (Blue row) Gain (unsigned 13 bits, 0:4:9)
 */
void ISC_WB_Set_Bayer_Color(uint32_t r_offset, uint32_t gr_offset,
                            uint32_t b_offset, uint32_t gb_offset,
                            uint32_t r_gain, uint32_t gr_gain,
                            uint32_t b_gain, uint32_t gb_gain)
{
    ISC_REGS->ISC_WB_O_RGR = ISC_WB_O_RGR_ROFST(r_offset) |
                             ISC_WB_O_RGR_GROFST(gr_offset);
    ISC_REGS->ISC_WB_O_BGB = ISC_WB_O_BGB_BOFST(b_offset) |
                             ISC_WB_O_BGB_GBOFST(gb_offset);
    ISC_REGS->ISC_WB_G_RGR = ISC_WB_G_RGR_RGAIN(r_gain) |
                             ISC_WB_G_RGR_GRGAIN(gr_gain);
    ISC_REGS->ISC_WB_G_BGB = ISC_WB_G_BGB_BGAIN(b_gain) |
                             ISC_WB_G_BGB_GBGAIN(gb_gain);
}

/**
 *  Enables/disable Color Filter Array Interpolation.
 */
void ISC_CFA_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_CFA_CTRL = ISC_CFA_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_CFA_CTRL = 0;
}

/**
 * Configure color filter array interpolation.
 * pattern Color Filter Array Pattern
 * edge Edge Interpolation
			0: Edges are not interpolated.
			1: Edge interpolation is performed.
 */
void ISC_CFA_Configure(uint8_t pattern, uint8_t edge)
{
    ISC_REGS->ISC_CFA_CFG = ISC_CFA_CFG_BAYCFG(pattern) | ISC_CFA_CFG_EITPOL(edge);
}

/**
 *  Enables/disable Color Correction.
 */
void ISC_CC_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_CC_CTRL = ISC_CC_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_CC_CTRL = 0;
}

/**
 * Color correction with color component.
 * cc Pointer to structure _color_correct
 */
void ISC_CC_Configure(PLIB_ISC_COLOR_CORRECT* cc)
{
    ISC_REGS->ISC_CC_RR_RG = ISC_CC_RR_RG_RRGAIN(cc->rr_gain) | ISC_CC_RR_RG_RGGAIN(cc->rg_gain);
    ISC_REGS->ISC_CC_RB_OR = ISC_CC_RB_OR_RBGAIN(cc->rb_gain) | ISC_CC_RB_OR_ROFST(cc->r_offset);
    ISC_REGS->ISC_CC_GR_GG = ISC_CC_GR_GG_GRGAIN(cc->gr_gain) | ISC_CC_GR_GG_GGGAIN(cc->gg_gain);
#ifdef ISC_CC_GB_OG_ROFST
    ISC_REGS->ISC_CC_GB_OG = ISC_CC_GB_OG_GBGAIN(cc->gb_gain) | ISC_CC_GB_OG_ROFST(cc->g_offset);
#endif
#ifdef ISC_CC_GB_OG_GOFST
    ISC_REGS->ISC_CC_GB_OG = ISC_CC_GB_OG_GBGAIN(cc->gb_gain) | ISC_CC_GB_OG_GOFST(cc->g_offset);
#endif
    ISC_REGS->ISC_CC_BR_BG = ISC_CC_BR_BG_BRGAIN(cc->br_gain) | ISC_CC_BR_BG_BGGAIN(cc->bg_gain);
    ISC_REGS->ISC_CC_BB_OB = ISC_CC_BB_OB_BBGAIN(cc->bb_gain) | ISC_CC_BB_OB_BOFST(cc->b_offset);
}

/**
 * Enables/disable Gamma Correction with giving channels.
 * enable 1: Enable, 0: disable
 * channels ISC_GAM_CTRL_BENABLE/ISC_GAM_CTRL_GENABLE/ISC_GAM_CTRL_RENABLE
 */
void ISC_Gamma_Enable(uint8_t enable, uint8_t channels, uint8_t bipart_enable)
{
    if (enable)
#ifdef ISC_GAM_CTRL_BIPART_Msk
        ISC_REGS->ISC_GAM_CTRL |= ISC_GAM_CTRL_ENABLE(1) | channels | (bipart_enable ? ISC_GAM_CTRL_BIPART(1) : 0);
#else
        ISC_REGS->ISC_GAM_CTRL |= ISC_GAM_CTRL_ENABLE(1) | channels;
#endif
    else
        ISC_REGS->ISC_GAM_CTRL = 0;
}

/**
 *  Configure gamma correction with give table.
 * r_gam_constant Pointer to red Color Constant instance (64 half-word).
 * r_gam_slope    Pointer to red Color Slope instance (64 half-word).
 * g_gam_constant Pointer to green Color Constant instance (64 half-word).
 * g_gam_slope    Pointer to green Color Slope instance (64 half-word).
 * b_gam_constant Pointer to blue Color Constant instance (64 half-word).
 * b_gam_slope    Pointer to blue Color Slope instance (64 half-word).
 */
void ISC_Gamma_Configure(uint16_t* r_gam_constant, uint16_t* r_gam_slope,
                         uint16_t* g_gam_constant, uint16_t* g_gam_slope,
                         uint16_t* b_gam_constant, uint16_t* b_gam_slope)
{
    uint8_t i;
    for (i = 0; i < 64 ; i++)
    {
        ISC_REGS->ISC_GAM_BENTRY[i] = ISC_GAM_BENTRY_BCONSTANT(b_gam_constant[i]) |
                                      ISC_GAM_BENTRY_BSLOPE(b_gam_slope[i]);
        ISC_REGS->ISC_GAM_GENTRY[i] = ISC_GAM_GENTRY_GCONSTANT(b_gam_constant[i]) |
                                      ISC_GAM_GENTRY_GSLOPE(b_gam_slope[i]);
        ISC_REGS->ISC_GAM_RENTRY[i] = ISC_GAM_RENTRY_RCONSTANT(b_gam_constant[i]) |
                                      ISC_GAM_RENTRY_RSLOPE(b_gam_slope[i]);
    }
}

#ifdef ISC_VHXS_CTRL_Msk
/**
 *  Enables/disable Horizontal or Vertical Scaler
 */
void ISC_Scaler_Enable(uint8_t h_enable, uint8_t v_enable)
{
    ISC_REGS->ISC_VHXS_CTRL &= (~ISC_VHXS_CTRL_Msk);
    if (h_enable)
        ISC_REGS->ISC_VHXS_CTRL |= ISC_VHXS_CTRL_HXSEN(1);
    if (v_enable)
        ISC_REGS->ISC_VHXS_CTRL |= ISC_VHXS_CTRL_VXSEN(1);
}
void ISC_Scaler_Source_Size(uint16_t hxsize, uint16_t vxsize)
{
    ISC_REGS->ISC_VHXS_SS = ISC_VHXS_SS_XS(hxsize) | ISC_VHXS_SS_YS(vxsize);
}

void ISC_Scaler_Destination_Size(uint16_t hxsize, uint16_t vxsize)
{
    ISC_REGS->ISC_VHXS_DS = ISC_VHXS_DS_XD(hxsize) | ISC_VHXS_DS_YD(vxsize);
}
#endif

/**
 *  Enables/disable Color Space Conversion.
 */
void ISC_CSC_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_CSC_CTRL = ISC_CSC_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_CSC_CTRL = 0;
}

/**
 * Color space convert with color space component.
 * cs Pointer to structure _color_space
 */
void ISC_CSC_Configure(PLIB_ISC_COLOR_SPACE* cs)
{
    ISC_REGS->ISC_CSC_YR_YG = ISC_CSC_YR_YG_YRGAIN(cs->yr_gain) |
                              ISC_CSC_YR_YG_YGGAIN(cs->yg_gain);
    ISC_REGS->ISC_CSC_YB_OY = ISC_CSC_YB_OY_YBGAIN(cs->yb_gain) |
                              ISC_CSC_YB_OY_YOFST(cs->y_offset);
    ISC_REGS->ISC_CSC_CBR_CBG = ISC_CSC_CBR_CBG_CBRGAIN(cs->cbr_gain) |
                                ISC_CSC_CBR_CBG_CBGGAIN(cs->cbg_gain);
    ISC_REGS->ISC_CSC_CBB_OCB = ISC_CSC_CBB_OCB_CBBGAIN(cs->cbb_gain) |
                                ISC_CSC_CBB_OCB_CBOFST(cs->cb_offset);
    ISC_REGS->ISC_CSC_CRR_CRG = ISC_CSC_CRR_CRG_CRRGAIN(cs->crr_gain) |
                                ISC_CSC_CRR_CRG_CRGGAIN(cs->crg_gain);
    ISC_REGS->ISC_CSC_CRB_OCR = ISC_CSC_CRB_OCR_CRBGAIN(cs->crb_gain) |
                                ISC_CSC_CRB_OCR_CROFST(cs->cr_offset);
}

/**
 *  Enables/disable contrast and brightness control.
 */
void ISC_CBC_Enable(uint8_t enable)
{
#ifdef ISC_CBHS_CTRL_Msk
    if (enable)
        ISC_REGS->ISC_CBHS_CTRL = ISC_CBHS_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_CBHS_CTRL = 0;
#else
    if (enable)
        ISC_REGS->ISC_CBC_CTRL = ISC_CBC_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_CBC_CTRL = 0;
#endif
}

/**
 * Configure Contrast and brightness with give parameter.
 * ccir656 CCIR656 Stream Enable.
				0: Raw mode
				1: CCIR mode
 * byte_order CCIR656 Byte Ordering.
 * brightness Brightness Control (signed 11 bits 1:10:0).
 * contrast Contrast (signed 12 bits 1:3:8).
 */
void ISC_CBC_Configure(uint8_t ccir656, uint8_t byte_order,
                       uint32_t brightness, uint32_t contrast)
{
#ifdef ISC_CBHS_CFG_Msk
    if (ccir656)
        ISC_REGS->ISC_CBHS_CFG = ISC_CBHS_CFG_CCIR(1) | byte_order;
    else
        ISC_REGS->ISC_CBHS_CFG = 0;
#else
    if (ccir656)
        ISC_REGS->ISC_CBC_CFG = ISC_CBC_CFG_CCIR(1) | byte_order;
    else
        ISC_REGS->ISC_CBC_CFG = 0;
#endif

#ifdef ISC_CBHS_BRIGHT_Msk
    ISC_REGS->ISC_CBHS_BRIGHT = ISC_CBHS_BRIGHT_BRIGHT(brightness);
#else
    ISC_REGS->ISC_CBC_BRIGHT = ISC_CBC_BRIGHT_BRIGHT(brightness);
#endif

#ifdef ISC_CBHS_CONT_CONTRAST_Msk
    ISC_REGS->ISC_CBHS_CONT = ISC_CBHS_CONT_CONTRAST(contrast);
#else
    ISC_REGS->ISC_CBC_CONTRAST = ISC_CBC_CONTRAST_CONTRAST(contrast);
#endif
}

#ifdef ISC_CBHS_HUE_Msk
/**
 * Configure Saturation and hue with give parameter.
 * Hue Control (unsigned 9 bits 0:9:0).
 * Saturation Contrast (unsigned 12 bits 0:8:4).
 */
void ISC_CBHS_Configure(uint32_t hue, uint32_t saturation)
{
    ISC_REGS->ISC_CBHS_HUE = ISC_CBHS_HUE_HUE(hue);
    ISC_REGS->ISC_CBHS_SAT = ISC_CBHS_SAT_SATURATION(saturation);
}
#endif

/*------------------------------------------
 *       Sub-sampling functions
 *----------------------------------------*/

/**
 *  Enables/disable 4:4:4 to 4:2:2 Chrominance Horizontal Subsampling Filter Enable.
 */
void ISC_Sub422_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_SUB422_CTRL = ISC_SUB422_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_SUB422_CTRL = 0;
}

/**
 * Configure Subsampling 4:4:4 to 4:2:2 with giving value.
 * ccir656 CCIR656 Stream Enable.
				0: Raw mode
				1: CCIR mode
 * byte_order CCIR656 Byte Ordering.
 * lpf Low Pass Filter Selection.
 */
void ISC_Sub422_Configure(uint8_t ccir656, uint8_t byte_order, uint8_t lpf)
{
    if (ccir656)
        ISC_REGS->ISC_SUB422_CFG = ISC_SUB422_CFG_CCIR(1) | byte_order;
    else
        ISC_REGS->ISC_SUB422_CFG = 0;
    ISC_REGS->ISC_SUB422_CFG &= ~ISC_SUB422_CFG_FILTER_Msk;
    ISC_REGS->ISC_SUB422_CFG |= lpf;
}

/**
 *  Configure 4:2:2 to 4:2:0 Vertical Subsampling Filter Enable
		(Center Aligned) with giving value.
 * enable Subsampler enable.
				0: disabled
				1: enable
 * filter Interlaced or Progressive Chrominance Filter.
		0: Progressive filter {0.5, 0.5}
		1: Field-dependent filter, top field filter is {0.75, 0.25},
			bottom field filter is {0.25, 0.75}
 */
void ISC_Sub420_Configure(uint8_t enable, uint8_t filter)
{
    if (enable)
    {
        ISC_REGS->ISC_SUB420_CTRL = ISC_SUB420_CTRL_ENABLE(1);
        if (filter)
            ISC_REGS->ISC_SUB420_CTRL |= ISC_SUB420_CTRL_FILTER(1);
    }
    else
    {
        ISC_REGS->ISC_SUB420_CTRL = 0;
    }
}

/*------------------------------------------
 * Rounding, Limiting and Packing functions
 *----------------------------------------*/

/**
 * Configure Rounding, Limiting and Packing Mode.
 * rlp_mode Rounding, Limiting and Packing Mode.
 * alpha Alpha Value for Alpha-enable RGB Mode.
 */
void ISC_RLP_Configure(uint8_t rlp_mode, uint8_t alpha)
{
    ISC_REGS->ISC_RLP_CFG &= ~ISC_RLP_CFG_MODE_Msk;
    ISC_REGS->ISC_RLP_CFG |= rlp_mode;
    if (alpha)
        ISC_REGS->ISC_RLP_CFG |= ISC_RLP_CFG_ALPHA(alpha);
    //
    //    ISC_REGS->ISC_RLP_CFG |= ISC_RLP_CFG_LSH(1);
    //
    //    ISC_REGS->ISC_RLP_CFG |= ISC_RLP_CFG_REP(1);
}

/*------------------------------------------
 *         Histogram functions
 *----------------------------------------*/

/**
 * Enables/disable Histogram
 */
void ISC_Histogram_Enable(uint8_t enable)
{
    if (enable)
        ISC_REGS->ISC_HIS_CTRL = ISC_HIS_CTRL_ENABLE(1);
    else
        ISC_REGS->ISC_HIS_CTRL = 0;
}

/**
 * Configure Histogram.
 * mode Histogram Operating Mode.
 * baySel Bayer Color Component Selection.
 * reset Histogram Reset After Read
			0: Reset after read mode is disabled
			1: Reset after read mode is enabled.
 */
void ISC_Histogram_Configure(uint8_t mode, uint8_t bay_sel, uint8_t reset)
{
    ISC_REGS->ISC_HIS_CFG = ISC_HIS_CFG_MODE(mode) | ISC_HIS_CFG_BAYSEL(bay_sel);
    if (reset)
        ISC_REGS->ISC_HIS_CFG |= ISC_HIS_CFG_RAR(1);
}

/**
*  update histogram table.
*/
void ISC_Update_Histogram_Table(void)
{
    while ((ISC_REGS->ISC_CTRLSR & ISC_CTRLSR_HISREQ(1)) == ISC_CTRLSR_HISREQ(1));
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_HISREQ_Msk;
}

/**
 *   clear histogram table.
 */
void ISC_Clear_Histogram_Table(void)
{
    while (ISC_Sync_InProgress());
    ISC_REGS->ISC_CTRLEN = ISC_CTRLEN_HISCLR_Msk;
}

/**
 * Configure ISC DMA input mode.
 * mode Operating Mode.
 */
void ISC_DMA_Configure_Input_Mode(uint32_t mode)
{
    ISC_REGS->ISC_DCFG = mode;
}

/**
 * Configure ISC DMA with giving entry.
 * descEntry entry of DMA descriptor VIEW.
 */
void ISC_DMA_Configure_Desc_Entry(uint32_t desc_entry)
{
    ISC_REGS->ISC_DNDA = desc_entry;
}

/**
 * Enable ISC DMA with giving view.
 * ctrlSetting for DMA descriptor VIEW.
 */
void ISC_DMA_Enable(uint32_t ctrl)
{
    ISC_REGS->ISC_DCTRL = ctrl;
}

/**
 * Configure ISC DMA start address.
 * channel channel number.
 * address address for giving channel.
 * stride stride for giving channel.
 */
void ISC_DMA_Address(uint8_t channel, uint32_t address, uint32_t stride)
{
#ifdef ISC_SUB0_NUMBER

    ISC_REGS->ISC_SUB0[channel].ISC_DAD = address;
    ISC_REGS->ISC_SUB0[channel].ISC_DST = stride;

#else
    switch (channel)
    {
    case 0:
        ISC_REGS->ISC_DAD0 = address;
        ISC_REGS->ISC_DST0 = stride;
        break;
    case 1:
        ISC_REGS->ISC_DAD1 = address;
        ISC_REGS->ISC_DST1 = stride;
        break;
    case 2:
        ISC_REGS->ISC_DAD2 = address;
        ISC_REGS->ISC_DST2 = stride;
        break;
    default:
        break;
    }
#endif
}

void ISC_Initialize(void)
{
    int counter = 1000;

    ISC_Configure_Master_Clock(PLIB_ISC_MCK_DIV_VAL, PLIB_ISC_MCK_SEL_VAL);
    ISC_Enable_Master_Clock();
#if defined(ISC_CLKCFG_ICDIV_Msk) && defined(ISC_CLKCFG_ICSEL_Msk)
    ISC_Configure_ISP_Clock(PLIB_ISC_ISP_CLK_DIV_VAL, PLIB_ISC_ISP_CLK_SEL_VAL);
#endif
    ISC_Enable_ISP_Clock();
    while ((ISC_REGS->ISC_CLKSR != ((ISC_CLKSR_ICSR_Msk) | (ISC_CLKSR_MCSR_Msk))) && counter--);

    /* Disable capture in ISC*/
    ISC_Stop_Capture();
}
