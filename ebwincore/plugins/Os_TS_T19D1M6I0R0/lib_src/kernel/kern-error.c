/* kern-error.c
 *
 * This file contains the OS_Error function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-error.c 29605 2018-12-11 09:28:41Z magu869 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 17.2 (required)
 * Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 * Function OS_Error is called indirectly by OS_CallErrorHook and OS_ErrorAction.
 * These functions provide different reactions for detected errors, including
 * system shutdown or the call of a hook function. The Implementation of OS_Error and
 * OS_ErrorAction ensures that no recursion is possible.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Dead assignment to "act" eliminated.
 *
 * Reason: Not an issue, variable will be used if error actions aren't optimized away
 *   (OS_EXCLUDE_ERRORACTION).
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: Not an issue. Only if the protection hook is excluded (OS_EXCLUDE_PROTECTIONHOOK),
 *   this comparison can be evaluated at compile time.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  If there are neither ErrorHooks nor ProtectionHooks,
 *  the respective if-condition can be evaluated at compile time.
 *
 * TOOLDIAG-4) Possible diagnostic: CompilerCouldNotEstablishStackProtection
 *   SSP - in function 'OS_Error', variable 'esSave' is still vulnerable
 *         after stack reordering because of its struct layout
 *
 * Reason:
 *  The compiler notices that os_errorstatus_t contains an array.
 *  It cannot add canaries to detect accesses which are out of bounds, because this is a struct.
 *  As a consequence the fields of this variable are not protected by the compilers mechanism.
 *  This is not an issue.
 *  This variable is not written by any user input.
 *  Out of bound accesses are prevented by AutoCore OS quality measures.
 *  In this particular example the array in 'esSave' is written via struct assignment,
 *  which can only cause out of bound accesses if the compiler is defective.
*/

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_Error
 *
 * This function is called whenever an error condition is detected.
 * It looks up the calling function id and the error condition in the
 * error tables, and takes appropriate action.
 * Finally, it returns an appropriate error code (OS_E_XXX)
 * to the caller.
 *
 * Each element of the OS_serviceErrors array is a pointer to
 * another array containing a list of error entries. Each list is
 * terminated with an entry containing an invalid error as the
 * error condition. An error entry tells the error handler what
 * to do and what to return as a result of this error. The invalid
 * error entry tells what to do for an unexpected error.
 *
 * The OS_serviceErrors entry for a service that has no error
 * conditions simply points to an invalid error entry.
 *
 * The action taken after the error has been reported comes initially
 * from the error tables, and can be modified by the hook functions
 * with write access to the kernel_data->errorStatus structure. Furthermore,
 * the return value of ProtectionHookcan modifies the action. Finally,
 * the action taken depends on exactly where the error occurred;
 * for example an action to kill a task or ISR is meaningless if the
 * error occurred in an alarm callback routine. The local function
 * ErrorAction() decides on and takes the corrective action.
 *
 * !LINKSTO Kernel.API.Hooks.ErrorHook, 1
 *
