/* Os_WINLIN_timer_config.h - Conversion macros for the timer driver.
 *
 * This file contains the macros that map ticks to nanoseconds and vice
 * versa. This header file shall be used only in the board header file.
 * It shall be included after OS_BoardTicksToNs and OS_BoardTicksToNs
 * macros are defined.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_timer_config.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_WINLIN_TIMER_CONFIG_H
#define OS_WINLIN_TIMER_CONFIG_H

#define OS_NsToTicks_TIMER0(x) OS_BoardNsToTicks(x)
#define OS_NsToTicks_TIMER1(x) OS_BoardNsToTicks(x)
#define OS_NsToTicks_TIMER2(x) OS_BoardNsToTicks(x)
#define OS_NsToTicks_TIMER3(x) OS_BoardNsToTicks(x)

#define OS_TicksToNs_TIMER0(x) OS_BoardTicksToNs(x)
#define OS_TicksToNs_TIMER1(x) OS_BoardTicksToNs(x)
#define OS_TicksToNs_TIMER2(x) OS_BoardTicksToNs(x)
#define OS_TicksToNs_TIMER3(x) OS_BoardTicksToNs(x)

/* The following macros are in effect, when OsTimestampTimer == INTERNAL. */
#define OS_NsToTicks_TbTimer(x)	(x)
#define OS_TicksToNs_TbTimer(x)	(x)

#endif /* OS_WINLIN_TIMER_CONFIG_H */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
