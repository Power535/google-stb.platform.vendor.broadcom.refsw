/***************************************************************************
*  Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
*
* API Description:
*   API name: Audio Module
*    Top-Level Audio Module APIs
*
***************************************************************************/
#include "nexus_audio_module.h"
#define RAAGA_DEBUG_LOG_CHANGES 1
#if BAPE_DSP_SUPPORT
#include "bdsp_raaga.h"
#endif
#if NEXUS_ARM_AUDIO_SUPPORT
#include "bdsp_arm.h"
#endif
#include "priv/nexus_core_preinit.h"
#if NEXUS_HAS_SECURITY
#include "nexus_security_datatypes.h"
#include "priv/nexus_security_regver_priv.h"
#endif

#if NEXUS_HAS_SECURITY
#define LOCK_SECURITY() NEXUS_Module_Lock( g_NEXUS_audioModuleData.internalSettings.modules.security )
#define UNLOCK_SECURITY() NEXUS_Module_Unlock( g_NEXUS_audioModuleData.internalSettings.modules.security )
#endif


BDBG_MODULE(nexus_audio_module);
BDBG_FILE_MODULE(nexus_audio_memest);

NEXUS_ModuleHandle g_NEXUS_audioModule;
NEXUS_AudioModuleData g_NEXUS_audioModuleData;


/******************************************************************************
The array to represent the value of volume in hex corresponding to the value
in DB. The application inputs the volume in terms of DB and the Corresponding
HEX value is mentioned here. The formula used for the same is:

    HEX = (2^23) * 10^(DB/20)

Note: 23 is the number of bits in the volume control field.

The volume can range from 0-1. 0 in hex corresponds to the 139 DB from the above
Formula. If application enters more than this value, it is forced to 0.
******************************************************************************/
static const int32_t g_db2linear[] =
{
    0x800000,   0x721482,   0x65AC8C,   0x5A9DF7,   0x50C335,
    0x47FACC,   0x4026E7,   0x392CED,   0x32F52C,   0x2D6A86,
    0x287A26,   0x241346,   0x2026F3,   0x1CA7D7,   0x198A13,
    0x16C310,   0x144960,   0x12149A,   0x101D3F,   0xE5CA1,
    0xCCCCC,    0xB6873,    0xA2ADA,    0x90FCB,    0x81385,
    0x732AE,    0x66A4A,    0x5B7B1,    0x51884,    0x48AA7,
    0x40C37,    0x39B87,    0x33718,    0x2DD95,    0x28DCE,
    0x246B4,    0x20756,    0x1CEDC,    0x19C86,    0x16FA9,
    0x147AE,    0x1240B,    0x10449,    0xE7FA,     0xCEC0,
    0xB844,     0xA43A,     0x925E,     0x8273,     0x7443,
    0x679F,     0x5C5A,     0x524F,     0x495B,     0x4161,
    0x3A45,     0x33EF,     0x2E49,     0x2940,     0x24C4,
    0x20C4,     0x1D34,     0x1A07,     0x1732,     0x14AC,
    0x126D,     0x106C,     0xEA3,      0xD0B,      0xBA0,
    0xA5C,      0x93C,      0x83B,      0x755,      0x689,
    0x5D3,      0x531,      0x4A0,      0x420,      0x3AD,
    0x346,      0x2EB,      0x29A,      0x251,      0x211,
    0x1D7,      0x1A4,      0x176,      0x14D,      0x129,
    0xEC,       0xD2,       0xA7,       0x95,       0x84,
    0x76,       0x69,       0x5E,       0x53,       0x4A,
    0x42,       0x3B,       0x34,       0x2F,       0x2A,
    0x25,       0x21,       0x1D,       0x1A,       0x17,
    0x15,       0x12,       0x10,       0xE,        0xD,
    0xB,        0xA,        0x9,        0x8,        0x7,
    0x6,        0x5,        0x5,        0x4,        0x4,
    0x3,        0x3,        0x2,        0x2,        0x2,
    0x2,        0x1,        0x1,        0x1,        0x1,
    0x1,        0x1,        0x1,        0x0
};


/* required for the debug output */
extern NEXUS_AudioDecoderHandle g_decoders[NEXUS_NUM_AUDIO_DECODERS];

static NEXUS_AudioDolbyCodecVersion NEXUS_GetDolbyAudioCodecVersion(void);

static NEXUS_Error secureFirmwareAudio( BDSP_Handle hRagga );

static void NEXUS_P_GetAudioCapabilities(NEXUS_AudioCapabilities *pCaps);

static void NEXUS_AudioModule_Print(void)
{
#if BDBG_DEBUG_BUILD
    unsigned i,j;
    BDBG_LOG(("Audio:"));

    BDBG_LOG((" handles: ape:%p dsp:%p", (void *)g_NEXUS_audioModuleData.apeHandle, (void *)g_NEXUS_audioModuleData.dspHandle));
    BDBG_LOG((" img ctxt:%p", (void *)g_NEXUS_audioModuleData.pImageContext));
    BDBG_LOG((" settings: wd:%d id:%d",g_NEXUS_audioModuleData.settings.watchdogEnabled,g_NEXUS_audioModuleData.settings.independentDelay));

    for (i=0; i < g_NEXUS_audioModuleData.capabilities.numDecoders; i++) {
        NEXUS_AudioDecoderStatus status;
        NEXUS_AudioDecoderHandle handle = g_decoders[i];
        if (handle) {
            NEXUS_Error errCode;
            NEXUS_PidChannelStatus pidChannelStatus;
            BAPE_DecoderTsmSettings tsmSettings;

            errCode = NEXUS_AudioDecoder_GetStatus(handle, &status);
            if( errCode )
            {
                BDBG_LOG((" channel%d: can't get audio decoder status, errCode=%x", i, errCode));
                continue;
            }
            if (handle->programSettings.pidChannel) {
                errCode = NEXUS_PidChannel_GetStatus(handle->programSettings.pidChannel, &pidChannelStatus);
                if( errCode )
                {
                    BDBG_LOG((" channel%d: can't get pidChannel status for pidCh=%p, errCode=%x", i, (void *)handle->programSettings.pidChannel, errCode));
                    continue;
                }
            }
            else {
                BKNI_Memset(&pidChannelStatus, 0, sizeof(pidChannelStatus));
            }
            BAPE_Decoder_GetTsmSettings(handle->channel, &tsmSettings);
            BDBG_LOG((" channel%d: (%p) %s", i, (void *)handle->channel, status.locked ? "locked " : ""));
            BDBG_LOG(("  dsp index=%d started=%s, codec=%d, pid=0x%x, pidCh=%p, stcCh=%p",handle->openSettings.dspIndex,
                status.started == NEXUS_AudioRunningState_eStarted ? "started" : (status.started == NEXUS_AudioRunningState_eSuspended ? "suspended" : "stopped"),
                status.started ? status.codec : 0, pidChannelStatus.pid, (void *)handle->programSettings.pidChannel, (void *)handle->programSettings.stcChannel));
            BDBG_LOG(("  fifo: %d/%d (%d%%), queued: %d", status.fifoDepth, status.fifoSize, status.fifoSize ? status.fifoDepth*100/status.fifoSize : 0, status.queuedFrames));
            BDBG_LOG(("  TSM: %s pts=%#x pts_stc_diff=%d pts_offset=%#x errors=%d", status.tsm ? "enabled" : "disabled", status.pts, status.ptsStcDifference,
                tsmSettings.ptsOffset, status.ptsErrorCount));
            BDBG_LOG(("  Decode: frames decoded=%d errors=%d", status.framesDecoded, status.frameErrors));
            BDBG_LOG(("  watchdogs: %d", status.numWatchdogs));
            for (j=0; j < NEXUS_AudioDecoderConnectorType_eMax; j++)
            {
                NEXUS_AudioMixerHandle mixerHandle = NEXUS_AudioInput_P_LocateMixer( &handle->connectors[j], NULL);
                while (mixerHandle)
                {
                    NEXUS_AudioInputHandle connector = NEXUS_AudioMixer_GetConnector(mixerHandle);
                    BDBG_LOG(("  %s: (%p)", connector->pName, (void *)mixerHandle));
                    mixerHandle = NEXUS_AudioInput_P_LocateMixer( &handle->connectors[j], mixerHandle);
                }
            }
        }
    }

    for (i=0; i < g_NEXUS_audioModuleData.capabilities.numPlaybacks; i++) {
        NEXUS_AudioPlaybackHandle playbackHandle = NEXUS_AudioPlayback_P_GetPlaybackByIndex(i);
        if (playbackHandle)
        {
            NEXUS_AudioInputHandle connector = NEXUS_AudioPlayback_GetConnector(playbackHandle);
            NEXUS_AudioMixerHandle mixerHandle = NEXUS_AudioInput_P_LocateMixer(connector, NULL);
            NEXUS_AudioPlaybackSettings playbackSettings;
            NEXUS_AudioPlayback_GetSettings(playbackHandle, &playbackSettings);
            BDBG_LOG((" PLAYBACK%d (%p)",i, (void *)playbackHandle));
            BDBG_LOG(("  started=%c", NEXUS_AudioPlayback_P_IsRunning(playbackHandle)?'y':'n'));
            while (mixerHandle)
            {
                NEXUS_AudioInputHandle mixerConnector = NEXUS_AudioMixer_GetConnector(mixerHandle);
                BDBG_LOG(("  %s: (%p)", mixerConnector->pName, (void *)mixerHandle));
                mixerHandle = NEXUS_AudioInput_P_LocateMixer(connector, mixerHandle);
            }
        }
    }

    for (i=0; i < g_NEXUS_audioModuleData.capabilities.numMixers; i++) {
        NEXUS_AudioMixerHandle mixerHandle = NEXUS_AudioMixer_P_GetMixerByIndex(i);
        if (mixerHandle)
        {
            NEXUS_AudioMixerSettings * pMixerSettings;
            pMixerSettings = BKNI_Malloc(sizeof(NEXUS_AudioMixerSettings));
            if ( pMixerSettings == NULL )
            {
                BDBG_ERR(("Unable to allocate memory for NEXUS_AudioMixerSettings"));
                BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
                return;
            }
            NEXUS_AudioMixer_GetSettings(mixerHandle, pMixerSettings);

            BDBG_LOG((" MIXER%d: (%p)", i, (void *)mixerHandle));
            if (pMixerSettings->mixUsingDsp)
            {
                BDBG_LOG(("  type: DSP, DSP Index %d, started=%c",
                    pMixerSettings->dspIndex, NEXUS_AudioMixer_P_IsStarted(mixerHandle)?'y':'n'));
            }
            else
            {
                BDBG_LOG(("  type: FMM"));
            }

            BKNI_Free(pMixerSettings);
            pMixerSettings = NULL;
        }
    }

    BDBG_LOG(("DSP Firmware Version %s", g_NEXUS_audioModuleData.firmwareVersionInfo));

#endif
}

