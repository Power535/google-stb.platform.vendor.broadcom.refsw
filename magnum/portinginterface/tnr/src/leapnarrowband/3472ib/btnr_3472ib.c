/******************************************************************************
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
 *****************************************************************************/
#include "bstd.h"
#include "bkni.h"
#include "btnr.h"
#include "btnr_priv.h"
#include "btnr_3472ib.h"
#include "btnr_34xxib_priv.h"

BDBG_MODULE(btnr_3472);

#define DEV_MAGIC_ID                ((BERR_TNR_ID<<16) | 0xFACE)
#define BTNR_3472_SETTINGS_I2CADDR  (0x6C)          /* 7bit addr */
#define BTNR_3472_MAX_CHANNELS      2

/*******************************************************************************/
BERR_Code BTNR_3472_Open(
    BTNR_Handle *phDev,                 /* [output] Returns handle */
    BTNR_3472_Settings *pSettings, /* [Input] settings structure */
    BHAB_Handle hHab                  /* [Input] Hab Handle */   
    )
{
    BERR_Code retCode = BERR_SUCCESS;
    BTNR_34xx_Handle h34xxDev;
    BTNR_P_34xx_Settings *pTnrImplData;
    BTNR_Handle hDev;

    BDBG_ENTER(BTNR_34xx_Open);
    BDBG_ASSERT( hHab );
    
    hDev = NULL;
    /* Alloc memory from the system heap */
    h34xxDev = (BTNR_34xx_Handle) BKNI_Malloc( sizeof( BTNR_P_34xx_Handle ) );
    if( h34xxDev == NULL )
    {
        retCode = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        BDBG_ERR(("BTNR_3472_Open: BKNI_malloc() failed"));
        goto done;
    }
    BKNI_Memset( h34xxDev, 0x00, sizeof( BTNR_P_34xx_Handle ) );

    hDev = (BTNR_Handle) BKNI_Malloc( sizeof( BTNR_P_Handle ) );
    if( hDev == NULL )
    {
        retCode = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        BDBG_ERR(("BTNR_3472_Open: BKNI_malloc() failed"));
        BKNI_Free( h34xxDev );
        goto done;
    }
    BKNI_Memset( hDev, 0x00, sizeof( BTNR_P_Handle ) );

    h34xxDev->magicId = DEV_MAGIC_ID;
    pTnrImplData = &h34xxDev->settings;
    pTnrImplData->rfFreq = 0;
    pTnrImplData->tunerMode = BTNR_TunerMode_eLast;
    h34xxDev->channelNo = pSettings->channelNo;
    h34xxDev->hHab = hHab;
    hDev->hDevImpl = (void *) h34xxDev;
    hDev->magicId = DEV_MAGIC_ID;
    hDev->pSetRfFreq = (BTNR_SetRfFreqFunc) BTNR_34xx_SetRfFreq;
    hDev->pGetRfFreq = (BTNR_GetRfFreqFunc) BTNR_34xx_GetRfFreq;
    hDev->pGetAgcRegVal = (BTNR_GetAgcRegValFunc) BTNR_P_34xx_GetAgcRegVal;
    hDev->pSetAgcRegVal = (BTNR_SetAgcRegValFunc) BTNR_34xx_SetAgcRegVal;
    hDev->pGetInfo = (BTNR_GetInfoFunc) BTNR_34xx_GetInfo;
    hDev->pClose = (BTNR_CloseFunc) BTNR_34xx_Close;
    hDev->pGetPowerSaver = (BTNR_GetPowerSaverFunc) BTNR_34xx_GetPowerSaver;
    hDev->pSetPowerSaver = (BTNR_SetPowerSaverFunc) BTNR_34xx_SetPowerSaver;
    hDev->pGetSettings = (BTNR_GetSettingsFunc) BTNR_34xx_GetSettings;
    hDev->pSetSettings = (BTNR_SetSettingsFunc) BTNR_34xx_SetSettings;
    hDev->pVersionInfo = (BTNR_GetVersionInfoFunc) BTNR_34xx_GetVersionInfo;
    
    if(pSettings->channelNo >= pSettings->mxChnNo)
    {
        retCode = BERR_TRACE(BERR_NOT_SUPPORTED); 
        BDBG_ERR(("Only %d TNR channels supported", pSettings->mxChnNo)); 
    }
    else
        *phDev = hDev;    
done:
    if( retCode != BERR_SUCCESS )
    {
        if( hDev != NULL )
        {
            BKNI_Free( hDev );
        }
        *phDev = NULL;
    }
    BDBG_LEAVE(BTNR_3472_Open);
    return( retCode );
}

BERR_Code BTNR_3472_GetDefaultSettings(
    BTNR_3472_Settings *pDefSettings  /* [out] Returns default setting */
    )
{
    BDBG_ASSERT(NULL != pDefSettings);

    BKNI_Memset(pDefSettings, 0, sizeof(*pDefSettings));
    pDefSettings->i2cAddr = BTNR_3472_SETTINGS_I2CADDR;
    pDefSettings->mxChnNo = BTNR_3472_MAX_CHANNELS;

    return BERR_SUCCESS;
}

