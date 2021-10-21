/* WINLIN-isrthread.c
 *
 * This file contains the implementation of the ISR thread bootstrap function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-isrthread.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastToPtrFromIntDiffSize
 * Cast to pointer from integer of different size [-Wint-to-pointer-cast]
 *
 * Reason:
 * An integer must be converted into a pointer, because the pointer type is required by
 * the Pthreads API for thread local storage, but the item to put there is a plain integer.
 */

#include <Os_kernel.h>
#include <Os_messagequeue.h>

#include <memmap/Os_mm_code_begin.h>

static void OS_WINLINHandleIrq(os_isr_t const* pIsr);

/** \brief The start function of an ISR thread.
 *
 * Each ISR is always executed by the same thread. This thread
 * is created at startup and starts its life in this function.
 * When ISRs are triggered serveral times this thread is reused to
 * run the ISR another time.
 *
 * \param[in]	args	Points to the ISR the thread is supposed to run.
 *						It's a pointer to a constant instance of os_isr_t.
 *
 * \return Always returns 0.
 */
#if (OS_ARCH == OS_WINDOWS)
os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINIsrThread(void* args)
#elif (OS_ARCH == OS_LINUX)
void* OS_WINLINIsrThread(void* args)
#else
#error "Unsupported architecture. Please check your makefiles."
#endif
{
	os_isr_t const* const pIsr = (os_isr_t const*)args;

	/* Possible diagnostic TOOLDIAG-1 <START> */
	OS_WINLINInitializeIsrThread(pIsr);
	/* Possible diagnostic TOOLDIAG-1 <STOP> */

	OS_WINLINAssert(
			!OS_WINLIN_OSAL_IsEventSet(
					OS_WINLINGetSyscallPortOfIsr(
							(pIsr)->dynamic)->syscallFinishedEvent));

	while (1)
	{
		OS_WINLINHandleIrq(pIsr);

		/* Signal that the interrupt request has been handled.
		 * This thread will get stuck in this function until the interrupt
		 * is requested again in the future.
		 */
		OS_WINLINFinishedIrq(pIsr);
	}

#if (OS_ARCH == OS_WINDOWS)
	return 0;		/* Return value not used. */
#elif (OS_ARCH == OS_LINUX)
	return OS_NULL;	/* Return value not used. */
#endif
}

/** \brief Handles the interrupt request.
 *
 * \post The value of OS_GetKernelData()->inKernel didn't changed compared to its value
 * at the time when the ISR started, i.e. when this function was called.
 *
 * \param pIsr		Points to the triggered ISR. This is the one to be served.
 */
static void OS_WINLINHandleIrq(os_isr_t const* pIsr)
{
	os_uint8_t oldInKernel = 255U;

	/* Internal interrupts do not have dedicated ISR threads. They are handled by
	 * a core thread directly. Hence, those must not show up here.
	 */
	OS_WINLINAssert(!OS_WINLINIsIsrInternal(pIsr));

#if (OS_N_CORES > 1)
	if ((pIsr->flags & OS_ISRF_CROSSCORE) != 0)
	{
		oldInKernel = OS_CrossCoreNotifyIsr(pIsr->isrId);
	}
	else
#endif
	if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT2)
	{
		oldInKernel = OS_Cat2Entry(pIsr->isrId);
	}
	else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT1)
	{
		oldInKernel = OS_Cat1Entry(pIsr->isrId);
	}
	else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CATK)
	{
		oldInKernel = OS_CatKEntry(pIsr->isrId);
	}
	else
	{
		OS_WINLINPanic();
	}

	OS_GetKernelData()->inKernel = oldInKernel;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
