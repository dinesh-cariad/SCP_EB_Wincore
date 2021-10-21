/* Os_api.h
 *
 * This file defines the OS User API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api.h 28588 2018-04-18 11:51:55Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator must be used to generate an identifier as demanded by specification.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: RedundantDecl
 *   Redundant declaration of OS_TASK_*
 *
 * Reason:
 *   False positive. The header file Os_api.h is part of the OS and is not used
 *   to declare specific tasks. This is done in header files of a user project.
 */

#ifndef OS_API_H
#define OS_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Os_defs.h>
#include <Os_types.h>

#if (OS_KERNEL_TYPE==OS_MICROKERNEL)

#include <Os_api_microkernel.h>
#include <Os_api_timestamp.h>

#else
#include <Os_error.h>
#include <Os_api_arch.h>			/* include first to allow overriding */
#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)
#include <Os_api_nosyscall.h>
#endif  /* (OS_KERNEL_TYPE==OS_FUNCTION_CALL) */
#include <Os_acct_api.h>
#include <Os_api_simtimer.h>
#include <Os_api_timestamp.h>
#include <Os_callouts.h>

/* Application states:
 * Constants named by AUTOSAR
*/
#define APPLICATION_ACCESSIBLE OS_APP_ACCESSIBLE
#define APPLICATION_RESTARTING OS_APP_RESTARTING
#define APPLICATION_TERMINATED OS_APP_QUARANTINED

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL)
/*!
 * OS system call stubs
 *
 * These are short assembler routines that make the
 * appropriate system call.
*/
#ifndef OS_ASM

/* OSEK/VDX API */
extern os_result_t OS_UserActivateTask(os_taskid_t);
extern os_result_t OS_UserTerminateTask(void);
extern os_result_t OS_UserChainTask(os_taskid_t);
extern os_result_t OS_UserSchedule(void);
extern os_result_t OS_UserGetTaskId(os_taskid_t*);
extern os_result_t OS_UserGetTaskState(os_taskid_t, os_taskstate_t*);
extern void OS_UserSuspendInterrupts(os_intlocktype_t);
extern void OS_UserResumeInterrupts(os_intlocktype_t);
extern os_result_t OS_UserGetResource(os_resourceid_t);
extern os_result_t OS_UserReleaseResource(os_resourceid_t);
extern os_result_t OS_UserSetEvent(os_taskid_t, os_eventmask_t);
extern os_result_t OS_UserClearEvent(os_eventmask_t);
extern os_result_t OS_UserGetEvent(os_taskid_t, os_eventmask_t*);
extern os_result_t OS_UserWaitEvent(os_eventmask_t);
extern os_result_t OS_UserGetAlarmBase(os_alarmid_t, os_alarmbase_t*);
extern os_result_t OS_UserGetAlarm(os_alarmid_t, os_tick_t*);
extern os_result_t OS_UserSetRelAlarm(os_alarmid_t, os_tick_t, os_tick_t);
extern os_result_t OS_UserSetAbsAlarm(os_alarmid_t, os_tick_t, os_tick_t);
extern os_result_t OS_UserCancelAlarm(os_alarmid_t);
extern os_appmodeid_t OS_UserGetActiveApplicationMode(void);
extern void OS_UserStartOs(os_appmodeid_t);
extern void OS_UserShutdownOs(os_result_t);

/* ProOSEK 4.x API
*/
extern os_result_t OS_UserIncrementCounter(os_counterid_t);

/* AUTOSAR API
*/
extern os_result_t OS_UserGetStackInfo(os_taskorisr_t, os_stackinfo_t*);
extern os_result_t OS_UserStartScheduleTable(os_scheduleid_t, os_tick_t, os_boolean_t);
extern os_result_t OS_UserStartScheduleTableSynchron(os_scheduleid_t);
extern os_result_t OS_UserChainScheduleTable(os_scheduleid_t, os_scheduleid_t);
extern os_result_t OS_UserStopScheduleTable(os_scheduleid_t);
extern os_result_t OS_UserGetScheduleTableStatus(os_scheduleid_t, os_uint8_t*);
extern os_result_t OS_UserTerminateApplication(os_applicationid_t, os_restart_t);
extern os_applicationid_t OS_UserGetApplicationId(void);
extern os_applicationid_t OS_UserCheckObjectOwnership(os_objecttype_t, os_objectid_t);
extern os_boolean_t OS_UserCheckObjectAccess(os_applicationid_t, os_objecttype_t, os_objectid_t);
extern os_isrid_t OS_UserGetIsrId(void);
extern os_memoryaccess_t OS_UserCheckIsrMemoryAccess(os_isrid_t, void*, os_size_t);
extern os_memoryaccess_t OS_UserCheckTaskMemoryAccess(os_taskid_t, void*, os_size_t);
extern os_result_t OS_UserCallTrustedFunction(os_functionid_t, void*);
extern os_result_t OS_UserSyncScheduleTable(os_scheduleid_t, os_tick_t);
extern os_result_t OS_UserSetScheduleTableAsync(os_scheduleid_t);
extern os_result_t OS_UserDisableInterruptSource(os_isrid_t);
extern os_result_t OS_UserEnableInterruptSource(os_isrid_t);
extern os_result_t OS_UserGetCounterValue(os_counterid_t, os_tick_t*);
extern os_result_t OS_UserGetElapsedCounterValue(os_counterid_t, os_tick_t*, os_tick_t*);
#if (OS_N_CORES > 1)
extern os_result_t OS_UserTryToGetSpinlock(os_autosarspinlockid_t, os_tryspinlock_t*);
extern os_result_t OS_UserReleaseSpinlock(os_autosarspinlockid_t);
extern void OS_UserShutdownAllCores(os_result_t);
#endif /* OS_N_CORES > 1 */
extern os_result_t OS_UserGetApplicationState(os_applicationid_t, os_appstate_t *);
extern os_result_t OS_UserAllowAccess(void);

