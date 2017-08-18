/***************************************************************************
 * Copyright (C) 2016-2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *
 * Module Description:
 *
 ***************************************************************************/


/* this file should not be directly included, and it could be included multiple times  */
/* if new entry have to be added please add it into the  bchp_memc_clients_chip_map_all.h and regenerate file */
/* if for whatever reason file has to be manually edited please remove line below */
/* ************** THIS FILE IS AUTOGENERATED. DO NOT EDIT **************************/
/*****
GENERATED by:
perl magnum/basemodules/chp/src/common/bchp_memc_clients_chip_map.pl magnum/basemodules/chp/src/common/bchp_memc_clients_chip_map_all.h magnum/basemodules/chp/include/7260/common/memc/bchp_memc_clients_chip_A0.h magnum/basemodules/chp/include/7260/common/memc/bchp_memc_clients_chip_map_A0.h
*******/


BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_RS,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_XC,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_CDB,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_ITB_MSG,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_RS,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_XC_RMX_MSG,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_XC_RAVE,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_PB,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_MEMDMA,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(GENET0_WR,ETHERNET,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(GENET0_RD,ETHERNET,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SATA,SATA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BSP,BSP,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SCPU,SCPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(FLASH_DMA,FLASH,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SDIO_EMMC,SDIO,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SDIO_CARD,SDIO,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MCIF_RD,MCARD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MCIF_WR,MCARD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(UART_DMA_RD,UART,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(UART_DMA_WR,UART,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_HI_0,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_LO_0,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_X_BDC_0,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(RAAGA,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(RAAGA_1,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(AUD_AIO,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VEC_VIP0,VEC,VIP_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VEC_VIP1,VEC,VIP_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_DBLK,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_DBLK_1,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_ILCPU,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_OLCPU,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_CAB,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_ILSI,AVD,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SID,SID,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_PIX_FD,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_QUANT,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_PIX_CAP,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MFD0,BVN,MFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MFD0_1,BVN,MFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MFD1,BVN,MFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MFD1_1,BVN,MFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_VFD0,BVN,VFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_VFD1,BVN,VFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_CAP0,BVN,CAP_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_CAP1,BVN,CAP_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_GFD0,BVN,GFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_GFD1,BVN,GFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_RDC,BVN,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VEC_VBI_ENC0,VEC_VBI,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(M2MC_0,M2MC,GFX_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(M2MC_1,M2MC,GFX_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(M2MC_2,M2MC,GFX_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(PCIE_0,PCI,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HOST_CPU_MCP_R_HI,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HOST_CPU_MCP_R_LO,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HOST_CPU_MCP_W_HI,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HOST_CPU_MCP_W_LO,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(V3D_MCP_R_HI,3D,V3D_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(V3D_MCP_R_LO,3D,V3D_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(V3D_MCP_W_HI,3D,V3D_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(V3D_MCP_W_LO,3D,V3D_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(UBUS_RD,UBUS,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(UBUS_WR,UBUS,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(HVD0_MOCOMP,PREFETCH,HVD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(TRACELOG,TRACE,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MEMC_RESERVED_0,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(ZCQS_ENGINE,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MSA,MSA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(DIS0,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(DIS1,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(DRAM_INIT_ZQCS,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(REFRESH,MEMC,NOT_MAP)
