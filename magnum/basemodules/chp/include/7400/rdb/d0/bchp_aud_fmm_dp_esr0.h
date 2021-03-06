/***************************************************************************
 *     Copyright (c) 1999-2007, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Mon Jul 23 12:00:47 2007
 *                 MD5 Checksum         61f9c4d8dcdcd06017506dddbf23f434
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008004
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_AUD_FMM_DP_ESR0_H__
#define BCHP_AUD_FMM_DP_ESR0_H__

/***************************************************************************
 *AUD_FMM_DP_ESR0 - FMM_DP Error Status 0
 ***************************************************************************/
#define BCHP_AUD_FMM_DP_ESR0_STATUS              0x00635000 /* Error Status Register */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET          0x00635004 /* Error Set Register */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR        0x00635008 /* Error Clear Register */
#define BCHP_AUD_FMM_DP_ESR0_MASK                0x0063500c /* Mask Status Register */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET            0x00635010 /* Mask Set Register */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR          0x00635014 /* Mask Clear Register */

/***************************************************************************
 *STATUS - Error Status Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: STATUS :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_reserved0_MASK                 0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_reserved0_SHIFT                16

/* AUD_FMM_DP_ESR0 :: STATUS :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT3_RATE_ERR_MASK          0x00008000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT3_RATE_ERR_SHIFT         15

/* AUD_FMM_DP_ESR0 :: STATUS :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT2_RATE_ERR_MASK          0x00004000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT2_RATE_ERR_SHIFT         14

/* AUD_FMM_DP_ESR0 :: STATUS :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT1_RATE_ERR_MASK          0x00002000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT1_RATE_ERR_SHIFT         13

/* AUD_FMM_DP_ESR0 :: STATUS :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT0_RATE_ERR_MASK          0x00001000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLIENT0_RATE_ERR_SHIFT         12

/* AUD_FMM_DP_ESR0 :: STATUS :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE3_MASK            0x00000800
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE3_SHIFT           11

/* AUD_FMM_DP_ESR0 :: STATUS :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE2_MASK            0x00000400
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE2_SHIFT           10

/* AUD_FMM_DP_ESR0 :: STATUS :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE1_MASK            0x00000200
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE1_SHIFT           9

/* AUD_FMM_DP_ESR0 :: STATUS :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE0_MASK            0x00000100
#define BCHP_AUD_FMM_DP_ESR0_STATUS_VOL_RAMP_DONE0_SHIFT           8

/* AUD_FMM_DP_ESR0 :: STATUS :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE3_MASK          0x00000080
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE3_SHIFT         7

/* AUD_FMM_DP_ESR0 :: STATUS :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE2_MASK          0x00000040
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE2_SHIFT         6

/* AUD_FMM_DP_ESR0 :: STATUS :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE1_MASK          0x00000020
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE1_SHIFT         5

/* AUD_FMM_DP_ESR0 :: STATUS :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE0_MASK          0x00000010
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SCALE_RAMP_DONE0_SHIFT         4

/* AUD_FMM_DP_ESR0 :: STATUS :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW3_MASK                0x00000008
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW3_SHIFT               3

/* AUD_FMM_DP_ESR0 :: STATUS :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW2_MASK                0x00000004
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW2_SHIFT               2

/* AUD_FMM_DP_ESR0 :: STATUS :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW1_MASK                0x00000002
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW1_SHIFT               1

/* AUD_FMM_DP_ESR0 :: STATUS :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW0_MASK                0x00000001
#define BCHP_AUD_FMM_DP_ESR0_STATUS_UNDERFLOW0_SHIFT               0

/***************************************************************************
 *STATUS_SET - Error Set Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: STATUS_SET :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_reserved0_MASK             0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_reserved0_SHIFT            16

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT3_RATE_ERR_MASK      0x00008000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT3_RATE_ERR_SHIFT     15

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT2_RATE_ERR_MASK      0x00004000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT2_RATE_ERR_SHIFT     14

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT1_RATE_ERR_MASK      0x00002000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT1_RATE_ERR_SHIFT     13

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT0_RATE_ERR_MASK      0x00001000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_CLIENT0_RATE_ERR_SHIFT     12

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE3_MASK        0x00000800
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE3_SHIFT       11

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE2_MASK        0x00000400
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE2_SHIFT       10

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE1_MASK        0x00000200
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE1_SHIFT       9

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE0_MASK        0x00000100
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_VOL_RAMP_DONE0_SHIFT       8

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE3_MASK      0x00000080
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE3_SHIFT     7

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE2_MASK      0x00000040
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE2_SHIFT     6

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE1_MASK      0x00000020
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE1_SHIFT     5

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE0_MASK      0x00000010
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_SCALE_RAMP_DONE0_SHIFT     4

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW3_MASK            0x00000008
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW3_SHIFT           3

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW2_MASK            0x00000004
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW2_SHIFT           2

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW1_MASK            0x00000002
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW1_SHIFT           1

/* AUD_FMM_DP_ESR0 :: STATUS_SET :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW0_MASK            0x00000001
#define BCHP_AUD_FMM_DP_ESR0_STATUS_SET_UNDERFLOW0_SHIFT           0

/***************************************************************************
 *STATUS_CLEAR - Error Clear Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_reserved0_MASK           0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_reserved0_SHIFT          16

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT3_RATE_ERR_MASK    0x00008000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT3_RATE_ERR_SHIFT   15

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT2_RATE_ERR_MASK    0x00004000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT2_RATE_ERR_SHIFT   14

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT1_RATE_ERR_MASK    0x00002000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT1_RATE_ERR_SHIFT   13

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT0_RATE_ERR_MASK    0x00001000
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_CLIENT0_RATE_ERR_SHIFT   12

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE3_MASK      0x00000800
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE3_SHIFT     11

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE2_MASK      0x00000400
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE2_SHIFT     10

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE1_MASK      0x00000200
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE1_SHIFT     9

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE0_MASK      0x00000100
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_VOL_RAMP_DONE0_SHIFT     8

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE3_MASK    0x00000080
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE3_SHIFT   7

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE2_MASK    0x00000040
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE2_SHIFT   6

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE1_MASK    0x00000020
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE1_SHIFT   5

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE0_MASK    0x00000010
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_SCALE_RAMP_DONE0_SHIFT   4

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW3_MASK          0x00000008
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW3_SHIFT         3

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW2_MASK          0x00000004
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW2_SHIFT         2

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW1_MASK          0x00000002
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW1_SHIFT         1

/* AUD_FMM_DP_ESR0 :: STATUS_CLEAR :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW0_MASK          0x00000001
#define BCHP_AUD_FMM_DP_ESR0_STATUS_CLEAR_UNDERFLOW0_SHIFT         0

/***************************************************************************
 *MASK - Mask Status Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: MASK :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_reserved0_MASK                   0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_MASK_reserved0_SHIFT                  16

/* AUD_FMM_DP_ESR0 :: MASK :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT3_RATE_ERR_MASK            0x00008000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT3_RATE_ERR_SHIFT           15

/* AUD_FMM_DP_ESR0 :: MASK :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT2_RATE_ERR_MASK            0x00004000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT2_RATE_ERR_SHIFT           14

/* AUD_FMM_DP_ESR0 :: MASK :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT1_RATE_ERR_MASK            0x00002000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT1_RATE_ERR_SHIFT           13

/* AUD_FMM_DP_ESR0 :: MASK :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT0_RATE_ERR_MASK            0x00001000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLIENT0_RATE_ERR_SHIFT           12

/* AUD_FMM_DP_ESR0 :: MASK :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE3_MASK              0x00000800
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE3_SHIFT             11

/* AUD_FMM_DP_ESR0 :: MASK :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE2_MASK              0x00000400
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE2_SHIFT             10

/* AUD_FMM_DP_ESR0 :: MASK :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE1_MASK              0x00000200
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE1_SHIFT             9

/* AUD_FMM_DP_ESR0 :: MASK :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE0_MASK              0x00000100
#define BCHP_AUD_FMM_DP_ESR0_MASK_VOL_RAMP_DONE0_SHIFT             8

/* AUD_FMM_DP_ESR0 :: MASK :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE3_MASK            0x00000080
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE3_SHIFT           7

/* AUD_FMM_DP_ESR0 :: MASK :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE2_MASK            0x00000040
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE2_SHIFT           6

/* AUD_FMM_DP_ESR0 :: MASK :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE1_MASK            0x00000020
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE1_SHIFT           5

/* AUD_FMM_DP_ESR0 :: MASK :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE0_MASK            0x00000010
#define BCHP_AUD_FMM_DP_ESR0_MASK_SCALE_RAMP_DONE0_SHIFT           4

/* AUD_FMM_DP_ESR0 :: MASK :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW3_MASK                  0x00000008
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW3_SHIFT                 3

/* AUD_FMM_DP_ESR0 :: MASK :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW2_MASK                  0x00000004
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW2_SHIFT                 2

/* AUD_FMM_DP_ESR0 :: MASK :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW1_MASK                  0x00000002
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW1_SHIFT                 1

/* AUD_FMM_DP_ESR0 :: MASK :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW0_MASK                  0x00000001
#define BCHP_AUD_FMM_DP_ESR0_MASK_UNDERFLOW0_SHIFT                 0

/***************************************************************************
 *MASK_SET - Mask Set Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: MASK_SET :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_reserved0_MASK               0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_reserved0_SHIFT              16

/* AUD_FMM_DP_ESR0 :: MASK_SET :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT3_RATE_ERR_MASK        0x00008000
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT3_RATE_ERR_SHIFT       15

/* AUD_FMM_DP_ESR0 :: MASK_SET :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT2_RATE_ERR_MASK        0x00004000
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT2_RATE_ERR_SHIFT       14

/* AUD_FMM_DP_ESR0 :: MASK_SET :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT1_RATE_ERR_MASK        0x00002000
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT1_RATE_ERR_SHIFT       13

/* AUD_FMM_DP_ESR0 :: MASK_SET :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT0_RATE_ERR_MASK        0x00001000
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_CLIENT0_RATE_ERR_SHIFT       12

/* AUD_FMM_DP_ESR0 :: MASK_SET :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE3_MASK          0x00000800
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE3_SHIFT         11

/* AUD_FMM_DP_ESR0 :: MASK_SET :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE2_MASK          0x00000400
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE2_SHIFT         10

/* AUD_FMM_DP_ESR0 :: MASK_SET :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE1_MASK          0x00000200
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE1_SHIFT         9

/* AUD_FMM_DP_ESR0 :: MASK_SET :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE0_MASK          0x00000100
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_VOL_RAMP_DONE0_SHIFT         8

/* AUD_FMM_DP_ESR0 :: MASK_SET :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE3_MASK        0x00000080
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE3_SHIFT       7

/* AUD_FMM_DP_ESR0 :: MASK_SET :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE2_MASK        0x00000040
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE2_SHIFT       6

/* AUD_FMM_DP_ESR0 :: MASK_SET :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE1_MASK        0x00000020
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE1_SHIFT       5

/* AUD_FMM_DP_ESR0 :: MASK_SET :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE0_MASK        0x00000010
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_SCALE_RAMP_DONE0_SHIFT       4

/* AUD_FMM_DP_ESR0 :: MASK_SET :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW3_MASK              0x00000008
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW3_SHIFT             3

/* AUD_FMM_DP_ESR0 :: MASK_SET :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW2_MASK              0x00000004
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW2_SHIFT             2

/* AUD_FMM_DP_ESR0 :: MASK_SET :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW1_MASK              0x00000002
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW1_SHIFT             1

/* AUD_FMM_DP_ESR0 :: MASK_SET :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW0_MASK              0x00000001
#define BCHP_AUD_FMM_DP_ESR0_MASK_SET_UNDERFLOW0_SHIFT             0

/***************************************************************************
 *MASK_CLEAR - Mask Clear Register
 ***************************************************************************/
