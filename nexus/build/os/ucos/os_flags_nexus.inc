############################################################
#     (c)2003-2013 Broadcom Corporation
#
#  This program is the proprietary software of Broadcom Corporation and/or its licensors,
#  and may only be used, duplicated, modified or distributed pursuant to the terms and
#  conditions of a separate, written license agreement executed between you and Broadcom
#  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
#  no license (express or implied), right to use, or waiver of any kind with respect to the
#  Software, and Broadcom expressly reserves all rights in and to the Software and all
#  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
#  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
#  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
#
#  Except as expressly set forth in the Authorized License,
#
#  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
#  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
#  and to use this information only in connection with your use of Broadcom integrated circuit products.
#
#  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
#  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
#  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
#  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
#  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
#  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
#  USE OR PERFORMANCE OF THE SOFTWARE.
#
#  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
#  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
#  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
#  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
#  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
#  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
#  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
#  ANY LIMITED REMEDY.
#
# $brcm_Workfile: $
# $brcm_Revision: $
# $brcm_Date: $
#
# Module Description:
#   This file defines the NEXUS OS-specific flags for building in linuxuser mode.
#
# Revision History:
#
# $brcm_Log: $
#
############################################################

############################################################
# Overview:
#
# this file should only be included by nexus.inc
# it is only used for internal nexus build
#
############################################################

#
# Default to -Os by default
#
B_REFSW_OPTIMIZER_LEVEL ?= s

#
# Default to PIC code by default
#
ifndef B_REFSW_SHAREABLE
B_REFSW_SHAREABLE := n
endif

ifeq ($(B_REFSW_DEBUG),y)
NEXUS_CFLAGS += -g -O${B_REFSW_OPTIMIZER_LEVEL}
else
NEXUS_CFLAGS += -O${B_REFSW_OPTIMIZER_LEVEL}
endif

#
# Allow for shared and non-shared builds.  Shared will be relocatable, non-shared will not.
#
ifeq ($(B_REFSW_SHAREABLE),y)
NEXUS_OBJ_SUFFIX := lo
NEXUS_CFLAGS += -DPIC -fpic
NEXUS_SHARED_LIB := $(NEXUS_OBJ_DIR)/libnexus${NEXUS_LIB_SUFFIX}.so
else
NEXUS_OBJ_SUFFIX := o
NEXUS_STATIC_LIB := $(NEXUS_OBJ_DIR)/libnexus${NEXUS_LIB_SUFFIX}.a
MAGNUM_LIB := $(NEXUS_OBJ_DIR)/libmagnum${NEXUS_LIB_SUFFIX}.a
# compile code so that unused code can be removed. this also requires LDFLAGS has -Wl,--gc-sections
NEXUS_CFLAGS += -fdata-sections -ffunction-sections
endif

NEXUS_MAGNUM_PRECOMPILED_HEADER_H := ${NEXUS_OBJ_DIR}/MAGNUM/$(notdir ${NEXUS_MAGNUM_PRECOMPILED_HEADER})
NEXUS_PRECOMPILED_HEADER_H := ${NEXUS_TOP}/base/include/$(notdir ${NEXUS_PRECOMPILED_HEADER})


