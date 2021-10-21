/* kern-actitask.c
 *
 * This file contains the OS_ActiTask function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-actitask.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ActiTask
 *
 * The specified task is activated. The task *must* be in the
 * SUSPENDED or QUARANTINED state.
*/
void OS_ActiTask(const os_task_t *tp)
{
	os_taskdynamic_t *td;

	td = tp->dynamic;

	td->prio = tp->queuePrio;

	OS_TRACE_STATE_TASK( OS_GetKernelData()->taskCurrent->taskId, td->state, OS_TS_NEW );
	td->state = OS_TS_NEW;

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
	OS_ClearCurrentActivation(td);
#endif
	OS_ClearPendingEvents(td);

	/* Reset the task's execution time measurement
	 *
	 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
	*/
	OS_InitTaskEtb(tp);

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
	OS_EnqueueTask(tp, tp->queuePrio);
#else
	OS_Enqueue(tp);
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
