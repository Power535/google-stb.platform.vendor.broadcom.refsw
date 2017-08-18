/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Fri May 26 15:56:44 2017
 *                 Full Compile MD5 Checksum  f5e63307e5dcf1369c866f7e71f68daa
 *                     (minus title and desc)
 *                 MD5 Checksum               c4c446cddb09eae8aa92810408526a00
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1481
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   LOCAL home/pntruong/sbin/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_WEBHIF_L1_MASK_H__
#define BCHP_WEBHIF_L1_MASK_H__

/***************************************************************************
 *WEBHIF_L1_MASK - WEBHIF L1 INTR Mask Registers
 ***************************************************************************/
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK         0x20201c00 /* [RW][32] WebHIF Interrupt Mask Register 0 */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK         0x20201c04 /* [RW][32] WebHIF Interrupt Mask Register 1 */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK         0x20201c08 /* [RW][32] WebHIF Interrupt Mask Register 2 */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK         0x20201c0c /* [RW][32] WebHIF Interrupt Mask Register 3 */

/***************************************************************************
 *INTR_W0_MASK - WebHIF Interrupt Mask Register 0
 ***************************************************************************/
/* WEBHIF_L1_MASK :: INTR_W0_MASK :: IPI0_CPU_INTR [31:31] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_IPI0_CPU_INTR_MASK        0x80000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_IPI0_CPU_INTR_SHIFT       31
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_IPI0_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: HIF_SPI_CPU_INTR [30:30] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_SPI_CPU_INTR_MASK     0x40000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_SPI_CPU_INTR_SHIFT    30
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_SPI_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: HIF_CPU_INTR [29:29] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_CPU_INTR_MASK         0x20000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_CPU_INTR_SHIFT        29
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HIF_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: XPT_SEC_1_CPU_INTR [28:28] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_1_CPU_INTR_MASK   0x10000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_1_CPU_INTR_SHIFT  28
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: XPT_SEC_0_CPU_INTR [27:27] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_0_CPU_INTR_MASK   0x08000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_0_CPU_INTR_SHIFT  27
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_XPT_SEC_0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: HDMI_TX_CPU_INTR [26:26] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HDMI_TX_CPU_INTR_MASK     0x04000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HDMI_TX_CPU_INTR_SHIFT    26
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_HDMI_TX_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: AVS_CPU_INTR [25:25] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AVS_CPU_INTR_MASK         0x02000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AVS_CPU_INTR_SHIFT        25
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AVS_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CLKGEN_CPU_INTR [24:24] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CLKGEN_CPU_INTR_MASK      0x01000000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CLKGEN_CPU_INTR_SHIFT     24
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CLKGEN_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNB_1_CPU_INTR [23:23] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_1_CPU_INTR_MASK      0x00800000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_1_CPU_INTR_SHIFT     23
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_1_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNM_0_CPU_INTR [22:22] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNM_0_CPU_INTR_MASK      0x00400000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNM_0_CPU_INTR_SHIFT     22
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNM_0_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNF_16_CPU_INTR [21:21] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_16_CPU_INTR_MASK     0x00200000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_16_CPU_INTR_SHIFT    21
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_16_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNF_9_CPU_INTR [20:20] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_9_CPU_INTR_MASK      0x00100000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_9_CPU_INTR_SHIFT     20
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_9_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNF_5_CPU_INTR [19:19] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_5_CPU_INTR_MASK      0x00080000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_5_CPU_INTR_SHIFT     19
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_5_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNF_1_CPU_INTR [18:18] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_1_CPU_INTR_MASK      0x00040000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_1_CPU_INTR_SHIFT     18
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_1_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNF_0_CPU_INTR [17:17] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_0_CPU_INTR_MASK      0x00020000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_0_CPU_INTR_SHIFT     17
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNF_0_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BVNB_0_CPU_INTR [16:16] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_0_CPU_INTR_MASK      0x00010000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_0_CPU_INTR_SHIFT     16
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BVNB_0_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: VEC_CPU_INTR [15:15] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_VEC_CPU_INTR_MASK         0x00008000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_VEC_CPU_INTR_SHIFT        15
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_VEC_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: GFX_CPU_INTR [14:14] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_GFX_CPU_INTR_MASK         0x00004000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_GFX_CPU_INTR_SHIFT        14
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_GFX_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: AIO_CPU_INTR [13:13] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AIO_CPU_INTR_MASK         0x00002000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AIO_CPU_INTR_SHIFT        13
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_AIO_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: NMI_PIN_CPU_INTR [12:12] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_NMI_PIN_CPU_INTR_MASK     0x00001000
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_NMI_PIN_CPU_INTR_SHIFT    12
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_NMI_PIN_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_THERM_LO_CPU_INTR [11:11] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_LO_CPU_INTR_MASK 0x00000800
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_LO_CPU_INTR_SHIFT 11
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_LO_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_THERM_HI_CPU_INTR [10:10] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_HI_CPU_INTR_MASK 0x00000400
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_HI_CPU_INTR_SHIFT 10
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_THERM_HI_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_AXIERRIRQ_CPU_INTR [09:09] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_AXIERRIRQ_CPU_INTR_MASK 0x00000200
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_AXIERRIRQ_CPU_INTR_SHIFT 9
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_AXIERRIRQ_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_INTERRIRQ_CPU_INTR [08:08] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_INTERRIRQ_CPU_INTR_MASK 0x00000100
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_INTERRIRQ_CPU_INTR_SHIFT 8
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_INTERRIRQ_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_PMUIRQ_3_CPU_INTR [07:07] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_3_CPU_INTR_MASK 0x00000080
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_3_CPU_INTR_SHIFT 7
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_3_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_PMUIRQ_2_CPU_INTR [06:06] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_2_CPU_INTR_MASK 0x00000040
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_2_CPU_INTR_SHIFT 6
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_2_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_PMUIRQ_1_CPU_INTR [05:05] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_1_CPU_INTR_MASK 0x00000020
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_1_CPU_INTR_SHIFT 5
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_PMUIRQ_0_CPU_INTR [04:04] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_0_CPU_INTR_MASK 0x00000010
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_0_CPU_INTR_SHIFT 4
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_PMUIRQ_0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_ACCESS_VIOL1_CPU_INTR [03:03] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL1_CPU_INTR_MASK 0x00000008
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL1_CPU_INTR_SHIFT 3
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: CPU_ACCESS_VIOL0_CPU_INTR [02:02] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL0_CPU_INTR_MASK 0x00000004
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL0_CPU_INTR_SHIFT 2
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_CPU_ACCESS_VIOL0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: SCPU_CPU_INTR [01:01] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_SCPU_CPU_INTR_MASK        0x00000002
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_SCPU_CPU_INTR_SHIFT       1
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_SCPU_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W0_MASK :: BSP_CPU_INTR [00:00] */
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BSP_CPU_INTR_MASK         0x00000001
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BSP_CPU_INTR_SHIFT        0
#define BCHP_WEBHIF_L1_MASK_INTR_W0_MASK_BSP_CPU_INTR_DEFAULT      0x00000001

