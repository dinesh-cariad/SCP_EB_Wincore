/* kern-releaseresource.c
 *
 * This file contains the OS_KernReleaseResource function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-releaseresource.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if timing protection is enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: UnusedVariable
 *   Variable is never used.
 *
 * Reason: Not an issue, variable will be used if the scheduling algorithm is CLZ.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if ISRs are allowed to take resources.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if context checks are enabled.
 *
 * TOOLDIAG-5) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if status is EXTENDED.
 *
 * TOOLDIAG-6) Possible diagnostic: StatementIsUnreachable
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

#define OS_SID	OS_SID_ReleaseResource
#define OS_SIF	OS_svc_ReleaseResource

#include <Os_kernel.h>
#include <Os_taskqueue.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*
 * Created these two helper functions to reduce cyclomatic complexity of the
 * function OS_KernReleaseResource
*/
static os_result_t OS_DoReleaseResourceTask
	(os_resourceid_t, os_kerneldata_t * const);
static os_result_t OS_DoReleaseResourceISRC2
	(os_resourceid_t, os_kerneldata_t * const);

static os_result_t OS_DoReleaseResourceTask
(	os_resourceid_t r,
	os_kerneldata_t * const kernel_data
)
{
	os_resourcedynamic_t *rd;
	os_taskdynamic_t *td;
	os_intstatus_t is;
	os_prio_t currprio;
	os_prio_t prevprio;
	os_result_t result = OS_E_OK;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)r);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)kernel_data);

	td = kernel_data->taskCurrent->dynamic;
	rd = OS_ResourceGetDynamic(r);

	is = OS_IntDisable();

	/* Is the lock (i.e. resource or spinlock) at the head of the list of locks that the
	 * task has taken? If it isn't, this means one of two things:
	 *
	 * - the task is releasing locks that it never successfully took,
	 *   or in the wrong order, or
	 * - the task's locks have been freed by an outside agency
	 *
	 * See the "else" cases for an explanation.
	 * We could make the test for "still running" first, but that
	 * would add computation to the "good" branch, thus increasing the
	 * normal execution time.
	*/
	if ( td->lastLock == OS_CreateObjectId(r, OS_OBJ_RESOURCE) )
	{
		/* Resource was the last one taken by the task.
		 * OK, let's free it up ...
		 *
		 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.NestedResources, 1
		*/

		/* Remember the old (before ReleaseResource) task priority,
		 * and unlink the lock (i.e. resource, spinlock) from the chain
		*/
		currprio = td->prio;
		td->lastLock = rd->next;

		/* Make a note of the priority stored in the resource, then
		 * clean up the resource data so that we know
		 * nothing occupies it.
		*/
		prevprio = rd->lastPrio;
		rd->next = OS_NULLOBJECT;
		rd->takenBy = OS_NULLTASK;
		rd->lastPrio = 0;

		/* Stop resource-lock timing, restore previous timing
		*/
		if ( OS_GET_RLOCK((kernel_data->taskCurrent->resourceLockTime != OS_NULL) &&
				(kernel_data->taskCurrent->resourceLockTime[r] != 0)) )
		{
			OS_STOPRESLOCKTIMING(rd);
		}


		if ( (currprio == prevprio) || (td->state != OS_TS_RUNNING) )
		{
			/* No priority change, or the task was killed by OS_STOPRESLOCKTIMING().
			 * Interrupt status is restored in this branch.
			*/
			OS_IntRestore(is);
		}
		else
		{
			/* Lower the current task's priority.
			*/
			OS_LowerCurrentTasksPriority(prevprio);

			/* If the priority of the resource being released is an
			 * interrupt priority, we must set the interrupt level
			 * even if the priority being restored is a task priority.
			 * In the latter case we must set the interrupt level to 0.
			*/
			if ( OS_IsIsrPrio(currprio) )
			{
				/* This sets the current interrupt level, so we must
				 * not restore the saved interrupt status afterwards.
				*/
				if ( !OS_IsIsrPrio(prevprio) )
				{
					/* Possible diagnostic TOOLDIAG-3 <+1> */
					prevprio = OS_NULLISRPRIO;
				}

				/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
				OS_SetIsrResourceLevel(prevprio);
				OS_IntRestoreHardLock(is);
			}
			else
			{
				/* Interrupt status is restored in this branch.
				*/
				OS_IntRestore(is);
			}
		}

		/* No call to OS_IntRestore() here. See comments above
		*/

	}
	else
	if ( td->state == OS_TS_RUNNING )
	{
		/* The first of the 'else' cases. If the task is still
		 * in the running state, then it alone is responsible for
		 * the error.
		*/
		OS_IntRestore(is);
		/* OSEK spec for ReleaseResource says:
		 * 1. E_OS_NOFUNC if resource is not occupied or another resource
		 *    needs releasing first.
		 * 2. E_OS_ACCESS if resource has lower priority than task base.
		 *
		 * Case 1: we're not going to release if the resource wasn't the
		 * last one taken by this task. If the resource *is* the last one
		 * taken, it implies it was taken ...
		 *
		 * Case 2: cannot happen because GetResource wouldn't allow it.
		 *
		 * The OSEK/VDX spec is weird here ...
		 *
		 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.Unoccupied, 1
		*/
		result = OS_ERROR(OS_ERROR_ResourceNestingError, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* The second of the 'else' cases. If the task is no longer
		 * in the running state, it has been killed, perhaps as
		 * a result of a time-limit exceeded interrupt.
		 * In this case we just return and let the dispatcher handle
		 * the task switch.
		*/
		OS_IntRestore(is);
	}

	return result;
}

