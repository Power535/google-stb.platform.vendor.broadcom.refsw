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
 *      This header describes types and API for the ZDO ZDP service Get Power_Desc.
 *
*******************************************************************************/

#ifndef _ZBPRO_ZDO_SAP_TYPES_GET_POWER_DESC_HANDLER_H
#define _ZBPRO_ZDO_SAP_TYPES_GET_POWER_DESC_HANDLER_H

/************************* INCLUDES *****************************************************/
#include "bbZbProZdoCommon.h"
#include "private/bbZbProZdoIb.h"

/************************* TYPES ********************************************************/
/**//**
 * \brief Power_Desc_req command formats. See ZigBee Spec r20, Figure 2.23.
 * \ingroup ZBPRO_ZDO_PowerDescReq
 */
typedef struct _ZBPRO_ZDO_PowerDescReqParams_t
{
    ZBPRO_ZDO_Address_t                 zdpDstAddress;      /*!< ZDP Destination address. */
    ZBPRO_ZDO_NwkAddr_t                 nwkAddrOfInterest;  /*!< Short address of interest. */
    SYS_Time_t                          respWaitTimeout;    /*!< Response waiting timeout, in milliseconds.
                                                                 Zero means 'Use default ZDO timeout'. */
} ZBPRO_ZDO_PowerDescReqParams_t;

/**//**
 * \brief Power_Desc_resp command formats. See ZigBee Spec r20, Figure 2.65.
 * \ingroup ZBPRO_ZDO_PowerDescConf
 */
typedef struct _ZBPRO_ZDO_PowerDescConfParams_t
{
    ZBPRO_ZDO_Status_t                  status;             /*!< Confirmation status */
    ZBPRO_ZDO_NwkAddr_t                 nwkAddrOfInterest;  /*!< Short address of interest. */
    ZbProZdoPowerDescriptor_t           powerDescriptor;    /*!< Power Descriptor */
} ZBPRO_ZDO_PowerDescConfParams_t;

/**//**
 * \brief ZDO ZDP Power_Desc_req descriptor prototype.
 * \ingroup ZBPRO_ZDO_PowerDescReq
 */
typedef struct _ZBPRO_ZDO_PowerDescReqDescr_t  ZBPRO_ZDO_PowerDescReqDescr_t;

/**//**
 * \brief Callback function type.
 * \ingroup ZBPRO_ZDO_PowerDescConf
 */
typedef void ZBPRO_ZDO_PowerDescConfCallback_t(ZBPRO_ZDO_PowerDescReqDescr_t   *const reqDescr,
                                               ZBPRO_ZDO_PowerDescConfParams_t *const confParams);

/**//**
 * \brief ZDO ZDP Power_Desc_req descriptor data type.
 * \ingroup ZBPRO_ZDO_PowerDescReq
 */
struct _ZBPRO_ZDO_PowerDescReqDescr_t
{
    ZbProZdoLocalRequest_t              service;            /*!< Request service field */
    ZBPRO_ZDO_PowerDescConfCallback_t  *callback;           /*!< Confirmation callback */
    ZBPRO_ZDO_PowerDescReqParams_t      params;             /*!< Request parameters */
};

/************************* PROTOTYPES ***************************************************/
/*************************************************************************************//**
 * \brief ZDO ZDP Power_Desc_req function.
 * \ingroup ZBPRO_ZDO_Functions
 * \param[in]   reqDescr        Pointer to ZDO Request descriptor.
 * \return Nothing.
*****************************************************************************************/
void ZBPRO_ZDO_PowerDescReq(ZBPRO_ZDO_PowerDescReqDescr_t *const reqDescr);

#endif /* _ZBPRO_ZDO_SAP_TYPES_GET_POWER_DESC_HANDLER_H */

/* eof bbZbProZdoSapTypesGetPowerDescHandler.h */