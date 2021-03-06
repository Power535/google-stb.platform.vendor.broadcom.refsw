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
 * Date:           Generated on               Thu Dec  4 13:59:03 2014
 *                 Full Compile MD5 Checksum  7f8620d6db569529bdb0529f9fa1fcf2
 *                     (minus title and desc)
 *                 MD5 Checksum               48c8fdf2af4291bac0fa2b0d5245d05c
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15262
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_H__
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_H__

/***************************************************************************
 *RAAGA_DSP_MEM_SUBSYSTEM_1 - Raaga Dsp Memory Subsystem
 ***************************************************************************/
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_ID2R_RDATA 0x00d30000 /* [RO] Peek and Poke IMEM/DMEM RBUS Read Data */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_WDATA 0x00d30004 /* [RW] Peek and Poke IMEM/DMEM RBUS Write Data */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_CMD  0x00d30008 /* [RW] Peek and Poke Command Register  for IMEM and DMEM */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_ADDR 0x00d3000c /* [RW] IMEM or DMEM Address for peek/poke access */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_AUTO 0x00d30010 /* [RW] Set Peek/poke in Auto increment mode */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_AUTO_RD_TRIG 0x00d30014 /* [RW] Peek trigger in auto inc mode */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_R2ID_AUTO_ADDRESS_STATUS 0x00d30018 /* [RO] Current address for peek/poke operation in auto inc mode. */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_FP_STALL_ALLOW 0x00d30040 /* [RW] FP can stall IMEM for this many clock cycles */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_DMA_ALLOW 0x00d30044 /* [RW] DMA priority is raised over FP for this many clock cycles */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_IMEM_STATUS 0x00d30048 /* [RO] IMEM STATUS Register */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_DMEM_STATUS 0x00d3004c /* [RO] DMEM STATUS Register */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOM_MISS_STATUS 0x00d30050 /* [RO] VOM MISS STATUS Register */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_CNTRL 0x00d30070 /* [RW] MRU Control Register */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_0 0x00d30074 /* [RO] MRU_Page_0 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_1 0x00d30078 /* [RO] MRU_Page_1 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_2 0x00d3007c /* [RO] MRU_Page_2 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_3 0x00d30080 /* [RO] MRU_Page_3 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_4 0x00d30084 /* [RO] MRU_Page_4 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_5 0x00d30088 /* [RO] MRU_Page_5 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_6 0x00d3008c /* [RO] MRU_Page_6 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MRU_PAGE_7 0x00d30090 /* [RO] MRU_Page_7 */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_PC_STATUS 0x00d30094 /* [RO] FP_PC_STATUS */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_INST_OUT_LOWER 0x00d3009c /* [RO] INST_OUT_LOWER_HALF */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_INST_OUT_UPPER 0x00d300a0 /* [RO] INST_OUT_UPPER_HALF */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_INST_OUT_READY 0x00d300a4 /* [RO] INST_OUT_READY */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MEMSUB_ERROR_STATUS 0x00d300a8 /* [RO] MEMSUB_ERROR_STATUS */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MEMSUB_ERROR_CLEAR 0x00d300ac /* [WO] MEMSUB_ERROR_CLEAR */

/***************************************************************************
 *MDMEMRAM%i - MDMEM RAM
 ***************************************************************************/
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_ARRAY_BASE        0x00d34000
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_ARRAY_START       0
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_ARRAY_END         511
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_ARRAY_ELEMENT_SIZE 32

/***************************************************************************
 *MDMEMRAM%i - MDMEM RAM
 ***************************************************************************/
/* RAAGA_DSP_MEM_SUBSYSTEM_1 :: MDMEMRAMi :: reserved0 [31:08] */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_reserved0_MASK    0xffffff00
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_reserved0_SHIFT   8

/* RAAGA_DSP_MEM_SUBSYSTEM_1 :: MDMEMRAMi :: dmem_index [07:00] */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_dmem_index_MASK   0x000000ff
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_MDMEMRAMi_dmem_index_SHIFT  0


/***************************************************************************
 *VOMRAM%i - VOM RAM
 ***************************************************************************/
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_ARRAY_BASE          0x00d38000
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_ARRAY_START         0
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_ARRAY_END           4095
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_ARRAY_ELEMENT_SIZE  32

/***************************************************************************
 *VOMRAM%i - VOM RAM
 ***************************************************************************/
/* RAAGA_DSP_MEM_SUBSYSTEM_1 :: VOMRAMi :: reserved0 [31:08] */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_reserved0_MASK      0xffffff00
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_reserved0_SHIFT     8

/* RAAGA_DSP_MEM_SUBSYSTEM_1 :: VOMRAMi :: imem_index [07:00] */
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_imem_index_MASK     0x000000ff
#define BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_VOMRAMi_imem_index_SHIFT    0


#endif /* #ifndef BCHP_RAAGA_DSP_MEM_SUBSYSTEM_1_H__ */

/* End of File */
