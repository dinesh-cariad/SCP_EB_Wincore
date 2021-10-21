/* kern-localsyncscheduletable.c
 *
 * This file contains the OS_LocalSyncScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localsyncscheduletable.c 28260 2018-02-06 11:40:21Z olme8414 $
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

#define OS_SID	OS_SID_SyncScheduleTable
#define OS_SIF	OS_svc_SyncScheduleTable

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C checkers want prototypes for static helpers */
static os_errorresult_t OS_DoSyncRunningScheduleTable
	( const os_schedule_t *, os_scheduledynamic_t *, os_tick_t, os_intstatus_t );
static os_errorresult_t OS_DoSyncWaitingScheduleTable
	( const os_schedule_t *, os_scheduledynamic_t *, os_tick_t, os_intstatus_t );

/* Helper to synchronize a schedule table in RUNNING state.
 * The caller already has disabled interrupts. This helper may re-enable interrupts internally.
 * In any case, interrupts must be re-enabled before this helper exits.
 * This helper may report errors and therefore gets a parameter pointer from the caller.
*/
static os_errorresult_t OS_DoSyncRunningScheduleTable
(	const os_schedule_t *ss,
	os_scheduledynamic_t *sd,
	os_tick_t globalTime,
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_intstatus_t is			/* to re-enable interrupts */
)
{
	os_tick_t shorten;
	os_tick_t lengthen;
	os_tick_t local;
	os_tick_t delta;
	os_int_t gadRes;
	os_errorresult_t e = OS_ERROR_NoError;

	const os_scheduleentry_t *currp;
	os_counterid_t counter;
	os_tick_t err;

	/* Get time remaining on alarm -- internal version of OSEK/VDX GetAlarm().
	*/
	gadRes = OS_GetAlarmDelta(ss->alarm, &OS_alarmTableBase[ss->alarm], OS_alarmTableBase[ss->alarm].dynamic, &delta);

	if ( gadRes < 0 )
	{
		/* Serious error: the alarm list is corrupt!
		*/
		e = OS_PANIC(OS_PANIC_InvalidAlarmState);

		OS_IntRestore(is);
	}
	else
	if ( gadRes > 0 )
	{
		/* Alarm is not running. There are two possibilities:
		 * - the schedule table was stopped or came to an end. In this case the schedule
		 *   table is no longer running
		 * - SyncScheduleTable is called from an ISR that has interrupted
		 *   RunCounter/RunSchedule. In this case the schedule table is still
		 *   running.
		*/

		if ( (sd->status & OS_ST_STATE) == OS_ST_RUNNING )
		{
			/* Schedule table is active and in the middle of being processed.
			 * The alarm has expired, but RunScheduleTable() hasn't been called yet.
			 * This happens if an interrupt occurs in the short interrupts-open part of
			 * the alarm processing loop and the ISR calls SyncScheduleTable().
			 *
			 * In this case we simply set the alarm delta to zero. Setting gadRes to zero
			 * ensures that the synchronisation computation still proceeds.
			*/
			gadRes = 0;
			delta = 0;
		}
		else
		{
			/* Schedule table is no longer active and so cannot be synchronised.
			*/
			OS_IntRestore(is);

			e = OS_ERRORCODE_CHECK(OS_ERROR_NotRunning);
		}
	}
	else
	{
		/* gadRes is 0 ==> OK.
		 *
		 * BUT the delta could be outside the duration of the schedule table.
		 * This happens when (for example) StartScheduleTableRel() is called with an
		 * offset significantly bigger than the duration, then SyncScheduleTable() is called
		 * immediately afterwards.
		 *
		 * If this happens, we coerce the delta to be in the range 0..duration-1 using a modulo.
		 * This effectively discards (ignores) all the full rounds that must occur before the
		 * first expiry point when calculating the amount of adjustment needed. The first
		 * expiry point still happens when it has been programmed to happen, and adjustment
		 * starts after that.
		*/
		if ( delta >= ss->period )
		{
			delta = delta % ss->period;
		}
	}

	if ( gadRes == 0 )
	{
		/* Schedule table is running
		 *
		 * Clear out the direction and synchronous flags.
		 * They get recalculated in the following code.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable.Synchronous, 1
		 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Deviation, 1
		*/
		sd->status &= (os_schedulestatus_t) ~(OS_ST_SYNCDIR | OS_ST_SYNCHRONOUS);

		currp = &ss->table[sd->next];

		/* Compute local time as
		 * offset of next event minus time to next event
		*/
		if ( delta > currp->offset )
		{
			/* Time to next event is greater than time of event
			 * therefore event is in next round. Add period...
			*/
			local = (currp->offset - delta) + ss->period;
		}
		else
		{
			local = currp->offset - delta;
		}

		/* Take account of any unprocessed time while running the counter.
		*/
		counter = OS_alarmTableBase[ss->alarm].counter;
		err = OS_counterTableBase[counter].dynamic->error;

		if ( local >= err )
		{
			local -= err;
		}
		else
		{
			local = (local + ss->period) - err;
		}

		if ( globalTime == local )
		{
			/* Exactly in sync. Nothing to do!
			 *
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.QuerySynch.Criterion, 2
			*/
			sd->adjRemaining = 0;
			sd->status |= OS_ST_SYNCHRONOUS;
		}
		else
		{
			if ( globalTime > local )
			{
				/* Our table is running late, so we need to shorten
				 * the intervals until it is in sync again. However, it
				 * might be quicker to lengthen the intervals, depending
				 * on the exact relative values of global and local time,
				 * and on the MAX_INC/DEC values.
				*/
				shorten = globalTime - local;
				shorten %= ss->period;
				lengthen = ss->period - shorten;
			}
			else
			{
				/* Our table is running early, so we need to lengthen
				 * the intervals until it is in sync again. However, it
				 * might be quicker to shorten the intervals, depending
				 * on the exact relative values of global and local time,
				 * and on the MAX_INC/DEC values.
				*/
				lengthen = local - globalTime;
				lengthen %= ss->period;
				shorten = ss->period - lengthen;
			}

			if ( lengthen <= ss->tolerance )
			{
				/* ST is synchronous.
				*/
				sd->status |= OS_ST_SYNCHRONOUS;

				/* If there is some increase available in the round then we can attempt
				 * to bring the deviation down to zero. Otherwise we set the schedule
				 * table to "do not sync" to avoid going through the motion of adjusting by
				 * zero ticks on every expiry point.
				 *
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.QuerySynch.Criterion, 2
				*/
				if ( (ss->flags & OS_ST_SYNCINC) == 0u )
				{
					/* No increase available. If we can decrease and still stay in sync
					 * then we'll go that way round instead.
					*/
					if ( ((ss->flags & OS_ST_SYNCDEC) != 0u) && (shorten <= ss->tolerance) )
					{
						sd->status |= OS_ST_SHORTEN;
						sd->adjRemaining = shorten;
					}
					else
					{
						sd->adjRemaining = lengthen;
					}
				}
				else
				{
					/* Can increase length to reduce deviation. Can we also decrease it
					 * without going asynchronous?
					*/
					if ( ((ss->flags & OS_ST_SYNCDEC) != 0u) && (shorten <= ss->tolerance) )
					{
						/* We can synchronise in either direction without going asynchronous
						 * Which is quicker?
						*/
						if ( shorten < ss->dirCutoff )
						{
							/* ST is asynchronous. We can synchronise in fewer steps by
							 * shortening the period by maxDecrease each time.
							 *
							 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Deviation.Asymmetric, 1
							*/
							sd->status |= OS_ST_SHORTEN;
							sd->adjRemaining = shorten;
						}
						else
						{
							/* ST is asynchronous. We can synchronise in fewer steps by
							 * lengthening the period by maxIncrease each time.
							 *
							 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Deviation.Asymmetric, 1
							*/
							sd->status |= OS_ST_LENGTHEN;
							sd->adjRemaining = lengthen;
						}
					}
					else
					{
						sd->status |= OS_ST_LENGTHEN;
						sd->adjRemaining = lengthen;
					}
				}
			}
			else
			if ( shorten <= ss->tolerance )
			{
				/* ST is synchronous.
				*/
				sd->status |= OS_ST_SYNCHRONOUS;
				sd->adjRemaining = shorten;

				/* If there is some decrease available in the round then we can attempt
				 * to bring the deviation down to zero. Otherwise we set the schedule
				 * table to "do not sync" to avoid going throuhg the motion of adjusting by
				 * zero ticks on every expiry point.
				 *
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.QuerySynch.Criterion, 2
				*/
				if ( (ss->flags & OS_ST_SYNCDEC) != 0u )
				{
					sd->status |= OS_ST_SHORTEN;
				}
			}
			else
			if ( shorten < ss->dirCutoff )
			{
				/* ST is asynchronous. We can synchronise in fewer steps by
				 * shortening the period by maxDecrease each time.
				 *
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Deviation.Asymmetric, 1
				*/
				sd->status |= OS_ST_SHORTEN;
				sd->adjRemaining = shorten;
			}
			else
			{
				/* ST is asynchronous. We can synchronise in fewer steps by
				 * lengthening the period by maxIncrease each time.
				 *
				 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Deviation.Asymmetric, 1
				*/
				sd->status |= OS_ST_LENGTHEN;
				sd->adjRemaining = lengthen;
			}
		}

		OS_IntRestore(is);
	}

	return e;
}

