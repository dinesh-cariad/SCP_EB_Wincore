/* kern-starttaskexectiming.c
 *
 * This file contains the OS_StartTaskExecTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-starttaskexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off, OS_GET_ACCT always evaluates to OS_NULL.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off, OS_GET_TP always evaluates to zero.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_StartTaskExecTiming
 *
 * This function gets the amount of execution time that has been used
 * by a task and stores it into the global accounting structure. It then
 * sets the limit in the global accounting structure and sets the timer
 * interrupt for the difference.
 * The function is called when a task gains or regains the CPU.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
*/
void OS_StartTaskExecTiming(const os_task_t *tp)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskaccounting_t *acc = OS_GET_ACCT(tp->accounting);
	os_tick_t remaining;

	OS_PARAM_UNUSED(tp);

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( acc != OS_NULL )
	{
		kernel_data->accounting.frc = OS_ReadExecTimer();
		kernel_data->accounting.inFunction = OS_INTASK;
		kernel_data->accounting.etType = acc->etType;
		kernel_data->accounting.etUsed = acc->etUsed;
		kernel_data->accounting.etLimit = OS_GET_TP(tp->execBudget);

		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (kernel_data->accounting.etLimit != 0)
		{
			if (kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit)
			{
				/* This should never happen.
				*/
				remaining = 1;
			}
			else
			{
				remaining = kernel_data->accounting.etLimit - kernel_data->accounting.etUsed;
			}
			OS_SetExecTimingInterrupt(kernel_data->accounting.frc, remaining);
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
