/* kern-sendchainscheduletable.c
 *
 * This file contains the OS_SendChainScheduleTable() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendchainscheduletable.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendChainScheduleTable
 *
 * Sends the ChainScheduleTable() request to the core on which the schedule table 'sc' resides.
*/
os_errorresult_t OS_SendChainScheduleTable
(	os_scheduleid_t sc,			/* Currently-running schedule table */
	os_scheduleid_t sn,			/* Schedule table to chain */
	const os_schedule_t *scs	/* Ptr to sc structure */
)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)sc;
	msgParam[1] = (os_paramtype_t)sn;

	return OS_SendMessageWait(OS_GetScheduleTableCoreId(scs), OS_XC_OP_ChainScheduleTable, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
