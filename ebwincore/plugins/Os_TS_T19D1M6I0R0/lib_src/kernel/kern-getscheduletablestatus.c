/* kern-getscheduletablestatus.c
 *
 * This file contains the OS_KernGetScheduleTableStatus() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getscheduletablestatus.c 27921 2017-11-20 16:30:08Z mist9353 $
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

#define OS_SID	OS_SID_GetScheduleTableStatus
#define OS_SIF	OS_svc_GetScheduleTableStatus

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>
#if (OS_N_CORES > 1) && (OS_KERNEL_TYPE != OS_MICROKERNEL)
#include <Os_messagequeue.h>
#endif

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetScheduleTableStatus implements the API GetScheduleTableStatus
 *
 * The status of the specified schedule table is written to the specified
 * output location.
 *
 * Interrupts are enabled on entry and remain so throughout. The
 * status returned is therefore a snapshot of the schedule table's state,
 * and could have changed by the time the caller actually gets it.
 * However, this could happen anyway.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetScheduleTableStatus, 2
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.QuerySynch, 2
*/
os_result_t OS_KernGetScheduleTableStatus
(	os_scheduleid_t s,
	os_schedulestatus_t *out
)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)s);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETSCHEDULETABLESTATUS_ENTRY(s);

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
	if ( !OS_IsValidScheduleId(s) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.GetScheduleTableStatus.Invalid, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidScheduleId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_APPISACCESSIBLE(OS_scheduleTableBase[s].app) )
	{
		r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
	}
	else
	{
		const os_schedule_t *ss = &OS_scheduleTableBase[s];
#if (OS_N_CORES == 1) || (OS_KERNEL_TYPE == OS_MICROKERNEL)
		*out = ss->dynamic->status;
#else
		os_errorresult_t e = OS_ERRORCODE_NOCHECK(OS_ERROR_NoError);

		if (OS_GetScheduleTableCoreId(ss) == OS_GetMyCoreId())
		{
			*out = ss->dynamic->status;
		}
		else
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.GetScheduleTableStatus.CrossCore, 1
			*/
			e = OS_SendGetScheduleTableStatus(s, ss, out);
		}

		if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
		{
			r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
		}
#endif
	}

	OS_TRACE_GETSCHEDULETABLESTATUS_EXIT_P(r,s);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetScheduleTableStatus</name>
  <synopsis>Get a schedule table's status</synopsis>
  <syntax>
    os_result_t OS_UserGetScheduleTableStatus
    (   os_scheduleid_t s   /@ ID of schedule table @/
        os_uint8_t *out     /@ Where to put the result @/
    )
  </syntax>
  <description>
    <code>OS_UserGetScheduleTableStatus()</code> writes the current
    status of the schedule table to the specified location.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>Status=Success</returns>
  <returns>OS_E_OK=Success</returns>
  <returns>OS_E_ACCESS=Schedule Table's application is not accessbile.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
