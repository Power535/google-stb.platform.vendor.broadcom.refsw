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
#include "nexus_audio_output.h"
#include "nexus_spdif_output.h"
#include "nexus_component_output.h"
#if NEXUS_HAS_HDMI_OUTPUT
#include "nexus_hdmi_output.h"
#include "nexus_hdmi_output_hdcp.h"
#endif
#include "nexus_core_utils.h"
#define NEXUS_HAS_PLAYBACK 1

#if NEXUS_HAS_PLAYBACK
#define ENABLE_PLAYBACK     1
#if ENABLE_PLAYBACK
#include "nexus_playback.h"
#include "nexus_file.h"
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bstd.h"

#include "bkni.h"

BDBG_MODULE(audio_decode_low_latency) ;


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
#if ENABLE_PLAYBACK
/*static const char *fname = "videos/mummy_MI_5Element.mpg";*/
static const char *fname = "videos/bugs_toys2_jurassic_q64_cd.mpg";
#endif
#define TRANSPORT_TYPE NEXUS_TransportType_eTs
#define VIDEO_CODEC NEXUS_VideoCodec_eMpeg2
#define AUDIO_CODEC NEXUS_AudioCodec_eAc3
#define VIDEO_PID 0x11
#define AUDIO_PID 0x14

static void print_usage(void);

static NEXUS_PlatformConfiguration platformConfig;
static NEXUS_AudioDecoderHandle pcmDecoder, compressedDecoder;
static NEXUS_AudioDecoderStartSettings audioProgram;

