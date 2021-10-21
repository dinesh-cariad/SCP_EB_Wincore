/* kern-sendgetevent.c
 *
 * This file contains the OS_SendGetEvent() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendgetevent.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendGetEvent
 *
 * Sends the GetEvent() request to the core on which the task 't' resides.
*/
os_errorresult_t OS_SendGetEvent(os_taskid_t t, const os_task_t *tp, os_eventmask_t *ep)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = t;
	msgParam[1] = (os_paramtype_t)ep;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheInvalidate(ep, sizeof(*ep));
#endif

	return OS_SendMessageWait(OS_GetTaskCoreId(tp), OS_XC_OP_GetEvent, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
