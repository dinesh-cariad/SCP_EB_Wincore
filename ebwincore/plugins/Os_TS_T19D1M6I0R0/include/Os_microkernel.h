/* Os_microkernel.h
 *
 * This file maps OS concepts such as OS_IntDisable() onto the microkernel API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_microkernel.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_MICROKERNEL_H
#define OS_MICROKERNEL_H

#include <Os_defs.h>
#include <public/Mk_defs.h>

/* This file only gets included when the OS is being compiled to run under the microkernel
 * This is a sanity check.
*/
#if OS_KERNEL_TYPE != OS_MICROKERNEL
#error "Os_microkernel.h should only be included when using the microkernel."
#endif

#include <public/Mk_public_api.h>
#include <public/Mk_error.h>
#include <Os_api_microkernel.h>
#include <Os_panic.h>

#ifndef MK_HAS_DUAL_RETURN_VALUE
#define MK_HAS_DUAL_RETURN_VALUE 0
#endif

#ifndef OS_MK_FAST_INTERRUPT_LOCKING
#define OS_MK_FAST_INTERRUPT_LOCKING 0
#endif

/* OS_IntEnable(), OS_IntDisable(), OS_IntRestore(), OS_IntDisableAll()
 *
 * These routines enable, disable and restore previous state of interrupts.
 * When using the microkernel 2.x the QM-OS activities are sequenced in a single thread,
 * so locking is rarely needed.
 * OS_IntEnable() and OS_IntDisableAll() should not be used under the Microkernel,
 * so they are defined to give a syntax error if used.
 * OS_IntDisableConditional() is the same as OS_IntDisable() - the API is always "conditional".
*/
#define OS_IntDisable()				(E_OS_NOFUNC)
#define OS_IntDisableConditional()	(E_OS_NOFUNC)
#define OS_IntDisableAll()			(E_OS_NOFUNC)
#define OS_IntRestore(p)			do { } while (0)
#define OS_IntRestoreAll(p)			do { } while (0)

#define OS_IntEnable()				{ error }

#define OS_ArchCanWrite(o,s)		(1)

/* These two macros dis/enable interrupts, even if the OS is driven by the microkernel.
*/
#if OS_MK_FAST_INTERRUPT_LOCKING
#define OS_ForceIntDisable()		MK_ConditionalGetResource(MK_resLockCat2)
#else
#define OS_ForceIntDisable()		MK_UsrAcquireLock(MK_resLockCat2)
#endif
#define OS_ForceIntRestore(p)		( ((p) == MK_E_OK) ? MK_UsrReleaseLock(MK_resLockCat2) : (p) )

/* QM-OS part of TerminateApplication
*/
#ifndef OS_ASM
os_result_t OS_MKTerminateApplication(os_applicationid_t, os_restart_t);
#endif

/* OS_ERROR for the Safety OS.
*/
#ifndef OS_ASM
os_result_t OS_MkiReportError(mk_serviceid_t, mk_errorid_t, os_paramtype_t *);
#endif

/* OS_InternalActivateTask(), OS_InternalSetEvent()
 *
 * These routines are called internally (autostart, alarm wrappers etc.) to control timed task scheduling
*/
#define OS_GetNTasks()				MK_LibGetNTasks()
#define OS_InternalActivateTask(t)	MK_UsrActivateTask(t)
#define OS_InternalSetEvent(t,e)	MK_UsrSetEvent(t,e)

#define OS_ERROR(e, p)				OS_MkiReportError(OS_SID, (mk_errorid_t)(e), (os_paramtype_t*)(p))
#define OS_ERROR_NOCHECK(e, p)		OS_ERROR((e), (p))
#define OS_ERRORCODE_CHECK(e)		(e)
#define OS_ERRORCODE_NOCHECK(e)		(e)