int main(int argc, char **argv)
{
    NEXUS_VideoDecoderHandle videoDecoder;
    NEXUS_VideoDecoderStartSettings videoProgram;
    NEXUS_StcChannelHandle stcChannel;
    NEXUS_StcChannelSettings stcSettings;
    NEXUS_PidChannelHandle videoPidChannel, audioPidChannel;
    NEXUS_DisplayHandle display;
    NEXUS_VideoWindowHandle window;
    NEXUS_PlatformSettings platformSettings;
    #if ENABLE_PLAYBACK
    NEXUS_FilePlayHandle file;
    NEXUS_PlaypumpHandle playpump;
    NEXUS_PlaybackHandle playback;
    NEXUS_PlaybackSettings playbackSettings;
    NEXUS_PlaybackPidChannelSettings playbackPidSettings;
    #else
    NEXUS_ParserBand parserBand = NEXUS_ParserBand_e0;
    NEXUS_ParserBandSettings parserBandSettings;
    #endif
    NEXUS_DisplaySettings displaySettings;
    #if NEXUS_HAS_HDMI_OUTPUT
    NEXUS_HdmiOutputSettings hdmiSettings;
    NEXUS_HdmiOutputStatus hdmiStatus;
    hotplugCallbackParameters hotPlugCbParams;
    #endif
    bool done = false;
    bool compressed = false;
    unsigned audioPid = AUDIO_PID;
    unsigned videoPid = VIDEO_PID;
    NEXUS_AudioCodec audioCodec = AUDIO_CODEC;
    NEXUS_VideoCodec videoCodec = VIDEO_CODEC;
    NEXUS_TransportType transportType = TRANSPORT_TYPE;
    NEXUS_AudioDecoderLatencyMode latencyMode = NEXUS_AudioDecoderLatencyMode_eLow;
    NEXUS_AudioCapabilities audioCapabilities;
    char * fileName = (char*)fname;

    int i;

    for ( i = 1; i < argc; i++ )
    {
        if (!strcmp("--help", argv[i]) ||
            !strcmp("-h", argv[i]) ||
            !strcmp("-?", argv[i])) {
            print_usage();
            return 1;
        }
        else if (!strcmp("-audio_id", argv[i]) && (i+1) < argc) {
            audioPid = atoi(argv[++i]);
            printf("Audio Pid %d\n", audioPid);
        }
        else if (!strcmp("-video_id", argv[i]) && (i+1) < argc) {
            videoPid = atoi(argv[++i]);
            printf("Video Pid %d\n", videoPid);
        }
        else if (!strcmp("-audio_codec", argv[i]) && (i+1) < argc) {
            audioCodec = (NEXUS_AudioCodec)atoi(argv[++i]);
            printf("Audio Codec %d\n", audioCodec);
        }
        else if (!strcmp("-video_codec", argv[i]) && (i+1) < argc) {
            videoCodec = (NEXUS_VideoCodec)atoi(argv[++i]);
            printf("Video Codec %d\n", videoCodec);
        }
        else if (!strcmp("-transport", argv[i]) && (i+1) < argc) {
            transportType = (NEXUS_TransportType)atoi(argv[++i]);
            printf("Transport type %d\n", transportType);
        }
        else if (!strcmp(argv[i], "-file") && i+1 < argc)
        {
            #if NEXUS_HAS_PLAYBACK
            fileName = argv[++i];
            printf("Using file: %s\n", fileName);
            #endif
        }
        else if (!strcmp("-latency", argv[i]) && (i+1) < argc) {
            latencyMode = (NEXUS_AudioDecoderLatencyMode)atoi(argv[++i]);
            printf("Delay Mode %d\n", latencyMode);
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

    if (audioCapabilities.numDecoders == 0)
    {
        printf("This application is not supported on this platform (requires decoder).\n");
        return 0;
    }

    /* bring up decoders and connect to display */
    pcmDecoder = NEXUS_AudioDecoder_Open(0, NULL);
    compressedDecoder = NEXUS_AudioDecoder_Open(1, NULL);
    videoDecoder = NEXUS_VideoDecoder_Open(0, NULL); /* take default capabilities */
    NEXUS_Display_GetDefaultSettings(&displaySettings);
    display = NEXUS_Display_Open(0, &displaySettings);
    window = NEXUS_VideoWindow_Open(display, 0);
    NEXUS_VideoWindow_AddInput(window, NEXUS_VideoDecoder_GetConnector(videoDecoder));

#if ENABLE_PLAYBACK
    playpump = NEXUS_Playpump_Open(0, NULL);
    assert(playpump);
    playback = NEXUS_Playback_Create();
    assert(playback);

    file = NEXUS_FilePlay_OpenPosix(fileName, NULL);
    if (!file) {
        fprintf(stderr, "can't open file:%s\n", fileName);
        return -1;
    }
#else
    /* Map input band and parser band. This could be eliminated because a straight mapping and TS config is a good default. */
    NEXUS_ParserBand_GetSettings(parserBand, &parserBandSettings);
    parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
    NEXUS_Platform_GetStreamerInputBand(0, &parserBandSettings.sourceTypeSettings.inputBand);
    parserBandSettings.transportType = NEXUS_TransportType_eTs;
    NEXUS_ParserBand_SetSettings(parserBand, &parserBandSettings);
#endif

    /* By default, StcChannel will configure NEXUS_Timebase with the info it has */
    NEXUS_StcChannel_GetDefaultSettings(0, &stcSettings);

#if ENABLE_PLAYBACK
    /* configure stc channel */
    stcSettings.timebase = NEXUS_Timebase_e0;
    stcSettings.mode = NEXUS_StcChannelMode_eAuto;
    stcChannel = NEXUS_StcChannel_Open(0, &stcSettings);

    /* connect playpump and playback */
    NEXUS_Playback_GetSettings(playback, &playbackSettings);
    playbackSettings.stcChannel = stcChannel;
    playbackSettings.playpump = playpump;
    playbackSettings.playpumpSettings.transportType = TRANSPORT_TYPE;
    NEXUS_Playback_SetSettings(playback, &playbackSettings);

    /* Open the audio and video pid channels */
    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eVideo;
    playbackPidSettings.pidTypeSettings.video.codec = VIDEO_CODEC; /* must be told codec for correct handling */
    playbackPidSettings.pidTypeSettings.video.index = true;
    playbackPidSettings.pidTypeSettings.video.decoder = videoDecoder;
    videoPidChannel = NEXUS_Playback_OpenPidChannel(playback, VIDEO_PID, &playbackPidSettings);
    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
    playbackPidSettings.pidTypeSettings.audio.primary = pcmDecoder;
    audioPidChannel = NEXUS_Playback_OpenPidChannel(playback, AUDIO_PID, &playbackPidSettings);
    printf("audioPidChannel %p, videoPidChannel %p\n", (void*)audioPidChannel, (void*)videoPidChannel);

#else
    /* Open the audio and video pid channels */
    videoPidChannel = NEXUS_PidChannel_Open(parserBand, VIDEO_PID, NULL);
    audioPidChannel = NEXUS_PidChannel_Open(parserBand, AUDIO_PID, NULL);

    /* configure stc channel */
    stcSettings.timebase = NEXUS_Timebase_e0;
    stcSettings.mode = NEXUS_StcChannelMode_ePcr; /* live */
    stcSettings.modeSettings.pcr.pidChannel = videoPidChannel; /* PCR on video pid */
    stcChannel = NEXUS_StcChannel_Open(0, &stcSettings);
#endif

    printf("audioPidChannel %p, videoPidChannel %p\n", (void*)audioPidChannel, (void*)videoPidChannel);
    NEXUS_VideoDecoder_GetDefaultStartSettings(&videoProgram);
    videoProgram.codec = VIDEO_CODEC;
    videoProgram.pidChannel = videoPidChannel;
    videoProgram.stcChannel = stcChannel;
    NEXUS_AudioDecoder_GetDefaultStartSettings(&audioProgram);

    /********************* START: Audio Low latency specific settings *********************************************************/

    /*
    NEXUS_AudioDecoderLatencyMode_eAuto,         Auto delay. Auto indicates that for playback cases "Normal" delay
                                                   will be configured. However, if data is coming from an input, reduced
                                                   delay will be used.
    NEXUS_AudioDecoderLatencyMode_eNormal,       Normal/Default Fixed delay. In this mode, the decoder will use the
                                                   standard fixed path delay mode, which is compatible across all codecs.
    NEXUS_AudioDecoderLatencyMode_eLow,          Low Fixed delay. This is a reduced fixed delay mode. This mode
                                                   will apply to a select group of codecs that can support a reduced
                                                   delay mode. Codecs that cannot support this mode will fall back to
                                                   eNormal mode. No Post Processing, Passthrough, or Transcode supported
                                                   in this mode.
    NEXUS_AudioDecoderLatencyMode_eLowest,       Lowest Variable delay. This is a lowest possible, variable delay that
                                                   varies by codec. In this mode, the decoder will operate with a minimal
                                                   output delay based on the input format.  This can be useful for very
                                                   low delay applications such as receiving from HDMI input, game mode
                                                   processing, or voice conferencing.
    */

    audioProgram.latencyMode = latencyMode;

    /* This setting will ensure low latency by starting the decode when just-enough data is available */

    audioProgram.targetSyncEnabled = false;

    /*********************  END: Audio Low latency specific settings *************************************************************/

    audioProgram.codec = AUDIO_CODEC;
    audioProgram.pidChannel = audioPidChannel;
    audioProgram.stcChannel = stcChannel;

    /* connect audio outputs */
    /* Output PCM to DAC, SPDIF, and HDMI by default */
    #if NEXUS_NUM_AUDIO_DACS
    NEXUS_AudioOutput_AddInput(NEXUS_AudioDac_GetConnector(platformConfig.outputs.audioDacs[0]),
                               NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioDecoderConnectorType_eStereo));
    #endif
    #if NEXUS_NUM_SPDIF_OUTPUTS
    NEXUS_AudioOutput_AddInput(NEXUS_SpdifOutput_GetConnector(platformConfig.outputs.spdif[0]),
                               NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioDecoderConnectorType_eStereo));
    #endif
    #if NEXUS_HAS_HDMI_OUTPUT
    NEXUS_AudioOutput_AddInput(NEXUS_HdmiOutput_GetAudioConnector(platformConfig.outputs.hdmi[0]),
                               NEXUS_AudioDecoder_GetConnector(pcmDecoder, NEXUS_AudioDecoderConnectorType_eMultichannel));

    /* bring up display */
    NEXUS_HdmiOutput_GetStatus(platformConfig.outputs.hdmi[0], &hdmiStatus);
    #endif

#if NEXUS_NUM_COMPONENT_OUTPUTS
    if(platformConfig.outputs.component[0]){
        NEXUS_Display_AddOutput(display, NEXUS_ComponentOutput_GetConnector(platformConfig.outputs.component[0]));
    }
#endif
#if 0 /* NEXUS_NUM_COMPOSITE_OUTPUTS */
    NEXUS_Display_AddOutput(display, NEXUS_CompositeOutput_GetConnector(platformConfig.outputs.composite[0]));
#endif
    NEXUS_Display_AddOutput(display, NEXUS_HdmiOutput_GetVideoConnector(platformConfig.outputs.hdmi[0]));

    #if NEXUS_HAS_HDMI_OUTPUT
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

    /* Start Decoders */
    NEXUS_VideoDecoder_Start(videoDecoder, &videoProgram);
    NEXUS_AudioDecoder_Start(pcmDecoder, &audioProgram);

    NEXUS_HdmiOutput_GetStatus(platformConfig.outputs.hdmi[0], &hdmiStatus);
    NEXUS_Display_GetSettings(display, &displaySettings);
    displaySettings.format = hdmiStatus.preferredVideoFormat;
    NEXUS_Display_SetSettings(display, &displaySettings);

#if ENABLE_PLAYBACK
    /* Start playback */
    NEXUS_Playback_Start(playback, file, NULL);
#endif

    while (!done)
    {
        bool changeDelay = false;
        int tmp;

        /* Display Menu */
        printf("Main Menu\n");
        printf(" 0) Exit\n");
        printf(" 1) %sNormal Delay%s\n",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eNormal ? "[":"",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eNormal ? "]":""
               );
        printf(" 2) %sLow Delay%s\n",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eLow ? "[":"",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eLow ? "]":""
               );
        printf(" 3) %sLowest Delay%s\n",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eLowest ? "[":"",
               audioProgram.latencyMode == NEXUS_AudioDecoderLatencyMode_eLowest ? "]":""
               );
        printf("Enter Selection: \n");
        scanf("%d", &tmp);
        switch ( tmp )
        {
        case 0:
            done = true;
            break;
        case 1:
            audioProgram.latencyMode = NEXUS_AudioDecoderLatencyMode_eNormal;
            changeDelay = true;
            break;
        case 2:
            audioProgram.latencyMode = NEXUS_AudioDecoderLatencyMode_eLow;
            changeDelay = true;
            break;
        case 3:
            audioProgram.latencyMode = NEXUS_AudioDecoderLatencyMode_eLowest;
            changeDelay = true;
            break;
        default:
            break;
        }

        if ( changeDelay )
        {
            if ( compressed )
            {
                NEXUS_AudioDecoder_Stop(compressedDecoder);
            }
            NEXUS_AudioDecoder_Stop(pcmDecoder);

            NEXUS_AudioDecoder_Start(pcmDecoder, &audioProgram);
            if ( compressed )
            {
                NEXUS_AudioDecoder_Start(compressedDecoder, &audioProgram);
            }
        }
    }

#if ENABLE_PLAYBACK
    NEXUS_Playback_Stop(playback);
    NEXUS_Playback_CloseAllPidChannels(playback);
    NEXUS_FilePlay_Close(file);
    NEXUS_Playback_Destroy(playback);
    NEXUS_Playpump_Close(playpump);
#endif

    if ( compressed )
    {
        NEXUS_AudioDecoder_Stop(compressedDecoder);
    }
    NEXUS_AudioDecoder_Stop(pcmDecoder);
    NEXUS_VideoDecoder_Stop(videoDecoder);


    NEXUS_VideoDecoder_Close(videoDecoder);
    NEXUS_VideoWindow_Close(window);

    #if NEXUS_HAS_HDMI_OUTPUT
    /* stop/remove HDMI callbacks associated with display,
    so those callbacks do not access display once it is removed */
    NEXUS_StopCallbacks(platformConfig.outputs.hdmi[0]);
    #endif
    NEXUS_Display_Close(display);
    NEXUS_AudioDecoder_Close(pcmDecoder);
    NEXUS_AudioDecoder_Close(compressedDecoder);
    #if !ENABLE_PLAYBACK
    NEXUS_PidChannel_Close(videoPidChannel);
    NEXUS_PidChannel_Close(audioPidChannel);
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
    printf("audio_decode_low_latency"
           " [-file <file1>]"
           " [-audio_id <pid>]"
           " [-video_id <pid>]"
           " [-audio_codec <unsigned>]"
           " [-video_codec <unsigned>]"
           " [-transport <unsigned>]"
           " [-delay <1,2,3> (Normal, Low, Lowest)]"
           "\n");
}

#else /* NEXUS_HAS_AUDIO */
int main(void)
{
    printf("This application is not supported on this platform (needs audio)!\n");
    return 0;
}

#endif
