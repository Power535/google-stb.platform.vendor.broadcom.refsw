/***************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
 *
 * [File Description:]
 *
 ***************************************************************************/

/* base modules */
#include "bstd.h"           /* standard types */
#include "berr.h"           /* error code */
#include "bdbg.h"           /* debug interface */
#include "bkni.h"           /* kernel interface */

#include "bvce.h"
#include "bvce_priv.h"
#include "bvce_debug_priv.h"

#include "bchp_common.h"
#include "bchp_vice2_misc.h"
#include "bchp_vice2_arcss_ess_ctrl_0.h"
#include "bchp_vice2_arcss_ess_ctrl_1.h"
#include "bchp_vice2_arcss_misc.h"
#include "bchp_vice2_arcss_ess_dccm_0.h"
#include "bchp_vice2_arcss_ess_dccm_1.h"
#include "bchp_vice2_arcss_ess_p1_intr2_0.h"
#include "bchp_vice2_arcss_ess_hostif_0.h"
#include "bchp_vice2_arcss_ess_hostif_1.h"
#include "bchp_vice2_l2.h"
#include "bchp_vice2_cabac_0.h"
#include "bchp_vice2_vip_0.h"
#include "bchp_vice2_vip1_0.h"
#include "bchp_int_id_vice2_l2.h"
#include "bchp_sun_top_ctrl.h"
#include "bavc_vce_mbox.h"

#ifdef BCHP_PWR_SUPPORT
#include "bchp_pwr.h"
#endif

/* For Debug */
#include "bchp_vice2_cme_0.h"

BDBG_MODULE(BVCE_PLATFORM_7425);

#define BVCE_P_REGISTER_BASE 0x700000
#define BVCE_P_REGISTER_START (BCHP_PHYSICAL_OFFSET + BVCE_P_REGISTER_BASE)

static const BVCE_Platform_P_RegisterSetting s_astViceResetRegisterSettings[] =
{
 { "Arc[0] Boot",               BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START,                                    0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START_ENABLE_MASK },
 { "Vice SUNTOP Reset (Set)",   BCHP_SUN_TOP_CTRL_SW_INIT_1_SET,                                                0xFFFFFFFF,             BCHP_SUN_TOP_CTRL_SW_INIT_1_SET_vice2_sw_init_MASK },
 { "Vice SUNTOP Reset (Clear)", BCHP_SUN_TOP_CTRL_SW_INIT_1_CLEAR,                                              0xFFFFFFFF,             BCHP_SUN_TOP_CTRL_SW_INIT_1_SET_vice2_sw_init_MASK },
 { "Vice CPU Speed (MISC_SCRATCH_0)", BCHP_VICE2_MISC_SCRATCH_0,                                                0x00000000,             0xFFFFFFFF },
 { "Reg Addr Offset",           BCHP_VICE2_ARCSS_MISC_INIT_SYS_REG_ADDR_OFFSET,                                 BVCE_P_REGISTER_START,  BCHP_VICE2_ARCSS_MISC_INIT_SYS_REG_ADDR_OFFSET_OFFSET_MASK },
 { "Arc Config",                BCHP_VICE2_ARCSS_MISC_INIT_SYS_ARC_CONFIG,                                      0x00000000,             BCHP_VICE2_ARCSS_MISC_INIT_SYS_ARC_CONFIG_SEL_MASK },
 { "Arc[0] Host I/F",           BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_HOST_IF,                                   0xFFFFFFFF,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_HOST_IF_SEL_MASK },
 { "Cache Miss",                BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL,                      0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL_CACHE_MISS_EN_MASK },
 { "Read Swap",                 BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL,                      0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL_DATA_RD_SWAP_MASK },
 { "Write Swap",                BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL,                      0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_BVCI2SCB_BRIDGE_CTRL_DATA_WR_SWAP_MASK },
 { "Host2Vice",                 BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE + HOST2VICE_MBOX_OFFSET,           VICE_BOOT_STATUS_INIT, BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_DATA_MASK },
 { "Vice2Host",                 BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE + VICE2HOST_MBOX_OFFSET,           0x00000000,             BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_DATA_MASK },
 { "Arc[0] Boot",               BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START,                                    0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START_ENABLE_MASK },
};

