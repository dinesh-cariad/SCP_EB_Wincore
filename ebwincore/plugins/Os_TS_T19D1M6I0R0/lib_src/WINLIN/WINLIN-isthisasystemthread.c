/* WINLIN-isthisasystemthread.c
 *
 * This file contains the implementation of a function which checks if its
 * caller is a system thread.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-isthisasystemthread.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <WINLIN/Os_WINLIN_timer.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Checks if the calling thread is a system thread.
 *
 * A <em>system thread</em> is a thread, which can't be suspended by a core thread.
 * Currently only core and FRC threads are considered system threads.
 * Those threads may use Windows/Linux services at will.
 *
 * \retval OS_FALSE	The calling thread is not a system thread.
 * \retval OS_TRUE	The calling thread is a system thread and hence may use Windows/Linux
 *					services at will. Currently, this is true only, if the calling
 *					thread is a FRC or core thread.
 */
os_boolean_t OS_WINLINIsThisASystemThread(void)
{
	return (OS_WINLINIsThisACoreThread()) || (OS_FrcIsThisAFrcThread());
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
