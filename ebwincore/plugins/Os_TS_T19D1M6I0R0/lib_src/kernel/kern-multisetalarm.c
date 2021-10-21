/* kern-multisetalarm.c
 *
 * This file contains the OS_MultiSetAlarm function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-multisetalarm.c 27818 2017-11-08 16:44:06Z mist9353 $
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
 * OS_MultiSetAlarm is used to implement SetAlarm on multi-core systems.
*/
os_errorresult_t OS_MultiSetAlarm(
	os_alarmid_t a,
	const os_alarm_t *as,
	const os_counter_t *cs,
	os_tick_t val,
	os_tick_t cyc,
	os_boolean_t rel)
{
	os_errorresult_t e;

	if (OS_GetAlarmCoreId(as) == OS_GetMyCoreId())
	{
		e = OS_LocalSetAlarm(a, as, cs, val, cyc, rel);
	}
	else
	{
		if (rel)
		{
			/* !LINKSTO Kernel.Autosar.Multicore.SetRelAlarm.CrossCore, 1
			*/
			e = OS_SendSetRelAlarm(a, as, val, cyc);
		}
		else
		{
			/* !LINKSTO Kernel.Autosar.Multicore.SetAbsAlarm.CrossCore, 1
			*/
			e = OS_SendSetAbsAlarm(a, as, val, cyc);
		}
	}

	return e;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
