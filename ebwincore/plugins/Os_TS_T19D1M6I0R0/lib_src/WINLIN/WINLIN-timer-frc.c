/* WINLIN-timer-frc.c - WINLIN timer driver for AutosarOS
 *
 * This file contains the FRC implementation for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-timer-frc.c 27650 2017-10-17 12:20:25Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_tool.h>
#include <WINLIN/Os_WINLIN_timer.h>
#include <WINLIN/Os_WINLIN_critsec.h>
#include <stdlib.h>

static void OS_FrcTimeEndPeriod(void);
#if (OS_ARCH == OS_WINDOWS)
static os_winlinDword_t OS_WINDOWS_STDCALL OS_FrcThread(void*);
#elif (OS_ARCH == OS_LINUX)
static void* OS_FrcThread(void*);
#endif

/** \brief Represents a free running counter (FRC). */
struct os_frcdescr_s
{
	/** \brief The handle of the waitable timer. */
	os_winlinHandle_t waitableTimer;
};
typedef struct os_frcdescr_s os_frcdescr_t;

/** \brief The number of FRCs available. */
#define OS_WINLIN_NUMBER_OF_FRCS 4

/** \brief Gets the FRC of the given hardware timer.
 *
 * \param[in] pHwt		Points to an constant instance of os_hwt_t, which represents
 *						the hardware timer of which the FRC shall be returned.
 *
 * \return Pointer to the FRC connected with \a pHwt.
 */
#define OS_WINLINGetFrcOfHwt(pHwt) (&(OS_frcDescr[(pHwt)->major][(pHwt)->minor]))

#include <memmap/Os_mm_var_begin.h>

/** \brief The thread ID of \c OS_FrcThread. */
static os_winlinTid_t OS_frcThreadId;

/** \brief This event signals the end of the FRC thread initialization.
 *
 * During global initialization OS_FrcGlobalInit() this event is used to determine,
 * when the FRC thread has finished its initialization. It thereby marks the end of
 * the global initialization.
 */
static os_winlinHandle_t OS_frcThreadInitializedEvent;

/** \brief The free running counter instances.
 *
 * On WINLIN there are four free running counters (FRC).
 * The major number identifies the timer instance. The minor number has to be always 0,
 * because there is only one "comparator" per timer. This comparator concept was introduced
 * by other architectures.
 */
static os_frcdescr_t OS_frcDescr[OS_WINLIN_NUMBER_OF_FRCS][1];

/** \brief The time in milliseconds at which the AUTOSAR system started.
 *
 * This is the point in time provided by WINLIN using OS_WINLIN_OSAL_TimeGetTime()
 * at which the AUTOSAR system was started.
 * FRCs takes this value as a reference to determine the time
 * elapsed since startup. It's comparable to the reset of counter registers
 * in real timer hardware.
 *
 * It's the time in milliseconds at which the AUTOSAR application started.
 * This time begins to run when host OS starts. Hence, this variable contains
 * the number of milliseconds Windows has run before the AUTOSAR application
 * was started.
 */
static os_winlinDword_t OS_frcInitializationTime = 0;

/** \brief Minimum time period.
 *
 * This is the value obtained from OS_FrcTimeEndPeriod() at initialization
 * time. It represents the smallest possible time resolution is milliseconds.
 */
static os_uint_t OS_frcMinimumTimePeriod = 0;

#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_const_begin.h>

/** \brief FRC driver table.
 *
 * This table tells the kernel about the FRC driver's functions.
 * It contains the publicly callable functions to control a FRC.
 */
const os_hwtdrv_t OS_frcDriver =
{
	&OS_FrcInit,
	&OS_FrcRead,
	&OS_FrcStart,
	&OS_FrcStop
};

#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Ends the period of increased time resolution.
 *
 * This function is intended to be called by the OS_WINLIN_OSAL_AtExit() mechanism
 * at application termination. It complements the call to OS_WINLIN_OSAL_TimeBeginPeriod()
 * during FRC initialization.
 */
static void OS_FrcTimeEndPeriod(void)
{
	if (OS_frcMinimumTimePeriod != 0)
	{
		OS_WINLIN_OSAL_TimeEndPeriod(OS_frcMinimumTimePeriod);
	}
}

/** \brief Thread for all FRCs.
 *
 * This is the thread responsible of operating all FRCs in OS_frcDescr[], i.e. in this AUTOSAR system.
 *
 * \param args		Not used.
 *
 * \return Always returns OS_NULL.
 */
 
