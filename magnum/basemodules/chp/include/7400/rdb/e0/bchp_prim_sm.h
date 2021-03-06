/***************************************************************************
 *     Copyright (c) 1999-2008, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Wed Jan  9 09:11:00 2008
 *                 MD5 Checksum         847dc12a9d71c4c68a648bbf19a883e3
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_PRIM_SM_H__
#define BCHP_PRIM_SM_H__

/***************************************************************************
 *PRIM_SM - Primary Subcarrier Modulator
 ***************************************************************************/
#define BCHP_PRIM_SM_SM_REV_ID                   0x00180e00 /* REVISION ID REGISTER */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR          0x00180e08 /* ENVELOPE GENERATOR CONTROL REGISTER */
#define BCHP_PRIM_SM_PG_CNTRL                    0x00180e0c /* PHASE GENERATOR CONTROL REGISTER */
#define BCHP_PRIM_SM_PG_CONFIG                   0x00180e10 /* PHASE GENERATOR CONFIG REGISTER */
#define BCHP_PRIM_SM_SC_FREQ_0                   0x00180e14 /* SUBCARRIER FREQUENCY 0 */
#define BCHP_PRIM_SM_COMP_CNTRL                  0x00180e18 /* COMPOSITOR CONTROL REGISTER */
#define BCHP_PRIM_SM_SA_CH2                      0x00180e1c /* CHANNEL2 SA STATUS REGISTER */
#define BCHP_PRIM_SM_SA_CH1                      0x00180e20 /* CHANNEL1 SA STATUS REGISTER */
#define BCHP_PRIM_SM_SA_CH0                      0x00180e24 /* CHANNEL0 SA STATUS REGISTER */
#define BCHP_PRIM_SM_SA_CONFIG                   0x00180e28 /* SA CONFIGURATION REGISTER */
#define BCHP_PRIM_SM_COMP_CONFIG                 0x00180e2c /* COMPOSITOR CONFIG REGISTER */
#define BCHP_PRIM_SM_FM_CONTROL                  0x00180e30 /* FM Control Register */
#define BCHP_PRIM_SM_FM_FMAMP                    0x00180e34 /* FM AMP Register */
#define BCHP_PRIM_SM_FM_SHAPER                   0x00180e38 /* FM Shaper Register */
#define BCHP_PRIM_SM_FM_SC_FREQ                  0x00180e3c /* FM Base Sub-carrier Frequency */

/***************************************************************************
 *SM_REV_ID - REVISION ID REGISTER
 ***************************************************************************/
/* PRIM_SM :: SM_REV_ID :: reserved0 [31:16] */
#define BCHP_PRIM_SM_SM_REV_ID_reserved0_MASK                      0xffff0000
#define BCHP_PRIM_SM_SM_REV_ID_reserved0_SHIFT                     16

/* PRIM_SM :: SM_REV_ID :: REVISION_ID [15:00] */
#define BCHP_PRIM_SM_SM_REV_ID_REVISION_ID_MASK                    0x0000ffff
#define BCHP_PRIM_SM_SM_REV_ID_REVISION_ID_SHIFT                   0

/***************************************************************************
 *ENVELOPE_GENERATOR - ENVELOPE GENERATOR CONTROL REGISTER
 ***************************************************************************/
/* PRIM_SM :: ENVELOPE_GENERATOR :: reserved0 [31:28] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_reserved0_MASK             0xf0000000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_reserved0_SHIFT            28

/* PRIM_SM :: ENVELOPE_GENERATOR :: AMP_CONTROL [27:26] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_MASK           0x0c000000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_SHIFT          26
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_CONSTANT       0
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_INC_BY_HALF    1
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_INC_BY_ONE     2
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CONTROL_INC_BY_ONE_AND_HALF 3

/* PRIM_SM :: ENVELOPE_GENERATOR :: U_BURST_AMP_SEL [25:25] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_BURST_AMP_SEL_MASK       0x02000000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_BURST_AMP_SEL_SHIFT      25
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_BURST_AMP_SEL_POSITIVE   1
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_BURST_AMP_SEL_NEGATIVE   0

/* PRIM_SM :: ENVELOPE_GENERATOR :: V_BURST_AMP_SEL [24:24] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_BURST_AMP_SEL_MASK       0x01000000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_BURST_AMP_SEL_SHIFT      24
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_BURST_AMP_SEL_POSITIVE   1
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_BURST_AMP_SEL_NEGATIVE   0

/* PRIM_SM :: ENVELOPE_GENERATOR :: U_ENABLE [23:23] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_ENABLE_MASK              0x00800000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_ENABLE_SHIFT             23
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_ENABLE_ON                1
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_U_ENABLE_OFF               0

/* PRIM_SM :: ENVELOPE_GENERATOR :: V_ENABLE [22:22] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_ENABLE_MASK              0x00400000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_ENABLE_SHIFT             22
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_ENABLE_ON                1
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_V_ENABLE_OFF               0

/* PRIM_SM :: ENVELOPE_GENERATOR :: INIT_AMP [21:12] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_INIT_AMP_MASK              0x003ff000
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_INIT_AMP_SHIFT             12

/* PRIM_SM :: ENVELOPE_GENERATOR :: reserved1 [11:10] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_reserved1_MASK             0x00000c00
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_reserved1_SHIFT            10

/* PRIM_SM :: ENVELOPE_GENERATOR :: AMP_CLAMP [09:00] */
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CLAMP_MASK             0x000003ff
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CLAMP_SHIFT            0
#define BCHP_PRIM_SM_ENVELOPE_GENERATOR_AMP_CLAMP_SECAM            424

