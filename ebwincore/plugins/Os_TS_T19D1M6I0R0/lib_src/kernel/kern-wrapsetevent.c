/* kern-wrapsetevent.c
 *
 * This file contains the OS_WrapSetEvent wrapper function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-wrapsetevent.c 27818 2017-11-08 16:44:06Z mist9353 $
*/
#define OS_SID OS_SID_SetEvent
#define OS_SIF OS_svc_SetEvent

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_WrapSetEvent
 *
 * This function calls the kernel OS_KernSetEvent function with
 * a task id and event mask as specified in the taskevent part of the alarm
 * parameter union.
 *
 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.ActionSetEvent, 1
 * !LINKSTO Kernel.API.Alarms.SetAbsAlarm.ActionSetEvent, 1
*/
void OS_WrapSetEvent(const os_alarm_t *a)
{
#if OS_KERNEL_TYPE==OS_MICROKERNEL

	MK_UsrSetEvent(a->object, a->event);

#else
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_taskid_t t = (os_taskid_t) a->object;
	const os_task_t *ts = &OS_taskTableBase[t];
	const os_eventmask_t evt = a->event;
	os_uint8_t save;
	os_errorresult_t e;
	OS_PARAMETERACCESS_DECL

	save = kernel_data->inFunction;
	kernel_data->inFunction = OS_ININTERNAL;

	/* !LINKSTO Kernel.Autosar.Multicore.ExpiryActions.SetEvent, 1
	*/
	e = OS_DoSetEvent(t, ts, evt);
	if ( e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError) )
	{
		OS_SAVE_PARAMETER_N(0,(os_paramtype_t)t);
		OS_SAVE_PARAMETER_N(1,(os_paramtype_t)evt);

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
