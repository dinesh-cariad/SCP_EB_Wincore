/* Os_api_WINDOWS.h
 *
 * This file is the architecture-dependent public API prototypes file for WINDOWS.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_WINDOWS.h 27282 2017-08-24 13:57:43Z olme8414 $
 */

#ifndef OS_API_WINDOWS_H
#define OS_API_WINDOWS_H

#include <WINDOWS/Os_api_WINDOWS_shared.h>
#include <WINLIN/Os_api_WINLIN.h>

#define OS_WINDOWSPreStartOs(argc, argv)			OS_WINLINPreStartOs(argc, argv)
#define OS_WINDOWSClearInterrupt(isrId)				OS_WINLINClearInterrupt(isrId)
#define OS_WINDOWSGoingToUseWindowsService()		OS_WINLINGoingToUseHostOsService()
#define OS_WINDOWSFinishedUsingWindowsService(is)	OS_WINLINFinishedUsingHostOsService(is)

#endif /* OS_API_WINDOWS_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
