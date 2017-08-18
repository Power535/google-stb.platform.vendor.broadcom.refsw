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
 *
 * Module Description:
 *
******************************************************************************/
/* Nexus example app: single live a/v decode from an input band, routed to hdmi output */

#include "nexus_platform.h"
#include <stdio.h>

#if NEXUS_HAS_AUDIO
#include "nexus_pid_channel.h"
#include "nexus_parser_band.h"
#include "nexus_video_decoder.h"
#include "nexus_stc_channel.h"
#include "nexus_display.h"
#include "nexus_video_window.h"
#include "nexus_audio_dac.h"
#include "nexus_audio_decoder.h"
#include "nexus_audio_playback.h"
#include "nexus_i2s_input.h"
#include "nexus_audio_mixer.h"
#include "nexus_audio_processor.h"
#include "nexus_audio_output.h"
#include "nexus_spdif_output.h"
#include "nexus_component_output.h"
#if NEXUS_HAS_HDMI_OUTPUT
#include "nexus_hdmi_output.h"
#include "nexus_hdmi_output_hdcp.h"
#endif
#include "nexus_core_utils.h"
#include "nexus_playback.h"
#include "nexus_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bstd.h"

#include "bkni.h"

BDBG_MODULE(audio_karaoke);

/* General application defines */
#define ENABLE_I2S_INPUT        0
#define DSP_MIXER               0

#if NEXUS_HAS_HDMI_OUTPUT
/*****************/
/* For HDCP TESTING  */
/*    1) insert the Production Key Set set generated by BCrypt  */
/*    2) set the USE_PRODUCTION_KEYS macro to to 1 */
/*****************/
#define USE_PRODUCTION_KEYS     0

typedef struct hotplugCallbackParameters
{
    NEXUS_HdmiOutputHandle hdmiOutput;
    NEXUS_DisplayHandle display;
} hotplugCallbackParameters;

static bool hdmiHdcpEnabled = false ;
static void initializeHdmiOutputHdcpSettings(void);
static void hdmiOutputHdcpStateChanged(void *pContext, int param);
static void hotplug_callback(void *pParam, int iParam);
#endif

/* the following define the input file and its characteristics -- these will vary by input file */
static char *musicFname = "videos/Fox&Cats_WriteItOff_44.1k.mp3";
#define TRANSPORT_TYPE0 NEXUS_TransportType_eEs
#define AUDIO_CODEC0 NEXUS_AudioCodec_eMp3
#define AUDIO_PID0 0x1

#if !ENABLE_I2S_INPUT
static char *vocalFname = "videos/44.1k_2000Hz_beep_1s_30s.wav";
#define TRANSPORT_TYPE1 NEXUS_TransportType_eWav
#define AUDIO_CODEC1 NEXUS_AudioCodec_ePcmWav
#define AUDIO_PID1 0x1
#endif

static void print_usage(void);
static NEXUS_PlatformConfiguration platformConfig;

