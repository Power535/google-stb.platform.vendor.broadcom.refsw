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

#include <linux/version.h>
#include <linux/kconfig.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/opcodes-smc.h>
#include <linux/brcmstb/memory_api.h>

#include "astra_version.h"
#include "tzioc_api.h"
#include "tzioc_ioctls.h"
#include "tzioc_drv.h"
#include "tzioc_ioctl.h"
#include "tzioc_msg.h"
#include "tzioc_mem.h"
#include "tzioc_client.h"
#include "tzioc_sys_msg.h"
#include "tracelog.h"
#include "vuart.h"

#if KERNEL_BCM_MQ_PATCH
extern long bcm_mq_timedsend(
    struct task_struct *mqtask,
    mqd_t mqdes,
    const char *u_msg_ptr,
    size_t msg_len,
    unsigned int msg_prio,
    const struct timespec *u_abs_timeout);
#endif

/*
 * Function Declarations
 */

#if TZIOC_DEV_SUPPORT
static int tzioc_mdev_open(
    struct inode *inode,
    struct file *file);

static int tzioc_mdev_release(
    struct inode *ignored,
    struct file *file);

static int tzioc_mdev_mmap(
    struct file *file,
    struct vm_area_struct *vma);

static long tzioc_mdev_ioctl(
    struct file *file,
    unsigned int cmd,
    unsigned long arg);
#endif /* TZIOC_DEV_SUPPORT */

static int  tzioc_module_init(void);
static void tzioc_module_exit(void);
static int  tzioc_module_deinit(void);
static int  tzioc_module_reset(void);

static void tzioc_isr(void);
static void tzioc_proc(struct work_struct *work);

#if CPUTIME_ACCOUNTING
static void tzioc_cputime_account(void);
#endif

static int tzioc_sys_msg_proc(struct tzioc_msg_hdr *pHdr);
#if TZIOC_MSG_ECHO
static int tzioc_echo_msg_proc(struct tzioc_msg_hdr *pHdr);
#endif /*TZIOC_MSG_ECHO */

extern int astra_module_init(void);
extern void astra_module_exit(void);
extern int astra_module_deinit(void);

/*
 * Variable Declarations
 */

#if TZIOC_DEV_SUPPORT
static char tzioc_mdev_name[16] = "tzioc";
module_param_string(mdevname, tzioc_mdev_name, sizeof(tzioc_mdev_name), 0);

static const struct file_operations tzioc_mdev_fops = {
    .owner          = THIS_MODULE,
    .read           = NULL,
    .write          = NULL,
    .open           = tzioc_mdev_open,
    .release        = tzioc_mdev_release,
    .unlocked_ioctl = tzioc_mdev_ioctl,
    .mmap           = tzioc_mdev_mmap,
};

static struct miscdevice tzioc_mdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = tzioc_mdev_name,
    .fops = &tzioc_mdev_fops,
};
#endif /* TZIOC_DEV_SUPPORT */

static struct tzioc_device tzioc_tdev;
struct tzioc_device *tdev = &tzioc_tdev;

#if TZIOC_DEV_SUPPORT

/*
 * Misc Device Functions
 */

static int tzioc_mdev_open(
    struct inode *inode,
    struct file *file)
{
    int err = 0;

    err = generic_file_open(inode, file);
    if (unlikely(err)) {
        LOGE("Failed to open inode!");
        return err;
    }
    return 0;
}

static int tzioc_mdev_release(
    struct inode *ignored,
    struct file *file)
{
    return 0;
}

static int tzioc_mdev_mmap(
    struct file *file,
    struct vm_area_struct *vma)
{
    unsigned offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned size = vma->vm_end - vma->vm_start;

    if (offset + size > tdev->smemSize)
        return -EINVAL;

#if IOREMAP_SHARED_MEM
    /* allow uncached mmap */
    if (file->f_flags & O_SYNC)
        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#endif

    return remap_pfn_range(
        vma,
        vma->vm_start,
        (tdev->smemStart + offset) >> PAGE_SHIFT,
        size,
        vma->vm_page_prot);
}

static long tzioc_mdev_ioctl(
    struct file *file,
    unsigned int cmd,
    unsigned long arg)
{
    if (!file) {
        LOGE("Invalid file in ioctl call");
        return -EBADF;
    }

    if (cmd < TZIOC_IOCTL_FIRST ||
        cmd >= TZIOC_IOCTL_LAST) {
        LOGE("Invalid cmd 0x%x in ioctl call", cmd);
        return -EINVAL;
    }

    return _tzioc_ioctl_do_ioctl(cmd, (void *)arg);
}

