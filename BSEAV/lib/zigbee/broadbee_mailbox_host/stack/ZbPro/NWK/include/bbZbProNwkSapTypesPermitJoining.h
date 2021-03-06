/******************************************************************************
* (c) 2014 Broadcom Corporation
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
******************************************************************************/
/*****************************************************************************
*
* FILENAME: $Workfile: trunk/stack/ZbPro/NWK/include/bbZbProNwkSapTypesPermitJoining.h $
*
* DESCRIPTION:
*   Network Layer Management Entity Permit Joining primitive declarations
*
* $Revision: 2465 $
* $Date: 2014-05-21 11:21:20Z $
*
****************************************************************************************/

#ifndef _ZBPRO_NWK_SAP_TYPES_PERMIT_JOINING_H
#define _ZBPRO_NWK_SAP_TYPES_PERMIT_JOINING_H

/************************* INCLUDES ****************************************************/
#include "bbZbProNwkCommon.h"

/************************* DEFINITIONS *************************************************/
/**//**
 * \brief Special value of Permit duration parameter. ZigBee spec r20 3.2.2.5.3.
 */
#define ZBPRO_NWK_PERMIT_JOINING_PERMANENTLY    0xFF

/**//**
 * \brief Special value of Permit duration parameter. ZigBee spec r20 3.2.2.5.3.
 */
#define ZBPRO_NWK_FORBID_JOINING_PERMANENTLY    0x00

/************************* TYPES *******************************************************/
/**//**
 * \brief NLME-PERMIT-JOINING request parameters. ZigBee Spec r20 3.2.2.5.
 */
typedef struct _ZBPRO_NWK_PermitJoiningReqParams_t
{
    /* The length of time in seconds during which the ZigBee coordinator or
     * router will allow associations. The value 0x00 and 0xff indicate that
     * permission is disabled or enabled, respectively, without a specified
     * time limit. */
    ZBPRO_NWK_PermitJoinDuration_t permitDuration;
} ZBPRO_NWK_PermitJoiningReqParams_t;

/**//**
 * \brief NLME-PERMIT-JOINING confirmation parameters. ZigBee Spec r20 3.2.2.6.
 */
typedef struct _ZBPRO_NWK_PermitJoiningConfParams_t
{
    ZBPRO_NWK_Status_t status;
} ZBPRO_NWK_PermitJoiningConfParams_t;

/**//**
 * \brief NLME-PERMIT-JOINING.request descriptor data type declaration.
 */
typedef struct _ZBPRO_NWK_PermitJoiningReqDescr_t  ZBPRO_NWK_PermitJoiningReqDescr_t;

/**//**
 * \brief NLME-PERMIT-JOINING.confirm primitive callback function type.
 */
typedef void (*ZBPRO_NWK_PermitJoiningConfCallback_t)(ZBPRO_NWK_PermitJoiningReqDescr_t *const reqDescr,
        ZBPRO_NWK_PermitJoiningConfParams_t *const conf);

/**//**
 * \brief NLME-PERMIT-JOINING.request descriptor data type definition.
 */
struct _ZBPRO_NWK_PermitJoiningReqDescr_t
{
    ZbProNwkServiceField_t                    service;
    ZBPRO_NWK_PermitJoiningReqParams_t        params;
    ZBPRO_NWK_PermitJoiningConfCallback_t     callback;
};

/************************* PROTOTYPES **************************************************/

/************************************************************************************//**
  \brief NLME-PERMIT-JOINING request primitive function
  \param[in] reqDescr - pointer to the request descriptor structure
****************************************************************************************/
void ZBPRO_NWK_PermitJoiningReq(ZBPRO_NWK_PermitJoiningReqDescr_t *reqDescr);

#endif /* _ZBPRO_NWK_SAP_TYPES_PERMIT_JOINING_H */