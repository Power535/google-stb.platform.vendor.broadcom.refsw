/*
 * Linux OS Independent Layer
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
 * $Id: linux_osl.c 665699 2016-10-18 22:49:58Z $
 */

#define LINUX_PORT

#include <typedefs.h>
#include <bcmendian.h>
#include <linuxver.h>
#include <bcmdefs.h>

#ifdef mips
#include <asm/paccess.h>
#include <asm/cache.h>
#include <asm/r4kcache.h>
#undef ABS
#endif /* mips */

#if !defined(STBLINUX)
#ifdef __ARM_ARCH_7A__
#include <asm/cacheflush.h>
#endif /* __ARM_ARCH_7A__ */
#endif /* STBLINUX */

#include <linux/random.h>

#include <osl.h>
#include <bcmutils.h>
#include <linux/delay.h>
#include <pcicfg.h>



#if defined(BCM_BLOG)
#include <linux/nbuff.h>
#include <wl_blog.h>
#endif

#if defined(PKTC_TBL)
#include <wl_pktc.h>
#endif

#include <linux/fs.h>
#ifdef BCM_SECURE_DMA
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <asm/io.h>
#include <linux/skbuff.h>
#include <linux/vmalloc.h>
#include <stbutils.h>
#if !defined(__ARM_ARCH_7A__)
#include <linux/highmem.h>
#endif
#include <linux/dma-mapping.h>
#if defined(__ARM_ARCH_7A__)
#include <asm/memory.h>
#include <linux/brcmstb/cma_driver.h>
#endif /* __ARM_ARCH_7A__ */
#endif /* BCM_SECURE_DMA */
#if (defined(BCM47XX_CA9) || defined(STB))
#include <linux/spinlock.h>
extern spinlock_t l2x0_reg_lock;
#endif /* BCM47XX_CA9 */

#define PCI_CFG_RETRY		10

#define OS_HANDLE_MAGIC		0x1234abcd	/* Magic # to recognize osh */
#define BCM_MEM_FILENAME_LEN	24		/* Mem. filename length */
#define DUMPBUFSZ 1024

#ifdef CONFIG_DHD_USE_STATIC_BUF
#define DHD_SKB_HDRSIZE		336
#define DHD_SKB_1PAGE_BUFSIZE	((PAGE_SIZE*1)-DHD_SKB_HDRSIZE)
#define DHD_SKB_2PAGE_BUFSIZE	((PAGE_SIZE*2)-DHD_SKB_HDRSIZE)
#define DHD_SKB_4PAGE_BUFSIZE	((PAGE_SIZE*4)-DHD_SKB_HDRSIZE)

#define STATIC_BUF_MAX_NUM	16
#define STATIC_BUF_SIZE	(PAGE_SIZE*2)
#define STATIC_BUF_TOTAL_LEN	(STATIC_BUF_MAX_NUM * STATIC_BUF_SIZE)

typedef struct bcm_static_buf {
	struct semaphore static_sem;
	unsigned char *buf_ptr;
	unsigned char buf_use[STATIC_BUF_MAX_NUM];
} bcm_static_buf_t;

static bcm_static_buf_t *bcm_static_buf = 0;

#define STATIC_PKT_MAX_NUM	8
#if defined(ENHANCED_STATIC_BUF)
#define STATIC_PKT_4PAGE_NUM	1
#define DHD_SKB_MAX_BUFSIZE	DHD_SKB_4PAGE_BUFSIZE
#else
#define STATIC_PKT_4PAGE_NUM	0
#define DHD_SKB_MAX_BUFSIZE DHD_SKB_2PAGE_BUFSIZE
#endif /* ENHANCED_STATIC_BUF */

typedef struct bcm_static_pkt {
	struct sk_buff *skb_4k[STATIC_PKT_MAX_NUM];
	struct sk_buff *skb_8k[STATIC_PKT_MAX_NUM];
#ifdef ENHANCED_STATIC_BUF
	struct sk_buff *skb_16k;
#endif
	struct semaphore osl_pkt_sem;
	unsigned char pkt_use[STATIC_PKT_MAX_NUM * 2 + STATIC_PKT_4PAGE_NUM];
} bcm_static_pkt_t;

static bcm_static_pkt_t *bcm_static_skb = 0;

void* wifi_platform_prealloc(void *adapter, int section, unsigned long size);
#endif /* CONFIG_DHD_USE_STATIC_BUF */

typedef struct bcm_mem_link {
	struct bcm_mem_link *prev;
	struct bcm_mem_link *next;
	uint	size;
	int	line;
	void 	*osh;
	char	file[BCM_MEM_FILENAME_LEN];
} bcm_mem_link_t;

struct osl_cmn_info {
	atomic_t malloced;
	atomic_t pktalloced;    /* Number of allocated packet buffers */
	spinlock_t dbgmem_lock;
	bcm_mem_link_t *dbgmem_list;
#ifdef BCMDBG_PKT    /* pkt logging for debugging */
	spinlock_t pktlist_lock;
	pktlist_info_t pktlist;
#endif  /* BCMDBG_PKT */
	spinlock_t pktalloc_lock;
	atomic_t refcount; /* Number of references to this shared structure. */
};
typedef struct osl_cmn_info osl_cmn_t;

struct osl_info {
	osl_pubinfo_t pub;
	uint32  flags;		/* If specific cases to be handled in the OSL */
#ifdef CTFPOOL
	ctfpool_t *ctfpool;
#endif /* CTFPOOL */
	uint magic;
	void *pdev;
	uint failed;
	uint bustype;
	osl_cmn_t *cmn; /* Common OSL related data shred between two OSH's */

	void *bus_handle;
#ifdef BCMDBG_CTRACE
	spinlock_t ctrace_lock;
	struct list_head ctrace_list;
	int ctrace_num;
#endif /* BCMDBG_CTRACE */
#ifdef	BCM_SECURE_DMA
#ifdef NOT_YET
	struct sec_mem_elem *sec_list_512;
	struct sec_mem_elem *sec_list_base_512;
	struct sec_mem_elem *sec_list_2048;
	struct sec_mem_elem *sec_list_base_2048;
#endif /* NOT_YET */
	struct sec_mem_elem *sec_list_4096;
	struct sec_mem_elem *sec_list_base_4096;
	phys_addr_t  contig_base_alloc;
	phys_addr_t  contig_base;
	void *contig_base_alloc_va;
	void *contig_base_va;
	struct cma_dev *cma;
	struct device *dev;
	phys_addr_t contig_base_alloc_coherent;
	void *coherent_base_alloc_va;
	void *coherent_base_va;
	void *contig_delta_va_pa;
#endif /* BCM_SECURE_DMA */
};

#define OSL_PKTTAG_CLEAR(p) \
do { \
	struct sk_buff *s = (struct sk_buff *)(p); \
	ASSERT(OSL_PKTTAG_SZ == 32); \
	*(uint32 *)(&s->cb[0]) = 0; *(uint32 *)(&s->cb[4]) = 0; \
	*(uint32 *)(&s->cb[8]) = 0; *(uint32 *)(&s->cb[12]) = 0; \
	*(uint32 *)(&s->cb[16]) = 0; *(uint32 *)(&s->cb[20]) = 0; \
	*(uint32 *)(&s->cb[24]) = 0; *(uint32 *)(&s->cb[28]) = 0; \
} while (0)

/* PCMCIA attribute space access macros */
#if defined(CONFIG_PCMCIA)
struct pcmcia_dev {
	dev_link_t link;	/* PCMCIA device pointer */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
	dev_node_t node;	/* PCMCIA node structure */
#endif
	void *base;		/* Mapped attribute memory window */
	size_t size;		/* Size of window */
	void *drv;		/* Driver data */
};
#endif 

/* Global ASSERT type flag */
uint32 g_assert_type = 1;

#ifdef	BCM_SECURE_DMA
#define	SECDMA_MODULE_PARAMS	0
#define	SECDMA_EXT_FILE	1
#define	SECDMA_INTERNAL_CMA	2
int stb_ext_params = 2;
unsigned long secdma_addr = 0;
u32 secdma_size = 0;
module_param(secdma_addr, ulong, 0);
module_param(secdma_size, int, 0);
#endif
static int16 linuxbcmerrormap[] =
{	0, 			/* 0 */
	-EINVAL,		/* BCME_ERROR */
	-EINVAL,		/* BCME_BADARG */
	-EINVAL,		/* BCME_BADOPTION */
	-EINVAL,		/* BCME_NOTUP */
	-EINVAL,		/* BCME_NOTDOWN */
	-EINVAL,		/* BCME_NOTAP */
	-EINVAL,		/* BCME_NOTSTA */
	-EINVAL,		/* BCME_BADKEYIDX */
	-EINVAL,		/* BCME_RADIOOFF */
	-EINVAL,		/* BCME_NOTBANDLOCKED */
	-EINVAL, 		/* BCME_NOCLK */
	-EINVAL, 		/* BCME_BADRATESET */
	-EINVAL, 		/* BCME_BADBAND */
	-E2BIG,			/* BCME_BUFTOOSHORT */
	-E2BIG,			/* BCME_BUFTOOLONG */
	-EBUSY, 		/* BCME_BUSY */
	-EINVAL, 		/* BCME_NOTASSOCIATED */
	-EINVAL, 		/* BCME_BADSSIDLEN */
	-EINVAL, 		/* BCME_OUTOFRANGECHAN */
	-EINVAL, 		/* BCME_BADCHAN */
	-EFAULT, 		/* BCME_BADADDR */
	-ENOMEM, 		/* BCME_NORESOURCE */
	-EOPNOTSUPP,		/* BCME_UNSUPPORTED */
	-EMSGSIZE,		/* BCME_BADLENGTH */
	-EINVAL,		/* BCME_NOTREADY */
	-EPERM,			/* BCME_EPERM */
	-ENOMEM, 		/* BCME_NOMEM */
	-EINVAL, 		/* BCME_ASSOCIATED */
	-ERANGE, 		/* BCME_RANGE */
	-EINVAL, 		/* BCME_NOTFOUND */
	-EINVAL, 		/* BCME_WME_NOT_ENABLED */
	-EINVAL, 		/* BCME_TSPEC_NOTFOUND */
	-EINVAL, 		/* BCME_ACM_NOTSUPPORTED */
	-EINVAL,		/* BCME_NOT_WME_ASSOCIATION */
	-EIO,			/* BCME_SDIO_ERROR */
	-ENODEV,		/* BCME_DONGLE_DOWN */
	-EINVAL,		/* BCME_VERSION */
	-EIO,			/* BCME_TXFAIL */
	-EIO,			/* BCME_RXFAIL */
	-ENODEV,		/* BCME_NODEVICE */
	-EINVAL,		/* BCME_NMODE_DISABLED */
	-ENODATA,		/* BCME_NONRESIDENT */
	-EINVAL,		/* BCME_SCANREJECT */
	-EINVAL,		/* BCME_USAGE_ERROR */
	-EIO,     		/* BCME_IOCTL_ERROR */
	-EIO,			/* BCME_SERIAL_PORT_ERR */
	-EOPNOTSUPP,	/* BCME_DISABLED, BCME_NOTENABLED */
	-EIO,			/* BCME_DECERR */
	-EIO,			/* BCME_ENCERR */
	-EIO,			/* BCME_MICERR */
	-ERANGE,		/* BCME_REPLAY */
	-EINVAL,		/* BCME_IE_NOTFOUND */

/* When an new error code is added to bcmutils.h, add os
 * specific error translation here as well
 */
/* check if BCME_LAST changed since the last time this function was updated */
#if BCME_LAST != -52
#error "You need to add a OS error translation in the linuxbcmerrormap \
	for new error code defined in bcmutils.h"
#endif
};

