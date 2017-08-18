/*
 * DSLCPE specific functions
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id$
 */

#include <typedefs.h>
#include <linuxver.h>

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>

#include <osl.h>

#include <wlioctl.h>
#include "bcm_map.h"
#include "bcm_intr.h"
#include "board.h"
#include "bcmnet.h"
#include "boardparms.h"
#include <wl_linux_dslcpe.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30)
typedef struct wl_info wl_info_t;
extern int wl_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
extern int __devinit wl_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
extern void wl_free(wl_info_t *wl);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
extern irqreturn_t wl_isr(int irq, void *dev_id);
#else
extern irqreturn_t wl_isr(int irq, void *dev_id, struct pt_regs *ptregs);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20) */

static struct net_device_ops wl_dslcpe_netdev_ops;
#endif /* if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30) */

#include <bcmendian.h>
#include <bcmdevs.h>

#ifdef DSLCPE_SDIO
#include <bcmsdh.h>
extern struct wl_info *sdio_dev;
void *bcmsdh_osh = NULL;
#endif /* DSLCPE_SDIO */

/* USBAP */
#ifdef BCMDBUS
#include "dbus.h"
/* BMAC_NOTES: Remove, but just in case your Linux system has this defined */
#undef CONFIG_PCI
void *wl_dbus_probe_cb(void *arg, const char *desc, uint32 bustype, uint32 hdrlen);
void wl_dbus_disconnect_cb(void *arg);
#endif

#ifdef BCMDBG
extern int msglevel;
#endif
extern struct pci_device_id wl_id_table[];

extern bool
wlc_chipmatch(uint16 vendor, uint16 device);

struct net_device *
wl_netdev_get(struct wl_info *wl);
extern void wl_reset_cnt(struct net_device *dev);

/*
 * wl_dslcpe_open:
 * extended hook for device open for DSLCPE.
 */
int wl_dslcpe_open(struct net_device *dev)
{
	return 0;
}

/*
 * wl_dslcpe_close:
 * extended hook for device close for DSLCPE.
 */
int wl_dslcpe_close(struct net_device *dev)
{
	return 0;
}
/*
 * wlc_dslcpe_boardflags:
 * extended hook for modifying boardflags for DSLCPE.
 */
void wlc_dslcpe_boardflags(uint32 *boardflags, uint32 *boardflags2)
{
	return;
}

/*
 * wlc_dslcpe_led_attach:
 * extended hook for when led is to be initialized for DSLCPE.
 */

void wlc_dslcpe_led_attach(void *config, dslcpe_setup_wlan_led_t setup_dslcpe_wlan_led)
{
#if defined(CONFIG_BCM96816)
	setup_dslcpe_wlan_led(config, 0, 1, WL_LED_ACTIVITY, 0);
	setup_dslcpe_wlan_led(config, 1, 1, WL_LED_BRADIO, 0);
#else
	setup_dslcpe_wlan_led(config, 0, 0, WL_LED_ACTIVITY, 1);
	setup_dslcpe_wlan_led(config, 1, 1, WL_LED_BRADIO, 1);
#endif
	return;
}

/*
 * wlc_dslcpe_led_detach:
 * extended hook for when led is to be de-initialized for DSLCPE.
 */
void wlc_dslcpe_led_detach(void)
{
	return;
}
/*
 * wlc_dslcpe_timer_led_blink_timer:
 * extended hook for when periodical(10ms) led timer is called for DSLCPE when wlc is up.
 */
void wlc_dslcpe_timer_led_blink_timer(void)
{
	return;
}
/*
 * wlc_dslcpe_led_timer:
 * extended hook for when led blink timer(200ms) is called for DSLCPE when wlc is up.
 */
void wlc_dslcpe_led_timer(void)
{
	return;
}

/*
 * wl_dslcpe_ioctl:
 * extended ioctl support on BCM63XX.
 */
int
wl_dslcpe_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int isup = 0;
	int error = -1;

	if (cmd >= SIOCGLINKSTATE && cmd < SIOCLAST) {
		error = 0;
		/* we can add sub-command in ifr_data if we need to in the future */
		switch (cmd) {
			case SIOCGLINKSTATE:
				if (dev->flags&IFF_UP) isup = 1;
				if (copy_to_user((void*)(int*)ifr->ifr_data, (void*)&isup,
					sizeof(int))) {
					return -EFAULT;
				}
				break;
			case SIOCSCLEARMIBCNTR:
				wl_reset_cnt(dev);
				break;
		}
	} else {
		error = wl_ioctl(dev, ifr, cmd);
	}
	return error;
}

#if defined(DSLCPE_SDIO) || defined(CONFIG_PCI)
/*
 * wl_dslcpe_isr:
 */
irqreturn_t
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
wl_dslcpe_isr(int irq, void *dev_id)
#else
wl_dslcpe_isr(int irq, void *dev_id, struct pt_regs *ptregs)
#endif	/* < 2.6.20 */
{
	bool ours;

#ifdef DSLCPE_SDIO
	ours = bcmsdh_intr_handler(wl_sdh_get((struct wl_info *)dev_id));
#else
	/* ignore wl_isr return value due to dedicated interrupt line */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
	ours = wl_isr(irq, dev_id);
#else
	ours = wl_isr(irq, dev_id, ptregs);
#endif	/* < 2.6.20 */
#endif /* DSLCPE_SDIO */

	BcmHalInterruptEnable(irq);

	return IRQ_RETVAL(ours);
}

/* special deal for dslcpe */
int __devinit
wl_dslcpe_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct wl_info *wl;
	struct net_device *dev;
	int irq;

#ifdef DSLCPE_SDIO
	wl = (struct wl_info *)sdio_dev;
