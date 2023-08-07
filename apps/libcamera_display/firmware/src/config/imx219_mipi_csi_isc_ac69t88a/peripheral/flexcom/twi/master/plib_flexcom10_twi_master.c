/*******************************************************************************
  FLEXCOM TWI Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_flexcom10_twi_master.c

  Summary
    FLEXCOM TWI Master peripheral library interface.

  Description
    This file defines the interface to the FLEXCOM TWI peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

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
#include "plib_flexcom10_twi_master.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Local Data Type Definitions
// *****************************************************************************
// *****************************************************************************

#define FLEXCOM_TWI_MASTER_MAX_BAUDRATE        (400000U)
#define FLEXCOM_TWI_LOW_LEVEL_TIME_LIMIT       (384000U)
#define FLEXCOM_TWI_CLK_DIVIDER                     (2U)
#define FLEXCOM_TWI_CLK_CALC_ARGU                   (3U)
#define FLEXCOM_TWI_CLK_DIV_MAX                  (0xFFU)
#define FLEXCOM_TWI_CLK_DIV_MIN                     (7U)

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

volatile static FLEXCOM_TWI_OBJ flexcom10TwiObj;

// *****************************************************************************
// *****************************************************************************
// FLEXCOM10 TWI PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void FLEXCOM10_TWI_Initialize(void)

   Summary:
    Initializes given instance of the FLEXCOM TWI peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/

void FLEXCOM10_TWI_Initialize(void)
{
    /* Set FLEXCOM TWI operating mode */
    FLEXCOM10_REGS->FLEX_MR = FLEX_MR_OPMODE_TWI;

    // Reset the i2c Module
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_SWRST_Msk;

    // Disable the I2C Master/Slave Mode
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSDIS_Msk |
                                          FLEX_TWI_CR_SVDIS_Msk;

    // Set Baud rate
    FLEXCOM10_REGS->FLEX_TWI_CWGR = (FLEXCOM10_REGS->FLEX_TWI_CWGR & FLEX_TWI_CWGR_HOLD_Msk)  |
                                                    FLEX_TWI_CWGR_BRSRCCLK_PERIPH_CLK |
                                                    (FLEX_TWI_CWGR_CLDIV(165) |
                                                     FLEX_TWI_CWGR_CHDIV(165) |
                                                     FLEX_TWI_CWGR_CKDIV(1));

    // Starts the transfer by clearing the transmit hold register
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_THRCLR_Msk;

    // Disable TXRDY, TXCOMP and RXRDY interrupts
    FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXCOMP_Msk | FLEX_TWI_IDR_TXRDY_Msk | FLEX_TWI_IDR_RXRDY_Msk;

    // Enables interrupt on nack and arbitration lost
    FLEXCOM10_REGS->FLEX_TWI_IER = FLEX_TWI_IER_NACK_Msk |
                                           FLEX_TWI_IER_ARBLST_Msk;

    // Enable Master Mode
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSEN_Msk;

    // Initialize the flexcom twi PLib Object
    flexcom10TwiObj.error   = FLEXCOM_TWI_ERROR_NONE;
    flexcom10TwiObj.state   = FLEXCOM_TWI_STATE_IDLE;
}


/******************************************************************************
Local Functions
******************************************************************************/

static void FLEXCOM10_TWI_InitiateRead(void)
{

    flexcom10TwiObj.state = FLEXCOM_TWI_STATE_TRANSFER_READ;

    FLEXCOM10_REGS->FLEX_TWI_MMR |= FLEX_TWI_MMR_MREAD_Msk;

    /* When a single data byte read is performed,
    the START and STOP bits must be set at the same time */
    if(flexcom10TwiObj.readSize == 1U)
    {
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_START_Msk | FLEX_TWI_CR_STOP_Msk;
    }
    else
    {
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_START_Msk;
    }

    __enable_irq();
    FLEXCOM10_REGS->FLEX_TWI_IER = FLEX_TWI_IER_RXRDY_Msk | FLEX_TWI_IER_TXCOMP_Msk;
}