/***************************************************************************
 *INTR_W1_MASK - WebHIF Interrupt Mask Register 1
 ***************************************************************************/
/* WEBHIF_L1_MASK :: INTR_W1_MASK :: UPG_MAIN_CPU_INTR [31:31] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_MAIN_CPU_INTR_MASK    0x80000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_MAIN_CPU_INTR_SHIFT   31
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_MAIN_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: UPG_BSC_AON_CPU_INTR [30:30] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_AON_CPU_INTR_MASK 0x40000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_AON_CPU_INTR_SHIFT 30
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_AON_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: UPG_BSC_CPU_INTR [29:29] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_CPU_INTR_MASK     0x20000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_CPU_INTR_SHIFT    29
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_BSC_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: UPG_AUX_AON_CPU_INTR [28:28] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_AUX_AON_CPU_INTR_MASK 0x10000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_AUX_AON_CPU_INTR_SHIFT 28
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_UPG_AUX_AON_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SYS_PM_CPU_INTR [27:27] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_PM_CPU_INTR_MASK      0x08000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_PM_CPU_INTR_SHIFT     27
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_PM_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SYS_AON_CPU_INTR [26:26] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_AON_CPU_INTR_MASK     0x04000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_AON_CPU_INTR_SHIFT    26
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_AON_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SYS_CPU_INTR [25:25] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_CPU_INTR_MASK         0x02000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_CPU_INTR_SHIFT        25
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SYS_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SATA_0_GRB_CPU_INTR [24:24] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_GRB_CPU_INTR_MASK  0x01000000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_GRB_CPU_INTR_SHIFT 24
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_GRB_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SATA_0_AHCI_CPU_INTR [23:23] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_AHCI_CPU_INTR_MASK 0x00800000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_AHCI_CPU_INTR_SHIFT 23
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SATA_0_AHCI_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: MEMC0_CPU_INTR [22:22] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_MEMC0_CPU_INTR_MASK       0x00400000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_MEMC0_CPU_INTR_SHIFT      22
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_MEMC0_CPU_INTR_DEFAULT    0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: RAAGA_FW_CPU_INTR [21:21] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_FW_CPU_INTR_MASK    0x00200000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_FW_CPU_INTR_SHIFT   21
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_FW_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: RAAGA_CPU_INTR [20:20] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_CPU_INTR_MASK       0x00100000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_CPU_INTR_SHIFT      20
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_RAAGA_CPU_INTR_DEFAULT    0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: HVD0_0_CPU_INTR [19:19] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_HVD0_0_CPU_INTR_MASK      0x00080000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_HVD0_0_CPU_INTR_SHIFT     19
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_HVD0_0_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_NMI_CPU_INTR [18:18] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_NMI_CPU_INTR_MASK  0x00040000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_NMI_CPU_INTR_SHIFT 18
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_NMI_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_MSI_INTR_CPU_INTR [17:17] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_MSI_INTR_CPU_INTR_MASK 0x00020000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_MSI_INTR_CPU_INTR_SHIFT 17
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_MSI_INTR_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_INTR_CPU_INTR [16:16] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTR_CPU_INTR_MASK 0x00010000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTR_CPU_INTR_SHIFT 16
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTR_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_INTD_CPU_INTR [15:15] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTD_CPU_INTR_MASK 0x00008000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTD_CPU_INTR_SHIFT 15
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTD_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_INTC_CPU_INTR [14:14] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTC_CPU_INTR_MASK 0x00004000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTC_CPU_INTR_SHIFT 14
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTC_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_INTB_CPU_INTR [13:13] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTB_CPU_INTR_MASK 0x00002000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTB_CPU_INTR_SHIFT 13
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTB_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_INTA_CPU_INTR [12:12] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTA_CPU_INTR_MASK 0x00001000
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTA_CPU_INTR_SHIFT 12
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_INTA_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: PCIE_0_ERR_ATTN_CPU_INTR [11:11] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_ERR_ATTN_CPU_INTR_MASK 0x00000800
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_ERR_ATTN_CPU_INTR_SHIFT 11
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_PCIE_0_ERR_ATTN_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_TIMER_CPU_INTR [10:10] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_TIMER_CPU_INTR_MASK 0x00000400
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_TIMER_CPU_INTR_SHIFT 10
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_TIMER_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_STB_IPI1_CPU_INTR [09:09] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI1_CPU_INTR_MASK 0x00000200
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI1_CPU_INTR_SHIFT 9
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_STB_IPI0_CPU_INTR [08:08] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI0_CPU_INTR_MASK 0x00000100
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI0_CPU_INTR_SHIFT 8
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_STB_IPI0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_IPI1_CPU_INTR [07:07] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI1_CPU_INTR_MASK 0x00000080
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI1_CPU_INTR_SHIFT 7
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_IPI0_CPU_INTR [06:06] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI0_CPU_INTR_MASK 0x00000040
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI0_CPU_INTR_SHIFT 6
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_IPI0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: WEBHIF_CPU_INTR [05:05] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_CPU_INTR_MASK      0x00000020
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_CPU_INTR_SHIFT     5
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_WEBHIF_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SDIO1_0_CPU_INTR [04:04] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO1_0_CPU_INTR_MASK     0x00000010
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO1_0_CPU_INTR_SHIFT    4
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO1_0_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: SDIO0_0_CPU_INTR [03:03] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO0_0_CPU_INTR_MASK     0x00000008
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO0_0_CPU_INTR_SHIFT    3
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_SDIO0_0_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: IPI3_CPU_INTR [02:02] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI3_CPU_INTR_MASK        0x00000004
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI3_CPU_INTR_SHIFT       2
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI3_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: IPI2_CPU_INTR [01:01] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI2_CPU_INTR_MASK        0x00000002
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI2_CPU_INTR_SHIFT       1
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI2_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W1_MASK :: IPI1_CPU_INTR [00:00] */
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI1_CPU_INTR_MASK        0x00000001
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI1_CPU_INTR_SHIFT       0
#define BCHP_WEBHIF_L1_MASK_INTR_W1_MASK_IPI1_CPU_INTR_DEFAULT     0x00000001

