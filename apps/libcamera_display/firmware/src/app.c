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


#define CANVAS_XPOS    0
#define CANVAS_YPOS    0
#define CANVAS_WIDTH   640
#define CANVAS_HEIGHT  480

#define ISC_CANVAS_ID 0
#define ISC_CANVAS_LAYER 2 /* HEO Layer */

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

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void camera_callback(uintptr_t context)
{
    uint32_t addr = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    
    if(context == SYS_MODULE_OBJ_INVALID)
        return;
    
    SYS_MODULE_OBJ object = (SYS_MODULE_OBJ) context;

    CAMERA_Get_Frame(object, &addr, &width, &height);
    if (addr != 0)
    { 
        gfxcSetPixelBuffer(ISC_CANVAS_ID, width, height, GFX_COLOR_MODE_RGB_565, (void *)addr);  
        gfxcCanvasUpdate(ISC_CANVAS_ID);
    }
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
    appData.state = APP_STATE_INIT;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    uint32_t bufferPtr = 0;
    uint32_t width = CANVAS_WIDTH;
    uint32_t height = CANVAS_HEIGHT;
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized)
            {
                gfxcSetLayer(ISC_CANVAS_ID, ISC_CANVAS_LAYER);
                gfxcSetWindowSize(ISC_CANVAS_ID, width, height);
                gfxcSetWindowPosition(ISC_CANVAS_ID, 80, 0);    
                gfxcShowCanvas(ISC_CANVAS_ID);
                
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }
        case APP_STATE_SERVICE_TASKS:
        {
            // START OF CUSTOM CODE
            static bool once = true;            
            if(once)
            {
                /* Enable AC69T88A Display Backlight */
                AC69T88A_BACKLIGHT_EN_Set();
                once = false;
            }
            appData.state = APP_STATE_CAMERA_OPEN;
            break;
        }        
        case APP_STATE_CAMERA_OPEN:
        {
            if (sysObj.devCamera != SYS_MODULE_OBJ_INVALID)
            {
                CAMERA_Register_CallBack(camera_callback, sysObj.devCamera);
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
                CAMERA_Start_Capture(sysObj.devCamera);
                appData.state = APP_STATE_IDLE;    
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r CAMERA_Start_Capture : success \n\r");
            }
            break;
        }
        case APP_STATE_CAMERA_GET_FRAME:
        {            
            if (sysObj.devCamera != SYS_MODULE_OBJ_INVALID)
            {
                bufferPtr = 0;
                CAMERA_Stop_Capture(sysObj.devCamera);
                CAMERA_Get_Frame(sysObj.devCamera, &bufferPtr, &width, &height);
                appData.state = APP_STATE_CANVAS_UPDATE;
            }
            break;
        }        
        case APP_STATE_CANVAS_UPDATE:
        {
            if (bufferPtr != 0)
            {       
                gfxcSetPixelBuffer(ISC_CANVAS_ID, width, height, GFX_COLOR_MODE_RGB_565, (void *)bufferPtr);  
                gfxcCanvasUpdate(ISC_CANVAS_ID);
            }
            appData.state = APP_STATE_CAMERA_START;            
            break;
        }
        case APP_STATE_IDLE:
        {
            appData.state = APP_STATE_IDLE;            
            break;
        }       
        case APP_STATE_ERROR:
        default:
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r APP_STATE_ERROR \n\r");
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
