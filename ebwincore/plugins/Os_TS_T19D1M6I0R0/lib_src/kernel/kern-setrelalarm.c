/* kern-setrelalarm.c
 *
 * This file contains the OS_KernSetRelAlarm function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-setrelalarm.c 27921 2017-11-20 16:30:08Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "calling context checks" are not excluded.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "interrupt enable checks" are not excluded.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if EXTENDED status is used.
*/

#define OS_SID	OS_SID_SetRelAlarm
#define OS_SIF	OS_svc_SetRelAlarm

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernSetRelAlarm
 *
 * The alarm is set to expire in the specified number of timer ticks.
 * The alarm's period is also set as specified.
 *
 * If the increment is 0, the OSEK/VDX OS specification says it is up
 * to the implementation what to do. Here, we have chosen to expire the
 * alarm immediately.
 * For Autosar conformance, an increment of zero results in an error.
 *
 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.API, 1
*/
os_result_t OS_KernSetRelAlarm
(	os_alarmid_t a,
	os_tick_t inc,
	os_tick_t cyc
)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)inc);
	OS_SAVE_PARAMETER_N(2,(os_paramtype_t)cyc);

	OS_TRACE_SETRELALARM_ENTRY(a,inc,cyc);

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
	if ( ! OS_IsValidAlarmId( a ) )
	{
		/* !LINKSTO Kernel.API.Alarms.SetRelAlarm.UnknownAlarm, 1
		*/
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidAlarmId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( inc == 0u )
	{
		/* !LINKSTO Kernel.Autosar.OSEK.Differences.SetRelAlarmZero, 1
		 *
		 * Without this check, the alarm would expire immediately.
		 * Perfectly OK, but not Autosar-conformant.
		*/
		r = OS_ERROR(OS_ERROR_IncrementZero, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();
		const os_alarm_t * const as = &OS_alarmTableBase[a];

		if ( !OS_HasPermission(app, as->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		{
			const os_counter_t * const cs = &OS_counterTableBase[as->counter];

			if ( (inc > cs->maxallowedvalue) ||
				 ( (cyc != 0u) &&
				   ( (cyc < cs->mincycle) || (cyc > cs->maxallowedvalue) ) ) )
			{
				/* !LINKSTO Kernel.API.Alarms.SetRelAlarm.InvalidIncrement, 1
				 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.InvalidCycle, 1
				*/
				r = OS_ERROR(OS_ERROR_ParameterOutOfRange, OS_GET_PARAMETER_VAR());
			}
			else
			{
				os_errorresult_t e;

				/* !LINKSTO Kernel.API.Alarms.SetRelAlarm.CyclicAlarm, 1
				 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.SingleAlarm, 1
				 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.Increment, 1
				*/
				e = OS_DoSetAlarm(a, as, cs, inc, cyc, OS_TRUE);
				if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
				{
					/* These errors can occur here:
					 * Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
					 *
					 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
					 * OS_ERRORCODE_CHECK(OS_ERROR_Quarantined);
					 * OS_ERRORCODE_CHECK(OS_ERROR_AlarmInUse);
					 * OS_ERRORCODE_CHECK(OS_ERROR_ApplicationNotAccessible);
					*/
					r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
				}
			}
		}
	}

	OS_TRACE_SETRELALARM_EXIT_P(r,a,inc,cyc);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserSetRelAlarm</name>
  <synopsis>Set an alarm at a relative counter value</synopsis>
  <syntax>
    os_result_t OS_UserSetRelAlarm
    (   os_alarmid_t a,    /@ ID of the alarm @/
        os_tick_t inc,     /@ First expiry time @/
        os_tick_t cyc      /@ Subsequent expiry times @/
    )
  </syntax>
  <description>
    <code>OS_UserSetRelAlarm()</code> sets the specified alarm to expire
    after <code>inc</code> ticks of its associated counter and,
    if the <code>cyc</code> parameter is non zero, thereafter every
    <code>cyc</code> ticks of the counter.
    <para>The values of <code>start</code> and <code>cyc</code> must lie within
    the permitted range configured for the counter.</para>
    <para>The specified alarm must not already be in use.</para>
    <para>If the <code>inc</code> value is very small, the
    alarm could expire before <code>OS_UserSetRelAlarm()</code> returns.</para>
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