static os_result_t OS_DoReleaseResourceISRC2
(	os_resourceid_t r,
	os_kerneldata_t * const kernel_data
)
{
	os_intstatus_t is;
	os_resourcedynamic_t *rd;
	os_prio_t prevprio;
	os_result_t result = OS_E_OK;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)r);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)kernel_data);

	/* Disable interrupts to prevent the execution timer from messing
	 * with a partially-modified resource structure, if it trips in
	 * just at the wrong time
	*/
	is = OS_IntDisable();

	/* !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.NestedResources, 1
	*/
	if ( OS_IsrTookLock(r, OS_OBJ_RESOURCE) )
	{
		/* Possible diagnostic TOOLDIAG-3 <+2> */
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_isr_t * const isrp = &OS_isrTableBase[kernel_data->isrCurrent];

		/* Resource was the last one taken by the ISR.
		 * OK, let's free it up ...
		*/
		rd = OS_ResourceGetDynamic(r);

		/* Unlink the resource from the chain
		*/
		kernel_data->isrLastLock = rd->next;

		/* Make a note of the priority stored in the resource, then
		 * clean up the resource data so that we know
		 * nothing occupies it.
		*/
		prevprio = rd->lastPrio;
		rd->next = OS_NULLOBJECT;
		rd->takenBy = OS_NULLTASK;
		rd->lastPrio = 0;

		/* Stop resource-lock timing, restore previous timing.
		 * We must disable interrupts here while manipulating
		 * the global accounting structure.
		*/
		if ( OS_GET_RLOCK((isrp->resourceLockTime != OS_NULL) && (isrp->resourceLockTime[r] != 0)) )
		{
			OS_STOPRESLOCKTIMING(rd);
		}

		/* Switch the interrupt level to previous value stored in the
		 * resource.
		 * No need to re-enable interrupts here - setting the level to
		 * the saved level does that.
		*/
		/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
		OS_SetIsrResourceLevel(prevprio);
		OS_IntRestoreHardLock(is);
	}
	else
	{
		OS_IntRestore(is);

		result = OS_ERROR(OS_ERROR_ResourceNestingError, OS_GET_PARAMETER_VAR());
	}

	return result;
}

/* OS_KernReleaseResource
 *
 * This function implements the ReleaseResource system service in the kernel.
 *
 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.API, 1
 *
 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.Schedule, 1
 *		The system-call dispatcher automatically handles this.
*/
os_result_t OS_KernReleaseResource(os_resourceid_t r)
{
	/* Possible diagnostic TOOLDIAG-2 <1> */
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_result_t result = OS_E_OK;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)r);

	OS_TRACE_RELEASERESOURCE_ENTRY(r);

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		result = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheckAlways() )
	{
		result = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_IsValidResourceId(r) )
	{
		/* !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.InvalidResource, 1
		*/
		/* Possible diagnostic TOOLDIAG-5 <+1> */
		result = OS_ERROR(OS_ERROR_InvalidResourceId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( kernel_data->inFunction == OS_INTASK )
	{
		/* !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.Task, 1
		*/
		result = OS_DoReleaseResourceTask(r, kernel_data);
	}
	else
	if ( OS_IS_INCAT2( kernel_data->inFunction ) )
	{
		/* !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.ISRC2, 1
		*/
		/* Possible diagnostic TOOLDIAG-6 <+1> */
		result = OS_DoReleaseResourceISRC2(r, kernel_data);
	}
	else
	{
		/* MISRA */
	}

	OS_TRACE_RELEASERESOURCE_EXIT_P(result,r);
	return result;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserReleaseResource</name>
  <synopsis>Leave a critical section</synopsis>
  <syntax>
    os_result_t OS_UserReleaseResource
    (   os_resourceid_t r    /@ ID of the resource @/
    )
  </syntax>
  <description>
    <code>OS_UserReleaseResource()</code> signals that the calling task
    has left a critical section of code associated with the resource. Other
    tasks that use the same resource are now permitted to run.
    <para>A task must release resources in the reverse order to which they
    were taken.</para>
  </description>
  <availability>
    <code>OS_UserReleaseResource()</code> may be used in tasks. On some
    architectures <code>OS_UserReleaseResource()</code> can be called
    from Category 2 ISRs as well.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
