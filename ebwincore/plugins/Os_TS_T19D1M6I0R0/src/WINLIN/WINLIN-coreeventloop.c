/* WINLIN-coreeventloop.c
 *
 * This file contains the OS_WINLINCoreEventLoop() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-coreeventloop.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#include <LINUX/Os_LINUX_threadsyncspot.h>
#endif

#include <Os_kernel.h>
#include <Os_configuration.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

static os_size_t OS_WINLINAssembleCoreThreadEvents(
		os_winlinHandle_t eventArray[],
		os_isr_t const* associatedIsrs[],
		os_size_t eventArrayLength);
static void OS_WINLINServeSyscall(void);
#if (OS_NINTERRUPTS != 0)
static void OS_WINLINServeIrq(os_isr_t const* pIsr);
static void OS_WINLINServeInternalIrq(os_isr_t const* pIsr);
#endif
static void OS_WINLINWaitForAllThreadsOnThisCoreBeingInPolePosition(void);

/** \brief The maximum number of events a core thread observes.
 *
 * A core thread must observe the event which signals initiation of
 * system calls and those for interrupt requests.
 *
 * Please note that hook and callback functions are run directly by
 * a core thread. Hence there are no events to observe to enable this
 * kind of functions to initiate system calls.
 */
#define OS_MAX_NUMBER_OF_EVENTS_OBSERVED_BY_CORE_THREAD (OS_NINTERRUPTS + 1)

/** \brief Handles all jobs commissioned to a core thread.
 *
 * This function must only be called by a core thread.
 *
 * It implements all event management a core thread must perform, like
 * -	serving system calls,
 * -	starting and interrupting of interrupt handlers based on their priorities,
 * -	task scheduling according to AUTOSAR.
 */
void OS_WINLINCoreEventLoop(void)
{
	/* ATTENTION! The following variables must be (at least) qualified volatile,
	 * because of OS_WINLINCoreSetjmp() and OS_WINLINCoreLongjmp(). The intent is
	 * to have them accessed each time they are used to have fresh copy. Stale
	 * registers shall be discarded when OS_WINLINCoreSetjmp() returns after a
	 * OS_WINLINCoreLongjmp().
	 * If this is not done, compiler optimization can lead to an (unintended)
	 * infinite loop, because auto-start tasks would be started in each iteration
	 * of the main while-loop below.
	 * A compiler barrier might also help, when put into the "true" branch of the
	 * if-statement below, which checks, if OS_WINLINCoreSetjmp() returns 0.
	 *
	 * See also section "7.10 setjmp and longjmp Functions" in "Advanced Programming in
	 * the UNIX Environment" by Richard Stevens and Stephen A. Rago, published 2005 by
	 * Addison Wesley Professional, second edition.
	 */
	volatile os_boolean_t startAutoTasks = OS_TRUE;
	volatile os_boolean_t firstEventLoopIteration = OS_TRUE;

	OS_WINLINAssert(OS_WINLINIsThisACoreThread());
	OS_WINLINAssert(OS_WINLINGetCoreThreadId() == OS_WINLIN_OSAL_GetCurrentThreadId());

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Core %u has entered its event loop.",
			OS_GetMyCoreId());

	OS_WINLINWaitForAllThreadsOnThisCoreBeingInPolePosition();

	while (1)
	{
		os_winlinHandle_t coreEvents[OS_MAX_NUMBER_OF_EVENTS_OBSERVED_BY_CORE_THREAD];
		os_isr_t const* associatedIsrs[OS_MAX_NUMBER_OF_EVENTS_OBSERVED_BY_CORE_THREAD];

		if (OS_WINLINCoreSetjmp() == 0)
		{
			os_size_t numEvents = 0;
			os_winlinDword_t eventIdx = 0;

			if (firstEventLoopIteration == OS_FALSE)
			{
				OS_WINLINLog(
						OS_WINLIN_LOG_DEBUG,
						"Core %u has restarted its event loop.",
						OS_GetMyCoreId());
			}

			/* When we enter the core event loop for the first time, we must care about
			 * auto-start tasks. If there are some waiting in the wings, we must start
			 * them now. Otherwise the call of OS_Dispatch() will bring us back to the
			 * line where OS_WINLINCoreSetjmp() was called, because OS_Idle() was
			 * called during the task dispatch.
			 */
			if (startAutoTasks)
			{
				startAutoTasks = OS_FALSE;
				OS_Dispatch(OS_TS_READY_SYNC);
			}

			/* After having started auto-start tasks, this iteration is no longer considered
			 * the first one, when we reach this point.
			 */
			firstEventLoopIteration = OS_FALSE;

			numEvents = OS_WINLINAssembleCoreThreadEvents(
					coreEvents,
					associatedIsrs,
					OS_MAX_NUMBER_OF_EVENTS_OBSERVED_BY_CORE_THREAD);

			if (numEvents == 0)
			{
				/* This point is reached in case there is no work left to do. This means,
				 * there are no eligible tasks to schedule and no ISRs to observe.
				 * Note, if there are at least two cores, then there is always at least one
				 * ISR to observe per core, namely the cross-core ISR.
				 * The only sensible way to continue here, is to quit. This is achieved by
				 * terminating the core event loop.
				 */
				OS_WINLINLog(
						OS_WINLIN_LOG_WARNING,
						"There is no eligible task to schedule and there are no ISRs to observe. "
						"This program will terminate.",
						OS_WINLINLOG_UNUSED_PARAM);
				break;
			}

			OS_WINLINLogObservedIsrs(associatedIsrs, numEvents);

			/* Wait until a system call is initiated by the current task or
			 * an interrupt is raised.
			 */
			eventIdx = OS_WINLIN_OSAL_WaitForMultipleEvents(
					numEvents,
					coreEvents,
					OS_FALSE);

			if (associatedIsrs[eventIdx] == OS_NULL)	/* then a syscall must be served ... */
			{
				OS_WINLINServeSyscall();
			}
			else /* ... an interrupt was requested. */
			{
#if (OS_NINTERRUPTS != 0)
				OS_WINLINServeIrq(associatedIsrs[eventIdx]);
#else
				OS_WINLINPanic();
#endif
			}
		}
	}
}

