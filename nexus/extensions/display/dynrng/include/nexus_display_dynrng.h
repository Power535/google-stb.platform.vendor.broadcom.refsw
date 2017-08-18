/******************************************************************************
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
 ******************************************************************************/
#ifndef NEXUS_DISPLAY_DYNRNG_H__
#define NEXUS_DISPLAY_DYNRNG_H__

#include "nexus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
Summary:
Processing type for dynamic range processing.
****************************************************************************/
typedef enum NEXUS_DynamicRangeProcessingType
{
    NEXUS_DynamicRangeProcessingType_ePlm, /* Broadcom Programmable Luminance Mapping */
    NEXUS_DynamicRangeProcessingType_eDolbyVision,
    NEXUS_DynamicRangeProcessingType_eTechnicolorPrime,
    NEXUS_DynamicRangeProcessingType_eMax
} NEXUS_DynamicRangeProcessingType;

/***************************************************************************
Summary:
Processing mode for dynamic range processing.
****************************************************************************/
typedef enum NEXUS_DynamicRangeProcessingMode
{
    NEXUS_DynamicRangeProcessingMode_eAuto, /* default, BRCM software will decide if it is necessary to enable or disable processing */
    NEXUS_DynamicRangeProcessingMode_eOff, /* disable processing */
    NEXUS_DynamicRangeProcessingMode_eMax
} NEXUS_DynamicRangeProcessingMode;

/***************************************************************************
Summary:
Dynamic range processing settings
****************************************************************************/
typedef struct NEXUS_DynamicRangeProcessingSettings {
    NEXUS_DynamicRangeProcessingMode processingModes[NEXUS_DynamicRangeProcessingType_eMax]; /* processing mode per processing type */
} NEXUS_DynamicRangeProcessingSettings;

/***************************************************************************
Summary:
Dynamic range processing capabilities
****************************************************************************/
typedef struct NEXUS_DynamicRangeProcessingCapabilities {
    bool typesSupported[NEXUS_DynamicRangeProcessingType_eMax]; /* processing types supported */
} NEXUS_DynamicRangeProcessingCapabilities;

/***************************************************************************
Summary:
Get the per-window video dynamic range processing capabilities for the display with HDMI attached
****************************************************************************/
void NEXUS_VideoWindow_GetDynamicRangeProcessingCapabilities(
    unsigned windowId,
    NEXUS_DynamicRangeProcessingCapabilities * pCapabilities /* [out] */
);

/***************************************************************************
Summary:
Get the current per-window video dynamic range processing settings for the display with HDMI attached
****************************************************************************/
void NEXUS_VideoWindow_GetDynamicRangeProcessingSettings(
    unsigned windowId,
    NEXUS_DynamicRangeProcessingSettings * pSettings /* [out] */
);

/***************************************************************************
Summary:
Set the per-window video dynamic range processing settings for the display with HDMI attached
****************************************************************************/
NEXUS_Error NEXUS_VideoWindow_SetDynamicRangeProcessingSettings(
    unsigned windowId,
    const NEXUS_DynamicRangeProcessingSettings * pSettings
);

/***************************************************************************
Summary:
Get the graphics dynamic range processing capabilities for the display with HDMI attached
****************************************************************************/
void NEXUS_Display_GetGraphicsDynamicRangeProcessingCapabilities(
    NEXUS_DynamicRangeProcessingCapabilities * pCapabilities /* [out] */
);

/***************************************************************************
Summary:
Get the current graphics dynamic range processing settings for the display with HDMI attached
****************************************************************************/
void NEXUS_Display_GetGraphicsDynamicRangeProcessingSettings(
    NEXUS_DynamicRangeProcessingSettings * pSettings /* [out] */
);

/***************************************************************************
Summary:
Set the graphics dynamic range processing settings for the display with HDMI attached
****************************************************************************/
NEXUS_Error NEXUS_Display_SetGraphicsDynamicRangeProcessingSettings(
    const NEXUS_DynamicRangeProcessingSettings * pSettings
);

#ifdef __cplusplus
}
#endif

#endif