extern os_result_t OS_UserIocWrite(os_uint32_t, const void *);
extern os_result_t OS_UserIocSend(os_uint32_t, const void *);
extern os_result_t OS_UserIocEmptyQueue(os_uint32_t);
extern os_result_t OS_UserIocAddonExt(os_uint16_t, os_uint32_t, const void *);

#endif  /* OS_ASM */
#endif  /* (OS_KERNEL_TYPE==OS_SYSTEM_CALL) */

#endif  /* (OS_KERNEL_TYPE==OS_MICROKERNEL) */

/* The OS defines OS_HAS_ATOMIC_MONITOR_SUPPORT as OS_HAS_TRANSACTION_SUPPORT for backward compatibility.
 * This can be removed when all the Platforms have been updated.
*/
#define OS_HAS_ATOMIC_MONITOR_SUPPORT	OS_HAS_TRANSACTION_SUPPORT

/* Fast (but non-conformant) implementations of SuspendOSInterrupts etc.
*/
#ifndef OS_ASM
extern void OS_FastSuspendOsInterrupts(void);
extern void OS_FastResumeOsInterrupts(void);
extern void OS_FastSuspendAllInterrupts(void);
extern void OS_FastResumeAllInterrupts(void);
#endif  /* OS_ASM */

/* EB-specific system services
*/
/* !LINKSTO Kernel.Feature.InterruptSource.API.DisableInterruptSource, 1
*/
#define OS_DisableInterruptSource(i)				OS_UserDisableInterruptSource(i)
#define OSServiceID_DisableInterruptSource			OS_SID_DisableInterruptSource
#define OSError_DisableInterruptSource_DisableISR()	((os_isrid_t)(OS_GetKernelData()->errorStatus.parameter[0]))

/* !LINKSTO Kernel.Feature.InterruptSource.API.EnableInterruptSource, 1
*/
#define OS_EnableInterruptSource(i)					OS_UserEnableInterruptSource(i)
#define OSServiceID_EnableInterruptSource			OS_SID_EnableInterruptSource
#define OSError_EnableInterruptSource_EnableISR()	((ISRType)(OS_GetKernelData()->errorStatus.parameter[0]))

#ifndef OS_ASM
/* User library routines
*/
extern os_int_t OS_StackCheck(void);
extern void OS_GetCurrentStackArea(void **, void **);

/* OS_StartGptTickers
 *
 * This function is defined in src/Os_starttickers.c if there are GPT tickers to start and if
 * automatic startup of the Gpt tickers has been inhibited by defining OS_INHIBIT_GPT_STARTUP=1
 * in board.h
*/
extern void OS_StartGptTickers(void);

/* Extra APIs to help improve the performance of non- and mixed-premptive systems.
 *
 * !LINKSTO Kernel.Feature.FastSchedule, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ErrorChecking, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ScheduleIfNecessary, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ScheduleIfNecessary.ReturnValue, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ScheduleIfWorthwhile, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ScheduleIfWorthwhile.ReturnValue, 1
*/
extern os_boolean_t OS_IsScheduleNecessary(void);
extern os_boolean_t OS_IsScheduleWorthwhile(void);

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
/* Obtain error information (from kernel data). */
extern const os_errorstatus_t * OS_GetErrorInfo(void);
#endif

#endif   /* OS_ASM */

#define OS_ScheduleIfNecessary()	( OS_IsScheduleNecessary() ? OS_UserSchedule() : OS_E_OK )
#define OS_ScheduleIfWorthwhile()	( OS_IsScheduleWorthwhile() ? OS_UserSchedule() : OS_E_OK )

/* AUTOSAR multicore API
*/
#if (OS_N_CORES > 1)
#ifndef OS_ASM
extern void OS_StartCore(os_coreid_t, os_result_t*);
extern os_result_t OS_GetSpinlock(os_autosarspinlockid_t);
#endif /* OS_ASM */
#endif /* OS_N_CORES > 1 */

/* Mapping for RES_SCHEDULER
 *
 * In a multi-core configuration, this map can be used to get RES_SCHEDULER of the respective core.
*/
#if (OS_N_CORES > 1)
#ifndef OS_ASM
extern os_resourceid_t OS_GetResScheduler(void);
#endif /* OS_ASM */
#endif /* OS_N_CORES > 1 */

/* If we're running on a microkernel that supports the WaitGetClearEvent API, map OS_WaitGetClearEvent to it.
*/
#if OS_KERNEL_TYPE == OS_MICROKERNEL

#ifdef MK_HAS_WAITGETCLEAREVENT

#define OS_HAS_WAITGETCLEAREVENT	MK_HAS_WAITGETCLEAREVENT

#if OS_HAS_WAITGETCLEAREVENT
#define OS_WaitGetClearEvent(e, ep)	MK_WaitGetClearEvent((e), (ep))
#endif

#endif /* MK_HAS_WAITGETCLEAREVENT */

