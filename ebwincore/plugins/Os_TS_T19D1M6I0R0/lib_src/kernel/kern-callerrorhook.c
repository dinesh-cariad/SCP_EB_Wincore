/* kern-callerrorhook.c
 *
 * This file contains the application-specific error hook wrapper.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-callerrorhook.c 30165 2019-04-16 10:56:42Z hagr271648 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer
 * to object.
 *
 * Reason:
 * The type conversion is necessary to have byte-precise calculation of pointer
 * differences.
 *
 * MISRAC2012-2) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * This kind of cast is necessary to determine the amount of free/used space
 * on the stack. Furthermore, different architectures return information about
 * their stacks differently, so this cast serves as an unificator.
 *
 * MISRAC2012-3) Deviated Rule: 18.4 (advisory)
 * The +, -, += and -= operators should not be applied to an expression of
 * pointer type.
 *
 * Reason:
 * Pointer arithmetic is required and in this context more natural than
 * array indexing.
 *
 * MISRAC2012-4) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and any
 * other type.
 *
 * Reason:
 * The function OS_CORTEXM_Call() is used for different types of functions.
 * Hence, a common type must be used in its declaration. In this software,
 * this common type (os_vfuncu_t) is compatible with all those, which are
 * converted to it.
 *
 * MISRAC2012-5) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different
 * essential type category or a wider essential type.
 *
 * Reason:
 * The cast is necessary, because the length of a stack is stored w/o a sign,
 * but the calculation of remaining stack space needs one.
 *
 * MISRAC2012-6) Deviated Rule: 10.7 (required)
 * If a composite expression is used as one operand of an operator in which the
 * usual arithmetic conversions are performed then the other operand shall not
 * have wider essential type.
 *
 * Reason:
 * This is a false positive, because there are type casts before the subtraction
 * is performed to determine the remaining kernel stack. This means, both
 * operands have identical types. Furthermore, the given length argument is
 * converted to os_ptrdiff_t before the comparison. This type
 * has sufficient precision to represent pointer differences. Hence, an object
 * of type ptrdiff_t is compared with another object of type os_ptrdiff_t.
 *
 * MISRAC2012-7) Deviated Rule: 17.2 (required)
 * Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 * Function OS_CallErrorHook is called indirectly by OS_Error and OS_ErrorAction.
 * These functions provide different reactions for detected errors, including
 * system shutdown or the call of a hook function. The Implementation of OS_Error and
 * OS_ErrorAction ensures that no recursion is possible.
 *
 * MISRAC2012-8) Deviated Rule: 17.2 (required)
 *   Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 *   OS_HookRestoreMemprot() can call OS_Panic() indirectly.
 *   OS_Panic() detects recursive calls by means of the core-local variable
 *   panicCode. If OS_Panic() is entered a second time, it calls
 *   OS_ShutdownNoHooks() which doesn't call OS_Panic() again. Instead, it
 *   stops the system, and, therefore, the maximum nesting level is two calls.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if stack checks are not excluded, see OS_EXCLUDE_STACKCHECK.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  Depending on whether application error hooks are called directly,
 *  the respective branch is either reachable or unreachable.
*/

#define OS_SID	OS_SID_HookHandler
#define OS_SIF	OS_svc_HookHandler

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>


/* Restore memory protection settings of the calling context.
 *
 * Parameters:
 * kernel_data		pointer to the current kernel_data
 * app				pointer to the application of the ErrorHook that just executed
 * oldApp			pointer to the old hook application.
 * oldInFunction	Previously running kernel function.
*/
static void OS_HookRestoreMemprot
(
	os_kerneldata_t *,
	const os_appcontext_t *,
	const os_appcontext_t *,
	os_uint8_t
);

/* Deviation MISRAC2012-8 <1> */
static void OS_HookRestoreMemprot
(
	os_kerneldata_t *kernel_data,
	const os_appcontext_t *app,
	const os_appcontext_t *oldApp,
	os_uint8_t oldInFunction
)
{
	if ( (app->flags & OS_APP_TRUSTED) == 0U )
	{
		switch ( oldInFunction )
		{
		case OS_INCAT2:
			if ( kernel_data->isrCurrent < OS_nInterrupts )
			{
				OS_SETISRPROTECTION(
						&OS_isrTableBase[kernel_data->isrCurrent],
						OS_isrTableBase[kernel_data->isrCurrent].dynamic);
			}
			break;

		case OS_INTASK:
			if ( kernel_data->taskCurrent != OS_NULL )
			{
				OS_SETPROTECTION(kernel_data->taskCurrent);
			}
			break;

		case OS_INSTARTUPHOOK:
			if ( (oldApp->flags & OS_APP_TRUSTED) == 0U )
			{
				/* Non-trusted application startup / shutdown hook. */
				OS_ARCHSETHOOKPROTECTION(
					oldApp,
					&kernel_data->sHookContext.c,
					OS_APPGETSUHOOKPROTSTACKSIZE(oldApp));
			}
			break;

		case OS_INSHUTDOWNHOOK:
			if ( (oldApp->flags & OS_APP_TRUSTED) == 0U )
			{
				/* Non-trusted application startup / shutdown hook. */
				OS_ARCHSETHOOKPROTECTION(
					oldApp,
					&kernel_data->sHookContext.c,
					OS_APPGETSDHOOKPROTSTACKSIZE(oldApp));
			}
			break;

		default:
			/* Nothing needs to be restored. */
			break;
		}
	}
}

