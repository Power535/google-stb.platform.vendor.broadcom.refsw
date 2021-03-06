/***************************************************************************
 *     Copyright (c) 1999-2010, Broadcom Corporation
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
 * Date:           Generated on         Fri Jan 22 20:21:36 2010
 *                 MD5 Checksum         a2d1f2163f65e87d228a0fb491cb442d
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

#ifndef BCHP_DS_TUNER_REF_0_H__
#define BCHP_DS_TUNER_REF_0_H__

/***************************************************************************
 *DS_TUNER_REF_0 - Tuner Reference PLL Control Registers 0
 ***************************************************************************/
#define BCHP_DS_TUNER_REF_0_REF_00               0x04c31000 /* Analog Tuner Reference PLL Control Register */
#define BCHP_DS_TUNER_REF_0_REF_01               0x04c31004 /* Analog Tuner Reference PLL Control Register */
#define BCHP_DS_TUNER_REF_0_REF_02               0x04c31008 /* Analog Tuner Reference PLL Control Register */
#define BCHP_DS_TUNER_REF_0_REF_03               0x04c3100c /* Analog Tuner Reference PLL Control Register */
#define BCHP_DS_TUNER_REF_0_REF_04               0x04c31010 /* Analog Tuner Reference PLL Control Register */
#define BCHP_DS_TUNER_REF_0_SPLIT_00             0x04c31014 /* Analog Tuner Splitter Control Register */
#define BCHP_DS_TUNER_REF_0_SPARE_00             0x04c31018 /* Analog Tuner Reference PLL Control Register */

/***************************************************************************
 *REF_00 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: REF_00 :: SPARE0 [31:31] */
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE0_MASK                     0x80000000
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE0_SHIFT                    31

/* DS_TUNER_REF_0 :: REF_00 :: I_CLFCNT [30:29] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_CLFCNT_MASK                   0x60000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_CLFCNT_SHIFT                  29

/* DS_TUNER_REF_0 :: REF_00 :: I_VCO_PWRUP [28:28] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_VCO_PWRUP_MASK                0x10000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_VCO_PWRUP_SHIFT               28

/* DS_TUNER_REF_0 :: REF_00 :: I_VCOBUF_LATCH_ON [27:27] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_VCOBUF_LATCH_ON_MASK          0x08000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_VCOBUF_LATCH_ON_SHIFT         27

/* DS_TUNER_REF_0 :: REF_00 :: I_DIV_FBREG_PWRDN [26:26] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FBREG_PWRDN_MASK          0x04000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FBREG_PWRDN_SHIFT         26

/* DS_TUNER_REF_0 :: REF_00 :: I_DIV4_PWRDN [25:25] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV4_PWRDN_MASK               0x02000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV4_PWRDN_SHIFT              25

/* DS_TUNER_REF_0 :: REF_00 :: I_DIV_FB_PWRDN [24:24] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FB_PWRDN_MASK             0x01000000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FB_PWRDN_SHIFT            24

/* DS_TUNER_REF_0 :: REF_00 :: I_DIV_REFBUF_PWRDN [23:23] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_REFBUF_PWRDN_MASK         0x00800000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_REFBUF_PWRDN_SHIFT        23

/* DS_TUNER_REF_0 :: REF_00 :: I_DAC_DIV2RST [22:22] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_DIV2RST_MASK              0x00400000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_DIV2RST_SHIFT             22

/* DS_TUNER_REF_0 :: REF_00 :: I_DAC_DIV4RST [21:21] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_DIV4RST_MASK              0x00200000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_DIV4RST_SHIFT             21

/* DS_TUNER_REF_0 :: REF_00 :: I_DAC_FBDIVRST [20:20] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_FBDIVRST_MASK             0x00100000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_FBDIVRST_SHIFT            20

/* DS_TUNER_REF_0 :: REF_00 :: I_DAC_VCRST [19:19] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_VCRST_MASK                0x00080000
#define BCHP_DS_TUNER_REF_0_REF_00_I_DAC_VCRST_SHIFT               19

/* DS_TUNER_REF_0 :: REF_00 :: I_REF_CMLBIAS [18:17] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_REF_CMLBIAS_MASK              0x00060000
#define BCHP_DS_TUNER_REF_0_REF_00_I_REF_CMLBIAS_SHIFT             17

/* DS_TUNER_REF_0 :: REF_00 :: I_DIV_FB [16:09] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FB_MASK                   0x0001fe00
#define BCHP_DS_TUNER_REF_0_REF_00_I_DIV_FB_SHIFT                  9

/* DS_TUNER_REF_0 :: REF_00 :: I_LD_EN [08:08] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_LD_EN_MASK                    0x00000100
#define BCHP_DS_TUNER_REF_0_REF_00_I_LD_EN_SHIFT                   8

/* DS_TUNER_REF_0 :: REF_00 :: LD_RESET_STRT [07:07] */
#define BCHP_DS_TUNER_REF_0_REF_00_LD_RESET_STRT_MASK              0x00000080
#define BCHP_DS_TUNER_REF_0_REF_00_LD_RESET_STRT_SHIFT             7

