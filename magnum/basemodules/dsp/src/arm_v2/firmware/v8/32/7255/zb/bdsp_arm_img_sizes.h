/*******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/


/****Kernel specific parameters for the host ****/

/****Size of RDB vars ****/
#define BDSP_ARM_IMG_SYSTEM_RDBVARS_SIZE                               ((uint32_t)1024)

/**** Size of RDB mapped space ****/
#define BDSP_ARM_IMG_SYSTEM_IO_SIZE                                   ((uint32_t)8192)

/**** Size of Shared memory between MP and AP ****/
#define BDSP_ARM_IMG_MP_AP_SHARED_MEMORY_SIZE                          ((uint32_t)4096)

/**** Size of Shared memory between MP and AP for Soft FMM****/
#define BDSP_ARM_IMG_SOFT_FMM_MP_AP_SHARED_MEMORY_SIZE                 ((uint32_t)8192)

/****Details of Kernel and ROMFS for the host ****/

/****Codec specific parameters for the host ****/


/*COMMON Size requirement*/
#define BDSP_ARM_IMG_COMMON_SIZE                                     ((uint32_t)55808)

/*DEBUG_CONTROL Size requirement*/
#define BDSP_ARM_IMG_DEBUG_CONTROL_SIZE                              ((uint32_t)22016)

/*MM_CLIENT Size requirement*/
#define BDSP_ARM_IMG_MM_CLIENT_SIZE                                  ((uint32_t)96768)

/*KERNEL_OPS Size requirement*/
#define BDSP_ARM_IMG_KERNEL_OPS_SIZE                                 ((uint32_t)77312)

/*STAGE_IO Size requirement*/
#define BDSP_ARM_IMG_STAGE_IO_SIZE                                   ((uint32_t)201216)

/*COMMON_IDS Size requirement*/
#define BDSP_ARM_IMG_COMMON_IDS_SIZE                                 ((uint32_t)233472)

/*INIT_PROCESS Size requirement*/
#define BDSP_ARM_IMG_INIT_PROCESS_SIZE                               ((uint32_t)30720)

/*SCHEDULING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_SCHEDULING_PROCESS_SIZE                         ((uint32_t)143872)

/*ROUTING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_ROUTING_PROCESS_SIZE                            ((uint32_t)65024)

/*MESSAGING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_MESSAGING_PROCESS_SIZE                          ((uint32_t)130560)

/*ALGO_PROCESS Size requirement*/
#define BDSP_ARM_IMG_ALGO_PROCESS_SIZE                               ((uint32_t)174592)

/*IDLE_PROCESS Size requirement*/
#define BDSP_ARM_IMG_IDLE_PROCESS_SIZE                               ((uint32_t)13824)

/*MM_SERVER Size requirement*/
#define BDSP_ARM_IMG_MM_SERVER_SIZE                                  ((uint32_t)150016)

/*AIDS_WAVFORMATEX Size requirement*/
#define BDSP_ARM_IMG_AIDS_WAVFORMATEX_SIZE                           ((uint32_t)41472)

/*AIDS_MPEG1 Size requirement*/
#define BDSP_ARM_IMG_AIDS_MPEG1_SIZE                                 ((uint32_t)25600)

/*AIDS_ADTS Size requirement*/
#define BDSP_ARM_IMG_AIDS_ADTS_SIZE                                  ((uint32_t)19456)

/*AIDS_LOAS Size requirement*/
#define BDSP_ARM_IMG_AIDS_LOAS_SIZE                                  ((uint32_t)30208)

/*ADEC_PCMWAV Size requirement*/
#define BDSP_ARM_IMG_ADEC_PCMWAV_SCRATCH_SIZE                        ((uint32_t)32768)
#define BDSP_ARM_IMG_ADEC_PCMWAV_SIZE                                ((uint32_t)39936)
#define BDSP_ARM_IMG_ADEC_PCMWAV_INTER_FRAME_SIZE                    ((uint32_t)5120)
#define BDSP_ARM_IMG_ADEC_PCMWAV_INTER_FRAME_ENCODED_SIZE            ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_PCMWAV_TABLES_SIZE                         ((uint32_t)5120)

