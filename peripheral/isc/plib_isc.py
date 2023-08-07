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
    PLIB_ISC_C = component.createFileSymbol("PLIB_ISC_C", None)
    PLIB_ISC_C.setDestPath("vision/peripheral/isc/")
    PLIB_ISC_C.setOutputName("plib_isc.c")
    PLIB_ISC_C.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/isc")
    PLIB_ISC_C.setType("SOURCE")
    PLIB_ISC_C.setMarkup(True)
    PLIB_ISC_C.setSourcePath("src/plib_isc.c.ftl")
    
    PLIB_ISC_H = component.createFileSymbol("PLIB_ISC_H", None)
    PLIB_ISC_H.setDestPath("vision/peripheral/isc/")
    PLIB_ISC_H.setOutputName("plib_isc.h")
    PLIB_ISC_H.setProjectPath("config/" + Variables.get("__CONFIGURATION_NAME") + "/vision/peripheral/isc")
    PLIB_ISC_H.setType("HEADER")
    PLIB_ISC_H.setMarkup(True)
    PLIB_ISC_H.setSourcePath("inc/plib_isc.h.ftl")
