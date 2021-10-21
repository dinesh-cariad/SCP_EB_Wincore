/* WINLIN-clearinterrupt.c
 *
 * This file contains the implentation of OS_WINLINClearInterrupt().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-clearinterrupt.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_api_arch.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Clears a interrupt request.
 *
 * If an interrupt has been requested but not yet serviced, this
 * function can be used to cancel the request. In this case, the
 * interrupt won't be triggered.
 *
 * If the interrupt hasn't been requested at the time this function
 * was called, it has effectively no effects.
 *
 * \param[in]	iid		The ISR ID of the interrupt to be cleared.
 */
void OS_WINLINClearInterrupt(os_isrid_t iid)
{
	(void)OS_WINLINSyscall(
			OS_SC_WINLINClearInterrupt,
			(os_paramtype_t)iid,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
