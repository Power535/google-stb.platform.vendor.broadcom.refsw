/***************************************************************************
 *     Copyright (c) 2009-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * [File Description:]
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#ifndef _BXVD_PLATFORM_REVN0_H_
#define _BXVD_PLATFORM_REVN0_H_

/* Rev N core with dual IL pipe uses 1.79 MB */
#if ((BCHP_CHIP == 7445) || (BCHP_CHIP == 7250) || (BCHP_CHIP == 7268) || \
     (BCHP_CHIP == 7271) || (BCHP_CHIP == 7364) || (BCHP_CHIP == 7366) || \
     (BCHP_CHIP == 7439) || (BCHP_CHIP == 74371) || (BCHP_CHIP == 7586))
#define BXVD_P_FW_IMAGE_SIZE              0x001CA000
#else
#define BXVD_P_FW_IMAGE_SIZE              0x00180000
#endif

#define BXVD_P_FW_INNER_IMAGE_OFFSET      0x00090000
#define BXVD_P_FW_INNER_2_IMAGE_OFFSET    0x0012d000


/* CPU Clock speed for FW UART emulation */
/* UART clock frequency in mhz */

#if ((BCHP_CHIP == 7445) && (BCHP_VER < BCHP_VER_C0))
#define BXVD_P_AVD_OL_CLK_FREQ 444
#define BXVD_P_AVD_IL_CLK_FREQ 444

#elif (BCHP_CHIP == 7145)
#define BXVD_P_AVD_OL_CLK_FREQ 514
#define BXVD_P_AVD_IL_CLK_FREQ 514

#if 0
/* Not supported yet */
#elif ((BCHP_CHIP == 73625) || (BCHP_CHIP == 75635) || (BCHP_CHIP == 75845))
/* Use BCHP_GetFeature to determine video decoder CPU clock speed */
#define BXVD_P_DYNAMIC_AVD_CORE_FREQ 1
#endif

#elif ((BCHP_CHIP == 7364) || (BCHP_CHIP == 7250))
#define BXVD_P_AVD_OL_CLK_FREQ 430
#define BXVD_P_AVD_IL_CLK_FREQ 430

#else
#define BXVD_P_AVD_OL_CLK_FREQ 600
#define BXVD_P_AVD_IL_CLK_FREQ 600
#endif

#if (BCHP_CHIP == 7445)
#define  BXVD_P_MULTIPLE_HVD_PRESENT 1
#define  BXVD_P_THREE_HVD_DECODERS_PRESENT 1

#if (BCHP_VER < BCHP_VER_D0)
#define  BXVD_P_PRE_REVQ2_CORE 1
#endif

#if (BCHP_VER <= BCHP_VER_D0)
#define BXVD_P_8N3_BUG 1
#endif
#define  BXVD_P_USE_HVD_INTRS 1
#define  BXVD_P_HEVD_DUAL_PIPE_PRESENT 1

#define  BXVD_MAX_INSTANCE_COUNT      3
#define  BXVD_P_RUL_DONE_MASK_64_BITS 1

#define  BXVD_P_STB_REG_BASE   0
#define  BXVD_P_USE_HVD_INTRS  1

#define  BXVD_P_DECODER_REVP   1
#endif

#if (BCHP_CHIP == 7145)
#define  BXVD_P_MULTIPLE_HVD_PRESENT 1
#define  BXVD_P_TWO_HVD_DECODERS_PRESENT 1
#define  BXVD_P_PRE_REVQ2_CORE 1
#define  BXVD_P_8N3_BUG 1

#define  BXVD_MAX_INSTANCE_COUNT      2
#define  BXVD_P_STB_REG_BASE          BCHP_HEVD_OL_CPU_REGS_0_REG_START

#if (BCHP_VER >= BCHP_VER_B0)
#define  BXVD_P_USE_HVD_INTRS 1
#define  BXVD_P_DECODER_REVP         1

#else
#define  BXVD_P_USE_REV_N_RESET_METHOD 1
#define  BXVD_P_DECODER_REVN1         1
#endif
#endif

