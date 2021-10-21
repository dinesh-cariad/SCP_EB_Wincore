/* kern-recvterminateapplication.c
 *
 * This file contains the function OS_RecvTerminateApplication().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvterminateapplication.c 27881 2017-11-15 11:42:38Z miwi261726 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvTerminateApplication
 *
 * Handles cross-core TerminateApplication() calls.
 */
void OS_RecvTerminateApplication(os_message_t * msg)
{
	os_applicationid_t const appId = (os_applicationid_t)msg->parameter[0];

	if (OS_IsValidApplicationId(appId) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];
		os_appcontext_t const * const app = &OS_appTableBase[appId];
		os_restart_t const rst = (os_restart_t)msg->parameter[1];

		/* ASCOS-2659 */
		replyParams[0] = (os_paramtype_t)OS_LocalTerminateApplication(app, rst);

#if OS_XC_REPLY_DIRECTLY
		OS_WriteMessageResult(msg, replyParams[0]);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnResult, msg->result, replyParams);
#endif
		/* ASCOS-2659 */
	}
	else
	{
		(void)OS_PANIC(OS_PANIC_CrosscoreCommunicationError);
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
