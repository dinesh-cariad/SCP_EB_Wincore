/* kern-hwcounterupdate.c
 *
 * This file contains the OS_HwCounterUpdate function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-hwcounterupdate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_HwCounterUpdate
 *
 * This function is called whenver a HARDWARE counter needs to be updated. This happens when
 * the interrupt occurs. It also happens when an API function needs to know the current
 * value of the counter.
 *
 * This version of the function doesn't support the generic timestamp. For the timer to which
 * the generic timestamp is attached, OS_HwCounterTimestampUpdate() should be called instead.
 *
 * See also OS_HwCounterTimestampUpdate() in file kern-hwcountertimestampupdate.c
 *
 * This is an internal function that is only ever called from kernel or
 * generated code, so no parameter checking is done.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.HARDWARE, 1
*/
void OS_HwCounterUpdate(const os_counter_t *cs)
{
	os_counterdynamic_t *cd;
	const os_hwt_t *hwt;
	os_timervalue_t current;
	os_tick_t delta;
	os_tick_t nTicks;
	os_boolean_t again;
	os_intstatus_t is;

	cd = cs->dynamic;
	hwt = cs->hwt;

	is = OS_IntDisable();

	OS_HwtStop(hwt);

	do {
		/* Calculate number of ticks since last time we looked
		 * Remember current timer value for the next update.
		*/
		current = OS_HwtRead(hwt);
		nTicks = OS_HwtSub(hwt, current, *hwt->lastValue);
		*hwt->lastValue = current;

		/* Advance the counter by that number of ticks. The function returns the delta
		 * of the first alarm in the queue, or zero if there isn't one.
		*/
		delta = OS_AdvanceCounter(cs, cd, nTicks, is);

		/* Loop stops when if delta is zero (no more alarms in queue)
		 * Loop stops when OS_HwtStart reports that it has set a time in the future.
		*/
		if ( delta == 0U )
		{
			again = OS_FALSE;
		}
		else
		{
			/* Limit the delta to what the timer can manage.
			*/
			if ( delta > hwt->maxDelta )
			{
				delta = hwt->defDelta;
			}

			again = OS_HwtStart(hwt, *hwt->lastValue, delta);
		}
	} while ( again );

	OS_IntRestore(is);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
