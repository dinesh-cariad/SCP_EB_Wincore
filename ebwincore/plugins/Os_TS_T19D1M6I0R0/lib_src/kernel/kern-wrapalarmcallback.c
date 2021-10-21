/* kern-wrapalarmcallback.c
 *
 * This file contains the OS_WrapAlarmCallback wrapper function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-wrapalarmcallback.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#define OS_SID	OS_SID_HookHandler
#define OS_SIF	OS_svc_HookHandler

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_WrapAlarmCallback
 *
 * This function calls the AlarmCallback function specified in the callback
 * part of the alarm parameter union.
 *
 * !LINKSTO Kernel.API.Alarms.SetRelAlarm.ActionAlarmCallback, 1
 * !LINKSTO Kernel.API.Alarms.SetAbsAlarm.ActionAlarmCallback, 1
*/
void OS_WrapAlarmCallback(const os_alarm_t *a)
{
	os_intstatus_t is;

#if OS_KERNEL_TYPE!=OS_MICROKERNEL
	os_uint8_t saveInFunction;
	os_uint8_t saveNestSuspendAll;
	os_oldlevel_t saveOldSuspendAll;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	saveInFunction = kernel_data->inFunction;
	kernel_data->inFunction = OS_INACB;
#endif

	/* !LINKSTO Kernel.API.Alarms.SetRelAlarm.ActionAlarmCallbackISR, 1
	 * !LINKSTO Kernel.API.Alarms.SetAbsAlarm.ActionAlarmCallbackISR, 1
	 *
	 * OS v2.1 now permits Suspend/Resume/Disable/Enable-All/OS-Interrupts
	 * inside alarm callback functions. The 'OS' variants are NO-OPs because
	 * Cat2 interrupts are already locked. However, we must protect the
	 * 'All' variants against abuse.
	 *
	 * Disabling and then restoring ensures that the real state of the
	 * interrupt lock is restored afterwards.
	 * Savings and then restoring the old level and nesting count ensures
	 * that the ACB has a clean slate.
	 * Finally, setting the nesting count to zero means we can detect
	 * incorrect nesting.
	*/
	is = OS_ForceIntDisable();

#if OS_KERNEL_TYPE!=OS_MICROKERNEL
	saveNestSuspendAll = kernel_data->nestSuspendAll;
	saveOldSuspendAll = kernel_data->oldSuspendAll;
	kernel_data->nestSuspendAll = 0;
#endif

	/* Look up the alarm callback function in the table of callbacks and call it
	*/
	(*OS_alarmCallback[a->object])();

#if OS_KERNEL_TYPE!=OS_MICROKERNEL
	if ( kernel_data->nestSuspendAll != 0 )
	{
		/* Calls to Suspend/ResumeAllInterrupts() inside the
		 * error hook function were not correctly nested.
		*/
		/* can't propagate the return value of OS_ERROR -> ignore it */
		(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
	}

	kernel_data->nestSuspendAll = saveNestSuspendAll;
	kernel_data->oldSuspendAll = saveOldSuspendAll;
#endif

	OS_ForceIntRestore(is);

#if OS_KERNEL_TYPE!=OS_MICROKERNEL
	kernel_data->inFunction = saveInFunction;
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