/***************************************************************************
 *PG_CNTRL - PHASE GENERATOR CONTROL REGISTER
 ***************************************************************************/
/* PRIM_SM :: PG_CNTRL :: reserved0 [31:21] */
#define BCHP_PRIM_SM_PG_CNTRL_reserved0_MASK                       0xffe00000
#define BCHP_PRIM_SM_PG_CNTRL_reserved0_SHIFT                      21

/* PRIM_SM :: PG_CNTRL :: PIXEL_FRAC_ENABLE [20:20] */
#define BCHP_PRIM_SM_PG_CNTRL_PIXEL_FRAC_ENABLE_MASK               0x00100000
#define BCHP_PRIM_SM_PG_CNTRL_PIXEL_FRAC_ENABLE_SHIFT              20
#define BCHP_PRIM_SM_PG_CNTRL_PIXEL_FRAC_ENABLE_ON                 1
#define BCHP_PRIM_SM_PG_CNTRL_PIXEL_FRAC_ENABLE_OFF                0

/* PRIM_SM :: PG_CNTRL :: INIT_PHASE [19:10] */
#define BCHP_PRIM_SM_PG_CNTRL_INIT_PHASE_MASK                      0x000ffc00
#define BCHP_PRIM_SM_PG_CNTRL_INIT_PHASE_SHIFT                     10

/* PRIM_SM :: PG_CNTRL :: ACTIVE_PHASE_OFFSET [09:00] */
#define BCHP_PRIM_SM_PG_CNTRL_ACTIVE_PHASE_OFFSET_MASK             0x000003ff
#define BCHP_PRIM_SM_PG_CNTRL_ACTIVE_PHASE_OFFSET_SHIFT            0

/***************************************************************************
 *PG_CONFIG - PHASE GENERATOR CONFIG REGISTER
 ***************************************************************************/
/* PRIM_SM :: PG_CONFIG :: reserved0 [31:05] */
#define BCHP_PRIM_SM_PG_CONFIG_reserved0_MASK                      0xffffffe0
#define BCHP_PRIM_SM_PG_CONFIG_reserved0_SHIFT                     5

/* PRIM_SM :: PG_CONFIG :: U_CORING_ENABLE [04:04] */
#define BCHP_PRIM_SM_PG_CONFIG_U_CORING_ENABLE_MASK                0x00000010
#define BCHP_PRIM_SM_PG_CONFIG_U_CORING_ENABLE_SHIFT               4
#define BCHP_PRIM_SM_PG_CONFIG_U_CORING_ENABLE_ON                  1
#define BCHP_PRIM_SM_PG_CONFIG_U_CORING_ENABLE_OFF                 0

/* PRIM_SM :: PG_CONFIG :: V_CORING_ENABLE [03:03] */
#define BCHP_PRIM_SM_PG_CONFIG_V_CORING_ENABLE_MASK                0x00000008
#define BCHP_PRIM_SM_PG_CONFIG_V_CORING_ENABLE_SHIFT               3
#define BCHP_PRIM_SM_PG_CONFIG_V_CORING_ENABLE_ON                  1
#define BCHP_PRIM_SM_PG_CONFIG_V_CORING_ENABLE_OFF                 0

