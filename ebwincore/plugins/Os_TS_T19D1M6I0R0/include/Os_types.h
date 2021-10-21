/* Os_types.h
 *
 * This file defines the public data types for the kernel. This file is
 * included in both kernel- and user-mode compilations.
 *
 * Architecture-dependent types are included from the appropriate ARCH-types.h
 * include file depending on the chosen architecture. This file must define
 * the fixed-width data types, and can optionally declare architecture-specific
 * types for some of the standard types.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_types.h 28314 2018-02-13 15:33:57Z olme8414 $
*/

#ifndef OS_TYPES_H
#define OS_TYPES_H

#include <Os_defs.h>

#if OS_KERNEL_TYPE == OS_MICROKERNEL
#include <Os_microkernel_types.h>
#else
#include <Os_types_arch.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* OS_NULL: the null pointer constant */
#ifndef OS_NULL
#define OS_NULL  ((void *)0)
#endif

/* Native types (MISRA avoidance)
*/
#ifndef OS_ASM

#if !OS_ARCH_HAS_CHAR_T
typedef signed char		os_char_t;
typedef unsigned char	os_uchar_t;
#endif

typedef short			os_short_t;
typedef int				os_int_t;
typedef long			os_long_t;
typedef unsigned short	os_ushort_t;
typedef unsigned		os_unsigned_t;
typedef unsigned int	os_uint_t;
typedef unsigned long	os_ulong_t;

#endif	/* !OS_ASM */

/*!
 * os_reg8_t, os_reg16_t, os_reg32_t
 *
 * Memory mapped hardware registers. These are defined in terms of the
 * fixed-length unsigned types, with the addition of the 'volatile'
 * attribute because hardware registers have a habit of changing all
 * by themselves.
*/
#ifndef OS_ASM
typedef volatile os_uint8_t os_reg8_t;
typedef volatile os_uint16_t os_reg16_t;
typedef volatile os_uint32_t os_reg32_t;
#endif

/*!
 * OS_REG8(), OS_REG16(), OS_REG32()
 *
 * Macros for defining macros to read/write memory mapped hardware registers.
 * Example: #define OS_PIC_EOI      OS_REG32(OS_PIC_EOI_ADR)
*/
#ifndef OS_ASM
#define OS_REG8(Address)    (*(os_reg8_t *)(Address))
#define OS_REG16(Address)   (*(os_reg16_t *)(Address))
#define OS_REG32(Address)   (*(os_reg32_t *)(Address))
#endif

/*!
 * OS_IsPowerOf2()
 *
 * This macro returns TRUE if the non-zero unsigned parameter is a power of 2.
 * Since a parameter of zero also returns TRUE, this can be considered as testing if
 * 2**(wordlength) is a power of 2, which might be useful - for example it might be
 * necessary to test if a value is 2**n-1 for some n, and this would need to work
 * for all possible values, including 0xffffffff (on 32 bit machines).
 *
 * See http://wiki.elektrobit.com/index.php/AutoCore_OS/Kernel_Hackers'_Guide/Power-of-2_Test
*/
#define OS_IsPowerOf2(x)	(((x)&((x)-1u))==0u)

/*!
 * os_boolean_t
 *
 * This type holds OS_TRUE or OS_FALSE and can be tested directly
 * in 'if' statements.
 *
 * Programmers beware: the data type uses the natural int size of the
 * architecture for speed efficiency, so may not be the best choice
 * for use in static data structures where size is important.
*/
#ifndef OS_ASM
typedef os_int_t os_boolean_t;
#endif

#define OS_TRUE		(os_boolean_t)1
#define OS_FALSE	(os_boolean_t)0

/*!
 * os_result_t
 *
 * Return value from system calls (when there is one).
 *
 * The values OS_E_OK to OS_E_OS_VALUE are defined by OSEK/VDX.
 * !LINKSTO Kernel.API.StatusType, 1
 *
 * The values OS_E_STACKPROT to OS_E_EXCEPTPROT are defined by AUTOSAR.
 * Other values up to 255 are also defined for various other OSEK systems.
*/
#ifndef OS_ASM
typedef os_uint8_t os_result_t;
#endif

#define OS_E_OK						OS_U(0)
#define OS_E_ACCESS					OS_U(1)
#define OS_E_CALLLEVEL				OS_U(2)
#define OS_E_ID						OS_U(3)
#define OS_E_LIMIT					OS_U(4)
#define OS_E_NOFUNC					OS_U(5)
#define OS_E_RESOURCE				OS_U(6)
#define OS_E_STATE					OS_U(7)
#define OS_E_VALUE					OS_U(8)

