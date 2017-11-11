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
 * Date:           Generated on               Thu Apr 13 10:09:30 2017
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

#ifndef BCHP_AMOLE_ANCIL_0_H__
#define BCHP_AMOLE_ANCIL_0_H__

/***************************************************************************
 *AMOLE_ANCIL_0 - AMOLE_ANCIL_0 registers
 ***************************************************************************/
#define BCHP_AMOLE_ANCIL_0_REVID                 0x000ea800 /* [RO][32] AMOL Encoder Revision ID Register */
#define BCHP_AMOLE_ANCIL_0_CONTROL               0x000ea808 /* [RW][32] Control Register */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP     0x000ea80c /* [RW][32] Top Field Active Line and Base Register */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT     0x000ea810 /* [RW][32] Bottom Field Active Line and Base Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK0      0x000ea81c /* [RW][32] Bank 0 Word 0 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK0      0x000ea820 /* [RW][32] Bank 0 Word 1 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK0      0x000ea824 /* [RW][32] Bank 0 Word 2 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK0      0x000ea828 /* [RW][32] Bank 0 Word 3 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK0      0x000ea82c /* [RW][32] Bank 0 Word 4 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK0      0x000ea830 /* [RW][32] Bank 0 Word 5 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK1      0x000ea834 /* [RW][32] Bank 1 Word 0 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK1      0x000ea838 /* [RW][32] Bank 1 Word 1 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK1      0x000ea83c /* [RW][32] Bank 1 Word 2 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK1      0x000ea840 /* [RW][32] Bank 1 Word 3 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK1      0x000ea844 /* [RW][32] Bank 1 Word 4 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK1      0x000ea848 /* [RW][32] Bank 1 Word 5 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK2      0x000ea84c /* [RW][32] Bank 2 Word 0 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK2      0x000ea850 /* [RW][32] Bank 2 Word 1 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK2      0x000ea854 /* [RW][32] Bank 2 Word 2 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK2      0x000ea858 /* [RW][32] Bank 2 Word 3 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK2      0x000ea85c /* [RW][32] Bank 2 Word 4 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK2      0x000ea860 /* [RW][32] Bank 2 Word 5 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK3      0x000ea864 /* [RW][32] Bank 3 Word 0 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK3      0x000ea868 /* [RW][32] Bank 3 Word 1 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK3      0x000ea86c /* [RW][32] Bank 3 Word 2 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK3      0x000ea870 /* [RW][32] Bank 3 Word 3 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK3      0x000ea874 /* [RW][32] Bank 3 Word 4 Data Register */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK3      0x000ea878 /* [RW][32] Bank 3 Word 5 Data Register */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE        0x000ea87c /* [RW][32] Bytes Per Line */
#define BCHP_AMOLE_ANCIL_0_NULL                  0x000ea880 /* [RW][32] NULL Register */
#define BCHP_AMOLE_ANCIL_0_WRPTR                 0x000ea884 /* [RW][32] Write Pointer Register */
#define BCHP_AMOLE_ANCIL_0_RDPTR                 0x000ea888 /* [RW][32] Read Pointer Register */
#define BCHP_AMOLE_ANCIL_0_STATUS                0x000ea88c /* [RW][32] Status Register */

/***************************************************************************
 *REVID - AMOL Encoder Revision ID Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: REVID :: reserved0 [31:16] */
#define BCHP_AMOLE_ANCIL_0_REVID_reserved0_MASK                    0xffff0000
#define BCHP_AMOLE_ANCIL_0_REVID_reserved0_SHIFT                   16

/* AMOLE_ANCIL_0 :: REVID :: REVID [15:00] */
#define BCHP_AMOLE_ANCIL_0_REVID_REVID_MASK                        0x0000ffff
#define BCHP_AMOLE_ANCIL_0_REVID_REVID_SHIFT                       0
#define BCHP_AMOLE_ANCIL_0_REVID_REVID_DEFAULT                     0x00004000

