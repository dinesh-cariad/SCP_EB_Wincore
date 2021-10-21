/* kern-missingterminatetask.c
 *
 * This file contains the OS_MissingTerminateTask function, which is used as a
 * default task return address for user-mode kernels or for CPUs that cannot
 * force an exception for incorrect return from a task.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-missingterminatetask.c 23966 2016-04-12 12:57:53Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: InfiniteLoop
 *   Possible infinite loop.
 *
 * Reason: Infinite loop is wanted here.
 */

#include <Os_api.h> /* for OS_UserShutdownOs */
#include <Os_tool.h> /* for OS_ENDLESSLOOP */
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* Standard value for the macro, if not defined.
 * In the standard case we report the error. If that ever returns, we
 * try to shut down, and if that doesn't work either, we sit in a loop.
 * When error handling for returns from tasks is disabled, Os_optimize.h
 * will omit this handling and only use the endless loop.
*/
#ifndef OS_MttHandleError
#define OS_MttHandleError()	\
	do \
	{ \
		OS_UserTaskReturn(); \
		OS_UserShutdownOs(OS_E_TASKRETURN); \
	} while (0)
#endif

/*!
 * OS_MissingTerminateTask
 *
 * This function is used as the return address for tasks. Its purpose is to
 * trap a return from the task's main function and perform the necessary
 * actions required by the OS specification.
 *
 * It is always implemented as a true function because we need its address.
 *
 * Here's what it does:
 *
 * 1. Call error handler via OS_UserTaskReturn(). This is either a macro or a
 *    system call wrapper which will invoke OS_KernTaskReturn().
 *
 * Usually, the error handler in OS_KernTaskReturn will kill the task and
 * leaving the kernel will switch to another task or idle, so we're done.
 *
 * 2. Call ShutdownOS() just to be sure - if Os_UserTaskReturn() returned,
 *    something is wrong
 *
 * 3. Loop endlessly.
 *
 * As an optimization, the user can choose to leave out the error handling
 * parts (steps 1 and 2) and only do the endless loop (step 3).
*/
void OS_MissingTerminateTask(void)
{
	OS_MttHandleError();

	/* wait forever */
	OS_ENDLESSLOOP();
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
