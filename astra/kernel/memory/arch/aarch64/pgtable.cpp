/***************************************************************************
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
 ***************************************************************************/

#include <hwtimer.h>
#include "lib_printf.h"
#include "kernel.h"
#include "pgtable.h"

static uint8_t kernPageTableMem[sizeof(PageTable)];

PageTable *PageTable::kernPageTable;
ALIGN_PT  uint8_t PageTable::pageTableBlocks[PAGE_SIZE_4K_BYTES * MAX_NUM_PAGE_TABLE_BLOCKS];
uint8_t* PageTable::freeBlockStack[MAX_NUM_PAGE_TABLE_BLOCKS + NUM_BOOTSTRAP_BLOCKS];
int PageTable::stackTop;
SpinLock PageTable::allocLock;

void* PageTable::operator new(size_t sz, void *where) {
	UNUSED(sz);
	PageTable *pt = (PageTable *)where;
	pt->topLevelDir = nullptr;
	spinLockInit(&pt->lock);

	return pt;
}

void PageTable::init() {
	uint8_t *currPage = &pageTableBlocks[0];
	for (int i = 0; i<MAX_NUM_PAGE_TABLE_BLOCKS; i++) {
		freeBlockStack[i] = currPage;
		currPage += PAGE_SIZE_4K_BYTES;
	}
	stackTop = MAX_NUM_PAGE_TABLE_BLOCKS-1;

	kernPageTable = new(kernPageTableMem) PageTable;

	uint64_t l0Table;
	register unsigned int ttbr1Low, ttbr1High;
	ARCH_SPECIFIC_GET_TTBR1(ttbr1Low,ttbr1High);
	l0Table = ((uint64_t)ttbr1High << 32) | ttbr1Low;

	kernPageTable->topLevelDir = (uint64_t *)TzMem::physToVirt((const void *)l0Table);
	kernPageTable->asid = KERNEL_ASID;
	printf("virt Kernel page table base addr %p\n", (void *)kernPageTable->topLevelDir);

	spinLockInit(&allocLock);
}

PageTable *PageTable::kernelPageTable() {
	return kernPageTable;
}

TzMem::VirtAddr PageTable::allocPageTableBlock() {

	SpinLocker locker(&allocLock);

	if (stackTop < 0) {
		err_msg("Ran out of page table blocks\n");
		kernelHalt("Page table blocks exhausted");
	}

	uint8_t *rv = freeBlockStack[stackTop--];
	return rv;
}

void PageTable::freePageTableBlock(TzMem::VirtAddr vaddr) {
	SpinLocker locker(&allocLock);

	if (stackTop == (MAX_NUM_PAGE_TABLE_BLOCKS + NUM_BOOTSTRAP_BLOCKS - 1)) {
		err_msg("Page table block stack corrupted. Some blocks were double freed.\n");
		kernelHalt("Page table block stack corruption");
	}

	freeBlockStack[++stackTop] = (uint8_t *)vaddr;
}