/***************************************************************************
 *CONTROL - Control Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: CONTROL :: reserved0 [31:20] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved0_MASK                  0xfff00000
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved0_SHIFT                 20

/* AMOLE_ANCIL_0 :: CONTROL :: reserved_for_eco1 [19:16] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved_for_eco1_MASK          0x000f0000
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved_for_eco1_SHIFT         16
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved_for_eco1_DEFAULT       0x00000000

/* AMOLE_ANCIL_0 :: CONTROL :: reserved2 [15:03] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved2_MASK                  0x0000fff8
#define BCHP_AMOLE_ANCIL_0_CONTROL_reserved2_SHIFT                 3

/* AMOLE_ANCIL_0 :: CONTROL :: FIFO_FREEZE [02:02] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_FIFO_FREEZE_MASK                0x00000004
#define BCHP_AMOLE_ANCIL_0_CONTROL_FIFO_FREEZE_SHIFT               2
#define BCHP_AMOLE_ANCIL_0_CONTROL_FIFO_FREEZE_DEFAULT             0x00000000

/* AMOLE_ANCIL_0 :: CONTROL :: NULL_MODE [01:01] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_NULL_MODE_MASK                  0x00000002
#define BCHP_AMOLE_ANCIL_0_CONTROL_NULL_MODE_SHIFT                 1
#define BCHP_AMOLE_ANCIL_0_CONTROL_NULL_MODE_DEFAULT               0x00000000

/* AMOLE_ANCIL_0 :: CONTROL :: ENABLE [00:00] */
#define BCHP_AMOLE_ANCIL_0_CONTROL_ENABLE_MASK                     0x00000001
#define BCHP_AMOLE_ANCIL_0_CONTROL_ENABLE_SHIFT                    0
#define BCHP_AMOLE_ANCIL_0_CONTROL_ENABLE_DEFAULT                  0x00000000
#define BCHP_AMOLE_ANCIL_0_CONTROL_ENABLE_OFF                      0
#define BCHP_AMOLE_ANCIL_0_CONTROL_ENABLE_ON                       1

/***************************************************************************
 *ACT_LINE_BASE_TOP - Top Field Active Line and Base Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_TOP :: reserved0 [31:24] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_reserved0_MASK        0xff000000
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_reserved0_SHIFT       24

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_TOP :: ACTIVE_LINE [23:08] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_ACTIVE_LINE_MASK      0x00ffff00
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_ACTIVE_LINE_SHIFT     8
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_ACTIVE_LINE_DEFAULT   0x00000000

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_TOP :: reserved1 [07:07] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_reserved1_MASK        0x00000080
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_reserved1_SHIFT       7

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_TOP :: BASE [06:00] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_BASE_MASK             0x0000007f
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_BASE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_TOP_BASE_DEFAULT          0x00000014

/***************************************************************************
 *ACT_LINE_BASE_BOT - Bottom Field Active Line and Base Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_BOT :: reserved0 [31:24] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_reserved0_MASK        0xff000000
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_reserved0_SHIFT       24

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_BOT :: ACTIVE_LINE [23:08] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_ACTIVE_LINE_MASK      0x00ffff00
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_ACTIVE_LINE_SHIFT     8
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_ACTIVE_LINE_DEFAULT   0x00000000

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_BOT :: reserved1 [07:07] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_reserved1_MASK        0x00000080
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_reserved1_SHIFT       7

/* AMOLE_ANCIL_0 :: ACT_LINE_BASE_BOT :: BASE [06:00] */
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_BASE_MASK             0x0000007f
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_BASE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_ACT_LINE_BASE_BOT_BASE_DEFAULT          0x0000001b

