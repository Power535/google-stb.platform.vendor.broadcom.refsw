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
#include "nexus_platform_client.h"
#include "nxclient.h"
#include "nexus_core_utils.h"
#include "platform.h"
#include "platform_priv.h"
#include "platform_scheduler_priv.h"
#include "bdbg.h"
#include "bkni.h"
#include <string.h>
#include <sys/time.h>

BDBG_MODULE(platform);

PlatformHandle platform_open(const char * appName)
{
    PlatformHandle platform;
    PlatformSchedulerCreateSettings schedulerSettings;
    int rc = 0;
    NxClient_JoinSettings joinSettings;
    unsigned i;

    NxClient_GetDefaultJoinSettings(&joinSettings);
    BKNI_Snprintf(joinSettings.name, NXCLIENT_MAX_NAME, "%s", appName);
    rc = NxClient_Join(&joinSettings);
    if (rc) goto out_join_failure;

    platform = BKNI_Malloc(sizeof(*platform));
    BDBG_ASSERT(platform);
    BKNI_Memset(platform, 0, sizeof(*platform));

    platform->schedulers[PLATFORM_SCHEDULER_MAIN] = platform_scheduler_p_create(platform, NULL);
    BDBG_ASSERT(platform->schedulers[PLATFORM_SCHEDULER_MAIN] );

    platform_scheduler_p_get_default_create_settings(&schedulerSettings);
    schedulerSettings.index = PLATFORM_SCHEDULER_GFX;
    schedulerSettings.period = 10;
    platform->schedulers[PLATFORM_SCHEDULER_GFX] = platform_scheduler_p_create(platform, &schedulerSettings);
    BDBG_ASSERT(platform->schedulers[PLATFORM_SCHEDULER_GFX]);

    platform_scheduler_p_get_default_create_settings(&schedulerSettings);
    schedulerSettings.index = PLATFORM_SCHEDULER_USAGE;
    schedulerSettings.period = 2000;
    platform->schedulers[PLATFORM_SCHEDULER_USAGE] = platform_scheduler_p_create(platform, &schedulerSettings);
    BDBG_ASSERT(platform->schedulers[PLATFORM_SCHEDULER_USAGE]);

    NxClient_GetDefaultCallbackThreadSettings(&platform->callbackThreadSettings);
    platform->callbackThreadSettings.hdmiOutputHotplug.callback = platform_p_hotplug_handler;
    platform->callbackThreadSettings.hdmiOutputHotplug.context = platform;
    platform->callbackThreadSettings.hdmiOutputHotplug.param = 0;
    rc = NxClient_StartCallbackThread(&platform->callbackThreadSettings);
    BDBG_ASSERT(!rc);

    NEXUS_GetVideoDecoderCapabilities(&platform->media.videoCaps);
    NEXUS_GetDisplayCapabilities(&platform->display.caps);

    for (i=0; i<NEXUS_MAX_VIDEO_DECODERS; i++)
    {
        if (platform->media.videoCaps.memory[i].used)
        {
            if (platform->media.videoCaps.memory[i].mosaic.maxNumber)
            {
                platform->media.video[i].maxStreams = platform->media.videoCaps.memory[i].mosaic.maxNumber;
            }
            else
            {
                platform->media.video[i].maxStreams = 1;
            }
        }

        BDBG_MSG(("decode %d streams %d", i, platform->media.video[i].maxStreams));
        platform->media.maxStreams += platform->media.video[i].maxStreams;
        if (platform->media.maxStreams >= MAX_STREAMS)
        {
            platform->media.maxStreams = MAX_STREAMS;
            break;
        }
    }

    platform->display.maxWindows = platform->display.caps.display[0].numVideoWindows;
    BDBG_MSG(("display %d windows %d", 0, platform->display.maxWindows));

    platform->streamId.main = 0;
    platform->streamId.pip = platform->media.maxStreams - 1;

    return platform;

out_join_failure:
    return NULL;
}

