/* kern-initexecutiontiming.c
 *
 * This file contains the OS_InitExecutionTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initexecutiontiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_InitExecutionTiming
 *
 * This function initialises the kernel's execution-time monitoring feature.
 * The timer used is initialised using the architecture-dependent function.
*/
void OS_InitExecutionTiming(void)
{
	OS_GetKernelData()->accounting.inFunction = OS_INNOTHING;
	OS_InitExecTimer();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
