/* kern-localstopscheduletable.c
 *
 * This file contains the OS_LocalStopScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localstopscheduletable.c 27881 2017-11-15 11:42:38Z miwi261726 $
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

#define OS_SID OS_SID_StopScheduleTable
#define OS_SIF OS_svc_StopScheduleTable

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalStopScheduleTable
 *
 * The specified schedule table 's' is stopped immediately.
*/
os_errorresult_t OS_LocalStopScheduleTable(os_scheduleid_t s, const os_schedule_t *ss)
{
	os_scheduledynamic_t *sd;
	os_scheduleid_t chain;
	os_schedulestatus_t oldstatus;

	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;

	sd = ss->dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	is = OS_IntDisable();

	oldstatus = (os_schedulestatus_t) (sd->status & OS_ST_STATE_X);

	if ( !OS_APPISACCESSIBLE(ss->app) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( oldstatus == OS_ST_STOPPED )
	{
		/* Stopped or quarantined.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StopScheduleTable.NotStarted, 2
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_NotRunning);
	}
	else
	{
		chain = sd->chain;

		sd->adjRemaining = 0;
		sd->chain = OS_NULLSCHEDULE;
		sd->next = 0;
		sd->status = OS_ST_STOPPED;

		if ( oldstatus == OS_ST_RUNNING )
		{
			/* OS_KillAlarm complains if the alarm wasn't in the queue, but this is
			 * not an error here - so we ignore the return value. */
			(void) OS_KillAlarm(ss->alarm, OS_ALARM_IDLE);
		}
		else
		if ( oldstatus == OS_ST_CHAINED )
		{
			/* If the schedule table is chained to another, we need to find the other
			 * in order to break the link.
			*/
			os_scheduleid_t i;
			for (i = 0; i < OS_nSchedules; i++)
			{
				sd = OS_scheduleTableBase[i].dynamic;
				if (sd->chain == s)
				{
					/* Found the chainer - break the link and stop the loop
					*/
					sd->chain = OS_NULLSCHEDULE;
					break;
				}
			}
		}
		else
		{
			/* MISRA-C */
		}

		/* Now walk the list of chained schedule tables and set
		 * each one to STOPPED.
		 * At the time of writing, Autosar does not permit more than
		 * one schedule table to be chained because the table to
		 * chain *to* must be running. However, this loop copes
		 * with the possibility that this limitation might be removed
		 * in the future.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StopScheduleTable.HasChained, 1
		*/
		while ( chain != OS_NULLSCHEDULE )
		{
			sd = OS_scheduleTableBase[chain].dynamic;
			sd->status = OS_ST_STOPPED;
			chain = sd->chain;
			sd->chain = OS_NULLSCHEDULE;
		}
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
