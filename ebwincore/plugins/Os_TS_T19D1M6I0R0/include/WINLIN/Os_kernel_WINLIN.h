/* Os_kernel_WINLIN.h
 *
 * Kernel-internal definitions for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_kernel_WINLIN.h 31527 2020-03-03 11:09:45Z arla271030 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: AnonVaArgsMacrosInC99
 *   Anonymous variadic macros were introduced in C99
 *
 * Reason:
 * Variadic macros are required for the logging infrastructure.
 */

#ifndef OS_KERNEL_WINLIN_H
#define OS_KERNEL_WINLIN_H

#include <Os_tool.h>
#include <Os_syscalls.h>
#include <WINLIN/Os_WINLIN_cpu.h>
#include <WINLIN/Os_WINLIN_jump.h>
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#include <LINUX/Os_LINUX_threadsyncspot.h>
#endif

/* Architecture characteristics. If necessary, some of these could be defined at the derivative level.
 *
 *   - OS_STACKGROWS:			either OS_STACKGROWSDOWN or OS_STACKGROWSUP as necessary
 *   - OS_STACKFILL:			0xebe..b, enough to fill an os_stackelement_t with bytes of 0xeb
 *   - OS_HASMEMPROT:			0 (no memory protection) or 1 (memory protection is supported)
 *   - OS_TYPEOF_ININTERRUPT:	data type for OS_inInterrupt. Leave undefined of OS_inInterrupt is not needed.
 *   - OS_HWT_POWEROF2:         1 if all supported hardware timers wrap cleanly on the (2**n)th tick, 0 otherwise.
 *   - OS_ARCH_HAS_ASSEMBLY_LANGUAGE: we don't use assembly language on Windows/Linux.
 *   - OS_CACHE_MODE:			x86 processor have coherent caches.
 *   - OS_CACHE_LINE_LEN:		length of a cache line in bytes; disregarded on Windows/Linux.
 *   - OS_HAVE_ARCHISRDYN_T:	we need some special extensions to manage ISRs at run-time. See os_archisrdynamic_t.
 *   - OS_ARCH_HAS_KERNELDATA:	there are some extension to os_kerneldata_t for WINLIN.
 *   - OS_HAVE_ARCHAPP_T:		we don't need any extensions to application contexts.
 *   - OS_HAVE_ARCHTASK_T:		we need special and constant extensions to the original structure, e.g. for a task's name.
 *   - OS_HAVE_ARCHHWT_T:		we want to know which ISR to trigger when a timer expires.
 *   - OS_SIMTIMER_NTIMER:		we have four simulated timers and each has OS_SIMTIMER_NCOMPARE comparators
 *   - OS_SIMTIMER_NCOMPARE:	number of comparators of each simulated timer
 *   - OS_HAS_AWAIT_XCORE_REPLY: there is a specialized function available to await cross-core replies
*/
#define OS_STACKGROWS			OS_STACKGROWSDOWN
#define OS_STACKFILL			OS_U(0xebebebeb)
#define OS_HASMEMPROT			0
/* #define OS_TYPEOF_ININTERRUPT	os_uint8_t */
#define OS_HWT_POWEROF2			1
#define OS_ARCH_HAS_ASSEMBLY_LANGUAGE 0
#define OS_CACHE_MODE			OS_CACHE_MODE_NONE
#define OS_CACHE_LINE_LEN		0
#define OS_HAVE_ARCHISRDYN_T	1
#define OS_ARCH_HAS_KERNELDATA	1
#define OS_HAVE_ARCHAPP_T		0
#define OS_HAVE_ARCHTASK_T		1
#define OS_HAVE_ARCHHWT_T		1
#define OS_SIMTIMER_NTIMER		4
#define OS_SIMTIMER_NCOMPARE	1
#define OS_HAS_AWAIT_XCORE_REPLY 1

#ifndef OS_EXCLUDE_KILLABLE_ISR
/* Killing of ISRs is not supported.
 * NB: killing of tasks isn't supported as well.
 */
#define OS_DoQueryIsr(x)	OS_FALSE
#define OS_DoMarkIsr(x)		do {} while(0)
#endif

#ifndef OS_EXCLUDE_CPULOAD
/* CPU load measurement is not accurate on WINLIN. Furthermore, due to the
 * concurrent execution of OS_CatXEntry(), the implementation of this macro may
 * cause problems, because it is not reentrant. Nevertheless, it shall be possible
 * to "play" with CPU load measurement. Therefore, it is possible to configure
 * it and call its functions, but no real measurements are returned.
 */
#define OS_LEAVEIDLESTATE() do {} while (0)
#endif

#ifndef OS_EXCLUDE_TIMINGPROTECTION
/* Timing protection is not supported. */
#define OS_DoTimingProtection(x)
#endif

#ifndef OS_EXCLUDE_PROTECTION
/* Memory protection is not supported. */
#define OS_DoMemoryProtection(x)
#endif

#if (OS_N_CORES > 1)
/** \def OS_AwaitXCoreReply()
 * \brief Awaits cross-core replies.
 *
 * This macro awaits corss-core replies. While doing so, it processes incoming
 * cross-core messages. The caller detects this by checking the value of the
 * memory pMsgResult points to. If it's not equal to OS_XC_ERROR_WaitingForReply,
 * then there is the awaited cross-core reply.
 * Note, processing of incoming cross-core messages is necessary, because the
 * kernel in WINDOWS/WIN32X86 is not interruptible and hence such messages must be
 * explicitly received. Otherwise, there won't be any cross-core replies at all.
 *
 * \param pMsgResult	Not used. The caller checks the pointed to memory in a
 * 						loop to detect cross-core replies.
 */
#define OS_AwaitXCoreReply(pMsgResult)		\
	do {									\
		OS_ReceiveMessage(OS_GetMyCoreId());\
	} while (0)
#endif /* OS_N_CORES > 1 */

/** \def OS_ARCH_TIMESTAMP_INIT
 * \brief Initializes the time-stamp value depending on its bit-width.
 */
#if OS_ARCH_HAS_64BIT
#define OS_ARCH_TIMESTAMP_INIT(x)	x
#else
#define OS_ARCH_TIMESTAMP_INIT(x)	{x}
#endif

/** \def OS_CLZ
 * \brief Counts the leading zeros in the given integer.
 *
 * \param x		The integer in which the leading zeros shall be counted.
 *
 * \return The leading zeros in \a x.
 */
#define OS_CLZ(x)	(OS_CountLeadingZeros(x))

/** \name Spinlocks
 *
 * On Windows/Linux spinlocks are based on its interlocked operations.
 */
/** @{ */
#define OS_HAS_ARCHSPINLOCK		1
#define OS_ArchTakeSpinlock(a)			\
	do {								\
		OS_WINLIN_OSAL_TakeSpinlock(a);	\
	} while (0)

#define OS_ArchDropSpinlock(a)			\
	do {								\
		OS_WINLIN_OSAL_DropSpinlock(a);	\
	} while (0)
/** @} */

/** \name Memory protection
 *
 * On Windows/Linux no memory protection nor stack management is implemented.
 * It's all done by Windows/Linux itself. Hence, AutoCore OS checks about such
 * things must be prohibited to prevent bogus results. This means:
 * - OS_ArchCanWrite() must always return 1, because AutoCore OS'
 *   internal data structures are not kept synchronized with a Windows/Linux
 *   thread's current stack information.
 * - Just to be sure, there is a OS_ArchMisalignedData() defined, which
 *   always returns 0. Note, on x86 processors misaligned data is handled
 *   gracefully; hence is not an issue.
 *
 * Refer to Os_kernel_common.h for the original definition of these macros.
 * There, also the default behaviour is set, which is now overwritten by the
 * following.
 */
/** @{ */
#define OS_ArchCanWrite(b, l) 1
#define OS_ArchMisalignedData(b, l) 0
/** @} */

/** \def OS_DoStackCheck
 *
 * Stack checks are not supported, because the host OS manages the stacks.
 * Therefore, these checks must be completely disabled.
 */
#define OS_DoStackCheck(x)	0

/** \brief Handles task termination in case there is no TerminateTask().
 *
 * This macro represents the name of the function to call, when a task
 * has reached the end of its life, i.e. returned from user-supplied code
 * w/o calling TerminateTask().
 */
#define OS_K_TASKRETURN			(&OS_WINLINTaskReturn)

#ifndef OS_ASM
/** \name Interrupt levels. */
/** @{ */
extern const os_intstatus_t OS_intDisableLevel;
extern const os_intstatus_t OS_intDisableLevelAll;
extern const os_intstatus_t OS_kernDisableLevel;
extern const os_intstatus_t OS_kernOnlyXcoreLevel;
extern const os_intstatus_t OS_intDisableLevelMax;
/** @} */
#endif

/** \name Time stamp timer
 * \brief Time stamp timer configuration.
 *
 * Always use the internal time stamp timer.
 *
 * \see https://docs.microsoft.com/en-us/windows/desktop/SysInfo/acquiring-high-resolution-time-stamps
 */
/** @{ */
#define OS_USEGENERICTIMESTAMP			0
#define OS_USE_GLOBAL_GENERICTIMESTAMP	0
#define OS_INITTIMESTAMP				(&OS_WINLINInitTimeStamp)
/** @} */

#ifndef OS_ASM
/** \brief Represents architecture-specific extensions to hardware timers. */
struct os_archhwt_s
{
	/** \brief The ID of the ISR to trigger, when the timer expires. */
	os_isrid_t isrId;

	/** \brief The name of the timer. */
	os_char_t const* const timerName;
};
typedef struct os_archhwt_s os_archhwt_t;

/** \brief Initialize architecture dependent extension of os_hwt_t.
 *
 * \param[in] isrId		The ID of the ISR to be called, when the timer want to
 *						trigger an interrupt.
 * \param[in] timerName	The name of the timer.
 */
#define OS_ARCHHWT_INIT(isrId, timerName)	\
	{										\
		(isrId),							\
		(timerName)							\
	}

/** \brief Gets the name of a timer.
 *
 * \param[in] pHwt		Points to a constant instance of os_hwt_t representing the
 *						hardware timer of which to return the name.
 *
 * \return The name of the hardware timer pointed to by \a pHwt.
 */
