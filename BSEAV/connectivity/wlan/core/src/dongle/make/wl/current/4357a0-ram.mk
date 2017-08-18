# Makefile for hndrte based 4357a0 full RAM image building
#
# Broadcom Proprietary and Confidential. Copyright (C) 2017,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
# $Id:$
# chip specification
include $(TOPDIR)/current/4347a0-ram.mk

CLM_TYPE	:= 4357a0
CLM_BLOBS	+= 4357a0

# Option to use discovery engine in fw or host
WL_NAN_DISC	:= 1