static const BVCE_Platform_P_RegisterSetting s_astViceWatchdogHandlerEnableRegisterSettings[BVCE_PLATFORM_P_NUM_ENCODE_INSTANCES][1] =
{
   {
      { "Vice SW INIT Mode (Set)",   BCHP_VICE2_MISC_SW_INIT_CONTROL,                                              0xFFFFFFFF,             BCHP_VICE2_MISC_SW_INIT_CONTROL_SW_INIT_MODE_MASK },
   },
};

static const BVCE_Platform_P_RegisterSetting s_astViceWatchdogHandlerDisableRegisterSettings[BVCE_PLATFORM_P_NUM_ENCODE_INSTANCES][1] =
{
   {
      { "Vice SW INIT Mode (Clear)",         BCHP_VICE2_MISC_SW_INIT_CONTROL,                                              0x00000000,             BCHP_VICE2_MISC_SW_INIT_CONTROL_SW_INIT_MODE_MASK },
   },
};

static const BVCE_Platform_P_RegisterSetting s_astViceBootRegisterSettings[] =
{
 { "Arc[0] Boot",               BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START,                                    0XFFFFFFFF,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_PROC_START_ENABLE_MASK },
};

static const BVCE_Platform_P_RegisterSetting s_astViceWatchdogDisableRegisterSettings[] =
{
 { "Arc[0] Watchdog Enable",    BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL,                             0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL_ENABLE_MASK },
 { "Arc[1] Watchdog Enable",    BCHP_VICE2_ARCSS_ESS_CTRL_1_INIT_SYS_WATCHDOG_CTRL,                             0x00000000,             BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL_ENABLE_MASK },
};

extern const BVCE_Platform_P_Register s_astViceHardwareRegisters[];
extern const unsigned s_uiViceHardwareRegistersCount;

