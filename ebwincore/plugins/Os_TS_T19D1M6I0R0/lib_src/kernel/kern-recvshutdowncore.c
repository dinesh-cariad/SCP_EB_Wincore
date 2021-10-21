/* kern-recshutdowncore.c
 *
 * This file contains the function OS_RecvShutdownCore().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvshutdowncore.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

/*!
 * OS_RecvShutdownCore
 *
 * Handles cross-core ShutdownCore() calls.
 */
void OS_RecvShutdownCore(os_message_t * msg)
{
	OS_Shutdown((os_result_t)msg->parameter[0], OS_TRUE);
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
