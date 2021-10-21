/* kern-stopisrexectiming.c
 *
 * This file contains the OS_StopIsrExecTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-stopisrexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
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
 * OS_StopIsrExecTiming
 *
 * This function is called from the category 2 interrupt exit routine.
 * It accepts the time-remaining value returned by the call to
 * OS_StartIsrExecTiming during interrupt entry.
 * The accounting inFunction has already been restored by the exit
 * routine.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.ISR, 1
*/
void OS_StopIsrExecTiming(const os_isr_t *isr)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_israccounting_t *acc = OS_GET_ACCT(isr->accounting);
	os_tick_t used;

	OS_ResetExecTimingInterrupt();

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( acc != OS_NULL )
	{
		used = OS_GetTimeUsed();

		/* We don't check the limit here. The task manage to get as far as the disable interrupts
		 * section in terminate task wihtout being killed, so we give it the benefit of the doubt.
		*/
		kernel_data->accounting.etUsed += used;

		if (((isr->flags & OS_ISRF_MEASUREEXEC) != 0) && (kernel_data->accounting.etUsed > acc->etMax))
		{
			acc->etMax = kernel_data->accounting.etUsed;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