void NEXUS_AudioModule_GetDefaultSettings(const struct NEXUS_Core_PreInitState *preInitState, NEXUS_AudioModuleSettings *pSettings)
{
    BAPE_Settings apeSettings;
#if BAPE_DSP_SUPPORT
    BDSP_RaagaSettings raagaSettings;
#endif
    const char *pEnv;
    unsigned i;
    unsigned num_encoders = 0;

    BKNI_Memset(pSettings, 0, sizeof(NEXUS_AudioModuleSettings));
    BAPE_GetDefaultSettings(&apeSettings);
    pSettings->watchdogEnabled = true;
    pSettings->maxAudioDspTasks = apeSettings.maxDspTasks;
    if ( NULL != (pEnv = NEXUS_GetEnv("audio_max_delay")) )
    {
        pSettings->maxIndependentDelay = NEXUS_atoi(pEnv);
        BDBG_WRN(("Maximum audio delay set to %u ms", pSettings->maxIndependentDelay));
    }
    else
    {
        pSettings->maxIndependentDelay = apeSettings.maxIndependentDelay;
    }
    pSettings->maxPcmSampleRate = apeSettings.maxPcmSampleRate;

    if (preInitState) {
        /* total VCE channels */
        for (i=0;i<BBOX_VCE_MAX_INSTANCE_COUNT;i++) {
            /* convert bitmask to count */
            unsigned ch = preInitState->boxConfig.stVce.stInstance[i].uiChannels;
            while (ch) {
                if (ch & 0x1) num_encoders++;
                ch >>= 1;
            }
        }
    }
    pSettings->numPcmBuffers = apeSettings.numPcmBuffers + (num_encoders?num_encoders-1:0);
    pSettings->numCompressedBuffers = apeSettings.numCompressedBuffers;
    pSettings->numCompressed4xBuffers = apeSettings.numCompressed4xBuffers;
    pSettings->numCompressed16xBuffers = apeSettings.numCompressed16xBuffers;
    pSettings->numRfEncodedPcmBuffers = apeSettings.numRfEncodedPcmBuffers;
    pSettings->independentDelay = pSettings->maxIndependentDelay > 0 ? true : false;
    BDBG_CASSERT(NEXUS_AudioLoudnessEquivalenceMode_eNone == (int)BAPE_LoudnessEquivalenceMode_eNone);
    BDBG_CASSERT(NEXUS_AudioLoudnessEquivalenceMode_eAtscA85 == (int)BAPE_LoudnessEquivalenceMode_eAtscA85);
    BDBG_CASSERT(NEXUS_AudioLoudnessEquivalenceMode_eEbuR128 == (int)BAPE_LoudnessEquivalenceMode_eEbuR128);
    BDBG_CASSERT(NEXUS_AudioLoudnessEquivalenceMode_eMax == (int)BAPE_LoudnessEquivalenceMode_eMax);
    pSettings->loudnessMode = (NEXUS_AudioLoudnessEquivalenceMode)apeSettings.loudnessMode;
    pSettings->heapIndex = NEXUS_MAX_HEAPS;
    pSettings->firmwareHeapIndex = NEXUS_MAX_HEAPS;

#if BAPE_DSP_SUPPORT
    BDSP_Raaga_GetDefaultSettings(&raagaSettings);
    for ( i = 0; i < NEXUS_AudioDspDebugType_eMax; i++ )
    {
        pSettings->dspDebugSettings.typeSettings[i].enabled = false;    /* Disable all debug by default */
        pSettings->dspDebugSettings.typeSettings[i].bufferSize = raagaSettings.debugSettings[i].bufferSize;
    }

    for ( i = 0; i < NEXUS_AudioDspAlgorithmType_eMax; i++ )
    {
        pSettings->dspAlgorithmSettings.typeSettings[i].count = raagaSettings.maxAlgorithms[i];
    }
#endif
}

void NEXUS_AudioModule_GetDefaultInternalSettings(
    NEXUS_AudioModuleInternalSettings *pSettings    /* [out] */
    )
{
    BKNI_Memset(pSettings, 0, sizeof(NEXUS_AudioModuleInternalSettings));
}

#if BAPE_DSP_SUPPORT
static void NEXUS_AudioModule_P_PopulateRaagaOpenSettings(
    const BBOX_Config *boxConfig,
    const NEXUS_AudioModuleSettings *pSettings,
    BDSP_RaagaSettings * raagaSettings /* out */
    )
{
    unsigned i;

    #if 1
    {
        static BIMG_Interface imgInterface;
        /* TODO: Connect IMG to DSP interface */
        if ( Nexus_Core_P_Img_Create(NEXUS_CORE_IMG_ID_RAP, &g_NEXUS_audioModuleData.pImageContext, &imgInterface) == NEXUS_SUCCESS )
        {
            BDBG_WRN(("FW download used"));
            raagaSettings->pImageContext = g_NEXUS_audioModuleData.pImageContext;
            raagaSettings->pImageInterface = &imgInterface;
        }
    }
    #endif

    for ( i = 0; i < NEXUS_AudioDspDebugType_eMax; i++ )
    {
        raagaSettings->debugSettings[i].enabled = pSettings->dspDebugSettings.typeSettings[i].enabled;
        raagaSettings->debugSettings[i].bufferSize = pSettings->dspDebugSettings.typeSettings[i].bufferSize;

        if ( pSettings->dspDebugSettings.typeSettings[i].enabled )
        {
            BDBG_WRN(("Enabling audio FW debug type %u [%s]", i, (i == NEXUS_AudioDspDebugType_eDramMessage)?"DRAM Message":
                                                                 (i == NEXUS_AudioDspDebugType_eUartMessage)?"UART Message":
                                                                 (i == NEXUS_AudioDspDebugType_eCoreDump)?"Core Dump":
                                                                  "Target Print"));
        }
    }

    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioDecode == (int)BDSP_AlgorithmType_eAudioDecode);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioPassthrough == (int)BDSP_AlgorithmType_eAudioPassthrough);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioEncode == (int)BDSP_AlgorithmType_eAudioEncode);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioMixer == (int)BDSP_AlgorithmType_eAudioMixer);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioEchoCanceller == (int)BDSP_AlgorithmType_eAudioEchoCanceller);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eAudioProcessing == (int)BDSP_AlgorithmType_eAudioProcessing);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eVideoDecode == (int)BDSP_AlgorithmType_eVideoDecode);
    BDBG_CASSERT(NEXUS_AudioDspAlgorithmType_eVideoEncode == (int)BDSP_AlgorithmType_eVideoEncode);
    for ( i = 0; i < NEXUS_AudioDspAlgorithmType_eMax; i++ )
    {
        raagaSettings->maxAlgorithms[i] = pSettings->dspAlgorithmSettings.typeSettings[i].count;
    }

    raagaSettings->NumDsp = boxConfig->stAudio.numDsps;
}
#endif

