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
 * Date:           Generated on               Tue Jun 27 10:52:38 2017
 *                 Full Compile MD5 Checksum  de13a1e8011803b5a40ab14e4d71d071
 *                     (minus title and desc)
 *                 MD5 Checksum               b694fcab41780597392ed5a8f558ad3e
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1570
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   LOCAL home/pntruong/sbin/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_SECAM_0_H__
#define BCHP_SECAM_0_H__

/***************************************************************************
 *SECAM_0 - Secam Signal Processor SECAM_0
 ***************************************************************************/
#define BCHP_SECAM_0_SECAM_REV_ID                0x206e2200 /* [RO][32] REVISION ID REGISTER */
#define BCHP_SECAM_0_FM_CONTROL                  0x206e2204 /* [RW][32] FM Control Register */
#define BCHP_SECAM_0_FM_FMAMP                    0x206e2208 /* [RW][32] FM AMP Register */
#define BCHP_SECAM_0_FM_SHAPER                   0x206e220c /* [RW][32] FM Shaper Register */
#define BCHP_SECAM_0_FM_SC_FREQ0                 0x206e2210 /* [RW][32] FM Base Sub-carrier Max and Min Frequency */
#define BCHP_SECAM_0_FM_SC_FREQ1                 0x206e2214 /* [RW][32] FM Base Sub-carrier Frequency Diff */

/***************************************************************************
 *SECAM_REV_ID - REVISION ID REGISTER
 ***************************************************************************/
/* SECAM_0 :: SECAM_REV_ID :: reserved0 [31:16] */
#define BCHP_SECAM_0_SECAM_REV_ID_reserved0_MASK                   0xffff0000
#define BCHP_SECAM_0_SECAM_REV_ID_reserved0_SHIFT                  16

/* SECAM_0 :: SECAM_REV_ID :: REVISION_ID [15:00] */
#define BCHP_SECAM_0_SECAM_REV_ID_REVISION_ID_MASK                 0x0000ffff
#define BCHP_SECAM_0_SECAM_REV_ID_REVISION_ID_SHIFT                0
#define BCHP_SECAM_0_SECAM_REV_ID_REVISION_ID_DEFAULT              0x00004000

/***************************************************************************
 *FM_CONTROL - FM Control Register
 ***************************************************************************/
/* SECAM_0 :: FM_CONTROL :: reserved_for_eco0 [31:19] */
#define BCHP_SECAM_0_FM_CONTROL_reserved_for_eco0_MASK             0xfff80000
#define BCHP_SECAM_0_FM_CONTROL_reserved_for_eco0_SHIFT            19
#define BCHP_SECAM_0_FM_CONTROL_reserved_for_eco0_DEFAULT          0x00000000

/* SECAM_0 :: FM_CONTROL :: FINE_LUMA_DELAY [18:17] */
#define BCHP_SECAM_0_FM_CONTROL_FINE_LUMA_DELAY_MASK               0x00060000
#define BCHP_SECAM_0_FM_CONTROL_FINE_LUMA_DELAY_SHIFT              17
#define BCHP_SECAM_0_FM_CONTROL_FINE_LUMA_DELAY_DEFAULT            0x00000000

/* SECAM_0 :: FM_CONTROL :: reserved1 [16:15] */
#define BCHP_SECAM_0_FM_CONTROL_reserved1_MASK                     0x00018000
#define BCHP_SECAM_0_FM_CONTROL_reserved1_SHIFT                    15

/* SECAM_0 :: FM_CONTROL :: GROSS_LUMA_DELAY [14:10] */
#define BCHP_SECAM_0_FM_CONTROL_GROSS_LUMA_DELAY_MASK              0x00007c00
#define BCHP_SECAM_0_FM_CONTROL_GROSS_LUMA_DELAY_SHIFT             10
#define BCHP_SECAM_0_FM_CONTROL_GROSS_LUMA_DELAY_DEFAULT           0x00000000

