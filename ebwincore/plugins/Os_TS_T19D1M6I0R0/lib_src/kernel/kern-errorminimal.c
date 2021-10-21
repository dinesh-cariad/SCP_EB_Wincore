/* kern-errorminimal.c
 *
 * This file contains the OS_ErrorMinimal function, which is the
 * equivalent of OS_Error for the minimal error handling (with ErrorHook) case.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-errorminimal.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ErrorMinimal
 *
 * This function is potentially called whenever an error condition is detected.
 * It calls the error hook, then returns the error code to the caller.
 *
 * This version of the function does not support OSErrorGetServiceId() nor OSError_<service>_<param>
 * Nor does it support the other hook types (ProtectionHook, application-defined ErrorHooks).
 *
 * !LINKSTO Kernel.API.Hooks.ErrorHook, 1
 *
*/
os_result_t OS_ErrorMinimal
(	os_result_t e						/* The OSEKified error code */
)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_uint8_t oldInFunction;
	os_intstatus_t is;

	if ( kernel_data->errorHookNesting == 0 )
	{
		is = OS_IntDisable();

		kernel_data->errorHookNesting = 1;
		oldInFunction = kernel_data->inFunction;
		kernel_data->inFunction = OS_INERRORHOOK;
		OS_TRACE_HOOK_ENTRY(OS_INERRORHOOK);

		ErrorHook(e);

		OS_TRACE_HOOK_EXIT_P(OS_INERRORHOOK);
		kernel_data->inFunction = oldInFunction;
		kernel_data->errorHookNesting = 0;

		OS_IntRestore(is);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
