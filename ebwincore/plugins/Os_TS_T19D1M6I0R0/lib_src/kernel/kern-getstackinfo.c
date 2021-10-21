/* kern-getstackinfo.c
 *
 * This file contains the OS_KernGetStackInfo function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getstackinfo.c 30506 2019-07-15 12:44:46Z hagr271648 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This comparison is architecture-dependent;
 *   it becomes pointless when no private stack is used for ISRs.
 *   See macro 'OS_GetIsrStackBase()' for more details.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if status is EXTENDED.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Different architectures store their stack-related information differently.
 * To present user-space a coherent view on this information, this type cast
 * serves as a unificator and hence, is necessary.
*/

#define OS_SID	OS_SID_GetStackInfo
#define OS_SIF	OS_svc_GetStackInfo

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>
/* MISRA-C checkers want prototypes for static helpers */
static void OS_ComputeStackStatus(os_stackinfo_t *);

/* Helper to compute and fill out->StackStatus and out->StackClean.
 * Caller has already filled out->StackBase, out->stackLen and out->stackPointer.
*/
static void OS_ComputeStackStatus(os_stackinfo_t *out)
{
	os_size_t clean = 0;
	os_stackelement_t *sp;

#if OS_STACKGROWS==OS_STACKGROWSDOWN
	sp = (os_stackelement_t *)out->stackBase;

	while ( (*sp == OS_STACKFILL) && (clean < out->stackLen) )
	{
		clean += sizeof(os_stackelement_t);
		sp++;
	}

	/* The limits here might need adjusting if the processor's
	 * stack pointer mechanism is post-decrement rather than
	 * pre-decrement. Are there any processors that work like this?
	 * The condition for underflow will be incorrect.
	 * The condition for overflow will be a little aggressive.
	*/

	if ( (out->stackPointer != 0) && (out->stackPointer < out->stackBase) )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Overflow, 1
		*/
		out->stackStatus = +1;
	}
	else
	if ( (out->stackPointer != 0) && (out->stackPointer > (out->stackBase + out->stackLen)) )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Underflow, 1
		*/
		out->stackStatus = -1;
	}
	else
	if ( clean == 0 )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Overflow, 1
		*/
		out->stackStatus = +1;
	}
	else
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Ok, 1
		*/
		out->stackStatus = 0;
	}
#else
	sp = (os_stackelement_t *)(out->stackBase + out->stackLen - sizeof(os_stackelement_t));

	while ( (*sp == OS_STACKFILL) && (clean < out->stackLen) )
	{
		clean += sizeof(os_stackelement_t);
		sp--;
	}

	/* The limits here might need adjusting if the processor's
	 * stack pointer mechanism is post-decrement rather than
	 * pre-decrement. Are there any processors that work like this?
	 * The condition for underflow will be incorrect.
	 * The condition for overflow will be a little aggressive.
	*/

	if ( (out->stackPointer != 0) && (out->stackPointer < out->stackBase) )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Underflow, 1
		*/
		out->stackStatus = -1;
	}
	else
	if ( (out->stackPointer != 0) && (out->stackPointer > (out->stackBase + out->stackLen)) )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Overflow, 1
		*/
		out->stackStatus = +1;
	}
	else
	if ( clean == 0 )
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Overflow, 1
		*/
		out->stackStatus = +1;
	}
	else
	{
		/* !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.Ok, 1
		*/
		out->stackStatus = 0;
	}
#endif

	out->stackClean = clean;
}

/* MISRA-C checkers want prototypes for static helpers */
static void OS_FillStackInfo(os_kerneldata_t *, const os_task_t *, const os_isr_t *, os_stackinfo_t *);

