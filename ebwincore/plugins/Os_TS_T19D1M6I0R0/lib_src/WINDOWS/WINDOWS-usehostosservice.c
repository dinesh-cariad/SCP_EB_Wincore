/* WINDOWS-usehostosservice.c
 *
 * This file contains the implementation of OS_WINDOWSUseHostOsService().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINDOWS-usehostosservice.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <WINLIN/Os_WINLIN_syscalls.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Controls the use of a Windows service by AUTOSAR software.
 *
 * \remarks Only user-mode software may call this function. Cf. OS_WINDOWSKernUseHostOsService().
 *
 * \remarks Generic AUTOSAR software should use OS_WINDOWSGoingToUseHostOsService() instead.
 * That function is more versatile and more convenient.
 *
 * \param kind				Indicates whether AUTOSAR software begins of finishes the use of a
 *							Windows service.
 * \param previousState		When AUTOSAR software finishes the use of a Windows service, this
 *							argument represents the interrupt disable level at the time
 *							AUTOSAR software started to use that Windows service.
 *							It is only required, when \a kind is
 *							OS_WINDOWS_FINISH_USE_OF_HOST_OS_SERVICE; otherwise it's disregarded.
 *
 * \return The current interrupt disable level on the core of the caller, before it was
 * changed to begin or finish the use of a Windows service.
 */
os_intstatus_t OS_WINDOWSUseHostOsService(
		os_winlinUseHostOsService_t kind,
		os_intstatus_t previousState)
{
	return OS_WINLINSyscall(
			OS_SC_WINDOWSUseHostOsService,
			(os_paramtype_t)kind,
			(os_paramtype_t)previousState,
			(os_paramtype_t)-1,
			(os_paramtype_t)-1);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
