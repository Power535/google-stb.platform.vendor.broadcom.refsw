/*
 * Trace messages sent over HBUS
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id: logtrace.c 287537 2011-10-03 23:43:46Z $
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmendian.h>
#include <bcmutils.h>
#include <msgtrace.h>
#include <event_log.h>
#include <rte.h>
#include <rte_timer.h>
#include <rte_dev.h>
#include <dngl_logtrace.h>

/* Retry timeout value to handle the retry of lost message */
#define RETRY_TIMEOUT_VALUE	500

/* Send timeout value to trigger immmediately the sending by leaving the context of caller */
#define SEND_TIMEOUT_VALUE	0

/* Definition of trace buffer for sending trace over host bus */
typedef struct logtrace {
	uint32	seqnum;			/* Sequence number of event sent */
	hnd_timer_t *timer;		/* Timer used to trigger the sending  of trace buffer and
					 * used to handle the retry of lost event
					 */
	bool	pending;		/* Msg sent but not ackd */
	bool	timer_active;		/* Timer is active value */
	logtrace_sendup_trace_fn_t func_send; /* Function pointer to send trace event */
	void	*ctx;			/* ctx : Context used to send trace event */
	bool 	event_trace_enabled;   	/* EVENT_TRACE enabled/disabled flag */
	uint8	*last_trace;		/* Pointer to last trace sent */
} logtrace_t;


static logtrace_t *logtrace = NULL;

void
logtrace_stop(void)
{
	if (!logtrace)
		return;
	if (logtrace->event_trace_enabled) {
		if (logtrace->timer_active) {
			hnd_timer_stop(logtrace->timer);
			logtrace->timer_active = FALSE;
		}
		logtrace->event_trace_enabled = FALSE;
	}
}

void
logtrace_start(void)
{
	if (!logtrace)
		return;
	if (!logtrace->event_trace_enabled) {
		logtrace->event_trace_enabled = TRUE;
	}
}

int logtrace_sent_call = 0;
int sent_call_add = 0;
int sent_call_cancel = 0;

static void logtrace_trigger(void *ctx);

/* Called when the trace has been sent over the HBUS. */
int
logtrace_sent(void)
{
	if (!logtrace)
		return 0;
	logtrace_sent_call++;
	if (logtrace->timer_active) {
		sent_call_cancel++;
		hnd_timer_stop(logtrace->timer);
		logtrace->timer_active = FALSE;
	}

	/* Clear indicators */
	logtrace->pending = FALSE;

	if (!logtrace->event_trace_enabled) {
		return 0;
	}

	/* Trigger again to see if thre is more to send */
	logtrace_trigger(logtrace);

	return 1;
}

static void
logtrace_trigger(void *ctx)
{
	logtrace_t *logtrace = (logtrace_t *)ctx;

	if (logtrace->timer_active) {
		return;				/* Already pending */
	}

	if (logtrace->event_trace_enabled) {
		/* Trigger immediately the sending by setting the timer to 0 */
		hnd_timer_start(logtrace->timer, SEND_TIMEOUT_VALUE, FALSE);
		logtrace->timer_active = TRUE;
	}
}

