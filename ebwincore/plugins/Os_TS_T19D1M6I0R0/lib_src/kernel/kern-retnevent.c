/* kern-retnevent.c
 *
 * This file contains the function OS_RetnEvent().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retnevent.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnEvent()
 *
 * This function completes a GetEvent() service request, which was sent to another core, because the
 * queried event is assigned to that core. It unpacks the reply from the other core and returns it
 * to the original caller of GetEvent().
 */
void OS_RetnEvent(os_message_t * msg)
{
	/* Store the pending events in msg->parameter[2] into the location msg->parameter[1] only if
	 * the operation completed successfully, as indicated by the error code in msg->parameter[0].
	 */
	os_errorresult_t const e = (os_errorresult_t)msg->parameter[0];
	if (e == OS_ERROR_NoError)
	{
		os_eventmask_t * const pevents = (os_eventmask_t *)msg->parameter[1];
		*pevents = (os_eventmask_t)msg->parameter[2];

#if OS_XC_REPLY_DIRECTLY
		OS_CacheFlush(pevents, sizeof(*pevents));
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
