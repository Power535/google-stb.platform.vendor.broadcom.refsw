/***************************************************************************
 *  Copyright (C) 2016 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 ***************************************************************************/
#ifndef NEXUS_FILE_FIFO_CHUNK_H__
#define NEXUS_FILE_FIFO_CHUNK_H__

#include "nexus_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
Summary:
Type of record file suitable for timeshifting
*/
typedef struct NEXUS_ChunkedFifoRecord *NEXUS_ChunkedFifoRecordHandle;

/*
Summary:
Configuration parameters for the timeshifting buffer
*/
typedef struct NEXUS_ChunkedFifoRecordLimit {
    off_t soft; /* see NEXUS_FifoRecordLimit.soft */
    off_t hard; /* unused */
    off_t chunkSize;    /* Maximum size of the single chunk, 0 - unlimited size */
} NEXUS_ChunkedFifoRecordLimit;

/*
Summary:
Configuration parameters for the timeshifting file
*/
typedef struct NEXUS_ChunkedFifoRecordSettings {
    unsigned interval; /* Size of the timeshifting buffer, in seconds */
    unsigned snapshotInterval; /* interval between metadata writes, in seconds. 0 means metadata written only when file is closed */
    NEXUS_ChunkedFifoRecordLimit data; /* Limits for the timeshifting data file */
    NEXUS_ChunkedFifoRecordLimit index; /* Limits for the timeshifting index file */
    char chunkTemplate[128]; /* Template for the individual chunk file names. See NEXUS_ChunkedFilePlayOpenSettings for requirements. */
} NEXUS_ChunkedFifoRecordSettings;

/*
Summary:
Get default settings
*/
void NEXUS_ChunkedFifoRecord_GetDefaultSettings(
    NEXUS_ChunkedFifoRecordSettings *pSettings /* [out] */
    );

/*
Summary:
This function opens new timeshifting buffer

Description:
The NEXUS_ChunkedFifoRecordHandle can be used for only one record at a time.
The files will be created if they don't already exist, and truncated if they do.
The index file is mandatory for the timeshifting.
*/
NEXUS_ChunkedFifoRecordHandle NEXUS_ChunkedFifoRecord_Create(
    const char *mpegFileName, /* mpeg file name to create */
    const char *indexFileName /* index file name to create */
    );

/*
Summary:
Re-open an existing timeshifting buffer for export.
This cannot be used for new recording.
*/
NEXUS_ChunkedFifoRecordHandle NEXUS_ChunkedFifoRecord_ReOpenForExport(
    const char *mpegFileName,
    const char *indexFileName
    );

/*
Summary:
This function returns a handle to be passed into the NEXUS_Record_Start().
*/
NEXUS_FileRecordHandle NEXUS_ChunkedFifoRecord_GetFile(
    NEXUS_ChunkedFifoRecordHandle fifo /* Handle returned by NEXUS_ChunkedFifoRecord_Create */
    );

/**
Summary:
This value is returned by NEXUS_ChunkedFifoRecord_GetPosition to indicate if timestamp or index position is invalid
**/
#define NEXUS_FILE_INVALID_POSITION ((unsigned long)-1)

/*
Summary:
This function returns current position of the recorded file
*/
NEXUS_Error NEXUS_ChunkedFifoRecord_GetPosition(
    NEXUS_ChunkedFifoRecordHandle fifo, /* Handle returned by NEXUS_ChunkedFifoRecord_Create */
    NEXUS_FilePosition *pFirst,  /* [out] first position in the file */
    NEXUS_FilePosition *pLast    /* [out] last position in the file */
    );

/*
Summary:
Open a playback file to pass to NEXUS_Playback_Start().

Description:
This function shall be used only for files created by NEXUS_ChunkedFifoRecord_Create.
The NEXUS_FilePlayHandle can be used for only one playback at a time.

templateName and chunksize is obtained from NEXUS_ChunkedFifoRecord or metadata in indexfile.
*/
NEXUS_FilePlayHandle NEXUS_ChunkedFifoPlay_Open(
    const char *mpegFileName,
    const char *indexFileName,
    NEXUS_ChunkedFifoRecordHandle writer /* Handle returned by NEXUS_ChunkedFifoRecord_Create */
    );

/*
Summary:
This function returns current configuration of the timeshifting fifo
*/
void NEXUS_ChunkedFifoRecord_GetSettings(
    NEXUS_ChunkedFifoRecordHandle fifo, /* Handle returned by NEXUS_ChunkedFifoRecord_Create */
    NEXUS_ChunkedFifoRecordSettings *pSettings /* [out] current settings of timeshifting buffer */
    );

/*
Summary:
This function sets the new configuration of the timeshifting fifo
*/
NEXUS_Error NEXUS_ChunkedFifoRecord_SetSettings(
    NEXUS_ChunkedFifoRecordHandle fifo, /* Handle returned by NEXUS_ChunkedFifoRecord_Create */
    const NEXUS_ChunkedFifoRecordSettings *pSettings /* new settings of timeshifting buffer */
    );

typedef struct NEXUS_ChunkedFifoRecordExportSettings
{
    const char *filename; /* chunked file pattern applied to chunkTemplate */
    const char *indexname; /* optional index file. not chunked. */
    char chunkTemplate[128]; /* Template for the individual chunk file names. See NEXUS_ChunkedFilePlayOpenSettings for requirements. */
    struct {
        unsigned timestamp;
    } first, last;
} NEXUS_ChunkedFifoRecordExportSettings;

void NEXUS_ChunkedFifoRecord_GetDefaultExportSettings(
    NEXUS_ChunkedFifoRecordExportSettings *pSettings
    );

typedef struct NEXUS_ChunkedFifoRecordExport *NEXUS_ChunkedFifoRecordExportHandle;

/*
Export a linear chunked file from a continuous chunked file fifo by creating hardlinks
to chunks and creating a new linear index.

Export will continue until last timestamp is reached or StopExport is called.
*/
NEXUS_ChunkedFifoRecordExportHandle NEXUS_ChunkedFifoRecord_StartExport(
    NEXUS_ChunkedFifoRecordHandle fifo,
    const NEXUS_ChunkedFifoRecordExportSettings *pSettings
    );

void NEXUS_ChunkedFifoRecord_StopExport(
    NEXUS_ChunkedFifoRecordExportHandle handle
    );

typedef enum NEXUS_ChunkedFifoRecordExportState
{
    NEXUS_ChunkedFifoRecordExportState_eIdle,   /* initial state */
    NEXUS_ChunkedFifoRecordExportState_eStarted,
    NEXUS_ChunkedFifoRecordExportState_eFailed, /* terminal state */
    NEXUS_ChunkedFifoRecordExportState_eDone    /* terminal state */
} NEXUS_ChunkedFifoRecordExportState;

typedef struct NEXUS_ChunkedFifoRecordExportStatus
{
    NEXUS_ChunkedFifoRecordExportState state;
    struct {
        /* 'first' values may be greater than requested based on what is available.
        'last' values may be less than requested if export is not done yet. */
        unsigned timestamp;
        unsigned chunkNumber;
        off_t offset;
    } first, last;
} NEXUS_ChunkedFifoRecordExportStatus;

NEXUS_Error NEXUS_ChunkedFifoRecord_GetExportStatus(
    NEXUS_ChunkedFifoRecordExportHandle handle,
    NEXUS_ChunkedFifoRecordExportStatus *pStatus
    );

#ifdef __cplusplus
}
#endif

#endif /* NEXUS_FILE_FIFO_CHUNK_H__ */
