/* kern-synchronize.c
 *
 * This file contains the OS_Synchronize function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-synchronize.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_Synchronize
 *
 * This function performs the synchronisation adjustments necessary.
 * If no synchronisation is taking place (SYNCDIR==NOSYNC), the delta is returned
 * unchanged.
 *
 * The amount of adjustment is computed and then subtracted from the delta and from the
 * remaining adjustment. The amount of adjustment is nominally the smaller of the "max"
 * amount computed by SyncScheduleTable() and the amount remaining. If the length is
 * being shortened, however, the amount of adjustment cannot be more than the delta
 * value that is provided to this function.
 *
 * If the remaining adjustment goes to zero, the sync direction is set to NONE to shorten
 * future calls to thes function.
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation, 2
*/
os_tick_t OS_Synchronize
(	os_scheduledynamic_t *sd,			/* ST's dynamic data */
	const os_scheduleentry_t *nextp,	/* Next expiry point */
	os_tick_t delta						/* Computed delta before adjustment */
)
{
	os_tick_t adj;
	os_schedulestatus_t dir = (os_schedulestatus_t) (sd->status & OS_ST_SYNCDIR);

	if ( dir == OS_ST_NOSYNC )
	{
		/* No synchronising in progress.
		*/
	}
	else
	{
		/* We always make the largest adjustment permitted, unless otherwise restricted
		 *
		 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.Adjust, 1
		*/
		adj = ( dir == OS_ST_SHORTEN ) ? nextp->maxDecrease : nextp->maxIncrease;

		/* We never make more adjustment than what remains
		*/
		if ( adj > sd->adjRemaining )
		{
			adj = sd->adjRemaining;
		}

		if ( dir == OS_ST_SHORTEN )
		{
			/* Shortening the interval.
			 * The adjusted interval cannot go negative, as this would mean setting a time
			 * that is in the past, which is indistinguishable from a time that is in the distant future.
			 * The resulting delta might still be in the past when we try to program a
			 * hardware counter, but the timer driver will have to cope with that.
			*/
			if ( adj > delta )
			{
				adj = delta;
				delta = 0;
			}
			else
			{
				delta -= adj;
			}
		}
		else
		{
			/* Lengthening the interval.
			 * We can increase the interval by as much as we like subject to the limit computed above.
			*/
			delta += adj;
		}

		/* Deduct the computed adjustment from the amount remaining. If it goes to zero, clear the
		 * sync-direction flags but DON'T set the SYNCHRONOUS flag.
		*/
		sd->adjRemaining -= adj;

		if ( sd->adjRemaining <= 0U )
		{
			sd->status &= (os_schedulestatus_t) ~OS_ST_SYNCDIR;
		}
	}

	return delta;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
