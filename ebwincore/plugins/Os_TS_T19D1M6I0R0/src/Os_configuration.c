/* Os_configuration.c
 *
 * This file contains the architecture-independent kernel configuration data
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration.c 31344 2020-02-05 08:50:53Z ache271795 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and
 * any other type.
 *
 * Reason:
 * Depending on the configuration, a "null function" pointer using a compatible
 * prototype might be used instead of the real function pointer.
 *
 *
 * MISRAC2012-2) Deviated Rule: 11.1 (required)
 * Conversions shall not be performed between a pointer to a function and
 * any type other than an integral type.
 *
 * Reason:
 * For filling the system call table, a "generic" function pointer using a
 * compatible prototype is used.
 *
 *
 * MISRAC2012-3) Deviated Rule: 14.3 (required)
 * Controlling expressions shall not be invariant.
 *
 * Reason:
 * This is intended to have the truth value of the expression determined at
 * compile-time.
 *
 *
 * MISRAC2012-4) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * Macro parameter is used in a way that doesn't allow the use of parentheses,
 * e.g. for designating struct members or initializers.
 *
 *
 * MISRAC2012-5) Deviated Rule: 9.5 (required)
 * Where designated initializers are used to initialize an array object the
 * size of the array shall be specified explicitly.
 *
 * Reason:
 * The array is terminated with a NULL-entry. Hence, the accessing software
 * does not need an explicit size specification.
 *
 *
 * MISRAC2012-6) Deviated Rule: 14.3 (required)
 * Controlling expressions shall not be invariant.
 *
 * Reason:
 * The number of priorities for a core depends on the configuration. Hence,
 * it changes only, when the configuration changes and is otherwise invariant.
 *
 * MISRAC2012-7) Deviated Rule: 12.4 (advisory)
 * Evaluation of constant expressions should not lead to unsigned integer
 * wrap-around.
 *
 * Reason:
 * The number of kernel and hardware locks is extremely small compared to the
 * number of bits used by the compiler to calculate the result of constant
 * expressions. Hence, there is no danger of overflows.
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration.
 *   The statements can be reached on multi-core systems.
*/

#include <Os_tool.h>
#include <Os_configuration.h>
#include <Os_syscalls.h>
#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>
#include <Os_cpuload.h>
#include <board.h>
#include <Os_api.h>
#include <Os_taskqueue.h>
#include <Os_messagequeue.h>
#include <Os_configuration_decls.h>

/* Spinlocks are only required if we have a multicore system.
*/
#if (OS_N_CORES > 1)
#include <Os_spinlock.h>
#endif /* OS_N_CORES > 1 */

#if (OS_IOC_ENABLED==1)
#include <public/Ioc_public_api.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>
static os_result_t OS_IocStartupChecksWrapper(void);
#endif /* OS_IOC_ENABLED */

#ifndef OS_GENERATION_ID_OS_H
#error "OS_GENERATION_ID_OS_H is not defined."
#endif

#ifndef OS_GENERATION_ID_OS_CONF_H
#error "OS_GENERATION_ID_OS_CONF_H is not defined"
#endif

#if (defined(OS_GENERATION_ID_OS_H)) && (defined(OS_GENERATION_ID_OS_CONF_H))

#if (OS_GENERATION_ID_OS_H == OS_GENERATION_ID_OS_CONF_H)

#include <memmap/Os_mm_const32_begin.h>
const os_uint32_t OS_configurationId = OS_GENERATION_ID_OS_H;
#include <memmap/Os_mm_const32_end.h>

#else
#error "OS_GENERATION_ID_OS_H and OS_GENERATION_ID_OS_CONF_H are different"
#endif

#endif

/* The definitions in this file support the range [1,8] for OS_N_CORES.
 * Any violations are reported here.
 */
#ifndef OS_N_CORES
#error "OS_N_CORES is undefined - check your makefiles."
#endif
#if ((OS_N_CORES < 1) || (OS_N_CORES > 8))
#error "Unsupported value for OS_N_CORES."
#endif

/*!
 * OS_appMode
 *
 * This variable holds the startup mode. The content is only valid after
 * the kernel has been started, i.e. when inFunction != OS_INSTARTUP holds true.
 * See also section "7.9.15  Start of the OS" in AUTOSAR SWS OS, document
 * revision 5.3.0, release 4.1 revision 3.
 *
 * This variable is protected by the lock with ID OS_APPMODE_LOCKID.
 */
#include <memmap/Os_mm_var8_begin.h>
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_appMode, .data.shared.os.OS_appMode)
os_appmodeid_t
		OS_SECTION_ATTRIB_PRE(.data.shared.os.OS_appMode)
		OS_appMode
		OS_SECTION_ATTRIB_POST(.data.shared.os.OS_appMode) = OS_NULLAPPMODE;
/* CHECK: PARSE */
#include <memmap/Os_mm_var8_end.h>

/*!
 * OS_configMode
 *
 * This constant holds various aspects of the kernel's configuration.
 * There are sevaral bitfields, representing configuration modes
 *  - BCC1/BCC2/ECC1/ECC2
 *  - STANDARD/EXTENDED
 *  - etc. See Os_kernel.h
*/
#include <memmap/Os_mm_const16_begin.h>
const os_uint32_t OS_configMode = OS_CONFIGMODE;
#include <memmap/Os_mm_const16_end.h>

/*!
 * OS_iecMode
 *
 * This constant determines how the checks for "interrupts disabled"
 * in the system services is performed. It can take one of three values:
 *
 *	OS_IEC_OSEK - the kernel performs only those checks necessary
 *		for correct functioning
 *	OS_IEC_OSEKEXTRA - the kernel performs checks to achieve similar
 *		behaviour to ProOSEK4 with EXTRA_RUNTIME_CHECKS turned on,
 *		except that the errors are detected much earlier and don't
 *		result in ShutdownOS.
 *	OS_IEC_AUTOSAR - the kernel performs strict Autosar checking,
 *		which is somewhat pointless and might be a little irritating
 *		However, that's what is specified :-(
*/
#include <memmap/Os_mm_const8_begin.h>
const os_uint8_t OS_iecMode = OS_IECMODE;
#include <memmap/Os_mm_const8_end.h>

/*!
 * OS_hookSelection
 *
 * This constant determines globally which hook functions will be called.
 * It has 4 bits, corresponding to the OS_ACTION_xxxHOOKxxx constant macros in Os_error.h:
 *
 * OS_ACTION_PROTECTIONHOOK  - the protection hook will get called
 * OS_ACTION_ERRORHOOK_APP   - the application-specific error hook will get called
 * OS_ACTION_ERRORHOOK       - the global error hook will get called for StatusType APIs
 * OS_ACTION_ERRORHOOK_EB    - the global error hook will get called for non-StatusType APIs
*/
#include <memmap/Os_mm_const8_begin.h>
const os_uint8_t OS_hookSelection = OS_HOOKSELECTION;
#include <memmap/Os_mm_const8_end.h>

/*!
 * OS_nApps
 *
 * The number of applications in the application table.
 *
 * OS_appTable
 *
 * An array of application contexts. These specify which memory
 * the application is allowed to access.
*/

#if OS_NAPPS==0

#define OS_K_GETAPPLICATIONID			&OS_KernUnknownSyscall
#define OS_K_TERMINATEAPPLICATION		&OS_KernUnknownSyscall
#define OS_K_GETAPPLICATIONSTATE		&OS_KernUnknownSyscall
#define OS_K_ALLOWACCESS				&OS_KernUnknownSyscall
#define OS_XC_RETNAPPLICATIONSTATE		&OS_RecvUnknownCall
#define OS_XC_RECVTERMINATEAPPLICATION	&OS_RecvUnknownCall
#define OS_XC_RECVGETAPPLICATIONSTATE	&OS_RecvUnknownCall
#define OS_KILLHOOKFUNC				OS_NULL

#define OS_APPBASE					OS_NULL

#else /* OS_NAPPS == 0*/

#define OS_CFG_INITAPP				&OS_InitApp
#define OS_CFG_INITAPPDATA			&OS_InitApplicationData
#define OS_KILLHOOKFUNC				&OS_KillHook

#define OS_APPBASE					OS_appTable

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
static const os_appcontext_t OS_appTable[OS_NAPPS] = { OS_APPCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NAPPS == 0*/

#include <memmap/Os_mm_const_begin.h>
const os_appcontext_t * const OS_appTableBase = OS_APPBASE;
const os_killhookfunc_t OS_killHookFunc = OS_KILLHOOKFUNC;
#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_const8_begin.h>
const os_applicationid_t OS_nApps = OS_NAPPS;
#include <memmap/Os_mm_const8_end.h>

/*!
 * OS_nFunctions
 *
 * The number of trusted functions in the table.
 *
 * OS_functionTable
 *
 * An array of trusted functions. These specify the function address,
 * owner, permissions and stack requirements for each trusted function.
*/
#if OS_NFUNCTIONS==0

#define OS_K_CALLTRUSTEDFUNCTION	&OS_KernUnknownSyscall
#define OS_TFBASE					OS_NULL

#else

#define OS_TFBASE					OS_functionTable

#include <memmap/Os_mm_const_begin.h>
static const os_function_t OS_functionTable[OS_NFUNCTIONS] = { OS_FUNCTIONCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NFUNCTIONS==0 */

#include <memmap/Os_mm_const_begin.h>
const os_functionid_t OS_nFunctions = OS_NFUNCTIONS;
const os_function_t * const OS_functionTableBase = OS_TFBASE;
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_nTasks
 *
 * The number of tasks in the task table.
 *
 * OS_taskTable
 *
 * OS_taskTable is an array of tasks containing the configuration of
 * each task.
 *
 * In principle we can choose whether to initialise the task stacks or
 * not. At the moment they are always initialised.
 *
 * OS_taskPtrs_coreX
 *
 * This array is a list of tasks corresponding to the OS_taskActivations_coreX
 * array. Each task owns 1 or more slots in the OS_taskActivations_coreX array,
 * and for each slot owned, the corresponding slot (same index) in
 * OS_taskPtrs points to the owner task. The first element of the array
 * is not used (OS_NULL). Index 0 is used as a null end-of-linked-list value.
 *
 * OS_taskDynamic_coreX
 *
 * The dynamic status of tasks, one entry per task. Note: this is not the
 * context of the task, though it may contain it.
*/

#if (OS_NTASKS == 0)

#define OS_ACTITASKFUNC			OS_NULL
#define OS_KILLTASKFUNC			(os_killtaskfunc_t)&OS_NullFunction

#define OS_K_ACTIVATETASK		&OS_KernUnknownSyscall
#define OS_K_TERMINATETASK		&OS_KernUnknownSyscall
#define OS_K_CHAINTASK			&OS_KernUnknownSyscall
#define OS_K_SCHEDULE			&OS_KernUnknownSyscall
#define OS_K_GETTASKID			&OS_KernUnknownSyscall
#define OS_K_GETTASKSTATE		&OS_KernUnknownSyscall

#define OS_XC_RECVACTIVATETASK	&OS_RecvUnknownCall
#define OS_XC_RECVGETTASKID		&OS_RecvUnknownCall
#define OS_XC_RECVGETTASKSTATE	&OS_RecvUnknownCall
#define OS_XC_RETNTASKSTATE		&OS_RecvUnknownCall

#define OS_TASKBASE				OS_NULL

#else /* OS_NTASKS != 0 */

#define OS_CFG_INITTASKS		&OS_InitTasks

#if (OS_CONFIGMODE & OS_STACKCHECK)
#define OS_CFG_INITTASKSTACKS	&OS_InitTaskStacks
#endif

#define OS_ACTITASKFUNC			&OS_ActiTask

#ifdef OS_EXCLUDE_KILLTASK
#define OS_KILLTASKFUNC			(os_killtaskfunc_t)&OS_NullFunction
#else
#define OS_KILLTASKFUNC			&OS_KillTask
#endif

#define OS_TASKBASE				OS_taskTable

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
const os_task_t OS_taskTable[OS_NTASKS] = { OS_TASKCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NTASKS==0 */

#include <memmap/Os_mm_const_begin.h>
const os_taskid_t OS_nTasks = OS_NTASKS;
const os_task_t * const OS_taskTableBase = OS_TASKBASE;
const os_actitaskfunc_t OS_actiTaskFunc = OS_ACTITASKFUNC;
const os_killtaskfunc_t OS_killTaskFunc = OS_KILLTASKFUNC;
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_maxPrio
 *
 * This constant contains the highest priority that a task can take.
 * It is used in several places. For example, as the priority for a task that
 * takes a resource that is shared with an ISR.
*/
#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-6 */
const os_taskprio_t OS_maxPrio[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_MAXPRIO_CORE) };
#include <memmap/Os_mm_const_end.h>

/*!
 * CLZ scheduling algorithm.
 */
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)

#include <memmap/Os_mm_const_begin.h>
const os_int_t OS_nPriorities[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_NPRIORITIES_CORE) };
const os_int_t OS_nPrioritySlots[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_NPRIORITYSLOTS_CORE) };
#include <memmap/Os_mm_const_end.h>

