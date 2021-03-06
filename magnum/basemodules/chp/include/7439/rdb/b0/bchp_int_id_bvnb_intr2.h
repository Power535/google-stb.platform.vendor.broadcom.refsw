/****************************************************************************
 *     Copyright (c) 1999-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Mon Jul 28 13:31:43 2014
 *                 Full Compile MD5 Checksum  c5ff84748c7fa0d54c801cf0c03aeeca
 *                     (minus title and desc)
 *                 MD5 Checksum               62ec62fd8717a3b00aa7784ade95abce
 *
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     14541
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#include "bchp.h"
#include "bchp_bvnb_intr2.h"

#ifndef BCHP_INT_ID_BVNB_INTR2_H__
#define BCHP_INT_ID_BVNB_INTR2_H__

#define BCHP_INT_ID_CAP0_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP0_INTR_SHIFT)
#define BCHP_INT_ID_CAP1_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP1_INTR_SHIFT)
#define BCHP_INT_ID_CAP2_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP2_INTR_SHIFT)
#define BCHP_INT_ID_CAP3_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP3_INTR_SHIFT)
#define BCHP_INT_ID_CAP4_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP4_INTR_SHIFT)
#define BCHP_INT_ID_CAP5_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CAP5_INTR_SHIFT)
#define BCHP_INT_ID_CMP0_G0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP0_G0_INTR_SHIFT)
#define BCHP_INT_ID_CMP0_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP0_INTR_SHIFT)
#define BCHP_INT_ID_CMP0_V0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP0_V0_INTR_SHIFT)
#define BCHP_INT_ID_CMP0_V1_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP0_V1_INTR_SHIFT)
#define BCHP_INT_ID_CMP1_G0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP1_G0_INTR_SHIFT)
#define BCHP_INT_ID_CMP1_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP1_INTR_SHIFT)
#define BCHP_INT_ID_CMP1_V0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP1_V0_INTR_SHIFT)
#define BCHP_INT_ID_CMP1_V1_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP1_V1_INTR_SHIFT)
#define BCHP_INT_ID_CMP2_G0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP2_G0_INTR_SHIFT)
#define BCHP_INT_ID_CMP2_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP2_INTR_SHIFT)
#define BCHP_INT_ID_CMP2_V0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP2_V0_INTR_SHIFT)
#define BCHP_INT_ID_CMP3_G0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP3_G0_INTR_SHIFT)
#define BCHP_INT_ID_CMP3_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP3_INTR_SHIFT)
#define BCHP_INT_ID_CMP3_V0_INTR              BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_CMP3_V0_INTR_SHIFT)
#define BCHP_INT_ID_GFD0_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_GFD0_INTR_SHIFT)
#define BCHP_INT_ID_GFD1_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_GFD1_INTR_SHIFT)
#define BCHP_INT_ID_GFD2_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_GFD2_INTR_SHIFT)
#define BCHP_INT_ID_GFD3_INTR                 BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_GFD3_INTR_SHIFT)
#define BCHP_INT_ID_LAB_HISTO_DONE_INTR       BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_LAB_HISTO_DONE_INTR_SHIFT)
#define BCHP_INT_ID_LAB_MIN_MAX_DONE_INTR     BCHP_INT_ID_CREATE(BCHP_BVNB_INTR2_CPU_STATUS, BCHP_BVNB_INTR2_CPU_STATUS_LAB_MIN_MAX_DONE_INTR_SHIFT)

#endif /* #ifndef BCHP_INT_ID_BVNB_INTR2_H__ */

/* End of File */
