/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on              Mon Sep 23 09:50:36 2013
 *                 Full Compile MD5 Checksum fcccce298b546dd6a1f4cbad288478da
 *                   (minus title and desc)  
 *                 MD5 Checksum              211556602e37a33262598b3d5eeba81c
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_XPT_MEMDMA_MCPB_CH19_H__
#define BCHP_XPT_MEMDMA_MCPB_CH19_H__

/***************************************************************************
 *XPT_MEMDMA_MCPB_CH19 - MCPB Channel 19 Configuration
 ***************************************************************************/
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_CONTROL 0x00a63200 /* MCPB Channel x Descriptor control information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DATA_CONTROL 0x00a63204 /* MCPB Channel x Data control information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_CURR_DESC_ADDRESS 0x00a63208 /* MCPB Channel x Current Descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_NEXT_DESC_ADDRESS 0x00a6320c /* MCPB Channel x Next Descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_BASE_ADDRESS_UPPER 0x00a63210 /* MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_BASE_ADDRESS_LOWER 0x00a63214 /* MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_END_ADDRESS_UPPER 0x00a63218 /* MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_END_ADDRESS_LOWER 0x00a6321c /* MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_CURR_RD_ADDRESS_UPPER 0x00a63220 /* MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_CURR_RD_ADDRESS_LOWER 0x00a63224 /* MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_WRITE_ADDRESS_UPPER 0x00a63228 /* MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BUFF_WRITE_ADDRESS_LOWER 0x00a6322c /* MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_STATUS_0   0x00a63230 /* MCPB Channel x Status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_STATUS_1   0x00a63234 /* MCPB Channel x CRC value */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_STATUS_2   0x00a63238 /* MCPB Channel x Manual mode status */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_STATUS_0 0x00a6323c /* MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_STATUS_1 0x00a63240 /* MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_CURR_DESC_ADDR 0x00a63244 /* MCPB Channel x  Descriptor Slot 0 Current Descriptor Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_CURR_DATA_ADDR_UPPER 0x00a63248 /* MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_CURR_DATA_ADDR_LOWER 0x00a6324c /* MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_NEXT_TIMESTAMP 0x00a63250 /* MCPB Channel x Descriptor Slot 0 Next Packet Timestamp */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT0_PKT2PKT_TIMESTAMP_DELTA 0x00a63254 /* MCPB Channel x Descriptor Slot 0 Packet to packet Timestamp delta */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_STATUS_0 0x00a63258 /* MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_STATUS_1 0x00a6325c /* MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_CURR_DESC_ADDR 0x00a63260 /* MCPB Channel x  Descriptor Slot 1 Current Descriptor Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_CURR_DATA_ADDR_UPPER 0x00a63264 /* MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_CURR_DATA_ADDR_LOWER 0x00a63268 /* MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_NEXT_TIMESTAMP 0x00a6326c /* MCPB Channel x Descriptor Slot 1 Next Packet Timestamp */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_DESC_SLOT1_PKT2PKT_TIMESTAMP_DELTA 0x00a63270 /* MCPB Channel x Descriptor Slot 1 Packet to packet Timestamp delta */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_PKT_LEN     0x00a63274 /* MCPB Channel x Packet length control */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_PARSER_CTRL 0x00a63278 /* MCPB Channel x Parser control */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_PARSER_CTRL1 0x00a6327c /* MCPB Channel x Parser control 1 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_TS_CONFIG   0x00a63280 /* MCPB Channel x TS Configuration */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_PES_ES_CONFIG 0x00a63284 /* MCPB Channel x PES and ES Configuration */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_PES_SYNC_COUNTER 0x00a63288 /* MCPB Channel x PES Sync counter */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_ASF_CONFIG  0x00a6328c /* MCPB Channel x ASF Configuration */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_0 0x00a63290 /* MCPB Channel x Stream Processor State Register 0 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_1 0x00a63294 /* MCPB Channel x Stream Processor State Register 1 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_2 0x00a63298 /* MCPB Channel x Stream Processor State Register 2 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_3 0x00a6329c /* MCPB Channel x Stream Processor State Register 3 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_4 0x00a632a0 /* MCPB Channel x Stream Processor State Register 4 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_5 0x00a632a4 /* MCPB Channel x Stream Processor State Register 5 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_6 0x00a632a8 /* MCPB Channel x Stream Processor State Register 6 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_7 0x00a632ac /* MCPB Channel x Stream Processor State Register 7 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_8 0x00a632b0 /* MCPB Channel x Stream Processor State Register 8 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_9 0x00a632b4 /* MCPB Channel x Stream Processor State Register 9 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_10 0x00a632b8 /* MCPB Channel x Stream Processor State Register 10 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_11 0x00a632bc /* MCPB Channel x Stream Processor State Register 11 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_12 0x00a632c0 /* MCPB Channel x Stream Processor State Register 12 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_SP_STATE_REG_13 0x00a632c4 /* MCPB Channel x Stream Processor State Register 13 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF_CTRL 0x00a632c8 /* MCPB Channel x Burst buffer control */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF_CRC  0x00a632cc /* MCPB Channel x Current CRC value */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF0_RW_STATUS 0x00a632d0 /* MCPB Channel x Burst buffer 0 data specific information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF0_RO_STATUS 0x00a632d4 /* MCPB Channel x Burst buffer 0 control specific information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF1_RW_STATUS 0x00a632d8 /* MCPB Channel x Burst buffer 1 data specific information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DMA_BBUFF1_RO_STATUS 0x00a632dc /* MCPB Channel x Burst buffer 1 control specific information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_BLOCKOUT_CTRL 0x00a632e0 /* MCPB Channel x Blockout control information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_NEXT_BO_MON 0x00a632e4 /* MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_CTRL 0x00a632e8 /* MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_REF_DIFF_VALUE_TS_MBOX 0x00a632ec /* MCPB Channel x reference difference value and next Timestamp information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TS_ERR_BOUND_EARLY 0x00a632f0 /* MCPB Channel x TS error bound early information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TS_ERR_BOUND_LATE 0x00a632f4 /* MCPB Channel x TS error bound late information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_NEXT_GPC_MON 0x00a632f8 /* MCPB Channel x next Global Pacing Counter and Timestamp monitor information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_REF_DIFF_VALUE_SIGN 0x00a632fc /* MCPB Channel x reference difference value sign information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_PES_PACING_CTRL 0x00a63300 /* MCPB Channel x PES pacing control information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_SLOT_STATUS 0x00a63304 /* MCPB Channel x Slot 0 and Slot 1 information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_SLOT0_REG1 0x00a63308 /* MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_SLOT0_REG2 0x00a6330c /* MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_SLOT1_REG1 0x00a63310 /* MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_SLOT1_REG2 0x00a63314 /* MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_LAST_TIMESTAMP_DELTA 0x00a63318 /* MCPB Channel x last TS delta value */
#define BCHP_XPT_MEMDMA_MCPB_CH19_TMEU_TIMING_INFO_LAST_NEXT_TIMESTAMP 0x00a6331c /* MCPB Channel x last NEXT TS value */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_STATUS    0x00a63320 /* MCPB Channel x DCPM status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DESC_ADDR 0x00a63324 /* MCPB Channel x DCPM descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DESC_DONE_INT_ADDR 0x00a63328 /* MCPB Channel x DCPM descriptor done interrupt address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_PAUSE_AFTER_GROUP_PACKETS_CTRL 0x00a6332c /* MCPB Channel x Pause after group of packets control information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_PAUSE_AFTER_GROUP_PACKETS_PKT_COUNTER 0x00a63330 /* MCPB Channel x Pause after group of packets local packet counter */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_LOCAL_PACKET_COUNTER 0x00a63334 /* MCPB Channel x local packet counter */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_UPPER 0x00a63338 /* MCPB Channel x DCPM data address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_LOWER 0x00a6333c /* MCPB Channel x DCPM data address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_CURR_DESC_ADDR 0x00a63340 /* MCPB Channel x DCPM current descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_SLOT_STATUS 0x00a63344 /* MCPB Channel x DCPM slot status information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DESC_ADDR_SLOT_0 0x00a63348 /* MCPB Channel x DCPM completed slot 0 descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_SLOT_0_UPPER 0x00a6334c /* MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_SLOT_0_LOWER 0x00a63350 /* MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DESC_ADDR_SLOT_1 0x00a63354 /* MCPB Channel x DCPM completed slot 1 descriptor address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_SLOT_1_UPPER 0x00a63358 /* MCPB Channel x DCPM completed slot 1 data address information */
#define BCHP_XPT_MEMDMA_MCPB_CH19_DCPM_DATA_ADDR_SLOT_1_LOWER 0x00a6335c /* MCPB Channel x DCPM completed slot 1 data address information */

#endif /* #ifndef BCHP_XPT_MEMDMA_MCPB_CH19_H__ */

/* End of File */