#if !defined(LOGTRACE_PCIE)
static void
logtrace_timeout(hnd_timer_t *t)
{
	int set_num;
	msgtrace_hdr_t hdr;

	if (!logtrace)
		return;
	logtrace->timer_active = FALSE;		/* Just fired */

	if (!logtrace->event_trace_enabled) {
		return;
	}

	if (logtrace->pending == FALSE) {
		/* Look for a set with something to send.  We do not
		 * round-robin under the assumption that the sets are ordered
		 * from most active or most important to last.
		 */
		for (set_num = 0; set_num < NUM_EVENT_LOG_SETS; set_num++) {
			logtrace->last_trace = event_log_next_logtrace(set_num);
			if (logtrace->last_trace != NULL) {
				/* Only looking for destination "HOST" log sets. */
				if (event_log_set_destination_get(set_num)
					!= SET_DESTINATION_HOST) {
					continue;
				}
				/* First send of the event. */
				logtrace->seqnum++;
				break;			/* Found one */
			}
		}
	}

	logtrace->pending = FALSE;		/* Assume not pending */

	if (logtrace->func_send == NULL) {
		return;
	}

	if (logtrace->last_trace != NULL) {
		/* Fill the trace header */
		uint16 len       = *((uint16 *) logtrace->last_trace);
		hdr.version	     = MSGTRACE_VERSION;
		hdr.trace_type	     = MSGTRACE_HDR_TYPE_LOG;
		hdr.len              = hton16(len);
		hdr.seqnum           = hton32(logtrace->seqnum);

		/* Just to make debugging/skipping these unused fields easier */
		hdr.discarded_bytes = 0xffffffff;
		hdr.discarded_printf = 0xffffffff;

		logtrace->pending = TRUE;
		logtrace->func_send(logtrace->ctx,
		                    (uint8*)&hdr, sizeof(msgtrace_hdr_t),
		                    logtrace->last_trace, len);

		hnd_timer_start(logtrace->timer, RETRY_TIMEOUT_VALUE, FALSE);
		logtrace->timer_active = TRUE;
	}
}
#else
#if defined(BCMPCIEDEV_ENABLED)
extern hnd_dev_t pciedev_dev;
extern osl_t* hnd_osh;
#else
#error "LOGTRACE_PCIE in non PCIE target"
#endif
#include <bcmmsgbuf.h>
static void
logtrace_timeout(hnd_timer_t *t)
{
	int set_num;
	void *trace;

	if (!logtrace)
		return;
	logtrace->timer_active = FALSE;		/* Just fired */

	if (!logtrace->event_trace_enabled) {
		return;
	}

	/* Look for a set with something to send. */
	for (set_num = 0; set_num < NUM_EVENT_LOG_SETS; set_num++) {
		/* Only looking for destination "HOST" log sets. */
		if (event_log_set_destination_get(set_num) != SET_DESTINATION_HOST) {
			continue;
		}
		while ((trace = event_log_next_logtrace(set_num)) != NULL) {
			uint16 trace_len;
			msgtrace_hdr_t hdr;
			void *p;
			uint8 *databuf;
			struct lbuf *lb;
			info_buf_payload_hdr_t payload_hdr;
			uint32 pktlen;

			logtrace->seqnum++;

			trace_len = *((uint16 *)trace);
			pktlen = sizeof(uint32) + sizeof(struct info_buf_payload_hdr_s) +
				sizeof(msgtrace_hdr_t) + trace_len;

			if ((p = PKTGET(hnd_osh, pktlen + BCMEXTRAHDROOM, FALSE)) == NULL) {
				return;
			}

			PKTPULL(hnd_osh, p, BCMEXTRAHDROOM);

			databuf = PKTDATA(hnd_osh, p);

			/* Fill info buffer version */
			*((uint32 *)databuf) = PCIE_INFOBUF_V1;
			databuf += sizeof(uint32);

			/* Fill info buffer payload type and length */
			payload_hdr.type = PCIE_INFOBUF_V1_TYPE_LOGTRACE;
			payload_hdr.length = sizeof(msgtrace_hdr_t) + trace_len;
			bcopy(&payload_hdr, databuf, sizeof(struct info_buf_payload_hdr_s));
			databuf += sizeof(struct info_buf_payload_hdr_s);

			/* Fill the trace header */
			hdr.version	     = MSGTRACE_VERSION;
			hdr.trace_type	     = MSGTRACE_HDR_TYPE_LOG;
			hdr.len              = hton16(trace_len);
			hdr.seqnum           = hton32(logtrace->seqnum);

			/* Just to make debugging/skipping these unused fields easier */
			hdr.discarded_bytes = 0xffffffff;
			hdr.discarded_printf = 0xffffffff;

			bcopy(&hdr, databuf, sizeof(msgtrace_hdr_t));
			databuf += sizeof(msgtrace_hdr_t);

			bcopy(trace, databuf, trace_len);

			/* PCIE INFO BUF packets are being "flagged" as an ALTINTF packet.  We are
			 * out of packet flag bits on this branch so I need to hijack a bit.  The
			 * ALTINTF feature was never deployed and isn't expected to return any time
			 * soon ... if ever.  Technically the right thing to do would be to
			 * redefine the flag bit but the whole packet flag bit is over-abstracted.
			 * It would requiring creating new named functions/abstractions through both
			 * the lbuf and OSL.  In trunk, first there are more flag bits, and second
			 * the abstraction should probably be changed to be actual packet flag
			 * bits/mask, not named functions.  Bottom line: I am just going to hijack
			 * the bit as opposed to doing a bunch of thrash on this branch that what
			 * we would not port to trunk regardless.  Instead I am making a nice
			 * comment. :-)
			 */
			PKTSETALTINTF(p, TRUE);

			lb = PKTTONATIVE(hnd_osh, p);

			if (pciedev_dev.ops->xmit(NULL, &pciedev_dev, lb) != 0) {
				lb_free(lb);
			}
		}
	}
}
#endif /* !defined(LOGTRACE_PCIE) */

int
BCMATTACHFN(logtrace_init)(osl_t* osh)
{
	if (logtrace != NULL) {
		return BCME_OK;
	}

	if ((logtrace = (logtrace_t*)MALLOCZ(osh, sizeof(logtrace_t))) == NULL) {
		return BCME_NOMEM;
	}

	logtrace->pending = FALSE;
	logtrace->timer_active = FALSE;
	logtrace->last_trace = NULL;

	logtrace->timer  = hnd_timer_create(NULL, NULL, logtrace_timeout, NULL, NULL);

#ifdef EVENT_LOG_COMPILE
	event_log_set_logtrace_trigger_fn(logtrace_trigger, logtrace);
#endif

	return BCME_OK;
}

void
BCMATTACHFN(logtrace_deinit)(osl_t* osh)
{
#ifdef EVENT_LOG_COMPILE
	event_log_set_logtrace_trigger_fn(NULL, NULL);
#endif
	if (logtrace != NULL) {
		MFREE(osh, logtrace, sizeof(logtrace_t));
		logtrace = NULL;
	}
}

void
BCMATTACHFN(logtrace_set_sendup_trace_fn)(logtrace_sendup_trace_fn_t fn, void *ctx)
{
	ASSERT(fn != NULL);
	ASSERT(logtrace != NULL);

	logtrace->func_send = fn;
	logtrace->ctx = ctx;
}
