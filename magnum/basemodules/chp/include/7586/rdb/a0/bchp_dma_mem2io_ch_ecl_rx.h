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
 * Date:           Generated on               Wed Feb 11 10:13:59 2015
 *                 Full Compile MD5 Checksum  f7f4bd55341805fcfe958ba5e47e65f4
 *                     (minus title and desc)
 *                 MD5 Checksum               95b679a9655597a92593cae55222c397
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_DMA_MEM2IO_CH_ECL_RX_H__
#define BCHP_DMA_MEM2IO_CH_ECL_RX_H__

/***************************************************************************
 *DMA_MEM2IO_CH_ECL_RX - DMA_MEM2IO_CH registers
 ***************************************************************************/
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ADDR   0x00fed800 /* [RW] LLM Command Address */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DESC_BASE_ADDR 0x00fed804 /* [RW] The physical base address of the descriptors in memory */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_BUFF_BASE_ADDR 0x00fed808 /* [RW] The physical base address of the buffers in  memory */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL     0x00fed80c /* [RW] Channel Control Register */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD        0x00fed810 /* [RW] "LLM Command Register.,Contains set of commands/parameters DMA should pass to LLM while issuing various LLM commands." */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1     0x00fed814 /* [RO] Debug Register #1 */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2     0x00fed818 /* [RO] Debug Register #2 */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3     0x00fed81c /* [RO] Debug Register #3 */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4     0x00fed820 /* [RO] "Debug Register #4,Reflects parameters of fetched descriptor" */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5     0x00fed824 /* [RO] "Debug Register #5,Reflects parameters of fetched descriptor" */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6     0x00fed828 /* [RO] "Debug Register #6,Reflects parameters of fetched descriptor" */