/* Service IDs defined in terms of Microkernel service IDs
*/
#define OS_SID_SetRelAlarm					MK_sid_SetRelAlarm
#define OS_SID_SetAbsAlarm					MK_sid_SetAbsAlarm
#define OS_SID_CancelAlarm					MK_sid_CancelAlarm
#define OS_SID_IncrementCounter				MK_sid_IncrementCounter
#define OS_SID_StartScheduleTable			MK_sid_StartScheduleTable
#define OS_SID_StartScheduleTableAbs		MK_sid_StartScheduleTableAbs
#define OS_SID_StartScheduleTableRel		MK_sid_StartScheduleTableRel
#define OS_SID_StartScheduleTableSynchron	MK_sid_StartScheduleTableSynchron
#define OS_SID_ChainScheduleTable			MK_sid_NextScheduleTable
#define OS_SID_StopScheduleTable			MK_sid_StopScheduleTable
#define OS_SID_SyncScheduleTable			MK_sid_SyncScheduleTable
#define OS_SID_SetScheduleTableAsync		MK_sid_SetScheduleTableAsync
#define OS_SID_TerminateApplication			MK_sid_TerminateApplication
#define OS_SID_DisableInterruptSource		MK_sid_DisableInterruptSource
#define OS_SID_EnableInterruptSource		MK_sid_EnableInterruptSource
#define OS_SID_SimTimerAdvance				MK_sid_SimTimerAdvance
#define OS_SID_GetCounterValue				MK_sid_GetCounterValue
#define OS_SID_KillAlarm					MK_sid_KillAlarm
#define OS_SID_RunSchedule					MK_sid_RunSchedule
#define OS_SID_UnknownSyscall				MK_sid_UnconfiguredOsCall
#define OS_SID_StartOs						MK_sid_OS_StartOs
#define OS_SID_GetAlarmBase					MK_sid_GetAlarmBase
#define OS_SID_GetAlarm						MK_sid_GetAlarm
#define OS_SID_GetScheduleTableStatus		MK_sid_GetScheduleTableStatus

/* Error IDs define in terms of Microkernel error IDs.
*/
#define OS_ERROR_NoError					MK_eid_NoError
#define OS_ERROR_AlarmInUse					MK_eid_OsAlarmInUse
#define OS_ERROR_AlarmNotInQueue			MK_eid_OsAlarmNotInQueue
#define OS_ERROR_AlarmNotInUse				MK_eid_OsAlarmNotInUse
#define OS_ERROR_CounterIsHw				MK_eid_OsCounterIsHw
#define OS_ERROR_DifferentCounters			MK_eid_OsDifferentCounters
#define OS_ERROR_ImplicitSyncStartRel		MK_eid_OsImplicitSyncStartRel
#define OS_ERROR_IncrementZero				MK_eid_OsIncrementZero
#define OS_ERROR_InterruptDisabled			MK_eid_OsInterruptDisabled
#define OS_ERROR_InvalidAlarmId				MK_eid_OsInvalidAlarmId
#define OS_ERROR_InvalidApplicationId		MK_eid_OsInvalidApplication
#define OS_ERROR_InvalidCounterId			MK_eid_OsInvalidCounterId
#define OS_ERROR_InvalidRestartOption		MK_eid_OsParameterOutOfRange
#define OS_ERROR_InvalidScheduleId			MK_eid_OsInvalidScheduleId
#define OS_ERROR_InvalidStartMode			MK_eid_OsInvalidStartMode
#define OS_ERROR_NotChained					MK_eid_OsNotChained
#define OS_ERROR_NotRunning					MK_eid_OsNotRunning
#define OS_ERROR_NotStopped					MK_eid_OsNotStopped
#define OS_ERROR_NotSyncable				MK_eid_OsNotSyncable
#define OS_ERROR_ParameterOutOfRange		MK_eid_OsParameterOutOfRange
#define OS_ERROR_Permission					MK_eid_OsPermission
#define OS_ERROR_Quarantined				MK_eid_OsQuarantined
#define OS_ERROR_ScheduleTableNotIdle		MK_eid_OsScheduleTableNotIdle
#define OS_ERROR_UnknownSystemCall			MK_eid_OsUnknownSystemCall
#define OS_ERROR_WriteProtect				MK_eid_OsWriteProtect
#define OS_ERROR_WrongContext				MK_eid_OsWrongContext
#define OS_ERROR_ApplicationNotAccessible	MK_eid_OsQuarantined
#define OS_ERROR_InvalidAlarmState			MK_eid_OsInvalidAlarmState

