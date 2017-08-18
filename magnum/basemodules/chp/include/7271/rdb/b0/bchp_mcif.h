/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.

 ******************************************************************************/


#ifndef BCHP_MCIF_H__
#define BCHP_MCIF_H__

/***************************************************************************
 *MCIF - M-CARD CPU Interface - Channel 0
 ***************************************************************************/
#define BCHP_MCIF_CONTROL                        0x2040b400 /* [RW][32] MCIF CONTROL REGISTER */
#define BCHP_MCIF_TX_BUF_PTR_LO                  0x2040b404 /* [RW][32] STARTING ADDRESS OF DATA TO BE WRITTEN TO M-CARD */
#define BCHP_MCIF_TX_BUF_PTR_HI                  0x2040b408 /* [RW][32] STARTING ADDRESS OF DATA TO BE WRITTEN TO M-CARD */
#define BCHP_MCIF_RX_BUF_PTR_LO                  0x2040b40c /* [RW][32] STARTING ADDRESS OF BUFFER FOR DATA READ FROM M-CARD */
#define BCHP_MCIF_RX_BUF_PTR_HI                  0x2040b410 /* [RW][32] STARTING ADDRESS OF BUFFER FOR DATA READ FROM M-CARD */
#define BCHP_MCIF_TX_LEN                         0x2040b414 /* [RW][32] M-CARD WRITE LENGTH REGISTER */
#define BCHP_MCIF_TX_CTRL                        0x2040b418 /* [RW][32] M-CARD WRITE CONTROL REGISTER */
#define BCHP_MCIF_RX_TEST                        0x2040b41c /* [RO][32] M-CARD READ DATA STATUS FOR TEST */
#define BCHP_MCIF_RX_STATUS                      0x2040b420 /* [RW][32] M-CARD HOST READY TO RECEIVE STATUS REGISTER */
#define BCHP_MCIF_RX_DATA_STATUS                 0x2040b424 /* [RO][32] M-CARD READ DATA STATUS REGISTER */
#define BCHP_MCIF_SPARE                          0x2040b428 /* [RW][32] RESERVED */

/***************************************************************************
 *CONTROL - MCIF CONTROL REGISTER
 ***************************************************************************/
/* MCIF :: CONTROL :: reserved0 [31:11] */
#define BCHP_MCIF_CONTROL_reserved0_MASK                           0xfffff800
#define BCHP_MCIF_CONTROL_reserved0_SHIFT                          11

/* MCIF :: CONTROL :: ignore_hr [10:10] */
#define BCHP_MCIF_CONTROL_ignore_hr_MASK                           0x00000400
#define BCHP_MCIF_CONTROL_ignore_hr_SHIFT                          10
#define BCHP_MCIF_CONTROL_ignore_hr_DEFAULT                        0x00000000

/* MCIF :: CONTROL :: ignore_cr [09:09] */
#define BCHP_MCIF_CONTROL_ignore_cr_MASK                           0x00000200
#define BCHP_MCIF_CONTROL_ignore_cr_SHIFT                          9
#define BCHP_MCIF_CONTROL_ignore_cr_DEFAULT                        0x00000000

/* MCIF :: CONTROL :: sisp_en [08:08] */
#define BCHP_MCIF_CONTROL_sisp_en_MASK                             0x00000100
#define BCHP_MCIF_CONTROL_sisp_en_SHIFT                            8
#define BCHP_MCIF_CONTROL_sisp_en_DEFAULT                          0x00000000

/* MCIF :: CONTROL :: sisp [07:06] */
#define BCHP_MCIF_CONTROL_sisp_MASK                                0x000000c0
#define BCHP_MCIF_CONTROL_sisp_SHIFT                               6
#define BCHP_MCIF_CONTROL_sisp_DEFAULT                             0x00000000

