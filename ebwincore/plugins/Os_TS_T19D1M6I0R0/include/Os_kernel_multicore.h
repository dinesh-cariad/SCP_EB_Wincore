/* Os_kernel_multicore.h
 *
 * This file contains anything related to multicore features as used inside the kernel.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_kernel_multicore.h 30633 2019-08-12 14:02:48Z ache271795 $
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
 *
 * MISRAC2012-2) Deviated Rule: 2.4 (advisory)
 * A project should not contain unused tag declarations.
 *
 * Reason:
 * Required by coding guidelines.
 *
 * MISRAC2012-3) Deviated Rule: 8.6 (required)
 * An identifier with external linkage shall have exactly one external definition.
 *
 * Reason:
 * False positive: OS_kernelData is defined in Os_configuration_c0.c
 */


#ifndef OS_KERNEL_DATA_H_
#define OS_KERNEL_DATA_H_

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#include <Os_kernel_task.h>
#include <Os_cpuload_kernel.h>
#include <Os_messagequeue.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>
#include <Os_tool.h>
#include <Os_alci.h>
#endif

#if (OS_N_CORES != 1) && (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <public/Mk_public_api.h>
#endif

#include <Os_timestamp.h>

/*!
 * OS_kerneldata
 *
 * This structure holds all global variables used by an OS kernel.
 *
 * In a multicore system with shared memory, multiple OS instances will be placed in the same memory block.
 * So that, each OS instance must have its separate data storage to avoid conflicts.
 *
 * Thus, all kernel data of an OS instance will be placed in this structure.
 * For N multiple OS instances, an array of the pointer of type OS_kerneldata_t with N slots will be allocated.
 * Each OS instance must select its own slot by using its core id.
 *
 * The functions of each member is explained here:
 *
 * taskQueueHead
 *     Pointer to first task in the task queue, for convenience.
 *
 * taskCurrent
 *     This is a pointer to the currently executing task. When
 *     the task is running, this will be the same as the task at the head
 *     of the queue (see taskQueueHead). When leaving the kernel,
 *     the scheduler will be called if this is not the case. At this point,
 *     OS_NULL means that the most recently executing task was terminated.
 *
 * isrCurrent
 *     This is the id of the currently-executing ISR (or OS_NULLISR
 *     if executing at task level)
 *
 * inFunction
 *     This variable indicates whether we are currently running in an ISR,
 *     hook function or task. The values it takes are:
 *     OS_INTASK, OS_INCAT1, OS_INCAT2, OS_INxxxHOOK ... and others
 *     The "startup" level is defined to be 0.
 *
 * nestSuspendOs
 * oldSuspendOs
 * nestSuspendAll
 * oldSuspendAll
 *     These variables are the nesting counters and old-level stores for
 *     Suspend/ResumeOSInterrupts and Suspend/ResumeAllInterrupts when called
 *     from a non-task context. When the interrupt management functions are
 *     called from a task context, per-task variables in the task dynamic
 *     structure are used. The XXXAll variables are also used by
 *     Disable/EnableAllInterrupts.
 *
 * isrLastLock
 *     This variable is the head of a chain of locks taken at interrupt level.
 *     It may either identify a hardware/software spinlock or a resource. This
 *     means the object type is either OS_OBJ_SPINLOCK or OS_OBJ_RESOURCE.
 *     The head of the chain is saved and set to OS_NULLOBJECT
 *     by the interrupt entry code. On exit it must still be OS_NULLOBJECT,
 *     otherwise the ISR has not released all resources that it takes. After
 *     cleanup, the original value is restored.
 *
 * accounting
 *     This data structure holds the current task/ISR accounting information.
 *     When a task gets preempted, the etbRemaining field is stored into
 *     the task's context. During an ISR, the etbRemaining field is stored on
 *     the stack.
 *
 * errorHookNesting
 *     This variable contains 3 bits, one for each type of error hook.
 *     If the bit is set, we're already nested inside the hook.
 *     This is to prevent recursive nesting of the hook functions when
 *     errors are caused by the hook functions themselves.
 *
 * hookApp
 *     This variable contains the address of the current application's
 *     appcontext structure. The content of the variable is only valid
 *     during application-specific error-, startup- and shutdown-hooks.
 *     It contains OS_NULL at all other times.
 *
 * sHookContext
 * eHookContext
 *     These variables store the kernel context from which a call to an
 *     application specific hook function must return. The return must
 *     be via a kernel trap of some sort.
 *     sHookContext is for the startup and shutdown hooks.
 *     eHookContext is for the error hook, which needs a separate context
 *     since it might get called during one of the other hook functions.
 *
 * inInterrupt
 *     This variable is used to determine when we are in an interrupt, and
 *     so whether to switch to the interrupt stack. Its type and semantics
 *     are architecture-dependent.
 *     Some architectures do not need this variable, so if the type macro is
 *     not defined we do not declare the variable.
 *
 *     The initial value of the variable should be 1 because the startup code
 *     runs on the kernel stack.
 *
 * timeStampValue
 * lastTimeStampTime
 * timeStampTimer
 *     Data for the generic time-stamp (used by CPU load measurement and possibly others) if
 *     the architecture doesn't provide a special time-stamp timer. Which hardware timer to
 *     use is defined by the pointer timeStampTimer.
 *
 * The following variables are related to kernel but defined somewhere for user access:
 *
 * errorStatus
 *     This data structure holds the last recorded error. It is set up
 *     in OS_Error() just before the error hook is called.
 *
 *     Its contents can be read using OS_GetErrorInfo().
 *
 * inKernel
 *     This variable indicates whether we are currently running in the
 *     kernel or not. Zero means a task is executing. This means that we
 *     must switch to the kernel stack if an exception or interrupt occurs.
 *
 *     On some architectures (typically those with a system-call mechanism)
 *     this is a flag. On others, including the non-system-call kernels,
 *     it is a counter.
 *
 *     On non-system-call kernels it starts with value 1 so that if the user calls
 *     OS_SuspendAllInterrupts() etc. before StartOS(), the dispatcher doesn't get
 *     called on exit. This means that StartOS() must set the counter to 0.
 *
 * masterPrioWord
 *     This word has a bit set for each word pointed to by OS_GetSlavePrioWord() that
 *     has one of more bits set. If the number of priorities will fit into a
 *     single word, *OS_GetSlavePrioWord(OS_GetMyCoreId(), 0) is always selected and
 *     masterPrioWord is never referenced.
 *
 * iStackBase, iStackLen
 *     The base address and length [bytes] of the interrupt stack
 *
 */

