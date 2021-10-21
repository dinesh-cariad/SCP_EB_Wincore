/* kern-syncscheduletable.c
 *
 * This file contains the OS_SyncScheduleTable function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-syncscheduletable.c 27926 2017-11-21 17:19:25Z mist9353 $
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

#define OS_SID	OS_SID_SyncScheduleTable
#define OS_SIF	OS_svc_SyncScheduleTable

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>
#include <Os_panic.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernSyncScheduleTable implements the API SyncScheduleTable
 *
 * The synchronisation variables (status and adjRemaining) of the
 * schedule table are set so as to bring the schedule table into
 * synchronisation with global time, either gradually or all at once
 * depending on the max decrease and increase attributes of the
 * schedule table.
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation, 2
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.GlobalTime, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable, 2
*/
os_result_t OS_KernSyncScheduleTable(os_scheduleid_t s, os_tick_t globalTime)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)s);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)globalTime);

	OS_TRACE_SYNCSCHEDULETABLE_ENTRY(s,globalTime);

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
		/* !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable.Invalid, 2
		*/
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		const os_schedule_t * const ss = &OS_scheduleTableBase[s];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();

		if ( globalTime > ss->period )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable.OutOfRange, 1
			*/
			r = OS_ERROR(OS_ERROR_ParameterOutOfRange, OS_GET_PARAMETER_VAR());
		}
		else
		if ( !OS_HasPermission(app, ss->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( (ss->flags & OS_ST_SYNCABLE) == 0u )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.SyncScheduleTable.NonSynch, 1
			*/
			r = OS_ERROR(OS_ERROR_NotSyncable, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			/* !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT, 1
			*/
			/* If globalTime is equal to period, set it to zero (in next round). This is because of the
			 * (slight) relaxation of globalTime > length ==> error  (was assumed >=).
			 *
			 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy.EXPLICIT.GlobalTime.Resolution, 1
			 * The following calculations all assume that the values provided to us as global time have
			 * the same range and resolution as local time.
			*/
			if ( globalTime == ss->period )
			{
				globalTime = 0;
			}

			e = OS_DoSyncScheduleTable(s, ss, globalTime);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_SYNCSCHEDULETABLE_EXIT_P(r,s,globalTime);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserSyncScheduleTable</name>
  <synopsis>Synchronise a schedule table to global time</synopsis>
  <syntax>
    os_result_t OS_UserSyncScheduleTable
    (   os_scheduleid_t s,     /@ Schedule table @/
        os_tick_t globalTime   /@ Current global time @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserSyncScheduleTable()</code> sets up the synchronisation
    variables of the schedule table such that the period will be adjusted
    at the next and subsequent end-of-round interrupts, subject to the
    configured maximum increase and maximum decrease values, until the
    time discrepancy is zero. When performing the adjustment, the
    adjustment direction is chosen so as to minimise the number of rounds
    taken to perform the synchronisation.
    </para>
    <para>
    The local time needed for the calculations is itself caclulated
    from the time-to-next-interrupt and the offset of the next expiry point.
    This means that processing delays in the schedule table mechanisms,
    including this function, cannot be eliminated.
    </para>
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
