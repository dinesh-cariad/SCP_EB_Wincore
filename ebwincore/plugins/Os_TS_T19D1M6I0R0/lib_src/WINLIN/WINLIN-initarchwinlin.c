/* WINLIN-initarchwinlin.c
 *
 * This file contains the OS_InitArchWINLIN function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-initarchwinlin.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

#include <Os_kernel.h>
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#include <LINUX/Os_LINUX_threadsyncspot.h>
#endif

#include <memmap/Os_mm_code_begin.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
#endif

static void OS_WINLINCreateTaskThreads(void);

/** \brief Creates threads for all tasks assigned to the current core.
 *
 * This creates threads for all tasks to be run on the current core.
 * All threads are created in the suspended state and hence no task
 * runs when this function returns. Tasks start to run, when the they
 * explicitly selected by the AutoCore OS scheduler, i.e. by the
 * OS_Dispatch() function.
 */
static void OS_WINLINCreateTaskThreads(void)
{
	const os_coreid_t myCore = OS_GetMyCoreId();

	os_taskid_t taskIdIter;
	for (taskIdIter = 0; taskIdIter < OS_nTasks; taskIdIter++)
	{
		os_task_t const* const tp = &OS_taskTableBase[taskIdIter];
		if (OS_GetTaskCoreId(tp) == myCore)
		{
			os_winlinHandle_t taskThread;
			os_winlinTid_t taskThreadTid;
			os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfTask(tp);

			OS_WINLINInitializeSyscallPort(pSyscallPort);
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
			OS_LINUXInitializeThreadSyncSpotForSuspendedThread(
					OS_LINUXGetThreadSyncSpotForTask(tp));
#endif
			taskThread = OS_WINLIN_OSAL_CreateThread(
					&OS_WINLINTaskThread,
					(void *) tp,
					&taskThreadTid,
					OS_WINLINGetNameOfTask(tp),
					OS_WINLIN_THREAD_TYPE_TASK);
			OS_WINLINSetThreadOfTask(tp, taskThread);
			OS_WINLINSetTidOfTaskThread(tp, taskThreadTid);
			OS_WINLINSetInterruptedStateOfTask(tp, OS_FALSE);

			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Created thread %#x=%d for task %s.",
					taskThreadTid,
					taskThreadTid,
					OS_WINLINGetNameOfTask(tp));
		}
	}
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif

/** \brief Initializes the environment of one core.
 *
 * This function does architecture-dependent initialization of the
 * environment of one core.
 */
void OS_InitArchWINLIN(void)
{
	OS_WINLINCreateTaskThreads();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