PageTable::PageTable(const PageTable& rhs, uint8_t aid, bool fork) {
	topLevelDir = (uint64_t *)allocPageTableBlock();
	//printf("Created page table %p %p\n", TzMem::virtToPhys(topLevelDir), topLevelDir);

	this->asid = aid;
	spinLockInit(&lock);

	for (int i=0; i<L0_PAGE_NUM_ENTRIES; i++) {
		if (rhs.topLevelDir[i] == 0) {
			topLevelDir[i] = 0;
			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&topLevelDir[i]);
			continue;
		}
		uint32_t rhsL1Block = (uint32_t)(rhs.topLevelDir[i] & L1_BLOCK_ADDR_MASK);
		uint64_t *rhsL1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t) rhsL1Block);

		uint64_t *l1Dir = (uint64_t *)allocPageTableBlock();
		uint64_t l0Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(l1Dir) & L1_BLOCK_ADDR_MASK);
		l0Entry |= 0x3;
		topLevelDir[i] = l0Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&topLevelDir[i]);
		for (int j=0; j<L1_PAGE_NUM_ENTRIES; j++) {
			if (rhsL1Dir[j] == 0) {
				l1Dir[j] = 0;
				// We wrote the entry into cached memory. Force it to
				// main memory by doing a DCCIMVAC on the entry.
				ARCH_SPECIFIC_DCCIMVAC(&l1Dir[j]);
				continue;
			}

		uint32_t rhsL2Block = (uint32_t)(rhsL1Dir[j] & L2_BLOCK_ADDR_MASK);
		uint64_t *rhsL2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t) rhsL2Block);

		uint64_t *l2Dir = (uint64_t *)allocPageTableBlock();
		uint64_t l1Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(l2Dir) & L2_BLOCK_ADDR_MASK);
		l1Entry |= 0x3;
		l1Dir[j] = l1Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l1Dir[j]);
		for (int k=0; k<L2_PAGE_NUM_ENTRIES; k++) {
			if (rhsL2Dir[k] == 0) {
				l2Dir[k] = 0;
				// We wrote the entry into cached memory. Force it to
				// main memory by doing a DCCIMVAC on the entry.
				ARCH_SPECIFIC_DCCIMVAC(&l2Dir[k]);
				continue;
			}
			uint32_t rhsL3Block = (uint32_t)(rhsL2Dir[k] & L2_BLOCK_ADDR_MASK);
			uint64_t *rhsL3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)rhsL3Block);

			uint64_t *l3Dir = (uint64_t *)allocPageTableBlock();
			uint64_t l2Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(l3Dir) & L3_BLOCK_ADDR_MASK);
			l2Entry |= 0x3;
			l2Dir[k] = l2Entry;
			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&l2Dir[k]);
			for (int l=0; l<L3_PAGE_NUM_ENTRIES; l++) {
				uint64_t l3Entry = rhsL3Dir[l];
					// A SharedMem page is inherited as SharedMem;
					// a AllocOnWrite page is inherited as AllocOnWrite;
					// an SW None page becomes either AllocOnWrite or shared
					if (GET_MEMORY_ACCESS_SW_BITS(rhsL3Dir[l]) == None) {
						if (fork &&
							GET_MEMORY_ACCESS_PERMS(rhsL3Dir[l]) == MEMORY_ACCESS_RW_USER) {

							CLEAR_MEMORY_ACCESS_PERMS(l3Entry);
							SET_MEMORY_ACCESS_PERMS(l3Entry, MEMORY_ACCESS_RO_USER);

							CLEAR_MEMORY_ACCESS_SW_BITS(l3Entry);
							SET_MEMORY_ACCESS_SW_BITS(l3Entry, AllocOnWrite);
						}
						else {
							CLEAR_MEMORY_ACCESS_SW_BITS(l3Entry);
							SET_MEMORY_ACCESS_SW_BITS(l3Entry, SharedMem);
						}
					}

				l3Dir[l] = l3Entry;

				// We wrote the entry into cached memory. Force it to
				// main memory by doing a DCCIMVAC on the entry.
				ARCH_SPECIFIC_DCCIMVAC(&l3Dir[l]);
			}
		}
        }
	}
	//dump();
}

PageTable::~PageTable() {

	if (topLevelDir == nullptr)
		return;

	for (int i=0; i<L0_PAGE_NUM_ENTRIES; i++) {
		if (topLevelDir[i] == 0)
			continue;

		uint32_t l1Block = (uint32_t)(topLevelDir[i] & L1_BLOCK_ADDR_MASK);
		uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)l1Block);

		for (int j=0; j<L1_PAGE_NUM_ENTRIES; j++) {
			if (l1Dir[j] == 0)
				continue;

			uint32_t l2Block = (uint32_t)(l1Dir[j] & L2_BLOCK_ADDR_MASK);
			uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)l2Block);

			for (int k=0; k<L2_PAGE_NUM_ENTRIES; k++) {
				if (l2Dir[k] == 0)
					continue;

				uint32_t l3Block = (uint32_t)(l2Dir[k] & L3_BLOCK_ADDR_MASK);
				uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)l3Block);

				for (int l=0; l<L3_PAGE_NUM_ENTRIES; l++) {
					uint64_t l3Entry = l3Dir[l];
					if (l3Entry == 0)
						continue;

					if ((l3Entry & 0x3) != 0x3)
						continue;

                if (GET_MEMORY_ACCESS_SW_BITS(l3Entry) != AllocOnWrite &&
                    GET_MEMORY_ACCESS_SW_BITS(l3Entry) != SharedMem) {
						TzMem::PhysAddr cowPage = (void *)(l3Entry & L3_PHYS_ADDR_MASK);
						if (cowPage != (TzMem::PhysAddr) L3_PHYS_ADDR_MASK) {
							TzMem::freePage(cowPage);
						}
					}
				}

				freePageTableBlock(l3Dir);
			}

			freePageTableBlock(l2Dir);
		}

		freePageTableBlock(l1Dir);
	}
	topLevelDir = nullptr;
}

