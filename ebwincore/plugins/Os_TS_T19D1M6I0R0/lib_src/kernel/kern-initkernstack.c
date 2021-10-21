/* kern-initkernstack.c
 *
 * This file contains the OS_InitKernStack() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initkernstack.c 31085 2019-11-21 07:49:57Z masa8317 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 18.4 (advisory)
 * The +, -, += and -= operators should not be applied to an expression of
 * pointer type.
 *
 * Reason:
 * Pointer arithmetic is used for classical tasks like initializing the
 * stack to make them more readable and maintainable.
*/

#include <Os_kernel.h>

/*!
 * OS_GetNumOfUnusedStackElements()
 *
 * Determines the number of unused stack elements based on the given
 * parameters the the direction of growth.
 * If necessary, this can be defined in the architecture header file.
 *
 * Scenario 0: stack grows up
 * --------------------------
 * Elements 0, 1, 2, 3 are used, only element 4 is still unused yet, hence
 * numElemsTotal  = 5 (== iStackLen / sizeof(os_stackelement_t))
 * numElemsUnused = 1
 * --- <-- stack limit
 * |4|
 * |3|<-- current SP
 * |2|
 * |1|
 * |0|<-- iStackBase
 *
 * Scenario 1: stack grows down
 * ----------------------------
 * Elements 3, 4 are used, only elements 0, 1, 2 are still unused, hence
 * numElemsTotal  = 5 (== iStackLen / sizeof(os_stackelement_t))
 * numElemsUnused = 3
 * --- <-- stack limit
 * |4|
 * |3|<-- current SP
 * |2|
 * |1|
 * |0|<-- iStackBase
*/
#ifndef OS_GetNumOfUnusedStackElements
#if (OS_STACKGROWS == OS_STACKGROWSDOWN)

#define OS_GetNumOfUnusedStackElements(StackBase, StackLen, CurrentSp) \
	( ((CurrentSp) - (StackBase)) )

#else

#define OS_GetNumOfUnusedStackElements(StackBase, StackLen, CurrentSp) \
	( ( ((StackBase) + (StackLen)) - (CurrentSp) ) - 1 )

#endif /* OS_STACKGROWS == OS_STACKGROWSDOWN */
#endif /* defined(OS_GetNumOfUnusedStackElements) */

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitKernStack()
 *
 * Set the kernel stack's content to OS_STACKFILL.
*/
void OS_InitKernStack(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_size_t i;
	os_size_t numElemsTotal;
	os_size_t numElemsUnused;
	/* These two variables are volatile to keep the compiler from optimizing
	 * the stack-fill loop such that it overwrites used parts of the stack.
	*/
	volatile os_stackelement_t * p;
	volatile os_stackelement_t *sp;

	/* Initialize the kernel/interrupt stack. We're running in that stack,
	 * so we have to be very careful not to go past where we currently are.
	 * So numElemsTotal is the number of stackelements in the stack and
	 * numElemsUnused is the number of unused stackelelements.
	*/
	p = kernel_data->iStackBase;		/* lowest stack address */
	sp = (os_stackelement_t *)OS_GetCurrentSp();
	numElemsTotal = kernel_data->iStackLen / sizeof(os_stackelement_t);
	numElemsUnused = OS_GetNumOfUnusedStackElements(p, numElemsTotal, sp);

#if OS_STACKGROWS == OS_STACKGROWSDOWN
	/* filling from lowest to highest address */
#else
	/* filling from highest to lowest address */
	/* Deviation MISRAC2012-1 */
	p += numElemsTotal - 1u;			/* calculate highest stack address */
#endif

	/* This check allows certain architectures to enforce the initialization of the whole stack.
	 * Usually numElemsUnused <= numElemsTotal holds true. By violating this condition, an architecture
	 * can cause the whole stack to be initialized.
	*/
	if (numElemsTotal < numElemsUnused)
	{
		numElemsUnused = numElemsTotal;
	}

	for ( i = 0; i < numElemsUnused; i++ )
	{
		*p = OS_STACKFILL;
#if OS_STACKGROWS == OS_STACKGROWSDOWN
		p++;
#else
		p--;
#endif
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
