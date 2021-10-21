/* kern-localchainscheduletable.c
 *
 * This file contains the OS_LocalChainScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localchainscheduletable.c 27881 2017-11-15 11:42:38Z miwi261726 $
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

#define OS_SID OS_SID_ChainScheduleTable
#define OS_SIF OS_svc_ChainScheduleTable

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalChainScheduleTable implements the API ChainScheduleTable
 *
 * The specified "next" schedule table is chained to be started when the
 * specified "current" schedule table's current period finishes.
 *
 * Checks:
 * - next table to chain must be stopped.
 * - current table must be running
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Chain, 2
*/
os_errorresult_t OS_LocalChainScheduleTable
(	os_scheduleid_t sc,
	os_scheduleid_t sn
)
{
	os_scheduledynamic_t *scd;
	os_scheduledynamic_t *snd;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;
	os_boolean_t curIsAccessible, nextIsAccessible;
	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);

	scd = OS_scheduleTableBase[sc].dynamic;
	snd = OS_scheduleTableBase[sn].dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	is = OS_IntDisable();

	/* Both applications have to be accessible.
	*/
	curIsAccessible = OS_APPISACCESSIBLE(OS_scheduleTableBase[sc].app);
	nextIsAccessible = OS_APPISACCESSIBLE(OS_scheduleTableBase[sn].app);
	if ( (!curIsAccessible) || (!nextIsAccessible) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( ( (scd->status & OS_ST_STATE) != OS_ST_RUNNING ) &&
		 ( (scd->status & OS_ST_STATE) != OS_ST_WAITING ) )
	{
		/* Current table is not running
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.ChainScheduleTable.NotStarted, 2
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_NotRunning);
	}
	else
	if ( snd->status != OS_ST_STOPPED )
	{
		/* Table to chain is not in STOPPED state
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.ChainScheduleTable.NotIdle, 2
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_NotStopped);
	}
	else
	{
		if ( scd->chain != OS_NULLSCHEDULE )
		{
			/* If the current ST already has a chained ST, the new one replaces it. (OS324 - Autosar 2.0)
			 * The previously-chained ST must be set to STOPPED.
			 *
			 * If at some future date a proper chain of schedule tables is allowed, we'll have to
			 * walk the chain here and set them all to STOPPED
			*/
			OS_scheduleTableBase[scd->chain].dynamic->status = OS_ST_STOPPED;
		}

		scd->chain = sn;

		snd->status = OS_ST_CHAINED;
		snd->adjRemaining = 0;
		snd->chain = OS_NULLSCHEDULE;
		snd->next = 0;
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
