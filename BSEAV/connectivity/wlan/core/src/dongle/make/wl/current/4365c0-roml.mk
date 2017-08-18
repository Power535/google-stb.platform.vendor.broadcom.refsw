#
# Makefile for hndrte based 4365c0 ROM Offload image building
#
# Broadcom Proprietary and Confidential. Copyright (C) 2017,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
# $Id: 4365c0-roml.mk$
#

####################################################################################################
# This makefile is used when building a ROM offload image, so an image that runs in RAM and calls
# routines that reside in ROM. It is not used when building a ROM. Default settings are defined in
# the 'wlconfig', settings in there may be redefined in this file when a 'default' ROM offload image
# should support more or less features than the ROM.
####################################################################################################

# chip specification
CHIP		:= 4365
ROMREV		:= c0
REV		:= c0
REVID		:= 4
# default targets
TARGETS		:= \
	pcie-ag-err-assert-splitrx \
	pcie-ag-mfgtest-seqcmds-splitrx-phydbg \
	pcie-ag-mfgtest-seqcmds-splitrx-phydbg-err-assert \
	pcie-ag-p2p-mchan-idauth-idsup-pno-aoe-pktfilter-pf2-keepalive-splitrx \
	pcie-ag-p2p-mchan-idauth-idsup-pno-aoe-pktfilter-pf2-keepalive-splitrx-err-assert \
	pcie-ag-p2p-mchan-idauth-idsup-pno-aoe-ndoe-pktfilter-pf2-keepalive-splitrx-toe-ccx-mfp-anqpo-p2po-wl11k-wl11u-wnm-relmcast-txbf-fbt-tdls-sr-pktctx-amsdutx-proxd \
	pcie-ag-p2p-mchan-idauth-idsup-pno-aoe-ndoe-pktfilter-pf2-keepalive-splitrx-toe-ccx-mfp-anqpo-p2po-wl11k-wl11u-wnm-relmcast-txbf-fbt-tdls-sr-pktctx-amsdutx-proxd-err-assert \
	pcie-ag-splitrx-fdap-mbss-mfp-wl11k-wl11u-wnm-txbf-pktctx-amsdutx-ampduretry-proptxstatus \
	pcie-ag-splitrx-fdap-mbss-mfp-wl11k-wl11u-wnm-txbf-pktctx-amsdutx-ampduretry-proptxstatus-err-assert \
	pcie-ag-splitrx-fdap-mbss-mfp-wl11k-wl11u-txbf-pktctx-amsdutx-ampduretry-chkd2hdma-proptxstatus-txpwr-11nprop-obss-dbwsw-err-assert-dbgam-dbgams-ringer-dmaindex16-bgdfs-msgbuf-ndis-vista-dhdoid-pktfilter-mfp-keepalive

TEXT_START	:= 0x200000
DATA_START	:= 0x200100

# 800KB ROM
ROM_LOW		?= 0x00000000
ROM_HIGH	?= 0x000C8000

# common target attributes
TARGET_ARCH	:= arm
TARGET_CPU	:= ca7
TARGET_HBUS	:= pcie
THUMB		:= 1
HBUS_PROTO	:= msgbuf


# wlconfig & wltunable for rest of the build targets
WLCONFFILE	:= wlconfig_rte_4365c0_dev
WLTUNEFILE	:= wltunable_rte_4365c0.h

# ROM image info
ROMOFFLOAD	:= 1
ROMLDIR		:= $(TOPDIR)/../../../../components/chips/images/roml/$(CHIP)$(ROMREV)
ROMLLIB		:= roml.exe

# Use TCAM to patch ROM functions
TCAM		:= 1
JMPTBL_TCAM	:= 1
GLOBALIZE	:= 1
TCAM_PCNT	:= 2
TCAM_SIZE	:= 512

