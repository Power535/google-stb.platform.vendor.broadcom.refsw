/*
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


#include "wl_rte.h"
#include <osl.h>
#include <hnd_cplt.h>
#include <siutils.h>
#include <pcie_core.h>
#include <epivers.h>
#include <proto/ethernet.h>
#include <bcmdevs.h>
#include <wlioctl.h>

#include <proto/802.11.h>
#include <proto/802.3.h>
#include <proto/vlan.h>
#include <d11.h>
#include <d11_cfg.h>
#include <wlc_channel.h>
#include <wlc_pub.h>
#include <wlc.h>
#include <wlc_hw.h>
#include <wlc_hw_priv.h>
#include <wlc_bmac.h>
#include <bcmsrom_fmt.h>
#include <bcmsrom.h>

#ifdef MSGTRACE
#include <dngl_msgtrace.h>
#endif
#ifdef LOGTRACE
#include <dngl_logtrace.h>
#endif

#include <wl_export.h>

#include <wl_toe.h>
#include <wl_arpoe.h>
#include <wl_keep_alive.h>
#include <wl_eventq.h>
#include <wl_gas.h>
#include <wl_p2po_disc.h>
#include <wl_p2po.h>
#include <wl_anqpo.h>
#include <wlc_pkt_filter.h>
#include <wlc_pcb.h>
#include <wlc_dump.h>
#include <wl_bdo.h>
#include <wl_tko.h>
#include <wl_icmp.h>


#ifdef BCMDBG
#include <bcmutils.h>
#endif

#include <dngl_bus.h>
#include <dngl_wlhdr.h>

#define WL_IFTYPE_BSS	1
#define WL_IFTYPE_WDS	2

#ifdef BCMASSERT_LOG
#include <bcm_assert_log.h>
#endif
#include <wlc_bsscfg.h>

#if defined(BCMAUTH_PSK)
#include <proto/eapol.h>
#include <wlc_auth.h>
#endif

#include <wlc_ampdu_rx.h>
#if defined(PROP_TXSTATUS)
#include <wlc_wlfc.h>
#ifdef BCMPCIEDEV
#include <flring_fc.h>
#include <bcmmsgbuf.h>
#include <wlfc_proto.h>
#endif /* BCMPCIEDEV */
#ifdef AP
#include <wlc_apps.h>
#endif
#endif /* PROP_TXSTATUS */
#include <wl_nfc.h>

#include <wlc_objregistry.h>

#ifdef WL_TBOW
#include <wlc_tbow.h>
#endif
#ifdef WL_SHIF
#include <wl_shub.h>
#endif
#include <wlc_iocv.h>

#include <rte_dev.h>
#include <rte_cons.h>
#include <rte_isr.h>
#include <rte.h>
#include <rte_mem.h>
#include <rte_ioctl.h>
#include <rte_timer.h>

#include <hnd_pktpool.h>
#include <hnd_pt.h>

#ifdef BCMPCIEDEV
#include <wlc_scb.h>
#include <wlc_key.h>

#if defined(BCMWAPI_WPI) || defined(BCMWAPI_WAI)
#include <wlc_wapi.h>
#endif
#include <rte_pktfetch.h>
#endif /* BCMPCIEDEV */
#ifdef WLWNM_AP
#include <wlc_wnm.h>
#endif /* WLWNM_AP */
#include <wlc_event.h>
#include <wlc_keymgmt.h>

#ifdef WL_MONITOR
#ifdef WL_STA_MONITOR
#include <wlc_stamon.h>
#endif /* WL_STA_MONITOR */
#endif /* WL_MONITOR */

#include <proto/monitor.h>

#ifdef	WLC_TSYNC
#include <wlc_tsync.h>
#endif /* WLC_TSYNC */

#ifdef HEALTH_CHECK
#include <hnd_hchk.h>
#endif

#ifdef WL_NATOE
#include <wl_natoe.h>
#endif

#include <phy_noise_api.h>

#ifdef HEALTH_CHECK
#define WL_HC_BUF_SIZE	256
#endif

typedef struct wl_cmn_data {
	void *osh_cmn;
	void *objreg;
} wl_cmn_data_t;

#define WL_IF(dev)   ((wl_if_t *)(dev->priv))

/* host wakeup filter flags */
#define HWFFLAG_UCAST	1		/* unicast */
#define HWFFLAG_BCAST	2		/* broadcast */

/* iovar table */
enum {
	IOV_HWFILTER,		/* host wakeup filter */
	IOV_DEEPSLEEP,		/* Deep sleep mode */
	IOV_DNGL_STATS,
	IOV_RTETIMESYNC,
	IOV_LAST		/* In case of a need to check max ID number */
};

static const bcm_iovar_t wl_iovars[] = {
	{"hwfilter", IOV_HWFILTER, 0, 0, IOVT_BUFFER, 0},
	{"deepsleep", IOV_DEEPSLEEP, 0, 0, IOVT_BOOL, 0},
	{"dngl_stats", IOV_DNGL_STATS, 0, 0, IOVT_BUFFER, 0},
	{"rte_timesync", IOV_RTETIMESYNC, 0, 0, IOVT_UINT32, 0},
	{NULL, 0, 0, 0, 0, 0}
};

#ifdef BCMDBG
static void do_wlmsg_cmd(void *arg, int argc, char *argv[]);
#endif
static void wl_statsupd(wl_info_t *wl);
static void wl_timer_main(hnd_timer_t *t);
static int wl_doiovar(void *hdl, uint32 actionid,
	void *p, uint plen, void *a, uint alen, uint vsize, struct wlc_if *wlcif);
/* Driver entry points */
void *wl_probe(hnd_dev_t *dev, volatile void *regs, uint bus,
	uint16 devid, uint coreid, uint unit);
static void wl_free(wl_info_t *wl, osl_t *osh);
#ifndef RTE_POLL
static void wl_isr(void *cbdata);
#ifdef THREAD_SUPPORT
static void wl_dpc_thread(void *cbdata);
#endif	/* THREAD_SUPPORT */
#endif /* !RTE_POLL */
static void wl_run(hnd_dev_t *dev);
static void _wl_dpc(hnd_timer_t *timer);
static void wl_dpc(wl_info_t *wl);
static int wl_open(hnd_dev_t *dev);
static int wl_send(hnd_dev_t *src, hnd_dev_t *dev, struct lbuf *lb);
static int wl_close(hnd_dev_t *dev);
#if defined(BCMPCIEDEV)
static void wl_tx_pktfetch(wl_info_t *wl, struct lbuf *lb,
	hnd_dev_t *src, hnd_dev_t *dev, wlc_bsscfg_t *bsscfg);
static void wl_send_cb(void *lbuf, void *orig_lfrag, void *ctx, bool cancelled);
static void wl_rx_pktfetch_cb(void *lbuf, void *orig_lfrag, void *ctx, bool cancelled);
#endif /* BCMPCIEDEV */
#ifdef BCMPCIEDEV
static int32 wl_flowring_update(hnd_dev_t *dev, uint16 flowid, uint8 op, uint8 * sa,
	uint8 *da, uint8 tid);
#endif /* BCMPCIEDEV */

static int wl_ioctl(hnd_dev_t *dev, uint32 cmd, void *buf, int len, int *used, int *needed,
	int set);

#ifdef TOE
static void _wl_toe_send_proc(wl_info_t *wl, void *p);
static void _wl_toe_recv_proc(wl_info_t *wl, void *p);
#endif /* TOE */

static hnd_dev_ops_t wl_funcs = {
	probe:		wl_probe,
	open:		wl_open,
	close:		wl_close,
	xmit:		wl_send,
	ioctl:		wl_ioctl,
	poll:           wl_run,
};

#ifndef NUMD11CORES
#define  NUMD11CORES 1
#endif

hnd_dev_t bcmwl[ /* NUMD11CORES */ ] = {
#if (NUMD11CORES == 2)
	{
		name:"wl",
		ops:&wl_funcs
	},
#endif
	{
		name:"wl",
		ops:&wl_funcs
	}
};

#define IOCTL_DBG_STR "wl%d: %s: bus ioctl %d failed with error %d\n"
#define POOL_ERR_STR "wl%d: %s[%d]: Pool fill failed with error %d\n"

#ifdef EXT_STA
static void wl_rx_ctxt_push(wl_info_t *wl, void *pkt);

/**
 * rx_ctxt_t is included per packet under EXT_STA. We push the packet and copy it to the front.
 * From host we need to retrieve it from front and and pull it back.
 */
static void wl_rx_ctxt_push(wl_info_t *wl, void *pkt)
{
	rx_ctxt_t * rx_ctxt;
	wlc_pkttag_t *pkttag = PKTTAG(pkt);

	/* Push the packet to make space for rx context */
	PKTPUSH(wl->pub->osh, pkt, sizeof(rx_ctxt_t));
	rx_ctxt = (rx_ctxt_t *) PKTDATA(wl->pub->osh, pkt);

	/* convert rpsec to rate in 500Kbps units */
	rx_ctxt->rate = RSPEC2KBPS(pkttag->rspec) / 500;
	rx_ctxt->rssi = pkttag->pktinfo.misc.rssi;
	rx_ctxt->channel = pkttag->rxchannel;
}
#endif /* EXT_STA */

/* This includes the auto generated ROM IOCTL/IOVAR patch handler C source file (if auto patching is
 * enabled). It must be included after the prototypes and declarations above (since the generated
 * source file may reference private constants, types, variables, and functions).
 */
#include <wlc_patch.h>

static int
wl_doiovar(void *hdl, uint32 actionid,
           void *p, uint plen, void *arg, uint alen, uint vsize, struct wlc_if *wlcif)
{
	wl_info_t *wl = (wl_info_t *)hdl;
	wlc_info_t *wlc = wl->wlc;
	int32 int_val = 0;
	int32 *ret_int_ptr;
	bool bool_val;
	int err = 0;
	int radio;

	/* convenience int and bool vals for first 8 bytes of buffer */
	if (plen >= (int)sizeof(int_val))
		bcopy(p, &int_val, sizeof(int_val));
	bool_val = (int_val != 0) ? TRUE : FALSE;

	/* convenience int ptr for 4-byte gets (requires int aligned arg) */
	ret_int_ptr = (int32 *)arg;

	switch (actionid) {
	case IOV_GVAL(IOV_HWFILTER):
		*ret_int_ptr = wl->hwfflags;
		break;

	case IOV_SVAL(IOV_HWFILTER):
		wl->hwfflags = (uint8)int_val;
		break;

	case IOV_GVAL(IOV_DEEPSLEEP):
		if ((err = wlc_get(wlc, WLC_GET_RADIO, &radio)))
			break;
		*ret_int_ptr = (radio & WL_RADIO_SW_DISABLE) ? TRUE : FALSE;
		break;

	case IOV_SVAL(IOV_DEEPSLEEP):
		wlc_set(wlc, WLC_SET_RADIO, (WL_RADIO_SW_DISABLE << 16)
		        | (bool_val ? WL_RADIO_SW_DISABLE : 0));
		/* suspend or resume timers */
		if (bool_val)
			hnd_suspend_timer();
		else
			hnd_resume_timer();
		break;

	case IOV_GVAL(IOV_DNGL_STATS):
		wl_statsupd(wl);
		bcopy(&wl->stats, arg, MIN(plen, sizeof(wl->stats)));
		break;



	default:
		err = BCME_UNSUPPORTED;
	}

	return err;
} /* wl_doiovar */

static void
BCMINITFN(_wl_init)(wl_info_t *wl)
{
	wl_reset(wl);

	wlc_init(wl->wlc);
}

void
wl_init(wl_info_t *wl)
{
	WL_TRACE(("wl%d: wl_init\n", wl->unit));

		_wl_init(wl);
}

uint
BCMINITFN(wl_reset)(wl_info_t *wl)
{
	WL_TRACE(("wl%d: wl_reset\n", wl->unit));

	wlc_reset(wl->wlc);

	/* during big hammer wlc discards
	 * pending interrupts.
	 * Stop dpc timer to cancel dpc hanling
	 */
	if (wl->dpctimer_armed) {
		hnd_timer_stop(wl->dpcTimer);
		wl->dpctimer_armed = FALSE;
	}

	return 0;
}

bool
BCMUCODEFN(wl_alloc_dma_resources)(wl_info_t *wl, uint addrwidth)
{
	return TRUE;
}

/**
 * These are interrupt on/off enter points.
 * Since wl_run is serialized with other drive rentries using spinlock,
 * They are SMP safe, just call common routine directly,
 */
void
wl_intrson(wl_info_t *wl)
{
	wlc_intrson(wl->wlc);
}

uint32
wl_intrsoff(wl_info_t *wl)
{
	return wlc_intrsoff(wl->wlc);
}

void
wl_intrsrestore(wl_info_t *wl, uint32 macintmask)
{
	wlc_intrsrestore(wl->wlc, macintmask);
}

/** BMAC driver has alternative up/down etc. */
int
wl_up(wl_info_t *wl)
{
	int ret, err;
	BCM_REFERENCE(err);
	wlc_info_t *wlc = (wlc_info_t *) wl->wlc;

	WL_TRACE(("wl%d: wl_up\n", wl->unit));

	if (wl->pub->up)
		return 0;

#if defined(BCMNODOWN)
	if (bcm_reclaimed) {
		return (wlc_minimal_up(wl->wlc));
	}
#endif /* defined (BCMNODOWN) */

	if (wl->pub->up)
		return 0;

	/* Reset the hw to known state */
	ret = wlc_up(wlc);

#ifndef RSOCK

#ifdef BCMPKTPOOL
	if (POOL_ENAB(wl->pub->pktpool)) {
		err = hnd_pktpool_fill(wl->pub->pktpool, FALSE);
		if (err != BCME_OK) {
			WL_ERROR((POOL_ERR_STR, wl->unit, __FUNCTION__, __LINE__, err));
		}
#ifdef PROP_TXSTATUS
		wlc_send_credit_map(wl->wlc);
#endif /* PROP_TXSTATUS */
	}
#ifdef BCMFRAGPOOL
	if (POOL_ENAB(wl->pub->pktpool_lfrag)) {
		err = hnd_pktpool_fill(wl->pub->pktpool_lfrag, FALSE);
		if (err != BCME_OK) {
			WL_ERROR((POOL_ERR_STR, wl->unit, __FUNCTION__, __LINE__, err));
		}
	}
#endif /* BCMFRAGPOOL */
#ifdef BCMRXFRAGPOOL
	if (POOL_ENAB(wl->pub->pktpool_rxlfrag)) {
		err = hnd_pktpool_fill(wl->pub->pktpool_rxlfrag, FALSE);
		if (err != BCME_OK) {
			WL_ERROR((POOL_ERR_STR, wl->unit, __FUNCTION__, __LINE__, err));
		}
	}
#endif /* BCMRXFRAGPOOL */
#endif /* BCMPKTPOOL */

#endif /* RSOCK */

	return ret;
}

void
wl_down(wl_info_t *wl)
{
	WL_TRACE(("wl%d: wl_down\n", wl->unit));
	if (!wl->pub->up)
		return;

#ifdef BCMNODOWN
	wlc_minimal_down(wl->wlc);
#else
	wlc_down(wl->wlc);
	wl->pub->hw_up = FALSE;
#endif /* BCMNODOWN */
	/* Handle RSDB mpc case */
	if (!wlc_isup(wl->wlc)) {
		wl_indicate_maccore_state(wl, LTR_SLEEP);
	}
}

void
wl_dump_ver(wl_info_t *wl, struct bcmstrbuf *b)
{
	bcm_bprintf(b, "wl%d: %s %s version %s FWID 01-%x\n", wl->unit,
		__DATE__, __TIME__, EPI_VERSION_STR, gFWID);
}

#if defined(BCMDBG) || defined(WLDUMP)
static int
wl_dump(wl_info_t *wl, struct bcmstrbuf *b)
{
	wl_dump_ver(wl, b);

	return 0;
}
#endif /* BCMDBG || WLDUMP */

