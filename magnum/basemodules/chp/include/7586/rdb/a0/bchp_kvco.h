/****************************************************************************
 *     Copyright (c) 1999-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Wed Feb 11 10:14:00 2015
 *                 Full Compile MD5 Checksum  f7f4bd55341805fcfe958ba5e47e65f4
 *                     (minus title and desc)
 *                 MD5 Checksum               95b679a9655597a92593cae55222c397
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_KVCO_H__
#define BCHP_KVCO_H__

/***************************************************************************
 *KVCO - KVCO Registers
 ***************************************************************************/
#define BCHP_KVCO_KVCO_CTL1                      0x014e0200 /* [RW] KVCO Adc and kvcoStart Delay */
#define BCHP_KVCO_KVCO_CTL2                      0x014e0204 /* [RW] KVCO KvcoCal and KvcoEnd Delay */
#define BCHP_KVCO_KVCO_CTL3                      0x014e0208 /* [RW] KVCO IcpConvChan and PLLval Overrides */
#define BCHP_KVCO_KVCO_CTL4                      0x014e020c /* [RW] KVCO IcpN and IcpP Value Overrides */
#define BCHP_KVCO_KVCO_CTL5                      0x014e0210 /* [RW] KVCO Vhi and Vlo Value Overrides */
#define BCHP_KVCO_KVCO_CTL6                      0x014e0214 /* [RW] KVCO Icp_Target and IcpTrim_OR and IcpIndex */
#define BCHP_KVCO_KVCO_CTL7                      0x014e0218 /* [RW] KVCO Index */
#define BCHP_KVCO_KVCO_CTL8                      0x014e021c /* [RW] KVCO Misc Controls */
#define BCHP_KVCO_KVCO_RBVAL                     0x014e0220 /* [RO] KVCO_RBVAL */
#define BCHP_KVCO_TSSI_CTL                       0x014e0224 /* [RW] CAL TSSI Control */

/***************************************************************************
 *KVCO_CTL1 - KVCO Adc and kvcoStart Delay
 ***************************************************************************/
/* KVCO :: KVCO_CTL1 :: reserved0 [31:27] */
#define BCHP_KVCO_KVCO_CTL1_reserved0_MASK                         0xf8000000
#define BCHP_KVCO_KVCO_CTL1_reserved0_SHIFT                        27

/* KVCO :: KVCO_CTL1 :: DLY_Adc [26:16] */
#define BCHP_KVCO_KVCO_CTL1_DLY_Adc_MASK                           0x07ff0000
#define BCHP_KVCO_KVCO_CTL1_DLY_Adc_SHIFT                          16
#define BCHP_KVCO_KVCO_CTL1_DLY_Adc_DEFAULT                        0x0000021c

/* KVCO :: KVCO_CTL1 :: reserved1 [15:11] */
#define BCHP_KVCO_KVCO_CTL1_reserved1_MASK                         0x0000f800
#define BCHP_KVCO_KVCO_CTL1_reserved1_SHIFT                        11

/* KVCO :: KVCO_CTL1 :: DLY_Kvcostart [10:00] */
#define BCHP_KVCO_KVCO_CTL1_DLY_Kvcostart_MASK                     0x000007ff
#define BCHP_KVCO_KVCO_CTL1_DLY_Kvcostart_SHIFT                    0
#define BCHP_KVCO_KVCO_CTL1_DLY_Kvcostart_DEFAULT                  0x0000010e

/***************************************************************************
 *KVCO_CTL2 - KVCO KvcoCal and KvcoEnd Delay
 ***************************************************************************/
/* KVCO :: KVCO_CTL2 :: reserved0 [31:27] */
#define BCHP_KVCO_KVCO_CTL2_reserved0_MASK                         0xf8000000
#define BCHP_KVCO_KVCO_CTL2_reserved0_SHIFT                        27

/* KVCO :: KVCO_CTL2 :: DLY_KvcoEnd [26:16] */
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoEnd_MASK                       0x07ff0000
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoEnd_SHIFT                      16
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoEnd_DEFAULT                    0x0000010e

/* KVCO :: KVCO_CTL2 :: reserved1 [15:11] */
#define BCHP_KVCO_KVCO_CTL2_reserved1_MASK                         0x0000f800
#define BCHP_KVCO_KVCO_CTL2_reserved1_SHIFT                        11

