/* kern-triggerinterrupt.c
 *
 * This file contains the OS_TriggerInterrupt function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-triggerinterrupt.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_TriggerInterrupt triggers the interrupt source of the specified ISR.
 *
 * If the ISR is not valid, the command is silently ignored.
*/
void OS_TriggerInterrupt(os_isrid_t i)
{
	const os_isr_t *isr;

	if ( i < OS_nInterrupts )
	{
		isr = &OS_isrTableBase[i];

		OS_TriggerIsr(isr);
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