/* ./nexus audio_karaoke [file name][suppression level][frequency suppression][output boost] */
int main(int argc, char **argv)
{
    NEXUS_PlatformSettings platformSettings;
    NEXUS_FilePlayHandle musicFile = NULL;
    static NEXUS_AudioDecoderHandle pcmDecoder;
    static NEXUS_AudioDecoderStartSettings audioProgram;
    NEXUS_StcChannelHandle stcChannel;
    NEXUS_StcChannelSettings stcSettings;
    NEXUS_PidChannelHandle audioPidChannel;
    NEXUS_DisplayHandle display;
    NEXUS_PlaypumpHandle playpump;
    NEXUS_PlaybackHandle playback;
    NEXUS_PlaybackSettings playbackSettings;
    NEXUS_PlaybackPidChannelSettings playbackPidSettings;
    NEXUS_DisplaySettings displaySettings;
    #if NEXUS_HAS_HDMI_OUTPUT
    NEXUS_HdmiOutputSettings hdmiSettings;
    NEXUS_HdmiOutputStatus hdmiStatus;
    hotplugCallbackParameters hotPlugCbParams;
    #endif
    NEXUS_AudioDecoderOpenSettings decoderOpenSettings;
    NEXUS_AudioDecoderSettings decoderSettings;
    NEXUS_AudioDecoderHandle inputDecoder;
    NEXUS_AudioDecoderStartSettings decoderStartSettings;
    #if ENABLE_I2S_INPUT
    NEXUS_I2sInputHandle i2s;
    NEXUS_I2sInputSettings i2sSettings;
    #else
    NEXUS_FilePlayHandle vocalFile = NULL;
    NEXUS_PlaypumpHandle vocalPlaypump;
    NEXUS_PlaybackHandle vocalPlayback;
    NEXUS_StcChannelHandle vocalStcChannel;
    NEXUS_PidChannelHandle vocalAudioPidChannel;
    #endif
    NEXUS_AudioProcessorHandle karaokeVocal;
    NEXUS_AudioProcessorSettings karaokeVocalSettings;
    NEXUS_AudioProcessorOpenSettings karaokeVocalOpenSettings;
    NEXUS_AudioMixerHandle mixer;
    NEXUS_AudioMixerSettings mixerSettings;
    unsigned suppressionLevel = 90;
    unsigned suppressionFreq = 4688;
    unsigned outputBoost = 3;
    bool echoEnabled = true;
    unsigned echoLevel = 20;
    unsigned echoInterval = 100;
    NEXUS_AudioCapabilities audioCapabilities;
    NEXUS_AudioOutputHandle audioDacHandle = NULL;
    NEXUS_AudioOutputHandle audioSpdifHandle = NULL;
    NEXUS_AudioOutputHandle audioHdmiHandle = NULL;
    int i;

    for ( i = 1; i < argc; i++ )
    {
        if (!strcmp("--help", argv[i]) ||
            !strcmp("-h", argv[i]) ||
            !strcmp("-?", argv[i])) {
            print_usage();
            return 1;
        }
        #if 0
        else if ( !strcmp("-music_file", argv[i]) && (i+1) < argc ) {
            musicFname = argv[++i];
        }
        else if ( !strcmp("-vocal_file", argv[i]) && (i+1) < argc ) {
            vocalFname = argv[++i];
        }
        #endif
        else if ( !strcmp("-music_vocal_suppress", argv[i]) && (i+1) < argc ) {
            suppressionLevel = atoi(argv[++i]);
        }
        else if ( !strcmp("-music_vocal_suppress_freq", argv[i]) && (i+1) < argc ) {
            suppressionFreq = atoi(argv[++i]);
        }
        else if ( !strcmp("-music_boost", argv[i]) && (i+1) < argc ) {
            outputBoost = atoi(argv[++i]);
        }
        else if ( !strcmp("-echo", argv[i]) && (i+1) < argc ) {
            echoEnabled = atoi(argv[++i]) == 1 ? true : false;
        }
        else if ( !strcmp("-echo_level", argv[i]) && (i+1) < argc ) {
            echoLevel = atoi(argv[++i]);
        }
        else if ( !strcmp("-echo_interval", argv[i]) && (i+1) < argc ) {
            echoInterval = atoi(argv[++i]);
        }
        else {
            printf("Unknown param: %s\n", argv[i]);
            return -1;
        }
    }

    /* Bring up all modules for a platform in a default configuration for this platform */
    NEXUS_Platform_GetDefaultSettings(&platformSettings);
    platformSettings.openFrontend = false;
    NEXUS_Platform_Init(&platformSettings);
    NEXUS_Platform_GetConfiguration(&platformConfig);
    NEXUS_GetAudioCapabilities(&audioCapabilities);

    if (audioCapabilities.numDecoders < 2 ||
    #if ENABLE_I2S_INPUT
        audioCapabilities.numInputs.i2s == 0 ||
    #endif
        audioCapabilities.numMixers == 0)
    {
        printf("This application is not supported on this platform (requires decoder and mixers).\n");
        return 0;
    }

    if (audioCapabilities.numOutputs.dac > 0)
    {
        audioDacHandle = NEXUS_AudioDac_GetConnector(platformConfig.outputs.audioDacs[0]);
    }

    if (audioCapabilities.numOutputs.spdif > 0)
    {
        audioSpdifHandle = NEXUS_SpdifOutput_GetConnector(platformConfig.outputs.spdif[0]);
    }

    #if NEXUS_NUM_HDMI_OUTPUTS
    if (audioCapabilities.numOutputs.hdmi > 0)
    {
        audioHdmiHandle = NEXUS_HdmiOutput_GetAudioConnector(platformConfig.outputs.hdmi[0]);
    }
    #endif


    /* Create our input sources */
    NEXUS_AudioDecoder_GetDefaultOpenSettings(&decoderOpenSettings);
    decoderOpenSettings.karaokeSupported = true;
    pcmDecoder = NEXUS_AudioDecoder_Open(0, &decoderOpenSettings);
    assert(pcmDecoder);
    #if ENABLE_I2S_INPUT
    NEXUS_I2sInput_GetDefaultSettings(&i2sSettings);
    /* customize i2s settings here to match ADC */
    i2sSettings.sampleRate = 44100;
    i2s = NEXUS_I2sInput_Open(0, &i2sSettings);
    inputDecoder = NEXUS_AudioDecoder_Open(1, NULL);
    #else
    inputDecoder = NEXUS_AudioDecoder_Open(1, &decoderOpenSettings);
    #endif

    /* Create Karaoke Vocal PP */
    NEXUS_AudioProcessor_GetDefaultOpenSettings(&karaokeVocalOpenSettings);
    karaokeVocalOpenSettings.type = NEXUS_AudioPostProcessing_eKaraokeVocal;
    karaokeVocal = NEXUS_AudioProcessor_Open(&karaokeVocalOpenSettings);
    assert(karaokeVocal);
    NEXUS_AudioProcessor_GetSettings(karaokeVocal, &karaokeVocalSettings);
    karaokeVocalSettings.settings.karaokeVocal.echo.enabled = echoEnabled;
    karaokeVocalSettings.settings.karaokeVocal.echo.attenuation = echoLevel;
    karaokeVocalSettings.settings.karaokeVocal.echo.delay = echoInterval;
    NEXUS_AudioProcessor_SetSettings(karaokeVocal, &karaokeVocalSettings);

    /* Create DSP and FMM Mixer */
    NEXUS_AudioMixer_GetDefaultSettings(&mixerSettings);
    mixerSettings.outputSampleRate = 44100;
#if DSP_MIXER
    mixerSettings.mixUsingDsp = true;
#endif
    mixer = NEXUS_AudioMixer_Open(&mixerSettings);
    assert(mixer);

    /* Create Video / Playback resources */
    NEXUS_Display_GetDefaultSettings(&displaySettings);
    display = NEXUS_Display_Open(0, &displaySettings);
    assert(display);

    playpump = NEXUS_Playpump_Open(0, NULL);
    assert(playpump);
    playback = NEXUS_Playback_Create();
    assert(playback);
#if !ENABLE_I2S_INPUT
    vocalPlaypump = NEXUS_Playpump_Open(1, NULL);
    assert(playpump);
    vocalPlayback = NEXUS_Playback_Create();
    assert(playback);

#endif
    musicFile = NEXUS_FilePlay_OpenPosix(musicFname, NULL);
    if (!musicFile) {
        fprintf(stderr, "can't open music file:%s\n", musicFname);
        return -1;
    }

    #if !ENABLE_I2S_INPUT

    vocalFile = NEXUS_FilePlay_OpenPosix(vocalFname, NULL);
    if (!musicFile) {
        fprintf(stderr, "can't open music file:%s\n", vocalFname);
        return -1;
    }
    #endif

    NEXUS_AudioDecoder_GetSettings(pcmDecoder,&decoderSettings);
    BDBG_ERR(("Music Karaoke Processing Settings:"));
    BDBG_ERR(("  suppressionLevel %d suppressionFreq %d outputBoost %d",suppressionLevel,suppressionFreq,outputBoost));
    decoderSettings.karaokeSettings.vocalSuppressionLevel = suppressionLevel;
    decoderSettings.karaokeSettings.vocalSuppressionFrequency = suppressionFreq;
    decoderSettings.karaokeSettings.outputMakeupBoost = outputBoost;
    NEXUS_AudioDecoder_SetSettings(pcmDecoder,&decoderSettings);

    /* Make Filter Graph Connections */

    /* Connect input vocal source to Karaoke Vocal PP */
    NEXUS_AudioProcessor_AddInput(karaokeVocal,
                                  NEXUS_AudioDecoder_GetConnector(inputDecoder, NEXUS_AudioConnectorType_eStereo));

    /* Connect inputs to FMM mixer */
    NEXUS_AudioMixer_AddInput(mixer,
                              NEXUS_AudioProcessor_GetConnectorByType(karaokeVocal, NEXUS_AudioConnectorType_eStereo));
    NEXUS_AudioMixer_AddInput(mixer,
                              NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioConnectorType_eStereo));

    NEXUS_AudioMixer_GetSettings(mixer, &mixerSettings);
    mixerSettings.master = NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioConnectorType_eStereo);
    NEXUS_AudioMixer_SetSettings(mixer, &mixerSettings);

    /* connect audio outputs */
    /* Output PCM to DAC, SPDIF, and HDMI by default */
    if (audioDacHandle) {
        NEXUS_AudioOutput_AddInput(audioDacHandle,
                                   NEXUS_AudioMixer_GetConnector(mixer));
    }
    if (audioSpdifHandle) {
        NEXUS_AudioOutput_AddInput(audioSpdifHandle,
                                   NEXUS_AudioMixer_GetConnector(mixer));
    }
    #if NEXUS_NUM_HDMI_OUTPUTS
    if (audioHdmiHandle) {
        NEXUS_AudioOutput_AddInput(audioHdmiHandle,
                                   NEXUS_AudioMixer_GetConnector(mixer));
    }
    #endif

    /* Set the Timebase Master */
    NEXUS_AudioMixer_GetSettings(mixer, &mixerSettings);
    mixerSettings.master = NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioConnectorType_eStereo);
    NEXUS_AudioMixer_SetSettings(mixer, &mixerSettings);

    /* By default, StcChannel will configure NEXUS_Timebase with the info it has */
    NEXUS_StcChannel_GetDefaultSettings(0, &stcSettings);

    /* configure stc channel */
    stcSettings.timebase = NEXUS_Timebase_e0;
    stcSettings.mode = NEXUS_StcChannelMode_eAuto;
    stcChannel = NEXUS_StcChannel_Open(0, &stcSettings);

    /* connect playpump and playback */
    NEXUS_Playback_GetSettings(playback, &playbackSettings);
    playbackSettings.stcChannel = stcChannel;
    playbackSettings.playpump = playpump;
    playbackSettings.playpumpSettings.transportType = TRANSPORT_TYPE0;
    NEXUS_Playback_SetSettings(playback, &playbackSettings);

    #if !ENABLE_I2S_INPUT
        /* configure stc channel */
    stcSettings.timebase = NEXUS_Timebase_e1;
    stcSettings.mode = NEXUS_StcChannelMode_eAuto;
    vocalStcChannel = NEXUS_StcChannel_Open(1, &stcSettings);

    /* connect playpump and playback */
    NEXUS_Playback_GetSettings(vocalPlayback, &playbackSettings);
    playbackSettings.stcChannel = vocalStcChannel;
    playbackSettings.playpump = vocalPlaypump;
    playbackSettings.playpumpSettings.transportType = TRANSPORT_TYPE1;
    NEXUS_Playback_SetSettings(vocalPlayback, &playbackSettings);

    #endif

    /* Open the audio and video pid channels */
    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
    playbackPidSettings.pidTypeSettings.audio.primary = pcmDecoder;
    audioPidChannel = NEXUS_Playback_OpenPidChannel(playback, AUDIO_PID0, &playbackPidSettings);
    printf("audioPidChannel %p\n", (void*)audioPidChannel);

    NEXUS_AudioDecoder_GetDefaultStartSettings(&audioProgram);
    audioProgram.codec = AUDIO_CODEC0;
    audioProgram.pidChannel = audioPidChannel;
    audioProgram.stcChannel = stcChannel;
    audioProgram.karaokeModeEnabled = true;

    #if NEXUS_NUM_COMPONENT_OUTPUTS
    if(platformConfig.outputs.component[0]){
        NEXUS_Display_AddOutput(display, NEXUS_ComponentOutput_GetConnector(platformConfig.outputs.component[0]));
    }
    #endif

    #if NEXUS_HAS_HDMI_OUTPUT
    /* bring up display */
    NEXUS_HdmiOutput_GetStatus(platformConfig.outputs.hdmi[0], &hdmiStatus);
    NEXUS_Display_AddOutput(display, NEXUS_HdmiOutput_GetVideoConnector(platformConfig.outputs.hdmi[0]));
    /* Install hotplug callback -- video only for now */
    NEXUS_HdmiOutput_GetSettings(platformConfig.outputs.hdmi[0], &hdmiSettings);
    hdmiSettings.hotplugCallback.callback = hotplug_callback;
    hotPlugCbParams.hdmiOutput = platformConfig.outputs.hdmi[0];
    hotPlugCbParams.display = display;
    hdmiSettings.hotplugCallback.context = &hotPlugCbParams;
    NEXUS_HdmiOutput_SetSettings(platformConfig.outputs.hdmi[0], &hdmiSettings);

    /* initialize HDCP settings, keys, etc. */
    initializeHdmiOutputHdcpSettings() ;

    /* Force a hotplug to switch to preferred format */
    hotplug_callback(&hotPlugCbParams, 0);
    #endif

    /* Start Decoder */
    NEXUS_AudioDecoder_Start(pcmDecoder, &audioProgram);
    /* Start playback */
    NEXUS_Playback_Start(playback, musicFile, NULL);

    #if ENABLE_I2S_INPUT
    NEXUS_AudioDecoder_GetDefaultStartSettings(&decoderStartSettings);
    decoderStartSettings.codec = NEXUS_AudioCodec_ePcm;
    decoderStartSettings.input = NEXUS_I2sInput_GetConnector(i2s);
    NEXUS_AudioDecoder_Start(inputDecoder, &decoderStartSettings);
    #else

    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
    playbackPidSettings.pidTypeSettings.audio.primary = inputDecoder;
    vocalAudioPidChannel = NEXUS_Playback_OpenPidChannel(vocalPlayback, AUDIO_PID1, &playbackPidSettings);


    NEXUS_Playback_Start(vocalPlayback, vocalFile, NULL);
    NEXUS_AudioDecoder_GetDefaultStartSettings(&decoderStartSettings);
    decoderStartSettings.codec = AUDIO_CODEC1;
    decoderStartSettings.pidChannel = vocalAudioPidChannel;
    decoderStartSettings.stcChannel = vocalStcChannel;
    decoderStartSettings.karaokeModeEnabled = true;
    NEXUS_AudioDecoder_Start(inputDecoder, &decoderStartSettings);
    #endif

    #if NEXUS_HAS_HDMI_OUTPUT
    NEXUS_HdmiOutput_GetStatus(platformConfig.outputs.hdmi[0], &hdmiStatus);
    NEXUS_Display_GetSettings(display, &displaySettings);
    displaySettings.format = hdmiStatus.preferredVideoFormat;
    NEXUS_Display_SetSettings(display, &displaySettings);
    #endif

    {
        bool done = false;
        while (!done)
        {
            int tmp;

            /* Display Menu */
            printf("Main Menu\n");
            printf(" 0) Exit\n");
            printf("Enter Selection: \n");
            scanf("%d", &tmp);
            switch ( tmp )
            {
            case 0:
                done = true;
                break;
            default:
                break;
            }
        }
    }

    NEXUS_Playback_Stop(playback);
    #if !ENABLE_I2S_INPUT
    NEXUS_Playback_Stop(vocalPlayback);
    #endif
    NEXUS_AudioDecoder_Stop(inputDecoder);
    NEXUS_AudioDecoder_Stop(pcmDecoder);
    #if NEXUS_HAS_HDMI_OUTPUT
    /* stop/remove HDMI callbacks associated with display,
    so those callbacks do not access display once it is removed */
    NEXUS_StopCallbacks(platformConfig.outputs.hdmi[0]);
    #endif
    NEXUS_AudioProcessor_RemoveAllInputs(karaokeVocal);
    NEXUS_AudioMixer_RemoveAllInputs(mixer);
    NEXUS_AudioProcessor_Close(karaokeVocal);
    NEXUS_AudioMixer_Close(mixer);
    NEXUS_Display_Close(display);
    NEXUS_AudioDecoder_Close(pcmDecoder);
    NEXUS_AudioDecoder_Close(inputDecoder);
    #if ENABLE_I2S_INPUT
    NEXUS_I2sInput_Close(i2s);
    #else
    NEXUS_Playback_Destroy(vocalPlayback);
    NEXUS_Playpump_Close(vocalPlaypump);
    #endif
    NEXUS_Playback_Destroy(playback);
    NEXUS_Playpump_Close(playpump);
    NEXUS_FilePlay_Close(musicFile);
    #if !ENABLE_I2S_INPUT
    NEXUS_FilePlay_Close(vocalFile);
    #endif
    NEXUS_Platform_Uninit();

    return 0;
}


