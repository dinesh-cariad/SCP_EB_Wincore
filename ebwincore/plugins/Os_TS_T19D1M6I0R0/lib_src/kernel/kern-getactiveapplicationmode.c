/* kern-getactiveapplicationmode.c
 *
 * This file contains the OS_KernGetActiveApplicationMode function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getactiveapplicationmode.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if "error handling for APIs returning void" is not excluded.
 *   See also OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI.
*/

#define OS_SID	OS_SID_GetActiveApplicationMode
#define OS_SIF	OS_svc_GetActiveApplicationMode

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetActiveApplicationMode
 *
 * Returns the current "application mode" (startup mode)
 *
 * !LINKSTO Kernel.API.OSControl.GetActiveApplicationMode.API, 1
*/
os_appmodeid_t OS_KernGetActiveApplicationMode(void)
{
	os_appmodeid_t m = OS_NULLAPPMODE;

	/* !LINKSTO Kernel.API.OSControl.GetActiveApplicationMode.ApplicationMode, 1
	*/

	OS_TRACE_GETACTIVEAPPLICATIONMODE_ENTRY();

	if ( !OS_CallingContextCheck() )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_WrongContext, OS_NULL);
		}
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
	}
	else
	{
		m = OS_GetAppMode();
	}

	OS_TRACE_GETACTIVEAPPLICATIONMODE_EXIT_P(m);
	return m;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetActiveApplicationMode</name>
  <synopsis>Get the current application mode</synopsis>
  <syntax>
    os_appmodeid_t OS_UserGetActiveApplicationMode(void)
  </syntax>
  <description>
    <code>OS_UserGetActiveApplicationMode</code> returns the application
    mode that was given to <code>OS_UserStartOs()</code> when the system
    started.
  </description>
  <availability>
  </availability>
  <returns>mode=Current application mode</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
