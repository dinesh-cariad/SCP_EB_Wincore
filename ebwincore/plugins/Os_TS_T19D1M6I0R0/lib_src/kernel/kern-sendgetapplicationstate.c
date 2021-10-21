/* kern-sendgetapplicationstate.c
 *
 * This file contains the OS_SendGetApplicationState() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendgetapplicationstate.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendGetApplicationState
 *
 * Sends the GetApplicationState() request to the core on which the application 'a' resides.
*/
os_errorresult_t OS_SendGetApplicationState(os_applicationid_t a, const os_appcontext_t *ap, os_appstate_t *out)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)a;
	msgParam[1] = (os_paramtype_t)out;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheInvalidate(out, sizeof(*out));
#endif

	return OS_SendMessageWait(OS_GetAppCoreId(ap), OS_XC_OP_GetApplicationState, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