/* translate bcmerrors into linux errors */
int
osl_error(int bcmerror)
{
	if (bcmerror > 0)
		bcmerror = 0;
	else if (bcmerror < BCME_LAST)
		bcmerror = BCME_ERROR;

	/* Array bounds covered by ASSERT in osl_attach */
	return linuxbcmerrormap[-bcmerror];
}
#ifdef SHARED_OSL_CMN
osl_t *
osl_attach(void *pdev, uint bustype, bool pkttag, void **osl_cmn)
{
#else
osl_t *
osl_attach(void *pdev, uint bustype, bool pkttag)
{
	void **osl_cmn = NULL;
#endif /* SHARED_OSL_CMN */
	osl_t *osh;
	gfp_t flags;

#ifdef BCM_SECURE_DMA
	u32 secdma_memsize;
#endif
	flags = CAN_SLEEP() ? GFP_KERNEL: GFP_ATOMIC;
	if (!(osh = kmalloc(sizeof(osl_t), flags)))
		return osh;

	ASSERT(osh);

	bzero(osh, sizeof(osl_t));

	if (osl_cmn == NULL || *osl_cmn == NULL) {
		if (!(osh->cmn = kmalloc(sizeof(osl_cmn_t), flags))) {
			kfree(osh);
			return NULL;
		}
		bzero(osh->cmn, sizeof(osl_cmn_t));
		if (osl_cmn)
			*osl_cmn = osh->cmn;
		atomic_set(&osh->cmn->malloced, 0);
		osh->cmn->dbgmem_list = NULL;
		spin_lock_init(&(osh->cmn->dbgmem_lock));

#ifdef BCMDBG_PKT
		spin_lock_init(&(osh->cmn->pktlist_lock));
#endif
		spin_lock_init(&(osh->cmn->pktalloc_lock));

	} else {
		osh->cmn = *osl_cmn;
	}
	atomic_add(1, &osh->cmn->refcount);

	/* Check that error map has the right number of entries in it */
	ASSERT(ABS(BCME_LAST) == (ARRAYSIZE(linuxbcmerrormap) - 1));

	osh->failed = 0;
	osh->pdev = pdev;
	osh->pub.pkttag = pkttag;
	osh->bustype = bustype;
	osh->magic = OS_HANDLE_MAGIC;

#ifdef BCM_SECURE_DMA
	if ((secdma_addr != 0) && (secdma_size != 0)) {
		printk("linux_osl.c: CMA info passed via module params, using it.\n");
		osh->contig_base_alloc = secdma_addr;
		secdma_memsize = secdma_size;
		osh->contig_base = osh->contig_base_alloc;
		printf("linux_osl.c: secdma_cma_size = 0x%x\n", secdma_memsize);
		printf("linux_osl.c: secdma_cma_addr = 0x%" PRI_FMT_X "\n", osh->contig_base_alloc);
		stb_ext_params = SECDMA_MODULE_PARAMS;
	}
	else if (stbpriv_init(osh) == 0) {
		printk("linux_osl.c: stbpriv.txt found. Get CMA mem info.\n");
		osh->contig_base_alloc = bcm_strtoul(stbparam_get("secdma_cma_addr"), NULL, 0);
		secdma_memsize = bcm_strtoul(stbparam_get("secdma_cma_size"), NULL, 0);
		ASSERT(secdma_memsize);
		osh->contig_base = osh->contig_base_alloc;
		printf("linux_osl.c: secdma_cma_size = 0x%x\n", secdma_memsize);
		printf("linux_osl.c: secdma_cma_addr = 0x%" PRI_FMT_X "\n", osh->contig_base_alloc);
		stb_ext_params = SECDMA_EXT_FILE;
	}
	else {
		printk("linux_osl.c: No stbpriv.txt found, allocate internally.\n");
		if (BCME_OK != osl_sec_dma_setup_contig_mem(osh, CMA_MEMBLOCK, CONT_REGION)) {
		        kfree(osh);
			return NULL;
		}
		stb_ext_params = SECDMA_INTERNAL_CMA;
	}
	ASSERT(osh->contig_base_alloc);
#ifdef BCM47XX_CA9
	osh->coherent_base_alloc_va = osl_sec_dma_ioremap(osh,
		PHYS_TO_PAGE(osh->contig_base_alloc),
		CMA_DMA_DESC_MEMBLOCK, TRUE, TRUE);
#else
	osh->coherent_base_alloc_va = osl_sec_dma_ioremap(osh,
		PHYS_TO_PAGE(osh->contig_base_alloc),
		CMA_DMA_DESC_MEMBLOCK, FALSE, TRUE);
#endif /* BCM47XX_CA9 */

	if (osh->coherent_base_alloc_va == NULL) {
	    osl_sec_dma_free_contig_mem(osh, CMA_MEMBLOCK, CONT_REGION);
		if (osh->cmn)
			kfree(osh->cmn);
	    kfree(osh);
	    return NULL;
	}
	osh->coherent_base_va = osh->coherent_base_alloc_va;
	osh->contig_base_alloc_coherent = osh->contig_base_alloc;

	osh->contig_base_alloc += CMA_DMA_DESC_MEMBLOCK;

	osh->contig_base_alloc_va = osl_sec_dma_ioremap(osh,
		PHYS_TO_PAGE(osh->contig_base_alloc), CMA_DMA_DATA_MEMBLOCK, TRUE, FALSE);

	if (osh->contig_base_alloc_va == NULL) {
		osl_sec_dma_iounmap(osh, osh->coherent_base_va, CMA_DMA_DESC_MEMBLOCK);
		osl_sec_dma_free_contig_mem(osh, CMA_MEMBLOCK, CONT_REGION);
		if (osh->cmn)
			kfree(osh->cmn);
		kfree(osh);
		return NULL;
	}
	osh->contig_base_va = osh->contig_base_alloc_va;

#ifdef NOT_YET
	/*
	* osl_sec_dma_init_elem_mem_block(osh, CMA_BUFSIZE_512, CMA_BUFNUM, &osh->sec_list_512);
	* osh->sec_list_base_512 = osh->sec_list_512;
	* osl_sec_dma_init_elem_mem_block(osh, CMA_BUFSIZE_2K, CMA_BUFNUM, &osh->sec_list_2048);
	* osh->sec_list_base_2048 = osh->sec_list_2048;
	*/
#endif /* NOT_YET */

	if (BCME_OK != osl_sec_dma_init_elem_mem_block(osh,
		CMA_BUFSIZE_4K, CMA_BUFNUM, &osh->sec_list_4096)) {
	    osl_sec_dma_iounmap(osh, osh->coherent_base_va, CMA_DMA_DESC_MEMBLOCK);
	    osl_sec_dma_iounmap(osh, osh->contig_base_va, CMA_DMA_DATA_MEMBLOCK);
	    osl_sec_dma_free_contig_mem(osh, CMA_MEMBLOCK, CONT_REGION);
	    if (osh->cmn)
			kfree(osh->cmn);
		kfree(osh);
		return NULL;
	}
	osh->sec_list_base_4096 = osh->sec_list_4096;

#endif /* BCM_SECURE_DMA */

	switch (bustype) {
		case PCI_BUS:
		case SI_BUS:
		case PCMCIA_BUS:
			osh->pub.mmbus = TRUE;
			break;
		case JTAG_BUS:
		case SDIO_BUS:
		case USB_BUS:
		case SPI_BUS:
		case RPC_BUS:
			osh->pub.mmbus = FALSE;
			break;
		default:
			ASSERT(FALSE);
			break;
	}

#ifdef BCMDBG_CTRACE
	spin_lock_init(&osh->ctrace_lock);
	INIT_LIST_HEAD(&osh->ctrace_list);
	osh->ctrace_num = 0;
#endif /* BCMDBG_CTRACE */


#ifdef BCMDBG_ASSERT
	if (pkttag) {
		struct sk_buff *skb;
		BCM_REFERENCE(skb);
		ASSERT(OSL_PKTTAG_SZ <= sizeof(skb->cb));
	}
#endif
	return osh;
}

int osl_static_mem_init(osl_t *osh, void *adapter)
{
#if defined(CONFIG_DHD_USE_STATIC_BUF)
		if (!bcm_static_buf && adapter) {
			if (!(bcm_static_buf = (bcm_static_buf_t *)wifi_platform_prealloc(adapter,
				3, STATIC_BUF_SIZE + STATIC_BUF_TOTAL_LEN))) {
				printk("can not alloc static buf!\n");
				bcm_static_skb = NULL;
				ASSERT(osh->magic == OS_HANDLE_MAGIC);
				kfree(osh);
				return -ENOMEM;
			}
			else
				printk("alloc static buf at %x!\n", (unsigned int)bcm_static_buf);


			sema_init(&bcm_static_buf->static_sem, 1);

			bcm_static_buf->buf_ptr = (unsigned char *)bcm_static_buf + STATIC_BUF_SIZE;
		}

		if (!bcm_static_skb && adapter) {
			int i;
			void *skb_buff_ptr = 0;
			bcm_static_skb = (bcm_static_pkt_t *)((char *)bcm_static_buf + 2048);
			skb_buff_ptr = wifi_platform_prealloc(adapter, 4, 0);
			if (!skb_buff_ptr) {
				printk("cannot alloc static buf!\n");
				bcm_static_buf = NULL;
				bcm_static_skb = NULL;
				ASSERT(osh->magic == OS_HANDLE_MAGIC);
				kfree(osh);
				return -ENOMEM;
			}

			bcopy(skb_buff_ptr, bcm_static_skb, sizeof(struct sk_buff *) *
				(STATIC_PKT_MAX_NUM * 2 + STATIC_PKT_4PAGE_NUM));
			for (i = 0; i < STATIC_PKT_MAX_NUM * 2 + STATIC_PKT_4PAGE_NUM; i++)
				bcm_static_skb->pkt_use[i] = 0;

			sema_init(&bcm_static_skb->osl_pkt_sem, 1);
		}
#endif /* CONFIG_DHD_USE_STATIC_BUF */

	return 0;
}

void osl_set_bus_handle(osl_t *osh, void *bus_handle)
{
	osh->bus_handle = bus_handle;
}

void* osl_get_bus_handle(osl_t *osh)
{
	return osh->bus_handle;
}

void
osl_detach(osl_t *osh)
{
	if (osh == NULL)
		return;

#ifdef BCM_SECURE_DMA
	if (stb_ext_params == SECDMA_EXT_FILE)
		stbpriv_exit(osh);
	else if (stb_ext_params == SECDMA_INTERNAL_CMA)
		osl_sec_dma_free_contig_mem(osh, CMA_MEMBLOCK, CONT_REGION);
#ifdef NOT_YET
	osl_sec_dma_deinit_elem_mem_block(osh, CMA_BUFSIZE_512, CMA_BUFNUM, osh->sec_list_base_512);
	osl_sec_dma_deinit_elem_mem_block(osh, CMA_BUFSIZE_2K, CMA_BUFNUM, osh->sec_list_base_2048);
#endif /* NOT_YET */
	osl_sec_dma_deinit_elem_mem_block(osh, CMA_BUFSIZE_4K, CMA_BUFNUM, osh->sec_list_base_4096);
	osl_sec_dma_iounmap(osh, osh->coherent_base_va, CMA_DMA_DESC_MEMBLOCK);
	osl_sec_dma_iounmap(osh, osh->contig_base_va, CMA_DMA_DATA_MEMBLOCK);
#endif /* BCM_SECURE_DMA */

#ifdef BCMDBG_MEM
	if (MEMORY_LEFTOVER(osh)) {
		static char dumpbuf[DUMPBUFSZ];
		struct bcmstrbuf b;

		printf("%s: MEMORY LEAK %d bytes\n", __FUNCTION__, MALLOCED(osh));
		bcm_binit(&b, dumpbuf, DUMPBUFSZ);
		MALLOC_DUMP(osh, &b);
		printf("%s", b.origbuf);
	}
#endif
	ASSERT(osh->magic == OS_HANDLE_MAGIC);
	atomic_sub(1, &osh->cmn->refcount);
	if (atomic_read(&osh->cmn->refcount) == 0) {
			kfree(osh->cmn);
	}
	kfree(osh);
}

int osl_static_mem_deinit(osl_t *osh, void *adapter)
{
#ifdef CONFIG_DHD_USE_STATIC_BUF
	if (bcm_static_buf) {
		bcm_static_buf = 0;
	}
	if (bcm_static_skb) {
		bcm_static_skb = 0;
	}
#endif
	return 0;
}

/* APIs to set/get specific quirks in OSL layer */
void BCMFASTPATH
osl_flag_set(osl_t *osh, uint32 mask)
{
	osh->flags |= mask;
}

void
osl_flag_clr(osl_t *osh, uint32 mask)
{
	osh->flags &= ~mask;
}

#if (defined(BCM47XX_CA9) || defined(STB))
inline bool BCMFASTPATH
#else
bool
#endif /* BCM47XX_CA9 || STB */
osl_is_flag_set(osl_t *osh, uint32 mask)
{
	return (osh->flags & mask);
}

#if defined(mips)
inline void BCMFASTPATH
osl_cache_flush(void *va, uint size)
{
	unsigned long l = ROUNDDN((unsigned long)va, L1_CACHE_BYTES);
	unsigned long e = ROUNDUP((unsigned long)(va+size), L1_CACHE_BYTES);
	while (l < e)
	{
		flush_dcache_line(l);                         /* Hit_Writeback_Inv_D  */
		l += L1_CACHE_BYTES;                          /* next cache line base */
	}
}

inline void BCMFASTPATH
osl_cache_inv(void *va, uint size)
{
	unsigned long l = ROUNDDN((unsigned long)va, L1_CACHE_BYTES);
	unsigned long e = ROUNDUP((unsigned long)(va+size), L1_CACHE_BYTES);
	while (l < e)
	{
		invalidate_dcache_line(l);                    /* Hit_Invalidate_D     */
		l += L1_CACHE_BYTES;                          /* next cache line base */
	}
}

inline void BCMFASTPATH
osl_prefetch(const void *ptr)
{
	__asm__ __volatile__(
		".set mips4\npref %0,(%1)\n.set mips0\n" :: "i" (Pref_Load), "r" (ptr));
}

#elif defined(__ARM_ARCH_7A__)

inline int BCMFASTPATH
osl_arch_is_coherent(void)
{
#ifdef BCM47XX_CA9
	return arch_is_coherent();
#else
	return 0;
#endif
}

inline int BCMFASTPATH
osl_acp_war_enab(void)
{
#ifdef BCM47XX_CA9
	return ACP_WAR_ENAB();
#else
	return 0;
#endif /* BCM47XX_CA9 */
}

inline void BCMFASTPATH
osl_cache_flush(void *va, uint size)
{
#ifdef BCM47XX_CA9
	if (arch_is_coherent() || (ACP_WAR_ENAB() && (virt_to_phys(va) < ACP_WIN_LIMIT)))
		return;
#endif /* BCM47XX_CA9 */

	dma_sync_single_for_device(OSH_NULL, virt_to_dma(OSH_NULL, va), size, DMA_TO_DEVICE);
}

inline void BCMFASTPATH
osl_cache_inv(void *va, uint size)
{
#ifdef BCM47XX_CA9
	if (arch_is_coherent() || (ACP_WAR_ENAB() && (virt_to_phys(va) < ACP_WIN_LIMIT)))
		return;
#endif /* BCM47XX_CA9 */

	dma_sync_single_for_cpu(OSH_NULL, virt_to_dma(OSH_NULL, va), size, DMA_FROM_DEVICE);
}

inline void BCMFASTPATH
osl_prefetch(const void *ptr)
{
	__asm__ __volatile__("pld\t%0" :: "o"(*(char *)ptr) : "cc");
}

#endif /* !mips && !__ARM_ARCH_7A__ */

/*
 * To avoid ACP latency, a fwder buf will be sent directly to DDR using
 * DDR aliasing into non-ACP address space. Such Fwder buffers must be
 * explicitly managed from a coherency perspective.
 */
static inline void BCMFASTPATH
osl_fwderbuf_reset(osl_t *osh, struct sk_buff *skb)
{
}

static struct sk_buff *osl_alloc_skb(osl_t *osh, unsigned int len)
{
	struct sk_buff *skb;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25)
	gfp_t flags = (in_atomic() || irqs_disabled()) ? GFP_ATOMIC : GFP_KERNEL;
#if defined(CONFIG_SPARSEMEM) && defined(CONFIG_ZONE_DMA)
	flags |= GFP_ATOMIC;
#endif
	skb = __dev_alloc_skb(len, flags);
#else
	skb = dev_alloc_skb(len);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25) */
