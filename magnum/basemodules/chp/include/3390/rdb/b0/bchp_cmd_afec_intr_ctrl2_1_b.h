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
 * Date:           Generated on               Thu Oct 29 13:14:11 2015
 *                 Full Compile MD5 Checksum  e7a8666924d6f16d6c48f8d4180ae83b
 *                     (minus title and desc)
 *                 MD5 Checksum               60098f94fd56f39bea342d634b9c6b61
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     414
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_CMD_AFEC_INTR_CTRL2_1_B_H__
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_H__

/***************************************************************************
 *CMD_AFEC_INTR_CTRL2_1_B - CMD AFEC L2 Interrupt Control 1 Registers
 ***************************************************************************/
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_STATUS  0x022d4400 /* [RO] CPU interrupt Status Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_SET     0x022d4404 /* [WO] CPU interrupt Set Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_CLEAR   0x022d4408 /* [WO] CPU interrupt Clear Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_MASK_STATUS 0x022d440c /* [RO] CPU interrupt Mask Status Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_MASK_SET 0x022d4410 /* [WO] CPU interrupt Mask Set Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_CPU_MASK_CLEAR 0x022d4414 /* [WO] CPU interrupt Mask Clear Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_STATUS  0x022d4418 /* [RO] PCI interrupt Status Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_SET     0x022d441c /* [WO] PCI interrupt Set Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_CLEAR   0x022d4420 /* [WO] PCI interrupt Clear Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_MASK_STATUS 0x022d4424 /* [RO] PCI interrupt Mask Status Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_MASK_SET 0x022d4428 /* [WO] PCI interrupt Mask Set Register */
#define BCHP_CMD_AFEC_INTR_CTRL2_1_B_PCI_MASK_CLEAR 0x022d442c /* [WO] PCI interrupt Mask Clear Register */

#endif /* #ifndef BCHP_CMD_AFEC_INTR_CTRL2_1_B_H__ */

/* End of File */
