/***************************************************************************
*     (c)2004-2013 Broadcom Corporation
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
* API Description:
*   API name: Base
*
* Revision History:
*
* $brcm_Log: $
* 
***************************************************************************/
#ifndef NEXUS_AUDIO_TYPES_H
#define NEXUS_AUDIO_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************
Summary:
Audio codecs
***************************************************************************/
typedef enum NEXUS_AudioCodec 
{
    NEXUS_AudioCodec_eUnknown = 0,    /* unknown/not supported audio format */
    NEXUS_AudioCodec_eMpeg,           /* MPEG1/2, layer 1/2. This does not support layer 3 (mp3). */
    NEXUS_AudioCodec_eMp3,            /* MPEG1/2, layer 3. */
    NEXUS_AudioCodec_eAac,            /* Advanced audio coding with ADTS (Audio Data Transport Format) sync */
    NEXUS_AudioCodec_eAacAdts=NEXUS_AudioCodec_eAac,    /* Advanced audio coding with ADTS (Audio Data Transport Format) sync */
    NEXUS_AudioCodec_eAacLoas,        /* Advanced audio coding with LOAS (Low Overhead Audio Stream) sync and LATM mux */
    NEXUS_AudioCodec_eAacPlus,        /* AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) sync and LATM mux */
    NEXUS_AudioCodec_eAacPlusLoas =NEXUS_AudioCodec_eAacPlus,    /* AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) sync and LATM mux */
    NEXUS_AudioCodec_eAacPlusAdts,    /* AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format) sync */
    NEXUS_AudioCodec_eAc3,            /* Dolby Digital AC3 audio */
    NEXUS_AudioCodec_eAc3Plus,        /* Dolby Digital Plus (AC3+ or DDP) audio */
    NEXUS_AudioCodec_eDts,            /* DTS Core audio */
    NEXUS_AudioCodec_eLpcmDvd,        /* LPCM, DVD mode */
    NEXUS_AudioCodec_eLpcmHdDvd,      /* LPCM, HD-DVD mode */
    NEXUS_AudioCodec_eLpcmBluRay,     /* LPCM, Blu-Ray mode */
    NEXUS_AudioCodec_eDtsHd,          /* DTS-HD audio.  Supports both DTS-Core and DTS-HD streams.  */
    NEXUS_AudioCodec_eWmaStd,         /* WMA Standard */
    NEXUS_AudioCodec_eWmaStdTs,       /* WMA Standard with a 24-byte extended header */
    NEXUS_AudioCodec_eWmaPro,         /* WMA Professional */
    NEXUS_AudioCodec_eAvs,            /* AVS */ 
    NEXUS_AudioCodec_ePcm,            /* PCM audio - Generally used only with inputs such as SPDIF or HDMI. */ 
    NEXUS_AudioCodec_ePcmWav,         /* PCM audio with Wave header - Used with streams containing PCM audio */
    NEXUS_AudioCodec_eAmrNb,          /* Adaptive Multi-Rate compression (Narrow-Band, typically used w/3GPP) */
    NEXUS_AudioCodec_eAmr=NEXUS_AudioCodec_eAmrNb,
    NEXUS_AudioCodec_eAmrWb,          /* Adaptive Multi-Rate compression (Wide-Band) */
    NEXUS_AudioCodec_eDra,            /* Dynamic Resolution Adaptation.  Used in Blu-Ray and China Broadcasts. */
    NEXUS_AudioCodec_eCook,           /* Real Audio 8 LBR */
    NEXUS_AudioCodec_eAdpcm,          /* MS ADPCM audio format */
    NEXUS_AudioCodec_eSbc,            /* Sub Band Codec used in Bluetooth A2DP audio */
    NEXUS_AudioCodec_eDtsCd,          /* DTS-CD format (14-bit and 16-bit). */
    NEXUS_AudioCodec_eDtsLegacy=NEXUS_AudioCodec_eDtsCd,
    NEXUS_AudioCodec_eDtsExpress,     /* DTS-Express (DTS-LBR).  Low bit rate DTS format used in BluRay and streaming applications. */
    NEXUS_AudioCodec_eVorbis,         /* Vorbis audio codec.  Typically used with OGG or WebM container formats. */
    NEXUS_AudioCodec_eLpcm1394,       /* IEEE-1394 LPCM audio  */
    NEXUS_AudioCodec_eG711,           /* G.711 a-law and u-law companding.  Typically used for voice transmission. */
    NEXUS_AudioCodec_eG723_1,         /* G.723.1 Dual Rate Speech Coder for Multimedia Communications.  Used in H.324 and 3GPP 3G-324M.  This is different from G.723, which was superceded by G.726. */
    NEXUS_AudioCodec_eG726,           /* G.726 ADPCM speech codec.  Supercedes G.723 and G.721. */
    NEXUS_AudioCodec_eG729,           /* G.729 CS-ACELP speech codec.  Often used in VOIP applications. */
    NEXUS_AudioCodec_eFlac,           /* Free Lossless Audio Codec (see http://flac.sourceforge.net) */ 
    NEXUS_AudioCodec_eMlp,            /* Meridian Lossless Packing (Used in Dolby TrueHD) */
    NEXUS_AudioCodec_eApe,            /* Monkey's Audio (see http://www.monkeysaudio.com/) */
    NEXUS_AudioCodec_eIlbc,           /* Internet Low Bitrate Codec (see http://www.webrtc.org/ilbc-freeware)*/
    NEXUS_AudioCodec_eIsac,           /* Internet Speech Audio Codec */
    NEXUS_AudioCodec_eOpus,           /* Opus Audio Codec */
    NEXUS_AudioCodec_eAls,            /* MPEG-4 Audio Lossless Codec */
    NEXUS_AudioCodec_eAc4,            /* AC-4 Audio Codec */
    NEXUS_AudioCodec_eMax
} NEXUS_AudioCodec;