#ifdef CTFMAP
	if (skb) {
		_DMA_MAP(osh, PKTDATA(osh, skb), len, DMA_RX, NULL, NULL);
	}
#endif /* CTFMAP */
	return skb;
}

#ifdef CTFPOOL

#ifdef CTFPOOL_SPINLOCK
#define CTFPOOL_LOCK(ctfpool, flags)	spin_lock_irqsave(&(ctfpool)->lock, flags)
#define CTFPOOL_UNLOCK(ctfpool, flags)	spin_unlock_irqrestore(&(ctfpool)->lock, flags)
#else
#define CTFPOOL_LOCK(ctfpool, flags)	spin_lock_bh(&(ctfpool)->lock)
#define CTFPOOL_UNLOCK(ctfpool, flags)	spin_unlock_bh(&(ctfpool)->lock)
#endif /* CTFPOOL_SPINLOCK */
/*
 * Allocate and add an object to packet pool.
 */
void *
osl_ctfpool_add(osl_t *osh)
{
	struct sk_buff *skb;
#ifdef CTFPOOL_SPINLOCK
	unsigned long flags;
#endif /* CTFPOOL_SPINLOCK */

	if ((osh == NULL) || (osh->ctfpool == NULL))
		return NULL;

	CTFPOOL_LOCK(osh->ctfpool, flags);
	ASSERT(osh->ctfpool->curr_obj <= osh->ctfpool->max_obj);

	/* No need to allocate more objects */
	if (osh->ctfpool->curr_obj == osh->ctfpool->max_obj) {
		CTFPOOL_UNLOCK(osh->ctfpool, flags);
		return NULL;
	}

	/* Allocate a new skb and add it to the ctfpool */
	skb = osl_alloc_skb(osh, osh->ctfpool->obj_size);
	if (skb == NULL) {
		printf("%s: skb alloc of len %d failed\n", __FUNCTION__,
		       osh->ctfpool->obj_size);
		CTFPOOL_UNLOCK(osh->ctfpool, flags);
		return NULL;
	}

	/* Add to ctfpool */
	skb->next = (struct sk_buff *)osh->ctfpool->head;
	osh->ctfpool->head = skb;
	osh->ctfpool->fast_frees++;
	osh->ctfpool->curr_obj++;

	/* Hijack a skb member to store ptr to ctfpool */
	CTFPOOLPTR(osh, skb) = (void *)osh->ctfpool;

	/* Use bit flag to indicate skb from fast ctfpool */
	PKTFAST(osh, skb) = FASTBUF;

	/* If ctfpool's osh is a fwder osh, reset the fwder buf */
	osl_fwderbuf_reset(osh->ctfpool->osh, skb);

	CTFPOOL_UNLOCK(osh->ctfpool, flags);

	return skb;
}

/*
 * Add new objects to the pool.
 */
void
osl_ctfpool_replenish(osl_t *osh, uint thresh)
{
	if ((osh == NULL) || (osh->ctfpool == NULL))
		return;

	/* Do nothing if no refills are required */
	while ((osh->ctfpool->refills > 0) && (thresh--)) {
		osl_ctfpool_add(osh);
		osh->ctfpool->refills--;
	}
}

/*
 * Initialize the packet pool with specified number of objects.
 */
int32
osl_ctfpool_init(osl_t *osh, uint numobj, uint size)
{
	gfp_t flags;

	flags = CAN_SLEEP() ? GFP_KERNEL: GFP_ATOMIC;
	osh->ctfpool = kzalloc(sizeof(ctfpool_t), flags);
	ASSERT(osh->ctfpool);

	osh->ctfpool->osh = osh;

	osh->ctfpool->max_obj = numobj;
	osh->ctfpool->obj_size = size;

	spin_lock_init(&osh->ctfpool->lock);

	while (numobj--) {
		if (!osl_ctfpool_add(osh))
			return -1;
		osh->ctfpool->fast_frees--;
	}

	return 0;
}

/*
 * Cleanup the packet pool objects.
 */
void
osl_ctfpool_cleanup(osl_t *osh)
{
	struct sk_buff *skb, *nskb;
#ifdef CTFPOOL_SPINLOCK
	unsigned long flags;
#endif /* CTFPOOL_SPINLOCK */

	if ((osh == NULL) || (osh->ctfpool == NULL))
		return;

	CTFPOOL_LOCK(osh->ctfpool, flags);

	skb = osh->ctfpool->head;

	while (skb != NULL) {
		nskb = skb->next;
		dev_kfree_skb(skb);
		skb = nskb;
		osh->ctfpool->curr_obj--;
	}

	ASSERT(osh->ctfpool->curr_obj == 0);
	osh->ctfpool->head = NULL;
	CTFPOOL_UNLOCK(osh->ctfpool, flags);

	kfree(osh->ctfpool);
	osh->ctfpool = NULL;
}

void
osl_ctfpool_stats(osl_t *osh, void *b)
{
	struct bcmstrbuf *bb;

	if ((osh == NULL) || (osh->ctfpool == NULL))
		return;

#ifdef CONFIG_DHD_USE_STATIC_BUF
	if (bcm_static_buf) {
		bcm_static_buf = 0;
	}
	if (bcm_static_skb) {
		bcm_static_skb = 0;
	}
#endif /* CONFIG_DHD_USE_STATIC_BUF */

	bb = b;

	ASSERT((osh != NULL) && (bb != NULL));

	bcm_bprintf(bb, "max_obj %d obj_size %d curr_obj %d refills %d\n",
	            osh->ctfpool->max_obj, osh->ctfpool->obj_size,
	            osh->ctfpool->curr_obj, osh->ctfpool->refills);
	bcm_bprintf(bb, "fast_allocs %d fast_frees %d slow_allocs %d\n",
	            osh->ctfpool->fast_allocs, osh->ctfpool->fast_frees,
	            osh->ctfpool->slow_allocs);
}

static inline struct sk_buff *
osl_pktfastget(osl_t *osh, uint len)
{
	struct sk_buff *skb;
#ifdef CTFPOOL_SPINLOCK
	unsigned long flags;
#endif /* CTFPOOL_SPINLOCK */

	/* Try to do fast allocate. Return null if ctfpool is not in use
	 * or if there are no items in the ctfpool.
	 */
	if (osh->ctfpool == NULL)
		return NULL;

	CTFPOOL_LOCK(osh->ctfpool, flags);
	if (osh->ctfpool->head == NULL) {
		ASSERT(osh->ctfpool->curr_obj == 0);
		osh->ctfpool->slow_allocs++;
		CTFPOOL_UNLOCK(osh->ctfpool, flags);
		return NULL;
	}

	if (len > osh->ctfpool->obj_size) {
		osh->ctfpool->slow_allocs++;
		CTFPOOL_UNLOCK(osh->ctfpool, flags);
		return NULL;
	}

	ASSERT(len <= osh->ctfpool->obj_size);

	/* Get an object from ctfpool */
	skb = (struct sk_buff *)osh->ctfpool->head;
	osh->ctfpool->head = (void *)skb->next;

	osh->ctfpool->fast_allocs++;
	osh->ctfpool->curr_obj--;
	ASSERT(CTFPOOLHEAD(osh, skb) == (struct sock *)osh->ctfpool->head);
	CTFPOOL_UNLOCK(osh->ctfpool, flags);

	/* Init skb struct */
	skb->next = skb->prev = NULL;
#if defined(__ARM_ARCH_7A__)
	skb->data = skb->head + NET_SKB_PAD;
	skb->tail = skb->head + NET_SKB_PAD;
#else
	skb->data = skb->head + 16;
	skb->tail = skb->head + 16;
#endif /* __ARM_ARCH_7A__ */
	skb->len = 0;
	skb->cloned = 0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 14)
	skb->list = NULL;
#endif
	atomic_set(&skb->users, 1);

	PKTSETCLINK(skb, NULL);
	PKTCCLRATTR(skb);
	PKTFAST(osh, skb) &= ~(CTFBUF | SKIPCT | CHAINED);

	return skb;
}
#endif /* CTFPOOL */


/* Convert a driver packet to native(OS) packet
 * In the process, packettag is zeroed out before sending up
 * IP code depends on skb->cb to be setup correctly with various options
 * In our case, that means it should be 0
 */
