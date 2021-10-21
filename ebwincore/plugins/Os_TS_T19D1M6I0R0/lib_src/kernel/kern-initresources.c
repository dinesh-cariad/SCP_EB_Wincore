/* kern-initresources.c
 *
 * This file contains the OS_InitResources function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initresources.c 26577 2017-04-27 14:02:09Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is never used.
 *
 * Reason: Not an issue, variable will be used if there are at least two cores.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_InitResources
 *
 * The dynamic data for each resource needs initializing because the default
 * bss value of 0 has an unwanted meaning in the kernel.
*/
void OS_InitResources(void)
{
	os_unsigned_t i;
	os_coreid_t const myCore = OS_GetMyCoreId();

	for (i = 0; i < OS_nResources; ++i)
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		os_resource_t const * const rs = &OS_resourceTableBase[i];
		/* Possible diagnostic TOOLDIAG-2 <1> */
		if (OS_GetResourceCoreId(rs) == myCore)
		{
			os_resourcedynamic_t * const rd = OS_ResourceGetDynamic(i);
			rd->lastPrio = 0;
			rd->takenBy = OS_NULLTASK;
			rd->next = OS_NULLOBJECT;
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
