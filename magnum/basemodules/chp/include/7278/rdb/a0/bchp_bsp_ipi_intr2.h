/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
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
 * Date:           Generated on               Mon Sep 12 14:24:34 2016
 *                 Full Compile MD5 Checksum  2d2ed423991a1e4e4d03ca98bc390b2c
 *                     (minus title and desc)
 *                 MD5 Checksum               8cad5c3953d7e5df4439153720b00628
 *
 * lock_release:   r_1099
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1119
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_BSP_IPI_INTR2_H__
#define BCHP_BSP_IPI_INTR2_H__

/***************************************************************************
 *BSP_IPI_INTR2 - BSP to SCPU Inter-Processor Level 2 Interrupt Controller Registers
 ***************************************************************************/
#define BCHP_BSP_IPI_INTR2_CPU_STATUS            0x00310500 /* [RO][32] CPU interrupt Status Register */
#define BCHP_BSP_IPI_INTR2_CPU_SET               0x00310504 /* [WO][32] CPU interrupt Set Register */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR             0x00310508 /* [WO][32] CPU interrupt Clear Register */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS       0x0031050c /* [RO][32] CPU interrupt Mask Status Register */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET          0x00310510 /* [WO][32] CPU interrupt Mask Set Register */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR        0x00310514 /* [WO][32] CPU interrupt Mask Clear Register */
#define BCHP_BSP_IPI_INTR2_PCI_STATUS            0x00310518 /* [RO][32] PCI interrupt Status Register */
#define BCHP_BSP_IPI_INTR2_PCI_SET               0x0031051c /* [WO][32] PCI interrupt Set Register */
#define BCHP_BSP_IPI_INTR2_PCI_CLEAR             0x00310520 /* [WO][32] PCI interrupt Clear Register */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_STATUS       0x00310524 /* [RO][32] PCI interrupt Mask Status Register */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_SET          0x00310528 /* [WO][32] PCI interrupt Mask Set Register */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_CLEAR        0x0031052c /* [WO][32] PCI interrupt Mask Clear Register */

/***************************************************************************
 *CPU_STATUS - CPU interrupt Status Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_STATUS :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_TIMING_VIOL_MASK    0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_TIMING_VIOL_SHIFT   31
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_TIMING_VIOL_DEFAULT 0x00000000

/* BSP_IPI_INTR2 :: CPU_STATUS :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_ZZYZX_INTR_MASK          0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_ZZYZX_INTR_SHIFT         30
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_ZZYZX_INTR_DEFAULT       0x00000000

/* BSP_IPI_INTR2 :: CPU_STATUS :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_SPARE_IPI_MASK               0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_SPARE_IPI_SHIFT              2
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_SPARE_IPI_DEFAULT            0x00000000

/* BSP_IPI_INTR2 :: CPU_STATUS :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_OLOAD_MASK          0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_OLOAD_SHIFT         1
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_OLOAD_DEFAULT       0x00000000

/* BSP_IPI_INTR2 :: CPU_STATUS :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_DRDY_MASK           0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_DRDY_SHIFT          0
#define BCHP_BSP_IPI_INTR2_CPU_STATUS_BSP_SCPU_DRDY_DEFAULT        0x00000000

/***************************************************************************
 *CPU_SET - CPU interrupt Set Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_SET :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_TIMING_VIOL_MASK       0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_TIMING_VIOL_SHIFT      31
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_TIMING_VIOL_DEFAULT    0x00000000

/* BSP_IPI_INTR2 :: CPU_SET :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_ZZYZX_INTR_MASK             0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_ZZYZX_INTR_SHIFT            30
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_ZZYZX_INTR_DEFAULT          0x00000000

/* BSP_IPI_INTR2 :: CPU_SET :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_SET_SPARE_IPI_MASK                  0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_SET_SPARE_IPI_SHIFT                 2
#define BCHP_BSP_IPI_INTR2_CPU_SET_SPARE_IPI_DEFAULT               0x00000000

/* BSP_IPI_INTR2 :: CPU_SET :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_OLOAD_MASK             0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_OLOAD_SHIFT            1
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_OLOAD_DEFAULT          0x00000000

/* BSP_IPI_INTR2 :: CPU_SET :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_DRDY_MASK              0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_DRDY_SHIFT             0
#define BCHP_BSP_IPI_INTR2_CPU_SET_BSP_SCPU_DRDY_DEFAULT           0x00000000

/***************************************************************************
 *CPU_CLEAR - CPU interrupt Clear Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_CLEAR :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_TIMING_VIOL_MASK     0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_TIMING_VIOL_SHIFT    31
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_TIMING_VIOL_DEFAULT  0x00000000

/* BSP_IPI_INTR2 :: CPU_CLEAR :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_ZZYZX_INTR_MASK           0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_ZZYZX_INTR_SHIFT          30
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_ZZYZX_INTR_DEFAULT        0x00000000

/* BSP_IPI_INTR2 :: CPU_CLEAR :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_SPARE_IPI_MASK                0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_SPARE_IPI_SHIFT               2
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_SPARE_IPI_DEFAULT             0x00000000

/* BSP_IPI_INTR2 :: CPU_CLEAR :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_OLOAD_MASK           0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_OLOAD_SHIFT          1
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_OLOAD_DEFAULT        0x00000000

/* BSP_IPI_INTR2 :: CPU_CLEAR :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_DRDY_MASK            0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_DRDY_SHIFT           0
#define BCHP_BSP_IPI_INTR2_CPU_CLEAR_BSP_SCPU_DRDY_DEFAULT         0x00000000

/***************************************************************************
 *CPU_MASK_STATUS - CPU interrupt Mask Status Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_MASK_STATUS :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_TIMING_VIOL_MASK 0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_TIMING_VIOL_SHIFT 31
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_TIMING_VIOL_DEFAULT 0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_STATUS :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_ZZYZX_INTR_MASK     0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_ZZYZX_INTR_SHIFT    30
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_ZZYZX_INTR_DEFAULT  0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_STATUS :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_SPARE_IPI_MASK          0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_SPARE_IPI_SHIFT         2
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_SPARE_IPI_DEFAULT       0x0fffffff

/* BSP_IPI_INTR2 :: CPU_MASK_STATUS :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_OLOAD_MASK     0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_OLOAD_SHIFT    1
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_OLOAD_DEFAULT  0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_STATUS :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_DRDY_MASK      0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_DRDY_SHIFT     0
#define BCHP_BSP_IPI_INTR2_CPU_MASK_STATUS_BSP_SCPU_DRDY_DEFAULT   0x00000001

/***************************************************************************
 *CPU_MASK_SET - CPU interrupt Mask Set Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_MASK_SET :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_TIMING_VIOL_MASK  0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_TIMING_VIOL_SHIFT 31
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_TIMING_VIOL_DEFAULT 0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_SET :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_ZZYZX_INTR_MASK        0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_ZZYZX_INTR_SHIFT       30
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_ZZYZX_INTR_DEFAULT     0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_SET :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_SPARE_IPI_MASK             0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_SPARE_IPI_SHIFT            2
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_SPARE_IPI_DEFAULT          0x0fffffff

/* BSP_IPI_INTR2 :: CPU_MASK_SET :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_OLOAD_MASK        0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_OLOAD_SHIFT       1
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_OLOAD_DEFAULT     0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_SET :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_DRDY_MASK         0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_DRDY_SHIFT        0
#define BCHP_BSP_IPI_INTR2_CPU_MASK_SET_BSP_SCPU_DRDY_DEFAULT      0x00000001

/***************************************************************************
 *CPU_MASK_CLEAR - CPU interrupt Mask Clear Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: CPU_MASK_CLEAR :: BSP_SCPU_TIMING_VIOL [31:31] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_TIMING_VIOL_MASK 0x80000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_TIMING_VIOL_SHIFT 31
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_TIMING_VIOL_DEFAULT 0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_CLEAR :: BSP_ZZYZX_INTR [30:30] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_ZZYZX_INTR_MASK      0x40000000
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_ZZYZX_INTR_SHIFT     30
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_ZZYZX_INTR_DEFAULT   0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_CLEAR :: SPARE_IPI [29:02] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_SPARE_IPI_MASK           0x3ffffffc
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_SPARE_IPI_SHIFT          2
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_SPARE_IPI_DEFAULT        0x0fffffff

/* BSP_IPI_INTR2 :: CPU_MASK_CLEAR :: BSP_SCPU_OLOAD [01:01] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_OLOAD_MASK      0x00000002
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_OLOAD_SHIFT     1
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_OLOAD_DEFAULT   0x00000001

/* BSP_IPI_INTR2 :: CPU_MASK_CLEAR :: BSP_SCPU_DRDY [00:00] */
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_DRDY_MASK       0x00000001
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_DRDY_SHIFT      0
#define BCHP_BSP_IPI_INTR2_CPU_MASK_CLEAR_BSP_SCPU_DRDY_DEFAULT    0x00000001