/* SECAM_0 :: FM_CONTROL :: FINE_SC_DELAY [09:08] */
#define BCHP_SECAM_0_FM_CONTROL_FINE_SC_DELAY_MASK                 0x00000300
#define BCHP_SECAM_0_FM_CONTROL_FINE_SC_DELAY_SHIFT                8
#define BCHP_SECAM_0_FM_CONTROL_FINE_SC_DELAY_DEFAULT              0x00000000

/* SECAM_0 :: FM_CONTROL :: reserved2 [07:06] */
#define BCHP_SECAM_0_FM_CONTROL_reserved2_MASK                     0x000000c0
#define BCHP_SECAM_0_FM_CONTROL_reserved2_SHIFT                    6

/* SECAM_0 :: FM_CONTROL :: GROSS_SC_DELAY [05:01] */
#define BCHP_SECAM_0_FM_CONTROL_GROSS_SC_DELAY_MASK                0x0000003e
#define BCHP_SECAM_0_FM_CONTROL_GROSS_SC_DELAY_SHIFT               1
#define BCHP_SECAM_0_FM_CONTROL_GROSS_SC_DELAY_DEFAULT             0x00000000

/* SECAM_0 :: FM_CONTROL :: ENABLE [00:00] */
#define BCHP_SECAM_0_FM_CONTROL_ENABLE_MASK                        0x00000001
#define BCHP_SECAM_0_FM_CONTROL_ENABLE_SHIFT                       0
#define BCHP_SECAM_0_FM_CONTROL_ENABLE_DEFAULT                     0x00000000
#define BCHP_SECAM_0_FM_CONTROL_ENABLE_ON                          1
#define BCHP_SECAM_0_FM_CONTROL_ENABLE_OFF                         0

/***************************************************************************
 *FM_FMAMP - FM AMP Register
 ***************************************************************************/
/* SECAM_0 :: FM_FMAMP :: reserved0 [31:30] */
#define BCHP_SECAM_0_FM_FMAMP_reserved0_MASK                       0xc0000000
#define BCHP_SECAM_0_FM_FMAMP_reserved0_SHIFT                      30

/* SECAM_0 :: FM_FMAMP :: LOWER_LIMIT [29:22] */
#define BCHP_SECAM_0_FM_FMAMP_LOWER_LIMIT_MASK                     0x3fc00000
#define BCHP_SECAM_0_FM_FMAMP_LOWER_LIMIT_SHIFT                    22
#define BCHP_SECAM_0_FM_FMAMP_LOWER_LIMIT_DEFAULT                  0x00000040

/* SECAM_0 :: FM_FMAMP :: reserved1 [21:20] */
#define BCHP_SECAM_0_FM_FMAMP_reserved1_MASK                       0x00300000
#define BCHP_SECAM_0_FM_FMAMP_reserved1_SHIFT                      20

/* SECAM_0 :: FM_FMAMP :: UPPER_LIMIT [19:12] */
#define BCHP_SECAM_0_FM_FMAMP_UPPER_LIMIT_MASK                     0x000ff000
#define BCHP_SECAM_0_FM_FMAMP_UPPER_LIMIT_SHIFT                    12
#define BCHP_SECAM_0_FM_FMAMP_UPPER_LIMIT_DEFAULT                  0x000000c0

/* SECAM_0 :: FM_FMAMP :: SLOPE_ADJUST [11:09] */
#define BCHP_SECAM_0_FM_FMAMP_SLOPE_ADJUST_MASK                    0x00000e00
#define BCHP_SECAM_0_FM_FMAMP_SLOPE_ADJUST_SHIFT                   9
#define BCHP_SECAM_0_FM_FMAMP_SLOPE_ADJUST_DEFAULT                 0x00000003