/* MCIF :: CONTROL :: endian_override [05:05] */
#define BCHP_MCIF_CONTROL_endian_override_MASK                     0x00000020
#define BCHP_MCIF_CONTROL_endian_override_SHIFT                    5
#define BCHP_MCIF_CONTROL_endian_override_DEFAULT                  0x00000000

/* MCIF :: CONTROL :: endian [04:04] */
#define BCHP_MCIF_CONTROL_endian_MASK                              0x00000010
#define BCHP_MCIF_CONTROL_endian_SHIFT                             4
#define BCHP_MCIF_CONTROL_endian_DEFAULT                           0x00000000

/* MCIF :: CONTROL :: tx_dis [03:03] */
#define BCHP_MCIF_CONTROL_tx_dis_MASK                              0x00000008
#define BCHP_MCIF_CONTROL_tx_dis_SHIFT                             3
#define BCHP_MCIF_CONTROL_tx_dis_DEFAULT                           0x00000000

/* MCIF :: CONTROL :: rx_dis [02:02] */
#define BCHP_MCIF_CONTROL_rx_dis_MASK                              0x00000004
#define BCHP_MCIF_CONTROL_rx_dis_SHIFT                             2
#define BCHP_MCIF_CONTROL_rx_dis_DEFAULT                           0x00000000

/* MCIF :: CONTROL :: sclk_en [01:01] */
#define BCHP_MCIF_CONTROL_sclk_en_MASK                             0x00000002
#define BCHP_MCIF_CONTROL_sclk_en_SHIFT                            1
#define BCHP_MCIF_CONTROL_sclk_en_DEFAULT                          0x00000000

/* MCIF :: CONTROL :: sctl_en [00:00] */
#define BCHP_MCIF_CONTROL_sctl_en_MASK                             0x00000001
#define BCHP_MCIF_CONTROL_sctl_en_SHIFT                            0
#define BCHP_MCIF_CONTROL_sctl_en_DEFAULT                          0x00000000

/***************************************************************************
 *TX_BUF_PTR_LO - STARTING ADDRESS OF DATA TO BE WRITTEN TO M-CARD
 ***************************************************************************/
/* MCIF :: TX_BUF_PTR_LO :: tx_buf_ptr_lo [31:00] */
#define BCHP_MCIF_TX_BUF_PTR_LO_tx_buf_ptr_lo_MASK                 0xffffffff
#define BCHP_MCIF_TX_BUF_PTR_LO_tx_buf_ptr_lo_SHIFT                0
#define BCHP_MCIF_TX_BUF_PTR_LO_tx_buf_ptr_lo_DEFAULT              0x00000000

/***************************************************************************
 *TX_BUF_PTR_HI - STARTING ADDRESS OF DATA TO BE WRITTEN TO M-CARD
 ***************************************************************************/
/* MCIF :: TX_BUF_PTR_HI :: reserved0 [31:08] */
#define BCHP_MCIF_TX_BUF_PTR_HI_reserved0_MASK                     0xffffff00
#define BCHP_MCIF_TX_BUF_PTR_HI_reserved0_SHIFT                    8

/* MCIF :: TX_BUF_PTR_HI :: tx_buf_ptr_hi [07:00] */
#define BCHP_MCIF_TX_BUF_PTR_HI_tx_buf_ptr_hi_MASK                 0x000000ff
#define BCHP_MCIF_TX_BUF_PTR_HI_tx_buf_ptr_hi_SHIFT                0
#define BCHP_MCIF_TX_BUF_PTR_HI_tx_buf_ptr_hi_DEFAULT              0x00000000

/***************************************************************************
 *RX_BUF_PTR_LO - STARTING ADDRESS OF BUFFER FOR DATA READ FROM M-CARD
 ***************************************************************************/
/* MCIF :: RX_BUF_PTR_LO :: rx_buf_ptr_lo [31:00] */
#define BCHP_MCIF_RX_BUF_PTR_LO_rx_buf_ptr_lo_MASK                 0xffffffff
#define BCHP_MCIF_RX_BUF_PTR_LO_rx_buf_ptr_lo_SHIFT                0
#define BCHP_MCIF_RX_BUF_PTR_LO_rx_buf_ptr_lo_DEFAULT              0x00000000