NEXUS_ModuleHandle NEXUS_AudioModule_Init(
    const NEXUS_AudioModuleSettings *pSettings,
    const NEXUS_AudioModuleInternalSettings *pInternalSettings
    )
{
    BERR_Code errCode;
    NEXUS_ModuleSettings moduleSettings;
    BAPE_Settings apeSettings;
    #if BAPE_DSP_SUPPORT
    BDSP_RaagaSettings raagaSettings;
    #endif
    #if NEXUS_ARM_AUDIO_SUPPORT
    BDSP_ArmSettings armSettings;
    #endif
    unsigned heapIndex, fwHeapIndex;
    BCHP_MemoryInfo memoryInfo;
    unsigned i;

    NEXUS_Module_GetDefaultSettings(&moduleSettings);
    moduleSettings.priority = NEXUS_ModulePriority_eLow; /* decoder interface is slow */
    moduleSettings.dbgPrint = NEXUS_AudioModule_Print;
    moduleSettings.dbgModules = "nexus_audio_module";
    g_NEXUS_audioModule = NEXUS_Module_Create("audio", &moduleSettings);

    if ( NULL == g_NEXUS_audioModule )
    {
        errCode=BERR_TRACE(BERR_OS_ERROR);
        goto err_module;
    }
    NEXUS_LockModule();

    if ( NULL == pSettings ) {
        BERR_TRACE(NEXUS_NOT_SUPPORTED);
        return NULL;
    }

    if ( NULL == pInternalSettings ) {
        BERR_TRACE(NEXUS_NOT_SUPPORTED);
        return NULL;
    }

    g_NEXUS_audioModuleData.settings = *pSettings;
    g_NEXUS_audioModuleData.internalSettings = *pInternalSettings;

    heapIndex = pSettings->heapIndex;
    if ( heapIndex >= NEXUS_MAX_HEAPS )
    {
        heapIndex = g_pCoreHandles->defaultHeapIndex;
    }
    if ( heapIndex >= NEXUS_MAX_HEAPS ||
         g_pCoreHandles->heap[heapIndex].mem == NULL )
    {
        BDBG_ERR(("Invalid heap provided."));
        goto err_heap;
    }
    fwHeapIndex = pSettings->firmwareHeapIndex;
    if ( fwHeapIndex >= NEXUS_MAX_HEAPS )
    {
        fwHeapIndex = g_pCoreHandles->defaultHeapIndex;
    }
    if ( fwHeapIndex >= NEXUS_MAX_HEAPS ||
         g_pCoreHandles->heap[fwHeapIndex].mem == NULL )
    {
        BDBG_ERR(("Invalid firmware heap provided."));
        goto err_heap;
    }

#if BAPE_DSP_SUPPORT
    BDSP_Raaga_GetDefaultSettings(&raagaSettings);

    NEXUS_AudioModule_P_PopulateRaagaOpenSettings(g_pCoreHandles->boxConfig, pSettings, &raagaSettings);
    g_NEXUS_audioModuleData.numDsps = raagaSettings.NumDsp;
    g_NEXUS_audioModuleData.verifyFirmware = false; /*may be updated below.*/

   #if NEXUS_HAS_SECURITY
    /* check if firmware verification is required for Raaga0/1 */
    LOCK_SECURITY();
    {
        g_NEXUS_audioModuleData.verifyFirmware = NEXUS_Security_RegionVerification_IsRequired_priv( NEXUS_SecurityRegverRegionID_eRaaga0 );
        if( !g_NEXUS_audioModuleData.verifyFirmware && g_NEXUS_audioModuleData.numDsps > 1 ) {
            g_NEXUS_audioModuleData.verifyFirmware = NEXUS_Security_RegionVerification_IsRequired_priv( NEXUS_SecurityRegverRegionID_eRaaga1 );
        }
    }
    UNLOCK_SECURITY();
   #endif

    raagaSettings.authenticationEnabled = g_NEXUS_audioModuleData.verifyFirmware;

    BCHP_GetMemoryInfo(g_pCoreHandles->chp, &memoryInfo);
    for (i=0;i<NEXUS_MAX_MEMC;i++) {
        /* DSP is unable to access above region[0] on each MEMC */
        raagaSettings.memc[i].baseAddress = g_pCoreHandles->memoryLayout.memc[i].region[0].addr;
        raagaSettings.memc[i].size = g_pCoreHandles->memoryLayout.memc[i].region[0].size;
        raagaSettings.memc[i].stripeWidth = memoryInfo.memc[i].ulStripeWidth;
    }

    BDBG_MSG(("Calling BDSP_Raaga_Open"));
    errCode = BDSP_Raaga_Open(&g_NEXUS_audioModuleData.dspHandle,
                              g_pCoreHandles->chp,
                              g_pCoreHandles->reg,
                              g_pCoreHandles->heap[fwHeapIndex].mma,
                              g_pCoreHandles->bint,
                              g_pCoreHandles->tmr,
                              g_pCoreHandles->box,
                              &raagaSettings);
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_dsp;
    }

    if ( g_NEXUS_audioModuleData.verifyFirmware )
    {
        errCode = secureFirmwareAudio(g_NEXUS_audioModuleData.dspHandle);
        if ( errCode ) { (void)BERR_TRACE(errCode); goto err_dsp; }

        BDBG_MSG(("Calling BDSP_Raaga_Initialize"));
        errCode = BDSP_Raaga_Initialize( g_NEXUS_audioModuleData.dspHandle );
        if ( errCode ) { (void)BERR_TRACE(errCode); goto err_dsp; }
    }

    #if NEXUS_ARM_AUDIO_SUPPORT
    if ( !NEXUS_GetEnv("disable_arm_audio") )
    {
        BTEE_InstanceStatus teeStatus;
        BDBG_ASSERT(NULL != g_pCoreHandles->tee);

        /* Check Astra compability */
        errCode = BTEE_Instance_GetStatus(g_pCoreHandles->tee, &teeStatus);
        if ( errCode ) { (void)BERR_TRACE(errCode); goto err_dsp; }

        BDBG_LOG(("Astra Version %u.%u.%u, Astra Enabled %d", teeStatus.version.major, teeStatus.version.minor, teeStatus.version.build, teeStatus.enabled));
        if ( teeStatus.enabled /* && version matches bdsp */ )
        {
            BDSP_Arm_GetDefaultSettings(&armSettings);
            /* TBDARM - customize ARM settings
            NEXUS_AudioModule_P_PopulateArmOpenSettings(pSettings, &armSettings); */
            armSettings.hBteeInstance = g_pCoreHandles->tee;
            errCode = BDSP_Arm_Open(&g_NEXUS_audioModuleData.armHandle,
                                    g_pCoreHandles->chp,
                                    g_pCoreHandles->reg,
                                    g_pCoreHandles->heap[fwHeapIndex].mma,
                                    g_pCoreHandles->bint,
                                    g_pCoreHandles->tmr,
                                    &armSettings);
            if ( errCode )
            {
                (void)BERR_TRACE(errCode);
                goto err_dsp;
            }
        }
    }
    #endif
    #endif /* BAPE_DSP_SUPPORT */

    BAPE_GetDefaultSettings(&apeSettings);
    apeSettings.maxDspTasks = pSettings->maxAudioDspTasks;
    apeSettings.maxIndependentDelay = pSettings->independentDelay ? pSettings->maxIndependentDelay : 0;
    apeSettings.maxPcmSampleRate = pSettings->maxPcmSampleRate;
    apeSettings.numPcmBuffers = pSettings->numPcmBuffers;
    apeSettings.numCompressedBuffers = pSettings->numCompressedBuffers;
    apeSettings.numCompressed4xBuffers = pSettings->numCompressed4xBuffers;
    apeSettings.numCompressed16xBuffers = pSettings->numCompressed16xBuffers;
    apeSettings.numRfEncodedPcmBuffers = pSettings->numRfEncodedPcmBuffers;
    apeSettings.loudnessMode = (BAPE_LoudnessEquivalenceMode)pSettings->loudnessMode;
    for (i=0;i<NEXUS_MAX_MEMC;i++) {
        /* APE is unable to access above region[0] on each MEMC */
        apeSettings.memc[i].baseAddress = g_pCoreHandles->memoryLayout.memc[i].region[0].addr;
    }

    if ( NEXUS_GetEnv("audio_ramp_disabled") )
    {
        apeSettings.rampPcmSamples = false;
    }

    BDBG_MSG(("Calling BAPE_Open"));
    errCode = BAPE_Open(&NEXUS_AUDIO_DEVICE_HANDLE,
                        g_pCoreHandles->chp,
                        g_pCoreHandles->reg,
                        g_pCoreHandles->heap[heapIndex].mma,
                        g_pCoreHandles->bint,
                        g_pCoreHandles->tmr,
                        g_NEXUS_audioModuleData.dspHandle,
                        g_NEXUS_audioModuleData.armHandle,
                        &apeSettings);
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_ape;
    }

    /* Save Capabilites */
    NEXUS_P_GetAudioCapabilities(&g_NEXUS_audioModuleData.capabilities);

    BDBG_MSG(("Calling NEXUS_AudioDecoder_P_Init"));
    errCode = NEXUS_AudioDecoder_P_Init();
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_decoder;
    }

    BDBG_MSG(("Calling NEXUS_AudioInput_P_Init"));
    errCode = NEXUS_AudioInput_P_Init();
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_input;
    }

#if NEXUS_NUM_DSP_VIDEO_DECODERS
    errCode = NEXUS_AudioModule_P_InitDspVideoDecoder();
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_video;
    }
#endif

#if NEXUS_NUM_DSP_VIDEO_ENCODERS
    errCode = NEXUS_AudioModule_P_InitDspVideoEncoder();
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_encoder;
    }
#endif

#if BDBG_DEBUG_BUILD
    errCode = NEXUS_AudioDebug_Init();
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_debug;
    }
#endif

    NEXUS_UnlockModule();
    return g_NEXUS_audioModule;
err_debug:
#if NEXUS_NUM_DSP_VIDEO_ENCODERS
    NEXUS_AudioModule_P_UninitDspVideoEncoder();
err_encoder:
#if NEXUS_NUM_DSP_VIDEO_DECODERS
    NEXUS_AudioModule_P_UninitDspVideoDecoder();
err_video:
#endif
    NEXUS_AudioInput_P_Uninit();
#else
#if NEXUS_NUM_DSP_VIDEO_DECODERS
err_video:
    NEXUS_AudioInput_P_Uninit();
#endif
#endif
err_input:
    NEXUS_AudioDecoder_P_Uninit();
err_decoder:
    BAPE_Close(NEXUS_AUDIO_DEVICE_HANDLE);
err_ape:
#if BAPE_DSP_SUPPORT
    BDSP_Close(g_NEXUS_audioModuleData.dspHandle);
err_dsp:
#endif
    NEXUS_UnlockModule();
    NEXUS_Module_Destroy(g_NEXUS_audioModule);
err_heap:
    if (g_NEXUS_audioModuleData.pImageContext) {
        Nexus_Core_P_Img_Destroy(g_NEXUS_audioModuleData.pImageContext);
    }
err_module:
    return NULL;
}

