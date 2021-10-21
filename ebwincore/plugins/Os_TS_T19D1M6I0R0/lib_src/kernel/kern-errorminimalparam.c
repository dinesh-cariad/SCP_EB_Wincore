/* kern-errorminimalparam.c
 *
 * This file contains the OS_ErrorMinimalParam function, which is the
 * equivalent of OS_Error for the minimal error handling (with ErrorHook and Parameter Access) case.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-errorminimalparam.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_ErrorMinimalParam
 *
 * This function is potentially called whenever an error condition is detected.
 * It calls the error hook, then returns the error code to the caller.
 * OSErrorGetServiceId() and OSError_<service>_<param> are supported.
 *
 * This version of the error handling does not support the other hook types
 * (ProtectionHook, application-defined ErrorHooks).
 *
 * !LINKSTO Kernel.API.Hooks.ErrorHook, 1
 *
*/
os_result_t OS_ErrorMinimalParam
(	os_serviceid_t sid,					/* The service ID (OS_SID_xxx) */
	os_result_t e,						/* The OSEKified error code */
	os_paramtype_t *p					/* The parameters to the service, OS_NULL if none */
)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_uint8_t oldInFunction;
	os_intstatus_t is;
	os_int_t i;

	if ( kernel_data->errorHookNesting == 0 )
	{
		is = OS_IntDisable();

		kernel_data->errorHookNesting = 1;
		oldInFunction = kernel_data->inFunction;
		kernel_data->inFunction = OS_INERRORHOOK;

		kernel_data->errorStatus.result = e;
		kernel_data->errorStatus.action = OS_ACTION_RETURN;
		kernel_data->errorStatus.calledFrom = oldInFunction;
		kernel_data->errorStatus.serviceId = sid;
		kernel_data->errorStatus.errorCondition = OS_ERROR_UnknownError;

		if ( p != OS_NULL )
		{
			for ( i = 0; i < OS_MAXPARAM; i++ )
			{
				kernel_data->errorStatus.parameter[i] = p[i];
			}
		}
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
