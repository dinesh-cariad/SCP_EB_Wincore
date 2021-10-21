/* kern-localcancelalarm.c
 *
 * This file contains the OS_LocalCancelAlarm() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localcancelalarm.c 27881 2017-11-15 11:42:38Z miwi261726 $
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

#define OS_SID OS_SID_CancelAlarm
#define OS_SIF OS_svc_CancelAlarm

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalCancelAlarm
 *
 * The alarm 'a' is cancelled, provided it is currently in use.
*/
os_errorresult_t OS_LocalCancelAlarm(os_alarmid_t a, const os_alarm_t *as)
{
	os_errorresult_t e;
	os_alarmdynamic_t *ad;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t is;

	ad = as->dynamic;

	/* Possible diagnostic TOOLDIAG-1 <1> */
	is = OS_IntDisable();

	if ( !OS_APPISACCESSIBLE(as->app) )
	{
		e = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( ad->inUse == OS_ALARM_INUSE )
	{
		/* !LINKSTO Kernel.API.Alarms.CancelAlarm.CancelAlarm, 1
		*/
		e = OS_KillAlarm(a, OS_ALARM_IDLE);
	}
	else
	{
		/* !LINKSTO Kernel.API.Alarms.CancelAlarm.UnusedAlarm, 1
		*/
		e = OS_ERRORCODE_CHECK(OS_ERROR_AlarmNotInUse);
	}

	OS_IntRestore(is);

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
