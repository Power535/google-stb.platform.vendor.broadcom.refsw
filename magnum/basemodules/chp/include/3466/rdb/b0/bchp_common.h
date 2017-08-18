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
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Tue May 23 18:01:56 2017
 *                 Full Compile MD5 Checksum  faf7ad9783d64fd3b76af19c41ef463c
 *                     (minus title and desc)
 *                 MD5 Checksum               b7c66081f7ba94771fae7095d4e08762
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1459
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   LOCAL home/pntruong/sbin/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_COMMON_H__
#define BCHP_COMMON_H__

/***************************************************************************
 *BCM3466_B0 - BCM3466 B0 RBUS
 ***************************************************************************/
#define BCHP_PHYSICAL_OFFSET                               0x00000000
#define BCHP_REGISTER_START                                0x00000000 /* LEAP_ROM is first */
#define BCHP_REGISTER_END                                  0x07800040 /* MTAP_CONTROL is last */
#define BCHP_REGISTER_SIZE                                 0x01e00010 /* Number of registers */
#define BCHP_REGISTER_HAS_CORE_INFO                                 1 /* Has core info list */
#define BCHP_REGISTER_HAS_REGISTER_INFO                             1 /* Has register info list */
#define BCHP_REGISTER_HAS_16_BIT                                    0 /* Has 16-bit wide register */
#define BCHP_REGISTER_HAS_32_BIT                                    1 /* Has 32-bit wide register */
#define BCHP_REGISTER_HAS_64_BIT                                    0 /* Has 64-bit wide register */
#define BCHP_UINT64_C(hi, lo)             (((uint64_t)hi)<<32 | (lo)) /* C89 64-bit literal */

/****************************************************************************
 * Core instance register start address.
 ***************************************************************************/
