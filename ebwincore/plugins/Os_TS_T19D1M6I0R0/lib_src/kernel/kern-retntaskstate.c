/* kern-retntaskstate.c
 *
 * This file contains the implementation of the function OS_RetnTaskState().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retntaskstate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnTaskState()
 *
 * It is run by the core that sent the GetTaskState() call to a remote core,
 * because the task of interest runs there. Its purpose is to return the task state
 * (and the result code) to its caller.
 * All this is triggered, because the remote core sent a reply message containing the
 * required information.
 */
void OS_RetnTaskState(os_message_t * msg)
{
	/* Place the task's state (msg->parameter[2]) in desired location (msg->parameter[1]),
	 * if error code indicates no error.
	 * Note, OS_KernGetTaskState() already sanitized the output pointer in msg->parameter[1].
	*/
	os_errorresult_t const e = (os_errorresult_t)msg->parameter[0];
	if (e == OS_ERROR_NoError)
	{
		os_taskstate_t * const pstate = (os_taskstate_t *)msg->parameter[1];
		*pstate = (os_taskstate_t)msg->parameter[2];

#if OS_XC_REPLY_DIRECTLY
		OS_CacheFlush(pstate, sizeof(*pstate));
#endif
	}

	*(msg->result) = (os_paramtype_t)e;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheFlush(msg->result, sizeof(os_paramtype_t));
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
