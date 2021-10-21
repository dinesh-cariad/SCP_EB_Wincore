/* kern-erroraction.c
 *
 * This file contains the OS_ErrorAction function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-erroraction.c 29605 2018-12-11 09:28:41Z magu869 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 * Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 * Function OS_ErrorActionTask is called indirectly by OS_Error and OS_ErrorAction.
 * These functions provide different reactions for detected errors, including
 * system shutdown or the call of a hook function. The Implementation of OS_Error and
 * OS_ErrorAction ensures that no recursion is possible.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason:
 * Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: SetButNeverUsed
 *   Dead assignment.
 *
 * Reason:
 * Not an issue, assignment is effective depending on OS error handling level.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if there is at least one Cat-2 ISR.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if there is an ErrorHook.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*
 * The following table shows the action that will in fact be taken
 * for any given context/action pair. The abbreviations used are mostly
 * be obvious, those that aren't are
 *   QU = quarantine
 *   S'DOWN = shutdown
 *   SD-NH = shutdown-no-hooks
 *
 * "Kill" means kill the offending task, isr or application-specific hook.
 * "Quarantine" means kill the offending task or ISR. For application-specific
 * hooks the whole app must be quarantined.
 *
 * The (APP) lines under the startup, shutdown and error hook lines are for
 * application-specific hooks, which can be individually killed.
 * Calling Shutdown for an error caused in a shutdown hook would cause the
 * faulty hook function to be called again. So for the global hook, shutdown
 * with no hooks is called. For application-specific hooks, we simply kill
 * the hook function in (almost) all cases because the system is shutting
 * down anyway.
 *
 *         |  Action required (from error db or return from ProtectionHook)
 * inFunc. | IGNORE | RETURN | KILL   | QU.    | QU.APP | REST.  | S'DOWN
 * =============================================================================
 *         |        |        |        |        |        |        |
 * BOOT    | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * TASK    | IGNORE | RETURN | KILL   | QU.    | QU.APP | RSTART | S'DOWN
 *         |        |        |        |        |        |        |
 * CAT1    | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * CAT2    | IGNORE | RETURN | KILL   | QU.    | QU.APP | RSTART | S'DOWN
 *         |        |        |        |        |        |        |
 * ACB     | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * S'DOWN  | IGNORE | RETURN | RETURN | RETURN | RETURN | RETURN | RETURN
 *         |        |        |        |        |        |        |
 * INTERN  | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * SU.HOOK | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 * (APP)   | IGNORE | RETURN | KILL   | QU.    | QU.APP | RSTART | S'DOWN
 *         |        |        |        |        |        |        |
 * SD.HOOK | IGNORE | RETURN | SD-NH  | SD-NH  | SD-NH  | SD-NH  | SD-NH
 * (APP)   | IGNORE | RETURN | KILL   | KILL   | KILL   | KILL   | KILL
 *         |        |        |        |        |        |        |
 * E.HOOK  | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 * (APP)   | IGNORE | RETURN | KILL   | KILL   | QU.APP | RSTART | S'DOWN
 *         |        |        |        |        |        |        |
 * PRETASK | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * POSTTSK | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * PREISR  | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 * POSTISR | IGNORE | RETURN | S'DOWN | S'DOWN | S'DOWN | S'DOWN | S'DOWN
 *         |        |        |        |        |        |        |
 *
 * Some of the functionality is in smaller helper functions to avoid getting
 * a huge cyclomatic complexity figure or Halstead length for this function.
 *
 * WARNING: these functions will not return to the caller in some circumstances.
*/

/* prototypes for helper functions */
static void OS_ErrorActionTask(os_erroraction_t act, os_result_t res);
static void OS_ErrorActionCat2(os_erroraction_t act, os_result_t res);
static void OS_ErrorActionStartupHook(os_erroraction_t act, os_result_t res);
static void OS_ErrorActionShutdownHook(void);
static void OS_ErrorActionErrorHook(os_erroraction_t act, os_result_t res);

