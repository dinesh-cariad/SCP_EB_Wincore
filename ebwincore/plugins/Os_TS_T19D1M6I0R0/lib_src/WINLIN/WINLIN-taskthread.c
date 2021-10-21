/* WINLIN-taskthread.c
 *
 * This file contains the implementation of the bootstrap function of task threads.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-taskthread.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastToPtrFromIntDiffSize
 * Cast to pointer from integer of different size [-Wint-to-pointer-cast]
 *
 * Reason:
 * An integer must be converted into a pointer, because the pointer type is required by
 * the Pthreads API for thread local storage, but the item to put there is a plain integer.
 */

#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief The start function of a task thread.
 *
 * A task thread always starts its life in this function. It is passed to
 * Windows when a new task thread is created. Because task thread are recycled,
 * there is only one thread per task and it is always the same for the whole
 * application lifetime. The recycling is realized by setjmp/longjmp, so that
 * at task termination the respective thread can jump to the beginning to run
 * the same task again.
 *
 * \param[in]	param	An instance of <tt>os_task_t const*</tt>, which points to the
 *						task the thread is supposed to execute for the rest of
 *						its lifetime.
 *
 * \return Always returns 0.
 */
#if (OS_ARCH == OS_WINDOWS)
os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINTaskThread(void * param)
#elif (OS_ARCH == OS_LINUX)
void* OS_WINLINTaskThread(void * param)
#endif
{
	os_task_t const* const tp = (const os_task_t *)param;

	/* Possible diagnostic TOOLDIAG-1 <START> */
	OS_WINLINInitializeTaskThread(tp);
	/* Possible diagnostic TOOLDIAG-1 <STOP> */

	OS_WINLINAssert(!OS_WINLIN_OSAL_IsEventSet(OS_WINLINGetSyscallPortOfTask(tp)->syscallFinishedEvent));

	while (1)
	{
		/* Prepare for the longjmp, which is done when a task terminates normally
		 * and start the task.
		 */
		if (OS_WINLINTaskSetjmpById(tp->taskId) == 0)
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Going to run task %s.",
					OS_WINLINGetNameOfTask(tp));

			tp->entry();	/* Run the task. */

			OS_WINLINLog(
					OS_WINLIN_LOG_WARNING,
					"Did you forget to terminate task %s explicitly by calling "
					"TerminateTask() or ChainTask()? Going to call TerminateTask().",
					OS_WINLINGetNameOfTask(tp));

			OS_UserTaskReturn();
		}
		else
		{
			/* The task pointed to by tp has terminated normally, i.e. by calling
			 * ChainTask() or TerminateTask() AND has been selected again to run another
			 * time. This means, when the current thread reaches this point it's supposed
			 * to run the task tp once again.
			 * Note, that when a task terminates normally its thread becomes stuck in the
			 * system call, which caused the task to become terminated. This system call
			 * finishes not earlier than when the task is restarted.
			 * See OS_WINLINHandleTaskTermination().
			 */
		}
	}

#if (OS_ARCH == OS_WINDOWS)
	return 0;		/* Return value is never used. */
#elif (OS_ARCH == OS_LINUX)
	return OS_NULL;	/* Return value is never used. */
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
