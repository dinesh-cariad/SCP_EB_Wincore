/* Os_kernel_alarm.h
 *
 * Kernel data structures - architecture-independent. For alarm and schedule table management
 *
 * This file is only eve included in Os_kernel.h, therefore it does not include
 * the header files that it needs. Including this file directly into
 * a source file will result in error messages, so DON'T DO IT!
 *
 * The reason for all this is to work around a stupid MISRA rule.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_kernel_alarm.h 27818 2017-11-08 16:44:06Z mist9353 $
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (advisory)
 * When an array with external linkage is declared, its size should be
 * explicitly specified.
 *
 * Reason:
 * Array size and initialization are depending on configuration whereas the
 * array declaration is used by configuration-independent library code.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * Macro parameter is used in a way that doesn't allow the use of parentheses,
 * e.g. for designating struct members or initializers.
 *
 * MISRAC2012-3) Deviated Rule: 8.6 (required)
 * An identifier with external linkage shall have exactly one external
 * definition.
 *
 * Reason:
 * The definition of the object depends on the configuration. Hence, it's
 * possible, that its definition is missing.
 */

#ifndef OS_KERNEL_ALARM_H
#define OS_KERNEL_ALARM_H

#include <Os_defs.h>
#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#include <Os_kernel_app.h>
#endif
#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <public/Mk_public_types.h>
#endif

/* A macro to select how the application is referenced in the schedule table, alarm and counter structures:
 *
 * In the normal OS, the reference is a pointer to an app. context, or OS_NULL.
 * Under the microkernel, the reference is an index, or 0xffffffff.
*/
#if OS_KERNEL_TYPE == OS_MICROKERNEL

#define OS_APPREF_SELECT(appId, appPtr)		(appId)

#else

#define OS_APPREF_SELECT(appId, appPtr)		(appPtr)

#endif

/*!
 * os_hwt_t
 *
 * Hardware timer initialisation. This structure defines a descriptor
 * for a hardware timer.
 */
#ifndef OS_ASM

typedef struct os_hwt_s os_hwt_t;
typedef struct os_hwtdrv_s os_hwtdrv_t;

typedef void (*os_hwtinit_t)(const os_hwt_t*);
typedef os_timervalue_t (*os_hwtread_t)(const os_hwt_t*);
typedef os_boolean_t (*os_hwtstart_t)(const os_hwt_t*, os_timervalue_t, os_tick_t);
typedef void (*os_hwtstop_t)(const os_hwt_t*);

struct os_hwtdrv_s
{
	os_hwtinit_t hwtInit;		/* Initialise the timer */
	os_hwtread_t hwtRead;		/* Read timer */
	os_hwtstart_t hwtStart;		/* Start the timer */
	os_hwtstop_t hwtStop;		/* Stop the timer */
};

#define OS_HwtInit(hwt)			(*(hwt)->driver->hwtInit)((hwt))
#define OS_HwtRead(hwt)			(*(hwt)->driver->hwtRead)((hwt))
#define OS_HwtStart(hwt, b, d)	(*(hwt)->driver->hwtStart)((hwt), (b), (d))
#define OS_HwtStop(hwt)			(*(hwt)->driver->hwtStop)((hwt))

#if OS_HWT_POWEROF2
#define OS_HwtSub(hwt,x,y)		(((x)-(y))&((hwt)->wrapMask))
#else
#define OS_HwtSub(hwt,x,y)		(((x)>=(y)) ? ((x)-(y)) : ((x)-(y)+(hwt)->wrapMask+1))
#endif

struct os_hwt_s
{
	os_hwtdrv_t const *driver;		/* Table of driver functions */
	os_timervalue_t *lastValue;		/* Last known value of this hardware timer */
	os_timervalue_t maxDelta;		/* Maximum delta allowed */
	os_timervalue_t defDelta;		/* Delta to use if max. exceeded. */
	os_timervalue_t	wrapMask;		/* Mask for clearing non-existent bits */
	os_uint8_t	major;				/* Major device no. */
	os_uint8_t	minor;				/* Minor device no. */
	os_coreid_t core;				/* Core on which the timer resides (multicore) */
#if OS_HAVE_ARCHHWT_T
	os_archhwt_t arch;				/* Architecture-dependent extensions. */
#endif
};

extern const os_uint8_t OS_nHwTimers;
extern const os_hwt_t * const OS_hwTimerTableBase;

#endif	/* OS_ASM */

