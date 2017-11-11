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
 *****************************************************************************/

#include "nexus_platform.h"
#include <stdio.h>

#if NEXUS_HAS_FRONTEND
#include "nexus_frontend.h"
#include "nexus_parser_band.h"
#include "nexus_recpump.h"
#if NEXUS_HAS_RECORD
#include "nexus_record.h"
#endif
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "bmmt.h"

BDBG_MODULE(mmt_record);
/**
 * mmt_record app generates a regular mpeg2ts stream ( ISO/IEC
 * standard 13818-1 or ITU-T Rec. H.222.0) from a live mmt
 * source in mpeg2ts (3 bytes header + 185 bytes data) format
 *
 * DekTek DTU-215 streamer is used to live stream a file to a
 * QAM tuner/demod on the reference board.
 *
 * In the streamXpress app for DTU-215 modulator:
 * 1. rmx should be turned off
 * 2. Modulation parameters should be QAM-C and QAM-256
 *
 * output file can be tested using any nexus playback app.
 **/
#define MAX_ACQUIRE_TIME 20000
#define MAX_PACKAGES 8
static void lock_changed_callback(void *context, int param)
{
    BSTD_UNUSED(param);
    BKNI_SetEvent((BKNI_EventHandle)context);
}

static void async_status_ready_callback(void *context, int param)
{
    BSTD_UNUSED(param);
    BKNI_SetEvent((BKNI_EventHandle)context);
}

static void cppm_complete_callback(void *context, int param)
{
    BSTD_UNUSED(param);

    BDBG_WRN(("cppm_complete_callback"));
    BKNI_SetEvent((BKNI_EventHandle)context);

    /* Any processing required on CPPM completion would go here, for instance setting an event to wait on, or a boolean. */
}

/* Demonstrate the use of recalibration using cppm.
    This callback is called when the frontend needs to notify the app that composite plant power measurement is required.
    The application needs to run cppm only when convenient, because doing so will probably unlock all the currently locked channels.
*/
static void cppm_callback(void *context, int param)
{
    NEXUS_Error rc;
    NEXUS_FrontendHandle frontend = (NEXUS_FrontendHandle) context;
    NEXUS_FrontendDeviceHandle deviceHandle;
    NEXUS_FrontendDeviceRecalibrateSettings calibrateSettings;
    BSTD_UNUSED(param);

    deviceHandle = NEXUS_Frontend_GetDevice(frontend);
    if (!deviceHandle) {
        BDBG_ERR(("Unable to retrieve frontend device handle.\n"));
    }

    NEXUS_FrontendDevice_GetDefaultRecalibrateSettings(&calibrateSettings);
    calibrateSettings.cppm.enabled = true;
    calibrateSettings.cppm.threshold = 250;
    calibrateSettings.cppm.thresholdHysteresis = 50;
    calibrateSettings.cppm.powerLevelChange.callback = cppm_callback;
    calibrateSettings.cppm.powerLevelChange.context = (void *) frontend;
    rc = NEXUS_FrontendDevice_Recalibrate(deviceHandle, &calibrateSettings);
    if(rc) {
        BDBG_ERR(("NEXUS_FrontendDevice_Recalibrate exited with rc = %d", rc));
    }
}

static unsigned b_get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000 + tv.tv_usec/1000;
}

static void print_usage(void)
{
    printf(
        "Usage: mmt_record <options> filename.ts \n"
        "  --help or -h for help\n"
        "  -freq   #frequency in MHz\n"
        "  -tlv_pid # MPEG2TS pid carrying a TLV stream\n"
         );
}