#if NEXUS_HAS_HDMI_OUTPUT

#if USE_PRODUCTION_KEYS

/*****************************/
/* INSERT PRODUCTION KeySet HERE */
/*****************************/

#else


/**************************************/
/* HDCP Specification Test Key Set    */
/*                                    */
/* NOTE: the default declared Test    */
/* KeySet below is from the HDCP Spec */
/* and it *IS NOT* compatible with    */
/* production devices                 */
/**************************************/


static NEXUS_HdmiOutputHdcpKsv hdcpTxAksv =
{    {0x14, 0xF7, 0x61, 0x03, 0xB7} };

static NEXUS_HdmiOutputHdcpKey encryptedTxKeySet[NEXUS_HDMI_OUTPUT_HDCP_NUM_KEYS] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x691e138f, 0x58a44d00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x0950e658, 0x35821f00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x0d98b9ab, 0x476a8a00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xcac5cb52, 0x1b18f300},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xb4d89668, 0x7f14fb00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x818f4878, 0xc98be000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x412c11c8, 0x64d0a000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x44202428, 0x5a9db300},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x6b56adbd, 0xb228b900},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xf6e46c4a, 0x7ba49100},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x589d5e20, 0xf8005600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xa03fee06, 0xb77f8c00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x28bc7c9d, 0x8c2dc000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x059f4be5, 0x61125600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xcbc1ca8c, 0xdef07400},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x6adbfc0e, 0xf6b83b00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xd72fb216, 0xbb2ba000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x98547846, 0x8e2f4800},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x38472762, 0x25ae6600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xf2dd23a3, 0x52493d00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x543a7b76, 0x31d2e200},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x2561e6ed, 0x1a584d00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xf7227bbf, 0x82603200},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x6bce3035, 0x461bf600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x6b97d7f0, 0x09043600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xf9498d61, 0x05e1a100},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x063405d1, 0x9d8ec900},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x90614294, 0x67c32000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xc34facce, 0x51449600},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x8a8ce104, 0x45903e00},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xfc2d9c57, 0x10002900},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x80b1e569, 0x3b94d700},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x437bdd5b, 0xeac75400},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xba90c787, 0x58fb7400},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xe01d4e36, 0xfa5c9300},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xae119a15, 0x5e070300},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x01fb788a, 0x40d30500},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xb34da0d7, 0xa5590000},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x409e2c4a, 0x633b3700},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0x412056b4, 0xbb732500}
} ;

 #endif