void platform_close(PlatformHandle platform)
{
    if (!platform) return;
    NxClient_StopCallbackThread();
    platform_scheduler_p_destroy(platform->schedulers[PLATFORM_SCHEDULER_USAGE]);
    platform_scheduler_p_destroy(platform->schedulers[PLATFORM_SCHEDULER_GFX]);
    platform_scheduler_p_destroy(platform->schedulers[PLATFORM_SCHEDULER_MAIN]);
    BKNI_Free(platform);
    NxClient_Uninit();
}

static const char * capabilityStrings[] =
{
    "           NO",
    "         YES",
    "    UNKNOWN",
    NULL
};

const char * platform_get_capability_name(PlatformCapability cap)
{
    return capabilityStrings[cap];
}

static const char * dynrngStrings[] =
{
    "AUTO",
    "LEGACY",
    "SDR",
    "HLG",
    "HDR10",
    "DBV",
    "HDR10+",
    "TCH",
    "UNKNOWN",
    "UNSUPPORTED",
    NULL
};

const char * platform_get_dynamic_range_name(PlatformDynamicRange dynrng)
{
    return dynrngStrings[dynrng];
}

PlatformDynamicRange platform_get_dynamic_range_from_path(const char * path)
{
    BDBG_ASSERT(path);
    if (strstr(path, "hdr") || strstr(path, "HDR") || strstr(path, "Hdr"))
    {
        return PlatformDynamicRange_eHdr10;
    }
    else if (strstr(path, "hlg") || strstr(path, "HLG") || strstr(path, "Hlg"))
    {
        return PlatformDynamicRange_eHlg;
    }
    else if (strstr(path, "dvs") || strstr(path, "DVS") || strstr(path, "Dvs") || strstr(path, "dovi") || strstr(path, "DoVi") || strstr(path, "dbv"))
    {
        return PlatformDynamicRange_eDolbyVision;
    }
    else
    {
        return PlatformDynamicRange_eSdr;
    }
}

NEXUS_VideoDynamicRangeMode platform_p_output_dynamic_range_to_nexus(PlatformDynamicRange dynrng)
{
    NEXUS_VideoDynamicRangeMode nxDynrng = NEXUS_VideoDynamicRangeMode_eAuto;

    switch (dynrng)
    {
    case PlatformDynamicRange_eLegacy:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eLegacy;
        break;
    case PlatformDynamicRange_eSdr:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eSdr;
        break;
    case PlatformDynamicRange_eHlg:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eHlg;
        break;
    case PlatformDynamicRange_eHdr10:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eHdr10;
        break;
    case PlatformDynamicRange_eDolbyVision:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eDolbyVision;
        break;
    case PlatformDynamicRange_eHdr10Plus:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eHdr10Plus;
        break;
    case PlatformDynamicRange_eAuto:
    case PlatformDynamicRange_eUnknown:
    default:
        nxDynrng = NEXUS_VideoDynamicRangeMode_eAuto;
        break;
    }

    return nxDynrng;
}

PlatformDynamicRange platform_p_input_dynamic_range_from_nexus(NEXUS_VideoEotf nxEotf, NEXUS_VideoDecoderDynamicRangeMetadataType dynamicMetadataType)
{
    PlatformDynamicRange dynrng;

    switch (dynamicMetadataType)
    {
        case NEXUS_VideoDecoderDynamicRangeMetadataType_eDolbyVision:
            dynrng = PlatformDynamicRange_eDolbyVision;
            break;
        case NEXUS_VideoDecoderDynamicRangeMetadataType_eTechnicolorPrime:
            dynrng = PlatformDynamicRange_eTechnicolorPrime;
            break;
        default:
        case NEXUS_VideoDecoderDynamicRangeMetadataType_eNone:
            switch (nxEotf)
            {
                case NEXUS_VideoEotf_eSdr:
                    dynrng = PlatformDynamicRange_eSdr;
                    break;
                case NEXUS_VideoEotf_eHlg:
                    dynrng = PlatformDynamicRange_eHlg;
                    break;
                case NEXUS_VideoEotf_eHdr10:
                    dynrng = PlatformDynamicRange_eHdr10;
                    break;
                case NEXUS_VideoEotf_eInvalid:
                    dynrng = PlatformDynamicRange_eLegacy;
                    break;
                case NEXUS_VideoEotf_eMax:
                    dynrng = PlatformDynamicRange_eAuto;
                    break;
                default:
                    dynrng = PlatformDynamicRange_eUnknown;
                    break;
            }
            break;
    }

    return dynrng;
}

