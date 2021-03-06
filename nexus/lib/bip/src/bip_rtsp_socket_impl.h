/******************************************************************************
 * (c) 2016 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 *****************************************************************************/

#ifndef BIP_RTSP_SOCKET_IMPL_H
#define BIP_RTSP_SOCKET_IMPL_H

#include "bip_priv.h"
#include "bip_rtsp_lm_server.h"
#include "bip_rtsp_server.h"

typedef enum
{
    BIP_RtspSocketState_eIdle,           /* state after create when there are no pending messages on this socket */
    BIP_RtspSocketState_eMessagePending, /* there are one or more pending messages yet to be received by the app */
    BIP_RtspSocketState_eError           /* error has occurred on the socket: peer closed or network error */
} BIP_RtspSocketState;

typedef struct BIP_RtspSocketMessageInfo
{
    char    *pBuffer;                                           /* buffer that holds the actual message */
    unsigned bufferLength;                                      /* lenght of bytes in buffer */
    BLST_Q_ENTRY( BIP_RtspSocketMessageInfo ) pendingMessageInfoListNext; /* list of pending messages: happens when client sends pipelines of messages or when main & pip on the client use the same RtspSocket for RTSP */
    BIP_RtspSocketHandle hRtspSocket;
} BIP_RtspSocketMessageInfo;

struct BIP_RtspSocket
{
    BDBG_OBJECT( BIP_RtspSocket )
    BIP_RtspSocketCreateSettings createSettings;
    BIP_RtspSocketSettings        settings;
    BIP_RtspLiveMediaSocketHandle hRtspLmSocket;
    BIP_RtspSocketState           state;
    /* list of pending RTSP messages that app needs to receive */
    BLST_Q_HEAD( pendingMessageInfoListHead, BIP_RtspSocketMessageInfo ) pendingMessageInfoListHead;
    BKNI_MutexHandle lock;
    BLST_Q_ENTRY( BIP_RtspSocket ) rtspSocketListNext; /* list of active BIP_RtspSocket objects */
    char *pInterfaceName;                              /* Interface Name over which this socket connection is established */
} BIP_RtspSocket;

#endif /* BIP_RTSP_SOCKET_IMPL_H */