#if (OS_N_CORES > 1)
#include <memmap/Os_mm_const_begin.h>
os_clzword_t * const OS_slavePrioWord_ptr[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_SLAVEWORDPTR_CORE) };
#include <memmap/Os_mm_const_end.h>
#endif /* OS_N_CORES > 1 */

#if (OS_N_CORES > 1)
#include <memmap/Os_mm_const_begin.h>
os_priorityqueueentry_t * const OS_prioritySlot_ptr[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_PRIOSLOTPTR_CORE) };
#include <memmap/Os_mm_const_end.h>
#endif /* OS_N_CORES > 1 */

#if (OS_N_CORES > 1)
#include <memmap/Os_mm_const_begin.h>
os_priorityqueue_t const * const OS_priorityQueue_ptr[OS_N_CORES_MAX] =
{
	OS_MAKE_COREARRAY_FROM(OS_PRIOTITYQUEUEPTR_CORE)
};
#include <memmap/Os_mm_const_end.h>
#endif /* OS_N_CORES > 1 */

#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

/*!
 * Linked list scheduling algorithm.
 *
 * OS_taskActivations_coreX
 *
 * The links in the linked list of the task queue. Each slot contains the
 * index of the next slot in the queue. The corresponding (same index)
 * entry in OS_taskPtrs[] contains the address of the task that owns
 * the slot. There is always at least 1 entry in this array.
 * The first entry (index 0) is used to store the index of the current
 * activation of the currently running task or 0, if there is none.
 */
#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)

#if (OS_N_CORES > 1)
#include <memmap/Os_mm_const_begin.h>
const os_task_t * const * const OS_taskPtrs_ptr[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_PTASKPTRS_CORE) };
os_tasklink_t * const OS_taskActivations_ptr[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_TASKACTIVATIONSPTR_CORE) };
#include <memmap/Os_mm_const_end.h>
#endif /* (OS_N_CORES > 1) */

#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

/*!
 * OS_nStartModes
 *
 * This constant contains the number of startup modes that have been
 * configured. There must be at least 1 startup mode.
*/
#if OS_NSTARTMODES==0

#error "Configuration error: there must be at least 1 startup mode"

#else

#include <memmap/Os_mm_const8_begin.h>
const os_appmodeid_t OS_nStartModes = OS_NSTARTMODES;
#include <memmap/Os_mm_const8_end.h>

#endif /* OS_NSTARTMODES==0 */

/*!
 * OS_startModeTasks
 *
 * This array contains, for each defined start mode (of which there is
 * at least one) an index into the OS_autoStartTasks array.
 *
 * OS_autoStartTasks
 *
 * This array contains the ids of the tasks that should be started for
 * each start mode. Each mode's tasks are terminated by the null task id.
 * The index of the first entry for each mode is stored in the appropriate
 * location in OS_startModeTasks
*/
#ifdef OS_STARTMODETASKS

#define OS_CFG_ACTIVATEAUTOTASKS	&OS_ActivateAutoTasks

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeTasks[OS_NSTARTMODES] = { OS_STARTMODETASKS };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_taskid_t OS_autoStartTasks[] = { OS_AUTOSTARTTASKS };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_STARTMODETASKS */

/*!
 * OS_startModeAlarms
 *
 * This array contains, for each defined start mode (of which there is
 * at least one) an index into the OS_autoStartAlarms array.
 *
 * OS_autoStartAlarms
 *
 * This array contains the ids, times and cycles of the alarms that should
 * be started for each start mode. Each mode's alarms are terminated by the
 * null alarm id.
 * The index of the first entry for each mode is stored in the appropriate
 * location in OS_startModeAlarms
*/
#ifdef OS_STARTMODEALARMS

#define OS_CFG_ACTIVATEAUTOALARMS	&OS_ActivateAutoAlarms

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeAlarms[OS_NSTARTMODES] = { OS_STARTMODEALARMS };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-5 */
const os_autoalarm_t OS_autoStartAlarms[] = { OS_AUTOSTARTALARMS };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_STARTMODEALARMS */

/*!
 * OS_startModeSchedules
 *
 * This array contains, for each defined start mode (of which there is
 * at least one) an index into the OS_autoStartSchedules array.
 *
 * OS_autoStartSchedules
 *
 * This array contains the ids and offsets of the schedule tables that should
 * be started for each start mode. Each mode's schedule tables are terminated by the
 * null schedule table id.
 * The index of the first entry for each mode is stored in the appropriate
 * location in OS_startModeSchedules
*/
#ifdef OS_STARTMODESCHEDULES

#define OS_CFG_ACTIVATEAUTOSCHEDULES	&OS_ActivateAutoSchedules

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeSchedules[OS_NSTARTMODES] = { OS_STARTMODESCHEDULES };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-5 */
const os_autoschedule_t OS_autoStartSchedules[] = { OS_AUTOSTARTSCHEDULES };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_STARTMODESCHEDULES */

/*!
 * OS_CFG_INITKERNSTACK
 *
 * This function initializes the kernel/interrupt stack.
*/
#if (OS_CONFIGMODE & OS_STACKCHECK)
#define OS_CFG_INITKERNSTACK	&OS_InitKernStack
#endif

/*!
 * OS_nHwTimers
 *
 * This constant contains the number of hardware timers that need
 * initialising.
 *
 * OS_hwTimer
 *
 * This array contains an element for each hardware timer that needs
 * initialising.
 *
 * The format of the array is architecture-dependent, but includes a pointer
 * to a function that takes the address of the os_hwt_t as its
 * only parameter. Timer drivers therefore need to be written to conform
 * to this specification.
*/

#if OS_NHWTIMERS!=0

#define OS_HWTIMERBASE		OS_hwTimer
#define OS_CFG_INITTIMERS	&OS_InitTimers

#include <memmap/Os_mm_const_begin.h>
static const os_hwt_t OS_hwTimer[OS_NHWTIMERS] = { OS_HWTIMER };
#include <memmap/Os_mm_const_end.h>

#else /* OS_NHWTIMERS==0 */

#define OS_HWTIMERBASE		OS_NULL

#endif /* OS_NHWTIMERS!=0 */

#include <memmap/Os_mm_const8_begin.h>
const os_uint8_t OS_nHwTimers = OS_NHWTIMERS;
#include <memmap/Os_mm_const8_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_hwt_t * const OS_hwTimerTableBase = OS_HWTIMERBASE;
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_nCounters
 *
 * This constant contains the number of counters configured.
 *
 * OS_counter
 *
 * This array contains the static part of the counters.
 *
 * We can omit the IncrementCounter system call if all the counters
 * are hardware counters; ie if NCOUNTERS is <= NHWTIMERS
*/

#if OS_NCOUNTERS==0

#define OS_K_GETCOUNTERVALUE		&OS_KernUnknownSyscall
#define OS_K_GETELAPSEDCOUNTERVALUE	&OS_KernUnknownSyscall

#define OS_XC_RECVGETCOUNT			&OS_RecvUnknownCall
#define OS_XC_RECVADVANCECOUNTER	&OS_RecvUnknownCall

#define OS_CTRBASE					OS_NULL

#else

#define OS_CFG_INITCOUNTERS			&OS_InitCounters
#define OS_CTRBASE					OS_counter

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
static const os_counter_t OS_counter[OS_NCOUNTERS] = { OS_COUNTER };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NCOUNTERS == 0 */

#include <memmap/Os_mm_const8_begin.h>
const os_uint8_t OS_nCounters = OS_NCOUNTERS;
#include <memmap/Os_mm_const8_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_counter_t * const OS_counterTableBase = OS_CTRBASE;
#include <memmap/Os_mm_const_end.h>

#if OS_NALARMS==0

/* No real alarms. We can set all related system calls to "Unknown
 * system call".
 * Note that there may be some alarms in the table due to
 * configured ScheduleTables, but these are not started or
 * manipulated using the normal alarm APIs.
*/

#define OS_K_GETALARMBASE		&OS_KernUnknownSyscall
#define OS_K_GETALARM			&OS_KernUnknownSyscall
#define OS_K_SETRELALARM		&OS_KernUnknownSyscall
#define OS_K_SETABSALARM		&OS_KernUnknownSyscall
#define OS_K_CANCELALARM		&OS_KernUnknownSyscall
#define OS_XC_RECVGETALARM		&OS_RecvUnknownCall
#define OS_XC_RECVSETRELALARM	&OS_RecvUnknownCall
#define OS_XC_RECVSETABSALARM	&OS_RecvUnknownCall
#define OS_XC_RECVCANCELALARM	&OS_RecvUnknownCall

#endif /* OS_NALARMS */

