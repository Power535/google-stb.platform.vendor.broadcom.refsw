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
 * Date:           Generated on               Thu Apr 13 10:09:32 2017
 *                 Full Compile MD5 Checksum  7f180d7646477bba2bae1a701efd9ef5
 *                     (minus title and desc)
 *                 MD5 Checksum               a2a4a53aa20c0c2f46073b879159b85d
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1395
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL tools/dvtsw/current/Linux/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_USB_INTR2_H__
#define BCHP_USB_INTR2_H__

/***************************************************************************
 *USB_INTR2 - USB Level 2 Interrupt Registers
 ***************************************************************************/
#define BCHP_USB_INTR2_CPU_STATUS                0x00b38180 /* [RO][32] CPU interrupt Status Register */
#define BCHP_USB_INTR2_CPU_SET                   0x00b38184 /* [WO][32] CPU interrupt Set Register */
#define BCHP_USB_INTR2_CPU_CLEAR                 0x00b38188 /* [WO][32] CPU interrupt Clear Register */
#define BCHP_USB_INTR2_CPU_MASK_STATUS           0x00b3818c /* [RO][32] CPU interrupt Mask Status Register */
#define BCHP_USB_INTR2_CPU_MASK_SET              0x00b38190 /* [WO][32] CPU interrupt Mask Set Register */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR            0x00b38194 /* [WO][32] CPU interrupt Mask Clear Register */
#define BCHP_USB_INTR2_PCI_STATUS                0x00b38198 /* [RO][32] PCI interrupt Status Register */
#define BCHP_USB_INTR2_PCI_SET                   0x00b3819c /* [WO][32] PCI interrupt Set Register */
#define BCHP_USB_INTR2_PCI_CLEAR                 0x00b381a0 /* [WO][32] PCI interrupt Clear Register */
#define BCHP_USB_INTR2_PCI_MASK_STATUS           0x00b381a4 /* [RO][32] PCI interrupt Mask Status Register */
#define BCHP_USB_INTR2_PCI_MASK_SET              0x00b381a8 /* [WO][32] PCI interrupt Mask Set Register */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR            0x00b381ac /* [WO][32] PCI interrupt Mask Clear Register */

