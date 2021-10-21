/* kern-preempttaskexectiming.c
 *
 * This file contains the OS_PreemptTaskExecTiming() function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-preempttaskexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off, OS_GET_ACCT always evaluates to OS_NULL.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_PreemptTaskExecTiming
 *
 * This function gets the amount of execution time that has passed since
 * the last measurement point, substracts it from the time remaining,
 * and stores the result in the given task's remaining budget.
 *
 * This function is called by the dispatcher when a task is pre-empted, after the state
 * has been set to READY_SYNC or READY_ASYNC. It is also called by the Category 2 wrapper
 * when a task has been interrupted. It is therefore safe to call OS_Error (and
 * possibly kill the task) here.
 *
*/
void OS_PreemptTaskExecTiming(const os_task_t *tp)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskaccounting_t *acc = OS_GET_ACCT(tp->accounting);
	os_tick_t used;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( acc != OS_NULL )
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		kernel_data->accounting.etUsed += used;
		acc->etUsed = kernel_data->accounting.etUsed;
		acc->etType = kernel_data->accounting.etType;

		if ( ((tp->flags & OS_TF_MEASUREEXEC) != 0) && (kernel_data->accounting.etUsed > acc->etMax) )
		{
			acc->etMax = kernel_data->accounting.etUsed;
		}

		if ( (kernel_data->accounting.etLimit != 0)
				&& (kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit) )
		{
			OS_ExceedExecTime();
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
