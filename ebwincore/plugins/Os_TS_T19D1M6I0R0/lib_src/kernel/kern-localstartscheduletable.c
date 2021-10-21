/* kern-localstartscheduletable.c
 *
 * This file contains the OS_LocalStartScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localstartscheduletable.c 27926 2017-11-21 17:19:25Z mist9353 $
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

#define OS_SID		varSid
#define OS_SID_ABS	OS_SID_StartScheduleTableAbs
#define OS_SID_REL	OS_SID_StartScheduleTableRel

/* This macro is used to pick the error code if error handling is disabled.
*/
#define OS_SID_EC	OS_SID_StartScheduleTableAbs

#define OS_SIF		*varSif
#define OS_SIF_ABS	OS_svc_StartScheduleTableAbs
#define OS_SIF_REL	OS_svc_StartScheduleTableRel

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernStartScheduleTable implements the APIs StartScheduleTableRel and StartScheduleTableAbs
 *
 * The specified schedule table is started such that the first event happens
 * either at time "offset" now or when the counter reaches "offset", depending
 * on the value of "rel".
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.StartAbsolute, 2
 * !LINKSTO Kernel.Autosar.ScheduleTable.StartRelative, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel, 2
*/
os_errorresult_t OS_LocalStartScheduleTable
(	const os_schedule_t *ss,
	os_counterid_t c,
	os_tick_t offset,
	os_boolean_t rel
)
{
	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
	os_scheduledynamic_t *sd;
	os_alarmdynamic_t *ad;
	const os_counter_t *cs;
	os_counterdynamic_t *cd;
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_intstatus_t is;

	sd = ss->dynamic;
	ad = OS_alarmTableBase[ss->alarm].dynamic;
	cs = &OS_counterTableBase[c];
	cd = cs->dynamic;

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
		if (cd->lock == 0)
		{
			OS_CtrUpdate(cs);
		}
	}

	/* Possible diagnostic TOOLDIAG-2 <1> */
	is = OS_IntDisable();

	if ( !OS_APPISACCESSIBLE(ss->app) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( sd->status != OS_ST_STOPPED )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs.Started, 2
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.Started, 2
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_ScheduleTableNotIdle);
	}
	else
	if ( ad->inUse != OS_ALARM_IDLE )
	{
		/* This should never happen.
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_AlarmInUse);
	}
	else
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs.State, 1
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.State, 1
		*/
		sd->adjRemaining = OS_MAXTICK;
		sd->chain = OS_NULLSCHEDULE;
		sd->next = 0;
		sd->status = OS_ST_RUNNING;
		if ( (ss->flags & OS_ST_IMPLICIT) != 0 )
		{
			/* Implicit-sync schedule tables are always synchronous! */
			sd->status |= OS_ST_SYNCHRONOUS;
		}
		ad->inUse = OS_ALARM_INUSE;
		ad->period = 0;

		/* Calculate an appropriate delta
		*/
		if ( rel )
		{
			/* Relative value: the delta is simply the given plus the offset of the first
			 * expiry point (modulo counter wrap)
			*/
			ad->delta = OS_CounterAdd(offset, ss->table->offset, cs->maxallowedvalue);
			sd->ctrAbs = OS_CounterAdd(cd->current, ad->delta, cs->maxallowedvalue);
		}
		else
		{
			/* Absolute value: delta is time from current counter value to given value
			 * Delta of zero (absvalue equals current counter) means immediate expiry.
			*/
			sd->ctrAbs = OS_CounterAdd(offset, ss->table->offset, cs->maxallowedvalue);
			ad->delta = OS_CounterSub(sd->ctrAbs, cd->current, cs->maxallowedvalue);
		}

		OS_EnqueueAlarm(cs, cd, ss->alarm, ad);

		/* If our alarm is  at the beginning of the queue of a hardware counter we need
		 * to adjust the timer's time-to-next-interrupt to reflect this.
		 * (But not if the counter is locked)
		*/
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_CounterIsHw(cs))
		{
			/* Possible diagnostic TOOLDIAG-3 <1> */
			if ((cd->lock == 0) && (cd->head == ss->alarm))
			{
				OS_CtrUpdate(cs);
			}
		}
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
