/*
 * MBO implementation for
 * Broadcom 802.11bang Networking Device Driver
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 * $Id$
 *
 * <<Broadcom-WL-IPTag/Proprietary:>>
 */

/**
 * @file
 * @brief
 * This file implements a part of WFA MBO features
 */
#ifdef STA
#include <wlc_cfg.h>
#include <typedefs.h>
#include <osl.h>
#include <wl_dbg.h>
#include <wlioctl.h>
#include <wlc_bsscfg.h>

#include <wlc_pub.h>
#include <wlc.h>
#include <wlc_types.h>
#include <wlc_ie_mgmt_types.h>
#include <wlc_ie_mgmt.h>
#include <wlc_ie_mgmt_vs.h>
#include <proto/mbo.h>
#include <wlc_mbo.h>
#include <wlc_wnm.h>
#include <bcmiov.h>
#include <wlc_ie_mgmt_ft.h>

#define WL_MBO_ERR(X) WL_MBO_DBG(X)

/* macro to control alloc and dealloc for sta only entity */
#define MBO_BSSCFG_STA(cfg) (BSSCFG_INFRA_STA(cfg) && ((cfg)->type == BSSCFG_TYPE_GENERIC))

/* 255 - ( OUI + OUI Type ) */
#define MBO_ELEM_MAX_ATTRS_LEN 251
/* 255 - ( OUI + OUI Type ) - sizeof(wifi_mbo_cell_data_cap_attr_t) */
#define MBO_MAX_NON_PREF_CHAN_ATTRS_LEN 248
#define MBO_MAX_NON_PREF_CHAN_SE_LEN 248
#define MAX_NON_PREF_CHAN_BODY_LEN  32
#define MBO_MAX_CELL_DATA_CAP_SE_LEN 7

/* flag handling macros */
#define MBO_FLAG_BIT_SET(flags, bit) ((flags) |= (bit))
#define MBO_FLAG_IS_BIT_SET(flags, bit) ((flags) & (bit))
#define MBO_FLAG_BIT_RESET(flags, bit) ((flags) &= ~(bit))

typedef struct np_chan_entry {
	uint8 chan;
	uint8 ref_cnt;
} np_chan_entry_t;

typedef struct mbo_chan_pref_list mbo_chan_pref_list_t;
struct mbo_chan_pref_list {
	mbo_chan_pref_list_t *next;
	uint8 opclass;
	uint8 chan;
	uint8	pref;
	uint8 reason;
};

typedef struct wlc_mbo_data wlc_mbo_data_t;
struct wlc_mbo_data {
	/* configured cellular data capability of device */
	uint8 cell_data_cap;
	uint8 max_chan_pref_entries;
};

struct wlc_mbo_info {
	wlc_info_t *wlc;
	/* shared data */
	wlc_mbo_data_t *mbo_data;
	int      cfgh;    /* mbo bsscfg cubby handle */
	bcm_iov_parse_context_t *iov_parse_ctx;
};

/* NOTE: This whole struct is being cloned across with its contents and
 * original memeory allocation intact.
 */
typedef struct wlc_mbo_bsscfg_cubby {
	/* counters */
	wl_mbo_counters_t *cntrs;
	/* flags for associated bss capability etc. */
	uint32 flags;
	/* configured non pref chan list for this bss */
	mbo_chan_pref_list_t *chan_pref_list_head;
	uint8 *np_chan_attr_buf;
	uint16 np_chan_attr_buf_len;
	/* bss transition reject reason */
	uint8 bsstrans_reject_reason;
} wlc_mbo_bsscfg_cubby_t;

#define MBO_BSSCFG_CUBBY_LOC(mbo, cfg) ((wlc_mbo_bsscfg_cubby_t **)BSSCFG_CUBBY(cfg, (mbo)->cfgh))
#define MBO_BSSCFG_CUBBY(mbo, cfg) (*MBO_BSSCFG_CUBBY_LOC(mbo, cfg))
#define MBO_CUBBY_CFG_SIZE  sizeof(wlc_mbo_bsscfg_cubby_t)

#define SUBCMD_TBL_SZ(_cmd_tbl)  (sizeof(_cmd_tbl)/sizeof(*_cmd_tbl))

static const bcm_iovar_t mbo_iovars[] = {
	{"mbo", 0, 0, 0, IOVT_BUFFER, 0},
	{NULL, 0, 0, 0, 0, 0}
};

static void wlc_mbo_watchdog(void *ctx);
static int wlc_mbo_wlc_up(void *ctx);
static int wlc_mbo_wlc_down(void *ctx);
static int wlc_mbo_bsscfg_init(void *ctx, wlc_bsscfg_t *cfg);
static void wlc_mbo_bsscfg_deinit(void *ctx, wlc_bsscfg_t *cfg);
static void *
mbo_iov_context_alloc(void *ctx, uint size);
static void
mbo_iov_context_free(void *ctx, void *iov_ctx, uint size);
static int
BCMATTACHFN(mbo_iov_get_digest_cb)(void *ctx, bcm_iov_cmd_digest_t **dig);
static int
wlc_mbo_iov_cmd_validate(const bcm_iov_cmd_digest_t *dig, uint32 actionid,
	const uint8 *ibuf, size_t ilen, uint8 *obuf, size_t *olen);
static int wlc_mbo_ie_build_fn(void *ctx, wlc_iem_build_data_t *data);
static uint wlc_mbo_ie_calc_len(void *ctx, wlc_iem_calc_data_t *data);
static uint wlc_mbo_ie_supp_opclass_len(void *ctx, wlc_iem_calc_data_t *data);
static int wlc_mbo_ie_supp_opclass_build_fn(void *ctx, wlc_iem_build_data_t *data);
static int wlc_mbo_ie_parse_fn(void *ctx, wlc_iem_parse_data_t *data);
static void wlc_mbo_free_chan_pref_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg);
static void wlc_mbo_add_non_pref_chan_attr_header(uint8 *cp, uint8 buf_len, uint8 len);
static void wlc_mbo_add_non_pref_chan_subelem_header(uint8 *cp, uint8 buf_len, uint8 len);
static int
wlc_mbo_iov_add_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_validate_chan_pref(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref);
static int
wlc_mbo_add_chan_pref_to_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref);
static int
wlc_mbo_del_chan_pref_from_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref);
static int
wlc_mbo_prep_non_pref_chan_report(wlc_mbo_info_t *mbo,
	wlc_bsscfg_t *bsscfg, uint8 *np_chan_se_buf,
	uint16 *np_chan_se_buf_len);
static int
wlc_mbo_prep_non_pref_chan_report_body(mbo_chan_pref_list_t *start,
	mbo_chan_pref_list_t *end, uint8 chan_list_len, uint8* buf, uint8 *buf_len);
