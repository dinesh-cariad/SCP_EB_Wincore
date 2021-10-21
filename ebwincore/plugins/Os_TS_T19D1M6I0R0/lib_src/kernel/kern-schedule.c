/* kern-schedule.c
 *
 * This file contains the OS_KernSchedule function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-schedule.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
*/

#define OS_SID	OS_SID_Schedule
#define OS_SIF	OS_svc_Schedule

#include <Os_kernel.h>
#include <Os_taskqueue.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernSchedule implements the API Schedule
 *
 * If the current task is non-preemptable it is requeued with
 * its true priority. However, since the task is already running,
 * it is enqueued AHEAD of those with equal priority, because they
 * might want to use the same context.
 *
 * Interrupts are enabled on entry. We must disable them before doing
 * anything and restore them afterwards.
 *
 * !LINKSTO Kernel.API.TaskManagement.Schedule.API, 1
*/
os_result_t OS_KernSchedule(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_intstatus_t is;
	os_result_t r = OS_E_OK;
	os_taskdynamic_t *td;

	OS_TRACE_SCHEDULE_ENTRY();

	if ( kernel_data->inFunction == OS_INTASK )
	{
		td = kernel_data->taskCurrent->dynamic;

		if ( !OS_InterruptEnableCheckFromTask(OS_IEC_OSEKEXTRA, td) )
		{
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
		else
		if ( td->lastLock != OS_NULLOBJECT )
		{
			/* !LINKSTO Kernel.API.TaskManagement.Schedule.Resources, 1
			 *
			 * If this requirement is ever relaxed, the requeue priority
			 * (below) will need to be adjusted so that it is not lower
			 * than that of the highest-priority resource that has been
			 * taken.
			*/
			if (OS_GetTypeFromObjectId(td->lastLock) == OS_OBJ_SPINLOCK)
			{
				/* !LINKSTO Kernel.Autosar.Multicore.Schedule.Spinlock, 1
				*/
				r = OS_ERROR(OS_ERROR_HoldsLock, OS_NULL);
			}
			else
			{
				r = OS_ERROR(OS_ERROR_HoldsResource, OS_NULL);
			}
		}
		else
		{
#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
			os_tasklink_t link;
			os_tasklink_t next;
			os_tasklink_t * const taskActivations = OS_GetTaskActivations();
#endif

			is = OS_IntDisable();

			/* Requeue the task with its base priority if there's something
			 * with a higher priority waiting. We only do this if the task
			 * is in the running state --- it could have been killed at ISR
			 * level just before disabling interrupts.
			 *
			 * !LINKSTO Kernel.API.TaskManagement.Schedule.HigherPrioTask, 1
			 * !LINKSTO Kernel.API.TaskManagement.Schedule.LowerPrioTask, 1
			 * !LINKSTO Kernel.API.TaskManagement.Schedule.Schedule, 1
			 * !LINKSTO Kernel.API.TaskManagement.Schedule.InternalResource, 1
			*/
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
			if ( (td->state == OS_TS_RUNNING) &&
				 (kernel_data->taskCurrent->queuePrio != kernel_data->taskCurrent->runPrio) )
			{
				td->prio =  kernel_data->taskCurrent->queuePrio;
				OS_DequeueTask(kernel_data->taskCurrent, kernel_data->taskCurrent->runPrio);
			}
#else
			link = OS_CurrentLink(kernel_data->taskCurrent, td);
			next = taskActivations[link];

			if ( (td->state == OS_TS_RUNNING) &&
				 (next != 0) &&
				 (kernel_data->taskCurrent->queuePrio < OS_FindPrio(next)) )
			{
				td->prio =  kernel_data->taskCurrent->queuePrio;
				OS_RequeueDown(link, td->prio);
			}
#endif

			OS_IntRestore(is);
		}
	}
	else
	{
		/* !LINKSTO Kernel.API.TaskManagement.Schedule.ISRC2, 1
		*/
		r = OS_ERROR(OS_ERROR_WrongContext, OS_NULL);
	}

	OS_TRACE_SCHEDULE_EXIT_P(r);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserSchedule</name>
  <synopsis>Voluntarily yield the CPU</synopsis>
  <syntax>
    os_result_t OS_UserSchedule(void)
  </syntax>
  <description>
    <code>OS_UserSchedule()</code> allows the calling task to yield the
    CPU voluntarily. Active tasks whose running priorities are lower than the
    running priority of the current task but higher that its base priority
    are allowed to run. <code>OS_UserSchedule()</code> returns when there
    are no more such tasks.
    <para>Tasks get a higher running priority than their base priority when they
    are preemptive or have an internal resource allocated to them.</para>
    <para>A task that holds a standard resource is not permitted to call
    <code>OS_UserSchedule()</code> since this would interfere with
    the resource's ceiling priority.</para>
  </description>
  <availability>
    <code>OS_UserSchedule()</code> may only be called from a task.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
