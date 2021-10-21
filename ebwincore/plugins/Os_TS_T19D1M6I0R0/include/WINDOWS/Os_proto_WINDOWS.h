/* Os_proto_WINDOWS.h
 *
 * Function prototypes for WINDOWS.
 *
 * The prototypes of kernel functions are in a separate file because they
 * are included in the kernel/os-kernel.h file AFTER the generic types
 * have been defined, whereas the architecture-dependent types file is
 * included BEFORE the generic types are defined.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_proto_WINDOWS.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_PROTO_WINDOWS_H
#define OS_PROTO_WINDOWS_H

#include <WINLIN/Os_proto_WINLIN.h>

/** \brief Represents an infinite time span.
 *
 * This value may be passed to
 * - OS_WINLIN_OSAL_WaitForSingleObjectWithTimeout() and
 * - OS_WINLIN_OSAL_WaitForMultipleObjectsWithTimeout().
 */
#define OS_WINDOWS_INFINITE	0xFFFFFFFFU

/** \brief Returned by OS_WINLIN_OSAL_WaitFor(MultipleObjects|SingleObject)WithTimeout() in case of a timeout. */
#define OS_WINDOWS_WAIT_TIMEOUT 255U

#ifndef OS_ASM
extern os_intstatus_t OS_WINDOWSUseHostOsService(os_winlinUseHostOsService_t, os_intstatus_t);
extern os_intstatus_t OS_WINDOWSKernUseHostOsService(os_winlinUseHostOsService_t, os_intstatus_t);
#endif
#define OS_WINLINUseHostOsService(a, b)	OS_WINDOWSUseHostOsService((a), (b))

#endif	/* OS_PROTO_WINDOWS_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
