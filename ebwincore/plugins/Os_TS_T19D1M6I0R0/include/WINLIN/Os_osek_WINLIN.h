/* Os_osek_WINLIN.h
 *
 * Tweaks of the OSEK API necessary for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_osek_WINLIN.h 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#ifndef OS_OSEK_WINLIN_H_
#define OS_OSEK_WINLIN_H_

/* Fast locking is not supported on Windows/Linux. Interrupts must be enabled/disabled by asking
 * the respective core thread to do this. This is required to ensure proper and synchronized
 * access of a core's interrupt disable level in a multi-threaded environment.
 */
#ifdef OS_USE_FAST_LOCKING
#undef OS_USE_FAST_LOCKING
#endif

#endif /* OS_OSEK_WINLIN_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