static bool FLEXCOM10_TWI_InitiateTransfer(uint16_t address, bool type)
{
    uint32_t timeoutCntr = 160000;

    // 10-bit Slave Address
    if( address > 0x007FU )
    {
        FLEXCOM10_REGS->FLEX_TWI_MMR = FLEX_TWI_MMR_DADR(((uint32_t)address & 0x00007F00U) >> 8U) |
                                               FLEX_TWI_MMR_IADRSZ(1U);

        // Set internal address
        FLEXCOM10_REGS->FLEX_TWI_IADR = FLEX_TWI_IADR_IADR((uint32_t)address & 0x000000FFU );
    }
    // 7-bit Slave Address
    else
    {
        FLEXCOM10_REGS->FLEX_TWI_MMR = FLEX_TWI_MMR_DADR((uint32_t)address) | FLEX_TWI_MMR_IADRSZ(0U);
    }

    flexcom10TwiObj.writeCount= 0;
    flexcom10TwiObj.readCount= 0;

    // Write transfer
    if(type == false)
    {
        // Single Byte Write
        if( flexcom10TwiObj.writeSize == 1U )
        {
            // Single Byte write only
            if(  flexcom10TwiObj.readSize == 0U  )
            {
                // Load last byte in transmit register, issue stop condition
                // Generate TXCOMP interrupt after STOP condition has been sent
                flexcom10TwiObj.state = FLEXCOM_TWI_STATE_WAIT_FOR_TXCOMP;

                FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[0]);
                flexcom10TwiObj.writeCount++;
                FLEXCOM10_REGS->FLEX_TWI_CR =  FLEX_TWI_CR_STOP_Msk;
                FLEXCOM10_REGS->FLEX_TWI_IER = FLEX_TWI_IER_TXCOMP_Msk;
            }
            // Single Byte write and than read transfer
            else
            {
                // START bit must be set before the byte is shifted out. Hence disabled interrupt
                __disable_irq();
                FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[0]);
                flexcom10TwiObj.writeCount++;
                // Wait for control byte to be transferred before initiating repeat start for read
                while((FLEXCOM10_REGS->FLEX_TWI_SR & (FLEX_TWI_SR_TXCOMP_Msk | FLEX_TWI_SR_TXRDY_Msk)) != 0U)
                {

                }
                while((FLEXCOM10_REGS->FLEX_TWI_SR & (FLEX_TWI_SR_TXRDY_Msk)) == 0U)
                {
                    if (timeoutCntr == 0U)
                    {
                        flexcom10TwiObj.error = FLEXCOM_TWI_BUS_ERROR;
                        __enable_irq();
                        return false;
                    }
                    timeoutCntr--;
                }
                type=true;
            }
        }
        // Multi-Byte Write
        else
        {
            flexcom10TwiObj.state = FLEXCOM_TWI_STATE_TRANSFER_WRITE;

            FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[0]);
            flexcom10TwiObj.writeCount++;
            FLEXCOM10_REGS->FLEX_TWI_IER = FLEX_TWI_IDR_TXRDY_Msk | FLEX_TWI_IER_TXCOMP_Msk;
        }
    }
    // Read transfer
    if(type)
    {
        FLEXCOM10_TWI_InitiateRead();
    }

    return true;
}

// *****************************************************************************
/* Function:
    void FLEXCOM10_TWI_CallbackRegister(FLEXCOM_TWI_CALLBACK callback, uintptr_t contextHandle)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given FLEXCOM TWI's transfer events occur.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the FLEXCOM_TWI_CALLBACK data type.

    context - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

   Returns:
    None.
*/

void FLEXCOM10_TWI_CallbackRegister(FLEXCOM_TWI_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback != NULL)
    {
        flexcom10TwiObj.callback = callback;
        flexcom10TwiObj.context = contextHandle;
    }
}

// *****************************************************************************
/* Function:
    bool FLEXCOM10_TWI_IsBusy(void)

   Summary:
    Returns the Peripheral busy status.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    None.

   Returns:
    true - Busy.
    false - Not busy.
*/

bool FLEXCOM10_TWI_IsBusy(void)
{
    return (flexcom10TwiObj.state != FLEXCOM_TWI_STATE_IDLE );
}



