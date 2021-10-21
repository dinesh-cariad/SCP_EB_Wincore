/* kern-recvstopscheduletable.c
 *
 * This file contains the function OS_RecvStopScheduleTable().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvstopscheduletable.c 27881 2017-11-15 11:42:38Z miwi261726 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvStopScheduleTable
 *
 * Handles cross-core StopScheduleTable() calls.
 */
void OS_RecvStopScheduleTable(os_message_t * msg)
{
	os_scheduleid_t const s = (os_scheduleid_t)msg->parameter[0];
	if (OS_IsValidScheduleId(s) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];
		os_schedule_t const * const ss = &OS_scheduleTableBase[s];

		replyParams[0] = (os_paramtype_t)OS_LocalStopScheduleTable(s, ss);

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
