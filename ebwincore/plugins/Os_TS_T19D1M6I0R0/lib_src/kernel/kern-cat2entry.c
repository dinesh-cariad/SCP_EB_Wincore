/* kern-cat2entry.c
 *
 * This file contains the Cat2 interrupt handler wrapper.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-cat2entry.c 28252 2018-02-05 12:52:52Z mist9353 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain
 * persistent side effects.
 *
 * Reason:
 * The rate monitoring function has to make modifications to the global system state.
 *
 *
 * MISRAC2012-2) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and
 * any type other type.
 *
 * Reason:
 * Depending on the architecture, the prototype of common assembly code for
 * setting up an ISR's environment might use another function pointer type
 * than the one given here.
 *
 *
 * MISRAC2012-4) Deviated Rule: 11.5 (required)
 * A conversion shall not be performed between a pointer to void into a pointer
 * to object.
 *
 * Reason:
 * Cast is used for pointer arithmetic to calculate addresses.
 * No access to referenced value takes place.
 *
 *
 * MISRAC2012-5) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * This kind of cast is necessary to determine the amount of free/used space
 * on the stack. Furthermore, different architectures return information about
 * their stacks differently, so this cast serves as an unificator.
 *
 *
 * MISRAC2012-6) Deviated Rule: 20.7 (required)
 * Expressions resulting from expansion of macro parameters shall be enclosed
 * in parentheses.
 *
 * Reason:
 * If done, this would result in a syntax error. Hence, it's not viable.
 *
 *
 * MISRAC2012-7) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different
 * essential type category or a wider essential type.
 *
 * Reason:
 * The cast is necessary, because the length of a stack is stored w/o a sign,
 * but the calculation of remaining stack space needs one.
 *
 *
 * MISRAC2012-8) Deviated Rule: 10.7 (required)
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

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_C_COMPL_010:path]
 *  The code shall adhere to the [HISSRCMETRIC] Metrics.
 *
 * Reason:
 *  The function OS_Cat2Entry uses macros and if-elseif-else-cascades to select
 *  the proper actions. Breaking it up in multiple smaller functions is not a
 *  valid option, because this is the interrupt entry function which needs
 *  to be as quick as possible.
 *  This leads to a high PATH value.
 *
 * DCG-2) Deviated Rule: [OS_C_COMPL_010:stmt]
 *  The code shall adhere to the [HISSRCMETRIC] Metrics.
 *
 * Reason:
 *  The function OS_Cat2Entry uses macros and if-elseif-else-cascades to select
 *  the proper actions. Breaking it up in multiple smaller functions is not a
 *  valid option, because this is the interrupt entry function which needs
 *  to be as quick as possible.
 *  This leads to a high stmt value.
 *
 * DCG-3) Deviated Rule: [OS_C_COMPL_010:vg]
 *  The code shall adhere to the [HISSRCMETRIC] Metrics.
 *
 * Reason:
 *  The function OS_Cat2Entry uses macros and if-elseif-else-cascades to select
 *  the proper actions. Breaking it up in multiple smaller functions is not a
 *  valid option, because this is the interrupt entry function which needs
 *  to be as quick as possible.
 *  This leads to a high VG value.
*/
/*
 * TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   unused parameter 'isr'
 *
 * Reason:
 * For some configurations OS_InsufficientStackForIsr() and  OS_RATEMONITORCHECK are evaluated as constant.
 * In that case 'isr' is not used.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statements can be reached if stack checks are enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statements can be reached if rate monitoring is enabled.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statements can either be reached if there are killable ISRs or if there no killable ISRs.
 *
 * TOOLDIAG-5) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The OS only checks if the ISR returned while holding resources, if status is EXTENDED.
 *  The OS only does the nesting check if status is EXTENDED.
*/

#define OS_SID OS_SID_IsrHandler
#define OS_SIF OS_svc_IsrHandler

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C checkers want prototypes for static helpers */
static void OS_CleanUpISRResources(os_isrdynamic_t *);
static os_boolean_t OS_IsrErrorChecksOk(const os_isr_t *isr, os_isrdynamic_t *id);