PlatformDynamicRange platform_p_output_dynamic_range_from_nexus(NEXUS_VideoDynamicRangeMode nxDynrng)
{
    PlatformDynamicRange dynrng;

    switch (nxDynrng)
    {
    case NEXUS_VideoDynamicRangeMode_eAuto:
        dynrng = PlatformDynamicRange_eAuto;
        break;
    case NEXUS_VideoDynamicRangeMode_eLegacy:
        dynrng = PlatformDynamicRange_eLegacy;
        break;
    case NEXUS_VideoDynamicRangeMode_eSdr:
        dynrng = PlatformDynamicRange_eSdr;
        break;
    case NEXUS_VideoDynamicRangeMode_eHlg:
        dynrng = PlatformDynamicRange_eHlg;
        break;
    case NEXUS_VideoDynamicRangeMode_eHdr10:
        dynrng = PlatformDynamicRange_eHdr10;
        break;
    case NEXUS_VideoDynamicRangeMode_eDolbyVision:
        dynrng = PlatformDynamicRange_eDolbyVision;
        break;
    case NEXUS_VideoDynamicRangeMode_eHdr10Plus:
        dynrng = PlatformDynamicRange_eHdr10Plus;
        break;
    default:
        dynrng = PlatformDynamicRange_eUnknown;
        break;
    }

    return dynrng;
}

static const char * colorimetryStrings[] =
{
    "AUTO",
    "BT601",
    "BT709",
    "BT2020",
    "INVALID",
    "UNKNOWN",
    "UNSUPPORTED",
    NULL
};

const char * platform_get_colorimetry_name(PlatformColorimetry colorimetry)
{
    return colorimetryStrings[colorimetry];
}

NEXUS_MatrixCoefficients platform_p_colorimetry_to_nexus(PlatformColorimetry colorimetry)
{
    NEXUS_MatrixCoefficients nxColorimetry;

    switch (colorimetry)
    {
        case PlatformColorimetry_e601:
            nxColorimetry = NEXUS_MatrixCoefficients_eXvYCC_601;
            break;
        case PlatformColorimetry_e709:
            nxColorimetry = NEXUS_MatrixCoefficients_eItu_R_BT_709;
            break;
        case PlatformColorimetry_e2020:
            nxColorimetry = NEXUS_MatrixCoefficients_eItu_R_BT_2020_NCL;
            break;
        case PlatformColorimetry_eAuto:
        case PlatformColorimetry_eUnknown:
        default:
            nxColorimetry = NEXUS_MatrixCoefficients_eMax;
            break;
    }

    return nxColorimetry;
}

PlatformColorimetry platform_p_colorimetry_from_nexus(NEXUS_MatrixCoefficients nxColorimetry)
{
    PlatformColorimetry colorimetry;

    switch (nxColorimetry)
    {
        case NEXUS_MatrixCoefficients_eUnknown:
        case NEXUS_MatrixCoefficients_eItu_R_BT_470_2_BG:
        case NEXUS_MatrixCoefficients_eSmpte_170M:
        case NEXUS_MatrixCoefficients_eXvYCC_601:
            colorimetry = PlatformColorimetry_e601;
            break;
        case NEXUS_MatrixCoefficients_eSmpte_240M:
        case NEXUS_MatrixCoefficients_eXvYCC_709:
        case NEXUS_MatrixCoefficients_eItu_R_BT_709:
            colorimetry = PlatformColorimetry_e709;
            break;
        case NEXUS_MatrixCoefficients_eItu_R_BT_2020_CL:
        case NEXUS_MatrixCoefficients_eItu_R_BT_2020_NCL:
            colorimetry = PlatformColorimetry_e2020;
            break;
        case NEXUS_MatrixCoefficients_eMax:
            colorimetry = PlatformColorimetry_eAuto;
            break;
        default:
            colorimetry = PlatformColorimetry_eUnknown;
            break;
    }

    return colorimetry;
}