/* PRIM_SM :: PG_CONFIG :: reserved1 [02:00] */
#define BCHP_PRIM_SM_PG_CONFIG_reserved1_MASK                      0x00000007
#define BCHP_PRIM_SM_PG_CONFIG_reserved1_SHIFT                     0

/***************************************************************************
 *SC_FREQ_0 - SUBCARRIER FREQUENCY 0
 ***************************************************************************/
/* PRIM_SM :: SC_FREQ_0 :: FRE0 [31:00] */
#define BCHP_PRIM_SM_SC_FREQ_0_FRE0_MASK                           0xffffffff
#define BCHP_PRIM_SM_SC_FREQ_0_FRE0_SHIFT                          0

/***************************************************************************
 *COMP_CNTRL - COMPOSITOR CONTROL REGISTER
 ***************************************************************************/
/* PRIM_SM :: COMP_CNTRL :: reserved0 [31:28] */
#define BCHP_PRIM_SM_COMP_CNTRL_reserved0_MASK                     0xf0000000
#define BCHP_PRIM_SM_COMP_CNTRL_reserved0_SHIFT                    28

/* PRIM_SM :: COMP_CNTRL :: CHROMA_OFFSET [27:16] */
#define BCHP_PRIM_SM_COMP_CNTRL_CHROMA_OFFSET_MASK                 0x0fff0000
#define BCHP_PRIM_SM_COMP_CNTRL_CHROMA_OFFSET_SHIFT                16

/* PRIM_SM :: COMP_CNTRL :: reserved1 [15:06] */
#define BCHP_PRIM_SM_COMP_CNTRL_reserved1_MASK                     0x0000ffc0
#define BCHP_PRIM_SM_COMP_CNTRL_reserved1_SHIFT                    6

/* PRIM_SM :: COMP_CNTRL :: COMP_OUT_SEL [05:04] */
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_MASK                  0x00000030
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_SHIFT                 4
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_COMPONENT             0
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_CHROMA_OUT            1
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_Y_AND_SIN             2
#define BCHP_PRIM_SM_COMP_CNTRL_COMP_OUT_SEL_COMPOSITE             3

/* PRIM_SM :: COMP_CNTRL :: COMBINE_CHROMA [03:03] */
#define BCHP_PRIM_SM_COMP_CNTRL_COMBINE_CHROMA_MASK                0x00000008
#define BCHP_PRIM_SM_COMP_CNTRL_COMBINE_CHROMA_SHIFT               3
#define BCHP_PRIM_SM_COMP_CNTRL_COMBINE_CHROMA_ON                  1
#define BCHP_PRIM_SM_COMP_CNTRL_COMBINE_CHROMA_OFF                 0

/* PRIM_SM :: COMP_CNTRL :: reserved2 [02:02] */
#define BCHP_PRIM_SM_COMP_CNTRL_reserved2_MASK                     0x00000004
#define BCHP_PRIM_SM_COMP_CNTRL_reserved2_SHIFT                    2

/* PRIM_SM :: COMP_CNTRL :: SIN_COEF_EN [01:01] */
#define BCHP_PRIM_SM_COMP_CNTRL_SIN_COEF_EN_MASK                   0x00000002
#define BCHP_PRIM_SM_COMP_CNTRL_SIN_COEF_EN_SHIFT                  1
#define BCHP_PRIM_SM_COMP_CNTRL_SIN_COEF_EN_USE_SIN                1
#define BCHP_PRIM_SM_COMP_CNTRL_SIN_COEF_EN_USE_ONE                0

/* PRIM_SM :: COMP_CNTRL :: COS_COEF_EN [00:00] */
#define BCHP_PRIM_SM_COMP_CNTRL_COS_COEF_EN_MASK                   0x00000001
#define BCHP_PRIM_SM_COMP_CNTRL_COS_COEF_EN_SHIFT                  0
#define BCHP_PRIM_SM_COMP_CNTRL_COS_COEF_EN_USE_COS                1
#define BCHP_PRIM_SM_COMP_CNTRL_COS_COEF_EN_USE_ONE                0

/***************************************************************************
 *SA_CH2 - CHANNEL2 SA STATUS REGISTER
 ***************************************************************************/
/* PRIM_SM :: SA_CH2 :: STATUS [31:00] */
#define BCHP_PRIM_SM_SA_CH2_STATUS_MASK                            0xffffffff
#define BCHP_PRIM_SM_SA_CH2_STATUS_SHIFT                           0

/***************************************************************************
 *SA_CH1 - CHANNEL1 SA STATUS REGISTER
 ***************************************************************************/