/*!
 * OS_nAlarms
 *
 * This constant contains the number of OSEK alarms configured.
 *
 * OS_totalAlarms
 *
 * This constant contains the total number of alarms configured. The
 * figure includes alarms that are present for other purposes such as
 * schedule tables etc.
 *
 * OS_alarm
 *
 * This array contains the static part of the alarms.
*/
#define OS_TOTALALARMS	(OS_NALARMS + OS_NSCHEDULES)

#if OS_TOTALALARMS==0

#define OS_ALARMBASE			OS_NULL
#define OS_KILLALARMFUNC		(os_killalarmfunc_t)&OS_NullFunction

#else

#define OS_CFG_INITALARMS		&OS_InitAlarms
#define OS_ALARMBASE			OS_alarm

#ifdef OS_EXCLUDE_KILLALARM
#define OS_KILLALARMFUNC		(os_killalarmfunc_t)&OS_NullFunction
#else
#define OS_KILLALARMFUNC		&OS_KillAlarm
#endif

#if OS_NALARMCALLBACKS != 0
#include <memmap/Os_mm_const_begin.h>
const os_alarmcallback_t OS_alarmCallback[OS_NALARMCALLBACKS] = { OS_ALARMCALLBACK };
#include <memmap/Os_mm_const_end.h>
#endif

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
static const os_alarm_t OS_alarm[OS_TOTALALARMS] = { OS_ALARM };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_TOTALALARMS==0 */

#include <memmap/Os_mm_const8_begin.h>
const os_alarmid_t OS_nAlarms = OS_NALARMS;
const os_alarmid_t OS_totalAlarms = OS_TOTALALARMS;
#include <memmap/Os_mm_const8_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_killalarmfunc_t OS_killAlarmFunc = OS_KILLALARMFUNC;
const os_alarm_t * const OS_alarmTableBase = OS_ALARMBASE;
#include <memmap/Os_mm_const_end.h>

/*!
 * Event-related system calls
 *
 * This macro is not used to initialise a constant because the number of
 * events is of no interest to the kernel. However, if there are _no_
 * events at all we can omit the event-related system calls.
*/
#if OS_NEVENTS==0

#define OS_K_SETEVENT		&OS_KernUnknownSyscall
#define OS_K_CLEAREVENT		&OS_KernUnknownSyscall
#define OS_K_WAITEVENT		&OS_KernUnknownSyscall
#define OS_XC_RECVSETEVENT	&OS_RecvUnknownCall
#define OS_XC_RECVGETEVENT	&OS_RecvUnknownCall
#define OS_XC_RETNEVENT		&OS_RecvUnknownCall

#endif

#if OS_NETASKS==0

#define OS_K_GETEVENT		&OS_KernUnknownSyscall

#endif

/*!
 * OS_nSchedules
 *
 * This constant contains the number of schedule tables configured.
 *
 * OS_schedule
 *
 * This array contains an element for each schedule table configured.
 *
 * OS_stEntries
 *
 * This array contains all the expiry points for all the configured
 * schedule tables.
*/
#if OS_NSCHEDULES==0

#define OS_K_STARTSCHEDULETABLE			&OS_KernUnknownSyscall
#define OS_K_STARTSCHEDULETABLESYNCHRON	&OS_KernUnknownSyscall
#define OS_K_CHAINSCHEDULETABLE			&OS_KernUnknownSyscall
#define OS_K_STOPSCHEDULETABLE			&OS_KernUnknownSyscall
#define OS_K_SYNCSCHEDULETABLE			&OS_KernUnknownSyscall
#define OS_K_SETSCHEDULETABLEASYNC		&OS_KernUnknownSyscall
#define OS_K_GETSCHEDULETABLESTATUS		&OS_KernUnknownSyscall

#define OS_XC_RETNSCHEDULETABLESTATUS			&OS_RecvUnknownCall
#define OS_XC_RECVSTARTSCHEDULETABLE			&OS_RecvUnknownCall
#define OS_XC_RECVCHAINSCHEDULETABLE			&OS_RecvUnknownCall
#define OS_XC_RECVSTOPSCHEDULETABLE				&OS_RecvUnknownCall
#define OS_XC_RECVSYNCSCHEDULETABLE				&OS_RecvUnknownCall
#define OS_XC_RECVSETSCHEDULETABLEASYNC			&OS_RecvUnknownCall
#define OS_XC_RECVGETSCHEDULETABLESTATUS		&OS_RecvUnknownCall
#define OS_XC_RECVSTARTSCHEDULETABLESYNCHRON	&OS_RecvUnknownCall

#define OS_SCHEDULEBASE					OS_NULL
#define OS_KILLSCHEDULEFUNC				(os_killschedulefunc_t)&OS_NullFunction
#define OS_SYNCHRONIZEFUNC				OS_NULL

#else

#define OS_SYNCHRONIZEFUNC				&OS_Synchronize
#define OS_CFG_INITSCHEDULES			&OS_InitSchedules
#define OS_SCHEDULEBASE					OS_schedule
#define OS_KILLSCHEDULEFUNC				&OS_KillSchedule

