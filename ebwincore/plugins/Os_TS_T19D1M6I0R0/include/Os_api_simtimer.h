/* Os_api_simtimer.h - Simulated hardware timer driver, API
 *
 * This file is a header file for the simulated hardware timer driver.
 * The simulator implements two (or more) free-running timers in software. Each timer has
 * two (or more) compare registers that can be programmed to generate an "interrupt"
 * whenever the timer passes the value in the compare register.
 *
 * The difference between this timer and a real hardware timer is that the timer does not
 * automatically advance. It is advanced by calling the OS_SimTimerAdvance() function from a
 * (trusted) task, ISR etc. Thus the timer can be controlled precisely by the test program,
 * permitting fine-grained control over the "timing" and therefore accurate testing
 * of the software that uses it. It is especiall useful for testing the synchronisation
 * features.
 *
 * In addition to the 4 standard init/read/start/stop functions provided by this driver,
 * two additional functions are provided:
 *
 *  - OS_SimTimerSetup(tmr, mask, isr) sets up the timer.
 *  - OS_SimTimerAdvance(tmr, incr) advances the timer by incr ticks.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_simtimer.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_API_SIMTIMER_H
#define OS_API_SIMTIMER_H

#include <Os_types.h>

#ifndef OS_ASM

/* These are the extra functions.
*/
#define OS_SimTimerAdvance(t, i)   OS_UserSimTimerAdvance(t, i)

extern os_result_t OS_SimTimerSetup(os_unsigned_t, os_unsigned_t, os_isrid_t);

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL)
extern os_result_t OS_UserSimTimerAdvance(os_unsigned_t, os_uint32_t);
#endif

#endif /* OS_ASM */

#endif /* OS_API_SIMTIMER_H */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
