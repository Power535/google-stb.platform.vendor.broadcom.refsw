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
 * Date:           Generated on               Tue Jun 27 10:52:39 2017
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

#ifndef BCHP_RVC_0_H__
#define BCHP_RVC_0_H__

/***************************************************************************
 *RVC_0
 ***************************************************************************/
#define BCHP_RVC_0_CTL                           0x20026d00 /* [CFG][32] Control register */
#define BCHP_RVC_0_PUT                           0x20026d08 /* [RO][64] Put pointer */
#define BCHP_RVC_0_GET                           0x20026d10 /* [CFG][64] Get pointer */
#define BCHP_RVC_0_BASE                          0x20026d18 /* [RW][64] Circular buffer base address */
#define BCHP_RVC_0_END                           0x20026d20 /* [RW][64] Circular buffer end address */
#define BCHP_RVC_0_RANGE1_START                  0x20026d28 /* [CFG][32] Address range start */
#define BCHP_RVC_0_RANGE1_END                    0x20026d2c /* [CFG][32] Address range end */
#define BCHP_RVC_0_RANGE2_START                  0x20026d30 /* [CFG][32] Address range start */
#define BCHP_RVC_0_RANGE2_END                    0x20026d34 /* [CFG][32] Address range end */

/***************************************************************************
 *CTL - Control register
 ***************************************************************************/
/* RVC_0 :: CTL :: reserved0 [31:04] */
#define BCHP_RVC_0_CTL_reserved0_MASK                              0xfffffff0
#define BCHP_RVC_0_CTL_reserved0_SHIFT                             4

/* RVC_0 :: CTL :: Capture_select [03:01] */
#define BCHP_RVC_0_CTL_Capture_select_MASK                         0x0000000e
#define BCHP_RVC_0_CTL_Capture_select_SHIFT                        1
#define BCHP_RVC_0_CTL_Capture_select_DEFAULT                      0x00000000

/* RVC_0 :: CTL :: Ena [00:00] */
#define BCHP_RVC_0_CTL_Ena_MASK                                    0x00000001
#define BCHP_RVC_0_CTL_Ena_SHIFT                                   0
#define BCHP_RVC_0_CTL_Ena_DEFAULT                                 0x00000000

/***************************************************************************
 *PUT - Put pointer
 ***************************************************************************/
/* RVC_0 :: PUT :: reserved0 [63:40] */
#define BCHP_RVC_0_PUT_reserved0_MASK                              BCHP_UINT64_C(0xffffff00, 0x00000000)
#define BCHP_RVC_0_PUT_reserved0_SHIFT                             40

/* RVC_0 :: PUT :: Put_Ptr [39:00] */
#define BCHP_RVC_0_PUT_Put_Ptr_MASK                                BCHP_UINT64_C(0x000000ff, 0xffffffff)
#define BCHP_RVC_0_PUT_Put_Ptr_SHIFT                               0
#define BCHP_RVC_0_PUT_Put_Ptr_DEFAULT                             50331648

/***************************************************************************
 *GET - Get pointer
 ***************************************************************************/
/* RVC_0 :: GET :: reserved0 [63:40] */
#define BCHP_RVC_0_GET_reserved0_MASK                              BCHP_UINT64_C(0xffffff00, 0x00000000)
#define BCHP_RVC_0_GET_reserved0_SHIFT                             40

/* RVC_0 :: GET :: Get_Ptr [39:05] */
#define BCHP_RVC_0_GET_Get_Ptr_MASK                                BCHP_UINT64_C(0x000000ff, 0xffffffe0)
#define BCHP_RVC_0_GET_Get_Ptr_SHIFT                               5
#define BCHP_RVC_0_GET_Get_Ptr_DEFAULT                             0

/* RVC_0 :: GET :: reserved1 [04:00] */
#define BCHP_RVC_0_GET_reserved1_MASK                              BCHP_UINT64_C(0x00000000, 0x0000001f)
#define BCHP_RVC_0_GET_reserved1_SHIFT                             0

/***************************************************************************
 *BASE - Circular buffer base address
 ***************************************************************************/
