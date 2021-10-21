/* kern-enqueuealarm.c
 *
 * This file contains the OS_EnqueueAlarm and OS_EnqueueAlarmNoAdjust functions.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-enqueuealarm.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_EnqueueAlarm
 * OS_EnqueueAlarmNoAdjust
 *
 * The specified alarm is inserted into the alarm queue of the specified
 * counter, in order of expiry time.
 *
 * An alarm queue is a delta-list, therefore the delta value of each
 * item on the list is subtracted from this alarm's delta until
 * either the end of the list is reached or an alarm with a greater
 * delta is found.
 *
 * Interrupts must be disabled before calling these functions.
 *
 * It is permitted that the head of the alarm list can have a delta of zero. This
 * can happen when we are running all the alarms that have expired during this tick.
 *
 * The "normal" function is used under most circumstances. The "no adjust" version is used
 * when requeueing a periodic alarm because here we need to queue against the "last known"
 * counter value, not the "current" value.
 *
 * See also SetRelAlarm and SetAbsAlarm.
*/
void OS_EnqueueAlarm
(	const os_counter_t *cs,		/* Counter address (static stuff) */
	os_counterdynamic_t *cd,	/* Counter address (dynamic stuff) */
	os_alarmid_t a,				/* Id of alarm to insert */
	os_alarmdynamic_t *ad		/* Address of alarm (dynamic) to insert */
)
{
	/* Take any outstanding time (the "error" term) into account when running the counter
	 * The error term tels us how much the current counter value differs from the queue.
	*/
	ad->delta = OS_CounterAdd(ad->delta, cd->error, cs->maxallowedvalue);

	OS_EnqueueAlarmNoAdjust(cd, a, ad);
}


void OS_EnqueueAlarmNoAdjust
(	os_counterdynamic_t *cd,	/* Counter address (dynamic stuff) */
	os_alarmid_t a,				/* Id of alarm to insert */
	os_alarmdynamic_t *ad		/* Address of alarm (dynamic) to insert */
)
{
	os_alarmdynamic_t *lp = OS_NULL;
	os_alarmid_t n = cd->head;
	os_alarmdynamic_t *np = OS_AlarmIdToDynamic(n);


	/* Walk down the list until we find an alarm whose delta is greater
	 * than or equal to the remaining delta for our alarm. This means
	 * that the new alarm will be inserted BEFORE other alarms with the
	 * same expiry time, but that doesn't matter, since all such alarms
	 * expire simultaneously. Doing this might save an unnecessary trip
	 * around the loop.
	*/
	while ( (np != OS_NULL) && (np->delta < ad->delta) )
	{
		/* At each stage, subtract the alarm's delta from ours
		*/
		ad->delta -= np->delta;

		n = np->next;
		lp = np;
		np = OS_AlarmIdToDynamic(n);
	}

	if ( np != OS_NULL )
	{
		/* If there's an alarm after ours on the queue, we must subtract
		 * our delta from its delta to keep its expiry time correct.
		*/
		np->delta -= ad->delta;
	}

	if ( lp == OS_NULL )
	{
		/* Insert at the head of the queue
		*/
		ad->next = cd->head;
		cd->head = a;
	}
	else
	{
		/* Insert in the queue after lp
		*/
		ad->next = n;
		lp->next = a;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