/* Helper to compute and fill out->StackStatus and out->StackClean.
 * Caller has already filled out->StackBase, out->stackLen and out->stackPointer.
*/
static void OS_FillStackInfo
(	os_kerneldata_t * kernel_data,
	const os_task_t *ts,
	const os_isr_t *is,
	os_stackinfo_t *out
)
{
	os_taskdynamic_t *td;

	if ( ts != OS_NULL )
	{
		/*
		 * !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.TaskLevel, 1
		*/
		td = ts->dynamic;

		out->stackBase = (os_stackinfoptr_t)ts->stackBase;
		out->stackLen = ts->stackLen;
		/* We only get the stackpointer for non-running tasks. If
		 * the task is currently running, we can't determine it.
		 * This avoids overwriting any SP that the caller has put there.
		*/
		if ( td->state != OS_TS_RUNNING )
		{
			/* Deviation MISRAC2012-1 <1> */
			out->stackPointer = (os_stackinfoptr_t)OS_GetTaskSp(ts);
		}

		/* The initiator is not an ISR, hence invalidate respective fields. */
		out->isrStackBase = 0U;
		out->isrStackLen = 0U;
	}
	else
	{
		/*
		 * !LINKSTO Kernel.Feature.StackCheck.API.stackCheck.ISRLevel, 1
		*/

		if (is == OS_NULL)
		{
			/* OS_NULLISR and no ISR in execution -> return kernel stack
			 * information.
			 */
			out->stackBase = 0U;
			out->isrStackLen = kernel_data->iStackLen;
			out->isrStackBase = (os_stackinfoptr_t)kernel_data->iStackBase;
		}
		else
		{
			/* Depending on the architecture, ISRs run either on a private
			 * stack or on the global kernel/interrupt stack.
			 * OS_GetIsrStackBase() will return OS_NULL, in case the global
			 * kernel/interrupt stack is used.
			 */
			out->stackBase = (os_stackinfoptr_t)OS_GetIsrStackBase(is);
			out->isrStackLen = is->stackLen;
#if OS_STACKGROWS==OS_STACKGROWSDOWN
			out->isrStackBase = (os_stackinfoptr_t)
					( (os_address_t)(is->dynamic->c.stackLimit) - is->stackLen );
#else
			out->isrStackBase = (os_stackinfoptr_t)is->dynamic->c.stackLimit;
#endif
		}

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (out->stackBase == 0U)
		{
			/* kernel stack is used */
			out->stackBase = (os_stackinfoptr_t)kernel_data->iStackBase;
			out->stackLen = kernel_data->iStackLen;
			out->stackPointer = (os_stackinfoptr_t)OS_GetCurrentSp();
		}
		else
		{
			/* private stack is used */
			out->stackLen = is->stackLen;
			out->stackPointer = (os_stackinfoptr_t)OS_GetCurrentIsrSp();
		}
	}
}

