/* kern-gettaskid.c
 *
 * This file contains the OS_KernGetTaskId function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-gettaskid.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if context checks are enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
*/


#define OS_SID	OS_SID_GetTaskId
#define OS_SIF	OS_svc_GetTaskId

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetTaskId implements the API GetTaskID
 *
 * The id of the current task (or OS_NULLTASK if none) is
 * returned. It is up to the user library routine to write the
 * task Id to the specified location and return the appropriate
 * StatusType result.
 *
 * It is not permitted to write to addresses specified by the
 * user task while in kernel mode without first checking that
 * the application has permission to write there. It is therefore
 * quicker to return a value to the application and allow it
 * to attempt the write while in userland.
 *
 * NOTES:
 * 1. Interrupts are enabled on entry and remain so throughout.
 *    However, OS_taskCurrent cannot change during this time, because
 *    it only gets changed in the dispatcher, which can only run after
 *    the handler returns.
 * 2. If this sytem call is called from an ISR, it returns the ID of the
 *    task that was running when the interrupt occurred, even if a new
 *    higher-priority task has been activated earlier in the ISR.
*/
os_result_t OS_KernGetTaskId(os_taskid_t *out)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)out);

	OS_TRACE_GETTASKID_ENTRY();

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( OS_ArchCanWrite(out, sizeof(*out)) == 0 )
	{
		r = OS_ERROR(OS_ERROR_WriteProtect, OS_GET_PARAMETER_VAR());
	}
	else
	if ( OS_GetKernelData()->taskCurrent == OS_NULL )
	{
		/* !LINKSTO Kernel.API.TaskManagement.GetTaskID.ISRC2, 1
		 * !LINKSTO Kernel.API.TaskManagement.GetTaskID.Hook, 1
		*/
		*out = OS_NULLTASK;
	}
	else
	{
		/* !LINKSTO Kernel.API.TaskManagement.GetTaskID.TaskID, 1
		*/
		*out = OS_GetKernelData()->taskCurrent->taskId;
	}

	OS_TRACE_GETTASKID_EXIT_P(r);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetTaskId</name>
  <synopsis>Get the ID of the current task</synopsis>
  <syntax>
    os_result_t OS_UserGetTaskId(os_taskid_t *out)
  </syntax>
  <description>
    <code>OS_UserGetTaskId()</code> writes the ID of the current task
    to the user-specified location "out".
    If no task is currently running, <code>OS_NULLTASK</code> is written
    instead.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>TaskId=ID of current task</returns>
  <returns>OS_NULLTASK=No task is running</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