*/
/* Possible diagnostic TOOLDIAG-4 <START> */
/* Deviation MISRAC2012-1 */
os_result_t OS_Error
(	const os_serviceinfo_t * const s,	/* Infomration about the calling service */
	os_error_t e,						/* The error code */
	os_paramtype_t *p					/* The parameters to the service, OS_NULL if none */
)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_result_t res;
	os_erroraction_t act;
	os_erroraction_t hooks;
	os_uint8_t ehNestingSave;
	const os_appcontext_t *hookAppSave;
	os_errorstatus_t esSave;
	const os_errorentry_t *ep;
	const os_appcontext_t *app;
	os_intstatus_t is;
	os_uint8_t en;

	/* Determine the application of the "caller" (whatever caused the error) so that we know which
	 * application's hook function to call.
	*/
	app = OS_CurrentApp();

	ep = s->errorTable;
	en = s->nErrors;

	while ( (en > 0) && (ep->condition != e) )
	{
		ep++;
		en--;
	}

	if ( (ep->condition == e) )
	{
		act = ep->action;
		res = ep->result;
	}
	else
	{
		act = OS_ACTION_SHUTDOWN|OS_ACTION_ERRORHOOK;
		res = OS_E_INTERNAL;
	}

	hooks = (os_erroraction_t) (act & OS_hookSelection);
	act &= OS_ACTION_MASK;

	/* Check if any hook calls are needed.
	 * Recursive calls to error hook function are prevented.
	*/
	if ( OS_IS_ACTION_HOOK(hooks) )
	{
		/* !LINKSTO Kernel.API.Hooks.ErrorHook.API, 1
		 * !LINKSTO Kernel.API.Hooks.ErrorHook.Error, 1
		*/
		/* Remember where we're called from and which error hooks are nested
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		ehNestingSave = kernel_data->errorHookNesting;
		esSave = kernel_data->errorStatus;
		hookAppSave = kernel_data->hookApp;

		/* Hooks are always called with interrupts disabled. Among
		 * other things, this assures the integrity of the error status
		 * information
		 *
		 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
		*/
		is = OS_IntDisableConditional();

		/* Make the extended error information available to the
		 * error hook via the OS_GetExtendedErrorStatus() function.
		*/
		kernel_data->errorStatus.calledFrom = kernel_data->inFunction;
		kernel_data->errorStatus.serviceId = s->sid;

		if ( p != OS_NULL )
		{
			/* !LINKSTO Kernel.API.Hooks.Macros, 1
			*/
			os_int_t i;

			for ( i = 0; i < OS_MAXPARAM; i++ )
			{
				kernel_data->errorStatus.parameter[i] = p[i];
			}
		}

		kernel_data->errorStatus.errorCondition = e;
		kernel_data->errorStatus.action = act;
		kernel_data->errorStatus.result = res;

		/* Normally there will be only one hook function set for each
		 * kind of error, but we handle the possibility that more might
		 * be set in the future
		 * UPDATE 21.7.2005 this came true in Autosar 1.5 ;-)
		 * And the order changed - now we must call the global error hook
		 * first - so we must test for no app-hook separately.
		*/
		if ( (hooks & OS_ACTION_ERRORHOOK_APP) != 0 )
		{
			/* The application's own error hook should be called.
			 * If the application cannot be determined or the application
			 * has no hook function, the global error hook is called instead.
			 *
			 * We don't call the application hook if we're already nested
			 * inside any error hook at all.
			*/
			if ( (OS_AppIsNull(app)) ||
				 (app->errorHook == OS_NULL) ||
				 (kernel_data->errorHookNesting != 0) )
			{
				hooks |= OS_ACTION_ERRORHOOK;
			}
		}

		/* !LINKSTO Kernel.API.Hooks.ErrorHook.StatusType, 1
		 *
		 * We don't call the error hook function if we're already
		 * inside the error hook somewhere.
		 * This can also happen if a category 1 ISR interrupts the
		 * error hook and then itself causes an error.
		*/
		if ( ((hooks & (OS_ACTION_ERRORHOOK | OS_ACTION_ERRORHOOK_EB)) != 0) &&
			 ((kernel_data->errorHookNesting & OS_EHN_ERR) == 0) )
		{
			kernel_data->errorHookNesting = (os_uint8_t) (ehNestingSave | OS_EHN_ERR);
			kernel_data->hookApp = OS_NULL;
			OS_CALLERRORHOOK(res);
			kernel_data->errorHookNesting = ehNestingSave;
		}

		if ( (hooks & OS_ACTION_ERRORHOOK_APP) != 0 )
		{
			/* !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.ErrorHook.Call, 1
			 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.ErrorHook.Order, 1
			 * Call the application's own error hook if there is one.
			 * If the application cannot be determined or the application
			 * has no hook function, the global error hook is called instead.
			 * At the moment, a nontrusted app can't have an error hook.
			 *
			 * We don't call the application hook if we're already nested
			 * inside any error hook at all.
			*/
			if ( (OS_AppIsNull(app)) ||
				 (kernel_data->appsStarted == 0) ||
				 (app->errorHook == OS_NULL) ||
				 (kernel_data->errorHookNesting != 0) )
			{
				/* Use No Hooks! */
			}
			else
			{
				/* !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.ErrorHook.AccessRights, 1
				 * Call application's error hook
				*/
				kernel_data->errorHookNesting = (os_uint8_t) (ehNestingSave | OS_EHN_APP);
				kernel_data->hookApp = app;
				OS_CallErrorHook(app, res);
				kernel_data->errorHookNesting = ehNestingSave;
			}
		}

		/*
		 * We don't call the protection hook function if we're already
		 * inside the protection hook somewhere.
		 * This can also happen if a category 1 ISR interrupts the
		 * protection hook and then itself causes an protection fault.
		*/
		if ( ((hooks & OS_ACTION_PROTECTIONHOOK) != 0) &&
			 ((kernel_data->errorHookNesting & OS_EHN_PROT) == 0) )
		{
			/* !LINKSTO Kernel.Autosar.Protection.ProtectionHook, 1
			 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.Call, 1
			*/
			kernel_data->errorHookNesting = (os_uint8_t) (ehNestingSave | OS_EHN_PROT);
			kernel_data->hookApp = OS_NULL;
			OS_CALLPROTECTIONHOOK(res, act);
			kernel_data->errorHookNesting = ehNestingSave;

			/* If the return value is legal, we store it into the global
			 * location. Otherwise we leave the global location as it is.
			 * This permits full AUTOSAR conformance while at the same time
			 * permitting the ProtectionHook to return non-AUTOSAR
			 * values by writing to the error-status and then returning
			 * an illegal value.
			 *
			*/
			/* Possible diagnostic TOOLDIAG-2 <1> */
			if ( act == OS_ACTION_DEFAULT )
			{
				/* The action in kernel_data->errorStatus (which is the default from the
				 * error table unless the ProtectionHook has changed it directly) is the
				 * action we take. This is an EB extended feature.
				*/
			}
			else
			/* Possible diagnostic TOOLDIAG-2 <1> */
			if ( (act == OS_ACTION_DONOTHING) && (res == OS_E_RATEPROT) )
			{
				/* This is the Autosar SWS "Ignore" return value (PRO_IGNORE).
				 * The semantics are unclear.
				 *
				 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.DoNothing, 1
				 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.ReturnValue.IGNORE, 1
				*/
				kernel_data->errorStatus.action = OS_ACTION_RETURN;
			}
			/* Possible diagnostic TOOLDIAG-2 <2> */
			else
			if ( OS_ActionIsValid(act) )
			{
				kernel_data->errorStatus.action = act;
			}
			else
			{
				kernel_data->errorStatus.action = OS_ACTION_SHUTDOWN;
			}
		}

		/* Local copies of act and res reloaded here before re-enabling
		 * interrupts
		*/
		/* Possible diagnostic TOOLDIAG-1 <1> */
		act = kernel_data->errorStatus.action;
		res = kernel_data->errorStatus.result;

		OS_IntRestore(is);

		/* Restore saved states of these variables
		 * kernel_data->errorHookNesting was restored each time it was modified,
		 * so no need to restore it again here.
		*/
		kernel_data->hookApp = hookAppSave;
		kernel_data->errorStatus = esSave;
	}

	/* Now do what we're supposed to do to the perpetrator.
	 * WARNING: OS_ERRORACTION might not return under some circumstances.
	*/
	return OS_ERRORACTION(act, res);
}
/* Possible diagnostic TOOLDIAG-4 <STOP> */

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
