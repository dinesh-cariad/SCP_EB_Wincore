/* kern-getalarmbase.c
 *
 * This file contains the OS_KernGetAlarmBase function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getalarmbase.c 27921 2017-11-20 16:30:08Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "calling context checks" are not excluded.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "interrupt enable checks" are not excluded.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if EXTENDED status is used.
*/

#define OS_SID	OS_SID_GetAlarmBase
#define OS_SIF	OS_svc_GetAlarmBase

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetAlarmBase
 *
 * The alarm base characterisitics for the alarm are written to the
 * indicated location.
 *
 * !LINKSTO Kernel.API.Alarms.GetAlarmBase.API, 1
 *
 * !LINKSTO Kernel.API.Alarms.GetAlarmBase.Hook, 1
 *		OS_KernGetAlarmBase() returns constant values, so the
 *		only thing that might limit the context in which it can be called
 *		might be the write permission function OS_ArchCanWrite().
 * !LINKSTO Kernel.Autosar.Multicore.GetAlarmBase.CrossCore, 1
 *		no restrictions based on the core the alarm is bound to.
*/
os_result_t OS_KernGetAlarmBase
(	os_alarmid_t a,
	os_alarmbase_t *out
)
{
	os_result_t r = OS_E_OK;
	const os_counter_t *c;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETALARMBASE_ENTRY(a);

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
	if ( ! OS_IsValidAlarmId( a ) )
	{
		/* !LINKSTO Kernel.API.Alarms.GetAlarmBase.InvalidAlarm, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidAlarmId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_APPISACCESSIBLE(OS_alarmTableBase[a].app) )
	{
		r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* !LINKSTO Kernel.API.Alarms.GetAlarmBase.Info, 1
		*/
		c = &OS_counterTableBase[OS_alarmTableBase[a].counter];
		out->maxallowedvalue = c->maxallowedvalue;
		out->ticksperbase = c->ticksperbase;
		out->mincycle = c->mincycle;
	}

	OS_TRACE_GETALARMBASE_EXIT_P(r,a);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetAlarmBase</name>
  <synopsis>Get alarm configuration</synopsis>
  <syntax>
    os_result_t OS_UserGetAlarmBase
    (   os_alarmid_t a,     /@ ID of the alarm @/
        os_alarmbase_t *out /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_UserGetAlarmBase()</code> places the configured parameters
    <code>maxallowedvalue</code>, <code>mincycle</code> and
    <code>ticksperbase</code> into the specified <code>out</code> variable.
    If an error occurs, the <code>out</code> variable remains unchanged.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success</returns>
  <returns>OS_E_ACCESS=Alarm's application is not accessible.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