/***************************************************************************
 *CPU_STATUS - CPU interrupt Status Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_STATUS :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_STATUS_reserved0_MASK                   0xffffff00
#define BCHP_USB_INTR2_CPU_STATUS_reserved0_SHIFT                  8

/* USB_INTR2 :: CPU_STATUS :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_CPU_STATUS_USB_DRD_STATE_CHG_INTR_MASK      0x00000080
#define BCHP_USB_INTR2_CPU_STATUS_USB_DRD_STATE_CHG_INTR_SHIFT     7
#define BCHP_USB_INTR2_CPU_STATUS_USB_DRD_STATE_CHG_INTR_DEFAULT   0x00000000

/* USB_INTR2 :: CPU_STATUS :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_CPU_STATUS_USB_BDC_HSE_INTR_MASK            0x00000040
#define BCHP_USB_INTR2_CPU_STATUS_USB_BDC_HSE_INTR_SHIFT           6
#define BCHP_USB_INTR2_CPU_STATUS_USB_BDC_HSE_INTR_DEFAULT         0x00000000

/* USB_INTR2 :: CPU_STATUS :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_CPU_STATUS_USB_SCB_INVLD_ADDR_INTR_MASK     0x00000020
#define BCHP_USB_INTR2_CPU_STATUS_USB_SCB_INVLD_ADDR_INTR_SHIFT    5
#define BCHP_USB_INTR2_CPU_STATUS_USB_SCB_INVLD_ADDR_INTR_DEFAULT  0x00000000

/* USB_INTR2 :: CPU_STATUS :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_CPU_STATUS_USB_XHC_HSE_INTR_MASK            0x00000010
#define BCHP_USB_INTR2_CPU_STATUS_USB_XHC_HSE_INTR_SHIFT           4
#define BCHP_USB_INTR2_CPU_STATUS_USB_XHC_HSE_INTR_DEFAULT         0x00000000

/* USB_INTR2 :: CPU_STATUS :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_STATUS_reserved1_MASK                   0x0000000e
#define BCHP_USB_INTR2_CPU_STATUS_reserved1_SHIFT                  1

/* USB_INTR2 :: CPU_STATUS :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_CPU_STATUS_USB_GR_BRIDGE_INTR_MASK          0x00000001
#define BCHP_USB_INTR2_CPU_STATUS_USB_GR_BRIDGE_INTR_SHIFT         0
#define BCHP_USB_INTR2_CPU_STATUS_USB_GR_BRIDGE_INTR_DEFAULT       0x00000000

/***************************************************************************
 *CPU_SET - CPU interrupt Set Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_SET :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_SET_reserved0_MASK                      0xffffff00
#define BCHP_USB_INTR2_CPU_SET_reserved0_SHIFT                     8

/* USB_INTR2 :: CPU_SET :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_CPU_SET_USB_DRD_STATE_CHG_INTR_MASK         0x00000080
#define BCHP_USB_INTR2_CPU_SET_USB_DRD_STATE_CHG_INTR_SHIFT        7
#define BCHP_USB_INTR2_CPU_SET_USB_DRD_STATE_CHG_INTR_DEFAULT      0x00000000

/* USB_INTR2 :: CPU_SET :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_CPU_SET_USB_BDC_HSE_INTR_MASK               0x00000040
#define BCHP_USB_INTR2_CPU_SET_USB_BDC_HSE_INTR_SHIFT              6
#define BCHP_USB_INTR2_CPU_SET_USB_BDC_HSE_INTR_DEFAULT            0x00000000

/* USB_INTR2 :: CPU_SET :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_CPU_SET_USB_SCB_INVLD_ADDR_INTR_MASK        0x00000020
#define BCHP_USB_INTR2_CPU_SET_USB_SCB_INVLD_ADDR_INTR_SHIFT       5
#define BCHP_USB_INTR2_CPU_SET_USB_SCB_INVLD_ADDR_INTR_DEFAULT     0x00000000

/* USB_INTR2 :: CPU_SET :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_CPU_SET_USB_XHC_HSE_INTR_MASK               0x00000010
#define BCHP_USB_INTR2_CPU_SET_USB_XHC_HSE_INTR_SHIFT              4
#define BCHP_USB_INTR2_CPU_SET_USB_XHC_HSE_INTR_DEFAULT            0x00000000

/* USB_INTR2 :: CPU_SET :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_SET_reserved1_MASK                      0x0000000e
#define BCHP_USB_INTR2_CPU_SET_reserved1_SHIFT                     1

/* USB_INTR2 :: CPU_SET :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_CPU_SET_USB_GR_BRIDGE_INTR_MASK             0x00000001
#define BCHP_USB_INTR2_CPU_SET_USB_GR_BRIDGE_INTR_SHIFT            0
#define BCHP_USB_INTR2_CPU_SET_USB_GR_BRIDGE_INTR_DEFAULT          0x00000000

/***************************************************************************
 *CPU_CLEAR - CPU interrupt Clear Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_CLEAR :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_CLEAR_reserved0_MASK                    0xffffff00
#define BCHP_USB_INTR2_CPU_CLEAR_reserved0_SHIFT                   8

/* USB_INTR2 :: CPU_CLEAR :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_CPU_CLEAR_USB_DRD_STATE_CHG_INTR_MASK       0x00000080
#define BCHP_USB_INTR2_CPU_CLEAR_USB_DRD_STATE_CHG_INTR_SHIFT      7
#define BCHP_USB_INTR2_CPU_CLEAR_USB_DRD_STATE_CHG_INTR_DEFAULT    0x00000000

/* USB_INTR2 :: CPU_CLEAR :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_CPU_CLEAR_USB_BDC_HSE_INTR_MASK             0x00000040
#define BCHP_USB_INTR2_CPU_CLEAR_USB_BDC_HSE_INTR_SHIFT            6
#define BCHP_USB_INTR2_CPU_CLEAR_USB_BDC_HSE_INTR_DEFAULT          0x00000000

/* USB_INTR2 :: CPU_CLEAR :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_CPU_CLEAR_USB_SCB_INVLD_ADDR_INTR_MASK      0x00000020
#define BCHP_USB_INTR2_CPU_CLEAR_USB_SCB_INVLD_ADDR_INTR_SHIFT     5
#define BCHP_USB_INTR2_CPU_CLEAR_USB_SCB_INVLD_ADDR_INTR_DEFAULT   0x00000000

/* USB_INTR2 :: CPU_CLEAR :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_CPU_CLEAR_USB_XHC_HSE_INTR_MASK             0x00000010
#define BCHP_USB_INTR2_CPU_CLEAR_USB_XHC_HSE_INTR_SHIFT            4
#define BCHP_USB_INTR2_CPU_CLEAR_USB_XHC_HSE_INTR_DEFAULT          0x00000000

/* USB_INTR2 :: CPU_CLEAR :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_CLEAR_reserved1_MASK                    0x0000000e
#define BCHP_USB_INTR2_CPU_CLEAR_reserved1_SHIFT                   1

/* USB_INTR2 :: CPU_CLEAR :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_CPU_CLEAR_USB_GR_BRIDGE_INTR_MASK           0x00000001
#define BCHP_USB_INTR2_CPU_CLEAR_USB_GR_BRIDGE_INTR_SHIFT          0
#define BCHP_USB_INTR2_CPU_CLEAR_USB_GR_BRIDGE_INTR_DEFAULT        0x00000000

/***************************************************************************
 *CPU_MASK_STATUS - CPU interrupt Mask Status Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_MASK_STATUS :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_reserved0_MASK              0xffffff00
#define BCHP_USB_INTR2_CPU_MASK_STATUS_reserved0_SHIFT             8

/* USB_INTR2 :: CPU_MASK_STATUS :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_DRD_STATE_CHG_MASK_MASK 0x00000080
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_DRD_STATE_CHG_MASK_SHIFT 7
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_STATUS :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_BDC_HSE_MASK_MASK       0x00000040
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_BDC_HSE_MASK_SHIFT      6
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_BDC_HSE_MASK_DEFAULT    0x00000001

/* USB_INTR2 :: CPU_MASK_STATUS :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_MASK 0x00000020
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_SHIFT 5
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_STATUS :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_XHC_HSE_INTR_MASK_MASK  0x00000010
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_XHC_HSE_INTR_MASK_SHIFT 4
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_XHC_HSE_INTR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_STATUS :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_reserved1_MASK              0x0000000e
#define BCHP_USB_INTR2_CPU_MASK_STATUS_reserved1_SHIFT             1

/* USB_INTR2 :: CPU_MASK_STATUS :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_MASK 0x00000001
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_SHIFT 0
#define BCHP_USB_INTR2_CPU_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

/***************************************************************************
 *CPU_MASK_SET - CPU interrupt Mask Set Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_MASK_SET :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_MASK_SET_reserved0_MASK                 0xffffff00
#define BCHP_USB_INTR2_CPU_MASK_SET_reserved0_SHIFT                8

/* USB_INTR2 :: CPU_MASK_SET :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_DRD_STATE_CHG_MASK_MASK    0x00000080
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_DRD_STATE_CHG_MASK_SHIFT   7
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_SET :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_BDC_HSE_MASK_MASK          0x00000040
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_BDC_HSE_MASK_SHIFT         6
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_BDC_HSE_MASK_DEFAULT       0x00000001

/* USB_INTR2 :: CPU_MASK_SET :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_SCB_INVLD_ADDR_MASK_MASK   0x00000020
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_SCB_INVLD_ADDR_MASK_SHIFT  5
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_SET :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_XHC_HSE_INTR_MASK_MASK     0x00000010
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_XHC_HSE_INTR_MASK_SHIFT    4
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_XHC_HSE_INTR_MASK_DEFAULT  0x00000001

/* USB_INTR2 :: CPU_MASK_SET :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_MASK_SET_reserved1_MASK                 0x0000000e
#define BCHP_USB_INTR2_CPU_MASK_SET_reserved1_SHIFT                1

/* USB_INTR2 :: CPU_MASK_SET :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_GR_BRIDGE_INTR_MASK_MASK   0x00000001
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_GR_BRIDGE_INTR_MASK_SHIFT  0
#define BCHP_USB_INTR2_CPU_MASK_SET_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

/***************************************************************************
 *CPU_MASK_CLEAR - CPU interrupt Mask Clear Register
 ***************************************************************************/
