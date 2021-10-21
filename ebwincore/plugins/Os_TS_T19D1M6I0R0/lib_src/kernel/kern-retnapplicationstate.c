/* kern-retnapplicationstate.c
 *
 * This file contains the function OS_RetnApplicationState().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retnapplicationstate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnApplicationState
 *
 * This function is used to return the result of GetApplicationState() to its original caller.
 */
void OS_RetnApplicationState(os_message_t * msg)
{
	os_errorresult_t const e = (os_errorresult_t)msg->parameter[0];

	if ( e == OS_ERROR_NoError )
	{
		os_appstate_t * const pstate = (os_appstate_t *)msg->parameter[1];
		*pstate = (os_appstate_t)msg->parameter[2];

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