#define OS_WINLINGetNameOfTimer(pHwt) ((pHwt)->arch.timerName)
#endif /* !OS_ASM */

#ifndef OS_ASM
/** \brief Input and output buffer for system calls (syscalls).
 *
 * Used to exchange system call parameters and return value
 * between task and ISR threads and a core thread.
 *
 * Please note, that not all AutoCore OS syscall really have four
 * parameters. This is just the maximum of all offered syscalls to have
 * one syscall buffer type for all of them. The initiated syscall finally
 * exactly knows, how many parameters it has and passing more than
 * that is not a problem, because the surplus parameters are not regarded.
 */
struct os_syscallbuffer_s
{
	os_serviceid_t syscallId;	/**< Identifies the syscall. */
	os_paramtype_t p1;			/**< First parameter of the syscall \c syscallId. */
	os_paramtype_t p2;			/**< Second parameter of the syscall \c syscallId. */
	os_paramtype_t p3;			/**< Third parameter of the syscall \c syscallId. */
	os_paramtype_t p4;			/**< Fourth parameter of the syscall \c syscallId. */
	os_winlinresult_t rv;		/**< The syscall's return value. */
};
typedef struct os_syscallbuffer_s os_syscallbuffer_t;
#endif

#ifndef OS_ASM
/** \brief Represents a system call port.
 *
 * An entity, e.g. task, ISR or hook, which wants to make system calls,
 * must do this via its own system call port. Such interfaces
 * can't be shared, because they contain a buffer, which is exclusively
 * owned by a single entity.
 */
struct os_syscallport_s
{
	/** \brief Buffer for system calls.
	 *
	 * Each task/ISR/hook thread has its own buffer to make syscalls.
	 * It contains input for the system call, its return value (if any)
	 * and additional data to identify a initiated syscall.
	 */
	os_syscallbuffer_t	syscallBuffer;

	/** \brief Signals the initiation of a syscall.
	 *
	 * This event is raised by tasks/ISRs to notify the resp. core thread about the
	 * initiated syscall. The core thread observes this event of the current
	 * task or ISR only and if raised performs the requested syscall on behalf of
	 * the task or ISR.
	 */
	os_winlinHandle_t syscallInitiatedEvent;

	/** \brief Signals the end of a syscall.
	 *
	 * This event is raised at the end of a syscall, when a task/ISR is
	 * synchronously resumed. It's used to block a task/ISR thread, while
	 * the respective core thread executes the initiated system call.
	 * When the core thread finishes the syscall, this event is raised.
	 *
	 * This is also the event used to block a task, when it has been terminated.
	 * As long as a task stays in this state it sticks here. When it is
	 * restarted this event is raised, which makes the task returning from the
	 * original system call, which put the task into the terminated state. The
	 * special post-processing done by OS_WINLINHandleTaskTermination() results
	 * in properly restarting the task.
	 */
	os_winlinHandle_t	syscallFinishedEvent;
};
typedef struct os_syscallport_s os_syscallport_t;
#endif

/** \brief Initializes a system call port.
 *
 * \param[in,out]	p		Points to the system call port port to be initialized.
 *							\a p has the type os_syscallport_t*.
 */
#define OS_WINLINInitializeSyscallPort(p)										\
	do {																		\
		(p)->syscallInitiatedEvent = OS_WINLIN_OSAL_CreateEvent(OS_TRUE, OS_FALSE);	\
		(p)->syscallFinishedEvent = OS_WINLIN_OSAL_CreateEvent(OS_TRUE, OS_FALSE);	\
		(p)->syscallBuffer.syscallId = OS_SC_UnknownSyscall;					\
	} while (0)

/** \brief Initializes a system call port and marks it as \b invalid.
 *
 * \attention The syscall port \a p can't be used as a full-blown system call port.
 * It may be used on in special circumstanced, e.g. by hook functions.
 *
 * \param[in,out]	p		Points to an instance of type os_syscallport_t, which
 *							shall be initialized.
 */
#define OS_WINLINInitializeSyscallPortAsInvalid(p)								\
	do {																		\
		(p)->syscallInitiatedEvent = OS_NULL;									\
		(p)->syscallFinishedEvent = OS_NULL;									\
		(p)->syscallBuffer.syscallId = OS_SC_UnknownSyscall;					\
	} while (0)

/** \brief Gets the system call buffer of the given system call port.
 *
 * \param[in]		p		Points to the system call port of which the system call
 *							buffer shall be returned. \a p points to an instance
 *							of os_syscallport_t.
 *
 * \return Pointer to the system call buffer of the given system call port \a p.
 */
#define OS_WINLINGetSyscallBuffer(p) (&((p)->syscallBuffer))

/** \brief Gets the calling thread's syscall port.
 *
 * \return The calling thread's syscall port, i.e. a pointer to an instance of
 * type os_syscallport_t.
 */
#define OS_WINLINGetSyscallPortOfCurrentThread() \
	((os_syscallport_t*)OS_WINLIN_OSAL_TlsGetValue(OS_winlinTlsSyscallPort))

/** \brief Sets the current thread's syscall port.
 *
 * \param[in] pSyscallPort	Points to an instance of os_syscallport_t, which shall become the
 *							calling thread's syscall port.
 */
#define OS_WINLINSetSyscallPortOfCurrentThread(pSyscallPort)					\
	do {																		\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsSyscallPort, (pSyscallPort));	\
	} while (0)

#ifndef OS_ASM
typedef struct os_archtask_s os_archtask_t;

/** \brief Architecture-specific extensions to os_task_t. */
struct os_archtask_s
{
	/** \brief The name of the owning task. */
	os_char_t const* taskName;
};
#endif /* OS_ASM */

/** \brief Initializes the architecture-specific extensions of os_task_t instance.
 *
 * \param taskName		The name of the task.
 */
#define OS_ARCHTASK_INIT(taskName)	\
	{								\
		(taskName)					\
	}

/** \brief Gets the name of a task.
 *
 * \param[in] t		Points to a constant instance of os_task_t of which to return the name.
 *
 * \return The name of task \a t as C string.
 */
#define OS_WINLINGetNameOfTask(t) ((t)->arch.taskName)

#ifndef OS_ASM
typedef struct os_archcontext_s os_archcontext_t;

/** \brief A task's architecture-dependent context.
 *
 * On WINLIN we store task thread related stuff in a context.
 */
struct os_archcontext_s
{
	/** \brief The Windows/Linux handle of the task thread. */
	os_winlinHandle_t	taskThread;

	/** \brief The TID of the task thread.
	 *
	 * This is the Windows/Linux TID of \c taskThread.
	 */
	os_winlinTid_t		taskThreadId;

	/** \brief The syscall port of the owning task.
	 *
	 * The owning task it the task, to which this structure belongs to.
	 * It's a component of os_taskdynamic_t.
	 */
	os_syscallport_t taskSyscallPort;

	/** \brief A flag to indicate, whether a task was interrupted.
	 *
	 * This flag is used by OS_WINLINServeIrq() to remember, that a task was
	 * interrupted. OS_WINLINKernFinishedIrq() resets this flag, as soon as
	 * the ISR finishes.
	 */
	os_boolean_t taskWasInterrupted;

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
	/** \brief Sync-spot used to control the thread of this task. */
	os_linuxThreadSyncSpot_t syncSpot;
#endif
};
#endif /* !OS_ASM */

/** \brief Size in bytes of the architecture-dependent task context.
 *
 * \remarks This is only necessary for assembler code. Because there is no
 * such code in WINLIN the actual value is not important. Nevertheless
 * it must be defined, because it's referenced elsewhere.
 */
#define OS_SIZEOF_CTXT	0

/** \brief Gets the Windows/Linux TID of a task thread.
 *
 * \param[in] pTask		Points to the task of which the TID of its associated Windows/Linux thread
 *						shall be returned.
 *
 * \return The Windows/Linux TID of the thread associated with \a pTask.
 */
#define OS_WINLINGetTidOfTaskThread(pTask) ((pTask)->dynamic->context.taskThreadId)

/** \brief Gets the Windows/Linux thread of a task.
 *
 * \param[in] pTask		Points to the task of which the associated Windows/Linux thread
 *						shall be returned.
 *
 * \return The associated Windows/Linux thread of the task \a pTask. This thread
 * is called the <em>task thread</em> of \a pTask.
 */
#define OS_WINLINGetThreadOfTask(pTask) ((pTask)->dynamic->context.taskThread)

/** \brief Gets the syscall port of a task thread.
 *
 * \param[in] pTask		Points to the task of which the syscall port shall be returned.
 *
 * \return A pointer to the syscall port of the task \a pTask.
 */
#define OS_WINLINGetSyscallPortOfTask(pTask) (&((pTask)->dynamic->context.taskSyscallPort))

/** \brief Checks, whether the given task was interrupted.
 *
 * \param[in] pTask		Points to a constant instance of os_task_t.
 *
 * \return An indication whether \a pTask has been interrupted or not.
 * \retval OS_TRUE	\a pTask was interrupted and the respective ISR is still active.
 * \retval OS_FALSE	There was no interrupt request up to now, which could have interrupted
 *					\a pTask.
 */
#define OS_WINLINWasTaskInterrupted(pTask) ((pTask)->dynamic->context.taskWasInterrupted)

/** \brief Sets the TID of the thread of task \a pTask.
 *
 *\attention When POSIX signals are used to control threads, the thread ID is set by
 * the thread itself when it starts its life. This is because, the creator of that thread
 * is not told the thread ID (by pthread_create()) and hence can't set it.
 *
 * \param[in] pTask		Points to the task of which the TID of its associated thread shall be set.
 * \param tid			The new TID of the thread of \a pTask.
 */
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#define OS_WINLINSetTidOfTaskThread(pTask, tid)
#else
#define OS_WINLINSetTidOfTaskThread(pTask, tid)			\
	do {												\
		(pTask)->dynamic->context.taskThreadId = (tid);	\
	} while (0)
#endif

