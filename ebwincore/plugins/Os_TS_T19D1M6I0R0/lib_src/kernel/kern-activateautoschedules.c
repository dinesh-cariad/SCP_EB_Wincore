/* kern-activateautoschedules.c
 *
 * This file contains the OS_ActivateAutoSchedules function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-activateautoschedules.c 27842 2017-11-10 12:18:30Z mist9353 $
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

/* OS_ActivateAutoSchedules()
 *
 * Activate all schedule tables that are specified for the requested mode
 *
 * !LINKSTO Kernel.Autosar.Autostart, 1
 * !LINKSTO Kernel.Autosar.ScheduleTable.Autostart, 1
*/
void OS_ActivateAutoSchedules(void)
{
	os_uint16_t idx;
	const os_autoschedule_t *as;
	os_scheduleid_t s;
	const os_schedule_t *ss;
	os_coreid_t const myCoreId = OS_GetMyCoreId();

	idx = OS_startModeSchedules[OS_GetAppMode()];
	as = &OS_autoStartSchedules[idx];
	s = as->schedule;

	while ( s < OS_nSchedules )
	{
		ss = &OS_scheduleTableBase[s];

		/* Possible diagnostic TOOLDIAG-1 <+2> */
		/* Deviation MISRAC2012-1 */
		if(OS_GetCoreIdFromScheduleTableId(s) == myCoreId)
		{

			if ( as->method == OS_STARTMETHOD_SYNC )
			{
				/* !LINKSTO Kernel.Autosar.ScheduleTable.Autostart.Synchronously, 1
				*/
				/* when called during autostart, DoStartScheduleTableSynchron can't
				 * fail, so we can safely ignore its return value
				 */
				(void) OS_LocalStartScheduleTableSynchron(ss);
			}
			else
			{
				/* An incorrectly generated method will start "relative".
				 *
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Autostart.Absolute, 1
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Autostart.Relative, 1
				 */
				/* when called during autostart, DoStartScheduleTable can't fail, so
				 * we can safely ignore its return value
				 */
				(void) OS_LocalStartScheduleTable(ss, OS_alarmTableBase[ss->alarm].counter,
						as->offset, (os_boolean_t)(as->method != OS_STARTMETHOD_ABS));
			}
		}
		idx++;
		as = &OS_autoStartSchedules[idx];
		s = as->schedule;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
