/*
 * NVRAM variable manipulation (direct mapped flash)
 *
 * Copyright (C) 2017, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: nvram_rw.c 258972 2011-05-11 08:57:26Z $
 */

#include <bcm_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <bcmnvram.h>
#include <bcmendian.h>
#include <flashutl.h>
#include <hndsoc.h>
#include <sbchipc.h>
#include <LzmaDec.h>
#ifdef NFLASH_SUPPORT
#include <hndnand.h>
#endif	/* NFLASH_SUPPORT */

struct nvram_tuple *_nvram_realloc(struct nvram_tuple *t, const char *name, const char *value);
void  _nvram_free(struct nvram_tuple *t);
int  _nvram_read(void *buf, int idx);

extern char *_nvram_get(const char *name);
extern int _nvram_set(const char *name, const char *value);
extern int _nvram_unset(const char *name);
extern int _nvram_getall(char *buf, int count);
extern int _nvram_commit(struct nvram_header *header);
extern int _nvram_init(void *si, int idx);
extern void _nvram_exit(void);

static struct nvram_header *nvram_header = NULL;
static int nvram_do_reset = FALSE;



#define NVRAM_LOCK()	do {} while (0)
#define NVRAM_UNLOCK()	do {} while (0)

/* Convenience */
#define KB * 1024
#define MB * 1024 * 1024

char *
nvram_get(const char *name)
{
	char *value;


	NVRAM_LOCK();
	value = _nvram_get(name);
	NVRAM_UNLOCK();

	return value;
}

int
nvram_getall(char *buf, int count)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_getall(buf, count);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_set)(const char *name, const char *value)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_set(name, value);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_unset)(const char *name)
{
	int ret;

	NVRAM_LOCK();
	ret = _nvram_unset(name);
	NVRAM_UNLOCK();

	return ret;
}

int
BCMINITFN(nvram_resetgpio_init)(void *si)
{
	char *value;
	int gpio;
	si_t *sih;

	sih = (si_t *)si;

	value = nvram_get("reset_gpio");
	if (!value)
		return -1;

	gpio = (int) bcm_atoi(value);
	if (gpio > 31)
		return -1;

	/* Setup GPIO input */
	si_gpioouten(sih, ((uint32) 1 << gpio), 0, GPIO_DRV_PRIORITY);

	return gpio;
}

int
BCMINITFN(nvram_reset)(void  *si)
{
	int gpio;
	uint msec;
	si_t * sih = (si_t *)si;

	if ((gpio = nvram_resetgpio_init((void *)sih)) < 0)
		return FALSE;

	/* GPIO reset is asserted low */
	for (msec = 0; msec < 5000; msec++) {
		if (si_gpioin(sih) & ((uint32) 1 << gpio))
			return FALSE;
		OSL_DELAY(1000);
	}

	nvram_do_reset = TRUE;
	return TRUE;
}

#ifdef NFLASH_SUPPORT
static unsigned char nand_nvh[MAX_NVRAM_SPACE];

static struct nvram_header *
BCMINITFN(nand_find_nvram)(hndnand_t *nfl, uint32 off)
{
	int blocksize = nfl->blocksize;
	unsigned char *buf = nand_nvh;
	int rlen = sizeof(nand_nvh);
	int len;

	for (; off < nfl_boot_size(nfl); off += blocksize) {
		if (hndnand_checkbadb(nfl, off) != 0)
			continue;

		len = blocksize;
		if (len >= rlen)
			len = rlen;

		if (hndnand_read(nfl, off, len, buf) == 0)
			break;

		buf += len;
		rlen -= len;
		if (rlen == 0)
			return (struct nvram_header *)nand_nvh;
	}

	return NULL;
}
#endif /* NFLASH_SUPPORT */

extern unsigned char embedded_nvram[];

