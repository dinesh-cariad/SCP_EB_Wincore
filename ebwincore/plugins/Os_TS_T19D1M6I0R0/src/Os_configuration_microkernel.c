/* Os_configuration_microkernel.c
 *
 * This file contains the architecture-independent kernel configuration data for a microkernel-based system
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_microkernel.c 30910 2019-10-09 08:34:25Z miwi261726 $
*/

#include <Os_tool.h>
#include <Os_configuration.h>
#include <Os_kernel.h>
#include <Mk_qmboard.h>
#include <Os_configuration_decls.h>
#include <public/Mk_hw_characteristics.h>
#include <Os_panic.h>

#if (OS_IOC_ENABLED==1)
#include <Ioc/Ioc_kern.h>
#endif /* OS_IOC_ENABLED */

#ifndef OS_GENERATION_ID_OS_H
#error "OS_GENERATION_ID_OS_H is not defined."
#endif

#ifndef OS_GENERATION_ID_OS_CONF_H
#error "OS_GENERATION_ID_OS_CONF_H is not defined"
#endif

#if defined(OS_GENERATION_ID_OS_H) && defined(OS_GENERATION_ID_OS_CONF_H)

#if (OS_GENERATION_ID_OS_H == OS_GENERATION_ID_OS_CONF_H)

#include <memmap/Os_mm_const32_begin.h>
const os_uint32_t OS_configurationId = OS_GENERATION_ID_OS_H;
#include <memmap/Os_mm_const32_end.h>

#else
#error "OS_GENERATION_ID_OS_H and OS_GENERATION_ID_OS_CONF_H are different"
#endif

#endif

/* The definitions in this file support the range [1,OS_N_CORES_MAX] for OS_N_CORES.
 * Any violations are reported here.
 */
#if (!defined(OS_N_CORES) || (OS_N_CORES == 0) || (OS_N_CORES > OS_N_CORES_MAX))
#error "Unsupported value for OS_N_CORES."
#endif

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

#endif

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

#define OS_CFG_ACTIVATEAUTOTASKS	OS_ActivateAutoTasks

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeTasks[OS_NSTARTMODES] = { OS_STARTMODETASKS };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_taskid_t OS_autoStartTasks[] = { OS_AUTOSTARTTASKS };
#include <memmap/Os_mm_const_end.h>

#endif

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

#define OS_CFG_ACTIVATEAUTOALARMS	OS_ActivateAutoAlarms

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeAlarms[OS_NSTARTMODES] = { OS_STARTMODEALARMS };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_autoalarm_t OS_autoStartAlarms[] = { OS_AUTOSTARTALARMS };
#include <memmap/Os_mm_const_end.h>

#endif

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

#define OS_CFG_ACTIVATEAUTOSCHEDULES	OS_ActivateAutoSchedules

#include <memmap/Os_mm_const16_begin.h>
const os_uint16_t OS_startModeSchedules[OS_NSTARTMODES] = { OS_STARTMODESCHEDULES };
#include <memmap/Os_mm_const16_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_autoschedule_t OS_autoStartSchedules[] = { OS_AUTOSTARTSCHEDULES };
#include <memmap/Os_mm_const_end.h>

#endif

/* Applications
*/

#include <memmap/Os_mm_const8_begin.h>
const os_applicationid_t OS_nApps = OS_NAPPS;
#include <memmap/Os_mm_const8_end.h>

/*!
 * OS_nHwTimers
 *
 * This constant contains the number of hardware timers that need
 * initializing.
 *
 * OS_hwTimer
 *
 * This array contains an element for each hardware timer that needs
 * initializing.
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
 *
 * OS_counterDynamic
 *
 * The dynamic state of all counters, one entry per counter.
 * The 'head' member must be set to OS_NULLALARM during StartOS().
*/

#if OS_NCOUNTERS==0

#define OS_K_GETCOUNTERVALUE		&OS_KernUnknownSyscall
#define OS_K_GETELAPSEDCOUNTERVALUE	&OS_KernUnknownSyscall

#define OS_CTRBASE					OS_NULL
#define OS_COUNTERDYNAMICPTR_SIZE	1
#define OS_COUNTERDYNAMICPTR_INIT	OS_NULL

#else

#define OS_CFG_INITCOUNTERS			&OS_InitCounters
#define OS_CTRBASE					OS_counter

#include <memmap/Os_mm_const_begin.h>
const os_counter_t OS_counter[OS_NCOUNTERS] = { OS_COUNTER };
#include <memmap/Os_mm_const_end.h>

#endif /* OS_NCOUNTERS == 0 */

