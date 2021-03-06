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
 * Date:           Generated on               Mon Jul 28 13:28:42 2014
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

#ifndef BCHP_HDCP2_RX_0_H__
#define BCHP_HDCP2_RX_0_H__

/***************************************************************************
 *HDCP2_RX_0 - HDCP2 RX Registers
 ***************************************************************************/
#define BCHP_HDCP2_RX_0_CFG_0                    0x006f2800 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_CTRL_0                   0x006f2804 /* [RW] HDCP 2.x Control Register */
#define BCHP_HDCP2_RX_0_I2C_CFG_0                0x006f2808 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0            0x006f280c /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0         0x006f2810 /* [RO] HDCP 2.x Status */
#define BCHP_HDCP2_RX_0_STATUS_0                 0x006f2814 /* [RO] HDCP 2.x Status */
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG         0x006f2818 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG        0x006f281c /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG       0x006f2820 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG        0x006f2824 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0           0x006f2828 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD              0x006f282c /* [RW] HDCP 2.x Read_Message FIFO command */
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA             0x006f2830 /* [RW] HDCP 2.x Read_Message FIFO data */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0         0x006f2834 /* [RO] HDCP 2.x Read_Message FIFO status */
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0           0x006f2838 /* [RW] HDCP 2.x Configuration */
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE             0x006f283c /* [RO] HDCP 2.x Write_Message FIFO data type */
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD              0x006f2840 /* [RO] HDCP 2.x Write_Message FIFO command */
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA             0x006f2844 /* [RO] HDCP 2.x Write_Message FIFO data */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0         0x006f2848 /* [RO] HDCP 2.x Write_Message FIFO status */
#define BCHP_HDCP2_RX_0_HAE_MONITOR              0x006f284c /* [RO] HDCP 2.x Status */
#define BCHP_HDCP2_RX_0_RXCAPS                   0x006f2850 /* [RW] HDCP 2.x RXCAPS */
#define BCHP_HDCP2_RX_0_TXCAPS                   0x006f2854 /* [RW] HDCP 2.x TXCAPS */
#define BCHP_HDCP2_RX_0_MAILBOX_0                0x006f2858 /* [RW] HDCP 2.x Mailbox */
#define BCHP_HDCP2_RX_0_MAILBOX_1                0x006f285c /* [RW] HDCP 2.x Mailbox */
#define BCHP_HDCP2_RX_0_SPARE_REGISTER_BITS      0x006f29fc /* [RW] Spare Bits For ECO's */

/***************************************************************************
 *CFG_0 - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: CFG_0 :: reserved0 [31:16] */
#define BCHP_HDCP2_RX_0_CFG_0_reserved0_MASK                       0xffff0000
#define BCHP_HDCP2_RX_0_CFG_0_reserved0_SHIFT                      16

/* HDCP2_RX_0 :: CFG_0 :: FAST_OFFSET [15:08] */
#define BCHP_HDCP2_RX_0_CFG_0_FAST_OFFSET_MASK                     0x0000ff00
#define BCHP_HDCP2_RX_0_CFG_0_FAST_OFFSET_SHIFT                    8
#define BCHP_HDCP2_RX_0_CFG_0_FAST_OFFSET_DEFAULT                  0x00000070

/* HDCP2_RX_0 :: CFG_0 :: reserved1 [07:07] */
#define BCHP_HDCP2_RX_0_CFG_0_reserved1_MASK                       0x00000080
#define BCHP_HDCP2_RX_0_CFG_0_reserved1_SHIFT                      7

/* HDCP2_RX_0 :: CFG_0 :: REVERSE_AES_WORD_ORDER [06:06] */
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_WORD_ORDER_MASK          0x00000040
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_WORD_ORDER_SHIFT         6
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_WORD_ORDER_DEFAULT       0x00000000

/* HDCP2_RX_0 :: CFG_0 :: REVERSE_AES_OUT [05:05] */
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_OUT_MASK                 0x00000020
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_OUT_SHIFT                5
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_AES_OUT_DEFAULT              0x00000000

/* HDCP2_RX_0 :: CFG_0 :: REVERSE_CIPHER [04:04] */
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_CIPHER_MASK                  0x00000010
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_CIPHER_SHIFT                 4
#define BCHP_HDCP2_RX_0_CFG_0_REVERSE_CIPHER_DEFAULT               0x00000000

