/* kern-localsetalarm.c
 *
 * This file contains the OS_LocalSetAlarm() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localsetalarm.c 27926 2017-11-21 17:19:25Z mist9353 $
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

/* OS_LocalSetAlarm doesn't report errors itself. However it is called
 * from OS_KernSetRelAlarm and OS_KernSetAbsAlarm and returns
 * error codes to these.
 * Hence we reuse the service id from one of both, since both error codes
 * must be identical.
 * Note, all this is relevant for asc_Os/tools/errdb-to-c.pl
 */
#define OS_SID	OS_SID_SetRelAlarm

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalSetAlarm
 *
 * The alarm is set to expire in the specified number of timer ticks.
 * The alarm's period is also set as specified.
 *
 * If the increment is 0, the OSEK/VDX OS specification says it is up
 * to the implementation what to do. Here, we have chosen to expire the
 * alarm immediately.
*/
os_errorresult_t OS_LocalSetAlarm
(	os_alarmid_t a,				/* The alarm (index) */
	const os_alarm_t *as,		/* The alarm (static stuff) */
	const os_counter_t *cs,		/* The counter (static stuff) */
	os_tick_t val,				/* The relative or absolute counter value */
	os_tick_t per,				/* The period */
	os_boolean_t rel			/* TRUE if val is a relative value */
)
{
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_intstatus_t is;
	os_errorresult_t result;
	os_alarmdynamic_t *ad;
	os_counterdynamic_t *cd;

	ad = as->dynamic;
	cd = cs->dynamic;

	if ( !OS_APPISACCESSIBLE(as->app) )
	{
		result = OS_ERRORCODE_NOCHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	{
		/* For a hardware counter we need the current value from the hardware,
		 * and we need to update the delta of the head alarm in the queue.
		 *
		 * This is done before disabling interrupts. If a timer interrupt
		 * happens here it is safe.
		 *
		 * If the counter is locked the value is already current.
		*/
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_CounterIsHw(cs))
		{
			/* Possible diagnostic TOOLDIAG-3 <1> */
			if (cd->lock == 0u)
			{
				OS_CtrUpdate(cs);
			}
		}

		/* Possible diagnostic TOOLDIAG-2 <1> */
		is = OS_IntDisable();

		/* Remember the current state of the alarm. This is for error handling
		 * in the caller.
		*/
		result = ad->inUse;

		/* If the alarm is still available, enqueue it
		*/
		if ( result == OS_ALARM_IDLE )
		{
			ad->inUse = OS_ALARM_INUSE;
			ad->period = per;

			/* Calculate an appropriate delta
			*/
			if ( rel )
			{
				/* Relative value: the delta is simply the given value
				*/
				ad->delta = val;
			}
			else
			{
				/* Absolute value: compute the delta with wraparound.
				*/
				ad->delta = OS_CounterSub(val, cd->current, cs->maxallowedvalue);
			}

			OS_EnqueueAlarm(cs, cd, a, ad);
			OS_IntRestore(is);
		}
		else
		{
			OS_IntRestore(is);

			if (result == OS_ALARM_INUSE)
			{
				/* !LINKSTO Kernel.API.Alarms.SetRelAlarm.AlreadyInUse, 1,
				 * !        Kernel.API.Alarms.SetAbsAlarm.AlreadyInUse, 1
				*/
				result = OS_ERRORCODE_NOCHECK(OS_ERROR_AlarmInUse);
			}
			else if (result == OS_ALARM_QUARANTINED)
			{
				result = OS_ERRORCODE_NOCHECK(OS_ERROR_Quarantined);
			}
			else
			{
				/* MISRA */
			}
		}

		/* If our alarm is at the beginning of the queue of a hardware counter we need
		 * to adjust the timer's time-to-next-interrupt to reflect this.
		 * (But not if the counter is locked)
		*/
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_CounterIsHw(cs))
		{
			/* Possible diagnostic TOOLDIAG-3 <1> */
			if ((cd->lock == 0u) && (cd->head == a))
			{
				OS_CtrUpdate(cs);
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
