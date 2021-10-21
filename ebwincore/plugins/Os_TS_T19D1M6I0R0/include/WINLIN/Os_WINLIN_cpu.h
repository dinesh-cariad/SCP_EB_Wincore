/* Os_WINLIN_cpu.h
 *
 * CPU header file for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_cpu.h 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#ifndef OS_WINLIN_CPU_H_
#define OS_WINLIN_CPU_H_

#include <Os_defs.h>

#if (OS_ARCH == OS_WINDOWS)
#include <WINDOWS/Os_WINDOWS_cpu.h>
#elif (OS_ARCH == OS_LINUX)
#include <LINUX/Os_LINUX_cpu.h>
#else
#error "Unsupported architecture. Please check your makefiles."
#endif

#endif	/* OS_WINLIN_CPU_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
