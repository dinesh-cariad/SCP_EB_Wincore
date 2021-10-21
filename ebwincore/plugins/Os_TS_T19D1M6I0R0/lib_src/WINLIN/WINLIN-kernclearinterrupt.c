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
 * $Id: WINLIN-kernclearinterrupt.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Clears an interrupt request.
 *
 * \see OS_WINLINClearInterrupt()
 *
 * \pre The caller must be a core thread, i.e. <tt>OS_GetKernelData()->inKernel != 0</tt> must hold true.
 * User code must call OS_WINLINClearInterrupt().
 *
 * \param iid	The ID of the ISR of which the request shall be cleared.
 */
void OS_WINLINKernClearInterrupt(os_isrid_t iid)
{
	if (OS_IsValidIsrId(iid))
	{
		os_isrdynamic_t* const pIsrDyn = OS_isrTableBase[iid].dynamic;
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetRequestedEventOfIsr(pIsrDyn));
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
