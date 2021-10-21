/* kern-multiterminateapplication.c
 *
 * This file contains the OS_MultiTerminateApplication function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-multiterminateapplication.c 27818 2017-11-08 16:44:06Z mist9353 $
*/

#include <Os_kernel.h>

#if (OS_N_CORES == 1)
#error "This file is only required for multi-core systems - please check your makefiles."
#endif
#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "OS applications are implemented by the microkernel - please check your makefiles."
#endif

#include <Os_messagequeue.h>


#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_MultiTerminateApplication terminates an application in a multi-core system.
*/
os_errorresult_t OS_MultiTerminateApplication(
	os_applicationid_t appid,
	const os_appcontext_t *app,
	os_restart_t restart)
{
	os_errorresult_t e;

	if ( OS_GetAppCoreId(app) == OS_GetMyCoreId() )
	{
		e = OS_LocalTerminateApplication(app, restart);
	}
	else
	{
		e = OS_SendTerminateApplication(appid, app, restart);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
