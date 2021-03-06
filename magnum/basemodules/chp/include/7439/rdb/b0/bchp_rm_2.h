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
 * Date:           Generated on               Mon Jul 28 13:28:41 2014
 *                 Full Compile MD5 Checksum  c5ff84748c7fa0d54c801cf0c03aeeca
 *                     (minus title and desc)
 *                 MD5 Checksum               62ec62fd8717a3b00aa7784ade95abce
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     14541
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_RM_2_H__
#define BCHP_RM_2_H__

/***************************************************************************
 *RM_2 - Rate Manger RM_2
 ***************************************************************************/
#define BCHP_RM_2_CONTROL                        0x006e4700 /* [RW] Rate Manager Controls */
#define BCHP_RM_2_RATE_RATIO                     0x006e4704 /* [RW] Rate Manager Output Rate Setting I */
#define BCHP_RM_2_SAMPLE_INC                     0x006e4708 /* [RW] Rate Manager Output Rate Setting II */
#define BCHP_RM_2_PHASE_INC                      0x006e470c /* [RW] Rate Manager NCO Phase Increment */
#define BCHP_RM_2_INTEGRATOR_HI                  0x006e4710 /* [WO] Loop Filter Integrator HI Value */
#define BCHP_RM_2_INTEGRATOR_LO                  0x006e4714 /* [WO] Loop Filter Integrator LO Value */
#define BCHP_RM_2_SKIP_REPEAT_CONTROL            0x006e4718 /* [RW] Time Base Skip or Repeat Control Register */
#define BCHP_RM_2_SKIP_REPEAT_GAP                0x006e471c /* [RW] Time Base Skip or Repeat Gap Count Register */
#define BCHP_RM_2_SKIP_REPEAT_NUMBER             0x006e4720 /* [RW] Time Base Skip or Repeat Count Register */
#define BCHP_RM_2_INTEGRATOR_HI_STATUS           0x006e4724 /* [RO] Loop Filter Integrator HI Status */
#define BCHP_RM_2_INTEGRATOR_LO_STATUS           0x006e4728 /* [RO] Loop Filter Integrator LO Status */
#define BCHP_RM_2_STATUS                         0x006e472c /* [RO] Rate Manager Status Register */
#define BCHP_RM_2_STATUS_CLEAR                   0x006e4730 /* [RW] Rate Manager Status Clear Register */

#endif /* #ifndef BCHP_RM_2_H__ */

/* End of File */
