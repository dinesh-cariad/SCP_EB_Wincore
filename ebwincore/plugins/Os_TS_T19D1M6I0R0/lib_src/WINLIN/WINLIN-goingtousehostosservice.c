/* WINLIN-goingtousehostosservice.c
 *
 * This file contains the implementation of a function to tell AutoCore OS,
 * that a host OS service is going to be used.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-goingtousehostosservice.c 27660 2017-10-18 14:07:36Z olme8414 $
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

/** \brief Starts a section of code in which a Windows/Linux service is used.
 *
 * To use a Windows/Linux service, e.g. file I/O or printf(), a user must surround
 * this code as in the following example. Failing to do so might result in
 * unexpected behavior or deadlocks. This is due to the fact, that threads
 * are suspended using the Windows service SuspendThread() or POSIX signals and this
 * can lead to the mentioned effects. Because only a core thread calls SuspendThread()
 * or sends a POSIX signals, it must be prevented from doing so. That's the use case of
 * this macro and its counterpart OS_WINLINFinishedUsingHostOsService().
 *
 * \par Example
 * \code{.c}
 * void MyFunc(void)
 * {
 *		[...]
 *		{
 *			os_intstatus_t is = OS_WINLINGoingToUseHostOsService();
 *			... Using a Windows/Linux service. ...
 *			OS_WINLINFinishedUsingHostOsService(is);
 *		}
 *		[...]
 * }
 * \endcode
 *
 * \see OS_WINLINFinishedUsingHostOsService()
 *
 * \return The interrupt-disable level at the time of calling this macro.
 */
os_intstatus_t OS_WINLINGoingToUseHostOsService(void)
{
	os_intstatus_t is = 0;

	if (OS_WINLINIsInitialized())
	{
		if ((!OS_WINLINIsThisASystemThread()) && (!OS_WINLINIsThisAnExternalThread()))
		{
			is = OS_WINLINUseHostOsService(OS_WINLIN_BEGIN_USE_OF_HOST_OS_SERVICE, 0);
		}
	}

	return is;
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
