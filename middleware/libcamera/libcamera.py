# coding: utf-8
##############################################################################
# Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

def instantiateComponent(component):
    LIBCAMERA_C = component.createFileSymbol("LIBCAMERA_C", None)
    LIBCAMERA_C.setDestPath("vision/libcamera/")
    LIBCAMERA_C.setOutputName("camera.c")
    LIBCAMERA_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/libcamera")
    LIBCAMERA_C.setType("SOURCE")
    LIBCAMERA_C.setMarkup(True)
    LIBCAMERA_C.setSourcePath("src/camera.c.ftl")

    LIBCAMERA_H = component.createFileSymbol("LIBCAMERA_H", None)
    LIBCAMERA_H.setDestPath("vision/libcamera")
    LIBCAMERA_H.setOutputName("camera.h")
    LIBCAMERA_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/libcamera")
    LIBCAMERA_H.setType("HEADER")
    LIBCAMERA_H.setMarkup(True)
    LIBCAMERA_H.setSourcePath("inc/camera.h.ftl")

    LIBCAMERA_INIT_DATA = component.createFileSymbol("LIBCAMERA_INIT_DATA", None)
    LIBCAMERA_INIT_DATA.setType("STRING")
    LIBCAMERA_INIT_DATA.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    if any(x in Variables.get("__PROCESSOR") for x in [ "SAM9X7", "SAMA7"]):
        LIBCAMERA_INIT_DATA.setSourcePath("templates/camera_init.ftl")
    else:
        LIBCAMERA_INIT_DATA.setSourcePath("templates/camera_init_sama5d2.ftl")
    LIBCAMERA_INIT_DATA.setMarkup(True)

    LIBCAMERA_OBJ_DATA = component.createFileSymbol("LIBCAMERA_OBJ_DATA", None)
    LIBCAMERA_OBJ_DATA.setType("STRING")
    LIBCAMERA_OBJ_DATA.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    LIBCAMERA_OBJ_DATA.setSourcePath("templates/camera_definitions_obj.h.ftl")
    LIBCAMERA_OBJ_DATA.setMarkup(True)

    LIBCAMERA_DEFINITIONS_H = component.createFileSymbol("LIBCAMERA_DEFINITIONS_H", None)
    LIBCAMERA_DEFINITIONS_H.setType("STRING")
    LIBCAMERA_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    LIBCAMERA_DEFINITIONS_H.setSourcePath("templates/camera_definitions.h.ftl")
    LIBCAMERA_DEFINITIONS_H.setMarkup(True)

    LIBCAMERA_INIT_C = component.createFileSymbol("LIBCAMERA_INIT_C", None)
    LIBCAMERA_INIT_C.setType("STRING")
    LIBCAMERA_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    LIBCAMERA_INIT_C.setSourcePath("templates/camera_initialize.c.ftl")
    LIBCAMERA_INIT_C.setMarkup(True)

    LIBCAMERA_SYS_TASK_C = component.createFileSymbol("LIBCAMERA_SYS_TASK_C", None)
    LIBCAMERA_SYS_TASK_C.setType("STRING")
    LIBCAMERA_SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    LIBCAMERA_SYS_TASK_C.setSourcePath("templates/camera_tasks.c.ftl")
    LIBCAMERA_SYS_TASK_C.setMarkup(True)

    LIBCAMERA_ENABLE_AWB = component.createBooleanSymbol("LibcameraEnableAWB", None)
    LIBCAMERA_ENABLE_AWB.setLabel("Enable Auto White Balance Algorithm")
    LIBCAMERA_ENABLE_AWB.setDefaultValue(True)

    LIBCAMERA_ENABLE_DEBUG = component.createBooleanSymbol("LibcameraEnableDebug", None)
    LIBCAMERA_ENABLE_DEBUG.setLabel("Enable Debug Messages")
    LIBCAMERA_ENABLE_DEBUG.setDefaultValue(False)

    LIBCAMERA_CONFIG = component.createFileSymbol("LIBCAMERA_CONFIG_H", None)
    LIBCAMERA_CONFIG.setType("STRING")
    LIBCAMERA_CONFIG.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    LIBCAMERA_CONFIG.setSourcePath("templates/camera_defines.ftl")
    LIBCAMERA_CONFIG.setMarkup(True)

