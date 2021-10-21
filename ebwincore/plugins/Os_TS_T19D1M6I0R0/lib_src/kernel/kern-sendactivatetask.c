/* kern-sendactivatetask.c
 *
 * This file contains the OS_SendActivateTask() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendactivatetask.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendActivateTask()
 *
 * Sends a message to the core the task with ID 't' is running on to activate it.
*/
os_errorresult_t OS_SendActivateTask(const os_task_t *tp)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t) tp->taskId;

	/* !LINKSTO Kernel.Autosar.Multicore.ActivateTask.Synchronous, 1
	*/
	return OS_SendMessageWait(OS_GetTaskCoreId(tp), OS_XC_OP_ActivateTask, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
