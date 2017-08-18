/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Mon Sep 12 14:24:32 2016
 *                 Full Compile MD5 Checksum  2d2ed423991a1e4e4d03ca98bc390b2c
 *                     (minus title and desc)
 *                 MD5 Checksum               8cad5c3953d7e5df4439153720b00628
 *
 * lock_release:   r_1099
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1119
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_BSP_GLB_CONTROL_H__
#define BCHP_BSP_GLB_CONTROL_H__

/***************************************************************************
 *BSP_GLB_CONTROL - BSP GLOBAL Control Register
 ***************************************************************************/
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION        0x003b0800 /* [RO][32] Revision Register */
#define BCHP_BSP_GLB_CONTROL_GLB_TIMER           0x003b0804 /* [RO][32] Timer */
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR 0x003b0808 /* [RW][32] HOST to BSP Processor General Interrupt Trigger Register */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0           0x003b080c /* [RW][32] Aegis Miscellaneous Control Configuration */
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS    0x003b0814 /* [RW][32] Download Status flags */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS            0x003b0818 /* [RO][32] HW Error Flags */
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER          0x003b081c /* [RW][32] FW Error Flags */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD      0x003b0820 /* [RW][32] Debug/EJTAG Password Data */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL 0x003b0824 /* [RW][32] Debug/EJTAG Password Ctrl/Status */
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_HI        0x003b0830 /* [RO][32] Otp value Poway_UID */
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_LO        0x003b0834 /* [RO][32] Otp value Poway_UID */
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY          0x003b0838 /* [RO][32] Poway Busy Register */
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE          0x003b0840 /* [RW][32] SCPU Data Done */
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT        0x003b0844 /* [RW][32] SCPU Nonsecure Software Init register. */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP    0x003b0850 /* [RW][32] ZZYZX Clock Stop */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET      0x003b0854 /* [RW][32] public ZZYZX software reset Register */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL          0x003b0858 /* [RW][32] ZZYZX Control */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL      0x003b085c /* [RW][32] ZZYZX RNG Control */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2           0x003b0860 /* [RO][32] Input Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2           0x003b0864 /* [RW][32] Output Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2          0x003b0868 /* [WO][32] Input Command Buffer 2 Loaded */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1           0x003b0870 /* [RO][32] Input Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1           0x003b0874 /* [RW][32] Output Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1          0x003b0878 /* [WO][32] Input Command Buffer 1 Loaded */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR         0x003b0880 /* [RO][32] Input Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR         0x003b0884 /* [RW][32] Output Command Buffer Ready */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR        0x003b0888 /* [WO][32] Challenge/Response Input Command Buffer Loaded */
#define BCHP_BSP_GLB_CONTROL_OTP_KEY0_HASH_CRC   0x003b0890 /* [RO][32] OTP KEY0 HASH CRC Register */
#define BCHP_BSP_GLB_CONTROL_OTP_BBL_HASH_CRC    0x003b0894 /* [RO][32] OTP BBL HASH CRC Register */
#define BCHP_BSP_GLB_CONTROL_OTP_BBL2_HASH_CRC   0x003b0898 /* [RO][32] OTP BBL2 HASH CRC Register */
#define BCHP_BSP_GLB_CONTROL_OTP_BFW_HASH_CRC    0x003b089c /* [RO][32] OTP BFW HASH CRC Register */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_hi 0x003b08a0 /* [RO][32] Otp value v_PubOtpUniqueID_hi */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_lo 0x003b08a4 /* [RO][32] Otp value v_PubOtpUniqueID_lo */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpOID         0x003b08a8 /* [RO][32] Otp value v_PubOtpOID */
#define BCHP_BSP_GLB_CONTROL_OTP_MSP_CRC         0x003b08ac /* [RO][32] MSP CRC Register */
#define BCHP_BSP_GLB_CONTROL_v_Version           0x003b08b0 /* [RO][32] Otp value v_Version */
#define BCHP_BSP_GLB_CONTROL_FW_FLAGS            0x003b08b4 /* [RW][32] FW Error Flags */
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_VERSION     0x003b08b8 /* [RO][32] BBL/BFW Version number */
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_INFO        0x003b08bc /* [RO][32] BBL / BFW info register */
#define BCHP_BSP_GLB_CONTROL_ASKM_STB_OwnerID    0x003b08cc /* [RO][32] Otp value ASKM_STB_OwnerID */

