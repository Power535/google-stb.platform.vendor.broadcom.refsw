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
#define BDSP_ARM_IMG_COMMON_SIZE                                     ((uint32_t)69120)

/*DEBUG_CONTROL Size requirement*/
#define BDSP_ARM_IMG_DEBUG_CONTROL_SIZE                              ((uint32_t)25600)

/*MM_CLIENT Size requirement*/
#define BDSP_ARM_IMG_MM_CLIENT_SIZE                                  ((uint32_t)108544)

/*KERNEL_OPS Size requirement*/
#define BDSP_ARM_IMG_KERNEL_OPS_SIZE                                 ((uint32_t)93184)

/*STAGE_IO Size requirement*/
#define BDSP_ARM_IMG_STAGE_IO_SIZE                                   ((uint32_t)230912)

/*COMMON_IDS Size requirement*/
#define BDSP_ARM_IMG_COMMON_IDS_SIZE                                 ((uint32_t)272384)

/*INIT_PROCESS Size requirement*/
#define BDSP_ARM_IMG_INIT_PROCESS_SIZE                               ((uint32_t)33792)

/*SCHEDULING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_SCHEDULING_PROCESS_SIZE                         ((uint32_t)159744)

/*ROUTING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_ROUTING_PROCESS_SIZE                            ((uint32_t)75776)

/*MESSAGING_PROCESS Size requirement*/
#define BDSP_ARM_IMG_MESSAGING_PROCESS_SIZE                          ((uint32_t)142848)

/*ALGO_PROCESS Size requirement*/
#define BDSP_ARM_IMG_ALGO_PROCESS_SIZE                               ((uint32_t)184832)

/*IDLE_PROCESS Size requirement*/
#define BDSP_ARM_IMG_IDLE_PROCESS_SIZE                               ((uint32_t)16384)

/*MM_SERVER Size requirement*/
#define BDSP_ARM_IMG_MM_SERVER_SIZE                                  ((uint32_t)165376)

/*AIDS_WAVFORMATEX Size requirement*/
#define BDSP_ARM_IMG_AIDS_WAVFORMATEX_SIZE                           ((uint32_t)49664)

/*AIDS_MPEG1 Size requirement*/
#define BDSP_ARM_IMG_AIDS_MPEG1_SIZE                                 ((uint32_t)29184)

/*AIDS_ADTS Size requirement*/
#define BDSP_ARM_IMG_AIDS_ADTS_SIZE                                  ((uint32_t)22528)

/*AIDS_LOAS Size requirement*/
#define BDSP_ARM_IMG_AIDS_LOAS_SIZE                                  ((uint32_t)34816)

/*ADEC_PCMWAV Size requirement*/
#define BDSP_ARM_IMG_ADEC_PCMWAV_SCRATCH_SIZE                        ((uint32_t)32768)
#define BDSP_ARM_IMG_ADEC_PCMWAV_SIZE                                ((uint32_t)39424)
#define BDSP_ARM_IMG_ADEC_PCMWAV_INTER_FRAME_SIZE                    ((uint32_t)1536)
#define BDSP_ARM_IMG_ADEC_PCMWAV_INTER_FRAME_ENCODED_SIZE            ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_PCMWAV_TABLES_SIZE                         ((uint32_t)1536)

/*ADEC_MPEG1 Size requirement*/
#define BDSP_ARM_IMG_ADEC_MPEG1_SCRATCH_SIZE                         ((uint32_t)36864)
#define BDSP_ARM_IMG_ADEC_MPEG1_SIZE                                 ((uint32_t)133632)
#define BDSP_ARM_IMG_ADEC_MPEG1_INTER_FRAME_SIZE                     ((uint32_t)10752)
#define BDSP_ARM_IMG_ADEC_MPEG1_INTER_FRAME_ENCODED_SIZE             ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MPEG1_TABLES_SIZE                          ((uint32_t)15360)

