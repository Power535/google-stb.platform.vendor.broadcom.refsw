/***************************************************************************
 * Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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


/*= Module Overview *********************************************************
<verbatim>

Overview
The KPD PI module controls the keypad controller within the BCM7038-B0.  The
keypad controller (along with the LED controller) is not available in the
BCM7038-A0 version.

Design
The design for BKPD PI API is broken into three parts.

o Part 1 (open/close/configuration):

  These APIs are used for opening and closing BKPD device.
    
o Part 2 (wait for key press):

  These APIs are used to scan for key presses.

o Part 3 (get the data):

  These APIs are used to read the data from the keypad.

Usage
The usage of BKPD involves the following:

  * Configure/Open of BKPD

    * Configure BKPD device for the target system
    * Open BKPD device

  * Wait for key press

  * Read the data

Sample Code
void main( void )
{
    BKPD_Handle             hKPD;
    BKPD_Settings           defSettings;
    BREG_Handle             hReg;
    BCHP_Handle             hChip;
    BINT_Handle             hInt;
    bool                    keyReady = FALSE;
    uint16_t                key;

    // Do other initialization, i.e. for BREG, BCHP, etc.

    // Now, ready to setup the BKPD
    BKPD_GetDefaultSettings( &defSettings, hChip );

    // Make any changes required from the default values
    defSettings.prescale = 0x60;
    defSettings.debounce = 0x40;

    BKPD_Open (&hKPD, hChip, hReg, hInt, &defSettings);

    // Wait for a key to be hit
    do
    {
        BKPD_IsDataReady (hKPD, &keyReady);
    }   
    while (keyReady == FALSE);

    // key is now ready, go read it
    BKPD_Read (hKPD, &key);

}

</verbatim>
***************************************************************************/


#ifndef BKPD_H__
#define BKPD_H__

#include "bchp.h"
#include "breg_mem.h"
#include "bint.h"
#include "bkni.h"
#include "berr_ids.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
Summary:
    Error Codes specific to BKPD

Description:

See Also:

****************************************************************************/
#define BKPD_ERR_NO_KEY             BERR_MAKE_CODE(BERR_KPD_ID, 0)  /* no key press */

/***************************************************************************
Summary:
    The handle for kpd module.

Description:

See Also:
    BKPD_Open()

****************************************************************************/
typedef struct BKPD_P_Handle                *BKPD_Handle;

/***************************************************************************
Summary:
    Required default settings structure for KPD module.

Description:
    The default setting structure defines the default configure of
    KPD when the device is opened.  

See Also:
    BKPD_Open()

****************************************************************************/
typedef struct BKPD_Settings
{
   uint32_t         prescale;       /* 27mhz clock prescale value */
   uint8_t          debounce;       /* debounce count */
   bool             intMode;        /* interrupt enable */
   uint16_t         keyMask;        /* bitmask to filter interrupts. not supported on all silicon. */
} BKPD_Settings;


/***************************************************************************
Summary:
    Callback function for BKPD

Description:
    The callback function is registered by the caller.  It gets called
    when KPD interrupt happens.

See Also:
    BKPD_InstallInterruptCallback()

****************************************************************************/
typedef void (*BKPD_CallbackFunc)( void *pParm1, int parm2, void *unused);

/***************************************************************************
Summary:
    This function opens KPD module.

Description:
    This function is responsible for opening BKPD module. When BKPD is
    opened, it will create a module handle and configure the module based
    on the default settings. Once the device is opened, it must be closed
    before it can be opened again.

See Also:
    BKPD_Close()
    BKPD_GetDefaultSettings()

****************************************************************************/
BERR_Code BKPD_Open(
    BKPD_Handle *pKPD,                  /* [out] Returns handle */
    BCHP_Handle hChip,                  /* [in] Chip handle */
    BREG_Handle hRegister,              /* [in] Register handle */
    BINT_Handle hInterrupt,             /* [in] Interrupt handle */
    const BKPD_Settings *pDefSettings   /* [in] Default settings */
    );

/***************************************************************************
Summary:
    This function closes KPD module.

Description:
    This function is responsible for closing BKPD module. Closing BKPD 
    will free main BKPD handle. 

See Also:
    BKPD_Open()

****************************************************************************/
BERR_Code BKPD_Close(
    BKPD_Handle hDev                    /* [in] Device handle */
    );

/***************************************************************************
Summary:
    This function returns the default settings for KPD module.

Description:
    This function is responsible for returns the default setting for 
    BKPD module. The returning default setting should be when
    opening the device.

See Also:
    BKPD_Open()

****************************************************************************/
BERR_Code BKPD_GetDefaultSettings(
    BKPD_Settings *pDefSettings,        /* [out] Returns default setting */
    BCHP_Handle hChip                   /* [in] Chip handle */
    );

/***************************************************************************
Summary:
    This function checks for a key press

Description:
    This function is used to check for a key press.
****************************************************************************/
BERR_Code BKPD_IsDataReady ( 
    BKPD_Handle         hKpd,           /* [in] Device handle */
    bool                *ready          /* [out] flag indicating key is pressed */
    );

/***************************************************************************
Summary:
    This function reads the data from the keypad.

Description:
    This function is used to get the key that was pressed.
        
Returns:
    BKPD_ERR_NO_KEY if there is no key press.
****************************************************************************/
BERR_Code BKPD_Read_isrsafe (
    BKPD_Handle         hKpd,           /* [in] Device handle */
    uint16_t            *pData          /* [out] pointer to data read from keypad */
    );

/* backward compat */
#define BKPD_Read BKPD_Read_isrsafe
#define BKPD_Read_isr BKPD_Read_isrsafe

/***************************************************************************
Summary:
    This function installs user-specified interrupt callback function

Description:
    This function is used to install an interrupt callback function.
        
Returns:
    BERR_SUCCESS
****************************************************************************/
BERR_Code BKPD_InstallInterruptCallback (
    BKPD_Handle         hKpd,           /* [in] Device handle */
    BKPD_CallbackFunc   callback_isr,   /* [in] ISR callback function */
    void                *pParm1,        /* [in] application specified parameter */
    int                 parm2           /* [in] application specified parameter */
);

/***************************************************************************
Summary:
    This function uninstalls user-specified interrupt callback function

Description:
    This function is used to uninstall an interrupt callback function.
        
Returns:
    BERR_SUCCESS
****************************************************************************/
BERR_Code BKPD_UnInstallInterruptCallback (
    BKPD_Handle         hKpd            /* [in] Device handle */
);

/***************************************************************************
Summary:
    This function gets the event handle for BKPD module.

Description:
    This function is responsible for getting the event handle. The
    application code should use this function get BKPD's event handle, 
    which the application should use to pend on.  The KPD ISR will 
    set the event.
****************************************************************************/
BERR_Code BKPD_GetEventHandle(
    BKPD_Handle         hKpd,           /* [in] Device handle */
    BKNI_EventHandle    *phEvent        /* [out] Returns event handle */
    );

#ifdef __cplusplus
}
#endif
 
#endif