/*
from HDCP Spec:
Table 51 gives the format of the HDCP SRM. All values are stored in big endian format.

Specify KSVs here in big endian;
*/
#define NUM_REVOKED_KSVS 3
static uint8_t NumRevokedKsvs = NUM_REVOKED_KSVS ;
static const NEXUS_HdmiOutputHdcpKsv RevokedKsvs[NUM_REVOKED_KSVS] =
{
    /* MSB ... LSB */
    {{0xa5, 0x1f, 0xb0, 0xc3, 0x72}},
    {{0x65, 0xbf, 0x04, 0x8a, 0x7c}},
    {{0x65, 0x65, 0x1e, 0xd5, 0x64}}
} ;

static void hotplug_callback(void *pParam, int iParam)
{
    NEXUS_HdmiOutputStatus status;
    NEXUS_HdmiOutputHandle hdmi;
    NEXUS_DisplayHandle display;
    NEXUS_DisplaySettings displaySettings;
    NEXUS_HdmiOutputSettings hdmiSettings;
    hotplugCallbackParameters *hotPlugCbParams ;

    hotPlugCbParams = (hotplugCallbackParameters *) pParam ;
    hdmi = hotPlugCbParams->hdmiOutput ;
    display = hotPlugCbParams->display ;

    NEXUS_HdmiOutput_GetStatus(hdmi, &status);
    /* the app can choose to switch to the preferred format, but it's not required. */
    if ( !status.connected )
    {
        BDBG_WRN(("No RxDevice Connected")) ;
        return ;
    }

    NEXUS_Display_GetSettings(display, &displaySettings);
    if ( !status.videoFormatSupported[displaySettings.format] )
    {
        BDBG_ERR(("Current format not supported by attached monitor. Switching to preferred format %d",
            status.preferredVideoFormat)) ;
        displaySettings.format = status.preferredVideoFormat;
    }
    NEXUS_Display_SetSettings(display, &displaySettings);

    /* force HDMI updates after a hotplug */
    NEXUS_HdmiOutput_GetSettings(hdmi, &hdmiSettings) ;
    NEXUS_HdmiOutput_SetSettings(hdmi, &hdmiSettings) ;

    /* restart HDCP if it was previously enabled */
    if (hdmiHdcpEnabled)
    {
        NEXUS_HdmiOutput_StartHdcpAuthentication(platformConfig.outputs.hdmi[0]);
    }
}