/** \brief Sets the thread of \a pTask.
 *
 * \param[in] pTask		Points to the task which shall be associated with the thread \a thread.
 * \param thread		The new thread for the task \a pTask.
 */
#define OS_WINLINSetThreadOfTask(pTask, thread)			\
	do {												\
		(pTask)->dynamic->context.taskThread = (thread);\
	} while (0)

/** \brief Sets the interrupted state of the given task.
 *
 * \param[in] pTask		Points to a constant instance of type os_task_t, of which the
 *						interupted state shall be set to \a state.
 * \param state			The new interrupted state of \a pTask.
 */
#define OS_WINLINSetInterruptedStateOfTask(pTask, state)		\
	do {														\
		(pTask)->dynamic->context.taskWasInterrupted = (state);	\
	} while (0)

/** \def OS_WINLINInitializeTaskThread(pTask)
 * \brief Initializes the Windows/Linux thread of the given task.
 *
 * \remarks The syscall initiated and finished events are used in combination with the
 * function OS_WINLINWaitForAllThreadsBeingInPolePosition() to ensure that all threads
 * are in their pole position, before AutoCore OS scheduling is started.
 *
 * \remarks The syscall finished event of the syscall port associated with \a pTask
 * is also reset. This is only important for the first time a task is started. Then
 * this event is also set by OS_StartTask(), because it's handled the same as synchronous
 * task resumption. To correct this it's reset at initialization time.
 *
 * \param[in] pTask		Points to a constant instance of os_task_t. This is the task,
 *						of which the Windows/Linux thread shall be initialized.
 */
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#define OS_WINLINInitializeTaskThread(pTask)														\
	do {																							\
		(pTask)->dynamic->context.taskThreadId = OS_WINLIN_OSAL_GetCurrentThreadId();				\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void *)(os_address_t)OS_GetTaskCoreId((pTask)));			\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[OS_GetTaskCoreId((pTask))]);	\
		OS_WINLIN_OSAL_TlsSetValue(OS_linuxTlsSyncSpot, &(pTask)->dynamic->context.syncSpot);		\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfTask(pTask));				\
		OS_LINUXControlBlockageOfPosixSignal(OS_FALSE);												\
		OS_WINLIN_OSAL_SignalEventAndWait(															\
				OS_WINLINGetSyscallPortOfTask(pTask)->syscallInitiatedEvent,						\
				OS_WINLINGetSyscallPortOfTask(pTask)->syscallFinishedEvent);						\
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetSyscallPortOfTask(pTask)->syscallFinishedEvent);		\
	} while (0)
#else
#define OS_WINLINInitializeTaskThread(pTask)														\
	do {																							\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void *)(os_address_t)OS_GetTaskCoreId((pTask)));			\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[OS_GetTaskCoreId((pTask))]);	\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfTask(pTask));				\
		OS_WINLIN_OSAL_SignalEventAndWait(															\
				OS_WINLINGetSyscallPortOfTask(pTask)->syscallInitiatedEvent,						\
				OS_WINLINGetSyscallPortOfTask(pTask)->syscallFinishedEvent);						\
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetSyscallPortOfTask(pTask)->syscallFinishedEvent);		\
	} while (0)
#endif

/** \brief Suspends the task thread.
 *
 * This function is used to suspend the task thread associated with \a a, what
 * is the equivalent of saving its context. After having saved the context a
 * new task is dispatched synchronously.
 *
 * \param[in,out] a		Points to an instance of os_archcontext_t owned by the thread
 *						of which the context shall be saved.
*/
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#define OS_SaveTaskContextSync(a)						\
	do {												\
		OS_WINLINLog(									\
				OS_WINLIN_LOG_DEBUG,					\
				"Going to suspend task thread %#x=%d.",	\
				(a)->taskThreadId,						\
				(a)->taskThreadId);						\
		OS_Dispatch(OS_TS_READY_SYNC);					\
	} while (0)
#else
#define OS_SaveTaskContextSync(a)						\
	do {												\
		OS_WINLINLog(									\
				OS_WINLIN_LOG_DEBUG,					\
				"Going to suspend task thread %#x=%d.",	\
				(a)->taskThreadId,						\
				(a)->taskThreadId);						\
		(void)OS_WINLIN_OSAL_SuspendThread((a)->taskThread);\
		OS_Dispatch(OS_TS_READY_SYNC);					\
	} while (0)
#endif

/** \brief Frees a task's context resources.
 *
 * The macro OS_FreeTaskContext() releases any allocated resources
 * that are held in the task's context. For most processors this
 * macro will be empty.
 *
 * \remarks Not needed by WINLIN.
 *
 * \param[in]	t	Points to the task, of which the context shall be freed.
 */
#define OS_FreeTaskContext(t)

/** \brief Free the current task's context resources.
 *
 * The macro OS_FreeCurrentContext() releases any allocated resources
 * that are held by the CPU (i.e. for the currently-running thread).
 * For most processors this macro will be empty.
 *
 * \remarks Not needed by WINLIN.
 */
#define OS_FreeCurrentContext()

/** \brief Stores a task's current stack pointer.
 *
 * This macro stores the current stack pointer in the outgoing task's context.
 * It is used by OS_LeaveKernel() and only needs to do its job when stack-checking
 * is (possibly) enabled.
 * This is a typical implementation.
 *
 * \remarks On Windows/Linux we don't need this, because the stacks are managed by Windows/Linux itself.
*/
#define OS_StoreTaskSp()

/** \name Functions to get a task's stack pointer.
 *
 * This macro returns the value of the stack pointer from a task's
 * saved context. The saved context is only valid if the task is in
 * one of the states OS_TS_READY_ASYNC, OS_TS_READY_SYNC or
 * OS_TS_WAITING. On some architectures the saved context might
 * be different depending on the task's state.
 * If the system-call handler and ISR handler save the task's SP in
 * its context as part of switching over to the kernel stack, the
 * SP is also available when the task is RUNNING.
 *
 * The "Force" version is called (indirectly) from the dispatcher
 * before the task leaves the running state.
 *
 * \remarks On Windows/Linux this is not needed, because Windows/Linux manages the stacks.
 */
/** @{ */
#define OS_GetTaskSp(t)				OS_NULL
#define OS_GetTaskSpForce(tp,td)	OS_NULL
/** @} */

#ifndef OS_ASM
/** \brief Represents a saved context.
 *
 * This structure stores the current context so that an execption handler
 * can return to the kernel. It is used for interrupt handling and
 * application-specific hook functions.
 *
 * \remarks This structure is not needed on Windows/Linux, but expected by
 * various kernel functions.
 */
struct os_savedcontext_s
{
	/* Dummy element for compatibility. Windows/Linux manages the stacks and hence it's actually not needed.
	 * Nevertheless, it has to be a proper structure, because it's used elsewhere.
	 */
	os_stackelement_t * stackLimit;
};
typedef struct os_savedcontext_s os_savedcontext_t;

#define OS_SAVEDCONTEXT_INIT	\
	{							\
		OS_NULL /*stackLimit*/	\
	}
#endif /* OS_ASM */

#ifndef OS_ASM
/** \brief Represents architecture-dependent extension of os_isr_t.
 *
 * A structure describing an ISR to the kernel.
 *
 * An architecture will typically add a priority field and possibly more.
*/
struct os_archisr_s
{
	/** \brief The configured priority of the ISR.
	 *
	 * This is an AUTOSAR priority and is calculated by the OS generator.
	 * It must not be confused with the thread priorities Windows/Linux uses
	 * to schedule its threads.
	 */
	os_uint8_t prio;

	/** \brief The name of the ISR. */
	os_char_t const* isrName;

	/** \brief Marks this ISR as internal.
	 *
	 * ISRs of internal interrupt are directly executed by a core thread.
	 * This means they don't have a syscall port at their disposal, because
	 * they call AutoCore OS services directly. To put it another way, they
	 * dodge the syscall mechanism. Examples for internal interrupts are
	 * hardware timer or cross-core interrupts.
	 */
	os_boolean_t isrIsInternal;
};
typedef struct os_archisr_s os_archisr_t;
#endif /*!OS_ASM*/

/** \brief Initializes an instance of os_archisr_t.
 *
 * \param prio			The configured priority of an ISR.
 * \param isrName		The name of the ISR.
 * \param isrIsInternal	Indicates, whether this ISR is internal.
 */
#define OS_ARCHISR_INIT(prio, isrName, isrIsInternal)	\
	{					\
		(prio),			\
		(isrName),		\
		(isrIsInternal)	\
	}

/** \brief Gets the name of an ISR.
 *
 * \param[in] pIsr		Points to a constant instance of os_isr_t of which the
 *						name shall be returned.
 *
 * \return The name of \a pIsr.
 */
#define OS_WINLINGetNameOfIsr(pIsr)	((pIsr)->arch.isrName)

/** \brief Determines, whether the ISR belongs to an internal interrupt.
 *
 * \param[in] pIsr		Points to the ISR to test.
 * \return OS_TRUE, when the ISR belongs to an internal interrupt or OS_FALSE otherwise.
 */
#define OS_WINLINIsIsrInternal(pIsr)	((pIsr)->arch.isrIsInternal)

#ifndef OS_ASM
/** \brief WINLIN-specific extensions of ISRs. */
struct os_archisrdynamic_s
{
	/** \brief The Windows/Linux thread running the ISR.
	 *
	 * The ISR run by \c isrThread is the one, which has an instance of
	 * the surrounding structure as one of its members.
	 *
	 * Note, that ISR threads are recycled. This means, an ISR thread
	 * runs the same ISR for the whole application lifetime.
	 */
	os_winlinHandle_t isrThread;

	/** \brief The Windows/Linux TID of \c isrThread. */
	os_winlinTid_t isrThreadId;

	/** \brief The system call port of the ISR thread \c isrThread. */
	os_syscallport_t isrSyscallPort;

	/** \brief This event is raised when the respective interrupt is triggered.
	 *
	 * Task and ISR threads, but also <em>external threads</em> use this event to trigger
	 * an interrupt. External threads are those threads used to connect the Windows/Linux
	 * and AUTOSAR worlds. They are neither task nor ISR threads and hence can't make
	 * AUTOSAR OS syscalls.
	 */
	os_winlinHandle_t isrRequestedEvent;