#ifndef OS_ARCH_HAS_KERNELDATA
#define OS_ARCH_HAS_KERNELDATA	0	/* Set default value. */
#endif

#ifndef OS_ASM

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
typedef struct os_kerneldata_s os_kerneldata_t;
/* Deviation MISRAC2012-2 */
struct os_kerneldata_s
{
	/* The variables of interest to assembler code are listed first in this structure.
	 * This is done to allow easy access by small offsets and prevent interferences
	 * from padding to align the members. Therefore, they are listed in descending order
	 * of their sizeof() values.
	 *
	 * ATTENTION!!!
	 * Any modifications (changes of types, order, etc.) of the following members might
	 * require to update their byte-offset values OS_T_* defined below.
	 */

#if (OS_ARCH_HAS_KERNELDATA == 1)
	os_archkerneldata_t	archData;
#endif

	const os_task_t * taskQueueHead;	/* head pointer of the ready queue of this core */
	const os_task_t * taskCurrent;		/* currently running task on this core */
	os_ispinitial_t iSpInitial;			/* kernel stack base of this core */

	os_cpuload_t * const cpuLoad;		/* management data for CPU load measurement on this core */

	const os_appcontext_t *hookApp;

#ifdef OS_TYPEOF_ININTERRUPT
	OS_TYPEOF_ININTERRUPT inInterrupt;
#endif

	os_uint8_t inKernel;

	os_uint8_t inFunction;

	/* The following variables are not of interest to assembler code.
	 * Put new members after this comment, when they aren't accessed by assembler code, too.
	 */
	os_stackelement_t * const iStackBase;
	os_stacklen_t const iStackLen;