/* HDCP2_RX_0 :: CFG_0 :: reserved2 [03:02] */
#define BCHP_HDCP2_RX_0_CFG_0_reserved2_MASK                       0x0000000c
#define BCHP_HDCP2_RX_0_CFG_0_reserved2_SHIFT                      2

/* HDCP2_RX_0 :: CFG_0 :: HDCP_VERSION_SELECT [01:00] */
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_MASK             0x00000003
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_SHIFT            0
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_DEFAULT          0x00000002
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_VER_1            0
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_VER_2            1
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_AUTO             2
#define BCHP_HDCP2_RX_0_CFG_0_HDCP_VERSION_SELECT_RESERVED         3

/***************************************************************************
 *CTRL_0 - HDCP 2.x Control Register
 ***************************************************************************/
/* HDCP2_RX_0 :: CTRL_0 :: reserved0 [31:01] */
#define BCHP_HDCP2_RX_0_CTRL_0_reserved0_MASK                      0xfffffffe
#define BCHP_HDCP2_RX_0_CTRL_0_reserved0_SHIFT                     1

/* HDCP2_RX_0 :: CTRL_0 :: HDMI2HAE_INFO [00:00] */
#define BCHP_HDCP2_RX_0_CTRL_0_HDMI2HAE_INFO_MASK                  0x00000001
#define BCHP_HDCP2_RX_0_CTRL_0_HDMI2HAE_INFO_SHIFT                 0
#define BCHP_HDCP2_RX_0_CTRL_0_HDMI2HAE_INFO_DEFAULT               0x00000000

/***************************************************************************
 *I2C_CFG_0 - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: I2C_CFG_0 :: reserved0 [31:24] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_reserved0_MASK                   0xff000000
#define BCHP_HDCP2_RX_0_I2C_CFG_0_reserved0_SHIFT                  24

/* HDCP2_RX_0 :: I2C_CFG_0 :: NO_INCR_WR_MSG_OFFSET [23:16] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_MASK       0x00ff0000
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_SHIFT      16
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_DEFAULT    0x00000060

/* HDCP2_RX_0 :: I2C_CFG_0 :: NO_INCR_RD_MSG_OFFSET [15:08] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_MASK       0x0000ff00
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_SHIFT      8
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_DEFAULT    0x00000080

/* HDCP2_RX_0 :: I2C_CFG_0 :: reserved1 [07:02] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_reserved1_MASK                   0x000000fc
#define BCHP_HDCP2_RX_0_I2C_CFG_0_reserved1_SHIFT                  2

/* HDCP2_RX_0 :: I2C_CFG_0 :: NO_INCR_WR_MSG_OFFSET_ENABLE [01:01] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_ENABLE_MASK 0x00000002
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_ENABLE_SHIFT 1
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_WR_MSG_OFFSET_ENABLE_DEFAULT 0x00000001

/* HDCP2_RX_0 :: I2C_CFG_0 :: NO_INCR_RD_MSG_OFFSET_ENABLE [00:00] */
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_ENABLE_MASK 0x00000001
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_ENABLE_SHIFT 0
#define BCHP_HDCP2_RX_0_I2C_CFG_0_NO_INCR_RD_MSG_OFFSET_ENABLE_DEFAULT 0x00000001

/***************************************************************************
 *TIMEOUT_CFG_0 - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: TIMEOUT_CFG_0 :: PERIOD_0 [31:16] */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_PERIOD_0_MASK                0xffff0000
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_PERIOD_0_SHIFT               16
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_PERIOD_0_DEFAULT             0x00000000

/* HDCP2_RX_0 :: TIMEOUT_CFG_0 :: reserved0 [15:12] */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_reserved0_MASK               0x0000f000
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_reserved0_SHIFT              12

/* HDCP2_RX_0 :: TIMEOUT_CFG_0 :: MSG_ID_0 [11:04] */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_MSG_ID_0_MASK                0x00000ff0
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_MSG_ID_0_SHIFT               4
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_MSG_ID_0_DEFAULT             0x00000000

