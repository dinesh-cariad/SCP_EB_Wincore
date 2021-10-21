/* kern-multiactivatetask.c
 *
 * This file contains the OS_MultiActivateTask function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-multiactivatetask.c 27818 2017-11-08 16:44:06Z mist9353 $
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
 * OS_MultiActivateTask activates a task in a multi-core system.
*/
os_errorresult_t OS_MultiActivateTask(const os_task_t *tp)
{
	os_errorresult_t e;

	if (OS_GetTaskCoreId(tp) == OS_GetMyCoreId())
	{
		e = OS_LocalActivateTask(tp);
	}
	else
	{
		/* !LINKSTO Kernel.Autosar.Multicore.ActivateTask.CrossCore, 1
		 * !LINKSTO Kernel.Autosar.Multicore.ChainTask.CrossCore, 1
		*/
		e = OS_SendActivateTask(tp);
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
