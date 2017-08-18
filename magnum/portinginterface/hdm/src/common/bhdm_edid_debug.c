/******************************************************************************
 *  Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.
 ******************************************************************************/

#include "bhdm.h"
#include "bhdm_priv.h"
#include "bavc.h"

#define BHDM_EDID_P_DEBUG_MAX_EXTENSIONS 4

BDBG_MODULE(BHDM_EDID) ;

static const char * const BHDM_EDID_DEBUG_P_CeaAudioSampleRateText[] =
{
	"32 kHz",
	"44.1 kHz (CD)",
	"48 kHz",
	"96 kHz"
#if BHDM_CONFIG_88_2KHZ_AUDIO_SUPPORT
	, "88.2 kHz"
#endif

#if BHDM_CONFIG_176_4KHZ_AUDIO_SUPPORT
	, "176.4 kHz"
#endif

#if BHDM_CONFIG_192KHZ_AUDIO_SUPPORT
	, "192 kHz"
#endif
} ;

static const char EnumError[] = BDBG_STRING_INLINE("Enum Error") ;

const char * BHDM_EDID_DEBUG_CeaAudioSampleRateToStr(
    uint8_t uiCeaSampleRateId)
{
    uint8_t entries ;

    entries=
        sizeof(BHDM_EDID_DEBUG_P_CeaAudioSampleRateText) /
        sizeof(*BHDM_EDID_DEBUG_P_CeaAudioSampleRateText) ;

    if (uiCeaSampleRateId < entries)
        return BHDM_EDID_DEBUG_P_CeaAudioSampleRateText[uiCeaSampleRateId] ;
    else
        return EnumError;
}



#if BDBG_DEBUG_BUILD
static void BHDM_EDID_DEBUG_P_PrintEdidBlock(uint8_t *pEDID) ;
#endif

/*
the following EDID(s) can be used to debug any Rx EDID that a BCMxxxx chip may have problems parsing.
a copy of the EDID must first be obtained from the actual TV/Receiver and copied here;

The EDID must be copied to the approproiate macro below
	HDMI 2.0 EDIDs (e.g. 4Kp50/60)
or
	the else part for basic HDMI 1.x EDIDs

Enable the BHDM_EDID debug messages
     # export msg_modules=BHDM_EDID
to get a copy of the EDID; prior to running any app.

NOTE: This following EDID is used for DEBUGGING ONLY! i.e. debugging the parsing of an EDID which
you may not have the HDMI Rx.  Presumeably the EDID was provided by the user with the TV.

*/


static const uint8_t DebugRxEdid[] =
{
#if BHDM_HAS_HDMI_20_SUPPORT

	/* HDMI 2.0 EDID 3840x2160p60 Support */
	/* BLOCK 0 */

	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x08,0x6D,0x45,0x74,0x67,0x45,0x23,0x01,
	0x1B,0x1B,0x01,0x03,0x80,0x80,0x48,0x78,0x0A,0xDA,0xFF,0xA3,0x58,0x4A,0xA2,0x29,
	0x17,0x49,0x4B,0x20,0x00,0x00,0x31,0x40,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x08,0xE8,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,
	0x8A,0x00,0xBA,0x88,0x21,0x00,0x00,0x1E,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
	0x58,0x2C,0x45,0x00,0xBA,0x88,0x21,0x00,0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x42,
	0x72,0x6F,0x61,0x64,0x63,0x6F,0x6D,0x20,0x32,0x2E,0x30,0x0A,0x00,0x00,0x00,0xFD,
	0x00,0x17,0x3D,0x0F,0x88,0x3C,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x01,0x62,

	/* V3 Timing Extension  */

	0x02,0x03,0x5B,0xF0,0x51,0x10,0x05,0x61,0x60,0x1F,0x22,0x21,0x20,0x02,0x07,0x12,
	0x16,0x04,0x13,0x5F,0x5E,0x5D,0x3E,0x0F,0x7F,0x07,0x15,0x07,0x50,0x19,0x07,0x38,
	0x21,0x07,0x28,0x29,0x03,0x30,0x35,0x07,0x48,0x3E,0x1F,0xC0,0x57,0x7F,0x01,0x5F,
	0x7F,0x01,0x67,0x7F,0x01,0x77,0x03,0x0C,0x00,0x10,0x00,0xB8,0x3C,0x20,0xC8,0x6A,
	0x01,0x03,0x04,0x81,0x41,0x00,0x16,0x06,0x08,0x00,0x56,0x58,0x00,0x67,0xD8,0x5D,
	0xC4,0x01,0x78,0x80,0x03,0xE2,0x00,0x4B,0xE2,0x0F,0x0C,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,

#else


	/* HDMI 1.x EDID e.g. 1080p Support */
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x08, 0x6D, 0x25, 0x74, 0x67, 0x45, 0x23, 0x01,
	0x18, 0x18, 0x01, 0x03, 0x80, 0x80, 0x48, 0x78, 0x0A, 0xDA, 0xFF, 0xA3, 0x58, 0x4A, 0xA2, 0x29,
	0x17, 0x49, 0x4B, 0x20, 0x00, 0x00, 0x31, 0x40, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
	0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x04, 0x74, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80,
	0xB0, 0x58, 0x8A, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x42,
	0x72, 0x6F, 0x61, 0x64, 0x63, 0x6F, 0x6D, 0x20, 0x31, 0x2E, 0x30, 0x0A, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x17, 0x3D, 0x0F, 0x88, 0x1E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xBF,
	0x02, 0x03, 0x2F, 0xF0, 0x4B, 0x10, 0x05, 0x20, 0x22, 0x04, 0x03, 0x02, 0x07, 0x06, 0x5E, 0x5F,
	0x23, 0x09, 0x07, 0x01, 0x77, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x3C, 0x21, 0xC8, 0x6A, 0x01,
	0x03, 0x04, 0x81, 0x41, 0x00, 0x16, 0x06, 0x08, 0x00, 0x56, 0x58, 0x00, 0xE2, 0x00, 0x4B, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8D

 #endif
} ;