static int
wlc_mbo_iov_del_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_list_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static uint16
wlc_mbo_count_pref_chan_list_entry(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg);
static int
wlc_mbo_iov_set_cellular_data_cap(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_get_cellular_data_cap(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_dump_counters(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_clear_counters(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_bsscfg_get(void *ctx, wlc_bsscfg_t *cfg, uint8 *data, int *len);
static int
wlc_mbo_bsscfg_set(void *ctx, wlc_bsscfg_t *cfg, const uint8 *data, int len);
static void
wlc_mbo_update_assoc_disallowed(wlc_bss_info_t *bi,
	wifi_mbo_assoc_disallowed_attr_t *assoc_dis);
static void
wlc_mbo_update_ap_cap(wlc_bss_info_t *bi,
	wifi_mbo_ap_cap_ind_attr_t *ap_cap);
#ifdef WL_MBO_WFA_CERT
static int
wlc_mbo_iov_set_force_assoc(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_get_force_assoc(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_set_force_assoc(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 value);
static int
wlc_mbo_get_force_assoc(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 *value);
static int
wlc_mbo_iov_set_bsstrans_reject(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
static int
wlc_mbo_iov_get_bsstrans_reject(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen);
#endif /* WL_MBO_WFA_CERT */

#define MAX_ADD_CHAN_PREF_CFG_SIZE  32
#define MAX_DEL_CHAN_PREF_CFG_SIZE  16
#define MAX_CELLULAR_DATA_CAP_SIZE  8
#define MAX_FORCE_ASSOC_CFG_SIZE  8
#define MIN_BSSTRANS_REJECT_CFG_SIZE 8
#define MAX_BSSTRANS_REJECT_CFG_SIZE 16
static const bcm_iov_cmd_info_t mbo_sub_cmds[] = {
	{WL_MBO_CMD_ADD_CHAN_PREF, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, NULL, wlc_mbo_iov_add_chan_pref, 0,
	MAX_ADD_CHAN_PREF_CFG_SIZE, MAX_ADD_CHAN_PREF_CFG_SIZE, 0, 0
	},
	{WL_MBO_CMD_DEL_CHAN_PREF, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, NULL, wlc_mbo_iov_del_chan_pref, 0,
	MAX_DEL_CHAN_PREF_CFG_SIZE, MAX_DEL_CHAN_PREF_CFG_SIZE, 0, 0
	},
	{WL_MBO_CMD_LIST_CHAN_PREF, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, wlc_mbo_iov_list_chan_pref, NULL, 0,
	0, 0, 0, 0
	},
	{WL_MBO_CMD_CELLULAR_DATA_CAP, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, wlc_mbo_iov_get_cellular_data_cap,
	wlc_mbo_iov_set_cellular_data_cap, 0,
	MAX_CELLULAR_DATA_CAP_SIZE, MAX_CELLULAR_DATA_CAP_SIZE, 0, 0
	},
	{WL_MBO_CMD_DUMP_COUNTERS, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, wlc_mbo_iov_dump_counters, NULL, 0,
	0, 0, 0, 0
	},
	{WL_MBO_CMD_CLEAR_COUNTERS, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, NULL, wlc_mbo_iov_clear_counters, 0,
	0, 0, 0, 0
	},
#ifdef WL_MBO_WFA_CERT
	{WL_MBO_CMD_FORCE_ASSOC, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, wlc_mbo_iov_get_force_assoc,
	wlc_mbo_iov_set_force_assoc, 0,
	MAX_FORCE_ASSOC_CFG_SIZE, MAX_FORCE_ASSOC_CFG_SIZE, 0, 0
	},
	{WL_MBO_CMD_BSSTRANS_REJECT, BCM_IOV_CMD_FLAG_NONE,
	0, BCM_XTLV_OPTION_ALIGN32,
	wlc_mbo_iov_cmd_validate, wlc_mbo_iov_get_bsstrans_reject,
	wlc_mbo_iov_set_bsstrans_reject, 0,
	MIN_BSSTRANS_REJECT_CFG_SIZE, MAX_BSSTRANS_REJECT_CFG_SIZE, 0, 0
	},
#endif /* WL_MBO_WFA_CERT */
};

wlc_mbo_info_t *
BCMATTACHFN(wlc_mbo_attach)(wlc_info_t *wlc)
{
	wlc_mbo_info_t *mbo = NULL;
	wlc_mbo_data_t *mbo_data = NULL;
	int ret = BCME_OK;
	bcm_iov_parse_context_t *parse_ctx = NULL;
	bcm_iov_parse_config_t parse_cfg;
	bsscfg_cubby_params_t cubby_params;
	uint16 mbo_ie_build_fstbmp = FT2BMP(FC_ASSOC_REQ) |
		FT2BMP(FC_REASSOC_REQ) |
		FT2BMP(FC_PROBE_REQ);
	uint16 supp_opcls_fstbmp = FT2BMP(FC_ASSOC_REQ) |
		FT2BMP(FC_REASSOC_REQ);
	uint16 mbo_ie_parse_fstbmp = FT2BMP(FC_BEACON) |
		FT2BMP(FC_PROBE_RESP) |
		FT2BMP(FC_ASSOC_RESP) |
		FT2BMP(FC_REASSOC_RESP) |
		FT2BMP(WLC_IEM_FC_SCAN_BCN) |
		FT2BMP(WLC_IEM_FC_SCAN_PRBRSP);

	mbo = (wlc_mbo_info_t *)MALLOCZ(wlc->osh, sizeof(*mbo));
	if (mbo == NULL) {
		WL_ERROR(("wl%d: %s:out of mem. alloced %u bytes\n",
			wlc->pub->unit, __FUNCTION__,  MALLOCED(wlc->osh)));
		goto fail;
	}
	mbo_data = (wlc_mbo_data_t *) obj_registry_get(wlc->objr, OBJR_MBO_CMN_DATA);
	if (mbo_data == NULL) {
		mbo_data = (wlc_mbo_data_t *)MALLOCZ(wlc->osh, sizeof(*mbo_data));
		if (mbo_data == NULL) {
			WL_ERROR(("wl%d: %s:out of mem. alloced %u bytes\n",
				wlc->pub->unit, __FUNCTION__,  MALLOCED(wlc->osh)));
			goto fail;
		}
		obj_registry_set(wlc->objr, OBJR_MBO_CMN_DATA, mbo_data);
	}
	mbo->mbo_data = mbo_data;
	(void)obj_registry_ref(wlc->objr, OBJR_MBO_CMN_DATA);

	mbo->wlc = wlc;
	mbo->mbo_data->max_chan_pref_entries = MBO_MAX_CHAN_PREF_ENTRIES;

	/* parse config */
	parse_cfg.alloc_fn = (bcm_iov_malloc_t)mbo_iov_context_alloc;
	parse_cfg.free_fn = (bcm_iov_free_t)mbo_iov_context_free;
	parse_cfg.dig_fn = (bcm_iov_get_digest_t)mbo_iov_get_digest_cb;
	parse_cfg.max_regs = 1;
	parse_cfg.alloc_ctx = (void *)mbo;

	/* parse context */
	ret = bcm_iov_create_parse_context((const bcm_iov_parse_config_t *)&parse_cfg,
		&parse_ctx);
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: %s parse context creation failed\n",
			wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	mbo->iov_parse_ctx = parse_ctx;

	/* register module */
	ret = wlc_module_register(wlc->pub, mbo_iovars, "MBO", (void *)parse_ctx,
		bcm_iov_doiovar, wlc_mbo_watchdog,
		wlc_mbo_wlc_up, wlc_mbo_wlc_down);
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: %s wlc_module_register() failed\n",
			wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	/* reserve cubby in the bsscfg container for per-bsscfg private data */
	memset(&cubby_params, 0, sizeof(cubby_params));

	cubby_params.context = mbo;
	cubby_params.fn_init = wlc_mbo_bsscfg_init;
	cubby_params.fn_deinit = wlc_mbo_bsscfg_deinit;
	cubby_params.fn_get = wlc_mbo_bsscfg_get;
	cubby_params.fn_set = wlc_mbo_bsscfg_set;
	cubby_params.config_size = MBO_CUBBY_CFG_SIZE;

	mbo->cfgh = wlc_bsscfg_cubby_reserve_ext(wlc, sizeof(wlc_mbo_bsscfg_cubby_t *),
		&cubby_params);
	if (mbo->cfgh < 0) {
		WL_ERROR(("wl%d: %s: wlc_bsscfg_cubby_reserve() failed\n",
			wlc->pub->unit, __FUNCTION__));
			goto fail;
	}
	/* register mbo subcommands */
	ret = bcm_iov_register_commands(mbo->iov_parse_ctx, (void *)mbo,
		&mbo_sub_cmds[0], (size_t)SUBCMD_TBL_SZ(mbo_sub_cmds), NULL, 0);

	/* register MBO IE build callbacks */
	ret = wlc_iem_vs_add_build_fn_mft(wlc->iemi, mbo_ie_build_fstbmp, WLC_IEM_VS_IE_PRIO_MBO,
		wlc_mbo_ie_calc_len, wlc_mbo_ie_build_fn, mbo);
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_iem_vs_add_build_fn_mft(cell data) failed %d, \n",
			wlc->pub->unit, __FUNCTION__, ret));
		goto fail;
	}

	/* register MBO IE parse callbacks */
	ret = wlc_iem_vs_add_parse_fn_mft(wlc->iemi, mbo_ie_parse_fstbmp, WLC_IEM_VS_IE_PRIO_MBO,
		wlc_mbo_ie_parse_fn, mbo);
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_iem_vs_add_parse_fn failed:mbo ap cap ind\n",
		          wlc->pub->unit, __FUNCTION__));
		goto fail;
	}

	/* register supported opclass element build callback */
	ret = wlc_iem_add_build_fn_mft(wlc->iemi, supp_opcls_fstbmp, DOT11_MNG_REGCLASS_ID,
		wlc_mbo_ie_supp_opclass_len, wlc_mbo_ie_supp_opclass_build_fn, mbo);
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_iem_add_build_fn_mft(supp opclass) failed %d\n",
			wlc->pub->unit, __FUNCTION__, ret));
		goto fail;
	}
	wlc->pub->cmn->_mbo = TRUE;
	return mbo;
fail:
	MODULE_DETACH(mbo, wlc_mbo_detach);
	return NULL;
}

void
BCMATTACHFN(wlc_mbo_detach)(wlc_mbo_info_t* mbo)
{
	if (mbo) {
		mbo->wlc->pub->cmn->_mbo = FALSE;
		if (mbo->mbo_data && obj_registry_unref(mbo->wlc->objr, OBJR_MBO_CMN_DATA) == 0) {
			obj_registry_set(mbo->wlc->objr, OBJR_MBO_CMN_DATA, NULL);
			MFREE(mbo->wlc->osh, mbo->mbo_data, sizeof(*mbo->mbo_data));
			mbo->mbo_data = NULL;
		}
		wlc_module_unregister(mbo->wlc->pub, "mbo", mbo);
		MFREE(mbo->wlc->osh, mbo, sizeof(*mbo));
		mbo = NULL;
	}
}

static void
wlc_mbo_watchdog(void *ctx)
{

}