/*!
 * os_counter_t and os_counterdynamic_t
 *
 * These two structures are the constant and variable parts of the
 * counter data.
 *
 * STATIC
 * app             - pointer to the counter's application
 * permissions     - which nontrusted applications can access the counter
 * hwt             - pointer to hardware timer structure. OS_NULL for s/w counter
 * maxallowedvalue - from OIL
 * ticksperbase    - from OIL
 * mincycle        - from OIL
 *
 * DYNAMIC
 * current         - current value. For h/w counter: last known value
 * error           - error value. Used while running counter queue
 * head            - first alarm in delta list. OS_NULLALARM means NONE.
 * lock            - indicates that the hw timer is locked and should not be updated
 */
#ifndef OS_ASM
typedef struct os_counter_s os_counter_t;
typedef struct os_counterdynamic_s os_counterdynamic_t;
typedef void (*os_hwctrupdatefunc_t)(const os_counter_t *);

struct os_counter_s
{
	/* Deviation MISRAC2012-2 <START> */
#if OS_KERNEL_TYPE == OS_MICROKERNEL
	OS_Cfg_app_decl(os_int32_t app)
#else
	OS_Cfg_app_decl(const os_appcontext_t *app)
#endif
	os_hwctrupdatefunc_t updFunc;
	OS_Cfg_app_decl(os_permission_t permissions)
	const os_hwt_t *hwt;
	os_tick_t maxallowedvalue;
	os_tick_t ticksperbase;
	os_tick_t mincycle;
	os_counterdynamic_t *dynamic;
	/* Deviation MISRAC2012-2 <STOP> */
};

/* Deviation MISRAC2012-2 <START> */
#define OS_COUNTER_INIT(appId, appPtr, updFunc, permissions, hwt, maxallowedvalue, ticksperbase, mincycle, dyn ) \
	{																		\
		OS_Cfg_app_init(OS_APPREF_SELECT((appId), (appPtr)))				\
		updFunc,															\
		OS_Cfg_app_init(permissions)										\
		hwt,																\
		maxallowedvalue,													\
		ticksperbase,														\
		mincycle,															\
		dyn																	\
	}
/* Deviation MISRAC2012-2 <STOP> */

struct os_counterdynamic_s
{
	os_tick_t		current;
	os_tick_t		error;
	os_alarmid_t	head;
	os_uint8_t		lock;
};

/* This macro calls the update function for a counter. Optimisable if the
 * timestamp never hangs off a hardware counter, or if the only hardware
 * counter is also the timestamp.
 */
#ifndef OS_CtrUpdate
#define OS_CtrUpdate(cs)		((*((cs)->updFunc))(cs))
#endif

/* This macro determines whether the delta (d) is within the range of the hardware timer.
 * If it is not, one or more additional interval interrupts is inserted.
 * Since the full range of the timer is not possible (missed or delayed interrupts cause
 * severe timer drift), a max delta and a default delta are used. The max delta is typically
 * 75% of the full timer range. If this is exceeded, the default delta (typically 50%) is used
 * instead. This means that the next interval will be at least 25% of the timer range.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.HARDWARE.ExtendedValue, 1
 */
#define OS_CtrStart(hwt, b, d) \
	do {												\
		os_tick_t OS_hwtStartDelta = (d);				\
		if ( OS_hwtStartDelta > (hwt)->maxDelta )		\
		{												\
			OS_hwtStartDelta = (hwt)->defDelta;			\
		}												\
		OS_HwtStart((hwt), (b), (OS_hwtStartDelta));	\
	} while (0)

extern const os_counterid_t OS_nCounters;
extern const os_counter_t * const OS_counterTableBase;

#ifndef OS_CounterIsHw
#define OS_CounterIsHw(cs)	((cs)->hwt != OS_NULL)
#endif

#define OS_CounterIsSw(cs)	(!OS_CounterIsHw(cs))

#endif /* OS_ASM */

/*!
 * OS_CounterAdd()
 * Adds an increment (inc) onto the current counter value (cur) and wraps the result around the
 * max-allowed-value (mav).
 * The naiive solution of adding the current and increment together and then testing the result
 * against mav could fail if mav is large relative to the maximum value representable in the
 * variable size. Eg in a 16-bit system, the largest number is 65535. If mav is (say) 49999,
 * both cur and inc could legally be 40000. Adding them together would result in an overflow, and
 * the result (mod 65536) would be less than mav and thus would not trigger the wrap-around.
 *
 * The solution is to work out beforehand if the sum would wrap around mav by computing the
 * distance from cur to mav (mav - cur). If the distance is less than the increment, we need to
 * wrap the result by subtracting (mav+1). Otherwise, we can simply add the increment on to the
 * current value.
 *
 * Any overflows/underflows that occur in the wrapping operation can safely be ignored.
 *
 * OS_CounterSub() does subtraction in a similar way.
 *
 * These macros evaluate their parameters several times each, so please ensure that they
 * are simple variables or constant expressions.
 */
