/* kern-sendenableinterruptsource.c
 *
 * This file contains the OS_SendEnableInterruptSource() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendenableinterruptsource.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendEnableInterruptSource
 *
 * Sends the EnableInterruptSource() request to the core on which the ISR 'i' resides.
*/
os_errorresult_t OS_SendEnableInterruptSource(os_isrid_t i, const os_isr_t *isr)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)i;

	return OS_SendMessageWait(OS_GetIsrCoreId(isr), OS_XC_OP_EnableInterruptSource, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
