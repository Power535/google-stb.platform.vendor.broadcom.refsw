#
# BCM947xx Linux External Router Full Source build/release Makefile
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

PARENT_MAKEFILE := linux-external-router.mk
CHECKOUT_RULES  := checkout-rules.mk
DEFAULT_TARGET  := default
ALL_TARGET      := all

$(DEFAULT_TARGET): $(ALL_TARGET)

ROUTERSRC := true
KERNELCFG := defconfig-bcm947xx-router
ROUTERCFG := defconfig-vista-router
HNDSVN_BOM:= linux-vistapremium.sparse

# Include custom (if exists) or central checkout rules
include $(strip $(firstword $(wildcard \
        $(CHECKOUT_RULES) \
        /home/hwnbuild/src/tools/release/$(CHECKOUT_RULES) \
        /projects/hnd_software/gallery/src/tools/release/$(CHECKOUT_RULES))))

include $(PARENT_MAKEFILE)

DEFS += FULL ROUTERSRC WLSRC NASSRC EZCSRC SESSRC BCMWPS BCMVISTAROUTER
UNDEFS := $(filter-out FULL ROUTERSRC WLSRC NASSRC EZCSRC SESSRC BCMWPS BCMVISTAROUTER BCMDBG BCMDBG_ERR,$(UNDEFS))
SRCFILELISTS_COMPONENTS += src/tools/release/components/wps-filelist.txt
