#ifdef BCMSMS4_TEST
/*
 * sms4_vectors.h
 * SMS4 block cipher test vectors
 *
 * Copyright (C) 2017, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: sms4_vectors.h 241182 2011-02-17 21:50:03Z $
 */

#include <typedefs.h>

/* Example 1 from SMS4 specification */
uint32 skey_00[] = {
	0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210
	};

uint32 sinput_00[] = {
	0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210
	};

uint32 sniter_00 = 1;

uint32 sref_00[] = {
	0x681edf34, 0xd206965e, 0x86b3e94f, 0x536e4246
	};

/* Example 2 from SMS4 specification */
uint32 skey_01[] = {
	0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210
	};

uint32 sinput_01[] = {
	0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210
	};

uint32 sniter_01 = 1000000;

uint32 sref_01[] = {
	0x595298c7, 0xc6fd271f, 0x0402f804, 0xc33d3f66
	};

typedef struct {
	uint32 *key;
	uint32 *input;
	uint32 *niter;
	uint32 *ref;
} sms4_vector_t;

#define SMS4_VECTOR_ENTRY(x)    \
	{ skey_##x, sinput_##x, &sniter_##x, sref_##x }

sms4_vector_t sms4_vec[] = {
    SMS4_VECTOR_ENTRY(00),
    SMS4_VECTOR_ENTRY(01)
    };

#define NUM_SMS4_VECTORS  (sizeof(sms4_vec)/sizeof(sms4_vec[0]))

/* Locally generated vectors for SMS4-WPI-CBC-MAC */
uint8 cmick_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 cmpn_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

/* Last two octets of AAD are the Data Len */
uint8 cmaad_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
	};

/* Buffer must be big enough to hold MAC */
uint8 cminput_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 cmref_00[] = {
	0x7b, 0xb6, 0xfd, 0x05, 0x00, 0xf2, 0x17, 0x34,
	0x33, 0xeb, 0xf4, 0x82, 0x30, 0x53, 0x18, 0xa5
	};

uint8 cmick_01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

uint8 cmpn_01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

/* Last two octets of AAD are the Data Len */
uint8 cmaad_01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x00, 0x10
	};

/* Buffer must be big enough to hold MAC */
uint8 cminput_01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 cmref_01[] = {
	0x89, 0xbb, 0x1a, 0x35, 0x23, 0xf7, 0x4a, 0x6a,
	0x5d, 0xca, 0x99, 0x40, 0xa0, 0x1b, 0x09, 0x24
	};

