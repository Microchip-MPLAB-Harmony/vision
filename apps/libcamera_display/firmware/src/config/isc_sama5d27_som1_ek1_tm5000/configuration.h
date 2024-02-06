/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                            (0)
#define SYS_TIME_MAX_TIMERS                         (5)
#define SYS_TIME_HW_COUNTER_WIDTH                   (32)
#define SYS_TIME_HW_COUNTER_PERIOD                  (4294967295U)
#define SYS_TIME_HW_COUNTER_HALF_PERIOD             (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY                (492000000)
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES    (2200)



// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Instance 0 Configuration Options */
#define DRV_I2C_INDEX_0                       0
#define DRV_I2C_CLIENTS_NUMBER_IDX0           1
#define DRV_I2C_QUEUE_SIZE_IDX0               2
#define DRV_I2C_CLOCK_SPEED_IDX0              400000


/*** ISC PLib Configuration ***/
#define PLIB_ISC_MCK_SEL_VAL			0
#define PLIB_ISC_MCK_DIV_VAL			7
#define PLIB_ISC_ISP_CLK_SEL_VAL		0
#define PLIB_ISC_ISP_CLK_DIV_VAL		2
#define ISC_HSYNC_POLARITY_VAL		0
#define ISC_VSYNC_POLARITY_VAL		1


/*** Image Sensor Driver Configuration ***/
#define DRV_IMAGE_SENSOR_I2C_MODULE_INDEX		0

/* I2C Driver Common Configuration Options */
#define DRV_I2C_INSTANCES_NUMBER              (1U)



/*** Image Sensor Configuration ***/
#define IMAGE_SENSOR_NAME					"AutoDectect"
#define IMAGE_SENSOR_OUTPUT_RESOLUTION		DRV_IMAGE_SENSOR_WVGA
#define IMAGE_SENSOR_OUTPUT_FORMAT			DRV_IMAGE_SENSOR_RAW_BAYER
#define IMAGE_SENSOR_OUTPUT_BUS_WIDTH		DRV_IMAGE_SENSOR_8_BIT


/*** ISC Image Sensor Configuration ***/
#define ISC_INPUT_FORMAT_TYPE			DRV_IMAGE_SENSOR_RAW_BAYER
#define ISC_INPUT_BIT_WIDTH				DRV_IMAGE_SENSOR_8_BIT
#define ISC_OUTPUT_FORMAT_TYPE			ISC_RLP_CFG_MODE_RGB565
#define ISC_OUTPUT_LAYOUT_TYPE			ISC_LAYOUT_PACKED16
#define ISC_BAYER_PATTERN_TYPE			ISC_CFA_CFG_BAYCFG_BGBG_Val
#define ISC_ENABLE_GAMMA				true
#define ISC_GAMMA_RED_ENTRIES			false
#define ISC_GAMMA_BLUE_ENTRIES			false
#define ISC_GAMMA_GREEN_ENTRIES			false
#define ISC_ENABLE_WHITE_BALANCE		true
#define ISC_WB_R_OFFSET					0
#define ISC_WB_GR_OFFSET				0
#define ISC_WB_B_OFFSET					0
#define ISC_WB_GB_OFFSET				0
#define ISC_WB_R_GAIN					512
#define ISC_WB_GR_GAIN					512
#define ISC_WB_B_GAIN					512
#define ISC_WB_GB_GAIN					512
#define ISC_ENABLE_HISTOGRAM			false
#define ISC_ENABLE_MIPI_INTERFACE		false
#define ISC_ENABLE_VIDEO_MODE			true
#define ISC_ENABLE_BRIGHTNESS_CONTRAST	true
#define ISC_CBC_BRIGHTNESS_VAL			32
#define ISC_CBC_CONTRAST_VAL			255
#define ISC_CBHS_HUE_VAL				0
#define ISC_CBHS_SATURATION_VAL			0
#define ISC_ENABLE_PROGRESSIVE_MODE		true



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
