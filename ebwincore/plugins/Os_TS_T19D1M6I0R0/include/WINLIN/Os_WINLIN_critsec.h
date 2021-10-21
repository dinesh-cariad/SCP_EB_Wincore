/* Os_WINLIN_critsec.h
 *
 * Redirects the #include request depending on the architecture.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_critsec.h 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#ifndef OS_WINLIN_CRITSEC_H_
#define OS_WINLIN_CRITSEC_H_

#if (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_WINDOWS_critsec.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_LINUX_critsec.h>
#else
#error "This file shall only be included for WINDOWS or LINUX."
#endif

#ifndef OS_ASM
extern void OS_WINLIN_OSAL_InitializeCriticalSection(os_winlinCriticalSection_t* pCs);
extern void OS_WINLIN_OSAL_EnterCriticalSection(os_winlinCriticalSection_t* pCs);
extern void OS_WINLIN_OSAL_LeaveCriticalSection(os_winlinCriticalSection_t* pCs);
#endif

#endif /* OS_WINLIN_CRITSEC_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