#include <memmap/Os_mm_const_begin.h>
extern const os_scheduleentry_t OS_stEntries[OS_NSTENTRIES];
const os_scheduleentry_t OS_stEntries[OS_NSTENTRIES] = { OS_STENTRIES };
/* Deviation MISRAC2012-4 */
static const os_schedule_t OS_schedule[OS_NSCHEDULES] = { OS_STCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NSCHEDULES==0 */

#include <memmap/Os_mm_const8_begin.h>
const os_scheduleid_t OS_nSchedules = OS_NSCHEDULES;
#include <memmap/Os_mm_const8_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_schedule_t * const OS_scheduleTableBase = OS_SCHEDULEBASE;
const os_killschedulefunc_t OS_killScheduleFunc = OS_KILLSCHEDULEFUNC;
const os_synchronizefunc_t OS_synchronizeFunc = OS_SYNCHRONIZEFUNC;
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_nResources
 *
 * This constant contains the number of resources configured.
 *
 * OS_resource
 *
 * This array contains the static part of the configured resources.
*/
#if OS_NRESOURCES==0

#define OS_K_GETRESOURCE			&OS_KernUnknownSyscall
#define OS_K_RELEASERESOURCE		&OS_KernUnknownSyscall
#define OS_RESBASE					OS_NULL

#else

#define OS_CFG_INITRESOURCES		&OS_InitResources
#define OS_RESBASE					OS_resource

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
static const os_resource_t OS_resource[OS_NRESOURCES] = { OS_RESOURCES };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NRESOURCES==0 */

#include <memmap/Os_mm_const_begin.h>
const os_resource_t * const OS_resourceTableBase = OS_RESBASE;
#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_const8_begin.h>
const os_resourceid_t OS_nResources = OS_NRESOURCES;
#include <memmap/Os_mm_const8_end.h>

/*!
 * OS_nInterrupts
 * OS_isrTable
 *
 * OS_nInterrupts is the number of interrupts configured - 0 to 255
 * OS_isrTable is the table describing the interrupts to the kernel.
 */
#if OS_NINTERRUPTS==0

#define OS_ISRBASE				OS_NULL

#define OS_KILLISRFUNC					OS_NULL
#define OS_K_DISABLEINTERRUPTSOURCE		&OS_KernUnknownSyscall
#define OS_K_ENABLEINTERRUPTSOURCE		&OS_KernUnknownSyscall

#define OS_XC_RECVDISABLEINTERRUPTSOURCE	&OS_RecvUnknownCall
#define OS_XC_RECVENABLEINTERRUPTSOURCE		&OS_RecvUnknownCall

#else

#define OS_CFG_INITINTERRUPTS	&OS_InitInterrupts
#define OS_ISRBASE				OS_isrTable

#ifdef OS_EXCLUDE_KILLISR
#define OS_KILLISRFUNC			OS_NULL
#else
#define OS_KILLISRFUNC			&OS_KillIsr
#endif

#if ((OS_CONFIGMODE & OS_STACKCHECK) != 0)
#define OS_CFG_INITISRSTACKS	&OS_InitIsrStacks
#endif /* ((OS_CONFIGMODE & OS_STACKCHECK) != 0) */

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-4 */
static const os_isr_t OS_isrTable[OS_NINTERRUPTS] = { OS_ISRCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NINTERRUPTS==0 */

#include <memmap/Os_mm_const_begin.h>
const os_isr_t * const OS_isrTableBase = OS_ISRBASE;
const os_killisrfunc_t OS_killIsrFunc = OS_KILLISRFUNC;
const os_isrid_t OS_nInterrupts = OS_NINTERRUPTS;
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_NAPPSNONPRIV
 *
 * This macro defines the number of non-privileged applications in the system.
 * Its only action is to enable protected mode.
 *
 * Protected mode is also not enabled if the field OS_DBGPROT
 * of the macro OS_CONFIGMODE is set to OS_DBGPROT_OFF
 * The Generator sets this field in response to options in the OIL
 * file.
 *
 * Some architectures may need to always enter protected mode. These can define
 * OS_CFG_INITPROTECTED unconditionally.
*/
#if (OS_NAPPSNONPRIV != 0)

#if (OS_HASMEMPROT != 0) && ((OS_CONFIGMODE & OS_DBGPROT) != OS_DBGPROT_OFF)
#ifndef OS_CFG_INITPROTECTED
#define OS_CFG_INITPROTECTED	&OS_EnterProtectedMode
#endif
#endif

#endif /* OS_NAPPSNONPRIV != 0 */

/*!
 * Accounting
 *
 * Various macros define the different types of timing that are in progress:
 *  - basic execution timing for tasks and ISRs
 *  - interrupt lock timing for tasks and ISRs
 *  - resource lock timing
 *
 * Fortunately we can simplify the decisions somewhat:
 *  - if there are any tasks with accounting structures, we need start/stop/preempt for tasks.
 *  - if there are any ISRs with accounting structures we need start/stop/preempt/resume for ISRs.
 *  - if any tasks have interrupt lock timing we need start/stop int lock timing for tasks
 *  - if any ISRs have interrupt lock timing we need start/stop int lock timing for ISRs
 *  - if any tasks or ISRs have resource lock timing we need start/stop res lock timing
 *  - if there's any timing at all we need the init function.
*/
#if ( (OS_NTASKACCOUNTING == 0) && (OS_NISRACCOUNTING == 0) )

#define OS_HASEXECTIMING			0

#else

#define OS_HASEXECTIMING			1
#define OS_CFG_INITEXECTIME			&OS_InitExecutionTiming

#endif

#if ( OS_NTASKACCOUNTING == 0 )

#define OS_CFG_STARTTASKTIMING		(os_starttaskexectimingfp_t)&OS_NullFunction
#define OS_CFG_STOPTASKTIMING		(os_stoptaskexectimingfp_t)&OS_NullFunction
#define OS_CFG_PREEMPTTASKTIMING	(os_preempttaskexectimingfp_t)&OS_NullFunction

#else

#define OS_CFG_STARTTASKTIMING		&OS_StartTaskExecTiming
#define OS_CFG_STOPTASKTIMING		&OS_StopTaskExecTiming
#define OS_CFG_PREEMPTTASKTIMING	&OS_PreemptTaskExecTiming

#endif

#if ( OS_NISRACCOUNTING == 0 )

#define OS_CFG_STARTISRTIMING		(os_startisrexectimingfp_t)&OS_NullFunction
#define OS_CFG_PREEMPTISRTIMING		(os_preemptisrexectimingfp_t)&OS_NullFunction
#define OS_CFG_RESUMEISRTIMING		(os_resumeisrexectimingfp_t)&OS_NullFunction
#define OS_CFG_STOPISRTIMING		(os_stopisrexectimingfp_t)&OS_NullFunction

#else

#define OS_CFG_STARTISRTIMING		&OS_StartIsrExecTiming
#define OS_CFG_PREEMPTISRTIMING		&OS_PreemptIsrExecTiming
#define OS_CFG_RESUMEISRTIMING		&OS_ResumeIsrExecTiming
#define OS_CFG_STOPISRTIMING		&OS_StopIsrExecTiming

#endif

#if ( OS_NTASKSINTLOCKLIMIT == 0 )

#define OS_CFG_STARTTASKILOCKTIMING	(os_starttaskintlocktimingfp_t)&OS_NullFunction
#define OS_CFG_STOPTASKILOCKTIMING	(os_stoptaskintlocktimingfp_t)&OS_NullFunction

#else

#define OS_CFG_STARTTASKILOCKTIMING	&OS_StartTaskIntLockTiming
#define OS_CFG_STOPTASKILOCKTIMING	&OS_StopTaskIntLockTiming

#endif

#if ( OS_NISRSINTLOCKLIMIT == 0 )

#define OS_CFG_STARTISRILOCKTIMING	(os_startisrintlocktimingfp_t)&OS_NullFunction
#define OS_CFG_STOPISRILOCKTIMING	(os_stopisrintlocktimingfp_t)&OS_NullFunction

#else

#define OS_CFG_STARTISRILOCKTIMING	&OS_StartIsrIntLockTiming
#define OS_CFG_STOPISRILOCKTIMING	&OS_StopIsrIntLockTiming

#endif

#if ( (OS_NTASKSRESLOCKLIMIT == 0) && (OS_NISRSRESLOCKLIMIT == 0) )

#define OS_CFG_STARTRESLOCKTIMING	(os_startreslocktimingfp_t)&OS_NullFunction
#define OS_CFG_STOPRESLOCKTIMING	(os_stopreslocktimingfp_t)&OS_NullFunction

#else

#define OS_CFG_STARTRESLOCKTIMING	&OS_StartResLockTiming
#define OS_CFG_STOPRESLOCKTIMING	&OS_StopResLockTiming

#endif

/*!
 * OS_executionTimer
 *
 * Execution budget monitoring.
 */
#include <memmap/Os_mm_const_begin.h>
const os_hwt_t * const OS_executionTimer[OS_N_CORES_MAX] = { OS_EXECUTIONTIMER_INIT };
#include <memmap/Os_mm_const_end.h>

#if (!OS_USE_OPTIMIZATION_OPTIONS) || (!defined(OS_EXCLUDE_TIMINGPROTECTION))

/* Deviation MISRAC2012-1 <START> */
const os_starttaskexectimingfp_t	OS_startTaskExecTimingFp	= OS_CFG_STARTTASKTIMING;
const os_preempttaskexectimingfp_t	OS_preemptTaskExecTimingFp	= OS_CFG_PREEMPTTASKTIMING;
const os_stoptaskexectimingfp_t		OS_stopTaskExecTimingFp		= OS_CFG_STOPTASKTIMING;
const os_starttaskintlocktimingfp_t	OS_startTaskIntLockTimingFp	= OS_CFG_STARTTASKILOCKTIMING;
const os_stoptaskintlocktimingfp_t	OS_stopTaskIntLockTimingFp	= OS_CFG_STOPTASKILOCKTIMING;
const os_startisrexectimingfp_t		OS_startIsrExecTimingFp		= OS_CFG_STARTISRTIMING;
const os_preemptisrexectimingfp_t	OS_preemptIsrExecTimingFp	= OS_CFG_PREEMPTISRTIMING;
const os_resumeisrexectimingfp_t	OS_resumeIsrExecTimingFp	= OS_CFG_RESUMEISRTIMING;
const os_stopisrexectimingfp_t		OS_stopIsrExecTimingFp		= OS_CFG_STOPISRTIMING;
const os_startisrintlocktimingfp_t	OS_startIsrIntLockTimingFp	= OS_CFG_STARTISRILOCKTIMING;
const os_stopisrintlocktimingfp_t	OS_stopIsrIntLockTimingFp	= OS_CFG_STOPISRILOCKTIMING;
const os_startreslocktimingfp_t		OS_startResLockTimingFp		= OS_CFG_STARTRESLOCKTIMING;
const os_stopreslocktimingfp_t		OS_stopResLockTimingFp		= OS_CFG_STOPRESLOCKTIMING;
/* Deviation MISRAC2012-1 <STOP> */

#endif /* !OS_USE_OPTIMIZATION_OPTIONS) || (!defined(OS_EXCLUDE_TIMINGPROTECTION) */

/*!
 * OS_rateIndex
 * OS_rateTimer
 *
 * The dynamic state of all rate-monitors. The index array (one per
 * rate-monitor) is the index into the rate-monitor's own ring buffer.
 * The timer array is the aggregated ring buffer array, COUNTLIMIT
 * entries per rate-monitor.
*/
#if OS_NRATEMONS != 0

#if (OS_N_CORES == 1)
#include <memmap/Os_mm_var_begin.h>
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_rateIndex, .bss.shared.os.OS_rateIndex)
os_rateindex_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_rateIndex)
		OS_rateIndex[OS_NRATEMONS]
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_rateIndex);
/* CHECK: PARSE */
static os_timestamp_t OS_rateTimer[OS_NRATETIMERS];
#include <memmap/Os_mm_var_end.h>
#else
#include <memmap/Os_mm_const_begin.h>
os_rateindex_t * const OS_rateIndex_ptr[OS_NRATEMONS] = { OS_RATEINDEXPTR_INIT };
#include <memmap/Os_mm_const_end.h>
#endif /* OS_N_CORES == 1 */

#endif /* OS_NRATEMONS != 0 */

/*!
 * OS_rateMonitor
 * OS_rateMonitorFunc
 *
 * OS_rateMonitor is the array of rate-monitors. If the size of the array
 * is 0 the array does not exist and the rate-monitor and rate monitor
 * initialisation function pointers are set to OS_NULL.
 *
 * OS_rateMonitorFunc is the address of the rate-monitor function, or
 * OS_NULL of no rate-monitoring is configured.
*/
#if OS_NRATEMONS == 0

#define OS_CFG_RATEMONITOR	OS_NULL

#else

#define OS_CFG_RATEMONITOR	&OS_RateMonitor

#include <memmap/Os_mm_const_begin.h>
const os_ratemonitor_t OS_rateMonitor[OS_NRATEMONS] = { OS_RATEMONCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NRATEMONS == 0 */

#include <memmap/Os_mm_const_begin.h>
const os_ratemonitorid_t OS_nRateMonitors = OS_NRATEMONS;
#include <memmap/Os_mm_const_end.h>

#if (!OS_USE_OPTIMIZATION_OPTIONS)

#include <memmap/Os_mm_const_begin.h>
const os_ratemonitorfunc_t OS_rateMonitorFunc = OS_CFG_RATEMONITOR;
#include <memmap/Os_mm_const_end.h>

#endif /* !OS_USE_OPTIMIZATION_OPTIONS */

/*!
 * OS_resourceLockTimes
 *
 * This is the (possibly optimised) table of resource lock times that
 * is referenced from the task and ISR tables.
*/
#if (OS_NRESLOCKTIMES != 0)

#include <memmap/Os_mm_const_begin.h>
const os_tick_t OS_resourceLockTimes[OS_NRESLOCKTIMES] = { OS_RESLOCKTIMES };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NRESLOCKTIMES != 0 */

/*!
 * OS_CFG_POSTINITARCH
 *
 * Allows post initialisation of specific hardware
 *
 * This is defined by the architecture ...
*/

/*!
 * OS_CFG_STARTTICKERS
 *
 * Allows a generated function void OS_StartTickers(void) to start all the internal drivers and GPT channels
 * that are configured to 'tick' software counters.
 * OS_StartTickers() must be called after all hardware initialisation is done.
 *
 * CAVEAT:
 * We assume that Gpt_Init() has already been called, which is not particularly pleasant.
 * We also assume that the Gpt API functions can be called safely from within the kernel. Since the
 * MCAL drivers and the OS are not tightly integrated this might not be the case. If there are
 * problems, address your complaints to those responsible for the Autosar Layered Architecture.
 *
 * If there are problems with any of the above points, define the macro OS_INHIBIT_GPT_STARTUP = 1 in
 * board.h, and call the function OS_StartGptTickers() from a task (typically the Init task), after
 * Gpt_Init() has been called.
 *
*/
#ifndef OS_INHIBIT_GPT_STARTUP
#define OS_INHIBIT_GPT_STARTUP	0
#endif

#ifndef OS_CFG_STARTTICKERS

#if (OS_NINTERNALTICKERS != 0)

#define OS_CFG_STARTTICKERS		(&OS_StartTickers)

#elif (!OS_INHIBIT_GPT_STARTUP) && (OS_NGPTTICKERS != 0)

#define OS_CFG_STARTTICKERS		(&OS_StartTickers)

#endif /* OS_NINTERNALTICKERS != 0 */

#endif /* OS_CFG_STARTTICKERS */

/*!
 * Time-stamp timer
 *
 * Time stamp initialization function to be used during startup, i.e. in StartOS() via OS_initFunc[].
 * In case the global variant of the time-stamp timer is used, the global variables are also defined here.
 */
#ifdef OS_INITTIMESTAMP
#define OS_CFG_INITTIMESTAMP	OS_INITTIMESTAMP
#endif

#if (OS_USEGENERICTIMESTAMP && OS_USE_GLOBAL_GENERICTIMESTAMP)
#ifdef OS_TIMESTAMPTIMERINDEX
os_hwt_t const * const OS_timeStampTimer = &OS_hwTimer[OS_TIMESTAMPTIMERINDEX];
#else
os_hwt_t const * const OS_timeStampTimer = OS_NULL;
#endif
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_lastTimeStampTime, .bss.shared.os.OS_lastTimeStampTime)
os_timervalue_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_lastTimeStampTime)
		OS_lastTimeStampTime
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_lastTimeStampTime);
OS_SECTION_PRAGMA(OS_timeStampValue, .bss.shared.os.OS_timeStampValue)
os_timestamp_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_timeStampValue)
		OS_timeStampValue
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_timeStampValue);
/* CHECK: PARSE */
#endif	/* OS_USEGENERICTIMESTAMP && OS_USE_GLOBAL_GENERICTIMESTAMP */

