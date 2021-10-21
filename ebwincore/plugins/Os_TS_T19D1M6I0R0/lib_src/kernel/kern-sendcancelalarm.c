/* kern-sendcancelalarm.c
 *
 * This file contains the OS_SendCancelAlarm() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendcancelalarm.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendCancelAlarm()
 *
 * Sends a CancelAlarm() call to the core on which the alarm 'a' resides.
*/
os_errorresult_t OS_SendCancelAlarm(os_alarmid_t a, const os_alarm_t *as)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)a;

	return OS_SendMessageWait(OS_GetAlarmCoreId(as), OS_XC_OP_CancelAlarm, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
