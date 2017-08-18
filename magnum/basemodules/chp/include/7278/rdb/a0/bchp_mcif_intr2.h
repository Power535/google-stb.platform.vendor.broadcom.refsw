/********************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
********************************************************************************/

#ifndef BCHP_MCIF_INTR2_H__
#define BCHP_MCIF_INTR2_H__

/***************************************************************************
 *MCIF_INTR2 - MCIF Level 2 Interrupt Registers
 ***************************************************************************/
#define BCHP_MCIF_INTR2_CPU_STATUS               0x0040b480 /* [RO][32] CPU interrupt Status Register */
#define BCHP_MCIF_INTR2_CPU_SET                  0x0040b484 /* [WO][32] CPU interrupt Set Register */
#define BCHP_MCIF_INTR2_CPU_CLEAR                0x0040b488 /* [WO][32] CPU interrupt Clear Register */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS          0x0040b48c /* [RO][32] CPU interrupt Mask Status Register */
#define BCHP_MCIF_INTR2_CPU_MASK_SET             0x0040b490 /* [WO][32] CPU interrupt Mask Set Register */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR           0x0040b494 /* [WO][32] CPU interrupt Mask Clear Register */
#define BCHP_MCIF_INTR2_PCI_STATUS               0x0040b498 /* [RO][32] PCI interrupt Status Register */
#define BCHP_MCIF_INTR2_PCI_SET                  0x0040b49c /* [WO][32] PCI interrupt Set Register */
#define BCHP_MCIF_INTR2_PCI_CLEAR                0x0040b4a0 /* [WO][32] PCI interrupt Clear Register */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS          0x0040b4a4 /* [RO][32] PCI interrupt Mask Status Register */
#define BCHP_MCIF_INTR2_PCI_MASK_SET             0x0040b4a8 /* [WO][32] PCI interrupt Mask Set Register */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR           0x0040b4ac /* [WO][32] PCI interrupt Mask Clear Register */
#define BCHP_MCIF_INTR2_SCPU_STATUS              0x0040b4b0 /* [RO][32] SCPU interrupt Status Register */
#define BCHP_MCIF_INTR2_SCPU_SET                 0x0040b4b4 /* [WO][32] SCPU interrupt Set Register */
#define BCHP_MCIF_INTR2_SCPU_CLEAR               0x0040b4b8 /* [WO][32] SCPU interrupt Clear Register */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS         0x0040b4bc /* [RO][32] SCPU interrupt Mask Status Register */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET            0x0040b4c0 /* [WO][32] SCPU interrupt Mask Set Register */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR          0x0040b4c4 /* [WO][32] SCPU interrupt Mask Clear Register */

