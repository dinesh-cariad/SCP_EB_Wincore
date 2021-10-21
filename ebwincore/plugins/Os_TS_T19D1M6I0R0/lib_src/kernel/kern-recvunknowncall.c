/* kern-recvunknowncall.c
 *
 * This file contains the function OS_RecvUnknownCall().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvunknowncall.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

/*!
 * OS_RecvUnknownCall()
 *
 * Handle unknown calls gracefully.
 */
void OS_RecvUnknownCall(os_message_t * msg)
{
	OS_PARAM_UNUSED(msg);
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