/* HDCP2_RX_0 :: TIMEOUT_CFG_0 :: STOP_0 [03:02] */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_MASK                  0x0000000c
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_SHIFT                 2
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_DEFAULT               0x00000000
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_DISABLED              0
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_STARTWRITE            1
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_STOPWRITE             2
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_STOP_0_RESERVED              3

/* HDCP2_RX_0 :: TIMEOUT_CFG_0 :: START_0 [01:00] */
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_MASK                 0x00000003
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_SHIFT                0
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_DEFAULT              0x00000000
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_DISABLED             0
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_MSG_RDY              1
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_STARTREAD            2
#define BCHP_HDCP2_RX_0_TIMEOUT_CFG_0_START_0_STOPREAD             3

/***************************************************************************
 *TIMEOUT_STATUS_0 - HDCP 2.x Status
 ***************************************************************************/
/* HDCP2_RX_0 :: TIMEOUT_STATUS_0 :: reserved0 [31:16] */
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0_reserved0_MASK            0xffff0000
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0_reserved0_SHIFT           16

/* HDCP2_RX_0 :: TIMEOUT_STATUS_0 :: COUNTER_0 [15:00] */
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0_COUNTER_0_MASK            0x0000ffff
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0_COUNTER_0_SHIFT           0
#define BCHP_HDCP2_RX_0_TIMEOUT_STATUS_0_COUNTER_0_DEFAULT         0x00000000

/***************************************************************************
 *STATUS_0 - HDCP 2.x Status
 ***************************************************************************/
/* HDCP2_RX_0 :: STATUS_0 :: reserved0 [31:03] */
#define BCHP_HDCP2_RX_0_STATUS_0_reserved0_MASK                    0xfffffff8
#define BCHP_HDCP2_RX_0_STATUS_0_reserved0_SHIFT                   3

/* HDCP2_RX_0 :: STATUS_0 :: HDCP_VERSION [02:02] */
#define BCHP_HDCP2_RX_0_STATUS_0_HDCP_VERSION_MASK                 0x00000004
#define BCHP_HDCP2_RX_0_STATUS_0_HDCP_VERSION_SHIFT                2
#define BCHP_HDCP2_RX_0_STATUS_0_HDCP_VERSION_DEFAULT              0x00000000
#define BCHP_HDCP2_RX_0_STATUS_0_HDCP_VERSION_VER_1                0
#define BCHP_HDCP2_RX_0_STATUS_0_HDCP_VERSION_VER_2                1

/* HDCP2_RX_0 :: STATUS_0 :: ENCRYPTION_ENABLED [01:01] */
#define BCHP_HDCP2_RX_0_STATUS_0_ENCRYPTION_ENABLED_MASK           0x00000002
#define BCHP_HDCP2_RX_0_STATUS_0_ENCRYPTION_ENABLED_SHIFT          1
#define BCHP_HDCP2_RX_0_STATUS_0_ENCRYPTION_ENABLED_DEFAULT        0x00000000
#define BCHP_HDCP2_RX_0_STATUS_0_ENCRYPTION_ENABLED_DISABLED       0
#define BCHP_HDCP2_RX_0_STATUS_0_ENCRYPTION_ENABLED_ENABLED        1

/* HDCP2_RX_0 :: STATUS_0 :: AUTH_STATUS [00:00] */
#define BCHP_HDCP2_RX_0_STATUS_0_AUTH_STATUS_MASK                  0x00000001
#define BCHP_HDCP2_RX_0_STATUS_0_AUTH_STATUS_SHIFT                 0
#define BCHP_HDCP2_RX_0_STATUS_0_AUTH_STATUS_DEFAULT               0x00000000
#define BCHP_HDCP2_RX_0_STATUS_0_AUTH_STATUS_UNAUTHENTICATED       0
#define BCHP_HDCP2_RX_0_STATUS_0_AUTH_STATUS_AUTHENTICATED         1

/***************************************************************************
 *HDCP2VERSION_CFG - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: HDCP2VERSION_CFG :: reserved0 [31:08] */
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG_reserved0_MASK            0xffffff00
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG_reserved0_SHIFT           8

/* HDCP2_RX_0 :: HDCP2VERSION_CFG :: VERSION [07:00] */
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG_VERSION_MASK              0x000000ff
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG_VERSION_SHIFT             0
#define BCHP_HDCP2_RX_0_HDCP2VERSION_CFG_VERSION_DEFAULT           0x00000000

