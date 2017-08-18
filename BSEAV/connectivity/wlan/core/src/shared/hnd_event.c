/*
 * Event support for hnd.
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id:$
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <rte.h>
#include <bcmutils.h>
#include <bcmdevs.h>
#include <hnd_debug.h>
#include <bcmendian.h>
#include <proto/ethernet.h>
#include <proto/802.11.h>
#include <hnd_event.h>

#ifdef EVENT_LOG_COMPILE
#define HND_EVENT_ERROR(args)	\
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_HNDRTE_EVENT_ERROR, args)
#else
#define HND_EVENT_ERROR(args)	printf args
#endif

/* Definition of trace buffer for sending trace over host bus */
typedef struct hnd_event {
	osl_t *osh;
	hnd_dev_t    *busdev;
	uint32  maxsz;          /* maximum size allowed */
	uint32  seqnum;         /* Sequence number of event sent */
} hnd_event_t;

static hnd_event_t *hnd_ev_ptr = NULL;

void
BCMATTACHFN(hnd_event_init)(osl_t *osh, hnd_dev_t *busdev)
{
	if (busdev == NULL)
		return;

	hnd_ev_ptr = (hnd_event_t *) MALLOCZ(osh, sizeof(hnd_event_t));
	if (!hnd_ev_ptr)
		return;

	hnd_ev_ptr->osh = osh;
	hnd_ev_ptr->busdev = busdev;
	hnd_ev_ptr->maxsz = PKTBUFSZ;

}

int
hnd_send_dngl_event(uint32 dngl_event, uchar *data, uint32 datalen)
{
	void *p;
	bcm_dngl_event_t *msg;
	char *databuf;
	struct lbuf *lb;
	uint32 pktlen = sizeof(bcm_dngl_event_t) + datalen + BCMEXTRAHDROOM + 2;

	if (!hnd_ev_ptr) {
		return BCME_ERROR;
	}

	if (pktlen > hnd_ev_ptr->maxsz) {
		HND_EVENT_ERROR(("datalen:%d execeeds event data size\n", datalen));
		return BCME_ERROR;
	}

	if ((p = PKTGET(hnd_ev_ptr->osh, pktlen, FALSE)) == NULL) {
		return BCME_ERROR;
	}

	ASSERT(ISALIGNED(PKTDATA(hnd_ev_ptr->osh, p), sizeof(uint32)));

	memset(PKTDATA(hnd_ev_ptr->osh, p), 0, pktlen);

	/* make room for headers; ensure we start on an odd 16 bit offset */
	PKTPULL(hnd_ev_ptr->osh, p, BCMEXTRAHDROOM + 2);

	msg = (bcm_dngl_event_t *) PKTDATA(hnd_ev_ptr->osh, p);

	msg->eth.ether_type = hton16(ETHER_TYPE_BRCM);

	/* BCM Vendor specific header... */
	msg->bcm_hdr.subtype = hton16(BCMILCP_SUBTYPE_VENDOR_LONG);
	msg->bcm_hdr.version = BCMILCP_BCM_SUBTYPEHDR_VERSION;
	bcopy(BRCM_OUI, &msg->bcm_hdr.oui[0], DOT11_OUI_LEN);

	/* vendor spec header length + pvt data length (private indication hdr + actual message
	 * itself)
	 */
	msg->bcm_hdr.length = hton16(BCMILCP_BCM_SUBTYPEHDR_MINLENGTH +
		BCM_DNGL_LEN + (uint16)datalen);
	msg->bcm_hdr.usr_subtype = hton16(BCMILCP_BCM_SUBTYPE_DNGLEVENT);


	/* update the event struct */
	msg->dngl_event.version = hton16(BCM_DNGL_EVENT_MSG_VERSION);
	msg->dngl_event.event_type = hton16(dngl_event);
	msg->dngl_event.datalen = hton16(datalen);

	databuf = (char *)(msg + 1);
	if (data) {
		/* Copy the data */
		memcpy(databuf, data, datalen);
	}
	PKTSETLEN(hnd_ev_ptr->osh, p, (sizeof(bcm_dngl_event_t) + datalen)
		+ ETHER_TYPE_LEN);

	PKTSETTYPEEVENT(hnd_ev_ptr->osh, p);

	lb = PKTTONATIVE(hnd_ev_ptr->osh, p);

	if (hnd_ev_ptr->busdev->ops->xmit(NULL, hnd_ev_ptr->busdev, lb) != 0) {
		lb_free(lb);
		return BCME_ERROR;
	}
	hnd_ev_ptr->seqnum ++;
	return BCME_OK;
}


void
hnd_event_socramind_assrt(const char *fn, uint32 caller)
{
	uint8 *p;
	int fnlen = strlen(fn) + 1;
	int fnlen_pad, datalen, totlen;
	bcm_dngl_socramind_t	*dngl_socramind_ptr;

	if (!hnd_ev_ptr)
		return;

	/* Add the data in TLV format. Tag+Len is 32 bit */
	fnlen_pad = ROUNDUP(fnlen, sizeof(uint32));
	datalen = fnlen_pad + sizeof(caller);
	totlen = datalen + sizeof(uint32);

	dngl_socramind_ptr = MALLOCZ(hnd_ev_ptr->osh, totlen);
	p = (uchar *)dngl_socramind_ptr;
	if (dngl_socramind_ptr) {
		dngl_socramind_ptr->tag = SOCRAM_IND_ASSERT_TAG;
		dngl_socramind_ptr->length = datalen;
		p = dngl_socramind_ptr->value;
		memcpy(p, (uint8 *)fn, fnlen);
		p += fnlen_pad;
		memcpy(p, &caller, sizeof(uint32));

		if (hnd_send_dngl_event(DNGL_E_SOCRAM_IND, (uchar *)dngl_socramind_ptr, totlen)
			!= BCME_OK) {
			HND_EVENT_ERROR(("Couldn't send Socram indication event\n"));
		}
		MFREE(hnd_ev_ptr->osh, dngl_socramind_ptr, totlen);
	}
}
