/* kern-startscheduletablesynchron.c
 *
 * This file contains the OS_KernStartScheduleTableSynchron() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startscheduletablesynchron.c 27926 2017-11-21 17:19:25Z mist9353 $
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

#define OS_SID OS_SID_StartScheduleTableSynchron
#define OS_SIF OS_svc_StartScheduleTableSynchron

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernStartScheduleTableSynchron
 *
 * The specified schedule table is placed in the "waiting" state
 * and will start only when SyncScheduleTable is called for it.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron, 2
*/
os_result_t OS_KernStartScheduleTableSynchron(os_scheduleid_t s)
{
	os_result_t r = OS_E_OK;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)s);

	OS_TRACE_STARTSCHEDULETABLESYNCHRON_ENTRY(s);

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
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron.Invalid, 2
		*/
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		const os_schedule_t * const ss = &OS_scheduleTableBase[s];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t *app = OS_CurrentApp();

		if ( !OS_HasPermission(app, ss->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( (ss->flags & OS_ST_SYNCABLE) == 0u )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableSynchron.NonSynch, 1
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.IMPLICIT, 1
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.IMPLICIT.Start, 1
			*/
			r = OS_ERROR(OS_ERROR_NotSyncable, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			/* !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT, 1
			*/
			e = OS_DoStartScheduleTableSynchron(s, ss);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_STARTSCHEDULETABLESYNCHRON_EXIT_P(r,s);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserStartScheduleTableSynchron</name>
  <synopsis>Start a schedule table synchronously</synopsis>
  <syntax>
    os_result_t OS_UserStartScheduleTableSynchron
    (   os_scheduleid_t s     /@ ID of table @/
    )
  </syntax>
  <description>
    <code>OS_UserStartScheduleTableSynchron()</code> places a schedule table
    into the WAITING state so that it will start synchronously when
    global time becomes available.
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
