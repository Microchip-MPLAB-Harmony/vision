/*******************************************************************************
  CSI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_csi.h

  Summary:
    CSI PLIB Header File

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

#ifndef PLIB_CSI_H
#define PLIB_CSI_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// 0x18 to 0x1F YUV Data Format
#define CSI2_DATA_FORMAT_YUV420_8   0x18
#define CSI2_DATA_FORMAT_YUV420_10  0x19
#define CSI2_DATA_FORMAT_YUV420_8L  0x1A
#define CSI2_DATA_FORMAT_RSRV10     0x1B
#define CSI2_DATA_FORMAT_YUV420_8C  0x1C
#define CSI2_DATA_FORMAT_YUV420_10C 0x1D
#define CSI2_DATA_FORMAT_YUV422_8   0x1E
#define CSI2_DATA_FORMAT_YUV422_10  0x1F
  
// 0x20 to 0x27 RGB Data Format
#define CSI2_DATA_FORMAT_RGB444     0x20
#define CSI2_DATA_FORMAT_RGB555     0x21
#define CSI2_DATA_FORMAT_RGB565     0x22
#define CSI2_DATA_FORMAT_RGB666     0x23
#define CSI2_DATA_FORMAT_RGB888     0x24
#define CSI2_DATA_FORMAT_RSRV11     0x25
#define CSI2_DATA_FORMAT_RSRV12     0x26
#define CSI2_DATA_FORMAT_RSRV13     0x27

// 0x28 to 0x2F RAW Data Format
#define CSI2_DATA_FORMAT_RAW6       0x28
#define CSI2_DATA_FORMAT_RAW7       0x29
#define CSI2_DATA_FORMAT_RAW8       0x2A
#define CSI2_DATA_FORMAT_RAW10      0x2B
#define CSI2_DATA_FORMAT_RAW12      0x2C
#define CSI2_DATA_FORMAT_RAW14      0x2D

/****************************** CSI API *********************************/

/* Function:
    void CSI_Shutdown(void)

  Summary:
    This function shutdown CSI D-PHY.

  Description:
    Shutdown mode is operating mode with lowest power consumption.
    All analog blocks are disabled and digital logic is reset. To
    enter this mode, write ?0? to the Reset register (CSI_DPHY_RSTZ)
    and Shutdown register (CSI_PHY_SHUTDOWNZ).

  Precondition:
    CSI D-PHY should be in Reset mode.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Shutdown();
    </code>

  Remarks:
    None.
*/
void CSI_Shutdown(void);

/* Function:
    void CSI_Exit_Shutdown(void)

  Summary:
    This function exits CSI D-PHY shutdown mode.

  Description:
    CSI D-PHY exits Shutdown mode and starts an initialization procedure.

  Precondition:
    CSI D-PHY should have exited reset mode.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Exit_Shutdown();
    </code>

  Remarks:
    None.
*/
void CSI_Exit_Shutdown(void);        
        
/* Function:
    void CSI_Configure_Lane(uint8_t lane)

  Summary:
    This function configure number of active data lanes.

  Description:
    The CSI D-PHY comprises 4 data lanes, in some scenario it may require fewer.
    Number of lanes is configured in Lane Configuration register (CSI_N_LANES).

  Precondition:
    Configure only when CSI DPHY is in Shutdown mode.

  Parameters:
    lane - Numbers of lanes (0 - 3).
 
  Returns:
    None.

  Example:
    <code>
    CSI_Configure_Lane();
    </code>

  Remarks:
    None.
*/        
void CSI_Configure_Lane(uint8_t lane);

/* Function:
    void CSI_Reset(void)

  Summary:
    This function enters CSI internal controller logic in reset state.

  Description:
    Enters CSI internal controller logic in reset state.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Reset();
    </code>

  Remarks:
    None.
*/
void CSI_Reset(void);

/* Function:
    void CSI_Exit_Reset(void)

  Summary:
    This function exists CSI reset state.

  Description:
    Exits CSI reset state.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Exit_Reset();
    </code>

  Remarks:
    None.
*/
void CSI_Exit_Reset(void);


/* Function:
    void CSI_Reset_DPhy(void)

  Summary:
    This function enters CSI D-PHY in reset state.

  Description:
    Enters CSI D-PHY in reset state.

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Reset_DPhy();
    </code>

  Remarks:
    None.
*/
void CSI_Reset_DPhy(void);

/* Function:
    void CSI_Exit_Reset_DPhy(void)

  Summary:
    This function exists CSI D-PHY reset state.

  Description:
    Exits CSI D-PHY reset state..

  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Exit_Reset_DPhy();
    </code>

  Remarks:
    None.
*/
void CSI_Exit_Reset_DPhy(void);

/* Function:
    void CSI_Configure_DataId(uint8_t id, uint8_t vchannel, uint8_t datatype)

  Summary:
    This function configures DataID definition for data format and virtual channel
    identifiers

  Description:
    This function configures DataID definition for data format and virtual channel
    identifiers

  Precondition:
    None.

  Parameters:
    id - Data Id.
    vchannel - Virtual channel number
    datatype - Data format type
 
  Returns:
    None.

  Example:
    <code>
    CSI_Configure_DataId();
    </code>

  Remarks:
    None.
*/
void CSI_Configure_DataId(uint8_t id, uint8_t vchannel, uint8_t datatype);

/* Function:
    void CSI_Analog_Init(uint32_t bit_rate)

  Summary:
    This function configure number of active data lanes.

  Description:
    Follow the steps listed below for configuration:
    1. Reset the analog configuration by generating a high pulse on CSI_PHY_TEST_CTRL0. PHY_TESTCLR.
    2. Write '1' to CSI_PHY_TEST_CTRL0.PHY_TESTCLK.
    3. Write 0x44 to CSI_PHY_TEST_CTRL1.PHY_TESTDIN and write '1' to CSI_PHY_TEST_CTRL1.PHY_TESTEN.
    4. Write a '0' to CSI_PHY_TEST_CTRL0.PHY_TESTCLK to create a falling edge on PHY_TESTCLK.
    5. Write a '0' to CSI_PHY_TEST_CTRL1.PHY_TESTEN and write the configuration value from the following
       table to CSI_PHY_TEST_CTRL1.PHY_TESTDIN.
    6. Write a high pulse to CSI_PHY_TEST_CTRL0.PHY_TESTCLK by writing ?1? immediately followed by '0'.

  Precondition:
     Ensure CSI D-PHY is in Shutdown mode..

  Parameters:
    bit_rate - bit rate of the data lines.
 
  Returns:
    None.

  Example:
    <code>
    CSI_Analog_Init();
    </code>

  Remarks:
    None.
*/
void CSI_Analog_Init(uint8_t bit_rate);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif
