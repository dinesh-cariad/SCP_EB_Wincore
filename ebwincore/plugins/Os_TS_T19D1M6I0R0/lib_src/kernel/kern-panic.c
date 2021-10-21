/* kern-panic.c
 *
 * This file contains the OS_Panic function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-panic.c 29563 2018-11-30 08:59:45Z olme8414 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   The statement cannot be reached by the control flow.
 *
 * Reason: This statement shall never be reached, as the program is halted because
 * of an unrecoverable error.
 *
 * TOOLDIAG-2) Possible diagnostic: InfiniteLoop
 *   Possible infinite loop
 *
 * Reason: Intentional endless loop in case of QM-OS panic in a Safety OS system.
 */

#include <Os_kernel.h>
#include <Os_panic.h>

#if OS_KERNEL_TYPE==OS_MICROKERNEL
#include <public/Mk_autosar.h>
#endif

#include <memmap/Os_mm_code_begin.h>

/* OS_Panic
 *
 * This function is called whenever the kernel detects an internal error
 * from which it cannot recover.
*/
/* Deviation MISRAC2012-1 <1> */
os_result_t OS_Panic(os_panic_t curPanicCode)
{
#if OS_KERNEL_TYPE==OS_MICROKERNEL
	*(OS_panicCodePtrs[GetCoreID()]) = curPanicCode;

	/* Possible diagnostic TOOLDIAG-2 <1> */
	for (;;) {}
#else
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	if (kernel_data->panicCode == OS_PANIC_NONE)
	{
		/* This is the first time OS_Panic is entered, so try OS_Shutdown. */
		kernel_data->panicCode = curPanicCode;
		OS_Shutdown(OS_E_PANIC, OS_FALSE);
	}
	else
	{
		/* Entered OS_panic the second time, so call OS_ShutdownNoHooks, which can't panic. */
		OS_ShutdownNoHooks();
	}
#endif
	/* Possible diagnostic TOOLDIAG-1 <1> */
	return OS_E_PANIC;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