#ifdef OS_EXCLUDE_CPULOAD

/* If the load measurement is excluded from the build, there's
 * no need for the configuration structure
*/
#define OS_K_GETCPULOAD					&OS_KernUnknownSyscall
#define OS_K_RESETPEAKCPULOAD			&OS_KernUnknownSyscall
#define OS_XC_RETNCPULOAD				&OS_RecvUnknownCall
#define OS_XC_RECVMEASURECPULOAD		&OS_RecvUnknownCall

#else

#include <memmap/Os_mm_const_begin.h>
os_tick_t * const OS_cpuLoadBusyBuffer_ptr[OS_N_CORES_MAX] =
{
	OS_MAKE_COREARRAY_FROM(OS_CPULOADBUSYBUFFERPTR_CORE)
};
#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_cpuloadcfg_t OS_cpuLoadCfg =
{
	OS_CPULOADCFG_INTERVAL,
	OS_CPULOADCFG_NINTERVALS,
	OS_CPULOADCFG_WINDOW,
	OS_CPULOADCFG_ROUNDING,
	OS_CPULOADCFG_BUSYOVERFLOWLIMIT,
	OS_CPULOADCFG_WINDOW100,
	OS_CPULOADCFG_ROUNDING100
};
#include <memmap/Os_mm_const_end.h>

#endif /* OS_EXCLUDE_CPULOAD */

/*!
 * OS_crosscoreMessageQueue_ptr
 *
 * Inter-core message queues.
 */
#if (OS_N_CORES > 1)
#define	OS_CFG_INITMESSAGEQUEUE	&OS_InitMessageQueue
os_messagequeue_t* const OS_crosscoreMessageQueue_ptr[OS_N_CORES_MAX] =
{
	OS_MAKE_COREARRAY_FROM(OS_MESSAGEQUEUEPTR_CORE)
};
#endif /* OS_N_CORES > 1 */

/*!
 * OS_messageHandler
 *
 * This is the table of cross-core message handler functions used by the multicore kernel
 * to route OS system calls to the correct function.
 * The entries in here must be in exactly the right order as given by
 * the indices in Os_message.h
 *
 * Entry 0 (UnknownCall) is always configured.
 *
 * We start by defining macros for all known message calls, but only if they
 * are not already defined. The configuration sections above can define
 * the macros as OS_RecvUnknownCall, if the feature is not required.
 *
 * The whole table is omitted for single-core systems
*/
#if (OS_N_CORES > 1)
#ifndef OS_XC_RETNRESULT
#define OS_XC_RETNRESULT						&OS_RetnResult
#endif

#ifndef OS_XC_RETNTASKSTATE
#define OS_XC_RETNTASKSTATE						&OS_RetnTaskState
#endif

#if ((OS_NALARMS == 0) && (OS_NCOUNTERS == 0))
#define OS_XC_RETNTICK							&OS_RecvUnknownCall
#else
#define OS_XC_RETNTICK							&OS_RetnTick
#endif

#ifndef OS_XC_RETNEVENT
#define OS_XC_RETNEVENT							&OS_RetnEvent
#endif

#ifndef OS_XC_RETNSCHEDULETABLESTATUS
#define OS_XC_RETNSCHEDULETABLESTATUS			&OS_RetnScheduleTableStatus
#endif

#ifndef OS_XC_RETNAPPLICATIONSTATE
#define OS_XC_RETNAPPLICATIONSTATE				&OS_RetnApplicationState
#endif

#ifndef OS_XC_RETNCPULOAD
#define OS_XC_RETNCPULOAD						&OS_RetnCpuLoad
#endif

#ifndef OS_XC_RECVACTIVATETASK
#define OS_XC_RECVACTIVATETASK					&OS_RecvActivateTask
#endif

#ifndef OS_XC_RECVGETTASKSTATE
#define OS_XC_RECVGETTASKSTATE					&OS_RecvGetTaskState
#endif

#ifndef OS_XC_RECVSETEVENT
#define OS_XC_RECVSETEVENT						&OS_RecvSetEvent
#endif

#ifndef OS_XC_RECVGETEVENT
#define OS_XC_RECVGETEVENT						&OS_RecvGetEvent
#endif

#ifndef OS_XC_RECVGETALARM
#define OS_XC_RECVGETALARM						&OS_RecvGetAlarm
#endif

#ifndef OS_XC_RECVSETRELALARM
#define OS_XC_RECVSETRELALARM					&OS_RecvSetRelAlarm
#endif

#ifndef OS_XC_RECVSETABSALARM
#define OS_XC_RECVSETABSALARM					&OS_RecvSetAbsAlarm
#endif

#ifndef OS_XC_RECVCANCELALARM
#define OS_XC_RECVCANCELALARM					&OS_RecvCancelAlarm
#endif

#ifndef OS_XC_RECVSTARTSCHEDULETABLE
#define OS_XC_RECVSTARTSCHEDULETABLE			&OS_RecvStartScheduleTable
#endif

#ifndef OS_XC_RECVCHAINSCHEDULETABLE
#define OS_XC_RECVCHAINSCHEDULETABLE			&OS_RecvChainScheduleTable
#endif

#ifndef OS_XC_RECVSTOPSCHEDULETABLE
#define OS_XC_RECVSTOPSCHEDULETABLE				&OS_RecvStopScheduleTable
#endif

#ifndef OS_XC_RECVSYNCSCHEDULETABLE
#define OS_XC_RECVSYNCSCHEDULETABLE				&OS_RecvSyncScheduleTable
#endif

#ifndef OS_XC_RECVSETSCHEDULETABLEASYNC
#define OS_XC_RECVSETSCHEDULETABLEASYNC			&OS_RecvSetScheduleTableAsync
#endif

#ifndef OS_XC_RECVGETSCHEDULETABLESTATUS
#define OS_XC_RECVGETSCHEDULETABLESTATUS		&OS_RecvGetScheduleTableStatus
#endif

#ifndef OS_XC_RECVDISABLEINTERRUPTSOURCE
#define OS_XC_RECVDISABLEINTERRUPTSOURCE		&OS_RecvDisableInterruptSource
#endif

#ifndef OS_XC_RECVENABLEINTERRUPTSOURCE
#define OS_XC_RECVENABLEINTERRUPTSOURCE			&OS_RecvEnableInterruptSource
#endif

#ifndef OS_XC_RECVGETCOUNT
#define OS_XC_RECVGETCOUNT						&OS_RecvGetCount
#endif

#ifndef OS_XC_RECVADVANCECOUNTER
#define OS_XC_RECVADVANCECOUNTER				&OS_RecvAdvanceCounter
#endif

#ifndef OS_XC_RECVSTARTSCHEDULETABLESYNCHRON
#define OS_XC_RECVSTARTSCHEDULETABLESYNCHRON	&OS_RecvStartScheduleTableSynchron
#endif

#ifndef OS_XC_RECVTERMINATEAPPLICATION
#define OS_XC_RECVTERMINATEAPPLICATION			&OS_RecvTerminateApplication
#endif

#ifndef OS_XC_RECVGETAPPLICATIONSTATE
#define OS_XC_RECVGETAPPLICATIONSTATE			&OS_RecvGetApplicationState
#endif

#if (OS_N_CORES == 1)
#define OS_XC_RECVSHUTDOWNCORE					&OS_RecvUnknownCall
#else
#define OS_XC_RECVSHUTDOWNCORE					&OS_RecvShutdownCore
#endif

#ifndef OS_XC_RECVMEASURECPULOAD
#define OS_XC_RECVMEASURECPULOAD				&OS_RecvMeasureCpuLoad
#endif

#include <memmap/Os_mm_const_begin.h>
const os_messagehandler_t OS_messageHandler[OS_N_CROSSCORE_HANDLERS] =
{
	&OS_RecvUnknownCall,					/*  0 */
	OS_XC_RECVACTIVATETASK,					/*  1 */
	OS_XC_RECVGETTASKSTATE,					/*  2 */
	OS_XC_RECVSETEVENT,						/*  3 */
	OS_XC_RECVGETEVENT,						/*  4 */
	OS_XC_RECVGETALARM,						/*  5 */
	OS_XC_RECVSETRELALARM,					/*  6 */
	OS_XC_RECVSETABSALARM,					/*  7 */
	OS_XC_RECVCANCELALARM,					/*  8 */
	OS_XC_RECVSTARTSCHEDULETABLE,			/*  9 */
	OS_XC_RECVCHAINSCHEDULETABLE,			/* 10 */
	OS_XC_RECVSTOPSCHEDULETABLE,			/* 11 */
	OS_XC_RECVSYNCSCHEDULETABLE,			/* 12 */
	OS_XC_RECVSETSCHEDULETABLEASYNC,		/* 13 */
	OS_XC_RECVGETSCHEDULETABLESTATUS,		/* 14 */
	OS_XC_RECVDISABLEINTERRUPTSOURCE,		/* 15 */
	OS_XC_RECVENABLEINTERRUPTSOURCE,		/* 16 */
	OS_XC_RECVGETCOUNT,						/* 17 */
	OS_XC_RECVADVANCECOUNTER,				/* 18 */
	OS_XC_RECVSTARTSCHEDULETABLESYNCHRON,	/* 19 */
	OS_XC_RECVTERMINATEAPPLICATION,			/* 20 */
	OS_XC_RECVGETAPPLICATIONSTATE,			/* 21 */
	OS_XC_RECVSHUTDOWNCORE,					/* 22 */
	OS_XC_RECVMEASURECPULOAD,				/* 23 */
	OS_XC_RETNRESULT,						/* 24 */
	OS_XC_RETNTASKSTATE,					/* 25 */
	OS_XC_RETNTICK,							/* 26 */
	OS_XC_RETNEVENT,						/* 27 */
	OS_XC_RETNSCHEDULETABLESTATUS,			/* 28 */
	OS_XC_RETNAPPLICATIONSTATE,				/* 29 */
	OS_XC_RETNCPULOAD						/* 30 */
};
#endif /* OS_N_CORES > 1 */

/*!
 * OS kernel data.
 *
 * OS kernel data local to each core. Hence, each core has its own instance,
 * which needs not to be synchronized with the ones of the other cores (if any).
 */
#include <memmap/Os_mm_const_begin.h>
/* This array is always defined, even when there is only one core. The reason for this is, that the assembly
 * language code parts of this OS can always rely on its availability. This is beneficial for some architectures.
 */
os_kerneldata_t * const OS_kernel_ptr[OS_N_CORES_MAX] =
{
	OS_MAKE_COREARRAY_FROM(OS_KERNELDATAPTR_CORE)
};
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_coreStarted
 *
 * This array records, which cores have already been started. The init-core entry is effectively
 * not used, because the core with ID OS_INITCORE is always started "automatically" (by hardware, another OS, etc).
 * Therefore, 'OS_coreStarted[OS_INITCORE] == OS_FALSE' always holds true.
 * This array is protected by the lock OS_CORESTARTED_LOCKID, which is only relevant during
 * startup. Once all cores have been started this array may be treated as read-only and
 * hence may be accessed without grabbing the lock first.
 */