#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
#endif

#if (OS_ARCH == OS_WINDOWS)
static os_winlinDword_t OS_WINDOWS_STDCALL OS_FrcThread(void *args)
#elif (OS_ARCH == OS_LINUX)
static void *OS_FrcThread(void *args)
#endif
{
	os_hwt_t const * pHwt[OS_WINLIN_NUMBER_OF_FRCS];
	os_winlinHandle_t handles[OS_WINLIN_NUMBER_OF_FRCS];
	OS_PARAM_UNUSED(args);

	OS_WINLINInitializeFrcThread(&OS_hwTimerTableBase[0], &OS_frcThreadId);
	{
		os_uint_t i;
		for (i = 0; i < OS_nHwTimers; ++i)
		{
			os_hwt_t const * const td = &OS_hwTimerTableBase[i];
			os_frcdescr_t * const fd = OS_WINLINGetFrcOfHwt(td);
			OS_WINLIN_OSAL_CreateWaitableTimer(&fd->waitableTimer, OS_frcThreadId);

			pHwt[i] = td;
			handles[i] = fd->waitableTimer;
		}

		/* The FRC thread initialization is done. The thread executing OS_FrcGlobalInit() may now continue. */
		OS_WINLIN_OSAL_SetEvent(OS_frcThreadInitializedEvent);
	}

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"FRC thread %#x=%d started.",
			OS_frcThreadId,
			OS_frcThreadId);

	while (1)
	{
		os_winlinDword_t const idx = OS_WINLIN_OSAL_WaitForWaitableTimers(OS_nHwTimers, &handles[0]);

		OS_WINLINAssumeFrcIdentity(pHwt[idx]);

		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Timer %s (major = %d, minor = %d) expired.",
				OS_WINLINGetNameOfTimer(pHwt[idx]),
				pHwt[idx]->major,
				pHwt[idx]->minor);

		OS_WINLINTriggerInterrupt(pHwt[idx]->arch.isrId);
	}

#if (OS_ARCH == OS_WINDOWS)
	return 0;
#elif (OS_ARCH == OS_LINUX)
	return OS_NULL;
#endif
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif
/** \brief Does all global initialization.
 *
 * \attention This function must be called before OS_FrcInit().
 *
 * \param timeWarpFactor	The new warp factor.
 */
void OS_FrcGlobalInit(void)
{
	/* If there aren't any hardware counters, we don't have to do any initialization.
	 * This is because, no one will use the services offered in this file. Anything else
	 * would be an implementation error.
	 */
	if (OS_hwTimerTableBase != OS_NULL)
	{
		OS_WINLINAssert(OS_winlinTimeWarpFactor >= 1);

		OS_frcInitializationTime = OS_WINLIN_OSAL_TimeGetTime();
		OS_frcMinimumTimePeriod = OS_WINLIN_OSAL_GetMinimumPeriod();

		/* Start the period of increased timer resolution. Note that this must be
		 * undone at application exit. That's why OS_FrcTimeEndPeriod() is registered
		 * to be called at that time.
		 */
		OS_WINLIN_OSAL_TimeBeginPeriod(OS_frcMinimumTimePeriod);
		OS_WINLIN_OSAL_AtExit(&OS_FrcTimeEndPeriod);

		OS_frcThreadInitializedEvent = OS_WINLIN_OSAL_CreateEvent(OS_TRUE, OS_FALSE);
		(void)OS_WINLIN_OSAL_CreateThread(
				&OS_FrcThread,
				OS_NULL,
				&OS_frcThreadId,
				"FrcThread",
				OS_WINLIN_THREAD_TYPE_FRC);

		/* Note, all the rest of the global initialization is done by the FRC thread.
		 * We just wait for it to finish its work.
		 */
		OS_WINLIN_OSAL_WaitForSingleEvent(OS_frcThreadInitializedEvent);

		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Globally initialized all FRCs. Timer resolution is %u ms. "
				"Initialization time %lu ms.",
				OS_frcMinimumTimePeriod,
				OS_frcInitializationTime);
	}
}

/** \brief Initializes a hardware timer.
 *
 * \param[in] td		Points to the hardware timer, of which the underlying FRC
 *						shall be initialized.
 */
void OS_FrcInit(const os_hwt_t *td)
{
	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Initialized timer %s (major = %d, minor = %d).",
			OS_WINLINGetNameOfTimer(td),
			td->major,
			td->minor);
}