/***************************************************************************
Summary:
Audio Connector Format/Type
***************************************************************************/
typedef enum NEXUS_AudioConnectorType
{
    NEXUS_AudioConnectorType_eStereo,           /* Stereo (2.0) PCM output */
    NEXUS_AudioConnectorType_eMultichannel,     /* Multichannel (5.1/7.1) PCM output */
    NEXUS_AudioConnectorType_eCompressed,       /* Compressed IEC-61937 audio up to 48 kHz for SPDIF/HDMI applications.  */
    NEXUS_AudioConnectorType_eCompressed4x,     /* Compressed IEC-61937 audio for HDMI up to 192kHz.  Typically used for AC3+ and DTS-HD HRA audio formats. */
    NEXUS_AudioConnectorType_eCompressed16x,    /* Compressed IEC-61937 audio for HDMI HBR packets up to 768kHz.  Typically used for MAT/MLP (Dolby TrueHD) 
                                                   and DTS-HD MA audio formats.  Available in HDMI 1.3 and later. */
    NEXUS_AudioConnectorType_eMono,             /* Mono (1.0) PCM output.  This data type is not commonly used, and is generally only for voice conferencing applications. */
    NEXUS_AudioConnectorType_eMax
} NEXUS_AudioConnectorType;

/***************************************************************************
Summary:
Audio Multichannel Format
***************************************************************************/
typedef enum NEXUS_AudioMultichannelFormat
{
    NEXUS_AudioMultichannelFormat_eNone,
    NEXUS_AudioMultichannelFormat_eStereo = NEXUS_AudioMultichannelFormat_eNone,
    NEXUS_AudioMultichannelFormat_e5_1,
    NEXUS_AudioMultichannelFormat_e7_1,
    NEXUS_AudioMultichannelFormat_eMax
} NEXUS_AudioMultichannelFormat;

/***************************************************************************
Summary:
Audio Channels
***************************************************************************/
typedef enum NEXUS_AudioChannel
{
    NEXUS_AudioChannel_eLeft,           /* Left Front */
    NEXUS_AudioChannel_eRight,          /* Right Front */
    NEXUS_AudioChannel_eLeftSurround,   /* Left Surround */
    NEXUS_AudioChannel_eRightSurround,  /* Right Surround */
    NEXUS_AudioChannel_eCenter,         /* Center Channel */
    NEXUS_AudioChannel_eLfe,            /* Low Frequency Effects */
    NEXUS_AudioChannel_eLeftRear,       /* Left Rear (7.1 only) */
    NEXUS_AudioChannel_eRightRear,      /* Right Rear (7.1 only) */
    NEXUS_AudioChannel_eMax
} NEXUS_AudioChannel;

