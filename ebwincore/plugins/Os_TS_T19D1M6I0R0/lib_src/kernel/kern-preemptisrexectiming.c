/* kern-preemptisrexectiming.c
 *
 * This file contains the OS_PreemptIsrExecTiming() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-preemptisrexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_PreemptIsrExecTiming
 *
 * This function is called from the category 2 interrupt entry routine.
 * It stops any current ISR execution timing and stores the state into
 * the specified accounting save structure.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
*/
void OS_PreemptIsrExecTiming(os_accountingsave_t *save)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_tick_t used;

	if ( kernel_data->accounting.inFunction == OS_INNOTHING )
	{
		save->inFunction = OS_INNOTHING;
		save->etType = OS_ACC_NONE;
		save->etUsed = 0;
		save->etLimit = 0;
	}
	else
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		/* We don't check for exceeded here - that's best done in the "resume"
		 * function. Doing it here would "kill" the ISR immediately, and the
		 * ISR whose wrapper we're in won't run.
		*/
		save->etUsed = kernel_data->accounting.etUsed + used;
		save->etLimit = kernel_data->accounting.etLimit;
		save->inFunction = kernel_data->accounting.inFunction;
		save->etType = kernel_data->accounting.etType;

		kernel_data->accounting.inFunction = OS_INNOTHING;
		kernel_data->accounting.etType = OS_ACC_NONE;
		kernel_data->accounting.etUsed = 0;
		kernel_data->accounting.etLimit = 0;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