# features (sync with romlsym/4365c0.mk)
MEMBASE     := $(TEXT_START)
MEMSIZE		:= 2228224
WLTINYDUMP	:= 0
DBG_ASSERT	:= 0
DBG_ASSERT_TRAP	:= 1
DBG_ERROR	:= 1
DBG_TEMPSENSE	:= 1
WLRXOV		:= 0
PROP_TXSTATUS	:= 1

# Memory reduction features:
# - HNDLBUFCOMPACT: Compacts head/end pointers in lbuf to single word
#   To disable set HNDLBUFCOMPACT = 0
# - BCMPKTIDMAP: Suppresses pkt pointers to Ids in lbuf<next,link>, pktpool, etc
#   Must specify max number of packets (various pools + heap)
# 4365c0 has 2M+128KB MEMSIZE, so need to disable HNDLBUFCOMPACT feature.
HNDLBUFCOMPACT	:= 0
BCMPKTIDMAP     := 1
BCMFRAGPOOL	:= 1
BCMRXFRAGPOOL	:= 1
BCMLFRAG	:= 1
BCMSPLITRX	:= 1

#Microsoft Extensible STA for Dongle
NDISFW=1
EXTSTA=1

DLL_USE_MACROS	:= 1
HNDLBUF_USE_MACROS := 1

POOL_LEN_MAX    := 400
POOL_LEN        := 32

WL_POST_FIFO1   := 2
MFGTESTPOOL_LEN := 10
FRAG_POOL_LEN	:= 400
RXFRAG_POOL_LEN	:= 320
PKT_MAXIMUM_ID  := 1024

H2D_DMAQ_LEN	:= 256
D2H_DMAQ_LEN	:= 256

PCIE_NTXD	:= 256
PCIE_NRXD	:= 256

WL_POST_CLASSIFIED_FIFO := 4
WL_SPLITRX_MODE := 2
WL_CLASSIFY_FIFO := 2
ifeq ($(WL_SPLITRX_MODE),2)
EXTRA_DFLAGS    += -DFORCE_RX_FIFO1
endif

ifeq ($(call opt,ate),1)
        TARGET_HBUS     := sdio
endif

ifeq ($(call opt,mfgtest),1)
	#allow MFG image to write OTP
	BCMNVRAMR	:= 0
	BCMNVRAMW	:= 1
endif

ifeq ($(call opt,fdap),1)
	#router image, enable router specific features
	CLM_TYPE			:= 4365a0_access

	AP				:= 1
	SUFFIX_ENAB			:= 1
	INITDATA_EMBED			:= 1
	WLC_DISABLE_DFS_RADAR_SUPPORT	:= 0
	# Max MBSS virtual slave devices
	MBSS_MAXSLAVES			:= 8
	# Max Tx Flowrings
	PCIE_TXFLOWS			:= 264

	# Reduce packet pool lens for internal assert
	# builds to fix out of memory issues
	ifeq ($(call opt,assert),1)
		FRAG_POOL_LEN		:= 160
		RXFRAG_POOL_LEN		:= 160
	endif
	WLBSSLOAD			:= 1
	WLOSEN				:= 1
	WLPROBRESP_MAC_FILTER		:= 1
	EXTRA_DFLAGS	+= -DPKTC_FDAP

else
	# CLM info
	CLM_TYPE	:= 4365a0

	NAR_MAX_TRANSIT_PACKETS         := 128
endif

# Reduce stack size to increase free heap
HNDRTE_STACK_SIZE	:= 4608
EXTRA_DFLAGS		+= -DHNDRTE_STACK_SIZE=$(HNDRTE_STACK_SIZE)

EXTRA_DFLAGS	+= -DBCMPKTPOOL_ENABLED

# Add flops support
FLOPS_SUPPORT	:= 1

#Enable GPIO
EXTRA_DFLAGS	+= -DWLGPIOHLR

TOOLSVER	:= 2013.11
NOFNRENAME	:= 1

# Hard code some PHY characteristics to reduce RAM code size
# RADIO
EXTRA_DFLAGS	+= -DBCMRADIOREV=$(BCMRADIOREV)
EXTRA_DFLAGS	+= -DBCMRADIOVER=$(BCMRADIOVER)
EXTRA_DFLAGS	+= -DBCMRADIOID=$(BCMRADIOID)