#define BCHP_LEAP_ROM_REG_START                            0x00000000
#define BCHP_LEAP_ROM_REG_END                              0x00008ffc
#define BCHP_LEAP_PROG0_MEM_REG_START                      0x00020000
#define BCHP_LEAP_PROG0_MEM_REG_END                        0x0005fffc
#define BCHP_LEAP_PROG2_MEM_REG_START                      0x00060000
#define BCHP_LEAP_PROG2_MEM_REG_END                        0x0006fffc
#define BCHP_LEAP_PROG4_MEM_REG_START                      0x00070000
#define BCHP_LEAP_PROG4_MEM_REG_END                        0x00077ffc
#define BCHP_LEAP_DATA0_MEM_REG_START                      0x00080000
#define BCHP_LEAP_DATA0_MEM_REG_END                        0x000afffc
#define BCHP_LEAP_CPU_CORE_REGS_REG_START                  0x000d0000
#define BCHP_LEAP_CPU_CORE_REGS_REG_END                    0x000d00fc
#define BCHP_LEAP_CPU_AUX_REGS_REG_START                   0x000e0000
#define BCHP_LEAP_CPU_AUX_REGS_REG_END                     0x000e10c4
#define BCHP_LEAP_HAB_MEM_REG_START                        0x000e8000
#define BCHP_LEAP_HAB_MEM_REG_END                          0x000e83fc
#define BCHP_LEAP_UART_REG_START                           0x000e9000
#define BCHP_LEAP_UART_REG_END                             0x000e9ffc
#define BCHP_LEAP_WDG_REG_START                            0x000ea000
#define BCHP_LEAP_WDG_REG_END                              0x000eaffc
#define BCHP_LEAP_CTRL_REG_START                           0x00100000
#define BCHP_LEAP_CTRL_REG_END                             0x0010035c
#define BCHP_LEAP_L1_REG_START                             0x00100400
#define BCHP_LEAP_L1_REG_END                               0x00100430
#define BCHP_LEAP_L2_0_REG_START                           0x00100500
#define BCHP_LEAP_L2_0_REG_END                             0x00100514
#define BCHP_LEAP_L2_1_REG_START                           0x00100600
#define BCHP_LEAP_L2_1_REG_END                             0x00100614
#define BCHP_LEAP_HOST_L1_REG_START                        0x00100700
#define BCHP_LEAP_HOST_L1_REG_END                          0x00100730
#define BCHP_LEAP_HOST_L2_0_REG_START                      0x00100800
#define BCHP_LEAP_HOST_L2_0_REG_END                        0x00100814
#define BCHP_LEAP_HOST_L2_1_REG_START                      0x00100900
#define BCHP_LEAP_HOST_L2_1_REG_END                        0x00100914
#define BCHP_LEAP_CTRL_MISC_REG_START                      0x00100a00
#define BCHP_LEAP_CTRL_MISC_REG_END                        0x00100a4c
#define BCHP_LEAP_ROM_PATCH_REG_START                      0x00101000
#define BCHP_LEAP_ROM_PATCH_REG_END                        0x0010103c
#define BCHP_LEAP_CTRL_TIMERS_REG_START                    0x00101200
#define BCHP_LEAP_CTRL_TIMERS_REG_END                      0x0010127c
#define BCHP_UFE_AFE_0_REG_START                           0x04000000
#define BCHP_UFE_AFE_0_REG_END                             0x04000094
#define BCHP_SDADC_0_REG_START                             0x04000200
#define BCHP_SDADC_0_REG_END                               0x04000210
#define BCHP_UFE_MISC_0_REG_START                          0x04000600
#define BCHP_UFE_MISC_0_REG_END                            0x040006dc
#define BCHP_UFE_GR_BRIDGE_0_REG_START                     0x04000700
#define BCHP_UFE_GR_BRIDGE_0_REG_END                       0x0400070c
#define BCHP_UFE_MISC2_0_REG_START                         0x04000780
#define BCHP_UFE_MISC2_0_REG_END                           0x040007ec
#define BCHP_UFE_0_REG_START                               0x04000800
#define BCHP_UFE_0_REG_END                                 0x04000868
#define BCHP_UFE_SAW_0_REG_START                           0x04000900
#define BCHP_UFE_SAW_0_REG_END                             0x04000980
#define BCHP_UFE_AFE_1_REG_START                           0x04100000
#define BCHP_UFE_AFE_1_REG_END                             0x04100094
#define BCHP_SDADC_1_REG_START                             0x04100200
#define BCHP_SDADC_1_REG_END                               0x04100210
#define BCHP_UFE_MISC_1_REG_START                          0x04100600
#define BCHP_UFE_MISC_1_REG_END                            0x041006dc
#define BCHP_UFE_GR_BRIDGE_1_REG_START                     0x04100700
#define BCHP_UFE_GR_BRIDGE_1_REG_END                       0x0410070c
#define BCHP_UFE_MISC2_1_REG_START                         0x04100780
#define BCHP_UFE_MISC2_1_REG_END                           0x041007ec
#define BCHP_UFE_1_REG_START                               0x04100800
#define BCHP_UFE_1_REG_END                                 0x04100868
#define BCHP_UFE_SAW_1_REG_START                           0x04100900
#define BCHP_UFE_SAW_1_REG_END                             0x04100980
#define BCHP_DS_TOPM_0_REG_START                           0x04202000
#define BCHP_DS_TOPM_0_REG_END                             0x04202068
#define BCHP_DS_TOPS_0_REG_START                           0x04203000
#define BCHP_DS_TOPS_0_REG_END                             0x0420309c
#define BCHP_DS_0_REG_START                                0x04204000
#define BCHP_DS_0_REG_END                                  0x0420509c
#define BCHP_DS_TOPM_1_REG_START                           0x04302000
#define BCHP_DS_TOPM_1_REG_END                             0x04302068
#define BCHP_DS_TOPS_1_REG_START                           0x04303000
#define BCHP_DS_TOPS_1_REG_END                             0x0430309c
#define BCHP_DS_1_REG_START                                0x04304000
#define BCHP_DS_1_REG_END                                  0x0430509c
#define BCHP_T2_BICM_SYS_0_REG_START                       0x04400000
#define BCHP_T2_BICM_SYS_0_REG_END                         0x0440001c
#define BCHP_T2_BICM_INTR2_0_0_REG_START                   0x04400200
#define BCHP_T2_BICM_INTR2_0_0_REG_END                     0x0440022c
#define BCHP_T2_BICM_INTR2_1_0_REG_START                   0x04400300
#define BCHP_T2_BICM_INTR2_1_0_REG_END                     0x0440032c
#define BCHP_T2_BICM_CORE_0_REG_START                      0x04400400
#define BCHP_T2_BICM_CORE_0_REG_END                        0x0440068c
#define BCHP_T2_BICM_SYS_1_REG_START                       0x04500000
#define BCHP_T2_BICM_SYS_1_REG_END                         0x0450001c
#define BCHP_T2_BICM_INTR2_0_1_REG_START                   0x04500200
#define BCHP_T2_BICM_INTR2_0_1_REG_END                     0x0450022c
#define BCHP_T2_BICM_INTR2_1_1_REG_START                   0x04500300
#define BCHP_T2_BICM_INTR2_1_1_REG_END                     0x0450032c
#define BCHP_T2_BICM_CORE_1_REG_START                      0x04500400
#define BCHP_T2_BICM_CORE_1_REG_END                        0x0450068c
#define BCHP_ODS_INTR_0_REG_START                          0x04600000
#define BCHP_ODS_INTR_0_REG_END                            0x0460002c
#define BCHP_ODS_INTR1_0_REG_START                         0x04600200
#define BCHP_ODS_INTR1_0_REG_END                           0x0460022c
#define BCHP_ODS_GLB_0_REG_START                           0x04600400
#define BCHP_ODS_GLB_0_REG_END                             0x046004ec
#define BCHP_ODS_FE_0_REG_START                            0x04600600
#define BCHP_ODS_FE_0_REG_END                              0x0460069c
#define BCHP_ODS_OFDM_0_REG_START                          0x04600800
#define BCHP_ODS_OFDM_0_REG_END                            0x04600c94
#define BCHP_ODS_FEC_0_REG_START                           0x04601000
#define BCHP_ODS_FEC_0_REG_END                             0x046010d0
#define BCHP_ODS_OI_0_REG_START                            0x04601200
#define BCHP_ODS_OI_0_REG_END                              0x046013b8
#define BCHP_ODS_INTR_1_REG_START                          0x04700000
#define BCHP_ODS_INTR_1_REG_END                            0x0470002c
#define BCHP_ODS_INTR1_1_REG_START                         0x04700200
#define BCHP_ODS_INTR1_1_REG_END                           0x0470022c
#define BCHP_ODS_GLB_1_REG_START                           0x04700400
#define BCHP_ODS_GLB_1_REG_END                             0x047004ec
#define BCHP_ODS_FE_1_REG_START                            0x04700600
#define BCHP_ODS_FE_1_REG_END                              0x0470069c
#define BCHP_ODS_OFDM_1_REG_START                          0x04700800
#define BCHP_ODS_OFDM_1_REG_END                            0x04700c94
#define BCHP_ODS_FEC_1_REG_START                           0x04701000
#define BCHP_ODS_FEC_1_REG_END                             0x047010d0
#define BCHP_ODS_OI_1_REG_START                            0x04701200
#define BCHP_ODS_OI_1_REG_END                              0x047013b8
#define BCHP_AVS_HOST_L2_REG_START                         0x04811200
#define BCHP_AVS_HOST_L2_REG_END                           0x04811244
#define BCHP_AVS_CPU_CTRL_REG_START                        0x04811300
#define BCHP_AVS_CPU_CTRL_REG_END                          0x0481135c
#define BCHP_AVS_BSTI_REG_START                            0x04811400
#define BCHP_AVS_BSTI_REG_END                              0x04811404
#define BCHP_AVS_TMON_REG_START                            0x04811500
#define BCHP_AVS_TMON_REG_END                              0x04811524
#define BCHP_AVS_TOP_CTRL_REG_START                        0x04811800
#define BCHP_AVS_TOP_CTRL_REG_END                          0x04811854
#define BCHP_AVS_HW_MNTR_REG_START                         0x04812000
#define BCHP_AVS_HW_MNTR_REG_END                           0x048120c8
#define BCHP_AVS_PVT_MNTR_CONFIG_REG_START                 0x04812100
#define BCHP_AVS_PVT_MNTR_CONFIG_REG_END                   0x04812124
#define BCHP_AVS_RO_REGISTERS_0_REG_START                  0x04812200
#define BCHP_AVS_RO_REGISTERS_0_REG_END                    0x048122e0
#define BCHP_AVS_RO_REGISTERS_1_REG_START                  0x04812800
#define BCHP_AVS_RO_REGISTERS_1_REG_END                    0x04812810
#define BCHP_AVS_ROSC_THRESHOLD_1_REG_START                0x04812d00
#define BCHP_AVS_ROSC_THRESHOLD_1_REG_END                  0x04812dfc
#define BCHP_AVS_ROSC_THRESHOLD_2_REG_START                0x04812e00
#define BCHP_AVS_ROSC_THRESHOLD_2_REG_END                  0x04812efc
#define BCHP_AVS_PMB_S_000_REG_START                       0x04818000
#define BCHP_AVS_PMB_S_000_REG_END                         0x04818024
#define BCHP_AVS_PMB_S_001_REG_START                       0x04818040
#define BCHP_AVS_PMB_S_001_REG_END                         0x04818064
#define BCHP_AVS_PMB_S_002_REG_START                       0x04818080
#define BCHP_AVS_PMB_S_002_REG_END                         0x048180a4
#define BCHP_AVS_PMB_S_003_REG_START                       0x048180c0
#define BCHP_AVS_PMB_S_003_REG_END                         0x048180e4
#define BCHP_AVS_PMB_S_004_REG_START                       0x04818100
#define BCHP_AVS_PMB_S_004_REG_END                         0x04818124
#define BCHP_AVS_PMB_S_005_REG_START                       0x04818140
#define BCHP_AVS_PMB_S_005_REG_END                         0x04818164
#define BCHP_AVS_PMB_S_006_REG_START                       0x04818180
#define BCHP_AVS_PMB_S_006_REG_END                         0x048181a4
#define BCHP_AVS_PMB_S_007_REG_START                       0x048181c0
#define BCHP_AVS_PMB_S_007_REG_END                         0x048181e4
#define BCHP_AVS_PMB_S_008_REG_START                       0x04818200
#define BCHP_AVS_PMB_S_008_REG_END                         0x04818224
#define BCHP_AVS_PMB_S_009_REG_START                       0x04818240
#define BCHP_AVS_PMB_S_009_REG_END                         0x04818264
#define BCHP_AVS_PMB_REGISTERS_REG_START                   0x0481a000
#define BCHP_AVS_PMB_REGISTERS_REG_END                     0x0481a008
#define BCHP_TOP_CTRL_REG_START                            0x04820000
#define BCHP_TOP_CTRL_REG_END                              0x04820210
#define BCHP_TM_REG_START                                  0x04820400
#define BCHP_TM_REG_END                                    0x04820730
#define BCHP_OCTOPUS_IRQ_REG_START                         0x04820c00
#define BCHP_OCTOPUS_IRQ_REG_END                           0x04820c14
#define BCHP_JTAG_OTP_REG_START                            0x04821000
#define BCHP_JTAG_OTP_REG_END                              0x0482105c
#define BCHP_BSCA_REG_START                                0x04821800
#define BCHP_BSCA_REG_END                                  0x04821854
#define BCHP_BSCB_REG_START                                0x04821880
#define BCHP_BSCB_REG_END                                  0x048218d4
#define BCHP_BSCC_REG_START                                0x04821900
#define BCHP_BSCC_REG_END                                  0x04821954
#define BCHP_TIMER_REG_START                               0x04821980
#define BCHP_TIMER_REG_END                                 0x048219bc
#define BCHP_GIO_REG_START                                 0x04821a00
#define BCHP_GIO_REG_END                                   0x04821a1c
#define BCHP_DIAG_CAPT_REG_START                           0x04821c00
#define BCHP_DIAG_CAPT_REG_END                             0x04821c5c
#define BCHP_CLKGEN_REG_START                              0x04828000
#define BCHP_CLKGEN_REG_END                                0x04828154
#define BCHP_DEMOD_XPT_MTSIF_TX0_IO_REG_START              0x04b00200
#define BCHP_DEMOD_XPT_MTSIF_TX0_IO_REG_END                0x04b00204
#define BCHP_DEMOD_XPT_MTSIF_TX1_IO_REG_START              0x04b00300
#define BCHP_DEMOD_XPT_MTSIF_TX1_IO_REG_END                0x04b00304
#define BCHP_DEMOD_XPT_WAKEUP_REG_START                    0x04b01000
#define BCHP_DEMOD_XPT_WAKEUP_REG_END                      0x04b01fbc
#define BCHP_DEMOD_XPT_FE_REG_START                        0x04b02000
#define BCHP_DEMOD_XPT_FE_REG_END                          0x04b037fc
#define BCHP_MTAP_CONTROL_REG_START                        0x07800000
#define BCHP_MTAP_CONTROL_REG_END                          0x0780003c


#endif /* #ifndef BCHP_COMMON_H__ */

/* End of File */
