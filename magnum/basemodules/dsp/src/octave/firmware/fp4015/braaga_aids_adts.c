/*******************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 ******************************************************************************/

#include "bchp.h"

const uint32_t BDSP_IMG_aids_adts_array1[] = {0x464c457f,
0x10101,0,0,0x4e0003,1,0,0x34,0x1d10,0xd4050000,0x200034,0x280006,0xf0012,1,0x200,0,0,
0x9a8,0x9a8,5,0x200,1,0xba8,0x9a8,0x9a8,0x4ea,0x4ea,0x100006,4,1,0x1094,0xe94,0xe94,
0x5a4,0x5a4,0x100004,4,1,0x1638,0x1438,0x1438,4,4,4,4,1,0x1640,0x1440,0x1440,
0x201,0x201,0x10000006,8,2,0xba8,0x9a8,0x9a8,0x138,0x138,0x100006,4,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x3a3f9b71,
0x3f59f208,0xa7c48e70,0x2cc10e4a,0x3fd85c09,0xa7d01e40,0x27d02e48,0x2c4e0fc1,0xe7c81e48,0x27ccae50,0x27cc0e78,0x27c8ae40,0xfc000475,0x22b00280,0x27c49e78,0x27cb9e50,0xbc000473,
0x22b14fc0,0x27c3ee68,0xe2b01fc3,0x27cc8e60,0x23912fc2,0x22b00fc1,0xbc00040f,0x22b00200,0x27c03e68,0xda4e0e7e,0x1a500e7f,0xe7c3efc0,0x22b0cfc0,0x7c000457,0x27c3ee68,0xe2b02ffe,
0x27cc8e68,0x22b00200,0xa7c3ee68,0x3c00008f,0xda4e8e7e,0x1a508e7f,0x22b18fc0,0xe7c3efc0,0x7c000441,0x27c3ee68,0xec400fc2,0x2ffff000,0x22b03fc7,0x27cc8e70,0xbc000409,0x3da42200,
0x27c07e68,0xda4f0e7e,0x1a4f8e7f,0x2cc10e40,0xe7c3efc0,0x3c00042d,0x383f9b71,0x9b430e7e,0x1a428e4e,0xa78fee50,0x3face00c,0x2d5be38f,0x3facf020,0x60100013,0x328a03c2,0x3c00041f,
0xbc000423,0x1a4e0e40,0x22b14fc1,0xbc000423,0x27c3ee68,0x1a4d0e40,0xbc000417,0x1a4f0e40,0x22b18fc1,0xbc000417,0x27c3ee68,0x1a4d0e40,0xbc00040b,0x1a4e8e40,0x22b0cfc1,0xbc00040b,
0x27c3ee68,0x1a4d0e40,0x3fa3a000,0xe2b01fca,0x22b00fc8,0xa7c0a040,0x1a468089,0x3fac9020,0xda40808c,0x1a40800f,0x1a41000b,0x1a470081,0xdb42008e,0x1a40008d,0x2cc012c3,0x27c8a078,
0xed58c3c2,0x27cff068,0x278ce048,0x27ccd060,0xa7cff058,0x27c02008,0x27c03010,0x3fa3a000,0x7c0003d2,0x22b0cfc1,0xbf59ec3f,0x6cc08fc8,0x2783f008,0x7fa3a000,0x27c08000,0x7fa3a000,
0x22b01fc8,0x7fa3a000,0x1a420008,0x7c0003c8,0x1a408000,0x3a039a01,0xbc0003c3,0x22b00020,0x1a408000,0x62b14fc1,0x22b00800,0x37839a01,0x3c0003aa,0xf0bb9e45,0x27afae70,0x1f5bf002,
0x1f5bf043,0xed0020d1,0x2cc90e52,0x2c4e0fc1,0x2f000001,0xfda7ef82,0x2390ffd8,0x2c4d0fc0,0x2f000002,0xec415fc8,0x2f000004,0x2c480fc9,0x2f000007,0xec454fd0,0x2f000010,0x2c40afd4,
0x2f000002,0xec4c0fd5,0x2f000003,0x2c4a0fd6,0x2f000005,0xec42afd7,0x2f000008,0x2c480fd9,0x2f000016,0xec440fca,0x2f00000b,0x2ca2a20e,0x2f000008,0xeca0024c,0x2f00000f,0x2caa8408,
0x2f000020,0xecac0044,0x2f000003,0x2caa0006,0x2f000005,0xeca8028a,0x2f000016,0x2d051493,0x28382e41,0xeca15505,0x2f000004,0x2ca80547,0x2f000007,0xeca4058f,0x2f00000b,0x2ca545cd,
0x2f000010,0xeca0060b,0x2f00001e,0x2ca00649,0x2f00002d,0xffda0000,0x28084e41,0x3fda0000,0x28106e41,0xffda0000,0x2818ee41,0x3fda0000,0x2820ce41,0xffda0000,0x2828ae41,0x3fda0000,
0x28308e41,0xffa3a000,0x28402e41,0x2cc90e79,0x1a7004c8,0x3a6f9a0e,0xda400048,0x279b7e70,0x279f7e40,0x2cc90e77,0xe2b00ded,0x3da40062,0x22b00125,0x22b00fc3,0xf0c3fb47,0x22b39fc1,
0x2c499fe4,0x2fef46ba,0xda41821f,0x22b00b42,0x22b00880,0x2a820928,0xbfa5fe80,0x22b00e61,0x22f07fe7,0xa7c3f8e0,0x3fda0000,0x3fd87580,0x7c0002fd,0x1a700de9,0xecc1623e,0x2cc02226,
0x2cc06209,0x62507f83,0xec4b2fc0,0x2ff00000,0x2cc180c5,0x2cc0926a,0xed100ffe,0x2d5c5e79,0x2cd01982,0x7d4e8f84,0xa7804e62,0x27027e60,0x3fd87581,0xfc0002e1,0x2cc1ae40,0x2cc19e6b,
0x2703fe72,0xecc10e60,0x2d5a6ac6,0x22b00a82,0x22b01fc0,0xbc0002d7,0x27c29180,0x22b00801,0xe01000cf,0x1a700dfe,0x22b00879,0x76041fb1,0xda400891,0x2cd70dea,0x3da7ffc4,0x22b38fc3,
0xe2b00a82,0x22b39fc1,0x22b00880,0x1a41045f,0x3fa5fe80,0x3f908171,0xbf800353,0x22b00a9d,0x2cd36dd9,0xdb80077e,0x1b80275c,0x2cc0275d,0x6a008f9e,0x7d4de73e,0xb003e653,0xbfd87580,
0x1ab98deb,0x2a820920,0x3c000291,0xecc1622c,0x2cc02228,0x2cc06227,0x22b00e66,0xe2507b09,0x22f07fe9,0x2c46efc8,0x2ff00000,0xecc18240,0x2d108fca,0x2cd01a02,0x2cc099ea,0xed5c0e79,
0x3d4e02be,0x67029e60,0xe783ee62,0xbfd87581,0x2cc1ae40,0x2cc19e6c,0x7c00026f,0x2703fe72,0xed5a8b03,0x2cc10e67,0x22b00a82,0x22b01fc0,0xbc000269,0x27c2b0c0,0x22b009c1,0xa2b009b9,
0x1ab9cdea,0x3fd87580,0x3c000251,0xecc16205,0x2cc02228,0x2c44efc6,0x2ff00000,0xe250714b,0x2d106fc7,0x2cc06204,0x2cd01a02,0xecc182cd,0x3d4e01cc,0x2cc0912c,0x22b00aa7,0xed5cde79,
0x67029e60,0x6780ce62,0x6cc1ae40,0xbfd87581,0x2cc19e69,0x2703fe72,0x3c00022f,0xecc10e60,0x2d5a8a4e,0x22b00b02,0x22b01fc0,0xbc00022b,0x27c2a380,0x22b00801,0xdaba0dd0,0x22803abe,
0x22b009b9,0x6a00bf8f,0xaa60543e,0x7d4cff92,0x6d4be493,0xe0140011,0x3fda0000,0x362274fc,0x2f17fe17,0xe01800fe,0x2a10aa94,0x6200f520,0x76420816,0x7c00003e,0x27c7fdf8,0xbc0001f7,
0x3fda0000,0x3fd87580,0xecc12207,0x2cc0220d,0x2c42afcb,0x2ff00000,0xe25071ce,0x2d10bfcc,0x2cc0b222,0x2cd01342,0xecc183be,0x3d4e833f,0xad5fee79,0xe783fe62,0xa7027e60,0x2cc1ae40,
0x3fd87581,0xbc0001d9,0x2cc10e60,0x2703fe72,0xfc0001d9,0x22b00882,0x22b01fc0,0x22b00801,0x22b00879,0xa2b01ffe,0x27c7fdf8,0xa7c3e8e0,0xbc0001bd,0x3fda0000,0x3fd87580,0xecc12204,
0x2a820903,0x22f07fcd,0x22b00e62,0xe2507107,0x2cc02206,0x2c402fe4,0x2ff00000,0xed124fc5,0x2cc181fe,0x2cd01182,0xad5fee79,0xfd4c314b,0x2700de60,0x2cc0b224,0x2cc1ae40,0xa780be62,
0x2cc10e60,0x3fd87581,0x7c000197,0x2703fe72,0xfc000199,0x22b00902,0x22b01fc0,0x22b00801,0xecc18dc2,0x2783fdd0,0x2783fde0,0x2cc08dce,0x7007f084,0x22b008b9,0x3007f084,0x3007f084,
0x3007f084,0xb007f084,0x6783f080,0x1a4088cf,0xdb410dd2,0x22b00950,0x27c8fdc0,0x2cc10391,0xe2b01fc8,0x30052404,0x1b410393,0x6cc08414,0xa7813950,0x1c01047e,0xb007e504,0x5c01047e,
0xb007e504,0x5c01047e,0xb007e504,0x5c01047e,0xb007e504,0xdc01047e,0xb007e504,0x5b40047f,0xe783f500,0x62b00879,0x5b5b0e77,0x1b5c0e77,0x386f9a0e,0xbc00013b,0x3fda0000,0x3fd87580,
0xecc12212,0x2cc02215,0x2c48efd3,0x2ff00000,0xe2507496,0x2d113fd4,0x2cc0b229,0x2cd01542,0xecc185be,0x3d4e853f,0xad5fee79,0xe783fe62,0xa7027e60,0x2cc10e66,0x3fd87581,0xbc00011d,
0x2cc1ae40,0x2703fe72,0xfc00011d,0x22b00a42,0x22b01fc0,0x22b00981,0x7c3ffe60,0x22b00879,0xda400895,0x1f5bf826,0x2c480fd8,0x2f00003e,0xec411fdb,0x2f00002b,0x2cae060a,0x2f00002e,
0xeca406cb,0x2f00001f,0x2c400fd6,0x2f000177,0xe39fafd9,0x2810ade1,0x2c444fd7,0x2f0000ac,0xfda7ef8a,0x2d066b5c,0x2c4c0fda,0x2f00005d,0xeca8064f,0x2f0000bb,0x2ca8858e,0x2f000158,
0xeca005cc,0x2f00007d,0x2ca2268d,0x2f000056,0xea203add,0x2800ede1,0x1a43055f,0x1a4088c8,0xdf5bf4a7,0x2808cde1,0x23904fc9,0x2818ade1,0xda51070a,0x2280175e,0x22b00fc5,0x22b01fc4,
0xecd78dc3,0x22b00fc2,0x22b39fc1,0x22b00880,0xe7d9e200,0x27d7f238,0x27d0a230,0x27c0ade0,0xffa5fe80,0x27c09dd8,0x27c49dc0,0x27c27dd0,0xdb710dfe,0x22300fc1,0x22300800,0xa783edf0,
0x3c3ffcdb,0xda4088cf,0x1a700dfe,0x27c48dc8,0xb8e3e9d1,0xe1effec8,0x27da63c8,0x27c8fdc0,0x27cbfdc8,0xfc3fff16,0x22b01ffe,0x2cc08dce,0xa7c7edf8,0x3a039a01,0xbf59d03f,0x6cc08fc8,
0x22b00020,0xe7c01010,0x27c02018,0x27c3f020,0x27c08800,0x7c00007d,0x22bc8fc0,0x27c08808,0x38039a01,0x3fa3a000,0xffa3a000,0x2901103e,0x3fda0000,0xa803e041,0x7fa3a000,0x1a428008,
0x7c000042,0x22b18fc1,0xbf59b83f,0x6cc08fc8,0x22b01fc9,0xe7c01008,0x27c02018,0x27c09028,0x27c08000,0x3fa3a000,0xbc000058,0x3fda0000,0x3fd84a01,0x7fa3a000,0x22b00fc8,0x7fa3a000,
0x2783f008,0x7fa3a000,0x1a410008,0x7fa3a000,0x1a418008,0x7f59523d,0x2fffffff,0x3fa3d000,0x7f59423d,0x2fffffff,0x3fa3d000,0x7f593c3d,0x2fffffff,0x3fa3d000,0x7f59543d,0x2fffffff,
0x3fa3d000,0x7f595c3d,0x2fffffff,0x3fa3d000,0x7f59603d,0x2fffffff,0x3fa3d000,0x7f597e3d,0x2fffffff,0x3fa3d000,0x7f597a3d,0x2fffffff,0x3fa3d000,0x7f59463d,0x2fffffff,0x3fa3d000,
0x7f59403d,0x2fffffff,0x3fa3d000,0x7f59483d,0x2fffffff,0x3fa3d000,0x7f594c3d,0x2fffffff,0x3fa3d000,1,0x172,1,0x189,1,0x19a,1,
0x1ac,1,0x1b9,1,0x1c9,1,0x1dd,1,0x1f0,0xe,0xbc,0x10,0,4,0xe94,5,
0xc90,6,0xae0,0xa,0x202,0xb,0x10,3,0x1440,0x70000004,4,0x70000005,0x90,7,0x1030,8,
0x408,9,0xc,0x70000000,0x14d0,0x70000001,0x114c,0x70000002,4,0x70000003,8,0x70000006,0xe90,0x70000007,0x1230,0x70000008,
0x1154,0x70000009,0x161c,0x7000000a,4,0x7000000b,0x20,0x7000000c,0xef46ba99,0x7000000d,0x8ec,0x6ffffff9,0x4a,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0x1f0,0,0,4,0x1dd,
0,0,4,0x1c9,0,0,4,0x1b9,0,0,4,0x189,0,0,4,0x1ac,
0,0,4,0x19a,0,0,4,0x172,0,0,4,0xbc,0,0,4,1,
0,0x14c,0x10012,0x151,0x8ec,0xc,0x10012,0x82,0,0,0x12,0x131,0,0,0x12,0x7c,
0,0,0x12,0x12a,0,0,0x12,0x14a,0,0,0x12,0x15f,0,0,0x12,0xf5,
0,0,0x11,0x66,0,0,0x12,0x9a,0,0,0x12,0xa0,0,0,0x12,0xa8,
0,0,0x12,0xcf,0,0,0x11,0x50,0,0,0x12,0x123,0,0,0x12,0x11e,
0,0,0x12,0x325a5f00,0x74654732,0x6f676c41,0x63657053,0x63696669,0x49736449,0x506f666e,0x53633950,0x49636e79,0x506f666e,0x63393150,0x73726150,0x646e4165,
0x696c6156,0x65746164,0x50507550,0x41633331,0x6d756363,0x74616c75,0x755065,0x78675f5f,0x65705f78,0x6e6f7372,0x74696c61,0x6a735f79,0x555f0030,0x6e69776e,0x6a535f64,0x525f6a4c,
0x73696765,0x726574,0x776e5a5f,0x555f006d,0x6e69776e,0x6a535f64,0x555f6a4c,0x6765726e,0x65747369,0x62610072,0x74726f,0x6c645a5f,0x6d7650,0x776e555f,0x5f646e69,0x6a4c6a53,
0x7365525f,0x656d75,0x6162696c,0x5f736469,0x73746461,0x312e3040,0x5f00302e,0x4e56545a,0x5f5f3031,0x61787863,0x31766962,0x5f5f3731,0x73616c63,0x79745f73,0x695f6570,0x456f666e,
0x545a5f00,0x30314e56,0x78635f5f,0x69626178,0x30323176,0x69735f5f,0x616c635f,0x745f7373,0x5f657079,0x6f666e69,0x72660045,0x73006565,0x656c7274,0x656d006e,0x7970636d,0x61545f00,
0x74656772,0x6972505f,0x505f746e,0x746e6972,0x7361485f,0x616d0068,0x636f6c6c,0x735f5f00,0x65726168,0x6e695f64,0x5f007469,0x5f78745f,0x5f6f7364,0x74696e69,0x6e69665f,0x696c0069,
0x64747362,0x2d2b2b63,0x6d696c73,0x30373240,0x302e302e,0x62696c00,0x5f636367,0x37324073,0x2e302e30,0x696c0030,0x72687462,0x78646165,0x2e314070,0x302e30,0x6362696c,0x30373240,
0x302e302e,0x62696c00,0x6d6d6f63,0x30406e6f,0x302e312e,0x62696c00,0x6d6d6f63,0x695f6e6f,0x30407364,0x302e312e,0x62696c00,0x635f6d6d,0x6e65696c,0x2e304074,0x302e31,0x7362696c,
0x65676174,0x406f695f,0x2e312e30,0x30,0x11,0x1b,0,0x16,0,0xe,0x19,0,0x17,0x18,0x15,0,
0x14,0xb,0x1a,0,0,0xd,0x13,0,0,0,0,0,0,0,0,0,
0,0x11,0,0,0,0,0,0,0,0xc,0,0x10,0xf,0x12,0,0,
0,0xa,0,0x8ec,0,0x1f0,0x1dd,0x1c9,0x1b9,0x189,0x1ac,0x19a,0x172,0xbc,1,0x151,
0x82,0x131,0x7c,0x12a,0x14a,0x15f,0xf5,0x66,0x9a,0xa0,0xa8,0xcf,0x50,0x123,0x11e,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0x161c,0x121,0,0x1620,0x221,
2,0x1624,0x321,1,0x1628,0x421,0,0x1448,0x521,0x13,0x1474,0x521,0x10,0x1490,0x521,0x11,
0x145c,0x621,0x27f,0x1460,0x621,0x26e,0x1478,0x621,0x1c1,0x14c4,0x621,0x2d2,0x14cc,0x621,0x219,0x162c,
0x621,0x7d,0x1450,0x721,0x12a,0x1468,0x721,0x129,0x1630,0x721,0,0x1454,0x821,0x25,0x146c,0x821,
2,0x1488,0x821,0x22,0x1494,0x821,0,0x14b4,0x821,6,0x1634,0x821,0x27,0x1440,0x902,0x154c,
0x1444,0x902,0x8f8,0x144c,0x902,0x908,0x1458,0x902,0x15fb,0x1464,0x902,0x1538,0x1470,0x902,0x1514,0x147c,
0x902,0x1540,0x1480,0x902,0x910,0x1484,0x902,0x14d0,0x148c,0x902,0x15a2,0x1498,0x902,0x1500,0x149c,0x902,
0x15c6,0x14a0,0x902,0x15dc,0x14a4,0x902,0x900,0x14a8,0x902,0x14e4,0x14b0,0x902,0x1606,0x14b8,0x902,0x14d8,
0x14bc,0x902,0x1508,0x14c0,0x902,0x1592,0x14d4,0x902,0x14c0,0x14dc,0x902,0x148c,0x14e0,0x902,0x1484,0x14e8,
0x902,0x14b8,0x14ec,0x902,0x14c,0x14f0,0x902,0x1a4,0x14f4,0x902,0x150,0x14f8,0x902,0x1444,0x14fc,0x902,
0x14a4,0x1504,0x902,0x149c,0x150c,0x902,0x14a0,0x1510,0x902,0x1498,0x1518,0x902,0x14bc,0x151c,0x902,0x1d0,
0x1520,0x902,0x1d8,0x1524,0x902,0x318,0x1528,0x902,0x1c0,0x152c,0x902,0x144c,0x1530,0x902,0x1480,0x1534,
0x902,0x1c8,0x153c,0x902,0x1458,0x1544,0x902,0x14b0,0x1548,0x902,0x1464,0x1550,0x902,0x147c,0x1554,0x902,
0x8a8,0x1558,0x902,0x8c4,0x155c,0x902,0x8ac,0x1560,0x902,0x8bc,0x156c,0x902,0x161c,0x1578,0x902,0x161c,
0x157c,0x902,0x161c,0x1580,0x902,0x161c,0x1584,0x902,0x161c,0x14d0,0x91e,0x1494,0x14d0,0x925,8,0x14d8,
0x91e,0x146c,0x14d8,0x925,8,0x1500,0x91e,0x1494,0x1500,0x925,8,0x1508,0x91e,0x146c,0x1508,0x925,
8,0x1538,0x91e,0x1494,0x1538,0x925,8,0x1540,0x91e,0x146c,0x1540,0x925,8,0xef46ba99,0,0x154c,
0x8f8,0,0x908,0,0,0x15fb,0,0,0x1538,0,0,0x1514,0,0,0x1540,0x910,
0x14d0,0,0x15a2,0,0,0x1500,0x15c6,0x15dc,0x900,0x14e4,0,0x1606,0,0x14d8,0x1508,0x1592,
0,0,0,8,0x1592,8,0x15a2,0x14d0,0,0x14d8,0x14c,0x1a4,0x150,0x8f8,0x900,8,
0x15c6,8,0x15dc,0x1500,0,0x1508,0x1d0,0x1d8,0x318,0x1c0,0x908,0x910,0x1c8,8,0x15fb,8,
0x1606,0x1538,0,0x1540,0x8a8,0x8c4,0x8ac,0x8bc,0,0,0x161c,0,0,0x161c,0x161c,0x161c,
0x161c,0x601ffff,0x10000,0x33310002,0x63634163,0x6c756d75,0x50657461,0x32320075,0x63614163,0x64416568,0x63417374,0x756d7563,0x6574616c,0x56007550,0x64696c61,0x50657461,
0x39310075,0x72615063,0x6e416573,0x6c615664,0x74616469,0x755065,0x41633832,0x65686361,0x73746441,0x73726150,0x646e4165,0x696c6156,0x65746164,0x39007550,0x6e795363,0x666e4963,
0x3831006f,0x63614163,0x64416568,0x79537374,0x6e49636e,0x6f66,0,0,0,0,0,0,0,0,0,0,
0x464e490a,0x253a3a4f,0x39323a73,0x70203a30,0x6e695f75,0x69206f66,0x4f4e2073,0x61762054,0xa64696c,0,0x464e490a,0x253a3a4f,0x38323a73,0x63203a32,0x73206264,0x20657a69,
0x206f6f74,0x6c616d73,0xa6c,0x464e490a,0x253a3a4f,0x39313a73,0x63203a36,0x645f6264,0x5b617461,0x253d5d31,0xa78,0x464e490a,0x253a3a4f,0x39313a73,0x63203a35,0x645f6264,
0x5b617461,0x253d5d30,0xa78,0x464e490a,0x253a3a4f,0x38313a73,0x73203a37,0x65676174,0x206f6920,0x64616572,0x72726520,0xa726f,0x464e490a,0x253a3a4f,0x38313a73,0x63203a33,
0x69736264,0x253d657a,0xa64,0,0,0,0,0,0,0,0x10003,0,0x918,0,0x20003,0,
0x1438,0,0x30003,0,0x1440,0,0x40003,0,0x1641,0,0x50003,0,0x1641,0,0x60003,0,
0xf0000000,0,0x70003,0,0xae0,0,0x80003,0,0xc90,0,0x90003,0,0x9a8,0,0xa0003,0,
0xe94,0,0xb0003,0,0x1030,0,0xc0003,0,0xf4c,0,0xd0003,0,0xf54,0,0xe0003,1,
0,0,0x12,0x19,0,0,0x12,0x32,0,0,0x12,0x38,0,0,0x12,0x3f,
0,0,0x12,0x46,0x8ec,0xc,0x10012,0x54,0,0,0x12,0x67,0,0,0x11,0x90,
0,0,0x12,0xa6,0,0,0x12,0xac,0,0,0x12,0xb4,0,0,0x12,0xc8,
0,0,0x11,0xee,0,0x14c,0x10012,0x13d,0,0,0x12,0x153,0,0,0x12,0x15a,
0,0,0x12,0x6e555f00,0x646e6977,0x4c6a535f,0x6e555f6a,0x69676572,0x72657473,0x61545f00,0x74656772,0x6972505f,0x505f746e,0x746e6972,0x7361485f,0x5a5f0068,
0x6d776e,0x636d656d,0x6d007970,0x6f6c6c61,0x5f5f0063,0x72616873,0x695f6465,0x74696e,0x78745f5f,0x6f73645f,0x696e695f,0x69665f74,0x5f00696e,0x4e56545a,0x5f5f3031,0x61787863,
0x31766962,0x5f5f3032,0x635f6973,0x7373616c,0x7079745f,0x6e695f65,0x456f66,0x776e555f,0x5f646e69,0x6a4c6a53,0x6765525f,0x65747369,0x62610072,0x74726f,0x6c645a5f,0x6d7650,
0x776e555f,0x5f646e69,0x6a4c6a53,0x7365525f,0x656d75,0x56545a5f,0x5f30314e,0x7878635f,0x76696261,0x5f373131,0x616c635f,0x745f7373,0x5f657079,0x6f666e69,0x5a5f0045,0x65473232,
0x676c4174,0x6570536f,0x69666963,0x73644963,0x6f666e49,0x63395050,0x636e7953,0x6f666e49,0x39315050,0x72615063,0x6e416573,0x6c615664,0x74616469,0x50755065,0x63333150,0x75636341,
0x616c756d,0x75506574,0x675f5f00,0x705f7878,0x6f737265,0x696c616e,0x735f7974,0x7300306a,0x656c7274,0x7266006e,0x6565,0x6d79732e,0x626174,0x7274732e,0x626174,0x7368732e,
0x61747274,0x742e0062,0x747865,0x746c702e,0x65732e00,0x61685f67,0x2e006873,0x61746164,0x72702e00,0x7365636f,0x686b5f73,0x706165,0x6f72702e,0x73736563,0x7061635f,0x702e0073,
0x746e6972,0x69727453,0x73676e,0x6e79642e,0x6d7973,0x6e79642e,0x727473,0x6e79642e,0x63696d61,0x61682e00,0x2e006873,0x616c6572,0x6e79642e,0x70662e00,0x736e7964,0x2e006d79,
0x79647066,0x7274736e,0x66666f,0,0,0,0,0,0,0,0,0,0,0x1b,1,6,
0,0x200,0x918,0,0,0x200,0,0x21,1,6,0x918,0xb18,0x90,0,0,4,
0,0x26,1,2,0x1438,0x1638,4,0,0,4,0,0x30,1,0x400003,0x1440,0x1640,
0x201,0,0,8,0,0x36,1,0x10800001,0x1641,0x1841,0,0,0,1,0,0x45,
1,0x40800001,0x1641,0x1841,0,0,0,1,0,0x53,1,0,0xf0000000,0x1844,0xcb,0,
0,4,0,0x61,0xb,0x800002,0xae0,0xce0,0x1b0,9,0xa,4,0x10,0x69,3,0x800002,
0xc90,0xe90,0x202,0,0,1,0,0x71,6,0x800003,0x9a8,0xba8,0x138,9,0,4,
8,0x7a,5,0x800002,0xe94,0x1094,0xb8,8,0,4,4,0x80,4,0x800002,0x1030,0x1230,
0x408,8,0,4,0xc,0x8a,1,0x800002,0xf4c,0x114c,8,0,0,4,0,0x94,
1,0x800002,0xf54,0x1154,0xdc,0,0,4,0,0x11,3,0,0,0x1c6f,0xa1,0,
0,1,0,1,2,0,0,0x1910,0x200,0x11,0xf,4,0x10,9,3,0,
0,0x1b10,0x15f,0,0,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x0};
const uint32_t BDSP_IMG_aids_adts_header [2] = {sizeof(BDSP_IMG_aids_adts_array1), 1};
const void * const BDSP_IMG_aids_adts [2] = {BDSP_IMG_aids_adts_header, BDSP_IMG_aids_adts_array1};
/* End of File */