/* KVCO :: KVCO_CTL2 :: DLY_KvcoCal [10:00] */
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoCal_MASK                       0x000007ff
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoCal_SHIFT                      0
#define BCHP_KVCO_KVCO_CTL2_DLY_KvcoCal_DEFAULT                    0x0000032a

/***************************************************************************
 *KVCO_CTL3 - KVCO IcpConvChan and PLLval Overrides
 ***************************************************************************/
/* KVCO :: KVCO_CTL3 :: reserved0 [31:31] */
#define BCHP_KVCO_KVCO_CTL3_reserved0_MASK                         0x80000000
#define BCHP_KVCO_KVCO_CTL3_reserved0_SHIFT                        31

/* KVCO :: KVCO_CTL3 :: IcpConvChan_OR [30:16] */
#define BCHP_KVCO_KVCO_CTL3_IcpConvChan_OR_MASK                    0x7fff0000
#define BCHP_KVCO_KVCO_CTL3_IcpConvChan_OR_SHIFT                   16
#define BCHP_KVCO_KVCO_CTL3_IcpConvChan_OR_DEFAULT                 0x00000000

/* KVCO :: KVCO_CTL3 :: reserved1 [15:10] */
#define BCHP_KVCO_KVCO_CTL3_reserved1_MASK                         0x0000fc00
#define BCHP_KVCO_KVCO_CTL3_reserved1_SHIFT                        10

/* KVCO :: KVCO_CTL3 :: PllvalOR [09:00] */
#define BCHP_KVCO_KVCO_CTL3_PllvalOR_MASK                          0x000003ff
#define BCHP_KVCO_KVCO_CTL3_PllvalOR_SHIFT                         0
#define BCHP_KVCO_KVCO_CTL3_PllvalOR_DEFAULT                       0x00000000

/***************************************************************************
 *KVCO_CTL4 - KVCO IcpN and IcpP Value Overrides
 ***************************************************************************/
/* KVCO :: KVCO_CTL4 :: reserved0 [31:31] */
#define BCHP_KVCO_KVCO_CTL4_reserved0_MASK                         0x80000000
#define BCHP_KVCO_KVCO_CTL4_reserved0_SHIFT                        31

/* KVCO :: KVCO_CTL4 :: IcpN_OR [30:16] */
#define BCHP_KVCO_KVCO_CTL4_IcpN_OR_MASK                           0x7fff0000
#define BCHP_KVCO_KVCO_CTL4_IcpN_OR_SHIFT                          16
#define BCHP_KVCO_KVCO_CTL4_IcpN_OR_DEFAULT                        0x00000000

/* KVCO :: KVCO_CTL4 :: reserved1 [15:15] */
#define BCHP_KVCO_KVCO_CTL4_reserved1_MASK                         0x00008000
#define BCHP_KVCO_KVCO_CTL4_reserved1_SHIFT                        15

/* KVCO :: KVCO_CTL4 :: IcpP_OR [14:00] */
#define BCHP_KVCO_KVCO_CTL4_IcpP_OR_MASK                           0x00007fff
#define BCHP_KVCO_KVCO_CTL4_IcpP_OR_SHIFT                          0
#define BCHP_KVCO_KVCO_CTL4_IcpP_OR_DEFAULT                        0x00000000

/***************************************************************************
 *KVCO_CTL5 - KVCO Vhi and Vlo Value Overrides
 ***************************************************************************/
/* KVCO :: KVCO_CTL5 :: reserved0 [31:31] */
#define BCHP_KVCO_KVCO_CTL5_reserved0_MASK                         0x80000000
#define BCHP_KVCO_KVCO_CTL5_reserved0_SHIFT                        31

/* KVCO :: KVCO_CTL5 :: Vhi_OR [30:16] */
#define BCHP_KVCO_KVCO_CTL5_Vhi_OR_MASK                            0x7fff0000
#define BCHP_KVCO_KVCO_CTL5_Vhi_OR_SHIFT                           16
#define BCHP_KVCO_KVCO_CTL5_Vhi_OR_DEFAULT                         0x00000000

/* KVCO :: KVCO_CTL5 :: reserved1 [15:15] */
#define BCHP_KVCO_KVCO_CTL5_reserved1_MASK                         0x00008000
#define BCHP_KVCO_KVCO_CTL5_reserved1_SHIFT                        15

