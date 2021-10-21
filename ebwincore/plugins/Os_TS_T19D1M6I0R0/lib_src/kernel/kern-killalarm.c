/* kern-killalarm.c
 *
 * This file contains the OS_KillAlarm function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-killalarm.c 27926 2017-11-21 17:19:25Z mist9353 $
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
 * TOOLDIAG-2) Possible diagnostic: UnusedVariable
 *   Dead assignment eliminated.
 *
 * Reason: Not an issue. This variable isn't used in the Safety OS. The Safety OS
 *   serializes all QM-OS calls. Therefore we don't need to lock interrupts here.
 *   The standalone QM-OS uses this variable.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason: See TOOLDIAG-1.
*/

#define OS_SID	OS_SID_KillAlarm
#define OS_SIF	OS_svc_KillAlarm

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernKillAlarm
 *
 * The alarm is cancelled and its state is set to IDLE or QUARANTINED,
 * depending on the 2nd parameter.
 * An alarm never goes from the QUARANTINED to the IDLE state as a
 * result of this call.
*/
os_result_t OS_KillAlarm(os_alarmid_t a, os_uint8_t s)
{
	os_result_t r = OS_E_OK;
	const os_alarm_t *as;
	os_alarmdynamic_t *ad;
	const os_counter_t *cs;
	os_counterdynamic_t *cd;
	os_alarmid_t ap;
	os_alarmdynamic_t *pp;
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_intstatus_t is;

	as = &OS_alarmTableBase[a];
	ad = as->dynamic;
	cs = &OS_counterTableBase[as->counter];
	cd = cs->dynamic;

	/* Possible diagnostic TOOLDIAG-2 <1> */
	is = OS_IntDisable();

	if ( ad->inUse == OS_ALARM_INUSE )
	{
		/* Find the predecessor of the alarm in the list.
		*/
		ap = cd->head;
		pp = OS_NULL;

		while ( (ap < OS_totalAlarms) && (ap != a) )
		{
			pp = OS_alarmTableBase[ap].dynamic;
			ap = pp->next;
		}

		if ( ap == a )
		{
			/* !LINKSTO Kernel.API.Alarms.CancelAlarm.CancelAlarm, 1
			*/
			/* pp now points to the alarm before ours in the counter
			 * queue, or OS_NULL if there isn't one.
			 * Dequeue our alarm.
			*/
			if ( pp == OS_NULL )
			{
				cd->head = ad->next;
			}
			else
			{
				pp->next = ad->next;
			}

			/* If there's an alarm after ours in the queue, we must add
			 * our remaining delta to its delta to maintain its correct
			 * timing
			*/
			if ( ad->next < OS_totalAlarms )
			{
				OS_alarmTableBase[ad->next].dynamic->delta += ad->delta;
			}

			/* If the counter is a hardware timer and our alarm was first in the queue
			 * we should reprogram it with the new delta. We never operate
			 * on a locked counter.
			*/
			/* Possible diagnostic TOOLDIAG-1 <1> */
			if ( (pp == OS_NULL) && OS_CounterIsHw(cs) && (cd->lock == 0u) )
			{
				/* Possible diagnostic TOOLDIAG-3 <1> */
				OS_CtrUpdate(cs);
			}
		}
		else
		{
			OS_PARAMETERACCESS_DECL

			OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);
			OS_SAVE_PARAMETER_N(1,(os_paramtype_t)s);

			r = OS_ERROR(OS_ERROR_AlarmNotInQueue, OS_GET_PARAMETER_VAR());
		}

		ad->inUse = s;
		ad->period = 0;
		ad->delta = 0;
		ad->next = OS_NULLALARM;
	}
	else
	{
		ad->inUse = s;
	}

	OS_IntRestore(is);

	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_INTERNAL">
  <name>OS_KillAlarm</name>
  <synopsis>Kill an alarm</synopsis>
  <syntax>
    os_result_t OS_KillAlarm
    (   os_alarmid_t a,    /@ ID of the alarm @/
        os_uint8_t s       /@ New state @/
    )
  </syntax>
  <description>
    <code>OS_KillAlarm()</code> resets the expiration time of
    the specified alarm and sets its state as required.
  </description>
  <availability>
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
