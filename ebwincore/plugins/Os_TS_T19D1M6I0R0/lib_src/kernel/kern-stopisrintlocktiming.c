/* kern-stopisrintlocktiming.c
 *
 * This file contains the OS_StopIsrIntLockTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-stopisrintlocktiming.c 27842 2017-11-10 12:18:30Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if timing protection is enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off, OS_GET_ILOCK always evaluates to zero.
 *
 * TOOLDIAG-3) Possible diagnostic: ControllingExpressionIsConstant
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
 * OS_StopIsrIntLockTiming
 *
 * This function stops interrupt-lock timing for the isr. If execution
 * time monitoring was previously running the time remaining is computed
 * and monitoring is restarted.
*/
void OS_StopIsrIntLockTiming(os_isrid_t i, os_intlocktype_t locktype)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_isr_t * const isr = &OS_isrTableBase[i];
	os_tick_t locktime;
	os_tick_t used;

	OS_PARAM_UNUSED(i);

	locktime = OS_GET_ILOCK((locktype == OS_LOCKTYPE_OS) ? isr->osLockTime : isr->allLockTime);

	/* Possible diagnostic TOOLDIAG-2 <1> */
	if ( locktime != 0 )
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		if ( locktype == OS_LOCKTYPE_OS )
		{
			kernel_data->accounting.etUsed += used + kernel_data->accounting.osSaveUsed;
			kernel_data->accounting.etLimit = kernel_data->accounting.osSaveLimit;
			kernel_data->accounting.etType = kernel_data->accounting.osSaveType;
		}
		else
		{
			kernel_data->accounting.etUsed += used + kernel_data->accounting.allSaveUsed;
			kernel_data->accounting.etLimit = kernel_data->accounting.allSaveLimit;
			kernel_data->accounting.etType = kernel_data->accounting.allSaveType;
		}

		/* Possible diagnostic TOOLDIAG-3 <1> */
		if ( OS_GET_ACCT(((isr->flags & OS_ISRF_MEASUREEXEC) != 0)
				&& (isr->accounting->etMax < kernel_data->accounting.etUsed)) )
		{
			/* Deviation MISRAC2012-1 <1> */
			OS_SET_ACCT(isr->accounting->etMax = kernel_data->accounting.etUsed);
		}

		if ( kernel_data->accounting.etLimit != 0 )
		{
			if ( kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit )
			{
				OS_ExceedExecTime();
			}
			else
			{
				OS_SetExecTimingInterrupt(
						kernel_data->accounting.frc,
						(kernel_data->accounting.etLimit - kernel_data->accounting.etUsed));
			}
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
