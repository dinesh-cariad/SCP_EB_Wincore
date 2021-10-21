/* kern-stopscheduletable.c
 *
 * This file contains the OS_KernStopScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-stopscheduletable.c 28040 2017-12-07 13:30:18Z olme8414 $
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
 *   This statement can be reached if context checks are enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if interrupt enable checks are enabled.
*/

#define OS_SID OS_SID_StopScheduleTable
#define OS_SIF OS_svc_StopScheduleTable

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
 * OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible)
 * OS_ERRORCODE_CHECK(OS_ERROR_NotRunning)
 *
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernStopScheduleTable implements the API StopScheduleTable
 *
 * The specified schedule table is stopped immediately.
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Stop, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.StopScheduleTable, 2
*/
os_result_t OS_KernStopScheduleTable(os_scheduleid_t s)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)s);

	OS_TRACE_STOPSCHEDULETABLE_ENTRY(s);

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( s >= OS_nSchedules )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StopScheduleTable.Invalid, 1
		*/
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();
		const os_schedule_t * const ss = &OS_scheduleTableBase[s];

		if ( !OS_HasPermission(app, ss->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			e = OS_DoStopScheduleTable(s, ss);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_STOPSCHEDULETABLE_EXIT_P(r,s);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserStopScheduleTable</name>
  <synopsis>Stop a schedule table</synopsis>
  <syntax>
    os_result_t OS_UserStopScheduleTable
    (   os_scheduleid_t s    /@ ID of table @/
    )
  </syntax>
  <description>
    <code>OS_UserStopScheduleTable()</code> stops a schedule table
    immediately. If another schedule table has been chained behind
    the specified schedule table, that chained table is also placed
    in the STOPPED state. If the specified schedule table is itself
    in the CHAINED state, the chaining link is broken.
  </description>
  <availability>
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