/***************************************************************************
 *RX_BUF_PTR_HI - STARTING ADDRESS OF BUFFER FOR DATA READ FROM M-CARD
 ***************************************************************************/
/* MCIF :: RX_BUF_PTR_HI :: reserved0 [31:08] */
#define BCHP_MCIF_RX_BUF_PTR_HI_reserved0_MASK                     0xffffff00
#define BCHP_MCIF_RX_BUF_PTR_HI_reserved0_SHIFT                    8

/* MCIF :: RX_BUF_PTR_HI :: rx_buf_ptr_hi [07:00] */
#define BCHP_MCIF_RX_BUF_PTR_HI_rx_buf_ptr_hi_MASK                 0x000000ff
#define BCHP_MCIF_RX_BUF_PTR_HI_rx_buf_ptr_hi_SHIFT                0
#define BCHP_MCIF_RX_BUF_PTR_HI_rx_buf_ptr_hi_DEFAULT              0x00000000

/***************************************************************************
 *TX_LEN - M-CARD WRITE LENGTH REGISTER
 ***************************************************************************/
/* MCIF :: TX_LEN :: reserved0 [31:13] */
#define BCHP_MCIF_TX_LEN_reserved0_MASK                            0xffffe000
#define BCHP_MCIF_TX_LEN_reserved0_SHIFT                           13

/* MCIF :: TX_LEN :: tx_len [12:00] */
#define BCHP_MCIF_TX_LEN_tx_len_MASK                               0x00001fff
#define BCHP_MCIF_TX_LEN_tx_len_SHIFT                              0
#define BCHP_MCIF_TX_LEN_tx_len_DEFAULT                            0x00000000

/***************************************************************************
 *TX_CTRL - M-CARD WRITE CONTROL REGISTER
 ***************************************************************************/
/* MCIF :: TX_CTRL :: reserved0 [31:09] */
#define BCHP_MCIF_TX_CTRL_reserved0_MASK                           0xfffffe00
#define BCHP_MCIF_TX_CTRL_reserved0_SHIFT                          9

/* MCIF :: TX_CTRL :: go [08:08] */
#define BCHP_MCIF_TX_CTRL_go_MASK                                  0x00000100
#define BCHP_MCIF_TX_CTRL_go_SHIFT                                 8
#define BCHP_MCIF_TX_CTRL_go_DEFAULT                               0x00000000

/* MCIF :: TX_CTRL :: host_x7 [07:07] */
#define BCHP_MCIF_TX_CTRL_host_x7_MASK                             0x00000080
#define BCHP_MCIF_TX_CTRL_host_x7_SHIFT                            7
#define BCHP_MCIF_TX_CTRL_host_x7_DEFAULT                          0x00000000

/* MCIF :: TX_CTRL :: reserved1 [06:06] */
#define BCHP_MCIF_TX_CTRL_reserved1_MASK                           0x00000040
#define BCHP_MCIF_TX_CTRL_reserved1_SHIFT                          6

/* MCIF :: TX_CTRL :: host_ec [05:05] */
#define BCHP_MCIF_TX_CTRL_host_ec_MASK                             0x00000020
#define BCHP_MCIF_TX_CTRL_host_ec_SHIFT                            5
#define BCHP_MCIF_TX_CTRL_host_ec_DEFAULT                          0x00000000

/* MCIF :: TX_CTRL :: host_bl [04:04] */
#define BCHP_MCIF_TX_CTRL_host_bl_MASK                             0x00000010
#define BCHP_MCIF_TX_CTRL_host_bl_SHIFT                            4
#define BCHP_MCIF_TX_CTRL_host_bl_DEFAULT                          0x00000000