/* Helper to synchronize a schedule table in WAITING state.
 * The caller already has disabled interrupts. This helper may re-enable interrupts internally.
 * In any case, interrupts must be re-enabled before this helper exits.
*/
static os_errorresult_t OS_DoSyncWaitingScheduleTable
(	const os_schedule_t *ss,
	os_scheduledynamic_t *sd,
	os_tick_t globalTime,
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_intstatus_t is			/* to re-enable interrupts */
)
{
	const os_alarm_t *as = &OS_alarmTableBase[ss->alarm];
	os_alarmdynamic_t *ad = as->dynamic;
	const os_counter_t *cs = &OS_counterTableBase[as->counter];
	os_counterdynamic_t *cd = cs->dynamic;

	os_tick_t offset;
	os_tick_t maxallowedvalue;

	os_errorresult_t e = OS_ERROR_NoError;

	/* Schedule table starts synchronously and is waiting for
	 * global time to be available.
	 *
	 * Global time is now available.
	*/

	/* We need to know whether we can still get the first
	 * event in this round.
	*/
	if ( globalTime < ss->table->offset )
	{
		offset = ss->table->offset - globalTime;
	}
	else
	{
		offset = (ss->table->offset - globalTime) + ss->period;
	}

	maxallowedvalue = cs->maxallowedvalue;

	if ( offset > maxallowedvalue )
	{
		/* The delay required is beyond the reach of the
		 * counter. We must do it in short steps
		*/
		sd->adjRemaining = offset - maxallowedvalue;
		offset = maxallowedvalue;
		sd->status = OS_ST_WAITING | OS_ST_SYNCHRONOUS;
	}
	else
	{
		sd->adjRemaining = 0;
		sd->status = OS_ST_RUNNING | OS_ST_SYNCHRONOUS;
	}

	/* Simply set the alarm running.
	*/
	ad->inUse = OS_ALARM_INUSE;
	ad->period = 0;

	/* Get an up-to-date idea of the current
	 * timer value. For software counters and locked hardware counters,
	 * the latest "current" value is good enough.
	 * This might mean that some alarms disappear from the queue.
	*/
	/* Possible diagnostic TOOLDIAG-1 <1> */
	if (OS_CounterIsHw(cs))
	{
		/* Possible diagnostic TOOLDIAG-3 <1> */
		if (cd->lock == 0u)
		{
			OS_IntRestore(is);
			OS_CtrUpdate(cs);
			/* Possible diagnostic TOOLDIAG-2 <1> */
			is = OS_IntDisable();
		}
	}

	/* Calculate an appropriate delta
	*/
	ad->delta = offset;
	sd->ctrAbs = OS_CounterAdd(cd->current, offset, cs->maxallowedvalue);

	OS_EnqueueAlarm(cs, cd, ss->alarm, ad);

	OS_IntRestore(is);

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if (OS_CounterIsHw(cs))
	{
		/* Possible diagnostic TOOLDIAG-3 <1> */
		if ((cd->lock == 0u) && (cd->head == ss->alarm))
		{
			OS_CtrUpdate(cs);
		}
	}

	return e;
}