/*ADEC_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_AACHE_SCRATCH_SIZE                         ((uint32_t)249856)
#define BDSP_ARM_IMG_ADEC_AACHE_SIZE                                 ((uint32_t)374784)
#define BDSP_ARM_IMG_ADEC_AACHE_INTER_FRAME_SIZE                     ((uint32_t)185856)
#define BDSP_ARM_IMG_ADEC_AACHE_INTER_FRAME_ENCODED_SIZE             ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_AACHE_TABLES_SIZE                          ((uint32_t)71680)

/*ADEC_PASSTHRU Size requirement*/
#define BDSP_ARM_IMG_ADEC_PASSTHRU_SCRATCH_SIZE                      ((uint32_t)36864)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_SIZE                              ((uint32_t)68096)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_INTER_FRAME_SIZE                  ((uint32_t)2048)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_INTER_FRAME_ENCODED_SIZE          ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_PASSTHRU_TABLES_SIZE                       ((uint32_t)1536)

/*AIDS_DDP Size requirement*/
#define BDSP_ARM_IMG_AIDS_DDP_SIZE                                   ((uint32_t)31232)

/*ADEC_DDP Size requirement*/
#define BDSP_ARM_IMG_ADEC_DDP_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_DDP_SIZE                                   ((uint32_t)814592)
#define BDSP_ARM_IMG_ADEC_DDP_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_DDP_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_DDP_TABLES_SIZE                            ((uint32_t)205312)

/*ADEC_AC3 Size requirement*/
#define BDSP_ARM_IMG_ADEC_AC3_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_AC3_SIZE                                   ((uint32_t)814592)
#define BDSP_ARM_IMG_ADEC_AC3_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_AC3_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_AC3_TABLES_SIZE                            ((uint32_t)205312)

/*ADEC_UDC Size requirement*/
#define BDSP_ARM_IMG_ADEC_UDC_SCRATCH_SIZE                           ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_UDC_SIZE                                   ((uint32_t)814592)
#define BDSP_ARM_IMG_ADEC_UDC_INTER_FRAME_SIZE                       ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_UDC_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_UDC_TABLES_SIZE                            ((uint32_t)205312)

/*APP_DPCMR Size requirement*/
#define BDSP_ARM_IMG_APP_DPCMR_SCRATCH_SIZE                          ((uint32_t)40960)
#define BDSP_ARM_IMG_APP_DPCMR_SIZE                                  ((uint32_t)112128)
#define BDSP_ARM_IMG_APP_DPCMR_INTER_FRAME_SIZE                      ((uint32_t)175616)
#define BDSP_ARM_IMG_APP_DPCMR_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_APP_DPCMR_TABLES_SIZE                           ((uint32_t)3584)

/*APP_MIXER_DAPV2 Size requirement*/
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_SCRATCH_SIZE                    ((uint32_t)1273856)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_SIZE                            ((uint32_t)109056)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_INTER_FRAME_SIZE                ((uint32_t)331776)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_INTER_FRAME_ENCODED_SIZE        ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MIXER_DAPV2_TABLES_SIZE                     ((uint32_t)89600)

/*ADEC_DOLBY_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_SCRATCH_SIZE                   ((uint32_t)528384)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_SIZE                           ((uint32_t)755712)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_INTER_FRAME_SIZE               ((uint32_t)525312)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_INTER_FRAME_ENCODED_SIZE       ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_DOLBY_AACHE_TABLES_SIZE                    ((uint32_t)159744)

/*AENC_DDP Size requirement*/
#define BDSP_ARM_IMG_AENC_DDP_SCRATCH_SIZE                           ((uint32_t)81920)
#define BDSP_ARM_IMG_AENC_DDP_SIZE                                   ((uint32_t)827392)
#define BDSP_ARM_IMG_AENC_DDP_INTER_FRAME_SIZE                       ((uint32_t)219648)
#define BDSP_ARM_IMG_AENC_DDP_INTER_FRAME_ENCODED_SIZE               ((uint32_t)512)
#define BDSP_ARM_IMG_AENC_DDP_TABLES_SIZE                            ((uint32_t)28160)

/*ADEC_MS11PLUS_UDC Size requirement*/
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_SCRATCH_SIZE                  ((uint32_t)593920)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_SIZE                          ((uint32_t)811008)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_INTER_FRAME_SIZE              ((uint32_t)1049600)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_INTER_FRAME_ENCODED_SIZE      ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_UDC_TABLES_SIZE                   ((uint32_t)205312)

