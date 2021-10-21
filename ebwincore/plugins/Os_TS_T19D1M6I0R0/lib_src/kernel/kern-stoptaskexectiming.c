/* kern-stoptaskexectiming.c
 *
 * This file contains the OS_StopTaskExecTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-stoptaskexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
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
 * OS_StopTaskExecTiming
 *
 * This function stops the hardware timer used for execution timing.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
*/
void OS_StopTaskExecTiming(const os_task_t *tp)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskaccounting_t *acc = OS_GET_ACCT(tp->accounting);
	os_tick_t used;

	OS_PARAM_UNUSED(tp);

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( acc != OS_NULL )
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		/* We don't check the limit here. The task managed to get as far as the disable interrupts
		 * section in terminate task wihtout being killed, so we give it the benefit of the doubt.
		*/
		kernel_data->accounting.etUsed += used;

		if (kernel_data->accounting.etUsed > acc->etMax)
		{
			acc->etMax = kernel_data->accounting.etUsed;
		}

		kernel_data->accounting.inFunction = OS_INNOTHING;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
