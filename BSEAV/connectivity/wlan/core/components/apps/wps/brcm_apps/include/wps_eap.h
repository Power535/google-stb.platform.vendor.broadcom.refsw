/*
 * WPS eap
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

#ifndef __WPS_EAP_H__
#define __WPS_EAP_H__

int wps_eap_init();
void wps_eap_deinit();
int wps_eap_process_msg(char *buf, int buflen);

#endif	/* __WPS_EAP_H__ */