/** \brief Assembles the events observed by a core thread.
 *
 * \pre \a eventArray and \a associatedIsrs have the same length.
 *
 * \param [out] eventArray			The array into which the assembled events will be put. The
 *									syscall initiated event of the current task/ISR (if any) comes
 *									last and any preceding elements are events of interrupts,
 *									which might be requested regarding the current interrupt
 *									disable level. This sorting is compatible with
 *									OS_WINLIN_OSAL_WaitForMultipleEvents(), because it always returns the
 *									index of the first event, which has been raised. Hence, we favor
 *									interrupt over syscall events.
 * \param [out]	associatedIsrs		The array of ISRs associated with the events in \a eventArray.
 *									The last entry in this array is always OS_NULL, because there is
 *									no associated ISR with the syscall initiated event of the
 *									current task/ISR, if there is one.
 *									The preceding entries point to the ISRs associated with the
 *									corresponding entry in \a eventArray.
 * \param		eventArrayLength	The length of \a eventArray, i.e. the number of entries.
 *
 * \return The number of assembled events in \a eventArray. This value can be directly passed to
 * OS_WINLIN_OSAL_WaitForMultipleEvents() along with \a eventArray. First come the events of the
 * currently observed ISRs and then the syscall initiated event of the current task or ISR,
 * if any. Hence, if there is no current task/ISR this syscall initiated event is missing.
 * The return value may be zero in this case, if all interrupt are disabled.
 */
static os_size_t OS_WINLINAssembleCoreThreadEvents(
		os_winlinHandle_t eventArray[],
		os_isr_t const* associatedIsrs[],
		os_size_t eventArrayLength)
{
	/* Put any ISR-related event first into eventArray. */
	os_size_t numEvents = OS_WINLINAssembleIsrEvents(
			eventArray,
			associatedIsrs,
			eventArrayLength,
			OS_FALSE);

	/* This event is raised when syscall are commissioned to a core thread. */
	OS_WINLINAssert(numEvents < eventArrayLength);
	{
		os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfCurrentThread();
		if (pSyscallPort != OS_NULL)
		{
			eventArray[numEvents]		= pSyscallPort->syscallInitiatedEvent;
			associatedIsrs[numEvents]	= OS_NULL;
			++numEvents;
		}
		/* else: there is no current ISR or task. */
	}

	return numEvents;
}

/** \brief Serves a system call.
 *
 * A core thread was awakened because a task or ISR function initiated a system call.
 * The core thread calls this function to serve this client. During the time of the
 * syscall no further interrupts can happen, i.e. the kernel appears to be non-reentrant.
 */