/***************************************************************************
 *RXSTATUS_AUTH_CFG - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: RXSTATUS_AUTH_CFG :: reserved0 [31:01] */
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG_reserved0_MASK           0xfffffffe
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG_reserved0_SHIFT          1

/* HDCP2_RX_0 :: RXSTATUS_AUTH_CFG :: REAUTH_REQ [00:00] */
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG_REAUTH_REQ_MASK          0x00000001
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG_REAUTH_REQ_SHIFT         0
#define BCHP_HDCP2_RX_0_RXSTATUS_AUTH_CFG_REAUTH_REQ_DEFAULT       0x00000000

/***************************************************************************
 *RXSTATUS_READY_CFG - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: RXSTATUS_READY_CFG :: reserved0 [31:01] */
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG_reserved0_MASK          0xfffffffe
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG_reserved0_SHIFT         1

/* HDCP2_RX_0 :: RXSTATUS_READY_CFG :: READY [00:00] */
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG_READY_MASK              0x00000001
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG_READY_SHIFT             0
#define BCHP_HDCP2_RX_0_RXSTATUS_READY_CFG_READY_DEFAULT           0x00000000

/***************************************************************************
 *RXSTATUS_SIZE_CFG - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: RXSTATUS_SIZE_CFG :: reserved0 [31:10] */
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG_reserved0_MASK           0xfffffc00
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG_reserved0_SHIFT          10

/* HDCP2_RX_0 :: RXSTATUS_SIZE_CFG :: MESSAGE_SIZE [09:00] */
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG_MESSAGE_SIZE_MASK        0x000003ff
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG_MESSAGE_SIZE_SHIFT       0
#define BCHP_HDCP2_RX_0_RXSTATUS_SIZE_CFG_MESSAGE_SIZE_DEFAULT     0x00000000

/***************************************************************************
 *RD_FIFO_CTRL_0 - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: RD_FIFO_CTRL_0 :: reserved0 [31:03] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_reserved0_MASK              0xfffffff8
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_reserved0_SHIFT             3

/* HDCP2_RX_0 :: RD_FIFO_CTRL_0 :: CLEAR_READY_ON_MESSAGE [02:02] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READY_ON_MESSAGE_MASK 0x00000004
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READY_ON_MESSAGE_SHIFT 2
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READY_ON_MESSAGE_DEFAULT 0x00000000

/* HDCP2_RX_0 :: RD_FIFO_CTRL_0 :: CLEAR_READ_FIFO_ERR [01:01] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READ_FIFO_ERR_MASK    0x00000002
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READ_FIFO_ERR_SHIFT   1
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_CLEAR_READ_FIFO_ERR_DEFAULT 0x00000000

/* HDCP2_RX_0 :: RD_FIFO_CTRL_0 :: FIFO_RESET [00:00] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_FIFO_RESET_MASK             0x00000001
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_FIFO_RESET_SHIFT            0
#define BCHP_HDCP2_RX_0_RD_FIFO_CTRL_0_FIFO_RESET_DEFAULT          0x00000000

/***************************************************************************
 *RD_FIFO_CMD - HDCP 2.x Read_Message FIFO command
 ***************************************************************************/
/* HDCP2_RX_0 :: RD_FIFO_CMD :: reserved0 [31:18] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_reserved0_MASK                 0xfffc0000
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_reserved0_SHIFT                18

/* HDCP2_RX_0 :: RD_FIFO_CMD :: SIZE [17:08] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_SIZE_MASK                      0x0003ff00
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_SIZE_SHIFT                     8
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_SIZE_DEFAULT                   0x00000000

/* HDCP2_RX_0 :: RD_FIFO_CMD :: reserved1 [07:04] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_reserved1_MASK                 0x000000f0
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_reserved1_SHIFT                4

/* HDCP2_RX_0 :: RD_FIFO_CMD :: TYPE [03:00] */
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_TYPE_MASK                      0x0000000f
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_TYPE_SHIFT                     0
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_TYPE_DEFAULT                   0x00000000
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_TYPE_HEADER                    1
#define BCHP_HDCP2_RX_0_RD_FIFO_CMD_TYPE_FOOTER                    2

/***************************************************************************
 *RD_FIFO_DATA - HDCP 2.x Read_Message FIFO data
 ***************************************************************************/