/* The service-info structures aren't used for microkernel errors. This is only needed in the
 * "varSif" APIs (those functions that provide 2 or more APIs)
 * To keep the compiler happy, os_serviceinfo_t is typedef'd to os_serviceid_t and
 * the missing OS_svc_ structures are defined as varSid, which is always present in such APIs
 * This construct doesn't make sense, but it never gets used. We just need something with valid syntax.
 *
 * CHECK: RULE 402 OFF
*/
#ifndef OS_ASM
typedef os_serviceid_t os_serviceinfo_t;
#endif
#define OS_svc_StartScheduleTableRel	varSid
#define OS_svc_StartScheduleTableAbs	varSid
/* CHECK: RULE 402 ON
*/

/* Getting the application mode
*/
#ifndef OS_ASM
#define OS_GetAppMode()	OS_MkiGetAppMode()
/* Determine the application mode in a Safety OS.
*/
os_appmodeid_t OS_MkiGetAppMode(void);
#endif

/*!
 * Application accessibility check.
 */
#ifndef OS_APPISACCESSIBLE
#define OS_APPISACCESSIBLE(app)		(MK_GetAppStateForQmOs(app) == APPLICATION_ACCESSIBLE)
#endif

/*!
 * OS_IsValidApplicationId
 *
 * Checks if a given OS application ID is valid.
 */
#define OS_IsValidApplicationId(a)		OS_DoExtendedStatusTrue((a) < OS_nApps)


/* Currently the Safety OS doesn't perform calling context or permission checks for calls to the QM-OS,
 * because they are not vital for spatial isolation or the consistency of the microkernel.
*/
#define OS_CallingContextCheck()		1
#define OS_InterruptEnableCheck(a)		1
#define OS_CurrentApp()					MK_NULL
#define OS_HasPermission(a,b)			(1)

/* The Safety OS doesn't perform error handling for APIs, which don't return a StatusType,
 * since this is an EB specific extension.
 * Currently there is no QM-OS service in the Safety OS, which doesn't return a StatusType.
*/
#define OS_ErrorHandlingForVoidApi()	{ error }

#ifndef OS_ASM
typedef mk_uint32_t os_permission_t;
#endif
#define OS_MAXPARAM		3

#ifndef OS_ASM
extern const os_applicationid_t OS_nApps;
#endif

#ifndef OS_ASM
/* Possible values for OS_GetKernelData()->inFunction */
#define OS_INBOOT			0		/* Initial state - MUST BE ZERO */
/* 0..5 reserved for non-microkernel variant. */
#define OS_ININTERNAL		6		/* In some internal kernel function */
#endif


/* When not driven by the MK, these declarations appear in Os_callouts.h
*/
#ifndef OS_ASM
extern void StartupHook(void);
#endif

/* When not driven by the MK, these definitions appear in Os_kernel_app.h
*/
#ifndef OS_ASM
typedef void (*os_startuphook_t)(void);
#endif

/* When not driven by the MK, these definitions appear in Os_kernel_task.h
*/
#ifndef OS_CALLSTARTUPHOOK
#ifndef OS_ASM
extern const os_startuphook_t		OS_ptrStartupHook;
#endif
#define OS_CALLSTARTUPHOOK()		OS_CALLHOOKVOID_INDIRECT(OS_ptrStartupHook, OS_INSTARTUPHOOK)
#endif

#define OS_CALLHOOKVOID_INDIRECT(hp,h)	\
	do {								\
		OS_TRACE_HOOK_ENTRY(h);			\
		(*(hp))();						\
		OS_TRACE_HOOK_EXIT_P(h);		\
	} while (0)
#define OS_CALLHOOKVOID_DIRECT(hf,h)	\
	do {								\
		OS_TRACE_HOOK_ENTRY(h);			\
		(hf)();							\
		OS_TRACE_HOOK_EXIT_P(h);		\
	} while (0)

/* Lookup-table for core local QM-OS panic code variables.
*/
#ifndef OS_ASM
extern volatile os_panic_t * const OS_panicCodePtrs[MK_MAXCORES];
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