/* PRIM_SM :: SA_CH1 :: STATUS [31:00] */
#define BCHP_PRIM_SM_SA_CH1_STATUS_MASK                            0xffffffff
#define BCHP_PRIM_SM_SA_CH1_STATUS_SHIFT                           0

/***************************************************************************
 *SA_CH0 - CHANNEL0 SA STATUS REGISTER
 ***************************************************************************/
/* PRIM_SM :: SA_CH0 :: STATUS [31:00] */
#define BCHP_PRIM_SM_SA_CH0_STATUS_MASK                            0xffffffff
#define BCHP_PRIM_SM_SA_CH0_STATUS_SHIFT                           0

/***************************************************************************
 *SA_CONFIG - SA CONFIGURATION REGISTER
 ***************************************************************************/
/* PRIM_SM :: SA_CONFIG :: reserved0 [31:11] */
#define BCHP_PRIM_SM_SA_CONFIG_reserved0_MASK                      0xfffff800
#define BCHP_PRIM_SM_SA_CONFIG_reserved0_SHIFT                     11

/* PRIM_SM :: SA_CONFIG :: EDGE_SELECT [10:09] */
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_MASK                    0x00000600
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_SHIFT                   9
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_RISE_EDGE               0
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_FALL_EDGE               1
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_BOTH_EDGES              2
#define BCHP_PRIM_SM_SA_CONFIG_EDGE_SELECT_NOT_DEFINED             3

/* PRIM_SM :: SA_CONFIG :: SA_CH0_EN [08:08] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_EN_MASK                      0x00000100
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_EN_SHIFT                     8
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_EN_ON                        1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_EN_OFF                       0

/* PRIM_SM :: SA_CONFIG :: SA_CH1_EN [07:07] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_EN_MASK                      0x00000080
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_EN_SHIFT                     7
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_EN_ON                        1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_EN_OFF                       0

/* PRIM_SM :: SA_CONFIG :: SA_CH2_EN [06:06] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_EN_MASK                      0x00000040
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_EN_SHIFT                     6
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_EN_ON                        1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_EN_OFF                       0

/* PRIM_SM :: SA_CONFIG :: SA_CH0_CLEAR [05:05] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_CLEAR_MASK                   0x00000020
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_CLEAR_SHIFT                  5
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_CLEAR_ON                     1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH0_CLEAR_OFF                    0

/* PRIM_SM :: SA_CONFIG :: SA_CH1_CLEAR [04:04] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_CLEAR_MASK                   0x00000010
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_CLEAR_SHIFT                  4
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_CLEAR_ON                     1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH1_CLEAR_OFF                    0

/* PRIM_SM :: SA_CONFIG :: SA_CH2_CLEAR [03:03] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_CLEAR_MASK                   0x00000008
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_CLEAR_SHIFT                  3
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_CLEAR_ON                     1
#define BCHP_PRIM_SM_SA_CONFIG_SA_CH2_CLEAR_OFF                    0

/* PRIM_SM :: SA_CONFIG :: SA_PROBE_RATE [02:00] */
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_MASK                  0x00000007
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_SHIFT                 0
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_ZERO                  0
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_WINDOW            1
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_2WINDOWS          2
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_3WINDOWS          3
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_4WINDOWS          4
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_5WINDOWS          5
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_6WINDOWS          6
#define BCHP_PRIM_SM_SA_CONFIG_SA_PROBE_RATE_PER_7WINDOWS          7

/***************************************************************************
 *COMP_CONFIG - COMPOSITOR CONFIG REGISTER
 ***************************************************************************/
/* PRIM_SM :: COMP_CONFIG :: reserved0 [31:17] */
#define BCHP_PRIM_SM_COMP_CONFIG_reserved0_MASK                    0xfffe0000
#define BCHP_PRIM_SM_COMP_CONFIG_reserved0_SHIFT                   17

/* PRIM_SM :: COMP_CONFIG :: COMPOSITE_CLAMP_SEL [16:16] */
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_CLAMP_SEL_MASK          0x00010000
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_CLAMP_SEL_SHIFT         16
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_CLAMP_SEL_CLAMP         1
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_CLAMP_SEL_NO_CLAMP      0

/* PRIM_SM :: COMP_CONFIG :: U_LINE_SEL [15:15] */
#define BCHP_PRIM_SM_COMP_CONFIG_U_LINE_SEL_MASK                   0x00008000
#define BCHP_PRIM_SM_COMP_CONFIG_U_LINE_SEL_SHIFT                  15
#define BCHP_PRIM_SM_COMP_CONFIG_U_LINE_SEL_ALT                    1
#define BCHP_PRIM_SM_COMP_CONFIG_U_LINE_SEL_FIXED                  0