void NEXUS_AudioModule_Uninit(void)
{
    NEXUS_LockModule();
#if BDBG_DEBUG_BUILD
    NEXUS_AudioDebug_Uninit();
#endif
#if NEXUS_NUM_DSP_VIDEO_ENCODERS
    NEXUS_AudioModule_P_UninitDspVideoEncoder();
#endif
#if NEXUS_NUM_DSP_VIDEO_DECODERS
    NEXUS_AudioModule_P_UninitDspVideoDecoder();
#endif
    NEXUS_AudioInput_P_Uninit();
    NEXUS_AudioDecoder_P_Uninit();

    if ( NEXUS_AUDIO_DEVICE_HANDLE )
    {
        BAPE_Close(NEXUS_AUDIO_DEVICE_HANDLE);
    }
    #if BAPE_DSP_SUPPORT
    if ( g_NEXUS_audioModuleData.armHandle )
    {
        BDSP_Close(g_NEXUS_audioModuleData.armHandle);
    }
    if ( g_NEXUS_audioModuleData.dspHandle )
    {
        BDSP_Close(g_NEXUS_audioModuleData.dspHandle);
    }
    #endif

   #if NEXUS_HAS_SECURITY
    if ( g_NEXUS_audioModuleData.verifyFirmware )
    {
        LOCK_SECURITY();
        NEXUS_Security_RegionVerifyDisable_priv( NEXUS_SecurityRegverRegionID_eRaaga0 );
        UNLOCK_SECURITY();

        if( g_NEXUS_audioModuleData.numDsps > 1 )
        {
            LOCK_SECURITY();
            NEXUS_Security_RegionVerifyDisable_priv( NEXUS_SecurityRegverRegionID_eRaaga1 );
            UNLOCK_SECURITY();
        }
    }
   #endif

    if (g_NEXUS_audioModuleData.pImageContext) {
        Nexus_Core_P_Img_Destroy(g_NEXUS_audioModuleData.pImageContext);
    }

    NEXUS_UnlockModule();
    NEXUS_Module_Destroy(g_NEXUS_audioModule);
}

void NEXUS_AudioModule_GetRampStepSettings(
    NEXUS_AudioRampStepSettings *pRampStepSettings      /* [out] ramping step size for scale change for all output ports */
    )
{
    uint32_t val;
    if ( NULL == pRampStepSettings )
    {
        return;
    }
    BAPE_GetOutputVolumeRampStep(NEXUS_AUDIO_DEVICE_HANDLE, &val);
    pRampStepSettings->mixerRampStep = val;
    BAPE_GetSampleRateConverterRampStep(NEXUS_AUDIO_DEVICE_HANDLE, &val);
    pRampStepSettings->srcRampStep = val;
}

NEXUS_Error NEXUS_AudioModule_SetRampStepSettings(
    const NEXUS_AudioRampStepSettings *pRampStepSettings  /* ramping step size for scale change for all output ports */
    )
{
    BERR_Code errCode;

    if ( NULL == pRampStepSettings )
    {
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    errCode = BAPE_SetOutputVolumeRampStep(NEXUS_AUDIO_DEVICE_HANDLE, pRampStepSettings->mixerRampStep);
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }

    errCode = BAPE_SetSampleRateConverterRampStep(NEXUS_AUDIO_DEVICE_HANDLE, pRampStepSettings->srcRampStep);
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }

    return BERR_SUCCESS;
}

