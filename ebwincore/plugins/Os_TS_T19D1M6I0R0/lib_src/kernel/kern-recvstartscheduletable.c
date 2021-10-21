/* kern-recvstartscheduletable.c
 *
 * This file contains the function OS_RecvStartScheduleTable().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvstartscheduletable.c 27881 2017-11-15 11:42:38Z miwi261726 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvStartScheduleTable()
 *
 * Handles cross-core StartScheduleTable() calls.
 */
void OS_RecvStartScheduleTable(os_message_t * msg)
{
	os_scheduleid_t const s = (os_scheduleid_t)msg->parameter[0];
	if (OS_IsValidScheduleId(s) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];
		os_tick_t const offset			= (os_tick_t)msg->parameter[1];
		os_boolean_t const rel			= (os_boolean_t)msg->parameter[2];
		os_schedule_t const * const ss	= &OS_scheduleTableBase[s];
		os_alarm_t const * const as		= &OS_alarmTableBase[ss->alarm];

		replyParams[0] = (os_paramtype_t)OS_LocalStartScheduleTable(ss, as->counter, offset, rel);

#if OS_XC_REPLY_DIRECTLY
		OS_WriteMessageResult(msg, replyParams[0]);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnResult, msg->result, replyParams);
#endif
	}
	else
	{
		(void)OS_PANIC(OS_PANIC_CrosscoreCommunicationError);
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
