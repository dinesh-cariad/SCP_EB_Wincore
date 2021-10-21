/* kern-getresourcefromtask.c
 *
 * This file contains the OS_GetResourceFromTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getresourcefromtask.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if ISRs are allowed to take resources.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 *
 *
 * MISRAC2012-3) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 */

#define OS_SID	OS_SID_GetResource
#define OS_SIF	OS_svc_GetResource

#include <Os_kernel.h>
#include <Os_taskqueue.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_GetResourceFromTask
 *
 * This function implements part of the GetResource system service in the kernel, when the caller is a Task
*/
os_result_t OS_GetResourceFromTask(os_resourceid_t r, os_resourcedynamic_t *rd, os_paramtype_t *p)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_result_t result = OS_E_OK;
	const os_resource_t *rs = &OS_resourceTableBase[r];
	os_intstatus_t is;
	os_taskdynamic_t *td;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_appcontext_t * const app = OS_GET_APP(kernel_data->taskCurrent->app);

	OS_PARAM_UNUSED(p);

	/* !LINKSTO Kernel.API.ResourceManagement.GetResource.Task, 1
	*/
	if ( !OS_HasPermission(app, rs->permissions) )
	{
		result = OS_ERROR(OS_ERROR_Permission, p);
	}
	else
	if ( kernel_data->taskCurrent->queuePrio > rs->prio )
	{
		/* !LINKSTO Kernel.API.ResourceManagement.GetResource.LowCeilingPriority, 1
		 * !LINKSTO Kernel.API.ResourceManagement.ReleaseResource.LowCeilingPriority, 1
		*/
		result = OS_ERROR(OS_ERROR_ResourcePriorityError, p);
	}
	else
	{
		td = kernel_data->taskCurrent->dynamic;

		/* Now we must disable interrupts to prevent an ISR that
		 * takes the resource from overwriting what we write into
		 * the dynamic structure.
		*/
		is = OS_IntDisable();

		/* If the task is no longer running (perhaps killed by
		 * a timeout interrupt) we don't want to occupy the resource
		*/
		if ( td->state == OS_TS_RUNNING )
		{
			/* Chain the resource into the task's lock queue.
			 * !LINKSTO Kernel.API.ResourceManagement.GetResource.NestedResources, 1
			*/
			rd->next = td->lastLock;
			td->lastLock = OS_CreateObjectId(r, OS_OBJ_RESOURCE);

			/* Store the old task priority into the resource's
			 * save location.
			*/
			rd->lastPrio = td->prio;

			/* Set current task as taker of the resource
			*/
			rd->takenBy = OS_TaskToTOI(kernel_data->taskCurrent->taskId);

			/* Start resource-lock timing if necessary
			*/
			if ( OS_GET_RLOCK((kernel_data->taskCurrent->resourceLockTime != OS_NULL) &&
					(OS_GET_RLOCK(kernel_data->taskCurrent->resourceLockTime[r]) != 0)) )
			{
				OS_STARTRESLOCKTIMING(rd, kernel_data->taskCurrent->resourceLockTime[r]);
			}

			/* If resource's priority is higher, set the new task
			 * priority.
			 * We bail out here if the task is no longer running. It could have been shot down
			 * during OS_STARTRESLOCKTIMING().
			 * !LINKSTO Kernel.ResourceManagement.PriorityCeiling, 1
			*/
			if ( (td->state == OS_TS_RUNNING) && (rs->prio > rd->lastPrio) )
			{
				/* Raise the task's priority.
				 *
				 * !LINKSTO Kernel.API.ResourceManagement.GetResource.PriorityCeiling, 1
				*/
				OS_RaiseTaskPriority(td, rs->prio);

				if ( OS_IsIsrPrio(rs->prio) )
				{
					/* This sets the current interrupt level, so we must
					 * not restore the saved interrupt status afterwards.
					*/
					/* Possible diagnostic TOOLDIAG-2 <+2> */
					/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
					OS_SetIsrResourceLevel(rs->prio);
					OS_IntRestoreHardLock(is);
				}
				else
				{
					/* Interrupt status is restored in this branch.
					*/
					OS_IntRestore(is);
				}
			}
			else
			{
				/* Interrupt status is restored in this branch.
				*/
				OS_IntRestore(is);
			}
		}
		else
		{
			/* Killed task - just restore interrupt status.
			*/
			OS_IntRestore(is);
		}

		/* No call to OS_IntRestore() here. See comments above
		*/
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