#endif /* TZIOC_DEV_SUPPORT */

/*
 * TZIOC Module Functions
 */

static int tzioc_module_init(void)
{
    struct work_struct sysIrqWorkSave;
    struct device_node *node;
    const uint32_t *smem_params;
    uint64_t smemStart64, smemSize64;
    uint32_t version;
    uint32_t sysIrq;
    struct tzioc_msg_hdr hdr;
    int err = 0;

    /* check if in sysIrqWork context */
    if (current_work() == &tdev->sysIrqWork)
        /* save sysIrqWork */
        sysIrqWorkSave = tdev->sysIrqWork;

    /* clear TZIOC device control block */
    memset(tdev, 0, sizeof(*tdev));

    /* find TZIOC device tree node */
    node = of_find_node_by_name(NULL, "tzioc");
    if (!node) {
        LOGE("No TZIOC node in device tree");
        err = -ENOENT;
        goto ERR_EXIT;
    }

    /* get shared memory info from device tree */
    smem_params = of_get_address(node, 0, &smemSize64, NULL);
    if (!smem_params) {
        LOGE("Failed to get TZIOC memory info from device tree");
        err = -EINVAL;
        goto ERR_EXIT;
    }

    smemStart64 = of_translate_address(node, smem_params);

    tdev->smemStart = (uintptr_t)smemStart64;
    tdev->smemSize  = (uintptr_t)smemSize64;
    LOGI("TZIOC shared memory at 0x%zx, size 0x%zx",
         (size_t)tdev->smemStart, (size_t)tdev->smemSize);

    /* map shared memory */
#if IOREMAP_SHARED_MEM
    tdev->psmem = (struct tzioc_shared_mem *)
        ioremap(tdev->smemStart, tdev->smemSize);
#else
    tdev->psmem = (struct tzioc_shared_mem *)
        brcmstb_memory_kva_map_phys(tdev->smemStart, tdev->smemSize, true);
#endif
    if (!tdev->psmem) {
        LOGE("Failed to remap TZIOC memory");
        err = -ENODEV;
        goto ERR_EXIT;
    }

    /* check version */
    version = tdev->psmem->ulMagic;
    if (ASTRA_VERSION_MAJOR_GET(version) != ASTRA_VERSION_MAJOR) {
        LOGE("Mismatched TZIOC Major version 0x%08x, expecting 0x%08x",
             version, ASTRA_VERSION_WORD);
        err = -ENODEV;
        goto ERR_EXIT;
    }
    if (ASTRA_VERSION_MINOR_GET(version) < ASTRA_VERSION_MINOR) {
        LOGE("Not compatible TZIOC Minor version 0x%08x, expecting 0x%08x",
             version, ASTRA_VERSION_WORD);
        err = -ENODEV;
        goto ERR_EXIT;
    }

    /* Remember secure flag */
    tdev->secure = tdev->psmem->secure;

    /* Clear reset flag */
    tdev->psmem->reset = false;

    /* get system IRQ info from device tree */
    err = of_property_read_u32(node, "irq", &sysIrq);
    if (err) {
        LOGE("Failed to get TZIOC system IRQ info from device tree");
        err = -EINVAL;
        goto ERR_EXIT;
    }

    tdev->sysIrq = sysIrq;
    LOGI("TZIOC system IRQ %d", tdev->sysIrq);

    /* check if in sysIrqWork context */
    if (current_work() == &tdev->sysIrqWork)
        /* restore sysIrqWork */
        tdev->sysIrqWork = sysIrqWorkSave;
    else
        /* init work for bottom half */
        INIT_WORK(&tdev->sysIrqWork, tzioc_proc);

#ifdef KERNEL_IPI_PATCH
    /* request system IRQ */
    err = set_ipi_handler(tdev->sysIrq, tzioc_isr, "TZIOC");
    if (err) {
        LOGE("Failed to set TZIOC system IRQ handler");
        tdev->sysIrq = 0;
        goto ERR_EXIT;
    }
#endif

#if TZIOC_DEV_SUPPORT
    /* register misc device */
    err = misc_register(&tzioc_mdev);
    if (err) {
        LOGE("Failed to register misc device");
        goto ERR_EXIT;
    }

    tdev->mdev = &tzioc_mdev;
#endif

    /* init spinlock */
    spin_lock_init(&tdev->lock);

    /* init ioctl module */
    err = _tzioc_ioctl_module_init();
    if (err) {
        LOGE("Failed to init TZIOC ioctl module");
        goto ERR_EXIT;
    }

    /* init msg module */
    err = _tzioc_msg_module_init();
    if (err) {
        LOGE("Failed to init TZIOC msg module");
        goto ERR_EXIT;
    }

    /* init mem module */
    err = _tzioc_mem_module_init();
    if (err) {
        LOGE("Failed to init TZIOC mem module");
        goto ERR_EXIT;
    }

    /* init client module */
    err = _tzioc_client_module_init();
    if (err) {
        LOGE("Failed to init TZIOC client module");
        goto ERR_EXIT;
    }

    /* open system client */
    tdev->pSysClient = _tzioc_kernel_client_open(
        "tzioc_system",
        NULL, /* sys msgs dispatched directly */
        0); /* private data not used */

    if (!tdev->pSysClient) {
        LOGE("Failed to open TZIOC system client");
        goto ERR_EXIT;
    }

#if 0
    if (tracelog_init()) {
        LOGE("Failed to init tracelog driver");
        goto ERR_EXIT;
    }
#endif

    /* send system up cmd to TZOS */
    hdr.ucType = SYS_MSG_UP;
    hdr.ucOrig = TZIOC_CLIENT_ID_SYS;
    hdr.ucDest = TZIOC_CLIENT_ID_SYS;
    hdr.ucSeq  = 0;
    hdr.ulLen  = 0;

    err = _tzioc_msg_send(
        tdev->pSysClient,
        &hdr, NULL);

    if (err) {
        LOGE("Failed to send system up cmd");
        goto ERR_EXIT;
    }

    LOGI("TZIOC initialized");

    /* init astra module */
    err = astra_module_init();
    if (err) {
        LOGE("Failed to init astra module");
        goto ERR_EXIT;
    }

    /* immediately switch to TZOS */
    _tzioc_call_smc(0x83000007);
    return 0;

 ERR_EXIT:
    tzioc_module_deinit();
    return err;
}