static int
wlc_mbo_wlc_up(void *ctx)
{
	return BCME_OK;
}

static int
wlc_mbo_wlc_down(void *ctx)
{
	return BCME_OK;
}

static int
wlc_mbo_bsscfg_init(void *ctx, wlc_bsscfg_t *cfg)
{
	int ret = BCME_OK;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	wlc_mbo_bsscfg_cubby_t **pmbc = NULL;
	wlc_info_t *wlc;

	/* Initialize only sta */
	if (!MBO_BSSCFG_STA(cfg)) {
		return ret;
	}
	ASSERT(mbo != NULL);

	wlc = cfg->wlc;
	pmbc = MBO_BSSCFG_CUBBY_LOC(mbo, cfg);
	mbc = MALLOCZ(wlc->osh, sizeof(*mbc));
	if (mbc == NULL) {
		WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
			wlc->pub->unit, __FUNCTION__, MALLOCED(wlc->osh)));
		ret = BCME_NOMEM;
		goto fail;
	}
	*pmbc = mbc;
	/* during clone re-use counters of old cfg */
	if (BSSCFG_IS_RSDB_CLONE(cfg)) {
		return BCME_OK;
	}
	mbc->cntrs = MALLOCZ(wlc->osh, sizeof(*mbc->cntrs));
	if (mbc->cntrs == NULL) {
		WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
			wlc->pub->unit, __FUNCTION__, MALLOCED(wlc->osh)));
		ret = BCME_NOMEM;
		goto fail;
	}
	return ret;
fail:
	if (mbc) {
		wlc_mbo_bsscfg_deinit(ctx, cfg);
	}
	return ret;
}

static void
wlc_mbo_bsscfg_deinit(void *ctx, wlc_bsscfg_t *cfg)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	wlc_mbo_bsscfg_cubby_t **pmbc = NULL;
	wlc_info_t *wlc;

	/* deinitialize only sta */
	if (!MBO_BSSCFG_STA(cfg)) {
		return;
	}
	ASSERT(mbo != NULL);
	wlc = cfg->wlc;
	pmbc = MBO_BSSCFG_CUBBY_LOC(mbo, cfg);
	mbc = *pmbc;
	ASSERT(mbc != NULL);
	if (mbc->cntrs) {
		MFREE(wlc->osh, mbc->cntrs, sizeof(*mbc->cntrs));
		mbc->cntrs = NULL;
	}
	if (mbc->np_chan_attr_buf) {
		MFREE(wlc->osh, mbc->np_chan_attr_buf, MBO_MAX_NON_PREF_CHAN_ATTRS_LEN);
		mbc->np_chan_attr_buf = NULL;
		mbc->np_chan_attr_buf_len = 0;
	}
	if (mbc->chan_pref_list_head) {
		wlc_mbo_free_chan_pref_list(mbo, cfg);
	}
	MFREE(wlc->osh, mbc, sizeof(*mbc));
	pmbc = NULL;
	return;
}

/* bsscfg copy :get function */
static int
wlc_mbo_bsscfg_get(void *ctx, wlc_bsscfg_t *cfg, uint8 *data, int *len)
{
#ifdef STA
	if (MBO_BSSCFG_STA(cfg)) {
		wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
		wlc_mbo_bsscfg_cubby_t *mbc = NULL;

		if (data == NULL || *len < MBO_CUBBY_CFG_SIZE) {
			*len  = MBO_CUBBY_CFG_SIZE;
			return BCME_BUFTOOSHORT;
		}
		mbc = MBO_BSSCFG_CUBBY(mbo, cfg);
		if (mbc == NULL) {
			*len = 0;
			return BCME_OK;
		}
		memcpy(data, mbc, MBO_CUBBY_CFG_SIZE);
		*len = MBO_CUBBY_CFG_SIZE;
		/* reset the data pointers to NULL so that wlc_mbo_bsscfg_deinit()
		 * doesn't free up allocated data
		 */
		memset(mbc, 0, MBO_CUBBY_CFG_SIZE);
	}
#endif /* STA */
	return BCME_OK;
}

/* bsscfg copy: set function */
static int
wlc_mbo_bsscfg_set(void *ctx, wlc_bsscfg_t *cfg, const uint8 *data, int len)
{
#ifdef STA
	if (MBO_BSSCFG_STA(cfg)) {
		wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
		wlc_mbo_bsscfg_cubby_t *mbc = NULL;
		mbc = MBO_BSSCFG_CUBBY(mbo, cfg);
		if (mbc == NULL) {
			return BCME_OK;
		}
		if (data == NULL || len < MBO_CUBBY_CFG_SIZE) {
			return BCME_BADARG;
		}
		memcpy(mbc, (wlc_mbo_bsscfg_cubby_t *)data, MBO_CUBBY_CFG_SIZE);
	}
#endif /* STA */
	return BCME_OK;
}

/* iovar context alloc */
static void *
mbo_iov_context_alloc(void *ctx, uint size)
{
	uint8 *iov_ctx = NULL;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;

	ASSERT(mbo != NULL);

	iov_ctx = MALLOCZ(mbo->wlc->osh, size);
	if (iov_ctx == NULL) {
		WL_ERROR(("wl%d: %s: MALLOC failed, size %d malloced %d bytes\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
	}

	return iov_ctx;
}

/* iovar context free */
static void
mbo_iov_context_free(void *ctx, void *iov_ctx, uint size)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;

	ASSERT(mbo != NULL);
	if (iov_ctx) {
		MFREE(mbo->wlc->osh, iov_ctx, size);
	}
}

/* command digest alloc function */
static int
BCMATTACHFN(mbo_iov_get_digest_cb)(void *ctx, bcm_iov_cmd_digest_t **dig)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	int ret = BCME_OK;
	uint8 *iov_cmd_dig = NULL;

	ASSERT(mbo != NULL);
	iov_cmd_dig = MALLOCZ(mbo->wlc->osh, sizeof(bcm_iov_cmd_digest_t));
	if (iov_cmd_dig == NULL) {
		WL_ERROR(("wl%d: %s: MALLOC failed, size %d malloced %d bytes\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		ret = BCME_NOMEM;
		goto fail;
	}
	*dig = (bcm_iov_cmd_digest_t *)iov_cmd_dig;
fail:
	return ret;
}

/* validation function for IOVAR */
static int
wlc_mbo_iov_cmd_validate(const bcm_iov_cmd_digest_t *dig, uint32 actionid,
	const uint8 *ibuf, size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	wlc_info_t *wlc = NULL;
	wlc_mbo_info_t *mbo = NULL;

	ASSERT(dig);
	mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	ASSERT(mbo);
	wlc = mbo->wlc;
	if (!MBO_ENAB(wlc->pub) &&
		(dig->cmd_info->cmd != WL_MBO_CMD_ADD_CHAN_PREF)) {
		ret = BCME_UNSUPPORTED;
		goto fail;
	}
fail:
	return ret;
}

/* unpack calback for channel preference */
static int
wlc_mbo_chan_pref_cbfn(void *ctx, const uint8 *data, uint16 type, uint16 len)
{
	wlc_mbo_chan_pref_t *ch_pref = (wlc_mbo_chan_pref_t *)ctx;
	if (ctx == NULL || data == NULL) {
		return BCME_BADARG;
	}
	switch (type) {
		case WL_MBO_XTLV_OPCLASS:
			ch_pref->opclass = *data;
			break;
		case WL_MBO_XTLV_CHAN:
			ch_pref->chan = *data;
			break;
		case WL_MBO_XTLV_PREFERENCE:
			ch_pref->pref = *data;
			break;
		case WL_MBO_XTLV_REASON_CODE:
			ch_pref->reason = *data;
			break;
		default:
			WL_MBO_ERR(("%s: Unknown tlv type %u\n", __FUNCTION__, type));
	}
	return BCME_OK;
}

/* "wl mbo add_chan_pref <>" handler */
static int
wlc_mbo_iov_add_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	wlc_mbo_chan_pref_t ch_pref;

	memset(&ch_pref, 0, sizeof(ch_pref));
	ret = bcm_unpack_xtlv_buf(&ch_pref, ibuf, ilen, BCM_XTLV_OPTION_ALIGN32,
		wlc_mbo_chan_pref_cbfn);
	if (ret != BCME_OK) {
		goto fail;
	}
	/* handle chan pref data */
	ret = wlc_mbo_add_chan_pref(mbo, dig->bsscfg, &ch_pref);
fail:
	return ret;
}

/* API to add channel preference for MBO */
int
wlc_mbo_add_chan_pref(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref)
{
	int ret = BCME_OK;
	wlc_info_t *wlc = mbo->wlc;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	uint8 *np_chan_se_buf = NULL;
	uint16 np_chan_se_buf_len = 0;

	/* add validation check */
	ret = wlc_mbo_validate_chan_pref(mbo, bsscfg, ch_pref);
	if (ret != BCME_OK) {
		goto fail;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	/* add to the ordered list */
	ret = wlc_mbo_add_chan_pref_to_list(mbo, bsscfg, ch_pref);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d:non pref chan add failed %d\n",
			wlc->pub->unit, __func__, ret));
		goto fail;
	}
	/* allocate buffer for pre-built non-preferred chan report attr */
	if (mbc->np_chan_attr_buf == NULL) {
		mbc->np_chan_attr_buf = MALLOCZ(wlc->osh, MBO_MAX_NON_PREF_CHAN_ATTRS_LEN);
		if (mbc->np_chan_attr_buf == NULL) {
			WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
				wlc->pub->unit, __FUNCTION__, MALLOCED(wlc->osh)));
			ret = BCME_NOMEM;
			goto fail;
		}
	}
	/* allocate buffer for np-chan report sub-element */
	np_chan_se_buf = MALLOCZ(mbo->wlc->osh, MBO_MAX_NON_PREF_CHAN_SE_LEN);
	if (np_chan_se_buf == NULL) {
		WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		ret = BCME_NOMEM;
		goto fail;
	}
	np_chan_se_buf_len = MBO_MAX_NON_PREF_CHAN_SE_LEN;
	/* update pre-built non preferred chan report */
	/* prepare non pref chan sub-elem buffer */
	ret = wlc_mbo_prep_non_pref_chan_report(mbo, bsscfg,
		np_chan_se_buf, &np_chan_se_buf_len);
	if (ret != BCME_OK) {
		goto fail;
	}