	os_isrid_t isrCurrent;
	os_objectid_t isrLastLock;

	os_clzword_t masterPrioWord;

	os_oldlevel_t oldSuspendOs;
	os_oldlevel_t oldSuspendAll;
	os_uint8_t nestSuspendOs;
	os_uint8_t nestSuspendAll;

	os_errorstatus_t errorStatus;
	os_fastsuspendresumestatus_t fastSuspendResumeStatus;

	os_hookcontext_t sHookContext;
	os_hookcontext_t eHookContext;

	os_uint8_t errorHookNesting;

	os_accounting_t accounting;

	os_uint8_t appsStarted;		/* Flag: indicates whether app-specific ErrorHooks are called */

#if (OS_USEGENERICTIMESTAMP && (!OS_USE_GLOBAL_GENERICTIMESTAMP))
	os_timervalue_t lastTimeStampTime;
	os_timestamp_t timeStampValue;
	os_hwt_t const * const timeStampTimer;
#endif

	os_panic_t panicCode;
};
#endif /*(OS_KERNEL_TYPE == OS_MICROKERNEL)*/

#endif /* OS_ASM */

/*!
 * Byte-offsets to members in os_kerneldata_t, which are of interest to assembler code.
 *
 * For use in assembler-language, therefore no sizeof() allowed!
*/
#if (OS_ARCH_HAS_KERNELDATA == 1)
#define OS_MEMBER_SIZEOF_ARCHDATA	OS_SIZEOF_ARCHDATA
#else
#define OS_MEMBER_SIZEOF_ARCHDATA	OS_U(0)
#endif


#if (defined(OS_TYPEOF_ININTERRUPT) && (OS_KERNEL_TYPE != OS_MICROKERNEL))
#define OS_MEMBER_SIZEOF_ININTERRUPT	OS_SIZEOF_ININTERRUPT
#else
#define OS_MEMBER_SIZEOF_ININTERRUPT	OS_U(0)
#endif

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#define OS_MEMBER_SIZEOF_TASKQHEAD		OS_SIZEOF_PTR
#define OS_MEMBER_SIZEOF_TASKCURRENT	OS_SIZEOF_PTR
#define OS_MEMBER_SIZEOF_ISPINITIAL		OS_SIZEOF_PTR
#define OS_MEMBER_SIZEOF_CPULOAD		OS_SIZEOF_PTR
#define OS_MEMBER_SIZEOF_HOOKAPP		OS_SIZEOF_PTR
#define OS_MEMBER_SIZEOF_INKERNEL		OS_U(1)
#else
#define OS_MEMBER_SIZEOF_TASKQHEAD		OS_U(0)
#define OS_MEMBER_SIZEOF_TASKCURRENT	OS_U(0)
#define OS_MEMBER_SIZEOF_ISPINITIAL		OS_U(0)
#define OS_MEMBER_SIZEOF_CPULOAD		OS_U(0)
#define OS_MEMBER_SIZEOF_HOOKAPP		OS_U(0)
#define OS_MEMBER_SIZEOF_INKERNEL		OS_U(0)
#endif

#define OS_T_TASKQHEAD		OS_MEMBER_SIZEOF_ARCHDATA
#define OS_T_TASKCURRENT	(OS_T_TASKQHEAD + OS_MEMBER_SIZEOF_TASKQHEAD)
#define OS_T_ISPINITIAL		(OS_T_TASKCURRENT + OS_MEMBER_SIZEOF_TASKCURRENT)
#define OS_T_CPULOAD		(OS_T_ISPINITIAL + OS_MEMBER_SIZEOF_ISPINITIAL)
#define OS_T_HOOKAPP		(OS_T_CPULOAD + OS_MEMBER_SIZEOF_CPULOAD)
#define OS_T_ININTERRUPT	(OS_T_HOOKAPP + OS_MEMBER_SIZEOF_HOOKAPP)
#define OS_T_INKERNEL		(OS_T_ININTERRUPT + OS_MEMBER_SIZEOF_ININTERRUPT)
#define OS_T_INFUNCTION		(OS_T_INKERNEL + OS_MEMBER_SIZEOF_INKERNEL)

