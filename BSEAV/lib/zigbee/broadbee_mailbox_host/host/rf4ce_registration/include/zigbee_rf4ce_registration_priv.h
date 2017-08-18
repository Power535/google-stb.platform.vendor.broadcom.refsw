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
******************************************************************************/

/*****************************************************************************
*
* DESCRIPTION:
*   The RF4CE registration function used to deliver the indication.
*
*****************************************************************************/

#ifndef _ZIGBEE_RF4CE_REGISTRATION_PRIV_H
#define _ZIGBEE_RF4CE_REGISTRATION_PRIV_H

#include "bbSysBasics.h"            /* Basic system environment set. */
#include "bbSysQueue.h"

typedef struct _RF4CE_RegisterVirtualDeviceInternalInfo_t
{
    /* 32-bit data. */
    SYS_QueueElement_t elem;   /*!< Service field. */
    RF4CE_RegisterVirtualDeviceParams_t     params;        /*!< Request parameters set.     */
    int clientId;
} RF4CE_RegisterVirtualDeviceInternalInfo_t;

typedef struct _RF4CERegistrationServiceDescr_t
{
    SYS_QueueDescriptor_t registrationInfoQueue;
    pthread_mutex_t       registrationInfoLock;
    SYS_QueueDescriptor_t waitForPairQueue;
    pthread_mutex_t       waitForPairLock;
} RF4CERegistrationServiceDescr_t;

#endif

/* eof zigbee_rf4ce_registration_priv.h */