/*******************************************************************************
    CSI2DC PLIB

    Company:
        Microchip Technology Inc.

    File Name:
    plib_csi2dc.h

    Summary:
    CSI2DC PLIB Header File

    Description:
        None

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

#ifndef PLIB_CSI2DC_H
#define PLIB_CSI2DC_H

#include <stdint.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
    // DOM-IGNORE-END
    // 0x18 to 0x1F YUV Data Format
#define CSI2DC_DATA_FORMAT_YUV420_8   0x18
#define CSI2DC_DATA_FORMAT_YUV420_10  0x19
#define CSI2DC_DATA_FORMAT_YUV420_8L  0x1A
#define CSI2DC_DATA_FORMAT_RSRV10     0x1B
#define CSI2DC_DATA_FORMAT_YUV420_8C  0x1C
#define CSI2DC_DATA_FORMAT_YUV420_10C 0x1D
#define CSI2DC_DATA_FORMAT_YUV422_8   0x1E
#define CSI2DC_DATA_FORMAT_YUV422_10  0x1F

    // 0x20 to 0x27 RGB Data Format
#define CSI2DC_DATA_FORMAT_RGB444     0x20
#define CSI2DC_DATA_FORMAT_RGB555     0x21
#define CSI2DC_DATA_FORMAT_RGB565     0x22
#define CSI2DC_DATA_FORMAT_RGB666     0x23
#define CSI2DC_DATA_FORMAT_RGB888     0x24
#define CSI2DC_DATA_FORMAT_RSRV11     0x25
#define CSI2DC_DATA_FORMAT_RSRV12     0x26
#define CSI2DC_DATA_FORMAT_RSRV13     0x27

    // 0x28 to 0x2F RAW Data Format
#define CSI2DC_DATA_FORMAT_RAW6       0x28
#define CSI2DC_DATA_FORMAT_RAW7       0x29
#define CSI2DC_DATA_FORMAT_RAW8       0x2A
#define CSI2DC_DATA_FORMAT_RAW10      0x2B
#define CSI2DC_DATA_FORMAT_RAW12      0x2C
#define CSI2DC_DATA_FORMAT_RAW14      0x2D

    void CSI2DC_Reset(void);
    void CSI2DC_Global_Config(uint32_t cfg);
    uint32_t CSI2DC_Global_Status(void);
    void CSI2DC_Enable_Interrupt(uint32_t flag);
    void CSI2DC_Disable_Interrupt(uint32_t flag);
    uint32_t CSI2DC_Interrupt_Status(void);
    void CSI2DC_Configure_VideoPipe(uint32_t dt, uint32_t vc, uint32_t align_isc);
    void CSI2DC_Enable_VideoPipe(void);
    void CSI2DC_Enable_VideoPipe_Interrupt(uint32_t flag);
    void CSI2DC_Disable_VideoPipe_Interrupt(uint32_t flag);
    uint32_t CSI2DC_VideoPipe_Interrupt_Status(void);
    void CSI2DC_Configure_DataPipe(uint32_t dt, uint32_t vc, uint32_t bo);
    void CSI2DC_Enable_DataPipe(void);
    void CSI2DC_Configure_DataPipe_DMA(uint32_t count, uint8_t chuck_size, bool enable);
    void CSI2DC_Enable_DataPipe_Interrupt(uint32_t flag);
    void CSI2DC_Disable_DataPipe_Interrupt(uint32_t flag);
    uint32_t CSI2DC_DataPipe_Interrupt_Status(void);
    void CSI2DC_Update_Pipe(uint32_t Pipe);
    uint32_t CSI2DC_Pipe_Status(void);

    // DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
#endif