/* HDCP2_RX_0 :: RD_FIFO_DATA :: BYTE3 [31:24] */
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE3_MASK                    0xff000000
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE3_SHIFT                   24
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE3_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: RD_FIFO_DATA :: BYTE2 [23:16] */
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE2_MASK                    0x00ff0000
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE2_SHIFT                   16
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE2_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: RD_FIFO_DATA :: BYTE1 [15:08] */
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE1_MASK                    0x0000ff00
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE1_SHIFT                   8
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE1_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: RD_FIFO_DATA :: BYTE0 [07:00] */
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE0_MASK                    0x000000ff
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE0_SHIFT                   0
#define BCHP_HDCP2_RX_0_RD_FIFO_DATA_BYTE0_DEFAULT                 0x00000000

/***************************************************************************
 *RD_FIFO_STATUS_0 - HDCP 2.x Read_Message FIFO status
 ***************************************************************************/
/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: MSG_HEADER_ERR [31:31] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_HEADER_ERR_MASK       0x80000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_HEADER_ERR_SHIFT      31
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_HEADER_ERR_DEFAULT    0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: MSG_DATA_ERR [30:30] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_DATA_ERR_MASK         0x40000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_DATA_ERR_SHIFT        30
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_DATA_ERR_DEFAULT      0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: MSG_FOOTER_ERR [29:29] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_FOOTER_ERR_MASK       0x20000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_FOOTER_ERR_SHIFT      29
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_FOOTER_ERR_DEFAULT    0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: reserved0 [28:28] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved0_MASK            0x10000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved0_SHIFT           28

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: START_ERR [27:27] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_START_ERR_MASK            0x08000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_START_ERR_SHIFT           27
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_START_ERR_DEFAULT         0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: STOP_ERR [26:26] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_STOP_ERR_MASK             0x04000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_STOP_ERR_SHIFT            26
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_STOP_ERR_DEFAULT          0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: CMD_ERR [25:25] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_CMD_ERR_MASK              0x02000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_CMD_ERR_SHIFT             25
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_CMD_ERR_DEFAULT           0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: RD_MSG_EARLY [24:24] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_RD_MSG_EARLY_MASK         0x01000000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_RD_MSG_EARLY_SHIFT        24
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_RD_MSG_EARLY_DEFAULT      0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: TMOUT_0_FAILURE [23:23] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_TMOUT_0_FAILURE_MASK      0x00800000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_TMOUT_0_FAILURE_SHIFT     23
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_TMOUT_0_FAILURE_DEFAULT   0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: reserved1 [22:22] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved1_MASK            0x00400000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved1_SHIFT           22

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: ZERO_PAD [21:21] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ZERO_PAD_MASK             0x00200000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ZERO_PAD_SHIFT            21
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ZERO_PAD_DEFAULT          0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: WORD_CNT [20:12] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_WORD_CNT_MASK             0x001ff000
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_WORD_CNT_SHIFT            12
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_WORD_CNT_DEFAULT          0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: MSG_CNT [11:08] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_CNT_MASK              0x00000f00
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_CNT_SHIFT             8
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_MSG_CNT_DEFAULT           0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: OVERFLOW [07:07] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_OVERFLOW_MASK             0x00000080
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_OVERFLOW_SHIFT            7
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_OVERFLOW_DEFAULT          0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: UNDERFLOW [06:06] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_UNDERFLOW_MASK            0x00000040
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_UNDERFLOW_SHIFT           6
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_UNDERFLOW_DEFAULT         0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: FULL [05:05] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_FULL_MASK                 0x00000020
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_FULL_SHIFT                5
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_FULL_DEFAULT              0x00000000

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: EMPTY [04:04] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_EMPTY_MASK                0x00000010
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_EMPTY_SHIFT               4
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_EMPTY_DEFAULT             0x00000001

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: reserved2 [03:01] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved2_MASK            0x0000000e
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_reserved2_SHIFT           1

/* HDCP2_RX_0 :: RD_FIFO_STATUS_0 :: ACTIVE [00:00] */
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ACTIVE_MASK               0x00000001
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ACTIVE_SHIFT              0
#define BCHP_HDCP2_RX_0_RD_FIFO_STATUS_0_ACTIVE_DEFAULT            0x00000000

