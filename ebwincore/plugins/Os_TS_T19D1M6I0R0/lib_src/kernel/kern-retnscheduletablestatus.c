/* kern-retnscheduletablestatus.c
 *
 * This file contains the function OS_RetnScheduleTableStatus().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retnscheduletablestatus.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnScheduleTableStatus
 *
 * This function returns the result of a cross-core GetScheduleTableStatus() call to its original caller.
 */
void OS_RetnScheduleTableStatus(os_message_t * msg)
{
	/* Place expected answer in msg->parameter[2] in the desired location msg->parameter[1],
	 * if error code in msg->parameter[0] indicates no error.
	*/
	os_errorresult_t const e = (os_errorresult_t)msg->parameter[0];
	if (e == OS_ERROR_NoError)
	{
		os_schedulestatus_t * const pstatus = (os_schedulestatus_t *)msg->parameter[1];
		*pstatus = (os_schedulestatus_t)msg->parameter[2];

#if OS_XC_REPLY_DIRECTLY
		OS_CacheFlush(pstatus, sizeof(*pstatus));
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