NEXUS_Error NEXUS_AudioModule_EnableExternalMclk(
    unsigned mclkIndex,
    NEXUS_AudioOutputPll pllIndex,
    NEXUS_ExternalMclkRate mclkRate
    )
{
    BERR_Code       errCode;
    BAPE_Pll        bapePll;
    BAPE_MclkRate   bapeMclkRate;

    switch ( pllIndex )
    {
    case 0:
        bapePll = BAPE_Pll_e0;
        break;
    case 1:
        bapePll = BAPE_Pll_e1;
        break;
    case 2:
        bapePll = BAPE_Pll_e2;
        break;

    default:
        BDBG_ERR(("pllIndex:%u is invalid", pllIndex));
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    switch( mclkRate )
    {
    case NEXUS_ExternalMclkRate_e128Fs:
        bapeMclkRate = BAPE_MclkRate_e128Fs;
        break;

    case NEXUS_ExternalMclkRate_e256Fs:
        bapeMclkRate = BAPE_MclkRate_e256Fs;
        break;

    case NEXUS_ExternalMclkRate_e384Fs:
        bapeMclkRate = BAPE_MclkRate_e384Fs;
        break;

    case NEXUS_ExternalMclkRate_e512Fs:
        bapeMclkRate = BAPE_MclkRate_e512Fs;
        break;

    default:
        BDBG_ERR(("mclkRate:%u is invalid", mclkRate));
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    errCode = BAPE_Pll_EnableExternalMclk( NEXUS_AUDIO_DEVICE_HANDLE, bapePll, mclkIndex, bapeMclkRate );
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }
    return BERR_SUCCESS;
}

BAVC_AudioCompressionStd NEXUS_Audio_P_CodecToMagnum(NEXUS_AudioCodec codec)
{
    return NEXUS_P_AudioCodec_ToMagnum(codec);
}

NEXUS_AudioCodec NEXUS_Audio_P_MagnumToCodec(BAVC_AudioCompressionStd codec)
{
    return NEXUS_P_AudioCodec_FromMagnum(codec);
}

#if BAPE_DSP_SUPPORT
static BDSP_Algorithm NEXUS_Audio_P_PostProcessingToBdspAlgo(NEXUS_AudioPostProcessing pp)
{
    switch (pp)
    {
    case NEXUS_AudioPostProcessing_eSampleRateConverter:
        return BDSP_Algorithm_eSrc;
    case NEXUS_AudioPostProcessing_eCustomVoice:
        return BDSP_Algorithm_eCustomVoice;
    case NEXUS_AudioPostProcessing_eAutoVolumeLevel:
        return BDSP_Algorithm_eBrcmAvl;
    case NEXUS_AudioPostProcessing_eTrueSurround:
        return BDSP_Algorithm_eSrsTruSurroundHd;
    case NEXUS_AudioPostProcessing_eTruVolume:
        return BDSP_Algorithm_eSrsTruVolume;
    case NEXUS_AudioPostProcessing_eDsola:
        return BDSP_Algorithm_eDsola;
    case NEXUS_AudioPostProcessing_eBtsc:
        return BDSP_Algorithm_eBtscEncoder;
    case NEXUS_AudioPostProcessing_eFade:
        return BDSP_Algorithm_eFadeCtrl;
    case NEXUS_AudioPostProcessing_eKaraokeVocal:
        return BDSP_Algorithm_eVocalPP;
    default:
        return BDSP_Algorithm_eMax;
    }
}
#endif

static BAPE_PostProcessorType NEXUS_AudioModule_P_NexusProcessingTypeToPiProcessingType(NEXUS_AudioPostProcessing procType)
{
    switch ( procType )
    {
    case NEXUS_AudioPostProcessing_eAutoVolumeLevel:
        return BAPE_PostProcessorType_eAutoVolumeLevel;
    case NEXUS_AudioPostProcessing_eTrueSurround:
        return BAPE_PostProcessorType_eTruSurround;
    case NEXUS_AudioPostProcessing_eTruVolume:
        return BAPE_PostProcessorType_eTruVolume;
    case NEXUS_AudioPostProcessing_eFade:
        return BAPE_PostProcessorType_eFade;
    case NEXUS_AudioPostProcessing_eKaraokeVocal:
        return BAPE_PostProcessorType_eKaraokeVocal;
    default:
        break;
    }

    return BAPE_PostProcessorType_eMax;
}

NEXUS_Error NEXUS_AudioModule_Standby_priv(
    bool enabled,
    const NEXUS_StandbySettings *pSettings
    )
{
    BERR_Code rc = NEXUS_SUCCESS;
    NEXUS_AudioCapabilities audioCapabilities;

    BSTD_UNUSED(pSettings);

    NEXUS_GetAudioCapabilities(&audioCapabilities);

#if NEXUS_POWER_MANAGEMENT
    if(enabled) {
    unsigned i;
    for(i=0; i<audioCapabilities.numDecoders; i++) {
        NEXUS_AudioDecoderHandle handle = g_decoders[i];
        /* Force decoder to stop; if not already stopped */
        if(handle && handle->started) {
            BDBG_WRN(("Forcing Audio Decoder %d Stop for Standby", i));
            NEXUS_AudioDecoder_Stop(handle);
        }
    }

    for (i=0; i < audioCapabilities.numMixers; i++) {
        NEXUS_AudioMixerHandle mixerHandle = NEXUS_AudioMixer_P_GetMixerByIndex(i);
        if (mixerHandle && NEXUS_AudioMixer_P_IsStarted(mixerHandle))
        {
            BDBG_WRN(("Forcing Audio Mixer %d Stop for Standby", i));
            NEXUS_AudioMixer_Stop(mixerHandle);
        }
    }
    rc = BAPE_Standby(g_NEXUS_audioModuleData.apeHandle, NULL);
    if (rc) { rc = BERR_TRACE(rc); goto err; }

    #if BAPE_DSP_SUPPORT
    rc = BDSP_Standby(g_NEXUS_audioModuleData.dspHandle, NULL);
    if (rc) { rc = BERR_TRACE(rc); goto err; }

       /* disable region verification on RAAGA. */
       #if NEXUS_HAS_SECURITY
        if ( g_NEXUS_audioModuleData.verifyFirmware )
        {
            LOCK_SECURITY();
            NEXUS_Security_RegionVerifyDisable_priv( NEXUS_SecurityRegverRegionID_eRaaga0 );
            UNLOCK_SECURITY();

            if( g_NEXUS_audioModuleData.numDsps > 1 )
            {
                LOCK_SECURITY();
                NEXUS_Security_RegionVerifyDisable_priv( NEXUS_SecurityRegverRegionID_eRaaga1 );
                UNLOCK_SECURITY();
            }
        }
       #endif
    #endif

    } else {

        #if BAPE_DSP_SUPPORT
        rc = BDSP_Resume(g_NEXUS_audioModuleData.dspHandle);
        if (rc) { rc = BERR_TRACE(rc); goto err; }

        if ( g_NEXUS_audioModuleData.verifyFirmware )
        {
            rc = secureFirmwareAudio(g_NEXUS_audioModuleData.dspHandle);
            if (rc) { rc = BERR_TRACE(rc); goto err; }

            rc = BDSP_Raaga_Initialize(g_NEXUS_audioModuleData.dspHandle);
            if (rc) { rc = BERR_TRACE(rc); goto err; }
        }
        #endif

        rc = BAPE_Resume(g_NEXUS_audioModuleData.apeHandle);
        if (rc) { rc = BERR_TRACE(rc); goto err; }
    }

err :
#else
    BSTD_UNUSED(enabled);
#endif

    return rc;
}

void NEXUS_P_GetAudioCapabilities(NEXUS_AudioCapabilities *pCaps)
{
    BAPE_Capabilities apeCaps;
    unsigned i;
    BDBG_ASSERT(NULL != pCaps);

    BAPE_GetCapabilities(g_NEXUS_audioModuleData.apeHandle, &apeCaps);

    BKNI_Memset(pCaps, 0, sizeof(NEXUS_AudioCapabilities));

    BKNI_Memcpy(&g_NEXUS_audioModuleData.firmwareVersionInfo, &apeCaps.dsp.versionInfo, sizeof(apeCaps.dsp.versionInfo));

    #ifdef NEXUS_NUM_HDMI_INPUTS
    pCaps->numInputs.hdmi = NEXUS_NUM_HDMI_INPUTS < apeCaps.numInputs.mai ? NEXUS_NUM_HDMI_INPUTS : apeCaps.numInputs.mai;
    #endif
    #ifdef NEXUS_NUM_I2S_INPUTS
    pCaps->numInputs.i2s = NEXUS_NUM_I2S_INPUTS < apeCaps.numInputs.i2s ? NEXUS_NUM_I2S_INPUTS : apeCaps.numInputs.i2s;
    #endif
    #ifdef NEXUS_NUM_SPDIF_INPUTS
    pCaps->numInputs.spdif = NEXUS_NUM_SPDIF_INPUTS < apeCaps.numInputs.spdif ? NEXUS_NUM_SPDIF_INPUTS : apeCaps.numInputs.spdif;
    #endif
    #ifdef NEXUS_NUM_AUDIO_RETURN_CHANNEL
    pCaps->numOutputs.audioReturnChannel = NEXUS_NUM_AUDIO_RETURN_CHANNEL < apeCaps.numOutputs.audioReturnChannel ? NEXUS_NUM_AUDIO_RETURN_CHANNEL : apeCaps.numOutputs.audioReturnChannel;
    #endif
    #ifdef NEXUS_NUM_AUDIO_CAPTURES
    pCaps->numOutputs.capture = NEXUS_NUM_AUDIO_CAPTURES < apeCaps.numOutputs.capture ? NEXUS_NUM_AUDIO_CAPTURES : apeCaps.numOutputs.capture;
    #endif
    #ifdef NEXUS_NUM_AUDIO_DACS
    pCaps->numOutputs.dac = NEXUS_NUM_AUDIO_DACS < apeCaps.numOutputs.dac ? NEXUS_NUM_AUDIO_DACS : apeCaps.numOutputs.dac;
    #endif
    #ifdef NEXUS_NUM_AUDIO_DUMMY_OUTPUTS
    pCaps->numOutputs.dummy = NEXUS_NUM_AUDIO_DUMMY_OUTPUTS < apeCaps.numOutputs.dummy ? NEXUS_NUM_AUDIO_DUMMY_OUTPUTS : apeCaps.numOutputs.dummy;
    #endif
    #ifdef NEXUS_NUM_HDMI_OUTPUTS
    pCaps->numOutputs.hdmi = NEXUS_NUM_HDMI_OUTPUTS < apeCaps.numOutputs.mai ? NEXUS_NUM_HDMI_OUTPUTS : apeCaps.numOutputs.mai;
    #endif
    #ifdef NEXUS_NUM_I2S_OUTPUTS
    pCaps->numOutputs.i2s = NEXUS_NUM_I2S_OUTPUTS < apeCaps.numOutputs.i2s ? NEXUS_NUM_I2S_OUTPUTS : apeCaps.numOutputs.i2s;
    #endif
    pCaps->numOutputs.loopback = apeCaps.numOutputs.loopback;
    #ifdef NEXUS_NUM_RFM_OUTPUTS
    pCaps->numOutputs.rfmod = NEXUS_NUM_RFM_OUTPUTS < apeCaps.numOutputs.rfmod ? NEXUS_NUM_RFM_OUTPUTS : apeCaps.numOutputs.rfmod;
    #endif
    #ifdef NEXUS_NUM_SPDIF_OUTPUTS
    pCaps->numOutputs.spdif = NEXUS_NUM_SPDIF_OUTPUTS < apeCaps.numOutputs.spdif ? NEXUS_NUM_SPDIF_OUTPUTS : apeCaps.numOutputs.spdif;
    #endif

    #ifdef NEXUS_NUM_AUDIO_DECODERS
    pCaps->numDecoders = NEXUS_NUM_AUDIO_DECODERS < apeCaps.numDecoders ? NEXUS_NUM_AUDIO_DECODERS : apeCaps.numDecoders;
    #endif
    #ifdef NEXUS_NUM_AUDIO_PLAYBACKS
    pCaps->numPlaybacks = NEXUS_NUM_AUDIO_PLAYBACKS < apeCaps.numPlaybacks ? NEXUS_NUM_AUDIO_PLAYBACKS : apeCaps.numPlaybacks;
    #endif
    #ifdef NEXUS_NUM_AUDIO_INPUT_CAPTURES
    pCaps->numInputCaptures = NEXUS_NUM_AUDIO_INPUT_CAPTURES < apeCaps.numInputCaptures ? NEXUS_NUM_AUDIO_INPUT_CAPTURES : apeCaps.numInputCaptures;
    #endif
    #ifdef NEXUS_NUM_AUDIO_MIXERS
    pCaps->numMixers = NEXUS_NUM_AUDIO_MIXERS < apeCaps.numMixers ? NEXUS_NUM_AUDIO_MIXERS : apeCaps.numMixers;
    #endif
    pCaps->numVcxos = apeCaps.numVcxos;
    pCaps->numPlls = apeCaps.numPlls;
    pCaps->numNcos = apeCaps.numNcos;
    pCaps->numStcs = apeCaps.numStcs;
    pCaps->numCrcs = apeCaps.numCrcs;

    pCaps->numDsps = apeCaps.numDsps;
    for ( i = 0; i < NEXUS_AudioCodec_eMax; i++ )
    {
        BAVC_AudioCompressionStd codec = NEXUS_Audio_P_CodecToMagnum(i);
        if ( codec != BAVC_AudioCompressionStd_eMax )
        {
            pCaps->dsp.codecs[i].decode = apeCaps.dsp.codecs[codec].decode;
            pCaps->dsp.codecs[i].passthrough = apeCaps.dsp.codecs[codec].passthrough;
            pCaps->dsp.codecs[i].encode = apeCaps.dsp.codecs[codec].encode;
        }
    }
    pCaps->dsp.audysseyAbx = apeCaps.dsp.audysseyAbx;
    pCaps->dsp.audysseyAdv = apeCaps.dsp.audysseyAdv;
    pCaps->dsp.autoVolumeLevel = apeCaps.dsp.autoVolumeLevel;
    pCaps->dsp._3dSurround = apeCaps.dsp._3dSurround;
    pCaps->dsp.decodeRateControl = apeCaps.dsp.decodeRateControl;
    pCaps->dsp.dolbyDigitalReencode = apeCaps.dsp.dolbyDigitalReencode;
    pCaps->dsp.dolbyVolume258 = apeCaps.dsp.dolbyVolume;
    pCaps->dsp.echoCanceller.supported = apeCaps.dsp.echoCanceller.supported;
    pCaps->dsp.echoCanceller.algorithms[NEXUS_EchoCancellerAlgorithm_eSpeex] = apeCaps.dsp.echoCanceller.algorithms[BAPE_EchoCancellerAlgorithm_eSpeex];
    pCaps->dsp.encoder = apeCaps.dsp.encoder;
    pCaps->dsp.mixer = apeCaps.dsp.mixer;
    pCaps->dsp.muxOutput = apeCaps.dsp.muxOutput;
    pCaps->dsp.rfEncoder.supported = apeCaps.dsp.rfEncoder.supported;
    pCaps->dsp.rfEncoder.encodings[NEXUS_RfAudioEncoding_eBtsc] = apeCaps.dsp.rfEncoder.encodings[BAPE_RfAudioEncoding_eBtsc];
    pCaps->dsp.studioSound = apeCaps.dsp.studioSound;
    pCaps->dsp.truVolume = apeCaps.dsp.truVolume;
    pCaps->dsp.karaoke = apeCaps.dsp.karaoke;
    pCaps->equalizer.supported = apeCaps.equalizer.supported;

    for ( i = 0; i < NEXUS_AudioPostProcessing_eMax; i++ )
    {
        pCaps->dsp.processing[(NEXUS_AudioPostProcessing)i] = apeCaps.dsp.processing[NEXUS_AudioModule_P_NexusProcessingTypeToPiProcessingType((NEXUS_AudioPostProcessing)i)];
    }
}

void NEXUS_GetAudioCapabilities(NEXUS_AudioCapabilities *pCaps)
{
    BDBG_ASSERT(NULL != pCaps);
    BDBG_ASSERT(NULL != &g_NEXUS_audioModuleData.capabilities);
    BKNI_Memcpy(pCaps, &g_NEXUS_audioModuleData.capabilities, sizeof(NEXUS_AudioCapabilities));
}

void NEXUS_AudioOutputPll_GetSettings(
    NEXUS_AudioOutputPll pll,
    NEXUS_AudioOutputPllSettings *pSettings       /* [out] Current Settings */
    )
{
    BAPE_PllSettings apePllSettings;
    BAPE_Pll apePll;

    apePll = (BAPE_Pll)pll;
    BDBG_ASSERT(NULL != pSettings);

    BAPE_Pll_GetSettings(g_NEXUS_audioModuleData.apeHandle, apePll, &apePllSettings);
    pSettings->mode = apePllSettings.mode ? NEXUS_AudioOutputPllMode_eCustom : NEXUS_AudioOutputPllMode_eVcxo;
    pSettings->modeSettings.vcxo.vcxo = (NEXUS_Vcxo)apePllSettings.vcxo;
    pSettings->modeSettings.custom.value[0] = apePllSettings.nDivInt;
    pSettings->modeSettings.custom.value[1] = apePllSettings.mDivCh0;
}

NEXUS_Error NEXUS_AudioOutputPll_SetSettings(
    NEXUS_AudioOutputPll pll,
    const NEXUS_AudioOutputPllSettings *pSettings
    )
{
    BAPE_PllSettings apePllSettings;
    BERR_Code errCode;
    BAPE_Pll apePll;

    apePll = (BAPE_Pll)pll;
    BDBG_ASSERT(NULL != pSettings);

    BAPE_Pll_GetSettings(g_NEXUS_audioModuleData.apeHandle, apePll, &apePllSettings);
    apePllSettings.mode = pSettings->mode == NEXUS_AudioOutputPllMode_eCustom ? BAPE_PllMode_eCustom : BAPE_PllMode_eAuto;
    apePllSettings.vcxo = (unsigned)pSettings->modeSettings.vcxo.vcxo;
    apePllSettings.nDivInt = pSettings->modeSettings.custom.value[0];
    apePllSettings.mDivCh0 = pSettings->modeSettings.custom.value[1];
    errCode = BAPE_Pll_SetSettings(g_NEXUS_audioModuleData.apeHandle, apePll, &apePllSettings);
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }

    return BERR_SUCCESS;
}