/***************************************************************************
 *INTR_W2_MASK - WebHIF Interrupt Mask Register 2
 ***************************************************************************/
/* WEBHIF_L1_MASK :: INTR_W2_MASK :: GENET_0_B_CPU_INTR [31:31] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_B_CPU_INTR_MASK   0x80000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_B_CPU_INTR_SHIFT  31
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_B_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: GENET_0_A_CPU_INTR [30:30] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_A_CPU_INTR_MASK   0x40000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_A_CPU_INTR_SHIFT  30
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_GENET_0_A_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_USBD_CPU_INTR [29:29] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_USBD_CPU_INTR_MASK   0x20000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_USBD_CPU_INTR_SHIFT  29
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_USBD_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_OHCI_1_CPU_INTR [28:28] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_1_CPU_INTR_MASK 0x10000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_1_CPU_INTR_SHIFT 28
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_EHCI_1_CPU_INTR [27:27] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_1_CPU_INTR_MASK 0x08000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_1_CPU_INTR_SHIFT 27
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_BRIDGE_CPU_INTR [26:26] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_BRIDGE_CPU_INTR_MASK 0x04000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_BRIDGE_CPU_INTR_SHIFT 26
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_BRIDGE_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_OHCI_0_CPU_INTR [25:25] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_0_CPU_INTR_MASK 0x02000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_0_CPU_INTR_SHIFT 25
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_OHCI_0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: USB0_EHCI_0_CPU_INTR [24:24] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_0_CPU_INTR_MASK 0x01000000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_0_CPU_INTR_SHIFT 24
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_USB0_EHCI_0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_EXTCARD_CPU_INTR [23:23] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_EXTCARD_CPU_INTR_MASK 0x00800000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_EXTCARD_CPU_INTR_SHIFT 23
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_EXTCARD_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_WMDMA_CPU_INTR [22:22] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_WMDMA_CPU_INTR_MASK   0x00400000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_WMDMA_CPU_INTR_SHIFT  22
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_WMDMA_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_MDMA_MCPB_CPU_INTR [21:21] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MDMA_MCPB_CPU_INTR_MASK 0x00200000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MDMA_MCPB_CPU_INTR_SHIFT 21
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MDMA_MCPB_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_MCPB_CPU_INTR [20:20] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MCPB_CPU_INTR_MASK    0x00100000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MCPB_CPU_INTR_SHIFT   20
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MCPB_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_STATUS_CPU_INTR [19:19] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_STATUS_CPU_INTR_MASK  0x00080000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_STATUS_CPU_INTR_SHIFT 19
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_STATUS_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_RAV_CPU_INTR [18:18] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_RAV_CPU_INTR_MASK     0x00040000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_RAV_CPU_INTR_SHIFT    18
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_RAV_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_PCR_CPU_INTR [17:17] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_PCR_CPU_INTR_MASK     0x00020000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_PCR_CPU_INTR_SHIFT    17
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_PCR_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_MSG_CPU_INTR [16:16] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_CPU_INTR_MASK     0x00010000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_CPU_INTR_SHIFT    16
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_MSG_STAT_CPU_INTR [15:15] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_STAT_CPU_INTR_MASK 0x00008000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_STAT_CPU_INTR_SHIFT 15
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_MSG_STAT_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_OVFL_CPU_INTR [14:14] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_OVFL_CPU_INTR_MASK    0x00004000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_OVFL_CPU_INTR_SHIFT   14
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_OVFL_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: XPT_FE_CPU_INTR [13:13] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_FE_CPU_INTR_MASK      0x00002000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_FE_CPU_INTR_SHIFT     13
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_XPT_FE_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: V3D_HUB_CPU_INTR [12:12] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_HUB_CPU_INTR_MASK     0x00001000
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_HUB_CPU_INTR_SHIFT    12
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_HUB_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: V3D_CPU_INTR [11:11] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_CPU_INTR_MASK         0x00000800
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_CPU_INTR_SHIFT        11
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_V3D_CPU_INTR_DEFAULT      0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: EXT_IRQ_03_CPU_INTR [10:10] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_03_CPU_INTR_MASK  0x00000400
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_03_CPU_INTR_SHIFT 10
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_03_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: EXT_IRQ_02_CPU_INTR [09:09] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_02_CPU_INTR_MASK  0x00000200
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_02_CPU_INTR_SHIFT 9
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_02_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: EXT_IRQ_01_CPU_INTR [08:08] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_01_CPU_INTR_MASK  0x00000100
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_01_CPU_INTR_SHIFT 8
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_01_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: EXT_IRQ_00_CPU_INTR [07:07] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_00_CPU_INTR_MASK  0x00000080
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_00_CPU_INTR_SHIFT 7
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_EXT_IRQ_00_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_UART2_CPU_INTR [06:06] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART2_CPU_INTR_MASK   0x00000040
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART2_CPU_INTR_SHIFT  6
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART2_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_UART1_CPU_INTR [05:05] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART1_CPU_INTR_MASK   0x00000020
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART1_CPU_INTR_SHIFT  5
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_UART0_CPU_INTR [04:04] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART0_CPU_INTR_MASK   0x00000010
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART0_CPU_INTR_SHIFT  4
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_UART0_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_TMR_CPU_INTR [03:03] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_TMR_CPU_INTR_MASK     0x00000008
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_TMR_CPU_INTR_SHIFT    3
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_TMR_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_SPI_CPU_INTR [02:02] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SPI_CPU_INTR_MASK     0x00000004
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SPI_CPU_INTR_SHIFT    2
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SPI_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_SC_CPU_INTR [01:01] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SC_CPU_INTR_MASK      0x00000002
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SC_CPU_INTR_SHIFT     1
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_SC_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W2_MASK :: UPG_MAIN_AON_CPU_INTR [00:00] */
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_MAIN_AON_CPU_INTR_MASK 0x00000001
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_MAIN_AON_CPU_INTR_SHIFT 0
#define BCHP_WEBHIF_L1_MASK_INTR_W2_MASK_UPG_MAIN_AON_CPU_INTR_DEFAULT 0x00000001