#if (BCHP_CHIP == 7366)
#define  BXVD_P_PRE_REVQ2_CORE 1
#define  BXVD_MAX_INSTANCE_COUNT      1
#define  BXVD_P_STB_REG_BASE          0
#define  BXVD_P_SET_CHICKEN_BIT_BEFORE_RESET 1
#define  BXVD_P_DECODER_REVP         1
#define  BXVD_P_8N3_BUG 1

#if (BCHP_VER > BCHP_VER_A0)
#define BXVD_P_USE_HVD_INTRS 1
#define BXVD_P_HEVD_DUAL_PIPE_PRESENT 1
#define  BXVD_P_PICTURE_DATA_RDY_2_SUPPORTTED  1

#else
#define BXVD_P_USE_REV_N_RESET_METHOD 1
#define BXVD_P_USE_BVNF_INTR_HVD1 1
#endif
#endif

#if (BCHP_CHIP == 7364) || (BCHP_CHIP == 7250)
#define  BXVD_P_USE_HVD_INTRS 1
#define  BXVD_P_HEVD_DUAL_PIPE_PRESENT 1
#define  BXVD_P_HEVD_PFRI_DEBUG_PFRI_GROUPING_PRESENT 1

#define  BXVD_MAX_INSTANCE_COUNT      1
#define  BXVD_P_STB_REG_BASE          0
#define  BXVD_P_SET_CHICKEN_BIT_BEFORE_RESET 1
#define  BXVD_P_DECODER_REVP         1
#define  BXVD_P_8N3_BUG 1
#endif

#if (BCHP_CHIP == 7586) || (BCHP_CHIP == 7268) || (BCHP_CHIP == 7271)

#if (BCHP_CHIP == 7268) || (BCHP_CHIP == 7271)
#define  BXVD_P_HEVD_PFRI_DEBUG_PFRI_GROUPING_PRESENT 1
#define  BXVD_P_STB_REG_BASE          BCHP_HEVD_OL_CPU_REGS_0_REG_START
#else
#define  BXVD_P_STB_REG_BASE          0
#define  BXVD_P_8N3_BUG 1
#endif

#define  BXVD_P_USE_HVD_INTRS 1
#define  BXVD_P_HEVD_DUAL_PIPE_PRESENT 1

#define  BXVD_MAX_INSTANCE_COUNT      1

#define  BXVD_P_SET_CHICKEN_BIT_BEFORE_RESET 1
#define  BXVD_P_DECODER_REVP         1
#endif

#if ((BCHP_CHIP == 7439) || (BCHP_CHIP == 74371))
#define  BXVD_P_STB_REG_BASE          0
#define  BXVD_P_SET_CHICKEN_BIT_BEFORE_RESET 1
#define  BXVD_P_DECODER_REVP          1

#if (BCHP_CHIP == 74371)
#define  BXVD_P_8N3_BUG 1
#endif

#if (BCHP_VER == BCHP_VER_A0)
#define BXVD_P_USE_REV_N_RESET_METHOD 1
#define BXVD_P_PRE_REVQ2_CORE 1
#define BXVD_P_USE_BVNF_INTR_HVD1 1
#define BXVD_MAX_INSTANCE_COUNT      1
#define BXVD_P_8N3_BUG 1
#else
#define BXVD_P_MULTIPLE_HVD_PRESENT 1
#define BXVD_P_TWO_HVD_DECODERS_PRESENT 1
#define BXVD_P_USE_HVD_INTRS 1
#define BXVD_P_HEVD_DUAL_PIPE_PRESENT 1
#define BXVD_P_HEVD_PFRI_DEBUG_PFRI_GROUPING_PRESENT 1
#define BXVD_MAX_INSTANCE_COUNT      2
#define BXVD_P_PICTURE_DATA_RDY_2_SUPPORTTED  1
#endif

#endif /* 7439, 74371 */

