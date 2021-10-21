/* kern-dogetapplicationid.c
 *
 * This file contains the OS_KernGetApplicationId function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dogetapplicationid.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_DoGetApplicationId gets the "id" of the currently running application.
 *
 * The id of the current application (or OS_NULLAPP if none) is
 * returned.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetApplicationID, 2
*/
os_applicationid_t OS_DoGetApplicationId(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_uint8_t inFunction;
	os_applicationid_t a = OS_NULLAPP;
	const os_appcontext_t *app = OS_NULL;

	{
		/* Select the calling context. For error and protection
		 * hooks we need the parent context.
		*/
		if ( (kernel_data->inFunction == OS_INERRORHOOK) || (kernel_data->inFunction == OS_INPROTECTIONHOOK) )
		{
			inFunction = kernel_data->errorStatus.calledFrom;
		}
		else
		{
			inFunction = kernel_data->inFunction;
		}

		switch (inFunction)
		{
		case OS_INTASK:
		case OS_INPRETASKHOOK:
		case OS_INPOSTTASKHOOK:
			/* In these cases, the current app is the current task's
			 * application.
			*/
			if ( kernel_data->taskCurrent != OS_NULL )
			{
				app = OS_GET_APP(kernel_data->taskCurrent->app);
			}
			break;

		case OS_INCAT1:
		case OS_INCAT2:
		case OS_INPREISRHOOK:
		case OS_INPOSTISRHOOK:
			/* In these cases, the current app is the current ISR's
			 * application.
			*/
			if ( kernel_data->isrCurrent < OS_nInterrupts )
			{
				app = OS_GET_APP(OS_isrTableBase[kernel_data->isrCurrent].app);
			}
			break;

		case OS_INERRORHOOK:
		case OS_INSTARTUPHOOK:
		case OS_INSHUTDOWNHOOK:
			/* In these cases, it might be an application-specific hook.
			*/
			app = kernel_data->hookApp;
			break;

		default:
			/* In all other cases (BOOT, INTERNAL, ACB) there's no application,
			 * so we just fall through leaving app at OS_NULL.
			*/
			break;
		}

		/* Now we look up the app's ID
		*/
		if ( !OS_AppIsNull(app) )
		{
			a = app->appId;
		}
	}

	return a;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