static const char * colorSpaceStrings[] =
{
    "AUTO",
    "RGB",
    "YCbCr",
    "INVALID",
    "UNKNOWN",
    "UNSUPPORTED",
    NULL
};

const char * platform_get_color_space_name(PlatformColorSpace colorSpace)
{
    return colorSpaceStrings[colorSpace];
}

NEXUS_ColorSpace platform_p_color_space_and_sampling_to_nexus(PlatformColorSpace colorSpace, int colorSampling)
{
    NEXUS_ColorSpace nxColorSpace;

    switch (colorSpace)
    {
        case PlatformColorSpace_eAuto:
            nxColorSpace = NEXUS_ColorSpace_eAuto;
            break;
        case PlatformColorSpace_eRgb:
            nxColorSpace = NEXUS_ColorSpace_eRgb;
            break;
        case PlatformColorSpace_eYCbCr:
            switch (colorSampling)
            {
                case 0:
                    nxColorSpace = NEXUS_ColorSpace_eAuto;
                    break;
                case 420:
                    nxColorSpace = NEXUS_ColorSpace_eYCbCr420;
                    break;
                case 422:
                    nxColorSpace = NEXUS_ColorSpace_eYCbCr422;
                    break;
                default:
                case 444:
                    nxColorSpace = NEXUS_ColorSpace_eYCbCr444;
                    break;
            }
            break;
        case PlatformColorSpace_eInvalid:
        case PlatformColorSpace_eUnknown:
        default:
            nxColorSpace = NEXUS_ColorSpace_eMax;
            break;
    }

    return nxColorSpace;
}

PlatformColorSpace platform_p_color_space_from_nexus(NEXUS_ColorSpace nxColorSpace)
{
    PlatformColorSpace colorSpace;

    switch (nxColorSpace)
    {
        case NEXUS_ColorSpace_eAuto:
            colorSpace = PlatformColorSpace_eAuto;
            break;
        case NEXUS_ColorSpace_eRgb:
            colorSpace = PlatformColorSpace_eRgb;
            break;
        case NEXUS_ColorSpace_eYCbCr420:
        case NEXUS_ColorSpace_eYCbCr422:
        case NEXUS_ColorSpace_eYCbCr444:
            colorSpace = PlatformColorSpace_eYCbCr;
            break;
        case NEXUS_ColorSpace_eMax:
            colorSpace = PlatformColorSpace_eInvalid;
            break;
        default:
            colorSpace = PlatformColorSpace_eUnknown;
            break;
    }

    return colorSpace;
}

int platform_p_color_sampling_from_nexus(NEXUS_ColorSpace nxColorSpace)
{
    int colorSampling;

    switch (nxColorSpace)
    {
        case NEXUS_ColorSpace_eAuto:
            colorSampling = 0;
            break;
        case NEXUS_ColorSpace_eRgb:
            colorSampling = 444;
            break;
        case NEXUS_ColorSpace_eYCbCr420:
            colorSampling = 420;
            break;
        case NEXUS_ColorSpace_eYCbCr422:
            colorSampling = 422;
            break;
        case NEXUS_ColorSpace_eYCbCr444:
            colorSampling = 444;
            break;
        case NEXUS_ColorSpace_eMax:
            colorSampling = -1;
            break;
        default:
            colorSampling = -1;
            break;
    }

    return colorSampling;
}