#define OS_E_STACKPROT				OS_U(9)
#define OS_E_MEMPROT				OS_U(10)
#define OS_E_TIMEPROT				OS_U(11)
#define OS_E_LOCKPROT				OS_U(12)
#define OS_E_RATEPROT				OS_U(13)
#define OS_E_EXCEPTPROT				OS_U(14)

#define OS_E_ADDRESS				OS_U(15)
#define OS_E_INTDISABLE				OS_U(16)
#define OS_E_TASKRETURN				OS_U(17)
#define OS_E_TFID					OS_U(18)
#define OS_E_ARITH					OS_U(19)

#define OS_E_DENIED					OS_U(20)
#define OS_E_INTERNAL				OS_U(21)
#define OS_E_PANIC					OS_U(22)

#define OS_E_CORE					OS_U(23)
#define OS_E_NESTING_DEADLOCK		OS_U(24)
#define OS_E_INTERFERENCE_DEADLOCK	OS_U(25)
#define OS_E_SPINLOCK				OS_U(26)

#define OS_E_ISRRETURNRESLOCKED		OS_E_RESOURCE
#define OS_E_ISRRETURNSPINLOCKED	OS_E_SPINLOCK
#define OS_E_ISRRETURNINTLOCKED		OS_E_INTDISABLE

#define OS_E_UNSUPPORTED			OS_U(30)	/* Operation is not supported; eg. by customisation. */
#define OS_E_UNKNOWN				OS_U(31)	/* An unknown error */

/*!
 * os_taskid_t
 *
 * Task ID as handled by applications. The task ID is the index
 * of the task entry in the task table. The NULL value is
 * all 1s.
 * Up to 255 tasks.
*/
#ifndef OS_ASM
typedef os_uint8_t os_taskid_t;
#endif

/* Attention: the same macro is also used by the OS generator. Search for OS_NULLTASK in *.java files. */
#define OS_NULLTASK			OS_U(0xff)
#define OS_SIZEOF_TASKID	1

/*!
 * os_taskstate_t
 *
 * Task status.
 *
 * If the states change, you also need to change the table
 * and possibly the logic in lib-gettaskstate.
*/
#ifndef OS_ASM
typedef os_uint8_t os_taskstate_t;
#endif

#define OS_TS_SUSPENDED		OS_U(0)
#define OS_TS_QUARANTINED	OS_U(1)
#define OS_TS_NEW			OS_U(2)
#define OS_TS_READY_SYNC	OS_U(3)
#define OS_TS_READY_ASYNC	OS_U(4)
#define OS_TS_RUNNING		OS_U(5)
#define OS_TS_WAITING		OS_U(6)
#define OS_TS_NSTATES		OS_U(7)
#define OS_TS_INVALID		OS_U(255)	/* GetTaskState for invalid task */
#define OS_TS_MAX_TERMINATING	OS_U(2)	/* States <= this are terminating (see OS_LeaveKernel() and arch. equivalents) */

/*!
 * os_resourceid_t
 *
 * Resource identifier
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.Resources, 1
*/
#ifndef OS_ASM
typedef os_uint16_t os_resourceid_t;
#endif

/*!
 * os_eventmask_t
 *
 * Event mask - only if not already defined
 * If the architecture defines the event mask to be a different size
 * the architecture-specific Checker must also set MAX_EVENT_PER_TASK
 * to the correct value.
*/
#ifndef OS_ARCH_EVENTMASK_T
#ifndef OS_ASM
typedef os_uint32_t os_eventmask_t;
#endif
#define OS_SIZEOF_EVENTMASK	4
#endif

/*!
 * os_alarmid_t
 *
 * Alarm identifier. NULL is 0xffff, so we can have up to 65535 alarms.
 * The generator limits the number of real alarms to 255, but can add
 * more "internal" alarms for use by schedule tables and ISRs (rate-monitor).
*/
#ifndef OS_ASM
typedef os_uint16_t os_alarmid_t;
#endif

#define OS_NULLALARM	OS_U(0xffff)

/*!
 * os_alarmcallback_t
 *
 * Pointer to a callback function
*/
#ifndef OS_ASM
typedef void (*os_alarmcallback_t)(void);
#endif

