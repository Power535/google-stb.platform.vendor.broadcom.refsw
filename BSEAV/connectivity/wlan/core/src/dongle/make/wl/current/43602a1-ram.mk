# Makefile for hndrte based 43602a1 full ram Image
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

# chip specification
CHIP		:= 43602
REV		:= a1
REVID		:= 1
PCIEREVID	:= 9
CCREV		:= 48
PMUREV		:= 24
GCIREV		:= 3
AOBENAB		:= 0
OTPWRTYPE	:= 1
BUSCORETYPE	:= PCIE2_CORE_ID

# default targets
TARGETS		:= \
	pcie-ag-assert-err-splitrx \
	pcie-ag-mfgtest-seqcmds-splitrx \
	pcie-ag-p2p-mchan-splitrx-pktctx-proptxstatus-ampduhostreorder-logtrace-redux-swdiv-txpwrcap

# common target attributes
TARGET_ARCH	:= arm
TARGET_CPU	:= cr4
TARGET_HBUS	:= pcie
THUMB		:= 1
HBUS_PROTO	:= msgbuf
NODIS		:= 0

# wlconfig & wltunable for rest of the build targets
WLCONFFILE	:= wlconfig_rte_43602a1_bu
WLTUNEFILE	:= wltunable_rte_43602a1.h

# 0x0018_0000 is start of RAM
TEXT_START	:= 0x00180000

# REMAIN is exported from rte/wl/Makefile. this contains the string specifying the bus (pci)
ifeq ($(REMAIN),)
$(error $(REMAIN) is undefined)
endif

REMAIN := $(subst /,,$(REMAIN))

# 43602 has 512KB ATCM (instructions+data) + 448KB BxTCM (data) = 960KB = 983040 bytes
MEMBASE		:= 0x00180000
MEMSIZE		:= 983040
MFGTEST		:= 0
WLTINYDUMP	:= 0
# Enabling DBG_ASSERT would have an adverse effect on throughput.
DBG_ASSERT	:= 0
DBG_ASSERT_TRAP	:= 1
DBG_ERROR	:= 0
WLRXOV		:= 0

HNDLBUFCOMPACT	:= 1
BCMPKTIDMAP	:= 1
BCMFRAGPOOL	:= 1
BCMLFRAG	:= 1
BCMSPLITRX	:= 1

POOL_LEN_MAX    := 128
POOL_LEN        := 6
WL_POST_FIFO1   := 2
MFGTESTPOOL_LEN := 10
# Reduce packet pool lens for internal assert
# builds to fix out of memory issues
FRAG_POOL_LEN	:= 128
RXFRAG_POOL_LEN	:= 128

# To make NcSim possible
#INTERNAL	:= 1

# Reduce preferred settings to make RAM dongle firmware runable.
PCIE_NTXD		:= 128
PCIE_NRXD		:= 128
PCIE_D2H_NRXD	:= 64
PCIE_D2H_NTXD	:= 64
PCIE_H2D_NRXD	:= 64
PCIE_H2D_NTXD	:= 64
H2D_DMAQ_LEN	:= 64
D2H_DMAQ_LEN	:= 64

ifeq ($(call opt,mfgtest),1)
	BCMNVRAMR	:= 0
	BCMNVRAMW	:= 1
endif

ifeq ($(call opt,fdap),1)
	#router image, enable router specific features
	CLM_TYPE			:= 43602a1_access

	AP				:= 1
	SUFFIX_ENAB			:= 1
	INITDATA_EMBED			:= 1
	WLC_DISABLE_DFS_RADAR_SUPPORT	:= 0
	# Max MBSS virtual slave devices
	MBSS_MAXSLAVES			:= 4
	# Max Tx Flowrings
	PCIE_TXFLOWS			:= 132

	WLBSSLOAD			:= 1
	WLOSEN				:= 1
	WLPROBRESP_MAC_FILTER		:= 1

	EXTRA_DFLAGS	+= -DPKTC_FDAP

	#Turn on 16bit indices by default
	BCMDMAINDEX16 := 1