#else /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/*!
 * ProOSEK 4.x API
 *
 * Additional data types for ProOSEK 4.x compatibility
*/
#ifndef OS_ASM
typedef os_isrid_t IsrType;
#endif

/*!
 * ProOSEK 4.x API
 *
 * These macros define the additional features of ProOSEK 4.x in
 * terms of the OS system-call stubs and library calls
 *
 * Note: AdvanceCounter and IAdvanceCounter both map to the same
 * OS API. This is not an error.
 *
 * CHECK: SAVE
 * CHECK: RULE 402 OFF (Legacy API macros)
*/
#define stackCheck()				OS_StackCheck()
#define getUsedTaskStack(t)			OS_GetUsedTaskStack(t)
#define getUnusedTaskStack(t)		OS_GetUnusedTaskStack(t)
#define getUsedIsrStack()			OS_GetUsedIsrStack()
#define getUnusedIsrStack()			OS_GetUnusedIsrStack()

#define AdvanceCounter(c)			OS_UserIncrementCounter(c)
#define IAdvanceCounter(c)			OS_UserIncrementCounter(c)

/* CHECK: RESTORE
*/

/*!
 * ISR1
 *
 * This macro can be used to define category 1 interrupt functions
 * in an architecture-independent way.
 *
 * NOTE: Defining this macro in terms of the standard OSEK ISR() macro
 * does not work. The compiler expands the parameter before expanding
 * ISR(), and the result is OS_ISR_n (where n is the numeric ID of
 * the ISR from os.h)
*/
/* Deviation MISRAC2012-1 */
#define ISR1(x)				OS_EXTERN_C void OS_ISR_##x(void)


#ifndef OS_ASM
/* Library routines to map some OS system calls onto ProOSEK APIs
*/
os_size_t OS_GetUsedTaskStack(os_taskid_t t);
os_size_t OS_GetUnusedTaskStack(os_taskid_t t);
os_size_t OS_GetUsedIsrStack(void);
os_size_t OS_GetUnusedIsrStack(void);
#endif


/*!
 * OSEK/VDX API
 *
 * The OSEK/VDX API is defined in terms of the OS API, which is
 * has a different naming convention and in one or two places is
 * slightly different to facilitate the memory-protection requirements.
 *
 * !LINKSTO Kernel.Autosar.OSEK, 1
*/

/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX data types in terms of
 * OSEMKP data types.
 *
 * !LINKSTO Kernel.API.StatusType, 1
 * !LINKSTO Kernel.API.TaskManagement.DataTypes, 1
 * !LINKSTO Kernel.API.ResourceManagement.DataTypes, 1
 * !LINKSTO Kernel.API.EventControl.DataTypes, 1
 * !LINKSTO Kernel.API.Alarms.DataTypes, 1
 * !LINKSTO Kernel.API.OSControl.DataTypes, 1
 * !LINKSTO Kernel.API.Hooks.DataTypes, 1
*/
#if (!defined STATUSTYPEDEFINED)
/* Conformant with OSEK Binding Specification.
 * We have to check and set STATUSTYPEDEFINED to be compatible here.
*/
#ifndef OS_ASM
#define STATUSTYPEDEFINED	1
typedef unsigned char StatusType;
#endif
#endif

#if !defined(RTE_TYPE_TickType) /* needed for RTE interaction */
#define RTE_TYPE_TickType
#ifndef OS_ASM
typedef os_tick_t	TickType;
#endif
#endif

#ifndef OS_ASM
typedef os_taskid_t			TaskType;
typedef os_taskid_t*		TaskRefType;
typedef os_taskstate_t      TaskStateType;
typedef os_taskstate_t*     TaskStateRefType;
typedef os_resourceid_t     ResourceType;
typedef os_eventmask_t      EventMaskType;
typedef os_eventmask_t*     EventMaskRefType;
typedef os_tick_t*          TickRefType;
typedef os_alarmbase_t      AlarmBaseType;
typedef os_alarmbase_t*     AlarmBaseRefType;
typedef os_alarmid_t        AlarmType;
typedef os_appmodeid_t      AppModeType;
typedef os_serviceid_t      OSServiceIdType;
#endif

/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX constructional elements
 *
 * !LINKSTO Kernel.API.TaskManagement.DeclareTask, 1
 * !LINKSTO Kernel.API.InterruptHandling.ISR, 1
 * !LINKSTO Kernel.API.ResourceManagement.DeclareResource, 1
 * !LINKSTO Kernel.API.EventControl.DeclareEvent, 1
 * !LINKSTO Kernel.API.Alarms.DeclareAlarm, 1
 * !LINKSTO Kernel.API.Alarms.AlarmCallback, 1
 *
 * Alarms, resources and events don't need an external declaration, but if we leave
 * the macros empty some compilers complain when working on very strict settings.
 * The previous solution of declaring a function prototype for OS_NullFunction()
 * also causes warnings when the "redundant redeclarations" warning in gcc is turned on.
 * Declaring a unique prototype for a non-existent function is also not desirable in case
 * some intelligent linker decides to warn for functions that are prototyped but don't
 * exist. The solution adopted is to declare a unique data type using typedef. This should be
 * harmless.
*/
#ifdef __cplusplus
#define OS_EXTERN_C extern "C"
#else
#define OS_EXTERN_C
#endif

