/* kern-startupchecksTrustedFunction.c
 *
 * This file contains the OS_StartupChecksTrustedFunction function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupcheckstrustedfunction.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

os_result_t OS_StartupChecksTrustedFunction(void)
{
	os_result_t result = OS_E_OK;

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
