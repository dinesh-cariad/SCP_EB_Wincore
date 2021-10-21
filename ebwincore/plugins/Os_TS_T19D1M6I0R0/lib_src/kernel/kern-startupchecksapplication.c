/* kern-startupchecksapplication.c
 *
 * This file contains the OS_StartupChecksApplication function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecksapplication.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

os_result_t OS_StartupChecksApplication(void)
{
	os_result_t result = OS_E_OK;

	if (OS_GetMyCoreId() == (os_coreid_t)OS_INITCORE)
	{
		os_unsigned_t i;
		os_permission_t permBits = 0;

		for ( i = 0; i < OS_nApps; i++ )
		{
			os_appcontext_t const* const app = &OS_appTableBase[i];

			if (  app->appId != i )
			{
				result = OS_PANIC(OS_PANIC_SCHK_AppIdIsNotIndexInAppTable);
			}

			if (  *app->buildId != OS_configurationId )
			{
				result = OS_PANIC(OS_PANIC_SCHK_AppConfigurationIdMismatch);
			}

			if ( app->restartTask != OS_NULLTASK )
			{
				if ( OS_GET_APP(OS_taskTableBase[app->restartTask].app) != app )
				{
					result = OS_PANIC(OS_PANIC_SCHK_AppRestartTaskNotMemberOfApp);
				}
			}

			if ( app->permissionBit == 0 )
			{
				result = OS_PANIC(OS_PANIC_SCHK_AppHasNoPermissions);
			}

			/* System applications have all bits set in their permissionBit field, because they
			 * may access any object. Hence, their permission identification value, i.e. 'permissionBit',
			 * is not unique.
			 */
			if ( !OS_AppIsSystemApp(app) )
			{
				if ( (permBits & app->permissionBit) != 0 )
				{
					result = OS_PANIC(OS_PANIC_SCHK_AppPermissionBitNotUnique);
				}

				permBits |= app->permissionBit;
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