#define OS_CounterAdd(cur, inc, mav)	\
	( (((mav) - (cur)) < (inc)) ? ((((cur) + (inc)) - (mav)) - 1u) : ((cur) + (inc)))

#define OS_CounterSub(cur, dec, mav)	\
	( ((cur) < (dec)) ? ((((cur) - (dec)) + (mav)) + 1u) : ((cur) - (dec)))

/*!
 * OS_TimerSub()
 * This macro shall be used in timer drivers for calculation of timer
 * differences i.g current timer (in ticks) minus old time.
 *
 * The mask parameter is used to correct the overflow range.
 * If the mask is powerof2 this can be done by simple AND masking.
 * If mask is not powerof2 the already existing counter sub macro is used.
 */
#define OS_TimerSub(cur,old,mask) \
	( OS_IsPowerOf2((mask)+1u) ? (((cur)-(old))&(mask)) : OS_CounterSub((cur),(old),(mask)) )

/* OS_TickSwCounter() advances a software counter by 1.
 * This macro is used in generated ISRs for the internal and gpt ticker interrupts
 */
#define OS_TickSwCounter(c) \
	do {																			\
		os_intstatus_t OS_tscIs = OS_IntDisable();									\
		(void) OS_AdvanceCounter(&OS_counterTableBase[(c)], OS_counterTableBase[(c)].dynamic, 1, OS_tscIs);	\
		OS_IntRestore(OS_tscIs);													\
	} while (0)

/*!
 * os_alarm_t and os_alarmdynamic_t
 *
 * These two structures are the constant and variable parts of the
 * alarm data.
 *
 * STATIC
 * app         - pointer to application that owns the alarm
 * wrapper     - pointer to the wrapper function for the alarm action
 * p           - parameters for the wrapper function. Type depends on action
 * permissions - nontrusted applications that can access this alarm
 * counter     - id of counter on which this alarm hangs.
 *
 * DYNAMIC
 * delta       - time until expiry. If first in counter list, time from
 *               now, or last known timer vale. If not 1st, time after
 *               previous alarm in list.
 * period      - period for cyclic alarm
 * next        - id of next alarm in delta list
 * inUse       - current status
 */
#ifndef OS_ASM
typedef struct os_alarm_s os_alarm_t;
typedef struct os_alarmdynamic_s os_alarmdynamic_t;

typedef void (*os_alarmwrapper_t)(const os_alarm_t *);

struct os_alarm_s
{
	/* Deviation MISRAC2012-2 <START> */
#if OS_KERNEL_TYPE == OS_MICROKERNEL
	OS_Cfg_app_decl(os_int32_t app)
#else
	OS_Cfg_app_decl(const os_appcontext_t *app)
#endif
	OS_Cfg_app_decl(os_permission_t permissions)
	os_alarmwrapper_t wrapper;
	os_eventmask_t event;		/* Event, if applicable */
	os_objectid_t object;		/* ID of Task/Counter/ST/callback */
	os_counterid_t counter;		/* to which the alarm is attached */
	os_alarmdynamic_t *dynamic; /* Points to the dynamic part of an alarm. */
	/* Deviation MISRAC2012-2 <STOP> */
};

/* Deviation MISRAC2012-2 <START> */
#define OS_ALARM_INIT(appId, appPtr, permissions, wrapper, event, object, counter, dyn) \
	{																		\
		OS_Cfg_app_init(OS_APPREF_SELECT((appId), (appPtr)))				\
		OS_Cfg_app_init(permissions)										\
		wrapper,															\
		event,																\
		object,																\
		counter,															\
		dyn																	\
	}
/* Deviation MISRAC2012-2 <STOP> */

struct os_alarmdynamic_s
{
	os_tick_t delta;
	os_tick_t period;
	os_alarmid_t next;
	os_uint8_t inUse;
};

extern const os_alarmid_t OS_nAlarms;
extern const os_alarmid_t OS_totalAlarms;
extern const os_alarm_t * const OS_alarmTableBase;
/* Deviation MISRAC2012-1, MISRAC2012-3 */
extern const os_alarmcallback_t OS_alarmCallback[];

/* CHECK: SAVE
 * CHECK: RULE 306 OFF
 */
typedef os_result_t (*os_killalarmfunc_t)(os_alarmid_t, os_uint8_t);
/* CHECK: RESTORE
 */
extern const os_killalarmfunc_t OS_killAlarmFunc;