/*!
 * OS_KernGetStackInfo implements the API OS_UserGetStackInfo
 *
 * The information about the specified stack is placed in the 'out'
 * location.
 *
 * Interrupts are enabled on entry and remain so throughout. The
 * state returned is therefore a snapshot of the task's state,
 * and could have changed by the time the task actually gets it.
 * However, this could happen anyway.
 *
 * !LINKSTO Kernel.Autosar.StackMonitoring, 2
 * !LINKSTO Kernel.Feature.StackCheck.API.stackCheck, 1
*/
/* Deviation DCG-1 <START> */
os_result_t OS_KernGetStackInfo
(	os_taskorisr_t toi,	/* Task or ISR ID */
	os_stackinfo_t *out	/* Where to put the answer */
)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_taskid_t tid;
	const os_task_t *ts = OS_NULL;
	const os_isr_t *is = OS_NULL;
	os_result_t r = OS_E_OK;
	os_boolean_t ok = OS_FALSE;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)toi);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)out);

	OS_TRACE_GETSTACKINFO_ENTRY(toi);

	if ( OS_ArchCanWrite(out, sizeof(*out)) == 0 )
	{
		r = OS_ERROR(OS_ERROR_WriteProtect, OS_GET_PARAMETER_VAR());
	}
	else
	if ( toi == OS_TOI_CURRENTCONTEXT )
	{
		if ( kernel_data->inFunction == OS_INTASK )
		{
			ts = kernel_data->taskCurrent;
			ok = OS_TRUE;
		}
		else
		if ( kernel_data->isrCurrent != OS_NULLISR )
		{
			is = &OS_isrTableBase[kernel_data->isrCurrent];
			ok = OS_TRUE;
		}
		else
		{
			r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( OS_IsTaskId(toi) )
	{
		tid = (os_taskid_t) OS_TOIToTask(toi);

		if ( tid == OS_NULLTASK )
		{
			if ( kernel_data->taskCurrent == OS_NULL )
			{
				/* Not an error */
				r = OS_E_NOFUNC;
			}
			else
			{
				ts = kernel_data->taskCurrent;
				ok = OS_TRUE;
			}
		}
		else
		if ( OS_IsValidTaskId(tid) )
		{
			ts = &OS_taskTableBase[tid];
			ok = OS_TRUE;
		}
		else
		{
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			r = OS_ERROR(OS_ERROR_InvalidTaskId, OS_GET_PARAMETER_VAR());
		}
	}
	else
	{
		os_isrid_t iid = (os_isrid_t) OS_TOIToIsr(toi);

		if (iid == OS_NULLISR)
		{
			if (kernel_data->isrCurrent != OS_NULLISR)
			{
				is = &OS_isrTableBase[kernel_data->isrCurrent];
			}
			/* else: is == OS_NULL -> return kernel stack information. */
			ok = OS_TRUE;
		}
		else
		if (iid < OS_nInterrupts)
		{
			is = &OS_isrTableBase[iid];
			ok = OS_TRUE;
		}
		else
		{
			r = OS_ERROR(OS_ERROR_InvalidIsrId, OS_GET_PARAMETER_VAR());
		}
	}

	if ( ok )
	{
		OS_FillStackInfo(kernel_data, ts, is, out);
		OS_ComputeStackStatus(out);
	}

	OS_TRACE_GETSTACKINFO_EXIT_P(r,toi);
	return r;
}
/* Deviation DCG-1 <END> */

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetStackInfo</name>
  <synopsis>Get information about a stack</synopsis>
  <syntax>
    os_result_t OS_UserGetStackInfo
    (   os_taskorisr_t id,    /@ ID of task or ISR @/
        os_stackinfo_t *out   /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_UserGetStackInfo()</code> places information about
    a task or ISR stack into the specified 'out' location.
    <para>
      <code>OS_TaskToTOI(task_id)</code> should be used
      to specify a task id. If the task ID is OS_NULLTASK, information
      about the current task is returned. If there is no current task,
      the 'out' location is not modified and OS_E_NOFUNC is returned,
      but the error handler is not called.
    </para>
    <para>
      <code>OS_IsrToTOI(isr_id)</code> should be used
      to specify an ISR id. If the ISR_ID is OS_NULLISR, information
      about the current ISR is returned. If there is no current ISR,
      information about the global kernel stack is returned. Depending
      on the architecture and on the calling mechanism of ISRs, the kernel
      stack may get shared for ISRs, or private ISR stacks may get used.
      If private ISR stacks are used - which is quite the exception -
      it is not advisable to estimate free ISR stack using OS_NULLISR
      outside of an ISR.
    </para>
    <para>
      As a special case, if the id parameter is OS_TOI_CURRENTCONTEXT,
      the information about the caller's context is returned. In this case
      the sp is always OS_NULL.
    </para>
    <para>
      The stackPointer field of the 'out' variable is not updated if the
      request is for the current task. This allows the caller to place
      the current SP value there before calling
      <code>OS_UserGetStackInfo()</code>
    </para>
    <para>
      The fields isrStackBase and isrStackLen only apply to ISRs; they
      are set to NULL and 0 respectively, when a task queries its stack
      information. These fields represent the currently accessible part
      of the area described by the fields stackBase and stackLen. This is
      relevant, when memory protection is enabled.
    </para>
  </description>
  <availability>
    Can be called from Tasks and Category 2 ISRs.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