#ifdef BCMDBG
	prhex("np chan attr:", mbc->np_chan_attr_buf, mbc->np_chan_attr_buf_len);
	prhex("np chan se:", np_chan_se_buf, np_chan_se_buf_len);
#endif /* BCMDBG */
#ifdef WLWNM
	/* if joined, send WNM Notification to AP */
	if (WLC_BSS_CONNECTED(bsscfg) && WLWNM_ENAB(wlc->pub)) {
		ret = wlc_wnm_notif_req_send(wlc->wnm_info, bsscfg,
			NULL, MBO_SUBELEM_ID, np_chan_se_buf,
			np_chan_se_buf_len);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d:WNM-Notif send failed %d\n",
				wlc->pub->unit, __func__, ret));
			goto fail;
		}
	}
#endif /* WLWNM */
fail:
	if (np_chan_se_buf) {
		MFREE(wlc->osh, np_chan_se_buf, MBO_MAX_NON_PREF_CHAN_SE_LEN);
		np_chan_se_buf = NULL;
		np_chan_se_buf_len = 0;
	}
	return ret;
}

/* An operating class with no channel number means preference is applicable
 * for all channels of that operating class. Operating class with channel
 * number means preference applicable to that channel only. These two type
 * of configuration is mutually exclusive i.e if any one type of configuration
 * is present the other one wont be allowed.
 */
static int
wlc_mbo_validate_chan_pref(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	mbo_chan_pref_list_t *cur = NULL;
	if (mbo == NULL || bsscfg == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	cur = mbc->chan_pref_list_head;
	while (cur != NULL) {
		if ((cur->opclass == ch_pref->opclass) &&
			(((cur->chan == 0) && (ch_pref->chan != 0)) ||
			((cur->chan != 0) && (ch_pref->chan == 0)))) {
			WL_MBO_ERR(("wl%d:either all channel of band or individual channel "
				"under the band\n", mbo->wlc->pub->unit, __func__));
			return BCME_BADOPTION;
		}
		cur = cur->next;
	}
	return BCME_OK;
}

/* Add chan preference to the list */
static int
wlc_mbo_add_chan_pref_to_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref)
{
	wlc_info_t *wlc = mbo->wlc;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	mbo_chan_pref_list_t *cur = NULL, *loc = NULL, *new = NULL, *prev = NULL;

	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	/* remove from list if already exist. Ignore return value */
	wlc_mbo_del_chan_pref_from_list(mbo, bsscfg, ch_pref);
	/* we should not cross max limit */
	if (wlc_mbo_count_pref_chan_list_entry(mbo, bsscfg) >=
			mbo->mbo_data->max_chan_pref_entries) {
		WL_MBO_ERR(("wl%d:Max limit reached\n",
			mbo->wlc->pub->unit, __func__));
		return BCME_NORESOURCE;
	}
	/* Now, find the right place */
	cur = mbc->chan_pref_list_head;
	while (cur != NULL) {
		if ((cur->opclass == ch_pref->opclass) &&
			(cur->pref == ch_pref->pref) &&
			(cur->reason == ch_pref->reason)) {
				loc = cur;
		}
		prev = cur;
		/* go for next */
		cur = cur->next;
	}
	/* allocate */
	new = MALLOCZ(wlc->osh, sizeof(*new));
	if (new == NULL) {
		WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
			wlc->pub->unit, __FUNCTION__, MALLOCED(wlc->osh)));
		return BCME_NOMEM;
	}
	/* fill it up */
	new->opclass = ch_pref->opclass;
	new->chan = ch_pref->chan;
	new->pref = ch_pref->pref;
	new->reason = ch_pref->reason;
	/* link the node */
	if ((mbc->chan_pref_list_head == NULL)) {
		mbc->chan_pref_list_head = new;
	} else {
		if (loc) {
			/* add at right position */
			new->next = loc->next;
			loc->next = new;
		} else {
			ASSERT(prev != NULL);
			/* add at the end */
			prev->next = new;
		}
	}
	return BCME_OK;
}

/* delete chan preference from the list */
static int
wlc_mbo_del_chan_pref_from_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	mbo_chan_pref_list_t *cur = NULL, *prev = NULL;

	ASSERT(mbo);
	ASSERT(bsscfg);
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	cur = prev = mbc->chan_pref_list_head;

	while (cur) {
		if ((cur->opclass == ch_pref->opclass) &&
			(cur->chan == ch_pref->chan)) {
			if (cur ==  mbc->chan_pref_list_head) {
				mbc->chan_pref_list_head = cur->next;
			} else {
				prev->next = cur->next;
			}
			MFREE(mbo->wlc->osh, cur, sizeof(*cur));
			return BCME_OK;
		}
		prev = cur;
		cur = cur->next;
	}
	return BCME_NOTFOUND;
}

