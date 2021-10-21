/* kern-initmeasurecpuload.c
 *
 * This file contains the OS_InitMeasureCpuLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initmeasurecpuload.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#ifndef OS_EXCLUDE_CPULOAD

#include <Os_timestamp.h>
#include <Os_cpuload_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_InitMeasureCpuLoad
 *
 * This function initialises the CPU load measurement system.
 *
 * It is the "startup" initialisation, it does NOT perform the functionality of the
 * API InitCpuLoad(), which merely resets the peak detector.
 *
 * !LINKSTO  Kernel.Feature.CpuLoadMeasurement.API.InitCpuLoad, 1
*/
void OS_InitMeasureCpuLoad(void)
{
	os_cpuload_t * const cpuLoad = OS_GetKernelData()->cpuLoad;
	os_int_t i;

	OS_GetTimeStamp(&cpuLoad->idleExitTime);
	OS_TimeCopy(&cpuLoad->intervalStartTime, cpuLoad->idleExitTime);
	cpuLoad->busyNestingCounter = 1;
	cpuLoad->busyTime = 0;
	cpuLoad->busyTimeSum = 0;
	cpuLoad->busyIndex = OS_cpuLoadCfg.nIntervals;
	cpuLoad->currentLoad = 0;
	cpuLoad->peakLoad = 0;
	cpuLoad->busyBuffer = OS_cpuLoadBusyBuffer_ptr[OS_GetMyCoreId()];

	for ( i = 0; i < OS_cpuLoadCfg.nIntervals; i++ )
	{
		cpuLoad->busyBuffer[i] = 0;
	}
}

#include <memmap/Os_mm_code_end.h>
#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
