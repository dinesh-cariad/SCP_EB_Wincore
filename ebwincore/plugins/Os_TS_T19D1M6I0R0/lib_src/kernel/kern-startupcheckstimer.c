/* kern-startupcheckstimer.c
 *
 * This file contains the OS_StartupChecksTimer function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupcheckstimer.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_StartupChecksTimer
 *
 * This function performs a variety of checks of the hardware timer systems
*/
os_result_t OS_StartupChecksTimer(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_result_t result = OS_E_OK;
	os_unsigned_t i;

	for ( i = 0; i < OS_nHwTimers; i++ )
	{
		os_hwt_t const* const hwt = &OS_hwTimerTableBase[i];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetTimerCoreId(hwt) == myCoreId)
		{
#if OS_HWT_POWEROF2
			/* If the architecture only supports power-of-2 timer wrap values we must check that
			 * the wrap masks are correct.
			*/
			if ( !OS_IsPowerOf2(hwt->wrapMask+1) )
			{
				result = OS_PANIC(OS_PANIC_SCHK_NonPowerOfTwoTimerWraparoundNotSupported);
			}
#endif

			if ( (hwt->wrapMask <= hwt->maxDelta) || (hwt->maxDelta <= hwt->defDelta) )
			{
				result = OS_PANIC(OS_PANIC_SCHK_TimerMaxDeltaAndDefDeltaInconsistent);
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