#if (BCHP_CHIP == 75525) || (BCHP_CHIP == 75635) || (BCHP_CHIP == 73625) || (BCHP_CHIP == 74295) || (BCHP_CHIP == 75845) || (BCHP_CHIP == 73465)
#define  BXVD_P_STB_REG_BASE          0
#define  BXVD_P_SET_CHICKEN_BIT_BEFORE_RESET 1
#define  BXVD_P_DECODER_REVP          1
#define  BXVD_P_MIPS_CORE             1

#define BXVD_P_PRE_REVQ2_CORE 1
#define BXVD_P_8N3_BUG 1

#define BXVD_P_USE_HVD_INTRS 1
#define BXVD_P_HEVD_DUAL_PIPE_PRESENT 0
/* #define BXVD_P_HEVD_PFRI_DEBUG_PFRI_GROUPING_PRESENT 1 */
#define BXVD_MAX_INSTANCE_COUNT      1
#endif

#define BXVD_P_HVD_PRESENT 1

#define BXVD_P_VP6_SUPPORT 1
#define BXVD_P_ILS_BUFFERS_INTERNAL 1

#include "bchp_common.h"
#include "bchp_bvnf_intr2_0.h"

#if BXVD_P_USE_HVD_INTRS
#include "bchp_hvd_intr2_0.h"
#include "bchp_int_id_hvd_intr2_0.h"
#else
#include "bchp_shvd_intr2_0.h"
#include "bchp_int_id_shvd_intr2_0.h"
#endif

#include "bchp_reg_cabac2bins2_0.h"
#include "bchp_hevd_ol_ctl_0.h"
#include "bchp_decode_sint_0.h"
#include "bchp_hevd_ol_sint_0.h"
#include "bchp_decode_main_0.h"
#include "bchp_hevd_pcache_0.h"

#include "bchp_hevd_ol_cpu_regs_0.h"
#include "bchp_hevd_il_cpu_regs_0.h"

#if BXVD_P_MULTIPLE_HVD_PRESENT
#include "bchp_hevd_ol_cpu_regs_1.h"
#include "bchp_hevd_il_cpu_regs_1.h"
#include "bchp_hevd_ol_cpu_debug_1.h"
#include "bchp_hevd_il_cpu_debug_1.h"

#if BXVD_P_USE_HVD_INTRS
#include "bchp_hvd_intr2_1.h"
#include "bchp_int_id_hvd_intr2_1.h"
#else
#include "bchp_shvd_intr2_1.h"
#include "bchp_int_id_shvd_intr2_1.h"
#endif

#include "bchp_hevd_ol_ctl_1.h"
#include "bchp_decode_sint_1.h"
#include "bchp_hevd_ol_sint_1.h"
#include "bchp_reg_cabac2bins2_1.h"
#include "bchp_decode_main_1.h"
#include "bchp_hevd_pcache_1.h"
#include "bchp_rvc_1.h"
#include "bchp_dcd_pipe_ctl_1.h"
#include "bchp_hevd_pfri_1.h"
#endif

#include "bchp_hevd_ol_cpu_debug_0.h"
#include "bchp_hevd_il_cpu_debug_0.h"

#include "bchp_hevd_pfri_0.h"

#include "bchp_dcd_pipe_ctl_0.h"

#include "bchp_rvc_0.h"

#if BXVD_P_RUL_DONE_MASK_64_BITS

#include "bchp_bvnf_intr2_11.h"

#if BXVD_P_USE_HVD_INTRS
#include "bchp_int_id_hvd_intr2_2.h"
#else
#include "bchp_int_id_shvd_intr2_2.h"
#endif

#include "bchp_reg_cabac2bins2_2.h"
#include "bchp_hevd_ol_ctl_2.h"

#include "bchp_decode_sint_2.h"
#include "bchp_hevd_ol_sint_2.h"

#include "bchp_decode_main_2.h"
#include "bchp_hevd_pcache_2.h"
#include "bchp_rvc_2.h"

#include "bchp_hevd_ol_cpu_regs_2.h"
#include "bchp_hevd_il_cpu_regs_2.h"

#include "bchp_hevd_ol_cpu_debug_2.h"
#include "bchp_hevd_il_cpu_debug_2.h"