/* SECAM_0 :: FM_FMAMP :: reserved2 [08:07] */
#define BCHP_SECAM_0_FM_FMAMP_reserved2_MASK                       0x00000180
#define BCHP_SECAM_0_FM_FMAMP_reserved2_SHIFT                      7

/* SECAM_0 :: FM_FMAMP :: SCALE [06:00] */
#define BCHP_SECAM_0_FM_FMAMP_SCALE_MASK                           0x0000007f
#define BCHP_SECAM_0_FM_FMAMP_SCALE_SHIFT                          0
#define BCHP_SECAM_0_FM_FMAMP_SCALE_DEFAULT                        0x00000040

/***************************************************************************
 *FM_SHAPER - FM Shaper Register
 ***************************************************************************/
/* SECAM_0 :: FM_SHAPER :: reserved0 [31:16] */
#define BCHP_SECAM_0_FM_SHAPER_reserved0_MASK                      0xffff0000
#define BCHP_SECAM_0_FM_SHAPER_reserved0_SHIFT                     16

/* SECAM_0 :: FM_SHAPER :: FMAMP_COEFF1 [15:08] */
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF1_MASK                   0x0000ff00
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF1_SHIFT                  8
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF1_DEFAULT                0x00000040

/* SECAM_0 :: FM_SHAPER :: FMAMP_COEFF0 [07:00] */
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF0_MASK                   0x000000ff
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF0_SHIFT                  0
#define BCHP_SECAM_0_FM_SHAPER_FMAMP_COEFF0_DEFAULT                0x00000020

/***************************************************************************
 *FM_SC_FREQ0 - FM Base Sub-carrier Max and Min Frequency
 ***************************************************************************/
/* SECAM_0 :: FM_SC_FREQ0 :: reserved0 [31:22] */
#define BCHP_SECAM_0_FM_SC_FREQ0_reserved0_MASK                    0xffc00000
#define BCHP_SECAM_0_FM_SC_FREQ0_reserved0_SHIFT                   22

/* SECAM_0 :: FM_SC_FREQ0 :: MIN_FREQ_CLAMP [21:11] */
#define BCHP_SECAM_0_FM_SC_FREQ0_MIN_FREQ_CLAMP_MASK               0x003ff800
#define BCHP_SECAM_0_FM_SC_FREQ0_MIN_FREQ_CLAMP_SHIFT              11
#define BCHP_SECAM_0_FM_SC_FREQ0_MIN_FREQ_CLAMP_DEFAULT            0x0000064c

/* SECAM_0 :: FM_SC_FREQ0 :: MAX_FREQ_CLAMP [10:00] */
#define BCHP_SECAM_0_FM_SC_FREQ0_MAX_FREQ_CLAMP_MASK               0x000007ff
#define BCHP_SECAM_0_FM_SC_FREQ0_MAX_FREQ_CLAMP_SHIFT              0
#define BCHP_SECAM_0_FM_SC_FREQ0_MAX_FREQ_CLAMP_DEFAULT            0x00000266

/***************************************************************************
 *FM_SC_FREQ1 - FM Base Sub-carrier Frequency Diff
 ***************************************************************************/
/* SECAM_0 :: FM_SC_FREQ1 :: reserved0 [31:10] */
#define BCHP_SECAM_0_FM_SC_FREQ1_reserved0_MASK                    0xfffffc00
#define BCHP_SECAM_0_FM_SC_FREQ1_reserved0_SHIFT                   10

/* SECAM_0 :: FM_SC_FREQ1 :: FREQ_DIFF [09:00] */
#define BCHP_SECAM_0_FM_SC_FREQ1_FREQ_DIFF_MASK                    0x000003ff
#define BCHP_SECAM_0_FM_SC_FREQ1_FREQ_DIFF_SHIFT                   0
#define BCHP_SECAM_0_FM_SC_FREQ1_FREQ_DIFF_DEFAULT                 0x000000bd

#endif /* #ifndef BCHP_SECAM_0_H__ */

/* End of File */
