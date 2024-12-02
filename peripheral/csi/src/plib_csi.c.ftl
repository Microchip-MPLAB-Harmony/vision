/*******************************************************************************
  CSI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_csi.c

  Summary:
    CSI PLIB Implementation File

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
#include "plib_csi.h"

void CSI_Configure_Lane(uint8_t lane)
{
    CSI_REGS->CSI_N_LANES = CSI_N_LANES_N_LANES(lane);
}

void CSI_Reset(void)
{
    CSI_REGS->CSI_CSI2_RESETN = CSI_CSI2_RESETN_CSI2_RESETN(0);
}

void CSI_Exit_Reset(void)
{
    CSI_REGS->CSI_CSI2_RESETN = CSI_CSI2_RESETN_CSI2_RESETN(1);
}

void CSI_Shutdown(void)
{
    CSI_REGS->CSI_PHY_SHUTDOWNZ = CSI_PHY_SHUTDOWNZ_PHY_SHUTDOWNZ(0);
}

void CSI_Exit_Shutdown(void)
{
    CSI_REGS->CSI_PHY_SHUTDOWNZ = CSI_PHY_SHUTDOWNZ_PHY_SHUTDOWNZ(1);
}

void CSI_Reset_DPhy(void)
{
    CSI_REGS->CSI_DPHY_RSTZ = CSI_DPHY_RSTZ_DPHY_RSTZ(0);
}

void CSI_Exit_Reset_DPhy(void)
{
    CSI_REGS->CSI_DPHY_RSTZ = CSI_DPHY_RSTZ_DPHY_RSTZ(1);
}

void CSI_Configure_DataId(uint8_t id, uint8_t vchannel, uint8_t datatype)
{
#if 0  
    switch (id)
    {
    case 0:
        CSI_REGS->CSI_DATA_IDS_1 = CSI_DATA_IDS_1_DI0_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_1 |= CSI_DATA_IDS_1_DI0_VC(vchannel);
        break;
    case 1:
        CSI_REGS->CSI_DATA_IDS_1 = CSI_DATA_IDS_1_DI1_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_1 |= CSI_DATA_IDS_1_DI1_VC(vchannel);
        break;
    case 2:
        CSI_REGS->CSI_DATA_IDS_1 = CSI_DATA_IDS_1_DI2_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_1 |= CSI_DATA_IDS_1_DI2_VC(vchannel);
        break;
    case 3:
        CSI_REGS->CSI_DATA_IDS_1 = CSI_DATA_IDS_1_DI3_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_1 |= CSI_DATA_IDS_1_DI3_VC(vchannel);
        break;
    case 4:
        CSI_REGS->CSI_DATA_IDS_2 = CSI_DATA_IDS_2_DI4_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_2 |= CSI_DATA_IDS_2_DI4_VC(vchannel);
        break;
    case 5:
        CSI_REGS->CSI_DATA_IDS_2 = CSI_DATA_IDS_2_DI5_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_2 |= CSI_DATA_IDS_2_DI5_VC(vchannel);
        break;
    case 6:
        CSI_REGS->CSI_DATA_IDS_2 = CSI_DATA_IDS_2_DI6_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_2 |= CSI_DATA_IDS_2_DI6_VC(vchannel);
        break;
    case 7:
        CSI_REGS->CSI_DATA_IDS_2 = CSI_DATA_IDS_2_DI7_DT(datatype);
        CSI_REGS->CSI_DATA_IDS_2 |= CSI_DATA_IDS_2_DI7_VC(vchannel);
        break;
    }
#endif    
}

void CSI_Analog_Init(uint8_t bit_rate, uint8_t nlanes)
{
    // Release PHY test codes from reset
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x00) | 0;
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLR(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;

    // PHY RX only
    // HS RX Control of Clock Lane (code = 0x34)
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x34) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;

    // HS RX Control of Lane 0 (code = 0x44)
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x44) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(bit_rate) | 0; //Bit rate range = 150-169 Mbps (0x02)
    CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
    CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;

    if (nlanes == CSI_DATA_LANES_2)
    {
        // HS RX Control of Lane 1 (code = 0x54)
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x54) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    }
    else if (nlanes == CSI_DATA_LANES_3)
    {
        // HS RX Control of Lane 2 (code = 0x64)
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x24) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    }
    else if (nlanes == CSI_DATA_LANES_4)
    {
        // HS RX Control of Lane 2 (code = 0x64)
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x24) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;

        // HS RX Control of Lane 3 (code = 0x74)
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x74) | CSI_PHY_TEST_CTRL1_PHY_TESTEN(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
        CSI_REGS->CSI_PHY_TEST_CTRL1 = CSI_PHY_TEST_CTRL1_PHY_TESTDIN(0x94) | 0; // HS RX offset calibration + termination enable
        CSI_REGS->CSI_PHY_TEST_CTRL0 = CSI_PHY_TEST_CTRL0_PHY_TESTCLK(1);
        CSI_REGS->CSI_PHY_TEST_CTRL0 = 0;
    }
}

