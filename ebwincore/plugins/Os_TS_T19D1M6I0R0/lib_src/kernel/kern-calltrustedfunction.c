/* kern-calltrustedfunction.c
 *
 * This file contains the OS_KernCallTrustedFunction function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-calltrustedfunction.c 28042 2017-12-07 13:32:23Z olme8414 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "calling context checks" are not excluded.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  This warning depends on the OS configuration and library optimization.
 *  The statement can be reached, if "interrupt enable checks" are not excluded.
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and on arithmetic type.
 *
 * Reason:
 * This is done to capture the parameters passed to the function.
 * The conversion ensures, that no bits are lost.
 *
 *
 * MISRAC2012-2) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and on arithmetic type.
 *
 * Reason:
 * Some architectures have to prepare information to be processed by hardware,
 * or query its current status. In both cases type casts are necessary,
 * because hardware has no notion of C language types.
 *
 *
 * MISRAC2012-3) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * Some architectures have to prepare information to be processed by hardware,
 * or query its current status. In both cases type casts are necessary,
 * because hardware has no notion of C language types.
 *
 *
 * MISRAC2012-4) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and any
 * other type.
 *
 * Reason:
 * Some architectures have to prepare information to be processed by hardware,
 * or query its current status. In both cases type casts are necessary,
 * because hardware has no notion of C language types.
 * 
 * MISRAC2012-5) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and on arithmetic type.
 *
 * Reason:
 * The conversion of a pointer into a number is intentional, because this is
 * necessary to do arithmetic and because the caller is only interested in
 * its numerical value to do bounds checks.
*/

#define OS_SID	OS_SID_CallTrustedFunction
#define OS_SIF	OS_svc_CallTrustedFunction

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

#if OS_NEEDS_GetTFStackBounds_Fctcall
static void OS_GetTFStackBounds_Fctcall(os_address_t *, os_address_t *);

static void OS_GetTFStackBounds_Fctcall(os_address_t *lbp, os_address_t *ubp)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	if ( kernel_data->inFunction == OS_INTASK )
	{
		*lbp = OS_TASK_STACK_START;
		*ubp = OS_TASK_STACK_END;
	}
	else
	{
		os_isr_t const * const isr = &OS_isrTableBase[kernel_data->isrCurrent];
		void *isrStackLimit = OS_GetIsrStackLimit(isr->dynamic);

		if (isrStackLimit == OS_NULL)
		{
			*lbp = (os_address_t)kernel_data->iStackBase;
			*ubp = *lbp + kernel_data->iStackLen;
		}
		else
		{
#if (OS_STACKGROWS == OS_STACKGROWSDOWN)
			/* Deviation MISRAC2012-5 <1> */
			*ubp = (os_address_t)isrStackLimit;
			*lbp = *ubp - isr->stackLen;
#else /* OS_STACKGROWSUP */
			/* Deviation MISRAC2012-5 <1> */
			*lbp = (os_address_t)isrStackLimit;
			*ubp = *lbp + isr->stackLen;
#endif /* OS_STACKGROWS */
		}
	}
}
#endif /* OS_NEEDS_GetTFStackBounds_Fctcall */

