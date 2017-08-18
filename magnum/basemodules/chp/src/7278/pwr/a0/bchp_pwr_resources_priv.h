 /******************************************************************************
 *  Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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

/***************************************************************************
*
* This file is auto-generated
*
* This file contains a list of private power resource IDs that
* represent HW clocks, and function prototypes for controlling
* them.
*
***************************************************************************/

#ifndef BCHP_PWR_RESOURCES_PRIV_H__
#define BCHP_PWR_RESOURCES_PRIV_H__

#include "bchp_pwr.h"

/* Private power resource IDs */
#define BCHP_PWR_HW_AIO                                           0xff000001
#define BCHP_PWR_HW_AIO_SRAM                                      0xff000002
#define BCHP_PWR_HW_AUD_PLL0                                      0xff000003
#define BCHP_PWR_HW_AUD_PLL1                                      0xff000004
#define BCHP_PWR_HW_BVN                                           0xff000005
#define BCHP_PWR_HW_BVN_DVPHR0_DVPHT0_VEC                         0xff000006
#define BCHP_PWR_HW_BVN_SRAM                                      0xff000007
#define BCHP_PWR_HW_DVPHR0                                        0xff000008
#define BCHP_PWR_HW_DVPHR0_SRAM                                   0xff000009
#define BCHP_PWR_HW_DVPHT0                                        0xff00000a
#define BCHP_PWR_HW_HDMI_RX0_PHY                                  0xff00000b
#define BCHP_PWR_HW_HDMI_TX0_PHY                                  0xff00000c
#define BCHP_PWR_HW_HVD0                                          0xff00000d
#define BCHP_PWR_HW_HVD0_SECBUS                                   0xff00000e
#define BCHP_PWR_HW_HVD0_SID                                      0xff00000f
#define BCHP_PWR_HW_HVD0_SRAM                                     0xff000010
#define BCHP_PWR_HW_HVD1                                          0xff000011
#define BCHP_PWR_HW_HVD1_SECBUS                                   0xff000012
#define BCHP_PWR_HW_HVD1_SRAM                                     0xff000013
#define BCHP_PWR_HW_HVDP0_HVDP0_CORE                              0xff000014
#define BCHP_PWR_HW_HVDP0_HVDP0_CPU                               0xff000015
#define BCHP_PWR_HW_HVDP0_SRAM                                    0xff000016
#define BCHP_PWR_HW_HVDP1_HVDP1_CORE                              0xff000017
#define BCHP_PWR_HW_HVDP1_HVDP1_CPU                               0xff000018
#define BCHP_PWR_HW_HVDP1_SRAM                                    0xff000019
#define BCHP_PWR_HW_HVDS0_HVDS0_CORE                              0xff00001a
#define BCHP_PWR_HW_HVDS0_HVDS0_CPU                               0xff00001b
#define BCHP_PWR_HW_HVDS0_SRAM                                    0xff00001c
#define BCHP_PWR_HW_HVDS1_HVDS1_CORE                              0xff00001d
#define BCHP_PWR_HW_HVDS1_HVDS1_CPU                               0xff00001e
#define BCHP_PWR_HW_HVDS1_SRAM                                    0xff00001f
#define BCHP_PWR_HW_HVD_CH_CTRL_CH_0_POST_DIV_HOLD_CH0            0xff000020
#define BCHP_PWR_HW_HVD_CH_CTRL_CH_1_POST_DIV_HOLD_CH1            0xff000021
#define BCHP_PWR_HW_HVD_CH_CTRL_CH_2_POST_DIV_HOLD_CH2            0xff000022
#define BCHP_PWR_HW_HVD_CH_CTRL_CH_5_POST_DIV_HOLD_CH5            0xff000023
#define BCHP_PWR_HW_M2MC0                                         0xff000024
#define BCHP_PWR_HW_M2MC0_SRAM                                    0xff000025
#define BCHP_PWR_HW_M2MC1                                         0xff000026
#define BCHP_PWR_HW_MMM2MC0_SRAM                                  0xff000027
#define BCHP_PWR_HW_NETWORK_CH_CTRL_CH_5_POST_DIV_HOLD_CH5        0xff000028
#define BCHP_PWR_HW_RAAGA0                                        0xff000029
#define BCHP_PWR_HW_RAAGA0_AIO                                    0xff00002a
#define BCHP_PWR_HW_RAAGA0_SRAM                                   0xff00002b
#define BCHP_PWR_HW_RAAGA_DSP_0_AIO_RAAGA0_DSP_AIO_RAAGA0         0xff00002c
#define BCHP_PWR_HW_RAAGA_DSP_0_RAAGA0_DSP_RAAGA0                 0xff00002d
#define BCHP_PWR_HW_SC0_CH_CTRL_CH_0_POST_DIV_HOLD_CH0            0xff00002e
#define BCHP_PWR_HW_SC0_CTRL_WRAPPER_CONTROL_PWRDN_REQ            0xff00002f
#define BCHP_PWR_HW_SC1_CH_CTRL_CH_0_POST_DIV_HOLD_CH0            0xff000030
#define BCHP_PWR_HW_SC1_CTRL_WRAPPER_CONTROL_PWRDN_REQ            0xff000031
#define BCHP_PWR_HW_SID                                           0xff000032
#define BCHP_PWR_HW_SYS_CTRL_SC0                                  0xff000033
#define BCHP_PWR_HW_SYS_CTRL_SC1                                  0xff000034
#define BCHP_PWR_HW_V3D                                           0xff000035
#define BCHP_PWR_HW_V3D_SRAM                                      0xff000036
#define BCHP_PWR_HW_V3D_V3D                                       0xff000037
#define BCHP_PWR_HW_VCXO0_CH_CTRL_CH_0_POST_DIV_HOLD_CH0          0xff000038
#define BCHP_PWR_HW_VCXO0_CH_CTRL_CH_1_POST_DIV_HOLD_CH1          0xff000039
#define BCHP_PWR_HW_VCXO0_CTRL_WRAPPER_CONTROL_PWRDN_REQ          0xff00003a
#define BCHP_PWR_HW_VCXO1_CH_CTRL_CH_0_POST_DIV_HOLD_CH0          0xff00003b
#define BCHP_PWR_HW_VCXO1_CH_CTRL_CH_1_POST_DIV_HOLD_CH1          0xff00003c
#define BCHP_PWR_HW_VCXO1_CTRL_WRAPPER_CONTROL_PWRDN_REQ          0xff00003d
#define BCHP_PWR_HW_VDAC                                          0xff00003e
#define BCHP_PWR_HW_VEC                                           0xff00003f
#define BCHP_PWR_HW_VEC_AIO                                       0xff000040
#define BCHP_PWR_HW_VEC_AIO_GFX_M2MC0_GFX_M2MC0_M2MC0             0xff000041
#define BCHP_PWR_HW_VEC_AIO_GFX_MM_M2MC0_GFX_MM_M2MC0_MM_M2MC0    0xff000042
#define BCHP_PWR_HW_VEC_AIO_GFX_VEC_ITU656_0                      0xff000043
#define BCHP_PWR_HW_VEC_ITU656                                    0xff000044
#define BCHP_PWR_HW_VEC_SRAM                                      0xff000045
#define BCHP_PWR_HW_VICE20                                        0xff000046
#define BCHP_PWR_HW_VICE20_SECBUS                                 0xff000047
#define BCHP_PWR_HW_XPT_CH_CTRL_CH_1_POST_DIV_HOLD_CH1            0xff000048
#define BCHP_PWR_HW_XPT_CH_CTRL_CH_2_POST_DIV_HOLD_CH2            0xff000049
#define BCHP_PWR_HW_XPT_CH_CTRL_CH_4_POST_DIV_HOLD_CH4            0xff00004a
#define BCHP_PWR_HW_XPT_CH_CTRL_CH_5_POST_DIV_HOLD_CH5            0xff00004b
#define BCHP_PWR_HW_XPT_REMUX                                     0xff00004c
#define BCHP_PWR_HW_XPT_SECBUS_XPT_REMUX                          0xff00004d
#define BCHP_PWR_HW_XPT_SRAM                                      0xff00004e
#define BCHP_PWR_HW_XPT_XPT_REMUX                                 0xff00004f
#define BCHP_PWR_MX_ITU656_0_MUX_SELECT_VEC_ITU656_0_CLOCK        0xff000050
#define BCHP_PWR_MX_RAAGA_DSP_0_RAAGA0_DSP_SELECT_RAAGA0          0xff000051
#define BCHP_PWR_MX_SMARTCARD_MUX_SELECT_SC0_CLOCK                0xff000052
#define BCHP_PWR_MX_SMARTCARD_MUX_SELECT_SC1_CLOCK                0xff000053
#define BCHP_PWR_DV_HVD_CTRL_CH0_div                              0xff000054
#define BCHP_PWR_DV_HVD_CTRL_CH1_div                              0xff000055
#define BCHP_PWR_DV_HVD_CTRL_CH2_div                              0xff000056
#define BCHP_PWR_DV_HVD_CTRL_CH5_div                              0xff000057
#define BCHP_PWR_DV_NETWORK_CTRL_CH5_div                          0xff000058
#define BCHP_PWR_DV_XPT_CTRL_CH4_div                              0xff000059

/* This is the link between the public and private interface */
void BCHP_PWR_P_HW_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, bool activate);
void BCHP_PWR_P_HW_ControlId(BCHP_Handle handle, unsigned id, bool activate);
void BCHP_PWR_P_MUX_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, unsigned *mux, bool set);
void BCHP_PWR_P_DIV_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, unsigned *mult, unsigned *prediv, unsigned *postdiv, bool set);

#define BCHP_PWR_P_NUM_NONLEAFS     59
#define BCHP_PWR_P_NUM_NONLEAFSHW   32
#define BCHP_PWR_P_NUM_LEAFS        47
#define BCHP_PWR_P_NUM_MUXES        4
#define BCHP_PWR_P_NUM_DIVS         6
#define BCHP_PWR_P_NUM_ALLNODES     148
#define BCHP_PWR_NUM_P_PMAPSETTINGS 6

#endif
