#
# BCM947xx Linux External AP build/release Makefile
#
# Broadcom Proprietary and Confidential. Copyright (C) 2017,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
# $Id$
#

KERNELCFG := defconfig-bcm947xx-ap
ROUTERCFG := defconfig-ap

include linux-external-router.mk

%.mk: OVFILE=$(if $(OVERRIDE),$(OVERRIDE)/tools/release/$@,)
%.mk:
	cvs co $(if $(TAG),-r $(TAG)) -p src/tools/release/$@ > $@
ifneq ($(OVERRIDE),)
	-[ -f "$(OVFILE)" ] && cp $(OVFILE) $@
endif
