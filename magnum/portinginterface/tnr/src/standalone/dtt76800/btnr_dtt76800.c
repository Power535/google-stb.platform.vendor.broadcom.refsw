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
#include "btnr_dtt76800.h"

BDBG_MODULE(btnr_dtt76800);

#define DEV_MAGIC_ID            ((BERR_TNR_ID<<16) | 0xFACE)

#define CHK_RETCODE( rc, func )         \
do {                                        \
    if( (rc = BERR_TRACE(func)) != BERR_SUCCESS ) \
    {                                       \
        goto done;                          \
    }                                       \
} while(0)


/*******************************************************************************
*
*   Private Module Handles
*
*******************************************************************************/


typedef struct BTNR_P_DTT76800_Settings
{
    unsigned long ifFreq;
    BREG_I2C_Handle hI2CReg;
    unsigned short i2cAddr;

    unsigned long rfFreq;
    BTNR_TunerMode tunerMode;
    bool isInitialized;
} BTNR_P_DTT76800_Settings;

typedef struct BTNR_P_DTT76800_Handle       *BTNR_DTT76800_Handle;

typedef struct BTNR_P_DTT76800_Handle
{
    uint32_t magicId;                   /* Used to check if structure is corrupt */

    BTNR_P_DTT76800_Settings settings;
} BTNR_P_DTT76800_Handle;



/*******************************************************************************
*
*   Default Module Settings
*
*******************************************************************************/
static const BTNR_DTT76800_Settings defDevSettings =
{
    BTNR_DTT76800_SETTINGS_IFFREQ,
    BTNR_DTT76800_SETTINGS_I2CADDR
};

/*******************************************************************************
*
*   Private Module Data
*
*******************************************************************************/
static BERR_Code BTNR_DTT76800_Close(
    BTNR_Handle hDev                    /* [in] Device handle */
    )
{
    BERR_Code retCode = BERR_SUCCESS;


    BDBG_ENTER(BTNR_DTT76800_Close);
    BDBG_ASSERT( hDev );
    BDBG_ASSERT( hDev->magicId == DEV_MAGIC_ID );

    hDev->magicId = 0x00;       /* clear it to catch inproper use */
    BKNI_Free( (void *) hDev->hDevImpl );
    BKNI_Free( (void *) hDev );

    BDBG_LEAVE(BTNR_DTT76800_Close);
    return( retCode );
}

static BERR_Code BTNR_DTT76800_SetRfFreq(
    BTNR_DTT76800_Handle hDev,              /* [in] Device handle */
    uint32_t rfFreq,                    /* [in] Requested tuner freq., in Hertz */
    BTNR_TunerMode tunerMode            /* [in] Requested tuner mode */
    )
{
    BERR_Code retCode = BERR_SUCCESS;
    BTNR_P_DTT76800_Settings *pTnrImplData;
    uint8_t tnr[6];
    unsigned long n_cnt;

    BDBG_ENTER(BTNR_DTT76800_SetRfFreq);
    BDBG_ASSERT( hDev );
    BDBG_ASSERT( hDev->magicId == DEV_MAGIC_ID );

    pTnrImplData = &hDev->settings;
    if( pTnrImplData->isInitialized == false )
    {
        /* If required */
        pTnrImplData->isInitialized = true;
    }

    if( rfFreq < 136250001 )
    {
        tnr[3] = 0x39;
    }
    else if( rfFreq < 417250001 )
    {
        tnr[3] = 0x7c;
    }
    else
    {
        tnr[3] = 0xb5;
    }

    n_cnt = (rfFreq + pTnrImplData->ifFreq) / (1000000 / 16);

    tnr[0] = 0x7F & (n_cnt >> 8);
    tnr[1] = 0xFF & n_cnt;
    tnr[2] = 0x85;
    tnr[4] = 0x03;
    tnr[5] = 0x80;

    CHK_RETCODE( retCode, BREG_I2C_WriteNoAddr( pTnrImplData->hI2CReg, pTnrImplData->i2cAddr, tnr, sizeof(tnr) ) );

    if( tunerMode == BTNR_TunerMode_eAnalog )
    {
        BKNI_Sleep(20);
        tnr[5] = 0xA0; /* AGD (bit 5) = 1 to disable internal wideband AGC */
        CHK_RETCODE( retCode, BREG_I2C_WriteNoAddr( pTnrImplData->hI2CReg, pTnrImplData->i2cAddr, tnr, sizeof(tnr) ) );
    }
    else
    {
        BKNI_Sleep(25);
        tnr[4] = 0x23; /* set ATC=1 25msec later, for slow AGC time constant */
        CHK_RETCODE( retCode, BREG_I2C_WriteNoAddr( pTnrImplData->hI2CReg, pTnrImplData->i2cAddr, tnr, sizeof(tnr) ) );
    }

    pTnrImplData->rfFreq = rfFreq;
    pTnrImplData->tunerMode = tunerMode;

done:
    BDBG_LEAVE(BTNR_DTT76800_SetRfFreq);
    return( retCode );
}

