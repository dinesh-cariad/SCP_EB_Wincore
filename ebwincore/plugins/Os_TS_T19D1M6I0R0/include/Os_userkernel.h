/* Os_userkernel.h
 *
 * This file defines the bits of the kernel API that are needed for the user-mode system calls.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_userkernel.h 28983 2018-07-27 10:31:55Z vaka852 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedValue
 *   value computed is not used.
 *
 * Reason: This is reported from test source code, and is not an issue. In many of our tests,
 * we don't need to collect the value returned from an OS API through OS_CALL macro.
*/

#ifndef OS_USERKERNEL_H
#define OS_USERKERNEL_H

#include <Os_kernel.h>

#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)

/* Kernel entry - just increment the inKernel counter
*/
#define OS_EnterKernel()	(OS_GetKernelData()->inKernel++)

/* Kernel exit is more complex, so it's a function by default
*/
#ifndef OS_ArchLeaveKernel
#define OS_ArchLeaveKernel(e)	OS_LeaveKernel(e)
#endif

#ifndef OS_ASM
os_unsigned_t OS_LeaveKernel(os_unsigned_t);

#ifdef OS_ARCH_LEAVEKERNEL_DECL
OS_ARCH_LEAVEKERNEL_DECL
#endif

#endif

/* A simulated "system-call" means simply entering the kernel, calling the kernel routine,
 * then leaving the kernel. OS_ArchLeaveKernel() returns the value passed as its parameter, which
 * is the return value from the kernel routine. This only works for non-void kernel routines,
 * so there's a special OS_VOIDCALL macro for void routines.
 *
 * it would be much easier if all kernel routines returned something.
*/
/* Possible diagnostic TOOLDIAG-1 <+1> */
#define OS_CALL(typ, call)		((typ)OS_ArchLeaveKernel((OS_EnterKernel(), (os_unsigned_t)(call))))

#define OS_VOIDCALL(call) \
	do {									\
		OS_EnterKernel();					\
		(call);								\
		(void)OS_ArchLeaveKernel(OS_E_OK);	\
	} while(0);

#endif /* OS_KERNEL_TYPE==OS_FUNCTION_CALL */

#ifndef OS_ASM

/* Kernel functions for system services
*/
#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
extern void OS_KernReturnFromCall(void);
extern os_result_t OS_KernTerminateTask(void);
extern os_result_t OS_KernChainTask(os_taskid_t);
extern os_result_t OS_KernActivateTask(os_taskid_t);
extern os_result_t OS_KernSchedule(void);
extern os_result_t OS_KernGetTaskId(os_taskid_t*);
extern os_result_t OS_KernGetTaskState(os_taskid_t, os_taskid_t*);
extern os_result_t OS_KernGetResource(os_resourceid_t);
extern os_result_t OS_KernReleaseResource(os_resourceid_t);
extern os_result_t OS_KernSetEvent(os_taskid_t, os_eventmask_t);
extern os_result_t OS_KernClearEvent(os_eventmask_t);
extern os_result_t OS_KernWaitEvent(os_eventmask_t);
extern os_result_t OS_KernGetStackInfo(os_taskorisr_t, os_stackinfo_t*);
extern void OS_KernSuspendInterrupts(os_intlocktype_t);
extern void OS_KernResumeInterrupts(os_intlocktype_t);
extern os_applicationid_t OS_KernGetApplicationId(void);
extern os_applicationid_t OS_KernCheckObjectOwnership(os_objecttype_t, os_objectid_t);
extern os_boolean_t OS_KernCheckObjectAccess(os_applicationid_t, os_objecttype_t, os_objectid_t);
extern os_isrid_t OS_KernGetIsrId(void);
extern os_memoryaccess_t OS_KernCheckIsrMemoryAccess(os_isrid_t, const void*, os_size_t);
extern os_memoryaccess_t OS_KernCheckTaskMemoryAccess(os_taskid_t, const void*, os_size_t);
extern os_result_t OS_KernCallTrustedFunction(os_functionid_t, void*);
extern os_result_t OS_KernDisableInterruptSource(os_isrid_t);
extern os_result_t OS_KernEnableInterruptSource(os_isrid_t);
#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

extern os_result_t OS_KernUnknownSyscall(void);
extern os_result_t OS_KernSetRelAlarm(os_alarmid_t, os_tick_t, os_tick_t);
extern os_result_t OS_KernSetAbsAlarm(os_alarmid_t, os_tick_t, os_tick_t);
extern os_result_t OS_KernCancelAlarm(os_alarmid_t);
extern os_result_t OS_KernGetAlarm(os_alarmid_t, os_tick_t*);
extern os_result_t OS_KernGetAlarmBase(os_alarmid_t, os_alarmbase_t*);
extern os_result_t OS_KernGetEvent(os_taskid_t, os_eventmask_t*);
extern void OS_KernStartOs(os_appmodeid_t);
extern void OS_KernShutdownOs(os_result_t);
extern os_result_t OS_KernIncrementCounter(os_counterid_t);
extern os_result_t OS_KernTerminateApplication(os_applicationid_t, os_restart_t);
extern os_appmodeid_t OS_KernGetActiveApplicationMode(void);
extern os_result_t OS_KernStartScheduleTable(os_scheduleid_t, os_tick_t, os_boolean_t);
extern os_result_t OS_KernStartScheduleTableSynchron(os_scheduleid_t);
extern os_result_t OS_KernChainScheduleTable(os_scheduleid_t, os_scheduleid_t);
extern os_result_t OS_KernStopScheduleTable(os_scheduleid_t);
extern os_result_t OS_KernSyncScheduleTable(os_scheduleid_t, os_tick_t);
extern os_result_t OS_KernGetScheduleTableStatus(os_scheduleid_t, os_schedulestatus_t*);
extern os_result_t OS_KernSetScheduleTableAsync(os_scheduleid_t);
extern os_result_t OS_KernGetCounterValue(os_counterid_t, os_tick_t*);
extern os_result_t OS_KernGetElapsedCounterValue(os_counterid_t, os_tick_t*, os_tick_t*);
#if (OS_N_CORES > 1)
extern os_result_t OS_KernTryToGetSpinlock(os_autosarspinlockid_t, os_tryspinlock_t*);
extern os_result_t OS_KernReleaseSpinlock(os_autosarspinlockid_t);
extern void OS_KernShutdownAllCores(os_result_t);
#endif /* OS_N_CORES > 1 */
extern os_result_t OS_KernGetApplicationState(os_applicationid_t, os_appstate_t *);
extern os_result_t OS_KernAllowAccess(void);

#endif /* OS_ASM */

#endif /* OS_USERKERNEL_H */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