const uint8_t * BHDM_EDID_P_GetDebugEdid(void)
{
	return &DebugRxEdid[0] ;
}


#if BDBG_DEBUG_BUILD

static int _strlen(const char *s) {
	int i=0;
	while (*s++) i++;
	return i;
}

/******************************************************************************
BERR_Code BHDM_EDID_DEBUG_P_PrintEdidBlock
Summary: Print/Display EDID Block
*******************************************************************************/
static void BHDM_EDID_DEBUG_P_PrintEdidBlock(uint8_t *pEDID)
{
#if !BDBG_NO_LOG
	uint8_t i ;
	uint8_t display_string[128] = "" ;
	uint8_t *ptr ;

	i = 0 ;
	do
	{
		if ((i % BHDM_EDID_BLOCKSIZE == 0) /* add a blank line between blocks*/
		&&  (i != 0)
		&&  (i != BHDM_EDID_BLOCKSIZE))
			BDBG_LOG((" ")) ;

		if ((i % 16 == 0) )                /* 16 bytes per line */
		{
			BDBG_LOG(("%s", display_string)) ;
			BKNI_Snprintf((char *) display_string, BHDM_EDID_BLOCKSIZE, "%02X> ", i) ;
		}

		ptr = display_string + _strlen((char *) display_string) ;
		BKNI_Snprintf((char *) ptr, BHDM_EDID_BLOCKSIZE, "0x%02X,", pEDID[i]) ;
		i++ ;
	} while (i < BHDM_EDID_BLOCKSIZE) ;

	/* pirint last line */
	BDBG_LOG(("%s", display_string)) ;

	BDBG_LOG((" ")) ;
#endif
}



/******************************************************************************
BERR_Code BHDM_EDID_DumpRawEDID
Summary: Dump EDID data
*******************************************************************************/
BERR_Code BHDM_EDID_DumpRawEDID(const BHDM_Handle hHDMI)
{
	BERR_Code rc = BERR_SUCCESS ;
#if !BDBG_NO_LOG
	uint8_t NumExtensions ;
	uint8_t i ;

	BDBG_OBJECT_ASSERT(hHDMI, HDMI) ;

	/* read the 1st EDID Block */
	BHDM_CHECK_RC(rc, BHDM_EDID_GetNthBlock(hHDMI,
		0, hHDMI->AttachedEDID.Block, BHDM_EDID_BLOCKSIZE)) ;

	BDBG_LOG((" ")) ;
	BDBG_LOG(("     /* RAW EDID DUMP from %.13s: */", hHDMI->AttachedEDID.MonitorName)) ;
	BDBG_LOG(("     /* insert into bhdm_edid_debug.c (DebugRxEdid) for simulation/testing */")) ;
	BDBG_LOG(("     /* enable with 'export hdmi_use_debug_edid=y' */")) ;

	BHDM_EDID_DEBUG_P_PrintEdidBlock( hHDMI->AttachedEDID.Block ) ;

	NumExtensions = hHDMI->AttachedEDID.BasicData.Extensions;

	/* MAX number of EDID blocks to dump for EDID debugging; */
	/* any number > 4 is probably meaningless */
	if (NumExtensions > BHDM_EDID_P_DEBUG_MAX_EXTENSIONS)
	{
		BDBG_WRN(("EDID Is probably INVALID!!! due to the large number of extensions: %d",
			NumExtensions)) ;
		BDBG_WRN(("Printing first %d blocks only", BHDM_EDID_P_DEBUG_MAX_EXTENSIONS)) ;
		NumExtensions = BHDM_EDID_P_DEBUG_MAX_EXTENSIONS ;
	}


	/* Search EDID Extension blocks for additional timing descriptors */
	for (i = 1 ; i <= NumExtensions; i++)
	{
		BDBG_LOG((" ")) ;
		BDBG_LOG(("     /* EXTENSION BLOCK %d */", i)) ;

		/* read the next 128 Byte EDID block */
		BHDM_CHECK_RC(rc, BHDM_EDID_GetNthBlock(hHDMI, i,
			hHDMI->AttachedEDID.Block, BHDM_EDID_BLOCKSIZE)) ;

		BHDM_EDID_DEBUG_P_PrintEdidBlock( hHDMI->AttachedEDID.Block ) ;
	}

done :
#endif
	return rc ;
}
#else
BERR_Code BHDM_EDID_DumpRawEDID(const BHDM_Handle hHDMI)
{
	BSTD_UNUSED(hHDMI);
	return BERR_SUCCESS;
}
#endif
