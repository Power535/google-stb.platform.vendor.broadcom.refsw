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

#include "bstd.h"           /* standard types */
#include "bdbg.h"           /* Dbglib */
#include "bkni.h"           /* For critical sections */
#include "bvbi.h"           /* VBI processing, this module. */
#include "bvbi_cap.h"
#include "bvbi_priv.h"      /* VBI internal data structures */

BDBG_MODULE(BVBI);


/***************************************************************************
* Forward declarations of static (private) functions
***************************************************************************/


/***************************************************************************
* Implementation of "BVBI_" API functions
***************************************************************************/


/***************************************************************************
 *
 */
BERR_Code BVBI_Field_GetVPSData_isr( BVBI_Field_Handle fieldHandle,
                                     BVBI_VPSData *pVPSData )
{
    BVBI_P_Field_Handle* pVbi_Fld;
    BERR_Code eErr = BERR_SUCCESS;

    BDBG_ENTER(BVBI_Field_GetVPSData_isr);

    /* check parameters */
    pVbi_Fld = fieldHandle;
    BDBG_OBJECT_ASSERT (pVbi_Fld, BVBI_FIELD);
    if(!pVPSData)
    {
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    /* Verify that data is present on this field handle */
    if (!(pVbi_Fld->ulWhichPresent & BVBI_P_SELECT_VPS))
        return (BVBI_ERR_FIELD_NODATA);
    if (pVbi_Fld->ulErrInfo & BVBI_LINE_ERROR_VPS_NOENCODE)
        eErr = (BVBI_ERR_FIELD_BADDATA);

    /* Check that field handle was properly sized */
    if (!pVbi_Fld->pVPSData)
    {
        return BERR_TRACE (BVBI_ERR_FLDH_CONFLICT);
    }

    /* Return data as requested */
    *pVPSData = *pVbi_Fld->pVPSData;

    BDBG_LEAVE(BVBI_Field_GetVPSData_isr);
    return eErr;
}

/***************************************************************************
 *
 */
BERR_Code BVBI_Field_SetVPSData_isr( BVBI_Field_Handle fieldHandle,
                                     BVBI_VPSData *pVPSData )
{
    BVBI_P_Field_Handle* pVbi_Fld;

    BDBG_ENTER(BVBI_Field_SetVPSData_isr);

    /* check parameters */
    pVbi_Fld = fieldHandle;
    BDBG_OBJECT_ASSERT (pVbi_Fld, BVBI_FIELD);
    if(!pVPSData)
    {
        BDBG_ERR(("Invalid parameter"));
        return BERR_TRACE(BERR_INVALID_PARAMETER);
    }

    /* Check that field handle was properly sized */
    if (!pVbi_Fld->pVPSData)
    {
        return BERR_TRACE (BVBI_ERR_FLDH_CONFLICT);
    }

    /* Store data as requested */
    *pVbi_Fld->pVPSData = *pVPSData;

    /* Indicate valid data is present */
    pVbi_Fld->ulWhichPresent |= BVBI_P_SELECT_VPS;

    BDBG_LEAVE(BVBI_Field_SetVPSData_isr);
    return BERR_SUCCESS;
}


/***************************************************************************
* Implementation of supporting VPS functions that are not in API
***************************************************************************/


#if BVBI_NUM_WSE > 0
BERR_Code BVBI_P_VPS_Init( BVBI_P_Handle *pVbi )
{
    uint8_t hwIndex;
    BERR_Code eErr = BERR_SUCCESS;

    BDBG_ENTER(BVBI_P_VPS_Init);

    /* Initialize VPS encoders */
    for (hwIndex = 0 ; hwIndex < BVBI_NUM_WSE ; ++hwIndex)
        BVBI_P_VPS_Enc_Init (pVbi->hReg, hwIndex);

    BDBG_LEAVE(BVBI_P_VPS_Init);
    return eErr;
}
#endif


/***************************************************************************
* Static (private) functions
***************************************************************************/

/* End of file */