#include "bchp_hevd_pfri_2.h"

#include "bchp_dcd_pipe_ctl_2.h"
#endif

#if BXVD_P_HEVD_DUAL_PIPE_PRESENT
#include "bchp_hevd_il_cpu_debug_2_0.h"
#include "bchp_hevd_il_cpu_regs_2_0.h"

#include "bchp_hevd_pfri_2_0.h"
#include "bchp_hevd_pcache_2_0.h"
#endif

/* Common to all flavors */
#include "bchp_sun_top_ctrl.h"
#include "bchp_sun_gisb_arb.h"
#include "bchp_xpt_pcroffset.h"
#include "bchp_xpt_rave.h"

#include "bafl.h"

#define BXVD_P_CHIP_PRODUCT_REVISION  BCHP_SUN_TOP_CTRL_PRODUCT_ID

#define  BXVD_P_AVD_ARC600 1
#define  BXVD_P_FW_HIM_API 1
#define  BXVD_P_PLATFORM_STRIPE_WIDTH_NUM 3

#if (BXVD_MAX_INSTANCE_COUNT == 1)

#if BXVD_P_USE_HVD_INTRS
/* SVD support removed */
#define  BXVD_P_HVD_INTR2_0_CPU_SET   BCHP_HVD_INTR2_0_CPU_SET
#else
#define  BXVD_P_HVD_INTR2_0_CPU_SET   BCHP_SHVD_INTR2_0_CPU_SET
#endif

#if BXVD_P_MIPS_CORE
#ifdef  BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_svd0_sw_init_MASK
#define  BXVD_P_SW_INIT_0_SET_hvd0_sw_init_MASK     BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_svd0_sw_init_MASK
#define  BXVD_P_SW_INIT_0_CLEAR_hvd0_sw_init_MASK   BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR_svd0_sw_init_MASK

#define  BXVD_P_BVNF_INTR2_3_HVD0_STATUS       BCHP_BVNF_INTR2_3_SVD0_STATUS
#define  BXVD_P_BVNF_INTR2_3_HVD0_CLEAR        BCHP_BVNF_INTR2_3_SVD0_CLEAR
#define  BXVD_P_BVNF_INTR2_3_HVD0_MASK_CLEAR   BCHP_BVNF_INTR2_3_SVD0_MASK_CLEAR

#define  BXVD_P_GISB_ARB_REQ_MASK_hvd_0_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_svd_0_MASK

#else
#define  BXVD_P_SW_INIT_0_SET_hvd0_sw_init_MASK   BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_avd0_sw_init_MASK
#define  BXVD_P_SW_INIT_0_CLEAR_hvd0_sw_init_MASK   BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR_avd0_sw_init_MASK

#define  BXVD_P_BVNF_INTR2_3_HVD0_STATUS       BCHP_BVNF_INTR2_3_AVD0_STATUS
#define  BXVD_P_BVNF_INTR2_3_HVD0_CLEAR        BCHP_BVNF_INTR2_3_AVD0_CLEAR
#define  BXVD_P_BVNF_INTR2_3_HVD0_MASK_CLEAR   BCHP_BVNF_INTR2_3_AVD0_MASK_CLEAR

#define  BXVD_P_GISB_ARB_REQ_MASK_hvd_0_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_avd_0_MASK
#endif
#endif /* If MIPS_CORE */

#else /* Multi-decoders */

#if BXVD_P_USE_HVD_INTRS
#define  BXVD_P_DECODER_REVP         1

#define  BXVD_P_SW_INIT_0_SET_xvd0_sw_init_MASK  BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_hvd0_sw_init_MASK
#define  BXVD_P_SW_INIT_0_CLEAR_xvd0_sw_init_MASK BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR_hvd0_sw_init_MASK

#define  BXVD_P_BVNF_INTR2_3_HVD0_STATUS       BCHP_BVNF_INTR2_3_HVD0_STATUS
#define  BXVD_P_BVNF_INTR2_3_HVD0_CLEAR        BCHP_BVNF_INTR2_3_HVD0_CLEAR
#define  BXVD_P_BVNF_INTR2_3_HVD0_MASK_CLEAR   BCHP_BVNF_INTR2_3_HVD0_MASK_CLEAR

