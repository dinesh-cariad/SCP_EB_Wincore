/* kern-getcount.c
 *
 * This file contains the OS_GetCount function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getcount.c 27926 2017-11-21 17:19:25Z mist9353 $
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
 * OS_GetCount
 *
 * The current tick value of the counter is returned.
 *
 * For software counters it is simply the "current" value.
 *
 * For hardware counters, the time elapsed on the hardware must be added to the current value
 * and any alarms that expire as a reult must be processed. If the counter is locked, the
 * current value is returned. This means that the value will reflect the state of the alarm queue.
*/
os_tick_t OS_GetCount(const os_counter_t *cs)
{
	os_tick_t count;
	os_counterdynamic_t *cd;

	cd = cs->dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	if (OS_CounterIsHw(cs))
	{
		/* For a hardware counter that is not locked we need to update the counter.
		*/
		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (cd->lock == 0U)
		{
			OS_CtrUpdate(cs);
		}
	}

	OS_ATOMIC_ASSIGN_32(count, cd->current);

	return count;
}

#include <memmap/Os_mm_code_end.h>

/* API entries for documentation
 * CHECK: NOPARSE

<api API="OS_INTERNAL">
  <name>OS_GetCount</name>
  <synopsis>Get the current value of the counter</synopsis>
  <syntax>
    os_tick_t OS_GetCount
    (   const os_counter_t *cs /@ Counter's static data @/
    )
  </syntax>
  <description>
    <code>OS_GetCount()</code> returns the current value of the specified
    counter. If the counter is a hardware counter that is locked, the best
    approximation is the current value according to the counter's dynamic
    data. This happens if the function is called in an ISR that interrupts
    the counter ISR.
  </description>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