/* USB_INTR2 :: CPU_MASK_CLEAR :: reserved0 [31:08] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_reserved0_MASK               0xffffff00
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_reserved0_SHIFT              8

/* USB_INTR2 :: CPU_MASK_CLEAR :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_MASK  0x00000080
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_SHIFT 7
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_CLEAR :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_BDC_HSE_MASK_MASK        0x00000040
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_BDC_HSE_MASK_SHIFT       6
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_BDC_HSE_MASK_DEFAULT     0x00000001

/* USB_INTR2 :: CPU_MASK_CLEAR :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_MASK 0x00000020
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_SHIFT 5
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_CLEAR :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_MASK   0x00000010
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_SHIFT  4
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: CPU_MASK_CLEAR :: reserved1 [03:01] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_reserved1_MASK               0x0000000e
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_reserved1_SHIFT              1

/* USB_INTR2 :: CPU_MASK_CLEAR :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_MASK 0x00000001
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_SHIFT 0
#define BCHP_USB_INTR2_CPU_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

/***************************************************************************
 *PCI_STATUS - PCI interrupt Status Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_STATUS :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_STATUS_reserved0_MASK                   0xffffff00
#define BCHP_USB_INTR2_PCI_STATUS_reserved0_SHIFT                  8

/* USB_INTR2 :: PCI_STATUS :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_PCI_STATUS_USB_DRD_STATE_CHG_INTR_MASK      0x00000080
#define BCHP_USB_INTR2_PCI_STATUS_USB_DRD_STATE_CHG_INTR_SHIFT     7
#define BCHP_USB_INTR2_PCI_STATUS_USB_DRD_STATE_CHG_INTR_DEFAULT   0x00000000

/* USB_INTR2 :: PCI_STATUS :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_PCI_STATUS_USB_BDC_HSE_INTR_MASK            0x00000040
#define BCHP_USB_INTR2_PCI_STATUS_USB_BDC_HSE_INTR_SHIFT           6
#define BCHP_USB_INTR2_PCI_STATUS_USB_BDC_HSE_INTR_DEFAULT         0x00000000

/* USB_INTR2 :: PCI_STATUS :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_PCI_STATUS_USB_SCB_INVLD_ADDR_INTR_MASK     0x00000020
#define BCHP_USB_INTR2_PCI_STATUS_USB_SCB_INVLD_ADDR_INTR_SHIFT    5
#define BCHP_USB_INTR2_PCI_STATUS_USB_SCB_INVLD_ADDR_INTR_DEFAULT  0x00000000

/* USB_INTR2 :: PCI_STATUS :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_PCI_STATUS_USB_XHC_HSE_INTR_MASK            0x00000010
#define BCHP_USB_INTR2_PCI_STATUS_USB_XHC_HSE_INTR_SHIFT           4
#define BCHP_USB_INTR2_PCI_STATUS_USB_XHC_HSE_INTR_DEFAULT         0x00000000

/* USB_INTR2 :: PCI_STATUS :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_STATUS_reserved1_MASK                   0x0000000e
#define BCHP_USB_INTR2_PCI_STATUS_reserved1_SHIFT                  1

/* USB_INTR2 :: PCI_STATUS :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_PCI_STATUS_USB_GR_BRIDGE_INTR_MASK          0x00000001
#define BCHP_USB_INTR2_PCI_STATUS_USB_GR_BRIDGE_INTR_SHIFT         0
#define BCHP_USB_INTR2_PCI_STATUS_USB_GR_BRIDGE_INTR_DEFAULT       0x00000000

/***************************************************************************
 *PCI_SET - PCI interrupt Set Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_SET :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_SET_reserved0_MASK                      0xffffff00
#define BCHP_USB_INTR2_PCI_SET_reserved0_SHIFT                     8

/* USB_INTR2 :: PCI_SET :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_PCI_SET_USB_DRD_STATE_CHG_INTR_MASK         0x00000080
#define BCHP_USB_INTR2_PCI_SET_USB_DRD_STATE_CHG_INTR_SHIFT        7
#define BCHP_USB_INTR2_PCI_SET_USB_DRD_STATE_CHG_INTR_DEFAULT      0x00000000

/* USB_INTR2 :: PCI_SET :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_PCI_SET_USB_BDC_HSE_INTR_MASK               0x00000040
#define BCHP_USB_INTR2_PCI_SET_USB_BDC_HSE_INTR_SHIFT              6
#define BCHP_USB_INTR2_PCI_SET_USB_BDC_HSE_INTR_DEFAULT            0x00000000

/* USB_INTR2 :: PCI_SET :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_PCI_SET_USB_SCB_INVLD_ADDR_INTR_MASK        0x00000020
#define BCHP_USB_INTR2_PCI_SET_USB_SCB_INVLD_ADDR_INTR_SHIFT       5
#define BCHP_USB_INTR2_PCI_SET_USB_SCB_INVLD_ADDR_INTR_DEFAULT     0x00000000

/* USB_INTR2 :: PCI_SET :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_PCI_SET_USB_XHC_HSE_INTR_MASK               0x00000010
#define BCHP_USB_INTR2_PCI_SET_USB_XHC_HSE_INTR_SHIFT              4
#define BCHP_USB_INTR2_PCI_SET_USB_XHC_HSE_INTR_DEFAULT            0x00000000

/* USB_INTR2 :: PCI_SET :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_SET_reserved1_MASK                      0x0000000e
#define BCHP_USB_INTR2_PCI_SET_reserved1_SHIFT                     1

/* USB_INTR2 :: PCI_SET :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_PCI_SET_USB_GR_BRIDGE_INTR_MASK             0x00000001
#define BCHP_USB_INTR2_PCI_SET_USB_GR_BRIDGE_INTR_SHIFT            0
#define BCHP_USB_INTR2_PCI_SET_USB_GR_BRIDGE_INTR_DEFAULT          0x00000000

/***************************************************************************
 *PCI_CLEAR - PCI interrupt Clear Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_CLEAR :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_CLEAR_reserved0_MASK                    0xffffff00
#define BCHP_USB_INTR2_PCI_CLEAR_reserved0_SHIFT                   8

/* USB_INTR2 :: PCI_CLEAR :: USB_DRD_STATE_CHG_INTR [07:07] */
#define BCHP_USB_INTR2_PCI_CLEAR_USB_DRD_STATE_CHG_INTR_MASK       0x00000080
#define BCHP_USB_INTR2_PCI_CLEAR_USB_DRD_STATE_CHG_INTR_SHIFT      7
#define BCHP_USB_INTR2_PCI_CLEAR_USB_DRD_STATE_CHG_INTR_DEFAULT    0x00000000

