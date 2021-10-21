/* WINLIN-syscall.c
 *
 * This file contains the OS_WINLINSyscall function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-syscall.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

#if OS_USE_TRACE
/* The macro OS_TRACE_CALLTRUSTEDFUNCTION_EXIT_P() is used below to handle
 * trusted function calls. This macro depends on the following definitions,
 * when tracing is actually used. Hence, these macros were copied from
 * kern-calltrustedfunction.c to here.
 */
#define OS_SID	OS_SID_CallTrustedFunction
#define OS_SIF	OS_svc_CallTrustedFunction
#endif

#include <Os_api.h>
#include <Os_kernel.h>
#include <Os_syscalls.h>
#include <WINLIN/Os_WINLIN_syscalls.h>
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C requires a declaration for each definition. */
static void OS_WINLINHandleTaskTermination(os_syscallbuffer_t* pSyscallBuffer);
static os_boolean_t OS_WINLINHandlePreOsStartSyscalls(os_serviceid_t syscallId);
static void OS_WINLINHandleTrustedFunctionCalls(os_syscallbuffer_t* pSyscallBuffer);
static void OS_WINLINWarnAboutCertainSyscalls(os_serviceid_t syscallId);

/** \brief Commissions a core thread to do a system call.
 *
 * This function is called from the user threads via the syscall wrappers.
 * It requests the execution of said syscall by the core's respective kernel
 * thread, if the caller is a task or ISR. Otherwise the respective system
 * call is directly run by the resp. core thread itself.
 *
 * Not always are all parameters necessary for a system call. The number of
 * parameters offered by this function is just the maximum of all defined
 * system calls of AutoCore OS. If parameters are not used for a specific
 * system call, their concrete values do not matter.
 *
 * \param[in]	syscallId	The ID of the commissioned system call.
 * \param[in]	p1			The first parameter of the system call.
 * \param[in]	p2			The second parameter of the system call.
 * \param[in]	p3			The third parameter of the system call.
 * \param[in]	p4			The fourth parameter of the system call.
 *
 * \return The return value of the executed system call identified by \a syscallId.
 */