/*!
 * os_counterid_t
 *
 * Counter identifier. Up to 255 counters, because of OS_nCounters.
*/
#ifndef OS_ASM
typedef os_uint8_t os_counterid_t;
#endif

/*!
 * os_scheduleid_t
 *
 * Schedule table identifier. Up to 255 schedule tables, because of
 * OS_nSchedules.
 *
 * Also values/bit fields in the schedule-table status returned by
 * OS_UserGetScheduleTableStatus()
*/
#ifndef OS_ASM
typedef os_uint8_t os_scheduleid_t;
#endif

#define OS_NULLSCHEDULE	OS_U(0xff)

/*!
 * os_schedulestatus_t
 *
 * Schedule table status flags, contents see below.
*/
#ifndef OS_ASM
typedef os_uint8_t os_schedulestatus_t;
#endif

/* Bits in the status field. Note: the values of STOPPED and QUARANTINED
 * are chosen so that they are equivalent if the special mask (STATE_X) is used.
*/
#define OS_ST_STATE			OS_U(0x07)	/* Mask for schedule state */
#define OS_ST_STATE_X		OS_U(0x03)	/* Mask for state in which STOPPED and QUARANTINED are the same */
#define OS_ST_STOPPED		OS_U(0x00)	/* State = STOPPED */
#define OS_ST_WAITING		OS_U(0x01)	/* State = WAITING for global time */
#define OS_ST_CHAINED		OS_U(0x02)	/* State = CHAINED */
#define OS_ST_RUNNING		OS_U(0x03)	/* State = RUNNING */
#define OS_ST_QUARANTINED	OS_U(0x04)	/* State = QUARANTINED */
#define OS_ST_SYNCHRONOUS	OS_U(0x08)	/* Running Synchronously */
#define OS_ST_ASYNCHRONOUS	OS_U(0x00)	/* Not synchronous */
#define OS_ST_SYNCDIR		OS_U(0x30)	/* Which direction is sync? */
#define OS_ST_NOSYNC		OS_U(0x00)	/* No sync (fully synchronous or set async) */
#define OS_ST_SHORTEN		OS_U(0x10)	/* Shorten period (decrease) */
#define OS_ST_LENGTHEN		OS_U(0x20)	/* Lengthen period (increase) */
#define OS_ST_RUNOUT		OS_U(0x80)	/* Runout period when chaining */

/* External status representation */
#define OS_STE_STOPPED		OS_U(1)
#define OS_STE_NEXT			OS_U(2)
#define OS_STE_WAITING		OS_U(3)
#define OS_STE_ASYNCHRONOUS	OS_U(4)
#define OS_STE_SYNCHRONOUS	OS_U(5)

/*!
 * os_physicaltime_t
 *
 * Physical time in nanoseconds etc. (if not already defined)
*/
#ifndef OS_ARCH_PHYSICALTIME_T
#ifndef OS_ASM
typedef os_uint32_t os_physicaltime_t;
#endif
#endif

/*!
 * os_timeunit_t
 *
 * Physical time unit type
*/
#ifndef OS_ASM
typedef os_uint8_t os_timetype_t;
#endif

#define OS_TIMEUNIT_NS	OS_U(1)	/* Nanoseconds */
#define OS_TIMEUNIT_US	OS_U(2)	/* Microseconds */
#define OS_TIMEUNIT_MS	OS_U(3)	/* Milliseconds */
#define OS_TIMEUNIT_S	OS_U(4)	/* Seconds */

/*!
 * os_applicationid_t
 *
 * Application identifier. Up to 255 applications, plus OS_NULLAPP
*/
#ifndef OS_ASM
typedef os_uint8_t os_applicationid_t;
#endif

#define OS_NULLAPP OS_U(0xff)
/* os_appstate_t
 *
 * Application state.
*/
#ifndef OS_ASM
typedef os_uint8_t os_appstate_t;
#endif

#define OS_APP_QUARANTINED		OS_U(0)
#define OS_APP_RESTARTING		OS_U(1)
#define OS_APP_ACCESSIBLE		OS_U(2)

/*!
 * os_functionid_t
 *
 * (Trusted) function identifier. Up to 255 functions.
*/
#ifndef OS_ASM
typedef os_uint8_t os_functionid_t;
#endif

