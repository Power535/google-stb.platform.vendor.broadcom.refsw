/*
 * ACPHY RADIO contorl module implementation
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 *
 * <<Broadcom-WL-IPTag/Proprietary:>>
 *
 * $Id$
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <phy_dbg.h>
#include <phy_mem.h>
#include <phy_radio.h>
#include "phy_type_radio.h"
#include <phy_ac.h>
#include <phy_ac_radio.h>
#include <phy_ac_misc.h>
#include <phy_ac_tbl.h>
#include <phy_papdcal.h>
#include <phy_tpc.h>
#include <phy_utils_reg.h>
#include <wlc_phy_radio.h>
#include <wlc_radioreg_20691.h>
#include <wlc_phytbl_ac.h>
#include <wlc_phytbl_20691.h>
#include <wlc_radioreg_20693.h>
#include <wlc_radioreg_20694.h>
#include <wlc_radioreg_20695.h>
#include <wlc_radioreg_20696.h>
#include <wlc_phyreg_ac.h>
#include <sbchipc.h>
#include <hndpmu.h>
#include <wlc_phytbl_20693.h>
#include "wlc_phytbl_20694.h"
#include "wlc_phytbl_20695.h"
#include "wlc_phytbl_20696.h"
#include "phy_ac_tpc.h"
#include "bcmutils.h"
#include "phy_ac_pllconfig_20695.h"
#include "phy_ac_pllconfig_20694.h"
#include "phy_ac_pllconfig_20696.h"
#include "phy_utils_var.h"
#include "phy_rstr.h"
#include <phy_stf.h>
#include <phy_ac_nap.h>

#include <phy_dbg.h>

#include <phy_rstr.h>
#include <bcmdevs.h>

#ifndef ALL_NEW_PHY_MOD
/* < TODO: all these are going away... */
#include <wlc_phy_int.h>
#include <wlc_phy_hal.h>
/* TODO: all these are going away... > */
#endif
#include <phy_ac_info.h>
#include <bcmotp.h>

typedef enum {
	TINY_RCAL_MODE0_OTP = 0,
	TINY_RCAL_MODE1_STATIC,
	TINY_RCAL_MODE2_SINGLE_CORE_CAL,
	TINY_RCAL_MODE3_BOTH_CORE_CAL
} acphy_tiny_rcal_modes_t;

#ifdef PHY_DUMP_BINARY
/* AUTOGENRATED by the tool : phyreg.py
 * These values cannot be in const memory since
 * the const area might be over-written in case of
 * crash dump
 */
phyradregs_list_t rad20693_majorrev2_registers[] = {
	{0x0,  {0x80, 0x0, 0x0, 0x89}},
	{0x8a,  {0x80, 0x0, 0x0, 0xe7}},
	{0x200,  {0x80, 0x0, 0x0, 0x89}},
	{0x28a,  {0x80, 0x0, 0x0, 0xe7}},
};

phyradregs_list_t rad20695_majorrev1_registers[] = {
	{0x0,  {0x80, 0x0, 0x1, 0x1c}},
	{0x800,  {0x80, 0x0, 0x0, 0xec}},
};
#endif /* PHY_DUMP_BINARY */

#define MAX_2069_RCCAL_WAITLOOPS (100 * 100)
#define NUM_2069_RCCAL_CAPS 3

/* module private states */
struct phy_ac_radio_info {
	phy_info_t *pi;
	phy_ac_info_t *aci;
	phy_radio_info_t *ri;
	phy_ac_radio_data_t *data; /* shared data */
	acphy_pmu_core1_off_radregs_t *pmu_c1_off_info_orig;
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig;
	void	*chan_info_tbl;
	uint32	chan_info_tbl_len;
	uint32	rccal_dacbuf_cmult;
	uint16	rccal_adc_gmult;
	uint8	rccal_adc_rc;
	uint8	prev_subband;
	/* std params */
	uint16	rxRfctrlCoreRxPus0;
	uint16	rxRfctrlOverrideRxPus0;
	uint16	afeRfctrlCoreAfeCfg10;
	uint16	afeRfctrlCoreAfeCfg20;
	uint16	afeRfctrlOverrideAfeCfg0;
	uint16	txRfctrlCoreTxPus0;
	uint16	txRfctrlOverrideTxPus0;
	uint16	radioRfctrlCmd;
	uint16	radioRfctrlCoreGlobalPus;
	uint16	radioRfctrlOverrideGlobalPus;
	uint8	crisscross_priority_core_80p80;
	uint8	crisscross_priority_core_rsdb;
	uint8	is_crisscross_actv;
	/* To select the low power mode */
	uint8	acphy_lp_mode;
	uint8	acphy_prev_lp_mode;
	acphy_lp_modes_t	lpmode_2g;
	acphy_lp_modes_t	lpmode_5g;
	/* add other variable size variables here at the end */
};

typedef struct {
	phy_ac_radio_info_t info;
	phy_ac_radio_data_t data;
} phy_ac_radio_mem_t;

typedef struct {
	uint16 gi;
	uint16 g21;
	uint16 g32;
	uint16 g43;
	uint16 r12;
	uint16 r34;
	uint16 gff1;
	uint16 gff2;
	uint16 gff3;
	uint16 gff4;
	uint16 g11;
	uint16 ri3;
	uint16 g54;
	uint16 g65;
} tiny_adc_tuning_array_t;

/* local functions */
static void phy_ac_radio_std_params_attach(phy_ac_radio_info_t *info);
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void wlc_phy_radio20693_mimo_cm1_lpopt_saveregs(phy_info_t *pi);
static void wlc_phy_radio20693_mimo_cm23_lp_opt_saveregs(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

static void phy_ac_radio_switch(phy_type_radio_ctx_t *ctx, bool on);
static void phy_ac_radio_on(phy_type_radio_ctx_t *ctx);
static void phy_ac_radio_init_lpmode(phy_ac_radio_info_t *ri);
#ifdef PHY_DUMP_BINARY
static int phy_ac_radio_getlistandsize(phy_type_radio_ctx_t *ctx, phyradregs_list_t **radreglist,
	uint16 *radreglist_sz);
#endif
#if ((defined(BCMDBG) || defined(BCMDBG_DUMP)) && (defined(BCMINTPHYDBG) || \
	defined(DBG_PHY_IOV))) || defined(BCMDBG_PHYDUMP)
static int phy_ac_radio_dump(phy_type_radio_ctx_t *ctx, struct bcmstrbuf *b);
#else
#define phy_ac_radio_dump NULL
#endif /* BCMDBG, BCMDBG_DUMP, BCMINTPHYDBG, DBG_PHY_IOV, BCMDBG_PHYDUMP */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
/* 20693 tuning Table related */
static void wlc_phy_radio20693_pll_tune(phy_info_t *pi, const void *chan_info_pll,
	uint8 core);
static void wlc_phy_radio20693_rffe_tune(phy_info_t *pi, const void *chan_info_rffe,
	uint8 core);
static void wlc_phy_radio20693_pll_tune_wave2(phy_info_t *pi, const void *chan_info_pll,
	uint8 core);
static void wlc_phy_radio20693_rffe_rsdb_wr(phy_info_t *pi, uint8 core, uint8 reg_type,
	uint16 reg_val, uint16 reg_off);
static void wlc_phy_radio20693_set_logencore_pu(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core);
static void wlc_phy_radio20693_set_rfpll_vco_12g_buf_pu(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core);
static void wlc_phy_radio20693_afeclkdiv_12g_sel(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core);
static void wlc_phy_radio20693_afe_clk_div_mimoblk_pu(phy_info_t *pi, uint8 core,
	radio_20693_adc_modes_t adc_mode);
static void wlc_phy_radio20693_set_logen_mimosrcdes_pu(phy_info_t *pi, uint8 core,
	radio_20693_adc_modes_t adc_mode, uint16 mac_mode);
static void wlc_phy_radio20693_tx2g_set_freq_tuning_ipa_as_epa(phy_info_t *pi,
	uint8 core, uint8 chan);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

static void wlc_phy_set_regtbl_on_pwron_acphy(phy_info_t *pi);
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void wlc_phy_radio20693_mimo_cm1_lpopt(phy_info_t *pi);
static void wlc_phy_radio20693_mimo_cm1_lpopt_restore(phy_info_t *pi);
static void wlc_phy_radio20693_mimo_cm23_lp_opt(phy_info_t *pi, uint8 coremask);
static void wlc_phy_radio20693_mimo_cm23_lp_opt_restore(phy_info_t *pi);
static void wlc_phy_radio20693_afecal(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

/** 20696 ADC cap cal modes */
#define ADC_CAP_CAL_MODE_BYPASS		0
#define ADC_CAP_CAL_MODE_PARALLEL	1
#define ADC_CAP_CAL_MODE_SEQUENTIAL	2

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void wlc_phy_radio20696_adc_setup(phy_info_t *pi, uint8 core);
static void wlc_phy_radio20696_adc_cap_cal(phy_info_t *pi, int mode);
static void wlc_phy_radio20696_adc_cap_cal_setup(phy_info_t *pi, uint8 core);
static void wlc_phy_radio20696_adc_cap_cal_parallel(phy_info_t *pi, uint8 core, uint16 *timeout);
static void wlc_phy_radio20696_adc_cap_cal_sequential(phy_info_t *pi, uint8 core, uint16 *timeout);
static void wlc_phy_radio20696_apply_adc_cal_result(phy_info_t *pi, uint8 core);
static void wlc_phy_radio20696_apply_adc_cal_result_from_cache(phy_info_t *pi, uint8 core);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

/* Cleanup Chanspec */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void chanspec_setup_radio_20691(phy_info_t *pi);
static void chanspec_tune_radio_20691(phy_info_t *pi);
static void wlc_phy_chanspec_radio20691_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void chanspec_setup_radio_20693(phy_info_t *pi);
static void chanspec_tune_radio_20693(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void chanspec_setup_radio_20694(phy_info_t *pi);
static void chanspec_tune_radio_20694(phy_info_t *pi);
static void wlc_phy_chanspec_radio20694_setup(phy_info_t *pi, uint8 ch,
	uint8 toggle_logen_reset, uint8 core);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void chanspec_setup_radio_20695(phy_info_t *pi);
static void chanspec_setup_xtal_20695(phy_info_t *pi);
static void chanspec_tune_radio_20695(phy_info_t *pi);
static void wlc_phy_chanspec_radio20695_setup(phy_info_t *pi, uint8 ch,
	uint8 toggle_logen_reset, uint8 core);
static int BCMRAMFN(wlc_phy_ac_populate_20695_chan_info_tbl)(phy_info_t *pi,
		phy_ac_radio_info_t *radioi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void chanspec_setup_radio_20696(phy_info_t *pi);
static void wlc_phy_chanspec_radio20696_setup(phy_info_t *pi, uint8 ch,
	uint8 toggle_logen_reset);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void chanspec_setup_radio_2069(phy_info_t *pi);
static void chanspec_tune_radio_2069(phy_info_t *pi);
static void wlc_phy_chanspec_radio2069_setup(phy_info_t *pi, const void *chan_info,
	uint8 toggle_logen_reset);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static int BCMRAMFN(wlc_phy_ac_get_20694_chan_info_tbl)(phy_info_t *pi,
	const chan_info_radio20694_rffe_t **chan_info, uint32 *p_tbl_len);
static pll_config_20694_tbl_t *
BCMRAMFN(wlc_phy_ac_get_20694_pll_config)(phy_info_t *pi);
static radio_20xx_prefregs_t *
BCMRAMFN(wlc_phy_ac_get_20694_prfd_vals_tbl)(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static pll_config_20695_tbl_t *
BCMRAMFN(wlc_phy_ac_get_20695_pll_config)(phy_info_t *pi);
static radio_20xx_prefregs_t *
BCMRAMFN(wlc_phy_ac_get_20695_prfd_vals_tbl)(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */


#define RADIO20693_TUNE_REG_WR_SHIFT 0
#define RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT 1
/* Define affinities for each core
for 4349A0:
core 0's affinity: "2g"
core 1's affinity: "5g"
*/
#define RADIO20693_CORE0_AFFINITY 2
#define RADIO20693_CORE1_AFFINITY 5

/* Tuning reg type */
#define RADIO20693_TUNEREG_TYPE_2G 2
#define RADIO20693_TUNEREG_TYPE_5G 5
#define RADIO20693_TUNEREG_TYPE_NONE 0


#define RADIO20693_NUM_PLL_TUNE_REGS 32
/* Rev5,7 & 6,8 */
#define RADIO20693_NUM_RFFE_TUNE_REGS 3

#define BCM4349_UMC_FAB_ID 5
#define BCM4349_TSMC_FAB_ID 2
#define UMC_FAB_RCAL_VAL 8
#define TSMC_FAB_RCAL_VAL 11

static const uint8 rffe_tune_reg_map_20693[RADIO20693_NUM_RFFE_TUNE_REGS] = {
	RADIO20693_TUNEREG_TYPE_2G,
	RADIO20693_TUNEREG_TYPE_5G,
	RADIO20693_TUNEREG_TYPE_2G
};

typedef enum {
	PLL2G,
	PLL5G,
	LOGEN5G,
	LOGEN5GTO2G
} pll_logen_block_20695_t;

/* TIA LUT tables to be used in wlc_tiny_tia_config() */
static const uint8 tiaRC_tiny_8b_20[]= { /* LUT 0--51 (20 MHz) */
	0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff,
	0xb7, 0xb5, 0x97, 0x81, 0xe5, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x14, 0x1d, 0x28, 0x34, 0x34, 0x34,
	0x34, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40,
	0x5b, 0x6c, 0x80, 0x80
};

static const uint8 tiaRC_tiny_8b_40[]= { /* LUT 0--51 (40 MHz) */
	0xff, 0xe1, 0xb9, 0x81, 0x5d, 0xff, 0xad, 0x82,
	0x6d, 0x5d, 0x4c, 0x41, 0x73, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x16, 0x16, 0x1b, 0x1d, 0x1d, 0x1f,
	0x20, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x4c,
	0x5b, 0x6c, 0x80, 0x80
};

static const uint8 tiaRC_tiny_8b_80[]= { /* LUT 0--51 (80 MHz) */
	0xc2, 0x86, 0x5d, 0xff, 0xbe, 0x88, 0x5f, 0x43,
	0x37, 0x2e, 0x26, 0x20, 0x39, 0x00, 0x00, 0x00,
	0x0b, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0d,
	0x0d, 0x07, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
	0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x4c,
	0x5b, 0x6c, 0x80, 0x80
};

static const uint16 tiaRC_tiny_16b_20[]= { /* LUT 52--82 (20 MHz) */
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00b5, 0x0080, 0x005a,
	0x0040, 0x002d, 0x0020, 0x0017, 0x0000, 0x0100, 0x00b5, 0x0080,
	0x005b, 0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0080, 0x001f, 0x1fe0, 0xf500, 0x00ff
};

static const uint16 tiaRC_tiny_16b_20_rem[]= { /* LUT 79--82 (20 MHz) */
	0x001f, 0x1fe0, 0xf500, 0x00ff
};

static const uint16 tiaRC_tiny_16b_20_rem_war[]= { /* LUT 79--82 (20 MHz) */
	0x001f, 0x1fe0, 0xc500, 0x00ff
};

static const uint16 tiaRC_tiny_16b_80[]= { /* LUT 52--82 (80 MHz) */
	0x0000, 0x0000, 0x0000, 0x016b, 0x0100, 0x00b6, 0x0080, 0x005a,
	0x0040, 0x002d, 0x0020, 0x0017, 0x0000, 0x0100, 0x00b5, 0x0080,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0080, 0x0007, 0x1ff8, 0xf500, 0x00ff
};

static const uint16 tiaRC_tiny_16b_80_rem[]= { /* LUT 79--82 (80 MHz) */
	0x0007, 0x1ff8, 0xf500, 0x00ff
};

static const uint16 tiaRC_tiny_16b_80_rem_war[]= { /* LUT 79--82 (80 MHz) */
	0x0007, 0x1ff8, 0xc500, 0x00ff
};

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static const uint16 *tiaRC_tiny_16b_40 = tiaRC_tiny_16b_20;  /* LUT 52--78 (40 MHz) (no LUT 67) */
static const uint16 *tiaRC_tiny_16b_40_rem = tiaRC_tiny_16b_20_rem;  /* LUT 79--82 (40 MHz) */
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

/* Coefficients generated by 47xxtcl/rgphy/20691/ */
/* lpf_tx_coefficient_generator/filter_tx_tiny_generate_python_and_tcl.py */
static const uint16 lpf_g10[7][15] = {
	{1188, 1527, 1866, 2206, 2545, 2545, 1188, 1188,
	1188, 1188, 1188, 1188, 1188, 1188, 1188},
	{3300, 4242, 5185, 6128, 7071, 7071, 3300, 3300,
	3300, 3300, 3300, 3300, 3300, 3300, 3300},
	{16059, 16059, 16059, 17294, 18529, 18529, 9882,
	1976, 2470, 3088, 3953, 4941, 6176, 7906, 12353},
	{24088, 24088, 25941, 31500, 37059, 37059, 14823,
	2964, 3705, 4632, 5929, 7411, 9264, 11859, 18529},
	{29647, 32118, 34589, 42001, 49412, 49412, 19765,
	3705, 4941, 6176, 7411, 9882, 12353, 14823, 24706},
	{32941, 36236, 39530, 42824, 46118, 49412, 19765,
	4117, 4941, 6588, 8235, 9882, 13176, 16470, 26353},
	{10706, 10706, 10706, 11529, 12353, 12353, 6588,
	1317, 1647, 2058, 2635, 3294, 4117, 5270, 8235}
};
static const uint16 lpf_g12[7][15] = {
	{1882, 1922, 1866, 1752, 1606, 1275, 2984, 14956,
	11880, 9436, 7495, 5954, 4729, 3756, 2370},
	{5230, 5341, 5185, 4868, 4461, 3544, 8289, 41544,
	33000, 26212, 20821, 16539, 13137, 10435, 6584},
	{24872, 19757, 15693, 13424, 11425, 9075, 24258, 24316,
	24144, 23972, 24374, 24201, 24029, 24432, 24086},
	{37309, 29635, 25351, 24452, 22850, 18151, 36388, 36474,
	36216, 35959, 36561, 36302, 36044, 36648, 36130},
	{44360, 38172, 32654, 31496, 29433, 23379, 46870, 44045,
	46648, 46318, 44150, 46759, 46428, 44254, 46538},
	{49288, 43066, 37319, 32113, 27471, 23379, 46870, 48939,
	46648, 49406, 49055, 46759, 49523, 49172, 49640},
	{16581, 13171, 10462, 8949, 7616, 6050, 16172, 16210,
	16096, 15981, 16249, 16134, 16019, 16288, 16057}
};
static const uint16 lpf_g21[7][15] = {
	{1529, 1497, 1542, 1643, 1793, 2257, 965, 192, 242,
	305, 384, 483, 609, 766, 1215},
	{4249, 4160, 4285, 4565, 4981, 6270, 2681, 534, 673,
	847, 1067, 1343, 1691, 2129, 3375},
	{6135, 7723, 9723, 11367, 13356, 16814, 6290, 6275,
	6320, 6365, 6260, 6305, 6350, 6245, 6335},
	{9202, 11585, 13543, 14041, 15025, 18916, 9435, 9413,
	9480, 9548, 9391, 9458, 9525, 9368, 9503},
	{13760, 15990, 18693, 19380, 20738, 26108, 13023, 13858,
	13085, 13178, 13825, 13054, 13147, 13793, 13116},
	{22016, 25197, 29078, 33791, 39502, 46415, 23152, 22173,
	23262, 21964, 22121, 23207, 21912, 22068, 21860},
	{4090, 5149, 6482, 7578, 8904, 11209, 4193, 4183, 4213,
	4243, 4173, 4203, 4233, 4163, 4223}
};
static const uint16 lpf_g11[7] = {994, 2763, 12353, 18529, 17470, 23293, 8235};
static const uint16 g_passive_rc_tx[7] = {62, 172, 772, 1158, 1544, 2058, 514};
static const uint16 biases[7] = {24, 48, 96, 96, 128, 128, 96};
static const int8 g_index1[15] = {0, 1, 2, 3, 4, 5, -2, -9, -8, -7, -6, -5, -4, -3, -1};
static rxadccal_coeffs_t rxadccal_coeffs_cache[PHY_CORE_MAX];

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static uint8 wlc_phy_radio20693_tuning_get_access_info(phy_info_t *pi, uint8 core, uint8 reg_type);
static uint8 wlc_phy_radio20693_tuning_get_access_type(phy_info_t *pi, uint8 core, uint8 reg_type);

static void wlc_phy_radio20693_minipmu_pwron_seq(phy_info_t *pi);
static void wlc_phy_radio20693_pmu_override(phy_info_t *pi);
static void wlc_phy_radio20693_xtal_pwrup(phy_info_t *pi);
static void wlc_phy_radio20693_upd_prfd_values(phy_info_t *pi);
static void wlc_phy_radio20693_pmu_pll_config(phy_info_t *pi);
static void wlc_phy_switch_radio_acphy_20693(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

static void wlc_phy_switch_radio_acphy(phy_info_t *pi, bool on);
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void wlc_phy_radio2069_mini_pwron_seq_rev16(phy_info_t *pi);
static void wlc_phy_radio2069_mini_pwron_seq_rev32(phy_info_t *pi);
static void wlc_phy_radio2069_pwron_seq(phy_info_t *pi);
static void wlc_phy_radio2069_rcal(phy_info_t *pi);
static void wlc_phy_radio2069_upd_prfd_values(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM2069) || \
	defined(RADIO_BCM20696)))
static void wlc_phy_radio2069_rccal(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM2069) || defined(RADIO_BCM20696))) */


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static void wlc_phy_tiny_radio_pwron_seq(phy_info_t *pi);
static void wlc_phy_radio_tiny_rcal(phy_info_t *pi, acphy_tiny_rcal_modes_t mode);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void wlc_phy_switch_radio_acphy_20691(phy_info_t *pi);
static void wlc_phy_radio20691_upd_prfd_values(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
static void WLBANDINITFN(wlc_phy_static_table_download_acphy)(phy_info_t *pi);
static void wlc_phy_radio_tiny_rccal(phy_ac_radio_info_t *ri);

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void wlc_phy_radio_tiny_rcal_wave2(phy_info_t *pi, uint8 mode);
static void wlc_phy_radio_tiny_rccal_wave2(phy_ac_radio_info_t *ri);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void wlc_phy_radio20694_upd_prfd_values(phy_info_t *pi);
static void wlc_phy_radio20694_pwron_seq_phyregs(phy_info_t *pi);
static void wlc_phy_switch_radio_acphy_20694(phy_info_t *pi);
int8 wlc_phy_20694_radio_minipmu_cal(phy_info_t *pi);
int8 wlc_phy_20694_radio_minipmu_cal_byaux(phy_info_t *pi);
static void wlc_phy_20694_radio_rcal(phy_info_t *pi, uint8 mode);
static void wlc_phy_20694_radio_rcal_byaux(phy_info_t *pi, uint8 mode);
static void wlc_phy_radio20694_rccal(phy_info_t *pi);
static void wlc_phy_20694_logen_mode(phy_info_t *pi, uint8 core);
static void
wlc_phy_radio20694_pll_tune(phy_info_t *pi, uint32 chan_freq, uint8 core);
static void wlc_phy_radio20694_rffe_tune(phy_info_t *pi,
		const chan_info_radio20694_rffe_t *chan_info_rffe, uint8 core);
static void wlc_phy_radio20694_upd_band_related_reg(phy_info_t *pi);
static void phy_ac_radio20694_pll_config_ch_dep_calc(phy_info_t *pi, uint32 lo_freq,
	uint8 vco_select, uint32 icp_fx, uint32 loop_band,
	uint8 ac_mode, pll_config_20694_tbl_t *pll);
static void phy_ac_radio20694_write_pll_config(phy_info_t *pi, pll_config_20694_tbl_t *pll);
static void BCMATTACHFN(phy_ac_radio20694_pll_config_const_calc)(phy_info_t *pi,
		pll_config_20694_tbl_t *pll);
static int BCMATTACHFN(phy_ac_radio20694_populate_pll_config_tbl)(phy_info_t *pi,
		pll_config_20694_tbl_t *pll);
static void wlc_phy_radio20694_low_current_mode(phy_info_t *pi, uint8 mode);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void wlc_phy_radio20695_minipmu_pupd_seq(phy_info_t *pi, uint8 pupdbit);
static void wlc_phy_radio20695_upd_prfd_values(phy_info_t *pi);
static void wlc_phy_switch_radio_acphy_20695(phy_info_t *pi);
static radio_pll_sel_t
wlc_phy_radio20695_pll_tune(phy_info_t *pi, uint32 chan_freq, uint8 core);
static void wlc_phy_radio20695_rffe_tune(phy_info_t *pi,
		const chan_info_radio20695_rffe_t *chan_info_rffe, uint8 core);
static void phy_ac_radio20695_pll_config_ch_dep_calc(phy_info_t *pi, uint32 lo_freq,
		pll_config_20695_tbl_t *pll);
static void phy_ac_radio20695_write_pll_config(phy_info_t *pi, radio_pll_sel_t pll_sel,
		pll_config_20695_tbl_t *pll);
static void BCMRAMFN(phy_ac_radio20695_extra_pll_config_write)(phy_info_t *pi, uint16 chan_freq,
		radio_pll_sel_t pll_sel);
static void wlc_phy_radio20695_pll_logen_pu_config(phy_info_t *pi, uint32 chan_freq, uint8 core);
static void wlc_phy_radio20695_clear_adc_dac_en_ovr(phy_info_t *pi);
static void BCMATTACHFN(phy_ac_radio20695_pll_config_const_calc)(phy_info_t *pi,
		pll_config_20695_tbl_t *pll);
static int BCMATTACHFN(phy_ac_radio20695_populate_pll_config_tbl)(phy_info_t *pi,
		pll_config_20695_tbl_t *pll);
static void wlc_phy_28nm_radio_pwron_seq_phyregs(phy_info_t *pi, uint8 core);
static void wlc_phy_28nm_radio_rcal(phy_info_t *pi, uint8 mode);
static void wlc_phy_28nm_radio_rccal(phy_info_t *pi);
static void wlc_phy_28nm_radio_pll_logen_pupd_seq(phy_info_t *pi, pll_logen_block_20695_t block,
		uint8 pupdbit, uint8 core);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void wlc_phy_radio20696_minipmu_pwron_seq(phy_info_t *pi);
static void wlc_phy_radio20696_pwron_seq_phyregs(phy_info_t *pi);
static void wlc_phy_radio20696_minipmu_cal(phy_info_t *pi);
static void wlc_phy_radio20696_r_cal(phy_info_t *pi, uint8 mode);
static void wlc_phy_radio20696_rccal(phy_info_t *pi);
static void wlc_phy_radio20696_rffe_tune(phy_info_t *pi,
		const chan_info_radio20696_rffe_t *chan_info_rffe);
static void wlc_phy_radio20696_upd_band_related_reg(phy_info_t *pi);
static void wlc_phy_radio20696_pmu_pll_pwrup(phy_info_t *pi);
static void wlc_phy_radio20696_wars(phy_info_t *pi);
static void wlc_phy_radio20696_refclk_en(phy_info_t *pi);
static void wlc_phy_radio20696_upd_prfd_values(phy_info_t *pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

int8 wlc_phy_28nm_radio_minipmu_cal(phy_info_t *pi);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
static void wlc_phy_set_clb_femctrl_static_settings(phy_info_t *pi);
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */

#ifdef RADIO_HEALTH_CHECK
static bool phy_ac_radio_check_pll_lock(phy_type_radio_ctx_t *ctx);
#endif /* RADIO_HEALTH_CHECK */
/* query radio idcode */
static uint32
_phy_ac_radio_query_idcode(phy_type_radio_ctx_t *ctx)
{
	phy_ac_radio_info_t *info = (phy_ac_radio_info_t *)ctx;
	phy_info_t *pi = info->pi;
	uint32 idcode;

	idcode = phy_ac_radio_query_idcode(pi);
#ifdef BCMRADIOREV
	if (ISSIM_ENAB(pi->sh->sih)) {
		idcode = (idcode & ~IDCODE_ACPHY_REV_MASK) | (BCMRADIOREV <<IDCODE_ACPHY_REV_SHIFT);
	}
#endif	/* BCMRADIOREV */

	return idcode;
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
/* Structure to hold 20695 PLL config values */
static pll_config_20695_tbl_t pll_conf_20695;
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
/* Structure to hold 20694 PLL config values */
static pll_config_20694_tbl_t pll_conf_20694;

void print_pll_config_20694(pll_config_20694_tbl_t *pll, uint32 lo_freq,
	uint32 loop_bw, uint32 icp_fx);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

static void phy_ac_radio_nvram_attach(phy_info_t *pi, phy_ac_radio_info_t *radioi);

/* Register/unregister ACPHY specific implementation to common layer. */
phy_ac_radio_info_t *
BCMATTACHFN(phy_ac_radio_register_impl)(phy_info_t *pi, phy_ac_info_t *aci,
	phy_radio_info_t *ri)
{
	phy_ac_radio_info_t *info;
	acphy_pmu_core1_off_radregs_t *pmu_c1_off_info_orig = NULL;
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = NULL;
	phy_type_radio_fns_t fns;

	PHY_TRACE(("%s\n", __FUNCTION__));

	/* allocate all storage in once */
	if ((info = phy_malloc(pi, sizeof(phy_ac_radio_mem_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pmu_c1_off_info_orig =
		phy_malloc(pi, sizeof(acphy_pmu_core1_off_radregs_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc pmu_c1_off_info_orig failed\n", __FUNCTION__));
		goto fail;
	}
	if ((pmu_lp_opt_orig =
		phy_malloc(pi, sizeof(acphy_pmu_mimo_lp_opt_radregs_t))) == NULL) {
		PHY_ERROR(("%s: phy_malloc pmu_lp_opt_orig failed\n", __FUNCTION__));
		goto fail;
	}

	info->pi = pi;
	info->aci = aci;
	info->ri = ri;
	info->data = &(((phy_ac_radio_mem_t *) info)->data);

	info->pmu_c1_off_info_orig = pmu_c1_off_info_orig;
	info->pmu_lp_opt_orig = pmu_lp_opt_orig;
	pmu_lp_opt_orig->is_orig = FALSE;

	/* By default, use 5g pll for 2g will be disabled */
	info->data->use_5g_pll_for_2g = (uint8)PHY_GETINTVAR_DEFAULT(pi, rstr_use5gpllfor2g, 0);

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	/* Compute 20695 PLL config constants */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		pll_config_20695_tbl_t *pll;

		/* Populate chan info table */
		if (wlc_phy_ac_populate_20695_chan_info_tbl(pi, info) != BCME_OK) {
			PHY_ERROR(("%s: wlc_phy_ac_populate_20695_chan_info_tbl failed\n",
					__FUNCTION__));
			goto fail;
		}

		/* Get pointer to structure */
		pll = wlc_phy_ac_get_20695_pll_config(pi);

		if (phy_ac_radio20695_populate_pll_config_tbl(pi, pll) != BCME_OK) {
			PHY_ERROR(("%s: phy_ac_radio20695_populate_pll_config_tbl failed\n",
					__FUNCTION__));
			goto fail;
		}

		phy_ac_radio20695_pll_config_const_calc(pi, pll);
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
		if (pi->fabid == BCM4349_TSMC_FAB_ID)
			pi->rcal_value = TSMC_FAB_RCAL_VAL;
		else
			pi->rcal_value = UMC_FAB_RCAL_VAL;
	} else {
		/* falling back to some value */
		pi->rcal_value = UMC_FAB_RCAL_VAL;
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	/* Compute 20694 PLL config constants */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		pll_config_20694_tbl_t *pll;

		/* Get pointer to structure */
		pll = wlc_phy_ac_get_20694_pll_config(pi);

		if (phy_ac_radio20694_populate_pll_config_tbl(pi, pll) != BCME_OK) {
			PHY_ERROR(("%s: phy_ac_radio20694_populate_pll_config_tbl failed\n",
					__FUNCTION__));
			goto fail;
		}

		phy_ac_radio20694_pll_config_const_calc(pi, pll);
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
	/* Populate 20696 PLL configuration table */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID)) {
		if (phy_ac_radio20696_populate_pll_config_tbl(pi) != BCME_OK) {
			PHY_ERROR(("%s: phy_ac_radio20696_populate_pll_config_tbl failed\n",
					__FUNCTION__));
			goto fail;
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

	/* Read srom params from nvram */
	phy_ac_radio_nvram_attach(pi, info);

	phy_ac_radio_std_params_attach(info);

	/* make sure the radio is off until we do an "up" */
	phy_ac_radio_switch(info, OFF);

	/* Register PHY type specific implementation */
	bzero(&fns, sizeof(fns));
	fns.ctrl = phy_ac_radio_switch;
	fns.on = phy_ac_radio_on;
#ifdef PHY_DUMP_BINARY
	fns.getlistandsize = phy_ac_radio_getlistandsize;
#endif
	fns.dump = phy_ac_radio_dump;
	fns.id = _phy_ac_radio_query_idcode;
#ifdef RADIO_HEALTH_CHECK
	fns.pll_lock = phy_ac_radio_check_pll_lock;
#endif /* RADIO_HEALTH_CHECK */

	fns.ctx = info;

	phy_ac_radio_init_lpmode(info);

	phy_radio_register_impl(ri, &fns);

	return info;

fail:
	if (pmu_c1_off_info_orig != NULL) {
		phy_mfree(pi, pmu_c1_off_info_orig, sizeof(acphy_pmu_core1_off_radregs_t));
	}
	if (pmu_lp_opt_orig != NULL) {
		phy_mfree(pi, pmu_lp_opt_orig, sizeof(*pmu_lp_opt_orig));
	}
	if (info != NULL) {
		phy_mfree(pi, info, sizeof(phy_ac_radio_mem_t));
	}
	return NULL;
}

void
BCMATTACHFN(phy_ac_radio_unregister_impl)(phy_ac_radio_info_t *info)
{
	phy_info_t *pi;
	phy_radio_info_t *ri;

	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(info->pi->sh->physhim);

	ASSERT(info);
	pi = info->pi;
	ri = info->ri;

	phy_radio_unregister_impl(ri);

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID))
		phy_ac_radio20696_populate_pll_config_mfree(pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

	phy_mfree(pi, info->pmu_c1_off_info_orig, sizeof(acphy_pmu_core1_off_radregs_t));

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		pll_config_20695_tbl_t *pll;

		/* Get pointer to structure */
		pll = wlc_phy_ac_get_20695_pll_config(pi);

		/* Free the allocated memory */
		phy_mfree(pi, pll->reg_addr_2g, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_addr_5g, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_mask_2g,
				(sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_mask_5g,
				(sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_shift_2g,
				(sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_shift_5g,
				(sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_val, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
		phy_mfree(pi, info->chan_info_tbl,
				(sizeof(chan_info_radio20695_rffe_t) * NROWS_CHAN_TUNE_20695));
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		pll_config_20694_tbl_t *pll;

		/* Get pointer to structure */
		pll = wlc_phy_ac_get_20694_pll_config(pi);

		/* Free the allocated memory */
		phy_mfree(pi, pll->reg_addr, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_mask,
				(sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_shift,
				(sizeof(uint8) * PLL_CONFIG_20694_ARRAY_SIZE));
		phy_mfree(pi, pll->reg_field_val, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
		if (sicoreunit == DUALMAC_MAIN) {
			phy_mfree(pi, pll->reg_addr_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
			phy_mfree(pi, pll->reg_field_mask_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
			phy_mfree(pi, pll->reg_field_shift_main,
				(sizeof(uint8) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
			phy_mfree(pi, pll->reg_field_val_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

	phy_mfree(pi, info->pmu_lp_opt_orig, sizeof(acphy_pmu_mimo_lp_opt_radregs_t));

	phy_mfree(pi, info, sizeof(phy_ac_radio_mem_t));
}

static void
BCMATTACHFN(phy_ac_radio_std_params_attach)(phy_ac_radio_info_t *radioi)
{
	radioi->data->dac_mode = 1;
	radioi->prev_subband = 15;
	radioi->data->acphy_4335_radio_pd_status = 0;
	radioi->data->rccal_gmult = 128;
	radioi->data->rccal_gmult_rc = 128;
	radioi->data->rccal_dacbuf = 12;
	/* AFE */
	radioi->afeRfctrlCoreAfeCfg10 = READ_PHYREG(radioi->pi, RfctrlCoreAfeCfg10);
	radioi->afeRfctrlCoreAfeCfg20 = READ_PHYREG(radioi->pi, RfctrlCoreAfeCfg20);
	radioi->afeRfctrlOverrideAfeCfg0 = READ_PHYREG(radioi->pi, RfctrlOverrideAfeCfg0);
	/* Radio RX */
	radioi->rxRfctrlCoreRxPus0 = READ_PHYREG(radioi->pi, RfctrlCoreRxPus0);
	radioi->rxRfctrlOverrideRxPus0 = READ_PHYREG(radioi->pi, RfctrlOverrideRxPus0);
	/* Radio TX */
	radioi->txRfctrlCoreTxPus0 = READ_PHYREG(radioi->pi, RfctrlCoreTxPus0);
	radioi->txRfctrlOverrideTxPus0 = READ_PHYREG(radioi->pi, RfctrlOverrideTxPus0);

	/* {radio, rfpll, pllldo}_pu = 0 */
	radioi->radioRfctrlCmd = READ_PHYREG(radioi->pi, RfctrlCmd);
	radioi->radioRfctrlCoreGlobalPus = READ_PHYREG(radioi->pi, RfctrlCoreGlobalPus);
	radioi->radioRfctrlOverrideGlobalPus = READ_PHYREG(radioi->pi, RfctrlOverrideGlobalPus);

	/* 4349A0: priority flags for 80p80 and RSDB modes
	   these flags indicate which core's tuning settings takes
	   precedence in conflict scenarios
	 */
	radioi->crisscross_priority_core_80p80 = 0;
	radioi->crisscross_priority_core_rsdb = 0;

	radioi->is_crisscross_actv = 0;
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(radioi->pi->pubpi->phy_rev)) {
		radioi->data->acphy_force_lpvco_2G = 1; /* Enable 2G LP mode */
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	{
		radioi->data->acphy_force_lpvco_2G = 0;
	}
	radioi->acphy_lp_mode = 1;
	radioi->acphy_prev_lp_mode = radioi->acphy_lp_mode;
	radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
	radioi->data->ulp_tx_mode = 1;
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(radioi->pi->pubpi->phy_rev)) {
		radioi->data->ulp_adc_mode =
			(uint8) PHY_GETINTVAR_DEFAULT(radioi->pi, rstr_ulpadc, 1);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
		radioi->data->ulp_adc_mode = 0;
	}
}

/* switch radio on/off */
static void
phy_ac_radio_switch(phy_type_radio_ctx_t *ctx, bool on)
{
	phy_ac_radio_info_t *info = (phy_ac_radio_info_t *)ctx;
	phy_info_t *pi = info->pi;
	uint8 core;

	core = 0;
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) &&
		(RADIOMAJORREV(pi) != 3) && (pi->radio_is_on == TRUE) && (!on)) {
		if (phy_get_phymode(pi) == PHYMODE_MIMO) {
			wlc_phy_radio20693_mimo_lpopt_restore(pi);
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	/* Set read override to before mux in 20695 radio */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_28NM(pi, RF, READOVERRIDES, core,
					ReadOverrides_reg, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, READOVERRIDES, core,
					ReadOverrides_reg, 0x0);
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	/* ported from tcl confirm ??  bpeiris (janath) */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		FOREACH_CORE(pi, core) {
			if (core == 0) {
				MOD_RADIO_REG_20694(pi, RF, READOVERRIDES, 0,
					ReadOverrides_reg, 0x1);
			} else if (core == 1) {
				MOD_RADIO_REG_20694(pi, RF, READOVERRIDES, 1,
					ReadOverrides_reg, 0x1);
			}

		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

	/* sync up soft_radio_state with hard_radio_state */
	pi->radio_is_on = FALSE;

	PHY_TRACE(("wl%d: %s %d Phymode: %x\n", pi->sh->unit,
		__FUNCTION__, on, phy_get_phymode(pi)));

	wlc_phy_switch_radio_acphy(pi, on);
}

/* turn radio on */
static void
WLBANDINITFN(phy_ac_radio_on)(phy_type_radio_ctx_t *ctx)
{
	phy_ac_radio_info_t *info = (phy_ac_radio_info_t *)ctx;
	phy_ac_info_t *aci = info->aci;
	phy_info_t *pi = aci->pi;

	/* To make sure that chanspec_set_acphy doesn't get called twice during init */
	phy_ac_chanmgr_get_data(aci->chanmgri)->init_done = FALSE;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	/* update phy_corenum and phy_coremask */
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		phy_ac_update_phycorestate(pi);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

	phy_ac_radio_switch(ctx, ON);

	/* Init regs/tables only once that do not get reset on phy_reset */
	wlc_phy_set_regtbl_on_pwron_acphy(pi);
}

/* Internal data api between ac modules */
phy_ac_radio_data_t *
phy_ac_radio_get_data(phy_ac_radio_info_t *radioi)
{
	return radioi->data;
}

void phy_ac_radio_set_modes(phy_ac_radio_info_t *radioi, uint8 dac, uint8 adc)
{
	radioi->data->ulp_tx_mode = dac;
	radioi->data->ulp_adc_mode = adc;
}

/* query radio idcode */
uint32
phy_ac_radio_query_idcode(phy_info_t *pi)
{
	uint32 b0, b1;

	W_REG(pi->sh->osh, &pi->regs->radioregaddr, 0);
#ifdef __mips__
	(void)R_REG(pi->sh->osh, &pi->regs->radioregaddr);
#endif
	b0 = (uint32)R_REG(pi->sh->osh, &pi->regs->radioregdata);
	W_REG(pi->sh->osh, &pi->regs->radioregaddr, 1);
#ifdef __mips__
	(void)R_REG(pi->sh->osh, &pi->regs->radioregaddr);
#endif
	b1 = (uint32)R_REG(pi->sh->osh, &pi->regs->radioregdata);

	/* For ACPHY (starting with 4360A0), address 0 has the revid and
	   address 1 has the devid
	*/
	return (b0 << 16) | b1;
}

/* parse radio idcode and write to pi->pubpi */
void
phy_ac_radio_parse_idcode(phy_info_t *pi, uint32 idcode)
{
	PHY_TRACE(("%s: idcode 0x%08x\n", __FUNCTION__, idcode));

	pi->pubpi->radioid = (idcode & IDCODE_ACPHY_ID_MASK) >> IDCODE_ACPHY_ID_SHIFT;
	pi->pubpi->radiorev = (idcode & IDCODE_ACPHY_REV_MASK) >> IDCODE_ACPHY_REV_SHIFT;

	/* ACPHYs do not use radio ver. This param is invalid */
	pi->pubpi->radiover = 0;
}

void
wlc_phy_lp_mode(phy_ac_radio_info_t *ri, int8 lp_mode)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(ri->pi->pubpi->phy_rev) || ACMAJORREV_3(ri->pi->pubpi->phy_rev)) {
		if (lp_mode == 1) {
			/* AC MODE (Full Pwr mode) */
			ri->acphy_lp_mode = 1;
		} else if (lp_mode == 2) {
			/* 11n MODE (VCO in 11n Mode) */
			ri->acphy_lp_mode = 2;
		} else if (lp_mode == 3) {
			/* Low pwr MODE */
			ri->acphy_lp_mode = 3;
		} else {
			return;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	{
		return;
	}
}

static void
BCMATTACHFN(phy_ac_radio_init_lpmode)(phy_ac_radio_info_t *ri)
{
	ri->lpmode_2g = ACPHY_LPMODE_NONE;
	ri->lpmode_5g = ACPHY_LPMODE_NONE;

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_3)))
	if ((ACMAJORREV_1(ri->pi->pubpi->phy_rev) && ACMINORREV_2(ri->pi)) ||
	    ACMAJORREV_3(ri->pi->pubpi->phy_rev)) {
		switch (BF3_ACPHY_LPMODE_2G(ri->aci)) {
			case 1:
				ri->lpmode_2g = ACPHY_LPMODE_LOW_PWR_SETTINGS_1;
				break;
			case 2:
				ri->lpmode_2g = ACPHY_LPMODE_LOW_PWR_SETTINGS_2;
				break;
			case 3:
				ri->lpmode_2g = ACPHY_LPMODE_NORMAL_SETTINGS;
				break;
			case 0:
			default:
				ri->lpmode_2g = ACPHY_LPMODE_NONE;
		}

		switch (BF3_ACPHY_LPMODE_5G(ri->aci)) {
			case 1:
				ri->lpmode_5g = ACPHY_LPMODE_LOW_PWR_SETTINGS_1;
				break;
			case 2:
				ri->lpmode_5g = ACPHY_LPMODE_LOW_PWR_SETTINGS_2;
				break;
			case 3:
				ri->lpmode_5g = ACPHY_LPMODE_NORMAL_SETTINGS;
				break;
			case 0:
			default:
				ri->lpmode_5g = ACPHY_LPMODE_NONE;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_3))) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(ri->pi->pubpi->phy_rev)) {
		switch (BF3_ACPHY_LPMODE_2G(ri->aci)) {
			case 1:
				ri->lpmode_2g = ACPHY_LPMODE_LOW_PWR_SETTINGS_1;
				break;
			case 0:
			default:
				ri->lpmode_2g = ACPHY_LPMODE_NONE;
		}

		switch (BF3_ACPHY_LPMODE_5G(ri->aci)) {
			case 1:
				ri->lpmode_5g = ACPHY_LPMODE_LOW_PWR_SETTINGS_1;
				break;
			case 0:
			default:
				ri->lpmode_5g = ACPHY_LPMODE_NONE;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
	{
	}
}

void
acphy_set_lpmode(phy_info_t *pi, acphy_lp_opt_levels_t lp_opt_lvl)
{
	phy_ac_radio_info_t *ri;
	ri = pi->u.pi_acphy->radioi;
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_3)))
	if ((ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi)) ||
	    ACMAJORREV_3(pi->pubpi->phy_rev)) {
		switch (lp_opt_lvl) {
		case ACPHY_LP_RADIO_LVL_OPT:
			if (CHSPEC_IS2G(pi->radio_chanspec)) {
				if (ri->lpmode_2g == ACPHY_LPMODE_LOW_PWR_SETTINGS_1) {
					wlc_phy_radio_vco_opt(pi, ACPHY_VCO_2P5V);
				} else if (ri->lpmode_2g == ACPHY_LPMODE_LOW_PWR_SETTINGS_2) {
					wlc_phy_radio_vco_opt(pi, ACPHY_VCO_1P35V);
				}
			} else {
				/* lpmode reset for 5G */
				MOD_RADIO_REG_20691(pi, PLL_VCO3, 0, rfpll_vco_cvar_extra, 0xf);
				MOD_RADIO_REG_20691(pi, PLL_VCO2, 0, rfpll_vco_cvar, 0x8);
				MOD_RADIO_REG_20691(pi, PLL_VCO6, 0, rfpll_vco_bias_mode, 0x1);
				MOD_RADIO_REG_20691(pi, PLL_VCO6, 0, rfpll_vco_ALC_ref_ctrl, 0xf);
				MOD_RADIO_REG_20691(pi, PLL_CP4, 0, rfpll_cp_kpd_scale, 0x34);
				if (CHIPID(pi->sh->chip) != BCM4364_CHIP_ID) {
					MOD_RADIO_REG_20691(pi, PLL_XTALLDO1, 0,
							ldo_1p2_xtalldo1p2_lowquiescenten, 0x0);
				}
				MOD_RADIO_REG_20691(pi, PLL_HVLDO2, 0,
					ldo_2p5_lowquiescenten_VCO, 0x0);
				MOD_RADIO_REG_20691(pi, PLL_HVLDO2, 0,
					ldo_2p5_lowquiescenten_CP, 0x0);
				MOD_RADIO_REG_20691(pi, PLL_HVLDO4, 0, ldo_2p5_static_load_CP, 0x0);
				MOD_RADIO_REG_20691(pi, PLL_HVLDO4, 0,
					ldo_2p5_static_load_VCO, 0x0);
				MOD_RADIO_REG_20691(pi, PLL_CFG3, 0, rfpll_spare1, 0x0);
				MOD_RADIO_REG_20691(pi, PLL_CFG3, 0, rfpll_spare0, 0x34);
			}
			break;
		case ACPHY_LP_CHIP_LVL_OPT:
		case ACPHY_LP_PHY_LVL_OPT:
		default:
			break;
		}
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_1) || defined(PHY_ACMAJORREV_3))) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		/* 4349 related power optimizations */
		bool for_2g = ((ri->lpmode_2g != ACPHY_LPMODE_NONE) &&
			(CHSPEC_IS2G(pi->radio_chanspec)));
		bool for_5g = ((ri->lpmode_5g != ACPHY_LPMODE_NONE) &&
			(CHSPEC_IS5G(pi->radio_chanspec)));
		phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);

		switch (lp_opt_lvl) {
		case ACPHY_LP_RADIO_LVL_OPT:
		{
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
			if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) &&
					(phy_get_phymode(pi) == PHYMODE_MIMO) &&
					((for_2g == TRUE) || (for_5g == TRUE))) {
				uint8 new_rxchain = stf_shdata->phyrxchain;
				uint8 old_rxchain = phy_ac_chanmgr_get_data
					(ri->aci->chanmgri)->phyrxchain_old;
				uint8 to_cm1_rx = (new_rxchain == 1) &&
					((old_rxchain == 2) || (old_rxchain == 3));
				uint8 rx_cm1_to_cm1 = (new_rxchain == 1) && (old_rxchain == 1);

			PHY_TRACE(("[%s]chains: newrx:%d oldrx: %d newtx:%d both_txrxchain:%d\n",
				__FUNCTION__, stf_shdata->phyrxchain,
				phy_ac_chanmgr_get_data
				(ri->aci->chanmgri)->phyrxchain_old,
				stf_shdata->phytxchain, phy_ac_chanmgr_get_data
				(ri->aci->chanmgri)->both_txchain_rxchain_eq_1));

				wlc_phy_radio20693_mimo_lpopt_restore(pi);
				if (to_cm1_rx || rx_cm1_to_cm1) {
					wlc_phy_radio20693_mimo_cm1_lpopt(pi);
				} else {
					wlc_phy_radio20693_mimo_cm23_lp_opt(pi, new_rxchain);
				}
			}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
		}
		break;
		case ACPHY_LP_CHIP_LVL_OPT:
			break;
		case ACPHY_LP_PHY_LVL_OPT:
			if ((for_5g == TRUE) && (stf_shdata->phyrxchain != 2)) {
				MOD_PHYREG(pi, CRSMiscellaneousParam, bphy_pre_det_en, 1);
				MOD_PHYREG(pi, CRSMiscellaneousParam,
					b_over_ag_falsedet_en, 0);
			} else {
				MOD_PHYREG(pi, CRSMiscellaneousParam, bphy_pre_det_en, 0);
				MOD_PHYREG(pi, CRSMiscellaneousParam,
					b_over_ag_falsedet_en, 1);
			}
			if (!((phy_get_phymode(pi) == PHYMODE_RSDB) &&
				(phy_get_current_core(pi) == 1))) {
				uint16 val;
				wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_BFECONFIG2X2TBL,
					1, 0, 16, &val);
				BCM_REFERENCE(val);
			}
			/*
			if {[hwaccess] == $def(hw_jtag)} {
				jtag w 0x18004492 2 0x0
				jtag w 0x18001492 2 0x2
			}
			*/

			WRITE_PHYREG(pi, FFTSoftReset, 0x2);
			break;
		default:
			break;
		}

	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
	{
	}
}

void
wlc_phy_force_lpvco_2G(phy_info_t *pi, int8 force_lpvco_2G)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if ACMAJORREV_1(pi->pubpi->phy_rev)	{
		phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
		pi_ac->radioi->data->acphy_force_lpvco_2G = force_lpvco_2G;
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
	{
		return;
	}
}

/* Proc to power on dac_clocks though override */
bool wlc_phy_poweron_dac_clocks(phy_info_t *pi, uint8 core, uint16 *orig_dac_clk_pu,
	uint16 *orig_ovr_dac_clk_pu)
{
	uint16 dacpwr;

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		dacpwr = 0;
		/* This condition is put to prevent wrong reg access */
		ASSERT(0);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
		dacpwr = READ_RADIO_REGFLD_TINY(pi, TX_DAC_CFG1, core, DAC_pwrup);

		if (dacpwr == 0) {
			*orig_dac_clk_pu = READ_RADIO_REGFLD_TINY(pi, CLK_DIV_CFG1, core,
					dac_clk_pu);
			*orig_ovr_dac_clk_pu = READ_RADIO_REGFLD_TINY(pi, CLK_DIV_OVR1, core,
					ovr_dac_clk_pu);
			MOD_RADIO_REG_TINY(pi, CLK_DIV_CFG1, core, dac_clk_pu, 1);
			MOD_RADIO_REG_TINY(pi, CLK_DIV_OVR1, core, ovr_dac_clk_pu, 1);
		}
	}

	return (dacpwr == 0);
}

/* Proc to resotre dac_clock_pu and the corresponding ovrride registers */
void wlc_phy_restore_dac_clocks(phy_info_t *pi, uint8 core, uint16 orig_dac_clk_pu,
	uint16 orig_ovr_dac_clk_pu)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		/* This condition is put to prevent wrong reg access */
		ASSERT(0);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
		MOD_RADIO_REG_TINY(pi, CLK_DIV_CFG1, core, dac_clk_pu, orig_dac_clk_pu);
		MOD_RADIO_REG_TINY(pi, CLK_DIV_OVR1, core, ovr_dac_clk_pu, orig_ovr_dac_clk_pu);
	}
}

#if ((defined(BCMDBG) || defined(BCMDBG_DUMP)) && (defined(BCMINTPHYDBG) || \
	defined(DBG_PHY_IOV))) || defined(BCMDBG_PHYDUMP)
static int
phy_ac_radio_dump(phy_type_radio_ctx_t *ctx, struct bcmstrbuf *b)
{
	phy_ac_radio_info_t *gi = (phy_ac_radio_info_t *)ctx;
	phy_info_t *pi = gi->pi;
	const char *name = NULL;
	int i, jtag_core;
	uint16 addr = 0;
	const radio_20xx_dumpregs_t *radio20xxdumpregs = NULL;


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
		name = "20693";
		if ((RADIO20693_MAJORREV(pi->pubpi->radiorev) == 1) ||
			(RADIO20693_MAJORREV(pi->pubpi->radiorev) == 2)) {
			uint16 phymode = phy_get_phymode(pi);
			if (phymode == PHYMODE_RSDB)
				radio20xxdumpregs = dumpregs_20693_rsdb;
			else if (phymode == PHYMODE_MIMO)
				radio20xxdumpregs = dumpregs_20693_mimo;
			else if (phymode == PHYMODE_80P80)
				radio20xxdumpregs = dumpregs_20693_80p80;
			else
				ASSERT(0);
		} else if (RADIO20693_MAJORREV(pi->pubpi->radiorev) == 3) {
			radio20xxdumpregs = dumpregs_20693_rev32;
		} else {
			return BCME_NOTFOUND;
		}
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
	if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
		name = "2069";
		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
			radio20xxdumpregs = dumpregs_2069_rev32;
		} else if ((RADIO2069REV(pi->pubpi->radiorev) == 16) ||
			(RADIO2069REV(pi->pubpi->radiorev) == 18)) {
			radio20xxdumpregs = dumpregs_2069_rev16;
		} else if (RADIO2069REV(pi->pubpi->radiorev) == 17) {
			radio20xxdumpregs = dumpregs_2069_rev17;
		} else if (RADIO2069REV(pi->pubpi->radiorev) == 25) {
			radio20xxdumpregs = dumpregs_2069_rev25;
		} else if (RADIO2069REV(pi->pubpi->radiorev) == 64) {
			radio20xxdumpregs = dumpregs_2069_rev64;
		} else if (RADIO2069REV(pi->pubpi->radiorev) == 66) {
			radio20xxdumpregs = dumpregs_2069_rev64;
		} else {
			radio20xxdumpregs = dumpregs_2069_rev0;
		}
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
		name = "20691";
		switch (RADIO20691REV(pi->pubpi->radiorev)) {
		case 60:
		case 68:
			radio20xxdumpregs = dumpregs_20691_rev68;
			break;
		case 75:
			radio20xxdumpregs = dumpregs_20691_rev75;
			break;
		case 79:
			radio20xxdumpregs = dumpregs_20691_rev79;
			break;
		case 74:
		case 82:
			radio20xxdumpregs = dumpregs_20691_rev82;
			break;
		case 129:
			radio20xxdumpregs = dumpregs_20691_rev129;
			break;
		default:
			;
		}
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		name = "20694";
		switch (HW_RADIOREV(pi->pubpi->radiorev)) {
		   case 5:
				radio20xxdumpregs = dumpregs_20694_rev5;
		   break;
		   case 8:
				radio20xxdumpregs = dumpregs_20694_rev8;
		   break;
		   case 9:
				radio20xxdumpregs = dumpregs_20694_rev9;
		   break;
		   case 36:
				radio20xxdumpregs = dumpregs_20694_rev36;
			 break;
		default:
			return 0;
		}
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
	    name = "20695";
		radio20xxdumpregs = dumpregs_20695_rev39;
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

		return BCME_ERROR;

	i = 0;
	ASSERT(radio20xxdumpregs != NULL);    /* to indicate developer to put in the table */
	if (radio20xxdumpregs == NULL)
		return BCME_OK; /* If the table is not yet implemented, */
						/* just skip the output (for non-assert builds) */

	bcm_bprintf(b, "----- %08s -----\n", name);
	bcm_bprintf(b, "Add Value");
	if (PHYCORENUM(pi->pubpi->phy_corenum) > 1)
		bcm_bprintf(b, "0 Value1 ...");
	bcm_bprintf(b, "\n");

	while ((addr = radio20xxdumpregs[i].address) != 0xffff) {

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM2069) || defined(RADIO_BCM20694) ))
		if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID) ||
			RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
			jtag_core = (addr & JTAG_2069_MASK);
			addr &= (~JTAG_2069_MASK);
		} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM2069 || defined(RADIO_BCM20694)) )) */
			jtag_core = 0;

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
		if ((RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) &&
			(jtag_core == JTAG_2069_ALL)) {
			switch (PHYCORENUM(pi->pubpi->phy_corenum)) {
			case 1:
				bcm_bprintf(b, "%03x %04x\n", addr,
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR0));
				break;
			case 2:
				bcm_bprintf(b, "%03x %04x %04x\n", addr,
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR0),
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR1));
				break;
			case 3:
				bcm_bprintf(b, "%03x %04x %04x %04x\n", addr,
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR0),
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR1),
					phy_utils_read_radioreg(pi, addr | JTAG_2069_CR2));
				break;
			default:
				break;
			}
		} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
		{
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
			if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) &&
			    (RADIOMAJORREV(pi) == 3)) {
				if ((addr >> JTAG_20693_SHIFT) ==
				    (JTAG_20693_CR0 >> JTAG_20693_SHIFT)) {
					bcm_bprintf(b, "%03x %04x %04x %04x %04x\n", addr,
						phy_utils_read_radioreg(pi, addr | JTAG_20693_CR0),
						phy_utils_read_radioreg(pi, addr | JTAG_20693_CR1),
						phy_utils_read_radioreg(pi, addr | JTAG_20693_CR2),
						phy_utils_read_radioreg(pi, addr | JTAG_20693_CR3));
				} else if ((addr >> JTAG_20693_SHIFT) ==
				           (JTAG_20693_PLL0 >> JTAG_20693_SHIFT)) {
					bcm_bprintf(b, "%03x %04x %04x\n", addr,
					phy_utils_read_radioreg(pi, addr | JTAG_20693_PLL0),
					phy_utils_read_radioreg(pi, addr | JTAG_20693_PLL1));
				}
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
			{
			bcm_bprintf(b, "%03x %04x\n", addr,
			            phy_utils_read_radioreg(pi, addr | jtag_core));
			}
		}
		i++;
	}

	return BCME_OK;
}
#endif /* BCMDBG, BCMDBG_DUMP, BCMINTPHYDBG, DBG_PHY_IOV, BCMDBG_PHYDUMP */

static void
BCMATTACHFN(phy_ac_radio_nvram_attach)(phy_info_t *pi, phy_ac_radio_info_t *radioi)
{
#ifndef BOARD_FLAGS3
	uint32 bfl3; /* boardflags3 */
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
#endif

	if (BF3_RCAL_OTP_VAL_EN(pi->u.pi_acphy) == 1) {
		if (!otp_read_word(pi->sh->sih, ACPHY_RCAL_OFFSET, &pi->sromi->rcal_otp_val)) {
			pi->sromi->rcal_otp_val &= 0xf;
		} else {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
			if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
				if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
					pi->sromi->rcal_otp_val = ACPHY_RCAL_VAL_2X2;
				} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
					pi->sromi->rcal_otp_val = ACPHY_RCAL_VAL_1X1;
				}
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
				pi->sromi->rcal_otp_val = ACPHY_RCAL_VAL_1X1;
			}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
		}
	}

	if (CHIPID(pi->sh->chip) == BCM4364_CHIP_ID) {
		uint16 rcal_value = 0;
		if (!otp_read_word(pi->sh->sih, ACPHY_RCAL_OFFSET, &pi->sromi->rcal_otp_val)) {
			rcal_value = (pi->sromi->rcal_otp_val & 0x300) >> 8;
			otp_read_word(pi->sh->sih, 0x14, &pi->sromi->rcal_otp_val);
			pi->sromi->rcal_otp_val = ((pi->sromi->rcal_otp_val &
				0x3) << 2) | rcal_value;
		}
	} else if ((CHIPID(pi->sh->chip) == BCM4345_CHIP_ID) &&
		(RADIO20691_MAJORREV(pi->pubpi->radiorev) == 1) &&
		(RADIO20691_MINORREV(pi->pubpi->radiorev) >= 5)) {
		if (!otp_read_word(pi->sh->sih, ACPHY_RCAL_OFFSET, &pi->sromi->rcal_otp_val))
			pi->sromi->rcal_otp_val = (pi->sromi->rcal_otp_val & 0xf00) >> 8;
	}

	pi->dacratemode2g = (uint8) (PHY_GETINTVAR_DEFAULT_SLICE(pi, rstr_dacratemode2g, 1));
	pi->dacratemode5g = (uint8) (PHY_GETINTVAR_DEFAULT_SLICE(pi, rstr_dacratemode5g, 1));
	radioi->data->srom_txnospurmod5g = (uint8)PHY_GETINTVAR_DEFAULT_SLICE(pi,
		rstr_txnospurmod5g, 1);
	radioi->data->srom_txnospurmod2g = (uint8)PHY_GETINTVAR_DEFAULT_SLICE(pi,
		rstr_txnospurmod2g, 0);
	radioi->data->vcodivmode = (uint8) (PHY_GETINTVAR_DEFAULT_SLICE(pi, rstr_vcodivmode, 0));

#ifndef BOARD_FLAGS3
	if ((PHY_GETVAR_SLICE(pi, rstr_boardflags3)) != NULL) {
		bfl3 = (uint32)PHY_GETINTVAR_SLICE(pi, rstr_boardflags3);
		BF3_RCAL_WAR(pi_ac) = ((bfl3 & BFL3_RCAL_WAR) != 0);
		BF3_RCAL_OTP_VAL_EN(pi_ac) = ((bfl3 & BFL3_RCAL_OTP_VAL_EN) != 0);
		BF3_TSSI_DIV_WAR(pi_ac) = (bfl3 & BFL3_TSSI_DIV_WAR) >> BFL3_TSSI_DIV_WAR_SHIFT;
	} else {
		BF3_RCAL_WAR(pi_ac) = 0;
		BF3_RCAL_OTP_VAL_EN(pi_ac) = 0;
		BF3_TSSI_DIV_WAR(pi_ac) = 0;
	}
#endif /* BOARD_FLAGS3 */
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
/* 20695 radio related functions */
void
wlc_phy_radio20695_afe_div_ratio(phy_info_t *pi, uint8 ulp_tx_mode, uint8 ulp_adc_mode)
{
	uint16 fc;
	uint8 *adc_div_lut;
	uint8 *dac_div_lut;
	uint8 *dac_bypN2_lut;
	uint8 *adc_bypN2_lut;
	uint8 adc_bypN2 = 0;
	uint8 adc_div, dac_div, dac_bypN2;

	/* ADC div lut for different channels */
	/* ADC rates for adc_mode 0:3 are 41/50/180/120mhz */
	uint8 adc_div_lut_ge_5745[] = {40, 13, 4, 6};
	uint8 adc_div_lut_ge_5180[] = {40, 13, 4, 6};
	uint8 adc_div_lut_ge_4905[] = {39, 12, 7, 5};
	uint8 adc_div_lut_2g[] = {14, 12, 7, 5};

	uint8 adc_bypN2_lut_ge_5745[] = {0, 0, 0, 0};
	uint8 adc_bypN2_lut_ge_5180[] = {0, 0, 0, 0};
	uint8 adc_bypN2_lut_ge_4905[] = {0, 0, 1, 0};
	uint8 adc_bypN2_lut_2g[] = {0, 0, 1, 0};

	/* DAC div lut for different channels */
	/* DAC rates for mode 0:4 are 360/240/180/120/90mhz */
	uint8 dac_div_lut_ge_5745[] = {4, 6, 8, 12, 40};
	uint8 dac_div_lut_ge_5180[] = {4, 6, 8, 12, 14};
	uint8 dac_div_lut_ge_4905[] = {7, 5, 7, 10, 13};
	uint8 dac_div_lut_2g[] = {7, 5, 7, 10, 13};
	uint8 dac_bypN2_lut_ge_5745[] = {0, 0, 0, 0, 0};
	uint8 dac_bypN2_lut_ge_5180[] = {0, 0, 0, 0, 0};
	uint8 dac_bypN2_lut_ge_4905[] = {1, 0, 0, 0, 0};
	uint8 dac_bypN2_lut_2g[] = {1, 0, 0, 0, 0};

	/* Get center frequency of the channel */
	if (CHSPEC_CHANNEL(pi->radio_chanspec) > 14) {
		fc = CHAN5G_FREQ(CHSPEC_CHANNEL(pi->radio_chanspec));
	} else {
		fc = CHAN2G_FREQ(CHSPEC_CHANNEL(pi->radio_chanspec));
	}

	if (ulp_adc_mode == 2) {
		/* 180MHz mode */
		MOD_RADIO_REG_28NM(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_power_mode, 1);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, rxadc_power_mode, 3);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_REG1, 0, rxadc_reg1_15_7, 0);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_REG0, 0, rxadc_reg0_7_0, 0x80);
	} else {
		MOD_RADIO_REG_28NM(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_power_mode, 1);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, rxadc_power_mode, 0);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_REG1, 0, rxadc_reg1_15_7, 0x6);
		MOD_RADIO_REG_28NM(pi, RF, RXADC_REG0, 0, rxadc_reg0_7_0, 0x0);
	}

	/* Based on channel frequency, select appropriate LUT */
	if (fc >= 5745) {
		adc_div_lut = adc_div_lut_ge_5745;
		dac_div_lut = dac_div_lut_ge_5745;
		adc_bypN2_lut = adc_bypN2_lut_ge_5745;
		dac_bypN2_lut = dac_bypN2_lut_ge_5745;
	} else if (fc >= 5180) {
		adc_div_lut = adc_div_lut_ge_5180;
		dac_div_lut = dac_div_lut_ge_5180;
		adc_bypN2_lut = adc_bypN2_lut_ge_5180;
		dac_bypN2_lut = dac_bypN2_lut_ge_5180;
	} else if (fc >= 4905) {
		adc_div_lut = adc_div_lut_ge_4905;
		dac_div_lut = dac_div_lut_ge_4905;
		adc_bypN2_lut = adc_bypN2_lut_ge_4905;
		dac_bypN2_lut = dac_bypN2_lut_ge_4905;
	} else {
		adc_div_lut = adc_div_lut_2g;
		dac_div_lut = dac_div_lut_2g;
		adc_bypN2_lut = adc_bypN2_lut_2g;
		dac_bypN2_lut = dac_bypN2_lut_2g;
	}

	adc_div = adc_div_lut[ulp_adc_mode];
	adc_bypN2 = adc_bypN2_lut[ulp_adc_mode];

	/* ulp_tx_mode goes from 1 to 5, subtract 1 */
	dac_div = dac_div_lut[ulp_tx_mode - 1];
	dac_bypN2 = dac_bypN2_lut[ulp_tx_mode - 1];

	if (fc < 3000) {
		/* 2G */
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_adc, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_dac, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_adc_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_dac_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_adc_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_dac_en, 0x1)

			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG4, 0, afediv5g_pu, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG5, 0, afediv5g_adc_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG5, 0, afediv5g_dac_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv_rst_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG4, 0, afediv2g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_adc_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_dac_en, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);


		if (RADIOMAJORREV(pi) >= 2) {
			//43012b0
			 //setting ntssi override to zero
			MOD_RADIO_REG_20695(pi, RF, SPARE_CFG1, 0, ovr_afediv2g_adc_Ntssi, 1);
			//setting bypN2 override to one
			MOD_RADIO_REG_20695(pi, RF, SPARE_CFG1, 0, ovr_afediv2g_adc_bypN2, 1);
		}
		//will be controled from direct line in b0

		MOD_RADIO_REG_20695_MULTI_3(pi, RF, AFEDIV2G_REG1, 0,
				afediv2g_adc_bypN2, adc_bypN2,
				afediv2g_dac_bypN2, dac_bypN2,
				afediv2g_adc_Ntssi, 0x0);
		MOD_RADIO_REG_20695(pi, RF, AFEDIV2G_REG3, 0, afediv2g_adc_div, adc_div);
		MOD_RADIO_REG_20695(pi, RF, AFEDIV2G_REG2, 0, afediv2g_dac_div, dac_div);
		MOD_RADIO_REG_20695(pi, RF, TXMIX2G_CFG4, 0, sel_lpf_CM_bias_5g, 0);
	} else {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_adc, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_dac, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_adc_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0,
					ovr_afediv5g_dac_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_adc, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_dac, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0,
					ovr_afediv2g_dac_en, 0x1)


			/* RST sequence */
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG4, 0, afediv2g_pu, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_adc_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_dac_en, 0x0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv_rst_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG4, 0, afediv5g_pu, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG5, 0, afediv5g_adc_en, 0x1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_REG5, 0, afediv5g_dac_en, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);


		if (RADIOMAJORREV(pi) >= 2) {
			//43012b0
			 //setting ntssi override to zero
			MOD_RADIO_REG_20695(pi, RF, SPARE_CFG1, 0, ovr_afediv5g_adc_Ntssi, 1);
			//setting bypN2 override to one
			MOD_RADIO_REG_20695(pi, RF, SPARE_CFG1, 0, ovr_afediv5g_adc_bypN2, 1);
		}
		//will be controled from direct line in b0


		MOD_RADIO_REG_20695_MULTI_3(pi, RF, AFEDIV5G_REG1, 0,
				afediv5g_adc_bypN2, adc_bypN2,
				afediv5g_dac_bypN2, dac_bypN2,
				afediv5g_adc_Ntssi, 0x0);
		MOD_RADIO_REG_20695(pi, RF, AFEDIV5G_REG3, 0, afediv5g_adc_div, adc_div);
		MOD_RADIO_REG_20695(pi, RF, AFEDIV5G_REG2, 0, afediv5g_dac_div, dac_div);
		MOD_RADIO_REG_20695_MULTI_2(pi, RF, TX5G_CFG1_OVR, 0, ovr_pad5g_idac_gm, 1,
				ovr_pad5g_idac_pmos, 1);
		MOD_RADIO_REG_20695(pi, RF, TXMIX2G_CFG4, 0, sel_lpf_CM_bias_5g, 1);
	}

	if (PHY_NAP_ENAB(pi->sh->physhim)) {
		/* Clear afediv_adc_en and afediv_dac_en override
		   These will be controlled using direct lines
		*/
		wlc_phy_radio20695_clear_adc_dac_en_ovr(pi);
	}
}

static int
BCMATTACHFN(wlc_phy_ac_populate_20695_chan_info_tbl)(phy_info_t *pi, phy_ac_radio_info_t *radioi)
{
	int ret_status = BCME_OK;
	chan_info_radio20695_rffe_t *chan_info_fcbga = NULL;
	chan_info_radio20695_rffe_t *chan_info_wlbga = NULL;
	chan_info_radio20695_rffe_t *chan_info_wlcsp = NULL;
	chan_info_radio20695_rffe_t *chan_info = NULL;
	uint pkg_id = pi->sh->chippkg;	/* Chip package info */

	if ((radioi->chan_info_tbl = (void *)phy_malloc(pi,
			sizeof(chan_info_radio20695_rffe_t) * NROWS_CHAN_TUNE_20695)) == NULL) {
		PHY_ERROR(("%s: phy_malloc failed\n", __FUNCTION__));
		ret_status = BCME_NOMEM;
		goto status_return;
	}

	switch (RADIOREV(pi->pubpi->radiorev)) {
		case 32:
			/* A0 EPA variant */
			chan_info_fcbga = chan_tune_20695_rev32_fcbga;
			chan_info_wlbga = chan_tune_20695_rev32_wlbga;
			chan_info_wlcsp = chan_tune_20695_rev32_fcbga;
			break;

		case 33:
			/* A0 IPA variant */
			chan_info_fcbga = chan_tune_20695_rev33;
			break;

		case 38:
			/* B0 EPA variant */
			chan_info_fcbga = chan_tune_20695_rev38_fcbga;
			chan_info_wlbga = chan_tune_20695_rev38_wlbga;
			chan_info_wlcsp = chan_tune_20695_rev38_fcbga;
			break;

		case 39:
			/* B0 IPA variant */
			chan_info_fcbga = chan_tune_20695_rev39_fcbga;
			chan_info_wlbga = chan_tune_20695_rev39_wlbga;
			chan_info_wlcsp = chan_tune_20695_rev39_fcbga;
			break;

		default:
			PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
				pi->sh->unit, __FUNCTION__, RADIOREV(pi->pubpi->radiorev)));
			ROMMABLE_ASSERT(FALSE);
			ret_status = BCME_ERROR;
	}

	/* Selection of table based on package type */
	if (pkg_id == BCM943012_WLBGA_PKG_ID) {
		/* WLBGA package */
		chan_info = chan_info_wlbga;
	} else if ((pkg_id == BCM943012_FCBGA_PKG_ID) ||
		(pkg_id == BCM943012_FCBGAWE_PKG_ID)) {
		/* FCBGA package */
		chan_info = chan_info_fcbga;
	} else if ((pkg_id == BCM943012_WLCSPWE_PKG_ID) ||
		(pkg_id == BCM943012_WLCSPOLY_PKG_ID)) {
		/* WLCSP (same as FCBGA) package */
		chan_info = chan_info_wlcsp;
	} else {
		PHY_ERROR(("wl%d: %s: Unsupported package id %d\n",
			pi->sh->unit, __FUNCTION__, pkg_id));
		ROMMABLE_ASSERT(FALSE);
		ret_status = BCME_ERROR;
	}

	/* Copy the chan info table to allocated memory */
	if (chan_info != NULL) {
		memcpy(radioi->chan_info_tbl, chan_info,
			(sizeof(chan_info_radio20695_rffe_t) * NROWS_CHAN_TUNE_20695));
		radioi->chan_info_tbl_len = NROWS_CHAN_TUNE_20695;
	}

status_return:
	if (ret_status != BCME_OK) {
		phy_mfree(pi, radioi->chan_info_tbl,
				(sizeof(chan_info_radio20695_rffe_t) * NROWS_CHAN_TUNE_20695));
	}

	return ret_status;
}

static pll_config_20695_tbl_t *
BCMRAMFN(wlc_phy_ac_get_20695_pll_config)(phy_info_t *pi)
{
	return &pll_conf_20695;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static pll_config_20694_tbl_t *
BCMRAMFN(wlc_phy_ac_get_20694_pll_config)(phy_info_t *pi)
{
	return &pll_conf_20694;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
int
wlc_phy_chan2freq_20695(phy_info_t *pi, uint8 channel,
	const chan_info_radio20695_rffe_t **chan_info)
{
	uint32 index, tbl_len;
	chan_info_radio20695_rffe_t *p_chan_info_tbl = NULL;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	if (!RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		PHY_ERROR(("wl%d: %s: Unsupported radio %d\n",
			pi->sh->unit, __FUNCTION__, RADIOID(pi->pubpi->radioid)));
		ASSERT(FALSE);
		return -1;
	}

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	tbl_len = pi_ac->radioi->chan_info_tbl_len;
	p_chan_info_tbl = (chan_info_radio20695_rffe_t *)pi_ac->radioi->chan_info_tbl;

	if (p_chan_info_tbl != NULL) {
		for (index = 0; index < tbl_len && p_chan_info_tbl[index].channel != channel;
			index++);

		if (index >= tbl_len) {
			PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
				pi->sh->unit, __FUNCTION__, channel));
			if (!ISSIM_ENAB(pi->sh->sih)) {
				/* Do not assert on QT since we leave the tables empty on purpose */
				ASSERT(index < tbl_len);
			}
			return -1;
		}

		*chan_info = p_chan_info_tbl + index;
		return p_chan_info_tbl[index].freq;
	} else {
		return -1;
	}
}

static void
chanspec_tune_radio_20695(phy_info_t *pi)
{
	uint16 bbmult, bbmult_zero = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
		pi->dacratemode2g : pi->dacratemode5g;

	/* Configure ulp tx mode */
	wlc_phy_dac_rate_mode_acphy(pi, pi_ac->radioi->data->dac_mode);

	if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac) || CCT_BAND_CHG(pi_ac)) {
		wlc_phy_get_tx_bbmult_acphy(pi, &bbmult, 0);
		wlc_phy_set_tx_bbmult_acphy(pi, &bbmult_zero, 0);
		wlc_phy_radio_afecal(pi);
		wlc_phy_set_tx_bbmult_acphy(pi, &bbmult, 0);
	}

	/* Configure DAC buffer BW */
	if (PHY_PAPDEN(pi)) {
		wlc_phy_radio20695_txdac_bw_setup(pi, BUTTERWORTH, DACBW_120);
	} else {
		wlc_phy_radio20695_txdac_bw_setup(pi, BUTTERWORTH, DACBW_30);
	}

	/* forcing reg bits for ADC power up  */
	MOD_RADIO_REG_28NM(pi, RF, RXADC_REG0, 0, rxadc_force_resetb, 1);
	MOD_RADIO_REG_28NM(pi, RF, RXADC_REG1, 0, rxadc_pu_clkgen, 1);
	MOD_RADIO_REG_28NM(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_rx_bq1, 1);
	MOD_RADIO_REG_28NM(pi, RF, TIA_REG18, 0, tia_sw_rx_bq1, 1);
}

static void
wlc_phy_radio20695_clear_adc_dac_en_ovr(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0, ovr_afediv5g_adc_en, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV5G_CFG1_OVR, 0, ovr_afediv5g_dac_en, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0, ovr_afediv2g_adc_en, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0, ovr_afediv2g_dac_en, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0, ovr_afediv_rst_en, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_CFG1_OVR, 0, ovr_afediv_reset, 0)
	ACPHY_REG_LIST_EXECUTE(pi);
}

static void
wlc_phy_radio20695_minipmu_pupd_seq(phy_info_t *pi, uint8 pupdbit)
{
	uint8 core;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	if (pupdbit == 1) {
		PHY_INFORM(("wl%d: %s powering up 20695 minipmu", pi->sh->unit,
				__FUNCTION__));

		MOD_RADIO_REG_28NM(pi, RFP, BG_REG12, 0, ldo1p8_puok_en, 0x1);
		MOD_RADIO_REG_28NM(pi, RFP, LDO1P8_STAT, 0, ldo1p8_pu, 0x1);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, 0, bg_pu_bgcore, 0x1);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, 0, bg_pu_V2I, 0x1);

		/* 30 us delay to allow LDO to settle */
		OSL_DELAY(30);

		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, vref_select, 0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG5, core, wlpmu_en, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG5, core, wlpmu_LDO2P2_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_AFEldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_TXldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_TXldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, RX2G_REG4, core, rx2g_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, RX5G_REG5, core, rx5g_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, LDO1P65_STAT, core, ldo1p65_pu, 0x1);
		}
	} else {
		PHY_INFORM(("wl%d: %s powering down 20695 minipmu", pi->sh->unit,
				__FUNCTION__));

		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_28NM(pi, RF, LDO1P65_STAT, core, ldo1p65_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, RX5G_REG5, core, rx5g_ldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, RX2G_REG4, core, rx2g_ldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_TXldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_TXldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, core, wlpmu_AFEldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG5, core, wlpmu_LDO2P2_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RF, PMU_CFG5, core, wlpmu_en, 0x0);
		}

		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, 0, bg_pu_V2I, 0x0);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, 0, bg_pu_bgcore, 0x0);
		MOD_RADIO_REG_28NM(pi, RFP, LDO1P8_STAT, 0, ldo1p8_pu, 0x0);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
/* PLL PWR-UP */
static void wlc_phy_radio20696_pmu_pll_pwrup(phy_info_t *pi)
{
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_pu_ldo_CP, 0x1);	/* rfpll cp ldo pu */
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_pu_ldo_VCO, 0x1); /* rfpll vco ldo pu */

	/* Overwrites */
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_pu_ldo_CP, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_pu_ldo_VCO, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_bias_reset_CP, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_bias_reset_VCO, 0x1);

	/* Assert Bias reset */
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_bias_reset_CP, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_bias_reset_VCO, 0x1);
	OSL_DELAY(1000);

	/* Deasssert Bias Reset */
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_bias_reset_CP, 0x0);
	MOD_RADIO_PLLREG_20696(pi, PLL_HVLDO1, ldo_1p8_bias_reset_VCO, 0x0);

	/* Deassert Bias Reset overwrites	 */
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_bias_reset_CP, 0x0);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_ldo_1p8_bias_reset_VCO, 0x0);
}

/* Work-arounds */
static void wlc_phy_radio20696_wars(phy_info_t *pi)
{
	uint8 core;

	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, i_configRX_pd_adc_clk, 0x0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q, 0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I, 0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q, 1);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I, 1);

		WRITE_RADIO_REG_20696(pi, RXADC_CFG0, core, 0x285c);
		WRITE_RADIO_REG_20696(pi, RXADC_CFG1, core, 0x57ff);
		WRITE_RADIO_REG_20696(pi, RXADC_CFG2, core, 0x007f);
		WRITE_RADIO_REG_20696(pi, RXADC_CFG3, core, 0x33e);
		WRITE_RADIO_REG_20696(pi, AFE_CFG1_OVR2, core, 0xcc77);

		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q, 0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I, 0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q, 1);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I, 1);

		/* 7271: BUGFIX - HWJUSTY-285 TxEVM in 5GHz influenced by ovr_auxpga_i_pu */
		MOD_RADIO_REG_20696(pi, AUXPGA_OVR1, core, ovr_auxpga_i_pu, 0x1);
		MOD_RADIO_REG_20696(pi, AUXPGA_CFG1, core, auxpga_i_pu, 0x1);

		/* HWJUSTY-272 part 1 */
		MOD_RADIO_REG_20696(pi, TXDAC_REG0, core, iqdac_buf_cmsel, 1);
	}
	/* HWJUSTY-272 part 2, improve LOFT comp range */
	MOD_PHYREG(pi, Core1TxControl, loft_comp_shift, 1);
	MOD_PHYREG(pi, Core2TxControl, loft_comp_shift, 1);
	MOD_PHYREG(pi, Core3TxControl, loft_comp_shift, 1);
	MOD_PHYREG(pi, Core4TxControl, loft_comp_shift, 1);

	/* 7271: HWJUSTY-259 missing this reg write causes Tx tone to fail on all cores */
	MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG5, 3, logen_mux_pu, 1);

	/* 7271: frontLDO may be unstabe to power down on 28nm radios */
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_OVR0, ovr_LDO1P2_BG_en_1p0, 0x1);
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_OVR0, ovr_LDO1P2_cntl_en_1p0, 0x1);
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_OVR0, ovr_LDO1P2_pu_1p0, 0x1);
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_REG0, LDO1P2_BG_en_1p0, 0x1);
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_REG0, LDO1P2_cntl_en_1p0, 0x1);
	MOD_RADIO_PLLREG_20696(pi, FRONTLDO_REG0, LDO1P2_pu_1p0, 0x1);
}

static void wlc_phy_radio20696_refclk_en(phy_info_t *pi)
{
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER1, RefDoubler_pu, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_RefDoubler_pu_pfddrv, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER3, RefDoubler_pu_pfddrv, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER2, RefDoubler_pu_clkvcocal, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER2, RefDoubler_pu_clkrcal, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER2, RefDoubler_pu_clkrccal, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR2, ovr_RefDoubler_ldo_1p8_pu_ldo, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_REFDOUBLER1, RefDoubler_ldo_1p8_pu_ldo, 0x1);
}


/* Load additional perfer value after power up */
static void wlc_phy_radio20696_upd_prfd_values(phy_info_t *pi)
{
	uint8 core;
	uint i = 0;
	radio_20xx_prefregs_t *prefregs_20696_ptr = NULL;

	ASSERT(RADIOID(pi->pubpi->radioid) == BCM20696_ID);

	/* Choose the right table to use */
	switch (RADIOREV(pi->pubpi->radiorev)) {
	case 0: prefregs_20696_ptr = prefregs_20696_rev0;
		break;
	case 2: /* TODO: Update when there is a prefregs_20696_rev2 */
		prefregs_20696_ptr = prefregs_20696_rev0;
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIOREV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return;
	}

	/* Update preferred values */
	while (prefregs_20696_ptr[i].address != 0xffff) {
		phy_utils_write_radioreg(pi, prefregs_20696_ptr[i].address,
			(uint16)prefregs_20696_ptr[i].init);
		i++;
	}

	wlc_phy_radio20696_refclk_en(pi);

	/* turn ON Common LOGen pu's */
	MOD_RADIO_PLLREG_20696(pi, LOGEN_REG0, logen_vcobuf_pu, 0x1);
	MOD_RADIO_PLLREG_20696(pi, LOGEN_REG0, logen_pu, 0x1);
	MOD_RADIO_REG_20696(pi, BG_REG10, 1, bg_wlpmu_pu_cbuck_ref, 0x1);
	/* this is not working in veloce when placed here - pmu cal failed? */
	MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_pu, 0x0);

	FOREACH_CORE(pi, core) {
		/* logen */
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core, logen_lc_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core, logen_gm_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core, logen_bias_pu, 0x1);

		/* lpf bias pu */
		MOD_RADIO_REG_20696(pi, LPF_OVR1, core, ovr_lpf_bias_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LPF_REG6, core, lpf_bias_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LPF_OVR1, 0, ovr_lpf_bq_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LPF_REG6, 0, lpf_bq_pu, 0x1);

		/* new optimised 2G settings */
		/* Tx */
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG0, core, tx2g_mx_gm_offset_en, 1);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG1, core, tx2g_mx_idac_mirror_cas, 4);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG1, core, tx2g_mx_idac_mirror_cas_bb, 4);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG1, core, tx2g_mx_idac_mirror_cas_replica, 4);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG3, core, tx2g_mx_idac_cas_off, 9);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG3, core, tx2g_mx_idac_mirror_lo, 3);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG1, core, tx2g_pad_idac_cas_off, 9);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG3, core, tx2g_pad_idac_cas, 6);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_pa2g_tssi_ctrl_sel, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_pa2g_bias_bw, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_tx2g_mx_bias_reset_cas, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_tx2g_mx_bias_reset_lo, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_tx2g_mx_bias_reset_bbdc, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_pa2g_tssi_ctrl_range, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_tx2g_pad_bias_reset_gm, 1);
		MOD_RADIO_REG_20696(pi, TX2G_CFG1_OVR, core, ovr_tx2g_pad_bias_reset_cas, 1);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG1, core, tx2g_pad_idac_gm, 4);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG2, core, tx2g_mx_idac_bb, 11);
		/* Rx */
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi0_low_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi0_mid_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi0_high_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi1_low_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi1_mid_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG5, core, rx2g_wrssi1_high_pu, 1);
		MOD_RADIO_REG_20696(pi, RX2G_REG2, core, rx2g_gm_idac_nmos_ds, 0);
		MOD_RADIO_REG_20696(pi, RX2G_REG2, core, rx2g_gm_idac_pmos_ds, 0);

		/* new optimised 5G settings */
		/* Tx */
		MOD_RADIO_REG_20696(pi, TX5G_MIX_GC_REG, core, tx5g_mx_gc, 1);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG1, core, tx5g_idac_mx5g_tuning, 9);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG1, core, tx5g_idac_mx_bbdc, 26);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG2, core, tx5g_idac_mx_lodc, 15);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG1, core, tx5g_idac_pad_main, 28);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG1, core, tx5g_idac_pad_main_slope, 3);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG1, core, tx5g_idac_pad_cas_bot, 15);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG2, core, tx5g_idac_pad_tuning, 8);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG1, core, tx5g_idac_pa_cas, 35);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG2, core, tx5g_idac_pa_incap_compen, 32);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG1, core, tx5g_idac_pa_main, 52);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG3, core, tx5g_idac_pa_tuning, 11);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG1, core, tx5g_mx_de_q, 0);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, core, tx5g_pad_de_q, 0);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, core, tx5g_pad_max_cas_bias, 0);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, core, tx5g_pad_max_pmos_vbias, 0);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, core, tx5g_pa_max_cas_vbias, 0);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, core, tx5g_pa_max_pmos_vbias, 0);
		MOD_RADIO_REG_20696(pi, TX5G_CFG2_OVR, core, ovr_pa5g_tssi_ctrl_range, 1);
		MOD_RADIO_REG_20696(pi, TX5G_MISC_CFG1, core, pa5g_tssi_ctrl_range, 1);
		/* Rx */
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi0_low_pu, 1);
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi0_mid_pu, 1);
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi0_high_pu, 1);
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi1_low_pu, 1);
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi1_mid_pu, 1);
		MOD_RADIO_REG_20696(pi, RX5G_WRSSI, core, rx5g_wrssi1_high_pu, 1);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_28nm_radio_pwron_seq_phyregs(phy_info_t *pi, uint8 core)
{
	/* # power down everything */
	/* do not touch bg_pulse */
	WRITE_PHYREGCE(pi, RfctrlCoreTxPus, core, 0);
	WRITE_PHYREGCE(pi, RfctrlOverrideTxPus, core, 0x1ff);

	/* Using usleep of 100us below, so don't need these */
	WRITE_PHYREG(pi, Pllldo_resetCtrl, 0);
	WRITE_PHYREG(pi, Rfpll_resetCtrl, 0);
	WRITE_PHYREG(pi, Logen_AfeDiv_reset, 0x2000);

	/* Start with everything off: {radio, rfpll, plldlo, logen}_{pu, reset} = 0 */
	WRITE_PHYREG(pi, RfctrlCmd, 0);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0);
	WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0xd);


#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	/* Update preferred values */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		wlc_phy_radio20695_upd_prfd_values(pi);
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

	/* Make RfctrlCmd.chip_pu = 1
	   This indicates WLAN RF active
	   This is required for coex to work
	*/
	MOD_PHYREG(pi, RfctrlCmd, chip_pu, 1);
}

static radio_20xx_prefregs_t *
BCMRAMFN(wlc_phy_ac_get_20695_prfd_vals_tbl)(phy_info_t *pi)
{
	/* Choose the right table to use */
	switch (RADIOREV(pi->pubpi->radiorev)) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
		case 32:
			return prefregs_20695_rev32;

		case 33:
			return prefregs_20695_rev33;

		case 38:
			return prefregs_20695_rev38;

		case 39:
			return prefregs_20695_rev39;
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

		default:
			PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
				pi->sh->unit, __FUNCTION__,
				RADIOREV(pi->pubpi->radiorev)));
			ROMMABLE_ASSERT(FALSE);
			return NULL;
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (!defined(RADIO_BCM20696) || \
	defined(RADIO_BCM20691) || defined(RADIO_BCM20694) || defined(RADIO_BCM20695)))
static uint
wlc_phy_init_radio_prefregs_allbands(phy_info_t *pi, const radio_20xx_prefregs_t *radioregs)
{
	uint i;

	for (i = 0; radioregs[i].address != 0xffff; i++) {
		phy_utils_write_radioreg(pi, radioregs[i].address,
		                (uint16)radioregs[i].init);
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			OSL_DELAY(100);
		}
	}

	return i;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && !defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_radio20695_upd_prfd_values(phy_info_t *pi)
{
	radio_20xx_prefregs_t *prefregs_20695_ptr = NULL;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	if ((prefregs_20695_ptr = wlc_phy_ac_get_20695_prfd_vals_tbl(pi)) == NULL) {
		PHY_ERROR(("wl%d: %s: Preferred value table not known for radio_rev %d\n",
				pi->sh->unit, __FUNCTION__,
				RADIOREV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return;
	}

	/* Update preferred values */
	wlc_phy_init_radio_prefregs_allbands(pi, prefregs_20695_ptr);

	/* temporary place holder for B0 updates till prefered values are not finalyzed */
	if (RADIOMAJORREV(pi) >= 2) {
		/* War for 5G PLL not locking issue */
		MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, 0, rfpll_5g_div_buff_str, 0x0);
	}
	if (PHY_IPA(pi)) {
		MOD_RADIO_REG_28NM(pi, RF, PA2G_CFG14, 0, pad2g_tssi_ctrl, 0x0);
	}
}

static void
wlc_phy_switch_radio_acphy_20695(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	/* RCAL */
	wlc_phy_28nm_radio_rcal(pi, 1);


	/* minipmu_cal */
	wlc_phy_28nm_radio_minipmu_cal(pi);

	/* Xtal settings for better phase noise */
	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL_OVR1, 0, ovr_xtal_bias_adj, 0x1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL_OVR1, 0,
				ovr_xtal_core_strength_nmos, 0x1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL_OVR1, 0,
				ovr_xtal_core_strength_pmos, 0x1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL_OVR1, 0, ovr_xtal_sel_bias_res, 0x1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL_OVR1, 0, ovr_xt_res_bypass, 0x1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL3, 0, xtal_bias_adj, 0x4)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL2, 0, xtal_coresize_nmos, 0x10)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL2, 0, xtal_coresize_pmos, 0x10)
		MOD_RADIO_REG_28NM_ENTRY(pi, RFP, XTAL1, 0, xt_res_bypass, 0x4)
	ACPHY_REG_LIST_EXECUTE(pi);

	if (RADIOREV(pi->pubpi->radiorev) == 33) {
		MOD_RADIO_REG_28NM(pi, RFP, XTAL3, 0, xtal_sel_bias_res, 0x4);
	} else {
		MOD_RADIO_REG_28NM(pi, RFP, XTAL3, 0, xtal_sel_bias_res, 0x2);
	}
	/* Modifying PMU registers which impact XTAL settings mentioned above */
	chanspec_setup_xtal_20695(pi);

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

int8
wlc_phy_28nm_radio_minipmu_cal(phy_info_t *pi)
{
	uint8 calsuccesful = 1;
	int8 cal_status = 1;
	int16 waitcounter = 0;
	uint16 temp_code;
	uint32 save_gcichipctrl7;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	/* Enable the PMU cal clock using GCI Control 7 Register (set bit 13 and 14) */
	save_gcichipctrl7 = si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_07, 0, 0);
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_07, (0x3 << 13), (0x3 << 13));

	/* Setup the cal */
	MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, 0, ovr_pmu_bg_wlpmu_en_i, 0x1);
	MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, 0, ovr_bgpmucal_pu, 0x1);
	WRITE_RADIO_REG_28NM(pi, RFP, BG_REG10, 0, 0x37);

	/* 25 us delay to allow PMU cal to happen */
	OSL_DELAY(25);

	while (READ_RADIO_REGFLD_28NM(pi, RFP, BG_REG11, 0, bg_wlpmu_cal_done) == 0) {
		/* 10 us delay wait time */
		OSL_DELAY(10);
		waitcounter++;

		if (waitcounter > 5) {
			PHY_TRACE(("\nWarning:Mini PMU Cal Failed on Core0"));
			calsuccesful = 0;
			ROMMABLE_ASSERT(0);
			break;
		}
	}

	/* Cleanup */
	WRITE_RADIO_REG_28NM(pi, RFP, BG_REG10, 0, 0x6C);
	MOD_RADIO_REG_28NM(pi, RF, PMU_CFG1, 0, vref_select, 0x1);

	if (calsuccesful == 1) {
		PHY_TRACE(("MiniPMU cal done on core0, Cal code = %d",
			READ_RADIO_REGFLD_28NM(pi, RFP, BG_REG11, 0, bg_wlpmu_calcode)));

		temp_code = READ_RADIO_REGFLD_28NM(pi, RFP, BG_REG11, 0, bg_wlpmu_calcode);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG9, 0, bg_wlpmu_cal_mancodes, temp_code);
	} else {
		PHY_TRACE(("Cal Unsuccesful on Core0"));
		cal_status = -1;
	}

	/* Restore gcichipctrl 7 reg */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_07, 0xffffffff, save_gcichipctrl7);

	return cal_status;
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
/*  R Calibration (takes ~50us) */
static void
wlc_phy_28nm_radio_rcal(phy_info_t *pi, uint8 mode)
{
	/* Format: 20695_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* The rcalcode argument works only with mode 1 and is optional. */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP. This is what driver should */
	/* do but is not good for bringup because the OTP is not programmed yet. */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */
	uint8 rcal_valid, loop_iter, rcal_value;
	uint8 core = 0;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	if (mode == 0) {
		/* Use OTP stored rcal value */
		MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, core, ovr_otp_rcal_sel, 0x1);
	} else if (mode == 1) {
		/* Default RCal code to be used with mode 1 is 0x36 */
		rcal_value = 0x36;
		MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, core, ovr_otp_rcal_sel, 0);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG8, core, bg_rcal_trim, rcal_value);
		MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, core, ovr_bg_rcal_trim, 1);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG1, core, bg_ate_rcal_trim, rcal_value);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, core, bg_ate_rcal_trim_en, 1);
	} else if (mode == 2) {
		/* This should run only for core 0 */
		/* Run R Cal and use its output */

		MOD_RADIO_REG_20695_MULTI_2(pi, RFP, XTAL5, core, xtal_pu_wlandig, 1,
			xtal_pu_RCCAL, 1);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, core, bg_ate_rcal_trim_en, 0x0);
		/* Make connection with the external 10k resistor */
		/* Also powers up rcal (RF0_rcal_cfg_north.rcal_pu = 1) */

		MOD_RADIO_REG_20695_MULTI_2(pi, RFP, GPAIO_SEL2, core, gpaio_rcal_pu, 0,
			gpaio_pu, 1);
		MOD_RADIO_REG_28NM(pi, RFP, GPAIO_SEL0, core,
				gpaio_sel_0to15_port, 0x0);
		MOD_RADIO_REG_28NM(pi, RFP, GPAIO_SEL1, core,
				gpaio_sel_16to31_port, 0x0);
		MOD_RADIO_REG_28NM(pi, RFP, GPAIO_SEL3, core,
				gpaio_sel_32to47_port, 0x0);
		MOD_RADIO_REG_28NM(pi, RFP, GPAIO_SEL2, core,
				gpaio_rcal_pu, 1);
		MOD_RADIO_REG_28NM(pi, RFP, BG_REG3, core,
				bg_rcal_pu, 1);
		MOD_RADIO_REG_28NM(pi, RF, RCAL_CFG_NORTH, core,
				rcal_pu, 1);

		rcal_valid = 0;
		loop_iter = 0;
		while ((rcal_valid == 0) && (loop_iter <= 10)) {
			/* 1 ms delay wait time */
			OSL_DELAY(1000);
			rcal_valid = READ_RADIO_REGFLD_28NM(pi, RF, RCAL_CFG_NORTH, core,
					rcal_valid);
			loop_iter++;
		}

		if (rcal_valid == 1) {
			rcal_value = READ_RADIO_REGFLD_28NM(pi, RF, RCAL_CFG_NORTH, core,
					rcal_value);
			/* Use the output of the rcal engine */
			/* Don't use OTP value */
			MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, core, ovr_otp_rcal_sel, 0);
			MOD_RADIO_REG_28NM(pi, RFP, BG_REG8, core, bg_rcal_trim, rcal_value);
			MOD_RADIO_REG_28NM(pi, RFP, BG_OVR1, core, ovr_bg_rcal_trim, 1);


			MOD_RADIO_REG_28NM(pi, RFP, BG_REG2, core, bg_ate_rcal_trim_en, 1);
			MOD_RADIO_REG_28NM(pi, RFP, BG_REG1, core, bg_ate_rcal_trim, rcal_value);

			/* Very coarse sanity check */
			if ((rcal_value < 2) || (12 < rcal_value)) {
				PHY_ERROR(("*** ERROR: R Cal value out of range."
				" 4bit Rcal = %d.\n", rcal_value));
			}
		} else {
			PHY_ERROR(("%s RCal unsucessful. RCal valid bit is %d.\n",
				__FUNCTION__, rcal_valid));
		}

		/* Power down blocks not needed anymore */
		MOD_RADIO_REG_28NM(pi, RF, RCAL_CFG_NORTH, core,
				rcal_pu, 0);
	}
}

static void
wlc_phy_28nm_radio_rccal(phy_info_t *pi)
{
	uint8 cal, done;
	uint16 rccal_itr, n0, n1;

	/* lpf, dacbuf */
	uint8 sr[] = {0x1, 0x0};
	uint8 sc[] = {0x0, 0x1};

	/*  Q1 = 0 -> gmult_dacbuf_k = 154142
	 *  Q1 = 1 -> gmult_dacbuf_k = 77071
	 *  Q1 = 2 -> gmult_dacbuf_k = 38535
	*/
	uint32 gmult_dacbuf_k = 77071;

	/*  Q1 = 0 -> gmult_lpf_k = 149600
	 *  Q1 = 1 -> gmult_lpf_k = 74800
	 *  Q1 = 2 -> gmult_lpf_k = 37400
	*/
	uint32 gmult_lpf_k    = 74800;
	uint32 gmult_dacbuf;

	phy_ac_radio_data_t *data = pi->u.pi_acphy->radioi->data;
	uint32 dn;

	pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
	data->rccal_gmult_rc     = 4260;

	if (ISSIM_ENAB(pi->sh->sih)) {
		return;
	}

	/* Powerup rccal driver & set divider radio (rccal needs to run at 20mhz) */
	MOD_RADIO_REG_28NM(pi, RFP, XTAL5, 0, xtal_pu_RCCAL, 1);
	MOD_RADIO_REG_28NM(pi, RFP, XTAL5, 0, xtal_pu_wlandig, 1);

	/* Calibrate lpf, dacbuf cal 0 = lpf 1 = dacbuf */
	for (cal = 0; cal < 2; cal++) {
		/* Setup */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG0, 0, rccal_sr, sr[cal]);
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG0, 0, rccal_sc, sc[cal]);
		/* Q!=2 -> 16 counts */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG1, 0, rccal_Q1, 0x1);


		/* Toggle RCCAL power */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG1, 0, rccal_START, 1);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		done = 0;
		for (rccal_itr = 0;
			(rccal_itr < MAX_2069_RCCAL_WAITLOOPS) && (done == 0);
			rccal_itr++) {
			OSL_DELAY(100);
			done = READ_RADIO_REGFLD_28NM(pi, RFP, RCCAL_CFG3, 0, rccal_DONE);
		}

		/* Stop RCCAL */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG1, 0, rccal_START, 0);

		/* Make sure that RC Cal ran to completion */
		ASSERT(done);

		n0 = READ_RADIO_REGFLD_28NM(pi, RFP, RCCAL_CFG4, 0, rccal_N0);
		n1 = READ_RADIO_REGFLD_28NM(pi, RFP, RCCAL_CFG5, 0, rccal_N1);
		dn = n1 - n0; /* set dn [expr {$N1 - $N0}] */

		if (cal == 0) {
			/* lpf  values */
			data->rccal_gmult = (gmult_lpf_k * dn) / (pi->xtalfreq >> 12);
			data->rccal_gmult_rc = data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
			            __FUNCTION__, data->rccal_gmult));
		} else if (cal == 1) {
			/* dacbuf  */
			gmult_dacbuf  = (gmult_dacbuf_k * dn) / (pi->xtalfreq >> 12);
			pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = (1 << 24) / (gmult_dacbuf);
			PHY_INFORM(("wl%d: %s rccal_dacbuf_cmult = %d\n", pi->sh->unit,
				__FUNCTION__, pi->u.pi_acphy->radioi->rccal_dacbuf_cmult));
		}
		/* Turn off rccal */
		MOD_RADIO_REG_28NM(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 0);

	}
	/* Powerdown rccal driver */
	MOD_RADIO_REG_28NM(pi, RFP, XTAL5, 0, xtal_pu_RCCAL, 0);

	/* nominal values when rc cal failes  */
	if (done == 0) {
		pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
		data->rccal_gmult_rc     = 4260;
	}

	/* Programming lpf gmult and tia gmult */
	MOD_RADIO_REG_28NM(pi, RF, TIA_GMULT_BQ1, 0, tia_gmult_bq1, data->rccal_gmult_rc);
	MOD_RADIO_REG_28NM(pi, RF, TIA_GMULT_BQ2, 0, tia_gmult_bq2, data->rccal_gmult_rc);

}

static void
wlc_phy_28nm_radio_pll_logen_pupd_seq(phy_info_t *pi, pll_logen_block_20695_t block, uint8 pupdbit,
		uint8 core)
{
	if (pupdbit == 1) {
		if (block == PLL2G) {
			PHY_INFORM(("wl%d: %s powering up 20695 core%d 2G PLL\n",
					pi->sh->unit, __FUNCTION__, core));

			/* Set rfpll_vcocal_rst_n to 0 using override mode */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_VCOCAL1, core, rfpll_vcocal_rst_n, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_VCOCAL_OVR1, core,
					ovr_rfpll_vcocal_rst_n, 0x1);

			/* Power up pfd LDO 10 us before synth PU */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core,
					ovr_rfpll_pfd_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO2, core,
					rfpll_rfvco_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core,
					ovr_rfpll_vco_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vco_pu, 0x1);

			/* Wait for 10 us before powering up Synth */
			OSL_DELAY(10);

			/* reset -> high */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO2, core, rfpll_rfvco_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vco_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CP1, core, rfpll_cp_bias_reset, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_cp_bias_reset, 0x1);

			/* Power up PLL */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_synth_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_synth_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_buf_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vco_buf_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_mmd_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_mmd_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO4, core, rfpll_vcobuf2g_sel2g, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vcobuf2g_sel2g, 0x1);

			/* Wait for 10 us before pulling reset low */
			OSL_DELAY(10);

			/* reset -> low */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO2, core,
					rfpll_rfvco_ldo_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO2, core, rfpll_rfvco_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CP1, core, rfpll_cp_bias_reset, 0x0);
		} else if (block == PLL5G) {
			PHY_INFORM(("wl%d: %s powering up 20695 core%d 5G PLL\n",
					pi->sh->unit, __FUNCTION__, core));

			/* Set rfpll_vcocal_rst_n to 0 using override mode */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_VCOCAL1, core,
					rfpll_5g_vcocal_rst_n, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_VCOCAL_OVR1, core,
					ovr_rfpll_5g_vcocal_rst_n, 0x1);

			/* Power up pfd LDO 10 us before synth PU */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_PFDLDO1, core,
					rfpll_5g_pfd_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_pfd_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_vco_ldo_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_PFDLDO1, core, rfpll_5g_pfd_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_vco_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_vco_pu, 0x1);

			/* Wait for 10 us before powering up Synth */
			OSL_DELAY(10);

			/* reset -> high */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_vco_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CP1, core, rfpll_5g_cp_bias_rst, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_cp_bias_reset, 0x1);

			/* Power up PLL */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_synth_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_synth_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_mmd_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_mmd_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, core, rfpll_5g_div_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_div_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_VthM_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_rfvco_VthM_pu, 0x1);

			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CP1, core, rfpll_5g_cp_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_LF6, core, rfpll_5g_lf_cm_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_pfd_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, core, rfpll_5g_frct_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_pfd_ldo_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_PFDLDO1, core, rfpll_5g_pfd_ldo_pu, 0x1);

			/* Wait for 10 us before pulling reset low */
			OSL_DELAY(10);

			/* reset -> low */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_PFDLDO1, core,
					rfpll_5g_pfd_ldo_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_ldo_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_bias_rst, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CP1, core, rfpll_5g_cp_bias_rst, 0x0);
		} else if (block == LOGEN5G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 5G LOGEN\n",
					pi->sh->unit, __FUNCTION__, core));
			/* Power up LOGEN */
			MOD_RADIO_REG_28NM(pi, RF, LOGEN5G_REG4, core, logen5g_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, LOGEN_OVR1, core, ovr_logen5g_pu, 0x1);
		} else if (block == LOGEN5GTO2G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 5G LOGEN to 2G\n",
					pi->sh->unit, __FUNCTION__, core));
			/* Power up LOGEN */
			MOD_RADIO_REG_28NM(pi, RF, LO5GTO2G_CFG3, core, lo5g_to_2g_pu, 0x1);
			MOD_RADIO_REG_28NM(pi, RF, LOGEN_OVR1, core, ovr_lo5g_to_2g_pu, 0x1);
		}
	} else {
		if (block == PLL2G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 2G PLL\n",
					pi->sh->unit, __FUNCTION__, core));

			/* Power down synth */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO4, core, rfpll_vcobuf2g_sel2g, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_mmd_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_buf_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_synth_pu, 0x0);

			/* Power down LDO */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_pu, 0x0);
		} else if (block == PLL5G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 5G PLL\n",
					pi->sh->unit, __FUNCTION__, core));

			/* Power down synth */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO2, core,
					rfpll_5g_rfvco_VthM_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, core, rfpll_5g_div_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_mmd_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_synth_pu, 0x0);

			/* Power down LDO */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_PFDLDO1, core, rfpll_5g_pfd_ldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_PFDLDO1, core, rfpll_pfd_ldo_pu, 0x0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG1, core, rfpll_5g_vco_pu, 0x0);
		} else if (block == LOGEN5G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 5G LOGEN\n",
					pi->sh->unit, __FUNCTION__, core));
			/* Power down LOGEN */
			MOD_RADIO_REG_28NM(pi, RF, LOGEN5G_REG4, core, logen5g_pu, 0x0);
		} else if (block == LOGEN5GTO2G) {
			PHY_INFORM(("wl%d: %s powering down 20695 core%d 5G LOGEN to 2G\n",
					pi->sh->unit, __FUNCTION__, core));
			/* Power down LOGEN */
			MOD_RADIO_REG_28NM(pi, RF, LO5GTO2G_CFG3, core, lo5g_to_2g_pu, 0x0);
		}
	}
}

static void
wlc_phy_chanspec_radio20695_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset, uint8 core)
{
	const chan_info_radio20695_rffe_t *chan_info;

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID));

	if (wlc_phy_chan2freq_20695(pi, ch, &chan_info) < 0) {
		return;
	}

	/* logen_reset needs to be toggled whenever bandsel bit if changed */
	/* On a bw change, phy_reset is issued which causes currentBand getting reset to 0 */
	/* So, issue this on both band & bw change */
	if (toggle_logen_reset == 1) {
		wlc_phy_logen_reset(pi, core);
	}

	/* pll tuning */
	pi->u.pi_acphy->radioi->data->pll_sel =
		wlc_phy_radio20695_pll_tune(pi, chan_info->freq, core);

	/* rffe tuning */
	wlc_phy_radio20695_rffe_tune(pi, chan_info, core);

	/* Do VCO cal based on PLL */
	wlc_phy_28nm_radio_vcocal(pi, VCO_CAL_MODE_20695,
			VCO_CAL_COUPLING_MODE_20695);

	if (chan_info->freq > 4000) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_adc_en, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG5, 0, afediv2g_dac_en, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFEDIV2G_REG4, 0, afediv2g_pu, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TX5G_CFG1_OVR, 0,
					ovr_pad5g_idac_gm, 1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TX5G_CFG1_OVR, 0,
					ovr_pad5g_idac_pmos, 1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXMIX2G_CFG4, 0,
					sel_lpf_CM_bias_5g, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
void
wlc_phy_chanspec_radio20696_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset)
{
	const chan_info_radio20696_rffe_t *chan_info;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20696_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (wlc_phy_chan2freq_20696(pi, ch, &chan_info) < 0)
		return;

	/* logen_reset needs to be toggled whenever bandsel bit if changed */
	/* On a bw change, phy_reset is issued which causes currentBand getting reset to 0 */
	/* So, issue this on both band & bw change */
	if (toggle_logen_reset == 1) {
		wlc_phy_logen_reset(pi, 0);
	}

	/* rffe tuning */
	wlc_phy_radio20696_rffe_tune(pi, chan_info);

	/* band related settings */
	wlc_phy_radio20696_upd_band_related_reg(pi);

	/* pll tuning */
	wlc_phy_radio20696_pll_tune(pi, chan_info->freq);

	/* Do a VCO cal */
	wlc_phy_20696_radio_vcocal(pi, VCO_CAL_MODE_20696, VCO_CAL_COUPLING_MODE_20696);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_radio20695_pll_logen_pu_config(phy_info_t *pi, uint32 lo_freq, uint8 core)
{
	/* Based on fc and USE_5G_PLL_FOR_2G, decide the required PLL to be on */
	if (lo_freq > 4000) {
		/* Aband
		Normal mode: 5G PLL is used for 5G. There is no other option
		*/
		MOD_RADIO_REG_28NM(pi, RF, PLL_CRISSCROSS_5GAS2G, core,
				internal_direct_rfpll_5g_as_2g, 0);
		MOD_RADIO_REG_28NM(pi, RFP, PLL_CRISSCROSS_5GAS2G, core,
				internal_direct_rfpll_5g_as_2g, 0);
		wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5GTO2G, 0, 0);
		wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL2G, 0, 0);
		wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL5G, 1, 0);
		wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5G, 1, 0);

		MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, core, rfpll_5g_div_2G_buf_pu, 0);
		MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core, ovr_rfpll_5g_div_2G_buf_pu, 0x1);
	} else {
		/* Gband */
		if (pi->u.pi_acphy->radioi->data->use_5g_pll_for_2g == 0) {
			/* Normal mode:  2G PLL is used for 2G
			NO Logen.
			*/
			MOD_RADIO_REG_28NM(pi, RF, PLL_CRISSCROSS_5GAS2G, core,
					internal_direct_rfpll_5g_as_2g, 0);

			MOD_RADIO_REG_28NM(pi, RFP, PLL_CRISSCROSS_5GAS2G, core,
					internal_direct_rfpll_5g_as_2g, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5GTO2G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL5G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL2G, 1, 0);
		} else {
			/* Special mode: 5G PLL is used for 2G
			5G PLL output gets divided by 2 and then goes to 2G LOgen.
			*/
			lo_freq *= 2;
			MOD_RADIO_REG_28NM(pi, RF, PLL_CRISSCROSS_5GAS2G, core,
					internal_direct_rfpll_5g_as_2g, 1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CRISSCROSS_5GAS2G, core,
					internal_direct_rfpll_5g_as_2g, 1);

			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL2G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL5G, 1, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5GTO2G, 1, 0);

			/* Need these additional PUs if using 5G PLL for 2G */
			/* Turn on 2G VCO LDO and VCO buf */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_pu, 1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vco_pu, 1);

			MOD_RADIO_REG_28NM(pi, RFP, PLL_CFG1, core, rfpll_vco_buf_pu, 1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vco_buf_pu, 1);

			/* Use 5G PLL output for LO */
			MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO4, core, rfpll_vcobuf2g_sel2g, 0);
			MOD_RADIO_REG_28NM(pi, RFP, PLL_OVR1, core, ovr_rfpll_vcobuf2g_sel2g, 1);

			/* Turn on 5G PLL div2 output for 2G operation */
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_CFG6, core, rfpll_5g_div_2G_buf_pu, 1);
			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_OVR1, core,
					ovr_rfpll_5g_div_2G_buf_pu, 1);

			MOD_RADIO_REG_28NM(pi, RFP, PLL5G_RFVCO5, core,
					rfpll_5g_rfvco_VthM_oa_Rn, 0x6);
		}
	}
}

static radio_pll_sel_t
wlc_phy_radio20695_pll_tune(phy_info_t *pi, uint32 chan_freq, uint8 core)
{
	uint32 lo_freq = chan_freq;
	radio_pll_sel_t pll_sel;
	pll_config_20695_tbl_t *pll;

	/* Based on fc and USE_5G_PLL_FOR_2G, decide the required PLL to be on */
	if (lo_freq > 4000) {
		/* Aband
		Normal mode: 5G PLL is used for 5G. There is no other option
		*/
		pll_sel = PLL_5G;
	} else {
		/* Gband */
		if (pi->u.pi_acphy->radioi->data->use_5g_pll_for_2g == 0) {
			/* Normal mode:  2G PLL is used for 2G
			NO Logen.
			*/
			pll_sel = PLL_2G;
		} else {
			/* Special mode: 5G PLL is used for 2G
			5G PLL output gets divided by 2 and then goes to 2G LOgen.
			*/
			lo_freq *= 2;
			pll_sel = PLL_5G;
		}
	}

	/* Get pointer to structure */
	pll = wlc_phy_ac_get_20695_pll_config(pi);

	if (pll->use_doubler) {
		MOD_RADIO_REG_28NM(pi, RFP, XTAL7, core, xtal_doubler_pu, 1);
	} else {
		MOD_RADIO_REG_28NM(pi, RFP, XTAL7, core, xtal_doubler_pu, 0);
	}

	/* Calculate PLL config values */
	phy_ac_radio20695_pll_config_ch_dep_calc(pi, lo_freq, pll);

	/* Write computed values to PLL registers */
	phy_ac_radio20695_write_pll_config(pi, pll_sel, pll);

	return pll_sel;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_20694_logen_mode(phy_info_t *pi, uint8 core)
{
	pll_config_20694_tbl_t *pll;
	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	/* Get pointer to structure */
	pll = wlc_phy_ac_get_20694_pll_config(pi);

	if (sicoreunit == DUALMAC_MAIN) {
		if (pll->logen_mode == 1) {
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG3, core, logen2g_div3ovr2, 1);
		} else {
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG3, core, logen2g_div3ovr2, 0);
		}
	} else if (core == 0) {
		if (pll->logen_mode == 1) {
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG3, 0, logen2g_div3ovr2, 1);
		} else {
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG3, 0, logen2g_div3ovr2, 0);
		}
	}
}
static void
wlc_phy_radio20694_pll_tune(phy_info_t *pi, uint32 chan_freq, uint8 core)
{
	uint32 lo_freq = chan_freq;
	uint8 vco_select;
	uint32 icp_fx;
	uint32 loop_band;
	uint8 ac_mode;

	pll_config_20694_tbl_t *pll;

	/* Get pointer to structure */
	pll = wlc_phy_ac_get_20694_pll_config(pi);

	if (core == 0) {
		if (pll->use_doubler == 1) {
			MOD_RADIO_REG_20694(pi, RFP, XTAL7, core, xtal_doubler_pu, 1);
		} else if (pll->use_doubler == 0) {
			MOD_RADIO_REG_20694(pi, RFP, XTAL7, core, xtal_doubler_pu, 0);
		} else if (pll->use_doubler == 2) {
			/* dual edge is added in B0,
			 * we double frequency in PFD instead of doubler
			 */
			/* disable XTAL doubler manually */
			MOD_RADIO_REG_20694(pi, RFP, XTAL7, core, xtal_doubler_pu, 0);
			/* PFD Risng and Falling Edge */
			MOD_RADIO_REG_20694(pi, RFP, PLL_CFG3, core, rfpll_spare1, 3);
		}
	}

	if (core == 0) {
		/* **************** */
		/* Here we can specify band/channel dependent specific pll_config parameters */
		/* Calculate PLL config values */
		/* **************** */
		vco_select = 0;
		/* if 0, then select_VCO=0, enable_coupled_VCO=0;  */
		/* elseif 1, then select_VCO=1, enable_coupled_VCO=0; */
		/* elseif 2, then select_VCO=0, enable_coupled_VCO=1; */
		icp_fx = 131072;	/*  default = 2 mA; 0.16.16 format -> (2.0*pow(2,16)) */
		loop_band = 340;	/* MHz, default */
		ac_mode = 1;
		pll->logen_mode = 0;
		if (lo_freq < 4000) {
			ac_mode = 0;
		}
		/* Using VCO1 for the following frac-N channels for spur control */
		if ((lo_freq == 5270) || (lo_freq == 5500) ||
			(lo_freq == 5220) || (lo_freq == 5720)) {
			vco_select =  1;
		}

		phy_ac_radio20694_pll_config_ch_dep_calc(pi, lo_freq, vco_select, icp_fx,
			loop_band, ac_mode, pll);

		/* Write computed values to PLL registers */
		phy_ac_radio20694_write_pll_config(pi, pll);
	}

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_radio20695_rffe_tune(phy_info_t *pi, const chan_info_radio20695_rffe_t *chan_info_rffe,
	uint8 core)
{
	if (chan_info_rffe->freq > 4000) {
		/* 5G front end */
		MOD_RADIO_REG_28NM(pi, RF, LOGEN5G_REG5, core, logen5g_tune,
				chan_info_rffe->lo_gen);
		MOD_RADIO_REG_28NM(pi, RF, RX5G_REG1, core, rx5g_lna_tune,
				chan_info_rffe->rx_lna);
		MOD_RADIO_REG_28NM(pi, RF, MX5G_CFG5, core, mx5g_tune,
				chan_info_rffe->tx_mix);
		MOD_RADIO_REG_28NM(pi, RF, PA5G_CFG4, core, pad5g_tune,
				chan_info_rffe->tx_pad);
	} else {
		/* 2G front end */
		MOD_RADIO_REG_28NM(pi, RFP, PLL_RFVCO4, core, rfpll_vcobuf2g_tune,
				chan_info_rffe->vco_buf);
		MOD_RADIO_REG_28NM(pi, RF, LO5GTO2G_CFG3, core, lo5g_to_2g_tune,
				chan_info_rffe->lo_gen);
		MOD_RADIO_REG_28NM(pi, RF, LNA2G_REG1, core, lna2g_lna1_freq_tune,
				chan_info_rffe->rx_lna);
		MOD_RADIO_REG_28NM(pi, RF, TXMIX2G_CFG5, core, mx2g_tune,
				chan_info_rffe->tx_mix);
		MOD_RADIO_REG_28NM(pi, RF, PAD2G_CFG2, core, pad2g_tune,
				chan_info_rffe->tx_pad);
	}
}

static void
phy_ac_radio20695_pll_config_ch_dep_calc(phy_info_t *pi, uint32 lo_freq,
		pll_config_20695_tbl_t *pll)
{
	uint32 pfd_ref_freq_fx;
	uint8 nf;
	uint64 temp_64;
	uint32 temp_32;
	uint32 vco_freq;
	uint32 divide_ratio_fx;
	uint32 ndiv_fx;
	uint32 loop_band;
	uint32 rfpll_vcocal_XtalCount_raw_fx;
	uint8 band_idx;
	uint32 rfpll_vcocal_TargetCountCenter = 0;
	uint32 rfpll_frct_wild_base_dec_fx;
	uint32 wn_fx;
	uint32 loop_band_square_fx;
	uint32 c1_passive_lf_fx;
	uint32 c1_cap_multiplier_mode_fx;
	uint32 i_off_fx;
	uint64 r1_passive_lf_fx;
	uint64 rf_fx;
	uint64 rs_fx;
	uint32 rfpll_lf_lf_r2_ref_fx;
	uint32 rfpll_lf_lf_r2_dec_fx = 0;
	uint64 rfpll_lf_lf_rs_cm_ref_fx;
	uint32 rfpll_lf_lf_rs_cm_dec_fx;
	uint64 rfpll_lf_lf_rf_cm_ref_fx;
	uint32 rfpll_lf_lf_rf_cm_dec_fx;
	uint32 rfpll_lf_lf_c1_ref_fx;
	uint32 rfpll_lf_lf_c1_dec_fx;
	uint32 rfpll_lf_lf_c2_dec_fx;
	uint32 rfpll_lf_lf_c2_ref_fx;
	uint32 rfpll_lf_lf_c3_ref_fx;
	uint32 rfpll_lf_lf_c3_dec_fx;
	uint32 rfpll_cp_ioff_ref_fx;
	uint32 rfpll_cp_ioff_dec_fx;
	int64 temp_sign;
	uint16 rfpll_vcocal_XtalCount_dec;
	uint16 rfpll_vcocal_DeltaPllVal_dec;
	uint16 rfpll_vcocal_ErrorThres = 0;
	uint16 rfpll_vcocal_InitCapA = 0;
	uint16 rfpll_vcocal_InitCapB = 0;
	uint16 rfpll_vcocal_NormCountLeft;
	uint16 rfpll_vcocal_NormCountRight;
	uint16 rfpll_vcocal_TargetCountCenter_dec;
	uint16 rfpll_vcocal_MidCodeSel_dec;
	uint16 rfpll_lf_lf_r2_ideal = 0;
	uint16 rfpll_lf_lf_rs_cm_ideal = 0;
	uint16 rfpll_lf_lf_rf_cm_ideal = 0;
	uint16 rfpll_lf_lf_c1_ideal = 0;
	uint16 rfpll_lf_lf_c2_ideal = 0;
	uint16 rfpll_lf_lf_c3_ideal = 0;

	/* Band dependent constants array */
	const uint32 VCO_cal_cap_bits[] = {VCO_CAL_CAP_BITS_5G,
		VCO_CAL_CAP_BITS_2G};
	const uint32 kvco_code_fx[] = {KVCO_CODE_FX_5G,
		KVCO_CODE_FX_2G};
	const uint32 temp_code_fx[] = {TEMP_CODE_FX_5G,
		TEMP_CODE_FX_2G};
	const uint32 icp_fx[] = {ICP_FX_5G,
		ICP_FX_2G};
	const uint64 c1_passive_lf_const_fx[] = {C1_PASSIVE_LF_CONST_FX_5G,
		C1_PASSIVE_LF_CONST_FX_2G};
	const uint32 r1_passive_lf_const_fx[] = {R1_PASSIVE_LF_CONST_FX_5G,
		R1_PASSIVE_LF_CONST_FX_2G};
	const uint32 rfpll_lf_lf_r2_ref_const_fx[] = {RFPLL_LF_LF_R2_REF_CONST_FX_5G,
		RFPLL_LF_LF_R2_REF_CONST_FX_2G};
	const uint32 rfpll_cp_kpd_scale_dec_fx[] = {RFPLL_CP_KPD_SCALE_DEC_FX_5G,
		RFPLL_CP_KPD_SCALE_DEC_FX_2G};
	const uint32 rfpll_vcocal_XtalCount_fx[] = {1, 0};
	const uint32 ndiv_mult[] = {2, 1};
	const uint32 loop_band_width[] = {LOOP_BW_5G, LOOP_BW_2G};
	uint32 div_ratio_den[2] = {0};

	/* ------------------------------------------------------------------- */
	/* PFD Reference Frequency */
	/* <0.8.24> * <0.32.0> --> <0.8.24> */
	pfd_ref_freq_fx = pll->xtal_fx << pll->use_doubler;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* VCO Frequency */
	/* UP_CONVERSION_RATIO = 2 */
	/* <0.32.0> * <0.32.0> --> <0.32.0> */
	vco_freq = lo_freq << 1;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Band dependent constant selection */
	/* <0.8.24> * <0.32.0> --> <0.8.24> */
	div_ratio_den[0] = pfd_ref_freq_fx << 1;
	div_ratio_den[1] = pfd_ref_freq_fx;
	band_idx = (lo_freq > 4000) ? 0 : 1;

	if (pll->loop_bw == 0) {
		/* User did not specify a loop bandwidth, use default */
		loop_band = loop_band_width[band_idx];
	} else {
		loop_band = pll->loop_bw;
	}
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* VCO Frequency dependent calculations */
	/* <0.32.0> / <0.8.24> --> <0.(32 - nf).nf> */
	nf = fp_div_64(vco_freq, div_ratio_den[band_idx], NF0, NF24, &divide_ratio_fx);
	/* floor(<0.(32-nf).nf>, (nf-20)) -> 0.12.20 */
	divide_ratio_fx = fp_floor_32(divide_ratio_fx, (nf - NF20));

	/* <0.12.20> * <0.32.0> --> <0.16.16> */
	ndiv_fx = (uint32)fp_mult_64(divide_ratio_fx, ndiv_mult[band_idx], NF20, NF0, NF16);
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Defines the duration of time that vcocal should wait after it applied a
	   new calibration bit to to vco before it starts to measure vco frequency
	   to calculate the next bit. This time is required by vco to settle.
	*/
	/* <0.8.24> * <0.32.0> --> <0.8.16> */
	rfpll_vcocal_XtalCount_raw_fx = fp_round_32(pll->xtal_fx <<
			rfpll_vcocal_XtalCount_fx[band_idx], (NF24 - NF16));
	/* ceil(<0.8.16>, 16) -> <0.8.0> */
	rfpll_vcocal_XtalCount_dec = (uint16)fp_ceil_64(rfpll_vcocal_XtalCount_raw_fx,
			NF16);
	PLL_CONFIG_20695_VAL_ENTRY(pll, XTALCNT_2G, rfpll_vcocal_XtalCount_dec);
	PLL_CONFIG_20695_VAL_ENTRY(pll, XTALCNT_5G, rfpll_vcocal_XtalCount_dec);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_XTALCOUNT,
			rfpll_vcocal_XtalCount_dec);

	/* <0.32.0> / <0.16.16> --> <0.(32 - nf).nf> */
	nf = fp_div_64(rfpll_vcocal_XtalCount_dec, rfpll_vcocal_XtalCount_raw_fx,
			NF0, NF16, &temp_32);
	/* round(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
	temp_32 = fp_round_32(temp_32, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_32	= temp_32 - FX_ONE;
	/* round(<0.16.16> * <0.32.0> ,16) -> <0.32.0> */
	rfpll_vcocal_DeltaPllVal_dec = (uint16)fp_round_64((temp_32 * (1 << 13)), 16);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_DELTAPLLVAL,
			rfpll_vcocal_DeltaPllVal_dec);

	/* Frequency range dependent constants */
	if (lo_freq >= 2000 && lo_freq <= 3000) {
		if (pll->logen_mode == 0) {
			rfpll_vcocal_InitCapA = 1100;
			rfpll_vcocal_InitCapB = 1356;
		} else {
			rfpll_vcocal_InitCapA = 2500;
			rfpll_vcocal_InitCapB = 3012;
		}
		rfpll_vcocal_ErrorThres  = 5;
		rfpll_vcocal_TargetCountCenter  = 2442;
	} else if  (lo_freq < 5250) {
		rfpll_vcocal_InitCapA  = 2048;
		rfpll_vcocal_InitCapB  = 2304;
		rfpll_vcocal_ErrorThres = 8;
		rfpll_vcocal_TargetCountCenter = 5070;
	} else if (lo_freq >= 5250 && lo_freq < 5500) {
		rfpll_vcocal_InitCapA = 1408;
		rfpll_vcocal_InitCapB = 1664;
		rfpll_vcocal_ErrorThres = 9;
		rfpll_vcocal_TargetCountCenter = 5370;
	} else if (lo_freq >= 5500) {
		rfpll_vcocal_InitCapA = 768;
		rfpll_vcocal_InitCapB = 1024;
		rfpll_vcocal_ErrorThres = 14;
		rfpll_vcocal_TargetCountCenter = 5700;
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_ERRORTHRES, rfpll_vcocal_ErrorThres);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_INITCAPA, rfpll_vcocal_InitCapA);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_INITCAPB, rfpll_vcocal_InitCapB);

	if (lo_freq >= 2000 && lo_freq <= 3000) {
		rfpll_vcocal_NormCountLeft = 985;
		rfpll_vcocal_NormCountRight = 39;
	} else {
		rfpll_vcocal_NormCountLeft = 974;
		rfpll_vcocal_NormCountRight = 50;
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_NORMCOUNTLEFT,
			rfpll_vcocal_NormCountLeft);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_NORMCOUNTRIGHT,
			rfpll_vcocal_NormCountRight);

	/* <0.32.0> - <0.32.0> -> <0.32.0> */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_ROUNDLSB, (12 - VCO_cal_cap_bits[band_idx]));

	/* <0.32.0> * <0.32.0> -> <0.32.0> */
	temp_64 = rfpll_vcocal_TargetCountCenter * rfpll_vcocal_DeltaPllVal_dec;
	/* <0.32.0> + round(<0.32.0> * <0.16.16>, 16) -> <0.32.0> */
	rfpll_vcocal_TargetCountCenter_dec = (uint16)(rfpll_vcocal_TargetCountCenter +
			fp_round_64((temp_64 * FIX_2POW_MIN13), NF16));
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_TARGETCOUNTCENTER,
			rfpll_vcocal_TargetCountCenter_dec);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_PLL_VAL, (uint16)lo_freq);
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Other VCOCAL register (not channel/XTAL dependent) */
	if (RFPLL_VCOCAL_ENABLECOUPLING_DEC == 0) {
		if ((VCO_CAL_AUX_CAP_BITS < 6) || (VCO_CAL_AUX_CAP_BITS > 9)) {
			PHY_ERROR(("wl%d: %s: Unsupported value VCO_cal_aux_cap_bits\n",
				pi->sh->unit, __FUNCTION__));
			ASSERT(0);
			return;
		}

		rfpll_vcocal_MidCodeSel_dec = VCO_CAL_AUX_CAP_BITS - 6;
	} else {
		if ((VCO_CAL_AUX_CAP_BITS < 4) || (VCO_CAL_AUX_CAP_BITS > 7)) {
			PHY_ERROR(("wl%d: %s: Unsupported value VCO_cal_aux_cap_bits\n",
				pi->sh->unit, __FUNCTION__));
			ASSERT(0);
			return;
		}

		rfpll_vcocal_MidCodeSel_dec = VCO_CAL_AUX_CAP_BITS - 4;
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_MIDCODESEL, rfpll_vcocal_MidCodeSel_dec);
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* VCO register */
	/* Fract-N (Sigma-Delta) register */
	rfpll_frct_wild_base_dec_fx = divide_ratio_fx;
	temp_32 = rfpll_frct_wild_base_dec_fx >> 16;
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_FRCT_WILD_BASE_HIGH,
			(uint16)(temp_32 & 0xFFFF));
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_FRCT_WILD_BASE_LOW,
			(uint16)(rfpll_frct_wild_base_dec_fx & 0xFFFF));

	/* <0.24.8> * <0.32.0> -> <0.26.6> */
	wn_fx = (uint32)fp_mult_64(WN_CONST_FX, loop_band, NF8, NF0, NF6);
	/* <0.32.0> * <0.32.0> -> <0.32.0> */
	loop_band_square_fx = loop_band * loop_band;
	/* <0.16.16> * <0.32.0> -> <0.26.6> */
	temp_32 = (uint32)fp_mult_64(ndiv_fx, loop_band_square_fx, NF16, NF0, NF6);
	/* <0.16.16> / <0.26.6> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_const_fx[band_idx], temp_32, NF16, NF6, &c1_passive_lf_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	c1_passive_lf_fx = fp_round_32(c1_passive_lf_fx, (nf - NF16));

	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, CAP_MULTIPLIER_RATIO_PLUS_ONE, NF16, NF0,
			&c1_cap_multiplier_mode_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	c1_cap_multiplier_mode_fx = fp_round_32(c1_cap_multiplier_mode_fx, (nf - NF16));

	/* <0.16.16> * <0.8.24> -> <0.16.16> */
	temp_32 = (uint32)fp_mult_64(T_OFF_FX, pfd_ref_freq_fx, NF16, NF24, NF16);
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	i_off_fx = (uint32)fp_mult_64(temp_32, icp_fx[band_idx], NF16, NF16, NF16);

	/* <0.6.26> * <0.0.32> -> <0.16.16> */
	temp_64 = fp_mult_64(wn_fx, r1_passive_lf_const_fx[band_idx], NF6, NF32, NF16);
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(ndiv_fx, 1000, NF16, NF0, &temp_32);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	temp_32 = fp_round_32(temp_32, (nf - NF16));
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	r1_passive_lf_fx = fp_mult_64(temp_64, temp_32, NF16, NF16, NF16);
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	rf_fx = fp_mult_64(r1_passive_lf_fx, RF_CONST_FX, NF16, NF16, NF16);
	/* <0.32.0> * <0.16.16> -> <0.16.16> */
	rs_fx = CAP_MULTIPLIER_RATIO * rf_fx;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Register Definitions (lower) */
	/* rfpll_lf_lf_r2 */
	/* <0.6.26> * <0.16.16> -> <0.24.8> */
	temp_64 = fp_mult_64(wn_fx, ndiv_fx, NF6, NF16, NF8);
	/* <0.24.8> * <0.0.32> -> <0.16.16> */
	temp_64 = fp_mult_64(temp_64, rfpll_lf_lf_r2_ref_const_fx[band_idx], NF8, NF32, NF16);
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(temp_64, 1000, NF16, NF0, &rfpll_lf_lf_r2_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_lf_lf_r2_ref_fx = fp_round_32(rfpll_lf_lf_r2_ref_fx, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_r2_ref_fx - (int64)CONST_800_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_400_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_r2_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_r2_ideal = LIMIT((int32)rfpll_lf_lf_r2_dec_fx, 0, 127);
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_R2, rfpll_lf_lf_r2_ideal);

	/* rfpll_lf_lf_r3 */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_R3, rfpll_lf_lf_r2_ideal);

	/* rfpll_lf_lf_rs_cm */
	rfpll_lf_lf_rs_cm_ref_fx = rs_fx;
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_rs_cm_ref_fx - (int64)CONST_3200_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_1600_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_rs_cm_dec_fx =  fp_round_32(temp_32, nf);
		rfpll_lf_lf_rs_cm_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_rs_cm_dec_fx,
				0, 255));
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_RS_CM, rfpll_lf_lf_rs_cm_ideal);

	/* rfpll_lf_lf_rf_cm */
	rfpll_lf_lf_rf_cm_ref_fx = rf_fx;
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_rf_cm_ref_fx - (int64)CONST_800_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_400_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_rf_cm_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_rf_cm_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_rf_cm_dec_fx,
				0, 127));
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_RF_CM, rfpll_lf_lf_rf_cm_ideal);

	/* rfpll_lf_lf_c1 */
	rfpll_lf_lf_c1_ref_fx  = c1_cap_multiplier_mode_fx;
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c1_ref_fx - (int64)CONST_5P4_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P9_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c1_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c1_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c1_dec_fx, 0, 31));
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_C1, rfpll_lf_lf_c1_ideal);

	/* rfpll_lf_lf_c2 */
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, 15, NF16, NF0, &rfpll_lf_lf_c2_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_lf_lf_c2_ref_fx = fp_round_32(rfpll_lf_lf_c2_ref_fx, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c2_ref_fx - (int64)CONST_3P4_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P6_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c2_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c2_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c2_dec_fx, 0, 31));
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_C2, rfpll_lf_lf_c2_ideal);

	/* rfpll_lf_lf_c3 */
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, C_CONSTANT, NF16, NF0, &rfpll_lf_lf_c3_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_lf_lf_c3_ref_fx = fp_round_32(rfpll_lf_lf_c3_ref_fx, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c3_ref_fx - (int64)CONST_3P4_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P6_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c3_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c3_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c3_dec_fx, 0, 31));
	}

	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_C3, rfpll_lf_lf_c3_ideal);

	/* rfpll_lf_lf_c4 */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_LF_LF_C4, rfpll_lf_lf_c3_ideal);

	/* cp_kpd_scale */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_CP_KPD_SCALE,
			(uint16)(LIMIT((int32)rfpll_cp_kpd_scale_dec_fx[band_idx], 0, 255)));

	/* rfpll_cp_ioff */
	rfpll_cp_ioff_ref_fx = i_off_fx;
	/* round(<0.16.16>, 16) -> <0.32.0> */
	rfpll_cp_ioff_dec_fx = fp_round_32(rfpll_cp_ioff_ref_fx, NF16);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_CP_IOFF,
			(uint16)(LIMIT((int32)rfpll_cp_ioff_dec_fx, 0, 255)));
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Put other register values to structure */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_RFVCO_KVCO_CODE, (uint16)kvco_code_fx[band_idx]);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_RFVCO_TEMP_CODE, (uint16)temp_code_fx[band_idx]);
	/* ------------------------------------------------------------------- */
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
phy_ac_radio20694_pll_config_ch_dep_calc(phy_info_t *pi, uint32 lo_freq, uint8 vco_select,
	uint32 icp_fx, uint32 loop_band, uint8 ac_mode, pll_config_20694_tbl_t *pll)
{
	uint32 pfd_ref_freq_fx;
	uint8 nf;
	uint64 temp_64;
	uint32 temp_32;
	uint32 temp_32_1;
	uint32 vco_freq;
	uint32 divide_ratio_fx;
	uint8 enable_coupled_VCO;
	uint8 select_VCO;
	uint8 rfpll_vcocal_enablecoupling_dec;
	uint32 lo_div_vco_ratio_fx;
	uint32 rfpll_vcocal_XtalCount_raw_fx;
	uint32 rfpll_vcocal_TargetCountCenter = 0;
	uint32 rfpll_frct_wild_base_dec_fx;
	uint32 wn_fx;
	uint32 loop_band_square_fx;
	uint32 c1_passive_lf_fx;
	uint32 c1_cap_multiplier_mode_fx;
	uint32 i_off_fx;
	uint64 r1_passive_lf_fx;
	uint64 rf_fx;
	uint64 rs_fx;
	uint32 rfpll_lf_lf_r2_dec_fx;
	uint64 rfpll_lf_lf_rs_cm_ref_fx;
	uint32 rfpll_lf_lf_rs_cm_dec_fx;
	uint64 rfpll_lf_lf_rf_cm_ref_fx;
	uint32 rfpll_lf_lf_rf_cm_dec_fx;
	uint32 rfpll_lf_lf_c1_ref_fx;
	uint32 rfpll_lf_lf_c1_dec_fx;
	uint32 rfpll_lf_lf_c2_dec_fx;
	uint32 rfpll_lf_lf_c2_ref_fx;
	uint32 rfpll_lf_lf_c3_ref_fx;
	uint32 rfpll_lf_lf_c3_dec_fx;
	uint32 rfpll_lf_lf_c4_ref_fx;
	uint32 rfpll_lf_lf_c4_dec_fx;
	uint32 rfpll_cp_kpd_scale_ref_fx;
	uint32 rfpll_cp_kpd_scale_dec_fx;
	uint32 rfpll_cp_ioff_dec_fx;
	int64 temp_sign;
	uint16 rfpll_vcocal_force_caps2_ovr_dec;
	uint16 rfpll_vcocal_force_caps_ovr_dec;
	uint16 rfpll_vcocal_CouplThres2_dec = 0;
	uint16 rfpll_vco_core1_en_dec = 0;
	uint16 rfpll_vco_core2_en_dec = 0;
	uint16	rfpll_vco_cvar_dec = 0;
	uint16 rfpll_vco_ALC_ref_ctrl_dec = 0;
	uint16 rfpll_vco_bias_mode_dec = 0;
	uint16 rfpll_vco_tempco_dcadj_dec = 0;
	uint16 rfpll_vco_tempco_dec = 0;
	uint16 rfpll_vcocal_XtalCount_dec;
	uint16 rfpll_vcocal_DeltaPllVal_dec;
	uint16 rfpll_vcocal_ErrorThres = 0;
	uint16 rfpll_vcocal_InitCapA = 0;
	uint16 rfpll_vcocal_InitCapB = 0;
	uint16 rfpll_vcocal_NormCountLeft;
	uint16 rfpll_vcocal_NormCountRight;
	uint16 rfpll_vcocal_TargetCountCenter_dec;
	uint16 rfpll_vcocal_MidCodeSel_dec;
	uint16 rfpll_lf_lf_r2_ideal = 0;
	uint16 rfpll_lf_lf_rs_cm_ideal = 0;
	uint16 rfpll_lf_lf_rf_cm_ideal = 0;
	uint16 rfpll_lf_lf_c1_ideal = 0;
	uint16 rfpll_lf_lf_c2_ideal = 0;
	uint16 rfpll_lf_lf_c3_ideal = 0;
	uint16 rfpll_lf_lf_c4_ideal = 0;
	uint16 rfpll_cp_kpd_scale_ideal = 0;
	uint16 rfpll_cp_ioff_ideal = 0;

	/* ------------------------------------------------------------------- */
	/* PFD Reference Frequency */
	/* <0.8.24> * <0.32.0> --> <0.8.24> */
	pfd_ref_freq_fx = pll->xtal_fx << pll->use_doubler;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* VCO Frequency */
	/* In 2G regular mode. VCO = LO x 3/2, In 2G RSDB mode(some channels) */
	/* VCO = LO x 4/3 to avoid VCO pulling #C14 */
	/* In 5G, VCO  = LO x 2/3  #C15 */
	/* lo_freq #C9,	## logen_mode = C14  */
	if (lo_freq <= 3000 && pll->logen_mode == 0) {
		/* <0.32.0> * <0.32.0> --> <0.32.0> */
		vco_freq = (uint32)fp_mult_64(lo_freq, 3, NF0, NF0, NF0);
		/* <0.32.0> / <0.32.0> --> <0.(32 - nf).nf> */
		nf = fp_div_64(vco_freq, 2, NF0, NF0, &vco_freq);
		/* floor(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
		vco_freq = fp_floor_32(vco_freq, (nf - NF16));
	} else if (lo_freq <= 3000 && pll->logen_mode == 1) {
		/* <0.32.0> * <0.32.0> --> <0.32.0> */
		vco_freq = (uint32)fp_mult_64(lo_freq, 4, NF0, NF0, NF0);
		/* <0.32.0> / <0.32.0> --> <0.(32 - nf).nf> */
		nf = fp_div_64(vco_freq, 3, NF0, NF0, &vco_freq);
		/* floor(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
		vco_freq = fp_floor_32(vco_freq, (nf - NF16));
	} else {
		/* <0.32.0> / <0.32.0> --> <0.(32 - nf).nf> */
		nf = fp_div_64((lo_freq << 1), 3, NF0, NF0, &vco_freq);
		/* floor(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
		vco_freq = fp_floor_32(vco_freq, (nf - NF16));
	}
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	pll->loop_bw = loop_band;
	if (vco_select == 0) {
		enable_coupled_VCO = 0;
		select_VCO = 0;
		rfpll_vcocal_force_caps_ovr_dec = 0;
		rfpll_vcocal_force_caps2_ovr_dec = 1; /* VCO1 */
	} else if (vco_select == 1) {
		enable_coupled_VCO = 0;
		select_VCO = 1;
		rfpll_vcocal_force_caps_ovr_dec = 1;
		rfpll_vcocal_force_caps2_ovr_dec = 0; /* VCO2 */
	} else if (vco_select == 2) {
		enable_coupled_VCO = 1;
		select_VCO = 0;
		rfpll_vcocal_force_caps_ovr_dec = 0;
		rfpll_vcocal_force_caps2_ovr_dec = 0; /* Coupled VCO */
	} else {
		PHY_ERROR(("wl%d: %s: vco_select arg must be 0(VCO1),1(VCO2) or 2(VCO1+VCO2)\n",
			pi->sh->unit, __FUNCTION__));
		ASSERT(0);
		return;
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS_OVR,
		rfpll_vcocal_force_caps_ovr_dec);
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVR,
		rfpll_vcocal_force_caps2_ovr_dec);
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* VCO Frequency dependent calculations */
	/* <0.16.16> / <0.8.24> --> <0.(32 - nf).nf> */
	nf = fp_div_64(vco_freq, pfd_ref_freq_fx, NF16, NF24, &divide_ratio_fx);
	/* floor(<0.(32-nf).nf>, (nf-20)) -> 0.12.20 */
	divide_ratio_fx = fp_floor_32(divide_ratio_fx, (nf - NF20));

	/* <0.16.16> / <0.16.16> --> <0.(32 - nf).nf> */
	nf = fp_div_64(lo_freq, vco_freq, NF0, NF16, &lo_div_vco_ratio_fx);
	/* floor(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
	lo_div_vco_ratio_fx = fp_floor_32(lo_div_vco_ratio_fx, (nf - NF16));

	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Defines the duration of time that vcocal should wait after it applied a
	   new calibration bit to to vco before it starts to measure vco frequency
	   to calculate the next bit. This time is required by vco to settle.
	*/
	/* <0.16.16> * <0.8.24> --> <0.8.16> */
	rfpll_vcocal_XtalCount_raw_fx = (uint32)fp_mult_64(lo_div_vco_ratio_fx << 1,
		pll->xtal_fx, NF16, NF24, NF16);
	/* ceil(<0.8.16>, 16) -> <0.8.0> */
	rfpll_vcocal_XtalCount_dec = (uint16)fp_ceil_64(rfpll_vcocal_XtalCount_raw_fx, NF16);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_XTALCOUNT,
		rfpll_vcocal_XtalCount_dec);

	/* <0.32.0> / <0.16.16> --> <0.(32 - nf).nf> */
	nf = fp_div_64(rfpll_vcocal_XtalCount_dec, rfpll_vcocal_XtalCount_raw_fx,
			NF0, NF16, &temp_32);
	/* round(<0.(32-nf).nf>, (nf-16)) -> 0.16.16 */
	temp_32 = fp_round_32(temp_32, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_32	= temp_32 - FX_ONE;
	/* round(<0.16.16> * <0.32.0> ,16) -> <0.32.0> */
	rfpll_vcocal_DeltaPllVal_dec = (uint16)fp_round_64((temp_32 * (1 << 13)), 16);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_DELTAPLLVAL,
		rfpll_vcocal_DeltaPllVal_dec);

	/* Frequency range dependent constants */
	if (lo_freq >= 2000 && lo_freq <= 3000) {
		if (pll->logen_mode == 0) {
			rfpll_vcocal_InitCapA = 1536;
			rfpll_vcocal_InitCapB = 1792;
			rfpll_vcocal_CouplThres2_dec = 25;
		} else {
			rfpll_vcocal_InitCapA = 2944;
			rfpll_vcocal_InitCapB = 3200;
			rfpll_vcocal_CouplThres2_dec = 40;
		}
		rfpll_vcocal_ErrorThres  = 3;
		rfpll_vcocal_TargetCountCenter  = 2442;
	} else if  (lo_freq < 5250) {
		rfpll_vcocal_InitCapA  = 2432;
		rfpll_vcocal_InitCapB  = 2688;
		rfpll_vcocal_ErrorThres = 5;
		rfpll_vcocal_CouplThres2_dec = 40;
		rfpll_vcocal_TargetCountCenter = 5070;
	} else if (lo_freq >= 5250 && lo_freq < 5500) {
		rfpll_vcocal_InitCapA = 1792;
		rfpll_vcocal_InitCapB = 2048;
		rfpll_vcocal_ErrorThres = 7;
		rfpll_vcocal_CouplThres2_dec = 40;
		rfpll_vcocal_TargetCountCenter = 5370;
	} else if (lo_freq >= 5500) {
		rfpll_vcocal_InitCapA = 1088;
		rfpll_vcocal_InitCapB = 1344;
		rfpll_vcocal_ErrorThres = 8;
		rfpll_vcocal_CouplThres2_dec = 40;
		rfpll_vcocal_TargetCountCenter = 5700;
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_ERRORTHRES, rfpll_vcocal_ErrorThres);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_INITCAPA, rfpll_vcocal_InitCapA);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_INITCAPB, rfpll_vcocal_InitCapB);
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLTHRES2,
			rfpll_vcocal_CouplThres2_dec);

	if (lo_freq >= 2000 && lo_freq <= 3000) {
		rfpll_vcocal_NormCountLeft = -39;
		rfpll_vcocal_NormCountRight = 39;
	} else {
		rfpll_vcocal_NormCountLeft = -50;
		rfpll_vcocal_NormCountRight = 50;
	}

	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_NORMCOUNTLEFT,
			rfpll_vcocal_NormCountLeft);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_NORMCOUNTRIGHT,
			rfpll_vcocal_NormCountRight);

	/* <0.32.0> - <0.32.0> -> <0.32.0> */
	// pll->rfpll_vcocal_RoundLSB = 12 - VCO_CAL_CAP_BITS;
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_ROUNDLSB, (12 - VCO_CAL_CAP_BITS));

	/* <0.32.0> * <0.32.0> -> <0.32.0> */
	temp_64 = rfpll_vcocal_TargetCountCenter * rfpll_vcocal_DeltaPllVal_dec;
	/* <0.32.0> + round(<0.32.0> * <0.16.16>, 16) -> <0.32.0> */
	rfpll_vcocal_TargetCountCenter_dec = (uint16)(rfpll_vcocal_TargetCountCenter +
			fp_round_64((temp_64 * FIX_2POW_MIN13), NF16));
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_TARGETCOUNTCENTER,
			rfpll_vcocal_TargetCountCenter_dec);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_PLL_VAL, (uint16)lo_freq);

	if (enable_coupled_VCO == 1) {
		rfpll_vcocal_enablecoupling_dec = 1;
	} else {
		rfpll_vcocal_enablecoupling_dec = 0;
	}
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_ENABLECOUPLING,
		rfpll_vcocal_enablecoupling_dec);

	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Other VCOCAL register (not channel/XTAL dependent) */
	if (RFPLL_VCOCAL_COUPLINGMODE_DEC == 0) {
		if ((VCO_CAL_AUX_CAP_BITS < 6) || (VCO_CAL_AUX_CAP_BITS > 9)) {
			PHY_ERROR(("wl%d: %s: Unsupported value VCO_cal_aux_cap_bits\n",
				pi->sh->unit, __FUNCTION__));
			ASSERT(0);
			return;
		}

		rfpll_vcocal_MidCodeSel_dec = VCO_CAL_AUX_CAP_BITS - 6;
	} else {
		if ((VCO_CAL_AUX_CAP_BITS < 4) || (VCO_CAL_AUX_CAP_BITS > 7)) {
			PHY_ERROR(("wl%d: %s: Unsupported value VCO_cal_aux_cap_bits\n",
				pi->sh->unit, __FUNCTION__));
			ASSERT(0);
			return;
		}

		rfpll_vcocal_MidCodeSel_dec = VCO_CAL_AUX_CAP_BITS - 4;
	}
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_MIDCODESEL, rfpll_vcocal_MidCodeSel_dec);
	/* ------------------------------------------------------------------- */
	/* VCO register */
	/* ------------------------------------------------------------------- */
	if (enable_coupled_VCO == 1) {
		rfpll_vco_core1_en_dec = 1;
		rfpll_vco_core2_en_dec = 1;
	} else if (select_VCO == 0) {
		rfpll_vco_core1_en_dec = 1;
		rfpll_vco_core2_en_dec = 0;
	} else if (select_VCO == 1) {
		rfpll_vco_core1_en_dec = 0;
		rfpll_vco_core2_en_dec = 1;
	}
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCO_CORE1_EN, rfpll_vco_core1_en_dec);
	PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCO_CORE2_EN, rfpll_vco_core2_en_dec);

	/* Fract-N (Sigma-Delta) register */
	rfpll_frct_wild_base_dec_fx = divide_ratio_fx;
	temp_32 = rfpll_frct_wild_base_dec_fx >> 16;
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_FRCT_WILD_BASE_HIGH,
			(uint16)(temp_32 & 0xFFFF));
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_FRCT_WILD_BASE_LOW,
			(uint16)(rfpll_frct_wild_base_dec_fx & 0xFFFF));

	/* ------------------------------------------------------------------- */
	/* VCO register (11n vs 11ac mode) */
	/* ------------------------------------------------------------------- */
	/* There is no Class-C VCO anymore. Always CMOS VCO */
	/* 2 VCO mode: 11abgn, 11ac */
	if (ac_mode == 0) {
		rfpll_vco_cvar_dec = 8;
		rfpll_vco_ALC_ref_ctrl_dec = 8;
		rfpll_vco_bias_mode_dec = 0;
		rfpll_vco_tempco_dcadj_dec = 1;
		rfpll_vco_tempco_dec = 3;
	} else if (ac_mode == 1) {
		rfpll_vco_cvar_dec = 8;
		rfpll_vco_ALC_ref_ctrl_dec = 13;
		rfpll_vco_bias_mode_dec = 1;
		rfpll_vco_tempco_dcadj_dec = 3;
		rfpll_vco_tempco_dec = 5;
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_CVAR, rfpll_vco_cvar_dec);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_ALC_REF_CTRL, rfpll_vco_ALC_ref_ctrl_dec);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_BIAS_MODE, rfpll_vco_bias_mode_dec);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_TEMPCO_DCADJ, rfpll_vco_tempco_dcadj_dec);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_TEMPCO, rfpll_vco_tempco_dec);

	/* ----------------------------- */
	/* CP and Loop Filter Registers  */
	/* ----------------------------- */
	/* <0.24.8> * <0.32.0> -> <0.26.6> */
	wn_fx = (uint32)fp_mult_64(WN_CONST_FX, loop_band, NF8, NF0, NF6);
	/* <0.32.0> * <0.32.0> -> <0.32.0> */
	loop_band_square_fx = loop_band * loop_band;

	/* <0.12.20> * <0.32.0> -> <0.26.6> */
	temp_32 = (uint32)fp_mult_64(divide_ratio_fx, loop_band_square_fx, NF20, NF0, NF6);
	/* <0.32.32> / <0.26.6> -> <0.(32-nf).nf> */
	nf = fp_div_64(C1_PASSIVE_LF_CONST_FX, temp_32, NF16, NF6, &temp_32_1);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	temp_32_1 = fp_round_32(temp_32_1, (nf - NF16));
	c1_passive_lf_fx = (uint32)fp_mult_64(temp_32_1, icp_fx, NF16, NF16, NF16);

	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, CAP_MULTIPLIER_RATIO_PLUS_ONE_20694, NF16, NF0,
			&c1_cap_multiplier_mode_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	c1_cap_multiplier_mode_fx = fp_round_32(c1_cap_multiplier_mode_fx, (nf - NF16));

	/* <0.16.16> * <0.8.24> -> <0.16.16> */
	temp_32 = (uint32)fp_mult_64(T_OFF_FX, pfd_ref_freq_fx, NF16, NF24, NF16);
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	i_off_fx = (uint32)fp_mult_64(temp_32, icp_fx, NF16, NF16, NF16);

	/* <0.26.6> * <0.0.32> -> <0.16.16> */
	temp_64 = fp_mult_64(wn_fx, R1_PASSIVE_LF_CONST_FX, NF6, NF32, NF16);
	/* <0.12.20> / <0.16.16> -> <0.(32-nf).nf> */
	nf = fp_div_64(divide_ratio_fx, icp_fx, NF20, NF16, &temp_32);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	temp_32 = fp_round_32(temp_32, (nf - NF16));
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(temp_32, 1000, NF16, NF0, &temp_32_1);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	temp_32_1 = fp_round_32(temp_32_1, (nf - NF16));
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	r1_passive_lf_fx = fp_mult_64(temp_64, temp_32_1, NF16, NF16, NF16);
	/* <0.16.16> * <0.16.16> -> <0.16.16> */
	rf_fx = fp_mult_64(r1_passive_lf_fx, RF_CONST1_FX, NF16, NF16, NF16);
	/* <0.32.0> * <0.16.16> -> <0.16.16> */
	rs_fx = CAP_MULTIPLIER_RATIO_20694 * rf_fx;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Register Definitions (lower) */
	/* rfpll_lf_lf_r2 */
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)(r1_passive_lf_fx - (int64)CONST_303_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_280_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_r2_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_r2_ideal = LIMIT((int32)rfpll_lf_lf_r2_dec_fx, 0, 255);
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_R2, rfpll_lf_lf_r2_ideal);

	/* rfpll_lf_lf_r3 */
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_R3, rfpll_lf_lf_r2_ideal);

	/* rfpll_lf_lf_rs_cm */
	rfpll_lf_lf_rs_cm_ref_fx = rs_fx;   /* cap_multiplier_PU = 1 */
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_rs_cm_ref_fx - (int64)CONST_303_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_280_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_rs_cm_dec_fx =  fp_round_32(temp_32, nf);
		rfpll_lf_lf_rs_cm_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_rs_cm_dec_fx,
			0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_RS_CM, rfpll_lf_lf_rs_cm_ideal);

	/* rfpll_lf_lf_rf_cm */
	rfpll_lf_lf_rf_cm_ref_fx = rf_fx;  /* cap_multiplier_PU = 1 */
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_rf_cm_ref_fx - (int64)CONST_303_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_280_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_rf_cm_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_rf_cm_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_rf_cm_dec_fx,
				0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_RF_CM, rfpll_lf_lf_rf_cm_ideal);

	/* rfpll_lf_lf_c1 */
	rfpll_lf_lf_c1_ref_fx  = c1_cap_multiplier_mode_fx;
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c1_ref_fx - (int64)CONST_3_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P52_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c1_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c1_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c1_dec_fx, 0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_C1, rfpll_lf_lf_c1_ideal);

	/* rfpll_lf_lf_c2 */
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, 20, NF16, NF0, &rfpll_lf_lf_c2_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_lf_lf_c2_ref_fx = fp_round_32(rfpll_lf_lf_c2_ref_fx, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c2_ref_fx - (int64)CONST_1P32_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P25_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c2_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c2_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c2_dec_fx, 0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_C2, rfpll_lf_lf_c2_ideal);

	/* rfpll_lf_lf_c3 */
	/* <0.16.16> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(c1_passive_lf_fx, 40, NF16, NF0, &rfpll_lf_lf_c3_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_lf_lf_c3_ref_fx = fp_round_32(rfpll_lf_lf_c3_ref_fx, (nf - NF16));
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c3_ref_fx - (int64)CONST_P663_FX);
	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P125_FX, NF16, NF16, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c3_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c3_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c3_dec_fx, 0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_C3, rfpll_lf_lf_c3_ideal);

	/* rfpll_lf_lf_c4 */
	rfpll_lf_lf_c4_ref_fx = rfpll_lf_lf_c3_ref_fx >> 1;
	/* <0.16.16> - <0.16.16> -> <0.16.16> */
	temp_sign = (int64)((int64)rfpll_lf_lf_c4_ref_fx - (int64)CONST_P317_FX);

	if (temp_sign > 0) {
		/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
		nf = fp_div_64((uint64)temp_sign, CONST_P061_FX, NF16, NF32, &temp_32);
		/* round(<0.(32-nf).nf>, nf) -> <0.32.0> */
		rfpll_lf_lf_c4_dec_fx = fp_round_32(temp_32, nf);
		rfpll_lf_lf_c4_ideal = (uint16)(LIMIT((int32)rfpll_lf_lf_c4_dec_fx, 0, 255));
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_LF_LF_C4, rfpll_lf_lf_c4_ideal);

	/* cp_kpd_scale */
	/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
	nf = fp_div_64(icp_fx, CONST_P0384_FX, NF16, NF0, &rfpll_cp_kpd_scale_ref_fx);
	/* round(<0.(32-nf).nf>, (nf-16)) -> <0.16.16> */
	rfpll_cp_kpd_scale_dec_fx = fp_round_32(rfpll_cp_kpd_scale_ref_fx, (nf - NF16));
	rfpll_cp_kpd_scale_ideal = (uint16)(LIMIT((int32)rfpll_cp_kpd_scale_dec_fx,
			0, 127));
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_CP_KPD_SCALE, rfpll_cp_kpd_scale_ideal);

	/* rfpll_cp_ioff */
	/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
	nf = fp_div_64(i_off_fx, CONST_P032_FX, NF16, NF16, &temp_32);
	/* round(<0.16.16>, 16) -> <0.16.16> */
	temp_32 = fp_round_32(temp_32, (nf - NF16));
	/* <0.16.16> / <0.16.16> -> <0.(32-nf).nf> */
	nf = fp_div_64(temp_32, rfpll_cp_kpd_scale_dec_fx, NF16, NF16, &temp_32_1);
	/* round(<0.(32-nf).nf>, (nf+16)) -> <0.16.16> ; here (nf+16) since nf < 16 */
	rfpll_cp_ioff_dec_fx = fp_round_32(temp_32_1, (nf+NF16));
	rfpll_cp_ioff_ideal = (uint16)(LIMIT((int32)rfpll_cp_ioff_dec_fx, 0, 255));
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_CP_IOFF, rfpll_cp_ioff_ideal);
	/* ------------------------------------------------------------------- */

	#if defined(PRINT_PLL_CONFIG_FLAG_20694) && PRINT_PLL_CONFIG_FLAG_20694
	/* ------------------------------------------------------------------- */
		print_pll_config_20694(pll, lo_freq, loop_band, icp_fx);
	#endif /* End of PRINT_PLL_CONFIG_FLAG_20694 */
}

void
print_pll_config_20694(pll_config_20694_tbl_t *pll, uint32 lo_freq, uint32 loop_bw, uint32 icp_fx)
{
	printf("------------------------------------------------------------\n");
	printf("\nLO_Freq = %d MHz\n", lo_freq);
	printf("\nLoopFilter_bandwidth = %d MHz\n", loop_bw);
	printf("\nIcp_fx = %u (0.16.16 format)\n", icp_fx);
	printf("\nRegister Definition (upper):\n");
	printf("------------------------------------------------------------\n");

	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_DELAYEND);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_DELAYSTARTCOLD);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_DELAYSTARTWARM);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_XTALCOUNT);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_DELTAPLLVAL);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_ERRORTHRES);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FASTSWITCH);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FIXMSB);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FOURTHMESEN);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_INITCAPA);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_INITCAPB);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_NORMCOUNTLEFT);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_NORMCOUNTRIGHT);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_PAUSECNT);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_ROUNDLSB);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_TARGETCOUNTBASE);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_TARGETCOUNTCENTER);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_OFFSETIN);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_PLL_VAL);

	printf("\nother VCOCAL register (not channel/XTAL dependent):\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_SLOPEIN);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_UPDATESEL);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_CALCAPRBMODE);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_TESTVCOCNT);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FORCE_CAPS_OVR);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FORCE_CAPS_OVRVAL);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FAST_SETTLE_OVR);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FAST_SETTLE_OVRVAL);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVR);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL);

	printf("\nVCOCAL register (required by coupled VCOs):\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_ENABLECOUPLING);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_COUPLINGMODE);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_SWAPVCO12);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_MIDCODESEL);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_COUPLTHRES);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_COUPLTHRES2);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_SECONDMESEN);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_UPDATESELCOUP);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_COUPLINGIN);

	printf("\nVCOCAL register (optional for coupled VCOs):\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVR);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_AUX1_OVR);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_AUX1_OVRVAL);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_AUX2_OVR);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCOCAL_FORCE_AUX2_OVRVAL);

	printf("\nVCO register:\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCO_CORE1_EN);
	PRINT_PLL_CONFIG_20694_MAIN(pll, RFPLL_VCO_CORE2_EN);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_CAP_MODE);

	printf("\nFract-N (Sigma-Delta) register:\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694(pll, RFPLL_FRCT_WILD_BASE_HIGH);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_FRCT_WILD_BASE_LOW);

	printf("\nVCO Register (11n vs 11ac mode):\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_CVAR);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_ALC_REF_CTRL);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_BIAS_MODE);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_TEMPCO_DCADJ);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_VCO_TEMPCO);

	printf("\nCP and Loop Filter register:\n");
	printf("------------------------------------------------------------\n");
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_R2);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_R3);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_RS_CM);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_RF_CM);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_C1);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_C2);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_C3);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_LF_LF_C4);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_CP_IOFF);
	PRINT_PLL_CONFIG_20694(pll, RFPLL_CP_KPD_SCALE);

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
BCMATTACHFN(phy_ac_radio20695_pll_config_const_calc)(phy_info_t *pi, pll_config_20695_tbl_t *pll)
{
	uint32 xtal_freq;
	uint32 xtal_fx;
	uint64 temp_64;
	uint8 nf;

	/* Default parameters to be used in chan dependent computation */
	pll->use_doubler = USE_DOUBLER;
	pll->loop_bw = LOOP_BW;
	pll->logen_mode = LOGEN_MODE;

	/* ------------------------------------------------------------------- */
	/* XTAL frequency in 1.7.24 */
	xtal_freq = pi->xtalfreq;
	/* <0.32.0> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(xtal_freq, 1000000, 0, 0, &xtal_fx);
	/* round(<0.(32-nf).nf>, (nf-24)) -> <0.8.24> */
	pll->xtal_fx = fp_round_32(xtal_fx, (nf - NF24));
	xtal_fx = pll->xtal_fx;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* XTAL dependent */
	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYEND_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYEND, (uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYSTARTCOLD_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYSTARTCOLD,
			(uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYSTARTWARM_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYSTARTWARM,
			(uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_PAUSECNT_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_PAUSECNT, (uint16)fp_round_64(temp_64, NF16));
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Put other register values to structure */
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_TARGETCOUNTBASE, RFPLL_VCOCAL_TARGETCOUNTBASE);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_OFFSETIN, RFPLL_VCOCAL_OFFSETIN);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_UPDATESEL, RFPLL_VCOCAL_UPDATESEL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_CALCAPRBMODE, RFPLL_VCOCAL_CALCAPRBMODE_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_TESTVCOCNT, RFPLL_VCOCAL_TESTVCOCNT_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS_OVR,
			RFPLL_VCOCAL_FORCE_CAPS_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS_OVRVAL,
			RFPLL_VCOCAL_FORCE_CAPS_OVRVAL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FAST_SETTLE_OVR,
			RFPLL_VCOCAL_FAST_SETTLE_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FAST_SETTLE_OVRVAL,
			RFPLL_VCOCAL_FAST_SETTLE_OVRVAL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVR,
			RFPLL_VCOCAL_FORCE_VCTRL_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL,
			RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_ENABLECOUPLING,
			RFPLL_VCOCAL_ENABLECOUPLING_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLINGMODE, RFPLL_VCOCAL_COUPLINGMODE_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_SWAPVCO12, RFPLL_VCOCAL_SWAPVCO12_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLTHRES, RFPLL_VCOCAL_COUPLTHRES_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLTHRES2, RFPLL_VCOCAL_COUPLTHRES2_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_SECONDMESEN, RFPLL_VCOCAL_SECONDMESEN_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_UPDATESELCOUP, RFPLL_VCOCAL_UPDATESELCOUP_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLINGIN, RFPLL_VCOCAL_COUPLINGIN_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVR,
			RFPLL_VCOCAL_FORCE_CAPS2_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL,
			RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX1_OVR,
			RFPLL_VCOCAL_FORCE_AUX1_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX1_OVRVAL,
			RFPLL_VCOCAL_FORCE_AUX1_OVRVAL_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX2_OVR,
			RFPLL_VCOCAL_FORCE_AUX2_OVR_DEC);
	PLL_CONFIG_20695_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX2_OVRVAL,
			RFPLL_VCOCAL_FORCE_AUX2_OVRVAL_DEC);

	PLL_CONFIG_20695_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_OFFSETIN, 0);
	PLL_CONFIG_20695_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_COUPLINGIN, 1);
	PLL_CONFIG_20695_VAL_ENTRY(pll, OVR_MUX_XT_DEL, 1);
	PLL_CONFIG_20695_VAL_ENTRY(pll, OVR_MUX_SEL_BAND, 1);
	/* ------------------------------------------------------------------- */
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
BCMATTACHFN(phy_ac_radio20694_pll_config_const_calc)(phy_info_t *pi, pll_config_20694_tbl_t *pll)
{
	uint32 xtal_freq;
	uint32 xtal_fx;
	uint64 temp_64;
	uint8 nf;
	uint sicoreunit;

	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	/* Default parameters to be used in chan dependent computation */
	pll->use_doubler = USE_DOUBLER;
	// pll->loop_bw = LOOP_BW;
	pll->logen_mode = LOGEN_MODE;

	/* ------------------------------------------------------------------- */
	/* XTAL frequency in 1.7.24 */
	xtal_freq = pi->xtalfreq;
	/* <0.32.0> / <0.32.0> -> <0.(32-nf).nf> */
	nf = fp_div_64(xtal_freq, 1000000, 0, 0, &xtal_fx);
	/* round(<0.(32-nf).nf>, (nf-24)) -> <0.8.24> */
	pll->xtal_fx = fp_round_32(xtal_fx, (nf - NF24));
	xtal_fx = pll->xtal_fx;
	/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* XTAL dependent */
	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYEND_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYEND, (uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYSTARTCOLD_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYSTARTCOLD,
			(uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_DELAYSTARTWARM_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_DELAYSTARTWARM,
			(uint16)fp_round_64(temp_64, NF16));

	/* <0.8.24> * <0.16.16> -> <0.16.16> */
	temp_64 = fp_mult_64(xtal_fx, RFPLL_VCOCAL_PAUSECNT_FX, NF24, NF16, NF16);
	/* round(0.16.16, 16) -> <0.32.0> */
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_PAUSECNT, (uint16)fp_round_64(temp_64, NF16));
/* ------------------------------------------------------------------- */

	/* ------------------------------------------------------------------- */
	/* Put other register values to structure */
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_SLOPEIN, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_OFFSETIN, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_RST_N, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_RFPLL_VCOCAL_COUPLINGIN, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, SEL_BAND, 3);
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_MUX_XT_DEL, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, OVR_MUX_SEL_BAND, 1);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_TARGETCOUNTBASE, RFPLL_VCOCAL_TARGETCOUNTBASE);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_OFFSETIN, RFPLL_VCOCAL_OFFSETIN);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_SLOPEIN, RFPLL_VCOCAL_SLOPEIN_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_UPDATESEL, RFPLL_VCOCAL_UPDATESEL_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_CALCAPRBMODE, RFPLL_VCOCAL_CALCAPRBMODE_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FASTSWITCH, RFPLL_VCOCAL_FASTSWITCH_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FIXMSB, RFPLL_VCOCAL_FIXMSB_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FOURTHMESEN, RFPLL_VCOCAL_FOURTHMESEN_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_TESTVCOCNT, RFPLL_VCOCAL_TESTVCOCNT_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS_OVR,
		RFPLL_VCOCAL_FORCE_CAPS_OVR_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS_OVRVAL,
		RFPLL_VCOCAL_FORCE_CAPS_OVRVAL_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FAST_SETTLE_OVR,
		RFPLL_VCOCAL_FAST_SETTLE_OVR_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FAST_SETTLE_OVRVAL,
		RFPLL_VCOCAL_FAST_SETTLE_OVRVAL_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVR,
		RFPLL_VCOCAL_FORCE_VCTRL_OVR_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL,
		RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL_DEC);
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCOCAL_TARGETCOUNTBASE,
		RFPLL_VCOCAL_TARGETCOUNTBASE_DEC);
	if (sicoreunit == DUALMAC_MAIN) {
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLINGMODE,
			RFPLL_VCOCAL_COUPLINGMODE_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_SWAPVCO12,
			RFPLL_VCOCAL_SWAPVCO12_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLTHRES,
			RFPLL_VCOCAL_COUPLTHRES_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_SECONDMESEN,
			RFPLL_VCOCAL_SECONDMESEN_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_UPDATESELCOUP,
			RFPLL_VCOCAL_UPDATESELCOUP_DEC_20694);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_COUPLINGIN,
			RFPLL_VCOCAL_COUPLINGIN_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL,
			RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX1_OVR,
			RFPLL_VCOCAL_FORCE_AUX1_OVR_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX1_OVRVAL,
			RFPLL_VCOCAL_FORCE_AUX1_OVRVAL_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX2_OVR,
			RFPLL_VCOCAL_FORCE_AUX2_OVR_DEC);
		PLL_CONFIG_20694_MAIN_VAL_ENTRY(pll, RFPLL_VCOCAL_FORCE_AUX2_OVRVAL,
			RFPLL_VCOCAL_FORCE_AUX2_OVRVAL_DEC);
	}
	PLL_CONFIG_20694_VAL_ENTRY(pll, RFPLL_VCO_CAP_MODE, RFPLL_VCO_CAP_MODE_DEC);
	/* ------------------------------------------------------------------- */
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static int
BCMATTACHFN(phy_ac_radio20695_populate_pll_config_tbl)(phy_info_t *pi,
		pll_config_20695_tbl_t *pll)
{
	if ((pll->reg_addr_2g =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_addr_2g failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_mask_2g =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_mask_2g failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_shift_2g =
		phy_malloc(pi, (sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_shift_2g failed\n", __FUNCTION__));
		goto fail;
	}

#if defined(DBAND) || defined(USE_5G_PLL_FOR_2G)
	if ((pll->reg_addr_5g =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_addr_5g failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_mask_5g =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_mask_5g failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_shift_5g =
		phy_malloc(pi, (sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_shift_5g failed\n", __FUNCTION__));
		goto fail;
	}
#endif /* DBAND || USE_5G_PLL_FOR_2G */

	if ((pll->reg_field_val =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_val failed\n", __FUNCTION__));
		goto fail;
	}

	/* Register addresses */
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_OFFSETIN, RFP, PLL_VCOCAL_OVR1,
			PLL5G_VCOCAL_OVR1, ovr_rfpll_vcocal_offsetIn,
			ovr_rfpll_5g_vcocal_offsetIn);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_COUPLINGIN, RFP, PLL_VCOCAL_OVR1,
			PLL5G_VCOCAL_OVR1, ovr_rfpll_vcocal_couplingIn,
			ovr_rfpll_5g_vcocal_couplingIn);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, OVR_MUX_XT_DEL, RFP, PLL_MUXSELECT_LINE,
			PLL5G_MUXSELECT_LINE, ovr_mux_xt_del, ovr_pll5g_mux_xt_del);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, OVR_MUX_SEL_BAND, RFP, PLL_MUXSELECT_LINE,
			PLL5G_MUXSELECT_LINE, ovr_mux_sel_band, ovr_pll5g_mux_sel_band);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, XTALCNT_2G, RFP, PLL_XTAL_CNT1, PLL5G_XTAL_CNT1,
			XtalCnt_2g, pll5g_XtalCnt_2g);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, XTALCNT_5G, RFP, PLL_XTAL_CNT2, PLL5G_XTAL_CNT2,
			XtalCnt_5g, pll5g_XtalCnt_5g);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYEND, RFP, PLL_VCOCAL18,
			PLL5G_VCOCAL18, rfpll_vcocal_DelayEnd, rfpll_5g_vcocal_DelayEnd);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYSTARTCOLD, RFP, PLL_VCOCAL3,
			PLL5G_VCOCAL3, rfpll_vcocal_DelayStartCold,
			rfpll_5g_vcocal_DelayStartCold);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYSTARTWARM, RFP, PLL_VCOCAL4,
			PLL5G_VCOCAL4, rfpll_vcocal_DelayStartWarm,
			rfpll_5g_vcocal_DelayStartWarm);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_XTALCOUNT, RFP, PLL_VCOCAL7,
			PLL5G_VCOCAL7, rfpll_vcocal_XtalCount, rfpll_5g_vcocal_XtalCount);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_CALCAPRBMODE, RFP, PLL_VCOCAL7,
			PLL5G_VCOCAL7, rfpll_vcocal_CalCapRBMode, rfpll_5g_vcocal_CalCapRBMode);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELTAPLLVAL, RFP, PLL_VCOCAL8,
			PLL5G_VCOCAL8, rfpll_vcocal_DeltaPllVal, rfpll_5g_vcocal_DeltaPllVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ERRORTHRES, RFP, PLL_VCOCAL20,
			PLL5G_VCOCAL20, rfpll_vcocal_ErrorThres, rfpll_5g_vcocal_ErrorThres);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_INITCAPA, RFP, PLL_VCOCAL12,
			PLL5G_VCOCAL12, rfpll_vcocal_InitCapA, rfpll_5g_vcocal_InitCapA);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_INITCAPB, RFP, PLL_VCOCAL13,
			PLL5G_VCOCAL13, rfpll_vcocal_InitCapB, rfpll_5g_vcocal_InitCapB);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_NORMCOUNTLEFT, RFP, PLL_VCOCAL10,
			PLL5G_VCOCAL10, rfpll_vcocal_NormCountLeft, rfpll_5g_vcocal_NormCountLeft);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_NORMCOUNTRIGHT, RFP, PLL_VCOCAL11,
			PLL5G_VCOCAL11, rfpll_vcocal_NormCountRight,
			rfpll_5g_vcocal_NormCountRight);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_PAUSECNT, RFP, PLL_VCOCAL19,
			PLL5G_VCOCAL19, rfpll_vcocal_PauseCnt, rfpll_5g_vcocal_PauseCnt);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ROUNDLSB, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_RoundLSB, rfpll_5g_vcocal_RoundLSB);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_UPDATESEL, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_UpdateSel, rfpll_5g_vcocal_UpdateSel);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TESTVCOCNT, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_testVcoCnt, rfpll_5g_vcocal_testVcoCnt);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FAST_SETTLE_OVR, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_fast_settle_ovr,
			rfpll_5g_vcocal_fast_settle_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FAST_SETTLE_OVRVAL, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_fast_settle_ovrVal,
			rfpll_5g_vcocal_fast_settle_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_VCTRL_OVR, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_force_vctrl_ovr,
			rfpll_5g_vcocal_force_vctrl_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL, RFP, PLL_VCOCAL1,
			PLL5G_VCOCAL1, rfpll_vcocal_force_vctrl_ovrVal,
			rfpll_5g_vcocal_force_vctrl_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TARGETCOUNTBASE, RFP, PLL_VCOCAL6,
			PLL5G_VCOCAL6, rfpll_vcocal_TargetCountBase,
			rfpll_5g_vcocal_TargetCountBase);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TARGETCOUNTCENTER, RFP, PLL_VCOCAL9,
			PLL5G_VCOCAL9, rfpll_vcocal_TargetCountCenter,
			rfpll_5g_vcocal_TargetCountCenter);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_OFFSETIN, RFP, PLL_VCOCAL17,
			PLL5G_VCOCAL17, rfpll_vcocal_offsetIn, rfpll_5g_vcocal_offsetIn);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_PLL_VAL, RFP, PLL_VCOCAL5,
			PLL5G_VCOCAL5, rfpll_vcocal_pll_val, rfpll_5g_vcocal_pll_val);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS_OVR, RFP, PLL_VCOCAL2,
			PLL5G_VCOCAL2, rfpll_vcocal_force_caps_ovr,
			rfpll_5g_vcocal_force_caps_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS_OVRVAL, RFP, PLL_VCOCAL2,
			PLL5G_VCOCAL2, rfpll_vcocal_force_caps_ovrVal,
			rfpll_5g_vcocal_force_caps_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_FRCT_WILD_BASE_HIGH, RFP, PLL_FRCT2,
			PLL5G_FRCT2, rfpll_frct_wild_base_high, rfpll_5g_frct_wild_base_high);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_FRCT_WILD_BASE_LOW, RFP, PLL_FRCT3,
			PLL5G_FRCT3, rfpll_frct_wild_base_low, rfpll_5g_frct_wild_base_low);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ENABLECOUPLING, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_enableCoupling,
			rfpll_5g_vcocal_enableCoupling);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLINGMODE, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_couplingMode, rfpll_5g_vcocal_couplingMode);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_SWAPVCO12, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_swapVco12, rfpll_5g_vcocal_swapVco12);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_MIDCODESEL, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_MidCodeSel, rfpll_5g_vcocal_MidCodeSel);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_SECONDMESEN, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_SecondMesEn, rfpll_5g_vcocal_SecondMesEn);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_UPDATESELCOUP, RFP, PLL_VCOCAL24,
			PLL5G_VCOCAL24, rfpll_vcocal_UpdateSelCoup, rfpll_5g_vcocal_UpdateSelCoup);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLTHRES, RFP, PLL_VCOCAL26,
			PLL5G_VCOCAL26, rfpll_vcocal_CouplThres, rfpll_5g_vcocal_CouplThres);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLTHRES2, RFP, PLL_VCOCAL26,
			PLL5G_VCOCAL26, rfpll_vcocal_CouplThres2, rfpll_5g_vcocal_CouplThres2);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLINGIN, RFP, PLL_VCOCAL25,
			PLL5G_VCOCAL25, rfpll_vcocal_couplingIn, rfpll_5g_vcocal_couplingIn);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS2_OVR, RFP, PLL_VCOCAL21,
			PLL5G_VCOCAL21, rfpll_vcocal_force_caps2_ovr,
			rfpll_5g_vcocal_force_caps2_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL, RFP,
			PLL_VCOCAL21, PLL5G_VCOCAL21, rfpll_vcocal_force_caps2_ovrVal,
			rfpll_5g_vcocal_force_caps2_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX1_OVR, RFP, PLL_VCOCAL22,
			PLL5G_VCOCAL22, rfpll_vcocal_force_aux1_ovr,
			rfpll_5g_vcocal_force_aux1_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX1_OVRVAL, RFP, PLL_VCOCAL22,
			PLL5G_VCOCAL22, rfpll_vcocal_force_aux1_ovrVal,
			rfpll_5g_vcocal_force_aux1_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX2_OVR, RFP, PLL_VCOCAL23,
			PLL5G_VCOCAL23, rfpll_vcocal_force_aux2_ovr,
			rfpll_5g_vcocal_force_aux2_ovr);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX2_OVRVAL, RFP, PLL_VCOCAL23,
			PLL5G_VCOCAL23, rfpll_vcocal_force_aux2_ovrVal,
			rfpll_5g_vcocal_force_aux2_ovrVal);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_R2, RFP, PLL_LF4, PLL5G_LF4,
			rfpll_lf_lf_r2, rfpll_5g_lf_lf_r2);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_R3, RFP, PLL_LF5, PLL5G_LF5,
			rfpll_lf_lf_r3, rfpll_5g_lf_lf_r3);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_RS_CM, RFP, PLL_LF7, PLL5G_LF7,
			rfpll_lf_lf_rs_cm, rfpll_5g_lf_lf_rs_cm);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_RF_CM, RFP, PLL_LF7, PLL5G_LF7,
			rfpll_lf_lf_rf_cm, rfpll_5g_lf_lf_rf_cm);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C1, RFP, PLL_LF2, PLL5G_LF2,
			rfpll_lf_lf_c1, rfpll_5g_lf_lf_c1);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C2, RFP, PLL_LF2, PLL5G_LF2,
			rfpll_lf_lf_c2, rfpll_5g_lf_lf_c2);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C3, RFP, PLL_LF3, PLL5G_LF3,
			rfpll_lf_lf_c3, rfpll_5g_lf_lf_c3);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C4, RFP, PLL_LF3, PLL5G_LF3,
			rfpll_lf_lf_c4, rfpll_5g_lf_lf_c4);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_CP_IOFF, RFP, PLL_CP4, PLL5G_CP4,
			rfpll_cp_ioff, rfpll_5g_cp_ioff);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_CP_KPD_SCALE, RFP, PLL_CP4, PLL5G_CP4,
			rfpll_cp_kpd_scale, rfpll_5g_cp_kpd_scale);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_RFVCO_KVCO_CODE, RFP, PLL_RFVCO1,
			PLL5G_RFVCO1, rfpll_rfvco_KVCO_CODE, rfpll_5g_rfvco_KVCO_CODE);
	PLL_CONFIG_20695_REG_INFO_ENTRY(pi, pll, RFPLL_RFVCO_TEMP_CODE, RFP, PLL_RFVCO1,
			PLL5G_RFVCO1, rfpll_rfvco_TEMP_CODE, rfpll_5g_rfvco_TEMP_CODE);

	return BCME_OK;

fail:
	if (pll->reg_addr_2g != NULL) {
		phy_mfree(pi, pll->reg_addr_2g, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
	}

	if (pll->reg_field_mask_2g != NULL) {
		phy_mfree(pi, pll->reg_field_mask_2g,
				(sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
	}

	if (pll->reg_field_shift_2g != NULL) {
		phy_mfree(pi, pll->reg_field_shift_2g,
				(sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE));
	}

#if defined(DBAND) || defined(USE_5G_PLL_FOR_2G)
	if (pll->reg_addr_5g != NULL) {
		phy_mfree(pi, pll->reg_addr_5g, (sizeof(uint16) *
			PLL_CONFIG_20695_ARRAY_SIZE));
	}

	if (pll->reg_field_mask_5g != NULL) {
		phy_mfree(pi, pll->reg_field_mask_5g,
				(sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
	}

	if (pll->reg_field_shift_5g != NULL) {
		phy_mfree(pi, pll->reg_field_shift_5g,
				(sizeof(uint8) * PLL_CONFIG_20695_ARRAY_SIZE));
	}
#endif /* DBAND || USE_5G_PLL_FOR_2G */

	if (pll->reg_field_val != NULL) {
		phy_mfree(pi, pll->reg_field_val, (sizeof(uint16) * PLL_CONFIG_20695_ARRAY_SIZE));
	}

	return BCME_NOMEM;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static int
BCMATTACHFN(phy_ac_radio20694_populate_pll_config_tbl)(phy_info_t *pi,
		pll_config_20694_tbl_t *pll)
{

uint sicoreunit;
sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	if ((pll->reg_addr =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_addr failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_mask =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_mask failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_shift =
		phy_malloc(pi, (sizeof(uint8) * PLL_CONFIG_20694_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_shift failed\n", __FUNCTION__));
		goto fail;
	}

	if ((pll->reg_field_val =
		phy_malloc(pi, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE))) == NULL) {
		PHY_ERROR(("%s: phy_malloc reg_field_val failed\n", __FUNCTION__));
		goto fail;
	}

	if (sicoreunit == DUALMAC_MAIN) {
		if ((pll->reg_addr_main =
			phy_malloc(pi, (sizeof(uint16) *
				PLL_CONFIG_20694_MAIN_ARRAY_SIZE))) == NULL) {
			PHY_ERROR(("%s: phy_malloc reg_addr_main failed\n", __FUNCTION__));
			goto fail;
		}

		if ((pll->reg_field_mask_main =
			phy_malloc(pi, (sizeof(uint16) *
				PLL_CONFIG_20694_MAIN_ARRAY_SIZE))) == NULL) {
			PHY_ERROR(("%s: phy_malloc reg_field_mask_main failed\n", __FUNCTION__));
			goto fail;
		}

		if ((pll->reg_field_shift_main =
			phy_malloc(pi, (sizeof(uint8) *
				PLL_CONFIG_20694_MAIN_ARRAY_SIZE))) == NULL) {
			PHY_ERROR(("%s: phy_malloc reg_field_shift_main failed\n", __FUNCTION__));
			goto fail;
		}

		if ((pll->reg_field_val_main =
			phy_malloc(pi, (sizeof(uint16) *
				PLL_CONFIG_20694_MAIN_ARRAY_SIZE))) == NULL) {
			PHY_ERROR(("%s: phy_malloc reg_field_val_main failed\n", __FUNCTION__));
			goto fail;
		}
	}
	/* Register addresses */
	if (sicoreunit == DUALMAC_MAIN) {
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_CORE1_EN, RFP, PLL_CFG5,
			rfpll_vco_core1_en);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_CORE2_EN, RFP, PLL_CFG5,
			rfpll_vco_core2_en);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ENABLECOUPLING,
			RFP, PLL_VCOCAL24, rfpll_vcocal_enableCoupling);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLINGMODE,
			RFP, PLL_VCOCAL24, rfpll_vcocal_couplingMode);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_SWAPVCO12,
			RFP, PLL_VCOCAL24, rfpll_vcocal_swapVco12);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_MIDCODESEL,
			RFP, PLL_VCOCAL24, rfpll_vcocal_MidCodeSel);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_SECONDMESEN,
			RFP, PLL_VCOCAL24, rfpll_vcocal_SecondMesEn);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_UPDATESELCOUP,
			RFP, PLL_VCOCAL24, rfpll_vcocal_UpdateSelCoup);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLTHRES,
			RFP, PLL_VCOCAL26, rfpll_vcocal_CouplThres);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLTHRES2,
			RFP, PLL_VCOCAL26, rfpll_vcocal_CouplThres2);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_COUPLINGIN,
			RFP, PLL_VCOCAL25, rfpll_vcocal_couplingIn);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS2_OVR,
			RFP, PLL_VCOCAL21, rfpll_vcocal_force_caps2_ovr);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS2_OVRVAL,
			RFP, PLL_VCOCAL21, rfpll_vcocal_force_caps2_ovrVal);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX1_OVR,
			RFP, PLL_VCOCAL22, rfpll_vcocal_force_aux1_ovr);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX1_OVRVAL,
			RFP, PLL_VCOCAL22, rfpll_vcocal_force_aux1_ovrVal);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX2_OVR,
			RFP, PLL_VCOCAL23, rfpll_vcocal_force_aux2_ovr);
		PLL_CONFIG_20694_MAIN_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_AUX2_OVRVAL,
			RFP, PLL_VCOCAL23, rfpll_vcocal_force_aux2_ovrVal);
	}
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_SLOPEIN, RFP, PLL_VCOCAL_OVR1,
			ovr_rfpll_vcocal_slopeIn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_OFFSETIN, RFP, PLL_VCOCAL_OVR1,
			ovr_rfpll_vcocal_offsetIn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_RST_N, RFP, PLL_VCOCAL_OVR1,
			ovr_rfpll_vcocal_rst_n);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_RFPLL_VCOCAL_COUPLINGIN, RFP, PLL_VCOCAL_OVR1,
			ovr_rfpll_vcocal_couplingIn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, SEL_BAND, RFP, PLL_MUXSELECT_LINE,
			sel_band);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_MUX_XT_DEL, RFP, PLL_MUXSELECT_LINE,
			ovr_mux_xt_del);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, OVR_MUX_SEL_BAND, RFP, PLL_MUXSELECT_LINE,
			ovr_mux_sel_band);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYEND, RFP, PLL_VCOCAL18,
			rfpll_vcocal_DelayEnd);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYSTARTCOLD, RFP, PLL_VCOCAL3,
			rfpll_vcocal_DelayStartCold);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELAYSTARTWARM, RFP, PLL_VCOCAL4,
			rfpll_vcocal_DelayStartWarm);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_XTALCOUNT, RFP, PLL_VCOCAL7,
			rfpll_vcocal_XtalCount);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_CALCAPRBMODE, RFP, PLL_VCOCAL7,
			rfpll_vcocal_CalCapRBMode);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_DELTAPLLVAL, RFP, PLL_VCOCAL8,
			rfpll_vcocal_DeltaPllVal);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ERRORTHRES, RFP, PLL_VCOCAL20,
			rfpll_vcocal_ErrorThres);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_INITCAPA, RFP, PLL_VCOCAL12,
			rfpll_vcocal_InitCapA);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_INITCAPB, RFP, PLL_VCOCAL13,
			rfpll_vcocal_InitCapB);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_NORMCOUNTLEFT, RFP, PLL_VCOCAL10,
			rfpll_vcocal_NormCountLeft);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_NORMCOUNTRIGHT, RFP, PLL_VCOCAL11,
			rfpll_vcocal_NormCountRight);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_PAUSECNT, RFP, PLL_VCOCAL19,
			rfpll_vcocal_PauseCnt);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_ROUNDLSB, RFP, PLL_VCOCAL1,
			rfpll_vcocal_RoundLSB);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_UPDATESEL, RFP, PLL_VCOCAL1,
			rfpll_vcocal_UpdateSel);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TESTVCOCNT, RFP, PLL_VCOCAL1,
			rfpll_vcocal_testVcoCnt);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FAST_SETTLE_OVR, RFP, PLL_VCOCAL1,
			rfpll_vcocal_fast_settle_ovr);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FAST_SETTLE_OVRVAL, RFP, PLL_VCOCAL1,
			rfpll_vcocal_fast_settle_ovrVal);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_VCTRL_OVR, RFP, PLL_VCOCAL1,
			rfpll_vcocal_force_vctrl_ovr);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_VCTRL_OVRVAL, RFP, PLL_VCOCAL1,
			rfpll_vcocal_force_vctrl_ovrVal);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TARGETCOUNTBASE, RFP, PLL_VCOCAL6,
			rfpll_vcocal_TargetCountBase);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_TARGETCOUNTCENTER, RFP, PLL_VCOCAL9,
			rfpll_vcocal_TargetCountCenter);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_OFFSETIN, RFP, PLL_VCOCAL17,
			rfpll_vcocal_offsetIn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_SLOPEIN, RFP, PLL_VCOCAL15,
			rfpll_vcocal_slopeIn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FASTSWITCH, RFP, PLL_VCOCAL1,
			rfpll_vcocal_FastSwitch);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FIXMSB, RFP, PLL_VCOCAL1,
			rfpll_vcocal_FixMSB);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FOURTHMESEN, RFP, PLL_VCOCAL1,
			rfpll_vcocal_FourthMesEn);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_CAP_MODE, RFP, PLL_VCO2,
			rfpll_vco_cap_mode);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_PLL_VAL, RFP, PLL_VCOCAL5,
			rfpll_vcocal_pll_val);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS_OVR, RFP, PLL_VCOCAL2,
			rfpll_vcocal_force_caps_ovr);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCOCAL_FORCE_CAPS_OVRVAL, RFP, PLL_VCOCAL2,
			rfpll_vcocal_force_caps_ovrVal);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_FRCT_WILD_BASE_HIGH, RFP, PLL_FRCT2,
			rfpll_frct_wild_base_high);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_FRCT_WILD_BASE_LOW, RFP, PLL_FRCT3,
			rfpll_frct_wild_base_low);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_CVAR, RFP, PLL_VCO2,
			rfpll_vco_cvar);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_ALC_REF_CTRL, RFP, PLL_VCO6,
			rfpll_vco_ALC_ref_ctrl);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_BIAS_MODE, RFP, PLL_VCO6,
			rfpll_vco_bias_mode);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_TEMPCO_DCADJ, RFP, PLL_VCO5,
			rfpll_vco_tempco_dcadj);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_VCO_TEMPCO, RFP, PLL_VCO4,
			rfpll_vco_tempco);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_R2, RFP, PLL_LF4, rfpll_lf_lf_r2);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_R3, RFP, PLL_LF5, rfpll_lf_lf_r3);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_RS_CM, RFP,
		PLL_LF7, rfpll_lf_lf_rs_cm);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_RF_CM, RFP,
		PLL_LF7, rfpll_lf_lf_rf_cm);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C1, RFP,
		PLL_LF2, rfpll_lf_lf_c1);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C2, RFP,
		PLL_LF2, rfpll_lf_lf_c2);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C3, RFP,
		PLL_LF3, rfpll_lf_lf_c3);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_LF_LF_C4, RFP,
		PLL_LF3, rfpll_lf_lf_c4);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_CP_IOFF, RFP,
		PLL_CP4, rfpll_cp_ioff);
	PLL_CONFIG_20694_REG_INFO_ENTRY(pi, pll, RFPLL_CP_KPD_SCALE, RFP,
		PLL_CP4, rfpll_cp_kpd_scale);

	return BCME_OK;

fail:
	if (pll->reg_addr != NULL) {
		phy_mfree(pi, pll->reg_addr, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
	}

	if (pll->reg_field_mask != NULL) {
		phy_mfree(pi, pll->reg_field_mask,
				(sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
	}

	if (pll->reg_field_shift != NULL) {
		phy_mfree(pi, pll->reg_field_shift,
				(sizeof(uint8) * PLL_CONFIG_20694_ARRAY_SIZE));
	}

	if (pll->reg_field_val != NULL) {
		phy_mfree(pi, pll->reg_field_val, (sizeof(uint16) * PLL_CONFIG_20694_ARRAY_SIZE));
	}

	if (sicoreunit == DUALMAC_MAIN) {
		if (pll->reg_addr_main != NULL) {
			phy_mfree(pi, pll->reg_addr_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
		}

		if (pll->reg_field_mask_main != NULL) {
			phy_mfree(pi, pll->reg_field_mask_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
		}

		if (pll->reg_field_shift_main != NULL) {
			phy_mfree(pi, pll->reg_field_shift_main,
				(sizeof(uint8) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
		}

		if (pll->reg_field_val_main != NULL) {
			phy_mfree(pi, pll->reg_field_val_main,
				(sizeof(uint16) * PLL_CONFIG_20694_MAIN_ARRAY_SIZE));
		}
	}
	return BCME_NOMEM;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
phy_ac_radio20695_write_pll_config(phy_info_t *pi, radio_pll_sel_t pll_sel,
		pll_config_20695_tbl_t *pll)
{
	uint16 *reg_val;
	uint16 *reg_addr;
	uint16 *reg_mask;
	uint8 *reg_shift;
	uint8 i;
	const chan_info_radio20695_rffe_t *chan_info = NULL;

	BCM_REFERENCE(chan_info);

	/* Get channel info */
	wlc_phy_chan2freq_20695(pi, CHSPEC_CHANNEL(pi->radio_chanspec), &chan_info);

	reg_val = pll->reg_field_val;

	if (pll_sel == PLL_2G) {
		reg_addr = pll->reg_addr_2g;
		reg_mask = pll->reg_field_mask_2g;
		reg_shift = pll->reg_field_shift_2g;
	} else {
		reg_addr = pll->reg_addr_5g;
		reg_mask = pll->reg_field_mask_5g;
		reg_shift = pll->reg_field_shift_5g;
	}

	/* Write to register fields */
	for (i = 0; i < PLL_CONFIG_20695_ARRAY_SIZE; i++) {
		phy_utils_mod_radioreg(pi, reg_addr[i], reg_mask[i], (reg_val[i] << reg_shift[i]));
	}

	/* PLL Config extras */
	phy_ac_radio20695_extra_pll_config_write(pi, chan_info->freq, pll_sel);

	/* Reset SD modulator after setting new channel parameters */
	if (pll_sel == PLL_2G) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_OVR1, 0, ovr_rfpll_rst_n, 1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_CFG2, 0, rfpll_rst_n, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_CFG2, 0, rfpll_rst_n, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_OVR1, 0, ovr_rfpll_5g_rst_n, 1)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_CFG2, 0, rfpll_5g_rst_n, 0)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_CFG2, 0, rfpll_5g_rst_n, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}

static void
BCMRAMFN(phy_ac_radio20695_extra_pll_config_write)(phy_info_t *pi, uint16 chan_freq,
		radio_pll_sel_t pll_sel)
{
	/* WAR's (overwrite registers) */
	if (BOARDFLAGS(GENERIC_PHY_INFO(pi)->boardtype) == BCM943012FCREF_SSID) {
		/* 43012A0 FCBGA package */
		if (chan_freq == 2412) {
			PHY_INFORM(("%s : Ch1 600kHz Spur WAR: kvco = 40, lbw = 200\n",
					__FUNCTION__));
			ACPHY_REG_LIST_START
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF2, 0, rfpll_lf_lf_c1,
						RFPLL_LF_LF_C1_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF2, 0, rfpll_lf_lf_c2,
						RFPLL_LF_LF_C2_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF3, 0, rfpll_lf_lf_c3,
						RFPLL_LF_LF_C3_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF3, 0, rfpll_lf_lf_c4,
						RFPLL_LF_LF_C4_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF4, 0, rfpll_lf_lf_r2,
						RFPLL_LF_LF_R2_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF5, 0, rfpll_lf_lf_r3,
						RFPLL_LF_LF_R3_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF7, 0, rfpll_lf_lf_rf_cm,
						RFPLL_LF_LF_RF_CM_CH2412)
				MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL_LF7, 0, rfpll_lf_lf_rs_cm,
						RFPLL_LF_LF_RS_CM_CH2412)
			ACPHY_REG_LIST_EXECUTE(pi);
		}
	}

	if (pll_sel == PLL_2G) {
		/* 2G PLL related configs */
	} else {
		/* 5G PLL related configs */
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_RFVCO1, 0,
				rfpll_5g_rfvco_TEMP_CODE, 0xf)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_RFVCO1, 0,
				rfpll_5g_rfvco_KVCO_CODE, 0xb)
			MOD_RADIO_REG_28NM_ENTRY(pi, RFP, PLL5G_LF1, 0, rfpll_5g_lf_extvcbin, 0x4)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
phy_ac_radio20694_write_pll_config(phy_info_t *pi, pll_config_20694_tbl_t *pll)
{
	uint16 *reg_val;
	uint16 *reg_addr;
	uint16 *reg_mask;
	uint8 *reg_shift;
	uint16 *reg_val_main;
	uint16 *reg_addr_main;
	uint16 *reg_mask_main;
	uint8 *reg_shift_main;
	uint8 i;
	uint sicoreunit;

	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	reg_val = pll->reg_field_val;
	reg_addr = pll->reg_addr;
	reg_mask = pll->reg_field_mask;
	reg_shift = pll->reg_field_shift;
	reg_val_main = pll->reg_field_val_main;
	reg_addr_main = pll->reg_addr_main;
	reg_mask_main = pll->reg_field_mask_main;
	reg_shift_main = pll->reg_field_shift_main;

	/* Write to register fields */
	for (i = 0; i < PLL_CONFIG_20694_ARRAY_SIZE; i++) {
	  phy_utils_mod_radioreg(pi, reg_addr[i], reg_mask[i], (reg_val[i] << reg_shift[i]));
	}
	if (sicoreunit == DUALMAC_MAIN) {
		for (i = 0; i < PLL_CONFIG_20694_MAIN_ARRAY_SIZE; i++) {
			phy_utils_mod_radioreg(pi, reg_addr_main[i], reg_mask_main[i],
			(reg_val_main[i] << reg_shift_main[i]));
		}
	}
}

/* 20694 radio related functions */
int
wlc_phy_chan2freq_20694(phy_info_t *pi, uint8 channel,
	const chan_info_radio20694_rffe_t **chan_info)
{
	uint32 index, tbl_len;
	const chan_info_radio20694_rffe_t *p_chan_info_tbl;
	int status;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID) &&
		(ACMAJORREV_25(pi->pubpi->phy_rev) || ACMAJORREV_40(pi->pubpi->phy_rev)));

	if (!RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		PHY_ERROR(("wl%d: %s: Unsupported radio %d\n",
			pi->sh->unit, __FUNCTION__, RADIOID(pi->pubpi->radioid)));
		ASSERT(FALSE);
		return -1;
	}
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));
	status = wlc_phy_ac_get_20694_chan_info_tbl(pi, &p_chan_info_tbl, &tbl_len);

	if (!status) {
		for (index = 0; index < tbl_len && p_chan_info_tbl[index].channel != channel;
		     index++);

		if (index >= tbl_len) {
			PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
				pi->sh->unit, __FUNCTION__, channel));
			if (!ISSIM_ENAB(pi->sh->sih)) {
				/* Do not assert on QT since we leave the tables empty on purpose */
				ASSERT(index < tbl_len);
			}
			return -1;
		}
		*chan_info = p_chan_info_tbl + index;
		return p_chan_info_tbl[index].freq;
	} else {
		return -1;
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
int
wlc_phy_chan2freq_20696(phy_info_t *pi, uint8 channel,
	const chan_info_radio20696_rffe_t **chan_info)
{
	uint32 index, tbl_len;
	const chan_info_radio20696_rffe_t *p_chan_info_tbl;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20696_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));
	p_chan_info_tbl = chan_tune_20696_rev0;
	tbl_len = chan_tune_20696_rev0_length;
	for (index = 0; index < tbl_len && p_chan_info_tbl[index].channel != channel;
	     index++);

	if (index >= tbl_len) {
		PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
			pi->sh->unit, __FUNCTION__, channel));
		if (!ISSIM_ENAB(pi->sh->sih)) {
			/* Do not assert on QT since we leave the tables empty on purpose */
			ASSERT(index < tbl_len);
		}
		return -1;
	}
	*chan_info = p_chan_info_tbl + index;
	return p_chan_info_tbl[index].freq;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_radio20694_upd_prfd_values(phy_info_t *pi)
{
	radio_20xx_prefregs_t *prefregs_20694_ptr = NULL;
	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));
	if ((prefregs_20694_ptr = wlc_phy_ac_get_20694_prfd_vals_tbl(pi)) == NULL) {
		PHY_ERROR(("wl%d: %s: Preferred value table not known for radio_rev %d\n",
				pi->sh->unit, __FUNCTION__,
				RADIOREV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return;
	}
	/* Update preferred values */
	wlc_phy_init_radio_prefregs_allbands(pi, prefregs_20694_ptr);

	if (sicoreunit == DUALMAC_AUX) {
		/* 4347B0 doesn't need invclko enabled in 2G */
		if (RADIOREV(pi->pubpi->radiorev) < 8) {
			MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, 0, iqdac_invclko,
				CHSPEC_IS2G(pi->radio_chanspec));
		}
		/* Initialize shared power down PMU LDOs of MAIN slice to be 0 for those LDOs
		 * not needed by the AUX slice
		 */
		MOD_RADIO_REG_20694(pi, RF, PMU_REG1_MAINAUX_SHRD, 0, shrd_wlpmu_AFEldo_pu, 0);
		MOD_RADIO_REG_20694(pi, RF, PMU_REG1_MAINAUX_SHRD, 0, shrd_wlpmu_TXldo_pu, 0);
		MOD_RADIO_REG_20694(pi, RF, PMU_REG1_MAINAUX_SHRD, 0, shrd_wlpmu_ADCldo_pu, 0);
		MOD_RADIO_REG_20694(pi, RF, PMU_REG2_MAINAUX_SHRD, 0, shrd_wlpmu_LDO2P2_pu, 0);
	}

}

static void
wlc_phy_radio20694_rffe_tune(phy_info_t *pi, const chan_info_radio20694_rffe_t *chan_info_rffe,
	uint8 core)
{

	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	if (chan_info_rffe->freq > 4000) {
	/* 5G front end */
	    if (core == 0) {
			MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG5, core, logen5g_x3_ctune,
				chan_info_rffe->RF0_logen5g_reg5_logen5g_x3_ctune_5G);
			MOD_RADIO_REG_20694(pi, RF, RX5G_REG1, core, rx5g_lna_tune,
				chan_info_rffe->RF0_rx5g_reg1_rx5g_lna_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, TXMIX5G_CFG6, core, mx5g_tune,
				chan_info_rffe->RF0_txmix5g_cfg6_mx5g_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, PA5G_CFG4, core, pad5g_tune,
				chan_info_rffe->RF0_pa5g_cfg4_pad5g_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, PA5G_CFG4, core, pa5g_tune,
				chan_info_rffe->RF0_pa5g_cfg4_pa5g_tune_5G);

	        if (sicoreunit == DUALMAC_AUX) {
				MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG6, core,
				logen5g_buf_ctune_c0,
				chan_info_rffe->RF0_logen5g_reg6_logen5g_buf_ctune_c0_5G_AUX);
				MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG6, core,
				logen5g_buf_ctune_c1,
				chan_info_rffe->RF0_logen5g_reg6_logen5g_buf_ctune_c1_5G_AUX);
	        } else {
				MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG8, core, logen5g_buf_ctune,
				chan_info_rffe->RF0_logen5g_reg8_logen5g_buf_ctune_5G);
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG5, core,
				logen5g_mimobuf_ctune,
				chan_info_rffe->RF0_logen2g_reg5_logen5g_mimobuf_ctune_5G);
	        }
	    } else if (core == 1) {
			MOD_RADIO_REG_20694(pi, RF, RX5G_REG1, core, rx5g_lna_tune,
				chan_info_rffe->RF1_rx5g_reg1_rx5g_lna_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, TXMIX5G_CFG6, core, mx5g_tune,
				chan_info_rffe->RF1_txmix5g_cfg6_mx5g_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, PA5G_CFG4, core, pad5g_tune,
				chan_info_rffe->RF1_pa5g_cfg4_pad5g_tune_5G);
			MOD_RADIO_REG_20694(pi, RF, PA5G_CFG4, core, pa5g_tune,
				chan_info_rffe->RF1_pa5g_cfg4_pa5g_tune_5G);
			if (sicoreunit == DUALMAC_MAIN) {
				MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG8, core, logen5g_buf_ctune,
				chan_info_rffe->RF1_logen5g_reg8_logen5g_buf_ctune_5G);
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG5,
					core, logen5g_mimobuf_ctune,
					chan_info_rffe->RF1_logen2g_reg5_logen5g_mimobuf_ctune_5G);
				MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG5, core, logen5g_x3_ctune,
					chan_info_rffe->RF1_logen5g_reg5_logen5g_x3_ctune_5G);
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG5,
					core, logen5g_mimobuf_ctune,
					chan_info_rffe->RF1_logen2g_reg5_logen5g_mimobuf_ctune_5G);
			}
		}
	} else {
	    if (core == 0) {
			MOD_RADIO_REG_20694(pi, RF, LNA2G_REG1, core, lna2g_lna1_freq_tune,
				chan_info_rffe->RF0_lna2g_reg1_lna2g_lna1_freq_tune_2G);
			MOD_RADIO_REG_20694(pi, RF, TXMIX2G_CFG5, core, mx2g_tune,
				chan_info_rffe->RF0_txmix2g_cfg5_mx2g_tune_2G);
			MOD_RADIO_REG_20694(pi, RF, PA2G_CFG2, core, pad2g_tune,
				chan_info_rffe->RF0_pa2g_cfg2_pad2g_tune_2G);
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, core, logen2g_x2_ctune,
				chan_info_rffe->RF0_logen2g_reg2_logen2g_x2_ctune_2G);
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, core, logen2g_buf_ctune,
				chan_info_rffe->RF0_logen2g_reg2_logen2g_buf_ctune_2G);

			if (sicoreunit == DUALMAC_AUX) {
			MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG4, core, logen2g_lobuf2g2_ctune,
				chan_info_rffe->RF0_logen2g_reg4_logen2g_lobuf2g2_ctune_2G_AUX);
			}

	    } else if (core == 1) {
			MOD_RADIO_REG_20694(pi, RF, LNA2G_REG1, core, lna2g_lna1_freq_tune,
				chan_info_rffe->RF1_lna2g_reg1_lna2g_lna1_freq_tune_2G);
			MOD_RADIO_REG_20694(pi, RF, TXMIX2G_CFG5, core, mx2g_tune,
				chan_info_rffe->RF1_txmix2g_cfg5_mx2g_tune_2G);
			MOD_RADIO_REG_20694(pi, RF, PA2G_CFG2, core, pad2g_tune,
				chan_info_rffe->RF1_pa2g_cfg2_pad2g_tune_2G);
			if (sicoreunit == DUALMAC_MAIN) {
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, core, logen2g_buf_ctune,
					chan_info_rffe->RF1_logen2g_reg2_logen2g_buf_ctune_2G);
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, core, logen2g_x2_ctune,
					chan_info_rffe->RF1_logen2g_reg2_logen2g_x2_ctune_2G);
			}
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void wlc_phy_radio20696_rffe_tune(phy_info_t *pi,
	const chan_info_radio20696_rffe_t *chan_info_rffe)
{
	if (CHSPEC_IS5G(pi->radio_chanspec)) {
		/* 5G front end */
		MOD_RADIO_PLLREG_20696(pi, LOGEN_REG1, logen_mix_ctune,
				chan_info_rffe->u.val_5G.RFP0_logen_reg1_logen_mix_ctune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 0, logen_lc_ctune,
				chan_info_rffe->u.val_5G.RF0_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, RXADC_REG2, 0, rxadc_ti_ctune_Q,
				chan_info_rffe->u.val_5G.RF0_rxadc_reg2_rxadc_ti_ctune_Q);
		MOD_RADIO_REG_20696(pi, RXADC_REG2, 0, rxadc_ti_ctune_I,
				chan_info_rffe->u.val_5G.RF0_rxadc_reg2_rxadc_ti_ctune_I);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG2, 0, mx5g_tune,
				chan_info_rffe->u.val_5G.RF0_tx5g_mix_reg2_mx5g_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, 0, tx5g_pa_tune,
				chan_info_rffe->u.val_5G.RF0_tx5g_pa_reg4_tx5g_pa_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, 0, pad5g_tune,
				chan_info_rffe->u.val_5G.RF0_tx5g_pad_reg3_pad5g_tune);
		MOD_RADIO_REG_20696(pi, RX5G_REG1, 0, rx5g_lna_tune,
				chan_info_rffe->u.val_5G.RF0_rx5g_reg1_rx5g_lna_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 1, logen_lc_ctune,
				chan_info_rffe->u.val_5G.RF1_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG2, 1, mx5g_tune,
				chan_info_rffe->u.val_5G.RF1_tx5g_mix_reg2_mx5g_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, 1, tx5g_pa_tune,
				chan_info_rffe->u.val_5G.RF1_tx5g_pa_reg4_tx5g_pa_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, 1, pad5g_tune,
				chan_info_rffe->u.val_5G.RF1_tx5g_pad_reg3_pad5g_tune);
		MOD_RADIO_REG_20696(pi, RX5G_REG1, 1, rx5g_lna_tune,
				chan_info_rffe->u.val_5G.RF1_rx5g_reg1_rx5g_lna_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 2, logen_lc_ctune,
				chan_info_rffe->u.val_5G.RF2_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG2, 2, mx5g_tune,
				chan_info_rffe->u.val_5G.RF2_tx5g_mix_reg2_mx5g_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, 2, tx5g_pa_tune,
				chan_info_rffe->u.val_5G.RF2_tx5g_pa_reg4_tx5g_pa_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, 2, pad5g_tune,
				chan_info_rffe->u.val_5G.RF2_tx5g_pad_reg3_pad5g_tune);
		MOD_RADIO_REG_20696(pi, RX5G_REG1, 2, rx5g_lna_tune,
				chan_info_rffe->u.val_5G.RF2_rx5g_reg1_rx5g_lna_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 3, logen_lc_ctune,
				chan_info_rffe->u.val_5G.RF3_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX5G_MIX_REG2, 3, mx5g_tune,
				chan_info_rffe->u.val_5G.RF3_tx5g_mix_reg2_mx5g_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PA_REG4, 3, tx5g_pa_tune,
				chan_info_rffe->u.val_5G.RF3_tx5g_pa_reg4_tx5g_pa_tune);
		MOD_RADIO_REG_20696(pi, TX5G_PAD_REG3, 3, pad5g_tune,
				chan_info_rffe->u.val_5G.RF3_tx5g_pad_reg3_pad5g_tune);
		MOD_RADIO_REG_20696(pi, RX5G_REG1, 3, rx5g_lna_tune,
				chan_info_rffe->u.val_5G.RF3_rx5g_reg1_rx5g_lna_tune);
	} else {
		/* 2G front end */
		MOD_RADIO_PLLREG_20696(pi, LOGEN_REG1, logen_mix_ctune,
				chan_info_rffe->u.val_2G.RFP0_logen_reg1_logen_mix_ctune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 0, logen_lc_ctune,
				chan_info_rffe->u.val_2G.RF0_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, RXADC_REG2, 0, rxadc_ti_ctune_Q,
				chan_info_rffe->u.val_2G.RF0_rxadc_reg2_rxadc_ti_ctune_Q);
		MOD_RADIO_REG_20696(pi, RXADC_REG2, 0, rxadc_ti_ctune_I,
				chan_info_rffe->u.val_2G.RF0_rxadc_reg2_rxadc_ti_ctune_I);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG4, 0, mx2g_tune,
				chan_info_rffe->u.val_2G.RF0_tx2g_mix_reg4_mx2g_tune);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG3, 0, pad2g_tune,
				chan_info_rffe->u.val_2G.RF0_tx2g_pad_reg3_pad2g_tune);
		MOD_RADIO_REG_20696(pi, LNA2G_REG1, 0, lna2g_lna1_freq_tune,
				chan_info_rffe->u.val_2G.RF0_lna2g_reg1_lna2g_lna1_freq_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 1, logen_lc_ctune,
				chan_info_rffe->u.val_2G.RF1_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG4, 1, mx2g_tune,
				chan_info_rffe->u.val_2G.RF1_tx2g_mix_reg4_mx2g_tune);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG3, 1, pad2g_tune,
				chan_info_rffe->u.val_2G.RF1_tx2g_pad_reg3_pad2g_tune);
		MOD_RADIO_REG_20696(pi, LNA2G_REG1, 1, lna2g_lna1_freq_tune,
				chan_info_rffe->u.val_2G.RF1_lna2g_reg1_lna2g_lna1_freq_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 2, logen_lc_ctune,
				chan_info_rffe->u.val_2G.RF2_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG4, 2, mx2g_tune,
				chan_info_rffe->u.val_2G.RF2_tx2g_mix_reg4_mx2g_tune);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG3, 2, pad2g_tune,
				chan_info_rffe->u.val_2G.RF2_tx2g_pad_reg3_pad2g_tune);
		MOD_RADIO_REG_20696(pi, LNA2G_REG1, 2, lna2g_lna1_freq_tune,
				chan_info_rffe->u.val_2G.RF2_lna2g_reg1_lna2g_lna1_freq_tune);
		MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG3, 3, logen_lc_ctune,
				chan_info_rffe->u.val_2G.RF3_logen_core_reg3_logen_lc_ctune);
		MOD_RADIO_REG_20696(pi, TX2G_MIX_REG4, 3, mx2g_tune,
				chan_info_rffe->u.val_2G.RF3_tx2g_mix_reg4_mx2g_tune);
		MOD_RADIO_REG_20696(pi, TX2G_PAD_REG3, 3, pad2g_tune,
				chan_info_rffe->u.val_2G.RF3_tx2g_pad_reg3_pad2g_tune);
		MOD_RADIO_REG_20696(pi, LNA2G_REG1, 3, lna2g_lna1_freq_tune,
				chan_info_rffe->u.val_2G.RF3_lna2g_reg1_lna2g_lna1_freq_tune);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_radio20694_upd_band_related_reg(phy_info_t *pi)
{
	uint16 fc;
	uint8 core;
	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	/* Get center frequency of the channel */
	if (CHSPEC_CHANNEL(pi->radio_chanspec) > 14) {
	  fc = CHAN5G_FREQ(CHSPEC_CHANNEL(pi->radio_chanspec));
	} else {
	  fc = CHAN2G_FREQ(CHSPEC_CHANNEL(pi->radio_chanspec));
	}
	FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20694(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_hpm, 1);
			WRITE_RADIO_REG_20694(pi, RF, READOVERRIDES, core, 0);
	}

	MOD_RADIO_REG_20694(pi, RFP, XTAL1, 0, xtal_core_buf_ds, 0x8);

	MOD_RADIO_REG_20694(pi, RFP, PLL_VCO9, 0, rfpll_vco_cap_bias_buf_ib, 0x3);
	MOD_RADIO_REG_20694(pi, RFP, PLL_HVLDO2, 0, ldo_1p8_lowquiescenten_CP, 0x1);
	MOD_RADIO_REG_20694(pi, RFP, PLL_HVLDO2, 0, ldo_1p8_lowquiescenten_VCO, 0x1);
	MOD_RADIO_REG_20694(pi, RFP, PLL_LVLDO1, 0, ldo_1p0_lowquiescenten_PFDMMD, 0x1);
	MOD_RADIO_REG_20694(pi, RFP, PLL_CP5, 0, rfpll_cp_ioff_bias_n, 0x3);
	MOD_RADIO_REG_20694(pi, RFP, BG_REG8, 0, bg_ate_rcal_trim, 0x66);
	if (sicoreunit == DUALMAC_MAIN) {
	  if (fc < 3000) {
	    /* 2G front end */
	    FOREACH_CORE(pi, core) {
	      MOD_RADIO_REG_20694(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_hpm, 0);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc, 0xa);
	      MOD_RADIO_REG_20694(pi, RF, TX2G_CFG1_OVR, core, ovr_bst2g_pu, 1);
	      MOD_RADIO_REG_20694(pi, RF, TXMIX2G_CFG5, core, bst2g_pu, 0);
	      MOD_RADIO_REG_20694(pi, RF, TX_CFG2_OVR, core, ovr_bstr_bias_pu, 1);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG1, core, bstr_bias_pu, 0);
	      MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG6, core, ibias_uncal_20uA_LDO_2P5V_mag, 0xb);
	      MOD_RADIO_REG_20694(pi, RF, PA2G_CFG5, core, pa2g_stby_ldo_adj, 0x3);
	      MOD_RADIO_REG_20694(pi, RF, PA2G_CFG1, core, pa2g_tssi_ctrl_sel, 0x0);

	      //MOD_RADIO_REG_20694(pi, RF, TX_CFG1_OVR, core, ovr_pa_idac_main, 0x1);
	      if (RADIOREV(pi->pubpi->radiorev) >= 8) {
	          MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core, pa_idac_main, 43);
	      } else {
	          MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core, pa_idac_main, 35);
	      }
	      MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG5, core, idac_pa_casref, 8);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_cascode, 14);
	      if (RADIOREV(pi->pubpi->radiorev) >= 8) {
	          /* below for 2G EVM improvement */
	          MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG4, core, pa_ptat_slope_main, 10);
	          MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc_slope, 8);
	          MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc, 14);
	      }
	      MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, core, logen2g_x2_bias, 3);
	    }
	  } else {
	    /* 5G front end */
	    FOREACH_CORE(pi, core) {
	      MOD_RADIO_REG_20694(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_hpm, 1);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc, 0xa);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG4, core, mx_idac_lobuf, 0x7);
	      MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_cascode, 24);

	      MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core, ovr_pad5g_idac_gm, 1);
	      if (RADIOREV(pi->pubpi->radiorev) >= 8) {
			MOD_RADIO_REG_20694(pi, RF, TXMIX5G_CFG8, core, pad5g_idac_gm, 15);
			MOD_RADIO_REG_20694(pi, RF, TX5G_CFG3, core, pad5g_idac_cas_bot, 10);
	      } else {
			MOD_RADIO_REG_20694(pi, RF, TXMIX5G_CFG8, core, pad5g_idac_gm, 31);
			MOD_RADIO_REG_20694(pi, RF, TX5G_CFG3, core, pad5g_idac_cas_bot, 18);
	      }
	      MOD_RADIO_REG_20694(pi, RF, TX5G_CFG2, core, pad5g_ptat_slope_gm, 7);

	      MOD_RADIO_REG_20694(pi, RF, TX_CFG1_OVR, core, ovr_pa_idac_main, 1);
	      if (RADIOREV(pi->pubpi->radiorev) >= 8) {
		      MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core, pa_idac_main, 23);
	      } else {
		      MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core, pa_idac_main, 60);
	      }
	      MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG5, core, idac_pa_casref, 12);
	      MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core, ovr_bst5g_pu, 1);
	      MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1, core, bst5g_pu, 0);
	      MOD_RADIO_REG_20694(pi, RF, PA5G_CFG2, core, pa5g_idac_topc_op1, 0);
	      MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core, ovr_trsw5g_pu, 1);
	      MOD_RADIO_REG_20694(pi, RF, TRSW5G_CFG1, core, trsw5g_pu, 0);

	      MOD_RADIO_REG_20694(pi, RF, LOGEN5G_REG8, core, logen5g_buf_bias, 0x7);
	      if (core == 1) {
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, 1, afediv_bias_str, 0x1);
	      }
	    }
	  }
	} else if (sicoreunit == DUALMAC_AUX) {
		if (fc < 3000) {
			if (core != 1) {
				MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, 0, logen2g_x2_bias, 3);
			}
			if (!(PHY_IPA(pi))) {
				FOREACH_CORE(pi, core) {
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc, 0xa);
				MOD_RADIO_REG_20694(pi, RF, TX2G_CFG1_OVR, core, ovr_bst2g_pu, 1);
				MOD_RADIO_REG_20694(pi, RF, TXMIX2G_CFG5, core, bst2g_pu, 0);
				MOD_RADIO_REG_20694(pi, RF, TX_CFG2_OVR, core, ovr_bstr_bias_pu, 1);
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG1, core, bstr_bias_pu, 0);
				MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG6, core,
					ibias_uncal_20uA_LDO_2P5V_mag, 0xb);
				MOD_RADIO_REG_20694(pi, RF, PA2G_CFG5, core,
					pa2g_stby_ldo_adj, 0x3);
				MOD_RADIO_REG_20694(pi, RF, PA2G_CFG1, core,
					pa2g_tssi_ctrl_sel, 0x0);

				//MOD_RADIO_REG_20694(pi, RF, TX_CFG1_OVR, core,
				//	ovr_pa_idac_main, 0x1);
				if (RADIOREV(pi->pubpi->radiorev) >= 8) {
					MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core,
						pa_idac_main, 43);
				} else {
					MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core,
						pa_idac_main, 35);
				}
				MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG5, core, idac_pa_casref, 8);
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_cascode, 14);
				if (RADIOREV(pi->pubpi->radiorev) >= 8) {
					MOD_RADIO_REG_20694(pi, RF, PMU_CFG1, core,
						wlpmu_LOGENldo_hpm, 0);
					/* below for 2G EVM improvement */
					MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG4, core,
						pa_ptat_slope_main, 10);
					MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core,
						mx_idac_bbdc_slope, 8);
					MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core,
						mx_idac_bbdc, 14);
					/* Rx: Cgs size in 2G-Main and Aux 4347B0 needs to be 0x0 */
					MOD_RADIO_REG_20694(pi, RF, LNA2G_REG2, core,
						lna2g_lna1_Cgs_size, 0);
					/* logen 2G: hack to lower bias which improves PN and EVM */
					if (core != 1) {
						MOD_RADIO_REG_20694(pi, RF, LOGEN2G_REG2, 0,
							logen2g_x2_bias, 3);
					}
				}
			    }
			} else if ((PHY_IPA(pi)) && (RADIOREV(pi->pubpi->radiorev) == 4)) {
			/* placeholder */
			}
		}  else {
		    /* 5G */
		    FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20694(pi, RF, PMU_CFG1, core, wlpmu_LOGENldo_hpm, 1);
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_bbdc, 0xa);
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG4, core, mx_idac_lobuf, 0x7);
				MOD_RADIO_REG_20694(pi, RF, TX_MX_CFG3, core, mx_idac_cascode, 24);

				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core,
					ovr_pad5g_idac_gm, 1);
				MOD_RADIO_REG_20694(pi, RF, TXMIX5G_CFG8, core, pad5g_idac_gm, 31);
				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG3, core,
					pad5g_idac_cas_bot, 18);
				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG2, core,
					pad5g_ptat_slope_gm, 7);

				MOD_RADIO_REG_20694(pi, RF, TX_CFG1_OVR, core, ovr_pa_idac_main, 1);
				MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG2, core, pa_idac_main, 60);
				MOD_RADIO_REG_20694(pi, RF, TX_PA_CFG5, core, idac_pa_casref, 12);
				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core, ovr_bst5g_pu, 1);
				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1, core, bst5g_pu, 0);
				MOD_RADIO_REG_20694(pi, RF, PA5G_CFG2, core, pa5g_idac_topc_op1, 0);
				MOD_RADIO_REG_20694(pi, RF, TX5G_CFG1_OVR, core, ovr_trsw5g_pu, 1);
				MOD_RADIO_REG_20694(pi, RF, TRSW5G_CFG1, core, trsw5g_pu, 0);
			}
		}
	}
	FOREACH_CORE(pi, core) {
		WRITE_RADIO_REG_20694(pi, RF, READOVERRIDES, core, 1);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
wlc_phy_radio20696_upd_band_related_reg(phy_info_t *pi)
{
	uint8 core;
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		MOD_RADIO_PLLREG_20696(pi, LOGEN_REG0, logen_div3en, 1);
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core, logen_div2_pu, 0x1);
			// new 2G optimised settings
			MOD_RADIO_REG_20696(pi, LPF_OVR1, core, ovr_lpf_g_passive_rc, 1);
			MOD_RADIO_REG_20696(pi, LPF_REG5, core, lpf_g_passive_rc, 700);
			MOD_RADIO_REG_20696(pi, TXDAC_REG1, core, iqdac_buf_Cfb, 127);
			MOD_RADIO_REG_20696(pi, RX2G_REG2, core, rx2g_gm_ptat, 7);
			MOD_RADIO_REG_20696(pi, LNA2G_REG2, core, lna2g_lna1_bias_ptat, 7);
		}
	} else {
		MOD_RADIO_PLLREG_20696(pi, LOGEN_REG0, logen_div3en, 0);
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core, logen_div2_pu, 0x0);
			// new 5G optimised settings
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG4, core, logen_tx_rccr_tune, 0);
			MOD_RADIO_REG_20696(pi, TXDAC_REG1, core, iqdac_buf_Cfb, 127);
			MOD_RADIO_REG_20696(pi, TXDAC_REG1, core, iqdac_Rfb, 0);
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core,
			    logen_5g_tx_rccr_iqbias_short, 1);
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG2, core, logen_tx_rccr_idac26u, 7);
			// RX settings RCCR related 5G
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG4, core, logen_rx_rccr_tune, 0);
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG2, core, logen_rx_rccr_idac26u, 7);
			MOD_RADIO_REG_20696(pi, LOGEN_CORE_REG0, core,
			    logen_5g_rx_rccr_iqbias_short, 1);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_switch_radio_acphy_20694(phy_info_t *pi)
{
	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	/* RCAL */
	wlc_phy_20694_radio_rcal(pi, 1);
	if (sicoreunit == DUALMAC_AUX) {
		wlc_phy_20694_radio_rcal_byaux(pi, 1);
	}

	wlc_phy_20694_radio_minipmu_cal(pi);
	if (sicoreunit == DUALMAC_AUX) {
		wlc_phy_20694_radio_minipmu_cal_byaux(pi);
	}
}


static void
chanspec_tune_radio_20694(phy_info_t *pi)
{
	/* TODO: Add 20694 specific code to be added */
	uint8 core, dacbw;
	uint16 bbmult[2], bbmult_zero = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
		pi->dacratemode2g : pi->dacratemode5g;

	/* Configure ulp tx mode */
	//ulp dac mode is not supported for 4347 yet.
	//wlc_phy_dac_rate_mode_acphy(pi, pi_ac->dac_mode);

	/* adc_cap_cal */
	if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac) || CCT_BAND_CHG(pi_ac)) {
		FOREACH_CORE(pi, core) {
			wlc_phy_get_tx_bbmult_acphy(pi, &bbmult[core], core);
			wlc_phy_set_tx_bbmult_acphy(pi, &bbmult_zero, core);
		}
		wlc_phy_radio_afecal(pi);

		FOREACH_CORE(pi, core) {
			wlc_phy_set_tx_bbmult_acphy(pi, &bbmult[core], core);
		}
	}

	/* Configure DAC buffer BW */
	if (PHY_PAPDEN(pi)) {
		wlc_phy_radio20694_txdac_bw_setup(pi, BUTTERWORTH, DACBW_120);
	} else {
		dacbw = CHSPEC_IS20(pi->radio_chanspec) ? DACBW_30:
			CHSPEC_IS40(pi->radio_chanspec) ? DACBW_60:
			CHSPEC_IS80(pi->radio_chanspec) ? DACBW_120: DACBW_150;
		wlc_phy_radio20694_txdac_bw_setup(pi, 0, dacbw);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
chanspec_tune_radio_20696(phy_info_t *pi)
{
	uint8 core, dacbw;
	uint16 bbmult[4], bbmult_zero = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
		pi->dacratemode2g : pi->dacratemode5g;

	/* adc_cap_cal */
	if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac) || CCT_BAND_CHG(pi_ac)) {
		FOREACH_CORE(pi, core) {
			wlc_phy_get_tx_bbmult_acphy(pi, &bbmult[core], core);
			wlc_phy_set_tx_bbmult_acphy(pi, &bbmult_zero, core);
		}
		wlc_phy_radio_afecal(pi);

		FOREACH_CORE(pi, core) {
			wlc_phy_set_tx_bbmult_acphy(pi, &bbmult[core], core);
		}
	}

	/* Configure DAC buffer BW */
	if (PHY_PAPDEN(pi)) {
		wlc_phy_radio20696_txdac_bw_setup(pi, BUTTERWORTH, DACBW_120);
	} else {
		dacbw = CHSPEC_IS20(pi->radio_chanspec) ? DACBW_30:
			CHSPEC_IS40(pi->radio_chanspec) ? DACBW_60:
			CHSPEC_IS80(pi->radio_chanspec) ? DACBW_120: DACBW_150;
		wlc_phy_radio20696_txdac_bw_setup(pi, 0, dacbw);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
void
wlc_phy_radio20694_afe_div_ratio(phy_info_t *pi, uint8 use_ovr, uint8 ipapd)
{
	uint8 is_5g = CHSPEC_IS5G(pi->radio_chanspec);
	uint8 dac_en_div2 = 0;
	uint8 adc_en_div2 = 0;
	uint8 dac_ratio_idx = 0;
	uint8 adc_ratio_idx = 0;
	uint16 NAD, NDA;
	uint8 core, i;

	uint8 inv_phase_AD_val, rsb_AD_val, div2_AD_val, div_AD_val;
	uint8 inv_phase_DA_val, rsb_DA_val, div2_DA_val, div_DA_val;

	uint16 AFEdiv_AD_ratio_list[2][4]	= {
				{12, 24, 960, 0},
				{16, 24, 48, 1920}};
	uint8 AFEdiv_AD_en_div2_code[2][4]	= {
				{0, 0, 0, 0},
				{1, 1, 1, 1}};
	uint8 AFEdiv_AD_rsb_code[2][4]		= {
				{7, 15, 15, 0},
				{7, 7, 15, 15}};
	uint8 AFEdiv_AD_div_code[2][4]		= {
				{4, 8, 14, 0},
				{0, 4, 8, 14}};
	uint8 AFEdiv_AD_inv_phase_code[2][4] = {
				{1, 1, 1, 0},
				{0, 1, 1, 1}};
	uint8 AFEdiv_AD_div2_code[2][4]		= {
				{1, 1, 3, 0},
				{1, 1, 1, 3}};
	uint16 AFEdiv_DA_ratio_list[2][3] 	= {
				{4, 6, 12},
				{8, 12, 24}};
	uint8 AFEdiv_DA_en_div2_code[2][3] 	= {
				{0, 0, 0},
				{1, 1, 1}};
	uint8 AFEdiv_DA_rsb_code[2][3]		= {
				{3, 3, 7},
				{3, 3, 7}};
	uint8 AFEdiv_DA_div_code[2][3]		= {
				{0, 2, 4},
				{0, 2, 4}};
	uint8 AFEdiv_DA_inv_phase_code[2][3] = {
				{0, 0, 1},
				{0, 0, 1}};
	uint8 AFEdiv_DA_div2_code[2][3]		= {
				{1, 1, 1},
				{1, 1, 1}};

	uint sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
	pi->dacratemode2g : pi->dacratemode5g;

	MOD_PHYREG(pi, RxFeCtrl1, soft_sdfeFifoReset, 1);

	if (CHSPEC_IS160(pi->radio_chanspec)) {
		NAD = 16;
		NDA = 8;
	} else if (CHSPEC_IS80(pi->radio_chanspec)) {
		NAD = 16;
		NDA = 8;
	} else if (CHSPEC_IS40(pi->radio_chanspec)) {
		NAD = (12 << is_5g);
		NDA = (6 << is_5g);
	} else {
		NAD = (24 << is_5g);
#ifdef REGULATORY_DEBUG
		NDA = (6 << is_5g);
#else
		NDA = (12 << is_5g);
#endif
	}

	if ((pi_ac->radioi->data->dac_mode) == 2) {
		NAD = (8 << is_5g);
		NDA = (4 << is_5g);
	}

	if ((pi_ac->radioi->data->dac_mode) == 3) {
		NAD = (24 << is_5g);
		NDA = (6 << is_5g);
	}

	for (i = 0; i < 3; i++) {
		if (NDA == AFEdiv_DA_ratio_list[is_5g][i]) {
			dac_ratio_idx = i;
			break;
		}
	}

	for (i = 0; i < (3 + is_5g); i++) {
		if (NAD == AFEdiv_AD_ratio_list[is_5g][i]) {
			adc_ratio_idx = i;
			break;
		}
	}
	dac_en_div2 = AFEdiv_DA_en_div2_code[is_5g][dac_ratio_idx];
	adc_en_div2 = AFEdiv_AD_en_div2_code[is_5g][adc_ratio_idx];

	if (use_ovr) {
		if (dac_en_div2 == adc_en_div2) {
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 0, afediv_en_div2, adc_en_div2);
			if (sicoreunit != DUALMAC_AUX) {
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 1,
				afediv_en_div2, adc_en_div2);
			}
		} else {
			PHY_ERROR(("wl%d: %s: ADC and DAC en_div2 values don't agree,"
				"check NAD and NDA values in 20694_afe_div_ratio!\n",
				pi->sh->unit, __FUNCTION__));
			ASSERT(FALSE);
		}

		inv_phase_AD_val = AFEdiv_AD_inv_phase_code[is_5g][adc_ratio_idx];
		rsb_AD_val = AFEdiv_AD_rsb_code[is_5g][adc_ratio_idx];
		div2_AD_val = AFEdiv_AD_div2_code[is_5g][adc_ratio_idx];
		div_AD_val = AFEdiv_AD_div_code[is_5g][adc_ratio_idx];

		inv_phase_DA_val = AFEdiv_DA_inv_phase_code[is_5g][dac_ratio_idx];
		rsb_DA_val = AFEdiv_DA_rsb_code[is_5g][dac_ratio_idx];
		div2_DA_val	= AFEdiv_DA_div2_code[is_5g][dac_ratio_idx];
		div_DA_val = AFEdiv_DA_div_code[is_5g][dac_ratio_idx];
		FOREACH_CORE(pi, core) {
			if ((sicoreunit != DUALMAC_AUX) || (core != 1)) {
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_en_div2, 0x1);
				/* adc overwrite */
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_adc_inv_phase, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_adc_rsb, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_adc_div2, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_adc_div, 0x1);
				/* dac overwrite */
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_dac_inv_phase, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_dac_rsb, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_dac_div2, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core,
				ovr_afe_dac_div, 0x1);

				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG3, core,
				afediv_adc_inv_phase, inv_phase_AD_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG3, core,
				afediv_adc_rsb, rsb_AD_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG3, core,
				afediv_adc_div2, div2_AD_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG3, core,
				afediv_adc_div, div_AD_val);

				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG2, core,
				afediv_dac_inv_phase, inv_phase_DA_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG2, core,
				afediv_dac_rsb, rsb_DA_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG2, core,
				afediv_dac_div2, div2_DA_val);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG2, core,
				afediv_dac_div, div_DA_val);

				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_pu, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_lo_sel, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_adc_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_dac_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_mm_route_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_arst_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_arst, 0x1);

				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, core, afediv_pu, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_dac_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_adc_en, 0x1);

				/* only implement mode = MIMO in driver */
				if (is_5g) {
					MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core,
					afediv_lo_sel, 0x1);
				} else {
					MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core,
					afediv_lo_sel, 0x0);
				}

				/* turn off everything 1st in case other core is not correct */
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core,
				afediv_mm_route_en, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core, afediv_arst_en, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, core,
				afediv_srst_share_only, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, core, afediv_pu, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_dac_en, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_adc_en, 0x0);
			}
		}
		MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 0, afediv_mm_route_en, 0x1);
		MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 0, afediv_arst_en, 0x1);
		MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, 0,
		ovr_direct_afediv_srst_share_only, 0x1);
		MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, 0, afediv_srst_share_only, 0x0);
		MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, 0, afediv_pu, 0x1);

		if (sicoreunit != DUALMAC_AUX) {
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 1, afediv_mm_route_en, 0x0);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, 1, afediv_arst_en, 0x1);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, 1,
			ovr_direct_afediv_srst_share_only, 0x1);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, 1, afediv_srst_share_only, 0x0);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG4, 1, afediv_pu, 0x1);
		}

		FOREACH_CORE(pi, core) {
			if ((sicoreunit != DUALMAC_AUX) || (core != 1)) {
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_dac_en, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core, afediv_adc_en, 0x1);
				/* Trigger Areset */
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core, afediv_arst, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core, afediv_arst, 0x1);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG1, core, afediv_arst, 0x0);
				/* Remove over-rides for arst_en and arst */
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_arst_en, 0x0);
				MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
				ovr_afediv_arst, 0x0);
			}
		}
	} else {
		/* remove override mode */
		FOREACH_CORE(pi, core) {
			if ((sicoreunit != DUALMAC_AUX) || (core != 1)) {
				WRITE_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core, 0);
				WRITE_RADIO_REG_20694(pi, RF, AFEDIV_CFG2_OVR, core, 0);
			}
		}
	}
	MOD_PHYREG(pi, RxFeCtrl1, soft_sdfeFifoReset, 0);
}

static void
wlc_phy_radio20694_rccal(phy_info_t *pi)
{

	uint8 cal, done, core;
	uint16 rccal_itr, n0, n1;
	/* lpf, dacbuf */
	uint8 sr[] = {0x1, 0x0};
	uint8 sc[] = {0x0, 0x1};
	uint8 X1[] = {0x1c, 0x40};
	uint16 rccal_trc_set[] = {0x22d, 0x10a};

	uint32 gmult_dacbuf_k = 77705;
	uint32 gmult_lpf_k = 74800;
	uint32 gmult_dacbuf;

	phy_ac_radio_data_t *data = pi->u.pi_acphy->radioi->data;
	uint32 dn;

	pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
	data->rccal_gmult_rc     = 4260;

	/* Powerup rccal driver & set divider radio (rccal needs to run at 20mhz) */
	MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_wlandig, 1);
	MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_RCCAL, 1);

	/* Calibrate lpf, dacbuf cal 0 = lpf 1 = dacbuf */
	for (cal = 0; cal < 2; cal++) {
		/* Setup */
		/* Q!=2 -> 16 counts */
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG1, 0, rccal_Q1, 0x1);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG0, 0, rccal_sr, sr[cal]);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG0, 0, rccal_sc, sc[cal]);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG1, 0, rccal_X1, X1[cal]);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG2, 0, rccal_Trc, rccal_trc_set[cal]);

		/* Toggle RCCAL PU */
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG1, 0, rccal_START, 0);
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG1, 0, rccal_START, 1);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		done = 0;
		for (rccal_itr = 0;
		     (rccal_itr < MAX_2069_RCCAL_WAITLOOPS) && (done == 0);
		     rccal_itr++) {
			OSL_DELAY(100);
			done = READ_RADIO_REGFLD_20694(pi, RFP, RCCAL_CFG3, 0, rccal_DONE);
		}

		/* Stop RCCAL */
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG1, 0, rccal_START, 0);

		/* Make sure that RC Cal ran to completion */
		ASSERT(done);

		n0 = READ_RADIO_REGFLD_20694(pi, RFP, RCCAL_CFG4, 0, rccal_N0);
		n1 = READ_RADIO_REGFLD_20694(pi, RFP, RCCAL_CFG5, 0, rccal_N1);
		dn = n1 - n0; /* set dn [expr {$N1 - $N0}] */
		// dn = 1;  // JANATH added to avoid devide by zero error

		if (cal == 0) {
			/* lpf  values */
			data->rccal_gmult = (gmult_lpf_k * dn) / (pi->xtalfreq >> 12);
			data->rccal_gmult_rc = data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
			            __FUNCTION__, data->rccal_gmult));
		} else if (cal == 1) {
			/* dacbuf  */
			gmult_dacbuf  = (gmult_dacbuf_k * dn) / (pi->xtalfreq >> 12);
			pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = (1 << 24) / (gmult_dacbuf);
			PHY_INFORM(("wl%d: %s rccal_dacbuf_cmult = %d\n", pi->sh->unit,
				__FUNCTION__, pi->u.pi_acphy->radioi->rccal_dacbuf_cmult));
		}
		/* Turn off rccal */
		MOD_RADIO_REG_20694(pi, RFP, RCCAL_CFG0, 0, rccal_pu, 0);
	}
	/* Powerdown rccal driver */
	MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_RCCAL, 0);

	/* nominal values when rc cal failes  */
	if (done == 0) {
		pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
		data->rccal_gmult_rc     = 4260;
	}

	/* Program the register */
	FOREACH_CORE(pi, core) {
		WRITE_RADIO_REG_20694(pi, RF, TIA_GMULT_BQ1, core, data->rccal_gmult_rc);
		WRITE_RADIO_REG_20694(pi, RF, LPF_GMULT_BQ2, core, data->rccal_gmult_rc);
		WRITE_RADIO_REG_20694(pi, RF, LPF_GMULT_RC, core, data->rccal_gmult_rc);
	}

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
wlc_phy_radio20696_rccal(phy_info_t *pi)
{

	uint8 cal, done, core;
	uint16 rccal_itr;
	int16 n0, n1;
	/* lpf, dacbuf */
	uint8 sr[] = {0x1, 0x0};
	uint8 sc[] = {0x0, 0x1};
	uint8 X1[] = {0x1c, 0x40};
	uint16 rccal_trc_set[] = {0x22d, 0x10a};

	uint32 gmult_dacbuf_k = 77705;
	uint32 gmult_lpf_k = 74800;
	uint32 gmult_dacbuf;

	phy_ac_radio_data_t *data = pi->u.pi_acphy->radioi->data;
	uint32 dn;

	pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
	data->rccal_gmult_rc     = 4260;

	if (ISSIM_ENAB(pi->sh->sih)) {
		PHY_INFORM(("wl%d: %s BYPASSED in QT\n", pi->sh->unit, __FUNCTION__));
		return;
	}

	/* Calibrate lpf, dacbuf cal 0 = lpf 1 = dacbuf */
	for (cal = 0; cal < 2; cal++) {
		/* Setup */
		/* Q!=2 -> 16 counts */
		MOD_RADIO_REG_20696(pi, RCCAL_CFG2, 1, rccal_Q1, 0x1);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_sr, sr[cal]);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_sc, sc[cal]);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG2, 1, rccal_X1, X1[cal]);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG3, 1, rccal_Trc, rccal_trc_set[cal]);

		/* Toggle RCCAL PU */
		MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_REG_20696(pi, RCCAL_CFG2, 1, rccal_START, 0);
		MOD_RADIO_REG_20696(pi, RCCAL_CFG2, 1, rccal_START, 1);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		done = 0;
		for (rccal_itr = 0;
		     (rccal_itr < MAX_2069_RCCAL_WAITLOOPS) && (done == 0);
		     rccal_itr++) {
			OSL_DELAY(100);
			done = READ_RADIO_REGFLD_20696(pi, RF, RCCAL_CFG4, 1, rccal_DONE);
		}

		/* Stop RCCAL */
		MOD_RADIO_REG_20696(pi, RCCAL_CFG2, 1, rccal_START, 0);

		/* Make sure that RC Cal ran to completion */
		ASSERT(done);

		/* n0 and n1 are 12 bit signed */
		n0 = READ_RADIO_REGFLD_20696(pi, RF, RCCAL_CFG5, 1, rccal_N0);
		if (n0 & 0x1000)
			n0 = n0 - 0x2000;

		n1 = READ_RADIO_REGFLD_20696(pi, RF, RCCAL_CFG6, 1, rccal_N1);
		if (n1 & 0x1000)
			n1 = n1 - 0x2000;

		dn = n1 - n0;
		PHY_INFORM(("wl%d: %s n0 = %d\n", pi->sh->unit, __FUNCTION__, n0));
		PHY_INFORM(("wl%d: %s n1 = %d\n", pi->sh->unit, __FUNCTION__, n1));

		if (cal == 0) {
			/* lpf  values */
			data->rccal_gmult = (gmult_lpf_k * dn) / (pi->xtalfreq >> 12);
			data->rccal_gmult_rc = data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
			            __FUNCTION__, data->rccal_gmult));
		} else if (cal == 1) {
			/* dacbuf  */
			gmult_dacbuf  = (gmult_dacbuf_k * dn) / (pi->xtalfreq >> 12);
			pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = (1 << 24) / (gmult_dacbuf);
			PHY_INFORM(("wl%d: %s rccal_dacbuf_cmult = %d\n", pi->sh->unit,
				__FUNCTION__, pi->u.pi_acphy->radioi->rccal_dacbuf_cmult));
		}
		/* Turn off rccal */
		MOD_RADIO_REG_20696(pi, RCCAL_CFG0, 1, rccal_pu, 0);

	}
	/* nominal values when rc cal failes  */
	if (done == 0) {
		pi->u.pi_acphy->radioi->rccal_dacbuf_cmult = 4096;
		data->rccal_gmult_rc = 4260;
	}
	/* Programming lpf gmult and tia gmult */
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20696(pi, TIA_GMULT_BQ1, core, tia_gmult_bq1, data->rccal_gmult_rc);
		MOD_RADIO_REG_20696(pi, LPF_GMULT_BQ2, core, lpf_gmult_bq2, data->rccal_gmult_rc);
		MOD_RADIO_REG_20696(pi, LPF_GMULT_RC, core, lpf_gmult_rc, data->rccal_gmult_rc);
	}

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_radio20694_pwron_seq_phyregs(phy_info_t *pi)
{
	/* 20694 specific code  added */
	/* # power down everything */
	/* do not touch bg_pulse */

	uint8 core;

	FOREACH_CORE(pi, core) {
		WRITE_PHYREGCE(pi, RfctrlCoreTxPus, core, 0x0);
		WRITE_PHYREGCE(pi, RfctrlOverrideTxPus, core, 0x3ff);
	}

	/* Using usleep of 100us below, so don't need these */
	WRITE_PHYREG(pi, Pllldo_resetCtrl, 0);
	WRITE_PHYREG(pi, Rfpll_resetCtrl, 0);
	WRITE_PHYREG(pi, Logen_AfeDiv_reset, 0x2000);

	/* Start with everything off: {radio, rfpll, plldlo, logen}_{pu, reset} = 0 */
	WRITE_PHYREG(pi, RfctrlCmd, 0);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0);
	WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0xd);

	/* Reset radio, jtag */
	WRITE_PHYREG(pi, RfctrlCmd, 0x7);
	OSL_DELAY(100);
	WRITE_PHYREG(pi, RfctrlCmd, 0x6);

	//4347 Bringup WAR - ldo1p8_pu need to be asserted again
	if (RADIO20694REV(pi->pubpi->radiorev) >= 8) {
		MOD_RADIO_REG_20694(pi, RFP, LDO1P8_STAT, 0, ldo1p8_pu, 0x1);
	}

	// YOU ABSOLUTELY NEED TO UPDATE THE PREFERRED VALUES HERE.
	// RfctrlCmd wipes out a metric ton of RF registers. You gotta re-init.
	// Update preferred values


	wlc_phy_radio20694_upd_prfd_values(pi);

	wlc_phy_radio20694_low_current_mode(pi, 2);

	/* rfpll, pllldo, logen_pu, reset */
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0xd);
	WRITE_PHYREG(pi, RfctrlCmd, 0x2);

	WRITE_PHYREGCE(pi, RfctrlCoreTxPus, 0, 0x180);
	OSL_DELAY(100);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x4);

	FOREACH_CORE(pi, core) {
		WRITE_PHYREGCE(pi, RfctrlCoreTxPus, core, 0x80);
		WRITE_PHYREGCE(pi, RfctrlOverrideTxPus, core, 0x180);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
/* Global Radio PU's */
static void wlc_phy_radio20696_pwron_seq_phyregs(phy_info_t *pi)
{
	/*  Note: if RfctrlCmd.rfctrl_bundle_en = 0, then rfpll_pu = radio_pu */
	/*  rfpll_pu = 0 & radio_pwrup = 1, make RfctrlCmd.rfctrl_bundle_en = 1 */

	/* power down everything	 */
	ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0);
	ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x3ff);

	/* Using usleep of 100us below, so don't need these */
	WRITE_PHYREG(pi, Pllldo_resetCtrl, 0);
	WRITE_PHYREG(pi, Rfpll_resetCtrl, 0);
	WRITE_PHYREG(pi, Logen_AfeDiv_reset, 0x2000);

	/*  Start with everything off: {radio, rfpll, plldlo, logen}_{pu, reset} = 0 */
	WRITE_PHYREG(pi, RfctrlCmd, 0);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0);
	WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0xd);

	/*  Reset radio, jtag */
	WRITE_PHYREG(pi, RfctrlCmd, 0x7);
	OSL_DELAY(1000);
	WRITE_PHYREG(pi, RfctrlCmd, 0x6);

	/*  Update preferred values */
	wlc_phy_radio20696_upd_prfd_values(pi);

	/*  {rfpll, pllldo, logen}_{pu, reset} */
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0xd);
	WRITE_PHYREG(pi, RfctrlCmd, 0x2);

	ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0x180);
	OSL_DELAY(1000);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x4);
	ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0x80);
	ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x180);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
wlc_phy_chanspec_radio20694_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset, uint8 core)
{

	const chan_info_radio20694_rffe_t *chan_info;

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	if (wlc_phy_chan2freq_20694(pi, ch, &chan_info) < 0) {
		return;
	}

	/* logen_reset needs to be toggled whenever bandsel bit if changed */
	/* On a bw change, phy_reset is issued which causes currentBand getting reset to 0 */
	/* So, issue this on both band & bw change */
	if (toggle_logen_reset == 1) {
		wlc_phy_logen_reset(pi, core);
	}
	PHY_TRACE(("wl%d: BEFORE wlc_phy_radio20694_pll_tune", pi->sh->unit));
	/* pll tuning */
	wlc_phy_radio20694_pll_tune(pi, chan_info->freq, core);
	PHY_TRACE(("wl%d: AFTER wlc_phy_radio20694_pll_tune", pi->sh->unit));
	/* rffe tuning */
	wlc_phy_radio20694_rffe_tune(pi, chan_info, core);
	wlc_phy_20694_radio_vcocal(pi, 0, 1);

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		wlc_phy_20694_logen_mode(pi, core);
	}

	if (core == 0) {
		/* disable BG bias filter */
		MOD_RADIO_REG_20694(pi, RFP, BG_REG2, core, bg_pulse, 1);
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, core, ovr_bg_pulse, 1);
	}

	/* Load band related regs */
	wlc_phy_radio20694_upd_band_related_reg(pi);
}


static void
wlc_phy_20694_radio_rcal(phy_info_t *pi, uint8 mode)
{
	/* Format: 20694_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* The rcalcode argument works only with mode 1 and is optional. */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP. This is what driver should */
	/* do but is not good for bringup because the OTP is not programmed yet. */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */
	uint8 rcal_value; // rcal_valid, loop_iter, rcal_value;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_wlandig, 1);

	if (mode == 0) {
		/* Use OTP stored rcal value */
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_otp_rcal_sel, 0x1);
	} else if (mode == 1) {
		rcal_value = 0x33;
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0,
			ovr_otp_rcal_sel, 0);
		MOD_RADIO_REG_20694(pi, RFP, BG_REG8, 0,
			bg_rcal_trim, rcal_value);
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0,
			ovr_bg_rcal_trim, 1);
	} else if (mode == 2) {
		/* add code here */
	}

}

static void
wlc_phy_20694_radio_rcal_byaux(phy_info_t *pi, uint8 mode)
{
	/* Format: 20694_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* The rcalcode argument works only with mode 1 and is optional. */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP. This is what driver should */
	/* do but is not good for bringup because the OTP is not programmed yet. */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */

	uint8 rcal_value;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_wlandig, 1);

	if (mode == 0) {
		/* Use OTP stored rcal value */
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_mainauxshared_otp_rcal_sel, 0x1);
	} else if (mode == 1) {
		/* Default RCal code to be used with mode 1 is 0x32 */
		rcal_value = 0x33;
		MOD_RADIO_REG_20694(pi, RFP, BG_REG1_MAINAUX_SHRD, 0,
			shrd_bg_rtl_rcal_trim, rcal_value);
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_mainauxshared_otp_rcal_sel, 0x0);
		MOD_RADIO_REG_20694(pi, RFP, BG_REG2_MAINAUX_SHRD, 0,
			shrd_bg_rcal_trim, rcal_value);
		MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_mainauxshared_bg_rcal_trim, 0x1);
	} else if (mode == 2) {
		/* add code here */

	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
/*  R Calibration (takes ~50us) */
/* copied from IGUANA and adabted for 7271 */
/* original code in wlc_phy_28nm_radio_rcal */
/* modified were needed from 20696_procs.tcl */
static void
wlc_phy_radio20696_r_cal(phy_info_t *pi, uint8 mode)
{
	/* Format: 20696_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */
	uint8 rcal_valid, loop_iter, rcal_value;
	uint8 core = 1;

	if (mode == 0) {
		/* Use OTP stored rcal value */
		MOD_RADIO_REG_20696(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0x1);
	} else if (mode == 1) {
		/* Default RCal code to be used with mode 1 is 0x32 */
		/* value taken from 20696_procs.tcl */
		rcal_value = 0x32;
		MOD_RADIO_REG_20696(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0);
		MOD_RADIO_REG_20696(pi, BG_REG8, core, bg_rcal_trim, rcal_value);
		MOD_RADIO_REG_20696(pi, BG_OVR1, core, ovr_bg_rcal_trim, 1);
		MOD_RADIO_REG_20696(pi, BG_REG1, core, bg_rtl_rcal_trim, rcal_value);
	} else if (mode == 2) {
		/* This should run only for core 1 */
		/* Run R Cal and use its output */
		MOD_RADIO_REG_20696(pi, GPAIO_SEL2, core, gpaio_pu, 0);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL6, core, gpaio_top_pu, 0);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL8, core, gpaio_rcal_pu, 0);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL0, core, gpaio_sel_0to15_port, 0x0);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL1, core, gpaio_sel_16to31_port, 0x0);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL6, core, gpaio_top_pu, 1);
		MOD_RADIO_REG_20696(pi, GPAIO_SEL8, core, gpaio_rcal_pu, 1);
		MOD_RADIO_REG_20696(pi, BG_REG3, core, bg_rcal_pu, 1);
		MOD_RADIO_REG_20696(pi, RCAL_CFG_NORTH, core, rcal_pu, 1);

		rcal_valid = 0;
		loop_iter = 0;
		while ((rcal_valid == 0) && (loop_iter <= 100)) {
			/* 1 ms delay wait time */
			OSL_DELAY(1000);
			rcal_valid = READ_RADIO_REGFLD_20696(pi, RF, RCAL_CFG_NORTH,
				core, rcal_valid);
			loop_iter++;
		}

		MOD_RADIO_REG_20696(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0);
		MOD_RADIO_REG_20696(pi, BG_OVR1, core, ovr_bg_rcal_trim, 1);

		if (rcal_valid == 1) {
			rcal_value = READ_RADIO_REGFLD_20696(pi, RF, RCAL_CFG_NORTH, core,
				rcal_value);

			/* Very coarse sanity check */
			if ((rcal_value < 2) || (60 < rcal_value)) {
				PHY_ERROR(("*** ERROR: R Cal value out of range."
				" 6bit Rcal = %d.\n", rcal_value));
				rcal_value = 0x32;
			}
		} else {
			PHY_ERROR(("%s RCal unsucessful. RCal valid bit is %d.\n",
				__FUNCTION__, rcal_valid));
			/* take some sane default value */
			rcal_value = 0x32;
		}
		MOD_RADIO_REG_20696(pi, BG_REG1, core, bg_rtl_rcal_trim, rcal_value);
		MOD_RADIO_REG_20696(pi, BG_REG8, core, bg_rcal_trim, rcal_value);


		/* Power down blocks not needed anymore */
		MOD_RADIO_REG_20696(pi, RCAL_CFG_NORTH, core, rcal_pu, 0);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
void
wlc_phy_radio20694_txdac_bw_setup(phy_info_t *pi, uint8 filter_type, uint8 dacbw)
{
	uint16 code_cfb = 0x50;
	uint32 dacbuf_cmult = pi->u.pi_acphy->radioi->rccal_dacbuf_cmult;
	uint8 cfb_default_code[4] = {0x50, 0x3c, 0x30, 0x1c};
	uint8 core;

	ASSERT(dacbw != 0);

	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG1, core, iqdac_buf_op_cur, 0x0);

		if ((dacbw == DACBW_120) || (dacbw == DACBW_150)) {
			MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_buf_bw, 0x0);
		} else if ((dacbw == DACBW_60) || (dacbw == DACBW_30)) {
			MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_buf_bw, 0x4);
		}

		if (dacbw == DACBW_30) {
			code_cfb = cfb_default_code[0];
		} else if (dacbw == DACBW_60) {
			code_cfb = cfb_default_code[1];
		} else if (dacbw == DACBW_120) {
			code_cfb = cfb_default_code[2];
		} else {
			code_cfb = cfb_default_code[3];
		}

		code_cfb = (dacbuf_cmult * code_cfb) >> 12;
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG1, core, iqdac_buf_Cfb, code_cfb);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
void
wlc_phy_radio20696_txdac_bw_setup(phy_info_t *pi, uint8 filter_type, uint8 dacbw)
{
	uint8 core;

	ASSERT(dacbw != 0);

	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20696(pi, TXDAC_REG1, core, iqdac_buf_op_cur, 0x0);

		if ((dacbw == DACBW_120) || (dacbw == DACBW_150)) {
			MOD_RADIO_REG_20696(pi, TXDAC_REG0, core, iqdac_buf_bw, 0x0);
		} else if ((dacbw == DACBW_60) || (dacbw == DACBW_30)) {
			MOD_RADIO_REG_20696(pi, TXDAC_REG0, core, iqdac_buf_bw, 0x4);
		}

		MOD_RADIO_REG_20696(pi, TXDAC_REG1, core, iqdac_buf_Cfb, 127);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
int8
wlc_phy_20694_radio_minipmu_cal(phy_info_t *pi)
{
	uint8 calsuccesful = 1;
	int8 cal_status = 1;
	int16 waitcounter = 0;
	uint16 temp_code;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	/* Setup the cal */
	MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_bg_refadj_cbuck, 0x1);
	MOD_RADIO_REG_20694(pi, RFP, XTAL_OVR1, 0, ovr_xtal_refadj_cbuck, 0x1);

	WRITE_RADIO_REG_20694(pi, RFP, BG_REG10, 0, 0x3f);
	MOD_RADIO_REG_20694(pi, RFP, BG_OVR1, 0, ovr_pmu_bg_wlpmu_en_i, 0x1);

	/* 25 us delay to allow PMU cal to happen */
	OSL_DELAY(25);

	while (READ_RADIO_REGFLD_20694(pi, RFP, BG_REG11, 0, bg_wlpmu_cal_done) == 0) {
		/* 10 us delay wait time */
	        OSL_DELAY(100);
		waitcounter++;

		if (waitcounter > 100) {
			PHY_TRACE(("\nWarning:Mini PMU Cal Failed on Core0"));
			calsuccesful = 0;
			break;
		}
	}

	/* Cleanup */

	WRITE_RADIO_REG_20694(pi, RFP, BG_REG10, 0, 0x6c);
	MOD_RADIO_REG_20694(pi, RF, PMU_CFG3, 0, vref_select, 0x1);
	MOD_RADIO_REG_20694(pi, RF, PMU_CFG3, 1, vref_select, 0x1);

	if (calsuccesful == 1) {
		PHY_TRACE(("MiniPMU cal done on core0, Cal code = %d",
			READ_RADIO_REGFLD_20694(pi, RFP, BG_REG11, 0, bg_wlpmu_calcode)));
		temp_code = READ_RADIO_REGFLD_20694(pi, RFP, BG_REG11, 0, bg_wlpmu_calcode);
		MOD_RADIO_REG_20694(pi, RFP, BG_REG9, 0, bg_wlpmu_cal_mancodes, temp_code);
		cal_status = 1;
	} else {
		PHY_TRACE(("Cal Unsuccesful on Core1"));
		cal_status = -1;
	}

	return cal_status;
}

int8
wlc_phy_20694_radio_minipmu_cal_byaux(phy_info_t *pi)
{
	uint8 calsuccesful = 1;
	int8 cal_status = 1;
	int16 waitcounter = 0;
	uint16 temp_code;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID));

	ACPHY_REG_LIST_START
		/* Setup the cal */
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_OVR1, 0, ovr_bg_refadj_cbuck, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, PMU_REG3_MAINAUX_SHRD,
				0, shrd_ldo1p8_pu, 0x1)


		MOD_RADIO_REG_20694_ENTRY(pi, RFP, XTAL_OVR1, 0, ovr_xtal_refadj_cbuck, 0x1)

		//set 1p6V LDO ratio
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_bypcal, 0x0)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_en_i, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_pu_cbuck_ref, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_vref_sel, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_pu_shrd_bg_ref, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_clk10M_en, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_startcal, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* 25 us delay to allow PMU cal to happen */
	OSL_DELAY(25);

	while (READ_RADIO_REGFLD_20694(pi, RFP, BG_REG4_MAINAUX_SHRD, 0,
		shrd_bg_wlpmu_cal_done) == 0) {

		/* 10 us delay wait time */
		OSL_DELAY(10);
		waitcounter++;

		if (waitcounter > 100) {
			PHY_TRACE(("\nWarning:Mini PMU Cal Failed on Core0"));
			calsuccesful = 0;
			break;
		}
	}

	/* Cleanup */
	ACPHY_REG_LIST_START
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_bypcal, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_en_i, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_pu_cbuck_ref, 0x0)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_vref_sel, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_pu_shrd_bg_ref, 0x1)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_clk10M_en, 0x0)
		MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG4_MAINAUX_SHRD,
			0, shrd_bg_wlpmu_startcal, 0x0)

		MOD_RADIO_REG_20694_ENTRY(pi, RF, PMU_REG1_MAINAUX_SHRD,
			0, shrd_vref_select, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);

	if (calsuccesful == 1) {
		PHY_TRACE(("MiniPMU cal done on core0, Cal code = %d",
			READ_RADIO_REGFLD_20694(pi, RFP, BG_REG4_MAINAUX_SHRD,
				0, shrd_bg_wlpmu_calcode)));
		temp_code = READ_RADIO_REGFLD_20694(pi, RFP, BG_REG4_MAINAUX_SHRD,
				0, shrd_bg_wlpmu_calcode);
		MOD_RADIO_REG_20694(pi, RFP, BG_REG3_MAINAUX_SHRD, 0,
			shrd_bg_wlpmu_cal_mancodes, temp_code);
		cal_status = 1;
	} else {
		PHY_TRACE(("Cal Unsuccesful on Core1"));
		cal_status = -1;
	}

	return cal_status;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
/* Perform MINI PMU CAL */
static void wlc_phy_radio20696_minipmu_cal(phy_info_t *pi)
{
	uint8 waitcounter;
	uint8 core;
	bool calsuccesful;

	WRITE_RADIO_REG_20696(pi, BG_REG10, 1, 0x37); /* to enable Cbuck (vrefsel) */
	OSL_DELAY(25);

	/* Wait for cal_done	 */
	calsuccesful = TRUE;
	waitcounter = 0;
	while (READ_RADIO_REGFLD_20696(pi, RF, BG_REG11, 1, bg_wlpmu_cal_done) == 0) {
		OSL_DELAY(100);
		waitcounter++;
		if (waitcounter > 100) {
			/* This means the cal_done bit is not 1 even after waiting a while */
			/* Exit gracefully */
			PHY_INFORM(("wl%d: %s: Warning : Mini PMU Cal Failed\n",
				pi->sh->unit, __FUNCTION__));
			calsuccesful = FALSE;
			break;
		}
	}
	/* Cleanup : Setting the caldone bit to 0 here */
	WRITE_RADIO_REG_20696(pi, BG_REG10, 1, 0x2C);
	MOD_RADIO_REG_20696(pi, BG_REG10, 1, bg_wlpmu_bypcal, 0x1);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20696(pi, PMU_CFG3, core, vref_select, 0x1);
	}
	if (calsuccesful) {
		uint8 tempcalcode;

		tempcalcode = READ_RADIO_REGFLD_20696(pi, RF, BG_REG11, 1, bg_wlpmu_calcode);
		PHY_INFORM(("wl%d: %s MiniPMU done. Cal Code = %d\n", pi->sh->unit,
				__FUNCTION__, tempcalcode));
		MOD_RADIO_REG_20696(pi, BG_REG9, 1, bg_wlpmu_cal_mancodes, tempcalcode);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
/* 20693 radio related functions */
int
wlc_phy_chan2freq_20693(phy_info_t *pi, uint8 channel,
	const chan_info_radio20693_pll_t **chan_info_pll,
	const chan_info_radio20693_rffe_t **chan_info_rffe,
	const chan_info_radio20693_pll_wave2_t **chan_info_pll_wave2)
{
	uint i;
	uint16 freq;
	const chan_info_radio20693_pll_t *chan_info_tbl_pll = NULL;
	const chan_info_radio20693_rffe_t *chan_info_tbl_rffe = NULL;
	const chan_info_radio20693_pll_wave2_t *chan_info_tbl_pll_wave2_part1 = NULL;
	const chan_info_radio20693_pll_wave2_t *chan_info_tbl_pll_wave2_part2 = NULL;
	uint32 tbl_len, tbl_len1 = 0;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	/* Choose the right table to use */
	switch (RADIO20693REV(pi->pubpi->radiorev)) {
	case 3:
	case 4:
	case 5:
		chan_info_tbl_pll = chan_tuning_20693_rev5_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev5_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev5_pll);
		break;
	case 6:
		chan_info_tbl_pll = chan_tuning_20693_rev6_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev6_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev6_pll);
		break;
	case 7:
		chan_info_tbl_pll = chan_tuning_20693_rev5_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev5_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev5_pll);
		break;
	case 8:
		chan_info_tbl_pll = chan_tuning_20693_rev6_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev6_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev6_pll);
		break;
	case 10:
		chan_info_tbl_pll = chan_tuning_20693_rev10_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev10_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev10_pll);
		break;
	case 11:
	case 12:
	case 13:
		chan_info_tbl_pll = chan_tuning_20693_rev13_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev13_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev13_pll);
		break;
	case 14:
	case 19:
		chan_info_tbl_pll = chan_tuning_20693_rev14_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev14_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev14_pll);
		break;
	case 15:
		if (PHY_XTAL_IS37M4(pi)) {
			chan_info_tbl_pll = chan_tuning_20693_rev15_pll_37p4MHz;
			chan_info_tbl_rffe = chan_tuning_20693_rev15_rffe_37p4MHz;
			tbl_len = ARRAYSIZE(chan_tuning_20693_rev15_pll_37p4MHz);
		} else {
			chan_info_tbl_pll = chan_tuning_20693_rev15_pll_40MHz;
			chan_info_tbl_rffe = chan_tuning_20693_rev15_rffe_40MHz;
			tbl_len = ARRAYSIZE(chan_tuning_20693_rev15_pll_40MHz);
		}
		break;
	case 16:
	case 17:
	case 20:
		chan_info_tbl_pll = chan_tuning_20693_rev5_pll;
		chan_info_tbl_rffe = chan_tuning_20693_rev5_rffe;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev5_pll);
		break;
	case 18:
	case 21:
		if (PHY_XTAL_IS37M4(pi)) {
			chan_info_tbl_pll = chan_tuning_20693_rev18_pll;
			chan_info_tbl_rffe = chan_tuning_20693_rev18_rffe;
			tbl_len = ARRAYSIZE(chan_tuning_20693_rev18_pll);
		} else {
			chan_info_tbl_pll = chan_tuning_20693_rev15_pll_40MHz;
			chan_info_tbl_rffe = chan_tuning_20693_rev15_rffe_40MHz;
			tbl_len = ARRAYSIZE(chan_tuning_20693_rev15_pll_40MHz);
		}
		break;
	case 23:
		/* Preferred values for 53573/53574/47189-B0 */
		chan_info_tbl_pll = chan_tuning_20693_rev15_pll_40MHz;
		chan_info_tbl_rffe = chan_tuning_20693_rev15_rffe_40MHz;
		tbl_len = ARRAYSIZE(chan_tuning_20693_rev15_pll_40MHz);
		break;
	case 32:
		chan_info_tbl_pll  = NULL;
		chan_info_tbl_rffe = NULL;
		chan_info_tbl_pll_wave2_part1 = chan_tuning_20693_rev32_pll_part1;
		chan_info_tbl_pll_wave2_part2 = chan_tuning_20693_rev32_pll_part2;
		tbl_len1 = ARRAYSIZE(chan_tuning_20693_rev32_pll_part1);
		tbl_len = tbl_len1 + ARRAYSIZE(chan_tuning_20693_rev32_pll_part2);
		break;
	case 33:
		chan_info_tbl_pll  = NULL;
		chan_info_tbl_rffe = NULL;
		chan_info_tbl_pll_wave2_part1 = chan_tuning_20693_rev33_pll_part1;
		chan_info_tbl_pll_wave2_part2 = chan_tuning_20693_rev33_pll_part2;
		tbl_len1 = ARRAYSIZE(chan_tuning_20693_rev32_pll_part1);
		tbl_len = tbl_len1 + ARRAYSIZE(chan_tuning_20693_rev32_pll_part2);
		break;
	case 9:
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIO20693REV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return -1;
	}

	if (RADIOMAJORREV(pi) == 3) {
		for (i = 0; i < tbl_len; i++) {
			if (i >= tbl_len1) {
				if (chan_info_tbl_pll_wave2_part2 != NULL && chan_info_tbl_pll_wave2_part2[i-tbl_len1].chan == channel) {
					*chan_info_pll_wave2 =
							&chan_info_tbl_pll_wave2_part2[i-tbl_len1];
					freq = chan_info_tbl_pll_wave2_part2[i-tbl_len1].freq;
					break;
				}
			} else {
				if (chan_info_tbl_pll_wave2_part1 != NULL && chan_info_tbl_pll_wave2_part1[i].chan == channel) {
					*chan_info_pll_wave2 = &chan_info_tbl_pll_wave2_part1[i];
					freq = chan_info_tbl_pll_wave2_part1[i].freq;
					break;
				}
			}
		}

		if (i >= tbl_len) {
			PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
				pi->sh->unit, __FUNCTION__, channel));
			if (!ISSIM_ENAB(pi->sh->sih)) {
				/* Do not assert on QT since we leave the tables empty on purpose */
				ASSERT(i < tbl_len);
			}
			return -1;
		}
	} else {
		if (chan_info_tbl_pll != NULL) {
			for (i = 0; i < tbl_len && chan_info_tbl_pll[i].chan != channel; i++);

			if (i >= tbl_len) {
				PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
					pi->sh->unit, __FUNCTION__, channel));
				if (!ISSIM_ENAB(pi->sh->sih)) {
					/* Do not assert on QT since we leave the tables empty on purpose */
					ASSERT(i < tbl_len);
				}
				return -1;
			}

			*chan_info_pll = &chan_info_tbl_pll[i];
			*chan_info_rffe = &chan_info_tbl_rffe[i];
			freq = chan_info_tbl_pll[i].freq;
		} else {
			return -1;
		}
	}

	return freq;
}

void
phy_ac_radio_20693_chanspec_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset,
	uint8 pllcore, uint8 mode)
{
	const chan_info_radio20693_pll_t *chan_info_pll;
	const chan_info_radio20693_rffe_t *chan_info_rffe;
	const chan_info_radio20693_pll_wave2_t *chan_info_pll_wave2;
	int fc[NUM_CHANS_IN_CHAN_BONDING];
	uint8 core, start_pllcore = 0, end_pllcore = 0;
	uint8 chans[NUM_CHANS_IN_CHAN_BONDING] = {0, 0};

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	/* mode = 0, setting per pllcore input
	 * mode = 1, setting both pllcores=0/1 at once
	 */
	if (mode == 0) {
		start_pllcore = pllcore;
		end_pllcore = pllcore;
		core = pllcore;
	} else if (mode == 1) {
		start_pllcore = 0;
		end_pllcore = 1;
	} else {
		ASSERT(0);
	}

	if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
		ASSERT(start_pllcore != end_pllcore);
		wf_chspec_get_80p80_channels(pi->radio_chanspec, chans);
	} else {
		chans[pllcore] = ch;
	}

	for (core = start_pllcore; core <= end_pllcore; core++) {
		fc[core] = wlc_phy_chan2freq_20693(pi, chans[core], &chan_info_pll,
				&chan_info_rffe, &chan_info_pll_wave2);

		if (fc[core] < 0)
			return;

		/* logen_reset needs to be toggled whenever bandsel bit if changed */
		/* On a bw change, phy_reset is issued which causes currentBand getting */
		/* reset to 0 */
		/* So, issue this on both band & bw change */
		if (toggle_logen_reset == 1) {
			wlc_phy_logen_reset(pi, core);
		}

		if (RADIOMAJORREV(pi) != 3) {
			/* pll tuning */
			wlc_phy_radio20693_pll_tune(pi, chan_info_pll, core);

			/* rffe tuning */
			wlc_phy_radio20693_rffe_tune(pi, chan_info_rffe, core);
		} else {
			/* pll tuning */
			wlc_phy_radio20693_pll_tune_wave2(pi, chan_info_pll_wave2, core);
			wlc_phy_radio20693_sel_logen_mode(pi);
		}
	}

	if ((RADIOMAJORREV(pi) == 3)) {
		MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1, ldo_1p2_xtalldo1p2_ctl, 0xe);
		MOD_RADIO_ALLPLLREG_20693(pi, PLL_LVLDO1, ldo_1p2_lowquiescenten_PFDMMD, 1);
		MOD_RADIO_ALLPLLREG_20693(pi, PLL_LVLDO1, ldo_1p2_ldo_PFDMMD_vout_sel, 0xc);
		MOD_RADIO_ALLPLLREG_20693(pi, PLL_LVLDO2, ldo_1p2_ldo_VCOBUF_vout_sel, 0xc);
		MOD_RADIO_ALLREG_20693(pi, TX_DAC_CFG1, DAC_scram_off, 1);

		if (pi->sromi->sr13_1p5v_cbuck) {
			MOD_RADIO_ALLREG_20693(pi, PMU_CFG1, wlpmu_vrefadj_cbuck, 0x5);
			PHY_TRACE(("wl%d: %s: setting wlpmu_vrefadj_cbuck=0x5",
				pi->sh->unit, __FUNCTION__));
		}

		if (CHSPEC_IS20(pi->radio_chanspec) && !PHY_AS_80P80(pi, pi->radio_chanspec)) {
			if (fc[pllcore] <= 2472) {
				if (fc[pllcore] == 2427) {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
					ldo_1p2_xtalldo1p2_ctl, 0xe);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x24);
				} else {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
					ldo_1p2_xtalldo1p2_ctl, 0xa);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x24);
				}
				if (pi->sromi->sr13_cck_spur_en == 1) {
					/* 2G cck spur reduction setting for 4366, core0 */
					MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG4, pllcore,
						wl_xtal_wlanrf_ctrl, 0x3);
					PHY_TRACE(("wl%d: %s, fc: %d, 2g cck spur reduction\n",
						pi->sh->unit, __FUNCTION__, fc[pllcore]));
				}
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG4,
					wl_xtal_wlanrf_ctrl, 0x3);
			} else if (fc[pllcore] >= 5180 && fc[pllcore] <= 5320) {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
				ldo_1p2_xtalldo1p2_ctl, 0xa);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x8);
			} else if (fc[pllcore] >= 5745 && fc[pllcore] <= 5825) {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
				ldo_1p2_xtalldo1p2_ctl, 0xa);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x8);
			} else {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
				ldo_1p2_xtalldo1p2_ctl, 0xe);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x24);
			}
		} else if (CHSPEC_IS40(pi->radio_chanspec) || CHSPEC_IS80(pi->radio_chanspec)) {
			if (fc[pllcore] >= 5745 && fc[pllcore] <= 5825) {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
				                          ldo_1p2_xtalldo1p2_ctl, 0xa);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x8);
			} else {
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1,
				                          ldo_1p2_xtalldo1p2_ctl, 0xe);
				MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
				MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x24);
			}
		} else {
			MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTALLDO1, ldo_1p2_xtalldo1p2_ctl, 0xe);
			MOD_RADIO_ALLPLLREG_20693(pi, PLL_XTAL_OVR1, ovr_xtal_core, 0x1);
			MOD_RADIO_ALLPLLREG_20693(pi, WL_XTAL_CFG2, wl_xtal_core, 0x24);
		}
	}

	if (RADIOMAJORREV(pi) == 3) {
		uint16 phymode = phy_get_phymode(pi);
		if (phymode == PHYMODE_3x3_1x1) {
			if (pllcore == 1) {
				wlc_phy_radio_tiny_vcocal_wave2(pi, 0, 4, 1, FALSE);
			}
		} else {
			if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
				wlc_phy_radio_tiny_vcocal_wave2(pi, 0, 2, 1, TRUE);
			} else if (CHSPEC_IS160(pi->radio_chanspec)) {
				ASSERT(0);
			} else {
				wlc_phy_radio_tiny_vcocal_wave2(pi, 0, 1, 1, TRUE);
			}
		}
	} else if (!((phy_get_phymode(pi) == PHYMODE_MIMO) && (pllcore == 1))) {
	/* Do a VCO cal after writing the tuning table regs */
	/* in mimo mode, vco cal needs to be done only for core 0 */
		wlc_phy_radio_tiny_vcocal(pi);
	}
}

static void
wlc_phy_radio20693_afeclkpath_setup(phy_info_t *pi, uint8 core,
	radio_20693_adc_modes_t adc_mode, uint8 direct_ctrl_en)
{
	uint8 pupd;
	uint16 mac_mode;
	pupd = (direct_ctrl_en == 1) ? 0 : 1;
	mac_mode = phy_get_phymode(pi);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (direct_ctrl_en == 0) {
		MOD_RADIO_REG_20693(pi, RFPLL_OVR1, core, ovr_rfpll_vco_12g_buf_pu, pupd);
		MOD_RADIO_REG_20693(pi, RFPLL_OVR1, core, ovr_rfpll_vco_buf_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR1, core, ovr_logencore_5g_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR2, core, ovr_logencore_5g_mimosrc_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR2, core, ovr_logencore_5g_mimodes_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR1, core, ovr_logencore_2g_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR2, core, ovr_logencore_2g_mimosrc_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_OVR2, core, ovr_logencore_2g_mimodes_pu, pupd);
		MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_12g_mimo_div2_pu, pupd);
		MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_6g_mimo_pu, pupd);

		/* set logen mimo src des pu */
		wlc_phy_radio20693_set_logen_mimosrcdes_pu(pi, core, adc_mode, mac_mode);
	}

	/* power up or down logen core based on band */
	wlc_phy_radio20693_set_logencore_pu(pi, adc_mode, core);
	/* Inside pll vco cell bufs 12G logen outputs at VCO freq */
	wlc_phy_radio20693_set_rfpll_vco_12g_buf_pu(pi, adc_mode, core);
	/* powerup afe clk div 12g sel */
	wlc_phy_radio20693_afeclkdiv_12g_sel(pi, adc_mode, core);
	/* power up afe clkdiv mimo blocks */
	wlc_phy_radio20693_afe_clk_div_mimoblk_pu(pi, core, adc_mode);
}

static void
wlc_phy_radio20693_set_logen_mimosrcdes_pu(phy_info_t *pi, uint8 core,
	radio_20693_adc_modes_t adc_mode, uint16 mac_mode)
{
	uint8 pupd;
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	pupd = (mac_mode == PHYMODE_MIMO) ? 1 : 0;

	/* band a */
	if (CHSPEC_IS5G(pi->radio_chanspec)) {
		if (core == 0) {
			MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimosrc_pu, pupd);
			MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimodes_pu, 0);
		} else {
			MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimosrc_pu, 0);
			MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimodes_pu, pupd);
		}

		MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_2g_mimosrc_pu, 0);
		MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_2g_mimodes_pu, 0);
	} else {
		/* band g */
		if (adc_mode == RADIO_20693_SLOW_ADC)  {
			if (core == 0) {
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimosrc_pu, pupd);
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimodes_pu, 0);
			} else {
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimosrc_pu, 0);
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimodes_pu, pupd);
			}
		} else {
			if (core == 0) {
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimosrc_pu, pupd);
			} else {
				MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
					logencore_2g_mimosrc_pu, 0);
			}

			MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core,
				logencore_2g_mimodes_pu, 0);
		}

		MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimosrc_pu, 0);
		MOD_RADIO_REG_20693(pi, LOGEN_CFG3, core, logencore_5g_mimodes_pu, 0);
	}
}

static void
wlc_phy_radio20693_adc_powerupdown(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 pupdbit, uint8 core)
{
	uint8 en_ovrride;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (pupdbit == 1) {
	    /* use direct control */
		en_ovrride = 0;
	} else {
	    /* to force powerdown */
		en_ovrride = 1;
	}
	FOREACH_CORE(pi, core) {
		switch (adc_mode) {
		case RADIO_20693_FAST_ADC:
	        MOD_RADIO_REG_20693(pi, ADC_OVR1, core, ovr_adc_clk_fast_pu, en_ovrride);
	        MOD_RADIO_REG_20693(pi, ADC_CFG15, core, adc_clk_fast_pu, pupdbit);
	        MOD_RADIO_REG_20693(pi, ADC_OVR1, core, ovr_adc_fast_pu, en_ovrride);
	        MOD_RADIO_REG_20693(pi, ADC_CFG1, core, adc_fast_pu, pupdbit);
			break;
		case RADIO_20693_SLOW_ADC:
	        MOD_RADIO_REG_20693(pi, ADC_OVR1, core, ovr_adc_clk_slow_pu, en_ovrride);
	        MOD_RADIO_REG_20693(pi, ADC_CFG15, core, adc_clk_slow_pu, pupdbit);
	        MOD_RADIO_REG_20693(pi, ADC_OVR1, core, ovr_adc_slow_pu, en_ovrride);
	        MOD_RADIO_REG_20693(pi, ADC_CFG1, core, adc_slow_pu, pupdbit);
			break;
	    default:
	        PHY_ERROR(("wl%d: %s: Wrong ADC mode \n",
	           pi->sh->unit, __FUNCTION__));
			ASSERT(0);
	        break;
	    }
	    MOD_RADIO_REG_20693(pi, TX_AFE_CFG1, core, afe_rst_clk, 0x0);
	}
}

static void
wlc_phy_radio20693_set_logencore_pu(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core)
{
	uint8 pupd = 0;
	bool mimo_core_idx_1;
	mimo_core_idx_1 = ((phy_get_phymode(pi) == PHYMODE_MIMO) && (core == 1)) ? 1 : 0;
	/* Based on the adcmode and band set the logencore_pu for 2G and 5G */
	if ((mimo_core_idx_1 == 1) &&
		(CHSPEC_IS2G(pi->radio_chanspec)) && (adc_mode == RADIO_20693_FAST_ADC)) {
		pupd = 1;
	} else if (mimo_core_idx_1 == 1) {
		pupd = 0;
	} else {
		pupd = 1;
	}

	MOD_RADIO_REG_20693(pi, LOGEN_OVR1, core, ovr_logencore_5g_pu, 1);
	MOD_RADIO_REG_20693(pi, LOGEN_OVR1, core, ovr_logencore_2g_pu, 1);

	if (CHSPEC_IS5G(pi->radio_chanspec)) {
		MOD_RADIO_REG_20693(pi, LOGEN_CFG2, core, logencore_5g_pu, pupd);
		MOD_RADIO_REG_20693(pi, LOGEN_CFG2, core, logencore_2g_pu, 0);
	} else {
		MOD_RADIO_REG_20693(pi, LOGEN_CFG2, core, logencore_5g_pu, 0);
		MOD_RADIO_REG_20693(pi, LOGEN_CFG2, core, logencore_2g_pu, pupd);
	}
}

static void
wlc_phy_radio20693_set_rfpll_vco_12g_buf_pu(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core)
{
	uint8 pupd = 0;
	bool mimo_core_idx_1;
	bool is_per_core_phy_bw80;
	mimo_core_idx_1 = ((phy_get_phymode(pi) == PHYMODE_MIMO) && (core == 1)) ? 1 : 0;
	is_per_core_phy_bw80 = (CHSPEC_IS80(pi->radio_chanspec) ||
		CHSPEC_IS8080(pi->radio_chanspec) || CHSPEC_IS160(pi->radio_chanspec));
	/* Based on the adcmode and band set the logencore_pu for 2G and 5G */
	if (mimo_core_idx_1 == 1) {
		pupd = 0;
	} else {
		pupd = 1;
	}
	if ((adc_mode == RADIO_20693_FAST_ADC) || (is_per_core_phy_bw80 == 1) ||
		(CHSPEC_IS5G(pi->radio_chanspec))) {
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_vco_12g_buf_pu, pupd);
	} else {
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_vco_12g_buf_pu, 0);
	}
	/* Feeds MMD and external clocks */
	MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_vco_buf_pu, pupd);
}

static void
wlc_phy_radio20693_afeclkdiv_12g_sel(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core)
{
	bool is_per_core_phy_bw80;
	const chan_info_radio20693_altclkplan_t *altclkpln = altclkpln_radio20693;
	int row;
	uint8 afeclkdiv;
	is_per_core_phy_bw80 = (CHSPEC_IS80(pi->radio_chanspec) ||
		CHSPEC_IS8080(pi->radio_chanspec) || CHSPEC_IS160(pi->radio_chanspec));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));
	if (ROUTER_4349(pi)) {
		altclkpln = altclkpln_radio20693_router4349;
	}
	row = wlc_phy_radio20693_altclkpln_get_chan_row(pi);
	if ((adc_mode == RADIO_20693_FAST_ADC) || (is_per_core_phy_bw80 == 1)) {
		/* 80Mhz  : afe_div = 3 */
		afeclkdiv = 0x4;
	} else {
		if (CHSPEC_IS20(pi->radio_chanspec)) {
			/* 20Mhz  : afe_div = 8 */
			afeclkdiv = 0x3;
		} else {
			/* 40Mhz  : afe_div = 4 */
			afeclkdiv = 0x2;
		}
	}
	if ((row >= 0) && (adc_mode != RADIO_20693_FAST_ADC)) {
		afeclkdiv = altclkpln[row].afeclkdiv;
	}
	MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core, afe_clk_div_12g_sel, afeclkdiv);
}

static void
wlc_phy_radio20693_afe_clk_div_mimoblk_pu(phy_info_t *pi, uint8 core,
	radio_20693_adc_modes_t adc_mode) {

	uint8 pupd = 0;
	bool mimo_core_idx_1;
	bool is_per_core_phy_bw80;
	mimo_core_idx_1 = ((phy_get_phymode(pi) == PHYMODE_MIMO) && (core == 1)) ? 1 : 0;
	is_per_core_phy_bw80 = (CHSPEC_IS80(pi->radio_chanspec) ||
		CHSPEC_IS8080(pi->radio_chanspec) || CHSPEC_IS160(pi->radio_chanspec));

	if (mimo_core_idx_1 == 1) {
		pupd = 1;
	} else {
		pupd = 0;
	}
	/* Enable the radio Ovrs */
	MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_6g_mimo_pu, 1);
	MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_12g_mimo_div2_pu, 1);
	MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_12g_mimo_pu, 1);
	if (CHSPEC_IS2G(pi->radio_chanspec))  {
		if (adc_mode == RADIO_20693_SLOW_ADC) {
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_12g_mimo_pu, 0);
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_12g_mimo_div2_pu, 0);
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_6g_mimo_pu, pupd);
		} else {
			if (wlapi_txbf_enab(pi->sh->physhim)) {
				MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
					core, afe_clk_div_12g_mimo_pu, 0);
			} else {
				MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
					core, afe_clk_div_12g_mimo_pu, pupd);
			}
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_12g_mimo_div2_pu, pupd);
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_6g_mimo_pu, 0);
		}
	} else if (CHSPEC_IS5G(pi->radio_chanspec))  {
		if ((adc_mode == RADIO_20693_SLOW_ADC) &&
			((is_per_core_phy_bw80 == 0))) {
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
				core, afe_clk_div_12g_mimo_pu, 0);
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core,
				afe_clk_div_12g_mimo_div2_pu, pupd);
		} else {
			if (wlapi_txbf_enab(pi->sh->physhim)) {
				MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1,
					core, afe_clk_div_12g_mimo_pu, 0);
			} else {
				MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core,
					afe_clk_div_12g_mimo_pu, pupd);
			}
			MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core,
				afe_clk_div_12g_mimo_div2_pu, 0);
			MOD_RADIO_REG_20693(pi, SPARE_CFG8, core,
				afe_BF_se_enb0, 1);
			MOD_RADIO_REG_20693(pi, SPARE_CFG8, core,
				afe_BF_se_enb1, 1);
		}
		MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core, afe_clk_div_6g_mimo_pu, 0);
	}
}

static void wlc_phy_radio20693_pll_tune(phy_info_t *pi, const void *chan_info_pll,
	uint8 core)
{
	const uint16 *val_ptr;
	uint16 *off_ptr;
	uint8 ct;
	const chan_info_radio20693_pll_t *ci20693_pll = chan_info_pll;

	uint16 pll_tune_reg_offsets_20693[] = {
		RADIO_REG_20693(pi, PLL_VCOCAL1, core),
		RADIO_REG_20693(pi, PLL_VCOCAL11, core),
		RADIO_REG_20693(pi, PLL_VCOCAL12, core),
		RADIO_REG_20693(pi, PLL_FRCT2, core),
		RADIO_REG_20693(pi, PLL_FRCT3, core),
		RADIO_REG_20693(pi, PLL_HVLDO1, core),
		RADIO_REG_20693(pi, PLL_LF4, core),
		RADIO_REG_20693(pi, PLL_LF5, core),
		RADIO_REG_20693(pi, PLL_LF7, core),
		RADIO_REG_20693(pi, PLL_LF2, core),
		RADIO_REG_20693(pi, PLL_LF3, core),
		RADIO_REG_20693(pi, SPARE_CFG1, core),
		RADIO_REG_20693(pi, SPARE_CFG14, core),
		RADIO_REG_20693(pi, SPARE_CFG13, core),
		RADIO_REG_20693(pi, TXMIX2G_CFG5, core),
		RADIO_REG_20693(pi, TXMIX5G_CFG6, core),
		RADIO_REG_20693(pi, PA5G_CFG4, core),
	};


	PHY_TRACE(("wl%d: %s\n core: %d Channel: %d\n", pi->sh->unit, __FUNCTION__,
		core, ci20693_pll->chan));

	off_ptr = &pll_tune_reg_offsets_20693[0];
	val_ptr = &ci20693_pll->pll_vcocal1;

	for (ct = 0; ct < ARRAYSIZE(pll_tune_reg_offsets_20693); ct++)
		phy_utils_write_radioreg(pi, off_ptr[ct], val_ptr[ct]);
}

static void wlc_phy_radio20693_pll_tune_wave2(phy_info_t *pi, const void *chan_info_pll,
	uint8 core)
{
	const uint16 *val_ptr;
	uint16 *off_ptr;
	uint8 ct, rdcore;
	const chan_info_radio20693_pll_wave2_t *ci20693_pll = chan_info_pll;

	uint16 pll_tune_reg_offsets_20693[] = {
		RADIO_PLLREG_20693(pi, WL_XTAL_CFG3, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL18, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL3, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL4, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL7, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL8, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL20, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL1, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL12, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL13, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL10, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL11, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL19, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL6, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL9, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL17, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL15, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL2, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL24, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL26, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL25, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL21, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL22, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL23, core),
		RADIO_PLLREG_20693(pi, PLL_VCO7, core),
		RADIO_PLLREG_20693(pi, PLL_VCO2, core),
		RADIO_PLLREG_20693(pi, PLL_FRCT2, core),
		RADIO_PLLREG_20693(pi, PLL_FRCT3, core),
		RADIO_PLLREG_20693(pi, PLL_VCO6, core),
		RADIO_PLLREG_20693(pi, PLL_VCO5, core),
		RADIO_PLLREG_20693(pi, PLL_VCO4, core),
		RADIO_PLLREG_20693(pi, PLL_LF4, core),
		RADIO_PLLREG_20693(pi, PLL_LF5, core),
		RADIO_PLLREG_20693(pi, PLL_LF7, core),
		RADIO_PLLREG_20693(pi, PLL_LF2, core),
		RADIO_PLLREG_20693(pi, PLL_LF3, core),
		RADIO_PLLREG_20693(pi, PLL_CP4, core),
		RADIO_PLLREG_20693(pi, PLL_VCOCAL5, core),
		RADIO_PLLREG_20693(pi, LO2G_LOGEN0_DRV, core),
		RADIO_PLLREG_20693(pi, LO2G_VCO_DRV_CFG2, core),
		RADIO_PLLREG_20693(pi, LO2G_LOGEN1_DRV, core),
		RADIO_PLLREG_20693(pi, LO5G_CORE0_CFG1, core),
		RADIO_PLLREG_20693(pi, LO5G_CORE1_CFG1, core),
		RADIO_PLLREG_20693(pi, LO5G_CORE2_CFG1, core),
		RADIO_PLLREG_20693(pi, LO5G_CORE3_CFG1, core),
	};

	uint8 ctbase = ARRAYSIZE(pll_tune_reg_offsets_20693);

	PHY_TRACE(("wl%d: %s\n pll_core: %d Channel: %d\n", pi->sh->unit, __FUNCTION__,
	        core, ci20693_pll->chan));

	off_ptr = &pll_tune_reg_offsets_20693[0];
	val_ptr = &ci20693_pll->wl_xtal_cfg3;

	for (ct = 0; ct < ARRAYSIZE(pll_tune_reg_offsets_20693); ct++) {
		phy_utils_write_radioreg(pi, off_ptr[ct], val_ptr[ct]);
	}

	if (ACMAJORREV_33(pi->pubpi->phy_rev) &&
			PHY_AS_80P80(pi, pi->radio_chanspec) &&
			(core == 1)) {
		FOREACH_CORE(pi, rdcore) {
		   uint16 radio_tune_reg_offsets_20693[] = {
				RADIO_REG_20693(pi, LNA2G_TUNE,			rdcore),
				RADIO_REG_20693(pi, LOGEN2G_RCCR,		rdcore),
				RADIO_REG_20693(pi, TXMIX2G_CFG5,		rdcore),
				RADIO_REG_20693(pi, PA2G_CFG2,			rdcore),
				RADIO_REG_20693(pi, TX_LOGEN2G_CFG1,	rdcore),
				RADIO_REG_20693(pi, LNA5G_TUNE,			rdcore),
				RADIO_REG_20693(pi, LOGEN5G_RCCR,		rdcore),
				RADIO_REG_20693(pi, TX5G_TUNE,			rdcore),
				RADIO_REG_20693(pi, TX_LOGEN5G_CFG1,	rdcore)
			};

			if (rdcore >= 2) {
				for (ct = 0; ct < ARRAYSIZE(radio_tune_reg_offsets_20693); ct++) {
					off_ptr = &radio_tune_reg_offsets_20693[0];
					phy_utils_write_radioreg(pi,
						off_ptr[ct], val_ptr[ctbase + ct]);
					PHY_INFORM(("wl%d: %s, %6x \n", pi->sh->unit, __FUNCTION__,
						phy_utils_read_radioreg(pi, off_ptr[ct])));
				}
			}
		}
	} else {
		uint16 radio_tune_allreg_offsets_20693[] = {
			RADIO_ALLREG_20693(pi, LNA2G_TUNE),
			RADIO_ALLREG_20693(pi, LOGEN2G_RCCR),
			RADIO_ALLREG_20693(pi, TXMIX2G_CFG5),
			RADIO_ALLREG_20693(pi, PA2G_CFG2),
			RADIO_ALLREG_20693(pi, TX_LOGEN2G_CFG1),
			RADIO_ALLREG_20693(pi, LNA5G_TUNE),
			RADIO_ALLREG_20693(pi, LOGEN5G_RCCR),
			RADIO_ALLREG_20693(pi, TX5G_TUNE),
			RADIO_ALLREG_20693(pi, TX_LOGEN5G_CFG1)
		};
		for (ct = 0; ct < ARRAYSIZE(radio_tune_allreg_offsets_20693); ct++) {
			off_ptr = &radio_tune_allreg_offsets_20693[0];
			phy_utils_write_radioreg(pi, off_ptr[ct], val_ptr[ctbase + ct]);
			PHY_INFORM(("wl%d: %s, %6x \n", pi->sh->unit, __FUNCTION__,
					phy_utils_read_radioreg(pi, off_ptr[ct])));
		}
	}
}

static void wlc_phy_radio20693_rffe_rsdb_wr(phy_info_t *pi, uint8 core, uint8 reg_type,
	uint16 reg_val, uint16 reg_off)
{
	uint8 access_info, write, is_crisscross_wr;
	access_info = wlc_phy_radio20693_tuning_get_access_info(pi, core, reg_type);
	write = (access_info >> RADIO20693_TUNE_REG_WR_SHIFT) & 1;
	is_crisscross_wr = (access_info >> RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT) & 1;

	PHY_TRACE(("wl%d: %s\n"
		"core: %d reg_type: %d reg_val: %x reg_off: %x write %d is_crisscross_wr %d\n",
		pi->sh->unit, __FUNCTION__,
		core, reg_type, reg_val, reg_off, write, is_crisscross_wr));

	if (write != 0) {
		/* switch base address to other core to access regs via criss-cross path */
		if (is_crisscross_wr == 1) {
			si_d11_switch_addrbase(pi->sh->sih, !core);
		}

		phy_utils_write_radioreg(pi, reg_off, reg_val);

		/* restore the based adress */
		if (is_crisscross_wr == 1) {
			si_d11_switch_addrbase(pi->sh->sih, core);
		}
	}
}
static void wlc_phy_radio20693_rffe_tune(phy_info_t *pi, const void *chan_info_rffe,
	uint8 core)
{
	const chan_info_radio20693_rffe_t *ci20693_rffe = chan_info_rffe;
	const uint16 *val_ptr;
	uint16 *off_ptr;
	uint8 ct;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint16 phymode = phy_get_phymode(pi);

	uint16 rffe_tune_reg_offsets_20693_cr0[RADIO20693_NUM_RFFE_TUNE_REGS] = {
		RADIO_REG_20693(pi, LNA2G_TUNE, 0),
		RADIO_REG_20693(pi, LNA5G_TUNE, 0),
		RADIO_REG_20693(pi, PA2G_CFG2,  0)
	};
	uint16 rffe_tune_reg_offsets_20693_cr1[RADIO20693_NUM_RFFE_TUNE_REGS] = {
		RADIO_REG_20693(pi, LNA2G_TUNE, 1),
		RADIO_REG_20693(pi, LNA5G_TUNE, 1),
		RADIO_REG_20693(pi, PA2G_CFG2,  1)
	};

	PHY_TRACE(("wl%d: %s core: %d crisscross_actv: %d\n", pi->sh->unit, __FUNCTION__, core,
		pi_ac->radioi->is_crisscross_actv));

	val_ptr = &ci20693_rffe->lna2g_tune;
	if ((pi_ac->radioi->is_crisscross_actv == 0) || (phymode == PHYMODE_MIMO)) {
		/* crisscross is disabled => its a 2 antenna board.
		in the case of RSDB mode:
			this proc gets called during each core init and hence
			just writing to current core would suffice.
		in the case of 80p80 mode:
			this proc gets called twice since fc is a two
			channel list and hence just writing to current core would suffice.
		in the case of MIMO mode:
			it should be a 2 antenna board.
		so, just writing to $core would take care of all the use case.
		 */
		off_ptr = (core == 0) ? &rffe_tune_reg_offsets_20693_cr0[0] :
			&rffe_tune_reg_offsets_20693_cr1[0];
		for (ct = 0; ct < RADIO20693_NUM_RFFE_TUNE_REGS; ct++)
			phy_utils_write_radioreg(pi, off_ptr[ct], val_ptr[ct]);
	} else {
		/* crisscross is active */
		if (phymode == PHYMODE_RSDB) {
			const uint8 *map_ptr = &rffe_tune_reg_map_20693[0];
			off_ptr = &rffe_tune_reg_offsets_20693_cr0[0];
			for (ct = 0; ct < RADIO20693_NUM_RFFE_TUNE_REGS; ct++) {
				wlc_phy_radio20693_rffe_rsdb_wr(pi, phy_get_current_core(pi),
					map_ptr[ct], val_ptr[ct], off_ptr[ct]);
			}
		} else if (phymode == PHYMODE_80P80) {
			uint8 access_info, write, is_crisscross_wr;

			access_info = wlc_phy_radio20693_tuning_get_access_info(pi, core, 0);
			write = (access_info >> RADIO20693_TUNE_REG_WR_SHIFT) & 1;
			is_crisscross_wr = (access_info >>
				RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT) & 1;

			PHY_TRACE(("wl%d: %s\n write %d is_crisscross_wr %d\n",
				pi->sh->unit, __FUNCTION__,	write, is_crisscross_wr));

			if (write != 0) {
				core = (is_crisscross_wr == 0) ? core : !core;
				off_ptr = (core == 0) ? &rffe_tune_reg_offsets_20693_cr0[0] :
					&rffe_tune_reg_offsets_20693_cr1[0];

				for (ct = 0; ct < RADIO20693_NUM_RFFE_TUNE_REGS; ct++) {
					phy_utils_write_radioreg(pi, off_ptr[ct], val_ptr[ct]);
				}
			} /* Write != 0 */
		} /* 80P80 */
	} /* Criss-Cross Active */
	if (ACMAJORREV_4(pi->pubpi->phy_rev) && (!(PHY_IPA(pi))) &&
		(RADIO20693REV(pi->pubpi->radiorev) == 13) && CHSPEC_IS2G(pi->radio_chanspec)) {
		/* 4349A2 TX2G die-2/rev-13 iPA used as ePA */
		wlc_phy_radio20693_tx2g_set_freq_tuning_ipa_as_epa(pi, core,
			CHSPEC_CHANNEL(pi->radio_chanspec));
	}
}

/* This function return 2bits.
bit 0: 0 means donot write. 1 means write
bit 1: 0 means direct access. 1 means criss-cross access
*/
static uint8 wlc_phy_radio20693_tuning_get_access_info(phy_info_t *pi, uint8 core, uint8 reg_type)
{
	uint16 phymode = phy_get_phymode(pi);
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 access_info = 0;

	ASSERT(phymode != PHYMODE_MIMO);

	PHY_TRACE(("wl%d: %s core: %d phymode: %d\n", pi->sh->unit, __FUNCTION__, core, phymode));

	if (phymode == PHYMODE_80P80) {
		if (core != pi_ac->radioi->crisscross_priority_core_80p80) {
			access_info = 0;
		} else {
			access_info = (1 << RADIO20693_TUNE_REG_WR_SHIFT);
			/* figure out which core has 5G affinity */
			if (RADIO20693_CORE1_AFFINITY == 5) {
				access_info |= (core == 1) ? 0 :
					(1 << RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT);
			} else {
				access_info |= (core == 0) ? 0 :
					(1 << RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT);
			}
		}
	} else if (phymode == PHYMODE_RSDB)	{
		uint8 band_curr_cr, band_oth_cr;
		phy_info_t *other_pi = phy_get_other_pi(pi);

		band_curr_cr = CHSPEC_IS2G(pi->radio_chanspec) ? RADIO20693_TUNEREG_TYPE_2G :
			RADIO20693_TUNEREG_TYPE_5G;
		band_oth_cr = CHSPEC_IS2G(other_pi->radio_chanspec) ? RADIO20693_TUNEREG_TYPE_2G :
			RADIO20693_TUNEREG_TYPE_5G;

		PHY_TRACE(("wl%d: %s current_core: %d other_core: %d\n", pi->sh->unit, __FUNCTION__,
			core, !core));
		PHY_TRACE(("wl%d: %s band_curr_cr: %d band_oth_cr: %d\n", pi->sh->unit,
			__FUNCTION__, band_curr_cr, band_oth_cr));

		/* if a tuning reg is neither a 2G reg nor a 5g reg,
		   then assume this register belongs to band of current core
		 */
		if (reg_type == RADIO20693_TUNEREG_TYPE_NONE) {
			reg_type = band_curr_cr;
		}

		/* Do not write anything if reg_type and band_curr_cr are different */
		if (reg_type != band_curr_cr) {
			access_info = 0;
			PHY_TRACE(("w%d: %s. IGNORE: reg_type and band_curr_cr are different\n",
				pi->sh->unit, __FUNCTION__));
			return (access_info);
		}

		/* find out if there is a conflict of bands between two cores.
		 if 	--> no conflict case
		 else 	--> Conflict case
		 */
		if (band_curr_cr != band_oth_cr) {
			PHY_TRACE(("wl%d: %s. No conflict case. bands are different\n",
				pi->sh->unit, __FUNCTION__));

			/* find out if its a direct access or criss-cross_access */
			access_info = wlc_phy_radio20693_tuning_get_access_type(pi, core, reg_type);
		} else {
			PHY_TRACE(("wl%d: %s. Conflict case. bands are same\n",
				pi->sh->unit, __FUNCTION__));
			if (core != pi_ac->radioi->crisscross_priority_core_rsdb) {
				PHY_TRACE(("wl%d: %s IGNORE: priority is not met\n",
					pi->sh->unit, __FUNCTION__));

				PHY_TRACE(("wl%d: %s current_core: %d Priority: %d\n",
					pi->sh->unit, __FUNCTION__, core,
					pi_ac->radioi->crisscross_priority_core_rsdb));

				access_info = 0;
			}

			/* find out if its a direct access or criss-cross_access */
			access_info = wlc_phy_radio20693_tuning_get_access_type(pi, core, reg_type);
		}
	}

	PHY_TRACE(("wl%d: %s. Access Info %x\n",
		pi->sh->unit, __FUNCTION__, access_info));

	return (access_info);
}
static uint8 wlc_phy_radio20693_tuning_get_access_type(phy_info_t *pi, uint8 core, uint8 reg_type)
{
	uint8 is_dir_access, access_info;

	/* find out if its a direct access or criss-cross_access */
	is_dir_access = ((core == 0) && (reg_type == RADIO20693_CORE0_AFFINITY));
	is_dir_access |= ((core == 1) && (reg_type == RADIO20693_CORE1_AFFINITY));

	/* its a direct access */
	access_info = (1 << RADIO20693_TUNE_REG_WR_SHIFT);

	if (is_dir_access == 0) {
		/* Its a criss-cross access */
		access_info |= (1 << RADIO20693_TUNE_REG_CRISSCROSS_WR_SHIFT);
	}

	return (access_info);
}

/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */

/* ********************************************* */
/*				Internal Definitions					*/
/* ********************************************* */

static void
wlc_phy_radio20693_minipmu_pwron_seq(phy_info_t *pi)
{
	uint8 core;
	/* Power up the TX LDO */
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_en, 0x1);
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_TXldo_pu, 1);
	}
	OSL_DELAY(110);
	/* Power up the radio Band gap and the 2.5V reg */
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, VREG_CFG, core, vreg25_pu, 1);
		if (RADIOMAJORREV(pi) == 3) {
			MOD_RADIO_PLLREG_20693(pi, BG_CFG1, core, bg_pu, 1);
		} else {
			MOD_RADIO_REG_TINY(pi, BG_CFG1, core, bg_pu, 1);
		}
	}
	OSL_DELAY(10);
	/* Power up the VCO, AFE, RX & ADC Ldo's */
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_VCOldo_pu, 1);
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_AFEldo_pu, 1);
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_RXldo_pu, 1);
		MOD_RADIO_REG_TINY(pi, PMU_CFG4, core, wlpmu_ADCldo_pu, 1);
	}
	OSL_DELAY(12);
	/* Enable the Synth and Force the wlpmu_spare to 0 */
	if (RADIOMAJORREV(pi) != 3) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_TINY(pi, PLL_CFG1, core, synth_pwrsw_en, 1);
			MOD_RADIO_REG_TINY(pi, PMU_CFG5, core, wlpmu_spare, 0);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
/* PU the TX/RX/VCO/..LDO's */
static void wlc_phy_radio20696_minipmu_pwron_seq(phy_info_t *pi)
{
	uint8 core;

	/* Turn on two 1p8LDOs and Bandgap  */
	MOD_RADIO_REG_20696(pi, BG_OVR1, 1, ovr_bg_pu_bgcore, 0x1);
	MOD_RADIO_REG_20696(pi, LDO1P8_STAT, 1, ldo1p8_pu, 0x1);
	MOD_RADIO_REG_20696(pi, LDO1P8_STAT, 3, ldo1p8_pu, 0x1);
	MOD_RADIO_REG_20696(pi, BG_REG2, 1, bg_pu_bgcore, 0x1);
	MOD_RADIO_REG_20696(pi, BG_REG2, 1, bg_pu_V2I, 0x1);

	/* Hold RF PLL in reset */
	MOD_RADIO_PLLREG_20696(pi, PLL_VCOCAL_OVR1, ovr_rfpll_vcocal_rst_n, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_CFG2, rfpll_rst_n, 0x0);
	MOD_RADIO_PLLREG_20696(pi, PLL_OVR1, ovr_rfpll_rst_n, 0x1);
	MOD_RADIO_PLLREG_20696(pi, PLL_VCOCAL1, rfpll_vcocal_rst_n, 0x0);

	FOREACH_CORE(pi, core) {
		/* Overwrites */
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_ADCldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_AFEldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_LDO2P1_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_LOGENldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_TXldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_OVR1, core, ovr_wlpmu_en, 0x1);

		MOD_RADIO_REG_20696(pi, RX2G_REG4, core, rx_ldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, LDO1P65_STAT, core, wlpmu_ldo1p6_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG1, core, wlpmu_ADCldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG1, core, wlpmu_AFEldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG4, core, wlpmu_LDO2P1_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG1, core, wlpmu_LOGENldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG1, core, wlpmu_TXldo_pu, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG4, core, wlpmu_en, 0x1);
		MOD_RADIO_REG_20696(pi, PMU_CFG3, core, vref_select, 0x0);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void wlc_phy_radio20693_pmu_override(phy_info_t *pi)
{
	uint8  core;
	uint16 data;

	FOREACH_CORE(pi, core) {
		data = READ_RADIO_REG_20693(pi, PMU_OVR, core);
		data = data | 0xFC;
		phy_utils_write_radioreg(pi, RADIO_REG_20693(pi, PMU_OVR, core), data);
	}
}

static void
wlc_phy_radio20693_xtal_pwrup(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	/* Note: Core0 XTAL will be powerup by PMUcore in chipcommon */
	if ((phy_get_phymode(pi) == PHYMODE_RSDB) &&
		((phy_get_current_core(pi) == PHY_RSBD_PI_IDX_CORE1))) {
		/* Powerup xtal ldo for MAC_Core = 1 in case of RSDB mode */
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 0,
				ldo_1p2_xtalldo1p2_vref_bias_reset, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTAL_OVR1, 0,
				ovr_ldo_1p2_xtalldo1p2_vref_bias_reset, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_BG_pu, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTAL_OVR1, 0,
				ovr_ldo_1p2_xtalldo1p2_BG_pu, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
		OSL_DELAY(300);
		MOD_RADIO_REG_20693(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_vref_bias_reset, 0);
	} else if ((phy_get_phymode(pi) != PHYMODE_RSDB)) {
		/* Powerup xtal ldo for Core 1 in case of MIMO and 80+80 */
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 1,
				ldo_1p2_xtalldo1p2_vref_bias_reset, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTAL_OVR1, 1,
				ovr_ldo_1p2_xtalldo1p2_vref_bias_reset, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 1, ldo_1p2_xtalldo1p2_BG_pu, 1)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTAL_OVR1, 1,
				ovr_ldo_1p2_xtalldo1p2_BG_pu, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
		OSL_DELAY(300);
		MOD_RADIO_REG_20693(pi, PLL_XTALLDO1, 1, ldo_1p2_xtalldo1p2_vref_bias_reset, 0);
	}
}

static void
wlc_phy_radio20693_upd_prfd_values(phy_info_t *pi)
{
	uint i = 0;
	const radio_20xx_prefregs_t *prefregs_20693_ptr = NULL;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	/* Choose the right table to use */
	switch (RADIO20693REV(pi->pubpi->radiorev)) {
	case 3:
	case 4:
	case 5:
		prefregs_20693_ptr = prefregs_20693_rev5;
		break;
	case 6:
		prefregs_20693_ptr = prefregs_20693_rev6;
		break;
	case 7:
		prefregs_20693_ptr = prefregs_20693_rev5;
		break;
	case 8:
		prefregs_20693_ptr = prefregs_20693_rev6;
		break;
	case 10:
		prefregs_20693_ptr = prefregs_20693_rev10;
		break;
	case 11:
	case 12:
	case 13:
		prefregs_20693_ptr = prefregs_20693_rev13;
		break;
	case 14:
		prefregs_20693_ptr = prefregs_20693_rev14;
		break;
	case 19:
		prefregs_20693_ptr = prefregs_20693_rev19;
		break;
	case 15:
		if (PHY_XTAL_IS37M4(pi)) {
			prefregs_20693_ptr = prefregs_20693_rev15_37p4MHz;
		} else {
			prefregs_20693_ptr = prefregs_20693_rev15_40MHz;
		}
		break;
	case 16:
	case 17:
	case 20:
		prefregs_20693_ptr = prefregs_20693_rev5;
		break;
	case 18:
	case 21:
		if (PHY_XTAL_IS37M4(pi)) {
			prefregs_20693_ptr = prefregs_20693_rev18;
		} else {
			prefregs_20693_ptr = prefregs_20693_rev15_40MHz;
		}
		break;
	case 23:
		/* Preferred values for 53573/53574/47189-B0 */
		prefregs_20693_ptr = prefregs_20693_rev23_40MHz;
		break;
	case 32:
	case 33:
		prefregs_20693_ptr = prefregs_20693_rev32;
		break;
	case 9:
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIO20693REV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return;
	}

	/* Update preferred values */
	while (prefregs_20693_ptr[i].address != 0xffff) {
		if (!((phy_get_phymode(pi) == PHYMODE_RSDB) &&
		(prefregs_20693_ptr[i].address & JTAG_20693_CR1)))
			phy_utils_write_radioreg(pi, prefregs_20693_ptr[i].address,
				(uint16)prefregs_20693_ptr[i].init);
		i++;
	}
}

void
wlc_phy_radio20693_vco_opt(phy_info_t *pi, bool isVCOTUNE_5G)
{
	uint8 core;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	PHY_TRACE(("%s\n", __FUNCTION__));
	if (pi_ac->radioi->lpmode_2g != ACPHY_LPMODE_NONE) {
		if (CHSPEC_IS2G(pi->radio_chanspec) && (PHY_IPA(pi))) {
			FOREACH_CORE(pi, core) {
				MOD_RADIO_REG_20693(pi, PLL_VCO3, core,
					rfpll_vco_en_alc, 0x1);
				MOD_RADIO_REG_20693(pi, PLL_VCO6, core,
					rfpll_vco_ALC_ref_ctrl, 0x8);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO2, core,
					ldo_2p5_lowquiescenten_CP, 0x1);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO2, core,
					ldo_2p5_lowquiescenten_VCO, 0x1);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO4, core,
					ldo_2p5_static_load_CP, 0x1);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO4, core,
					ldo_2p5_static_load_VCO, 0x1);
			}
		} else {
			FOREACH_CORE(pi, core) {
				MOD_RADIO_REG_20693(pi, PLL_VCO3, core,
					rfpll_vco_en_alc, 0x0);
				MOD_RADIO_REG_20693(pi, PLL_VCO6, core,
					rfpll_vco_ALC_ref_ctrl, 0x8);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO2, core,
					ldo_2p5_lowquiescenten_CP, 0x0);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO2, core,
					ldo_2p5_lowquiescenten_VCO, 0x0);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO4, core,
					ldo_2p5_static_load_CP, 0x0);
				MOD_RADIO_REG_20693(pi, PLL_HVLDO4, core,
					ldo_2p5_static_load_VCO, 0x0);
			}
		}
	}
	/* Loading vcotune settings */
	if (ROUTER_4349(pi)) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20693(pi, PLL_VCO3, core,
				rfpll_vco_en_alc, isVCOTUNE_5G ? 0x1 : 0x0);
			MOD_RADIO_REG_20693(pi, PLL_VCO6, core,
				rfpll_vco_ALC_ref_ctrl, isVCOTUNE_5G ? 0x6 : 0x0);
			MOD_RADIO_REG_20693(pi, PMU_CFG2, core,
				wlpmu_VCOldo_adj, isVCOTUNE_5G ? 0x3 : 0x0);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

int8
wlc_phy_tiny_radio_minipmu_cal(phy_info_t *pi)
{
	uint8 core;
	int8 cal_status = 1;
	phy_info_t *pi_core0 = phy_get_pi(pi, PHY_RSBD_PI_IDX_CORE0);
	bool is_rsdb_core1_cal = FALSE;

	if (BCM53573_CHIP(pi->sh->chip) && (CHIPREV((pi)->sh->chiprev) == 0)) {
		return 0;
	}

	ASSERT(TINY_RADIO(pi));
	/* Skip this function for QT */
	if (ISSIM_ENAB(pi->sh->sih))
		return 0;

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) &&
		(phy_get_phymode(pi) == PHYMODE_RSDB) &&
		(phy_get_current_core(pi) == PHY_RSBD_PI_IDX_CORE1) &&
		(RADIO20693REV(pi->pubpi->radiorev) <= 0x17) &&
		(RADIO20693REV(pi->pubpi->radiorev) != 0x13)) {
		 // this fix is not needed from 4355B1 variants
		is_rsdb_core1_cal = TRUE;
		wlc_phy_cals_mac_susp_en_other_cr(pi, TRUE);
	}

	/* Setup the cal */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_VCOldo_pu, 0);
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_AFEldo_pu, 0);
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_RXldo_pu, 0);
			MOD_RADIO_REG_TINY(pi, PMU_CFG4, core, wlpmu_ADCldo_pu, 0);
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

	FOREACH_CORE(pi, core) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
		if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
			MOD_RADIO_REG_TINY(pi, PMU_CFG3, core, wlpmu_selavg, 2);
		} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
		if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
			if (RADIOMAJORREV(pi) == 2) {
				MOD_RADIO_REG_20693(pi, PMU_CFG3, core, wlpmu_selavg_lo, 0);
			} else {
				MOD_RADIO_REG_20693(pi, PMU_CFG3, core, wlpmu_selavg, 0);
			}
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_vref_select, 1);
		MOD_RADIO_REG_TINY(pi, PMU_CFG2, core, wlpmu_bypcal, 0);
		MOD_RADIO_REG_TINY(pi, PMU_STAT, core, wlpmu_ldobg_cal_clken, 0);
	}
	OSL_DELAY(100);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_ldoref_start_cal, 1);
	}
	OSL_DELAY(100);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_ldoref_start_cal, 0);
	}
	OSL_DELAY(100);
	FOREACH_CORE(pi, core) {
		/* ldobg_cal_clken coming out from radiodig_xxx_core1 is not used */
		if (is_rsdb_core1_cal) {
			MOD_RADIO_REG_TINY(pi_core0, PMU_STAT, core, wlpmu_ldobg_cal_clken, 1);
		} else {
			MOD_RADIO_REG_TINY(pi, PMU_STAT, core, wlpmu_ldobg_cal_clken, 1);
		}
	}
	OSL_DELAY(100);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_ldoref_start_cal, 1);
	}
	FOREACH_CORE(pi, core) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		/* Wait for cal_done */
		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
			ACMAJORREV_33(pi->pubpi->phy_rev)) {
			uint8 calsuccesful = 1;
			int16 waitcounter = 0;
			while (READ_RADIO_REGFLD_TINY(pi, PMU_STAT, core,
				wlpmu_ldobg_cal_done) == 0) {
				OSL_DELAY(100);
				waitcounter ++;
				if (waitcounter > 100) {
					/* cal_done bit is not 1 even after waiting for a while */
					/* Exit gracefully */
					PHY_TRACE(("\nWarning:Mini PMU Cal Failed on Core%d \n",
						core));
					calsuccesful = 0;
					break;
				}
			}
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_ldoref_start_cal, 0);
			if (calsuccesful == 1) {
				PHY_TRACE(("MiniPMU cal done on core %d, Cal code = %d", core,
					READ_RADIO_REGFLD_TINY(pi, PMU_STAT, core, wlpmu_calcode)));
			} else {
				PHY_TRACE(("Cal Unsuccesful on Core %d", core));
				cal_status = -1;
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
		{
			SPINWAIT(READ_RADIO_REGFLD_TINY(pi, PMU_STAT, core,
				wlpmu_ldobg_cal_done) == 0, ACPHY_SPINWAIT_MINIPMU_CAL_STATUS);
			if (READ_RADIO_REGFLD_TINY(pi, PMU_STAT, core, wlpmu_ldobg_cal_done) == 0) {
				PHY_ERROR(("%s : Cal Unsuccesful on Core %d\n", __FUNCTION__,
					core));
				cal_status = -1;
				PHY_FATAL_ERROR_MESG(
					(" %s: SPINWAIT ERROR : PMU cal failed on Core%d\n",
					__FUNCTION__, core));
				PHY_FATAL_ERROR(pi, PHY_RC_PMUCAL_FAILED);
			}
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_ldoref_start_cal, 0);
			PHY_TRACE(("MiniPMU cal done on core %d, Cal code = %d\n", core,
				READ_RADIO_REGFLD_TINY(pi, PMU_STAT, core, wlpmu_calcode)));
		}
	}
	FOREACH_CORE(pi, core) {
		if (is_rsdb_core1_cal) {
			MOD_RADIO_REG_TINY(pi_core0, PMU_STAT, core, wlpmu_ldobg_cal_clken, 0);
		} else {
			MOD_RADIO_REG_TINY(pi, PMU_STAT, core, wlpmu_ldobg_cal_clken, 0);
		}
	}
	if (is_rsdb_core1_cal) {
		wlc_phy_cals_mac_susp_en_other_cr(pi, FALSE);
	}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_VCOldo_pu, 1);
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_AFEldo_pu, 1);
			MOD_RADIO_REG_TINY(pi, PMU_OP, core, wlpmu_RXldo_pu, 1);
			MOD_RADIO_REG_TINY(pi, PMU_CFG4, core, wlpmu_ADCldo_pu, 1);
		}
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

	return cal_status;
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio20693_pmu_pll_config(phy_info_t *pi)
{
	uint8 core;
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	/* # powerup pll */
	FOREACH_CORE(pi, core) {
		if ((phy_get_phymode(pi) == PHYMODE_MIMO) &&
			(core != 0))
			break;

		/* # VCO */
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_vco_pu, 1);
		/* # VCO LDO */
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_VCO, 1);
		/* # VCO buf */
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_vco_buf_pu, 1);
		/* # Synth global */
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_synth_pu, 1);
		/* # Charge Pump */
		MOD_RADIO_REG_20693(pi, PLL_CP1, core, rfpll_cp_pu, 1);
		/* # Charge Pump LDO */
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_CP, 1);
		/* # PLL lock monitor */
		MOD_RADIO_REG_20693(pi, PLL_CFG1, core, rfpll_monitor_pu, 1);
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_bias_reset_CP, 1);
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_bias_reset_VCO, 1);
	}
	OSL_DELAY(89);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_bias_reset_CP, 0);
		MOD_RADIO_REG_20693(pi, PLL_HVLDO1, core, ldo_2p5_bias_reset_VCO, 0);
	}
}

void
phy_ac_radio_20693_pmu_pll_config_wave2(phy_info_t *pi, uint8 pll_mode)
{
	// PLL/VCO operating modes: set by pll_mode
	// Mode 0. RFP0 non-coupled, e.g. 4x4 MIMO non-1024QAM
	// Mode 1. RFP0 coupled, e.g. 4x4 MIMO 1024QAM
	// Mode 2. RFP0 non-coupled + RFP1 non-coupled: 2x2 + 2x2 MIMO non-1024QAM
	// Mode 3. RFP0 non-coupled + RFP1 coupled: 3x3 + 1x1 scanning in 80MHz mode
	// Mode 4. RFP0 coupled + RFP1 non-coupled: 3x3 MIMO 1024QAM + 1x1 scanning in 20MHz mode
	// Mode 5. RFP0 coupled + RFP1 coupled: 3x3 MIMO 1024QAM + 1x1 scanning in 80MHz mode
	// Mode 6. RFP1 non-coupled
	// Mode 7. RFP1 coupled

	uint8 core, start_core, end_core;
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	PHY_INFORM(("wl%d: %s: pll_mode %d\n",
			pi->sh->unit, __FUNCTION__, pll_mode));

	switch (pll_mode) {
	case 0:
	        // intentional fall through
	case 1:
	        start_core = 0;
		end_core   = 0;
		break;
	case 2:
	        // intentional fall through
	case 3:
	        // intentional fall through
	case 4:
	        // intentional fall through
	case 5:
	        start_core = 0;
		end_core   = 1;
		break;
	case 6:
	        // intentional fall through
	case 7:
	        start_core = 1;
		end_core   = 1;
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported PLL/VCO operating mode %d\n",
			pi->sh->unit, __FUNCTION__, pll_mode));
		ASSERT(0);
		return;
	}

	MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG1, 0, wl_xtal_out_pu, 0x3f);

	for (core = start_core; core <= end_core; core++) {
		uint8  ct;
		// Put all regs/fields write/modification in a array
		uint16 pll_regs_bit_vals1[][3] = {
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_rfpll_synth_pu, 1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_rfpll_cp_pu,    1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_rfpll_vco_pu,   1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_rfpll_vco_buf_pu,   1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_ldo_2p5_pu_ldo_CP,  1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_ldo_2p5_pu_ldo_VCO, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG1, core, rfpll_synth_pu,   1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CP1,  core, rfpll_cp_pu,      1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG1, core, rfpll_vco_pu,     1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG1, core, rfpll_vco_buf_pu, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG1, core, rfpll_monitor_pu, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_LF6,  core, rfpll_lf_cm_pu,   1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_CP,  1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_VCO, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG1,   core, rfpll_pfd_en, 0x2),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core, ovr_rfpll_rst_n, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_VCOCAL_OVR1, core,
			ovr_rfpll_vcocal_rst_n, 1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core,
			ovr_rfpll_ldo_cp_bias_reset,  1),
			RADIO_PLLREGC_FLD_20693(pi, RFPLL_OVR1, core,
			ovr_rfpll_ldo_vco_bias_reset, 1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG2,    core, rfpll_rst_n,            0),
			RADIO_PLLREGC_FLD_20693(pi, PLL_VCOCAL1, core, rfpll_vcocal_rst_n,     0),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1,  core, ldo_2p5_bias_reset_CP,  1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1,  core, ldo_2p5_bias_reset_VCO, 1),
		};
		uint16 pll_regs_bit_vals2[][3] = {
			RADIO_PLLREGC_FLD_20693(pi, PLL_CFG2,    core, rfpll_rst_n,            1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_VCOCAL1, core, rfpll_vcocal_rst_n,     1),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1,  core, ldo_2p5_bias_reset_CP,  0),
			RADIO_PLLREGC_FLD_20693(pi, PLL_HVLDO1,  core, ldo_2p5_bias_reset_VCO, 0),
		};

		// now write/modification to radio regs
		for (ct = 0; ct < ARRAYSIZE(pll_regs_bit_vals1); ct++) {
			phy_utils_mod_radioreg(pi, pll_regs_bit_vals1[ct][0],
			                       pll_regs_bit_vals1[ct][1],
			                       pll_regs_bit_vals1[ct][2]);
		}
		OSL_DELAY(10);

		for (ct = 0; ct < ARRAYSIZE(pll_regs_bit_vals2); ct++) {
			phy_utils_mod_radioreg(pi, pll_regs_bit_vals2[ct][0],
			                       pll_regs_bit_vals2[ct][1],
			                       pll_regs_bit_vals2[ct][2]);
		}
	}
}

static void
wlc_phy_switch_radio_acphy_20693(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	/* minipmu_cal */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3)
		wlc_phy_tiny_radio_minipmu_cal(pi);

	/* r cal */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
	  wlc_phy_radio_tiny_rcal_wave2(pi, 2);
	}

	/* power up pmu pll */
	if (RADIOMAJORREV(pi) == 3) {
		if (phy_get_phymode(pi) == PHYMODE_80P80 ||
				PHY_AS_80P80(pi, pi->radio_chanspec)) {
				/* for 4365 C0 - turn on both PLL */
			phy_ac_radio_20693_pmu_pll_config_wave2(pi, 2);
		} else {
			phy_ac_radio_20693_pmu_pll_config_wave2(pi, 0);
		}
	} else {
		wlc_phy_radio20693_pmu_pll_config(pi);
	}

	if (!(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3)) {
		/* minipmu_cal */
		wlc_phy_tiny_radio_minipmu_cal(pi);

	/* RCAL */
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			if ((pi->sromi->rcal_otp_val >= 2) &&
				(pi->sromi->rcal_otp_val <= 12)) {
				pi->rcal_value = pi->sromi->rcal_otp_val;
			}
		}
		wlc_phy_radio_tiny_rcal(pi, TINY_RCAL_MODE1_STATIC);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

static void
wlc_phy_switch_radio_acphy(phy_info_t *pi, bool on)
{
	uint16 data = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
#ifdef LOW_TX_CURRENT_SETTINGS_2G
	uint8 is_ipa = 0;
#endif
	uint8 core;

	core = 0;
	PHY_TRACE(("wl%d: %s %s corenum %d\n", pi->sh->unit, __FUNCTION__, on ? "ON" : "OFF",
		pi->pubpi->phy_corenum));

	if (on) {
		if (!pi->radio_is_on) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
			if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
				if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
					/*  set the low pwoer reg before radio init */
					wlc_phy_set_lowpwr_phy_reg(pi);
					wlc_phy_radio2069_mini_pwron_seq_rev16(pi);
				} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
					/*  set the low pwoer reg before radio init */
					wlc_phy_set_lowpwr_phy_reg_rev3(pi);
					wlc_phy_radio2069_mini_pwron_seq_rev32(pi);
				}
				if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 0) &&
						((RADIO2069_MINORREV(pi->pubpi->radiorev) == 16) ||
						(RADIO2069_MINORREV(pi->pubpi->radiorev) == 17))) {
				   /* ToUnblock the QT tests on 4364-3x3 flavor added the */
				   /* reg config. Based on radio team reccomendatation */
				   /* to be reverted after proper settings */
				   MOD_RADIO_REG(pi, RFP, GP_REGISTER, gp_pcie, 3);
				}

				wlc_phy_radio2069_pwron_seq(pi);
				/* 4364 dual phy. when 3x3 is powered down, make sure the 1x1 */
				/* femctrl lines do not toggle ant0/ant1 this is done by giving */
				/* bt the control on ant0/ant1. If bt is not there (bt_reg_on 0) */
				/* this is fine. Ultimately bt firmware needs to write proper */
				/* values in the bt_clb_upi registers bt2clb_swctrl_smask_bt_antX */
				if (IS_4364_1x1(pi) || IS_4364_3x3(pi)) {
					si_gci_set_femctrl_mask_ant01(pi->sh->sih, pi->sh->osh, 1);
				}
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
				wlc_phy_tiny_radio_pwron_seq(pi);
				wlc_phy_switch_radio_acphy_20691(pi);
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
				acphy_pmu_core1_off_radregs_t *porig =
					(pi_ac->radioi->pmu_c1_off_info_orig);
				acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig =
					(pi_ac->radioi->pmu_lp_opt_orig);

				porig->is_orig = FALSE;
				pmu_lp_opt_orig->is_orig = FALSE;

				/* ------------------------------------------------------------- */
				/* In case of 4349A0, there will be 2 XTALs. Core0 XTAL will be  */
				/* Pwrdup by PMU and Core1 XTAL needs to be pwrdup accordingly   */
				/* in all the modes like RSDB, MIMO and 80P80                    */
				/* ------------------------------------------------------------- */
				wlc_phy_radio20693_xtal_pwrup(pi);

				wlc_phy_tiny_radio_pwron_seq(pi);

				if (RADIOMAJORREV(pi) == 2) {
					FOREACH_CORE(pi, core) {
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_en, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_AFEldo_pu, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_TXldo_pu, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_VCOldo_pu, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_RXldo_pu, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_ADCldo_pu, 0x1);
						MOD_RADIO_REG_20693(pi, PMU_OVR, core,
							ovr_wlpmu_selavg_lo, 0x1);
					}
				}

				// 4365: Enable the PMU overrides because we don't want the powerup
				// sequence to be controlled by the PMU sequencer
				if (RADIOMAJORREV(pi) == 3)
					wlc_phy_radio20693_pmu_override(pi);

				/* Power up the radio mini PMU Seq */
				wlc_phy_radio20693_minipmu_pwron_seq(pi);

				/* pll config, minipmu cal, RCAL */
				wlc_phy_switch_radio_acphy_20693(pi);
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {

				/* PHY regs configuration and update preferred values */
				wlc_phy_radio20694_pwron_seq_phyregs(pi);

				/* pll PU, minipmu cal, RCAL, RCCAL */
				PHY_TRACE(("wl%d: BEFORE wlc_phy_switch_radio_acphy_20694\n",
					pi->sh->unit));
				wlc_phy_switch_radio_acphy_20694(pi);
				PHY_TRACE(("wl%d: AFTER wlc_phy_switch_radio_acphy_20694\n",
					pi->sh->unit));
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
				/* PHY regs configuration and update preferred values */
				wlc_phy_28nm_radio_pwron_seq_phyregs(pi, 0);

				/* Power up the radio mini PMU Seq */
				wlc_phy_radio20695_minipmu_pupd_seq(pi, 1);

				/* pll PU, minipmu cal, RCAL, RCCAL */
				wlc_phy_switch_radio_acphy_20695(pi);
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
			if (RADIOID(pi->pubpi->radioid) == BCM20696_ID) {
				/* 20696procs.tcl proc 20696_init */

				/* Global Radio PU's */
				wlc_phy_radio20696_pwron_seq_phyregs(pi);

				/* PU the TX/RX/VCO/..LDO's  */
				wlc_phy_radio20696_minipmu_pwron_seq(pi);

				/* R_CAL will be done only for "Core1" */
				wlc_phy_radio20696_r_cal(pi, 1);

				/* Perform MINI PMU CAL */
				wlc_phy_radio20696_minipmu_cal(pi);

				/* Final R_CAL (on Core1) */
				wlc_phy_radio20696_r_cal(pi, 2);

				/* RC CAL */
				wlc_phy_radio20696_rccal(pi);

				/* PLL PWR-UP */
				wlc_phy_radio20696_pmu_pll_pwrup(pi);

				/* WAR's */
				wlc_phy_radio20696_wars(pi);
			}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

			if (!TINY_RADIO(pi) &&
				!(RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) &&
				!(RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) &&
				!(RADIOID_IS(pi->pubpi->radioid, BCM20696_ID))) {

			/* --------------------------RCAL WAR ---------------------------- */
			/* Currently RCAL resistor is not connected on the board. The pin  */
			/* labelled TSSI_G/GPIO goes into the TSSI pin of the FEM through  */
			/* a 0 Ohm resistor. There is an option to add a shunt 10k to GND  */
			/* on this trace but it is depop. Adding shunt resistance on the   */
			/* TSSI line may affect the voltage from the FEM to our TSSI input */
			/* So, this issue is worked around by forcing below registers      */
			/* THIS IS APPLICABLE FOR BOARDTYPE = $def(boardtype)              */
			/* --------------------------RCAL WAR ---------------------------- */

				if (BF3_RCAL_OTP_VAL_EN(pi_ac) == 1) {
					data = pi->sromi->rcal_otp_val;
				} else {
					if (BF3_RCAL_WAR(pi_ac) == 1) {
						if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
							data = ACPHY_RCAL_VAL_2X2;
						} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev)
						           == 1) {
							data = ACPHY_RCAL_VAL_1X1;
						}
					}
					else if (RADIO2069REV(pi->pubpi->radiorev) == 64) {
					   /* for 4364 3x3 hard coding for now */
					   MOD_RADIO_REG(pi, RF2, BG_CFG1, rcal_trim, 10);
					   MOD_RADIO_REG(pi, RF2, OVR2, ovr_bg_rcal_trim, 1);
					   MOD_RADIO_REG(pi, RF2, OVR2, ovr_otp_rcal_sel, 0);

					} else if (RADIO2069REV(pi->pubpi->radiorev) == 66) {
					   MOD_RADIO_REG(pi, RF2, BG_CFG1, rcal_trim, 7);
					   MOD_RADIO_REG(pi, RF2, OVR2, ovr_bg_rcal_trim, 1);
					   MOD_RADIO_REG(pi, RF2, OVR2, ovr_otp_rcal_sel, 0);
					}
					else {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
							wlc_phy_radio2069_rcal(pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

					}
				}

				if (BF3_RCAL_WAR(pi_ac) == 1 ||
					BF3_RCAL_OTP_VAL_EN(pi_ac) == 1) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
					if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
						if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
							FOREACH_CORE(pi, core) {
								MOD_RADIO_REGC(pi, GE32_BG_CFG1,
									core, rcal_trim, data);
								MOD_RADIO_REGC(pi, GE32_OVR2, core,
									ovr_bg_rcal_trim, 1);
								MOD_RADIO_REGC(pi, GE32_OVR2, core,
									ovr_otp_rcal_sel, 0);
							}
						} else if (RADIO2069_MAJORREV
							(pi->pubpi->radiorev) == 1) {
							MOD_RADIO_REG(pi, RFP,  GE16_BG_CFG1,
								rcal_trim, data);
							MOD_RADIO_REG(pi, RFP,  GE16_OVR2,
								ovr_bg_rcal_trim, 1);
							MOD_RADIO_REG(pi, RFP,  GE16_OVR2,
								ovr_otp_rcal_sel, 0);
						}
					}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
				}
			}

			if (TINY_RADIO(pi)) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
				if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) &&
				    RADIOMAJORREV(pi) == 3) {
					wlc_phy_radio_tiny_rccal_wave2(pi_ac->radioi);
				} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
				{
					wlc_phy_radio_tiny_rccal(pi_ac->radioi);
				}
			} else
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
				if (!ISSIM_ENAB(pi->sh->sih)) {
					wlc_phy_radio20694_rccal(pi);

				} else {
					PHY_ERROR(("bypass rccal in QT\n"));
				}
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
			if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
				wlc_phy_28nm_radio_rccal(pi);
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
			if (!RADIOID_IS(pi->pubpi->radioid, BCM20696_ID)) {
				wlc_phy_radio2069_rccal(pi);
			}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

			if (phy_ac_chanmgr_get_data(pi_ac->chanmgri)->init_done) {
				wlc_phy_set_regtbl_on_pwron_acphy(pi);
				wlc_phy_chanspec_set_acphy(pi, pi->radio_chanspec);
			}
			pi->radio_is_on = TRUE;
		}
	} else {
		/* wlc_phy_radio2069_off(); */
		pi->radio_is_on = FALSE;

		/* FEM */
		ACPHYREG_BCAST(pi, RfctrlIntc0, 0);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			impbf_radio_ovrs_4347(pi, 0);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */

		if (!ACMAJORREV_4(pi->pubpi->phy_rev)) {
			ACPHY_REG_LIST_START
				/* AFE */
				ACPHYREG_BCAST_ENTRY(pi, RfctrlCoreAfeCfg10, 0)
				ACPHYREG_BCAST_ENTRY(pi, RfctrlCoreAfeCfg20, 0)
				ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeCfg0, 0x1fff)

				/* Radio RX */
				ACPHYREG_BCAST_ENTRY(pi, RfctrlCoreRxPus0, 0)
				ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideRxPus0, 0xffff)
			ACPHY_REG_LIST_EXECUTE(pi);
		}

		/* Radio TX */
		ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0);
		ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x3ff);

		/* {radio, rfpll, pllldo}_pu = 0 */
		MOD_PHYREG(pi, RfctrlCmd, chip_pu, 0);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		/* Remove rfctrl_bundle_en to control rfpll_pu from chip_pu */
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			MOD_PHYREG(pi, RfctrlCmd, rfctrl_bundle_en, 0);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
		/* SW4345-58 */
		if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
			FOREACH_CORE(pi, core) {
				MOD_RADIO_REG_20691(pi, PMU_OP, core, wlpmu_en, 0);
				MOD_RADIO_REG_20691(pi, PLL_CFG1, core, rfpll_vco_pu, 0);
				MOD_RADIO_REG_20691(pi, PLL_CFG1, core, rfpll_synth_pu, 0);
				MOD_RADIO_REG_20691(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_VCO, 0);
				MOD_RADIO_REG_20691(pi, PLL_HVLDO1, core, ldo_2p5_pu_ldo_CP, 0);
				MOD_RADIO_REG_20691(pi, LOGEN_CFG2, core, logencore_5g_pu, 0);
				MOD_RADIO_REG_20691(pi, LNA5G_CFG2, core, lna5g_pu_lna2, 0);
			}
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
		if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
			if (RADIOMAJORREV(pi) != 3) {
				FOREACH_CORE(pi, core) {
					/* PD the RFPLL */
					data = READ_RADIO_REG_TINY(pi, PLL_CFG1, core);
					data = data & 0xE0FF;
					phy_utils_write_radioreg(pi, RADIO_REG_20693(pi,
						PLL_CFG1, core), data);
					MOD_RADIO_REG_TINY(pi, PLL_HVLDO1, core,
						ldo_2p5_pu_ldo_CP, 0);
					MOD_RADIO_REG_TINY(pi, PLL_HVLDO1, core,
						ldo_2p5_pu_ldo_VCO, 0);
					MOD_RADIO_REG_TINY(pi, PLL_CP1, core, rfpll_cp_pu, 0);
					/* Clear the VCO signal */
					MOD_RADIO_REG_TINY(pi, PLL_CFG2, core, rfpll_rst_n, 0);
					MOD_RADIO_REG_TINY(pi, PLL_VCOCAL13, core,
						rfpll_vcocal_rst_n, 0);
					MOD_RADIO_REG_TINY(pi, PLL_VCOCAL1, core,
						rfpll_vcocal_cal, 0);
					/* Power Down the mini PMU */
					MOD_RADIO_REG_TINY(pi, PMU_CFG4, core, wlpmu_ADCldo_pu, 0);
					data = READ_RADIO_REG_TINY(pi, PMU_OP, core);
					data = data & 0xFF61;
					phy_utils_write_radioreg(pi, RADIO_REG_20693(pi,
						PMU_OP, core), data);
					MOD_RADIO_REG_TINY(pi, BG_CFG1, core, bg_pu, 0);
					MOD_RADIO_REG_TINY(pi, VREG_CFG, core, vreg25_pu, 0);
				}
			} else {
				uint8 pll = 0;
				for (pll = 0; pll < 2; pll++) {
					/* PD the RFPLL */
					data = READ_RADIO_PLLREG_20693(pi, PLL_CFG1, pll);
					data = data & 0xE0FF;
					phy_utils_write_radioreg(pi, RADIO_PLLREG_20693(pi,
						PLL_CFG1, pll), data);
					MOD_RADIO_PLLREG_20693(pi, PLL_HVLDO1, pll,
						ldo_2p5_pu_ldo_CP, 0);
					MOD_RADIO_PLLREG_20693(pi, PLL_HVLDO1, pll,
						ldo_2p5_pu_ldo_VCO, 0);
					MOD_RADIO_PLLREG_20693(pi, PLL_CP1, pll, rfpll_cp_pu, 0);
					/* Clear the VCO signal */
					MOD_RADIO_PLLREG_20693(pi, PLL_CFG2, pll, rfpll_rst_n, 0);
					MOD_RADIO_PLLREG_20693(pi, PLL_VCOCAL13, pll,
						rfpll_vcocal_rst_n, 0);
					MOD_RADIO_PLLREG_20693(pi, PLL_VCOCAL1, pll,
						rfpll_vcocal_cal, 0);
				}
				FOREACH_CORE(pi, core) {
					/* Power Down the mini PMU */
					MOD_RADIO_REG_20693(pi, PMU_CFG4, core, wlpmu_ADCldo_pu, 0);
					data = READ_RADIO_REG_20693(pi, PMU_OP, core);
					data = data & 0xFF61;
					phy_utils_write_radioreg(pi, RADIO_REG_20693(pi,
						PMU_OP, core), data);
					MOD_RADIO_PLLREG_20693(pi, BG_CFG1, core, bg_pu, 0);
					MOD_RADIO_REG_20693(pi, VREG_CFG, core, vreg25_pu, 0);
				}
			}
			if ((phy_get_phymode(pi) == PHYMODE_RSDB) &&
				((phy_get_current_core(pi) == PHY_RSBD_PI_IDX_CORE1))) {
				/* Powerdown xtal ldo for MAC_Core = 1 in case of RSDB mode */
				MOD_RADIO_REG_TINY(pi, PLL_XTALLDO1, 0,
					ldo_1p2_xtalldo1p2_BG_pu, 0);
			} else if ((phy_get_phymode(pi) != PHYMODE_RSDB)) {
				/* Powerdown xtal ldo for Core 1 in case of MIMO and 80+80 */
				MOD_RADIO_REG_TINY(pi, PLL_XTALLDO1, 1,
					ldo_1p2_xtalldo1p2_BG_pu, 0);
			}
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
		/* 4364 dual phy. when 3x3 is powered down, make sure the 1x1 femctrl lines */
		/* do not toggle ant0/ant1 this is done by giving bt the control on ant0/ant1 */
		/* If bt is not there (bt_reg_on 0) this is fine. Ultimately bt firmware needs */
		/* to write proper values in the bt_clb_upi registers bt2clb_swctrl_smask_bt_antX */
		if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID) && IS_4364_3x3(pi)) {
			si_gci_set_femctrl_mask_ant01(pi->sh->sih, pi->sh->osh, 0);
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
		/* Turn off the mini PMU enable on all cores when going down.
		 * Will be powered up in the UP sequence
		 */
		if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID)) {
			if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
				FOREACH_CORE(pi, core)
				        MOD_RADIO_REGC(pi, GE32_PMU_OP, core, wlpmu_en, 0);
			}
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			/* Powering down PLL LDO and also putting RFPLL in reset */
			WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x1);
			WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0x5);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
		{
			WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x0);
			WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0x1);
		}

#ifdef STB_SOC_WIFI
		if (CHIPID(pi->sh->chip) == BCM4335_CHIP_ID) {
			ACPHY_REG_LIST_START
				/* WAR for XTAL power up issues */
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_xtal_pu_corebuf_pfd, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_pfddrv, 0)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_corebuf_pfd, 0)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_BT, 0)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL1, 0)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 0)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_caldrv, 0)
			ACPHY_REG_LIST_EXECUTE(pi);
		}
#endif /* STB_SOC_WIFI */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		/* Ref JIRA64559 for more details. Summary of the issue - In "wl down" path */
		/* AFE_CLK_DIV block was not being turned off, but the */
		/* preceding blocks - RF_LDO and RF_PLL were turned off. So, the output of RF_PLL */
		/* is in undefined state and with that as input, the output of AFE_CLK_DIV block */
		/* would also be in undefined state. The output clock of this block was causing */
		/* BT sensitivity degradation */
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			MOD_PHYREG(pi, AfeClkDivOverrideCtrl, afediv_sel_div_ovr, 1);
			MOD_PHYREG(pi, AfeClkDivOverrideCtrl, afediv_sel_div, 0x0);
			MOD_PHYREG(pi, AfeClkDivOverrideCtrl, afediv_en_ovr, 1);
			MOD_PHYREG(pi, AfeClkDivOverrideCtrl, afediv_en, 0x0);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
		if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
			FOREACH_CORE(pi, core) {
				if (core == 0) {
					ACPHY_REG_LIST_START
					MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG10, 0,
						bg_wlpmu_vref_sel, 0x0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, PMU_CFG4, 0,
						wlpmu_en, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RFP, LDO1P8_STAT, 0,
						ldo1p8_pu, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, LDO1P65_STAT,
						0, ldo1p6_pu, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, PMU_CFG4,
						0, wlpmu_LDO2P2_pu, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
				} else {
					ACPHY_REG_LIST_START
					MOD_RADIO_REG_20694_ENTRY(pi, RFP, BG_REG10, 0,
						bg_wlpmu_vref_sel, 0x0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, PMU_CFG4, 1,
						wlpmu_en, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RFP, LDO1P8_STAT, 0,
						ldo1p8_pu, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, LDO1P65_STAT,
						1, ldo1p6_pu, 0)
					MOD_RADIO_REG_20694_ENTRY(pi, RF, PMU_CFG4,
						1, wlpmu_LDO2P2_pu, 0)
					ACPHY_REG_LIST_EXECUTE(pi);
				}
			}
			WRITE_RADIO_REG_20694(pi, RFP, PLL_HVLDO1, 0, 0);
			MOD_RADIO_REG_20694(pi, RFP, XTAL6, 0, xtal_pu_caldrv, 0);
			MOD_RADIO_REG_20694(pi, RFP, XTAL5, 0, xtal_pu_RCCAL, 0);
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
		if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
			/* Turn off 2G and 5G PLL */
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL2G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, PLL5G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5G, 0, 0);
			wlc_phy_28nm_radio_pll_logen_pupd_seq(pi, LOGEN5GTO2G, 0, 0);

			/* Turn off minipmu */
			wlc_phy_radio20695_minipmu_pupd_seq(pi, 0);
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

		/* These register turn on AFE_CLK_DIV block in "wl down" path. Leaving the block
		 * turned on was causing BT BER issue. RB: 46565.
		 */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0))
		if (ACMAJORREV_0(pi->pubpi->phy_rev)) {
			WRITE_PHYREG(pi, AfeClkDivOverrideCtrl, 0xf);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
		if (ACMAJORREV_1(pi->pubpi->phy_rev)) {
			WRITE_PHYREG(pi, AfeClkDivOverrideCtrlN0, 0x1);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
		if (ACMAJORREV_2(pi->pubpi->phy_rev) && !ACMINORREV_0(pi)) {
			ACPHYREG_BCAST(pi, AfeClkDivOverrideCtrlN0, 0x1);
			WRITE_PHYREG(pi, AfeClkDivOverrideCtrl, 0x408);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
		if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
			ACPHYREG_BCAST(pi, AfeClkDivOverrideCtrlN0, 0x1);
			WRITE_PHYREG(pi, AfeClkDivOverrideCtrl, 0x8);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
		{
		}
	}

#ifndef STB_SOC_WIFI
	if ((CHIPID(pi->sh->chip) == BCM4335_CHIP_ID &&
	     !CST4335_CHIPMODE_USB20D(pi->sh->sih->chipst)) ||
	    (BCM4350_CHIP(pi->sh->chip) &&
	     !CST4350_CHIPMODE_HSIC20D(pi->sh->sih->chipst))) {
		/* Power down HSIC */
		ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));
		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) > 0) {
			MOD_RADIO_REG(pi, RFP,  GE16_PLL_XTAL2, xtal_pu_HSIC, 0x0);
		}
	} else if ((CHIPID(pi->sh->chip) == BCM4345_CHIP_ID &&
	           !CST4345_CHIPMODE_USB20D(pi->sh->sih->chipst))) {
		/* Power down HSIC */
		ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20691(pi, PLL_XTAL2, core, xtal_pu_HSIC, 0x0);
			MOD_RADIO_REG_20691(pi, PLL_XTAL_OVR1, core, ovr_xtal_pu_HSIC, 0x1);
		}
	}
#endif /* STB_SOC_WIFI */

#ifdef LOW_TX_CURRENT_SETTINGS_2G
/* TODO: iPa low power in 2G - check if condition is needed) */
	if ((CHSPEC_IS2G(pi->radio_chanspec) && (pi->sromi->extpagain2g == 2)) ||
		(CHSPEC_IS5G(pi->radio_chanspec) && (pi->sromi->extpagain5g == 2))) {
		is_ipa = 1;
	} else {
		is_ipa = 0;
	}

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));
	if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) && (is_ipa == 0)) {
		PHY_TRACE(("Modifying PA Bias settings for lower power for 2G!\n"));
		MOD_RADIO_REG(pi, RFX, PA2G_IDAC2, pa2g_biasa_main, 0x36);
		MOD_RADIO_REG(pi, RFX, PA2G_IDAC2, pa2g_biasa_aux, 0x36);
	}

#endif /* LOW_TX_CURRENT_SETTINGS_2G */
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_phy_radio2069_mini_pwron_seq_rev16(phy_info_t *pi)
{
	uint8 cntr = 0;

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, wlpmu_en, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, VCOldo_pu, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, TXldo_pu, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, AFEldo_pu, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, RXldo_pu, 1)
	ACPHY_REG_LIST_EXECUTE(pi);

	OSL_DELAY(100);

	ACPHY_REG_LIST_START
		/* WAR for XTAL power up issues */
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_xtal_pu_corebuf_pfd, 0)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_pfddrv, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_BT, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_caldrv, 1)

		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, synth_pwrsw_en, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, wlpmu_ldobg_clk_en, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PMU_OP, ldoref_start_cal, 1)
	ACPHY_REG_LIST_EXECUTE(pi);

	if (!ISSIM_ENAB(pi->sh->sih)) {
		while (READ_RADIO_REGFLD(pi, RFP, GE16_PMU_STAT, ldobg_cal_done) == 0) {
			OSL_DELAY(100);
			cntr++;
			if (cntr > 100) {
				PHY_ERROR(("PMU cal Fail \n"));
				break;
			}
		}
	}

	MOD_RADIO_REG(pi, RFP, GE16_PMU_OP, ldoref_start_cal, 0);
	MOD_RADIO_REG(pi, RFP, GE16_PMU_OP, wlpmu_ldobg_clk_en, 0);
}

static void
wlc_phy_radio2069_mini_pwron_seq_rev32(phy_info_t *pi)
{

	uint8 cntr = 0;
	phy_utils_write_radioreg(pi, RFX_2069_GE32_PMU_OP, 0x9e);

	OSL_DELAY(100);

	ACPHY_REG_LIST_START
		WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_PMU_OP, 0xbe)

		WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_PMU_OP, 0x20be)
		WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_PMU_OP, 0x60be)
	ACPHY_REG_LIST_EXECUTE(pi);

	if (!ISSIM_ENAB(pi->sh->sih)) {
		while (READ_RADIO_REGFLD(pi, RF0, GE32_PMU_STAT, ldobg_cal_done) == 0 ||
		       READ_RADIO_REGFLD(pi, RF1, GE32_PMU_STAT, ldobg_cal_done) == 0) {

			OSL_DELAY(100);
			cntr++;
			if (cntr > 100) {
				PHY_ERROR(("PMU cal Fail ...222\n"));
				break;
			}
		}
	}
	phy_utils_write_radioreg(pi, RFX_2069_GE32_PMU_OP, 0xbe);
}
#endif /* #if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_phy_radio2069_pwron_seq(phy_info_t *pi)
{
	/* Note: if RfctrlCmd.rfctrl_bundle_en = 0, then rfpll_pu = radio_pu
	   So, to make rfpll_pu = 0 & radio_pwrup = 1, make RfctrlCmd.rfctrl_bundle_en = 1
	*/
	uint16 txovrd = READ_PHYREG(pi, RfctrlCoreTxPus0);
	uint16 rfctrlcmd = READ_PHYREG(pi, RfctrlCmd) & 0xfc38;

	ACPHY_REG_LIST_START
		/* Using usleep of 100us below, so don't need these */
		WRITE_PHYREG_ENTRY(pi, Pllldo_resetCtrl, 0)
		WRITE_PHYREG_ENTRY(pi, Rfpll_resetCtrl, 0)
		WRITE_PHYREG_ENTRY(pi, Logen_AfeDiv_reset, 0x2000)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* Start with everything off: {radio, rfpll, plldlo, logen}_{pu, reset} = 0 */
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0);
	WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0xd);
	WRITE_PHYREG(pi, RfctrlCoreTxPus0, txovrd & 0x7e7f);
	WRITE_PHYREG(pi, RfctrlOverrideTxPus0, READ_PHYREG(pi, RfctrlOverrideTxPus0) | 0x180);

	/* ***  Start Radio rfpll pwron seq  ***
	   Start with chip_pu = 0, por_reset = 0, rfctrl_bundle_en = 0
	*/
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd);

	/* Toggle jtag reset (not required for uCode PM) */
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd | 1);
	OSL_DELAY(1);
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd | 0);

	/* Update preferred values (not required for uCode PM) */
	wlc_phy_radio2069_upd_prfd_values(pi);

	/* Toggle radio_reset (while radio_pu = 1) */
	MOD_RADIO_REG(pi, RF2, VREG_CFG, bg_filter_en, 0);   /* radio_reset = 1 */
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd | 6);   /* radio_pwrup = 1, rfpll_pu = 0 */
	OSL_DELAY(100);                                      /* radio_reset to be high for 100us */
	MOD_RADIO_REG(pi, RF2, VREG_CFG, bg_filter_en, 1);   /* radio_reset = 0 */

	/* {rfpll, pllldo, logen}_{pu, reset} pwron seq */
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0xd);
	WRITE_PHYREG(pi, RfctrlCmd, rfctrlcmd | 2);
	WRITE_PHYREG(pi, RfctrlCoreTxPus0, txovrd | 0x180);
	OSL_DELAY(100);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x4);
	WRITE_PHYREG(pi, RfctrlCoreTxPus0, txovrd & 0xfeff);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static void
wlc_phy_tiny_radio_pwron_seq(phy_info_t *pi)
{
	ASSERT(TINY_RADIO(pi));
	/* Note: if RfctrlCmd.rfctrl_bundle_en = 0, then rfpll_pu = radio_pu
	   So, to make rfpll_pu = 0 & radio_pwrup = 1, make RfctrlCmd.rfctrl_bundle_en = 1
	*/

	/* # power down everything */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0);
		ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x3ff);
	} else {
		ACPHY_REG_LIST_START
			WRITE_PHYREG_ENTRY(pi, RfctrlCoreTxPus0, 0)
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideTxPus0, 0x3ff)
		ACPHY_REG_LIST_EXECUTE(pi);
	}

	ACPHY_REG_LIST_START
		/* Using usleep of 100us below, so don't need these */
		WRITE_PHYREG_ENTRY(pi, Pllldo_resetCtrl, 0)
		WRITE_PHYREG_ENTRY(pi, Rfpll_resetCtrl, 0)
		WRITE_PHYREG_ENTRY(pi, Logen_AfeDiv_reset, 0x2000)

		/* Start with everything off: {radio, rfpll, plldlo, logen}_{pu, reset} = 0 */
		WRITE_PHYREG_ENTRY(pi, RfctrlCmd, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreGlobalPus, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlOverrideGlobalPus, 0xd)

		/* # Reset radio, jtag */
		/* NOTE: por_force doesnt have any effect in 4349A0 and */
		/* radio jtag reset is taken care by the PMU */
		WRITE_PHYREG_ENTRY(pi, RfctrlCmd, 0x7)
	ACPHY_REG_LIST_EXECUTE(pi);
	/* # radio_reset to be high for 100us */
	OSL_DELAY(100);
	WRITE_PHYREG(pi, RfctrlCmd, 0x6);

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
	/* Update preferred values (not required for uCode PM) */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
		wlc_phy_radio20691_upd_prfd_values(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
		wlc_phy_radio20693_upd_prfd_values(pi);
	}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
	ACPHY_REG_LIST_START
		/* # {rfpll, pllldo, logen}_{pu, reset} */
		/* # pllldo_{pu, reset} = 1, rfpll_reset = 1 */
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreGlobalPus, 0xd)
		/* # {radio, rfpll}_pwrup = 1 */
		WRITE_PHYREG_ENTRY(pi, RfctrlCmd, 0x2)

		/* # logen_{pwrup, reset} = 1 */
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreTxPus0, 0x180)
	ACPHY_REG_LIST_EXECUTE(pi);
		OSL_DELAY(100); /* # resets to be on for 100us */
	ACPHY_REG_LIST_START
		/* # {pllldo, rfpll}_reset = 0 */
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreGlobalPus, 0x4)
	ACPHY_REG_LIST_EXECUTE(pi);
		/* # logen_reset = 0 */
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0x80);
		ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x180);
	} else {
		ACPHY_REG_LIST_START
			WRITE_PHYREG_ENTRY(pi, RfctrlCoreTxPus0, 0x80)
			/* # leave overrides for logen_{pwrup, reset} */
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideTxPus0, 0x180)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

#define MAX_2069_RCAL_WAITLOOPS 100
/* rcal takes ~50us */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_phy_radio2069_rcal(phy_info_t *pi)
{
	uint8 done, rcal_val, core;

	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ACPHY_REG_LIST_START
		/* Power-up rcal clock (need both of them for rcal) */
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL1, 1)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 1)

		/* Rcal can run with 40mhz cls, no diving */
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL5, xtal_sel_RCCAL, 0)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL5, xtal_sel_RCCAL1, 0)
	ACPHY_REG_LIST_EXECUTE(pi);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	/* Make connection with the external 10k resistor */
	/* Turn off all test points in cgpaio block to avoid conflict */
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REGC(pi, GE32_CGPAIO_CFG1, core, cgpaio_pu, 1);
		}
		ACPHY_REG_LIST_START
			WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_CGPAIO_CFG2, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_CGPAIO_CFG3, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_CGPAIO_CFG4, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFX_2069_GE32_CGPAIO_CFG5, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_TOP_SPARE1, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_TOP_SPARE2, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_TOP_SPARE4, 0)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_TOP_SPARE6, 0)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_ENTRY(pi, RF2, CGPAIO_CFG1, cgpaio_pu, 1)
			WRITE_RADIO_REG_ENTRY(pi, RF2_2069_CGPAIO_CFG2, 0)
			WRITE_RADIO_REG_ENTRY(pi, RF2_2069_CGPAIO_CFG3, 0)
			WRITE_RADIO_REG_ENTRY(pi, RF2_2069_CGPAIO_CFG4, 0)
			WRITE_RADIO_REG_ENTRY(pi, RF2_2069_CGPAIO_CFG5, 0)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
	/* NOTE: xtal_pu, xtal_buf_pu & xtalldo_pu direct control lines should be(& are) ON */

	/* Toggle the rcal pu for calibration engine */
	MOD_RADIO_REG(pi, RF2, RCAL_CFG, pu, 0);
	OSL_DELAY(1);
	MOD_RADIO_REG(pi, RF2, RCAL_CFG, pu, 1);

	/* Wait for rcal to be done, max = 10us * 100 = 1ms  */
	done = 0;


	SPINWAIT(READ_RADIO_REGFLD(pi, RF2, RCAL_CFG,
		i_wrf_jtag_rcal_valid) == 0, ACPHY_SPINWAIT_RCAL_STATUS);

	done = READ_RADIO_REGFLD(pi, RF2, RCAL_CFG, i_wrf_jtag_rcal_valid);

	if (done == 0) {
		PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR : RCAL Failed", __FUNCTION__));
		PHY_FATAL_ERROR(pi, PHY_RC_RCAL_INVALID);
	}

	/* Status */
	rcal_val = READ_RADIO_REGFLD(pi, RF2, RCAL_CFG, i_wrf_jtag_rcal_value);
	rcal_val = rcal_val >> 1;
	PHY_INFORM(("wl%d: %s rcal=%d\n", pi->sh->unit, __FUNCTION__, rcal_val));

	/* Valid range of values for rcal */
	ASSERT((rcal_val > 0) && (rcal_val < 15));

	/*  Power down blocks not needed anymore */
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REGC(pi, GE32_CGPAIO_CFG1, core, cgpaio_pu, 0);
		}
	} else {
		MOD_RADIO_REG(pi, RF2, CGPAIO_CFG1, cgpaio_pu, 0);
	}

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL1, 0)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 0)
		MOD_RADIO_REG_ENTRY(pi, RF2, RCAL_CFG, pu, 0)
	ACPHY_REG_LIST_EXECUTE(pi);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

/* rccal takes ~3ms per, i.e. ~9ms total */
static void
wlc_phy_radio2069_rccal(phy_info_t *pi)
{
	uint8 cal, core, rccal_val[NUM_2069_RCCAL_CAPS];
	uint16 n0, n1;

	/* lpf, adc, dacbuf */
	uint8 sr[] = {0x1, 0x0, 0x0};
	uint8 sc[] = {0x0, 0x2, 0x1};
	uint8 x1[] = {0x1c, 0x70, 0x40};
	uint16 trc[] = {0x14a, 0x101, 0x11a};
	uint16 gmult_const = 193;

	phy_ac_radio_data_t *data = pi->u.pi_acphy->radioi->data;

	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));


	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) > 0) {
		if PHY_XTAL_IS40M(pi) {
		  if ((RADIO2069REV(pi->pubpi->radiorev) == 25) ||
		                      (RADIO2069REV(pi->pubpi->radiorev) == 26)) {
			gmult_const = 70;
			trc[0] = 0x294;
			trc[1] = 0x202;
			trc[2] = 0x214;
		  } else {
			gmult_const = 160;
		  }
		} else if (PHY_XTAL_IS37M4(pi)) {
		  if ((RADIO2069REV(pi->pubpi->radiorev) == 25) ||
		                      (RADIO2069REV(pi->pubpi->radiorev) == 26)) {
			gmult_const = 77;
			trc[0] = 0x45a;
			trc[1] = 0x1e0;
			trc[2] = 0x214;
		  } else {
			gmult_const = 158;
			trc[0] = 0x22d;
			trc[1] = 0xf0;
			trc[2] = 0x10a;
		  }
		} else if (PHY_XTAL_IS52M(pi)) {
			if ((RADIO2069REV(pi->pubpi->radiorev) == 25) ||
				(RADIO2069REV(pi->pubpi->radiorev) == 26)) {
				gmult_const = 77;
				trc[0] = 0x294;
				trc[1] = 0x202;
				trc[2] = 0x214;
			  } else {
				gmult_const = 160;
				trc[0] = 0x14a;
				trc[1] = 0x101;
				trc[2] = 0x11a;
			  }
		} else {
			gmult_const = 160;
		}

	} else {
		gmult_const = 193;
		if ((RADIO2069REV(pi->pubpi->radiorev) == 64) ||
			(RADIO2069REV(pi->pubpi->radiorev) == 66)) {
			gmult_const = 216;
			trc[0] = 0x22d;
			trc[1] = 0xf0;
			trc[2] = 0x10a;
		}
	}

	/* Powerup rccal driver & set divider radio (rccal needs to run at 20mhz) */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 1);
	MOD_RADIO_REG(pi, RFP, PLL_XTAL5, xtal_sel_RCCAL, 2);

	/* Calibrate lpf, adc, dacbuf */
	for (cal = 0; cal < NUM_2069_RCCAL_CAPS; cal++) {
		/* Setup */
		MOD_RADIO_REG(pi, RF2, RCCAL_CFG, sr, sr[cal]);
		MOD_RADIO_REG(pi, RF2, RCCAL_CFG, sc, sc[cal]);
		MOD_RADIO_REG(pi, RF2, RCCAL_LOGIC1, rccal_X1, x1[cal]);
		phy_utils_write_radioreg(pi, RF2_2069_RCCAL_TRC, trc[cal]);

		/* For dacbuf force fixed dacbuf cap to be 0 while calibration, restore it later */
		if (cal == 2) {
			FOREACH_CORE(pi, core) {
				MOD_RADIO_REGC(pi, DAC_CFG2, core, DACbuf_fixed_cap, 0);
				if (RADIO2069_MAJORREV(pi->pubpi->radiorev) > 0) {
					MOD_RADIO_REGC(pi, GE16_OVR22, core,
					               ovr_afe_DACbuf_fixed_cap, 1);
				} else {
					MOD_RADIO_REGC(pi, OVR21, core,
					               ovr_afe_DACbuf_fixed_cap, 1);
				}
			}
		}

		/* Toggle RCCAL power */
		MOD_RADIO_REG(pi, RF2, RCCAL_CFG, pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_REG(pi, RF2, RCCAL_CFG, pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_REG(pi, RF2, RCCAL_LOGIC1, rccal_START, 1);

		/* This delay is required before reading the RCCAL_LOGIC2
		*   without this delay, the cal values are incorrect.
		*/
		OSL_DELAY(100);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		SPINWAIT(READ_RADIO_REGFLD(pi, RF2, RCCAL_LOGIC2, rccal_DONE) == 0,
				MAX_2069_RCCAL_WAITLOOPS);
		if (READ_RADIO_REGFLD(pi, RF2, RCCAL_LOGIC2, rccal_DONE) == 0) {
			PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR : RCCAL invalid \n",
				__FUNCTION__));
			PHY_FATAL_ERROR(pi, PHY_RC_RCCAL_INVALID);
		}

		/* Stop RCCAL */
		MOD_RADIO_REG(pi, RF2, RCCAL_LOGIC1, rccal_START, 0);

		if (cal == 0) {
			/* lpf */
			n0 = READ_RADIO_REG(pi, RF2, RCCAL_LOGIC3);
			n1 = READ_RADIO_REG(pi, RF2, RCCAL_LOGIC4);
			/* gmult = (30/40) * (n1-n0) = (193 * (n1-n0)) >> 8 */
			rccal_val[cal] = (gmult_const * (n1 - n0)) >> 8;
			data->rccal_gmult = rccal_val[cal];
			data->rccal_gmult_rc = data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
				__FUNCTION__, rccal_val[cal]));
		} else if (cal == 1) {
			/* adc */
			rccal_val[cal] = READ_RADIO_REGFLD(pi, RF2, RCCAL_LOGIC5, rccal_raw_adc1p2);
			PHY_INFORM(("wl%d: %s rccal_adc = %d\n", pi->sh->unit,
				__FUNCTION__, rccal_val[cal]));

			/* don't change this loop to active core loop,
			   gives slightly higher floor, why?
			*/
			FOREACH_CORE(pi, core) {
				MOD_RADIO_REGC(pi, ADC_RC1, core, adc_ctl_RC_4_0, rccal_val[cal]);
				MOD_RADIO_REGC(pi, TIA_CFG3, core, rccal_hpc, rccal_val[cal]);
			}
			/* Store value, might be overriden upon channel change */
			pi->u.pi_acphy->radioi->rccal_adc_rc = rccal_val[cal];
		} else {
			/* dacbuf */
			rccal_val[cal] = READ_RADIO_REGFLD(pi, RF2, RCCAL_LOGIC5, rccal_raw_dacbuf);
			data->rccal_dacbuf = rccal_val[cal];

			/* take away the override on dacbuf fixed cap */
			FOREACH_CORE(pi, core) {
				if (RADIO2069_MAJORREV(pi->pubpi->radiorev) > 0) {
					MOD_RADIO_REGC(pi, GE16_OVR22, core,
					               ovr_afe_DACbuf_fixed_cap, 0);
				} else {
					MOD_RADIO_REGC(pi, OVR21, core,
					               ovr_afe_DACbuf_fixed_cap, 0);
				}
			}
			PHY_INFORM(("wl%d: %s rccal_dacbuf = %d\n", pi->sh->unit,
				__FUNCTION__, rccal_val[cal]));
		}

		/* Turn off rccal */
		MOD_RADIO_REG(pi, RF2, RCCAL_CFG, pu, 0);
	}

	/* Powerdown rccal driver */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 0);
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
wlc_phy_switch_radio_acphy_20691(phy_info_t *pi)
{

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));

	ACPHY_REG_LIST_START
		/* 20691_mini_pwron_seq_phyregs_rev12() */
		/* # mini PMU init */
		/* #Step 1 */
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_OP, 0, wlpmu_en, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_OP, 0, wlpmu_VCOldo_pu, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_OP, 0, wlpmu_TXldo_pu, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_OP, 0, wlpmu_AFEldo_pu, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_OP, 0, wlpmu_RXldo_pu, 1)
	ACPHY_REG_LIST_EXECUTE(pi);
	/* #Step 2 */
	OSL_DELAY(200);

	ACPHY_REG_LIST_START
		/* # DAC */
		MOD_RADIO_REG_20691_ENTRY(pi, CLK_DIV_CFG1, 0, dac_driver_size, 8)
		MOD_RADIO_REG_20691_ENTRY(pi, CLK_DIV_CFG1, 0, sel_dac_div, 3)
		MOD_RADIO_REG_20691_ENTRY(pi, TX_DAC_CFG5, 0, DAC_invclk, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, TX_DAC_CFG5, 0, DAC_pd_mode, 0)
		/* # improve settling behavior, need for level tracking */
		MOD_RADIO_REG_20691_ENTRY(pi, BG_CFG1, 0, bg_pulse, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, TX_DAC_CFG1, 0, DAC_scram_off, 1)

		/* # ADC */
		MOD_RADIO_REG_20691_ENTRY(pi, PMU_CFG4, 0, wlpmu_ADCldo_pu, 1)

		/* 20691_pmu_pll_pwrup */
		MOD_RADIO_REG_20691_ENTRY(pi, VREG_CFG, 0, vreg25_pu, 1) /* #powerup vreg2p5 */
		MOD_RADIO_REG_20691_ENTRY(pi, BG_CFG1, 0, bg_pu, 1) /* #powerup bandgap */

		/* # powerup pll */
		/* # VCO */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG1, 0, rfpll_vco_pu, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_rfpll_vco_pu, 1)
		/* # VCO LDO */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO1, 0, ldo_2p5_pu_ldo_VCO, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_ldo_2p5_pu_ldo_VCO, 1)
		/* # VCO buf */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG1, 0, rfpll_vco_buf_pu, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_rfpll_vco_buf_pu, 1)
		/* # Synth global */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG1, 0, rfpll_synth_pu, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_rfpll_synth_pu, 1)
		/* # Charge Pump */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_CP1, 0, rfpll_cp_pu, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_rfpll_cp_pu, 1)
		/* # Charge Pump LDO */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO1, 0, ldo_2p5_pu_ldo_CP, 1)
		/* # Enable override */
		MOD_RADIO_REG_20691_ENTRY(pi, RFPLL_OVR1, 0, ovr_ldo_2p5_pu_ldo_CP, 1)
		/* # PLL lock monitor */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG1, 0, rfpll_monitor_pu, 1)

		/* Nishant's hack to get PLL lock: */
		/* spare 127 (bit 15) is xtal_dcc_clk_sel */
		/* actually controls the inrush current limit on xgtal ldo */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL12, 0, xtal_dcc_clk_sel, 1)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* minipmu_cal */
	wlc_phy_tiny_radio_minipmu_cal(pi);

	/* r cal */
	if (((RADIO20691_MAJORREV(pi->pubpi->radiorev) == 1) &&
		(RADIO20691_MINORREV(pi->pubpi->radiorev) >= 5) &&
		((RADIO20691_MINORREV(pi->pubpi->radiorev) != 16) &&
		(RADIO20691_MINORREV(pi->pubpi->radiorev) != 17))) ||
		(RADIO20691_MAJORREV(pi->pubpi->radiorev) > 1)) {
		/* 4345c0 and after or 43909 */
		/* use OTP if available */
		wlc_phy_radio_tiny_rcal(pi, TINY_RCAL_MODE0_OTP);
	} else {
		/* hard-coded RCal value */
		wlc_phy_radio_tiny_rcal(pi, TINY_RCAL_MODE1_STATIC);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_phy_radio2069_upd_prfd_values(phy_info_t *pi)
{
	uint8 core;
	const radio_20xx_prefregs_t *prefregs_2069_ptr = NULL;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	switch (RADIO2069REV(pi->pubpi->radiorev)) {
	case 3:
		prefregs_2069_ptr = prefregs_2069_rev3;
		break;
	case 4:
	case 8:
		prefregs_2069_ptr = prefregs_2069_rev4;
		break;
	case 7:
		prefregs_2069_ptr = prefregs_2069_rev4;
		break;
	case 16:
		prefregs_2069_ptr = prefregs_2069_rev16;
		break;
	case 17:
		prefregs_2069_ptr = prefregs_2069_rev17;
		break;
	case 18:
		prefregs_2069_ptr = prefregs_2069_rev18;
		break;
	case 23:
		prefregs_2069_ptr = prefregs_2069_rev23;
		break;
	case 24:
		prefregs_2069_ptr = prefregs_2069_rev24;
		break;
	case 25:
		prefregs_2069_ptr = prefregs_2069_rev25;
		break;
	case 26:
		prefregs_2069_ptr = prefregs_2069_rev26;
		break;
	case 32:
	case 33:
	case 34:
	case 35:
	case 37:
	case 38:
		prefregs_2069_ptr = prefregs_2069_rev33_37;
		break;
	case 39:
	case 40:
	case 44:
		prefregs_2069_ptr = prefregs_2069_rev39;
		break;
	case 36:
		prefregs_2069_ptr = prefregs_2069_rev36;
		break;
	case 64:
	case 66:
		prefregs_2069_ptr = prefregs_2069_rev64;
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIO2069REV(pi->pubpi->radiorev)));
		ASSERT(0);
		return;
	}

	/* Update preferred values */
	wlc_phy_init_radio_prefregs_allbands(pi, prefregs_2069_ptr);

	if (BOARDFLAGS(GENERIC_PHY_INFO(pi)->boardflags) &
		BFL_SROM11_WLAN_BT_SH_XTL) {
		MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_BT, 1);
	}

	/* **** NOTE : Move the following to XLS (whenever possible) *** */

	/* Reg conflict with 2069 rev 16 */
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 0) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_ENTRY(pi, RFP, OVR15, ovr_rfpll_rst_n, 1)
			MOD_RADIO_REG_ENTRY(pi, RFP, OVR15, ovr_rfpll_en_vcocal, 1)
			MOD_RADIO_REG_ENTRY(pi, RFP, OVR16, ovr_rfpll_vcocal_rstn, 1)

			MOD_RADIO_REG_ENTRY(pi, RFP, OVR15, ovr_rfpll_cal_rst_n, 1)
			MOD_RADIO_REG_ENTRY(pi, RFP, OVR15, ovr_rfpll_pll_pu, 1)
			MOD_RADIO_REG_ENTRY(pi, RFP, OVR15, ovr_rfpll_vcocal_cal, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else {
		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
			ACPHY_REG_LIST_START
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_rfpll_en_vcocal, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR17, ovr_rfpll_vcocal_rstn, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_rfpll_rst_n, 1)

				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_rfpll_cal_rst_n, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_rfpll_pll_pu, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR16, ovr_rfpll_vcocal_cal, 1)
			ACPHY_REG_LIST_EXECUTE(pi);
		}

		/* Until this is moved to XLS (give bg_filter_en control to radio) */
		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
			MOD_RADIO_REG(pi, RFP, GE32_OVR1, ovr_vreg_bg_filter_en, 1);
		}

		/* Ensure that we read the values that are actually applied */
		/* to the radio block and not just the radio register values */

		phy_utils_write_radioreg(pi, RFX_2069_GE16_READOVERRIDES, 1);
		MOD_RADIO_REG(pi, RFP, GE16_READOVERRIDES, read_overrides, 1);

		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
			/* For VREG power up in radio jtag as there is a bug in the digital
			 * connection
			 */
			if (RADIO2069_MINORREV(pi->pubpi->radiorev) < 5) {
				MOD_RADIO_REG(pi, RF2, VREG_CFG, pup, 1);
				MOD_RADIO_REG(pi, RFP, GE32_OVR1, ovr_vreg_pup, 1);
			}

			/* This OVR enable is required to change the value of
			 * reg(RFP_pll_xtal4.xtal_outbufstrg) and used the value from the
			 * jtag. Otherwise the direct control from the chip has a fixed
			 * non-programmable value!
			 */
			MOD_RADIO_REG(pi, RFP, GE16_OVR27, ovr_xtal_outbufstrg, 1);
		}
	}

	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) < 2) {
		MOD_RADIO_REG(pi, RF2, BG_CFG1, bg_pulse, 1);
	}
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REGC(pi, GE32_BG_CFG1, core, bg_pulse, 1);
			MOD_RADIO_REGC(pi, GE32_OVR2, core, ovr_bg_pulse, 1);
		}
	}

	if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 0) &&
			(((RADIO2069_MINORREV(pi->pubpi->radiorev)) == 16) ||
			(RADIO2069_MINORREV(pi->pubpi->radiorev) == 17))) {
			/* 4364 3x3 : to address XTAL spur on core2 */
				MOD_RADIO_REG(pi, RFP, PLL_XTAL8, xtal_repeater3_size, 4);
	}

	/* Give control of bg_filter_en to radio */
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) < 2)
		MOD_RADIO_REG(pi, RF2, OVR2, ovr_vreg_bg_filter_en, 1);

	FOREACH_CORE(pi, core) {
		/* Fang's recommended settings */
		MOD_RADIO_REGC(pi, ADC_RC1, core, adc_ctl_RC_9_8, 1);
		MOD_RADIO_REGC(pi, ADC_RC2, core, adc_ctrl_RC_17_16, 2);

		/* These should be 0, as they are controlled via direct control lines
		   If they are 1, then during 5g, they will turn on
		*/
		MOD_RADIO_REGC(pi, PA2G_CFG1, core, pa2g_bias_cas_pu, 0);
		MOD_RADIO_REGC(pi, PA2G_CFG1, core, pa2g_2gtx_pu, 0);
		MOD_RADIO_REGC(pi, PA2G_CFG1, core, pa2g_bias_pu, 0);
		MOD_RADIO_REGC(pi, PAD2G_CFG1, core, pad2g_pu, 0);
	}
	/* SWWLAN-39535  LNA1 clamping issue for 4360b1 and 43602a0 */
	if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 0) &&
		((RADIO2069REV(pi->pubpi->radiorev) == 7) ||
		(RADIO2069REV(pi->pubpi->radiorev) == 8) ||
		(RADIO2069REV(pi->pubpi->radiorev) == 13))) {
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REGC(pi, RXRF5G_CFG1, core, pu_pulse, 1);
			MOD_RADIO_REGC(pi, OVR18, core, ovr_rxrf5g_pu_pulse, 1);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
wlc_phy_radio20691_upd_prfd_values(phy_info_t *pi)
{
	const radio_20xx_prefregs_t *prefregs_20691_ptr = NULL;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));

	/* Choose the right table to use */
	switch (RADIO20691REV(pi->pubpi->radiorev)) {
	case 60:
	case 68:
		prefregs_20691_ptr = prefregs_20691_rev68;
		break;
	case 75:
		prefregs_20691_ptr = prefregs_20691_rev75;
		break;
	case 79:
		prefregs_20691_ptr = prefregs_20691_rev79;
		break;
	case 74:
	case 82:
		prefregs_20691_ptr = prefregs_20691_rev82;
		break;
	case 85:
	case 86:
	case 87:
	case 88:
		prefregs_20691_ptr = prefregs_20691_rev88;
		break;
	case 129:
	case 130:
		prefregs_20691_ptr = prefregs_20691_rev129;
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIO20691REV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return;
	}

	/* Update preferred values */
	wlc_phy_init_radio_prefregs_allbands(pi, prefregs_20691_ptr);

	MOD_RADIO_REG_20691(pi, LOGEN_OVR1, 0, ovr_logencore_5g_en_lowband, 0x1);
	MOD_RADIO_REG_20691(pi, LOGEN_CFG2, 0, logencore_5g_en_lowband, 0x0);

	if (!PHY_IPA(pi)) {
		ACPHY_REG_LIST_START
			/* Saeed's settings for aband epa evm tuning */
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG1, 0, mx5g_idac_bleed_bias, 0x1f)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG5, 0, mx5g_idac_bbdc, 0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG5, 0, mx5g_idac_lodc, 0xf)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG8, 0, pad5g_idac_gm, 0xa)
			MOD_RADIO_REG_20691_ENTRY(pi, PA5G_IDAC1, 0, pa5g_idac_main, 0x17)
			MOD_RADIO_REG_20691_ENTRY(pi, PA5G_IDAC3, 0, pa5g_idac_tuning_bias, 0xf)

			/* Utku's second setting for gband epa evm tuning */
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_IDAC2, 0, pad2g_idac_tuning_bias, 6)
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_INCAP, 0, pa2g_idac_incap_compen_main,
				0x2c)
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_IDAC1, 0, pa2g_idac_main, 0x24)
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_IDAC1, 0, pa2g_idac_cas, 0x2b)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX2G_CFG2, 0, mx2g_idac_cascode, 0xc)
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LOGEN2G_CFG1, 0, logen2g_tx_xover, 4)
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_CFG1, 0, logencore_lcbuf_stg1_bias, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_CFG2, 0, logencore_lcbuf_stg2_bias, 7)
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_CFG1, 0, logencore_lcbuf_stg1_ftune,
				0x18)

			/* A band EVM floor tuning, dfu, cfraser */
			MOD_RADIO_REG_20691_ENTRY(pi, TX_TOP_2G_OVR_EAST, 0, ovr_tx2g_bias_pu, 1)
			MOD_RADIO_REG_20691_ENTRY(pi, TX2G_CFG1, 0, tx2g_bias_pu, 1)
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LPF_CFG3, 0, lpf_sel_2g_5g_cmref_gm, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG3, 0, mx5g_pu_bleed, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG8, 0, pad5g_idac_gm, 0x8)
			MOD_RADIO_REG_20691_ENTRY(pi, PA5G_INCAP, 0, pad5g_idac_pmos, 0x18)
			MOD_RADIO_REG_20691_ENTRY(pi, PA5G_IDAC1, 0, pa5g_idac_main, 0x1a)
			MOD_RADIO_REG_20691_ENTRY(pi, TXGM5G_CFG1, 0, pad5g_idac_cascode, 0xf)
		ACPHY_REG_LIST_EXECUTE(pi);
	}

	if ((RADIO20691_MAJORREV(pi->pubpi->radiorev) == 1) && PHY_IPA(pi)) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_INCAP, 0, pa2g_idac_incap_compen_main,
				0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, PA2G_IDAC1, 0, pa2g_idac_cas, 0x20)
			MOD_RADIO_REG_20691_ENTRY(pi, PA5G_IDAC1, 0, pa5g_idac_main, 0x18)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}

static void
wlc_phy_radio20691_xtal_tune_prep(phy_info_t *pi)
{
	/* Enable before changing channel */
	MOD_RADIO_REG_20691(pi, PLL_XTAL2, 0, xtal_pu_RCCAL, 0x1);
	MOD_RADIO_REG_20691(pi, PLL_XTAL2, 0, xtal_pu_RCCAL1, 0x1);
	MOD_RADIO_REG_20691(pi, PLL_XTAL2, 0, xtal_pu_caldrv, 0x1);
}

static void
wlc_phy_radio20691_xtal_tune(phy_info_t *pi)
{
	/* Channel has changed */

	ACPHY_REG_LIST_START
		/* Return the following to zero after channel change */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_RCCAL, 0x0)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_RCCAL1, 0x0)

		/*
		 * pll_xtal2.xtal_pu_caldrv handled differently in driver than in Tcl due to
		 * impact on VCO cal and ucode. SW4345-327
		 */

		/*
		 * These will only work with BT held in reset.
		 * Write the BT CLB register equivalent to 0x0 through backplane to get this to
		 * work in BT+WLAN mode
		 */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL_OVR1, 0, ovr_xtal_pu_corebuf_pfd, 0x1)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_corebuf_pfd, 0x0)

		/* This will only work if BT is held at reset. CANNOT use it in either BT or
		 * BT+WLAN mode
		 */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_BT, 0x0)

		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_corebuf_bb, 0x1)

		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL4, 0, xtal_outbufBBstrg, 0x0)

		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL_OVR1, 0, ovr_xtal_coresize_pmos, 0x1)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL_OVR1, 0, ovr_xtal_coresize_nmos, 0x1)

		/* pll_xtal3[11] (xtal_core_change) is unwired after 4345A0, don't touch */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL3, 0, xtal_refsel, 0x4)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL3, 0, xtal_xtal_swcap_in, 0x4)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL3, 0, xtal_xtal_swcap_out, 0xa)
	ACPHY_REG_LIST_EXECUTE(pi);
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL1, 0, xtal_coresize_pmos, 0x4)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL1, 0, xtal_coresize_nmos, 0x4)

			/* HSIC=On helps in 2G. Keep HSIC ON while reducing buffer strength to
			 * minimum
			 */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_HSIC, 0x1)

			/* 0x3 if doubler ON, 0x1 if doubler OFF */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL4, 0, xtal_xtbufstrg, 0x3)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL4, 0, xtal_outbufstrg, 0x2)

			/* To reduce a few of the output strengths a bit */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL6, 0, xtal_bufstrg_HSIC, 0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL6, 0, xtal_bufstrg_gci, 0x0)

			/* LDO voltage reduced to 4345B0@~1.06V, pll_xtalldo1 = 0x10e */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_ctl, 0x8)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else if (CHSPEC_IS5G(pi->radio_chanspec)) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL1, 0, xtal_coresize_pmos, 0x10)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL1, 0, xtal_coresize_nmos, 0x10)

			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL2, 0, xtal_pu_HSIC, 0x0)

			/* doubler no concern in 5G */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL4, 0, xtal_xtbufstrg, 0x7)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL4, 0, xtal_outbufstrg, 0x4)

			/* To reduce a few of the output strengths a bit */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL6, 0, xtal_bufstrg_HSIC, 0x4)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTAL6, 0, xtal_bufstrg_gci, 0x4)

			/* LDO voltage reduced to 4345B0@~1.06V, pll_xtalldo1 = 0x10e */
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_ctl, 0x8)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_2069_rfpll_150khz(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_LF4, rfpll_lf_lf_r1, 0)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_LF4, rfpll_lf_lf_r2, 2)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF5, 2)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_LF7, rfpll_lf_lf_rs_cm, 2)
		MOD_RADIO_REG_ENTRY(pi, RFP, PLL_LF7, rfpll_lf_lf_rf_cm, 0xff)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF2, 0xffff)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF3, 0xffff)
	ACPHY_REG_LIST_EXECUTE(pi);
}

static void
wlc_phy_2069_4335_set_ovrds(phy_info_t *pi)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	ACPHY_REG_LIST_START
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR30, 0x1df3)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR31, 0x1ffc)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR32, 0x0078)
	ACPHY_REG_LIST_EXECUTE(pi);

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		phy_utils_write_radioreg(pi, RF0_2069_GE16_OVR28, 0x0);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_OVR29, 0x0);
	} else {
		phy_utils_write_radioreg(pi, RF0_2069_GE16_OVR28, 0xffff);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_OVR29, 0xffff);
		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1)
			if (PHY_IPA(pi))
			    phy_utils_write_radioreg(pi, RFP_2069_GE16_OVR29, 0x6900);
	}
}

static void
wlc_phy_2069_4350_set_ovrds(phy_ac_radio_info_t *ri)
{
	uint8 core, afediv_size, afeldo1;
	phy_info_t *pi = ri->pi;
	uint32 fc = wf_channel2mhz(CHSPEC_CHANNEL(pi->radio_chanspec),
		CHSPEC_IS2G(pi->radio_chanspec) ? WF_CHAN_FACTOR_2_4_G
		: WF_CHAN_FACTOR_5_G);
	phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);

	BCM_REFERENCE(stf_shdata);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	ACPHY_REG_LIST_START
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR30, 0x1df3)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR31, 0x1ffc)
		WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR32, 0x0078)

		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_CP, 0x1)
		MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_VCO, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);
	if (PHY_IPA(pi)&&(pi->xtalfreq == 37400000)&&CHSPEC_IS2G(pi->radio_chanspec)) {
		FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, core) {
			MOD_RADIO_REGC(pi, PA2G_CFG1, core, pa2g_bias_reset, 1);
			MOD_RADIO_REGC(pi, GE16_OVR13, core, ovr_pa2g_bias_reset, 1);
			}
	}
	if ((RADIOREV(pi->pubpi->radiorev) == 36) || (RADIOREV(pi->pubpi->radiorev) >= 39)) {
		ACPHY_REG_LIST_START
			MOD_PHYREG_ENTRY(pi, radio_logen2g, idac_qb, 0x2)
			MOD_PHYREG_ENTRY(pi, radio_logen2gN5g, idac_itx, 0x3)
			MOD_PHYREG_ENTRY(pi, radio_logen2gN5g, idac_irx, 0x3)
			MOD_PHYREG_ENTRY(pi, radio_logen2gN5g, idac_qrx, 0x3)
			MOD_PHYREG_ENTRY(pi, radio_logen2g, idac_qtx, 0x3)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_CFG4, rfpll_spare2, 0x6)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_CFG4, rfpll_spare3, 0x34)

			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_TOP_SPARE7, 0x1)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF1, 0x48)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
	if (!PHY_IPA(pi)) {
		if (pi->xtalfreq == 37400000) {
			MOD_RADIO_REG(pi, RFP, PLL_XTALLDO1,
				ldo_1p2_xtalldo1p2_ctl, 0xb);
		}
		if (CHSPEC_IS5G(pi->radio_chanspec)) {
			if (CHSPEC_IS80(pi->radio_chanspec)) {
				switch (fc) {
				case 5690:
					afediv_size = 0xf;
					afeldo1 = 0x7;
					break;
				case 5775:
					afediv_size = 0xf;
					afeldo1 = 0x0;
					break;
				case 5210:
				case 5290:
					afediv_size = 0x8;
					afeldo1 = 0x0;
					break;
				default:
					afediv_size = 0xB;
					afeldo1 = 0x7;
				}
				MOD_RADIO_REG(pi, RFP, GE16_AFEDIV1,
				    afediv_main_driver_size, afediv_size);
				MOD_RADIO_REG(pi, RF1, GE32_PMU_CFG2, AFEldo_adj, afeldo1);
			} else {
				MOD_RADIO_REG(pi, RFP, GE16_AFEDIV1, afediv_main_driver_size,
				     (fc == 5190) ? 0xa : 0x8);
				MOD_RADIO_REG(pi, RF1, GE32_PMU_CFG2, AFEldo_adj, 0x7);
			}
			/* Override PAD gain for core 0 to be 255 */
			MOD_RADIO_REG(pi, RF0, GE16_OVR14, ovr_pad5g_gc, 0x1);
			MOD_RADIO_REG(pi, RF0, PAD5G_CFG1, gc, 0x7F);
			if (ri->data->srom_txnospurmod5g == 1) {
				/* Override PAD gain for core 1 to be 255 */
				MOD_RADIO_REG(pi, RF1, GE16_OVR14, ovr_pad5g_gc, 0x1);
				MOD_RADIO_REG(pi, RF1, PAD5G_CFG1, gc, 0x7F);
			} else {
				ACPHY_REG_LIST_START
					MOD_RADIO_REG_ENTRY(pi, RF1, PAD5G_IDAC, idac_main, 0x28)
					MOD_RADIO_REG_ENTRY(pi, RF1, PAD5G_TUNE, idac_aux, 0x28)
					MOD_RADIO_REGC_ENTRY(pi, PAD5G_INCAP, 1,
						idac_incap_compen_main, 0x8)
					MOD_RADIO_REGC_ENTRY(pi, PAD5G_INCAP, 1,
						idac_incap_compen_aux, 0x8)
				ACPHY_REG_LIST_EXECUTE(pi);
			}
			if (pi->fabid == 4) {
				/* UMC tuning for 5G EVM */
				FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, core) {
					MOD_RADIO_REGC(pi, PAD5G_IDAC, core, idac_main, 0x2);
					MOD_RADIO_REGC(pi, PAD5G_TUNE, core, idac_aux, 0x2a);
					MOD_RADIO_REGC(pi, PAD5G_INCAP, core,
						idac_incap_compen_main, 5);
					MOD_RADIO_REGC(pi, PAD5G_INCAP, core,
						idac_incap_compen_aux, 5);
					MOD_RADIO_REGC(pi, GE16_OVR14, core,
						ovr_pga5g_gainboost, 1);
					MOD_RADIO_REGC(pi, PGA5G_CFG1, core, gainboost, 0xf);
				}
			}
		} else {
			ACPHY_REG_LIST_START
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_AFEDIV1,
					afediv_main_driver_size, 0x8)
				MOD_RADIO_REG_ENTRY(pi, RF1, GE32_PMU_CFG2, AFEldo_adj, 0x0)
			ACPHY_REG_LIST_EXECUTE(pi);
			if ((ri->data->srom_txnospurmod2g == 0) && (pi->xtalfreq == 37400000)) {
				if (fc == 2412) {
				    ACPHY_REG_LIST_START
					/* setting 200khz loopbw */
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_CP4, 0xBC28)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF2, 0xFFD4)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF3, 0xF3F9)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF4, 0xA)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF5, 0xA)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF7, 0xC65)
				    ACPHY_REG_LIST_EXECUTE(pi);
				}
				if (fc == 2467) {
				    ACPHY_REG_LIST_START
					/* setting 200khz loopbw */
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_CP4, 0xBC28)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF2, 0xFDCF)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF3, 0xEDF3)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF4, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF5, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF7, 0xC68)
				    ACPHY_REG_LIST_EXECUTE(pi);
				}
			}
		}
	} else if ((PHY_IPA(pi)) && (CHSPEC_IS80(pi->radio_chanspec))) {
		MOD_RADIO_REG(pi, RFP, GE16_AFEDIV1,
		    afediv_main_driver_size, 0xb);
	}
	/* set xtal_pu_RCCAL1 to 0 by default to avoid it staying high without rcal */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL1, 0);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
wlc_phy_chanspec_radio20691_setup(phy_info_t *pi, uint8 ch, uint8 toggle_logen_reset)
{
	const chan_info_radio20691_t *ci20691;
	uint8 itr = 0;
	uint8 ovr_rxdiv2g_rs = 0;
	uint8 ovr_rxdiv2g_pu_bias = 0;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (wlc_phy_chan2freq_20691(pi, ch, &ci20691) < 0)
		return;

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		/* save the ovr_rxdiv2g radio registers */
		ovr_rxdiv2g_rs = READ_RADIO_REGFLD_20691(pi, RX_TOP_2G_OVR_EAST, 0, ovr_rxdiv2g_rs);
		ovr_rxdiv2g_pu_bias = READ_RADIO_REGFLD_20691(pi, RX_TOP_2G_OVR_EAST, 0,
		                                              ovr_rxdiv2g_pu_bias);

		/* disable trimodal DC WAR */
		MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_EAST, 0, ovr_rxdiv2g_rs, 0);
		MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_EAST, 0, ovr_rxdiv2g_pu_bias, 0);
	}

	wlc_phy_radio20691_xtal_tune_prep(pi);

	/* logen_reset needs to be toggled whenever bandsel bit if changed */
	/* On a bw change, phy_reset is issued which causes currentBand getting reset to 0 */
	/* So, issue this on both band & bw change */
	if (toggle_logen_reset == 1) {
		wlc_phy_logen_reset(pi, 0);
	}

	/* 20691_radio_tune() */
	/* Write chan specific tuning register */
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_VCOCAL1, 0), ci20691->RF_pll_vcocal1);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_VCOCAL11, 0),
	                         ci20691->RF_pll_vcocal11);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_VCOCAL12, 0),
	                         ci20691->RF_pll_vcocal12);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_FRCT2, 0), ci20691->RF_pll_frct2);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_FRCT3, 0), ci20691->RF_pll_frct3);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, LOGEN_CFG2, 0), ci20691->RF_logen_cfg2);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_LF4, 0), ci20691->RF_pll_lf4);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_LF5, 0), ci20691->RF_pll_lf5);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_LF7, 0), ci20691->RF_pll_lf7);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_LF2, 0), ci20691->RF_pll_lf2);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PLL_LF3, 0), ci20691->RF_pll_lf3);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, LOGEN_CFG1, 0), ci20691->RF_logen_cfg1);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, LNA2G_TUNE, 0), ci20691->RF_lna2g_tune);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, TXMIX2G_CFG5, 0),
	                         ci20691->RF_txmix2g_cfg5);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PA2G_CFG2, 0), ci20691->RF_pa2g_cfg2);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, LNA5G_TUNE, 0), ci20691->RF_lna5g_tune);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, TXMIX5G_CFG6, 0),
	                         ci20691->RF_txmix5g_cfg6);
	phy_utils_write_radioreg(pi, RADIO_REG_20691(pi, PA5G_CFG4, 0), ci20691->RF_pa5g_cfg4);

	if (ci20691->chan > CH_MAX_2G_CHANNEL && ci20691->freq < 5250 &&
	    ci20691->freq != 5190 && ci20691->freq != 5230) {
		MOD_RADIO_REG_20691(pi, PLL_VCO2, 0, rfpll_vco_cap_mode, 1);
	} else {
		MOD_RADIO_REG_20691(pi, PLL_VCO2, 0, rfpll_vco_cap_mode, 0);
	}

	/* # The reset/preferred value for ldo_vco and ldo_cp is 0 which is correct
	 * # The above tuning function writes to the whole hvldo register, instead of
	 * read-modify-write (easy for driver guys) and puts back the ldo_vco and ldo_cp
	 * registers to 0 (i.e. to their preferred values)
	 * # The problem is that PHY is not using direct control to turn on ldo_VCO and
	 * ldo_CP and hence it is important to make sure that the radio jtag value for
	 * these is 1 and not 0
	 */

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO1, 0, ldo_2p5_pu_ldo_VCO, 1)
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO1, 0, ldo_2p5_pu_ldo_CP, 1)

		/* 4345a0: Current optimization */
		/* Value for normal power mode, moved out of wlc_phy_radio_tiny_vcocal(). */
		MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO6, 0, rfpll_vco_ALC_ref_ctrl, 0xf)
	ACPHY_REG_LIST_EXECUTE(pi);

	acphy_set_lpmode(pi, ACPHY_LP_RADIO_LVL_OPT);

	/* Do a VCO cal after writing the tuning table regs */
	do {
		wlc_phy_radio_tiny_vcocal(pi);
		itr++;
		if (itr > 2)
			break;
	} while (READ_RADIO_REGFLD_20691(pi, PLL_DSPR27, 0, rfpll_monitor_need_refresh) == 1);

	wlc_phy_radio20691_xtal_tune(pi);

	/* restore the ovr_rxdiv2g radio registers */
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_EAST, 0, ovr_rxdiv2g_rs, ovr_rxdiv2g_rs);
		MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_EAST, 0, ovr_rxdiv2g_pu_bias,
		                    ovr_rxdiv2g_pu_bias);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
wlc_phy_chanspec_radio2069_setup(phy_info_t *pi, const void *chan_info, uint8 toggle_logen_reset)
{
	uint8 core;
	uint32 fc = wf_channel2mhz(CHSPEC_CHANNEL(pi->radio_chanspec),
	        CHSPEC_IS2G(pi->radio_chanspec) ? WF_CHAN_FACTOR_2_4_G
	        : WF_CHAN_FACTOR_5_G);
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);

	BCM_REFERENCE(stf_shdata);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));
	ASSERT(chan_info != NULL);

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	/* logen_reset needs to be toggled whenever bandsel bit if changed */
	/* On a bw change, phy_reset is issued which causes currentBand getting reset to 0 */
	/* So, issue this on both band & bw change */
	if (toggle_logen_reset == 1) {
		wlc_phy_logen_reset(pi, 0);
	}

	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		const chan_info_radio2069revGE32_t *ciGE32 = chan_info;

		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL5, ciGE32->RFP_pll_vcocal5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL6, ciGE32->RFP_pll_vcocal6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL2, ciGE32->RFP_pll_vcocal2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL1, ciGE32->RFP_pll_vcocal1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL11, ciGE32->RFP_pll_vcocal11);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL12, ciGE32->RFP_pll_vcocal12);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT2, ciGE32->RFP_pll_frct2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT3, ciGE32->RFP_pll_frct3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL10, ciGE32->RFP_pll_vcocal10);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL3, ciGE32->RFP_pll_xtal3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2, ciGE32->RFP_pll_vco2);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_CFG1, ciGE32->RFP_logen5g_cfg1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO8, ciGE32->RFP_pll_vco8);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO6, ciGE32->RFP_pll_vco6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO3, ciGE32->RFP_pll_vco3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_XTALLDO1, ciGE32->RFP_pll_xtalldo1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO1, ciGE32->RFP_pll_hvldo1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO2, ciGE32->RFP_pll_hvldo2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO5, ciGE32->RFP_pll_vco5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO4, ciGE32->RFP_pll_vco4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF4, ciGE32->RFP_pll_lf4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF5, ciGE32->RFP_pll_lf5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF7, ciGE32->RFP_pll_lf7);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF2, ciGE32->RFP_pll_lf2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF3, ciGE32->RFP_pll_lf3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_CP4, ciGE32->RFP_pll_cp4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF6, ciGE32->RFP_pll_lf6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL4, ciGE32->RFP_pll_xtal4);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN2G_TUNE, ciGE32->RFP_logen2g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_LNA2G_TUNE, ciGE32->RFX_lna2g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_TXMIX2G_CFG1, ciGE32->RFX_txmix2g_cfg1);
		phy_utils_write_radioreg(pi, RFX_2069_PGA2G_CFG2, ciGE32->RFX_pga2g_cfg2);
		phy_utils_write_radioreg(pi, RFX_2069_PAD2G_TUNE, ciGE32->RFX_pad2g_tune);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE1, ciGE32->RFP_logen5g_tune1);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE2, ciGE32->RFP_logen5g_tune2);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_IDAC1, ciGE32->RFP_logen5g_idac1);
		phy_utils_write_radioreg(pi, RFX_2069_LNA5G_TUNE, ciGE32->RFX_lna5g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_TXMIX5G_CFG1, ciGE32->RFX_txmix5g_cfg1);
		phy_utils_write_radioreg(pi, RFX_2069_PGA5G_CFG2, ciGE32->RFX_pga5g_cfg2);
		phy_utils_write_radioreg(pi, RFX_2069_PAD5G_TUNE, ciGE32->RFX_pad5g_tune);
		if ((RADIO2069_MINORREV(pi->pubpi->radiorev) == 4) &&
		    pi->sh->chippkg == 2 && PHY_XTAL_IS37M4(pi)) {
			if (fc == 5290) {
				ACPHY_REG_LIST_START
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL5, 0X5)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL6, 0x1C)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL2, 0xA09)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL1, 0xF89)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL11, 0xD4)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL12, 0x2A70)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_FRCT2, 0x350)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_FRCT3, 0xA9C1)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL10, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTAL3, 0x488)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO2, 0xCE8)
					WRITE_RADIO_REG_ENTRY(pi, RF0_2069_LOGEN5G_CFG1, 0x40)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO8, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO6, 0x1D6f)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO3, 0x1F00)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTALLDO1, 0x780)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_HVLDO1, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_HVLDO2, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO5, 0x49C)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO4, 0x3504)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF4, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF5, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF7, 0xD6F)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF2, 0xECBE)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF3, 0xDDE2)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_CP4, 0xBC28)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF6, 0x1)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTAL4, 0x36FF)
					WRITE_RADIO_REG_ENTRY(pi, RF0_2069_LOGEN5G_TUNE1, 0x80)
				ACPHY_REG_LIST_EXECUTE(pi);
			} else if (fc == 5180) {
				ACPHY_REG_LIST_START
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL5, 0x5)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL6, 0x1C)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL2, 0xA09)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL1, 0xF37)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL11, 0xCF)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL12, 0xC106)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_FRCT2, 0x33F)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_FRCT3, 0x41B)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCOCAL10, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTAL3, 0x488)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO2, 0xCE8)
					WRITE_RADIO_REG_ENTRY(pi, RF0_2069_LOGEN5G_CFG1, 0x40)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO8, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO6, 0x1D6F)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO3, 0x1F00)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTALLDO1, 0x780)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_HVLDO1, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_HVLDO2, 0x0)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO5, 0x49C)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_VCO4, 0x3505)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF4, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF5, 0xB)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF7, 0xD6D)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF2, 0xF1C3)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF3, 0xE2E7)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_CP4, 0xBC28)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_LF6, 0x1)
					WRITE_RADIO_REG_ENTRY(pi, RFP_2069_PLL_XTAL4, 0x36CF)
					WRITE_RADIO_REG_ENTRY(pi, RF0_2069_LOGEN5G_TUNE1, 0xA0)
				ACPHY_REG_LIST_EXECUTE(pi);
			}
		}
		/* Move nbclip by 2dBs to the right */
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REGC(pi, NBRSSI_CONFG, core, nbrssi_ib_Refladder, 7);
			MOD_RADIO_REGC(pi, DAC_CFG1, core, DAC_invclk, 1);
		}

		/* Fix drift/unlock behavior */
		MOD_RADIO_REG(pi, RFP, PLL_CFG3, rfpll_spare1, 0x8);

	} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
		if ((RADIO2069REV(pi->pubpi->radiorev) != 25) &&
			(RADIO2069REV(pi->pubpi->radiorev) != 26)) {
			const chan_info_radio2069revGE16_t *ciGE16 = chan_info;

			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL5, ciGE16->RFP_pll_vcocal5);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL6, ciGE16->RFP_pll_vcocal6);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL2, ciGE16->RFP_pll_vcocal2);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL1, ciGE16->RFP_pll_vcocal1);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL11,
			                         ciGE16->RFP_pll_vcocal11);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL12,
			                         ciGE16->RFP_pll_vcocal12);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT2, ciGE16->RFP_pll_frct2);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT3, ciGE16->RFP_pll_frct3);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL10,
			                         ciGE16->RFP_pll_vcocal10);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL3, ciGE16->RFP_pll_xtal3);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2, ciGE16->RFP_pll_vco2);
			phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_CFG1,
			                         ciGE16->RFP_logen5g_cfg1);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO8, ciGE16->RFP_pll_vco8);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO6, ciGE16->RFP_pll_vco6);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO3, ciGE16->RFP_pll_vco3);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_XTALLDO1,
			                         ciGE16->RFP_pll_xtalldo1);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO1, ciGE16->RFP_pll_hvldo1);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO2, ciGE16->RFP_pll_hvldo2);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO5, ciGE16->RFP_pll_vco5);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO4, ciGE16->RFP_pll_vco4);

			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF4, ciGE16->RFP_pll_lf4);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF5, ciGE16->RFP_pll_lf5);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF7, ciGE16->RFP_pll_lf7);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF2, ciGE16->RFP_pll_lf2);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF3, ciGE16->RFP_pll_lf3);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_CP4, ciGE16->RFP_pll_cp4);
			phy_utils_write_radioreg(pi, RFP_2069_PLL_LF6, ciGE16->RFP_pll_lf6);

			phy_utils_write_radioreg(pi, RF0_2069_LOGEN2G_TUNE,
			                         ciGE16->RFP_logen2g_tune);
			phy_utils_write_radioreg(pi, RF0_2069_LNA2G_TUNE, ciGE16->RF0_lna2g_tune);
			phy_utils_write_radioreg(pi, RF0_2069_TXMIX2G_CFG1,
			                         ciGE16->RF0_txmix2g_cfg1);
			phy_utils_write_radioreg(pi, RF0_2069_PGA2G_CFG2, ciGE16->RF0_pga2g_cfg2);
			phy_utils_write_radioreg(pi, RF0_2069_PAD2G_TUNE, ciGE16->RF0_pad2g_tune);
			phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE1,
			                         ciGE16->RFP_logen5g_tune1);
			phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE2,
			                         ciGE16->RFP_logen5g_tune2);
			phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_RCCR,
			                         ciGE16->RF0_logen5g_rccr);
			phy_utils_write_radioreg(pi, RF0_2069_LNA5G_TUNE, ciGE16->RF0_lna5g_tune);
			phy_utils_write_radioreg(pi, RF0_2069_TXMIX5G_CFG1,
			                         ciGE16->RF0_txmix5g_cfg1);
			phy_utils_write_radioreg(pi, RF0_2069_PGA5G_CFG2, ciGE16->RF0_pga5g_cfg2);
			phy_utils_write_radioreg(pi, RF0_2069_PAD5G_TUNE, ciGE16->RF0_pad5g_tune);
			/*
			* phy_utils_write_radioreg(pi, RFP_2069_PLL_CP5, ciGE16->RFP_pll_cp5);
			* phy_utils_write_radioreg(pi, RF0_2069_AFEDIV1, ciGE16->RF0_afediv1);
			* phy_utils_write_radioreg(pi, RF0_2069_AFEDIV2, ciGE16->RF0_afediv2);
			* phy_utils_write_radioreg(pi, RF0_2069_ADC_CFG5, ciGE16->RF0_adc_cfg5);
			*/

		} else {
			if (!PHY_XTAL_IS52M(pi)) {
				const chan_info_radio2069revGE25_t *ciGE25 = chan_info;
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL5,
				                         ciGE25->RFP_pll_vcocal5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL6,
				                         ciGE25->RFP_pll_vcocal6);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL2,
				                         ciGE25->RFP_pll_vcocal2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL1,
				                         ciGE25->RFP_pll_vcocal1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL11,
					ciGE25->RFP_pll_vcocal11);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL12,
					ciGE25->RFP_pll_vcocal12);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT2,
				                         ciGE25->RFP_pll_frct2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT3,
				                         ciGE25->RFP_pll_frct3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL10,
					ciGE25->RFP_pll_vcocal10);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL3,
				                         ciGE25->RFP_pll_xtal3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_CFG3,
				                         ciGE25->RFP_pll_cfg3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2,
				                         ciGE25->RFP_pll_vco2);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_CFG1,
					ciGE25->RFP_logen5g_cfg1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO8,
				                         ciGE25->RFP_pll_vco8);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO6,
				                         ciGE25->RFP_pll_vco6);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO3,
				                         ciGE25->RFP_pll_vco3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_XTALLDO1,
					ciGE25->RFP_pll_xtalldo1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO1,
				                         ciGE25->RFP_pll_hvldo1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO2,
				                         ciGE25->RFP_pll_hvldo2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO5,
				                         ciGE25->RFP_pll_vco5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO4,
				                         ciGE25->RFP_pll_vco4);

				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF4, ciGE25->RFP_pll_lf4);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF5, ciGE25->RFP_pll_lf5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF7, ciGE25->RFP_pll_lf7);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF2, ciGE25->RFP_pll_lf2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF3, ciGE25->RFP_pll_lf3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_CP4, ciGE25->RFP_pll_cp4);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF6, ciGE25->RFP_pll_lf6);

				phy_utils_write_radioreg(pi, RF0_2069_LOGEN2G_TUNE,
					ciGE25->RFP_logen2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_LNA2G_TUNE,
				                         ciGE25->RF0_lna2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_TXMIX2G_CFG1,
					ciGE25->RF0_txmix2g_cfg1);
				phy_utils_write_radioreg(pi, RF0_2069_PGA2G_CFG2,
				                         ciGE25->RF0_pga2g_cfg2);
				phy_utils_write_radioreg(pi, RF0_2069_PAD2G_TUNE,
				                         ciGE25->RF0_pad2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE1,
					ciGE25->RFP_logen5g_tune1);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE2,
					ciGE25->RFP_logen5g_tune2);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_RCCR,
					ciGE25->RF0_logen5g_rccr);
				phy_utils_write_radioreg(pi, RF0_2069_LNA5G_TUNE,
				                         ciGE25->RF0_lna5g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_TXMIX5G_CFG1,
					ciGE25->RF0_txmix5g_cfg1);
				phy_utils_write_radioreg(pi, RF0_2069_PGA5G_CFG2,
				                         ciGE25->RF0_pga5g_cfg2);
				phy_utils_write_radioreg(pi, RF0_2069_PAD5G_TUNE,
				                         ciGE25->RF0_pad5g_tune);

				/*
				 * phy_utils_write_radioreg(pi, RFP_2069_PLL_CP5,
				 *                          ciGE25->RFP_pll_cp5);
				 * phy_utils_write_radioreg(pi, RF0_2069_AFEDIV1,
				 *                          ciGE25->RF0_afediv1);
				 * phy_utils_write_radioreg(pi, RF0_2069_AFEDIV2,
				 *                          ciGE25->RF0_afediv2);
				 * phy_utils_write_radioreg(pi, RF0_2069_ADC_CFG5,
				 *                          ciGE25->RF0_adc_cfg5);
				 */

			} else {
				const chan_info_radio2069revGE25_52MHz_t *ciGE25 = chan_info;

				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL5,
				                         ciGE25->RFP_pll_vcocal5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL6,
				                         ciGE25->RFP_pll_vcocal6);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL2,
				                         ciGE25->RFP_pll_vcocal2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL1,
				                         ciGE25->RFP_pll_vcocal1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL11,
					ciGE25->RFP_pll_vcocal11);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL12,
					ciGE25->RFP_pll_vcocal12);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT2,
				                         ciGE25->RFP_pll_frct2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT3,
				                         ciGE25->RFP_pll_frct3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL10,
					ciGE25->RFP_pll_vcocal10);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL3,
				                         ciGE25->RFP_pll_xtal3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2,
				                         ciGE25->RFP_pll_vco2);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_CFG1,
					ciGE25->RFP_logen5g_cfg1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO8,
				                         ciGE25->RFP_pll_vco8);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO6,
				                         ciGE25->RFP_pll_vco6);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO3,
				                         ciGE25->RFP_pll_vco3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_XTALLDO1,
					ciGE25->RFP_pll_xtalldo1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO1,
				                         ciGE25->RFP_pll_hvldo1);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO2,
				                         ciGE25->RFP_pll_hvldo2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO5,
				                         ciGE25->RFP_pll_vco5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO4,
				                         ciGE25->RFP_pll_vco4);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF4, ciGE25->RFP_pll_lf4);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF5, ciGE25->RFP_pll_lf5);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF7, ciGE25->RFP_pll_lf7);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF2, ciGE25->RFP_pll_lf2);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF3, ciGE25->RFP_pll_lf3);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_CP4, ciGE25->RFP_pll_cp4);
				phy_utils_write_radioreg(pi, RFP_2069_PLL_LF6, ciGE25->RFP_pll_lf6);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN2G_TUNE,
					ciGE25->RFP_logen2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_LNA2G_TUNE,
				                         ciGE25->RF0_lna2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_TXMIX2G_CFG1,
					ciGE25->RF0_txmix2g_cfg1);
				phy_utils_write_radioreg(pi, RF0_2069_PGA2G_CFG2,
				                         ciGE25->RF0_pga2g_cfg2);
				phy_utils_write_radioreg(pi, RF0_2069_PAD2G_TUNE,
				                         ciGE25->RF0_pad2g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE1,
					ciGE25->RFP_logen5g_tune1);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE2,
					ciGE25->RFP_logen5g_tune2);
				phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_RCCR,
					ciGE25->RF0_logen5g_rccr);
				phy_utils_write_radioreg(pi, RF0_2069_LNA5G_TUNE,
				                         ciGE25->RF0_lna5g_tune);
				phy_utils_write_radioreg(pi, RF0_2069_TXMIX5G_CFG1,
					ciGE25->RF0_txmix5g_cfg1);
				phy_utils_write_radioreg(pi, RF0_2069_PGA5G_CFG2,
				                         ciGE25->RF0_pga5g_cfg2);
				phy_utils_write_radioreg(pi, RF0_2069_PAD5G_TUNE,
				                         ciGE25->RF0_pad5g_tune);
			}

			/* 43162 FCBGA Settings improving Tx EVM */
			/* (1) ch4/ch4m settings to reduce 500k xtal spur */
			/* (2) Rreducing 2440/2480 RX spur */
			if (RADIO2069REV(pi->pubpi->radiorev) == 25 && PHY_XTAL_IS40M(pi)) {
			  ACPHY_REG_LIST_START
			    MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR23, ovr_xtal_coresize_nmos, 0x1)
			    MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL1, xtal_coresize_nmos, 0x8)
			    MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR23, ovr_xtal_coresize_pmos, 0x1)
			    MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL1, xtal_coresize_pmos, 0x8)
			  ACPHY_REG_LIST_EXECUTE(pi);

			  if (CHSPEC_IS2G(pi->radio_chanspec)) {
			    if (CHSPEC_CHANNEL(pi->radio_chanspec) < 12) {
			      if (CHSPEC_CHANNEL(pi->radio_chanspec) == 4)
						phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2,
						                         0xce4);
			      ACPHY_REG_LIST_START
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL5, xtal_bufstrg_BT, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR27, ovr_xtal_xtbufstrg, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL4, xtal_xtbufstrg, 0x7)
			        MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR27, ovr_xtal_xtbufstrg, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL4, xtal_outbufstrg, 0x3)
			      ACPHY_REG_LIST_EXECUTE(pi);
			    } else {
			      ACPHY_REG_LIST_START
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL5, xtal_bufstrg_BT, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR27, ovr_xtal_xtbufstrg, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL4, xtal_xtbufstrg, 0x0)
			        MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR27, ovr_xtal_xtbufstrg, 0x1)
			        MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTAL4, xtal_outbufstrg, 0x1)
			      ACPHY_REG_LIST_EXECUTE(pi);
			    }
			  }
			}
		}
	} else {
		const chan_info_radio2069_t *ci = chan_info;

		/* Write chan specific tuning register */
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL5, ci->RFP_pll_vcocal5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL6, ci->RFP_pll_vcocal6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL2, ci->RFP_pll_vcocal2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL1, ci->RFP_pll_vcocal1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL11, ci->RFP_pll_vcocal11);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL12, ci->RFP_pll_vcocal12);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT2, ci->RFP_pll_frct2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_FRCT3, ci->RFP_pll_frct3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCOCAL10, ci->RFP_pll_vcocal10);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_XTAL3, ci->RFP_pll_xtal3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2, ci->RFP_pll_vco2);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_CFG1, ci->RF0_logen5g_cfg1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO8, ci->RFP_pll_vco8);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO6, ci->RFP_pll_vco6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO3, ci->RFP_pll_vco3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_XTALLDO1, ci->RFP_pll_xtalldo1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO1, ci->RFP_pll_hvldo1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_HVLDO2, ci->RFP_pll_hvldo2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO5, ci->RFP_pll_vco5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO4, ci->RFP_pll_vco4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF4, ci->RFP_pll_lf4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF5, ci->RFP_pll_lf5);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF7, ci->RFP_pll_lf7);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF2, ci->RFP_pll_lf2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_LF3, ci->RFP_pll_lf3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_CP4, ci->RFP_pll_cp4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP1, ci->RFP_pll_dsp1);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP2, ci->RFP_pll_dsp2);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP3, ci->RFP_pll_dsp3);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP4, ci->RFP_pll_dsp4);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP6, ci->RFP_pll_dsp6);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP7, ci->RFP_pll_dsp7);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP8, ci->RFP_pll_dsp8);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_DSP9, ci->RFP_pll_dsp9);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN2G_TUNE, ci->RF0_logen2g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_LNA2G_TUNE, ci->RFX_lna2g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_TXMIX2G_CFG1, ci->RFX_txmix2g_cfg1);
		phy_utils_write_radioreg(pi, RFX_2069_PGA2G_CFG2, ci->RFX_pga2g_cfg2);
		phy_utils_write_radioreg(pi, RFX_2069_PAD2G_TUNE, ci->RFX_pad2g_tune);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE1, ci->RF0_logen5g_tune1);
		phy_utils_write_radioreg(pi, RF0_2069_LOGEN5G_TUNE2, ci->RF0_logen5g_tune2);
		phy_utils_write_radioreg(pi, RFX_2069_LOGEN5G_RCCR, ci->RFX_logen5g_rccr);
		phy_utils_write_radioreg(pi, RFX_2069_LNA5G_TUNE, ci->RFX_lna5g_tune);
		phy_utils_write_radioreg(pi, RFX_2069_TXMIX5G_CFG1, ci->RFX_txmix5g_cfg1);
		phy_utils_write_radioreg(pi, RFX_2069_PGA5G_CFG2, ci->RFX_pga5g_cfg2);
		phy_utils_write_radioreg(pi, RFX_2069_PAD5G_TUNE, ci->RFX_pad5g_tune);
		phy_utils_write_radioreg(pi, RFP_2069_PLL_CP5, ci->RFP_pll_cp5);
		phy_utils_write_radioreg(pi, RF0_2069_AFEDIV1, ci->RF0_afediv1);
		phy_utils_write_radioreg(pi, RF0_2069_AFEDIV2, ci->RF0_afediv2);
		phy_utils_write_radioreg(pi, RFX_2069_ADC_CFG5, ci->RFX_adc_cfg5);

		/* We need different values for ADC_CFG5 for cores 1 and 2
		 * in order to get the best reduction of spurs from the AFE clk
		 */
		if (RADIO2069REV(pi->pubpi->radiorev) < 4) {
			ACPHY_REG_LIST_START
				WRITE_RADIO_REG_ENTRY(pi, RF1_2069_ADC_CFG5, 0x3e9)
				WRITE_RADIO_REG_ENTRY(pi, RF2_2069_ADC_CFG5, 0x3e9)
				MOD_RADIO_REG_ENTRY(pi, RFP, PLL_CP4, rfpll_cp_ioff, 0xa0)
			ACPHY_REG_LIST_EXECUTE(pi);
		}

		/* Reduce 500 KHz spur at fc=2427 MHz for both 4360 A0 and B0 */
		if (CHSPEC_CHANNEL(pi->radio_chanspec) == 4 &&
			((RADIO2069_MINORREV(pi->pubpi->radiorev) != 16) &&
				(RADIO2069_MINORREV(pi->pubpi->radiorev) != 17))) {
			phy_utils_write_radioreg(pi, RFP_2069_PLL_VCO2, 0xce4);
			MOD_RADIO_REG(pi, RFP, PLL_XTAL4, xtal_xtbufstrg, 0x5);
		}

		/* 43602 XTAL SPUR 2G WAR */
		if (ACMAJORREV_5(pi->pubpi->phy_rev) && CHSPEC_IS2G(pi->radio_chanspec)&&
				!(IS_4364_3x3(pi))) {
			MOD_RADIO_REG(pi, RFP, PLL_XTAL4, xtal_xtbufstrg, 0x3);
			MOD_RADIO_REG(pi, RFP, PLL_XTAL4, xtal_outbufstrg, 0x2);
		}

		/* Move nbclip by 2dBs to the right */
		MOD_RADIO_REG(pi, RFX, NBRSSI_CONFG, nbrssi_ib_Refladder, 7);

		/* 5g only: Changing RFPLL bandwidth to be 150MHz */
		if (CHSPEC_IS5G(pi->radio_chanspec) && !(IS_4364_3x3(pi)))
			wlc_2069_rfpll_150khz(pi);

		if ((RADIO2069_MINORREV(pi->pubpi->radiorev) == 7) &&
		   (BFCTL(pi->u.pi_acphy) == 3)) {
			if ((BF3_FEMCTRL_SUB(pi->u.pi_acphy) == 3 ||
			     BF3_FEMCTRL_SUB(pi->u.pi_acphy) == 5) &&
			    CHSPEC_IS2G(pi->radio_chanspec)) {
			  ACPHY_REG_LIST_START
			    /* 43602 MCH2: offtune to win back linear output power */
			    MOD_RADIO_REG_ENTRY(pi, RFX, PAD2G_TUNE, pad2g_tune, 0x1)

			    /* increase gain */
			    MOD_RADIO_REG_ENTRY(pi, RFX, PGA2G_CFG1, pga2g_gainboost, 0x2)
			    WRITE_RADIO_REG_ENTRY(pi, RFX_2069_PAD2G_INCAP, 0x7e7e)
			    MOD_RADIO_REG_ENTRY(pi, RFX, PAD2G_IDAC, pad2g_idac_main, 0x38)
			    MOD_RADIO_REG_ENTRY(pi, RFX, PGA2G_INCAP, pad2g_idac_aux, 0x38)
			    MOD_RADIO_REG_ENTRY(pi, RFX, PAD2G_IDAC, pad2g_idac_cascode, 0xe)
			  ACPHY_REG_LIST_EXECUTE(pi);
			} else if (BF3_FEMCTRL_SUB(pi->u.pi_acphy) == 0 &&
			           CHSPEC_IS5G(pi->radio_chanspec)) {
			  ACPHY_REG_LIST_START
			    /* 43602 MCH5: increase gain to win back linear output power */
			    MOD_RADIO_REGC_ENTRY(pi, TXMIX5G_CFG1, 2, gainboost, 0x4)
			    MOD_RADIO_REGC_ENTRY(pi, PGA5G_CFG1, 2, gainboost, 0x4)
			    MOD_RADIO_REG_ENTRY(pi, RFX, PAD5G_IDAC, idac_main, 0x3d)
			    MOD_RADIO_REG_ENTRY(pi, RFX, PAD5G_TUNE, idac_aux, 0x3d)
			  ACPHY_REG_LIST_EXECUTE(pi);
			}
		}
	}

	if (RADIO2069REV(pi->pubpi->radiorev) >= 4) {
		/* Make clamping stronger */
		phy_utils_write_radioreg(pi, RFX_2069_ADC_CFG5, 0x83e0);
	}

	if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) &&
	    (!(PHY_IPA(pi)))) {
	    MOD_RADIO_REG(pi, RFP, PLL_CP4, rfpll_cp_ioff, 0xe0);
	}

	/* increasing pabias to get good evm with pagain3 */
	if ((RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) &&
	    !(ACRADIO_2069_EPA_IS(pi->pubpi->radiorev))) {
		phy_utils_write_radioreg(pi, RF0_2069_PA5G_IDAC2, 0x8484);

		if (PHY_IPA(pi)) {
			ACPHY_REG_LIST_START
				WRITE_RADIO_REG_ENTRY(pi, RF0_2069_LOGEN5G_IDAC1, 0x3F37)
				WRITE_RADIO_REG_ENTRY(pi, RF0_2069_PGA5G_IDAC, 0x3838)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_OVR2, ovr_bg_pulse, 1)
				MOD_RADIO_REG_ENTRY(pi, RFP, GE16_BG_CFG1, bg_pulse, 1)
			ACPHY_REG_LIST_EXECUTE(pi);

			FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, core) {
				MOD_RADIO_REGC(pi, PAD5G_IDAC, core, idac_main, 0x20);
				MOD_RADIO_REGC(pi, PAD5G_TUNE, core, idac_aux, 0x20);

				MOD_RADIO_REGC(pi, PA5G_INCAP, core,
					pa5g_idac_incap_compen_main, 0x8);
				MOD_RADIO_REGC(pi, PA5G_INCAP, core,
					pa5g_idac_incap_compen_aux, 0x8);
				MOD_RADIO_REGC(pi, PA2G_INCAP, core,
					pa2g_ptat_slope_incap_compen_main, 0x0);
				MOD_RADIO_REGC(pi, PA2G_INCAP, core,
					pa2g_ptat_slope_incap_compen_aux, 0x0);

				if (pi->sh->chippkg == BCM4335_FCBGA_PKG_ID) {
					MOD_RADIO_REGC(pi, PA2G_CFG2, core,
						pa2g_bias_filter_main, 0x1);
					MOD_RADIO_REGC(pi, PA2G_CFG2, core,
						pa2g_bias_filter_aux, 0x1);
				} else {
					MOD_RADIO_REGC(pi, PA2G_CFG2, core,
						pa2g_bias_filter_main, 0x3);
					MOD_RADIO_REGC(pi, PA2G_CFG2, core,
						pa2g_bias_filter_aux, 0x3);
				}

				MOD_RADIO_REGC(pi, PAD5G_INCAP, core, idac_incap_compen_main, 0xc);
				MOD_RADIO_REGC(pi, PAD5G_INCAP, core, idac_incap_compen_aux, 0xc);
				MOD_RADIO_REGC(pi, PGA5G_INCAP, core, idac_incap_compen, 0x8);
				MOD_RADIO_REGC(pi, PA5G_CFG2, core, pa5g_bias_cas, 0x58);
				MOD_RADIO_REGC(pi, PA5G_IDAC2, core, pa5g_biasa_main, 0x84);
				MOD_RADIO_REGC(pi, PA5G_IDAC2, core, pa5g_biasa_aux, 0x84);
				MOD_RADIO_REGC(pi, GE16_OVR21, core, ovr_mix5g_gainboost, 0x1);
				MOD_RADIO_REGC(pi, TXMIX5G_CFG1, core, gainboost, 0x0);
				MOD_RADIO_REGC(pi, TXGM_CFG1, core, gc_res, 0x0);
				MOD_RADIO_REGC(pi, PA2G_CFG3, core, pa2g_ptat_slope_main, 0x7);
				MOD_RADIO_REGC(pi, PAD2G_SLOPE, core, pad2g_ptat_slope_main, 0x7);
				MOD_RADIO_REGC(pi, PGA2G_CFG2, core, pga2g_ptat_slope_main, 0x7);
				MOD_RADIO_REGC(pi, PGA2G_IDAC, core, pga2g_idac_main, 0x15);
				MOD_RADIO_REGC(pi, PAD2G_TUNE, core, pad2g_idac_tuning_bias, 0xc);
				MOD_RADIO_REGC(pi, TXMIX2G_CFG1, core, lodc, 0x3);
			}
		}
	}
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
		wlc_phy_2069_4335_set_ovrds(pi);
	} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		wlc_phy_2069_4350_set_ovrds(pi->u.pi_acphy->radioi);
	}

	/* Offset RCCAL by 4 when using divide by 5 for ADC stability */
	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2 && (!PHY_IPA(pi))) {
		FOREACH_CORE(pi, core) {
			if ((!(fc == 5190) && CHSPEC_IS40(pi->radio_chanspec)) ||
			    (CHSPEC_IS20(pi->radio_chanspec) &&
			     ((((fc == 5180) && (pi->sh->chippkg != 2)) ||
			       ((fc >= 5200) && (fc <= 5320)) ||
			       ((fc >= 5745) && (fc <= 5825)))))) {
				MOD_RADIO_REGC(pi, ADC_RC1, core, adc_ctl_RC_4_0,
				               pi_ac->radioi->rccal_adc_rc + 4);
			} else {
				MOD_RADIO_REGC(pi, ADC_RC1, core, adc_ctl_RC_4_0,
				               pi_ac->radioi->rccal_adc_rc);
			}
		}
	}

	/* 4335C0: Current optimization */
	acphy_set_lpmode(pi, ACPHY_LP_RADIO_LVL_OPT);

	/* Do a VCO cal after writing the tuning table regs */
	wlc_phy_radio2069_vcocal(pi);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

/**
 * initialize the static tables defined in auto-generated wlc_phytbl_ac.c,
 * see acphyprocs.tcl, proc acphy_init_tbls
 * After called in the attach stage, all the static phy tables are reclaimed.
 */
static void
WLBANDINITFN(wlc_phy_static_table_download_acphy)(phy_info_t *pi)
{
	uint idx;
	uint8 stall_val;
	stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);
	ACPHY_DISABLE_STALL(pi);
	if (pi->phy_init_por) {
		/* these tables are not affected by phy reset, only power down */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			if ((wlc_phy_ac_phycap_maxbw(pi) > BW_20MHZ)) {
				for (idx = 0; idx < acphytbl_info_sz_rev40; idx++) {
					wlc_phy_table_write_ext_acphy(pi,
						&acphytbl_info_rev40[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi,
					acphyzerotbl_info_rev40,
					acphyzerotbl_info_cnt_rev40);
			}
			else {
				for (idx = 0; idx < acphytbl_info_sz_maxbw20_rev40; idx++) {
					wlc_phy_table_write_ext_acphy(pi,
						&acphytbl_info_maxbw20_rev40[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi,
					acphyzerotbl_info_maxbw20_rev40,
					acphyzerotbl_info_cnt_maxbw20_rev40);
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			for (idx = 0; idx < acphytbl_info_sz_rev36; idx++) {
				wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev36[idx]);
			}
			wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev36,
				acphyzerotbl_info_cnt_rev36);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
		if (ACMAJORREV_32(pi->pubpi->phy_rev) ||
		           ACMAJORREV_33(pi->pubpi->phy_rev) ||
		           ACMAJORREV_37(pi->pubpi->phy_rev)) {
			for (idx = 0; idx < acphytbl_info_sz_rev32; idx++) {
				wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev32[idx]);
			}
			if (ACMAJORREV_33(pi->pubpi->phy_rev) ||
			    ACMAJORREV_37(pi->pubpi->phy_rev)) {
				for (idx = 0; idx < acphytbl_info_sz_rev33; idx++) {
					wlc_phy_table_write_ext_acphy(pi,
						&acphytbl_info_rev33[idx]);
				}
			}
			wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev32,
				acphyzerotbl_info_cnt_rev32);
		} else
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5))
		if (ACMAJORREV_5(pi->pubpi->phy_rev)) {
			for (idx = 0; idx < acphytbl_info_sz_rev9; idx++) {
				wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev9[idx]);
			}
			wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev9,
				acphyzerotbl_info_cnt_rev9);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_5)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
		if (ACMAJORREV_2(pi->pubpi->phy_rev)) {
			if (ACREV_IS(pi->pubpi->phy_rev, 9)) { /* 43602a0 */
				for (idx = 0; idx < acphytbl_info_sz_rev9; idx++) {
					wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev9[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev9,
					acphyzerotbl_info_cnt_rev9);
			} else {
				for (idx = 0; idx < acphytbl_info_sz_rev3; idx++) {
					wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev3[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev3,
				acphyzerotbl_info_cnt_rev3);
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
		if (ACMAJORREV_1(pi->pubpi->phy_rev)) {
			if (ACREV_IS(pi->pubpi->phy_rev, 6))	{
				for (idx = 0; idx < acphytbl_info_sz_rev6; idx++) {
					wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev6[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev6,
					acphyzerotbl_info_cnt_rev6);
			} else {
				for (idx = 0; idx < acphytbl_info_sz_rev2; idx++) {
					wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev2[idx]);
				}
				wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev2,
					acphyzerotbl_info_cnt_rev2);
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0))
		if (ACMAJORREV_0(pi->pubpi->phy_rev)) {
			for (idx = 0; idx < acphytbl_info_sz_rev0; idx++) {
				wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev0[idx]);
			}
			wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev0,
				acphyzerotbl_info_cnt_rev0);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_0)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
		if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
			uint16 phymode = phy_get_phymode(pi);
			for (idx = 0; idx < acphytbl_info_sz_rev12; idx++) {
				wlc_phy_table_write_ext_acphy(pi, &acphytbl_info_rev12[idx]);
			}
			wlc_phy_clear_static_table_acphy(pi, acphyzerotbl_info_rev12,
				acphyzerotbl_info_cnt_rev12);

			if ((phymode == PHYMODE_MIMO) || (phymode == PHYMODE_80P80)) {
				wlc_phy_clear_static_table_acphy(pi,
					acphyzerotbl_delta_MIMO_80P80_info_rev12,
					acphyzerotbl_delta_MIMO_80P80_info_cnt_rev12);
			}
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		{
		}
	}

	ACPHY_ENABLE_STALL(pi, stall_val);
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
/*  R Calibration (takes ~50us) */
static void
wlc_phy_radio_tiny_rcal(phy_info_t *pi, acphy_tiny_rcal_modes_t calmode)
{
	/* Format: 20691_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* The rcalcode argument works only with mode 1 and is optional. */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP. This is what driver should */
	/* do but is not good for bringup because the OTP is not programmed yet. */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */

	uint8 rcal_valid, rcal_value;
	uint8 core = 0;
	bool is_mode3_and_rsdb_core1 = FALSE;
	uint16 macmode = phy_get_phymode(pi);
	uint8 curr_core = phy_get_current_core(pi);
	phy_info_t *other_pi = phy_get_other_pi(pi), *tmp_pi;

	/* Skip this function for QT */
	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(TINY_RADIO(pi));


	if ((calmode == TINY_RCAL_MODE3_BOTH_CORE_CAL) &&
		(macmode == PHYMODE_RSDB) && (curr_core == PHY_RSBD_PI_IDX_CORE1)) {
		is_mode3_and_rsdb_core1 = TRUE;
	}

	if ((calmode == TINY_RCAL_MODE2_SINGLE_CORE_CAL) ||
		(calmode == TINY_RCAL_MODE3_BOTH_CORE_CAL)) {

		FOREACH_CORE(pi, core) {
			if (calmode == TINY_RCAL_MODE2_SINGLE_CORE_CAL) {
				if (((macmode == PHYMODE_RSDB) &&
					(curr_core == PHY_RSBD_PI_IDX_CORE1)) ||
					((macmode != PHYMODE_RSDB) && (core == 1))) {
					break;
				}
			}

			/* This should run only for core 0 */
			/* Run R Cal and use its output */
			tmp_pi = is_mode3_and_rsdb_core1 ? other_pi : pi;

			MOD_RADIO_REG_TINY(tmp_pi, PLL_XTAL2, 0, xtal_pu_RCCAL1, 0x1);
			MOD_RADIO_REG_TINY(tmp_pi, PLL_XTAL2, 0, xtal_pu_RCCAL, 0x1);

			/* Make connection with the external 10k resistor */
			/* Also powers up rcal (RF_rcal_cfg.rcal_pu = 1) */
			MOD_RADIO_REG_TINY(pi, GPAIO_SEL2, core, gpaio_pu, 1);
			MOD_RADIO_REG_TINY(pi, RCAL_CFG_NORTH, core, rcal_pu, 0);
			MOD_RADIO_REG_TINY(pi, GPAIO_SEL0, core, gpaio_sel_0to15_port, 0);
			MOD_RADIO_REG_TINY(pi, GPAIO_SEL1, core, gpaio_sel_16to31_port, 0);
			MOD_RADIO_REG_TINY(pi, RCAL_CFG_NORTH, core, rcal_pu, 1);

			SPINWAIT(READ_RADIO_REGFLD_TINY(pi, RCAL_CFG_NORTH,
				core, rcal_valid) == 0,
				ACPHY_SPINWAIT_RCAL_STATUS);
			if (READ_RADIO_REGFLD_TINY(pi, RCAL_CFG_NORTH,
					core, rcal_valid) == 0) {
				PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR :", __FUNCTION__));
				PHY_FATAL_ERROR_MESG(("RCAL is not valid for Core%d\n", core));
				PHY_FATAL_ERROR(pi, PHY_RC_RCAL_INVALID);
			}
			rcal_valid =  READ_RADIO_REGFLD_TINY(pi, RCAL_CFG_NORTH,
					core, rcal_valid);

			if (rcal_valid == 1) {
				rcal_value = READ_RADIO_REGFLD_TINY(pi, RCAL_CFG_NORTH,
					core, rcal_value) >> 1;

				if (pi->fabid == BCM4349_UMC_FAB_ID) {
					/* In UMC B0 & B1 materials, core0 reference voltage
					 * (Vref) used by RCAL is ~150mV lower from target value
					 * 0.52V It creates 1~2 codes lower than optimal value
					 * 1 RCAL code corresponds to ~150mV change in Vref
					 * So it is recommend to add one code to core0 RCAL
					 * results in ATE
					 */
					if (!((macmode == PHYMODE_RSDB) ? curr_core : core))
						rcal_value += 1;
				}

				PHY_TRACE(("*** rcal_value: %x\n", rcal_value));


				/* Use the output of the rcal engine */
				MOD_RADIO_REG_TINY(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0);
				if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
					/* Copy the rcal value instead of latching */
					MOD_RADIO_REG_TINY(pi, BG_OVR1, core,
						ovr_bg_rcal_trim,  1);
					MOD_RADIO_REG_TINY(pi, BG_CFG1, core,
						bg_rcal_trim,  rcal_value);
				}
				else { /* Use latched output of rcal engine */
					MOD_RADIO_REG_TINY(pi, BG_OVR1, core,
						ovr_bg_rcal_trim, 0);
				}
				/* Very coarse sanity check */
				if ((rcal_value < 2) || (12 < rcal_value)) {
					PHY_ERROR(("*** ERROR: R Cal value out of range."
					" 4bit Rcal = %d.\n", rcal_value));
				}
			} else {
				PHY_ERROR(("%s RCal unsucessful. RCal valid bit is %d.\n",
					__FUNCTION__, rcal_valid));
			}
			/* Power down blocks not needed anymore */
			MOD_RADIO_REG_TINY(pi, GPAIO_SEL2, core, gpaio_pu, 0);
			MOD_RADIO_REG_TINY(pi, PLL_XTAL2, core, xtal_pu_RCCAL1, 0);
			MOD_RADIO_REG_TINY(pi, PLL_XTAL2, core, xtal_pu_RCCAL,  0);
			MOD_RADIO_REG_TINY(pi, RCAL_CFG_NORTH, core, rcal_pu, 0);
		}

		if (calmode == TINY_RCAL_MODE2_SINGLE_CORE_CAL) {
			/* for RSDB/MIMO core 1, copy rcal value from pi of core 0 */
			if (macmode != PHYMODE_RSDB) {
				rcal_value = READ_RADIO_REGFLD_TINY(pi, BG_CFG1,
					0, bg_rcal_trim);
				core = 1;
			} else {
				phy_info_t *pi_core0 = phy_get_pi(pi, PHY_RSBD_PI_IDX_CORE0);
				rcal_value = READ_RADIO_REGFLD_TINY(pi_core0, BG_CFG1,
					0, bg_rcal_trim);
				core = 0;
			}
			MOD_RADIO_REG_TINY(pi, BG_CFG1, core, bg_rcal_trim,  rcal_value);
			MOD_RADIO_REG_TINY(pi, BG_OVR1, core, ovr_bg_rcal_trim,  1);
		}
	} else {
		FOREACH_CORE(pi, core) {
			if (calmode == TINY_RCAL_MODE0_OTP) {
				if ((pi->sromi->rcal_otp_val >= 2) &&
					(pi->sromi->rcal_otp_val <= 12)) {
					/* Use OTP stored rcal value */
					MOD_RADIO_REG_TINY(pi, BG_OVR1, core, ovr_otp_rcal_sel, 1);
				} else {
					wlc_phy_radio_tiny_rcal(pi, TINY_RCAL_MODE1_STATIC);
				}
			} else if (calmode == TINY_RCAL_MODE1_STATIC) {

				if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID) &&
					((RADIO20691_MAJORREV(pi->pubpi->radiorev) == 0) ||
					((RADIO20691_MAJORREV(pi->pubpi->radiorev) == 1) &&
					((RADIO20691_MINORREV(pi->pubpi->radiorev) < 5) ||
					((RADIO20691_MINORREV(pi->pubpi->radiorev) == 16)))))) {
					/* before 4345c0  and 4364 1x1 */
					rcal_value = 11;
				} else if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID) &&
					((RADIO20691_MAJORREV(pi->pubpi->radiorev) == 1) &&
					((RADIO20691_MINORREV(pi->pubpi->radiorev) == 17)))) {
					/* 4364B0 1x1 */
					rcal_value = 8;

				} else {
					/* 4345c0 and later */
					if (pi->fabid == BCM4349_TSMC_FAB_ID)
						rcal_value = TSMC_FAB_RCAL_VAL;
					else
						rcal_value = UMC_FAB_RCAL_VAL;
				}
				MOD_RADIO_REG_TINY(pi, BG_CFG1, core, bg_rcal_trim, rcal_value);
				MOD_RADIO_REG_TINY(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0);
				MOD_RADIO_REG_TINY(pi, BG_OVR1, core, ovr_bg_rcal_trim, 1);
			}
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio_tiny_rcal_wave2(phy_info_t *pi, uint8 mode)
{
	/* Format: 20691_r_cal [<mode>] [<rcalcode>] */
	/* If no arguments given, then mode is assumed to be 1 */
	/* The rcalcode argument works only with mode 1 and is optional. */
	/* If given, then that is the rcal value what will be used in the radio. */

	/* Documentation: */
	/* Mode 0 = Don't run cal, use rcal value stored in OTP. This is what driver should */
	/* do but is not good for bringup because the OTP is not programmed yet. */
	/* Mode 1 = Don't run cal, don't use OTP rcal value, use static rcal */
	/* value given here. Good for initial bringup. */
	/* Mode 2 = Run rcal and use the return value in bandgap. Needs the external 10k */
	/* resistor to be connected to GPAIO otherwise cal will return bogus value. */

	uint8 rcal_valid, loop_iter, rcal_value;
	uint8 core = 0;
	/* Skip this function for QT */
	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(TINY_RADIO(pi));
	if (mode == 0) {
		/* Use OTP stored rcal value */
		MOD_RADIO_PLLREG_20693(pi, BG_OVR1, 0, ovr_otp_rcal_sel, 1);
	} else if (mode == 1) {
		/* Default RCal code to be used with mode 1 is 8 */
		MOD_RADIO_PLLREG_20693(pi, BG_CFG1, core, bg_rcal_trim, 8);
		MOD_RADIO_PLLREG_20693(pi, BG_OVR1, core, ovr_otp_rcal_sel, 0);
		MOD_RADIO_PLLREG_20693(pi, BG_OVR1, core, ovr_bg_rcal_trim, 1);
	} else if (mode == 2) {
		/* This should run only for core 0 */
		/* Run R Cal and use its output */
		MOD_RADIO_PLLREG_20693(pi, RCAL_CFG_NORTH, 0, rcal_pu, 1);
		MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG1, 0, wl_xtal_out_pu, 0x33);

		/* Make connection with the external 10k resistor */
		/* clear all the test-point */
		MOD_RADIO_ALLREG_20693(pi, GPAIO_SEL0, gpaio_sel_0to15_port, 0);
		MOD_RADIO_ALLREG_20693(pi, GPAIO_SEL1, gpaio_sel_16to31_port, 0);
		MOD_RADIO_REG_20693(pi, GPAIOTOP_SEL0, 0,
		                    top_gpaio_sel_0to15_port, 0);
		MOD_RADIO_REG_20693(pi, GPAIOTOP_SEL1, 0,
		                    top_gpaio_sel_16to31_port, 0);
		MOD_RADIO_REG_20693(pi, GPAIOTOP_SEL2, 0, top_gpaio_pu, 1);
		MOD_RADIO_PLLREG_20693(pi, RCAL_CFG_NORTH, 0, rcal_pu, 1);

		rcal_valid = 0;
		loop_iter = 0;
		while ((rcal_valid == 0) && (loop_iter <= 100)) {
			OSL_DELAY(1000);
			rcal_valid = READ_RADIO_PLLREGFLD_20693(pi, RCAL_CFG_NORTH, 0, rcal_valid);
			loop_iter ++;
		}

		if (rcal_valid == 1) {
			rcal_value = READ_RADIO_PLLREGFLD_20693(pi,
				RCAL_CFG_NORTH, 0, rcal_value) >> 1;

			/* Use the output of the rcal engine */
			MOD_RADIO_PLLREG_20693(pi, BG_OVR1, 0, ovr_otp_rcal_sel, 0);
			/* MOD_RADIO_PLLREG_20693(pi, BG_CFG1, 0, bg_rcal_trim, rcal_value); */
			MOD_RADIO_PLLREG_20693(pi, BG_OVR1, 0, ovr_bg_rcal_trim, 0);

			/* Very coarse sanity check */
			if ((rcal_value < 2) || (12 < rcal_value)) {
				PHY_ERROR(("*** ERROR: R Cal value out of range."
				           " 4bit Rcal = %d.\n", rcal_value));
			}
		} else {
			PHY_ERROR(("%s RCal unsucessful. RCal valid bit is %d.\n",
			           __FUNCTION__, rcal_valid));
		}

		/* Power down blocks not needed anymore */
		MOD_RADIO_ALLREG_20693(pi, GPAIO_SEL2, gpaio_pu, 0);
		MOD_RADIO_REG_20693(pi, GPAIOTOP_SEL2, 0, top_gpaio_pu, 0);
		MOD_RADIO_PLLREG_20693(pi, RCAL_CFG_NORTH, 0, rcal_pu, 0);
		MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG1, 0, wl_xtal_out_pu, 0x3);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

static void
wlc_phy_radio_tiny_rccal(phy_ac_radio_info_t *ri)
{
	uint8 cal;
	uint16 n0, n1;

	/* lpf, adc, dacbuf */
	uint8 sr[] = {0x1, 0x1, 0x0};
	uint8 sc[] = {0x0, 0x1, 0x2};
	uint8 x1[] = {0x1c, 0x70, 0x40};
	uint16 trc[] = {0x22d, 0xf0, 0x10a};
	phy_info_t *pi = ri->pi;
	uint32 dn;

	if (IS_4364_1x1(pi)) {
	   sr[2] = 0;
	   sc[2] = 0x1;
	   x1[2] = 0x68;
	   trc[2] = 0x120;
	}

	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(TINY_RADIO(pi));

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	/* For RSDB core 1, copy core 0 values to core 1 for rc cal and return */
	if ((phy_get_phymode(pi) == PHYMODE_RSDB) &&
		(phy_get_current_core(pi) != PHY_RSBD_PI_IDX_CORE0) &&
		(ACMAJORREV_4(pi->pubpi->phy_rev))) {
		phy_info_t *pi_rsdb0 = phy_get_pi(pi, PHY_RSBD_PI_IDX_CORE0);
		ri->data->rccal_gmult = pi_rsdb0->u.pi_acphy->radioi->data->rccal_gmult;
		ri->data->rccal_gmult_rc = pi_rsdb0->u.pi_acphy->radioi->data->rccal_gmult_rc;
		ri->rccal_adc_gmult = pi_rsdb0->u.pi_acphy->radioi->rccal_adc_gmult;
		ri->data->rccal_dacbuf = pi_rsdb0->u.pi_acphy->radioi->data->rccal_dacbuf;
		return;
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */

	/* Powerup rccal driver & set divider radio (rccal needs to run at 20mhz) */
	MOD_RADIO_REG_TINY(pi, PLL_XTAL2, 0, xtal_pu_RCCAL, 1);

	/* Calibrate lpf, adc, dacbuf */
	for (cal = 0; cal < NUM_2069_RCCAL_CAPS; cal++) {
		/* Setup */
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG0, 0, rccal_sr, sr[cal]);
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG0, 0, rccal_sc, sc[cal]);
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG1, 0, rccal_X1, x1[cal]);
		phy_utils_write_radioreg(pi, RADIO_REG(pi, RCCAL_CFG2, 0), trc[cal]);

		/* For dacbuf force fixed dacbuf cap to be 0 while calibration, restore it later */
		if (cal == 2) {
			MOD_RADIO_REG_TINY(pi, TX_DAC_CFG5, 0, DACbuf_fixed_cap, 0);
			MOD_RADIO_REG_TINY(pi, TX_BB_OVR1, 0, ovr_DACbuf_fixed_cap, 1);
		}

		/* Toggle RCCAL power */
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG0, 0, rccal_pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG0, 0, rccal_pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG1, 0, rccal_START, 1);

		/* This delay is required before reading the RCCAL_CFG3
		*   without this delay, the cal values are incorrect.
		*/
		OSL_DELAY(100);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		SPINWAIT(READ_RADIO_REGFLD_TINY(pi, RCCAL_CFG3, 0, rccal_DONE) == 0,
				MAX_2069_RCCAL_WAITLOOPS);
		if (READ_RADIO_REGFLD_TINY(pi, RCCAL_CFG3, 0, rccal_DONE) == 0) {
			PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR : RCCAL invalid \n",
				__FUNCTION__));
			PHY_FATAL_ERROR(pi, PHY_RC_RCCAL_INVALID);
		}

		/* Stop RCCAL */
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG1, 0, rccal_START, 0);

		n0 = READ_RADIO_REGFLD_TINY(pi, RCCAL_CFG4, 0, rccal_N0);
		n1 = READ_RADIO_REGFLD_TINY(pi, RCCAL_CFG5, 0, rccal_N1);
		dn = n1 - n0; /* set dn [expr {$N1 - $N0}] */

		if (cal == 0) {
			/* lpf */
			/* set k [expr {$is_adc ? 102051 : 101541}] */
			/* set gmult_p12 [expr {$prod1 / $fxtal_pm12}] */
			ri->data->rccal_gmult = (101541 * dn) / (pi->xtalfreq >> 12);
			ri->data->rccal_gmult_rc = ri->data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
			            __FUNCTION__, ri->data->rccal_gmult));
		} else if (cal == 1) {
			/* adc */
			/* set k [expr {$is_adc ? 102051 : 101541}] */
			/* set gmult_p12 [expr {$prod1 / $fxtal_pm12}] */
			ri->rccal_adc_gmult = (85000 * dn) / (pi->xtalfreq >> 12);
			PHY_INFORM(("wl%d: %s rccal_adc = %d\n", pi->sh->unit,
			            __FUNCTION__, ri->rccal_adc_gmult));
		} else {
			/* dacbuf */
			ri->data->rccal_dacbuf = READ_RADIO_REGFLD_TINY(pi, RCCAL_CFG6, 0,
			                                              rccal_raw_dacbuf);
			MOD_RADIO_REG_TINY(pi, TX_BB_OVR1, 0, ovr_DACbuf_fixed_cap, 0);
			PHY_INFORM(("wl%d: %s rccal_dacbuf = %d\n", pi->sh->unit,
				__FUNCTION__, ri->data->rccal_dacbuf));
		}
		/* Turn off rccal */
		MOD_RADIO_REG_TINY(pi, RCCAL_CFG0, 0, rccal_pu, 0);
	}
	/* Powerdown rccal driver */
	MOD_RADIO_REG_TINY(pi, PLL_XTAL2, 0, xtal_pu_RCCAL, 0);
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio_tiny_rccal_wave2(phy_ac_radio_info_t *ri)
{
	uint8 cal, done;
	uint16 rccal_itr, n0, n1;

	/* lpf, adc, dacbuf */
	uint8 sr[] = {0x1, 0x1, 0x0};
	uint8 sc[] = {0x0, 0x2, 0x1};
	uint8 x1[] = {0x1c, 0x70, 0x68};
	uint16 trc[] = {0x22d, 0xf0, 0x134};
	phy_info_t *pi = ri->pi;
	uint32 dn;

	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(TINY_RADIO(pi));

	/* Powerup rccal driver & set divider radio (rccal needs to run at 20mhz) */
	MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG1, 0, wl_xtal_out_pu, 0x23);

	/* Calibrate lpf, adc, dacbuf */
	for (cal = 0; cal < NUM_2069_RCCAL_CAPS; cal++) {
		/* Setup */
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG0, 0, rccal_sr, sr[cal]);
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG0, 0, rccal_sc, sc[cal]);
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG1, 0, rccal_X1, x1[cal]);
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG2, 0, rccal_Trc, trc[cal]);

		/* For dacbuf force fixed dacbuf cap to be 0 while calibration, restore it later */
		if (cal == 2) {
			MOD_RADIO_ALLREG_20693(pi, TX_DAC_CFG5, DACbuf_fixed_cap, 0);
			MOD_RADIO_ALLREG_20693(pi, TX_BB_OVR1, ovr_DACbuf_fixed_cap, 1);
		}

		/* Toggle RCCAL power */
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG0, 0, rccal_pu, 0);
		OSL_DELAY(1);
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG0, 0, rccal_pu, 1);

		OSL_DELAY(35);

		/* Start RCCAL */
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG1, 0, rccal_START, 1);

		/* Wait for rcal to be done, max = 100us * 100 = 10ms  */
		done = 0;
		for (rccal_itr = 0;
			(rccal_itr < MAX_2069_RCCAL_WAITLOOPS) && (done == 0);
			rccal_itr++) {
			OSL_DELAY(100);
			done = READ_RADIO_PLLREGFLD_20693(pi, RCCAL_CFG3, 0, rccal_DONE);
		}

		/* Stop RCCAL */
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG1, 0, rccal_START, 0);

		/* Make sure that RC Cal ran to completion */
		ASSERT(done);

		n0 = READ_RADIO_PLLREGFLD_20693(pi, RCCAL_CFG4, 0, rccal_N0);
		n1 = READ_RADIO_PLLREGFLD_20693(pi, RCCAL_CFG5, 0, rccal_N1);
		dn = n1 - n0; /* set dn [expr {$N1 - $N0}] */

		if (cal == 0) {
			/* lpf */
			/* set k [expr {$is_adc ? (($def(d11_radio_major_rev) == 3)? */
			/* 85000 : 102051) : 101541}] */
			/* set gmult_p12 [expr {$prod1 / $fxtal_pm12}] */
			ri->data->rccal_gmult = (101541 * dn) / (pi->xtalfreq >> 12);
			ri->data->rccal_gmult_rc = ri->data->rccal_gmult;
			PHY_INFORM(("wl%d: %s rccal_lpf_gmult = %d\n", pi->sh->unit,
			            __FUNCTION__, ri->data->rccal_gmult));
		} else if (cal == 1) {
			/* adc */
			/* set k [expr {$is_adc ? (($def(d11_radio_major_rev) == 3)? */
			/* 85000 : 102051) : 101541}] */
			/* set gmult_p12 [expr {$prod1 / $fxtal_pm12}] */
			ri->rccal_adc_gmult = (85000 * dn) / (pi->xtalfreq >> 12);
			PHY_INFORM(("wl%d: %s rccal_adc = %d\n", pi->sh->unit,
			            __FUNCTION__, ri->rccal_adc_gmult));
		} else {
			/* dacbuf */
			ri->data->rccal_dacbuf = READ_RADIO_PLLREGFLD_20693(pi, RCCAL_CFG6, 0,
			                                              rccal_raw_dacbuf);
			MOD_RADIO_ALLREG_20693(pi, TX_BB_OVR1, ovr_DACbuf_fixed_cap, 0);
			PHY_INFORM(("wl%d: %s rccal_dacbuf = %d\n", pi->sh->unit,
				__FUNCTION__, ri->data->rccal_dacbuf));
		}
		/* Turn off rccal */
		MOD_RADIO_PLLREG_20693(pi, RCCAL_CFG0, 0, rccal_pu, 0);
	}

	MOD_RADIO_PLLREG_20693(pi, WL_XTAL_CFG1, 0, wl_xtal_out_pu, 0x3);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

/* ***************************** */
/*		External Defintions			*/
/* ***************************** */


/*
Initialize chip regs(RWP) & tables with init vals that do not get reset with phy_reset
*/
static void
wlc_phy_set_regtbl_on_pwron_acphy(phy_info_t *pi)
{
	uint8 core;
	uint16 val;
	uint16 rfseq_bundle_48[3];
	uint sicoreunit;

	bool flag2rangeon = ((CHSPEC_IS2G(pi->radio_chanspec) &&
		phy_tpc_get_tworangetssi2g(pi->tpci)) ||
		(CHSPEC_IS5G(pi->radio_chanspec) &&
		phy_tpc_get_tworangetssi5g(pi->tpci))) && PHY_IPA(pi);
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	/* force afediv(core 0, 1, 2) always high */
	if (!(ACMAJORREV_2(pi->pubpi->phy_rev) || ACMAJORREV_5(pi->pubpi->phy_rev)))
		WRITE_PHYREG(pi, AfeClkDivOverrideCtrl, 0x77);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(pi->pubpi->phy_rev)) {
		WRITE_PHYREG(pi, AfeClkDivOverrideCtrlN0, 0x3);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */

	ACPHY_REG_LIST_START
		/* Remove RFCTRL signal overrides for all cores */
		ACPHYREG_BCAST_ENTRY(pi, RfctrlIntc0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeCfg0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideGains0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfCT0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfSwtch0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeCfg0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLowPwrCfg0, 0)
		ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAuxTssi0, 0)
		ACPHYREG_BCAST_ENTRY(pi, AfectrlOverride0, 0)
	ACPHY_REG_LIST_EXECUTE(pi);
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_5) || defined(PHY_ACMAJORREV_36)))
	if ((ACMAJORREV_2(pi->pubpi->phy_rev) && !ACMINORREV_0(pi)) ||
			ACMAJORREV_5(pi->pubpi->phy_rev) ||
			ACMAJORREV_36(pi->pubpi->phy_rev)) {
		ACPHYREG_BCAST(pi, AfeClkDivOverrideCtrlN0, 0);
		WRITE_PHYREG(pi, AfeClkDivOverrideCtrl, 0);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_5) || defined(PHY_ACMAJORREV_36))) */

	/* logen_pwrup = 1, logen_reset = 0 */
	ACPHYREG_BCAST(pi, RfctrlCoreTxPus0, 0x80);
	ACPHYREG_BCAST(pi, RfctrlOverrideTxPus0, 0x180);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2))
	/* Force LOGENs on both cores in the 5G to be powered up for 435x */
	if (ACMAJORREV_2(pi->pubpi->phy_rev) && !ACMINORREV_0(pi)) {
		ACPHY_REG_LIST_START
			MOD_PHYREG_ENTRY(pi, RfctrlOverrideTxPus0, logen5g_lob_pwrup, 1)
			MOD_PHYREG_ENTRY(pi, RfctrlCoreTxPus0, logen5g_lob_pwrup, 1)
			MOD_PHYREG_ENTRY(pi, RfctrlOverrideTxPus1, logen5g_lob_pwrup, 1)
			MOD_PHYREG_ENTRY(pi, RfctrlCoreTxPus1, logen5g_lob_pwrup, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_2)) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	/* 43012A0 related programming */
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		ACPHY_REG_LIST_START
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideAfeDivCfg0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideETCfg0, 0x0)
			WRITE_PHYREG_ENTRY(pi, AfeClkDivOverrideCtrl28nm0, 0x0)
			WRITE_PHYREG_ENTRY(pi, LogenOverrideCtrl28nm0, 0x0)
			WRITE_PHYREG_ENTRY(pi, ETOverrideCtrl28nm0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfCtrlCorePwrSw0, 0x0)
			WRITE_PHYREG_ENTRY(pi, dyn_radiob0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideNapPus0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideLogenBias0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideExtraAfeDivCfg0, 0x0)
			WRITE_PHYREG_ENTRY(pi, Extra1AfeClkDivOverrideCtrl28nm0, 0x0)
			WRITE_PHYREG_ENTRY(pi, Extra2AfeClkDivOverrideCtrl28nm0, 0x0)
			MOD_PHYREG_ENTRY(pi, Dac_gain0, afe_iqdac_att, 0x000F)
			MOD_PHYREG_ENTRY(pi, RfctrlCmd, syncResetEn, 0x0)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
	if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
		/* 7271 related programming */
		ACPHY_REG_LIST_START
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeDivCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideETCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, AfeClkDivOverrideCtrl28nm0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, LogenOverrideCtrl28nm0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfCtrlCorePwrSw0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, dyn_radiob0, 0x0)
			MOD_PHYREG_ENTRY(pi, Dac_gain0, afe_iqdac_att, 0xf)
			MOD_PHYREG_ENTRY(pi, Dac_gain1, afe_iqdac_att, 0xf)
			MOD_PHYREG_ENTRY(pi, Dac_gain2, afe_iqdac_att, 0xf)
			MOD_PHYREG_ENTRY(pi, Dac_gain3, afe_iqdac_att, 0xf)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideNapPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLogenBias0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideExtraAfeDivCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, Extra1AfeClkDivOverrideCtrl28nm0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, Extra2AfeClkDivOverrideCtrl28nm0, 0x0)
			MOD_PHYREG_ENTRY(pi, RfctrlCmd, syncResetEn, 0)

			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideGlobalPus, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlCmd, 0x306)
			WRITE_PHYREG_ENTRY(pi, AfeClkDivOverrideCtrl, 0x1)
			ACPHYREG_BCAST_ENTRY(pi, AfeClkDivOverrideCtrlN0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlIntc0, 0x0)

			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideTxPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideRxPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideGains0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfCT0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfSwtch0, 0x0)

			//TCL has the following register write set to 0x0
			//ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeDivCfg0, 0x8f0b)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLowPwrCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAuxTssi0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, AfectrlOverride0, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlCoreGlobalPus, 0x4)
			//using phy override
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideGlobalPus, 0xd)
			MOD_PHYREG_ENTRY(pi, RfctrlCmd, chip_pu, 1)
		ACPHY_REG_LIST_EXECUTE(pi);

		// only one pll and need only 0x80 and 0x81 bundle commands(pll(ldo)pu reset)
		// bundle commands 0x85 and 0x86 are used for Rx farrow reset
		rfseq_bundle_48[0] = 0x703c;
		rfseq_bundle_48[1] = 0x1c;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 0, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x7014;
		rfseq_bundle_48[1] = 0x1c;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 1, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x0000;
		rfseq_bundle_48[1] = 0x00;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 2, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x0000;
		rfseq_bundle_48[1] = 0x00;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 3, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x0000;
		rfseq_bundle_48[1] = 0x00;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 4, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x0400;
		rfseq_bundle_48[1] = 0x00;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 5, 48,
				rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x0000;
		rfseq_bundle_48[1] = 0x00;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 6, 48,
				rfseq_bundle_48);
		MOD_PHYREG(pi, RfctrlCmd, bundleScheme2, 0x1);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	if (ACMAJORREV_40(pi->pubpi->phy_rev)) {

		PHY_INFORM(("rfseq_bundle_tbl : {0x01C7034 0x01C7014 0x02C703E 0x02C701C} \n"));

		/* set rfseq_bundle_tbl {0x01C7034 0x01C7014 0x02C703E 0x02C701C} */
		/* acphy_write_table RfseqBundle $rfseq_bundle_tbl 0 */
		rfseq_bundle_48[0] = 0x703C;
		rfseq_bundle_48[1] = 0x1c;
		rfseq_bundle_48[2] = 0x0;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 0, 48, rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x7014;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 1, 48, rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x700e;
		rfseq_bundle_48[1] = 0x2c;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 2, 48, rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x702c;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 3, 48, rfseq_bundle_48);
		rfseq_bundle_48[0] = 0x6020;
		rfseq_bundle_48[1] = 0x20;
		wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 4, 48, rfseq_bundle_48);

#ifdef BCMINTPHYDBG
		/* For verification */
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 0, 48, &rfseq_bundle_48);
		PHY_INFORM(("rfseq_bundle_tbl : offset %d, 0x%04x%04x%04x\n", 0,
			rfseq_bundle_48[2], rfseq_bundle_48[1], rfseq_bundle_48[0]));
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 1, 48, &rfseq_bundle_48);
		PHY_INFORM(("rfseq_bundle_tbl : offset %d, 0x%04x%04x%04x\n", 1,
			rfseq_bundle_48[2], rfseq_bundle_48[1], rfseq_bundle_48[0]));
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 2, 48, &rfseq_bundle_48);
		PHY_INFORM(("rfseq_bundle_tbl : offset %d, 0x%04x%04x%04x\n", 2,
			rfseq_bundle_48[2], rfseq_bundle_48[1], rfseq_bundle_48[0]));
		wlc_phy_table_read_acphy(pi, ACPHY_TBL_ID_RFSEQBUNDLE, 1, 3, 48, &rfseq_bundle_48);
		PHY_INFORM(("rfseq_bundle_tbl : offset %d, 0x%04x%04x%04x\n", 3,
			rfseq_bundle_48[2], rfseq_bundle_48[1], rfseq_bundle_48[0]));
#endif

		/* apply CLB FEM priority static settings */
		wlc_phy_set_clb_femctrl_static_settings(pi);

		/* Reset pktproc state and force RESET2RX sequence
		This is required to allow bundle commands loading in RFseq
		before we switch to bundle controls RfctrlCmd=0x306
		*/
		WRITE_PHYREG(pi, RfseqMode, 0);
		wlc_phy_resetcca_acphy(pi);

		/* Remove RFCTRL signal overrides for all cores */
		ACPHY_REG_LIST_START
			WRITE_PHYREG_ENTRY(pi, RfctrlOverrideGlobalPus, 0x0)
			WRITE_PHYREG_ENTRY(pi, RfctrlCmd, 0x306)
			WRITE_PHYREG_ENTRY(pi, AfeClkDivOverrideCtrl, 0x1)
			ACPHYREG_BCAST_ENTRY(pi, AfeClkDivOverrideCtrlN0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlIntc0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideTxPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideRxPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideGains0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfCT0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLpfSwtch0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAfeDivCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideETCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLowPwrCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideAuxTssi0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideLogenBias0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideExtraAfeDivCfg0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, RfctrlOverrideNapPus0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, AfectrlOverride0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, AfeClkDivOverrideCtrl28nm0, 0x0)
			ACPHYREG_BCAST_ENTRY(pi, LogenOverrideCtrl28nm0, 0x0)
		ACPHY_REG_LIST_EXECUTE(pi);
		/* for 4347, aux slice, setting rfpll_clk_buffer_pu using phy override */
		if (wlapi_si_coreunit(pi->sh->physhim) == DUALMAC_AUX) {
			WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0xc);
		}
		else {
			WRITE_PHYREG(pi, RfctrlCoreGlobalPus, 0x4);
		}
		WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, 0xd);
		MOD_PHYREG(pi, RfctrlCmd, chip_pu, 0x1);

	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
	{
	}

	/* Switch off rssi2 & rssi3 as they are not used in normal operation */
	ACPHYREG_BCAST(pi, RfctrlCoreRxPus0, 0);
	ACPHYREG_BCAST(pi, RfctrlOverrideRxPus0, 0x5000);

	/* Disable the SD-ADC's overdrive detect feature */
	val = READ_PHYREG(pi, RfctrlCoreAfeCfg20) |
	      ACPHY_RfctrlCoreAfeCfg20_afe_iqadc_reset_ov_det_MASK(pi->pubpi->phy_rev);
	/* val |= ACPHY_RfctrlCoreAfeCfg20_afe_iqadc_clamp_en_MASK; */
	ACPHYREG_BCAST(pi, RfctrlCoreAfeCfg20, val);
	val = READ_PHYREG(pi, RfctrlOverrideAfeCfg0) |
	      ACPHY_RfctrlOverrideAfeCfg0_afe_iqadc_reset_ov_det_MASK(pi->pubpi->phy_rev);
	/* val |= ACPHY_RfctrlOverrideAfeCfg0_afe_iqadc_clamp_en_MASK(pi->pubpi->phy_rev); */
	ACPHYREG_BCAST(pi, RfctrlOverrideAfeCfg0, val);

	/* initialize all the tables defined in auto-generated wlc_phytbl_ac.c,
	 * see acphyprocs.tcl, proc acphy_init_tbls
	 *  skip static one after first up
	 */
	PHY_TRACE(("wl%d: %s, dnld tables = %d\n", pi->sh->unit,
	           __FUNCTION__, pi->phy_init_por));

	/* these tables are not affected by phy reset, only power down */
	if (!RADIOID_IS(pi->pubpi->radioid, BCM20691_ID))
		wlc_phy_static_table_download_acphy(pi);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(pi->pubpi->phy_rev)) {
		/* wihr 0x15a 0x8000 */

		/* acphy_rfctrl_override bg_pulse 1 */
		/* Override is not required now ..bg pulsing is enabled along with tssi sleep */
		/* val  = READ_PHYREG(pi, RfctrlCoreTxPus0); */
		/* val |= (1 << 14); */
		/* WRITE_PHYREG(pi, ACPHY_RfctrlCoreTxPus0, val); */
		/* val = READ_PHYREG(pi, RfctrlOverrideTxPus0); */
		/* val |= (1 << 9); */
		/* WRITE_PHYREG(pi, ACPHY_RfctrlOverrideTxPus0, val); */

	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	/* Initialize idle-tssi to be -420 before doing idle-tssi cal */
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_path0, -190);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
	{
		ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_path0, 0x25C);
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_second_path0, 0x25C);
			ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_third_path0, 0x25C);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_1)))
	if ((ACMAJORREV_1(pi->pubpi->phy_rev) || ACMAJORREV_2(pi->pubpi->phy_rev)) &&
	    BF3_TSSI_DIV_WAR(pi->u.pi_acphy)) {
		ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_second_path0, 0x25C);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_1))) */

	if (flag2rangeon) {
		ACPHYREG_BCAST(pi, TxPwrCtrlIdleTssi_second_path0, 0x25C);
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_1)))
	/* Enable the Ucode TSSI_DIV WAR */
	if ((ACMAJORREV_1(pi->pubpi->phy_rev) || ACMAJORREV_2(pi->pubpi->phy_rev)) &&
	    BF3_TSSI_DIV_WAR(pi->u.pi_acphy)) {
		wlapi_bmac_mhf(pi->sh->physhim, MHF2, MHF2_PPR_HWPWRCTL, MHF2_PPR_HWPWRCTL,
		               WLC_BAND_ALL);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_2) || defined(PHY_ACMAJORREV_1))) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_36)))
	if (ACMAJORREV_4(pi->pubpi->phy_rev) || ACMAJORREV_36(pi->pubpi->phy_rev)) {
		uint8 lutinitval = 0;
		uint16 offset;
		phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);

		BCM_REFERENCE(stf_shdata);

		if (BF3_TSSI_DIV_WAR(pi->u.pi_acphy)) {
			wlapi_bmac_mhf(pi->sh->physhim, MHF2, MHF2_PPR_HWPWRCTL, MHF2_PPR_HWPWRCTL,
			WLC_BAND_2G);
		}
		/* Clean up the estPwrShiftLUT Table */
		/* Table is 26bit witdth, we are writing 32bit at a time */
		FOREACH_ACTV_CORE(pi, stf_shdata->hw_phyrxchain, core) {
			for (offset = 0; offset < 40; offset++) {
				wlc_phy_table_write_acphy(pi,
					wlc_phy_get_tbl_id_estpwrshftluts(pi, core),
					1, offset, 32, &lutinitval);
			}
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_4) || defined(PHY_ACMAJORREV_36))) */

	if (TINY_RADIO(pi)) {
		FOREACH_CORE(pi, core) {
			MOD_PHYREGCE(pi, RfctrlCoreRxPus, core, fast_nap_bias_pu, 0);
			MOD_PHYREGCE(pi, RfctrlOverrideRxPus, core, fast_nap_bias_pu, 1);
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
	/* Temp 4345 register poweron default updates - */
	/* These need to be farmed out to correct locations */
	if (ACMAJORREV_3(pi->pubpi->phy_rev)) {
		ACPHY_REG_LIST_START
			MOD_PHYREG_ENTRY(pi, RxSdFeConfig5, rx_farow_scale_value, 7)
			MOD_PHYREG_ENTRY(pi, FSTRHiPwrTh, finestr_hiPwrSm_th, 77)
			MOD_PHYREG_ENTRY(pi, FSTRHiPwrTh, finestr_hiPwr_th, 63)
			MOD_PHYREG_ENTRY(pi, crsThreshold2l, peakThresh, 77)
			MOD_PHYREG_ENTRY(pi, crsminpoweruSub10, crsminpower0, 54)
			MOD_PHYREG_ENTRY(pi, crsminpoweruSub10, crsminpower0, 54)
			MOD_PHYREG_ENTRY(pi, crsminpoweru0, crsminpower0, 54)
			MOD_PHYREG_ENTRY(pi, bphyaciThresh0, bphyaciThresh0, 0)
			MOD_PHYREG_ENTRY(pi, bphyaciPwrThresh2, bphyaciPwrThresh2, 0)
			MOD_PHYREG_ENTRY(pi, RxSdFeSampCap, capture_both, 0)
			/* MOD_PHYREG_ENTRY(pi, TxMacDelay, macdelay, 680) */
			MOD_PHYREG_ENTRY(pi, AfeseqRx2TxPwrUpDownDly20M, delayPwrUpDownRx2Tx20M, 3)
			MOD_PHYREG_ENTRY(pi, RfctrlOverrideTxPus0, bg_pulse, 1)
			MOD_PHYREG_ENTRY(pi, PapdEnable0, gain_dac_rf_override0, 1)
			MOD_PHYREG_ENTRY(pi, RfctrlCoreTxPus0, bg_pulse, 1)
			MOD_PHYREG_ENTRY(pi, PapdEnable0, gain_dac_rf_reg0, 0)
			MOD_PHYREG_ENTRY(pi, RfseqMode, CoreActv_override, 0)
			MOD_PHYREG_ENTRY(pi, RfctrlOverrideLowPwrCfg0, rxrf_lna2_gm_size, 0)
			MOD_PHYREG_ENTRY(pi, Rfpll_resetCtrl, rfpll_reset_dur, 64)
			MOD_PHYREG_ENTRY(pi, CoreConfig, NumRxAnt, 1)
			MOD_PHYREG_ENTRY(pi, Pllldo_resetCtrl, pllldo_reset_dur, 8000)
			MOD_PHYREG_ENTRY(pi, bphyaciPwrThresh0, bphyaciPwrThresh0, 0)
			MOD_PHYREG_ENTRY(pi, bphyaciThresh1, bphyaciThresh1, 0)
			MOD_PHYREG_ENTRY(pi, HTSigTones, support_gf, 1)
			MOD_PHYREG_ENTRY(pi, ViterbiControl0, CacheHitEn, 1)
			MOD_PHYREG_ENTRY(pi, sampleDepthCount, DepthCount, 19)
			MOD_PHYREG_ENTRY(pi, bphyaciPwrThresh1, bphyaciPwrThresh1, 0)
			MOD_PHYREG_ENTRY(pi, Core0cliploGainCodeB, clip1loTrTxIndex, 0)
			MOD_PHYREG_ENTRY(pi, clip2carrierDetLen, clip2carrierDetLen, 64)
			MOD_PHYREG_ENTRY(pi, Core0clip2GainCodeA, clip2LnaIndex, 1)
			MOD_PHYREG_ENTRY(pi, Core0clipmdGainCodeB, clip1mdBiQ0Index, 0)
			MOD_PHYREG_ENTRY(pi, Core0clip2GainCodeB, clip2BiQ1Index, 2)
			MOD_PHYREG_ENTRY(pi, Core0cliploGainCodeA, clip1loLnaIndex, 1)
			MOD_PHYREG_ENTRY(pi, Core0cliploGainCodeB, clip1loBiQ0Index, 0)
			MOD_PHYREG_ENTRY(pi, Core0clipHiGainCodeB, clip1hiBiQ0Index, 0)
			MOD_PHYREG_ENTRY(pi, Core0clipmdGainCodeA, clip1mdmixergainIndex, 3)
			MOD_PHYREG_ENTRY(pi, Core0cliploGainCodeB, clip1loTrRxIndex, 1)
			WRITE_PHYREG_ENTRY(pi, Core0InitGainCodeB, 0x2204)
			MOD_PHYREG_ENTRY(pi, AfectrlCore10, adc_pd, 1)
		ACPHY_REG_LIST_EXECUTE(pi);
	}

	if (ACMAJORREV_3(pi->pubpi->phy_rev)) {
		uint8 zeroval[4] = { 0 };
		uint16 x;
		uint8 stall_val = READ_PHYREGFLD(pi, RxFeCtrl1, disable_stalls);

		ACPHY_DISABLE_STALL(pi);

		for (x = 0; x < 256; x++) {
			/* Zero out the NV Noise Shaping Table for 4345A0 */
			wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_NVNOISESHAPINGTBL,
			                          1, x, 32, zeroval);

			/* Zero out the NV Rx EVM Table for 4345A0 */
			wlc_phy_table_write_acphy(pi, ACPHY_TBL_ID_NVRXEVMSHAPINGTBL,
			                          1, x, 8, zeroval);
		}
		ACPHY_ENABLE_STALL(pi, stall_val);

		/* Disable scram dyn en for 4345A0 */
		wlc_acphy_set_scramb_dyn_bw_en((wlc_phy_t *)pi, 0);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */


	if ((TINY_RADIO(pi) || (ACMAJORREV_36(pi->pubpi->phy_rev))) &&
		(PHY_IPA(pi) || phy_papdcal_epacal(pi->papdcali))) {
		/* Zero out paplutselect table used in txpwrctrl */
		uint8 initvalue = 0;
		uint16 j;
		for (j = 0; j < 128; j++) {
			wlc_phy_table_write_acphy(pi,
				ACPHY_TBL_ID_PAPDLUTSELECT0, 1, j, 8, &initvalue);
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
			if (ACMAJORREV_4(pi->pubpi->phy_rev) &&
				(phy_get_phymode(pi) == PHYMODE_MIMO)) {
				wlc_phy_table_write_acphy(pi,
					ACPHY_TBL_ID_PAPDLUTSELECT1, 1, j, 8, &initvalue);
			}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		// still empty here
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
	FOREACH_CORE(pi, core) {
		if (ACMAJORREV_40(pi->pubpi->phy_rev) && PHY_IPA(pi) &&
				(sicoreunit == DUALMAC_AUX)) {
			MOD_RADIO_REG_20694(pi, RF, TX2G_MISC_CFG1, core, cal2g_pa_atten, 2);
			MOD_RADIO_REG_20694(pi, RF, RX2G_REG1, core, rx2g_iloopback_attn, 2);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
static void  wlc_phy_set_clb_femctrl_static_settings(phy_info_t *pi)
{
	uint16 swctrl2glinesAnt0 = 0, swctrl2glinesAnt1 = 0;

	/* CLB FEM priority static settings */
	/* Only 2G switch control lines should allow the BT prisel mux logic */
	if (PHYCORENUM((pi)->pubpi->phy_corenum) >= 2) {
		swctrl2glinesAnt0 = pi->u.pi_acphy->sromi->nvram_femctrl_clb.map_2g[0][0] |
			pi->u.pi_acphy->sromi->nvram_femctrl_clb.map_2g[1][0];
		swctrl2glinesAnt1 = pi->u.pi_acphy->sromi->nvram_femctrl_clb.map_2g[0][1] |
			pi->u.pi_acphy->sromi->nvram_femctrl_clb.map_2g[1][1];
	}

	/* 4347_clb_reg_write clb_swctrl_smask_coresel_ant0_en 1 */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_08, 0x1<<29, 0x1<<29);
	/* 4347_clb_reg_write clb_swctrl_smask_coresel_ant1_en 1 */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_08, 0x1<<30, 0x1<<30);

	/* clb_swctrl_dmask_bt_ant0[9:0]: enable BT mux logic on 2G switch control lines */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_08, 0x3ff<<17,
		(0x3ff & ~swctrl2glinesAnt0)<<17);

	/* disable BT AoA override by default; uCode to enable dynamically for AoA grants */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_07, 0x3<<18, 0);
	/* clb_swctrl_smask_wlan_ant0[9:0] */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_07, 0x3ff<<20, 0x3ff<<20);

	/* clb_swctrl_dmask_bt_ant1[9:0]: enable BT mux logic on 2G switch control lines */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_10, 0x3ff<<10,
		(0x3ff & ~swctrl2glinesAnt1)<<10);

	/* clb_swctrl_smask_wlan_ant1[9:0] */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_11, 0x3ff<<0, 0x3ff<<0);

	/* program btc prisel mask (main/aux) */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_08, 0x3<<27,
		pi->u.pi_acphy->sromi->nvram_femctrl_clb.btc_prisel_mask<<27);

	/* program btc prisel ant mask */
	si_gci_chipcontrol(pi->sh->sih, CC_GCI_CHIPCTRL_11, 0x3<<26,
		pi->u.pi_acphy->sromi->nvram_femctrl_clb.btc_prisel_ant_mask<<26);

	/* Force WLAN PRISEL - otherwise for 4347B0 Aux-2G, BT stays in control.
	* This is causing incorrect lna2g_reg1 values. Also these lna2g_reg1 values
	* could not be changed using radio override bits.
	* Force WLAN priority until the priority control issue is resolved
	*/
	if (ACMAJORREV_40(pi->pubpi->phy_rev) && ACMINORREV_1(pi))  {
		/* Ucode better be suspended when we mess with BTCX regs directly */
		ASSERT(!(R_REG(pi->sh->osh, &pi->regs->maccontrol) & MCTL_EN_MAC));
		/* Enable manual BTCX mode */
		OR_REG(pi->sh->osh, &pi->regs->PHYREF_BTCX_CTRL, BTCX_CTRL_EN | BTCX_CTRL_SW);
		/* Force WLAN antenna and priority */
		OR_REG(pi->sh->osh, &pi->regs->PHYREF_BTCX_TRANS_CTRL,
			BTCX_TRANS_TXCONF | BTCX_TRANS_ANTSEL);
	}
}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */

void
wlc_phy_radio2069_pwrdwn_seq(phy_ac_radio_info_t *ri)
{
	phy_info_t *pi = ri->pi;

	/* AFE */
	ri->afeRfctrlCoreAfeCfg10 = READ_PHYREG(pi, RfctrlCoreAfeCfg10);
	ri->afeRfctrlCoreAfeCfg20 = READ_PHYREG(pi, RfctrlCoreAfeCfg20);
	ri->afeRfctrlOverrideAfeCfg0 = READ_PHYREG(pi, RfctrlOverrideAfeCfg0);
	ACPHY_REG_LIST_START
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreAfeCfg10, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreAfeCfg20, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlOverrideAfeCfg0, 0x1fff)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* Radio RX */
	ri->rxRfctrlCoreRxPus0 = READ_PHYREG(pi, RfctrlCoreRxPus0);
	ri->rxRfctrlOverrideRxPus0 = READ_PHYREG(pi, RfctrlOverrideRxPus0);
	WRITE_PHYREG(pi, RfctrlCoreRxPus0, 0x40);
	WRITE_PHYREG(pi, RfctrlOverrideRxPus0, 0xffbf);

	/* Radio TX */
	ri->txRfctrlCoreTxPus0 = READ_PHYREG(pi, RfctrlCoreTxPus0);
	ri->txRfctrlOverrideTxPus0 = READ_PHYREG(pi, RfctrlOverrideTxPus0);
	WRITE_PHYREG(pi, RfctrlCoreTxPus0, 0);
	WRITE_PHYREG(pi, RfctrlOverrideTxPus0, 0x3ff);

	/* {radio, rfpll, pllldo}_pu = 0 */
	ri->radioRfctrlCmd = READ_PHYREG(pi, RfctrlCmd);
	ri->radioRfctrlCoreGlobalPus = READ_PHYREG(pi, RfctrlCoreGlobalPus);
	ri->radioRfctrlOverrideGlobalPus = READ_PHYREG(pi, RfctrlOverrideGlobalPus);
	ACPHY_REG_LIST_START
		MOD_PHYREG_ENTRY(pi, RfctrlCmd, chip_pu, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlCoreGlobalPus, 0)
		WRITE_PHYREG_ENTRY(pi, RfctrlOverrideGlobalPus, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);
}

void
wlc_phy_radio2069_pwrup_seq(phy_ac_radio_info_t *ri)
{
	phy_info_t *pi = ri->pi;

	/* AFE */
	WRITE_PHYREG(pi, RfctrlCoreAfeCfg10, ri->afeRfctrlCoreAfeCfg10);
	WRITE_PHYREG(pi, RfctrlCoreAfeCfg20, ri->afeRfctrlCoreAfeCfg20);
	WRITE_PHYREG(pi, RfctrlOverrideAfeCfg0, ri->afeRfctrlOverrideAfeCfg0);

	/* Restore Radio RX */
	WRITE_PHYREG(pi, RfctrlCoreRxPus0, ri->rxRfctrlCoreRxPus0);
	WRITE_PHYREG(pi, RfctrlOverrideRxPus0, ri->rxRfctrlOverrideRxPus0);

	/* Radio TX */
	WRITE_PHYREG(pi, RfctrlCoreTxPus0, ri->txRfctrlCoreTxPus0);
	WRITE_PHYREG(pi, RfctrlOverrideTxPus0, ri->txRfctrlOverrideTxPus0);

	/* {radio, rfpll, pllldo}_pu = 0 */
	WRITE_PHYREG(pi, RfctrlCmd, ri->radioRfctrlCmd);
	WRITE_PHYREG(pi, RfctrlCoreGlobalPus, ri->radioRfctrlCoreGlobalPus);
	WRITE_PHYREG(pi, RfctrlOverrideGlobalPus, ri->radioRfctrlOverrideGlobalPus);
}

void
wlc_acphy_get_radio_loft(phy_info_t *pi,
	uint8 *ei0,
	uint8 *eq0,
	uint8 *fi0,
	uint8 *fq0)
{
	/* Not required for 4345 */
	*ei0 = 0;
	*eq0 = 0;
	*fi0 = 0;
	*fq0 = 0;
}

void
wlc_acphy_set_radio_loft(phy_info_t *pi,
	uint8 ei0,
	uint8 eq0,
	uint8 fi0,
	uint8 fq0)
{
	/* Not required for 4345 */
	return;
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
void
wlc_phy_radio20693_config_bf_mode(phy_info_t *pi, uint8 core)
{
	uint8 pupd, band_sel;
	uint8 afeBFpu0 = 0, afeBFpu1 = 0, afeclk_div12g_pu = 1;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (wlapi_txbf_enab(pi->sh->physhim)) {
		if (phy_get_phymode(pi) == PHYMODE_MIMO) {
			if (core == 1) {
				afeBFpu0 = 0;
				afeBFpu1 = 1;
				afeclk_div12g_pu = 0;
			} else {
				afeBFpu0 = 1;
				afeBFpu1 = 0;
				afeclk_div12g_pu = 1;
			}
		}
		pupd = 1;
	} else {
		pupd = 0;
	}

	MOD_RADIO_REG_20693(pi, SPARE_CFG8, core, afe_BF_pu0, afeBFpu0);
	MOD_RADIO_REG_20693(pi, SPARE_CFG8, core, afe_BF_pu1, afeBFpu1);
	MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core, afe_clk_div_12g_pu, afeclk_div12g_pu);
	MOD_RADIO_REG_20693(pi, CLK_DIV_OVR1, core, ovr_afeclkdiv_12g_pu, 0x1);

	band_sel = 0;

	if (CHSPEC_IS2G(pi->radio_chanspec))
		band_sel = 1;

	/* Set the 2G divider PUs */

	/* MOD_RADIO_REG_20693(pi, RXMIX2G_CFG1, core, rxmix2g_pu, 1); */
	MOD_RADIO_REG_20693(pi, TX_LOGEN2G_CFG1, core, logen2g_tx_pu_bias, (pupd & band_sel));
	/* Set the corresponding 2G Ovrs */
	/* MOD_RADIO_REG_20693(pi, RX_TOP_2G_OVR_EAST2, core, ovr_rxmix2g_pu, 1); */
	MOD_RADIO_REG_20693(pi, TX_TOP_2G_OVR_EAST, core, ovr_logen2g_tx_pu_bias,
		(pupd & band_sel));

	/* Set the 5G divider PUs */
	/* MOD_RADIO_REG_20693(pi, LNA5G_CFG3, core, mix5g_en, (pupd & (1-band_sel))); */
	MOD_RADIO_REG_20693(pi, TX_LOGEN5G_CFG1, core, logen5g_tx_enable_5g_low_band,
		(pupd & (1-band_sel)));
	/* Set the corresponding 5G Ovrs */
	/* MOD_RADIO_REG_20693(pi, RX_TOP_5G_OVR, core, ovr_mix5g_en, (pupd & (1-band_sel))); */
	MOD_RADIO_REG_20693(pi, TX_TOP_5G_OVR2, core, ovr_logen5g_tx_enable_5g_low_band,
		(pupd & (1-band_sel)));
}

static void
wlc_phy_radio20693_mimo_cm1_lpopt(phy_info_t *pi)
{
	uint16 phymode = phy_get_phymode(pi), temp;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	acphy_pmu_core1_off_radregs_t *porig = (pi_ac->radioi->pmu_c1_off_info_orig);

	BCM_REFERENCE(phymode);
	ASSERT(phymode == PHYMODE_MIMO);

	/* Turn off core 1 PMU + other lp optimizations */
	ASSERT(!porig->is_orig);
	porig->is_orig = TRUE;
	PHY_TRACE(("%s: Applying core 0 optimizations\n", __FUNCTION__));

	wlc_phy_radio20693_mimo_cm1_lpopt_saveregs(pi);

	ACPHY_REG_LIST_START
		/* Core 0 Settings */
		MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_lowquiescenten,
			0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 0, ldo_1p2_xtalldo1p2_pull_down_sw, 0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, PLL_CP1, 0, rfpll_cp_bias_low_power, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, VREG_CFG, 0, vreg25_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, VREG_OVR1, 0, ovr_vreg25_pu, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* MEM opt. Setting wlpmu_en, wlpmu_VCOldo_pu, wlpmu_TXldo_pu,
	   wlpmu_AFEldo_pu, wlpmu_RXldo_pu
	 */
	temp = READ_RADIO_REG_TINY(pi, PMU_OP, 0);
	phy_utils_write_radioreg(pi, RADIO_REG_20693(pi, PMU_OP, 0), (temp | 0x009E));

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_20693_ENTRY(pi, PMU_CFG4, 0, wlpmu_ADCldo_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_CFG2, 0, logencore_det_stg1_pu, 0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR1, 0, ovr_logencore_det_stg1_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_CFG3, 0, logencore_2g_mimodes_pu, 0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR2, 0, ovr_logencore_2g_mimodes_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_CFG3, 0, logencore_2g_mimosrc_pu, 0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR2, 0, ovr_logencore_2g_mimosrc_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_CFG1, 0, afe_clk_div_6g_mimo_pu, 0x0)
		MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_OVR1, 0, ovr_afeclkdiv_6g_mimo_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_CFG1, 0, afe_clk_div_12g_pu, 0x1)
		MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_OVR1, 0, ovr_afeclkdiv_12g_pu, 0x1)
	ACPHY_REG_LIST_EXECUTE(pi);

	MOD_RADIO_REG_20693(pi, SPARE_CFG2, 0, afe_clk_div_se_drive, 0x0);
	/* Power down core0 MIMO LO buffer */
	MOD_RADIO_REG_20693(pi, LOGEN_OVR2, 0, ovr_logencore_5g_mimosrc_pu, 0x1);
	MOD_RADIO_REG_20693(pi, LOGEN_CFG3, 0, logencore_5g_mimosrc_pu, 0x0);

	if (phy_ac_chanmgr_get_data(pi_ac->chanmgri)->both_txchain_rxchain_eq_1 == TRUE) {
		PHY_TRACE(("%s: Applying core 1 optimizations\n", __FUNCTION__));
		ACPHY_REG_LIST_START
			/* Core 1 Settings */
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 1,
				ldo_1p2_xtalldo1p2_lowquiescenten, 0x0)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 1,
				ldo_1p2_xtalldo1p2_pull_down_sw, 0x0)
			MOD_RADIO_REG_20693_ENTRY(pi, AUXPGA_OVR1, 1, ovr_auxpga_i_pu, 0x1)
			MOD_RADIO_REG_20693_ENTRY(pi, VREG_CFG, 1, vreg25_pu, 0x0)
			MOD_RADIO_REG_20693_ENTRY(pi, VREG_OVR1, 1, ovr_vreg25_pu, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);

		/* MEM opt. UN-Setting wlpmu_en, wlpmu_VCOldo_pu, wlpmu_TXldo_pu,
		   wlpmu_AFEldo_pu, wlpmu_RXldo_pu
		 */
		temp = READ_RADIO_REG_TINY(pi, PMU_OP, 1);
		phy_utils_write_radioreg(pi, RADIO_REG_20693(pi, PMU_OP, 1), (temp & ~0x009E));

		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20693_ENTRY(pi, PMU_CFG4, 1, wlpmu_ADCldo_pu, 0x0)
			MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR1, 1, ovr_logencore_det_stg1_pu, 0x1)
			MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR2, 1,
				ovr_logencore_2g_mimodes_pu, 0x1)
			MOD_RADIO_REG_20693_ENTRY(pi, LOGEN_OVR2, 1,
				ovr_logencore_2g_mimosrc_pu, 0x1)
			MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_OVR1, 1,
				ovr_afeclkdiv_6g_mimo_pu, 0x1)
			MOD_RADIO_REG_20693_ENTRY(pi, CLK_DIV_OVR1, 1, ovr_afeclkdiv_12g_pu, 0x1)
			/* Power down core1 XTAL LDO */
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTALLDO1, 1,
				ldo_1p2_xtalldo1p2_BG_pu, 0x0)
			MOD_RADIO_REG_20693_ENTRY(pi, PLL_XTAL_OVR1, 1,
				ovr_ldo_1p2_xtalldo1p2_BG_pu, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
static void wlc_phy_radio20693_mimo_cm1_lpopt_saveregs(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	acphy_pmu_core1_off_radregs_t *porig = (pi_ac->radioi->pmu_c1_off_info_orig);
	uint8 core;
	uint16 *ptr_start = (uint16 *)&porig->pll_xtalldo1[0];

	ASSERT(ISALIGNED(ptr_start, sizeof(uint16)));

	FOREACH_CORE(pi, core) {
		uint8 ct;
		uint16 *ptr;
		uint16 porig_offsets[] = {
			RADIO_REG_20693(pi, PLL_XTALLDO1, core),
			RADIO_REG_20693(pi, PLL_XTAL_OVR1, core),
			RADIO_REG_20693(pi, PLL_CP1, core),
			RADIO_REG_20693(pi, VREG_CFG, core),
			RADIO_REG_20693(pi, VREG_OVR1, core),
			RADIO_REG_20693(pi, PMU_OP, core),
			RADIO_REG_20693(pi, PMU_CFG4, core),
			RADIO_REG_20693(pi, LOGEN_CFG2, core),
			RADIO_REG_20693(pi, LOGEN_OVR1, core),
			RADIO_REG_20693(pi, LOGEN_CFG3, core),
			RADIO_REG_20693(pi, LOGEN_OVR2, core),
			RADIO_REG_20693(pi, CLK_DIV_CFG1, core),
			RADIO_REG_20693(pi, CLK_DIV_OVR1, core),
			RADIO_REG_20693(pi, SPARE_CFG2, core),
			RADIO_REG_20693(pi, AUXPGA_OVR1, core),
			RADIO_REG_20693(pi, TX_TOP_2G_OVR_EAST, core)
		};

		/* Here ptr is pointing to a PHY_CORE_MAX length array of uint16s. However,
		   the data access logis goec beyond this length to access next set of elements,
		   assuming contiguous memory allocation. So, ARRAY OVERRUN is intentional here
		 */
		for (ct = 0; ct < ARRAYSIZE(porig_offsets); ct++) {
			ptr = ptr_start + ((ct * PHY_CORE_MAX) + core);
			ASSERT(ptr <= &porig->tx_top_2g_ovr_east[PHY_CORE_MAX-1]);
			*ptr = _READ_RADIO_REG(pi, porig_offsets[ct]);
		}
	}
}
void wlc_phy_radio20693_mimo_lpopt_restore(phy_info_t *pi)
{
	uint16 phymode = phy_get_phymode(pi);
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	acphy_pmu_core1_off_radregs_t *porig = (pi_ac->radioi->pmu_c1_off_info_orig);
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = (pi_ac->radioi->pmu_lp_opt_orig);

	BCM_REFERENCE(phymode);
	ASSERT(phymode == PHYMODE_MIMO);

	ASSERT(!((pmu_lp_opt_orig->is_orig == TRUE) && (porig->is_orig == TRUE)));


	if (porig->is_orig == TRUE) {
		wlc_phy_radio20693_mimo_cm1_lpopt_restore(pi);
	} else if (pmu_lp_opt_orig->is_orig == TRUE) {
		wlc_phy_radio20693_mimo_cm23_lp_opt_restore(pi);
	}
}
static void wlc_phy_radio20693_mimo_cm23_lp_opt_saveregs(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint8 core;
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = (pi_ac->radioi->pmu_lp_opt_orig);
	uint16 *ptr_start = (uint16 *)&pmu_lp_opt_orig->pll_xtalldo1[0];

	ASSERT(ISALIGNED(ptr_start, sizeof(uint16)));

	FOREACH_CORE(pi, core) {
		uint8 ct;
		uint16 *ptr;
		uint16 porig_offsets[] = {
			RADIO_REG_20693(pi, PLL_XTALLDO1, core),
			RADIO_REG_20693(pi, PLL_XTAL_OVR1, core),
			RADIO_REG_20693(pi, PMU_OP, core),
			RADIO_REG_20693(pi, PMU_OVR, core),
			RADIO_REG_20693(pi, PMU_CFG4, core),
		};

		/* Here ptr is pointing to a PHY_CORE_MAX length array of uint16s. However,
		   the data access logis goec beyond this length to access next set of elements,
		   assuming contiguous memory allocation. So, ARRAY OVERRUN is intentional here
		 */
		for (ct = 0; ct < ARRAYSIZE(porig_offsets); ct++) {
			ptr = ptr_start + ((ct * PHY_CORE_MAX) + core);
			ASSERT(ptr <= &pmu_lp_opt_orig->pmu_cfg4[PHY_CORE_MAX-1]);
			*ptr = _READ_RADIO_REG(pi, porig_offsets[ct]);
		}
	}

}

static void
wlc_phy_radio20693_mimo_cm23_lp_opt(phy_info_t *pi, uint8 coremask)
{
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint8 core;
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = (pi_ac->radioi->pmu_lp_opt_orig);

	ASSERT(phy_get_phymode(pi) == PHYMODE_MIMO);
	ASSERT(coremask != 1);
	ASSERT(!pmu_lp_opt_orig->is_orig);
	pmu_lp_opt_orig->is_orig = TRUE;

	PHY_TRACE(("%s: Applying CM2/CM3 radio LP optimizations for coremask : %d\n",
		__FUNCTION__, coremask));

	wlc_phy_radio20693_mimo_cm23_lp_opt_saveregs(pi);

	/* Power down core1 XTAL LDO */
	MOD_RADIO_REG_20693(pi, PLL_XTALLDO1, 1, ldo_1p2_xtalldo1p2_BG_pu, 0x0);
	MOD_RADIO_REG_20693(pi, PLL_XTAL_OVR1, 1, ovr_ldo_1p2_xtalldo1p2_BG_pu, 0x1);

	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20693(pi, PLL_XTALLDO1, core, ldo_1p2_xtalldo1p2_pull_down_sw, 0x0);
	}
}

static void
wlc_phy_radio20693_mimo_cm23_lp_opt_restore(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint8 core;
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = (pi_ac->radioi->pmu_lp_opt_orig);
	uint16 *ptr_start = (uint16 *)&pmu_lp_opt_orig->pll_xtalldo1[0];


	ASSERT(phy_get_phymode(pi) == PHYMODE_MIMO);
	ASSERT(ISALIGNED(ptr_start, sizeof(uint16)));

	if (pmu_lp_opt_orig->is_orig == TRUE) {
		pmu_lp_opt_orig->is_orig = FALSE;
		PHY_TRACE(("%s: Restoring the radio lp settings\n", __FUNCTION__));

		FOREACH_CORE(pi, core) {
			uint8 ct;
			uint16 *ptr;
			uint16 porig_offsets[] = {
				RADIO_REG_20693(pi, PLL_XTALLDO1, core),
				RADIO_REG_20693(pi, PLL_XTAL_OVR1, core),
				RADIO_REG_20693(pi, PMU_OP, core),
				RADIO_REG_20693(pi, PMU_OVR, core),
				RADIO_REG_20693(pi, PMU_CFG4, core),
			};
			/* Here ptr is pointing to a PHY_CORE_MAX length array of uint16s. However,
			   the data access logic goes beyond this length to access next set of
			   elements, assuming contiguous memory allocation.
			   So, ARRAY OVERRUN is intentional here
			 */
			for (ct = 0; ct < ARRAYSIZE(porig_offsets); ct++) {
				ptr = ptr_start + ((ct * PHY_CORE_MAX) + core);
				ASSERT(ptr <= &pmu_lp_opt_orig->pmu_cfg4[PHY_CORE_MAX-1]);
				phy_utils_write_radioreg(pi, porig_offsets[ct], *ptr);
			}
		}
		wlc_phy_radio20693_minipmu_pwron_seq(pi);
	}
}

static void
wlc_phy_radio20693_mimo_cm1_lpopt_restore(phy_info_t *pi)
{
	uint16 phymode = phy_get_phymode(pi);
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	acphy_pmu_core1_off_radregs_t *porig = (pi_ac->radioi->pmu_c1_off_info_orig);
	uint16 *ptr_start = (uint16 *)&porig->pll_xtalldo1[0];
	uint8 core;

	BCM_REFERENCE(phymode);
	ASSERT(phymode == PHYMODE_MIMO);
	ASSERT(ISALIGNED(ptr_start, sizeof(uint16)));

	ASSERT(porig->is_orig);
	porig->is_orig = FALSE;
	PHY_TRACE(("%s: Restoring the core0/1 optimizations to their def values\n", __FUNCTION__));

	FOREACH_CORE(pi, core) {
		uint8 ct;
		uint16 *ptr;
		uint16 porig_offsets[] = {
			RADIO_REG_20693(pi, PLL_XTALLDO1, core),
			RADIO_REG_20693(pi, PLL_XTAL_OVR1, core),
			RADIO_REG_20693(pi, PLL_CP1, core),
			RADIO_REG_20693(pi, VREG_CFG, core),
			RADIO_REG_20693(pi, VREG_OVR1, core),
			RADIO_REG_20693(pi, PMU_OP, core),
			RADIO_REG_20693(pi, PMU_CFG4, core),
			RADIO_REG_20693(pi, LOGEN_CFG2, core),
			RADIO_REG_20693(pi, LOGEN_OVR1, core),
			RADIO_REG_20693(pi, LOGEN_CFG3, core),
			RADIO_REG_20693(pi, LOGEN_OVR2, core),
			RADIO_REG_20693(pi, CLK_DIV_CFG1, core),
			RADIO_REG_20693(pi, CLK_DIV_OVR1, core),
			RADIO_REG_20693(pi, SPARE_CFG2, core),
			RADIO_REG_20693(pi, AUXPGA_OVR1, core),
			RADIO_REG_20693(pi, TX_TOP_2G_OVR_EAST, core)
		};

		/* Here ptr is pointing to a PHY_CORE_MAX length array of uint16s. However,
		   the data access logis goec beyond this length to access next set of elements,
		   assuming contiguous memory allocation. So, ARRAY OVERRUN is intentional here
		 */
		for (ct = 0; ct < ARRAYSIZE(porig_offsets); ct++) {
			ptr = ptr_start + ((ct * PHY_CORE_MAX) + core);
			ASSERT(ptr <= &porig->tx_top_2g_ovr_east[PHY_CORE_MAX-1]);
			phy_utils_write_radioreg(pi, porig_offsets[ct], *ptr);
		}
	}
	wlc_phy_radio20693_minipmu_pwron_seq(pi);
	wlc_phy_tiny_radio_minipmu_cal(pi);
	wlc_phy_radio20693_pmu_pll_config(pi);
	wlc_phy_radio20693_afecal(pi);
}

void wlc_phy_radio20693_mimo_core1_pmu_restore_on_bandhcange(phy_info_t *pi)
{
	uint16 phymode = phy_get_phymode(pi);
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	acphy_pmu_core1_off_radregs_t *porig = (pi_ac->radioi->pmu_c1_off_info_orig);
	acphy_pmu_mimo_lp_opt_radregs_t *pmu_lp_opt_orig = (pi_ac->radioi->pmu_lp_opt_orig);
	bool for_2g = ((pi_ac->radioi->lpmode_2g != ACPHY_LPMODE_NONE) &&
		(CHSPEC_IS2G(pi->radio_chanspec)));
	bool for_5g = ((pi_ac->radioi->lpmode_5g != ACPHY_LPMODE_NONE) &&
		(CHSPEC_IS5G(pi->radio_chanspec)));

	BCM_REFERENCE(phymode);
	ASSERT(phymode == PHYMODE_MIMO);

	if (((for_2g == TRUE) || (for_5g == TRUE))) {
		if (porig->is_orig == TRUE) {
			wlc_phy_radio20693_mimo_cm1_lpopt_saveregs(pi);
		} else if (pmu_lp_opt_orig->is_orig == TRUE) {
			wlc_phy_radio20693_mimo_cm23_lp_opt_saveregs(pi);
		}
	}
}

static void wlc_phy_radio20693_tx2g_set_freq_tuning_ipa_as_epa(phy_info_t *pi,
	uint8 core, uint8 chan)
{
	uint8 mix2g_tune_tbl[] = { 0x4, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 0x2, 0x2,
		0x2, 0x2, 0x1};
	uint8 mix2g_tune_val;
	ASSERT((chan >= 1) && (chan <= 14));
	mix2g_tune_val = mix2g_tune_tbl[chan - 1];
	MOD_RADIO_REG_20693(pi, TXMIX2G_CFG5, core,
		mx2g_tune, mix2g_tune_val);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

void
phy_ac_dsi_radio_fns(phy_info_t *pi)
{
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
	wlc_phy_radio2069_mini_pwron_seq_rev16(pi);
	wlc_phy_radio2069_pwron_seq(pi);
	wlc_phy_radio2069_vcocal(pi);
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
	wlc_phy_radio2069x_vcocal_isdone(pi, TRUE, FALSE);
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
chanspec_setup_radio_20693(phy_info_t *pi)
{
	uint8 core = 0;
	const chan_info_radio20693_pll_t *chan_info_pll;
	const chan_info_radio20693_rffe_t *chan_info_rffe;
	const chan_info_radio20693_pll_wave2_t *chan_info_pll_wave2;
	uint8 ch[NUM_CHANS_IN_CHAN_BONDING], n_freq_seg;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	chanspec_get_operating_channels(pi, ch);
	pi_ac->radioi->data->fc = wlc_phy_chan2freq_20693(pi, ch[0], &chan_info_pll,
		&chan_info_rffe, &chan_info_pll_wave2);

	if (pi_ac->radioi->data->fc >= 0) {
		if (RADIOMAJORREV(pi) == 3) {
			if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
				PHY_INFORM(("wl%d: %s: setting chan1=%d, chan2=%d...\n",
						pi->sh->unit, __FUNCTION__, ch[0], ch[1]));
				if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac)) {
					phy_ac_radio_20693_pmu_pll_config_wave2(pi, 7);
				}
				/* setting both pll cores - mode = 1 */
				phy_ac_radio_20693_chanspec_setup(pi, ch[0],
					CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac), 0, 1);
			} else if (CHSPEC_IS160(pi->radio_chanspec)) {
				ASSERT(0);
			} else {
				phy_ac_radio_20693_chanspec_setup(pi, ch[0],
					CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac), 0, 0);
			}
			if (CCT_INIT(pi_ac) || CCT_BW_CHG_80P80(pi_ac)) {
				if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
					wlc_phy_radio20693_afe_clkdistribtion_mode(pi, 1);
				} else {
					wlc_phy_radio20693_afe_clkdistribtion_mode(pi, 0);
				}
			}
		} else {
			if (phy_get_phymode(pi) == PHYMODE_80P80) {
				for (n_freq_seg = 0; n_freq_seg < NUM_CHANS_IN_CHAN_BONDING;
						n_freq_seg++) {
					phy_ac_radio_20693_chanspec_setup(pi, ch[n_freq_seg],
						CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac),
						n_freq_seg, 0);
				}
			} else {
				FOREACH_CORE(pi, core) {
					phy_ac_radio_20693_chanspec_setup(pi, ch[0],
						CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac), core, 0);
				}
			}
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
chanspec_setup_radio_20691(phy_info_t *pi)
{
	const chan_info_radio20691_t *chan_info_20691;
	uint8 ch[NUM_CHANS_IN_CHAN_BONDING];

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	chanspec_get_operating_channels(pi, ch);
	pi_ac->radioi->data->fc = wlc_phy_chan2freq_20691(pi, ch[0], &chan_info_20691);

	if (pi_ac->radioi->data->fc >= 0) {
		wlc_phy_chanspec_radio20691_setup(pi, ch[0],
			CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac));
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
chanspec_setup_radio_2069(phy_info_t *pi)
{
	uint8 ch[NUM_CHANS_IN_CHAN_BONDING];
	const void *chan_info = NULL;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	/* 4335
	 * low power mode selection based on 2G or 5G
	 */
	if (ACMAJORREV_1(pi->pubpi->phy_rev)) {
		if (CHSPEC_IS2G(pi->radio_chanspec))
			pi_ac->radioi->acphy_lp_mode = 2;
		else
			pi_ac->radioi->acphy_lp_mode = 1;

		pi_ac->radioi->acphy_prev_lp_mode = pi_ac->radioi->acphy_lp_mode;
		pi_ac->radioi->data->acphy_lp_status = pi_ac->radioi->acphy_lp_mode;
	}

	chanspec_get_operating_channels(pi, ch);
	pi_ac->radioi->data->fc = wlc_phy_chan2freq_acphy(pi, ch[0], &chan_info);

	if (pi_ac->radioi->data->fc >= 0) {
		wlc_phy_chanspec_radio2069_setup(pi, chan_info,
			CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac));
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
chanspec_tune_radio_20693(phy_info_t *pi)
{
	uint16 dac_rate;
	uint8 lpf_gain = 1, lpf_bw, biq_bw_ofdm, biq_bw_cck, core;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		pi->u.pi_acphy->radioi->data->dac_mode = 1;
	} else {
		pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
			pi->dacratemode2g : pi->dacratemode5g;
		wlc_phy_dac_rate_mode_acphy(pi, pi_ac->radioi->data->dac_mode);
	}

	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		if (!ISSIM_ENAB(pi->sh->sih)) {
			if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac)) {
				/* bw_change requires afe cal. */
				/* so that all the afe_iqadc signals are correctly set */
				wlc_phy_radio_afecal(pi);
			}
		}
		if (CHSPEC_IS2G(pi->radio_chanspec)) {
			if (CHSPEC_IS20(pi->radio_chanspec)) {
				lpf_bw = 1;
				lpf_gain = -1;
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				lpf_bw = 3;
				lpf_gain = -1;
			} else {
				lpf_bw = 3;
				lpf_gain = -1;
			}
		} else {
			if (CHSPEC_IS20(pi->radio_chanspec)) {
				lpf_bw = 3;
				lpf_gain = -1;
			} else if (CHSPEC_IS40(pi->radio_chanspec)) {
				lpf_bw = 3;
				lpf_gain = -1;
			} else {
				lpf_bw = 3;
				lpf_gain = -1;
			}
		}
	} else {
		dac_rate = wlc_phy_get_dac_rate_from_mode(pi, pi_ac->radioi->data->dac_mode);

		/* For 4349 do afecal on chan change to take care of RSDB dac spurs */
		wlc_phy_radio_afecal(pi);

		/* tune lpf setting for 20693 based on dac rate */
		if (!(PHY_IPA(pi))) {
			if (dac_rate == 200) {
				lpf_bw = 1;
				if (CHSPEC_IS2G(pi->radio_chanspec)) {
					lpf_bw = 1;
					lpf_gain = 1;
				} else {
					lpf_bw = 2;
					lpf_gain = 0;
				}
			} else if (dac_rate == 400) {
				lpf_bw = 3;
				lpf_gain = 0;
			} else {
				lpf_bw = 5;
				lpf_gain = 0;
			}
		} else {
			if (dac_rate == 200) {
				lpf_bw = 2;
				if (CHSPEC_IS2G(pi->radio_chanspec)) {
					lpf_gain = 2;
				} else {
					lpf_gain = 0;
				}
			} else if (dac_rate == 400) {
				lpf_bw = 3;
				lpf_gain = 0;
			} else {
				lpf_bw = 5;
				lpf_gain = 0;
			}
		}
	}

	if (!(PHY_IPA(pi)) && (RADIOREV(pi->pubpi->radiorev) == 13)) {
		lpf_gain = 0;
	} else {
		if (!(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) &&
		RADIOMAJORREV(pi) == 3)) {
			lpf_gain = CHSPEC_IS2G(pi->radio_chanspec) ? 2 : 0;
		}
	}

	biq_bw_ofdm = lpf_bw;
	biq_bw_cck = lpf_bw - 1;

	/* WAR for FDIQI when bq_bw = 9, 25 MHz */
	if (!PHY_PAPDEN(pi) && !PHY_IPA(pi) && CHSPEC_IS2G(pi->radio_chanspec) &&
		CHSPEC_IS20(pi->radio_chanspec) && !(RADIOREV(pi->pubpi->radiorev) == 13)) {
		biq_bw_ofdm = lpf_bw - 1;
	}

	wlc_phy_radio_tiny_lpf_tx_set(pi, lpf_bw, lpf_gain, biq_bw_ofdm, biq_bw_cck);

	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		wlc_phy_radio20693_force_dacbuf_setting(pi);
	} else {
		/* dac swap */
		FOREACH_CORE(pi, core) {
			if (core == 0)
				MOD_PHYREG(pi, Core1TxControl, iqSwapEnable, 1);
			else if (core == 1)
				MOD_PHYREG(pi, Core2TxControl, iqSwapEnable, 1);
		}

		/* adc swap */
		MOD_PHYREG(pi, RxFeCtrl1, swap_iq0, 1);
		MOD_PHYREG(pi, RxFeCtrl1, swap_iq1, 1);
	}

	OSL_DELAY(20);

	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && RADIOMAJORREV(pi) == 3) {
		if (PHY_AS_80P80(pi, pi->radio_chanspec)) {
			uint8 bands[NUM_CHANS_IN_CHAN_BONDING];
			phy_ac_chanmgr_get_chan_freq_range_80p80(pi, 0, bands);
			pi_ac->radioi->prev_subband = bands[0];
			PHY_INFORM(("wl%d: %s: FIXME for 80P80\n", pi->sh->unit, __FUNCTION__));
		} else {
			pi_ac->radioi->prev_subband =
				phy_ac_chanmgr_get_chan_freq_range(pi, 0, PRIMARY_FREQ_SEGMENT);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
chanspec_tune_radio_20691(phy_info_t *pi)
{
	uint8 lpf_gain, lpf_bw, biq_bw_ofdm, biq_bw_cck, core;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	/* The VCO Calibration clock driver cannot be off until after VCO Cal is done */
	MOD_RADIO_REG_20691(pi, PLL_XTAL2, 0, xtal_pu_caldrv, 0x0);

	pi_ac->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec) ?
		pi->dacratemode2g : pi->dacratemode5g;
	wlc_phy_dac_rate_mode_acphy(pi, pi_ac->radioi->data->dac_mode);

	/* bw_change requires afe cal. */
	if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac))
		wlc_phy_radio_afecal(pi);

	/* tune lpf settings for 20691 */
	if (CHSPEC_IS20(pi->radio_chanspec))
		if (IS_4364_1x1(pi)) {
			lpf_bw = 6;
		} else {
			lpf_bw = 2;
		}
	else if (CHSPEC_IS40(pi->radio_chanspec))
		lpf_bw = 3;
	else
		lpf_bw = 5;

	if (!(PHY_IPA(pi)) && (RADIOREV(pi->pubpi->radiorev) == 13))
		lpf_gain = 0;
	else
		lpf_gain = CHSPEC_IS2G(pi->radio_chanspec) ? 2 : 0;

	biq_bw_ofdm = lpf_bw;

	/* WAR for FDIQI when bq_bw = 9, 25 MHz */
	if (!PHY_PAPDEN(pi) && !PHY_IPA(pi) && CHSPEC_IS2G(pi->radio_chanspec) &&
		CHSPEC_IS20(pi->radio_chanspec) && !(RADIOREV(pi->pubpi->radiorev) == 13)) {
			biq_bw_ofdm = lpf_bw - 1;
	}

	if (IS_4364_1x1(pi)) {
		biq_bw_cck = 1;
	} else {
		biq_bw_cck = lpf_bw - 1;
	}

	wlc_phy_radio_tiny_lpf_tx_set(pi, lpf_bw, lpf_gain, biq_bw_ofdm, biq_bw_cck);

	/* dac swap */
	FOREACH_CORE(pi, core) {
		if (core == 0)
			MOD_PHYREG(pi, Core1TxControl, iqSwapEnable, 1);
		else if (core == 1)
			MOD_PHYREG(pi, Core2TxControl, iqSwapEnable, 1);
	}

	/* REV7 fixed adc swap */
	MOD_PHYREG(pi, RxFeCtrl1, swap_iq0, 1);

	OSL_DELAY(20);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
static void
chanspec_tune_radio_2069(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 tx_gain_tbl_id = wlc_phy_get_tbl_id_gainctrlbbmultluts(pi, 0);

	/* bw_change requires afe cal. */
	if (CCT_INIT(pi_ac) || CCT_BW_CHG(pi_ac))
		wlc_phy_radio_afecal(pi);

	/* Applicable for non-TINY radios */
	if (BF3_VLIN_EN_FROM_NVRAM(pi_ac)) {
		uint16 txidxval, txgaintemp1[3], txgaintemp1a[3];
		uint16 tempmask;
		uint16 vlinmask;
		if (pi_ac->radioi->prev_subband != 15) {
			for (txidxval = phy_ac_chanmgr_get_data(pi_ac->chanmgri)->vlin_txidx;
					txidxval < 128; txidxval++) {
				wlc_phy_table_read_acphy(pi, tx_gain_tbl_id, 1,
						txidxval, 48, &txgaintemp1);
				txgaintemp1a[0] = (txgaintemp1[0] & 0x7FFF) -
					(phy_ac_chanmgr_get_data(pi_ac->chanmgri)->bbmult_comp);
				txgaintemp1a[1] = txgaintemp1[1];
				txgaintemp1a[2] = txgaintemp1[2];
				wlc_phy_tx_gain_table_write_acphy(pi_ac->tbli, 1,
						txidxval, 48, txgaintemp1a);
			}
		}
		tempmask = READ_PHYREGFLD(pi, FemCtrl, femCtrlMask);
		if (CHSPEC_IS2G (pi->radio_chanspec)) {
			vlinmask =
			1<<(phy_ac_chanmgr_get_data(pi_ac->chanmgri)->vlinmask2g_from_nvram);
		}
		else {
			vlinmask =
			1<<(phy_ac_chanmgr_get_data(pi_ac->chanmgri)->vlinmask5g_from_nvram);
		}
		MOD_PHYREG(pi, FemCtrl, femCtrlMask, (tempmask|vlinmask));
		wlc_phy_vlin_en_acphy(pi);
	}
	pi_ac->radioi->prev_subband =
		phy_ac_chanmgr_get_chan_freq_range(pi, 0, PRIMARY_FREQ_SEGMENT);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
chanspec_setup_radio_20695(phy_info_t *pi)
{
	uint8 core = 0;
	const chan_info_radio20695_rffe_t *chan_info;
	uint8 ch_num;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	ch_num =  CHSPEC_CHANNEL(pi->radio_chanspec);

	/* Do resetcca to send out band_sel signal */
	wlc_phy_resetcca_acphy(pi);

	pi_ac->radioi->data->fc = wlc_phy_chan2freq_20695(pi, ch_num,
			&chan_info);

	if (pi_ac->radioi->data->fc >= 0) {
		FOREACH_CORE(pi, core) {
			/* If band has changed turn on required PLL and LOGEN */
			if (CCT_BAND_CHG(pi_ac)) {
				wlc_phy_radio20695_pll_logen_pu_config(pi, chan_info->freq, core);
			}

			wlc_phy_chanspec_radio20695_setup(pi, ch_num,
				CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac), core);
		}
	}
}


static void
chanspec_setup_xtal_20695(phy_info_t *pi)
{

	si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL9,
			PMUCCTL09_43012_XTAL_CORESIZE_BIAS_ADJ_NORMAL_MASK,
			(0x4 << PMUCCTL09_43012_XTAL_CORESIZE_BIAS_ADJ_NORMAL_SHIFT));
	si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL9,
			PMUCCTL09_43012_XTAL_CORESIZE_RES_BYPASS_NORMAL_MASK,
			(0x4 << PMUCCTL09_43012_XTAL_CORESIZE_RES_BYPASS_NORMAL_SHIFT));
	si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL8,
			PMUCCTL08_43012_XTAL_CORE_SIZE_NMOS_NORMAL_MASK,
			(0x10 << PMUCCTL08_43012_XTAL_CORE_SIZE_NMOS_NORMAL_SHIFT));
	si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL8,
			PMUCCTL08_43012_XTAL_CORE_SIZE_PMOS_NORMAL_MASK,
			(0x10 << PMUCCTL08_43012_XTAL_CORE_SIZE_PMOS_NORMAL_SHIFT));

	if (RADIOREV(pi->pubpi->radiorev) == 33) {
		si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL8,
		PMUCCTL08_43012_XTAL_SEL_BIAS_RES_NORMAL_MASK,
		(0x4 << PMUCCTL08_43012_XTAL_SEL_BIAS_RES_NORMAL_SHIFT));

	} else {
		si_pmu_chipcontrol(pi->sh->sih, PMU_CHIPCTL8,
		PMUCCTL08_43012_XTAL_SEL_BIAS_RES_NORMAL_MASK,
		(0x2 << PMUCCTL08_43012_XTAL_SEL_BIAS_RES_NORMAL_SHIFT));
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void
chanspec_setup_radio_20694(phy_info_t *pi)
{
	uint8 core = 0;
	const chan_info_radio20694_rffe_t *chan_info;
	uint8 ch_num;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	ch_num =  CHSPEC_CHANNEL(pi->radio_chanspec);

	/* Do resetcca to send out band_sel signal */
	wlc_phy_resetcca_acphy(pi);

	pi_ac->radioi->data->fc = wlc_phy_chan2freq_20694(pi, ch_num,
			&chan_info);

	if (pi_ac->radioi->data->fc >= 0) {
		FOREACH_CORE(pi, core) {
			wlc_phy_chanspec_radio20694_setup(pi, ch_num,
				CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac), core);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
chanspec_setup_radio_20696(phy_info_t *pi)
{
	uint8 ch_num;

	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	ch_num =  CHSPEC_CHANNEL(pi->radio_chanspec);

	/* Do resetcca to send out band_sel signal */
	wlc_phy_resetcca_acphy(pi);

	wlc_phy_chanspec_radio20696_setup(pi, ch_num,
		CCT_BAND_CHG(pi_ac) | CCT_BW_CHG(pi_ac));
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

void
chanspec_setup_radio(phy_info_t *pi)
{
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID))
		chanspec_setup_radio_20693(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID))
		chanspec_setup_radio_20691(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
	if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID))
		chanspec_setup_radio_2069(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID))
		chanspec_setup_radio_20694(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID))
		chanspec_setup_radio_20695(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID))
		chanspec_setup_radio_20696(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */
	{
		PHY_ERROR(("wl%d %s: Invalid RADIOID! %d\n",
			PI_INSTANCE(pi), __FUNCTION__, pi->pubpi->radioid));
		ASSERT(0);
	}
}

void
chanspec_tune_radio(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	if (pi_ac->radioi->data->fc >= 0) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			wlc_phy_28nm_radio_vcocal_isdone(pi, FALSE);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
		if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
			wlc_phy_radio20696_vcocal_isdone(pi, FALSE, TRUE);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40))
		if (ACMAJORREV_40(pi->pubpi->phy_rev)) {
			wlc_phy_radio20694_vcocal_isdone(pi, FALSE, TRUE);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_40)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37))
		if (ACMAJORREV_37(pi->pubpi->phy_rev)) {
			wlc_phy_radio20696_vcocal_isdone(pi, FALSE, TRUE);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_37)) */
		{
			wlc_phy_radio2069x_vcocal_isdone(pi, FALSE, FALSE);
		}
	}
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID))
		chanspec_tune_radio_20693(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID))
		chanspec_tune_radio_20691(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))
	if (RADIOID_IS(pi->pubpi->radioid, BCM2069_ID))
		chanspec_tune_radio_2069(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID))
		chanspec_tune_radio_20694(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID))
		chanspec_tune_radio_20695(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID))
		chanspec_tune_radio_20696(pi);
	else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */
	{
		PHY_ERROR(("wl%d %s: Invalid RADIOID! %d\n",
			PI_INSTANCE(pi), __FUNCTION__, pi->pubpi->radioid));
		ASSERT(0);
	}
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static int
BCMRAMFN(wlc_phy_ac_get_20694_chan_info_tbl)(phy_info_t *pi,
	const chan_info_radio20694_rffe_t **chan_info,
	uint32 *p_tbl_len)
{
	int ret_status = 0;
	uint sicoreunit;

	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	if (sicoreunit == DUALMAC_MAIN) {
		switch (RADIOREV(pi->pubpi->radiorev)) {
		case 8:
			*chan_info = chan_tune_20694_rev8_MAIN;
			*p_tbl_len = ARRAYSIZE(chan_tune_20694_rev8_MAIN);
			break;
		default:
			*chan_info = chan_tune_20694_rev5_fcbu_e_MAIN;
			*p_tbl_len = ARRAYSIZE(chan_tune_20694_rev5_fcbu_e_MAIN);
			break;
		}
	} else {
		switch (RADIOREV_AUX(pi->pubpi->radiorev)) {
		case 9:
			*chan_info = chan_tune_20694_rev9_AUX;
			*p_tbl_len = ARRAYSIZE(chan_tune_20694_rev9_AUX);
			break;
		default:
			*chan_info = chan_tune_20694_rev5_fcbu_e_AUX;
			*p_tbl_len = ARRAYSIZE(chan_tune_20694_rev5_fcbu_e_AUX);
			break;
		}
	}

	return ret_status;
}

static radio_20xx_prefregs_t *
BCMRAMFN(wlc_phy_ac_get_20694_prfd_vals_tbl)(phy_info_t *pi)
{

	uint sicoreunit;
	sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	if (RADIOREV(pi->pubpi->radiorev) == 36) {
		return prefregs_20694_rev36_wlbga;
	} else if (sicoreunit == DUALMAC_MAIN) {
		switch (RADIOREV(pi->pubpi->radiorev)) {
		case 8:
			return prefregs_20694_rev8_main;
		default:
			return prefregs_20694_rev5_main;
		}
	} else if (sicoreunit == DUALMAC_AUX) {
		switch (RADIOREV_AUX(pi->pubpi->radiorev)) {
		case 9:
			return prefregs_20694_rev9_aux;
		default:
			return prefregs_20694_rev5_aux;
		}
	} else {
		return NULL;
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
void
wlc_phy_radio20695_txdac_bw_setup(phy_info_t *pi, uint8 filter_type, uint8 dacbw)
{
	uint8  cfb_max  = 127;
	uint8  cin_max  = 255;
	uint8  clpf_max = 127;
	uint16 code_cfb = 17, code_cin = 17, code_clpf = 0;
	uint32 dacbuf_cmult = pi->u.pi_acphy->radioi->rccal_dacbuf_cmult;

	ASSERT(dacbw != 0);

	MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG0, 0, iqdac_buf_bw, 0x4);
	MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG0, 0, iqdac_buf_cc, 0x0);
	MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG1, 0, iqdac_buf_op_cur, 0x0);
	MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG1, 0, iqdac_lowpwr_en,  0x3);

	if (filter_type == 1) { /* butterworth */
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG4, 0, IQDACbuf_biqd_byp, 0x0);
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG4, 0, IQDACbuf_lpfen, 0x0);

		code_cfb = ((dacbuf_cmult * 2132) / dacbw) >> 12;
		code_cin = ((dacbuf_cmult * 2132) / dacbw) >> 12;
		code_cfb = (code_cfb > cfb_max) ? cfb_max : code_cfb;
		code_cin = (code_cin > cin_max) ? cin_max : code_cin;

		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG1, 0, iqdac_buf_Cfb, code_cfb);
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG3, 0, IQDACbuf_Cin,  code_cin);

	} else if (filter_type == 2) { /* "chebyshev"  */
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG4, 0, IQDACbuf_biqd_byp, 0x0);
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG4, 0, IQDACbuf_lpfen, 0x1);

		code_cfb = ((dacbuf_cmult * 1508) / dacbw) >> 12;
		code_cin = ((dacbuf_cmult * 3016) / dacbw) >> 12;
		code_clpf = ((dacbuf_cmult * 1508) / dacbw) >> 12;

		code_cfb  = (code_cfb > cfb_max) ? cfb_max : code_cfb;
		code_cin  = (code_cin > cin_max) ? cin_max : code_cin;
		code_clpf = (code_clpf > clpf_max) ? clpf_max : code_clpf;

		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG1, 0, iqdac_buf_Cfb, code_cfb);
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG3, 0, IQDACbuf_Cin,  code_cin);
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG3, 0, IQDACbuf_Clpf, code_clpf);

	} else if (filter_type == 3) {   /* biquad bypass */
		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG4, 0, IQDACbuf_biqd_byp, 0x1);

		code_cfb  = ((dacbuf_cmult * 3016) / dacbw) >> 12;
		code_cfb  = (code_cfb > cfb_max) ? cfb_max : code_cfb;

		MOD_RADIO_REG_28NM(pi, RF, TXDAC_REG1, 0, iqdac_buf_Cfb, code_cfb);

	}

}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

uint16
wlc_phy_get_dac_rate_from_mode(phy_info_t *pi, uint8 dac_rate_mode)
{
	uint16 dac_rate = 200;

	if (CHSPEC_BW_LE20(pi->radio_chanspec)) {
		switch (dac_rate_mode) {
			case 2:
				dac_rate = 600;
				break;
			case 3:
				dac_rate = 400;
				break;
			default: /* dac rate mode 1 */
				dac_rate = 200;
				break;
		}
	} else if (CHSPEC_IS40(pi->radio_chanspec)) {
		switch (dac_rate_mode) {
			case 2:
				dac_rate = 600;
				break;
			default: /* dac rate mode 1 and 3 */
				dac_rate = 400;
				break;
		}
	} else {
		dac_rate = 600;
	}

	return (dac_rate);
}

void wlc_phy_dac_rate_mode_acphy(phy_info_t *pi, uint8 dac_rate_mode)
{
	uint16 dac_rate;
	uint8 bw_idx = 0;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	bw_idx = (CHSPEC_IS80(pi->radio_chanspec) || PHY_AS_80P80(pi, pi->radio_chanspec))
				? 2 : (CHSPEC_IS160(pi->radio_chanspec)) ? 3
				: (CHSPEC_IS40(pi->radio_chanspec)) ? 1 : 0;
	dac_rate = wlc_phy_get_dac_rate_from_mode(pi, dac_rate_mode);

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		/* For 43012 dac_rate_mode is treated as ulp_tx_mode */
		uint16 ulp_tx_mode;
		ulp_tx_mode = pi_ac->radioi->data->ulp_tx_mode;

		/* In case of 43012. Rely on ulp_tx_mode instead of dac mode to change settings */
		switch (ulp_tx_mode) {
			case 2:
				/* 240 MHz mode */
				si_core_cflags(pi->sh->sih, 0x300, 0x200);
				break;

			case 3:
				/* 180 MHz mode */
				si_core_cflags(pi->sh->sih, 0x300, 0x200);
				break;

			case 4:
				/* 120 MHz mode */
				si_core_cflags(pi->sh->sih, 0x300, 0x000);
				break;

			case 5:
				/* 90 MHz mode */
				si_core_cflags(pi->sh->sih, 0x300, 0x000);
				break;

			case 1:
			default:
				/* Default mode is 360 MHz */
				si_core_cflags(pi->sh->sih, 0x300, 0x300);
				break;
		}
		MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr_value, 0);

	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
	{
		if (dac_rate_mode == 2) {
			si_core_cflags(pi->sh->sih, 0x300, 0x200);
		} else if (dac_rate_mode == 3) {
			si_core_cflags(pi->sh->sih, 0x300, 0x300);
		} else {
			si_core_cflags(pi->sh->sih, 0x300, 0x100);
		}

		switch (dac_rate) {
			case 200:
				MOD_RADIO_REG_20691(pi, CLK_DIV_CFG1, 0, sel_dac_div,
					(pi_ac->radioi->data->vcodivmode & 0x1) ? 3 : 4);
				break;
			case 400:
				MOD_RADIO_REG_20691(pi, CLK_DIV_CFG1, 0, sel_dac_div,
					(pi_ac->radioi->data->vcodivmode & 0x2) ? 1 : 2);
				break;
			case 600:
				MOD_RADIO_REG_20691(pi, CLK_DIV_CFG1, 0, sel_dac_div, 0);
				break;
			default:
				PHY_ERROR(("Unsupported dac_rate %d\n", dac_rate));
				ASSERT(0);
				break;
		}

		if ((dac_rate_mode == 1) || (bw_idx == 2)) {
			MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr, 0);
			MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr_value, 0);
		} else {
			MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr, 1);
			if (bw_idx == 1)
				MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr_value, 2);
			else
				MOD_PHYREG(pi, sdfeClkGatingCtrl, txlbclkmode_ovr_value, 1);
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
	if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
		wlc_phy_radio20695_afe_div_ratio(pi, pi_ac->radioi->data->ulp_tx_mode,
			pi_ac->radioi->data->ulp_adc_mode);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */

	if (TINY_RADIO(pi)) {
		wlc_phy_farrow_setup_tiny(pi, pi->radio_chanspec);
	} else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36))
		if (ACMAJORREV_36(pi->pubpi->phy_rev)) {
			wlc_phy_farrow_setup_28nm_ulp(pi, pi_ac->radioi->data->ulp_tx_mode,
				pi_ac->radioi->data->ulp_adc_mode);
		} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_36)) */
		{
			if (!ACMAJORREV_40(pi->pubpi->phy_rev)) {
				wlc_phy_farrow_setup_acphy(pi, pi->radio_chanspec);
			}
		}
	}
}

#ifdef PHY_DUMP_BINARY
/* The function is forced to RAM since it accesses non-const tables */
static int BCMRAMFN(phy_ac_radio_getlistandsize_20693)(phy_info_t *pi,
                    phyradregs_list_t **radreglist, uint16 *radreglist_sz)
{
	if (RADIO20693_MAJORREV(pi->pubpi->radiorev) == 2) {
		*radreglist = (phyradregs_list_t *) &rad20693_majorrev2_registers[0];
		*radreglist_sz = sizeof(rad20693_majorrev2_registers);
	} else {
		PHY_INFORM(("%s: wl%d: unsupported BCM20693 radio rev %d\n",
			__FUNCTION__,  pi->sh->unit,  pi->pubpi->radiorev));
		return BCME_UNSUPPORTED;
	}

	return BCME_OK;
}

static int BCMRAMFN(phy_ac_radio_getlistandsize_20695)(phy_info_t *pi,
                    phyradregs_list_t **radreglist, uint16 *radreglist_sz)
{
	*radreglist = (phyradregs_list_t *) &rad20695_majorrev1_registers[0];
	*radreglist_sz = sizeof(rad20695_majorrev1_registers);

	return BCME_OK;
}
static int phy_ac_radio_getlistandsize(phy_type_radio_ctx_t *ctx,
                    phyradregs_list_t **radreglist, uint16 *radreglist_sz)
{
	phy_ac_radio_info_t *info = (phy_ac_radio_info_t *)ctx;
	phy_info_t *pi = info->pi;
	int ret = BCME_UNSUPPORTED;
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
		ret = phy_ac_radio_getlistandsize_20693(pi, radreglist, radreglist_sz);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		ret = phy_ac_radio_getlistandsize_20695(pi, radreglist, radreglist_sz);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
	{
		PHY_INFORM(("%s: wl%d: unsupported radio ID %d\n",
			__FUNCTION__,  pi->sh->unit,  pi->pubpi->radioid));
	}
	return ret;
}
#endif /* PHY_DUMP_BINARY */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
wlc_idac_preload_20691(phy_info_t *pi, int16 i, int16 q)
{
	uint16 cnt;
	ASSERT(ACREV_GE(pi->pubpi->phy_rev, 11));

	if (i < 0)
		i += 512;
	if (q < 0)
		q += 512;

	i &= 0x1ff;
	q &= 0x1ff;

	/* WAR for negative values -ensure fsm is running */
	MOD_PHYREG(pi, RfseqTrigger, en_pkt_proc_dcc_ctrl, 0x0);
	cnt = READ_PHYREG(pi, rx_tia_dc_loop_gain_5);
	WRITE_PHYREG(pi, rx_tia_dc_loop_gain_5, 15); /* restart gain is 0, ie NOP */
	MOD_PHYREG(pi, rx_tia_dc_loop_0, en_lock, 0); /* always run */
	OSL_DELAY(1);

	/* overide offset comp PU; phyctrl logic not reliable for preload WAR,
	   dac value fails to update occassionally
	*/
	MOD_RADIO_REG_20691(pi, TIA_CFG15, 0, tia_offset_comp_pwrup, 1);
	MOD_RADIO_REG_20691(pi, RX_BB_2G_OVR_NORTH, 0, ovr_tia_offset_comp_pwrup, 1);

	/* restart to enable preload WAR for negative value (CRDOT11ACPHY-871) */
	wlc_dcc_fsm_restart(pi);

	/* write the values across */
	WRITE_PHYREG(pi, BfmConfig3, 0x0200 | i);
	WRITE_PHYREG(pi, BfmConfig3, 0x0600 | q);
	WRITE_PHYREG(pi, BfmConfig3, 0x0000);

	/* remove comp pwrup force */
	MOD_RADIO_REG_20691(pi, RX_BB_2G_OVR_NORTH, 0, ovr_tia_offset_comp_pwrup, 0);

	/* stop loop running and restore config */
	MOD_PHYREG(pi, rx_tia_dc_loop_0, en_lock, 1);

	OSL_DELAY(1);

	WRITE_PHYREG(pi, rx_tia_dc_loop_gain_5, cnt);

	MOD_PHYREG(pi, RfseqTrigger, en_pkt_proc_dcc_ctrl, 0x1);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

void
phy_ac_radio_cal_init(phy_info_t *pi)
{
	if (TINY_RADIO(pi)) {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		/* switch back to original rx2tx seq and dly for tiny cal */
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			wlapi_suspend_mac_and_wait(pi->sh->physhim);
			phy_utils_phyreg_enter(pi);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

		phy_ac_rfseq_mode_set(pi, 1);

		if (pi->u.pi_acphy->radioi->data->dac_mode != 1) {
			pi->u.pi_acphy->radioi->data->dac_mode = 1;
			wlc_phy_dac_rate_mode_acphy(pi, pi->u.pi_acphy->radioi->data->dac_mode);
		}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
			phy_utils_phyreg_exit(pi);
			wlapi_enable_mac(pi->sh->physhim);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
	} else {
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_36) || defined(PHY_ACMAJORREV_40)))
		if ((ACMAJORREV_36(pi->pubpi->phy_rev)) ||
			(ACMAJORREV_40(pi->pubpi->phy_rev) && !(ACMINORREV_0(pi)) &&
				(pi->u.pi_acphy->sromi->srom_low_adc_rate_en))) {
			/* switch to cal rx2tx seq and dly */
			phy_ac_rfseq_mode_set(pi, 1);
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_36) || defined(PHY_ACMAJORREV_40))) */
	}
}

void
phy_ac_radio_cal_reset(phy_info_t *pi, int16 idac_i, int16 idac_q)
{
	if (TINY_RADIO(pi)) {
	/* switch to normal rx2tx seq and dly after tiny cal */
		phy_ac_rfseq_mode_set(pi, 0);
		if (pi->u.pi_acphy->radioi->data->dac_mode != (CHSPEC_IS2G(pi->radio_chanspec)
			? pi->dacratemode2g : pi->dacratemode5g)) {
			pi->u.pi_acphy->radioi->data->dac_mode = CHSPEC_IS2G(pi->radio_chanspec)
				? pi->dacratemode2g : pi->dacratemode5g;
			wlc_phy_dac_rate_mode_acphy(pi, pi->u.pi_acphy->radioi->data->dac_mode);
		}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
		if (ACMAJORREV_3(pi->pubpi->phy_rev)) {
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
			if (PHY_ILNA(pi)) {
				wlc_idac_preload_20691(pi, idac_i, idac_q);
				wlc_phy_enable_lna_dcc_comp_20691(pi, PHY_ILNA(pi));
			} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
			{
				phy_rxgcrs_stay_in_carriersearch(pi->rxgcrsi, TRUE);
				wlc_dcc_fsm_reset(pi);
				phy_rxgcrs_stay_in_carriersearch(pi->rxgcrsi, FALSE);
			}
		}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */
	} else {
		if ((ACMAJORREV_36(pi->pubpi->phy_rev)) ||
			(ACMAJORREV_40(pi->pubpi->phy_rev) && !(ACMINORREV_0(pi)) &&
				(pi->u.pi_acphy->sromi->srom_low_adc_rate_en))) {
			/* switch to normal rx2tx seq and dly after cal */
			phy_ac_rfseq_mode_set(pi, 0);
		}
	}

#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	if ((ACMAJORREV_32(pi->pubpi->phy_rev) && ACMINORREV_2(pi)) ||
	     (ACMAJORREV_33(pi->pubpi->phy_rev))) {
		/* Enable core2core sync */
		phy_ac_chanmgr_core2core_sync_setup(pi->u.pi_acphy->chanmgri, TRUE);
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */
}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void wlc_phy_radio20694_low_current_mode(phy_info_t *pi, uint8 mode)
{
	uint8 core;
	FOREACH_CORE(pi, core) {
		WRITE_RADIO_REG_20694(pi, RF, READOVERRIDES, core, 0);

		if (BF_ELNA_2G(pi->u.pi_acphy)) {
			if (mode == 0) {
				MOD_RADIO_REG_20694(pi, RF, LNA2G_REG2, core,
						lna2g_lna1_bias_idac, 3);
				MOD_RADIO_REG_20694(pi, RF, RX2G_REG2, core,
						rx2g_gm_idac_main, 3);
			} else if (mode == 1) {
				MOD_RADIO_REG_20694(pi, RF, LNA2G_REG2, core,
						lna2g_lna1_bias_idac, 2);
				MOD_RADIO_REG_20694(pi, RF, RX2G_REG2, core,
						rx2g_gm_idac_main, 3);
			} else {
				MOD_RADIO_REG_20694(pi, RF, LNA2G_REG2, core,
						lna2g_lna1_bias_idac, 2);
				MOD_RADIO_REG_20694(pi, RF, RX2G_REG2, core,
						rx2g_gm_idac_main, 2);
			}
		}

		if (BF_ELNA_5G(pi->u.pi_acphy)) {
			MOD_RADIO_REG_20694(pi, RF, RX5G_REG2, core, rx5g_lna_cc, 5);
			MOD_RADIO_REG_20694(pi, RF, RX5G_REG4, core, rx5g_gm_cc, 7);
		}
		WRITE_RADIO_REG_20694(pi, RF, READOVERRIDES, core, 1);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static int
wlc_tiny_sigdel_fast_mult(int raw_p8, int mult_p12, int max_val, int rshift)
{
	int prod;
	/* >> 20 follows from .12 fixed-point for mult, various.8 for raw */
	prod = (mult_p12 * raw_p8) >> rshift;
	return (prod > max_val) ? max_val : prod;
}

static int
wlc_tiny_sigdel_wrap(int prod, int max_val)
{
	/* to make sure you hit the maximum number of bits in word allocated  */
	return (prod > max_val) ? max_val : prod;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

#define WLC_TINY_GI_MULT_P12		4096U
#define WLC_TINY_GI_MULT_TWEAK_P12	4096U
#define WLC_TINY_GI_MULT		WLC_TINY_GI_MULT_P12
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static void
wlc_tiny_sigdel_slow_tune(phy_info_t *pi, int g_mult_raw_p12,
	tiny_adc_tuning_array_t *gvalues, uint8 bw)
{
	int g_mult_p12;
	int ri = 0;
	int r21;
	int r32;
	int r43;
	int rff1;
	int rff2;
	int rff3;
	int rff4;
	int r12v;
	int r34v;
	int r11v;
	int g21;
	int g32;
	int g43;
	int r12;
	int r34;
	int g11;
	int temp;
	int gff3_val = 0;
	uint8 shift_val = 0;
	BCM_REFERENCE(pi);
	/* RC cals the slow ADC IN 40MHz channels or 20MHz bandwidth, based on g_mult */
	/* input signal scaling, changes ADC gain, 4096 <=> 1.0 for g_mult and gi_mult */
	/* Function is 32 bit (signed) integer arithmetic and a/b division rounding  */
	/* is performed in integers from: (a-1)/b+1 */

	/* ERR! 20691_rc_cal "adc" returns the RC value, so correction is 1/rccal! */
	/* so invert it */
	/* This is a nice way of inverting the number... jnh */
	/* inverse of gmult precomputed to minimise division operations for speed */
	/* 4.12 fixed point so scale reciprocal by 2^24 */
	g_mult_p12 = g_mult_raw_p12 > 0 ? g_mult_raw_p12 : 1;
	/* tweak to g_mult */
	g_mult_p12 = (WLC_TINY_GI_MULT_TWEAK_P12 * g_mult_p12) >> 12;

	/* to avoid divide by zeros and negative values */
	if (g_mult_p12 <= 0)
		g_mult_p12 = 1;

	if (bw == 20) {
	    shift_val = 1;
		ri = 10176;
		gff3_val = 32000;
	} else if (bw == 40) {
		shift_val = 0;
		ri = 10670;
		gff3_val = 12000;
	}

	/* RC cal in slow ADC is mostly of the form Runit/(Rval/(g_mult/2**12)-Roff). */
	/* For integer manipulation do Runit/({Rval*2**12}/gmult-Roff).  */
	/* where Rval*2**12 are res design values in matlab script {x kint234 , kr12, kr34} */
	/* but right shifted a number of times */
	/* All but r11 and rff4 resistances are x2 for 20MHz. */

	/* Rvals from matlab already scaled by kint234, kr12 */
	/* or kr34 (due to amplifier finite GBW) */
	/* x2 for half the BW and half the sampling frequency. */
	ri = ri << shift_val;
	r21 = 8323 << shift_val;
	r32 = 6390 << shift_val;
	r43 = 6827 << shift_val;
	rff1 = 19768 << shift_val;
	rff2 = 16916 << shift_val;
	rff3 = 29113  << shift_val;
	/* rff4 does not double with 20MHz channels, 10MHz BW. */
	rff4 = 100000;
	r12v = 83205 << shift_val;
	r34v = 243530 << shift_val;
	/* rff4 does not double with 20MHz channels, 10MHz BW. */
	r11v = 8000;

	/* saturate correctly when you get negative numbers and round divisions */
	/* subject to gmult twice so scale gmult back to 12b so it only divides with 12b+ r21 */
	g21 = (g_mult_p12 * g_mult_p12) >> 12;
	if (g21 <= 0)
		g21 = 1;
	g21 = ((r21 << 12) - 1) / g21 + 1;
	g21 = (512000 - 1) / g21 + 1;
	g21 = wlc_tiny_sigdel_wrap(g21, 127);
	g32 = (256000 - 1) / (((r32 << 12) - 1) / g_mult_p12 + 1) + 1;
	g32 = wlc_tiny_sigdel_wrap(g32, 127);
	g43 = (256000 - 1) / (((r43 << 12) - 1) / g_mult_p12 + 1) + 1;
	g43  = wlc_tiny_sigdel_wrap(g43, 127);

	/* gff1234 subject to gmult and gimult; step operations so range */
	/* is not exceeded and scale is correct */
	/* gff1234 will overflow if $g_mult_p12*$gi_mult < 1023*2, eq. to 0.25, */
	/* assuming rff1234 {<<2} < 131072 */

	/* gi */
	temp = (((ri << 12) - 1) / WLC_TINY_GI_MULT) - 4000 + 1;
	ASSERT(temp > 0);	/* should be a positive value */
	temp = (256000 - 1) / temp + 1;
	temp = wlc_tiny_sigdel_wrap(temp, 127);
	gvalues->gi = (uint16) temp;

	/* gff1 */
	temp = ((((((rff1 << 12) - 1) / g_mult_p12 + 1) << 12) - 1) / WLC_TINY_GI_MULT) - 8000 + 1;
	if (temp <= 0)
		temp = 1;
	temp = (256000 - 1) / temp + 1;
	temp = wlc_tiny_sigdel_wrap(temp, 127);
	gvalues->gff1 = (uint16) temp;

	/* gff2 */
	temp = ((((((rff2 << 12) - 1) / g_mult_p12 + 1) << 12) - 1) / WLC_TINY_GI_MULT) - 4000 + 1;
	if (temp <= 0)
		temp = 1;
	temp = (256000 - 1) / temp + 1;
	temp = wlc_tiny_sigdel_wrap(temp, 127);
	gvalues->gff2 = (uint16) temp;

	/* gff3 */
	temp = ((((((rff3 << 12) - 1) / g_mult_p12 + 1) << 12) - 1) / WLC_TINY_GI_MULT) - gff3_val
	    + 1;

	if (temp <= 0)
		temp = 1;
	temp = (256000 - 1) / temp + 1;
	temp = wlc_tiny_sigdel_wrap(temp, 127);
	gvalues->gff3 = (uint16) temp;

	/* gff4 */
	temp = (((rff4 << 12) - 1) / WLC_TINY_GI_MULT) - 72000 + 1;	/* subject to gimult only */
	ASSERT(temp > 0);	/* should be a positive value */
	temp = (256000 - 1) / temp + 1;
	temp  = wlc_tiny_sigdel_wrap(temp, 255);
	gvalues->gff4 = (uint16) temp;

	/* stays constant to RC shifts, g21 shifts twice for it. */
	r12 = (r12v - 1) / 4000 + 1;
	r12 = wlc_tiny_sigdel_wrap(r12, 127);

	if (bw == 20)
		r34 = ((((r34v << 12) - 1) / g_mult_p12 +1) - 128000 - 1) / 4000 + 1;
	else
		r34 = ((r34v << 12) - 1) / g_mult_p12 / 4000 + 1;

	if (r34 <= 0)
		r34 = 1;
	r34 = wlc_tiny_sigdel_wrap(r34, 127);
	g11 = (((r11v << 12) - 1) / g_mult_p12 +1) - 2000;
	if (g11 <= 0)
		g11 = 1;
	g11 = (128000 - 1) / g11 + 1;
	g11 = wlc_tiny_sigdel_wrap(g11, 127);

	gvalues->g21 = (uint16) g21;
	gvalues->g32 = (uint16) g32;
	gvalues->g43 = (uint16) g43;
	gvalues->r12 = (uint16) r12;
	gvalues->r34 = (uint16) r34;
	gvalues->g11 = (uint16) g11;
	PHY_TRACE(("gi   = %i\n", gvalues->gi));
	PHY_TRACE(("g21  = %i\n", gvalues->g21));
	PHY_TRACE(("g32  = %i\n", gvalues->g32));
	PHY_TRACE(("g43  = %i\n", gvalues->g43));
	PHY_TRACE(("r12  = %i\n", gvalues->r12));
	PHY_TRACE(("r34  = %i\n", gvalues->r34));
	PHY_TRACE(("gff1 = %i\n", gvalues->gff1));
	PHY_TRACE(("gff2 = %i\n", gvalues->gff2));
	PHY_TRACE(("gff3 = %i\n", gvalues->gff3));
	PHY_TRACE(("gff4 = %i\n", gvalues->gff4));
	PHY_TRACE(("g11  = %i\n", gvalues->g11));
}

static void
wlc_tiny_sigdel_fast_tune(phy_info_t *pi, int g_mult_raw_p12, tiny_adc_tuning_array_t *gvalues)
{
	int g_mult_tweak_p12;
	int g_mult_p12;
	int g_inv_p12;
	int gi_inv_p12;
	int gi_p8;
	int ri3_p8;
	int g21_p8;
	int g32_p8;
	int g43_p8;
	int g54_p8;
	int g65_p8;
	int r12_p8;
	int r34_p8;
	int gi, ri3, g21, g32, g43, g54, g65, r12, r34;

	BCM_REFERENCE(pi);

	/* tweak to g_mult to trade off stability over PVT versus performance */
	g_mult_tweak_p12 = 4096;
	g_mult_p12 = (g_mult_tweak_p12 * g_mult_raw_p12) >> 12;

	/* inverse of gmult precomputed to minimise division operations for speed */
	g_inv_p12 = 16777216 / g_mult_p12;
	gi_inv_p12 = 16777216 / WLC_TINY_GI_MULT;

	/* untuned values in p8 fixed-point format, ie. multiplied by 2^8 */
	gi_p8 = 16384;
	ri3_p8 = 1477;
	g21_p8 = 17997;
	g32_p8 = 18341;
	g43_p8 = 15551;
	g54_p8 = 19915;
	g65_p8 = 12369;
	r12_p8 = 1156;
	r34_p8 = 4331;

	/* RC cal */
	gi = wlc_tiny_sigdel_fast_mult(gi_p8, WLC_TINY_GI_MULT, 127, 20);
	ri3 = wlc_tiny_sigdel_fast_mult(ri3_p8, gi_inv_p12, 63, 20);
	g21 = wlc_tiny_sigdel_fast_mult(g21_p8, g_mult_p12, 127, 20);
	g32 = wlc_tiny_sigdel_fast_mult(g32_p8, g_mult_p12, 127, 20);
	g43 = wlc_tiny_sigdel_fast_mult(g43_p8, g_mult_p12, 127, 20);
	g54 = wlc_tiny_sigdel_fast_mult(g54_p8, g_mult_p12, 127, 20);
	g65 = wlc_tiny_sigdel_fast_mult(g65_p8, g_mult_p12, 63, 20);
	r12 = wlc_tiny_sigdel_fast_mult(r12_p8, g_inv_p12, 63, 20);
	r34 = wlc_tiny_sigdel_fast_mult(r34_p8, g_inv_p12, 63, 20);

	gvalues->gi = (uint16) gi;
	gvalues->ri3 = (uint16) ri3;
	gvalues->g21 = (uint16) g21;
	gvalues->g32 = (uint16) g32;
	gvalues->g43 = (uint16) g43;
	gvalues->g54 = (uint16) g54;
	gvalues->g65 = (uint16) g65;
	gvalues->r12 = (uint16) r12;
	gvalues->r34 = (uint16) r34;
}

static void
wlc_tiny_adc_setup_slow(phy_info_t *pi, tiny_adc_tuning_array_t *gvalues, uint8 bw, uint8 core)
{
	const chan_info_radio20693_altclkplan_t *altclkpln = altclkpln_radio20693;
	int row;
	uint8 adcclkdiv = 0x1;
	uint8 sipodiv = 0x1;

	ASSERT(TINY_RADIO(pi));

	if (ROUTER_4349(pi)) {
		altclkpln = altclkpln_radio20693_router4349;
	}
	row = wlc_phy_radio20693_altclkpln_get_chan_row(pi);

	if (row >= 0) {
		adcclkdiv = altclkpln[row].adcclkdiv;
		sipodiv = altclkpln[row].sipodiv;
	}
	/* SETS UP THE slow ADC IN 20MHz channels or 10MHz bandwidth */
	/* Function should be 32 bit (signed) arithmetic */
	if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) && (bw == 20)) {
		if (RADIOMAJORREV(pi) != 3)
			MOD_RADIO_REG_20693(pi, SPARE_CFG2, core, adc_clk_slow_div, adcclkdiv);
	} else if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
		/* EXPLICITELY ENABLE/DISABLE ADCs and INTERNAL CLKs? */
		/* Only changes between fast/slow ADC, not 20/40MHz */
		MOD_RADIO_REG_20691(pi, ADC_OVR1, core, ovr_adc_fast_pu, 0x1);
		MOD_RADIO_REG_20691(pi, ADC_CFG1, core, adc_fast_pu, 0x0);
		MOD_RADIO_REG_20691(pi, ADC_OVR1, core, ovr_adc_slow_pu, 0x0);
		MOD_RADIO_REG_20691(pi, ADC_OVR1, core, ovr_adc_clk_fast_pu, 0x1);
		MOD_RADIO_REG_20691(pi, ADC_CFG15, core, adc_clk_fast_pu, 0x0);
		MOD_RADIO_REG_20691(pi, ADC_OVR1, core, ovr_adc_clk_slow_pu, 0x0);
	}
	/* Setup internal dividers and sipo for 1G2Hz mode */
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_drive_strength, 0x4);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_div8, sipodiv);
	/* set adc_clk_slow_div3 to 0x0 in 20MHz mode, 0x1 in 40MHz mode */
	if (bw == 20)
		MOD_RADIO_REG_TINY(pi, ADC_CFG15, core, adc_clk_slow_div3, 0x0);
	else if (bw == 40)
		MOD_RADIO_REG_TINY(pi, ADC_CFG15, core, adc_clk_slow_div3, 0x1);
	if ((RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) && (bw == 40) &&
		(RADIOMAJORREV(pi) != 3))
		MOD_RADIO_REG_20693(pi, SPARE_CFG2, core, adc_clk_slow_div, adcclkdiv);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_div8, sipodiv);
	MOD_RADIO_REG_TINY(pi, ADC_CFG10, core, adc_sipo_sel_fast, 0x0);

	MOD_RADIO_REG_TINY(pi, ADC_CFG18, core, adc_od_pu, 0x0);
	MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_od_pu, 0x1);

	/* Setup biases */
	/* Slow adc halves opamp current from 20MHz to 40MHz channels */
	/* Opamp1 is 26u/0, 4u/2 other 3 are 26u/0, 4u/4,	*/
	/* so opamp1 (40M, 20M) = (0x20, 0x10), opamp234 = (0x10, 0x8) */
	MOD_RADIO_REG_TINY(pi, ADC_CFG6, core, adc_biasadj_opamp1, (0x10 * (bw/20)));
	MOD_RADIO_REG_TINY(pi, ADC_CFG6, core, adc_biasadj_opamp2, (0x8 * (bw/20)));
	MOD_RADIO_REG_TINY(pi, ADC_CFG7, core, adc_biasadj_opamp3, (0x8 * (bw/20)));
	MOD_RADIO_REG_TINY(pi, ADC_CFG7, core, adc_biasadj_opamp4, (0x8 * (bw/20)));
	MOD_RADIO_REG_TINY(pi, ADC_CFG8, core, adc_ff_mult_opamp, 0x1);
	MOD_RADIO_REG_TINY(pi, ADC_CFG9, core, adc_cmref_control, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG9, core, adc_cmref4_control, 0x40);

	/* Setup transconductances. These are tuned with gmult(RC) and/or gimult(input gain) */
	/* rnm */
	MOD_RADIO_REG_TINY(pi, ADC_CFG2, core, adc_gi, gvalues->gi);
	MOD_RADIO_REG_TINY(pi, ADC_CFG3, core, adc_g21, gvalues->g21);
	MOD_RADIO_REG_TINY(pi, ADC_CFG3, core, adc_g32, gvalues->g32);
	MOD_RADIO_REG_TINY(pi, ADC_CFG4, core, adc_g43, gvalues->g43);
	/* rff */
	MOD_RADIO_REG_TINY(pi, ADC_CFG16, core, adc_gff1, gvalues->gff1);
	MOD_RADIO_REG_TINY(pi, ADC_CFG16, core, adc_gff2, gvalues->gff2);
	MOD_RADIO_REG_TINY(pi, ADC_CFG17, core, adc_gff3, gvalues->gff3);
	MOD_RADIO_REG_TINY(pi, ADC_CFG17, core, adc_gff4, gvalues->gff4);
	/* resonator and r11 */
	MOD_RADIO_REG_TINY(pi, ADC_CFG5, core, adc_r12, gvalues->r12);
	MOD_RADIO_REG_TINY(pi, ADC_CFG8, core, adc_r34, gvalues->r34);
	MOD_RADIO_REG_TINY(pi, ADC_CFG4, core, adc_g54, gvalues->g11);

	/* Setup feedback DAC and tweak delay compensation */
	if (bw == 20) {
		/* In slow 40MHz ADC rt is 0x0, in 20MHz ADC rt is 0x2 */
	    MOD_RADIO_REG_TINY(pi, ADC_CFG19, core, adc_rt, 0x2);
		/* In slow 40MHz ADC slow_dacs is 0x2 in 20MHz ADC rt is 0x1 */
	    MOD_RADIO_REG_TINY(pi, ADC_CFG19, core, adc_slow_dacs, 0x1);
	} else if (bw == 40) {
		/* In slow 40MHz ADC rt is 0x0, in 20MHz ADC rt is 0x2 */
		MOD_RADIO_REG_TINY(pi, ADC_CFG19, core, adc_rt, 0x0);
		/* In slow 40MHz ADC slow_dacs is 0x2 in 20MHz ADC rt is 0x1 */
		MOD_RADIO_REG_TINY(pi, ADC_CFG19, core, adc_slow_dacs, 0x2);
	}
	if (RADIOMAJORREV(pi) != 3) {
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_reset_adc, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_adcs_reset, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_adcs_reset, 0x0);
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_reset_adc, 0x0);
	}
}

static void
wlc_tiny_adc_setup_fast(phy_info_t *pi, tiny_adc_tuning_array_t *gvalues, uint8 core)
{

	/* Bypass override of slow/fast adc to power up/down for 4365 radio */
	if (!(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && (RADIOMAJORREV(pi) == 3))) {
		/* EXPLICITLY ENABLE/DISABLE ADCs and INTERNAL CLKs?  */
		/* This part is comment out in TCL */
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_fast_pu, 0x0);
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_slow_pu, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_slow_pu, 0x0);
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_clk_fast_pu, 0x0);
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_clk_slow_pu, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG15, core, adc_clk_slow_pu, 0x0);
	}
	/* Setup internal dividers and sipo for 3G2Hz mode. */
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_drive_strength, 0x4);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_div8, 0x0);
	MOD_RADIO_REG_TINY(pi, ADC_CFG15, core, adc_clk_slow_div3, 0x0);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_div8, 0x1);
	MOD_RADIO_REG_TINY(pi, ADC_CFG10, core, adc_sipo_sel_fast, 0x1);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_drive_strength, 0x4);
	MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_sipo_div8, 0x0);
	MOD_RADIO_REG_TINY(pi, ADC_CFG6, core, adc_biasadj_opamp1, 0x60);
	MOD_RADIO_REG_TINY(pi, ADC_CFG6, core, adc_biasadj_opamp2, 0x60);
	MOD_RADIO_REG_TINY(pi, ADC_CFG7, core, adc_biasadj_opamp3, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG7, core, adc_biasadj_opamp4, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG8, core, adc_ff_mult_opamp, 0x1);
	MOD_RADIO_REG_TINY(pi, ADC_CFG9, core, adc_cmref_control, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG9, core, adc_cmref4_control, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG10, core, adc_sipo_sel_fast, 0x1);

	/* Turn on overload detector */
	MOD_RADIO_REG_TINY(pi, ADC_CFG18, core, adc_od_bias_comp, 0x40);
	MOD_RADIO_REG_TINY(pi, ADC_CFG18, core, adc_od_threshold, 0x3);
	MOD_RADIO_REG_TINY(pi, ADC_CFG18, core, adc_od_reset_duration, 0x3);
	MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_adc_od_pu, 0x1);

	MOD_RADIO_REG_TINY(pi, ADC_CFG18, core, adc_od_pu, 0x1);
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && (RADIOMAJORREV(pi) == 3))
		MOD_RADIO_REG_TINY(pi, ADC_CFG13, core, adc_od_disable, 0x0);

	MOD_RADIO_REG_TINY(pi, ADC_CFG2, core, adc_gi, gvalues->gi);

	/* typo in spreadsheet for TC only - should be ri3 but got called ri1 */
	MOD_RADIO_REG_TINY(pi, ADC_CFG2, core, adc_ri3, gvalues->ri3);

	MOD_RADIO_REG_TINY(pi, ADC_CFG3, core, adc_g21, gvalues->g21);
	MOD_RADIO_REG_TINY(pi, ADC_CFG3, core, adc_g32, gvalues->g32);
	MOD_RADIO_REG_TINY(pi, ADC_CFG4, core, adc_g43, gvalues->g43);
	MOD_RADIO_REG_TINY(pi, ADC_CFG4, core, adc_g54, gvalues->g54);
	MOD_RADIO_REG_TINY(pi, ADC_CFG5, core, adc_g65, gvalues->g65);
	MOD_RADIO_REG_TINY(pi, ADC_CFG5, core, adc_r12, gvalues->r12);
	MOD_RADIO_REG_TINY(pi, ADC_CFG8, core, adc_r34, gvalues->r34);
	/* Is it need in 4365? */
	if (!(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID) && (RADIOMAJORREV(pi) == 3))) {
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_reset_adc, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_adcs_reset, 0x1);
		MOD_RADIO_REG_TINY(pi, ADC_CFG1, core, adc_adcs_reset, 0x0);
		MOD_RADIO_REG_TINY(pi, ADC_OVR1, core, ovr_reset_adc, 0x0);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio20693_adc_setup(phy_ac_radio_info_t *ri, uint8 core,
	radio_20693_adc_modes_t adc_mode)
{
	tiny_adc_tuning_array_t gvalues;
	uint8 bw;
	phy_info_t *pi = ri->pi;
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	bw = CHSPEC_IS20(pi->radio_chanspec) ? 20 : CHSPEC_IS40(pi->radio_chanspec) ? 40 : 80;

	if ((adc_mode == RADIO_20693_FAST_ADC) || (CHSPEC_IS80(pi->radio_chanspec)) ||
		(CHSPEC_IS160(pi->radio_chanspec)) ||
		(CHSPEC_IS8080(pi->radio_chanspec))) {
		/* 20 and 40MHz fast mode and 80MHz channel */
		wlc_tiny_sigdel_fast_tune(pi, ri->rccal_adc_gmult, &gvalues);
		wlc_tiny_adc_setup_fast(pi, &gvalues, core);
	} else {
		/* slow mode for 20 and 40MHz channel */
		wlc_tiny_sigdel_slow_tune(pi, ri->rccal_adc_gmult, &gvalues, bw);
		wlc_tiny_adc_setup_slow(pi, &gvalues, bw, core);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
wlc_phy_radio20696_adc_setup(phy_info_t *pi, uint8 core)
{
	uint8 mode;
	ASSERT(RADIOID(pi->pubpi->radioid) == BCM20696_ID);

	MOD_RADIO_REG_20696(pi, AFE_CFG1_OVR2, core, ovr_rxadc_power_mode, 0x1);
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_power_mode_enb, 0x0);

	if (CHSPEC_IS160(pi->radio_chanspec)) {
		mode = 0x0;
	} else if (CHSPEC_IS80(pi->radio_chanspec)) {
		mode = 0x1;
	} else if (CHSPEC_IS40(pi->radio_chanspec)) {
		mode = 0x2;
	} else {
		mode = 0x3;
	}
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_power_mode, mode);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */


/*
 *  The TIA has 13 distinct gain steps.
 *  Each of the tia_* scalers are packed with the
 *  tia settings for each gain step.
 *  Mapping for each gain step is:
 *  pwrup_amp2, amp2_bypass, R1, R2, R3, R4, C1, C2, enable_st1
 */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || \
	defined(RADIO_BCM20693)))
static void
wlc_tiny_tia_config(phy_info_t *pi, uint8 core)
{
/*
 *  The TIA has 13 distinct gain steps.
 *  Each of the tia_* scalers are packed with the
 *  tia settings for each gain step.
 *  Mapping for each gain step is:
 *  pwrup_amp2, amp2_bypass, R1, R2, R3, R4, C1, C2, enable_st1
 */
	const uint8  *p8;
	const uint16 *p16;
	const uint16 *prem = NULL;
	uint16 lut, lut_51, lut_82;
	int i;

	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_80) == 52);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_40) == 52);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_20) == 52);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_16b_80) == 30);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_16b_20) == 30);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_16b_80_rem) == 4);
		STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_16b_20_rem) == 4);
	}

	ASSERT(TINY_RADIO(pi));

	if (CHSPEC_IS80(pi->radio_chanspec) ||
	PHY_AS_80P80(pi, pi->radio_chanspec)) {
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))
			prem = tiaRC_tiny_16b_80_rem;
		else
			STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_80) +
				ARRAYSIZE(tiaRC_tiny_16b_80) == 82);
		p8 = tiaRC_tiny_8b_80;
		p16 = tiaRC_tiny_16b_80;
	} else if (CHSPEC_IS40(pi->radio_chanspec)) {
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))
			prem = tiaRC_tiny_16b_40_rem;
		else
			STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_40) +
				ARRAYSIZE(tiaRC_tiny_16b_20) == 82);
		p8 = tiaRC_tiny_8b_40;
		p16 = tiaRC_tiny_16b_40;
	} else {
		if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev))
			prem = tiaRC_tiny_16b_20_rem;
		else
			STATIC_ASSERT(ARRAYSIZE(tiaRC_tiny_8b_20) +
				ARRAYSIZE(tiaRC_tiny_16b_20) == 82);
		p8 = tiaRC_tiny_8b_20;
		p16 = tiaRC_tiny_16b_20;
	}

	lut = RADIO_REG(pi, TIA_LUT_0, core);
	lut_51 = RADIO_REG(pi, TIA_LUT_51, core);
	lut_82 = RADIO_REG(pi, TIA_LUT_82, core);

	/* the assumption is that all the TIA LUT registers are in sequence */
	ASSERT(lut_82 - lut == 81);

	if (ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) {
		for (i = 0; i < 52; i++) /* LUT0-LUT51 */
			phy_utils_write_radioreg(pi, lut++, p8[i]);

		for (i = 0; i < 26; i++) /* LUT52-LUT78 (no LUT67) */
			phy_utils_write_radioreg(pi, lut++, p16[i]);

		for (i = 0; i < 4; i++) /* LUT79-LUT82 */
			phy_utils_write_radioreg(pi, lut++, prem[i]);
	} else {
		do {
			phy_utils_write_radioreg(pi, lut++, *p8++);
		} while (lut <= lut_51);

		do {
			phy_utils_write_radioreg(pi, lut++, *p16++);
		} while (lut <= lut_82);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && (defined(RADIO_BCM20691) || defined(RADIO_BCM20693))) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
void
wlc_phy_set_regtbl_on_band_change_acphy_20691(phy_info_t *pi)
{
	uint8 core, bw;
	tiny_adc_tuning_array_t gvalues;
	phy_ac_radio_info_t *ri = pi->u.pi_acphy->radioi;
	uint8 phyrxchain;

	BCM_REFERENCE(phyrxchain);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));
	bw = CHSPEC_BW_LE20(pi->radio_chanspec) ? 20 : CHSPEC_IS40(pi->radio_chanspec) ? 40 : 80;

	/* ### 20691_band_set(pi); */
	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		/* # Restore PHY control for Gband blocks which may have been switched
		 * off in Aband
		 */
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, TIA_CFG8, 0, tia_offset_dac_biasadj, 4)
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_OVR1, 0, ovr_logencore_2g_pu, 0)
			WRITE_RADIO_REG_ENTRY(pi, RADIO_REG_20691(pi, TX_TOP_2G_OVR_EAST, 0), 0)
			WRITE_RADIO_REG_ENTRY(pi,
				RADIO_REG_20691(pi, TX_TOP_2G_OVR_NORTH, 0), 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_2G_OVR_EAST, 0, ovr_gm2g_auxgm_pwrup,
				1)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_2G_OVR_EAST, 0, ovr_gm2g_pwrup, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_2G_OVR_EAST, 0,
				ovr_lna2g_dig_wrssi1_pu, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_2G_OVR_NORTH, 0, ovr_rxmix2g_pu, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_2G_OVR_NORTH, 0, ovr_lna2g_lna1_pu, 0)

			/* lna5g_pu_lna2 seems to get switched on during 5G band switch */
			MOD_RADIO_REG_20691_ENTRY(pi, LNA5G_CFG2, 0, lna5g_pu_lna2, 0)

			/* # Misc */
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LPF_CFG2, 0, lpf_sel_5g_out_gm, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LPF_CFG3, 0, lpf_sel_2g_5g_cmref_gm, 0)
		ACPHY_REG_LIST_EXECUTE(pi);

		if (IS_4364_1x1(pi)) {
			/* RSDB 1x1 Tx + 3x3 Rx: Remove 2G TR override when 1x1 is in 2G */
			MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_NORTH, 0, ovr_lna2g_tr_rx_en, 0);
			MOD_RADIO_REG_20691(pi, LNA2G_CFG1, 0, lna2g_tr_rx_en, 1);
			/* RSDB 1x1 Tx + 3x3 Rx: Force 5G TR override when 1x1 is in 2G */
			MOD_RADIO_REG_20691(pi, RX_TOP_5G_OVR, 0, ovr_lna5g_tr_rx_en, 1);
			MOD_RADIO_REG_20691(pi, LNA5G_CFG1, 0, lna5g_tr_rx_en, 0);
		}

		if (!PHY_IPA(pi)) {
			MOD_RADIO_REG_20691(pi, TXMIX2G_CFG6, 0, mx2g_idac_bbdc, 0x20);
		}

	} else {
		ACPHY_REG_LIST_START
			/* # clear 5g overrides */
			MOD_RADIO_REG_20691_ENTRY(pi, TIA_CFG8, 0, tia_offset_dac_biasadj, 12)
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_OVR1, 0, ovr_logencore_5g_pu, 0)
			WRITE_RADIO_REG_ENTRY(pi, RADIO_REG_20691(pi, TX_TOP_5G_OVR1, 0), 0)
			WRITE_RADIO_REG_ENTRY(pi, RADIO_REG_20691(pi, TX_TOP_5G_OVR2, 0), 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_5G_OVR, 0, ovr_lna5g_lna1_pu, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_5G_OVR, 0, ovr_gm5g_pwrup, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_5G_OVR, 0, ovr_lna5g_dig_wrssi1_pu, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, RX_TOP_5G_OVR, 0, ovr_lna5g_pu_auxlna2, 1)
			MOD_RADIO_REG_20691_ENTRY(pi, LNA5G_CFG2, 0, lna5g_pu_auxlna2, 0)

			/* to power up/down logen appropriately */
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LOGEN5G_CFG1, 0,
				logen5g_tx_enable_5g_low_band, 0)
			MOD_RADIO_REG_20691_ENTRY(pi, TX_TOP_5G_OVR2, 0,
				ovr_logen5g_tx_enable_5g_low_band, 1)

			/* # Restore PHY control for Gband blocks which may have been switched
			 * off in Aband
			 */
			MOD_RADIO_REG_20691_ENTRY(pi, LOGEN_OVR1, 0, ovr_logencore_5g_pu, 0)

			/* # Misc */
			/* # There is no direct control for this */
			MOD_RADIO_REG_20691_ENTRY(pi, TX_LPF_CFG2, 0, lpf_sel_5g_out_gm, 1)
		ACPHY_REG_LIST_EXECUTE(pi);

		/* Fix for LNA clamping issue in 4364-1x1 slice */
		if (IS_4364_1x1(pi)) {
			MOD_RADIO_REG_20691(pi, RX_TOP_5G_OVR, 0, ovr_rxrf5g_pu_pulse, 1);
			MOD_RADIO_REG_20691(pi, RXRF5G_CFG1, 0, rxrf5g_pu_pulse, 1);
			/* RSDB 1x1 Tx + 3x3 Rx: Force 2G TR override when 1x1 is in 5G */
			MOD_RADIO_REG_20691(pi, RX_TOP_2G_OVR_NORTH, 0, ovr_lna2g_tr_rx_en, 1);
			MOD_RADIO_REG_20691(pi, LNA2G_CFG1, 0, lna2g_tr_rx_en, 0);
			/* RSDB 1x1 Tx + 3x3 Rx: Remove 5G TR override when 1x1 is in 5G */
			MOD_RADIO_REG_20691(pi, RX_TOP_5G_OVR, 0, ovr_lna5g_tr_rx_en, 0);
			MOD_RADIO_REG_20691(pi, LNA5G_CFG1, 0, lna5g_tr_rx_en, 1);
		}
		/* # There is no direct control for this */
		MOD_RADIO_REG_20691(pi, TX_LPF_CFG3, 0, lpf_sel_2g_5g_cmref_gm,
				(PHY_IPA(pi)) ? 1 : 0);

		if ((RADIO20691_MAJORREV(pi->pubpi->radiorev) != 0) && !(PHY_IPA(pi))) {
			MOD_RADIO_REG_20691(pi, TXMIX2G_CFG6, 0, mx2g_idac_bbdc, 0xb);
			if (IBOARD(pi)) {
				ACPHY_REG_LIST_START
					MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG8, 0,
						pad5g_idac_gm, 0x18)
					MOD_RADIO_REG_20691_ENTRY(pi, PA5G_INCAP, 0,
						pad5g_idac_pmos, 0x34)
					MOD_RADIO_REG_20691_ENTRY(pi, TX5G_CFG1, 0, pad5g_slope_gm,
						0x0)
					MOD_RADIO_REG_20691_ENTRY(pi, TXMIX5G_CFG6, 0,
						mx5g_ptat_slope_lodc, 0x0)
					MOD_RADIO_REG_20691_ENTRY(pi, PA5G_INCAP, 0,
						pa5g_idac_incap_compen_main, 0x2f)
				ACPHY_REG_LIST_EXECUTE(pi);
			}
		}
	}
	if (CHSPEC_IS80(pi->radio_chanspec)) {
		/* set gvalues [20691_sigdel_fast_tune $def(radio_rccal_adc_gmult)] */
		wlc_tiny_sigdel_fast_tune(pi, ri->rccal_adc_gmult, &gvalues);
		/* 20691_adc_setup_fast $gvalues */
		wlc_tiny_adc_setup_fast(pi, &gvalues, 0);
	} else {
		/* set gvalues [20691_sigdel_slow0g6_tune $def(radio_rccal_adc_gmult)] */
		wlc_tiny_sigdel_slow_tune(pi, ri->rccal_adc_gmult, &gvalues, bw);
		/* 20691_adc_setup_slow0g6 $gvalues */
		wlc_tiny_adc_setup_slow(pi, &gvalues, bw, 0);
	}

	phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
	FOREACH_ACTV_CORE(pi, phyrxchain, core) {
	    wlc_tiny_tia_config(pi, core);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069))

/*  lookup radio-chip-specific channel code */
int
wlc_phy_chan2freq_acphy(phy_info_t *pi, uint8 channel, const void **chan_info)
{
	uint i;
	const chan_info_radio2069_t *chan_info_tbl = NULL;
	const chan_info_radio2069revGE16_t *chan_info_tbl_GE16 = NULL;
	const chan_info_radio2069revGE25_t *chan_info_tbl_GE25 = NULL;
	const chan_info_radio2069revGE32_t *chan_info_tbl_GE32 = NULL;
	const chan_info_radio2069revGE25_52MHz_t *chan_info_tbl_GE25_52MHz = NULL;

	phy_ac_radio_info_t *radioi = pi->u.pi_acphy->radioi;
	uint32 tbl_len = 0;
	int freq;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));
	switch (RADIO2069_MAJORREV(pi->pubpi->radiorev)) {
	case 0:
		switch (RADIO2069REV(pi->pubpi->radiorev)) {
		case 3:
			chan_info_tbl = chan_tuning_2069rev3;
			tbl_len = ARRAYSIZE(chan_tuning_2069rev3);
		break;

		case 4:
		case 8:
			chan_info_tbl = chan_tuning_2069rev4;
			tbl_len = ARRAYSIZE(chan_tuning_2069rev4);
			break;
		case 7: /* e.g. 43602a0 */
			chan_info_tbl = chan_tuning_2069rev7;
			tbl_len = ARRAYSIZE(chan_tuning_2069rev7);
			break;
		case 64: /* e.g. 4364 */
			chan_info_tbl = chan_tuning_2069rev64;
			tbl_len = ARRAYSIZE(chan_tuning_2069rev64);
			break;
		case 66: /* e.g. 4364 */
			chan_info_tbl = chan_tuning_2069rev66;
			tbl_len = ARRAYSIZE(chan_tuning_2069rev66);
			break;
		default:

			PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			           pi->sh->unit, __FUNCTION__, RADIO2069REV(pi->pubpi->radiorev)));
			ASSERT(0);
		}
		break;

	case 1:
		switch (RADIO2069REV(pi->pubpi->radiorev)) {
			case 16:
				if (PHY_XTAL_IS40M(pi)) {
#ifndef ACPHY_1X1_37P4
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure the LP mode settings */
						/* For Rev16/17/18 using the same LP setting TBD */
						chan_info_tbl_GE16 = chan_tuning_2069rev_GE16_40_lp;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_GE16_40_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_16_17_40;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_16_17_40);
					}
#else
					ASSERT(0);
#endif /* ACPHY_1X1_37P4 */
				} else {
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure the LP mode settings */
						/* For Rev16/17/18 using the same LP setting TBD */
						chan_info_tbl_GE16 = chan_tuning_2069rev_GE16_lp;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_GE16_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_16_17;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_16_17);
					}
				}
				radioi->acphy_prev_lp_mode = radioi->acphy_lp_mode;
				break;
			case 17:
			case 23:
				if (PHY_XTAL_IS40M(pi)) {
#ifndef ACPHY_1X1_37P4
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure the LP mode settings */
						/* For Rev16/17/18 using the same LP setting TBD */
						chan_info_tbl_GE16 =
						       chan_tuning_2069rev_GE16_40_lp;
						tbl_len =
						       ARRAYSIZE(chan_tuning_2069rev_GE16_40_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_16_17_40;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_16_17_40);
					}
#else
					ASSERT(0);
#endif /* ACPHY_1X1_37P4 */
				} else {
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
#ifndef ACPHY_1X1_37P4
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure the LP mode settings */
						/* For Rev16/17/18 using the same LP setting TBD */
						chan_info_tbl_GE16 = chan_tuning_2069rev_GE16_lp;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_GE16_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_16_17;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_16_17);
					}
#else
					if ((RADIO2069REV(pi->pubpi->radiorev)) == 23) {
						chan_info_tbl_GE16 =
						 chan_tuning_2069rev_23_2Glp_5Gnonlp;
						tbl_len =
						 ARRAYSIZE(chan_tuning_2069rev_23_2Glp_5Gnonlp);

					} else {
						chan_info_tbl_GE16 =
						 chan_tuning_2069rev_GE16_2Glp_5Gnonlp;
						tbl_len =
						 ARRAYSIZE(chan_tuning_2069rev_GE16_2Glp_5Gnonlp);
					}
#endif /* ACPHY_1X1_37P4 */
				}
				radioi->acphy_prev_lp_mode = radioi->acphy_lp_mode;
				break;
			case 18:
			case 24:
				if (PHY_XTAL_IS40M(pi)) {
#ifndef ACPHY_1X1_37P4
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure the LP mode settings */
						/* For Rev16/17/18 using the same LP setting TBD */
						chan_info_tbl_GE16 =
						    chan_tuning_2069rev_GE16_40_lp;
						tbl_len =
						    ARRAYSIZE(chan_tuning_2069rev_GE16_40_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_18_40;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_18_40);
					}
#else
					ASSERT(0);
#endif /* ACPHY_1X1_37P4 */
				} else {
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
					    (radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						/* In this configure LP mode settings */
						/* For Rev16/17/18 using same LP setting TBD */
						chan_info_tbl_GE16 =
						       chan_tuning_2069rev_GE16_lp;
						tbl_len =
						       ARRAYSIZE(chan_tuning_2069rev_GE16_lp);
					} else {
						chan_info_tbl_GE16 = chan_tuning_2069rev_18;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_18);
					}
				}
				radioi->acphy_prev_lp_mode = radioi->acphy_lp_mode;
				break;
			case 25:
			case 26:
				if (PHY_XTAL_IS40M(pi)) {
#ifndef ACPHY_1X1_37P4
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;

					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						chan_info_tbl_GE25 =
							chan_tuning_2069rev_GE_25_40MHz_lp;
						tbl_len =
						ARRAYSIZE(chan_tuning_2069rev_GE_25_40MHz_lp);
					} else {
						chan_info_tbl_GE25 =
						     chan_tuning_2069rev_GE_25_40MHz;
						tbl_len =
						     ARRAYSIZE(chan_tuning_2069rev_GE_25_40MHz);
					}
#else
					ASSERT(0);
#endif /* ACPHY_1X1_37P4 */
				} else if (PHY_XTAL_IS52M(pi)) {
					chan_info_tbl_GE25_52MHz = phy_ac_tbl_get_data
						(radioi->aci->tbli)->chan_tuning;
					tbl_len = phy_ac_tbl_get_data
						(radioi->aci->tbli)->chan_tuning_tbl_len;
				} else {
					radioi->data->acphy_lp_status = radioi->acphy_lp_mode;
					if ((radioi->acphy_lp_mode == 2) ||
						(radioi->acphy_lp_mode == 3) ||
						(radioi->data->acphy_force_lpvco_2G == 1 &&
						CHSPEC_IS2G(pi->radio_chanspec))) {
						chan_info_tbl_GE25 = chan_tuning_2069rev_GE_25_lp;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_GE_25_lp);
					} else {
						chan_info_tbl_GE25 = chan_tuning_2069rev_GE_25;
						tbl_len = ARRAYSIZE(chan_tuning_2069rev_GE_25);
					}
				}
				radioi->acphy_prev_lp_mode = radioi->acphy_lp_mode;
				break;
			default:
				PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
				   pi->sh->unit, __FUNCTION__, RADIO2069REV(pi->pubpi->radiorev)));
				ASSERT(0);
		}

		break;

	case 2:
		switch (RADIO2069REV(pi->pubpi->radiorev)) {
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
		case 44:
			/* can have more conditions based on different radio revs */
			/*  RADIOREV(pi->pubpi->radiorev) =32/33/34 */
			/* currently tuning tbls for these are all same */
			chan_info_tbl_GE32 = phy_ac_tbl_get_data(radioi->aci->tbli)->chan_tuning;
			tbl_len = phy_ac_tbl_get_data(radioi->aci->tbli)->chan_tuning_tbl_len;
			break;

		default:

			PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			           pi->sh->unit, __FUNCTION__, RADIO2069REV(pi->pubpi->radiorev)));
			ASSERT(0);
		}
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio major revision %d\n",
		           pi->sh->unit, __FUNCTION__, RADIO2069_MAJORREV(pi->pubpi->radiorev)));
		ASSERT(0);
	}

	for (i = 0; i < tbl_len; i++) {

		if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
			if (chan_info_tbl_GE32[i].chan == channel)
				break;
		} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
			if ((RADIO2069REV(pi->pubpi->radiorev) == 25) ||
			   (RADIO2069REV(pi->pubpi->radiorev) == 26))  {
			    if (!PHY_XTAL_IS52M(pi)) {
					if (chan_info_tbl_GE25[i].chan == channel)
						break;
				} else {
					if (chan_info_tbl_GE25_52MHz[i].chan == channel)
						break;
				}
			}
			else if (chan_info_tbl_GE16[i].chan == channel)
				break;
		} else {
			if (chan_info_tbl[i].chan == channel)
				break;
		}
	}

	if (i >= tbl_len) {
		PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
		           pi->sh->unit, __FUNCTION__, channel));
		ASSERT(i < tbl_len);

		return -1;
	}

	if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 2) {
		*chan_info = &chan_info_tbl_GE32[i];
		freq = chan_info_tbl_GE32[i].freq;
	} else if (RADIO2069_MAJORREV(pi->pubpi->radiorev) == 1) {
		if ((RADIO2069REV(pi->pubpi->radiorev) == 25) ||
			(RADIO2069REV(pi->pubpi->radiorev) == 26)) {
				if (!PHY_XTAL_IS52M(pi)) {
					*chan_info = &chan_info_tbl_GE25[i];
					freq = chan_info_tbl_GE25[i].freq;
				} else {
					*chan_info = &chan_info_tbl_GE25_52MHz[i];
					freq = chan_info_tbl_GE25_52MHz[i].freq;
				}
		} else {
			*chan_info = &chan_info_tbl_GE16[i];
			freq = chan_info_tbl_GE16[i].freq;
		}
	} else {
		*chan_info = &chan_info_tbl[i];
		freq = chan_info_tbl[i].freq;
	}

	return freq;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM2069)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
int
wlc_phy_chan2freq_20691(phy_info_t *pi, uint8 channel, const chan_info_radio20691_t **chan_info)
{
	uint i;
	const chan_info_radio20691_t *chan_info_tbl;
	uint32 tbl_len;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20691_ID));

	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	/* Choose the right table to use */
	switch (RADIO20691REV(pi->pubpi->radiorev)) {
	case 60:
	case 68:
		chan_info_tbl = chan_tuning_20691_rev68;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev68);
		break;
	case 75:
		chan_info_tbl = chan_tuning_20691_rev75;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev75);
		break;
	case 79:
		chan_info_tbl = chan_tuning_20691_rev79;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev79);
		break;
	case 74:
	case 82:
		chan_info_tbl = chan_tuning_20691_rev82;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev82);
		break;
	case 85:
	case 86:
	case 87:
	case 88:
		chan_info_tbl = chan_tuning_20691_rev88;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev88);
		break;
	case 129:
		chan_info_tbl = chan_tuning_20691_rev129;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev129);
		break;
	case 130:
		chan_info_tbl = chan_tuning_20691_rev130;
		tbl_len = ARRAYSIZE(chan_tuning_20691_rev130);
		break;
	default:
		PHY_ERROR(("wl%d: %s: Unsupported radio revision %d\n",
			pi->sh->unit, __FUNCTION__, RADIO20691REV(pi->pubpi->radiorev)));
		ASSERT(FALSE);
		return -1;
	}

	for (i = 0; i < tbl_len && chan_info_tbl[i].chan != channel; i++);

	if (i >= tbl_len) {
		PHY_ERROR(("wl%d: %s: channel %d not found in channel table\n",
		           pi->sh->unit, __FUNCTION__, channel));
		ASSERT(tbl_len == 0 || i < tbl_len);
		return -1;
	}

	*chan_info = &chan_info_tbl[i];

	return chan_info_tbl[i].freq;
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

/* 20691_lpf_tx_set is the top Tx LPF function and should be the usual */
/* function called from acphyprocs or used from the REPL in the lab */
void
wlc_phy_radio_tiny_lpf_tx_set(phy_info_t *pi, int8 bq_bw, int8 bq_gain,
	int8 rc_bw_ofdm, int8 rc_bw_cck)
{
	uint8 i, core;
	uint16 gmult;
	uint16 gmult_rc;
	uint16 g10_tuned, g11_tuned, g12_tuned, g21_tuned, bias;

	gmult = pi->u.pi_acphy->radioi->data->rccal_gmult;
	gmult_rc = pi->u.pi_acphy->radioi->data->rccal_gmult_rc;

	/* search for given bq_gain */
	for (i = 0; i < ARRAYSIZE(g_index1); i++) {
		if (bq_gain == g_index1[i])
			break;
	}

	if (i < ARRAYSIZE(g_index1)) {
		uint16 g_passive_rc_tx_tuned_ofdm, g_passive_rc_tx_tuned_cck;
		g10_tuned = (lpf_g10[bq_bw][i] * gmult) >> 15;
		g11_tuned = (lpf_g11[bq_bw] * gmult) >> 15;
		g12_tuned = (lpf_g12[bq_bw][i] * gmult) >> 15;
		g21_tuned = (lpf_g21[bq_bw][i] * gmult) >> 15;
		g_passive_rc_tx_tuned_ofdm = (g_passive_rc_tx[rc_bw_ofdm] * gmult_rc) >> 15;
		g_passive_rc_tx_tuned_cck = (g_passive_rc_tx[rc_bw_cck] * gmult_rc) >> 15;

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
		if (ACMAJORREV_3(pi->pubpi->phy_rev) &&
				RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
		   g10_tuned = (g10_tuned > 8191) ? 8191 : g10_tuned;
		   g11_tuned = (g11_tuned > 8191) ? 8191 : g11_tuned;
		   g12_tuned = (g12_tuned > 8191) ? 8191 : g12_tuned;
		   g21_tuned = (g21_tuned > 8191) ? 8191 : g21_tuned;
		   g_passive_rc_tx_tuned_ofdm = (g_passive_rc_tx_tuned_ofdm > 511)
			   ? 511 : g_passive_rc_tx_tuned_ofdm;
		   g_passive_rc_tx_tuned_cck = (g_passive_rc_tx_tuned_cck > 511)
			   ? 511 : g_passive_rc_tx_tuned_cck;
		}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

		bias = biases[bq_bw];
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG3, core, lpf_g10, g10_tuned);
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG7, core, lpf_g11, g11_tuned);
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG4, core, lpf_g12, g12_tuned);
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG5, core, lpf_g21, g21_tuned);
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG6, core, lpf_g_passive_rc_tx,
				g_passive_rc_tx_tuned_ofdm);
			MOD_RADIO_REG_TINY(pi, TX_LPF_CFG8, core, lpf_bias_bq, bias);
		}
		if (D11REV_IS(pi->sh->corerev, 47) || D11REV_IS(pi->sh->corerev, 54) ||
			D11REV_IS(pi->sh->corerev, 58)) {
			/* Note down the values of the passive_rc for OFDM and CCK in Shmem */
			wlapi_bmac_write_shm(pi->sh->physhim,
				M_LPF_PASSIVE_RC_OFDM(pi), g_passive_rc_tx_tuned_ofdm);
			wlapi_bmac_write_shm(pi->sh->physhim, M_LPF_PASSIVE_RC_CCK(pi),
				g_passive_rc_tx_tuned_cck);
		}
	} else {
		PHY_ERROR(("wl%d: %s: Invalid bq_gain %d\n", pi->sh->unit, __FUNCTION__, bq_gain));
	}
#if !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33)))
	/* Change IIR filter shape to solve bandedge problem for 42q(5210q) channel */
	if ((ACMAJORREV_32(pi->pubpi->phy_rev) || ACMAJORREV_33(pi->pubpi->phy_rev)) &&
		(CHSPEC_IS80(pi->radio_chanspec))) {
		if (CHSPEC_CHANNEL(pi->radio_chanspec) == 42) {
			WRITE_PHYREG(pi, txfilt80st0a1, 0xf8b);
			WRITE_PHYREG(pi, txfilt80st0a2, 0x343);
			WRITE_PHYREG(pi, txfilt80st1a1, 0xe72);
			WRITE_PHYREG(pi, txfilt80st1a2, 0x11f);
			WRITE_PHYREG(pi, txfilt80st2a1, 0xc57);
			WRITE_PHYREG(pi, txfilt80st2a2, 0x166);
			/* Reducing rc_filter bandwidth for 42q channel */
			/* MOD_RADIO_ALLREG_20693(pi, TX_LPF_CFG6, lpf_g_passive_rc_tx, 25); */
		} else {
			WRITE_PHYREG(pi, txfilt80st0a1, 0xf93);
			WRITE_PHYREG(pi, txfilt80st0a2, 0x36e);
			WRITE_PHYREG(pi, txfilt80st1a1, 0xdf7);
			WRITE_PHYREG(pi, txfilt80st1a2, 0x257);
			WRITE_PHYREG(pi, txfilt80st2a1, 0xbe2);
			WRITE_PHYREG(pi, txfilt80st2a2, 0x16c);
		}
	}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && (defined(PHY_ACMAJORREV_32) || defined(PHY_ACMAJORREV_33))) */

}

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio20693_set_channel_bw(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core)
{
	const chan_info_radio20693_altclkplan_t *altclkpln = altclkpln_radio20693;
	int row;
	uint8 dac_rate_mode;
	uint8 dacclkdiv = 0;
	uint16 dac_rate;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (ROUTER_4349(pi)) {
		altclkpln = altclkpln_radio20693_router4349;
	}
	row = wlc_phy_radio20693_altclkpln_get_chan_row(pi);

	dac_rate_mode = pi_ac->radioi->data->dac_mode;
	dac_rate = wlc_phy_get_dac_rate_from_mode(pi, dac_rate_mode);
	ASSERT(dac_rate_mode <= 2);

	if (adc_mode == RADIO_20693_FAST_ADC) {
		if (dac_rate_mode == 1) {
			if (dac_rate == 200)
				dacclkdiv = 3;
			else if (dac_rate == 400)
				dacclkdiv = 1;
			else
				dacclkdiv = 0;
		} else if (dac_rate_mode == 2) {
			dacclkdiv = 0;
		}
	} else {
		if (dac_rate_mode == 1) {
			if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
				if (CHSPEC_IS5G(pi->radio_chanspec) &&
					(CHSPEC_IS20(pi->radio_chanspec) ||
					CHSPEC_IS40(pi->radio_chanspec)) &&
					!PHY_IPA(pi) && !ROUTER_4349(pi)) {
					dacclkdiv = 1;
				} else {
					dacclkdiv = 0;
				}
			} else {
				dacclkdiv = 0;
			}
		} else {
			PHY_ERROR(("wl%d: %s: unknown dac rate mode slow\n",
				pi->sh->unit, __FUNCTION__));
		}
	}
	if ((row >= 0) && (adc_mode != RADIO_20693_FAST_ADC)) {
		dacclkdiv = altclkpln[row].dacclkdiv;
	}
	MOD_RADIO_REG_20693(pi, CLK_DIV_CFG1, core, sel_dac_div, dacclkdiv);
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
		if (CHSPEC_IS5G(pi->radio_chanspec) &&
			(CHSPEC_IS20(pi->radio_chanspec) ||
			CHSPEC_IS40(pi->radio_chanspec)) &&
			!PHY_IPA(pi) && !ROUTER_4349(pi)) {
			MOD_RADIO_REG_20693(pi, TX_AFE_CFG1, core, afe_ctl_misc, 0);
		} else {
			MOD_RADIO_REG_20693(pi, TX_AFE_CFG1, core, afe_ctl_misc, 4);
		}

		/* SWWLAN-79504: Increasing the DAC frequency for Ch-64 in order to avoid DAC spur
		 * in other core in RSDB mode
		 */
		if (row >= 0) {
			MOD_RADIO_REG_20693(pi, TX_AFE_CFG1, core, afe_ctl_misc,
				altclkpln[row].dacdiv << 2);
		}

	}
}

int
wlc_phy_radio20693_altclkpln_get_chan_row(phy_info_t *pi)
{
	const chan_info_radio20693_altclkplan_t *altclkpln = altclkpln_radio20693;
	int tbl_len = ARRAYSIZE(altclkpln_radio20693);
	int row;
	int channel = CHSPEC_CHANNEL(pi->radio_chanspec);
	int bw	= CHSPEC_IS20(pi->radio_chanspec) ? 20 : CHSPEC_IS40(pi->radio_chanspec) ? 40 : 80;

	if (ROUTER_4349(pi)) {
		altclkpln = altclkpln_radio20693_router4349;
	        tbl_len = ARRAYSIZE(altclkpln_radio20693_router4349);
	}
#ifdef WL11ULB
	if (PHY_ULB_ENAB(pi)) {
		if (CHSPEC_IS10(pi->radio_chanspec))
			bw = 10;
		else if (CHSPEC_IS5(pi->radio_chanspec))
			bw = 5;
		else if (CHSPEC_IS2P5(pi->radio_chanspec))
			bw = 2;
	}
#endif	/* WL11ULB */
	for (row = 0; row < tbl_len; row++) {
		if ((altclkpln[row].channel == channel) && (altclkpln[row].bw == bw)) {
			break;
		}
	}

	/* 53574: SWWLAN-79504 & SWWLAN-76882: DAC/ADC-SIPO spur issue is seen in RSDB mode */
	if (ROUTER_4349(pi) && (phy_get_phymode(pi) != PHYMODE_RSDB)) {
		row = tbl_len;
	}
	return ((row < tbl_len) &&
		(ROUTER_4349(pi) ? ALTCLKPLN_ENABLE_ROUTER4349 : ALTCLKPLN_ENABLE)) ? row : -1;
}

static void
wlc_phy_radio20693_adc_config_overrides(phy_info_t *pi,
	radio_20693_adc_modes_t adc_mode, uint8 core)
{
	uint8 is_fast;
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	is_fast = (adc_mode == RADIO_20693_FAST_ADC);
	wlc_phy_radio20693_adc_powerupdown(pi, RADIO_20693_SLOW_ADC, !is_fast, core);
	wlc_phy_radio20693_adc_powerupdown(pi, RADIO_20693_FAST_ADC, is_fast, core);
}

static void
wlc_phy_radio20693_adc_dac_setup(phy_info_t *pi, radio_20693_adc_modes_t adc_mode, uint8 core)
{
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (RADIOMAJORREV(pi) != 3) {
		wlc_phy_radio20693_afeclkpath_setup(pi, core, adc_mode, 0);
		wlc_phy_radio20693_adc_config_overrides(pi, adc_mode, core);
	}

	wlc_phy_radio20693_adc_setup(pi->u.pi_acphy->radioi, core, adc_mode);

	if (RADIOMAJORREV(pi) != 3)
		wlc_phy_radio20693_set_channel_bw(pi, adc_mode, core);

	if (RADIOMAJORREV(pi) != 3)
		wlc_phy_radio20693_config_bf_mode(pi, core);
}

static void
wlc_phy_radio20693_setup_crisscorss_ovr(phy_info_t *pi, uint8 core)
{
	uint8 pupd = 1;
	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	MOD_RADIO_REG_20693(pi, TX_TOP_5G_OVR2, core, ovr_tx5g_80p80_cas_pu, pupd);
	MOD_RADIO_REG_20693(pi, TX_TOP_5G_OVR2, core, ovr_tx5g_80p80_gm_pu, pupd);
	MOD_RADIO_REG_20693(pi, TX_TOP_2G_OVR1_EAST, core, ovr_tx2g_20p20_cas_pu, pupd);
	MOD_RADIO_REG_20693(pi, TX_TOP_2G_OVR1_EAST, core, ovr_tx2g_20p20_gm_pu, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_5G_OVR, core, ovr_rx5g_80p80_src_pu, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_5G_OVR, core, ovr_rx5g_80p80_des_pu, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_5G_OVR, core, ovr_rx5g_80p80_gc, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_2G_OVR_EAST, core, ovr_rx2g_20p20_src_pu, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_2G_OVR_EAST, core, ovr_rx2g_20p20_des_pu, pupd);
	MOD_RADIO_REG_20693(pi, RX_TOP_2G_OVR_EAST, core, ovr_rx2g_20p20_gc, pupd);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_radio20695_apply_adccal_result(phy_info_t *pi)
{
	uint8 temp_var[3];
	uint16 temp[3];

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG3, 0,
			rxadc_coeff_out_ctrl_adc_I, 0x3)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG4, 0,
			rxadc_coeff_out_ctrl_adc_Q, 0x3)
	ACPHY_REG_LIST_EXECUTE(pi);
	WRITE_RADIO_REG_28NM(pi, RF, AFE_CFG1_OVR1, 0, 0x0FFF);
	temp[0] = READ_RADIO_REG_20695(pi, RF, RXADC_CAP0_STAT, 0);
	temp[1] = READ_RADIO_REG_20695(pi, RF, RXADC_CAP1_STAT, 0);
	temp[2] = READ_RADIO_REG_20695(pi, RF, RXADC_CAP2_STAT, 0);

	temp_var[0] = (uint8)((temp[0] & 0xFE00) >> 9);
	temp_var[1] = (uint8)((temp[1] & 0xFE00) >> 9);
	temp_var[2] = (uint8)((temp[2] & 0xFE00) >> 9);

	MOD_RADIO_REG_20695_MULTI_3(pi, RF, RXADC_CFG5, 0, rxadc_coeff_cap0_adc_I, temp_var[0],
		rxadc_coeff_cap1_adc_I, temp_var[1], rxadc_coeff_cap2_adc_I, temp_var[2]);

	temp_var[0] = (uint8)(temp[0] & 0x7F);
	temp_var[1] = (uint8)(temp[1] & 0x7F);
	temp_var[2] = (uint8)(temp[2] & 0x7F);

	MOD_RADIO_REG_20695_MULTI_3(pi, RF, RXADC_CFG7, 0, rxadc_coeff_cap0_adc_Q, temp_var[0],
		rxadc_coeff_cap1_adc_Q, temp_var[1], rxadc_coeff_cap2_adc_Q, temp_var[2]);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
static void
wlc_phy_radio20691_afecal(phy_info_t *pi)
{
	uint8 core, bw;
	tiny_adc_tuning_array_t gvalues;
	phy_ac_radio_info_t *ri = pi->u.pi_acphy->radioi;
	uint8 phyrxchain;

	BCM_REFERENCE(phyrxchain);

	bw = CHSPEC_IS20(pi->radio_chanspec) ? 20 : CHSPEC_IS40(pi->radio_chanspec) ? 40 : 80;

	/* enable ?? wlc_phy_radio20691_rccal(pi); */
	if (CHSPEC_IS80(pi->radio_chanspec)) {
		/* set gvalues [20691_sigdel_fast_tune $def(radio_rccal_adc_gmult)] */
		wlc_tiny_sigdel_fast_tune(pi, ri->rccal_adc_gmult, &gvalues);
		/* 20691_adc_setup_fast $gvalues */
		wlc_tiny_adc_setup_fast(pi, &gvalues, 0);
	} else {
		/* set gvalues [20691_sigdel_slow1g2_tune $def(radio_rccal_adc_gmult)] */
		wlc_tiny_sigdel_slow_tune(pi, ri->rccal_adc_gmult, &gvalues, bw);
		/* 20691_adc_setup_slow1g2 $gvalues */
		wlc_tiny_adc_setup_slow(pi, &gvalues, bw, 0);
	}

	phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
	FOREACH_ACTV_CORE(pi, phyrxchain, core) {
		wlc_tiny_tia_config(pi, core);
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
static void
wlc_phy_radio20693_afecal(phy_info_t *pi)
{
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;
	uint8 core;
	radio_20693_adc_modes_t adc_mode;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM20693_ID));
	PHY_TRACE(("wl%d: %s\n", pi->sh->unit, __FUNCTION__));

	if (((phy_ac_chanmgr_get_data(pi_ac->chanmgri))->fast_adc_en == 1) ||
		(CHSPEC_IS80(pi->radio_chanspec)) || (CHSPEC_IS160(pi->radio_chanspec)) ||
		(CHSPEC_IS8080(pi->radio_chanspec))) {
		adc_mode = RADIO_20693_FAST_ADC;
	} else {
		adc_mode = RADIO_20693_SLOW_ADC;
	}

	FOREACH_CORE(pi, core) {
		wlc_phy_radio20693_adc_dac_setup(pi, adc_mode, core);
		wlc_tiny_tia_config(pi, core);
		if (RADIOMAJORREV(pi) != 3) {
			wlc_phy_radio20693_setup_crisscorss_ovr(pi, core);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
static void phy_ac_radio_verify_20694_afecal(phy_info_t *pi, uint16 invclk_sv[]);
static void phy_ac_radio_apply_20694_afecal(phy_info_t *pi, uint16 r[], uint16 invclk_sv[],
	uint8 adccapcal_timeout[]);

static void
phy_ac_radio_20694_afecal(phy_info_t *pi)
{
	uint8 core;
	uint16 invclk_sv[PHY_CORE_MAX];
	uint16 rr;
	uint sicoreunit = wlapi_si_coreunit(pi->sh->physhim);

	FOREACH_CORE(pi, core) {
		if ((sicoreunit != DUALMAC_AUX) || (core != 1)) {
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
					ovr_afediv_adc_en, 1);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core,
					afediv_adc_en, 1);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
					ovr_afediv_dac_en, 1);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_REG5, core,
					afediv_dac_en, 1);
		}
	}
	wlc_phy_resetcca_acphy(pi);

	FOREACH_CORE(pi, core) {
		invclk_sv[core] = READ_RADIO_REGFLD_20694(pi, RF, TXDAC_REG0, core, iqdac_invclko);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_invclko, 0);

		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_diode, 1);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_pwrup_diode, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_I, 1);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_pwrup_I, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_Q, 1);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_pwrup_Q, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdacbuf_pu, 1);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_buf_pu, 1);

		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc0_en, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc0_en, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_pu_I, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_puI, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_pu_Q, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_puQ, 1);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_power_mode, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_power_mode_enb, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_power_mode, 3);
		rr = READ_RADIO_REG_20694(pi, RF, RXADC_REG6, core);
		rr = rr | 0x00c0;
		WRITE_RADIO_REG_20694(pi, RF, RXADC_REG6, core, rr);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, afe_en_loopback, 1);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_buf_cmsel, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG3, core, rxadc_cal_cap, 7);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_buffer_per, 31);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_wait_per, 31);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG2, core, rxadc_sum_per, 127);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_sum_div, 5);
		//MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_I, 0);
		//MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc1_I, 0);
		//MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_Q, 0);
		//MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc1_Q, 0);

		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_bq2_adc, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_bq2_rc, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_dac_bq2, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_dac_rc, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_bq1_adc, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_bq1_bq2, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_aux_adc, 1);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_bq2_adc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_bq2_rc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_dac_bq2, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_dac_rc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_bq1_adc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_bq1_bq2, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_REG7, core, lpf_sw_aux_adc, 0);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_corr_dis, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_coeff_sel, 0);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG3, core, rxadc_coeff_out_ctrl_adc0_I, 3);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG4, core, rxadc_coeff_out_ctrl_adc0_Q, 3);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_I, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_Q, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc1_I, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc1_Q, 0);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_reset_n_adc0_I, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_reset_n_adc0_Q, 0);
	}
	OSL_DELAY(20);
	FOREACH_CORE(pi, core) {
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_reset_n_adc0_I, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_reset_n_adc0_Q, 1);
		OSL_DELAY(1);

		//trigger CAL
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_I, 1);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_Q, 1);
	}
	OSL_DELAY(10);

	phy_ac_radio_verify_20694_afecal(pi, invclk_sv);
}

static void
phy_ac_radio_verify_20694_afecal(phy_info_t *pi, uint16 invclk_sv[])
{
	uint8 core;
	uint16 r[PHY_CORE_MAX];
	uint8 adccapcal_timeout[PHY_CORE_MAX];
	uint8 repeat_cal[PHY_CORE_MAX];
	uint8 repeat_full_cal;

	FOREACH_CORE(pi, core) {
		adccapcal_timeout[core] = 20;
		r[core] = READ_RADIO_REG_20694(pi, RF, RXADC_CAL_STATUS, core) & 0x5;
		while ((adccapcal_timeout[core] > 10) && (r[core] != 0x5)) {
			OSL_DELAY(10);
			r[core] = READ_RADIO_REG_20694(pi, RF, RXADC_CAL_STATUS, core);
			adccapcal_timeout[core]--;
		}
	}
	repeat_full_cal = 0;
	FOREACH_CORE(pi, core) {
		repeat_cal[core] = 0;
		if (adccapcal_timeout[core] == 10 && (r[core] != 0x5)) {
			repeat_cal[core] = 1;
			repeat_full_cal = 1;
		}
	}
	while (repeat_full_cal == 1) {
		repeat_full_cal = 0;
		FOREACH_CORE(pi, core) {
			if (repeat_cal[core] == 1) {
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_start_cal_adc0_I, 0);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_start_cal_adc0_Q, 0);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_reset_n_adc0_I, 0);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_reset_n_adc0_Q, 0);
			}
		}
		OSL_DELAY(20);
		FOREACH_CORE(pi, core) {
			if (repeat_cal[core] == 1) {
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_reset_n_adc0_I, 1);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_reset_n_adc0_Q, 1);
				OSL_DELAY(1);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_start_cal_adc0_I, 1);
				MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core,
						rxadc_start_cal_adc0_Q, 1);
			}
		}
		OSL_DELAY(30);
		FOREACH_CORE(pi, core) {
			if (repeat_cal[core] == 1) {
				repeat_cal[core] = 0;
				adccapcal_timeout[core]--;
				r[core] = READ_RADIO_REG_20694(pi, RF, RXADC_CAL_STATUS, core)
					& 0x5;
				if (adccapcal_timeout[core] > 0 && (r[core] != 0x5)) {
					repeat_cal[core] = 1;
					repeat_full_cal = 1;
				}
			}
		}
	}

	phy_ac_radio_apply_20694_afecal(pi, r, invclk_sv, adccapcal_timeout);
}

static void
phy_ac_radio_apply_20694_afecal(phy_info_t *pi, uint16 r[], uint16 invclk_sv[],
	uint8 adccapcal_timeout[])
{
	uint8 core;
	uint16 rr;

	FOREACH_CORE(pi, core) {
		if (r[core] != 0x5) {
			PHY_ERROR(("ADCCAPCAL is not done properly on core %d\n", core));
		} else if (r[core] == 0x5 && adccapcal_timeout[core] >= 10) {
			PHY_INFORM(("ADCCAPCAL is good in first try on core %d\n", core));
		} else {
			PHY_INFORM(("ADCCAPCAL is good after %d extra try on core %d\n",
				10-adccapcal_timeout[core], core));
		}

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_I, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, rxadc_start_cal_adc0_Q, 0);

		//apply_adc_cal_result
		WRITE_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR1, core, 0x0FFF);

		rr = READ_RADIO_REG_20694(pi, RF, RXADC0_CAP0_STAT, core);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG6, core,
		rxadc_coeff_cap0_adc0_I, (rr>>8)&0xFF);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG9, core,
		rxadc_coeff_cap0_adc0_Q, rr&0xFF);

		rr = READ_RADIO_REG_20694(pi, RF, RXADC0_CAP1_STAT, core);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG5, core,
		rxadc_coeff_cap1_adc0_I, (rr>>8)&0xFF);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG8, core,
		rxadc_coeff_cap1_adc0_Q, rr&0xFF);

		rr = READ_RADIO_REG_20694(pi, RF, RXADC0_CAP2_STAT, core);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG5, core,
		rxadc_coeff_cap2_adc0_I, (rr>>8)&0xFF);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG8, core,
		rxadc_coeff_cap2_adc0_Q, rr&0xFF);

		WRITE_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR1, core, 0x0000);

		//MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_corr_dis, 0);
		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG1, core, rxadc_coeff_sel, 1);

		PHY_INFORM(("ADCCAPCAL: after write back, core%d: corr_dis %d coeff_sel %d\n", core,
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG1,
			core, rxadc_corr_dis),
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG1,
			core, rxadc_coeff_sel)));
		PHY_INFORM(("ADCCAPCAL: core%d: adc0: cap0 I: 0x%2x Q: 0x%2x\n", core,
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG6,
			core, rxadc_coeff_cap0_adc0_I),
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG9,
			core, rxadc_coeff_cap0_adc0_Q)));
		PHY_INFORM(("ADCCAPCAL: core%d: adc0: cap1 I: 0x%2x Q: 0x%2x\n", core,
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG5,
			core, rxadc_coeff_cap1_adc0_I),
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG8,
			core, rxadc_coeff_cap1_adc0_Q)));
		PHY_INFORM(("ADCCAPCAL: core%d: adc0: cap2 I: 0x%2x Q: 0x%2x\n", core,
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG5,
			core, rxadc_coeff_cap2_adc0_I),
				READ_RADIO_REGFLD_20694(pi, RF, RXADC_CFG8,
			core, rxadc_coeff_cap2_adc0_Q)));

		//clear out overrides
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_bq2_adc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_bq2_rc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_dac_bq2, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR1, core, ovr_lpf_sw_dac_rc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_bq1_adc, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_bq1_bq2, 0);
		MOD_RADIO_REG_20694(pi, RF, LPF_OVR2, core, ovr_lpf_sw_aux_adc, 0);

		MOD_RADIO_REG_20694(pi, RF, RXADC_CFG0, core, afe_en_loopback, 0);
		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_buf_cmsel, 0);

		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc0_en, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_pu_I, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_pu_Q, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_adc_power_mode, 0);

		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_diode, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_I, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdac_pu_Q, 0);
		MOD_RADIO_REG_20694(pi, RF, AFE_CFG1_OVR2, core, ovr_iqdacbuf_pu, 0);

		rr = READ_RADIO_REG_20694(pi, RF, RXADC_REG6, core);
		rr = rr & 0xff3f;
		WRITE_RADIO_REG_20694(pi, RF, RXADC_REG6, core, rr);

		MOD_RADIO_REG_20694(pi, RF, TXDAC_REG0, core, iqdac_invclko, invclk_sv[core]);
	}

	FOREACH_CORE(pi, core) {
		if ((wlapi_si_coreunit(pi->sh->physhim) != DUALMAC_AUX) || (core != 1)) {
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
					ovr_afediv_adc_en, 0);
			MOD_RADIO_REG_20694(pi, RF, AFEDIV_CFG1_OVR, core,
					ovr_afediv_dac_en, 0);
		}
	}
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
static void
wlc_phy_radio20695_afecal(phy_info_t *pi)
{
	uint16 adccapcal_timeout_I, adccapcal_timeout_Q, adccapcal_done;
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint16 save_forcetxgatedClksOn;

	/* Save AFE registers */
	phy_ac_reg_cache_save(pi_ac, RADIOREGS_AFECAL);

	if (CHSPEC_IS2G(pi->radio_chanspec)) {
		MOD_RADIO_REG_28NM(pi, RF, AFEDIV2G_CFG1_OVR, 0, ovr_afediv2g_dac_en, 1);
		MOD_RADIO_REG_28NM(pi, RF, AFEDIV2G_REG5, 0, afediv2g_dac_en, 1);
	} else {
		MOD_RADIO_REG_28NM(pi, RF, AFEDIV5G_CFG1_OVR, 0, ovr_afediv5g_dac_en, 1);
		MOD_RADIO_REG_28NM(pi, RF, AFEDIV5G_REG5, 0, afediv5g_dac_en, 1);
	}

	if (pi_ac->radioi->data->ulp_adc_mode == 2) {
		MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, rxadc_power_mode, 3);
	} else {
		MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, rxadc_power_mode, 0);
	}

	/* Force Tx Gated clocks on to ensure DAC input is valid */
	save_forcetxgatedClksOn = READ_PHYREGFLD(pi, fineclockgatecontrol, forcetxgatedClksOn);
	MOD_PHYREG(pi, fineclockgatecontrol, forcetxgatedClksOn, 1);

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_buf_cmsel, 1)

		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdacbuf_pu, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_diode, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_I, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_Q, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_pwrup_I, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_pwrup_Q, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_buf_pu, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_pwrup_diode, 1)

		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_pu_I, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_pu_Q, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_power_mode, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_puI, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_puQ, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_power_mode_enb, 0)

		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_bq2_adc, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_wbcal_adc, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_aux_adc, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_REG16, 0, tia_sw_bq2_adc, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_REG18, 0, tia_sw_wbcal_adc, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_REG16, 0, tia_sw_aux_adc, 0)

		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG1, 0, rxadc_buffer_per, 0x1f)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG1, 0, rxadc_wait_per, 0x1f)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG2, 0, rxadc_sum_per, 0x3f)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG1, 0, rxadc_sum_div, 4)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG3, 0, rxadc_cal_cap, 7)

		/* Change coeff sel to zero before cal */
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG1, 0, rxadc_coeff_sel, 0)

		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, afe_en_loopback, 0x1)
		/* Reset ADC cal engine	*/
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_reset_n_adc_I, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_reset_n_adc_I, 0)
		/* Start the calibration for I */
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_start_cal_adc_I, 1)
	ACPHY_REG_LIST_EXECUTE(pi);

	adccapcal_timeout_I = 20;
	adccapcal_done = READ_RADIO_REGFLD_28NM(pi, RF, RXADC_CAL_STATUS, 0, rxadc_cal_done_I);
	while ((adccapcal_done != 1) && (adccapcal_timeout_I > 0))
	{
		OSL_DELAY(10);
		adccapcal_done = READ_RADIO_REGFLD_28NM(pi, RF, RXADC_CAL_STATUS, 0,
			rxadc_cal_done_I);
		adccapcal_timeout_I = adccapcal_timeout_I -1;
	}

	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_start_cal_adc_I, 0)
		/* Reset ADC cal engine	*/
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_reset_n_adc_Q, 1)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_reset_n_adc_Q, 0)
		/* Start the calibration for Q */
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, RXADC_CFG0, 0, rxadc_start_cal_adc_Q, 1)
	ACPHY_REG_LIST_EXECUTE(pi);

	adccapcal_timeout_Q = 20;
	adccapcal_done = READ_RADIO_REGFLD_28NM(pi, RF, RXADC_CAL_STATUS, 0, rxadc_cal_done_Q);
	while ((adccapcal_done != 1) && (adccapcal_timeout_Q > 0))
	{
		OSL_DELAY(10);
		adccapcal_done = READ_RADIO_REGFLD_28NM(pi, RF, RXADC_CAL_STATUS, 0,
			rxadc_cal_done_Q);
		adccapcal_timeout_Q = adccapcal_timeout_Q -1;
	}
	MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, rxadc_start_cal_adc_Q, 0);

	if ((adccapcal_timeout_I > 0) && (adccapcal_timeout_Q > 0))
	{
		wlc_phy_radio20695_apply_adccal_result(pi);
	}

	MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG0, 0, afe_en_loopback, 0);
	ACPHY_REG_LIST_START
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_bq2_adc, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_wbcal_adc, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TIA_CFG2_OVR, 0, ovr_tia_sw_aux_adc, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_pu_I, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_pu_Q, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_adc_power_mode, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdacbuf_pu, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_diode, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_I, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, AFE_CFG1_OVR2, 0, ovr_iqdac_pu_Q, 0)
		MOD_RADIO_REG_28NM_ENTRY(pi, RF, TXDAC_REG0, 0, iqdac_buf_cmsel, 0)
	ACPHY_REG_LIST_EXECUTE(pi);

	/* Restore forced tx gated clock */
	MOD_PHYREG(pi, fineclockgatecontrol, forcetxgatedClksOn, save_forcetxgatedClksOn);

	/* Restore AFE registers */
	phy_ac_reg_cache_restore(pi_ac, RADIOREGS_AFECAL);

	/* Change coeff sel to one after cal (take bypass coeffs) */
	MOD_RADIO_REG_28NM(pi, RF, RXADC_CFG1, 0, rxadc_coeff_sel, 1);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */

#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
static void
wlc_phy_radio20696_afecal(phy_info_t *pi)
{
	uint8 core;
	phy_info_acphy_t *pi_ac = pi->u.pi_acphy;

	ASSERT(RADIOID(pi->pubpi->radioid) == BCM20696_ID);

	/* adc_cap calibration needs to be done once only at power up */
	if (CCT_INIT(pi_ac)) {
		FOREACH_CORE(pi, core) {
			wlc_phy_radio20696_adc_setup(pi, core);
		}
		wlc_phy_radio20696_adc_cap_cal(pi, ADC_CAP_CAL_MODE_PARALLEL);
	}
	/* Updaating adc_cap_cal from cache just in case radio was powered down */
	else if (CCT_BAND_CHG(pi_ac)) {
		FOREACH_CORE(pi, core) {
			wlc_phy_radio20696_apply_adc_cal_result_from_cache(pi, core);
		}
	}
}

static void
wlc_phy_radio20696_adc_cap_cal(phy_info_t *pi, int mode)
{
	phy_info_acphy_t *pi_ac = (phy_info_acphy_t *)pi->u.pi_acphy;
	uint8 core;
	uint16 adccapcal_Timeout;
	uint16 bbmult = 0;

	PHY_INFORM(("%s\n", __FUNCTION__));

	/* WAR: Force RFSeq is needed to get rid of time-outs and set_tx_bbmult
	 *      to get also the first time after a power cycle valid results
	 */
	wlc_phy_force_rfseq_acphy(pi, ACPHY_RFSEQ_RX2TX);
	FOREACH_CORE(pi, core) {
		wlc_phy_set_tx_bbmult_acphy(pi, &bbmult, core);
	}

	if (mode == ADC_CAP_CAL_MODE_BYPASS) {
		/* Bypass ADC cap calibration and use ideal calibration values */
		FOREACH_CORE(pi, core) {
			MOD_RADIO_REG_20696(pi, RXADC_CFG1, core, rxadc_corr_dis, 0x1);
		}
	} else {
		/* Save AFE registers */
		phy_ac_reg_cache_save(pi_ac, RADIOREGS_AFECAL);

		FOREACH_CORE(pi, core) {
			wlc_phy_radio20696_adc_cap_cal_setup(pi, core);
			if (mode == ADC_CAP_CAL_MODE_PARALLEL) {
				wlc_phy_radio20696_adc_cap_cal_parallel(pi, core,
					&adccapcal_Timeout);
			} else {
				wlc_phy_radio20696_adc_cap_cal_sequential(pi, core,
					&adccapcal_Timeout);
			}
			if (adccapcal_Timeout == 0) {
				PHY_ERROR(("%s: adc_cap_cal -- core%d - DISABLED correction, "
					"due to cal TIMEOUT\n", __FUNCTION__, core));
				MOD_RADIO_REG_20696(pi, RXADC_CFG1, core, rxadc_corr_dis, 0x1);
			} else {
				wlc_phy_radio20696_apply_adc_cal_result(pi, core);
				MOD_RADIO_REG_20696(pi, RXADC_CFG1, core, rxadc_corr_dis, 0x0);
			}
			/* Cleanup */
			MOD_RADIO_REG_20696(pi, RXADC_CFG1, core, rxadc_coeff_sel, 0x1);
			MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, afe_en_loopback, 0x0);
		}
		/* Restore AFE registers */
		phy_ac_reg_cache_restore(pi_ac, RADIOREGS_AFECAL);
	}
}

static void
wlc_phy_radio20696_adc_cap_cal_setup(phy_info_t *pi, uint8 core)
{
	uint16 val;

	/* Power up DAC and ADC by override */
	/* Set following AFE_CFG1_OVR2 bits:
	 * ovr_iqdac_reset       0x2000
	 * ovr_iqdac_buf_pu      0x0800
	 * ovr_iqdac_pwrup_diode 0x0400
	 * ovr_iqdac_pwrup_I     0x0040
	 * ovr_iqdac_pwrup_Q     0x0020
	 * ovr_rxadc_puI         0x0004
	 * ovr_rxadc_puQ         0x0002
	 * ovr_rxadc_power_mode  0x0001
	 */
	val = READ_RADIO_REG_20696(pi, AFE_CFG1_OVR2, core);
	val |= 0x2c67;
	WRITE_RADIO_REG_20696(pi, AFE_CFG1_OVR2, core, val);
	/* Set following TXDAC_REG0 bits:
	 * iqdac_pwrup_diode     0x0001
	 * iqdac_pwrup_I         0x0002
	 * iqdac_pwrup_Q         0x0004
	 * iqdac_buf_pu          0x0008
	 * iqdac_reset           0x0010
	 * iqdac_buf_cmsel       0x0400
	 */
	val = READ_RADIO_REG_20696(pi, TXDAC_REG0, core);
	val |= 0x041f;
	WRITE_RADIO_REG_20696(pi, TXDAC_REG0, core, val);

	/* Set following RXADC_CFG0 bits:
	 * rxadc_puI             0x0004
	 * rxadc_puQ             0x0008
	 * rxadc_power_mode      0x0300
	 * afe_en_loopback       0x8000
	 * Clear following RXADC_CFG0 bits:
	 * rxadc_power_mode_enb  0x0400
	 */
	val = READ_RADIO_REG_20696(pi, RXADC_CFG0, core);
	val |= 0x830c;
	val &= 0xfbff;
	WRITE_RADIO_REG_20696(pi, RXADC_CFG0, core, val);

	/* Disconnect ADC from others */
	/* Set following LPF_OVR1 bits:
	 * ovr_lpf_sw_bq2_adc    0x0400
	 * ovr_lpf_sw_bq2_rc     0x0800
	 * ovr_lpf_sw_dac_bq2    0x1000
	 * ovr_lpf_sw_dac_rc     0x2000
	 */
	val = READ_RADIO_REG_20696(pi, LPF_OVR1, core);
	val |= 0x3c00;
	WRITE_RADIO_REG_20696(pi, LPF_OVR1, core, val);
	/* Set following LPF_OVR2 bits:
	 * ovr_lpf_sw_bq1_adc    0x0001
	 * ovr_lpf_sw_bq1_bq2    0x0002
	 * ovr_lpf_sw_aux_adc    0x0004
	 */
	val = READ_RADIO_REG_20696(pi, LPF_OVR2, core);
	val |= 0x0007;
	WRITE_RADIO_REG_20696(pi, LPF_OVR2, core, val);
	/* Clear following LPF_REG7 bits:
	 * lpf_sw_bq1_bq2        0x0004
	 * lpf_sw_bq2_adc        0x0080
	 * lpf_sw_bq1_adc        0x0008
	 * lpf_sw_dac_bq2        0x0020
	 * lpf_sw_bq2_rc         0x0040
	 * lpf_sw_dac_rc         0x0010
	 * lpf_sw_aux_adc        0x0002
	 */
	val = READ_RADIO_REG_20696(pi, LPF_REG7, core);
	val &= 0xff01;
	WRITE_RADIO_REG_20696(pi, LPF_REG7, core, val);
}

static void
wlc_phy_radio20696_adc_cap_cal_parallel(phy_info_t *pi, uint8 core, uint16 *timeout)
{
	uint8 normal = 1;
	uint16 adccapcal_Timeout;
	uint16 adccapcal_done;

	if (normal) {
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_start_cal_adc_I, 0x0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_start_cal_adc_Q, 0x0);

		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_reset_n_adc_I, 0x0);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_reset_n_adc_Q, 0x0);
		OSL_DELAY(100);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_reset_n_adc_I, 0x1);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_reset_n_adc_Q, 0x1);
		OSL_DELAY(100);
		MOD_RADIO_REG_20696(pi, RXADC_CFG3, core,
			rxadc_coeff_out_ctrl_adc_I, 0x3);
		MOD_RADIO_REG_20696(pi, RXADC_CFG3, core,
			rxadc_coeff_out_ctrl_adc_Q, 0x3);
	} else {
		WRITE_RADIO_REG_20696(pi, RXADC_CFG1, core, 0x53ff);
		/* Reset ADC cal engine */
		WRITE_RADIO_REG_20696(pi, RXADC_CFG0, core, 0x830d);
	}
	/* Trigger CAL */
	if (normal) {
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_start_cal_adc_I, 0x1);
		MOD_RADIO_REG_20696(pi, RXADC_CFG0, core,
			rxadc_start_cal_adc_Q, 0x1);
	} else {
		/* Reset ADC cal engine */
		WRITE_RADIO_REG_20696(pi, RXADC_CFG0, core, 0xfbfd);
	}

	/* 1000 * 10 = 10ms max time to wait */
	adccapcal_Timeout = 100;

	/* Test both I and Q rail done bit */
	adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAL_STATUS,
		core, o_rxadc_cal_done_I);
	adccapcal_done &= READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAL_STATUS,
		core, o_rxadc_cal_done_Q);
	while ((adccapcal_done != 1) && (adccapcal_Timeout > 0)) {
		OSL_DELAY(10);
		adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF,
			RXADC_CAL_STATUS, core, o_rxadc_cal_done_I);
		adccapcal_done &= READ_RADIO_REGFLD_20696(pi, RF,
			RXADC_CAL_STATUS, core, o_rxadc_cal_done_Q);
		adccapcal_Timeout--;
	}
	if (adccapcal_Timeout == 0) {
		PHY_ERROR(("%s: ADC cap calibration TIMEOUT\n",
			__FUNCTION__));
	}
	*timeout = adccapcal_Timeout;
}

static void
wlc_phy_radio20696_adc_cap_cal_sequential(phy_info_t *pi, uint8 core, uint16 *timeout)
{
	uint16 adccapcal_Timeout;
	uint16 adccapcal_done;

	/* Sequential, one path at a time.
	 * Reset ADC cal engine.
	 */
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I,
		0x0);
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_I,
		0x1);
	/* Start the calibration process for I */
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_start_cal_adc_I,
		0x1);
	/* 100 * 10 = 1ms max time to wait */
	adccapcal_Timeout = 100;
	adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAL_STATUS,
		core, o_rxadc_cal_done_I);
	while ((adccapcal_done != 1) && (adccapcal_Timeout > 0)) {
		OSL_DELAY(10);
		adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF,
			RXADC_CAL_STATUS, core, o_rxadc_cal_done_I);
	        adccapcal_Timeout--;
	}
	if (adccapcal_Timeout == 0) {
		PHY_ERROR(("%s: ADC cap calibration TIMEOUT for Core%d "
			"ADC I\n", __FUNCTION__, core));
	}

	/* Reset ADC cal engine	*/
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q,
		0x0);
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_reset_n_adc_Q,
		0x1);
	/* Start the calibration process for Q */
	MOD_RADIO_REG_20696(pi, RXADC_CFG0, core, rxadc_start_cal_adc_Q,
		0x1);
	/* 100 * 10 = 1ms max time to wait */
	adccapcal_Timeout = 100;
	adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAL_STATUS,
		core, o_rxadc_cal_done_Q);
	while ((adccapcal_done != 1) && (adccapcal_Timeout > 0)) {
		OSL_DELAY(10);
		adccapcal_done = READ_RADIO_REGFLD_20696(pi, RF,
			RXADC_CAL_STATUS, core, o_rxadc_cal_done_Q);
	        adccapcal_Timeout--;
	}
	if (adccapcal_Timeout == 0) {
		PHY_ERROR(("%s: ADC cap calibration TIMEOUT for Core%d ADC"
		" Q\n", __FUNCTION__, core));
	}
	*timeout = adccapcal_Timeout;
}


static void
wlc_phy_radio20696_apply_adc_cal_result(phy_info_t *pi, uint8 core)
{
	uint16 value;

	WRITE_RADIO_REG_20696(pi, AFE_CFG1_OVR1, core, 0x0FFF);

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP0_STAT, core, o_coeff_cap0_adc_I);
	MOD_RADIO_REG_20696(pi, RXADC_CFG6, core, rxadc_coeff_cap0_adc_I, value);
	rxadccal_coeffs_cache[core].cap0_i = value;
	PHY_INFORM(("core%d: cap0 %3d ", core, ((int16)(value << 10)) >> 10));

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP0_STAT, core, o_coeff_cap0_adc_Q);
	MOD_RADIO_REG_20696(pi, RXADC_CFG9, core, rxadc_coeff_cap0_adc_Q, value);
	rxadccal_coeffs_cache[core].cap0_q = value;
	PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP1_STAT, core, o_coeff_cap1_adc_I);
	MOD_RADIO_REG_20696(pi, RXADC_CFG5, core, rxadc_coeff_cap1_adc_I, value);
	rxadccal_coeffs_cache[core].cap12_i = value << 8;
	PHY_INFORM(("core%d: cap1 %3d ", core, ((int16)(value << 10)) >> 10));

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP1_STAT, core, o_coeff_cap1_adc_Q);
	MOD_RADIO_REG_20696(pi, RXADC_CFG8, core, rxadc_coeff_cap1_adc_Q, value);
	rxadccal_coeffs_cache[core].cap12_q = value << 8;
	PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP2_STAT, core, o_coeff_cap2_adc_I);
	MOD_RADIO_REG_20696(pi, RXADC_CFG5, core, rxadc_coeff_cap2_adc_I, value);
	rxadccal_coeffs_cache[core].cap12_i = rxadccal_coeffs_cache[core].cap12_i | value;
	PHY_INFORM(("core%d: cap2 %3d ", core, ((int16)(value << 10)) >> 10));

	value = READ_RADIO_REGFLD_20696(pi, RF, RXADC_CAP2_STAT, core, o_coeff_cap2_adc_Q);
	MOD_RADIO_REG_20696(pi, RXADC_CFG8, core, rxadc_coeff_cap2_adc_Q, value);
	rxadccal_coeffs_cache[core].cap12_q = rxadccal_coeffs_cache[core].cap12_q | value;
	PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));
}


static void
wlc_phy_radio20696_apply_adc_cal_result_from_cache(phy_info_t *pi, uint8 core)
{
		uint16 value;

		WRITE_RADIO_REG_20696(pi, AFE_CFG1_OVR1, core, 0x0FFF);

		value = rxadccal_coeffs_cache[core].cap0_i;
		MOD_RADIO_REG_20696(pi, RXADC_CFG6, core, rxadc_coeff_cap0_adc_I, value);
		PHY_INFORM(("core%d: cache cap0 %3d ", core, ((int16)(value << 10)) >> 10));

		value = rxadccal_coeffs_cache[core].cap0_q;
		MOD_RADIO_REG_20696(pi, RXADC_CFG9, core, rxadc_coeff_cap0_adc_Q, value);
		PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));

		value = rxadccal_coeffs_cache[core].cap12_i >> 8;
		MOD_RADIO_REG_20696(pi, RXADC_CFG5, core, rxadc_coeff_cap1_adc_I, value);
		PHY_INFORM(("core%d: cache cap1 %3d ", core, ((int16)(value << 10)) >> 10));

		value = rxadccal_coeffs_cache[core].cap12_q >> 8;
		MOD_RADIO_REG_20696(pi, RXADC_CFG8, core, rxadc_coeff_cap1_adc_Q, value);
		PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));

		value = rxadccal_coeffs_cache[core].cap12_i & 0xff;
		MOD_RADIO_REG_20696(pi, RXADC_CFG5, core, rxadc_coeff_cap2_adc_I, value);
		PHY_INFORM(("core%d: cache cap2 %3d ", core, ((int16)(value << 10)) >> 10));

		value = rxadccal_coeffs_cache[core].cap12_q & 0xff;
		MOD_RADIO_REG_20696(pi, RXADC_CFG8, core, rxadc_coeff_cap2_adc_Q, value);
		PHY_INFORM(("%3d\n", ((int16)(value << 10)) >> 10));
}

void
wlc_phy_radio20696_afe_div_ratio(phy_info_t *pi, uint8 calmode)
{
	uint16 nad;
	uint16 nda;
	uint8 core;
	uint16 overrides;

	if (CHSPEC_IS160(pi->radio_chanspec)) {
		PHY_ERROR(("%s: 160MHz bandwidth is not supported on this device\n",
			__FUNCTION__));
		nda = 4;
		nad = 8;
	} else if (CHSPEC_IS80(pi->radio_chanspec)) {
		nda = 4;
		nad = 8;
	} else if (CHSPEC_IS40(pi->radio_chanspec)) {
		nda = 6;
		nad = 12;
	} else {
		nda = 12;
		nad = 24;
	}

	FOREACH_CORE(pi, core) {
		/* Set AFEDIV overrides:
			ovr_afediv_adc_div1
			ovr_afediv_adc_div2
			ovr_afediv_dac_div1
			ovr_afediv_dac_div2
		*/
		overrides = READ_RADIO_REG_20696(pi, AFEDIV_CFG1_OVR, core);
		if (calmode == 1) {
			overrides |= 0x0078;
		} else {
			overrides &= ~0x0078;
		}
		WRITE_RADIO_REG_20696(pi, AFEDIV_CFG1_OVR, core, overrides);
	}
	/* Set div2 to divide by two and div1 to nda/nad per phybw selected above */
	WRITE_RADIO_ALLREG_20696(pi, AFEDIV_REG1, (nda << 10) | (0x2 << 3) | (0x1));
	WRITE_RADIO_ALLREG_20696(pi, AFEDIV_REG2, (nad << 10) | (0x2 << 3) | (0x1));
}

static void
wlc_phy_radio2069_afecal_invert(phy_info_t *pi)
{
	uint8 core;
	uint16 calcode;
	uint8 phyrxchain;

	BCM_REFERENCE(phyrxchain);

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	/* Switch on the clk */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 1);

	/* Output calCode = 1:14, latched = 15:28 */

	phyrxchain = phy_stf_get_data(pi->stfi)->phyrxchain;
	FOREACH_ACTV_CORE(pi, phyrxchain, core) {
		/* Use calCodes 1:14 instead of 15:28 */
		MOD_RADIO_REGC(pi, OVR3, core, ovr_afe_iqadc_flash_calcode_Ich, 1);
		MOD_RADIO_REGC(pi, OVR3, core, ovr_afe_iqadc_flash_calcode_Qch, 1);

		/* Invert the CalCodes */
		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE28, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE14(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE27, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE13(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE26, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE12(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE25, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE11(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE24, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE10(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE23, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE9(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE22, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE8(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE21, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE7(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE20, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE6(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE19, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE5(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE18, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE4(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE17, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE3(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE16, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE2(core), ~calcode & 0xffff);

		calcode = READ_RADIO_REGC(pi, RF, ADC_CALCODE15, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CALCODE1(core), ~calcode & 0xffff);
	}

	/* Turn off the clk */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 0);
}
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */

#define MAX_2069_AFECAL_WAITLOOPS 10
#define SAVE_RESTORE_AFE_CFG_REGS 3
/* 3 registers per AFE core: RfctrlCoreAfeCfg1, RfctrlCoreAfeCfg2, RfctrlOverrideAfeCfg */
static void
wlc_phy_radio2069_afecal(phy_info_t *pi)
{
	uint8 core, done_i, done_q;
	uint16 adc_cfg4, afe_cfg_arr[SAVE_RESTORE_AFE_CFG_REGS*PHY_CORE_MAX] = {0};
	phy_stf_data_t *stf_shdata = phy_stf_get_data(pi->stfi);

	BCM_REFERENCE(stf_shdata);

	if (ISSIM_ENAB(pi->sh->sih))
		return;

	ASSERT(RADIOID_IS(pi->pubpi->radioid, BCM2069_ID));

	/* Used to latch (clk register) rcal, rccal, ADC cal code */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 1);

	/* Save config registers and issue reset */
	FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, core) {
		/* Cfg1  in 0 to PHYCORENUM-1 */
		afe_cfg_arr[core] = READ_PHYREGCE(pi, RfctrlCoreAfeCfg1, core);
		/* Cfg2 in PHYCORENUM to 2*PHYCORENUM -1 */
		afe_cfg_arr[core + PHYCORENUM(pi->pubpi->phy_corenum)] =
		  READ_PHYREGCE(pi, RfctrlCoreAfeCfg2, core);
		/* Overrides in 2*PHYCORENUM to 3*PHYCORENUM - 1 */
		afe_cfg_arr[core + 2*PHYCORENUM(pi->pubpi->phy_corenum)] =
		  READ_PHYREGCE(pi, RfctrlOverrideAfeCfg, core);
		MOD_PHYREGCE(pi, RfctrlCoreAfeCfg1, core, afe_iqadc_reset, 1);
		MOD_PHYREGCE(pi, RfctrlOverrideAfeCfg, core, afe_iqadc_reset, 1);
		MOD_RADIO_REGC(pi, ADC_CFG3, core, flash_calrstb, 0); /* reset */
	}

	OSL_DELAY(100);

	/* Bring each AFE core back from reset and perform cal */
	FOREACH_ACTV_CORE(pi, stf_shdata->phyrxchain, core) {
		MOD_RADIO_REGC(pi, ADC_CFG3, core, flash_calrstb, 1);
		adc_cfg4 = READ_RADIO_REGC(pi, RF, ADC_CFG4, core);
		phy_utils_write_radioreg(pi, RF_2069_ADC_CFG4(core), adc_cfg4 | 0xf);


		SPINWAIT((READ_RADIO_REGFLDC(pi, RF_2069_ADC_STATUS(core),
			ADC_STATUS, i_wrf_jtag_afe_iqadc_Ich_cal_state) &&
			READ_RADIO_REGFLDC(pi, RF_2069_ADC_STATUS(core), ADC_STATUS,
				i_wrf_jtag_afe_iqadc_Qch_cal_state)) == 0,
				ACPHY_SPINWAIT_AFE_CAL_STATUS);
		done_i = READ_RADIO_REGFLDC(pi, RF_2069_ADC_STATUS(core), ADC_STATUS,
			i_wrf_jtag_afe_iqadc_Ich_cal_state);
		done_q = READ_RADIO_REGFLDC(pi, RF_2069_ADC_STATUS(core), ADC_STATUS,
			i_wrf_jtag_afe_iqadc_Qch_cal_state);
		/* Don't assert for QT */
		if (!ISSIM_ENAB(pi->sh->sih)) {
			if (done_i == 0 || done_q == 0) {
				PHY_FATAL_ERROR_MESG((" %s: SPINWAIT ERROR:", __FUNCTION__));
				PHY_FATAL_ERROR_MESG(("AFECAL FAILED on Core %d\n", core));
				PHY_FATAL_ERROR(pi, PHY_RC_AFE_CAL_FAILED);
			}
		}

		/* calMode = 0 */
		phy_utils_write_radioreg(pi, RF_2069_ADC_CFG4(core), (adc_cfg4 & 0xfff0));
		/* Restore AFE config registers for that core with saved values */
		WRITE_PHYREGCE(pi, RfctrlCoreAfeCfg1, core, afe_cfg_arr[core]);
		WRITE_PHYREGCE(pi, RfctrlCoreAfeCfg2, core,
			afe_cfg_arr[core + PHYCORENUM(pi->pubpi->phy_corenum)]);
		WRITE_PHYREGCE(pi, RfctrlOverrideAfeCfg, core,
			afe_cfg_arr[core + 2 * PHYCORENUM(pi->pubpi->phy_corenum)]);
	}

	/* Turn off clock */
	MOD_RADIO_REG(pi, RFP, PLL_XTAL2, xtal_pu_RCCAL, 0);
	if (RADIO2069REV(pi->pubpi->radiorev) < 4) {
	  /* JIRA (CRDOT11ACPHY-153) calCodes are inverted for 4360a0 */
	  wlc_phy_radio2069_afecal_invert(pi);
	}
}

void
wlc_phy_radio_afecal(phy_info_t *pi)
{
	wlc_phy_resetcca_acphy(pi);
	OSL_DELAY(1);
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20691_ID)) {
		wlc_phy_radio20691_afecal(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20691)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20693_ID)) {
		wlc_phy_radio20693_afecal(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20693)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20694_ID)) {
		phy_ac_radio_20694_afecal(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20694)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20695_ID)) {
		wlc_phy_radio20695_afecal(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20695)) */
#if !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696))
	if (RADIOID_IS(pi->pubpi->radioid, BCM20696_ID)) {
		wlc_phy_radio20696_afecal(pi);
	} else
#endif /* !defined(RADIO_ID)  || (defined(RADIO_ID) && defined(RADIO_BCM20696)) */
	{
		wlc_phy_radio2069_afecal(pi);
	}

}

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
static void
wlc_phy_radio2069_4335C0_vco_opt(phy_info_t *pi, uint8 vco_mode)
{
	uint16 temp_reg;

	if (vco_mode == ACPHY_VCO_2P5V) {
		ACPHY_REG_LIST_START
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR27, 0xfff8)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO8, rfpll_vco_vctrl_buf_ical, 0x0)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO6, rfpll_vco_bypass_vctrl_buf, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO3, rfpll_vco_cvar_extra, 0xa)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO2, rfpll_vco_cvar, 0xf)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO6, rfpll_vco_bias_mode, 0x0)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO6, rfpll_vco_ALC_ref_ctrl, 0x0)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_CP4, rfpll_cp_kpd_scale, 0x21)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_HVLDO2, ldo_2p5_lowquiescenten_VCO, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_HVLDO2, ldo_2p5_lowquiescenten_CP, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_CP, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_VCO, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);

		temp_reg = phy_utils_read_radioreg(pi, RFP_2069_GE16_PLL_CFG3);
		temp_reg |= (0x3 << 10);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_PLL_CFG3, temp_reg);

		temp_reg = phy_utils_read_radioreg(pi, RFP_2069_GE16_TOP_SPARE3);
		temp_reg |= (0x3 << 11);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_TOP_SPARE3, temp_reg);

	} else if (vco_mode == ACPHY_VCO_1P35V) {
		ACPHY_REG_LIST_START
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_OVR27, 0xfff8)

			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO3, rfpll_vco_cvar_extra, 0xf)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO2, rfpll_vco_cvar, 0xf)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO6, rfpll_vco_bias_mode, 0x0)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_VCO6, rfpll_vco_ALC_ref_ctrl, 0x0)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_CP4, rfpll_cp_kpd_scale, 0x21)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_XTALLDO1,
				ldo_1p2_xtalldo1p2_lowquiescenten, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_HVLDO2, ldo_2p5_lowquiescenten_VCO, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, PLL_HVLDO2, ldo_2p5_lowquiescenten_CP, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_CP, 0x1)
			MOD_RADIO_REG_ENTRY(pi, RFP, GE16_PLL_HVLDO4, ldo_2p5_static_load_VCO, 0x1)
		ACPHY_REG_LIST_EXECUTE(pi);

		temp_reg = phy_utils_read_radioreg(pi, RFP_2069_GE16_PLL_CFG4);
		temp_reg |= (0x3 << 12);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_PLL_CFG4, temp_reg);

		MOD_RADIO_REG(pi, RFP, PLL_VCO2, rfpll_vco_USE_2p5V, 0x0);

		temp_reg = phy_utils_read_radioreg(pi, RFP_2069_GE16_PLL_CFG3);
		temp_reg |= (0x3 << 10);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_PLL_CFG3, temp_reg);

		temp_reg = phy_utils_read_radioreg(pi, RFP_2069_GE16_TOP_SPARE3);
		temp_reg |= (0x3 << 11);
		phy_utils_write_radioreg(pi, RFP_2069_GE16_TOP_SPARE3, temp_reg);

		ACPHY_REG_LIST_START
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_PLL_LF2, 0x5555)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_PLL_LF3, 0x5555)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_PLL_LF4, 0xe)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_PLL_LF5, 0xe)
			WRITE_RADIO_REG_ENTRY(pi, RFP_2069_GE16_PLL_LF7, 0x1085)
		ACPHY_REG_LIST_EXECUTE(pi);
	}

}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */

#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
static void
wlc_phy_radio20691_4345_vco_opt(phy_info_t *pi, uint8 vco_mode)
{

	if (vco_mode == ACPHY_VCO_2P5V) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO3, 0, rfpll_vco_cvar_extra, 0xa)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO2, 0, rfpll_vco_cvar, 0xf)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO6, 0, rfpll_vco_bias_mode, 0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO6, 0, rfpll_vco_ALC_ref_ctrl, 0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_CP4, 0, rfpll_cp_kpd_scale, 0x21)
		ACPHY_REG_LIST_EXECUTE(pi);
			/* For 4364 1x1 core, this LP mode setting causes a delay */
			/* in VCO cal convergence when switching from 2G to 5G */
			if (CHIPID(pi->sh->chip) != BCM4364_CHIP_ID) {
				MOD_RADIO_REG_20691(pi, PLL_XTALLDO1, 0,
					ldo_1p2_xtalldo1p2_lowquiescenten, 0x1);
			}
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO2, 0,
				ldo_2p5_lowquiescenten_VCO, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO2, 0, ldo_2p5_lowquiescenten_CP, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO4, 0, ldo_2p5_static_load_CP, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO4, 0, ldo_2p5_static_load_VCO, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG3, 0, rfpll_spare1, 0x3)
		ACPHY_REG_LIST_EXECUTE(pi);
	} else if (vco_mode == ACPHY_VCO_1P35V) {
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO3, 0, rfpll_vco_cvar_extra, 0xf)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO2, 0, rfpll_vco_cvar, 0xf)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO6, 0, rfpll_vco_bias_mode, 0x0)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_VCO6, 0, rfpll_vco_ALC_ref_ctrl, 0x3)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_CP4, 0, rfpll_cp_kpd_scale, 0x21)
		ACPHY_REG_LIST_EXECUTE(pi);
			/* For 4364 1x1 core, this LP mode setting causes a delay */
			/* in VCO cal convergence when switching from 2G to 5G */
			if (CHIPID(pi->sh->chip) != BCM4364_CHIP_ID) {
				MOD_RADIO_REG_20691(pi, PLL_XTALLDO1, 0,
					ldo_1p2_xtalldo1p2_lowquiescenten, 0x1);
			}
		ACPHY_REG_LIST_START
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO2, 0,
				ldo_2p5_lowquiescenten_VCO, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO2, 0, ldo_2p5_lowquiescenten_CP, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO4, 0, ldo_2p5_static_load_CP, 0x1)
			MOD_RADIO_REG_20691_ENTRY(pi, PLL_HVLDO4, 0, ldo_2p5_static_load_VCO, 0x1)

			MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG3, 0, rfpll_spare0, 0xb4)

			MOD_RADIO_REG_20691_ENTRY(pi, PLL_CFG3, 0, rfpll_spare1, 0x3)
		ACPHY_REG_LIST_EXECUTE(pi);
	}
}
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */

void
wlc_phy_radio_vco_opt(phy_info_t *pi, uint8 vco_mode)
{
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1))
	if (ACMAJORREV_1(pi->pubpi->phy_rev) && ACMINORREV_2(pi)) {
		wlc_phy_radio2069_4335C0_vco_opt(pi, vco_mode);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_1)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3))
	if (ACMAJORREV_3(pi->pubpi->phy_rev)) {
		wlc_phy_radio20691_4345_vco_opt(pi, vco_mode);
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_3)) */
#if !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4))
	if (ACMAJORREV_4(pi->pubpi->phy_rev)) {
	} else
#endif /* !defined(PHY_VER)  || (defined(PHY_VER) && defined(PHY_ACMAJORREV_4)) */
	{
	}
}

#ifdef RADIO_HEALTH_CHECK
static bool
phy_ac_radio_check_pll_lock(phy_type_radio_ctx_t *ctx)
{
	phy_ac_radio_info_t *info = (phy_ac_radio_info_t *)ctx;
	phy_info_t *pi = info->pi;
	bool ret = TRUE;
	int8 need_refresh = 0;
	wlapi_suspend_mac_and_wait(pi->sh->physhim);
	if (RADIOID(pi->pubpi->radioid) == BCM20691_ID)
		need_refresh = READ_RADIO_REGFLD_20691(pi, PLL_DSPR27, 0,
				rfpll_monitor_need_refresh);
	else if ((RADIOID(pi->pubpi->radioid) == BCM20693_ID) ||
		(RADIOID(pi->pubpi->radioid) == BCM2069_ID))
		need_refresh = READ_RADIO_REGFLD(pi, RFP, PLL_DSPR27, rfpll_monitor_need_refresh);
	wlapi_enable_mac(pi->sh->physhim);
	if (need_refresh == 1)
		ret = FALSE;
	return (ret);
}
#endif /* RADIO_HEALTH_CHECK */

#if defined(WLTEST)
int
phy_ac_radio_set_pd(phy_ac_radio_info_t *radioi, uint16 int_val)
{
	if (CHIPID(radioi->pi->sh->chip) == BCM4335_CHIP_ID) {
		if (int_val == 1) {
			radioi->data->acphy_4335_radio_pd_status = 1;
			wlc_phy_radio2069_pwrdwn_seq(radioi);
		} else if (int_val == 0) {
			wlc_phy_radio2069_pwrup_seq(radioi);
			radioi->data->acphy_4335_radio_pd_status = 0;
		} else {
			PHY_ERROR(("RADIO PD %d is not supported \n", int_val));
		}
	} else {
		PHY_ERROR(("RADIO PD is not supported for this chip \n"));
		return BCME_UNSUPPORTED;
	}
	return BCME_OK;
}
#endif /* WLTEST */
