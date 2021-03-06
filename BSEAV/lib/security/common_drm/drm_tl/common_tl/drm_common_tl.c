/***************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
 ***************************************************************************/
#include <errno.h>
#include <string.h>

#include "bstd.h"
#include "bdbg.h"
#include "bkni.h"
#include "bkni_multi.h"

#include "bcrypt.h"
#include "bcrypt_rsa_sw.h"
#include "bcrypt_sha256_sw.h"

#include "drm_metadata_tl.h"
#include "drm_types.h"
#include "drm_common_priv.h"
#include "drm_common_tl.h"
#include "drm_common_swcrypto_types.h"

#include "nexus_base_os.h"
#include "nexus_dma.h"
#include "nexus_memory.h"
#include "nexus_platform_client.h"
#include "nexus_random_number.h"
#include "nexus_otpmsp.h"
#include "nexus_base_os.h"

#include "bsagelib_types.h"
#include "sage_srai.h"

#include "drm_playback_tl.h"
#include "drm_types.h"

#define DEFAULT_DRM_BIN_FILESIZE (64*1024)
#define DRM_COMMON_OVERWRITE_BIN_FILE (1)
#define SRAI_PlatformId_eCommonDrm (0)
#define MAX_DMA_BLOCKS DRM_COMMON_TL_MAX_DMA_BLOCKS

static int DrmCommon_TL_Counter = 0;
static BKNI_MutexHandle drmCommonTLMutex = 0;

static uint8_t *drm_bin_file_buff = NULL;
static drm_bin_header_t drm_bin_header;

static SRAI_PlatformHandle platformHandle = NULL;
static BSAGElib_State platform_status;

static uint32_t DRM_Common_P_CheckDrmBinFileSize(void);

BDBG_MODULE(drm_common_tl);

DrmRC
DRM_Common_TL_Initialize( DrmCommonInit_t *pCommonTLSettings)
{
    DrmRC rc = Drm_Success;
    BSAGElib_InOutContainer *container = NULL;
    BERR_Code sage_rc = BERR_SUCCESS;

    BDBG_ENTER(DRM_Common_Initialize);

    if(drmCommonTLMutex == 0)
    {
        if(BKNI_CreateMutex(&drmCommonTLMutex) != BERR_SUCCESS)
        {
            BDBG_ERR(("%s - Error calling creating mutex", __FUNCTION__));
            goto ErrorExit;
        }
    }
    BKNI_AcquireMutex(drmCommonTLMutex);

    /* Call to  DRM_Common_BasicInitialize needed? Possible for DMA operations */
    rc = DRM_Common_BasicInitialize(pCommonTLSettings);
    if(rc != Drm_Success)
    {
        BDBG_ERR(("%s - Error calling BasicInitialize", __FUNCTION__));
        goto ErrorExit;
    }

    if(platformHandle == NULL) {
        sage_rc = SRAI_Platform_Open(BSAGE_PLATFORM_ID_COMMONDRM, &platform_status, &platformHandle);
        if (sage_rc != BERR_SUCCESS)
        {
            BDBG_ERR(("%s - Error calling platform_open", __FUNCTION__));
            rc = Drm_Err;
            goto ErrorExit;
        }

        if(platform_status == BSAGElib_State_eUninit)
        {
            BDBG_WRN(("%s - platform_status == BSAGElib_State_eUninit ************************* (platformHandle = 0x%08x)", __FUNCTION__, platformHandle));
            container = SRAI_Container_Allocate();
            if(container == NULL)
            {
                BDBG_ERR(("%s - Error fetching container", __FUNCTION__));
                rc = Drm_Err;
                goto ErrorExit;
            }

            sage_rc = SRAI_Platform_Init(platformHandle, container);
            if (sage_rc != BERR_SUCCESS)
            {
                BDBG_ERR(("%s - Error calling platform init", __FUNCTION__));
                rc = Drm_Err;
                goto ErrorExit;
            }


            /* does any info need to be extracted from container ??? */
        }
        else{
            BDBG_WRN(("%s - Platform already initialized *************************", __FUNCTION__));
        }
    }
    else{
        BDBG_WRN(("%s - Platform already opened *************************", __FUNCTION__));
    }

    DrmCommon_TL_Counter++;
ErrorExit:

    if (container != NULL) {
        SRAI_Container_Free(container);
    }

    BDBG_LEAVE(DRM_Common_Initialize);

    BKNI_ReleaseMutex(drmCommonTLMutex);
    return rc;
}


