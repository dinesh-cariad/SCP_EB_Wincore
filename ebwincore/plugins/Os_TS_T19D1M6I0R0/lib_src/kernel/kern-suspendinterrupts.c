/* kern-suspendinterrupts.c
 *
 * This file contains the OS_KernSuspendInterrupts() function.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-suspendinterrupts.c 28669 2018-05-14 07:18:01Z beoe851 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable is not used on all platforms.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if "error handling for APIs returning void" is not excluded.
 *   See also OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if there are Cat-2 ISRs.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 *
 *
 * MISRAC2012-2) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 */

#define OS_SID OS_SID_SuspendInterrupts
#define OS_SIF OS_svc_SuspendInterrupts

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernSuspendInterrupts()
 *
 * This function disables interrupts, implementing the SuspendOSInterrupts(),
 * SuspendAllInterrupts() and DisableAllInterrupts() system services.
 * The type of lock is determined by the locktype parameter.
 *
 * The OSEK/VDX requirement that the corresponding Resume/Enable
 * system service shall restore the state saved here, and that
 * the Suspend/Resume services shall be nestable, introduces some complexity.
 *
 * The normal approach, taken in ProOSEK 4.0, is to maintain a nesting
 * count and an old level. The old level is only saved when the nesting
 * count is zero. The nesting count is incremented.
 *
 * However, the system call mechanism of OS, plus the reentrant
 * nature of the kernel, mean that this approach won't work. This
 * is because an interrupt can be taken between the system call
 * and entering this function, and if this interrupt activates a
 * higher priority task than the caller, the task will not be dispatched
 * until after this function returns. The implication of this is that
 * the new task will start with the nesting count set to 1.
 *
 * To avoid this problem, we maintain a nesting count and old level
 * variable for each task. When the system service is called from a
 * task context, the fields of the current task context are used.
 * Because we are simply manipulating the task's local storage,
 * there is no need to protect this code from interrupts.
 *
 * When the system service is called from any context other than a
 * task context, a global nesting count/old level pair is used. This
 * works because the ISRs and other contexts that may call this service
 * behave in a strictly nested manner. However, special care must be taken
 * to prevent unexpected nesting.
 *
 * A change to the standard, making the caller responsible for storing
 * the old value, would eliminate all this kernel complexity.
 *
 * If this function is called nested within a SuspendAllInterrupts()/
 * ResumeAllInterrupts() pair, the suspendAll counter is incremented
 * instead of the suspendOs counter. This prevents false nesting from
 * incorrectly enabling or disabling interrupts later.
 *
 * Look! No Cross-core calls!
 * !LINKSTO Kernel.Autosar.Multicore.InterruptAPI.CoreLocal, 1