/* DS_TUNER_REF_0 :: REF_00 :: SPARE1 [06:06] */
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE1_MASK                     0x00000040
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE1_SHIFT                    6

/* DS_TUNER_REF_0 :: REF_00 :: SPARE2 [05:05] */
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE2_MASK                     0x00000020
#define BCHP_DS_TUNER_REF_0_REF_00_SPARE2_SHIFT                    5

/* DS_TUNER_REF_0 :: REF_00 :: I_UGB_PWRUP [04:04] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_UGB_PWRUP_MASK                0x00000010
#define BCHP_DS_TUNER_REF_0_REF_00_I_UGB_PWRUP_SHIFT               4

/* DS_TUNER_REF_0 :: REF_00 :: I_MUX_SEL [03:02] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_SEL_MASK                  0x0000000c
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_SEL_SHIFT                 2

/* DS_TUNER_REF_0 :: REF_00 :: I_MUX_FBOFF [01:01] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_FBOFF_MASK                0x00000002
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_FBOFF_SHIFT               1

/* DS_TUNER_REF_0 :: REF_00 :: I_MUX_PWRUP [00:00] */
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_PWRUP_MASK                0x00000001
#define BCHP_DS_TUNER_REF_0_REF_00_I_MUX_PWRUP_SHIFT               0

/***************************************************************************
 *REF_01 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: REF_01 :: SPARE0 [31:29] */
#define BCHP_DS_TUNER_REF_0_REF_01_SPARE0_MASK                     0xe0000000
#define BCHP_DS_TUNER_REF_0_REF_01_SPARE0_SHIFT                    29

/* DS_TUNER_REF_0 :: REF_01 :: I_OUTDIV_PWRDN_M [28:22] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_PWRDN_M_MASK           0x1fc00000
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_PWRDN_M_SHIFT          22

/* DS_TUNER_REF_0 :: REF_01 :: I_OUTDIV_REG_PWRDN [21:21] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_REG_PWRDN_MASK         0x00200000
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_REG_PWRDN_SHIFT        21

/* DS_TUNER_REF_0 :: REF_01 :: I_OUTDIV_RESET_M [20:14] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_RESET_M_MASK           0x001fc000
#define BCHP_DS_TUNER_REF_0_REF_01_I_OUTDIV_RESET_M_SHIFT          14

/* DS_TUNER_REF_0 :: REF_01 :: SPARE1 [13:11] */
#define BCHP_DS_TUNER_REF_0_REF_01_SPARE1_MASK                     0x00003800
#define BCHP_DS_TUNER_REF_0_REF_01_SPARE1_SHIFT                    11

