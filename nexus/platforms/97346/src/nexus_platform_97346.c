/***************************************************************************
*     (c)2010-2013 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
*
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/
#include "nexus_platform_priv.h"
#include "bchp_common.h"
#include "bchp_sun_top_ctrl.h"
#include "priv/nexus_core.h"
#include "bchp_common.h"
#include "bchp_sun_top_ctrl.h"
#include "bchp_memc_ddr23_shim_addr_cntl_0.h"
#include "bchp_memc_ddr_0.h"
#if BCHP_CHIP==7344
BDBG_MODULE(nexus_platform_97344);
#else
BDBG_MODULE(nexus_platform_97346);
#endif

#if ((((BCHP_CHIP==7346) || (BCHP_CHIP==7344)) && (BCHP_VER >= BCHP_VER_B0)) || \
    ((BCHP_CHIP==73465) && (BCHP_VER >= BCHP_VER_A0))) \
    && (defined BDSP_VP6_SUPPORT || defined BDSP_H264_ENCODE_SUPPORT)
#include "bchp_memc_arb_0.h"
#endif

static void nexus_p_modifyDefaultMemoryConfigurationSettings(NEXUS_MemoryConfigurationSettings *pSettings)
{
#if NEXUS_HAS_VIDEO_DECODER
    unsigned int familyID = BREG_Read32(g_pPreInitState->hReg, BCHP_SUN_TOP_CTRL_CHIP_FAMILY_ID);

    if (familyID == 0x07346500) {
        unsigned int i;
        for (i=0; i < NEXUS_NUM_VIDEO_DECODERS; ++i) {
            /* Enable HEVC support for 73465. */
            pSettings->videoDecoder[i].supportedCodecs[NEXUS_VideoCodec_eH265] = true;
            /* If you require HEVC 10bit uncomment the line below. This will consume an extra 11MB per video decoder. */
            pSettings->videoDecoder[i].colorDepth = 10;
        }
        BDBG_MSG(("Enabling 1080p HEVC%s decode support",(pSettings->videoDecoder[0].colorDepth==10)?" 10bit":""));
    }
#else
    BSTD_UNUSED(pSettings);
#endif
}

void NEXUS_Platform_P_SetSpecificOps(struct NEXUS_PlatformSpecificOps *pOps)
{
    pOps->modifyDefaultMemoryConfigurationSettings = nexus_p_modifyDefaultMemoryConfigurationSettings;
}

void NEXUS_Platform_P_GetPlatformHeapSettings(NEXUS_PlatformSettings *pSettings, unsigned boxMode)
{
    BSTD_UNUSED(boxMode);
    /* kernel suggested boot options bmem=192M@64M bmem=512M@512M for boards with >750M memory
       bmem=192M@64M bmem=256M@512M for the boards with 512 Mbytes of memory */

    pSettings->heap[NEXUS_MEMC0_MAIN_HEAP].memcIndex = 0;
    pSettings->heap[NEXUS_MEMC0_MAIN_HEAP].subIndex = 0;
    pSettings->heap[NEXUS_MEMC0_MAIN_HEAP].size = -1;
    pSettings->heap[NEXUS_MEMC0_MAIN_HEAP].memoryType = NEXUS_MemoryType_eFull;

    pSettings->heap[NEXUS_MEMC0_PICTURE_BUFFER_HEAP].memcIndex = 0;
    pSettings->heap[NEXUS_MEMC0_PICTURE_BUFFER_HEAP].subIndex = 1;
    pSettings->heap[NEXUS_MEMC0_PICTURE_BUFFER_HEAP].size = 0; /* calculated */
    pSettings->heap[NEXUS_MEMC0_PICTURE_BUFFER_HEAP].memoryType = NEXUS_MemoryType_eDeviceOnly; /* unmapped */

    pSettings->heap[NEXUS_MEMC0_GRAPHICS_HEAP].memcIndex = 0;
    pSettings->heap[NEXUS_MEMC0_GRAPHICS_HEAP].subIndex = 1;
    pSettings->heap[NEXUS_MEMC0_GRAPHICS_HEAP].size = -1;
    pSettings->heap[NEXUS_MEMC0_GRAPHICS_HEAP].memoryType = NEXUS_MemoryType_eApplication; /* cached only */
    pSettings->heap[NEXUS_MEMC0_GRAPHICS_HEAP].heapType |= NEXUS_HEAP_TYPE_GRAPHICS;
}

NEXUS_HeapHandle NEXUS_Platform_P_GetFramebufferHeap(unsigned displayIndex)
{
    BSTD_UNUSED(displayIndex);
    return g_pCoreHandles->heap[NEXUS_MEMC0_GRAPHICS_HEAP].nexus;
}

NEXUS_Error NEXUS_Platform_P_InitBoard(void)
{
#if ((((BCHP_CHIP==7346) || (BCHP_CHIP==7344)) && (BCHP_VER >= BCHP_VER_B0)) || \
    ((BCHP_CHIP==73465) && (BCHP_VER >= BCHP_VER_A0))) \
    && (defined BDSP_VP6_SUPPORT || defined BDSP_H264_ENCODE_SUPPORT)

    uint32_t regData;

    regData = BREG_Read32(g_pCoreHandles->reg, BCHP_MEMC_ARB_0_CLIENT_INFO_51);
    if (BCHP_GET_FIELD_DATA(regData, MEMC_ARB_0_CLIENT_INFO_51, BO_VAL) != 0x113) {
        BDBG_WRN(("/******************************************************************************/"));
        BDBG_WRN(("MEMC0 RTS SETINGS ARE INCORRECT FOR RAAGA TRANSCODE. PLEASE UPDATE TO LATEST CFE"));
        BDBG_WRN(("/******************************************************************************/"));
    }
#endif
    return 0;
}

void NEXUS_Platform_P_UninitBoard(void)
{
}

#if NEXUS_PLATFORM_7418SFF_H
extern NEXUS_Error NEXUS_Platform_P_InitExternalRfm(const NEXUS_PlatformConfiguration *pConfig)
{
    NEXUS_GpioHandle channelGpio;
    NEXUS_GpioSettings gpioSettings;
    NEXUS_GpioStatus gpioStatus;
    bool channel4 = false;
    static uint8_t channel3Data[4] = {0x80, 0x00, 0x1e, 0xa3};
    static uint8_t channel4Data[4] = {0x80, 0x00, 0x21, 0xa3};
    uint8_t *pData;

    /* Make sure that platform initialized the i2c controllers */
    if ( pConfig->i2c[NEXUS_I2C_CHANNEL_EXT_RFM])
    {
    NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eAonStandard, &gpioSettings);
    gpioSettings.mode = NEXUS_GpioMode_eInput;
    channelGpio = NEXUS_Gpio_Open(NEXUS_GpioType_eAonStandard, 5, &gpioSettings);
    if ( channelGpio )
    {
        NEXUS_Gpio_GetStatus(channelGpio, &gpioStatus);
        channel4 = gpioStatus.value == NEXUS_GpioValue_eLow ? false : true;
        NEXUS_Gpio_Close(channelGpio);
    }

    BDBG_WRN(("Initializing external RFM for channel %u", channel4?4:3));
    pData = channel4?channel4Data:channel3Data;

    NEXUS_I2c_WriteNoAddr(pConfig->i2c[NEXUS_I2C_CHANNEL_EXT_RFM], 0x65, pData, 4);
    }

    return NEXUS_SUCCESS;
}
#endif

