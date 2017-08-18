#!/bin/bash
#
# Broadcom Proprietary and Confidential. Copyright (C) 2017,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
# Check all files in a GPL license tree for Broadcom header comment
#
# $Id$
#

TMP=/tmp/gplcheck$$

trap 'rm -f $TMP' INT EXIT

if [ "$1" = "" ]; then
    echo >&2 "$0: Missing directory argument"
    exit 1
fi

if [ ! -d "$1/src" ]; then
    echo >&2 "$0: $1/src directory not found"
    exit 1
fi

grep -l -r "UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation" "$1/src" > $TMP
nongpl=$?

if [ $nongpl -eq 0 ]; then
    prefix="ERROR: Found file in the GPL tarball with proprietary Broadcom header comment: "
    sed -e "s/^/$prefix/" $TMP >&2
    exit 1;
else
    echo "GPL Header Check Done"
fi