void OS_WrapActivateTask(const os_alarm_t *);
void OS_WrapSetEvent(const os_alarm_t *);
void OS_WrapAlarmCallback(const os_alarm_t *);
void OS_WrapIncrementCounter(const os_alarm_t *);
void OS_WrapRunSchedule(const os_alarm_t *);
#endif

#define OS_ALARM_IDLE			OS_U(0)
#define OS_ALARM_INUSE			OS_U(1)
#define OS_ALARM_QUARANTINED	OS_U(2)

#define OS_AlarmIdToDynamic(a)	\
	( ((a) == OS_NULLALARM) ? OS_NULL : OS_alarmTableBase[(a)].dynamic )

/*!
 * os_schedule_t and os_scheduledynamic_t
 *
 * These two structures represent the constant and variable parts of a
 * Schedule Table to the kernel.
 *
 * STATIC
 * app          - owner application
 * permissions  - which nontrusted applications can access this ST.
 * table        - pointer to first action point in this ST
 * period       - period of ST. 0 means non-periodic (single-shot)
 * maxIncrease  - maximum increase per round when synchronising
 * maxDecrease  - maximum decrease per round when synchronising
 * dirCutoff    - break value to determine whether to shorten or lengthen
 *                the period when synchronising. This value is computed by
 *                the generator to avoid complicated multi-precision
 *                arithmetic in real-time.
 * tolerance    - if ST is within this amount of global-time, it is SYNCRONOUS
 *                (PRECISION oil attribute)
 * alarm        - index of the alarm that this ST uses
 * nEntries     - no. of action points in the ST
 * flags        - various flags: REPEATING, SYNCABLE, etc.
 *
 * DYNAMIC
 * adjRemaining - remaining adjustment (for synchronisation)
 * chain        - id of chained ST (or NULLSCHEDULE if none)
 * next         - index of next action-point.
 * status       - various status bits and bit-fields: STATE, SYNCHRONOUS etc.
 *
 * Schedule Entry (action-point)
 * offset       - time (counting from start-of-round)
 * event        - event to set for task (if eventType is SETEVENT)
 * task         - task to activate/set event for.
 * eventType    - ACTIVATETASK or SETEVENT
 * OS_stEntries[] is an array of these, ordered by ST and by offset
 * within the ST blocks. The ST's static part has a pointer to the ST's
 * first entry.
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Counter, 1
 *   A schedule table is driven by a single alarm that can only be attached to a single counter.
 */
#ifndef OS_ASM
typedef struct os_schedule_s os_schedule_t;
typedef struct os_scheduledynamic_s os_scheduledynamic_t;
typedef struct os_scheduleentry_s os_scheduleentry_t;

struct os_schedule_s
{
	/* Deviation MISRAC2012-2 <START> */
#if OS_KERNEL_TYPE == OS_MICROKERNEL
	OS_Cfg_app_decl(os_int32_t app)
#else
	OS_Cfg_app_decl(const os_appcontext_t *app)
#endif
	OS_Cfg_app_decl(os_permission_t permissions)
	const os_scheduleentry_t *table;
	os_tick_t period;			/* Length of round */
	os_tick_t dirCutoff;		/* Limit for deciding sync direction when ST is asynchronous */
	os_tick_t tolerance;		/* Tolerance for ST to be synchronous */
	os_uint16_t nEntries;
	os_alarmid_t alarm;
	os_uint8_t flags;
	os_scheduledynamic_t *dynamic;	/* Points to the dynamic part of a schedule table. */
	/* Deviation MISRAC2012-2 <STOP> */
};

/* Deviation MISRAC2012-2 <START> */
#define OS_STCONFIG_INIT(appId, appPtr, permissions, table, period, dirCutoff, tolerance, alarm, nEntries, flags, dyn) \
	{																				\
		OS_Cfg_app_init(OS_APPREF_SELECT((appId), (appPtr)))						\
		OS_Cfg_app_init(permissions)												\
		table,																		\
		period,																		\
		dirCutoff,																	\
		tolerance,																	\
		nEntries,																	\
		alarm,																		\
		flags,																		\
		dyn																			\
	}
/* Deviation MISRAC2012-2 <STOP> */

struct os_scheduledynamic_s
{
	os_tick_t ctrAbs;			/* Absolute counter value for current expiry point. */
	os_tick_t adjRemaining;		/* Synchronisation adjustment remaining. */
	os_uint16_t next;			/* Index of next event */
	os_scheduleid_t chain;		/* Id of chained schedule table. */
	os_schedulestatus_t status;	/* Status flags */
};

