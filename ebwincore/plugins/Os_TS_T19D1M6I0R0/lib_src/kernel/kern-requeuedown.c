/* kern-requeuedown.c
 *
 * This file contains the OS_RequeueDown function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-requeuedown.c 27842 2017-11-10 12:18:30Z mist9353 $
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
 * OS_RequeueDown
 *
 * The specified task activation is moved from its position in the task queue to somewhere
 * further down the queue because it has decreased its priority.
 *
 * OS_RequeueDown() must be called with interrupts locked
*/
void OS_RequeueDown(os_tasklink_t tAct, os_prio_t prio)
{
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_tasklink_t qAct = taskActivations[0];
	os_tasklink_t qOld = 0;		/* Old predecessor in queue */

	while ( (qAct != tAct) && (qAct != 0) )
	{
		qOld = qAct;
		qAct = taskActivations[qAct];
	}

	if ( qAct == tAct )
	{
		os_tasklink_t qSkip = taskActivations[tAct];	/* Old successor in queue */
		os_tasklink_t qNew = qOld;							/* New predecessor in queue */
		qAct = qSkip;

		/* Possible diagnostic TOOLDIAG-1 <+2> */
		/* Deviation MISRAC2012-1 <1> */
		while ( (qAct != 0) && (OS_FindPrio(qAct) > prio) )
		{
			qNew = qAct;
			qAct = taskActivations[qAct];
		}

		/* Now we remove the activation from its former position
		 * (after qOld) and insert it after qNew.
		*/
		if ( qOld == qNew )
		{
			/* No change */
		}
		else
		{
			/* Remove from old place in queue
			*/
			taskActivations[qOld] = qSkip;
			OS_GetKernelData()->taskQueueHead = OS_GetTaskPtrs(taskActivations[0]);

			/* Insert at new place in queue
			*/
			taskActivations[qNew] = tAct;
			taskActivations[tAct] = qAct;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
