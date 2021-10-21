/* WINLIN-syscalls.h
 *
 * This file contains the syscall IDs of WINLIN-specific system calls.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_syscalls.h 23692 2016-02-17 12:38:51Z olme8414 $
 */

#ifndef WINLIN_SYSCALLS_H_
#define WINLIN_SYSCALLS_H_

#include <Os_defs.h>
#include <Os_syscalls.h>

/** \brief Syscall ID OS_SC_WINLINClearInterrupt.
 *
 * User mode software uses OS_WINLINClearInterrupt() to use this system service.
 */
#define OS_SC_WINLINClearInterrupt				OS_SC(OS_N_SYSCALL + OS_U(0))

#if (OS_ARCH == OS_WINDOWS)
/** \brief Syscall ID OS_SC_WINDOWSUseHostOsService.
 *
 * User mode software uses OS_WINDOWSUseHostOsService().
 */
#define OS_SC_WINDOWSUseHostOsService			OS_SC(OS_N_SYSCALL + OS_U(1))
#endif

/** \brief Syscall ID OS_SC_WINLINFinishedIrq.
 *
 * User mode software uses OS_WINLINFinishedIrq().
 */
#define OS_SC_WINLINFinishedIrq					OS_SC(OS_N_SYSCALL + OS_U(2))

/** \brief Syscall ID OS_SC_WINLINTriggerInterruptViaSyscall.
 *
 * User mode software uses OS_WINLINTriggerInterruptViaSyscall().
 */
#define OS_SC_WINLINTriggerInterruptViaSyscall	OS_SC(OS_N_SYSCALL + OS_U(3))

/** \typedef os_syscallfuncptr_t
 * \brief Pointer of a system call.
 *
 * This pointer is used when system call are extracted from the syscall table to be called.
 * For this to succeed, we simply assume maximum number of parameters and a return value, even
 * though some syscalls use less parameters and/or don't return anything.
 */
#ifndef OS_ASM
typedef os_winlinresult_t (*os_syscallfuncptr_t)(os_paramtype_t, os_paramtype_t, os_paramtype_t, os_paramtype_t);
#endif

#endif /* WINLIN_SYSCALLS_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