DrmRC
DRM_Common_TL_ModuleInitialize(DrmCommon_ModuleId_e module_id,
                           char * drm_bin_filename,
                           BSAGElib_InOutContainer *container,
                            SRAI_ModuleHandle *moduleHandle)
{
    DrmRC rc = Drm_Success;
    FILE * fptr = NULL;
    uint32_t filesize = 0;
    uint32_t read_size = 0;
    uint32_t filesize_from_header = 0;
    uint32_t write_size = 0;
    BERR_Code sage_rc = BERR_SUCCESS;

    BKNI_AcquireMutex(drmCommonTLMutex);

    BDBG_ENTER(DRM_Common_ModuleInitialize);


    /* if module uses a bin file - read it and add it to the container */
    if(drm_bin_filename != NULL)
    {
        BDBG_MSG(("%s - DRM bin filename '%s'", __FUNCTION__, drm_bin_filename));
        /*
         * 1) allocate drm_bin_file_buff
         * 2) read bin file
         * 3) check size
         * */
        rc = DRM_Common_P_GetFileSize(drm_bin_filename, &filesize);
        if(rc != Drm_Success)
        {
            BDBG_ERR(("%s - Error determine file size of bin file", __FUNCTION__));
            goto ErrorExit;
        }

        DRM_Common_MemoryAllocate(&drm_bin_file_buff, filesize);
        if(drm_bin_file_buff == NULL)
        {
            BDBG_ERR(("%s - Error allocating '%u' bytes", __FUNCTION__, filesize));
            (*moduleHandle) = NULL;
            rc = Drm_MemErr;
            goto ErrorExit;
        }

        fptr = fopen(drm_bin_filename, "rb");
        if(fptr == NULL)
        {
            BDBG_ERR(("%s - Error opening drm bin file (%s)", __FUNCTION__, drm_bin_filename));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }

        read_size = fread(drm_bin_file_buff, 1, filesize, fptr);
        if(read_size != filesize)
        {
            BDBG_ERR(("%s - Error reading drm bin file size (%u != %u)", __FUNCTION__, read_size, filesize));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }

        /* close file and set to NULL */
        if(fclose(fptr) != 0)
        {
            BDBG_ERR(("%s - Error closing drm bin file '%s'.  (%s)", __FUNCTION__, drm_bin_filename, strerror(errno)));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }

        fptr = NULL;

        /* verify allocated drm_bin_file_buff size with size in header */
        filesize_from_header = DRM_Common_P_CheckDrmBinFileSize();
        if(filesize_from_header != filesize)
        {
            BDBG_ERR(("%s - Error validating file size in header (%u != %u)", __FUNCTION__, filesize_from_header, filesize));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }

        BDBG_MSG(("%s - Error validating file size in header (%u ?=? %u)", __FUNCTION__, filesize_from_header, filesize));

        /* All index '0' shared blocks will be reserved for drm bin file data */

        /* first verify that it has not been already used by the calling module */
        if(container->blocks[0].data.ptr != NULL)
        {
            BDBG_ERR(("%s - Shared block[0] reserved for all DRM modules with bin file to pass to Sage.", __FUNCTION__));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }

        container->blocks[0].len = filesize_from_header;

        container->blocks[0].data.ptr = SRAI_Memory_Allocate(filesize_from_header, SRAI_MemoryType_Shared);
        BDBG_MSG(("%s - Allocating SHARED MEMORY of '%u' bytes for shared block[0] (address %p)", __FUNCTION__, filesize_from_header, container->blocks[0].data.ptr));
        if (container->blocks[0].data.ptr == NULL)
        {
            BDBG_ERR(("%s - Error allocating SRAI memory", __FUNCTION__));
            (*moduleHandle) = NULL;
            rc = Drm_SraiModuleError;
            goto ErrorExit;
        }
        BKNI_Memcpy(container->blocks[0].data.ptr, drm_bin_file_buff, filesize_from_header);

        BDBG_MSG(("%s - Copied '%u' bytes into SRAI container (address %p)", __FUNCTION__, filesize_from_header, container->blocks[0].data.ptr));
    }

    /* All modules will call SRAI_Module_Init */
    BDBG_MSG(("%s - ************************* (platformHandle = 0x%08x)", __FUNCTION__, platformHandle));
    sage_rc = SRAI_Module_Init(platformHandle, module_id, container, moduleHandle);
    if(sage_rc != BERR_SUCCESS)
    {
        BDBG_ERR(("%s - Error calling SRAI_Module_Init", __FUNCTION__));
        (*moduleHandle) = NULL;
        rc = Drm_SraiModuleError;
        goto ErrorExit;
    }

    /* Extract DRM bin file manager response from basic[0].  Free memory if failed */
    sage_rc = container->basicOut[0];
    if(sage_rc != BERR_SUCCESS)
    {
        BDBG_ERR(("%s - SAGE error processing DRM bin file", __FUNCTION__));

        if(container->blocks[0].data.ptr != NULL){
            SRAI_Memory_Free(container->blocks[0].data.ptr);
            container->blocks[0].data.ptr = NULL;
        }

        (*moduleHandle) = NULL;
        rc = Drm_SraiModuleError;

        goto ErrorExit;
    }

#ifndef CMNDRM_SKIP_BINFILE_OVERWRITE
    /* Overwrite the drm bin file in rootfs, free up the buffer since a copy will exist on the sage side */
    if(drm_bin_filename != NULL)
    {
        if(container->basicOut[1] == DRM_COMMON_OVERWRITE_BIN_FILE)
        {
            BDBG_MSG(("%s - Overwriting file '%s'", __FUNCTION__, drm_bin_filename));

            if(fptr != NULL)
            {
                BDBG_ERR(("%s - File pointer already opened, invalid state.  '%s'", __FUNCTION__, drm_bin_filename));
                rc = Drm_Err;
                goto ErrorExit;
            }

            /* Overwrite drm bin file once bounded */
            fptr = fopen(drm_bin_filename, "w+b");
            if(fptr == NULL)
            {
                BDBG_ERR(("%s - Error opening DRM bin file (%s) in 'w+b' mode.  '%s'", __FUNCTION__, drm_bin_filename, strerror(errno)));
                rc = Drm_Err;
                goto ErrorExit;
            }

            write_size = fwrite(container->blocks[0].data.ptr, 1, filesize_from_header, fptr);
            if(write_size != filesize)
            {
                BDBG_ERR(("%s - Error writing drm bin file size to rootfs (%u != %u)", __FUNCTION__, write_size, filesize));
                (*moduleHandle) = NULL;
                rc = Drm_SraiModuleError;
                goto ErrorExit;
            }
            fclose(fptr);
            fptr = NULL;
        }
        else{
            BDBG_MSG(("%s - No need to overwrite file '%s'", __FUNCTION__, drm_bin_filename));
        }
    }
#endif

ErrorExit:

    /* if shared block[0] is not null, free since there was an error processing (i.e. can't trust the data) */
    if(container->blocks[0].data.ptr != NULL){
        SRAI_Memory_Free(container->blocks[0].data.ptr);
        container->blocks[0].data.ptr = NULL;
    }

    if(drm_bin_file_buff != NULL){
        DRM_Common_MemoryFree(drm_bin_file_buff);
        drm_bin_file_buff = NULL;
    }

    if(fptr != NULL){
        fclose(fptr);
        fptr = NULL;
    }

    BDBG_LEAVE(DRM_Common_ModuleInitialize);

    BKNI_ReleaseMutex(drmCommonTLMutex);
    return rc;
}


