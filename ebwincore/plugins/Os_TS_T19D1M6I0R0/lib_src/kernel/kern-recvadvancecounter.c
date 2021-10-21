/* kern-recvadvancecounter.c
 *
 * This file contains the function OS_RecvAdvanceCounter().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvadvancecounter.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvAdvanceCounter
 *
 * Handles cross-core AdvanceCounter() requests. These are initiated by AUTOSAR alarms
 * or IncrementCounter() system calls.
 */
void OS_RecvAdvanceCounter(os_message_t * msg)
{
	os_counterid_t const c = (os_counterid_t)msg->parameter[0];

	if (OS_IsValidCounterId(c) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];
		os_counter_t const * const cs = &OS_counterTableBase[c];
		os_counterdynamic_t * const cd = cs->dynamic;
		os_tick_t const incr = (os_tick_t) msg->parameter[1];
		os_intstatus_t is;

		is = OS_IntDisable();
		(void) OS_AdvanceCounter(cs, cd, incr, is);
		OS_IntRestore(is);

		replyParams[0] = OS_ERROR_NoError;

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