/*ADEC_MPEG1 Size requirement*/
#define BDSP_ARM_IMG_ADEC_MPEG1_SCRATCH_SIZE                         ((uint32_t)36864)
#define BDSP_ARM_IMG_ADEC_MPEG1_SIZE                                 ((uint32_t)132608)
#define BDSP_ARM_IMG_ADEC_MPEG1_INTER_FRAME_SIZE                     ((uint32_t)13312)
#define BDSP_ARM_IMG_ADEC_MPEG1_INTER_FRAME_ENCODED_SIZE             ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MPEG1_TABLES_SIZE                          ((uint32_t)17408)

/*ADEC_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_AACHE_SCRATCH_SIZE                         ((uint32_t)253952)
#define BDSP_ARM_IMG_ADEC_AACHE_SIZE                                 ((uint32_t)435712)
#define BDSP_ARM_IMG_ADEC_AACHE_INTER_FRAME_SIZE                     ((uint32_t)189440)
#define BDSP_ARM_IMG_ADEC_AACHE_INTER_FRAME_ENCODED_SIZE             ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_AACHE_TABLES_SIZE                          ((uint32_t)74752)

/*ADEC_PASSTHRU Size requirement*/
#define BDSP_ARM_IMG_ADEC_PASSTHRU_SCRATCH_SIZE                      ((uint32_t)40960)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_SIZE                              ((uint32_t)68608)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_INTER_FRAME_SIZE                  ((uint32_t)5120)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_INTER_FRAME_ENCODED_SIZE          ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_TABLES_SIZE                       ((uint32_t)5120)

/*AIDS_DDP Size requirement*/
#define BDSP_ARM_IMG_AIDS_DDP_SIZE                                   ((uint32_t)27648)

/*ADEC_DDP Size requirement*/
#define BDSP_ARM_IMG_ADEC_DDP_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_DDP_SIZE                                   ((uint32_t)728064)
#define BDSP_ARM_IMG_ADEC_DDP_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_DDP_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_DDP_TABLES_SIZE                            ((uint32_t)205824)

/*ADEC_AC3 Size requirement*/
#define BDSP_ARM_IMG_ADEC_AC3_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_AC3_SIZE                                   ((uint32_t)728064)
#define BDSP_ARM_IMG_ADEC_AC3_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_AC3_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_AC3_TABLES_SIZE                            ((uint32_t)205824)

/*ADEC_UDC Size requirement*/
#define BDSP_ARM_IMG_ADEC_UDC_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_UDC_SIZE                                   ((uint32_t)728064)
#define BDSP_ARM_IMG_ADEC_UDC_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_UDC_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_UDC_TABLES_SIZE                            ((uint32_t)205824)

/*APP_DPCMR Size requirement*/
#define BDSP_ARM_IMG_APP_DPCMR_SCRATCH_SIZE                          ((uint32_t)45056)
#define BDSP_ARM_IMG_APP_DPCMR_SIZE                                  ((uint32_t)118272)
#define BDSP_ARM_IMG_APP_DPCMR_INTER_FRAME_SIZE                      ((uint32_t)177152)
#define BDSP_ARM_IMG_APP_DPCMR_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_APP_DPCMR_TABLES_SIZE                           ((uint32_t)5120)

/*APP_MIXER_DAPV2 Size requirement*/
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_SCRATCH_SIZE                    ((uint32_t)1290240)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_SIZE                            ((uint32_t)104448)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_INTER_FRAME_SIZE                ((uint32_t)332800)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_INTER_FRAME_ENCODED_SIZE        ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_TABLES_SIZE                     ((uint32_t)70656)

/*ADEC_DOLBY_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_SCRATCH_SIZE                   ((uint32_t)528384)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_SIZE                           ((uint32_t)620032)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_INTER_FRAME_SIZE               ((uint32_t)525312)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_INTER_FRAME_ENCODED_SIZE       ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_TABLES_SIZE                    ((uint32_t)160768)

/*AENC_DDP Size requirement*/
#define BDSP_ARM_IMG_AENC_DDP_SCRATCH_SIZE                           ((uint32_t)81920)
#define BDSP_ARM_IMG_AENC_DDP_SIZE                                   ((uint32_t)775680)
#define BDSP_ARM_IMG_AENC_DDP_INTER_FRAME_SIZE                       ((uint32_t)222208)
#define BDSP_ARM_IMG_AENC_DDP_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_AENC_DDP_TABLES_SIZE                            ((uint32_t)29696)

