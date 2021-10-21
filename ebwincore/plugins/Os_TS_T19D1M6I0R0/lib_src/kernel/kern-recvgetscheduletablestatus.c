/* kern-recvgetscheduletablestatus.c
 *
 * This file contains the function OS_RecvGetScheduleTableStatus().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvgetscheduletablestatus.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvGetScheduleTableStatus
 *
 * Handles cross-core GetScheduleTableStatus() calls.
 */
void OS_RecvGetScheduleTableStatus(os_message_t * msg)
{
	os_scheduleid_t const s = (os_scheduleid_t)msg->parameter[0];
	if (OS_IsValidScheduleId(s) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];

		replyParams[0] = (os_paramtype_t)OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
		replyParams[1] = msg->parameter[1];		/* points to where the status shall be stored to */
		replyParams[2] = (os_paramtype_t)OS_scheduleTableBase[s].dynamic->status;

#if OS_XC_REPLY_DIRECTLY
		msg->parameter[0] = replyParams[0];
		msg->parameter[1] = replyParams[1];
		msg->parameter[2] = replyParams[2];
		OS_RetnScheduleTableStatus(msg);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnScheduleTableStatus, msg->result, replyParams);
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
