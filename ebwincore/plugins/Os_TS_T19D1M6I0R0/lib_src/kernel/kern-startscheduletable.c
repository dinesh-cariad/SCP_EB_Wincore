/* kern-startscheduletable.c
 *
 * This file contains the OS_KernStartScheduleTable function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startscheduletable.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if full error handling is enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if context checks are enabled.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if interrupt enable checks are enabled.
*/


#define OS_SID		varSid
#define OS_SID_ABS	OS_SID_StartScheduleTableAbs
#define OS_SID_REL	OS_SID_StartScheduleTableRel

/* This macro is used to pick the error code if error handling is disabled.
*/
#define OS_SID_EC	OS_SID_StartScheduleTableAbs

#define OS_SIF		*varSif
#define OS_SIF_ABS	OS_svc_StartScheduleTableAbs
#define OS_SIF_REL	OS_svc_StartScheduleTableRel

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernStartScheduleTable implements the APIs StartScheduleTableRel and StartScheduleTableAbs
 *
 * The specified schedule table is started such that the first event happens
 * either at time "offset" now or when the counter reaches "offset", depending
 * on the value of "rel".
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.StartAbsolute, 2
 * !LINKSTO Kernel.Autosar.ScheduleTable.StartRelative, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs, 2
 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel, 2
*/
os_result_t OS_KernStartScheduleTable
(	os_scheduleid_t s,
	os_tick_t offset,
	os_boolean_t rel
)
{
	/* Possible diagnostic TOOLDIAG-2 <2> */
	const os_serviceid_t varSid = ((rel != OS_FALSE) ? (os_serviceid_t)OS_SID_REL : (os_serviceid_t)OS_SID_ABS);
	const os_serviceinfo_t * const varSif = ((rel != OS_FALSE) ? &OS_SIF_REL : &OS_SIF_ABS);
	os_result_t r = OS_E_OK;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)s);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)offset);

	OS_TRACE_STARTSCHEDULETABLE_ENTRY(s,offset,rel);

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( s >= OS_nSchedules )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs.Invalid, 1
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.Invalid, 1
		*/
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( (rel != OS_FALSE) && (offset == 0u) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.OutOfRange, 2
		*/
		r = OS_ERROR(OS_ERROR_ParameterOutOfRange, OS_GET_PARAMETER_VAR());
	}
	else
	{
		const os_schedule_t * const ss = &OS_scheduleTableBase[s];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();
		const os_alarm_t * const as = &OS_alarmTableBase[ss->alarm];
		const os_counter_t * const cs = &OS_counterTableBase[as->counter];
		os_tick_t max;

		/* Calculate the max offset. For 'StartScheduleTableRel() we must be able to reach the first
		 * expiry point in a single step.
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs.OutOfRange, 1
		 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.OutOfRange, 2
		*/
		max = (rel != OS_FALSE) ? (cs->maxallowedvalue - ss->table->offset) : cs->maxallowedvalue;

		if ( !OS_HasPermission(app, ss->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( (rel != OS_FALSE) && ((ss->flags & OS_ST_IMPLICIT) != 0u) )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.ImplicitSync, 1
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.IMPLICIT, 1
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.IMPLICIT.Start, 1
			*/
			r = OS_ERROR(OS_ERROR_ImplicitSyncStartRel, OS_GET_PARAMETER_VAR());
		}
		else
		if ( offset > max )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableAbs.OutOfRange, 1
			 * !LINKSTO Kernel.Autosar.API.SystemServices.StartScheduleTableRel.OutOfRange, 2
			*/
			r = OS_ERROR(OS_ERROR_ParameterOutOfRange, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			/* !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT, 1
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.IMPLICIT, 1
			 * For IMPLICIT sync, 'rel' has already been excluded
			*/
			e = OS_DoStartScheduleTable(s, ss, as->counter, offset, rel);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_STARTSCHEDULETABLE_EXIT_P(r,s,offset,rel);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserStartScheduleTable</name>
  <synopsis>Start a schedule table</synopsis>
  <syntax>
    os_result_t OS_UserStartScheduleTable
    (   os_scheduleid_t s    /@ ID of table @/
        os_tick_t offset,    /@ Time of first event @/
        os_boolean_t rel     /@ TRUE if offset is relative @/
    )
  </syntax>
  <description>
    <code>OS_UserStartScheduleTable()</code> starts a schedule table such
    that the first expiry point occurs either <code>offset</code> ticks from
    now or when the underlying counter reaches the absolute <code>offset</code>
    value, depending on the value of <code>rel</code>.
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
