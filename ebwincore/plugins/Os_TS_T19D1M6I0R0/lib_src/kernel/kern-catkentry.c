/* kern-catkentry.c
 *
 * This file contains the CatK interrupt handler wrapper.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-catkentry.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_CatKEntry() - call a Category K (Kernel) ISR.
 *
 * This function calls a category K (Kernel) ISR.
 *
 * The previous value of inKernel is saved, then inKernel is set
 * to 1.
 * Then the ISR function itself is called.
 * Finally, the saved value of inKernel is returned. The real inKernel
 * flag is left at 1 because we might need to reschedule in the exit
 * routine.
 *
 * Interrupts remain at the kernel lock level priority throughout. The run priority
 * is not used for category K interrupts.
*/
os_uint8_t OS_CatKEntry(os_isrid_t iid)
{
	const os_isr_t *isr;
	os_isrfunc_t f;
	os_uint8_t inKernel;

	OS_CPULOAD_GENERICCAT2_ENTRY();

	isr = &OS_isrTableBase[iid];
	f = isr->func;

	inKernel = OS_GetKernelData()->inKernel;
	OS_SetIsrinKernel();

	(*f)();

	return inKernel;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