/*ADEC_MS11PLUS_UDC Size requirement*/
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_SCRATCH_SIZE                  ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_SIZE                          ((uint32_t)727552)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_INTER_FRAME_SIZE              ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_INTER_FRAME_ENCODED_SIZE      ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_TABLES_SIZE                   ((uint32_t)205824)

/*APP_MS11PLUS_DPCMR Size requirement*/
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_SCRATCH_SIZE                 ((uint32_t)45056)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_SIZE                         ((uint32_t)118272)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_INTER_FRAME_SIZE             ((uint32_t)177152)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_INTER_FRAME_ENCODED_SIZE     ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_TABLES_SIZE                  ((uint32_t)5120)

/*APP_MS11PLUS_MIXER Size requirement*/
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_SCRATCH_SIZE                 ((uint32_t)1290240)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_SIZE                         ((uint32_t)104448)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_INTER_FRAME_SIZE             ((uint32_t)332800)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_INTER_FRAME_ENCODED_SIZE     ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_TABLES_SIZE                  ((uint32_t)70656)

/*ADEC_MS11PLUS_DOLBY_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_SCRATCH_SIZE          ((uint32_t)528384)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_SIZE                  ((uint32_t)620032)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_INTER_FRAME_SIZE      ((uint32_t)525312)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_INTER_FRAME_ENCODED_SIZE ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_TABLES_SIZE           ((uint32_t)160768)

/*AENC_MS11PLUS_DD Size requirement*/
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_SCRATCH_SIZE                   ((uint32_t)81920)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_SIZE                           ((uint32_t)775680)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_INTER_FRAME_SIZE               ((uint32_t)222208)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_INTER_FRAME_ENCODED_SIZE       ((uint32_t)512)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_TABLES_SIZE                    ((uint32_t)29696)

/*APP_SRC Size requirement*/
#define BDSP_ARM_IMG_APP_SRC_SCRATCH_SIZE                            ((uint32_t)49152)
#define BDSP_ARM_IMG_APP_SRC_SIZE                                    ((uint32_t)39936)
#define BDSP_ARM_IMG_APP_SRC_INTER_FRAME_SIZE                        ((uint32_t)17408)
#define BDSP_ARM_IMG_APP_SRC_INTER_FRAME_ENCODED_SIZE                ((uint32_t)512)
#define BDSP_ARM_IMG_APP_SRC_TABLES_SIZE                             ((uint32_t)5120)

/*APP_FW_MIXER Size requirement*/
#define BDSP_ARM_IMG_APP_FW_MIXER_SCRATCH_SIZE                       ((uint32_t)102400)
#define BDSP_ARM_IMG_APP_FW_MIXER_SIZE                               ((uint32_t)131072)
#define BDSP_ARM_IMG_APP_FW_MIXER_INTER_FRAME_SIZE                   ((uint32_t)33792)
#define BDSP_ARM_IMG_APP_FW_MIXER_INTER_FRAME_ENCODED_SIZE           ((uint32_t)512)
#define BDSP_ARM_IMG_APP_FW_MIXER_TABLES_SIZE                        ((uint32_t)9216)

/*APP_DSOLA Size requirement*/
#define BDSP_ARM_IMG_APP_DSOLA_SCRATCH_SIZE                          ((uint32_t)8192)
#define BDSP_ARM_IMG_APP_DSOLA_SIZE                                  ((uint32_t)31232)
#define BDSP_ARM_IMG_APP_DSOLA_INTER_FRAME_SIZE                      ((uint32_t)78848)
#define BDSP_ARM_IMG_APP_DSOLA_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_APP_DSOLA_TABLES_SIZE                           ((uint32_t)5120)

/*ADEC_OPUS Size requirement*/
#define BDSP_ARM_IMG_ADEC_OPUS_SCRATCH_SIZE                          ((uint32_t)143360)
#define BDSP_ARM_IMG_ADEC_OPUS_SIZE                                  ((uint32_t)225792)
#define BDSP_ARM_IMG_ADEC_OPUS_INTER_FRAME_SIZE                      ((uint32_t)218112)
#define BDSP_ARM_IMG_ADEC_OPUS_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_OPUS_TABLES_SIZE                           ((uint32_t)21504)