	/** \brief The enabled flag of this ISR. */
	os_boolean_t isrEnabled;

	/** \brief The ID of the interrupted ISR.
	 *
	 * If an ISR gets interrupted by a higher priority one, the ID of
	 * this interrupted ISR is stored here. It is used to resume the ISR
	 * thread after the higher priority one has finished its work.
	 * If there is no interrupted ISR this field is OS_NULLISR.
	 */
	os_isrid_t idOfInterruptedIsr;

	/** \brief The current interrupt disable level before the ISR was started.
	 *
	 * This member is used to backup and restore the interrupt disable level
	 * before and after an ISR has run.
	 */
	os_intstatus_t savedInterruptDisableLevel;

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
	/** \brief Sync-spot used to control the thread of this ISR. */
	os_linuxThreadSyncSpot_t syncSpot;
#endif
};
typedef struct os_archisrdynamic_s os_archisrdynamic_t;
#endif /*!OS_ASM*/

/** \name Getters and setters for an architecture-dependent, dynamic and ISR-related data.
 *
 * This data is stored in an instance of type os_archisrdynamic_t.
 */
/** @{ */
#define OS_WINLINGetTidOfIsrThread(pIsrDyn)			((pIsrDyn)->arch.isrThreadId)
#define OS_WINLINGetThreadOfIsr(pIsrDyn)			((pIsrDyn)->arch.isrThread)
#define OS_WINLINGetSyscallPortOfIsr(pIsrDyn)		(&((pIsrDyn)->arch.isrSyscallPort))
#define OS_WINLINGetRequestedEventOfIsr(pIsrDyn)	((pIsrDyn)->arch.isrRequestedEvent)
#define OS_WINLINIsIsrEnabled(pIsrDyn)				((pIsrDyn)->arch.isrEnabled)
#define OS_WINLINGetIdOfInterruptIsr(pIsrDyn)		((pIsrDyn)->arch.idOfInterruptedIsr)

/** \def OS_WINLINSetTidOfIsrThread
 * \brief Sets this thread ID of the given ISR thread.
 *
 * \attention When POSIX signals are used to control threads, the thread ID is set by
 * the thread itself when it starts its life. This is because, the creator of that thread
 * is not told the thread ID (by pthread_create()) and hence can't set it.
 *
 * \param pIsrDyn	Points to the dynamic part of an ISR.
 * \param tid		The thread ID to assign to the ISR.
 */
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#define OS_WINLINSetTidOfIsrThread(pIsrDyn, tid)
#else
#define OS_WINLINSetTidOfIsrThread(pIsrDyn, tid)	\
	do {											\
		(pIsrDyn)->arch.isrThreadId = (tid);		\
	} while (0)
#endif

#define OS_WINLINSetThreadOfIsr(pIsrDyn, thread)	\
	do {											\
		(pIsrDyn)->arch.isrThread = (thread);		\
	} while (0)

#define OS_WINLINSetRequestedEventOfIsr(pIsrDyn, hEvent)	\
	do {													\
		(pIsrDyn)->arch.isrRequestedEvent = (hEvent);		\
	} while (0)

#define OS_WINLINSetIsrEnabled(pIsrDyn, state) \
	do {										\
		(pIsrDyn)->arch.isrEnabled = (state);	\
	} while (0)

#define OS_WINLINSetIdOfInterruptedIsr(pIsrDyn, isrId) \
	do {												\
		(pIsrDyn)->arch.idOfInterruptedIsr = (isrId);	\
	} while (0)

#define OS_WINLINSetSavedInterruptDisableLevelOfIsr(pIsrDyn, lvl)	\
	do {															\
		(pIsrDyn)->arch.savedInterruptDisableLevel = (lvl);			\
	} while (0)
/** @} */

/** \name Interrupt disable level backup and restore.
 *
 * These macros are used to save and restore the interrupt disable level
 * before an ISR starts and after it ran. This means, the interrupt disable
 * level change shall be transparent to the interrupted entity.
 */
/** @{ */
#define OS_WINLINRestoreInterruptDisableLevelAfterIsrRan(pIsrDyn)	\
	do {															\
		(void)OS_WINLINSetInterruptDisableLevel(					\
				(pIsrDyn)->arch.savedInterruptDisableLevel,			\
				OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT);	\
	} while (0)

#define OS_WINLINSaveAndSetInterruptDisableLevelBeforeIsrStarts(pIsr)			\
	do {																		\
		os_intstatus_t lvl = OS_WINLINSetInterruptDisableLevel(				\
				(pIsr)->arch.prio,												\
				OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT);				\
		/* OS_WINLINAssembleCoreThreadEvents() must ensure this, i.e. that		\
		 * only those IRQs are observed, which are higher in priority, than		\
		 * the current interrupt disable level.									\
		 */																		\
		OS_WINLINAssert(lvl < (pIsr)->arch.prio);								\
		(pIsr)->dynamic->arch.savedInterruptDisableLevel = lvl;	\
	} while (0)
/** @} */

/** \def OS_WINLINInitializeIsrThread(pIsr)
 * \brief Initializes the Windows/Linux thread of the given ISR.
 *
 * \remarks The syscall initiated and finished events are used in combination with the
 * function OS_WINLINWaitForAllThreadsBeingInPolePosition() to ensure that all threads
 * are in their pole position, before AutoCore OS scheduling is started.
 *
 * \remarks The unify the way ISR threads are started/resumed, the syscall finished event
 * of the ISR's syscall port is also set in this case. When a ISR thread is started
 * for the first time, setting the syscall finished event is not correct and hence
 * must be corrected during initialization. Afterwards the syscall finished event must
 * be set alongside resuming the ISR thread, because the thread got stuck in
 * OS_WINLINFinishedIrq() after having finished handling its most recent request.
 *
 * \param[in] pIsr		Points to a constant instance of os_isr_t. This is the ISR,
 *						of which the Windows/Linux thread shall be initialized.
 */
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#define OS_WINLINInitializeIsrThread(pIsr)															\
	do {																							\
		(pIsr)->dynamic->arch.isrThreadId = OS_WINLIN_OSAL_GetCurrentThreadId();					\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void *)(os_address_t)OS_GetIsrCoreId((pIsr)));			\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[OS_GetIsrCoreId((pIsr))]);	\
		OS_WINLIN_OSAL_TlsSetValue(OS_linuxTlsSyncSpot, &(pIsr)->dynamic->arch.syncSpot);			\
		OS_WINLINSetSyscallPortOfCurrentThread(														\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic));										\
		OS_LINUXControlBlockageOfPosixSignal(OS_FALSE);												\
		OS_WINLIN_OSAL_SignalEventAndWait(															\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallInitiatedEvent,				\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallFinishedEvent);				\
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallFinishedEvent);\
	} while (0)
#else
#define OS_WINLINInitializeIsrThread(pIsr)															\
	do {																							\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void *)(os_address_t)OS_GetIsrCoreId((pIsr)));			\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[OS_GetIsrCoreId((pIsr))]);	\
		OS_WINLINSetSyscallPortOfCurrentThread(														\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic));										\
		OS_WINLIN_OSAL_SignalEventAndWait(															\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallInitiatedEvent,				\
				OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallFinishedEvent);				\
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetSyscallPortOfIsr((pIsr)->dynamic)->syscallFinishedEvent);\
	} while (0)
#endif

/** \name Interrupt Control Routines
 *
 * OS_SetupIsr(), OS_EnableIsr(), OS_DisableIsr() and OS_ClearIsr();
 *
 * Setup/enable/disable/clear a specific ISR.
 */
/** @{ */
#define OS_SetupIsr(isr)		\
	do {						\
		OS_WINLINSetupIsr(isr);	\
	} while (0)
#define OS_EnableIsr(isr)								\
	do {												\
		OS_WINLINSetIsrEnabled((isr)->dynamic, OS_TRUE);\
	} while (0)
#define OS_DisableIsr(isr)									\
	do {													\
		OS_WINLINSetIsrEnabled((isr)->dynamic, OS_FALSE);	\
	} while (0)
#define OS_ClearIsr(isr)															\
	do {																			\
		OS_WINLIN_OSAL_ResetEvent(OS_WINLINGetRequestedEventOfIsr((isr)->dynamic));	\
	} while (0)
/** @} */

/** \name Suspend of Interrupts
 *
 * This macro is used by OS_KernSuspendInterrupts()
 * It disables at the specified level, both in the calling context and
 * in the current context.
 *
 * No special mechanism is in place to avoid multiple evaluations of the
 * parameters. However, in practice each parameter is only used once.
 *
 * The normal version is used where interrupts have not been disabled in the
 * kernel function. In this version we save the current PSW and then disable.
 *
 * The X version is used where interrupts have already been disabled in the
 * kernel function.
 *
 * \param[out]	olp		Points to an instance of type os_oldlevel_t which shal receive
 *						the current interrupt disable level.
 * \param[in]	lvl		The target interrupt disable level.
 */
/** @{ */
#define OS_SuspendInterrupts(olp, lvl)										\
	do {																	\
		*(olp) = (os_oldlevel_t)OS_WINLINSetInterruptDisableLevel((lvl),	\
				OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT);				\
	}while (0)

#define OS_SuspendInterruptsX(olp, lvl, is) \
	do {									\
		OS_SuspendInterrupts(olp, lvl);		\
		*(olp) = (os_oldlevel_t)(is);		\
	} while(0)
/** @} */

/** \brief Resumes interrupts.
 *
 * This macro is used by OS_KernResumeInterrupts()
 * It disables at the specified level, both in the calling context and
 * in the current context.
 *
 * No special mechanism is in place to avoid multiple evaluations of the
 * parameters. However, in practice the parameter is only used once.
 *
 * \param[in] olp		Points to an instance of os_oldlevel_t which contains the
 *						interrupt disable level to be restored.
 */
#define OS_ResumeInterrupts(olp)																		\
	do {																								\
		(void)OS_WINLINSetInterruptDisableLevel(*(olp), OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_RESTORE); \
	} while (0)

