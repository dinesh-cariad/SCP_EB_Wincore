/* kern-multisetevent.c
 *
 * This file contains the OS_MultiSetEvent function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-multisetevent.c 27818 2017-11-08 16:44:06Z mist9353 $
*/

#include <Os_kernel.h>

#if (OS_N_CORES == 1)
#error "This file is only required for multi-core systems - please check your makefiles."
#endif
#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "Tasks are implemented by the microkernel - please check your makefiles."
#endif

#include <Os_messagequeue.h>


#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_MultiSetEvent is used to implement SetEvent on multi-core systems.
*/
os_errorresult_t OS_MultiSetEvent(os_taskid_t t, const os_task_t *ts, os_eventmask_t evt)
{
	os_errorresult_t e;

	if (OS_GetTaskCoreId(ts) == OS_GetMyCoreId())
	{
		e = OS_LocalSetEvent(ts, evt);
	}
	else
	{
		/* !LINKSTO Kernel.Autosar.Multicore.SetEvent.CrossCore, 1
		*/
		e = OS_SendSetEvent(t, ts, evt);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
