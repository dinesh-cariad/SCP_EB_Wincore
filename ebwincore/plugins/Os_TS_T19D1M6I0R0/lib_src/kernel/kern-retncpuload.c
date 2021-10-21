/* kern-retncpuload.c
 *
 * This file contains the function OS_RetnCpuLoad().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retncpuload.c 27282 2017-08-24 13:57:43Z olme8414 $
 */

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnCpuLoad
 *
 * This function is used to return the result of GetCpuLoad() to its original caller.
 */
void OS_RetnCpuLoad(os_message_t * msg)
{
	os_errorresult_t const e = (os_errorresult_t)msg->parameter[0];

	if ( e == OS_ERROR_NoError )
	{
		os_uint8_t * const pLoad = (os_uint8_t *)msg->parameter[1];
		*pLoad = (os_uint8_t)msg->parameter[2];

#if OS_XC_REPLY_DIRECTLY
		OS_CacheFlush(pLoad, sizeof(*pLoad));
#endif
	}

	*(msg->result) = (os_paramtype_t)e;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheFlush(msg->result, sizeof(os_paramtype_t));
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
