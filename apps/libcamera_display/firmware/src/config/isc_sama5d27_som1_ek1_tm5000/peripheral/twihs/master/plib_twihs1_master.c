/*******************************************************************************
  TWIHS Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_twihs1_master.c

  Summary
    TWIHS Master peripheral library interface.

  Description

  Remarks:

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

// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_twihs1_master.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

volatile static TWIHS_OBJ twihs1Obj;

// *****************************************************************************
// *****************************************************************************
// TWIHS1 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

void TWIHS1_Initialize( void )
{
    // Reset the i2c Module
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    // Disable the I2C Master/Slave Mode
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    // Set Baud rate
// START OF CUSTOM CODE    
    TWIHS1_REGS->TWIHS_CWGR = TWIHS_CWGR_HOLD(50) | TWIHS_CWGR_CLDIV(103) | TWIHS_CWGR_CHDIV(95) | TWIHS_CWGR_CKDIV(0) | TWIHS_CWGR_CKSRC_PERIPH_CK;
// End OF CUSTOM CODE
    
    // Starts the transfer by clearing the transmit hold register
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_THRCLR_Msk;

    // Disable TXRDY, TXCOMP and RXRDY interrupts
    TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

    // Enables interrupt on nack and arbitration lost
    TWIHS1_REGS->TWIHS_IER = TWIHS_IER_NACK_Msk | TWIHS_IER_ARBLST_Msk;

    // Enable Master Mode
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

    // Initialize the twihs PLib Object
    twihs1Obj.error = TWIHS_ERROR_NONE;
    twihs1Obj.state = TWIHS_STATE_IDLE;
}

static void TWIHS1_InitiateRead( void )
{
    twihs1Obj.state = TWIHS_STATE_TRANSFER_READ;

    TWIHS1_REGS->TWIHS_MMR |= TWIHS_MMR_MREAD_Msk;

    /* When a single data byte read is performed,
     * the START and STOP bits must be set at the same time
     */
    if(twihs1Obj.readSize == 1U)
    {
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_START_Msk | TWIHS_CR_STOP_Msk;
    }
    else
    {
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_START_Msk;
    }

    __enable_irq();

    TWIHS1_REGS->TWIHS_IER = TWIHS_IER_RXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
}

static bool TWIHS1_InitiateTransfer( uint16_t address, bool type )
{
    uint32_t timeoutCntr = 98400;

    // 10-bit Slave Address
    if( address > 0x007FU )
    {
        TWIHS1_REGS->TWIHS_MMR = TWIHS_MMR_DADR(((uint32_t)address & 0x00007F00U) >> 8U) | TWIHS_MMR_IADRSZ(1);

        // Set internal address
        TWIHS1_REGS->TWIHS_IADR = TWIHS_IADR_IADR((uint32_t)address & 0x000000FFU );
    }
    // 7-bit Slave Address
    else
    {
        TWIHS1_REGS->TWIHS_MMR = TWIHS_MMR_DADR(address) | TWIHS_MMR_IADRSZ(0);
    }

    twihs1Obj.writeCount = 0;
    twihs1Obj.readCount = 0;

    // Write transfer
    if(type == false)
    {
        // Single Byte Write
        if( twihs1Obj.writeSize == 1U )
        {
            // Single Byte write only
            if( twihs1Obj.readSize == 0U )
            {
                // Load last byte in transmit register, issue stop condition
                // Generate TXCOMP interrupt after STOP condition has been sent
                twihs1Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;

                TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[0]);
                twihs1Obj.writeCount++;
                TWIHS1_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                TWIHS1_REGS->TWIHS_IER = TWIHS_IER_TXCOMP_Msk;
            }
            // Single Byte write and than read transfer
            else
            {
                // START bit must be set before the byte is shifted out. Hence disabled interrupt
                __disable_irq();

                TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[0]);
                twihs1Obj.writeCount++;

                // Wait for control byte to be transferred before initiating repeat start for read
                while((TWIHS1_REGS->TWIHS_SR & (TWIHS_SR_TXCOMP_Msk | TWIHS_SR_TXRDY_Msk)) != 0U)
                {
                    /* Do Nothing */
                }

                while((TWIHS1_REGS->TWIHS_SR & (TWIHS_SR_TXRDY_Msk)) == 0U)
                {
                    if (timeoutCntr == 0U)
                    {
                        twihs1Obj.error = TWIHS_BUS_ERROR;
                        __enable_irq();
                        return false;
                    }
                    timeoutCntr--;
                }

                type = true;
            }
        }
        // Multi-Byte Write
        else
        {
            twihs1Obj.state = TWIHS_STATE_TRANSFER_WRITE;

            TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[0]);
            twihs1Obj.writeCount++;

            TWIHS1_REGS->TWIHS_IER = TWIHS_IDR_TXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
        }
    }
    // Read transfer
    if(type)
    {
        TWIHS1_InitiateRead();
    }
    return true;
}

