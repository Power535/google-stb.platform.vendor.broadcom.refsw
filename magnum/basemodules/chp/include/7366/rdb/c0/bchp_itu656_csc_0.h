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
 * Date:           Generated on               Thu Feb 12 15:15:52 2015
 *                 Full Compile MD5 Checksum  ca339b82db08da0250a17ca09932699d
 *                     (minus title and desc)
 *                 MD5 Checksum               502556bfbdc2f4341f93db8b4326b3ab
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_ITU656_CSC_0_H__
#define BCHP_ITU656_CSC_0_H__

/***************************************************************************
 *ITU656_CSC_0 - ITU 656 Color Space Converter
 ***************************************************************************/
#define BCHP_ITU656_CSC_0_CSC_REV_ID             0x006a6600 /* [RO] Revision ID register */
#define BCHP_ITU656_CSC_0_CSC_MODE               0x006a6608 /* [RW] Color space converter mode register */
#define BCHP_ITU656_CSC_0_CSC_MIN_MAX            0x006a660c /* [RW] Color space converter min_max clamp register */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C01_C00      0x006a6610 /* [RW] Video Encoder Color Matrix coefficients c01 and c00 */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C03_C02      0x006a6614 /* [RW] Video Encoder Color Matrix coefficients c03 and c02 */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C11_C10      0x006a6618 /* [RW] Video Encoder Color Matrix coefficients c11 and c10 */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C13_C12      0x006a661c /* [RW] Video Encoder Color Matrix coefficients c13 and c12 */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C21_C20      0x006a6620 /* [RW] Video Encoder Color Matrix coefficients c21 and c20 */
#define BCHP_ITU656_CSC_0_CSC_COEFF_C23_C22      0x006a6624 /* [RW] Video Encoder Color Matrix coefficients c23 and c22 */
#define BCHP_ITU656_CSC_0_DITHER_CONTROL         0x006a6628 /* [RW] Color Space Converter Dither Control */
#define BCHP_ITU656_CSC_0_DITHER_LFSR            0x006a662c /* [RW] Color Space Converter Dither LFSR Control */
#define BCHP_ITU656_CSC_0_DITHER_LFSR_INIT       0x006a6630 /* [RW] Color Space Converter Dither LFSR Init value and control */

#endif /* #ifndef BCHP_ITU656_CSC_0_H__ */

/* End of File */