/* Helper to clean up an ISR's resources if the ISR exited without releasing them.
 * Also reports the corresponding error in case the ISR wasn't killed.
*/
static void OS_CleanUpISRResources(os_isrdynamic_t *id)
{
	os_objectid_t lock;

	OS_PARAM_UNUSED(id);

	lock = OS_GetKernelData()->isrLastLock;

	if ( !OS_IsIsrKilled(id) )
	{
		/* ISR wasn't killed, i.e. terminated voluntarily. So this is an error in the ISR,
		 * because it must release all resources and spinlocks at its voluntary termination.
		 * If it was killed, it's not considered an error, because it wouldn't have had a
		 * chance to release all locks.
		*/
		if (OS_GetTypeFromObjectId(lock) == OS_OBJ_SPINLOCK)
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_HoldsLock, OS_NULL);
		}
		else
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			(void) OS_ERROR(OS_ERROR_HoldsResource, OS_NULL);
		}
	}

	/* Release the resources that were taken by the ISR
	 *
	 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.KillISR, 1
	*/
	OS_ReleaseLocks(lock);
}

/* Helper to do ISR related error checks and reporting
 * returns false if we have a stack overflow problem or if we've exceeded
 * our rate limit; returns true if everything is fine and the ISR can run.
*/
/* Possible diagnostic TOOLDIAG-1 */
static os_boolean_t OS_IsrErrorChecksOk(const os_isr_t *isr, os_isrdynamic_t *id)
{
	os_boolean_t allok = OS_FALSE;

	/* Deviation MISRAC2012-4, MISRAC2012-5, MISRAC2012-7, MISRAC2012-8 <1> */
	if ( OS_InsufficientStackForIsr(isr) )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_InsufficientStack, OS_NULL);
	}
	else
	/* Deviation MISRAC2012-1 */
	if ( OS_RATEMONITORCHECK(isr->rateMonitor) )
	{
		/* Rate monitoring
		 *
		 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.RateLimit.Interrupts, 2
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		id->statusflags |= OS_ISRF_RATEEX;
		OS_ClearIsr(isr);

		/* This error must not attempt to kill the current ISR, because it isn't running.
		*/
		/* can't propagate the return value of OS_ERROR -> ignore it */
		(void) OS_ERROR(OS_ERROR_RateLimitExceeded, OS_NULL);
	}
	else
	{
		/* everything ok */
		allok = OS_TRUE;
	}

	return allok;
}