static struct nvram_header *
BCMINITFN(find_nvram)(si_t *sih, bool embonly, bool *isemb)
{
	struct nvram_header *nvh;
	uint32 off, lim = SI_FLASH2_SZ;
	uint32 flbase = SI_FLASH2;
	int bootdev;
#ifdef NFLASH_SUPPORT
	hndnand_t *nfl_info = NULL;
#endif

	bootdev = soc_boot_dev((void *)sih);
#ifdef NFLASH_SUPPORT
	if (bootdev == SOC_BOOTDEV_NANDFLASH) {
		/* Init nand anyway */
		nfl_info = hndnand_init(sih);
		if (nfl_info)
			flbase = nfl_info->phybase;
	}
	else
#endif /* NFLASH_SUPPORT */
	if (bootdev == SOC_BOOTDEV_SFLASH) {
	if (sih->ccrev == 42)
		flbase = SI_NS_NORFLASH;
	}

	if (!embonly) {
		*isemb = FALSE;
#ifdef NFLASH_SUPPORT
		if (nfl_info) {
			uint32 blocksize;

			blocksize = nfl_info->blocksize;
			off = blocksize;
			for (; off < nfl_boot_size(nfl_info); off += blocksize) {
				if (hndnand_checkbadb(nfl_info, off) != 0)
					continue;
				nvh = (struct nvram_header *)OSL_UNCACHED(flbase + off);
				if (nvh->magic != NVRAM_MAGIC)
					continue;

				/* Read into the nand_nvram */
				if ((nvh = nand_find_nvram(nfl_info, off)) == NULL)
					continue;
				if (nvram_calc_crc(nvh) == (uint8)nvh->crc_ver_init)
					return nvh;
			}
		}
		else
#endif /* NFLASH_SUPPORT */
		{
			off = FLASH_MIN;
			while (off <= lim) {
				nvh = (struct nvram_header *)
					OSL_UNCACHED(flbase + off - MAX_NVRAM_SPACE);
				if (nvh->magic == NVRAM_MAGIC) {
					if (nvram_calc_crc(nvh) == (uint8) nvh->crc_ver_init) {
						return (nvh);
					}
				}
				off <<= 1;
			}
		}
#ifdef BCMDBG
		printf("find_nvram: nvram not found, trying embedded nvram next\n");
#endif /* BCMDBG */
	}

	/*
	 * Provide feedback to user when nvram corruption detected.
	 * Must be non-BCMDBG for customer release.
	 */
	printf("Corrupt NVRAM found, trying embedded NVRAM next.\n");

	/* Now check embedded nvram */
	*isemb = TRUE;
	nvh = (struct nvram_header *)OSL_UNCACHED(flbase + (4 * 1024));
	if (nvh->magic == NVRAM_MAGIC)
		return (nvh);
	nvh = (struct nvram_header *)OSL_UNCACHED(flbase + 1024);
	if (nvh->magic == NVRAM_MAGIC)
		return (nvh);
	printf("find_nvram: no nvram found\n");
	return (NULL);
}

int
BCMATTACHFN(nvram_init)(void *si)
{
	bool isemb;
	int ret;
	si_t *sih;
	static int nvram_status = -1;


	/* Check for previous 'restore defaults' condition */
	if (nvram_status == 1)
		return 1;

	/* Check whether nvram already initilized */
	if (nvram_status == 0 && !nvram_do_reset)
		return 0;

	sih = (si_t *)si;

	/* Restore defaults from embedded NVRAM if button held down */
	if (nvram_do_reset) {
		/* Initialize with embedded NVRAM */
		nvram_header = find_nvram(sih, TRUE, &isemb);
		ret = _nvram_init(si, 0);
		if (ret == 0) {
			nvram_status = 1;
			return 1;
		}
		nvram_status = -1;
		_nvram_exit();
	}

	/* Find NVRAM */
	nvram_header = find_nvram(sih, FALSE, &isemb);
	ret = _nvram_init(si, 0);
	if (ret == 0) {
		/* Restore defaults if embedded NVRAM used */
		if (nvram_header && isemb) {
			ret = 1;
		}
	}
	nvram_status = ret;
	return ret;
}

int
BCMINITFN(nvram_append)(void *si, char *vars, uint varsz)
{
	return 0;
}

void
BCMINITFN(nvram_exit)(void *si)
{
	si_t *sih;

	sih = (si_t *)si;

	_nvram_exit();
}

/* LZMA need to be able to allocate memory,
 * so set it up to use the OSL memory routines,
 * only the linux debug osl uses the osh on malloc and the osh and size on
 * free, and the debug code checks if they are valid, so pass NULL as the osh
 * to tell the OSL that we don't have a valid osh
 */
