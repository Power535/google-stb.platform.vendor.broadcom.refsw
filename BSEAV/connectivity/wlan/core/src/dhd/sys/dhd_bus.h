/*
 * Header file describing the internal (inter-module) DHD interfaces.
 *
 * Provides type definitions and function prototypes used to link the
 * DHD OS, bus, and protocol modules.
 *
 * Copyright (C) 2017, Broadcom. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id$
 */

#ifndef _dhd_bus_h_
#define _dhd_bus_h_

/*
 * Exported from dhd bus module (dhd_usb, dhd_sdio)
 */

/* Indicate (dis)interest in finding dongles. */
extern int dhd_bus_register(void);
extern void dhd_bus_unregister(void);

/* Download firmware image and nvram image */
extern int dhd_bus_download_firmware(struct dhd_bus *bus, osl_t *osh, char *fw_path, char *nv_path);

/* Stop bus module: clear pending frames, disable data flow */
extern void dhd_bus_stop(struct dhd_bus *bus, bool enforce_mutex);

/* Initialize bus module: prepare for communication w/dongle */
extern int dhd_bus_init(dhd_pub_t *dhdp, bool enforce_mutex);

/* Get the Bus Idle Time */
extern void dhd_bus_getidletime(dhd_pub_t *dhdp, int *idletime);

/* Set the Bus Idle Time */
extern void dhd_bus_setidletime(dhd_pub_t *dhdp, int idle_time);

/* Send a data frame to the dongle.  Callee disposes of txp. */
#ifdef BCMPCIE
extern int dhd_bus_txdata(struct dhd_bus *bus, void *txp, uint8 ifidx);
#else
extern int dhd_bus_txdata(struct dhd_bus *bus, void *txp);
#endif


/* Send/receive a control message to/from the dongle.
 * Expects caller to enforce a single outstanding transaction.
 */
extern int dhd_bus_txctl(struct dhd_bus *bus, uchar *msg, uint msglen);
extern int dhd_bus_rxctl(struct dhd_bus *bus, uchar *msg, uint msglen);

/* Watchdog timer function */
extern bool dhd_bus_watchdog(dhd_pub_t *dhd);

extern int dhd_bus_oob_intr_register(dhd_pub_t *dhdp);
extern void dhd_bus_oob_intr_unregister(dhd_pub_t *dhdp);
extern void dhd_bus_oob_intr_set(dhd_pub_t *dhdp, bool enable);
extern void dhd_bus_dev_pm_stay_awake(dhd_pub_t *dhdpub);
extern void dhd_bus_dev_pm_relax(dhd_pub_t *dhdpub);
extern bool dhd_bus_dev_pm_enabled(dhd_pub_t *dhdpub);

#if defined(DHD_DEBUG)
/* Device console input function */
extern int dhd_bus_console_in(dhd_pub_t *dhd, uchar *msg, uint msglen);
#endif /* defined(DHD_DEBUG) */

/* Deferred processing for the bus, return TRUE requests reschedule */
extern bool dhd_bus_dpc(struct dhd_bus *bus);
extern void dhd_bus_isr(bool * InterruptRecognized, bool * QueueMiniportHandleInterrupt, void *arg);


/* Check for and handle local prot-specific iovar commands */
extern int dhd_bus_iovar_op(dhd_pub_t *dhdp, const char *name,
                            void *params, int plen, void *arg, int len, bool set);

/* Add bus dump output to a buffer */
extern void dhd_bus_dump(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);

/* Clear any bus counters */
extern void dhd_bus_clearcounts(dhd_pub_t *dhdp);

/* return the dongle chipid */
extern uint dhd_bus_chip(struct dhd_bus *bus);

/* return the dongle chiprev */
extern uint dhd_bus_chiprev(struct dhd_bus *bus);

/* Set user-specified nvram parameters. */
extern void dhd_bus_set_nvram_params(struct dhd_bus * bus, const char *nvram_params);

extern void *dhd_bus_pub(struct dhd_bus *bus);
extern void *dhd_bus_txq(struct dhd_bus *bus);
extern const void *dhd_bus_sih(struct dhd_bus *bus);
extern uint dhd_bus_hdrlen(struct dhd_bus *bus);
#define dhd_bus_set_dotxinrx(a, b) do {} while (0)

