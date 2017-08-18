/*
 * Threadx OS Support Extension Layer
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 *
 * <<Broadcom-WL-IPTag/Proprietary:>>
 * $Id$
 */


#ifndef _threadx_osl_ext_h_
#define _threadx_osl_ext_h_

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <tx_api.h>
#include <typedefs.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Interrupt control */
#define OSL_INTERRUPT_SAVE_AREA		unsigned int interrupt_save;
#define OSL_DISABLE			interrupt_save = tx_interrupt_control(TX_INT_DISABLE);
#define OSL_RESTORE			tx_interrupt_control(interrupt_save);

/* Debugging helpers */
#define THREAD_NAMEPTR(t)		((TX_THREAD*)t)->tx_thread_name
#define CURRENT_THREAD_NAMEPTR		THREAD_NAMEPTR(osl_ext_task_current())
#define THREAD_ID(t)			((TX_THREAD*)t)->tx_thread_id
#define CURRENT_THREAD_ID		THREAD_ID(osl_ext_task_current())

#define ASSERT_THREAD(name)		ASSERT((name) == CURRENT_THREAD_NAMEPTR)

/* The default depth of the DPC event queue */
#ifndef OSL_DEFAULT_EVENT_DEPTH
#define OSL_DEFAULT_EVENT_DEPTH		32
#endif

/* This is really platform specific and not OS specific. */
#ifndef BWL_THREADX_TICKS_PER_SECOND
#define BWL_THREADX_TICKS_PER_SECOND	1000
#endif

#define OSL_USEC_TO_TICKS(usec)	 (usec)
#define OSL_TICKS_TO_USEC(ticks) (ticks)

/* Semaphore. */
typedef TX_SEMAPHORE osl_ext_sem_t;
#define OSL_EXT_SEM_DECL(sem)		osl_ext_sem_t  sem;

/* Mutex. */
typedef TX_MUTEX osl_ext_mutex_t;
#define OSL_EXT_MUTEX_DECL(mutex)	osl_ext_mutex_t  mutex;

/* Timer. */
typedef TX_TIMER osl_ext_timer_t;
#define OSL_EXT_TIMER_DECL(timer)	osl_ext_timer_t  timer;

/* Task. */
typedef TX_THREAD osl_ext_task_t;
#define OSL_EXT_TASK_DECL(task)		osl_ext_task_t  task;

/* Queue. */
typedef TX_QUEUE osl_ext_queue_t;
#define OSL_EXT_QUEUE_DECL(queue)	osl_ext_queue_t  queue;

/* Event. */
typedef TX_EVENT_FLAGS_GROUP osl_ext_event_t;
#define OSL_EXT_EVENT_DECL(event)	osl_ext_event_t  event;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


#ifdef __cplusplus
	}
#endif

#endif  /* _threadx_osl_ext_h_  */
