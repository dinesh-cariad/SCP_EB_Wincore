/* kern-incrementcounter.c
 *
 * This file contains the OS_KernIncrementCounter() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-incrementcounter.c 27921 2017-11-20 16:30:08Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always false/true.
 *
 * Reason: This warning depends on the OS configuration and library optimization.
 *   If there are no hardware counters (OS_EXCLUDE_HWCOUNTER), this condition is always false.
 *   If there are no software counters (OS_EXCLUDE_SWCOUNTER), this condition is always true.
 *   Otherwise this condition must be evaluated at runtime.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "calling context checks" are not excluded.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "interrupt enable checks" are not excluded.
 *
 * TOOLDIAG-5) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if EXTENDED status is used.
 *
 * TOOLDIAG-6)  Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *   This warning depends on the OS configuration and library optimization.
 *   This statement can be reached if there are hardware counters.
 *   See also TOOLDIAG-2.
 *
 * TOOLDIAG-7)  Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *   This warning depends on the OS configuration and library optimization.
 *   These statement can be reached if there are software counters.
 *   See also TOOLDIAG-2.
*/

#define OS_SID	OS_SID_IncrementCounter
#define OS_SIF	OS_svc_IncrementCounter

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
 * OS_KernIncrementCounter
 *
 * The specified counter is advanced by 1. The alarm at the head of the
 * queue (if there is one) is decremented. If it reaches zero, the
 * alarm and all subsequent alarms with zero delta-count are triggered.
 *
 * After all the checks have been made, OS_AdvanceCounter does the job.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.SOFTWARE, 1
*/
os_result_t OS_KernIncrementCounter(os_counterid_t c)
{
	const os_counter_t *cs;
	os_result_t r = OS_E_OK;
	os_intstatus_t is;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)c);

	OS_TRACE_INCREMENTCOUNTER_ENTRY(c);

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_OSEKEXTRA) )
	{
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( ! OS_IsValidCounterId( c ) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.IncrementCounter.Invalid, 1
		*/
		/* Possible diagnostic TOOLDIAG-5 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidCounterId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();
		cs = &OS_counterTableBase[c];

		if ( !OS_HasPermission(app, cs->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( !OS_APPISACCESSIBLE(cs->app) )
		{
			r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
		}
		/* Possible diagnostic TOOLDIAG-2 <2> */
		else
		if ( OS_CounterIsHw(cs) )
		{
			/* Possible diagnostic TOOLDIAG-6 <+1> */
			r = OS_ERROR(OS_ERROR_CounterIsHw, OS_GET_PARAMETER_VAR());
		}
		else
		{
			/* Everything OK: we can advance the counter
			 *
			 * The return value of IncrementCounter() is not changed if OS_AdvanceCounter()
			 * detects an error - this will be done asynchronously via error hook callback.
			 * Therefore, the return value of OS_AdvanceCounter is ignored.
			 *
			 * !LINKSTO Kernel.Autosar.API.SystemServices.IncrementCounter.AlarmError, 1
			*/
#if (OS_N_CORES == 1) || (OS_KERNEL_TYPE == OS_MICROKERNEL)
			/* Possible diagnostic TOOLDIAG-7 <+1> */
			is = OS_IntDisable();
			(void) OS_AdvanceCounter(cs, cs->dynamic, 1, is);
			OS_IntRestore(is);
#else
			/* Possible diagnostic TOOLDIAG-7 <+1> */
			if (OS_GetCounterCoreId(cs) == OS_GetMyCoreId())
			{
				is = OS_IntDisable();
				(void) OS_AdvanceCounter(cs, cs->dynamic, 1, is);
				OS_IntRestore(is);
			}
			else
			{
				/* According to AUTOSAR we have to return E_OS_CORE here, instead of
				 * calling OS_SendAdvanceCounter.
				 *
				 * !LINKSTO Kernel.Autosar.API.SystemServices.IncrementCounter.NoCrossCore, 1
				*/
				r = OS_ERROR(OS_ERROR_CoreIsDown, OS_GET_PARAMETER_VAR());
			}
#endif
		}
	}

	OS_TRACE_INCREMENTCOUNTER_EXIT_P(r,c);

	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserIncrementCounter</name>
  <synopsis>Increment a counter</synopsis>
  <syntax>
    os_result_t OS_UserIncrementCounter
    (   os_counterid_t c    /@ ID of the counter @/
    )
  </syntax>
  <description>
    <code>OS_UserIncrementCounter()</code> increments a counter. If any
    alarms attached to the counter expire as a result, the configured
    action for that alarm is performed. The alarm action always runs in
    the context of the kernel.
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