/***************************************************************************
Summary:
Audio Channel Pairs
***************************************************************************/
typedef enum NEXUS_AudioChannelPair
{
    NEXUS_AudioChannelPair_eLeftRight,          /* Front Left and Right */
    NEXUS_AudioChannelPair_eLeftRightSurround,  /* Surround Left and Right */
    NEXUS_AudioChannelPair_eCenterLfe,          /* Center Channel and LFE */
    NEXUS_AudioChannelPair_eLeftRightRear,      /* Rear Left and Right (7.1 only) */
    NEXUS_AudioChannelPair_eMax
} NEXUS_AudioChannelPair;

/***************************************************************************
Summary:
Audio Channel Mode
***************************************************************************/
typedef enum NEXUS_AudioVolumeType
{
    NEXUS_AudioVolumeType_eDecibel, /* Decibel scaling - Values are specified in 1/100 dB.  Currently supports attenuation only (negative values). */
    NEXUS_AudioVolumeType_eLinear,  /* Linear scaling - values are in a 4.23 format, 0x800000 = unity. */
    NEXUS_AudioVolumeType_eMax
} NEXUS_AudioVolumeType;

/***************************************************************************
Summary:
Audio Channel Mode
***************************************************************************/
typedef enum NEXUS_AudioChannelMode
{
    NEXUS_AudioChannelMode_eStereo,     /* Normal ordering, left-> left, right->right */
    NEXUS_AudioChannelMode_eLeft,       /* Duplicate left channel on left & right */
    NEXUS_AudioChannelMode_eRight,      /* Duplicate right channel on left & right */
    NEXUS_AudioChannelMode_eSwapped,    /* Reverse ordering, left->right, right->left */
    NEXUS_AudioChannelMode_eMax
} NEXUS_AudioChannelMode;

/***************************************************************************
Summary:
Audio Mono Channel Mode
***************************************************************************/
typedef enum NEXUS_AudioMonoChannelMode
{
    NEXUS_AudioMonoChannelMode_eLeft,
    NEXUS_AudioMonoChannelMode_eRight,
    NEXUS_AudioMonoChannelMode_eMix,
    NEXUS_AudioMonoChannelMode_eMax
} NEXUS_AudioMonoChannelMode;

/***************************************************************************
Summary:
    Audio  Mode
Description:
    Selects the audio channel mode 
***************************************************************************/
typedef enum NEXUS_AudioMode
{
    NEXUS_AudioMode_eAuto,     
    NEXUS_AudioMode_e1_0,
    NEXUS_AudioMode_e1_1,
    NEXUS_AudioMode_e2_0,
    NEXUS_AudioMode_e3_0,
    NEXUS_AudioMode_e2_1,
    NEXUS_AudioMode_e3_1,
    NEXUS_AudioMode_e2_2,
    NEXUS_AudioMode_e3_2,
    NEXUS_AudioMode_e3_4,
    NEXUS_AudioMode_eMax
} NEXUS_AudioMode;

/***************************************************************************
Summary:
Audio Volume Constants
***************************************************************************/
#define NEXUS_AUDIO_VOLUME_LINEAR_MAX 0x0fffffff
#define NEXUS_AUDIO_VOLUME_LINEAR_NORMAL 0x00800000
#define NEXUS_AUDIO_VOLUME_LINEAR_MIN 0

#define NEXUS_AUDIO_VOLUME_DB_MAX 0
#define NEXUS_AUDIO_VOLUME_DB_NORMAL 0
#define NEXUS_AUDIO_VOLUME_DB_MIN -9000

/***************************************************************************
Summary:
Audio Raw Channel Status
***************************************************************************/
typedef struct NEXUS_AudioRawChannelStatus
{
    uint32_t leftChannelHigh;   /* Bits 32..63 */
    uint32_t leftChannelLow;    /* Bits  0..31 */
    uint32_t rightChannelHigh;  /* Bits 32..63 */
    uint32_t rightChannelLow;   /* Bits  0..31 */
} NEXUS_AudioRawChannelStatus;

/**
Summary:
Audio channel status used in HDMI and SPDIF outputs
**/
typedef struct NEXUS_AudioChannelStatusInfo
{
    bool           professionalMode;    /* [0:0] The professional mode flag.
                                            TRUE: Professional mode. Other user
                                            options will not be considered.
                                            FALSE: Consumer mode.*/
    bool           swCopyRight;         /* [2:2] Software CopyRight assert.
                                            TRUE: CopyRight is asserted
                                            FALSE: CopyRight is not asserted */
    uint16_t       categoryCode;        /* [8:15] Category Code */
    uint16_t       clockAccuracy;       /* [28:29] Clock Accuracy */
    bool           separateLRChanNum;   /* TRUE:  Left channel num = 0000
                                                  Right Channel Num = 0000
                                           FALSE: Left channel num = 1000
                                                  Right Channel Num = 0100 */
    uint8_t        cgmsA;               /* CGMS-A copy bits.  Only supported for SPDIF.
                                           Typical values are
                                           0=Copy Freely 1=Copy Once
                                           2=Reserved 3=Copy Never */
} NEXUS_AudioChannelStatusInfo;

