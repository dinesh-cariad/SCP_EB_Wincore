/* kern-stoptaskintlocktiming.c
 *
 * This file contains the OS_StopTaskIntLockTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-stoptaskintlocktiming.c 27842 2017-11-10 12:18:30Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off, OS_GET_ILOCK always evaluates to zero.
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
 * OS_StopTaskIntLockTiming
 *
 * This function stops interrupt-lock timing for the task. If execution
 * time monitoring was previously running the time remaining is computed
 * and monitoring is restarted.
*/
void OS_StopTaskIntLockTiming(const os_task_t *tp, os_intlocktype_t locktype)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskaccounting_t *acc = OS_GET_ACCT(tp->accounting);
	os_tick_t locktime;
	os_tick_t used;

	OS_PARAM_UNUSED(tp);

	locktime = OS_GET_ILOCK((locktype == OS_LOCKTYPE_OS) ? tp->osLockTime : tp->allLockTime);

	/* No need to test if acc is OS_NULL - the generator ensures that it isn't
	*/
	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( locktime != 0 )
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		if ( locktype == OS_LOCKTYPE_OS )
		{
			kernel_data->accounting.etUsed += used + acc->osSaveUsed;
			kernel_data->accounting.etLimit = acc->osSaveLimit;
			kernel_data->accounting.etType = acc->osSaveType;
		}
		else
		{
			kernel_data->accounting.etUsed += used + acc->allSaveUsed;
			kernel_data->accounting.etLimit = acc->allSaveLimit;
			kernel_data->accounting.etType = acc->allSaveType;
		}

		/* Possible diagnostic TOOLDIAG-2 <1> */
		if ( OS_GET_ACCT(((tp->flags & OS_TF_MEASUREEXEC) != 0)
				&& (tp->accounting->etMax < kernel_data->accounting.etUsed)) )
		{
			/* Deviation MISRAC2012-1 <1> */
			OS_SET_ACCT(tp->accounting->etMax = kernel_data->accounting.etUsed);
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
