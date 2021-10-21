/* kern-recvdisableinterruptsource.c
 *
 * This file contains the function OS_RecvDisableInterruptSource().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvdisableinterruptsource.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#define OS_SID OS_SID_DisableInterruptSource
#define OS_SIF OS_svc_DisableInterruptSource

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvDisableInterruptSource
 *
 * Handles cross-core DisableInterruptSource() calls.
 */
void OS_RecvDisableInterruptSource(os_message_t * msg)
{
	os_isrid_t const isrId = (os_isrid_t)msg->parameter[0];
	if (OS_IsValidIsrId(isrId) && (msg->result != OS_NULL))
	{
		os_paramtype_t replyParams[OS_MAXPARAM];

		os_isr_t const * const isr = &OS_isrTableBase[isrId];
		if ( !OS_APPISACCESSIBLE(isr->app) )
		{
			replyParams[0] = (os_paramtype_t)OS_ERRORCODE_NOCHECK(OS_ERROR_ApplicationNotAccessible);
		}
		else
		{
			OS_DisableIsr(isr);
			replyParams[0] = (os_paramtype_t)OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
		}

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
