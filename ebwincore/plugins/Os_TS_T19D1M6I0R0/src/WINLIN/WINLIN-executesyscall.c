/* WINLIN-executesyscall.c
 *
 * This file contains the implementation of OS_WINLINExecuteSyscall().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-executesyscall.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <Os_configuration.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

/* The following checks assert some assumptions about syscall return types. See comment
 * 'RETURN VALUE MASKING' below.
 * Note, checking the size of os_boolean_t is not necessary, because objects with this
 * type are only checked, whether they are zero or not. Hence, using a 8 bit mask is not
 * harmful.
 */
OS_STATIC_ASSERT(sizeof(os_result_t) == sizeof(os_uint8_t), OS_WINLINExecuteSyscallCheck0);
OS_STATIC_ASSERT(sizeof(os_appmodeid_t) == sizeof(os_uint8_t), OS_WINLINExecuteSyscallCheck1);
OS_STATIC_ASSERT(sizeof(os_applicationid_t) == sizeof(os_uint8_t), OS_WINLINExecuteSyscallCheck2);
OS_STATIC_ASSERT(sizeof(os_isrid_t) == sizeof(os_uint16_t), OS_WINLINExecuteSyscallCheck3);
OS_STATIC_ASSERT(sizeof(os_memoryaccess_t) == sizeof(os_uint8_t), OS_WINLINExecuteSyscallCheck4);
OS_STATIC_ASSERT(sizeof(os_winlinresult_t) == sizeof(os_uint16_t), OS_WINLINExecuteSyscallCheck5);

/** \brief Executes the system call as specified by \a pSyscallBuffer.
 *
 * \param [in,out] pSyscallBuffer		The buffer containing the arguments for the system
 *										call to be executed. It also identifies the system
 *										call and stores its return value, if any.
 */
void OS_WINLINExecuteSyscall(os_syscallbuffer_t* pSyscallBuffer)
{
	os_syscallfuncptr_t syscallPtr = OS_NULL;

	OS_WINLINAssert(pSyscallBuffer->syscallId != OS_SC_UnknownSyscall);

	if (pSyscallBuffer->syscallId < OS_N_SYSCALL)
	{
#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
#endif
		syscallPtr = (os_syscallfuncptr_t)OS_syscallTable[pSyscallBuffer->syscallId];
#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif
	}
	else
	{
		switch (pSyscallBuffer->syscallId)
		{
		case OS_SC_WINLINClearInterrupt:
			syscallPtr = (os_syscallfuncptr_t)&OS_WINLINKernClearInterrupt;
			break;

#if (OS_ARCH == OS_WINDOWS)
		case OS_SC_WINDOWSUseHostOsService:
			syscallPtr = (os_syscallfuncptr_t)&OS_WINDOWSKernUseHostOsService;
			break;
#endif

		case OS_SC_WINLINFinishedIrq:
			syscallPtr = (os_syscallfuncptr_t)&OS_WINLINKernFinishedIrq;
			break;

		case OS_SC_WINLINTriggerInterruptViaSyscall:
			syscallPtr = (os_syscallfuncptr_t)&OS_WINLINKernTriggerInterruptViaSyscall;
			break;

		default:
			OS_WINLINPanic();
			break;
		}
	}

	OS_EnterKernel();

	pSyscallBuffer->rv = syscallPtr(
			pSyscallBuffer->p1,
			pSyscallBuffer->p2,
			pSyscallBuffer->p3,
			pSyscallBuffer->p4);

	/* RETURN VALUE MASKING
	 * The variable 'syscallPtr' is of type os_syscallfuncptr_t, and hence  always returns 16 bits.
	 * The majority of syscall, though, returns only 8 bits. To get a proper return value, any 
	 * excess bits are masked out.
	 */
	if (pSyscallBuffer->syscallId != OS_SC_GetIsrId)
	{
		pSyscallBuffer->rv &= 0xffU;
	}

	/* As long as ISRs run there must be no task dispatch.
	 * One reason for this is, that any currently running ISR, which returns from a syscall (e.g., SetEvent()),
	 * must have a chance to call OS_WINLINFinishIrq(), before normal task dispatching may be resumed. In
	 * this case, the effects of entering the kernel are simply undone. Otherwise, normal task dispatching
	 * is resumed by calling OS_LeaveKernel().
	 */
	if (OS_WINLINGetCoreIsrCurrent() == OS_NULLISR)
	{
		(void)OS_LeaveKernel(pSyscallBuffer->rv); /* allow task dispatch */
	}
	else
	{
		OS_WINLINUndoEnterKernel();	/* prevent task dispatch */
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