/* KVCO :: KVCO_CTL5 :: Vlo_OR [14:00] */
#define BCHP_KVCO_KVCO_CTL5_Vlo_OR_MASK                            0x00007fff
#define BCHP_KVCO_KVCO_CTL5_Vlo_OR_SHIFT                           0
#define BCHP_KVCO_KVCO_CTL5_Vlo_OR_DEFAULT                         0x00000000

/***************************************************************************
 *KVCO_CTL6 - KVCO Icp_Target and IcpTrim_OR and IcpIndex
 ***************************************************************************/
/* KVCO :: KVCO_CTL6 :: reserved0 [31:28] */
#define BCHP_KVCO_KVCO_CTL6_reserved0_MASK                         0xf0000000
#define BCHP_KVCO_KVCO_CTL6_reserved0_SHIFT                        28

/* KVCO :: KVCO_CTL6 :: IcpIndex [27:23] */
#define BCHP_KVCO_KVCO_CTL6_IcpIndex_MASK                          0x0f800000
#define BCHP_KVCO_KVCO_CTL6_IcpIndex_SHIFT                         23
#define BCHP_KVCO_KVCO_CTL6_IcpIndex_DEFAULT                       0x00000008

/* KVCO :: KVCO_CTL6 :: IcpTrim_OR [22:16] */
#define BCHP_KVCO_KVCO_CTL6_IcpTrim_OR_MASK                        0x007f0000
#define BCHP_KVCO_KVCO_CTL6_IcpTrim_OR_SHIFT                       16
#define BCHP_KVCO_KVCO_CTL6_IcpTrim_OR_DEFAULT                     0x00000000

/* KVCO :: KVCO_CTL6 :: reserved1 [15:14] */
#define BCHP_KVCO_KVCO_CTL6_reserved1_MASK                         0x0000c000
#define BCHP_KVCO_KVCO_CTL6_reserved1_SHIFT                        14

/* KVCO :: KVCO_CTL6 :: Icp_Target [13:00] */
#define BCHP_KVCO_KVCO_CTL6_Icp_Target_MASK                        0x00003fff
#define BCHP_KVCO_KVCO_CTL6_Icp_Target_SHIFT                       0
#define BCHP_KVCO_KVCO_CTL6_Icp_Target_DEFAULT                     0x00000a91

/***************************************************************************
 *KVCO_CTL7 - KVCO Index
 ***************************************************************************/
/* KVCO :: KVCO_CTL7 :: reserved0 [31:05] */
#define BCHP_KVCO_KVCO_CTL7_reserved0_MASK                         0xffffffe0
#define BCHP_KVCO_KVCO_CTL7_reserved0_SHIFT                        5

/* KVCO :: KVCO_CTL7 :: IcpKvcoCal [04:00] */
#define BCHP_KVCO_KVCO_CTL7_IcpKvcoCal_MASK                        0x0000001f
#define BCHP_KVCO_KVCO_CTL7_IcpKvcoCal_SHIFT                       0
#define BCHP_KVCO_KVCO_CTL7_IcpKvcoCal_DEFAULT                     0x00000008

/***************************************************************************
 *KVCO_CTL8 - KVCO Misc Controls
 ***************************************************************************/
/* KVCO :: KVCO_CTL8 :: reserved0 [31:14] */
#define BCHP_KVCO_KVCO_CTL8_reserved0_MASK                         0xffffc000
#define BCHP_KVCO_KVCO_CTL8_reserved0_SHIFT                        14

/* KVCO :: KVCO_CTL8 :: KvcoDeltaFreqDiv [13:12] */
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqDiv_MASK                  0x00003000
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqDiv_SHIFT                 12
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqDiv_DEFAULT               0x00000000

/* KVCO :: KVCO_CTL8 :: KvcoCalRBMode [11:08] */
#define BCHP_KVCO_KVCO_CTL8_KvcoCalRBMode_MASK                     0x00000f00
#define BCHP_KVCO_KVCO_CTL8_KvcoCalRBMode_SHIFT                    8
#define BCHP_KVCO_KVCO_CTL8_KvcoCalRBMode_DEFAULT                  0x00000000

/* KVCO :: KVCO_CTL8 :: dlpffilt_neg [07:07] */
#define BCHP_KVCO_KVCO_CTL8_dlpffilt_neg_MASK                      0x00000080
#define BCHP_KVCO_KVCO_CTL8_dlpffilt_neg_SHIFT                     7
#define BCHP_KVCO_KVCO_CTL8_dlpffilt_neg_DEFAULT                   0x00000000