/**
Summary:
NEXUS_AudioInput is an abstract connector token for routing audio from a source.

Description:
See Also:
NEXUS_AudioDecoder_GetConnector
NEXUS_HdmiInput_GetAudioConnector
NEXUS_SpdifInput_GetConnector
**/ 
typedef struct NEXUS_AudioInputObject *NEXUS_AudioInput;

/**
Summary:
NEXUS_AudioOutput is an abstract connector token for routing audio to a source. 

Description:
See Also:
NEXUS_AudioDac_GetConnector
NEXUS_SpdifOutput_GetConnector
NEXUS_HdmiOutput_GetAudioConnector
NEXUS_RfOutput_GetConnector
**/
#ifdef __cplusplus
/* in C++  typeded struct A *A; is invalid, e.g. there is no separate namespace for structures */
typedef struct NEXUS_AudioOutputObject *NEXUS_AudioOutput;
#else
typedef struct NEXUS_AudioOutput *NEXUS_AudioOutput;
#endif
/**
Summary:
NEXUS_AudioOutputHandle is synonym for NEXUS_AudioOutput
**/
typedef NEXUS_AudioOutput NEXUS_AudioOutputHandle;

/***************************************************************************
Summary:
RF Audio Encodings
***************************************************************************/
typedef enum NEXUS_RfAudioEncoding
{
    NEXUS_RfAudioEncoding_eBtsc,
    NEXUS_RfAudioEncoding_eMax
} NEXUS_RfAudioEncoding;

/***************************************************************************
Summary:
Echo Cancellation Algorithms
***************************************************************************/
typedef enum NEXUS_EchoCancellerAlgorithm
{
    NEXUS_EchoCancellerAlgorithm_eSpeex,     /* Speex Algorithm (http://www.speex.org) */
    NEXUS_EchoCancellerAlgorithm_eMax
} NEXUS_EchoCancellerAlgorithm;

/***************************************************************************
Summary:
Audio PLL Selection 

Description: 
Many audio outputs require a PLL to generate their clock.  By default, 
all outputs will use the PLL0, unless 1) a different "defaultPll" has been 
specified in the NEXUS_AudioModuleSettings.defaultPll during the call to 
NEXUS_AudioModule_Init(), or 2) the PLL used by an output has been 
specifed in the NEXUS_AudioOutputSettings for that output.  You 
will need to use multiple PLLs if you have outputs that require different 
sampling rates from one another.  For example, if you have a decoder 
outputting 48kHz on I2S and another decoder outputting 44.1kHz on SPDIF, 
you must use separate PLLs for the two outputs.  If both use the same 
sampling rate, they can share the PLL. There are also cases where sampling 
rates that are multiples of one another can share a PLL (e.g. Dolby Digital 
Plus passthrough with simultaneous AC3 conversion). 
 
All outputs connected to the same source must share the same PLL.  You can 
not use multiple PLLs with a single source. 
 
Currently, DACs and RFM do not require a PLL.  I2S and SPDIF do 
require one. HDMI, DummyOutput, and OutputCapture outputs may use either 
a PLL or an NCO clock source.
***************************************************************************/
typedef enum
{
    NEXUS_AudioOutputPll_e0,
    NEXUS_AudioOutputPll_e1,
    NEXUS_AudioOutputPll_e2,
    NEXUS_AudioOutputPll_e3,
    NEXUS_AudioOutputPll_eMax
} NEXUS_AudioOutputPll;

