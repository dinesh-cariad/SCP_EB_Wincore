/* kern-getapplicationid.c
 *
 * This file contains the OS_KernGetApplicationId function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getapplicationid.c 27926 2017-11-21 17:19:25Z mist9353 $
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

#define OS_SID	OS_SID_GetApplicationId
#define OS_SIF	OS_svc_GetApplicationId

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetApplicationId implements the API GetApplicationID
 *
 * The id of the current application (or OS_NULLAPP if none) is
 * returned.
 *
 * !LINKSTO Kernel.Autosar.OsApplication.QueryCurrent, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetApplicationID, 2
*/
os_applicationid_t OS_KernGetApplicationId(void)
{
	os_applicationid_t a = OS_NULLAPP;

	OS_TRACE_GETAPPLICATIONID_ENTRY();

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
		a = OS_DoGetApplicationId();
	}

	OS_TRACE_GETAPPLICATIONID_EXIT_P(a);
	return a;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetApplicationId</name>
  <synopsis>Get the current application</synopsis>
  <syntax>
    os_applicationid_t OS_UserGetApplicationId(void)
  </syntax>
  <description>
    <code>OS_UserGetApplicationId()</code> returns the ID of the current
    application. If no category 2 ISR or task is running, or if the current
    ISR or task does not belong to an application, <code>OS_NULLAPP</code>
    is returned instead.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>AppId=ID of current application</returns>
  <returns>OS_NULLAPP=No application is running</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
