/* kern-gettaskstate.c
 *
 * This file contains the OS_KernGetTaskState function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-gettaskstate.c 27926 2017-11-21 17:19:25Z mist9353 $
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
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if status is EXTENDED.
*/

#define OS_SID	OS_SID_GetTaskState
#define OS_SIF	OS_svc_GetTaskState

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>
#if (OS_N_CORES > 1)
#include <Os_messagequeue.h>
#endif

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetTaskState implements the API GetTaskState
 *
 * The state of the specified task is returned. It is up to the
 * the user library routine to write the task Id to the specified
 * location and return the appropriate StatusType result.
 *
 * It is not permitted to write to addresses specified by the
 * user task while in kernel mode without first checking that
 * the application has permission to write there. It is therefore
 * quicker to return a value to the application and allow it
 * to attempt the write while in userland.
 *
 * Interrupts are enabled on entry and remain so throughout. The
 * state returned is therefore a snapshot of the task's state,
 * and could have changed by the time the caller actually gets it.
 * However, this could happen anyway.
*/
os_result_t OS_KernGetTaskState
(	os_taskid_t t,
	os_taskstate_t *out
)
{
#if (OS_N_CORES > 1)
	os_errorresult_t e;
#endif
	os_task_t const * tp;
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)t);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETTASKSTATE_ENTRY(t);

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
		/* !LINKSTO Kernel.API.TaskManagement.GetTaskState.InvalidState, 1
		*/
		r = OS_ERROR(OS_ERROR_WriteProtect, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_IsValidTaskId(t) )
	{
		/* !LINKSTO Kernel.API.TaskManagement.GetTaskState.InvalidTaskID, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidTaskId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_APPISACCESSIBLE(OS_taskTableBase[t].app) )
	{
		r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* !LINKSTO Kernel.API.TaskManagement.GetTaskState.TaskState, 1
		 */
		tp = &OS_taskTableBase[t];
#if (OS_N_CORES == 1)
		*out = tp->dynamic->state;
#else
		if (OS_GetTaskCoreId(tp) == OS_GetMyCoreId())
		{
			*out = tp->dynamic->state;
		}
		else
		{
			e = OS_SendGetTaskState(t, tp, out);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
#endif /* OS_N_CORES == 1 */
	}

	OS_TRACE_GETTASKSTATE_EXIT_P(r,t);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetTaskState</name>
  <synopsis>Get state of a task</synopsis>
  <syntax>
    os_result_t OS_UserGetTaskState
    (   os_taskid_t t,      /@ ID of task @/
        os_taskstate_t *out /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_UserGetTaskState()</code> writes the current state of the
    specified task to the location specified in the "out" parameter.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success</returns>
  <returns>OS_E_ID=Invalid Task ID specified</returns>
  <returns>OS_E_ACCESS=Task's application is not accessible</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