void PageTable::reserveRange(TzMem::VirtAddr vaddrFirstPage, TzMem::VirtAddr vaddrLastPage) {
	TzMem::VirtAddr vaddr = vaddrFirstPage;

	if (topLevelDir == nullptr) {
		topLevelDir = (uint64_t *)allocPageTableBlock();
		for (int i=0; i<L0_PAGE_NUM_ENTRIES; i++) {
			topLevelDir[i] = 0;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&topLevelDir[0]);
		}
	}


	while (vaddr < vaddrLastPage) {

		// L0 table
		const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t)vaddr);
		uint64_t l0Entry = topLevelDir[l0Idx];
		if (l0Entry == 0) {
			uint64_t *ptBlock = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L1_PAGE_NUM_ENTRIES; i++)
				ptBlock[i] = 0;

			l0Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(ptBlock) & L1_BLOCK_ADDR_MASK);
			l0Entry |= 0x3;
			topLevelDir[l0Idx] = l0Entry;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&topLevelDir[l0Idx]);
		}

		// L1 table
		uint32_t blockAddr = (uint32_t)(l0Entry & L1_BLOCK_ADDR_MASK);
		uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l1Entry = l1Dir[l1Idx];
		if (l1Entry == 0) {

			uint64_t *ptBlock = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L2_PAGE_NUM_ENTRIES; i++)
				ptBlock[i] = 0;

			l1Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(ptBlock) & L2_BLOCK_ADDR_MASK);
			l1Entry |= 0x3;
			l1Dir[l1Idx] = l1Entry;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&l1Dir[l1Idx]);
		}

		blockAddr = (uint32_t)(l1Entry & L2_BLOCK_ADDR_MASK);
		uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l2Entry = l2Dir[l2Idx];
		// printf("Mapping vaddr %p: l2Idx %d l2Entry 0x%x%x\n", vaddr, l2Idx, (unsigned int)(l2Entry >> 32), (unsigned int)(l2Entry & 0xffffffff));
		if (l2Entry == 0) {

			uint64_t *ptBlock = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L3_PAGE_NUM_ENTRIES; i++)
				ptBlock[i] = 0;

			l2Entry = (uint64_t)((uintptr_t)TzMem::virtToPhys(ptBlock) & L3_BLOCK_ADDR_MASK);
			l2Entry |= 0x3;

			l2Dir[l2Idx] = l2Entry;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&l2Dir[l2Idx]);
		}

		blockAddr = (uint32_t)(l2Entry & L3_BLOCK_ADDR_MASK);
		uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("Reserving vaddr %p: l3Idx %d\n", vaddr, l3Idx);

		uint64_t l3Entry = 0x3;
		l3Entry |=  (uint64_t)((uint32_t)0xFFFFFFFF & L3_PHYS_ADDR_MASK); //0xFFFFF000
		SET_MEMORY_ACCESS_FLAG(l3Entry, ACCESS_FLAG_FAULT_GEN);

		l3Dir[l3Idx] = l3Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l3Dir[l3Idx]);

		vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
	}
}

void PageTable::releaseAddrRange(TzMem::VirtAddr vaddrFirstPage, unsigned int rangeSize) {
	TzMem::VirtAddr vaddr = vaddrFirstPage;
	TzMem::VirtAddr vaddrLastPage = PAGE_START_4K((uint8_t *)vaddr + rangeSize);

	if (topLevelDir == nullptr)
		return;

	while (vaddr < vaddrLastPage) {
		const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l0Entry = topLevelDir[l0Idx];
		// printf("UnMapping vaddr %p: l1Idx %d l1Entry 0x%x%x\n", vaddr, l1Idx, (unsigned int)(l1Entry >> 32), (unsigned int)(l1Entry & 0xffffffff));
		if (l0Entry == 0)
			return;

		uint32_t blockAddr = (uint32_t)(l0Entry & L1_BLOCK_ADDR_MASK);
		uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l1Entry = l1Dir[l1Idx];
		// printf("UnMapping vaddr %p: l1Idx %d l1Entry 0x%x%x\n", vaddr, l1Idx, (unsigned int)(l1Entry >> 32), (unsigned int)(l1Entry & 0xffffffff));
		if (l1Entry == 0)
			return;

		blockAddr = (uint32_t)(l1Entry & L2_BLOCK_ADDR_MASK);
		uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l2Entry = l2Dir[l2Idx];
		// printf("UnMapping vaddr %p: l2Idx %d l2Entry 0x%x%x\n", vaddr, l2Idx, (unsigned int)(l2Entry >> 32), (unsigned int)(l2Entry & 0xffffffff));
		if (l2Entry == 0)
			return;

		blockAddr = (uint32_t)(l2Entry & L3_BLOCK_ADDR_MASK);
		uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
		int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("Release vaddr %p: l3Idx %d\n", vaddr, l3Idx);

		uint64_t l3Entry = 0x0;
		l3Dir[l3Idx] = l3Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l3Dir[l3Idx]);

		vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
	}

}

