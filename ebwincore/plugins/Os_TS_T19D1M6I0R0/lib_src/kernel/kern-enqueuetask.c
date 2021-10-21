/* kern-enqueuetask.c
 *
 * This file contains the OS_EnqueueTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-enqueuetask.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_EnqueueTask
 *
 * The specified task is added to the end of the priority fifo of the given priority.
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.NonSuspendedTasksBCC, 1
 * !LINKSTO Kernel.Architecture.ConformanceClasses.NonSuspendedTasksECC, 1
 *		The generator creates queues of appropriate dimensions such that all tasks can be active simultaneously
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.TasksPerPrio, 1
 *		The number of tasks per priority is determined by the generator.
 *
 * !LINKSTO Kernel.TaskManagement.Scheduling.PrioQueue, 1
 * !LINKSTO Kernel.TaskManagement.Scheduling, 1
 * !LINKSTO Kernel.TaskManagement, 1
 *		When there are multiple tasks per priority, the priority queue is a FIFO ring buffer.
*/
void OS_EnqueueTask(const os_task_t *tp, os_prio_t prio)
{
	const os_priorityqueue_t *q = OS_GetPriorityQueueOfCore(OS_GetMyCoreId(), prio);
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_DEVSANITYCHECK(OS_PriorityQueueIsFull(prio, q)) )
	{
		/* PANIC!!! Priority queue is already full */
		/* can't propagate the return value of OS_PANIC -> ignore it */
		(void) OS_PANIC(OS_PANIC_PriorityQueueAlreadyFull);
	}

	/* Add the task to the back of the priority queue
	*/
	OS_Append_PriorityQueue(prio, q, OS_Get_PriorityQueueDynamic(q), tp->taskId);

	/* Set the queue's bit in the master word
	*/
	OS_Set_PriorityBits(q);

	if ( (kernel_data->taskQueueHead == OS_NULL) || (prio > kernel_data->taskQueueHead->dynamic->prio) )
	{
		kernel_data->taskQueueHead = tp;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