static void tzioc_module_exit(void)
{
    tzioc_module_deinit();
}

static int tzioc_module_deinit(void)
{
    struct tzioc_msg_hdr hdr;
    int err = 0;

    /* exit astra module */
    if (tdev->adev)
        astra_module_deinit();

    if (tdev->peerUp) {
        /* send system down msg to TZOS */
        hdr.ucType = SYS_MSG_DOWN;
        hdr.ucOrig = TZIOC_CLIENT_ID_SYS;
        hdr.ucDest = TZIOC_CLIENT_ID_SYS;
        hdr.ucSeq  = 0;
        hdr.ulLen  = 0;

        err = _tzioc_msg_send(
            tdev->pSysClient,
            &hdr, NULL);

        if (err) {
            LOGE("Failed to send system down msg to TZOS");
        }

        /* immediately switch to TZOS */
        _tzioc_call_smc(0x83000007);
    }

    /* exit tracelog */
    tracelog_exit();

    /* exit vuart */
    bcm_vuart_exit();

    /* close system client */
    if (tdev->pSysClient)
        _tzioc_kernel_client_close(tdev->pSysClient);

    /* deinit client module */
    if (tdev->pClientMod)
        _tzioc_client_module_deinit();

    /* deinit mem module */
    if (tdev->pMemMod)
        _tzioc_mem_module_deinit();

    /* deinit msg module */
    if (tdev->pMsgMod)
        _tzioc_msg_module_deinit();

    /* deinit ioctl module */
    if (tdev->pIoctlMod)
        _tzioc_ioctl_module_deinit();

    /* unmap shared memory */
    if (tdev->psmem)
#if IOREMAP_SHARED_MEM
        iounmap(tdev->psmem);
#else
        brcmstb_memory_kva_unmap(tdev->psmem);
#endif

    /* free system IRQ */
#if KERNEL_IPI_PATCH
    if (tdev->sysIrq)
        clear_ipi_handler(tdev->sysIrq);
#endif

    /* check if NOT in sysIrqWork context */
    if (current_work() != &tdev->sysIrqWork)
        /* cancel scheduled work */
        cancel_work_sync(&tdev->sysIrqWork);

#if TZIOC_DEV_SUPPORT
    /* deregister misc device */
    if (tdev->mdev)
        misc_deregister(tdev->mdev);
#endif

    LOGI("TZIOC uninitialized");
    return err;
}