/* DS_TUNER_REF_0 :: REF_01 :: I_QP_PWRDN [10:10] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_QP_PWRDN_MASK                 0x00000400
#define BCHP_DS_TUNER_REF_0_REF_01_I_QP_PWRDN_SHIFT                10

/* DS_TUNER_REF_0 :: REF_01 :: I_REGQPPFD_PWRDN [09:09] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_REGQPPFD_PWRDN_MASK           0x00000200
#define BCHP_DS_TUNER_REF_0_REF_01_I_REGQPPFD_PWRDN_SHIFT          9

/* DS_TUNER_REF_0 :: REF_01 :: I_PFD_RESET [08:08] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_PFD_RESET_MASK                0x00000100
#define BCHP_DS_TUNER_REF_0_REF_01_I_PFD_RESET_SHIFT               8

/* DS_TUNER_REF_0 :: REF_01 :: I_QP_ICTRL [07:05] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_QP_ICTRL_MASK                 0x000000e0
#define BCHP_DS_TUNER_REF_0_REF_01_I_QP_ICTRL_SHIFT                5

/* DS_TUNER_REF_0 :: REF_01 :: I_PFD_DLY [04:03] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_PFD_DLY_MASK                  0x00000018
#define BCHP_DS_TUNER_REF_0_REF_01_I_PFD_DLY_SHIFT                 3

/* DS_TUNER_REF_0 :: REF_01 :: I_LF_RCNTL [02:00] */
#define BCHP_DS_TUNER_REF_0_REF_01_I_LF_RCNTL_MASK                 0x00000007
#define BCHP_DS_TUNER_REF_0_REF_01_I_LF_RCNTL_SHIFT                0

/***************************************************************************
 *REF_02 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: REF_02 :: SPARE0 [31:29] */
#define BCHP_DS_TUNER_REF_0_REF_02_SPARE0_MASK                     0xe0000000
#define BCHP_DS_TUNER_REF_0_REF_02_SPARE0_SHIFT                    29

/* DS_TUNER_REF_0 :: REF_02 :: I_OUTDIV_M0 [28:21] */
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTDIV_M0_MASK                0x1fe00000
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTDIV_M0_SHIFT               21

/* DS_TUNER_REF_0 :: REF_02 :: I_CLKEN_M [20:14] */
#define BCHP_DS_TUNER_REF_0_REF_02_I_CLKEN_M_MASK                  0x001fc000
#define BCHP_DS_TUNER_REF_0_REF_02_I_CLKEN_M_SHIFT                 14

/* DS_TUNER_REF_0 :: REF_02 :: I_OUTCMOS_PWRDN_M [13:07] */
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTCMOS_PWRDN_M_MASK          0x00003f80
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTCMOS_PWRDN_M_SHIFT         7

/* DS_TUNER_REF_0 :: REF_02 :: I_OUTCML_PWRDN_M [06:00] */
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTCML_PWRDN_M_MASK           0x0000007f
#define BCHP_DS_TUNER_REF_0_REF_02_I_OUTCML_PWRDN_M_SHIFT          0

/***************************************************************************
 *REF_03 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: REF_03 :: I_OUTDIV_M4 [31:24] */
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M4_MASK                0xff000000
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M4_SHIFT               24

/* DS_TUNER_REF_0 :: REF_03 :: I_OUTDIV_M3 [23:16] */
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M3_MASK                0x00ff0000
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M3_SHIFT               16

/* DS_TUNER_REF_0 :: REF_03 :: I_OUTDIV_M2 [15:08] */
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M2_MASK                0x0000ff00
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M2_SHIFT               8

/* DS_TUNER_REF_0 :: REF_03 :: I_OUTDIV_M1 [07:00] */
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M1_MASK                0x000000ff
#define BCHP_DS_TUNER_REF_0_REF_03_I_OUTDIV_M1_SHIFT               0

/***************************************************************************
 *REF_04 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: REF_04 :: SPARE0 [31:28] */
