/* WINLIN-win32api.c
 *
 * This file implements the OS-wrappers for the Win32-API.
 *
 * It tries to separate AUTOSAR and Windows worlds by selectively
 * including only the parts needed from Windows. One reason for that,
 * is to avoid name collisions, when including windows.h everywhere
 * in this code. Here is the only place, where this is done.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINDOWS-win32api.c 25725 2017-01-18 11:41:30Z vaka852 $
 */

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastFromFuncCallToNonMatchingType
 *   Cast from function call of a type to a non-matching type
 *
 * Reason: The API _beginthreadex() is defined by MinGW and returns an unsigned long.
 *   The compatibility with CreateThread() defined by Microsoft Windows requires a cast.
 */

/* This file has to be excluded from a Polyspace analysis, because it makes it crash with a segmentation fault.
 * The executable ps_porky.exe of Polyspace causes this fault.
 * Hence, when EB_STATIC_CHECK is defined, i.e., when Polyspace analyzes this code, this file becomes empty.
 */
#ifndef EB_STATIC_CHECK

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#error "On WINDOWS threads cannot be controlled via POSIX signals."
#endif

#include <Os_kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <WINLIN/Os_WINLIN_critsec.h>

/* There is also a SetEvent function in the Windows API!
 * We must the AUTOSAR one, before including the Windows header.
 */
#ifdef SetEvent
#undef SetEvent
#endif

#include <WINDOWS/Os_WINDOWS_WIN32.h>
#include <process.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C requires a declaration for each definition. */
static os_long_t OS_WINDOWSInterlockedExchange(os_long_t volatile*, os_long_t);
static os_winlinDword_t OS_WINDOWSWaitForMultipleObjectsWithTimeout(
 		os_winlinDword_t,
 		os_winlinHandle_t const *,
 		os_boolean_t,
 		os_winlinDword_t);
static os_winlinDword_t OS_WINDOWSWaitForSingleObjectWithTimeout(os_winlinHandle_t, os_winlinDword_t);
static void OS_WINDOWSSyscallFailed(os_char_t const*);
static os_winlinHandle_t OS_WINDOWSCreateThreadHelper(
		os_winlinThreadEntry_t,
		void *,
		os_winlinDword_t,
		os_winlinTid_t*,
		os_winlinThreadType_t);
static void OS_WINDOWSSetSchedulingAttributesOfThread(os_winlinHandle_t, os_winlinThreadType_t);

/** \brief Initializes the Win32 interface.
 *
 * \remarks On WINDOWS there is no need to initialize the interface with it.
 * This functions is defined to comply with the OSAL interface.
 */
void OS_WINLIN_OSAL_Init(void)
{
}

/**
 * \see http://msdn.microsoft.com/en-us/library/ms686801%28v=vs.85%29.aspx
 */
