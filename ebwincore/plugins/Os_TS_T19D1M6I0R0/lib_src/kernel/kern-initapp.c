/* kern-initapp.c
 *
 * This file contains the OS_InitApp() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initapp.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison is always true.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitApp()
 *
 * This function initialises the state of every OS-application in the system
*/
void OS_InitApp(void)
{
	os_applicationid_t a;
	os_coreid_t const myCoreId = OS_GetMyCoreId();

	for ( a = 0; a < OS_nApps; a++ )
	{
		const os_appcontext_t *ap = &OS_appTableBase[a];

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if ( OS_GetAppCoreId(ap) == myCoreId )
		{
			os_appdynamic_t *ad = ap->dynamic;
			/* !LINKSTO Kernel.Autosar.OsApplication.ApplicationStates.Startup, 1
			*/
			ad->appstate = OS_APP_ACCESSIBLE;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
