/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
static uint32_t filenumber = 0;
char fileName[32];
uint8_t writeData[640*480*4];

/*Reference: https://en.wikipedia.org/wiki/BMP_file_format*/
static uint8_t bmpfileheader[54]; // = {'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0}; /*54 is the location of the image*/
//static uint8_t bmpinfoheader[40];

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void user_button_callback(PIO_PIN pin, uintptr_t context)
{
    appData.state = APP_STATE_OPEN_FILE;
}

static size_t writeCanvasToFile(SYS_FS_HANDLE handle,
    uint8_t *buffer, uint32_t w, uint32_t h)
{
    int imagesize = 4 * w * h;
    int filesize = 54 + imagesize;  /*w is your image width, h is image height, both int*/
    size_t ret;
    /*The file header offset 2 has the size of the BMP file in bytes*/
    bmpfileheader[0] = 'B';
    bmpfileheader[1] = 'M';
    bmpfileheader[2] = (uint8_t)(filesize);
    bmpfileheader[3] = (uint8_t)(filesize >> 8);
    bmpfileheader[4] = (uint8_t)(filesize >> 16);
    bmpfileheader[5] = (uint8_t)(filesize >> 24);
    
    bmpfileheader[10] = 54;
    
    //The info header size
    bmpfileheader[14] = 40;
    // The info header has the width
    bmpfileheader[18] = bmpfileheader[38] = (uint8_t)(w);
    bmpfileheader[19] = bmpfileheader[39] = (uint8_t)(w >> 8);
    bmpfileheader[20] = bmpfileheader[40] = (uint8_t)(w >> 16);
    bmpfileheader[21] = bmpfileheader[41] = (uint8_t)(w >> 24);
    
    // The info header has the height
    bmpfileheader[22] = bmpfileheader[42] = (uint8_t)(h);
    bmpfileheader[23] = bmpfileheader[43] = (uint8_t)(h >> 8);
    bmpfileheader[24] = bmpfileheader[44] = (uint8_t)(h >> 16);
    bmpfileheader[25] = bmpfileheader[48] = (uint8_t)(h >> 24);
    
    //the number of color planes (must be 1)
    bmpfileheader[26] = 1;
    bmpfileheader[27] = 0;
    
    //the number of bits per pixel, which is the color depth of the image.
    //Typical values are 1, 4, 8, 16, 24 and 32.
    bmpfileheader[28] = 32;
    bmpfileheader[29] = 0;
    
    bmpfileheader[34] = (uint8_t)(imagesize);
    bmpfileheader[35] = (uint8_t)(imagesize >> 8);
    bmpfileheader[36] = (uint8_t)(imagesize >> 16);
    bmpfileheader[37] = (uint8_t)(imagesize >> 24);
    
    // write header and data to file
    ret = SYS_FS_FileWrite(handle, (const void *) bmpfileheader, sizeof(bmpfileheader));

    SYS_FS_FileWrite(handle, (const void *)&buffer[0], imagesize);
    
    return ret;
}


