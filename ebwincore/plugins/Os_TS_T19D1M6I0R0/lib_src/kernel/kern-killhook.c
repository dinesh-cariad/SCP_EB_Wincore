/* kern-killhook.c
 *
 * This file contains OS_KillHook
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-killhook.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KillHook
 *
 * The hook is killed by calling the architecture specific function
 * or macro OS_KillCalledContext() with the hook's saved
 * context as parameter.
*/
os_result_t OS_KillHook(os_hookcontext_t *hc)
{
	return OS_KillCalledContext(&hc->c);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