/* USB_INTR2 :: PCI_CLEAR :: USB_BDC_HSE_INTR [06:06] */
#define BCHP_USB_INTR2_PCI_CLEAR_USB_BDC_HSE_INTR_MASK             0x00000040
#define BCHP_USB_INTR2_PCI_CLEAR_USB_BDC_HSE_INTR_SHIFT            6
#define BCHP_USB_INTR2_PCI_CLEAR_USB_BDC_HSE_INTR_DEFAULT          0x00000000

/* USB_INTR2 :: PCI_CLEAR :: USB_SCB_INVLD_ADDR_INTR [05:05] */
#define BCHP_USB_INTR2_PCI_CLEAR_USB_SCB_INVLD_ADDR_INTR_MASK      0x00000020
#define BCHP_USB_INTR2_PCI_CLEAR_USB_SCB_INVLD_ADDR_INTR_SHIFT     5
#define BCHP_USB_INTR2_PCI_CLEAR_USB_SCB_INVLD_ADDR_INTR_DEFAULT   0x00000000

/* USB_INTR2 :: PCI_CLEAR :: USB_XHC_HSE_INTR [04:04] */
#define BCHP_USB_INTR2_PCI_CLEAR_USB_XHC_HSE_INTR_MASK             0x00000010
#define BCHP_USB_INTR2_PCI_CLEAR_USB_XHC_HSE_INTR_SHIFT            4
#define BCHP_USB_INTR2_PCI_CLEAR_USB_XHC_HSE_INTR_DEFAULT          0x00000000

