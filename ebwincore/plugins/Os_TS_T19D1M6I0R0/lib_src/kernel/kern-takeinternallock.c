/* kern-takeinternallock.c
 *
 * This file contains the function OS_TakeInternalLock().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-takeinternallock.c 29745 2019-01-30 08:27:51Z vaka852 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_TakeInternalLock() might call itself via OS_Panic().
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
*/

#include <Os_kernel.h>
#include <Os_spinlock.h>
#include <Os_panic.h>

/*!
 * OS_TakeInternalLock()
 *
 * Takes the internal lock identified by 'lockId', disables interrupts and returns the previous interrupt
 * enabled/disabled state.
 *
 * The macro OS_NHARDWARELOCKS denotes the number of locks implemented in hardware, if provided by an architecture.
 * Any remaining locks are implemented by first taking a special hardware lock to protect the array
 * of software locks. Besides this, the macro OS_HAS_ARCHSPINLOCK allows the use of a completely architecture-dependent
 * way of locking, e.g. atomic compare-and-swap instructions.
 */
/* Deviation MISRAC2012-1 <1> */
os_intstatus_t OS_TakeInternalLock(os_lockid_t lockId)
{
	os_intstatus_t is;
	os_lockid_t lockIndex;

	is = OS_IntDisable();

#if (OS_NHARDWARELOCKS != 0)
	if ( lockId < OS_NHARDWARELOCKS )
	{
		OS_ArchTakeHardwareLock(lockId, is);
	}
	else
#endif
	{
		lockIndex = lockId - OS_NHARDWARELOCKS;
		if (OS_DEVSANITYCHECK(lockIndex >= OS_nSoftwareLocks))
		{
			(void) OS_PANIC(OS_PANIC_InvalidInternalSpinlockConfig);
		}
#if (OS_HAS_ARCHSPINLOCK == 1)
		OS_ArchTakeSpinlock(&OS_softwareLock[lockIndex]);
#else
		OS_ArchTakeHardwareLock(OS_SOFTWARELOCK_LOCKID, is);
		OS_CacheInvalidate(&OS_softwareLock[lockIndex], sizeof(OS_softwareLock[lockIndex]));

		while ( OS_softwareLock[lockIndex] != 0 )
		{
			OS_ArchDropHardwareLock(OS_SOFTWARELOCK_LOCKID);
			OS_IntRestore(is);
			is = OS_IntDisable();
			OS_ArchTakeHardwareLock(OS_SOFTWARELOCK_LOCKID, is);
			OS_CacheInvalidate(&OS_softwareLock[lockIndex], sizeof(OS_softwareLock[lockIndex]));
		}

		OS_softwareLock[lockIndex] = 1;
		OS_CacheFlush(&OS_softwareLock[lockIndex], sizeof(OS_softwareLock[lockIndex]));
		OS_ArchDropHardwareLock(OS_SOFTWARELOCK_LOCKID);
#endif /* OS_HAS_ARCHSPINLOCK == 1 */
	}

	return is;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
