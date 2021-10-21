/* kern-advancecounter.c
 *
 * This file contains the OS_AdvanceCounter function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-advancecounter.c 28273 2018-02-07 13:25:23Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   unused parameter 'is'
 *
 * Reason: Not an issue. This variable isn't used in Safety OS.
 *   The microkernel serializes all calls to the lower integrity OS server.
 *   Therefore we don't need to lock interrupts here.
 *   Standalone AutoCore OS uses this variable.
 *
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_AdvanceCounter
 *
 * This function is called in order to advance a counter by a specified number of ticks.
 * The return value is the number of ticks remaining (delta) of the first alarm in the queue, or
 * 0 if there are no alarms.
 *
 * On entry: interrupts are disabled. The "enable" level is passed as a parameter.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.SOFTWARE, 1
 * !LINKSTO Kernel.Autosar.Counter.Types.HARDWARE, 1
*/
os_tick_t OS_AdvanceCounter(
	const os_counter_t *cs,
	os_counterdynamic_t *cd,
	os_tick_t adv,
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is)
{
	os_alarmid_t a;
	os_alarmdynamic_t *ad;
	os_tick_t returnTicks = 0;

	/* Add the number-of-ticks-to-advance to the current counter value
	*/
	cd->current = OS_CounterAdd(cd->current, adv, cs->maxallowedvalue);
	cd->error += adv;

	/* If the counter is locked, just add the no-of-ticks to the error and let the
	 * background processing take care of the alarm queue. This takes care of incrementing
	 * software counter in a high-priority interrupt while it is being incremented in a
	 * task or lower-priority interrupt.
	*/
	if ( cd->lock == 0u )
	{
		cd->error = adv;
		cd->lock = (os_uint8_t)OS_TRUE;

		/* Head alarm in queue (if any)
		*/
		a = cd->head;

		while (a != OS_NULLALARM)
		{
			ad = OS_alarmTableBase[a].dynamic;

			if (ad->delta > cd->error)
			{
				/* There is an alarm remaining in the queue. Subtract the leftover error
				 * error from its delta and exit the while loop.
				*/
				ad->delta -= cd->error;
				returnTicks = ad->delta;
				a = OS_NULLALARM; /* no need to go further */
			}
			else
			{
				/* Remember the alarm and detach it from the queue
				*/
				cd->head = ad->next;
				ad->next = OS_NULLALARM;

				/* Subtract the alarm's delta from our running error
				*/
				cd->error -= ad->delta;

				/* If the alarm is periodic, requeue it. Otherwise set it
				* to IDLE
				*/
				ad->delta = ad->period;
				if ( ad->delta == 0u )
				{
					/* Nonperiodic
					*
					* !LINKSTO Kernel.API.Alarms.SetRelAlarm.SingleAlarm, 1
					* !LINKSTO Kernel.API.Alarms.SetAbsAlarm.SingleAlarm, 1
					*/
					ad->inUse = OS_ALARM_IDLE;
				}
				else
				{
					/* Periodic
					*
					* !LINKSTO Kernel.API.Alarms.SetRelAlarm.CyclicAlarm, 1
					* !LINKSTO Kernel.API.Alarms.SetAbsAlarm.CyclicAlarm, 1
					*/
					OS_EnqueueAlarmNoAdjust(cd, a, ad);
				}

				/* Now perform the alarm's action. Interrupts are enabled first because
				 * the wrappers expect this. The alarm and the counter are in a consistent
				 * state, apart from the hardware timer, but updating the timer is
				 * inhibited by the lock flag.
				*/
				OS_IntRestore(is);
				(OS_alarmTableBase[a].wrapper)(&OS_alarmTableBase[a]);
				(void) OS_IntDisable();	/* Old level discarded */

				/* New head alarm in queue (if any)
				*/
				a = cd->head;
			}
		} /* while (a != OS_NULLALARM) */

		/* Set the error to zero. If there's an alarm in the queue we had accounted
		 * for the error. If not, we don't want any leftover error upsetting future
		 * SetxxxAlarm calls.
		*/
		cd->error = 0;

		/* Finally, unlock the counter
		*/
		cd->lock = (os_uint8_t)OS_FALSE;
	}

	return returnTicks;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
