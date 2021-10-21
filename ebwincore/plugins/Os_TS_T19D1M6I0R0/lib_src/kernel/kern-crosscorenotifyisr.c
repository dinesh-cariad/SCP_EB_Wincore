/* kern-crosscorenotifyisr.c
 *
 * This file contains the cross-core notification interrupt handler.
 * This ISR is triggered, when messages shall be exchanged between cores.
 * see also OS_NotifyCore().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-crosscorenotifyisr.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (required)
 * A conversion shall not be performed between a pointer to void into a pointer
 * to object.
 *
 * Reason:
 * Cast is used for pointer arithmetic to calculate addresses.
 * No access to referenced value takes place.
 *
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from expansion of macro parameters shall be enclosed
 * in parentheses.
 *
 * Reason:
 * If done, this would result in a syntax error. Hence, it's not viable.
 *
 *
 * MISRAC2012-3) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * This kind of type cast is necessary to determine the amount of free/used
 * space on the stack.
 *
 *
 * MISRAC2012-4) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason:
 * Some architectures require certain measures to ensure proper exectuion
 * flow (e.g., flushing the instruction pipeline). These measures might be
 * classified as 'dead code', even though they are vital.
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
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 * Variable is set but never used.
 *
 * Reason:
 * Not an issue, variable will be used if stack checks are enabled.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if stack checks are enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if there are killable ISRs.
*/

#define OS_SID OS_SID_IsrHandler
#define OS_SIF OS_svc_IsrHandler

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_CrossCoreNotifyIsr()
 *
 * This function is called whenever a cross-core notification interrupt occurs.
 * It is used in place of the Cat2/CatK entry function and so must behave exactly like
 * that function:
 *
 *  - The previous value of inKernel is saved, then inKernel is set
 *    to 1.
 *  - The previous value of inFunction is saved, then inFunction is set
 *    to ININTERNAL (unless it is already INSHUTDOWN!).
 *  - If execution timing is configured, any running timer is suspended.
 *  - All cross-core notification events are processed.
 *  - The suspended timer (if any) is restarted.
 *  - Finally, the saved value of inFunction is restored, and the saved
 *    value of inKernel is returned. We must remain "in-kernel" because the
 *    interrupt exit routine might need to reschedule.
*/
os_uint8_t OS_CrossCoreNotifyIsr(os_isrid_t iid)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	/* Possible diagnostic TOOLDIAG-1 */
	os_isr_t const* isr;
	os_uint8_t inKernel;
	os_uint8_t inFunction;
	/* Deviation MISRAC2012-2 <1> */
	OS_CAT2PREEMPT_DECL(accSave)

	/* Generic CPU load measurement: enter a busy interval.
	*/
	OS_CPULOAD_GENERICCAT2_ENTRY();

	/* Suspend any execution timing that is running.
	*/
	OS_CAT2PREEMPTEXECTIMING(&accSave);

	inKernel = kernel_data->inKernel;
	kernel_data->inKernel = 1;

	/* Deviation MISRAC2012-4 <1> */
	OS_AckNotifyCore();

	inFunction = kernel_data->inFunction;
	if (inFunction != OS_INSHUTDOWN)
	{
		kernel_data->inFunction = OS_ININTERNAL;
	}

	/* Possible diagnostic TOOLDIAG-1 */
	isr = &OS_isrTableBase[iid];
	/* Deviation MISRAC2012-1, MISRAC2012-3, MISRAC2012-5, MISRAC2012-6 <1> */
	if ( OS_InsufficientStackForIsr(isr) )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_InsufficientStack, OS_NULL);
	}
	else
	{
		OS_ReceiveMessage(OS_GetMyCoreId());
	}

	/* Check for a kernel stack overflow. This shouldn't happen if trusted ISRs
	 * keep within their bounds, but there's always the possibility ...
	 *
	 * !LINKSTO Kernel.Autosar.StackMonitoring.Detection, 2
	*/
	if ( OS_KernelStackOverflow() )
	{
		/*
		 * !LINKSTO Kernel.Feature.StackCheck.Automatic, 1
		 *
		 * What actually happens is that the protection hook is always called (if configured).
		 * The default action (which is what happens if there is no protection hook) is to shut down the system.
		*/
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_KernelStackOverflow, OS_NULL);
	}

	kernel_data->inFunction = inFunction;

	/* Check if the interrupted ISR is still permitted to run, i.e.
	 * hasn't been killed by a higher-priority ISR or something else.
	*/
	if ( OS_KillNextIsr() )
	{
		/* On targets which require a late notification of the handled interrupt,
		 * this must done before killing further interrupts to properly end the
		 * (nested) interrupt(s) in the handler.
		 * NOTE: OS_KillIsr() won't return!
		*/
		/* Possible diagnostic TOOLDIAG-3 <START> */
		OS_LateEndOfInterrupt(isr);

		/* no way to propagate errors, and none expected anyway -> ignore return value */
		(void) OS_KillIsr(kernel_data->isrCurrent);
		/* Possible diagnostic TOOLDIAG-3 <STOP> */
	}

	/* Now we resume the execution timing for the interrupted whatever.
	*/
	OS_CAT2RESUMEEXECTIMING(&accSave);

	/* inKernel remains set, but the old value is returned to the
	 * exit function so that it can determine if a task-switch is needed.
	*/
	return inKernel;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
