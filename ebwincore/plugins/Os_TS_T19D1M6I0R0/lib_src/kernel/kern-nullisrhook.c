/* kern-nullisrhook.c
 *
 * This file contains the OS_NullIsrHook function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-nullisrhook.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_NullIsrHook
 *
 * This function does nothing. It is used as a replacement for
 * PreIsrHook and PostIsrHook when they are not configured.
*/
void OS_NullIsrHook(os_isrid_t isr)
{
	OS_PARAM_UNUSED(isr);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
