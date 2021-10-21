/* lib-getcoreid.c
 *
 * This file contains the OS_GetCoreID function.
 * It returns the logical ID of the current core.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-getcoreid.c 30633 2019-08-12 14:02:48Z ache271795 $
*/

#include <Os_kernel.h>
#include <Os_api.h>
#include <Os_alci.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetCoreId implements the API GetCoreID
 *
 * It returns the logical ID of the current core.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetCoreID, 1
 * Look! Can be called at any time!
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetCoreID.BeforeStartOS, 1
*/
os_coreid_t OS_GetCoreId(void)
{
	return (os_coreid_t)OS_GetLogicalCoreId(OS_ArchGetCoreId());
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

