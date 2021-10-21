/* kern-canwrite.c
 *
 * This file contains the OS_CanWrite function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-canwrite.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 14.4 (requried)
 * The controlling expression of an if statement and the controlling
 * expression of an iteration-statement shall have essentially Boolean type.
 *
 * Reason:
 * False positive reported by the tool.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_CanWrite
 *
 * Returns TRUE if the given application is permitted to write to the
 * whole of the specified memory area.
 *
 * LIMITATION: If two of the regions to which writing is permitted are
 * contiguous and the queried region straddles the two contiguous regions,
 * the test will still fail. However, this is not considered to be a problem,
 * since correctly-defined C data structures will always lie entirely inside
 * one region.
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.Address, 2
 * !LINKSTO Kernel.Autosar.ServiceErrors.Address.Alignment, 1
*/

os_boolean_t OS_CanWrite(const void *base, os_size_t len)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_address_t first;
	os_address_t last;
	os_boolean_t result = OS_FALSE;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_appcontext_t *app;

	first = (os_address_t)base;
	last = first + len;

	if ( (first == 0) || (last <= first) )
	{
		/* Indicates parameter oddity - null pointer, or the specified
		 * region wraps over the highest address, or then length is zero.
		 * This can cause false permission to be granted if not trapped.
		 *
		 * Fails (result is OS_FALSE)
		*/
	}
	else
	/* Deviation MISRAC2012-1 */
	if ( OS_MisalignedData(base, len) )
	{
		/* Indicates data is misaligned and cannot be written.
		 *
		 * Fails (result is OS_FALSE)
		*/
	}
	else
	if ( kernel_data->inFunction == OS_INTASK )
	{
		/* Called from a task.
		*/
		app = OS_GET_APP(kernel_data->taskCurrent->app);

		if ( OS_AppIsTrusted(app) )
		{
			/* Trusted tasks can write anywhere
			*/
			result = OS_TRUE;
		}
		else
		if ( (first >= OS_TASK_STACK_START) && (last <= OS_TASK_STACK_END) )
		{
			/* Non-trusted tasks can write to their own stack
			*/
			result = OS_TRUE;
		}
		else
		if ( OS_IS_WITHIN_APP_REGION(first, last) )
		{
			/* Non-trusted tasks can write to their application's common
			 * data area.
			*/
			result = OS_TRUE;
		}
		else
		if ( OS_IS_WITHIN_TASK_REGION(first, last) )
		{
			/* Non-trusted tasks can write to their own private data
			 * area.
			*/
			result = OS_TRUE;
		}
		else
		{
			/* Fails (result is OS_FALSE) */
		}
	}
	else
	if ( kernel_data->inFunction == OS_INCAT2 )
	{
		/* Called from a Category 2 ISR.
		*/
		os_isr_t const * const isr = &OS_isrTableBase[kernel_data->isrCurrent];
		app = OS_GET_APP(isr->app);

		if ( OS_AppIsTrusted(app) )
		{
			/* Trusted ISRs can write anywhere
			*/
			result = OS_TRUE;
		}
		else
		if ( OS_IS_WITHIN_APP_REGION(first, last) )
		{
			/* Non-trusted ISRs can write to their application's common
			 * data area.
			*/
			result = OS_TRUE;
		}
		else
		if ( OS_IS_WITHIN_ISR_REGION(first, last) )
		{
			/* Non-trusted ISRs can write to their own private data
			 * area.
			*/
			result = OS_TRUE;
		}
		else
		if ( (OS_GetIsrStackLimit(isr->dynamic) != OS_NULL) &&
			 (first >= ((os_address_t)OS_GetIsrStackLimit(isr->dynamic) - isr->stackLen)) &&
			 (last <= (os_address_t)OS_GetIsrStackLimit(isr->dynamic)) )
		{
			/* Note: the condition above is not a MISRA violation, no matter what your checker claims. */

			/* Non-trusted ISRs can write to their own private stack area.
			*/
			result = OS_TRUE;
		}
		else
		{
			/* Fails (result is OS_FALSE) */
		}
	}
	else
	{
		/* Called from some other context. All contexts other than tasks
		 * and category 2 ISRs are presumed to be privileged.
		*/
		result = OS_TRUE;
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
