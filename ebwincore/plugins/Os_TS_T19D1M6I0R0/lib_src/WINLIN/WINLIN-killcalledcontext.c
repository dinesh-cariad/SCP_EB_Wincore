/* WINLIN-killcalledcontext.c
 *
 * This file contains the implementation of OS_KillCalledContext() for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-killcalledcontext.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_tool.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Kills an indirectly called ISR or hook.
 *
 * A called context is a context such as an ISR or a hook function that
 * is called, and normally returns, in a properly nested fashion. That
 * means that the information about the call (return address, etc)
 * is normally held in the processor's call stack or equivalent.
 *
 * For WINLIN this function is not implemented yet, therefore ISRs
 * and hook functions cannot be killed.
 *
 * \param[in,out]	sc		The context of an ISR or hook function to
 *							be killed.
 *
 * \return Always returns OS_E_INTERNAL to indicate, that the conext
 * can't be killed.
 */
os_result_t OS_KillCalledContext(os_savedcontext_t *sc)
{
	/* remove unused parameter warning */
	OS_PARAM_UNUSED(sc);

	/* If we get here, we can't kill the called context. Returning some code
	 * other than OS_E_OK indicates this.
	*/
	return OS_E_INTERNAL;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
