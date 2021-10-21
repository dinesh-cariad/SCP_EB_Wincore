/* kern-startreslocktiming.c
 *
 * This file contains the OS_StartResLockTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startreslocktiming.c 27842 2017-11-10 12:18:30Z mist9353 $
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
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * If the parameter was enclosed in parentheses, this would result in a
 * C language syntax error. Hence, this isn't viable.
*/
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_StartResLockTiming
 *
 * This function starts resource-lock timing for the resource.
 *
 * This function is not called if the resource lock is not being timed.
 * Interrupts are disabled.
*/
void OS_StartResLockTiming
(	os_resourcedynamic_t *rd,
	os_tick_t tLim
)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_tick_t used;
	os_tick_t remaining;

	OS_PARAM_UNUSED(rd);

	OS_ResetExecTimingInterrupt();
	used = OS_GetTimeUsed();

	kernel_data->accounting.etUsed += used;

	if ( kernel_data->accounting.inFunction == OS_INTASK )
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_task_t * const taskCurrent = kernel_data->taskCurrent;
		/* Possible diagnostic TOOLDIAG-2 <1> */
		if ( OS_GET_ACCT(((taskCurrent->flags & OS_TF_MEASUREEXEC) != 0)
				&& (taskCurrent->accounting->etMax < kernel_data->accounting.etUsed)) )
		{
			/* Deviation MISRAC2012-1 <1> */
			OS_SET_ACCT(taskCurrent->accounting->etMax = kernel_data->accounting.etUsed);
		}
	}
	else
	if ( kernel_data->accounting.inFunction == OS_INCAT2 )
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
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
		/* MISRA-C */
	}

	if ((kernel_data->accounting.etLimit != 0) && (kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit))
	{
		OS_ExceedExecTime();
	}
	else
	{
		if ( kernel_data->accounting.etLimit == 0 )
		{
			remaining = OS_MAXTICK;
		}
		else
		{
			remaining = kernel_data->accounting.etLimit - kernel_data->accounting.etUsed;
		}

		/* Deviation MISRAC2012-1 <3> */
		OS_SET_RLOCK(rd->etUsed = kernel_data->accounting.etUsed);
		OS_SET_RLOCK(rd->etLimit = kernel_data->accounting.etLimit);
		OS_SET_RLOCK(rd->etType = kernel_data->accounting.etType);

		kernel_data->accounting.inFunction = kernel_data->inFunction;
		kernel_data->accounting.etUsed = 0;

		if (tLim >= remaining)
		{	/* old remaining time is shorter than new time */
			/* use old time for accounting (and leave accounting type unchanged) */
			kernel_data->accounting.etLimit = remaining;
		}
		else
		{	/* new limit is smaller then remaining time */
			/* use new time for accounting and set new accounting type */
			kernel_data->accounting.etLimit = tLim;
			kernel_data->accounting.etType = OS_ACC_RESLOCK;
		}

		OS_SetExecTimingInterrupt(kernel_data->accounting.frc, kernel_data->accounting.etLimit);
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