DrmRC
DRM_Common_TL_ModuleFinalize(SRAI_ModuleHandle moduleHandle)
{

    BKNI_AcquireMutex(drmCommonTLMutex);

    BDBG_ENTER(DRM_Common_ModuleFinalize);

    SRAI_Module_Uninit(moduleHandle);

    BDBG_LEAVE(DRM_Common_ModuleFinalize);

    BKNI_ReleaseMutex(drmCommonTLMutex);
    return Drm_Success;
}



uint32_t
DRM_Common_P_CheckDrmBinFileSize(void)
{
    uint32_t tmp_file_size = 0;

    BDBG_ENTER(DRM_Common_P_CheckDrmBinFileSize);

    BKNI_Memcpy((uint8_t*)&drm_bin_header, drm_bin_file_buff, sizeof(drm_bin_header_t));

    tmp_file_size = GET_UINT32_FROM_BUF(drm_bin_header.bin_file_size);
    BDBG_MSG(("%s - line = %u", __FUNCTION__, __LINE__));
    if(tmp_file_size > DEFAULT_DRM_BIN_FILESIZE)
    {
        BDBG_MSG(("%s - line = %u", __FUNCTION__, __LINE__));
        DRM_Common_MemoryFree(drm_bin_file_buff);
        drm_bin_file_buff = NULL;
        BDBG_MSG(("%s - line = %u", __FUNCTION__, __LINE__));
        DRM_Common_MemoryAllocate(&drm_bin_file_buff, tmp_file_size);
        BDBG_MSG(("%s - line = %u", __FUNCTION__, __LINE__));
        BKNI_Memset(drm_bin_file_buff, 0x00, tmp_file_size);
    }

    BDBG_MSG(("%s - Exiting function (%u)", __FUNCTION__, tmp_file_size));

    return tmp_file_size;
}