struct os_scheduleentry_s
{
	os_tick_t offset;		/* Offset in ticks from start of schedule table round */
	os_tick_t maxIncrease;	/* Max increase of the delay to this event */
	os_tick_t maxDecrease;	/* Max decrease of the delay to this event */
	os_eventmask_t event;	/* Event(s) to set for task. 0 ==> ActivateTask */
	os_taskid_t task;
};

extern const os_scheduleid_t OS_nSchedules;
extern const os_schedule_t * const OS_scheduleTableBase;

/* CHECK: SAVE
 * CHECK: RULE 306 OFF
 */
typedef os_result_t (*os_killschedulefunc_t)(os_scheduleid_t, os_schedulestatus_t);
typedef os_tick_t (*os_synchronizefunc_t)(os_scheduledynamic_t *, const os_scheduleentry_t *, os_tick_t);
/* CHECK: RESTORE
 */

extern const os_killschedulefunc_t OS_killScheduleFunc;
extern const os_synchronizefunc_t OS_synchronizeFunc;

#endif /* OS_ASM */

/* Values for eventType
 */
#define OS_ST_ACTIVATETASK	OS_U(0x01)
#define OS_ST_SETEVENT		OS_U(0x02)

/* Bit fields in the constant flags word
 *
 * !LINKSTO Kernel.Autosar.ScheduleTable.Synchronisation.Strategy, 1
 */
#define OS_ST_REPEATING		OS_U(0x01)	/* Schedule table repeats (is "periodic") */
#define OS_ST_SYNCABLE		OS_U(0x02)	/* Schedule table can be synchronised (i.e. EXPLICITLY synchronised) */
#define OS_ST_IMPLICIT		OS_U(0x04)	/* Schedule table is implicitily synchronous */
#define OS_ST_SYNCINC		OS_U(0x40)	/* Syncronising increase available per round is non-zero */
#define OS_ST_SYNCDEC		OS_U(0x80)	/* Syncronising decrease available per round is non-zero */

/* Bit fields in the dynamic status word - see Os_types.h
 */

/*!
 * OS_nStartModes,
 * OS_startModeTasks, OS_autoStartTasks
 * OS_startModeAlarms, OS_autoStartAlarms
 * OS_startModeSchedules, OS_autoStartSchedules
 *
 * These constants define what gets started automatically by StartOS.
 * - OS_nStartModes contains the number of start modes (for error check)
 * - OS_startModeTasks is an array of indexes into OS_autoStartTasks[]
 * - OS_autoStartTasks contains several sub-lists of task IDs, each list
 *   terminated by OS_NULLTASK. A list specifies which tasks to
 *   start in a particular start mode.
 * - OS_startModeAlarms is an array of indexes into OS_autoStartAlarms[]
 * - OS_autoStartAlarms contains several sub-lists of os_autoalarm_t
 *   records, each of which specifies an alarm to start, and the interval
 *   and cycle time. The parameters are passed to (the equivalent of)
 *   SetRelAlarm() or SetAbsAlarm(), depending on the method.
 * - OS_startModeSchedules is an array of indexes into OS_autoStartSchedules[]
 * - OS_autoStartSchedules contains several sub-lists of os_autoschedule_t
 *   records, each of which specifies an schedule table to start, and the interval
 *   and cycle time. The parameters are passed to (the equivalent of)
 *   StartScheduleTableRel(), StartScheduleTableAbs() or StartScheduleTableSynchron(),
 *   depending on the method.
 */
#ifndef OS_ASM
extern const os_appmodeid_t OS_nStartModes;
/* Deviation MISRAC2012-1 <2> */
extern const os_uint16_t OS_startModeTasks[];
extern const os_taskid_t OS_autoStartTasks[];

typedef struct os_autoalarm_s os_autoalarm_t;

struct os_autoalarm_s
{
	os_alarmid_t alarm;
	os_uint8_t method;
	os_tick_t interval;
	os_tick_t cycle;
};

/* Deviation MISRAC2012-1 <2> */
extern const os_uint16_t OS_startModeAlarms[];
extern const os_autoalarm_t OS_autoStartAlarms[];

typedef struct os_autoschedule_s os_autoschedule_t;

struct os_autoschedule_s
{
	os_scheduleid_t schedule;
	os_uint8_t method;
	os_tick_t offset;
};

/* Deviation MISRAC2012-1, MISRAC2012-3 <2> */
extern const os_uint16_t OS_startModeSchedules[];
extern const os_autoschedule_t OS_autoStartSchedules[];

/* Possible values for the "method" fields for alarms and schedule tables.
 */
#define OS_STARTMETHOD_ABS	OS_U(1)
#define OS_STARTMETHOD_REL	OS_U(2)
#define OS_STARTMETHOD_SYNC	OS_U(3)		/* Schedule tables only */

#endif /* !OS_ASM */

