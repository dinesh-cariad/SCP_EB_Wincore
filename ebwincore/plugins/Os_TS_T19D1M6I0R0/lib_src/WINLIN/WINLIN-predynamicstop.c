/* WINLIN-predynamicstop.c
 *
 * This file contains the implementation of the pre-dynamic-stop feature.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-predynamicstop.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <stdlib.h>
#include <WINLIN/Os_WINLIN_critsec.h>

#include <memmap/Os_mm_var_begin.h>

/** \var OS_winlinNumberOfCoresShutdown
 * \brief The number of cores, which have been shutdown.
 */
static os_size_t OS_winlinNumberOfCoresShutdown = 0;

/** \var OS_winlinNumberOfCoresShutdownCritSec
 * \brief Protects OS_winlinNumberOfCoresShutdown from concurrent access.
 */
static os_winlinCriticalSection_t OS_winlinNumberOfCoresShutdownCritSec;

#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Initializes the dynamic stop mechanism. */
void OS_WINLINPreDynamicStopInit(void)
{
	OS_WINLIN_OSAL_InitializeCriticalSection(&OS_winlinNumberOfCoresShutdownCritSec);
}

/** \brief Handles the shutdown of a core.
 *
 * When a core is shut down the following happens:
 * -	If there is still at least one core, which hasn't been shut down yet,
 *		the core thread of the just shut down core restarts its event loop. There
 *		might be some inter-core messages to process in the future.
 * -	If all cores have been shut down the whole AUTOSAR application terminates.
 */
void OS_WINLINPreDynamicStop(void)
{
	OS_WINLINLog(OS_WINLIN_LOG_INFO, "Core %u has been shutdown.", OS_GetMyCoreId());

	/* Terminate the whole application, if all cores have been shutdown. */
	{
		os_size_t numCoresShutdown;

		OS_WINLIN_OSAL_EnterCriticalSection(&OS_winlinNumberOfCoresShutdownCritSec);
		++OS_winlinNumberOfCoresShutdown;
		numCoresShutdown = OS_winlinNumberOfCoresShutdown;
		OS_WINLIN_OSAL_LeaveCriticalSection(&OS_winlinNumberOfCoresShutdownCritSec);

		if ((os_coreid_t)numCoresShutdown == OS_N_CORES)
		{
			OS_WINLINLog(OS_WINLIN_LOG_INFO, "All cores have been shutdown.", OS_WINLINLOG_UNUSED_PARAM);
			exit(EXIT_SUCCESS);
		}
	}

	/* If not all cores have been shutdown yet, this core restarts its event loop.
	 * There might be inter-core requests in the future.
	 */
	OS_WINLINCoreLongjmp();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