static const struct {
    unsigned frequency;
    bool dropFrame;
    NEXUS_VideoFrameRate nexusFramerate;
} b_verticalfrequency[NEXUS_VideoFrameRate_eMax] = {
/* array should be sorted by the frequency to facilitate implementations of NEXUS_P_RefreshRate_FromFrameRate_isrsafe and NEXUS_P_FrameRate_FromRefreshRate_isrsafe */
    {  8, true, NEXUS_VideoFrameRate_e7_493},
    {  8, false, NEXUS_VideoFrameRate_e7_5},
    { 10, true, NEXUS_VideoFrameRate_e9_99},
    { 10, false, NEXUS_VideoFrameRate_e10},
    { 12, true, NEXUS_VideoFrameRate_e11_988},
    { 12, false, NEXUS_VideoFrameRate_e12},
    { 13, false, NEXUS_VideoFrameRate_e12_5},
    { 14, true, NEXUS_VideoFrameRate_e14_985},
    { 15, false, NEXUS_VideoFrameRate_e15},
    { 20, true, NEXUS_VideoFrameRate_e19_98},
    { 20, false, NEXUS_VideoFrameRate_e20},
    { 24, true, NEXUS_VideoFrameRate_e23_976},
    { 24, false, NEXUS_VideoFrameRate_e24},
    { 25, false, NEXUS_VideoFrameRate_e25},
    { 30, true, NEXUS_VideoFrameRate_e29_97},
    { 30, false, NEXUS_VideoFrameRate_e30},
    { 50, false, NEXUS_VideoFrameRate_e50},
    { 60, true, NEXUS_VideoFrameRate_e59_94},
    { 60, false, NEXUS_VideoFrameRate_e60},
    {100, false, NEXUS_VideoFrameRate_e100},
    {120, true, NEXUS_VideoFrameRate_e119_88},
    {120, true, NEXUS_VideoFrameRate_e120}
};

unsigned platform_p_frame_rate_from_nexus(NEXUS_VideoFrameRate frameRate)
{
    unsigned i;
    for(i=0;i<sizeof(b_verticalfrequency)/sizeof(*b_verticalfrequency);i++) {
        if (frameRate == b_verticalfrequency[i].nexusFramerate) {
            return b_verticalfrequency[i].frequency;
        }
    }
    BERR_TRACE(NEXUS_NOT_SUPPORTED);
    return 0; /* NEXUS_VideoFrameRate_eUnknown */
}

bool platform_p_drop_frame_from_nexus(NEXUS_VideoFrameRate frameRate)
{
    unsigned i;
    for(i=0;i<sizeof(b_verticalfrequency)/sizeof(*b_verticalfrequency);i++) {
        if (frameRate == b_verticalfrequency[i].nexusFramerate) {
            return b_verticalfrequency[i].dropFrame;
        }
    }
    BERR_TRACE(NEXUS_NOT_SUPPORTED);
    return false;
}

void platform_p_picture_format_from_nexus(NEXUS_VideoFormat format, PlatformPictureFormat * pFormat)
{
    NEXUS_VideoFormatInfo info;
    BDBG_ASSERT(pFormat);
    NEXUS_VideoFormat_GetInfo(format, &info);
    pFormat->width = info.width;
    pFormat->height = info.height;
    pFormat->interlaced = info.interlaced;
    pFormat->rate = info.verticalFreq;
    if (pFormat->rate % 10)
    {
        pFormat->dropFrame = true;
        pFormat->rate *= 1001;
        pFormat->rate /= 1000;
    }
    pFormat->rate /= 10;
    if (pFormat->rate % 10)
    {
        pFormat->rate += 10;
    }
    pFormat->rate /= 10;
}

