/* kern-killschedule.c
 *
 * This file contains the OS_KillSchedule function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-killschedule.c 26233 2017-03-20 13:34:00Z masa8317 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Dead assignment eliminated.
 *
 * Reason: Not an issue. This variable isn't used in the Safety OS. The Safety OS
 *   serializes all QM-OS calls. Therefore we don't need to lock interrupts here.
 *   The standalone QM-OS uses this variable.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KillSchedule
 *
 * The schedule is stopped and its state is set to STOPPED or QUARANTINED,
 * depending on the 2nd parameter.
 * A schedule never goes from the QUARANTINED to the STOPPED state as a
 * result of this call.
 *
 * Schedules chained after this table are also set to STOPPED.
*/
os_result_t OS_KillSchedule
(	os_scheduleid_t sid,
	os_schedulestatus_t state
)
{
	const os_schedule_t *ss;
	os_scheduledynamic_t *sd;
	os_scheduleid_t s;

	os_result_t r = OS_E_OK;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;

	s = sid;

	while ( s != OS_NULLSCHEDULE )
	{
		ss = &OS_scheduleTableBase[s];
		sd = ss->dynamic;

		/* Disabling interrupts here is OK for the current case when
		 * a ST can only be chained onto a running ST (ie. max chain length
		 * is 2. If longer chains are ever permitted this might need to
		 * be reconsidered
		*/
		/* Possible diagnostic TOOLDIAG-1 <1> */
		is = OS_IntDisable();

		sd->adjRemaining = 0;
		sd->next = 0;
		sd->status = state;

		/* OS_KillAlarm complains if the alarm wasn't in the queue, but this is
		 * not an error here - so we ignore the return value. */
		(void) OS_KillAlarm(ss->alarm, OS_ALARM_IDLE);

		s = sd->chain;
		sd->chain = OS_NULLSCHEDULE;

		/* See comment above by IntDisable()
		*/
		OS_IntRestore(is);

		/* All subsequent schedules in the chain are set to STOPPED,
		 * not QUARANTINED to prevent collateral damage. If they
		 * are also to be quarantined, that will be done explicitly
		*/
		state = OS_ST_STOPPED;
	}

	return r;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