/* MCIF :: TX_CTRL :: host_bf [03:03] */
#define BCHP_MCIF_TX_CTRL_host_bf_MASK                             0x00000008
#define BCHP_MCIF_TX_CTRL_host_bf_SHIFT                            3
#define BCHP_MCIF_TX_CTRL_host_bf_DEFAULT                          0x00000000

/* MCIF :: TX_CTRL :: reserved2 [02:02] */
#define BCHP_MCIF_TX_CTRL_reserved2_MASK                           0x00000004
#define BCHP_MCIF_TX_CTRL_reserved2_SHIFT                          2

/* MCIF :: TX_CTRL :: force_er [01:01] */
#define BCHP_MCIF_TX_CTRL_force_er_MASK                            0x00000002
#define BCHP_MCIF_TX_CTRL_force_er_SHIFT                           1
#define BCHP_MCIF_TX_CTRL_force_er_DEFAULT                         0x00000000

/* MCIF :: TX_CTRL :: host_x0 [00:00] */
#define BCHP_MCIF_TX_CTRL_host_x0_MASK                             0x00000001
#define BCHP_MCIF_TX_CTRL_host_x0_SHIFT                            0
#define BCHP_MCIF_TX_CTRL_host_x0_DEFAULT                          0x00000000

/***************************************************************************
 *RX_TEST - M-CARD READ DATA STATUS FOR TEST
 ***************************************************************************/
/* MCIF :: RX_TEST :: reserved0 [31:23] */
#define BCHP_MCIF_RX_TEST_reserved0_MASK                           0xff800000
#define BCHP_MCIF_RX_TEST_reserved0_SHIFT                          23

/* MCIF :: RX_TEST :: rx_test_iqb [22:17] */
#define BCHP_MCIF_RX_TEST_rx_test_iqb_MASK                         0x007e0000
#define BCHP_MCIF_RX_TEST_rx_test_iqb_SHIFT                        17

/* MCIF :: RX_TEST :: reserved1 [16:13] */
#define BCHP_MCIF_RX_TEST_reserved1_MASK                           0x0001e000
#define BCHP_MCIF_RX_TEST_reserved1_SHIFT                          13

/* MCIF :: RX_TEST :: rx_test_len [12:00] */
#define BCHP_MCIF_RX_TEST_rx_test_len_MASK                         0x00001fff
#define BCHP_MCIF_RX_TEST_rx_test_len_SHIFT                        0
#define BCHP_MCIF_RX_TEST_rx_test_len_DEFAULT                      0x00000000

/***************************************************************************
 *RX_STATUS - M-CARD HOST READY TO RECEIVE STATUS REGISTER
 ***************************************************************************/
/* MCIF :: RX_STATUS :: reserved0 [31:01] */
#define BCHP_MCIF_RX_STATUS_reserved0_MASK                         0xfffffffe
#define BCHP_MCIF_RX_STATUS_reserved0_SHIFT                        1

/* MCIF :: RX_STATUS :: host_hr [00:00] */
#define BCHP_MCIF_RX_STATUS_host_hr_MASK                           0x00000001
#define BCHP_MCIF_RX_STATUS_host_hr_SHIFT                          0
#define BCHP_MCIF_RX_STATUS_host_hr_DEFAULT                        0x00000000

/***************************************************************************
 *RX_DATA_STATUS - M-CARD READ DATA STATUS REGISTER
 ***************************************************************************/
/* MCIF :: RX_DATA_STATUS :: reserved0 [31:22] */
#define BCHP_MCIF_RX_DATA_STATUS_reserved0_MASK                    0xffc00000
#define BCHP_MCIF_RX_DATA_STATUS_reserved0_SHIFT                   22

/* MCIF :: RX_DATA_STATUS :: card_iqb [21:17] */
#define BCHP_MCIF_RX_DATA_STATUS_card_iqb_MASK                     0x003e0000
#define BCHP_MCIF_RX_DATA_STATUS_card_iqb_SHIFT                    17
#define BCHP_MCIF_RX_DATA_STATUS_card_iqb_DEFAULT                  0x00000000