void TWIHS1_CallbackRegister( TWIHS_CALLBACK callback, uintptr_t contextHandle )
{
    if (callback != NULL)
    {
        twihs1Obj.callback = callback;

        twihs1Obj.context = contextHandle;
    }
}

bool TWIHS1_IsBusy( void )
{
    // Check for ongoing transfer
    if( twihs1Obj.state == TWIHS_STATE_IDLE )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void TWIHS1_TransferAbort( void )
{
    twihs1Obj.error = TWIHS_ERROR_NONE;

    // Reset the PLib objects and Interrupts
    twihs1Obj.state = TWIHS_STATE_IDLE;
    TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

    // Disable and Enable I2C Master
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;
}

bool TWIHS1_Read( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs1Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }
    if ((TWIHS1_REGS->TWIHS_SR & (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk)) != (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk))
    {
        twihs1Obj.error = TWIHS_BUS_ERROR;
        return false;
    }

    twihs1Obj.address = address;
    twihs1Obj.readBuffer = pdata;
    twihs1Obj.readSize = length;
    twihs1Obj.writeBuffer = NULL;
    twihs1Obj.writeSize = 0;
    twihs1Obj.error = TWIHS_ERROR_NONE;

    return TWIHS1_InitiateTransfer(address, true);
}

bool TWIHS1_Write( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs1Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }
    if ((TWIHS1_REGS->TWIHS_SR & (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk)) != (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk))
    {
        twihs1Obj.error = TWIHS_BUS_ERROR;
        return false;
    }

    twihs1Obj.address = address;
    twihs1Obj.readBuffer = NULL;
    twihs1Obj.readSize = 0;
    twihs1Obj.writeBuffer = pdata;
    twihs1Obj.writeSize = length;
    twihs1Obj.error = TWIHS_ERROR_NONE;

    return TWIHS1_InitiateTransfer(address, false);
}

bool TWIHS1_WriteRead( uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength )
{

    // Check for ongoing transfer
    if( twihs1Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }
    if ((TWIHS1_REGS->TWIHS_SR & (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk)) != (TWIHS_SR_SDA_Msk | TWIHS_SR_SCL_Msk))
    {
        twihs1Obj.error = TWIHS_BUS_ERROR;
        return false;
    }

    twihs1Obj.address = address;
    twihs1Obj.readBuffer = rdata;
    twihs1Obj.readSize = rlength;
    twihs1Obj.writeBuffer = wdata;
    twihs1Obj.writeSize = wlength;
    twihs1Obj.error = TWIHS_ERROR_NONE;

    return TWIHS1_InitiateTransfer(address, false);
}

TWIHS_ERROR TWIHS1_ErrorGet( void )
{
    TWIHS_ERROR error = TWIHS_ERROR_NONE;

    error = twihs1Obj.error;

    twihs1Obj.error = TWIHS_ERROR_NONE;

    return error;
}

bool TWIHS1_TransferSetup( TWIHS_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t i2cClkSpeed;
    uint32_t cldiv;
    uint8_t ckdiv = 0;

    if (setup == NULL)
    {
        return false;
    }

    i2cClkSpeed = setup->clkSpeed;

    /* Maximum I2C clock speed in Master mode cannot be greater than 400 KHz */
    if (i2cClkSpeed > 4000000U)
    {
        return false;
    }

    if(srcClkFreq == 0U)
    {
        srcClkFreq = 82000000;
    }

    /* Formula for calculating baud value involves two unknowns. Fix one unknown and calculate the other.
       Fix the CKDIV value and see if CLDIV (or CHDIV) fits into the 8-bit register. */

    /* Calculate CLDIV with CKDIV set to 0 */
    cldiv = (srcClkFreq /(2U * i2cClkSpeed)) - 3U;

    /* CLDIV must fit within 8-bits and CKDIV must fit within 3-bits */
    while ((cldiv > 255U) && (ckdiv < 7U))
    {
        ckdiv++;
        cldiv /= 2U;
    }

    if (cldiv > 255U)
    {
        /* Could not generate CLDIV and CKDIV register values for the requested baud rate */
        return false;
    }

    /* set clock waveform generator register */
    TWIHS1_REGS->TWIHS_CWGR = (TWIHS_CWGR_HOLD_Msk & TWIHS1_REGS->TWIHS_CWGR) | TWIHS_CWGR_CLDIV(cldiv) | TWIHS_CWGR_CHDIV(cldiv) | TWIHS_CWGR_CKDIV(ckdiv);

    return true;
}

