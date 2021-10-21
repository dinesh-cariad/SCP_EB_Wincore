/* kern-inserttask.c
 *
 * This file contains the OS_InsertTask() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-inserttask.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InsertTask
 *
 * The specified task is added to the beginning of the priority FIFO of the given priority.
 *
 * This function is only used when there can be multiple tasks at a priority. When there's
 * only a single slot at each priority the function OS_EnqueueTask() is identical.
 * It's part of the CLZ queue algorithm.
*/
void OS_InsertTask(const os_task_t *tp, os_prio_t prio)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_priorityqueue_t *q = OS_GetPriorityQueueOfCore(OS_GetMyCoreId(), prio);

	if ( OS_DEVSANITYCHECK(OS_PriorityQueueIsFull(prio, q)) )
	{
		/* PANIC!!! Priority queue is already full */
		/* can't propagate the return value of OS_PANIC -> ignore it */
		(void) OS_PANIC(OS_PANIC_PriorityQueueAlreadyFull);
	}

	/* Add the task to the front of the priority queue.
	*/
	OS_Prepend_PriorityQueue(prio, q, OS_Get_PriorityQueueDynamic(q), tp->taskId);

	/* Set the queue's bit in the master word.
	*/
	OS_Set_PriorityBits(q);

	/* Set the task's new priority.
	*/
	if ( tp->dynamic->prio < prio )
	{
		tp->dynamic->prio = prio;
	}

	if ( (kernel_data->taskQueueHead == OS_NULL) || (prio >= kernel_data->taskQueueHead->dynamic->prio) )
	{
		kernel_data->taskQueueHead = tp;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
