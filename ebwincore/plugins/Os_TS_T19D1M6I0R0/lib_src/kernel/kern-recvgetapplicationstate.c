/* kern-recvgetapplicationstate.c
 *
 * This file contains the function OS_RecvGetApplicationState().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvgetapplicationstate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvGetApplicationState
 *
 * Handles cross-core GetApplicationState() calls.
 */
void OS_RecvGetApplicationState(os_message_t * msg)
{
	os_applicationid_t const aid = (os_applicationid_t)msg->parameter[0];

	if (OS_IsValidApplicationId(aid) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];

		replyParams[0] = (os_paramtype_t)OS_ERROR_NoError;
		replyParams[1] = msg->parameter[1];
		replyParams[2] = (os_paramtype_t)OS_appTableBase[aid].dynamic->appstate;

#if OS_XC_REPLY_DIRECTLY
		msg->parameter[0] = replyParams[0];
		msg->parameter[1] = replyParams[1];
		msg->parameter[2] = replyParams[2];
		OS_RetnApplicationState(msg);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnApplicationState, msg->result, replyParams);
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
