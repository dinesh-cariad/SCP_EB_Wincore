/* kern-startupchecksisr.c
 *
 * This file contains the OS_StartupChecksIsr function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecksisr.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false.
 *
 * Reason: If timing protection is switched off,
 *  OS_GET_TP, OS_GET_ILOCK, OS_GET_RLOCK and OS_GET_ACCT always evaluate to zero or NULL.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if there is at least one ISR using accounting.
 *   See also TOOLDIAG-1.
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

os_result_t OS_StartupChecksIsr(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_result_t result = OS_E_OK;
	os_unsigned_t i;

	for ( i = 0; i < OS_nInterrupts; i++ )
	{
		os_isr_t const* const isr = &OS_isrTableBase[i];
		os_appcontext_t const* const app = OS_GET_APP(isr->app);

		if ( OS_AppIsNull(app) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_NoOwningApplication);
		}

		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (OS_GetIsrCoreId(isr) == myCoreId)
		{
			if ( ((isr->flags & OS_ISRF_MEASUREEXEC) != 0) && (OS_GET_ACCT(isr->accounting) == OS_NULL) )
			{
				result = OS_PANIC(OS_PANIC_SCHK_IsrWithMeasureExecButNoAccountingStructure);
			}

			/* Possible diagnostic TOOLDIAG-1 <4> */
			if ( (OS_GET_TP(isr->execBudget) != 0) ||
				 (OS_GET_ILOCK(isr->osLockTime) != 0) ||
				 (OS_GET_ILOCK(isr->allLockTime) != 0) ||
				 (OS_GET_RLOCK(isr->resourceLockTime) != OS_NULL) )
			{
				/* Possible diagnostic TOOLDIAG-1 <1> */
				if ( OS_GET_ACCT(isr->accounting) == OS_NULL )
				{
					/* Possible diagnostic TOOLDIAG-3 <+1> */
					result = OS_PANIC(OS_PANIC_SCHK_IsrWithExecTimeLimitButNoAccountingStructure);
				}

				/* This check is here because there's an implicit assumption in SuspendOSInterrupts that
				 * if any kind of interrupts have been locked there's a time limit already running that is less
				 * than or equal to the OS lock limit, so no new timing is started.
				 *
				 * The check won't fail if both are zero - that's OK.
				 * It *will* fail if osLockTime != 0 and allLockTime == 0 - that's OK.
				*/
				/* Possible diagnostic TOOLDIAG-1 <1> */
				if ( OS_GET_ILOCK(isr->osLockTime) < OS_GET_ILOCK(isr->allLockTime) )
				{
					result = OS_PANIC(OS_PANIC_SCHK_IsrWithOsIntLockTimeLessThanAllIntLockTime);
				}
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