static void hdmiOutputHdcpStateChanged(void *pContext, int param)
{

    bool success = false ;
    NEXUS_HdmiOutputHandle handle = pContext;
    NEXUS_HdmiOutputHdcpStatus hdcpStatus;

    BSTD_UNUSED(param) ;

    NEXUS_HdmiOutput_GetHdcpStatus(handle, &hdcpStatus);
    switch (hdcpStatus.hdcpError)
    {
    case NEXUS_HdmiOutputHdcpError_eSuccess :
        BDBG_WRN(("HDCP Authentication Successful\n"));
        success = true ;
        hdmiHdcpEnabled = true ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRxBksvError :
        BDBG_ERR(("HDCP Rx BKsv Error")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRxBksvRevoked :
        BDBG_ERR(("HDCP Rx BKsv/Keyset Revoked")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRxBksvI2cReadError :
    case NEXUS_HdmiOutputHdcpError_eTxAksvI2cWriteError :
        BDBG_ERR(("HDCP I2C Read Error")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eTxAksvError :
        BDBG_ERR(("HDCP Tx Aksv Error")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eReceiverAuthenticationError :
        BDBG_ERR(("HDCP Receiver Authentication Failure")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRepeaterAuthenticationError :
    case NEXUS_HdmiOutputHdcpError_eRepeaterLinkFailure :    /* Repeater Error; unused */
        BDBG_ERR(("HDCP Repeater Authentication Failure")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRxDevicesExceeded :
        BDBG_ERR(("HDCP Repeater MAX Downstram Devices Exceeded")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRepeaterDepthExceeded :
        BDBG_ERR(("HDCP Repeater MAX Downstram Levels Exceeded")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRepeaterFifoNotReady :
        BDBG_ERR(("Timeout waiting for Repeater")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eRepeaterDeviceCount0 : /* unused */
        break ;

    case NEXUS_HdmiOutputHdcpError_eLinkRiFailure :
        BDBG_ERR(("HDCP Ri Integrity Check Failure")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eLinkPjFailure :
        BDBG_ERR(("HDCP Pj Integrity Check Failure")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eFifoUnderflow :
    case NEXUS_HdmiOutputHdcpError_eFifoOverflow :
        BDBG_ERR(("Video configuration issue")) ;
        break ;

    case NEXUS_HdmiOutputHdcpError_eMultipleAnRequest : /* Should not reach here; but flag if occurs */
        BDBG_WRN(("Multiple Authentication Request... ")) ;

    default :
        BDBG_WRN(("Unknown HDCP Authentication Error %d", hdcpStatus.hdcpError)) ;
    }

    if (!success)
    {
        fprintf(stderr, "\nHDCP Authentication Failed.  Current State %d\n", hdcpStatus.hdcpState);

        /* always retry */
        NEXUS_HdmiOutput_StartHdcpAuthentication(platformConfig.outputs.hdmi[0]);
    }
}

static void initializeHdmiOutputHdcpSettings(void)
{
    NEXUS_HdmiOutputHdcpSettings hdmiOutputHdcpSettings;

    NEXUS_HdmiOutput_GetHdcpSettings(platformConfig.outputs.hdmi[0], &hdmiOutputHdcpSettings);

        /* copy the encrypted key set and its Aksv here  */
        BKNI_Memcpy(hdmiOutputHdcpSettings.encryptedKeySet, encryptedTxKeySet,
            NEXUS_HDMI_OUTPUT_HDCP_NUM_KEYS * sizeof(NEXUS_HdmiOutputHdcpKey)) ;
        BKNI_Memcpy(&hdmiOutputHdcpSettings.aksv, &hdcpTxAksv,
            NEXUS_HDMI_OUTPUT_HDCP_KSV_LENGTH) ;

        /* install HDCP success  callback */
        hdmiOutputHdcpSettings.successCallback.callback = hdmiOutputHdcpStateChanged ;
        hdmiOutputHdcpSettings.successCallback.context = platformConfig.outputs.hdmi[0];

        /* install HDCP failure callback */
        hdmiOutputHdcpSettings.failureCallback.callback = hdmiOutputHdcpStateChanged ;
        hdmiOutputHdcpSettings.failureCallback.context = platformConfig.outputs.hdmi[0];

    NEXUS_HdmiOutput_SetHdcpSettings(platformConfig.outputs.hdmi[0], &hdmiOutputHdcpSettings);

    /* install list of revoked KSVs from SRMs (System Renewability Message) if available */
    NEXUS_HdmiOutput_SetHdcpRevokedKsvs(platformConfig.outputs.hdmi[0],
        RevokedKsvs, NumRevokedKsvs) ;

}
#endif

void print_usage(void)
{
    printf("Usage:\n");
    printf("audio_karaoke"
           " [-music_file <file1>]"
           " [-vocal_file <file2>]"
           " [-music_vocal_suppress <amount>]"
           " [-music_vocal_suppress_freq <freq>]"
           " [-music_boost <amount>]"
           " [-echo <enabled>]"
           " [-echo_level <level>]"
           " [-echo_interval <interval>"
           "\n");
}

#else /* NEXUS_HAS_AUDIO */
int main(void)
{
    printf("This application is not supported on this platform (needs audio)!\n");
    return 0;
}

#endif
