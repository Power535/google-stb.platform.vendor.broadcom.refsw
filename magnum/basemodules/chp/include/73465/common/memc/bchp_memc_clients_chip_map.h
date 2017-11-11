/***************************************************************************
 * Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
perl magnum/basemodules/chp/src/common/bchp_memc_clients_chip_map.pl magnum/basemodules/chp/src/common/bchp_memc_clients_chip_map_all.h magnum/basemodules/chp/include/73465/common/memc/bchp_memc_clients_chip.h magnum/basemodules/chp/include/73465/common/memc/bchp_memc_clients_chip_map.h
*******/


#if defined(BCHP_P_MEMC_DEFINE_CLIENT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_RS,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_XC,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_CDB,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_WR_ITB_MSG,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_RS,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_XC_RMX_MSG,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_XC_RAVE,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(XPT_RD_PB,XPT,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_DBLK,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_ILCPU,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_OLCPU,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_CAB,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_ILSI,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_BLCPU,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_BLSI,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_MVSCL,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_SPIXSTR,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BSP,BSP,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_DBLK_1,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SATA,SATA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(TPCAP,TPCAP,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(CPU,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SDIO_CARD,SDIO,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(GENET_WR,ETHERNET,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(GENET_RD,ETHERNET,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MOCA_DATA_WR,MOCA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MOCA_DATA_RD,MOCA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MOCA_MIPS,MOCA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_HI_0,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_LO_0,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_HI_1,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(USB_LO_1,USB,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(RAAGA,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(AUD_AIO,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(RAAGA_1,AUD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_RDC,BVN,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_MFD1_0,BVN,MFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_MFD1_1,BVN,MFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_MFD0_0,BVN,MFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_MFD0_1,BVN,MFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_CAP3,BVN,CAP_3)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_CAP2,BVN,CAP_2)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_CAP1,BVN,CAP_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_CAP0,BVN,CAP_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_VFD3,BVN,VFD_3)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_VFD2,BVN,VFD_2)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_VFD1,BVN,VFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_VFD0,BVN,VFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_PIX_CAP,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_PIX_FD,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVN_MAD_QUANT,BVN,MAD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_GFD1,BVN,GFD_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(BVNF_GFD0,BVN,GFD_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VEC_VBI_ENC1,VEC_VBI,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VEC_VBI_ENC0,VEC_VBI,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(M2M_DMA0,M2M,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VC4_3D_0,3D,V3D_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(VC4_3D_1,3D,V3D_1)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(M2MC,M2MC,GFX_0)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SID,SID,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_SPIXSCALE,AVD,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(MSA,MSA,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(DIS,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(SVD_MOCOMP,PREFETCH,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(CPU_LMB_HIGH,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(CPU_LMB_LOW,CPU,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(DRAM_INIT,MEMC,NOT_MAP)
BCHP_P_MEMC_DEFINE_CLIENT_MAP(REF,MEMC,NOT_MAP)

#else /* #if defined(BCHP_P_MEMC_DEFINE_CLIENT_MAP) */

#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_WR_RS 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_WR_XC 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_WR_CDB 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_WR_ITB_MSG 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_RD_RS 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_RD_XC_RMX_MSG 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_RD_XC_RAVE 1
#define BCHP_P_MEMC_CLIENT_EXISTS_XPT_RD_PB 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_DBLK 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_ILCPU 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_OLCPU 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_CAB 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_ILSI 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_BLCPU 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_BLSI 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_MVSCL 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_SPIXSTR 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BSP 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_DBLK_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SATA 1
#define BCHP_P_MEMC_CLIENT_EXISTS_TPCAP 1
#define BCHP_P_MEMC_CLIENT_EXISTS_CPU 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SDIO_CARD 1
#define BCHP_P_MEMC_CLIENT_EXISTS_GENET_WR 1
#define BCHP_P_MEMC_CLIENT_EXISTS_GENET_RD 1
#define BCHP_P_MEMC_CLIENT_EXISTS_MOCA_DATA_WR 1
#define BCHP_P_MEMC_CLIENT_EXISTS_MOCA_DATA_RD 1
#define BCHP_P_MEMC_CLIENT_EXISTS_MOCA_MIPS 1
#define BCHP_P_MEMC_CLIENT_EXISTS_USB_HI_0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_USB_LO_0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_USB_HI_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_USB_LO_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_RAAGA 1
#define BCHP_P_MEMC_CLIENT_EXISTS_AUD_AIO 1
#define BCHP_P_MEMC_CLIENT_EXISTS_RAAGA_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_RDC 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_MFD1_0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_MFD1_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_MFD0_0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_MFD0_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_CAP3 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_CAP2 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_CAP1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_CAP0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_VFD3 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_VFD2 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_VFD1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_VFD0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVN_MAD_PIX_CAP 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVN_MAD_PIX_FD 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVN_MAD_QUANT 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_GFD1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_BVNF_GFD0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_VEC_VBI_ENC1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_VEC_VBI_ENC0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_M2M_DMA0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_VC4_3D_0 1
#define BCHP_P_MEMC_CLIENT_EXISTS_VC4_3D_1 1
#define BCHP_P_MEMC_CLIENT_EXISTS_M2MC 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SID 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_SPIXSCALE 1
#define BCHP_P_MEMC_CLIENT_EXISTS_MSA 1
#define BCHP_P_MEMC_CLIENT_EXISTS_DIS 1
#define BCHP_P_MEMC_CLIENT_EXISTS_SVD_MOCOMP 1
#define BCHP_P_MEMC_CLIENT_EXISTS_CPU_LMB_HIGH 1
#define BCHP_P_MEMC_CLIENT_EXISTS_CPU_LMB_LOW 1
#define BCHP_P_MEMC_CLIENT_EXISTS_DRAM_INIT 1
#define BCHP_P_MEMC_CLIENT_EXISTS_REF 1
#endif /* #else #if defined(BCHP_P_MEMC_DEFINE_CLIENT_MAP) */
