/* Os_atomics_WINLIN.h
 *
 * This file redirects to more specific files depending on
 * which tool chain is in use.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_atomics_WINLIN.h 27492 2017-09-15 08:57:37Z olme8414 $
*/
#ifndef OS_ATOMICS_WINLIN_H
#define OS_ATOMICS_WINLIN_H

#include <Os_defs.h>
#include <Os_tool.h>

#if (OS_TOOL == OS_gnu)
#include <WINLIN/Os_atomics_WINLIN_gnu.h>
#else
#error "Unsupported OS_TOOL defined. Check your Makefiles!"
#endif

#endif /* OS_ATOMICS_WINLIN_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