/*!
 * OS_LocalSyncScheduleTable
 *
 * The synchronisation variables (status and adjRemaining) of the
 * schedule table are set so as to bring the schedule table into
 * synchronisation with global time, either gradually or all at once
 * depending on the max decrease and increase attributes of the
 * schedule table.
 */
os_errorresult_t OS_LocalSyncScheduleTable
(	const os_schedule_t *ss,
	os_tick_t globalTime
)
{
	os_scheduledynamic_t * sd;
	os_errorresult_t e;
	os_intstatus_t is;
	os_schedulestatus_t state;

	sd = ss->dynamic;

	/* Possible diagnostic TOOLDIAG-2 <1> */
	is = OS_IntDisable();

	state = (os_schedulestatus_t) (sd->status & OS_ST_STATE);

	if ( !OS_APPISACCESSIBLE(ss->app) )
	{
		OS_IntRestore(is);

		e = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( state == OS_ST_RUNNING )
	{
		e = OS_DoSyncRunningScheduleTable(ss, sd, globalTime, is);
	}
	else
	if ( state == OS_ST_WAITING )
	{
		e = OS_DoSyncWaitingScheduleTable(ss, sd, globalTime, is);
	}
	else
	{
		/* Schedule table is not active and so cannot be synchronised.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable.NotStarted, 2
		*/
		OS_IntRestore(is);

		e = OS_ERRORCODE_CHECK(OS_ERROR_NotRunning);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
