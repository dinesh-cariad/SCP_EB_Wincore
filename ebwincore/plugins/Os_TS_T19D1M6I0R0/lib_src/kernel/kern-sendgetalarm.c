/* kern-sendgetalarm.c
 *
 * This file contains the OS_SendGetAlarm() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendgetalarm.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendGetAlarm()
 *
 * Sends the GetAlarm() service call to the respective core on which the alarm 'a' resides.
*/
os_errorresult_t OS_SendGetAlarm(os_alarmid_t a, const os_alarm_t *as, os_tick_t *out)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)a;
	msgParam[1] = (os_paramtype_t)out;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheInvalidate(out, sizeof(*out));
#endif

	return OS_SendMessageWait(OS_GetAlarmCoreId(as), OS_XC_OP_GetAlarm, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