BERR_Code
BVCE_Platform_P_GetConfig(
         BBOX_Handle hBox,
         unsigned uiInstance,
         BVCE_Platform_P_Config *pstPlatformConfig
         )
{
   BDBG_ASSERT( pstPlatformConfig );
   BDBG_ASSERT( uiInstance < BVCE_PLATFORM_P_NUM_ENCODE_INSTANCES );

   BSTD_UNUSED( uiInstance );

   BKNI_Memset(
            pstPlatformConfig,
            0,
            sizeof( BVCE_Platform_P_Config )
            );

#ifdef BAVC_VICE_MBOX_OFFSET_BVN2VICE_DATA_0_START
   BDBG_CASSERT( 1 == BAVC_VICE_MBOX_REVISION_ID );
   BDBG_CASSERT( HOST2VICE_MBOX_OFFSET == BAVC_VICE_MBOX_OFFSET_HOST2VICE );
   BDBG_CASSERT( VICE2HOST_MBOX_OFFSET == BAVC_VICE_MBOX_OFFSET_VICE2HOST );
   BDBG_CASSERT( BVN2VICE_MBOX_OFFSET == BAVC_VICE_MBOX_OFFSET_BVN2VICE );
   BDBG_CASSERT( BVN2VICE_MBOX_PAYLOAD_OFFSET == BAVC_VICE_MBOX_OFFSET_BVN2VICE_DATA_0_START );
#else
   BDBG_CASSERT( HOST2VICE_MBOX_OFFSET == BCHP_VICE2_REGSET_MISC_MBOX_INTERFACE_HOST2VICE_OFFSET );
   BDBG_CASSERT( VICE2HOST_MBOX_OFFSET == BCHP_VICE2_REGSET_MISC_MBOX_INTERFACE_VICE2HOST_OFFSET );
   BDBG_CASSERT( BVN2VICE_MBOX_OFFSET == BCHP_VICE2_REGSET_MISC_MBOX_INTERFACE_BVN2VICE_OFFSET );
   BDBG_CASSERT( BVN2VICE_MBOX_PAYLOAD_OFFSET == BCHP_VICE2_REGSET_MISC_MBOX_INTERFACE_DWORD_00_BVB_PIC_SIZE_OFFSET );
#endif

   pstPlatformConfig->stViceReset.astRegisterSettings = s_astViceResetRegisterSettings;
   pstPlatformConfig->stViceReset.uiRegisterCount = sizeof( s_astViceResetRegisterSettings ) / sizeof ( BVCE_Platform_P_RegisterSetting );

   pstPlatformConfig->stViceWatchdogHandlerEnable.astRegisterSettings = s_astViceWatchdogHandlerEnableRegisterSettings[uiInstance];
   pstPlatformConfig->stViceWatchdogHandlerEnable.uiRegisterCount = sizeof( s_astViceWatchdogHandlerEnableRegisterSettings[uiInstance] ) / sizeof ( BVCE_Platform_P_RegisterSetting );

   pstPlatformConfig->stViceWatchdogHandlerDisable.astRegisterSettings = s_astViceWatchdogHandlerDisableRegisterSettings[uiInstance];
   pstPlatformConfig->stViceWatchdogHandlerDisable.uiRegisterCount = sizeof( s_astViceWatchdogHandlerDisableRegisterSettings[uiInstance] ) / sizeof ( BVCE_Platform_P_RegisterSetting );

   pstPlatformConfig->stViceBoot.astRegisterSettings = s_astViceBootRegisterSettings;
   pstPlatformConfig->stViceBoot.uiRegisterCount = sizeof( s_astViceBootRegisterSettings ) / sizeof ( BVCE_Platform_P_RegisterSetting );

   pstPlatformConfig->stViceWatchdogDisable.astRegisterSettings = s_astViceWatchdogDisableRegisterSettings;
   pstPlatformConfig->stViceWatchdogDisable.uiRegisterCount = sizeof( s_astViceWatchdogDisableRegisterSettings ) / sizeof ( BVCE_Platform_P_RegisterSetting );

   BDBG_ASSERT( 2 == BVCE_PLATFORM_P_NUM_ARC_CORES );
   pstPlatformConfig->stCore[0].uiInstructionStartPhysicalAddress = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_ARC_INSTR_ADDR_OFFSET;
   pstPlatformConfig->stCore[0].uiDataSpaceStartRelativeOffset = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_DATA_SPACE_START;
   pstPlatformConfig->stCore[0].uiDCCMBase = BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE;
   pstPlatformConfig->stCore[1].uiInstructionStartPhysicalAddress = BCHP_VICE2_ARCSS_ESS_CTRL_1_INIT_SYS_ARC_INSTR_ADDR_OFFSET;
   pstPlatformConfig->stCore[1].uiDataSpaceStartRelativeOffset = BCHP_VICE2_ARCSS_ESS_CTRL_1_INIT_SYS_DATA_SPACE_START;
   pstPlatformConfig->stCore[1].uiDCCMBase = BCHP_VICE2_ARCSS_ESS_DCCM_1_DATAi_ARRAY_BASE;

   BDBG_ASSERT( 2 == BVCE_PLATFORM_P_NUM_OUTPUT_CHANNELS );
   pstPlatformConfig->stOutput[0].stCDB.uiReadPointer = BCHP_VICE2_CABAC_0_CDB_CTX0_READ_PTR;
   pstPlatformConfig->stOutput[0].stCDB.uiBasePointer = BCHP_VICE2_CABAC_0_CDB_CTX0_BASE_PTR;
   pstPlatformConfig->stOutput[0].stCDB.uiValidPointer = BCHP_VICE2_CABAC_0_CDB_CTX0_VALID_PTR;
   pstPlatformConfig->stOutput[0].stCDB.uiWritePointer = BCHP_VICE2_CABAC_0_CDB_CTX0_WRITE_PTR;
   pstPlatformConfig->stOutput[0].stCDB.uiEndPointer = BCHP_VICE2_CABAC_0_CDB_CTX0_END_PTR;

   pstPlatformConfig->stOutput[0].stITB.uiReadPointer = BCHP_VICE2_CABAC_0_ITB_CTX0_READ_PTR;
   pstPlatformConfig->stOutput[0].stITB.uiBasePointer = BCHP_VICE2_CABAC_0_ITB_CTX0_BASE_PTR;
   pstPlatformConfig->stOutput[0].stITB.uiValidPointer = BCHP_VICE2_CABAC_0_ITB_CTX0_VALID_PTR;
   pstPlatformConfig->stOutput[0].stITB.uiWritePointer = BCHP_VICE2_CABAC_0_ITB_CTX0_WRITE_PTR;
   pstPlatformConfig->stOutput[0].stITB.uiEndPointer = BCHP_VICE2_CABAC_0_ITB_CTX0_END_PTR;

   pstPlatformConfig->stOutput[1].stCDB.uiReadPointer = BCHP_VICE2_CABAC_0_CDB_CTX1_READ_PTR;
   pstPlatformConfig->stOutput[1].stCDB.uiBasePointer = BCHP_VICE2_CABAC_0_CDB_CTX1_BASE_PTR;
   pstPlatformConfig->stOutput[1].stCDB.uiValidPointer = BCHP_VICE2_CABAC_0_CDB_CTX1_VALID_PTR;
   pstPlatformConfig->stOutput[1].stCDB.uiWritePointer = BCHP_VICE2_CABAC_0_CDB_CTX1_WRITE_PTR;
   pstPlatformConfig->stOutput[1].stCDB.uiEndPointer = BCHP_VICE2_CABAC_0_CDB_CTX1_END_PTR;

   pstPlatformConfig->stOutput[1].stITB.uiReadPointer = BCHP_VICE2_CABAC_0_ITB_CTX1_READ_PTR;
   pstPlatformConfig->stOutput[1].stITB.uiBasePointer = BCHP_VICE2_CABAC_0_ITB_CTX1_BASE_PTR;
   pstPlatformConfig->stOutput[1].stITB.uiValidPointer = BCHP_VICE2_CABAC_0_ITB_CTX1_VALID_PTR;
   pstPlatformConfig->stOutput[1].stITB.uiWritePointer = BCHP_VICE2_CABAC_0_ITB_CTX1_WRITE_PTR;
   pstPlatformConfig->stOutput[1].stITB.uiEndPointer = BCHP_VICE2_CABAC_0_ITB_CTX1_END_PTR;

   BDBG_CASSERT( 28 == HOST_CMD_MAILBOX_INTERRUPT_LEVEL );
   BDBG_CASSERT( 27 == HOST_EVENTS_INTERRUPT_LEVEL );
   pstPlatformConfig->stInterrupt.uiInterruptStatusRegister = BCHP_VICE2_L2_CPU_STATUS;

   pstPlatformConfig->stInterrupt.idWatchdog[0] = BCHP_INT_ID_VICE2_WDOG_0_INTR;

   pstPlatformConfig->stInterrupt.idWatchdog[1] = BCHP_INT_ID_VICE2_WDOG_1_INTR;

   pstPlatformConfig->stInterrupt.idDataReady[0] = BCHP_INT_ID_VICE2_CABAC_RDY_0_INTR;

#ifdef BCHP_VICE2_L2_CPU_STATUS_VICE2_CABAC_RDY_1_INTR_MASK
   pstPlatformConfig->stInterrupt.idDataReady[1] = BCHP_INT_ID_VICE2_CABAC_RDY_1_INTR;
#endif

   pstPlatformConfig->stInterrupt.idMailbox = BCHP_INT_ID_VICE2_MBOX_INTR;

   pstPlatformConfig->stInterrupt.idEvent = BCHP_INT_ID_VICE2_ERROR_INTR;

   pstPlatformConfig->stDebug.uiCMEPictureIndex = BCHP_VICE2_CME_0_PICTURE_INDEX;
   pstPlatformConfig->stDebug.uiArcPC[0] = BCHP_VICE2_ARCSS_ESS_HOSTIF_0_HOSTIFi_ARRAY_BASE + 0x18;
   pstPlatformConfig->stDebug.uiArcPC[1] = BCHP_VICE2_ARCSS_ESS_HOSTIF_1_HOSTIFi_ARRAY_BASE + 0x18;
   pstPlatformConfig->stDebug.uiArcHostIF[0] =  BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_HOST_IF;
   pstPlatformConfig->stDebug.uiArcHostIF[1] = BCHP_VICE2_ARCSS_ESS_CTRL_1_INIT_SYS_HOST_IF;
   pstPlatformConfig->stDebug.uiArcHostIFMask[0] = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_HOST_IF_SEL_MASK;
   pstPlatformConfig->stDebug.uiArcHostIFMask[1] = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_HOST_IF_SEL_MASK;
   pstPlatformConfig->stDebug.uiPicArcStatus32 = BCHP_VICE2_ARCSS_ESS_HOSTIF_0_HOSTIFi_ARRAY_BASE + 0x28;
   pstPlatformConfig->stDebug.uiCDBDepth[0] = BCHP_VICE2_CABAC_0_CDB_CTX0_DEPTH;
   pstPlatformConfig->stDebug.uiCDBDepth[1] = BCHP_VICE2_CABAC_0_CDB_CTX1_DEPTH;
#if (BCHP_VER >= BCHP_VER_B0)
   pstPlatformConfig->stDebug.uiSTC[0] = BCHP_VICE2_ARCSS_MISC_STC0_LOWER;
   pstPlatformConfig->stDebug.uiSTC[1] = BCHP_VICE2_ARCSS_MISC_STC1_LOWER;
#else
   pstPlatformConfig->stDebug.uiSTC[0] = BCHP_VICE2_ARCSS_MISC_STC;
#endif

   pstPlatformConfig->stMailbox.uiHost2ViceMailboxAddress = BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE + HOST2VICE_MBOX_OFFSET;
   pstPlatformConfig->stMailbox.uiVice2HostMailboxAddress = BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE + VICE2HOST_MBOX_OFFSET;
   pstPlatformConfig->stMailbox.uiBvn2ViceMailboxAddress = BCHP_VICE2_ARCSS_ESS_DCCM_0_DATAi_ARRAY_BASE + BVN2VICE_MBOX_OFFSET;
   pstPlatformConfig->stMailbox.uiHost2ViceInterruptAddress = BCHP_VICE2_ARCSS_ESS_P1_INTR2_0_ARC_P0_SET;
   pstPlatformConfig->stMailbox.uiHost2ViceInterruptMask = BCHP_VICE2_ARCSS_ESS_P1_INTR2_0_ARC_P0_SET_ARCESS_SOFT1_10_INTR_MASK;


#ifdef BCHP_PWR_SUPPORT
#if BCHP_PWR_RESOURCE_VICE0_PWR
   pstPlatformConfig->stPower.astResource[BVCE_Power_Type_ePower].id = BCHP_PWR_RESOURCE_VICE0_PWR;
#endif
#if BCHP_PWR_RESOURCE_VICE0_CLK
   pstPlatformConfig->stPower.astResource[BVCE_Power_Type_eClock].id = BCHP_PWR_RESOURCE_VICE0_CLK;
#endif
#endif

   pstPlatformConfig->stPower.stCore[0].stSleep.uiAddress = BCHP_VICE2_ARCSS_ESS_HOSTIF_0_HOSTIFi_ARRAY_BASE + 0x14;
   pstPlatformConfig->stPower.stCore[0].stSleep.uiMask = (((uint32_t) 1) << 23);
   pstPlatformConfig->stPower.stCore[0].stSleep.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stPower.stCore[0].stWatchdog.uiAddress = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL;
   pstPlatformConfig->stPower.stCore[0].stWatchdog.uiMask = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL_ENABLE_MASK;
   pstPlatformConfig->stPower.stCore[0].stWatchdog.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stPower.stCore[0].stHalt.uiAddress = BCHP_VICE2_ARCSS_ESS_HOSTIF_0_HOSTIFi_ARRAY_BASE + 0x28;
   pstPlatformConfig->stPower.stCore[0].stHalt.uiMask = 0x01;
   pstPlatformConfig->stPower.stCore[0].stHalt.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stPower.stCore[1].stSleep.uiAddress = BCHP_VICE2_ARCSS_ESS_HOSTIF_1_HOSTIFi_ARRAY_BASE + 0x14;
   pstPlatformConfig->stPower.stCore[1].stSleep.uiMask = (((uint32_t) 1) << 23);
   pstPlatformConfig->stPower.stCore[1].stSleep.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stPower.stCore[1].stWatchdog.uiAddress = BCHP_VICE2_ARCSS_ESS_CTRL_1_INIT_SYS_WATCHDOG_CTRL;
   pstPlatformConfig->stPower.stCore[1].stWatchdog.uiMask = BCHP_VICE2_ARCSS_ESS_CTRL_0_INIT_SYS_WATCHDOG_CTRL_ENABLE_MASK;
   pstPlatformConfig->stPower.stCore[1].stWatchdog.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stPower.stCore[1].stHalt.uiAddress = BCHP_VICE2_ARCSS_ESS_HOSTIF_1_HOSTIFi_ARRAY_BASE + 0x28;
   pstPlatformConfig->stPower.stCore[1].stHalt.uiMask = 0x01;
   pstPlatformConfig->stPower.stCore[1].stHalt.uiValue = 0xFFFFFFFF;

   pstPlatformConfig->stWatchdogRegisterDumpList.iInstanceOffset = BVCE_P_REGISTER_BASE;
   pstPlatformConfig->stWatchdogRegisterDumpList.astRegisters = s_astViceHardwareRegisters;
   pstPlatformConfig->stWatchdogRegisterDumpList.uiCount = s_uiViceHardwareRegistersCount;

   pstPlatformConfig->stDebug.stCmd.uiBasePointer = BCHP_VICE2_CABAC_0_CMD_BUFF_START_ADDR;
   pstPlatformConfig->stDebug.stCmd.uiEndPointer = BCHP_VICE2_CABAC_0_CMD_BUFF_END_ADDR;
   pstPlatformConfig->stDebug.stCmd.uiReadPointer = BCHP_VICE2_CABAC_0_CMD_BUFF_RD_ADDR;
   pstPlatformConfig->stDebug.stCmd.uiWritePointer = BCHP_VICE2_CABAC_0_CMD_BUFF_WR_ADDR;

   pstPlatformConfig->stDebug.stBin[0].uiBasePointer = BCHP_VICE2_CABAC_0_BIN_BUFF0_START_ADDR;
   pstPlatformConfig->stDebug.stBin[0].uiEndPointer = BCHP_VICE2_CABAC_0_BIN_BUFF0_END_ADDR;
   pstPlatformConfig->stDebug.stBin[0].uiReadPointer = BCHP_VICE2_CABAC_0_BIN_BUFF0_RD_ADDR;
   pstPlatformConfig->stDebug.stBin[0].uiWritePointer = BCHP_VICE2_CABAC_0_BIN_BUFF0_WR_ADDR;

   pstPlatformConfig->stDebug.stBin[1].uiBasePointer = BCHP_VICE2_CABAC_0_BIN_BUFF1_START_ADDR;
   pstPlatformConfig->stDebug.stBin[1].uiEndPointer = BCHP_VICE2_CABAC_0_BIN_BUFF1_END_ADDR;
   pstPlatformConfig->stDebug.stBin[1].uiReadPointer = BCHP_VICE2_CABAC_0_BIN_BUFF1_RD_ADDR;
   pstPlatformConfig->stDebug.stBin[1].uiWritePointer = BCHP_VICE2_CABAC_0_BIN_BUFF1_WR_ADDR;

   /* Set Box Mode Defaults */
   BVCE_Platform_P_GetTotalInstances( hBox, &pstPlatformConfig->stBox.uiTotalInstances );
   BVCE_Platform_P_GetTotalChannels( hBox, uiInstance, &pstPlatformConfig->stBox.uiTotalChannels );

   return BERR_TRACE( BERR_SUCCESS );
}

