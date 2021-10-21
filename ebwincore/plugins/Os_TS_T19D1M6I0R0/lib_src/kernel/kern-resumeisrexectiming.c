/* kern-resumeisrexectiming.c
 *
 * This file contains the OS_ResumeIsrExecTiming function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-resumeisrexectiming.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ResumeIsrExecTiming
 *
 * This function is called from the category 2 interrupt entry routine.
 * It restarts any saved ISR execution timing, unless the limit has been reached.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.ISR, 1
*/
void OS_ResumeIsrExecTiming(os_accountingsave_t *save)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	kernel_data->accounting.inFunction = save->inFunction;
	kernel_data->accounting.etType = save->etType;
	kernel_data->accounting.etUsed = save->etUsed;
	kernel_data->accounting.etLimit = save->etLimit;

	if ((kernel_data->accounting.etLimit != 0) && (kernel_data->accounting.etUsed >= kernel_data->accounting.etLimit))
	{
		OS_ExceedExecTime();
	}
	else
	{
		kernel_data->accounting.frc = OS_ReadExecTimer();
		if (kernel_data->accounting.etLimit != 0)
		{
			OS_SetExecTimingInterrupt(kernel_data->accounting.frc,
					(kernel_data->accounting.etLimit - kernel_data->accounting.etUsed));
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
