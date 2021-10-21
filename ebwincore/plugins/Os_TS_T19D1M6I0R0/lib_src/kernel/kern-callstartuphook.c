/* kern-callstartuphook.c
 *
 * This file contains the wrapper for application-specific startup hooks
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-callstartuphook.c 27921 2017-11-20 16:30:08Z mist9353 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and
 * any type other type.
 *
 * Reason:
 * Depending on the architecture, the prototype of common assembly code for
 * setting up a hook's environment might use another function pointer type
 * than the one given here.
 *
 * MISRAC2012-2) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * This kind of cast is necessary to determine the amount of free/used space
 * on the stack. Furthermore, different architectures return information about
 * their stacks differently, so this cast serves as an unificator.
 *
 * MISRAC2012-4) Deviated Rule: 11.5 (required)
 * A conversion shall not be performed between a pointer to void into pointer
 * to object.
 *
 * Reason:
 * Cast is used for pointer arithmetic to calculate addresses.
 * No access to referenced value takes place.
 *
 * MISRAC2012-5) Deviated Rule: 18.4 (required)
 * The +, -, += and -= operators should not be applied to an expression
 * of pointer type.
 *
 * Reason:
 * Pointer arithmetic is needed to calculate stack pointer values on
 * some CPU families.
 *
 * MISRAC2012-6) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different
 * essential type category or a wider essential type.
 *
 * Reason:
 * The cast is necessary, because the length of a stack is stored w/o a sign,
 * but the calculation of remaining stack space needs one.
 *
 * MISRAC2012-7) Deviated Rule: 10.7 (required)
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
 *  Depending on whether application startup hooks are called directly,
 *  the respective branch is either reachable or unreachable.
*/

#define OS_SID	OS_SID_HookHandler
#define OS_SIF	OS_svc_HookHandler

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_CallStartupHook() - call an application's startup hook
 *
 * This function calls the startup hook of an application.
 *
 * On entry, inKernel and inFunction are already set appropriately.
 *
 * There is no execution timing for non-trusted startup hooks.
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
 * The hook function is called via the OS_CallStartupHook wrapper.
 * This wrapper stores its own context as a way of returning from a killed
 * hook function.
 *
 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.StartupHook.Call, 2
 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.StartupHook.AccessRights, 1
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Nontrusted, 2
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Trusted, 2
*/
void OS_CallStartupHook(const os_appcontext_t *app)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_appcontext_t *saveHookApp;
	os_uint8_t saveNestSuspendAll;
	os_oldlevel_t saveOldSuspendAll;
	os_intstatus_t is;
	OS_HOOK_SAVE_INKERNEL_DECL

	/* Save and set the variable that tracks the current application
	 * for hook functions
	*/
	saveHookApp = kernel_data->hookApp;
	kernel_data->hookApp = app;

	/* Check that there's enough kernel stack for this hook.
	 *
	 * Should this check be enabled with STACKCHECK or permenantly?
	 * At the moment it's always enabled.
	*/
	/* Deviation MISRAC2012-4, MISRAC2012-2, MISRAC2012-6, MISRAC2012-7 <1> */
	if ( OS_InsufficientStackForStHook(app) )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		(void) OS_ERROR(OS_ERROR_InsufficientStack, OS_NULL);
	}
	else
	{
		/* Theres' no need so save/set kernel_data->inFunction, because this function is called
		 * from a context where kernel_data->inFunction is already correctly set.\
		*/
		/* Interrupts are already disabled, but we "disable" here again
		 * to get the current state to recover from an error in the hook
		 * function. We also save the nesting state for the services
		 * that can be used inside the hook function.
		*/
		is = OS_IntDisable();
		saveNestSuspendAll = kernel_data->nestSuspendAll;
		saveOldSuspendAll = kernel_data->oldSuspendAll;
		kernel_data->nestSuspendAll = 0;

		/* Now call the hook. The value of OS_GetKernelData()->inKernel is saved on the "counting" architectures only,
		 * and only in the "killable" branch.
		*/
		if ( OS_CallAppSHookDirectly() )
		{
			/* Call the hook function directly. No protection is possible.
			*/
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(*(app->startupHook))();
		}
		else
		{
			/* We can't set up the protection registers here because we don't
			 * yet know how much stack OS_CallStartupHook() requires.
			*/
			/* Possible diagnostic TOOLDIAG-2 <START> */
			OS_HOOK_SAVE_INKERNEL();
			/* Deviation MISRAC2012-1, MISRAC2012-5 */
			OS_ArchCallStartupHook(app);
			/* Possible diagnostic TOOLDIAG-2 <STOP> */

			/* The exception handler might "return" here directly without
			 * executing the tail end of ArchCallStartupHook() -
			 * architecture-dependent.
			 * There's no need to restore any protection mechanisms
			 * because nothing is using them yet - still in StartOS()!
			*/
			OS_HOOK_RESTORE_INKERNEL();
		}

		if ( kernel_data->nestSuspendAll != 0 )
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
	}

	/* Restore the saved current application for hook functions
	*/
	kernel_data->hookApp = saveHookApp;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