static BERR_Code BTNR_DTT76800_GetRfFreq(
    BTNR_DTT76800_Handle hDev,              /* [in] Device handle */
    uint32_t *rfFreq,                   /* [output] Returns tuner freq., in Hertz */
    BTNR_TunerMode *tunerMode           /* [output] Returns tuner mode */
    )
{
    BERR_Code retCode = BERR_SUCCESS;
    BTNR_P_DTT76800_Settings *pTnrImplData;


    BDBG_ENTER(BTNR_DTT76800_GetRfFreq);
    BDBG_ASSERT( hDev );
    BDBG_ASSERT( hDev->magicId == DEV_MAGIC_ID );

    pTnrImplData = &hDev->settings;

    *rfFreq = pTnrImplData->rfFreq;
    *tunerMode = pTnrImplData->tunerMode;

    BDBG_LEAVE(BTNR_DTT76800_GetRfFreq);
    return( retCode );
}

/*******************************************************************************
*
*   Public Module Functions
*
*******************************************************************************/

BERR_Code BTNR_DTT76800_Open(
    BTNR_Handle *phDev,                 /* [output] Returns handle */
    BCHP_Handle hChip,                  /* [in] Chip handle */
    BREG_I2C_Handle hI2CReg,            /* [in] I2C Register handle */
    const BTNR_DTT76800_Settings *pDefSettings /* [in] Default settings */
    )
{
    BERR_Code retCode = BERR_SUCCESS;
    BTNR_DTT76800_Handle hDTT76800Dev;
    BTNR_P_DTT76800_Settings *pTnrImplData;
    BTNR_Handle hDev;


    BDBG_ENTER(BTNR_DTT76800_Open);
    BDBG_ASSERT( hI2CReg );
    BSTD_UNUSED(hChip);

    hDev = NULL;
    /* Alloc memory from the system heap */
    hDTT76800Dev = (BTNR_DTT76800_Handle) BKNI_Malloc( sizeof( BTNR_P_DTT76800_Handle ) );
    if( hDTT76800Dev == NULL )
    {
        retCode = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        BDBG_ERR(("BTNR_DTT76800_Open: BKNI_malloc() failed"));
        goto done;
    }
    BKNI_Memset( hDTT76800Dev, 0x00, sizeof( BTNR_P_DTT76800_Handle ) );

    hDev = (BTNR_Handle) BKNI_Malloc( sizeof( BTNR_P_Handle ) );
    if( hDev == NULL )
    {
        retCode = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        BDBG_ERR(("BTNR_DTT76800_Open: BKNI_malloc() failed"));
        BKNI_Free( hDTT76800Dev );
        goto done;
    }
    BKNI_Memset( hDev, 0x00, sizeof( BTNR_P_Handle ) );

    hDTT76800Dev->magicId = DEV_MAGIC_ID;
    pTnrImplData = &hDTT76800Dev->settings;
    pTnrImplData->hI2CReg = hI2CReg;
    pTnrImplData->ifFreq = pDefSettings->ifFreq;
    pTnrImplData->i2cAddr = pDefSettings->i2cAddr;
    if( (pDefSettings->i2cAddr & 0x80) )
    {
        BDBG_ERR(("I2C Address must be 7 bit format"));
        BDBG_ASSERT( !(pDefSettings->i2cAddr & 0x80) );
    }

    pTnrImplData->rfFreq = 0;
    pTnrImplData->tunerMode = BTNR_TunerMode_eDigital;
    pTnrImplData->isInitialized = false;

    hDev->hDevImpl = (void *) hDTT76800Dev;
    hDev->magicId = DEV_MAGIC_ID;
    hDev->pSetRfFreq = (BTNR_SetRfFreqFunc) BTNR_DTT76800_SetRfFreq;
    hDev->pGetRfFreq = (BTNR_GetRfFreqFunc) BTNR_DTT76800_GetRfFreq;
    hDev->pGetAgcRegVal = (BTNR_GetAgcRegValFunc) NULL;
    hDev->pSetAgcRegVal = (BTNR_SetAgcRegValFunc) NULL;
    hDev->pClose = (BTNR_CloseFunc) BTNR_DTT76800_Close;
    hDev->pGetPowerSaver = (BTNR_GetPowerSaverFunc)NULL;
    hDev->pSetPowerSaver = (BTNR_SetPowerSaverFunc)NULL;


done:
    *phDev = hDev;
    BDBG_LEAVE(BTNR_DTT76800_Open);
    return( retCode );
}

BERR_Code BTNR_DTT76800_GetDefaultSettings(
    BTNR_DTT76800_Settings *pDefSettings,   /* [output] Returns default setting */
    BCHP_Handle hChip                   /* [in] Chip handle */
    )
{
    BERR_Code retCode = BERR_SUCCESS;


    BDBG_ENTER(BTNR_DTT76800_GetDefaultSettings);
    BSTD_UNUSED(hChip);

    *pDefSettings = defDevSettings;

    BDBG_LEAVE(BTNR_DTT76800_GetDefaultSettings);
    return( retCode );
}

