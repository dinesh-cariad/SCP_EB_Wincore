/* kern-inittimers.c
 *
 * This file contains the OS_InitTimers() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-inittimers.c 30337 2019-06-06 08:07:42Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Comparison with constant compile-time result
 *
 * Reason: In single-core configurations, the result is compile-time constant.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 14.3 (required)
 * Controlling expressions shall not be invariant.
 *
 * Reason:
 * The invariance depends on the configuration. In single-core systems,
 * for example, the expression is always true.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitTimers
 *
 * Each configured timer is initialised by calling its driver's "INIT" function.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.HARDWARE.Initialisation, 1
*/
void OS_InitTimers(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_uint8_t i;

	for (i = 0; i < OS_nHwTimers; ++i)
	{
		os_hwt_t const * const t = &OS_hwTimerTableBase[i];
		/* Deviation MISRAC2012-1 */
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetTimerCoreId(t) == myCoreId)
		{
			OS_HwtInit(t);
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