/* PRIM_SM :: COMP_CONFIG :: V_LINE_SEL [14:14] */
#define BCHP_PRIM_SM_COMP_CONFIG_V_LINE_SEL_MASK                   0x00004000
#define BCHP_PRIM_SM_COMP_CONFIG_V_LINE_SEL_SHIFT                  14
#define BCHP_PRIM_SM_COMP_CONFIG_V_LINE_SEL_ALT                    1
#define BCHP_PRIM_SM_COMP_CONFIG_V_LINE_SEL_FIXED                  0

/* PRIM_SM :: COMP_CONFIG :: U_FIXED_LINE [13:13] */
#define BCHP_PRIM_SM_COMP_CONFIG_U_FIXED_LINE_MASK                 0x00002000
#define BCHP_PRIM_SM_COMP_CONFIG_U_FIXED_LINE_SHIFT                13
#define BCHP_PRIM_SM_COMP_CONFIG_U_FIXED_LINE_ONE                  1
#define BCHP_PRIM_SM_COMP_CONFIG_U_FIXED_LINE_ZERO                 0

/* PRIM_SM :: COMP_CONFIG :: V_FIXED_LINE [12:12] */
#define BCHP_PRIM_SM_COMP_CONFIG_V_FIXED_LINE_MASK                 0x00001000
#define BCHP_PRIM_SM_COMP_CONFIG_V_FIXED_LINE_SHIFT                12
#define BCHP_PRIM_SM_COMP_CONFIG_V_FIXED_LINE_ONE                  1
#define BCHP_PRIM_SM_COMP_CONFIG_V_FIXED_LINE_ZERO                 0

/* PRIM_SM :: COMP_CONFIG :: COMPOSITE_OFFSET [11:00] */
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_OFFSET_MASK             0x00000fff
#define BCHP_PRIM_SM_COMP_CONFIG_COMPOSITE_OFFSET_SHIFT            0

/***************************************************************************
 *FM_CONTROL - FM Control Register
 ***************************************************************************/
/* PRIM_SM :: FM_CONTROL :: reserved_for_eco0 [31:19] */
#define BCHP_PRIM_SM_FM_CONTROL_reserved_for_eco0_MASK             0xfff80000
#define BCHP_PRIM_SM_FM_CONTROL_reserved_for_eco0_SHIFT            19

/* PRIM_SM :: FM_CONTROL :: FINE_LUMA_DELAY [18:17] */
#define BCHP_PRIM_SM_FM_CONTROL_FINE_LUMA_DELAY_MASK               0x00060000
#define BCHP_PRIM_SM_FM_CONTROL_FINE_LUMA_DELAY_SHIFT              17

/* PRIM_SM :: FM_CONTROL :: reserved1 [16:15] */
#define BCHP_PRIM_SM_FM_CONTROL_reserved1_MASK                     0x00018000
#define BCHP_PRIM_SM_FM_CONTROL_reserved1_SHIFT                    15

/* PRIM_SM :: FM_CONTROL :: GROSS_LUMA_DELAY [14:10] */
#define BCHP_PRIM_SM_FM_CONTROL_GROSS_LUMA_DELAY_MASK              0x00007c00
#define BCHP_PRIM_SM_FM_CONTROL_GROSS_LUMA_DELAY_SHIFT             10

/* PRIM_SM :: FM_CONTROL :: FINE_SC_DELAY [09:08] */
#define BCHP_PRIM_SM_FM_CONTROL_FINE_SC_DELAY_MASK                 0x00000300
#define BCHP_PRIM_SM_FM_CONTROL_FINE_SC_DELAY_SHIFT                8

/* PRIM_SM :: FM_CONTROL :: reserved2 [07:06] */
#define BCHP_PRIM_SM_FM_CONTROL_reserved2_MASK                     0x000000c0
#define BCHP_PRIM_SM_FM_CONTROL_reserved2_SHIFT                    6

/* PRIM_SM :: FM_CONTROL :: GROSS_SC_DELAY [05:01] */
#define BCHP_PRIM_SM_FM_CONTROL_GROSS_SC_DELAY_MASK                0x0000003e
#define BCHP_PRIM_SM_FM_CONTROL_GROSS_SC_DELAY_SHIFT               1