#define DHD_SET_BUS_STATE_DOWN(_bus)  do { \
	(_bus)->dhd->busstate = DHD_BUS_DOWN; \
} while (0)

/* Register a dummy SDIO client driver in order to be notified of new SDIO device */
extern int dhd_bus_reg_sdio_notify(void* semaphore);
extern void dhd_bus_unreg_sdio_notify(void);
extern void dhd_txglom_enable(dhd_pub_t *dhdp, bool enable);
extern int dhd_bus_get_ids(struct dhd_bus *bus, uint32 *bus_type, uint32 *bus_num,
	uint32 *slot_num);

#ifdef DHD_FW_COREDUMP
extern int dhd_bus_mem_dump(dhd_pub_t *dhd);
#endif

#ifdef BCMPCIE
enum {
	/* Scratch buffer confiuguration update */
	D2H_DMA_SCRATCH_BUF,
	D2H_DMA_SCRATCH_BUF_LEN,

	/* DMA Indices array buffers for: H2D WR and RD, and D2H WR and RD */
	H2D_DMA_INDX_WR_BUF, /* update H2D WR dma indices buf base addr to dongle */
	H2D_DMA_INDX_RD_BUF, /* update H2D RD dma indices buf base addr to dongle */
	D2H_DMA_INDX_WR_BUF, /* update D2H WR dma indices buf base addr to dongle */
	D2H_DMA_INDX_RD_BUF, /* update D2H RD dma indices buf base addr to dongle */

	/* DHD sets/gets WR or RD index, in host's H2D and D2H DMA indices buffer */
	H2D_DMA_INDX_WR_UPD, /* update H2D WR index in H2D WR dma indices buf */
	H2D_DMA_INDX_RD_UPD, /* update H2D RD index in H2D RD dma indices buf */
	D2H_DMA_INDX_WR_UPD, /* update D2H WR index in D2H WR dma indices buf */
	D2H_DMA_INDX_RD_UPD, /* update D2H RD index in D2H RD dma indices buf */

	/* DHD Indices array buffers and update for: H2D flow ring WR */
	H2D_IFRM_INDX_WR_BUF, /* update H2D WR dma indices buf base addr to dongle */
	H2D_IFRM_INDX_WR_UPD, /* update H2D WR dma indices buf base addr to dongle */

	/* H2D and D2H Mailbox data update */
	H2D_MB_DATA,
	D2H_MB_DATA,

	/* (Common) MsgBuf Ring configuration update */
	RING_BUF_ADDR,       /* update ring base address to dongle */
	RING_ITEM_LEN,       /* update ring item size to dongle */
	RING_MAX_ITEMS,      /* update ring max items to dongle */

	/* Update of WR or RD index, for a MsgBuf Ring */
	RING_RD_UPD,         /* update ring read index from/to dongle */
	RING_WR_UPD,         /* update ring write index from/to dongle */

	TOTAL_LFRAG_PACKET_CNT,
	MAX_HOST_RXBUFS,
	HOST_API_VERSION,
	DNGL_TO_HOST_TRAP_ADDR,
#ifdef HOFFLOAD_MODULES
	WRT_HOST_MODULE_ADDR
#endif
};

typedef void (*dhd_mb_ring_t) (struct dhd_bus *, uint32);
typedef void (*dhd_mb_ring_2_t) (struct dhd_bus *, uint32, bool);
extern void dhd_bus_cmn_writeshared(struct dhd_bus *bus, void * data, uint32 len, uint8 type,
	uint16 ringid);
extern void dhd_bus_ringbell(struct dhd_bus *bus, uint32 value);
extern void dhd_bus_ringbell_2(struct dhd_bus *bus, uint32 value, bool devwake);
extern void dhd_bus_cmn_readshared(struct dhd_bus *bus, void* data, uint8 type, uint16 ringid);
extern uint32 dhd_bus_get_sharedflags(struct dhd_bus *bus);

#ifdef DHD_WAKE_STATUS
extern void dhd_bus_rx_frame(struct dhd_bus *bus, void* pkt, int ifidx, uint pkt_count,
	int pkt_wake);
#else
extern void dhd_bus_rx_frame(struct dhd_bus *bus, void* pkt, int ifidx, uint pkt_count);
#endif /* DHD_WAKE_STATUS */