/* The following reset code is for 7425 B0 */
#include "bchp_vice2_mau_0.h"
BERR_Code
BVCE_Platform_P_PreReset(
   unsigned uiInstance,
   BREG_Handle hReg
   )
{
   uint32_t uiReg;
   BDBG_ASSERT( uiInstance < BVCE_PLATFORM_P_NUM_ENCODE_INSTANCES );

   BSTD_UNUSED( uiInstance );

   BDBG_ENTER( BVCE_Platform_P_PreReset );

#if (BCHP_CHIP == 7425) && (BCHP_VER >= BCHP_VER_B0)
   /* VICE Shutdown Sequence from Brad Delanghe 05/06/2011:
    *
    * Proposal for 7425B0:
    *  1. SCB transaction will be discarded immediately
    *    Note:
    *       a. This does not comply with the SCB bus spec
    *       b. This could potentially corrupt data for downstream blocks
    *       c. This will not work for PFRI transactions, due to the multi-cycle nature of the standard
    *  2. PFRI transactions need to be shut-down using a multi cycle procedure.
    *       a. Write 1 to MAU register FORCE_SHUTOFF to stop any future patch requests from ViCE internal clients
    *       b. Poll MAU core status register to confirm it is IDLE
    *       c. Issue sw_init
    *  3. ViCE design team is making efforts to verify that the sw_init can be issued at any time (while idle or while processing), but there is no guarantee that all possible cases can be verified.  Please use the random sw_init with caution.
    *
    */

   /* Check MAU Status */
   uiReg = BREG_Read32( hReg, BCHP_VICE2_MAU_0_STATUS );
   if ( BCHP_FIELD_ENUM( VICE2_MAU_0_STATUS, MAU_STATUS, BUSY) == BCHP_GET_FIELD_DATA( uiReg, VICE2_MAU_0_STATUS, MAU_STATUS ) )
   {
      BDBG_WRN(("MAU is BUSY before FORCE_SHUTOFF"));
   }

   /* Set Force Shutoff */
   uiReg = 0;
   BCHP_SET_FIELD_ENUM(uiReg, VICE2_MAU_0_FW_CONTROL, FORCE_SHUTOFF, ENABLE );
   BREG_Write32(hReg, BCHP_VICE2_MAU_0_FW_CONTROL, uiReg );

   /* SW7425-3816: SW INIT MAU_PFRI to clear the outstanding PFRI state */
   BKNI_Sleep(10);
   uiReg = 0;
   BCHP_SET_FIELD_DATA(uiReg, VICE2_MISC_SW_INIT_SET, MAU_PFRI, 1 );
   BREG_Write32(hReg, BCHP_VICE2_MISC_SW_INIT_SET, uiReg );
   BKNI_Sleep(1);
   BREG_Write32(hReg, BCHP_VICE2_MISC_SW_INIT_CLR, uiReg );

   /* Check status until idle */
   uiReg = BREG_Read32( hReg, BCHP_VICE2_MAU_0_STATUS );
   while ( BCHP_FIELD_ENUM( VICE2_MAU_0_STATUS, MAU_STATUS, BUSY) == BCHP_GET_FIELD_DATA( uiReg, VICE2_MAU_0_STATUS, MAU_STATUS ) )
   {
      BDBG_WRN(("MAU is BUSY..."));
      BKNI_Sleep(1);
      uiReg = BREG_Read32( hReg, BCHP_VICE2_MAU_0_STATUS );
   }
#else
   BSTD_UNUSED( hReg );
   BSTD_UNUSED( uiReg );
#endif

   BDBG_LEAVE( BVCE_Platform_P_PreReset );

   return BERR_TRACE( BERR_SUCCESS );
}

