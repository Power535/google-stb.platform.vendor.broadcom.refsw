/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
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
 * Date:           Generated on              Tue Oct 15 10:16:05 2013
 *                 Full Compile MD5 Checksum b60ac7dfc280cb4bf0d80e3267cd40cf
 *                   (minus title and desc)
 *                 MD5 Checksum              3f170c7b9754bf2f738d4d5d54ee17b6
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#ifndef BCHP_DVI_FC_0_H__
#define BCHP_DVI_FC_0_H__

/***************************************************************************
 *DVI_FC_0 - DVI Frontend Format Conversion 0
 ***************************************************************************/
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL        0x006e5a00 /* FORMAT CONVERSION CONTROL REGISTER */
#define BCHP_DVI_FC_0_FC_REV_ID                  0x006e5a04 /* Revision ID register */

/***************************************************************************
 *FORMAT_CONV_CONTROL - FORMAT CONVERSION CONTROL REGISTER
 ***************************************************************************/
/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: reserved0 [31:25] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_reserved0_MASK           0xfe000000
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_reserved0_SHIFT          25

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: HTOTAL_SIZE [24:13] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_HTOTAL_SIZE_MASK         0x01ffe000
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_HTOTAL_SIZE_SHIFT        13
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_HTOTAL_SIZE_DEFAULT      0x00000898

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: COEFF_1 [12:09] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_1_MASK             0x00001e00
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_1_SHIFT            9
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_1_DEFAULT          0x00000004

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: COEFF_0 [08:05] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_0_MASK             0x000001e0
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_0_SHIFT            5
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_COEFF_0_DEFAULT          0x00000004

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: FORMAT [04:03] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_MASK              0x00000018
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_SHIFT             3
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_DEFAULT           0x00000000
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_FORMAT_444        0
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_FORMAT_422        1
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FORMAT_FORMAT_420        2

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: FILTER_MODE [02:01] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_MASK         0x00000006
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_SHIFT        1
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_DEFAULT      0x00000003
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_FILTER1      0
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_FILTER2      1
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_FILTER3      2
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_FILTER_MODE_BYPASS       3

/* DVI_FC_0 :: FORMAT_CONV_CONTROL :: DERING_EN [00:00] */
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_DERING_EN_MASK           0x00000001
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_DERING_EN_SHIFT          0
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_DERING_EN_DEFAULT        0x00000000
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_DERING_EN_OFF            0
#define BCHP_DVI_FC_0_FORMAT_CONV_CONTROL_DERING_EN_ON             1

/***************************************************************************
 *FC_REV_ID - Revision ID register
 ***************************************************************************/
/* DVI_FC_0 :: FC_REV_ID :: reserved0 [31:16] */
#define BCHP_DVI_FC_0_FC_REV_ID_reserved0_MASK                     0xffff0000
#define BCHP_DVI_FC_0_FC_REV_ID_reserved0_SHIFT                    16

/* DVI_FC_0 :: FC_REV_ID :: REVISION_ID [15:00] */
#define BCHP_DVI_FC_0_FC_REV_ID_REVISION_ID_MASK                   0x0000ffff
#define BCHP_DVI_FC_0_FC_REV_ID_REVISION_ID_SHIFT                  0
#define BCHP_DVI_FC_0_FC_REV_ID_REVISION_ID_DEFAULT                0x00001000

#endif /* #ifndef BCHP_DVI_FC_0_H__ */

/* End of File */