void PageTable::mapPageRange(const TzMem::VirtAddr vaddrFirstPage, const TzMem::VirtAddr vaddrLastPage,
			const TzMem::PhysAddr paddrFirstPage, const int memAttr, const int memAccessPerms, const bool executeNever, const bool mapShared, const bool nonSecure) {

	//printf("page table %p: map virt[%p, %p] to phys[%p]\n", this, vaddrFirstPage, vaddrLastPage, paddrFirstPage);
	SpinLocker locker(&lock);

	TzMem::VirtAddr vaddr = vaddrFirstPage;
	TzMem::PhysAddr paddr = paddrFirstPage;

	if (topLevelDir == nullptr) {
		topLevelDir = (uint64_t *)allocPageTableBlock();
		for (int i=0; i<L0_PAGE_NUM_ENTRIES; i++) {
			topLevelDir[i] = INVALID_TABLE_DESCRIPTOR;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&topLevelDir[i]);
		}
	}
	uint64_t *l0Table = topLevelDir;

	while (vaddr <= vaddrLastPage) {
		// L0 table
		const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		const uint64_t l0Desc = l0Table[l0Idx];
		//printf("l0Table=%p l0Idx=%d\n",(void *)l0Table,l0Idx);
		if (!IS_VALID_TABLE_DESCRIPTOR(l0Desc)) {
			uint64_t *table = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L1_PAGE_NUM_ENTRIES; i++)
				table[i] = 0;

			l0Table[l0Idx] = MAKE_TABLE_DESCRIPTOR(TzMem::virtToPhys(table));

			ARCH_SPECIFIC_DCCIMVAC(&l0Table[l0Idx]);
			asm volatile("dsb ishst":::"memory");
			asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
			asm volatile("dsb ish":::"memory");
		}
		uint64_t *l1Table = (uint64_t *)(TABLE_PHYS_ADDR(l0Table[l0Idx]));
		l1Table = (uint64_t *)TzMem::physToVirt((void *)l1Table);

		// L1 table
		const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l1Table=%p l1Idx=%d\n",(void *)l1Table,l1Idx);
		const uint64_t l1Desc = l1Table[l1Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l1Desc)) {
			uint64_t *ptBlock = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L2_PAGE_NUM_ENTRIES; i++)
				ptBlock[i] = 0;

		l1Table[l1Idx] = MAKE_TABLE_DESCRIPTOR(TzMem::virtToPhys(ptBlock));

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&l1Table[l1Idx]);
			asm volatile("dsb ishst":::"memory");
			asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
			asm volatile("dsb ish":::"memory");
		}
		uint64_t *l2Table = (uint64_t *)(TABLE_PHYS_ADDR(l1Table[l1Idx]));
		l2Table = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)l2Table);

		// L2 Table
		int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		uint64_t l2Desc = l2Table[l2Idx];
		//printf("l2Table=%p l2Idx=%d l2Desc=%lx\n",(void *)l2Table,l2Idx,l2Desc);
		if (!IS_VALID_TABLE_DESCRIPTOR(l2Desc)) {

			uint64_t *ptBlock = (uint64_t *)allocPageTableBlock();
			for (int i=0; i<L3_PAGE_NUM_ENTRIES; i++)
				ptBlock[i] = 0;

			l2Desc = (uint64_t)((uintptr_t)TzMem::virtToPhys(ptBlock) & L3_BLOCK_ADDR_MASK);
			l2Desc |= 0x3;

			l2Table[l2Idx] = l2Desc;

			// We wrote the entry into cached memory. Force it to
			// main memory by doing a DCCIMVAC on the entry.
			ARCH_SPECIFIC_DCCIMVAC(&l2Table[l2Idx]);
			asm volatile("dsb ishst":::"memory");
			asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
			asm volatile("dsb ish":::"memory");
		}

		uint64_t *l3Table = (uint64_t *)(TABLE_PHYS_ADDR(l2Table[l2Idx] ));
		l3Table = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)l3Table);
		int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("Mapping vaddr %p: l3Idx %d\n", vaddr, l3Idx);

		uint64_t currDesc = l3Table[l3Idx];

		uint64_t l3Desc = 0x3;
		l3Desc |=  (uint64_t)((uintptr_t)paddr & L3_PHYS_ADDR_MASK); //0xFFFFF000

		SET_MEMORY_ACCESS_FLAG(l3Desc, ACCESS_FLAG_NO_FAULT_GEN);
		SET_MEMORY_ATTR(l3Desc, memAttr);
		SET_MEMORY_SH_ATTR(l3Desc, INNER_SHAREABLE);
		SET_MEMORY_ACCESS_PERMS(l3Desc, memAccessPerms);

		if (executeNever)
			SET_MEMORY_ACCESS_NO_EXEC(l3Desc);

		if (nonSecure)
			SET_MEMORY_NS_BIT(l3Desc, NS_BIT_NON_SECURE);

		if (GET_MEMORY_ACCESS_SW_BITS(currDesc) == AllocOnWrite)
			SET_MEMORY_ACCESS_SW_BITS(l3Desc, None);

		if (mapShared)
			SET_MEMORY_ACCESS_SW_BITS(l3Desc, SharedMem);

		l3Table[l3Idx] = l3Desc;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l3Table[l3Idx]);
		asm volatile("dsb ishst":::"memory");
		asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
		asm volatile("dsb ish":::"memory");

		vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
		paddr = (TzMem::PhysAddr)((uint8_t *)paddr + PAGE_SIZE_4K_BYTES);
	}

	// Issue a memory barrier: Data accesses and instructions that follow this point should not get
	// re-ordered to run before this point

	ARCH_SPECIFIC_MEMORY_BARRIER;
}