/*!
 * os_appmodeid_t
 *
 * Application mode identifier. This can be one of the predefined modes OS_*APPMODE or DONOTCARE definded
 * below, or a custom one.
*/
#ifndef OS_ASM
typedef os_uint8_t os_appmodeid_t;
#endif

/*!
 * OS_INCONSISTENT_APPMODE
 * OS_NULLAPPMODE
 *
 * The predefined application modes.
 *
 * OS_NULLAPPMODE is used as initialization value to indicate that no mode has been set.
 * DONOTCARE is defined by AUTOSAR and has the same value (see Os_api.h). Its value
 * must be equal to OS_NULLAPPMODE.
 * The value OS_INCONSISTENT_APPMODE indicates in multi-core systems,
 * that the OS was started with inconsistent modes and hence must panic.
 *
 * See also sec. 7.9.15.1 in AUTOSAR 4.0 Rev 3.
 */
#define OS_INCONSISTENT_APPMODE		OS_U(254)
#define OS_NULLAPPMODE				OS_U(255)

/*!
 * os_serviceid_t
 *
 * Service function identifier
*/
#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#ifndef OS_ASM
typedef os_uint8_t os_serviceid_t;
#endif
#endif

/*
 * os_stacklen_t
 *
 * A data type for holding a stack length [bytes]. Most supported MCUs are fine with 32
 * bits, but the 16-bitters perform much better with only 16 bits.
 *
 * To override the default, define OS_STACKLEN_T in Os_types_ARCH.h
*/
#ifndef OS_STACKLEN_T
#define OS_STACKLEN_T			os_uint32_t
#define OS_SIZEOF_STACKLEN		OS_U(4)
#endif

#ifndef OS_ASM
typedef OS_STACKLEN_T os_stacklen_t;
#endif

/*!
 * os_stackinfo_t
 *
 * Stack information structure
*/
#ifndef OS_ASM
typedef struct os_stackinfo_s os_stackinfo_t;
typedef os_address_t os_stackinfoptr_t;

struct os_stackinfo_s
{
	os_stackinfoptr_t stackBase;
	os_stackinfoptr_t stackPointer;
	os_size_t		stackLen;
	os_size_t		stackClean;
	os_int_t		stackStatus;
	os_stackinfoptr_t isrStackBase;
	os_size_t		isrStackLen;
};
#endif

/*!
 * os_memoryaccess_t
 *
 * Return value from CheckXxxMemoryAccess. Contains bits to identify
 * memory: readable, writable, executable, stack
*/
#ifndef OS_ASM
typedef os_uint8_t os_memoryaccess_t;
#endif

#define OS_MA_READ	OS_U(0x01)
#define OS_MA_WRITE	OS_U(0x02)
#define OS_MA_EXEC	OS_U(0x04)
#define OS_MA_STACK	OS_U(0x08)

/*!
 * os_objecttype_t and os_objectid_t
 *
 * Types for describing what type of object an object is, and for holding an object identifier.
 * The general-purpose object identifier os_objectid_t is 32-bits wide, even though most individual
 * object identifiers are only 8-bit or 16-bit, like os_lockid_t or os_resourceid_t. This allows to put the individual
 * object's ID into the lower 24 bits and the object's type into the uppermost 8 bits of an os_objectid_t instance.
 * Hence, an instance of os_objectid_t is the combination of an individual object's ID with its type.
 *
 * Object types are encoded by the macros OS_OBJ_*. The special value OS_NULLOBJECT represents "no object" and
 * must be different from anything returned by OS_CreateObjectId().
*/
#ifndef OS_ASM
typedef os_uint8_t os_objecttype_t;
typedef os_uint32_t os_objectid_t;
#endif

#define OS_OBJ_NONE				OS_U(0)
#define OS_OBJ_APPLICATION		OS_U(1)
#define OS_OBJ_TASK				OS_U(2)
#define OS_OBJ_ISR				OS_U(3)
#define OS_OBJ_RESOURCE			OS_U(4)
#define OS_OBJ_COUNTER			OS_U(5)
#define OS_OBJ_ALARM			OS_U(6)
#define OS_OBJ_SCHEDULETABLE	OS_U(7)
#define OS_OBJ_TRUSTEDFUNCTION	OS_U(8)
#define OS_OBJ_SPINLOCK			OS_U(9)

#define OS_NULLOBJECT			OS_U(0xffffffff)
#define OS_SIZEOF_OBJECTID		4

