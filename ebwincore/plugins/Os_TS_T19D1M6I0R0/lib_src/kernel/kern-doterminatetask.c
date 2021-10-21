/* kern-doterminatetask.c
 *
 * This file contains the OS_DoTerminateTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-doterminatetask.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if there is at least one task which allows multiple activations.
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_DoTerminateTask implements the business end of the API TerminateTask
 *
 * The task indicated by tp and td is terminated. and removed from the task queue.
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
 *      Happens automatically because internal resources are
 *      implemented by setting the run priority of the task.
*/
void OS_DoTerminateTask(const os_task_t *tp, os_taskdynamic_t *td)
{
	os_intstatus_t is;

	is = OS_IntDisable();

	if ( td->state == OS_TS_RUNNING )
	{
		/* Stop the execution timer.
		 * This could kill the task!
		*/
		OS_ARCH_STOPTASKEXECTIMING(tp);
		OS_STOPTASKEXECTIMING(tp);
	}

	if ( td->state == OS_TS_RUNNING )
	{

		/* Call post-task hook before task goes to SUSPENDED state.
		 * Interrupts are already disabled here.
		 *
		 * !LINKSTO Kernel.API.Hooks.PostTaskHook.API, 1
		 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
		 *		Interrupts are disabled here
		*/
		OS_CALLPOSTTASKHOOK();

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
		/* If the run priority of the task is different from the base priority
		 * we need to dequeue the task from the runPrio queue too.
		*/
		if ( tp->runPrio != tp->queuePrio)
		{
			OS_DequeueTask(tp, tp->runPrio);
		}
		OS_DequeueTask(tp, tp->queuePrio);
#else
		OS_Dequeue(OS_CurrentLink(tp, td));
#endif

		if ( OS_ActivationPending(td) )
		{
			/* !LINKSTO Kernel.Architecture.ConformanceClasses.MultipleActivation, 1
			*/
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			td->nAct--;
			td->prio = tp->queuePrio;

			OS_TRACE_STATE_TASK( tp->taskId, td->state, OS_TS_NEW );
			td->state = OS_TS_NEW;

			/* Reset the task's execution time measurement
			 *
			 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
			*/
			OS_InitTaskEtb(tp);

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
			/* Advance the current activation index.
			*/
			if ( td->cAct >= tp->maxAct )
			{
				td->cAct = 0;
			}
			else
			{
				td->cAct++;
			}
#endif
		}
		else
		{
			/* !LINKSTO Kernel.API.TaskManagement.TerminateTask.StateChange, 1
			*/
			OS_TRACE_STATE_TASK( tp->taskId, td->state, OS_TS_SUSPENDED );
			td->state = OS_TS_SUSPENDED;
		}
	}

	OS_IntRestore(is);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