/* Free up channel preference list */
static void
wlc_mbo_free_chan_pref_list(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	mbo_chan_pref_list_t *cur = NULL;
	mbo_chan_pref_list_t *temp = NULL;

	if ((mbo == NULL) || (bsscfg == NULL)) {
		return;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	cur = mbc->chan_pref_list_head;
	while (cur) {
		temp = cur;
		cur = cur->next;
		MFREE(mbo->wlc->osh, temp, sizeof(*temp));
	}
	mbc->chan_pref_list_head = NULL;
}

/* prepare non-preferred channel report body for attribute/sub-element */
static int
wlc_mbo_prep_non_pref_chan_report_body(mbo_chan_pref_list_t *start,
	mbo_chan_pref_list_t *end, uint8 chan_list_len, uint8* buf, uint8 *buf_len)
{
	mbo_chan_pref_list_t *cur = start;
	uint8 *cp = buf;

	if (*buf_len < MBO_NON_PREF_CHAN_ATTR_LEN(chan_list_len)) {
		return BCME_BUFTOOSHORT;
	}
	/* op class */
	*cp = start->opclass;
	cp += MBO_NON_PREF_CHAN_ATTR_OPCALSS_LEN;

	/* chan list */
	while (cur != end) {
		*cp = cur->chan;
		cp++;
		/* move to next */
		cur = cur->next;
	}
	/* This takes care of last node and start euqals to end case */
	if (end) {
		*cp = cur->chan;
		cp++;
	}

	/* preference */
	*cp = start->pref;
	cp += MBO_NON_PREF_CHAN_ATTR_PREF_LEN;

	/* Reason Code */
	*cp = start->reason;
	cp += MBO_NON_PREF_CHAN_ATTR_REASON_LEN;

	*buf_len = MBO_NON_PREF_CHAN_ATTR_LEN(chan_list_len);
	return BCME_OK;
}

/* Non-preferred Channel Report Attribute header.
 * Refer: section 4.2.2 of MBO Tech Spec
 * cp = buffer to be filled
 * buf_len = length of the buffer
 * len = length value to be assigined in attribute header
 */
static void
wlc_mbo_add_non_pref_chan_attr_header(uint8 *cp, uint8 buf_len, uint8 len)
{
	ASSERT(buf_len >= (MBO_ATTR_ID_LEN + MBO_ATTR_LEN_LEN));
	/* attr id */
	*cp = MBO_ATTR_NON_PREF_CHAN_REPORT;
	cp += MBO_ATTR_ID_LEN;

	/* attr len */
	*cp = len;
	cp += MBO_ATTR_LEN_LEN;
}

/* Non-preferred Channel Report subelement header.
 * Refer: section 4.4.1 of MBO Tech Spec
 * cp = buffer to be filled
 * buf_len = length of the buffer
 * len = length value to be assigined in attribute header
 */
static void
wlc_mbo_add_non_pref_chan_subelem_header(uint8 *cp, uint8 buf_len, uint8 len)
{
	ASSERT(buf_len >= (MBO_SUBELEM_ID_LEN + MBO_SUBELEM_LEN_LEN +
		WFA_OUI_LEN + MBO_ATTR_ID_LEN));
	/* subelement id */
	*cp = MBO_SUBELEM_ID;
	cp += MBO_SUBELEM_ID_LEN;

	/* subelem len */
	*cp = len;
	cp += MBO_SUBELEM_LEN_LEN;

	/* subelem oui */
	memcpy(cp, MBO_SUBELEM_OUI, WFA_OUI_LEN);
	cp += WFA_OUI_LEN;

	/* subelem oui type */
	*cp = MBO_ATTR_NON_PREF_CHAN_REPORT;
	cp += MBO_ATTR_ID_LEN;
}

/* This function prepares Non-preferred Channel Report Attrinut/Subelement
 * and store them in buffers(in bsscfg cubby) for future use.
 * np_chan_attr_buf: To be used for non-preferred chan report attr
 * np_chan_se_buf: To be used for non-preferred chan report subelement
 */
static int
wlc_mbo_prep_non_pref_chan_report(wlc_mbo_info_t *mbo,
	wlc_bsscfg_t *bsscfg, uint8 *np_chan_se_buf,
	uint16 *np_chan_se_buf_len)
{
	int ret = BCME_OK;
	mbo_chan_pref_list_t *cur = NULL, *start = NULL, *end = NULL;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	uint8 tot_attr_len = 0;
	uint8 tot_subelem_len = 0;
	uint8 *cp1 = NULL;
	uint8 *cp2 = NULL;
	uint8 chan_list_len = 0;
	uint8 buf[MAX_NON_PREF_CHAN_BODY_LEN];
	uint8 buf_len = 0;

	if (!mbo || !bsscfg || !np_chan_se_buf || !np_chan_se_buf_len) {
		return BCME_BADARG;
	}
	ASSERT(mbc->np_chan_attr_buf);

	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	/* reset old attr report */
	memset(mbc->np_chan_attr_buf, 0, MBO_MAX_NON_PREF_CHAN_ATTRS_LEN);
	mbc->np_chan_attr_buf_len = 0;
	cp1 = mbc->np_chan_attr_buf;
	/* reset old se report */
	cp2 = np_chan_se_buf;

	cur = mbc->chan_pref_list_head;
	if (cur == NULL) {
		/* Empty list i.e entries have been deleted just now
		 * Prepare a no-body Non-preferred Channel report subelemet only
		 */
		wlc_mbo_add_non_pref_chan_subelem_header(cp2,
			MBO_MAX_NON_PREF_CHAN_SE_LEN, 4);
		*np_chan_se_buf_len = MBO_SUBELEM_HDR_LEN;
		return BCME_OK;
	}
	start = cur;
	do {
		if ((cur->next == NULL) || ((cur->next != NULL) &&
			((cur->opclass != cur->next->opclass) ||
			(cur->pref != cur->next->pref) || (cur->reason != cur->reason)))) {
			/* NOTE: we are stopping on non pref chan buffer len consumed */
			/* validate sufficient space for another attr */
			if ((MBO_NON_PREF_CHAN_ATTR_TOT_LEN(chan_list_len)) >
				(MBO_MAX_NON_PREF_CHAN_ATTRS_LEN - tot_attr_len) ||
				(MBO_NON_PREF_CHAN_SUBELEM_TOT_LEN(chan_list_len) >
				(MBO_MAX_NON_PREF_CHAN_SE_LEN - tot_subelem_len))) {
				break;
			}
			end = cur;
			/* create non pref chan body */
			memset(buf, 0, sizeof(buf));
			buf_len = MAX_NON_PREF_CHAN_BODY_LEN;
			chan_list_len++;
			ret = wlc_mbo_prep_non_pref_chan_report_body(start, end,
				chan_list_len, buf, &buf_len);
			if (ret != BCME_OK) {
				return ret;
			}
			/* add non pref chan attr header */
			wlc_mbo_add_non_pref_chan_attr_header
				(cp1, MBO_MAX_NON_PREF_CHAN_ATTRS_LEN,
				MBO_NON_PREF_CHAN_ATTR_LEN(chan_list_len));
			cp1 += MBO_ATTR_HDR_LEN;
			/* append  non pref chan body */
			memcpy(cp1, buf, buf_len);
			cp1 += buf_len;

			/* add non pref chan subelement header */
			wlc_mbo_add_non_pref_chan_subelem_header
				(cp2, MBO_MAX_NON_PREF_CHAN_SE_LEN,
				MBO_NON_PREF_CHAN_SUBELEM_LEN_LEN(chan_list_len));
			cp2 += MBO_SUBELEM_HDR_LEN;

			/* append  non pref chan body */
			memcpy(cp2, buf, buf_len);
			cp2 += buf_len;

			tot_attr_len += MBO_NON_PREF_CHAN_ATTR_TOT_LEN(chan_list_len);
			tot_subelem_len += MBO_NON_PREF_CHAN_SUBELEM_TOT_LEN(chan_list_len);

			/* update for next report */
			start = cur->next;
			chan_list_len = 0;
		} else {
			chan_list_len++;
		}
		cur = cur->next;
	} while (cur != NULL);
	/* update num of bytes used */
	mbc->np_chan_attr_buf_len = tot_attr_len;
	*np_chan_se_buf_len = tot_subelem_len;
	return BCME_OK;
}

/* "wl mbo del_chan_pref <>" handler */
static int
wlc_mbo_iov_del_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	wlc_mbo_chan_pref_t ch_pref;

	memset(&ch_pref, 0, sizeof(ch_pref));
	ret = bcm_unpack_xtlv_buf(&ch_pref, ibuf, ilen, BCM_XTLV_OPTION_ALIGN32,
		wlc_mbo_chan_pref_cbfn);
	if (ret != BCME_OK) {
		goto fail;
	}
	/* handle chan pref data */
	ret = wlc_mbo_del_chan_pref(mbo, dig->bsscfg, &ch_pref);
fail:
	return ret;
}

/* API to delete chan/band preference from list */
int
wlc_mbo_del_chan_pref(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	wlc_mbo_chan_pref_t *ch_pref)
{
	int ret = BCME_OK;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	uint8 *np_chan_se_buf = NULL;
	uint16 np_chan_se_buf_len = 0;

	UNUSED_PARAMETER(mbc);

	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	/* delete from list */
	if (mbc->chan_pref_list_head) {
		ret = wlc_mbo_del_chan_pref_from_list(mbo, bsscfg, ch_pref);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d:pref chan del failed %d\n",
				mbo->wlc->pub->unit, __func__, ret));
			goto fail;
		}
		/* allocate buffer for np-chan report sub-element */
		np_chan_se_buf = MALLOCZ(mbo->wlc->osh, MBO_MAX_NON_PREF_CHAN_SE_LEN);
		if (np_chan_se_buf == NULL) {
			WL_ERROR(("wl%d: %s: out of mem, malloced %d bytes\n",
				mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
			ret = BCME_NOMEM;
			goto fail;
		}
		np_chan_se_buf_len = MBO_MAX_NON_PREF_CHAN_SE_LEN;
		/* update pre-built non preferred chan report */
		/* prepare non pref chan report attr */
		ret = wlc_mbo_prep_non_pref_chan_report(mbo, bsscfg,
			np_chan_se_buf, &np_chan_se_buf_len);
		if (ret != BCME_OK) {
			goto fail;
		}
#ifdef WLWNM
		/* if joined, send WNM Notification to AP */
		if (WLC_BSS_CONNECTED(bsscfg) && WLWNM_ENAB(mbo->wlc->pub)) {
			ret = wlc_wnm_notif_req_send(mbo->wlc->wnm_info, bsscfg,
				NULL, MBO_SUBELEM_ID, np_chan_se_buf,
				np_chan_se_buf_len);
			if (ret != BCME_OK) {
				WL_MBO_ERR(("wl%d:WNM-Notif send failed %d\n",
					mbo->wlc->pub->unit, __func__, ret));
				goto fail;
			}
		}
#endif /* WLWNM */
	}
fail:
	if (mbc->chan_pref_list_head == NULL) {
		/* entries are not there, free up pre-built buffer for np-chan report if-any */
		if (mbc->np_chan_attr_buf) {
			MFREE(mbo->wlc->osh, mbc->np_chan_attr_buf,
				MBO_MAX_NON_PREF_CHAN_ATTRS_LEN);
			mbc->np_chan_attr_buf = NULL;
			mbc->np_chan_attr_buf_len = 0;
		}
	}
	if (np_chan_se_buf) {
		MFREE(mbo->wlc->osh, np_chan_se_buf, MBO_MAX_NON_PREF_CHAN_SE_LEN);
		np_chan_se_buf = NULL;
		np_chan_se_buf_len = 0;
	}
	return ret;
}

