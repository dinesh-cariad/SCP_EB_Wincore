/* kern-localactivatetask.c
 *
 * This file contains the OS_LocalActivateTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-localactivatetask.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right-hand operand of a logical && or || operator shall not contain
 * persistent side effects.
 *
 * Reason:
 * The rate monitoring function has to make modifications to the global
 * system state.
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if there are tasks which allow multiple activations.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if rate monitoring is used.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if status is EXTENDED.
*/

/* The SID and serviceinfo aren't actually used in the code, but the error table builder
 * needs them to check for error codes that are defined but not used, or used but not defined.
*/
#define OS_SID OS_SID_ActivateTask
#define OS_SIF OS_svc_ActivateTask

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_LocalActivateTask implements the dynamic part of API ActivateTask
 *
 * The specified task is activated. If it is already active, the
 * pending activation counter is incremented (up to the maximum).
 * Otherwise the task state is set to NEW and it is enqueued.
 *
 * Interrupts could be enabled on entry, so this function disables them
 * before loading the task state, because the state could get changed
 * during an ISR. The saved interrupt state is restored before
 * returning.
 *
 * !LINKSTO Kernel.API.TaskManagement.ActivateTask, 1
*/
os_errorresult_t OS_LocalActivateTask(const os_task_t *tp)
{
	os_errorresult_t result = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
	os_taskdynamic_t *td;
	os_taskstate_t ts;
	os_intstatus_t is;
#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
	os_tasklink_t link;
#endif

	td = tp->dynamic;
	is = OS_IntDisable();
	ts = td->state;

	if ( !OS_APPISACCESSIBLE(tp->app) )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
	}
	else
	if ( ts == OS_TS_SUSPENDED )
	{
		/* !LINKSTO Kernel.API.TaskManagement.ActivateTask.NPTask, 1
		 * !LINKSTO Kernel.API.TaskManagement.ActivateTask.PTask, 1
		 * !LINKSTO Kernel.API.TaskManagement.ActivateTask.ExtendedTaskClearEvents, 1
		*/
		/* Deviation MISRAC2012-1 */
		if ( OS_RATEMONITORCHECK(tp->rateMonitor) )
		{
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			result = OS_ERRORCODE_CHECK(OS_ERROR_RateLimitExceeded);
		}
		else
		{
			td->prio = tp->queuePrio;

			OS_TRACE_STATE_TASK( tp->taskId, td->state, OS_TS_NEW );
			td->state = OS_TS_NEW;

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
			OS_ClearCurrentActivation(td);
#endif
			OS_ClearPendingEvents(td);

			/* Reset the task's execution time measurement
			 *
			 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
			*/
			OS_InitTaskEtb(tp);

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
			OS_EnqueueTask(tp, tp->queuePrio);
#else
			OS_Enqueue(tp);
#endif
		}
	}
	else
	if ( ts == OS_TS_QUARANTINED )
	{
		result = OS_ERRORCODE_CHECK(OS_ERROR_Quarantined);
	}
	else
	if ( OS_IsValidTaskState(ts) )
	{
		if ( OS_ActivationLimitExceeded(tp, td) )
		{
			/* !LINKSTO Kernel.API.TaskManagement.ActivateTask.TooManyActivations, 1
			 * !LINKSTO Kernel.API.TaskManagement.ChainTask.TooManyActivations, 1
			 * !LINKSTO Kernel.TaskManagement.MultipleActivations, 1
			 * !LINKSTO Kernel.TaskManagement, 1
			 */
			result = OS_ERRORCODE_CHECK(OS_ERROR_MaxActivations);
		}
		else
		{
			/* Possible diagnostic TOOLDIAG-1 <+2> */
			/* Deviation MISRAC2012-1 */
			if ( OS_RATEMONITORCHECK(tp->rateMonitor) )
			{
				/* Possible diagnostic TOOLDIAG-2 <+1> */
				result = OS_ERRORCODE_CHECK(OS_ERROR_RateLimitExceeded);
			}
			else
			{
				/* Multiple activations: increment the counter and
				 * enqueue the activation. The task states don't change.
				 *
				 * !LINKSTO Kernel.Architecture.ConformanceClasses.MultipleActivation, 1
				 * !LINKSTO Kernel.TaskManagement.MultipleActivations, 1
				 * !LINKSTO Kernel.TaskManagement, 1
				*/
				td->nAct++;
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
				OS_EnqueueTask(tp, tp->queuePrio);
#else
				link = (os_tasklink_t)td->cAct + (os_tasklink_t)td->nAct;
				if ( link > (os_tasklink_t)tp->maxAct )
				{
					link -= ((os_tasklink_t)tp->maxAct + 1);
				}
				link += tp->linkBase;
				OS_EnqueueLink(link, tp->queuePrio);
#endif
			}
		}
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		result = OS_PANIC(OS_PANIC_InvalidTaskState);
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