/***************************************************************************
 *INTR_W3_MASK - WebHIF Interrupt Mask Register 3
 ***************************************************************************/
/* WEBHIF_L1_MASK :: INTR_W3_MASK :: reserved0 [31:08] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_reserved0_MASK            0xffffff00
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_reserved0_SHIFT           8

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: USBC_CPU_INTR [07:07] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_USBC_CPU_INTR_MASK        0x00000080
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_USBC_CPU_INTR_SHIFT       7
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_USBC_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: SID0_0_CPU_INTR [06:06] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_SID0_0_CPU_INTR_MASK      0x00000040
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_SID0_0_CPU_INTR_SHIFT     6
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_SID0_0_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: UPG_MC_CPU_INTR [05:05] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_MC_CPU_INTR_MASK      0x00000020
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_MC_CPU_INTR_SHIFT     5
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_MC_CPU_INTR_DEFAULT   0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: MPM_TOP_CPU_INTR [04:04] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_MPM_TOP_CPU_INTR_MASK     0x00000010
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_MPM_TOP_CPU_INTR_SHIFT    4
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_MPM_TOP_CPU_INTR_DEFAULT  0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: CBUS_CPU_INTR [03:03] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_CBUS_CPU_INTR_MASK        0x00000008
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_CBUS_CPU_INTR_SHIFT       3
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_CBUS_CPU_INTR_DEFAULT     0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: UPG_UART_DMA2_CPU_INTR [02:02] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA2_CPU_INTR_MASK 0x00000004
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA2_CPU_INTR_SHIFT 2
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA2_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: UPG_UART_DMA1_CPU_INTR [01:01] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA1_CPU_INTR_MASK 0x00000002
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA1_CPU_INTR_SHIFT 1
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA1_CPU_INTR_DEFAULT 0x00000001

/* WEBHIF_L1_MASK :: INTR_W3_MASK :: UPG_UART_DMA0_CPU_INTR [00:00] */
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA0_CPU_INTR_MASK 0x00000001
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA0_CPU_INTR_SHIFT 0
#define BCHP_WEBHIF_L1_MASK_INTR_W3_MASK_UPG_UART_DMA0_CPU_INTR_DEFAULT 0x00000001

#endif /* #ifndef BCHP_WEBHIF_L1_MASK_H__ */

/* End of File */
