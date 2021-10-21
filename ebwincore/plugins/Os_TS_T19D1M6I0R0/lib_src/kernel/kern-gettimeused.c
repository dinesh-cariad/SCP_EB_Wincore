/* kern-gettimeused.c
 *
 * This file contains the OS_GetTimeUsed function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-gettimeused.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetTimeUsed
 *
 * This function returns the amount of time used since it was last called.
 * It uses the global accounting structure, so it guarantees that the
 * global frc location holds the time of the last call and can therefore
 * be used as the baseline when setting the execution-timer interrupt.
*/
os_tick_t OS_GetTimeUsed(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_timervalue_t frc = OS_ReadExecTimer();
	os_tick_t used = (os_tick_t)OS_ExecTimerSub(frc, kernel_data->accounting.frc);
	kernel_data->accounting.frc = frc;

	return used;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