static int tzioc_module_reset(void)
{
    int err = 0;

    /* mark peer down first */
    tdev->peerUp = false;

    /* deinit entire TZIOC kernel module */
    tzioc_module_deinit();

    /* reinit TZIOC kernel module afresh */
    err = tzioc_module_init();
    if (err) {
        LOGE("Failed to reset TZIOC");
        return err;
    }

    LOGI("TZIOC reset");
    return 0;
}

module_init(tzioc_module_init);
module_exit(tzioc_module_exit);
MODULE_LICENSE("GPL");

/*
 * TZIOC System Functions
 */

static void tzioc_isr(void)
{
    if (!work_busy(&tdev->sysIrqWork))
        schedule_work_on(0, &tdev->sysIrqWork);
}

static void tzioc_proc(struct work_struct *work)
{
    LOGD("Received TZIOC system IRQ");

    /* check for peer reset */
    if (tdev->psmem->reset) {
        LOGI("TZIOC peer is reset");

        /* reset TZIOC kernel module */
        tzioc_module_reset();
        return;
    }

#if CPUTIME_ACCOUNTING
    /* account cputime */
    tzioc_cputime_account();
#endif

    /* process incoming msgs */
    while (1) {
        static uint8_t msg[TZIOC_MSG_SIZE_MAX];
        struct tzioc_msg_hdr *pHdr = (struct tzioc_msg_hdr *)msg;
        int err;

        /* get received msg */
        err = _tzioc_msg_receive(
            tdev->pSysClient,
            pHdr,
            TZIOC_MSG_PAYLOAD(pHdr),
            TZIOC_MSG_PAYLOAD_MAX);

        if (err == -ENOMSG)
            break;
        if (err) {
            LOGE("Error receiving msg, err %d", err);
            continue;
        }

        /* dispatch received msg */
        if (pHdr->ucDest == TZIOC_CLIENT_ID_SYS) {
            err = tzioc_sys_msg_proc(pHdr);
        }
#if TZIOC_MSG_ECHO
        else if (pHdr->ucDest == TZIOC_CLIENT_ID_MAX) {
            err = tzioc_echo_msg_proc(pHdr);
        }
#endif
        else {
            struct tzioc_client *pClient;

            pClient = _tzioc_client_find_by_id(pHdr->ucDest);

            if (!pClient) {
                LOGW("Unknown msg dest %d", pHdr->ucDest);
                continue;
            }

            if (pClient->kernel) {
                err = pClient->msgProc(pHdr, pClient->privData);
            }
            else {
                struct task_struct *pTask = (struct task_struct *)pClient->task;
                if (pTask->exit_state)
                    continue;
#if KERNEL_BCM_MQ_PATCH
                err = bcm_mq_timedsend(
                    pTask,
                    (mqd_t)pClient->msgQ,
                    (char *)pHdr,
                    pHdr->ulLen + sizeof(*pHdr),
                    0,
                    NULL);
#endif
            }
        }
    }
}

#if CPUTIME_ACCOUNTING
static void tzioc_cputime_account(void)
{
    static uint64_t tzUsecsPrev = 0;
    uint64_t tzUsecsCurr = tdev->psmem->ullTzUsecs;

    if (tzUsecsPrev) {
        static uint32_t tzUsecsWin[CPUTIME_WINDOW_SIZE];
        static uint32_t tzUsecsSum = 0;
        static int32_t  tzUsecsRem = 0;
        static int idx = 0;
        uint32_t tzCputime;

        tzUsecsSum -= tzUsecsWin[idx];
        tzUsecsWin[idx] = tzUsecsCurr - tzUsecsPrev;
        tzUsecsSum += tzUsecsWin[idx];
        if (++idx == CPUTIME_WINDOW_SIZE) idx = 0;

        /* remaining usecs may be negative due to cputime round-up */
        tzUsecsRem += tzUsecsSum / CPUTIME_WINDOW_SIZE;
        tzCputime = (tzUsecsRem > 0) ? usecs_to_cputime(tzUsecsRem) : 0;
        tzUsecsRem -= cputime_to_usecs(tzCputime);

        if (tzCputime) {
            LOGE("#=%x c=%x", idx, tzCputime);
            account_steal_time(tzCputime);
        }
    }

    tzUsecsPrev = tzUsecsCurr;
}
#endif