/*!
 * Kernel startup functions.
 *
 * The initialisation functions are called by OS_KernStartOs() via an array of
 * constant pointers. The array will only be as big as it needs to be, and will be
 * null-terminated (see Os_configuration.c). Functions that are not needed are not
 * present in the array, thus saving code space.
 */
#ifndef OS_ASM
typedef void (*os_initfunc_t)(void);
/* Deviation MISRAC2012-1 */
extern os_initfunc_t const OS_initFunc[];
#endif

/*!
 * Kernel startup check functions.
 *
 * The startup-check functions are called by OS_StartupChecks() via an array of
 * constant pointers. The array will only be as big as it needs to be, and will be
 * null-terminated (see Os_configuration.c). Functions that are not needed are not
 * present in the array, thus saving code space.
 */
#ifndef OS_ASM
typedef os_result_t (*os_schkfunc_t)(void);
/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern os_schkfunc_t const OS_startupCheckFunc[];
#endif

/*!
 * OS_configMode
 *
 * These variables hold various aspects of the kernel's configuration.
 * Not all of them are much use.
 *
 * OS_configMode
 * Bit		Mask (OS_*)				Meaning
 * ========================================
 * 0-1		CC						OSEK conformance class
 * 2-3		SCHEDULE				ProOSEK SCHEDULE (NON/MIXED/FULL)
 * 4		CPULOAD					Perform CPU load measurement
 * 5		EXTRACHECK				enables a range of extra checks
 * 6		STACKCHECK				enables stack-checking
 * 7		EXTENDED status			OSEK extended status
 * 8-9		DBGPROT					FULL/OFF controls use of memory protection unit (for debugging)
 * 10		VOIDERROR				Enables error handling for APIs that don't return StatusType
 * 11		CALLISRDIRECT			ISRs are called directly, are trusted and not killable
 * 12		CALLAPPSHOOKDIRECT		App. startup/shutdown hooks are called directly, are trusted and not killable
 * 13		CALLAPPEHOOKDIRECT		App. error hooks are called directly, are trusted and not killable
 * 14		SERVICETRACE			???
 * 15		TRACE					???
 * 16		STARTUPCHECK			enables a range of extra checks at startup
 */
#define OS_CC					OS_U(0x0003)
#define OS_BCC1					OS_U(0x0000)
#define OS_BCC2					OS_U(0x0001)
#define OS_ECC1					OS_U(0x0002)
#define OS_ECC2					OS_U(0x0003)

#define OS_CC2					OS_U(0x0001)	/* Can be used with simple AND to test for xCC1 */
#define OS_ECC					OS_U(0x0002)	/* Can be used with simple AND to test for ECCn */

#define OS_SCHEDULE				OS_U(0x000c)
#define OS_SCHEDNON				OS_U(0x0000)
#define OS_SCHEDMIX				OS_U(0x0004)
#define OS_SCHEDFULL			OS_U(0x0008)

#define OS_CPULOAD				OS_U(0x0010)

#define OS_EXTRACHECK			OS_U(0x0020)

#define OS_STACKCHECK			OS_U(0x0040)

#define OS_STANDARD				OS_U(0x0000)
#define OS_EXTENDED				OS_U(0x0080)

#define OS_DBGPROT				OS_U(0x0200)
#define OS_DBGPROT_FULL			OS_U(0x0000)
#define OS_DBGPROT_OFF			OS_U(0x0200)	/* OS_DBGPROT_PART was 0x0100 (ASCOS-4046) */

#define OS_VOIDERROR			OS_U(0x0400)

#define OS_CALLISRDIRECT		OS_U(0x0800)
#define OS_CALLAPPSHOOKDIRECT	OS_U(0x1000)
#define OS_CALLAPPEHOOKDIRECT	OS_U(0x2000)

#define OS_SERVICETRACE			OS_U(0x4000)
#define OS_TRACE				OS_U(0x8000)

#define OS_STARTUPCHECK			OS_U(0x10000)

#ifndef OS_ASM
extern const os_uint32_t OS_configMode;
#endif

/*!
 * OS_ExtraCheck()
 *
 * This macro can be used to turn extra checks on and off. This is the
 * generic library version
 *
 */
#ifndef OS_ExtraCheck
#define OS_ExtraCheck(x)	(((OS_configMode & OS_EXTRACHECK) != 0) && (x))
#endif