/* Enable UART for Vice Arc 0/1 */
BERR_Code
BVCE_Platform_P_EnableUART(
   BREG_Handle hReg
   )
{
   uint32_t uiReg;

   uiReg = BREG_Read32(
            hReg,
            BCHP_SUN_TOP_CTRL_UART_ROUTER_SEL
            );

   BCHP_SET_FIELD_ENUM(uiReg, SUN_TOP_CTRL_UART_ROUTER_SEL, port_6_cpu_sel, VICE2_ARC0);
   BCHP_SET_FIELD_ENUM(uiReg, SUN_TOP_CTRL_UART_ROUTER_SEL, port_5_cpu_sel, VICE2_ARC1);
   BCHP_SET_FIELD_ENUM(uiReg, SUN_TOP_CTRL_UART_ROUTER_SEL, port_4_cpu_sel, VICE2_ARC0);
   BCHP_SET_FIELD_ENUM(uiReg, SUN_TOP_CTRL_UART_ROUTER_SEL, port_3_cpu_sel, VICE2_ARC1);

   BREG_Write32(
            hReg,
            BCHP_SUN_TOP_CTRL_UART_ROUTER_SEL,
            uiReg
            );

   uiReg = BREG_Read32(
      hReg,
      BCHP_SUN_TOP_CTRL_PIN_MUX_CTRL_10
      );

   BCHP_SET_FIELD_ENUM( uiReg, SUN_TOP_CTRL_PIN_MUX_CTRL_10, gpio_041, TP_IN_21);
   BCHP_SET_FIELD_ENUM( uiReg, SUN_TOP_CTRL_PIN_MUX_CTRL_10, gpio_042, TP_OUT_22);
   BCHP_SET_FIELD_ENUM( uiReg, SUN_TOP_CTRL_PIN_MUX_CTRL_10, gpio_037, TP_IN_17);
   BCHP_SET_FIELD_ENUM( uiReg, SUN_TOP_CTRL_PIN_MUX_CTRL_10, gpio_038, TP_OUT_18);

   BREG_Write32(
            hReg,
            BCHP_SUN_TOP_CTRL_PIN_MUX_CTRL_10,
            uiReg
            );

   return BERR_SUCCESS;
}

const BVCE_P_GetFeatureInfo BVCE_P_MemcLUT[BVCE_PLATFORM_P_NUM_ENCODE_INSTANCES] =
{
   {BCHP_Feature_eMemCtrl1DDRDeviceTechCount, BCHP_Feature_eMemCtrl1DramWidthCount},
};
