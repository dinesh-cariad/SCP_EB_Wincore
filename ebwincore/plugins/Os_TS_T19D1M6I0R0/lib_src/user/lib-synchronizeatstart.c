/* lib-synchronizeatstart.c
 *
 * Synchronize CPU cores before AUTOSAR startup
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-synchronizeatstart.c 23966 2016-04-12 12:57:53Z mist9353 $
 */

#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_SynchronizeAtStart
 *
 * This function can be called by the startup code BEFORE main is called. It is used to synchronize
 * a multi-core system that already has all its cores running before the OS startup code is executed.
 *
 * All cores wait here till all other cores have arrived. Then the master proceeds immediately, but
 * the other cores wait until OS_StartCore is called.
 *
 * Note: This function depends on the OS_syncArray and OS_coreStarted being initialized to
 *       zero before it is called by any core
*/
void OS_SynchronizeAtStart(void)
{
	os_coreid_t me = OS_GetMyCoreId();

	/* Wait here until all other cores get here */
	OS_SyncHere();

	if (me != OS_INITCORE)
	{
		/* Master continues, all others wait until they are explicitly started
		 * by some other core. */
		do {
			/* Wait here until told to start */
			OS_CacheInvalidate(&(OS_coreStarted[me]), sizeof(OS_coreStarted[0]));
		} while ( OS_coreStarted[me] != OS_TRUE );
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