/* "wl mbo list_chan_pref" handler */
static int
wlc_mbo_iov_list_chan_pref(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	mbo_chan_pref_list_t *cur = NULL;
	uint16 buflen = 0, count = 0, xtlv_size = 0;

	count = wlc_mbo_count_pref_chan_list_entry(mbo, dig->bsscfg);
	xtlv_size = bcm_xtlv_size_for_data(sizeof(uint8), BCM_XTLV_OPTION_ALIGN32);
	if ((count * xtlv_size) > *olen) {
		*olen = 0;
		ret = BCME_BUFTOOSHORT;
		goto fail;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, dig->bsscfg);
	cur = mbc->chan_pref_list_head;
	buflen = *olen;
	while (cur) {
		xtlv_desc_t xtlv_ar[] = {
			{WL_MBO_XTLV_OPCLASS,  sizeof(uint8), &cur->opclass},
			{WL_MBO_XTLV_CHAN,  sizeof(uint8), &cur->chan},
			{WL_MBO_XTLV_PREFERENCE,  sizeof(uint8), &cur->pref},
			{WL_MBO_XTLV_REASON_CODE,  sizeof(uint8), &cur->reason},
			{0, 0, NULL}
		};
		ret = bcm_pack_xtlv_buf_from_mem(&obuf, &buflen, xtlv_ar,
			BCM_XTLV_OPTION_ALIGN32);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d: %s: packing xtlvs failed\n",
				mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
			goto fail;
		}
		cur = cur->next;
	}
	*olen = *olen - buflen;
fail:
	return ret;
}

/* returns count of chan preference entry present in the list */
static uint16
wlc_mbo_count_pref_chan_list_entry(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg)
{
	mbo_chan_pref_list_t *cur = NULL;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	uint16 count = 0;
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	cur = mbc->chan_pref_list_head;
	while (cur) {
		count++;
		cur = cur->next;
	}
	return count;
}

/* Build "Cellular Capabilities Subelement".
 * Refer section 4.4.2 of MBO Tech spec
 */
static void
wlc_mbo_prep_cell_data_cap_subelement(uint8 cap, uint8 *buf, uint16 *buf_len)
{
	wifi_mbo_cell_cap_subelem_t *se = NULL;
	if (*buf_len != MBO_MAX_CELL_DATA_CAP_SE_LEN) {
		*buf_len = 0;
		return;
	}
	se = (wifi_mbo_cell_cap_subelem_t *)buf;
	se->sub_elem_id = MBO_SUBELEM_ID;
	se->len = sizeof(*se) - (MBO_SUBELEM_ID_LEN + MBO_SUBELEM_LEN_LEN);
	/* subelem oui */
	memcpy(se->oui, MBO_SUBELEM_OUI, WFA_OUI_LEN);
	se->oui_type = MBO_ATTR_CELL_DATA_CAP;
	se->cell_conn = cap;
}

/* API to set cellular data capability */
int
wlc_mbo_set_cellular_data_cap(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 cell_data_cap)
{
	uint8 se_buf[MBO_MAX_CELL_DATA_CAP_SE_LEN];
	uint16 se_buf_len = MBO_MAX_CELL_DATA_CAP_SE_LEN;

	if (mbo == NULL || bsscfg == NULL) {
		return BCME_BADARG;
	}
	/* set cellular data capability */
	mbo->mbo_data->cell_data_cap = cell_data_cap;

	wlc_mbo_prep_cell_data_cap_subelement(cell_data_cap, se_buf, &se_buf_len);
	if (se_buf_len != MBO_MAX_CELL_DATA_CAP_SE_LEN) {
		return BCME_BADLEN;
	}
#ifdef BCMDBG
	prhex("cell data se", se_buf, se_buf_len);
#endif /* BCMDBG */
#ifdef WLWNM
	/* if joined, send WNM Notification to AP */
	if (WLC_BSS_CONNECTED(bsscfg) && WLWNM_ENAB(mbo->wlc->pub)) {
		int ret = wlc_wnm_notif_req_send(mbo->wlc->wnm_info, bsscfg,
			NULL, MBO_SUBELEM_ID, se_buf, se_buf_len);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d:WNM-Notif send failed %d\n",
				mbo->wlc->pub->unit, __func__, ret));
			return ret;
		}
	}
#endif /* WLWNM */
	return BCME_OK;
}

/* API to get cellular data capability */
int
wlc_mbo_get_cellular_data_cap(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 *cell_data_cap)
{
	if (mbo == NULL || bsscfg == NULL || cell_data_cap == NULL) {
		return BCME_BADARG;
	}
	/* set cellular data capability */
	*cell_data_cap = mbo->mbo_data->cell_data_cap;
	return BCME_OK;
}

/* "wl mbo cell_data_cap <>" handler */
static int
wlc_mbo_iov_set_cellular_data_cap(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint8 data = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;

	UNUSED_PARAMETER(mbo);
	ret = bcm_unpack_xtlv_entry((uint8 **)&ibuf, WL_MBO_XTLV_CELL_DATA_CAP, sizeof(uint8),
			&data, BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: unpacking xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		goto fail;
	}
	ret = wlc_mbo_set_cellular_data_cap(dig->cmd_ctx, dig->bsscfg, data);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: set cellular data capability failed\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		goto fail;
	}
fail:
	return ret;
}

/* "wl mbo cell_data_cap" handler */
static int
wlc_mbo_iov_get_cellular_data_cap(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint16 xtlv_size = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	uint16 buflen = 0;

	if (mbo == NULL) {
		return BCME_BADARG;
	}
	xtlv_size = bcm_xtlv_size_for_data(sizeof(mbo->mbo_data->cell_data_cap),
		BCM_XTLV_OPTION_ALIGN32);
	if (xtlv_size > *olen) {
		*olen = 0;
		ret = BCME_BUFTOOSHORT;
		goto fail;
	}
	buflen = *olen;
	ret = bcm_pack_xtlv_entry(&obuf, &buflen, WL_MBO_XTLV_CELL_DATA_CAP,
			sizeof(mbo->mbo_data->cell_data_cap), &mbo->mbo_data->cell_data_cap,
			BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: packing xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		goto fail;
	}
	*olen = *olen - buflen;

fail:
	return ret;
}

/* "wl mbo counters" handler */
static int
wlc_mbo_iov_dump_counters(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	uint16 buflen = 0, xtlv_size = 0;

	if (mbo == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, dig->bsscfg);
	if (mbc->cntrs) {
		xtlv_size = bcm_xtlv_size_for_data(sizeof(*mbc->cntrs), BCM_XTLV_OPTION_ALIGN32);
		if (xtlv_size > *olen) {
			*olen = 0;
			ret = BCME_BUFTOOSHORT;
			goto fail;
		}
		buflen = *olen;
		ret = bcm_pack_xtlv_entry(&obuf, &buflen, WL_MBO_XTLV_COUNTERS,
			sizeof(*mbc->cntrs), (uint8 *)mbc->cntrs, BCM_XTLV_OPTION_ALIGN32);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d: %s: packing xtlvs failed\n",
				mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
			goto fail;
		}
		*olen = *olen - buflen;
	} else {
		*olen = 0;
	}
fail:
	return ret;
}

/* "wl mbo clear_counters" handler */
static int
wlc_mbo_iov_clear_counters(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;

	if (mbo == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, dig->bsscfg);
	if (mbc->cntrs) {
		memset(mbc->cntrs, 0, sizeof(*mbc->cntrs));
	}
	return BCME_OK;
}

/* This function writes "Supported Operaing Classes" element.
 * Refer section 3.2 "Channel and Band Indication and preference"
 * of MBO Tech spec
 */
static int
wlc_mbo_ie_supp_opclass_build_fn(void *ctx, wlc_iem_build_data_t *data)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	chanspec_t chanspec;
	uint8 rclen;                       /* regulatory class length */
	uint8 rclist[MAXRCLISTSIZE];       /* regulatory class list */

	ASSERT(data != NULL && data->cfg != NULL && data->cfg->target_bss);
	chanspec = data->cfg->target_bss->chanspec;
	rclen = wlc_get_regclass_list(mbo->wlc->cmi, rclist, MAXRCLISTSIZE, chanspec, TRUE);
	if (rclen <= data->buf_len) {
		bcm_write_tlv_safe(DOT11_MNG_REGCLASS_ID, rclist, rclen, data->buf, data->buf_len);
	} else {
		ASSERT(0);
	}

	return BCME_OK;
}

/* Return num of bytes require for  "Supported Operaing Classes" element. */
static uint
wlc_mbo_ie_supp_opclass_len(void *ctx, wlc_iem_calc_data_t *data)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	chanspec_t chanspec;
	uint8 rclen;                       /* regulatory class length */
	uint8 rclist[MAXRCLISTSIZE];       /* regulatory class list */

	ASSERT(mbo != NULL);
	ASSERT(data != NULL && data->cfg != NULL && data->cfg->target_bss);
	chanspec = data->cfg->target_bss->chanspec;
	rclen = wlc_get_regclass_list(mbo->wlc->cmi, rclist, MAXRCLISTSIZE, chanspec, TRUE);
	return TLV_HDR_LEN + rclen;
}