void PageTable::unmapPageRange(const TzMem::VirtAddr vaddrFirstPage,
							   const TzMem::VirtAddr vaddrLastPage,
							   const bool releaseVaddr) {

	//printf("page table %p: unmap [%p, %p]\n", this, vaddrFirstPage, vaddrLastPage);
	SpinLocker locker(&lock);

	TzMem::VirtAddr vaddr = vaddrFirstPage;

	uint64_t *l0Table = topLevelDir;

	while (vaddr <= vaddrLastPage) {
		const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l0Table=%p l0Idx=%d\n",(void *)l0Table,l0Idx);
		const uint64_t l0Desc = l0Table[l0Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l0Desc)) {
			err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
			kernelHalt("Attempted unmap a page that wasn't mapped");
		}

		uint64_t *l1Table = (uint64_t *)TABLE_PHYS_ADDR(l0Table[l0Idx]);
		l1Table = (uint64_t *)TzMem::physToVirt((void *)l1Table);

		int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l1Table=%p l1Idx=%d\n",(void *)l1Table,l1Idx);
		const uint64_t l1Desc = l1Table[l1Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l1Desc)) {
			err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
			kernelHalt("Attempted unmap a page that wasn't mapped");
		}

		uint64_t *l2Table = (uint64_t *)TABLE_PHYS_ADDR(l1Desc);
		l2Table = (uint64_t *)TzMem::physToVirt(l2Table);

		int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l2Table=%p l2Idx=%d addr=%p\n",(void *)l2Table,l2Idx,(void *)&l2Table[l2Idx]);
		const uint64_t l2Desc = l2Table[l2Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l2Desc)) {
			if (IS_VALID_BLOCK_DESCRIPTOR(l2Desc)) {
				l2Table[l2Idx] = INVALID_TABLE_DESCRIPTOR;

				ARCH_SPECIFIC_DCCIMVAC(&l2Table[l2Idx]);
				asm volatile("dsb ishst":::"memory");
				asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
				asm volatile("dsb ish":::"memory");

				vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_2M_BYTES);
				continue;
			}

			vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
			continue;
			err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
			kernelHalt("Attempted unmap a page that wasn't mapped");

		}

		uint64_t *l3Table = (uint64_t *)TABLE_PHYS_ADDR(l2Desc);
		l3Table = (uint64_t *)TzMem::physToVirt(l3Table);

		int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l3Table=%p l3Idx=%d addr=%p\n",(void *)l3Table,l3Idx,(void *)&l3Table[l3Idx]);
		if (l3Table[l3Idx] == 0) {
			err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
			kernelHalt("Attempted unmap a page that wasn't mapped");
		}

		// Unmap the page.
		//printf("UnMapping vaddr %p: l3Idx %d\n", vaddr, l3Idx);
		uint64_t l3Entry;
		if (releaseVaddr) {
			// Release the page.
			l3Entry = INVALID_TABLE_DESCRIPTOR;
		}
		else {
			// Keep the page reserved.
			l3Entry = 0x3;
			l3Entry |=  (uint64_t)((uint32_t)0xFFFFFFFF & L3_PHYS_ADDR_MASK); //0xFFFFF000
			SET_MEMORY_ACCESS_FLAG(l3Entry, ACCESS_FLAG_FAULT_GEN);
		}
		l3Table[l3Idx] = l3Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l3Table[l3Idx]);
		asm volatile("dsb ishst":::"memory");
		asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
		asm volatile("dsb ish":::"memory");

		vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);

		if (!releaseVaddr)
			continue;
		// Check if the l3Table can be freed.
		bool canFree = true;
		for (int i=0; i<L3_PAGE_NUM_ENTRIES; i++) {
			if (l3Table[i] != 0) {
				// No there is at-least one valid entry in this dir.
				canFree = false;
				break;
			}
		}
		if (!canFree)
			continue;
		// There are no valid entries in L3 table. Free it.
		freePageTableBlock(l3Table);
		l2Table[l2Idx] = INVALID_TABLE_DESCRIPTOR;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l2Table[l2Idx]);
		asm volatile("dsb ishst":::"memory");
		asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
		asm volatile("dsb ish":::"memory");


		// Now check if l2Dir can be freed.
		canFree = true;
		for (int i=0; i<L2_PAGE_NUM_ENTRIES; i++) {
			if (l2Table[i] != 0) {
				// No there is at-least one valid entry in this dir.
				canFree = false;
				break;
			}
		}
		if (!canFree)
			continue;

		// There are no valid entries in L3 table. Free it.
		freePageTableBlock(l2Table);
		l1Table[l1Idx] = INVALID_TABLE_DESCRIPTOR;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l1Table[l1Idx]);
		asm volatile("dsb ishst":::"memory");
		asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
		asm volatile("dsb ish":::"memory");


		// Now check if l1Table can be freed.
		canFree = true;
		for (int i=0; i<L1_PAGE_NUM_ENTRIES; i++) {
			if (l1Table[i] != 0) {
				// No there is at-least one valid entry in this dir.
				canFree = false;
				break;
			}
		}
		if (!canFree)
			continue;

		// There are no valid entries in L1 table. Free it.
		freePageTableBlock(l1Table);

		// Now check if l0Table can be freed.
		canFree = true;
		for (int i=0; i<L0_PAGE_NUM_ENTRIES; i++) {
			if (l0Table[i] != 0) {
				// No there is at-least one valid entry in this dir.
				canFree = false;
				break;
			}
		}
		if (!canFree)
			continue;

		// There are no valid entries in L0 table. Free it.
		freePageTableBlock(l0Table);
		topLevelDir = nullptr;
		break;
	}

	// Flush the TLB to remove any stale references to the now unmapped pages.
	// Also invalidate the branch predictor.
	ARCH_SPECIFIC_TLB_FLUSH;
}