// *****************************************************************************
/* Function:
    bool FLEXCOM10_TWI_Read(uint16_t address, uint8_t *pdata, size_t length)

   Summary:
    Reads data from the slave.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to destination data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool FLEXCOM10_TWI_Read(uint16_t address, uint8_t *pdata, size_t length)
{
    // Check for ongoing transfer
    if( flexcom10TwiObj.state != FLEXCOM_TWI_STATE_IDLE )
    {
        return false;
    }
    if ((FLEXCOM10_REGS->FLEX_TWI_SR & (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk)) != (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk))
    {
        flexcom10TwiObj.error = FLEXCOM_TWI_BUS_ERROR;
        return false;
    }

    flexcom10TwiObj.address=address;
    flexcom10TwiObj.readBuffer=pdata;
    flexcom10TwiObj.readSize=length;
    flexcom10TwiObj.writeBuffer=NULL;
    flexcom10TwiObj.writeSize=0;
    flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

    return FLEXCOM10_TWI_InitiateTransfer(address, true);
}

// *****************************************************************************
/* Function:
    bool FLEXCOM10_TWI_Write(uint16_t address, uint8_t *pdata, size_t length)

   Summary:
    Writes data onto the slave.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to source data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool FLEXCOM10_TWI_Write(uint16_t address, uint8_t *pdata, size_t length)
{
    // Check for ongoing transfer
    if( flexcom10TwiObj.state != FLEXCOM_TWI_STATE_IDLE )
    {
        return false;
    }
    if ((FLEXCOM10_REGS->FLEX_TWI_SR & (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk)) != (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk))
    {
        flexcom10TwiObj.error = FLEXCOM_TWI_BUS_ERROR;
        return false;
    }

    flexcom10TwiObj.address=address;
    flexcom10TwiObj.readBuffer=NULL;
    flexcom10TwiObj.readSize=0;
    flexcom10TwiObj.writeBuffer=pdata;
    flexcom10TwiObj.writeSize=length;
    flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

    return FLEXCOM10_TWI_InitiateTransfer(address, false);
}

// *****************************************************************************
/* Function:
    bool FLEXCOM10_TWI_WriteRead(uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength)

   Summary:
    Write and Read data from Slave.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    wdata   - pointer to write data buffer
    wlength - write data length in bytes.
    rdata   - pointer to read data buffer.
    rlength - read data length in bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool FLEXCOM10_TWI_WriteRead(uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength)
{

    // Check for ongoing transfer
    if( flexcom10TwiObj.state != FLEXCOM_TWI_STATE_IDLE )
    {
        return false;
    }
    if ((FLEXCOM10_REGS->FLEX_TWI_SR & (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk)) != (FLEX_TWI_SR_SDA_Msk | FLEX_TWI_SR_SCL_Msk))
    {
        flexcom10TwiObj.error = FLEXCOM_TWI_BUS_ERROR;
        return false;
    }

    flexcom10TwiObj.address=address;
    flexcom10TwiObj.readBuffer=rdata;
    flexcom10TwiObj.readSize=rlength;
    flexcom10TwiObj.writeBuffer=wdata;
    flexcom10TwiObj.writeSize=wlength;
    flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

    return FLEXCOM10_TWI_InitiateTransfer(address, false);
}

void FLEXCOM10_TWI_TransferAbort( void )
{
    flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

    // Reset the PLib objects and Interrupts
    flexcom10TwiObj.state = FLEXCOM_TWI_STATE_IDLE;
    FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXCOMP_Msk | FLEX_TWI_IDR_TXRDY_Msk | FLEX_TWI_IDR_RXRDY_Msk;

    // Disable and Enable I2C Master
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSDIS_Msk;
    FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSEN_Msk;
}

// *****************************************************************************
/* Function:
    FLEXCOM_TWI_ERROR FLEXCOM10_TWI_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    FLEXCOM10_TWI_Initialize must have been called for the associated FLEXCOM TWI instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/

FLEXCOM_TWI_ERROR FLEXCOM10_TWI_ErrorGet(void)
{
    FLEXCOM_TWI_ERROR error;

    error = flexcom10TwiObj.error;
    flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

    return error;
}

bool FLEXCOM10_TWI_TransferSetup(FLEXCOM_TWI_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t i2cClkSpeed;
    uint32_t cldiv;
    uint8_t ckdiv = 0;
    bool setupStatus = false;
    if (setup != NULL)
    {
        i2cClkSpeed = setup->clkSpeed;

        /* Maximum I2C clock speed in Master mode cannot be greater than 400 KHz */
        if (i2cClkSpeed <= 4000000U)
        {
            if( srcClkFreq == 0U)
            {
                srcClkFreq = 266666666;
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

            if (cldiv <= 255U)
            {
                // Set Baud rate
                FLEXCOM10_REGS->FLEX_TWI_CWGR = ( FLEX_TWI_CWGR_HOLD_Msk & FLEXCOM10_REGS->FLEX_TWI_CWGR) |
                                                        FLEX_TWI_CWGR_BRSRCCLK_PERIPH_CLK |
                                                        ( FLEX_TWI_CWGR_CLDIV(cldiv) |
                                                        FLEX_TWI_CWGR_CHDIV(cldiv) |
                                                        FLEX_TWI_CWGR_CKDIV((uint32_t)ckdiv) );
                setupStatus = true;
            }
        }
    }
    return setupStatus;
}

