/*
 * MAC debug and print functions
 * Broadcom 802.11abg Networking Device Driver
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

#include <wlc_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <sbchipc.h>
#include <bcmendian.h>
#include <wlc_types.h>
#include <wlioctl.h>
#include <proto/802.11.h>
#include <d11.h>
#include <hnddma.h>
#include <wlc_pub.h>
#include <wlc.h>
#include <wlc_bmac.h>
#include <wlc_ampdu.h>
#include <wlc_macdbg.h>
#include <wlc_bsscfg.h>
#include <hndpmu.h>
#include <bcmdevs.h>
#include <wlc_hw.h>
#include <wlc_event_utils.h>
#include <wlc_event.h>
#include <phy_dbg_api.h>
#include "d11reglist.h"
#include <wlc_tx.h>
#include <wlc_dump.h>
#include <wlc_iocv.h>

#ifdef WLVASIP
#include <phy_vasip_api.h>
#include <wlc_hw_priv.h>
#include <d11vasip_code.h>
#define SVMP_ACCESS_VIA_PHYTBL
#endif	/* WLVASIP */

#define SC_NUM_OPTNS_GE50	4
#define SC_OPTN_LT50NA		0

/* frameid tracing */
typedef struct pkt_hist pkt_hist_t;
typedef struct txs_hist txs_hist_t;
typedef struct sync_hist sync_hist_t;

/* Module private states */
struct wlc_macdbg_info {
	wlc_info_t *wlc;
	uint16 smpl_ctrl;	/* Sample Capture / Play Contrl */
	void *smpl_info;	/* Sample Capture Setup Params */
	CONST d11regs_list_t *pd11regs; /* dump register list */
	CONST d11regs_list_t *pd11regs_x; /* dump register list for second core if exists */
	uint d11regs_sz;
	uint d11regsx_sz;
	uint log_done;		/* reason bitmap */
	/* frameid tracing */
	pkt_hist_t *pkt_hist;
	int pkt_hist_cnt;
	txs_hist_t *txs_hist;
	int txs_hist_cnt;
	sync_hist_t *sync_hist;
	int sync_hist_next;
	int sync_hist_cnt;
};

/* this is for dump_ucode_fatal */
typedef struct _d11print_list {
	char name[16]; /* maximum 16 chars */
	uint16 addr;
} d11print_list_t;

/* this is for dump_shmem */
typedef struct _shmem_list {
	uint16	start;
	uint16	cnt;
} shmem_list_t;

/* ucode dump constant */
#define BCM4345_BCM4350_BUF_SIZE_FOR_PMU_DUMP	400
#define BCM4345_BCM4350_BUF_SIZE_FOR_UCODE_DUMP	12000