static int tzioc_sys_msg_proc(struct tzioc_msg_hdr *pHdr)
{
    int err = 0;

    LOGI("TZIOC system msg processing");

    switch (pHdr->ucType) {
    /* system up rpy */
    case SYS_MSG_UP:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen != 0) {
            LOGE("Invalid system up cmd received");
            err = -EINVAL;
            break;
        }

        tdev->peerUp = true;
        LOGI("TZIOC peer is up");
        break;

    /* system down rpy */
    case SYS_MSG_DOWN:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen != 0) {
            LOGE("Invalid system down cmd received");
            err = -EINVAL;
            break;
        }

        tdev->peerUp = false;
        LOGI("TZIOC peer is down");
        break;

    /* system tracelog msgs */
    case SYS_MSG_TRACELOG_ON:
    case SYS_MSG_TRACELOG_OFF:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen == 0) {
            LOGE("Invalid tracelog cmd received");
            err = -EINVAL;
            break;
        }

        tracelog_msg_proc(pHdr);
        break;

    /* system vuart msgs */
    case SYS_MSG_VUART_ON:
    case SYS_MSG_VUART_OFF:
        if (pHdr->ucOrig != TZIOC_CLIENT_ID_SYS ||
            pHdr->ulLen == 0) {
            LOGE("Invalid vuart cmd received");
            err = -EINVAL;
            break;
        }

        bcm_vuart_msg_proc(pHdr);
        break;

    default:
        LOGW("Unknown system msg %d", pHdr->ucType);
        err = -ENOENT;
    }
    return err;
}

#if TZIOC_MSG_ECHO
static int tzioc_echo_msg_proc(struct tzioc_msg_hdr *pHdr)
{
    uint8_t id;
    int err = 0;

    id = pHdr->ucOrig;
    pHdr->ucOrig = pHdr->ucDest;
    pHdr->ucDest = id;

    err = _tzioc_msg_send(
        tdev->pSysClient,
        pHdr,
        TZIOC_MSG_PAYLOAD(pHdr));

    if (err) {
        LOGE("Failed to send echo msg");
    }
    return err;
}
#endif

int _tzioc_call_smc(uint32_t ulCallnum)
{
#ifdef __aarch64__
    asm volatile(
        "mov x0, %[cn] \r\n"
        "smc #0\r\n"
        : /* No output registers */
        : [cn] "r" (ulCallnum)
        : "x0" /* x0 is clobbered. */
    );
#else
    asm volatile(
        "mov r0, %[cn] \r\n"
        __SMC(0)
        : /* No output registers */
        : [cn] "r" (ulCallnum)
        : "r0" /* r0 is clobbered. */
    );
#endif
    return 0;
}

uintptr_t _tzioc_offset2addr(uintptr_t ulOffset)
{
    if (ulOffset < tdev->smemSize)
        return ulOffset + (uintptr_t)tdev->psmem;
    else
        return (uintptr_t)-1;
}

uintptr_t _tzioc_addr2offset(uintptr_t ulAddr)
{
    if (ulAddr >= (uintptr_t)tdev->psmem &&
        ulAddr <  (uintptr_t)tdev->psmem + tdev->smemSize)
        return ulAddr - (uintptr_t)tdev->psmem;
    else
        return (uintptr_t)-1;
}

uintptr_t _tzioc_vaddr2paddr(uintptr_t ulVaddr)
{
    uintptr_t ulOffset = _tzioc_addr2offset(ulVaddr);
    if (ulOffset != -1)
        return tdev->smemStart + ulOffset;
    else
        return (uintptr_t)-1;
}

uintptr_t _tzioc_paddr2vaddr(uintptr_t ulPaddr)
{
    uintptr_t ulOffset = ulPaddr - tdev->smemStart;
    return _tzioc_offset2addr(ulOffset);
}

int _tzioc_config_get(struct tzioc_config *pConfig)
{
    pConfig->smemSize = tdev->smemSize;
    pConfig->pmemSize = tdev->pmemSize;
    return 0;
}

int _tzioc_status_get(struct tzioc_status *pStatus)
{
    pStatus->up = tdev->peerUp;
    pStatus->secure = tdev->secure;
    return 0;
}
