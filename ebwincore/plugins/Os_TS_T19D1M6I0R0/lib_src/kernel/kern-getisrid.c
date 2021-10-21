/* kern-getisrid.c
 *
 * This file contains the OS_GetIsrId function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getisrid.c 27927 2017-11-21 18:08:15Z mist9353 $
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
 *   (In addition to this calling context checks or interrupt enable checks have to be enabled.)
*/

#define OS_SID	OS_SID_GetIsrId
#define OS_SIF	OS_svc_GetIsrId

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernGetIsrId implements the API GetIsrId
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetISRID, 2
*/
os_isrid_t OS_KernGetIsrId(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_isrid_t i = OS_NULLISR;

	OS_TRACE_GETISRID_ENTRY();

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
		i = kernel_data->isrCurrent;
	}

	OS_TRACE_GETISRID_EXIT_P(i);
	return i;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetIsrId</name>
  <synopsis>Return the id of the current ISR</synopsis>
  <syntax>
    os_isrid_t OS_UserGetIsrId(void)
  </syntax>
  <description>
    If <code>OS_UserGetIsrId()</code> is called from an ISR of category
    category 2, or from an ErrorHook or ProtectionHook caused by an ISR
    of category 2, it returns the ID of the ISR. Otherwise it returns
    OS_NULLISR.
    <para>If the more relaxed (but not Autosar-conformant) calling context checks are configured,
          the ISR ID is also returned when called from a category 1 ISR or from an alarm
          callback function.
    </para>
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>IsrId=Success</returns>
  <returns>OS_NULLISR=Not called from an ISR.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