DrmRC DRM_Common_TL_URR_Toggle(void)
{
    DrmRC drmRc = Drm_Success;
    DrmPlaybackHandle_t playbackHandle = NULL;

    drmRc = DRM_Playback_Initialize(&playbackHandle);
    if (drmRc != Drm_Success || playbackHandle == NULL) {
        BDBG_ERR(("%s: DRM_Playback_Initialize returned error %d", __FUNCTION__, drmRc));
        goto toggle_error_exit;
    }
    drmRc = DRM_Playback_Stop(playbackHandle);
    if (drmRc != Drm_Success) {
        BDBG_ERR(("%s: DRM_Playback_Stop returned error %d", __FUNCTION__, drmRc));
        /* Fall through */
    }

    drmRc = DRM_Playback_Finalize(playbackHandle);
    if (drmRc != Drm_Success) {
         BDBG_ERR(("%s: DRM_Playback_Finalize returned error %d",__FUNCTION__, drmRc));
    }

toggle_error_exit:
    return drmRc;
}


DrmRC DRM_Common_TL_Finalize(void)
{
    DrmRC rc = Drm_Success;

    BDBG_ASSERT(drmCommonTLMutex != NULL);
    BKNI_AcquireMutex(drmCommonTLMutex);

    BDBG_MSG(("%s - Entered function (init = '%d')", __FUNCTION__, DrmCommon_TL_Counter));

    /* sanity check */
    if(DrmCommon_TL_Counter <= 0)
    {
        BDBG_WRN(("%s - DrmCommon_TL_Counter value is invalid ('%d').  Possible bad thread exit", __FUNCTION__, DrmCommon_TL_Counter));
    }

    /* if there's one DRM module left calling DRM_Common_Finalize, clean everything up
     * Otherwise skip the clean up and decrement the counter */
    if(DrmCommon_TL_Counter == 1)
    {
        /* Call to URR Toggle. DRM_Common_TL_Finalize() will be called within its own context so we must
        * release the mutex.  The counter will increment again preventing an infinite loop. */
        BKNI_ReleaseMutex(drmCommonTLMutex);

        BDBG_MSG(("%s - Start URR toggle", __FUNCTION__));
        rc = DRM_Common_TL_URR_Toggle();
        if (rc != Drm_Success) {
            BDBG_ERR(("%s - Error performing URR toggle", __FUNCTION__));
        }
        BDBG_MSG(("%s - URR toggle completed", __FUNCTION__));

        BKNI_AcquireMutex(drmCommonTLMutex);
        BDBG_MSG(("%s - Cleaning up Common DRM TL only parameters ***************************", __FUNCTION__));
        if (platformHandle) {
            SRAI_Platform_Close(platformHandle);
            platformHandle = NULL;
        }

        /* Finalize and decrement the counter after we finish the URR toggle */
        DRM_Common_Finalize();
        DrmCommon_TL_Counter--;

        BKNI_ReleaseMutex(drmCommonTLMutex);
        BKNI_DestroyMutex(drmCommonTLMutex);
        drmCommonTLMutex = 0;
    }
    else
    {
        DRM_Common_Finalize();
        if(DrmCommon_TL_Counter > 0){
            DrmCommon_TL_Counter--;
        }

        BKNI_ReleaseMutex(drmCommonTLMutex);
    }

    BDBG_MSG(("%s - Exiting function (init = '%d')", __FUNCTION__, DrmCommon_TL_Counter));

    return rc;
}


