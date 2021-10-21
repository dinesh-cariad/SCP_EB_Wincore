/* Ioc_k_ac_intralock_allint.c
 *
 * This file contains kernel-mode core-local lock functions for the AutoCore OS.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_intralock_allint.c 2711 2018-06-28 05:17:41Z masa8317 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastDiscardsQualPtrTgtType
 *   Volatile is cast away from pointer target type.
 *
 * Reason: Not an issue, as pointer is not used at all.
*/

#include <public/Ioc_basic_types.h>
#include <public/Ioc_libconfig.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only meant for the AutoCore OS variant of the IOC. Check your makefiles."
#endif

#include <Os_types.h>
#include <Os_kernel.h>

/*
 * !LINKSTO IOC.Function.IOC_OsInterruptsKernel_Lock, 1
*/
ioc_return_t IOC_OsInterruptsKernel_Lock(volatile void * unused_lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	IOC_PARAM_UNUSED(unused_lock);
	OS_KernSuspendInterrupts(OS_LOCKTYPE_OS);
	return IOC_E_OK;
}

/*
 * !LINKSTO IOC.Function.IOC_OsInterruptsKernel_Unlock, 1
*/
ioc_return_t IOC_OsInterruptsKernel_Unlock(volatile void * unused_lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	IOC_PARAM_UNUSED(unused_lock);
	OS_KernResumeInterrupts(OS_LOCKTYPE_OS);
	return IOC_E_OK;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