NEXUS_VideoFormat platform_p_picture_format_to_nexus(const PlatformPictureFormat * pFormat)
{
    NEXUS_VideoFormat format = NEXUS_VideoFormat_eUnknown;

    BDBG_ASSERT(pFormat);

    if (pFormat->interlaced)
    {
        switch (pFormat->height)
        {
            case 1080:
                switch (pFormat->rate)
                {
                    case 50:
                        format = NEXUS_VideoFormat_e1080i50hz;
                        break;
                    case 60:
                    default:
                        format = NEXUS_VideoFormat_e1080i;
                        break;
                }
                break;
            case 576:
                format = NEXUS_VideoFormat_ePal;
                break;
            case 480:
            default:
                format = NEXUS_VideoFormat_eNtsc;
                break;
        }
    }
    else /* progressive */
    {
        switch (pFormat->height)
        {
            case 480:
                format = NEXUS_VideoFormat_e480p;
                break;
            case 576:
                format = NEXUS_VideoFormat_e576p;
                break;
            case 720:
                switch (pFormat->rate)
                {
                    case 24:
                        format = NEXUS_VideoFormat_e720p24hz;
                        break;
                    case 25:
                        format = NEXUS_VideoFormat_e720p25hz;
                        break;
                    case 30:
                        format = NEXUS_VideoFormat_e720p30hz;
                        break;
                    case 50:
                        format = NEXUS_VideoFormat_e720p50hz;
                        break;
                    case 60:
                    default:
                        format = NEXUS_VideoFormat_e720p;
                        break;
                }
                break;
            case 2160:
                switch (pFormat->rate)
                {
                    case 24:
                        format = NEXUS_VideoFormat_e3840x2160p24hz;
                        break;
                    case 25:
                        format = NEXUS_VideoFormat_e3840x2160p25hz;
                        break;
                    case 30:
                        format = NEXUS_VideoFormat_e3840x2160p30hz;
                        break;
                    case 50:
                        format = NEXUS_VideoFormat_e3840x2160p50hz;
                        break;
                    case 60:
                    default:
                        format = NEXUS_VideoFormat_e3840x2160p60hz;
                        break;
                }
                break;
            case 1080:
            default:
                switch (pFormat->rate)
                {
                    case 24:
                        format = NEXUS_VideoFormat_e1080p24hz;
                        break;
                    case 25:
                        format = NEXUS_VideoFormat_e1080p25hz;
                        break;
                    case 30:
                        format = NEXUS_VideoFormat_e1080p30hz;
                        break;
                    case 50:
                        format = NEXUS_VideoFormat_e1080p50hz;
                        break;
                    case 60:
                    default:
                        format = NEXUS_VideoFormat_e1080p60hz;
                        break;
                }
                break;
        }
    }

    return format;
}

static const char * aspectRatioTypeStrings[] =
{
    "AUTO",
    "DAR",
    "SAR",
    "UNSUPPORTED",
    NULL
};

const char * platform_get_aspect_ratio_type_name(PlatformAspectRatioType type)
{
    return aspectRatioTypeStrings[type];
}

void platform_p_aspect_ratio_from_nexus(PlatformAspectRatio * pAr, NEXUS_DisplayAspectRatio ar, unsigned x, unsigned y)
{
    BDBG_ASSERT(pAr);
    BKNI_Memset(pAr, 0, sizeof(*pAr));
    switch (ar)
    {
        case NEXUS_DisplayAspectRatio_e16x9:
            pAr->type = PlatformAspectRatioType_eDisplay;
            pAr->x = 16;
            pAr->y = 9;
            break;
        case NEXUS_DisplayAspectRatio_e4x3:
            pAr->type = PlatformAspectRatioType_eDisplay;
            pAr->x = 4;
            pAr->y = 3;
            break;
        case NEXUS_DisplayAspectRatio_eSar:
            pAr->type = PlatformAspectRatioType_ePixel;
            pAr->x = x;
            pAr->y = y;
            break;
        default:
        case NEXUS_DisplayAspectRatio_eAuto:
            pAr->type = PlatformAspectRatioType_eAuto;
            break;
    }
}

NEXUS_DisplayAspectRatio platform_p_aspect_ratio_to_nexus(const PlatformAspectRatio * pAr, unsigned * pX, unsigned * pY)
{
    NEXUS_DisplayAspectRatio ar;

    BDBG_ASSERT(pAr);

    switch (pAr->type)
    {
        case PlatformAspectRatioType_eDisplay:
            if (pAr->x == 16 && pAr->y == 9)
            {
                ar = NEXUS_DisplayAspectRatio_e16x9;
            }
            else if (pAr->x == 4 && pAr->y == 3)
            {
                ar = NEXUS_DisplayAspectRatio_e4x3;
            }
            else
            {
                ar = NEXUS_DisplayAspectRatio_eAuto;
            }
            break;
        case PlatformAspectRatioType_ePixel:
            ar = NEXUS_DisplayAspectRatio_eSar;
            *pX = pAr->x;
            *pY = pAr->y;
            break;
        default:
        case PlatformAspectRatioType_eAuto:
            ar = NEXUS_DisplayAspectRatio_eAuto;
            break;
    }

    return ar;
}