/* PRIM_SM :: FM_CONTROL :: ENABLE [00:00] */
#define BCHP_PRIM_SM_FM_CONTROL_ENABLE_MASK                        0x00000001
#define BCHP_PRIM_SM_FM_CONTROL_ENABLE_SHIFT                       0
#define BCHP_PRIM_SM_FM_CONTROL_ENABLE_ON                          1
#define BCHP_PRIM_SM_FM_CONTROL_ENABLE_OFF                         0

/***************************************************************************
 *FM_FMAMP - FM AMP Register
 ***************************************************************************/
/* PRIM_SM :: FM_FMAMP :: reserved0 [31:26] */
#define BCHP_PRIM_SM_FM_FMAMP_reserved0_MASK                       0xfc000000
#define BCHP_PRIM_SM_FM_FMAMP_reserved0_SHIFT                      26

/* PRIM_SM :: FM_FMAMP :: LOWER_LIMIT [25:18] */
#define BCHP_PRIM_SM_FM_FMAMP_LOWER_LIMIT_MASK                     0x03fc0000
#define BCHP_PRIM_SM_FM_FMAMP_LOWER_LIMIT_SHIFT                    18

/* PRIM_SM :: FM_FMAMP :: UPPER_LIMIT [17:10] */
#define BCHP_PRIM_SM_FM_FMAMP_UPPER_LIMIT_MASK                     0x0003fc00
#define BCHP_PRIM_SM_FM_FMAMP_UPPER_LIMIT_SHIFT                    10

/* PRIM_SM :: FM_FMAMP :: SLOPE_ADJUST [09:07] */
#define BCHP_PRIM_SM_FM_FMAMP_SLOPE_ADJUST_MASK                    0x00000380
#define BCHP_PRIM_SM_FM_FMAMP_SLOPE_ADJUST_SHIFT                   7

/* PRIM_SM :: FM_FMAMP :: SCALE [06:00] */
#define BCHP_PRIM_SM_FM_FMAMP_SCALE_MASK                           0x0000007f
#define BCHP_PRIM_SM_FM_FMAMP_SCALE_SHIFT                          0

/***************************************************************************
 *FM_SHAPER - FM Shaper Register
 ***************************************************************************/
/* PRIM_SM :: FM_SHAPER :: reserved0 [31:16] */
#define BCHP_PRIM_SM_FM_SHAPER_reserved0_MASK                      0xffff0000
#define BCHP_PRIM_SM_FM_SHAPER_reserved0_SHIFT                     16

/* PRIM_SM :: FM_SHAPER :: FMAMP_COEFF1 [15:08] */
#define BCHP_PRIM_SM_FM_SHAPER_FMAMP_COEFF1_MASK                   0x0000ff00
#define BCHP_PRIM_SM_FM_SHAPER_FMAMP_COEFF1_SHIFT                  8

/* PRIM_SM :: FM_SHAPER :: FMAMP_COEFF0 [07:00] */
#define BCHP_PRIM_SM_FM_SHAPER_FMAMP_COEFF0_MASK                   0x000000ff
#define BCHP_PRIM_SM_FM_SHAPER_FMAMP_COEFF0_SHIFT                  0

/***************************************************************************
 *FM_SC_FREQ - FM Base Sub-carrier Frequency
 ***************************************************************************/
/* PRIM_SM :: FM_SC_FREQ :: MIN_FREQ_CLAMP [31:21] */
#define BCHP_PRIM_SM_FM_SC_FREQ_MIN_FREQ_CLAMP_MASK                0xffe00000
#define BCHP_PRIM_SM_FM_SC_FREQ_MIN_FREQ_CLAMP_SHIFT               21

/* PRIM_SM :: FM_SC_FREQ :: MAX_FREQ_CLAMP [20:10] */
#define BCHP_PRIM_SM_FM_SC_FREQ_MAX_FREQ_CLAMP_MASK                0x001ffc00
#define BCHP_PRIM_SM_FM_SC_FREQ_MAX_FREQ_CLAMP_SHIFT               10

/* PRIM_SM :: FM_SC_FREQ :: FREQ_DIFF [09:00] */
#define BCHP_PRIM_SM_FM_SC_FREQ_FREQ_DIFF_MASK                     0x000003ff
#define BCHP_PRIM_SM_FM_SC_FREQ_FREQ_DIFF_SHIFT                    0

#endif /* #ifndef BCHP_PRIM_SM_H__ */

/* End of File */