void camera_callback(uintptr_t context)
{
    uint32_t addr = 0;
    uint32_t width = 640;
    uint32_t height = 480;    
    size_t size = 0;
    
    if(context == SYS_MODULE_OBJ_INVALID)
        return;
    
    SYS_MODULE_OBJ object = (SYS_MODULE_OBJ) context;

    CAMERA_Get_Frame(object, &addr, &width, &height);
    
    if (addr != 0)
    {
        size = width * height * 4;
        memcpy((void*) writeData, (const void *) addr, size);
        appData.state = APP_STATE_WRITE_TO_FILE;
    }
    
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r camera_callback : Done \n\r");
}

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
                    
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
            appData.deviceIsConnected = true;
            
            break;
            
        case SYS_FS_EVENT_UNMOUNT:
            appData.deviceIsConnected = false;
            break;
            
        default:
            break;
    }
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_SYSFSEventHandler : Done \n\r");
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_BUS_ENABLE;
    appData.deviceIsConnected = false;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    switch(appData.state)
    {
        case APP_STATE_BUS_ENABLE:
        {              
           /* Set the event handler and enable the bus */
            SYS_FS_EventHandlerSet((void *)APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(USB_HOST_BUS_ALL);
            
            
            CAMERA_Register_CallBack(camera_callback, sysObj.devCamera);
    
            PIO_PinInterruptEnable(PIO_PIN_PA12);
            PIO_PinInterruptCallbackRegister(PIO_PIN_PA12, user_button_callback, 0);
                
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_BUS_ENABLE : Done \n\r");
            break;
        }    
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
            if(USB_HOST_BusIsEnabled(USB_HOST_BUS_ALL))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE : Done \n\r");
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        }
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
        {
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(appData.deviceIsConnected)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_WAIT_FOR_DEVICE_ATTACH : Done \n\r");
                appData.state = APP_STATE_DEVICE_CONNECTED;
            }
            break;
        }
        case APP_STATE_DEVICE_CONNECTED:
        {
            /* Device was connected. We can try mounting the disk */
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_DEVICE_CONNECTED : Done \n\r");
            appData.state = APP_STATE_OPEN_FILE;
            break;
        }
        case APP_STATE_OPEN_FILE:
        {
            /* Try opening the file for append */
            sprintf(fileName,
                "/mnt/myDrive1/file_%ld.bmp",
                filenumber++);
            appData.fileHandle = SYS_FS_FileOpen(fileName, (SYS_FS_FILE_OPEN_APPEND_PLUS));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r SYS_FS_FileOpen : error \n\r");
                appData.state = APP_STATE_ERROR;

            }
            else
            {
                /* File opened successfully. Write to file */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_OPEN_FILE : Done \n\r");
                appData.state = APP_STATE_CAMERA_OPEN;
            }
            break;
        }
        case APP_STATE_CAMERA_OPEN:
        {
            if (sysObj.devCamera != SYS_MODULE_OBJ_INVALID)
            {
                if (CAMERA_Open(sysObj.devCamera))
                {
                    appData.state = APP_STATE_CAMERA_START;    
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r CAMERA_Open : success \n\r");                    
                }
                else
                {                    
                    appData.state = APP_STATE_ERROR;
                }
            }
            break;
        }        
        case APP_STATE_CAMERA_START:
        {            
            if (sysObj.devCamera != SYS_MODULE_OBJ_INVALID)
            {
                LED_GREEN_Off();
                LED_BLUE_On();
                CAMERA_Start_Capture(sysObj.devCamera);
                appData.state = APP_STATE_IDLE;    
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r CAMERA_Start_Capture : success \n\r");
            }
            break;
        }
        case APP_STATE_WRITE_TO_FILE:
        {
            if(writeCanvasToFile(appData.fileHandle, (uint8_t*) writeData, 640, 480) == -1)
            {
                /* Write was not successful. Close the file
                 * and error out.*/
                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_STATE_ERROR;
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r SYS_FS_FileWrite : error \n\r");
            }
            else
            {
                /* We are done writing. Close the file */
                appData.state = APP_STATE_CLOSE_FILE;
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_WRITE_TO_FILE : Done \n\r");
            }
            break;
        }
        case APP_STATE_CLOSE_FILE:
        {    
            if (sysObj.devCamera != SYS_MODULE_OBJ_INVALID)
            {
                CAMERA_Stop_Capture(sysObj.devCamera);
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r CAMERA_Stop_Capture : success \n\r");
            }
            
            /* Close the file */
            SYS_FS_FileClose(appData.fileHandle);
            
            /* Indicate User that File operation has been completed */
            LED_BLUE_Off();
            LED_GREEN_On();
            /* The test was successful. Lets idle. */
            appData.state = APP_STATE_IDLE;
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_CLOSE_FILE : Done \n\r");
            break;
        }
        case APP_STATE_IDLE:
        {
            /* The application comes here when the demo has completed
             * successfully. Provide LED indication. Wait for device detach
             * and if detached, wait for attach. */

            if(appData.deviceIsConnected == false)
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        }
        case APP_STATE_ERROR:
        {
            /* The application comes here when the demo
             * has failed. Provide LED indication .*/

            if(SYS_FS_Unmount("/mnt/myDrive1") != 0)
            {
                /* The disk could not be un mounted. Try
                 * un mounting again untill success. */

                appData.state = APP_STATE_ERROR;
            }
            else
            {
                /* UnMount was successful. Wait for device attach */
                appData.state =  APP_STATE_WAIT_FOR_DEVICE_ATTACH;
                appData.deviceIsConnected = false; 

            }
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_ERROR : Done \n\r");
            break;
        }
        default:
            break;
    }
}

/*******************************************************************************
 End of File
 */
