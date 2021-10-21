/* WINLIN-startos.c
 *
 * This file contains the WINLIN implementation of the StartOs() API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-startos.c 27660 2017-10-18 14:07:36Z olme8414 $
*/

#include <Os_kernel.h>
#include <stdlib.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Starts AutoCore OS in an architecture-dependent manner.
 *
 * This function is called by a core management thread.
 * It starts the OS and then handles the events, like system calls or
 * triggering ISRs, for one logical core. Which logical core is defined
 * by the return value of OS_GetMyCoreId().
 *
 * \see For \a mode see <a href="http://portal.osek-vdx.org/">OSEK Specification</a>
 * 2.2.3, section 13.7.2.2.
 *
 * \param[in]	mode	The mode in which the OS shall be started.
 */
void OS_ArchUserStartOs(os_uint8_t mode)
{
	/* At this point we must pretend that we are in the kernel, because this is the
	 * only means to keep startup and application startup hooks from dispatching
	 * tasks, when they return from syscalls. OS_LeaveKernel() won't call OS_Dispatch(),
	 * if we set inKernel to 1 here. Since entering the kernel means, that inKernel
	 * is incremented each time, (application) startup hooks can't dispatch tasks.
	 */
	OS_GetKernelData()->inKernel = 1;

	/* Generic OS startup done by OS_KernStartOs(). */
	OS_KernStartOs(mode);

	OS_WINLINSignalOsWasStartedOnCore(OS_GetMyCoreId());

	/* We've left OS_KernStartOs() with interrupts disabled. Here enable them again. */
	(void)OS_IntEnable();

	/* Now handle core events. */
	OS_WINLINCoreEventLoop();

	/* Note, the core event loop terminates, in case there is nothing left to do.
	 * Because StartOS() shall not return, this program terminates here.
	 */
	exit(EXIT_SUCCESS);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
