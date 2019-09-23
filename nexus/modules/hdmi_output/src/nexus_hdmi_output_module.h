/******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *****************************************************************************/

#include "nexus_base.h"
#include "nexus_video_types.h"
#include "nexus_hdmi_output_thunks.h"
#include "nexus_hdmi_output.h"
#include "nexus_hdmi_output_hdcp.h"
#include "nexus_hdmi_output_init.h"
#include "priv/nexus_hdmi_output_standby_priv.h"
#include "nexus_hdmi_output_extra.h"
#include "bhdm.h"
#include "bhdm_edid.h"
#include "bhdm_monitor.h"
#include "nexus_platform_features.h"
#include "priv/nexus_core_video.h"
#include "priv/nexus_core_audio.h"
#include "priv/nexus_core_img_id.h"
#include "priv/nexus_core_img.h"
#include "priv/nexus_hdmi_output_priv.h"

#include "nexus_hdmi_output_dynrng_impl.h"
#include "nexus_hdmi_output_drmif_impl.h"
#if NEXUS_DBV_SUPPORT
#include "nexus_hdmi_output_dbv.h"
#include "nexus_hdmi_output_dbv_impl.h"
#endif

#if NEXUS_HAS_SAGE && NEXUS_HAS_HDCP_2X_SUPPORT
#include "bchp_common.h"
#ifndef BCHP_HDCP2_TX_HAE_INTR2_0_REG_START
#undef NEXUS_HAS_HDCP_2X_SUPPORT
#endif
#endif

#include "bhdm_hdcp.h"
#if NEXUS_HAS_SECURITY
#include "bhdcplib.h"
#include "bhdcplib_keyloader.h"
#endif

#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
#include "bsagelib_client.h"
#include "bsagelib_management.h"
#endif

#ifndef NEXUS_HDMI_OUTPUT_MODULE_H__
#define NEXUS_HDMI_OUTPUT_MODULE_H__

#ifdef NEXUS_MODULE_SELF
#error Cant be in two modules at the same time
#endif

#define NEXUS_MODULE_NAME hdmi_output
#define NEXUS_MODULE_SELF g_NEXUS_hdmiOutputModule

#if BHDM_OVERRIDE_DBG_MSG_LEVEL
#undef BDBG_MSG
#define BDBG_MSG(format) BDBG_P_PRINTMSG(BDBG_eMsg, format)
#undef BDBG_WRN
#define BDBG_WRN(format) BDBG_P_PRINTMSG(BDBG_eWrn, format)
#endif

/* Global Types */

typedef enum NEXUS_HdmiOutputState
{
    NEXUS_HdmiOutputState_eDisconnected,
    NEXUS_HdmiOutputState_eRxSenseCheck,
    NEXUS_HdmiOutputState_ePoweredDown,
    NEXUS_HdmiOutputState_ePoweredOn
} NEXUS_HdmiOutputState;

typedef struct NEXUS_HdmiOutputDisplaySettingsWrapper
{
    NEXUS_HdmiOutputDisplaySettings settings;
    bool valid;
} NEXUS_HdmiOutputDisplaySettingsWrapper;


