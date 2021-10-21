/* kern-enableinterruptsource.c
 *
 * This file contains the OS_KernEnableInterruptSource() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-enableinterruptsource.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if status is EXTENDED.
*/

#define OS_SID	OS_SID_EnableInterruptSource
#define OS_SIF	OS_svc_EnableInterruptSource

/* Note: this comment is needed for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
*/

#include <Os_kernel.h>
#if (OS_N_CORES > 1)
#include <Os_messagequeue.h>
#endif

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernEnableInterruptSource implements the API EnableInteruptSource
 *
 * The interrupt source corresponding to the given parameter is enabled.
*/
os_result_t OS_KernEnableInterruptSource(os_isrid_t i)
{
	os_result_t r = OS_E_OK;
	const os_isr_t *isr;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)i);

	OS_TRACE_ENABLEINTERRUPTSOURCE_ENTRY(i);
	if (!OS_IsValidIsrId(i))
	{
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		r = OS_ERROR(OS_ERROR_InvalidIsrId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* !LINKSTO Kernel.Feature.InterruptSource.API.EnableInterruptSource, 1
		*/
		isr = &OS_isrTableBase[i];

#if (OS_N_CORES == 1)
		if ( !OS_APPISACCESSIBLE(isr->app) )
		{
			r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
		}
		else
		{
			OS_EnableIsr(isr);
		}
#else
		if ( OS_GetIsrCoreId(isr) == OS_GetMyCoreId() )
		{
			if ( !OS_APPISACCESSIBLE(isr->app) )
			{
				r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
			}
			else
			{
				OS_EnableIsr(isr);
			}
		}
		else
		{
			os_errorresult_t const e = OS_SendEnableInterruptSource(i, isr);

			if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
			{
				r = OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
#endif
	}

	OS_TRACE_ENABLEINTERRUPTSOURCE_EXIT_P(r,i);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserEnableInterruptSource</name>
  <synopsis>Enable the Specified Interrupt Source </synopsis>
  <syntax>
    os_result_t OS_UserEnableInterruptSource(os_isrid_t)
  </syntax>
  <description>
    <code>OS_UserEnableInterruptSource()</code> enables the specified
    interrupt source.
    </description>
  <availability>
    No restrictions.
  </availability>
  <returns>E_OS_OK = success</returns>
  <returns>E_OS_ID = the isr id was invalid</returns>
  <returns>OS_E_ACCESS = the application is not accessible</returns>
  <returns>OS_E_CORE = the core has been shut down, which is responsible for the ISR</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
