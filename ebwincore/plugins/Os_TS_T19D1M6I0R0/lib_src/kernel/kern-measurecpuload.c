/* kern-measurecpuload.c
 *
 * This file contains the OS_MeasureCpuLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-measurecpuload.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#ifndef OS_EXCLUDE_CPULOAD

#include <Os_timestamp.h>
#include <Os_cpuload_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_MeasureCpuLoad
 *
 * This function measures the CPU load. It is called on entry to the idle loop. Interrupts
 * must be fully disabled so that no interrupts of any category can occur during its execution.
 *
 * CPU load is measured by noting the times of entry and exit to the idle loop.
 *
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement.Execution, 1
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement.Execution.Load, 1
*/
void OS_MeasureCpuLoad(void)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_cpuload_t * const cpuLoad = kernel_data->cpuLoad;
	os_timestamp_t idleEntryTime;
	os_timestamp_t intervalEndTime;
	os_intstatus_t is;

	/* Disables interrupts for CPU load measurement. This ensures that the idle time is correctly measured.
	 * If the idle time period could be interrupted by ISRs, of course there would be an error in the measurement.
	 * Depending on the presence of execution time budget monitoring and/or cross-core ISRs the level
	 * OS_kernDisableLevel may be larger than OS_intDisableLevelAll. The larger one must be used to properly disable
	 * all interrupts.
	 */
	is = OS_IntDisableMax();

	/* Get the time of entry to the idle loop. All computation done after this is therefore in the
	 * "idle time" and doesn't count as part of the CPU load.
	*/
	OS_GetTimeStamp(&idleEntryTime);

	/* If the idle exit time is zero, something went wrong.
	 * Should we report the error?
	 * In any case, assume a zero-length busy time to prevent it skewing the calculations
	*/
	if ( OS_TimeIsZero(cpuLoad->idleExitTime) )
	{
		OS_TimeCopy(&cpuLoad->idleExitTime, idleEntryTime);
	}

	/* First, we compute the time measurement for all intervals that have completed since the last time we were here.
	*/
	OS_TimeAdd32(&intervalEndTime, cpuLoad->intervalStartTime, OS_cpuLoadCfg.intervalDuration);

	while ( OS_TimeGe(idleEntryTime, intervalEndTime) )
	{
		if ( OS_TimeLt(cpuLoad->idleExitTime, intervalEndTime) )
		{
			/* Part of the last busy interval was in the old measurement interval.
			 * That portion is added to the busy time before the load calculation, and busy interval for the
             * new measurement interval is* deemed to start at the start of the interval.
			*/
			cpuLoad->busyTime += OS_TimeSubTickT(intervalEndTime, cpuLoad->idleExitTime);
			OS_TimeCopy(&cpuLoad->idleExitTime, intervalEndTime);
		}

		/* Slide the window along one place...
		*/
		cpuLoad->busyIndex++;
		if ( cpuLoad->busyIndex >= OS_cpuLoadCfg.nIntervals )
		{
			cpuLoad->busyIndex = 0;
		}

		/* Remove the old busyTime from the new slot out of the calculation of the average.
		*/
		cpuLoad->busyTimeSum -= cpuLoad->busyBuffer[cpuLoad->busyIndex];

		/* Put the new busy time into the slot and bring it into the calculation of the average.
		*/
		cpuLoad->busyBuffer[cpuLoad->busyIndex] = cpuLoad->busyTime;
		cpuLoad->busyTimeSum += cpuLoad->busyTime;

		/* Calculate the new average load over the last nIntervals slots. If that's a peak, remember it.
		*/
		if ( cpuLoad->busyTimeSum < OS_cpuLoadCfg.busyOverflowLimit )
		{
			cpuLoad->currentLoad =
					(os_uint8_t)(((cpuLoad->busyTimeSum * 100) + OS_cpuLoadCfg.rounding)
							/ OS_cpuLoadCfg.windowDuration);
		}
		else
		{
			cpuLoad->currentLoad =
					(os_uint8_t)((cpuLoad->busyTimeSum + OS_cpuLoadCfg.rounding100)
							/ OS_cpuLoadCfg.windowDuration100);
		}

		if ( cpuLoad->currentLoad > cpuLoad->peakLoad )
		{
			cpuLoad->peakLoad = cpuLoad->currentLoad;
		}

		/* Restart busy time from zero for the next loop or idle-entry
		*/
		cpuLoad->busyTime = 0;

		/* Calculate the start and end of the next interval.
		 * If the end time is in the future the loop terminates.
		*/
		OS_TimeCopy(&cpuLoad->intervalStartTime, intervalEndTime);
		OS_TimeAdd32(&intervalEndTime, cpuLoad->intervalStartTime, OS_cpuLoadCfg.intervalDuration);
	}

	/* idleExitTime is now in the current interval
	*/
	cpuLoad->busyTime += OS_TimeSubTickT(idleEntryTime, cpuLoad->idleExitTime);

	OS_TimeZero(&cpuLoad->idleExitTime);

	OS_IntRestoreMax(is);
}

#include <memmap/Os_mm_code_end.h>

#endif	/* OS_EXCLUDE_CPULOAD */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
