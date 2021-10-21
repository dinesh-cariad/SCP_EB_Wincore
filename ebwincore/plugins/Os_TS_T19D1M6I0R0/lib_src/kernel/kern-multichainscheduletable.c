/* kern-multichainscheduletable.c
 *
 * This file contains the OS_MultiChainScheduleTable function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-multichainscheduletable.c 27818 2017-11-08 16:44:06Z mist9353 $
*/

#include <Os_kernel.h>

#if (OS_N_CORES == 1)
#error "This file is only required for multi-core systems - please check your makefiles."
#endif
#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "This file is not required if running on microkernel - please check your makefiles."
#endif

#include <Os_messagequeue.h>


#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_MultiChainScheduleTable chains a schedule table in a multi-core system.
*/
os_errorresult_t OS_MultiChainScheduleTable(
	os_scheduleid_t sc,
	os_scheduleid_t sn,
	const os_schedule_t *scs)
{
	os_errorresult_t e;

	if (OS_GetScheduleTableCoreId(scs) == OS_GetMyCoreId())
	{
		e = OS_LocalChainScheduleTable(sc, sn);
	}
	else
	{
		e = OS_SendChainScheduleTable(sc, sn, scs);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