#define BCHP_DS_TUNER_REF_0_REF_04_SPARE0_MASK                     0xf0000000
#define BCHP_DS_TUNER_REF_0_REF_04_SPARE0_SHIFT                    28

/* DS_TUNER_REF_0 :: REF_04 :: CLK_BYP_EN_GPHY [27:27] */
#define BCHP_DS_TUNER_REF_0_REF_04_CLK_BYP_EN_GPHY_MASK            0x08000000
#define BCHP_DS_TUNER_REF_0_REF_04_CLK_BYP_EN_GPHY_SHIFT           27

/* DS_TUNER_REF_0 :: REF_04 :: CLK_BYP_EN_APM [26:26] */
#define BCHP_DS_TUNER_REF_0_REF_04_CLK_BYP_EN_APM_MASK             0x04000000
#define BCHP_DS_TUNER_REF_0_REF_04_CLK_BYP_EN_APM_SHIFT            26

/* DS_TUNER_REF_0 :: REF_04 :: I_OUTCML_DIODE_PWRDN [25:25] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTCML_DIODE_PWRDN_MASK       0x02000000
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTCML_DIODE_PWRDN_SHIFT      25

/* DS_TUNER_REF_0 :: REF_04 :: I_IGEN_PWRDN [24:24] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_IGEN_PWRDN_MASK               0x01000000
#define BCHP_DS_TUNER_REF_0_REF_04_I_IGEN_PWRDN_SHIFT              24

/* DS_TUNER_REF_0 :: REF_04 :: I_OUTDIV_IGEN_PWRDN [23:23] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_IGEN_PWRDN_MASK        0x00800000
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_IGEN_PWRDN_SHIFT       23

/* DS_TUNER_REF_0 :: REF_04 :: I_OUTCML_LOWCUR [22:16] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTCML_LOWCUR_MASK            0x007f0000
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTCML_LOWCUR_SHIFT           16

/* DS_TUNER_REF_0 :: REF_04 :: I_OUTDIV_M6 [15:08] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_M6_MASK                0x0000ff00
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_M6_SHIFT               8

/* DS_TUNER_REF_0 :: REF_04 :: I_OUTDIV_M5 [07:00] */
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_M5_MASK                0x000000ff
#define BCHP_DS_TUNER_REF_0_REF_04_I_OUTDIV_M5_SHIFT               0

/***************************************************************************
 *SPLIT_00 - Analog Tuner Splitter Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: SPLIT_00 :: SPARE0 [31:18] */
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPARE0_MASK                   0xfffc0000
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPARE0_SHIFT                  18

/* DS_TUNER_REF_0 :: SPLIT_00 :: SPARE1 [17:05] */
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPARE1_MASK                   0x0003ffe0
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPARE1_SHIFT                  5

/* DS_TUNER_REF_0 :: SPLIT_00 :: SPLITTER_CM_CNTL [04:03] */
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPLITTER_CM_CNTL_MASK         0x00000018
#define BCHP_DS_TUNER_REF_0_SPLIT_00_SPLITTER_CM_CNTL_SHIFT        3

/* DS_TUNER_REF_0 :: SPLIT_00 :: RF_ATT [02:00] */
#define BCHP_DS_TUNER_REF_0_SPLIT_00_RF_ATT_MASK                   0x00000007
#define BCHP_DS_TUNER_REF_0_SPLIT_00_RF_ATT_SHIFT                  0

/***************************************************************************
 *SPARE_00 - Analog Tuner Reference PLL Control Register
 ***************************************************************************/
/* DS_TUNER_REF_0 :: SPARE_00 :: SPARE_00 [31:00] */
#define BCHP_DS_TUNER_REF_0_SPARE_00_SPARE_00_MASK                 0xffffffff
#define BCHP_DS_TUNER_REF_0_SPARE_00_SPARE_00_SHIFT                0

#endif /* #ifndef BCHP_DS_TUNER_REF_0_H__ */

/* End of File */
