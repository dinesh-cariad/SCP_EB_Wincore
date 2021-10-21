/* kern-dropinternallock.c
 *
 * This file contains the function OS_DropInternalLock().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dropinternallock.c 29745 2019-01-30 08:27:51Z vaka852 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_DropInternalLock() might call itself indirectly via OS_Panic() in
 *   OS_DropInternalSpinlockOnly().
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
*/

#include <Os_kernel.h>
#include <Os_spinlock.h>

/*!
 * OS_DropInternalLock()
 *
 * Drops the internal spinlock identified by 'lockId' and restores the interrupt enabled/disabled state according
 * to 'is'.
 */
/* Deviation MISRAC2012-1 <1> */
void OS_DropInternalLock(os_lockid_t lockId, os_intstatus_t is)
{
	OS_DropInternalSpinlockOnly(lockId);
	OS_IntRestore(is);
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