/*!
 * OS_CreateObjectId
 * OS_GetIdFromObjectId
 * OS_GetTypeFromObjectId
 *
 * Macros used to create object IDs and to decompose their instances again.
 */
#define OS_CreateObjectId(id, type)			((os_objectid_t)(((os_objectid_t)(((os_objectid_t)(type)) << 24u)) | (id)))
#define OS_GetIdFromObjectId(objectid)		((os_objectid_t)((objectid) & OS_U(0xffffff)))
#define OS_GetTypeFromObjectId(objectid)	((os_objectid_t)((objectid) >> 24u))

/*!
 * os_ratemonitorid_t
 *
 * Data type for rate monitor id (== index into the table of rate monitors)
 * Needs to have enough range to allow all tasks AND all ISRs to be
 * monitored, so 8 bits isn't enough.
 *
 * OS_NULLRATEMONITOR is the value used to indicate that rate monitoring
 * is disabled for a particular object.
*/
#ifndef OS_ASM
typedef os_uint16_t os_ratemonitorid_t;
#endif

#define OS_NULLRATEMONITOR	OS_U(0xffff)
#define OS_SIZEOF_RATEMONID	2

/*!
 * os_restart_t
 *
 * This data type defines the use of a Restart Task after terminating an
 * OSApplication.
 * Constants of this type are OS_APPL_RESTART and OS_APPL_NO_RESTART.
*/
#ifndef OS_ASM
typedef os_uint8_t os_restart_t;
#endif

#define OS_APPL_NO_RESTART	OS_U(0)
#define OS_APPL_RESTART		OS_U(1)

/*!
 * os_intlocktype_t
 *
 * This data type defines an enumerated type for use in OS_KernSuspendInterrupts()
 * and OS_KernResumeInterrupts() to select what type of Suspend/Resume is needed.
*/
#ifndef OS_ASM
enum os_intlocktype_e
{
	OS_LOCKTYPE_OS = 0,				/* Suspend/ResumeOSInterrupts()	*/
	OS_LOCKTYPE_ALL = 1,			/* Suspend/ResumeAllInterrupts() */
	OS_LOCKTYPE_NONEST = 2			/* Disable/EnableAllInterrupts() */
};

typedef enum os_intlocktype_e os_intlocktype_t;

/*!
 * os_spinlocklockmethod_t
 *
 * The different lock methods a spinlock can have.
*/
enum os_spinlocklockmethod_e
{
	OS_SPINLOCKLM_LOCK_NOTHING = 0,
	OS_SPINLOCKLM_LOCK_WITH_RES_SCHEDULER = 1,
	OS_SPINLOCKLM_LOCK_CAT2_INTERRUPTS = 2,
	OS_SPINLOCKLM_LOCK_ALL_INTERRUPTS = 3
};
typedef enum os_spinlocklockmethod_e os_spinlocklockmethod_t;

/*!
 * os_coreid_t
 *
 * On multi-core systems this is the type used for the core ID wherever it is stored or used.
*/
typedef os_uint8_t os_coreid_t;

/*!
 * OS_CORE_ID_*
 *
 * These macros identify cores. The master core, i.e. the one which plays the main role in booting the system,
 * has the ID OS_CORE_ID_MASTER. To indicate invalid core IDs, the value OS_CORE_ID_INVALID is used.
 * The special value OS_CORE_ID_THIS_CORE identifies the calling core. Other core IDs are in the range
 * [0, OS_N_CORES_MAX).
 * !LINKSTO Kernel.Autosar.API.DataTypes.CoreIdType.Constants, 1
 */
#ifndef OS_CORE_ID_MASTER
#define OS_CORE_ID_MASTER		OS_U(0)
#endif
#define OS_CORE_ID_THIS_CORE	OS_U(254)
#define OS_CORE_ID_INVALID		OS_U(255)

#if OS_KERNEL_TYPE != OS_MICROKERNEL
/* Timestamp related types. */
#if OS_ARCH_HAS_64BIT

/* If the architecture has a 64-bit type, or if long long is permitted,
 * the timestamp computation is much easier.
*/
typedef os_uint64_t os_timestamp_t;

#else /* !OS_ARCH_HAS_64BIT */

/* The architecture has no 64-bit data type, so we have to synthesize one out of
 * two 32-bit variables. This is naturally a bit fiddly, but there's only a limited range
 * of operations we need.
*/
typedef struct os_timestamp_s os_timestamp_t;