/** \brief Reads the current value of a hardware timer.
 *
 * \param[in] td		Points to the hardware timer, of which the current value
 *						shall be read and returned.
 *						The special value \c OS_NULL may be passed as argument. In this case,
 *						the current time relative to the initialization time of the FRC, i.e.
 *						the point in time at which OS_FrcGlobalInit() was called, is
 *						returned.
 *
 * \return The current value of \a td. This value is relative to the point in time
 * at which OS_FrcGlobalInit() was called, the FRC's initialization time.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.HARDWARE.Value, 1
 */
os_timervalue_t OS_FrcRead(const os_hwt_t *td)
{
	os_winlinDword_t retVal = OS_WINLIN_OSAL_TimeGetTime();

	/* Make time value relative to FRC initialization time and handle
	 * overflows. Such overflows are
	 * passed through to the calling code, but are shifted back
	 * in time depending on OS_frcInitializationTime.
	 * The following operation is carried out modulo 2^32, because that's
	 * the maximum we get on Microsoft Windows.
	 */
	retVal = (os_winlinDword_t)(retVal - OS_frcInitializationTime);
	retVal &= 0xffffffffu;	/* modulo 2^32 */

	/* Apply the warp factor to slow down AUTOSAR time. */
	retVal /= (os_winlinDword_t)OS_winlinTimeWarpFactor;

	if (td != OS_NULL)
	{
		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Read value %u from timer %s (major = %d, minor = %d).",
				retVal,
				OS_WINLINGetNameOfTimer(td),
				td->major,
				td->minor);
	}

	return (os_timervalue_t)retVal;
}

/** \brief Starts the given hardware timer.
 *
 * \param[in] td		The hardware timer to be started.
 * \param[in] old		Ignored. The current point in time is used instead.
 * \param[in] delta		The number of ticks to wait before waking up. The current point in
 *						time is used as reference to which \a delta is added. Note, this
 *						is a deviation from the usual approach, which uses \a old as
 *						reference.
 *
 * \retval OS_FALSE The only value returned by this function. This is because,
 * the current point in time is taken as reference and not \a old. Hence, the expiry
 * point is always in the future and can never be <em>already passed</em>.
 * This is, what the return value reports: if the intended expiry point has already
 * passed or not.
 */
os_boolean_t OS_FrcStart(const os_hwt_t *td, os_timervalue_t old, os_tick_t delta)
{
	os_frcdescr_t* const fd = OS_WINLINGetFrcOfHwt(td);
	os_boolean_t alreadyPassed = OS_FALSE;

	/* Mind the conversion to 100-nanoseconds!
	 * Waitable timers in Windows use 100-nanoseconds as base unit.
	 */
	os_winlinTimerValue_t const relTime =
			( ( ( (os_winlinTimerValue_t)delta )
			    * OS_winlinNsPerTick
			    * OS_winlinTimeWarpFactor
			  )
			  / ( (os_winlinTimerValue_t)100U )
			);

	/* Note, that the parameter 'old' is ignored and the current point in
	 * time is used instead. This is why the 'relTime' is multiplied by -1 to
	 * make the delay relative to now.
	 */
	OS_WINLIN_OSAL_SetWaitableTimer(fd->waitableTimer, -1 * relTime);

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Set timer %s (major = %d, minor = %d) to expire in %u ms at time %u. "
			"Warp factor is %u.",
			OS_WINLINGetNameOfTimer(td),
			td->major,
			td->minor,
			delta,
			(os_uint_t)(old + delta),
			(os_uint_t)OS_winlinTimeWarpFactor);

	return alreadyPassed;
}

/** \brief Stops a hardware timer.
 *
 * \param[in] td		Points to the hardware timer to be stopped.
 */
void OS_FrcStop(const os_hwt_t *td)
{
	os_frcdescr_t* fd = OS_WINLINGetFrcOfHwt(td);
	OS_WINLIN_OSAL_CancelWaitableTimer(fd->waitableTimer);

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Stopped timer %s (major = %d, minor = %d).",
			OS_WINLINGetNameOfTimer(td),
			td->major,
			td->minor);
}

/** \brief Checks if the calling thread is a FRC thread.
 *
 * \retval OS_FALSE The calling thread is not a FRC thread.
 * \retval OS_TRUE	The calling thread is a FRC thread, i.e. has the function
 *					OS_FrcThread() on its stack.
 */
os_boolean_t OS_FrcIsThisAFrcThread(void)
{
	return OS_WINLIN_OSAL_GetCurrentThreadId() == OS_frcThreadId;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