void NEXUS_GetAudioFirmwareVersion(
    NEXUS_AudioFirmwareVersion *pVersion /* [out] */
    )
{
    #if BAPE_DSP_SUPPORT
    BDSP_Status dspStatus;

    BDBG_ASSERT(NULL != pVersion);

    BDSP_GetStatus(g_NEXUS_audioModuleData.dspHandle, &dspStatus);

    pVersion->major = dspStatus.firmwareVersion.majorVersion;
    pVersion->minor = dspStatus.firmwareVersion.minorVersion;
    pVersion->branch[0] = dspStatus.firmwareVersion.branchVersion;
    pVersion->branch[1] = dspStatus.firmwareVersion.branchSubVersion;
    #else
    BKNI_Memset(pVersion, 0, sizeof(NEXUS_AudioFirmwareVersion));
    #endif
}

const char * NEXUS_AudioCodecToString(
    NEXUS_AudioCodec codec
    )
{
    switch(codec)
    {
        case NEXUS_AudioCodec_eUnknown: return "Unknown";
        case NEXUS_AudioCodec_eMpeg: return "MPEG";
        case NEXUS_AudioCodec_eMp3: return "MP3";
        case NEXUS_AudioCodec_eAac: return "AAC ADTS";
        case NEXUS_AudioCodec_eAacLoas: return "AAC LOAS";
        case NEXUS_AudioCodec_eAacPlus: return "AAC+ LOAS";
        case NEXUS_AudioCodec_eAacPlusAdts: return "AAC+ ADTS";
        case NEXUS_AudioCodec_eAc3: return "AC3";
        case NEXUS_AudioCodec_eAc3Plus: return "AC3+";
        case NEXUS_AudioCodec_eDts: return "DTS";
        case NEXUS_AudioCodec_eLpcmDvd: return "LPCM DVD";
        case NEXUS_AudioCodec_eLpcmHdDvd: return "LPCM HD-DVD";
        case NEXUS_AudioCodec_eLpcmBluRay: return "LPCM BLU-RAY";
        case NEXUS_AudioCodec_eDtsHd: return "DTS-HD";
        case NEXUS_AudioCodec_eWmaStd: return "WMA STD";
        case NEXUS_AudioCodec_eWmaStdTs: return "WMA STD TS";
        case NEXUS_AudioCodec_eWmaPro: return "WMA PRO";
        case NEXUS_AudioCodec_eAvs: return "AVS";
        case NEXUS_AudioCodec_ePcm: return "PCM";
        case NEXUS_AudioCodec_ePcmWav: return "PCM WAV";
        case NEXUS_AudioCodec_eAmrNb: return "AMR-NB";
        case NEXUS_AudioCodec_eAmrWb: return "AMR-WB";
        case NEXUS_AudioCodec_eDra: return "DRA";
        case NEXUS_AudioCodec_eCook: return "COOK";
        case NEXUS_AudioCodec_eAdpcm: return "ADPCM";
        case NEXUS_AudioCodec_eSbc: return "SBC";
        case NEXUS_AudioCodec_eDtsCd: return "DTS-CD / LEGACY";
        case NEXUS_AudioCodec_eDtsExpress: return "DTS EXPRESS";
        case NEXUS_AudioCodec_eVorbis: return "VORBIS";
        case NEXUS_AudioCodec_eLpcm1394: return "LPCM 1394";
        case NEXUS_AudioCodec_eG711: return "G.711";
        case NEXUS_AudioCodec_eG723_1: return "G.732.1";
        case NEXUS_AudioCodec_eG726: return "G.726";
        case NEXUS_AudioCodec_eG729: return "G.729";
        case NEXUS_AudioCodec_eFlac: return "FLAC";
        case NEXUS_AudioCodec_eMlp: return "MLP";
        case NEXUS_AudioCodec_eApe: return "APE";
        case NEXUS_AudioCodec_eIlbc: return "ILBC";
        case NEXUS_AudioCodec_eIsac: return "ISAC";
        case NEXUS_AudioCodec_eOpus: return "Opus";
        case NEXUS_AudioCodec_eAls: return "MPEG-4 ALS ES";
        case NEXUS_AudioCodec_eAlsLoas: return "MPEG-4 ALS LOAS";
        case NEXUS_AudioCodec_eAc4: return "AC4";
        case NEXUS_AudioCodec_eMax: return "Invalid";
        default: return "Invalid";
    }
}

const char * NEXUS_AudioInputFormatToString(
    NEXUS_AudioInputFormat inputformat
    )
{
    switch(inputformat)
    {
        case NEXUS_AudioInputFormat_eNone: return "Auto";
        case NEXUS_AudioInputFormat_ePcmStereo: return "PCM Stereo";
        case NEXUS_AudioInputFormat_ePcm5_1: return "PCM 5.1";
        case NEXUS_AudioInputFormat_ePcm7_1: return "PCM 7.1";
        case NEXUS_AudioInputFormat_ePcmMono: return "PCM Mono";
        case NEXUS_AudioInputFormat_eCompressed: return "Compressed";
        case NEXUS_AudioInputFormat_eMax: return "Invalid";
        default: return "Invalid";
    }
}

/**
Summary:
Get Default Usage Settings
**/
void NEXUS_AudioModule_GetDefaultUsageSettings(
    NEXUS_AudioModuleUsageSettings *pSettings   /* [out] */
    )
{
    NEXUS_AudioModuleSettings amSettings;
    unsigned i;

    BDBG_ASSERT(NULL != pSettings);
    BKNI_Memset(pSettings, 0, sizeof(NEXUS_AudioModuleUsageSettings));

    NEXUS_AudioModule_GetDefaultSettings(NULL, &amSettings);

    pSettings->maxIndependentDelay = amSettings.maxIndependentDelay;
    pSettings->maxDecoderOutputChannels = (amSettings.numPcmBuffers - 1) * 2;
    pSettings->maxDecoderOutputSamplerate = amSettings.maxPcmSampleRate;

    pSettings->numDecoders = NEXUS_NUM_AUDIO_DECODERS;

    pSettings->dolbyCodecVersion = NEXUS_GetDolbyAudioCodecVersion();

    #if BAPE_DSP_SUPPORT
    /* Populate Defaults */
    for ( i=0; i<NEXUS_AudioCodec_eMax; i++ )
    {
        if ( BAPE_GetCodecAudioDecode(NEXUS_Audio_P_CodecToMagnum(i)) != BDSP_Algorithm_eMax )
        {
            pSettings->decodeCodecEnabled[i] = true;
        }
        if ( BAPE_GetCodecAudioEncode(NEXUS_Audio_P_CodecToMagnum(i)) != BDSP_Algorithm_eMax )
        {
            pSettings->encodeCodecEnabled[i] = true;
        }
    }
    #endif
}