/***************************************************************************
 *DATA_WORD0_BANK0 - Bank 0 Word 0 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD0_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD1_BANK0 - Bank 0 Word 1 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD1_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD2_BANK0 - Bank 0 Word 2 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD2_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD3_BANK0 - Bank 0 Word 3 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD3_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD4_BANK0 - Bank 0 Word 4 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD4_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD5_BANK0 - Bank 0 Word 5 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD5_BANK0 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK0_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK0_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK0_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD0_BANK1 - Bank 1 Word 0 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD0_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD1_BANK1 - Bank 1 Word 1 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD1_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD2_BANK1 - Bank 1 Word 2 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD2_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD3_BANK1 - Bank 1 Word 3 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD3_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD4_BANK1 - Bank 1 Word 4 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD4_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD5_BANK1 - Bank 1 Word 5 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD5_BANK1 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK1_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK1_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK1_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD0_BANK2 - Bank 2 Word 0 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD0_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD1_BANK2 - Bank 2 Word 1 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD1_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD2_BANK2 - Bank 2 Word 2 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD2_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD3_BANK2 - Bank 2 Word 3 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD3_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD4_BANK2 - Bank 2 Word 4 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD4_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD5_BANK2 - Bank 2 Word 5 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD5_BANK2 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK2_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK2_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK2_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD0_BANK3 - Bank 3 Word 0 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD0_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD0_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD1_BANK3 - Bank 3 Word 1 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD1_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD1_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD2_BANK3 - Bank 3 Word 2 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD2_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD2_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD3_BANK3 - Bank 3 Word 3 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD3_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD3_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD4_BANK3 - Bank 3 Word 4 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD4_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD4_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *DATA_WORD5_BANK3 - Bank 3 Word 5 Data Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: DATA_WORD5_BANK3 :: VALUE [31:00] */
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK3_VALUE_MASK             0xffffffff
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK3_VALUE_SHIFT            0
#define BCHP_AMOLE_ANCIL_0_DATA_WORD5_BANK3_VALUE_DEFAULT          0x00000000

/***************************************************************************
 *BYTES_PER_LINE - Bytes Per Line
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: reserved0 [31:08] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_reserved0_MASK           0xffffff00
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_reserved0_SHIFT          8

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE22_BANK3 [07:07] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK3_MASK        0x00000080
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK3_SHIFT       7
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK3_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK3_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK3_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE20_BANK3 [06:06] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK3_MASK        0x00000040
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK3_SHIFT       6
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK3_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK3_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK3_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE22_BANK2 [05:05] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK2_MASK        0x00000020
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK2_SHIFT       5
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK2_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK2_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK2_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE20_BANK2 [04:04] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK2_MASK        0x00000010
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK2_SHIFT       4
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK2_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK2_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK2_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE22_BANK1 [03:03] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK1_MASK        0x00000008
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK1_SHIFT       3
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK1_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK1_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK1_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE20_BANK1 [02:02] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK1_MASK        0x00000004
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK1_SHIFT       2
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK1_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK1_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK1_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE22_BANK0 [01:01] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK0_MASK        0x00000002
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK0_SHIFT       1
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK0_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK0_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE22_BANK0_TWELVE_BYTES 1

/* AMOLE_ANCIL_0 :: BYTES_PER_LINE :: LINE20_BANK0 [00:00] */
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK0_MASK        0x00000001
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK0_SHIFT       0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK0_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK0_SIX_BYTES   0
#define BCHP_AMOLE_ANCIL_0_BYTES_PER_LINE_LINE20_BANK0_TWELVE_BYTES 1

/***************************************************************************
 *NULL - NULL Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: NULL :: reserved0 [31:12] */
#define BCHP_AMOLE_ANCIL_0_NULL_reserved0_MASK                     0xfffff000
#define BCHP_AMOLE_ANCIL_0_NULL_reserved0_SHIFT                    12

/* AMOLE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK3 [11:11] */
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK3_MASK             0x00000800
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK3_SHIFT            11
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK3_DEFAULT          0x00000000

/* AMOLE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK2 [10:10] */
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK2_MASK             0x00000400
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK2_SHIFT            10
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK2_DEFAULT          0x00000000

/* AMOLE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK1 [09:09] */
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK1_MASK             0x00000200
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK1_SHIFT            9
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK1_DEFAULT          0x00000000

/* AMOLE_ANCIL_0 :: NULL :: NULL_ENABLE_BANK0 [08:08] */
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK0_MASK             0x00000100
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK0_SHIFT            8
#define BCHP_AMOLE_ANCIL_0_NULL_NULL_ENABLE_BANK0_DEFAULT          0x00000000

/* AMOLE_ANCIL_0 :: NULL :: CHARACTER [07:00] */
#define BCHP_AMOLE_ANCIL_0_NULL_CHARACTER_MASK                     0x000000ff
#define BCHP_AMOLE_ANCIL_0_NULL_CHARACTER_SHIFT                    0
#define BCHP_AMOLE_ANCIL_0_NULL_CHARACTER_DEFAULT                  0x00000000

