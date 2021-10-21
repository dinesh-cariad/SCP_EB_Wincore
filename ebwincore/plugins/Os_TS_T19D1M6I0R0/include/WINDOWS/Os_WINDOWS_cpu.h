/* Os_WINDOWS_cpu.h
 *
 * CPU header file for WINDOWS.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINDOWS_cpu.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_WINDOWS_CPU_H
#define OS_WINDOWS_CPU_H

#include <WINDOWS/Os_defs_WINDOWS.h>

/* Include the appropriate header file for the derivative.
 *
 * CAVEAT:
 * The presence here of an include directive for a particular derivative
 * does not imply support for that derivative, nor does it imply a
 * commitment or intention to support that derivative in the future.
*/
#if (OS_CPU == OS_WIN32X86)
#include <WINDOWS/WIN32X86/Os_WIN32X86.h>
#else
#error "OS_CPU is not properly defined. Check your makefiles!"
#endif

#endif	/* OS_WINDOWS_CPU_H */
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
