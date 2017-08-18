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

#ifndef BSP_RV__H
#define BSP_RV__H

#ifdef __cplusplus
extern "C"
{
#endif

/* COMMANDS for the "Rv" component */
typedef enum
{
    Bsp_CmdRv_eRsaKeySet = 0x00,
    Bsp_CmdRv_eRsaKeyClear = 0x01,
    Bsp_CmdRv_eQueryRegion = 0x03,
    Bsp_CmdRv_eQueryAllRegions = 0x04,
    Bsp_CmdRv_eEnableRegion = 0x05,
    Bsp_CmdRv_eDisableRegion = 0x06,
    Bsp_CmdRv_eRedacted_0x07 = 0x07,
    Bsp_CmdRv_eHostItchDisable = 0x08,
    Bsp_CmdRv_eRedacted_0x09 = 0x09,
    Bsp_CmdRv_eRedacted_0x0A = 0x0A,
    Bsp_CmdRv_eRedacted_0x0B = 0x0B,
    Bsp_CmdRv_eMax
} Bsp_CmdRv_e;


/*PARAMATERS for the Bsp_CmdRv_eRsaKeySet command  */
/*input*/
typedef struct
{
    uint8_t  rsaKeyId; /* enum type Bsp_CmdRv_RsaKeyId_e */
    uint8_t  rootKeySrc; /* enum type Bsp_CmdRv_RsaRootKeyId_e */
    uint8_t  multiTierKey;
    uint8_t  rovf;
    uint32_t addressMsb;
    uint32_t address;

}Bsp_CmdRv_RsaKeySet_InFields_t;

/*PARAMATERS for the Bsp_CmdRv_eRsaKeyClear command  */
/*input*/
typedef struct
{
    uint8_t  rsaKeyId; /* enum type Bsp_CmdRv_RsaKeyId_e */
    uint8_t  reserved0[3];

}Bsp_CmdRv_RsaKeyClear_InFields_t;

/*PARAMATERS for the Bsp_CmdRv_eQueryRegion command  */
/*input*/
typedef struct
{
    uint8_t  regionId; /* enum type Bsp_CmdRv_RegionId_e */
    uint8_t  reserved0[3];

}Bsp_CmdRv_QueryRegion_InFields_t;
/*output*/
typedef struct
{
    uint32_t regionStatus; /* enum type Bsp_CmdRv_QueryStatusBits_e */
    uint8_t  bgChkCount;
    uint8_t  reserved1;
    uint8_t  itchId;
    uint8_t  wrchId;
    uint32_t startAddrMsb0;
    uint32_t startAddr0;
    uint32_t endAddrMsb0;
    uint32_t endAddr0;
    uint32_t sigStartAddrMsb;
    uint32_t sigStartAddr;
    uint32_t sigEndAddrMsb;
    uint32_t sigEndAddr;
    uint32_t startAddrMsb1;
    uint32_t startAddr1;
    uint32_t endAddrMsb1;
    uint32_t endAddr1;

}Bsp_CmdRv_QueryRegion_OutFields_t;


/*PARAMATERS for the Bsp_CmdRv_eQueryAllRegions command  */
/*input*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_CmdRv_QueryAllRegions_InFields_t;
/*output*/
typedef struct
{
    uint32_t regionStatus[31]; /* enum type Bsp_CmdRv_QueryStatusBits_e */

}Bsp_CmdRv_QueryAllRegions_OutFields_t;


/*PARAMATERS for the Bsp_CmdRv_eEnableRegion command  */
/*input*/
typedef struct
{
    uint8_t  regionId; /* enum type Bsp_CmdRv_RegionId_e */
    uint8_t  reserved0[3];
    uint32_t startAddrMsb;
    uint32_t startAddr;
    uint32_t endAddrMsb;
    uint32_t endAddr;
    uint32_t secondRangeStartAddrMsb;
    uint32_t secondRangeStartAddr;
    uint32_t secondRangeEndAddrMsb;
    uint32_t secondRangeEndAddr;
    uint32_t sigStartAddrMsb;
    uint32_t sigStartAddr;
    uint32_t sigEndAddrMsb;
    uint32_t sigEndAddr;
    uint8_t  intervalCheckBw;
    uint8_t  secondRangeAvailable;
    uint8_t  reserved3;
    uint8_t  resetOnVerifyFailure; /* enum type Bsp_CmdRv_ResetOnVerifyFailure_e */
    uint8_t  rsaKeyId; /* enum type Bsp_CmdRv_RsaKeyId_e */
    uint8_t  ssblDecryptionSel; /* enum type Bsp_CmdRv_SsblDecryptionMode_e */
    uint8_t  keyLayer; /* enum type Bsp_KeyLadderLayer_e */
    uint8_t  vklId; /* enum type Bsp_VklId_e */
    uint8_t  instrCheck; /* enum type Bsp_CmdRv_InstrCheck_e */
    uint8_t  bgCheck; /* enum type Bsp_CmdRv_BgCheck_e */
    uint8_t  allowRegionDisable; /* enum type Bsp_CmdRv_RegionDisable_e */
    uint8_t  enforceAuth; /* enum type Bsp_CmdRv_EnforceAuth_e */
    uint32_t sigParamsStartAddrMsb;
    uint32_t sigParamsStartAddr;
    uint32_t dstStartAddrMsb;
    uint32_t dstStartAddr;
    uint32_t secondDstStartAddrMsb;
    uint32_t secondDstStartAddr;
    uint32_t decryptClearKeyData[4];

}Bsp_CmdRv_EnableRegion_InFields_t;

/*PARAMATERS for the Bsp_CmdRv_eDisableRegion command  */
/*input*/
typedef struct
{
    uint8_t  regionId; /* enum type Bsp_CmdRv_RegionId_e */
    uint8_t  reserved0[3];

}Bsp_CmdRv_DisableRegion_InFields_t;

/*PARAMATERS for the Bsp_CmdRv_eHostItchDisable command  */
/*input*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_CmdRv_HostItchDisable_InFields_t;

#ifdef __cplusplus
}
#endif
#endif