struct sk_buff * BCMFASTPATH
osl_pkt_tonative(osl_t *osh, void *pkt)
{
	struct sk_buff *nskb;
#ifdef BCMDBG_CTRACE
	struct sk_buff *nskb1, *nskb2;
#endif
#ifdef BCMDBG_PKT
	unsigned long flags;
#endif

	if (osh->pub.pkttag)
		OSL_PKTTAG_CLEAR(pkt);

	/* Decrement the packet counter */
	for (nskb = (struct sk_buff *)pkt; nskb; nskb = nskb->next) {
#ifdef BCMDBG_PKT
		spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
		pktlist_remove(&(osh->cmn->pktlist), (void *) nskb);
		spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
#endif  /* BCMDBG_PKT */
		atomic_sub(PKTISCHAINED(nskb) ? PKTCCNT(nskb) : 1, &osh->cmn->pktalloced);

#ifdef BCMDBG_CTRACE
		for (nskb1 = nskb; nskb1 != NULL; nskb1 = nskb2) {
			if (PKTISCHAINED(nskb1)) {
				nskb2 = PKTCLINK(nskb1);
			}
			else
				nskb2 = NULL;

			DEL_CTRACE(osh, nskb1);
		}
#endif /* BCMDBG_CTRACE */
	}
	return (struct sk_buff *)pkt;
}

/* Convert a native(OS) packet to driver packet.
 * In the process, native packet is destroyed, there is no copying
 * Also, a packettag is zeroed out
 */
#ifdef BCMDBG_PKT
void *
osl_pkt_frmnative(osl_t *osh, void *pkt, int line, char *file)
#else /* BCMDBG_PKT pkt logging for debugging */
#ifdef BCMDBG_CTRACE
void * BCMFASTPATH
osl_pkt_frmnative(osl_t *osh, void *pkt, int line, char *file)
#else
void * BCMFASTPATH
osl_pkt_frmnative(osl_t *osh, void *pkt)
#endif /* BCMDBG_CTRACE */
#endif /* BCMDBG_PKT */
{
	struct sk_buff *nskb;
#ifdef BCMDBG_CTRACE
	struct sk_buff *nskb1, *nskb2;
#endif
#ifdef BCMDBG_PKT
	unsigned long flags;
#endif

	if (osh->pub.pkttag)
		OSL_PKTTAG_CLEAR(pkt);

	/* Increment the packet counter */
	for (nskb = (struct sk_buff *)pkt; nskb; nskb = nskb->next) {
#ifdef BCMDBG_PKT
		spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
		pktlist_add(&(osh->cmn->pktlist), (void *) nskb, line, file);
		spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
#endif  /* BCMDBG_PKT */
		atomic_add(PKTISCHAINED(nskb) ? PKTCCNT(nskb) : 1, &osh->cmn->pktalloced);

#ifdef BCMDBG_CTRACE
		for (nskb1 = nskb; nskb1 != NULL; nskb1 = nskb2) {
			if (PKTISCHAINED(nskb1)) {
				nskb2 = PKTCLINK(nskb1);
			}
			else
				nskb2 = NULL;

			ADD_CTRACE(osh, nskb1, file, line);
		}
#endif /* BCMDBG_CTRACE */
	}
	return (void *)pkt;
}

/* Return a new packet. zero out pkttag */
#ifdef BCMDBG_PKT
void * BCMFASTPATH
osl_pktget(osl_t *osh, uint len, int line, char *file)
#else /* BCMDBG_PKT */
#ifdef BCMDBG_CTRACE
void * BCMFASTPATH
osl_pktget(osl_t *osh, uint len, int line, char *file)
#else
void * BCMFASTPATH
osl_pktget(osl_t *osh, uint len)
#endif /* BCMDBG_CTRACE */
#endif /* BCMDBG_PKT */
{
	struct sk_buff *skb;
#ifdef BCMDBG_PKT
	unsigned long flags;
#endif

#ifdef CTFPOOL
	/* Allocate from local pool */
	skb = osl_pktfastget(osh, len);
	if ((skb != NULL) || ((skb = osl_alloc_skb(osh, len)) != NULL)) {
#else /* CTFPOOL */
	if ((skb = osl_alloc_skb(osh, len))) {
#endif /* CTFPOOL */
#ifdef BCMDBG
		skb_put(skb, len);
#else
		skb->tail += len;
		skb->len  += len;
#endif
		skb->priority = 0;

#ifdef BCMDBG_CTRACE
		ADD_CTRACE(osh, skb, file, line);
#endif
#ifdef BCMDBG_PKT
		spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
		pktlist_add(&(osh->cmn->pktlist), (void *) skb, line, file);
		spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
#endif
		atomic_inc(&osh->cmn->pktalloced);
	}

	return ((void*) skb);
}

#ifdef CTFPOOL
static inline void
osl_pktfastfree(osl_t *osh, struct sk_buff *skb)
{
	ctfpool_t *ctfpool;
#ifdef CTFPOOL_SPINLOCK
	unsigned long flags;
#endif /* CTFPOOL_SPINLOCK */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 14)
	skb->tstamp.tv.sec = 0;
#else
	skb->stamp.tv_sec = 0;
#endif

	/* We only need to init the fields that we change */
	skb->dev = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
	skb->dst = NULL;
#endif
	OSL_PKTTAG_CLEAR(skb);
	skb->ip_summed = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
	skb_orphan(skb);
#else
	skb->destructor = NULL;
#endif

	ctfpool = (ctfpool_t *)CTFPOOLPTR(osh, skb);
	ASSERT(ctfpool != NULL);

	/* if osh is a fwder osh, reset the fwder buf */
	osl_fwderbuf_reset(ctfpool->osh, skb);

	/* Add object to the ctfpool */
	CTFPOOL_LOCK(ctfpool, flags);
	skb->next = (struct sk_buff *)ctfpool->head;
	ctfpool->head = (void *)skb;

	ctfpool->fast_frees++;
	ctfpool->curr_obj++;

	ASSERT(ctfpool->curr_obj <= ctfpool->max_obj);
	CTFPOOL_UNLOCK(ctfpool, flags);
}
#endif /* CTFPOOL */

/* Free the driver packet. Free the tag if present */
void BCMFASTPATH
osl_pktfree(osl_t *osh, void *p, bool send)
{
	struct sk_buff *skb, *nskb;
#ifdef BCMDBG_PKT
	unsigned long flags;
#endif
	if (osh == NULL)
		return;

	skb = (struct sk_buff*) p;

	if (send && osh->pub.tx_fn)
		osh->pub.tx_fn(osh->pub.tx_ctx, p, 0);

	PKTDBG_TRACE(osh, (void *) skb, PKTLIST_PKTFREE);

	/* perversion: we use skb->next to chain multi-skb packets */
	while (skb) {
		nskb = skb->next;
		skb->next = NULL;

#ifdef BCMDBG_CTRACE
		DEL_CTRACE(osh, skb);
#endif
#ifdef BCMDBG_PKT
		spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
		pktlist_remove(&(osh->cmn->pktlist), (void *) skb);
		spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
#endif

#ifdef CTFMAP
		/* Clear the map ptr before freeing */
		PKTCLRCTF(osh, skb);
		CTFMAPPTR(osh, skb) = NULL;
#endif

#ifdef CTFPOOL
		if (PKTISFAST(osh, skb)) {
			if (atomic_read(&skb->users) == 1)
				smp_rmb();
			else if (!atomic_dec_and_test(&skb->users))
				goto next_skb;
			osl_pktfastfree(osh, skb);
		} else
#endif
		{
			if (skb->destructor)
				/* cannot kfree_skb() on hard IRQ (net/core/skbuff.c) if
				 * destructor exists
				 */
				dev_kfree_skb_any(skb);
			else
				/* can free immediately (even in_irq()) if destructor
				 * does not exist
				 */
				dev_kfree_skb(skb);
		}
#ifdef CTFPOOL
next_skb:
#endif
		atomic_dec(&osh->cmn->pktalloced);
		skb = nskb;
	}
}

#ifdef CONFIG_DHD_USE_STATIC_BUF
void*
osl_pktget_static(osl_t *osh, uint len)
{
	int i = 0;
	struct sk_buff *skb;

	if (len > DHD_SKB_MAX_BUFSIZE) {
		printk("%s: attempt to allocate huge packet (0x%x)\n", __FUNCTION__, len);
		return osl_pktget(osh, len);
	}

	down(&bcm_static_skb->osl_pkt_sem);

	if (len <= DHD_SKB_1PAGE_BUFSIZE) {
		for (i = 0; i < STATIC_PKT_MAX_NUM; i++) {
			if (bcm_static_skb->pkt_use[i] == 0)
				break;
		}

		if (i != STATIC_PKT_MAX_NUM) {
			bcm_static_skb->pkt_use[i] = 1;

			skb = bcm_static_skb->skb_4k[i];
			skb->tail = skb->data + len;
			skb->len = len;

			up(&bcm_static_skb->osl_pkt_sem);
			return skb;
		}
	}

	if (len <= DHD_SKB_2PAGE_BUFSIZE) {
		for (i = 0; i < STATIC_PKT_MAX_NUM; i++) {
			if (bcm_static_skb->pkt_use[i + STATIC_PKT_MAX_NUM]
				== 0)
				break;
		}

		if (i != STATIC_PKT_MAX_NUM) {
			bcm_static_skb->pkt_use[i + STATIC_PKT_MAX_NUM] = 1;
			skb = bcm_static_skb->skb_8k[i];
			skb->tail = skb->data + len;
			skb->len = len;

			up(&bcm_static_skb->osl_pkt_sem);
			return skb;
		}
	}

#if defined(ENHANCED_STATIC_BUF)
	if (bcm_static_skb->pkt_use[STATIC_PKT_MAX_NUM * 2] == 0) {
		bcm_static_skb->pkt_use[STATIC_PKT_MAX_NUM * 2] = 1;

		skb = bcm_static_skb->skb_16k;
		skb->tail = skb->data + len;
		skb->len = len;

		up(&bcm_static_skb->osl_pkt_sem);
		return skb;
	}
#endif

	up(&bcm_static_skb->osl_pkt_sem);
	printk("%s: all static pkt in use!\n", __FUNCTION__);
	return osl_pktget(osh, len);
}

void
osl_pktfree_static(osl_t *osh, void *p, bool send)
{
	int i;
	if (!bcm_static_skb) {
		osl_pktfree(osh, p, send);
		return;
	}

	down(&bcm_static_skb->osl_pkt_sem);
	for (i = 0; i < STATIC_PKT_MAX_NUM; i++) {
		if (p == bcm_static_skb->skb_4k[i]) {
			bcm_static_skb->pkt_use[i] = 0;
			up(&bcm_static_skb->osl_pkt_sem);
			return;
		}
	}

	for (i = 0; i < STATIC_PKT_MAX_NUM; i++) {
		if (p == bcm_static_skb->skb_8k[i]) {
			bcm_static_skb->pkt_use[i + STATIC_PKT_MAX_NUM] = 0;
			up(&bcm_static_skb->osl_pkt_sem);
			return;
		}
	}
#ifdef ENHANCED_STATIC_BUF
	if (p == bcm_static_skb->skb_16k) {
		bcm_static_skb->pkt_use[STATIC_PKT_MAX_NUM * 2] = 0;
		up(&bcm_static_skb->osl_pkt_sem);
		return;
	}
#endif
	up(&bcm_static_skb->osl_pkt_sem);
	osl_pktfree(osh, p, send);
}
#endif /* CONFIG_DHD_USE_STATIC_BUF */

uint32
osl_pci_read_config(osl_t *osh, uint offset, uint size)
{
	uint val = 0;
	uint retry = PCI_CFG_RETRY;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	/* only 4byte access supported */
	ASSERT(size == 4);

	do {
		pci_read_config_dword(osh->pdev, offset, &val);
		if (val != 0xffffffff)
			break;
	} while (retry--);

#ifdef BCMDBG
	if (retry < PCI_CFG_RETRY)
		printk("PCI CONFIG READ access to %d required %d retries\n", offset,
		       (PCI_CFG_RETRY - retry));
#endif /* BCMDBG */

	return (val);
}

void
osl_pci_write_config(osl_t *osh, uint offset, uint size, uint val)
{
	uint retry = PCI_CFG_RETRY;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	/* only 4byte access supported */
	ASSERT(size == 4);

	do {
		pci_write_config_dword(osh->pdev, offset, val);
		if (offset != PCI_BAR0_WIN)
			break;
		if (osl_pci_read_config(osh, offset, size) == val)
			break;
	} while (retry--);

#ifdef BCMDBG
	if (retry < PCI_CFG_RETRY)
		printk("PCI CONFIG WRITE access to %d required %d retries\n", offset,
		       (PCI_CFG_RETRY - retry));
#endif /* BCMDBG */
}

/* return bus # for the pci device pointed by osh->pdev */
uint
osl_pci_bus(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

#if defined(__ARM_ARCH_7A__) && LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
	return pci_domain_nr(((struct pci_dev *)osh->pdev)->bus);
#else
	return ((struct pci_dev *)osh->pdev)->bus->number;
#endif
}

/* return slot # for the pci device pointed by osh->pdev */
uint
osl_pci_slot(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

#if defined(__ARM_ARCH_7A__) && LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
	return PCI_SLOT(((struct pci_dev *)osh->pdev)->devfn) + 1;
#else
	return PCI_SLOT(((struct pci_dev *)osh->pdev)->devfn);
#endif
}

/* return domain # for the pci device pointed by osh->pdev */
uint
osl_pcie_domain(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

	return pci_domain_nr(((struct pci_dev *)osh->pdev)->bus);
}

/* return bus # for the pci device pointed by osh->pdev */
uint
osl_pcie_bus(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

	return ((struct pci_dev *)osh->pdev)->bus->number;
}

/* return the pci device pointed by osh->pdev */
struct pci_dev *
osl_pci_device(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

	return osh->pdev;
}

const char *
osl_pci_name(osl_t *osh)
{
	ASSERT(osh && (osh->magic == OS_HANDLE_MAGIC) && osh->pdev);

	return (pci_name((struct pci_dev *)osh->pdev));
}

static void
osl_pcmcia_attr(osl_t *osh, uint offset, char *buf, int size, bool write)
{
}

void
osl_pcmcia_read_attr(osl_t *osh, uint offset, void *buf, int size)
{
	osl_pcmcia_attr(osh, offset, (char *) buf, size, FALSE);
}

void
osl_pcmcia_write_attr(osl_t *osh, uint offset, void *buf, int size)
{
	osl_pcmcia_attr(osh, offset, (char *) buf, size, TRUE);
}

#ifdef BCMDBG_MEM
/* In BCMDBG_MEM configurations osl_malloc is only used internally in
 * the implementation of osl_debug_malloc.  Because we are using the GCC
 * -Wstrict-prototypes compile option, we must always have a prototype
 * for a global/external function.  So make osl_malloc static in
 * the BCMDBG_MEM case.
 */
static
#endif
void *
osl_malloc(osl_t *osh, uint size)
{
	void *addr;
	gfp_t flags;

	/* only ASSERT if osh is defined */
	if (osh)
		ASSERT(osh->magic == OS_HANDLE_MAGIC);

#ifdef CONFIG_DHD_USE_STATIC_BUF
	if (bcm_static_buf)
	{
		int i = 0;
		if ((size >= PAGE_SIZE)&&(size <= STATIC_BUF_SIZE))
		{
			down(&bcm_static_buf->static_sem);

			for (i = 0; i < STATIC_BUF_MAX_NUM; i++)
			{
				if (bcm_static_buf->buf_use[i] == 0)
					break;
			}

			if (i == STATIC_BUF_MAX_NUM)
			{
				up(&bcm_static_buf->static_sem);
				printk("all static buff in use!\n");
				goto original;
			}

			bcm_static_buf->buf_use[i] = 1;
			up(&bcm_static_buf->static_sem);

			bzero(bcm_static_buf->buf_ptr+STATIC_BUF_SIZE*i, size);
			if (osh)
				atomic_add(size, &osh->cmn->malloced);

			return ((void *)(bcm_static_buf->buf_ptr+STATIC_BUF_SIZE*i));
		}
	}
original:
#endif /* CONFIG_DHD_USE_STATIC_BUF */

	flags = CAN_SLEEP() ? GFP_KERNEL: GFP_ATOMIC;
	if ((addr = kmalloc(size, flags)) == NULL) {
		if (osh)
			osh->failed++;
		return (NULL);
	}
	if (osh && osh->cmn)
		atomic_add(size, &osh->cmn->malloced);

	return (addr);
}

#ifndef BCMDBG_MEM
void *
osl_mallocz(osl_t *osh, uint size)
{
	void *ptr;

	ptr = osl_malloc(osh, size);

	if (ptr != NULL) {
		bzero(ptr, size);
	}

	return ptr;
}
#endif

#ifdef BCMDBG_MEM
/* In BCMDBG_MEM configurations osl_mfree is only used internally in
 * the implementation of osl_debug_mfree.  Because we are using the GCC
 * -Wstrict-prototypes compile option, we must always have a prototype
 * for a global/external function.  So make osl_mfree static in
 * the BCMDBG_MEM case.
 */
static
#endif
void
osl_mfree(osl_t *osh, void *addr, uint size)
{
#ifdef CONFIG_DHD_USE_STATIC_BUF
	if (bcm_static_buf)
	{
		if ((addr > (void *)bcm_static_buf) && ((unsigned char *)addr
			<= ((unsigned char *)bcm_static_buf + STATIC_BUF_TOTAL_LEN)))
		{
			int buf_idx = 0;

			buf_idx = ((unsigned char *)addr - bcm_static_buf->buf_ptr)/STATIC_BUF_SIZE;

			down(&bcm_static_buf->static_sem);
			bcm_static_buf->buf_use[buf_idx] = 0;
			up(&bcm_static_buf->static_sem);

			if (osh && osh->cmn) {
				ASSERT(osh->magic == OS_HANDLE_MAGIC);
				atomic_sub(size, &osh->cmn->malloced);
			}
			return;
		}
	}
#endif /* CONFIG_DHD_USE_STATIC_BUF */
	if (osh && osh->cmn) {
		ASSERT(osh->magic == OS_HANDLE_MAGIC);

		ASSERT(size <= osl_malloced(osh));

		atomic_sub(size, &osh->cmn->malloced);
	}
	kfree(addr);
}

uint
osl_check_memleak(osl_t *osh)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	if (atomic_read(&osh->cmn->refcount) == 1)
		return (atomic_read(&osh->cmn->malloced));
	else
		return 0;
}

