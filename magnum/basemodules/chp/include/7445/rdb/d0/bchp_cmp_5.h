/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
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
 * Date:           Generated on              Mon Sep 23 09:50:33 2013
 *                 Full Compile MD5 Checksum fcccce298b546dd6a1f4cbad288478da
 *                   (minus title and desc)  
 *                 MD5 Checksum              211556602e37a33262598b3d5eeba81c
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_CMP_5_H__
#define BCHP_CMP_5_H__

/***************************************************************************
 *CMP_5 - Video Compositor 5 Registers
 ***************************************************************************/
#define BCHP_CMP_5_REVISION                      0x00644c00 /* Compositor Revision ID */
#define BCHP_CMP_5_HW_CONFIGURATION              0x00644c04 /* Compositor HW Configuration */
#define BCHP_CMP_5_CANVAS_CTRL                   0x00644c08 /* Canvas control */
#define BCHP_CMP_5_CANVAS_SIZE                   0x00644c0c /* Canvas Vertical and Horizontal Size */
#define BCHP_CMP_5_BG_COLOR                      0x00644c10 /* Background color register */
#define BCHP_CMP_5_BLEND_0_CTRL                  0x00644c14 /* Blending Control for First stage Blender */
#define BCHP_CMP_5_BLEND_1_CTRL                  0x00644c18 /* Blending Control for Second stage Blender */
#define BCHP_CMP_5_CMP_CTRL                      0x00644c28 /* Compositor Control */
#define BCHP_CMP_5_CMP_OUT_CTRL                  0x00644c40 /* Compositor Output Control */
#define BCHP_CMP_5_CRC_CTRL                      0x00644c44 /* BVN CRC Control Register */
#define BCHP_CMP_5_CRC_Y_STATUS                  0x00644c48 /* BVN CRC Luma Status Register */
#define BCHP_CMP_5_CRC_CB_STATUS                 0x00644c4c /* BVN CRC Chroma(Cb) Status Register */
#define BCHP_CMP_5_CRC_CR_STATUS                 0x00644c50 /* BVN CRC Chroma(Cr) Status Register */
#define BCHP_CMP_5_READBACK_POSITION             0x00644c54 /* Compositor Position For CMP Output Readback */
#define BCHP_CMP_5_READBACK_VALUE                0x00644c58 /* Compositor Readback Value at Specified Position */
#define BCHP_CMP_5_CSC_DEMO_SETTING              0x00644c5c /* Compositor Color Space Converter Demo Setting */
#define BCHP_CMP_5_SCRATCH_REGISTER              0x00644c60 /* Compositor Scratch Register */
#define BCHP_CMP_5_TEST_REGISTER                 0x00644c68 /* Compositor Test Register */
#define BCHP_CMP_5_TEST_REGISTER1                0x00644c6c /* Compositor Test Register1 */
#define BCHP_CMP_5_G0_SURFACE_SIZE               0x00644c80 /* Graphics Surface 0 Vertical and Horizontal Size */
#define BCHP_CMP_5_G0_SURFACE_OFFSET             0x00644c84 /* Graphics Surface 0 Vertical and Horizontal Offset */
#define BCHP_CMP_5_G0_DISPLAY_SIZE               0x00644c88 /* Graphics Surface 0 Display Vertical and Horizontal Size */
#define BCHP_CMP_5_G0_CANVAS_OFFSET              0x00644c8c /* Graphics Surface 0 Canvas Vertical and Horizontal Offset */
#define BCHP_CMP_5_G0_CANVAS_X_OFFSET_R          0x00644c90 /* Graphics Surface 0 Canvas Horizontal Offset For Right or Under Window in 3D mode */
#define BCHP_CMP_5_G0_SURFACE_CTRL               0x00644c94 /* Graphics Surface 0 Control */
#define BCHP_CMP_5_G0_BVB_IN_STATUS_CLEAR        0x00644c98 /* Graphics Surface 0 BVB Input Status Clear */
#define BCHP_CMP_5_G0_BVB_IN_STATUS              0x00644c9c /* Graphics Surface 0 BVB Input Status */
#define BCHP_CMP_5_V0_SURFACE_SIZE               0x00644cb0 /* Video Surface 0 Vertical and Horizontal Size */
#define BCHP_CMP_5_V0_SURFACE_OFFSET             0x00644cb4 /* Video Surface 0 Vertical and Horizontal Offset */
#define BCHP_CMP_5_V0_DISPLAY_SIZE               0x00644cb8 /* Video Surface 0 Display Vertical and Horizontal Size */
#define BCHP_CMP_5_V0_CANVAS_OFFSET              0x00644cbc /* Video Surface 0 Canvas Vertical and Horizontal Offset */
#define BCHP_CMP_5_V0_CANVAS_X_OFFSET_R          0x00644cc0 /* Video Surface 0 Canvas Horizontal Offset For Right or Under Window in 3D mode */
#define BCHP_CMP_5_V0_SURFACE_CTRL               0x00644cc4 /* Video Surface 0 Control */
#define BCHP_CMP_5_V0_BVB_IN_STATUS_CLEAR        0x00644cc8 /* Video Surface 0 BVB Input Status Clear */
#define BCHP_CMP_5_V0_BVB_IN_STATUS              0x00644ccc /* Video Surface 0 BVB Input Status */
#define BCHP_CMP_5_V0_CONST_COLOR                0x00644cd0 /* Video Surface 0 Constant Color Register */
#define BCHP_CMP_5_V0_CB_KEYING                  0x00644cd4 /* Video Surface 0 Chroma (Blue) Key */
#define BCHP_CMP_5_V0_CR_KEYING                  0x00644cd8 /* Video Surface 0 Chroma (Red) Key */
#define BCHP_CMP_5_V0_LUMA_KEYING                0x00644cdc /* Video Surface 0 Luma Key */
#define BCHP_CMP_5_V0_RECT_CSC_INDEX_0           0x00644ce0 /* Video Surface 0 rectangle CSC index 0 */
#define BCHP_CMP_5_V0_RECT_CSC_INDEX_1           0x00644ce4 /* Video Surface 0 rectangle CSC index 1 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C00            0x00644ce8 /* Video Surface 0 Color Matrix R0 coefficient c00 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C01            0x00644cec /* Video Surface 0 Color Matrix R0 coefficient c01 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C02            0x00644cf0 /* Video Surface 0 Color Matrix R0 coefficient c02 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C03            0x00644cf4 /* Video Surface 0 Color Matrix R0 coefficient c03 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C10            0x00644cf8 /* Video Surface 0 Color Matrix R0 coefficient c10 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C11            0x00644cfc /* Video Surface 0 Color Matrix R0 coefficient c11 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C12            0x00644d00 /* Video Surface 0 Color Matrix R0 coefficient c12 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C13            0x00644d04 /* Video Surface 0 Color Matrix R0 coefficient c13 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C20            0x00644d08 /* Video Surface 0 Color Matrix R0 coefficient c20 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C21            0x00644d0c /* Video Surface 0 Color Matrix R0 coefficient c21 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C22            0x00644d10 /* Video Surface 0 Color Matrix R0 coefficient c22 */
#define BCHP_CMP_5_V0_R0_MC_COEFF_C23            0x00644d14 /* Video Surface 0 Color Matrix R0 coefficient c23 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C00            0x00644d18 /* Video Surface 0 Color Matrix R1 coefficient c00 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C01            0x00644d1c /* Video Surface 0 Color Matrix R1 coefficient c01 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C02            0x00644d20 /* Video Surface 0 Color Matrix R1 coefficient c02 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C03            0x00644d24 /* Video Surface 0 Color Matrix R1 coefficient c03 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C10            0x00644d28 /* Video Surface 0 Color Matrix R1 coefficient c10 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C11            0x00644d2c /* Video Surface 0 Color Matrix R1 coefficient c11 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C12            0x00644d30 /* Video Surface 0 Color Matrix R1 coefficient c12 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C13            0x00644d34 /* Video Surface 0 Color Matrix R1 coefficient c13 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C20            0x00644d38 /* Video Surface 0 Color Matrix R1 coefficient c20 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C21            0x00644d3c /* Video Surface 0 Color Matrix R1 coefficient c21 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C22            0x00644d40 /* Video Surface 0 Color Matrix R1 coefficient c22 */
#define BCHP_CMP_5_V0_R1_MC_COEFF_C23            0x00644d44 /* Video Surface 0 Color Matrix R1 coefficient c23 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C00            0x00644d48 /* Video Surface 0 Color Matrix R2 coefficient c00 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C01            0x00644d4c /* Video Surface 0 Color Matrix R2 coefficient c01 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C02            0x00644d50 /* Video Surface 0 Color Matrix R2 coefficient c02 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C03            0x00644d54 /* Video Surface 0 Color Matrix R2 coefficient c03 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C10            0x00644d58 /* Video Surface 0 Color Matrix R2 coefficient c10 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C11            0x00644d5c /* Video Surface 0 Color Matrix R2 coefficient c11 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C12            0x00644d60 /* Video Surface 0 Color Matrix R2 coefficient c12 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C13            0x00644d64 /* Video Surface 0 Color Matrix R2 coefficient c13 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C20            0x00644d68 /* Video Surface 0 Color Matrix R2 coefficient c20 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C21            0x00644d6c /* Video Surface 0 Color Matrix R2 coefficient c21 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C22            0x00644d70 /* Video Surface 0 Color Matrix R2 coefficient c22 */
#define BCHP_CMP_5_V0_R2_MC_COEFF_C23            0x00644d74 /* Video Surface 0 Color Matrix R2 coefficient c23 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C00            0x00644d78 /* Video Surface 0 Color Matrix R3 coefficient c00 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C01            0x00644d7c /* Video Surface 0 Color Matrix R3 coefficient c01 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C02            0x00644d80 /* Video Surface 0 Color Matrix R3 coefficient c02 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C03            0x00644d84 /* Video Surface 0 Color Matrix R3 coefficient c03 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C10            0x00644d88 /* Video Surface 0 Color Matrix R3 coefficient c10 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C11            0x00644d8c /* Video Surface 0 Color Matrix R3 coefficient c11 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C12            0x00644d90 /* Video Surface 0 Color Matrix R3 coefficient c12 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C13            0x00644d94 /* Video Surface 0 Color Matrix R3 coefficient c13 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C20            0x00644d98 /* Video Surface 0 Color Matrix R3 coefficient c20 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C21            0x00644d9c /* Video Surface 0 Color Matrix R3 coefficient c21 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C22            0x00644da0 /* Video Surface 0 Color Matrix R3 coefficient c22 */
#define BCHP_CMP_5_V0_R3_MC_COEFF_C23            0x00644da4 /* Video Surface 0 Color Matrix R3 coefficient c23 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C00            0x00644da8 /* Video Surface 0 Color Matrix R4 coefficient c00 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C01            0x00644dac /* Video Surface 0 Color Matrix R4 coefficient c01 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C02            0x00644db0 /* Video Surface 0 Color Matrix R4 coefficient c02 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C03            0x00644db4 /* Video Surface 0 Color Matrix R4 coefficient c03 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C10            0x00644db8 /* Video Surface 0 Color Matrix R4 coefficient c10 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C11            0x00644dbc /* Video Surface 0 Color Matrix R4 coefficient c11 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C12            0x00644dc0 /* Video Surface 0 Color Matrix R4 coefficient c12 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C13            0x00644dc4 /* Video Surface 0 Color Matrix R4 coefficient c13 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C20            0x00644dc8 /* Video Surface 0 Color Matrix R4 coefficient c20 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C21            0x00644dcc /* Video Surface 0 Color Matrix R4 coefficient c21 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C22            0x00644dd0 /* Video Surface 0 Color Matrix R4 coefficient c22 */
#define BCHP_CMP_5_V0_R4_MC_COEFF_C23            0x00644dd4 /* Video Surface 0 Color Matrix R4 coefficient c23 */
#define BCHP_CMP_5_V0_RECT_COLOR                 0x00644dd8 /* Video Surface 0 Rectangle Function Color Register */
#define BCHP_CMP_5_V0_RECT_TOP_CTRL              0x00644ddc /* Video Surface 0 Top Level Rectangle Control for Mosaic or See Through. */
#define BCHP_CMP_5_V0_RECT_ENABLE_MASK           0x00644de0 /* Video Surface 0 Rectangle Function Enable Mask */

