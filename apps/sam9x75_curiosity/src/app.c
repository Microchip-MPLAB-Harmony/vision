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

#include "app.h"

#define ISC_CANVAS_ID 0
#define ISC_CANVAS_LAYER 1

APP_DATA appData;

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

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
}

void APP_Tasks ( void )
{
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized)
            {    
                gfxcSetLayer(ISC_CANVAS_ID, ISC_CANVAS_LAYER);
                gfxcShowCanvas(ISC_CANVAS_ID);
                appData.state = APP_STATE_CAMERA_OPEN;
            }
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