typedef struct NEXUS_HdmiOutput
{
    NEXUS_OBJECT(NEXUS_HdmiOutput);
    bool opened;
    bool videoConnected;

#define RESOLVE_ALIAS(handle) do {(handle) = ((handle)->alias.master?(handle)->alias.master:(handle));}while(0)
#define IS_ALIAS(handle) ((handle)->alias.master != NULL)
    struct {
        NEXUS_HdmiOutputHandle master;
    } alias;

    NEXUS_HdmiOutputState rxState;

    NEXUS_VideoOutputObject videoConnector;
    NEXUS_AudioOutputObject audioConnector;
    BHDM_Handle hdmHandle;
    NEXUS_HdmiOutputOpenSettings openSettings;
    NEXUS_HdmiOutputSettings settings;
    NEXUS_HdmiOutputDisplaySettingsWrapper displaySettings;
    NEXUS_HdmiOutputSettings previousSettings;
    NEXUS_HdmiOutputExtraSettings extraSettings;
    NEXUS_HdmiOutputExtraStatus extraStatus;
    NEXUS_EventCallbackHandle hotplugEventCallback;
    NEXUS_EventCallbackHandle scrambleEventCallback;
    NEXUS_EventCallbackHandle avRateChangeEventCallback;
    NEXUS_TimerHandle powerTimer;
    NEXUS_TimerHandle postFormatChangeTimer;
    unsigned retryPostFormatChangeCount;

    bool forceSendRxIdList;
    bool hdcpRequiredPostFormatChange;
    bool formatChangeMute;
    bool avMuteSetting;
    bool hdcpStarted;
    uint8_t retryScrambleCount ;
    uint32_t pixelClkRatePreFormatChange;

    NEXUS_TimerHandle rxSenseCheckTimer;
    unsigned checkRxSenseCount ;
    bool forceDisconnect_isr;

    bool contentChangeOnly;

    BAVC_AudioSamplingRate sampleRate;
    BAVC_AudioBits audioBits;
    BAVC_AudioFormat audioFormat;
    unsigned audioNumChannels;    /* PCM only */

    NEXUS_TaskCallbackHandle hdcpFailureCallback;
    NEXUS_TaskCallbackHandle hdcpStateChangedCallback;
    NEXUS_TaskCallbackHandle hdcpSuccessCallback;
    NEXUS_TaskCallbackHandle hotplugCallback;
    NEXUS_TaskCallbackHandle mhlStandbyCallback;
    NEXUS_TaskCallbackHandle rxStatusCallback;
    BKNI_EventHandle notifyDisplayEvent;
    BKNI_EventHandle notifyAudioEvent;
    BKNI_EventHandle notifyHotplugEvent ;

    BHDM_HDCP_Version eHdcpVersion;
    NEXUS_HdmiOutputHdcpVersion hdcpVersionSelect;
#if NEXUS_HAS_SECURITY
    BHDCPlib_Handle hdcpHandle;
    NEXUS_EventCallbackHandle hdcpHotplugCallback;
    NEXUS_EventCallbackHandle riCallback;
    NEXUS_EventCallbackHandle pjCallback;
    NEXUS_TimerHandle hdcpTimer;
    NEXUS_TimerHandle hdcpFailedStartTimer;
    NEXUS_HdmiOutputHdcpSettings hdcpSettings;
    NEXUS_HdmiOutputHdcpKsv *pRevokedKsvs;
    uint16_t numRevokedKsvs;
    uint16_t revokedKsvsSize;
    BHDCPlib_State hdcp1xState;
    BHDCPlib_HdcpError hdcp1xError;
#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
    NEXUS_HdmiInputHandle hdmiInput;
    NEXUS_EventCallbackHandle hdcp2xReAuthRequestCallback;
    NEXUS_EventCallbackHandle hdcp2xAuthenticationStatusCallback;
    struct {
        uint8_t *buffer;
        uint32_t bufferSize;
        uint32_t length;
    } hdcp2xKeys;
#endif
#endif
    NEXUS_StandbyMode standbyMode ;
    BHDM_Settings hdmSettings;
    bool edidProcDebugDisplayed ;

    BHDM_EDID_STATE edidState ;
    bool invalidEdidReported ;
    bool edidHdmiDevice ;
    BHDM_EDID_RxVendorSpecificDB edidVendorSpecificDB ;

    NEXUS_HdmiOutputTxHardwareStatus txHwStatus ;
    BHDM_MONITOR_TxHwStatusExtra txHwStatusExtra ;
    NEXUS_HdmiOutputRxHardwareStatus rxHwStatus ;
    unsigned phyChangeRequestCounter ;

    /* HDCP Stats */
    struct {
        struct {
            struct {
                unsigned attemptCounter;
                unsigned passCounter;
                unsigned failCounter;
            } auth;
            unsigned bCapsReadFailureCounter;  /* i2c Read of Rx BCaps register */
            unsigned bksvReadFailureCounter;
            unsigned invalidBksvCounter;    /* unable to read Rx Bksv or read invalid/test Bksv */
        } hdcp1x;

#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
        struct {
            struct {
                unsigned attemptCounter;
                unsigned passCounter;
                unsigned failCounter;
            } auth;
            unsigned ReceiverIdReadError;

            unsigned validReauthReqCounter;   /* ReAuth requests when HDCP is enabled */
            unsigned invalidReauthReqCounter; /* ReAuth requests when HDCP is disabled (should not happen) */
            unsigned watchdogCounter;  /* reset of Sage */
            unsigned timeoutCounter;  /* HDCP auth step has timed out */
            unsigned akeSendCertFailures; /* total HDCP AkeSendCert failures */
        } hdcp22;
#endif
    } hdcpMonitor;

    struct {
        NEXUS_HdmiOutputCrcData *queue;
        unsigned size; /* num entries, not num bytes */
        unsigned rptr, wptr;
    } crc;

    char audioConnectorName[14];   /* HDMI OUTPUT %d */

    NEXUS_HdmiOutputDynrngData dynrng;

    NEXUS_HdmiVendorSpecificInfoFrame vsif;
    NEXUS_HdmiAviInfoFrame avif;

    uint16_t supported3DFormats[BFMT_VideoFmt_eMaxCount];
    BHDM_EDID_AudioDescriptor supportedAudioFormats[BAVC_AudioCompressionStd_eMax];

} NEXUS_HdmiOutput;

