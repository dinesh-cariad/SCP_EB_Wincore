/* WINLIN-arethereanypendingirqstoserve.c
 *
 * This file contains the implementation of OS_WINLINAreThereAnyPendingIrqsToServe().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-arethereanypendingirqstoserve.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_configuration.h>
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Checks, if there are any pending ISR to serve.
 *
 * This function checks, if there are currently other interrupts
 * requested right now. \e Requested means, that those interrupts have not yet
 * been served by OS_WINLINServeIrq(). It is important to understand, that
 * even when this function returns OS_FALSE, there may be other lower-priority
 * interrupts or ISRs active, but those have been interrupted by higher-priority ones.
 *
 * \param considerAllIsrs	A switch to control, whether the current interrupt disable
 *							level is regarded to pick out the ISR events to check.
 *							If OS_FALSE is passed, only ISR events with a higher priority
 *							than the current interrupt disable level are checked, if any
 *							of them has been raised. If OS_TRUE is passed, \e all ISR events
 *							are checked.
 *
 * \retval OS_FALSE There are no interrupts requested right now. Though, there might
 *					be some active, i.e. their ISRs were started but interrupted by
 *					other higher-priority ISRs.
 * \retval OS_TRUE	There is at least one interrupt requested, which is \e enabled
 *					and not yet served. This means, when the core thread restarts,
 *					this interrupt will be served.
 */
os_boolean_t OS_WINLINAreThereAnyPendingIrqsToServe(os_boolean_t considerAllIsrs)
{
	os_boolean_t isrsPending = OS_FALSE;
#if (OS_NINTERRUPTS != 0)
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_winlinHandle_t isrEvents[OS_NINTERRUPTS];
	os_isr_t const* associatedIsrs[OS_NINTERRUPTS];
	os_size_t const numEvents = OS_WINLINAssembleIsrEvents(
			isrEvents,
			associatedIsrs,
			OS_NINTERRUPTS,
			considerAllIsrs);
	os_boolean_t const osInterruptsSuspended =
			(kernel_data->nestSuspendOs
			 + (OS_GetKernelData()->taskCurrent != OS_NULL
			    ? OS_GetKernelData()->taskCurrent->dynamic->nestSuspendOs
				: 0U)) != 0;
	os_boolean_t const allInterruptsSuspended =
			(kernel_data->nestSuspendAll
			 + (OS_GetKernelData()->taskCurrent != OS_NULL
				? OS_GetKernelData()->taskCurrent->dynamic->nestSuspendAll
				: 0U)) != 0;

	{
		os_size_t i;
		for (i = 0; i < numEvents; ++i)
		{
			if (OS_WINLIN_OSAL_IsEventSet(isrEvents[i]))
			{
				isrsPending = OS_WINLINIsIsrEnabled(associatedIsrs[i]->dynamic);
				if (isrsPending)
				{
					if ((((associatedIsrs[i]->flags & OS_ISRF_CAT) == OS_ISRF_CAT2)
						 && osInterruptsSuspended)
						|| allInterruptsSuspended)
					{
						isrsPending = OS_FALSE;
					}
				}
				if (isrsPending)
				{
					break;
				}
			}
		}
	}
#else
	OS_PARAM_UNUSED(considerAllIsrs);
#endif	/* OS_NINTERRUPTS != 0 */

	return isrsPending;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
