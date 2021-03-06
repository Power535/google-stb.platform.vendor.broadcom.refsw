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
 * Date:           Generated on               Tue May 12 15:31:57 2015
 *                 Full Compile MD5 Checksum  654f5b1025c3f32e1ac79a0158cb9296
 *                     (minus title and desc)
 *                 MD5 Checksum               f5dfcaebcf2741b1de57e2e58f246be2
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     16053
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_AVS_BSTI_H__
#define BCHP_AVS_BSTI_H__

/***************************************************************************
 *AVS_BSTI - BSTI Registers
 ***************************************************************************/
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA        0x04111400 /* [RW] Dedicate to issue BSTI commands which includes access type, address and write data. (NOTE: A write to the register will trigger a transaction on the BSTI) */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA          0x04111404 /* [RO] Indicate the status of BSTI and read data */

/***************************************************************************
 *BSTI_CMD_ADDR_WDATA - Dedicate to issue BSTI commands which includes access type, address and write data. (NOTE: A write to the register will trigger a transaction on the BSTI)
 ***************************************************************************/
/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: reserved0 [31:29] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved0_MASK           0xe0000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved0_SHIFT          29

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: read_write [28:28] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_MASK          0x10000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_SHIFT         28
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_read_write_DEFAULT       0x00000000

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: reserved1 [27:26] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved1_MASK           0x0c000000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_reserved1_SHIFT          26

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: address [25:16] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_MASK             0x03ff0000
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_SHIFT            16
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_address_DEFAULT          0x00000000

/* AVS_BSTI :: BSTI_CMD_ADDR_WDATA :: write_data [15:00] */
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_MASK          0x0000ffff
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_SHIFT         0
#define BCHP_AVS_BSTI_BSTI_CMD_ADDR_WDATA_write_data_DEFAULT       0x00000000

/***************************************************************************
 *BSTI_STATUS_RDATA - Indicate the status of BSTI and read data
 ***************************************************************************/
/* AVS_BSTI :: BSTI_STATUS_RDATA :: reserved0 [31:17] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_reserved0_MASK             0xfffe0000
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_reserved0_SHIFT            17

/* AVS_BSTI :: BSTI_STATUS_RDATA :: busy [16:16] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_MASK                  0x00010000
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_SHIFT                 16
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_busy_DEFAULT               0x00000000

/* AVS_BSTI :: BSTI_STATUS_RDATA :: read_data [15:00] */
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_MASK             0x0000ffff
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_SHIFT            0
#define BCHP_AVS_BSTI_BSTI_STATUS_RDATA_read_data_DEFAULT          0x00000000

#endif /* #ifndef BCHP_AVS_BSTI_H__ */

/* End of File */