/*!
 * os_kerneldata_t access macros
 *
 * Use the following macros to access members of os_kerneldata_t. This allows to "abstract away" the mechanisms
 * how the respective core-local instance is accessed, because this might depend on the number of (configured) cores,
 * the target and toolchain.
*/
#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#ifndef OS_ASM
/* Deviation MISRAC2012-1 */
extern os_kerneldata_t * const OS_kernel_ptr[];
#endif

#if (OS_N_CORES == 1)
#ifndef OS_ASM
extern os_kerneldata_t
		OS_SECTION_ATTRIB_PRE_DECL(OS_SECTIONNAME(.data, OS_INITCORE, kernelData))
/* Deviation MISRAC2012-3 */
		OS_kernelData
		OS_SECTION_ATTRIB_POST_DECL(OS_SECTIONNAME(.data, OS_INITCORE, kernelData));
#endif

#ifndef OS_GetKernelData
#define OS_GetKernelData()				(&OS_kernelData)
#endif

#else

#ifndef EB_STATIC_CHECK
#ifndef OS_GetKernelData
#define OS_GetKernelData()				(OS_kernel_ptr[(OS_GetMyCoreId())])
#endif

#else

/* In case this code is statically checked, the macro OS_GetKernelData() uses a constant as
 * array subscript to avoid many violations of MISRA-C rule 12.4:
 * "The right hand operand of a logical && or || operator shall not contain side effects."
 */
#ifndef OS_GetKernelData
#define OS_GetKernelData()				(OS_kernel_ptr[0u])
#endif

#endif /* EB_STATIC_CHECK */
#endif /* (OS_N_CORES == 1) */
#endif

/* Support macros to determine the ID of the assigned core of OS entities.
 * These macros return values of type os_coreid_t.
*/
#if (OS_N_CORES == 1)

#define OS_GetMyCoreId()					((os_coreid_t)OS_INITCORE)
#define OS_GetAppCoreId(a)					((os_coreid_t)OS_INITCORE)
#define OS_GetTaskCoreId(tsx)				((os_coreid_t)OS_INITCORE)
#define OS_GetIsrCoreId(isx)				((os_coreid_t)OS_INITCORE)
#define OS_GetResourceCoreId(rsx)			((os_coreid_t)OS_INITCORE)
#define OS_GetAlarmCoreId(asx)				((os_coreid_t)OS_INITCORE)
#define OS_GetCounterCoreId(csx)			((os_coreid_t)OS_INITCORE)
#define OS_GetScheduleTableCoreId(ssx)		((os_coreid_t)OS_INITCORE)
#define OS_GetTrustedFunctionCoreId(tfx)	((os_coreid_t)OS_INITCORE)
#define OS_GetTimerCoreId(tsx)				((os_coreid_t)OS_INITCORE)

#define OS_GetCoreIdFromTaskId(tsx)				((os_coreid_t)OS_INITCORE)
#define OS_GetCoreIdFromAlarmId(asx)			((os_coreid_t)OS_INITCORE)
#define OS_GetCoreIdFromCounterId(csx)			((os_coreid_t)OS_INITCORE)
#define OS_GetCoreIdFromScheduleTableId(ssx)	((os_coreid_t)OS_INITCORE)

#else

