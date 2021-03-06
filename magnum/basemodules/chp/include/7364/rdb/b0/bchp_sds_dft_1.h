/****************************************************************************
 *     Copyright (c) 1999-2015, Broadcom Corporation
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
 * Date:           Generated on               Wed Apr  1 11:23:11 2015
 *                 Full Compile MD5 Checksum  267f8e92d9b43928c0a06f1ab29c511c
 *                     (minus title and desc)
 *                 MD5 Checksum               0548f7f0a8e20364fd383a7aa29c0b86
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15956
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_SDS_DFT_1_H__
#define BCHP_SDS_DFT_1_H__

/***************************************************************************
 *SDS_DFT_1 - SDS DFT Register Set
 ***************************************************************************/
#define BCHP_SDS_DFT_1_CTRL0                     0x01241580 /* [RW] DFT Control 0 */
#define BCHP_SDS_DFT_1_CTRL1                     0x01241584 /* [RW] DFT Control 1 */
#define BCHP_SDS_DFT_1_STATUS                    0x01241588 /* [RO] DFT Status */
#define BCHP_SDS_DFT_1_RANGE_START               0x0124158c /* [RW] DFT Starting Bin */
#define BCHP_SDS_DFT_1_RANGE_END                 0x01241590 /* [RW] DFT Ending Bin */
#define BCHP_SDS_DFT_1_DDFS_FCW                  0x01241594 /* [RW] DFT DDFS Frequency Control Word */
#define BCHP_SDS_DFT_1_PEAK_POW                  0x01241598 /* [RO] DFT Peak Power */
#define BCHP_SDS_DFT_1_PEAK_BIN                  0x0124159c /* [RO] DFT Peak Bin */
#define BCHP_SDS_DFT_1_TOTAL_POW                 0x012415a0 /* [RO] DFT Total Power */
#define BCHP_SDS_DFT_1_MEM_RADDR                 0x012415a4 /* [RW] DFT Bin Memory Read Address */
#define BCHP_SDS_DFT_1_MEM_RDATA                 0x012415a8 /* [RO] DFT Bin Memory Read Data */

#endif /* #ifndef BCHP_SDS_DFT_1_H__ */

/* End of File */