static NEXUS_Error get_status(NEXUS_FrontendHandle frontend, BKNI_EventHandle statusEvent)
{
    NEXUS_FrontendQamStatus qamStatus;
    int rc;
    NEXUS_FrontendDeviceHandle deviceHandle;
    NEXUS_FrontendDeviceStatus deviceStatus;

    deviceHandle = NEXUS_Frontend_GetDevice(frontend);
    if (!deviceHandle) {
        fprintf(stderr, "Unable to retrieve frontend device handle.\n");
    }

    rc = NEXUS_Frontend_RequestQamAsyncStatus(frontend);
    if(rc == NEXUS_SUCCESS){
        rc = BKNI_WaitForEvent(statusEvent, 1000);
        if (rc) {
            printf("Status not returned\n");
            return BERR_TRACE(rc);
        }
        NEXUS_Frontend_GetQamAsyncStatus(frontend , &qamStatus);

        if (deviceHandle) {
            rc = NEXUS_FrontendDevice_GetStatus(deviceHandle, &deviceStatus);
            if(rc) return BERR_TRACE(rc);
        }
    }
    else if(rc == NEXUS_NOT_SUPPORTED){
        rc = NEXUS_Frontend_GetQamStatus(frontend, &qamStatus);
        if (rc) return BERR_TRACE(rc);
    }
    else {
        return BERR_TRACE(rc);
    }

    printf("\nDownstream lock = %d\n", qamStatus.fecLock);
    printf("Frequency = %d\n", qamStatus.settings.frequency);
    if(qamStatus.settings.mode == NEXUS_FrontendQamMode_e64)
        printf("Mode = NEXUS_FrontendQamMode_e64\n");
    else if(qamStatus.settings.mode == NEXUS_FrontendQamMode_e256)
        printf("Mode  = NEXUS_FrontendQamMode_e256\n");
    else
        printf("Mode = %d\n", qamStatus.settings.mode);
    if(qamStatus.settings.annex == NEXUS_FrontendQamAnnex_eA)
        printf("Annex = NEXUS_FrontendQamAnnex_eA\n");
    else if(qamStatus.settings.annex == NEXUS_FrontendQamAnnex_eB)
        printf("Annex  = NEXUS_FrontendQamAnnex_eB\n");
    else
        printf("Annex = %d\n", qamStatus.settings.annex);
    printf("Symbol rate = %d\n", qamStatus.symbolRate);
    printf("Snr estimate = %d\n", qamStatus.snrEstimate/100 );
    printf("FecCorrected = %d\n", qamStatus.fecCorrected);
    printf("FecUncorrected = %d\n", qamStatus.fecUncorrected);
    printf("DsChannelPower in dBmV = %d\n", qamStatus.dsChannelPower/10);
    printf("DsChannelPower in dBm = %d\n", qamStatus.dsChannelPower/10 - 48);
    if (deviceHandle) {
        printf("AVS enabled = %d\n", deviceStatus.avs.enabled);
        printf("AVS voltage = %d\n", deviceStatus.avs.voltage);
        printf("Device temperature = %d\n", deviceStatus.temperature);
    }
    return 0;
}

