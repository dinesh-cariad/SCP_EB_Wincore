/* Os_proto_WINLIN.h
 *
 * Function prototypes for WINLIN.
 *
 * The prototypes of kernel functions are in a separate file because they
 * are included in the kernel/os-kernel.h file AFTER the generic types
 * have been defined, whereas the architecture-dependent types file is
 * included BEFORE the generic types are defined.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_proto_WINLIN.h 30052 2019-03-27 14:30:23Z olme8414 $
*/

#ifndef OS_PROTO_WINLIN_H
#define OS_PROTO_WINLIN_H

#include <WINLIN/Os_api_WINLIN_shared.h>

#ifndef OS_ASM

extern void OS_WINLINCoreEventLoop(void);
extern void OS_WINLINExecuteSyscall(os_syscallbuffer_t*);
extern os_winlinresult_t OS_WINLINSyscall(
		os_serviceid_t,
		os_paramtype_t,
		os_paramtype_t,
		os_paramtype_t,
		os_paramtype_t);

#if (OS_ARCH == OS_WINDOWS)
extern os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINTaskThread(void*);
extern os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINIsrThread(void*);
#elif (OS_ARCH == OS_LINUX)
extern void* OS_WINLINTaskThread(void*);
extern void* OS_WINLINIsrThread(void*);
#else
#error "Unsupported architecture. Please check your makefiles."
#endif

extern void OS_WINLINPanic(void);
extern os_boolean_t OS_WINLINIsThisACoreThread(void);
extern os_intstatus_t OS_WINLINUseHostOsService(
		os_winlinUseHostOsService_t,
		os_intstatus_t);
extern void OS_WINLINFinishedIrq(os_isr_t const*);
extern void OS_WINLINKernFinishedIrq(os_isr_t const*);
extern os_intstatus_t OS_WINLINSetInterruptDisableLevel(
		os_intstatus_t,
		os_setInterruptDisableLevelMode_t);
extern void OS_WINLINSetupIsr(os_isr_t const*);
extern void OS_WINLINInitTimeStamp(void);
extern void OS_WINLINKernClearInterrupt(os_isrid_t);
extern os_char_t const* OS_WINLINGetStartOfFilename(os_char_t const*);
extern os_boolean_t OS_WINLINIsThisASystemThread(void);
extern void OS_WINLINInitializeLogging(void);
extern void OS_WINLINLogObservedIsrs(os_isr_t const* const associatedIsrs[], os_size_t);
extern void OS_WINLINLogSyscall(os_syscallport_t const*);
extern os_size_t OS_WINLINAssembleIsrEvents(
		os_winlinHandle_t eventArray[],
		os_isr_t const* associatedIsrs[],
		os_size_t,
		os_boolean_t);
extern os_boolean_t OS_WINLINAreThereAnyPendingIrqsToServe(os_boolean_t);
extern void OS_WINLINResetSyscallEventsOfAllSyscallPorts(void);
extern void OS_WINLINTriggerInterruptViaSyscall(os_isr_t const*);
extern void OS_WINLINKernTriggerInterruptViaSyscall(os_isr_t const*);
extern void OS_WINLINTaskReturn(void);

/* Wrappers for Win32-API */

extern void OS_WINLIN_OSAL_Init(void);
extern os_winlinTlsIndex_t OS_WINLIN_OSAL_TlsAlloc(void);
extern void OS_WINLIN_OSAL_TlsSetValue(os_winlinTlsIndex_t, void *);
extern void * OS_WINLIN_OSAL_TlsGetValue(os_winlinTlsIndex_t);
extern os_winlinTid_t OS_WINLIN_OSAL_GetCurrentThreadId(void);
extern os_winlinHandle_t OS_WINLIN_OSAL_CreateThread(
		os_winlinThreadEntry_t,
		void*,
		os_winlinTid_t*,
		os_char_t const*,
		os_winlinThreadType_t);
extern void OS_WINLIN_OSAL_TerminateCurrentProcess(os_winlinDword_t);
extern os_winlinDword_t OS_WINLIN_OSAL_SuspendThread(os_winlinHandle_t);
extern os_winlinDword_t OS_WINLIN_OSAL_SuspendThreadHelper(
		os_winlinHandle_t,
		os_boolean_t);
extern os_winlinDword_t OS_WINLIN_OSAL_ResumeThread(os_winlinHandle_t);
extern os_winlinHandle_t OS_WINLIN_OSAL_CreateEvent(os_boolean_t, os_boolean_t);
extern void OS_WINLIN_OSAL_SetEvent(os_winlinHandle_t);
extern void OS_WINLIN_OSAL_ResetEvent(os_winlinHandle_t);
extern os_boolean_t OS_WINLIN_OSAL_IsEventSet(os_winlinHandle_t);
extern void OS_WINLIN_OSAL_SignalEventAndWait(
		os_winlinHandle_t,
		os_winlinHandle_t);
extern os_winlinDword_t OS_WINLIN_OSAL_WaitForMultipleEvents(
		os_winlinDword_t,
		os_winlinHandle_t const*,
		os_boolean_t);
extern void OS_WINLIN_OSAL_WaitForSingleEvent(os_winlinHandle_t);
extern os_winlinTimerValue_t OS_WINLIN_OSAL_QueryPerformanceCounter(void);
extern os_winlinTimerValue_t OS_WINLIN_OSAL_QueryPerformanceFrequency(void);
extern os_boolean_t OS_WINLIN_OSAL_IsPerformanceCounterAvailable(void);
extern void OS_WINLIN_OSAL_DisablePriorityBoostOfThisProcess(void);
extern void OS_WINLIN_OSAL_MakeThisThreadVeryImportant(void);
extern void OS_WINLIN_OSAL_AtExit(void (*func)(void));
extern void OS_WINLIN_OSAL_WaitForThreadToBecomeBlocked(os_winlinHandle_t);
extern void OS_WINLIN_OSAL_InitSpinlock(os_syncspot_t*);
extern void OS_WINLIN_OSAL_TakeSpinlock(os_syncspot_t*);
extern void OS_WINLIN_OSAL_DropSpinlock(os_syncspot_t*);

/* Time API */

extern void OS_WINLIN_OSAL_CreateWaitableTimer(os_winlinHandle_t*, os_winlinTid_t);
extern void OS_WINLIN_OSAL_SetWaitableTimer(
		os_winlinHandle_t,
		os_winlinTimerValue_t);
extern void OS_WINLIN_OSAL_CancelWaitableTimer(os_winlinHandle_t);
extern os_winlinDword_t OS_WINLIN_OSAL_WaitForWaitableTimers(os_winlinDword_t, os_winlinHandle_t const*);
extern os_winlinDword_t OS_WINLIN_OSAL_TimeGetTime(void);
extern void OS_WINLIN_OSAL_TimeBeginPeriod(os_uint_t);
extern void OS_WINLIN_OSAL_TimeEndPeriod(os_uint_t);
extern os_uint_t OS_WINLIN_OSAL_GetMinimumPeriod(void);

#endif /* !OS_ASM */

/** \brief The maximum number of handles, which may be passed to OS_WINLIN_OSAL_WaitForMultipleEvents(). */
#define OS_WINLIN_MAXIMUM_WAIT_OBJECTS	64U

#endif OS_PROTO_WINLIN_H

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
