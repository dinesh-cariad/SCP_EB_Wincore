/* WINLINS-panic.c
 *
 * This file contains the panic switch.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-panic.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <stdlib.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Panics.
 *
 * To panic means to terminate the whole application by calling abort().
 */
void OS_WINLINPanic(void)
{
	abort();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