static void *SzAlloc(void *p, size_t size) { p = p; return MALLOC(NULL, size); }
static void SzFree(void *p, void *address) { p = p; MFREE(NULL, address, 0); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

int
BCMINITFN(_nvram_read)(void *buf, int idx)
{
	uint32 *src, *dst;
	uint i;

	if (!nvram_header)
		return -19; /* -ENODEV */

	src = (uint32 *) nvram_header;

	dst = (uint32 *) buf;

	for (i = 0; i < sizeof(struct nvram_header); i += 4)
		*dst++ = *src++;

	/* Since we know what the first 3 bytes of the lzma properties
	 * should be based on what we used to compress, check them
	 * to see if we need to decompress (uncompressed this would show up a
	 * a single [ and then the end of nvram marker so its invalid in an
	 * uncompressed nvram block
	 */
	if ((((unsigned char *)src)[0] == 0x5d) &&
	    (((unsigned char *)src)[1] == 0) &&
	    (((unsigned char *)src)[2] == 0)) {
		unsigned int dstlen = nvram_header->len;
		unsigned int srclen = MAX_NVRAM_SPACE-LZMA_PROPS_SIZE-NVRAM_HEADER_SIZE;
		unsigned char *cp = (unsigned char *)src;
		CLzmaDec state;
		SRes res;
		ELzmaStatus status;

		LzmaDec_Construct(&state);
		res = LzmaDec_Allocate(&state, cp, LZMA_PROPS_SIZE, &g_Alloc);
		if (res != SZ_OK) {
			printf("Error Initializing LZMA Library\n");
			return -19;
		}
		LzmaDec_Init(&state);
		res = LzmaDec_DecodeToBuf(&state,
		                          (unsigned char *)dst, &dstlen,
		                          &cp[LZMA_PROPS_SIZE], &srclen,
		                          LZMA_FINISH_ANY,
		                          &status);

		LzmaDec_Free(&state, &g_Alloc);
		if (res != SZ_OK) {
			printf("Error Decompressing eNVRAM\n");
			return -19;
		}
	} else {
		for (; i < nvram_header->len && i < MAX_NVRAM_SPACE; i += 4)
			*dst++ = ltoh32(*src++);
	}
	return 0;
}

struct nvram_tuple *
BCMINITFN(_nvram_realloc)(struct nvram_tuple *t, const char *name, const char *value)
{
	if (!(t = MALLOC(NULL, sizeof(struct nvram_tuple) + strlen(name) + 1 +
	                 strlen(value) + 1))) {
		printf("_nvram_realloc: our of memory\n");
		return NULL;
	}

	/* Copy name */
	t->name = (char *) &t[1];
	strcpy(t->name, name);

	/* Copy value */
	t->value = t->name + strlen(name) + 1;
	strcpy(t->value, value);

	return t;
}

void
BCMINITFN(_nvram_free)(struct nvram_tuple *t)
{
	if (t)
		MFREE(NULL, t, sizeof(struct nvram_tuple) + strlen(t->name) + 1 +
		      strlen(t->value) + 1);
}


int
BCMINITFN(nvram_commit_internal)(bool nvram_corrupt)
{
	struct nvram_header *header;
	int ret;
	uint32 *src, *dst;
	uint i;

	if (!(header = (struct nvram_header *) MALLOC(NULL, MAX_NVRAM_SPACE))) {
		printf("nvram_commit: out of memory\n");
		return -12; /* -ENOMEM */
	}

	NVRAM_LOCK();

	/* Regenerate NVRAM */
	ret = _nvram_commit(header);
	if (ret)
		goto done;

	src = (uint32 *) &header[1];
	dst = src;

	for (i = sizeof(struct nvram_header); i < header->len && i < MAX_NVRAM_SPACE; i += 4)
		*dst++ = htol32(*src++);

	if (sysFlashInit(NULL) == 0) {
		/* set/write invalid MAGIC # (in case writing image fails/is interrupted)
		 * write the NVRAM image to flash(with invalid magic)
		 * set/write valid MAGIC #
		 */
		header->magic = NVRAM_CLEAR_MAGIC;
		nvWriteChars((unsigned char *)&header->magic, sizeof(header->magic));

		header->magic = NVRAM_INVALID_MAGIC;
		nvWrite((unsigned short *) header, MAX_NVRAM_SPACE);

		header->magic = NVRAM_MAGIC;
		nvWriteChars((unsigned char *)&header->magic, sizeof(header->magic));
	}

done:
	NVRAM_UNLOCK();
	MFREE(NULL, header, MAX_NVRAM_SPACE);
	return ret;
}

int
BCMINITFN(nvram_commit)(void)
{
	/* do not corrupt nvram */
	return nvram_commit_internal(FALSE);
}