/* Deviation MISRAC2012-1 */
#define TASK(x)				OS_EXTERN_C void OS_TASK_##x(void)
/* Deviation MISRAC2012-1 */
#define ALARMCALLBACK(x)	OS_EXTERN_C void OS_ACB_##x(void)
/* Deviation MISRAC2012-1 */
#define ISR(x)				OS_EXTERN_C void OS_ISR_##x(void)

#ifndef DeclareTask
/* Deviation MISRAC2012-1 <2> */
/* Possible diagnostic TOOLDIAG-1 <1> */
#define DeclareTask(x)		OS_EXTERN_C void OS_TASK_##x(void)
#endif
#ifndef DeclareResource
/* Deviation MISRAC2012-1 */
#define DeclareResource(x)	typedef os_int_t os_0##x
#endif
#ifndef DeclareEvent
/* Deviation MISRAC2012-1 */
#define DeclareEvent(x)		typedef os_int_t os_0##x
#endif
#ifndef DeclareAlarm
/* Deviation MISRAC2012-1 */
#define DeclareAlarm(x)		typedef os_int_t os_0##x
#endif


/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX API values in terms of OS
 * constants.
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.ResourcesBCC1, 1
*/
#define SUSPENDED			0
#define READY				1
#define RUNNING				2
#define WAITING				3
#define INVALID_TASK		OS_NULLTASK

/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX error codes in terms of
 * OS error codes.
 *
 * NOTE 1: The OS must define the correct values for E_OK --- E_OS_VALUE to
 * be OSEK-conformant.
 *
 * NOTE 2: E_OK is defined as 0 here, not OS_E_OK, to try to avoid conflicts in
 * Autosar. If it is only defined if not previously defined. There's also a sanity
 * check for it afterwards.
 *
 * !LINKSTO Kernel.API.StatusType, 1
 * !LINKSTO Kernel.API.StatusType.E_OK, 1
*/
#ifndef E_OK
#define E_OK			0
#endif
#define E_OS_ACCESS		OS_E_ACCESS
#define E_OS_CALLLEVEL	OS_E_CALLLEVEL
#define E_OS_CALLEVEL	OS_E_CALLLEVEL	/* Typo in spec? */
#define E_OS_ID			OS_E_ID
#define E_OS_LIMIT		OS_E_LIMIT
#define E_OS_NOFUNC		OS_E_NOFUNC
#define E_OS_RESOURCE	OS_E_RESOURCE
#define E_OS_STATE		OS_E_STATE
#define E_OS_VALUE		OS_E_VALUE

#if ( E_OK != 0 )
#error "E_OK is non-zero. This is not OSEK-conformant"
#endif

/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX API calls in
 * terms of the OS system-call stubs
 *
 * !LINKSTO Kernel.API.TaskManagement.ActivateTask.API, 1
 * !LINKSTO Kernel.API.TaskManagement.TerminateTask.API, 1
 * !LINKSTO Kernel.API.TaskManagement.ChainTask.API, 1
 * !LINKSTO Kernel.API.TaskManagement.Schedule.API, 1
 * !LINKSTO Kernel.API.TaskManagement.GetTaskID.API, 1
 * !LINKSTO Kernel.API.TaskManagement.GetTaskState.API, 1
 * !LINKSTO Kernel.API.ResourceManagement.GetResource.API, 1
 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.API, 1
 * !LINKSTO Kernel.API.EventControl.SetEvent.API, 1
 * !LINKSTO Kernel.API.EventControl.ClearEvent.API, 1
 * !LINKSTO Kernel.API.EventControl.GetEvent.API, 1
 * !LINKSTO Kernel.API.EventControl.WaitEvent.API, 1
 * !LINKSTO Kernel.API.Alarms.GetAlarmBase.API, 1
 * !LINKSTO Kernel.API.Alarms.GetAlarm.API, 1
 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.API, 1
 * !LINKSTO Kernel.API.Alarms.SetAbsAlarm.API, 1
 * !LINKSTO Kernel.API.Alarms.CancelAlarm.API, 1
 * !LINKSTO Kernel.API.OSControl.GetActiveApplicationMode.API, 1
 * !LINKSTO Kernel.API.OSControl.StartOS.API, 1
 * !LINKSTO Kernel.API.OSControl.ShutdownOS.API, 1
*/
#define ActivateTask(t)				OS_UserActivateTask(t)
#define TerminateTask()				OS_UserTerminateTask()
#define ChainTask(t)				OS_UserChainTask(t)
#define Schedule()					OS_UserSchedule()
#define GetTaskID(tr)				OS_UserGetTaskId(tr)
#define GetTaskState(t,sr)			OS_GetTaskState((t),(sr))
#define GetResource(r)				OS_UserGetResource(r)
#define ReleaseResource(r)			OS_UserReleaseResource(r)
#define SetEvent(t,e)				OS_UserSetEvent((t),(e))
#define ClearEvent(e)				OS_UserClearEvent(e)
#define GetEvent(t,er)				OS_UserGetEvent((t),(er))
#define WaitEvent(e)				OS_UserWaitEvent(e)
#define GetAlarmBase(a,br)			OS_UserGetAlarmBase((a),(br))
#define GetAlarm(a,tr)				OS_UserGetAlarm((a),(tr))
#define SetRelAlarm(a,i,c)			OS_UserSetRelAlarm((a),(i),(c))
#define SetAbsAlarm(a,i,c)			OS_UserSetAbsAlarm((a),(i),(c))
#define CancelAlarm(a)				OS_UserCancelAlarm(a)
#define GetActiveApplicationMode()	OS_UserGetActiveApplicationMode()
#define ShutdownOS(err)				OS_UserShutdownOs(err)

