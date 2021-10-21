/* kern-dispatch.c
 *
 * This file contains the OS_Dispatch function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dispatch.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain
 * persistent side effects.
 *
 * Reason:
 * The "side effect" in question is a call to an assembler function that returns the
 * stack pointer for stack checks. As a result, there's no extra change in the execution
 * environment of the caller.
 *
 *
 * MISRAC2012-2) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and any
 * other type.
 *
 * Reason:
 * Tasks are started by low-level code, which doesn't use C language type
 * information. Therefore, this information is stripped by a type cast.
 *
 *
 * MISRAC2012-3) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason:
 * This is a false positive, becuase the criticized functions do relephant work.
 *
 *
 * MISRAC2012-4) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
*/

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_C_COMPL_010:calls]
 *  The code shall adhere to the [HISSRCMETRIC] Metrics.
 *
 * Reason:
 *  The function OS_Dispatch uses if-elseif-else-cascades to select the
 *  the proper actions. Breaking it up in multiple smaller functions is not a
 *  valid option, because this function dispatches the tasks which needs
 *  to be as quick as possible.
 *  This leads to a high CALLS value.
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if stack checks are enabled.
*/

#define OS_SID	OS_SID_Dispatch
#define OS_SIF	OS_svc_Dispatch

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_Dispatch completes saving the state of the outgoing
 * task, then loads the incoming task if there is one.
 *
 * The outgoing task's registers have already been saved into its
 * task context area, but we still need to modify the task's
 * state.
 * After that, the incoming task (OS_GetKernelData()->taskQueueHead) is started.
 * If there is no incoming task, the routine waits, calling the idle
 * function, until something comes along.
 *
 * The idle function OS_Idle() must be very carefully implemented.
 * The simplest case can enable interrupts and then disable them again,
 * but there must be sufficient time in between to allow an interrupt to
 * percolate through the CPU. There must be no HALT instruction between
 * the enable and disable, otherwise an interrupt that is taken immediately
 * might make a task ready that would then remain ready while the CPU
 * is halted.
 *
 * Another approach is to clear the inKernel flag, enable interrupts and
 * then sit in a nontermination loop. When a task becomes active the
 * it will be dispatched from the interrupt handler. In this case the loop
 * is permitted to contain a HALT instruction.
 *
 * Conditions on entry:
 *   interrupts disabled
 *   inKernel == 1
 *   taskCurrent == OS_NULL OR  taskCurrent's context saved
 *   taskQueueHead == OS_NULL  OR  taskCurrent != taskQueueHead
 *   parameter newstate: OS_TS_READY_ASYNC if called from ISR, OS_TS_READY_SYNC if called from SYSCALL.
 *
 * Note: if a task makes a system call which is interrupted, the task will
 * still get the state READY_SYNC, even though it didn't really give up
 * the CPU voluntarily. The SYNC/ASYNC is only an internal state that
 * informs the dispatcher how much context to restore.
 *
 * Interrupts remain disabled throughout, unless there is no task to run.
 * In that case, interrupts are enabled in a loop until there is a task
 * to run.