struct os_timestamp_s
{
#if OS_ENDIAN == OS_BIGENDIAN
	os_uint32_t tsHi;
	os_uint32_t tsLo;
#else
	os_uint32_t tsLo;
	os_uint32_t tsHi;
#endif
};

#endif /* OS_ARCH_HAS_64BIT */
#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */
#endif /* OS_ASM */

/*!
 * os_autosarspinlockid_t
 *
 * This is the type used to identify an AUTOSAR spinlock. It does *NOT* identify an internal spinlock,
 * which is achieved with the type os_lockid_t. Hence, the type os_autosarspinlockid_t is used by
 * software external to AutoCore OS.
 */
#ifndef OS_ASM
typedef os_uint16_t os_autosarspinlockid_t;
#endif

#define OS_NULLAUTOSARSPINLOCK	OS_U(0xffff)

/*!
 * os_tryspinlock_t
 *
 * This is the type used for the "success/fail" answer from TryToGetSpinlock().
*/
#ifndef OS_ASM
typedef os_boolean_t os_tryspinlock_t;
#endif

/*!
 * OS_TRYTOGETSPINLOCK_NOSUCCESS OS_TRYTOGETSPINLOCK_SUCCESS
 *
 * These are the two possible values of os_tryspinlock_t.
*/
#define OS_TRYTOGETSPINLOCK_NOSUCCESS	0
#define OS_TRYTOGETSPINLOCK_SUCCESS		(1 == 1)

/*!
 * os_lockid_t
 *
 * Individual hardware and software locks are identified by instances of os_lockid_t.
 * The functions t OS_TakeInternalLock() and OS_DropInternalLock() are passed instances of this
 * type to select individual locks.
 */
#ifndef OS_ASM
typedef os_uint16_t os_lockid_t;
#endif

#define OS_NULLSPINLOCK			OS_U(0xffff)

/* os_coreprop_t describes the properties of a core.
 * Currently the only property is the "USED bit".
*/
#ifndef OS_ASM
typedef os_uint8_t os_coreprop_t;
#endif
#define OS_COREPROP_USED_BIT			OS_U(1)
#define OS_CorePropHasUsedBit(cprop)	(((cprop) & OS_COREPROP_USED_BIT) == OS_COREPROP_USED_BIT)

/*!
 * os_isrid_t
 *
 * Type big enough to hold an ISR Id. This variable limits the number
 * of ISRs permissible in the configuration. 16-bit allows 65535 ISRs, plus
 * OS_NULLISR.
*/
#ifndef OS_ASM
typedef os_uint16_t os_isrid_t;
#endif

/* OS_NULLISR - "invalid ISR" id.
 * A value of 0xfff limits the maximum number of ISRs to 4k which should be big
 * enough for any future derivative. Mustn't conflict with the use in
 * os_taskorisr_t, see below.
*/
#define OS_NULLISR	OS_U(0xfff)

/*!
 * os_taskorisr_t
 *
 * Variable big enough to hold either a task ID or an isr ID and be
 * capable of telling which type it is.
 *
 * Tasks run from 0 to 254. 255 means no specific task.
 * ISRs run from 256 upwards. (OS_TOI_ISR + OS_NULLISR) means no specific ISR.
*/
#ifndef OS_ASM
typedef os_uint16_t os_taskorisr_t;
#endif

#define OS_TOI_TASK				OS_U(0)
#define OS_TOI_ISR				OS_U(0x100) /* Attention: the same macro is also used by the OS generator.
 	 	 	 	 	 	 	 	 	 	 	 * Search for OS_TOI_ISR in *.java files.
 	 	 	 	 	 	 	 	 	 	 	 */
#define OS_TOI_CURRENTCONTEXT	OS_U(0xffff)

#define OS_TaskToTOI(i)			((OS_TOI_TASK)+((os_taskorisr_t) (i)))
#define OS_IsrToTOI(i)			((OS_TOI_ISR)+(i))
#define OS_IsTaskId(i)			((i)<OS_TOI_ISR)
#define OS_TOIToTask(i)			(i)
#define OS_TOIToIsr(i)			((i)-OS_TOI_ISR)
#define OS_SplitTaskOrIsr(i)	( OS_IsTaskId(i) ? OS_TOIToTask(i) : ((i)-OS_TOI_ISR) )

#ifdef __cplusplus
}
#endif

#endif /* OS_TYPES_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
