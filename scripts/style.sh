#!/bin/sh

#
# Copyright (C) 2023 Microchip Technology Inc.  All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
# Script to enforce the coding style of this project.
#
#
#


ASTYLE_BIN=${ASTYLE_BIN:-astyle}

if [ "$(${ASTYLE_BIN} -V)" \< "Artistic Style Version 3.1" ]
then
    echo "Your astyle version is too old. Update astyle to at least version 3.1"
    exit 1
fi

set -x

OPTIONS="--style=allman \
--suffix=none \
--unpad-paren \
--pad-comma \
--pad-oper \
--pad-header \
--mode=c \
--preserve-date \
--lineend=linux \
--indent-col1-comments \
--align-reference=type \
--align-pointer=type \
--keep-one-line-statements \
--keep-one-line-blocks \
--min-conditional-indent=0 \
--attach-closing-while \
--indent=spaces=4"

filelist=("./drivers/csi/inc/drv_csi.h.ftl"  
 "./drivers/csi/src/drv_csi.c.ftl"
 "./drivers/csi2dc/inc/drv_csi2dc.h.ftl" 
 "./drivers/csi2dc/src/drv_csi2dc.c.ftl" 
 "./drivers/image_sensor/inc/drv_image_sensor.h.ftl" 
 "./drivers/image_sensor/src/drv_image_sensor.c.ftl" 
 "./drivers/isc/inc/drv_isc.h.ftl" 
 "./drivers/isc/src/drv_isc.c.ftl" 
 "./image_sensors/imx219/src/imx219.c.ftl" 
 "./image_sensors/ov5640/src/ov5640.c.ftl"
 "./image_sensors/ov5647/src/ov5640.c.ftl"
 "./image_sensors/ov5647/src/ov5647.c.ftl"
 "./middleware/libcamera/inc/camera.h.ftl" 
 "./middleware/libcamera/src/camera.c.ftl"
 "./peripheral/csi/inc/plib_csi.h.ftl"
 "./peripheral/csi/src/plib_csi.c.ftl"
 "./peripheral/csi2dc/inc/plib_csi2dc.h.ftl" 
 "./peripheral/csi2dc/src/plib_csi2dc.c.ftl" 
 "./peripheral/isc/inc/plib_isc.h.ftl" 
 "./peripheral/isc/src/plib_isc.c.ftl" ) 

for str in ${filelist[@]}; do
  ${ASTYLE_BIN} ${OPTIONS} $str
done

