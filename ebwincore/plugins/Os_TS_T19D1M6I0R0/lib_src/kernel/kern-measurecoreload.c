/* kern-measurecoreload.c
 *
 * This file contains the OS_MeasureCoreLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-measurecoreload.c 24097 2016-04-29 13:23:20Z mist9353 $
*/

#include <Os_kernel.h>
#ifndef OS_EXCLUDE_CPULOAD

#include <Os_cpuload_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_MeasureCoreLoad
 *
 * Measures the current or peak load on the calling core.
 *
 * Before returning the value, we update to include the current busy interval. To perform
 * the computation we "enter" the idle state and then "leave" it again.
 *
 * NOTE: this function must not clear the busyNestingCounter because we enter idle mode only temporarily,
 * and the nesting must remain correct when we return to busy mode at the end. For this reason we don't
 * use OS_LeaveIdleState() either.
 *
 * If this function is called with an unknown measurementType,
 * it returns a cpu load, which is above 100%, to indicate an error.
 */
os_uint8_t OS_MeasureCoreLoad(os_cpuloadmeasurementtype_t measurementType)
{
	os_cpuload_t * const cpuLoad = OS_GetKernelData()->cpuLoad;
	os_uint8_t result;
	os_intstatus_t is;

	is = OS_IntDisableMax();

	OS_MeasureCpuLoad();
	OS_LeaveIdleState();

	if (measurementType == OS_CPULOAD_GET_CURRENT)
	{
		result = cpuLoad->currentLoad;
	}
	else if (measurementType == OS_CPULOAD_GET_PEAK)
	{
		result = cpuLoad->peakLoad;
	}
	else if (measurementType == OS_CPULOAD_RESET_PEAK)
	{
		cpuLoad->peakLoad = cpuLoad->currentLoad;
		result = cpuLoad->currentLoad;
	}
	else
	{
		/* Unknown measurementType - return an invalid CPU load.*/
		result = 0xffu;
	}

	OS_IntRestoreMax(is);

	return result;
}

#include <memmap/Os_mm_code_end.h>

#endif	/* OS_EXCLUDE_CPULOAD */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
