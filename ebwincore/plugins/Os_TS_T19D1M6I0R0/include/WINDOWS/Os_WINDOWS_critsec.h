/* Os_WINDOWS_critsec.h
 *
 * This file contains the declarations of functions dealing with critical sections.
 * These sections are implemented by Windows.
 * They were put here in a distinct header file and not in Os_proto_WINDOWS.h as the
 * other Windows API functions found in WINDOWS, because it should be avoided to repeat
 * the declaration of CRITICAL_SECTION. Instead, its declaration shall always come from
 * official headers. To avoid cluttering up the whole namespace with Windows-related
 * declarations just because of CRITICAL_SECTION, this header was created. Code using
 * critical sections must include this header in addition to Os_proto_arch.h.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINDOWS_critsec.h 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#ifndef OS_WINDOWS_CRITSEC_H_
#define OS_WINDOWS_CRITSEC_H_

#include <WINDOWS/Os_WINDOWS_WIN32.h>

#ifndef OS_ASM
#ifndef EB_STATIC_CHECK
typedef CRITICAL_SECTION os_winlinCriticalSection_t;
#else
/* Polyspace does not know anything about Microsoft Windows header files.
 * Hence, to avoid warnings during a Polyspace run this dummy structure must be defined.
 */
struct os_CRITICAL_SECTION_s
{
	os_uint_t dummy;
};
typedef struct os_CRITICAL_SECTION_s os_winlinCriticalSection_t;
#endif
#endif

#endif /* OS_WINDOWS_CRITSEC_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
