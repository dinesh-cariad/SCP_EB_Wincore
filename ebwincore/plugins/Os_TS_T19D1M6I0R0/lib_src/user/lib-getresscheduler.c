/* lib-getresscheduler.c
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-getresscheduler.c 30633 2019-08-12 14:02:48Z ache271795 $
*/

#include <Os_kernel.h>
#include <Os_api.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "This file is not meant for Safety OS. Check your Makefiles."
#endif
#if (OS_N_CORES < 2)
#error "This file is not meant for single-core configurations. Check your Makefiles."
#endif

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetResScheduler implements the API RES_SCHEDULER for multi-core systems.
 *
 * It returns the RES_SCHEDULER resource of the current core.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetResScheduler, 1
*/
os_resourceid_t OS_GetResScheduler(void)
{
	return OS_resSchedulerMap[OS_ArchGetCoreId()];
}
#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