/* USB_INTR2 :: PCI_CLEAR :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_CLEAR_reserved1_MASK                    0x0000000e
#define BCHP_USB_INTR2_PCI_CLEAR_reserved1_SHIFT                   1

/* USB_INTR2 :: PCI_CLEAR :: USB_GR_BRIDGE_INTR [00:00] */
#define BCHP_USB_INTR2_PCI_CLEAR_USB_GR_BRIDGE_INTR_MASK           0x00000001
#define BCHP_USB_INTR2_PCI_CLEAR_USB_GR_BRIDGE_INTR_SHIFT          0
#define BCHP_USB_INTR2_PCI_CLEAR_USB_GR_BRIDGE_INTR_DEFAULT        0x00000000

/***************************************************************************
 *PCI_MASK_STATUS - PCI interrupt Mask Status Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_MASK_STATUS :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_reserved0_MASK              0xffffff00
#define BCHP_USB_INTR2_PCI_MASK_STATUS_reserved0_SHIFT             8

/* USB_INTR2 :: PCI_MASK_STATUS :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_DRD_STATE_CHG_MASK_MASK 0x00000080
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_DRD_STATE_CHG_MASK_SHIFT 7
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_STATUS :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_BDC_HSE_MASK_MASK       0x00000040
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_BDC_HSE_MASK_SHIFT      6
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_BDC_HSE_MASK_DEFAULT    0x00000001

/* USB_INTR2 :: PCI_MASK_STATUS :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_MASK 0x00000020
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_SHIFT 5
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_STATUS :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_XHC_HSE_INTR_MASK_MASK  0x00000010
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_XHC_HSE_INTR_MASK_SHIFT 4
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_XHC_HSE_INTR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_STATUS :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_reserved1_MASK              0x0000000e
#define BCHP_USB_INTR2_PCI_MASK_STATUS_reserved1_SHIFT             1

/* USB_INTR2 :: PCI_MASK_STATUS :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_MASK 0x00000001
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_SHIFT 0
#define BCHP_USB_INTR2_PCI_MASK_STATUS_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

/***************************************************************************
 *PCI_MASK_SET - PCI interrupt Mask Set Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_MASK_SET :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_MASK_SET_reserved0_MASK                 0xffffff00
#define BCHP_USB_INTR2_PCI_MASK_SET_reserved0_SHIFT                8

/* USB_INTR2 :: PCI_MASK_SET :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_DRD_STATE_CHG_MASK_MASK    0x00000080
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_DRD_STATE_CHG_MASK_SHIFT   7
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_SET :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_BDC_HSE_MASK_MASK          0x00000040
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_BDC_HSE_MASK_SHIFT         6
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_BDC_HSE_MASK_DEFAULT       0x00000001

/* USB_INTR2 :: PCI_MASK_SET :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_SCB_INVLD_ADDR_MASK_MASK   0x00000020
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_SCB_INVLD_ADDR_MASK_SHIFT  5
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_SET :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_XHC_HSE_INTR_MASK_MASK     0x00000010
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_XHC_HSE_INTR_MASK_SHIFT    4
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_XHC_HSE_INTR_MASK_DEFAULT  0x00000001

/* USB_INTR2 :: PCI_MASK_SET :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_MASK_SET_reserved1_MASK                 0x0000000e
#define BCHP_USB_INTR2_PCI_MASK_SET_reserved1_SHIFT                1

/* USB_INTR2 :: PCI_MASK_SET :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_GR_BRIDGE_INTR_MASK_MASK   0x00000001
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_GR_BRIDGE_INTR_MASK_SHIFT  0
#define BCHP_USB_INTR2_PCI_MASK_SET_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

/***************************************************************************
 *PCI_MASK_CLEAR - PCI interrupt Mask Clear Register
 ***************************************************************************/