uint8 cmick_02[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

uint8 cmpn_02[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

/* Last two octets of AAD are the Data Len */
uint8 cmaad_02[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x00, 0x01, 0x02, 0x00, 0x11
	};

/* Buffer must be big enough to hold MAC */
uint8 cminput_02[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00
	};

uint8 cmref_02[] = {
	0xf8, 0xae, 0x11, 0x01, 0xf7, 0xe4, 0x88, 0x25,
	0xdb, 0xed, 0xd1, 0xa7, 0xec, 0xe9, 0x34, 0xb8
	};


typedef struct {
	uint8 *ick;
	uint8 *pn;
	size_t al;
	uint8 *aad;
	uint8 *input;
	uint8 *ref;
} sms4_cbc_mac_vector_t;

#define SMS4_CBC_MAC_VECTOR_ENTRY(x)    \
	{ cmick_##x, cmpn_##x, sizeof(cmaad_##x), cmaad_##x, cminput_##x, cmref_##x }

sms4_cbc_mac_vector_t sms4_cbc_mac_vec[] = {
    };

#define NUM_SMS4_CBC_MAC_VECTORS  (sizeof(sms4_cbc_mac_vec)/sizeof(sms4_cbc_mac_vec[0]))

/* Locally generated vectors for SMS4-OFB */
uint8 ofbek_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbpn_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbinput_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbref_00[] = {
	0xff, 0x7b, 0x1f, 0x9f, 0x38, 0x11, 0x55, 0x6f,
	0x53, 0x30, 0x94, 0x4d, 0xd3, 0x8f, 0x53, 0x1e
	};

uint8 ofbek_01[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

uint8 ofbpn_01[] = {
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	};

uint8 ofbinput_01[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbref_01[] = {
	0xb0, 0x3b, 0xe1, 0xd2, 0xa0, 0x38, 0xcc, 0xe3,
	0xfa, 0x5f, 0xee, 0x3f, 0x10, 0xb6, 0x15, 0xe7
	};

uint8 ofbek_02[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};

uint8 ofbpn_02[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbinput_02[] = {
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	};

uint8 ofbref_02[] = {
	0xfc, 0x57, 0x0d, 0x48, 0x02, 0x30, 0xf1, 0x3f,
	0x52, 0x9e, 0xf9, 0x85, 0x83, 0x1b, 0xdb, 0x2b
	};

uint8 ofbek_03[] = {
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	};

uint8 ofbpn_03[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbinput_03[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 ofbref_03[] = {
	0x70, 0x68, 0xd0, 0xf8, 0x1e, 0x32, 0xea, 0xb9,
	0x3b, 0x1d, 0x6a, 0xe9, 0xb5, 0xa5, 0x2f, 0x0e,
	0xfc, 0xe7, 0x9a, 0xac, 0x3a, 0x10, 0x01, 0xca,
	0x73, 0x10, 0x14, 0x1c, 0xcf, 0x78, 0xc9, 0x7f,
	0x81, 0xca, 0xe1, 0x71, 0xe4, 0x27, 0x92, 0xca
	};


typedef struct {
	uint8 *ek;
	uint8 *pn;
	uint16 il;
	uint8 *input;
	uint8 *ref;
} sms4_ofb_vector_t;

#define SMS4_OFB_VECTOR_ENTRY(x)    \
	{ ofbek_##x, ofbpn_##x, sizeof(ofbinput_##x), ofbinput_##x, ofbref_##x }

sms4_ofb_vector_t sms4_ofb_vec[] = {
    };

#define NUM_SMS4_OFB_VECTORS  (sizeof(sms4_ofb_vec)/sizeof(sms4_ofb_vec[0]))

/* Locally generated packet vectors for SMS4-WPI */
uint8 wpipek_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 wpipick_00[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

/* Buffer must be big enough to hold MAC */
uint8 wpipinput_00[] = {
	0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
	};

uint8 wpipref_00[] = {
	0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x9f, 0x1f, 0x7b, 0xff, 0x6f, 0x55,
	0x11, 0x38, 0x4d, 0x94, 0x30, 0x53, 0x1e, 0x53,
	0x8f, 0xd3, 0x27, 0x8a, 0xe4, 0x93, 0xfa, 0x4f,
	0xc7, 0x34, 0x83, 0x3a, 0x28, 0xd2, 0xba, 0x57,
	0xf0, 0xc1
	};


/* Locally generated timing packet vectors for SMS4-WPI */
uint8 wpipek_03[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x05, 0xdc
	};

uint8 wpipick_03[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x05, 0xdc
	};

/* Buffer must be big enough to hold MAC */
/* MTU frame, non-qos: MAC hdr 24, WAPI IV 18, Data 1500, WAPI MIC 16 = 1558 */
uint8 wpipinput_03[] = {
	0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

uint8 wpipref_03[] = {
	0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x97, 0xf6, 0xf0, 0xa8, 0xb0, 0x72,
	0x8a, 0xbb, 0xbf, 0x50, 0x53, 0x83, 0xff, 0xf2,
	0xb4, 0xe2, 0x53, 0xf6, 0x8f, 0xb2, 0xcb, 0x93,
	0x02, 0xda, 0x60, 0xec, 0xa8, 0x11, 0xdc, 0xe7,
	0xf0, 0xfa, 0x10, 0x3d, 0x07, 0x19, 0x44, 0x69,
	0x1a, 0x55, 0xdb, 0x54, 0x52, 0x2e, 0x66, 0x8b,
	0xb1, 0xe9, 0xac, 0x31, 0x88, 0x60, 0xa4, 0x3f,
	0x3a, 0x8c, 0x7f, 0x73, 0x83, 0x00, 0xa4, 0x3a,
	0x29, 0x08, 0x84, 0x0e, 0x88, 0x8d, 0x84, 0x1d,
	0xe0, 0x0b, 0x16, 0x59, 0x8a, 0x43, 0xc6, 0xbb,
	0x28, 0x9a, 0x99, 0xb3, 0x5e, 0x90, 0xe2, 0x5a,
	0x17, 0xd0, 0xe3, 0x8b, 0xcb, 0x12, 0xee, 0xb3,
	0x87, 0x87, 0x3d, 0xd6, 0xd7, 0xd1, 0x67, 0xff,
	0xa1, 0x7a, 0x01, 0x52, 0xe3, 0x48, 0x27, 0x5b,
	0x02, 0xb6, 0xdb, 0xbb, 0x4b, 0x33, 0x3e, 0xe9,
	0x18, 0x42, 0xd2, 0xef, 0x13, 0x26, 0xd3, 0xd7,
	0xbf, 0xdf, 0xed, 0xae, 0x03, 0xfa, 0xed, 0x58,
	0xe5, 0x80, 0x1a, 0xfc, 0xb3, 0x81, 0x56, 0x4d,
	0x34, 0xc0, 0xdd, 0x29, 0x14, 0x0c, 0xfa, 0x23,
	0x44, 0xcc, 0x72, 0xf0, 0x7d, 0x35, 0xf4, 0x80,
	0x38, 0xbd, 0x78, 0xf6, 0x51, 0xed, 0x45, 0xc1,
	0xc0, 0xb5, 0x44, 0x0f, 0xec, 0x52, 0xcb, 0x69,
	0xe5, 0x4a, 0xd6, 0x3e, 0xa7, 0x50, 0x0c, 0xc6,
	0x86, 0x69, 0xbd, 0x66, 0xef, 0x15, 0x7f, 0x2e,
	0x9c, 0xcc, 0xcc, 0xc4, 0xb4, 0xc0, 0xa2, 0xde,
	0xa7, 0x6e, 0x37, 0x51, 0x7d, 0x17, 0x98, 0xfd,
	0x05, 0xb5, 0x0a, 0x1d, 0xf4, 0x87, 0xcd, 0x7b,
	0xac, 0xa1, 0xef, 0x57, 0x13, 0x14, 0x41, 0x7a,
	0x77, 0x88, 0x29, 0x97, 0xf5, 0xd8, 0x87, 0x37,
	0x6e, 0xbc, 0xb5, 0x54, 0x5d, 0x70, 0xd1, 0x59,
	0xe0, 0xbf, 0x4c, 0xca, 0x66, 0x06, 0xca, 0x9d,
	0xa9, 0xfa, 0x14, 0xa8, 0x9d, 0x80, 0x04, 0xd4,
	0x94, 0x7e, 0x23, 0x2d, 0x91, 0xaa, 0x87, 0xfc,
	0xc2, 0xf7, 0x16, 0xcb, 0xc2, 0xcc, 0xef, 0x51,
	0xe6, 0x5d, 0x0d, 0x61, 0x14, 0x2b, 0x2e, 0x3a,
	0xf2, 0xc2, 0x65, 0x20, 0x5f, 0x46, 0xdd, 0x68,
	0x37, 0x50, 0xd4, 0xd9, 0xac, 0xf6, 0x6d, 0xa1,
	0x73, 0x9d, 0x9e, 0x90, 0x34, 0x62, 0xe1, 0xe6,
	0x4a, 0xbb, 0xff, 0x50, 0xf7, 0x9f, 0x63, 0xc4,
	0xc8, 0xdf, 0xed, 0x6c, 0x60, 0x89, 0x38, 0xbc,
	0x2b, 0x7b, 0xd9, 0x9c, 0xb1, 0xa8, 0x64, 0x01,
	0xa4, 0x2c, 0x15, 0x7a, 0x0b, 0x3e, 0xaf, 0xab,
	0x8d, 0x1e, 0xb5, 0x14, 0xac, 0x52, 0xdf, 0x47,
	0x1e, 0x09, 0x93, 0x93, 0x96, 0xe0, 0x25, 0x0e,
	0x54, 0x21, 0xc7, 0xcf, 0xc0, 0x27, 0x51, 0x25,
	0x5c, 0xbb, 0xed, 0x7e, 0x13, 0xfc, 0x5e, 0xdd,
	0xfe, 0xe0, 0x06, 0x71, 0x2e, 0x27, 0xd5, 0x17,
	0x91, 0xd7, 0xa3, 0xef, 0xe9, 0x12, 0xed, 0x1c,
	0x1c, 0x3d, 0x33, 0x64, 0x3e, 0x49, 0xe5, 0xd1,
	0xb4, 0x50, 0x54, 0x7e, 0x67, 0xae, 0x79, 0x01,
	0xf7, 0xb3, 0xcb, 0xb3, 0xba, 0xa5, 0x3d, 0x60,
	0xea, 0xc8, 0x10, 0xff, 0x87, 0x01, 0x14, 0x44,
	0x1d, 0xd5, 0x51, 0x99, 0x1b, 0x79, 0xfe, 0x1b,
	0xff, 0xa2, 0xb4, 0x31, 0xc2, 0x8d, 0x0c, 0xa5,
	0x0a, 0x59, 0xfb, 0xd2, 0x1a, 0xef, 0x0e, 0xc7,
	0x21, 0x84, 0xa4, 0xe8, 0x16, 0x2d, 0x40, 0x9a,
	0x5c, 0x1a, 0xa8, 0x92, 0x70, 0x16, 0xc1, 0xf1,
	0xd5, 0xb5, 0x97, 0x11, 0x82, 0x5c, 0x54, 0x52,
	0xd5, 0x7d, 0xd8, 0x37, 0x74, 0x99, 0x29, 0xde,
	0x6f, 0x88, 0x10, 0xbe, 0x37, 0x8d, 0x83, 0x5e,
	0x90, 0x67, 0xa0, 0x34, 0x78, 0xe2, 0x47, 0x40,
	0xd6, 0x58, 0x4f, 0xec, 0x06, 0x9d, 0x30, 0xa0,
	0x61, 0xce, 0x1f, 0xa6, 0xe9, 0x23, 0x56, 0x5d,
	0xe0, 0xe4, 0x80, 0x11, 0x54, 0x9e, 0x94, 0xaa,
	0xbf, 0xc7, 0x46, 0xfd, 0x40, 0xc3, 0xe9, 0x83,
	0x7a, 0xa5, 0x43, 0x11, 0x58, 0xa3, 0xb9, 0x4d,
	0xa7, 0x09, 0xcd, 0x79, 0x27, 0x8e, 0xc0, 0xfb,
	0xab, 0x92, 0xa0, 0x29, 0x99, 0xb5, 0x0a, 0x72,
	0xaf, 0x92, 0x8b, 0xa7, 0xa0, 0xff, 0xb1, 0x42,
	0x0f, 0xd0, 0x63, 0x9c, 0x3e, 0x52, 0x8a, 0x83,
	0x59, 0x0f, 0x32, 0xd1, 0x83, 0x1c, 0xdb, 0xad,
	0xf7, 0x21, 0xfd, 0x75, 0x32, 0x51, 0x92, 0xaf,
	0x4a, 0x58, 0x1d, 0x85, 0x8a, 0x37, 0xb8, 0xd5,
	0xfc, 0xa2, 0xb5, 0x98, 0xb0, 0xfe, 0x18, 0x55,
	0xc9, 0x89, 0x61, 0xb1, 0xf0, 0xb3, 0xf9, 0x0f,
	0x2c, 0x99, 0x14, 0x2c, 0x30, 0xa4, 0x60, 0x68,
	0x3f, 0x94, 0x38, 0x58, 0x6b, 0xb5, 0xaf, 0x89,
	0x45, 0xa2, 0xab, 0xb1, 0x91, 0x78, 0xe6, 0x51,
	0x62, 0xb2, 0xe7, 0xa5, 0x6e, 0x14, 0xe1, 0x80,
	0xc5, 0x82, 0x05, 0x99, 0xca, 0xf8, 0x8f, 0x44,
	0xa2, 0xab, 0xbb, 0xeb, 0xe8, 0x41, 0xb3, 0x84,
	0xa1, 0x21, 0x2c, 0xc1, 0xe4, 0xc1, 0x2a, 0x4c,
	0x25, 0x67, 0xb4, 0x34, 0xe2, 0xfc, 0x77, 0x8c,
	0xdb, 0xdd, 0xf9, 0x36, 0xe4, 0x05, 0xfd, 0x02,
	0x23, 0x9b, 0x69, 0x40, 0x31, 0x8c, 0xab, 0x12,
	0xa4, 0x11, 0xd6, 0xca, 0xfd, 0x79, 0x1b, 0x17,
	0x59, 0xc7, 0xe4, 0x4c, 0xf6, 0x1b, 0x1a, 0x95,
	0x3e, 0x10, 0x71, 0x29, 0x8e, 0xe6, 0x08, 0x19,
	0xfd, 0x5f, 0x13, 0xbb, 0xc2, 0x39, 0xf5, 0x7a,
	0x12, 0xc2, 0x10, 0x9b, 0x7e, 0x2c, 0x91, 0x61,
	0x27, 0x24, 0x48, 0xc9, 0x05, 0xbb, 0x29, 0x23,
	0x82, 0x13, 0xd1, 0xe1, 0xff, 0x4e, 0x3d, 0xe8,
	0xf3, 0x03, 0xb2, 0x36, 0x3f, 0x62, 0xc3, 0xed,
	0xe8, 0x55, 0x5e, 0x50, 0x62, 0x1b, 0x6c, 0xb2,
	0x37, 0xe3, 0x82, 0xb6, 0x5f, 0xa7, 0x0a, 0x8d,
	0xcb, 0xc8, 0x51, 0xe6, 0x8c, 0x24, 0xc2, 0x02,
	0x49, 0x7b, 0x52, 0x59, 0x34, 0x69, 0x21, 0xba,
	0x98, 0xd3, 0x55, 0x62, 0x14, 0xed, 0x2b, 0xe1,
	0x23, 0xd0, 0x1f, 0xe5, 0xaf, 0xf3, 0x01, 0x23,
	0x19, 0x9d, 0x4c, 0xcb, 0x06, 0x3f, 0x60, 0xde,
	0xd2, 0xe2, 0xdc, 0xd2, 0x8f, 0x2d, 0x77, 0xab,
	0x03, 0x19, 0xb8, 0x5a, 0x75, 0x30, 0xa6, 0xe1,
	0x9e, 0xc0, 0xde, 0x5b, 0x6c, 0xfc, 0xd2, 0xae,
	0xec, 0x47, 0x13, 0x17, 0x88, 0xdf, 0x35, 0x9e,
	0x13, 0x89, 0x25, 0x11, 0x18, 0x51, 0x35, 0xae,
	0xd4, 0x27, 0xa8, 0xd0, 0x73, 0x2b, 0x00, 0x5c,
	0xfe, 0x93, 0x9d, 0x16, 0x78, 0x4a, 0x51, 0xd1,
	0x38, 0x6b, 0x1b, 0xb1, 0xc5, 0x83, 0x97, 0xdf,
	0xaf, 0x67, 0xb2, 0x25, 0xd8, 0xfe, 0xc5, 0x7f,
	0x79, 0xfa, 0x2f, 0x52, 0xfd, 0x42, 0xd9, 0xfa,
	0x2a, 0x9c, 0xa6, 0x18, 0x96, 0x3a, 0x7c, 0x8f,
	0x8b, 0xcc, 0x56, 0x98, 0x40, 0xaa, 0x9f, 0xaa,
	0x90, 0xe6, 0x94, 0xd6, 0xaf, 0x65, 0xe7, 0x69,
	0x7f, 0xef, 0x64, 0x00, 0x89, 0xe2, 0x74, 0x81,
	0x71, 0x28, 0x8e, 0x54, 0x28, 0x2d, 0x08, 0x87,
	0x49, 0x6b, 0x22, 0xe3, 0xa1, 0xbf, 0x96, 0x0e,
	0xc1, 0xdc, 0x16, 0x58, 0xad, 0x09, 0x7a, 0xb4,
	0xdf, 0x99, 0xfd, 0x14, 0x21, 0xe0, 0x26, 0x27,
	0x22, 0x9d, 0x0e, 0x1a, 0xb3, 0xdf, 0x22, 0x41,
	0x22, 0x4f, 0x35, 0xcb, 0x84, 0xf5, 0x46, 0x5c,
	0x4c, 0x67, 0x16, 0x8c, 0x46, 0xb7, 0x9f, 0x46,
	0xe2, 0x5f, 0x16, 0x98, 0x4d, 0x02, 0xf3, 0x12,
	0xa7, 0x4b, 0x3d, 0x19, 0x4c, 0x33, 0xb8, 0xba,
	0x78, 0x9d, 0xd7, 0xb3, 0x8f, 0x27, 0x99, 0x1b,
	0xa2, 0xb5, 0x56, 0xae, 0x2e, 0xd2, 0xb0, 0x51,
	0x20, 0x3a, 0xf5, 0x84, 0x60, 0x82, 0x8a, 0xf0,
	0x08, 0x5e, 0xcf, 0x8a, 0x31, 0x0a, 0x9c, 0x21,
	0x03, 0xcc, 0x86, 0xdc, 0xf5, 0x1d, 0x9c, 0x7b,
	0x57, 0xc4, 0x77, 0x28, 0xdb, 0xf4, 0x16, 0xe0,
	0xe0, 0xfc, 0x35, 0x93, 0x77, 0x62, 0xf2, 0xb6,
	0xe5, 0xdf, 0xf2, 0x81, 0x6d, 0x4f, 0xd4, 0xf7,
	0x5b, 0x12, 0x2d, 0xfd, 0xe8, 0xda, 0xc0, 0xb6,
	0x02, 0x79, 0x48, 0xae, 0x79, 0x0e, 0x6f, 0xed,
	0xd8, 0x57, 0xfc, 0x96, 0xdf, 0x08, 0x35, 0xb1,
	0xf7, 0x35, 0x6a, 0x98, 0x6d, 0x7f, 0x17, 0xcb,
	0xb5, 0x8f, 0x2f, 0x6b, 0x70, 0x54, 0xdd, 0xe1,
	0xe9, 0xab, 0xfc, 0xd6, 0xf8, 0xde, 0xb2, 0xcb,
	0xe6, 0xbf, 0xae, 0xb0, 0xa2, 0x39, 0xbd, 0x38,
	0xb2, 0x78, 0x24, 0xe7, 0x92, 0x28, 0xcf, 0x6b,
	0x85, 0x1a, 0x48, 0x1b, 0x06, 0x9f, 0x6e, 0xd3,
	0xe9, 0xee, 0x57, 0x57, 0x24, 0xd4, 0x11, 0x35,
	0x88, 0x63, 0x50, 0x82, 0x02, 0x5d, 0x17, 0x27,
	0x20, 0x73, 0xa1, 0x22, 0x95, 0x63, 0xa9, 0xf8,
	0x9d, 0xa6, 0x20, 0x43, 0xc4, 0x6d, 0xac, 0x39,
	0x64, 0xe9, 0x5d, 0x89, 0xdf, 0xdf, 0x56, 0xfc,
	0xf4, 0xd0, 0x88, 0xa4, 0x09, 0x81, 0x79, 0x66,
	0xe6, 0xb6, 0x89, 0xb9, 0xb2, 0xa3, 0xe8, 0xc5,
	0x41, 0x3c, 0x5c, 0xe7, 0xfd, 0xf7, 0xc7, 0xd3,
	0x39, 0x1c, 0x45, 0xe6, 0xdd, 0x1a, 0x6d, 0x4c,
	0x1a, 0x32, 0xf8, 0x28, 0x61, 0xb2, 0x95, 0x3a,
	0x4c, 0xb1, 0x90, 0x34, 0x86, 0x78, 0xc1, 0xcf,
	0x8e, 0xbe, 0xe8, 0xec, 0xde, 0x40, 0x20, 0x45,
	0x00, 0x9d, 0x3b, 0xec, 0xc5, 0x65, 0xb2, 0x73,
	0x26, 0x4b, 0x94, 0x22, 0x3b, 0x0d, 0x70, 0x64,
	0x8d, 0xe3, 0x8c, 0x49, 0xf4, 0x96, 0xf4, 0xbb,
	0xd8, 0x82, 0x0e, 0x29, 0x3a, 0xd3, 0x72, 0x93,
	0x02, 0x6e, 0x79, 0x7c, 0x0c, 0xa6, 0xcf, 0xef,
	0x8a, 0x73, 0x9e, 0x1a, 0x1d, 0xac, 0xfd, 0xa7,
	0xa1, 0xc3, 0x89, 0x44, 0x4e, 0xc6, 0x00, 0xaa,
	0x4a, 0x82, 0x87, 0xa0, 0x1f, 0xd7, 0x3c, 0x8c,
	0x7f, 0x3e, 0xa3, 0x03, 0x34, 0x89, 0xfe, 0x1f,
	0x8c, 0xbe, 0xf7, 0x57, 0x0c, 0x96, 0xb0, 0x62,
	0x02, 0x66, 0x18, 0x2d, 0xab, 0x60, 0x22, 0xa8,
	0x6c, 0x32, 0x5e, 0x6f, 0x23, 0xc2, 0x26, 0x39,
	0xe3, 0xc5, 0x54, 0x3b, 0x76, 0x73, 0x4a, 0x94,
	0x49, 0x3a, 0x2a, 0x47, 0x72, 0x66, 0x8a, 0xdc,
	0xb8, 0x0d, 0x72, 0x5e, 0xe9, 0x70, 0xc2, 0xda,
	0x60, 0x29, 0x9d, 0xf6, 0x7f, 0x25, 0x39, 0x1e,
	0x0d, 0x93, 0xc8, 0x99, 0x42, 0x72, 0x4c, 0x1b,
	0x8a, 0x17, 0xa0, 0x46, 0xe3, 0x33, 0x38, 0xd6,
	0xc5, 0x6f, 0xe6, 0x1b, 0x48, 0x02, 0x20, 0x0b,
	0x4b, 0x60, 0x72, 0xac, 0x13, 0x94, 0x4a, 0x7c,
	0xa4, 0x36, 0x99, 0x56, 0x67, 0xd6, 0xe3, 0x71,
	0xf5, 0xb9, 0x7f, 0x47, 0xeb, 0x39, 0x33, 0x83,
	0x44, 0x00, 0xc3, 0x06, 0x46, 0xea, 0x6a, 0xa7,
	0x1b, 0x0a, 0xf7, 0x60, 0xcb, 0x1e, 0x28, 0x9b,
	0xa4, 0x0a, 0x0c, 0x71, 0x86, 0xb7, 0xd3, 0xb3,
	0x50, 0xc1, 0x34, 0x9f, 0xb2, 0x29, 0x0b, 0xec,
	0xf2, 0x77, 0x80, 0x0e, 0x6f, 0x22, 0x8e, 0x1b,
	0xb3, 0x51, 0xf9, 0x96, 0xc7, 0x7a, 0x33, 0x1e,
	0x33, 0x8f, 0x79, 0x4b, 0xde, 0x4f, 0xb6, 0xb8,
	0x4f, 0xd3, 0xaf, 0x4f, 0x90, 0x89, 0x0e, 0x7f,
	0x79, 0x30, 0xd0, 0x3b, 0x7a, 0x5c, 0x29, 0xe9,
	0x4d, 0x84, 0x24, 0x37, 0xe1, 0x4b, 0x64, 0xe3,
	0x4b, 0xfd, 0xe5, 0xeb, 0x24, 0xc6, 0x01, 0xbc,
	0xc9, 0x80, 0x89, 0xea, 0xdd, 0xc2, 0x26, 0x45,
	0x9b, 0xe2, 0x00, 0x85, 0xa2, 0x64, 0x7f, 0x1d,
	0xf9, 0x44, 0x34, 0x29, 0xb1, 0xbe, 0xa3, 0xe0,
	0x97, 0x7a, 0xd1, 0x2d, 0xc7, 0x14, 0xeb, 0xff,
	0xed, 0xff, 0xd1, 0xd8, 0xc5, 0xc7
	};

typedef struct {
	uint8 *ek;
	uint8 *ick;
	uint16 il;	/* length of packet with MAC */
	uint8 *input;
	uint8 *ref;
} sms4_wpi_pkt_vector_t;

#define SMS4_WPI_PKT_VECTOR_ENTRY(x)    \
	{ wpipek_##x, wpipick_##x, sizeof(wpipinput_##x), \
		wpipinput_##x, wpipref_##x }

sms4_wpi_pkt_vector_t sms4_wpi_pkt_vec[] = {
    SMS4_WPI_PKT_VECTOR_ENTRY(00),
    SMS4_WPI_PKT_VECTOR_ENTRY(03)
    };

#define NUM_SMS4_WPI_PKT_VECTORS  (sizeof(sms4_wpi_pkt_vec)/sizeof(sms4_wpi_pkt_vec[0]))
sms4_wpi_pkt_vector_t sms4_wpi_tpkt_vec[] = {
    SMS4_WPI_PKT_VECTOR_ENTRY(03)
    };

#define NUM_SMS4_WPI_TIMING_VECTORS  (sizeof(sms4_wpi_tpkt_vec)/sizeof(sms4_wpi_tpkt_vec[0]))

#endif /* BCMSMS4_TEST */