os_winlinresult_t OS_WINLINSyscall(
		os_serviceid_t syscallId,
		os_paramtype_t p1,
		os_paramtype_t p2,
		os_paramtype_t p3,
		os_paramtype_t p4)
{
	os_winlinresult_t res = OS_E_OK;

	OS_WINLINWarnAboutCertainSyscalls(syscallId);

	if (!OS_WINLINHandlePreOsStartSyscalls(syscallId))
	{
		os_syscallport_t hookSyscallPort;
		os_syscallport_t* pSyscallPort = OS_NULL;
		os_syscallbuffer_t* pSyscallBuffer = OS_NULL;
		os_boolean_t commissionCoreThread = OS_TRUE;

		pSyscallPort = OS_WINLINGetSyscallPortOfCurrentThread();
		/* Note, that the Windows-specific syscall OS_SC_WINLINFinishedIrq must be handled
		 * specially, because is never handled in the context of a task or ISR. Hence, we
		 * can't rely upon the return value of OS_GetKernelData()->inFunction. This is because, the
		 * functions OS_Cat<N>Entry() restore this value at exit to the value it was at
		 * entry and when we reach this function, we always query the restored value, never
		 * OS_INCAT<N>. It might be OS_INTASK, when a currently running task is interrupted.
		 */
		if (syscallId != OS_SC_WINLINFinishedIrq)
		{
			/* If a hook function makes a syscall we bypass the usual mechanism,
			 * because hook functions are executed by core threads directly. If
			 * the usual mechanism was used, there would be no one (i.e. a core thread)
			 * to answer the syscall.
			 *
			 * The same applies, when the OS has not yet been started on the core on
			 * which a syscall is initiated. In this case, there wouldn't be a core
			 * thread answering the syscall.
			 */
			os_uint8_t const inFunction = OS_GetKernelData()->inFunction;
			if (OS_WINLINInHookOrAlarmCallback(inFunction)
				|| (!OS_WINLINWasOsStartedOnCore(OS_GetMyCoreId())))
			{
				/* Mark handle as invalid. This is important in OS_WINLINServeSyscall(). */
				OS_WINLINInitializeSyscallPortAsInvalid(&hookSyscallPort);
				pSyscallPort = &hookSyscallPort;
				commissionCoreThread = OS_FALSE;
			}
		}
		OS_WINLINAssert(pSyscallPort != OS_NULL);
		pSyscallBuffer = OS_WINLINGetSyscallBuffer(pSyscallPort);

		/* Copy the system call's arguments into the syscall port's buffer. */
		pSyscallBuffer->syscallId = syscallId;
		pSyscallBuffer->p1        = p1;
		pSyscallBuffer->p2        = p2;
		pSyscallBuffer->p3        = p3;
		pSyscallBuffer->p4        = p4;
		pSyscallBuffer->rv        = 0u;

		/* Depending on the type of the caller the respective core thread is engaged to
		 * perform the system call or it is directly executed. The former happens, when
		 * the caller is a task or ISR. If we are currently running a hook or callback
		 * function, the system call is directly executed, because such functions are
		 * directly run by a core thread itself.
		 */
		if (commissionCoreThread)
		{
			os_task_t const* const syscallTask = OS_GetKernelData()->taskCurrent;
			os_uint8_t const inFunction = OS_GetKernelData()->inFunction;

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
			/* It shall be avoided to interrupt Pthread functions, which are used here and in the
			 * signal handler OS_LINUXPosixSignalHandler(). The reason for this is to avoid potential
			 * re-entrance issues. By blocking the signal OS_LINUX_POSIX_SIGNAL used by this application to
			 * suspend task and ISR threads, this goal can be achieved. Well, only partly, because pthread_sigmask()
			 * used by OS_LINUXControlBlockageOfPosixSignal() might still be interrupted at the time it unblocks
			 * OS_LINUX_POSIX_SIGNAL again. This seems to be no issue, though.
			 */
			OS_LINUXControlBlockageOfPosixSignal(OS_TRUE);
#endif

			OS_WINLINAssert(!OS_WINLIN_OSAL_IsEventSet(pSyscallPort->syscallFinishedEvent));
			OS_WINLIN_OSAL_SignalEventAndWait(
					pSyscallPort->syscallInitiatedEvent,
					pSyscallPort->syscallFinishedEvent);
			OS_WINLIN_OSAL_ResetEvent(pSyscallPort->syscallFinishedEvent);

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
			OS_LINUXControlBlockageOfPosixSignal(OS_FALSE);
#endif

			/* Note, when the following condition is false, this thread must be an ISR
			 * thread, which was just restarted. This is because an ISR thread blocks
			 * on pSyscallPort->syscallFinishedEvent when the ISR has run to completion
			 * and is ready to serve the next interrupt request. This next request is
			 * denoted by raising pSyscallPort->syscallFinishedEvent.
			 */
			if (syscallId != OS_SC_WINLINFinishedIrq)
			{
				if ((syscallTask != OS_NULL) && (inFunction == OS_INTASK))
				{
					OS_WINLINHandleTaskTermination(pSyscallBuffer);
				}

				if (syscallId == OS_SC_CallTrustedFunction)
				{
					OS_WINLINHandleTrustedFunctionCalls(pSyscallBuffer);
				}
			}
		}
		else
		{
			OS_WINLINExecuteSyscall(pSyscallBuffer);
		}

		res = pSyscallBuffer->rv;

		/* Reset the syscall buffer just to catch problems earlier. */
		pSyscallBuffer->syscallId = OS_SC_UnknownSyscall;
	}

	return res;
}

/** \brief Handles trusted function calls.
 *
 * Trusted functions are not called inside the kernel, but in the context of
 * the original caller. This is necessary, because trusted functions may do
 * syscalls and hence, there must be a core thread to serve them. Also triggering
 * interrupts from trusted functions is a problem, when they were executed by
 * core threads. Task and ISR callers are properly synchronized in
 * OS_WINLINTriggerInterrupt().
 *
 * \pre The given syscall buffer pointed to by \a pSyscallBuffer must reflect
 * the trusted function call to handle.
 *
 * \param[in] pSyscallBuffer	The syscall buffer used to initiate the trusted function call.
 */
static void OS_WINLINHandleTrustedFunctionCalls(os_syscallbuffer_t* pSyscallBuffer)
{
	OS_WINLINAssert(pSyscallBuffer->syscallId == OS_SC_CallTrustedFunction);

	if (pSyscallBuffer->rv == OS_E_OK)
	{
		os_functionid_t const fid = (os_functionid_t)pSyscallBuffer->p1;
		void* const parms = (void*)pSyscallBuffer->p2;
		os_function_t const* const fs = &OS_functionTableBase[fid];

		(fs->function)(fid, parms);

		/* Cat2 ISRs must be disabled, before the following function is called.
		 * This is because trusted functions may be used by Cat2 ISRs too and because
		 * the following operation is not thread-safe, because os_archkerneldata_s::coreInTrustedFunction
		 * might be concurrently accessed by this and a core thread.
		 */
		OS_UserSuspendInterrupts(OS_LOCKTYPE_OS);
		OS_WINLINLeaveTrustedFunction();
		OS_UserResumeInterrupts(OS_LOCKTYPE_OS);

		/* Trusted functions can use AUTOSAR OS services using the same syscall
		 * buffer pointed to by pSyscallBuffer. To reflect the outcome of the
		 * syscall CallTrustedFunction() we must set the return value here.
		 */
		pSyscallBuffer->rv = OS_E_OK;
	}

	OS_TRACE_CALLTRUSTEDFUNCTION_EXIT_P(pSyscallBuffer->rv, (os_functionid_t)pSyscallBuffer->p1);
}

