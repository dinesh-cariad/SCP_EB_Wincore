/* WINLIN-triggerinterruptviasyscall.c
 *
 * The implementation of an function to trigger interrupts via a syscall.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-triggerinterruptviasyscall.c 24660 2016-08-17 12:31:56Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_configuration.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
/* All system calls end in OS_WINLINSyscall(), which must provide a generic prototype.
 * This means, that type casts must be used for its arguments, what leads to compiler warnings.
 * These warnings shall be suppressed herewith.
 */
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
#endif

/** \brief Triggers the given ISR via a syscall.
 *
 * This is done to meet the expectation that once an interrupt has been triggered on
 * line n, it is served between that line an the following, i.e. line n + 1. Triggering
 * via a syscall helps in this case, because the syscall interface takes care about
 * thread suspension/resumption in the context of pending interrupts. This is realized
 * by the call of OS_WINLINAreThereAnyPendingIrqsToServe() in OS_WINLINServeSyscall().
 *
 * \param [in] pIsr	The ISR to trigger.
 */
void OS_WINLINTriggerInterruptViaSyscall(os_isr_t const* pIsr)
{
#if (OS_NINTERRUPTS != 0)
	(void)OS_WINLINSyscall(
			OS_SC_WINLINTriggerInterruptViaSyscall,
			(os_paramtype_t)pIsr,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1);
#else
	OS_WINLINLog(
				OS_WINLIN_LOG_ERROR,
				"The interrupt with ID %u should be triggered via a syscall, "
				"but there are no interrupts configured at all.",
				pIsr->isrId);
	OS_WINLINPanic();
#endif
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