#else
	if (wl_pci_probe(pdev, ent))
		return -ENODEV;

	wl = pci_get_drvdata(pdev);
#endif
	ASSERT(wl);

	/* hook ioctl */
	dev = wl_netdev_get(wl);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30)
	memcpy(&wl_dslcpe_netdev_ops, dev->netdev_ops, sizeof(struct net_device_ops));
	wl_dslcpe_netdev_ops.ndo_do_ioctl = wl_dslcpe_ioctl;
	dev->netdev_ops = &wl_dslcpe_netdev_ops;
#else
	ASSERT(dev);
	ASSERT(dev->do_ioctl);
	dev->do_ioctl = wl_dslcpe_ioctl;
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30) */


#ifdef DSLCPE_SDIO
	irq = dev->irq;
#else
	irq = pdev->irq;
#endif
	if (irq && wl) {
		free_irq(irq, wl);
		if (BcmHalMapInterrupt((FN_HANDLER)wl_dslcpe_isr, (unsigned int)wl, irq)) {
			printk("wl_dslcpe: request_irq() failed\n");
			goto fail;
		}
		BcmHalInterruptEnable(irq);
	}

	return 0;

fail:
	wl_free(wl);
	return -ENODEV;
}

#ifndef DSLCPE_SDIO
void __devexit wl_remove(struct pci_dev *pdev);

static struct pci_driver wl_pci_driver = {
	name:		"wl",
	probe:		wl_dslcpe_probe,
	remove:		__devexit_p(wl_remove),
	id_table:	wl_id_table,
	};
#endif
#endif  /* defined(DSLCPE_SDIO) || defined(CONFIG_PCI) */

/* USBAP  Could combined with wl_dslcpe_probe */
#ifdef BCMDBUS
static void *wl_dslcpe_dbus_probe_cb(void *arg, const char *desc, uint32 bustype, uint32 hdrlen)
{
	struct net_device *dev;
	wl_info_t *wl = wl_dbus_probe_cb(arg, desc, bustype, hdrlen);
	int irq;

	ASSERT(wl);

	/* hook ioctl */
	dev = wl_netdev_get(wl);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30)
	memcpy(&wl_dslcpe_netdev_ops, dev->netdev_ops, sizeof(struct net_device_ops));
	wl_dslcpe_netdev_ops.ndo_do_ioctl = wl_dslcpe_ioctl;
	dev->netdev_ops = &wl_dslcpe_netdev_ops;
#else
	ASSERT(dev);
	ASSERT(dev->do_ioctl);
	dev->do_ioctl = wl_dslcpe_ioctl;
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30) */

#ifdef DSLCPE_DGASP
	kerSysRegisterDyingGaspHandler(dev->name, &wl_shutdown_handler, wl);
#endif
	return 0;
}

static void wl_dslcpe_dbus_disconnect_cb(void *arg)
{
	wl_dbus_disconnect_cb(arg);
}
#endif /* BCMDBUS */

static int __init
wl_module_init(void)
{

	int error;
#ifdef CONFIG_SMP
	printk("--SMP support\n");
#endif
#ifdef CONFIG_BCM_WAPI
	printk("--WAPI support\n");
#endif

#ifdef BCMDBG
	if (msglevel != 0xdeadbeef) {
		/* wl_msg_level = msglevel; */
		printf("%s: msglevel set to 0x%x\n", __FUNCTION__, msglevel);
	}
#endif /* BCMDBG */

#ifdef DSLCPE_SDIO
	bcmsdh_osh = osl_attach(NULL, SDIO_BUS, FALSE);

	if (!(error = wl_sdio_register(VENDOR_BROADCOM, BCM4318_CHIP_ID,
		(void *)0xfffe2300, bcmsdh_osh, INTERRUPT_ID_SDIO))) {
		if((error = wl_dslcpe_probe(0, 0)) != 0) {	/* to hookup entry points or misc */
			osl_detach(bcmsdh_osh);
			return error;
		}
	} else {
		osl_detach(bcmsdh_osh);
	}

#endif  /* DSLCPE_SDIO */

#ifdef CONFIG_PCI
	if (!(error = pci_module_init(&wl_pci_driver)))
		return (0);
#endif /* CONFIG_PCI */

#ifdef BCMDBUS
	/* BMAC_NOTE: define hardcode number, why NODEVICE is ok ? */
	error = dbus_register(BCM_DNGL_VID, BCM_DNGL_BDC_PID, wl_dslcpe_dbus_probe_cb,
		wl_dslcpe_dbus_disconnect_cb, NULL, NULL, NULL);
	if (error == DBUS_ERR_NODEVICE) {
		error = DBUS_OK;
	}
#endif /* BCMDBUS */
	return (error);
}

static void __exit
wl_module_exit(void)
{
#ifdef DSLCPE_SDIO
	wl_sdio_unregister();
	osl_detach(bcmsdh_osh);
#endif /* DSLCPE_SDIO */

#ifdef CONFIG_PCI
	pci_unregister_driver(&wl_pci_driver);
#endif	/* CONFIG_PCI */

#ifdef BCMDBUS
	dbus_deregister();
#endif /* BCMDBUS */
}

/* Turn 63xx GPIO LED On(1) or Off(0) */
void wl_dslcpe_led(unsigned char state)
{
/* if WLAN LED is from 63XX GPIO Line, define compiler flag GPIO_LED_FROM_63XX
#define GPIO_LED_FROM_63XX
*/

#ifdef GPIO_LED_FROM_63XX
	BOARD_LED_STATE led;
	led = state? kLedStateOn : kLedStateOff;

	kerSysLedCtrl(kLedSes, led);
#endif
}

module_init(wl_module_init);
module_exit(wl_module_exit);
MODULE_LICENSE("Proprietary");
