/* WINLIN-kernfinishedirq.c
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-kernfinishedirq.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <Os_api_arch.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Signals a the end of an interrupt request.
 *
 * At the end of an interrupt request this function is automatically called,
 * as soon as the user-supplied ISR returns.
 * It informs the respective core thread, that the ISR has finished and the
 * interrupted activity may be resumed.
 *
 * \param[in] pIsr		The ISR, which was executed for the requested interrupt.
 */
void OS_WINLINKernFinishedIrq(os_isr_t const* pIsr)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_boolean_t doDispatch = OS_TRUE;
	os_isrdynamic_t* const pIsrDyn = pIsr->dynamic;

	OS_WINLINAssert(kernel_data->inKernel != 0);

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Finishing ISR %s.",
			OS_WINLINGetNameOfIsr(pIsr));

	/* Restore interrupt disable level. Please note, that there is no need
	 * to suspend the ISR thread, which just called OS_WINLINFinishedIrq(), because it'll
	 * block on its syscall finished event until it is re-triggered again. This means,
	 * OS_WINLINFinishedIrq() will only return, if the ISR is triggered again.
	 * See also function OS_WINLINServeSyscall().
	 */
	OS_WINLINRestoreInterruptDisableLevelAfterIsrRan(pIsrDyn);

	/* The ISR *pIsr is terminating. Hence, there is no current ISR running on this core.
	 * In case there are interrupted or pending ISRs the code below readjusts this
	 * information accordingly.
	 */
	OS_WINLINSetCoreIsrCurrent(OS_NULLISR);

	/* ISR nesting is supported. Hence, resume interrupted ISR, if any.
	 * Don't dispatch in this case.
	 */
	{
		os_isrid_t const idOfInterruptedIsr = OS_WINLINGetIdOfInterruptIsr(pIsrDyn);
		OS_WINLINSetIdOfInterruptedIsr(pIsrDyn, OS_NULLISR); /* Clear memory. */
		if (idOfInterruptedIsr != OS_NULLISR)
		{
			os_isr_t const* const pIntdIsr = &OS_isrTableBase[idOfInterruptedIsr];
			os_isrdynamic_t* const pIntdIsrDyn = OS_isrTableBase[idOfInterruptedIsr].dynamic;

			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Going to resume thread %#x=%d to resume ISR %s.",
					OS_WINLINGetTidOfIsrThread(pIntdIsrDyn),
					OS_WINLINGetTidOfIsrThread(pIntdIsrDyn),
					OS_WINLINGetNameOfIsr(pIntdIsr));

			doDispatch = OS_FALSE;
			OS_WINLINUndoEnterKernel();
			OS_WINLINSetCoreIsrCurrent(idOfInterruptedIsr);
			/* Make the core thread sharing the syscall of the resumed ISR again. */
			/* MARK:OS_WINLINKernFinishedIrq.A */
			OS_WINLINSetSyscallPortOfCurrentThread(OS_WINLINGetSyscallPortOfIsr(pIntdIsrDyn));
			(void)OS_WINLIN_OSAL_ResumeThread(OS_WINLINGetThreadOfIsr(pIntdIsrDyn));
		}
		else if (OS_WINLINAreThereAnyPendingIrqsToServe(OS_FALSE))
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"There are ISRs pending; will not do task dispatch; will serve ISRs.",
					OS_WINLINLOG_UNUSED_PARAM);

			/* When there are other pending ISRs, which have a higher priority than
			 * the current interrupt disable level (see OS_WINLINGetCoreInterruptDisableLevel()),
			 * we must prevent OS_WINLINServeSyscall() from raising the syscallFinished event
			 * of the interrupted task, if any. Otherwise, there is race condition between
			 * the highest priority pending ISR and the resumed task, which is able to execute
			 * some code, before that ISR starts.
			 */
			kernel_data->inFunction = OS_ININTERNAL;
			doDispatch = OS_FALSE;
			OS_WINLINUndoEnterKernel();
		}
		else
		{
			/* MISRA */
		}
	}

	if (doDispatch)	/* then, there is no nested interrupt. Do dispatch on category. */
	{
#if (OS_N_CORES > 1)
		if ((pIsr->flags & OS_ISRF_CROSSCORE) != 0)
		{
			doDispatch = OS_TRUE;
		}
		else
#endif
		if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT2)
		{
			doDispatch = OS_TRUE;
		}
		else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CAT1)
		{
			doDispatch = OS_FALSE;
		}
		else if ((pIsr->flags & OS_ISRF_CAT) == OS_ISRF_CATK)
		{
			doDispatch = OS_TRUE;
		}
		else
		{
			OS_WINLINPanic();
		}
		{
			/* The current task (if any) seen in OS_WINLINServeIrq() at the time the
			 * ISR was started is now no longer considered 'interrupted'. If it keeps
			 * on running is up to OS_CheckForDispatch() et al.
			 */
			os_task_t const* const currentTask = kernel_data->taskCurrent;
			if (currentTask != OS_NULL)
			{
				OS_WINLINSetInterruptedStateOfTask(currentTask, OS_FALSE);
			}
		}
		/* When this core has been shut down, we must not dispatch anything. */
		if (kernel_data->inFunction != OS_INSHUTDOWN)
		{
			/* Note, for category 1 interrupts we don't do a "full" dispatch, but we still
			 * have to resume the interrupted task, if any. The way this has to be done is the
			 * same as for other interrupt categories. All this is handled by the OS_NO_DISP
			 * branch in the case-statement below.
			 */
			os_unsigned_t kindOfDispatch = OS_NO_DISP;

			if (doDispatch)
			{
				/* At this point we surely want to dispatch something, because there are no interrupts
				 * pending and no interrupted ISRs, too. Hence, argument oldInKernel is set to 0 to trigger
				 * the dispatch of a task.
				 */
				kindOfDispatch = OS_CheckForDispatch(0u);
			}

			switch(kindOfDispatch)
			{
			case OS_NO_DISP:
				if (kernel_data->taskCurrent != OS_NULL)
				{
					/* We're going to resume an interrupted task. Hence, set parameters accordingly.
					 * These parameters are set in accordance with OS_Dispatch(). We can't call
					 * OS_Dispatch() here, because the task was just interrupted and post task hooks
					 * etc. shall not be called.
					 * The current interrupt disable level is preserved in case the current task shares
					 * a resource with an ISR and was just interrupted by higher-priority ISRs. In
					 * this case, the interrupt disable level must be preserved to prevent other
					 * tasks and ISRs from accessing the resource.
					 */
					kernel_data->inFunction = OS_INTASK;
					kernel_data->inKernel = 0;
					OS_ResumeTaskAsyncHelper(
							kernel_data->taskCurrent,
							OS_WINLIN_RESUME_TASK_ASYNC_KEEP_INT_DISABLE_LEVEL);
				}
				break;

			case OS_DISP_WITHOUT_SAVE:
				/* Please note that killing of tasks is not supported. Hence OS_DISP_WITHOUT_SAVE
				 * should actually not occur, but in case of chained tasks it does. For chained
				 * tasks it must be treated the same as OS_DISP_WITH_SAVE to restart the chained
				 * task.
				 */
			case OS_DISP_WITH_SAVE:
				OS_Dispatch(OS_TS_READY_ASYNC);
				break;

			default:
				OS_WINLINPanic();
				break;
			}
		}
	}
	else
	{
		/* A dispatch was not done. This means, there are either pending IRQs or interrupted ISRs.
		 * In either case, we can't leave the kernel yet, because there is still work to do and
		 * this must be done inside the kernel. So we simple restart the core-event loop.
		 */
		OS_WINLINCoreLongjmp();
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
