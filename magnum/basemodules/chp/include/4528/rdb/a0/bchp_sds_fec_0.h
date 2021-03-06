/***************************************************************************
 *     Copyright (c) 1999-2011, Broadcom Corporation
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
 * Date:           Generated on         Wed Jan 12 18:41:36 2011
 *                 MD5 Checksum         6e6727f6c827233acdd395c9a9032c98
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_SDS_FEC_0_H__
#define BCHP_SDS_FEC_0_H__

/***************************************************************************
 *SDS_FEC_0 - SDS FEC Register Set 0
 ***************************************************************************/
#define BCHP_SDS_FEC_0_FECTL                     0x000b0440 /* Reed-Solomon Control Register */
#define BCHP_SDS_FEC_0_FSYN                      0x000b0444 /* RS sync acquisition/retention total and bad headers */
#define BCHP_SDS_FEC_0_FRS                       0x000b0448 /* RS sync retention total and bad blocks and sync inverse max */
#define BCHP_SDS_FEC_0_FMOD                      0x000b044c /* RS Mode */
#define BCHP_SDS_FEC_0_FERR                      0x000b0450 /* RS correctable and uncorrectable blocks */
#define BCHP_SDS_FEC_0_FRSV                      0x000b0454 /* RS Digicipher II packet header reserved words */

/***************************************************************************
 *FECTL - Reed-Solomon Control Register
 ***************************************************************************/
/* SDS_FEC_0 :: FECTL :: reserved0 [31:06] */
#define BCHP_SDS_FEC_0_FECTL_reserved0_MASK                        0xffffffc0
#define BCHP_SDS_FEC_0_FECTL_reserved0_SHIFT                       6

/* SDS_FEC_0 :: FECTL :: Rsvrdy [05:05] */
#define BCHP_SDS_FEC_0_FECTL_Rsvrdy_MASK                           0x00000020
#define BCHP_SDS_FEC_0_FECTL_Rsvrdy_SHIFT                          5

/* SDS_FEC_0 :: FECTL :: reserved_for_eco1 [04:04] */
#define BCHP_SDS_FEC_0_FECTL_reserved_for_eco1_MASK                0x00000010
#define BCHP_SDS_FEC_0_FECTL_reserved_for_eco1_SHIFT               4

/* SDS_FEC_0 :: FECTL :: CorDis [03:03] */
#define BCHP_SDS_FEC_0_FECTL_CorDis_MASK                           0x00000008
#define BCHP_SDS_FEC_0_FECTL_CorDis_SHIFT                          3

/* SDS_FEC_0 :: FECTL :: Repack [02:02] */
#define BCHP_SDS_FEC_0_FECTL_Repack_MASK                           0x00000004
#define BCHP_SDS_FEC_0_FECTL_Repack_SHIFT                          2

/* SDS_FEC_0 :: FECTL :: Side [01:01] */
#define BCHP_SDS_FEC_0_FECTL_Side_MASK                             0x00000002
#define BCHP_SDS_FEC_0_FECTL_Side_SHIFT                            1

/* SDS_FEC_0 :: FECTL :: Reset [00:00] */
#define BCHP_SDS_FEC_0_FECTL_Reset_MASK                            0x00000001
#define BCHP_SDS_FEC_0_FECTL_Reset_SHIFT                           0

/***************************************************************************
 *FSYN - RS sync acquisition/retention total and bad headers
 ***************************************************************************/
/* SDS_FEC_0 :: FSYN :: Act [31:24] */
#define BCHP_SDS_FEC_0_FSYN_Act_MASK                               0xff000000
#define BCHP_SDS_FEC_0_FSYN_Act_SHIFT                              24

/* SDS_FEC_0 :: FSYN :: Acb [23:16] */
#define BCHP_SDS_FEC_0_FSYN_Acb_MASK                               0x00ff0000
#define BCHP_SDS_FEC_0_FSYN_Acb_SHIFT                              16

/* SDS_FEC_0 :: FSYN :: Rtt [15:08] */
#define BCHP_SDS_FEC_0_FSYN_Rtt_MASK                               0x0000ff00
#define BCHP_SDS_FEC_0_FSYN_Rtt_SHIFT                              8

/* SDS_FEC_0 :: FSYN :: Rtb [07:00] */
#define BCHP_SDS_FEC_0_FSYN_Rtb_MASK                               0x000000ff
#define BCHP_SDS_FEC_0_FSYN_Rtb_SHIFT                              0

