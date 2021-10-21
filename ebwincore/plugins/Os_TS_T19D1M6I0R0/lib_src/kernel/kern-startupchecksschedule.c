/* kern-startupchecksschedule.c
 *
 * This file contains the OS_StartupChecksSchedule function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecksschedule.c 27842 2017-11-10 12:18:30Z mist9353 $
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

os_result_t OS_StartupChecksSchedule(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_result_t result = OS_E_OK;
	os_unsigned_t i;
	os_unsigned_t j;
	const os_schedule_t *schedule;
	const os_scheduleentry_t *evt;
	os_eventmask_t foundevents;
	os_tick_t counterMax;
	os_tick_t extraDelay;
	os_tick_t lastOffset;
	os_tick_t delay;
	os_boolean_t syncable;
	os_permission_t perms;
	os_appcontext_t const* app;

	for ( i = 0; i < OS_nSchedules; i++ )
	{
		schedule = &OS_scheduleTableBase[i];
		app = OS_GET_APP(schedule->app);

		if ( OS_AppIsNull(app) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_NoOwningApplication);
		}

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetScheduleTableCoreId(schedule) == myCoreId)
		{
			perms = OS_GET_APP(schedule->permissions);

			if ( !OS_HasPermission(app, perms) )
			{
				result = OS_PANIC(OS_PANIC_SCHK_OwningApplicationHasNoPermission);
			}

			counterMax = OS_counterTableBase[OS_alarmTableBase[schedule->alarm].counter].maxallowedvalue;
			syncable = (schedule->flags & OS_ST_SYNCABLE) != 0;

			foundevents = 0;
			lastOffset = 0;
			extraDelay = schedule->period - schedule->table[schedule->nEntries-1].offset;

			for ( j = 0; j < schedule->nEntries; j++ )
			{
				evt = &schedule->table[j];

				/* Remember events that have been set at this offset.
				 * Reset memory of events at each new offset
				*/
				if ( evt->offset == lastOffset )
				{
					foundevents |= evt->event;
				}
				else
				{
					foundevents = 0;
				}

				if ( (foundevents != 0) && (evt->event == 0) )
				{
					result = OS_PANIC(OS_PANIC_SCHK_ScheduleTableActivateTaskAfterSetEvent);
				}

				if ( evt->offset < lastOffset )
				{
					result = OS_PANIC(OS_PANIC_SCHK_ScheduleTableEventsNotInOrder);
				}

				/* These checks don't make sense if the offset is wrong.
				 * Furthermore, the sync-range checks are only done for the first event at a given offset.
				 * The sync parameters for the remainder of the events (at the same offset) are not used,
				 * so the OS doesn't care. Maybe the generator sets them to zero, maybe to the same as the
				 * first event at this offset.
				*/
				if ( (j == 0) || (evt->offset != lastOffset) )
				{
					/* This is the delay from the last expiry point to this one.
					*/
					delay = (evt->offset - lastOffset) + extraDelay;

					if ( delay > counterMax )
					{
						result = OS_PANIC(OS_PANIC_SCHK_DelayGreaterThanCounterMax);
					}

					if ( syncable && ((delay + evt->maxIncrease) > counterMax) )
					{
						result = OS_PANIC(OS_PANIC_SCHK_DelayPlusMaxIncreaseGreaterThanCounterMax);
					}

					if ( syncable && (evt->maxDecrease >= delay) )
					{
						result = OS_PANIC(OS_PANIC_SCHK_MaxDecreaseGreaterThanDelay);
					}

					if ( syncable && ((evt->maxDecrease + evt->maxIncrease) > schedule->period) )
					{
						result = OS_PANIC(OS_PANIC_SCHK_SynchronisationRangeGreaterThanScheduleTableLength);
					}
				}
				lastOffset = evt->offset;
				extraDelay = 0;
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
