<#--
/*******************************************************************************
  ISC Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_isc.c.ftl

  Summary:
    ISC Driver Freemarker Template File

  Description:

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

-->

/*** ISC Image Sensor Configuration ***/
#define ISC_INPUT_FORMAT_TYPE			${ISCInputFormatType}
#define ISC_INPUT_BIT_WIDTH				${ISCInputBitWidth}
#define ISC_OUTPUT_FORMAT_TYPE			${ISCOutputFormatType}
#define ISC_OUTPUT_LAYOUT_TYPE			${ISCOutputLayoutType}
#define ISC_BAYER_PATTERN_TYPE			${ISCBayerPatternType}
<#if ISCEnableGamma == true>
	<#lt>#define ISC_ENABLE_GAMMA				true
<#else>
	<#lt>#define ISC_ENABLE_GAMMA				false
</#if>
<#if ISCGammaRedEntries == true>
	<#lt>#define ISC_GAMMA_RED_ENTRIES			true
<#else>
	<#lt>#define ISC_GAMMA_RED_ENTRIES			false
</#if>
<#if ISCGammaBlueEntries == true>
	<#lt>#define ISC_GAMMA_BLUE_ENTRIES			true
<#else>
	<#lt>#define ISC_GAMMA_BLUE_ENTRIES			false
</#if>
<#if ISCGammaGreenEntries == true>
	<#lt>#define ISC_GAMMA_GREEN_ENTRIES			true
<#else>
	<#lt>#define ISC_GAMMA_GREEN_ENTRIES			false
</#if>
<#if ISCEnableWhiteBalance == true>
	<#lt>#define ISC_ENABLE_WHITE_BALANCE		true
<#else>
	<#lt>#define ISC_ENABLE_WHITE_BALANCE		false
</#if>
#define ISC_WB_R_OFFSET					${ISCWhiteBalance_R_Offset}
#define ISC_WB_GR_OFFSET				${ISCWhiteBalance_GR_Offset}
#define ISC_WB_B_OFFSET					${ISCWhiteBalance_B_Offset}
#define ISC_WB_GB_OFFSET				${ISCWhiteBalance_GB_Offset}
#define ISC_WB_R_GAIN					${ISCWhiteBalance_R_Gain}
#define ISC_WB_GR_GAIN					${ISCWhiteBalance_GR_Gain}
#define ISC_WB_B_GAIN					${ISCWhiteBalance_B_Gain}
#define ISC_WB_GB_GAIN					${ISCWhiteBalance_GB_Gain}
<#if ISCEnableHistogram == true>
	<#lt>#define ISC_ENABLE_HISTOGRAM			true
<#else>
	<#lt>#define ISC_ENABLE_HISTOGRAM			false
</#if>
<#if ISCEnableMIPI == true>
	<#lt>#define ISC_ENABLE_MIPI_INTERFACE		true
<#else>
	<#lt>#define ISC_ENABLE_MIPI_INTERFACE		false
</#if>
<#if ISCEnableVideoMode == true>
	<#lt>#define ISC_ENABLE_VIDEO_MODE			true
<#else>
	<#lt>#define ISC_ENABLE_VIDEO_MODE			false
</#if>
<#if ISCEnableBightnessAndContrast == true>
	<#lt>#define ISC_ENABLE_BRIGHTNESS_CONTRAST	true
<#else>
	<#lt>#define ISC_ENABLE_BRIGHTNESS_CONTRAST	false
</#if>
#define ISC_CBC_BRIGHTNESS_VAL			${ISCBightness}
#define ISC_CBC_CONTRAST_VAL			${ISCContrast}
#define ISC_CBHS_HUE_VAL				${ISCHue}
#define ISC_CBHS_SATURATION_VAL			${ISCSaturation}
<#if ISCEnableProgressiveMode == true>
	<#lt>#define ISC_ENABLE_PROGRESSIVE_MODE		true
<#else>
	<#lt>#define ISC_ENABLE_PROGRESSIVE_MODE		false
</#if>
<#if ISCEnableScaling == true>
	<#lt>#define ISC_ENABLE_SCALING		true
<#else>
	<#lt>#define ISC_ENABLE_SCALING		false
</#if>
#define ISC_SCALE_OUTPUT_WIDTH				${ISCScaleOutputWidth}
#define ISC_SCALE_OUTPUT_HEIGHT				${ISCScaleOutputHeight}
<#--
/*******************************************************************************
 End of File
*/
-->