/***************************************************************************
 *GLB_REVISION - Revision Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_REVISION :: reserved0 [31:16] */
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_reserved0_MASK           0xffff0000
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_reserved0_SHIFT          16

/* BSP_GLB_CONTROL :: GLB_REVISION :: MAJOR_REV_NUMBER [15:08] */
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MAJOR_REV_NUMBER_MASK    0x0000ff00
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MAJOR_REV_NUMBER_SHIFT   8
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MAJOR_REV_NUMBER_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: GLB_REVISION :: MINOR_REV_NUMBER [07:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MINOR_REV_NUMBER_MASK    0x000000ff
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MINOR_REV_NUMBER_SHIFT   0
#define BCHP_BSP_GLB_CONTROL_GLB_REVISION_MINOR_REV_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *GLB_TIMER - Timer
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_TIMER :: TIMER [31:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_TIMER_TIMER_MASK                  0xffffffff
#define BCHP_BSP_GLB_CONTROL_GLB_TIMER_TIMER_SHIFT                 0
#define BCHP_BSP_GLB_CONTROL_GLB_TIMER_TIMER_DEFAULT               0x00000000

/***************************************************************************
 *HOST_TO_BSP_GENERAL_INTR - HOST to BSP Processor General Interrupt Trigger Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: HOST_TO_BSP_GENERAL_INTR :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR_reserved0_MASK 0xfffffffe
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR_reserved0_SHIFT 1

/* BSP_GLB_CONTROL :: HOST_TO_BSP_GENERAL_INTR :: HOST_TO_BSP_INTR [00:00] */
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR_HOST_TO_BSP_INTR_MASK 0x00000001
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR_HOST_TO_BSP_INTR_SHIFT 0
#define BCHP_BSP_GLB_CONTROL_HOST_TO_BSP_GENERAL_INTR_HOST_TO_BSP_INTR_DEFAULT 0x00000000

/***************************************************************************
 *GLB_MISC0 - Aegis Miscellaneous Control Configuration
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_MISC0 :: reserved0 [31:18] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_reserved0_MASK              0xfffc0000
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_reserved0_SHIFT             18

/* BSP_GLB_CONTROL :: GLB_MISC0 :: OTP_DUMP_CLEAR [17:17] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_CLEAR_MASK         0x00020000
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_CLEAR_SHIFT        17
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_CLEAR_DEFAULT      0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: OTP_DUMP_DONE [16:16] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_DONE_MASK          0x00010000
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_DONE_SHIFT         16
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_OTP_DUMP_DONE_DEFAULT       0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: CONTROL_GENERAL_CFG2 [15:08] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_CONTROL_GENERAL_CFG2_MASK   0x0000ff00
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_CONTROL_GENERAL_CFG2_SHIFT  8
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_CONTROL_GENERAL_CFG2_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: STRAP_SUNDRY_TESTMODE [07:04] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_SUNDRY_TESTMODE_MASK  0x000000f0
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_SUNDRY_TESTMODE_SHIFT 4
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_SUNDRY_TESTMODE_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: ATE_SPARE [03:03] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_SPARE_MASK              0x00000008
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_SPARE_SHIFT             3
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_SPARE_DEFAULT           0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: ATE_EN_FLAG [02:02] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_EN_FLAG_MASK            0x00000004
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_EN_FLAG_SHIFT           2
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_ATE_EN_FLAG_DEFAULT         0x00000000

/* BSP_GLB_CONTROL :: GLB_MISC0 :: STRAP_TEST_DEBUG_EN [01:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_TEST_DEBUG_EN_MASK    0x00000003
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_TEST_DEBUG_EN_SHIFT   0
#define BCHP_BSP_GLB_CONTROL_GLB_MISC0_STRAP_TEST_DEBUG_EN_DEFAULT 0x00000000

/***************************************************************************
 *GLB_DWNLD_STATUS - Download Status flags
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_DWNLD_STATUS :: FW_STATUS [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_FW_STATUS_MASK       0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_FW_STATUS_SHIFT      1
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_FW_STATUS_DEFAULT    0x00000000

/* BSP_GLB_CONTROL :: GLB_DWNLD_STATUS :: DISASTER_RECOVER [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_DISASTER_RECOVER_MASK 0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_DISASTER_RECOVER_SHIFT 0
#define BCHP_BSP_GLB_CONTROL_GLB_DWNLD_STATUS_DISASTER_RECOVER_DEFAULT 0x00000000

/***************************************************************************
 *HW_FLAGS - HW Error Flags
 ***************************************************************************/
/* BSP_GLB_CONTROL :: HW_FLAGS :: reserved0 [31:07] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_reserved0_MASK               0xffffff80
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_reserved0_SHIFT              7

/* BSP_GLB_CONTROL :: HW_FLAGS :: BOOT_ROM_CRC_VALID [06:06] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BOOT_ROM_CRC_VALID_MASK      0x00000040
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BOOT_ROM_CRC_VALID_SHIFT     6
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BOOT_ROM_CRC_VALID_DEFAULT   0x00000000

/* BSP_GLB_CONTROL :: HW_FLAGS :: DMA_CNT_MISMATCH [05:05] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_DMA_CNT_MISMATCH_MASK        0x00000020
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_DMA_CNT_MISMATCH_SHIFT       5
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_DMA_CNT_MISMATCH_DEFAULT     0x00000000

/* BSP_GLB_CONTROL :: HW_FLAGS :: INSTRAM_SHA_MISMATCH [04:04] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_INSTRAM_SHA_MISMATCH_MASK    0x00000010
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_INSTRAM_SHA_MISMATCH_SHIFT   4
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_INSTRAM_SHA_MISMATCH_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: HW_FLAGS :: KEYSHA_MISMATCH [03:03] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_KEYSHA_MISMATCH_MASK         0x00000008
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_KEYSHA_MISMATCH_SHIFT        3
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_KEYSHA_MISMATCH_DEFAULT      0x00000000

/* BSP_GLB_CONTROL :: HW_FLAGS :: BSECK_STATE [02:00] */
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BSECK_STATE_MASK             0x00000007
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BSECK_STATE_SHIFT            0
#define BCHP_BSP_GLB_CONTROL_HW_FLAGS_BSECK_STATE_DEFAULT          0x00000000

/***************************************************************************
 *FW_COUNTER - FW Error Flags
 ***************************************************************************/
/* BSP_GLB_CONTROL :: FW_COUNTER :: reserved0 [31:08] */
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER_reserved0_MASK             0xffffff00
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER_reserved0_SHIFT            8

/* BSP_GLB_CONTROL :: FW_COUNTER :: FW_CNT [07:00] */
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER_FW_CNT_MASK                0x000000ff
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER_FW_CNT_SHIFT               0
#define BCHP_BSP_GLB_CONTROL_FW_COUNTER_FW_CNT_DEFAULT             0x00000000

/***************************************************************************
 *DEBUG_PASSWORD - Debug/EJTAG Password Data
 ***************************************************************************/
/* BSP_GLB_CONTROL :: DEBUG_PASSWORD :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_DATA_MASK              0xffffffff
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_DATA_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_DATA_DEFAULT           0x00000000

/***************************************************************************
 *DEBUG_PASSWORD_CTRL - Debug/EJTAG Password Ctrl/Status
 ***************************************************************************/
/* BSP_GLB_CONTROL :: DEBUG_PASSWORD_CTRL :: reserved0 [31:03] */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_reserved0_MASK    0xfffffff8
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_reserved0_SHIFT   3

/* BSP_GLB_CONTROL :: DEBUG_PASSWORD_CTRL :: DEBUG_PASSWORD_TYPE [02:02] */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_TYPE_MASK 0x00000004
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_TYPE_SHIFT 2
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_TYPE_DEFAULT 0x00000000
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_TYPE_PASSWORD_TYPE_DEBUG 0
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_TYPE_PASSWORD_TYPE_EJTAG 1

/* BSP_GLB_CONTROL :: DEBUG_PASSWORD_CTRL :: EJTAG_PASSWORD_PASS [01:01] */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_EJTAG_PASSWORD_PASS_MASK 0x00000002
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_EJTAG_PASSWORD_PASS_SHIFT 1
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_EJTAG_PASSWORD_PASS_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: DEBUG_PASSWORD_CTRL :: DEBUG_PASSWORD_PASS [00:00] */
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_PASS_MASK 0x00000001
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_PASS_SHIFT 0
#define BCHP_BSP_GLB_CONTROL_DEBUG_PASSWORD_CTRL_DEBUG_PASSWORD_PASS_DEFAULT 0x00000000

/***************************************************************************
 *POWAY_UID_HI - Otp value Poway_UID
 ***************************************************************************/
/* BSP_GLB_CONTROL :: POWAY_UID_HI :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_HI_DATA_MASK                0xffffffff
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_HI_DATA_SHIFT               0

/***************************************************************************
 *POWAY_UID_LO - Otp value Poway_UID
 ***************************************************************************/
/* BSP_GLB_CONTROL :: POWAY_UID_LO :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_LO_DATA_MASK                0xffffffff
#define BCHP_BSP_GLB_CONTROL_POWAY_UID_LO_DATA_SHIFT               0

/***************************************************************************
 *POWAY_BUSY - Poway Busy Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: POWAY_BUSY :: reserved0 [31:02] */
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_reserved0_MASK             0xfffffffc
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_reserved0_SHIFT            2

/* BSP_GLB_CONTROL :: POWAY_BUSY :: SP_BUSY [01:01] */
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_SP_BUSY_MASK               0x00000002
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_SP_BUSY_SHIFT              1
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_SP_BUSY_DEFAULT            0x00000000

/* BSP_GLB_CONTROL :: POWAY_BUSY :: PKL_BUSY [00:00] */
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_PKL_BUSY_MASK              0x00000001
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_PKL_BUSY_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_POWAY_BUSY_PKL_BUSY_DEFAULT           0x00000000

/***************************************************************************
 *SCPU_DDONE - SCPU Data Done
 ***************************************************************************/
/* BSP_GLB_CONTROL :: SCPU_DDONE :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE_reserved0_MASK             0xfffffffe
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE_reserved0_SHIFT            1

/* BSP_GLB_CONTROL :: SCPU_DDONE :: CMD_SCPU_DDONE [00:00] */
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE_CMD_SCPU_DDONE_MASK        0x00000001
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE_CMD_SCPU_DDONE_SHIFT       0
#define BCHP_BSP_GLB_CONTROL_SCPU_DDONE_CMD_SCPU_DDONE_DEFAULT     0x00000000

/***************************************************************************
 *SCPU_SW_INIT - SCPU Nonsecure Software Init register.
 ***************************************************************************/
/* BSP_GLB_CONTROL :: SCPU_SW_INIT :: reserved0 [31:03] */
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_reserved0_MASK           0xfffffff8
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_reserved0_SHIFT          3

/* BSP_GLB_CONTROL :: SCPU_SW_INIT :: SCPU_SCB_INIT_REQ [02:02] */
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SCB_INIT_REQ_MASK   0x00000004
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SCB_INIT_REQ_SHIFT  2
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SCB_INIT_REQ_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: SCPU_SW_INIT :: SCPU_NMI [01:01] */
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_NMI_MASK            0x00000002
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_NMI_SHIFT           1
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_NMI_DEFAULT         0x00000000

/* BSP_GLB_CONTROL :: SCPU_SW_INIT :: SCPU_SW_INIT [00:00] */
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SW_INIT_MASK        0x00000001
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SW_INIT_SHIFT       0
#define BCHP_BSP_GLB_CONTROL_SCPU_SW_INIT_SCPU_SW_INIT_DEFAULT     0x00000001

/***************************************************************************
 *ZZYZX_CLOCK_STOP - ZZYZX Clock Stop
 ***************************************************************************/
/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: reserved0 [31:05] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_reserved0_MASK       0xffffffe0
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_reserved0_SHIFT      5

/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: ZZYZX_N2NOCLK_EDGE [04:04] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_EDGE_MASK 0x00000010
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_EDGE_SHIFT 4
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_EDGE_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: ZZYZX_N2NOCLK [03:03] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_MASK   0x00000008
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_SHIFT  3
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_N2NOCLK_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: POWAY_SLOW_CLOCK [02:02] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_POWAY_SLOW_CLOCK_MASK 0x00000004
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_POWAY_SLOW_CLOCK_SHIFT 2
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_POWAY_SLOW_CLOCK_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: ZZYZX_SLOW_CLOCK [01:01] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_SLOW_CLOCK_MASK 0x00000002
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_SLOW_CLOCK_SHIFT 1
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_SLOW_CLOCK_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: ZZYZX_CLOCK_STOP :: ZZYZX_CLOCK_STOP [00:00] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_CLOCK_STOP_MASK 0x00000001
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_CLOCK_STOP_SHIFT 0
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CLOCK_STOP_ZZYZX_CLOCK_STOP_DEFAULT 0x00000000

/***************************************************************************
 *ZZYZX_SW_RESET - public ZZYZX software reset Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: ZZYZX_SW_RESET :: ZZYZX_PROTECT_PATN [31:01] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_PROTECT_PATN_MASK 0xfffffffe
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_PROTECT_PATN_SHIFT 1
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_PROTECT_PATN_DEFAULT 0x00000000

/* BSP_GLB_CONTROL :: ZZYZX_SW_RESET :: ZZYZX_SW_RST [00:00] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_SW_RST_MASK      0x00000001
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_SW_RST_SHIFT     0
#define BCHP_BSP_GLB_CONTROL_ZZYZX_SW_RESET_ZZYZX_SW_RST_DEFAULT   0x00000001

/***************************************************************************
 *ZZYZX_CTRL - ZZYZX Control
 ***************************************************************************/
/* BSP_GLB_CONTROL :: ZZYZX_CTRL :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL_reserved0_MASK             0xfffffffe
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL_reserved0_SHIFT            1

/* BSP_GLB_CONTROL :: ZZYZX_CTRL :: KTEACK [00:00] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL_KTEACK_MASK                0x00000001
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL_KTEACK_SHIFT               0
#define BCHP_BSP_GLB_CONTROL_ZZYZX_CTRL_KTEACK_DEFAULT             0x00000000

/***************************************************************************
 *ZZYZX_RNG_CTRL - ZZYZX RNG Control
 ***************************************************************************/
/* BSP_GLB_CONTROL :: ZZYZX_RNG_CTRL :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL_reserved0_MASK         0xfffffffe
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL_reserved0_SHIFT        1

/* BSP_GLB_CONTROL :: ZZYZX_RNG_CTRL :: RNGEmpiricalTest [00:00] */
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL_RNGEmpiricalTest_MASK  0x00000001
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL_RNGEmpiricalTest_SHIFT 0
#define BCHP_BSP_GLB_CONTROL_ZZYZX_RNG_CTRL_RNGEmpiricalTest_DEFAULT 0x00000000

/***************************************************************************
 *GLB_IRDY2 - Input Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_IRDY2 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2_reserved0_MASK              0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2_reserved0_SHIFT             1

/* BSP_GLB_CONTROL :: GLB_IRDY2 :: CMD_IRDY2 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2_CMD_IRDY2_MASK              0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2_CMD_IRDY2_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY2_CMD_IRDY2_DEFAULT           0x00000000

/***************************************************************************
 *GLB_ORDY2 - Output Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ORDY2 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2_reserved0_MASK              0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2_reserved0_SHIFT             1

/* BSP_GLB_CONTROL :: GLB_ORDY2 :: CMD_ORDY2 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2_CMD_ORDY2_MASK              0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2_CMD_ORDY2_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY2_CMD_ORDY2_DEFAULT           0x00000000

/***************************************************************************
 *GLB_ILOAD2 - Input Command Buffer 2 Loaded
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ILOAD2 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2_reserved0_MASK             0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2_reserved0_SHIFT            1

/* BSP_GLB_CONTROL :: GLB_ILOAD2 :: CMD_ILOAD2 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2_CMD_ILOAD2_MASK            0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2_CMD_ILOAD2_SHIFT           0
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD2_CMD_ILOAD2_DEFAULT         0x00000000

/***************************************************************************
 *GLB_IRDY1 - Input Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_IRDY1 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1_reserved0_MASK              0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1_reserved0_SHIFT             1

/* BSP_GLB_CONTROL :: GLB_IRDY1 :: CMD_IRDY1 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1_CMD_IRDY1_MASK              0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1_CMD_IRDY1_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY1_CMD_IRDY1_DEFAULT           0x00000000

/***************************************************************************
 *GLB_ORDY1 - Output Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ORDY1 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1_reserved0_MASK              0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1_reserved0_SHIFT             1

/* BSP_GLB_CONTROL :: GLB_ORDY1 :: CMD_ORDY1 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1_CMD_ORDY1_MASK              0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1_CMD_ORDY1_SHIFT             0
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY1_CMD_ORDY1_DEFAULT           0x00000000

/***************************************************************************
 *GLB_ILOAD1 - Input Command Buffer 1 Loaded
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ILOAD1 :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1_reserved0_MASK             0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1_reserved0_SHIFT            1

/* BSP_GLB_CONTROL :: GLB_ILOAD1 :: CMD_ILOAD1 [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1_CMD_ILOAD1_MASK            0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1_CMD_ILOAD1_SHIFT           0
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD1_CMD_ILOAD1_DEFAULT         0x00000000

/***************************************************************************
 *GLB_IRDY_CR - Input Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_IRDY_CR :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR_reserved0_MASK            0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR_reserved0_SHIFT           1

/* BSP_GLB_CONTROL :: GLB_IRDY_CR :: CMD_IRDY_CR [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR_CMD_IRDY_CR_MASK          0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR_CMD_IRDY_CR_SHIFT         0
#define BCHP_BSP_GLB_CONTROL_GLB_IRDY_CR_CMD_IRDY_CR_DEFAULT       0x00000000

/***************************************************************************
 *GLB_ORDY_CR - Output Command Buffer Ready
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ORDY_CR :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR_reserved0_MASK            0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR_reserved0_SHIFT           1

/* BSP_GLB_CONTROL :: GLB_ORDY_CR :: CMD_ORDY_CR [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR_CMD_ORDY_CR_MASK          0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR_CMD_ORDY_CR_SHIFT         0
#define BCHP_BSP_GLB_CONTROL_GLB_ORDY_CR_CMD_ORDY_CR_DEFAULT       0x00000000

/***************************************************************************
 *GLB_ILOAD_CR - Challenge/Response Input Command Buffer Loaded
 ***************************************************************************/
/* BSP_GLB_CONTROL :: GLB_ILOAD_CR :: reserved0 [31:01] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR_reserved0_MASK           0xfffffffe
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR_reserved0_SHIFT          1

/* BSP_GLB_CONTROL :: GLB_ILOAD_CR :: CMD_ILOAD_CR [00:00] */
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR_CMD_ILOAD_CR_MASK        0x00000001
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR_CMD_ILOAD_CR_SHIFT       0
#define BCHP_BSP_GLB_CONTROL_GLB_ILOAD_CR_CMD_ILOAD_CR_DEFAULT     0x00000000

/***************************************************************************
 *OTP_KEY0_HASH_CRC - OTP KEY0 HASH CRC Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: OTP_KEY0_HASH_CRC :: CRC [31:00] */
#define BCHP_BSP_GLB_CONTROL_OTP_KEY0_HASH_CRC_CRC_MASK            0xffffffff
#define BCHP_BSP_GLB_CONTROL_OTP_KEY0_HASH_CRC_CRC_SHIFT           0

/***************************************************************************
 *OTP_BBL_HASH_CRC - OTP BBL HASH CRC Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: OTP_BBL_HASH_CRC :: CRC [31:00] */
#define BCHP_BSP_GLB_CONTROL_OTP_BBL_HASH_CRC_CRC_MASK             0xffffffff
#define BCHP_BSP_GLB_CONTROL_OTP_BBL_HASH_CRC_CRC_SHIFT            0

/***************************************************************************
 *OTP_BBL2_HASH_CRC - OTP BBL2 HASH CRC Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: OTP_BBL2_HASH_CRC :: CRC [31:00] */
#define BCHP_BSP_GLB_CONTROL_OTP_BBL2_HASH_CRC_CRC_MASK            0xffffffff
#define BCHP_BSP_GLB_CONTROL_OTP_BBL2_HASH_CRC_CRC_SHIFT           0

/***************************************************************************
 *OTP_BFW_HASH_CRC - OTP BFW HASH CRC Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: OTP_BFW_HASH_CRC :: CRC [31:00] */
#define BCHP_BSP_GLB_CONTROL_OTP_BFW_HASH_CRC_CRC_MASK             0xffffffff
#define BCHP_BSP_GLB_CONTROL_OTP_BFW_HASH_CRC_CRC_SHIFT            0

/***************************************************************************
 *v_PubOtpUniqueID_hi - Otp value v_PubOtpUniqueID_hi
 ***************************************************************************/
/* BSP_GLB_CONTROL :: v_PubOtpUniqueID_hi :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_hi_DATA_MASK         0xffffffff
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_hi_DATA_SHIFT        0

/***************************************************************************
 *v_PubOtpUniqueID_lo - Otp value v_PubOtpUniqueID_lo
 ***************************************************************************/
/* BSP_GLB_CONTROL :: v_PubOtpUniqueID_lo :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_lo_DATA_MASK         0xffffffff
#define BCHP_BSP_GLB_CONTROL_v_PubOtpUniqueID_lo_DATA_SHIFT        0

/***************************************************************************
 *v_PubOtpOID - Otp value v_PubOtpOID
 ***************************************************************************/
/* BSP_GLB_CONTROL :: v_PubOtpOID :: reserved0 [31:16] */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpOID_reserved0_MASK            0xffff0000
#define BCHP_BSP_GLB_CONTROL_v_PubOtpOID_reserved0_SHIFT           16

/* BSP_GLB_CONTROL :: v_PubOtpOID :: PubOtpOID [15:00] */
#define BCHP_BSP_GLB_CONTROL_v_PubOtpOID_PubOtpOID_MASK            0x0000ffff
#define BCHP_BSP_GLB_CONTROL_v_PubOtpOID_PubOtpOID_SHIFT           0

/***************************************************************************
 *OTP_MSP_CRC - MSP CRC Register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: OTP_MSP_CRC :: CRC [31:00] */
#define BCHP_BSP_GLB_CONTROL_OTP_MSP_CRC_CRC_MASK                  0xffffffff
#define BCHP_BSP_GLB_CONTROL_OTP_MSP_CRC_CRC_SHIFT                 0

/***************************************************************************
 *v_Version - Otp value v_Version
 ***************************************************************************/
/* BSP_GLB_CONTROL :: v_Version :: Data [31:00] */
#define BCHP_BSP_GLB_CONTROL_v_Version_Data_MASK                   0xffffffff
#define BCHP_BSP_GLB_CONTROL_v_Version_Data_SHIFT                  0

/***************************************************************************
 *FW_FLAGS - FW Error Flags
 ***************************************************************************/
/* BSP_GLB_CONTROL :: FW_FLAGS :: FW_ERR [31:00] */
#define BCHP_BSP_GLB_CONTROL_FW_FLAGS_FW_ERR_MASK                  0xffffffff
#define BCHP_BSP_GLB_CONTROL_FW_FLAGS_FW_ERR_SHIFT                 0
#define BCHP_BSP_GLB_CONTROL_FW_FLAGS_FW_ERR_DEFAULT               0x00000000

/***************************************************************************
 *BBL_BFW_VERSION - BBL/BFW Version number
 ***************************************************************************/
/* BSP_GLB_CONTROL :: BBL_BFW_VERSION :: VERSION [31:00] */
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_VERSION_VERSION_MASK          0xffffffff
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_VERSION_VERSION_SHIFT         0

/***************************************************************************
 *BBL_BFW_INFO - BBL / BFW info register
 ***************************************************************************/
/* BSP_GLB_CONTROL :: BBL_BFW_INFO :: DATA [31:00] */
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_INFO_DATA_MASK                0xffffffff
#define BCHP_BSP_GLB_CONTROL_BBL_BFW_INFO_DATA_SHIFT               0

/***************************************************************************
 *ASKM_STB_OwnerID - Otp value ASKM_STB_OwnerID
 ***************************************************************************/
/* BSP_GLB_CONTROL :: ASKM_STB_OwnerID :: reserved0 [31:16] */
#define BCHP_BSP_GLB_CONTROL_ASKM_STB_OwnerID_reserved0_MASK       0xffff0000
#define BCHP_BSP_GLB_CONTROL_ASKM_STB_OwnerID_reserved0_SHIFT      16

/* BSP_GLB_CONTROL :: ASKM_STB_OwnerID :: OwnerID [15:00] */
#define BCHP_BSP_GLB_CONTROL_ASKM_STB_OwnerID_OwnerID_MASK         0x0000ffff
#define BCHP_BSP_GLB_CONTROL_ASKM_STB_OwnerID_OwnerID_SHIFT        0

#endif /* #ifndef BCHP_BSP_GLB_CONTROL_H__ */

/* End of File */
