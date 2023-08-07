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