PlatformRenderingPriority platform_p_rendering_priority_from_nexus(NEXUS_DisplayPriority nxRenderingPriority)
{
    PlatformRenderingPriority renderingPriority;

    switch (nxRenderingPriority)
    {
        case NEXUS_DisplayPriority_eVideo:
            renderingPriority = PlatformRenderingPriority_eVideo;
            break;
        case NEXUS_DisplayPriority_eGraphics:
            renderingPriority = PlatformRenderingPriority_eGraphics;
            break;
        case NEXUS_DisplayPriority_eAuto:
        default:
            renderingPriority = PlatformRenderingPriority_eAuto;
            break;
    }

    return renderingPriority;
}

NEXUS_DisplayPriority platform_p_rendering_priority_to_nexus(PlatformRenderingPriority renderingPriority)
{
    NEXUS_DisplayPriority nxRenderingPriority;

    switch (renderingPriority)
    {
        case PlatformRenderingPriority_eVideo:
            nxRenderingPriority = NEXUS_DisplayPriority_eVideo;
            break;
        case PlatformRenderingPriority_eGraphics:
            nxRenderingPriority = NEXUS_DisplayPriority_eGraphics;
            break;
        case PlatformRenderingPriority_eAuto:
        default:
            nxRenderingPriority = NEXUS_DisplayPriority_eAuto;
            break;
    }

    return nxRenderingPriority;
}

void platform_p_hotplug_handler(void * context, int param)
{
    PlatformHandle platform = context;
    BSTD_UNUSED(param);
    BDBG_ASSERT(platform);
    if (platform->rx)
    {
        platform_hdmi_receiver_p_hotplug_handler(platform->rx);
    }
}

void platform_get_default_model(PlatformModel * pModel)
{
    unsigned i;
    BKNI_Memset(pModel, 0, sizeof(*pModel));
    for(i=0; i<MAX_STREAMS; i++) {
        platform_get_default_picture_info(&pModel->vid[i].info);
        pModel->vid[i].processing = PlatformTriState_eMax;
    }
    platform_get_default_picture_info(&pModel->gfx.info);
    platform_get_default_picture_info(&pModel->out.info);
    pModel->rcv.dynrng = PlatformCapability_eUnknown;
    pModel->gfx.processing = PlatformTriState_eMax;
}

void platform_get_default_picture_info(PlatformPictureInfo * pInfo)
{
    BKNI_Memset(pInfo, 0, sizeof(*pInfo));
    pInfo->dynrng = PlatformDynamicRange_eUnknown;
    pInfo->gamut = PlatformColorimetry_eUnknown;
    pInfo->space = PlatformColorSpace_eUnknown;
}

void platform_print_picture_info(const char * tag, const PlatformPictureInfo * pInfo, char * buf, size_t len)
{
    BKNI_Snprintf(buf, len, "%s: %u%c%u%s %s %ux%u %u-bit %s%u %s",
        tag,
        pInfo->format.height,
        pInfo->format.interlaced ? 'i' : 'p',
        pInfo->format.rate,
        pInfo->format.dropFrame ? "d" : "",
        platform_get_aspect_ratio_type_name(pInfo->ar.type),
        pInfo->ar.x,
        pInfo->ar.y,
        pInfo->depth,
        platform_get_color_space_name(pInfo->space),
        pInfo->sampling,
        platform_get_colorimetry_name(pInfo->gamut)
    );
}

PlatformSchedulerHandle platform_get_scheduler(PlatformHandle platform, unsigned index)
{
    BDBG_ASSERT(platform);
    if (index < PLATFORM_SCHEDULER_COUNT)
    {
        return platform->schedulers[index];
    }
    else
    {
        return NULL;
    }
}

unsigned platform_get_main_stream_id(PlatformHandle platform)
{
    BDBG_ASSERT(platform);
    return platform->streamId.main;
}

unsigned platform_get_pip_stream_id(PlatformHandle platform)
{
    BDBG_ASSERT(platform);
    return platform->streamId.pip;
}

unsigned platform_get_max_stream_count(PlatformHandle platform)
{
    BDBG_ASSERT(platform);
    return platform->media.maxStreams;
}

unsigned platform_get_decoder_max_stream_count(PlatformHandle platform, unsigned decoderIndex)
{
    BDBG_ASSERT(platform);
    return platform->media.video[decoderIndex].maxStreams;
}

unsigned platform_p_get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000 + tv.tv_usec/1000;
}