/* This function builds MBO IE with "Non-preferred Channel report Attribute"
 * and "Cellular data Capability Attribute" for (re)assoc req and probe req.
 * Refer section 3.2, 3.3, 4.2.2 and 4.2.3 of MBO Tech Spec.
 */
static int
wlc_mbo_ie_build_fn(void *ctx, wlc_iem_build_data_t *data)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	uint8 *cp = NULL;
	wifi_mbo_ie_t *ie_hdr = NULL;

	uint total_len = 0;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	wlc_bsscfg_t *bsscfg = NULL;
	wifi_mbo_cell_data_cap_attr_t *cdc_attr = NULL;

	ASSERT(mbo != NULL);
	ASSERT(data != NULL);
	bsscfg = data->cfg;
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	if (mbo->mbo_data->cell_data_cap || mbc->np_chan_attr_buf_len) {
		cp = data->buf;
		ie_hdr = (wifi_mbo_ie_t *)cp;

		/* fill in MBO IE header */
		ie_hdr->id = MBO_IE_ID;
		memcpy(ie_hdr->oui, MBO_OUI, WFA_OUI_LEN);
		ie_hdr->oui_type = MBO_OUI_TYPE;
		cp += MBO_IE_HDR_SIZE;
		total_len = MBO_IE_NO_ATTR_LEN;

		switch (data->ft) {
			case FC_ASSOC_REQ:
			case FC_REASSOC_REQ:
			{
				if (mbc->np_chan_attr_buf_len) {
					ASSERT(mbc->np_chan_attr_buf);
					/* np chan report attr(s) */
					memcpy(cp, mbc->np_chan_attr_buf,
						mbc->np_chan_attr_buf_len);
					cp += mbc->np_chan_attr_buf_len;
					total_len += mbc->np_chan_attr_buf_len;
				}
			}
			/* Fall through */
			case FC_PROBE_REQ:
			{
				if (mbo->mbo_data->cell_data_cap) {
					cdc_attr = (wifi_mbo_cell_data_cap_attr_t *)cp;
					/* fill in Cellular Data Capability Attr */
					cdc_attr->id = MBO_ATTR_CELL_DATA_CAP;
					cdc_attr->len = sizeof(*cdc_attr) - MBO_ATTR_HDR_LEN;
					cdc_attr->cell_conn = mbo->mbo_data->cell_data_cap;

					cp += sizeof(*cdc_attr);
					total_len += sizeof(*cdc_attr);
				}
			}
			break;
			default:
				/* we should not be here !! */
				ASSERT(0);
		}
		/* update MBO IE len */
		ie_hdr->len = total_len;
	}

	return BCME_OK;
}

/* This function calculate bytes needed to write MBO IE with
 * "Non-preferred Channel report Attribute"
 * and "Cellular data Capability Attribute" for (re)assoc req and probe req.
 * Refer section 3.2, 3.3, 4.2.2 and 4.2.3 of MBO Tech Spec.
 */
static uint
wlc_mbo_ie_calc_len(void *ctx, wlc_iem_calc_data_t *data)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	uint total_len = 0;
	wlc_bsscfg_t *bsscfg = NULL;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;

	ASSERT(mbo != NULL);
	ASSERT(data != NULL);
	bsscfg = data->cfg;
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	if (mbo->mbo_data->cell_data_cap || mbc->np_chan_attr_buf_len) {
		total_len = MBO_IE_HDR_SIZE;
		switch (data->ft) {
			case FC_ASSOC_REQ:
			case FC_REASSOC_REQ:
			{
				if (mbc->np_chan_attr_buf_len) {
					total_len += mbc->np_chan_attr_buf_len;
				}
			}
			/* Fall through */
			case FC_PROBE_REQ:
			{
				if (mbo->mbo_data->cell_data_cap) {
					total_len += sizeof(wifi_mbo_cell_data_cap_attr_t);
				}
			}
			break;
			default:
			/* we should not be here !! */
			ASSERT(0);
		}
	}
	return total_len;
}

static void
wlc_mbo_update_assoc_disallowed(wlc_bss_info_t *bi,
	wifi_mbo_assoc_disallowed_attr_t *assoc_dis)
{
	ASSERT(bi != NULL);
	/* If assoc disallwed attr is present with a valid reason */
	if (assoc_dis &&
		(assoc_dis->reason_code >= MBO_ASSOC_DISALLOWED_RC_UNSPECIFIED) &&
		(assoc_dis->reason_code <= MBO_ASSOC_DISALLOWED_RC_INSUFFIC_RSSI)) {
		bi->bcnflags |= WLC_BSS_MBO_ASSOC_DISALLOWED;
	} else if ((assoc_dis == NULL) &&
		(bi->bcnflags & WLC_BSS_MBO_ASSOC_DISALLOWED)) {
		/* if assoc disallowed attr is not present and flag is set,
		* clear
		*/
		bi->bcnflags &= ~WLC_BSS_MBO_ASSOC_DISALLOWED;
	}
}

static void
wlc_mbo_update_ap_cap(wlc_bss_info_t *bi,
	wifi_mbo_ap_cap_ind_attr_t *ap_cap_attr)
{
	/* set MBO ap capability bit */
	if (ap_cap_attr) {
		bi->bcnflags |= WLC_BSS_MBO_CAPABLE;
	} else if (ap_cap_attr == NULL && (bi->bcnflags & WLC_BSS_MBO_CAPABLE)) {
		/* if MBO ap cap attr is not present and flag is set, clear */
		bi->bcnflags &= ~WLC_BSS_MBO_CAPABLE;
	}
}

/* From WFA MBO tech Spec v0.22: Section 3.6
 * "If an MBO AP cannot accept new associations, it shall include
 * the MBO IE with the Association Disallowed Attribute in Beacon,
 * Probe Response and (Re)Association Response frames and shall set
 * the value in the Status Code field in (Re)Association Response
 * frames to a value of seventeen (17) or thirty-one (31).
 * When an MBO STA receives a Beacon, Probe Response or
 * (Re)Association Response frame from an MBO AP containing an
 * MBO IE with the Association Disallowed Attribute, that MBO STA
 * shall not send a (Re)Association Request frame from that AP
 * without the Association Disallowed Attribute. The presence of the
 * MBO IE with the Association Disallowed Attribute in any of the Beacon,
 * Probe Response or (Re)Association Response frames shall be interpreted
 * as an indication that the MBO AP is currently not accepting associations."
 *
 * To support above requirements:
 * 1) (Re)Assoc Response: We don't process(parse IEs present in the frame)
 * Assoc Response if status is not success. Hence don't want to change this
 * behavior as we can handle this requirement via processing of scan beacon and
 * probe response without processing MBO IE in Assoc Response.
 * 2) Beacon and Probe response: MBO module will process Association Disallowed
 * Attr in beacon and probe response to set some flags. Later while pruning
 * join targets we prune the AP from join target list.
 * 3) As we go for assoc scan before initial assoc or reassoc steps taken in #2 above
 * take cares of not sending Assoc Req or Reassoc Req.
 * 4) Targeted or normal scan uses Probe Request with broadcast address for scan.
 * Hence, we dont have to take care Assoc Disallowed attr in case of normal probe request.
 * But, if we do unicast probe request  to associated AP we drop sending probe req
 * at wlc_sendprobe()
 */