/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: reserved0 [31:16] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_reserved0_MASK             0xffff0000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_reserved0_SHIFT            16

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: CLIENT3_RATE_ERR [15:15] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT3_RATE_ERR_MASK      0x00008000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT3_RATE_ERR_SHIFT     15

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: CLIENT2_RATE_ERR [14:14] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT2_RATE_ERR_MASK      0x00004000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT2_RATE_ERR_SHIFT     14

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: CLIENT1_RATE_ERR [13:13] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT1_RATE_ERR_MASK      0x00002000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT1_RATE_ERR_SHIFT     13

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: CLIENT0_RATE_ERR [12:12] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT0_RATE_ERR_MASK      0x00001000
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_CLIENT0_RATE_ERR_SHIFT     12

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: VOL_RAMP_DONE3 [11:11] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE3_MASK        0x00000800
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE3_SHIFT       11

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: VOL_RAMP_DONE2 [10:10] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE2_MASK        0x00000400
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE2_SHIFT       10

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: VOL_RAMP_DONE1 [09:09] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE1_MASK        0x00000200
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE1_SHIFT       9

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: VOL_RAMP_DONE0 [08:08] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE0_MASK        0x00000100
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_VOL_RAMP_DONE0_SHIFT       8

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: SCALE_RAMP_DONE3 [07:07] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE3_MASK      0x00000080
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE3_SHIFT     7

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: SCALE_RAMP_DONE2 [06:06] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE2_MASK      0x00000040
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE2_SHIFT     6

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: SCALE_RAMP_DONE1 [05:05] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE1_MASK      0x00000020
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE1_SHIFT     5

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: SCALE_RAMP_DONE0 [04:04] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE0_MASK      0x00000010
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_SCALE_RAMP_DONE0_SHIFT     4

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: UNDERFLOW3 [03:03] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW3_MASK            0x00000008
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW3_SHIFT           3

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: UNDERFLOW2 [02:02] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW2_MASK            0x00000004
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW2_SHIFT           2

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: UNDERFLOW1 [01:01] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW1_MASK            0x00000002
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW1_SHIFT           1

/* AUD_FMM_DP_ESR0 :: MASK_CLEAR :: UNDERFLOW0 [00:00] */
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW0_MASK            0x00000001
#define BCHP_AUD_FMM_DP_ESR0_MASK_CLEAR_UNDERFLOW0_SHIFT           0

#endif /* #ifndef BCHP_AUD_FMM_DP_ESR0_H__ */

/* End of File */
