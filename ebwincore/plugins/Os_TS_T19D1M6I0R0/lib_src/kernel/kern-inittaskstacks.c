/* kern-inittaskstacks.c
 *
 * This file contains the OS_InitTaskStacks() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-inittaskstacks.c 27282 2017-08-24 13:57:43Z olme8414 $
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
 * OS_InitTaskStacks()
 *
 * Set all task stacks' content to OS_STACKFILL.
*/
void OS_InitTaskStacks(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_taskid_t tid;

	for (tid = 0; tid < OS_nTasks; tid++)
	{
		os_task_t const * const tp = &OS_taskTableBase[tid];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetTaskCoreId(tp) == myCoreId)
		{
			/* Previously there was some code here that omitted to initialise stacks that were
			 * already initialised ( "if ( *p == OS_STACKFILL )" ), but that is not guaranteed
			 * to work, especially around a warm reboot.
			 * So we've removed the test, and now all stacks are initialised even if they've
			 * already been initialised. The disadvantage is a small increase in startup time.
			 * The old code also didn't take account of the fact that the shared stacks might
			 * be of different sizes.
			*/
			os_stackelement_t * p = tp->stackBase;
			os_size_t numElems = tp->stackLen / sizeof(os_stackelement_t);
			while ( numElems > 0 )
			{
				*p = OS_STACKFILL;
				p++;
				numElems--;
			}
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