/** \brief Whiles away time in case there is nothing to do.
 *
 * This macro is only used at a specific place in the dispatcher, so
 * no protection against use in conditionals etc. is needed.
 * It must present an optimisation barrier to the compiler to
 * ensure that global variables get updated; this can sometimes
 * be achieved by special attributes on an assembler disable or
 * enable macro. Alternatively, call a function.
 *
 * This macro must never return to the caller. Typically it calls
 * OS_GetKernelData()->inKernel with parameter 0, enables interrupts and
 * sits in an endless loop. When an interrupt occurs, the interrupt handler
 * will perform any necessary dispatching. However, some cleaning up might
 * be necessary on some architectures.
 * The endless loop can safely contain a HALT instruction because we
 * never return to it after an interrupt if a task switch is needed.
 *
 * On Windows/Linux, we jump back to the start (i.e. where OS_WINLINCoreSetjmp()
 * was called in OS_WINLINCoreEventLoop()) of the core thread, which is
 * responsible for the core of the caller of OS_Idle().
 * As safety measure, all events of all syscall ports of tasks and ISRs
 * are reset. It is safe to do this, because there is no task currently
 * undertaking a syscall, because otherwise we wouldn't call
 * OS_Idle(). There is also no ISR, because requests to start ISRs are
 * serialized by the core thread and OS_Idle() is call at the END of
 * Cat2 ISRs, i.e. they are done.
 */
#define OS_Idle()											\
	do {													\
		OS_GetKernelData()->inKernel = 0;					\
		(void)OS_IntEnable();								\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_NULL);	\
		OS_WINLINCoreLongjmp();								\
	} while (0)

/** \name Task resumption (sync) control macros.
 *
 * These macro controls how a task is resumed synchronously.
 *
 * \see OS_ResumeTaskSyncHelper().
 */
/** @{ */
#define OS_WINLIN_RESUME_TASK_WITH_PENDING_IRQS 3
#define OS_WINLIN_RESUME_TASK_AFTER_SYSCALL 2
#define OS_WINLIN_RESUME_TASK_AFTER_DISPLACEMENT 1
#define OS_WINLIN_RESUME_TASK_FROM_SCRATCH 0
/** @} */

/** \brief Helper for OS_ResumeTaskSync() and OS_StartTask().
 *
 * \param[in] t		Points to a constant instance of os_task_t, which represents
 *					the task to be resumed or started.
 * \param s			Describes the reason of the resumption.
 *					-	OS_WINLIN_RESUME_TASK_FROM_SCRATCH:
 *						The task \a t is started. Before is was in the suspended state.
 *						All interrupts (if any) are enabled again in this case.
 *					-	OS_WINLIN_RESUME_TASK_AFTER_DISPLACEMENT:
 *						The task \a t is resumed synchronously.
 *						The current interrupt disable level is not changed. This is important,
 *						if resources are involved.
 *					-	OS_WINLIN_RESUME_TASK_AFTER_SYSCALL:
 *						The task \a t may continue after having finished a syscall
 *						and is still the current task to be executed, i.e. the most
 *						recent syscall didn't cause the initiating task \a t to be
 *						displaced. The current interrupt disable level is not changed.
 *						This is important, if resources are involved.
 *					-	OS_WINLIN_RESUME_TASK_WITH_PENDING_IRQS:
 *						The task returns from a syscall \b and there are pending interrupts.
 *						In this case only the syscall finished event is raised,
 *						but the task is not resumed. It's resumed, when the pending interrupts
 *						finish.
 */
#define OS_ResumeTaskSyncHelper(t, s)														\
	do {																					\
		/* Don't call OS_IntEnable() if s is OS_WINLIN_RESUME_TASK_AFTER_SYSCALL.			\
		 * Mind the case, when a task holds a resource shared with an ISR and returns		\
		 * from a syscall. In this case, the interrupt disable level must not be changed.	\
		 * In any other case interrupts must be enabled again, because this helper macro	\
		 * was called indirectly by OS_Dispatch().											\
		 */																			\
		if ((s) == OS_WINLIN_RESUME_TASK_AFTER_DISPLACEMENT) {						\
			OS_WINLINLog(															\
					OS_WINLIN_LOG_DEBUG,											\
					"Going to resume (sync) task %s.",								\
					OS_WINLINGetNameOfTask(t));										\
			(void)OS_IntEnable();													\
		} else if ((s) == OS_WINLIN_RESUME_TASK_FROM_SCRATCH) {						\
			OS_WINLINLog(															\
					OS_WINLIN_LOG_DEBUG,											\
					"Going to start task %s.",										\
					OS_WINLINGetNameOfTask(t));										\
			(void)OS_IntEnable();													\
		} else {																	\
			/* MISRA */																\
		}																			\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfTask(t));	\
		OS_WINLIN_OSAL_SetEvent(OS_WINLINGetSyscallPortOfTask(t)->syscallFinishedEvent);	\
		if ((s) != OS_WINLIN_RESUME_TASK_WITH_PENDING_IRQS) {						\
			(void)OS_WINLIN_OSAL_ResumeThread(OS_WINLINGetThreadOfTask(t));			\
		} else {																	\
			OS_WINLIN_OSAL_WaitForThreadToBecomeBlocked(OS_WINLINGetThreadOfTask(t));		\
		}																			\
		OS_WINLINCoreLongjmp();														\
	} while(0)

/** \brief Resumes a task, which voluntarily relinquished the CPU.
 *
 * This macro resumes a task that voluntarily gave up the CPU by calling
 * Schedule, ActivateTask, WaitEvent, etc.
 *
 * It is here that the transition from kernel-mode to user-mode takes
 * place. This happens by signalling the task thread, that it may continue
 * by its \c syscallFinishedEvent event and resuming it. Raising an
 * event and resuming the thread may seem overkill, but it helps to unify
 * handling of task threads.
 *
 * Note, when a task is resumed the resuming core thread shares the syscall
 * port with the resumed task. This means, that a core thread can directly
 * access this syscall port via its TLS in case a system call is initiated
 * by the resumed task, a.k.a. the <em>current task</em>.
 *
 * \attention This macro makes a long-jump at its end. Hence, it will never
 * "return". The code following it is never executed. The target of this
 * long-jump is the start of the core-event loop.
 *
 * \param[in]	t		Points to the task to be resumed synchronously;
 *						\a t points to an constant instance of os_task_t.
*/
#define OS_ResumeTaskSync(t)													\
	do {																		\
		OS_ResumeTaskSyncHelper(t, OS_WINLIN_RESUME_TASK_AFTER_DISPLACEMENT);	\
	} while (0)

/** \name Task resumption (async) control macros.
 *
 * These macro controls how a task is resumed asynchronously.
 *
 * \see OS_ResumeTaskAsyncHelper().
 */
/** @{ */
#define OS_WINLIN_RESUME_TASK_ASYNC_KEEP_INT_DISABLE_LEVEL 0
#define OS_WINLIN_RESUME_TASK_ASYNC_ENABLE_INTERRUPTS 1
/** @} */

/** \brief Helper macro for OS_ResumeTaskAsync().
 *
 * \param[in]	t		Points to the task to be resumed asynchronously. \a t points
 *						to a constant instance of type os_task_t.
 *
 * \param[in]	s		Describes the way \a t shall be resumed. It must be one of the
 *						following:
 *						-	OS_WINLIN_RESUME_TASK_ASYNC_KEEP_INT_DISABLE_LEVEL:
 *							The task is resumed but the current interrupt disable level
 *							is preserved. This is important, when tasks share resources
 *							with ISRs and are interrupted by ISRs with higher priorities.
 *						-	OS_WINLIN_RESUME_TASK_ASYNC_ENABLE_INTERRUPTS:
 *							The task is resumed and interrupts are enabled again.
 */
#define OS_ResumeTaskAsyncHelper(t, s)												\
	do {																			\
		OS_WINLINLog(																\
				OS_WINLIN_LOG_DEBUG,												\
				"Going to resume (async) task %s.",									\
				OS_WINLINGetNameOfTask(t));											\
		if ((s) == OS_WINLIN_RESUME_TASK_ASYNC_ENABLE_INTERRUPTS) {					\
			(void)OS_IntEnable();													\
		}																			\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfTask(t));	\
		(void)OS_WINLIN_OSAL_ResumeThread(OS_WINLINGetThreadOfTask(t));				\
		OS_WINLINCoreLongjmp();														\
	} while(0)

/** \brief Resumes a task, which was interrupted.
 *
 * This function is essentially the same as OS_ResumeTaskSync() with the
 * difference, that the \c syscallFinishedEvent is not raised. This is because
 * the task was preempted by an ISR (i.e. was interrupted) and not because it
 * initiated a system call some time ago.
 *
 * \param[in]	t		Points to the task to be resumed asynchronously. \a t points
 *						to a constant instance of type os_task_t.
 */
#define OS_ResumeTaskAsync(t) 														\
	do {																			\
		OS_ResumeTaskAsyncHelper(t, OS_WINLIN_RESUME_TASK_ASYNC_ENABLE_INTERRUPTS);	\
	} while (0)

/** \brief Starts the given task.
 *
 * Starting a task is essentially identically to resuming it synchronously.
 * The reason for this is, that task threads are created at startup, but do
 * not run until they are explicitly resumed.
 *
 * Later on, i.e. after startup, task threads are recycled. They are stuck
 * in TerminateTask() or ChainTask(), when they've reached the end of their
 * lifetime. From there, they make a long-jump back to its start in OS_WINLINTaskThread()
 * to re-execute the task's function, when OS_StartTask() is called.
 * This effectively recycles the task thread.
 *
 * Please note, that a task thread becomes stuck in wrappers around the
 * original AutoCore OS TermianteTask() and ChainTask() functions. These wrappers
 * check the return code of those original AutoCore OS services and when they
 * finish successfully, they make the mentioned long-jump. Otherwise, they
 * simply return to execute the task's code following the call of TerminateTask()
 * or ChainTask().
 *
 * \attention This macro never returns, because it's an alias for OS_ResumeTaskSync().
 *
 * \see OS_ResumeTaskSync()
 *
 * \param[in]	t		Points to the task to be started. The type of \a t
 *						is <tt>os_task_t const*</tt>.
 */
