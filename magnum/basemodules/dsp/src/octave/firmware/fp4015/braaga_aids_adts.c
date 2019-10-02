/*******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/

#include "bchp.h"

const uint32_t BDSP_IMG_aids_adts_array1[] = {
0x464c457f,0x10101,0x0,0x0,0x4e0003,0x1,0x0,0x34,0x1df4,0xd5050000,0x200034,0x280008,0x110012,0x1,0x200,0x0,
0x0,0xa94,0xa94,0x5,0x200,0x1,0xc94,0xa94,0xa94,0x308,0x308,0x100006,0x4,0x1,0xf9c,0xd9c,
0xd9c,0x231,0x231,0x100004,0x1,0x1,0x11d0,0xfd0,0xfd0,0x140,0x140,0x100004,0x4,0x1,0x1310,0x1110,
0x1110,0x444,0x444,0x100004,0x4,0x1,0x1754,0x1554,0x1554,0x4,0x4,0x4,0x4,0x1,0x1758,0x1558,
0x1558,0x209,0x209,0x10000006,0x8,0x2,0xc94,0xa94,0xa94,0x148,0x148,0x100006,0x4,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xbc000036,0x3fda0000,0x3fd80001,0x3a039a03,0xed5c007e,0x22b00022,0x22b00fe0,0x6a102fa1,0x3fae100c,0x743a089f,0x2cc01820,0x3fa5fe80,0x61f3fffb,0x38820850,0x38039a03,0x3a039a02,
0xad5c007e,0x22b00021,0x6a102fa0,0x3fae000e,0xeccff83e,0x2f3fffff,0x2d5be820,0xb43be85f,0x3fa5fe80,0x3f9207f7,0x38039a02,0x3a039a02,0xbfac004c,0x3da41020,0x1a408048,0x6d5be203,
0xa7c03048,0x3f903041,0x3f59c604,0x3fac4008,0xbc0004c7,0x3fda0000,0x3f591200,0xbc3fffd1,0x1a440841,0x1a438840,0x5a410846,0x62b00187,0x3fac602a,0x1a400189,0x3fac9026,0x7c000006,
0x22b00fe0,0x22b002e0,0x6cc0180b,0x1c4091ca,0x3f90a7fb,0x743a019f,0x22b01fc0,0x3fa5fe80,0x7fae0013,0x2d5be805,0xda41087e,0x22b00160,0x22b01fc0,0xf43a0f9f,0x3fa5fe80,0x7f9207f6,
0x2d5be805,0x38039a02,0xe1f3ffff,0x2cc01200,0xa7c00048,0x72868002,0x1a410841,0x3fac1012,0x1a40005f,0x3fadf00e,0x7fa5fe80,0x22b00fc0,0x9a41087e,0x2cc01820,0xf43a0f9f,0x3f91f7f7,
0xbc3fff67,0x1a420841,0x1a418840,0x5a430841,0x1a428840,0x37839a02,0x3c3fff5a,0x3a3f9b71,0x3f59f208,0xa7c48e70,0x2cc10e4a,0x3fd86209,0xe7d01e40,0x27d02e48,0x2c440fc1,0x2f000002,
0xe7c81e48,0x27ccae50,0x27cc0e78,0x27c8ae40,0xfc00044d,0x22b00280,0x27c49e78,0x27cb9e50,0xbc00044b,0x22b18fc0,0x27c3ee68,0xe2b01fc3,0x27cc8e60,0x23912fc2,0x22b00fc1,0xbc0003df,
0x22b00200,0x27c03e68,0xda4e0e7e,0x1a500e7f,0xe7c3efc0,0x22b0cfc0,0x7c00042f,0x27c3ee68,0xe2b02ffe,0x27cc8e68,0x22b00200,0xa7c3ee68,0x3c000087,0xda4e8e7e,0x1a508e7f,0x22b18fc0,
0xe7c3efc0,0x7c000419,0x27c3ee68,0xec400fc2,0x2ffff000,0x22b03fc7,0x27cc8e70,0xbc0003df,0x3da42200,0x27c07e68,0xda4f0e7e,0x1a4f8e7f,0x2cc10e40,0xe7c3efc0,0x3c000405,0x383f9b71,
0x9b430e7e,0x1a428e4e,0xa78fee50,0x3face00c,0x2d5be38f,0x3facf018,0x60100013,0x328a03c2,0x3c0003f7,0x1a4e0e40,0x7c0003f9,0x22b18fc1,0xbc0003fb,0x27c3ee68,0x1a4d0e40,0x7c3ffff6,
0x1a4f0e40,0xbc0003eb,0x1a4e8e40,0x22b0cfc1,0xbc0003eb,0x27c3ee68,0x1a4d0e40,0x3fa3a000,0xe2b01fca,0x22b00fc8,0xa7c0a040,0x1a480089,0x3fac9020,0xda40808c,0x1a40800f,0x1a41000b,
0x1a488081,0xdb42008e,0x1a40008d,0x2cc012c3,0x2d58c3c2,0xe7c8a060,0x27cff050,0x2788e070,0x27ccd048,0xa7cff040,0x27c02008,0x27c03010,0x3fa3a000,0x7c0003b2,0x22b0cfc1,0xbf59ea3f,
0x6cc08fc8,0x2783f008,0x7fa3a000,0x27c08000,0x7fa3a000,0x22b01fc8,0x7fa3a000,0x1a428008,0xda40803e,0xa7c3e040,0x1a41003f,0xa7c3f080,0x3fa3a000,0x3a039a01,0xbc00039d,0x22b00020,
0x1a408000,0x1a410800,0x37839a01,0x3c000392,0x3a039a01,0xbc00038f,0x22b00020,0x1a408000,0x7c000389,0x1a410800,0x62b18fc1,0x22b00800,0x37839a01,0x3c000372,0xf0bb9e45,0x27afae70,
0x1f5bf002,0x1f5bf043,0xed0020d1,0x2cc90e52,0x3da7ef82,0x2390ffd8,0xec4e0fc1,0x2f000001,0x2c4d0fc0,0x2f000002,0xec415fc8,0x2f000004,0x2c480fc9,0x2f000007,0xec454fd0,0x2f000010,
0x2c40afd4,0x2f000002,0xec4c0fd5,0x2f000003,0x2c4a0fd6,0x2f000005,0xec42afd7,0x2f000008,0x2c480fd9,0x2f000016,0xec440fca,0x2f00000b,0x2ca2a20e,0x2f000008,0xeca0024c,0x2f00000f,
0x2caa8408,0x2f000020,0xecac0044,0x2f000003,0x2caa0006,0x2f000005,0xeca8028a,0x2f000016,0x2d051493,0x28382e41,0xeca15505,0x2f000004,0x2ca80547,0x2f000007,0xeca4058f,0x2f00000b,
0x2ca545cd,0x2f000010,0xeca0060b,0x2f00001e,0x2ca00649,0x2f00002d,0xffda0000,0x28084e41,0x3fda0000,0x28106e41,0xffda0000,0x2818ee41,0x3fda0000,0x2820ce41,0xffda0000,0x2828ae41,
0x3fda0000,0x28308e41,0xffa3a000,0x28402e41,0x2cc90e79,0x1a7004c8,0x3a7b9a08,0xda400048,0x279f7e70,0x2cca0e77,0x3da40062,0xf0c3fdc7,0x22b00124,0x22b00040,0x22b00fc3,0xe2b00dc2,
0x22b51fc1,0x2cca0e61,0x2cc30e60,0x6cc28e65,0x1a41821f,0x3fa5fe80,0xbf59ba3f,0x5a400fc0,0x27c3f8e8,0xda400004,0x3007f847,0x22b02fc2,0x22b00fc1,0x62b00843,0x1a43011f,0x3fa5fe80,
0x60100208,0x32868202,0xda440e46,0x22b00843,0x22b02fc2,0x22b00fc1,0xe014007b,0x3da7ffc4,0x22b00880,0x362411b1,0xda40088f,0x22b38fc3,0x22b00802,0x22b51fc1,0x1a4103df,0x3fa5fe80,
0x3f9080e1,0xbf800353,0x22b0081a,0x2cc6ae56,0xdb8006be,0x1b802699,0x2cc0269a,0x6a008f9b,0x7d4db67e,0xb003e593,0xbf59ba3f,0x5a400fc0,0x27d7fe50,0xda40001d,0x22b00843,0x22b02fc2,
0x22b00fc1,0x1a43075f,0x3fa5fe80,0xa010018b,0x1a8d8e66,0x328a0202,0xbf59ba3f,0x5a400fc0,0x27d7fe50,0xda400004,0x22b00843,0x22b02fc2,0x22b00fc1,0x1a43011f,0x3fa5fe80,0xe014018d,
0x1a8dce65,0x32aa0202,0x62b00960,0xda8e0e7f,0x2280383e,0x6a00bf86,0x6a605fca,0xfd4c6290,0x6d4be43e,0x76427fbc,0x2f17fe17,0x2008000b,0xe01c00b6,0x2a10a94c,0x6200f320,0x76620816,
0xbc00001c,0x27dffe50,0x2cc28e65,0xbf59ba3f,0x5a400fc0,0x27d7fe50,0x5a40003e,0x5a430f9f,0x3fa5fe80,0x60180126,0x32c68202,0xa2b01ffe,0x27dffe50,0xa7c3e8e8,0xbf59ba3f,0x5a400fc0,
0x27c7fe50,0xda40001f,0x22b00943,0x22b02fc2,0x22b00fc1,0x1a4307df,0x3fa5fe80,0x60140177,0x32aa0202,0xacc10840,0x2797fe50,0x2783f850,0x3007f004,0x3007f004,0x3007f004,0x3007f004,
0x3007f004,0x3007f004,0x7007f004,0x6783f000,0xdb550e47,0x1b4088c3,0x22b00905,0x2cc10846,0xe2b01fc8,0x30047144,0x279c3e58,0x1b41084a,0xacc08150,0x2780a910,0x1c01018b,0x7004b404,
0x1c01018c,0x7004c404,0x1c010191,0x70051404,0x1c01018d,0x7004d404,0x1c010192,0x70052404,0x1c010193,0x70053404,0x1c010194,0xf0054404,0x1b4001be,0xe783e400,0x1b5f0e77,0x387b9a08,
0xbf59ba3f,0x5a400fc0,0x22b00843,0xda400010,0x22b02fc2,0x22b00fc1,0x27d7fe50,0x1a43041f,0x3fa5fe80,0x61ffff11,0x32e68202,0xda550e55,0x22700fc0,0x2c4abfd1,0x2fd42e69,0xec46afd3,
0x2ff00000,0x2a820452,0x27c15e60,0xbc000199,0x2d113ffe,0x7d4d2f81,0x3c3ffef6,0xda400891,0x1f5bf827,0x2c480fd3,0x2f00003e,0xec411fd6,0x2f00002b,0x2cae04ca,0x2f00002e,0xeca4058b,
0x2f00001f,0x2c444fd2,0x2f0000ac,0xe39fafd4,0x2860ae61,0x2c4c0fd5,0x2f00005d,0xfda7ef8a,0x2d067df7,0x2c400fcd,0x2f000177,0xeca8050f,0x2f0000bb,0x2ca8834e,0x2f000158,0xeca0048c,
0x2f00007d,0x2ca2254d,0x2f000056,0xe3904fd9,0x2850ee61,0x1a4088d8,0x1a43045f,0xea20398e,0x2858ce61,0x2cc28e65,0x2868ae61,0xda510dd7,0x1f5bf426,0x2280138f,0x22b00fc5,0xe2b01fc4,
0x22b00fc2,0x22b00943,0x22b51fc1,0xe7cff618,0x27ccf620,0x22b00880,0x27c17628,0xe7d57e60,0x27d59e58,0x27d99e40,0x27d66e50,0x3fa5fe80,0xdb450e7e,0x22300fc1,0x22300800,0xa797ee70,
0x3c3ffd9d,0xda4088db,0x1a440e7e,0x27d88e48,0xb883e991,0xe1e3feec,0x27ce76e8,0x27ddbe58,0x27dffe60,0xbc3fff16,0x22b01ffe,0xa7dfee50,0xda550e4e,0x22700fc0,0x2c4abfca,0x2fd42e69,
0xec41efcc,0x2ff00000,0x2a82028b,0x27c0ee60,0xbc0000f7,0x2d10cffe,0x7d4cbf81,0x3c3ffec8,0xda550e48,0x22703fc0,0x2c4abfde,0x2fd42e69,0xec452fe0,0x2ff00000,0x2a82079f,0x27c26e70,
0xfc0000df,0x2d120ffe,0x27c08e60,0x7d4dff81,0x3c3ffe64,0xda8dce65,0x1a550e43,0x2c43afc9,0x2ff00000,0xec4abfc0,0x2fd42e69,0x2d109fc2,0x27c03e60,0xea820001,0x27c25e70,0x22703fc0,
0x22b00960,0x7c0000bd,0x3d4c1081,0x3c3ffe60,0xda440e43,0x1a550e45,0x2c486fc9,0x2ff00000,0xec4abfc0,0x2fd42e69,0x2d109fc2,0x27c03e70,0xea82003e,0x27c05e60,0x22703fc0,0xbd4fe081,
0x3c00009f,0x3c3ffde2,0xda450e42,0x22700fc0,0x2c4abfc8,0x2fd42e69,0xec402fc1,0x2ff00000,0x2a820204,0x27c02e60,0xbc00008b,0x2d101ffe,0x7d4c4f81,0x3c3ffe78,0x3a039a01,0xbf59ce3f,
0x6cc08fc8,0x22b00020,0xe7c01018,0x27c02020,0x27c3f828,0x22b78fc0,0x7c000079,0x27c08800,0xbc000075,0x27c08808,0x22b08fc0,0x27c08810,0x38039a01,0x3fa3a000,0xffa3a000,0x2901103e,
0x3fda0000,0xa803e041,0x7fa3a000,0x1a428008,0x7c000044,0x22b18fc1,0xbf59b83f,0x6cc08fc8,0x22b01fc9,0xe7c01008,0x27c02018,0x27c09028,0x27c08000,0x3fa3a000,0x7fa3a000,0x1a408008,
0x7fa3a000,0x2783f008,0x3fa3a000,0x7fa3a000,0x1a418008,0x7fa3a000,0x1a420008,0x7f59463d,0x2fffffff,0x3fa3d000,0x7f59503d,0x2fffffff,0x3fa3d000,0x7f59423d,0x2fffffff,0x3fa3d000,
0x7f593e3d,0x2fffffff,0x3fa3d000,0x7f59523d,0x2fffffff,0x3fa3d000,0x7f595a3d,0x2fffffff,0x3fa3d000,0x7f595e3d,0x2fffffff,0x3fa3d000,0x7f597e3d,0x2fffffff,0x3fa3d000,0x7f597a3d,
0x2fffffff,0x3fa3d000,0x7f59483d,0x2fffffff,0x3fa3d000,0x1,0x17a,0x1,0x191,0x1,0x1a2,0x1,0x1b0,0x1,0x1bd,0x1,
0x1d4,0x1,0x1e4,0x1,0x1f8,0x1,0x20b,0x1,0x21d,0xe,0x1e,0x10,0x0,0x4,0xfd0,0x5,
0xd9c,0x6,0xbdc,0xa,0x231,0xb,0x10,0x3,0x1558,0x70000004,0x4,0x70000005,0x90,0x7,0x1110,0x8,
0x444,0x9,0xc,0x70000000,0x15e8,0x70000001,0x1254,0x70000002,0x4,0x70000003,0x8,0x70000006,0xf9c,0x70000007,0x1310,0x70000008,
0x125c,0x70000009,0x1734,0x7000000a,0x4,0x7000000b,0x28,0x7000000c,0xd42e69ab,0x7000000d,0x0,0x6ffffff9,0x4f,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x191,0x0,0x0,0x4,0x21d,
0x0,0x0,0x4,0x20b,0x0,0x0,0x4,0x1f8,0x0,0x0,0x4,0x1e4,0x0,0x0,0x4,0x1d4,
0x0,0x0,0x4,0x1bd,0x0,0x0,0x4,0x1b0,0x0,0x0,0x4,0x1a2,0x0,0x0,0x4,0x17a,
0x0,0x0,0x4,0x1e,0x0,0x0,0x4,0x31,0x15c,0x140,0x10012,0x1,0x0,0x0,0x10012,0x154,
0x0,0x0,0x11,0xc6,0x0,0x0,0x12,0xc0,0x0,0x0,0x12,0xf,0x0,0x0,0x22,0x173,
0x0,0x0,0x12,0x126,0x0,0x0,0x11,0xaa,0x0,0x0,0x12,0xde,0x0,0x0,0x12,0xe4,
0x0,0x0,0x12,0xec,0x0,0x0,0x12,0x100,0x0,0x0,0x11,0x94,0x0,0x0,0x12,0x15e,
0x0,0x0,0x12,0x14f,0x0,0x0,0x12,0x735f5f00,0x65726168,0x6e695f64,0x5f007469,0x6178635f,0x6e69665f,0x7a696c61,0x696c0065,0x64696162,
0x64615f73,0x30407374,0x302e312e,0x325a5f00,0x74654736,0x6f676c41,0x63657053,0x63696669,0x49736449,0x416f666e,0x50737464,0x53633950,0x49636e79,0x506f666e,0x63393150,0x73726150,
0x646e4165,0x696c6156,0x65746164,0x50507550,0x41633331,0x6d756363,0x74616c75,0x31755065,0x53444234,0x6c415f50,0x69726f67,0x6d6874,0x78675f5f,0x65705f78,0x6e6f7372,0x74696c61,
0x6a735f79,0x555f0030,0x6e69776e,0x6a535f64,0x525f6a4c,0x73696765,0x726574,0x776e5a5f,0x555f006d,0x6e69776e,0x6a535f64,0x555f6a4c,0x6765726e,0x65747369,0x62610072,0x74726f,
0x6c645a5f,0x6d7650,0x776e555f,0x5f646e69,0x6a4c6a53,0x7365525f,0x656d75,0x56545a5f,0x5f30314e,0x7878635f,0x76696261,0x5f373131,0x616c635f,0x745f7373,0x5f657079,0x6f666e69,
0x5a5f0045,0x314e5654,0x635f5f30,0x62617878,0x32317669,0x735f5f30,0x6c635f69,0x5f737361,0x65707974,0x666e695f,0x6600456f,0x656572,0x62645f70,0x626f5f67,0x545f006a,0x65677261,
0x72505f74,0x5f746e69,0x68736148,0x6d006465,0x6f6c6c61,0x696c0063,0x64747362,0x2d2b2b63,0x6d696c73,0x30373240,0x302e302e,0x62696c00,0x5f636367,0x37324073,0x2e302e30,0x696c0030,
0x6f706662,0x2e314073,0x302e30,0x6362696c,0x30373240,0x302e302e,0x62696c00,0x75626564,0x6f635f67,0x6f72746e,0x2e30406c,0x302e31,0x6362696c,0x6f6d6d6f,0x2e30406e,0x302e31,
0x6362696c,0x6f6d6d6f,0x64695f6e,0x2e304073,0x302e31,0x6d62696c,0x6c635f6d,0x746e6569,0x312e3040,0x6c00302e,0x74736269,0x5f656761,0x30406f69,0x302e312e,0x62696c00,0x6e72656b,
0x6f5f6c65,0x30407370,0x302e312e,0x0,0x3,0x1c,0x13,0x1b,0x16,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x1a,0xe,0x0,0x0,0x0,0xf,0x11,0xd,0x10,0x12,0x15,
0x14,0x17,0x18,0x19,0xc,0x15c,0x0,0x0,0x191,0x21d,0x20b,0x1f8,0x1e4,0x1d4,0x1bd,0x1b0,
0x1a2,0x17a,0x1e,0x31,0x1,0x154,0xc6,0xc0,0xf,0x173,0x126,0xaa,0xde,0xe4,0xec,0x100,
0x94,0x15e,0x14f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x1564,0x121,0x13,0x1588,0x121,0x10,0x15a4,0x121,0x11,0x1734,0x221,0x0,
0x1738,0x321,0x0,0x173c,0x421,0x2,0x1740,0x521,0x2,0x1744,0x621,0x0,0x155c,0x721,0x5,0x1748,
0x721,0x0,0x1574,0x821,0x1c,0x1578,0x821,0x26e,0x158c,0x821,0x1c1,0x15e4,0x821,0x219,0x174c,0x821,
0x7d,0x15dc,0x921,0x149,0x1750,0x921,0x0,0x156c,0xa21,0x25,0x1580,0xa21,0x2,0x159c,0xa21,0x22,
0x15a8,0xa21,0x0,0x15cc,0xa21,0x6,0x1754,0xa21,0x27,0x1558,0xb02,0x1694,0x1560,0xb02,0x9f8,0x1568,
0xb02,0xa0c,0x1570,0xb02,0x1714,0x157c,0xb02,0x1680,0x1584,0xb02,0x1654,0x1590,0xb02,0x1688,0x1594,0xb02,
0xa14,0x1598,0xb02,0x1610,0x15a0,0xb02,0x16c6,0x15ac,0xb02,0x1640,0x15b0,0xb02,0x16df,0x15b4,0xb02,0x16f5,
0x15b8,0xb02,0xa00,0x15bc,0xb02,0x1624,0x15c0,0xb02,0xa08,0x15c8,0xb02,0x171f,0x15d0,0xb02,0x1618,0x15d4,
0xb02,0x1648,0x15d8,0xb02,0x16b6,0x15f0,0xb02,0x1734,0x15fc,0xb02,0x1734,0x1600,0xb02,0x1734,0x1604,0xb02,
0x1734,0x1608,0xb02,0x1734,0x160c,0xb02,0x160c,0x1614,0xb02,0x15d8,0x161c,0xb02,0x15a0,0x1620,0xb02,0x1598,
0x1628,0xb02,0x15d0,0x162c,0xb02,0x29c,0x1630,0xb02,0x2f4,0x1634,0xb02,0x2a0,0x1638,0xb02,0x1560,0x163c,
0xb02,0x15b8,0x1644,0xb02,0x15b0,0x164c,0xb02,0x15b4,0x1650,0xb02,0x15ac,0x1658,0xb02,0x15d4,0x165c,0xb02,
0x334,0x1660,0xb02,0x350,0x1664,0xb02,0x498,0x1668,0xb02,0x15c0,0x166c,0xb02,0x310,0x1670,0xb02,0x1568,
0x1674,0xb02,0x1594,0x1678,0xb02,0x318,0x167c,0xb02,0x320,0x1684,0xb02,0x1570,0x168c,0xb02,0x15c8,0x1690,
0xb02,0x157c,0x1698,0xb02,0x1590,0x169c,0xb02,0x9b4,0x16a0,0xb02,0x9d0,0x16a4,0xb02,0x9b8,0x16a8,0xb02,
0x9c8,0x1610,0xb1e,0x15a8,0x1610,0xb25,0x8,0x1618,0xb1e,0x1580,0x1618,0xb25,0x8,0x1640,0xb1e,0x15a8,
0x1640,0xb25,0x8,0x1648,0xb1e,0x1580,0x1648,0xb25,0x8,0x1680,0xb1e,0x15a8,0x1680,0xb25,0x8,0x1688,
0xb1e,0x1580,0x1688,0xb25,0x8,0xd42e69ab,0x1694,0x0,0x9f8,0x0,0xa0c,0x0,0x1714,0x0,0x0,0x1680,
0x0,0x1654,0x0,0x0,0x1688,0xa14,0x1610,0x0,0x16c6,0x0,0x0,0x1640,0x16df,0x16f5,0xa00,0x1624,
0xa08,0x0,0x171f,0x0,0x1618,0x1648,0x16b6,0x0,0x0,0x0,0x0,0x0,0x1734,0x0,0x0,0x1734,
0x1734,0x1734,0x1734,0x160c,0x8,0x16b6,0x8,0x16c6,0x1610,0x0,0x1618,0x29c,0x2f4,0x2a0,0x9f8,0xa00,
0x8,0x16df,0x8,0x16f5,0x1640,0x0,0x1648,0x334,0x350,0x498,0xa08,0x310,0xa0c,0xa14,0x318,0x320,
0x8,0x1714,0x8,0x171f,0x1680,0x0,0x1688,0x9b4,0x9d0,0x9b8,0x9c8,0x601ffff,0x10000,0x33310002,0x63634163,0x6c756d75,
0x50657461,0x32320075,0x63614163,0x64416568,0x63417374,0x756d7563,0x6574616c,0x31007550,0x61506339,0x41657372,0x6156646e,0x6164696c,0x75506574,0x63383200,0x68636141,0x74644165,
0x72615073,0x6e416573,0x6c615664,0x74616469,0x755065,0x79536339,0x6e49636e,0x31006f66,0x61416338,0x41656863,0x53737464,0x49636e79,0x6f666e,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2047534d,0x7570093a,0x666e695f,0x7369206f,0x544f4e20,0x6c617620,0xa6469,
0x2047534d,0x6463093a,0x69732062,0x7420657a,0x73206f6f,0x6c6c616d,0xa,0x2047534d,0x6463093a,0x61645f62,0x315b6174,0x78253d5d,0xa,0x2047534d,0x6463093a,0x61645f62,
0x305b6174,0x78253d5d,0xa,0x2047534d,0x7473093a,0x20656761,0x72206f69,0x20646165,0x6f727265,0xa72,0x2047534d,0x6463093a,0x7a697362,0x64253d65,0xa,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x10003,0x0,0xa1c,0x0,0x20003,0x0,0x1554,0x0,0x30003,0x0,
0x1558,0x0,0x40003,0x0,0x1761,0x0,0x50003,0x0,0x1761,0x0,0x60003,0x0,0xf0000000,0x0,0x70003,0x0,
0xbdc,0x0,0x80003,0x0,0xd9c,0x0,0x90003,0x0,0xa94,0x0,0xa0003,0x0,0xfd0,0x0,0xb0003,0x0,
0x1110,0x0,0xc0003,0x0,0x1054,0x0,0xd0003,0x0,0x105c,0x0,0xe0003,0x1,0x0,0x0,0x11,0xb,
0x0,0x0,0x12,0x23,0x0,0x0,0x12,0x29,0x0,0x0,0x22,0x38,0x0,0x0,0x12,0x3f,
0x0,0x0,0x10012,0x4d,0x0,0x0,0x11,0x76,0x0,0x0,0x12,0x8c,0x0,0x0,0x12,0x92,
0x0,0x0,0x12,0x9a,0x0,0x0,0x12,0xae,0x0,0x0,0x11,0xd4,0x0,0x0,0x12,0xea,
0x0,0x0,0x12,0xff,0x15c,0x140,0x10012,0x162,0x0,0x0,0x12,0x645f7000,0x6f5f6762,0x5f006a62,0x69776e55,0x535f646e,
0x5f6a4c6a,0x65726e55,0x74736967,0x5f007265,0x6d776e5a,0x635f5f00,0x665f6178,0x6c616e69,0x657a69,0x6c6c616d,0x5f00636f,0x6168735f,0x5f646572,0x74696e69,0x545a5f00,0x30314e56,
0x78635f5f,0x69626178,0x30323176,0x69735f5f,0x616c635f,0x745f7373,0x5f657079,0x6f666e69,0x555f0045,0x6e69776e,0x6a535f64,0x525f6a4c,0x73696765,0x726574,0x726f6261,0x5a5f0074,
0x76506c64,0x555f006d,0x6e69776e,0x6a535f64,0x525f6a4c,0x6d757365,0x5a5f0065,0x314e5654,0x635f5f30,0x62617878,0x31317669,0x635f5f37,0x7373616c,0x7079745f,0x6e695f65,0x456f66,
0x78675f5f,0x65705f78,0x6e6f7372,0x74696c61,0x6a735f79,0x545f0030,0x65677261,0x72505f74,0x5f746e69,0x68736148,0x5f006465,0x4736325a,0x6c417465,0x70536f67,0x66696365,0x64496369,
0x666e4973,0x7464416f,0x39505073,0x6e795363,0x666e4963,0x3150506f,0x61506339,0x41657372,0x6156646e,0x6164696c,0x75506574,0x33315050,0x63634163,0x6c756d75,0x50657461,0x42343175,
0x5f505344,0x6f676c41,0x68746972,0x7266006d,0x6565,0x6d79732e,0x626174,0x7274732e,0x626174,0x7368732e,0x61747274,0x742e0062,0x747865,0x746c702e,0x65732e00,0x61685f67,
0x2e006873,0x61746164,0x72702e00,0x7365636f,0x686b5f73,0x706165,0x6f72702e,0x73736563,0x7061635f,0x702e0073,0x746e6972,0x69727453,0x73676e,0x6e79642e,0x6d7973,0x6e79642e,
0x727473,0x6e79642e,0x63696d61,0x61682e00,0x2e006873,0x616c6572,0x6e79642e,0x70662e00,0x736e7964,0x2e006d79,0x79647066,0x7274736e,0x66666f,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1b,0x1,0x6,0x0,0x200,0xa1c,0x0,0x0,0x200,
0x0,0x21,0x1,0x6,0xa1c,0xc1c,0x78,0x0,0x0,0x4,0x0,0x26,0x1,0x2,0x1554,0x1754,
0x4,0x0,0x0,0x4,0x0,0x30,0x1,0x400003,0x1558,0x1758,0x209,0x0,0x0,0x8,0x0,0x36,
0x1,0x10800001,0x1761,0x1961,0x0,0x0,0x0,0x1,0x0,0x45,0x1,0x40800001,0x1761,0x1961,0x0,0x0,
0x0,0x1,0x0,0x53,0x1,0x0,0xf0000000,0x1964,0x96,0x0,0x0,0x4,0x0,0x61,0xb,0x800002,
0xbdc,0xddc,0x1c0,0x9,0xc,0x4,0x10,0x69,0x3,0x800002,0xd9c,0xf9c,0x231,0x0,0x0,0x1,
0x0,0x71,0x6,0x800003,0xa94,0xc94,0x148,0x9,0x0,0x4,0x8,0x7a,0x5,0x800002,0xfd0,0x11d0,
0x84,0x8,0x0,0x4,0x4,0x80,0x4,0x800002,0x1110,0x1310,0x444,0x8,0x0,0x4,0xc,0x8a,
0x1,0x800002,0x1054,0x1254,0x8,0x0,0x0,0x4,0x0,0x94,0x1,0x800002,0x105c,0x125c,0xb4,0x0,
0x0,0x4,0x0,0x1,0x2,0x0,0x0,0x19fc,0x1f0,0x10,0xf,0x4,0x10,0x9,0x3,0x0,
0x0,0x1bec,0x167,0x0,0x0,0x1,0x0,0x11,0x3,0x0,0x0,0x1d53,0xa1,0x0,0x0,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
const uint32_t BDSP_IMG_aids_adts_header [2] = {sizeof(BDSP_IMG_aids_adts_array1), 1};
const void * const BDSP_IMG_aids_adts [2] = {BDSP_IMG_aids_adts_header, BDSP_IMG_aids_adts_array1};
/* End of File */