/***************************************************************************
 *WRPTR - Write Pointer Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: WRPTR :: reserved0 [31:03] */
#define BCHP_AMOLE_ANCIL_0_WRPTR_reserved0_MASK                    0xfffffff8
#define BCHP_AMOLE_ANCIL_0_WRPTR_reserved0_SHIFT                   3

/* AMOLE_ANCIL_0 :: WRPTR :: VALUE [02:00] */
#define BCHP_AMOLE_ANCIL_0_WRPTR_VALUE_MASK                        0x00000007
#define BCHP_AMOLE_ANCIL_0_WRPTR_VALUE_SHIFT                       0
#define BCHP_AMOLE_ANCIL_0_WRPTR_VALUE_DEFAULT                     0x00000000

/***************************************************************************
 *RDPTR - Read Pointer Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: RDPTR :: reserved0 [31:03] */
#define BCHP_AMOLE_ANCIL_0_RDPTR_reserved0_MASK                    0xfffffff8
#define BCHP_AMOLE_ANCIL_0_RDPTR_reserved0_SHIFT                   3

/* AMOLE_ANCIL_0 :: RDPTR :: VALUE [02:00] */
#define BCHP_AMOLE_ANCIL_0_RDPTR_VALUE_MASK                        0x00000007
#define BCHP_AMOLE_ANCIL_0_RDPTR_VALUE_SHIFT                       0
#define BCHP_AMOLE_ANCIL_0_RDPTR_VALUE_DEFAULT                     0x00000000

/***************************************************************************
 *STATUS - Status Register
 ***************************************************************************/
/* AMOLE_ANCIL_0 :: STATUS :: reserved0 [31:06] */
#define BCHP_AMOLE_ANCIL_0_STATUS_reserved0_MASK                   0xffffffc0
#define BCHP_AMOLE_ANCIL_0_STATUS_reserved0_SHIFT                  6

/* AMOLE_ANCIL_0 :: STATUS :: FIFO_OVERFLOW [05:05] */
#define BCHP_AMOLE_ANCIL_0_STATUS_FIFO_OVERFLOW_MASK               0x00000020
#define BCHP_AMOLE_ANCIL_0_STATUS_FIFO_OVERFLOW_SHIFT              5
#define BCHP_AMOLE_ANCIL_0_STATUS_FIFO_OVERFLOW_DEFAULT            0x00000000

/* AMOLE_ANCIL_0 :: STATUS :: FIFO_UNDERFLOW [04:04] */
#define BCHP_AMOLE_ANCIL_0_STATUS_FIFO_UNDERFLOW_MASK              0x00000010
#define BCHP_AMOLE_ANCIL_0_STATUS_FIFO_UNDERFLOW_SHIFT             4

/* AMOLE_ANCIL_0 :: STATUS :: BANK3_656_ANCIL_DATA [03:03] */
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_MASK        0x00000008
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_SHIFT       3
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_XMIT        1
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK3_656_ANCIL_DATA_NOT_XMIT    0

/* AMOLE_ANCIL_0 :: STATUS :: BANK2_656_ANCIL_DATA [02:02] */
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_MASK        0x00000004
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_SHIFT       2
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_XMIT        1
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK2_656_ANCIL_DATA_NOT_XMIT    0

/* AMOLE_ANCIL_0 :: STATUS :: BANK1_656_ANCIL_DATA [01:01] */
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_MASK        0x00000002
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_SHIFT       1
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_XMIT        1
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK1_656_ANCIL_DATA_NOT_XMIT    0

/* AMOLE_ANCIL_0 :: STATUS :: BANK0_656_ANCIL_DATA [00:00] */
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_MASK        0x00000001
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_SHIFT       0
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_DEFAULT     0x00000000
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_XMIT        1
#define BCHP_AMOLE_ANCIL_0_STATUS_BANK0_656_ANCIL_DATA_NOT_XMIT    0

#endif /* #ifndef BCHP_AMOLE_ANCIL_0_H__ */

/* End of File */