/***************************************************************************
 *FRS - RS sync retention total and bad blocks and sync inverse max
 ***************************************************************************/
/* SDS_FEC_0 :: FRS :: rss [31:24] */
#define BCHP_SDS_FEC_0_FRS_rss_MASK                                0xff000000
#define BCHP_SDS_FEC_0_FRS_rss_SHIFT                               24

/* SDS_FEC_0 :: FRS :: im [23:16] */
#define BCHP_SDS_FEC_0_FRS_im_MASK                                 0x00ff0000
#define BCHP_SDS_FEC_0_FRS_im_SHIFT                                16

/* SDS_FEC_0 :: FRS :: rst [15:08] */
#define BCHP_SDS_FEC_0_FRS_rst_MASK                                0x0000ff00
#define BCHP_SDS_FEC_0_FRS_rst_SHIFT                               8

/* SDS_FEC_0 :: FRS :: rsb [07:00] */
#define BCHP_SDS_FEC_0_FRS_rsb_MASK                                0x000000ff
#define BCHP_SDS_FEC_0_FRS_rsb_SHIFT                               0

/***************************************************************************
 *FMOD - RS Mode
 ***************************************************************************/
/* SDS_FEC_0 :: FMOD :: reserved_for_eco0 [31:16] */
#define BCHP_SDS_FEC_0_FMOD_reserved_for_eco0_MASK                 0xffff0000
#define BCHP_SDS_FEC_0_FMOD_reserved_for_eco0_SHIFT                16

/* SDS_FEC_0 :: FMOD :: rs_disable [15:15] */
#define BCHP_SDS_FEC_0_FMOD_rs_disable_MASK                        0x00008000
#define BCHP_SDS_FEC_0_FMOD_rs_disable_SHIFT                       15

/* SDS_FEC_0 :: FMOD :: intlv_depth [14:12] */
#define BCHP_SDS_FEC_0_FMOD_intlv_depth_MASK                       0x00007000
#define BCHP_SDS_FEC_0_FMOD_intlv_depth_SHIFT                      12

/* SDS_FEC_0 :: FMOD :: rs_mode [11:10] */
#define BCHP_SDS_FEC_0_FMOD_rs_mode_MASK                           0x00000c00
#define BCHP_SDS_FEC_0_FMOD_rs_mode_SHIFT                          10

/* SDS_FEC_0 :: FMOD :: rs_tei [09:09] */
#define BCHP_SDS_FEC_0_FMOD_rs_tei_MASK                            0x00000200
#define BCHP_SDS_FEC_0_FMOD_rs_tei_SHIFT                           9

/* SDS_FEC_0 :: FMOD :: reserved_for_eco1 [08:05] */
#define BCHP_SDS_FEC_0_FMOD_reserved_for_eco1_MASK                 0x000001e0
#define BCHP_SDS_FEC_0_FMOD_reserved_for_eco1_SHIFT                5

/* SDS_FEC_0 :: FMOD :: rs_stuff [04:00] */
#define BCHP_SDS_FEC_0_FMOD_rs_stuff_MASK                          0x0000001f
#define BCHP_SDS_FEC_0_FMOD_rs_stuff_SHIFT                         0

/***************************************************************************
 *FERR - RS correctable and uncorrectable blocks
 ***************************************************************************/
/* SDS_FEC_0 :: FERR :: csym [31:16] */
#define BCHP_SDS_FEC_0_FERR_csym_MASK                              0xffff0000
#define BCHP_SDS_FEC_0_FERR_csym_SHIFT                             16

/* SDS_FEC_0 :: FERR :: bbclk [15:00] */
#define BCHP_SDS_FEC_0_FERR_bbclk_MASK                             0x0000ffff
#define BCHP_SDS_FEC_0_FERR_bbclk_SHIFT                            0

/***************************************************************************
 *FRSV - RS Digicipher II packet header reserved words
 ***************************************************************************/
/* SDS_FEC_0 :: FRSV :: reserved0 [31:16] */
#define BCHP_SDS_FEC_0_FRSV_reserved0_MASK                         0xffff0000
#define BCHP_SDS_FEC_0_FRSV_reserved0_SHIFT                        16

/* SDS_FEC_0 :: FRSV :: rsv [15:00] */
#define BCHP_SDS_FEC_0_FRSV_rsv_MASK                               0x0000ffff
#define BCHP_SDS_FEC_0_FRSV_rsv_SHIFT                              0

#endif /* #ifndef BCHP_SDS_FEC_0_H__ */

/* End of File */