#ifdef OS_ARCH_USERSTARTOS
#define StartOS(m)					OS_ArchUserStartOs(m)
#else
#define StartOS(m)					OS_UserStartOs(m)
#endif

/*!
 * OSEK/VDX API
 *
 * These macros define the OSEK/VDX Service IDs in
 * terms of the OS SIDs
 *
 * !LINKSTO Kernel.API.Hooks.Constants, 1
*/
#define OSServiceId_ActivateTask			OS_SID_ActivateTask
#define OSServiceId_TerminateTask			OS_SID_TerminateTask
#define OSServiceId_ChainTask				OS_SID_ChainTask
#define OSServiceId_GetTaskState			OS_SID_GetTaskState
#define OSServiceId_GetTaskID				OS_SID_GetTaskID
#define OSServiceId_Schedule				OS_SID_Schedule
#define OSServiceId_GetResource				OS_SID_GetResource
#define OSServiceId_ReleaseResource			OS_SID_ReleaseResource
#define OSServiceId_SetEvent				OS_SID_SetEvent
#define OSServiceId_ClearEvent				OS_SID_ClearEvent
#define OSServiceId_WaitEvent				OS_SID_WaitEvent
#define OSServiceId_GetEvent				OS_SID_GetEvent
#define OSServiceId_GetAlarm				OS_SID_GetAlarm
#define OSServiceId_GetAlarmBase			OS_SID_GetAlarmBase
#define OSServiceId_SetRelAlarm				OS_SID_SetRelAlarm
#define OSServiceId_SetAbsAlarm				OS_SID_SetAbsAlarm
#define OSServiceId_CancelAlarm				OS_SID_CancelAlarm
#define OSServiceId_SuspendOSInterrupts		OS_SID_SuspendInterrupts
#define OSServiceId_ResumeOSInterrupts		OS_SID_ResumeInterrupts
#define OSServiceId_SuspendAllInterrupts	OS_SID_SuspendInterrupts
#define OSServiceId_ResumeAllInterrupts		OS_SID_ResumeInterrupts
#define OSServiceId_DisableAllInterrupts	OS_SID_SuspendInterrupts
#define OSServiceId_EnableAllInterrupts		OS_SID_ResumeInterrupts
#define OSServiceId_StartOS					OS_SID_StartOs
#define OSServiceId_ShutdownOS				OS_SID_ShutdownOs

/*!
 * OSEK/VDX API
 *
 * The OSErrorGetServiceId() macro returns the service ID of the OSEK
 * service in which the error occurred. The value is not meaningful
 * outside the error hook, and its use outside the error hook might
 * cause a protection fault, depending on the context and configuration.
 *
 * The OSError_SIDXX_PARAMXX() macros return the value of the specified
 * parameter to the service. The return value is only meaningful inside
 * the error hook, and then only if the service ID is that which caused
 * the error.
 *
 * !LINKSTO Kernel.API.Hooks.Macros, 1
*/
#define OSErrorGetServiceId()			(OS_GetErrorInfo()->serviceId)

/* CHECK: SAVE
 * CHECK: RULE 306 OFF (Looks nicer)
*/
#define OSError_ActivateTask_TaskID()	((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_ChainTask_TaskID()		((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetTaskID_TaskID()		((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetTaskState_TaskID()	((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetTaskState_State()	((TaskStateRefType)	(OS_GetErrorInfo()->parameter[1]))
#define OSError_GetResource_ResID()		((ResourceType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_ReleaseResource_ResID()	((ResourceType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_SetEvent_TaskID()		((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_SetEvent_Mask()			((EventMaskType)	(OS_GetErrorInfo()->parameter[1]))
#define OSError_ClearEvent_Mask()		((EventMaskType)	(OS_GetErrorInfo()->parameter[0]))
#define OSError_WaitEvent_Mask()		((EventMaskType)	(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetEvent_TaskID()		((TaskType)			(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetEvent_Event()		((EventMaskRefType)	(OS_GetErrorInfo()->parameter[1]))
#define OSError_GetAlarm_AlarmID()		((AlarmType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetAlarm_Tick()			((TickRefType)		(OS_GetErrorInfo()->parameter[1]))
#define OSError_GetAlarmBase_AlarmID()	((AlarmType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_GetAlarmBase_Info()		((AlarmBaseRefType)	(OS_GetErrorInfo()->parameter[1]))
#define OSError_SetRelAlarm_AlarmID()	((AlarmType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_SetRelAlarm_increment()	((TickType)			(OS_GetErrorInfo()->parameter[1]))
#define OSError_SetRelAlarm_cycle()		((TickType)			(OS_GetErrorInfo()->parameter[2]))
#define OSError_SetAbsAlarm_AlarmID()	((AlarmType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_SetAbsAlarm_start()		((TickType)			(OS_GetErrorInfo()->parameter[1]))
#define OSError_SetAbsAlarm_cycle()		((TickType)			(OS_GetErrorInfo()->parameter[2]))
#define OSError_CancelAlarm_AlarmID()	((AlarmType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_StartOS_Mode()			((AppModeType)		(OS_GetErrorInfo()->parameter[0]))
#define OSError_ShutdownOS_Error()		((StatusType)		(OS_GetErrorInfo()->parameter[0]))
/* CHECK: RESTORE
*/

