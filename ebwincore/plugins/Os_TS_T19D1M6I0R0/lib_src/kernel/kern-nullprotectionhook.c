/* kern-nullprotectionhook.c
 *
 * This file contains the OS_NullProtectionHook function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-nullprotectionhook.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_NullProtectionHook
 *
 * This function does nothing but return OS_ACTION_SHUTDOWN (==PRO_SHUTDOWN).
 * The function is used as the protection hook when no user protection hook
 * is configured and strict Autosar * conformance is required.
 *
 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.NoHook, 2
*/
os_erroraction_t OS_NullProtectionHook(os_result_t err)
{
	OS_PARAM_UNUSED(err);
	return OS_ACTION_SHUTDOWN;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