# Only support EPA
EXTRA_DFLAGS	+= -DWLPHY_EPA_ONLY -DEPA_SUPPORT=1

# Don't support PAPD
EXTRA_DFLAGS	+= -DEPAPD_SUPPORT=0 -DWLC_DISABLE_PAPD_SUPPORT -DPAPD_SUPPORT=0

EXTRA_DFLAGS	+= -DAMPDU_SCB_MAX_RELEASE_AQM=32
EXTRA_DFLAGS	+= -DPKTC_DONGLE
EXTRA_DFLAGS	+= -DPCIEDEV_USE_EXT_BUF_FOR_IOCTL
PCIEDEV_MAX_IOCTLRSP_BUF_SIZE := 8192

# Ideal: (MAX_HOST_RXBUFS > (RXFRAG_POOL_LEN + POOL_LEN)); At least (MAX_HOST_RXBUFS > (WL_POST + WL_POST_FIFO1)) for pciedev_fillup_rxcplid callback from pktpool_get
# Also increase H2DRING_RXPOST_MAX_ITEM to match WL_POST
EXTRA_DFLAGS	+= -DMAX_HOST_RXBUFS=512

#wowl gpio pin 14, Polarity at logic low is 1
WOWL_GPIOPIN	:= 0xe
WOWL_GPIO_POLARITY := 0x1
EXTRA_DFLAGS    += -DWOWL_GPIO=$(WOWL_GPIOPIN) -DWOWL_GPIO_POLARITY=$(WOWL_GPIO_POLARITY)

# max fetch count at once
EXTRA_DFLAGS    += -DPCIEDEV_MAX_PACKETFETCH_COUNT=128
EXTRA_DFLAGS	+= -DPCIEDEV_MAX_LOCALBUF_PKT_COUNT=512
EXTRA_DFLAGS    += -DPD_NBUF_D2H_TXCPL=32
EXTRA_DFLAGS    += -DPD_NBUF_D2H_RXCPL=32

# PD_NBUF_H2D_RXPOST * items(32) > MAX_HOST_RXBUFS for pciedev_fillup_haddr=>pciedev_get_host_addr callback from pktpool_get
EXTRA_DFLAGS    += -DPD_NBUF_H2D_RXPOST=16
EXTRA_DFLAGS    += -DMAX_TX_STATUS_QUEUE=256
EXTRA_DFLAGS    += -DMAX_TX_STATUS_COMBINED=128

# Size of local queue to store completions
EXTRA_DFLAGS    += -DPCIEDEV_CNTRL_CMPLT_Q_SIZE=16

# RxOffsets for the PCIE mem2mem DMA
EXTRA_DFLAGS    += -DH2D_PD_RX_OFFSET=0
EXTRA_DFLAGS    += -DD2H_PD_RX_OFFSET=0

# Set deadman_to nvram to 160000 to enable a 5 seconds dead man timer.
# 160000 = 5 * 32 * 1000 (CA7 Counter Frequency is 32KHz)

# Dual Pkt AMSDU optimization
EXTRA_DFLAGS    += -DDUALPKTAMSDU

#Support for SROM format
EXTRA_DFLAGS	+= -DBCMPCIEDEV_SROM_FORMAT

#Support for STBC
EXTRA_DFLAGS	+= -DWL11N_STBC_RX_ENABLED

# Support for sliding window within flowrings
# This allows an option to set a large flowring size, but operate in a sliding
# window model where dongle only consumes packets upto the window size.
EXTRA_DFLAGS    += -DFLOWRING_SLIDING_WINDOW -DFLOWRING_SLIDING_WINDOW_SIZE=512
# Support for using smaller bitsets on each flowring - instead of the full flowring depth
EXTRA_DFLAGS    += -DFLOWRING_USE_SHORT_BITSETS