#include <memmap/Os_mm_var_begin.h>
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_coreStarted, .bss.shared.os.OS_coreStarted)
os_boolean_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_coreStarted)
		OS_coreStarted[OS_N_CORES_MAX]
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_coreStarted);
/* CHECK: PARSE */
#include <memmap/Os_mm_var_end.h>

/*!
 * IOC
 *
 * Function to integrate with the AUTOSAR IOC mechanism.
 */
#if (OS_IOC_ENABLED==1)

#define OS_K_IOCWRITE			&IOC_SysWrite
#define OS_K_IOCSEND			&IOC_SysSend
#define OS_K_EMPTYQUEUE			&IOC_SysEmptyQueue
#define OS_K_IOCADDONEXT		&IOC_SysOsExtDemux
#define OS_IOC_STARTUPCHECKS	&OS_IocStartupChecksWrapper
#define OS_CFG_INIT_IOC			&IOC_InitializeWithoutChecks
static os_result_t OS_IocStartupChecksWrapper(void)
{
	os_result_t result;
	if (OS_GetMyCoreId() == OS_INITCORE)
	{
		result = (os_result_t) IOC_StartupChecks();
	}
	else
	{
		/* Possible diagnostic TOOLDIAG-1 <+1> */
		result = OS_E_OK;
	}
	return result;
}
#else

#define OS_K_IOCWRITE		&OS_KernUnknownSyscall
#define OS_K_IOCSEND		&OS_KernUnknownSyscall
#define OS_K_EMPTYQUEUE		&OS_KernUnknownSyscall
#define OS_K_IOCADDONEXT	&OS_KernUnknownSyscall


#endif /* OS_IOC_ENABLED==1 */

/*
 * An array of software locks for internal use.
 *
 * The number of hardware locks is defined by the architecture/derivative.
 * If the total number of locks required is less than or equal to the number of hardware locks, we can implement
 * them all in hardware. Otherwise we need an array of additional software locks. In the latter case, one hardware
 * lock is used to protect the software locks. This is why the number OS_NHARDWARELOCKS is reduced by one, before
 * the difference to OS_NKERNELLOCKS is calculated to yield the total number of software locks OS_NSOFTWARELOCKS.
 * If we don't need any software locks the array is still declared to keep the linker happy.
 * Of course, the software locks must be in shared memory. Each hardware/software lock is identified by an instance
 * of os_lockid_t.
*/
#if (OS_N_CORES > 1)

#if OS_NKERNELLOCKS > OS_NHARDWARELOCKS
#define OS_NSOFTWARELOCKS	(OS_NKERNELLOCKS - (OS_NHARDWARELOCKS - 1U))
#else
#define OS_NSOFTWARELOCKS	1U
#endif

#include <memmap/Os_mm_var_begin.h>
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_softwareLock, .bss.shared.os.OS_softwareLock)
OS_ALIGNED_PRAGMA(OS_softwareLock, OS_SYNCSPOT_ALIGNMENT_GRANULE)
os_syncspot_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_softwareLock)
		OS_ALIGNED_ATTRIB_PRE(OS_SYNCSPOT_ALIGNMENT_GRANULE)
/* Deviation MISRAC2012-7 <1> */
		OS_softwareLock[OS_NSOFTWARELOCKS]
		OS_ALIGNED_ATTRIB_POST(OS_SYNCSPOT_ALIGNMENT_GRANULE)
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_softwareLock);
/* CHECK: PARSE */
#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-7 <1> */
const os_uint_t OS_nSoftwareLocks = OS_NSOFTWARELOCKS;
#include <memmap/Os_mm_const_end.h>

#endif /* OS_N_CORES > 1 */

/*
 * AUTOSAR spinlocks.
 *
 * An array of spinlocks implemented to the AUTOSAR specification. The dynamic array must be in shared memory.
*/
#if (OS_N_CORES == 1)

#define OS_K_TRYTOGETSPINLOCK			&OS_KernUnknownSyscall
#define OS_K_RELEASESPINLOCK			&OS_KernUnknownSyscall

#else

#if OS_NAUTOSARSPINLOCKS == 0

#define OS_K_TRYTOGETSPINLOCK			&OS_KernUnknownSyscall
#define OS_K_RELEASESPINLOCK			&OS_KernUnknownSyscall
#define OS_AUTOSARSPINLOCKTABLEBASE		OS_NULL

#else

#define OS_CFG_INITAUTOSARSPINLOCKS		&OS_InitAutosarSpinlocks
#define OS_AUTOSARSPINLOCKTABLEBASE		OS_autosarSpinlockTable

/* Note, the array OS_autosarSpinlockDynamic[] is referenced by the macro OS_AUTOSARSPINLOCKCONFIG.
 * Hence, it must be defined first.
 */
#include <memmap/Os_mm_var_begin.h>
/* CHECK: NOPARSE */
OS_SECTION_PRAGMA(OS_autosarSpinlockDynamic, .bss.shared.os.OS_autosarSpinlockDynamic)
static os_autosarspinlockdyn_t
		OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_autosarSpinlockDynamic)
		OS_autosarSpinlockDynamic[OS_NAUTOSARSPINLOCKS]
		OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_autosarSpinlockDynamic);
/* CHECK: PARSE */
#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_const_begin.h>
static const os_autosarspinlock_t OS_autosarSpinlockTable[OS_NAUTOSARSPINLOCKS] = { OS_AUTOSARSPINLOCKCONFIG };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NAUTOSARSPINLOCKS == 0 */

#include <memmap/Os_mm_const_begin.h>
const os_autosarspinlockid_t OS_nAutosarSpinlocks = OS_NAUTOSARSPINLOCKS;
const os_autosarspinlock_t * const OS_autosarSpinlockTableBase = OS_AUTOSARSPINLOCKTABLEBASE;
#include <memmap/Os_mm_const_end.h>

#endif /* OS_N_CORES == 1 */

/* Assert that the ranges of the types os_error_t and os_result_t are
 * sub-ranges of the type os_errorresult_t. See comment at definition
 * of OS_ERROR_* macros.
*/
/* CHECK: NOPARSE */
/* Deviation MISRAC2012-3 <2> */
OS_STATIC_ASSERT(sizeof(os_error_t) <= sizeof(os_errorresult_t), OS_errorresult_sizecheck0);
OS_STATIC_ASSERT(sizeof(os_result_t) <= sizeof(os_errorresult_t), OS_errorresult_sizecheck1);
/* CHECK: PARSE */

/*!
 * OS_startupCheckFunc
 *
 * This array contains pointers to functions that will
 * be called by OS_StartupChecks().
 *
 * If the startup checks are not enabled the OS_StartupChecks() function is not
 * called, so the array isn't needed.
*/
#if (OS_CONFIGMODE & OS_STARTUPCHECK)

#define OS_CFG_STARTUPCHECKS	OS_StartupChecks

#include <memmap/Os_mm_const_begin.h>
os_schkfunc_t const OS_startupCheckFunc[] =
{
#if ((OS_ARCH != OS_WINDOWS) && (OS_ARCH != OS_LINUX))
	/* There are no assembler code parts for Windows and Linux. Hence, checking assembler offsets is futile then. */
	OS_StartupChecksAssemblerOffsets,
#endif

#if (OS_NAPPS != 0)
	OS_StartupChecksApplication,
#endif

#if (OS_NFUNCTIONS != 0)
	OS_StartupChecksTrustedFunction,
#endif

#if (OS_NTASKS != 0)
	OS_StartupChecksTask,
#endif

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
	OS_StartupChecksQueue,
#endif

#if (OS_NCOUNTERS != 0)
	OS_StartupChecksCounter,
#endif

#if (OS_NALARMS != 0)
	OS_StartupChecksAlarm,
#endif

#if (OS_NHWTIMERS != 0)
	OS_StartupChecksTimer,
#endif

#if (OS_NSCHEDULES != 0)
	OS_StartupChecksSchedule,
#endif

#if (OS_NRESOURCES != 0)
	OS_StartupChecksResource,
#endif

#if (OS_NINTERRUPTS != 0)
	OS_StartupChecksIsr,
#endif

#ifdef OS_ARCH_STARTUPCHECKS
	OS_ARCH_STARTUPCHECKS,
#endif

#ifdef OS_IOC_STARTUPCHECKS
	OS_IOC_STARTUPCHECKS,
#endif

	OS_NULL			/* Terminator: must be at end */
};
#include <memmap/Os_mm_const_end.h>

#endif /* OS_CONFIGMODE & OS_STARTUPCHECK */

