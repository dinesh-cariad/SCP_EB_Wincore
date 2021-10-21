/* kern-nullfunction.c
 *
 * This file contains the OS_NullFunction function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-nullfunction.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_NullFunction
 *
 * This function does nothing. It is used wherever a dummy function call
 * is required, in particular for OS_NULL entries in the initialisation
 * functions table, to avoid having to test for a OS_NULL pointer.
*/
void OS_NullFunction(void)
{
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
