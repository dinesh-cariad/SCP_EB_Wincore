/* kern-shutdownallcores.c
 *
 * This file contains the OS_KernShutdownAllCores() function.
 *
 * $Id: kern-shutdownallcores.c 27928 2017-11-21 18:58:04Z mist9353 $
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
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

#define OS_SID	OS_SID_ShutdownAllCores
#define OS_SIF	OS_svc_ShutdownAllCores

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernShutdownAllCores
 *
 * This function implements the API ShutdownAllCores().
 * If the call is permitted, all other cores are informed.
 * Then the calling core shuts down, too.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.ShutdownAllCores, 1
*/
void OS_KernShutdownAllCores(os_result_t code)
{
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)code);

	OS_TRACE_SHUTDOWNALLCORES_ENTRY(code);

	if ( !OS_CallingContextCheck() )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void)OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void)OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
		}
	}
	else
	{
		os_appcontext_t const* const app = OS_CurrentApp();

		/* !LINKSTO Kernel.Autosar.ServiceErrors.Miscellaneous.Shutdown.Nontrusted, 1
		*/
		if ( (OS_AppIsNull(app)) || ( (app->flags & OS_APP_TRUSTED) != 0 ) )
		{
			os_paramtype_t msgParam[OS_MAXPARAM];
			os_coreid_t const myCoreId = OS_GetMyCoreId();
			os_coreid_t core;

			OS_GetKernelData()->inFunction = OS_ININTERNAL;

			/* !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
			 *
			 * Cross-core interrupts cannot happen here either.
			*/
			(void)OS_IntDisable();

			/* Send shutdown message to all cores. No answer is expected.
			*/
			for (core = 0; core < OS_N_CORES_MAX; core++)
			{
				if (OS_CoreIsInUse(core) && (core != myCoreId))
				{
					msgParam[0] = code;
					OS_SendMessage(core, OS_XC_OP_ShutdownCore, OS_NULL, msgParam);
				}
			}

			/* Now shut down myself. This is a synchronized shutdown, so the second parameter to
			 * OS_Shutdown() is non-zero
			*/
			OS_SHUTDOWN(code, OS_TRUE);
		}
		else
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.ShutdownAllCores.NonTrusted, 1
			*/
			/* Possible diagnostic TOOLDIAG-1 <+1> */
			(void)OS_ERROR(OS_ERROR_NotTrusted, OS_GET_PARAMETER_VAR());
		}
		else
		{
			/* MISRA-C */
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