/*!
 * OS_initFunc
 *
 * This array contains pointers to initialisation functions that will
 * be called by OS_KernStartOs(). The entries will all be
 * specific initialisation functions.
 * The array is initialised using macros that are defined earlier in
 * this file depending on the generated configuration. Exception:
 * OS_CFG_INITARCH is normally defined by the architecture.
 * Macros that are not defined are not included in the list.
 * The list is null-terminated.
 *
 * This file therefore defines which init functions are called, and in
 * what order.
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Autostart.Order, 1
*/
#include <memmap/Os_mm_const_begin.h>
os_initfunc_t const OS_initFunc[] =
{
#ifdef OS_CFG_STARTUPCHECKS
	OS_CFG_STARTUPCHECKS,
#endif

#ifdef OS_CFG_INITARCH
	OS_CFG_INITARCH,
#endif

#ifdef OS_CFG_INITMESSAGEQUEUE
	OS_CFG_INITMESSAGEQUEUE,
#endif

#ifdef OS_CFG_INITAUTOSARSPINLOCKS
	OS_CFG_INITAUTOSARSPINLOCKS,
#endif

#ifdef OS_CFG_INITKERNSTACK
	OS_CFG_INITKERNSTACK,
#endif

#ifdef OS_CFG_INITAPPDATA
	OS_CFG_INITAPPDATA,
#endif

#ifdef OS_CFG_INITTASKSTACKS
	OS_CFG_INITTASKSTACKS,
#endif

#ifdef OS_CFG_INITISRSTACKS
	OS_CFG_INITISRSTACKS,
#endif

#ifdef OS_CFG_INITAPP
	OS_CFG_INITAPP,
#endif

#ifdef OS_CFG_INITTASKS
	OS_CFG_INITTASKS,
#endif

#ifdef OS_CFG_INITTIMERS
	OS_CFG_INITTIMERS,
#endif

#ifdef OS_CFG_INITCOUNTERS
	OS_CFG_INITCOUNTERS,
#endif

#ifdef OS_CFG_INITALARMS
	OS_CFG_INITALARMS,
#endif

#ifdef OS_CFG_INITSCHEDULES
	OS_CFG_INITSCHEDULES,
#endif

#ifdef OS_CFG_INITRESOURCES
	OS_CFG_INITRESOURCES,
#endif

#ifdef OS_CFG_INITINTERRUPTS
	OS_CFG_INITINTERRUPTS,
#endif

#ifdef OS_CFG_INITTIMESTAMP
	OS_CFG_INITTIMESTAMP,
#endif

#ifdef OS_CFG_ACTIVATEAUTOTASKS
	OS_CFG_ACTIVATEAUTOTASKS,
#endif

#ifdef OS_CFG_ACTIVATEAUTOALARMS
	OS_CFG_ACTIVATEAUTOALARMS,
#endif

#ifdef OS_CFG_ACTIVATEAUTOSCHEDULES
	OS_CFG_ACTIVATEAUTOSCHEDULES,
#endif

#ifdef OS_CFG_INITPROTECTED
	OS_CFG_INITPROTECTED,
#endif

#ifdef OS_CFG_INITEXECTIME
	OS_CFG_INITEXECTIME,
#endif

#ifdef OS_CFG_POSTINITARCH
	OS_CFG_POSTINITARCH,
#endif

#ifdef OS_CFG_STARTTICKERS
	OS_CFG_STARTTICKERS,
#endif

	/* !LINKSTO Kernel.Autosar.Multicore.StartOS.IOCInit, 1
	*/
#ifdef OS_CFG_INIT_IOC
	OS_CFG_INIT_IOC,
#endif

	OS_NULL
};
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_ptrXXXHook
 *
 * These pointers point to the hook functions if configured.
 * Otherwise, they can be OS_NULL or OS_NullFunction.
 *
 * The pointers are only present when the generic kernel library is in use. The
 * optimised libraries call the functions directly. This means that the optimised
 * library does not support the hook-renaming that the generic libraries (coupled with
 * this configuration file) support. However, the renaming is not conformamt with the
 * OSEK/AutosarOS standards, and is not supported by the generator.
 *
 * If the given macros are not defined, the pointers map to:
 *   void OS_NullFunction(void)								(StartupHook,PreTaskHook,PostTaskHook)
 *   void OS_NullErrorHook(os_result_t)						(ShutdownHook,ErrorHook)
 *   void OS_NullIsrHook(os_isrid_t)						(PreIsrHook,PostIsrHook)
 *   os_erroraction_t OS_NullProtectionHook(os_result_t)	(ProtectionHook)
 *
 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.NoHook, 2
*/
#if !OS_USE_OPTIMIZATION_OPTIONS

#ifndef OS_STARTUPHOOK
#define OS_STARTUPHOOK	&OS_NullFunction
#endif

#ifndef OS_SHUTDOWNHOOK
#define OS_SHUTDOWNHOOK	&OS_NullErrorHook
#endif

#ifndef OS_ERRORHOOK
#define OS_ERRORHOOK	&OS_NullErrorHook
#endif

#ifndef OS_PRETASKHOOK
#define OS_PRETASKHOOK	&OS_NullFunction
#endif

#ifndef OS_POSTTASKHOOK
#define OS_POSTTASKHOOK	&OS_NullFunction
#endif

#ifndef OS_PREISRHOOK
#define OS_PREISRHOOK	&OS_NullIsrHook
#endif

#ifndef OS_POSTISRHOOK
#define OS_POSTISRHOOK	&OS_NullIsrHook
#endif

#ifndef OS_PROTECTIONHOOK
#define OS_PROTECTIONHOOK	&OS_NullProtectionHook
#endif

#include <memmap/Os_mm_const_begin.h>
const os_startuphook_t		OS_ptrStartupHook =		OS_STARTUPHOOK;
const os_pretaskhook_t		OS_ptrPreTaskHook =		OS_PRETASKHOOK;
const os_posttaskhook_t		OS_ptrPostTaskHook =	OS_POSTTASKHOOK;
const os_preisrhook_t		OS_ptrPreIsrHook =		OS_PREISRHOOK;
const os_postisrhook_t		OS_ptrPostIsrHook =		OS_POSTISRHOOK;
const os_errorhook_t		OS_ptrErrorHook =		OS_ERRORHOOK;
const os_shutdownhook_t		OS_ptrShutdownHook =	OS_SHUTDOWNHOOK;
const os_protectionhook_t	OS_ptrProtectionHook =	OS_PROTECTIONHOOK;
#include <memmap/Os_mm_const_end.h>

#endif

/*!
 * Stack Checking
 *
 * If stack checking is not enabled, we can omit the stack info
 * system call.
 *
 * The initialisation of task and kernel stacks also depends on
 * the OS_STACKCHECK setting in OS_CONFIGMODE.
*/
#if ((OS_CONFIGMODE & OS_STACKCHECK) == 0)
#define OS_K_GETSTACKINFO		&OS_KernUnknownSyscall
#endif

/*!
 * IncrementCounter
 *
 * Do we need the IncrementCounter/IncrementCounter system call?
 */
#if (OS_NSWCOUNTERS == 0) /* No software counter present */
#define OS_K_INCREMENTCOUNTER		&OS_KernUnknownSyscall
#endif

/*!
 * OS_syscallTable
 *
 * This is the table of kernel functions used by the system-call dispatcher
 * to route OS system calls to the correct function.
 * The entries in here must be in exactly the right order as given by
 * the indices in Os_syscalls.h
 *
 * Entries 0 (UnknownSyscall) and 1 (StartOs) are always configured.
 *
 * We start by defining macros for all known system calls, but only if they
 * are not already defined. The configuration sections above can define
 * the macros as OS_KernUnknownSyscall if the feature is not
 * required.
 *
 * The whole system-call table is omitted for architectures that do not use it.
*/
#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL)

#ifndef OS_K_ACTIVATETASK
#define OS_K_ACTIVATETASK				&OS_KernActivateTask
#endif

#ifndef OS_K_TERMINATETASK
#define OS_K_TERMINATETASK				&OS_KernTerminateTask
#endif

#ifndef OS_K_CHAINTASK
#define OS_K_CHAINTASK					&OS_KernChainTask
#endif

#ifndef OS_K_SCHEDULE
#define OS_K_SCHEDULE					&OS_KernSchedule
#endif

#ifndef OS_K_GETTASKID
#define OS_K_GETTASKID					&OS_KernGetTaskId
#endif

#ifndef OS_K_GETTASKSTATE
#define OS_K_GETTASKSTATE				&OS_KernGetTaskState
#endif

#ifndef OS_K_SUSPENDINTERRUPTS
#define OS_K_SUSPENDINTERRUPTS			&OS_KernSuspendInterrupts
#endif

#ifndef OS_K_RESUMEINTERRUPTS
#define OS_K_RESUMEINTERRUPTS			&OS_KernResumeInterrupts
#endif

#ifndef OS_K_GETRESOURCE
#define OS_K_GETRESOURCE				&OS_KernGetResource
#endif

#ifndef OS_K_RELEASERESOURCE
#define OS_K_RELEASERESOURCE			&OS_KernReleaseResource
#endif

#ifndef OS_K_SETEVENT
#define OS_K_SETEVENT					&OS_KernSetEvent
#endif

#ifndef OS_K_CLEAREVENT
#define OS_K_CLEAREVENT					&OS_KernClearEvent
#endif

#ifndef OS_K_GETEVENT
#define OS_K_GETEVENT					&OS_KernGetEvent
#endif

#ifndef OS_K_WAITEVENT
#define OS_K_WAITEVENT					&OS_KernWaitEvent
#endif

#ifndef OS_K_GETALARMBASE
#define OS_K_GETALARMBASE				&OS_KernGetAlarmBase
#endif

#ifndef OS_K_GETALARM
#define OS_K_GETALARM					&OS_KernGetAlarm
#endif

#ifndef OS_K_SETRELALARM
#define OS_K_SETRELALARM				&OS_KernSetRelAlarm
#endif

#ifndef OS_K_SETABSALARM
#define OS_K_SETABSALARM				&OS_KernSetAbsAlarm
#endif

#ifndef OS_K_CANCELALARM
#define OS_K_CANCELALARM				&OS_KernCancelAlarm
#endif

#ifndef OS_K_GETACTIVEAPPLICATIONMODE
#define OS_K_GETACTIVEAPPLICATIONMODE	&OS_KernGetActiveApplicationMode
#endif

#ifndef OS_K_SHUTDOWNOS
#define OS_K_SHUTDOWNOS					&OS_KernShutdownOs
#endif

#ifndef OS_K_INCREMENTCOUNTER
#define OS_K_INCREMENTCOUNTER			&OS_KernIncrementCounter
#endif

#ifndef OS_K_GETCOUNTERVALUE
#define OS_K_GETCOUNTERVALUE			&OS_KernGetCounterValue
#endif

#ifndef OS_K_GETELAPSEDCOUNTERVALUE
#define OS_K_GETELAPSEDCOUNTERVALUE		&OS_KernGetElapsedCounterValue
#endif

#ifndef OS_K_GETSTACKINFO
#define OS_K_GETSTACKINFO				&OS_KernGetStackInfo
#endif

#ifndef OS_K_STARTSCHEDULETABLE
#define OS_K_STARTSCHEDULETABLE			&OS_KernStartScheduleTable
#endif

#ifndef OS_K_STARTSCHEDULETABLESYNCHRON
#define OS_K_STARTSCHEDULETABLESYNCHRON	&OS_KernStartScheduleTableSynchron
#endif

#ifndef OS_K_CHAINSCHEDULETABLE
#define OS_K_CHAINSCHEDULETABLE			&OS_KernChainScheduleTable
#endif

#ifndef OS_K_STOPSCHEDULETABLE
#define OS_K_STOPSCHEDULETABLE			&OS_KernStopScheduleTable
#endif

#ifndef OS_K_SYNCSCHEDULETABLE
#define OS_K_SYNCSCHEDULETABLE			&OS_KernSyncScheduleTable
#endif

#ifndef OS_K_SETSCHEDULETABLEASYNC
#define OS_K_SETSCHEDULETABLEASYNC		&OS_KernSetScheduleTableAsync
#endif

#ifndef OS_K_GETSCHEDULETABLESTATUS
#define OS_K_GETSCHEDULETABLESTATUS		&OS_KernGetScheduleTableStatus
#endif

#ifndef OS_K_TERMINATEAPPLICATION
#define OS_K_TERMINATEAPPLICATION		&OS_KernTerminateApplication
#endif

#ifndef OS_K_GETAPPLICATIONID
#define OS_K_GETAPPLICATIONID			&OS_KernGetApplicationId
#endif

#ifndef OS_K_GETISRID
#define OS_K_GETISRID					&OS_KernGetIsrId
#endif

#ifndef OS_K_CALLTRUSTEDFUNCTION
#define OS_K_CALLTRUSTEDFUNCTION		&OS_KernCallTrustedFunction
#endif