*/
/* Deviation DCG-1 <START> */
void OS_Dispatch(os_taskstate_t newstate)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskstate_t qstate;
	os_taskdynamic_t *td;

	/* outgoing == OS_NULL ==>
	 *    - interrupted idle loop, or
	 *    - startup.
	*/
	if ( kernel_data->taskCurrent != OS_NULL )
	{
		td = kernel_data->taskCurrent->dynamic;

		/* Ensure that the task has not overwritten its stack
		 *
		 * !LINKSTO Kernel.Autosar.StackMonitoring.Detection, 2
		 * !LINKSTO Kernel.Feature.StackCheck.Automatic, 1
		*/
		/* Deviation MISRAC2012-1, MISRAC2012-4 <1> */
		if ( OS_TaskStackOverflow(kernel_data->taskCurrent, td) )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_TaskStackOverflow, OS_NULL);	/* Should never return */
		}

		/* If task is being pre-empted, set it to one of the "ready" states.
		*/
		if ( td->state == OS_TS_RUNNING )
		{
			/* Call the post-task hook for the outgoing task. For tasks that
			 * have gone to the WAITING or SUSPENDED state, the PostTaskHook
			 * has already been called by the corresponding kernel function.
			 * Interrupts are disabled here, so no need to do it again.
			 *
			 * !LINKSTO Kernel.API.Hooks.PostTaskHook.API, 1
			 * !LINKSTO Kernel.API.TaskManagement.GetTaskID.Hook, 1
			 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
			 *		Interrupts are disabled here.
			*/
			OS_CALLPOSTTASKHOOK();

			OS_TRACE_STATE_TASK( kernel_data->taskCurrent->taskId, td->state, newstate );
			td->state = newstate;

			OS_ARCH_PREEMPTTASKEXECTIMING(kernel_data->taskCurrent);
			OS_PREEMPTTASKEXECTIMING(kernel_data->taskCurrent);
		}

	}

	/* incoming == OS_NULL ==> no task ready to run. Wait until a task
	 * becomes ready.
	*/
	if ( kernel_data->taskQueueHead == OS_NULL )
	{
		kernel_data->taskCurrent = OS_NULL;
		OS_MEASURECPULOAD();
		OS_Idle();				/* Must NEVER return */
	}
	else
	{
		/* Head of queue becomes the running task. Remember the old
		 * state before switching to running, because we need to differentiate
		 * the 3 possible cases below.
		*/
		kernel_data->taskCurrent = kernel_data->taskQueueHead;
		td = kernel_data->taskCurrent->dynamic;
		qstate = td->state;

		OS_TRACE_STATE_TASK( kernel_data->taskCurrent->taskId, td->state, OS_TS_RUNNING);
		td->state = OS_TS_RUNNING;

		/* Set the task's running priority - only if not pre-empted, and
		 * only if the running priority is higher than the queued priority.
		 * This boils down to testing the 2nd condition only - preempted
		 * tasks will have a priority not less than their running priority
		 * anyway.
		 *
		 * Non-premptive tasks get a run priority equal to the highest-
		 * priority task. Tasks with internal resources get the priority
		 * of the highest of those resources. Others get their base priority.
		 * This is all programmed into the runPrio field by the generator.
		 *
		 * !LINKSTO Kernel.ResourceManagement.TakeInternalResource, 1
		 * !LINKSTO Kernel.Architecture.ConformanceClasses.InternalResources, 1
		*/
		if ( td->prio < kernel_data->taskCurrent->runPrio )
		{
			td->prio = kernel_data->taskCurrent->runPrio;
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
			OS_InsertTask(kernel_data->taskCurrent, kernel_data->taskCurrent->runPrio);
#endif
		}

		/* Call the pre-task hook for the incoming task
		 * Interrupts are disabled here, so no need to do it again.
		 *
		 * !LINKSTO Kernel.API.Hooks.PreTaskHook.API, 1
		 * !LINKSTO Kernel.API.Hooks.PreTaskHook.TaskChange, 1
		 * !LINKSTO Kernel.API.TaskManagement.GetTaskID.Hook, 1
		*/
		OS_CALLPRETASKHOOK();

		/* Now leaving the kernel. Interrupts remain disabled until the task
		 * runs.
		*/
		OS_GetKernelData()->inKernel = 0;

		/* Set up the memory and timing protection systems for the task.
		 *
		 * !LINKSTO Kernel.Autosar.Protection.HardwareProtection, 1
		 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime, 2
		 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Nontrusted, 2
		 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Trusted, 2
		*/
		kernel_data->inFunction = OS_INTASK;
		OS_SETPROTECTION(kernel_data->taskCurrent);
		OS_STARTTASKEXECTIMING(kernel_data->taskCurrent);
		OS_ARCH_STARTTASKEXECTIMING(kernel_data->taskCurrent, qstate);

		/* Optimisation: this is the original code. An alternative was tried but did not gain
		 * much. See svn history for details
		*/
		if ( qstate == OS_TS_NEW )
		{
			/* !LINKSTO Kernel.API.TaskManagement.ActivateTask.StartAddress, 1
			 *
			 * MISRA/ISO warnings/errors etc.:
			 * Depending on the architecture, there may be one or more conversions of "function pointers"
			 * to "object pointers" here. What we're actually doing is manipulating machine addresses for
			 * use by assembly code - we need an entry point and a return address for the task.
			 * It can be safely assumed that the code is correct at the machine level. If it were not,
			 * the entire OS would fail to function. There are no portability problems because the
			 * code in question is specifically tailored for the architecture on which it runs. Possibly
			 * also for the compiler and memory model too.
			*/
			/* Deviation MISRAC2012-2 <1> */
			OS_StartTask(kernel_data->taskCurrent);
			/* Never returns!
			*/
		}
		else
		if ( qstate == OS_TS_READY_ASYNC )
		{
			/* Deviation MISRAC2012-3 <1> */
			OS_ResumeTaskAsync(kernel_data->taskCurrent);
			/* Never returns!
			*/
		}
		else
		if ( qstate == OS_TS_READY_SYNC )
		{
			/* Deviation MISRAC2012-3 <1> */
			OS_ResumeTaskSync(kernel_data->taskCurrent);
			/* Never returns!
			*/
		}
		else
		{
			/* MISRA */
		}

		/* The code below is a final catch-all error handler in case
		 * one of the branches *does* return or if qstate has an incorrect value.
		 *
		 * !LINKSTO Kernel.TaskManagement, 1
		 * !LINKSTO Kernel.TaskManagement.Scheduling, 1
		 * !LINKSTO Kernel.TaskManagement.Scheduling.TaskState, 1
		 *
		 * Having code here also ensures that the compiler cannot optimise any
		 * calls in the various dispatcher macros to jumps. We might need those
		 * calls (TRICORE)!
		 *
		 * This code should not normally be reachable without some trickery, like deliberately corrupting the
		 * state of the tasks on the queue.
		 *
		 * CHECK: PARSE
		*/
		/* can't propagate the return value of OS_PANIC -> ignore it */
		(void) OS_PANIC(OS_PANIC_InvalidTaskState);	/* Should never return */

		/* In any case, this function MUST never return, so we call
		 * ShutdownNoHooks().
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		*/
		OS_ShutdownNoHooks();
	}
}
/* Deviation DCG-1 <END> */

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