/* OS_CallErrorHook() - call an application's error hook
 *
 * This function calls the error hook of an application.
 *
 * On entry, inKernel and inFunction are already set appropriately.
 *
 * Non-trusted error hooks have no explicit timing protection.
 * In most cases they run within the timing protection of the task or
 * ISR that caused the error.
 * Furthermore, practically no system services can be used by the
 * error hook functions, so there is little need to guard against
 * things like incorrect nesting of Get/ReleaseResource().
 * Suspend/ResumeOSInterrupts() are both NO-OPs when called from
 * error hooks.
 * However, the Suspend/Resume/Enable/DisableAllInterrupts() * pairs
 * are allowed, so here we save the nesting information and the current
 * interrupt level and start a new nesting context. On return from
 * the hook function we check for correct nesting, and in any case
 * restore the old nesting count and old level, and set the interrupt
 * level back the way it was.
 *
 * The hook function is called via the OS_ArchCallErrorHook wrapper.
 * This wrapper stores its own context as a way of returning from a killed
 * hook function.
 *
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Nontrusted, 2
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Trusted, 2
*/
/* Deviation MISRAC2012-7 */
void OS_CallErrorHook(const os_appcontext_t *app, os_result_t code)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_appcontext_t *saveHookApp;
	os_uint8_t saveNestSuspendAll;
	os_oldlevel_t saveOldSuspendAll;
	os_uint8_t saveInFunction;
	os_intstatus_t is;
	OS_HOOK_SAVE_INKERNEL_DECL

	/* Save and set the variable that tracks the current application
	 * for hook functions. We do this here so that error handling has
	 * has access to it.
	*/
	saveHookApp = kernel_data->hookApp;
	kernel_data->hookApp = app;

	/* Check that there's enough kernel stack for this hook.
	 *
	 * Should this check be enabled with STACKCHECK or permanently?
	 * At the moment it's always enabled.
	*/
	/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5, MISRAC2012-6 <1> */
	if ( OS_InsufficientStackForErrHook(app) )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		(void) OS_ERROR(OS_ERROR_InsufficientStack, OS_NULL);
	}
	else
	{
		saveInFunction = kernel_data->inFunction;
		kernel_data->inFunction = OS_INERRORHOOK;

		/* Interrupts are already disabled, but we "disable" here again
		 * to get the current state to recover from an error in the hook
		 * function. We also save the nesting state for the services
		 * that can be used inside the hook function.
		*/
		is = OS_IntDisableConditional();
		saveNestSuspendAll = kernel_data->nestSuspendAll;
		saveOldSuspendAll = kernel_data->oldSuspendAll;
		kernel_data->nestSuspendAll = 0;

		/* Now call the hook. The value of OS_GetKernelData()->inKernel is saved on the "counting" architectures only,
		 * and only in the "killable" branch.
		*/
		if ( OS_CallAppEHookDirectly() )
		{
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(*(app->errorHook))(code);
		}
		else
		{
			/* Possible diagnostic TOOLDIAG-2 <START> */
			OS_HOOK_SAVE_INKERNEL();
			/* Deviation MISRAC2012-3, MISRAC2012-4 <1> */
			OS_ArchCallErrorHook(app, code);
			/* Possible diagnostic TOOLDIAG-2 <STOP> */

			/* The exception handler might "return" here directly without
			 * executing the tail end of ArchCallErrorHook() -
			 * architecture-dependent.
			*/
			OS_HOOK_RESTORE_INKERNEL();

			/* Restore memory protection of calling context. */
			OS_HookRestoreMemprot(kernel_data, app, saveHookApp, saveInFunction);
		}

		if ( kernel_data->nestSuspendAll != 0U )
		{
			/* Calls to Suspend/ResumeAllInterrupts() inside the
			 * error hook function were not correctly nested.
			 *
			 * !LINKSTO Kernel.Feature.RuntimeChecks.CheckSuspendResumeNesting, 1
			 * !LINKSTO Kernel.Feature.RuntimeChecks, 1
			 */
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}

		/* Restore the saved interrupt status
		*/
		kernel_data->nestSuspendAll = saveNestSuspendAll;
		kernel_data->oldSuspendAll = saveOldSuspendAll;
		OS_IntRestore(is);
		kernel_data->inFunction = saveInFunction;
	}

	/* Restore the saved current application for hook functions
	*/
	kernel_data->hookApp = saveHookApp;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
