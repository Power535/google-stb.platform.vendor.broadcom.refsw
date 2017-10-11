
# Makefile for hndrte based 4345c0 full RAM image
#
# Broadcom Proprietary and Confidential. Copyright (C) 2017,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
# $Id: $:

# chip specification
CHIP		:= 4345
REV		:= c0
REVID		:= 6
PCIEREVID	:= 14
CCREV		:= 51
PMUREV		:= 27
GCIREV		:= 2
AOBENAB		:= 0
OTPWRTYPE	:= 1
BUSCORETYPE	:= PCIE2_CORE_ID

# default targets
TARGETS := \
	sdio-ag-pool \
	pcie-ag-msgbuf-pool \

# common target attributes
TARGET_ARCH	:= arm
TARGET_CPU	:= cr4
TARGET_HBUS	:= sdio pcie
THUMB		:= 1
HBUS_PROTO	:= cdc msgbuf
NODIS		:= 0
BCM_DNGL_BL_PID := 0xbd24

# wlconfig & wltunable for rest of the build targets
WLCONFFILE	:= wlconfig_rte_4345c0_bu
WLTUNEFILE	:= wltunable_rte_4345c0.h

TEXT_START := 0x00198000

# REMAIN is exported from rte/wl/Makefile. this contains the string specifying the bus (pci)
ifeq ($(REMAIN),)
$(error $(REMAIN) is undefined)
endif

REMAIN := $(subst /,,$(REMAIN))

# features (sync with 4345c0.mk, 4345c0-roml.mk, 4334a0-romlsim-4345c0.mk)
MEMBASE		:= 0x00198000
MEMSIZE		:= 819200
MFGTEST         := 0
WLTINYDUMP	:= 0
DBG_ASSERT	:= 0
DBG_ASSERT_TRAP := 1
DBG_ERROR	:= 0
WLRXOV		:= 0

# Memory reduction features:
# - HNDLBUFCOMPACT: Compacts head/end pointers in lbuf to single word
#   To disable set HNDLBUFCOMPACT = 0
# - BCMPKTIDMAP: Suppresses pkt pointers to Ids in lbuf<next,link>, pktpool, etc
#   Must specify max number of packets (various pools + heap)
HNDLBUFCOMPACT  := 1
BCMPKTIDMAP     := 1
BCMFRAGPOOL     := 1
BCMLFRAG        := 1
BCMSPLITRX      := 1

POOL_LEN_MAX	:= 64
POOL_LEN	:= 6
WL_POST_FIFO1   := 2
MFGTESTPOOL_LEN := 10
# Reduce packet pool lens for internal assert
# builds to fix out of memory issues
FRAG_POOL_LEN   := 64
RXFRAG_POOL_LEN := 64

ifeq ($(call opt,pcie),1)

# Reduce preferred settings to make RAM dongle firmware runable.
PCIE_NTXD		:= 64
PCIE_NRXD		:= 64
PCIE_D2H_NRXD	:= 32
PCIE_D2H_NTXD	:= 32
PCIE_H2D_NRXD	:= 32
PCIE_H2D_NTXD	:= 32
H2D_DMAQ_LEN	:= 32
D2H_DMAQ_LEN	:= 32
EXTRA_DFLAGS    += -DAMPDU_SCB_MAX_RELEASE_AQM=32

else #sdio

POOL_LEN	:= 32
WL_POST		:= 12
WL_NTXD		:= 128
EXTRA_DFLAGS    += -DAMPDU_SCB_MAX_RELEASE_AQM=8
EXTRA_DFLAGS	+= -DWLOVERTHRUSTER

endif

BUS_POST	:= 18
BUS_NRXD	:= 32
BUS_NTXD	:= 32
BUS_RXBND	:= 18

SDPCMD_RXBUFS	:= 40
SDPCMD_NRXD     := 128
SDPCMD_NTXD     := 128
SDPCMD_RXBND    := 16
SDPCMD_TXGLOM	:= 8

BUS_RXBUFS_MFGC := 18
BUS_NRXD_MFGC  := 32
BUS_NTXD_MFGC := 64

# CLM info
CLM_TYPE	:= 4345c0

CLM_BLOBS += 4345c0
CLM_BLOBS += 4345mfgtest
CLM_BLOBS += 4345mfgtestdfs

BCMXORCSUM := 1

# Reduce stack size to increase free heap
HND_STACK_SIZE  := 4608
EXTRA_DFLAGS    += -DHND_STACK_SIZE=$(HND_STACK_SIZE)
EXTRA_DFLAGS    += -DBCMPKTPOOL_ENABLED

# Add flops support
FLOPS_SUPPORT := 1

# Radio specific defines
ifeq ($(call opt,fcbga),1)
BCMRADIOREV     := 87
else	# wlbga
BCMRADIOREV     := 88
endif	# wlgba
BCMRADIOVER     := 0x1
BCMRADIOID      := 0x030b

EXTRA_DFLAGS    += -DBCMRADIOREV=$(BCMRADIOREV)
EXTRA_DFLAGS    += -DBCMRADIOVER=$(BCMRADIOVER)
EXTRA_DFLAGS    += -DBCMRADIOID=$(BCMRADIOID)

ifndef PA_TYPE
PA_TYPE	:= iPA
endif

ifeq ($(PA_TYPE),ePA)
EXTRA_DFLAGS    += -DWLPHY_EPA_ONLY
else
ifeq ($(PA_TYPE),iPA)
EXTRA_DFLAGS    += -DWLPHY_IPA_ONLY
endif
endif

# To limit PHY core checks
EXTRA_DFLAGS	+= -DBCMPHYCOREMASK=1

#Enable GPIO
EXTRA_DFLAGS	+= -DWLGPIOHLR

# Enable compiler option for inlining of simple functions for targets that run almost entirely from
# RAM. Inlining actually saves memory due to the elimination of function call overhead and more
# efficient register usage.
NOINLINE        :=

# Use the 2013.11 version of the toolchain. It provides significant memory savings
# relative to older toolchains.
TOOLSVER := 2013.11

# Enable compiler optimizations that might rename functions in order to save memory.
NOFNRENAME := 0

# For 4345, increase ATE sample capture buffer size to support 80MHz tests
EXTRA_DFLAGS	+= -DATE_SUPPORT80MHZ

EXTRA_DFLAGS	+= -DPOWPERCHANNL2G
EXTRA_DFLAGS	+= -DPOWPERCHANNL5G
EXTRA_DFLAGS	+= -DPOWPERBAND5G
