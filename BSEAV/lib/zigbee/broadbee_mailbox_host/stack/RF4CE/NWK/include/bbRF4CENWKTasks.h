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

/******************************************************************************
 *
 * DESCRIPTION:
 *      This is the header file for the RF4CE Network Layer component tasks handling routines.
 *
*******************************************************************************/

#ifndef _RF4CE_NWK_TASKS_H
#define _RF4CE_NWK_TASKS_H
/************************* INCLUDES ****************************************************/
#include "bbSysTypes.h"
#include "bbRF4CENWKConstants.h"
#include "bbRF4CENWKEnums.h"
#include "bbSysTaskScheduler.h"

/************************* DEFINITIONS *************************************************/
/**//**
 * \brief RF4CE NWK Task ID enumeration.
  */
typedef enum _RF4CE_NWK_TaskID_t
{
    RF4CE_NWK_START_RESET_REQUEST_HANDLER_ID = 0,
    RF4CE_NWK_GETSET_HANDLER_ID,
    RF4CE_NWK_AUTO_DISCOVERY_REQUEST_HANDLER_ID,
    RF4CE_NWK_DISCOVERY_REQUEST_HANDLER_ID,
    RF4CE_NWK_PAIR_REQUEST_HANDLER_ID,
    RF4CE_NWK_UPDATE_KEY_HANDLER_ID,
    RF4CE_NWK_DATA_REQUEST_HANDLER_ID,
    RF4CE_NWK_DATA_REQUEST_ENCRYPT_HANDLER_ID,
    RF4CE_NWK_ON_AUTO_DISCOVERY_HANDLER_ID,
    RF4CE_NWK_AUTO_DISCOVERY_REQUEST_HANDLER2_ID,
    RF4CE_NWK_ON_DISCOVERY_DURATION_HANDLER_ID,
    RF4CE_NWK_PAIR_ORIGINATOR_TIMEOUT_HANDLER_ID,
    RF4CE_NWK_PAIR_RECIPIENT_TIMEOUT_HANDLER_ID,
    RF4CE_NWK_ON_DATA_INDICATION_HANDLER_ID,
    RF4CE_NWK_POWER_SAVING_HANDLER_ID,
    RF4CE_NWK_FREQUENCY_AGILITY_HANDLER_ID,
    RF4CE_NWK_RX_ENABLE_HANDLER_ID,
    RF4CE_NWK_DATA_TIMEOUT
} RF4CE_NWK_TaskID_t;

#endif /* _RF4CE_NWK_TASKS_H */

/* eof bbRF4CENWKTasks.h */