void __attribute__((used)) TWIHS1_InterruptHandler( void )
{
    uint32_t status;
    uintptr_t context = twihs1Obj.context;

    // Read the peripheral status
    status = TWIHS1_REGS->TWIHS_SR;

    /* checks if Slave has Nacked */
    if(( status & TWIHS_SR_NACK_Msk ) != 0U)
    {
        twihs1Obj.state = TWIHS_STATE_ERROR;
        twihs1Obj.error = TWIHS_ERROR_NACK;
    }

    if(( status & TWIHS_SR_TXCOMP_Msk ) != 0U)
    {
        /* Disable and Enable I2C Master */
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        /* Disable Interrupt */
        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;
    }

    /* checks if the arbitration is lost in multi-master scenario */
    if(( status & TWIHS_SR_ARBLST_Msk ) != 0U)
    {
        /* Re-initiate the transfer if arbitration is lost in
         * between of the transfer
         */
        twihs1Obj.state = TWIHS_STATE_ADDR_SEND;
    }

    if( twihs1Obj.error == TWIHS_ERROR_NONE)
    {
        switch( twihs1Obj.state )
        {
            case TWIHS_STATE_ADDR_SEND:
            {
                if (twihs1Obj.writeSize != 0U )
                {
                    // Initiate Write transfer
                    (void) TWIHS1_InitiateTransfer(twihs1Obj.address, false);
                }
                else
                {
                    // Initiate Read transfer
                     (void) TWIHS1_InitiateTransfer(twihs1Obj.address, true);
                }
            }
            break;

            case TWIHS_STATE_TRANSFER_WRITE:
            {
                /* checks if master is ready to transmit */
                if(( status & TWIHS_SR_TXRDY_Msk ) != 0U)
                {
                    size_t writeCount = twihs1Obj.writeCount;
                    bool lastByteWrPending = (writeCount == (twihs1Obj.writeSize -1U));

                    // Write Last Byte and then initiate read transfer
                    if(( twihs1Obj.readSize != 0U ) && (lastByteWrPending))
                    {
                        // START bit must be set before the last byte is shifted out to generate repeat start. Hence disabled interrupt
                        __disable_irq();
                        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
                        TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[writeCount]);
                        writeCount++;
                        TWIHS1_InitiateRead();
                    }
                    // Write Last byte and then issue STOP condition
                    else if ( writeCount == (twihs1Obj.writeSize -1U))
                    {
                        // Load last byte in transmit register, issue stop condition
                        // Generate TXCOMP interrupt after STOP condition has been sent
                        TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[writeCount]);
                        writeCount++;
                        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS1_REGS->TWIHS_SR;

                        if(( status & TWIHS_SR_TXCOMP_Msk ) != 0U)
                        {
                            twihs1Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs1Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                    // Write next byte
                    else
                    {
                        TWIHS1_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs1Obj.writeBuffer[writeCount]);
                        writeCount++;
                    }

                    twihs1Obj.writeCount = writeCount;

                    // Dummy read to ensure that TXRDY bit is cleared
                    status = TWIHS1_REGS->TWIHS_SR;
                    (void) status;
                }

                break;
            }

            case TWIHS_STATE_TRANSFER_READ:
            {
                /* checks if master has received the data */
                if(( status & TWIHS_SR_RXRDY_Msk ) != 0U)
                {
                    size_t readCount = twihs1Obj.readCount;

                    // Set the STOP (or START) bit before reading the TWIHS_RHR on the next-to-last access
                    if(  readCount == (twihs1Obj.readSize - 2U) )
                    {
                        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                    }

                    /* read the received data */
                    twihs1Obj.readBuffer[readCount] = (uint8_t)(TWIHS1_REGS->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);
                    readCount++;

                    /* checks if transmission has reached at the end */
                    if( readCount == twihs1Obj.readSize )
                    {
                        /* Disable the RXRDY interrupt*/
                        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_RXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS1_REGS->TWIHS_SR;
                        if(( status & TWIHS_SR_TXCOMP_Msk ) != 0U)
                        {
                            twihs1Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs1Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }

                    twihs1Obj.readCount = readCount;
                }
                break;
            }

            case TWIHS_STATE_WAIT_FOR_TXCOMP:
            {
                if(( status & TWIHS_SR_TXCOMP_Msk ) != 0U)
                {
                    twihs1Obj.state = TWIHS_STATE_TRANSFER_DONE;
                }
                break;
            }

            default:
            {
                /* Do Nothing */
                break;
            }
        }
    }
    if (twihs1Obj.state == TWIHS_STATE_ERROR)
    {
        // NACK is received,
        twihs1Obj.state = TWIHS_STATE_IDLE;
        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs1Obj.callback != NULL )
        {
            twihs1Obj.callback( context );
        }
    }
    // check for completion of transfer
    if( twihs1Obj.state == TWIHS_STATE_TRANSFER_DONE )
    {
        twihs1Obj.error = TWIHS_ERROR_NONE;

        // Reset the PLib objects and Interrupts
        twihs1Obj.state = TWIHS_STATE_IDLE;
        TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs1Obj.callback != NULL )
        {
            twihs1Obj.callback( context );
        }
    }

    return;
}