#ifndef OS_ASM
/* Library routines to map some OS system calls onto OSEK/VDX APIs
*/
StatusType OS_GetTaskState(TaskType, TaskStateRefType);
#endif

/* Autosar is an extension of OSEK
 *
 * !LINKSTO Kernel.Autosar.OSEK, 1
*/

/*!
 * AUTOSAR API
 *
 * These macros define the AUTOSAR data types in terms of
 * OSEMKP data types.
 *
 * !LINKSTO Kernel.Autosar.API.DataTypes.ApplicationType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.TrustedFunctionIndexType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.TrustedFunctionParameterRefType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.AccessType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ObjectAccessType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ObjectTypeType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.MemoryStartAddressType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.MemorySizeType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ISRType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ScheduleTableType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ScheduleTableStatusType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ScheduleTableStatusRefType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.CounterType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ProtectionReturnType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.RestartType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.CoreIdType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.SpinlockIdType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.TryToGetSpinlockType, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.PhysicalTimeType, 1
*/

#if !defined(RTE_TYPE_CounterType) /* needed for RTE interaction */
#define RTE_TYPE_CounterType
#ifndef OS_ASM
typedef os_int32_t CounterType;
#endif
#endif

#ifndef OS_ASM
typedef os_applicationid_t		ApplicationType;
typedef os_appstate_t           ApplicationStateType;
typedef os_appstate_t*          ApplicationStateRefType;
typedef os_functionid_t         TrustedFunctionIndexType;
typedef void*                   TrustedFunctionParameterRefType;
typedef os_memoryaccess_t       AccessType;
typedef os_boolean_t            ObjectAccessType;
typedef os_objecttype_t         ObjectTypeType;
typedef void*                   MemoryStartAddressType;
typedef os_size_t               MemorySizeType;
typedef os_isrid_t              ISRType;
typedef os_scheduleid_t         ScheduleTableType;
typedef os_schedulestatus_t     ScheduleTableStatusType;
typedef os_schedulestatus_t*    ScheduleTableStatusRefType;
typedef os_erroraction_t        ProtectionReturnType;
typedef os_restart_t            RestartType;
typedef os_physicaltime_t       PhysicalTimeType;
typedef os_coreid_t             CoreIdType;
typedef os_autosarspinlockid_t  SpinlockIdType;
typedef os_tryspinlock_t        TryToGetSpinlockType;
#endif

/*!
 * AUTOSAR API
 *
 * Constructional elements
 *
 * TRUSTED() isn't really part of the Autosar API, but we define it here
 * anyway. t is the name of the function, i and p are the formal parameter
 * names
 *
 * !LINKSTO Kernel.Autosar.TrustedFunctions.Prototype, 2
*/
/* Deviation MISRAC2012-1 */
#define TRUSTED(t,i,p)	OS_EXTERN_C void TRUSTED_##t(TrustedFunctionIndexType (i), TrustedFunctionParameterRefType (p))

/*!
 * AUTOSAR API
 *
 * These macros define the AUTOSAR API values in terms of the kernel's own
 * API constants.
 *
 * !LINKSTO Kernel.Autosar.API.DataTypes.ApplicationType.Constants, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ObjectAccessType.Constants, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ObjectTypeType.Constants, 2
 * !LINKSTO Kernel.Autosar.API.DataTypes.ISRType.Constants, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.ScheduleTableStatusType.Constants, 2
 * !LINKSTO Kernel.Autosar.API.DataTypes.ProtectionReturnType.Constants, 2
 * !LINKSTO Kernel.Autosar.API.DataTypes.RestartType.Constants, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.SpinlockIdType.Constants, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.TryToGetSpinlockType.Constants, 1
*/
#define INVALID_OSAPPLICATION		OS_NULLAPP
#define ACCESS						OS_TRUE
#define NO_ACCESS					OS_FALSE
#define OBJECT_TASK					OS_OBJ_TASK
#define OBJECT_ISR					OS_OBJ_ISR
#define OBJECT_ALARM				OS_OBJ_ALARM
#define OBJECT_RESOURCE				OS_OBJ_RESOURCE
#define OBJECT_COUNTER				OS_OBJ_COUNTER
#define OBJECT_SCHEDULETABLE		OS_OBJ_SCHEDULETABLE
#define INVALID_ISR					OS_NULLISR
#define SCHEDULETABLE_STOPPED		OS_STE_STOPPED
#define SCHEDULETABLE_NEXT			OS_STE_NEXT
#define SCHEDULETABLE_WAITING		OS_STE_WAITING
#define SCHEDULETABLE_RUNNING		OS_STE_ASYNCHRONOUS
#define SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS	OS_STE_SYNCHRONOUS
#define PRO_IGNORE					OS_ACTION_DONOTHING
#define PRO_TERMINATETASKISR		OS_ACTION_KILL
#define PRO_TERMINATEAPPL			OS_ACTION_QUARANTINEAPP
#define PRO_TERMINATEAPPL_RESTART	OS_ACTION_RESTART
#define PRO_SHUTDOWN				OS_ACTION_SHUTDOWN
#define RESTART						OS_APPL_RESTART
#define NO_RESTART					OS_APPL_NO_RESTART
#define DONOTCARE					OS_NULLAPPMODE
#define INVALID_SPINLOCK			OS_NULLAUTOSARSPINLOCK
#define TRYTOGETSPINLOCK_NOSUCCESS	OS_TRYTOGETSPINLOCK_NOSUCCESS
#define TRYTOGETSPINLOCK_SUCCESS	OS_TRYTOGETSPINLOCK_SUCCESS

