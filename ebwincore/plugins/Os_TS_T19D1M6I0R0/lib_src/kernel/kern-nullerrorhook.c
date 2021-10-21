/* kern-nullerrorhook.c
 *
 * This file contains the OS_NullErrorHook function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-nullerrorhook.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_NullErrorHook
 *
 * This function does nothing. It is used as a replacement for ErrorHook
 * and ShutdownHook when the configuration specifies no hook.
*/
void OS_NullErrorHook(os_result_t code)
{
	OS_PARAM_UNUSED(code);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