#define OS_StartTask(t)													\
	do {																\
		OS_ResumeTaskSyncHelper(t, OS_WINLIN_RESUME_TASK_FROM_SCRATCH);	\
	} while (0)

/** \brief Enter the kernel (reentrant).
 *
 * That inKernel is incremented is important w.r.t. hook functions.
 * Hook functions are executed directly by a core thread and when they
 * initiate a system call inKernel is already 1. Hence it is set to 2
 * before the actual system call is served. This prevents OS_LeaveKernel()
 * from doing any re-scheduling activities.
 */
#define OS_EnterKernel()	(++OS_GetKernelData()->inKernel)

/** \brief Undoes entering the kernel.
 *
 * Undoes the effects of OS_EnterKernel().
 */
#define OS_WINLINUndoEnterKernel()	(--OS_GetKernelData()->inKernel)

/** \brief Prepares the call of a trusted function.
 *
 * This macro "pushes" a trusted function call into the calling stack
 * so that on return from the system-call handler the trusted function
 * is started with the specified parameters, with the protection
 * mechanisms set to "kernel". When the trusted function
 * returns, control must be returned to the place where the
 * CallTrustedFunction system call was executed, with the protection
 * mechanisms set as they were just before the syscall.
 *
 * When there is no system-call mechanism, so we simply call the trusted
 * function. However, the trusted function really ought to be called outside the
 * kernel, we leave the kernel, call the function, then enter the kernel again.
*/
#define OS_PushTrustedFunction(fid,tf,p)	\
	do {									\
		OS_WINLINEnterTrustedFunction();	\
	} while (0)


/** \brief Determines the bounds of the stack used for the call of a trusted function.
 *
 * \param [out] lbp	Points to the location where to store the lower bound. Always set to OS_NULL.
 * \param [out] ubp	Points to the location where to store the upper bound. Always set to OS_NULL.
 *
 * \remarks Not needed, because Windows/Linux manages the stacks.
 * Hence, any stack-related checks by AutoCore OS are bogus. For this reason,
 * OS_NULL is returned to catch such checks early during testing.
 */
#define OS_GetTrustedFunctionStackBounds(lbp,ubp)	\
	do {											\
		*(lbp) = (os_address_t)OS_NULL;				\
		*(ubp) = (os_address_t)OS_NULL;				\
	} while (0)

/** \brief Checks, whether the given stack is large enough to execute a trusted function.
 *
 * \return Always returns OS_FALSE, because Windows/Linux manages all stacks and those stacks
 * offer plenty of space to execute trusted functions.
 */
#define OS_IsTFuncStackSizeInsufficient(lowerSp, upperSp, lowerBound, upperBound)	OS_FALSE

/** \brief Calls an ISR directly.
 *
 * The macro's parameters must be simple variables, not expressions. This
 * should not be a problem, however, because it's only used in the
 * Cat2Entry and Cat2KEntry routines.
 *
 * \param [in] isr		Points to a constant instance of os_isr_t representing
 *						the ISR to call.
 * \param id			The ID of the ISR.
 */
#define OS_CallIsrDirect(isr, id)	\
	do {							\
		(*((isr)->func))();			\
	} while(0)

/** \brief Calls an ISR indirectly.
 *
 * This macro calls an ISR.
 *
 * If Autosar semantics are specified, the ISR is called via a special
 * mechanism that saves the context (typically a stack pointer and other
 * registers, cf. setjmp/longjmp) so that a killed ISR can appear to
 * return correctly.
 *
 * The macro's parameters must be simple variables, not expressions. This
 * should not be a problem, however, because it's only used in the
 * Cat2Entry routine.
 *
 * \attention On Windows/Linux interrupts must always be called directly. Killing
 * of interrupts is not supported.
 *
 * \param [in] isr		Points to a constant instance of os_isr_t representing
 *						the ISR to call.
 * \param id			The ID of the ISR.
*/
#define OS_CallIsrIndirect(isr, id) \
	do {							\
		OS_WINLINPanic();			\
	} while(0)

#ifndef OS_ASM
/** \brief Enumerates the modes of OS_WINLINSetInterruptDisableLevel(). */
enum os_setInterruptDisableLevelMode_e
{
	 /** \brief Sets the interrupt disable level as indicated. */
	OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT = 0,

	/** \brief Raises the interrupt disable level to OS_intDisableLevel,
	 * but only if current level is smaller.
	*/
	OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_CONDITIONAL,

	/** \brief Restores the interrupt disable level as indicated. */
	OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_RESTORE
};
typedef enum os_setInterruptDisableLevelMode_e os_setInterruptDisableLevelMode_t;
#endif /* !OS_ASM */

/** \name Interrupt Control Macros
 *
 * The macros
 * -	OS_IntEnable(),
 * -	OS_IntDisable(),
 * -	OS_IntRestore() and
 * -	OS_IntDisableAll()
 * .
 * are used to control the current interrupt-disable level.
 *
 * These routines enable, disable and restore previous state or disable-level
 * of interrupts.
 *
 * OS_IntEnable and OS_IntDisable both return the old value of the interrupt
 * state or disable level for use in a later call of OS_IntRestore(). The
 * parameter \a p of OS_IntRestore() must be set to the mentioned return values
 * of the other macros.
 *
 * But keep in mind, that the kernel in WINDOWS/WIN32X86 is not interruptible.
 * So once a syscall is running it runs to completion without interruptions.
 * Interrupts are only served, when they are explicitly observed by a core thread
 * in its event loop.
 */
/** @{ */
/** \brief The interrupt-disable level to enable interrupts :-) .
 *
 * If the current interrupt disable level is set to this value, then all interrupts
 * are effectively enabled. This is because, this is the lowest interrupt disable level
 * and all assigned ISR priorities are larger. Hence when were are at this disable
 * level, we can be interrupted by any ISR.
 *
 * As a consequence do all assigned ISR priorities start at OS_WINLIN_INTERRUPT_ENABLE_LEVEL + 1.
 */
#define OS_WINLIN_INTERRUPT_ENABLE_LEVEL OS_U(0)

#define OS_IntEnable()										\
	(OS_WINLINSetInterruptDisableLevel(						\
			OS_WINLIN_INTERRUPT_ENABLE_LEVEL,				\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT))

#define OS_IntDisable()										\
	(OS_WINLINSetInterruptDisableLevel(						\
			OS_kernDisableLevel,							\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT))

#define OS_IntDisableConditional()							\
	(OS_WINLINSetInterruptDisableLevel(						\
			OS_kernDisableLevel,							\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_CONDITIONAL))

#define OS_IntDisableAll()									\
	(OS_WINLINSetInterruptDisableLevel(						\
			OS_intDisableLevelAll,							\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT))

#define OS_IntDisableMax()									\
	(OS_WINLINSetInterruptDisableLevel(						\
			OS_intDisableLevelMax,							\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT))

#define OS_IntRestore(p)									\
	((void)OS_WINLINSetInterruptDisableLevel(				\
			(p),											\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_RESTORE))
/** @} */

/** \brief Sets the ISR resource level.
 *
 * This macro is called by GetResource() and ReleaseResource() to disable
 * interrupts up to the ceiling level of the resource, for a resource that
 * is used in ISRs and tasks.
 * It disables at the specified level, both in the current context and
 * in the context that called the system service.
 *
 * \param	prio		The resource priority to protect a resource from concurrent
 *						access. Let \c maxIntPrio be the maximum priority assignable to
 *						an ISR, then this value is always out of
 *						<tt>[OS_NULLISRPRIO, OS_NULLISRPRIO + maxIntPrio]</tt>.
 *						See OSEK Priority Ceiling Protocol for the underlying theory.
 */
#define OS_SetIsrResourceLevel(prio)							\
	do {														\
		(void)OS_WINLINSetInterruptDisableLevel(				\
				(prio) - OS_NULLISRPRIO,						\
				OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT);	\
	} while (0)

/** \brief Gets the interrupt
 *
 * This macro returns the current interrupt-locking level. It is used in
 * GetResource() to save the old level before setting the new.
 *
 * \param[in]	is		The interrupt-disable level at the time the resource should be
 *						acquired. This is not the current interrupt-disable level!
 *
 * \return The <em>resource priority</em> corresponding to \a is to protect the resource from
 * concurrent access. This resource priority is used in priority comparisons to manage
 * access to resources. This value may be directly passed to OS_SetIsrResourceLevel().
 */
#define OS_GetInterruptPriority(is)	(OS_NULLISRPRIO + (is))

/** \brief Gets the ISR's priority.
 *
 * \param[in] isrp		Points to a constant instance of os_isr_t.
 *
 * \return Return the ISR's priority for use in GetResource.
 */
#define OS_GetIsrResPrio(isrp)	((isrp)->arch.prio)

/** \brief Returns the current value of the stack pointer.
 *
 * It is used in the stack check code, and in the stack initialisation
 * in OS_KernStartOs().
 *
 * \remarks Not needed on WINLIN because Windows/Linux manages the stacks.
 * Hence, any stack-related checks by AutoCore OS are bogus. For this reason,
 * OS_NULL is returned to catch such checks early during testing.
 *
 * \return Always returns OS_NULL.
 */
#define OS_GetCurrentSp()	OS_NULL

/** \name Functions to determine access rights to task and ISR memory.
 *
 * These two macros check whether a non-trusted task/ISR has access to the
 * specified memory region. The private data and stack areas are checked
 * by the architecture-independent code, so we're only interested in the
 * global areas, which are read-only and possibly executable.
 *
 * \remarks On WINLIN there is no protection for memory assigned
 * to AUTOSAR entities, e.g. task or ISRs. Therefore, access is always granted.
 */
/** @{ */
#define OS_TaskMemoryAccess(t,f,l)	(OS_MA_READ | OS_MA_WRITE | OS_MA_EXEC | OS_MA_STACK)
#define OS_IsrMemoryAccess(i,f,l)	(OS_MA_READ | OS_MA_WRITE | OS_MA_EXEC | OS_MA_STACK)
/** @} */