TzMem::PhysAddr PageTable::lookUp(TzMem::VirtAddr vaddr, PageTable::EntryAttribs *attribs) const {
	SpinLocker locker(&lock);

	return lookUpNoLock(vaddr, attribs);
}

TzMem::PhysAddr PageTable::lookUpNoLock(TzMem::VirtAddr vaddr, PageTable::EntryAttribs *attribs) const {


	if (topLevelDir == nullptr)
		return nullptr;

	const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	const uint64_t l0Entry = topLevelDir[l0Idx];
	if (l0Entry == 0)
		return nullptr;

	uint32_t blockAddr = (uint32_t)(l0Entry & L1_BLOCK_ADDR_MASK);
	const uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((uint64_t *)(uintptr_t)blockAddr);
	const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	const uint64_t l1Entry = l1Dir[l1Idx];
	if (l1Entry == 0)
		return nullptr;

	blockAddr = (uint32_t)(l1Entry & L2_BLOCK_ADDR_MASK);
	const uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((uint64_t *)(uintptr_t)blockAddr);
	const int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	const uint64_t l2Entry = l2Dir[l2Idx];
	if (l2Entry == 0)
		return nullptr;

	blockAddr = (uint32_t)(l2Entry & L3_BLOCK_ADDR_MASK);
	const uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((uint64_t *)(uintptr_t)blockAddr);
	const int l3Idx = L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	const uint64_t l3Entry = l3Dir[l3Idx];

	if ((l3Entry & 0x3) == 0x3) {
		uint64_t physAddrBase = l3Entry & L3_PHYS_ADDR_MASK;
		uint64_t physAddr = physAddrBase | ((uintptr_t)vaddr & ~L3_PHYS_ADDR_MASK);

		if (attribs != nullptr) {
			uint8_t swBits = GET_MEMORY_ACCESS_SW_BITS(l3Entry);
			uint8_t nsBit = GET_MEMORY_NS_BIT(l3Entry);
			attribs->swAttribs = (PageTable::SwAttribs)swBits;
			attribs->nonSecure = (nsBit == 1) ? true : false;
		}

		return (TzMem::PhysAddr)(physAddr);
	}

	return nullptr;
}

bool PageTable::isAddrMapped(const TzMem::VirtAddr addr) {
	SpinLocker locker(&lock);
	TzMem::VirtAddr pageBoundary = (TzMem::VirtAddr)((unsigned long)addr & PAGE_MASK);
	if (lookUpNoLock(pageBoundary) == nullptr)
		return false;
	return true;

}

bool PageTable::isAddrRangeMapped(const TzMem::VirtAddr addr, const unsigned int rangeSize) {
	SpinLocker locker(&lock);
	int numPages = rangeSize/PAGE_SIZE_4K_BYTES;
	if (addr != PAGE_START_4K(addr))
		numPages++;

	uint8_t *currPage = (uint8_t *)PAGE_START_4K(addr);
	for (int i=0; i<numPages; i++) {
		if (lookUpNoLock(currPage) == nullptr)
			return false;

		currPage += PAGE_SIZE_4K_BYTES;
	}

	return true;
}

bool PageTable::isAddrRangeUnMapped(const TzMem::VirtAddr addr, const unsigned int rangeSize) {
	SpinLocker locker(&lock);
	int numPages = rangeSize/PAGE_SIZE_4K_BYTES;
	if (addr != PAGE_START_4K(addr))
		numPages++;

	uint8_t *currPage = (uint8_t *)PAGE_START_4K(addr);
	for (int i=0; i<numPages; i++) {
		if (lookUpNoLock(currPage) != nullptr)
			return false;

		currPage += PAGE_SIZE_4K_BYTES;
	}

	return true;
}

