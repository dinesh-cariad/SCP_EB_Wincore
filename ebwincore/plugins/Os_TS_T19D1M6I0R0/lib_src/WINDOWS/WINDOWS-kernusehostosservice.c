/* WINDOWS-kernusehostosservice.c
 *
 * This file contains the implementation of the kernel-mode version of OS_WINDOWSUseHostOsService().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINDOWS-kernusehostosservice.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \copydoc OS_WINDOWSUseHostOsService()
 *
 * \remarks This is the kernel-mode equivalent of OS_WINDOWSUseHostOsService().
 * Only core threads may call this function.
 */
os_intstatus_t OS_WINDOWSKernUseHostOsService(
		os_winlinUseHostOsService_t kind,
		os_intstatus_t previousState)
{
	os_intstatus_t is = OS_WINLIN_INTERRUPT_ENABLE_LEVEL;

	/* Don't do this, because core thread may use Windows services at will. */
	/*OS_WINLINAssert(OS_GetKernelData()->inKernel != 0);*/
	OS_WINLINAssert(OS_WINLINIsThisASystemThread());

	switch (kind)
	{
	case OS_WINLIN_BEGIN_USE_OF_HOST_OS_SERVICE:
		is = OS_IntDisableMax();
		break;

	case OS_WINLIN_FINISH_USE_OF_HOST_OS_SERVICE:
		OS_IntRestoreMax(previousState);
		is = OS_WINLINGetCoreInterruptDisableLevel();
		break;

	default:
		OS_WINLINPanic();
		break;
	}

	return is;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
