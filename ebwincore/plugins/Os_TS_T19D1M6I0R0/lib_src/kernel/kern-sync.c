/* kern-sync.c
 *
 * Synchronization of 2 or more cores during startup.
 *
 * $Id: kern-sync.c 29745 2019-01-30 08:27:51Z vaka852 $
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object shall be defined at block scope if its identifier only appears in
 * a single function.
 *
 * Reason:
 * Required by AUTOSAR memory mapping (see document ID 128: AUTOSAR_SWS_MemoryMapping).
 *
 *
 * MISRAC2012-2) Deviated Rule: 17.7 (required)
 * The value returned by a function having non-void return type shall be used.
 *
 * Reason:
 * False positive, because function OS_PANIC() never returns.
 *
 * MISRAC2012-3) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_SyncHere() might call itself indirectly via OS_Panic().
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
 */

#include <Os_kernel.h>
#include <Os_tool.h>
#include <Os_panic.h>
#include <Os_spinlock.h>

#include <memmap/Os_mm_var_begin.h>
/* Deviation MISRAC2012-1 <START> */
/* CHECK: NOPARSE */
static
OS_SECTION_PRAGMA(OS_syncArray, .bss.shared.os.OS_syncArray)
os_sw_syncspot_t
	OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_syncArray)
	OS_syncArray[OS_N_CORES_MAX]
	OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_syncArray);
/* CHECK: PARSE */
/* Deviation MISRAC2012-1 <STOP> */
#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SyncHere()
 *
 * This function performs barrier-style synchronization for all cores.
 *
 * It depends on
 *   - a properly initialized C-environment, i.e., the OS_syncArray is completely
 *     initialized to zeroes before this function is first invoked on any core.
 *   - the hardware being able to atomically write a value to an os_syncspot_t
 *     (hardware dependent type!)
 *   - called no more often than the highest positive number os_syncspot_t can
 *     represent (hardware dependent)
 *
 * How it works:
 *
 * Every core "owns" one element of the OS_syncArray and is the only one writing
 * to this element. At the beginning of the function, each core increments his
 * array element. Then it repeatedly checks if the other cores have done the same
 * by looking at their values in turn.
 *
 * Even though every core only writes to its entry in the array, synchronization
 * is needed because the cache flush operation may actually also write the entries
 * owned by other cores.
 *
 * When all cores have incremented their elements, all cores will recognize this
 * within the next iteration of the check loop. Then all cores proceed further.
*/
/* Deviation MISRAC2012-3 <1> */
void OS_SyncHere (void)
{
	os_coreid_t const me = OS_GetMyCoreId();
	os_sw_syncspot_t newval;
	os_uint_t i;
	os_int_t all;
	os_intstatus_t is;

	/* Increment the core's own entry in the sync array */
	is = OS_TakeInternalLock(OS_SYNCHERE_LOCKID);
	OS_CacheInvalidate(&OS_syncArray[me], sizeof(OS_syncArray[me]));
	newval = OS_syncArray[me] + 1;
	OS_syncArray[me] = newval;
	OS_CacheFlush(&OS_syncArray[me], sizeof(OS_syncArray[me]));
	OS_DropInternalLock(OS_SYNCHERE_LOCKID, is);

	do {
		/* Core waits here until all cores have arrived here.
		*/
		all = 1;
		OS_CacheInvalidate(OS_syncArray, sizeof(OS_syncArray));
		for ( i = 0; i < OS_N_CORES_MAX; i++ )
		{
			if ( OS_CoreIsInUse(i) && (OS_syncArray[i] < newval) )
			{
				all = 0;
			}
		}
		if ( OS_syncArray[me] != newval )
		{
			/* Somehow our syncArray-element changed. This really shouldn't happen.
			   If it happens anyway, there likely is a problem with the memory arbiter
			   or the cache coherency unit
			*/
			/* Deviation MISRAC2012-2 <1> */
			OS_PANIC(OS_PANIC_SyncArrayCorrupted);
		}
	} while ( all == 0 );
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
