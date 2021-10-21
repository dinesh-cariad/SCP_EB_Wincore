/* kern-sendgettaskstate.c
 *
 * This file contains the OS_SendGetTaskState() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendgettaskstate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendGetTaskState()
 *
 * Sends the GetTaskState() service request for task 'tid' to the respective core.
*/
os_errorresult_t OS_SendGetTaskState(os_taskid_t tid, const os_task_t *tp, os_taskstate_t *out)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)tid;
	msgParam[1] = (os_paramtype_t)out;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheInvalidate(out, sizeof(os_taskstate_t));
#endif

	return OS_SendMessageWait(OS_GetTaskCoreId(tp), OS_XC_OP_GetTaskState, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
