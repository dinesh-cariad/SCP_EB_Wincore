/* kern-chaintask.c
 *
 * This file contains the OS_KernChainTask function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-chaintask.c 27935 2017-11-22 11:52:06Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements (error reporting only) can be reached, if status is EXTENDED.
*/

#define OS_SID	OS_SID_ChainTask
#define OS_SIF	OS_svc_ChainTask

/* Error codes returned by DoActivateTask.
 * This comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_RateLimitExceeded)
 * OS_ERRORCODE_CHECK(OS_ERROR_Quarantined)
 * OS_ERRORCODE_CHECK(OS_ERROR_MaxActivations)
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
 * OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible)
*/

#include <Os_kernel.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "tasks are implemented by the microkernel - check your makefiles."
#endif

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernChainTask implements the API ChainTask.
 *
 * ChainTask is functionally equivalent to ActivateTask followed by TerminateTask. However,
 * one or two niceties have to be observed:
 *
 * - if the ActivateTask call fails the calling task does not terminate.
 * - if the TerminateTask would fail, the task to chain is not activated.
 * - if the task to chain is the current task, E_OS_LIMIT shall not be returned.
 *
 * !LINKSTO Kernel.API.TaskManagement.ChainTask.API, 1
 *
 * !LINKSTO Kernel.API.TaskManagement.ChainTask.Schedule, 1
 *      Happens automatically when this function returns.
 *
 * !LINKSTO Kernel.API.TaskManagement.ChainTask.InternalResource, 1
 *      Happens automatically because internal resources are
 *      implemented by setting the run priority of the task.
 *
 * !LINKSTO Kernel.API.TaskManagement.ChainTask.Events, 1
 *		Happens when
*/
os_result_t OS_KernChainTask(os_taskid_t t)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_task_t *otp;	/* Old task */
	os_taskdynamic_t *otd;
	const os_task_t *ntp;	/* New task */
	os_result_t r = OS_E_OK;
	os_intstatus_t is;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)t);

	OS_TRACE_CHAINTASK_ENTRY(t);

	if ( kernel_data->inFunction != OS_INTASK )
	{
		/* !LINKSTO Kernel.API.TaskManagement.ChainTask.ISRC2, 1
		*/
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_IsValidTaskId(t) )
	{
		/* !LINKSTO Kernel.API.TaskManagement.ChainTask.InvalidTask, 1
		*/
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidTaskId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		otp = kernel_data->taskCurrent;
		otd = otp->dynamic;

		if ( !OS_InterruptEnableCheckFromTask(OS_IEC_OSEKEXTRA, otd) )
		{
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
		else
		if ( OS_TaskOccupiesResource(otd) )
		{
			/* Note: This block can only be reached, if status is EXTENDED. */
			/* Possible diagnostic TOOLDIAG-2 <START> */
			/* !LINKSTO Kernel.API.TaskManagement.ChainTask.ExternalResource, 1
			*/
			if (OS_GetTypeFromObjectId(otd->lastLock) == OS_OBJ_SPINLOCK)
			{
				/* !LINKSTO Kernel.Autosar.Multicore.ChainTask.Spinlock, 1
				*/
				r = OS_ERROR(OS_ERROR_HoldsLock, OS_GET_PARAMETER_VAR());
			}
			else
			{
				r = OS_ERROR(OS_ERROR_HoldsResource, OS_GET_PARAMETER_VAR());
			}
			/* Possible diagnostic TOOLDIAG-2 <STOP> */
		}
		else
		{
			ntp = &OS_taskTableBase[t];

			if ( !OS_HasPermission(OS_CurrentApp(), ntp->permissions) )
			{
				r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
			}
			else
			{
				os_errorresult_t e;

				if ( ntp == otp )
				{
					/* We cannot activate then terminate here because that might result in a spurious
					 * E_OS_LIMIT.
					 * To avoid having special handling, we do the termination first. This unfortunately
					 * means that an error caused during the activation won't be returned as an error code.
					 * However, the only error that can occur is a rate limit error, and this can
					 * be ignored on the following grounds:
					 *
					 * 1. response to rate limit errors is poorly defined
					 * 2. it is very unlikely that a task with a rate limit will be chaining itself
					 * 3. if the rate error is ignored we could end up with E_OS_LIMIT too!
					 *
					 * The terminate/activate is done with interrupts disabled to avoid strangeness
					 * caused by the task getting killed from an interrupt.
					 * If the task has already been killed we don't do anything.
					*/
					is = OS_IntDisable();
					if ( otd->state == OS_TS_RUNNING )
					{
						OS_DoTerminateTask(otp, otd);
						e = OS_LocalActivateTask(ntp);
					}
					else
					{
						/* No error reported here - task has already been killed
						*/
						e = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);
					}
					OS_IntRestore(is);
				}
				else
				{
					/* !LINKSTO Kernel.API.TaskManagement.ChainTask.OtherTask, 1
					*/
					e = OS_DoActivateTask(ntp);
					if ( e == OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
					{
						OS_DoTerminateTask(otp, otp->dynamic);
					}
				}

				if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
				{
					r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
				}
			}
		}
	}

	OS_TRACE_CHAINTASK_EXIT_P(r,t);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserChainTask</name>
  <synopsis>Terminate the current task and activate another</synopsis>
  <syntax>
    os_result_t OS_UserChainTask
    (   os_taskid_t t    /@ ID of the task @/
    )
  </syntax>
  <description>
    <code>OS_UserChainTask()</code> terminates the current task and
    activates another. The activated task may be the same as the calling
    task, in which case the new activation is guaranteed not to exceed
    the maximum number of activations for the task. The function may
    return of there is an error.
  </description>
  <availability>
    <code>OS_UserChainTask()</code> may only be called from a task.
  </availability>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