int main(int argc, char **argv)
{
    NEXUS_PlatformSettings platformSettings;
    NEXUS_FrontendUserParameters userParams;
    NEXUS_FrontendHandle frontend = NULL;
    NEXUS_FrontendQamSettings qamSettings;
    NEXUS_PlatformConfiguration platformConfig;
    NEXUS_ParserBand parserBand;
    NEXUS_ParserBandSettings parserBandSettings;
    NEXUS_FrontendDeviceHandle deviceHandle;
    NEXUS_FrontendDeviceRecalibrateSettings calibrateSettings;
    NEXUS_Error rc;
    NEXUS_PidChannelHandle videoPidChannel, audioPidChannel;
    bool acquired = false;
    bool firstTune = true;
    unsigned i = 0;
    /* default freq & qam mode */
    unsigned freq = 576000000;
    BKNI_EventHandle statusEvent, lockChangedEvent, cppmEvent;
    bool done = false;
    bool cppm = false;
    bool waitForCppm = false;
    int curarg = 1;
    unsigned  mode = 256;
    unsigned maxAcquireTime = MAX_ACQUIRE_TIME;
    bmmt_t mmt = NULL;
    bmmt_open_settings open_settings;
    bmmt_stream_settings video_stream_settings;
    bmmt_stream_t video_stream;
    bmmt_stream_settings audio_stream_settings;
    bmmt_stream_t audio_stream;
    bmmt_msg_settings msg_settings;
    bmmt_msg_t amt_msg=NULL;
    bmmt_msg_t plt_msg=NULL;
    bmmt_msg_t mpt_msg=NULL;
    uint8_t mmt_si_buf[BMMT_MAX_MMT_SI_BUFFER_SIZE];
    uint8_t tlv_si_buf[BMMT_MAX_TLV_SI_BUFFER_SIZE];
    uint8_t msg_r = 0;
    bmmt_pl_table pl_table;
    bmmt_mp_table mp_table[MAX_PACKAGES];
    btlv_am_table am_table;
    btlv_ip_address ip_addr;
    NEXUS_FileRecordHandle file;
    NEXUS_RecpumpHandle recpump;
    NEXUS_RecordHandle record;
    NEXUS_RecordSettings recordSettings;
    char fname[256] = "stream.mpg";
    int file_arg=0;

    bmmt_get_default_open_settings(&open_settings);
    open_settings.tlv_pid = 0x2d;

    /**
     * read command line parameters
     **/
    while (curarg < argc) {
        if (!strcmp(argv[curarg], "-h") || !strcmp(argv[curarg], "--help")) {
            print_usage();
            return 0;
        }
        else if (*argv[curarg]!='\0' && (*argv[curarg]!='-' || argv[curarg][1]=='\0'))  {
            strcpy(fname,argv[curarg]);
            file_arg++;
        }
        else if (!strcmp(argv[curarg], "-freq") && argc>curarg+1) {
            float f;
            if (sscanf(argv[++curarg], "%f", &f) != 1) f = 0;
            if (f < 1000000)
                freq = (unsigned)(f*1000) * 1000;
            else
                freq = (unsigned)f;
        }
        else if (!strcmp("-tlv_pid",argv[curarg]) && argc>curarg+1) {
             open_settings.tlv_pid = strtol(argv[++curarg],NULL,0);
        }
        else {
            print_usage();
            return 1;
        }
        curarg++;
    }
    if (!file_arg)
    {
        print_usage();
        return 1;
    }
    /**
     * initalize nexus platform
     **/
    NEXUS_Platform_GetDefaultSettings(&platformSettings);
    platformSettings.openFrontend = false;
    platformSettings.transportModuleSettings.maxDataRate.parserBand[NEXUS_ParserBand_e0] = 50 * 1000 * 1000;
    platformSettings.videoDecoderModuleSettings.deferInit = true;
#if NEXUS_HAS_VIDEO_ENCODER
    platformSettings.videoEncoderSettings.deferInit = true;
#endif
    rc = NEXUS_Platform_Init(&platformSettings);
    if (rc) return -1;
    /**
     * initalize nexus frontend
     **/
    rc = NEXUS_Platform_InitFrontend();
    if(rc){rc = BERR_TRACE(rc); goto done;}

    {
        NEXUS_FrontendAcquireSettings settings;
        NEXUS_Frontend_GetDefaultAcquireSettings(&settings);
        settings.capabilities.qam = true;
        frontend = NEXUS_Frontend_Acquire(&settings);
        if (!frontend) {
            BDBG_ERR(("Unable to find QAM-capable frontend"));
            return -1;
        }
    }
    BKNI_CreateEvent(&statusEvent);
    BKNI_CreateEvent(&lockChangedEvent);
    BKNI_CreateEvent(&cppmEvent);
    deviceHandle = NEXUS_Frontend_GetDevice(frontend);
    if (!deviceHandle) {
        fprintf(stderr, "Unable to retrieve frontend device handle.\n");
    }
    /**
     * set front end parameters
     **/
    NEXUS_FrontendDevice_GetDefaultRecalibrateSettings(&calibrateSettings);
    calibrateSettings.cppm.enabled = false;
    calibrateSettings.cppm.threshold = 260;
    calibrateSettings.cppm.thresholdHysteresis = 50;
    calibrateSettings.cppm.powerLevelChange.callback = cppm_callback;
    calibrateSettings.cppm.powerLevelChange.context = (void *) frontend;
    calibrateSettings.cppm.calibrationComplete.callback = cppm_complete_callback;
    calibrateSettings.cppm.calibrationComplete.context = cppmEvent;
    rc = NEXUS_FrontendDevice_Recalibrate(deviceHandle, &calibrateSettings);
    if(rc) return BERR_TRACE(BERR_NOT_INITIALIZED);
    NEXUS_Frontend_GetDefaultQamSettings(&qamSettings);
    qamSettings.frequency = freq;
    qamSettings.mode = NEXUS_FrontendQamMode_e256;
    qamSettings.symbolRate = 5360537;
    qamSettings.annex = NEXUS_FrontendQamAnnex_eB;
    qamSettings.bandwidth = NEXUS_FrontendQamBandwidth_e6Mhz;
    qamSettings.lockCallback.callback = lock_changed_callback;
    qamSettings.lockCallback.context = lockChangedEvent;
    qamSettings.asyncStatusReadyCallback.callback = async_status_ready_callback;
    qamSettings.asyncStatusReadyCallback.context = statusEvent;
    qamSettings.autoAcquire = true;
    NEXUS_Frontend_GetUserParameters(frontend, &userParams);
    /**
     *  Map a parser band to the demod's input band.
     **/
    parserBand = NEXUS_ParserBand_e0;
    NEXUS_ParserBand_GetSettings(parserBand, &parserBandSettings);
    if (userParams.isMtsif)
    {
        parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eMtsif;
        parserBandSettings.sourceTypeSettings.mtsif = NEXUS_Frontend_GetConnector(frontend); /* NEXUS_Frontend_TuneXyz() will connect this frontend to this parser band */
    }
    else
    {
        parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
        parserBandSettings.sourceTypeSettings.inputBand = userParams.param1;  /* Platform initializes this to input band */
    }

    /**
      * set the parser band parameters so that no packets are dropped
      * because of continuity count errors and missing adaptation
      * field.
      **/
    parserBandSettings.transportType = NEXUS_TransportType_eTs;
    parserBandSettings.allPass=true;
    parserBandSettings.teiIgnoreEnabled = true;
    parserBandSettings.acceptAdapt00 = true;
    parserBandSettings.continuityCountEnabled = false;
    parserBandSettings.acceptNullPackets = false;
    NEXUS_ParserBand_SetSettings(parserBand, &parserBandSettings);

    /**
     * open nexus recpump, record and file for generating output
     * mpeg2ts file
     **/
    recpump = NEXUS_Recpump_Open(NEXUS_ANY_ID, NULL);
    BDBG_ASSERT(recpump);
    record = NEXUS_Record_Create();
    BDBG_ASSERT(record);
    NEXUS_Record_GetSettings(record, &recordSettings);
    recordSettings.recpump = recpump;
    NEXUS_Record_SetSettings(record, &recordSettings);
    file = NEXUS_FileRecord_OpenPosix(fname,NULL);
    BDBG_ASSERT(file);

    /**
      * Attempt to tune to the request QAM channel
      **/
    BKNI_ResetEvent(lockChangedEvent);
tune:
    BDBG_WRN(("tuning %d MHz... mode = %d", freq/1000000, qamSettings.mode));
    rc = NEXUS_Frontend_TuneQam(frontend, &qamSettings);
    if(rc){rc = BERR_TRACE(rc); goto done;}
    {
        unsigned start_time = b_get_time();
        while (1)
        {
            unsigned current_time = b_get_time();
            NEXUS_FrontendFastStatus status;
            if (current_time - start_time > maxAcquireTime)
            {
                BDBG_WRN(("application timeout. cannot acquire."));
                break;
            }
            rc = BKNI_WaitForEvent(lockChangedEvent, maxAcquireTime - (current_time - start_time));
            if (rc == BERR_TIMEOUT)
            {
                BDBG_WRN(("application timeout. cannot acquire."));
                break;
            }
            BDBG_ASSERT(!rc);
            rc = NEXUS_Frontend_GetFastStatus(frontend, &status);
            if (rc == NEXUS_SUCCESS)
            {
                if (status.lockStatus == NEXUS_FrontendLockStatus_eLocked)
                {
                    BDBG_WRN(("frontend locked"));
                    acquired = true;
                    break;
                }
                else
                    if (status.lockStatus == NEXUS_FrontendLockStatus_eUnlocked)
                    {
                        BDBG_WRN(("frontend unlocked (acquireInProgress=%d)", status.acquireInProgress));
                         /* Wait for maxAcquireTime when unlocked*/
                    }
                    else
                        if (status.lockStatus == NEXUS_FrontendLockStatus_eNoSignal)
                        {
                            if(!qamSettings.autoAcquire)
                            {
                                BDBG_WRN(("No signal at the tuned frequency."));
                                break;
                            }
                            else
                            BDBG_WRN(("No signal at the tuned frequency. Waiting till the application timesout to allow auto acquire to try again."));
                        }
                        else
                        {
                            BDBG_ERR(("unknown status: %d", status.lockStatus));
                        }
            }
            else
            {
                if (rc == NEXUS_NOT_SUPPORTED)
                {
                    NEXUS_FrontendQamStatus qamStatus;
                    rc = NEXUS_Frontend_GetQamStatus(frontend, &qamStatus);
                    if (rc) {rc = BERR_TRACE(rc); return false;}
                    BDBG_WRN(("frontend %s (fecLock=%d)", qamStatus.receiverLock?"locked":"unlocked", qamStatus.fecLock));
                    if (qamStatus.receiverLock)
                    {
                        acquired = true;
                        break;
                    }
                }
                else
                {
                     BERR_TRACE(rc);
                }
            }
        }
    }
    if (acquired)
    {
        /**
          *  mmt module instantiation
          **/
        open_settings.parserBand = parserBand;
        open_settings.input_format = ebmmt_input_format_mpeg2ts;
        mmt =  bmmt_open(&open_settings);
        BDBG_ASSERT(mmt);
        /**
          * open TLV AMT message context
          **/
        bmmt_msg_get_default_settings(&msg_settings);
        msg_settings.msg_type = ebmmt_msg_type_tlv;
        amt_msg = bmmt_msg_open(mmt,&msg_settings);
        /**
          * start the mmt module
          **/
        bmmt_start(mmt);
        /** extract service ids from input stream and their
          * network addresses from TLV SI AMT
          **/
        while (msg_r < BMMT_MAX_MSG_BUFFERS )
        {
            uint8_t *buf = tlv_si_buf;
            size_t len;
            msg_read1:
            len = bmmt_msg_get_buffer(amt_msg,buf,BMMT_MAX_TLV_SI_BUFFER_SIZE);
            if (len)
            {
                if (bmmt_get_am_table(buf,len,&am_table))
                    break;
                msg_r +=1;
            }
            else
            {
                BKNI_Sleep(50);
                goto msg_read1;
            }
        }
        /**
         *  close TLV msg context
         **/
        bmmt_msg_close(amt_msg);
        if (msg_r == BMMT_MAX_MSG_BUFFERS)
        {
            BDBG_ERR(("TLV SI AMT not found"));
            goto done_mmt;
        }
        /**
          *  set IP filtering for the TLV packets
          **/
        if (am_table.num_of_service_id)
        {
            if (am_table.services[0].is_ipv6)
            {
                ip_addr.type = btlv_ip_address_ipv6;
                BKNI_Memcpy(&ip_addr.address.ipv6.addr,&am_table.services[0].addr.ipv6.dst_addr,sizeof(ip_addr.address.ipv6.addr));
                ip_addr.address.ipv6.port = 0x0; /* ignore port since AMT doesn't provide port number */
            }
            else
            {
                ip_addr.type = btlv_ip_address_ipv4;
                BKNI_Memcpy(&ip_addr.address.ipv4.addr,&am_table.services[0].addr.ipv4.dst_addr,sizeof(ip_addr.address.ipv4.addr));
                ip_addr.address.ipv4.port = 0x0; /* ignore port since AMT doesn't provide port number */
            }
        }
        else
        {
            BDBG_WRN(("no services found in AMT"));
            goto done_mmt;
        }
        bmmt_set_ip_filter(mmt, &ip_addr);
        /**
          *  open PLT message context
          **/
        bmmt_msg_get_default_settings(&msg_settings);
        msg_settings.msg_type = ebmmt_msg_type_mmt;
        msg_settings.pid = 0x0;
        plt_msg = bmmt_msg_open(mmt,&msg_settings);
        /**
          *  extract PLT from PA message
          **/
        msg_r = 0;
        while (msg_r < BMMT_MAX_MSG_BUFFERS)
        {
            uint8_t *buf = mmt_si_buf;
            size_t len;
            msg_read2:
            len = bmmt_msg_get_buffer(plt_msg, buf,BMMT_MAX_MMT_SI_BUFFER_SIZE);
            if (len)
            {
                if (bmmt_get_pl_table(buf,len,&pl_table))
                    break;
                msg_r +=1;
            }
            else
            {
                BKNI_Sleep(50);
                goto msg_read2;
            }
        }
            /**
             *  close plt message context
             **/
        bmmt_msg_close(plt_msg);
        if (msg_r == BMMT_MAX_MSG_BUFFERS)
        {
            BDBG_ERR(("MMT SI PLT not found"));
            goto done_mmt;
        }
        else
        {
            if (pl_table.num_of_packages)
            {
                /*for (i=0;i<pl_table.num_of_packages;i++) */
                i = 0;
                {
                    bmmt_msg_get_default_settings(&msg_settings);
                    msg_settings.msg_type = ebmmt_msg_type_mmt;
                    switch (pl_table.packages[i].location_info.location_type)
                    {
                        case bmmt_general_location_type_id:
                            msg_settings.pid = pl_table.packages[i].location_info.data.packet_id;
                            break;
                        case bmmt_general_location_type_ipv4:
                            msg_settings.pid = pl_table.packages[i].location_info.data.mmt_ipv4.packet_id;
                        break;
                        case bmmt_general_location_type_ipv6:
                            msg_settings.pid = pl_table.packages[i].location_info.data.mmt_ipv6.packet_id;
                        break;
                        default:
                            BDBG_WRN(("MPT packet ID not known"));
                            goto done_mmt;
                    }
                    /**
                      * open MPT message context
                      **/
                    mpt_msg = bmmt_msg_open(mmt,&msg_settings);
                    /**
                      * extract MPT from PA message
                      **/
                    msg_r = 0;
                    while (msg_r < BMMT_MAX_MSG_BUFFERS )
                    {
                        uint8_t *buf = mmt_si_buf;
                        size_t len;
                        msg_read3:
                        len = bmmt_msg_get_buffer(mpt_msg, buf,BMMT_MAX_MMT_SI_BUFFER_SIZE);
                        if (len)
                        {
                            if (bmmt_get_mp_table(buf,len,&mp_table[i]))
                                break;
                            msg_r +=1;
                        }
                        else
                        {
                            BKNI_Sleep(50);
                            goto msg_read3;
                        }
                    }
                    if (msg_r == BMMT_MAX_MSG_BUFFERS)
                    {
                        BDBG_ERR(("MMT SI PMT not found in MMT PID %u",msg_settings.pid));
                        goto done_mmt;
                    }
                    /*bmmt_msg_close(mpt_msg);*/
                }
            }
            else
            {
                BDBG_WRN(("no packages found in the PLT"));
                goto done_mmt;
            }
        }
        /**
          * find video asset index in the 1st MPT
          **/
        for (i=0;(i<mp_table[0].num_of_assets && strcmp((char *)mp_table[0].assets[i].type,"hev1"));i++);
        /**
          * some sample streams have video string as hvc1
          **/
        if (i==mp_table[0].num_of_assets)
        {
            for (i=0;(i<mp_table[0].num_of_assets && strcmp((char *)mp_table[0].assets[i].type,"hvc1"));i++);
        }
        /**
          * open video stream context
          **/
        if (i!=mp_table[0].num_of_assets)
        {
            /**
              *   open video stream context for the video packet ID in the
              *   1st asset of MPT
              **/
            bmmt_stream_get_default_settings(&video_stream_settings);
            switch (mp_table[0].assets[i].location_info[0].location_type)
            {
            case bmmt_general_location_type_id:
                video_stream_settings.pid = mp_table[0].assets[i].id[0] << 8 | mp_table[0].assets[i].id[1];
                /*video_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.packet_id;*/
                break;
            case bmmt_general_location_type_ipv4:
                video_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.mmt_ipv4.packet_id;
                break;
            case bmmt_general_location_type_ipv6:
                video_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.mmt_ipv6.packet_id;
                break;
            default:
                BDBG_WRN(("video stream location ID not supported"));
                goto done_mmt;
            }
            if (video_stream_settings.pid )
            {
                video_stream_settings.stream_type = bmmt_stream_type_h265;
                BDBG_WRN(("mp_table[0].assets[i].type %s",mp_table[0].assets[i].type));
                BDBG_WRN(("video_stream_settings.pid %04x",video_stream_settings.pid));
                video_stream = bmmt_stream_open(mmt,&video_stream_settings);
                BDBG_ASSERT(video_stream);
                videoPidChannel = bmmt_stream_get_pid_channel(video_stream);
                rc = NEXUS_Record_AddPidChannel(record,videoPidChannel, NULL);
                BDBG_ASSERT(!rc);
            }
            else
            {
                BDBG_WRN(("video stream location ID not supported"));
            }
        }
        else
        {
            BDBG_WRN(("no video asset was found"));
        }
        /**
          * find audio asset index in the 1st MPT
          **/
        for (i=0;(i<mp_table[0].num_of_assets && strcmp((char *)mp_table[0].assets[i].type,"mp4a"));i++) ;
        /**
          *   open audio stream context
          **/
        if(i!=mp_table[0].num_of_assets)
        {
            /**
              * open video stream context for the video packet ID in the 1st
              * asset of MPT
              **/
            bmmt_stream_get_default_settings(&audio_stream_settings);
            switch (mp_table[0].assets[i].location_info[0].location_type)
            {
            case bmmt_general_location_type_id:
                audio_stream_settings.pid = mp_table[0].assets[i].id[0] << 8 | mp_table[0].assets[i].id[1];
                /*audio_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.packet_id;*/
                break;
            case bmmt_general_location_type_ipv4:
                audio_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.mmt_ipv4.packet_id;
                break;
            case bmmt_general_location_type_ipv6:
                audio_stream_settings.pid = mp_table[0].assets[i].location_info[0].data.mmt_ipv6.packet_id;
                break;
            default:
                BDBG_WRN(("video stream location ID not supported"));
                goto done_mmt;
            }
            if (audio_stream_settings.pid )
            {
                audio_stream_settings.stream_type = bmmt_stream_type_aac;
                BDBG_WRN(("mp_table[0].assets[i].type %s",mp_table[0].assets[i].type));
                BDBG_WRN(("audio_stream_settings.pid %04x",audio_stream_settings.pid));
                audio_stream = bmmt_stream_open(mmt,&audio_stream_settings);
                BDBG_ASSERT(audio_stream);
                audioPidChannel = bmmt_stream_get_pid_channel(audio_stream);
                rc = NEXUS_Record_AddPidChannel(record,audioPidChannel, NULL);
                BDBG_ASSERT(!rc);
            }
            else
            {
                BDBG_WRN(("audio stream location ID not supported"));
            }
        }
        else
        {
            BDBG_WRN(("no audio asset was found"));
        }
        NEXUS_Record_Start(record, file);
        BDBG_WRN(("press enter to exit"));
        getchar();
        NEXUS_Record_Stop(record);
done_mmt:
        bmmt_stop(mmt);
        bmmt_close(mmt);
        BDBG_WRN(("done with mmt"));
    }
    else
    {
        if (firstTune)
        {
            /* trigger CPPM due to lack of signal */
            firstTune = false;
            if (cppm)
            {
                calibrateSettings.cppm.enabled = true;
                rc = NEXUS_FrontendDevice_Recalibrate(deviceHandle, &calibrateSettings);
                if(rc) return BERR_TRACE(BERR_NOT_INITIALIZED);
                if (cppm && waitForCppm)
                {
                    BDBG_WRN(("Waiting for CPPM to complete..."));
                    BKNI_WaitForEvent(cppmEvent, 5000);
                }
                goto tune;
            }
        }
        BDBG_WRN(("not starting record because frontend is not acquired"));
   }

done:
    NEXUS_FileRecord_Close(file);
    NEXUS_Record_Destroy(record);
    if(statusEvent)BKNI_DestroyEvent(statusEvent);
    if(lockChangedEvent)BKNI_DestroyEvent(lockChangedEvent);
    if(cppmEvent)BKNI_DestroyEvent(cppmEvent);
    NEXUS_Platform_Uninit();
    return 0;
}

#else  /* if NEXUS_HAS_FRONTEND */
int main(void)
{
    printf("ERROR: This platform doesn't include frontend.inc \n");
    return -1;
}
#endif /* if NEXUS_HAS_FRONTEND */
