/* kern-ratemonitor.c
 *
 * This file contains the OS_RateMonitor function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-ratemonitor.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RateMonitor
 *
 * The specified rate-monitoring object is checked to see if there
 * have been more occurrences than permitted in the last time period.
 *
 * To do this, a "ring buffer" of timer values is used. The current
 * time is compared with the current slot to determine if the rate limit
 * has been exceeded. Afterwards, the current time is recorded in the current
 * slot and the slot index is advanced.
 * A rate error is signalled by returning TRUE. A return value of FALSE
 * indicates that the arrival rate limit has not been exceeded.
 *
 * The values of the slots should be initialised so that the times are
 * guaranteed to be more than the timelimit away. An initial value of
 * current time - timelimit will suffice.
 *
 * The spec is silent about whether a failed arrival should count as an
 * arrival for rate-monitoring purposes. The code here assumes that it does
 * and the ring buffer is updated even if the frequency is too high. This
 * also covers the possibility that the caller might choose to ignore
 * the error under certain circumstances.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.RateLimit, 1
*/
os_boolean_t OS_RateMonitor(os_ratemonitorid_t rmi)
{
	const os_ratemonitor_t *rm;
	os_rateindex_t *ri;
	os_timestamp_t now;
	os_timestamp_t frameEnd;
	os_boolean_t result = OS_FALSE;
	os_rateindex_t i;

	rm = &OS_rateMonitor[rmi];
	ri = OS_GetRateIndex(rmi);

	OS_GetTimeStamp(&now);

	i = *ri;

	/* Calculate time when "current" time frame ends
	*/
	OS_TimeAdd32(&frameEnd, rm->ringBuffer[i], rm->timeLimit);

	/* If "now" is before "frameEnd" then there's an arrival-rate violation
	 * If the ring buffer entry is zero this is the initial condition and must not trigger
	 * a rate exceeded error. The TimeIsZero() condition is placed second so that it isn't
	 * evaluated in the "normal" case of the rate limit not being exceeded, and so doesn't
	 * add execution time to the "normal" case.
	*/
	if ( (OS_TimeLt(now, frameEnd)) && (!OS_TimeIsZero(rm->ringBuffer[i])) )
	{
		/* Rate limit exceeded
		*/
		result = OS_TRUE;
	}
	else
	{
		/* Rate limit not exceeded. Note start of new time frame
		*/
		OS_TimeCopy(&rm->ringBuffer[i], now);

		/* Move index to next time frame
		*/
		i++;

		if ( i >= rm->countLimit )
		{
			*ri = 0;
		}
		else
		{
			*ri = i;
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
