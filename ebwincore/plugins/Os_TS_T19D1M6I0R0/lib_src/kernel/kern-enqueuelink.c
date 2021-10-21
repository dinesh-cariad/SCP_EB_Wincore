/* kern-enqueuelink.c
 *
 * This file contains the OS_EnqueueLink function.
 * The function is only necessary when multiple activations is enabled, but is
 * always present in the library.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-enqueuelink.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: Not an issue. Macro 'OS_FindPrio(qAct)' contains a comparison,
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
 * OS_EnqueueLink
 *
 * The specified task activation is inserted into the task activation queue
 * in priority order behind those with equal priority.
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.NonSuspendedTasksBCC, 1
 * !LINKSTO Kernel.Architecture.ConformanceClasses.NonSuspendedTasksECC, 1
 *		The number of tasks in the ready queue is not limited here.
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.TasksPerPrio, 1
 *		The number of tasks per priority is not limited here.
 *
 * !LINKSTO Kernel.TaskManagement.Scheduling.PrioQueue, 1
 * !LINKSTO Kernel.TaskManagement.Scheduling, 1
 * !LINKSTO Kernel.TaskManagement, 1
 *		'>=' in the while loop --> FIFO order.
*/
void OS_EnqueueLink(os_tasklink_t tAct, os_prio_t prio)
{
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_tasklink_t qAct = taskActivations[0];
	os_tasklink_t lAct = 0;

	/* Possible diagnostic TOOLDIAG-1 <+2> */
	/* Deviation MISRAC2012-1 */
	while ( (qAct != 0) && (OS_FindPrio(qAct) >= prio) )
	{
		lAct = qAct;
		qAct = taskActivations[qAct];
	}

	/* Now we insert tAct between lAct and qAct and recompute the queue head.
	*/
	taskActivations[lAct] = tAct;
	taskActivations[tAct] = qAct;
	OS_GetKernelData()->taskQueueHead = OS_GetTaskPtrs(taskActivations[0]);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
