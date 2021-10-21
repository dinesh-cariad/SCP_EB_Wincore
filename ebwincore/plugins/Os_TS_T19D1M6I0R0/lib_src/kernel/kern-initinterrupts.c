/* kern-initinterrupts.c
 *
 * This file contains the implementation of the function to initialize ISRs.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initinterrupts.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the number of configured cores.
 *   Only in case of a single core configuration, this comparison can be evaluated at compile time.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitInterrupts()
 *
 * This function initializes all the interrupt sources configured by the
 * OS generator.
*/
void OS_InitInterrupts(void)
{
	os_isrid_t i;
	os_coreid_t const myCoreId = OS_GetMyCoreId();

	for (i = 0; i < OS_nInterrupts; i++)
	{
		os_isr_t const * const isr = &OS_isrTableBase[i];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetIsrCoreId(isr) == myCoreId)
		{
			os_isrdynamic_t * const isrd = isr->dynamic;

			OS_SetupIsr(isr);

			if ( (isr->flags & OS_ISRF_ENABLE) != 0 )
			{
				OS_EnableIsr(isr);
			}
			else
			{
				isrd->statusflags = OS_ISRF_BLOCKED;
			}
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
