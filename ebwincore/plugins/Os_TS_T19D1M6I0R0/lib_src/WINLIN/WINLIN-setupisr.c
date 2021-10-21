/* WINLIN-setupisr.c
 *
 * This file contains the implementation of OS_WINLINSetupIsr().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-setupisr.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#include <LINUX/Os_LINUX_threadsyncspot.h>
#endif

#include <memmap/Os_mm_code_begin.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
#endif

/** \brief Initializes an ISR.
 *
 * This function creates a Windows thread to run \a pIsr, unless it's internal.
 * One thread runs the same ISR again and again, when it is triggered. Hence,
 * when the ISR is re-triggered, the same thread is used again as before.
 *
 * ISRs of Internal interrupts do not have a dedicated Windows thread. Those are
 * executed by a core thread directly.
 */
void OS_WINLINSetupIsr(os_isr_t const* pIsr)
{
	os_isrdynamic_t* const pIsrDyn = pIsr->dynamic;
	os_syscallport_t* const pSyscallPort = OS_WINLINGetSyscallPortOfIsr(pIsrDyn);

	if (!OS_WINLINIsIsrInternal(pIsr))
	{
		OS_WINLINInitializeSyscallPort(pSyscallPort);

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
		OS_LINUXInitializeThreadSyncSpotForSuspendedThread(
				OS_LINUXGetThreadSyncSpotForIsr(pIsr));
#endif
		{
			os_winlinTid_t isrThreadTid;
			os_winlinHandle_t isrThread = OS_WINLIN_OSAL_CreateThread(
					&OS_WINLINIsrThread,
					(void*)pIsr,
					&isrThreadTid,
					OS_WINLINGetNameOfIsr(pIsr),
					OS_WINLIN_THREAD_TYPE_ISR);
			OS_WINLINSetThreadOfIsr(pIsrDyn, isrThread);
			OS_WINLINSetTidOfIsrThread(pIsrDyn, isrThreadTid);
		}
		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Created thread %#x=%d for ISR %s",
				OS_WINLINGetTidOfIsrThread(pIsrDyn),
				OS_WINLINGetTidOfIsrThread(pIsrDyn),
				OS_WINLINGetNameOfIsr(pIsr));
	}
	else
	{
		/* Internal ISRs do not have a syscall port. Those call AutoCore OS functions directly
		 * and hence dodge the syscall mechanism. Their syscall ports are made unusable here,
		 * to catch such errors early.
		 */
		OS_WINLINInitializeSyscallPortAsInvalid(pSyscallPort);
	}

	{
		os_winlinHandle_t const isrEvent = OS_WINLIN_OSAL_CreateEvent(OS_FALSE, OS_FALSE);
		OS_WINLINSetRequestedEventOfIsr(pIsrDyn, isrEvent);
	}
	OS_WINLINSetSavedInterruptDisableLevelOfIsr(pIsrDyn, OS_WINLIN_INTERRUPT_ENABLE_LEVEL);
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
