/* WINLIN-globaldata.c
 *
 * This file contains definition of global data.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-globaldata.c 25266 2016-11-28 10:20:41Z olme8414 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_var_begin.h>

/* TLS index for kernel pointer TLS - shall be only written from this function */

/** \brief The thread local storage index for the core ID.
 *
 * The core ID of a thread is put into its local storage into the slot at
 * this index. A thread uses this index and OS_WINLIN_OSAL_TlsGetValue() to get
 * access to a thread core ID.
 *
 * Please note, that this core ID must be interpreted in the context of AUTOSAR.
 * It's \e not the ID to identify cores in the host PC.
 *
 * The respective TLS slot stores a pointer to an instance of type os_winlinTid_t.
 */
os_winlinTlsIndex_t OS_winlinTlsCoreId;

/** \brief The thread local storage index for the kernel array pointer.
 *
 * There is one entry per core in the kernel array OS_kernel_ptr[] and a pointer to this
 * entry is stored in the slot at index \c OS_winlinTlsKernPtr in a thread's
 * local storage.
 *
 * The respective TLS slot stores a pointer to an instance of type os_kerneldata_t.
 *
 * \see OS_winlinTlsCoreId
 */
os_winlinTlsIndex_t OS_winlinTlsKernPtr;

/** \brief The thread local storage index for the current system call port.
 *
 * AutoCore OS system calls are commissioned by a task or ISR thread at
 * a core thread. This means, that task, ISR or hook threads never run AutoCore OS
 * code directly; they must ask a core thread to do this on their behalf.
 *
 * Syscall ports are the means to achieve this. Each task, ISR or hook thread
 * has its own port through which it can initiate syscalls.
 *
 * The respective TLS slot stores a pointer to an instance of type os_syscallport_t.
 *
 * \see OS_winlinTlsCoreId
 */
os_winlinTlsIndex_t OS_winlinTlsSyscallPort;

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
/** \brief The thread local storage index for a thread's sync-spot.
 *
 * When threads are controlled using POSIX signals a thread's sync-spot is stored in
 * TLS using this index. Only task and ISR threads have a non-NULL value in their TLS,
 * which is a pointer to os_archcontext_t::syncSpot or os_archisrdynamic_t::syncSpot
 * respectively.
 */
os_winlinTlsIndex_t OS_linuxTlsSyncSpot;
#endif	/* OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL */

#if (OS_ARCH == OS_LINUX)
/** \brief The user-specified real-time priority.
 *
 * When the user specifies a real-time priority it serves as the base priority for
 * all threads created by AutoCore OS. The scheduling policy used is SCHED_RR.
 * A priority of -1 indicates, that no real-time priority has been specified by the user.
 *
 * The value OS_linuxRealTimePriority (if not equal to -1) is used for task and ISR threads,
 * the value OS_linuxRealTimePriority + 1 for the FRC thread and
 * the value OS_linuxRealTimePriority + 2 for the core threads.
 */
os_int_t OS_linuxRealTimePriority = -1;
#endif

/** \brief The filter threshold for log messages.
 *
 * Only when log messages are more or equally severe than indicated by this filter threshold
 * they may pass the filter to reach the record. Less severe log messages are discarded.
 */
os_winlinLogMessageSeverity_t OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_WARNING;

/** \brief One-time initialization flag.
 *
 * This flag prevents that OS_WINDOWSPreStartOs() is executed multiple
 * times. It may be called multiple times, but its body is only executed
 * once.
 */
os_boolean_t OS_winlinInitialized = OS_FALSE;

/** \name Command-line Arguments.
 *
 * These variables store the values passed by Windows to the main() function.
 * They are needed by OS_WINDOWSCoreThread() to properly restart from main()
 * for another core than the main core.
 * They are set by OS_WINDOWSPreStartOs(), when it's called for the first time.
 */
/** @{ */
os_int_t OS_winlinArgc = 0;
os_char_t** OS_winlinArgv = OS_NULL;
/** @} */

/** \brief The parameters to start core threads.
 *
 * There is one entry per core thread in this array. It is used by
 * OS_ArchStartCore() to pass parameters to a core thread.
 */
os_winlincorethreadparams_t OS_winlinCoreThreadParams[OS_N_CORES_MAX];

/** \brief Indicates, whether the OS was started on a core.
 *
 * This array contains one flag per core. The flag is set by OS_ArchUserStartOs()
 * after the OS was started on the respective core, but before the core's event
 * loop begins to run.
 *
 * The flags are "sticky", i.e. once set they remain set until application termination.
 */
os_boolean_t OS_winlinOsWasStarted[OS_N_CORES_MAX];

#include <memmap/Os_mm_var_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
