/*
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
 * Description: Implement functions handling WLAN activities
 *
 */

#include <stdlib.h>
#include <string.h>
#include <wpscli_osl.h>
#include <tutrace.h>

static int join_network_with_bssid(const char* ssid, uint32 wsec, const char *bssid);
static int leave_network(void);

brcm_wpscli_status wpscli_wlan_open(void)
{
    return WPS_STATUS_SUCCESS;
}

brcm_wpscli_status wpscli_wlan_close(void)
{
    return WPS_STATUS_SUCCESS;
}

// make a wlan connection. 
brcm_wpscli_status wpscli_wlan_connect(const char* ssid, uint32 wsec, const char *bssid,
	int num_chanspec, chanspec_t *chanspec)
{
	if(join_network_with_bssid(ssid, wsec, bssid) == 0)
		return WPS_STATUS_SUCCESS;

	return WPS_STATUS_WLAN_CONNECTION_ATTEMPT_FAIL;
}

// disconnect wlan connection
brcm_wpscli_status wpscli_wlan_disconnect(void)
{
	leave_network();

	return WPS_STATUS_SUCCESS;
}

brcm_wpscli_status wpscli_wlan_scan(wl_scan_results_t *ap_list, uint32 buf_size)
{
	brcm_wpscli_status status = WPS_STATUS_WLAN_NO_ANY_AP_FOUND;
	int retry;
	wl_scan_params_t* params;
	int params_size = WL_SCAN_PARAMS_FIXED_SIZE + WL_NUMCHANNELS * sizeof(uint16);

	TUTRACE((TUTRACE_INFO, "Entered: wpscli_wlan_scan\n"));

	if(ap_list == NULL)
		return WPS_STATUS_INVALID_NULL_PARAM;

	params = (wl_scan_params_t*)malloc(params_size);

	if (params == NULL) {
		printf("Error allocating %d bytes for scan params\n", params_size);
		return WPS_STATUS_SYSTEM_ERR;
	}

	memset(params, 0, params_size);
	params->bss_type = DOT11_BSSTYPE_ANY;
	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
	params->scan_type = -1;
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;

	wpscli_wlh_ioctl_set(WLC_SCAN, (const char *)params, params_size);

	/* Poll for the results once a second until the scan is done */
	for (retry = 0; retry < WLAN_SCAN_TIMEOUT; retry++) {
		wpscli_sleep(1000);

		ap_list->buflen = WPS_DUMP_BUF_LEN;

		status = wpscli_wlh_ioctl_get(WLC_SCAN_RESULTS, (char *)ap_list, buf_size);

		/* break out if the scan result is ready */
		if (status == WPS_STATUS_SUCCESS)
			break;
	}

	free(params);

	TUTRACE((TUTRACE_INFO, "Exit: wpscli_wlan_scan. status=%d\n", status));
	return status;
}

int join_network(char* ssid, uint32 wsec)
{
	int ret = 0, retry;
	wlc_ssid_t ssid_t;
	int auth = 0, infra = 1;
	int wpa_auth = WPA_AUTH_DISABLED;
	char bssid[6];

	TUTRACE((TUTRACE_INFO, "Entered: join_network. ssid=[%s] wsec=%d\n", ssid, wsec));

	printf("Joining network %s - %d\n", ssid, wsec);

	/*
	 * If wep bit is on,
	 * pick any WPA encryption type to allow association.
	 * Registration traffic itself will be done in clear (eapol).
	*/
	if (wsec)
		wsec = 2; /* TKIP */
	ssid_t.SSID_len = strlen(ssid);
	strncpy((char *)ssid_t.SSID, ssid, ssid_t.SSID_len);
	ssid_t.SSID[ssid_t.SSID_len] = '\0';

	/* set infrastructure mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_INFRA,
		(const char *)&infra, sizeof(int))) < 0)
		return ret;

	/* set authentication mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_AUTH,
		(const char *)&auth, sizeof(int))) < 0)
		return ret;

	/* set wsec mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_WSEC,
		(const char *)&wsec, sizeof(int))) < 0)
		return ret;

	/* set WPA_auth mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_WPA_AUTH,
		(const char *)&wpa_auth, sizeof(wpa_auth))) < 0)
		return ret;

	/* set ssid */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_SSID,
		(const char *)&ssid_t, sizeof(wlc_ssid_t))) == 0) {
		/* Poll for the results once a second until we got BSSID */
		for (retry = 0; retry < WLAN_CONNECTION_TIMEOUT; retry++) {

			wpscli_sleep(1000);

			ret = wpscli_wlh_ioctl_get(WLC_GET_BSSID, bssid, 6);

			/* break out if the scan result is ready */
			if (ret == 0)
				break;

			if (retry != 0 && retry % 10 == 0) {
				if ((ret = wpscli_wlh_ioctl_set(WLC_SET_SSID,
					(const char *)&ssid_t, sizeof(wlc_ssid_t))) < 0) {
					TUTRACE((TUTRACE_INFO, "join_network: Exiting. ret=%d\n", ret));
					return ret;
				}
			}
		}
	}

	TUTRACE((TUTRACE_INFO, "join_network: Exiting. ret=%d\n", ret));
	return ret;
}