/*APP_MS11PLUS_DPCMR Size requirement*/
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_SCRATCH_SIZE                 ((uint32_t)40960)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_SIZE                         ((uint32_t)112128)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_INTER_FRAME_SIZE             ((uint32_t)175616)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_INTER_FRAME_ENCODED_SIZE     ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MS11PLUS_DPCMR_TABLES_SIZE                  ((uint32_t)3584)

/*APP_MS11PLUS_MIXER Size requirement*/
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_SCRATCH_SIZE                 ((uint32_t)1273856)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_SIZE                         ((uint32_t)108544)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_INTER_FRAME_SIZE             ((uint32_t)331776)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_INTER_FRAME_ENCODED_SIZE     ((uint32_t)512)
#define BDSP_ARM_IMG_APP_MS11PLUS_MIXER_TABLES_SIZE                  ((uint32_t)89600)

/*ADEC_MS11PLUS_DOLBY_AACHE Size requirement*/
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_SCRATCH_SIZE          ((uint32_t)528384)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_SIZE                  ((uint32_t)755712)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_INTER_FRAME_SIZE      ((uint32_t)525312)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_INTER_FRAME_ENCODED_SIZE ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_MS11PLUS_DOLBY_AACHE_TABLES_SIZE           ((uint32_t)159744)

/*AENC_MS11PLUS_DD Size requirement*/
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_SCRATCH_SIZE                   ((uint32_t)81920)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_SIZE                           ((uint32_t)827392)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_INTER_FRAME_SIZE               ((uint32_t)219648)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_INTER_FRAME_ENCODED_SIZE       ((uint32_t)512)
#define BDSP_ARM_IMG_AENC_MS11PLUS_DD_TABLES_SIZE                    ((uint32_t)28160)

/*APP_SRC Size requirement*/
#define BDSP_ARM_IMG_APP_SRC_SCRATCH_SIZE                            ((uint32_t)45056)
#define BDSP_ARM_IMG_APP_SRC_SIZE                                    ((uint32_t)35328)
#define BDSP_ARM_IMG_APP_SRC_INTER_FRAME_SIZE                        ((uint32_t)14336)
#define BDSP_ARM_IMG_APP_SRC_INTER_FRAME_ENCODED_SIZE                ((uint32_t)512)
#define BDSP_ARM_IMG_APP_SRC_TABLES_SIZE                             ((uint32_t)4096)

/*APP_FW_MIXER Size requirement*/
#define BDSP_ARM_IMG_APP_FW_MIXER_SCRATCH_SIZE                       ((uint32_t)102400)
#define BDSP_ARM_IMG_APP_FW_MIXER_SIZE                               ((uint32_t)117248)
#define BDSP_ARM_IMG_APP_FW_MIXER_INTER_FRAME_SIZE                   ((uint32_t)33280)
#define BDSP_ARM_IMG_APP_FW_MIXER_INTER_FRAME_ENCODED_SIZE           ((uint32_t)512)
#define BDSP_ARM_IMG_APP_FW_MIXER_TABLES_SIZE                        ((uint32_t)6144)

/*APP_DSOLA Size requirement*/
#define BDSP_ARM_IMG_APP_DSOLA_SCRATCH_SIZE                          ((uint32_t)4096)
#define BDSP_ARM_IMG_APP_DSOLA_SIZE                                  ((uint32_t)32768)
#define BDSP_ARM_IMG_APP_DSOLA_INTER_FRAME_SIZE                      ((uint32_t)78336)
#define BDSP_ARM_IMG_APP_DSOLA_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_APP_DSOLA_TABLES_SIZE                           ((uint32_t)2048)

/*ADEC_OPUS Size requirement*/
#define BDSP_ARM_IMG_ADEC_OPUS_SCRATCH_SIZE                          ((uint32_t)143360)
#define BDSP_ARM_IMG_ADEC_OPUS_SIZE                                  ((uint32_t)230400)
#define BDSP_ARM_IMG_ADEC_OPUS_INTER_FRAME_SIZE                      ((uint32_t)215040)
#define BDSP_ARM_IMG_ADEC_OPUS_INTER_FRAME_ENCODED_SIZE              ((uint32_t)512)
#define BDSP_ARM_IMG_ADEC_OPUS_TABLES_SIZE                           ((uint32_t)18432)