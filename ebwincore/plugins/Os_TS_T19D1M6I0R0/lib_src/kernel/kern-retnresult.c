/* kern-retnresult.c
 *
 * This file contains the function OS_RetnResult().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-retnresult.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_RetnResult()
 *
 * This function is used to return the result (e.g. error code) of a cross-core call.
 * Note, this function is always executed on the core, which originally initiated the
 * cross-core system call. Hence, no cache management operations are necessary.
 */
void OS_RetnResult(os_message_t * msg)
{
	*(msg->result) = (os_paramtype_t)msg->parameter[0];
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