/* KVCO :: KVCO_CTL8 :: dlpf_bwmode [06:06] */
#define BCHP_KVCO_KVCO_CTL8_dlpf_bwmode_MASK                       0x00000040
#define BCHP_KVCO_KVCO_CTL8_dlpf_bwmode_SHIFT                      6
#define BCHP_KVCO_KVCO_CTL8_dlpf_bwmode_DEFAULT                    0x00000000

/* KVCO :: KVCO_CTL8 :: IcpMeasFirst [05:05] */
#define BCHP_KVCO_KVCO_CTL8_IcpMeasFirst_MASK                      0x00000020
#define BCHP_KVCO_KVCO_CTL8_IcpMeasFirst_SHIFT                     5
#define BCHP_KVCO_KVCO_CTL8_IcpMeasFirst_DEFAULT                   0x00000000

/* KVCO :: KVCO_CTL8 :: kvcocalEna [04:04] */
#define BCHP_KVCO_KVCO_CTL8_kvcocalEna_MASK                        0x00000010
#define BCHP_KVCO_KVCO_CTL8_kvcocalEna_SHIFT                       4
#define BCHP_KVCO_KVCO_CTL8_kvcocalEna_DEFAULT                     0x00000000

/* KVCO :: KVCO_CTL8 :: sampleAdc_OR [03:03] */
#define BCHP_KVCO_KVCO_CTL8_sampleAdc_OR_MASK                      0x00000008
#define BCHP_KVCO_KVCO_CTL8_sampleAdc_OR_SHIFT                     3
#define BCHP_KVCO_KVCO_CTL8_sampleAdc_OR_DEFAULT                   0x00000000

/* KVCO :: KVCO_CTL8 :: divMul_OR [02:02] */
#define BCHP_KVCO_KVCO_CTL8_divMul_OR_MASK                         0x00000004
#define BCHP_KVCO_KVCO_CTL8_divMul_OR_SHIFT                        2
#define BCHP_KVCO_KVCO_CTL8_divMul_OR_DEFAULT                      0x00000000

/* KVCO :: KVCO_CTL8 :: KvcoDeltaFreqSel [01:00] */
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqSel_MASK                  0x00000003
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqSel_SHIFT                 0
#define BCHP_KVCO_KVCO_CTL8_KvcoDeltaFreqSel_DEFAULT               0x00000000

/***************************************************************************
 *KVCO_RBVAL - KVCO_RBVAL
 ***************************************************************************/
/* KVCO :: KVCO_RBVAL :: reserved0 [31:16] */
#define BCHP_KVCO_KVCO_RBVAL_reserved0_MASK                        0xffff0000
#define BCHP_KVCO_KVCO_RBVAL_reserved0_SHIFT                       16

/* KVCO :: KVCO_RBVAL :: KvcoCalRBval [15:00] */
#define BCHP_KVCO_KVCO_RBVAL_KvcoCalRBval_MASK                     0x0000ffff
#define BCHP_KVCO_KVCO_RBVAL_KvcoCalRBval_SHIFT                    0
#define BCHP_KVCO_KVCO_RBVAL_KvcoCalRBval_DEFAULT                  0x00000000

/***************************************************************************
 *TSSI_CTL - CAL TSSI Control
 ***************************************************************************/
/* KVCO :: TSSI_CTL :: reserved0 [31:10] */
#define BCHP_KVCO_TSSI_CTL_reserved0_MASK                          0xfffffc00
#define BCHP_KVCO_TSSI_CTL_reserved0_SHIFT                         10

/* KVCO :: TSSI_CTL :: TSSIValReadEn [09:09] */
#define BCHP_KVCO_TSSI_CTL_TSSIValReadEn_MASK                      0x00000200
#define BCHP_KVCO_TSSI_CTL_TSSIValReadEn_SHIFT                     9
#define BCHP_KVCO_TSSI_CTL_TSSIValReadEn_DEFAULT                   0x00000000

/* KVCO :: TSSI_CTL :: calTssi [08:00] */
#define BCHP_KVCO_TSSI_CTL_calTssi_MASK                            0x000001ff
#define BCHP_KVCO_TSSI_CTL_calTssi_SHIFT                           0
#define BCHP_KVCO_TSSI_CTL_calTssi_DEFAULT                         0x00000000

#endif /* #ifndef BCHP_KVCO_H__ */

/* End of File */