typedef struct {
    BAPE_Settings apeSettings;
    BAPE_MemoryEstimate apeEstimate;
    #if BAPE_DSP_SUPPORT
    BDSP_RaagaSettings dspSettings;
    BDSP_RaagaUsageOptions dspUsage;
    BDSP_RaagaMemoryEstimate dspEstimate;
    #endif
    NEXUS_AudioModuleSettings audioModuleSettings;
    NEXUS_AudioModuleUsageSettings usageSettings;
} NEXUS_P_AudioMemoryEstimateData;
/**
Summary:
Get Memory Estimate

Description:
Get an estimated amount of memory required for specified usage
cases.
**/
NEXUS_Error NEXUS_AudioModule_GetMemoryEstimate(
    const struct NEXUS_Core_PreInitState *preInitState,
    const NEXUS_AudioModuleUsageSettings *pSettings,
    NEXUS_AudioModuleMemoryEstimate *pEstimate  /* [out] */
    )
{
    NEXUS_P_AudioMemoryEstimateData * pEstData = NULL;
    bool compressed4xAlgoEnabled = false;
    bool compressed16xAlgoEnabled = false;
    bool volumeLevelorEnabled = false;
    unsigned i;

    BDBG_ASSERT(NULL != pSettings);
    BDBG_ASSERT(NULL != pEstimate);

    pEstData = BKNI_Malloc(sizeof(NEXUS_P_AudioMemoryEstimateData));
    if ( pEstData == NULL )
    {
        BDBG_ERR(("Unable to allocate memory for NEXUS_P_AudioMemoryEstimateData"));
        return BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
    }

    BKNI_Memset(pEstimate, 0, sizeof(NEXUS_AudioModuleMemoryEstimate));
    BKNI_Memcpy(&(pEstData->usageSettings), pSettings, sizeof(NEXUS_AudioModuleUsageSettings));

    /* Get defaults */
    NEXUS_AudioModule_GetDefaultSettings(NULL, &(pEstData->audioModuleSettings));
    BAPE_GetDefaultSettings(&(pEstData->apeSettings));
    #if BAPE_DSP_SUPPORT
    BDSP_Raaga_GetDefaultSettings(&(pEstData->dspSettings));
    BKNI_Memset(&(pEstData->dspUsage), 0, sizeof(BDSP_RaagaUsageOptions));

    /* Enable Decoders */
    for ( i=0; i<NEXUS_AudioCodec_eMax; i++ )
    {
        if ( pEstData->usageSettings.decodeCodecEnabled[i] )
        {
            BDSP_Algorithm bdspAlgo = BAPE_GetCodecAudioDecode(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i));
            if (bdspAlgo != BDSP_Algorithm_eMax)
            {
                /* Adjust AAC codecs to override compile defines */
                if ( pEstData->usageSettings.dolbyCodecVersion == NEXUS_AudioDolbyCodecVersion_eMS12 )
                {
                    switch ( bdspAlgo )
                    {
                    case BDSP_Algorithm_eAacAdtsDecode:
                    case BDSP_Algorithm_eDolbyPulseAdtsDecode:
                        bdspAlgo = BDSP_Algorithm_eDolbyAacheAdtsDecode;
                        break;
                    case BDSP_Algorithm_eAacLoasDecode:
                    case BDSP_Algorithm_eDolbyPulseLoasDecode:
                        bdspAlgo = BDSP_Algorithm_eDolbyAacheLoasDecode;
                        break;
                    case BDSP_Algorithm_eAc3Decode:
                    case BDSP_Algorithm_eAc3PlusDecode:
                        bdspAlgo = BDSP_Algorithm_eUdcDecode;
                        break;
                    case BDSP_Algorithm_eAc3Passthrough:
                    case BDSP_Algorithm_eAc3PlusPassthrough:
                        bdspAlgo = BDSP_Algorithm_eUdcPassthrough;
                        break;
                    default:
                        break;
                    }
                }
                if ( pEstData->usageSettings.dolbyCodecVersion == NEXUS_AudioDolbyCodecVersion_eMS10 ||
                     pEstData->usageSettings.dolbyCodecVersion == NEXUS_AudioDolbyCodecVersion_eMS11 )
                {
                    switch ( bdspAlgo )
                    {
                    case BDSP_Algorithm_eAacAdtsDecode:
                    case BDSP_Algorithm_eDolbyAacheAdtsDecode:
                        bdspAlgo = BDSP_Algorithm_eDolbyPulseAdtsDecode;
                        break;
                    case BDSP_Algorithm_eAacLoasDecode:
                    case BDSP_Algorithm_eDolbyAacheLoasDecode:
                        bdspAlgo = BDSP_Algorithm_eDolbyPulseLoasDecode;
                        break;
                    case BDSP_Algorithm_eUdcDecode:
                        bdspAlgo = BDSP_Algorithm_eAc3PlusDecode;
                        break;
                    case BDSP_Algorithm_eUdcPassthrough:
                        bdspAlgo = BDSP_Algorithm_eAc3PlusPassthrough;
                        break;
                    default:
                        break;
                    }
                }
                else if ( pEstData->usageSettings.dolbyCodecVersion == NEXUS_AudioDolbyCodecVersion_eAc3 ||
                          pEstData->usageSettings.dolbyCodecVersion == NEXUS_AudioDolbyCodecVersion_eAc3Plus )
                {
                    switch ( bdspAlgo )
                    {
                    case BDSP_Algorithm_eDolbyPulseAdtsDecode:
                    case BDSP_Algorithm_eDolbyAacheAdtsDecode:
                        bdspAlgo = BDSP_Algorithm_eAacAdtsDecode;
                        break;
                    case BDSP_Algorithm_eDolbyPulseLoasDecode:
                    case BDSP_Algorithm_eDolbyAacheLoasDecode:
                        bdspAlgo = BDSP_Algorithm_eAacLoasDecode;
                        break;
                    case BDSP_Algorithm_eUdcDecode:
                        bdspAlgo = BDSP_Algorithm_eAc3PlusDecode;
                        break;
                    case BDSP_Algorithm_eUdcPassthrough:
                        bdspAlgo = BDSP_Algorithm_eAc3PlusPassthrough;
                        break;
                    default:
                        break;
                    }
                }
                pEstData->dspUsage.Codeclist[bdspAlgo] = true;
                BDBG_MODULE_MSG(nexus_audio_memest, ("Nexus Audio Decode Codec %d (BDSP Algo %d) enabled", i, bdspAlgo));

                /* Add associated Passthrough codec if applicable */
                bdspAlgo = BAPE_GetCodecAudioPassthrough(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i));
                if (bdspAlgo != BDSP_Algorithm_eMax)
                {
                    pEstData->dspUsage.Codeclist[bdspAlgo] = true;
                }
                if (BAPE_CodecRequiresSrc(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i)))
                {
                    pEstData->usageSettings.postProcessingEnabled[NEXUS_AudioPostProcessing_eSampleRateConverter] = true;
                }
                if ( BAPE_CodecSupportsCompressed4x(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i)) )
                {
                    BDBG_MODULE_MSG(nexus_audio_memest, ("BAVC Codec %d (Nexus Codec %d) supports 4x compressed", NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i), i));
                    compressed4xAlgoEnabled = true;
                }
                if ( BAPE_CodecSupportsCompressed16x(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i)) )
                {
                    BDBG_MODULE_MSG(nexus_audio_memest, ("BAVC Codec %d (Nexus Codec %d) supports 16x compressed", NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i), i));
                    compressed16xAlgoEnabled = true;
                }
            }
        }
    }

    /* Enable Encoders */
    for ( i=0; i<NEXUS_AudioCodec_eMax; i++ )
    {
        if ( pEstData->usageSettings.encodeCodecEnabled[i] )
        {
            BDSP_Algorithm bdspAlgo = BAPE_GetCodecAudioEncode(NEXUS_Audio_P_CodecToMagnum((NEXUS_AudioCodec)i));
            if ( bdspAlgo != BDSP_Algorithm_eMax )
            {
                pEstData->dspUsage.Codeclist[bdspAlgo] = true;
                BDBG_MODULE_MSG(nexus_audio_memest, ("Nexus Audio Encode Codec %d (BDSP Algo %d) enabled", i, bdspAlgo));
            }
        }
    }

    /* Make this calculation internally (below) to simplify application code */
    pEstData->usageSettings.numPostProcessing = 0;
    switch ( pEstData->usageSettings.dolbyCodecVersion )
    {
    case NEXUS_AudioDolbyCodecVersion_eMS12:
        /* tbd */
        break;
    case NEXUS_AudioDolbyCodecVersion_eMS11:
        pEstData->usageSettings.numPostProcessing += 2; /* DDRE + DV258 */
        volumeLevelorEnabled = true;
        break;
    default:
        break;

    }

    /* Enable Generic Post Processing */
    for ( i=0; i<NEXUS_AudioPostProcessing_eMax; i++ )
    {
        if ( pEstData->usageSettings.postProcessingEnabled[i] )
        {
            BDSP_Algorithm bdspAlgo = NEXUS_Audio_P_PostProcessingToBdspAlgo(i);
            if ( bdspAlgo != BDSP_Algorithm_eMax )
            {
                pEstData->dspUsage.Codeclist[bdspAlgo] = bdspAlgo;

                /* Add to PP list? */
                switch ( i )
                {
                case NEXUS_AudioPostProcessing_eAutoVolumeLevel:
                case NEXUS_AudioPostProcessing_eTruVolume:
                    if ( !volumeLevelorEnabled )
                    {
                        volumeLevelorEnabled = true;
                        pEstData->usageSettings.numPostProcessing++;
                    }
                    break;
                default:
                    pEstData->usageSettings.numPostProcessing++;
                    break;
                }
            }
        }
    }

    /* Enable Dolby Post Processing */
    switch ( pEstData->usageSettings.dolbyCodecVersion )
    {
    case NEXUS_AudioDolbyCodecVersion_eMS11:
        pEstData->dspUsage.Codeclist[BDSP_Algorithm_eDdre] = true;
        pEstData->dspUsage.Codeclist[BDSP_Algorithm_eDv258] = true;
        break;
    default:
    case NEXUS_AudioDolbyCodecVersion_eMS10:
    case NEXUS_AudioDolbyCodecVersion_eMS12:
    case NEXUS_AudioDolbyCodecVersion_eAc3Plus:
    case NEXUS_AudioDolbyCodecVersion_eAc3:
        break;
    }
    #endif

    /* Set up APE usage settings */
    pEstData->apeSettings.maxIndependentDelay = pEstData->usageSettings.maxIndependentDelay;
    pEstData->apeSettings.numPcmBuffers = (pEstData->usageSettings.maxDecoderOutputChannels / 2) + 1;
    pEstData->apeSettings.maxPcmSampleRate = pEstData->usageSettings.maxDecoderOutputSamplerate;
    pEstData->apeSettings.numCompressedBuffers = pEstData->usageSettings.numPassthroughDecoders;
    pEstData->apeSettings.numCompressed4xBuffers = pEstData->apeSettings.numCompressed16xBuffers = 0;
    if ( pEstData->usageSettings.numHbrPassthroughDecoders && compressed16xAlgoEnabled )
    {
        pEstData->apeSettings.numCompressed16xBuffers = pEstData->usageSettings.numHbrPassthroughDecoders;
    }
    if ( pEstData->usageSettings.numPassthroughDecoders && compressed4xAlgoEnabled && pEstData->apeSettings.numCompressed16xBuffers == 0 )
    {
        pEstData->apeSettings.numCompressed4xBuffers = pEstData->usageSettings.numPassthroughDecoders;
    }

    BDBG_MODULE_MSG(nexus_audio_memest, ("FMM USAGE: maxIndDelay %d ms, numPcmBuffers %d, maxPcmSR %d, numCompBuffers %d, numComp16xBuffers %d, numComp4xBuffers %d",
              pEstData->apeSettings.maxIndependentDelay,
              pEstData->apeSettings.numPcmBuffers,
              pEstData->apeSettings.maxPcmSampleRate,
              pEstData->apeSettings.numCompressedBuffers,
              pEstData->apeSettings.numCompressed16xBuffers,
              pEstData->apeSettings.numCompressed4xBuffers));

    BAPE_GetMemoryEstimate(&(pEstData->apeSettings), &(pEstData->apeEstimate));
    BDBG_MODULE_MSG(nexus_audio_memest, ("FMM USAGE: %d bytes", pEstData->apeEstimate.general));

    #if BAPE_DSP_SUPPORT
    #if BDBG_DEBUG_BUILD
    for ( i=0; i<BDSP_Algorithm_eMax; i++ )
    {
        if ( pEstData->dspUsage.Codeclist[i] )
        {
            BDBG_MODULE_MSG(nexus_audio_memest, ("DSP ALGO %d enabled", i));
        }
    }
    #endif

    pEstData->dspUsage.NumAudioDecoders = pEstData->usageSettings.numDecoders;
    pEstData->dspUsage.NumAudioEncoders = pEstData->usageSettings.numEncoders;
    pEstData->dspUsage.NumAudioPassthru = pEstData->usageSettings.numPassthroughDecoders;
    pEstData->dspUsage.NumAudioMixers = pEstData->usageSettings.numDspMixers;
    pEstData->dspUsage.NumAudioPostProcesses = pEstData->usageSettings.numPostProcessing;
    pEstData->dspUsage.NumAudioEchocancellers = pEstData->usageSettings.numEchoCancellers;
    pEstData->dspUsage.IntertaskBufferDataType = (pEstData->usageSettings.maxDecoderOutputChannels == 8) ? BDSP_DataType_ePcm7_1 : BDSP_DataType_ePcm5_1;
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eAc3 == (int)BDSP_AudioDolbyCodecVersion_eAC3);
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eAc3Plus == (int)BDSP_AudioDolbyCodecVersion_eDDP);
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eMS10 == (int)BDSP_AudioDolbyCodecVersion_eMS10);
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eMS11 == (int)BDSP_AudioDolbyCodecVersion_eMS11);
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eMS12 == (int)BDSP_AudioDolbyCodecVersion_eMS12);
    BDBG_CASSERT(NEXUS_AudioDolbyCodecVersion_eMax == (int)BDSP_AudioDolbyCodecVersion_eMax);
    pEstData->dspUsage.DolbyCodecVersion = (BDSP_AudioDolbyCodecVersion) pEstData->usageSettings.dolbyCodecVersion;

    BDBG_MODULE_MSG(nexus_audio_memest, ("DSP USAGE: numDecoders %d, numEncoders %d, numPassthrus %d, numMixers %d, numPostProcs %d, numEchoCancel %d, dolbyVer %d",
              pEstData->dspUsage.NumAudioDecoders,
              pEstData->dspUsage.NumAudioEncoders,
              pEstData->dspUsage.NumAudioPassthru,
              pEstData->dspUsage.NumAudioMixers,
              pEstData->dspUsage.NumAudioPostProcesses,
              pEstData->dspUsage.NumAudioEchocancellers,
              pEstData->dspUsage.DolbyCodecVersion));

    NEXUS_AudioModule_P_PopulateRaagaOpenSettings(&preInitState->boxConfig, &(pEstData->audioModuleSettings), &(pEstData->dspSettings));
    BDSP_Raaga_GetMemoryEstimate(&(pEstData->dspSettings), &(pEstData->dspUsage), (g_pCoreHandles!=NULL) ? g_pCoreHandles->box : NULL, &(pEstData->dspEstimate));
    BDBG_MODULE_MSG(nexus_audio_memest, ("DSP USAGE: firmware %d bytes, general %d bytes, total %d bytes", pEstData->dspEstimate.FirmwareMemory, pEstData->dspEstimate.GeneralMemory, pEstData->dspEstimate.GeneralMemory + pEstData->dspEstimate.FirmwareMemory));

    /* hardcode to mem controller 0 for now */
    pEstimate->memc[0].general = pEstData->apeEstimate.general + pEstData->dspEstimate.GeneralMemory + pEstData->dspEstimate.FirmwareMemory;
    #else
    pEstimate->memc[0].general = pEstData->apeEstimate.general;
    #endif

    if ( pEstData )
    {
        BKNI_Free(pEstData);
        pEstData = NULL;
    }

    return NEXUS_SUCCESS;
}