/***************************************************************************
 *CPU_STATUS - CPU interrupt Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_STATUS_reserved0_MASK                  0xffc00000
#define BCHP_MCIF_INTR2_CPU_STATUS_reserved0_SHIFT                 22

/* MCIF_INTR2 :: CPU_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_STATUS_reserved1_MASK                  0x003ff800
#define BCHP_MCIF_INTR2_CPU_STATUS_reserved1_SHIFT                 11

/* MCIF_INTR2 :: CPU_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_ALL_DONE_MASK                0x00000400
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_ALL_DONE_SHIFT               10
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_ALL_DONE_DEFAULT             0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MEM_RDY_MASK                 0x00000200
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MEM_RDY_SHIFT                9
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MEM_RDY_DEFAULT              0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_HW_ER_MASK                 0x00000100
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_HW_ER_SHIFT                8
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_HW_ER_DEFAULT              0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_FIFO_OF_MASK                 0x00000080
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_FIFO_OF_SHIFT                7
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_FIFO_OF_DEFAULT              0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_FIFO_UF_MASK                 0x00000040
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_FIFO_UF_SHIFT                6
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_FIFO_UF_DEFAULT              0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_ER_MASK                    0x00000020
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_ER_SHIFT                   5
#define BCHP_MCIF_INTR2_CPU_STATUS_HOST_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_STATUS_CARD_ER_MASK                    0x00000010
#define BCHP_MCIF_INTR2_CPU_STATUS_CARD_ER_SHIFT                   4
#define BCHP_MCIF_INTR2_CPU_STATUS_CARD_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_MWPKT_MASK                   0x00000008
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_MWPKT_SHIFT                  3
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_MWPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MRPKT_MASK                   0x00000004
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MRPKT_SHIFT                  2
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_MRPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_DONE_MASK                    0x00000002
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_DONE_SHIFT                   1
#define BCHP_MCIF_INTR2_CPU_STATUS_RX_DONE_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_DONE_MASK                    0x00000001
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_DONE_SHIFT                   0
#define BCHP_MCIF_INTR2_CPU_STATUS_TX_DONE_DEFAULT                 0x00000000

/***************************************************************************
 *CPU_SET - CPU interrupt Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_SET_reserved0_MASK                     0xffc00000
#define BCHP_MCIF_INTR2_CPU_SET_reserved0_SHIFT                    22

/* MCIF_INTR2 :: CPU_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_SET_reserved1_MASK                     0x003ff800
#define BCHP_MCIF_INTR2_CPU_SET_reserved1_SHIFT                    11

/* MCIF_INTR2 :: CPU_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_SET_RX_ALL_DONE_MASK                   0x00000400
#define BCHP_MCIF_INTR2_CPU_SET_RX_ALL_DONE_SHIFT                  10
#define BCHP_MCIF_INTR2_CPU_SET_RX_ALL_DONE_DEFAULT                0x00000000

/* MCIF_INTR2 :: CPU_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_SET_TX_MEM_RDY_MASK                    0x00000200
#define BCHP_MCIF_INTR2_CPU_SET_TX_MEM_RDY_SHIFT                   9
#define BCHP_MCIF_INTR2_CPU_SET_TX_MEM_RDY_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_SET_HOST_HW_ER_MASK                    0x00000100
#define BCHP_MCIF_INTR2_CPU_SET_HOST_HW_ER_SHIFT                   8
#define BCHP_MCIF_INTR2_CPU_SET_HOST_HW_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_SET_RX_FIFO_OF_MASK                    0x00000080
#define BCHP_MCIF_INTR2_CPU_SET_RX_FIFO_OF_SHIFT                   7
#define BCHP_MCIF_INTR2_CPU_SET_RX_FIFO_OF_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_SET_TX_FIFO_UF_MASK                    0x00000040
#define BCHP_MCIF_INTR2_CPU_SET_TX_FIFO_UF_SHIFT                   6
#define BCHP_MCIF_INTR2_CPU_SET_TX_FIFO_UF_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_SET_HOST_ER_MASK                       0x00000020
#define BCHP_MCIF_INTR2_CPU_SET_HOST_ER_SHIFT                      5
#define BCHP_MCIF_INTR2_CPU_SET_HOST_ER_DEFAULT                    0x00000000

/* MCIF_INTR2 :: CPU_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_SET_CARD_ER_MASK                       0x00000010
#define BCHP_MCIF_INTR2_CPU_SET_CARD_ER_SHIFT                      4
#define BCHP_MCIF_INTR2_CPU_SET_CARD_ER_DEFAULT                    0x00000000

/* MCIF_INTR2 :: CPU_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_SET_RX_MWPKT_MASK                      0x00000008
#define BCHP_MCIF_INTR2_CPU_SET_RX_MWPKT_SHIFT                     3
#define BCHP_MCIF_INTR2_CPU_SET_RX_MWPKT_DEFAULT                   0x00000000

/* MCIF_INTR2 :: CPU_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_SET_TX_MRPKT_MASK                      0x00000004
#define BCHP_MCIF_INTR2_CPU_SET_TX_MRPKT_SHIFT                     2
#define BCHP_MCIF_INTR2_CPU_SET_TX_MRPKT_DEFAULT                   0x00000000

/* MCIF_INTR2 :: CPU_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_SET_RX_DONE_MASK                       0x00000002
#define BCHP_MCIF_INTR2_CPU_SET_RX_DONE_SHIFT                      1
#define BCHP_MCIF_INTR2_CPU_SET_RX_DONE_DEFAULT                    0x00000000

/* MCIF_INTR2 :: CPU_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_SET_TX_DONE_MASK                       0x00000001
#define BCHP_MCIF_INTR2_CPU_SET_TX_DONE_SHIFT                      0
#define BCHP_MCIF_INTR2_CPU_SET_TX_DONE_DEFAULT                    0x00000000

/***************************************************************************
 *CPU_CLEAR - CPU interrupt Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_reserved0_MASK                   0xffc00000
#define BCHP_MCIF_INTR2_CPU_CLEAR_reserved0_SHIFT                  22

/* MCIF_INTR2 :: CPU_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_reserved1_MASK                   0x003ff800
#define BCHP_MCIF_INTR2_CPU_CLEAR_reserved1_SHIFT                  11

/* MCIF_INTR2 :: CPU_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_ALL_DONE_MASK                 0x00000400
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_ALL_DONE_SHIFT                10
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_ALL_DONE_DEFAULT              0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MEM_RDY_MASK                  0x00000200
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MEM_RDY_SHIFT                 9
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MEM_RDY_DEFAULT               0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_HW_ER_MASK                  0x00000100
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_HW_ER_SHIFT                 8
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_HW_ER_DEFAULT               0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_FIFO_OF_MASK                  0x00000080
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_FIFO_OF_SHIFT                 7
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_FIFO_OF_DEFAULT               0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_FIFO_UF_MASK                  0x00000040
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_FIFO_UF_SHIFT                 6
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_FIFO_UF_DEFAULT               0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_ER_MASK                     0x00000020
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_ER_SHIFT                    5
#define BCHP_MCIF_INTR2_CPU_CLEAR_HOST_ER_DEFAULT                  0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_CARD_ER_MASK                     0x00000010
#define BCHP_MCIF_INTR2_CPU_CLEAR_CARD_ER_SHIFT                    4
#define BCHP_MCIF_INTR2_CPU_CLEAR_CARD_ER_DEFAULT                  0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_MWPKT_MASK                    0x00000008
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_MWPKT_SHIFT                   3
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_MWPKT_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MRPKT_MASK                    0x00000004
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MRPKT_SHIFT                   2
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_MRPKT_DEFAULT                 0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_DONE_MASK                     0x00000002
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_DONE_SHIFT                    1
#define BCHP_MCIF_INTR2_CPU_CLEAR_RX_DONE_DEFAULT                  0x00000000

/* MCIF_INTR2 :: CPU_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_DONE_MASK                     0x00000001
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_DONE_SHIFT                    0
#define BCHP_MCIF_INTR2_CPU_CLEAR_TX_DONE_DEFAULT                  0x00000000

/***************************************************************************
 *CPU_MASK_STATUS - CPU interrupt Mask Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_MASK_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_reserved0_MASK             0xffc00000
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_reserved0_SHIFT            22

/* MCIF_INTR2 :: CPU_MASK_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_reserved1_MASK             0x003ff800
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_reserved1_SHIFT            11

/* MCIF_INTR2 :: CPU_MASK_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_ALL_DONE_MASK           0x00000400
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_ALL_DONE_SHIFT          10
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_ALL_DONE_DEFAULT        0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MEM_RDY_MASK            0x00000200
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MEM_RDY_SHIFT           9
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MEM_RDY_DEFAULT         0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_HW_ER_MASK            0x00000100
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_HW_ER_SHIFT           8
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_HW_ER_DEFAULT         0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_FIFO_OF_MASK            0x00000080
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_FIFO_OF_SHIFT           7
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_FIFO_OF_DEFAULT         0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_FIFO_UF_MASK            0x00000040
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_FIFO_UF_SHIFT           6
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_FIFO_UF_DEFAULT         0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_ER_MASK               0x00000020
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_ER_SHIFT              5
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_HOST_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_CARD_ER_MASK               0x00000010
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_CARD_ER_SHIFT              4
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_CARD_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_MWPKT_MASK              0x00000008
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_MWPKT_SHIFT             3
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_MWPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MRPKT_MASK              0x00000004
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MRPKT_SHIFT             2
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_MRPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_DONE_MASK               0x00000002
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_DONE_SHIFT              1
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_RX_DONE_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_DONE_MASK               0x00000001
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_DONE_SHIFT              0
#define BCHP_MCIF_INTR2_CPU_MASK_STATUS_TX_DONE_DEFAULT            0x00000001

/***************************************************************************
 *CPU_MASK_SET - CPU interrupt Mask Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_MASK_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_reserved0_MASK                0xffc00000
#define BCHP_MCIF_INTR2_CPU_MASK_SET_reserved0_SHIFT               22

/* MCIF_INTR2 :: CPU_MASK_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_reserved1_MASK                0x003ff800
#define BCHP_MCIF_INTR2_CPU_MASK_SET_reserved1_SHIFT               11

/* MCIF_INTR2 :: CPU_MASK_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_ALL_DONE_MASK              0x00000400
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_ALL_DONE_SHIFT             10
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_ALL_DONE_DEFAULT           0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MEM_RDY_MASK               0x00000200
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MEM_RDY_SHIFT              9
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MEM_RDY_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_HW_ER_MASK               0x00000100
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_HW_ER_SHIFT              8
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_HW_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_FIFO_OF_MASK               0x00000080
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_FIFO_OF_SHIFT              7
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_FIFO_OF_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_FIFO_UF_MASK               0x00000040
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_FIFO_UF_SHIFT              6
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_FIFO_UF_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_ER_MASK                  0x00000020
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_ER_SHIFT                 5
#define BCHP_MCIF_INTR2_CPU_MASK_SET_HOST_ER_DEFAULT               0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_CARD_ER_MASK                  0x00000010
#define BCHP_MCIF_INTR2_CPU_MASK_SET_CARD_ER_SHIFT                 4
#define BCHP_MCIF_INTR2_CPU_MASK_SET_CARD_ER_DEFAULT               0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_MWPKT_MASK                 0x00000008
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_MWPKT_SHIFT                3
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_MWPKT_DEFAULT              0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MRPKT_MASK                 0x00000004
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MRPKT_SHIFT                2
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_MRPKT_DEFAULT              0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_DONE_MASK                  0x00000002
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_DONE_SHIFT                 1
#define BCHP_MCIF_INTR2_CPU_MASK_SET_RX_DONE_DEFAULT               0x00000001

/* MCIF_INTR2 :: CPU_MASK_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_DONE_MASK                  0x00000001
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_DONE_SHIFT                 0
#define BCHP_MCIF_INTR2_CPU_MASK_SET_TX_DONE_DEFAULT               0x00000001

/***************************************************************************
 *CPU_MASK_CLEAR - CPU interrupt Mask Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: CPU_MASK_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_reserved0_MASK              0xffc00000
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_reserved0_SHIFT             22

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_reserved1_MASK              0x003ff800
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_reserved1_SHIFT             11

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_ALL_DONE_MASK            0x00000400
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_ALL_DONE_SHIFT           10
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_ALL_DONE_DEFAULT         0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MEM_RDY_MASK             0x00000200
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MEM_RDY_SHIFT            9
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MEM_RDY_DEFAULT          0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_HW_ER_MASK             0x00000100
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_HW_ER_SHIFT            8
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_HW_ER_DEFAULT          0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_FIFO_OF_MASK             0x00000080
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_FIFO_OF_SHIFT            7
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_FIFO_OF_DEFAULT          0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_FIFO_UF_MASK             0x00000040
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_FIFO_UF_SHIFT            6
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_FIFO_UF_DEFAULT          0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_ER_MASK                0x00000020
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_ER_SHIFT               5
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_HOST_ER_DEFAULT             0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_CARD_ER_MASK                0x00000010
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_CARD_ER_SHIFT               4
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_CARD_ER_DEFAULT             0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_MWPKT_MASK               0x00000008
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_MWPKT_SHIFT              3
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_MWPKT_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MRPKT_MASK               0x00000004
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MRPKT_SHIFT              2
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_MRPKT_DEFAULT            0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_DONE_MASK                0x00000002
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_DONE_SHIFT               1
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_RX_DONE_DEFAULT             0x00000001

/* MCIF_INTR2 :: CPU_MASK_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_DONE_MASK                0x00000001
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_DONE_SHIFT               0
#define BCHP_MCIF_INTR2_CPU_MASK_CLEAR_TX_DONE_DEFAULT             0x00000001

/***************************************************************************
 *PCI_STATUS - PCI interrupt Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_STATUS_reserved0_MASK                  0xffc00000
#define BCHP_MCIF_INTR2_PCI_STATUS_reserved0_SHIFT                 22

/* MCIF_INTR2 :: PCI_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_STATUS_reserved1_MASK                  0x003ff800
#define BCHP_MCIF_INTR2_PCI_STATUS_reserved1_SHIFT                 11

/* MCIF_INTR2 :: PCI_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_ALL_DONE_MASK                0x00000400
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_ALL_DONE_SHIFT               10
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_ALL_DONE_DEFAULT             0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MEM_RDY_MASK                 0x00000200
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MEM_RDY_SHIFT                9
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MEM_RDY_DEFAULT              0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_HW_ER_MASK                 0x00000100
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_HW_ER_SHIFT                8
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_HW_ER_DEFAULT              0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_FIFO_OF_MASK                 0x00000080
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_FIFO_OF_SHIFT                7
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_FIFO_OF_DEFAULT              0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_FIFO_UF_MASK                 0x00000040
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_FIFO_UF_SHIFT                6
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_FIFO_UF_DEFAULT              0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_ER_MASK                    0x00000020
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_ER_SHIFT                   5
#define BCHP_MCIF_INTR2_PCI_STATUS_HOST_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_STATUS_CARD_ER_MASK                    0x00000010
#define BCHP_MCIF_INTR2_PCI_STATUS_CARD_ER_SHIFT                   4
#define BCHP_MCIF_INTR2_PCI_STATUS_CARD_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_MWPKT_MASK                   0x00000008
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_MWPKT_SHIFT                  3
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_MWPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MRPKT_MASK                   0x00000004
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MRPKT_SHIFT                  2
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_MRPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_DONE_MASK                    0x00000002
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_DONE_SHIFT                   1
#define BCHP_MCIF_INTR2_PCI_STATUS_RX_DONE_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_DONE_MASK                    0x00000001
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_DONE_SHIFT                   0
#define BCHP_MCIF_INTR2_PCI_STATUS_TX_DONE_DEFAULT                 0x00000000

/***************************************************************************
 *PCI_SET - PCI interrupt Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_SET_reserved0_MASK                     0xffc00000
#define BCHP_MCIF_INTR2_PCI_SET_reserved0_SHIFT                    22

/* MCIF_INTR2 :: PCI_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_SET_reserved1_MASK                     0x003ff800
#define BCHP_MCIF_INTR2_PCI_SET_reserved1_SHIFT                    11

/* MCIF_INTR2 :: PCI_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_SET_RX_ALL_DONE_MASK                   0x00000400
#define BCHP_MCIF_INTR2_PCI_SET_RX_ALL_DONE_SHIFT                  10
#define BCHP_MCIF_INTR2_PCI_SET_RX_ALL_DONE_DEFAULT                0x00000000

/* MCIF_INTR2 :: PCI_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_SET_TX_MEM_RDY_MASK                    0x00000200
#define BCHP_MCIF_INTR2_PCI_SET_TX_MEM_RDY_SHIFT                   9
#define BCHP_MCIF_INTR2_PCI_SET_TX_MEM_RDY_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_SET_HOST_HW_ER_MASK                    0x00000100
#define BCHP_MCIF_INTR2_PCI_SET_HOST_HW_ER_SHIFT                   8
#define BCHP_MCIF_INTR2_PCI_SET_HOST_HW_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_SET_RX_FIFO_OF_MASK                    0x00000080
#define BCHP_MCIF_INTR2_PCI_SET_RX_FIFO_OF_SHIFT                   7
#define BCHP_MCIF_INTR2_PCI_SET_RX_FIFO_OF_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_SET_TX_FIFO_UF_MASK                    0x00000040
#define BCHP_MCIF_INTR2_PCI_SET_TX_FIFO_UF_SHIFT                   6
#define BCHP_MCIF_INTR2_PCI_SET_TX_FIFO_UF_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_SET_HOST_ER_MASK                       0x00000020
#define BCHP_MCIF_INTR2_PCI_SET_HOST_ER_SHIFT                      5
#define BCHP_MCIF_INTR2_PCI_SET_HOST_ER_DEFAULT                    0x00000000

/* MCIF_INTR2 :: PCI_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_SET_CARD_ER_MASK                       0x00000010
#define BCHP_MCIF_INTR2_PCI_SET_CARD_ER_SHIFT                      4
#define BCHP_MCIF_INTR2_PCI_SET_CARD_ER_DEFAULT                    0x00000000

/* MCIF_INTR2 :: PCI_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_SET_RX_MWPKT_MASK                      0x00000008
#define BCHP_MCIF_INTR2_PCI_SET_RX_MWPKT_SHIFT                     3
#define BCHP_MCIF_INTR2_PCI_SET_RX_MWPKT_DEFAULT                   0x00000000

/* MCIF_INTR2 :: PCI_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_SET_TX_MRPKT_MASK                      0x00000004
#define BCHP_MCIF_INTR2_PCI_SET_TX_MRPKT_SHIFT                     2
#define BCHP_MCIF_INTR2_PCI_SET_TX_MRPKT_DEFAULT                   0x00000000

/* MCIF_INTR2 :: PCI_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_SET_RX_DONE_MASK                       0x00000002
#define BCHP_MCIF_INTR2_PCI_SET_RX_DONE_SHIFT                      1
#define BCHP_MCIF_INTR2_PCI_SET_RX_DONE_DEFAULT                    0x00000000

/* MCIF_INTR2 :: PCI_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_SET_TX_DONE_MASK                       0x00000001
#define BCHP_MCIF_INTR2_PCI_SET_TX_DONE_SHIFT                      0
#define BCHP_MCIF_INTR2_PCI_SET_TX_DONE_DEFAULT                    0x00000000

/***************************************************************************
 *PCI_CLEAR - PCI interrupt Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_reserved0_MASK                   0xffc00000
#define BCHP_MCIF_INTR2_PCI_CLEAR_reserved0_SHIFT                  22

/* MCIF_INTR2 :: PCI_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_reserved1_MASK                   0x003ff800
#define BCHP_MCIF_INTR2_PCI_CLEAR_reserved1_SHIFT                  11

/* MCIF_INTR2 :: PCI_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_ALL_DONE_MASK                 0x00000400
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_ALL_DONE_SHIFT                10
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_ALL_DONE_DEFAULT              0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MEM_RDY_MASK                  0x00000200
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MEM_RDY_SHIFT                 9
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MEM_RDY_DEFAULT               0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_HW_ER_MASK                  0x00000100
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_HW_ER_SHIFT                 8
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_HW_ER_DEFAULT               0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_FIFO_OF_MASK                  0x00000080
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_FIFO_OF_SHIFT                 7
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_FIFO_OF_DEFAULT               0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_FIFO_UF_MASK                  0x00000040
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_FIFO_UF_SHIFT                 6
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_FIFO_UF_DEFAULT               0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_ER_MASK                     0x00000020
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_ER_SHIFT                    5
#define BCHP_MCIF_INTR2_PCI_CLEAR_HOST_ER_DEFAULT                  0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_CARD_ER_MASK                     0x00000010
#define BCHP_MCIF_INTR2_PCI_CLEAR_CARD_ER_SHIFT                    4
#define BCHP_MCIF_INTR2_PCI_CLEAR_CARD_ER_DEFAULT                  0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_MWPKT_MASK                    0x00000008
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_MWPKT_SHIFT                   3
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_MWPKT_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MRPKT_MASK                    0x00000004
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MRPKT_SHIFT                   2
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_MRPKT_DEFAULT                 0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_DONE_MASK                     0x00000002
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_DONE_SHIFT                    1
#define BCHP_MCIF_INTR2_PCI_CLEAR_RX_DONE_DEFAULT                  0x00000000

/* MCIF_INTR2 :: PCI_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_DONE_MASK                     0x00000001
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_DONE_SHIFT                    0
#define BCHP_MCIF_INTR2_PCI_CLEAR_TX_DONE_DEFAULT                  0x00000000

/***************************************************************************
 *PCI_MASK_STATUS - PCI interrupt Mask Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_MASK_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_reserved0_MASK             0xffc00000
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_reserved0_SHIFT            22

/* MCIF_INTR2 :: PCI_MASK_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_reserved1_MASK             0x003ff800
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_reserved1_SHIFT            11

/* MCIF_INTR2 :: PCI_MASK_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_ALL_DONE_MASK           0x00000400
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_ALL_DONE_SHIFT          10
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_ALL_DONE_DEFAULT        0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MEM_RDY_MASK            0x00000200
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MEM_RDY_SHIFT           9
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MEM_RDY_DEFAULT         0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_HW_ER_MASK            0x00000100
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_HW_ER_SHIFT           8
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_HW_ER_DEFAULT         0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_FIFO_OF_MASK            0x00000080
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_FIFO_OF_SHIFT           7
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_FIFO_OF_DEFAULT         0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_FIFO_UF_MASK            0x00000040
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_FIFO_UF_SHIFT           6
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_FIFO_UF_DEFAULT         0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_ER_MASK               0x00000020
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_ER_SHIFT              5
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_HOST_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_CARD_ER_MASK               0x00000010
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_CARD_ER_SHIFT              4
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_CARD_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_MWPKT_MASK              0x00000008
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_MWPKT_SHIFT             3
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_MWPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MRPKT_MASK              0x00000004
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MRPKT_SHIFT             2
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_MRPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_DONE_MASK               0x00000002
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_DONE_SHIFT              1
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_RX_DONE_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_DONE_MASK               0x00000001
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_DONE_SHIFT              0
#define BCHP_MCIF_INTR2_PCI_MASK_STATUS_TX_DONE_DEFAULT            0x00000001

/***************************************************************************
 *PCI_MASK_SET - PCI interrupt Mask Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_MASK_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_reserved0_MASK                0xffc00000
#define BCHP_MCIF_INTR2_PCI_MASK_SET_reserved0_SHIFT               22

/* MCIF_INTR2 :: PCI_MASK_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_reserved1_MASK                0x003ff800
#define BCHP_MCIF_INTR2_PCI_MASK_SET_reserved1_SHIFT               11

/* MCIF_INTR2 :: PCI_MASK_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_ALL_DONE_MASK              0x00000400
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_ALL_DONE_SHIFT             10
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_ALL_DONE_DEFAULT           0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MEM_RDY_MASK               0x00000200
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MEM_RDY_SHIFT              9
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MEM_RDY_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_HW_ER_MASK               0x00000100
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_HW_ER_SHIFT              8
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_HW_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_FIFO_OF_MASK               0x00000080
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_FIFO_OF_SHIFT              7
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_FIFO_OF_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_FIFO_UF_MASK               0x00000040
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_FIFO_UF_SHIFT              6
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_FIFO_UF_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_ER_MASK                  0x00000020
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_ER_SHIFT                 5
#define BCHP_MCIF_INTR2_PCI_MASK_SET_HOST_ER_DEFAULT               0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_CARD_ER_MASK                  0x00000010
#define BCHP_MCIF_INTR2_PCI_MASK_SET_CARD_ER_SHIFT                 4
#define BCHP_MCIF_INTR2_PCI_MASK_SET_CARD_ER_DEFAULT               0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_MWPKT_MASK                 0x00000008
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_MWPKT_SHIFT                3
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_MWPKT_DEFAULT              0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MRPKT_MASK                 0x00000004
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MRPKT_SHIFT                2
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_MRPKT_DEFAULT              0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_DONE_MASK                  0x00000002
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_DONE_SHIFT                 1
#define BCHP_MCIF_INTR2_PCI_MASK_SET_RX_DONE_DEFAULT               0x00000001

/* MCIF_INTR2 :: PCI_MASK_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_DONE_MASK                  0x00000001
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_DONE_SHIFT                 0
#define BCHP_MCIF_INTR2_PCI_MASK_SET_TX_DONE_DEFAULT               0x00000001

/***************************************************************************
 *PCI_MASK_CLEAR - PCI interrupt Mask Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: PCI_MASK_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_reserved0_MASK              0xffc00000
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_reserved0_SHIFT             22

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_reserved1_MASK              0x003ff800
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_reserved1_SHIFT             11

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_ALL_DONE_MASK            0x00000400
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_ALL_DONE_SHIFT           10
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_ALL_DONE_DEFAULT         0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MEM_RDY_MASK             0x00000200
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MEM_RDY_SHIFT            9
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MEM_RDY_DEFAULT          0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_HW_ER_MASK             0x00000100
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_HW_ER_SHIFT            8
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_HW_ER_DEFAULT          0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_FIFO_OF_MASK             0x00000080
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_FIFO_OF_SHIFT            7
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_FIFO_OF_DEFAULT          0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_FIFO_UF_MASK             0x00000040
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_FIFO_UF_SHIFT            6
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_FIFO_UF_DEFAULT          0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_ER_MASK                0x00000020
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_ER_SHIFT               5
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_HOST_ER_DEFAULT             0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_CARD_ER_MASK                0x00000010
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_CARD_ER_SHIFT               4
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_CARD_ER_DEFAULT             0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_MWPKT_MASK               0x00000008
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_MWPKT_SHIFT              3
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_MWPKT_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MRPKT_MASK               0x00000004
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MRPKT_SHIFT              2
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_MRPKT_DEFAULT            0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_DONE_MASK                0x00000002
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_DONE_SHIFT               1
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_RX_DONE_DEFAULT             0x00000001

/* MCIF_INTR2 :: PCI_MASK_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_DONE_MASK                0x00000001
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_DONE_SHIFT               0
#define BCHP_MCIF_INTR2_PCI_MASK_CLEAR_TX_DONE_DEFAULT             0x00000001

/***************************************************************************
 *SCPU_STATUS - SCPU interrupt Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_reserved0_MASK                 0xffc00000
#define BCHP_MCIF_INTR2_SCPU_STATUS_reserved0_SHIFT                22

/* MCIF_INTR2 :: SCPU_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_reserved1_MASK                 0x003ff800
#define BCHP_MCIF_INTR2_SCPU_STATUS_reserved1_SHIFT                11

/* MCIF_INTR2 :: SCPU_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_ALL_DONE_MASK               0x00000400
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_ALL_DONE_SHIFT              10
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_ALL_DONE_DEFAULT            0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MEM_RDY_MASK                0x00000200
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MEM_RDY_SHIFT               9
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MEM_RDY_DEFAULT             0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_HW_ER_MASK                0x00000100
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_HW_ER_SHIFT               8
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_HW_ER_DEFAULT             0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_FIFO_OF_MASK                0x00000080
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_FIFO_OF_SHIFT               7
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_FIFO_OF_DEFAULT             0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_FIFO_UF_MASK                0x00000040
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_FIFO_UF_SHIFT               6
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_FIFO_UF_DEFAULT             0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_ER_MASK                   0x00000020
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_ER_SHIFT                  5
#define BCHP_MCIF_INTR2_SCPU_STATUS_HOST_ER_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_CARD_ER_MASK                   0x00000010
#define BCHP_MCIF_INTR2_SCPU_STATUS_CARD_ER_SHIFT                  4
#define BCHP_MCIF_INTR2_SCPU_STATUS_CARD_ER_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_MWPKT_MASK                  0x00000008
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_MWPKT_SHIFT                 3
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_MWPKT_DEFAULT               0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MRPKT_MASK                  0x00000004
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MRPKT_SHIFT                 2
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_MRPKT_DEFAULT               0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_DONE_MASK                   0x00000002
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_DONE_SHIFT                  1
#define BCHP_MCIF_INTR2_SCPU_STATUS_RX_DONE_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_DONE_MASK                   0x00000001
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_DONE_SHIFT                  0
#define BCHP_MCIF_INTR2_SCPU_STATUS_TX_DONE_DEFAULT                0x00000000

/***************************************************************************
 *SCPU_SET - SCPU interrupt Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_SET_reserved0_MASK                    0xffc00000
#define BCHP_MCIF_INTR2_SCPU_SET_reserved0_SHIFT                   22

/* MCIF_INTR2 :: SCPU_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_SET_reserved1_MASK                    0x003ff800
#define BCHP_MCIF_INTR2_SCPU_SET_reserved1_SHIFT                   11

/* MCIF_INTR2 :: SCPU_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_SET_RX_ALL_DONE_MASK                  0x00000400
#define BCHP_MCIF_INTR2_SCPU_SET_RX_ALL_DONE_SHIFT                 10
#define BCHP_MCIF_INTR2_SCPU_SET_RX_ALL_DONE_DEFAULT               0x00000000

/* MCIF_INTR2 :: SCPU_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MEM_RDY_MASK                   0x00000200
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MEM_RDY_SHIFT                  9
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MEM_RDY_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_HW_ER_MASK                   0x00000100
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_HW_ER_SHIFT                  8
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_HW_ER_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_SET_RX_FIFO_OF_MASK                   0x00000080
#define BCHP_MCIF_INTR2_SCPU_SET_RX_FIFO_OF_SHIFT                  7
#define BCHP_MCIF_INTR2_SCPU_SET_RX_FIFO_OF_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_SET_TX_FIFO_UF_MASK                   0x00000040
#define BCHP_MCIF_INTR2_SCPU_SET_TX_FIFO_UF_SHIFT                  6
#define BCHP_MCIF_INTR2_SCPU_SET_TX_FIFO_UF_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_ER_MASK                      0x00000020
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_ER_SHIFT                     5
#define BCHP_MCIF_INTR2_SCPU_SET_HOST_ER_DEFAULT                   0x00000000

/* MCIF_INTR2 :: SCPU_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_SET_CARD_ER_MASK                      0x00000010
#define BCHP_MCIF_INTR2_SCPU_SET_CARD_ER_SHIFT                     4
#define BCHP_MCIF_INTR2_SCPU_SET_CARD_ER_DEFAULT                   0x00000000

/* MCIF_INTR2 :: SCPU_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_SET_RX_MWPKT_MASK                     0x00000008
#define BCHP_MCIF_INTR2_SCPU_SET_RX_MWPKT_SHIFT                    3
#define BCHP_MCIF_INTR2_SCPU_SET_RX_MWPKT_DEFAULT                  0x00000000

/* MCIF_INTR2 :: SCPU_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MRPKT_MASK                     0x00000004
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MRPKT_SHIFT                    2
#define BCHP_MCIF_INTR2_SCPU_SET_TX_MRPKT_DEFAULT                  0x00000000

/* MCIF_INTR2 :: SCPU_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_SET_RX_DONE_MASK                      0x00000002
#define BCHP_MCIF_INTR2_SCPU_SET_RX_DONE_SHIFT                     1
#define BCHP_MCIF_INTR2_SCPU_SET_RX_DONE_DEFAULT                   0x00000000

/* MCIF_INTR2 :: SCPU_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_SET_TX_DONE_MASK                      0x00000001
#define BCHP_MCIF_INTR2_SCPU_SET_TX_DONE_SHIFT                     0
#define BCHP_MCIF_INTR2_SCPU_SET_TX_DONE_DEFAULT                   0x00000000

/***************************************************************************
 *SCPU_CLEAR - SCPU interrupt Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_reserved0_MASK                  0xffc00000
#define BCHP_MCIF_INTR2_SCPU_CLEAR_reserved0_SHIFT                 22

/* MCIF_INTR2 :: SCPU_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_reserved1_MASK                  0x003ff800
#define BCHP_MCIF_INTR2_SCPU_CLEAR_reserved1_SHIFT                 11

/* MCIF_INTR2 :: SCPU_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_ALL_DONE_MASK                0x00000400
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_ALL_DONE_SHIFT               10
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_ALL_DONE_DEFAULT             0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MEM_RDY_MASK                 0x00000200
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MEM_RDY_SHIFT                9
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MEM_RDY_DEFAULT              0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_HW_ER_MASK                 0x00000100
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_HW_ER_SHIFT                8
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_HW_ER_DEFAULT              0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_FIFO_OF_MASK                 0x00000080
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_FIFO_OF_SHIFT                7
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_FIFO_OF_DEFAULT              0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_FIFO_UF_MASK                 0x00000040
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_FIFO_UF_SHIFT                6
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_FIFO_UF_DEFAULT              0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_ER_MASK                    0x00000020
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_ER_SHIFT                   5
#define BCHP_MCIF_INTR2_SCPU_CLEAR_HOST_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_CARD_ER_MASK                    0x00000010
#define BCHP_MCIF_INTR2_SCPU_CLEAR_CARD_ER_SHIFT                   4
#define BCHP_MCIF_INTR2_SCPU_CLEAR_CARD_ER_DEFAULT                 0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_MWPKT_MASK                   0x00000008
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_MWPKT_SHIFT                  3
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_MWPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MRPKT_MASK                   0x00000004
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MRPKT_SHIFT                  2
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_MRPKT_DEFAULT                0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_DONE_MASK                    0x00000002
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_DONE_SHIFT                   1
#define BCHP_MCIF_INTR2_SCPU_CLEAR_RX_DONE_DEFAULT                 0x00000000

/* MCIF_INTR2 :: SCPU_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_DONE_MASK                    0x00000001
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_DONE_SHIFT                   0
#define BCHP_MCIF_INTR2_SCPU_CLEAR_TX_DONE_DEFAULT                 0x00000000

/***************************************************************************
 *SCPU_MASK_STATUS - SCPU interrupt Mask Status Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_MASK_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_reserved0_MASK            0xffc00000
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_reserved0_SHIFT           22

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_reserved1_MASK            0x003ff800
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_reserved1_SHIFT           11

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_ALL_DONE_MASK          0x00000400
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_ALL_DONE_SHIFT         10
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_ALL_DONE_DEFAULT       0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MEM_RDY_MASK           0x00000200
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MEM_RDY_SHIFT          9
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MEM_RDY_DEFAULT        0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_HW_ER_MASK           0x00000100
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_HW_ER_SHIFT          8
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_HW_ER_DEFAULT        0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_FIFO_OF_MASK           0x00000080
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_FIFO_OF_SHIFT          7
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_FIFO_OF_DEFAULT        0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_FIFO_UF_MASK           0x00000040
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_FIFO_UF_SHIFT          6
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_FIFO_UF_DEFAULT        0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_ER_MASK              0x00000020
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_ER_SHIFT             5
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_HOST_ER_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_CARD_ER_MASK              0x00000010
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_CARD_ER_SHIFT             4
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_CARD_ER_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_MWPKT_MASK             0x00000008
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_MWPKT_SHIFT            3
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_MWPKT_DEFAULT          0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MRPKT_MASK             0x00000004
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MRPKT_SHIFT            2
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_MRPKT_DEFAULT          0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_DONE_MASK              0x00000002
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_DONE_SHIFT             1
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_RX_DONE_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_STATUS :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_DONE_MASK              0x00000001
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_DONE_SHIFT             0
#define BCHP_MCIF_INTR2_SCPU_MASK_STATUS_TX_DONE_DEFAULT           0x00000001

/***************************************************************************
 *SCPU_MASK_SET - SCPU interrupt Mask Set Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_MASK_SET :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_reserved0_MASK               0xffc00000
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_reserved0_SHIFT              22

/* MCIF_INTR2 :: SCPU_MASK_SET :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_reserved1_MASK               0x003ff800
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_reserved1_SHIFT              11

/* MCIF_INTR2 :: SCPU_MASK_SET :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_ALL_DONE_MASK             0x00000400
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_ALL_DONE_SHIFT            10
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_ALL_DONE_DEFAULT          0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MEM_RDY_MASK              0x00000200
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MEM_RDY_SHIFT             9
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MEM_RDY_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_HW_ER_MASK              0x00000100
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_HW_ER_SHIFT             8
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_HW_ER_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_FIFO_OF_MASK              0x00000080
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_FIFO_OF_SHIFT             7
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_FIFO_OF_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_FIFO_UF_MASK              0x00000040
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_FIFO_UF_SHIFT             6
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_FIFO_UF_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_ER_MASK                 0x00000020
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_ER_SHIFT                5
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_HOST_ER_DEFAULT              0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_CARD_ER_MASK                 0x00000010
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_CARD_ER_SHIFT                4
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_CARD_ER_DEFAULT              0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_MWPKT_MASK                0x00000008
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_MWPKT_SHIFT               3
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_MWPKT_DEFAULT             0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MRPKT_MASK                0x00000004
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MRPKT_SHIFT               2
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_MRPKT_DEFAULT             0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_DONE_MASK                 0x00000002
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_DONE_SHIFT                1
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_RX_DONE_DEFAULT              0x00000001

/* MCIF_INTR2 :: SCPU_MASK_SET :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_DONE_MASK                 0x00000001
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_DONE_SHIFT                0
#define BCHP_MCIF_INTR2_SCPU_MASK_SET_TX_DONE_DEFAULT              0x00000001

/***************************************************************************
 *SCPU_MASK_CLEAR - SCPU interrupt Mask Clear Register
 ***************************************************************************/
