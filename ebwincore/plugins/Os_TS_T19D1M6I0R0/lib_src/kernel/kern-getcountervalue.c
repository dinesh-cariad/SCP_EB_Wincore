/* kern-getcountervalue.c
 *
 * This file contains the OS_KernGetCounterValue function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getcountervalue.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if context checks are enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if status is EXTENDED.
*/

#define OS_SID	OS_SID_GetCounterValue
#define OS_SIF	OS_svc_GetCounterValue

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
 * OS_KernGetCounterValue
 *
 * The current value of the specified counter
 * is written to the indicated location.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetCounterValue, 1
*/
os_result_t OS_KernGetCounterValue(os_counterid_t c, os_tick_t *out)
{
#if (OS_N_CORES > 1) && (OS_KERNEL_TYPE != OS_MICROKERNEL)
	os_errorresult_t e = OS_ERROR_NoError;
#endif
	os_counter_t const * cs;
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)c);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETCOUNTERVALUE_ENTRY(c);

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
	if ( !OS_IsValidCounterId( c ) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.GetCounterValue.Invalid, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidCounterId, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_APPISACCESSIBLE(OS_counterTableBase[c].app) )
	{
		r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Get the value
		*/
		cs = &OS_counterTableBase[c];

#if (OS_N_CORES == 1) || (OS_KERNEL_TYPE == OS_MICROKERNEL)
		*out = OS_GetCount(cs);
#else
		if (OS_GetCounterCoreId(cs) == OS_GetMyCoreId())
		{
			*out = OS_GetCount(cs);
		}
		else
		{
			e = OS_SendGetCount(c, cs, out);
		}

		if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
		{
			r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
		}
#endif
	}

	OS_TRACE_GETCOUNTERVALUE_EXIT_P(r,c);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetCounterValue</name>
  <synopsis>Get the current value of the counter</synopsis>
  <syntax>
    os_result_t OS_UserGetCounterValue
    (   os_counterid_t c, /@ ID of the counter @/
        os_tick_t *out    /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_UserGetCounterValue()</code> places the current value of
    the specified counter in the designated <code>out</code> variable.
    If the counter does not exist or another error is detected, the
    <code>out</code> variable remains unchanged.
    If this system service is called from an ISR of higher priority
    than the counter's own ISR, the count value might occasionally be slightly
    less than expected, but this will reflect the state of the alarms in the counter's
    queue.
  </description>
  <availability>
  </availability>
  <returns>OS_E_OK=Success</returns>
  <returns>OS_E_ACCESS=Counter's application is not accessible.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
