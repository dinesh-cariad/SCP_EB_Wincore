/* kern-dequeue.c
 *
 * This file contains the OS_Dequeue function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dequeue.c 24285 2016-06-03 11:24:07Z mist8519 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_Dequeue
 *
 * The specified task activation is removed from the task queue. The task
 * can be anywhere in the queue.
 *
 * OS_Dequeue() must be called with interrupts locked
*/
void OS_Dequeue(os_tasklink_t tAct)
{
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_tasklink_t qAct = taskActivations[0];

	if ( tAct == qAct )
	{
		/* Task is at head of queue
		*/
		taskActivations[0] = taskActivations[tAct];
		OS_GetKernelData()->taskQueueHead = OS_GetTaskPtrs(taskActivations[0]);
		taskActivations[tAct] = 0;
	}
	else
	{
		/* Task is not at head. Might be somewhere further along.
		 * This can happen if a higher priority task gets activated by an
		 * interrupt while we're in the kernel TerminateTask routine with
		 * interrupts enabled. It also covers the case for killing a task
		 * that is READY. (Autosar, protection violation etc.)
		*/
		while ( (taskActivations[qAct] != tAct) && (qAct != 0) )
		{
			qAct = taskActivations[qAct];
		}

		if ( qAct == 0 )
		{
			/* Task is not in the queue - nothing to do */
		}
		else
		{
			taskActivations[qAct] = taskActivations[tAct];
			taskActivations[tAct] = 0;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