static void OS_WINLINServeSyscall(void)
{
	/* A core thread always shares the syscall port of the current task or ISR.
	 * This is setup when a task is started or resumed. Hence, we only need to
	 * consult this core thread's TLS to get the current task's or ISR's syscall port.
	 */
	os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfCurrentThread();
	OS_WINLINAssert(pSyscallPort != OS_NULL);

	/* We're going to serve the initiated syscall, hence reset the event. */
	OS_WINLIN_OSAL_ResetEvent(pSyscallPort->syscallInitiatedEvent);

	{
		os_syscallbuffer_t* const pSyscallBuffer = OS_WINLINGetSyscallBuffer(pSyscallPort);

		/* Sometimes the pSyscallPort->syscallInitiatedEvent is set even though there is no syscall
		 * to serve. These occurrences must be considered spurious and must be simply ignored.
		 * That's the reason for the extra check of the given syscall id.
		 */
		if (pSyscallBuffer->syscallId != OS_SC_UnknownSyscall)
		{
			OS_WINLINLogSyscall(pSyscallPort);
			OS_WINLINExecuteSyscall(pSyscallBuffer);

			/* We end up here, when the syscall just served did not lead to a preemption.
			 * This may be if an ISR makes a syscall, because ISR can't be preempted only interrupted.
			 * As well may be the case, that a task made a syscall, but it's still owning the
			 * CPU. In either case we must use the syscall finished event to notify the respective
			 * player about the end of the syscall. Please note, that there might be no such event.
			 * This is the case, when hook functions make syscall, because these are executed
			 * directly by a core thread.
			 */
			if (pSyscallPort->syscallFinishedEvent != OS_NULL) /* then, there is a valid syscall port. */
			{
				os_uint8_t const inFunction = OS_GetKernelData()->inFunction;
				if (inFunction == OS_INTASK) /* then, a task initiated the most recent syscall ... */
				{
					os_task_t const* const currentTask = OS_GetKernelData()->taskCurrent;
					if (currentTask != OS_NULL)
					{
						/* The same trick we do in OS_WINLINKernFinishedIrq() to prevent interrupted
						 * tasks from being resumed to early when there are pending interrupts,
						 * is used also here. Imagine a task shares a resource with an ISR. If that
						 * ISR is triggered while the task already owns the resource, we must keep
						 * it from progressing after it releases the resource. First the pending
						 * interrupt must be served, before the task is allowed to continue.
						 *
						 * To achieve this we mark the current task as 'interrupted', because this
						 * would be done anyway when the pending ISR is served by OS_WINLINServeIrq().
						 * This mark also prevents the mentioned function form suspending the thread
						 * of the current task once again, which would cause an imbalance in the
						 * suspend/resume counter managed by Windows. Furthermore, we resume the
						 * current task in a special way: only the syscall finished event is raised,
						 * but the task thread is not resumed. This is delayed until the pending
						 * ISR executes OS_WINLINKernFinishedIrq().
						 */
						if (OS_WINLINAreThereAnyPendingIrqsToServe(OS_FALSE))
						{
							OS_WINLINSetInterruptedStateOfTask(currentTask, OS_TRUE);
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
							(void)OS_WINLIN_OSAL_SuspendThreadHelper(
									OS_WINLINGetThreadOfTask(currentTask),
									OS_FALSE);
#else
							(void)OS_WINLIN_OSAL_SuspendThread(OS_WINLINGetThreadOfTask(currentTask));
#endif
							OS_ResumeTaskSyncHelper(
									currentTask,
									OS_WINLIN_RESUME_TASK_WITH_PENDING_IRQS);
						}
						else
						{
							OS_ResumeTaskSyncHelper(currentTask, OS_WINLIN_RESUME_TASK_AFTER_SYSCALL);
						}
					}
				}
				else /* ... it was due to an ISR. */
				{
					/* Don't raise the syscall finished event, if the syscall just executed is
					 * OS_SC_WINLINFinishedIrq.
					 * This is necessary to keep the ISR thread from re-executing the ISR once
					 * again, because the intent of OS_WINLINKernFinishedIrq is to prevent just
					 * this. The ISR thread, which uses this syscall blocks on its syscall finished
					 * event until it is triggered again. That's the same concept as with tasks, which
					 * also block on their syscall finished event when they've called TerminateTask()
					 * until they are re-activated again.
					 *
					 * ATTENTION: The current syscall port put into the TLS of this core thread
					 * might be different from pSyscallPort. This is the case, when there is an
					 * interrupted ISR, which shall be resumed, after the currently running ISR has
					 * called the OS_SC_WINLINFinishedIrq syscall. See code location marked with
					 * MARK:OS_WINLINKernFinishedIrq.A . That's why it's important to use the local
					 * pointer saved BEFORE the syscall was executed.
					 */
					if (pSyscallBuffer->syscallId != OS_SC_WINLINFinishedIrq)
					{
						OS_WINLIN_OSAL_SetEvent(pSyscallPort->syscallFinishedEvent);
					}
				}
			}
		}
	}
}