void
wl_monitor(wl_info_t *wl, wl_rxsts_t *rxsts, void *p)
{
#ifdef WL_MONITOR
	wlc_info_t *wlc = wl->wlc;
	struct lbuf *mon_pkt;
	mon_pkt = (struct lbuf *)p;
#ifdef BCMPCIEDEV
	wlc_d11rxhdr_t *wrxh;

	PKTPUSH(wlc->osh, mon_pkt, wlc->hwrxoff);

	wrxh = (wlc_d11rxhdr_t *)PKTDATA(wlc->osh, mon_pkt);

	if (RXFIFO_SPLIT() && !PKT_CLASSIFY_EN(D11RXHDR_ACCESS_VAL(&wrxh->rxhdr,
		wlc->pub->corerev, fifo)))
		PKTPULL(wlc->osh, mon_pkt, wlc->hwrxoff);

	((int8*)PKTTAG(mon_pkt))[MON_PKTTAG_NOISE_IDX] = phy_noise_avg((wlc_phy_t *)
		WLC_PI_BANDUNIT(wlc, CHSPEC_WLCBANDUNIT(D11RXHDR_ACCESS_VAL(&wrxh->rxhdr,
			wlc->pub->corerev, RxChan))));
	((int8*)PKTTAG(mon_pkt))[MON_PKTTAG_RSSI_IDX] = wrxh->rssi;

	PKTSET80211(mon_pkt);
	PKTSETMON(mon_pkt);
#else
	wlc_pkttag_t * pkttag = WLPKTTAG(p);
	wlc_pkttag_t * mon_pkttag;
	mon_pkttag = WLPKTTAG(mon_pkt);

	uint len = PKTLEN(wlc->osh, p) - D11_PHY_HDR_LEN;

	if ((mon_pkt = PKTGET(wlc->osh, len, FALSE)) == NULL)
		return;

	bcopy(PKTDATA(wlc->osh, p) + D11_PHY_HDR_LEN,
		PKTDATA(wlc->osh, mon_pkt),
		len);

	mon_pkttag->rxchannel = pkttag->rxchannel;
	mon_pkttag->pktinfo.misc.rssi = pkttag->pktinfo.misc.rssi;
	mon_pkttag->rspec = pkttag->rspec;

#endif /* BCMPCIEDEV */
	wl_sendup(wl, NULL, mon_pkt, 1);

#endif /* WL_MONITOR */
}

void
wl_set_monitor(wl_info_t *wl, int val)
{
#ifdef WL_MONITOR
	wlc_info_t *wlc = wl->wlc;
	wlc_tunables_t *tune = wlc->pub->tunables;

	/* Disable copycount */
	wl_set_copycount_bytes(wl, val ? 0 : tune->copycount, wlc->hwrxoff);

	if (RXFIFO_SPLIT()) {
		/* Disable Header conversion */
		wlc_update_splitrx_mode(wlc->hw, !val, FALSE);
	}
	wl->monitor_type = (uint32)val;
#endif /* WL_MONITOR */
}

char *
wl_ifname(wl_info_t *wl, struct wl_if *wlif)
{
	if (wlif == NULL)
		return wl->dev->name;
	else
		return wlif->dev->name;
}

static hnd_dev_ops_t*
get_wl_funcs(void)
{
	return &wl_funcs;
}

/** Allocate wl_if_t, hnd_dev_t, and wl_if_t * all together */
static wl_if_t *
wl_alloc_if(wl_info_t *wl, int iftype, uint subunit, struct wlc_if *wlcif, bool rebind)
{
	hnd_dev_t *dev;
	wl_if_t *wlif;
	osl_t *osh = wl->pub->osh;
	hnd_dev_t *bus = wl->dev->chained;
	uint len;
	int ifindex;

	/* the primary device must be binded to the bus */
	if (bus == NULL) {
		WL_ERROR(("wl%d: %s: device not binded\n", wl->pub->unit, __FUNCTION__));
		return NULL;
	}

	/* make sure subsequent allocations are nonpersistent */
	MALLOC_SET_NOPERSIST(osh);

	/* allocate wlif struct + dev struct (which includes a 'priv' pointer) */
	len = sizeof(wl_if_t) + sizeof(hnd_dev_t);
	if ((wlif = MALLOCZ(osh, len)) == NULL) {
		WL_ERROR((WLC_MALLOC_ERR, (wl->pub)?wl->pub->unit:subunit, __FUNCTION__, (int)len,
			MALLOCED(wl->pub->osh)));
		goto err;
	}
	bzero(wlif, len);

	dev = (hnd_dev_t *)(wlif + 1);

	wlif->dev = dev;
	wlif->wlcif = wlcif;

	dev->ops = get_wl_funcs();
	dev->softc = wl;
	if (iftype == WL_IFTYPE_WDS) {
		wlc_bsscfg_t *cfg = wlc_bsscfg_find_by_wlcif(wl->wlc, wlcif);
		ASSERT(cfg != NULL);
		(void)snprintf(dev->name, HND_DEV_NAME_MAX, "wds%d.%d.%d", wl->pub->unit,
				WLC_BSSCFG_IDX(cfg), subunit);

	} else {
		(void)snprintf(dev->name, HND_DEV_NAME_MAX, "wl%d.%d", wl->pub->unit, subunit);
	}

	dev->priv = wlif;

	/* use the return value as the i/f no. in the event to the host */
	if (!rebind) {
		ifindex = subunit;
		wlcif->index = (uint8)ifindex;
	}

	/* create and populate arpi for this IF */
	if (ARPOE_SUPPORT(wl->pub)) {
		wlif->arpi = wl_arp_alloc_ifarpi(wl->arpi, wlcif);
	}

#if defined(WLNDOE)
	if (NDOE_SUPPORT(wl->pub))
		wlif->ndi = wl_nd_alloc_ifndi(wl->ndi, wlcif);
#endif

#ifdef WLNFC
	if (NFC_ENAB(wl->pub)) {
		wlif->nfci = wl_nfc_alloc_ifnfci(wl->nfci, wlcif);
	}
#endif

	/* clear nopersist flag */
	MALLOC_CLEAR_NOPERSIST(osh);

	return wlif;

err:
	if (wlif != NULL)
		MFREE(osh, wlif, len);
	return NULL;
} /* wl_alloc_if */

static void
wl_free_if(wl_info_t *wl, wl_if_t *wlif)
{
	/* free arpi for this IF */
	if (ARPOE_SUPPORT(wl->pub)) {
		wl_arp_free_ifarpi(wlif->arpi);
	}

#ifdef WLNDOE
	/* free ndi for this IF */
	if (NDOE_SUPPORT(wl->pub)) {
		wl_nd_free_ifndi(wlif->ndi);
	}
#endif

#ifdef WLNFC
	/* free nfci for this IF */
	if (NFC_ENAB(wl->pub)) {
		wl_nfc_free_ifnfci(wlif->nfci);
	}
#endif

	MFREE(wl->pub->osh, wlif, sizeof(wl_if_t) + sizeof(hnd_dev_t));
}

/** called by WLC layer when adding a *secondary* interface */
struct wl_if *
wl_add_if(wl_info_t *wl, struct wlc_if *wlcif, uint unit, struct ether_addr *remote)
{
	wl_if_t *wlif;

	wlif = wl_alloc_if(wl, remote != NULL ? WL_IFTYPE_WDS : WL_IFTYPE_BSS, unit, wlcif, FALSE);

	if (wlif == NULL) {
		WL_ERROR(("wl%d: %s: failed to create %s interface %d\n", wl->pub->unit,
			__FUNCTION__, (remote)?"WDS":"BSS", unit));
		return NULL;
	}

	return wlif;
}

#ifdef WLRSDB
/** Update the wl pointer for RSDB bsscfg Move */
void
wl_update_if(struct wl_info *from_wl, struct wl_info *to_wl, wl_if_t *from_wlif,
	struct wlc_if *to_wlcif)
{
	wl_if_t *new_wlif;
	struct wlc_if * from_wlcif;
	uint8 cur_ifindex = 0;
	hnd_dev_t *bus = from_wl->dev->chained;
	bool free_to_wl = TRUE;
	ASSERT(from_wlif->wlcif != NULL);
	if (!(WLC_IF_IS_VIRTUAL(from_wlif->wlcif))) { /* primary bsscfg */
		bus_ops->rebinddev(bus, to_wl->dev, 0); /* real dev  is passed */
#ifdef ARPOE
	if (ARPOE_ENAB(to_wl->pub))
		wl_arp_clone_arpi(from_wl->arpi, to_wl->arpi);
#endif
#ifdef WLNDOE
	if (NDOE_ENAB(to_wl->pub))
		wl_nd_clone_ifndi(from_wl->ndi, to_wl->ndi);
#endif
	} else {
		cur_ifindex = from_wlif->wlcif->index;
		ASSERT(from_wlif->wlcif->type == WL_IFTYPE_BSS);
		from_wlcif = from_wlif->wlcif;

		if (to_wlcif->wlif) {
			int os_idx;
			new_wlif = to_wlcif->wlif;
			free_to_wl = FALSE;
			os_idx = wl_find_if(from_wlif);
			if (os_idx == -1) {
				WL_ERROR(("%s There is no idx associated with dev %p\n",
					__FUNCTION__, from_wlif->dev));
				return;
			}
			wl_rebind_if(new_wlif, os_idx, TRUE);
		} else {
			new_wlif = wl_alloc_if(to_wl, from_wlif->wlcif->type,
				WLC_BSSCFG_IDX(from_wlcif->u.bsscfg), to_wlcif, TRUE);
			ASSERT(new_wlif);
			free_to_wl = TRUE;
			/* virtual dev is passed */
			bus_ops->rebinddev(bus, new_wlif->dev, cur_ifindex);
			to_wlcif->index = cur_ifindex;
		}
		/*  Modify the device name for new interface.
		 * OS is not aware of bsscfg move during join
		 * and still sees the old interface name (ex wl0.2)
		 */
		strncpy(wl_ifname(to_wl, new_wlif), wl_ifname(from_wl, from_wlif),
			HND_DEV_NAME_MAX);
#ifdef ARPOE
		if (ARPOE_ENAB(to_wl->pub))
			wl_arp_clone_arpi(from_wlif->arpi, new_wlif->arpi);
#endif
#ifdef WLNDOE
		if (NDOE_ENAB(to_wl->pub))
			wl_nd_clone_ifndi(from_wlif->ndi, new_wlif->ndi);
#endif
		if (free_to_wl == TRUE) {
			to_wlcif->wlif = new_wlif;
			wl_free_if(from_wl, from_wlif);
			from_wlcif->wlif = NULL;
		}
	}
#ifdef WL_NATOE
	if (NATOE_ENAB(to_wl->pub)) {
		wl_natoe_notify_wlif_rsdb_upd(to_wl, TRUE, to_wlcif->wlif);
	}
#endif
}
#endif /* WLRSDB */

#include "proto/monitor.h"

int wl_find_if(struct wl_if *wlif)
{
	hnd_dev_t *dev = wlif->dev;
	hnd_dev_t *bus = dev->chained;
	if (bus_ops->findif) {
		return bus_ops->findif(bus, dev);
	} else {
		return -1;
	}
}

int wl_rebind_if(struct wl_if *wlif, int idx, bool rebind)
{
	hnd_dev_t *dev = wlif->dev;
	hnd_dev_t *bus = dev->chained;
	if (bus_ops->rebind_if) {
		return bus_ops->rebind_if(bus, dev, idx, rebind);
	} else {
		return BCME_OK;
	}
}

void
wl_del_if(wl_info_t *wl, struct wl_if *wlif)
{
	WL_TRACE(("wl%d: %s: bus_unbinddev idx %d\n", wl->pub->unit, __FUNCTION__,
		wlif->wlcif->index));
	wl_free_if(wl, wlif);
}

static void
wl_timer_main(hnd_timer_t *t)
{
	hnd_timer_auxfn_t auxfn = hnd_timer_get_auxfn(t);
	void *data = hnd_timer_get_data(t);

	ASSERT(auxfn != NULL);
	auxfn(data);
}

#undef wl_init_timer	/* see space saving hack in wl_export.h */

struct wl_timer *
wl_init_timer(wl_info_t *wl, void (*fn)(void* arg), void *arg, const char *name)
{
	return (struct wl_timer *)hnd_timer_create(wl, arg, wl_timer_main, fn, NULL);
}

void
wl_free_timer(wl_info_t *wl, struct wl_timer *t)
{
	hnd_timer_free((hnd_timer_t *)t);
}

void
wl_add_timer(wl_info_t *wl, struct wl_timer *t, uint ms, int periodic)
{
	ASSERT(t != NULL);
	hnd_timer_start((hnd_timer_t *)t, ms, periodic);
}

bool
wl_del_timer(wl_info_t *wl, struct wl_timer *t)
{
	if (t == NULL)
		return TRUE;
	return hnd_timer_stop((hnd_timer_t *)t);
}

#ifdef PROP_TXSTATUS
#ifdef PROP_TXSTATUS_DEBUG
static void
wl_wlfc_info_dump(void *arg, int argc, char *argv[])
{
	wl_info_t *wl = (wl_info_t *)arg;

	wlc_wlfc_info_dump(wl->wlc);
#ifdef AP
	{
	int hi = 0;
	int lo = 0;

	if (argc > 2) {
		hi = atoi(argv[1]);
		lo = atoi(argv[2]);
	}
	wlc_apps_dbg_dump(wl->wlc, hi, lo);
	}
#endif /* AP */
}
#endif /* PROP_TXSTATUS_DEBUG */
#endif /* PROP_TXSTATUS */

#define	CID_FMT_HEX	0x9999

#ifdef FWID
static const char BCMATTACHDATA(rstr_fmt_hello)[] =
	"wl%d: Broadcom BCM%s 802.11 Wireless Controller %s FWID 01-%x\n";
#else
static const char BCMATTACHDATA(rstr_fmt_hello)[] =
	"wl%d: Broadcom BCM%s 802.11 Wireless Controller %s\n";
#endif /* FWID */

static void
wl_devpwrstchg_notify(void *wl_p, bool hostmem_access_enabled)
{
	wl_info_t *wl = (wl_info_t *)wl_p;
	WL_PRINT(("%s: hostmem_access_enabled %d\n", __FUNCTION__, hostmem_access_enabled));
	wlc_devpwrstchg_change(wl->wlc, hostmem_access_enabled);
}
static void
wl_generate_pme_to_host(void *wl_p, bool state)
{
	wl_info_t *wl = (wl_info_t *)wl_p;
	wlc_generate_pme_to_host(wl->wlc, state);
}

static uint32
wldev_handle_trap(void *wl_p, uint8 trap_type)
{
	wl_info_t *wl = (wl_info_t *)wl_p;
	return wlc_halt_device(wl->wlc);
}

