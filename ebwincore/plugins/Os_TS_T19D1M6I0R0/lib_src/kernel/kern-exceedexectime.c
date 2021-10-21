/* kern-exceedexectime.c
 *
 * This file contains the OS_ExceedExecTime function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-exceedexectime.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#define OS_SID	OS_SID_TrapHandler
#define OS_SIF	OS_svc_TrapHandler

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ExceedExecTime
 *
 * This function is called when an execution-time limit is exceeded. That
 * can be as a result of the interrupt, or from the calculations in the
 * various exec-time functions.
 * It must determine whether the timeout came from
 * a task or interrupt context, and take action accordingly.
 *
 * If the accounting inFunction is anything other than TASK or CAT2,
 * we ignore the timeout. That's because this could happen when the timing
 * limit is very marginal; the timeout occurs just as the kernel is
 * stopping the timing, but the interrupt is only taken afterwards.
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime, 2
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Nontrusted, 1
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Trusted, 1
 *
 * OS_ERROR() in all these cases results in an call to the protection hook...
 *
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Exceed, 2
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.ExceedLock, 2
 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.ExceedResource, 2
*/
void OS_ExceedExecTime(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_uint8_t inFunctionSave;

	inFunctionSave = kernel_data->inFunction;
	kernel_data->inFunction = kernel_data->accounting.inFunction;
	kernel_data->accounting.inFunction = OS_INNOTHING;

	if ( kernel_data->inFunction == OS_INTASK )
	{
		kernel_data->accounting.taskTimingNesting = 1;

		switch ( kernel_data->accounting.etType )
		{
		case OS_ACC_RESLOCK:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_TaskResLockTimeExceeded, OS_NULL);
			break;
		case OS_ACC_INTLOCK:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_TaskIntLockTimeExceeded, OS_NULL);
			break;
		default:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_TaskTimeBudgetExceeded, OS_NULL);
			break;
		}

		kernel_data->accounting.taskTimingNesting = 0;
	}
	else
	if ( kernel_data->inFunction == OS_INCAT2 )
	{
		/* Warning: these might not return!
		*/
		switch ( kernel_data->accounting.etType )
		{
		case OS_ACC_RESLOCK:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_IsrResLockTimeExceeded, OS_NULL);
			break;
		case OS_ACC_INTLOCK:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_IsrIntLockTimeExceeded, OS_NULL);
			break;
		default:
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_IsrTimeBudgetExceeded, OS_NULL);
			break;
		}
	}
	else
	{
		/* MISRA */
	}

	kernel_data->inFunction = inFunctionSave;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
