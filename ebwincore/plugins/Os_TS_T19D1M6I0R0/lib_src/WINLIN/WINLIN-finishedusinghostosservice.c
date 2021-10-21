/* WINLIN-finishedusinghostosservice.c
 *
 * This file contains the implementation of
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-finishedusinghostosservice.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <Os_api_arch.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#endif

/** \brief Finishes the use of a Windows/Linux service.
 *
 * \see OS_WINLINGoingToUseHostOsService()
 *
 * \param previousState		The previous host OS service state returned by
 *							OS_WINLINGoingToUseHostOsService().
 */
void OS_WINLINFinishedUsingHostOsService(os_intstatus_t previousState)
{
	if (OS_WINLINIsInitialized())
	{
		if ((!OS_WINLINIsThisASystemThread()) && (!OS_WINLINIsThisAnExternalThread()))
		{
			(void)OS_WINLINUseHostOsService(OS_WINLIN_FINISH_USE_OF_HOST_OS_SERVICE, previousState);
		}
	}
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
