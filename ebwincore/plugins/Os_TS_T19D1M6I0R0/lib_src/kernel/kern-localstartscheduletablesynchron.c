/* kern-localstartscheduletablesynchron.c
 *
 * This file contains the OS_LocalStartScheduleTableSynchron function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localstartscheduletablesynchron.c 27818 2017-11-08 16:44:06Z mist9353 $
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

#define OS_SID OS_SID_StartScheduleTableSynchron
#define OS_SIF OS_svc_StartScheduleTableSynchron

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalStartScheduleTableSynchron
 *
 * The specified schedule table is placed in the "waiting" state
 * and will start only when SyncScheduleTable is called for it.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron, 2
*/
os_errorresult_t OS_LocalStartScheduleTableSynchron
(	const os_schedule_t *ss
)
{
	os_scheduledynamic_t *sd;
	os_alarmdynamic_t *ad;
	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;

	/* !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT, 1
	*/
	sd = ss->dynamic;
	ad = OS_alarmTableBase[ss->alarm].dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	is = OS_IntDisable();

	if ( !OS_APPISACCESSIBLE(ss->app) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( sd->status != OS_ST_STOPPED )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron.State, 2
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
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron.NewState, 1
		*/
		sd->adjRemaining = OS_MAXTICK;
		sd->chain = OS_NULLSCHEDULE;
		sd->next = 0;
		sd->status = OS_ST_WAITING;
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
