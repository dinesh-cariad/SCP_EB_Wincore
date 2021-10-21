/* Os_api_WINDOWS_shared.h
 *
 * This header file contains declarations of functions, which may
 * be used inside and outside of the kernel. They are declared in
 * a distinct header file and not in Os_proto_WINDOWS.h or
 * Os_api_WINDOWS.h, to avoid redundant declarations.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_WINDOWS_shared.h 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#ifndef OS_API_WINDOWS_SHARED_H_
#define OS_API_WINDOWS_SHARED_H_

#define OS_WINDOWSTriggerInterrupt(isrId)	OS_WINLINTriggerInterrupt(isrId)

#endif	/* OS_API_WINDOWS_SHARED_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