/*!
 * OS_IsValidAlarmId()			- Checks to see if Alarm id valid
 * OS_IsValidCounterId()		- Checks to see if Counter id valid
 * OS_IsValidScheduleId()		- Checks to see if Schedule id valid
 *
 * These macros are defined as a constant (disable the checking) in STANDARD mode.
 * Otherwise they compare the parameter against its valid range.
*/
#define OS_IsValidAlarmId(a)		OS_DoExtendedStatusTrue((a) < OS_nAlarms)
#define OS_IsValidCounterId(c)		OS_DoExtendedStatusTrue((c) < OS_nCounters)
#define OS_IsValidScheduleId(s)		OS_DoExtendedStatusTrue((s) < OS_nSchedules)

/* Parameter access (non-optimised) for all services that can pass parameters to the error handler
 */
#if (!defined(OS_PARAMETERACCESS_DECL)) && (!defined(OS_SAVE_PARAMETER_N)) && (!defined(OS_GET_PARAMETER_VAR))
#define OS_PARAMETERACCESS_DECL		os_paramtype_t p[OS_MAXPARAM];
#define OS_SAVE_PARAMETER_N(n,x)	p[(n)] = (x)
#define OS_GET_PARAMETER_VAR()		p
#endif

/* Parameter access (non-optimised) for all services that can pass parameters to the protection handler
 */
#if (!defined(OS_PH_PARAMETERACCESS_DECL)) && (!defined(OS_PH_SAVE_PARAMETER_N)) && (!defined(OS_PH_GET_PARAMETER_VAR))
#define OS_PH_PARAMETERACCESS_DECL	os_paramtype_t p[OS_MAXPARAM];
#define OS_PH_SAVE_PARAMETER_N(n,x)	p[(n)] = (x)
#define OS_PH_GET_PARAMETER_VAR()	p
#endif

/*
 * Function prototypes etc.
 */
#ifndef OS_ASM

/* Kernel startup functions.
 */
extern void OS_NullFunction(void);
extern void OS_InitTimers(void);
extern void OS_InitCounters(void);
extern void OS_InitAlarms(void);
extern void OS_InitSchedules(void);
extern void OS_StartTimers(void);
extern void OS_ActivateAutoTasks(void);
extern void OS_ActivateAutoAlarms(void);
extern void OS_ActivateAutoSchedules(void);

/* Internal kernel functions
 */
extern void OS_EnqueueAlarm(const os_counter_t *, os_counterdynamic_t *, os_alarmid_t , os_alarmdynamic_t *);
extern void OS_EnqueueAlarmNoAdjust(os_counterdynamic_t *, os_alarmid_t , os_alarmdynamic_t *);
extern os_errorresult_t OS_LocalGetAlarm(os_alarmid_t, const os_alarm_t *, os_tick_t *);
extern os_errorresult_t OS_LocalSetAlarm(
	os_alarmid_t, const os_alarm_t *, const os_counter_t *,
	os_tick_t , os_tick_t , os_boolean_t);
extern os_errorresult_t OS_LocalCancelAlarm(os_alarmid_t, const os_alarm_t *);
extern os_tick_t OS_AdvanceCounter(const os_counter_t *, os_counterdynamic_t *, os_tick_t, os_intstatus_t);
extern os_tick_t OS_GetCount(const os_counter_t *);
extern os_result_t OS_KillAlarm(os_alarmid_t, os_uint8_t);
extern os_result_t OS_KillSchedule(os_scheduleid_t, os_schedulestatus_t);
extern void OS_HwCounterUpdate(const os_counter_t *);
extern void OS_HwCounterTimestampUpdate(const os_counter_t *);
extern void OS_TimestampUpdate(void);
extern void OS_RunSchedule(os_scheduleid_t);
extern void OS_SetScheduleAlarm(const os_schedule_t *, os_scheduledynamic_t *, os_tick_t, os_boolean_t);
extern os_boolean_t OS_ResetScheduleAlarm(const os_schedule_t *, os_scheduledynamic_t *, os_tick_t);
extern os_int_t OS_GetAlarmDelta(os_alarmid_t, const os_alarm_t *, os_alarmdynamic_t *, os_tick_t *);

extern os_tick_t OS_Synchronize(os_scheduledynamic_t *, const os_scheduleentry_t *, os_tick_t);