#define  BXVD_P_BVNF_INTR2_11_HVD0_STATUS      BCHP_BVNF_INTR2_11_HVD0_STATUS
#define  BXVD_P_BVNF_INTR2_11_HVD0_CLEAR       BCHP_BVNF_INTR2_11_HVD0_CLEAR
#define  BXVD_P_BVNF_INTR2_11_HVD0_MASK_CLEAR  BCHP_BVNF_INTR2_11_HVD0_MASK_CLEAR

#define  BXVD_P_SW_INIT_0_SET_hvd1_sw_init_MASK  BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_hvd1_sw_init_MASK
#define  BXVD_P_SW_INIT_0_CLEAR_hvd1_sw_init_MASK BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR_hvd1_sw_init_MASK

#define  BXVD_P_BVNF_INTR2_3_HVD1_STATUS       BCHP_BVNF_INTR2_3_HVD1_STATUS
#define  BXVD_P_BVNF_INTR2_3_HVD1_CLEAR        BCHP_BVNF_INTR2_3_HVD1_CLEAR
#define  BXVD_P_BVNF_INTR2_3_HVD1_MASK_CLEAR   BCHP_BVNF_INTR2_3_HVD1_MASK_CLEAR

#define  BXVD_P_GISB_ARB_REQ_MASK_HVD0_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_hvd_0_MASK
#define  BXVD_P_GISB_ARB_REQ_MASK_HVD1_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_hvd_1_MASK

#define  BXVD_P_HVD_INTR2_0_CPU_SET   BCHP_HVD_INTR2_0_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR0          BCHP_INT_ID_HVD_INTR2_0_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR3          BCHP_INT_ID_HVD_INTR2_0_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_MBOX_INTR         BCHP_INT_ID_HVD_INTR2_0_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR4          BCHP_INT_ID_HVD_INTR2_0_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR5          BCHP_INT_ID_HVD_INTR2_0_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR2          BCHP_INT_ID_HVD_INTR2_0_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_WATCHDOG_INTR     BCHP_INT_ID_HVD_INTR2_0_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_REG_INTR         BCHP_INT_ID_HVD_INTR2_0_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_SCB_WR_INTR      BCHP_INT_ID_HVD_INTR2_0_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_OL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_0_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_IL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_0_VICH_IL_INST_RD_INTR

#ifdef BCHP_HVD_INTR2_1_CPU_SET
#define  BXVD_P_HVD_INTR2_1_CPU_SET   BCHP_HVD_INTR2_1_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR0          BCHP_INT_ID_HVD_INTR2_1_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR3          BCHP_INT_ID_HVD_INTR2_1_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_MBOX_INTR         BCHP_INT_ID_HVD_INTR2_1_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR4          BCHP_INT_ID_HVD_INTR2_1_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR5          BCHP_INT_ID_HVD_INTR2_1_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR2          BCHP_INT_ID_HVD_INTR2_1_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_WATCHDOG_INTR     BCHP_INT_ID_HVD_INTR2_1_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_REG_INTR         BCHP_INT_ID_HVD_INTR2_1_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_SCB_WR_INTR      BCHP_INT_ID_HVD_INTR2_1_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_OL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_1_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_IL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_1_VICH_IL_INST_RD_INTR
#endif

#ifdef BCHP_HVD_INTR2_2_CPU_SET
#define  BXVD_P_HVD_INTR2_2_CPU_SET   BCHP_HVD_INTR2_2_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR0          BCHP_INT_ID_HVD_INTR2_2_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR3          BCHP_INT_ID_HVD_INTR2_2_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_MBOX_INTR         BCHP_INT_ID_HVD_INTR2_2_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR4          BCHP_INT_ID_HVD_INTR2_2_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR5          BCHP_INT_ID_HVD_INTR2_2_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR2          BCHP_INT_ID_HVD_INTR2_2_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_WATCHDOG_INTR     BCHP_INT_ID_HVD_INTR2_2_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_REG_INTR         BCHP_INT_ID_HVD_INTR2_2_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_SCB_WR_INTR      BCHP_INT_ID_HVD_INTR2_2_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_OL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_2_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_IL_INST_RD_INTR  BCHP_INT_ID_HVD_INTR2_2_VICH_IL_INST_RD_INTR
#endif