TzMem::VirtAddr PageTable::reserveAddrRange(const TzMem::VirtAddr fromAddr, unsigned int rangeSize, ScanDirection direction) {

	SpinLocker locker(&lock);

	TzMem::VirtAddr nextAddr = fromAddr;
	TzMem::VirtAddr rangeStart = nextAddr;
	int currRangeSize = 0;

    if(rangeSize==0) {
        err_msg("%s:\n\t Attempt to reserve address range of zero size.\n", __PRETTY_FUNCTION__);
        kernelHalt("Bad page table request");

    }
	while ((currRangeSize < rangeSize) && ((unsigned long)nextAddr > 0x0000000000000000UL) && ((unsigned long)nextAddr < 0xFFFFFFFFFFFFFFFFUL)) {

		TzMem::PhysAddr pa = lookUpNoLock(nextAddr);

		bool mapped = (pa != nullptr);
		//printf("pa %p nextAddr %p %s mapped\n", pa, nextAddr, mapped ? "is" : "is not");

		nextAddr = (direction == ScanForward) ? (uint8_t *)nextAddr + PAGE_SIZE_4K_BYTES : (uint8_t *)nextAddr - PAGE_SIZE_4K_BYTES;
		if (mapped) {
			currRangeSize = 0;
			rangeStart = nextAddr;
		}
		else {
			currRangeSize += PAGE_SIZE_4K_BYTES;
		}
	}

    if (direction == ScanForward) {
        // rangeStart points to the first page
        // nextAddr points to the page beyond the last one
        reserveRange((uint8_t *)rangeStart,
                     (uint8_t *)nextAddr - 1);
		return rangeStart;
    }
    else {
        // rangeStart points to the last page
        // nextAddr points to the page before the first one
        reserveRange((uint8_t *)nextAddr   + PAGE_SIZE_4K_BYTES,
                     (uint8_t *)rangeStart + PAGE_SIZE_4K_BYTES -1);
        return (TzMem::VirtAddr)((uint8_t *)nextAddr   + PAGE_SIZE_4K_BYTES);
    }


}


void PageTable::unmapBootstrap(const void *devTree) {
	UNUSED(devTree);

	if (this != kernPageTable)
		return;

	// Set top level directory to user translation base register (TTBR0)
	// where the bootstrap was mapped
	register unsigned int ttbr0Low, ttbr0High;
	ARCH_SPECIFIC_GET_TTBR0(ttbr0Low,ttbr0High);
	uint64_t *l0Table = (uint64_t *)TzMem::physToVirt((void *)(((uint64_t)ttbr0High << 32) | ttbr0Low));

	TzMem::VirtAddr vaddr = (void *)devTree;
	TzMem::VirtAddr vaddrLastPage = (void *)((uintptr_t)devTree + MAX_DT_SIZE_BYTES - 1);

	while (vaddr <= vaddrLastPage) {
		const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l0Table=%p l0Idx=%d\n",(void *)l0Table,l0Idx);
		const uint64_t l0Desc = l0Table[l0Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l0Desc)) {
			vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
			continue;
		}

		uint64_t *l1Table = (uint64_t *)TABLE_PHYS_ADDR(l0Table[l0Idx]);
		l1Table = (uint64_t *)TzMem::physToVirt((void *)l1Table);

		int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l1Table=%p l1Idx=%d\n",(void *)l1Table,l1Idx);
		const uint64_t l1Desc = l1Table[l1Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l1Desc)) {
			vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
			continue;
		}

		uint64_t *l2Table = (uint64_t *)TABLE_PHYS_ADDR(l1Desc);
		l2Table = (uint64_t *)TzMem::physToVirt(l2Table);

		int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l2Table=%p l2Idx=%d addr=%p\n",(void *)l2Table,l2Idx,(void *)&l2Table[l2Idx]);
		const uint64_t l2Desc = l2Table[l2Idx];
		if (!IS_VALID_TABLE_DESCRIPTOR(l2Desc)) {
			if (IS_VALID_BLOCK_DESCRIPTOR(l2Desc)) {
				l2Table[l2Idx] = INVALID_TABLE_DESCRIPTOR;

				ARCH_SPECIFIC_DCCIMVAC(&l2Table[l2Idx]);
				asm volatile("dsb ishst":::"memory");
				asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
				asm volatile("dsb ish":::"memory");

				vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_2M_BYTES);
				continue;
			}

			vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
			continue;
		}

		uint64_t *l3Table = (uint64_t *)TABLE_PHYS_ADDR(l2Desc);
		l3Table = (uint64_t *)TzMem::physToVirt(l3Table);

		int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
		//printf("l3Table=%p l3Idx=%d addr=%p\n",(void *)l3Table,l3Idx,(void *)&l3Table[l3Idx]);
		if (l3Table[l3Idx] == 0) {
			err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
			kernelHalt("Attempted unmap a page that wasn't mapped");
		}

		// Unmap the page.
		//printf("UnMapping vaddr %p: l3Idx %d\n", vaddr, l3Idx);
		uint64_t l3Entry = INVALID_TABLE_DESCRIPTOR;
		l3Table[l3Idx] = l3Entry;
		// We wrote the entry into cached memory. Force it to
		// main memory by doing a DCCIMVAC on the entry.
		ARCH_SPECIFIC_DCCIMVAC(&l3Table[l3Idx]);
		asm volatile("dsb ishst":::"memory");
		asm volatile("tlbi VAE1IS, %[xt]"::[xt] "r" (vaddr));
		asm volatile("dsb ish":::"memory");

		vaddr = (TzMem::VirtAddr)((uint8_t *)vaddr + PAGE_SIZE_4K_BYTES);
	}

	// Flush the TLB to remove any stale references to the now unmapped pages.
	// Also invalidate the branch predictor.
	ARCH_SPECIFIC_TLB_FLUSH;
}

