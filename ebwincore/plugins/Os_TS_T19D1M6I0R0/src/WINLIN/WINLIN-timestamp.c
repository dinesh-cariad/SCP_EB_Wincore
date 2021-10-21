/* WINLIN-timestamp.c
 *
 * This file contains the implementation of the WINLIN time-stamp timer.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-timestamp.c 30052 2019-03-27 14:30:23Z olme8414 $
 */

#include <Os_configuration.h>
#include <Os_kernel.h>
#include <Os_tool.h>

#include <memmap/Os_mm_var_begin.h>

/** \brief This is the time at which the time stamp timer was started.
 *
 * All time stamps returned by this implementation are relative to this
 * point in time.
 */
static os_winlinTimerValue_t OS_winlinTimeStampInitializationTime = 0;

/** \brief The performance counter frequency [Hz] offered by Windows.
 *
 * This variable is just a shortcut and helps to avoid recurring calls of
 * OS_WINLIN_OSAL_QueryPerformanceFrequency().
 *
 * The special value 0 means one of the following:
 * -	The time stamp counter hasn't been started yet. The time stamp
 *		timer must be started before its first use.
 * -	Windows does not offer a high-performance counter. In this case,
 *		use the generic implementation.
 */
static os_winlinTimerValue_t OS_winlinPerfCounterFreq = 0;

/** \brief The performance counter period [nanoseconds].
 *
 * This value is derived from OS_winlinPerfCounterFreq at initialization time.
 */
static os_double_t OS_winlinPerfCounterPeriodInNs = 0.0;

#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_code_begin.h>

/* See generic implementation for documentation: kern-gettimestampgeneric.c. */
void OS_WINLINGetTimeStamp(os_timestamp_t* timeStamp)
{
	/* Check if time stamp has been initialized and is available on this platform. */
	OS_WINLINAssert(OS_winlinPerfCounterFreq != 0);

	{
		os_winlinTimerValue_t currentTime = OS_WINLIN_OSAL_QueryPerformanceCounter();
		if (currentTime < OS_winlinTimeStampInitializationTime)
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_ERROR,
					"Windows-specific timestamp counter overflowed.",
					OS_WINLINLOG_UNUSED_PARAM);
			OS_WINLINPanic();
		}
		/* Make current time relative to initialization time. */
		currentTime -= OS_winlinTimeStampInitializationTime;
		/* Turn into a rounded-up nanoseconds value. */
		currentTime = (os_winlinTimerValue_t)(
				((os_double_t)currentTime * OS_winlinPerfCounterPeriodInNs) + 0.5);

		currentTime /= OS_winlinTimeWarpFactor;

#if OS_ARCH_HAS_64BIT
		*timeStamp = currentTime;
#else
		timeStamp->tsHi = (os_uint32_t)((os_winlinUnsignedTimerValue_t)currentTime >> 32U);
		timeStamp->tsLo = (os_uint32_t)(currentTime);
#endif
	}
}

/* See generic implementation for documentation: kern-starttimestamptimergeneric.c. */
void OS_WINLINInitTimeStamp(void)
{
	/* This guard protects against multiple initializations. */
	if (OS_winlinTimeStampInitializationTime == 0)
	{
		/* If the hardware does not support high-resolution performance counters the
		 * following functions fail. A user is informed about any failures by virtue
		 * of OS_WINLIN_OSAL_SyscallFailed().
		 * In this case, use the generic implementation of the timer stamp timer instead,
		 * i.e. select TIMER0, TIMER1, TIMER2 or TIMER3 instead and not INTERNAL.
		 */
		OS_winlinTimeStampInitializationTime = OS_WINLIN_OSAL_QueryPerformanceCounter();
		OS_winlinPerfCounterFreq = OS_WINLIN_OSAL_QueryPerformanceFrequency();
		OS_winlinPerfCounterPeriodInNs = 1e9 / (os_double_t)OS_winlinPerfCounterFreq;
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
