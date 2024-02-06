/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
#include "plib_clk.h"






/*********************************************************************************
Initialize Generic clock
*********************************************************************************/




/*********************************************************************************
Initialize Peripheral clock
*********************************************************************************/

static void CLK_PeripheralClockInitialize(void)
{
// START OF CUSTOM CODE
    struct {
        uint8_t id;
        uint8_t clken;
        uint8_t gclken;
        uint8_t css;
        uint8_t div_val;
    } periphList[] =
    {
        { ID_PIOA, 1, 0, 0, 0},
        { ID_PIOB, 1, 0, 0, 0},
        { ID_PIOC, 1, 0, 0, 0},
        { ID_TC0, 1, 0, 0, 0},
        { ID_PIOD, 1, 0, 0, 0},
        { ID_TWIHS1, 1, 0, 0, 0},
        { ID_UART1, 1, 0, 0, 0},
        { ID_ISC, 1, 0, 0, 0},
        { ID_LCDC, 1, 0, 0, 0},        
        { ID_PERIPH_MAX + 1, 0, 0, 0, 0}//end of list marker
    };

    uint32_t count = sizeof(periphList)/sizeof(periphList[0]);
    for (uint32_t i = 0; i < count; i++)
    {
        if (periphList[i].id == (uint8_t)((uint32_t)ID_PERIPH_MAX + 1U))
        {
            break;
        }

        PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk |
                            PMC_PCR_GCKEN(periphList[i].gclken) |
                            PMC_PCR_EN(periphList[i].clken) |
                            PMC_PCR_GCKDIV(periphList[i].div_val) |
                            PMC_PCR_GCKCSS(periphList[i].css) |
                            PMC_PCR_PID(periphList[i].id);
    }
}
// End OF CUSTOM CODE
/*********************************************************************************
Initialize LCDC clock
*********************************************************************************/

static void CLK_ISCClockInitialize(void)
{
    PMC_REGS->PMC_SCER = PMC_SCER_ISCCK(1U);
}

/*********************************************************************************
Clock Initialize
*********************************************************************************/

void CLK_Initialize( void )
{
    /* Initialize Peripheral Clock */
    CLK_PeripheralClockInitialize();

    /* Initialize ISC (MCKx2) Clock */
    CLK_ISCClockInitialize();

}