#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
typedef struct NEXUS_HdmiOutputMemoryBlock {
    size_t len;
    void *buf;
} NEXUS_HdmiOutputMemoryBlock;

#define NEXUS_HDMI_OUTPUT_SAGE_INDICATION_QUEUE_SIZE 5
typedef struct NEXUS_HdmiOutputIndicationData
{
    BHDCPlib_SageIndicationData sageIndication;
    BHDCPlib_Handle hHDCPlib;
} NEXUS_HdmiOutputIndicationData;


typedef struct NEXUS_HdmiOutput_SageData
{
    BSAGElib_ClientHandle sagelibClientHandle;
    BKNI_EventHandle eventWatchdogRecv;
    NEXUS_EventCallbackHandle eventWatchdogRecvCallback;
    BKNI_EventHandle eventTATerminated;
    NEXUS_EventCallbackHandle eventTATerminatedCallback;
    BKNI_EventHandle eventResponseRecv;
    BKNI_EventHandle eventIndicationRecv;
    NEXUS_EventCallbackHandle eventIndicationRecvCallback;
    uint32_t async_id;
    NEXUS_HdmiOutputIndicationData indicationData[NEXUS_HDMI_OUTPUT_SAGE_INDICATION_QUEUE_SIZE];
    unsigned indicationReadPtr;
    unsigned indicationWritePtr;
} NEXUS_HdmiOutput_SageData;
#endif

/* Global module handle & data */
extern NEXUS_ModuleHandle g_NEXUS_hdmiOutputModule;
extern NEXUS_HdmiOutputModuleSettings g_NEXUS_hdmiOutputModuleSettings;
#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
extern NEXUS_HdmiOutput_SageData g_NEXUS_hdmiOutputSageData;
extern NEXUS_HdmiOutputMemoryBlock g_hdcpTABlock;
#endif

/* Internal Private Routines */
#define NEXUS_HdmiOutput_P_IsRxConnected(output) (output->rxState != NEXUS_HdmiOutputState_eDisconnected)
#define NEXUS_HdmiOutput_P_IsRxPowered_isrsafe(output) (output->rxState == NEXUS_HdmiOutputState_ePoweredOn)

NEXUS_Error NEXUS_HdmiOutput_P_InitHdcp(NEXUS_HdmiOutputHandle output);
void NEXUS_HdmiOutput_P_UninitHdcp(NEXUS_HdmiOutputHandle output);
void NEXUS_HdmiOutput_P_HdcpPowerDown(NEXUS_HdmiOutputHandle output);

void NEXUS_HdmiOutputModule_Print(void);

