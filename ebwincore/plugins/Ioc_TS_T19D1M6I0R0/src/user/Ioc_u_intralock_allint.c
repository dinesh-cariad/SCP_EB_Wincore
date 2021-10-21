/* Ioc_u_intralock_allint.c
 *
 * This file contains an local-core lock implementation using Autosar OS
 * Suspend/ResumeOsInterrupts, usable from user space.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_intralock_allint.c 2711 2018-06-28 05:17:41Z masa8317 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastDiscardsQualPtrTgtType
 *   Volatile is cast away from pointer target type.
 *
 * Reason: Not an issue, as pointer is not used at all.
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <Os.h>

/*
 * !LINKSTO IOC.Function.IOC_OsAllInterrupts_Lock, 1
*/
ioc_return_t IOC_OsAllInterrupts_Lock(volatile void *unused_lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	IOC_PARAM_UNUSED(unused_lock);

	SuspendAllInterrupts();
	return IOC_E_OK;
}

/*
 * !LINKSTO IOC.Function.IOC_OsAllInterrupts_Unlock, 1
*/
ioc_return_t IOC_OsAllInterrupts_Unlock(volatile void *unused_lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	IOC_PARAM_UNUSED(unused_lock);

	ResumeAllInterrupts();
	return IOC_E_OK;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
