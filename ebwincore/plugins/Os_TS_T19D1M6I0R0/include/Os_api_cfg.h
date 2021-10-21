/* Os_api_cfg.h - configuration dependent OS API
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_cfg.h 30633 2019-08-12 14:02:48Z ache271795 $
*/
#ifndef OS_API_CFG_H
#define OS_API_CFG_H

#include <Os_types.h>
#include <Os_user.h>
#include <Os_alci.h>

#if (OS_KERNEL_TYPE==OS_MICROKERNEL)
#error "Os_api_cfg.h should not be used in a microkernel environment"
#endif

/*
 * !LINKSTO Kernel.API.InterruptHandling.EnableAllInterrupts.API, 1
 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.API, 1
 * !LINKSTO Kernel.API.InterruptHandling.ResumeAllInterrupts.API, 1
 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.API, 1
 * !LINKSTO Kernel.API.InterruptHandling.ResumeOSInterrupts.API, 1
 * !LINKSTO Kernel.API.InterruptHandling.SuspendOSInterrupts.API, 1
*/
#if OS_USE_FAST_LOCKING

#define DisableAllInterrupts()		OS_FastSuspendAllInterrupts()
#define EnableAllInterrupts()		OS_FastResumeAllInterrupts()
#define SuspendAllInterrupts()		OS_FastSuspendAllInterrupts()
#define ResumeAllInterrupts()		OS_FastResumeAllInterrupts()
#define SuspendOSInterrupts()		OS_FastSuspendOsInterrupts()
#define ResumeOSInterrupts()		OS_FastResumeOsInterrupts()

#else

#define DisableAllInterrupts()		OS_UserSuspendInterrupts(OS_LOCKTYPE_NONEST)
#define EnableAllInterrupts()		OS_UserResumeInterrupts(OS_LOCKTYPE_NONEST)
#define SuspendAllInterrupts()		OS_UserSuspendInterrupts(OS_LOCKTYPE_ALL)
#define ResumeAllInterrupts()		OS_UserResumeInterrupts(OS_LOCKTYPE_ALL)
#define SuspendOSInterrupts()		OS_UserSuspendInterrupts(OS_LOCKTYPE_OS)
#define ResumeOSInterrupts()		OS_UserResumeInterrupts(OS_LOCKTYPE_OS)

#endif	/* OS_USE_FAST_LOCKING */


#if (OS_N_CORES > 1)

#define StartCore(coreid, status)			OS_StartCore(OS_GetPhysicalCoreId(coreid), (status))
#define GetSpinlock(s)						OS_GetSpinlock((s))
#define TryToGetSpinlock(lockid, status)	OS_UserTryToGetSpinlock((lockid), (status))
#define ReleaseSpinlock(s)					OS_UserReleaseSpinlock((s))
#define ShutdownAllCores(r)					OS_UserShutdownAllCores(r)

#else

#define StartCore(coreid, status)			do { *(status) = E_OS_STATE; } while (0)
#define GetSpinlock(s)						E_OS_ID
#define TryToGetSpinlock(lockid, status)	E_OS_ID
#define ReleaseSpinlock(s)					E_OS_ID
#define ShutdownAllCores(r)					ShutdownOS(r)

#endif /* OS_N_CORES > 1 */

#endif


/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