static int join_network_with_bssid(const char* ssid, uint32 wsec, const char *bssid)
{
#if !defined(WL_ASSOC_PARAMS_FIXED_SIZE) || !defined(WL_JOIN_PARAMS_FIXED_SIZE)
	return (join_network(ssid, wsec));
#else
	int ret = 0, retry;
	int auth = 0, infra = 1;
	int wpa_auth = WPA_AUTH_DISABLED;
	char associated_bssid[6];
	wl_join_params_t join_params;
	wlc_ssid_t *ssid_t = &join_params.ssid;
	wl_assoc_params_t *params_t = &join_params.params;

	TUTRACE((TUTRACE_INFO, "Entered: join_network_with_bssid. ssid=[%s] wsec=%d\n", ssid, wsec));
	
	printf("Joining network %s - %d\n", ssid, wsec);
	printf("BSSID: %02x-%02x-%02x-%02x-%02x-%02x\n",
		(unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2],
		(unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

	memset(&join_params, 0, sizeof(join_params));

	/*
	 * If wep bit is on,
	 * pick any WPA encryption type to allow association.
	 * Registration traffic itself will be done in clear (eapol).
	*/
	if (wsec)
		wsec = 2; /* TKIP */

	/* ssid */
	ssid_t->SSID_len = strlen(ssid);
	strncpy((char *)ssid_t->SSID, ssid, ssid_t->SSID_len);
	ssid_t->SSID[ssid_t->SSID_len] = '\0';

	/* bssid (if any) */
	if (bssid)
		memcpy(&params_t->bssid, bssid, ETHER_ADDR_LEN);
	else
		memcpy(&params_t->bssid, &ether_bcast, ETHER_ADDR_LEN);

	/* set infrastructure mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_INFRA,
		(const char *)&infra, sizeof(int))) < 0)
		return ret;

	/* set authentication mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_AUTH,
		(const char *)&auth, sizeof(int))) < 0)
		return ret;

	/* set wsec mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_WSEC,
		(const char *)&wsec, sizeof(int))) < 0)
		return ret;

	/* set WPA_auth mode */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_WPA_AUTH,
		(const char *)&wpa_auth, sizeof(wpa_auth))) < 0)
		return ret;

	/* set ssid */
	if ((ret = wpscli_wlh_ioctl_set(WLC_SET_SSID,
		(const char *)&join_params, sizeof(wl_join_params_t))) == 0) {
		/* Poll for the results once a second until we got BSSID */
		for (retry = 0; retry < WLAN_CONNECTION_TIMEOUT; retry++) {

			wpscli_sleep(1000);

			ret = wpscli_wlh_ioctl_get(WLC_GET_BSSID, associated_bssid, 6);

			/* break out if the scan result is ready */
			if (ret == 0)
				break;

			if (retry != 0 && retry % 10 == 0) {
				if ((ret = wpscli_wlh_ioctl_set(WLC_SET_SSID,
					(const char *)&join_params,	sizeof(wl_join_params_t))) < 0) {
					TUTRACE((TUTRACE_INFO, "join_network_with_bssid: Exiting. ret=%d\n", ret));
					return ret;
				}
			}
		}
	}

	TUTRACE((TUTRACE_INFO, "join_network_with_bssid: Exiting. ret=%d\n", ret));
	return ret;
#endif /* !defined(WL_ASSOC_PARAMS_FIXED_SIZE) || !defined(WL_JOIN_PARAMS_FIXED_SIZE) */
}

static int leave_network(void)
{
	return wpscli_wlh_ioctl_set(WLC_DISASSOC, NULL, 0);
}