#else

#define  BXVD_P_SW_INIT_0_SET_hvd1_sw_init_MASK  BCHP_SUN_TOP_CTRL_SW_INIT_0_SET_avd1_sw_init_MASK
#define  BXVD_P_SW_INIT_0_CLEAR_hvd1_sw_init_MASK BCHP_SUN_TOP_CTRL_SW_INIT_0_CLEAR_avd1_sw_init_MASK

#define  BXVD_P_BVNF_INTR2_3_HVD1_STATUS       BCHP_BVNF_INTR2_3_AVD1_STATUS
#define  BXVD_P_BVNF_INTR2_3_HVD1_CLEAR        BCHP_BVNF_INTR2_3_AVD1_CLEAR
#define  BXVD_P_BVNF_INTR2_3_HVD1_MASK_CLEAR   BCHP_BVNF_INTR2_3_AVD1_MASK_CLEAR

#define  BXVD_P_GISB_ARB_REQ_MASK_HVD0_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_avd_0_MASK
#define  BXVD_P_GISB_ARB_REQ_MASK_HVD1_MASK   BCHP_SUN_GISB_ARB_REQ_MASK_avd_1_MASK

#define  BXVD_P_HVD_INTR2_0_CPU_SET   BCHP_SHVD_INTR2_0_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR0          BCHP_INT_ID_SHVD_INTR2_0_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR3          BCHP_INT_ID_SHVD_INTR2_0_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_MBOX_INTR         BCHP_INT_ID_SHVD_INTR2_0_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR4          BCHP_INT_ID_SHVD_INTR2_0_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR5          BCHP_INT_ID_SHVD_INTR2_0_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_SW_INTR2          BCHP_INT_ID_SHVD_INTR2_0_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_0_AVD_WATCHDOG_INTR     BCHP_INT_ID_SHVD_INTR2_0_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_REG_INTR         BCHP_INT_ID_SHVD_INTR2_0_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_SCB_WR_INTR      BCHP_INT_ID_SHVD_INTR2_0_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_OL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_0_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_0_VICH_IL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_0_VICH_IL_INST_RD_INTR

#ifdef BCHP_SHVD_INTR2_1_CPU_SET
#define  BXVD_P_HVD_INTR2_1_CPU_SET   BCHP_SHVD_INTR2_1_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR0          BCHP_INT_ID_SHVD_INTR2_1_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR3          BCHP_INT_ID_SHVD_INTR2_1_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_MBOX_INTR         BCHP_INT_ID_SHVD_INTR2_1_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR4          BCHP_INT_ID_SHVD_INTR2_1_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR5          BCHP_INT_ID_SHVD_INTR2_1_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_SW_INTR2          BCHP_INT_ID_SHVD_INTR2_1_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_1_AVD_WATCHDOG_INTR     BCHP_INT_ID_SHVD_INTR2_1_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_REG_INTR         BCHP_INT_ID_SHVD_INTR2_1_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_SCB_WR_INTR      BCHP_INT_ID_SHVD_INTR2_1_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_OL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_1_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_1_VICH_IL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_1_VICH_IL_INST_RD_INTR
#endif

#ifdef BCHP_SHVD_INTR2_2_CPU_SET
#define  BXVD_P_HVD_INTR2_2_CPU_SET   BCHP_SHVD_INTR2_2_CPU_SET