/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: reserved0 [31:22] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_reserved0_MASK             0xffc00000
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_reserved0_SHIFT            22

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: reserved1 [21:11] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_reserved1_MASK             0x003ff800
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_reserved1_SHIFT            11

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: RX_ALL_DONE [10:10] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_ALL_DONE_MASK           0x00000400
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_ALL_DONE_SHIFT          10
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_ALL_DONE_DEFAULT        0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: TX_MEM_RDY [09:09] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MEM_RDY_MASK            0x00000200
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MEM_RDY_SHIFT           9
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MEM_RDY_DEFAULT         0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: HOST_HW_ER [08:08] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_HW_ER_MASK            0x00000100
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_HW_ER_SHIFT           8
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_HW_ER_DEFAULT         0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: RX_FIFO_OF [07:07] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_FIFO_OF_MASK            0x00000080
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_FIFO_OF_SHIFT           7
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_FIFO_OF_DEFAULT         0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: TX_FIFO_UF [06:06] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_FIFO_UF_MASK            0x00000040
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_FIFO_UF_SHIFT           6
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_FIFO_UF_DEFAULT         0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: HOST_ER [05:05] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_ER_MASK               0x00000020
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_ER_SHIFT              5
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_HOST_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: CARD_ER [04:04] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_CARD_ER_MASK               0x00000010
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_CARD_ER_SHIFT              4
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_CARD_ER_DEFAULT            0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: RX_MWPKT [03:03] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_MWPKT_MASK              0x00000008
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_MWPKT_SHIFT             3
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_MWPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: TX_MRPKT [02:02] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MRPKT_MASK              0x00000004
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MRPKT_SHIFT             2
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_MRPKT_DEFAULT           0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: RX_DONE [01:01] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_DONE_MASK               0x00000002
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_DONE_SHIFT              1
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_RX_DONE_DEFAULT            0x00000001

/* MCIF_INTR2 :: SCPU_MASK_CLEAR :: TX_DONE [00:00] */
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_DONE_MASK               0x00000001
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_DONE_SHIFT              0
#define BCHP_MCIF_INTR2_SCPU_MASK_CLEAR_TX_DONE_DEFAULT            0x00000001

#endif /* #ifndef BCHP_MCIF_INTR2_H__ */

/* End of File */