/* MCIF :: RX_DATA_STATUS :: reserved1 [16:13] */
#define BCHP_MCIF_RX_DATA_STATUS_reserved1_MASK                    0x0001e000
#define BCHP_MCIF_RX_DATA_STATUS_reserved1_SHIFT                   13

/* MCIF :: RX_DATA_STATUS :: rx_data_len [12:00] */
#define BCHP_MCIF_RX_DATA_STATUS_rx_data_len_MASK                  0x00001fff
#define BCHP_MCIF_RX_DATA_STATUS_rx_data_len_SHIFT                 0
#define BCHP_MCIF_RX_DATA_STATUS_rx_data_len_DEFAULT               0x00000000

/***************************************************************************
 *SPARE - RESERVED
 ***************************************************************************/
/* MCIF :: SPARE :: reserved0 [31:16] */
#define BCHP_MCIF_SPARE_reserved0_MASK                             0xffff0000
#define BCHP_MCIF_SPARE_reserved0_SHIFT                            16

/* MCIF :: SPARE :: SPARE [15:06] */
#define BCHP_MCIF_SPARE_SPARE_MASK                                 0x0000ffc0
#define BCHP_MCIF_SPARE_SPARE_SHIFT                                6
#define BCHP_MCIF_SPARE_SPARE_DEFAULT                              0x00000000

/* MCIF :: SPARE :: enable_byte_mask [05:05] */
#define BCHP_MCIF_SPARE_enable_byte_mask_MASK                      0x00000020
#define BCHP_MCIF_SPARE_enable_byte_mask_SHIFT                     5
#define BCHP_MCIF_SPARE_enable_byte_mask_DEFAULT                   0x00000000

/* MCIF :: SPARE :: bus_endianess [04:04] */
#define BCHP_MCIF_SPARE_bus_endianess_MASK                         0x00000010
#define BCHP_MCIF_SPARE_bus_endianess_SHIFT                        4
#define BCHP_MCIF_SPARE_bus_endianess_DEFAULT                      0x00000000

/* MCIF :: SPARE :: undo_last_scb_wr_req_dly [03:03] */
#define BCHP_MCIF_SPARE_undo_last_scb_wr_req_dly_MASK              0x00000008
#define BCHP_MCIF_SPARE_undo_last_scb_wr_req_dly_SHIFT             3
#define BCHP_MCIF_SPARE_undo_last_scb_wr_req_dly_DEFAULT           0x00000000

/* MCIF :: SPARE :: undo_2nd_scb_rd_req_dly [02:02] */
#define BCHP_MCIF_SPARE_undo_2nd_scb_rd_req_dly_MASK               0x00000004
#define BCHP_MCIF_SPARE_undo_2nd_scb_rd_req_dly_SHIFT              2
#define BCHP_MCIF_SPARE_undo_2nd_scb_rd_req_dly_DEFAULT            0x00000000

/* MCIF :: SPARE :: undo_fix_card_len [01:01] */
#define BCHP_MCIF_SPARE_undo_fix_card_len_MASK                     0x00000002
#define BCHP_MCIF_SPARE_undo_fix_card_len_SHIFT                    1
#define BCHP_MCIF_SPARE_undo_fix_card_len_DEFAULT                  0x00000000

/* MCIF :: SPARE :: undo_fix_rx_clr [00:00] */
#define BCHP_MCIF_SPARE_undo_fix_rx_clr_MASK                       0x00000001
#define BCHP_MCIF_SPARE_undo_fix_rx_clr_SHIFT                      0
#define BCHP_MCIF_SPARE_undo_fix_rx_clr_DEFAULT                    0x00000000

#endif /* #ifndef BCHP_MCIF_H__ */

/* End of File */
