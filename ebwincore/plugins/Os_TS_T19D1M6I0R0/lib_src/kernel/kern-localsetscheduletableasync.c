/* kern-localsetscheduletableasync.c
 *
 * This file contains the OS_LocalSetScheduleTableAsync() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localsetscheduletableasync.c 27881 2017-11-15 11:42:38Z miwi261726 $
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

#define OS_SID	OS_SID_SetScheduleTableAsync
#define OS_SIF	OS_svc_SetScheduleTableAsync

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalSetScheduleTableAsync implements the API SetScheduleTableAsync
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.GlobalTime, 1,
 * !        Kernel.Autosar.API.SystemServices.SetScheduleTableAsync, 1
*/
os_errorresult_t OS_LocalSetScheduleTableAsync(os_scheduleid_t s)
{
	os_scheduledynamic_t *sd;
	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;

	/* !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT, 1
	*/
	sd = OS_scheduleTableBase[s].dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	is = OS_IntDisable();

	if ( !OS_APPISACCESSIBLE(OS_scheduleTableBase[s].app) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( (sd->status & OS_ST_STATE) == OS_ST_RUNNING )
	{
		/* Clear the synchronous flag and the sync direction bits.
		 * No sync adjustment will be done as long as SYNCDIR remains 0.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.SetScheduleTableAsync.Running, 1,
		 * !        Kernel.Autosar.API.SystemServices.SetScheduleTableAsync.NoAdjust, 1
		*/
		sd->status &= (os_uint8_t)(~(OS_ST_SYNCHRONOUS | OS_ST_SYNCDIR));
	}
	else
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.SetScheduleTableAsync.State, 1
		*/
		result = OS_ERRORCODE_CHECK(OS_ERROR_NotRunning);
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
