/* Os_simtimer.h - Simulated hardware timer driver, internal part
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
 * of the software that uses it. It is especially useful for testing the synchronization
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
 * $Id: Os_simtimer.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_SIMTIMER_H
#define OS_SIMTIMER_H

#include <Os_kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/* No. of timers/No. of channels per timer.
 *
 * These can be defined externally if desired. The default is two timers, each with two
 * channels.
*/
#ifndef OS_SIMTIMER_NTIMER
#define OS_SIMTIMER_NTIMER		2
#endif
#ifndef OS_SIMTIMER_NCOMPARE
#define OS_SIMTIMER_NCOMPARE	2
#endif

/* Name of system-call function. If Os_config.h (generated) includes this file,
 * the OS_KernSimTimerAdvance system call is installed in the system call table. Otherwise
 * OS_KernUnknownSyscall is installed.
 *
 * Os_config.h will include this file if a simulated timer is configured. It is therefore
 * very important not to include this file in Os_kernel.h or any other global kernel header
 * file, otherwise the system-call will be installed all the time.
*/
#define OS_K_SIMTIMERADVANCE	(&OS_KernSimTimerAdvance)

#ifndef OS_ASM
extern const os_hwtdrv_t OS_simTimerDriver;

os_result_t OS_KernSimTimerAdvance(os_unsigned_t, os_tick_t);

#endif

#ifdef __cplusplus
}
#endif

#endif
/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
