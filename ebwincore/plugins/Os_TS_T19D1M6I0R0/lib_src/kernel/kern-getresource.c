/* kern-getresource.c
 *
 * This file contains the OS_KernGetResource function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getresource.c 27928 2017-11-21 18:58:04Z mist9353 $
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
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if there are Cat-2 ISRs.
*/

#define OS_SID	OS_SID_GetResource
#define OS_SIF	OS_svc_GetResource

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_KernGetResource
 *
 * This function implements the GetResource system service in the kernel.
 *
 * !LINKSTO Kernel.API.ResourceManagement.GetResource.API, 1
*/
os_result_t OS_KernGetResource(os_resourceid_t r)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_result_t result = OS_E_OK;
	os_resourcedynamic_t *rd;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)r);

	OS_TRACE_GETRESOURCE_ENTRY(r);

	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		result = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheckAlways() )
	{
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		result = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_IsValidResourceId( r ) )
	{
		/* !LINKSTO Kernel.API.ResourceManagement.GetResource.InvalidResource, 1
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		result = OS_ERROR(OS_ERROR_InvalidResourceId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		rd = OS_ResourceGetDynamic(r);

		/* No need to disable interrupts here, because the resource cannot
		 * get taken (and remain taken [*]) by anything that interrupts us.
		 *
		 * [*] A category 2 ISR could take the resource, but must release it
		 * again before returning. Failure to do so is a serious error that
		 * ought to be detected elsewhere.
		*/
		if ( OS_ResourceInUse(rd) )
		{
			/* !LINKSTO Kernel.API.ResourceManagement.GetResource.FixedResource, 1
			*/
			result = OS_ERROR(OS_ERROR_ResourceInUse, OS_GET_PARAMETER_VAR());
		}
		else
		if ( kernel_data->inFunction == OS_INTASK )
		{
			result = OS_GetResourceFromTask(r, rd, OS_GET_PARAMETER_VAR());
		}
		else
		if ( OS_IS_INCAT2( kernel_data->inFunction ) )
		{
			/* Possible diagnostic TOOLDIAG-4 <+1> */
			result = OS_GetResourceFromIsr(r, rd, OS_GET_PARAMETER_VAR());
		}
		else
		{
			/* MISRA */
		}
	}

	OS_TRACE_GETRESOURCE_EXIT_P(r,r);

	return result;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetResource</name>
  <synopsis>Enter a critical section</synopsis>
  <syntax>
    os_result_t OS_UserGetResource
    (   os_resourceid_t r
    )
  </syntax>
  <description>
    <code>OS_UserGetResource()</code> allows the calling task to enter
    a critical section of code associated with the resource. Other tasks
    that use the same resource must wait until this task releases the
    resource again.
    <para>A task may not call <code>OS_UserGetResource()</code> for a
    resource that it already holds.</para>
  </description>
  <availability>
    <code>OS_UserGetResource()</code> may be used in tasks. On some
    architectures <code>OS_UserGetResource()</code> can be called
    from Category 2 ISRs as well.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