/* USB_INTR2 :: PCI_MASK_CLEAR :: reserved0 [31:08] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_reserved0_MASK               0xffffff00
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_reserved0_SHIFT              8

/* USB_INTR2 :: PCI_MASK_CLEAR :: USB_DRD_STATE_CHG_MASK [07:07] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_MASK  0x00000080
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_SHIFT 7
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_DRD_STATE_CHG_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_CLEAR :: USB_BDC_HSE_MASK [06:06] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_BDC_HSE_MASK_MASK        0x00000040
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_BDC_HSE_MASK_SHIFT       6
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_BDC_HSE_MASK_DEFAULT     0x00000001

/* USB_INTR2 :: PCI_MASK_CLEAR :: USB_SCB_INVLD_ADDR_MASK [05:05] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_MASK 0x00000020
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_SHIFT 5
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_SCB_INVLD_ADDR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_CLEAR :: USB_XHC_HSE_INTR_MASK [04:04] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_MASK   0x00000010
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_SHIFT  4
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_XHC_HSE_INTR_MASK_DEFAULT 0x00000001

/* USB_INTR2 :: PCI_MASK_CLEAR :: reserved1 [03:01] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_reserved1_MASK               0x0000000e
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_reserved1_SHIFT              1

/* USB_INTR2 :: PCI_MASK_CLEAR :: USB_GR_BRIDGE_INTR_MASK [00:00] */
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_MASK 0x00000001
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_SHIFT 0
#define BCHP_USB_INTR2_PCI_MASK_CLEAR_USB_GR_BRIDGE_INTR_MASK_DEFAULT 0x00000001

#endif /* #ifndef BCHP_USB_INTR2_H__ */

/* End of File */