#ifndef OS_GetMyCoreId
#error "Compiling for multicore on an architecture or derivative that doesn't support it. Check your makefiles!"
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#define OS_GetCoreIdFromTaskId(tsx)				MK_GetPhysicalCoreId(MK_LibGetTaskCoreId(tsx))
#define OS_GetCoreIdFromAlarmId(asx)			MK_GetPhysicalCoreId(MK_LibGetAlarmCoreId(asx))
#define OS_GetCoreIdFromCounterId(csx)			MK_GetPhysicalCoreId(MK_LibGetCounterCoreId(csx))
#define OS_GetCoreIdFromScheduleTableId(ssx)	MK_GetPhysicalCoreId(MK_LibGetScheduleTableCoreId(ssx))
#else
#define OS_GetAppCoreId(asx)				((asx)->core)
#define OS_GetQmOsObjectCoreId(osx)			OS_GetAppCoreId(OS_GET_APP((osx)->app))
#define OS_GetTaskCoreId(tsx)				OS_GetQmOsObjectCoreId(tsx)
#define OS_GetIsrCoreId(isx)				OS_GetQmOsObjectCoreId(isx)
#define OS_GetResourceCoreId(rsx)			OS_GetQmOsObjectCoreId(rsx)
#define OS_GetTrustedFunctionCoreId(tfx)	OS_GetQmOsObjectCoreId(tfx)
#define OS_GetAlarmCoreId(asx)				OS_GetQmOsObjectCoreId(asx)
#define OS_GetCounterCoreId(csx)			OS_GetQmOsObjectCoreId(csx)
#define OS_GetScheduleTableCoreId(ssx)		OS_GetQmOsObjectCoreId(ssx)

#define OS_GetCoreIdFromTaskId(tsx)				OS_GetTaskCoreId(&OS_taskTableBase[(tsx)])
#define OS_GetCoreIdFromAlarmId(asx)			OS_GetAlarmCoreId(&OS_alarmTableBase[(asx)])
#define OS_GetCoreIdFromCounterId(csx)			OS_GetCounterCoreId(&OS_counterTableBase[(csx)])
#define OS_GetCoreIdFromScheduleTableId(ssx)	OS_GetScheduleTableCoreId(&OS_scheduleTableBase[(ssx)])
#endif /* (OS_KERNEL_TYPE == OS_MICROKERNEL) */

#define OS_GetTimerCoreId(tsx)				((tsx)->core)

#endif /* OS_N_CORES == 1 */

/*!
 * OS_NotifyCore
 *
 * Used for cross-core communication. It triggers an interrupt on a remote core to notify it about new messages
 * in its message queue. It is only necessary in multi-core systems, where it's declared in the derivate-specific
 * head file. Hence, when there is only one core, this function does nothing.
 */
#if (OS_N_CORES == 1)
#define OS_NotifyCore(c)			do { /*do nothing*/ } while(0)
#endif


#if (OS_N_CORES > 1)
#ifndef OS_ASM
extern void OS_SyncHere(void);
extern void OS_SyncHereIf(os_boolean_t);
extern os_result_t OS_DoTryToGetSpinlock(os_autosarspinlockid_t, os_tryspinlock_t *);
extern void OS_SynchronizeAtStart(void);

/* CHECK: NOPARSE */
extern os_boolean_t
	OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.os.OS_coreStarted)
	OS_coreStarted[OS_N_CORES_MAX]
	OS_SECTION_ATTRIB_POST_DECL(.bss.shared.os.OS_coreStarted);
/* CHECK: PARSE */

extern const os_coreprop_t OS_coreProperties[OS_N_CORES_MAX];

#define OS_CoreIsInUse(coreId)		OS_CorePropHasUsedBit(OS_coreProperties[(coreId)])
#define OS_UsrCoreIsInUse(coreId)	OS_CorePropHasUsedBit(OS_coreProperties[OS_GetPhysicalCoreId(coreId)])
#define OS_CoreIdIsValid(coreId)\
	(\
		((coreId) == OS_CORE_ID_THIS_CORE)\
		|| (((coreId) < OS_N_CORES_MAX) && OS_CoreIsInUse(coreId))\
	)

extern void OS_MultiDetermineAppMode(os_appmodeid_t);

#endif /* !OS_ASM */
#else /* OS_N_CORES > 1 */

#define OS_SyncHere() do { /*do nothing*/ } while(0)
#define OS_CoreIsInUse(coreId)	((coreId) == OS_CORE_ID_THIS_CORE)
#define OS_CoreIdIsValid(coreId)	(((coreId) == OS_CORE_ID_THIS_CORE) || ((coreId) == OS_INITCORE))
#endif /* OS_N_CORES > 1 */

#endif /* OS_KERNEL_DATA_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