/** \name Architecture-specific hook call routines.
 *
 * These macros call application-specific hook functions when Autosar sematics are configured.
 *
 * The hook is called via OS_CallAppHook().
 * OS_CallAppHook() saves its context so that if the hook misbehaves
 * the error handler can kill it by appearing to return from the call to
 * OS_CallAppHook(). For this reason setting the hook's saved
 * context to null is done here rather than in OS_CallAppHook().
 *
 * The startup and shutdown hooks can safely share a return context because
 * the two will never be used simultaneously.
 *
 * The macros' parameters must be simple variables, not expressions. This
 * should not be a problem, however, because they are only used in internal
 * kernel functions.
 *
 * \remarks On WINLIN hook functions are always called directly.
 */
/** @{ */
#define OS_ArchCallStartupHook(app)	\
	do {							\
		OS_WINLINPanic();			\
	} while (0)
#define OS_ArchCallShutdownHook(app, code)	\
	do {									\
		OS_WINLINPanic();					\
	} while (0)
#define OS_ArchCallErrorHook(app, code)	\
	do {								\
		OS_WINLINPanic();				\
	} while (0)
/** @} */

/** \brief Determines the limit of an ISR stack.
 *
 * \remark The macro value <tt>((id) != OS_NULL ? 0 : 0)</tt> looks strange at first
 * and indeed is. It's done this way to suppress the warning
 * <em>warning: variable 'isrd' set but not used [-Wunused-but-set-variable]</em>
 * reported in file OS_CanWrite(). Note, that function is never called in WINLIN/WIN32X86!
 * OS_GetIsrStackLimit() is also used in other places, than OS_CanWrite().
 *
 * \param[in] id		Points to an instance of os_isrdynmaic_t of the ISR, of
 *						which the stack limit shall be determined.
 *
 * \return Always returns OS_NULL, because this macro is only relevant in the
 * context of indirectly called ISRs. On Windows/Linux ISRs are always called directly.
 * Hence, there is actually no need for this macro, but it's referenced by some
 * kernel code. Under normal circumstances (i.e. not Windows/Linux), the macro
 * OS_CallIsrIndirect() sets up all the required information to make this macro work.
 */
#define OS_GetIsrStackLimit(id)	OS_NULL

/** \def OS_CFG_INITARCH
 * \brief Name of the function to initialize the architecture-dependent
 * parts of the WINLIN port.
 *
 * This macro defines the architecture-specific initialization
 * function that gets called as the first initialization function
 * in StartOS.
*/
#define OS_CFG_INITARCH	(&OS_InitArchWINLIN)
#ifndef OS_ASM
extern void OS_InitArchWINLIN(void);
#endif

/** \brief Gets the kernel array pointer from caller's TLS.
 *
 * \return	Pointer to the os_kerneldata_t instance, which belongs to
 *			the core of caller.
 */
#ifndef EB_STATIC_CHECK
/* When this code is statically checked, the generic version of OS_GetKernelData() is used to avoid
 * lots of MISRA-C violations.
 */
#define OS_GetKernelData()	((os_kerneldata_t *)OS_WINLIN_OSAL_TlsGetValue(OS_winlinTlsKernPtr))
#endif

#ifndef OS_ASM
struct os_archkerneldata_s
{
	/** \brief The Windows/Linux TID of the core thread.
	 *
	 * This is the TID of the core thread responsible for the core
	 * to which this os_archkerndata_s instance is connected to.
	 *
	 * It is set as soon as a core thread is started by StartCore().
	 * An exception is the core thread for the first core (a.k.a. master core),
	 * which is the main application thread and hence started by Windows/Linux.
	 * The reason for this is, that someone must be already there to serve
	 * system calls, when the whole system starts up.
	 *
	 * The special value 0 is used to indicate, that the respective core thread
	 * hasn't been started yet.
	 */
	os_winlinTid_t		coreThreadId;

	/** \brief A core's current interrupt disable level.
	 *
	 * The current interrupt disable level of a core. Interrupts with a lower
	 * or equal priority are disabled, than the current value of this member.
	 */
	os_intstatus_t coreInterruptDisableLevel;

	/** \brief Counts the number of trusted functions currently executed on a core.
	 *
	 * This counter is initially zero and is incremented by OS_PushTrustedFunction().
	 * When the trusted function finished execution it is decremented again.
	 */
	os_long_t coreInTrustedFunction;

	/* \brief The currently running ISR.
	 *
	 * This is effectively a copy of the field os_kerneldata_t::isrCurrent, which is used
	 * for the same purpose. The reason why this field is duplicated here is that the core
	 * thread needs to remember the currently running ISR, especially when OS_WINLINServeIrq()
	 * is called quickly back to back. Relying on os_kerneldata_t::isrCurrent might be
	 * insufficient in this case, because a resumed ISR thread might not yet have had the time
	 * to set this field, when the second invocation of OS_WINLINServeIrq() comes along.
	 */
	os_isrid_t coreIsrCurrent;
};

typedef struct os_archkerneldata_s os_archkerneldata_t;

#define OS_ARCHKERNELDATA_INIT	\
		{						\
			0u,					\
			0u,					\
			0,					\
			OS_NULLISR,			\
		}
#endif

/** \def OS_SIZEOF_ARCHDATA
 * \brief The size in bytes of os_archkerneldata_t.
 *
 * \attention Although it's not permitted to use sizeof() in the definition of this macro
 * there is no problem for WINLIN/WIN32X86, because there are no assembly language parts.
 */
#define OS_SIZEOF_ARCHDATA	(sizeof(os_archkerneldata_t))

/** \def OS_WINLINGetCoreThreadIdOfCore()
 * \brief Gets the Windows/Linux TID of the core thread of the given core.
 *
 * \param coreId		The ID of the core of which the core thread shall be returned.
 *
 * \return The Windows/Linux TID of the core thread of core \a coreId.
 */
#define OS_WINLINGetCoreThreadIdOfCore(coreId) (OS_kernel_ptr[(coreId)]->archData.coreThreadId)

/**\def OS_WINLINSetCoreThreadIdOfCore()
 * \brief Sets the ID of the core thread responsible for the given core.
 *
 * \param coreId		The ID of the core of which to set the TID of its core thread.
 * \param tid			The TID of the core thread responsible for core \a coreId.
 */
#define OS_WINLINSetCoreThreadIdOfCore(coreId, tid)			\
	do {														\
		OS_kernel_ptr[(coreId)]->archData.coreThreadId = (tid);	\
	} while (0)

/** \def OS_WINLINGetCoreThreadId()
 * \brief Gets the Windows/Linux TID of the core thread of the current core.
 *
 * \return The Windows/Linux TID of the core thread of the core, on which the
 * caller currently runs.
 */
#define OS_WINLINGetCoreThreadId()	(OS_GetKernelData()->archData.coreThreadId)

/** \def OS_WINLINSetCoreThreadId(id)
 * \brief Sets the Windows/Linux TID of the new core thread for the core, on which
 *		  the caller currently runs.
 *
 * \param[in]	id		The Windows/Linux TID of the new core thread.
 */
#define OS_WINLINSetCoreThreadId(id)						\
	do {													\
		OS_GetKernelData()->archData.coreThreadId = (id);	\
	} while (0)

/** \brief Gets the interrupt disable level of the current core.
 *
 * \return The current interrupt disable level of the core on which the caller runs.
 */
#define OS_WINLINGetCoreInterruptDisableLevel() \
	(OS_GetKernelData()->archData.coreInterruptDisableLevel)

/** \brief Sets the interrupt disable level of the current core.
 *
 * \param	lvl		The new interrupt disable level for the core on which the caller runs.
 */
#define OS_WINLINSetCoreInterruptDisableLevel(lvl)							\
	do {																	\
		OS_GetKernelData()->archData.coreInterruptDisableLevel = (lvl);	\
	} while (0)

/** \brief Gets the id of the currently running ISR.
 *
 * \return The id of the currently running ISR.
 */
#define OS_WINLINGetCoreIsrCurrent()	(OS_GetKernelData()->archData.coreIsrCurrent)

/**
 * \brief Remembers the id of the currently running ISR.
 *
 * \param isrid		The id of the currently running ISR.
 */
#define OS_WINLINSetCoreIsrCurrent(isrid)						\
	do {														\
		OS_GetKernelData()->archData.coreIsrCurrent = (isrid);	\
	} while (0)

/** \name Trusted function helper functions.
 *
 * The execution of trusted functions must be framed by the following macros.
 * Before they start to execute, OS_WINLINEnterTrustedFunction() must be called
 * to signal that this is going to happen. After their execution, OS_WINLINLeaveTrustedFunction()
 * must be called. This means, the macro calls must always be balanced.
 *
 * The reason for this is that the call of a trusted function is initiated by
 * OS_KernCallTrustedFunction() by the kernel but is actually performed in the
 * context of the calling task or ISR. This part is managed by OS_WINLINSyscall().
 */
/** @{ */
#define OS_WINLINEnterTrustedFunction() 						\
	do {														\
		++OS_GetKernelData()->archData.coreInTrustedFunction;	\
	} while (0)

#define OS_WINLINLeaveTrustedFunction()											\
	do {																		\
		--OS_GetKernelData()->archData.coreInTrustedFunction;					\
		OS_WINLINAssert(OS_GetKernelData()->archData.coreInTrustedFunction >= 0);	\
	} while (0)

/** \brief Sets the trusted function counter.
 *
 * \param v		The new value of the inTrustedFunction counter of the current core.
 */
#define OS_WINLINSetInTrustedFuntion(v)								\
	do {															\
		OS_GetKernelData()->archData.coreInTrustedFunction = (v);	\
	} while (0)

/** \brief Checks, if the calling thread is currently executing a trusted function.
 *
 * \retval OS_FALSE	The calling thread is \e not executing a trusted function.
 * \retval OS_TRUE	The calling thread is currently executing a trusted function.
 */
#define OS_WINLINIsCurrentThreadInTrustedFunction() \
	(OS_GetKernelData()->archData.coreInTrustedFunction != 0)
/** @} */

