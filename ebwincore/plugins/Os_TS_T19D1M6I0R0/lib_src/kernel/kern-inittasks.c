/* kern-inittasks.c
 *
 * This file contains the OS_InitTasks() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-inittasks.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitTasks
 *
 * The head of each task's last resource queue needs to be set to NULLRESOURCE.
 * This is because the default bss value (0) is a valid resource id.
*/
void OS_InitTasks(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_taskid_t tid;

	for (tid = 0; tid < OS_nTasks; tid++)
	{
		os_task_t const * const tp = &OS_taskTableBase[tid];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetTaskCoreId(tp) == myCoreId)
		{
			tp->dynamic->lastLock = OS_NULLOBJECT;
		}
	}

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
	{
		os_int_t i;
		for (i = 0; i < OS_nPrioritySlots[myCoreId]; i++)
		{
			*OS_GetPrioritySlotOfCore(myCoreId, i) = OS_NULLTASK;
		}
	}
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