#if defined(MSGTRACE) || defined(LOGTRACE)
static void
wl_event_sendup_trace(void *ctx, uint8 *hdr, uint16 hdrlen, uint8 *buf, uint16 buflen)
{
	wl_info_t *wl = (wl_info_t *)ctx;
	wlc_info_t *wlc = wl->wlc;
	void *p;
	bcm_event_t *msg;
	char *ptr, *databuf;
	struct lbuf *lb;
	uint16 len;
	osl_t *osh = wl->pub->osh;
	hnd_dev_t *bus = wl->dev->chained;

	if (bus == NULL)
		return;

	if (!wlc_eventq_test_ind(wlc->eventq, WLC_E_TRACE))
		return;

	len = hdrlen + buflen;
	ASSERT(len < (wlc->pub->tunables->rxbufsz - sizeof(bcm_event_t) - 2));

	if ((p = PKTGET(osh, wlc->pub->tunables->rxbufsz, FALSE)) == NULL) {
		return;
	}

	ASSERT(ISALIGNED(PKTDATA(osh, p), sizeof(uint32)));

	/* make room for headers; ensure we start on an odd 16 bit offset */
	PKTPULL(osh, p, BCMEXTRAHDROOM + 2);

	msg = (bcm_event_t *)PKTDATA(osh, p);

	msg->eth.ether_type = hton16(ETHER_TYPE_BRCM);

	/* BCM Vendor specific header... */
	msg->bcm_hdr.subtype = hton16(BCMILCP_SUBTYPE_VENDOR_LONG);
	msg->bcm_hdr.version = BCMILCP_BCM_SUBTYPEHDR_VERSION;
	bcopy(BRCM_OUI, &msg->bcm_hdr.oui[0], DOT11_OUI_LEN);
	/* vendor spec header length + pvt data length (private indication hdr + actual message
	 * itself)
	 */
	msg->bcm_hdr.length = hton16(BCMILCP_BCM_SUBTYPEHDR_MINLENGTH + BCM_MSG_LEN + (uint16)len);
	msg->bcm_hdr.usr_subtype = hton16(BCMILCP_BCM_SUBTYPE_EVENT);

	PKTSETLEN(osh, p, (sizeof(bcm_event_t) + len + 2));

	/* update the event struct */
	/* translate the wlc event into bcm event msg */
	msg->event.version = hton16(BCM_EVENT_MSG_VERSION);
	msg->event.event_type = hton32(WLC_E_TRACE);
	msg->event.status = hton32(WLC_E_STATUS_SUCCESS);
	msg->event.reason = 0;
	msg->event.auth_type = 0;
	msg->event.datalen = hton32(len);
	msg->event.flags = 0;
	msg->event.addr = wlc->perm_etheraddr;
	bzero(msg->event.ifname, sizeof(msg->event.ifname));
	msg->event.ifidx = 0;
	msg->event.flags |= WLC_EVENT_MSG_UNKIF;
	msg->event.bsscfgidx = 0;
	msg->event.flags |= WLC_EVENT_MSG_UNKBSS;

	PKTSETLEN(osh, p, (sizeof(bcm_event_t) + len + 2));

	/* Copy the data */
	databuf = (char *)(msg + 1);
	bcopy(hdr, databuf, hdrlen);
	bcopy(buf, databuf+hdrlen, buflen);

	ptr = (char *)databuf;

	PKTSETMSGTRACE(p, TRUE);

	/* Last 2 bytes of the message are 0x00 0x00 to signal that there are no ethertypes which
	 * are following this
	 */
	ptr[len+0] = 0x00;
	ptr[len+1] = 0x00;
	lb = PKTTONATIVE(osh, p);

	if (bus->ops->xmit(NULL, bus, lb) != 0) {
		lb_free(lb);
	}
}
#endif /* MSGTRACE || LOGTRACE */

