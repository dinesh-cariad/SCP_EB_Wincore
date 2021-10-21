/* WINLIN-isthisacorethread.c
 *
 * This file contains the implementation of OS_WINLINIsThisACoreThread().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-isthisacorethread.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_kernel.h>
#include <WINLIN/Os_WINLIN_cpu.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Determines if the calling thread is a core thread.
 *
 * A core thread is a thread running OS_WINLINCoreEventLoop().
 *
 * \retval OS_TRUE	The calling thread is a core thread.
 * \retval OS_FALSE	The calling thread is not a core thread, i.e. it's a task or ISR thread.
 */
os_boolean_t OS_WINLINIsThisACoreThread(void)
{
	os_boolean_t thisIsACoreThread = OS_FALSE;
	os_winlinTid_t const currentThreadId = OS_WINLIN_OSAL_GetCurrentThreadId();

	if (OS_WINLINIsInitialized())
	{
		os_coreid_t coreIdIter;
		for (coreIdIter = 0; coreIdIter < OS_N_CORES_MAX; ++coreIdIter)
		{
			if (OS_WINLINIsOsUsedOnCore(coreIdIter))
			{
				os_winlinTid_t const coreThreadId = OS_WINLINGetCoreThreadIdOfCore(coreIdIter);
				if (coreThreadId == currentThreadId)
				{
					thisIsACoreThread = OS_TRUE;
					break;
				}
			}
		}
	}
	else
	{
		/* During startup there are no core threads at all but this function might be
		 * called in the course of making log entries. In this case we simply pretend
		 * that the caller is a core thread. Otherwise the system would deadlock,
		 * due to lack of core threads to serve syscalls.
		 */
		thisIsACoreThread = OS_TRUE;
	}

	return thisIsACoreThread;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