/*!
 * AUTOSAR API
 *
 * These macros define the AUTOSAR error codes in terms of OS error codes.
 *
 * !LINKSTO Kernel.Autosar.API.ErrorCodes, 2
*/
#define E_OS_SERVICEID				OS_E_TFID
#define E_OS_ILLEGAL_ADDRESS		OS_E_ADDRESS
#define E_OS_MISSINGEND				OS_E_TASKRETURN
#define E_OS_DISABLEDINT			OS_E_INTDISABLE
#define E_OS_STACKFAULT				OS_E_STACKPROT
#define E_OS_PROTECTION_MEMORY		OS_E_MEMPROT
#define E_OS_PROTECTION_TIME		OS_E_TIMEPROT
#define E_OS_PROTECTION_ARRIVAL		OS_E_RATEPROT
#define E_OS_PROTECTION_LOCKED		OS_E_LOCKPROT
#define E_OS_PROTECTION_EXCEPTION	OS_E_EXCEPTPROT
#define E_OS_SPINLOCK				OS_E_SPINLOCK
#define E_OS_CORE					OS_E_CORE
#define E_OS_NESTING_DEADLOCK		OS_E_NESTING_DEADLOCK
#define E_OS_INTERFERENCE_DEADLOCK	OS_E_INTERFERENCE_DEADLOCK

/*!
 * AUTOSAR API
 *
 * Some required data-access macros.
 *
 * !LINKSTO Kernel.Autosar.API.DataTypes.AccessType.Macros.IS_READABLE, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.AccessType.Macros.IS_WRITEABLE, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.AccessType.Macros.IS_EXECUTABLE, 1
 * !LINKSTO Kernel.Autosar.API.DataTypes.AccessType.Macros.IS_STACKSPACE, 1
*/
/* Autosar 1.3 */
#define OSMEMORY_IS_READABLE(x)		(((x)&OS_MA_READ)!=0)
#define OSMEMORY_IS_WRITEABLE(x)	(((x)&OS_MA_WRITE)!=0)
#define OSMEMORY_IS_EXECUTABLE(x)	(((x)&OS_MA_EXEC)!=0)
#define OSMEMORY_IS_STACKSPACE(x)	(((x)&OS_MA_STACK)!=0)

/*!
 * AUTOSAR API
 *
 * These macros define the AUTOSAR API calls in
 * terms of the OS system-call stubs
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetApplicationID, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetISRID, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.CallTrustedFunction, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckISRMemoryAccess, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckTaskMemoryAccess, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectAccess, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckObjectOwnership, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StopScheduleTable, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.ChainScheduleTable, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.IncrementCounter, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetScheduleTableStatus, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.TerminateApplication, 3
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetCounterValue, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetElapsedCounterValue, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetNumberOfActivatedCores, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetCoreID, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartCore, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.ShutdownAllCores, 1
*/
#define IncrementCounter(c)				OS_UserIncrementCounter(c)

#define GetCounterValue(c,vr)			OS_UserGetCounterValue((c),(vr))

/* AUTOSAR 4.0 renamed GetElapsedCounterValue - we provide both names */
#define GetElapsedCounterValue(c,p,vr)	OS_UserGetElapsedCounterValue((c),(p),(vr))
#define GetElapsedValue(c,p,vr)			OS_UserGetElapsedCounterValue((c),(p),(vr))
#define StartScheduleTableRel(t,o)		OS_UserStartScheduleTable((t),(o),1)
#define StartScheduleTableAbs(t,o)		OS_UserStartScheduleTable((t),(o),0)
#define StartScheduleTableSynchron(t)	OS_UserStartScheduleTableSynchron(t)
#define NextScheduleTable(c,n)			OS_UserChainScheduleTable((c),(n))
#define StopScheduleTable(t)			OS_UserStopScheduleTable(t)
#define SyncScheduleTable(t,g)			OS_UserSyncScheduleTable((t),(g))
#define SetScheduleTableAsync(s)		OS_UserSetScheduleTableAsync(s)
#define GetScheduleTableStatus(t,sr)	OS_GetScheduleTableStatus((t),(sr))
#define TerminateApplication(a,r)			OS_UserTerminateApplication((a),(r))
#define GetApplicationID()				OS_UserGetApplicationId()
#define GetApplicationState(a, out)		OS_UserGetApplicationState((a), (out))
#define GetISRID()						OS_UserGetIsrId()
#define CallTrustedFunction(f,p)		OS_UserCallTrustedFunction((f),(p))
#define CheckISRMemoryAccess(i,b,l)		OS_UserCheckIsrMemoryAccess((i),(b),(l))
#define CheckTaskMemoryAccess(t,b,l)	OS_UserCheckTaskMemoryAccess((t),(b),(l))
#define CheckObjectAccess(a,t,o)		OS_UserCheckObjectAccess((a),(t),(o))
#define CheckObjectOwnership(t,o)		OS_UserCheckObjectOwnership((t),(o))
#define AllowAccess()					OS_UserAllowAccess()
#define GetCoreID()							OS_GetCoreId()
#define GetNumberOfActivatedCores()			OS_N_CORES