/***************************************************************************
Summary:
Audio NCO Selection 
 
Description: 
Some audio outputs (that do not require a PLL) can use an NCO (Mclkgen)
oscillator to generate their clock.  By default, audio outputs will never 
use an NCO, but an NCO can be explicitly assigned to one or more outputs. 
As with PLLs, you will need to use multiple NCOs if you have outputs that 
require different sampling rates from one another.
 
If any output uses an NCO, then all other outputs connected to the same 
source must share the same NCO.  You can not use multiple NCOs (or mix NCOs 
and PLLs) with a single source. 
 
Currently, NCOs should only be used for HDMI, DummyOutput, and 
OutputCapture outputs.  NCO clocks are not adequate for I2S, and SPDIF 
outputs, which require a PLL clock source. DACs and RFM outputs will 
always use the DAC's internal rate manager as their clock source. 

Note: Not all chips have an NCO (Mclkgen) clock source available.
***************************************************************************/
typedef enum
{
    NEXUS_AudioOutputNco_e0,
    NEXUS_AudioOutputNco_e1,
    NEXUS_AudioOutputNco_e2,
    NEXUS_AudioOutputNco_e3,
    NEXUS_AudioOutputNco_eMax
} NEXUS_AudioOutputNco;


/***************************************************************************
Summary:
Burst Settings for SPDIF/HDMI

Description:
Options to configure Pause, Null or no bursts when HDMI/SPDIF interfaces 
underflow.  Bursts are designed to keep a downstream receiver engaged 
so that when audio resumes no content is lost.
***************************************************************************/
typedef enum NEXUS_SpdifOutputBurstType
{
    NEXUS_SpdifOutputBurstType_eNull,       /* Insert NULL burst during underflow */
    NEXUS_SpdifOutputBurstType_ePause,      /* Insert PAUSE burst during underflow */
    NEXUS_SpdifOutputBurstType_eNone,       /* Don't insert any burst during underflow */
    NEXUS_SpdifOutputBurstType_eMax
}NEXUS_SpdifOutputBurstType;


/***************************************************************************
Summary:
Loudness Equivalence Modes
**************************************************************************/
typedef enum NEXUS_AudioLoudnessEquivalenceMode
{
    NEXUS_AudioLoudnessEquivalenceMode_eNone,       /* Default, no loudness equivalence */
    NEXUS_AudioLoudnessEquivalenceMode_eAtscA85,    /* ATSC A/85.  This standardizes all decoders to output
                                                       Stereo PCM at -24dB and Multichannel at -31dB.  Compressed data is output at -31dB.
                                                       We will internally the volume for outputs accordingly so
                                                       that PCM stereo is output to "active" outputs such as DAC at -24 dB
                                                       and PCM is sent to "passive" outputs such as SPDIF at -31dB. */
    NEXUS_AudioLoudnessEquivalenceMode_eEbuR128,    /* EBU-R128.  This standardizes Dolby decoders to output
                                                       Stereo PCM at -23dB and Multichannel at -31dB.  Non-Dolby decoders will
                                                       output Stereo at -23dB.  All encoders and
                                                       passthrough configurations will output compressed at -31dB.
                                                       We will internally set the volume for outputs accordingly so
                                                       that PCM stereo is output to "active" outputs such as DAC at -23 dB
                                                       and PCM is sent to "passive" outputs such as SPDIF at -31dB. */
    NEXUS_AudioLoudnessEquivalenceMode_eMax
} NEXUS_AudioLoudnessEquivalenceMode;


/***************************************************************************
Summary:
Audio Dolby Codec Version
**************************************************************************/
typedef enum NEXUS_AudioDolbyCodecVersion
{
    NEXUS_AudioDolbyCodecVersion_eAc3,
    NEXUS_AudioDolbyCodecVersion_eAc3Plus,
    NEXUS_AudioDolbyCodecVersion_eMS10,
    NEXUS_AudioDolbyCodecVersion_eMS11,
    NEXUS_AudioDolbyCodecVersion_eMS12,
    NEXUS_AudioDolbyCodecVersion_eMax
} NEXUS_AudioDolbyCodecVersion;

/***************************************************************************
Summary:
Audio Post Processing
**************************************************************************/
typedef enum NEXUS_AudioPostProcessing
{
    NEXUS_AudioPostProcessing_eSampleRateConverter,
    NEXUS_AudioPostProcessing_eCustomVoice,
    NEXUS_AudioPostProcessing_eAutoVolumeLevel,
    NEXUS_AudioPostProcessing_eTrueSurround,
    NEXUS_AudioPostProcessing_eTruVolume,
    NEXUS_AudioPostProcessing_eDsola,
    NEXUS_AudioPostProcessing_eBtsc,
    NEXUS_AudioPostProcessing_eFade,
    NEXUS_AudioPostProcessing_eKaraokeVocal,
    NEXUS_AudioPostProcessing_eMax
} NEXUS_AudioPostProcessing;

#ifdef __cplusplus
}
#endif

#endif /* NEXUS_AUDIO_TYPES_H */

