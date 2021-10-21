/* kern-terminatetask.c
 *
 * This file contains the OS_KernTerminateTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-terminatetask.c 27928 2017-11-21 18:58:04Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   These error checks are reachable, if status is EXTENDED.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements are reached if interrupt enable checks are enables.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements are reached if context checks are enables.
*/

#define OS_SID	OS_SID_TerminateTask
#define OS_SIF	OS_svc_TerminateTask

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernTerminateTask implements the API TerminateTask
 *
 * The current task is terminated. and removed from the task queue.
 * If there are any activations pending, the task is enqueued again at
 * its base priority with state NEW. Otherwise the state is set to
 * SUSPENDED.
 *
 * Interrupts are enabled on entry. taskCurrent cannot be changed
 * by an ISR because it only gets updated by the dispatcher after
 * returning from here. However, we need to disable interrupts before
 * manipulating the queue. We can enable them again at on exit.
 *
 * Because interrupts are enabled, taskCurrent is not guaranteed to
 * be at the head of the queue, so we must use Dequeue(), not DequeueHead().
 *
 * !LINKSTO Kernel.API.TaskManagement.TerminateTask.API, 1
 *
 * !LINKSTO Kernel.API.TaskManagement.TerminateTask.Schedule, 1
 *      Happens automatically when this function returns.
 *
 * !LINKSTO Kernel.API.TaskManagement.TerminateTask.InternalResource, 1
 * !LINKSTO Kernel.ResourceManagement.ReleaseInternalResource, 1
 *      Happens automatically because internal resources are
 *      implemented by setting the run priority of the task.
*/
os_result_t OS_KernTerminateTask(void)
{
	const os_task_t *tp;
	os_taskdynamic_t *td;
	os_result_t r = OS_E_OK;

	OS_TRACE_TERMINATETASK_ENTRY();

	if ( OS_IsCalledFromTask() )
	{
		/* !LINKSTO Kernel.API.TaskManagement.TerminateTask.Task, 1
		*/
		tp = OS_GetKernelData()->taskCurrent;
		td = tp->dynamic;

		if ( !OS_InterruptEnableCheckFromTask(OS_IEC_OSEKEXTRA, td) )
		{
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
		else
		if ( OS_TaskOccupiesResource(td) )
		{
			/* Note: This block can only be entered if status is EXTENDED. */
			/* Possible diagnostic TOOLDIAG-1 <START> */
			/* !LINKSTO Kernel.API.TaskManagement.TerminateTask.ExternalResource, 1
			*/
			if (OS_GetTypeFromObjectId(td->lastLock) == OS_OBJ_SPINLOCK)
			{
				/* !LINKSTO Kernel.Autosar.Multicore.TerminateTask.Spinlock, 1
				*/
				r = OS_ERROR(OS_ERROR_HoldsLock, OS_NULL);
			}
			else
			{
				r = OS_ERROR(OS_ERROR_HoldsResource, OS_NULL);
			}
			/* Possible diagnostic TOOLDIAG-1 <STOP> */
		}
		else
		{
			OS_DoTerminateTask(tp, td);
		}
	}
	else
	{
		/* !LINKSTO Kernel.API.TaskManagement.TerminateTask.ISRC2, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_NULL);
	}

	OS_TRACE_TERMINATETASK_EXIT_P(r);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserTerminateTask</name>
  <synopsis>Terminate the current task</synopsis>
  <syntax>
    os_result_t OS_UserTerminateTask(void)
  </syntax>
  <description>
    <code>OS_UserTerminateTask()</code> terminates the current task. The
    calling task is transferred from the <emphasis>running</emphasis> state to the
    <emphasis>suspended</emphasis> state. The calling task must have released all
    resources and resumed all suspended interrupts before calling
    <code>OS_UserTerminateTask()</code>.
    <para>
    The function does not normally return unless an error is detected.
    </para>
  </description>
  <availability>
    <code>OS_UserTerminateTask()</code> may only be called from a task.
  </availability>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