/***************************************************************************
 *PCI_STATUS - PCI interrupt Status Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_STATUS :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_STATUS_UNUSED_MASK                  0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_STATUS_UNUSED_SHIFT                 0
#define BCHP_BSP_IPI_INTR2_PCI_STATUS_UNUSED_DEFAULT               0x00000000

/***************************************************************************
 *PCI_SET - PCI interrupt Set Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_SET :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_SET_UNUSED_MASK                     0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_SET_UNUSED_SHIFT                    0
#define BCHP_BSP_IPI_INTR2_PCI_SET_UNUSED_DEFAULT                  0x00000000

/***************************************************************************
 *PCI_CLEAR - PCI interrupt Clear Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_CLEAR :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_CLEAR_UNUSED_MASK                   0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_CLEAR_UNUSED_SHIFT                  0
#define BCHP_BSP_IPI_INTR2_PCI_CLEAR_UNUSED_DEFAULT                0x00000000

/***************************************************************************
 *PCI_MASK_STATUS - PCI interrupt Mask Status Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_MASK_STATUS :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_STATUS_UNUSED_MASK             0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_MASK_STATUS_UNUSED_SHIFT            0
#define BCHP_BSP_IPI_INTR2_PCI_MASK_STATUS_UNUSED_DEFAULT          0xffffffff

/***************************************************************************
 *PCI_MASK_SET - PCI interrupt Mask Set Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_MASK_SET :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_SET_UNUSED_MASK                0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_MASK_SET_UNUSED_SHIFT               0
#define BCHP_BSP_IPI_INTR2_PCI_MASK_SET_UNUSED_DEFAULT             0xffffffff

/***************************************************************************
 *PCI_MASK_CLEAR - PCI interrupt Mask Clear Register
 ***************************************************************************/
/* BSP_IPI_INTR2 :: PCI_MASK_CLEAR :: UNUSED [31:00] */
#define BCHP_BSP_IPI_INTR2_PCI_MASK_CLEAR_UNUSED_MASK              0xffffffff
#define BCHP_BSP_IPI_INTR2_PCI_MASK_CLEAR_UNUSED_SHIFT             0
#define BCHP_BSP_IPI_INTR2_PCI_MASK_CLEAR_UNUSED_DEFAULT           0xffffffff

#endif /* #ifndef BCHP_BSP_IPI_INTR2_H__ */

/* End of File */