#ifndef OS_K_CHECKISRMEMORYACCESS
#define OS_K_CHECKISRMEMORYACCESS		&OS_KernCheckIsrMemoryAccess
#endif

#ifndef OS_K_CHECKTASKMEMORYACCESS
#define OS_K_CHECKTASKMEMORYACCESS		&OS_KernCheckTaskMemoryAccess
#endif

#ifndef OS_K_CHECKOBJECTACCESS
#define OS_K_CHECKOBJECTACCESS			&OS_KernCheckObjectAccess
#endif

#ifndef OS_K_CHECKOBJECTOWNERSHIP
#define OS_K_CHECKOBJECTOWNERSHIP		&OS_KernCheckObjectOwnership
#endif

#ifndef OS_K_DISABLEINTERRUPTSOURCE
#define OS_K_DISABLEINTERRUPTSOURCE		&OS_KernDisableInterruptSource
#endif

#ifndef OS_K_ENABLEINTERRUPTSOURCE
#define OS_K_ENABLEINTERRUPTSOURCE		&OS_KernEnableInterruptSource
#endif

#ifndef OS_K_GETCPULOAD
#define OS_K_GETCPULOAD					&OS_KernGetCpuLoad
#endif

#ifndef OS_K_RESETPEAKCPULOAD
#define OS_K_RESETPEAKCPULOAD			&OS_KernResetPeakCpuLoad
#endif

#ifndef OS_K_TASKRETURN
#define OS_K_TASKRETURN					&OS_KernUnknownSyscall
#endif

#ifndef OS_K_RETURNFROMCALL
#define OS_K_RETURNFROMCALL				&OS_KernUnknownSyscall
#endif

#ifndef OS_K_SIMTIMERADVANCE
#define OS_K_SIMTIMERADVANCE			&OS_KernUnknownSyscall
#endif

#ifndef OS_K_TRYTOGETSPINLOCK
#define OS_K_TRYTOGETSPINLOCK			&OS_KernTryToGetSpinlock
#endif

#ifndef OS_K_RELEASESPINLOCK
#define OS_K_RELEASESPINLOCK			&OS_KernReleaseSpinlock
#endif

#ifndef OS_K_ALLOWACCESS
#define OS_K_ALLOWACCESS				&OS_KernAllowAccess
#endif

#ifndef OS_K_GETAPPLICATIONSTATE
#define OS_K_GETAPPLICATIONSTATE		&OS_KernGetApplicationState
#endif

#ifndef OS_K_SHUTDOWNALLCORES
#if (OS_N_CORES == 1)
#define OS_K_SHUTDOWNALLCORES			&OS_KernUnknownSyscall
#else	/* OS_N_CORES != 1 */
#define OS_K_SHUTDOWNALLCORES			&OS_KernShutdownAllCores
#endif	/* OS_N_CORES == 1 */
#endif	/* OS_K_SHUTDOWNALLCORES */

/* CPU-family specific system calls.
*/
#ifndef OS_K_CPUFAMILY_1
#define OS_K_CPUFAMILY_1				&OS_KernUnknownSyscall
#endif

#ifndef OS_K_CPUFAMILY_2
#define OS_K_CPUFAMILY_2				&OS_KernUnknownSyscall
#endif

#ifndef OS_K_CPUFAMILY_3
#define OS_K_CPUFAMILY_3				&OS_KernUnknownSyscall
#endif

#ifndef OS_K_CPUFAMILY_4
#define OS_K_CPUFAMILY_4				&OS_KernUnknownSyscall
#endif

#include <memmap/Os_mm_const_begin.h>
const os_syscallptr_t OS_syscallTable[OS_N_SYSCALL] =
{
/* Deviation MISRAC2012-2 <START> */
	(os_syscallptr_t) &OS_KernUnknownSyscall,			/*  0 */
	(os_syscallptr_t) &OS_KernStartOs,					/*  1 */
	(os_syscallptr_t) OS_K_SUSPENDINTERRUPTS,			/*  2 */
	(os_syscallptr_t) OS_K_RESUMEINTERRUPTS,			/*  3 */
	(os_syscallptr_t) OS_K_CHAINTASK,					/*  4 */
	(os_syscallptr_t) OS_K_SCHEDULE,					/*  5 */
	(os_syscallptr_t) OS_K_GETTASKID,					/*  6 */
	(os_syscallptr_t) OS_K_GETTASKSTATE,				/*  7 */
	(os_syscallptr_t) OS_K_ACTIVATETASK,				/*  8 */
	(os_syscallptr_t) OS_K_TERMINATETASK,				/*  9 */
	(os_syscallptr_t) OS_K_GETRESOURCE,					/* 10 */
	(os_syscallptr_t) OS_K_RELEASERESOURCE,				/* 11 */
	(os_syscallptr_t) OS_K_SETEVENT,					/* 12 */
	(os_syscallptr_t) OS_K_CLEAREVENT,					/* 13 */
	(os_syscallptr_t) OS_K_GETEVENT,					/* 14 */
	(os_syscallptr_t) OS_K_WAITEVENT,					/* 15 */
	(os_syscallptr_t) OS_K_GETALARMBASE,				/* 16 */
	(os_syscallptr_t) OS_K_GETALARM,					/* 17 */
	(os_syscallptr_t) OS_K_SETRELALARM,					/* 18 */
	(os_syscallptr_t) OS_K_SETABSALARM,					/* 19 */
	(os_syscallptr_t) OS_K_CANCELALARM,					/* 20 */
	(os_syscallptr_t) OS_K_GETACTIVEAPPLICATIONMODE,	/* 21 */
	(os_syscallptr_t) OS_K_SHUTDOWNOS,					/* 22 */
	(os_syscallptr_t) OS_K_INCREMENTCOUNTER,			/* 23 */
	(os_syscallptr_t) OS_K_GETSTACKINFO,				/* 24 */
	(os_syscallptr_t) OS_K_STARTSCHEDULETABLE,			/* 25 */
	(os_syscallptr_t) OS_K_CHAINSCHEDULETABLE,			/* 26 */
	(os_syscallptr_t) OS_K_STOPSCHEDULETABLE,			/* 27 */
	(os_syscallptr_t) OS_K_SYNCSCHEDULETABLE,			/* 28 */
	(os_syscallptr_t) OS_K_SETSCHEDULETABLEASYNC,		/* 29 */
	(os_syscallptr_t) OS_K_GETSCHEDULETABLESTATUS,		/* 30 */
	(os_syscallptr_t) OS_K_TERMINATEAPPLICATION,		/* 31 */
	(os_syscallptr_t) OS_K_GETAPPLICATIONID,			/* 32 */
	(os_syscallptr_t) OS_K_GETISRID,					/* 33 */
	(os_syscallptr_t) OS_K_CALLTRUSTEDFUNCTION,			/* 34 */
	(os_syscallptr_t) OS_K_CHECKISRMEMORYACCESS,		/* 35 */
	(os_syscallptr_t) OS_K_CHECKTASKMEMORYACCESS,		/* 36 */
	(os_syscallptr_t) OS_K_CHECKOBJECTACCESS,			/* 37 */
	(os_syscallptr_t) OS_K_CHECKOBJECTOWNERSHIP,		/* 38 */
	(os_syscallptr_t) OS_K_DISABLEINTERRUPTSOURCE,		/* 39 */
	(os_syscallptr_t) OS_K_ENABLEINTERRUPTSOURCE,		/* 40 */
	(os_syscallptr_t) OS_K_GETCOUNTERVALUE,				/* 41 */
	(os_syscallptr_t) OS_K_GETELAPSEDCOUNTERVALUE,		/* 42 */
	(os_syscallptr_t) OS_K_STARTSCHEDULETABLESYNCHRON,	/* 43 */
	(os_syscallptr_t) OS_K_GETCPULOAD,					/* 44 */
	(os_syscallptr_t) OS_K_RESETPEAKCPULOAD,			/* 45 */
	(os_syscallptr_t) OS_K_TASKRETURN,					/* 46 */
	(os_syscallptr_t) OS_K_RETURNFROMCALL,				/* 47 */
	(os_syscallptr_t) OS_K_SIMTIMERADVANCE,				/* 48 */
	(os_syscallptr_t) OS_K_TRYTOGETSPINLOCK,			/* 49 */
	(os_syscallptr_t) OS_K_RELEASESPINLOCK,				/* 50 */
	(os_syscallptr_t) OS_K_ALLOWACCESS,                 /* 51 */
	(os_syscallptr_t) OS_K_GETAPPLICATIONSTATE,         /* 52 */
	(os_syscallptr_t) OS_K_SHUTDOWNALLCORES,            /* 53 */
	(os_syscallptr_t) OS_K_IOCWRITE,					/* 54 */
	(os_syscallptr_t) OS_K_IOCSEND,						/* 55 */
	(os_syscallptr_t) OS_K_EMPTYQUEUE,					/* 56 */
	(os_syscallptr_t) OS_K_IOCADDONEXT,					/* 57 */
	(os_syscallptr_t) &OS_KernUnknownSyscall,			/* 58 */
	(os_syscallptr_t) &OS_KernUnknownSyscall,			/* 59 */
	(os_syscallptr_t) OS_K_CPUFAMILY_1,					/* 60 */
	(os_syscallptr_t) OS_K_CPUFAMILY_2,					/* 61 */
	(os_syscallptr_t) OS_K_CPUFAMILY_3,					/* 62 */
	(os_syscallptr_t) OS_K_CPUFAMILY_4					/* 63 */
/* Deviation MISRAC2012-2 <STOP> */
};
#include <memmap/Os_mm_const_end.h>

#endif /* OS_KERNEL_TYPE==OS_SYSTEM_CALL */

/*!
 * OS_permittedContext
 *
 * This constant pointer points to the base of a constant array of
 * context-flag words, one per system service.
 *
 * For conformant Autosar operation, the OS_permittedContextAutosar table should be used.
 * The generator can relax the conditions by supplying a different table.
*/
#ifndef	OS_PERMITTEDCONTEXT
#define OS_PERMITTEDCONTEXT		OS_permittedContextAutosar
#endif

#include <memmap/Os_mm_const_begin.h>
const os_callermask_t * const OS_permittedContext = OS_PERMITTEDCONTEXT;
#include <memmap/Os_mm_const_end.h>

#if (OS_N_CORES > 1)
#include <memmap/Os_mm_const_begin.h>
const os_coreprop_t OS_coreProperties[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_PROPERTIES_CORE) };
#include <memmap/Os_mm_const_end.h>
#endif

#if (OS_USE_LOGICAL_CORE_IDS == 1)
#include <memmap/Os_mm_const_begin.h>
const os_coreid_t OS_alciPhyToLog[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_ALCI_PHY_TO_LOG_CORE) };
const os_coreid_t OS_alciLogToPhy[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_ALCI_LOG_TO_PHY_CORE) };
#include <memmap/Os_mm_const_end.h>
#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