static int
wlc_mbo_ie_parse_fn(void *ctx, wlc_iem_parse_data_t *data)
{
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)ctx;
	wlc_bsscfg_t *bsscfg = data->cfg;
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	wifi_mbo_ap_cap_ind_attr_t *ap_cap_attr = NULL;
	wifi_mbo_assoc_disallowed_attr_t *assoc_dis = NULL;

	ASSERT(BSSCFG_STA(data->cfg));
	/* validate minimum IE length */
	if (data->ie_len <= MBO_IE_HDR_SIZE) {
		return BCME_OK;
	}
	/* There can be one/more attribute within same MBO IE. To be safe parse */
	if (data->ie) {
		/* AP capability indication attribute */
		ap_cap_attr = (wifi_mbo_ap_cap_ind_attr_t *)
			bcm_parse_tlvs(data->ie + MBO_IE_HDR_SIZE, data->ie_len - MBO_IE_HDR_SIZE,
			MBO_ATTR_MBO_AP_CAPABILITY);

		/* Association Disallowed attribute */
		assoc_dis = (wifi_mbo_assoc_disallowed_attr_t *)
			bcm_parse_tlvs(data->ie + MBO_IE_HDR_SIZE, data->ie_len - MBO_IE_HDR_SIZE,
			MBO_ATTR_ASSOC_DISALLOWED);
	}

	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);
	switch (data->ft) {
		case WLC_IEM_FC_SCAN_BCN:
		case WLC_IEM_FC_SCAN_PRBRSP:
		{
			/* Pass it back in data->pparm->ft->scan.result */
			wlc_iem_ft_pparm_t *ftpparm = data->pparm->ft;
			wlc_bss_info_t *bi = ftpparm->scan.result;
			/* As per WFA MBO Interoperability Test Plan v0.0.26, section 2.3.2
			 * STA Test Bed Requirements, the test bed STA must be able to trigger
			 * an association request even when the AP is disallowing
			 * association request.
			 */
			if (!MBO_FLAG_IS_BIT_SET(mbc->flags, MBO_FLAG_FORCE_ASSOC_TO_AP)) {
				wlc_mbo_update_assoc_disallowed(bi, assoc_dis);
			}
			wlc_mbo_update_ap_cap(bi, ap_cap_attr);
		}
		break;
		case FC_ASSOC_RESP:
		case FC_REASSOC_RESP:
		{
			if (ap_cap_attr &&
				(ap_cap_attr->cap_ind & MBO_AP_CAP_IND_CELLULAR_AWARE)) {
				mbc->flags |= MBO_FLAG_AP_CELLULAR_AWARE;
			}
		}
		break;
		case FC_BEACON:
		case FC_PROBE_RESP:
		{
			wlc_bss_info_t *bi = bsscfg->current_bss;

			/* As per WFA MBO Interoperability Test Plan v0.0.26, section 2.3.2
			 * STA Test Bed Requirements, the test bed STA must be able to trigger
			 * an association request even when the AP is disallowing
			 * association request.
			 */
			if (!MBO_FLAG_IS_BIT_SET(mbc->flags, MBO_FLAG_FORCE_ASSOC_TO_AP)) {
				wlc_mbo_update_assoc_disallowed(bi, assoc_dis);
			}
			wlc_mbo_update_ap_cap(bi, ap_cap_attr);

		}
		break;
		default:
		{
			/* we should not be here */
			ASSERT(0);
		}
	}
	return BCME_OK;
}

#ifdef WL_MBO_WFA_CERT
/* Function to update forceful assoc attempt to AP
 * which is not accepting new connection
 */
static int
wlc_mbo_set_force_assoc(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 value)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	if (mbo == NULL || bsscfg == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	/* update force association value */
	if (value) {
		MBO_FLAG_BIT_SET(mbc->flags, MBO_FLAG_FORCE_ASSOC_TO_AP);
	} else {
		MBO_FLAG_BIT_RESET(mbc->flags, MBO_FLAG_FORCE_ASSOC_TO_AP);
	}
	/* reset already set flag in current BSS if we are going for force attempt */
	if (value) {
		bsscfg->current_bss->bcnflags &= ~WLC_BSS_MBO_ASSOC_DISALLOWED;
	}
	return BCME_OK;
}

/* "wl mbo force_assoc <>" handler */
static int
wlc_mbo_iov_set_force_assoc(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint8 data = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;

	UNUSED_PARAMETER(mbo);
	ret = bcm_unpack_xtlv_entry((uint8 **)&ibuf, WL_MBO_XTLV_ENABLE, sizeof(data),
			&data, BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: unpacking xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	ret = wlc_mbo_set_force_assoc(dig->cmd_ctx, dig->bsscfg, data);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: set force assoc failed\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
fail:
	return ret;
}

/* Function to get force assoc value */
static int
wlc_mbo_get_force_assoc(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 *value)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;

	if (mbo == NULL || bsscfg == NULL || value == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	/* return force association value */
	*value = MBO_FLAG_IS_BIT_SET(mbc->flags, MBO_FLAG_FORCE_ASSOC_TO_AP);

	return BCME_OK;
}

/* "wl mbo force_assoc" handler */
static int
wlc_mbo_iov_get_force_assoc(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint16 xtlv_size = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	uint16 buflen = 0;
	uint8 value = 0;

	if (mbo == NULL) {
		return BCME_BADARG;
	}
	xtlv_size = bcm_xtlv_size_for_data(sizeof(value),
		BCM_XTLV_OPTION_ALIGN32);
	if (xtlv_size > *olen) {
		*olen = 0;
		ret = BCME_BUFTOOSHORT;
		goto fail;
	}
	ret = wlc_mbo_get_force_assoc(mbo, dig->bsscfg, &value);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: failed to get force assoc value\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		return ret;
	}
	buflen = *olen;
	ret = bcm_pack_xtlv_entry(&obuf, &buflen, WL_MBO_XTLV_ENABLE,
			sizeof(value), &value, BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: packing xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	*olen = *olen - buflen;

fail:
	return ret;
}

int
wlc_mbo_set_bsstrans_reject(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 enable, uint8 reason)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	if (mbo == NULL || bsscfg == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	/* update bsstrans reject and reason value */
	if (enable) {
		MBO_FLAG_BIT_SET(mbc->flags, MBO_FLAG_FORCE_BSSTRANS_REJECT);
	} else {
		MBO_FLAG_BIT_RESET(mbc->flags, MBO_FLAG_FORCE_BSSTRANS_REJECT);
	}
	mbc->bsstrans_reject_reason = reason;

	return BCME_OK;
}

/* "wl mbo bsstrans_reject <>" handler */
static int
wlc_mbo_iov_set_bsstrans_reject(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint8 enable = 0, reason = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;

	UNUSED_PARAMETER(mbo);
	ret = bcm_unpack_xtlv_entry((uint8 **)&ibuf, WL_MBO_XTLV_ENABLE, sizeof(enable),
			&enable, BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: unpacking xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	if (enable) {
		ret = bcm_unpack_xtlv_entry((uint8 **)&ibuf,
			WL_MBO_XTLV_REASON_CODE, sizeof(reason),
			&reason, BCM_XTLV_OPTION_ALIGN32);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d: %s: unpacking xtlv failed\n",
				mbo->wlc->pub->unit, __FUNCTION__));
			goto fail;
		}
	}
	ret = wlc_mbo_set_bsstrans_reject(dig->cmd_ctx, dig->bsscfg, enable, reason);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: set bsstrans reject failed\n",
			mbo->wlc->pub->unit, __FUNCTION__, MALLOCED(mbo->wlc->osh)));
		goto fail;
	}
fail:
	return ret;
}

int
wlc_mbo_get_bsstrans_reject(wlc_mbo_info_t *mbo, wlc_bsscfg_t *bsscfg,
	uint8 *enable, uint8 *reason)
{
	wlc_mbo_bsscfg_cubby_t *mbc = NULL;
	if (mbo == NULL || bsscfg == NULL) {
		return BCME_BADARG;
	}
	mbc = MBO_BSSCFG_CUBBY(mbo, bsscfg);
	ASSERT(mbc != NULL);

	/* return bsstrans reject and reason value */
	*enable = MBO_FLAG_IS_BIT_SET(mbc->flags, MBO_FLAG_FORCE_BSSTRANS_REJECT);
	*reason = mbc->bsstrans_reject_reason;

	return BCME_OK;
}

/* "wl mbo bsstrans_reject" handler */
static int
wlc_mbo_iov_get_bsstrans_reject(const bcm_iov_cmd_digest_t *dig, const uint8 *ibuf,
	size_t ilen, uint8 *obuf, size_t *olen)
{
	int ret = BCME_OK;
	uint16 xtlv_size = 0;
	wlc_mbo_info_t *mbo = (wlc_mbo_info_t *)dig->cmd_ctx;
	uint16 buflen = 0;
	uint8 enable = 0, reason = 0;

	if (mbo == NULL) {
		return BCME_BADARG;
	}
	xtlv_size = bcm_xtlv_size_for_data(sizeof(enable),
		BCM_XTLV_OPTION_ALIGN32);
	if (enable) {
		xtlv_size = bcm_xtlv_size_for_data(sizeof(reason),
			BCM_XTLV_OPTION_ALIGN32);
	}
	if (xtlv_size > *olen) {
		*olen = 0;
		ret = BCME_BUFTOOSHORT;
		goto fail;
	}
	ret = wlc_mbo_get_bsstrans_reject(mbo, dig->bsscfg, &enable, &reason);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: failed to get basstrans reject values\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		return ret;
	}
	buflen = *olen;
	ret = bcm_pack_xtlv_entry(&obuf, &buflen, WL_MBO_XTLV_ENABLE,
			sizeof(enable), &enable, BCM_XTLV_OPTION_ALIGN32);
	if (ret != BCME_OK) {
		WL_MBO_ERR(("wl%d: %s: packing xtlv failed\n",
			mbo->wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	if (enable) {
		ret = bcm_pack_xtlv_entry(&obuf, &buflen, WL_MBO_XTLV_REASON_CODE,
				sizeof(reason), &reason, BCM_XTLV_OPTION_ALIGN32);
		if (ret != BCME_OK) {
			WL_MBO_ERR(("wl%d: %s: packing xtlv failed\n",
				mbo->wlc->pub->unit, __FUNCTION__));
			goto fail;
		}
	}
	*olen = *olen - buflen;

fail:
	return ret;
}
#endif /* WL_MBO_WFA_CERT */
#endif /* STA */
