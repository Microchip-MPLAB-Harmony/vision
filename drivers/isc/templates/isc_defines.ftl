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
#define ISC_INPUT_FORMAT_TYPE		${ISCInputFormatType}
#define ISC_INPUT_BIT_WIDTH			${ISCInputBitWidth}
#define ISC_OUTPUT_FORMAT_TYPE		${ISCOutputFormatType}
#define ISC_OUTPUT_LAYOUT_TYPE		${ISCOutputLayoutType}
#define ISC_BAYER_PATTERN_TYPE		${ISCBayerPatternType}
<#if ISCEnableGamma == true>
	<#lt>#define ISC_ENABLE_GAMMA		true
<#else>
	<#lt>#define ISC_ENABLE_GAMMA		false
</#if>
<#if ISCEnableWhiteBalance == true>
	<#lt>#define ISC_ENABLE_WHITE_BALANCE	true
<#else>
	<#lt>#define ISC_ENABLE_WHITE_BALANCE	false
</#if>
<#if ISCEnableHistogram == true>
	<#lt>#define ISC_ENABLE_HISTOGRAM		true
<#else>
	<#lt>#define ISC_ENABLE_HISTOGRAM	false
</#if>
<#if ISCEnableMIPI == true>
	<#lt>#define ISC_ENABLE_MIPI_INTERFACE	true
<#else>
	<#lt>#define ISC_ENABLE_MIPI_INTERFACE	false
</#if>
<#if ISCEnableVideoMode == true>
	<#lt>#define ISC_ENABLE_VIDEO_MODE		true
<#else>
	<#lt>#define ISC_ENABLE_VIDEO_MODE		false
</#if>
<#if ISCEnableBightnessAndContrast == true>
	<#lt>#define ISC_ENABLE_BRIGHTNESS_CONTRAST	true
<#else>
	<#lt>#define ISC_ENABLE_BRIGHTNESS_CONTRAST	false
</#if>
<#if ISCEnableProgressiveMode == true>
	<#lt>#define ISC_ENABLE_PROGRESSIVE_MODE	true
<#else>
	<#lt>#define ISC_ENABLE_PROGRESSIVE_MODE	false
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

