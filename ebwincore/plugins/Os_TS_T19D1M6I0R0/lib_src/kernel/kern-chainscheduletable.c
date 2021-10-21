/* kern-chainscheduletable.c
 *
 * This file contains the OS_KernChainScheduleTable function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-chainscheduletable.c 27926 2017-11-21 17:19:25Z mist9353 $
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

#define OS_SID OS_SID_ChainScheduleTable
#define OS_SIF OS_svc_ChainScheduleTable

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
 * OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible)
 * OS_ERRORCODE_CHECK(OS_ERROR_NotRunning)
 * OS_ERRORCODE_CHECK(OS_ERROR_NotStopped)
*/

#include <Os_kernel.h>


/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernChainScheduleTable implements the API ChainScheduleTable
 *
 * The specified "next" schedule table is chained to be started when the
 * specified "current" schedule table's current period finishes.
 *
 * Checks:
 * - both tables must exist and be accessible by the current app
 * - both tables must be driven by the same counter
 * - next table to chain must be stopped
 * - current table must be running
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Chain, 2
*/
os_result_t OS_KernChainScheduleTable
(	os_scheduleid_t sc,
	os_scheduleid_t sn
)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)sc);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)sn);

	OS_TRACE_NEXTSCHEDULETABLE_ENTRY(sc,sn);

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
	if ( (sc >= OS_nSchedules) || (sn >= OS_nSchedules) )
	{
		/* One or both of the specified tables is invalid
		 *
		 * !LINKSTO Kernel.Autosar.API.SystemServices.ChainScheduleTable.Invalid, 1
		*/
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();
		const os_schedule_t * const scs = &OS_scheduleTableBase[sc];
		const os_schedule_t * const sns = &OS_scheduleTableBase[sn];

		if ( (!OS_HasPermission(app, scs->permissions)) ||
			 (!OS_HasPermission(app, sns->permissions)) )
		{
			/* No permission for one or both of the tables
			*/
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( OS_alarmTableBase[scs->alarm].counter != OS_alarmTableBase[sns->alarm].counter )
		{
			/* The schedule tables are driven by different counters
			 *
			 * !LINKSTO Kernel.Autosar.API.SystemServices.ChainScheduleTable.DifferentCounter, 2
			*/
			r = OS_ERROR(OS_ERROR_DifferentCounters, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			e = OS_DoChainScheduleTable(sc, sn, scs);
			if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_NEXTSCHEDULETABLE_EXIT_P(r,sc,sn);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserChainScheduleTable</name>
  <synopsis>Chain a schedule table</synopsis>
  <syntax>
    os_result_t OS_UserChainScheduleTable
    (   os_scheduleid_t sc    /@ current table @/
        os_scheduleid_t sn    /@ next table @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserChainScheduleTable()</code> chains the schedule table
    <code>sn</code> to start after the current round of the table
    <code>sc</code> ends. Chaining is only permitted if the table to be
    chained is stopped and if the current table is running and does not
    already have a chained table.
    </para>
    <para>
    The timing is arranged such that the first action point of the chained
    table occurs at its proper offset after the end of the period of the
    "current" table. If the "current" table is not periodic, the first action
    point takes place at its offset from the last action point of the
    "current" table. The Autosar specification is silent on the latter case.
    </para>
    <para>
	<code>CAVEAT</code> The chaining takes place at the last action point
    of the "current" table. This means that if <code>NextScheduleTable()</code>
    (or <code>OS_UserChainScheduleTable()</code>)
    is called after this (for example, in the last schedule task)
    the running table will process one more complete round before the chaining
    takes place. If the "current" table is not periodic it may already have
    stopped and the call to <code>NextScheduleTable()</code> will fail with
    <code>OS_E_STATE</code>.
    </para>
  </description>
  <availability>
  <code>OS_UserChainScheduleTable()</code> can be called from tasks
  and category 2 ISRs.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