/* RVC_0 :: BASE :: reserved0 [63:40] */
#define BCHP_RVC_0_BASE_reserved0_MASK                             BCHP_UINT64_C(0xffffff00, 0x00000000)
#define BCHP_RVC_0_BASE_reserved0_SHIFT                            40

/* RVC_0 :: BASE :: Base_Addr [39:20] */
#define BCHP_RVC_0_BASE_Base_Addr_MASK                             BCHP_UINT64_C(0x000000ff, 0xfff00000)
#define BCHP_RVC_0_BASE_Base_Addr_SHIFT                            20
#define BCHP_RVC_0_BASE_Base_Addr_DEFAULT                          48

/* RVC_0 :: BASE :: Base_Addr_lsb [19:00] */
#define BCHP_RVC_0_BASE_Base_Addr_lsb_MASK                         BCHP_UINT64_C(0x00000000, 0x000fffff)
#define BCHP_RVC_0_BASE_Base_Addr_lsb_SHIFT                        0
#define BCHP_RVC_0_BASE_Base_Addr_lsb_DEFAULT                      0

/***************************************************************************
 *END - Circular buffer end address
 ***************************************************************************/
/* RVC_0 :: END :: reserved0 [63:40] */
#define BCHP_RVC_0_END_reserved0_MASK                              BCHP_UINT64_C(0xffffff00, 0x00000000)
#define BCHP_RVC_0_END_reserved0_SHIFT                             40

/* RVC_0 :: END :: End_Addr [39:20] */
#define BCHP_RVC_0_END_End_Addr_MASK                               BCHP_UINT64_C(0x000000ff, 0xfff00000)
#define BCHP_RVC_0_END_End_Addr_SHIFT                              20
#define BCHP_RVC_0_END_End_Addr_DEFAULT                            63

/* RVC_0 :: END :: End_Addr_lsb [19:00] */
#define BCHP_RVC_0_END_End_Addr_lsb_MASK                           BCHP_UINT64_C(0x00000000, 0x000fffff)
#define BCHP_RVC_0_END_End_Addr_lsb_SHIFT                          0
#define BCHP_RVC_0_END_End_Addr_lsb_DEFAULT                        1048575

/***************************************************************************
 *RANGE1_START - Address range start
 ***************************************************************************/
/* RVC_0 :: RANGE1_START :: capture_start_addr [31:00] */
#define BCHP_RVC_0_RANGE1_START_capture_start_addr_MASK            0xffffffff
#define BCHP_RVC_0_RANGE1_START_capture_start_addr_SHIFT           0
#define BCHP_RVC_0_RANGE1_START_capture_start_addr_DEFAULT         0x00000000

/***************************************************************************
 *RANGE1_END - Address range end
 ***************************************************************************/
/* RVC_0 :: RANGE1_END :: capture_end_addr [31:00] */
#define BCHP_RVC_0_RANGE1_END_capture_end_addr_MASK                0xffffffff
#define BCHP_RVC_0_RANGE1_END_capture_end_addr_SHIFT               0
#define BCHP_RVC_0_RANGE1_END_capture_end_addr_DEFAULT             0x00000000

/***************************************************************************
 *RANGE2_START - Address range start
 ***************************************************************************/
/* RVC_0 :: RANGE2_START :: capture_start_addr [31:00] */
#define BCHP_RVC_0_RANGE2_START_capture_start_addr_MASK            0xffffffff
#define BCHP_RVC_0_RANGE2_START_capture_start_addr_SHIFT           0
#define BCHP_RVC_0_RANGE2_START_capture_start_addr_DEFAULT         0x00000000

/***************************************************************************
 *RANGE2_END - Address range end
 ***************************************************************************/
/* RVC_0 :: RANGE2_END :: capture_end_addr [31:00] */
#define BCHP_RVC_0_RANGE2_END_capture_end_addr_MASK                0xffffffff
#define BCHP_RVC_0_RANGE2_END_capture_end_addr_SHIFT               0
#define BCHP_RVC_0_RANGE2_END_capture_end_addr_DEFAULT             0x00000000

#endif /* #ifndef BCHP_RVC_0_H__ */

/* End of File */