uint
osl_malloced(osl_t *osh)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	return (atomic_read(&osh->cmn->malloced));
}

uint
osl_malloc_failed(osl_t *osh)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	return (osh->failed);
}

#ifdef BCMDBG_MEM
#define MEMLIST_LOCK(osh, flags)	spin_lock_irqsave(&(osh)->cmn->dbgmem_lock, flags)
#define MEMLIST_UNLOCK(osh, flags)	spin_unlock_irqrestore(&(osh)->cmn->dbgmem_lock, flags)
void *
osl_debug_malloc(osl_t *osh, uint size, int line, const char* file)
{
	bcm_mem_link_t *p;
	const char* basename;
	unsigned long flags = 0;
	if (!size) {
		printk("%s: allocating zero sized mem at %s line %d\n", __FUNCTION__, file, line);
		ASSERT(0);
	}

	if (osh) {
		MEMLIST_LOCK(osh, flags);
	}
	if ((p = (bcm_mem_link_t*)osl_malloc(osh, sizeof(bcm_mem_link_t) + size)) == NULL) {
		if (osh) {
			MEMLIST_UNLOCK(osh, flags);
		}
		return (NULL);
	}

	p->size = size;
	p->line = line;
	p->osh = (void *)osh;

	basename = strrchr(file, '/');
	/* skip the '/' */
	if (basename)
		basename++;

	if (!basename)
		basename = file;

	strncpy(p->file, basename, BCM_MEM_FILENAME_LEN);
	p->file[BCM_MEM_FILENAME_LEN - 1] = '\0';

	/* link this block */
	if (osh) {
		p->prev = NULL;
		p->next = osh->cmn->dbgmem_list;
		if (p->next)
			p->next->prev = p;
		osh->cmn->dbgmem_list = p;
		MEMLIST_UNLOCK(osh, flags);
	}

	return p + 1;
}

void *
osl_debug_mallocz(osl_t *osh, uint size, int line, const char* file)
{
	void *ptr;

	ptr = osl_debug_malloc(osh, size, line, file);

	if (ptr != NULL) {
		bzero(ptr, size);
	}

	return ptr;
}

void
osl_debug_mfree(osl_t *osh, void *addr, uint size, int line, const char* file)
{
	bcm_mem_link_t *p = (bcm_mem_link_t *)((int8*)addr - sizeof(bcm_mem_link_t));
	unsigned long flags = 0;

	ASSERT(osh == NULL || osh->magic == OS_HANDLE_MAGIC);

	if (p->size == 0) {
		printk("osl_debug_mfree: double free on addr %p size %d at line %d file %s\n",
			addr, size, line, file);
		ASSERT(p->size);
		return;
	}

	if (p->size != size) {
		printk("%s: dealloca size does not match alloc size\n", __FUNCTION__);
		printk("Dealloc addr %p size %d at line %d file %s\n", addr, size, line, file);
		printk("Alloc size %d line %d file %s\n", p->size, p->line, p->file);
		ASSERT(p->size == size);
		return;
	}

	if (osh && ((osl_t*)p->osh)->cmn != osh->cmn) {
		printk("osl_debug_mfree: alloc osh %p does not match dealloc osh %p\n",
			((osl_t*)p->osh)->cmn, osh->cmn);
		printk("Dealloc addr %p size %d at line %d file %s\n", addr, size, line, file);
		printk("Alloc size %d line %d file %s\n", p->size, p->line, p->file);
		ASSERT(((osl_t*)p->osh)->cmn == osh->cmn);
		return;
	}

	/* unlink this block */
	if (osh && osh->cmn) {
		MEMLIST_LOCK(osh, flags);
		if (p->prev)
			p->prev->next = p->next;
		if (p->next)
			p->next->prev = p->prev;
		if (osh->cmn->dbgmem_list == p)
			osh->cmn->dbgmem_list = p->next;
		p->next = p->prev = NULL;
	}
	p->size = 0;

	osl_mfree(osh, p, size + sizeof(bcm_mem_link_t));
	if (osh && osh->cmn) {
		MEMLIST_UNLOCK(osh, flags);
	}
}

int
osl_debug_memdump(osl_t *osh, struct bcmstrbuf *b)
{
	bcm_mem_link_t *p;
	unsigned long flags = 0;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	MEMLIST_LOCK(osh, flags);

	if (osl_check_memleak(osh) && osh->cmn->dbgmem_list) {
		if (b != NULL)
			bcm_bprintf(b, "   Address   Size File:line\n");
		else
			printf("   Address   Size File:line\n");

		for (p = osh->cmn->dbgmem_list; p; p = p->next) {
			if (b != NULL)
				bcm_bprintf(b, "%p %6d %s:%d\n", (char*)p + sizeof(bcm_mem_link_t),
					p->size, p->file, p->line);
			else
				printf("%p %6d %s:%d\n", (char*)p + sizeof(bcm_mem_link_t),
					p->size, p->file, p->line);

			/* Detects loop-to-self so we don't enter infinite loop */
			if (p == p->next) {
				if (b != NULL)
					bcm_bprintf(b, "WARNING: loop-to-self "
						"p %p p->next %p\n", p, p->next);
				else
					printf("WARNING: loop-to-self "
						"p %p p->next %p\n", p, p->next);

				break;
			}
		}
	}
	MEMLIST_UNLOCK(osh, flags);

	return 0;
}

#endif	/* BCMDBG_MEM */

uint
osl_dma_consistent_align(void)
{
	return (PAGE_SIZE);
}

void*
osl_dma_alloc_consistent(osl_t *osh, uint size, uint16 align_bits, uint *alloced, dmaaddr_t *pap)
{
	void *va;
	uint16 align = (1 << align_bits);
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	if (!ISALIGNED(DMA_CONSISTENT_ALIGN, align))
		size += align;
	*alloced = size;

#ifdef __ARM_ARCH_7A__
#ifndef	BCM_SECURE_DMA
	va = kmalloc(size, GFP_ATOMIC | __GFP_ZERO);
	if (va)
		*pap = (ulong)__virt_to_phys((ulong)va);
#else
	va = osl_sec_dma_alloc_consistent(osh, size, align_bits, pap);
#endif /* BCM_SECURE_DMA */
#else
	{
		dma_addr_t pap_lin;
		va = pci_alloc_consistent(osh->pdev, size, &pap_lin);
		*pap = (dmaaddr_t)pap_lin;
	}
#endif /* __ARM_ARCH_7A__ */
	return va;
}

