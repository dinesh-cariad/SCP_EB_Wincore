/* Os_configuration_user.c
 *
 * This file contains the architecture-independent kernel configuration data,
 * which depends on user identifiers.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_user.c 26587 2017-04-28 10:58:05Z mist9353 $
*/

#include <Os_kernel.h>

/* Configuration dependent headers. */
#include <Os_configuration_decls.h>
#include <Os_user.h>


#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "This file is not meant for Safety OS. Check your Makefiles."
#endif
#if (OS_N_CORES < 2)
#error "This file is not meant for single-core configurations. Check your Makefiles."
#endif

/* This value should never be used.
 * Let's use a value which is likely to cause an error.
*/
#define OS_NULL_RES_SCHEDULER ((os_resourceid_t)~(os_resourceid_t)0u)

#ifndef RES_SCHEDULER_0
#define RES_SCHEDULER_0 OS_NULL_RES_SCHEDULER
#endif

#if (OS_N_CORES_MAX > 1)
#ifndef RES_SCHEDULER_1
#define RES_SCHEDULER_1 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 2)
#ifndef RES_SCHEDULER_2
#define RES_SCHEDULER_2 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 3)
#ifndef RES_SCHEDULER_3
#define RES_SCHEDULER_3 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 4)
#ifndef RES_SCHEDULER_4
#define RES_SCHEDULER_4 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 5)
#ifndef RES_SCHEDULER_5
#define RES_SCHEDULER_5 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 6)
#ifndef RES_SCHEDULER_6
#define RES_SCHEDULER_6 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 7)
#ifndef RES_SCHEDULER_7
#define RES_SCHEDULER_7 OS_NULL_RES_SCHEDULER
#endif
#endif

#if (OS_N_CORES_MAX > 8)
#error "Only up to 8 cores are supported."
#endif

#include <memmap/Os_mm_const_begin.h>
const os_resourceid_t OS_resSchedulerMap[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(RES_SCHEDULER_) };
#include <memmap/Os_mm_const_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