os_winlinTlsIndex_t OS_WINLIN_OSAL_TlsAlloc(void)
{
	os_winlinTlsIndex_t tlsIdx = TlsAlloc();
	if (tlsIdx == TLS_OUT_OF_INDEXES)
	{
		OS_WINDOWSSyscallFailed("TlsAlloc");
	}
	return tlsIdx;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/ms686818%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_TlsSetValue(os_winlinTlsIndex_t tlsIdx, void * data)
{
	if (TlsSetValue(tlsIdx, data) == OS_FALSE)
	{
		OS_WINDOWSSyscallFailed("TlsSetValue");
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/ms686812%28v=VS.85%29.aspx
 */
void * OS_WINLIN_OSAL_TlsGetValue(os_winlinTlsIndex_t tlsIdx)
{
	void * data = TlsGetValue(tlsIdx);
	if ((data == OS_NULL) && (GetLastError() != ERROR_SUCCESS))
	{
		OS_WINDOWSSyscallFailed("TlsGetValue");
	}
	return data;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683476%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_InitializeCriticalSection(os_winlinCriticalSection_t* pCs)
{
	(void)InitializeCriticalSectionAndSpinCount(pCs, 4000/*magic number*/);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682608%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_EnterCriticalSection(os_winlinCriticalSection_t* pCs)
{
	(void)EnterCriticalSection(pCs);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms684169%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_LeaveCriticalSection(os_winlinCriticalSection_t* pCs)
{
	(void)LeaveCriticalSection(pCs);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/ms683590%28VS.85%29.aspx
 */
static os_long_t OS_WINDOWSInterlockedExchange(os_long_t volatile* var, os_long_t val)
{
	return InterlockedExchange(var, val);
}

/** \brief Initializes the given spinlock.
 *
 * \param [in,out] spinlock		Points to the spinlock to be initialized.
 */
void OS_WINLIN_OSAL_InitSpinlock(os_syncspot_t* spinlock)
{
	OS_PARAM_UNUSED(spinlock);
}

/** \brief Takes the given spinlock.
 *
 * \param [in,out] spinlock		Points to the spinlock to be taken.
 */
void OS_WINLIN_OSAL_TakeSpinlock(os_syncspot_t* spinlock)
{
	while (OS_WINDOWSInterlockedExchange((long volatile*)spinlock, 1L) == 1L)
	{
		/* Wait or spin until spinlock has been acquired. */
	}
}

/** \brief Drops the given spinlock.
 *
 * \param [in,out] spinlock		Points to the spinlock to be dropped.
 */
void OS_WINLIN_OSAL_DropSpinlock(os_syncspot_t* spinlock)
{
	(void)OS_WINDOWSInterlockedExchange((long volatile*)spinlock, 0L);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683183%28v=vs.85%29.aspx
 */
os_winlinTid_t OS_WINLIN_OSAL_GetCurrentThreadId(void)
{
	return GetCurrentThreadId();
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686714%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_TerminateCurrentProcess(os_winlinDword_t exitCode)
{
	if (TerminateProcess(GetCurrentProcess(), exitCode) == 0)
	{
		OS_WINDOWSSyscallFailed("TerminateProcess");
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682453%28v=vs.85%29.aspx
 */
os_winlinHandle_t OS_WINLIN_OSAL_CreateThread(
		os_winlinThreadEntry_t entry,
		void * args,
		os_winlinTid_t * threadId,
		os_char_t const* threadName,
		os_winlinThreadType_t threadType)
{
	OS_PARAM_UNUSED(threadName);
	return OS_WINDOWSCreateThreadHelper(entry, args, 0, threadId, threadType);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686345%28v=vs.85%29.aspx
 */
os_winlinDword_t OS_WINLIN_OSAL_SuspendThread(os_winlinHandle_t handle)
{
	return OS_WINLIN_OSAL_SuspendThreadHelper(handle, OS_TRUE);
}

/** \brief Suspends a Windows thread.
 *
 * This function helps to suspend a Windows thread.
 *
 * \param handle		The handle of the thread to suspend.
 * \param synchronous	Not used.
 */
os_winlinDword_t OS_WINLIN_OSAL_SuspendThreadHelper(
		os_winlinHandle_t handle,
		os_boolean_t synchronous)
{
	os_winlinDword_t retval;
	OS_PARAM_UNUSED(synchronous);

	retval = SuspendThread(handle);
	if (retval == ((os_winlinDword_t) -1))
	{
		OS_WINDOWSSyscallFailed("SuspendThread");
	}

	return retval;
}

/** \brief Waits for the given Windows thread to become blocked.
 *
 * This function waits for the given Windows thread to become blocked.
 * This is only relevant, when \c OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL is \c 1.
 * Otherwise the Windows function SuspendThread() already accomplishes the job.
 *
 * \note To avoid making the macro OS_ResumeTaskSyncHelper() dependent on
 * OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL, the body of this function is in/excluded instead.
 *
 * \param handle			The handle of the Windows thread to observe.
 */
void OS_WINLIN_OSAL_WaitForThreadToBecomeBlocked(os_winlinHandle_t handle)
{
	OS_PARAM_UNUSED(handle);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms685086%28v=vs.85%29.aspx
 */
os_winlinDword_t OS_WINLIN_OSAL_ResumeThread(os_winlinHandle_t handle)
{
	os_winlinDword_t retval;

	retval = ResumeThread(handle);
	if (retval == ((os_winlinDword_t) -1))
	{
		OS_WINDOWSSyscallFailed("ResumeThread");
	}

	return retval;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682396%28v=vs.85%29.aspx
 */
os_winlinHandle_t OS_WINLIN_OSAL_CreateEvent(os_boolean_t manualReset,
		os_boolean_t initialState)
{
	os_winlinHandle_t handle = CreateEvent(OS_NULL, manualReset, initialState, OS_NULL );
	if (handle == OS_NULL )
	{
		OS_WINDOWSSyscallFailed("CreateEvent");
	}
	return handle;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686211%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_SetEvent(os_winlinHandle_t handle)
{
	os_boolean_t retval = SetEvent(handle);
	if (retval == OS_FALSE)
	{
		OS_WINDOWSSyscallFailed("SetEvent");
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms685081%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_ResetEvent(os_winlinHandle_t handle)
{
	os_boolean_t retval = ResetEvent(handle);
	if (retval == OS_FALSE)
	{
		OS_WINDOWSSyscallFailed("ResetEvent");
	}
}

/** \brief Tests the state of a Windows event.
 *
 * \note The current state of the event is not changed.
 *
 * \param handle	The handle of a Windows event to be tested.
 *
 * \retval OS_TRUE when the event is in the signalled state.
 * \retval OS_FALSE otherwise.
 */
os_boolean_t OS_WINLIN_OSAL_IsEventSet(os_winlinHandle_t handle)
{
	os_boolean_t isSet = OS_FALSE;
	if (OS_WINDOWSWaitForSingleObjectWithTimeout(handle, 0) != OS_WINDOWS_WAIT_TIMEOUT)
	{
		OS_WINLIN_OSAL_SetEvent(handle);	/* Restore original state. */
		isSet = OS_TRUE;
	}
	return isSet;
}

/**
 * \attention Only event handles are permitted, i.e. those created by OS_WINLIN_OSAL_CreateEvent().
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686293%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_SignalEventAndWait(
		os_winlinHandle_t eventToSignal,
		os_winlinHandle_t eventToWaitOn)
{
	os_winlinDword_t retval;

	retval = SignalObjectAndWait(eventToSignal, eventToWaitOn, INFINITE, FALSE);
	if (retval != WAIT_OBJECT_0)
	{
		OS_WINDOWSSyscallFailed("SignalObjectAndWait");
	}
}

/**
 * \attention Only handles returned by OS_WINLIN_OSAL_CreateEvent() are permitted in \a handles.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms687025%28v=vs.85%29.aspx
 *
 * \return The index into \a handles of the event, which became signaled first. This is
 * only relevant for the case when \a waitForAll was OS_FALSE. If \a waitForAll is OS_TRUE
 * the returned value is always 0, because this function return only after all of the
 * events in \a handles have become signaled.
 */
os_winlinDword_t OS_WINLIN_OSAL_WaitForMultipleEvents(
		os_winlinDword_t count,
		os_winlinHandle_t const * handles,
		os_boolean_t waitForAll)
{
	return OS_WINDOWSWaitForMultipleObjectsWithTimeout(
			count,
			handles,
			waitForAll,
			OS_WINDOWS_INFINITE);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms687025%28v=vs.85%29.aspx
 *
 * \return The same as OS_WINLIN_OSAL_WaitForMultipleEvents() or the special value
 * OS_WINDOWS_WAIT_TIMEOUT in case the wait operation experienced a time out.
 */
static os_winlinDword_t OS_WINDOWSWaitForMultipleObjectsWithTimeout(
		os_winlinDword_t count,
		os_winlinHandle_t const * handles,
		os_boolean_t waitForAll,
		os_winlinDword_t timeout)
{
	os_winlinDword_t retVal;
	OS_WINLINAssert(count < OS_WINDOWS_WAIT_TIMEOUT);

	{
		DWORD const dwTimeout = (timeout == OS_WINDOWS_INFINITE ? INFINITE : timeout);
		BOOL const convertedWaitForAll = (waitForAll == OS_TRUE ? TRUE : FALSE);
		retVal = WaitForMultipleObjects(
				count,
				handles,
				convertedWaitForAll,
				dwTimeout);
		if ((retVal == WAIT_FAILED) || (retVal == WAIT_ABANDONED_0))
		{
			OS_WINDOWSSyscallFailed("WaitForMultipleObjects");
		}
		else if (retVal == WAIT_TIMEOUT)
		{
			retVal = OS_WINDOWS_WAIT_TIMEOUT;
		}
		else
		{
			retVal -= WAIT_OBJECT_0;
		}
	}

	return retVal;
}

/**
 * \attention The given handle must have been created by OS_WINLIN_OSAL_CreateEvent().
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms687032%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_WaitForSingleEvent(os_winlinHandle_t handle)
{
	(void)OS_WINDOWSWaitForSingleObjectWithTimeout(handle, OS_WINDOWS_INFINITE);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms687032%28v=vs.85%29.aspx
 *
 * \return Only 0 and OS_WINDOWS_WAIT_TIMEOUT can be returned. The former in case \a handle
 * became signalled. The latter in case the wait operation experienced a time out.
 */
static os_winlinDword_t OS_WINDOWSWaitForSingleObjectWithTimeout(
		os_winlinHandle_t handle,
		os_winlinDword_t timeout)
{
	DWORD const dwTimeout = (timeout == OS_WINDOWS_INFINITE ? INFINITE : timeout);
	DWORD retVal = WaitForSingleObject(handle, dwTimeout);
	if ((retVal == WAIT_FAILED) || (retVal == WAIT_ABANDONED))
	{
		OS_WINDOWSSyscallFailed("WaitForSingleObject");
	}
	else if (retVal == WAIT_TIMEOUT)
	{
		retVal = OS_WINDOWS_WAIT_TIMEOUT;
	}
	else
	{
		retVal -= WAIT_OBJECT_0;
	}

	return retVal;
}

/**
 * \param [out]	pHandle				The pointed to memory is overwritten with the handle of the created timer.
 * \param [in]	tidOfhandlingThread	Not used on Windows.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682492%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_CreateWaitableTimer(os_winlinHandle_t* pHandle, os_winlinTid_t tidOfhandlingThread)
{
	OS_PARAM_UNUSED(tidOfhandlingThread);
	OS_WINLINAssert(pHandle != OS_NULL);

	{
		HANDLE const retVal = CreateWaitableTimer(OS_NULL, OS_FALSE, OS_NULL);
		if (retVal == OS_NULL)
		{
			OS_WINDOWSSyscallFailed("CreateWaitableTimer");
		}
		*pHandle = retVal;
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686289%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_SetWaitableTimer(
		os_winlinHandle_t hTimer,
		os_winlinTimerValue_t dueTime)
{
	BOOL ret;
	LARGE_INTEGER li;
	li.QuadPart = dueTime;

	ret = SetWaitableTimer(hTimer, &li, 0/*one-shot timer*/, OS_NULL, OS_NULL, FALSE);
	if (ret == FALSE)
	{
		OS_WINDOWSSyscallFailed("SetWaitableTimer");
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms681985%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_CancelWaitableTimer(os_winlinHandle_t hTimer)
{
	BOOL retVal = CancelWaitableTimer(hTimer);
	if (retVal == FALSE)
	{
		OS_WINDOWSSyscallFailed("CancelWaitableTimer");
	}
}

/** \brief Waits for the expiration of the given timers.
 *
 * This function returns, when at least one of the given timers expires.
 *
 * \param count		The number of handles in \a handles
 * \param handles	The handles of the timers previously created by OS_WINLIN_OSAL_CreateWaitableTimer().
 *
 * \return The index into \a handles of the expired timer.
 */
os_winlinDword_t OS_WINLIN_OSAL_WaitForWaitableTimers(os_winlinDword_t count, os_winlinHandle_t const * handles)
{
	return OS_WINDOWSWaitForMultipleObjectsWithTimeout(count, handles, OS_FALSE, OS_WINDOWS_INFINITE);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/dd757629%28v=vs.85%29.aspx
 */
os_winlinDword_t OS_WINLIN_OSAL_TimeGetTime(void)
{
	return timeGetTime();
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/dd757624%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_TimeBeginPeriod(os_uint_t period)
{
	MMRESULT res = timeBeginPeriod(period);
	if (res == TIMERR_NOCANDO)
	{
		OS_WINDOWSSyscallFailed("timeBeginPeriod");
	}
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/dd757626%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_TimeEndPeriod(os_uint_t period)
{
	MMRESULT res = timeEndPeriod(period);
	if (res == TIMERR_NOCANDO)
	{
		OS_WINDOWSSyscallFailed("timeEndPeriod");
	}
}

/** \brief Determines the minimum timer period supported by Windows.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/dd757627%28v=vs.85%29.aspx
 *
 * \return The minimum timer period in milliseconds supported by Windows.
 * This is the smallest amount of time between to consecutive timer ticks.
 * This value may be passed to OS_WINLIN_OSAL_TimeBeginPeriod() and OS_WINLIN_OSAL_TimeEndPeriod()
 * to adjust the current timer resolution.
 */
os_uint_t OS_WINLIN_OSAL_GetMinimumPeriod(void)
{
	TIMECAPS tc;
	MMRESULT res = timeGetDevCaps(&tc, sizeof(tc));
	if (res != MMSYSERR_NOERROR)
	{
		OS_WINDOWSSyscallFailed("timeGetDevCaps");
	}

	return tc.wPeriodMin;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904%28v=vs.85%29.aspx
 */
os_winlinTimerValue_t OS_WINLIN_OSAL_QueryPerformanceCounter(void)
{
	LARGE_INTEGER li;
	BOOL ret = QueryPerformanceCounter(&li);
	if (ret == FALSE)
	{
		OS_WINDOWSSyscallFailed("QueryPerformanceCounter");
	}

	return li.QuadPart;
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644905%28v=vs.85%29.aspx
 */
os_winlinTimerValue_t OS_WINLIN_OSAL_QueryPerformanceFrequency(void)
{
	LARGE_INTEGER li;
	BOOL ret = QueryPerformanceFrequency(&li);
	if (ret == FALSE)
	{
		OS_WINDOWSSyscallFailed("QueryPerformanceFrequency");
	}

	return li.QuadPart;
}

/** \brief Checks if the current platform offers high-resolution performance counters.
 *
 * The check is based upon the Windows service QueryPerformanceFrequency(), which
 * returns FALSE, if there are no high-resolution performance counters available.
 *
 * \retval OS_FALSE	High-resolution performance counters are not available.
 * \retval OS_TRUE	High-resolution performance counters are available.
 */
os_boolean_t OS_WINLIN_OSAL_IsPerformanceCounterAvailable(void)
{
	LARGE_INTEGER li;
	return QueryPerformanceFrequency(&li);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686225%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_DisablePriorityBoostOfThisProcess(void)
{
	BOOL b = SetProcessPriorityBoost(GetCurrentProcess(), TRUE);
	if (!b)
	{
		OS_WINDOWSSyscallFailed("SetProcessPriorityBoost");
	}
}

/** \brief Makes a thread more important than others.
 *
 * This function sets the priority of the calling thread to THREAD_PRIORITY_HIGHEST
 * and thus makes it a bit more important. This function is intended for
 * core threads, which should be scheduled in favor of tasks or ISRs.
 * The latter have the thread priority THREAD_PRIORITY_NORMAL.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686277%28v=vs.85%29.aspx
 */
void OS_WINLIN_OSAL_MakeThisThreadVeryImportant(void)
{
	OS_WINDOWSSetSchedulingAttributesOfThread(GetCurrentThread(), OS_WINLIN_THREAD_TYPE_CORE);
}

/**
 * \see http://msdn.microsoft.com/en-us/library/tze57ck3.aspx
 */
void OS_WINLIN_OSAL_AtExit(void (*func)(void))
{
	if (atexit(func) != 0)
	{
		OS_WINDOWSSyscallFailed("atexit");
	}
}

/** \brief Prints some information about the failed Win32 system call onto the
 * console and then panics.
 *
 * \remarks The function calls OS_WINLINPanic(), hence the whole application is
 * terminated and this function never returns.
 *
 * \param syscallName	The name of the failed Win32 system call.
 */
static void OS_WINDOWSSyscallFailed(os_char_t const* syscallName)
{
	os_intstatus_t is = OS_WINDOWSGoingToUseWindowsService();
	{
		HLOCAL hLocal = OS_NULL;
		os_boolean_t ok = OS_FALSE;
		os_winlinDword_t const lastError = GetLastError();

		if (lastError != ERROR_SUCCESS)	/* then, a real error occurred. */
		{
			os_winlinDword_t systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
			ok = FormatMessageA(
					FORMAT_MESSAGE_FROM_SYSTEM
					| FORMAT_MESSAGE_IGNORE_INSERTS
					| FORMAT_MESSAGE_ALLOCATE_BUFFER,
					OS_NULL,
					lastError,
					systemLocale,
					(LPTSTR)&hLocal,
					0,
					OS_NULL) != 0;
		}

		if (ok && (hLocal != OS_NULL))
		{
			(void)fprintf(stderr,
					"%-7lu WIN32 system call (%s) failed!\n"
					"The following problem occurred:\n%s.",
					OS_WINLIN_OSAL_GetCurrentThreadId(),
					syscallName,
					(char*)LocalLock(hLocal));
			/* Normally, we'd call the following function to free the allocated
			 * buffer for the error message, but we're going to die anyway and
			 * Windows always cleans up the bequests of a process.
			 */
			/*LocalFree(hLocal);*/
		}
		else
		{
			(void)fprintf(stderr,
					"%-7lu WIN32 system call (%s) failed!\n",
					OS_WINLIN_OSAL_GetCurrentThreadId(),
					syscallName);
		}
		(void)fflush(stderr);
		(void)fflush(stdout);

		OS_WINLINPanic();
	}
	OS_WINDOWSFinishedUsingWindowsService(is);
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
/* To suppress the warning
 * "warning: cast from function call of type 'long unsigned int' to non-matching type 'void *' [-Wbad-function-cast]"
 * concerning the call of _beginthreadex() below and the assignment of its return value to "handle". The return
 * value is always the Windows handle of the created thread. The problem is, that this return value is represented
 * by a different type (i.e., void *) in Microsoft Windows.
 */
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#endif

/**
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682453%28v=vs.85%29.aspx
 */
static os_winlinHandle_t OS_WINDOWSCreateThreadHelper(
		os_winlinThreadEntry_t entry,
		void * args,
		os_winlinDword_t creationFlags,
		os_winlinTid_t * threadId,
		os_winlinThreadType_t threadType)
{
#ifdef __CYGWIN__
	/* We're going to do something evil here, when this software is compiled with Cygwin.
	 * Unfortunately Cygwin does not provide _beginthreadex(), which is the safer way of
	 * creating threads, which use the C library. This safety aspect relates to the global
	 * objects in the C library and Windows' structured exception handling. If CreateThread()
	 * is used these features are not properly initialized and hence, one might get strange
	 * results with some (but few) C functions. Structured exception handling is used by
	 * the signal() mechanism and therefore might also malfunction.
	 * In this concrete use case the situation is less severe, because they all run AUTOSAR
	 * software, which does not know the C library or structured exception handling. Furthermore,
	 * all needed Windows threads are created at startup and live until application termination.
	 * Hence, there can't be no memory leaked, which is allocated by _beginthreadex() and stashed
	 * into a thread's local-storage.
	 *
	 * See also  [0, p. 231ff] and [1, p. 161ff].
	 *
	 * [0] Johnson M. Hart: Windows System Programming, 4th ed., Pearson Education Inc., 2013
	 * [1] Jeffrey Richter, Christophe Nassarre: Windows via C/C++, 5th ed., Microsoft Press, 2008
	 */
	os_winlinHandle_t handle = CreateThread(OS_NULL, 0, entry, args, creationFlags, threadId);
#else	/* !__CYGWIN__ */
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_winlinHandle_t handle = (os_winlinHandle_t)_beginthreadex(
			(void *)OS_NULL,
			0u,
			(unsigned (OS_WINDOWS_STDCALL *) (void *))entry,
			args,
			(unsigned)creationFlags,
			(unsigned *)threadId);
#endif	/* __CYGWIN__ */
	if (handle == OS_NULL)
	{
#ifdef __CYGWIN__
		OS_WINDOWSSyscallFailed("CreateThread");
#else
		OS_WINDOWSSyscallFailed("_beginthreadex");
#endif
	}

	OS_WINDOWSSetSchedulingAttributesOfThread(handle, threadType);

	return handle;
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif


/** \brief Sets the scheduling attributes of a thread.
 *
 * \param [in,out] handle	The thread, whose scheduling attributes shall be set.
 * \param [in] threadType	The type of thread represented by handle.
 */
static void OS_WINDOWSSetSchedulingAttributesOfThread(os_winlinHandle_t handle, os_winlinThreadType_t threadType)
{
	os_int_t prio = THREAD_PRIORITY_NORMAL;

	switch (threadType)
	{
	case OS_WINLIN_THREAD_TYPE_EXTERNAL:
	case OS_WINLIN_THREAD_TYPE_FRC:
		prio = THREAD_PRIORITY_ABOVE_NORMAL;
		break;

	case OS_WINLIN_THREAD_TYPE_ISR:
	case OS_WINLIN_THREAD_TYPE_TASK:
		prio = THREAD_PRIORITY_NORMAL;
		break;

	case OS_WINLIN_THREAD_TYPE_CORE:
		prio = THREAD_PRIORITY_HIGHEST;
		break;

	default:
		OS_WINLINPanic();
		break;
	}

	if (!SetThreadPriority(handle, prio))
	{
		OS_WINDOWSSyscallFailed("SetThreadPriority");
	}
}

#include <memmap/Os_mm_code_end.h>

#endif /* EB_STATIC_CHECK */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
 */
