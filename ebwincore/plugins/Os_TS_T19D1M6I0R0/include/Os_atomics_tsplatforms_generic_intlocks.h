/* Os_atomics_tsplatforms_generic_intlocks.h
 *
 * This file contains the generic implementation of the compatibility functions,
 * which were once provided by asc_Platforms for interrupt control.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_atomics_tsplatforms_generic_intlocks.h 30151 2019-04-11 13:05:07Z mist9353 $
*/

#ifndef OS_ATOMICS_TSPLATFORMS_GENERIC_INTLOCKS_H
#define OS_ATOMICS_TSPLATFORMS_GENERIC_INTLOCKS_H

#include <Os_tool.h>
#include <Os_atomics.h>
#include <Os_panic.h>

/* Introduce declarations of Enable/DisableAllInterrupts()
 * depending on kernel type.
 */
#if OS_KERNEL_TYPE == OS_MICROKERNEL
#include <public/Mk_autosar.h>
#else
#include <Os_api_cfg.h>
#endif


/* !LINKSTO Kernel.Atomics.TSFunctionsIntCtrl, 1
*/

static OS_TOOL_INLINE_KEYWORD os_intstatus_t OS_UserAtomicTSIntDisable(void)
{
	SuspendAllInterrupts();
	return 0u;
}

static OS_TOOL_INLINE_KEYWORD void OS_UserAtomicTSIntRestore(os_intstatus_t s)
{
	if (s != 0u)
	{
		(void)OS_PANIC(OS_PANIC_ATOMICS_InvalidInterruptStatus);
	}
	ResumeAllInterrupts();
}

#endif /* OS_ATOMICS_TSPLATFORMS_GENERIC_INTLOCKS_H */