void
osl_dma_free_consistent(osl_t *osh, void *va, uint size, dmaaddr_t pa)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

#ifdef __ARM_ARCH_7A__
#ifndef BCM_SECURE_DMA
	kfree(va);
#endif /* BCM_SECURE_DMA */
#else
	pci_free_consistent(osh->pdev, size, va, (dma_addr_t)pa);
#endif /* __ARM_ARCH_7A__ */
}

dmaaddr_t BCMFASTPATH
osl_dma_map(osl_t *osh, void *va, uint size, int direction, void *p, hnddma_seg_map_t *dmah)
{
	int dir;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	dir = (direction == DMA_TX)? PCI_DMA_TODEVICE: PCI_DMA_FROMDEVICE;

#if defined(BCM47XX_CA9) && defined(BCMDMASGLISTOSL)
	if (dmah != NULL) {
		int32 nsegs, i, totsegs = 0, totlen = 0;
		struct scatterlist *sg, _sg[MAX_DMA_SEGS];
		struct scatterlist *s;
		struct sk_buff *skb;

		for (skb = (struct sk_buff *)p; skb != NULL; skb = PKTNEXT(osh, skb)) {
			sg = &_sg[totsegs];
			if (skb_is_nonlinear(skb)) {
				nsegs = skb_to_sgvec(skb, sg, 0, PKTLEN(osh, skb));
				ASSERT((nsegs > 0) && (totsegs + nsegs <= MAX_DMA_SEGS));
				if (osl_is_flag_set(osh, OSL_ACP_COHERENCE)) {
					for_each_sg(sg, s, nsegs, i) {
						if (sg_phys(s) >= ACP_WIN_LIMIT) {
							dma_map_page(
							&((struct pci_dev *)osh->pdev)->dev,
							sg_page(s), s->offset, s->length, dir);
						}
					}
				} else
					pci_map_sg(osh->pdev, sg, nsegs, dir);
			} else {
				nsegs = 1;
				ASSERT(totsegs + nsegs <= MAX_DMA_SEGS);
				sg->page_link = 0;
				sg_set_buf(sg, PKTDATA(osh, skb), PKTLEN(osh, skb));

				/* not do cache ops */
				if (osl_is_flag_set(osh, OSL_ACP_COHERENCE) &&
					(virt_to_phys(PKTDATA(osh, skb)) < ACP_WIN_LIMIT))
					goto no_cache_ops;

#ifdef CTFMAP
				/* Map size bytes (not skb->len) for ctf bufs */
				pci_map_single(osh->pdev, PKTDATA(osh, skb),
				    PKTISCTF(osh, skb) ? CTFMAPSZ : PKTLEN(osh, skb), dir);
#else
				pci_map_single(osh->pdev, PKTDATA(osh, skb), PKTLEN(osh, skb), dir);

#endif
			}
no_cache_ops:
			totsegs += nsegs;
			totlen += PKTLEN(osh, skb);
		}

		dmah->nsegs = totsegs;
		dmah->origsize = totlen;
		for (i = 0, sg = _sg; i < totsegs; i++, sg++) {
			dmah->segs[i].addr = sg_phys(sg);
			dmah->segs[i].length = sg->length;
		}
		return dmah->segs[0].addr;
	}
#endif /* BCM47XX_CA9 && BCMDMASGLISTOSL */

#if defined(BCM47XX_CA9)
	if (osl_is_flag_set(osh, OSL_ACP_COHERENCE)) {
		uint pa = virt_to_phys(va);
		if (pa < ACP_WIN_LIMIT)
			return (pa);
	}
#endif /* BCM47XX_CA9 */

	return (pci_map_single(osh->pdev, va, size, dir));
}

void BCMFASTPATH
osl_dma_unmap(osl_t *osh, uint pa, uint size, int direction)
{
	int dir;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

#if defined(BCM47XX_CA9)
	if (osl_is_flag_set(osh, OSL_ACP_COHERENCE) && (pa < ACP_WIN_LIMIT))
		return;
#endif /* BCM47XX_CA9 */

	dir = (direction == DMA_TX)? PCI_DMA_TODEVICE: PCI_DMA_FROMDEVICE;
	pci_unmap_single(osh->pdev, (uint32)pa, size, dir);
}

/* OSL function for CPU relax */
inline void BCMFASTPATH
osl_cpu_relax(void)
{
	cpu_relax();
}

#if defined(BCMDBG_ASSERT)
void
osl_assert(const char *exp, const char *file, int line)
{
	char tempbuf[256];
	const char *basename;

	basename = strrchr(file, '/');
	/* skip the '/' */
	if (basename)
		basename++;

	if (!basename)
		basename = file;


#ifdef BCMDBG_ASSERT
	snprintf(tempbuf, 256, "assertion \"%s\" failed: file \"%s\", line %d\n",
		exp, basename, line);

	/* Print assert message and give it time to be written to /var/log/messages */
	if (!in_interrupt() && g_assert_type != 1) {
		const int delay = 3;
		printk("%s", tempbuf);
		printk("panic in %d seconds\n", delay);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(delay * HZ);
	}

	switch (g_assert_type) {
	case 0:
		panic("%s", tempbuf);
		break;
	case 1:
		printk("%s", tempbuf);
		break;
	case 2:
		printk("%s", tempbuf);
		BUG();
		break;
	default:
		break;
	}
#endif /* BCMDBG_ASSERT */

}
#endif 

void
osl_delay(uint usec)
{
	uint d;

	while (usec > 0) {
		d = MIN(usec, 1000);
		udelay(d);
		usec -= d;
	}
}

void
osl_sleep(uint ms)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
	if (ms < 20)
		usleep_range(ms*1000, ms*1000 + 1000);
	else
#endif
	msleep(ms);
}



/* Clone a packet.
 * The pkttag contents are NOT cloned.
 */
#ifdef BCMDBG_PKT
void *
osl_pktdup(osl_t *osh, void *skb, int line, char *file)
#else /* BCMDBG_PKT */
#ifdef BCMDBG_CTRACE
void *
osl_pktdup(osl_t *osh, void *skb, int line, char *file)
#else
void *
osl_pktdup(osl_t *osh, void *skb)
#endif /* BCMDBG_CTRACE */
#endif /* BCMDBG_PKT */
{
	void * p;
#ifdef BCMDBG_PKT
	unsigned long irqflags;
#endif

	ASSERT(!PKTISCHAINED(skb));

	/* clear the CTFBUF flag if set and map the rest of the buffer
	 * before cloning.
	 */
	PKTCTFMAP(osh, skb);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
	if ((p = pskb_copy((struct sk_buff *)skb, GFP_ATOMIC)) == NULL)
#else
	if ((p = skb_clone((struct sk_buff *)skb, GFP_ATOMIC)) == NULL)
#endif
		return NULL;

#ifdef CTFPOOL
	if (PKTISFAST(osh, skb)) {
		ctfpool_t *ctfpool;

		/* if the buffer allocated from ctfpool is cloned then
		 * we can't be sure when it will be freed. since there
		 * is a chance that we will be losing a buffer
		 * from our pool, we increment the refill count for the
		 * object to be alloced later.
		 */
		ctfpool = (ctfpool_t *)CTFPOOLPTR(osh, skb);
		ASSERT(ctfpool != NULL);
		PKTCLRFAST(osh, p);
		PKTCLRFAST(osh, skb);
		ctfpool->refills++;
	}
#endif /* CTFPOOL */

	/* Clear PKTC  context */
	PKTSETCLINK(p, NULL);
	PKTCCLRFLAGS(p);
	PKTCSETCNT(p, 1);
	PKTCSETLEN(p, PKTLEN(osh, skb));

	/* skb_clone copies skb->cb.. we don't want that */
	if (osh->pub.pkttag)
		OSL_PKTTAG_CLEAR(p);

	/* Increment the packet counter */
	atomic_inc(&osh->cmn->pktalloced);
#ifdef BCMDBG_CTRACE
	ADD_CTRACE(osh, (struct sk_buff *)p, file, line);
#endif
#ifdef BCMDBG_PKT
	spin_lock_irqsave(&osh->cmn->pktlist_lock, irqflags);
	pktlist_add(&(osh->cmn->pktlist), (void *) p, line, file);
	spin_unlock_irqrestore(&osh->cmn->pktlist_lock, irqflags);
#endif
	return (p);
}

/* Copy a packet.
 * The pkttag contents are NOT cloned.
 */
#ifdef BCMDBG_PKT
void *
osl_pktdup_cpy(osl_t *osh, void *skb, int line, char *file)
#else /* BCMDBG_PKT */
#ifdef BCMDBG_CTRACE
void *
osl_pktdup_cpy(osl_t *osh, void *skb, int line, char *file)
#else
void *
osl_pktdup_cpy(osl_t *osh, void *skb)
#endif /* BCMDBG_CTRACE */
#endif /* BCMDBG_PKT */
{
	void * p;
#ifdef BCMDBG_PKT
	unsigned long irqflags;
#endif

	ASSERT(!PKTISCHAINED(skb));

	/* clear the CTFBUF flag if set and map the rest of the buffer
	 * before cloning.
	 */
	PKTCTFMAP(osh, skb);

	if ((p = pskb_copy((struct sk_buff *)skb, GFP_ATOMIC)) == NULL)
		return NULL;

#ifdef CTFPOOL
	if (PKTISFAST(osh, skb)) {
		ctfpool_t *ctfpool;

		/* if the buffer allocated from ctfpool is cloned then
		 * we can't be sure when it will be freed. since there
		 * is a chance that we will be losing a buffer
		 * from our pool, we increment the refill count for the
		 * object to be alloced later.
		 */
		ctfpool = (ctfpool_t *)CTFPOOLPTR(osh, skb);
		ASSERT(ctfpool != NULL);
		PKTCLRFAST(osh, p);
		PKTCLRFAST(osh, skb);
		ctfpool->refills++;
	}
#endif /* CTFPOOL */

	/* Clear PKTC  context */
	PKTSETCLINK(p, NULL);
	PKTCCLRFLAGS(p);
	PKTCSETCNT(p, 1);
	PKTCSETLEN(p, PKTLEN(osh, skb));

	/* skb_clone copies skb->cb.. we don't want that */
	if (osh->pub.pkttag)
		OSL_PKTTAG_CLEAR(p);

	/* Increment the packet counter */
	atomic_inc(&osh->cmn->pktalloced);
#ifdef BCMDBG_CTRACE
	ADD_CTRACE(osh, (struct sk_buff *)p, file, line);
#endif
#ifdef BCMDBG_PKT
	spin_lock_irqsave(&osh->cmn->pktlist_lock, irqflags);
	pktlist_add(&(osh->cmn->pktlist), (void *) p, line, file);
	spin_unlock_irqrestore(&osh->cmn->pktlist_lock, irqflags);
#endif
	return (p);
}

#ifdef BCMDBG_CTRACE
int osl_pkt_is_frmnative(osl_t *osh, struct sk_buff *pkt)
{
	unsigned long flags;
	struct sk_buff *skb;
	int ck = FALSE;

	spin_lock_irqsave(&osh->ctrace_lock, flags);

	list_for_each_entry(skb, &osh->ctrace_list, ctrace_list) {
		if (pkt == skb) {
			ck = TRUE;
			break;
		}
	}

	spin_unlock_irqrestore(&osh->ctrace_lock, flags);
	return ck;
}

