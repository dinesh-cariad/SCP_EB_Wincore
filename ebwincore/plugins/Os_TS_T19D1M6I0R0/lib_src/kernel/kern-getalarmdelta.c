/* kern-getalarmdelta.c
 *
 * This file contains the OS_GetAlarmDelta function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getalarmdelta.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always false/true.
 *
 * Reason: This warning depends on the OS configuration and library optimization.
 *   If there are no hardware counters (OS_EXCLUDE_HWCOUNTER), this condition is always false.
 *   If there are no software counters (OS_EXCLUDE_SWCOUNTER), this condition is always true.
 *   Otherwise this condition must be evaluated at runtime.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason: See TOOLDIAG-1.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetAlarmDelta
 *
 * The total number of ticks remaining before the alarm expires
 * is written to the indicated location.
 *
 * Must be called with interrupts disabled.
 *
 * Returns 0  normally.
 *         1  if alarm is not in use.
 *         -1 if alarm could not be found (corrupt alarm list).
*/
os_int_t OS_GetAlarmDelta
(	os_alarmid_t a,			/* Alarm id */
	const os_alarm_t *as,	/* Corresponding static structure */
	os_alarmdynamic_t *ad,	/* Corresponding dynamic structure */
	os_tick_t *out			/* Where to put the result */
)
{
	const os_counter_t *cs;
	os_counterdynamic_t *cd;
	os_int_t result = 0;
	os_alarmid_t ap;
	os_tick_t total;

	/* If the alarm is attached to a hardware counter we need
	 * to update the current value and the head delta. This might
	* result in some alarms expiring from the queue.
	*/
	cs = &OS_counterTableBase[as->counter];
	cd = cs->dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if (OS_CounterIsHw(cs))
	{
		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (cd->lock == 0u)
		{
			OS_CtrUpdate(cs);
		}
	}

	if ( ad->inUse == OS_ALARM_INUSE )
	{
		/* Start with total = my delta, and find head of counter
		 * queue
		*/
		total = ad->delta;
		ap = cd->head;

		/* Add delta of all prior alarms in the counter queue
		*/
		while ( (ap < OS_totalAlarms) && (ap != a) )
		{
			ad = OS_alarmTableBase[ap].dynamic;
			total += ad->delta;
			ap = ad->next;
		}

		if ( ap == a )
		{
			/* !LINKSTO Kernel.API.Alarms.GetAlarm.Return, 1
			*/
			*out = total;
		}
		else
		{
			/* The alarm was not found in the delta list. This means
			 * a seriously corrupted list.
			*/
			result = -1;
		}
	}
	else
	{
		/* The alarm is not in use
		*/
		result = 1;
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
