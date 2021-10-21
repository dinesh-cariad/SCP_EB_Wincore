/* Os_syscalls.h
 *
 * System call table and indices.
 *
 * Editor tabs = 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_syscalls.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_SYSCALLS_H
#define OS_SYSCALLS_H

#include <Os_types_arch.h>

/* These are the system call indexes for the kernel. The indexes MUST match the
 * positions in the system-call table in file kern-systable.c
 * The OS_SC macro computes a number from the system-call index in an
 * architecture-dependent way. It might be more efficient on some architectures
 * to use a premultiplied constant as the index to make the array access.
*/
#ifndef OS_SC
#define OS_SC(x)	(x)
#endif

#define OS_SC_UnknownSyscall			OS_SC(0)
#define OS_SC_StartOs					OS_SC(1)
#define OS_SC_SuspendInterrupts			OS_SC(2)
#define OS_SC_ResumeInterrupts			OS_SC(3)
#define OS_SC_ChainTask					OS_SC(4)
#define OS_SC_Schedule					OS_SC(5)
#define OS_SC_GetTaskId					OS_SC(6)
#define OS_SC_GetTaskState				OS_SC(7)
#define OS_SC_ActivateTask				OS_SC(8)
#define OS_SC_TerminateTask				OS_SC(9)
#define OS_SC_GetResource				OS_SC(10)
#define OS_SC_ReleaseResource			OS_SC(11)
#define OS_SC_SetEvent					OS_SC(12)
#define OS_SC_ClearEvent				OS_SC(13)
#define OS_SC_GetEvent					OS_SC(14)
#define OS_SC_WaitEvent					OS_SC(15)
#define OS_SC_GetAlarmBase				OS_SC(16)
#define OS_SC_GetAlarm					OS_SC(17)
#define OS_SC_SetRelAlarm				OS_SC(18)
#define OS_SC_SetAbsAlarm				OS_SC(19)
#define OS_SC_CancelAlarm				OS_SC(20)
#define OS_SC_GetActiveApplicationMode	OS_SC(21)
#define OS_SC_ShutdownOs				OS_SC(22)
#define OS_SC_IncrementCounter			OS_SC(23)
#define OS_SC_GetStackInfo				OS_SC(24)
#define OS_SC_StartScheduleTable		OS_SC(25)
#define OS_SC_ChainScheduleTable		OS_SC(26)
#define OS_SC_StopScheduleTable			OS_SC(27)
#define OS_SC_SyncScheduleTable			OS_SC(28)
#define OS_SC_SetScheduleTableAsync		OS_SC(29)
#define OS_SC_GetScheduleTableStatus	OS_SC(30)
#define OS_SC_TerminateApplication		OS_SC(31)
#define OS_SC_GetApplicationId			OS_SC(32)
#define OS_SC_GetIsrId					OS_SC(33)
#define OS_SC_CallTrustedFunction		OS_SC(34)
#define OS_SC_CheckIsrMemoryAccess		OS_SC(35)
#define OS_SC_CheckTaskMemoryAccess		OS_SC(36)
#define OS_SC_CheckObjectAccess			OS_SC(37)
#define OS_SC_CheckObjectOwnership		OS_SC(38)
#define OS_SC_DisableInterruptSource	OS_SC(39)
#define OS_SC_EnableInterruptSource		OS_SC(40)
#define OS_SC_GetCounterValue			OS_SC(41)
#define OS_SC_GetElapsedCounterValue	OS_SC(42)
#define OS_SC_StartScheduleTableSynchron	OS_SC(43)
#define OS_SC_GetCpuLoad				OS_SC(44)
#define OS_SC_ResetPeakCpuLoad			OS_SC(45)
#define OS_SC_TaskReturn				OS_SC(46)
#define OS_SC_ReturnFromCall			OS_SC(47)
#define OS_SC_SimTimerAdvance			OS_SC(48)
#define OS_SC_TryToGetSpinlock			OS_SC(49)
#define OS_SC_ReleaseSpinlock			OS_SC(50)
#define OS_SC_AllowAccess				OS_SC(51)
#define OS_SC_GetApplicationState		OS_SC(52)
#define OS_SC_ShutdownAllCores			OS_SC(53)
#define OS_SC_IocWrite					OS_SC(54)
#define OS_SC_IocSend					OS_SC(55)
#define OS_SC_IocEmptyQueue				OS_SC(56)
#define OS_SC_IocAddonExt				OS_SC(57)
/* 58 unused */
/* 59 unused */
/* Comment: System calls 59, 60, 61 and 62 are reserved for
 * CPU families. Define the prototypes in OS_types_<cpufamily>.h and
 * map them to OS_K_CPUFAMILY<1-4>.
*/
#define OS_SC_CPUFAMILY_1				OS_SC(60)
#define OS_SC_CPUFAMILY_2				OS_SC(61)
#define OS_SC_CPUFAMILY_3				OS_SC(62)
#define OS_SC_CPUFAMILY_4				OS_SC(63)

/* Size of the system call table. Must be a power of 2, or the calculation
 * of OS_SC_ZERO won't work (see below)
*/
#define OS_N_SYSCALL	64

/* On some architectures is might be more efficient to use a mask
 * and check for zero rather than a range comparison, especially where
 * the SC index is premultiplied. For this to work, N_SYSCALL must be
 * a power of 2.
 * These bits of a syscall index must be zero, otherwise
 * the system call will be routed to "Unknown".
 * The mask below allows a 5-bit range, i.e. 32 system calls
 * System call numbers are always multiples of 4 to make the
 * lookup more efficient. Therefore the 2 LSBs must always be
 * zero.
*/
#define OS_SC_ZERO		(~OS_SC(OS_N_SYSCALL-1))

/*!
 * The system-call table and the type of its entries.
 *
 * A system call may accept at most OS_MAXPARAM many parameters and may return a result of type os_result_t.
 * The pointer os_syscallptr_t is the most general type to cover all possible C prototypes of system calls.
*/
#ifndef OS_ASM
typedef void (*os_syscallptr_t)(void);
extern const os_syscallptr_t OS_syscallTable[OS_N_SYSCALL];
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
