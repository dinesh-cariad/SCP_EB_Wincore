/* WINLIN-taskreturn.c
 *
 * This file contains the definition of OS_WINLINTaskReturn().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-taskreturn.c 24195 2016-05-19 07:48:20Z vaka852 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Cleans up after a task when it returns w/o calling TerminateTask().
 *
 * The intention is to clean up everything in this case. Most of this work
 * is already taken care of in OS_KernTaskReturn(). The current interrupt
 * disable level, though, must be explicitly reset.
 */
void OS_WINLINTaskReturn(void)
{
	(void)OS_IntEnable();
	OS_KernTaskReturn();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
