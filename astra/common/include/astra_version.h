/******************************************************************************
 *  Copyright (C) 2018 Broadcom.
 *  The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to
 *  the terms and conditions of a separate, written license agreement executed
 *  between you and Broadcom (an "Authorized License").  Except as set forth in
 *  an Authorized License, Broadcom grants no license (express or implied),
 *  right to use, or waiver of any kind with respect to the Software, and
 *  Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 *  THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 *  IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization,
 *  constitutes the valuable trade secrets of Broadcom, and you shall use all
 *  reasonable efforts to protect the confidentiality thereof, and to use this
 *  information only in connection with your use of Broadcom integrated circuit
 *  products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 *  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 *  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 *  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 *  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 *  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 *  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 *  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 *  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 *  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 *  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 *  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/


#ifndef ASTRA_VERSION_H
#define ASTRA_VERSION_H

/* current version numbers */
#define ASTRA_VERSION_MAJOR 2
#define ASTRA_VERSION_MINOR 0
#define ASTRA_VERSION_BUILD 0

/* utility macros */
#define ASTRA_VERSION_MAJOR_MASK  0xff000000
#define ASTRA_VERSION_MAJOR_SHIFT 24

#define ASTRA_VERSION_MINOR_MASK  0x00ff0000
#define ASTRA_VERSION_MINOR_SHIFT 16

#define ASTRA_VERSION_BUILD_MASK  0x0000ffff
#define ASTRA_VERSION_BUILD_SHIFT 0

#define ASTRA_VERSION_WORD                                  \
    ((ASTRA_VERSION_MAJOR << ASTRA_VERSION_MAJOR_SHIFT) |   \
     (ASTRA_VERSION_MINOR << ASTRA_VERSION_MINOR_SHIFT) |   \
     (ASTRA_VERSION_BUILD << ASTRA_VERSION_BUILD_SHIFT))

#define ASTRA_VERSION_MAJOR_GET(v) (((v) & ASTRA_VERSION_MAJOR_MASK) >> \
                                    ASTRA_VERSION_MAJOR_SHIFT)

#define ASTRA_VERSION_MINOR_GET(v) (((v) & ASTRA_VERSION_MINOR_MASK) >> \
                                    ASTRA_VERSION_MINOR_SHIFT)

#define ASTRA_VERSION_BUILD_GET(v) (((v) & ASTRA_VERSION_BUILD_MASK) >> \
                                    ASTRA_VERSION_BUILD_SHIFT)

#endif /* ASTRA_VERSION_H */
