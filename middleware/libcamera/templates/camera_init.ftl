<#--
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
const CAMERA_INIT drvCAMERAInitData =
{
    .imageSensorName               = IMAGE_SENSOR_NAME,
    .imageSensorResolution         = IMAGE_SENSOR_OUTPUT_RESOLUTION,
    .imageSensorOutputFormat       = IMAGE_SENSOR_OUTPUT_FORMAT,
    .imageSensorOutputBitWidth     = IMAGE_SENSOR_OUTPUT_BUS_WIDTH,
    .iscInputFormat                = ISC_INPUT_FORMAT_TYPE,
    .iscInputBits                  = ISC_INPUT_BIT_WIDTH,
    .iscOutputFormat               = ISC_OUTPUT_FORMAT_TYPE,
    .iscOutputLayout               = ISC_OUTPUT_LAYOUT_TYPE,
    .iscBayerPattern               = ISC_BAYER_PATTERN_TYPE,
    .iscEnableGamma                = ISC_ENABLE_GAMMA,
    .iscEnableWhiteBalance         = ISC_ENABLE_WHITE_BALANCE,
    .iscEnableHistogram            = ISC_ENABLE_HISTOGRAM,
    .iscEnableMIPI                 = ISC_ENABLE_MIPI_INTERFACE,
    .iscEnableVideoMode            = ISC_ENABLE_VIDEO_MODE,
    .iscEnableBightnessAndContrast = ISC_ENABLE_BRIGHTNESS_CONTRAST,
    .iscEnableProgressiveMode      = ISC_ENABLE_PROGRESSIVE_MODE,
    .csiDataFormat                 = CSI_DATA_FORMAT_TYPE,
    .drvI2CIndex                   = DRV_IMAGE_SENSOR_I2C_MODULE_INDEX,
};

// </editor-fold>
<#--
/*******************************************************************************
 End of File
*/
-->
