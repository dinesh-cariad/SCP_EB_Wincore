/* kern-startupchecksresource.c
 *
 * This file contains the OS_StartupChecksResource function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecksresource.c 27842 2017-11-10 12:18:30Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

os_result_t OS_StartupChecksResource(void)
{
	os_result_t result = OS_E_OK;
	os_resourceid_t i;
	const os_coreid_t myCoreId = OS_GetMyCoreId();

	for (i = 0; i < OS_nResources; i++)
	{
		const os_resource_t * const resource = &OS_resourceTableBase[i];
		const os_appcontext_t * const app = OS_GET_APP(resource->app);

		if ( OS_AppIsNull(app) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_NoOwningApplication);
		}

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetResourceCoreId(resource) == myCoreId)
		{
			const os_permission_t perms = OS_GET_APP(resource->permissions);

			if ( !OS_HasPermission(app, perms) )
			{
				result = OS_PANIC(OS_PANIC_SCHK_OwningApplicationHasNoPermission);
			}
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
