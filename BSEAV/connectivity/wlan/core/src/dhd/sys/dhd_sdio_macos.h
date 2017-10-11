/*
 * DHD SDIO Ethernet for MacOS
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id$
 *
 */
#ifndef __DHD_SDIO_MACOS_H_
#define __DHD_SDIO_MACOS_H_

#include "dhd_macos.h"

extern "C" {
	#include "dhdioctl.h"
	#include "wlioctl.h"
	#include "dngl_stats.h"
	#include "dhd.h"

	#include "sbconfig.h"
	#include "sbhnddma.h"
	#include "sdio.h"
	#include "sbsdio.h"
	#include "sbsdpcmdev.h"
}

const int sTxQLen = 0;
const int sRxQLen = 0;
const int sSdpcmReserve = (SDPCM_FRAMETAG_LEN + SDPCM_SWHEADER_LEN);

class DhdSdioEther : public DhdEther
{
	OSDeclareDefaultStructors(DhdSdioEther)

protected:

public:
	/*
	 * Static member variables
	 */
	static IOPCIDevice	*m_pciNub;

	virtual bool init(OSDictionary *properties);
	virtual void free();
	virtual bool start(IOService *provider);
	virtual void stop(IOService *provider);
	virtual bool terminate(IOOptionBits options = 0);

	UInt32 outputPacket(mbuf_t m, void *param);

	/*
	 * DhdEther methods - implement these
	 */
	virtual IOReturn getHardwareAddress(IOEthernetAddress *addr);
	virtual IOReturn enable(IONetworkInterface *netif);
	virtual IOReturn disable(IONetworkInterface *netif);
	virtual IOReturn setPromiscuousMode(IOEnetPromiscuousMode mode);
	virtual const OSString *newVendorString(void) const;
	virtual const OSString *newModelString(void) const;
	virtual const OSString *newRevisionString(void) const;
};
#endif /* __DHD_SDIO_MACOS_H_ */
