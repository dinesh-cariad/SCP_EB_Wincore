/* kern-recvsetevent.c
 *
 * This file contains the function OS_RecvSetEvent().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvsetevent.c 27881 2017-11-15 11:42:38Z miwi261726 $
*/

#include <Os_kernel.h>
#include <Os_panic.h>
#include <Os_messagequeue.h>

/*!
 * OS_RecvSetEvent()
 *
 * Handles cross-core SetEvent() calls.
 */
void OS_RecvSetEvent(os_message_t * msg)
{
	os_taskid_t const tid = (os_taskid_t)msg->parameter[0];
	if (OS_IsValidTaskId(tid) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];

		replyParams[0] = (os_paramtype_t)OS_LocalSetEvent(&OS_taskTableBase[tid], (os_eventmask_t)msg->parameter[1]);

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
