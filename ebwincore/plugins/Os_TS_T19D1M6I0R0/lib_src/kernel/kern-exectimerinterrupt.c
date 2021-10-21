/* kern-exectimerinterrupt.c
 *
 * This file contains the OS_ExecTimerInterrupt function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-exectimerinterrupt.c 27842 2017-11-10 12:18:30Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is never used.
 *
 * Reason: Not an issue, variable will be used if timing protection is enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: ControllingExpressionIsConstant
 *   Controlling expression is constant.
 *
 * Reason: Not an issue, controlling expression is not a constant if
 * timing protection is enabled.
 */
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * If enclosed in parentheses, it would result in syntax errors.
 * Hence, this isn't viable.
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ExecTimerInterrupt
 *
 * This function handles the interrupt that occurs when the execution-time
 * timeout interrupt occurs. It must calculate the time used and update
 * the appropriate etMax and perform execution-time aggregation.
 * Then it calls OS_ExceedExecTime() to kill the offending task or ISR.
 *
 * This function is used when the timestamp is not attached to the execution timer.
 * See also OS_ExecTimerTimestampInterrupt() in kern-exectimertimestampinterrupt.c
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime, 2
*/
void OS_ExecTimerInterrupt(void)
{
	os_tick_t used, remaining;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	/* Clear the timer interrupt
	*/
	OS_ResetExecTimingInterrupt();

	/* Get time used since last call. This updates kernel_data->accounting.frc.
	*/
	used = OS_GetTimeUsed();

	/* Add 'used' onto the current accounting sum, with saturation!
	*/
	if ( used > (OS_MAXTICK - kernel_data->accounting.etUsed) )
	{
		kernel_data->accounting.etUsed = OS_MAXTICK;
	}
	else
	{
		kernel_data->accounting.etUsed += used;
	}

	if ( kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit )
	{
		if ( kernel_data->inFunction == OS_INTASK )
		{
			kernel_data->accounting.taskTimingNesting = 1;

			/* Possible diagnostic TOOLDIAG-2 <1> */
			if ( OS_GET_ACCT(((kernel_data->taskCurrent->flags & OS_TF_MEASUREEXEC) != 0)
					&& (kernel_data->taskCurrent->accounting->etMax < kernel_data->accounting.etUsed)) )
			{
				/* Deviation MISRAC2012-1 <1> */
				OS_SET_ACCT(kernel_data->taskCurrent->accounting->etMax = kernel_data->accounting.etUsed);
			}
		}
		else
		if ( kernel_data->inFunction == OS_INCAT2 )
		{	/* Possible diagnostic TOOLDIAG-1 <1> */
			const os_isr_t *isr = &OS_isrTableBase[kernel_data->isrCurrent];

			/* Possible diagnostic TOOLDIAG-2 <1> */
			if ( OS_GET_ACCT(((isr->flags & OS_ISRF_MEASUREEXEC) != 0)
					&& (isr->accounting->etMax < kernel_data->accounting.etUsed)) )
			{
				/* Deviation MISRAC2012-1 <1> */
				OS_SET_ACCT(isr->accounting->etMax = kernel_data->accounting.etUsed);
			}
		}
		else
		{
			/* MISRA */
		}

		OS_ExceedExecTime();
	}
	else
	{
		remaining = kernel_data->accounting.etLimit - kernel_data->accounting.etUsed;

		OS_SetExecTimingInterrupt(kernel_data->accounting.frc, remaining);
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