extern os_errorresult_t OS_LocalStartScheduleTable(const os_schedule_t *, os_counterid_t, os_tick_t, os_boolean_t);
extern os_errorresult_t OS_LocalStartScheduleTableSynchron(const os_schedule_t *);
extern os_errorresult_t OS_LocalStopScheduleTable(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_LocalSyncScheduleTable(const os_schedule_t *, os_tick_t);
extern os_errorresult_t OS_LocalChainScheduleTable(os_scheduleid_t, os_scheduleid_t);
extern os_errorresult_t OS_LocalSetScheduleTableAsync(os_scheduleid_t);

extern void OS_StartTickers(void);

#if ((OS_N_CORES == 1) || (OS_KERNEL_TYPE == OS_MICROKERNEL))
#define OS_DoGetAlarm                           OS_LocalGetAlarm
#define OS_DoCancelAlarm                        OS_LocalCancelAlarm
#define OS_DoSetAlarm                           OS_LocalSetAlarm
#define OS_DoStartScheduleTable(s,ps,c,o,r)     OS_LocalStartScheduleTable((ps),(c),(o),(r))
#define OS_DoStartScheduleTableSynchron(s,ps)   OS_LocalStartScheduleTableSynchron(ps)
#define OS_DoStopScheduleTable                  OS_LocalStopScheduleTable
#define OS_DoSyncScheduleTable(s,ps,t)          OS_LocalSyncScheduleTable((ps),(t))
#define OS_DoChainScheduleTable(sc,sn,scs)      OS_LocalChainScheduleTable((sc),(sn))
#define OS_DoSetScheduleTableAsync(s,ps)        OS_LocalSetScheduleTableAsync(s)
#else /* OS_N_CORES, OS_KERNEL_TYPE */
#define OS_DoGetAlarm                           OS_MultiGetAlarm
#define OS_DoCancelAlarm                        OS_MultiCancelAlarm
#define OS_DoSetAlarm                           OS_MultiSetAlarm
#define OS_DoStartScheduleTable(s,ps,c,o,r)     OS_MultiStartScheduleTable((s),(ps),(c),(o),(r))
#define OS_DoStartScheduleTableSynchron(s,ps)   OS_MultiStartScheduleTableSynchron((s),(ps))
#define OS_DoStopScheduleTable                  OS_MultiStopScheduleTable
#define OS_DoSyncScheduleTable(s,ps,t)          OS_MultiSyncScheduleTable((s),(ps),(t))
#define OS_DoChainScheduleTable(sc,sn,scs)      OS_MultiChainScheduleTable((sc),(sn),(scs))
#define OS_DoSetScheduleTableAsync(s,ps)        OS_MultiSetScheduleTableAsync((s),(ps))
extern os_errorresult_t OS_MultiGetAlarm(os_alarmid_t, const os_alarm_t *, os_tick_t *);
extern os_errorresult_t OS_MultiSetAlarm(
	os_alarmid_t, const os_alarm_t *, const os_counter_t *,
	os_tick_t , os_tick_t , os_boolean_t);
extern os_errorresult_t OS_MultiCancelAlarm(os_alarmid_t, const os_alarm_t *);
extern os_errorresult_t OS_MultiStartScheduleTable(
	os_scheduleid_t, const os_schedule_t *, os_counterid_t,
	os_tick_t, os_boolean_t);
extern os_errorresult_t OS_MultiStartScheduleTableSynchron(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_MultiStopScheduleTable(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_MultiSyncScheduleTable(os_scheduleid_t, const os_schedule_t *, os_tick_t);
extern os_errorresult_t OS_MultiChainScheduleTable(os_scheduleid_t, os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_MultiSetScheduleTableAsync(os_scheduleid_t, const os_schedule_t *);
#endif /* OS_N_CORES, OS_KERNEL_TYPE */

#if OS_KERNEL_TYPE == OS_MICROKERNEL

/* Microkernel wrapper functions
 * These functions are used to "wrap" kernel functions that return information via referenced
 * variables. In a generalised memory protection system the OS has no write access, so values need to
 * be returned through registers and placed correctly by a library function running in the context of
 * the calling thread.
*/
extern mk_parametertype_t OS_MkwGetCounterValue(mk_parametertype_t, mk_parametertype_t, mk_parametertype_t);
extern mk_parametertype_t OS_MkwGetAlarm(mk_parametertype_t, mk_parametertype_t, mk_parametertype_t);

#endif

/* Startup check functions */
extern void OS_StartupChecks(void);
extern os_result_t OS_StartupChecksAssemblerOffsets(void);
extern os_result_t OS_StartupChecksApplication(void);
extern os_result_t OS_StartupChecksTrustedFunction(void);
extern os_result_t OS_StartupChecksTask(void);
extern os_result_t OS_StartupChecksQueue(void);
extern os_result_t OS_StartupChecksCounter(void);
extern os_result_t OS_StartupChecksAlarm(void);
extern os_result_t OS_StartupChecksTimer(void);
extern os_result_t OS_StartupChecksSchedule(void);
extern os_result_t OS_StartupChecksResource(void);
extern os_result_t OS_StartupChecksIsr(void);

#endif /* OS_ASM */

#endif /* OS_KERNEL_ALARM_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
