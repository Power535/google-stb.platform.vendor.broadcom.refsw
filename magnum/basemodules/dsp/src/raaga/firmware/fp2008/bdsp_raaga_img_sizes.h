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

#define BDSP_IMG_AACHEADTS_IDS_INTER_FRAME_SIZE                      ((uint32_t)16876)
#define BDSP_IMG_AACHEADTS_IDS_SIZE                                  ((uint32_t)3840)
#define BDSP_IMG_AACHELOAS_IDS_INTER_FRAME_SIZE                      ((uint32_t)16876)
#define BDSP_IMG_AACHELOAS_IDS_SIZE                                  ((uint32_t)5160)
#define BDSP_IMG_AACHE_DECODE_INTER_FRAME_SIZE                       ((uint32_t)197260)
#define BDSP_IMG_AACHE_DECODE_SIZE                                   ((uint32_t)233472)
#define BDSP_IMG_AACHE_DECODE_TABLES_SIZE                            ((uint32_t)85852)
#define BDSP_IMG_AACHE_ENCODE_INTER_FRAME_SIZE                       ((uint32_t)178812)
#define BDSP_IMG_AACHE_ENCODE_SIZE                                   ((uint32_t)330336)
#define BDSP_IMG_AACHE_ENCODE_TABLES_SIZE                            ((uint32_t)23944)
#define BDSP_IMG_AC3_DECODE_INTER_FRAME_SIZE                         ((uint32_t)62912)
#define BDSP_IMG_AC3_DECODE_SIZE                                     ((uint32_t)124992)
#define BDSP_IMG_AC3_DECODE_TABLES_SIZE                              ((uint32_t)29936)
#define BDSP_IMG_AC3_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_AC3_IDS_SIZE                                        ((uint32_t)10604)
#define BDSP_IMG_AC4_DECODE_INTER_FRAME_SIZE                         ((uint32_t)8688256)
#define BDSP_IMG_AC4_DECODE_SIZE                                     ((uint32_t)8)
#define BDSP_IMG_AC4_DECODE_TABLES_SIZE                              ((uint32_t)174080)
#define BDSP_IMG_AC4_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_AC4_IDS_SIZE                                        ((uint32_t)0)
#define BDSP_IMG_ADPCM_DECODE_INTER_FRAME_SIZE                       ((uint32_t)15408)
#define BDSP_IMG_ADPCM_DECODE_SIZE                                   ((uint32_t)10628)
#define BDSP_IMG_ADPCM_DECODE_TABLES_SIZE                            ((uint32_t)1280)
#define BDSP_IMG_ALGOLIB_CODE_SIZE                                   ((uint32_t)35944)
#define BDSP_IMG_ALGOLIB_TABLES_SIZE                                 ((uint32_t)13680)
#define BDSP_IMG_ALS_DECODE_INTER_FRAME_SIZE                         ((uint32_t)536)
#define BDSP_IMG_ALS_DECODE_SIZE                                     ((uint32_t)8)
#define BDSP_IMG_ALS_DECODE_TABLES_SIZE                              ((uint32_t)928)
#define BDSP_IMG_ALS_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_ALS_IDS_SIZE                                        ((uint32_t)17624)
#define BDSP_IMG_AMBISONICS_CODE_SIZE                                ((uint32_t)8)
#define BDSP_IMG_AMBISONICS_INTER_FRAME_SIZE                         ((uint32_t)77856)
#define BDSP_IMG_AMBISONICS_TABLES_SIZE                              ((uint32_t)111024)
#define BDSP_IMG_AMRWB_DECODE_INTER_FRAME_SIZE                       ((uint32_t)12332)
#define BDSP_IMG_AMRWB_DECODE_SIZE                                   ((uint32_t)68720)
#define BDSP_IMG_AMRWB_DECODE_TABLES_SIZE                            ((uint32_t)24268)
#define BDSP_IMG_AMRWB_ENCODE_INTER_FRAME_SIZE                       ((uint32_t)3940)
#define BDSP_IMG_AMRWB_ENCODE_SIZE                                   ((uint32_t)86964)
#define BDSP_IMG_AMRWB_ENCODE_TABLES_SIZE                            ((uint32_t)25268)
#define BDSP_IMG_AMR_DECODE_INTER_FRAME_SIZE                         ((uint32_t)7364)
#define BDSP_IMG_AMR_DECODE_SIZE                                     ((uint32_t)55784)
#define BDSP_IMG_AMR_DECODE_TABLES_SIZE                              ((uint32_t)30184)
#define BDSP_IMG_AMR_ENCODE_INTER_FRAME_SIZE                         ((uint32_t)3948)
#define BDSP_IMG_AMR_ENCODE_SIZE                                     ((uint32_t)113024)
#define BDSP_IMG_AMR_ENCODE_TABLES_SIZE                              ((uint32_t)31608)
#define BDSP_IMG_BRCM_3DSURROUND_CODE_SIZE                           ((uint32_t)8248)
#define BDSP_IMG_BRCM_3DSURROUND_INTER_FRAME_SIZE                    ((uint32_t)14316)
#define BDSP_IMG_BRCM_3DSURROUND_TABLES_SIZE                         ((uint32_t)123356)
#define BDSP_IMG_BRCM_AVL_CODE_SIZE                                  ((uint32_t)10784)
#define BDSP_IMG_BRCM_AVL_INTER_FRAME_SIZE                           ((uint32_t)12520)
#define BDSP_IMG_BRCM_AVL_TABLES_SIZE                                ((uint32_t)376)
#define BDSP_IMG_BTSCENC_CODE_SIZE                                   ((uint32_t)19312)
#define BDSP_IMG_BTSCENC_INTER_FRAME_SIZE                            ((uint32_t)1564)
#define BDSP_IMG_BTSCENC_TABLES_SIZE                                 ((uint32_t)2568)
#define BDSP_IMG_CDB_PASSTHRU_CODE_SIZE                              ((uint32_t)33604)
#define BDSP_IMG_CDB_PASSTHRU_INTER_FRAME_SIZE                       ((uint32_t)852)
#define BDSP_IMG_CDB_PASSTHRU_TABLES_SIZE                            ((uint32_t)176)
#define BDSP_IMG_CUSTOMVOICE_CODE_SIZE                               ((uint32_t)26392)
#define BDSP_IMG_CUSTOMVOICE_INTER_FRAME_SIZE                        ((uint32_t)9600)
#define BDSP_IMG_CUSTOMVOICE_TABLES_SIZE                             ((uint32_t)12004)
#define BDSP_IMG_DDP_DECODE_INTER_FRAME_SIZE                         ((uint32_t)62912)
#define BDSP_IMG_DDP_DECODE_SIZE                                     ((uint32_t)125108)
#define BDSP_IMG_DDP_DECODE_TABLES_SIZE                              ((uint32_t)29936)
#define BDSP_IMG_DDP_ENCODE_CODE_SIZE                                ((uint32_t)8)
#define BDSP_IMG_DDP_ENCODE_INTER_FRAME_SIZE                         ((uint32_t)217536)
#define BDSP_IMG_DDP_ENCODE_TABLES_SIZE                              ((uint32_t)24812)
#define BDSP_IMG_DDP_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_DDP_IDS_SIZE                                        ((uint32_t)10604)
#define BDSP_IMG_DDRE_CODE_SIZE                                      ((uint32_t)42600)
#define BDSP_IMG_DDRE_INTER_FRAME_SIZE                               ((uint32_t)43516)
#define BDSP_IMG_DDRE_TABLES_SIZE                                    ((uint32_t)6068)
#define BDSP_IMG_DD_TRANSCODE_INTER_FRAME_SIZE                       ((uint32_t)53040)
#define BDSP_IMG_DD_TRANSCODE_SIZE                                   ((uint32_t)52040)
#define BDSP_IMG_DD_TRANSCODE_TABLES_SIZE                            ((uint32_t)10864)
#define BDSP_IMG_DOLBY_AACHE_DECODE_INTER_FRAME_SIZE                 ((uint32_t)409044)
#define BDSP_IMG_DOLBY_AACHE_DECODE_SIZE                             ((uint32_t)8)
#define BDSP_IMG_DOLBY_AACHE_DECODE_TABLES_SIZE                      ((uint32_t)122016)
#define BDSP_IMG_DOLBY_MS_DDP_DECODE_INTER_FRAME_SIZE                ((uint32_t)62912)
#define BDSP_IMG_DOLBY_MS_DDP_DECODE_SIZE                            ((uint32_t)125120)
#define BDSP_IMG_DOLBY_MS_DDP_DECODE_TABLES_SIZE                     ((uint32_t)29936)
#define BDSP_IMG_DOLBY_PULSE_DECODE_INTER_FRAME_SIZE                 ((uint32_t)353580)
#define BDSP_IMG_DOLBY_PULSE_DECODE_SIZE                             ((uint32_t)462152)
#define BDSP_IMG_DOLBY_PULSE_DECODE_TABLES_SIZE                      ((uint32_t)126844)
#define BDSP_IMG_DPCMR_CODE_SIZE                                     ((uint32_t)8)
#define BDSP_IMG_DPCMR_INTER_FRAME_SIZE                              ((uint32_t)173032)
#define BDSP_IMG_DPCMR_TABLES_SIZE                                   ((uint32_t)2356)
#define BDSP_IMG_DRA_DECODE_INTER_FRAME_SIZE                         ((uint32_t)38112)
#define BDSP_IMG_DRA_DECODE_SIZE                                     ((uint32_t)36296)
#define BDSP_IMG_DRA_DECODE_TABLES_SIZE                              ((uint32_t)37304)
#define BDSP_IMG_DRA_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_DRA_IDS_SIZE                                        ((uint32_t)3328)
#define BDSP_IMG_DSOLA_CODE_SIZE                                     ((uint32_t)6012)
#define BDSP_IMG_DSOLA_INTER_FRAME_SIZE                              ((uint32_t)25612)
#define BDSP_IMG_DSOLA_TABLES_SIZE                                   ((uint32_t)1024)
#define BDSP_IMG_DTSHD_DECODE_INTER_FRAME_SIZE                       ((uint32_t)47580)
#define BDSP_IMG_DTSHD_DECODE_SIZE                                   ((uint32_t)93344)
#define BDSP_IMG_DTSHD_DECODE_TABLES_SIZE                            ((uint32_t)129164)
#define BDSP_IMG_DTSHD_IDS_INTER_FRAME_SIZE                          ((uint32_t)16876)
#define BDSP_IMG_DTSHD_IDS_SIZE                                      ((uint32_t)13480)
#define BDSP_IMG_DTS_ENCODE_INTER_FRAME_SIZE                         ((uint32_t)37428)
#define BDSP_IMG_DTS_ENCODE_SIZE                                     ((uint32_t)33896)
#define BDSP_IMG_DTS_ENCODE_TABLES_SIZE                              ((uint32_t)44132)
#define BDSP_IMG_DTS_EXPRESS_DECODE_INTER_FRAME_SIZE                 ((uint32_t)39624)
#define BDSP_IMG_DTS_EXPRESS_DECODE_SIZE                             ((uint32_t)121672)
#define BDSP_IMG_DTS_EXPRESS_DECODE_TABLES_SIZE                      ((uint32_t)29920)
#define BDSP_IMG_DTS_EXPRESS_IDS_DECODE_SIZE                         ((uint32_t)9004)
#define BDSP_IMG_DTS_EXPRESS_IDS_INTER_FRAME_SIZE                    ((uint32_t)16876)
#define BDSP_IMG_DTS_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_DTS_IDS_SIZE                                        ((uint32_t)5480)
#define BDSP_IMG_DV258_CODE_SIZE                                     ((uint32_t)62752)
#define BDSP_IMG_DV258_INTER_FRAME_SIZE                              ((uint32_t)19048)
#define BDSP_IMG_DV258_TABLES_SIZE                                   ((uint32_t)2268)
#define BDSP_IMG_DVDLPCM_IDS_INTER_FRAME_SIZE                        ((uint32_t)16876)
#define BDSP_IMG_DVDLPCM_IDS_SIZE                                    ((uint32_t)5712)
#define BDSP_IMG_FADECTRL_CODE_SIZE                                  ((uint32_t)4452)
#define BDSP_IMG_FADECTRL_INTER_FRAME_SIZE                           ((uint32_t)4164)
#define BDSP_IMG_FADECTRL_TABLES_SIZE                                ((uint32_t)4)
#define BDSP_IMG_FLAC_DECODE_INTER_FRAME_SIZE                        ((uint32_t)68)
#define BDSP_IMG_FLAC_DECODE_SIZE                                    ((uint32_t)34248)
#define BDSP_IMG_FLAC_DECODE_TABLES_SIZE                             ((uint32_t)772)
#define BDSP_IMG_FLAC_IDS_INTER_FRAME_SIZE                           ((uint32_t)16876)
#define BDSP_IMG_FLAC_IDS_SIZE                                       ((uint32_t)4344)
#define BDSP_IMG_FW_MIXER_CODE_SIZE                                  ((uint32_t)36812)
#define BDSP_IMG_FW_MIXER_INTER_FRAME_SIZE                           ((uint32_t)33464)
#define BDSP_IMG_FW_MIXER_TABLES_SIZE                                ((uint32_t)4780)
#define BDSP_IMG_G711_G726_DECODE_INTER_FRAME_SIZE                   ((uint32_t)8808)
#define BDSP_IMG_G711_G726_DECODE_SIZE                               ((uint32_t)14852)
#define BDSP_IMG_G711_G726_DECODE_TABLES_SIZE                        ((uint32_t)500)
#define BDSP_IMG_G711_G726_ENCODE_INTER_FRAME_SIZE                   ((uint32_t)1024)
#define BDSP_IMG_G711_G726_ENCODE_SIZE                               ((uint32_t)12968)
#define BDSP_IMG_G711_G726_ENCODE_TABLES_SIZE                        ((uint32_t)4)
#define BDSP_IMG_G722_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)940)
#define BDSP_IMG_G722_ENCODE_SIZE                                    ((uint32_t)5456)
#define BDSP_IMG_G722_ENCODE_TABLES_SIZE                             ((uint32_t)1128)
#define BDSP_IMG_G723_1_DECODE_INTER_FRAME_SIZE                      ((uint32_t)8648)
#define BDSP_IMG_G723_1_DECODE_SIZE                                  ((uint32_t)41936)
#define BDSP_IMG_G723_1_DECODE_TABLES_SIZE                           ((uint32_t)18212)
#define BDSP_IMG_G723_1_ENCODE_INTER_FRAME_SIZE                      ((uint32_t)3400)
#define BDSP_IMG_G723_1_ENCODE_SIZE                                  ((uint32_t)52152)
#define BDSP_IMG_G723_1_ENCODE_TABLES_SIZE                           ((uint32_t)18832)
#define BDSP_IMG_G729_DECODE_INTER_FRAME_SIZE                        ((uint32_t)5368)
#define BDSP_IMG_G729_DECODE_SIZE                                    ((uint32_t)42032)
#define BDSP_IMG_G729_DECODE_TABLES_SIZE                             ((uint32_t)7852)
#define BDSP_IMG_G729_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)5232)
#define BDSP_IMG_G729_ENCODE_SIZE                                    ((uint32_t)112816)
#define BDSP_IMG_G729_ENCODE_TABLES_SIZE                             ((uint32_t)7736)
#define BDSP_IMG_GEN_CDBITB_CODE_SIZE                                ((uint32_t)13600)
#define BDSP_IMG_GEN_CDBITB_INTER_FRAME_SIZE                         ((uint32_t)948)
#define BDSP_IMG_GEN_CDBITB_TABLES_SIZE                              ((uint32_t)40)
#define BDSP_IMG_H264_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)2993504)
#define BDSP_IMG_H264_ENCODE_SIZE                                    ((uint32_t)8)
#define BDSP_IMG_H264_ENCODE_TABLES_SIZE                             ((uint32_t)124184)
#define BDSP_IMG_IDSCOMMON_CODE_SIZE                                 ((uint32_t)37064)
#define BDSP_IMG_ILBC_DECODE_INTER_FRAME_SIZE                        ((uint32_t)10268)
#define BDSP_IMG_ILBC_DECODE_SIZE                                    ((uint32_t)41408)
#define BDSP_IMG_ILBC_DECODE_TABLES_SIZE                             ((uint32_t)3320)
#define BDSP_IMG_ILBC_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)1180)
#define BDSP_IMG_ILBC_ENCODE_SIZE                                    ((uint32_t)32684)
#define BDSP_IMG_ILBC_ENCODE_TABLES_SIZE                             ((uint32_t)4600)
#define BDSP_IMG_ISAC_DECODE_INTER_FRAME_SIZE                        ((uint32_t)2328)
#define BDSP_IMG_ISAC_DECODE_SIZE                                    ((uint32_t)35208)
#define BDSP_IMG_ISAC_DECODE_TABLES_SIZE                             ((uint32_t)30864)
#define BDSP_IMG_ISAC_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)9888)
#define BDSP_IMG_ISAC_ENCODE_SIZE                                    ((uint32_t)57116)
#define BDSP_IMG_ISAC_ENCODE_TABLES_SIZE                             ((uint32_t)25264)
#define BDSP_IMG_KARAOKE_CODE_SIZE                                   ((uint32_t)9144)
#define BDSP_IMG_KARAOKE_INTER_FRAME_SIZE                            ((uint32_t)53260)
#define BDSP_IMG_KARAOKE_TABLES_SIZE                                 ((uint32_t)49152)
#define BDSP_IMG_LPCM_DECODE_CODE_SIZE                               ((uint32_t)19408)
#define BDSP_IMG_LPCM_DECODE_INTER_FRAME_SIZE                        ((uint32_t)176)
#define BDSP_IMG_LPCM_DECODE_TABLES_SIZE                             ((uint32_t)4000)
#define BDSP_IMG_LPCM_ENCODE_CODE_SIZE                               ((uint32_t)3408)
#define BDSP_IMG_LPCM_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)1288)
#define BDSP_IMG_LPCM_ENCODE_TABLES_SIZE                             ((uint32_t)4)
#define BDSP_IMG_MAC_DECODE_INTER_FRAME_SIZE                         ((uint32_t)22176)
#define BDSP_IMG_MAC_DECODE_SIZE                                     ((uint32_t)16600)
#define BDSP_IMG_MAC_DECODE_TABLES_SIZE                              ((uint32_t)2112)
#define BDSP_IMG_MAC_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_MAC_IDS_SIZE                                        ((uint32_t)4856)
#define BDSP_IMG_MIXER_DAPV2_CODE_SIZE                               ((uint32_t)4)
#define BDSP_IMG_MIXER_DAPV2_IDS_INTER_FRAME_SIZE                    ((uint32_t)3964)
#define BDSP_IMG_MIXER_DAPV2_IDS_SIZE                                ((uint32_t)9296)
#define BDSP_IMG_MIXER_DAPV2_INTER_FRAME_SIZE                        ((uint32_t)520224)
#define BDSP_IMG_MIXER_DAPV2_TABLES_SIZE                             ((uint32_t)207536)
#define BDSP_IMG_MIXER_IDS_INTER_FRAME_SIZE                          ((uint32_t)44)
#define BDSP_IMG_MIXER_IDS_SIZE                                      ((uint32_t)3612)
#define BDSP_IMG_MLP_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_MLP_IDS_SIZE                                        ((uint32_t)4604)
#define BDSP_IMG_MLP_PASSTHROUGH_CODE_SIZE                           ((uint32_t)6544)
#define BDSP_IMG_MLP_PASSTHROUGH_INTER_FRAME_SIZE                    ((uint32_t)48)
#define BDSP_IMG_MLP_PASSTHROUGH_TABLES_SIZE                         ((uint32_t)32)
#define BDSP_IMG_MP3_ENCODE_INTER_FRAME_SIZE                         ((uint32_t)24572)
#define BDSP_IMG_MP3_ENCODE_SIZE                                     ((uint32_t)49592)
#define BDSP_IMG_MP3_ENCODE_TABLES_SIZE                              ((uint32_t)27344)
#define BDSP_IMG_MPEG1_DECODE_INTER_FRAME_SIZE                       ((uint32_t)9300)
#define BDSP_IMG_MPEG1_DECODE_SIZE                                   ((uint32_t)37724)
#define BDSP_IMG_MPEG1_DECODE_TABLES_SIZE                            ((uint32_t)14040)
#define BDSP_IMG_MPEG1_IDS_INTER_FRAME_SIZE                          ((uint32_t)16876)
#define BDSP_IMG_MPEG1_IDS_SIZE                                      ((uint32_t)9584)
#define BDSP_IMG_OPUS_DECODE_INTER_FRAME_SIZE                        ((uint32_t)213280)
#define BDSP_IMG_OPUS_DECODE_SIZE                                    ((uint32_t)106880)
#define BDSP_IMG_OPUS_DECODE_TABLES_SIZE                             ((uint32_t)17248)
#define BDSP_IMG_OPUS_ENCODE_CODE_SIZE                               ((uint32_t)8)
#define BDSP_IMG_OPUS_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)54260)
#define BDSP_IMG_OPUS_ENCODE_TABLES_SIZE                             ((uint32_t)17896)
#define BDSP_IMG_OUTPUTFORMATTER_CODE_SIZE                           ((uint32_t)2376)
#define BDSP_IMG_OUTPUTFORMATTER_INTER_FRAME_SIZE                    ((uint32_t)4)
#define BDSP_IMG_OUTPUTFORMATTER_TABLES_SIZE                         ((uint32_t)40)
#define BDSP_IMG_PCMWAV_DECODE_INTER_FRAME_SIZE                      ((uint32_t)56)
#define BDSP_IMG_PCMWAV_DECODE_SIZE                                  ((uint32_t)14024)
#define BDSP_IMG_PCMWAV_DECODE_TABLES_SIZE                           ((uint32_t)4)
#define BDSP_IMG_PCMWAV_IDS_INTER_FRAME_SIZE                         ((uint32_t)16876)
#define BDSP_IMG_PCMWAV_IDS_SIZE                                     ((uint32_t)4512)
#define BDSP_IMG_PCM_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_PCM_IDS_SIZE                                        ((uint32_t)2344)
#define BDSP_IMG_RALBR_DECODE_INTER_FRAME_SIZE                       ((uint32_t)17872)
#define BDSP_IMG_RALBR_DECODE_SIZE                                   ((uint32_t)29784)
#define BDSP_IMG_RALBR_DECODE_TABLES_SIZE                            ((uint32_t)29012)
#define BDSP_IMG_RALBR_IDS_INTER_FRAME_SIZE                          ((uint32_t)16876)
#define BDSP_IMG_RALBR_IDS_SIZE                                      ((uint32_t)4200)
#define BDSP_IMG_SCM1_DECODE_INTER_FRAME_SIZE                        ((uint32_t)4)
#define BDSP_IMG_SCM1_DECODE_SIZE                                    ((uint32_t)44)
#define BDSP_IMG_SCM1_DECODE_TABLES_SIZE                             ((uint32_t)4)
#define BDSP_IMG_SCM2_DECODE_INTER_FRAME_SIZE                        ((uint32_t)4)
#define BDSP_IMG_SCM2_DECODE_SIZE                                    ((uint32_t)44)
#define BDSP_IMG_SCM2_DECODE_TABLES_SIZE                             ((uint32_t)4)
#define BDSP_IMG_SCM3_DECODE_INTER_FRAME_SIZE                        ((uint32_t)4)
#define BDSP_IMG_SCM3_DECODE_SIZE                                    ((uint32_t)124292)
#define BDSP_IMG_SCM3_DECODE_TABLES_SIZE                             ((uint32_t)20868)
#define BDSP_IMG_SCM_TASK_CODE_SIZE                                  ((uint32_t)4428)
#define BDSP_IMG_SPEEXAEC_CODE_SIZE                                  ((uint32_t)74000)
#define BDSP_IMG_SPEEXAEC_INTER_FRAME_SIZE                           ((uint32_t)36920)
#define BDSP_IMG_SPEEXAEC_TABLES_SIZE                                ((uint32_t)4548)
#define BDSP_IMG_SRC_CODE_SIZE                                       ((uint32_t)19768)
#define BDSP_IMG_SRC_INTER_FRAME_SIZE                                ((uint32_t)13028)
#define BDSP_IMG_SRC_TABLES_SIZE                                     ((uint32_t)2704)
#define BDSP_IMG_SRS_TRUSURROUNDHD_CODE_SIZE                         ((uint32_t)31560)
#define BDSP_IMG_SRS_TRUSURROUNDHD_INTER_FRAME_SIZE                  ((uint32_t)712)
#define BDSP_IMG_SRS_TRUSURROUNDHD_TABLES_SIZE                       ((uint32_t)3288)
#define BDSP_IMG_SRS_TVOL_CODE_SIZE                                  ((uint32_t)29528)
#define BDSP_IMG_SRS_TVOL_INTER_FRAME_SIZE                           ((uint32_t)7344)
#define BDSP_IMG_SRS_TVOL_TABLES_SIZE                                ((uint32_t)4624)
#define BDSP_IMG_SYSLIB_CODE_SIZE                                    ((uint32_t)17560)
#define BDSP_IMG_SYSTEM_CODE_SIZE                                    ((uint32_t)67288)
#define BDSP_IMG_TRUEHD_DECODE_INTER_FRAME_SIZE                      ((uint32_t)5108)
#define BDSP_IMG_TRUEHD_DECODE_SIZE                                  ((uint32_t)75192)
#define BDSP_IMG_TRUEHD_DECODE_TABLES_SIZE                           ((uint32_t)4824)
#define BDSP_IMG_TSMCORRECTION_CODE_SIZE                             ((uint32_t)8936)
#define BDSP_IMG_TSMCORRECTION_INTER_FRAME_SIZE                      ((uint32_t)24616)
#define BDSP_IMG_TSMCORRECTION_TABLES_SIZE                           ((uint32_t)1024)
#define BDSP_IMG_UDC_DECODE_INTER_FRAME_SIZE                         ((uint32_t)566524)
#define BDSP_IMG_UDC_DECODE_SIZE                                     ((uint32_t)4)
#define BDSP_IMG_UDC_DECODE_TABLES_SIZE                              ((uint32_t)42288)
#define BDSP_IMG_UDC_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_UDC_IDS_SIZE                                        ((uint32_t)10604)
#define BDSP_IMG_VIDEO_DECODE_TASK_CODE_SIZE                         ((uint32_t)4480)
#define BDSP_IMG_VIDEO_ENCODE_TASK_CODE_SIZE                         ((uint32_t)4816)
#define BDSP_IMG_VIDIDSCOMMON_CODE_SIZE                              ((uint32_t)7644)
#define BDSP_IMG_VIDIDSCOMMON_INTER_FRAME_SIZE                       ((uint32_t)2188)
#define BDSP_IMG_VOCALS_CODE_SIZE                                    ((uint32_t)4956)
#define BDSP_IMG_VOCALS_INTER_FRAME_SIZE                             ((uint32_t)400408)
#define BDSP_IMG_VOCALS_TABLES_SIZE                                  ((uint32_t)4)
#define BDSP_IMG_VORBIS_DECODE_INTER_FRAME_SIZE                      ((uint32_t)424048)
#define BDSP_IMG_VORBIS_DECODE_SIZE                                  ((uint32_t)55720)
#define BDSP_IMG_VORBIS_DECODE_TABLES_SIZE                           ((uint32_t)26772)
#define BDSP_IMG_VORBIS_IDS_INTER_FRAME_SIZE                         ((uint32_t)16876)
#define BDSP_IMG_VORBIS_IDS_SIZE                                     ((uint32_t)5312)
#define BDSP_IMG_VP6_DECODE_INTER_FRAME_SIZE                         ((uint32_t)4472)
#define BDSP_IMG_VP6_DECODE_SIZE                                     ((uint32_t)74552)
#define BDSP_IMG_VP6_DECODE_TABLES_SIZE                              ((uint32_t)6824)
#define BDSP_IMG_VP6_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_VP6_IDS_SIZE                                        ((uint32_t)2364)
#define BDSP_IMG_WMA_DECODE_INTER_FRAME_SIZE                         ((uint32_t)32908)
#define BDSP_IMG_WMA_DECODE_SIZE                                     ((uint32_t)64224)
#define BDSP_IMG_WMA_DECODE_TABLES_SIZE                              ((uint32_t)36144)
#define BDSP_IMG_WMA_IDS_INTER_FRAME_SIZE                            ((uint32_t)16876)
#define BDSP_IMG_WMA_IDS_SIZE                                        ((uint32_t)5388)
#define BDSP_IMG_WMA_PRO_DECODE_INTER_FRAME_SIZE                     ((uint32_t)267936)
#define BDSP_IMG_WMA_PRO_DECODE_SIZE                                 ((uint32_t)313236)
#define BDSP_IMG_WMA_PRO_DECODE_TABLES_SIZE                          ((uint32_t)117468)
#define BDSP_IMG_WMA_PRO_IDS_INTER_FRAME_SIZE                        ((uint32_t)16876)
#define BDSP_IMG_WMA_PRO_IDS_SIZE                                    ((uint32_t)5216)
#define BDSP_IMG_X264_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)38704)
#define BDSP_IMG_X264_ENCODE_SIZE                                    ((uint32_t)8)
#define BDSP_IMG_X264_ENCODE_TABLES_SIZE                             ((uint32_t)201444)
#define BDSP_IMG_XVP8_ENCODE_INTER_FRAME_SIZE                        ((uint32_t)3123208)
#define BDSP_IMG_XVP8_ENCODE_SIZE                                    ((uint32_t)8)
#define BDSP_IMG_XVP8_ENCODE_TABLES_SIZE                             ((uint32_t)23140)
#define BDSP_IMG_SYSTEM_RDBVARS_SIZE 								 ((uint32_t)1024)
#define BDSP_IMG_SYSTEM_SCM1_DIGEST_SIZE 							 ((uint32_t)20)
#define BDSP_IMG_SYSTEM_SCM2_DIGEST_SIZE 							 ((uint32_t)20)
