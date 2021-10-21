/* Os_WINDOWS_WIN32.h - WINDOWS specific includes
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINDOWS_WIN32.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_WINDOWS_WIN32_H
#define OS_WINDOWS_WIN32_H

#if ((!defined(WINVER)) || (!defined(_WIN32_WINNT)))
/* These macros shall be defined via the command-line, i.e. in Os_defs_WINDOWS.mak. */
#error "The target version of Microsoft Windows shall be specified via the macros WINVER and _WIN32_WINNT."
#endif

#ifndef OS_ASM
#ifndef EB_STATIC_CHECK
#include <windows.h>
#include <mmsystem.h>
#endif
#endif

#endif	/* OS_WINDOWS_WIN32_H */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
