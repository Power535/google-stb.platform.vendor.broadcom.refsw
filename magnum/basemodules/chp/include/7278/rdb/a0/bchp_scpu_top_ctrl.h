/********************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Mon Sep 12 14:24:35 2016
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

#ifndef BCHP_SCPU_TOP_CTRL_H__
#define BCHP_SCPU_TOP_CTRL_H__

/***************************************************************************
 *SCPU_TOP_CTRL - SCPU Top Control Registers
 ***************************************************************************/
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR       0x00312000 /* [RW][32] SCPU MIPS RESET VECTOR Register */
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY        0x00312004 /* [RW][32] SCPU to Host Irdy Register */
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE        0x00312008 /* [WO][32] SCPU to BSP DDONE Register */

/***************************************************************************
 *ALT_BOOT_VECTOR - SCPU MIPS RESET VECTOR Register
 ***************************************************************************/
/* SCPU_TOP_CTRL :: ALT_BOOT_VECTOR :: ALT_BOOT_VECTOR [31:20] */
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR_ALT_BOOT_VECTOR_MASK    0xfff00000
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR_ALT_BOOT_VECTOR_SHIFT   20
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR_ALT_BOOT_VECTOR_DEFAULT 0x00000b03

/* SCPU_TOP_CTRL :: ALT_BOOT_VECTOR :: reserved0 [19:00] */
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR_reserved0_MASK          0x000fffff
#define BCHP_SCPU_TOP_CTRL_ALT_BOOT_VECTOR_reserved0_SHIFT         0

/***************************************************************************
 *SCPU_HOST_IRDY - SCPU to Host Irdy Register
 ***************************************************************************/
/* SCPU_TOP_CTRL :: SCPU_HOST_IRDY :: reserved0 [31:01] */
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY_reserved0_MASK           0xfffffffe
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY_reserved0_SHIFT          1

/* SCPU_TOP_CTRL :: SCPU_HOST_IRDY :: SCPU_HOST_IRDY [00:00] */
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY_SCPU_HOST_IRDY_MASK      0x00000001
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY_SCPU_HOST_IRDY_SHIFT     0
#define BCHP_SCPU_TOP_CTRL_SCPU_HOST_IRDY_SCPU_HOST_IRDY_DEFAULT   0x00000000

/***************************************************************************
 *SCPU_BSP_DDONE - SCPU to BSP DDONE Register
 ***************************************************************************/
/* SCPU_TOP_CTRL :: SCPU_BSP_DDONE :: reserved0 [31:01] */
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE_reserved0_MASK           0xfffffffe
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE_reserved0_SHIFT          1

/* SCPU_TOP_CTRL :: SCPU_BSP_DDONE :: SCPU_BSP_DDONE [00:00] */
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE_SCPU_BSP_DDONE_MASK      0x00000001
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE_SCPU_BSP_DDONE_SHIFT     0
#define BCHP_SCPU_TOP_CTRL_SCPU_BSP_DDONE_SCPU_BSP_DDONE_DEFAULT   0x00000000

#endif /* #ifndef BCHP_SCPU_TOP_CTRL_H__ */

/* End of File */
