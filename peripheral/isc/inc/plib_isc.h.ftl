#ifndef PLIB_ISC_H
#define PLIB_ISC_H

#include <stdint.h>

/** color correction components structure */
typedef struct
{
    /** Red Component Offset (signed 13 bits, 1:12:0) */
    uint16_t r_offset;
    /** Green Component Offset (signed 13 bits, 1:12:0)*/
    uint16_t g_offset;
    /** Green Component Offset (signed 13 bits, 1:12:0)*/
    uint16_t b_offset;
    /** Red Gain for Red Component (signed 12 bits, 1:3:8)*/
    uint16_t rr_gain;
    /** Green Component (Red row) Gain (unsigned 13 bits, 0:4:9)*/
    uint16_t rg_gain;
    /** Blue Gain for Red Component (signed 12 bits, 1:3:8)*/
    uint16_t rb_gain;
    /** Green Gain for Green Component (signed 12 bits, 1:3:8)*/
    uint16_t gg_gain;
    /** Red Gain for Green Component (signed 12 bits, 1:3:8)*/
    uint16_t gr_gain;
    /** Blue Gain for Green Component (signed 12 bits, 1:3:8)*/
    uint16_t gb_gain;
    /** Green Gain for Blue Component (signed 12 bits, 1:3:8) */
    uint16_t bg_gain;
    /** Red Gain for Blue Component (signed 12 bits, 1:3:8) */
    uint16_t br_gain;
    /** Blue Gain for Blue Component (signed 12 bits, 1:3:8)*/
    uint16_t bb_gain;
} PLIB_ISC_COLOR_CORRECT;

/** color space convertion components structure */
typedef struct
{
    /** Red Gain for Luminance (signed 12 bits 1:3:8) */
    uint16_t yr_gain;
    /** Green Gain for Luminance (signed 12 bits 1:3:8)*/
    uint16_t yg_gain;
    /** Blue Gain for Luminance Component (12 bits signed 1:3:8)*/
    uint16_t yb_gain;
    /** Luminance Offset (11 bits signed 1:10:0)*/
    uint16_t y_offset;
    /** Green Gain for Blue Chrominance (signed 12 bits 1:3:8)*/
    uint16_t cbr_gain;
    /** Red Gain for Blue Chrominance (signed 12 bits, 1:3:8)*/
    uint16_t cbg_gain;
    /** Blue Gain for Blue Chrominance (signed 12 bits 1:3:8)*/
    uint16_t cbb_gain;
    /** Blue Chrominance Offset (signed 11 bits 1:10:0)*/
    uint16_t cb_offset;
    /** Red Gain for Red Chrominance (signed 12 bits 1:3:8)*/
    uint16_t crr_gain;
    /** Green Gain for Red Chrominance (signed 12 bits 1:3:8)*/
    uint16_t crg_gain;
    /** Blue Gain for Red Chrominance (signed 12 bits 1:3:8)*/
    uint16_t crb_gain;
    /** Red Chrominance Offset (signed 11 bits 1:10:0)*/
    uint16_t cr_offset;
} PLIB_ISC_COLOR_SPACE;

void ISC_Start_Capture(void);
void ISC_Stop_Capture(void);
uint32_t ISC_Ctrl_Status(void);
int ISC_Update_Profile(void);
void ISC_Software_Reset(void);

#ifdef ISC_PFE_CFG0_MIPI_Msk
void ISC_PFE_MIPI_Enable(uint8_t en);
#endif
void ISC_PFE_Set_Video_Mode(uint32_t vmode);
void ISC_PFE_Set_Sync_Polarity(uint32_t hpol, uint32_t vpol);
void ISC_PFE_Set_Pixel_Polarity(uint32_t ppol);
void ISC_PFE_Set_Field_Polarity(uint32_t fpol);
void ISC_PFE_Set_Gated_Clock(uint8_t en);
void ISC_PFE_Set_Cropping_Enable(uint8_t enable_column,
                                 uint8_t enable_row);
void ISC_PFE_Set_BPS(uint32_t bps);
void ISC_PFE_Enable_Video_Mode(uint8_t enable);
void ISC_PFE_Set_Cropping_Area(uint32_t hstart, uint32_t hend,
                               uint32_t vstart, uint32_t vend);

#if defined(ISC_CLKCFG_ICDIV_Msk) && defined(ISC_CLKCFG_ICSEL_Msk)
void ISC_Configure_ISP_Clock(uint32_t isp_clk_div,
                             uint32_t isp_clk_sel);