/***************************************************************************
 *WR_FIFO_CTRL_0 - HDCP 2.x Configuration
 ***************************************************************************/
/* HDCP2_RX_0 :: WR_FIFO_CTRL_0 :: reserved0 [31:02] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_reserved0_MASK              0xfffffffc
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_reserved0_SHIFT             2

/* HDCP2_RX_0 :: WR_FIFO_CTRL_0 :: FIFO_READ [01:01] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_READ_MASK              0x00000002
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_READ_SHIFT             1
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_READ_DEFAULT           0x00000000

/* HDCP2_RX_0 :: WR_FIFO_CTRL_0 :: FIFO_RESET [00:00] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_RESET_MASK             0x00000001
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_RESET_SHIFT            0
#define BCHP_HDCP2_RX_0_WR_FIFO_CTRL_0_FIFO_RESET_DEFAULT          0x00000000

/***************************************************************************
 *WR_FIFO_TYPE - HDCP 2.x Write_Message FIFO data type
 ***************************************************************************/
/* HDCP2_RX_0 :: WR_FIFO_TYPE :: reserved0 [31:01] */
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_reserved0_MASK                0xfffffffe
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_reserved0_SHIFT               1

/* HDCP2_RX_0 :: WR_FIFO_TYPE :: READ_TYPE [00:00] */
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_READ_TYPE_MASK                0x00000001
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_READ_TYPE_SHIFT               0
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_READ_TYPE_DEFAULT             0x00000000
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_READ_TYPE_DATA                0
#define BCHP_HDCP2_RX_0_WR_FIFO_TYPE_READ_TYPE_CMD                 1

/***************************************************************************
 *WR_FIFO_CMD - HDCP 2.x Write_Message FIFO command
 ***************************************************************************/
/* HDCP2_RX_0 :: WR_FIFO_CMD :: reserved0 [31:18] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_reserved0_MASK                 0xfffc0000
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_reserved0_SHIFT                18

/* HDCP2_RX_0 :: WR_FIFO_CMD :: SIZE [17:08] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_SIZE_MASK                      0x0003ff00
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_SIZE_SHIFT                     8
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_SIZE_DEFAULT                   0x00000000

/* HDCP2_RX_0 :: WR_FIFO_CMD :: ERR_IND [07:04] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_ERR_IND_MASK                   0x000000f0
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_ERR_IND_SHIFT                  4
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_ERR_IND_DEFAULT                0x00000000

/* HDCP2_RX_0 :: WR_FIFO_CMD :: TYPE [03:00] */
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_TYPE_MASK                      0x0000000f
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_TYPE_SHIFT                     0
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_TYPE_DEFAULT                   0x00000000
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_TYPE_HEADER                    1
#define BCHP_HDCP2_RX_0_WR_FIFO_CMD_TYPE_FOOTER                    2

/***************************************************************************
 *WR_FIFO_DATA - HDCP 2.x Write_Message FIFO data
 ***************************************************************************/
/* HDCP2_RX_0 :: WR_FIFO_DATA :: BYTE3 [31:24] */
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE3_MASK                    0xff000000
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE3_SHIFT                   24
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE3_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: WR_FIFO_DATA :: BYTE2 [23:16] */
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE2_MASK                    0x00ff0000
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE2_SHIFT                   16
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE2_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: WR_FIFO_DATA :: BYTE1 [15:08] */
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE1_MASK                    0x0000ff00
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE1_SHIFT                   8
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE1_DEFAULT                 0x00000000

/* HDCP2_RX_0 :: WR_FIFO_DATA :: BYTE0 [07:00] */
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE0_MASK                    0x000000ff
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE0_SHIFT                   0
#define BCHP_HDCP2_RX_0_WR_FIFO_DATA_BYTE0_DEFAULT                 0x00000000

/***************************************************************************
 *WR_FIFO_STATUS_0 - HDCP 2.x Write_Message FIFO status
 ***************************************************************************/
