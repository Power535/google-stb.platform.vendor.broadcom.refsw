/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
******************************************************************************/

/* This file is autogenerated, do not edit. */

#ifndef BHSM_P_OTPMODE0__H
#define BHSM_P_OTPMODE0__H

#ifdef __cplusplus
extern "C"
{
#endif


#include "bhsm.h"

typedef struct BHSM_P_OtpMode0Mc0SRead
{
    /* input */
    struct {
        uint8_t    field;
    }in;

    /* output */
    struct {
        uint32_t    data;
    }out;
} BHSM_P_OtpMode0Mc0SRead;

typedef struct BHSM_P_OtpMode0KeyMc0Read
{
    /* input */
    struct {
        uint8_t    sectionIndex;
        uint8_t    field;
    }in;

    /* output */
    struct {
        uint32_t    data[2];
    }out;
} BHSM_P_OtpMode0KeyMc0Read;

typedef struct BHSM_P_OtpMode0MiscRead
{
    /* input */
    struct {
        uint8_t    field;
    }in;

    /* output */
    struct {
        uint32_t    data;
    }out;
} BHSM_P_OtpMode0MiscRead;

typedef struct BHSM_P_OtpMode0ProgPasswd
{
    /* input */
    struct {
        uint32_t    passwordArray[4];
    }in;
} BHSM_P_OtpMode0ProgPasswd;

typedef struct BHSM_P_OtpMode0Prog
{
    /* input */
    struct {
        uint8_t    sectionType;
        uint8_t    sectionIndex;
        uint8_t    numErrorsMax;
        uint32_t    progData[8];
    }in;
} BHSM_P_OtpMode0Prog;
BERR_Code BHSM_P_OtpMode0_Mc0SRead( BHSM_Handle hHsm, BHSM_P_OtpMode0Mc0SRead *pParam );
BERR_Code BHSM_P_OtpMode0_KeyMc0Read( BHSM_Handle hHsm, BHSM_P_OtpMode0KeyMc0Read *pParam );
BERR_Code BHSM_P_OtpMode0_MiscRead( BHSM_Handle hHsm, BHSM_P_OtpMode0MiscRead *pParam );
BERR_Code BHSM_P_OtpMode0_ProgPasswd( BHSM_Handle hHsm, BHSM_P_OtpMode0ProgPasswd *pParam );
BERR_Code BHSM_P_OtpMode0_Prog( BHSM_Handle hHsm, BHSM_P_OtpMode0Prog *pParam );

#ifdef __cplusplus
}
#endif
#endif