extern void dhd_bus_start_queue(struct dhd_bus *bus);
extern void dhd_bus_stop_queue(struct dhd_bus *bus);
extern dhd_mb_ring_t dhd_bus_get_mbintr_fn(struct dhd_bus *bus);
extern dhd_mb_ring_2_t dhd_bus_get_mbintr_2_fn(struct dhd_bus *bus);
extern void dhd_bus_write_flow_ring_states(struct dhd_bus *bus,
	void * data, uint16 flowid);
extern void dhd_bus_read_flow_ring_states(struct dhd_bus *bus,
	void * data, uint8 flowid);
extern int dhd_bus_flow_ring_create_request(struct dhd_bus *bus, void *flow_ring_node);
extern void dhd_bus_clean_flow_ring(struct dhd_bus *bus, void *flow_ring_node);
extern void dhd_bus_flow_ring_create_response(struct dhd_bus *bus, uint16 flow_id, int32 status);
extern int dhd_bus_flow_ring_delete_request(struct dhd_bus *bus, void *flow_ring_node);
extern void dhd_bus_flow_ring_delete_response(struct dhd_bus *bus, uint16 flowid, uint32 status);
extern int dhd_bus_flow_ring_flush_request(struct dhd_bus *bus, void *flow_ring_node);
extern void dhd_bus_flow_ring_flush_response(struct dhd_bus *bus, uint16 flowid, uint32 status);
extern uint32 dhd_bus_max_h2d_queues(struct dhd_bus *bus);
extern int dhd_bus_schedule_queue(struct dhd_bus *bus, uint16 flow_id, bool txs);
extern void dhd_bus_set_linkdown(dhd_pub_t *dhdp, bool val);

#ifdef IDLE_TX_FLOW_MGMT
extern void dhd_bus_flow_ring_resume_response(struct dhd_bus *bus, uint16 flowid, int32 status);
#endif /* IDLE_TX_FLOW_MGMT */

#ifdef BCMDBG
extern void
dhd_bus_flow_ring_cnt_update(struct dhd_bus *bus, uint16 flowid, uint32 txstatus);
#endif

#ifdef OEM_ANDROID
extern int dhdpcie_bus_clock_start(struct dhd_bus *bus);
extern int dhdpcie_bus_clock_stop(struct dhd_bus *bus);
extern int dhdpcie_bus_enable_device(struct dhd_bus *bus);
extern int dhdpcie_bus_disable_device(struct dhd_bus *bus);
extern int dhdpcie_bus_alloc_resource(struct dhd_bus *bus);
extern void dhdpcie_bus_free_resource(struct dhd_bus *bus);
extern bool dhdpcie_bus_dongle_attach(struct dhd_bus *bus);
extern int dhd_bus_release_dongle(struct dhd_bus *bus);
extern int dhd_bus_request_irq(struct dhd_bus *bus);
#endif /* OEM_ANDROID */

extern struct device * dhd_bus_to_dev(struct dhd_bus *bus);

extern void dhdpcie_cto_init(struct dhd_bus *bus, bool enable);


#ifdef DHD_FW_COREDUMP
extern struct dhd_bus *g_dhd_bus;
extern int dhd_dongle_mem_dump(void);
#endif /* DHD_FW_COREDUMP */

#ifdef IDLE_TX_FLOW_MGMT
extern void dhd_bus_idle_tx_ring_suspend(dhd_pub_t *dhd, uint16 flow_ring_id);
#endif /* IDLE_TX_FLOW_MGMT */
extern void dhd_bus_handle_mb_data(struct dhd_bus *bus, uint32 d2h_mb_data);
#endif /* BCMPCIE */

/* dump the device trap informtation  */
extern void dhd_bus_dump_trap_info(struct dhd_bus *bus, struct bcmstrbuf *b);

/* Function to set default min res mask */
extern bool dhd_bus_set_default_min_res_mask(struct dhd_bus *bus);

/* Function to reset PMU registers */
extern void dhd_bus_pmu_reg_reset(dhd_pub_t *dhdp);

#ifdef DHD_ULP
extern void dhd_bus_ulp_disable_console(dhd_pub_t *dhdp);
#endif /* DHD_ULP */
extern int dhd_bus_readwrite_bp_addr(dhd_pub_t *dhdp, uint addr, uint size, uint* data, bool read);
#endif /* _dhd_bus_h_ */
