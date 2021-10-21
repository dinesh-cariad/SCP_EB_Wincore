/* kern-appisaccessible.c
 *
 * This file contains the OS_AppIsAccessible() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-appisaccessible.c 24053 2016-04-25 13:17:22Z mist9353 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_AppIsAccessible
 *
 * Returns OS_TRUE if the specified application is accessible by the caller.
 *
 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationStates, 1
*/
os_boolean_t OS_AppIsAccessible(const os_appcontext_t *app)
{
	os_boolean_t result;

	if ( app == OS_NULL )
	{
		result = OS_TRUE;
	}
	else
	{
		switch ( app->dynamic->appstate )
		{
		case OS_APP_ACCESSIBLE:
			result = OS_TRUE;
			break;

		case OS_APP_RESTARTING:
			/* If this is a cross core request, OS_CurrentApp will return OS_NULL,
			 * because OS_CrossCoreNotifyIsr() sets the inFunction flag to OS_ININTERNAL.
			 * Therefore, an AppIsAccessible request from another core, will always return OS_FALSE,
			 * if 'app' is the current application and restarting - which is the correct behavior.
			*/
			if ( app == OS_CurrentApp() )
			{
				result = OS_TRUE;
			}
			else
			{
				result = OS_FALSE;
			}
			break;

		case OS_APP_QUARANTINED:
		default:
			result = OS_FALSE;
			break;
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