/***************************************************************************
 *V0_RECT_SIZE%i - Rectangle Vertical and Horizontal Size.
 ***************************************************************************/
#define BCHP_CMP_5_V0_RECT_SIZEi_ARRAY_BASE                        0x00644de4
#define BCHP_CMP_5_V0_RECT_SIZEi_ARRAY_START                       0
#define BCHP_CMP_5_V0_RECT_SIZEi_ARRAY_END                         15
#define BCHP_CMP_5_V0_RECT_SIZEi_ARRAY_ELEMENT_SIZE                32

/***************************************************************************
 *V0_RECT_SIZE%i - Rectangle Vertical and Horizontal Size.
 ***************************************************************************/
/* CMP_5 :: V0_RECT_SIZEi :: reserved0 [31:29] */
#define BCHP_CMP_5_V0_RECT_SIZEi_reserved0_MASK                    0xe0000000
#define BCHP_CMP_5_V0_RECT_SIZEi_reserved0_SHIFT                   29

/* CMP_5 :: V0_RECT_SIZEi :: HSIZE [28:16] */
#define BCHP_CMP_5_V0_RECT_SIZEi_HSIZE_MASK                        0x1fff0000
#define BCHP_CMP_5_V0_RECT_SIZEi_HSIZE_SHIFT                       16
#define BCHP_CMP_5_V0_RECT_SIZEi_HSIZE_DEFAULT                     0x00000000

