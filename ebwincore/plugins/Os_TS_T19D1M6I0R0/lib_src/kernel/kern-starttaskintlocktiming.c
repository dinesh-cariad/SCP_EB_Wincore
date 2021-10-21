/* kern-starttaskintlocktiming.c
 *
 * This file contains the OS_StartTaskIntLockTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-starttaskintlocktiming.c 27842 2017-11-10 12:18:30Z mist9353 $
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
 * OS_StartTaskIntLockTiming
 *
 * This function starts interrupt-lock timing for the task.
*/
void OS_StartTaskIntLockTiming(const os_task_t *tp, os_intlocktype_t locktype)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskaccounting_t *acc = OS_GET_ACCT(tp->accounting);
	os_tick_t locktime;
	os_tick_t used;
	os_tick_t remaining;

	OS_PARAM_UNUSED(tp);

	locktime = OS_GET_ILOCK((locktype == OS_LOCKTYPE_OS) ? tp->osLockTime : tp->allLockTime);

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if ( locktime != 0 )
	{
		OS_ResetExecTimingInterrupt();
		used = OS_GetTimeUsed();

		kernel_data->accounting.etUsed += used;

		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (OS_GET_ACCT(((tp->flags & OS_TF_MEASUREEXEC) != 0)
				&& (tp->accounting->etMax < kernel_data->accounting.etUsed)))
		{
			/* Deviation MISRAC2012-1 <1> */
			OS_SET_ACCT(tp->accounting->etMax = kernel_data->accounting.etUsed);
		}

		if ((kernel_data->accounting.etLimit != 0)
				&& (kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit))
		{
			OS_ExceedExecTime();
		}
		else
		{
			if (kernel_data->accounting.etLimit == 0)
			{
				remaining = OS_MAXTICK;
			}
			else
			{
				remaining = kernel_data->accounting.etLimit - kernel_data->accounting.etUsed;
			}

			if (locktype == OS_LOCKTYPE_OS)
			{
				acc->osSaveType = kernel_data->accounting.etType;
				acc->osSaveUsed = kernel_data->accounting.etUsed;
				acc->osSaveLimit = kernel_data->accounting.etLimit;
			}
			else
			{
				acc->allSaveType = kernel_data->accounting.etType;
				acc->allSaveUsed = kernel_data->accounting.etUsed;
				acc->allSaveLimit = kernel_data->accounting.etLimit;
			}

			/* Never allow a task to extend its execution budget by disabling
			 * interrupts!
			*/
			kernel_data->accounting.etUsed = 0;

			if (locktime >= remaining)
			{	/* old remaining time is shorter than new time */
				/* use old time for accounting (and leave accounting type unchanged) */
				kernel_data->accounting.etLimit = remaining;
			}
			else
			{	/* new time is shorter than old remaining time */
				/* use new time for accounting and set new accounting type */
				kernel_data->accounting.etLimit = locktime;
				kernel_data->accounting.inFunction = OS_INTASK;
				kernel_data->accounting.etType = OS_ACC_INTLOCK;
			}

			OS_SetExecTimingInterrupt(kernel_data->accounting.frc, kernel_data->accounting.etLimit);
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