/** \brief Applies special handling for task termination.
 *
 * This function is called after a system call initiated by task has completed.
 * Its job is to check, if that system call might have terminated the current
 * task and if this is true, restarts the task.
 *
 * Note that system calls, which might terminate a task, do not finish until the
 * initiating task is \e restarted by the core thread connected with the task's core.
 * This means if such a system call finishes successfully, the task which initiated
 * that system call must be restarted and may not simply return as it's the case
 * when other system calls finish.
 *
 * \param[in,out]	pSyscallBuffer		The system call buffer used to execute the system call.
 */
static void OS_WINLINHandleTaskTermination(os_syscallbuffer_t* pSyscallBuffer)
{
	switch (pSyscallBuffer->syscallId)
	{
	case OS_SC_ChainTask:
	case OS_SC_TerminateTask:
		if (pSyscallBuffer->rv == OS_E_OK)
		{
			/* Reset the syscall buffer just to catch problems earlier. */
			pSyscallBuffer->syscallId = OS_SC_UnknownSyscall;

			/* The system call successfully terminated the calling task.
			 * Thus it must be restarted from its beginning, which was marked
			 * by OS_WINLINTaskSetjmp() in OS_WINLINTaskThread().
			 */
			OS_WINLINTaskLongjmp();
		}
		break;

	default:
		/* All other system calls need no special treatment. */
		break;
	}
}

/** \brief Handles syscalls before the OS was started.
 *
 * Some syscall are allowed before the OS was started:
 * -	DisableAllInterrupts()
 * -	EnableAllInterrupts()
 * -	SuspendAllInterrupts()
 * -	ResumeAllInterrupts()
 * .
 * These system calls are handled by this function in this case.
 * After the OS was started, they are dispatched to the respective
 * core thread as any other syscall, too.
 *
 * \param syscallId		The ID of the syscall to be performed.
 *
 * \retval OS_FALSE	The syscall \e must be dispatched to a core thread, i.e.
 *					this function was not able to handle it.
 * \retval OS_TRUE	The syscall was handled and no further actions are necessary.
 */
static os_boolean_t OS_WINLINHandlePreOsStartSyscalls(os_serviceid_t syscallId)
{
	os_boolean_t syscallHandled = OS_FALSE;

	/* Note, we only check for the init-core, if the OS was started there.
	 * If this is true, the same condition applies to all other cores as well.
	 */
	if (!OS_WINLINWasOsStartedOnCore(OS_INITCORE))
	{
		if ((syscallId == OS_SC_SuspendInterrupts)
			|| (syscallId == OS_SC_ResumeInterrupts))
		{
			/* When controlling interrupts before the OS was started on the
			 * respective core, we must ensure that leaving the kernel does not
			 * lead to task dispatching, which happens in OS_LeaveKernel().
			 * This is because the setjmp/longjmp buffers have not yet been
			 * initialized, but they are used during task dispatch. Using before
			 * initializing them brings the program to somewhere but not where
			 * we want it to be. Hence, we set inKernel to 1, which prevents
			 * any syscall initiated before the OS was started from dispatching stuff.
			 */
			OS_GetKernelData()->inKernel = 1;
		}
	}

	return syscallHandled;
}

/** \brief Warns about certain system calls.
 *
 * Some system calls -- e.g. TerminateApplication() -- are not fully implemented.
 * This functions warns users, if they use such services.
 *
 * \param syscallId		The system call ID of the system call to warn about.
 */
static void OS_WINLINWarnAboutCertainSyscalls(os_serviceid_t syscallId)
{
	switch (syscallId)
	{
	case OS_SC_GetStackInfo:
		OS_WINLINLog(
				OS_WINLIN_LOG_WARNING,
				"The system call GetStackInfo() is not fully implemented.",
				OS_WINLINLOG_UNUSED_PARAM);
		break;

	case OS_SC_TerminateApplication:
		OS_WINLINLog(
				OS_WINLIN_LOG_WARNING,
				"The system call TerminateApplication() is not fully implemented.",
				OS_WINLINLOG_UNUSED_PARAM);
		break;

	default:
		/* MISRA */
		break;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
