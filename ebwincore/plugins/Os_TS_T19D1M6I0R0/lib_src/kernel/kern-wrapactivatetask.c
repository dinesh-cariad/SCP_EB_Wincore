/* kern-wrapactivatetask.c
 *
 * This file contains the OS_WrapActivateTask wrapper function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-wrapactivatetask.c 27818 2017-11-08 16:44:06Z mist9353 $
*/
#define OS_SID  OS_SID_ActivateTask
#define OS_SIF  OS_svc_ActivateTask

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_WrapActivateTask
 *
 * This function calls the kernel OS_KernActivateTask function with
 * a task id as specified in the task-event part of the alarm parameter union.
 *
 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.ActionActivateTask, 1
 * !LINKSTO Kernel.API.Alarms.SetAbsAlarm.ActionActivateTask, 1
*/
void OS_WrapActivateTask(const os_alarm_t *a)
{
#if OS_KERNEL_TYPE==OS_MICROKERNEL
	MK_UsrActivateTask(a->object);
#else
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_taskid_t t = (os_taskid_t)a->object;
	const os_task_t *tp = &OS_taskTableBase[t];
	os_uint8_t save;
	os_errorresult_t e;
	OS_PARAMETERACCESS_DECL

	save = kernel_data->inFunction;
	kernel_data->inFunction = OS_ININTERNAL;

	/* !LINKSTO Kernel.Autosar.Multicore.ExpiryActions.ActivateTask, 1
	*/
	e = OS_DoActivateTask(tp);
	if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
	{
		OS_SAVE_PARAMETER_N(0,t);

		/* no way to propagate errors -> ignore return value */
		(void) OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
	}

	kernel_data->inFunction = save;
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
