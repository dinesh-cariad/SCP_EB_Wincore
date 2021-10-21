/* kern-sendterminateapplication.c
 *
 * This file contains the OS_SendTerminateApplication() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendterminateapplication.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendTerminateApplication
 *
 * Sends TerminateApplication() requests to the core on which 'app' resides.
*/
os_errorresult_t OS_SendTerminateApplication
(	os_applicationid_t appId,
	const os_appcontext_t *app,
	os_restart_t restart
)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)appId;
	msgParam[1] = (os_paramtype_t)restart;

	return OS_SendMessageWait(OS_GetAppCoreId(app), OS_XC_OP_TerminateApplication, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