DrmRC DRM_Common_P_GetFileSize(char * filename, uint32_t *filesize)
{
    DrmRC rc = Drm_Success;
    FILE *fptr = NULL;
    int pos = 0;

    BDBG_ENTER(DRM_Common_P_GetFileSize);

    fptr = fopen(filename, "rb");
    if(fptr == NULL)
    {
        BDBG_ERR(("%s - Error opening file '%s'.  (%s)", __FUNCTION__, filename, strerror(errno)));
        rc = Drm_FileErr;
        goto ErrorExit;
    }

    pos = fseek(fptr, 0, SEEK_END);
    if(pos == -1)
    {
        BDBG_ERR(("%s - Error seeking to end of file '%s'.  (%s)", __FUNCTION__, filename, strerror(errno)));
        rc = Drm_FileErr;
        goto ErrorExit;
    }

    pos = ftell(fptr);
    if(pos == -1)
    {
        BDBG_ERR(("%s - Error determining position of file pointer of file '%s'.  (%s)", __FUNCTION__, filename, strerror(errno)));
        rc = Drm_FileErr;
        goto ErrorExit;
    }

    /* check vs. arbitrary large file size */
    if(pos >= 256*1024)
    {
        BDBG_ERR(("%s - Invalid file size detected for of file '%s'.  (%u)", __FUNCTION__, filename, pos));
        rc = Drm_FileErr;
        goto ErrorExit;
    }

    (*filesize) = pos;

ErrorExit:

    BDBG_MSG(("%s - Exiting function (%u bytes)", __FUNCTION__, (*filesize)));

    if(fptr != NULL)
    {
        /* error closing?!  weird error case not sure how to handle */
        if(fclose(fptr) != 0){
            BDBG_ERR(("%s - Error closing drm bin file '%s'.  (%s)", __FUNCTION__, filename, strerror(errno)));
            rc = Drm_Err;
        }
    }

    return rc;
}

