/* WINLIN-triggerinterrupt.c
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-triggerinterrupt.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_configuration.h>
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#endif


/** \brief Triggers an interrupt.
 *
 * External software can use this function to trigger interrupts.
 * The interrupt is identified by its ISR ID, which is its index
 * in the OS_isrTable[] array in Os_configuration.c. This index
 * is defined by the OS generator and available by including the
 * generated header file OS_user.h.
 *
 * There are no restrictions regarding concurrency. This function may be
 * called concurrently by different threads. <em>External threads</em>,
 * which are all threads not running any AUTOSAR software, are not blocked
 * by this function. They return immediately after the interrupt was
 * requested from the AUTOSAR OS. This means, that the respective ISR must
 * not run yet, when this function returns.
 *
 * On the other hand, AUTOSAR task and ISR threads are blocked until the
 * interrupt request has been \e accepted by AUTOSAR OS. Acceptance means,
 * that AUTOSAR OS had a chance to determine, if the triggering task or
 * ISR must be suspended/interrupted by the triggered interrupt. If this
 * is the case, it will be resumed after the ISR has finished. Otherwise,
 * it is allowed to continue immediately.
 *
 * \par Example
 * The following example triggers an interrupt from a task.
 * \code{.c}
 * ISR(MyIsr)
 * {
 *		[... some code ...]
 * }
 *
 * TASK(MyTask)
 * {
 *		[... some code ...]
 *		OS_WINLINTriggerInterrupt(MyIsr);
 *		[... some other code ...]
 * }
 * \endcode
 *
 * \remarks This function may be called by non-AUTOSAR threads.
 *
 * \attention This function must only be called after the OS was started
 * on the core to which the ISR with ID \a isrId had been assigned.
 *
 * \param[in] isrId		The ISR ID of the interrupt to be tiggered.
 */
void OS_WINLINTriggerInterrupt(os_isrid_t isrId)
{
#if (OS_NINTERRUPTS != 0)
	if (isrId < OS_NINTERRUPTS) /* then, isrId is valid ... */
	{
		os_isr_t const * const pIsr = &OS_isrTableBase[isrId];
		os_isrdynamic_t* const pIsrDyn = pIsr->dynamic;

		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Going to trigger ISR %s.",
				OS_WINLINGetNameOfIsr(pIsr));

		/* Triggers the given ISR regardless if it's enabled or not.
		 * The respective core thread will handle the case when an ISR is triggered but is
		 * actually disabled. This helps to synchronize dis-/enabling of ISRs and triggering
		 * them, because a core thread is the only one in charge.
		 *
		 * Another important aspect to consider is when system threads trigger interrupts, i.e.
		 * when hook functions or alarm callbacks trigger interrupts.
		 * In this case we must no wait for the accepted event. This is
		 * because the accepted event is set by the respective core thread, which also
		 * executes the hook function or alarm callback at the same time. A deadlock is
		 * the result.
		 *
		 * This is also true for external threads, but those can't be suspended by AUTOSAR OS
		 * anyway. Hence, they can just trigger the interrupt and go along.
		 *
		 * This problem does not arise, when external threads -- neither task or ISRs threads --
		 * trigger interrupt. In those cases the respective core thread can act upon the
		 * request, because it is not requesting the interrupt at the same time.
		 */
		if ((OS_WINLINIsThisAnExternalThread()) || (OS_WINLINIsThisASystemThread()))
		{
			OS_WINLIN_OSAL_SetEvent(OS_WINLINGetRequestedEventOfIsr(pIsrDyn));
		}
		else /* ... this is a task or ISR thread. */
		{
			/* The syscall handler sets the requested event of *pIsr. On return from that
			 * handler to respective core thread checks, if there are any pending IRQs and
			 * that'll be the case, because we just issued an interrupt request. Therefore,
			 * the core thread will process this IRQ first, before returning from the syscall.
			 */
			OS_WINLINTriggerInterruptViaSyscall(pIsr);
		}
	}
	else /* ... isrId is invalid. */
	{
		OS_WINLINLog(
				OS_WINLIN_LOG_ERROR,
				"The ISR ID %u is invalid. It must be out of [0, %u).",
				isrId,
				OS_NINTERRUPTS);
	}
#else
	OS_WINLINLog(
			OS_WINLIN_LOG_ERROR,
			"The interrupt with ID %u should be triggered, "
			"but there are no interrupts configured at all.",
			isrId);
	OS_WINLINPanic();
#endif /* OS_NINTERRUPTS != 0 */
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
