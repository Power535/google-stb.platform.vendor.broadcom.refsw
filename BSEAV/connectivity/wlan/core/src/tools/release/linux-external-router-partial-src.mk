#
# BCM947xx Linux External Router Partial Source build/release Makefile
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

# Include custom (if exists) or central checkout rules
include $(strip $(firstword $(wildcard \
        $(CHECKOUT_RULES) \
        /home/hwnbuild/src/tools/release/$(CHECKOUT_RULES) \
        /projects/hnd_software/gallery/src/tools/release/$(CHECKOUT_RULES))))

include $(PARENT_MAKEFILE)

DEFS   += ROUTERSRC
UNDEFS := $(filter-out ROUTERSRC,$(UNDEFS))

# The following removal of SES is temporary till we are allowed to
# ship SES to all customers
DEFS := $(filter-out __CONFIG_SES__ __CONFIG_SES_CL__,$(DEFS))
UNDEFS += __CONFIG_SES__ __CONFIG_SES_CL__
