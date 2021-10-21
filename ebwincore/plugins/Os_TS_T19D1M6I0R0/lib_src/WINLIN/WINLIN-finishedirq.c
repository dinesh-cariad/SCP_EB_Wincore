/* WINLIN-finishedirq.c
 *
 * This file contains the implementation of OS_WINLINFinishedIrq().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-finishedirq.c 23671 2016-02-16 14:27:54Z olme8414 $
 */

#include <Os_kernel.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Signals a the end of an interrupt request.
 *
 * At the end of an interrupt request this function is automatically called.
 * It informs the respective core thread, that the ISR has finished and the
 * interrupted activity may be resumed.
 *
 * \param[in] pIsr		The ISR, which was executed for the requested interrupt.
 */
void OS_WINLINFinishedIrq(os_isr_t const* pIsr)
{
	(void)OS_WINLINSyscall(
			OS_SC_WINLINFinishedIrq,
			(os_paramtype_t)pIsr,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
