/* kern-resetschedulealarm.c
 *
 * This file contains the OS_ResetScheduleAlarm function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-resetschedulealarm.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ResetScheduleAlarm
 *
 * This function resets the schedule table's alarm so that it will expire
 * at the correct time for the next expiry point or whatever.
 *
 * If the given delta is already in the past, OS_TRUE is returned and the alarm
 * is not set. However, sd->ctrAbs is updated correctly.
 *
 * This is an internal kernel function, so there is no error checking.
 * Interrupts must be already disabled when calling this function.
*/
os_boolean_t OS_ResetScheduleAlarm
(	const os_schedule_t *ss,
	os_scheduledynamic_t *sd,
	os_tick_t delta
)
{
	const os_alarm_t *as;
	os_alarmdynamic_t *ad;
	const os_counter_t *cs;
	os_counterdynamic_t *cd;
	os_tick_t diff;
	os_boolean_t repeat = OS_TRUE;

	as = &OS_alarmTableBase[ss->alarm];
	ad = as->dynamic;
	cs = &OS_counterTableBase[as->counter];
	cd = cs->dynamic;

	diff = OS_CounterSub(cd->current, sd->ctrAbs, cs->maxallowedvalue);

	sd->ctrAbs = OS_CounterAdd(sd->ctrAbs, delta, cs->maxallowedvalue);

	if ( delta > diff )
	{
		ad->delta = OS_CounterSub(sd->ctrAbs, cd->current, cs->maxallowedvalue);
		ad->period = 0;
		ad->inUse = OS_ALARM_INUSE;

		OS_EnqueueAlarm(cs, cd, ss->alarm, ad);

		repeat = OS_FALSE;
	}

	return repeat;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