// *****************************************************************************
/* Function:
    void FLEXCOM10_InterruptHandler(void)

   Summary:
    FLEXCOM10_TWI Peripheral Interrupt Handler.

   Description:
    This function is FLEXCOM10_TWI Peripheral Interrupt Handler and will
    called on every FLEXCOM10_TWI interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/

void __attribute__((used)) FLEXCOM10_InterruptHandler(void)
{
    uint32_t status;
    uintptr_t context = flexcom10TwiObj.context;

    // Read the peripheral status
    status = FLEXCOM10_REGS->FLEX_TWI_SR;

    /* checks if Slave has Nacked */
    if( (status & FLEX_TWI_SR_NACK_Msk) != 0U )
    {
        flexcom10TwiObj.state = FLEXCOM_TWI_STATE_ERROR;
        flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NACK;
    }

    if( (status & FLEX_TWI_SR_TXCOMP_Msk) != 0U )
    {
        /* Disable and Enable I2C Master */
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSDIS_Msk;
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSEN_Msk;

        /* Disable Interrupt */
        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXCOMP_Msk |
                                 FLEX_TWI_IDR_TXRDY_Msk  |
                                 FLEX_TWI_IDR_RXRDY_Msk;
    }

    /* checks if the arbitration is lost in multi-master scenario */
    if( (status & FLEX_TWI_SR_ARBLST_Msk) != 0U )
    {
        /* Re-initiate the transfer if arbitration is lost in
         * between of the transfer
         */
        flexcom10TwiObj.state = FLEXCOM_TWI_STATE_ADDR_SEND;
    }

    if( flexcom10TwiObj.error == FLEXCOM_TWI_ERROR_NONE )
    {
        switch( flexcom10TwiObj.state )
        {
            case FLEXCOM_TWI_STATE_ADDR_SEND:
            {
                if (flexcom10TwiObj.writeSize != 0U )
                {
                    // Initiate Write transfer
                    (void)FLEXCOM10_TWI_InitiateTransfer(flexcom10TwiObj.address, false);
                }
                else
                {
                    // Initiate Read transfer
                    (void)FLEXCOM10_TWI_InitiateTransfer(flexcom10TwiObj.address, true);
                }
            }
            break;

            case FLEXCOM_TWI_STATE_TRANSFER_WRITE:
            {
                size_t  writeCount = flexcom10TwiObj.writeCount;

                /* checks if master is ready to transmit */
                if( (status & FLEX_TWI_SR_TXRDY_Msk) != 0U )
                {
                    // Write Last Byte and then initiate read transfer
                    bool lastByteWrPending = (writeCount == (flexcom10TwiObj.writeSize -1U));

                    if(( flexcom10TwiObj.readSize != 0U ) && (lastByteWrPending))
                    {
                        // START bit must be set before the last byte is shifted out to generate repeat start. Hence disabled interrupt
                        __disable_irq();
                        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXRDY_Msk;
                        FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[writeCount]);
                        writeCount++;
                        FLEXCOM10_TWI_InitiateRead();
                    }
                    // Write Last byte and then issue STOP condition
                    else if (lastByteWrPending)
                    {
                        // Load last byte in transmit register, issue stop condition
                        // Generate TXCOMP interrupt after STOP condition has been sent
                        FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[writeCount]);
                        writeCount++;
                        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_STOP_Msk;
                        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = FLEXCOM10_REGS->FLEX_TWI_SR;
                        if( (status & FLEX_TWI_SR_TXCOMP_Msk) != 0U )
                        {
                            flexcom10TwiObj.state = FLEXCOM_TWI_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            flexcom10TwiObj.state = FLEXCOM_TWI_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                    // Write next byte
                    else
                    {
                        FLEXCOM10_REGS->FLEX_TWI_THR = FLEX_TWI_THR_TXDATA((uint32_t)flexcom10TwiObj.writeBuffer[writeCount]);
                        writeCount++;
                    }

                    flexcom10TwiObj.writeCount = writeCount;

                    // Dummy read to ensure that TXRDY bit is cleared
                    (void)FLEXCOM10_REGS->FLEX_TWI_SR;
                }

                break;
            }

            case FLEXCOM_TWI_STATE_TRANSFER_READ:
            {
                /* checks if master has received the data */
                if( (status & FLEX_TWI_SR_RXRDY_Msk) != 0U)
                {
                    size_t readCount = flexcom10TwiObj.readCount;

                    // Set the STOP (or START) bit before reading the FLEX_TWI_RHR on the next-to-last access
                    if(  readCount == (flexcom10TwiObj.readSize - 2U) )
                    {
                        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_STOP_Msk;
                    }

                    /* read the received data */
                    flexcom10TwiObj.readBuffer[readCount] = (uint8_t)(FLEXCOM10_REGS->FLEX_TWI_RHR & FLEX_TWI_RHR_RXDATA_Msk);
                    readCount++;

                    /* checks if transmission has reached at the end */
                    if( readCount == flexcom10TwiObj.readSize )
                    {
                        /* Disable the RXRDY interrupt*/
                        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_RXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = FLEXCOM10_REGS->FLEX_TWI_SR;
                        if( (status & FLEX_TWI_SR_TXCOMP_Msk) != 0U )
                        {
                            flexcom10TwiObj.state = FLEXCOM_TWI_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            flexcom10TwiObj.state = FLEXCOM_TWI_STATE_WAIT_FOR_TXCOMP;
                        }
                    }

                    flexcom10TwiObj.readCount = readCount;
                }
                break;
            }

            case FLEXCOM_TWI_STATE_WAIT_FOR_TXCOMP:
            {
                if( (status & FLEX_TWI_SR_TXCOMP_Msk) != 0U )
                {
                    flexcom10TwiObj.state = FLEXCOM_TWI_STATE_TRANSFER_DONE;
                }
                break;
            }

            default:
            {
                /*default*/
                break;
            }
        }
    }

    if (flexcom10TwiObj.state == FLEXCOM_TWI_STATE_ERROR)
    {
        // NACK is received,
        flexcom10TwiObj.state = FLEXCOM_TWI_STATE_IDLE;
        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXCOMP_Msk | FLEX_TWI_IDR_TXRDY_Msk | FLEX_TWI_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSDIS_Msk;
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSEN_Msk;

        if ( flexcom10TwiObj.callback != NULL )
        {
            flexcom10TwiObj.callback( context );
        }
    }

    // check for completion of transfer
    if( flexcom10TwiObj.state == FLEXCOM_TWI_STATE_TRANSFER_DONE )
    {

        flexcom10TwiObj.error = FLEXCOM_TWI_ERROR_NONE;

        // Reset the PLib objects and Interrupts
        flexcom10TwiObj.state = FLEXCOM_TWI_STATE_IDLE;
        FLEXCOM10_REGS->FLEX_TWI_IDR = FLEX_TWI_IDR_TXCOMP_Msk |
                                 FLEX_TWI_IDR_TXRDY_Msk  |
                                 FLEX_TWI_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSDIS_Msk;
        FLEXCOM10_REGS->FLEX_TWI_CR = FLEX_TWI_CR_MSEN_Msk;
        if ( flexcom10TwiObj.callback != NULL )
        {
            flexcom10TwiObj.callback( context );
        }
    }

    return;
}

/*******************************************************************************
 End of File
*/