void *
BCMATTACHFN(wl_probe)(hnd_dev_t *dev, volatile void *regs, uint bus, uint16 devid,
                      uint coreid, uint unit)
{
	wl_info_t *wl;
	wlc_info_t *wlc;
	wl_cmn_data_t *pdata = NULL;
	osl_t *osh;
	uint err;
	char cidstr[8];
	uint orig_unit = unit;
	struct wl_if *wlif;     /**< primary wl interface */

#ifdef BCMPCIEDEV_ENABLED
	/* When we have two full dongle radios hooked up to the host (as is the case
	 * with router platforms), we need to make sure the instances come up with
	 * different wl units - wl0 and wl1. But, since one dongle is not aware of
	 * the other, we need this information to come from the host.
	 */
	uint wlunit = 0;
	extern char *_vars;
	wlunit = getintvar(_vars, "wlunit");
	unit += wlunit;
#endif /* BCMPCIEDEV_ENABLED */

	BCM_REFERENCE(orig_unit);

	if (dev->commondata)
		pdata = (wl_cmn_data_t *)dev->commondata;
	else {
		/* allocate private info */
		if (!(pdata = dev->commondata =
			(wl_cmn_data_t *)MALLOCZ(NULL, sizeof(wl_cmn_data_t)))) {
			WL_ERROR((WLC_MALLOC_ERR, unit, __FUNCTION__, (int)sizeof(wl_cmn_data_t),
				0));
			return NULL;
		}
	}

	/* allocate private info */
	if (!(wl = (wl_info_t *)MALLOCZ(NULL, sizeof(wl_info_t)))) {
		WL_ERROR((WLC_MALLOC_ERR, unit, __FUNCTION__, (int)sizeof(wl_info_t), 0));
		return NULL;
	}

	wl->unit = unit;
	wl->dev = dev;

#ifdef SHARED_OSL_CMN
	/* Need to pass existing osl_cmn */
	if (pdata)
		osh = osl_attach(dev, &pdata->osh_cmn);
	else
		osh = osl_attach(dev, NULL);
#else
	osh = osl_attach(dev);
#endif /* SHARED_OSL_CMN */

#if (defined(HEALTH_CHECK) && !defined(HEALTH_CHECK_DISABLED))
	/* Request a block for WL health check */
	wl->hc = health_check_init(osh,
		(unit == 0) ? HCHK_SW_ENTITY_WL_PRIMARY : HCHK_SW_ENTITY_WL_SECONDARY,
		WL_HC_BUF_SIZE);

	if (wl->hc == NULL) {
		WL_ERROR(("wl%d: Health check init failed\n", unit));
		goto fail;
	}
#endif

#ifdef WL_OBJ_REGISTRY
	/* Registry is to be created only once for RSDB;
	 * Both WLC will share information over wlc->objr
	*/
	if (pdata) {
		if (!pdata->objreg) {
			wl->objr =
				obj_registry_alloc(osh, OBJR_MAX_KEYS);
				obj_registry_set(wl->objr, OBJR_SELF, wl->objr);
			pdata->objreg = (void *) wl->objr;
		} else {
			wl->objr = pdata->objreg;
		}
	}

	obj_registry_ref(wl->objr, OBJR_SELF);
#endif /* WL_OBJ_REGISTRY */

	/* common load-time initialization */
	if (!(wlc = wlc_attach(wl,			/* wl */
			VENDOR_BROADCOM,		/* vendor */
			devid,			/* device */
			unit,			/* unit */
			FALSE,			/* piomode */
			osh,			/* osh */
			regs,			/* regsva */
			bus,			/* bustype */
			wl,			/* sdh */
			wl->objr,		/* Registry for RSDB usage */
			&err))) {		/* perr */
		WL_ERROR(("wl%d: wlc_attach failed with error %d\n", unit, err));
		goto fail;
	}

	wl->wlc = (void *)wlc;
	wl->pub = wlc_pub((void *)wlc);
	wl->wlc_hw = wlc->hw;

	wl->dpcTimer = hnd_timer_create(NULL, wl, _wl_dpc, NULL, NULL);
	if (wl->dpcTimer == NULL)
		goto fail;

	(void)snprintf(dev->name, HND_DEV_NAME_MAX, "wl%d", unit);

	if (si_chipid(wlc->hw->sih) < CID_FMT_HEX)
		(void)snprintf(cidstr, sizeof(cidstr), "%04x", si_chipid(wlc->hw->sih));
	else
		(void)snprintf(cidstr, sizeof(cidstr), "%d", si_chipid(wlc->hw->sih));

#ifdef FWID
	/* print hello string */
	printf(rstr_fmt_hello, unit, cidstr, EPI_VERSION_STR, gFWID);
#else
	/* print hello string */
	printf(rstr_fmt_hello, unit, cidstr, EPI_VERSION_STR);
#endif

#ifndef RTE_POLL
	if (hnd_isr_register(0, coreid, orig_unit, wl_isr, dev,
		wl_dpc_thread, dev, NULL, bus) != BCME_OK)
	{
		WL_ERROR(("wl%d: hnd_isr_register failed\n", unit));
		goto fail;
	}
#endif	/* !RTE_POLL */

#ifdef PROP_TXSTATUS
#ifdef PROP_TXSTATUS_DEBUG
	if (PROP_TXSTATUS_ENAB(wlc->pub)) {
		if (!hnd_cons_add_cmd("np", wl_wlfc_info_dump, wl)) {
			WL_ERROR(("wl%d: hnd_cons_add_cmd failed\n", unit));
			goto fail;
		}
	}
#endif
#endif /* PROP_TXSTATUS */

#ifdef BCMPCIEDEV
	/* Initialize the flowring_link_update at run-time */
	if (BCMPCIEDEV_ENAB()) {
		wl_funcs.flowring_link_update = wl_flowring_update;
	}
#endif /* BCMPCIEDEV */

#ifdef STA
	/* algin watchdog with tbtt indication handling in PS mode */
	wl->pub->align_wd_tbtt = TRUE;
	wlc_bmac_set_defmacintmask(wlc->hw, MI_TTTT, MI_TTTT);

	/* Enable TBTT Interrupt */
	wlc_bmac_enable_tbtt(wlc->hw, TBTT_WD_MASK, TBTT_WD_MASK);
#endif

	wlc_eventq_set_ind(wlc->eventq, WLC_E_IF, TRUE);

#if defined(WLPFN) && !defined(WLPFN_DISABLED)
	/* initialize PFN handler state */
	if ((wl->pfn = wl_pfn_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_pfn_attach failed\n", unit));
		goto fail;
	}
	wl->pub->cmn->_wlpfn = TRUE;
#endif /* WLPFN */

#if defined(TOE) && !defined(TOE_DISABLED)
	/* allocate the toe info struct */
	if ((wl->toei = wl_toe_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_toe_attach failed\n", unit));
		goto fail;
	}
#endif /* TOE && !TOE_DISABLED */
#if defined(KEEP_ALIVE) && !defined(KEEP_ALIVE_DISABLED)
	/* allocate the keep-alive info struct */
	if ((wl->keep_alive_info = wl_keep_alive_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_keep_alive_attach failed\n", unit));
		goto fail;
	}
#endif /* KEEP_ALIVE && !KEEP_ALIVE_DISABLED */
#ifdef WL_EVENTQ
	/* allocate wl_eventq info struct */
	if ((wl->wlevtq = wl_eventq_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_eventq_attach failed\n", unit));
		goto fail;
	}
#endif /* WL_EVENTQ */
#if (defined(P2PO) && !defined(P2PO_DISABLED)) || (defined(ANQPO) && \
	!defined(ANQPO_DISABLED))
	/* allocate gas info struct */
	if ((wl->gas = wl_gas_attach(wlc, wl->wlevtq)) == NULL) {
		WL_ERROR(("wl%d: wl_gas_attach failed\n", unit));
		goto fail;
	}
#endif /* P2PO || ANQPO */

#if (defined(P2PO) || defined(P2POELO)) && !defined(P2PO_DISABLED)
	/* allocate the disc info struct */
	if ((wl->disc = wl_disc_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_disc_attach failed\n", unit));
		goto fail;
	}

	/* allocate the p2po info struct */
	if ((wl->p2po = wl_p2po_attach(wlc, wl->wlevtq, wl->gas, wl->disc)) == NULL) {
		WL_ERROR(("wl%d: wl_p2po_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(P2PO) && !defined(P2PO_DISABLED) */

#if defined(ANQPO) && !defined(ANQPO_DISABLED)
	/* allocate the anqpo info struct */
	if ((wl->anqpo = wl_anqpo_attach(wlc, wl->gas)) == NULL) {
		WL_ERROR(("wl%d: wl_anqpo_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(ANQPO) && !defined(ANPO_DISABLED) */

#if defined(BDO) && !defined(BDO_DISABLED)
	/* allocate the bdo info struct */
	if ((wl->bdo = wl_bdo_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_bdo_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(BDO) && !defined(BDO_DISABLED) */

#if defined(PACKET_FILTER) && !defined(PKT_FLT_DISABLED)
	/* allocate the packet filter info struct */
	if ((wl->pkt_filter_info = wlc_pkt_filter_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wlc_pkt_filter_attach failed\n", unit));
		goto fail;
	}
#endif /* PACKET_FILTER && !PKT_FLT_DISABLED */

#if defined(D0_COALESCING)
	/* allocate the packet filter info struct */
	if ((wl->d0_filter_info = wlc_d0_filter_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wlc_pkt_filter_attach failed\n", unit));
		goto fail;
	}
#endif /* D0_COALESCING */


#if defined(WLNDOE) && !defined(WLNDOE_DISABLED)
	/* allocate the nd info struct */
	if ((wl->ndi = wl_nd_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_nd_attach failed\n", unit));
		goto fail;
	}
	wl->pub->_ndoe_support = TRUE;
#endif
#if defined(WLNFC) && !defined(WLNFC_DISABLED)
	/* allocate the nfc info struct */
	if ((wl->nfci = wl_nfc_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_nfc_attach failed\n", unit));
		goto fail;
	}
#endif /* WLNFC && !WLNFC_DISABLED */
#if defined(ARPOE) && !defined(ARPOE_DISABLED)
	/* allocate the arp info struct */
	if ((wl->arpi = wl_arp_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_arp_attach failed\n", unit));
		goto fail;
	}
	wl->pub->_arpoe_support = TRUE;
#endif /* ARPOE && !ARPOE_DISABLED */

#if defined(WL_NATOE) && !defined(WL_NATOE_DISABLED)
	/* allocate the natoe info struct */
	if ((wl->natoe_info = wl_natoe_attach(wl)) == NULL) {
		WL_ERROR(("wl%d: wl_natoe_attach failed\n", unit));
		goto fail;
	}
#endif /* WL_NATOE && !WL_NATOE_DISABLED */

#if defined(AVS) && !defined(AVS_DISABLED)
	/* Register Adaptive Voltage Scaling module */
	if ((wl->avs = wl_avs_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_avs_attach failed\n", unit));
		goto fail;
	}
#endif /* AVS && !AVS_DISABLED */

#ifdef BCMDBG
	if (!hnd_cons_add_cmd("wlmsg", do_wlmsg_cmd, wl))
		goto fail;
#endif
	/* register module */
	if (wlc_module_register(wlc->pub, wl_iovars, "wl", wl, wl_doiovar, NULL, NULL, NULL)) {
		WL_ERROR(("wl%d: wlc_module_register() failed\n", unit));
		goto fail;
	}

#if defined(BCMDBG) || defined(WLDUMP)
	wlc_dump_register(wl->pub, "wl", (dump_fn_t)wl_dump, (void *)wl);
#endif

#ifdef MSGTRACE
	/* We set up the event and start the tracing immediately */
	wlc_eventq_set_ind(wlc->eventq, WLC_E_TRACE, TRUE);
	msgtrace_set_sendup_trace_fn(wl_event_sendup_trace, wl);
#endif

#ifdef LOGTRACE
	/* We set up the event and start the tracing immediately */
	wlc_eventq_set_ind(wlc->eventq, WLC_E_TRACE, TRUE);
	logtrace_set_sendup_trace_fn(wl_event_sendup_trace, wl);
	logtrace_start();
#endif

	if (hnd_register_trapnotify_callback(wldev_handle_trap, wl) < 0) {
		WL_ERROR(("wl:%d: Register trap callback failed\n", unit));
		goto fail;
	}

#if defined(TKO) && !defined(TKO_DISABLED)
	/* allocate the tko info struct */
	if ((wl->tko = wl_tko_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_tko_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(TKO) && !defined(TKO_DISABLED) */

#if defined(ICMP) && !defined(ICMP_DISABLED)
	/* allocate the icmp info struct */
	if ((wl->icmp = wl_icmp_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_icmp_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(ICMP) && !defined(ICMP_DISABLED) */

#if defined(WL_SHIF) && !defined(WL_SHIF_DISABLED)
	if ((wl->shub_info = wl_shub_attach(wlc)) == NULL) {
		WL_ERROR(("wl%d: wl_shub_attach failed\n", unit));
		goto fail;
	}
#endif /* defined(WL_SHIF) && !defined(WL_SHIF_DISABLED) */

	/* create and populate primary wl interface: */
	/* can be freed in wl_update_if */
	if ((wlif = MALLOCZ_NOPERSIST(osh, sizeof(wl_if_t))) == NULL) {
		WL_ERROR((WLC_MALLOC_ERR, (wl->pub) ? wl->pub->unit : 0, __FUNCTION__,
			sizeof(wl_if_t), MALLOCED(wl->pub->osh)));
		goto fail;
	}

	dev->priv   = wlif;
	wlif->wlcif = wlc_wlcif_get_by_index(wl->wlc,
			WLC_BSSCFG_IDX(wlc_bsscfg_primary(wl->wlc)));
	wlif->dev   = dev;
#ifdef WLNDOE
	wlif->ndi   = wl->ndi;
#endif /* WLNDOE */
	wlif->arpi  = wl->arpi;
	wlif->nfci  = wl->nfci;
	wlif->wlcif->wlif = wlif;

#ifdef HEALTH_CHECK
	/* register health check entity to be driven by wlc_watchdog() */
	/* Call health check on the descriptor WL holds.
	 * Other top level modules will also call (in their context)
	 * a similar function on the descriptor they hold.
	 */
	if (wlc_module_register(wlc->pub, NULL, "wl_hc", wl, NULL,
		wl_health_check_execute, NULL, NULL))
	{
		WL_ERROR(("wl%d: wlc_module_register() failed\n", unit));
		goto fail;
	}
#endif

	return (wl);

fail:
	wl_free(wl, osh);
	return NULL;
} /* wl_probe */

static void
BCMATTACHFN(wl_free)(wl_info_t *wl, osl_t *osh)
{
#ifndef BCMNODOWN
	if (wl->dpcTimer != NULL)
		hnd_timer_free(wl->dpcTimer);
#if defined(WL_NATOE) && !defined(WL_NATOE_DISABLED)
	if (wl->natoe_info) {
		wl_natoe_detach(wl->natoe_info);
	}
#endif /* WL_NATOE && !WL_NATOE_DISABLED */
#if defined(AVS) && !defined(AVS_DISABLED)
	if (wl->avs) {
		wl_avs_detach(wl->avs);
	}
#endif /* AVS && !AVS_DISABLED */
#if defined(ARPOE) && !defined(ARPOE_DISABLED)
	if (wl->arpi)
		wl_arp_detach(wl->arpi);
#endif /* ARPOE && !ARPOE_DISABLED */
#if defined(D0_COALESCING)
	if (wl->d0_filter_info)
		wlc_d0_filter_detach(wl->d0_filter_info);
#endif /* D0_COALESCING */

#if defined(PACKET_FILTER) && !defined(PKT_FLT_DISABLED)
	if (wl->pkt_filter_info)
		wlc_pkt_filter_detach(wl->pkt_filter_info);
#endif /* PACKET_FILTER && !PKT_FLT_DISABLED */
#if defined(KEEP_ALIVE) && !defined(KEEP_ALIVE_DISABLED)
	if (wl->keep_alive_info)
		wl_keep_alive_detach(wl->keep_alive_info);
#endif /* KEEP_ALIVE && !KEEP_ALIVE_DISABLED */
#if defined(TOE) && !defined(TOE_DISABLED)
	if (wl->toei)
		wl_toe_detach(wl->toei);
#endif /* TOE && !TOE_DISABLED */

#ifdef WLPFN
	if (WLPFN_ENAB(wl->pub) && wl->pfn)
		wl_pfn_detach(wl->pfn);
#endif
#if (defined(P2PO) || defined(P2POELO)) && !defined(P2PO_DISABLED)
	if (wl->p2po)
		wl_p2po_detach(wl->p2po);
	if (wl->disc)
		wl_disc_detach(wl->disc);
#endif /* defined(P2PO) && !defined(P2PO_DISABLED) */
#if defined(ANQPO) && !defined(ANQPO_DISABLED)
	if (wl->anqpo)
		wl_anqpo_detach(wl->anqpo);
#endif /* defined(ANQPO) && !defined(ANQPO_DISABLED) */
#if (defined(P2PO) && !defined(P2PO_DISABLED)) || (defined(ANQPO) && \
	!defined(ANQPO_DISABLED))
	if (wl->gas)
		wl_gas_detach(wl->gas);
#endif	/* P2PO || ANQPO */
#ifdef WL_EVENTQ
	if (wl->wlevtq)
		wl_eventq_detach(wl->wlevtq);
#endif /* WL_EVENTQ */
#if defined(BDO) && !defined(BDO_DISABLED)
	if (wl->bdo)
		wl_bdo_detach(wl->bdo);
#endif /* defined(BDO) && !defined(BDO_DISABLED) */

#if defined(HND_PT_GIANT) && defined(DMA_TX_FREE)
	hnd_pt_lowmem_unregister(&wl->lowmem_free_info);
#endif

#if defined(WLNDOE) && !defined(WLNDOE_DISABLED)
	if (wl->ndi)
		wl_nd_detach(wl->ndi);
#endif

#if defined(WLNFC) && !defined(WLNFC_DISABLED)
	if (wl->nfci)
		wl_nfc_detach(wl->nfci);
#endif

#if defined(TKO) && !defined(TKO_DISABLED)
	if (wl->tko)
		wl_tko_detach(wl->tko);
#endif /* defined(TKO) && !defined(TKO_DISABLED) */

#if defined(ICMP) && !defined(ICMP_DISABLED)
	if (wl->icmp)
		wl_icmp_detach(wl->icmp);
#endif /* defined(ICMP) && !defined(ICMP_DISABLED) */

#if defined(WL_SHIF) && !defined(WL_SHIF_DISABLED)
	if (wl->shub_info)
		wl_shub_detach(wl->shub_info);
#endif /* defined(WL_SHIF) && !defined(WL_SHIF_DISABLED) */

	/* common code detach */
	if (wl->wlc) {
		struct wlc_if *wlcif;     /**< primary wlc interface */
		wlcif = wlc_wlcif_get_by_index(wl->wlc,
			WLC_BSSCFG_IDX(wlc_bsscfg_primary(wl->wlc)));
		if (wlcif != NULL && wlcif->wlif != NULL)
			MFREE(osh, wlcif->wlif, sizeof(wl_if_t));
		wlc_detach(wl->wlc);
	}

#ifdef WL_OBJ_REGISTRY
	if (wl->objr && (obj_registry_unref(wl->objr, OBJR_SELF) == 0)) {
		obj_registry_set(wl->objr, OBJR_SELF, NULL);
		obj_registry_free(wl->objr, osh);
	}
#endif

#ifdef LOGTRACE
	logtrace_deinit(osh);
#endif /* LOGTRACE */

#endif /* BCMNODOWN */
	MFREE(osh, wl, sizeof(wl_info_t));
} /* wl_free */

#ifndef RTE_POLL
static void
wl_isr(void *cbdata)
{
	hnd_dev_t *dev = cbdata;
#ifdef THREAD_SUPPORT
	wl_info_t *wl = dev->softc;

	/* deassert interrupt */
	wlc_intrs_deassert(wl->wlc);
#else
	wl_run(dev);
#endif	/* THREAD_SUPPORT */
}

#ifdef THREAD_SUPPORT
/** THREAD_SUPPORT specific function */
static void
wl_dpc_thread(void *cbdata)
{
	hnd_dev_t *dev = cbdata;
	wl_run(dev);
}
#endif	/* THREAD_SUPPORT */
#endif /* !RTE_POLL */

static void
wl_run(hnd_dev_t *dev)
{
	wl_info_t *wl = dev->softc;
	bool dpc;

	WL_TRACE(("wl%d: wl_run\n", wl->unit));

	/* call common first level interrupt handler */
	if (wlc_isr(wl->wlc, &dpc)) {
		/* if more to do... */
		if (dpc) {
			wl_dpc(wl);
		}
	}
}

static void
wl_dpc(wl_info_t *wl)
{
	bool resched = 0;
	bool bounded = TRUE;

	/* call the common second level interrupt handler if we have enough memory */
	if (wl->wlc_hw->up) {
		wlc_dpc_info_t dpci = {0};
		resched = wlc_dpc(wl->wlc, bounded, &dpci);
	}

	/* wlc_dpc() may bring the driver down */
	if (!wl->wlc_hw->up)
		return;

	/* Driver is not down. Flush accumulated txrxstatus here */
	wl_busioctl(wl, BUS_FLUSH_CHAINED_PKTS, NULL,
		0, NULL, NULL, FALSE);

	/* re-schedule dpc or re-enable interrupts */
	if (resched) {
		if (!hnd_timer_start(wl->dpcTimer, 0, FALSE))
			ASSERT(FALSE);
		wl->dpctimer_armed = TRUE;
	} else
		wlc_intrson(wl->wlc);
} /* wl_dpc */

static void
_wl_dpc(hnd_timer_t *timer)
{
	wl_info_t *wl = (wl_info_t *) hnd_timer_get_data(timer);

	wl->dpctimer_armed = FALSE;
	if (wl->wlc_hw->up) {
		wlc_intrsupd(wl->wlc);
		wl_dpc(wl);
	}
}

static int
wl_open(hnd_dev_t *dev)
{
	wl_info_t *wl = dev->softc;
	int ret;

	WL_ERROR(("wl%d: wl_open\n", wl->unit));

	ret = wlc_ioctl(wl->wlc, WLC_UP, NULL, 0, NULL);
#ifdef WLRSDB
	if (RSDB_ENAB(wl->pub) &&
		(wl->pub->unit == 1) &&
		(ret == BCME_RADIOOFF)) {
		return BCME_OK;
	} else
#endif
	if (ret) {
		return ret;
	}

#ifdef HND_JOIN_SSID
	/*
	 * Feature useful for repetitious testing: if Make defines HND_JOIN_SSID
	 * to an SSID string, automatically join that SSID at driver startup.
	 */
	{
		wlc_info_t *wlc = wl->wlc;
		int infra = 1;
		int auth = 0;
		char *ss = HND_JOIN_SSID;
		wlc_ssid_t ssid;

		printf("Joining %s:\n", ss);
		/* set infrastructure mode */
		printf("  Set Infra\n");
		wlc_ioctl(wlc, WLC_SET_INFRA, &infra, sizeof(int), NULL);
		printf("  Set Auth\n");
		wlc_ioctl(wlc, WLC_SET_AUTH, &auth, sizeof(int), NULL);
		printf("  Set SSID %s\n", ss);
		ssid.SSID_len = strlen(ss);
		bcopy(ss, ssid.SSID, ssid.SSID_len);
		wlc_ioctl(wlc, WLC_SET_SSID, &ssid, sizeof(wlc_ssid_t), NULL);
	}
#endif /* HND_JOIN_SSID */

	return (ret);
}

#ifdef TKO
void * wl_get_tko(wl_info_t *wl, struct wl_if *wlif)
{
		return wl->tko;
}
#endif /* TKO */
#ifdef ICMP
void * wl_get_icmp(wl_info_t *wl, struct wl_if *wlif)
{
	return wl->icmp;
}
#endif /* ICMP */

#ifdef TOE
/** Process toe frames in receive direction */
static void
_wl_toe_recv_proc(wl_info_t *wl, void *p)
{
	if (TOE_ENAB(wl->pub))
		(void)wl_toe_recv_proc(wl->toei, p);
}
#endif /* TOE */

static bool
wl_hwfilter(wl_info_t *wl, void *p)
{
	struct ether_header *eh = (struct ether_header *)PKTDATA(wl->pub->osh, p);

	if (((wl->hwfflags & HWFFLAG_UCAST) && !ETHER_ISMULTI(eh->ether_dhost)) ||
	    ((wl->hwfflags & HWFFLAG_BCAST) && ETHER_ISBCAST(eh->ether_dhost)))
		return TRUE;

	return FALSE;
}

static void *
wl_pkt_header_push(wl_info_t *wl, void *p, uint8 *wl_hdr_words)
{
	wl_header_t *h;
	osl_t *osh = wl->pub->osh;
	wlc_pkttag_t *pkttag = WLPKTTAG(p);
	int8 rssi = pkttag->pktinfo.misc.rssi;

	if (PKTHEADROOM(osh, p) < WL_HEADER_LEN) {
		void *p1;
		int plen = PKTLEN(osh, p);

		/* Alloc a packet that will fit all the data; chaining the header won't work */
		if ((p1 = PKTGET(osh, plen + WL_HEADER_LEN, TRUE)) == NULL) {
			WL_ERROR(("PKTGET pkt size %d failed\n", plen));
			PKTFREE(osh, p, TRUE);
			return NULL;
		}

		/* Transfer other fields */
		PKTSETPRIO(p1, PKTPRIO(p));
		PKTSETSUMGOOD(p1, PKTSUMGOOD(p));

		bcopy(PKTDATA(osh, p), PKTDATA(osh, p1) + WL_HEADER_LEN, plen);
		PKTFREE(osh, p, TRUE);

		p = p1;
	} else
		PKTPUSH(osh, p, WL_HEADER_LEN);

	h = (wl_header_t *)PKTDATA(osh, p);
	h->type = WL_HEADER_TYPE;
	h->version = WL_HEADER_VER;
	h->rssi = rssi;
	h->pad = 0;
	/* Return header length in words */
	*wl_hdr_words = WL_HEADER_LEN/4;
	return p;
}

static void
wl_pkt_header_pull(wl_info_t *wl, void *p)
{
	/* Currently this is a placeholder function. We don't process wl header
	   on Tx side as no meaningful fields defined for tx currently.
	 */
	PKTPULL(wl->pub->osh, p, PKTDATAOFFSET(p));
	return;
}

/* Return the proper arpi pointer for either corr to an IF or
*	default. For IF case, Check if arpi is present. It is possible that, upon a
*	down->arpoe_en->up scenario, interfaces are not reallocated, and
*	so, wl->arpi could be NULL. If so, allocate it and use.
*/
static wl_arp_info_t *
wl_get_arpi(wl_info_t *wl, struct wl_if *wlif)
{
	ASSERT(ARPOE_SUPPORT(wl->pub));

	if (wlif != NULL) {
		if (wlif->arpi == NULL)
			wlif->arpi = wl_arp_alloc_ifarpi(wl->arpi, wlif->wlcif);
		/* note: this could be null if the above wl_arp_alloc_ifarpi fails */
		return wlif->arpi;
	} else
		return wl->arpi;
}

void *
wl_get_ifctx(wl_info_t *wl, int ctx_id, wl_if_t *wlif)
{
	if (ctx_id == IFCTX_ARPI)
		return (void *)wlif->arpi;

#ifdef WLNDOE
	if (ctx_id == IFCTX_NDI)
		return (void *)wlif->ndi;
#endif

	if (ctx_id == IFCTX_NETDEV)
		return (void *)((wlif == NULL) ? wl->dev : wlif->dev);

	return NULL;
}

#ifdef WLNDOE
/* Return the proper ndi pointer for either corr to an IF or
*	default. For IF case, Check if arpi is present. It is possible that, upon a
*	down->ndoe_en->up scenario, interfaces are not reallocated, and
*	so, wl->ndi could be NULL. If so, allocate it and use.
*/
static wl_nd_info_t *
wl_get_ndi(wl_info_t *wl, struct wl_if *wlif)
{
	ASSERT(NDOE_SUPPORT(wl->pub));
	if (wlif != NULL) {
		if (wlif->ndi == NULL)
			wlif->ndi = wl_nd_alloc_ifndi(wl->ndi, wlif->wlcif);
		/* note: this could be null if the above wl_arp_alloc_ifarpi fails */
		return wlif->ndi;
	} else
		return wl->ndi;
}
#endif /* WLNDOE */

#ifdef WLNFC
static wl_nfc_info_t *
wl_get_nfci(wl_info_t *wl, struct wl_if *wlif)
{
	if (wlif != NULL) {
		if (wlif->nfci == NULL) {
			wlif->nfci = wl_nfc_alloc_ifnfci(wl->nfci, wlif->wlcif);
		}
		return wlif->nfci;
	} else {
		return wl->nfci;
	}
}
#endif /* WLNFC */

void
wl_sendup_event(wl_info_t *wl, struct wl_if *wlif, void *p)
{
	wl_sendup(wl, wlif, p, 1);
}

/**
 * The last parameter was added for the build. Caller of this function should pass 1 for now.
 */
void
wl_sendup(wl_info_t *wl, struct wl_if *wlif, void *p, int numpkt)
{
	struct lbuf *lb;
	hnd_dev_t *dev;
	hnd_dev_t *chained;
	int ret_val;
	int no_filter;
	uint8 *buf;
	uint16 ether_type;
	bool brcm_specialpkt;

	WL_TRACE(("wl%d: wl_sendup: %d bytes\n", wl->unit, PKTLEN(NULL, p)));

	no_filter = 0;
	if (wlif == NULL)
		dev = wl->dev;
	else
		dev = wlif->dev;
	chained = dev->chained;

#ifdef	WL_FRWD_REORDER
	if (FRWD_REORDER_ENAB(((wlc_info_t *)wl->wlc)->pub) &&
		AMPDU_HOST_REORDER_ENAB(wl->pub)) {
		if ((p = wlc_ampdu_frwd_handle_host_reorder(((wlc_info_t *)(wl->wlc))->ampdu_rx,
			p, FALSE)) == NULL)
			return;
	}
#endif /* WLC_FRWD_PKT_REORDER */

	buf = PKTDATA(wl->pub->osh, p);
#ifdef PROP_TXSTATUS
	if (PROP_TXSTATUS_ENAB(((wlc_info_t *)(wl->wlc))->pub))
		brcm_specialpkt = !!PKTTYPEEVENT(wl->pub->osh, p);
	else
#endif
	brcm_specialpkt = ntoh16_ua(buf + ETHER_TYPE_OFFSET) == ETHER_TYPE_BRCM;

	if (!brcm_specialpkt) {
#ifdef TOE
		/* check TOE */
		_wl_toe_recv_proc(wl, p);
#endif /* TOE */

#ifdef EXT_STA
		if (WLEXTSTA_ENAB(wl->pub)) {
			if (((ltoh16(((struct dot11_header *)buf)->fc))
				& FC_KIND_MASK) == FC_QOS_DATA) {
				ether_type =
					ntoh16_ua(buf + DOT11_MAC_HDR_LEN +
						DOT11_QOS_LEN + SNAP_HDR_LEN);
			} else {
				ether_type = ntoh16_ua(buf + DOT11_MAC_HDR_LEN + SNAP_HDR_LEN);
			}
		}
		else
#endif /* EXT_STA */
		ether_type = ntoh16_ua(buf + ETHER_TYPE_OFFSET);

		if (ether_type == ETHER_TYPE_8021Q)
			ether_type = ntoh16_ua((const void *)(buf + VLAN_TAG_LEN));


		/* Apply ARP offload only for ETHER_TYPE_ARP */
		if (ARPOE_ENAB(wl->pub) &&
			(ether_type < ETHER_TYPE_MIN || ether_type == ETHER_TYPE_ARP)) {
			wl_arp_info_t *arpi = wl_get_arpi(wl, wlif);
			struct wlc_if *wlcif = (wlif != NULL) ? wlif->wlcif : NULL;
			wlc_bsscfg_t *bsscfg = wlc_bsscfg_find_by_wlcif(wl->wlc, wlcif);

			if (arpi) {
				ret_val = wl_arp_recv_proc(arpi, p);
				switch (ret_val) {
					case ARP_REQ_SINK:
						if (bsscfg && BSSCFG_AP(bsscfg) &&
							bsscfg->ap_isolate)
						/* for pcie, don't sink, pass it to host */
						break;
						/* fall through is intentional */
					case ARP_REPLY_PEER:
						PKTFREE(wl->pub->osh, p, FALSE);
						return;
					case ARP_FORCE_FORWARD:
						no_filter = 1;
						break;
				}
			}
		}
#ifdef WLNDOE
		/* Apply NS offload only for ETHER_TYPE_IPV6 */
		if (NDOE_ENAB(wl->pub) &&
			(ether_type < ETHER_TYPE_MIN || ether_type == ETHER_TYPE_IPV6)) {
			wl_nd_info_t *ndi = wl_get_ndi(wl, wlif);
			if (ndi) {
				ret_val = wl_nd_recv_proc(ndi, p);
				if ((ret_val == ND_REQ_SINK) || (ret_val == ND_REPLY_PEER)) {
					PKTFREE(wl->pub->osh, p, FALSE);
					return;
				}
				if (ret_val == ND_FORCE_FORWARD) {
					no_filter = 1;
				}
			}
		}
#endif /* WLNDOE */

#ifdef WLNFC
		/* Apply Secure WiFi thru NFC */
		if (NFC_ENAB(wl->pub)) {
			wl_nfc_info_t *nfci = wl_get_nfci(wl, wlif);
			if (nfci) {
				wl_nfc_recv_proc(nfci, p);
			}
		}
#endif

#ifdef WL_TBOW
		if (TBOW_ENAB(wl->pub)) {
			if (!tbow_recv_wlandata(((wlc_info_t *)(wl->wlc))->tbow_info, p)) {
				/* tbow packet, don't send up */
				return;
			}
		}
#endif

#ifdef BDO
		if (BDO_SUPPORT(wl->pub) && BDO_ENAB(wl->pub)) {
			if (wl_bdo_rx(wl->bdo, buf, PKTLEN(wl->pub->osh, p))) {
				PKTFREE(wl->pub->osh, p, FALSE);
				return;
			}
		}
#endif	/* BDO */

#ifdef TKO
		if (TKO_ENAB(wl->pub)) {
			struct wlc_if *wlcif = (wlif != NULL) ? wlif->wlcif : NULL;
			wlc_bsscfg_t *bsscfg = wlc_bsscfg_find_by_wlcif(wl->wlc, wlcif);
			if (wl_tko_rx(wl->tko, p, bsscfg)) {
				PKTFREE(wl->pub->osh, p, FALSE);
				return;
			}
		}
#endif	/* TKO */

#ifdef ICMP
		if (ICMP_ENAB(wl->pub)) {
			if (wl_icmp_rx(wl->icmp, p)) {
				PKTFREE(wl->pub->osh, p, FALSE);
				return;
			}
		}
#endif	/* ICMP */

#ifdef WL_NATOE
		if (NATOE_ACTIVE(wl->pub)) {
			if (wl_natoe_process_pkt(wl, wlif, p)) {
				return;
			}
		}
#endif	/* WL_NATOE */

	}

	if (chained) {

		/* Internally generated events have the special ether-type of
		 * ETHER_TYPE_BRCM; do not run these events through data packet filters.
		 */
		if (!brcm_specialpkt) {
			/* Apply packet filter */
			if ((chained->flags & RTEDEVFLAG_HOSTASLEEP) &&
			    wl->hwfflags && !wl_hwfilter(wl, p)) {
				PKTFREE(wl->pub->osh, p, FALSE);
				return;
			}

			/* Apply packet filtering. */
			if (!no_filter && PKT_FILTER_ENAB(wl->pub)) {
				if (!wlc_pkt_filter_recv_proc(wl->pkt_filter_info, p)) {
					/* Discard received packet. */
					PKTFREE(wl->pub->osh, p, FALSE);
					return;
				}
			}
#if defined(D0_COALESCING)
			/* Apply D0 packet filtering. */
			if (D0_FILTER_ENAB(wl->pub)) {
				if (!wlc_d0_filter_recv_proc(wl->d0_filter_info, p)) {
					return;
				}
			}
#endif /* D0_COALESCING */

		}
#ifdef EXT_STA
		if (WLEXTSTA_ENAB(wl->pub) && !brcm_specialpkt) {
			wl_rx_ctxt_push(wl, p);
		}
#endif /* EXT_STA */

#ifdef PROP_TXSTATUS
		if (PROP_TXSTATUS_ENAB(wl->pub)) {
#ifdef BCMPCIEDEV
			if (!BCMPCIEDEV_ENAB() || !PKTTYPEEVENT(wl->pub->osh, p))
#endif /* BCMPCIEDEV */
			{
				if (RXMETADATA_TO_HOST_ENAB(wl->pub) &&
					wlc_sendup_txstatus(wl->wlc, &p)) {
					return;
				}
			}
		} else
#endif /* PROP_TXSTATUS */
		{
#ifdef BCMPCIEDEV
			if (!BCMPCIEDEV_ENAB())
#endif /* BCMPCIEDEV */
			{
				uint8 wl_hdr_words = 0;
				if ((p = wl_pkt_header_push(wl, p, &wl_hdr_words)) == NULL) {
					return;
				}

				PKTSETDATAOFFSET(p, wl_hdr_words);
			}
		}
		lb = PKTTONATIVE(wl->pub->osh, p);
		if (chained->ops->xmit(dev, chained, lb) != 0) {
			WL_ERROR(("%s: xmit failed; free pkt 0x%p\n", __FUNCTION__, lb));
			lb_free(lb);
		}
	} else {
		/* only AP mode can be non chained */
		ASSERT(AP_ENAB(wl->pub));
		PKTFREE(wl->pub->osh, p, FALSE);
	}
} /* wl_sendup */

#if defined(D0_COALESCING)
void
wl_sendup_no_filter(wl_info_t *wl, struct wl_if *wlif, void *p, int numpkt)
{
	struct lbuf *lb;
	hnd_dev_t *dev;
	hnd_dev_t *chained;

	WL_TRACE(("wl%d: wl_sendup: %d bytes\n", wl->unit, PKTLEN(NULL, p)));

	if (wlif == NULL)
		dev = wl->dev;
	else
		dev = wlif->dev;
	chained = dev->chained;

	if (chained) {
#ifdef EXT_STA
		if (WLEXTSTA_ENAB(wl->pub)) {
			wl_rx_ctxt_push(wl, p);
		}
#endif /* EXT_STA */

#ifdef PROP_TXSTATUS
		if (PROP_TXSTATUS_ENAB(((wlc_info_t *)(wl->wlc))->pub) &&
		    (RXMETADATA_TO_HOST_ENAB(wl->pub))) {
			if (wlc_sendup_txstatus(wl->wlc, &p)) {
				return;
			}
		}
#endif /* PROP_TXSTATUS */

		lb = PKTTONATIVE(wl->pub->osh, p);
		if (chained->ops->xmit(dev, chained, lb) != 0) {
			WL_ERROR(("wl_sendup: xmit failed; free pkt 0x%p\n", lb));
			lb_free(lb);
		}
	} else {
		/* only AP mode can be non chained */
		ASSERT(AP_ENAB(wl->pub));
		PKTFREE(wl->pub->osh, p, FALSE);
	}
}
#endif 

/* buffer received from BUS driver(e.g USB, SDIO) in dongle framework
 *   For normal driver, push it to common driver sendpkt
 *   For BMAC driver, forward to RPC layer to process
 */
#ifdef TOE
/**
 * Process toe frames in transmit direction
 * most of the cases TOE is not in ROM, so to avoid big invalidation hooks to check TOE
 */
static void
_wl_toe_send_proc(wl_info_t *wl, void *p)
{
	if (TOE_ENAB(wl->pub))
		wl_toe_send_proc(wl->toei, p);
}
#endif /* TOE */

#ifdef PKTC_TX_DONGLE
/** PKTC_TX_DONGLE specific function */
static bool
wlconfig_tx_chainable(wl_info_t *wl, wlc_bsscfg_t *bsscfg)
{
	wlc_info_t *wlc = (wlc_info_t *)wl->wlc;

	/* For now don't do chaining for following configs. */
	if (PKTC_ENAB(wl->pub)) {
		if ((WLEXTSTA_ENAB(wl->pub) && BSSCFG_SAFEMODE(bsscfg)) ||
		    (wlc->wet && BSSCFG_STA(bsscfg)) ||
		    wlc->mac_spoof ||
#ifdef WLWNM_AP
		    (BSSCFG_AP(bsscfg) && WLWNM_ENAB(wlc->pub) &&
			!wlc_wnm_pkt_chainable(wlc->wnm_info, bsscfg)) ||
#endif /* WLWNM_AP */
		    CAC_ENAB(wl->pub)) {
			return FALSE;
		}
	}
	return TRUE;
}

static bool
wl_txframe_chainable(wl_info_t *wl, wlc_bsscfg_t *bsscfg, void *p, void *head)
{
	wlc_info_t *wlc = (wlc_info_t *)wl->wlc;
	bool chainable = FALSE;
	struct ether_header *eh, *head_eh;
	void *iph;

	eh = (struct ether_header *) PKTDATA(wlc->osh, p);
	iph = (void *)(eh + 1);

	if (BCMLFRAG_ENAB() && PKTISTXFRAG(wlc->osh, p)) {
		/* For LFRAG packets, we have only the ethernet header. IP header + Payload */
		/* is sitting in the host. So, don't bother to look into the IP Prot field */
		if ((ntoh16(eh->ether_type) == ETHER_TYPE_IP) ||
			(ntoh16(eh->ether_type) == ETHER_TYPE_IPV6) ||
			(ntoh16(eh->ether_type) <= ETHER_TYPE_MIN))
			chainable = TRUE;
	} else if (ntoh16(eh->ether_type) == ETHER_TYPE_IP) {
		ASSERT(IP_VER(iph) == IP_VER_4);

		if (IPV4_PROT(iph) == IP_PROT_TCP ||
		    IPV4_PROT(iph) == IP_PROT_UDP) {
			chainable = TRUE;
		}
	} else if (ntoh16(eh->ether_type) == ETHER_TYPE_IPV6) {
		ASSERT(IP_VER(iph) == IP_VER_6);

		if (IPV6_PROT(iph) == IP_PROT_TCP ||
		    IPV6_PROT(iph) == IP_PROT_UDP) {
			chainable = TRUE;
		}
	}

	if (!chainable)
		goto exit;

	if (head == NULL) {
		chainable = !ETHER_ISNULLDEST(eh->ether_dhost) &&
			!ETHER_ISMULTI(eh->ether_dhost);
	} else if (!BCMPCIEDEV_ENAB()) {
		/* For PCIe Dev, all the packets in the chain would
		 * have the same DA and PRIO. Since we are already checking
		 * the head packet, we do not need this.
		 */
		head_eh = (struct ether_header *) PKTDATA(wlc->osh, head);
		chainable = !eacmp(eh->ether_dhost, head_eh->ether_dhost) &&
			(PKTPRIO(p) == PKTPRIO(head));
	}

exit:
	return chainable;
} /* wl_txframe_chainable */
#endif /* PKTC_TX_DONGLE */

#ifdef BCMPCIEDEV
/**
 * For e.g. 802.1x packets, it is necessary to transfer the full packet from host memory into CPU
 * RAM, so firmware can parse packet contents before transmission.
 */
static void
wl_tx_pktfetch(wl_info_t *wl, struct lbuf *lb, hnd_dev_t *src, hnd_dev_t *dev,
	wlc_bsscfg_t *bsscfg)
{
	struct pktfetch_info *pinfo = NULL;
	struct pktfetch_generic_ctx *pctx = NULL;
	int ctx_count = 6;	/* No. of ctx variables needed to be saved */
	/* intention is to store ID - bsscfg Instance ID - to track delete before fetch cb */
	uint32 bsscfg_id = bsscfg->ID;

	pinfo = MALLOCZ(wl->pub->osh, sizeof(struct pktfetch_info));
	if (!pinfo) {
		WL_ERROR(("%s: Out of mem: Unable to alloc pktfetch ctx!\n", __FUNCTION__));
		goto error;
	}

	pctx = MALLOCZ(wl->pub->osh, sizeof(struct pktfetch_generic_ctx) +
		ctx_count*sizeof(void*));
	if (!pctx) {
		WL_ERROR(("%s: Out of mem: Unable to alloc pktfetch ctx!\n", __FUNCTION__));
		goto error;
	}

	/* Fill up context */
	pctx->ctx_count = ctx_count;
	pctx->ctx[0] = (void *)wl;
	pctx->ctx[1] = (void *)src;
	pctx->ctx[2] = (void *)dev;
	pctx->ctx[3] = (void *)pinfo;
	pctx->ctx[4] = (void *)bsscfg;
	pctx->ctx[5] = (void *)bsscfg_id;

	/* Fill up pktfetch info */
	pinfo->host_offset = 0;

	/* Need headroom of atleast 224 for TXOFF/amsdu headroom
	 * Rounded to 256
	 */
	pinfo->headroom = PKTFETCH_DEFAULT_HEADROOM;
	pinfo->lfrag = (void*)lb;
	pinfo->cb = wl_send_cb;
	pinfo->ctx = (void *)pctx;
	pinfo->next = NULL;
	pinfo->osh = wl->pub->osh;
	if (hnd_pktfetch(pinfo) != BCME_OK) {
		WL_ERROR(("%s: pktfetch request rejected\n", __FUNCTION__));
		goto error;
	}

	return;

error:
	if (pinfo)
		MFREE(wl->pub->osh, pinfo, sizeof(struct pktfetch_info));

	if (pctx)
		MFREE(wl->pub->osh, pctx, sizeof(struct pktfetch_generic_ctx) + 4*sizeof(uint32));

	if (lb)
		PKTFREE(wl->pub->osh, lb, TRUE);
}

/** Packet fetch callback. BCMPCIEDEV specific */
static void
wl_send_cb(void *lbuf, void *orig_lfrag, void *ctx, bool cancelled)
{
	wl_info_t *wl;
	struct pktfetch_info *pinfo;
	hnd_dev_t *src, *dev;
	struct pktfetch_generic_ctx *pctx = (struct pktfetch_generic_ctx *)ctx;
	uint32 bsscfg_ID;
	wlc_bsscfg_t *bsscfg = NULL;

	/* Retrieve contexts */
	wl = (wl_info_t *)pctx->ctx[0];
	src = (hnd_dev_t *)pctx->ctx[1];
	dev = (hnd_dev_t *)pctx->ctx[2];
	pinfo = (struct pktfetch_info *)pctx->ctx[3];
	bsscfg = (wlc_bsscfg_t *)pctx->ctx[4];
	bsscfg_ID = (uint32)pctx->ctx[5];

	PKTSETNEXT(wl->pub->osh, orig_lfrag, lbuf);
	PKTSETFRAGTOTLEN(wl->pub->osh, orig_lfrag, 0);
	PKTSETFRAGLEN(wl->pub->osh, orig_lfrag, 1, 0);
	PKTSETFRAGTOTNUM(wl->pub->osh, orig_lfrag, 0);

	/* Free the original pktfetch_info and generic ctx  */
	MFREE(wl->pub->osh, pinfo, sizeof(struct pktfetch_info));
	MFREE(wl->pub->osh, pctx, sizeof(struct pktfetch_generic_ctx)
		+ pctx->ctx_count*sizeof(void *));

	/* check for bsscfg sanity */
	if (bsscfg != wlc_bsscfg_find_by_ID(wl->wlc, bsscfg_ID)) {
		/* Drop the pkt, bsscfg not valid */
		WL_ERROR(("wl%d: Bsscfg %p Was freed during pktfetch, Drop the rqst \n",
			wl->unit, bsscfg));
		PKTFREE(wl->pub->osh, orig_lfrag, TRUE);
	} else {
		/* valid bsscfg, send to wl */
		wl_send(src, dev, orig_lfrag);
	}
}

/**
 * Per packet key check for SW TKIP MIC requirement, code largely borrowed from wlc_sendpkt.
 * BCMPCIEDEV specific.
 */
static bool
wl_sw_tkip_mic_enab(wl_info_t *wl, struct wlc_if *wlcif, wlc_bsscfg_t *bsscfg, struct lbuf *lb)
{
	struct scb *scb = NULL;
	struct ether_header *eh;
	struct ether_addr *dst;
#ifdef WDS
	struct ether_addr *wds = NULL;
#endif
	wlc_key_info_t key_info;
	wlc_key_t *key = NULL;
	uint bandunit;
	bool tkip_enab = FALSE;
	wlc_info_t *wlc = (wlc_info_t *)wl->wlc;

	/* WLDPT, WLTDLS, IAPP, WLAWDL cases currently not handled */

	/* Get dest. */
	eh = (struct ether_header*) PKTDATA(wl->pub->osh, lb);

#ifdef WDS
	if (wlcif && wlcif->type == WLC_IFTYPE_WDS) {
		scb = wlcif->u.scb;
		wds = &scb->ea;
	}

	if (wds)
		dst = wds;
	else
#endif /* WDS */
	if (BSSCFG_AP(bsscfg)) {
#ifdef WLWNM_AP
		/* Do the WNM processing */
		if (WLWNM_ENAB(wlc->pub) && wlc_wnm_dms_amsdu_on(wlc, bsscfg) &&
		    WLPKTTAGSCBGET(lb) != NULL) {
			dst = &(WLPKTTAGSCBGET(lb)->ea);
		} else
#endif /* WLWNM_AP */
		dst = (struct ether_addr*)eh->ether_dhost;
	} else {
		dst = bsscfg->BSS ? &bsscfg->BSSID : (struct ether_addr*)eh->ether_dhost;
	}

	/* Get key */
	bandunit = CHSPEC_WLCBANDUNIT(bsscfg->current_bss->chanspec);

	/* Class 3 (BSS) frame */
	if (TRUE &&
#ifdef WDS
		!wds &&
#endif
		bsscfg->BSS && !ETHER_ISMULTI(dst)) {
		scb = wlc_scbfindband(wlc, bsscfg, dst, bandunit);
	}
	/* Class 1 (IBSS/DPT) or 4 (WDS) frame */
	else {
		if (!ETHER_ISMULTI(dst))
			scb = wlc_scblookupband(wlc, bsscfg, dst, bandunit);
	}

	key = wlc_keymgmt_get_tx_key(wlc->keymgmt, scb, bsscfg, &key_info);

	if (scb && (key_info.algo == CRYPTO_ALGO_OFF)) {
		WL_INFORM(("wl%d: %s: key_info algo is off, use bss tx key instead\n",
			wl->unit, __FUNCTION__));
		key = wlc_keymgmt_get_bss_tx_key(wlc->keymgmt, bsscfg, FALSE, &key_info);
	}

	if (key == NULL)
		WL_ERROR(("wl%d: %s: key is NULL!\n", wl->unit, __FUNCTION__));

	/* If security algo is TKIP and MIC key is in HW, or PMF */
	if (key_info.algo == CRYPTO_ALGO_TKIP && !WLC_KEY_MIC_IN_HW(&key_info))
		tkip_enab = TRUE;

	return tkip_enab;
} /* wl_sw_tkip_mic_enab */

/** BCMPCIEDEV specific */
static bool
wl_tx_pktfetch_required(wl_info_t *wl, wl_if_t *wlif, wlc_bsscfg_t *bsscfg, struct lbuf *lb,
	wl_arp_info_t * arpi)
{
	struct wlc_if *wlcif = wlif != NULL ? wlif->wlcif : NULL;

	if (WSEC_ENABLED(bsscfg->wsec) && (WLPKTFLAG_PMF(WLPKTTAG(lb)) ||
	   (WSEC_TKIP_ENABLED(bsscfg->wsec) && wl_sw_tkip_mic_enab(wl, wlcif, bsscfg, lb))))

		return TRUE;

	if (ARPOE_ENAB(wl->pub)) {
		if (arpi) {
			if (wl_arp_send_pktfetch_required(arpi, lb))
				return TRUE;
		}
	}

	if (ntoh16_ua((const void *)(PKTDATA(wl->pub->osh, lb) + ETHER_TYPE_OFFSET))
		== ETHER_TYPE_802_1X) {
		return TRUE;
	}

	return FALSE;
}

/**
 * For e.g. ftp packets in NATOE, it is necessary to transfer the full rx packet
 * from host memory into CPU RAM, so firmware can parse packet contents before
 * forwarding for transmission.
 */
int
wl_rx_pktfetch(wl_info_t *wl, wl_if_t *wlif, struct lbuf *lb)
{
	struct pktfetch_info *pinfo = NULL;
	struct pktfetch_generic_ctx *pctx = NULL;
	int ctx_count = 5;	/* No. of ctx variables needed to be saved */
	/* intention is to store ID - bsscfg Instance ID - to track delete before fetch cb */

	wlc_bsscfg_t *bsscfg = wlc_bsscfg_find_by_wlcif(wl->wlc, wlif->wlcif);
	uint32 bsscfg_id = bsscfg->ID;

	osl_t * osh = wl->pub->osh;
	int ret = BCME_OK;

	pinfo = MALLOCZ(osh, sizeof(struct pktfetch_info));
	if (!pinfo) {
		WL_ERROR(("%s: Out of mem: Unable to alloc pktfetch ctx!\n", __FUNCTION__));
		ret = BCME_NOMEM;
		goto error;
	}

	pctx = MALLOCZ(osh, sizeof(struct pktfetch_generic_ctx) +
		ctx_count*sizeof(void*));
	if (!pctx) {
		WL_ERROR(("%s: Out of mem: Unable to alloc pktfetch ctx!\n", __FUNCTION__));
		ret = BCME_NOMEM;
		goto error;
	}

	/* Fill up context */
	pctx->ctx_count = ctx_count;
	pctx->ctx[0] = (void *)wl;
	pctx->ctx[1] = (void *)wlif;
	pctx->ctx[2] = (void *)pinfo;
	pctx->ctx[3] = (void *)bsscfg;
	pctx->ctx[4] = (void *)bsscfg_id;

	/* Fill up pktfetch info */
	/* Headroom does not need to be > PKTRXFRAGSZ */
	pinfo->osh = osh;
	pinfo->headroom = PKTRXFRAGSZ;
	pinfo->lfrag = (void*)lb;
	pinfo->ctx = (void *)pctx;
	pinfo->cb = wl_rx_pktfetch_cb;

	pinfo->host_offset = 0;

	ret = hnd_pktfetch(pinfo);
	if (ret != BCME_OK) {
		WL_ERROR(("%s: pktfetch request rejected\n", __FUNCTION__));
		goto error;
	}

	return ret;

error:
	if (pinfo) {
		MFREE(osh, pinfo, sizeof(struct pktfetch_info));
	}

	if (pctx) {
		MFREE(osh, pctx, sizeof(struct pktfetch_generic_ctx)
				+ pctx->ctx_count*sizeof(void *));
	}

	if (lb) {
		PKTFREE(osh, lb, FALSE);
	}
	return ret;
}

static void
wl_rx_pktfetch_cb(void *lbuf, void *orig_lfrag, void *ctx, bool cancelled)
{
	wl_info_t *wl;
	wl_if_t *wlif;
	osl_t *osh;
	uint32 bsscfg_ID;
	wlc_bsscfg_t *bsscfg = NULL;
	struct pktfetch_info *pinfo = NULL;

	struct pktfetch_generic_ctx *pctx = (struct pktfetch_generic_ctx *)ctx;

	/* Retrieve contexts */
	wl = (wl_info_t *)pctx->ctx[0];
	wlif = (wl_if_t *)pctx->ctx[1];
	pinfo = (struct pktfetch_info *)pctx->ctx[2];
	bsscfg = (wlc_bsscfg_t *)pctx->ctx[3];
	bsscfg_ID = (uint32)pctx->ctx[4];

	osh = pinfo->osh;

	if (bsscfg != wlc_bsscfg_find_by_ID(wl->wlc, bsscfg_ID)) {
		/* Drop the pkt, bsscfg not valid */
		WL_ERROR(("wl%d: Bsscfg %p Was freed during pktfetch, Drop the rqst \n",
			wl->unit, bsscfg));
		PKTFREE(osh, lbuf, FALSE);
		goto done;
	}

	/* Subsequent subframe fetches */
	PKTPUSH(osh, lbuf, PKTLEN(osh, orig_lfrag));

	/* Copy the original lfrag data  */
	memcpy(PKTDATA(osh, lbuf), PKTDATA(osh, orig_lfrag), PKTLEN(osh, orig_lfrag));

	/* Copy PKTTAG */
	memcpy(WLPKTTAG(lbuf), WLPKTTAG(orig_lfrag), sizeof(wlc_pkttag_t));

	PKTSETNEXT(osh, lbuf, NULL);

	wl_sendup(wl, wlif, lbuf, 1);

done:
	PKTFREE(osh, orig_lfrag, FALSE);

	MFREE(osh, pinfo, sizeof(struct pktfetch_info));
	MFREE(osh, pctx, sizeof(struct pktfetch_generic_ctx)
			+ pctx->ctx_count*sizeof(void *));
}

#if defined(BCMFRAGPOOL)
void *BCMFASTPATH
wl_create_fwdpkt(wl_info_t *wl, void *p, wl_if_t *wlif)
{
	osl_t *osh;
	void *p1;

	osh = wl->pub->osh;
	/* PKTLEN of receive lfrag 'p' should not be less than ETHER_HDR_LEN
	 * and if it is exactly equal to ETHER_HDR_LEN, then atleast PKTFRAGUSEDLEN
	 * should be non-zero for hnddma code to program dma properly.
	 */

	/* Get an lfrag from tx lfrag pool */
	if ((p1 = pktpool_get(SHARED_FRAG_POOL)) == NULL) {
		WL_INFORM(("wl%d: %s: Unable get lfrag \n", wl->unit, __FUNCTION__));
		PKTFREE(osh, p, FALSE);
		return NULL;
	}

	PKTPULL(osh, p1, TXOFF + ETHER_HDR_LEN);
	PKTSETLEN(osh, p1, 0);
	PKTPUSH(osh, p1, ETHER_HDR_LEN);

	/* Setup first pkt */
	bcopy((uint8*)PKTDATA(osh, p),
		PKTDATA(osh, p1), ETHER_HDR_LEN);
	PKTPULL(osh, p, ETHER_HDR_LEN);
	PKTSETFRAGDATA_HI(osh, p1, 1, 0);
	PKTSETFRAGDATA_LO(osh, p1, 1, 0);
	PKTSETFRAGTOTNUM(osh, p1, 0);
	PKTSETFRAGLEN(osh, p1, 1, ETHER_HDR_LEN);
	PKTSETFRAGTOTLEN(osh, p1, 0);
	PKTSETIFINDEX(osh, p1, PKTIFINDEX(osh, p));
	/* Reset meta data so that this frag wont take txstatus path */
	PKTRESETHASMETADATA(osh, (struct lbuf *)p1);
	PKTSETFRAGPKTID(osh, p1, 0xdeadbeaf);

	/* priority is set in rxfrag before this function is called.
	* Here it is shifted to tx frag
	*/
	PKTSETPRIO(p1, PKTPRIO(p));

	if (PKTFRAGUSEDLEN(osh, p)) {
		/* Prepare the rxfrag to be compatible to hnddma txfrag transmit.
		 * 1) set FRAGTOTNUM to 1 indicating host buffer address is valid.
		 * 2) Backup original host buffer length PKTFRAGLEN[1] in PKTFRAGLEN[2].
		 * 3) update number of bytes to transmit from host buffer in
		 *    PKTFRAGLEN[1] to be used by hnddma.
		 * 4) update total number of bytes to transmit from host buffers in FRAGTOTLEN.
		 */
		PKTSETFRAGTOTNUM(osh, p, 1);
		PKTSETFRAGLEN(osh, p, 2, PKTFRAGLEN(osh, p, 1));
		PKTSETFRAGLEN(osh, p, 1, PKTFRAGUSEDLEN(osh, p));
		PKTSETFRAGTOTLEN(osh, p, PKTFRAGUSEDLEN(osh, p));
	}

	PKTSETNEXT(osh, p1, p);
	p = p1;

	return p;
}
#endif /* BCMFRAGPOOL */
#endif	/* BCMPCIEDEV */

int BCMFASTPATH
wl_send_fwdpkt(wl_info_t *wl, void *p, wl_if_t *wlif)
{
	osl_t *osh;
	ASSERT(PKTCLINK(p) == NULL);

	osh = wl->pub->osh;
	/* Clear pkttag information saved in recv path */
	WLPKTTAGCLEAR(p);

	BCM_REFERENCE(osh);
	/* Before forwarding, fix the priority.
	 * For priority set in rxfrag will be shifted to
	 * tx frag in wl_create_fwdpkt()
	 */
	if (QOS_ENAB(wl->pub) && (PKTPRIO(p) == 0))  {
		pktsetprio(p, FALSE);
	}

#if defined(BCMPCIEDEV) && defined(BCMFRAGPOOL)
	if (BCMPCIEDEV_ENAB() && (PKTISRXFRAG(osh, p))) {
		if ((p = wl_create_fwdpkt(wl, p, wlif)) == NULL) {
			return BCME_NOMEM;
		}
	}
#endif /* BCMPCIEDEV && BCMFRAGPOOL */

#ifdef WL_NATOE
	/* Marking each forwarded pkt, so that it can be dropped in following cases.
	 * 1) while going to D3 mode, forwarded pkt using tx / rx frag packet should be
	 *    dropped.
	 * 2) if pkt is destined to a connected soft client on AP interface,
	 *    which is going into powersave.
	 */
	if (NATOE_ACTIVE(wl->pub)) {
		PKTSETFRWDPKT(osh, p);
#if (defined(PKTC) || defined PKTC_DONGLE)
		if ((wl_natoe_chain_pkts(wl, p, wlif)) == NATOE_PKT_CHAINED) {
			return BCME_OK;
		}
#endif /* (PKTC || PKTC_DONGLE) */
	}
#endif /* WL_NATOE */

	if (wlc_sendpkt(wl->wlc, p, wlif->wlcif)) {
		return BCME_ERROR;
	} else {
		return BCME_OK;
	}
}

/**
 * Called by the RTOS when one or more transmit packets are to be handed over to
 * the dot11 stack.
 * @param [in] src
 * @param [in] dev
 * @param [in] lb   linked list of one or more packets to transmit.
 * Return value: 0 on success, 1 if packet(s) were discarded
 */
static int
wl_send(hnd_dev_t *src, hnd_dev_t *dev, struct lbuf *lb)
{
	wl_info_t *wl = dev->softc;
	wl_if_t *wlif = WL_IF(dev);
	struct wlc_if *wlcif;
	void *p;
	struct lbuf *next;
	bool discarded = FALSE;
#ifdef BCMPCIEDEV
	bool pktfetch = FALSE;
#endif /* BCMPCIEDEV */
#ifdef PKTC_TX_DONGLE
	bool chainable = FALSE;
	void *head = NULL, *tail = NULL;
	bool chain_enable;
#endif
	wlc_info_t *wlc = (wlc_info_t *)wl->wlc;
	wl_arp_info_t *arpi = NULL;
#if defined(BCMPCIEDEV) || defined(PKTC_TX_DONGLE)
	wlc_bsscfg_t *bsscfg;
#endif /* BCMPCIEDEV || PKTC_TX_DONGLE */

	ASSERT(wlif != NULL);
	wlcif = wlif->wlcif;
	ASSERT(wlcif != NULL);

#if defined(BCMPCIEDEV) || defined(PKTC_TX_DONGLE)
	bsscfg = wlc_bsscfg_find_by_wlcif(wlc, wlcif);
	ASSERT(bsscfg != NULL);
#endif
	BCM_REFERENCE(wlc);

	WLPKTFLAG_EXEMPT_SET(WLPKTTAG(lb), PKTGETEXEMPT(lb));

#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB() && PKTISTXFRAG(wl->pub->osh, lb) &&
	    (PKTFRAGTOTLEN(wl->pub->osh, lb) > 0))
		pktfetch = TRUE;
#endif /* BCMPCIEDEV */

#ifdef PKTC_TX_DONGLE
	chain_enable = wlconfig_tx_chainable(wl, bsscfg);
#endif

	/* check if arp is enabled */
	if (ARPOE_ENAB(wl->pub)) {
		arpi = wl_get_arpi(wl, wlif);
		if (arpi) {
			if (wl_arp_components_enab() == FALSE) {
				WL_INFORM(("ARP offloads: no components enabled\n"));
				arpi = NULL;
			}
		}
	}

	while (lb != NULL) {
		next = PKTLINK(lb);
		PKTSETLINK(lb, NULL);

		p = PKTFRMNATIVE(wl->pub->osh, lb);
#ifdef PROP_TXSTATUS
		if (PROP_TXSTATUS_ENAB(wlc->pub)) {
			if (TRUE &&
#ifdef BCMPCIEDEV
			    (pktfetch || !BCMPCIEDEV_ENAB()) &&
#endif /* BCMPCIEDEV */
			    wlc_send_txstatus(wlc, p)) {
				goto nextlb;
			}
		} else
#endif /* PROP_TXSTATUS */
		{
			/* sets data pointer in packet to start of MSDU */
			wl_pkt_header_pull(wl, p);
		}

#ifdef BCMPCIEDEV
		/* For TKIP, MAC layer fragmentation does not work with current split-tx
		 * approach. We need to pull down the remaining payload and recreate the
		 * original 802.3 packet. Packet chaining, if any, is broken here anyway
		 * Need to check wl_sw_tkip_mic_enab call for all pkts in chain at this point
		 */
		if (pktfetch && wl_tx_pktfetch_required(wl, wlif, bsscfg, lb, arpi)) {
			wl_tx_pktfetch(wl, lb, src, dev, bsscfg); /* queues a host fetch request */
			goto nextlb;
		}
#endif /* BCMPCIEDEV */
		WL_TRACE(("wl%d: wl_send: len %d\n", wl->unit, PKTLEN(wl->pub->osh, p)));

		/* Apply ARP offload */
		if (ARPOE_ENAB(wl->pub)) {
			if (arpi) {
				if (wl_arp_send_proc(arpi, p) ==
					ARP_REPLY_HOST) {
					PKTFREE(wl->pub->osh, p, TRUE);
					goto nextlb;
				}
			}
		}
#ifdef WLNDOE
		/* Apply NS offload */
		if (NDOE_ENAB(wl->pub)) {
			wl_nd_info_t *ndi = wl_get_ndi(wl, wlif);
			if (ndi) {
				wl_nd_send_proc(ndi, p);
			}
		}
#endif

#ifdef WLNFC
		/* Apply NFC offload */
		if (NFC_ENAB(wl->pub)) {
			wl_nfc_info_t *nfci = wl_get_nfci(wl, wlif);
			wl_nfc_send_proc(nfci, p);
		}
#endif

#ifdef TOE
		/* check TOE */
		if (TOE_ENAB(wl->pub))
			_wl_toe_send_proc(wl, p);
#endif

#ifdef PKTC_TX_DONGLE
		if (PKTC_ENAB(wl->pub)) {
			// note: may chain packets meant for different destinations
			chainable = chain_enable && wl_txframe_chainable(wl, bsscfg, p, head);

			if (chainable) {
				PKTSETCHAINED(wl->pub->osh, p);
				PKTCENQTAIL(head, tail, p); // may change 'head' and/or 'tail'
			}

			if (head != NULL &&
			    (next == NULL || !chainable)) {
				// forward chain so far to WLC layer and restart chaining
				if (wlc_sendpkt(wlc, head, wlcif))
					discarded = TRUE;
				head = tail = NULL;
			}
		}

		if (!chainable)
#endif /* PKTC_TX_DONGLE */
		{
			if (wlc_sendpkt(wlc, p, wlcif))
				discarded = TRUE;
		}

nextlb:
		lb = next;
	}

#ifdef PKTC_TX_DONGLE
	/* if last pkt was header only, send remaining chain */
	if (PKTC_ENAB(wl->pub) && (head != NULL)) {
		if (wlc_sendpkt(wlc, head, wlcif))
			discarded = TRUE;
	}
#endif

	return discarded;
} /* wl_send */

int wl_busioctl(wl_info_t *wl, uint32 cmd, void *buf, int len, int *used, int *needed, int set)
{
	hnd_dev_t *chained = wl->dev->chained;
	if (chained && chained->ops->ioctl)
		return chained->ops->ioctl(chained, cmd, buf, len, used, needed, set);
	else
		return BCME_ERROR;
}

void
wl_txflowcontrol(wl_info_t *wl, struct wl_if *wlif, bool state, int prio)
{
	hnd_dev_t *chained = wl->dev->chained;

	/* sta mode must be chained */
	if (chained && chained->ops->txflowcontrol)
		chained->ops->txflowcontrol(chained, state, prio);
	else
		ASSERT(AP_ENAB(wl->pub));
}

void
wl_event(wl_info_t *wl, char *ifname, wlc_event_t *e)
{
#ifdef WLPFN
	/* Tunnel events into PFN for analysis */
	if (WLPFN_ENAB(wl->pub))
		wl_pfn_event(wl->pfn, e);
#endif /* WLPFN */

	switch (e->event.event_type) {
#ifdef EXT_STA
	case WLC_E_DISASSOC:
		if (wl->pub->wake_event_enable & WAKE_EVENT_AP_ASSOCIATION_LOST_BIT) {
			wl->pub->wake_event_status = WAKE_EVENT_AP_ASSOCIATION_LOST_BIT;
		}
		break;
	case WLC_E_PFN_NET_FOUND:
		if (wl->pub->wake_event_enable & WAKE_EVENT_NLO_DISCOVERY_BIT) {
			wl->pub->wake_event_status = WAKE_EVENT_NLO_DISCOVERY_BIT;
		}
		break;
	case WLC_E_EAPOL_MSG:
		if (wl->pub->wake_event_enable & WAKE_EVENT_4WAY_HANDSHAKE_REQUEST_BIT) {
			wl->pub->wake_event_status = WAKE_EVENT_4WAY_HANDSHAKE_REQUEST_BIT;
		}
		break;
	case WLC_E_PSK_SUP:
		if ((wl->pub->wake_event_enable & WAKE_EVENT_GTK_HANDSHAKE_ERROR_BIT) &&
			(e->event.reason == WLC_E_SUP_DECRYPT_KEY_DATA)) {
			/* trigger wake up interrupt */
			wl->pub->wake_event_status = WAKE_EVENT_GTK_HANDSHAKE_ERROR_BIT;
		}
		break;
#endif /* EXT_STA */
	case WLC_E_LINK:
		wl->link = e->event.flags&WLC_EVENT_MSG_LINK;
#ifdef WL_NATOE
		if (NATOE_ENAB(wl->pub)) {
			bool isup;
			int err = BCME_OK;

			isup = !!wl->link;
			wlc_bsscfg_t *bsscfg = wlc_bsscfg_find(wl->wlc, e->event.bsscfgidx, &err);
			if ((bsscfg != NULL) && (bsscfg->wlcif != NULL) &&
					(bsscfg->wlcif->wlif != NULL)) {
				wl_natoe_notify_wlif_link(wl, isup, bsscfg->wlcif->wlif);
			}
		}
#endif /* WL_NATOE */

		if (wl->link) {
			WL_ERROR(("wl%d: link up (%s)\n", wl->unit, ifname));
#ifdef WLNDOE_RA
			/* clear Router Advertisement Filter cache on every
			 * assoc event
			 */
			wl_nd_ra_filter_clear_cache(wl->ndi);
#endif /* WLNDOE_RA */
		}
/* Getting too many */
#ifndef EXT_STA
		else
			WL_ERROR(("wl%d: link down (%s)\n", wl->unit, ifname));
#endif /* EXT_STA */
		break;
#if ((defined(STA) && defined(BCMSUP_PSK)) || (defined(AP) && defined(BCMAUTH_PSK)))
	case WLC_E_MIC_ERROR:
		{
			int err = BCME_OK;
			wlc_bsscfg_t *cfg = wlc_bsscfg_find(wl->wlc, e->event.bsscfgidx, &err);
			if (cfg != NULL) {
#if defined(STA) && defined(BCMSUP_PSK)
				if (SUP_ENAB(wl->pub) && BSSCFG_STA(cfg))
					wlc_sup_mic_error(cfg,
						(e->event.flags&WLC_EVENT_MSG_GROUP) ==
						WLC_EVENT_MSG_GROUP);
#endif /* STA && BCMSUP_PSK */
#if defined(AP) && defined(BCMAUTH_PSK)
				if (BCMAUTH_PSK_ENAB(wl->pub) && BSSCFG_AP(cfg))
					wlc_auth_tkip_micerr_handle(wl->wlc, cfg);
#endif /* AP && BCMAUTH_PSK */
			}
		}
		break;
#endif /* (STA && BCMSUP_PSK) || (AP && BCMAUTH_PSK) */
	}
} /* wl_event */

void
wl_event_sync(wl_info_t *wl, char *ifname, wlc_event_t *e)
{
#ifdef WL_EVENTQ
	/* duplicate event for local event q */
	wl_eventq_dup_event(wl->wlevtq, e);
#endif /* WL_EVENTQ */
}

#ifdef WLOTA_EN
static int
wlc_iovar_wlota_filter(wlc_info_t * wlc, char * name, uint32 cmd)
{

	int bcmerror = BCME_UNSUPPORTED;
	char allowed[5][25] = {
			"ota_trigger",
			"ota_loadtest",
			"ota_teststatus",
			"ota_teststop",
			"bcmerrorstr"
			};
	uint8 i;
	if (wlc->iov_block == NULL)
		return BCME_OK;

	if (*wlc->iov_block != WL_OTA_TEST_ACTIVE)
		return BCME_OK;

	if (cmd <= 1)
		return BCME_OK;

	for (i = 0; i < 5; i++) {
		if (strcmp(name, allowed[i]) == 0) {
			bcmerror = BCME_OK;
			break;
		}
	}
	return bcmerror;
}
#endif /* WLOTA */

static int
wl_ioctl(hnd_dev_t *dev, uint32 cmd, void *buf, int len, int *used, int *needed, int set)
{
	wl_info_t *wl = dev->softc;
	wl_if_t *wlif = WL_IF(dev);
	struct wlc_if *wlcif;
	wlc_bsscfg_t *cfg = NULL;
	int ret = 0;
	int origcmd = cmd;
	int status = 0;
	uint32 *ret_int_ptr = (uint32 *)buf;

	WL_TRACE(("wl%d: wl_ioctl: cmd 0x%x\n", wl->unit, cmd));
	ASSERT(wlif != NULL);
	wlcif = wlif->wlcif;
	cfg = wlc_bsscfg_find_by_wlcif(wl->wlc, wlcif);
	ASSERT(cfg != NULL);

	switch (cmd) {
	case RTEGHWADDR:
		ret = wlc_iovar_op(wl->wlc, "cur_etheraddr", NULL, 0, buf, len, IOV_GET, wlcif);
		break;
	case RTESHWADDR:
		ret = wlc_iovar_op(wl->wlc, "cur_etheraddr", NULL, 0, buf, len, IOV_SET, wlcif);
		break;
	case RTEGPERMADDR:
		ret = wlc_iovar_op(wl->wlc, "perm_etheraddr", NULL, 0, buf, len, IOV_GET, wlcif);
		break;
	case RTEGMTU:
		*ret_int_ptr = ETHER_MAX_DATA;
		break;
	case RTEGSTATS:
		wl_statsupd(wl);
		bcopy(&wl->stats, buf, MIN(len, sizeof(wl->stats)));
		break;

	case RTEGALLMULTI:
		*ret_int_ptr = cfg->allmulti;
		break;
	case RTESALLMULTI:
		cfg->allmulti = *((uint32 *) buf);
		break;
	case RTEGPROMISC:
		cmd = WLC_GET_PROMISC;
		break;
	case RTESPROMISC:
		cmd = WLC_SET_PROMISC;
		break;
	case RTESMULTILIST: {
		int i;

		/* copy the list of multicasts into our private table */
		cfg->nmulticast = len / ETHER_ADDR_LEN;
		for (i = 0; i < cfg->nmulticast; i++)
			cfg->multicast[i] = ((struct ether_addr *)buf)[i];
		break;
	}
	case RTEGUP:
		cmd = WLC_GET_UP;
		break;
	case RTEDEVPWRSTCHG:
		if (*ret_int_ptr == 0)
			wl_devpwrstchg_notify(wl, FALSE);
		else
			wl_devpwrstchg_notify(wl, TRUE);
		break;
	case RTEDEVPMETOGGLE:
		wl_generate_pme_to_host(wl, *(uint32*)buf);
		break;
	case RTEDEVTIMESYNC:
#ifdef WLC_TSYNC
		wlc_tsync_process_host(((wlc_info_t *)wl->wlc)->tsync, buf, len);
#endif
		break;
	case RTEDEVDSNOTIFY:
#if defined(WLC_TSYNC)
		/* This is removed for now */
//		wlc_tsync_process_dsevt(((wlc_info_t *)wl->wlc)->tsync, buf, len);
#endif
		break;
	default:
		/* force call to wlc ioctl handler */
		origcmd = -1;
		break;
	}

#ifdef WLOTA_EN
	if ((ret = wlc_iovar_wlota_filter(wl->wlc, buf, cmd)) != BCME_OK) {
		return ret;
	}
#endif /* WLOTA */

	if (cmd != origcmd) {
		ret = wlc_ioctl(wl->wlc, cmd, buf, len, wlcif);
	}

	if (status)
		return status;

	return (ret);
} /* wl_ioctl */

static int
wl_close(hnd_dev_t *dev)
{
	wl_info_t *wl = dev->softc;
	BCM_REFERENCE(wl);
	uint8 objreg_freed = 0;

	WL_TRACE(("wl%d: wl_close\n", wl->unit));

#ifdef WL_OBJ_REGISTRY
	if (wl->objr && dev->devid) {
		objreg_freed = obj_registry_islast(wl->objr);
	}
#endif

	wl_down(wl);

	if (objreg_freed)
		MFREE(NULL, (dev->commondata), sizeof(wl_cmn_data_t));

	return 0;
}

static void
wl_statsupd(wl_info_t *wl)
{
	hnd_dev_stats_t *stats;

	WL_TRACE(("wl%d: wl_get_stats\n", wl->unit));

	stats = &wl->stats;

	/* refresh stats */
	if (wl->pub->up)
		wlc_statsupd(wl->wlc);

	stats->rx_packets = WLCNTVAL(wl->pub->_cnt->rxframe);
	stats->tx_packets = WLCNTVAL(wl->pub->_cnt->txframe);
	stats->rx_bytes = WLCNTVAL(wl->pub->_cnt->rxbyte);
	stats->tx_bytes = WLCNTVAL(wl->pub->_cnt->txbyte);
	stats->rx_errors = WLCNTVAL(wl->pub->_cnt->rxerror);
	stats->tx_errors = WLCNTVAL(wl->pub->_cnt->txerror);
	stats->rx_dropped = 0;
	stats->tx_dropped = 0;
	stats->multicast = WLCNTVAL(wl->pub->_cnt->rxmulti);
}

void
BCMATTACHFN(wl_isucodereclaimed)(uint8 *value)
{
#if defined(BCM_RECLAIM)
	*value = (uint8)bcm_preattach_part_reclaimed;
#endif /* BCM_RECLAIM */
}

void
BCMATTACHFN(wl_reclaim)(void)
{
#ifdef BCM_RECLAIM
	bool postattach_part_reclaimed_tmp = bcm_postattach_part_reclaimed;
#ifdef BCM_RECLAIM_INIT_FN_DATA
	bcm_reclaimed = TRUE;
#endif /* BCM_RECLAIM_INIT_FN_DATA */
	bcm_attach_part_reclaimed = TRUE;
	bcm_postattach_part_reclaimed = TRUE;
	hnd_reclaim();
	bcm_postattach_part_reclaimed = postattach_part_reclaimed_tmp;
#endif /* BCM_RECLAIM */
}

/* postattach is assumed to be called after wlc_bmac_init */
void
wl_reclaim_postattach(void)
{
#ifdef BCM_RECLAIM
#ifdef BCM_RECLAIM_INIT_FN_DATA
	bool bcm_reclaimed_tmp = bcm_reclaimed;
	bcm_reclaimed = TRUE;
#endif /* BCM_RECLAIM_INIT_FN_DATA */
	hnd_reclaim();
#ifdef BCM_RECLAIM_INIT_FN_DATA
	bcm_reclaimed = bcm_reclaimed_tmp;
#endif /* BCM_RECLAIM_INIT_FN_DATA */
#endif /* BCM_RECLAIM */
}


#ifdef BCMDBG
/* Mini command to control msglevel for BCMDBG builds */
static void
do_wlmsg_cmd(void *arg, int argc, char *argv[])
{
	switch (argc) {
	case 3:
		/* Set both msglevel and msglevel2 */
		wl_msg_level2 = strtoul(argv[2], 0, 0);
		/* fall through */
	case 2:
		/* Set msglevel */
		wl_msg_level = strtoul(argv[1], 0, 0);
		break;
	case 1:
		/* Display msglevel and msglevel2 */
		printf("msglvl1=0x%x msglvl2=0x%x\n", wl_msg_level, wl_msg_level2);
		break;
	}
}
#endif /* BCMDBG */

#ifdef WL_WOWL_MEDIA
void wl_wowl_dngldown(struct wl_info *wl)
{
	hnd_dev_t *chained = NULL;
	hnd_dev_t *dev = NULL;

	dev = wl->dev;
	if (dev)
	  chained = dev->chained;

	if (chained && chained->ops->wowldown) {
		chained->ops->wowldown(chained); // calls eg usbdev_wowldown() -> ch_wowldown()
	}
}
#endif

#ifdef BCMPCIEDEV
/** @param[in] op	e.g. FLOW_RING_CREATE, FLOW_RING_FLUSH, FLOW_RING_TIM_RESET */
static int32 wl_flowring_update(hnd_dev_t *dev, uint16 flowid, uint8 op, uint8 * sa,
	uint8 *da, uint8 tid)
{
#if defined(PROP_TXSTATUS)
	wl_info_t *wl = dev->softc;
	wl_if_t *wlif = WL_IF(dev);
	struct wlc_if *wlcif;

	ASSERT(wlif != NULL);
	wlcif = wlif->wlcif;

	if (PROP_TXSTATUS_ENAB(((wlc_info_t *)(wl->wlc))->pub)) {
		return wlc_link_txflow_scb(wl->wlc, wlcif, flowid, op, sa, da, tid);
	} else
#endif /* (PROP_TXSTATUS) */
	{
		return 0xFF;
	}
}
#endif /* BCMPCIEDEV */

#if defined(WLWFDS)
#endif /* WLWFDS */

/**
 * PROP_TXSTATUS && BCMPCIEDEV specific function. Called when the WL layer wants to report a flow
 * control related event (e.g. MAC_OPEN), this function will lead the event towards a higher
 * firmware layer that consumes the event.
 */
void wl_flowring_ctl(wl_info_t *wl, uint32 op, void *opdata)
{
#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB()) { /* forward event to bus layer */
		bus_ops->flowring_ctl((void *)wl->dev->chained, op, (void *)opdata);
	}
#endif /* BCMPCIEDEV */
}

/* Send wl traffic control information to bus level */
void wl_sendctl_tx(wl_info_t *wl, uint8 type, uint32 op, void *opdata)
{
	bus_ops->sendctl_tx((void *)wl->dev->chained, type, op, (void *)opdata);
}

#if defined(WLATF_DONGLE)
/** Enable/Disable Fair Fetch Scheduling in pciedev layer */
int
wlfc_enab_fair_fetch_scheduling(struct wl_info *wl, uint32 enab)
{
	flowring_op_data_t	op_data;

	if (BCMPCIEDEV_ENAB()) {
		bzero(&op_data, sizeof(flowring_op_data_t));
		op_data.extra_params = &enab;
		wl_flowring_ctl(wl, WLFC_CTL_TYPE_ENAB_FFSCH, (void *)&op_data);
	}
	return BCME_OK;
}

/** Get Fair Fetch Scheduling status from pciedev layer.
 * status - is the status (1 - on, 0 - off)
 */
int
wlfc_get_fair_fetch_scheduling(struct wl_info *wl, uint32 *status)
{
	int rv = BCME_OK;
	if (BCMPCIEDEV_ENAB()) {
		/* Need to generate an ioctl/iovar request to the bus */
		int cmd_len = strlen("bus:ffsched");
		int cmd_buf_len = cmd_len + 1 + sizeof(uint32);
		char* cmd_buf = (char*)MALLOC(wl->pub->osh, cmd_buf_len);
		if (cmd_buf != NULL) {
			strncpy(cmd_buf, "bus:ffsched", cmd_len);
			cmd_buf[cmd_len] = '\0';
			rv = wl_busioctl(wl, BUS_GET_VAR, cmd_buf,
			       cmd_buf_len, NULL, NULL, FALSE);
			if (rv == BCME_OK)
				*status = *((uint32*)cmd_buf);
			else
				WL_ERROR(("wl%d: %s: BUS IOCTL failed, error %d\n",
				  wl->unit, __FUNCTION__, rv));
			MFREE(wl->pub->osh, cmd_buf, cmd_buf_len);
		} else {
			WL_ERROR(("wl%d: %s: MALLOC failed\n",
			  wl->unit, __FUNCTION__));
			rv = BCME_NOMEM;
		}
	} else
		rv = BCME_UNSUPPORTED;

	return rv;
}

#endif /* WLATF_DONGLE */

/* Force function into RAM because behavior is dependent on used interface */
void
wl_flush_rxreorderqeue_flow(struct wl_info *wl, struct reorder_rxcpl_id_list *list)
{
#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB()) {
		uint32 buf[2];
		int ret;

		buf[0] = (uint32)list->head;
		buf[1] = list->cnt;

		ret = wl_busioctl(wl, BUS_FLUSH_RXREORDER_Q, buf,
			2*sizeof(uint32), NULL, NULL, FALSE);

		if (ret) {
			list->cnt = 0;
			list->head = list->tail = 0;
		}
	}
#endif /* BCMPCIEDEV */
}

uint32
wl_chain_rxcomplete_id(struct reorder_rxcpl_id_list *list, uint16 id, bool head)
{
#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB()) {
		if (list->cnt == 0) {
			list->head = list->tail = id;
		} else {
			if (head) {
				bcm_chain_rxcplid(id, list->head);
				list->head = id;
			} else {
				bcm_chain_rxcplid(list->tail, id);
				list->tail = id;
			}
		}
	}
	list->cnt++;
#endif /* BCMPCIEDEV */
	return 0;
}

/** link up rx cplid of amsdu subframes */
void
wl_chain_rxcompletions_amsdu(osl_t *osh, void *p, bool norxcpl)
{
#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB()) {
		void *p1;
		uint16 head,  current;
		head = PKTRXCPLID(osh, p);
		p1 = PKTNEXT(osh, p);
		while (p1 != NULL) {
			current = PKTRXCPLID(osh, p1);
			if (current == 0) {
				return;
			}
			/* Chain rx completions */
			bcm_chain_rxcplid(head, current);
			head = current;
			/* dont send out rx completions if pkt is queued up in pending list */
			if (norxcpl)
				PKTSETNORXCPL(osh, p1);
			p1 = PKTNEXT(osh, p1);
		}
	}
#endif /* BCMPCIEDEV */
}

void
wl_indicate_maccore_state(struct wl_info *wl, uint8 state)
{
#ifdef BCMPCIEDEV
	int err;
	if (BCMPCIEDEV_ENAB()) {
		uint32 buf;

		buf = (uint32)state;

		err = wl_busioctl(wl, BUS_SET_LTR_STATE, &buf, sizeof(uint32), NULL, NULL, FALSE);
		if (err != BCME_OK) {
			WL_ERROR((IOCTL_DBG_STR, wl->unit, __FUNCTION__, BUS_SET_LTR_STATE,
				err));
		}
	}
#endif /* BCMPCIEDEV */
}

/* Add Rx timestamp in the rxcpl_info */
void
wl_timesync_add_rx_timestamp(struct wl_info *wl, void *p, uint32 ts_low, uint32 ts_high)
{
#ifdef BCMPCIEDEV
	uint16 rxcplid;
	rxcpl_info_t *p_rxcpl_info;

	if (!BCMPCIEDEV_ENAB())
		return;

	if (ts_low == 0 && ts_high == 0) {
		WL_ERROR(("wl%d: Bad Rx Timestamps\n", wl->unit));
		return;
	}
	rxcplid = PKTRXCPLID(wl->pub->osh, p);
	p_rxcpl_info = bcm_id2rxcplinfo(rxcplid);
	if (!p_rxcpl_info) {
		WL_ERROR(("wl%d: %s Cannot add Rx Timestamp, no rxcplinfo found\n",
			wl->unit, __FUNCTION__));
		return;
	}
	p_rxcpl_info->rxpkt_ts_low = ts_low;
	p_rxcpl_info->rxpkt_ts_high = ts_high;

#endif /* BCMPCIEDEV */
}

/* Add Tx timestamp in the Tx lfrag */
void
wl_timesync_add_tx_timestamp(struct wl_info *wl, void *p, uint32 ts_low, uint32 ts_high)
{
#ifdef BCMPCIEDEV

	if (!BCMPCIEDEV_ENAB())
		return;
	if (ts_low == 0 && ts_high == 0) {
		WL_ERROR(("wl%d: Bad Tx Timestamps\n", wl->unit));
		return;
	}
	PKTSETTXTSINSERTED(wl->pub->osh, p);
	PKTSETFRAGDATA_LO(wl->pub->osh, p, 1, ts_low);
	PKTSETFRAGDATA_HI(wl->pub->osh, p, 1, ts_high);

#endif /* BCMPCIEDEV */
}

#ifdef HEALTH_CHECK
health_check_client_info_t*
BCMATTACHFN(wl_health_check_module_register)(struct wl_info *wl, const char* name,
	wl_health_check_fn fn, void* context, int module_id)
{
	/* Name parameter is ignored in dongle FW as module id is present */
	/* Its okay to register it this way because the health check may be trigerred
	 * in context of the caller.
	 * The descriptor wl->hc is owned by the wl_rte
	 */
	ASSERT((wl->hc != NULL));

	return health_check_module_register(wl->hc, fn, context, module_id);
}

int
wl_health_check_module_unregister(struct wl_info *wl, health_check_client_info_t *client)
{
	ASSERT((wl->hc != NULL));
	return health_check_module_unregister(wl->hc, client);
}

/* The following function can be registered with one second timer functionality
 * if desired. Hence the argument is void*.
 * This is wl_rte wrapper for health check execute. PCIEDEV_RTE may implement
 * similar wrapper.
 * This wrapper calls health check execution on the descriptor it holds.
 * The following function is intended to be called from wlc_watchdog()
 */
void
wl_health_check_execute(void *wl)
{
	struct wl_info *wl_ptr = (struct wl_info*) wl;
	ASSERT((wl_ptr->hc != NULL));

	health_check_execute(wl_ptr->hc, NULL, 0);
}

/* The following function is called for a subset of clients */
int
wl_health_check_execute_clients(struct wl_info *wl,
	health_check_client_info_t** modules, uint16 num_modules)
{
	ASSERT((wl->hc != NULL));

	return health_check_execute(wl->hc, modules, num_modules);
}
#endif /* HEALTH_CHECK */

void
wl_set_copycount_bytes(struct wl_info *wl, uint16 copycount, uint16 d11rxoffset)
{
	int err;
	uint32 buf[2] = {0};
	buf[0] = copycount;
	buf[1] = d11rxoffset;

	err = wl_busioctl(wl, BUS_SET_COPY_COUNT, buf,
		2 * sizeof(uint32), NULL, NULL, FALSE);
	if (err != BCME_OK) {
		WL_ERROR((IOCTL_DBG_STR, wl->unit, __FUNCTION__, BUS_SET_COPY_COUNT, err));
	}
}

#ifdef BCMFRWDPOOLREORG
static void
wl_set_frwd_resrv_bufcnt(struct wl_info *wl, uint16 frwd_resrv_bufcnt)
{
	int err;
	uint32 buf;
	buf = (uint32)frwd_resrv_bufcnt;
	err = wl_busioctl(wl, BUS_UPDATE_FRWD_RESRV_BUFCNT, &buf,
			sizeof(uint32), NULL, NULL, FALSE);
	if (err != BCME_OK) {
		WL_ERROR(("wl%d: %s: bus ioctl %d failed with error %d\n", wl->unit, __FUNCTION__,
			BUS_UPDATE_FRWD_RESRV_BUFCNT, err));
	}
}

void
wl_upd_frwd_resrv_bufcnt(wl_info_t *wl)
{
	uint16 frwd_resrv_bufcnt = 0;
	uint16 max_frwd_resrv_bufcnt = 0;
	wlc_info_t *wlc = wl->wlc;
	wlc_cmn_info_t *wlc_cmn = wlc->cmn;

	/* query frwd_resrv_bufcnt from all the offload engines here,
	 * which use pool reorg and want to reserve some tx frags.
	 * Take the highest frwd_resrv_bufcnt returned by all the modules
	 * and reserve those many tx frags.
	 */
#ifdef WL_NATOE
	if (NATOE_ENAB(wl->pub)) {
		/* update frwd_resrv_bufcnt */
		frwd_resrv_bufcnt = wl_natoe_get_resrv_buf_cnt(wl->natoe_info);
		max_frwd_resrv_bufcnt = MAX(max_frwd_resrv_bufcnt, frwd_resrv_bufcnt);
	}
#endif /* WL_NATOE */

	if (wlc_cmn->frwd_resrv_bufcnt != frwd_resrv_bufcnt) {
		wlc_cmn->frwd_resrv_bufcnt = frwd_resrv_bufcnt;
		wl_set_frwd_resrv_bufcnt(wl, frwd_resrv_bufcnt);
	}
}
#endif /* BCMFRWDPOOLREORG */

void
wl_inform_additional_buffers(struct wl_info *wl, uint16 buf_cnts)
{
#ifdef BCMPCIEDEV
	if (BCMPCIEDEV_ENAB()) {
		uint32 buf;
		buf = (uint32)buf_cnts;
		wl_busioctl(wl, BUS_UPDATE_EXTRA_TXLFRAGS, &buf,
			sizeof(uint32), NULL, NULL, FALSE);
	}
#endif
}

int wl_fatal_error(void * wl, int rc)
{
	return FALSE;
}
