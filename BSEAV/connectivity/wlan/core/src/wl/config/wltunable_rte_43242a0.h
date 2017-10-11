/*
 * Broadcom 802.11abg Networking Device Driver Configuration file for 43242
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id: wltunable_rte_43242a0.h 256691 2011-05-02 07:04:15Z $:
 *
 * wl driver tunables for RTE dev
 */

#define	D11CONF		0
#define	D11CONF2	0x00000010	/* D11 Core Rev 36 */
#define NCONF		0x100000	/* N-Phy Rev20(LCN 2x2 Rev4) */

#define NTXD			32
#define NRXD			8
#ifndef NRXBUFPOST
#define	NRXBUFPOST		4
#endif
#define WLC_DATAHIWAT	10
#define RXBND			4
#define WLC_MAXBSSCFG	8
#define WLC_MAXTDLS		1
#define	MAXSCB		    10
/* Should be at least WLC_MAXBSSCFG */
#if (MAXSCB < WLC_MAXBSSCFG )
#error MAXSCB definition too small
#endif
#if (MAXSCB < WLC_MAXBSSCFG + WLC_MAXTDLS)
#error MAXSCB definition too small
#endif
#define AIDMAPSZ		32
#define AMPDU_RX_BA_DEF_WSIZE	16	/* Default value to be overridden for dongle */
