
#ifndef DRV_CSI_H    /* Guard against multiple inclusion */
#define DRV_CSI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "vision/drivers/image_sensor/drv_image_sensor.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
    // DOM-IGNORE-END

    typedef struct {
        uint8_t numLanes;
        uint8_t csiDataType;
        uint8_t csiDataId;
        uint8_t csiVirtualChannel;
        uint32_t linkFreqMHz;
    } DRV_CSI_OBJ;

    bool DRV_CSI_Configure(DRV_CSI_OBJ* devObj);

    DRV_CSI_OBJ* DRV_CSI_Initalize(DRV_IMAGE_SENSOR_OBJ* iSensor);

    // DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
#endif