/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: reserved0 [31:21] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_reserved0_MASK            0xffe00000
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_reserved0_SHIFT           21

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: WORD_CNT [20:12] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_WORD_CNT_MASK             0x001ff000
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_WORD_CNT_SHIFT            12
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_WORD_CNT_DEFAULT          0x00000000

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: MSG_CNT [11:08] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_MSG_CNT_MASK              0x00000f00
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_MSG_CNT_SHIFT             8
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_MSG_CNT_DEFAULT           0x00000000

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: OVERFLOW [07:07] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_OVERFLOW_MASK             0x00000080
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_OVERFLOW_SHIFT            7
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_OVERFLOW_DEFAULT          0x00000000

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: UNDERFLOW [06:06] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_UNDERFLOW_MASK            0x00000040
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_UNDERFLOW_SHIFT           6
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_UNDERFLOW_DEFAULT         0x00000000

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: FULL [05:05] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_FULL_MASK                 0x00000020
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_FULL_SHIFT                5
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_FULL_DEFAULT              0x00000000

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: EMPTY [04:04] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_EMPTY_MASK                0x00000010
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_EMPTY_SHIFT               4
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_EMPTY_DEFAULT             0x00000001

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: reserved1 [03:01] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_reserved1_MASK            0x0000000e
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_reserved1_SHIFT           1

/* HDCP2_RX_0 :: WR_FIFO_STATUS_0 :: ACTIVE [00:00] */
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_ACTIVE_MASK               0x00000001
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_ACTIVE_SHIFT              0
#define BCHP_HDCP2_RX_0_WR_FIFO_STATUS_0_ACTIVE_DEFAULT            0x00000000

/***************************************************************************
 *HAE_MONITOR - HDCP 2.x Status
 ***************************************************************************/
/* HDCP2_RX_0 :: HAE_MONITOR :: reserved0 [31:03] */
#define BCHP_HDCP2_RX_0_HAE_MONITOR_reserved0_MASK                 0xfffffff8
#define BCHP_HDCP2_RX_0_HAE_MONITOR_reserved0_SHIFT                3

/* HDCP2_RX_0 :: HAE_MONITOR :: OTP_HDCP22_DISABLE [02:02] */
#define BCHP_HDCP2_RX_0_HAE_MONITOR_OTP_HDCP22_DISABLE_MASK        0x00000004
#define BCHP_HDCP2_RX_0_HAE_MONITOR_OTP_HDCP22_DISABLE_SHIFT       2
#define BCHP_HDCP2_RX_0_HAE_MONITOR_OTP_HDCP22_DISABLE_DEFAULT     0x00000000

/* HDCP2_RX_0 :: HAE_MONITOR :: SPARE_INPUT [01:01] */
#define BCHP_HDCP2_RX_0_HAE_MONITOR_SPARE_INPUT_MASK               0x00000002
#define BCHP_HDCP2_RX_0_HAE_MONITOR_SPARE_INPUT_SHIFT              1
#define BCHP_HDCP2_RX_0_HAE_MONITOR_SPARE_INPUT_DEFAULT            0x00000000

/* HDCP2_RX_0 :: HAE_MONITOR :: ENFORCE_ENCRYPTION [00:00] */
#define BCHP_HDCP2_RX_0_HAE_MONITOR_ENFORCE_ENCRYPTION_MASK        0x00000001
#define BCHP_HDCP2_RX_0_HAE_MONITOR_ENFORCE_ENCRYPTION_SHIFT       0
#define BCHP_HDCP2_RX_0_HAE_MONITOR_ENFORCE_ENCRYPTION_DEFAULT     0x00000000

/***************************************************************************
 *RXCAPS - HDCP 2.x RXCAPS
 ***************************************************************************/
/* HDCP2_RX_0 :: RXCAPS :: reserved0 [31:24] */
#define BCHP_HDCP2_RX_0_RXCAPS_reserved0_MASK                      0xff000000
#define BCHP_HDCP2_RX_0_RXCAPS_reserved0_SHIFT                     24

/* HDCP2_RX_0 :: RXCAPS :: VERSION [23:16] */
#define BCHP_HDCP2_RX_0_RXCAPS_VERSION_MASK                        0x00ff0000
#define BCHP_HDCP2_RX_0_RXCAPS_VERSION_SHIFT                       16
#define BCHP_HDCP2_RX_0_RXCAPS_VERSION_DEFAULT                     0x00000000