DrmRC DRM_Common_TL_M2mOperation(DrmCommonOperationStruct_t *pDrmCommonOpStruct, bool bSkipCacheFlush, bool bExternalIV )
{
    NEXUS_DmaJobBlockSettings jobBlkSettings[MAX_DMA_BLOCKS];

    CommonCryptoJobSettings jobSettings;
    unsigned int i = 0;
    unsigned int j = 0;
    DrmRC drmRc = Drm_Success;

    /* The mutex is still protecting the DRM Common Handle (resp CommonCrypto handle) table */
    BDBG_MSG(("%s - Entered function", __FUNCTION__));

    /* Sanity check */
    if ( (pDrmCommonOpStruct == NULL) || (pDrmCommonOpStruct->pDmaBlock == NULL) ||
         (pDrmCommonOpStruct->num_dma_block > MAX_DMA_BLOCKS) ||
         ( bExternalIV && pDrmCommonOpStruct->num_dma_block < 1 ))
    {
        BDBG_ERR(("%s - invalid paramters for M2M operation", __FUNCTION__));
        drmRc = Drm_CryptoDmaErr;
        goto ErrorExit;
    }

    BDBG_ASSERT(drmCommonTLMutex != NULL);
    BKNI_AcquireMutex(drmCommonTLMutex);

    DRM_Common_Handle drmHnd;
    drmRc = DRM_Common_GetHandle(&drmHnd);;

    if (drmRc == Drm_Success)
    {
        CommonCrypto_GetDefaultJobSettings(&jobSettings);

        /* Data format used when encrypting/decryption data. */
        /*jobSettings.dataFormat; *//* Set to NEXUS_DmaDataFormat_eBlock in CommonCrypto_GetDefaultJobSettings ?! */
        /* Key slot handle to use during the DMA transfer.  NULL(default) if not encrypting or decrypting data*/
        jobSettings.keySlot = pDrmCommonOpStruct->keyConfigSettings.keySlot;

        DmaBlockInfo_t* pDmaBlock = pDrmCommonOpStruct->pDmaBlock;

        if ( bExternalIV )
        {
            BDBG_MSG(("%s - btp 0x%08x size=%d", __FUNCTION__, pDmaBlock->pSrcData, pDmaBlock->uiDataSize));

            /* External IV BTP data in first dma block */
            NEXUS_DmaJob_GetDefaultBlockSettings(&jobBlkSettings[0]);
            jobBlkSettings[0].pSrcAddr                   = pDmaBlock->pSrcData;
            jobBlkSettings[0].pDestAddr                  = pDmaBlock->pDstData;
            jobBlkSettings[0].blockSize                  = pDmaBlock->uiDataSize;
            jobBlkSettings[0].resetCrypto                = true;
            jobBlkSettings[0].scatterGatherCryptoStart   = true;
            jobBlkSettings[0].scatterGatherCryptoEnd     = true;
            jobBlkSettings[0].securityBtp                = true;
            jobBlkSettings[0].cached                     = false;
            pDmaBlock++;
            j=1;
        }

        for ( i = j; i < pDrmCommonOpStruct->num_dma_block; i++)
        {
            BDBG_MSG(("%s - blkidx=%d, src=0x%08x, des=0x%08x, size=%d, start=%d,end=%d", __FUNCTION__, i, pDmaBlock->pSrcData, pDmaBlock->pDstData, pDmaBlock->uiDataSize, pDmaBlock->sg_start,pDmaBlock->sg_end ));
            NEXUS_DmaJob_GetDefaultBlockSettings (&jobBlkSettings[i]);
            jobBlkSettings[i].pSrcAddr = pDmaBlock->pSrcData;
            jobBlkSettings[i].pDestAddr = pDmaBlock->pDstData;
            jobBlkSettings[i].blockSize = pDmaBlock->uiDataSize;
            jobBlkSettings[i].scatterGatherCryptoStart = pDmaBlock->sg_start;
            jobBlkSettings[i].scatterGatherCryptoEnd = pDmaBlock->sg_end;
            if (bSkipCacheFlush) {
                jobBlkSettings[i].cached = true;
            }
            else
            {
                NEXUS_FlushCache(jobBlkSettings[i].pSrcAddr, jobBlkSettings[i].blockSize);
            }
            pDmaBlock++;
        }

        jobBlkSettings[j].resetCrypto = true;

        BDBG_MSG(("%s - dmaXfer [%d] blocks", __FUNCTION__, pDrmCommonOpStruct->num_dma_block ));
        if (CommonCrypto_DmaXfer(drmHnd->cryptHnd,
                                 &jobSettings,
                                 jobBlkSettings,
                                 pDrmCommonOpStruct->num_dma_block) != NEXUS_SUCCESS)
        {
            BDBG_ERR(("%s - Error with M2M DMA operation", __FUNCTION__));
            drmRc = Drm_CryptoDmaErr;
            goto ErrorExit;
        }

        if (!bSkipCacheFlush) {
            for (i = j; i < pDrmCommonOpStruct->num_dma_block; i++)
            {
                NEXUS_FlushCache(jobBlkSettings[i].pDestAddr, jobBlkSettings[i].blockSize);
            }
        }
    }

ErrorExit:
    BDBG_MSG(("%s - Exiting function", __FUNCTION__));
    BKNI_ReleaseMutex(drmCommonTLMutex);
    return drmRc;
}
