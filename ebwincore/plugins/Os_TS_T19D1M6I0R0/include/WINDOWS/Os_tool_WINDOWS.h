/* Os_tool_WINDOWS.h
 *
 * Specific definitions for the tools used for WINDOWS.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_tool_WINDOWS.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_TOOL_WINDOWS_H
#define OS_TOOL_WINDOWS_H

#include <Os_defs.h>

#if (OS_TOOL==OS_gnu)
#include <WINDOWS/Os_tool_WINDOWS_gnu.h>
#elif (OS_TOOL==OS_vc)
#include <WINDOWS/Os_tool_WINDOWS_vc.h>
#else
#error "Unknown or unsupported toolchain. Check your Makefiles!"
#endif

#ifndef OS_GccVersionIsAtLeast
#define OS_GccVersionIsAtLeast(major, minor, patchlevel)	0
#endif

/** \name Interrupt Control
 *
 * The macros
 * -	OS_ENABLE() and
 * -	OS_DISABLE()
 * .
 * are use aliases for OS_IntEnable() and OS_IntDisable() respectively.
 */
/** @{ */
#define OS_ENABLE()  OS_IntEnable()
#define OS_DISABLE() OS_IntDisable()
/** @} */

/** \def OS_PARAM_UNUSED(p)
 * \brief Mark parameter p as unused parameter (or variable).
 *
 * \param p		The parameter to be marked as unused.
 */
#ifndef EB_STATIC_CHECK
#define OS_PARAM_UNUSED(p) ((void)(p))
#else
#define OS_PARAM_UNUSED(p) do {} while (0)
#endif /* EB_STATIC_CHECK */

#endif	/* OS_TOOL_WINDOWS_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