/***************************************************************************
 *LLM_CMD_ADDR - LLM Command Address
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: LLM_CMD_ADDR :: llm_cmd_addr_val [31:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ADDR_llm_cmd_addr_val_MASK 0xffffffff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ADDR_llm_cmd_addr_val_SHIFT 0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ADDR_llm_cmd_addr_val_DEFAULT 0x00000000

/***************************************************************************
 *DESC_BASE_ADDR - The physical base address of the descriptors in memory
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DESC_BASE_ADDR :: desc_base_addr_val [31:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DESC_BASE_ADDR_desc_base_addr_val_MASK 0xffffffff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DESC_BASE_ADDR_desc_base_addr_val_SHIFT 0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DESC_BASE_ADDR_desc_base_addr_val_DEFAULT 0x00000000

/***************************************************************************
 *BUFF_BASE_ADDR - The physical base address of the buffers in  memory
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: BUFF_BASE_ADDR :: buff_base_addr_val [31:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_BUFF_BASE_ADDR_buff_base_addr_val_MASK 0xffffffff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_BUFF_BASE_ADDR_buff_base_addr_val_SHIFT 0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_BUFF_BASE_ADDR_buff_base_addr_val_DEFAULT 0x00000000

/***************************************************************************
 *CH_CONTROL - Channel Control Register
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: reserved0 [31:31] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_reserved0_MASK        0x80000000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_reserved0_SHIFT       31

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: rate_limit_bus_overhead [30:29] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_bus_overhead_MASK 0x60000000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_bus_overhead_SHIFT 29
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_bus_overhead_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: rate_limit_credit [28:17] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_credit_MASK 0x1ffe0000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_credit_SHIFT 17
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_credit_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: rate_limit_period [16:04] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_period_MASK 0x0001fff0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_period_SHIFT 4
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_period_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: rate_limit_en [03:03] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_en_MASK    0x00000008
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_en_SHIFT   3
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_rate_limit_en_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: burst_size [02:01] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_burst_size_MASK       0x00000006
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_burst_size_SHIFT      1
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_burst_size_DEFAULT    0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: CH_CONTROL :: enable [00:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_enable_MASK           0x00000001
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_enable_SHIFT          0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_CH_CONTROL_enable_DEFAULT        0x00000000

/***************************************************************************
 *LLM_CMD - "LLM Command Register.,Contains set of commands/parameters DMA should pass to LLM while issuing various LLM commands."
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: LLM_CMD :: reserved0 [31:12] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_reserved0_MASK           0xfffff000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_reserved0_SHIFT          12

/* DMA_MEM2IO_CH_ECL_RX :: LLM_CMD :: assign_packet_seq [11:11] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_assign_packet_seq_MASK   0x00000800
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_assign_packet_seq_SHIFT  11
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_assign_packet_seq_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: LLM_CMD :: ch_qid [10:01] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ch_qid_MASK              0x000007fe
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ch_qid_SHIFT             1
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_ch_qid_DEFAULT           0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: LLM_CMD :: llm_cmd_en [00:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_llm_cmd_en_MASK          0x00000001
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_llm_cmd_en_SHIFT         0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_LLM_CMD_llm_cmd_en_DEFAULT       0x00000000

/***************************************************************************
 *DEBUG_REG1 - Debug Register #1
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: reserved0 [31:19] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_reserved0_MASK        0xfff80000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_reserved0_SHIFT       19

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: fsm_state [18:16] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fsm_state_MASK        0x00070000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fsm_state_SHIFT       16
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fsm_state_DEFAULT     0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: descriptor_empty [15:15] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_empty_MASK 0x00008000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_empty_SHIFT 15
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_empty_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: descriptor_available [14:14] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_available_MASK 0x00004000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_available_SHIFT 14
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_descriptor_available_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: link_ptr [13:01] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_link_ptr_MASK         0x00003ffe
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_link_ptr_SHIFT        1
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_link_ptr_DEFAULT      0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG1 :: fifo_ready [00:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fifo_ready_MASK       0x00000001
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fifo_ready_SHIFT      0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG1_fifo_ready_DEFAULT    0x00000000

/***************************************************************************
 *DEBUG_REG2 - Debug Register #2
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG2 :: reserved0 [31:20] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_reserved0_MASK        0xfff00000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_reserved0_SHIFT       20

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG2 :: desc_byte_cnt [19:09] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_byte_cnt_MASK    0x000ffe00
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_byte_cnt_SHIFT   9
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_byte_cnt_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG2 :: desc_remaining_words [08:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_remaining_words_MASK 0x000001ff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_remaining_words_SHIFT 0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG2_desc_remaining_words_DEFAULT 0x00000000

/***************************************************************************
 *DEBUG_REG3 - Debug Register #3
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG3 :: reserved0 [31:23] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_reserved0_MASK        0xff800000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_reserved0_SHIFT       23

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG3 :: ll_desc_cnt [22:13] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_desc_cnt_MASK      0x007fe000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_desc_cnt_SHIFT     13
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_desc_cnt_DEFAULT   0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG3 :: ll_buff_desc_cnt [12:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_buff_desc_cnt_MASK 0x00001fff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_buff_desc_cnt_SHIFT 0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG3_ll_buff_desc_cnt_DEFAULT 0x00000000

/***************************************************************************
 *DEBUG_REG4 - "Debug Register #4,Reflects parameters of fetched descriptor"
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG4 :: reserved0 [31:26] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_reserved0_MASK        0xfc000000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_reserved0_SHIFT       26

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG4 :: desc_next_pld_ptr [25:13] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_pld_ptr_MASK 0x03ffe000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_pld_ptr_SHIFT 13
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_pld_ptr_DEFAULT 0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG4 :: desc_next_ptr [12:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_ptr_MASK    0x00001fff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_ptr_SHIFT   0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG4_desc_next_ptr_DEFAULT 0x00000000

/***************************************************************************
 *DEBUG_REG5 - "Debug Register #5,Reflects parameters of fetched descriptor"
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG5 :: desc_seq_num [31:16] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_seq_num_MASK     0xffff0000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_seq_num_SHIFT    16
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_seq_num_DEFAULT  0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG5 :: desc_buff_addr [15:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_buff_addr_MASK   0x0000ffff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_buff_addr_SHIFT  0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG5_desc_buff_addr_DEFAULT 0x00000000

/***************************************************************************
 *DEBUG_REG6 - "Debug Register #6,Reflects parameters of fetched descriptor"
 ***************************************************************************/
/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG6 :: reserved0 [31:23] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_reserved0_MASK        0xff800000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_reserved0_SHIFT       23

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG6 :: fwd_queue_id [22:13] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_fwd_queue_id_MASK     0x007fe000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_fwd_queue_id_SHIFT    13
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_fwd_queue_id_DEFAULT  0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG6 :: desc_pfd [12:12] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pfd_MASK         0x00001000
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pfd_SHIFT        12
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pfd_DEFAULT      0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG6 :: desc_pld [11:11] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pld_MASK         0x00000800
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pld_SHIFT        11
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_pld_DEFAULT      0x00000000

/* DMA_MEM2IO_CH_ECL_RX :: DEBUG_REG6 :: desc_data_len [10:00] */
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_data_len_MASK    0x000007ff
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_data_len_SHIFT   0
#define BCHP_DMA_MEM2IO_CH_ECL_RX_DEBUG_REG6_desc_data_len_DEFAULT 0x00000000

#endif /* #ifndef BCHP_DMA_MEM2IO_CH_ECL_RX_H__ */

/* End of File */