#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR0          BCHP_INT_ID_SHVD_INTR2_2_AVD_SW_INTR0
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR3          BCHP_INT_ID_SHVD_INTR2_2_AVD_SW_INTR3
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_MBOX_INTR         BCHP_INT_ID_SHVD_INTR2_2_AVD_MBOX_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR4          BCHP_INT_ID_SHVD_INTR2_2_AVD_SW_INTR4
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR5          BCHP_INT_ID_SHVD_INTR2_2_AVD_SW_INTR5
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_SW_INTR2          BCHP_INT_ID_SHVD_INTR2_2_AVD_SW_INTR2
#define  BXVD_P_INT_ID_HVD_INTR2_2_AVD_WATCHDOG_INTR     BCHP_INT_ID_SHVD_INTR2_2_AVD_WATCHDOG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_REG_INTR         BCHP_INT_ID_SHVD_INTR2_2_VICH_REG_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_SCB_WR_INTR      BCHP_INT_ID_SHVD_INTR2_2_VICH_SCB_WR_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_OL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_2_VICH_OL_INST_RD_INTR
#define  BXVD_P_INT_ID_HVD_INTR2_2_VICH_IL_INST_RD_INTR  BCHP_INT_ID_SHVD_INTR2_2_VICH_IL_INST_RD_INTR
#endif

#endif /* #if BXVD_P_USE_HVD_INTRS  */
#endif /* #if BXVD_P_MAX_INSTANCE_COUNT == 1 */

#include "bxvd_vdec_info.h"
#include "bxvd_priv.h"

#ifdef BCHP_MEMC_SENTINEL_0_0_REG_START
#define BXVD_P_MEMC_SENTINEL_0_REG_START BCHP_MEMC_SENTINEL_0_0_REG_START
#endif

#define BXVD_P_GET_MEMORY_CONFIG(hXvd, eMemCfgMode)   \
{                                                     \
   eMemCfgMode = BXVD_P_MemCfgMode_eUNKNOWN;          \
}

#if (((BCHP_CHIP == 7439) || (BCHP_CHIP == 74371)) && (BCHP_VER > BCHP_VER_A0)) || \
    ((BCHP_CHIP == 7445) && (BCHP_VER > BCHP_VER_D0)) || (BCHP_CHIP == 7268) || (BCHP_CHIP == 7271)
/* HVD/AVD Rev R */
#define BXVD_P_PLATFORM_SUPPORTED_PROTOCOLS  BXVD_P_REVR_DECODE_PROTOCOLS_MASK
#define BXVD_P_VP9_CAPABLE 1

#else
/* HVD/AVD Rev N */
#define BXVD_P_PLATFORM_SUPPORTED_PROTOCOLS  BXVD_P_REVN_DECODE_PROTOCOLS_MASK
#endif

#if (BCHP_CHIP == 7145)
#define BXVD_P_CORE_REVISION 'N'

#elif (((BCHP_CHIP == 7439) || (BCHP_CHIP == 7366) || (BCHP_CHIP == 74371)) && (BCHP_VER == BCHP_VER_A0))
#define BXVD_P_CORE_REVISION 'P'

#elif ((BCHP_CHIP == 7439) && (BCHP_VER > BCHP_VER_A0)) || \
      ((BCHP_CHIP == 7445) && (BCHP_VER > BCHP_VER_D0))
#define BXVD_P_CORE_REVISION 'R'

#else /* BCHP_CHIP == 7250, 7364, 7366, 7586, 7445 or 7439 B0+ */
#define BXVD_P_CORE_REVISION 'Q'
#endif

/* HW is RV9 capable, but it could still be disabled by OTP */
#define BXVD_P_RV9_CAPABLE 1

/* Get picture buffer atom size */
#define BXVD_P_USE_GET_BUFFER_ATOM_SIZE_REVN0 1
#define BXVD_P_GET_BUFFER_ATOM_SIZE BXVD_P_GetBufferAtomSize_RevN0
BXVD_P_SETUP_GET_BUFFER_ATOM_SIZE_PROTOTYPE(RevN0);

/* Platform Stripe Info */
#define BXVD_P_USE_DETERMINE_STRIPE_INFO_REVN0 1
#define BXVD_P_DETERMINE_STRIPE_INFO BXVD_P_DetermineStripeInfo_RevN0
BXVD_P_DETERMINE_STRIPE_INFO_PROTOTYPE(RevN0);

