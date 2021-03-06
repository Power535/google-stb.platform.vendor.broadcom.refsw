/***************************************************************************
 *     Copyright (c) 1999-2012, Broadcom Corporation
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
 * Date:           Generated on         Wed Jun 13 16:39:04 2012
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
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

#ifndef BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_H__
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_H__

/***************************************************************************
 *AUD_FMM_IOP_OUT_DAC_CTRL_0
 ***************************************************************************/
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0 0x008b0c00 /* Stream configuration */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR 0x008b0c10 /* DAC crossbar control */

/***************************************************************************
 *STREAM_CFG_0 - Stream configuration
 ***************************************************************************/
/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: ENA [31:31] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_ENA_MASK      0x80000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_ENA_SHIFT     31
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_ENA_DEFAULT   0x00000000

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: reserved0 [30:28] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_reserved0_MASK 0x70000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_reserved0_SHIFT 28

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: CHANNEL_GROUPING [27:24] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_CHANNEL_GROUPING_MASK 0x0f000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_CHANNEL_GROUPING_SHIFT 24
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_CHANNEL_GROUPING_DEFAULT 0x00000001

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: GROUP_ID [23:20] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_GROUP_ID_MASK 0x00f00000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_GROUP_ID_SHIFT 20
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_GROUP_ID_DEFAULT 0x00000000

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: STREAM_BIT_RESOLUTION [19:16] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_MASK 0x000f0000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_SHIFT 16
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_DEFAULT 0x00000008
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_16_Bit 0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_17_Bit 1
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_18_Bit 2
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_19_Bit 3
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_20_Bit 4
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_21_Bit 5
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_22_Bit 6
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_23_Bit 7
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_STREAM_BIT_RESOLUTION_Res_24_Bit 8

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: WAIT_FOR_VALID [15:15] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_WAIT_FOR_VALID_MASK 0x00008000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_WAIT_FOR_VALID_SHIFT 15
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_WAIT_FOR_VALID_DEFAULT 0x00000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_WAIT_FOR_VALID_Holdoff_request 1
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_WAIT_FOR_VALID_Keep_requesting 0

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: IGNORE_FIRST_UNDERFLOW [14:14] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_IGNORE_FIRST_UNDERFLOW_MASK 0x00004000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_IGNORE_FIRST_UNDERFLOW_SHIFT 14
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_IGNORE_FIRST_UNDERFLOW_DEFAULT 0x00000001
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_IGNORE_FIRST_UNDERFLOW_Ignore 1
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_IGNORE_FIRST_UNDERFLOW_Dont_ignore 0

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: INIT_SM [13:13] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INIT_SM_MASK  0x00002000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INIT_SM_SHIFT 13
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INIT_SM_DEFAULT 0x00000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INIT_SM_Init  1
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INIT_SM_Normal 0

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: INS_INVAL [12:12] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INS_INVAL_MASK 0x00001000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INS_INVAL_SHIFT 12
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INS_INVAL_DEFAULT 0x00000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INS_INVAL_Invalid 1
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_INS_INVAL_Valid 0

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: reserved1 [11:10] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_reserved1_MASK 0x00000c00
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_reserved1_SHIFT 10

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: STREAM_CFG_0 :: FCI_ID [09:00] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_FCI_ID_MASK   0x000003ff
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_FCI_ID_SHIFT  0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_STREAM_CFG_0_FCI_ID_DEFAULT 0x000003ff

/***************************************************************************
 *DAC_CROSSBAR - DAC crossbar control
 ***************************************************************************/
/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: DAC_CROSSBAR :: reserved0 [31:05] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_reserved0_MASK 0xffffffe0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_reserved0_SHIFT 5

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: DAC_CROSSBAR :: OUT_R [04:04] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_R_MASK    0x00000010
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_R_SHIFT   4
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_R_DEFAULT 0x00000001
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_R_In_l    0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_R_In_r    1

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: DAC_CROSSBAR :: reserved1 [03:01] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_reserved1_MASK 0x0000000e
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_reserved1_SHIFT 1

/* AUD_FMM_IOP_OUT_DAC_CTRL_0 :: DAC_CROSSBAR :: OUT_L [00:00] */
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_L_MASK    0x00000001
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_L_SHIFT   0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_L_DEFAULT 0x00000000
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_L_In_l    0
#define BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_DAC_CROSSBAR_OUT_L_In_r    1

#endif /* #ifndef BCHP_AUD_FMM_IOP_OUT_DAC_CTRL_0_H__ */

/* End of File */