void NEXUS_HdmiOutput_P_CheckHdcpVersion(NEXUS_HdmiOutputHandle output);

void NEXUS_HdmiOutput_P_CloseHdcp(NEXUS_HdmiOutputHandle output);

const char * NEXUS_HdmiOutput_P_ColorSpace_ToText(NEXUS_ColorSpace colorSpace);

/* Proxy conversion */
#define NEXUS_P_HDMI_OUTPUT_HDCP_KSV_SIZE(num) ((num)*sizeof(NEXUS_HdmiOutputHdcpKsv))

NEXUS_Error NEXUS_HdmiOutput_P_SetVideoSettings(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiOutputDisplaySettings *pstDisplaySettings
);

NEXUS_Error NEXUS_HdmiOutput_P_EnableDisplay(NEXUS_HdmiOutputHandle hdmiOutput, const BHDM_Settings * pSettings);
NEXUS_Error NEXUS_HdmiOutput_P_ApplyAviInfoFrame(NEXUS_HdmiOutputHandle handle);

void NEXUS_HdmiOutput_P_GetDefaultExtraSettings(NEXUS_HdmiOutputExtraSettings * pSettings);
void NEXUS_HdmiOutput_P_InitExtraStatus(NEXUS_HdmiOutputHandle output);

/* nexus_hdmi_output_hotplug */
void NEXUS_HdmiOutput_P_ProcessRxState(NEXUS_HdmiOutputHandle output, bool *deviceAttached, bool *rxSense);
void NEXUS_HdmiOutput_P_HotplugCallback(void *pContext);
void NEXUS_HdmiOutput_P_HotPlug_isr(void *context, int param, void *data) ;
void NEXUS_HdmiOutput_P_StopTimers(NEXUS_HdmiOutputHandle hdmiOutput) ;
void NEXUS_HdmiOutput_P_SetDisconnectedState(NEXUS_HdmiOutputHandle output);
NEXUS_Error NEXUS_HdmiOutput_P_SetTmdsSignalData( NEXUS_HdmiOutputHandle handle, bool tmdsDataEnable);
NEXUS_Error NEXUS_HdmiOutput_P_SetTmdsSignalClock( NEXUS_HdmiOutputHandle handle, bool tmdsClockEnable);
void NEXUS_HdmiOutput_TriggerCecCallback_priv(NEXUS_HdmiOutputHandle hdmiOutput);

void NEXUS_HdmiOutput_P_NotifyDisplay(NEXUS_HdmiOutputHandle hdmiOutput);

typedef struct NEXUS_HdmiOutputColorimetryParameters
{
    NEXUS_VideoFormat format ;
    bool xvYccEnabled ;
} NEXUS_HdmiOutputColorimetryParameters ;

/* Returns false if the format is not supported, true if it is */
void NEXUS_HdmiOutput_P_GetColorimetry(
    NEXUS_HdmiOutputHandle hdmiOutput,
    const NEXUS_HdmiOutputColorimetryParameters *parameters,
    NEXUS_HdmiOutputDisplaySettings * pDisplaySettings /* in/out */
    );

typedef struct NEXUS_HdmiOutputVideoSettings
{
    NEXUS_VideoFormat videoFormat;
    NEXUS_ColorSpace colorSpace;
    unsigned colorDepth;
} NEXUS_HdmiOutputVideoSettings;

NEXUS_Error NEXUS_HdmiOutput_P_ValidateVideoSettings(
    NEXUS_HdmiOutputHandle hdmiOutput,
    const NEXUS_HdmiOutputVideoSettings *requested,
    NEXUS_HdmiOutputVideoSettings *preferred /* [out] */
) ;

#if BDBG_DEBUG_BUILD
void NEXUS_HdmiOutput_P_ReportInvalidEdid(NEXUS_HdmiOutputHandle hdmiOutput, const char * tag);
#endif

NEXUS_Error NEXUS_HdmiOutput_P_DisableHdcpAuthentication( NEXUS_HdmiOutputHandle handle );

#endif /* #ifndef NEXUS_HDMI_OUTPUT_MODULE_H__ */