/* HDCP2_RX_0 :: RXCAPS :: RECEIVER_CAPABILITY_MASK [15:02] */
#define BCHP_HDCP2_RX_0_RXCAPS_RECEIVER_CAPABILITY_MASK_MASK       0x0000fffc
#define BCHP_HDCP2_RX_0_RXCAPS_RECEIVER_CAPABILITY_MASK_SHIFT      2
#define BCHP_HDCP2_RX_0_RXCAPS_RECEIVER_CAPABILITY_MASK_DEFAULT    0x00000000

/* HDCP2_RX_0 :: RXCAPS :: reserved1 [01:01] */
#define BCHP_HDCP2_RX_0_RXCAPS_reserved1_MASK                      0x00000002
#define BCHP_HDCP2_RX_0_RXCAPS_reserved1_SHIFT                     1

/* HDCP2_RX_0 :: RXCAPS :: REPEATER [00:00] */
#define BCHP_HDCP2_RX_0_RXCAPS_REPEATER_MASK                       0x00000001
#define BCHP_HDCP2_RX_0_RXCAPS_REPEATER_SHIFT                      0
#define BCHP_HDCP2_RX_0_RXCAPS_REPEATER_DEFAULT                    0x00000000

/***************************************************************************
 *TXCAPS - HDCP 2.x TXCAPS
 ***************************************************************************/
/* HDCP2_RX_0 :: TXCAPS :: reserved0 [31:24] */
#define BCHP_HDCP2_RX_0_TXCAPS_reserved0_MASK                      0xff000000
#define BCHP_HDCP2_RX_0_TXCAPS_reserved0_SHIFT                     24

/* HDCP2_RX_0 :: TXCAPS :: VERSION [23:16] */
#define BCHP_HDCP2_RX_0_TXCAPS_VERSION_MASK                        0x00ff0000
#define BCHP_HDCP2_RX_0_TXCAPS_VERSION_SHIFT                       16
#define BCHP_HDCP2_RX_0_TXCAPS_VERSION_DEFAULT                     0x00000000

/* HDCP2_RX_0 :: TXCAPS :: TRANSMITTER_CAPABILITY_MASK [15:00] */
#define BCHP_HDCP2_RX_0_TXCAPS_TRANSMITTER_CAPABILITY_MASK_MASK    0x0000ffff
#define BCHP_HDCP2_RX_0_TXCAPS_TRANSMITTER_CAPABILITY_MASK_SHIFT   0
#define BCHP_HDCP2_RX_0_TXCAPS_TRANSMITTER_CAPABILITY_MASK_DEFAULT 0x00000000

/***************************************************************************
 *MAILBOX_0 - HDCP 2.x Mailbox
 ***************************************************************************/
/* HDCP2_RX_0 :: MAILBOX_0 :: DATA [31:00] */
#define BCHP_HDCP2_RX_0_MAILBOX_0_DATA_MASK                        0xffffffff
#define BCHP_HDCP2_RX_0_MAILBOX_0_DATA_SHIFT                       0
#define BCHP_HDCP2_RX_0_MAILBOX_0_DATA_DEFAULT                     0x00000000

/***************************************************************************
 *MAILBOX_1 - HDCP 2.x Mailbox
 ***************************************************************************/
/* HDCP2_RX_0 :: MAILBOX_1 :: DATA [31:00] */
#define BCHP_HDCP2_RX_0_MAILBOX_1_DATA_MASK                        0xffffffff
#define BCHP_HDCP2_RX_0_MAILBOX_1_DATA_SHIFT                       0
#define BCHP_HDCP2_RX_0_MAILBOX_1_DATA_DEFAULT                     0x00000000

/***************************************************************************
 *SPARE_REGISTER_BITS - Spare Bits For ECO's
 ***************************************************************************/
/* HDCP2_RX_0 :: SPARE_REGISTER_BITS :: HDCP2_RX_SPARE [31:00] */
#define BCHP_HDCP2_RX_0_SPARE_REGISTER_BITS_HDCP2_RX_SPARE_MASK    0xffffffff
#define BCHP_HDCP2_RX_0_SPARE_REGISTER_BITS_HDCP2_RX_SPARE_SHIFT   0
#define BCHP_HDCP2_RX_0_SPARE_REGISTER_BITS_HDCP2_RX_SPARE_DEFAULT 0x00000000

#endif /* #ifndef BCHP_HDCP2_RX_0_H__ */

/* End of File */
