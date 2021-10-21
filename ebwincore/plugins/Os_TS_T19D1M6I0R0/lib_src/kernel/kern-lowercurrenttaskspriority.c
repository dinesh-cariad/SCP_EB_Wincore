/* kern-lowercurrenttaskspriority.c
 *
 * This file contains the OS_LowerCurrentTasksPriority function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-lowercurrenttaskspriority.c 25124 2016-10-25 08:57:01Z vaka852 $
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>


#include <memmap/Os_mm_code_begin.h>

/*
 * This function lowers the priority of the current task
 * by moving it to the correct position in the priority queue.
 * and setting the tasks priority field accordingly.
 *
 * This is a helper function for OS_GetResourceFromTask and OS_KernResumeInterrupts.
 *
 * Parameter:
 *  reqPrio: The priority to which the current task's priority is meant to be lowered.
 *
 * Preconditions:
 *  Interrupts must be disabled so there's no concurrent modification
 *  of the current task or the priority queue.
*/
void OS_LowerCurrentTasksPriority(os_prio_t reqPrio)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_task_t * const currentTask = kernel_data->taskCurrent;
	os_taskdynamic_t * const td = currentTask->dynamic;

	/* The following part depends on the queue implementation.
	*/
	{
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
		/* We only dequeue if the priority drops below the highest task priority.
		*/
		os_prio_t const currprio = td->prio;
		os_coreid_t const myCoreId = OS_GetMyCoreId();
		os_taskprio_t const maxPrio = OS_maxPrio[myCoreId];

		if (reqPrio < maxPrio)
		{
			OS_DequeueTask(currentTask, ((currprio > maxPrio) ? (os_prio_t) maxPrio : currprio));
		}
#else
		/* Priority changes ...
		 * We need to dequeue the task and then re-queue it again
		 * because there might be a higher priority task in the queue.
		*/
		os_tasklink_t link;

		link = OS_CurrentLink(currentTask, td);
		OS_RequeueDown(link, reqPrio);
#endif
	}

	td->prio = reqPrio;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
