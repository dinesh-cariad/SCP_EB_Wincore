/* kern-synchereif.c
 *
 * This file contains the OS_SyncHereIf function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-synchereif.c 29601 2018-12-10 12:52:29Z sesc270012 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_SyncHereIf() might call itself indirectly via OS_Panic() in OS_SyncHere().
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* Synchronize here if the given argument is true.
*/
/* Deviation MISRAC2012-1 <1> */
void OS_SyncHereIf(os_boolean_t sync)
{
	if (sync)
	{
		OS_SyncHere();
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
