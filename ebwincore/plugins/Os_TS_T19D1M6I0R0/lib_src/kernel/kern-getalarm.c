/* kern-getalarm.c
 *
 * This file contains the OS_KernGetAlarm function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getalarm.c 27921 2017-11-20 16:30:08Z mist9353 $
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

#define OS_SID	OS_SID_GetAlarm
#define OS_SIF	OS_svc_GetAlarm

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
 * OS_KernGetAlarm
 *
 * The total number of ticks remaining before the alarm expires
 * is written to the indicated location.
 *
 * !LINKSTO Kernel.API.Alarms.GetAlarm.API, 1
*/
os_result_t OS_KernGetAlarm(os_alarmid_t a, os_tick_t *out)
{
	os_result_t r = OS_E_OK;
	const os_alarm_t *as;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)a);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETALARM_ENTRY(a);

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
		/* !LINKSTO Kernel.API.Alarms.GetAlarm.UnknownAlarm, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidAlarmId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		as = &OS_alarmTableBase[a];

		if ( !OS_HasPermission(OS_CurrentApp(), as->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		{
			os_errorresult_t e;

			e = OS_DoGetAlarm(a, as, out);
			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
	}

	OS_TRACE_GETALARM_EXIT_P(r,a);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetAlarm</name>
  <synopsis>Get the time remaining on the alarm</synopsis>
  <syntax>
    os_result_t OS_UserGetAlarm
    (   os_alarmid_t a,   /@ ID of the alarm @/
        os_tick_t *out    /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_UserGetAlarm()</code> calculates the time remaining before
    the specified alarm expires and places the result in the designated
    <code>out</code> variable. If the alarm is not in use or another
    error is detected, the <code>out</code> variable remains unchanged.
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
