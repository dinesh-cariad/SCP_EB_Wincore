/* kern-findhighesttask.c
 *
 * This file contains the functions OS_FindHighestTask(),...
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-findhighesttask.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if scalability class 2 is used.
 *
 * TOOLDIAG-2) Possible diagnostic: SetButNeverUsed
 *   Dead assignment.
 *
 * Reason: Not an issue, assignment is effective depending on OS scheduling configuration.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement could be reached if a large task queue is used.
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

/*!
 * OS_FindHighestTask
 *
 * This function finds the highest-priority active (ready or running) task.
 *
 * First the index of the most significant bit of the master word is found
*/
#include <memmap/Os_mm_code_begin.h>

os_priorityqueueentry_t OS_FindHighestTask(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_int_t index;
	os_priorityqueueentry_t highTask = OS_NULLTASK;
	/* Possible diagnostic TOOLDIAG-1 <2> */
	const os_priorityqueue_t *q;
	const os_priorityqueuedynamic_t *qd;

	if ( OS_GetTopLevelPrioWord() != 0 )
	{
		index = OS_FindSlaveIndex();

		/* Sanity check for master delivering a slave that is unoccupied.
		 * If there's only one level this cannot fail and the compiler should
		 * optimise the code away because the test (OS_slavePrioWord[0]==0)
		 * is already known to be false because we're in this branch
		 * (OS_GetTopLevelPrioWord() is OS_slavePrioWord[0])
		*/
		if ( OS_SLAVEWORDNOTOCCUPIED(index) )
		{
			/* PANIC!!! Slave slot is not occupied */
			/* can't propagate the return value of OS_PANIC -> ignore it */
			/* Possible diagnostic TOOLDIAG-3 <+1> */
			(void) OS_PANIC(OS_PANIC_SlaveSlotNotOccupied);
		}
		else
		{
			index = (OS_CLZWORD_NBITS * index) + OS_CLZ(*OS_GetSlavePrioWord(myCoreId, index));

			/* Reverse the order; the priority queue array is in ascending order! */
			index = OS_nPriorities[myCoreId] - (index + 1);

			/* Possible diagnostic TOOLDIAG-1 <3> */
			q = OS_GetPriorityQueueOfCore(myCoreId, index);
			/* Possible diagnostic TOOLDIAG-2 <1> */
			qd = OS_Get_PriorityQueueDynamic(q);
			highTask = OS_Get_PriorityQueueHead(index, q, qd);
		}
	}

	return highTask;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
