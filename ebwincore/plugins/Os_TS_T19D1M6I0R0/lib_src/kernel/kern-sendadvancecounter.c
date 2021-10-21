/* kern-sendadvancecounter.c
 *
 * This file contains the OS_SendAdvanceCounter() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendadvancecounter.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendAdvanceCounter
 *
 * Sends the AdvanceCounter() request to the core on which the counter 'c' resides.
*/
os_errorresult_t OS_SendAdvanceCounter(os_counterid_t c, const os_counter_t *cs, os_tick_t incr)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)c;
	msgParam[1] = (os_paramtype_t)incr;

	return OS_SendMessageWait(OS_GetCounterCoreId(cs), OS_XC_OP_AdvanceCounter, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