/**
Summary:
Get Dolby Codec Version

Description:
Get the Dolby Codec version currently enabled by the
intersection of compile time defines and DSP FW present.
**/
static NEXUS_AudioDolbyCodecVersion NEXUS_GetDolbyAudioCodecVersion(void)
{
    NEXUS_AudioDolbyCodecVersion dolbyCodecVersion = NEXUS_AudioDolbyCodecVersion_eAc3Plus;
    #if BAPE_DSP_SUPPORT
    switch ( BAPE_GetDolbyMSVersion() )
    {
        case BAPE_DolbyMSVersion_eMS12:
            dolbyCodecVersion = NEXUS_AudioDolbyCodecVersion_eMS12;
            break;
        case BAPE_DolbyMSVersion_eMS11:
            dolbyCodecVersion = NEXUS_AudioDolbyCodecVersion_eMS11;
            break;
        case BAPE_DolbyMSVersion_eMS10:
            dolbyCodecVersion = NEXUS_AudioDolbyCodecVersion_eMS10;
            break;
        default:
            break;
    }
    #else
    dolbyCodecVersion = NEXUS_AudioDolbyCodecVersion_eMax;
    #endif

    return dolbyCodecVersion;
}


static NEXUS_Error secureFirmwareAudio( BDSP_Handle hRagga )
{
#if BAPE_DSP_SUPPORT
    NEXUS_Error rc = NEXUS_SUCCESS;
    uint32_t firmwareSize;
    void * firmwareAddress;
    BDSP_Raaga_DownloadStatus downloadStatus;
  #if NEXUS_HAS_SECURITY
    NEXUS_SecurityRegionConfiguration regionConfig;
  #endif

    rc = BDSP_Raaga_GetDownloadStatus( hRagga, &downloadStatus );
    if( rc != BERR_SUCCESS )
    {
        return BERR_TRACE( rc );
    }

    firmwareAddress = downloadStatus.pBaseAddress;
    firmwareSize    = downloadStatus.length;
    NEXUS_FlushCache( (const void*)firmwareAddress, firmwareSize );

    /* Raaga 0 */
  #if NEXUS_HAS_SECURITY
    LOCK_SECURITY();
    NEXUS_Security_RegionGetDefaultConfig_priv( NEXUS_SecurityRegverRegionID_eRaaga0, &regionConfig );
    rc = NEXUS_Security_RegionConfig_priv( NEXUS_SecurityRegverRegionID_eRaaga0, &regionConfig );
    UNLOCK_SECURITY();
    if( rc ) { BERR_TRACE( rc );  /* Failed to configure raaga0 */ goto exit; }

    LOCK_SECURITY();
    rc = NEXUS_Security_RegionVerifyEnable_priv( NEXUS_SecurityRegverRegionID_eRaaga0,
                                                 NEXUS_AddrToOffset( firmwareAddress ),
                                                 firmwareSize );
    UNLOCK_SECURITY();
    if( rc ) { BERR_TRACE( rc );  /* Failed to verify raaga0 */ goto exit; }

    if (g_NEXUS_audioModuleData.numDsps > 1) {
        /* Raaga 1 */
        LOCK_SECURITY();
        NEXUS_Security_RegionGetDefaultConfig_priv( NEXUS_SecurityRegverRegionID_eRaaga1, &regionConfig );
        rc = NEXUS_Security_RegionConfig_priv( NEXUS_SecurityRegverRegionID_eRaaga1, &regionConfig );
        UNLOCK_SECURITY();
        if( rc ) { BERR_TRACE( rc );  /* Failed to configure raaga1 */ goto exit; }

        LOCK_SECURITY();
        rc = NEXUS_Security_RegionVerifyEnable_priv( NEXUS_SecurityRegverRegionID_eRaaga1,
                                                     NEXUS_AddrToOffset( firmwareAddress ),
                                                     firmwareSize );
        UNLOCK_SECURITY();
        if( rc ) { BERR_TRACE( rc ); /* Failed to verify raaga1 */  goto exit; }
    }
exit:
  #endif

    return rc;
#else /* NEXUS_AUDIO_DECODER_SUPPORT */
    BSTD_UNUSED(hRagga);
    return BERR_TRACE(NEXUS_NOT_SUPPORTED);
#endif /* NEXUS_AUDIO_DECODER_SUPPORT */
}

int32_t NEXUS_Audio_P_ConvertDbToLinear(int index)
{
    return g_db2linear[index];
}