/* CMP_5 :: V0_RECT_SIZEi :: reserved1 [15:12] */
#define BCHP_CMP_5_V0_RECT_SIZEi_reserved1_MASK                    0x0000f000
#define BCHP_CMP_5_V0_RECT_SIZEi_reserved1_SHIFT                   12

/* CMP_5 :: V0_RECT_SIZEi :: VSIZE [11:00] */
#define BCHP_CMP_5_V0_RECT_SIZEi_VSIZE_MASK                        0x00000fff
#define BCHP_CMP_5_V0_RECT_SIZEi_VSIZE_SHIFT                       0
#define BCHP_CMP_5_V0_RECT_SIZEi_VSIZE_DEFAULT                     0x00000000


/***************************************************************************
 *V0_RECT_OFFSET%i - Rectangle Starting Point Offset from Surface Origin.
 ***************************************************************************/
#define BCHP_CMP_5_V0_RECT_OFFSETi_ARRAY_BASE                      0x00644e24
#define BCHP_CMP_5_V0_RECT_OFFSETi_ARRAY_START                     0
#define BCHP_CMP_5_V0_RECT_OFFSETi_ARRAY_END                       15
#define BCHP_CMP_5_V0_RECT_OFFSETi_ARRAY_ELEMENT_SIZE              32

