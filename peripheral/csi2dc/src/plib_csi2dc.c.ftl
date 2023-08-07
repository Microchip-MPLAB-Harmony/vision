/*******************************************************************************
	CSI2DC PLIB

	Company:
		Microchip Technology Inc.

	File Name:
		plib_csi2dc.c

	Summary:
		CSI2DC PLIB Implementation File

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

#include "device.h"
#include "plib_csi2dc.h"

void CSI2DC_Global_Config(uint32_t cfg)
{
	CSI2DC_REGS->CSI2DC_GCFGR = cfg;
}

void CSI2DC_Reset(void)
{
	int i = 0;
	CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_1;
	for(i = 0; i < 10000; i++);
	CSI2DC_REGS->CSI2DC_GCTLR = CSI2DC_GCTLR_SWRST_0;
}

uint32_t CSI2DC_Global_status(void)
{
	return CSI2DC_REGS->CSI2DC_GSR;
}

void CSI2DC_Enable_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_GIER = flag;
}

void CSI2DC_Disable_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_GIDR = flag;
}

uint32_t CSI2DC_Interrupt_Status(void)
{
	return CSI2DC_REGS->CSI2DC_GISR;
}

void CSI2DC_Configure_VideoPipe(uint32_t dt, uint32_t vc, uint32_t align_isc)
{
	CSI2DC_REGS->CSI2DC_VPCFGR = CSI2DC_VPCFGR_DT(dt) | CSI2DC_VPCFGR_VC(vc) | (align_isc ? CSI2DC_VPCFGR_PA_1 : 0);
}

void CSI2DC_Enable_VideoPipe(void)
{
	CSI2DC_REGS->CSI2DC_VPER = CSI2DC_VPER_ENABLE_1;
}

void CSI2DC_Enable_VideoPipe_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_VPIER = flag;
}

void CSI2DC_Disable_VideoPipe_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_VPIDR = flag;
}

uint32_t CSI2DC_VideoPipe_Interrupt_Status(void)
{
	return CSI2DC_REGS->CSI2DC_VPISR;
}

void CSI2DC_Configure_DataPipe(uint32_t dt, uint32_t vc, uint32_t bo)
{
	CSI2DC_REGS->CSI2DC_DPCFGR = CSI2DC_DPCFGR_DT(dt) | CSI2DC_DPCFGR_VC(vc) | CSI2DC_DPCFGR_BO(bo);
}

void CSI2DC_Enable_DataPipe(void)
{
	CSI2DC_REGS->CSI2DC_DPER = CSI2DC_DPER_ENABLE_1;
}

void CSI2DC_Configure_DataPipe_DMA(uint32_t count, uint8_t chuck_size, bool enable)
{
	CSI2DC_REGS->CSI2DC_DPDCR = (CSI2DC_DPDCR_TC(count) | CSI2DC_DPDCR_CSIZE(chuck_size) | (enable ? CSI2DC_DPDCR_DMA_1 : CSI2DC_DPDCR_DMA_0));
}

void CSI2DC_Enable_DataPipe_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_DPIER = flag;
}

void CSI2DC_Disable_DataPipe_Interrupt(uint32_t flag)
{
	CSI2DC_REGS->CSI2DC_DPIDR = flag;
}

uint32_t CSI2DC_DataPipe_Interrupt_Status(void)
{
	return CSI2DC_REGS->CSI2DC_DPISR;
}

void CSI2DC_Update_Pipe(uint32_t pipe)
{
	CSI2DC_REGS->CSI2DC_PUR = pipe;
}

uint32_t CSI2DC_Pipe_Status(void)
{
	return CSI2DC_REGS->CSI2DC_PUSR;
}
