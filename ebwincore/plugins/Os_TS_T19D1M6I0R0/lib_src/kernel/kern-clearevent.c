/* kern-clearevent.c
 *
 * This file contains the OS_KernClearEvent function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-clearevent.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
*/

#define OS_SID	OS_SID_ClearEvent
#define OS_SIF	OS_svc_ClearEvent

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernClearEvent
 *
 * The specified events are cleared from the calling task's pending event mask.
 *
 * !LINKSTO Kernel.API.EventControl.ClearEvent.API, 1
*/
os_result_t OS_KernClearEvent(os_eventmask_t e)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_intstatus_t is;
	const os_task_t *ts;
	os_taskdynamic_t *td;
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)e);

	OS_TRACE_CLEAREVENT_ENTRY(e);

	if ( kernel_data->inFunction == OS_INTASK )
	{
		ts = kernel_data->taskCurrent;
		td = ts->dynamic;

		/* This is more efficient than the generic OS_InterruptEnableCheck() because we know we can only
		 * be in a task context here.
		*/
		if ( !OS_InterruptEnableCheckFromTask(OS_IEC_OSEKEXTRA, td) )
		{
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
		else
		if ( (ts->flags & OS_TF_EXTENDED) != 0 )
		{
			/* !LINKSTO Kernel.API.EventControl.ClearEvent.Events, 1
			*/
			is = OS_IntDisable();

			td->pEvents &= ~e;

			OS_IntRestore(is);
		}
		else
		{
			/* !LINKSTO Kernel.API.EventControl.ClearEvent.BasicTask, 1
			*/
			r = OS_ERROR(OS_ERROR_TaskNotExtended, OS_GET_PARAMETER_VAR());
		}
	}
	else
	{
		/* !LINKSTO Kernel.API.EventControl.ClearEvent.ISRC2, 1
		*/
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}

	OS_TRACE_CLEAREVENT_EXIT_P(r,e);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserClearEvent</name>
  <synopsis>Clear one or more events</synopsis>
  <syntax>
    os_result_t OS_UserClearEvent
    (   os_eventmask_t e    /@ Events to be cleared @/
    )
  </syntax>
  <description>
    <code>OS_UserClearEvent()</code> clears all the specified events
    from the current task's pending events.
  </description>
  <availability>
    <code>OS_UserClearEvent()</code> may only be called from a task.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
