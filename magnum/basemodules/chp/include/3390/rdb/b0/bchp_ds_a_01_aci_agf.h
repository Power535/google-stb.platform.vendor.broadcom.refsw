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
 * Date:           Generated on               Thu Oct 29 13:14:17 2015
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

#ifndef BCHP_DS_A_01_ACI_AGF_H__
#define BCHP_DS_A_01_ACI_AGF_H__

/***************************************************************************
 *DS_A_01_ACI_AGF - Downstream Channel Isolation Processor Configuration Registers
 ***************************************************************************/
#define BCHP_DS_A_01_ACI_AGF_ACI                 0x02222200 /* [RW] CIP ACI control register */
#define BCHP_DS_A_01_ACI_AGF_AGF                 0x02222204 /* [RW] Digital AGC(Fine) Control Register */
#define BCHP_DS_A_01_ACI_AGF_AGFI                0x02222208 /* [RW] Digital AGC(Fine) Integrator Value */
#define BCHP_DS_A_01_ACI_AGF_AGFLI               0x0222220c /* [RW] Digital AGC(Fine) Leaky Integrator Value */
#define BCHP_DS_A_01_ACI_AGF_AGF_AVG             0x02222210 /* [RW] Digital AGC(Fine) Averager Control Register */
#define BCHP_DS_A_01_ACI_AGF_AGFI_AVG            0x02222214 /* [RW] Digital AGC(Fine) Integrator Averager Value */
#define BCHP_DS_A_01_ACI_AGF_FSCNT               0x02222218 /* [RW] Sample Rate Counter */

/***************************************************************************
 *HIST_%i - History Register
 ***************************************************************************/
#define BCHP_DS_A_01_ACI_AGF_HIST_i_ARRAY_BASE                     0x0222221c
#define BCHP_DS_A_01_ACI_AGF_HIST_i_ARRAY_START                    0
#define BCHP_DS_A_01_ACI_AGF_HIST_i_ARRAY_END                      88
#define BCHP_DS_A_01_ACI_AGF_HIST_i_ARRAY_ELEMENT_SIZE             32

/***************************************************************************
 *HIST_%i - History Register
 ***************************************************************************/
/* DS_A_01_ACI_AGF :: HIST_i :: DATA [31:00] */
#define BCHP_DS_A_01_ACI_AGF_HIST_i_DATA_MASK                      0xffffffff
#define BCHP_DS_A_01_ACI_AGF_HIST_i_DATA_SHIFT                     0
#define BCHP_DS_A_01_ACI_AGF_HIST_i_DATA_DEFAULT                   0x00000000


#endif /* #ifndef BCHP_DS_A_01_ACI_AGF_H__ */

/* End of File */