*/
void OS_KernSuspendInterrupts(os_intlocktype_t locktype)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskdynamic_t *td = OS_NULL;
	os_oldlevel_t *olp = OS_NULL;		/* Pointer to Old level store */
	os_uint8_t *ncp = OS_NULL;			/* Pointer to nesting counter */
	os_intstatus_t is;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_intstatus_t level = 0;
	os_prio_t *pPrioBeforeSuspend = OS_NULL;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)locktype);

	OS_TRACE_SUSPENDINTERRUPTS_ENTRY(locktype);

	if ( kernel_data->inFunction == OS_INTASK )
	{
		/* Called from a task context:
		 * Here we manipulate the task's own interrupt management
		 * variables.
		 *
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendOSInterrupts.DisableInterrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendOSInterrupts.Nest, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Interrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Nest, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Task, 1
		 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.Interrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.Task, 1
		*/
		td = kernel_data->taskCurrent->dynamic;

		if ( locktype == OS_LOCKTYPE_OS )
		{
			/* Operation is a no-op if nested inside SuspendAllInterrupts() or DisableAllInterrupts()
			*/
			if ( td->nestSuspendAll == 0 )
			{
				level = OS_intDisableLevel;
				olp = &td->oldSuspendOs;
				ncp = &td->nestSuspendOs;
				pPrioBeforeSuspend = &td->prioBeforeSuspendOs;
			}
		}
		else
		{
			level = OS_intDisableLevelAll;
			olp = &td->oldSuspendAll;
			ncp = &td->nestSuspendAll;
		}
	}
	else
	if ( OS_IS_INCAT2( kernel_data->inFunction )  )
	{
		/* Called from Category 2 ISR:
		 * Here we manipulate the global old level/nesting counter
		 * variables.
		 *
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendOSInterrupts.DisableInterrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendOSInterrupts.Nest, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Interrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Nest, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.ISRC1, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.ISRC2, 1
		 * !LINKSTO Kernel.API.InterruptHandling.SuspendAllInterrupts.Hook, 1
		 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.Interrupts, 1
		 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.ISRC1, 1
		 * !LINKSTO Kernel.API.InterruptHandling.DisableAllInterrupts.ISRC2, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		if ( locktype == OS_LOCKTYPE_OS )
		{
			/* Operation is a no-op if nested inside SuspendAllInterrupts() or DisableAllInterrupts()
			*/
			if ( kernel_data->nestSuspendAll == 0 )
			{
				level = OS_intDisableLevel;
				olp = &kernel_data->oldSuspendOs;
				ncp = &kernel_data->nestSuspendOs;
			}
		}
		else
		{
			level = OS_intDisableLevelAll;
			olp = &kernel_data->oldSuspendAll;
			ncp = &kernel_data->nestSuspendAll;
		}
	}
	else
	if ( OS_CallingContextCheck() )
	{
		/* In all other permitted contexts (Cat1 interrupt, hooks), category 2 interrupts
		 * are automatically blocked.
		 * SuspendOSInterrupts is a no-op in contexts other than Task and Cat2 ISR
		 *
		 * !LINKSTO Kernel.Autosar.OSEK.Differences.DisableInterruptsBeforeStart, 2
		*/
		if ( locktype != OS_LOCKTYPE_OS )
		{
			level = OS_intDisableLevelAll;
			olp = &kernel_data->oldSuspendAll;
			ncp = &kernel_data->nestSuspendAll;
		}
	}
	else
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* Called from any other context ---> error
			*/
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
	}

	if ( ncp != OS_NULL )
	{
		/* This branch actually does the job ...
		 *
		 * Interrupts disabled here to prevent a possible execution-time-
		 * overrun interrupt from messing things up, especially if the
		 * caller is a task.
		*/
		is = OS_IntDisableMax();

		if ( (td != OS_NULL) && (td->state != OS_TS_RUNNING) )
		{
			/* Task has been killed, e.g. by overrunning execution budget.
			 * In this case we just return without doing anything
			*/
			OS_IntRestoreMax(is);
		}
		else
		if ( *ncp == 0 )
		{
			/* OS_SuspendInterrupts is a macro that doesn't
			 * attempt to ensure that its parameters are only evaluated
			 * once. That's OK here because the parameters are simple
			 * variables, not expressions
			 *
			 * OS_SuspendInterrupts() disables OS interrupts in the
			 * current context AND in the calling context.
			 *
			 * We don't need to worry about a Cat1 ISR interrupting us,
			 * because SuspendOsInterrupts in Cat1 is a no-op.
			 *
			 * The saved interrupt state is not restored in this branch
			 * because OS_SuspendInterrupts() sets the required
			 * level.
			*/
			*ncp = 1;

			if ( kernel_data->inFunction == OS_INTASK )
			{
				OS_STARTTASKINTLOCKTIMING(kernel_data->taskCurrent, locktype);
				if (pPrioBeforeSuspend != OS_NULL)
				{
					os_coreid_t const myCoreId = OS_GetMyCoreId();
					os_taskprio_t const maxPrio = OS_maxPrio[myCoreId];

					*pPrioBeforeSuspend = td->prio;
					/* The configuration has to make sure, that any task can be raised to the
					 * maximum priority, see Generator.Processing.ImplicitResScheduler.
					*/
					OS_RaiseTaskPriority(td, maxPrio);
				}
			}
			else
			if ( OS_IS_INCAT2( kernel_data->inFunction ) )
			{
				/* Possible diagnostic TOOLDIAG-3 <+1> */
				OS_STARTISRINTLOCKTIMING(kernel_data->isrCurrent, locktype);
			}
			else
			{
				/* MISRA */
			}

			/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
			OS_SuspendInterruptsX(olp, level, is);
			OS_IntRestoreHardLock(is);
		}
		else
		if ( (locktype != OS_LOCKTYPE_NONEST) && (*ncp < OS_SUSPENDNESTLIMIT) )
		{
			(*ncp)++;
			OS_IntRestoreMax(is);
		}
		else
		{
			/*
			 * !LINKSTO Kernel.Feature.RuntimeChecks, 1
			 * !LINKSTO Kernel.Feature.RuntimeChecks.CheckSuspendResumeNesting, 1
			*/
			OS_IntRestoreMax(is);

			if ( OS_ErrorHandlingForVoidApi() )
			{
				/* can't propagate the return value of OS_ERROR -> ignore it */
				/* Possible diagnostic TOOLDIAG-2 <+1> */
				(void) OS_ERROR(OS_ERROR_NestingOverflow, OS_GET_PARAMETER_VAR());
			}
		}
	}
	OS_TRACE_SUSPENDINTERRUPTS_EXIT_P(locktype);
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserSuspendInterrupts</name>
  <synopsis>Suspend interrupts up to a given level</synopsis>
  <syntax>
    void OS_UserSuspendInterrupts(os_intlocktype_t locktype)
  </syntax>
  <description>
	<para>
    <code>OS_UserSuspendInterrupts()</code> raises the interrupt level of the processor
    or interrupt controller to a level that depends on the <code>locktype</code> parameter.
    It is used to implement the <code>SuspendOSInterrupts()</code>, <code>SuspendAllInterrupts()</code>
    and <code>DisableAllInterrupts()</code> system services by calling it with the <code>locktype</code>
    parameter equal to <code>OS_LOCKTYPE_OS</code>, <code>OS_LOCKTYPE_ALL</code> and
    <code>OS_LOCKTYPE_NONEST</code>, respectively.
    </para>
    <para>
    Both <code>SuspendOSInterrupts()</code> and <code>SuspendAllInterrupts()</code> are nestable;
    this is implemented by a counter. The interrupt level is only truly manipulated on the outermost
    of the nested calls.
    </para>
    <para>
    If <code>SuspendOSInterrupts()</code> is called from a permitted context other than a Task or
    Category 2 ISR it is a no-operation, or if it is called within a code section that is controlled
    a <code>SuspendAllInterrupts()</code> or <code>DisableAllInterrupts()</code>, it is treated as
    a no-operation since interrupts are already blocked at a higher level.
    </para>
    <para>
    Interrupt lock timing is implemented for Tasks and ISRs; the current context's "OS Interrupts
    Lock Time" is used for <code>SuspendOSInterrupts()</code> and "All Interrupts Lock Time" is used
    for the other two system services. If timing is already active its state is saved before
    activating the interrupt lock timing.
    </para>
    <para>
    WARNING: if <code>SuspendOSInterrupts()</code> is called for the first time within a code section
    protected by <code>SuspendAllInterrupts()</code> or <code>DisableAllInterrupts()</code>, the
    "OS" interrupt lock timing is not activated. The checker should always ensure that if the "OS
    Interrupt Lock Time" is activated for an OS-Object, the "All interrupt lock time" is also activated
    and is less than or equal to the "OS interrupt lock time"
    </para>
  </description>
  <availability>
  </availability>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
