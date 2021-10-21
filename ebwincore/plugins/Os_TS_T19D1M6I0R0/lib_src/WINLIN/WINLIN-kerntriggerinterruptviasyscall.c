/* WINLIN-kerntriggerinterruptviasyscall.c
 *
 * The kernel mode version of OS_WINLINTriggerInterruptViaSyscall().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-kerntriggerinterruptviasyscall.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \copydoc OS_WINLINTriggerInterruptViaSyscall()
 *
 * \param pIsr	Points to the ISR to be triggered.
 */
void OS_WINLINKernTriggerInterruptViaSyscall(os_isr_t const* pIsr)
{
	OS_WINLINAssert(pIsr != OS_NULL);
	OS_WINLINAssert(pIsr->isrId < OS_nInterrupts);

	OS_WINLIN_OSAL_SetEvent(OS_WINLINGetRequestedEventOfIsr(pIsr->dynamic));
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
