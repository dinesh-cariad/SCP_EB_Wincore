/* Os_types_WINDOWS.h
 *
 * Data types for WINDOWS.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_types_WINDOWS.h 27660 2017-10-18 14:07:36Z olme8414 $
*/

#ifndef OS_TYPES_WINDOWS_H
#define OS_TYPES_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#if (OS_CPU == OS_WIN32X86)
#include <WINDOWS/WIN32X86/Os_types_WIN32X86.h>
#else
#error "Unsupported derivative."
#endif

#include <WINLIN/Os_types_WINLIN.h>

#ifndef OS_ASM
/** \brief Synchronization spot with architecture support.
 *
 * This type defines a 'synchronization spot', which can be used by software
 * for synchronization/locking purposes, e.g. to implement mutual exclusion.
 * The definition of this type accounts for hardware-assisted operation, i.e.
 * alignment and size are appropriate to access instances of which to achieve
 * synchronization/locking.
 *
 * The type definition suits Windows' interlocked variable access functions.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms684122%28v=vs.85%29.aspx
 */
typedef volatile long os_syncspot_t;
#define OS_SYNCSPOT_ALIGNMENT_GRANULE 4

/** \brief Software synchronization spot.
 *
 * This type serves essentially the same purpose as os_syncspot_t,
 * with the exception that hardware-assistance is not a concern.
 * This means, instances of os_sw_syncspot_t just have to guarantee that
 * write accesses are atomic. Concurrent write accesses must be serialized
 * by hardware, whereby the concrete order is undefined.
 *
 * The type definition suits Windows' interlocked variable access functions.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms684122%28v=vs.85%29.aspx
*/
typedef volatile long os_sw_syncspot_t;

/** \typedef  os_winlinThreadEntry_t
 * \brief Windows thread entry function
 *
 * Each Windows/Linux thread starts its life in a function with this prototype.
 * The calling convention is imposed by CreateThread() and _beginthreadex().
 *
 * \see http://msdn.microsoft.com/en-us/library/aa246693%28v=vs.60%29.aspx
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682453%28v=vs.85%29.aspx
 * \see OS_WINLIN_OSAL_CreateThread()
 */
#ifndef EB_STATIC_CHECK
typedef os_winlinDword_t (OS_WINDOWS_STDCALL *os_winlinThreadEntry_t)(void*);
#else
/* PolySpece doesn't like additional attributes assigned to the function pointer. */
typedef os_winlinDword_t (*os_winlinThreadEntry_t)(void*);
#endif

/** \brief The type of an index into a thread's local storage.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686812%28v=vs.85%29.aspx
 */
typedef unsigned long os_winlinTlsIndex_t;

#endif	/* OS_ASM */

#ifdef __cplusplus
}
#endif

#endif	/* OS_TYPES_WINDOWS_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
