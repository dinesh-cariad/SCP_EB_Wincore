/* kern-cancelalarm.c
 *
 * This file contains the OS_KernCancelAlarm() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-cancelalarm.c 27921 2017-11-20 16:30:08Z mist9353 $
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
 *  Whether this code can be reached depends on the OS configuration.
 *  It can be reached, if "calling context checks" are not excluded.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  Whether this code can be reached depends on the OS configuration.
 *  It can be reached, if "interrupt enable checks" are not excluded.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  Whether this code can be reached depends on the OS configuration.
 *  It can be reached, if EXTENDED status is used.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 14.3 (requried)
 * Controlling expressions shall not be invariant.
 *
 * Reason:
 * The invariance depends on the configuration.
 */

#define OS_SID OS_SID_CancelAlarm
#define OS_SIF OS_svc_CancelAlarm

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernCancelAlarm
 *
 * The alarm 'a' is cancelled, provided it is currently in use.
 *
 * !LINKSTO Kernel.API.Alarms.CancelAlarm.API, 1
*/
os_result_t OS_KernCancelAlarm(os_alarmid_t a)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);

	OS_TRACE_CANCELALARM_ENTRY(a);

	/* Deviation MISRAC2012-1 */
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
		/* !LINKSTO Kernel.API.Alarms.CancelAlarm.InvalidAlarm, 1
		*/
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidAlarmId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <2> */
		const os_appcontext_t * const app = OS_CurrentApp();
		const os_alarm_t * const as = &OS_alarmTableBase[a];

		if ( !OS_HasPermission(app, as->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			e = OS_DoCancelAlarm(a, as);
			if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_CANCELALARM_EXIT_P(r,a);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserCancelAlarm</name>
  <synopsis>Cancel an alarm</synopsis>
  <syntax>
    os_result_t OS_UserCancelAlarm
    (   os_alarmid_t a    /@ ID of the alarm @/
    )
  </syntax>
  <description>
    <code>OS_UserCancelAlarm()</code> resets the expiration time of
    the specified alarm.
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