void PageTable::copyOnWrite(void *va) {
	UNUSED(va);
	//TODO: Move from TzTask::dataAbortException
}

void PageTable::makePageCopyOnWrite(const TzMem::VirtAddr vaddr) {
	const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (topLevelDir[l0Idx] == 0) {
		err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l0Entry = topLevelDir[l0Idx];
	uint32_t blockAddr = (uint32_t)(l0Entry & L1_BLOCK_ADDR_MASK);
	uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);
	const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l1Dir[l1Idx] == 0) {
		err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l1Entry = l1Dir[l1Idx];
	blockAddr = (uint32_t)(l1Entry & L2_BLOCK_ADDR_MASK);
	uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);

	int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l2Dir[l2Idx] == 0) {
		err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l2Entry = l2Dir[l2Idx];
	blockAddr = (uint32_t)(l2Entry & L3_BLOCK_ADDR_MASK);
	uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);

	int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l3Dir[l3Idx] == 0) {
		err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l3Entry = l3Dir[l3Idx];

	CLEAR_MEMORY_ACCESS_PERMS(l3Entry);
	SET_MEMORY_ACCESS_PERMS(l3Entry, MEMORY_ACCESS_RO_USER);

	CLEAR_MEMORY_ACCESS_SW_BITS(l3Entry);
	SET_MEMORY_ACCESS_SW_BITS(l3Entry, AllocOnWrite);

	l3Dir[l3Idx] = l3Entry;

	// We wrote the entry into cached memory. Force it to
	// main memory by doing a DCCIMVAC on the entry.
	ARCH_SPECIFIC_DCCIMVAC(&l3Dir[l3Idx]);
}

void PageTable::changePageAccessPerms(TzMem::VirtAddr vaddr, int accessPerms, bool noExec) {
	const int l0Idx = L0_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (topLevelDir[l0Idx] == 0) {
		err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l0Entry = topLevelDir[l0Idx];
	uint32_t blockAddr = (uint32_t)(l0Entry & L1_BLOCK_ADDR_MASK);
	uint64_t *l1Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);

	const int l1Idx = L1_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l1Dir[l1Idx] == 0) {
		err_msg("%s:\n\t Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l1Entry = l1Dir[l1Idx];
	blockAddr = (uint32_t)(l1Entry & L2_BLOCK_ADDR_MASK);
	uint64_t *l2Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);

	int l2Idx = L2_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l2Dir[l2Idx] == 0) {
		err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l2Entry = l2Dir[l2Idx];
	blockAddr = (uint32_t)(l2Entry & L3_BLOCK_ADDR_MASK);
	uint64_t *l3Dir = (uint64_t *)TzMem::physToVirt((void *)(uintptr_t)blockAddr);

	int l3Idx =  L3_PAGE_TABLE_SLOT((uintptr_t) vaddr);
	if (l3Dir[l3Idx] == 0) {
		err_msg("%s:\n\t  Attempt to unmap page %p that wasnt mapped\n", __PRETTY_FUNCTION__, vaddr);
		kernelHalt("Attempted unmap a page that wasn't mapped");
	}

	uint64_t l3Entry = l3Dir[l3Idx];

	CLEAR_MEMORY_ACCESS_PERMS(l3Entry);
	SET_MEMORY_ACCESS_PERMS(l3Entry, accessPerms);

	if (noExec)
		SET_MEMORY_ACCESS_NO_EXEC(l3Entry);

	l3Dir[l3Idx] = l3Entry;

	// We wrote the entry into cached memory. Force it to
	// main memory by doing a DCCIMVAC on the entry.
	ARCH_SPECIFIC_DCCIMVAC(&l3Dir[l3Idx]);
}
