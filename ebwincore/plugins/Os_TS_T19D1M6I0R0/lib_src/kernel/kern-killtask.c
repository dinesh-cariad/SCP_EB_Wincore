/* kern-killtask.c
 *
 * This file contains the OS_KillTask function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-killtask.c 23523 2016-02-04 07:13:12Z tojo2507 $
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KillTask
 *
 * The specified task is put into the SUSPENDED or QUARANTINED state. Any
 * resources occupied are freed, and all activations of the task are removed
 * from the task queue.
 *
 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.KillTask, 4
*/
os_result_t OS_KillTask(const os_task_t *t, os_taskstate_t ts)
{
	os_taskdynamic_t *td = t->dynamic;
	os_intstatus_t is;
	os_result_t result = OS_E_OK;

	is = OS_IntDisable();

	/* Release any resources or spinlocks that the task is occupying.
	*/
	OS_ReleaseLocks(td->lastLock);
	td->lastLock = OS_NULLOBJECT;

	/* Eliminate any nested interrupt locks. No need to manipulate the
	 * interrupt mechanisms - the kernel sorts all that out itself.
	 * This merely prevents weirdness when the task gets re-activated.
	*/
	td->nestSuspendOs = 0;
	td->nestSuspendAll = 0;

	switch ( td->state )
	{
	case OS_TS_NEW:				/* New - is in queue, has no stack */
		OS_DequeueAll(t);

		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	case OS_TS_RUNNING:			/* Running - is in queue, no context */
		if ( (OS_GetKernelData()->accounting.taskTimingNesting == 0)
			&& (OS_GetKernelData()->accounting.inFunction == OS_INTASK) )
		{
			OS_ARCH_STOPTASKEXECTIMING(t);
			OS_STOPTASKEXECTIMING(t);
		}
		/* Call post-task hook before task goes to SUSPENDED state.
		 * Interrupts are already disabled here.
		 *
		 * !LINKSTO Kernel.API.Hooks.PostTaskHook.API, 1
		 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
		 *		Interrupts are disabled here
		*/
		OS_CALLPOSTTASKHOOK();
		OS_DequeueAll(t);
		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	case OS_TS_READY_SYNC:		/* Ready - is in queue, has context */
	case OS_TS_READY_ASYNC:		/* Ready - is in queue, has context */
		OS_DequeueAll(t);
		OS_FreeTaskContext(t);
		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	case OS_TS_WAITING:			/* Waiting - is not in queue, has context */
		OS_FreeTaskContext(t);
		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	case OS_TS_SUSPENDED:		/* Suspended - nothing to do */
		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	case OS_TS_QUARANTINED:		/* Quarantined - nothing to do */
		OS_TRACE_STATE_TASK( t->taskId, td->state, ts );
		td->state = ts;
		break;

	default:						/* Invalid state - leave as is */
		break;
	}

	OS_IntRestore(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