/* OS_ErrorActionTask - handle non-trivial error actions for a task
*/
/* Deviation MISRAC2012-1 */
static void OS_ErrorActionTask(os_erroraction_t act, os_result_t res)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( (act == OS_ACTION_KILL) ||
		 (act == OS_ACTION_QUARANTINE) )
	{
		/* The error occurred in a task, so we kill that
		 * task. It can be restarted later if the action is KILL.
		*/
		/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATETASKISR, 1
		*/
		(*OS_killTaskFunc)(kernel_data->taskCurrent,
			(act==OS_ACTION_KILL) ? OS_TS_SUSPENDED
								  : OS_TS_QUARANTINED);
	}
	else
	if ( (act == OS_ACTION_QUARANTINEAPP) ||
		 (act == OS_ACTION_RESTART) )
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_GET_APP(kernel_data->taskCurrent->app);

		if ( OS_AppIsNull(app) || OS_AppIsSystemApp(app) )
		{
			/* The application cannot be determined or is a system application.
			 * The only thing we can do is shut down the OS.
			 *
			 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.UnknownApplication, 1
			*/
			OS_SHUTDOWN(res, OS_FALSE);
		}
		else
		{
			/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL, 1
			*/
			OS_QUARANTINEAPPLICATION(app);

			if ( act == OS_ACTION_RESTART )
			{
				/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL_RESTART, 1
				*/
				OS_RESTARTAPPLICATION(app);
			}
		}
	}
	else
	{
		/* Action is either SHUTDOWN or invalid.
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.SHUTDOWN, 1
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
}

/* OS_ErrorActionCat2 - handle non-trivial error actions for a category 2 ISR
*/
static void OS_ErrorActionCat2(os_erroraction_t act, os_result_t res)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_appcontext_t *app;
	os_result_t killresult = OS_E_INTERNAL;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( (act == OS_ACTION_KILL) ||
		 (act == OS_ACTION_QUARANTINE) )
	{
		/* We attempt to kill the ISR, after disabling its
		 * source in the QUARANTINE case.
		*/
		/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATETASKISR, 1
		*/
		if ( act==OS_ACTION_QUARANTINE )
		{
			OS_DisableIsr(&OS_isrTableBase[kernel_data->isrCurrent]);
		}

		if ( OS_killIsrFunc != OS_NULL )
		{
			killresult = (*OS_killIsrFunc)(kernel_data->isrCurrent);
		}

		if ( killresult != OS_E_OK )
		{
			/* If the ISR cannot be killed for some reason we
			 * must shut down
			 *
			 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
			*/
			OS_SHUTDOWN(res, OS_FALSE);
		}
	}
	else
	if ( (act == OS_ACTION_QUARANTINEAPP) ||
		 (act == OS_ACTION_RESTART) )
	{
		/* Possible diagnostic TOOLDIAG-2 <1> */
		app = OS_GET_APP(OS_isrTableBase[kernel_data->isrCurrent].app);

		if ( OS_AppIsNull(app) || OS_AppIsSystemApp(app) )
		{
			/* The application cannot be determined or is a system application.
			 * The only thing we can do is shut down the OS.
			 *
			 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.UnknownApplication, 1
			*/
			OS_SHUTDOWN(res, OS_FALSE);
		}
		else
		{
			/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL, 1
			*/
			OS_QUARANTINEAPPLICATION(app);

			if ( act == OS_ACTION_RESTART )
			{
				/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL_RESTART, 1
				*/
				OS_RESTARTAPPLICATION(app);
			}

			if ( OS_killIsrFunc != OS_NULL )
			{
				killresult = (*OS_killIsrFunc)(kernel_data->isrCurrent);
			}

			if ( killresult != OS_E_OK )
			{
				/* If the ISR cannot be killed for some reason we
				 * must shut down
				 *
				 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
				*/
				OS_SHUTDOWN(res, OS_FALSE);
			}
		}
	}
	else
	{
		/* Action is either SHUTDOWN or invalid.
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.SHUTDOWN, 1
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
}

/* OS_ErrorActionStartupHook - handle non-trivial error actions for a startup hook
*/
static void OS_ErrorActionStartupHook(os_erroraction_t act, os_result_t res)
{
	os_result_t killresult = OS_E_INTERNAL;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_AppIsNull(kernel_data->hookApp) || OS_AppIsSystemApp(kernel_data->hookApp) )
	{
		/* We're in the global startup hook.
		 * The only possible action is Shutdown.
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
	else
	if ( (act == OS_ACTION_KILL) ||
		 (act == OS_ACTION_QUARANTINE) ||
		 (act == OS_ACTION_QUARANTINEAPP) ||
		 (act == OS_ACTION_RESTART) )
	{
		/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.UnknownTask, 1
		 *
		 * We are unable to identify a task/ISR here because it wasn't a task that caused the problem!
		*/
		/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL, 1
		*/
		OS_QUARANTINEAPPLICATION(kernel_data->hookApp);

		if ( act == OS_ACTION_RESTART )
		{
			/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.TERMINATEAPPL_RESTART, 1
			*/
			OS_RESTARTAPPLICATION(kernel_data->hookApp);
		}

		if ( OS_killHookFunc != OS_NULL )
		{
			killresult = (*OS_killHookFunc)(&kernel_data->sHookContext);
		}

		if ( killresult != OS_E_OK )
		{
			/* If the hook cannot be killed for some reason we
			 * must shut down
			 *
			 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
			*/
			OS_SHUTDOWN(res, OS_FALSE);
		}
	}
	else
	{
		/* Action is either SHUTDOWN or invalid.
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.SHUTDOWN, 1
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
}

/* OS_ErrorActionShutdownHook - handle non-trivial error actions for a shutdown hook
*/
static void OS_ErrorActionShutdownHook(void)
{
	os_result_t killresult = OS_E_INTERNAL;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_AppIsNull(kernel_data->hookApp) || OS_AppIsSystemApp(kernel_data->hookApp) )
	{
		/* We're in the global shutdown hook.
		 * The only possible action is Shutdown without hooks.
		*/
		OS_ShutdownNoHooks();
	}
	else
	{
		/* In an application-specific shutdown hook we simply kill the
		 * offending hook in all circumstances. This ensures that
		 * the shutdown will proceed as normal.
		 * QUARANTINE(_APP) and RESTART are meaningless in this context.
		*/
		if ( OS_killHookFunc != OS_NULL )
		{
			killresult = (*OS_killHookFunc)(&kernel_data->sHookContext);
		}

		if ( killresult != OS_E_OK )
		{
			/* If the hook cannot be killed for some reason we
			 * must shut down without calling any further hooks.
			 *
			 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
			*/
			OS_ShutdownNoHooks();
		}
	}
}

/* OS_ErrorActionErrorHook - handle non-trivial error actions for an error hook
*/
static void OS_ErrorActionErrorHook(os_erroraction_t act, os_result_t res)
{
	os_result_t killresult = OS_E_INTERNAL;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( OS_AppIsNull(kernel_data->hookApp) || OS_AppIsSystemApp(kernel_data->hookApp) )
	{
		/* We're in the global error hook.
		 * The only possible action is Shutdown.
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
	else
	if ( (act == OS_ACTION_KILL) ||
		 (act == OS_ACTION_QUARANTINE) ||
		 (act == OS_ACTION_QUARANTINEAPP) ||
		 (act == OS_ACTION_RESTART) )
	{
		/* Application-specific error hook.
		 *
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.UnknownTask, 1
		 *
		 * We are unable to identify a task/ISR here because it wasn't a task that caused the problem!
		 * We can achieve the desired results by forcing either QUARANTINEAPP or RESTART in the outer
		 * context. Then we just kill the error hook.
		*/
		kernel_data->errorStatus.action = (act == OS_ACTION_RESTART) ? OS_ACTION_RESTART : OS_ACTION_QUARANTINEAPP;

		if ( OS_killHookFunc != OS_NULL )
		{
			killresult = (*OS_killHookFunc)(&kernel_data->eHookContext);
		}

		if ( killresult != OS_E_OK )
		{
			/* If the hook cannot be killed for some reason we
			 * must shut down
			 *
			 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
			*/
			OS_SHUTDOWN(res, OS_FALSE);
		}
	}
	else
	{
		/* Action is either SHUTDOWN or invalid.
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.SHUTDOWN, 1
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}
}


/*
 * OS_ErrorAction() - do what is required after reporting an error
 *
 * This function performs the trivial error actions itself. The more complicated
 * actions are passed on to helper functions.
*/
os_result_t OS_ErrorAction(os_erroraction_t act, os_result_t res)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if ( act == OS_ACTION_IGNORE )
	{
		/* Ignore totally - don't even tell caller
		*/
		res = OS_E_OK;
	}
	else
	if ( act == OS_ACTION_RETURN )
	{
		/* Return error code to caller.
		*/
	}
	else
	if ( kernel_data->inFunction == OS_INSHUTDOWN )
	{
		/* This case should never happen, but just in case it does,
		 * we simply return to the existing shutdown state no matter
		 * what the error is.
		*/
	}
	else
	if ( kernel_data->inFunction == OS_INTASK )
	{
		OS_ErrorActionTask(act, res);
	}
	else
	if ( OS_IS_INCAT2(kernel_data->inFunction) )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		OS_ErrorActionCat2(act, res);
	}
	else
	if ( OS_IS_INSTARTUPHOOK(kernel_data->inFunction) )
	{
		OS_ErrorActionStartupHook(act, res);
	}
	else
	if ( OS_IS_INSHUTDOWNHOOK(kernel_data->inFunction) )
	{
		OS_ErrorActionShutdownHook();
	}
	else
	if ( OS_IS_INERRORHOOK(kernel_data->inFunction) )
	{
		/* Possible diagnostic TOOLDIAG-4 <+1> */
		OS_ErrorActionErrorHook(act, res);
	}
	else
	{
		/* BOOT, CAT1, ACB, INTERN,
		 * PRETASK, POSTTASK, PREISR, POSTISR
		 *
		 * None of the above contexts can be individually killed, so
		 * the only response is to shut down the system.
		 *
		 * !LINKSTO Kernel.API.OSControl.ShutdownOS.InvalidState, 1
		 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.SHUTDOWN, 1
		*/
		OS_SHUTDOWN(res, OS_FALSE);
	}

	/* Don't put any code here - it is not guaranteed to be executed.
	*/
	return res;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
