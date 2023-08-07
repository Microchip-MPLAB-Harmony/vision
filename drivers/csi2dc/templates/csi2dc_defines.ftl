<#--
/*******************************************************************************
  CSI2DC Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_csi2dc.ftl

  Summary:
    CSI2DC Driver Freemarker Template File

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

/*** CSI2DC Driver Configuration ***/
#define CSI2DC_BUS_TYPE		${CSI2DCBusType}
#define CSI2DC_VIDEO_PIPE_FORMAT_TYPE		${CSI2DCVideoPipeFormatType}
#define CSI2DC_VIDEO_PIPE_CHANNEL_ID		${CSI2DCVideoPipeChannelId}
#define CSI2DC_DATA_PIPE_CHANNEL_ID			${CSI2DCDataPipeChannelId}
#define CSI2DC_DATA_PIPE_FORMAT_TYPE		${CSI2DCDataPipeFormatType}
#define CSI2DC_DATA_PIPE_DMA_CHUCK_SIZE		${CSI2DCDataPipeDMAChuckSize}
#define CSI2DC_DATA_PIPE_DMA_COUNT			${CSI2DCDMACount}
<#if CSI2DCMIPIClock == true>
	<#lt>#define CSI2DC_ENABLE_MIPI_CLOCK_FREE_RUN		true
<#else>
	<#lt>#define CSI2DC_ENABLE_MIPI_CLOCK_FREE_RUN		false
</#if>
<#if CSI2DCPostAligned == true>
	<#lt>#define CSI2DC_POST_ALIGNED		true
<#else>
	<#lt>#define CSI2DC_POST_ALIGNED		false
</#if>
<#if CSI2DCEnableDataPipe == true>
	<#lt>#define CSI2DC_ENABLE_DATA_PIPE		true
<#else>
	<#lt>#define CSI2DC_ENABLE_DATA_PIPE		false
</#if>
<#if CSI2DCDataPipeEnableDMA == true>
	<#lt>#define CSI2DC_DATA_PIPE_ENABLE_DMA		true
<#else>
	<#lt>#define CSI2DC_DATA_PIPE_ENABLE_DMA	false
</#if>

<#--
/*******************************************************************************
End of File
*/
-->