/*!
 * OS_KernCallTrustedFunction implements the API CallTrustedFunction
 *
 * Trusted functions are executed with kernel privileges on the kernel
 * stack in a kernel environment. The latter means that system calls
 * behave slightly differently.
 *
 * Calls that would not normally return immediately to the caller in a task
 * environment, such as ChainTask, TerminateTask, WaitEvent, Schedule and
 * ActivateTask will immediately return to the caller. Any context switch
 * that results from a system call inside a trusted function will happen
 * when the trusted function returns to its caller.
 *
 * If the trusted function has been called from an ISR context, ActivateTask
 * would do this in any case and the others are not permitted, so the
 * trusted function has to be written to handle this possibility in any case.
 *
 * !LINKSTO Kernel.Autosar.TrustedFunctions, 2
*/
os_result_t OS_KernCallTrustedFunction
(	os_functionid_t fid,
	void *parms
)
{
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL
	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)fid);
	/* Deviation MISRAC2012-1 <1> */
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)parms);

	OS_TRACE_CALLTRUSTEDFUNCTION_ENTRY(fid);

	/* !LINKSTO Kernel.Autosar.API.SystemServices.CallTrustedFunction.CalledByIsr, 1
	 * !LINKSTO Kernel.Autosar.API.SystemServices.CallTrustedFunction.CalledByTask, 1
	*/
	if ( !OS_CallingContextCheck() )
	{
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
	}
	else
	if ( fid >= OS_nFunctions )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.CallTrustedFunction.UnknownFunction, 1
		 * !LINKSTO Kernel.Autosar.TrustedFunctions.NotConfigured, 1
		*/
		r = OS_ERROR(OS_ERROR_InvalidFunctionId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		const os_function_t * const fs = &OS_functionTableBase[fid];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t * const app = OS_CurrentApp();

		if ( !OS_HasPermission(app, fs->permissions) )
		{
			r = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		if ( !OS_APPISACCESSIBLE(fs->app) )
		{
			r = OS_ERROR(OS_ERROR_ApplicationNotAccessible, OS_GET_PARAMETER_VAR());
		}
#if (OS_N_CORES != 1)
		else
		if(OS_GetMyCoreId() != fs->app->core)
		{
			/*
			 * !LINKSTO Kernel.Autosar.API.SystemServices.CallTrustedFunction.NoCrossCore, 1
			*/
			r = OS_ERROR(OS_ERROR_CallTrustedFunctionCrosscore, OS_GET_PARAMETER_VAR());
		}
#endif
		else
		{
			os_address_t upperSp;
			/* Possible diagnostic TOOLDIAG-1 <1> */
			os_address_t lowerSp;
			os_address_t lowerBound;
			os_address_t upperBound;

#if OS_STACKGROWS==OS_STACKGROWSDOWN
			/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
			OS_GetTrustedCallerSp(&upperSp);
			lowerSp = upperSp - fs->stackLen;
#else
			/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
			OS_GetTrustedCallerSp(&lowerSp);
			upperSp = lowerSp + fs->stackLen;
#endif

			OS_GetTrustedFunctionStackBounds(&lowerBound, &upperBound);

			if ( OS_IsTFuncStackSizeInsufficient(lowerSp, upperSp, lowerBound, upperBound) )
			{
				r = OS_ERROR(OS_ERROR_StackError, OS_GET_PARAMETER_VAR());
			}
			else
			{
				/* Deviation MISRAC2012-2, MISRAC2012-3, MISRAC2012-4 <1> */
				OS_PushTrustedFunction(fid, fs->function, parms);
			}
		}
	}

	OS_TRACE_CALLTRUSTEDFUNCTION_EXIT_P(r,fid);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserCallTrustedFunction</name>
  <synopsis>Call a trusted function</synopsis>
  <syntax>
    os_result_t OS_UserCallTrustedFunction
    (   os_functionid_t fid,    /@ Id of function @/
        void *parms             /@ Pointer parameter to pass @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserCallTrustedFunction()</code> calls the referenced
    trusted function with the parameter supplied, provided that the
    caller is in a permitted context and has permission to make the call.
    </para><para>
    It is recommended to make trusted functions as short as possible,
    doing only those jobs such as accessing peripheral devices that can
    only be done with full privileges. It is not recommended to call
    OSEK or AUTOSAR system services from a trusted function.
    </para><para>
    However, if it is absolutely necessary to use system services
    from a trusted function, please take careful note of the following
    restrictions and differences in semantic behaviour:
    </para><para>
    The trusted function is called in a kernel environment, which means that
    all system calls that it makes will return immediately to the caller;
    any resulting task switch will not happen until the trusted function
    returns, thus affecting the calling task but not the trusted function.
    </para><para>
    If the trusted function has been called from an ISR (category 2) context,
    the system services that it can call are restricted accordingly. Calling
    a system service that is not permitted will result in an error code
    being returned to the trusted function. In normal status mode it
    is possible that the calling application could have been terminated.
    </para>
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success</returns>
  <returns>OS_E_ACCESS=Trusted function's application not accessible.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>
