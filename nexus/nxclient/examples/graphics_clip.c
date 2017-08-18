/***************************************************************************
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
 **************************************************************************/
#include "nxclient.h"
#include "nexus_surface.h"
#include "nexus_surface_client.h"
#include "bstd.h"
#include "bkni.h"
#include "bkni_multi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

BDBG_MODULE(client);
#include "nxapp_prompt.inc"

int main(void)
{
    NxClient_AllocSettings allocSettings;
    NxClient_AllocResults allocResults;
    NEXUS_SurfaceHandle surface;
    NEXUS_SurfaceCreateSettings createSettings;
    NEXUS_SurfaceClientHandle blit_client;
    NEXUS_SurfaceMemory mem;
    NEXUS_Error rc;
    unsigned x, y;
    NEXUS_SurfaceComposition comp;

    rc = NxClient_Join(NULL);
    if (rc) return -1;

    NxClient_GetDefaultAllocSettings(&allocSettings);
    allocSettings.surfaceClient = 1;
    rc = NxClient_Alloc(&allocSettings, &allocResults);
    if (rc) return BERR_TRACE(rc);
    blit_client = NEXUS_SurfaceClient_Acquire(allocResults.surfaceClient[0].id);

    NEXUS_Surface_GetDefaultCreateSettings(&createSettings);
    createSettings.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
    createSettings.width = 720;
    createSettings.height = 480;
    surface = NEXUS_Surface_Create(&createSettings);

    NEXUS_Surface_GetMemory(surface, &mem);

    for (y=0;y<createSettings.height;y++) {
        uint32_t *ptr = (uint32_t *)(((uint8_t*)mem.buffer) + y * mem.pitch);
        for (x=0;x<createSettings.width;x++) {
            if (x < createSettings.width/2) {
                ptr[x] = y < createSettings.height/2 ? 0xFFFF0000 : 0xFF00FF00;
            }
            else {
                ptr[x] = y < createSettings.height/2 ? 0xFF0000FF : 0xFFFF00FF;
            }
        }
    }
    NEXUS_Surface_Flush(surface);

    rc = NEXUS_SurfaceClient_SetSurface(blit_client, surface);
    BDBG_ASSERT(!rc);

    nxapp_prompt("continue");

    BDBG_WRN(("clip left"));
    NxClient_GetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    comp.clipBase.width = 1920;
    comp.clipBase.height = 1080;
    comp.clipRect.x = comp.clipBase.width*1/4;
    comp.clipRect.y = 0;
    comp.clipRect.width = comp.clipBase.width*3/4;
    comp.clipRect.height = comp.clipBase.height;
    NxClient_SetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    nxapp_prompt("continue");

    BDBG_WRN(("clip right"));
    NxClient_GetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    comp.clipRect.x = 0;
    comp.clipRect.y = 0;
    comp.clipRect.width = comp.clipBase.width*3/4;
    comp.clipRect.height = comp.clipBase.height;
    NxClient_SetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    nxapp_prompt("continue");

    BDBG_WRN(("clip top"));
    NxClient_GetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    comp.clipRect.x = 0;
    comp.clipRect.y = comp.clipBase.height*1/4;
    comp.clipRect.width = comp.clipBase.width;
    comp.clipRect.height = comp.clipBase.height*3/4;
    NxClient_SetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    nxapp_prompt("continue");

    BDBG_WRN(("clip bottom"));
    NxClient_GetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    comp.clipRect.x = 0;
    comp.clipRect.y = 0;
    comp.clipRect.width = comp.clipBase.width;
    comp.clipRect.height = comp.clipBase.height*3/4;
    NxClient_SetSurfaceClientComposition(allocResults.surfaceClient[0].id, &comp);
    nxapp_prompt("continue");

    NEXUS_SurfaceClient_Release(blit_client);
    NxClient_Free(&allocResults);
    NxClient_Uninit();
    return 0;
}