/* OS_Cat2Entry() - call a Category 2 ISR.
 *
 * This function calls a category 2 ISR.
 *
 *  - The previous value if inKernel is saved, then inKernel is set
 *    to 1.
 *  - The previous value if inFunction is saved, then inFunction is set
 *    to INCAT2.
 *  - If execution timing is configured, any running timer is suspended
 *    and new one for the current ISR is started.
 *  - The pre-ISR hook (if any) is called.
 *  - Then the ISR is called via either the OS_CallIsrDirect wrapper or the OS_CallIsrIndirect wrapper.
 *    The indirect wrapper stores its own context as a way of returning from a killed ISR.
 *  - Then the post-ISR hook (if any) is called.
 *  - The exec timer is stopped and a the suspended timer
 *    (if any) is restarted.
 *  - Finally, the saved value of inFunction is restored, and the saved
 *    value of inKernel is returned. We must remain "in-kernel" because the
 *    interrupt exit routine might need to reschedule.
 *
 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
 *		The hook routines are called outside the runPrio region.
 *
 * !LINKSTO Kernel.Feature.IsrHooks.PreIsrHook, 1
 * !LINKSTO Kernel.Feature.IsrHooks.PostIsrHook, 1
 * !LINKSTO Kernel.Feature.IsrHooks, 1
 *
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Nontrusted, 2
 * !LINKSTO Kernel.Autosar.OsApplication.Trust.Trusted, 2
 *		ISRs are called via an architecture-dependent macro that sets the protection boundaries appropriately
*/
/* Deviation DCG-1 <START> */
/* Deviation DCG-2 <START> */
/* Deviation DCG-3 <START> */
os_uint8_t OS_Cat2Entry(os_isrid_t iid)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_isr_t *isr;
	os_isrdynamic_t *id;

	os_uint8_t inKernel;
	os_uint8_t inFunction;

	os_isrid_t oldIsr;

	/* Deviation MISRAC2012-6 <START> */
	OS_SAVEISRNESTSUSPEND_DECL(nestOs, nestAll)
	OS_SAVEISRLASTLOCK_DECL(oldLastLock)
	OS_CAT2PREEMPT_DECL(accSave)
	/* Deviation MISRAC2012-6 <STOP> */

	/* Generic CPU load measurement: enter a busy interval
	*/
	OS_CPULOAD_GENERICCAT2_ENTRY();

	/* Suspend any execution timing that is running
	*/
	OS_CAT2PREEMPTEXECTIMING(&accSave);

	/* Find the interrupt descriptor and get the address of the ISR itself
	*/
	isr = &OS_isrTableBase[iid];
	id = isr->dynamic;

	/* Save and set inKernel, inFunction, isrCurrent. Save accounting type.
	*/
	inKernel = kernel_data->inKernel;
	OS_SetIsrinKernel();

	inFunction = kernel_data->inFunction;
	kernel_data->inFunction = OS_INCAT2;

	oldIsr = kernel_data->isrCurrent;
	kernel_data->isrCurrent = iid;

	/* !LINKSTO Kernel.Feature.StackCheck.Automatic, 1
	*/
	if ( OS_IsrErrorChecksOk(isr, id) )
	{
		OS_SaveIsrLastLock(&oldLastLock);
		OS_SaveIsrNestSuspend(&nestOs, &nestAll);

		/* Call the PreISRHook() if configured
		*/
		OS_CALLPREISRHOOK(iid);

		/* Do ISR exec timing if configured
		 *
		 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.ISR, 1
		*/
		OS_STARTISREXECTIMING(isr->execBudget);

		/* Clear the "killed" flag. This flag gets set if the ISR gets killed. It is used
		 * to avoid falsely reporting that the ISR has terminated without releasing resources
		 * and/or re-enabling interrupts.
		*/
		OS_MarkIsrNotKilled(id);

		OS_TRACE_STATE_ISR(iid, OS_TRACE_ISR_SUSPENDED, OS_TRACE_ISR_RUNNING);

		OS_HW_TRACE_ISR(iid);
		if ( OS_CallIsrDirectly() )
		{
			/* Possible diagnostic TOOLDIAG-4 <+1> */
			OS_CallIsrDirect(isr, id);
		}
		else
		{
			/* We can't set up the protection registers here because we don't
			 * yet know how much stack OS_CallIsr() requires.
			*/
			/* Possible diagnostic TOOLDIAG-4 <+2> */
			/* Deviation MISRAC2012-2 */
			OS_CallIsrIndirect(isr, id);
		}

		OS_TRACE_STATE_ISR(iid, OS_TRACE_ISR_RUNNING, OS_TRACE_ISR_SUSPENDED);

		/* Stop the execution timing that was started earlier.
		*/
		OS_STOPISREXECTIMING(isr);

		/* Call the PostISRHook() if configured
		*/
		OS_CALLPOSTISRHOOK(iid);

		/* Restore the protection registers for the interrupted
		 * task or ISR.
		 * See comment above for why we can't do it in ArchCallIsr().
		 * WARNING: the registers are set back to their initial
		 * values for the interrupted task/ISR. This means that
		 * dynamic modification of the registers by a task or
		 * ISR is NOT SUPPORTED
		*/
		/* This implementation is not optimal - it always restores the protection
		 * registers even if they were not changed. A better implementation needs
		 * the concept of a "memory-protection owner" similar to that for MMU.
		 * See also ASCOS-1451.
		 * Remove this comment when implemented.
		*/
		if ( oldIsr < OS_nInterrupts )
		{
			OS_SETISRPROTECTION(&OS_isrTableBase[oldIsr], OS_isrTableBase[oldIsr].dynamic);
		}
		else
		if ( kernel_data->taskCurrent != OS_NULL )
		{
			OS_SETPROTECTION(kernel_data->taskCurrent);
		}
		else
		{
			/* MISRA-C */
		}

		/* Check that all resources taken have been released.
		 * isrCurrent still contains the ISR-ID, so can be used by
		 * the error handler to determine who is guilty.
		 * !LINKSTO Kernel.Autosar.ServiceErrors.Miscellaneous.IsrReturn.Resources, 2
		*/
		if ( OS_IsrOccupiesResource() )
		{
			/* Possible diagnostic TOOLDIAG-5 <+1> */
			OS_CleanUpISRResources(id);
		}
		else
		if ( OS_IsrSuspendNestingError(nestOs, nestAll) )
		{
			/* Possible diagnostic TOOLDIAG-5 <+1> */
			if ( !OS_IsIsrKilled(id) )
			{
				/* ISR wasn't killed, so this is an error in the ISR.
				 *
				 * !LINKSTO Kernel.Feature.RuntimeChecks.CheckSuspendResumeNesting, 1,
				 * !        Kernel.Feature.RuntimeChecks, 1,
				 * !        Kernel.Autosar.ServiceErrors.Miscellaneous.IsrReturn.DisabledInterrupts, 2
				 */
				/* can't propagate the return value of OS_ERROR -> ignore it */
				/* Possible diagnostic TOOLDIAG-4 <+1> */
				(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
			}
			/* No cleaning up to do - kernel manages interrupts perfectly
			 *
			 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.KillISR, 1
			*/
		}
		else
		{
			/* MISRA-C */
		}

		OS_RestoreIsrLastLock(oldLastLock);
		OS_RestoreNestSuspend(nestOs, nestAll);
	}

	/* Check for a kernel (or ISR) stack overflow. This shouldn't happen if trusted ISRs
	 * keep within their bounds, but there's always the possibility ...
	 *
	 * Note: On some CPU families, this code may run on a different stack than
	 * the ISR, so both need to get checked.
	 *
	 * !LINKSTO Kernel.Autosar.StackMonitoring.Detection, 2
	*/
	if ( OS_CAT2STACKOVERFLOW() )
	{
		/* !LINKSTO Kernel.Autosar.StackMonitoring.Reporting.NoProtectionHook, 1
		 * !LINKSTO Kernel.Autosar.StackMonitoring.Reporting.ProtectionHook, 1
		 * !LINKSTO Kernel.Feature.StackCheck.Automatic, 1
		 *
		 * The protection hook is always called (if configured).
		 * The default action (which is what happens if there is no protection hook) is to shut down the system.
		*/
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_KernelStackOverflow, OS_NULL);
	}

	/* Restore previous values of isrCurrent and inFunction
	*/
	kernel_data->isrCurrent = oldIsr;
	kernel_data->inFunction = inFunction;
	/* Must be after isrCurrent to avoid race with category 1 ISR tracing. */
	OS_HW_TRACE_ISR(oldIsr);

	/* Check if the interrupted ISR is still permitted to run (i.e.
	 * hasn't been killed by a higher-priority ISR or something
	*/
	if ( OS_KillNextIsr() )
	{
		/* Possible diagnostic TOOLDIAG-4 <START> */
		/* On targets which require a late notification of the handled interrupt,
		 * this must done before killing further interrupts to properly end the
		 * (nested) interrupt(s) in the handler.
		 * NOTE: OS_KillIsr() won't return!
		*/
		OS_LateEndOfInterrupt(isr);

		/* no way to propagate errors, and none expected anyways -> ignore return value */
		(void) OS_KillIsr(oldIsr);
		/* Possible diagnostic TOOLDIAG-4 <STOP> */
	}

	/* Now we resume the execution timing for the interrupted whatever.
	*/
	OS_CAT2RESUMEEXECTIMING(&accSave);

	/* inKernel remains set, but the old value is returned to the
	 * exit function so that it can determine if a task switch is needed.
	*/
	return inKernel;
}
/* Deviation DCG-3 <END> */
/* Deviation DCG-2 <END> */
/* Deviation DCG-1 <END> */

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