#if (OS_NINTERRUPTS != 0)
/** \brief Serves a triggered interrupt.
 *
 * After having resumed the corresponding ISR thread this function returns.
 * The ISR is processed by the ISR thread concurrently. The calling core
 * thread spins around to enable syscalls and higher-priority interrupts.
 *
 * \param pIsr		Points to the triggered ISR. This is the one to be served.
 */
static void OS_WINLINServeIrq(os_isr_t const* pIsr)
{
	os_isrdynamic_t* const pIsrDyn = pIsr->dynamic;

	if (OS_WINLINIsIsrEnabled(pIsrDyn))
	{
		/* Suspend -- or to be more precise: interrupt -- current task thread, if any.
		 * Do this only once per task interruption! This is because the Windows services
		 * SuspendThread() and ResumeThread() use a counter and calls to these services
		 * must be balanced. If a thread is suspended x times it must be resumed x times,
		 * otherwise it stays suspended. The ease management, we suspend an interrupted
		 * task thread only once.
		 */
		os_task_t const* const currentTask = OS_GetKernelData()->taskCurrent;
		if ((currentTask != OS_NULL) && (OS_WINLINWasTaskInterrupted(currentTask) == OS_FALSE))
		{
			(void)OS_WINLIN_OSAL_SuspendThreadHelper(
					OS_WINLINGetThreadOfTask(currentTask),
					OS_TRUE);
			OS_WINLINSetInterruptedStateOfTask(currentTask, OS_TRUE);

			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Task %s was interrupted by ISR %s.",
					OS_WINLINGetNameOfTask(currentTask),
					OS_WINLINGetNameOfIsr(pIsr));
		}

		/* Suspend the current ISR thread, if any. */
		{
			os_isrid_t const isrCurrent = OS_WINLINGetCoreIsrCurrent();
			OS_WINLINSetIdOfInterruptedIsr(pIsrDyn, isrCurrent); /* Remember interrupted ISR. */
			if (isrCurrent != OS_NULLISR) /* then, the current ISR thread must be suspended. */
			{
				os_isr_t const * const pIsrCurr = &OS_isrTableBase[isrCurrent];
				(void)OS_WINLIN_OSAL_SuspendThreadHelper(
						OS_WINLINGetThreadOfIsr(pIsrCurr->dynamic),
						OS_TRUE);

				OS_WINLINLog(
						OS_WINLIN_LOG_DEBUG,
						"ISR %s was interrupted by ISR %s.",
						OS_WINLINGetNameOfIsr(pIsrCurr),
						OS_WINLINGetNameOfIsr(pIsr));
			}
		}

		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Going to resume thread %#x=%d to run ISR %s.",
				OS_WINLINGetTidOfIsrThread(pIsrDyn),
				OS_WINLINGetTidOfIsrThread(pIsrDyn),
				OS_WINLINGetNameOfIsr(pIsr));

		/* Start the new ISR.
		 * It has a higher priority than the current one, if any, and hence interrupted it.
		 * As with tasks, the core thread shares the syscall port with the current ISR.
		 * This is important for OS_WINLINServeSyscall(), because it relies on this sharing.
		 * Also note, that when ISRs are inactive, but ran at least once in the past, their
		 * threads got stuck in the OS_SC_WINDOWSFinishedIrq syscall. That's why the
		 * syscallFinishedEvent is raised to release/restart them. It's the same protocol as
		 * with the termination of tasks.
		 */
		OS_WINLINSetCoreIsrCurrent(pIsr->isrId);
		OS_WINLINSaveAndSetInterruptDisableLevelBeforeIsrStarts(pIsr);

		if (OS_WINLINIsIsrInternal(pIsr))
		{
			OS_WINLINServeInternalIrq(pIsr);
		}
		else
		{
			OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfIsr(pIsrDyn));
			OS_WINLIN_OSAL_SetEvent(OS_WINLINGetSyscallPortOfIsr(pIsrDyn)->syscallFinishedEvent);
			(void)OS_WINLIN_OSAL_ResumeThread(OS_WINLINGetThreadOfIsr(pIsrDyn));
		}
	}
	else
	{
		/* When a specific ISR is disabled and is triggered we nevertheless get here,
		 * because the thread triggering the ISR does not check the enabled state before.
		 * This is done here and in case the ISR is disabled, we simply do nothing further.
		 * Keep in mind that the requested event is an auto-reset event: we don't have to
		 * reset it here.
		 */
		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"ISR %s was triggered but is disabled.",
				OS_WINLINGetNameOfIsr(pIsr));
	}
}
#endif /* OS_NINTERRUPTS != 0 */