/** \brief Initializes a core thread representing the given core.
 *
 * Only the bare minimum of initialization is done by this function.
 * The environment for OS_GetMyCoreId() as well as the kernel data pointer
 * is set up. The interrupt level allows interrupts to occur.
 *
 * \param coreId		The ID of the core represented by the core thread to be initialized.
 */
#define OS_WINLINInitializeCoreThread(coreId)											\
	do {																				\
		/* Initialize the environment of Os_MyCore(). */								\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void*)(os_address_t)(coreId));						\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[(coreId)]);				\
		/* There is no current task yet. Hence there is no syscall port to share. */	\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_NULL);								\
		OS_WINLINSetCoreThreadId(OS_WINLIN_OSAL_GetCurrentThreadId());						\
		OS_WINLINSetCoreInterruptDisableLevel(OS_WINLIN_INTERRUPT_ENABLE_LEVEL);		\
		OS_WINLINSetInTrustedFuntion(0);												\
	} while (0)

/** \brief Signals that the software environment has been initialized. */
#define OS_WINLINSignalEndOfInitialization() (OS_winlinInitialized = OS_TRUE)

/** \brief Determines whether the software environment has already been initialized. */
#define OS_WINLINIsInitialized() (OS_winlinInitialized)

/** \brief Used to suppress compiler warnings concerning OS_WINLINLog(), when just two arguments are passed.
 *
 * To suppress the compiler warning "warning: ISO C99 requires rest arguments to be used [enabled by default]"
 * pass this macro to OS_WINLINLog() as follows, when just the first two parameters are used:
 * OS_WINLINLog(OS_WINLIN_LOG_DEBUG, "my message", OS_WINLINLOG_UNUSED_PARAM)
 */
#define OS_WINLINLOG_UNUSED_PARAM 0

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif
#endif

/** \brief Logs a message.
 *
 * By default the function OS_WINLINDefaultLogFunction() is used for logging.
 * One may change this default and hook up a custom log function.
 *
 * \param severity	See OS_WINLINDefaultLogFunction().
 * \param message   See OS_WINLINDefaultLogFunction().
 */
/* Possible diagnostic TOOLDIAG-1 <8> */
#define OS_WINLINLog(severity, message, ...)	\
	OS_WINLINDefaultLogFunction(				\
			severity,							\
			__FILE__,							\
			__LINE__,							\
			message,							\
			##__VA_ARGS__)

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif

/** \brief Asserts that the given condition holds true.
 *
 * If the condition \a condition is false an error message is logged
 * and a core dump is generated.
 *
 * Otherwise, i.e. if \a condition is true, this macro has no side-effects.
 *
 * \param condition		The boolean condition to be asserted.
 */
#define OS_WINLINAssert(condition)		\
	do {								\
		if ( !(condition) )				\
		{								\
			OS_WINLINPanic();			\
		}								\
	} while (0)

/** \brief Initializes a FRC thread.
 *
 * \attention FRC threads must not make AUTOSAR OS syscalls. They have no syscall port.
 *
 * \param[in] pHwt			Points to a constant instance of type os_hwt_t.
 *							It's the hardware timer for which the calling FRC thread is responsible for.
 * \param[in] pFrcThreadId	Points to the ID of the FRC thread.
 */
#define OS_WINLINInitializeFrcThread(pHwt, pFrcThreadId)		\
	do {														\
		*(pFrcThreadId) = OS_WINLIN_OSAL_GetCurrentThreadId();	\
		OS_WINLINAssumeFrcIdentity(pHwt);						\
	} while (0)

/** \brief Assumes the identity of the given HW timer.
 *
 * \param [in] pHwt		Points to a constant instance of type os_hwt_t.
 *						It's the hardware timer for which the calling FRC thread is responsible for.
 */
#define OS_WINLINAssumeFrcIdentity(pHwt)										\
	do {																		\
		os_coreid_t const _coreId = OS_GetTimerCoreId((pHwt));					\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsCoreId, (void *)_coreId);		\
		OS_WINLIN_OSAL_TlsSetValue(OS_winlinTlsKernPtr, OS_kernel_ptr[_coreId]);\
		OS_WINLINSetSyscallPortOfCurrentThread(OS_NULL);						\
	} while (0)

/** \brief Checks for hook functions or alarm callbacks.
 *
 * \param inFunc		The value returned by OS_GetKernelData()->inFunction to be analyzed,
 *						if it designates a hook function or alarm callback.
 *						The value passed must have no side-effects, when it's evaluated.
 *
 * \retval OS_FALSE	When \a inFunc does not represent a hook function or alarm callback.
 * \retval OS_TRUE	The argument \a inFunc designates a hook function or alarm callback.
 */
#define OS_WINLINInHookOrAlarmCallback(inFunc) \
	(((OS_INHOOKMIN <= (inFunc)) && ((inFunc) <= OS_INHOOKMAX)) || ((inFunc) == OS_INACB))

/** \brief Checks if the caller is an external thread.
 *
 * <em>External threads</em> belong to the glue code between the AUTOSAR and Windows/Linux
 * world and hence do not run AUTOSAR-related software, e.g. like tasks or ISRs. Those
 * threads must not make any AUTOSAR OS syscalls, because they don't have the necessary
 * resources. They may only trigger interrupts via OS_WINLINTriggerInterrupt().
 *
 * \retval OS_FALSE The caller is not an external thread.
 * \retval OS_TRUE	The caller is an external thread and hence must make no AUTOSAR OS
 * syscalls. It may use Windows/Linux services at will, because it is never suspended by
 * AUTOSAR OS.
 */
#define OS_WINLINIsThisAnExternalThread()										\
	((OS_GetKernelData() == OS_NULL)											\
			&& (OS_WINLINGetSyscallPortOfCurrentThread() == OS_NULL)			\
			&& ((os_coreid_t)OS_GetMyCoreId() == 0u))

/** \brief Signals, that the OS was started on the given core.
 *
 * \param coreId	The ID of the core, on which the OS was started.
 */
#define OS_WINLINSignalOsWasStartedOnCore(coreId)	\
	do {											\
		OS_winlinOsWasStarted[(coreId)] = OS_TRUE;	\
	} while (0)

/** \brief Checks, if the OS was started on the given core.
 *
 * \param coreId	The ID of the core to check, if the OS was started for that core.
 *
 * \retval OS_FALSE	The OS hasn't been started yet on core \a coreId.
 * \retval OS_TRUE	The OS is already running on core \a coreId.
 */
#define OS_WINLINWasOsStartedOnCore(coreId) (OS_winlinOsWasStarted[(coreId)])

/** \brief Determines, if the given core is used by EB tresos AutoCore OS. */
#define OS_WINLINIsOsUsedOnCore(coreId) (OS_kernel_ptr[(coreId)] != OS_NULL)

#ifndef OS_ASM
extern os_boolean_t OS_winlinOsWasStarted[OS_N_CORES_MAX];
#endif	/* !OS_ASM */

#ifndef OS_ASM
/** \brief Enumerates the flags passed to OS_WINLINUseHostOsService(). */
enum os_winlinUseHostOsService_e
{
	OS_WINLIN_BEGIN_USE_OF_HOST_OS_SERVICE = 0,
	OS_WINLIN_FINISH_USE_OF_HOST_OS_SERVICE
};
typedef enum os_winlinUseHostOsService_e os_winlinUseHostOsService_t;
#endif	/* !OS_ASM */

#ifndef OS_ASM
/** \brief Represents the parameters to start a core thread. */
struct os_winlincorethreadparams_s
{
	/** \brief The ID of the core represented by a core thread. */
	os_coreid_t coreId;
};
typedef struct os_winlincorethreadparams_s os_winlincorethreadparams_t;

extern os_winlincorethreadparams_t OS_winlinCoreThreadParams[OS_N_CORES_MAX];
#endif	/* !OS_ASM */

/* See Os_arch.h for details. */
#define OS_ArchPreDynamicStop() 	\
	do {							\
		OS_WINLINPreDynamicStop();	\
	} while (0)
#ifndef OS_ASM
extern void OS_WINLINPreDynamicStop(void);
extern void OS_WINLINPreDynamicStopInit(void);
#endif	/* !OS_ASM */

#ifndef OS_ASM
/** \brief Enumerates the types of threads. */
enum os_winlinThreadType_e
{
	OS_WINLIN_THREAD_TYPE_FRC,
	OS_WINLIN_THREAD_TYPE_TASK,
	OS_WINLIN_THREAD_TYPE_ISR,
	OS_WINLIN_THREAD_TYPE_CORE,
	OS_WINLIN_THREAD_TYPE_EXTERNAL
};
typedef enum os_winlinThreadType_e os_winlinThreadType_t;
#endif /* !OS_ASM */

#ifndef OS_ASM
extern os_winlinTlsIndex_t OS_winlinTlsCoreId;
extern os_winlinTlsIndex_t OS_winlinTlsKernPtr;
extern os_winlinTlsIndex_t OS_winlinTlsSyscallPort;
extern os_winlinTlsIndex_t OS_linuxTlsSyncSpot;
extern os_int_t OS_linuxRealTimePriority;
extern os_winlinLogMessageSeverity_t OS_winlinLogMessageFilterSeverity;
extern os_boolean_t OS_winlinInitialized;
extern os_winlinTimerValue_t OS_winlinTimeWarpFactor;
extern os_winlinTimerValue_t const OS_winlinNsPerTick;
extern os_int_t OS_winlinArgc;
extern char** OS_winlinArgv;
extern os_isrid_t const OS_crossCoreIsrIds[OS_N_CORES_MAX];
#endif	/* !OS_ASM */

/** \fn OS_LeaveKernel
 *
 * The Windows/Linux port makes use of this this function, although only syscall kernels are supported,
 * i.e. OS_KERNEL_TYPE == OS_SYSTEM_CALL must always hold true. Normally, this prototype is only
 * declared, when OS_KERNEL_TYPE == OS_FUNCTION_CALL is true.
 */
#ifndef OS_ASM
extern os_unsigned_t OS_LeaveKernel(os_unsigned_t);
#endif

#endif /* OS_KERNEL_WINLIN_H */
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
