/* kern-dequeuetask.c
 *
 * This file contains the OS_DequeueTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dequeuetask.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is never used.
 *
 * Reason: Not an issue, variable will be used if scalability class 2 is used.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the OS configuration. If every queue has only one entry,
 *  OS_Get_PriorityQueueHeadAfterRemoval always returns OS_NULLTASK.
 */

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_DequeueTask
 *
 * The specified task is removed from the head of its given priority queue the new priority is set.
*/
void OS_DequeueTask(const os_task_t *tp, os_prio_t prio)
{
	const os_priorityqueue_t *q = OS_GetPriorityQueueOfCore(OS_GetMyCoreId(), prio);
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_priorityqueuedynamic_t *qd = OS_Get_PriorityQueueDynamic(q);
	os_priorityqueueentry_t newTask;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_DEVSANITYCHECK(OS_Get_PriorityQueueHead(prio, q, qd) != tp->taskId) )
	{
		/* PANIC!!! Task is not in priority queue */
		/* can't propagate the return value of OS_PANIC -> ignore it */
		(void) OS_PANIC(OS_PANIC_TaskIsNotInPriorityQueue);
	}
	else
	{
		OS_Remove_PriorityQueueHead(prio, q, qd);

		/* If the queue becomes empty, clear its bit(s) in the priority-word(s)
		 * and recalculate OS_GetKernelData()->taskQueueHead
		*/
		newTask = OS_Get_PriorityQueueHeadAfterRemoval(q, qd);
		/* Possible diagnostic TOOLDIAG-2 <1> */
		if ( newTask == OS_NULLTASK )
		{
			/* No more tasks at this priority, so we need to clear the appropriate bits in the
			 * the master/slave words and if the dequeued task was at the queue head, recalculate
			 * the new queue head from scratch.
			*/
			OS_Clr_PriorityBits(q);

			if ( kernel_data->taskQueueHead == tp )
			{
				newTask = OS_FindHighestTask();

				kernel_data->taskQueueHead = (newTask == OS_NULLTASK) ? OS_NULL : &OS_taskTableBase[newTask];
			}
		}
		else
		{
			/* More tasks at the same priority, so if the dequeued task was at the queue head
			 * we simply set it to the next task in the same priority queue.
			*/
			if ( kernel_data->taskQueueHead == tp )
			{
				kernel_data->taskQueueHead = &OS_taskTableBase[newTask];
			}
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