/** \brief Wait until all task and ISR threads, which are assigned to the core of the caller,
 * are in their respective pole position.
 *
 * \par Special note for Microsoft Windows:
 * There is a lot going on between the time a thread is started until it reaches the first line
 * of its function, i.e. the function passed to the call of CreateThread(). To avoid any deadlocks
 * caused by code running during these very first moments in a thread's live and the use of
 * SuspendThread() in the very same process, all such threads run off immediately and block on the
 * first line of their functions. The respective core thread is notified about that by raising
 * the syscall initiated event of the task or ISR thread's syscall port. This is observed by this
 * function. It returns, when all task and ISR threads have raised this event. Note, that those
 * threads are still blocked on the first line of their (thread) function. They continue to run,
 * when they are scheduled by the AutoCore OS scheduler.
 *
 * Also note, that WaitForMultipleObjects() can't be used to wait for all events in a single
 * Windows system call, because of the MAXIMUM_WAIT_OBJECTS limitation.
 */
static void OS_WINLINWaitForAllThreadsOnThisCoreBeingInPolePosition(void)
{
#if ((OS_NINTERRUPTS != 0) || (OS_NTASKS != 0))
	os_coreid_t const myCore = OS_GetMyCoreId();
#endif
#if (OS_NINTERRUPTS != 0)
	{
		os_uint8_t iidIter;
		for (iidIter = 0; iidIter < OS_NINTERRUPTS; ++iidIter)
		{
			os_isr_t const * const pIsr = &OS_isrTableBase[iidIter];
			if ((OS_GetIsrCoreId(pIsr) == myCore) && (!OS_WINLINIsIsrInternal(pIsr)))
			{
				os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfIsr(pIsr->dynamic);
				OS_WINLIN_OSAL_WaitForSingleEvent(pSyscallPort->syscallInitiatedEvent);
				OS_WINLIN_OSAL_ResetEvent(pSyscallPort->syscallInitiatedEvent);
			}
		}
	}
#endif
#if (OS_NTASKS != 0)
	{
		os_uint8_t tidIter;
		for (tidIter = 0; tidIter < OS_NTASKS; ++tidIter)
		{
			os_task_t const * const pTask = &OS_taskTableBase[tidIter];
			if (OS_GetTaskCoreId(pTask) == myCore)
			{
				os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfTask(pTask);
				OS_WINLIN_OSAL_WaitForSingleEvent(pSyscallPort->syscallInitiatedEvent);
				OS_WINLIN_OSAL_ResetEvent(pSyscallPort->syscallInitiatedEvent);
			}
		}
	}
#endif
}

#if (OS_NINTERRUPTS != 0)
/** \brief Handles internal interrupts.
 *
 * Internal interrupts are those, which belong to system applications and hence
 * may use AutoCore OS services directly. This means, they dodge the syscall
 * mechanism.
 *
 * \param pIsr The ISR of the internal interrupt to be served.
 */
static void OS_WINLINServeInternalIrq(os_isr_t const* pIsr)
{
	OS_WINLINAssert(OS_WINLINIsIsrInternal(pIsr));

	/* Internal ISRs must not use the syscall mechanism.
	 * The following is done, to catch such errors early.
	 */
	OS_WINLINSetSyscallPortOfCurrentThread(OS_NULL);

#if (OS_N_CORES > 1)
	if ((pIsr->flags & OS_ISRF_CROSSCORE) != 0)
	{
		(void)OS_CrossCoreNotifyIsr(pIsr->isrId);
	}
	else
#endif
	if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT2)
	{
		(void)OS_Cat2Entry(pIsr->isrId);
	}
	else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT1)
	{
		(void)OS_Cat1Entry(pIsr->isrId);
	}
	else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CATK)
	{
		(void)OS_CatKEntry(pIsr->isrId);
	}
	else
	{
		OS_WINLINPanic();
	}

	/* Note, we don't execute the following statement
	 * 		"OS_GetKernelData()->inKernel = oldInKernel;"
	 * as it is done in OS_WINLINHandleIrq() by an ISR thread, because we are already in
	 * the kernel and stay there. OS_WINLINKernFinishedIrq() will get us out again.
	 */

	OS_WINLINKernFinishedIrq(pIsr);
}
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