void osl_ctrace_dump(osl_t *osh, struct bcmstrbuf *b)
{
	unsigned long flags;
	struct sk_buff *skb;
	int idx = 0;
	int i, j;

	spin_lock_irqsave(&osh->ctrace_lock, flags);

	if (b != NULL)
		bcm_bprintf(b, " Total %d sbk not free\n", osh->ctrace_num);
	else
		printk(" Total %d sbk not free\n", osh->ctrace_num);

	list_for_each_entry(skb, &osh->ctrace_list, ctrace_list) {
		if (b != NULL)
			bcm_bprintf(b, "[%d] skb %p:\n", ++idx, skb);
		else
			printk("[%d] skb %p:\n", ++idx, skb);

		for (i = 0; i < skb->ctrace_count; i++) {
			j = (skb->ctrace_start + i) % CTRACE_NUM;
			if (b != NULL)
				bcm_bprintf(b, "    [%s(%d)]\n", skb->func[j], skb->line[j]);
			else
				printk("    [%s(%d)]\n", skb->func[j], skb->line[j]);
		}
		if (b != NULL)
			bcm_bprintf(b, "\n");
		else
			printk("\n");
	}

	spin_unlock_irqrestore(&osh->ctrace_lock, flags);

	return;
}
#endif /* BCMDBG_CTRACE */

#ifdef BCMDBG_PKT
#ifdef BCMDBG_PTRACE
void
osl_pkttrace(osl_t *osh, void *pkt, uint16 bit)
{
	pktlist_trace(&(osh->cmn->pktlist), pkt, bit);
}
#endif /* BCMDBG_PTRACE */

char *
osl_pktlist_dump(osl_t *osh, char *buf)
{
	pktlist_dump(&(osh->cmn->pktlist), buf);
	return buf;
}

void
osl_pktlist_add(osl_t *osh, void *p, int line, char *file)
{
	unsigned long flags;
	spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
	pktlist_add(&(osh->cmn->pktlist), p, line, file);
	spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
}

void
osl_pktlist_remove(osl_t *osh, void *p)
{
	unsigned long flags;
	spin_lock_irqsave(&osh->cmn->pktlist_lock, flags);
	pktlist_remove(&(osh->cmn->pktlist), p);
	spin_unlock_irqrestore(&osh->cmn->pktlist_lock, flags);
}
#endif /* BCMDBG_PKT */

/*
 * OSLREGOPS specifies the use of osl_XXX routines to be used for register access
 */
#ifdef OSLREGOPS
uint8
osl_readb(osl_t *osh, volatile uint8 *r)
{
	osl_rreg_fn_t rreg	= ((osl_pubinfo_t*)osh)->rreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	return (uint8)((rreg)(ctx, (void*)r, sizeof(uint8)));
}


uint16
osl_readw(osl_t *osh, volatile uint16 *r)
{
	osl_rreg_fn_t rreg	= ((osl_pubinfo_t*)osh)->rreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	return (uint16)((rreg)(ctx, (void*)r, sizeof(uint16)));
}

uint32
osl_readl(osl_t *osh, volatile uint32 *r)
{
	osl_rreg_fn_t rreg	= ((osl_pubinfo_t*)osh)->rreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	return (uint32)((rreg)(ctx, (void*)r, sizeof(uint32)));
}

void
osl_writeb(osl_t *osh, volatile uint8 *r, uint8 v)
{
	osl_wreg_fn_t wreg	= ((osl_pubinfo_t*)osh)->wreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	((wreg)(ctx, (void*)r, v, sizeof(uint8)));
}


void
osl_writew(osl_t *osh, volatile uint16 *r, uint16 v)
{
	osl_wreg_fn_t wreg	= ((osl_pubinfo_t*)osh)->wreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	((wreg)(ctx, (void*)r, v, sizeof(uint16)));
}

void
osl_writel(osl_t *osh, volatile uint32 *r, uint32 v)
{
	osl_wreg_fn_t wreg	= ((osl_pubinfo_t*)osh)->wreg_fn;
	void *ctx		= ((osl_pubinfo_t*)osh)->reg_ctx;

	((wreg)(ctx, (void*)r, v, sizeof(uint32)));
}
#endif /* OSLREGOPS */

/*
 * BINOSL selects the slightly slower function-call-based binary compatible osl.
 */

uint
osl_pktalloced(osl_t *osh)
{
	if (atomic_read(&osh->cmn->refcount) == 1)
		return (atomic_read(&osh->cmn->pktalloced));
	else
		return 0;
}

uint32
osl_rand(void)
{
	uint32 rand;

	get_random_bytes(&rand, sizeof(rand));

	return rand;
}

/* Linux Kernel: File Operations: start */
void *
osl_os_open_image(char *filename)
{
	struct file *fp;

	fp = filp_open(filename, O_RDONLY, 0);
	/*
	 * 2.6.11 (FC4) supports filp_open() but later revs don't?
	 * Alternative:
	 * fp = open_namei(AT_FDCWD, filename, O_RD, 0);
	 * ???
	 */
	 if (IS_ERR(fp))
		 fp = NULL;

	 return fp;
}

int
osl_os_get_image_block(char *buf, int len, void *image)
{
	struct file *fp = (struct file *)image;
	int rdlen;

	if (!image)
		return 0;

	rdlen = kernel_read(fp, fp->f_pos, buf, len);
	if (rdlen > 0)
		fp->f_pos += rdlen;

	return rdlen;
}

void
osl_os_close_image(void *image)
{
	if (image)
		filp_close((struct file *)image, NULL);
}

int
osl_os_image_size(void *image)
{
	int len = 0, curroffset;

	if (image) {
		/* store the current offset */
		curroffset = generic_file_llseek(image, 0, 1);
		/* goto end of file to get length */
		len = generic_file_llseek(image, 0, 2);
		/* restore back the offset */
		generic_file_llseek(image, curroffset, 0);
	}
	return len;
}

/* Linux Kernel: File Operations: end */

#if (defined(BCM47XX_CA9) || (defined(STB) && defined(__arm__)))
inline void osl_pcie_rreg(osl_t *osh, ulong addr, void *v, uint size)
{
	unsigned long flags = 0;
	int pci_access = 0;
	int acp_war_enab = ACP_WAR_ENAB();

	if (osh && BUSTYPE(osh->bustype) == PCI_BUS)
		pci_access = 1;

	if (pci_access && acp_war_enab)
		spin_lock_irqsave(&l2x0_reg_lock, flags);

	switch (size) {
	case sizeof(uint8):
		*(uint8*)v = readb((volatile uint8*)(addr));
		break;
	case sizeof(uint16):
		*(uint16*)v = readw((volatile uint16*)(addr));
		break;
	case sizeof(uint32):
		*(uint32*)v = readl((volatile uint32*)(addr));
		break;
	case sizeof(uint64):
		*(uint64*)v = *((volatile uint64*)(addr));
		break;
	}

	if (pci_access && acp_war_enab)
		spin_unlock_irqrestore(&l2x0_reg_lock, flags);
}
#endif /* BCM47XX_CA9 || (STB && __arm__) */

#ifdef BCM_SECURE_DMA

int
osl_sec_dma_setup_contig_mem(osl_t *osh, unsigned long memsize, int regn)
{
	int ret = BCME_OK;

	if (regn == CONT_REGION) {
		ret = osl_sec_dma_alloc_contig_mem(osh, memsize, regn);
	    if (ret != BCME_OK) {
			printk("linux_osl.c: CMA memory access failed\n");
		}
	}
	return ret;
	/* implement the MIPS Here */
}

int
osl_sec_dma_alloc_contig_mem(osl_t *osh, unsigned long memsize, int regn)
{
	u64 addr;

	printk("linux_osl.c: The value of cma mem block size = %ld\n", memsize);
	osh->cma = cma_dev_get_cma_dev(regn);
	printk("The value of cma = %p\n", osh->cma);
	if (!osh->cma) {
		printk("linux_osl.c:contig_region index is invalid\n");
		return BCME_ERROR;
	}
	if (cma_dev_get_mem(osh->cma, &addr, (u32)memsize, SEC_DMA_ALIGN) < 0) {
		printk("linux_osl.c: contiguous memory block allocation failure\n");
		return BCME_ERROR;
	}
	osh->dev = osh->cma->dev;
	osh->contig_base_alloc = (phys_addr_t)addr;
	osh->contig_base = osh->contig_base_alloc;
	printk("contig base alloc=0x%" PRI_FMT_X "\n", osh->contig_base_alloc);

	return BCME_OK;

}

void
osl_sec_dma_free_contig_mem(osl_t *osh, u32 memsize, int regn)
{
	int ret;
	if (SECDMA_INTERNAL_CMA == stb_ext_params) {
	ret = cma_dev_put_mem(osh->cma, (u64)osh->contig_base, memsize);
	if (ret)
		printf("%s contig base free failed\n", __FUNCTION__);
	}
}


void *
osl_sec_dma_ioremap(osl_t *osh, struct page *page, size_t size, bool iscache, bool isdecr)
{

	struct page **map;
	int order, i;
	void *addr = NULL;

	size = PAGE_ALIGN(size);
	order = get_order(size);

	map = kmalloc(sizeof(struct page *) << order, GFP_ATOMIC);
	if (map == NULL)
		return NULL;

	for (i = 0; i < (size >> PAGE_SHIFT); i++)
		map[i] = page + i;

	if (iscache) {
		addr = vmap(map, size >> PAGE_SHIFT, VM_MAP, __pgprot(PAGE_KERNEL));

		if (isdecr)
			osh->contig_delta_va_pa = (addr - page_to_phys(page));
	} else {

		addr = vmap(map, size >> PAGE_SHIFT, VM_MAP,
			pgprot_noncached(__pgprot(PAGE_KERNEL)));

		if (isdecr)
			osh->contig_delta_va_pa = (addr - page_to_phys(page));
	}

	kfree(map);
	return (void *)addr;
}

void
osl_sec_dma_iounmap(osl_t *osh, void *contig_base_va, size_t size)
{
	vunmap(contig_base_va);
}

void
osl_sec_dma_deinit_elem_mem_block(osl_t *osh, size_t mbsize, int max, void *sec_list_base)
{

	if (sec_list_base)
		kfree(sec_list_base);

}

int
osl_sec_dma_init_elem_mem_block(osl_t *osh, size_t mbsize, int max, sec_mem_elem_t **list)
{
	int i;
	int ret = BCME_OK;
	sec_mem_elem_t *sec_mem_elem;

	if ((sec_mem_elem = kmalloc(sizeof(sec_mem_elem_t)*(max), GFP_ATOMIC)) != NULL) {

		*list = sec_mem_elem;

		for (i = 0; i < max-1; i++) {
			sec_mem_elem->next = (sec_mem_elem + 1);
			sec_mem_elem->size = mbsize;
			sec_mem_elem->pa_cma = osh->contig_base_alloc;
			sec_mem_elem->vac = osh->contig_base_alloc_va;

			sec_mem_elem->pa_cma_page = PHYS_TO_PAGE(sec_mem_elem->pa_cma);

			osh->contig_base_alloc += mbsize;
			osh->contig_base_alloc_va += mbsize;

			sec_mem_elem = sec_mem_elem + 1;
		}
		sec_mem_elem->next = NULL;
		sec_mem_elem->size = mbsize;
		sec_mem_elem->pa_cma = osh->contig_base_alloc;
		sec_mem_elem->vac = osh->contig_base_alloc_va;

		sec_mem_elem->pa_cma_page = PHYS_TO_PAGE(sec_mem_elem->pa_cma);

		osh->contig_base_alloc += mbsize;
		osh->contig_base_alloc_va += mbsize;

	}
	else {
		printf("%s sec mem elem kmalloc failed\n", __FUNCTION__);
		ret = BCME_ERROR;
	}
	return ret;
}


sec_mem_elem_t * BCMFASTPATH
osl_sec_dma_alloc_mem_elem(osl_t *osh, void *va, uint size, int direction,
	struct sec_cma_info *ptr_cma_info, uint offset)
{
	sec_mem_elem_t *sec_mem_elem = NULL;

#ifdef NOT_YET
	if (size <= 512 && osh->sec_list_512) {
		sec_mem_elem = osh->sec_list_512;
		osh->sec_list_512 = sec_mem_elem->next;
	}
	else if (size <= 2048 && osh->sec_list_2048) {
		sec_mem_elem = osh->sec_list_2048;
		osh->sec_list_2048 = sec_mem_elem->next;
	}
	else
#else
	ASSERT(osh->sec_list_4096);
	sec_mem_elem = osh->sec_list_4096;
	osh->sec_list_4096 = sec_mem_elem->next;
#endif /* NOT_YET */

