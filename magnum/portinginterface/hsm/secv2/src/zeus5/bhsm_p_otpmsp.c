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

#include "../bhsm_bsp_msg.h"
#include "bsp_components.h"
#include "bsp_otpmsp.h"
#include "bhsm_p_otpmsp.h"


BDBG_MODULE(BHSMa);


BERR_Code BHSM_P_OtpMsp_Read( BHSM_Handle hHsm, BHSM_P_OtpMspRead *pParam )
{
	BERR_Code rc = BERR_SUCCESS;
	BHSM_BspMsg_h hMsg = NULL;
	uint16_t bspError = 0;
	BHSM_BspMsgCreate_t msgCreate;
	BHSM_BspMsgConfigure_t msgConfig;
	Bsp_OtpMsp_Read_InFields_t *pSend = NULL;
	Bsp_OtpMsp_Read_OutFields_t *pReceive = NULL;

	BDBG_ENTER( BHSM_P_OtpMsp_Read );

	BKNI_Memset( &msgCreate, 0, sizeof(msgCreate) );
	BKNI_Memset( &msgConfig, 0, sizeof(msgConfig) );

	hMsg = BHSM_BspMsg_Create( hHsm, &msgCreate );
	if( !hMsg ) { return BERR_TRACE( BHSM_STATUS_RESOURCE_ALLOCATION_ERROR ); }

	pSend = (Bsp_OtpMsp_Read_InFields_t*)msgCreate.pSend;
	pReceive = (Bsp_OtpMsp_Read_OutFields_t*)msgCreate.pReceive;

	msgConfig.component = Bsp_CmdComponent_eOtpMsp;
	msgConfig.command = Bsp_CmdOtpMsp_eRead;

	rc = BHSM_BspMsg_Configure( hMsg, &msgConfig );
	if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

	/* pack input paramters */
	pSend->cmdMspEnum = pParam->in.cmdMspEnum;

	rc = BHSM_BspMsg_SubmitCommand ( hMsg, &bspError );
	if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }

	if( bspError != 0 ) {
	    rc = BHSM_STATUS_BSP_ERROR;
	    BDBG_ERR(("%s BSP Status error [0x%X]", BSTD_FUNCTION, bspError ));
	    goto BHSM_P_DONE_LABEL;
	}

	/* extract output paramters */
	pParam->out.mspData = pReceive->mspData;
	pParam->out.mspLock = pReceive->mspLock;

BHSM_P_DONE_LABEL:

	(void)BHSM_BspMsg_Destroy( hMsg );

	BDBG_LEAVE( BHSM_P_OtpMsp_Read );

	return rc;
}


BERR_Code BHSM_P_OtpMsp_MultiRead( BHSM_Handle hHsm, BHSM_P_OtpMspMultiRead *pParam )
{
	BERR_Code rc = BERR_SUCCESS;
	BHSM_BspMsg_h hMsg = NULL;
	uint16_t bspError = 0;
	BHSM_BspMsgCreate_t msgCreate;
	BHSM_BspMsgConfigure_t msgConfig;
	Bsp_OtpMsp_MultiRead_InFields_t *pSend = NULL;
	Bsp_OtpMsp_MultiRead_OutFields_t *pReceive = NULL;

	BDBG_ENTER( BHSM_P_OtpMsp_MultiRead );

	BKNI_Memset( &msgCreate, 0, sizeof(msgCreate) );
	BKNI_Memset( &msgConfig, 0, sizeof(msgConfig) );

	hMsg = BHSM_BspMsg_Create( hHsm, &msgCreate );
	if( !hMsg ) { return BERR_TRACE( BHSM_STATUS_RESOURCE_ALLOCATION_ERROR ); }

	pSend = (Bsp_OtpMsp_MultiRead_InFields_t*)msgCreate.pSend;
	pReceive = (Bsp_OtpMsp_MultiRead_OutFields_t*)msgCreate.pReceive;

	msgConfig.component = Bsp_CmdComponent_eOtpMsp;
	msgConfig.command = Bsp_CmdOtpMsp_eMultiRead;

	rc = BHSM_BspMsg_Configure( hMsg, &msgConfig );
	if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

	/* pack input paramters */
	pSend->mspType = pParam->in.mspType;
	pSend->startRegIndex = pParam->in.startRegIndex;
	pSend->numRegs = pParam->in.numRegs;

	rc = BHSM_BspMsg_SubmitCommand ( hMsg, &bspError );
	if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }

	if( bspError != 0 ) {
	    rc = BHSM_STATUS_BSP_ERROR;
	    BDBG_ERR(("%s BSP Status error [0x%X]", BSTD_FUNCTION, bspError ));
	    goto BHSM_P_DONE_LABEL;
	}

	/* extract output paramters */
	BKNI_Memcpy( pParam->out.mspRegValue, pReceive->mspRegValue, 320 );

BHSM_P_DONE_LABEL:

	(void)BHSM_BspMsg_Destroy( hMsg );

	BDBG_LEAVE( BHSM_P_OtpMsp_MultiRead );

	return rc;
}


BERR_Code BHSM_P_OtpMsp_Prog( BHSM_Handle hHsm, BHSM_P_OtpMspProg *pParam )
{
	BERR_Code rc = BERR_SUCCESS;
	BHSM_BspMsg_h hMsg = NULL;
	uint16_t bspError = 0;
	BHSM_BspMsgCreate_t msgCreate;
	BHSM_BspMsgConfigure_t msgConfig;
	Bsp_OtpMsp_Prog_InFields_t *pSend = NULL;

	BDBG_ENTER( BHSM_P_OtpMsp_Prog );

	BKNI_Memset( &msgCreate, 0, sizeof(msgCreate) );
	BKNI_Memset( &msgConfig, 0, sizeof(msgConfig) );

	hMsg = BHSM_BspMsg_Create( hHsm, &msgCreate );
	if( !hMsg ) { return BERR_TRACE( BHSM_STATUS_RESOURCE_ALLOCATION_ERROR ); }

	pSend = (Bsp_OtpMsp_Prog_InFields_t*)msgCreate.pSend;

	msgConfig.component = Bsp_CmdComponent_eOtpMsp;
	msgConfig.command = Bsp_CmdOtpMsp_eProg;

	rc = BHSM_BspMsg_Configure( hMsg, &msgConfig );
	if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

	/* pack input paramters */
	pSend->cmdMspEnum = pParam->in.cmdMspEnum;
	pSend->numBits = pParam->in.numBits;
	pSend->mask = pParam->in.mask;
	pSend->data = pParam->in.data;

	rc = BHSM_BspMsg_SubmitCommand ( hMsg, &bspError );
	if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }

	if( bspError != 0 ) {
	    rc = BHSM_STATUS_BSP_ERROR;
	    BDBG_ERR(("%s BSP Status error [0x%X]", BSTD_FUNCTION, bspError ));
	    goto BHSM_P_DONE_LABEL;
	}


BHSM_P_DONE_LABEL:

	(void)BHSM_BspMsg_Destroy( hMsg );

	BDBG_LEAVE( BHSM_P_OtpMsp_Prog );

	return rc;
}
