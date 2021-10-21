/* kern-requeueup.c
 *
 * This file contains the OS_RequeueUp function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-requeueup.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: Not an issue. Generic macro 'OS_FindPrio(qAct)' contains a comparison,
 *   which becomes "always false" in this function if scheduling algorithm is CLZ_QUEUE
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain side effects.
 *
 * Reason:
 * False positive: OS_FindPrio() does not have side effects but only reads information.
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RequeueUp
 *
 * The specified task activation is moved from its position in the task queue to somewhere
 * further up the queue because it has increased its priority. The task's priority has already
 * been set to its new value.
 *
 * OS_RequeueUp() must be called with interrupts locked
*/
void OS_RequeueUp(os_tasklink_t tAct, os_prio_t prio)
{
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_tasklink_t qAct = taskActivations[0];
	os_tasklink_t qNew = 0;			/* New predecessor in queue */
	os_tasklink_t qOld;				/* Old predecessor in queue */
	os_tasklink_t qNewNext;			/* New successor in queue */

	/* Run through the queue till we find the new position. Note
	 * that this loop will automatically stop at the current position
	 * because the priority test will return "equal", so there's no
	 * need to explicitly stop at qAct==tAct.
	*/
	/* Possible diagnostic TOOLDIAG-1 <+2> */
	/* Deviation MISRAC2012-1 <1> */
	while ( (qAct != 0) && (OS_FindPrio(qAct) > prio) )
	{
		qNew = qAct;
		qAct = taskActivations[qAct];
	}

	/* If we leave the above loop with qAct == 0, the current task is not in the queue.
	 * We can't handle that here so we just leave the queue as it is.
	*/
	if ( qAct != 0 )
	{
		/* Remember the current queue position. This will be the new successor in the queue.
		*/
		qNewNext = qAct;

		/* Now we find the current ("old") position in the queue by searching from the current position.
		*/
		qOld = qNew;
		while ( (qAct != tAct) && (qAct != 0) )
		{
			qOld = qAct;
			qAct = taskActivations[qAct];
		}

		if ( qOld == qNew )
		{
			/* The old position and the new position are the same, so no change.
			*/
		}
		else
		{
			if ( qAct == tAct )
			{
				/* Now we remove the activation from its former position
				 * (after qOld) and insert it after qNew.
				 * The above test is just a sanity test. If qAct != tAct it means the task isn't
				 * in the queue.
				*/
				/* Remove from old place in queue
				*/
				taskActivations[qOld] = taskActivations[tAct];

				/* Insert at new place in queue
				*/
				taskActivations[qNew] = tAct;
				taskActivations[tAct] = qNewNext;

				/* Finally, set the new queue head because it might have changed.
				*/
				OS_GetKernelData()->taskQueueHead = OS_GetTaskPtrs(taskActivations[0]);
			}
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
