/*
 * Broadcom Dongle Host Driver (DHD), Generic work queue framework
 * Generic interface to handle dhd deferred work events
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
#ifndef _dhd_linux_wq_h_
#define _dhd_linux_wq_h_
/*
 *	Work event definitions
 */
enum _wq_event {
	DHD_WQ_WORK_IF_ADD = 1,
	DHD_WQ_WORK_IF_DEL,
	DHD_WQ_WORK_SET_MAC,
	DHD_WQ_WORK_SET_MCAST_LIST,
	DHD_WQ_WORK_IPV6_NDO,
	DHD_WQ_WORK_HANG_MSG,
	DHD_WQ_WORK_SOC_RAM_DUMP,
	DHD_WQ_WORK_DHD_LOG_DUMP,
	DHD_WQ_WORK_INFORM_DHD_MON,
	DHD_WQ_WORK_EVENT_LOGTRACE,
	DHD_WQ_WORK_NATOE_EVENT,
	DHD_WQ_WORK_NATOE_IOCTL,
	DHD_MAX_WQ_EVENTS
};

/*
 *	Work event priority
 */
enum wq_priority {
	DHD_WQ_WORK_PRIORITY_LOW = 1,
	DHD_WQ_WORK_PRIORITY_HIGH,
	DHD_WQ_MAX_PRIORITY
};

/*
 *	Error definitions
 */
#define DHD_WQ_STS_OK			 0
#define DHD_WQ_STS_FAILED		-1	/* General failure */
#define DHD_WQ_STS_UNINITIALIZED	-2
#define DHD_WQ_STS_SCHED_FAILED		-3
#define DHD_WQ_STS_UNKNOWN_EVENT	-4
#define DHD_WQ_STS_UNKNOWN_PRIORITY	-5

typedef void (*event_handler_t)(void *handle, void *event_data, u8 event);

void *dhd_deferred_work_init(void *dhd);
void dhd_deferred_work_deinit(void *workq);
int dhd_deferred_schedule_work(void *workq, void *event_data, u8 event,
	event_handler_t evt_handler, u8 priority);
#endif /* _dhd_linux_wq_h_ */