		sec_mem_elem->next = NULL;

	if (ptr_cma_info->sec_alloc_list_tail) {
		ptr_cma_info->sec_alloc_list_tail->next = sec_mem_elem;
		ptr_cma_info->sec_alloc_list_tail = sec_mem_elem;
	}
	else {
		/* First allocation: If tail is NULL, sec_alloc_list MUST also be NULL */
		ASSERT(ptr_cma_info->sec_alloc_list == NULL);
		ptr_cma_info->sec_alloc_list = sec_mem_elem;
		ptr_cma_info->sec_alloc_list_tail = sec_mem_elem;
	}
	return sec_mem_elem;
}


void BCMFASTPATH
osl_sec_dma_free_mem_elem(osl_t *osh, sec_mem_elem_t *sec_mem_elem)
{
	sec_mem_elem->dma_handle = 0x0;
	sec_mem_elem->va = NULL;
#ifdef NOT_YET
	if (sec_mem_elem->size == 512) {
		sec_mem_elem->next = osh->sec_list_512;
		osh->sec_list_512 = sec_mem_elem;
	}
	else if (sec_mem_elem->size == 2048) {
		sec_mem_elem->next = osh->sec_list_2048;
		osh->sec_list_2048 = sec_mem_elem;
	}
	else if (sec_mem_elem->size == 4096) {
#endif /* NOT_YET */
		sec_mem_elem->next = osh->sec_list_4096;
		osh->sec_list_4096 = sec_mem_elem;
#ifdef NOT_YET
	}
	else
	printf("%s free failed size=%d\n", __FUNCTION__, sec_mem_elem->size);
#endif /* NOT_YET */
}

#ifdef NOT_YET
sec_mem_elem_t * BCMFASTPATH
osl_sec_dma_find_elem(osl_t *osh, struct sec_cma_info *ptr_cma_info, void *va)
{
	sec_mem_elem_t *sec_mem_elem = ptr_cma_info->sec_alloc_list;

	while (sec_mem_elem != NULL)
	{
		if (sec_mem_elem->va == va)
			return sec_mem_elem;

		sec_mem_elem = sec_mem_elem->next;
	}
	return NULL;
}
#endif /* NOT_YET */

sec_mem_elem_t * BCMFASTPATH
osl_sec_dma_find_rem_elem(osl_t *osh, struct sec_cma_info *ptr_cma_info, dma_addr_t dma_handle)
{
	sec_mem_elem_t *sec_mem_elem = ptr_cma_info->sec_alloc_list;
	sec_mem_elem_t *sec_prv_elem = ptr_cma_info->sec_alloc_list;

	if (sec_mem_elem->dma_handle == dma_handle) {

		ptr_cma_info->sec_alloc_list = sec_mem_elem->next;

		if (sec_mem_elem == ptr_cma_info->sec_alloc_list_tail)
			ptr_cma_info->sec_alloc_list_tail = NULL;

		return sec_mem_elem;
	}
	sec_mem_elem = sec_mem_elem->next;

	while (sec_mem_elem != NULL) {

		if (sec_mem_elem->dma_handle == dma_handle) {

			sec_prv_elem->next = sec_mem_elem->next;
			if (sec_mem_elem == ptr_cma_info->sec_alloc_list_tail)
				ptr_cma_info->sec_alloc_list_tail = sec_prv_elem;

			return sec_mem_elem;
		}
		sec_prv_elem = sec_mem_elem;
		sec_mem_elem = sec_mem_elem->next;
	}
	return NULL;
}


dma_addr_t BCMFASTPATH
osl_sec_dma_map(osl_t *osh, void *va, uint size, int direction, void *p,
	hnddma_seg_map_t *dmah, void *ptr_cma_info, uint offset)
{

	sec_mem_elem_t *sec_mem_elem;
	void *pa_cma_kmap_va = NULL;
	struct page *pa_cma_page;
	int *fragva;
	uint buflen = 0;
	struct sk_buff *skb;
	dma_addr_t dma_handle = 0x0;
	uint loffset;
	int i = 0;
	BCM_REFERENCE(fragva);
	BCM_REFERENCE(i);
	BCM_REFERENCE(skb);

	ASSERT((direction == DMA_RX) || (direction == DMA_TX));
	sec_mem_elem = osl_sec_dma_alloc_mem_elem(osh, va, size, direction, ptr_cma_info, offset);

	sec_mem_elem->va = va;
	sec_mem_elem->direction = direction;

	pa_cma_page = sec_mem_elem->pa_cma_page;
	loffset = sec_mem_elem->pa_cma -(sec_mem_elem->pa_cma & ~(PAGE_SIZE-1));
	/* pa_cma_kmap_va = kmap_atomic(pa_cma_page);
	* pa_cma_kmap_va += loffset;
	*/

	pa_cma_kmap_va = sec_mem_elem->vac;
	buflen = size;

	if (direction == DMA_TX) {
		memcpy(pa_cma_kmap_va+offset, va, size);

#ifdef NOT_YET
		if (p == NULL) {

			memcpy(pa_cma_kmap_va+offset, va, size);
			/* prhex("Txpkt",pa_cma_kmap_va, size); */
		}
		else {
			for (skb = (struct sk_buff *)p; skb != NULL; skb = PKTNEXT(osh, skb)) {
				if (skb_is_nonlinear(skb)) {


					for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
						skb_frag_t *f = &skb_shinfo(skb)->frags[i];
						fragva = kmap_atomic(skb_frag_page(f));
						memcpy((pa_cma_kmap_va+buflen),
						(fragva + f->page_offset), skb_frag_size(f));
						kunmap_atomic(fragva);
						buflen += skb_frag_size(f);
					}
				}
				else {
					memcpy((pa_cma_kmap_va+buflen), skb->data, skb->len);
					buflen += skb->len;
				}
			}

		}
#endif /* NOT_YET */

		if (dmah) {
			dmah->nsegs = 1;
			dmah->origsize = buflen;
		}
	}
	else
	{
		if ((p != NULL) && (dmah != NULL)) {
			dmah->nsegs = 1;
			dmah->origsize = buflen;
		}

		*(phys_addr_t *)(pa_cma_kmap_va) = 0x0;
	}


		dma_handle = dma_map_page(osh->dev, pa_cma_page, loffset+offset, buflen,
			(direction == DMA_TX ? DMA_TO_DEVICE:DMA_FROM_DEVICE));

	if (dmah) {
		dmah->segs[0].addr = dma_handle;
		dmah->segs[0].length = buflen;
	}
	sec_mem_elem->dma_handle = dma_handle;
	/* kunmap_atomic(pa_cma_kmap_va-loffset); */
	return dma_handle;
}

dma_addr_t BCMFASTPATH
osl_sec_dma_dd_map(osl_t *osh, void *va, uint size, int direction, void *p, hnddma_seg_map_t *map)
{

	struct page *pa_cma_page;
	phys_addr_t pa_cma;
	dma_addr_t dma_handle = 0x0;
	uint loffset;

	pa_cma = (va - osh->contig_delta_va_pa);

	pa_cma_page = PHYS_TO_PAGE(pa_cma);

	loffset = pa_cma -(pa_cma & ~(PAGE_SIZE-1));

	dma_handle = dma_map_page(osh->dev, pa_cma_page, loffset, size,
		(direction == DMA_TX ? DMA_TO_DEVICE:DMA_FROM_DEVICE));

	return dma_handle;
}


void BCMFASTPATH
osl_sec_dma_unmap(osl_t *osh, dma_addr_t dma_handle, uint size, int direction,
void *p, hnddma_seg_map_t *map,	void *ptr_cma_info, uint offset)
{
	sec_mem_elem_t *sec_mem_elem;
	struct page *pa_cma_page;
	void *pa_cma_kmap_va = NULL;
	uint buflen = 0;
	dma_addr_t pa_cma;
	void *va;
	int read_count = 0;

	sec_mem_elem = osl_sec_dma_find_rem_elem(osh, ptr_cma_info, dma_handle);
	ASSERT(sec_mem_elem);

	va = sec_mem_elem->va;
	va -= offset;
	pa_cma = sec_mem_elem->pa_cma;

	pa_cma_page = sec_mem_elem->pa_cma_page;


	if (direction == DMA_RX) {

		if (p == NULL) {

			/* pa_cma_kmap_va = kmap_atomic(pa_cma_page);
			* pa_cma_kmap_va += loffset;
			*/

			pa_cma_kmap_va = sec_mem_elem->vac;

			do {
				dma_sync_single_for_cpu(osh->dev, sec_mem_elem->dma_handle,
					sizeof(int), DMA_FROM_DEVICE);

				buflen = *(phys_addr_t *)(pa_cma_kmap_va);
				if (buflen)
					break;

				OSL_DELAY(1);
				read_count++;
			} while (read_count < 200);

			dma_unmap_page(osh->dev, pa_cma, size+offset, DMA_FROM_DEVICE);
			memcpy(va, pa_cma_kmap_va, size+offset);
			/* prhex("rxpkt",pa_cma_kmap_va, 64); */
			/* kunmap_atomic(pa_cma_kmap_va); */
		}
#ifdef NOT_YET
		else {
			buflen = 0;
			for (skb = (struct sk_buff *)p; (buflen < size) &&
				(skb != NULL); skb = skb->next) {
				if (skb_is_nonlinear(skb)) {
					pa_cma_kmap_va = kmap_atomic(pa_cma_page);
					for (i = 0; (buflen < size) &&
						(i < skb_shinfo(skb)->nr_frags); i++) {
						skb_frag_t *f = &skb_shinfo(skb)->frags[i];
						cpuaddr = kmap_atomic(skb_frag_page(f));
						memcpy((cpuaddr + f->page_offset),
							(pa_cma_kmap_va+buflen), skb_frag_size(f));
						kunmap_atomic(cpuaddr);
						buflen += skb_frag_size(f);
					}
						kunmap_atomic(pa_cma_kmap_va);
				}
				else {
					pa_cma_kmap_va = kmap_atomic(pa_cma_page);
					memcpy(skb->data, (pa_cma_kmap_va + buflen), skb->len);
					kunmap_atomic(pa_cma_kmap_va);
					buflen += skb->len;
				}

			}

		}
#endif /* NOT YET */
	} else {
		dma_unmap_page(osh->dev, pa_cma, size+offset, DMA_TO_DEVICE);
	}

	osl_sec_dma_free_mem_elem(osh, sec_mem_elem);
}

void *
osl_sec_dma_alloc_consistent(osl_t *osh, uint size, uint16 align_bits, ulong *pap)
{
	/* Allocate 64k of bytes of CMA for every request, aligned with 64k boundary */
	/* Unlike system coherent memory cma is big */

	void *temp_va;
	unsigned long temp_pa;

	if ((osh->contig_base_alloc_coherent + size) > (osh->contig_base + CMA_DMA_DESC_MEMBLOCK)) {
		printf("%s No more coherent memory\n", __FUNCTION__);
		return NULL;
	}
	temp_va = osh->coherent_base_alloc_va;
	temp_pa = osh->contig_base_alloc_coherent;
	osh->coherent_base_alloc_va += SEC_DMA_ALIGN;
	osh->contig_base_alloc_coherent += SEC_DMA_ALIGN;

	*pap = (unsigned long)temp_pa;
	return temp_va;
}

void
osl_sec_cma_baseaddr_memsize(osl_t *osh, dma_addr_t *cma_baseaddr, size_t *cma_memsize)
{

	*cma_baseaddr = (dma_addr_t)osh->contig_base;
	*cma_memsize = (size_t)CMA_MEMBLOCK;
}

#endif /* BCM_SECURE_DMA */

#ifdef BCM_BLOG
uint
osl_pktprio(void *p)
{
	uint32 prio;
	prio = ((struct sk_buff*)p)->mark>>PRIO_LOC_NFMARK & 0x7;

	if (prio > 7)
		printk("osl_pktprio: wrong prio (0x%x) !!!\n", prio);

	return prio;
}

void
osl_pktsetprio(void *p, uint x)
{
		((struct sk_buff*)p)->mark &= ~(0x7 << PRIO_LOC_NFMARK);
		((struct sk_buff*)p)->mark |= (x & 0x7) << PRIO_LOC_NFMARK;
}
#endif /* BCM_BLOG */