#define	PRVAL(name)	bcm_bprintf(b, "%s %u ", #name, WLCNTVAL(cnt->name))
#define	PRNL()		bcm_bprintf(b, "\n")
#define PRVAL_RENAME(varname, prname)	\
	bcm_bprintf(b, "%s %u ", #prname, WLCNTVAL(cnt->varname))

#define	PRREG(name)	bcm_bprintf(b, #name " 0x%x ", R_REG(wlc->osh, &regs->name))
#define PRREG_INDEX(name, reg) bcm_bprintf(b, #name " 0x%x ", R_REG(wlc->osh, &reg))

typedef enum {
	 SMPL_CAPTURE_GE50 = 0,
	 SMPL_CAPTURE_LT50 = 1
} smpl_capture_corerev_t;

/* Compile flag validity check */
#if defined(WLC_SRAMPMAC) || defined(WLC_HOSTPMAC)
#error "WLC_SRAMPMAC and WLC_HOSTPMAC are only for DONGLEBUILD!"
#if defined(WLC_SRAMPMAC) && defined(WLC_HOSTPMAC)
#error "WLC_SRAMPMAC and WLC_HOSTPMAC should be mutually exclusive!"
#endif
#endif /* WLC_SRAMPMAC || WLC_HOSTPMAC */

static int wlc_macdbg_doiovar(void *hdl, uint32 actionid,
	void *params, uint p_len, void *arg, uint len, uint vsize, struct wlc_if *wlcif);
static int wlc_macdbg_doioctl(void *ctx, uint32 cmd, void *arg, uint len, struct wlc_if *wlcif);

static int wlc_macdbg_up(void *hdl);
#if WL_MACDBG || defined(WLC_SRAMPMAC) || defined(WLC_HOSTPMAC)
static int wlc_macdbg_init_dumplist(wlc_macdbg_info_t *macdbg);
#endif /* WL_MACDBG || WLC_SRAMPMAC || WLC_HOSTPMAC */
#if WL_MACDBG
static int wlc_write_d11reg(wlc_info_t *wlc, int idx, int type, uint16 addr, uint32 w_val);
static int wlc_dump_mac(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_dump_shmem(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_dump_sctpl(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_dump_bcntpls(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_dump_pio(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_macdbg_pmac(wlc_info_t *wlc, wl_macdbg_pmac_param_t *pmac,
	char *out_buf, int out_len);
/* MAC Sample Capture */
static void wlc_macdbg_smpl_capture_optnreg(wlc_info_t *wlc,
	uint8 *reg_addr, uint32 *val, int reg_size, bool set);
static int wlc_macdbg_smpl_capture_optns(wlc_info_t *wlc,
	wl_maccapture_params_t *params, bool set);
static int wlc_macdbg_smpl_capture_set(wlc_info_t *wlc,
	wl_maccapture_params_t *params);
static int wlc_macdbg_smpl_capture_get(wlc_info_t *wlc,
	char *outbuf, uint outlen);
#else
#define wlc_write_d11reg(a, b, c, d, e) 0
#endif /* WL_MACDBG */
#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static int wlc_dump_dma(wlc_info_t *wlc, struct bcmstrbuf *b);
static int wlc_dump_stats(wlc_info_t *wlc, struct bcmstrbuf *b);
#endif

#ifdef WLC_MACDBG_FRAMEID_TRACE
static int wlc_macdbg_frameid_trace_attach(wlc_macdbg_info_t *macdbg, wlc_info_t *wlc);
static void wlc_macdbg_frameid_trace_detach(wlc_macdbg_info_t *macdbg, wlc_info_t *wlc);
#endif

static void wlc_suspend_mac_debug(wlc_info_t *wlc, uint32 phydebug);

void check_ta(wlc_info_t* wlc, bool ta_ok);

/** iovar table */
enum {
	IOV_MACDBG_PMAC = 0,		/* print mac */
	IOV_MACDBG_CAPTURE = 1,		/* MAC Sample Capture */
	IOV_SRCHMEM = 2,		/* ucode search engine memory */
	IOV_DBGSEL = 3,
	IOV_MACDBG_SHMX = 4,	/* set/get shmemx */
	IOV_MACDBG_REGX = 5,	/* set/get psmx regs */
	IOV_LAST
};

static const bcm_iovar_t macdbg_iovars[] = {
	{"pmac", IOV_MACDBG_PMAC, (0), 0, IOVT_BUFFER, 0},
	{"mac_capture", IOV_MACDBG_CAPTURE, (0), 0, IOVT_BUFFER, 0},
#if defined(BCMDBG) && defined(MBSS)
	{"srchmem", IOV_SRCHMEM,
	(IOVF_SET_UP | IOVF_GET_UP), 0, IOVT_BUFFER, DOT11_MAX_SSID_LEN + 2 * sizeof(uint32)},
#endif	/* BCMDBG && MBSS */
	{"shmemx", IOV_MACDBG_SHMX, (IOVF_SET_CLK | IOVF_GET_CLK), 0, IOVT_BUFFER, 0},
	{"macregx", IOV_MACDBG_REGX, (IOVF_SET_CLK | IOVF_GET_CLK), 0, IOVT_BUFFER, 0},
	{NULL, 0, 0, 0, 0, 0}
};

#if defined(STBLINUX) && defined(WLC_DUMP_MAC_EXT)
#define DUMPMAC_BUF_SZ	 	(128*1024)
#define DUMPMAC_FILENAME_SZ 	(32)
#define DUMPMAC_PRINT_SZ 	(128)
int wlc_dump_mac_file(const char * file_name, uint32 flags, uint8 *buf, int size);
int wlc_macdbg_dumpmac(wlc_info_t *wlc, char *buf, int buflen,
	int *outbuflen, bool dump_x);
void wlc_dump_mac_ext(wlc_info_t *wlc, uint reason);
#endif /* STBLINUX && WLC_DUMP_MAC_EXT*/

/* This includes the auto generated ROM IOCTL/IOVAR patch handler C source file (if auto patching is
 * enabled). It must be included after the prototypes and declarations above (since the generated
 * source file may reference private constants, types, variables, and functions).
 */
#include <wlc_patch.h>

void
BCMATTACHFN(wlc_macdbg_detach)(wlc_macdbg_info_t *macdbg)
{
	wlc_info_t *wlc;

	if (!macdbg)
		return;
	wlc = macdbg->wlc;

	wlc_module_unregister(wlc->pub, "macdbg", macdbg);

	(void)wlc_module_remove_ioctl_fn(wlc->pub, macdbg);

#if WL_MACDBG
	if (macdbg->smpl_info) {
		MFREE(wlc->osh, macdbg->smpl_info, sizeof(wl_maccapture_params_t));
	}
#endif
#if WL_MACDBG || defined(WLC_SRAMPMAC) || defined(WLC_HOSTPMAC)
	macdbg->pd11regs = NULL;
	macdbg->d11regs_sz = 0;
	macdbg->pd11regs_x = NULL;
	macdbg->d11regsx_sz = 0;
#endif /* WL_MACDBG || WLC_SRAMPMAC || WLC_HOSTPMAC */

#ifdef WLC_MACDBG_FRAMEID_TRACE
	wlc_macdbg_frameid_trace_detach(macdbg, wlc);
#endif

	MFREE(wlc->osh, macdbg, sizeof(*macdbg));
}

wlc_macdbg_info_t *
BCMATTACHFN(wlc_macdbg_attach)(wlc_info_t *wlc)
{
	wlc_pub_t *pub = wlc->pub;
	wlc_macdbg_info_t *macdbg;

	if ((macdbg = MALLOCZ(wlc->osh, sizeof(wlc_macdbg_info_t))) == NULL) {
		WL_ERROR(("wl%d: %s: macdbg memory alloc. failed\n",
			wlc->pub->unit, __FUNCTION__));
		return NULL;
	}
	macdbg->wlc = wlc;

#ifdef WLC_MACDBG_FRAMEID_TRACE
	if (wlc_macdbg_frameid_trace_attach(macdbg, wlc) != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_macdbg_frameid_trace_attach failed\n",
			wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
#endif

	if (wlc_module_add_ioctl_fn(pub, macdbg, wlc_macdbg_doioctl, 0, NULL) != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_module_add_ioctl_fn() failed\n",
		          wlc->pub->unit, __FUNCTION__));
		goto fail;
	}

	if ((wlc_module_register(pub, macdbg_iovars, "macdbg",
		macdbg, wlc_macdbg_doiovar, NULL, wlc_macdbg_up, NULL)) != BCME_OK) {
		WL_ERROR(("wl%d: %s: wlc_module_register() failed\n",
			wlc->pub->unit, __FUNCTION__));
		goto fail;
	}

#if WL_MACDBG || defined(WLC_SRAMPMAC) || defined(WLC_HOSTPMAC)
	if (wlc_macdbg_init_dumplist(macdbg) != BCME_OK) {
		goto fail;
	}
#endif /* WL_MACDBG || WLC_SRAMPMAC || WLC_HOSTPMAC */

#if WL_MACDBG
	if ((macdbg->smpl_info = MALLOCZ(wlc->osh, sizeof(wl_maccapture_params_t))) == NULL) {
		WL_ERROR(("wl%d: %s: smp_info memory alloc. failed\n",
				wlc->pub->unit, __FUNCTION__));
		goto fail;
	}
	wlc_dump_register(pub, "mac", (dump_fn_t)wlc_dump_mac, (void *)wlc);
	wlc_dump_register(pub, "shmem", (dump_fn_t)wlc_dump_shmem, (void *)wlc);
	wlc_dump_register(pub, "sctpl", (dump_fn_t)wlc_dump_sctpl, (void *)wlc);
	wlc_dump_register(pub, "bcntpl", (dump_fn_t)wlc_dump_bcntpls, (void *)wlc);
	wlc_dump_register(pub, "pio", (dump_fn_t)wlc_dump_pio, (void *)wlc);
#endif /* WL_MACDBG */
#if defined(BCMDBG) || defined(BCMDBG_DUMP)
	wlc_dump_register(pub, "dma", (dump_fn_t)wlc_dump_dma, (void *)wlc);
	wlc_dump_register(pub, "stats", (dump_fn_t)wlc_dump_stats, (void *)wlc);
#endif /* BCMDBG || BCMDBG_DUMP */

	return macdbg;
fail:
	wlc_macdbg_detach(macdbg);
	return NULL;
}

/* add dump enum here */
static int
wlc_macdbg_doiovar(void *hdl, uint32 actionid,
	void *params, uint p_len, void *arg, uint len, uint vsize, struct wlc_if *wlcif)
{
	wlc_macdbg_info_t *macdbg = (wlc_macdbg_info_t*)hdl;
	wlc_info_t *wlc = macdbg->wlc;
	int32 int_val = 0, *ret_int_ptr;
	int err = BCME_OK;

	BCM_REFERENCE(wlc);
	BCM_REFERENCE(wlcif);
	BCM_REFERENCE(int_val);
	BCM_REFERENCE(ret_int_ptr);

	ASSERT(macdbg == wlc->macdbg);

	/* convenience int and bool vals for first 8 bytes of buffer */
	if (p_len >= (int)sizeof(int_val))
		bcopy(params, &int_val, sizeof(int_val));

	ret_int_ptr = (int32 *)arg;

	switch (actionid) {
#if WL_MACDBG
	case IOV_GVAL(IOV_MACDBG_PMAC):
		err = wlc_macdbg_pmac(wlc, params, arg, len);
		break;

	case IOV_GVAL(IOV_MACDBG_CAPTURE):
		err = wlc_macdbg_smpl_capture_get(wlc, arg, (uint)len);
		break;

	case IOV_SVAL(IOV_MACDBG_CAPTURE):
	{
		wl_maccapture_params_t *maccapture_params = (wl_maccapture_params_t *)arg;

		if (len < (int)sizeof(wl_maccapture_params_t)) {
			err = BCME_BUFTOOSHORT;
			break;
		}

		err = wlc_macdbg_smpl_capture_set(wlc, maccapture_params);
		break;
	}
#if defined(WL_PSMX)
	case IOV_GVAL(IOV_MACDBG_SHMX):
	{
		rw_reg_t *rwt = params;
		if (rwt->size != 2) {
			err = BCME_BADLEN;
			break;
		}
		if (rwt->byteoff & (rwt->size - 1)) {
			err = BCME_BADADDR;
			break;
		}
		ASSERT(PSMX_HWCAP(wlc->pub));
		*ret_int_ptr = wlc_read_shmx(wlc, rwt->byteoff);
		break;
	}
	case IOV_SVAL(IOV_MACDBG_SHMX):
	{
		rw_reg_t *rwt = params;
		if (rwt->size != 2) {
			err = BCME_BADLEN;
			break;
		}
		if (rwt->byteoff & (rwt->size - 1)) {
			err = BCME_BADADDR;
			break;
		}
		ASSERT(PSMX_HWCAP(wlc->pub));
		wlc_write_shmx(wlc, rwt->byteoff, (uint16)rwt->val);
		break;
	}
	case IOV_GVAL(IOV_MACDBG_REGX):
	{
		rw_reg_t *rwt = params;
		if (rwt->size != 2 && rwt->size != 4) {
			err = BCME_BADLEN;
			break;
		}
		if (rwt->byteoff & (rwt->size - 1)) {
			err = BCME_BADADDR;
			break;
		}
		if (rwt->byteoff < D11REG_IHR_BASE) {
			*ret_int_ptr = R_REG(wlc->osh,
				(volatile uint32*)((uint8*)((uintptr)wlc->regs) + rwt->byteoff));
		} else {
			ASSERT(PSMX_HWCAP(wlc->pub));
			*ret_int_ptr = wlc_read_macregx(wlc, rwt->byteoff);
		}
		break;
	}
	case IOV_SVAL(IOV_MACDBG_REGX):
	{
		rw_reg_t *rwt = params;
		if (rwt->size != 2 && rwt->size != 4) {
			err = BCME_BADLEN;
			break;
		}
		if (rwt->byteoff & (rwt->size - 1)) {
			err = BCME_BADADDR;
			break;
		}
		if (rwt->byteoff < D11REG_IHR_BASE) {
			W_REG(wlc->osh,
				(volatile uint32*)((uint8*)((uintptr)wlc->regs) + rwt->byteoff),
				rwt->val);
		} else {
			ASSERT(PSMX_HWCAP(wlc->pub));
			wlc_write_macregx(wlc, rwt->byteoff, (uint16)rwt->val);
		}
		break;
	}
#endif	/* WL_PSMX */
#endif /* WL_MACDBG */

#if defined(BCMDBG) && defined(MBSS)
	case IOV_GVAL(IOV_SRCHMEM): {
		uint reg = (uint)int_val;

		if (reg > 15) {
			err = BCME_BADADDR;
			break;
		}

		wlc_bmac_copyfrom_objmem(wlc->hw, reg * SHM_MBSS_SSIDSE_BLKSZ(wlc),
			arg, SHM_MBSS_SSIDSE_BLKSZ(wlc), OBJADDR_SRCHM_SEL);
		break;
	}
	case IOV_SVAL(IOV_SRCHMEM): {
		uint reg = (uint)int_val;
		int *parg = (int *)arg;

		/* srchmem set params:
		 *	uint32		register num
		 *	uint32		ssid len
		 *	int8[32]	ssid name
		 */

		if (reg > 15) {
			err = BCME_BADADDR;
			break;
		}

		wlc_bmac_copyto_objmem(wlc->hw, reg * SHM_MBSS_SSIDSE_BLKSZ(wlc),
			(parg + 1), SHM_MBSS_SSIDSE_BLKSZ(wlc), OBJADDR_SRCHM_SEL);
		break;
	}
#endif	/* BCMDBG && MBSS */

#ifdef BCMINTDBG
	case IOV_GVAL(IOV_DBGSEL):
		*ret_int_ptr = (int32)wlc_bmac_ucode_dbgsel_get(wlc->hw);
		break;

	case IOV_SVAL(IOV_DBGSEL):
		/* Starting from rev14 MAC has extra 2 bits for GPIOSel in MACControl1 register */
		if (int_val > 0xF) {
			err = BCME_RANGE;
		} else {
			wlc_bmac_ucode_dbgsel_set(wlc->hw, (uint16)int_val);
		}
		break;
#endif /* BCMINTDBG */

	default:
		err = BCME_UNSUPPORTED;
		break;
	}
	return err;
}

/* ioctl dispatcher */
static int
wlc_macdbg_doioctl(void *ctx, uint32 cmd, void *arg, uint len, struct wlc_if *wlcif)
{
	int bcmerror = BCME_OK;
	wlc_macdbg_info_t *macdbg = ctx;
	wlc_info_t *wlc = macdbg->wlc;
	int val, *pval;
	d11regs_t *regs = wlc->regs;
	uint band = 0;
	osl_t *osh = wlc->osh;
	uint i;
	bool ta_ok = FALSE;

	BCM_REFERENCE(ta_ok);
	BCM_REFERENCE(macdbg);
	BCM_REFERENCE(regs);
	BCM_REFERENCE(i);
	BCM_REFERENCE(band);
	BCM_REFERENCE(osh);

	/* default argument is generic integer */
	pval = (int *) arg;

	/* This will prevent the misaligned access */
	if ((uint32)len >= sizeof(val))
		bcopy(pval, &val, sizeof(val));
	else
		val = 0;

	switch (cmd) {

#if defined(BCMDBG) || defined(BCMQT)
	case WLC_GET_UCFLAGS:
		if (!wlc->pub->up) {
			bcmerror = BCME_NOTUP;
			break;
		}

		/* optional band is stored in the second integer of incoming buffer */
		band = (len < (int)(2 * sizeof(int))) ? WLC_BAND_AUTO : ((int *)arg)[1];

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;
#ifdef WL_PSMX
		if (PSMX_HWCAP(wlc->pub) && (val >= MXHF0)) {
			if (val >= MXHF0+MXHFMAX) {
				bcmerror = BCME_RANGE;
				break;
			}
		} else
#endif /* WL_PSMX  */
		if (val >= MHFMAX) {
			bcmerror = BCME_RANGE;
			break;
		}

		*pval = wlc_bmac_mhf_get(wlc->hw, (uint8)val, WLC_BAND_AUTO);
		break;

	case WLC_SET_UCFLAGS:
		if (!wlc->pub->up) {
			bcmerror = BCME_NOTUP;
			break;
		}

		/* optional band is stored in the second integer of incoming buffer */
		band = (len < (int)(2 * sizeof(int))) ? WLC_BAND_AUTO : ((int *)arg)[1];

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;
		i = (uint16)val;
#ifdef WL_PSMX
		if (PSMX_HWCAP(wlc->pub) && (i >= MXHF0)) {
			if (i >= MXHF0+MXHFMAX) {
				bcmerror = BCME_RANGE;
				break;
			}
		} else
#endif /* WL_PSMX  */
		if (i >= MHFMAX) {
			bcmerror = BCME_RANGE;
			break;
		}

		wlc_mhf(wlc, (uint8)i, 0xffff, (uint16)(val >> NBITS(uint16)), WLC_BAND_AUTO);
		break;

	case WLC_GET_SHMEM:
		ta_ok = TRUE;

		/* optional band is stored in the second integer of incoming buffer */
		band = (len < (int)(2 * sizeof(int))) ? WLC_BAND_AUTO : ((int *)arg)[1];

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;

		if (val & 1) {
			bcmerror = BCME_BADADDR;
			break;
		}

		*pval = wlc_read_shm(wlc, (uint16)val);
		break;

	case WLC_SET_SHMEM:
		ta_ok = TRUE;

		/* optional band is stored in the second integer of incoming buffer */
		band = (len < (int)(2 * sizeof(int))) ? WLC_BAND_AUTO : ((int *)arg)[1];

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;

		if (val & 1) {
			bcmerror = BCME_BADADDR;
			break;
		}

		wlc_write_shm(wlc, (uint16)val, (uint16)(val >> NBITS(uint16)));
		break;

	case WLC_W_REG:
	{
		rw_reg_t *r;
		ta_ok = TRUE;
		r = (rw_reg_t*)arg;
		band = WLC_BAND_AUTO;

		if (len < (int)(sizeof(rw_reg_t) - sizeof(uint))) {
			bcmerror = BCME_BUFTOOSHORT;
			break;
		}

		if (len >= (int)sizeof(rw_reg_t))
			band = r->band;

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;

		if (r->byteoff + r->size > sizeof(d11regs_t)) {
			bcmerror = BCME_BADADDR;
			break;
		}
		if (r->size == sizeof(uint32))
			W_REG(osh, (uint32 *)((uchar *)(uintptr)regs + r->byteoff), r->val);
		else if (r->size == sizeof(uint16))
			W_REG(osh, (uint16 *)((uchar *)(uintptr)regs + r->byteoff), r->val);
		else
			bcmerror = BCME_BADADDR;
		break;
	}
#endif /* BCMDBG || BCMQT */
#if defined(BCMDBG) || defined(BCMQT) || defined(BCMDBG_ERR) || defined(WLTEST)
	case WLC_R_REG:	/* MAC registers */
	{
		rw_reg_t *r;
#if defined(BCMDBG) || defined(BCMQT)
		ta_ok = TRUE;
#endif /* BCMDBG || BCMQT */
		r = (rw_reg_t*)arg;
		band = WLC_BAND_AUTO;

		if (len < (int)(sizeof(rw_reg_t) - sizeof(uint))) {
			bcmerror = BCME_BUFTOOSHORT;
			break;
		}

		if (len >= (int)sizeof(rw_reg_t))
			band = r->band;

		/* HW is turned off so don't try to access it */
		if (wlc->pub->hw_off) {
			bcmerror = BCME_RADIOOFF;
			break;
		}

		/* bcmerror checking */
		if ((bcmerror = wlc_iocregchk(wlc, band)))
			break;

		if ((r->byteoff + r->size) > sizeof(d11regs_t)) {
			bcmerror = BCME_BADADDR;
			break;
		}
		if (r->size == sizeof(uint32))
			r->val = R_REG(osh, (uint32 *)((uchar *)(uintptr)regs + r->byteoff));
		else if (r->size == sizeof(uint16))
			r->val = R_REG(osh, (uint16 *)((uchar *)(uintptr)regs + r->byteoff));
		else
			bcmerror = BCME_BADADDR;
		break;
	}
#endif /* BCMDBG || BCMQT|| BCMDBG_ERR || WLTEST */
	default:
		bcmerror = BCME_UNSUPPORTED;
		break;
	}

	check_ta(wlc, ta_ok);

	return bcmerror;
} /* wlc_macdbg_doioctl */

void
check_ta(wlc_info_t* wlc, bool ta_ok)
{
#if defined(BCMDBG) || defined(BCMQT)
	/* The sequence is significant.
	 *   Only if sbclk is TRUE, we can proceed with register access.
	 *   Even though ta_ok is TRUE, we still want to check(and clear) target abort
	 *   si_taclear returns TRUE if there was a target abort, In this case, ta_ok must be TRUE
	 *   to avoid assert
	 *   ASSERT and si_taclear are both under ifdef BCMDBG
	 */
	if (!(wlc->pub->hw_off))
		ASSERT(wlc_bmac_taclear(wlc->hw, ta_ok) || !ta_ok);
#endif /* BCMDBG || BCMQT */
}

static int
wlc_macdbg_up(void *hdl)
{
	wlc_macdbg_info_t *macdbg = (wlc_macdbg_info_t *)hdl;
#if WL_MACDBG
	wlc_info_t *wlc = macdbg->wlc;

	/* If MAC Sample Capture is set-up, start */
	if (((macdbg->smpl_ctrl) & SC_STRT) && (macdbg->smpl_info)) {
		wlc_macdbg_smpl_capture_set(wlc, (wl_maccapture_params_t *)macdbg->smpl_info);
	}
#endif /* WL_MACDBG */

	macdbg->log_done = 0;

	return BCME_OK;
}

#if defined(WLC_HOSTPMAC)
/* Send d11 register lists up to DHD by event */
void
BCMATTACHFN(wlc_macdbg_sendup_d11regs)(wlc_macdbg_info_t *macdbg)
{
	wlc_info_t *wlc = macdbg->wlc;

	/* Enable MACDBG event */
	wlc_eventq_set_ind(wlc->eventq, WLC_E_MACDBG, TRUE);

	if (macdbg->pd11regs && macdbg->d11regs_sz > 0) {
		wlc_mac_event(wlc, WLC_E_MACDBG, NULL, WLC_E_STATUS_SUCCESS,
			WLC_E_MACDBG_LIST_PSM, 0, (void *)macdbg->pd11regs,
			(macdbg->d11regs_sz * sizeof(macdbg->pd11regs[0])));
	}

#if defined(WL_PSMX)
	if (PSMX_HWCAP(wlc->pub) && macdbg->pd11regs_x && macdbg->d11regsx_sz > 0) {
		wlc_mac_event(wlc, WLC_E_MACDBG, NULL, WLC_E_STATUS_SUCCESS,
			WLC_E_MACDBG_LIST_PSMX, 0, (void *)macdbg->pd11regs_x,
			(macdbg->d11regsx_sz * sizeof(macdbg->pd11regs_x[0])));
	}
#endif /* WL_PSMX */

#if !WL_MACDBG
	/* DHD will keep the list, and dongle will never need these again. */
	macdbg->pd11regs = NULL;
	macdbg->d11regs_sz = 0;
	macdbg->pd11regs_x = NULL;
	macdbg->d11regsx_sz = 0;
#endif
	return;
}
#endif /* WLC_HOSTPMAC */

#if WL_MACDBG || defined(WLC_SRAMPMAC) || defined(WLC_HOSTPMAC)
static int
BCMATTACHFN(wlc_macdbg_init_dumplist)(wlc_macdbg_info_t *macdbg)
{
	wlc_info_t *wlc = macdbg->wlc;
	uint32 corerev = wlc->pub->corerev;

#ifdef WLC_MINMACLIST
	if (D11REV_GE(corerev, 40)) {
		macdbg->pd11regs = d11regsmin_ge40;
		macdbg->d11regs_sz = d11regsmin_ge40sz;
	} else {
		macdbg->pd11regs = d11regsmin_pre40;
		macdbg->d11regs_sz = d11regsmin_pre40sz;
	}
	macdbg->pd11regs_x = NULL;
#else /* WLC_MINMACLIST */
	if (D11REV_IS(corerev, 23)) {
		macdbg->pd11regs = d11regs23;
		macdbg->d11regs_sz = d11regs23sz;
	}
	else if (D11REV_IS(corerev, 39)) {
		macdbg->pd11regs = d11regs39;
		macdbg->d11regs_sz = d11regs39sz;
	}
	else if (D11REV_LT(corerev, 40)) {
		macdbg->pd11regs = d11regs_pre40;
		macdbg->d11regs_sz = d11regs_pre40sz;
	}
	else if (D11REV_IS(corerev, 48)) {
		macdbg->pd11regs = d11regs48;
		macdbg->d11regs_sz = d11regs48sz;
	}
	else if (D11REV_IS(corerev, 49)) {
		macdbg->pd11regs = d11regs49;
		macdbg->d11regs_sz = d11regs49sz;
	}
	else if (D11REV_IS(corerev, 59)) {
		macdbg->pd11regs = d11regs59;
		macdbg->d11regs_sz = d11regs59sz;
	}
	else if (D11REV_IS(corerev, 61)) {
		macdbg->pd11regs = d11regs61;
		macdbg->d11regs_sz = d11regs61sz;
	}
	else if (D11REV_IS(corerev, 64)) {
		macdbg->pd11regs = d11regs64;
		macdbg->d11regs_sz = d11regs64sz;
	}
	else if (D11REV_GE(corerev, 65)) {
		macdbg->pd11regs = d11regs65;
		macdbg->d11regs_sz = d11regs65sz;
	}
	else {
		/* Default */
		macdbg->pd11regs = d11regs42;
		macdbg->d11regs_sz = d11regs42sz;
	}
#if defined(WL_PSMX)
	if (D11REV_IS(corerev, 64)) {
		macdbg->pd11regs_x = d11regsx64;
		macdbg->d11regsx_sz = d11regsx64sz;
	}
	else if (D11REV_GE(corerev, 65)) {
		macdbg->pd11regs_x = d11regsx65;
		macdbg->d11regsx_sz = d11regsx65sz;
	} else {
		macdbg->pd11regs_x = NULL;
		macdbg->d11regsx_sz = 0;
	}
#endif /* WL_PSMX */
#endif /* WLC_MINMACLIST */

	WL_TRACE(("%s d11reg %p size %d d11reg_x %p size %d\n", __FUNCTION__,
		OSL_OBFUSCATE_BUF(macdbg->pd11regs), macdbg->d11regs_sz,
		OSL_OBFUSCATE_BUF(macdbg->pd11regs_x), macdbg->d11regsx_sz));

	return BCME_OK;
}
#endif /* WL_MACDBG || WLC_SRAMPMAC || WLC_HOSTPMAC */

#if WL_MACDBG || defined(WLC_SRAMPMAC)
/* dump functions */
static int
wlc_print_d11reg(wlc_info_t *wlc, int idx, int type, uint16 addr, struct bcmstrbuf *b, uint8 **p)
{
	d11regs_t *regs;
	osl_t *osh;
	uint16 val16 = -1;
	uint32 val32 = -1;
	bool print16 = TRUE;
	volatile uint8 *paddr;
	const char *regname[D11REG_TYPE_MAX] = D11REGTYPENAME;

	osh = wlc->osh;
	regs = wlc->regs;
	paddr = (volatile uint8*)(&regs->biststatus) - 0xC;

	switch (type) {
	case D11REG_TYPE_IHR32:
		val32 = R_REG(osh, (volatile uint32*)(paddr + addr));
		print16 = FALSE;
		break;
	case D11REG_TYPE_IHR16:
		val16 = R_REG(osh, (volatile uint16*)(paddr + addr));
		break;
	case D11REG_TYPE_SCR:
		wlc_bmac_copyfrom_objmem(wlc->hw, addr << 2,
			&val16, sizeof(val16), OBJADDR_SCR_SEL);
		break;
	case D11REG_TYPE_SHM:
		val16 = wlc_read_shm(wlc, addr);
		break;
	case D11REG_TYPE_TPL:
		W_REG(osh, &regs->tplatewrptr, addr);
		val32 = R_REG(osh, &regs->tplatewrdata);
		print16 = FALSE;
		break;
	case D11REG_TYPE_KEYTB:
		wlc_bmac_copyfrom_objmem(wlc->hw, addr,
			&val32, sizeof(val32), OBJADDR_KEYTBL_SEL);
		print16 = FALSE;
		break;
#if defined(WL_PSMX)
	case D11REG_TYPE_IHRX16:
		ASSERT(PSMX_HWCAP(wlc->pub));
		val16 = wlc_read_macregx(wlc, addr);
		break;
	case D11REG_TYPE_SCRX:
		wlc_bmac_copyfrom_objmem(wlc->hw, addr << 2,
			&val16, sizeof(val16), OBJADDR_SCRX_SEL);
		break;
	case D11REG_TYPE_SHMX:
		wlc_bmac_copyfrom_objmem(wlc->hw, addr,
			&val16, sizeof(val16), OBJADDR_SHMX_SEL);
		break;
#endif /* WL_PSMX */
	default:
		if (b) {
			bcm_bprintf(b, "%s: unrecognized type %d!\n", __FUNCTION__, type);
		} else {
			printf("%s: unrecognized type %d!\n", __FUNCTION__, type);
		}
		return 0;
	}
	if (print16) {
		if (b) {
			bcm_bprintf(b, "%-3d %s 0x%-4x = 0x%-4x\n",
				idx, regname[type], addr, val16);
		} else if (!p) {
			printf("%-3d %s 0x%-4x = 0x%-4x\n",
			       idx, regname[type], addr, val16);
		}
		if (p) {
			*((uint16 *)(*p)) = val16;
			*p += sizeof(val16);
		}
	} else {
		if (b) {
			bcm_bprintf(b, "%-3d %s 0x%-4x = 0x%-8x\n",
				idx, regname[type], addr, val32);
		} else if (!p) {
			printf("%-3d %s 0x%-4x = 0x%-8x\n",
			       idx, regname[type], addr, val32);
		}
		if (p) {
			*((uint32 *)(*p)) = val32;
			*p += sizeof(val32);
		}
	}
	return 1;
}

static int
wlc_pw_d11regs(wlc_info_t *wlc, CONST d11regs_list_t *pregs,
	int start_idx, struct bcmstrbuf *b, uint8 **p,
	bool w_en, uint32 w_val)
{
	uint32 lbmp;
	uint16 addr, lcnt;
	int idx;

	addr = pregs->addr;
	idx = start_idx;
	if (pregs->type >= D11REG_TYPE_MAX) {
		if (b && !w_en) {
			bcm_bprintf(b, "%s: wrong type %d\n", __FUNCTION__, pregs->type);
		} else {
			printf("%s: wrong type %d\n", __FUNCTION__, pregs->type);
		}
		return 0;
	}
	lbmp = pregs->bitmap;
	lcnt = pregs->cnt;
	while (lbmp || lcnt) {
		WL_TRACE(("idx %d bitmap %#x cnt %d addr %#x\n",
			idx, lbmp, lcnt, addr));
		if ((lbmp && (lbmp & 0x1)) || (!lbmp && lcnt)) {
			if (w_en) {
				idx += wlc_write_d11reg(wlc, idx, pregs->type, addr, w_val);
			} else {
				idx += wlc_print_d11reg(wlc, idx, pregs->type, addr, b, p);
			}
			if (lcnt) lcnt --;
		}
		lbmp = lbmp >> 1;
		addr += pregs->step;
	}
	return (idx - start_idx);
}

static int
wlc_pd11regs_bylist(wlc_info_t *wlc, CONST d11regs_list_t *d11dbg1,
	uint d11dbg1_sz, int start_idx, struct bcmstrbuf *b, uint8 **p)
{
	CONST d11regs_list_t *pregs;
	int i, idx;

	if (!wlc->clk)
		return BCME_NOCLK;

	WL_INFORM(("%s: ucode compile time 0x%04x 0x%04x\n", __FUNCTION__,
		wlc_read_shm(wlc, 0x4), wlc_read_shm(wlc, 0x6)));

	idx = start_idx;
	for (i = 0; i < (int)d11dbg1_sz; i++) {
		pregs = &(d11dbg1[i]);
		idx += wlc_pw_d11regs(wlc, pregs, idx, b, p, FALSE, 0);
	}
	return (idx - start_idx);
}
#endif /* WL_MACDBG || WLC_SRAMPMAC */

#if WL_MACDBG
/* dump functions */
static int
wlc_write_d11reg(wlc_info_t *wlc, int idx, int type, uint16 addr, uint32 w_val)
{
	d11regs_t *regs;
	osl_t *osh;
	uint16 w_val16 = (uint16)w_val;
	volatile uint8 *paddr;

	osh = wlc->osh;
	regs = wlc->regs;
	paddr = (volatile uint8*)(&regs->biststatus) - 0xC;

	switch (type) {
	case D11REG_TYPE_IHR32:
		W_REG(osh, (volatile uint32*)(paddr + addr), w_val);
		break;
	case D11REG_TYPE_IHR16:
		W_REG(osh, (volatile uint16*)(paddr + addr), w_val16);
		break;
	case D11REG_TYPE_SCR:
		wlc_bmac_copyto_objmem(wlc->hw, addr << 2,
			&w_val16, sizeof(w_val16), OBJADDR_SCR_SEL);
		break;
	case D11REG_TYPE_SHM:
		wlc_write_shm(wlc, addr, w_val16);
		break;
	case D11REG_TYPE_TPL:
		W_REG(osh, &regs->tplatewrptr, addr);
		W_REG(osh, &regs->tplatewrdata, w_val);
		break;
	case D11REG_TYPE_KEYTB:
		wlc_bmac_copyto_objmem(wlc->hw, addr,
			&w_val, sizeof(w_val), OBJADDR_KEYTBL_SEL);
		break;
#if defined(WL_PSMX)
	case D11REG_TYPE_IHRX16:
		wlc_write_macregx(wlc, addr, w_val16);
		break;
	case D11REG_TYPE_SCRX:
		wlc_bmac_copyto_objmem(wlc->hw, addr << 2,
			&w_val16, sizeof(w_val16), OBJADDR_SCRX_SEL);
		break;
	case D11REG_TYPE_SHMX:
		wlc_bmac_copyto_objmem(wlc->hw, addr,
			&w_val16, sizeof(w_val16), OBJADDR_SHMX_SEL);
		break;
#endif /* WL_PSMX */
	default:
		printf("%s: unrecognized type %d!\n", __FUNCTION__, type);
		return 0;
	}
	return 1;
}

static int
wlc_pw_d11regs_byaddr(wlc_info_t *wlc, CONST d11regs_addr_t *d11addrs,
	uint d11addrs_sz, int start_idx, struct bcmstrbuf *b,
	bool w_en, uint32 w_val)
{
	CONST d11regs_addr_t *paddrs;
	int i, j, idx;

	if (!wlc->clk)
		return BCME_NOCLK;

	idx = start_idx;
	for (i = 0; i < (int)d11addrs_sz; i++) {
		paddrs = &(d11addrs[i]);
		if (paddrs->type >= D11REG_TYPE_MAX) {
			if (b && !w_en)
				bcm_bprintf(b, "%s: wrong type %d. Skip %d entries.\n",
					__FUNCTION__, paddrs->type, paddrs->cnt);
			else
				printf("%s: wrong type %d. Skip %d entries.\n",
				       __FUNCTION__, paddrs->type, paddrs->cnt);
			continue;
		}
		for (j = 0; j < paddrs->cnt; j++) {
			if (w_en) {
				idx += wlc_write_d11reg(wlc, idx, paddrs->type,
					paddrs->addr[j], w_val);
			} else {
				idx += wlc_print_d11reg(wlc, idx, paddrs->type,
					paddrs->addr[j], b, NULL);
			}
		}
	}
	return (idx - start_idx);
}

static int
wlc_dump_mac(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	int cnt = 0;
	if (!wlc->clk) {
		WL_ERROR(("wl%d: %s: clock must be on\n", wlc->pub->unit, __FUNCTION__));
		return BCME_NOCLK;
	}
	ASSERT(wlc->macdbg->pd11regs != NULL && wlc->macdbg->d11regs_sz > 0);
	cnt = wlc_pd11regs_bylist(wlc, wlc->macdbg->pd11regs,
		wlc->macdbg->d11regs_sz, 0, b, NULL);
	return cnt;
}

static int
wlc_macdbg_pmac(wlc_info_t *wlc,
	wl_macdbg_pmac_param_t *params, char *out_buf, int out_len)
{
	uint8 i, type;
	int err = BCME_OK, idx = 0;
	struct bcmstrbuf bstr, *b;
	wl_macdbg_pmac_param_t pmac_params;
	wl_macdbg_pmac_param_t *pmac = &pmac_params;
	d11regs_list_t d11dbg1;
	d11regs_addr_t d11dbg2;
	bool skip1st = FALSE;
	bool align4 = FALSE;

	memcpy(pmac, params, sizeof(wl_macdbg_pmac_param_t));
	bcm_binit(&bstr, out_buf, out_len);
	b = &bstr;

	if (WL_TRACE_ON()) {
		printf("%s:\n", __FUNCTION__);
		printf("type %s\n", pmac->type);
		printf("step %u\n", pmac->step);
		printf("num  %u\n", pmac->num);
		printf("bitmap %#x\n", pmac->bitmap);
		printf("addr_raw %d\n", pmac->addr_raw);
		for (i = 0; i < pmac->addr_num; i++)
			printf("\taddr = %#x\n", pmac->addr[i]);
	}

	if (pmac->addr_num == 0) {
		bcm_bprintf(b, "%s line %d: no address is given!\n", __FUNCTION__, __LINE__);
		//err = BCME_BADARG;
		goto exit;
	}

	if (!strncmp(pmac->type, "shmx", 4)) {
		type = D11REG_TYPE_SHMX;
	} else if (!strncmp(pmac->type, "ihrx", 4)) {
		type = D11REG_TYPE_IHRX16;
	} else if (!strncmp(pmac->type, "keytb", 5)) {
		type = D11REG_TYPE_KEYTB;
		align4 = TRUE;
	} else if (!strncmp(pmac->type, "scrx", 4)) {
		type = D11REG_TYPE_SCRX;
	} else if (!strncmp(pmac->type, "scr", 3)) {
		type = D11REG_TYPE_SCR;
	} else if (!strncmp(pmac->type, "shm", 3)) {
		type = D11REG_TYPE_SHM;
	} else if (!strncmp(pmac->type, "tpl", 3)) {
		type = D11REG_TYPE_TPL;
		align4 = TRUE;
	} else if (!strncmp(pmac->type, "ihr32", 5)) {
		type = D11REG_TYPE_IHR32;
		align4 = TRUE;
	} else if (!strncmp(pmac->type, "ihr", 3)) {
		type = D11REG_TYPE_IHR16;
	} else {
		bcm_bprintf(b, "Unrecognized type: %s!\n", pmac->type);
		err = BCME_BADARG;
		goto exit;
	}
	if (type >= D11REG_TYPE_GE64 && D11REV_LT(wlc->pub->corerev, 64)) {
		bcm_bprintf(b, "%s: unsupported type %s for corerev %d!\n",
			__FUNCTION__, pmac->type, wlc->pub->corerev);
		goto exit;
	}

	if (type == D11REG_TYPE_SCR || type == D11REG_TYPE_SCRX) {
		if (pmac->step == (uint8)(-1)) {
			/* Set the default step when it is not given */
			pmac->step = 1;
		}
	} else {
		uint16 mask = align4 ? 0x3 : 0x1;
		for (i = 0; i < pmac->addr_num; i++) {
			if (pmac->addr_raw && !align4) {
				/* internal address => external address.
				 * only applies to 16-bit access type
				 */
				if ((type == D11REG_TYPE_IHR16) ||
					(type == D11REG_TYPE_IHRX16)) {
					pmac->addr[i] += D11REG_IHR_WBASE;
				}
				pmac->addr[i] <<= 1;
			}
			if ((type == D11REG_TYPE_IHR16 || type == D11REG_TYPE_IHRX16) &&
			    pmac->addr[i] < D11REG_IHR_BASE) {
				/* host address space: convert local type */
				type = D11REG_TYPE_IHR32;
				align4 = TRUE;
			}

			mask = align4 ? 0x3 : 0x1;
			if ((pmac->addr[i] & mask)) {
				/* Odd addr not expected here for external addr */
				WL_ERROR(("%s line %d: addr %#x is not %s aligned!\n",
					__FUNCTION__, __LINE__, pmac->addr[i],
					align4 ? "dword" : "word"));
				pmac->addr[i] &= ~mask;
			}
		}
		if (pmac->step == (uint8)(-1)) {
			/* Set the default step when it is not given */
			pmac->step = align4 ? 4 : 2;
		} else if (pmac->step & mask) {
			/* step size validation check. */
			bcm_bprintf(b, "%s line %d: wrong step size %d for type %s\n",
				__FUNCTION__, __LINE__, pmac->step, pmac->type);
			goto exit;
		}
	}

	/* generate formated lists */
	if (pmac->bitmap || pmac->num) {
		skip1st = TRUE;
		d11dbg1.type = type;
		d11dbg1.addr = pmac->addr[0];
		d11dbg1.bitmap = pmac->bitmap;
		d11dbg1.step = pmac->step;
		d11dbg1.cnt = pmac->num;
		WL_TRACE(("d11dbg1: type %d, addr 0x%x, bitmap 0x%x, step %d, cnt %d\n",
			d11dbg1.type,
			d11dbg1.addr,
			d11dbg1.bitmap,
			d11dbg1.step,
			d11dbg1.cnt));
		idx += wlc_pw_d11regs(wlc, &d11dbg1, idx, b, NULL,
			(bool)pmac->w_en, pmac->w_val);
	}
	if (pmac->addr_num) {
		int num = pmac->addr_num;
		uint16 *paddr = d11dbg2.addr;
		if (skip1st) {
			num --;
			paddr ++;
		}
		d11dbg2.type = type;
		d11dbg2.cnt = (uint16)num;
		memcpy(paddr, pmac->addr, (sizeof(uint16) * num));
		WL_TRACE(("d11dbg2: type %d cnt %d\n",
			d11dbg2.type,
			d11dbg2.cnt));
		for (i = 0; i < num; i++) {
			WL_TRACE(("[%d] 0x%x\n", i, d11dbg2.addr[i]));
		}
		if (num > 0) {
			idx += wlc_pw_d11regs_byaddr(wlc, &d11dbg2, 1, idx, b,
				(bool)pmac->w_en, pmac->w_val);
		}
	}
	if (idx == 0) {
		/* print nothing */
		err = BCME_BADARG;
	}
exit:
	return err;
}

static int
wlc_dump_shmem(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	uint i, k = 0;
	uint16 val, addr, end;

	static const shmem_list_t shmem_list[] = {
		{0, 3*1024},
	};

	if (!wlc->clk) {
		WL_ERROR(("wl%d: %s: clock must be on\n", wlc->pub->unit, __FUNCTION__));
		return BCME_NOCLK;
	}
	for (i = 0; i < ARRAYSIZE(shmem_list); i++) {
		end = shmem_list[i].start + 2 * shmem_list[i].cnt;
		for (addr = shmem_list[i].start; addr < end; addr += 2) {
			val = wlc_read_shm(wlc, addr);
			if (b) {
				bcm_bprintf(b, "%d shm 0x%03x = 0x%04x\n",
					k++, addr, val);
			} else {
				printf("%d shm 0x%03x = 0x%04x\n",
				       k++, addr, val);
			}
		}
	}

	return 0;
}

static int
wlc_dump_sctpl(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	d11regs_t *regs;
	osl_t *osh;
	wlc_pub_t *pub;
	uint i;
	int gpio_sel;
	uint16 scpctl, addr0, addr1, curptr, len, offset;

	pub = wlc->pub;
	if (D11REV_LT(pub->corerev, 40)) {
		WL_ERROR(("wl%d: %s only supported for corerev >= 40\n",
			pub->unit, __FUNCTION__));
		return BCME_UNSUPPORTED;
	}

	if (!wlc->clk) {
		WL_ERROR(("wl%d: %s: clock must be on\n", pub->unit, __FUNCTION__));
		return BCME_NOCLK;
	}

	regs = wlc->regs;
	osh = wlc->osh;

	/* stop sample capture */
	if (D11REV_IS(pub->corerev, 50) || D11REV_GE(pub->corerev, 54)) {
		scpctl = R_REG(osh, &regs->u.d11acregs.SampleCollectPlayCtrl);
		W_REG(osh, &regs->u.d11acregs.SampleCollectPlayCtrl, scpctl & ~SC_STRT);
	} else {
		scpctl = R_REG(osh, &regs->psm_phy_hdr_param);
		W_REG(osh, &regs->psm_phy_hdr_param, scpctl & ~PHYCTL_SC_STRT);
	}

	if (b) {
		bcm_bprintf(b, "corerev: %d ucode revision %d.%d features 0x%04x\n",
			pub->corerev, wlc_read_shm(wlc, M_BOM_REV_MAJOR(wlc)),
			wlc_read_shm(wlc, M_BOM_REV_MINOR(wlc)),
			wlc_read_shm(wlc, M_UCODE_FEATURES(wlc)));
	} else {
		printf("corerev: %d ucode revision %d.%d features 0x%04x\n",
			pub->corerev, wlc_read_shm(wlc, M_BOM_REV_MAJOR(wlc)),
			wlc_read_shm(wlc, M_BOM_REV_MINOR(wlc)),
			wlc_read_shm(wlc, M_UCODE_FEATURES(wlc)));
	}

	gpio_sel = R_REG(osh, &regs->maccontrol1);
	addr0 = R_REG(osh, &regs->u.d11acregs.SampleCollectStartPtr);
	addr1 = R_REG(osh, &regs->u.d11acregs.SampleCollectStopPtr);
	curptr = R_REG(osh, &regs->u.d11acregs.SampleCollectCurPtr);
	len = (addr1 - addr0 + 1) * 4;
	offset = addr0 * 4;

	if (b) {
		bcm_bprintf(b, "Capture mode: maccontrol1 0x%02x scctl 0x%02x\n",
			gpio_sel, scpctl);
		bcm_bprintf(b, "Start/stop/cur 0x%04x 0x%04x 0x%04x byt_offset 0x%04x entries %u\n",
			addr0, addr1, curptr, 4 *(curptr - addr0), len>>2);
		bcm_bprintf(b, "offset: low high\n");
	} else {
		printf("Capture mode: maccontrol1 0x%02x scpctl 0x%02x\n", gpio_sel, scpctl);
		printf("Start/stop/cur 0x%04x 0x%04x 0x%04x byt_offset 0x%04x entries %u\n",
		       addr0, addr1, curptr, 4 *(curptr - addr0), len>>2);
		printf("offset: low high\n");
	}

	W_REG(osh, &regs->tplatewrptr, offset);

	for (i = 0; i < (uint)len; i += 4) {
		uint32 tpldata;
		uint16 low16, hi16;

		tpldata = R_REG(osh, &regs->tplatewrdata);
		hi16 = (tpldata >> 16) & 0xffff;
		low16 = tpldata & 0xffff;
		if (b)
			bcm_bprintf(b, "%04X: %04X %04X\n", i, low16, hi16);
		else
			printf("%04X: %04X %04X\n", i, low16, hi16);
	}
	return BCME_OK;
}

/** dump beacon (from read_txe_ram in d11procs.tcl) */
static void
wlc_dump_bcntpl(wlc_info_t *wlc, struct bcmstrbuf *b, int offset, int len)
{
	d11regs_t *regs = wlc->regs;
	osl_t *osh = wlc->osh;
	uint i;

	len = (len + 3) & ~3;
	W_REG(osh, &regs->tplatewrptr, offset);
	bcm_bprintf(b, "tpl: offset %d len %d\n", offset, len);
	for (i = 0; i < (uint)len; i += 4) {
		bcm_bprintf(b, "%04X: %08X\n", i,
			R_REG(osh, &regs->tplatewrdata));
	}
}

static int
wlc_dump_bcntpls(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	uint16 len;

	if (!wlc->clk) {
		WL_ERROR(("wl%d: %s: clock must be on\n", wlc->pub->unit, __FUNCTION__));
		return BCME_NOCLK;
	}

	len = wlc_read_shm(wlc, M_BCN0_FRM_BYTESZ(wlc));
	bcm_bprintf(b, "bcn 0: len %u\n", len);
	wlc_dump_bcntpl(wlc, b, D11AC_T_BCN0_TPL_BASE, len);
	len = wlc_read_shm(wlc, M_BCN1_FRM_BYTESZ(wlc));
	bcm_bprintf(b, "bcn 1: len %u\n", len);
	wlc_dump_bcntpl(wlc, b, D11AC_T_BCN1_TPL_BASE, len);

	return 0;
}

static int
wlc_dump_pio(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	int i;

	if (!wlc->clk)
		return BCME_NOCLK;

	if (!PIO_ENAB(wlc->pub))
		return 0;

	for (i = 0; i < NFIFO; i++) {
		pio_t *pio = WLC_HW_PIO(wlc, i);
		bcm_bprintf(b, "PIO %d: ", i);
		if (pio != NULL)
			wlc_pio_dump(pio, b);
		bcm_bprintf(b, "\n");
	}

	return 0;
}
#endif /* WL_MACDBG */

#if defined(BCMDBG) || defined(BCMDBG_DUMP)
static int
wlc_dump_dma(wlc_info_t *wlc, struct bcmstrbuf *b)
{
	int i;
	wl_cnt_wlc_t *cnt = wlc->pub->_cnt;

	if (!wlc->clk)
		return BCME_NOCLK;

	for (i = 0; i < NFIFO; i++) {
		PRREG_INDEX(intstatus, wlc->regs->intctrlregs[i].intstatus);
		PRREG_INDEX(intmask, wlc->regs->intctrlregs[i].intmask);
		bcm_bprintf(b, "\n");
		if (!PIO_ENAB(wlc->pub)) {
			hnddma_t *di;
			di = WLC_HW_DI(wlc, i);
			bcm_bprintf(b, "DMA %d: ", i);
			if (di != NULL) {
				dma_dumptx(di, b, TRUE);
				if (i == RX_FIFO) {
					dma_dumprx(di, b, TRUE);
					PRVAL(rxuflo[i]);
				} else
					PRVAL(txuflo);
				PRNL();
			}
		}
		bcm_bprintf(b, "\n");
	}

	PRVAL(dmada); PRVAL(dmade); PRVAL(rxoflo); PRVAL(dmape);
	bcm_bprintf(b, "\n");

	return 0;
}

#ifdef WLCNT
#define PRCNT_MACSTAT_TX							\
do {										\
	/* UCODE SHM counters */						\
	/* tx start and those that do not end well */					\
	PRVAL(txallfrm); PRVAL(txbcnfrm); PRVAL(txrtsfrm); PRVAL(txctsfrm);	\
	PRVAL(txackfrm); PRVAL(txback); PRVAL(txdnlfrm); PRNL();		\
	PRVAL(txampdu); PRVAL(txmpdu); PRVAL(txucast);				\
	PRVAL(rxrsptmout); PRVAL(txinrtstxop); PRVAL(txrtsfail); PRNL();	\
	bcm_bprintf(b, "txper_ucastdt %d%% txper_rts %d%%\n",			\
		cnt->txucast > 0 ?						\
		(100 - ((cnt->rxackucast + cnt->rxback) * 100			\
		 / cnt->txucast)) : 0,						\
		cnt->txrtsfrm > 0 ?						\
		(100 - (cnt->rxctsucast * 100 / cnt->txrtsfrm)) : 0);		\
	bcm_bprintf(b, "txfunfl: ");						\
	for (i = 0; i < NFIFO; i++)						\
		bcm_bprintf(b, "%d ", cnt->txfunfl[i]);				\
	PRVAL(txtplunfl); PRVAL(txphyerror); PRNL(); PRNL();			\
} while (0)

#define PRCNT_MACSTAT_RX							\
do {										\
	/* rx with goodfcs */							\
	PRVAL(rxctlucast); PRVAL(rxrtsucast); PRVAL(rxctsucast);		\
	PRVAL(rxackucast); PRVAL(rxback); PRNL();				\
	PRVAL(rxbeaconmbss); PRVAL(rxdtucastmbss);				\
	PRVAL(rxmgucastmbss); PRNL();						\
	PRVAL(rxbeaconobss); PRVAL(rxdtucastobss); PRVAL(rxdtocast);		\
	PRVAL(rxmgocast); PRNL();						\
	PRVAL(rxctlocast); PRVAL(rxrtsocast); PRVAL(rxctsocast); PRNL();	\
	PRVAL(rxctlmcast); PRVAL(rxdtmcast); PRVAL(rxmgmcast); PRNL(); PRNL();	\
										\
	PRVAL(rxcgprqfrm); PRVAL(rxcgprsqovfl); PRVAL(txcgprsfail);		\
	PRVAL(txcgprssuc); PRVAL(prs_timeout); PRNL();				\
	PRVAL(pktengrxducast); PRVAL(pktengrxdmcast);				\
	PRVAL(bcntxcancl);							\
} while (0)

static INLINE void
wlc_dump_macstat(wlc_info_t *wlc, struct bcmstrbuf * b)
{
	int i;
	if (D11REV_GE(wlc->pub->corerev, 40)) {
		wl_cnt_ge40mcst_v1_t *cnt = wlc->pub->_mcst_cnt;
		PRCNT_MACSTAT_TX;
		/* rx start and those that do not end well */
		PRVAL(rxstrt); PRVAL(rxbadplcp); PRVAL(rxcrsglitch);
		PRVAL(rxtoolate); PRVAL(rxnodelim); PRNL();
		PRVAL(rxdrop20s); PRVAL(bphy_badplcp); PRVAL(bphy_rxcrsglitch); PRNL();
		PRVAL(rxbadfcs); PRVAL(rxfrmtoolong); PRVAL(rxfrmtooshrt); PRVAL(rxanyerr); PRNL();
		PRVAL(rxf0ovfl); PRVAL(rxf1ovfl); PRVAL(rxhlovfl); PRVAL(pmqovfl); PRNL();
		PRCNT_MACSTAT_RX;
		PRVAL(missbcn_dbg); PRNL();
		PRNL();
	} else {
		wl_cnt_lt40mcst_v1_t *cnt = wlc->pub->_mcst_cnt;
		PRCNT_MACSTAT_TX;
		PRVAL(rxstrt); PRVAL(rxbadplcp); PRVAL(rxcrsglitch);
		PRVAL(rxtoolate); PRVAL(rxnodelim); PRNL();
		PRVAL(bphy_badplcp); PRVAL(bphy_rxcrsglitch); PRNL();
		PRVAL(rxbadfcs); PRVAL(rxfrmtoolong); PRVAL(rxfrmtooshrt); PRVAL(rxanyerr); PRNL();
		PRVAL(rxf0ovfl); PRVAL(pmqovfl); PRNL();
		PRCNT_MACSTAT_RX;
		PRNL();
		PRVAL(dbgoff46); PRVAL(dbgoff47);
		PRVAL(dbgoff48); PRVAL(phywatch);
		PRNL();
	}
}
#endif /* WLCNT */

/** print aggregate (since driver load time) driver and macstat counter values */
static int
wlc_dump_stats(wlc_info_t *wlc, struct bcmstrbuf * b)
{
#ifdef WLCNT
	int i;
	wl_cnt_wlc_t *cnt = wlc->pub->_cnt;

	if (WLC_UPDATE_STATS(wlc)) {
		wlc_statsupd(wlc);
	}

	/* print UCODE MACSTATs */
	wlc_dump_macstat(wlc, b);

	/* Display reinit counters */
	PRVAL(reinit);
	bcm_bprintf(b, "reinitreason_counts: ");
	for (i = 0; i < WL_REINIT_RC_LAST; i++) {
		bcm_bprintf(b, "%d(%d) ", i, wlc->pub->reinitrsn->rsn[i]);
	}
	PRNL();
	PRVAL(reset); PRVAL(pciereset); PRVAL(cfgrestore); PRVAL(dma_hang); PRNL();

	/* summary stat counter line */
	PRVAL(txframe); PRVAL(txbyte); PRVAL(txretrans); PRVAL(txfail);
	PRVAL(txchanrej); PRVAL(tbtt); PRVAL(p2p_tbtt); PRVAL(p2p_tbtt_miss); PRNL();
	PRVAL(rxframe); PRVAL(rxbyte); PRVAL(rxerror); PRNL();
	PRVAL(txprshort); PRVAL(txdmawar); PRVAL(txnobuf); PRVAL(txnoassoc);
	PRVAL(txchit); PRVAL(txcmiss); PRNL();
	PRVAL(txserr); PRVAL(txphyerr); PRVAL(txphycrs); PRVAL(txerror); PRNL();

	PRVAL_RENAME(txfrag, d11_txfrag); PRVAL_RENAME(txmulti, d11_txmulti);
	PRVAL_RENAME(txretry, d11_txretry); PRVAL_RENAME(txretrie, d11_txretrie); PRNL();
	PRVAL_RENAME(txrts, d11_txrts); PRVAL_RENAME(txnocts, d11_txnocts);
	PRVAL_RENAME(txnoack, d11_txnoack); PRVAL_RENAME(txfrmsnt, d11_txfrmsnt); PRNL();

	PRVAL(rxcrc); PRVAL(rxnobuf); PRVAL(rxnondata); PRVAL(rxbadds);
	PRVAL(rxbadcm); PRVAL(rxdup); PRVAL(rxrtry); PRVAL(rxfragerr); PRNL();
	PRVAL(rxrunt); PRVAL(rxgiant); PRVAL(rxnoscb); PRVAL(rxbadproto);
	PRVAL(rxbadsrcmac); PRNL();

	PRVAL_RENAME(rxfrag, d11_rxfrag); PRVAL_RENAME(rxmulti, d11_rxmulti);
	PRVAL_RENAME(rxundec, d11_rxundec); PRVAL_RENAME(rxundec_mcst, d11_rxundec_mcst);
	PRNL();

	PRVAL(rxctl); PRVAL(rxbadda); PRVAL(rxfilter);
	bcm_bprintf(b, "rxuflo: ");
	for (i = 0; i < NFIFO; i++)
		bcm_bprintf(b, "%d ", cnt->rxuflo[i]);
	PRNL(); PRNL();

	/* WPA2 counters */
	PRVAL(tkipmicfaill); PRVAL(tkipicverr); PRVAL(tkipcntrmsr); PRNL();
	PRVAL(tkipreplay); PRVAL(ccmpfmterr); PRVAL(ccmpreplay); PRNL();
	PRVAL(ccmpundec); PRVAL(fourwayfail); PRVAL(wepundec); PRNL();
	PRVAL(wepicverr); PRVAL(decsuccess); PRVAL(rxundec); PRNL();

	PRVAL(tkipmicfaill_mcst); PRVAL(tkipicverr_mcst); PRVAL(tkipcntrmsr_mcst); PRNL();
	PRVAL(tkipreplay_mcst); PRVAL(ccmpfmterr_mcst); PRVAL(ccmpreplay_mcst); PRNL();
	PRVAL(ccmpundec_mcst); PRVAL(fourwayfail_mcst); PRVAL(wepundec_mcst); PRNL();
	PRVAL(wepicverr_mcst); PRVAL(decsuccess_mcst); PRVAL(rxundec_mcst); PRNL();
	PRNL();

	PRVAL(rx1mbps); PRVAL(rx2mbps); PRVAL(rx5mbps5); PRVAL(rx11mbps); PRNL();
	PRVAL(rx6mbps); PRVAL(rx9mbps); PRVAL(rx12mbps); PRVAL(rx18mbps); PRNL();
	PRVAL(rx24mbps); PRVAL(rx36mbps); PRVAL(rx48mbps); PRVAL(rx54mbps); PRNL();

	PRVAL(txmpdu_sgi); PRVAL(rxmpdu_sgi); PRVAL(txmpdu_stbc); PRVAL(rxmpdu_stbc);
	PRVAL(rxmpdu_mu); PRNL();

	PRVAL(cso_normal); PRVAL(cso_passthrough); PRNL();
	PRVAL(chained); PRVAL(chainedsz1); PRVAL(unchained);
	PRVAL(maxchainsz); PRVAL(currchainsz); PRNL();

	/* detailed amangement and control frame counters */
	PRVAL(txbar); PRVAL(txpspoll); PRVAL(rxbar);
	PRVAL(rxpspoll); PRNL();
	PRVAL(txnull); PRVAL(txqosnull); PRVAL(rxnull);
	PRVAL(rxqosnull); PRNL();
	PRVAL(txassocreq); PRVAL(txreassocreq); PRVAL(txdisassoc);
	PRVAL(txassocrsp); PRVAL(txreassocrsp); PRNL();
	PRVAL(txauth); PRVAL(txdeauth); PRVAL(txprobereq);
	PRVAL(txprobersp); PRVAL(txaction); PRNL();
	PRVAL(rxassocreq); PRVAL(rxreassocreq); PRVAL(rxdisassoc);
	PRVAL(rxassocrsp); PRVAL(rxreassocrsp); PRNL();
	PRVAL(rxauth); PRVAL(rxdeauth); PRVAL(rxprobereq);
	PRVAL(rxprobersp); PRVAL(rxaction); PRNL();
#endif /* WLCNT */
	return 0;
}
#endif  /* defined(BCMDBG) || defined(BCMDBG_DUMP) */

/* ************* end of dump_xxx function section *************************** */

/* print upon critical or fatal error */
static void
wlc_suspend_mac_debug(wlc_info_t *wlc, uint32 phydebug)
{
	osl_t *osh;
	d11regs_t *regs;
	wlcband_t *band = wlc->band;
	uint16 val, addr;
	int loop;
	d11print_list_t acphyreg[] = {
		{"fineclk_gatectl", 0x16b},
		{"RfseqStatus0", 0x403},
		{"RfctrlCmd", 0x408}
	};

	BCM_REFERENCE(phydebug);

	/* dump additional regs */
	if (WLCISACPHY(band)) {
		addr = 0x1f0;
	} else if (WLCISHTPHY(band) || WLCISNPHY(band)) {
		addr = 0x183;
	} else {
		return;
	}

	osh = wlc->osh;
	regs = wlc->regs;
	for (loop = 0; loop < 10; loop++) {
		W_REG(osh, &regs->phyregaddr, addr);
		val = R_REG(osh, &regs->phyregdata);
		WL_PRINT(("PHY reg %#x = 0x%x\n", addr, val));
		OSL_DELAY(10);
	}
	if (!WLCISACPHY(band)) {
		return;
	}

	for (loop = 0; loop < ARRAYSIZE(acphyreg); loop ++) {
		W_REG(osh, &regs->phyregaddr, acphyreg[loop].addr);
		val = R_REG(osh, &regs->phyregdata);
		WL_PRINT(("PHY reg %#x = %#x (%s)\n",
			acphyreg[loop].addr, val, acphyreg[loop].name));
	}

}

void
wlc_dump_ucode_fatal(wlc_info_t *wlc, uint reason)
{
	wlc_pub_t *pub;
	osl_t *osh;
	d11regs_t *regs;
	uint32 phydebug, psmdebug;
	uint16 val16[4];
	uint32 val32[4];
	int i, k;
	volatile uint8 *paddr;
	/* Three lists: one common, two corerev specific */
	d11print_list_t *plist[3] = {NULL};
	int lsize[3] = {0};
	const char reason_str[][20] = {
		"any",
		"psm watchdog",
		"mac suspend failure",
		"fail to wake up",
		"txfifo susp/flush",
		"psmx watchdog",
		"txstuck",
	};

	/* common list */
	d11print_list_t cmlist[] = {
		{"ifsstat", 0x690},
		{"ifsstat1", 0x698},
		{"txectl", 0x500},
		{"txestat", 0x50e},
		{"rxestat1", 0x41a},
		{"rxestat2", 0x41c},
		{"rcv_frmcnt", 0x40a},
		{"rxe_rxcnt", 0x418},
		{"wepctl", 0x7c0},
	};
	/* reg specific to corerev < 40 */
	d11print_list_t list_lt40[] = {
		{"xmtfifordy", 0x546},
		{"pcmctl", 0x7d0},
		{"pcmstat", 0x7d2},
	};
	/* reg specific to corerev >= 40 */
	d11print_list_t list_ge40[] = {
		{"aqmfifordy", 0x838},
		{"wepstat", 0x7c2},
		{"wep_ivloc", 0x7c4},
		{"wep_psdulen", 0x7c6},
	};
	d11print_list_t list_ge64[] = {
		{"aqmfifordyL", 0xbbc},
		{"aqmfifordyH", 0xbbe},
		{"rxe_errval", 0x448},
		{"rxe_errmask", 0x44a},
		{"rxe_status3", 0x44c},
		{"txestat2", 0x51c},
	};
	d11print_list_t nphyreg[] = {
		{"pktproc", 0x183}, /* repeat four times */
		{"pktproc", 0x183},
		{"pktproc", 0x183},
		{"pktproc", 0x183}
	};
	d11print_list_t acphyreg[] = {
		{"pktproc", 0x1f0}, /* repeat four times */
		{"pktproc", 0x1f0},
		{"pktproc", 0x1f0},
		{"pktproc", 0x1f0}
	};
	d11print_list_t acphy2reg[] = {
		{"TxFifoStatus0", 0x60d},
		{"TxFifoStatus1", 0x80d},
		{"TxFifoStatus2", 0xa0d},
		{"TxFifoStatus3", 0xc0d}
	};

	pub = wlc->pub;
	osh = wlc->osh;
	regs = wlc->regs;
	BCM_REFERENCE(val16);
	BCM_REFERENCE(val32);

	k = (reason >= PSM_FATAL_LAST) ? PSM_FATAL_ANY : reason;
	WL_PRINT(("wl%d: reason = %s at %d seconds. corerev %d ",
		pub->unit, reason_str[k], pub->now, pub->corerev));
	if (!wlc->clk) {
		WL_PRINT(("%s: no clk\n", __FUNCTION__));
		return;
	}
	WL_PRINT(("ucode revision %d.%d features 0x%04x\n",
		wlc_read_shm(wlc, M_BOM_REV_MAJOR(wlc)), wlc_read_shm(wlc, M_BOM_REV_MINOR(wlc)),
		wlc_read_shm(wlc, M_UCODE_FEATURES(wlc))));

	psmdebug = R_REG(osh, &regs->psmdebug);
	phydebug = R_REG(osh, &regs->phydebug);
	val32[0] = R_REG(osh, &regs->maccontrol);
	val32[1] = R_REG(osh, &regs->maccommand);
	val16[0] = R_REG(osh, &regs->psm_brc);
	val16[1] = R_REG(osh, &regs->psm_brc_1);
	val16[2] = wlc_read_shm(wlc, M_UCODE_DBGST(wlc));

	wlc_mac_event(wlc, WLC_E_PSM_WATCHDOG, NULL, psmdebug, phydebug, val16[0], NULL, 0);
	WL_PRINT(("psmdebug 0x%08x phydebug 0x%x macctl 0x%x maccmd 0x%x\n"
		 "psm_brc 0x%04x psm_brc_1 0x%04x M_UCODE_DBGST 0x%x\n",
		 psmdebug, phydebug, val32[0], val32[1], val16[0], val16[1], val16[2]));

	if ((wlc->macdbg->log_done & (1 << reason)) != 0) {
		WL_PRINT(("%s: log_done %#x\n", __FUNCTION__, wlc->macdbg->log_done));
		return;
	}

	paddr = (volatile uint8*)(&regs->biststatus) - 0xC;
	plist[0] = cmlist;
	lsize[0] = ARRAYSIZE(cmlist);
	lsize[1] = 0;
	lsize[2] = 0;
	if (D11REV_LT(pub->corerev, 40)) {
		plist[1] = list_lt40;
		lsize[1] = ARRAYSIZE(list_lt40);
	} else {
		plist[1] = list_ge40;
		lsize[1] = ARRAYSIZE(list_ge40);
	}
	if (D11REV_GE(pub->corerev, 64)) {
		plist[2] = list_ge64;
		lsize[2] = ARRAYSIZE(list_ge64);
	}

	for (i = 0; i < ARRAYSIZE(plist); i++) {
		if (plist[i] == NULL)
			continue;
		for (k = 0; k < lsize[i]; k++) {
			val16[0] = R_REG(osh, (volatile uint16*)(paddr + plist[i][k].addr));
			WL_PRINT(("%-12s 0x%-4x ", plist[i][k].name, val16[0]));
			if ((k % 4) == 3)
				WL_PRINT(("\n"));
		}
		if (k % 4) {
			WL_PRINT(("\n"));
		}
	}

#ifdef WLAMPDU_MAC
	if (AMPDU_MAC_ENAB(pub))
		wlc_dump_aggfifo(wlc, NULL);
#endif /* WLAMPDU_MAC */

	WL_PRINT(("PC :\n"));
	for (k = 0; k < 64; k += 4) {
		val32[0] = R_REG(osh, &regs->psmdebug);
		val32[1] = R_REG(osh, &regs->psmdebug);
		val32[2] = R_REG(osh, &regs->psmdebug);
		val32[3] = R_REG(osh, &regs->psmdebug);
		WL_PRINT(("0x%-8x 0x%-8x 0x%-8x 0x%-8x\n",
			val32[0], val32[1], val32[2], val32[3]));
	}
	/* phyreg */
	if (phydebug > 0) {
		WL_PRINT(("phydebug :\n"));
		for (k = 0; k < 32; k += 4) {
			val32[0] = R_REG(osh, &regs->phydebug);
			val32[1] = R_REG(osh, &regs->phydebug);
			val32[2] = R_REG(osh, &regs->phydebug);
			val32[3] = R_REG(osh, &regs->phydebug);
			WL_PRINT(("0x%-8x 0x%-8x 0x%-8x 0x%-8x\n",
				val32[0], val32[1], val32[2], val32[3]));
		}
		if (WLCISACPHY(wlc->band)) {
			plist[0] = acphyreg;
			lsize[0] = ARRAYSIZE(acphyreg);
		} else if (WLCISHTPHY(wlc->band) || WLCISNPHY(wlc->band)) {
			plist[0] = nphyreg;
			lsize[0] = ARRAYSIZE(nphyreg);
		} else {
			lsize[0] = 0;
		}
		lsize[1] = 0;
		lsize[2] = 0;

		/* acphy2 debug info */
		if (WLCISACPHY(wlc->band) && (ACREV_GE(wlc->band->phyrev, 32))) {
			plist[1] = acphy2reg;
			lsize[1] = ARRAYSIZE(acphy2reg);
		}
		for (i = 0; i < 3; i ++) {
			for (k = 0; k < lsize[i]; k++) {
				W_REG(osh, &regs->phyregaddr, plist[i][k].addr);
				val16[0] = R_REG(osh, &regs->phyregdata);
				WL_PRINT(("%-12s 0x%-4x ", plist[i][k].name, val16[0]));
				if ((k % 4) == 3)
					WL_PRINT(("\n"));
			}
			if (k % 4) {
				WL_PRINT(("\n"));
			}
		}

		/* acphy2 debug info2 */
		if (WLCISACPHY(wlc->band) && (ACREV_GE(wlc->band->phyrev, 32))) {
			uint8 gpiosel = 2;
			d11print_list_t acphy2_gpioregs[] = {
				{"gpiosel", 0x394},
				{"gpiolo", 0x12},
				{"gpiohi", 0x13}
			};

			for (i = 0; i < gpiosel; i ++) {
				/* select gpio */
				W_REG(osh, &regs->phyregaddr, acphy2_gpioregs[0].addr);
				W_REG(osh, &regs->phyregdata, i);

				for (k = 0; k < ARRAYSIZE(acphy2_gpioregs); k++) {
					W_REG(osh, &regs->phyregaddr, acphy2_gpioregs[k].addr);
					val16[0] = R_REG(osh, &regs->phyregdata);
					WL_PRINT(("%-12s 0x%-4x ",
						acphy2_gpioregs[k].name, val16[0]));
				}
				WL_PRINT(("\n"));
			}
		}
	} /* phydebug */

	WL_PRINT(("psm stack_status : 0x%x\n", R_REG(osh, &regs->psm_pc_reg_0)));
	WL_PRINT(("psm stack_entries:\n"));
	for (k = 0; k < 8; k++) {
		W_REG(osh, &regs->psm_pc_reg_1, k);
		val16[0] = R_REG(osh, &regs->psm_pc_reg_2);
		WL_PRINT(("0x%04x\n", val16[0]));
	}

	k = (reason == PSM_FATAL_SUSP) ? 1 : 0;
	if (k) {
		wlc_suspend_mac_debug(wlc, phydebug);
	}


#if defined(STBLINUX) && defined(WLC_DUMP_MAC_EXT)
	wlc_dump_mac_ext(wlc, reason);
#endif

#if defined(WLC_HOSTPMAC)
	/* Mac dump for full dongle driver */
	/* triggering DHD to dump d11core */
	wlc_mac_event(wlc, WLC_E_MACDBG, NULL, WLC_E_STATUS_SUCCESS,
		WLC_E_MACDBG_REGALL, 0, &reason, sizeof(reason));
#endif /* WLC_HOSTPMAC */

	wlc->macdbg->log_done |= (1 << reason);
}

#if defined(WL_PSMX)
void
wlc_bmac_psmx_errors(wlc_info_t *wlc)
{
	d11regs_t *regs = wlc->regs;
	uint32 macintstatus;
	osl_t *osh;

	osh = wlc->osh;

	macintstatus = R_REG(osh, &regs->macintstatus_x);

	/* PSMx watchdog fired */
	if (macintstatus & MI_GP0) {
		W_REG(osh, &regs->macintmask_x, MI_GP0);
		wlc_dump_psmx_fatal(wlc, PSMX_FATAL_PSMWD);

		WLCNTINCR(wlc->pub->_cnt->psmxwds);
#ifndef BCMNODOWN
		if (g_assert_type == 3) {
			/* bring the driver down without resetting hardware */
			wlc->down_override = TRUE;
			wlc->psm_watchdog_debug = TRUE;
			wlc_out(wlc);

			/* halt the PSM */
			wlc_bmac_mctrl(wlc->hw, MCTL_PSM_RUN, 0);
			wlc->psm_watchdog_debug = FALSE;
		} else
#endif /* BCMNODOWN */
		{
			/* big hammer */
			wlc_fatal_error(wlc);
		}
	}
}

#ifdef WLVASIP
void
wlc_dump_vasip_fatal(wlc_info_t *wlc)
{
#if defined(SVMP_ACCESS_VIA_PHYTBL)
	wlc_hw_info_t *wlc_hw = wlc->hw;
	uint16 i, m, n, c[4], s[4];
	uint32 address[18] = {0x20000, 0x20060, 0x20080, 0x21e10, 0x20300, 0x20700, 0x20b00,
		0x20bff, 0xc000, 0xe000, 0x10000, 0x12000, 0x14000, 0x16000, 0x18000, 0x1a000,
		0x1c000, 0x1e000};
	uint16 size[18] = {16, 1, 1, 16, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	uint16 counter[93];
	uint16 report_idx[10] = {64, 65, 0, 1, 2, 3, 4, 5, 6, 7};

	/*
	 * name           = address, index in array
	 * counter        = 0x20000, 0;
	 * fix_mcs        = 0x20060, 16;
	 * error_code     = 0x20080, 17;
	 * mcs            = 0x21e10, 18;
	 * m2v0           = 0x20300, 34;
	 * m2v1           = 0x20700, 50;
	 * v2m            = 0x20b00, 66;
	 * transfer_done  = 0x20bff, 82;
	 * header0        = 0xc000, 83;
	 * header1        = 0xe000, 84;
	...
	*/
#endif /* SVMP_ACCESS_VIA_PHYTBL */

	if (!wlc->clk) {
		WL_PRINT(("%s: no clk\n", __FUNCTION__));
		return;
	}

#if defined(SVMP_ACCESS_VIA_PHYTBL)
	WL_PRINT(("VASIP watchdog is triggered. VASIP code revision %d.%d \n",
		d11vasipcode_major, d11vasipcode_minor));

	n = 0;
	for (i = 0; i < 18; i++) {
		for (m = 0; m < size[i]; m++) {
			phy_vasip_read_svmp((phy_info_t *)wlc_hw->band->pi,
				address[i]+m, &counter[n++]);
		}
	}

	for (i = 0; i < 4; i++) {
		s[i] = ((counter[18+i] & 0xf0) >> 4) + 1;
		c[i] = counter[18+i] & 0xf;
	}

	WL_PRINT(("Received Interrupts:\n"
		"      bfr_module_done:0x%x     bfe_module_done:0x%x     bfe_imp_done:0x%x\n"
			"      m2v_transfer_done:0x%x   v2m_transfder_done:0x%x\n"
			"Fixed MCS: %d\n"
			"Number of users: %x\n"
			"Last steered MCS:\n"
			"      user0:c%ds%d;   user1:c%ds%d;   user2:c%ds%d;   user3:c%ds%d\n"
			"Last precoder SNR:\n"
			"      user0:0x%x;   user1:0x%x;   user2:0x%x;   user3:0x%x\n"
			"Error code is 0x%x\n"
			"v2m transfer is done: %d\n",
			counter[2], counter[3], counter[4], counter[8], counter[9], counter[16],
			counter[27], c[0], s[0], c[1], s[1], c[2], s[2], c[3], s[3], counter[23]/4,
			counter[24]/4, counter[25]/4, counter[26]/4, counter[17], counter[82]));

	WL_PRINT(("m2v0 message:\n"));
	for (i = 0; i < 4; i++)
		WL_PRINT(("0x%04x 0x%04x 0x%04x 0x%04x\n", counter[34+i*4], counter[35+4*i],
				counter[36+4*i], counter[37+4*i]));

	WL_PRINT(("m2v1 message:\n"));
	for (i = 0; i < 4; i++)
		WL_PRINT(("0x%04x 0x%04x 0x%04x 0x%04x\n", counter[50+i*4], counter[51+4*i],
				counter[52+4*i], counter[53+4*i]));

	WL_PRINT(("v2m message:\n"));
	for (i = 0; i < 4; i++)
		WL_PRINT(("0x%04x 0x%04x 0x%04x 0x%04x\n", counter[66+i*4], counter[67+4*i],
				counter[68+4*i], counter[69+4*i]));

	for (i = 0; i < 10; i++)
		WL_PRINT(("index %d, addr 0x%04x, header: 0x%04x\n", report_idx[i],
				address[8+i], counter[i+83]));
#endif /* SVMP_ACCESS_VIA_PHYTBL */
}
#endif /* WLVASIP */

void
wlc_dump_psmx_fatal(wlc_info_t *wlc, uint reason)
{
	d11regs_t *regs;
	osl_t *osh;
	wlc_pub_t *pub;
	uint32 val32[4];
	uint16 val16;
	uint16 k;
	const char reason_str[][20] = {
		"any failure",
		"watchdog",
		"suspend failure",
		"txstuck",
	};

	osh = wlc->osh;
	pub = wlc->pub;
	regs = wlc->regs;

	k = (reason >= PSMX_FATAL_LAST) ? PSMX_FATAL_ANY : reason;
	WL_PRINT(("wl%d: PSMx dump at %d seconds. corerev %d reason:%s ",
		pub->unit, pub->now, pub->corerev, reason_str[k]));

	if (!wlc->clk) {
		WL_PRINT(("%s: no clk\n", __FUNCTION__));
		return;
	}

	WL_PRINT(("ucode revision %d.%d features 0x%04x\n",
		wlc_read_shmx(wlc, M_BOM_REV_MAJOR(wlc)), wlc_read_shmx(wlc, M_BOM_REV_MINOR(wlc)),
		wlc_read_shm(wlc, M_UCODE_FEATURES(wlc))));

	WL_PRINT(("psmxdebug 0x%08x macctl 0x%x maccmd 0x%x\n"
		 "psm_brc 0x%04x psm_brc_1 0x%04x M_UCODE_DBGST 0x%x\n",
		  R_REG(osh, &regs->psmdebug_x),
		  R_REG(osh, &regs->maccontrol_x),
		  R_REG(osh, &regs->maccommand_x),
		  wlc_read_macregx(wlc, 0x490),
		  wlc_read_macregx(wlc, 0x4d8),
		  wlc_read_shmx(wlc, M_UCODE_DBGST(wlc))));

	if ((wlc->macdbg->log_done & (1 << PSM_FATAL_PSMXWD)) != 0) {
		WL_PRINT(("%s: log_done %#x\n", __FUNCTION__, wlc->macdbg->log_done));
		return;
	}
	wlc->macdbg->log_done |= (1 << PSM_FATAL_PSMXWD);
	WL_PRINT(("PC (psmdebug_x):\n"));
	for (k = 0; k < 64; k += 4) {
		val32[0] = R_REG(osh, &regs->psmdebug_x);
		val32[1] = R_REG(osh, &regs->psmdebug_x);
		val32[2] = R_REG(osh, &regs->psmdebug_x);
		val32[3] = R_REG(osh, &regs->psmdebug_x);
		WL_PRINT(("0x%-8x 0x%-8x 0x%-8x 0x%-8x\n",
			val32[0], val32[1], val32[2], val32[3]));
	}
	WL_PRINT(("PC (psmdebug):\n"));
	for (k = 0; k < 8; k += 4) {
		val32[0] = R_REG(osh, &regs->psmdebug);
		val32[1] = R_REG(osh, &regs->psmdebug);
		val32[2] = R_REG(osh, &regs->psmdebug);
		val32[3] = R_REG(osh, &regs->psmdebug);
		WL_PRINT(("0x%-8x 0x%-8x 0x%-8x 0x%-8x\n",
			val32[0], val32[1], val32[2], val32[3]));
	}
	val16 = wlc_read_macregx(wlc, 0x4d0); /* psm_srs_status */
	WL_PRINT(("psmx stack_status : 0x%x\n", val16));
	WL_PRINT(("psmx stack_entries:\n"));
	for (k = 0; k < 8; k++) {
		wlc_write_macregx(wlc, 0x4d2, k); /* psm_srs_ptr */
		val16 = wlc_read_macregx(wlc, 0x4d4); /* psm_srs_entry */
		WL_PRINT(("0x%04x\n", val16));
	}

#ifdef WLVASIP
	/* bit5 of txe_vasip_intsts indicates vasip watchdog is triggered */
	val16 = wlc_read_macregx(wlc, 0x870);
	WL_PRINT(("txe_vasip_intsts %#x\n", val16));
	if (val16 & (1 << 5)) {
		wlc_dump_vasip_fatal(wlc);
	}
#endif	/* WLVASIP */
}
#endif /* WL_PSMX */


void
wlc_dump_mac_fatal(wlc_info_t *wlc, uint reason)
{
	wlc_dump_ucode_fatal(wlc, reason);
#ifdef WL_PSMX
	if (BCM_DMA_CT_ENAB(wlc)) {
		wlc_dump_psmx_fatal(wlc, (reason == PSM_FATAL_TXSTUCK) ?
			PSMX_FATAL_TXSTUCK : PSMX_FATAL_ANY);
	}
#endif /* WL_PSMX */
}

#if WL_MACDBG
/* Depending CoreRev, some reggisters are 2B, some 4B */
static void
wlc_macdbg_smpl_capture_optnreg(wlc_info_t *wlc, uint8 *reg_addr,
	uint32 *val, int reg_size, bool set)
{
	/* Caller checks the clock */
	ASSERT(wlc->clk);

	switch (reg_size) {
		case 2:
			if (set) {
				/* Lo Addr */
				W_REG(wlc->osh, (uint16*)(reg_addr), *(uint16 *)val);
				/* Hi Addr */
				W_REG(wlc->osh, (uint16 *)(reg_addr + reg_size),
					(uint16)(*val >> 16));
			} else {
				*val = R_REG(wlc->osh, (uint16 *)reg_addr) |
					(R_REG(wlc->osh, (uint16 *)(reg_addr + reg_size)) << 16);
			}
			break;
		case 4:
			if (set)
				W_REG(wlc->osh, (uint32 *)reg_addr, *val);
			else
				*val = R_REG(wlc->osh, (uint32 *)reg_addr);
			break;
		default:
			WL_ERROR(("wl%d: %s Wrong register size.\n", wlc->pub->unit, __FUNCTION__));
			ASSERT(0);

	}
}

static int
wlc_macdbg_smpl_capture_optns(wlc_info_t *wlc, wl_maccapture_params_t *params, bool set)
{
	d11regs_t *regs = wlc->regs;
	uint8 opt_bmp = params->optn_bmp;
	uint32 *mask = NULL, *val = NULL;
	uint16 smpl_ctrl = 0, d11_regvalue = 0;
	volatile uint16 *smpl_ctrl_reg;
	int smpl_capture_rev;
	int i = 0, reg_incr, optn_shft = 0;
	int reg_size, addr_size;
	uint8 *reg_block = NULL, **current_reg = NULL;

	volatile uint16 *smpl_capture_optn_regs_ge50[] = {
		&(regs->u.d11acregs.TXE_SCT_MASK_L), &(regs->u.d11acregs.TXE_SCT_MASK_H),
		&(regs->u.d11acregs.TXE_SCT_VAL_L), &(regs->u.d11acregs.TXE_SCT_VAL_H),
		&(regs->u.d11acregs.TXE_SCS_MASK_L), &(regs->u.d11acregs.TXE_SCS_MASK_H),
		&(regs->u.d11acregs.TXE_SCX_MASK_L), &(regs->u.d11acregs.TXE_SCX_MASK_H),
		&(regs->u.d11acregs.TXE_SCM_MASK_L), &(regs->u.d11acregs.TXE_SCM_MASK_H),
		&(regs->u.d11acregs.TXE_SCM_VAL_L), &(regs->u.d11acregs.TXE_SCM_VAL_H)};

	volatile uint32 *smpl_capture_optn_regs_lt50[] = {
		&regs->dbgstrtrigmask,	/* Trigger Mask */
		&regs->dbgstrtrig,	/* Trigger Value */
		&regs->dbgstrmask};	/* Store Mask */
					/* No Transition Mask,
					 * No Match Mask
					 * No Match Value
					 */

	uint16 smpl_ctrl_en[2][SC_NUM_OPTNS_GE50] = {
		{SC_TRIG_EN, SC_STORE_EN, SC_TRANS_EN, SC_MATCH_EN},
		{PHYCTL_SC_TRIG_EN, PHYCTL_SC_STR_EN, PHYCTL_SC_TRANS_EN, SC_OPTN_LT50NA}};

	/* Core Specific Inits */
	if (D11REV_GE(wlc->pub->corerev, 50)) {
		smpl_capture_rev = SMPL_CAPTURE_GE50;
		smpl_ctrl_reg = &regs->u.d11acregs.SampleCollectPlayCtrl;
		reg_incr = 2; /* Per option, there are 2 regs. >= CoreRev 50 */
		reg_block = (void *)smpl_capture_optn_regs_ge50;
		addr_size = sizeof(smpl_capture_optn_regs_ge50[0]);
		reg_size = sizeof(*smpl_capture_optn_regs_ge50[0]);
	} else {
		ASSERT(!(opt_bmp & ((1 << WL_MACCAPT_TRANS) | (1 << WL_MACCAPT_MATCH))));
		smpl_capture_rev = SMPL_CAPTURE_LT50;
		smpl_ctrl_reg = &regs->psm_phy_hdr_param;
		reg_incr = 1; /* Per option, there is 1 reg. < CoreRev 50 */
		reg_block = (void *)smpl_capture_optn_regs_lt50;
		addr_size = sizeof(smpl_capture_optn_regs_lt50[0]);
		reg_size = sizeof(*smpl_capture_optn_regs_lt50[0]);
	}

	while (opt_bmp) {
		current_reg = (uint8 **)(reg_block + i * addr_size);
		if (opt_bmp & 1) {
			mask = (uint32*)(&params->tr_mask + (i / reg_incr));

			if (set) {
				/* Set the mask of the corresponding mode */
				wlc_macdbg_smpl_capture_optnreg(wlc, *current_reg,
				mask, reg_size, set);
			} else {
				/* Get the mask */
				wlc_macdbg_smpl_capture_optnreg(wlc, *current_reg,
				mask, reg_size, FALSE);
			}

			/* Some options have corresponding values */
			if ((optn_shft == WL_MACCAPT_TRIG) ||
			(optn_shft == WL_MACCAPT_MATCH)) {
				/* Value right after mask */
				val = (mask + 1);
				current_reg = (uint8 **)(reg_block + (i + reg_incr) * addr_size);

				if (set)
					wlc_macdbg_smpl_capture_optnreg(wlc, *current_reg, val,
					reg_size, set);
				else
					wlc_macdbg_smpl_capture_optnreg(wlc, *current_reg, val,
					reg_size, FALSE);
			}

			/* Populate soft-reg. value for SampleCollectPlayCtrl */
			smpl_ctrl |= (smpl_ctrl_en[smpl_capture_rev][optn_shft]);
		}
		opt_bmp >>= 1;

		if ((optn_shft == WL_MACCAPT_TRIG) ||
		(optn_shft == WL_MACCAPT_MATCH)) {
			/* WL_MACCAPT_TRIG and WL_MACCAPT_MATCH
			 * have "val" fields after mask so need to
			 * account for that
			 */
			i += reg_incr;
		}
		i += reg_incr;
		optn_shft++;
	}

	/* Enable the SC Options */
	if (smpl_ctrl && set) {
		d11_regvalue = R_REG(wlc->osh, smpl_ctrl_reg);
		W_REG(wlc->osh, smpl_ctrl_reg, d11_regvalue | smpl_ctrl);
	}

	return BCME_OK;
}

/* wlc_macdbg_smpl_capture_set initializes MAC sample capture.
 * If UP, also starts the sample capture.
 * Otherwise, sample capture will be started upon wl up
 */
static int
wlc_macdbg_smpl_capture_set(wlc_info_t *wlc, wl_maccapture_params_t *params)
{
	wlc_macdbg_info_t *macdbg = wlc->macdbg;
	wl_maccapture_params_t *cur_params = (wl_maccapture_params_t *)macdbg->smpl_info;
	d11regs_t *regs = wlc->regs;
	uint8 gpio_sel = params->gpio_sel;
	uint16 d11_regvalue = 0;

	if (D11REV_LT(wlc->pub->corerev, 26)) {
		WL_ERROR(("wl%d: %s only supported for corerev >=26\n",
			wlc->pub->unit, __FUNCTION__));
		return BCME_UNSUPPORTED;
	}

	if (!wlc->clk) {
		memcpy((void *)cur_params, (void *)params, sizeof(*params));
		wlc->macdbg->smpl_ctrl |= SC_STRT; /* Set the start bit of the soft register */
		WL_ERROR(("%s: MAC Sample Capture params. saved. "
			"Will  start upon wl up\n", __FUNCTION__));
		return BCME_OK;
	}

	if (params->cmd == WL_MACCAPT_STOP) {
		if (D11REV_GE(wlc->pub->corerev, 50)) {
			/* Clear Start bit */
			d11_regvalue =	R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectPlayCtrl);
			W_REG(wlc->osh, &regs->u.d11acregs.SampleCollectPlayCtrl,
				d11_regvalue & ~SC_STRT);
		} else {
			/* Clear start bit */
			AND_REG(wlc->osh, &regs->psm_phy_hdr_param, ~PHYCTL_SC_STRT);
		}

		/* Clear the start bit of the soft register */
		wlc->macdbg->smpl_ctrl &= ~SC_STRT;
		return BCME_OK;
	}

	if (params->la_mode) {
		if (D11REV_LT(wlc->pub->corerev, 50)) {
			/* Assign all GPIOs to other cores than ChipCommon */
			si_ccreg(wlc->pub->sih, CC_GPIOCTRL, ~0, 0xFFFFFFFF);

			/* Clear PHY Lo/Hi GPIO Out En Regs */
			phy_dbg_gpio_out_enab((phy_info_t *)WLC_PI(wlc), TRUE);
		}
		/* Enable GPIO based on mask */
		W_REG(wlc->osh, &regs->psm_gpio_oe, params->s_mask);
#ifdef BCMINTDBG
		/* Signal wlc_bmac about GPIO_SEL */
		wlc_bmac_ucode_dbgsel_set(wlc->hw, gpio_sel);
#endif /* BCMINTDBG */
	}

	/* GPIO Output Selection */
	d11_regvalue = (gpio_sel << MCTL1_GPIOSEL_SHIFT)
			& MCTL1_GPIOSEL_MASK; /* GPIO_SEL is 6bits */
	d11_regvalue = (R_REG(wlc->osh, &regs->maccontrol1) & ~MCTL1_GPIOSEL_MASK) | d11_regvalue;
	W_REG(wlc->osh, &regs->maccontrol1, d11_regvalue);

	/* Sample Collect Start & Stop Ptr */
	W_REG(wlc->osh, &regs->u.d11acregs.SampleCollectStartPtr, params->start_ptr);
	W_REG(wlc->osh, &regs->u.d11acregs.SampleCollectStopPtr, params->stop_ptr);

	/* Enable Sample Capture Clock */
	d11_regvalue =  R_REG(wlc->osh, &regs->psm_corectlsts);
	W_REG(wlc->osh, &regs->psm_corectlsts, d11_regvalue | PSM_CORE_CTL_SS);

	/* Setting options bitmap */
	/* Some options are not supported for CoreRev < 50 */
	if (D11REV_LT(wlc->pub->corerev, 50)) {
		if (params->x_mask) {
			WL_ERROR(("%s: Transition Mask not supported below CoreRev 50\n",
				__FUNCTION__));
			params->optn_bmp &= ~(1 << WL_MACCAPT_TRANS);
		}

		if ((params->m_val) || (params->m_mask)) {
			WL_ERROR(("%s: Match Mode not supported below CoreRev 50\n",
				__FUNCTION__));
			params->optn_bmp &= ~(1 << WL_MACCAPT_MATCH);
		}
	}

	/* Sample Capture Options */
	if (params->optn_bmp)
		wlc_macdbg_smpl_capture_optns(wlc, params, TRUE);

	/* Start MAC Sample Capture */
	if (D11REV_GE(wlc->pub->corerev, 50)) {
		/* Sample Capture Source and Start bits */
		d11_regvalue =	R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectPlayCtrl);
		d11_regvalue |= (SC_SRC_MAC << SC_SRC_SHIFT) | SC_STRT;
		W_REG(wlc->osh, &regs->u.d11acregs.SampleCollectPlayCtrl, d11_regvalue);

		WL_TRACE(("%s: SampleCollectPlayCtrl(0xb2e): "
			"0x%x, GPIO Out Sel:0x%02x\n", __FUNCTION__,
			R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectPlayCtrl),
			R_REG(wlc->osh, &regs->maccontrol1)));

	} else {
		d11_regvalue =  R_REG(wlc->osh, &regs->psm_phy_hdr_param);
		d11_regvalue |= (PHYCTL_PHYCLKEN | PHYCTL_SC_STRT | PHYCTL_SC_SRC_LB |
				PHYCTL_SC_TRANS_EN);
		W_REG(wlc->osh, &regs->psm_phy_hdr_param, d11_regvalue);

		WL_TRACE(("%s: PHY_CTL(0x492): 0x%x, GPIO Out Sel:0x%02x\n", __FUNCTION__,
				R_REG(wlc->osh, &regs->psm_phy_hdr_param),
				R_REG(wlc->osh, &regs->maccontrol1)));

	}

	/* Store config info */
	memcpy((void *)cur_params, params, sizeof(*params));
	wlc->macdbg->smpl_ctrl |= SC_STRT; /* Set the start bit of the soft register */

	return BCME_OK;
}

static int
wlc_macdbg_smpl_capture_get(wlc_info_t *wlc, char *outbuf, uint outlen)
{
	d11regs_t *regs = wlc->regs;
	struct bcmstrbuf bstr;
	wlc_macdbg_info_t *macdbg = wlc->macdbg;
	wl_maccapture_params_t *cur_params = (wl_maccapture_params_t *)macdbg->smpl_info;
	wl_maccapture_params_t get_params;
	uint32 cur_ptr = 0; /* Sample Capture cur_ptr */

	/* HW is turned off so don't try to access it */
	if (wlc->pub->hw_off || DEVICEREMOVED(wlc))
		return BCME_RADIOOFF;

	if (D11REV_LT(wlc->pub->corerev, 26)) {
		WL_ERROR(("wl%d: %s only supported for corerev >=26\n",
			wlc->pub->unit, __FUNCTION__));
		return BCME_UNSUPPORTED;
	}

	bcm_binit(&bstr, outbuf, outlen);

	memset((void *)outbuf, 0, outlen);
	memset((void *)&get_params, 0, sizeof(get_params));

	/* Check clock */
	if (!wlc->clk) {
		bcm_bprintf(&bstr, "MAC Capture\nNo Clock so returning saved state:\n");
		memcpy(&get_params, (void *)cur_params, sizeof(wl_maccapture_params_t));
		/* If no clock, return 0 for cur_ptr */
		goto print_values;
	}

	/* GPIO Out Sel */
	get_params.gpio_sel = (R_REG(wlc->osh, &regs->maccontrol1) & MCTL1_GPIOSEL_MASK)
		>> MCTL1_GPIOSEL_SHIFT;

	get_params.start_ptr = R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectStartPtr);
	get_params.stop_ptr = R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectStopPtr);

	/* Read the options from option regs */
	/* Supported options */
	if (D11REV_GE(wlc->pub->corerev, 50)) {
		get_params.optn_bmp = (1 << WL_MACCAPT_TRIG) | (1 << WL_MACCAPT_STORE) |
			(1 << WL_MACCAPT_TRANS) | (1 << WL_MACCAPT_MATCH);
	} else
		get_params.optn_bmp = (1 << WL_MACCAPT_TRIG) | (1 << WL_MACCAPT_STORE);

	wlc_macdbg_smpl_capture_optns(wlc, &get_params, FALSE);

	/* Cur. Ptr */
	cur_ptr = R_REG(wlc->osh, &regs->u.d11acregs.SampleCollectCurPtr);

	bcm_bprintf(&bstr, "MAC Capture Registers:\n");

print_values:
	bcm_bprintf(&bstr, "GPIO Sel:0x%x Logic Analyzer Mode On:%d\n"
			"start_ptr:0x%x, stop_ptr:0x%x, cur_ptr:0x%x\n"
			"store mask:0x%x, match mask:0x%x, match val:0x%x\n"
			"trans mask:0x%x, trig mask:0x%x, trig val:0x%x\n"
			"state:0x%02x\n", get_params.gpio_sel, get_params.la_mode,
			get_params.start_ptr, get_params.stop_ptr, cur_ptr,
			get_params.s_mask, get_params.m_mask, get_params.m_val,
			get_params.x_mask, get_params.tr_mask, get_params.tr_val,
			get_params.cmd);


	return BCME_OK;
}
#endif /* WL_MACDBG */

#if defined(BCMDBG) || defined(BCMDBG_DUMP)|| defined(PHYTXERR_DUMP)
void
wlc_dump_phytxerr(wlc_info_t *wlc, uint16 PhyErr)
{
	phy_dbg_txerr_dump((phy_info_t *)WLC_PI(wlc), PhyErr);

	if (D11REV_GE(wlc->pub->corerev, 40)) {
		uint16 addr;
		addr = 2 * wlc_read_shm(wlc, M_DEBUGBLK_PTR(wlc));
		/* plcp dump format here is L-SIG || Sig-A || Sig-B */
		printf("txerr valid (%d) reason %04x tst %02x pctls %04x %04x %04x\n"
			"plcp %04x %04x || %04x %04x %04x || %04x %04x || rxestats2 %04x\n",
			wlc_read_shm(wlc, addr + M_TXERR_NOWRITE_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_REASON_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_CTXTST_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PCTL0_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PCTL1_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PCTL2_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_LSIG0_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_LSIG1_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PLCP0_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PLCP1_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_PLCP2_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_SIGB0_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_SIGB1_OFFSET(wlc)),
			wlc_read_shm(wlc, addr + M_TXERR_RXESTATS2_OFFSET(wlc)));
			wlc_write_shm(wlc, addr, 0);
	}
}
#endif /* BCMDBG || BCMDBG_DUMP || PHYTXERR_DUMP */

#ifdef WLC_MACDBG_FRAMEID_TRACE
/* tx pkt history entry */
struct pkt_hist {
	void *pkt;
	uint32 flags;
	uint32 flags3;
	uint16 frameid;
	uint16 seq;
	uint8 epoch;
	uint8 fifo;
};

/* tx pkt history size */
#define PKT_HIST_NUM_ENT 1024

void
wlc_macdbg_frameid_trace_pkt(wlc_macdbg_info_t *macdbg, void *pkt, uint8 fifo,
	uint16 txFrameID, uint8 epoch)
{
	wlc_info_t *wlc = macdbg->wlc;

	if (macdbg->pkt_hist_cnt == PKT_HIST_NUM_ENT)
		macdbg->pkt_hist_cnt = 0;

	macdbg->pkt_hist[macdbg->pkt_hist_cnt].pkt = pkt;
	macdbg->pkt_hist[macdbg->pkt_hist_cnt].flags = WLPKTTAG(pkt)->flags;
	macdbg->pkt_hist[macdbg->pkt_hist_cnt].flags3 = WLPKTTAG(pkt)->flags3;
	macdbg->pkt_hist[macdbg->pkt_hist_cnt].frameid = txFrameID;
	macdbg->pkt_hist[macdbg->pkt_hist_cnt].seq = WLPKTTAG(pkt)->seq;
	if (D11REV_GE(wlc->pub->corerev, 40)) {
		macdbg->pkt_hist[macdbg->pkt_hist_cnt].epoch = epoch;
	}
	macdbg->pkt_hist[macdbg->pkt_hist_cnt].fifo = fifo;

	macdbg->pkt_hist_cnt++;
}

/* tx status history entry */
struct txs_hist {
	void *pkt;
	uint16 frameid;
	uint16 seq;
	uint16 status;
	uint16 ncons;
	uint16 cnt;	/* txstatus count - need special ucode */
};

/* tx status history size */
#define TXS_HIST_NUM_ENT 256

void
wlc_macdbg_frameid_trace_txs(wlc_macdbg_info_t *macdbg, void *pkt, tx_status_t *txs)
{
	wlc_info_t *wlc = macdbg->wlc;

	if (macdbg->txs_hist_cnt == TXS_HIST_NUM_ENT)
		macdbg->txs_hist_cnt = 0;

	macdbg->txs_hist[macdbg->txs_hist_cnt].pkt = pkt;
	macdbg->txs_hist[macdbg->txs_hist_cnt].frameid = txs->frameid;
	macdbg->txs_hist[macdbg->txs_hist_cnt].seq = txs->sequence;
	macdbg->txs_hist[macdbg->txs_hist_cnt].status = txs->status.raw_bits;
	if (D11REV_GE(wlc->pub->corerev, 40)) {
		macdbg->txs_hist[macdbg->txs_hist_cnt].ncons =
		        (txs->status.raw_bits & TX_STATUS40_NCONS) >> TX_STATUS40_NCONS_SHIFT;
	}
	macdbg->txs_hist[macdbg->txs_hist_cnt].cnt = (uint16)txs->status.s8;

	macdbg->txs_hist_cnt++;

	/* fifo sync'd packets */
	if (D11REV_GE(wlc->pub->corerev, 40) &&
	    pkt == NULL &&
	    macdbg->sync_hist_cnt > 0) {
		macdbg->sync_hist_next +=
		        (txs->status.raw_bits & TX_STATUS40_NCONS) >> TX_STATUS40_NCONS_SHIFT;
		if (macdbg->sync_hist_next >= macdbg->sync_hist_cnt) {
			macdbg->sync_hist_next = 0;
			macdbg->sync_hist_cnt = 0;
		}
	}
}

/* fifo sync history size */
#define SYNC_HIST_NUM_ENT 64

/* fifo sync history entry */
struct sync_hist {
	void *pkt;
};

void
wlc_macdbg_frameid_trace_sync(wlc_macdbg_info_t *macdbg, void *pkt)
{
	if (macdbg->sync_hist_cnt == TXS_HIST_NUM_ENT)
		return;

	macdbg->sync_hist[macdbg->sync_hist_cnt].pkt = pkt;

	macdbg->sync_hist_cnt ++;
}

/* dump histories */
void
wlc_macdbg_frameid_trace_dump(wlc_macdbg_info_t *macdbg)
{
	int k;

	printf("pkt_hist_cnt:%d \n", macdbg->pkt_hist_cnt);
	printf("pkt hist:\n");
	for (k = 0; k < PKT_HIST_NUM_ENT; k++) {
		printf("%d pkt:%p flags:0x%x flags3:0x%x frameid:0x%x seq:0x%x epoch:%d fifo:%d\n",
		       k, OSL_OBFUSCATE_BUF(macdbg->pkt_hist[k].pkt),
		       macdbg->pkt_hist[k].flags, macdbg->pkt_hist[k].flags3,
		       macdbg->pkt_hist[k].frameid, macdbg->pkt_hist[k].seq,
		       macdbg->pkt_hist[k].epoch, macdbg->pkt_hist[k].fifo);
	}

	printf("txs_hist_cnt:%d \n", macdbg->txs_hist_cnt);
	printf("txs hist:\n");
	for (k = 0; k < TXS_HIST_NUM_ENT; k++) {
		printf("%d pkt:%p frameid:0x%x seq:0x%x status:0x%x ncons:%d cnt:%d\n",
		       k, OSL_OBFUSCATE_BUF(macdbg->txs_hist[k].pkt),
		       macdbg->txs_hist[k].frameid, macdbg->txs_hist[k].seq,
		       macdbg->txs_hist[k].status, macdbg->txs_hist[k].ncons,
		       macdbg->txs_hist[k].cnt);
	}

	printf("sync_hist_next:%d \n", macdbg->sync_hist_next);
	printf("sync_hist_cnt:%d \n", macdbg->sync_hist_cnt);
	printf("sync hist:\n");
	for (k = 0; k < SYNC_HIST_NUM_ENT; k++) {
		printf("%d pkt:%p\n", k, OSL_OBFUSCATE_BUF(macdbg->sync_hist[k].pkt));
	}
}

/* attach/detach */
static int
BCMATTACHFN(wlc_macdbg_frameid_trace_attach)(wlc_macdbg_info_t *macdbg, wlc_info_t *wlc)
{
	if ((macdbg->pkt_hist =
	     MALLOCZ(wlc->osh, sizeof(*(macdbg->pkt_hist)) * PKT_HIST_NUM_ENT)) == NULL) {
		return BCME_NOMEM;
	}

	if ((macdbg->txs_hist =
	     MALLOCZ(wlc->osh, sizeof(*(macdbg->txs_hist)) * TXS_HIST_NUM_ENT)) == NULL) {
		return BCME_NOMEM;
	}

	if ((macdbg->sync_hist =
	     MALLOCZ(wlc->osh, sizeof(*(macdbg->sync_hist)) * SYNC_HIST_NUM_ENT)) == NULL) {
		return BCME_NOMEM;
	}

	return BCME_OK;
}

static void
BCMATTACHFN(wlc_macdbg_frameid_trace_detach)(wlc_macdbg_info_t *macdbg, wlc_info_t *wlc)
{
	if (macdbg->sync_hist != NULL) {
		MFREE(wlc->osh, macdbg->sync_hist, sizeof(*(macdbg->sync_hist))*SYNC_HIST_NUM_ENT);
		macdbg->sync_hist = NULL;
	}
	if (macdbg->txs_hist != NULL) {
		MFREE(wlc->osh, macdbg->txs_hist, sizeof(*(macdbg->txs_hist)) * TXS_HIST_NUM_ENT);
		macdbg->txs_hist = NULL;
	}
	if (macdbg->pkt_hist != NULL) {
		MFREE(wlc->osh, macdbg->pkt_hist, sizeof(*(macdbg->pkt_hist)) * PKT_HIST_NUM_ENT);
		macdbg->pkt_hist = NULL;
	}
}
#endif /* WLC_MACDBG_FRAMEID_TRACE */

#if defined(STBLINUX) && defined(WLC_DUMP_MAC_EXT)
int wlc_dump_mac_file(const char * file_name, uint32 flags, uint8 *buf, int size)
{
	int ret = 0;
	struct file *fp = NULL;
	mm_segment_t old_fs;
	loff_t pos = 0;

	/* change to KERNEL_DS address limit */
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	/* open file to write */
	fp = filp_open(file_name, flags, 0644);
	if (IS_ERR(fp)) {
		printf("%s: open file error, err = %ld\n", __FUNCTION__, PTR_ERR(fp));
		ret = -1;
		goto exit;
	}

	/* Write buf to file */
	if (fp->f_op) {
		vfs_write(fp, buf, size, &pos);
	} else
		printf("invalid f_op (%s) in %s\n",
			file_name, __FUNCTION__);
exit:
	/* close file before return */
	if (!IS_ERR(fp)) {
		filp_close(fp, current->files);
	}

	/* restore previous address limit */
	set_fs(old_fs);

	return ret;
}

int
wlc_macdbg_dumpmac(wlc_info_t *wlc, char *buf, int buflen,
	          int *outbuflen, bool dump_x)
{
	uint cnt = 0;
	struct bcmstrbuf bstr;

	bcm_binit(&bstr, buf, buflen);
	cnt = wlc_dump_mac(wlc, &bstr);
	*outbuflen = strlen(buf);

	return ((cnt > 0) ? BCME_OK : BCME_UNSUPPORTED);
}

void
wlc_dump_mac_ext(wlc_info_t *wlc, uint reason)
{
	char* prntbuf = NULL;
	char* dumpbuf = NULL;
	int dumpbuf_len = 0, off = 0;
	uint16 dump_signature;
	char dumpfilename[DUMPMAC_FILENAME_SZ] = {0, };

	dumpbuf = (char *)MALLOCZ(wlc->osh, DUMPMAC_BUF_SZ);
	prntbuf = (char *)MALLOCZ(wlc->osh, DUMPMAC_PRINT_SZ);

	if (dumpbuf) {
		/* Write macdump to a file */

		/* Get dump file signature */
		dump_signature = (uint16)OSL_RAND();
		memset(dumpbuf, 0, DUMPMAC_BUF_SZ);

		/* PSMr */
		if (wlc_macdbg_dumpmac(wlc, dumpbuf, DUMPMAC_BUF_SZ,
					&dumpbuf_len, FALSE) == BCME_OK) {
			snprintf(dumpfilename, DUMPMAC_FILENAME_SZ,
					"./d11reg_dump_%04X.txt", dump_signature);
			WL_INFORM(("%s: PSMr macreg dump to %s\n", __FUNCTION__, dumpfilename));

			WL_ERROR(("\n--- mac register dump start -- len(%d)\n", dumpbuf_len));
			for (off = 0; off < dumpbuf_len; off += DUMPMAC_PRINT_SZ) {
				strncpy(prntbuf, dumpbuf + off, DUMPMAC_PRINT_SZ);
				WL_ERROR(("%s", prntbuf));
			}
			WL_ERROR(("\n--- mac register dump end\n"));
			/* Write to a file */
			if (wlc_dump_mac_file(dumpfilename, (O_CREAT | O_WRONLY | O_SYNC),
						dumpbuf, dumpbuf_len)) {
				WL_ERROR(("%s: writing mac dump to the file failed\n",
							__FUNCTION__));
			}
		}

		MFREE(wlc->osh, dumpbuf, DUMPMAC_BUF_SZ);
	}

	MFREE(wlc->osh, prntbuf, DUMPMAC_PRINT_SZ);
}
#endif /* STBLINUX && WLC_DUMP_MAC_EXT */