/***************************************************************************
 *V0_RECT_OFFSET%i - Rectangle Starting Point Offset from Surface Origin.
 ***************************************************************************/
/* CMP_5 :: V0_RECT_OFFSETi :: reserved0 [31:29] */
#define BCHP_CMP_5_V0_RECT_OFFSETi_reserved0_MASK                  0xe0000000
#define BCHP_CMP_5_V0_RECT_OFFSETi_reserved0_SHIFT                 29

/* CMP_5 :: V0_RECT_OFFSETi :: X_OFFSET [28:16] */
#define BCHP_CMP_5_V0_RECT_OFFSETi_X_OFFSET_MASK                   0x1fff0000
#define BCHP_CMP_5_V0_RECT_OFFSETi_X_OFFSET_SHIFT                  16
#define BCHP_CMP_5_V0_RECT_OFFSETi_X_OFFSET_DEFAULT                0x00000000

/* CMP_5 :: V0_RECT_OFFSETi :: reserved1 [15:12] */
#define BCHP_CMP_5_V0_RECT_OFFSETi_reserved1_MASK                  0x0000f000
#define BCHP_CMP_5_V0_RECT_OFFSETi_reserved1_SHIFT                 12

/* CMP_5 :: V0_RECT_OFFSETi :: Y_OFFSET [11:00] */
#define BCHP_CMP_5_V0_RECT_OFFSETi_Y_OFFSET_MASK                   0x00000fff
#define BCHP_CMP_5_V0_RECT_OFFSETi_Y_OFFSET_SHIFT                  0
#define BCHP_CMP_5_V0_RECT_OFFSETi_Y_OFFSET_DEFAULT                0x00000000


#endif /* #ifndef BCHP_CMP_5_H__ */

/* End of File */