/*!
 * AUTOSAR API
 *
 * These macros define the AUTOSAR Service IDs in
 * terms of the OS SIDs
*/
#define OSServiceID_IncrementCounter		OS_SID_IncrementCounter
#define OSServiceID_StartScheduleTableRel	OS_SID_StartScheduleTableRel
#define OSServiceID_StartScheduleTableAbs	OS_SID_StartScheduleTableAbs
#define OSServiceID_StartScheduleTableSynchron	OS_SID_StartScheduleTableSynchron
#define OSServiceID_NextScheduleTable		OS_SID_ChainScheduleTable
#define OSServiceID_StopScheduleTable		OS_SID_StopScheduleTable
#define OSServiceID_SyncScheduleTable		OS_SID_SyncScheduleTable
#define OSServiceID_SetScheduleTableAsync	OS_SID_SetScheduleTableAsync
#define OSServiceID_GetScheduleTableStatus	OS_SID_GetScheduleTableStatus
#define OSServiceID_TerminateApplication	OS_SID_TerminateApplication
#define OSServiceID_GetApplicationId		OS_SID_GetApplicationId
#define OSServiceID_GetIsrId				OS_SID_GetIsrId
#define OSServiceID_CallTrustedFunction		OS_SID_CallTrustedFunction
#define OSServiceID_CheckIsrMemoryAccess	OS_SID_CheckIsrMemoryAccess
#define OSServiceID_CheckTaskMemoryAccess	OS_SID_CheckTaskMemoryAccess
#define OSServiceID_CheckObjectAccess		OS_SID_CheckObjectAccess
#define OSServiceID_CheckObjectOwnership	OS_SID_CheckObjectOwnership
#define OSServiceID_GetCounterValue			OS_SID_GetCounterValue
#define OSServiceID_GetElapsedCounterValue	OS_SID_GetElapsedCounterValue
#define OSServiceID_GetElapsedValue			OS_SID_GetElapsedCounterValue

/* CHECK: SAVE
 * CHECK: RULE 306 OFF (Looks nicer with long lines)
 *
 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.ErrorHook.Call.OsErrorMacros, 1
*/
#define OSError_IncrementCounter_CounterID()				((CounterType)						(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_StartScheduleTableRel_ScheduleTableID()		((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_StartScheduleTableRel_Offset()				((TickType)							(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_StartScheduleTableAbs_ScheduleTableID()		((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_StartScheduleTableAbs_Tickvalue()			((TickType)							(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_ChainScheduleTable_SchedTableID_current()	((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_ChainScheduleTable_SchedTableID_next()		((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_StopScheduleTable_ScheduleTableID()			((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_SyncScheduleTable_SchedTableID()			((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_SyncScheduleTable_GlobalTime()				((TickType)							(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_SetScheduleTableAsync_ScheduleID()			((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetScheduleTableStatus_ScheduleID()			((ScheduleTableType)				(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetScheduleTableStatus_ScheduleStatus()		((ScheduleTableStatusRefType)		(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_CallTrustedFunction_FunctionIndex()			((TrustedFunctionIndexType)			(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_CallTrustedFunction_FunctionParams()		((TrustedFunctionParameterRefType)	(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_CheckIsrMemoryAccess_ISRID()				((ISRType)							(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_CheckIsrMemoryAccess_Address()				((MemoryStartAddressType)			(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_CheckIsrMemoryAccess_Size()					((MemorySizeType)					(OS_GetKernelData()->errorStatus.parameter[2]))
#define OSError_CheckTaskMemoryAccess_TaskID()				((TaskType)							(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_CheckTaskMemoryAccess_Address()				((MemoryStartAddressType)			(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_CheckTaskMemoryAccess_Size()				((MemorySizeType)					(OS_GetKernelData()->errorStatus.parameter[2]))
#define OSError_CheckObjectAccess_ApplID()					((ApplicationType)					(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_CheckObjectAccess_ObjectType()				((ObjectTypeType)					(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_CheckObjectAccess_Object()					((os_objectid_t)					(OS_GetKernelData()->errorStatus.parameter[2]))
#define OSError_CheckObjectOwnership_ObjectType()			((ObjectTypeType)					(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_CheckObjectOwnership_Object()				((os_objectid_t)					(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_GetCounterValue_CounterID()					((CounterType)						(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetCounterValue_Value()						((TickRefType)						(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_StartScheduleTableSynchron_ScheduleTableID()	((ScheduleTableType)			(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetElapsedCounterValue_CounterID()			((CounterType)						(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetElapsedCounterValue_PreviousValue()		((TickType)							(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_GetElapsedCounterValue_Value()				((TickRefType)						(OS_GetKernelData()->errorStatus.parameter[2]))
/* API may be referenced as GetElapsedCounterValue (<= 3.1) or as GetElapsedValue (>= 4.0) */
#define OSError_GetElapsedValue_CounterID()					((CounterType)						(OS_GetKernelData()->errorStatus.parameter[0]))
#define OSError_GetElapsedValue_PreviousValue()				((TickType)							(OS_GetKernelData()->errorStatus.parameter[1]))
#define OSError_GetElapsedValue_Value()						((TickRefType)						(OS_GetKernelData()->errorStatus.parameter[2]))
/* CHECK: RESTORE
*/

#ifndef OS_ASM
/* Library routines to map some OS system calls onto AUTOSAR APIs
*/
StatusType OS_GetScheduleTableStatus(ScheduleTableType, ScheduleTableStatusRefType);
CoreIdType OS_GetCoreId(void);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* OS_API_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