else
	# CLM info
	CLM_TYPE			:= 43602a1
	STA_KEEP_ALIVE  := 1
	#Turn on 32bit indices by default
	BCMDMAINDEX32 := 1
endif

CLM_BLOBS += 43602a1
CLM_BLOBS += 43602a1_access
CLM_BLOBS += 43602_sig

BIN_TRX_OPTIONS_SUFFIX := -x 0x0 -x 0x0 -x 0x0

# Reduce stack size to increase free heap
HND_STACK_SIZE	:= 4608
EXTRA_DFLAGS	+= -DHND_STACK_SIZE=$(HND_STACK_SIZE)
EXTRA_DFLAGS	+= -DBCMPKTPOOL_ENABLED

# Add flops support
FLOPS_SUPPORT	:= 1

EXTRA_DFLAGS    += -DBCMRADIOREV=$(BCMRADIOREV)
EXTRA_DFLAGS    += -DBCMRADIOVER=$(BCMRADIOVER)
EXTRA_DFLAGS    += -DBCMRADIOID=$(BCMRADIOID)
# Only support EPA
EXTRA_DFLAGS	+= -DWLPHY_EPA_ONLY -DEPA_SUPPORT=1
# Don't support PAPD
EXTRA_DFLAGS	+= -DWLC_DISABLE_PAPD_SUPPORT -DPAPD_SUPPORT=0

EXTRA_DFLAGS	+= -DAMPDU_SCB_MAX_RELEASE_AQM=32
PKTC_DONGLE	:= 1
EXTRA_DFLAGS	+= -DPCIEDEV_USE_EXT_BUF_FOR_IOCTL
PCIEDEV_MAX_IOCTLRSP_BUF_SIZE := 8192

ifeq ($(call opt,redux),1)
	CLM_TYPE := 43602a1_min
	EXTRA_DFLAGS += -D__FUNCTION__=\"FUNCNAME\"
	BCMSROMREV      := 11
	EXTRA_DFLAGS    += -DBCMSROMREV=$(BCMSROMREV)
	POOL_LEN        := 3
	FRAG_POOL_LEN	:= 64
	RXFRAG_POOL_LEN	:= 64
endif

ifeq ($(call opt,err),1)
	EXTRA_DFLAGS += -D__FUNCTION__=\"FUNCNAME\"
endif

#Support for SROM format
EXTRA_DFLAGS	+= -DBCMPCIEDEV_SROM_FORMAT

# ARM (CR4) clock switching, firmware switches between performance and power save mode
CPU_CLK_SWITCHING := 1

# Enable compiler option for inlining of simple functions for targets that run almost entirely from
# RAM. Inlining actually saves memory due to the elimination of function call overhead and more
# efficient register usage.
NOINLINE        :=

# Use the 2013.11 version of the toolchain. It provides significant memory savings
# relative to older toolchains.
TOOLSVER        = 2013.11

# Enable compiler optimizations that might rename functions in order to save memory.
NOFNRENAME      := 0

# Support for sliding window within flowrings
# This allows an option to set a large flowring size, but operate in a sliding
# window model where dongle only consumes packets upto the window size.
#EXTRA_DFLAGS    += -DFLOWRING_SLIDING_WINDOW -DFLOWRING_SLIDING_WINDOW_SIZE=512
# Support for using smaller bitsets on each flowring - instead of the full flowring depth
#EXTRA_DFLAGS    += -DFLOWRING_USE_SHORT_BITSETS

# SW diversity GPIO control
# swdiv control gpio pins can be assigned as followings:
# gpioctrl reg in chipcommon will set by [CTRLBITS] << [OFFSET]
# uCode controls psm_gpio_oe based on this bit setting.
# limitation is that the pin assign have to be sequencial.

# board specific gpio active pins as a default
EXTRA_DFLAGS	+= -DWLC_SWDIV_GPIO_OFFSET=6
# board specific gpio active pins as a default
EXTRA_DFLAGS	+= -DWLC_SWDIV_GPIO_CTRLBITS=7