#endif
void ISC_Configure_ISP_Clock(uint32_t ispClockDiv,
                             uint32_t ispClockSelection);
void ISC_Enable_ISP_Clock(void);
void ISC_Disable_ISP_Clock(void);
void ISC_Reset_ISP_Clock(void);

void ISC_Configure_Master_Clock(uint32_t master_clk_div,
                                uint32_t master_clk_sel);
void ISC_Enable_Master_Clock(void);
void ISC_disable_Master_Clock(void);
void ISC_reset_Master_Clock(void);
uint32_t ISC_get_Clock_Status(void);


void ISC_Enable_Interrupt(uint32_t flag);
void ISC_Disable_Interrupt(uint32_t flag);
uint32_t ISC_Interrupt_Status(void);


void ISC_WB_Enable(uint8_t enabled);
void ISC_WB_Set_Bayer_Pattern(uint8_t pattern);
uint8_t ISC_WB_Get_Bayer_Pattern(void);
void ISC_WB_Set_Bayer_Color(uint32_t r_offset, uint32_t gr_offset,
                            uint32_t b_offset, uint32_t gb_offset,
                            uint32_t r_gain, uint32_t gr_gain,
                            uint32_t b_gain, uint32_t gb_gain);



void ISC_CFA_Enable(uint8_t enabled);
void ISC_CFA_Configure(uint8_t pattern, uint8_t edge);



void ISC_CC_Enable(uint8_t enabled);
void ISC_CC_Configure(PLIB_ISC_COLOR_CORRECT* cc);


void ISC_Gamma_Enable(uint8_t enabled, uint8_t channels, uint8_t bipart_Enable);
void ISC_Gamma_Configure(uint16_t* r_gam_constant, uint16_t* r_gam_slope,
                         uint16_t* g_gam_constant, uint16_t* g_gam_slope,
                         uint16_t* b_gam_constant, uint16_t* b_gam_slope);

#ifdef ISC_VHXS_CTRL_Msk
void ISC_Scaler_Enable(uint8_t h_enable, uint8_t v_enable);
void ISC_Scaler_Source_Size(uint16_t hxsize, uint16_t vxsize);
void ISC_Scaler_Destination_Size(uint16_t hxsize, uint16_t vxsize);
void ISC_Scaler_Vertical_Scaling_Factor(uint32_t vxfact);
void ISC_Scaler_Horizontal_Scaling_Factor(uint32_t hxfact);
void ISC_Scaler_Configure_Vertical_Scaling();
void ISC_Scaler_Configure_Horizontal_Scaling();
#endif

void ISC_CSC_Enable(uint8_t enabled);
void ISC_CSC_Configure(PLIB_ISC_COLOR_SPACE* cs);


void ISC_CBC_Enable(uint8_t enabled);
void ISC_CBC_Configure(uint8_t ccir656, uint8_t byte_order,
                       uint32_t brightness, uint32_t contrast);
#ifdef ISC_DPC_CTRL_Msk
void ISC_Enable_Black_Level(uint8_t enable, uint16_t level);
void ISC_Enable_Green_Correction(uint8_t enable, uint8_t clip);
#endif

#ifdef ISC_CBHS_HUE_Msk
void ISC_CBHS_Configure(uint32_t hue, uint32_t saturation);
#endif

void ISC_Sub422_Enable(uint8_t enabled);
void ISC_Sub422_Configure(uint8_t ccir656, uint8_t byte_order, uint8_t lpf);
void ISC_Sub420_Configure(uint8_t enabled, uint8_t filter);

void ISC_RLP_Configure(uint8_t rlp_mode, uint8_t alpha);

void ISC_Histogram_Enable(uint8_t enabled);
void ISC_Histogram_Configure(uint8_t mode, uint8_t bay_sel, uint8_t reset);
void ISC_Update_Histogram_Table(void);
void ISC_Clear_Histogram_Table(void);

void ISC_DMA_Configure_Input_Mode(uint32_t mode);
void ISC_DMA_Configure_Desc_Entry(uint32_t desc_entry);
void ISC_DMA_Enable(uint32_t ctrl);
void ISC_DMA_Address(uint8_t channel, uint32_t address, uint32_t stride);

void ISC_Initialize(void);
#endif // PLIB_ISC_H
