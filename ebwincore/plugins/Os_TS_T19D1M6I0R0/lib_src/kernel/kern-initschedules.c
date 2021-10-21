/* kern-initschedules.c
 *
 * This file contains the OS_InitSchedules() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initschedules.c 27282 2017-08-24 13:57:43Z olme8414 $
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
 * OS_InitSchedules
 *
 * The 'chain' of each alarm needs to be set to OS_NULLSCHEDULE.
 * This is because the default bss value (0) is a valid schedule id.
*/
void OS_InitSchedules(void)
{
	os_scheduleid_t i;
	os_coreid_t const myCoreId = OS_GetMyCoreId();

	for (i = 0; i < OS_nSchedules; ++i)
	{
		os_schedule_t const * const ss = &OS_scheduleTableBase[i];
		/* Deviation MISRAC2012-1 */
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetCoreIdFromScheduleTableId(i) == myCoreId)
		{
			ss->dynamic->chain = OS_NULLSCHEDULE;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
