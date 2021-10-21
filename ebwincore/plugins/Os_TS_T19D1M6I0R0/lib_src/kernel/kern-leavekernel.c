/* kern-leavekernel.c
 *
 * This file contains the OS_LeaveKernel function for user-mode kernels.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-leavekernel.c 27546 2017-09-25 09:10:06Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason:
 * This is a false positive, because the criticized functions do relevant work.
*/

#define OS_SID	OS_SID_Dispatch
#define OS_SIF	OS_svc_Dispatch

#include <Os_kernel.h>
#include <Os_panic.h>
#include <Os_taskqueue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LeaveKernel
 *
 * This function is called to leave the kernel. It is only used in
 * user-mode kernels (when no system-call interface is used).
 *
 * The inKernel counter is tested. If it is 1, a task-switch is tested for and
 * performed if necessary. If inKernel is not 1, it is decremented.
*/
os_unsigned_t OS_LeaveKernel(os_unsigned_t result)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_ExtraCheck(kernel_data->inKernel == 0) )
	{
		/* Serious kernel error
		*/
		/* can't propagate the return value of OS_PANIC -> ignore it */
		(void) OS_PANIC(OS_PANIC_IncorrectKernelNesting);
	}
	else
	if ( kernel_data->inKernel <= 1 )
	{
		os_intstatus_t is;

		if ( OS_KernelStackOverflow() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_KernelStackOverflow, OS_NULL);
		}

		/* Leaving the kernel. The testing for a task switch must be done with interrupts disabled.
		*/
		is = OS_IntDisableConditional();

		if ( kernel_data->taskQueueHead == kernel_data->taskCurrent )
		{
			if ( kernel_data->taskCurrent == OS_NULL )
			{
				/* Nothing to do - will end in idle task
				*/
				OS_FreeCurrentContext();
				OS_Dispatch(OS_TS_READY_SYNC);		/* Never returns */
			}
			else
			{
				/* Three possibilities for current task's state here:
				 *  RUNNING - no task switch, just return to caller
				 *  READY_SYNC - task waited for an event which was set in an ISR
				 *  NEW - task has chained self, or terminated with 2nd activation queued, or been killed & reactivated
				*/
				if ( kernel_data->taskCurrent->dynamic->state == OS_TS_RUNNING )
				{
					/* No task switch - return to caller.
					 * But first, set the task's running priority if necessary.
					*/
					if ( kernel_data->taskCurrent->dynamic->prio < kernel_data->taskCurrent->runPrio )
					{
						kernel_data->taskCurrent->dynamic->prio = kernel_data->taskCurrent->runPrio;
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
						OS_InsertTask(kernel_data->taskCurrent, kernel_data->taskCurrent->runPrio);
#endif
					}

				}
				else if ( kernel_data->taskCurrent->dynamic->state > OS_TS_MAX_TERMINATING )
				{
					/* ASCOS-2610 */
					/* Deviation MISRAC2012-1 <1> */
					OS_SaveTaskContextSync(&kernel_data->taskCurrent->dynamic->context);
				}
				else
				{
					/* Current task is terminating and restarting
					*/
					OS_FreeCurrentContext();
					OS_StoreTaskSp();					/* For stack check */
					OS_Dispatch(OS_TS_READY_SYNC);		/* Never returns */
				}
			}
		}
		else
		{
			if ( ( kernel_data->taskCurrent == OS_NULL )
					|| ( kernel_data->taskCurrent->dynamic->state <= OS_TS_MAX_TERMINATING ) )
			{
				OS_FreeCurrentContext();
				if ( kernel_data->taskCurrent != OS_NULL )
				{
					OS_StoreTaskSp();					/* For stack check */
				}
				OS_Dispatch(OS_TS_READY_SYNC);		/* Never returns */
			}
			else
			{
				/* ASCOS-2610 */
				/* Deviation MISRAC2012-1 <1> */
				OS_SaveTaskContextSync(&kernel_data->taskCurrent->dynamic->context);
			}
		}

		kernel_data->inKernel = 0;
		OS_IntRestore(is);
	}
	else
	{
		kernel_data->inKernel--;
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