/* Firmware loading */
#define BXVD_P_USE_FW_LOAD_CORE_REVN0 1
#define BXVD_P_FW_LOAD BXVD_P_FWLoad_RevN0
BXVD_P_FW_LOAD_PROTOTYPE(RevN0);

/* Chip enable */
#define BXVD_P_USE_CORE_CHIP_ENABLE_REVN0 1
#define BXVD_P_CHIP_ENABLE BXVD_P_ChipEnable_RevN0
BXVD_P_CHIP_ENABLE_PROTOTYPE(RevN0);

/* Chip Reset */
#define BXVD_P_USE_CORE_RESET_CHIP_REVN0 1
#define BXVD_P_RESET_CHIP BXVD_P_ChipReset_RevN0
BXVD_P_CHIP_RESET_PROTOTYPE(RevN0);

/* Init Register Pointers/Masks */
#define BXVD_P_USE_INIT_REG_PTRS_REVN0 1
#define BXVD_P_INIT_REG_PTRS BXVD_P_InitRegPtrs_RevN0
BXVD_P_INIT_REG_PTRS_PROTOTYPE(RevN0);

/* Setup FW Memory */
#define BXVD_P_USE_SETUP_FW_MEMORY_REVE0 1
#define BXVD_P_SETUP_FW_MEMORY BXVD_P_SetupFWMemory_RevE0
BXVD_P_SETUP_FW_MEMORY_PROTOTYPE(RevE0);

/* Teardown FW Memory */
#define BXVD_P_USE_TEAR_DOWN_FW_MEMORY_REVE0 1
#define BXVD_P_TEAR_DOWN_FW_MEMORY BXVD_P_TearDownFWMemory_RevE0
BXVD_P_TEAR_DOWN_FW_MEMORY_PROTOTYPE(RevE0);

/* Verify Watchdog Fired */
#define BXVD_P_USE_VERIFY_WATCHDOG_FIRED_REVK0 1
#define BXVD_P_VERIFY_WATCHDOG_FIRED BXVD_P_VerifyWatchdogFired_RevK0_isr
BXVD_P_VERIFY_WATCHDOG_FIRED_PROTOTYPE(RevK0);

/* Rave Context Register info needed by FW */
#define BXVD_P_RAVE_CONTEXT_SIZE        (BCHP_XPT_RAVE_CX1_AV_CDB_WRITE_PTR - BCHP_XPT_RAVE_CX0_AV_CDB_WRITE_PTR)
#define BXVD_P_RAVE_CX_HOLD_CLR_STATUS  BCHP_XPT_RAVE_CX_HOLD_CLR_STATUS
#define BXVD_P_RAVE_PACKET_COUNT        BCHP_XPT_RAVE_PACKET_COUNT

/* Use Host Interface Memory routines */
#define BXVD_P_USE_READ_DISPLAY_INFO_HIM_API_REVN0 1

#define BXVD_P_IS_DISPLAY_INFO_EQUAL BXVD_P_IsDisplayInfoEqual_HIM_API_isr

void BXVD_P_ReadDisplayInfo_HIM_API_isr(BXVD_Handle hXvd,
                                        uint32_t uiDisplayInfoOffset,
                                        BXVD_P_DisplayInfo *pstDisplayInfo);

bool BXVD_P_IsDisplayInfoEqual_HIM_API_isr(BXVD_P_DisplayInfo stDisplayInfo,
                                           BXVD_P_DisplayInfo stDisplayInfo1);

#if BXVD_P_POWER_MANAGEMENT

#define BXVD_P_USE_SET_POWER_STATE_REVK0 1
#define BXVD_P_SET_POWER_STATE BXVD_P_SetPowerState_RevK0

void BXVD_P_SetPowerState_RevK0(BXVD_Handle hXvd,
                                BXVD_P_PowerState PowerState);

#endif /* BXVD_P_POWER_MANAGEMENT */

#endif /* _BXVD_PLATFORM_REVN0_H_ */