#include <memmap/Os_mm_const8_begin.h>
const os_uint8_t OS_nCounters = OS_NCOUNTERS;
#include <memmap/Os_mm_const8_end.h>

#include <memmap/Os_mm_const_begin.h>
const os_counter_t * const OS_counterTableBase = OS_CTRBASE;
#include <memmap/Os_mm_const_end.h>


/* Alarm Configuration
*/
#if OS_NALARMS==0

/* No real alarms. We can set all related system calls to "Unknown system call".
 * Note that there may be some alarms in the table due to
 * configured ScheduleTables, but these are not started or
 * manipulated using the normal alarm APIs.
*/
#define OS_K_GETALARM			&OS_KernUnknownSyscall
#define OS_K_SETRELALARM		&OS_KernUnknownSyscall
#define OS_K_SETABSALARM		&OS_KernUnknownSyscall
#define OS_K_CANCELALARM		&OS_KernUnknownSyscall

#endif

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

/*
 * OS_timeStampTimer
 *
 * Configuration for timestamp (used by CPU load measurement and possibly others)
 *
 * If not configured, we choose OS_NULL. This supports older versions of the generator.
*/
#if OS_USEGENERICTIMESTAMP

#ifdef OS_TIMESTAMPTIMERINDEX

#define OS_TIMESTAMPTIMER				&OS_hwTimer[OS_TIMESTAMPTIMERINDEX]

#else

#define OS_TIMESTAMPTIMER				OS_NULL

#endif

#include <memmap/Os_mm_const_begin.h>
const os_hwt_t * const OS_timeStampTimer = OS_TIMESTAMPTIMER;
#include <memmap/Os_mm_const_end.h>

#endif

#if defined(OS_INITTIMESTAMP)
#define OS_CFG_INITTIMESTAMP		OS_INITTIMESTAMP
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
const os_scheduleentry_t OS_stEntries[OS_NSTENTRIES] = { OS_STENTRIES };
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

#define OS_CFG_STARTTICKERS		OS_StartTickers

#elif !OS_INHIBIT_GPT_STARTUP && (OS_NGPTTICKERS != 0)

#define OS_CFG_STARTTICKERS		OS_StartTickers

#endif

#endif

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
*/
#include <memmap/Os_mm_const_begin.h>
os_initfunc_t const OS_initFunc[] =
{
#ifdef OS_CFG_STARTUPCHECKS
	OS_CFG_STARTUPCHECKS,
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

#ifdef OS_CFG_ACTIVATEAUTOTASKS
	OS_CFG_ACTIVATEAUTOTASKS,
#endif

#ifdef OS_CFG_ACTIVATEAUTOALARMS
	OS_CFG_ACTIVATEAUTOALARMS,
#endif

#ifdef OS_CFG_ACTIVATEAUTOSCHEDULES
	OS_CFG_ACTIVATEAUTOSCHEDULES,
#endif

#ifdef OS_CFG_STARTTICKERS
	OS_CFG_STARTTICKERS,
#endif

	OS_NULL
};
#include <memmap/Os_mm_const_end.h>

/*!
 * OS_ptrXXXHook
 *
 * These pointers point to the hook functions if configured.
 * Otherwise, they can be OS_NULL or OS_NullFunction.
*/
#ifndef OS_STARTUPHOOK
#define OS_STARTUPHOOK	&OS_NullFunction
#endif

#include <memmap/Os_mm_const_begin.h>
const os_startuphook_t		OS_ptrStartupHook =		OS_STARTUPHOOK;
#include <memmap/Os_mm_const_end.h>


/*!
 * IncrementCounter
 *
 * Do we need the IncrementCounter/IncrementCounter system call?
 *
 * Without execution timing, if the number of counters is the same as the number of timers
 * there are no software counters, so we can omit the system call.
 * If execution timing is configured there's one hardware timer for execution timing, so we only omit
 * the system call if the number of counters is strictly less than the number of timers.
*/
#if OS_NCOUNTERS<=OS_NHWTIMERS
#define OS_K_INCREMENTCOUNTER		&OS_KernUnknownSyscall
#endif

/*!
 * OS_panicCodePtrs
 *
 * Lookup-table for panic variables.
*/
#include <memmap/Os_mm_const_begin.h>
volatile os_panic_t * const OS_panicCodePtrs[OS_N_CORES_MAX] = { OS_MAKE_COREARRAY_FROM(OS_QMOS_PANICCODE_PTR_CORE) };
#include <memmap/Os_mm_const_end.h>

/* Assertion */
#if (OS_N_CORES_MAX != MK_MAXCORES)
#error "Number of cores, which is assumed to be provided by the hardware, differs between QM-OS and microkernel."
#error "This could cause lookup-table accesses to be out of bounds."
#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
