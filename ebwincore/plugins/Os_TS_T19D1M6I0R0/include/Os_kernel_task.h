/* Os_kernel_task.h
 *
 * Kernel data structures - architecture-independent. For task and ISR management
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
 * $Id: Os_kernel_task.h 31140 2019-12-05 16:21:02Z sesc270012 $
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
 *
 * MISRAC2012-3) Deviated Rule: 8.6 (required)
 * An identifier with external linkage shall have exactly one external
 * definition.
 *
 * Reason:
 * The definition is dependent on the configuration.
 *
 *
 * MISRAC2012-4) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The parameters are used in a way, which prohibits parentheses.
 */

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: DuplicatedIfCondition
 *   Duplicated 'if' condition regarding OS_IS_WITHIN_TASK_REGION() and
 *   OS_IS_WITHIN_ISR_REGION().
 *
 * Reason:
 *   In case of function call kernels or architectures with no support for
 *   memory protection the use of the macros OS_IS_WITHIN_(TASK|ISR)_REGION may
 *   provoke duplicated 'if' conditions, when used in combination with similar
 *   macros (e.g., OS_IS_WITHIN_APP_REGION) in a if-else cascade. This doesn't
 *   pose a problem, because this warning appears only in common-code owing to
 *   the goal of covering all cases, and hence is intentional.
 */

#ifndef OS_KERNEL_TASK_H
#define OS_KERNEL_TASK_H

#include <Os_defs.h>
#include <Os_timestamp.h>
#include <Os_tool.h>

#ifndef OS_SCHEDULING_ALGORITHM
#error "OS_SCHEDULING_ALGORITHM is not defined."
#endif

/*!
 * OS_configurationId
 *
 * This constant contains the value of OS_GENERATION_ID_OS_H / OS_GENERATION_ID_OS_CONF_H.
 * It only exists if both macros are defined and are equal.
 */
#ifndef OS_ASM
extern const os_uint32_t OS_configurationId;
#endif

/*!
 * os_prio_t
 *
 * Task and resource priority. Needs to be able to cover all task priorities
 * plus all interrupt priorities, so it's 16-bit.
 */
#ifndef OS_ASM
typedef os_uint16_t os_prio_t;
#endif
#define OS_SIZEOF_PRIO	2

/*!
 * os_taskprio_t
 *
 * Data type for priorities where only a task priority is needed.
 *
 * OS_maxPrio is the highest priority that a task can take, including resource priorities.
 * Resources over this priority are shared with ISRs and need to block ISRs too.
 *
 * !LINKSTO Kernel.Architecture.ConformanceClasses.TaskPriosBCC, 1
 * !LINKSTO Kernel.Architecture.ConformanceClasses.TaskPriosECC, 1
 */
#ifndef OS_ASM
typedef os_uint8_t os_taskprio_t;
extern const os_taskprio_t OS_maxPrio[OS_N_CORES_MAX];
#endif

#define OS_MAXPOSSIBLETASKPRIO	OS_U(0xff)
#define OS_NULLISRPRIO			OS_U(0x100)
#define OS_SIZEOF_TPRIO			OS_U(1)

/*!
 * os_act_t
 *
 * Task activation counter. Absolute maximum pending activations is 255,
 * so 8-bit suffices.
 */
#ifndef OS_ASM
typedef os_uint8_t os_act_t;
#endif

#define OS_SIZEOF_ACT			OS_U(1)

/*!
 * os_task_t, os_taskdynamic_t - Task data structure
 *
 * Each task has one of each of these. The constant os_task_t
 * contains the address of the variable os_taskdynamic_t.
 *
 * STATIC
 * dynamic           - pointer to the task's variables
 * accounting        - pointer to the task's accounting variables (or OS_NULL)
 * entry             - the code entry point (function pointer) for the task
 * app               - the application to which the task belongs
 * stackBase         - the stack
 * stackLen          - the length of the stack, in bytes
 * dataStart         - pointer to task's private data/bss
 * dataEnd           - pointer to end of task's private data/bss
 * idataStart        - pointer to initial values for task's data
 * idataEnd          - pointer to end of initial values for task's data
 * execBudget        - the execution time allowed for this task.
 * osLockTime        - Interrupt lock time: OS interrupts
 * allLockTime       - Interrupt lock time: All interrupts
 * resourceLockTime  - pointer to an array of maximum resource lock times
 *                     (indexed by resource id).
 * permissions       - which nontrusted applications can access this task
 * linkBase          - index into the chain table of the task's first task
 *                     queue link
 * queuePrio         - this is the queuing priority of the task. The
 *                     task takes this priority when it is first activated
 *                     and when it voluntarily gives up the CPU. When the task
 *                     is running or preempted the priority my be higher -
 *                     at least the runPrio but will be modified when the task
 *                     takes a resource. This is the "calculated priority" of
 *                     the generator.
 * runPrio           - the lowest running priority for the task (see above).
 *                     this is the largest of all the internal resources occupied
 *                     by the task. It will be MAXTASKPRIO if the task is
 *                     non-preemptive.
 * maxAct            - the maximum value for nAct.
 * flags             - various task flags (NONPREEMPT etc.)
 * taskId            - Id of this task
 * arch              - architecture-specific (optional)
 *
 * DYNAMIC
 * context           - architecture-dependent register values
 * prio              - current priority
 * pEvents           - pending events
 * wEvents           - events that the task is waiting for
 * oldSuspendAll     - level saved by SuspendAllInterrupts
 * oldSuspendOs      - level saved by SuspendOSInterrupts
 * nestSuspendAll    - nesting count for SuspendAllInterrupts
 * nestSuspendOs     - nesting count for SuspendOSInterrupts
 * nAct              - number of pending activations
 * state             - task state (NEW/READY/RUNNING/WAITING/SUSPENDED)
 * lastRes           - head of a chain of resources taken, most recent first.
 * lastLock          - The last lock taken by the task. This can be a resource or a spinlock, i.e. this
 *                     member may be assigned the values OS_CreateObjectId(r, OS_OBJ_RESOURCE) or
 *                     OS_CreateObjectId(s, OS_OBJ_SPINLOCK) for resource 'r' and spinlock 's' respectively.
 * cAct              - index into chain array (offset from linkBase) of current
 *                     activation
 * prioBeforeSuspendOs
 *                   - The priority saved by SuspendOSInterrupts before raising the tasks priority
 *                     to maximum priority.
 */
#ifndef OS_ASM
typedef struct os_task_s os_task_t;
typedef struct os_taskdynamic_s os_taskdynamic_t;
typedef struct os_taskaccounting_s os_taskaccounting_t;

typedef os_uint16_t os_tasklink_t;
#endif

#define OS_SIZEOF_TASKLINK	OS_U(2)

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
#define OS_TASK_DATA_START						((os_address_t)OS_GetKernelData()->taskCurrent->dataStart)
#define OS_TASK_DATA_END						((os_address_t)OS_GetKernelData()->taskCurrent->dataEnd)
#define OS_IS_WITHIN_TASK_REGION(first,last)	(((first) >= OS_TASK_DATA_START) && ((last) <= OS_TASK_DATA_END))
#else
/* Deactivating task data region check for a non system call kernel */
#define OS_TASK_DATA_START						OS_U(0)
#define OS_TASK_DATA_END						OS_U(0)
/* Possible diagnostic TOOLDIAG-1 <1> */
#define OS_IS_WITHIN_TASK_REGION(first,last)	((last) == OS_TASK_DATA_END)
#endif

#define OS_TASK_STACK_START	((os_address_t)OS_GetKernelData()->taskCurrent->stackBase)
#define OS_TASK_STACK_END	(((os_address_t)OS_GetKernelData()->taskCurrent->stackBase) + \
							 OS_GetKernelData()->taskCurrent->stackLen)

#if OS_HAVE_ARCHTASK_T
#define OS_TaskCfg_arch_decl(x)		x;
#else
#define OS_TaskCfg_arch_decl(x)
#endif

#ifndef OS_ASM
/* Deviation MISRAC2012-2 <+2> */
/* Deviation MISRAC2012-4 <START> */
struct os_task_s
{
	/* Task's static configuration.
	 *
	 * IF YOU CHANGE THIS YOU NEED TO CHANGE THE DEFINITIONS
	 * BELOW TO REPRESENT THE OFFSET TO ASSEMBLER CODE!
	 *
	 * See OS_T_* macros defined below.
	 */
	os_taskdynamic_t *dynamic;
	os_taskprio_t runPrio;
	os_taskprio_t queuePrio;
	os_tasklink_t linkBase;
	os_uint8_t flags;
	os_act_t maxAct;
	os_taskid_t taskId;
	os_entry_t entry;
	os_stackelement_t *stackBase;
	os_stacklen_t stackLen;
	OS_Cfg_acct_decl(os_taskaccounting_t *accounting)
	OS_Cfg_mp_decl(os_uint8_t *dataStart)
	OS_Cfg_mp_decl(os_uint8_t *dataEnd)
	OS_Cfg_mp_decl(os_uint8_t *idataStart)
	OS_Cfg_mp_decl(os_uint8_t *idataEnd)
	OS_Cfg_tp_decl(os_tick_t execBudget)
	OS_Cfg_ilock_decl(os_tick_t osLockTime)
	OS_Cfg_ilock_decl(os_tick_t allLockTime)
	OS_Cfg_rlock_decl(const os_tick_t *resourceLockTime)
	OS_Cfg_rmon_decl(os_ratemonitorid_t rateMonitor)
	OS_Cfg_app_decl(const os_appcontext_t *app)
	OS_Cfg_app_decl(os_permission_t permissions)
	OS_TaskCfg_arch_decl(os_archtask_t arch)
};
/* Deviation MISRAC2012-4 <STOP> */

#define OS_TASKCONFIG_INIT(dynamic, runPrio, queuePrio, linkBase, entry, stackBase,			\
		stackLen, maxAct, flags, taskId, accounting, dataStart,	dataEnd, idataStart,		\
		idataEnd, execBudget, osLockTime, allLockTime, resourceLockTime, rateMonitor,		\
		app, permissions, arch)																\
	{																						\
		dynamic,																			\
		runPrio,																			\
		queuePrio,																			\
		linkBase,																			\
		flags,																				\
		maxAct,																				\
		taskId,																				\
		entry,																				\
		stackBase,																			\
		stackLen,																			\
		OS_Cfg_acct_init(accounting)														\
		OS_Cfg_mp_init(dataStart)															\
		OS_Cfg_mp_init(dataEnd)																\
		OS_Cfg_mp_init(idataStart)															\
		OS_Cfg_mp_init(idataEnd)															\
		OS_Cfg_tp_init(execBudget)															\
		OS_Cfg_ilock_init(osLockTime)														\
		OS_Cfg_ilock_init(allLockTime)														\
		OS_Cfg_rlock_init(resourceLockTime)													\
		OS_Cfg_rmon_init(rateMonitor)														\
		OS_Cfg_app_init(app)																\
		OS_Cfg_app_init(permissions)														\
		arch																				\
	}
/* Deviation MISRAC2012-2 */
struct os_taskdynamic_s
{
	/* Task's dynamic state
	 * IF YOU CHANGE THIS YOU NEED TO CHANGE THE DEFINITIONS
	 * BELOW TO REPRESENT THE OFFSET TO ASSEMBLER CODE!
	*/
	os_archcontext_t	context;
	os_prio_t			prio;
	os_taskstate_t		state;
	os_eventmask_t		pEvents;
	os_eventmask_t		wEvents;
	os_oldlevel_t		oldSuspendOs;
	os_oldlevel_t		oldSuspendAll;
	os_uint8_t			nestSuspendOs;
	os_uint8_t			nestSuspendAll;
	os_act_t			nAct;
	os_objectid_t		lastLock;
#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
	os_uint8_t			cAct;
#endif
	os_prio_t			prioBeforeSuspendOs;
};

/* Deviation MISRAC2012-2 */
struct os_taskaccounting_s
{
	os_tick_t		etUsed;			/* Execution budget/lock time used so far */
	os_tick_t		osSaveUsed;		/* etUsed time (save during "OS" interrupt lock timing) */
	os_tick_t		allSaveUsed;	/* etUsed time (save during "All" interrupt lock timing) */
	os_tick_t		etLimit;		/* Execution budget/lock time limit */
	os_tick_t		osSaveLimit;	/* etLimit (save during "OS" interrupt lock timing) */
	os_tick_t		allSaveLimit;	/* etLimit (save during "All" interrupt lock timing) */
	os_tick_t		etMax;			/* Longest execution time observed */
	os_uint8_t		etType;			/* global etType */
	os_uint8_t		osSaveType;		/* global etType (save during "OS" interrupt lock timing) */
	os_uint8_t		allSaveType;	/* global etType (save during "All" interrupt lock timing) */
};

#ifndef OS_ClearPendingEvents
#define OS_ClearPendingEvents(td)	(td)->pEvents = 0
#endif

#ifdef OS_EXCLUDE_MULTIPLE_ACTIVATIONS
#define OS_TaskHasMultipleActivations(tp)	OS_FALSE
#else
#define OS_TaskHasMultipleActivations(tp)	((tp)->maxAct != 0)
#endif

#if (!defined(OS_FindPrio)) && (!defined(OS_CurrentLink)) && (!defined(OS_ClearCurrentActivation)) && \
	(!defined(OS_ActivationLimitExceeded)) && (!defined(OS_ActivationPending))

#define OS_FindPrio(q)	\
	( ( (q) == OS_CurrentLink(OS_GetTaskPtrs(q), OS_GetTaskPtrs(q)->dynamic) )	\
		? OS_GetTaskPtrs(q)->dynamic->prio										\
		: OS_GetTaskPtrs(q)->queuePrio											\
	)

#define OS_ActivationLimitExceeded(tp, td)	((td)->nAct >= (tp)->maxAct)
#define OS_ActivationPending(td)			((td)->nAct > 0)

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#define OS_CurrentLink(tp, td)				0
#else
#define OS_CurrentLink(tp, td)				((tp)->linkBase + (os_tasklink_t)(td)->cAct)
#define OS_ClearCurrentActivation(td)		(td)->cAct = 0
#define OS_Enqueue(tp)						OS_EnqueueLink((tp)->linkBase, (tp)->queuePrio)
#define OS_IncCurrentActivation(tp, td)		do {														\
												if (((td)->nAct == 0) || ((td)->cAct >= (tp)->maxAct))	\
												{														\
													(td)->cAct = 0;										\
												}														\
												else													\
												{														\
													(td)->cAct++;										\
												}														\
											} while(0)
#define OS_ChainTask(tp,td)					do {															\
												os_tasklink_t link;											\
												link = (os_tasklink_t)(td)->cAct + (os_tasklink_t)(td)->nAct;	\
												if ( link > (os_tasklink_t)(tp)->maxAct )						\
												{															\
													link -= ((os_tasklink_t)(tp)->maxAct + 1);				\
												}															\
												link += (tp)->linkBase;										\
																											\
												OS_EnqueueLink(link, (tp)->queuePrio);						\
											} while(0)
#endif
#endif

#ifndef OS_InitTaskEtb
#define OS_InitTaskEtb(tp) \
	do {												\
		if ( (tp)->accounting != OS_NULL )				\
		{												\
			(tp)->accounting->etUsed = 0;				\
			(tp)->accounting->etType = OS_ACC_BUDGET;	\
		}												\
	} while(0)
#endif

extern const os_taskid_t OS_nTasks;
extern const os_task_t *OS_taskQueueHead;
extern const os_task_t * const OS_taskTableBase;

#if (OS_N_CORES == 1)
/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern const os_task_t * const OS_taskPtrs[];

/* Deviation MISRAC2012-1, MISRAC2012-3 <+3> */
extern os_tasklink_t
		OS_SECTION_ATTRIB_PRE_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, taskActivations))
		OS_taskActivations[]
		OS_SECTION_ATTRIB_POST_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, taskActivations));

#define OS_GetTaskPtrs(idx)				(OS_taskPtrs[(idx)])
#define OS_GetTaskActivations()		(OS_taskActivations)

#else

#define OS_GetTaskPtrs(idx)			((OS_taskPtrs_ptr[OS_GetMyCoreId()])[(idx)])
#define OS_GetTaskActivations()	(OS_taskActivations_ptr[OS_GetMyCoreId()])

/* Deviation MISRAC2012-1, MISRAC2012-3 <+2> */
extern const os_task_t * const * const OS_taskPtrs_ptr[];
extern os_tasklink_t * const OS_taskActivations_ptr[];

#endif /* OS_N_CORES == 1 */

#endif /* OS_ASM */

/*!
 * OS_GetNTasks(), OS_InternalActivateTask(), OS_InternalSetEvent()
 *
 * In the microkernel-based system these are the internal interfaces between the QM-OS and the
 * microkernel. For a standard system we must construct them in terms of internal functions.
*/
#define OS_GetNTasks()					OS_nTasks
#define OS_InternalActivateTask(tid)	OS_KernActivateTask(tid)
#define OS_InternalSetEvent(tid, evt)	OS_KernSetEvent((tid), (evt))

/*!
 * OS_T_DYNA defines the offset of the pointer to the dynamic task structure in os_task_t.
 * OS_T_RPRIO defines the offset of the runPrio in os_task_t.
 * OS_T_FLAGS defines the offset of the flags in os_task_t.
 * OS_T_CTXT defines the offset of the architecture-dependent task context in os_taskdynamic_t
 * OS_T_PRIO defines the offset of the current priority in os_taskdynamic_t
 * OS_T_STAT defines the offset of the task status in os_taskdynamic_t
 * For use in assembler-language, therefore no sizeof() allowed!
 */
#define OS_T_DYNA	OS_U(0)
#define OS_T_RPRIO	(OS_SIZEOF_PTR)
#define OS_T_FLAGS	( OS_SIZEOF_PTR \
					+ OS_SIZEOF_TPRIO \
					+ OS_SIZEOF_TPRIO \
					+ OS_SIZEOF_TASKLINK \
					)

#define OS_T_CTXT	OS_U(0)
#define OS_T_PRIO	(OS_T_CTXT + OS_SIZEOF_CTXT)
#define OS_T_STAT	(OS_T_PRIO + OS_SIZEOF_PRIO)

/* Context flags */
#define OS_CXF_IFULL		OS_U(0x01)	/* Full integer context */
#define OS_CXF_FLOAT		OS_U(0x10)	/* Has saved FPU context */
#define OS_CXF_FFULL		OS_U(0x20)	/* FPU context is full */

/* Task flags */
#define OS_TF_NONPREEMPT	OS_U(0x01)	/* Non-preepmtive task */
#define OS_TF_EXTENDED		OS_U(0x02)	/* Extended task - can call WaitEvent */
#define OS_TF_MEASUREEXEC	OS_U(0x40)	/* Longest execution time will be measured */
#define OS_TF_USEFPU		OS_U(0x80)	/* Uses FPU */

/*!
 * OS_IsValidIsrId
 *
 * Checks, if the given ISR ID is valid.
 */
#define OS_IsValidIsrId(isrid)	OS_DoExtendedStatusTrue((isrid) < OS_nInterrupts)

/*!
 * os_isr_t
 * os_isrdynamic_t
 *
 * Structures describing an ISR to the kernel.
 *
 * The constant (ROM) structure contains the following:
 *   app               - pointer to owner application's context.
 *   dataStart         - pointer to ISR's data/bss segment.
 *   dataEnd           - limit of ISR's data/bss segment.
 *   idataStart        - pointer to initialisation values for data.
 *   idataEnd          - limit of initialisation values for data.
 *   func              - pointer to the ISR function
 *   stackBase         - private stack (not supported) OS_NULL ==> use kernel stack
 *   execBudget        - max. runtime
 *   interruptLockTime - max. interrupt lock time
 *   resourceLockTime  - array of max. resource lock times
 *   stackLen          - max. stack length.
 *   rateMonitor       - id of rate monitor.
 *   isrId             - The ID of the ISR.
 *   dynamic           - Points to the dynamic part of the ISR.
 *   flags             - various config flags.
 *   arch              - architecture-specific stuff.
 *
 * There is a constant table of interrupts defined by the generator,
 * and a constant telling the kernel how many interrupts are in the
 * table.
 */
#ifndef OS_ASM
typedef struct os_isr_s os_isr_t;
typedef struct os_isrdynamic_s os_isrdynamic_t;
typedef struct os_israccounting_s os_israccounting_t;

typedef void (*os_isrfunc_t)(void);

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
#define OS_ISR_DATA_START					((os_address_t)isr->dataStart)
#define OS_ISR_DATA_END						((os_address_t)isr->dataEnd)
#define OS_IS_WITHIN_ISR_REGION(first,last)	(((first) >= OS_ISR_DATA_START) && ((last) <= OS_ISR_DATA_END))
#else
/* Deactivating interrupt data region check for a non system call kernel */
#define OS_ISR_DATA_START					OS_U(0)
#define OS_ISR_DATA_END						OS_U(0)
/* Possible diagnostic TOOLDIAG-1 <1> */
#define OS_IS_WITHIN_ISR_REGION(first,last)	((last) == OS_ISR_DATA_END)
#endif

/* Deviation MISRAC2012-2 <+2> */
/* Deviation MISRAC2012-4 <START> */
struct os_isr_s
{
	OS_Cfg_app_decl(const os_appcontext_t *app)
	OS_Cfg_app_decl(os_permission_t permissions)
	OS_Cfg_acct_decl(os_israccounting_t *accounting)
	OS_Cfg_mp_decl(os_uint8_t *dataStart)
	OS_Cfg_mp_decl(os_uint8_t *dataEnd)
	OS_Cfg_mp_decl(os_uint8_t *idataStart)
	OS_Cfg_mp_decl(os_uint8_t *idataEnd)
	os_isrfunc_t func;
	OS_Cfg_tp_decl(os_tick_t execBudget)
	OS_Cfg_ilock_decl(os_tick_t osLockTime)
	OS_Cfg_ilock_decl(os_tick_t allLockTime)
	OS_Cfg_rlock_decl(const os_tick_t *resourceLockTime)
	os_stacklen_t stackLen;
	OS_Cfg_rmon_decl(os_ratemonitorid_t rateMonitor)
	os_isrid_t isrId;
	os_isrdynamic_t *dynamic;
	os_uint8_t flags;
	os_archisr_t arch;
};
/* Deviation MISRAC2012-4 <STOP> */

#define OS_ISRCONFIG_INIT( app, permissions, accounting, dataStart, dataEnd,			\
			idataStart, idataEnd, func, execBudget, osLockTime, allLockTime,			\
			resourceLockTime, stackLen, rateMonitor, isrId, dyn, flags, arch )			\
	{																					\
		OS_Cfg_app_init(app)															\
		OS_Cfg_app_init(permissions)													\
		OS_Cfg_acct_init(accounting)													\
		OS_Cfg_mp_init(dataStart)														\
		OS_Cfg_mp_init(dataEnd)															\
		OS_Cfg_mp_init(idataStart)														\
		OS_Cfg_mp_init(idataEnd)														\
		func,																			\
		OS_Cfg_tp_init(execBudget)														\
		OS_Cfg_ilock_init(osLockTime)													\
		OS_Cfg_ilock_init(allLockTime)													\
		OS_Cfg_rlock_init(resourceLockTime)												\
		stackLen,																		\
		OS_Cfg_rmon_init(rateMonitor)													\
		isrId,																			\
		dyn,																			\
		flags,																			\
		arch																			\
	}

/* Deviation MISRAC2012-2 */
struct os_isrdynamic_s
{
	os_savedcontext_t c;
	os_uint8_t statusflags;
#if OS_HAVE_ARCHISRDYN_T
	os_archisrdynamic_t arch; /* Architecture-specific (optional) */
#endif
};

/* Deviation MISRAC2012-2 */
struct os_israccounting_s
{
	os_tick_t etMax;
};

extern const os_isrid_t OS_nInterrupts;
extern os_isr_t const * const OS_isrTableBase;

#endif /* OS_ASM */

/* Converts interrupt level to interrupt priority */
#define OS_IsrLevelToPrio(p)	((p) + OS_NULLISRPRIO)

/* Interrupt vector types
 */
#define OS_VECTOR_TYPE_INVALID		OS_U(0)
#define OS_VECTOR_TYPE_CAT1			OS_U(1)
#define OS_VECTOR_TYPE_CAT2			OS_U(2)
#define OS_VECTOR_TYPE_CATK			OS_U(3)
#define OS_VECTOR_TYPE_UNKNOWN		OS_U(4)
#define OS_VECTOR_TYPE_CATCC		OS_U(5)
#define OS_VECTOR_TYPE_USER1		OS_U(6)
#define OS_VECTOR_TYPE_SPURIOUS		OS_U(7)

/* Bit-fields in ISR flags
 */
#define OS_ISRF_ENABLE		OS_U(0x80)
#define OS_ISRF_MEASUREEXEC	OS_U(0x40)
#define OS_ISRF_CROSSCORE	OS_U(0x20)
#define OS_ISRF_CAT			OS_U(0x03)
#define OS_ISRF_CATT		OS_U(0x00)
#define OS_ISRF_CAT1		OS_U(0x01)
#define OS_ISRF_CAT2		OS_U(0x02)
#define OS_ISRF_CATK		OS_U(0x03)

/* Bit-fields in ISR statusflags
 * RATEEX  = 1: arrival rate is reached/exceeded, ISR disabled
 * BLOCKED = 1: ISR disabled by DisableInterruptSource()
 */
#define OS_ISRF_RATEEX	OS_U(0x10)
#define OS_ISRF_BLOCKED	OS_U(0x20)
#define OS_ISRF_KILLED	OS_U(0x40)

/* OS_TriggerIsr
 *
 * If hardware interrupts cannot be easily triggered we can fall back to simply calling the ISR
 * at the place where the interrupt should be triggered.
 */
#define OS_TriggerIsrGeneric(i)	\
	do {						\
		(*(i)->func)();			\
	} while(0)

#ifndef OS_TriggerIsr
#define OS_TriggerIsr(i)	OS_TriggerIsrGeneric(i)
#endif

/* OS_KernelStackRemaining()
 *
 * Returns no. of bytes of kernel stack remaining. In case of overflow a negative number is returned, which
 * reflects the number of bytes by which the kernel stack is overused.
 *
 * OS_InsufficientStackForIsr()
 * OS_InsufficientKernelStack()
 * OS_KernelStackOverflow()
 * OS_TaskStackOverflow()
 *
 * Check if there's enough kernel stack for this ISR. If the ISR
 * has a private stack we can assume it is big enough.
 *
 * Returns true if a stack overflow will occur.
 */
#if (OS_STACKGROWS == OS_STACKGROWSUP)

#define OS_KernelStackRemaining() \
		(   (os_char_t *)OS_GetKernelData()->iStackBase + OS_GetKernelData()->iStackLen \
		  - (os_char_t *)OS_GetCurrentSp() \
		)

#define OS_OptInsufficientKernelStack(len)	( (os_ptrdiff_t)OS_KernelStackRemaining() <= (os_ptrdiff_t)(len) )

#ifndef OS_OptInsufficientStackForIsr
#define OS_OptInsufficientStackForIsr(isr)	( OS_OptInsufficientKernelStack((isr)->stackLen) )
#endif

#ifndef OS_OptInsufficientStackErrHook
#define OS_OptInsufficientStackErrHook(app)	( OS_OptInsufficientKernelStack((app)->errorHookStack) )
#endif

#ifndef OS_OptInsufficientStackStHook
#define OS_OptInsufficientStackStHook(app)	( OS_OptInsufficientKernelStack((app)->startupHookStack) )
#endif

#ifndef OS_OptInsufficientStackShHook
#define OS_OptInsufficientStackShHook(app)	( OS_OptInsufficientKernelStack((app)->shutdownHookStack) )
#endif

#define OS_OptKernelStackOverflow() \
	(*((os_stackelement_t *)OS_GetKernelData()->iStackBase \
			+ OS_GetKernelData()->iStackLen/sizeof(os_stackelement_t) - 1) != OS_STACKFILL)

#define OS_OptTaskStackOverflow(tp,td) \
	(	(*(os_stackelement_t *)((tp)->stackBase + (tp)->stackLen/sizeof(os_stackelement_t) - 1) \
		!= OS_STACKFILL) \
	||	(OS_TaskSpOutOfBounds(OS_GetTaskSpForce((tp),(td)),(tp))) \
	)

#define OS_GetInitialStackPointer(base, size)	((os_stackelement_t*)(base))

#else	/* (OS_STACKGROWS == OS_STACKGROWSDOWN) */

/* Returns the remaining kernel stack size.
 * To get the difference in bytes, the pointers are reinterpreted as uint8 pointers.
*/
#define OS_KernelStackRemaining() \
		(((os_uint8_t*)((void*)OS_GetCurrentSp())) - ((os_uint8_t*)((void*)(OS_GetKernelData()->iStackBase))))

#define OS_OptKernelStackOverflow()		(*(os_stackelement_t *)OS_GetKernelData()->iStackBase != OS_STACKFILL)

#define OS_OptTaskStackOverflow(tp,td) \
		( (*((tp)->stackBase) != OS_STACKFILL) || \
		  ( OS_TaskSpOutOfBounds(OS_GetTaskSpForce((tp),(td)),(tp))  ))

/* Get the initial stack pointer of a stack that grows down.
 * 'size' should be a multiple of os_stackelement_t.
*/
#define OS_GetInitialStackPointer(base, size)	\
	(&(((os_stackelement_t*)(base))[(size) / sizeof(os_stackelement_t)]))

#endif /* (OS_STACKGROWS == OS_STACKGROWSUP) */

#ifndef OS_GetCurrentSpAdjusted
#define OS_GetCurrentSpAdjusted()	((os_stackelement_t *)OS_GetCurrentSp())
#endif

#define OS_OptInsufficientKernelStack(len)	( OS_KernelStackRemaining() <= (os_ptrdiff_t)(len) )

#ifndef OS_OptInsufficientStackForIsr
#define OS_OptInsufficientStackForIsr(isr)	( OS_OptInsufficientKernelStack((isr)->stackLen) )
#endif

#ifndef OS_OptInsufficientStackErrHook
#define OS_OptInsufficientStackErrHook(app)	( OS_OptInsufficientKernelStack((app)->errorHookStack) )
#endif

#ifndef OS_OptInsufficientStackStHook
#define OS_OptInsufficientStackStHook(app)	( OS_OptInsufficientKernelStack((app)->startupHookStack) )
#endif

#ifndef OS_OptInsufficientStackShHook
#define OS_OptInsufficientStackShHook(app)	( OS_OptInsufficientKernelStack((app)->shutdownHookStack) )
#endif

#ifndef OS_ASM
os_boolean_t OS_TaskSpOutOfBounds(os_address_t, const os_task_t *);
#endif

#ifndef OS_DoStackCheck
/* Non-optimized version
 */
#define OS_DoStackCheck(x)	(	(   ((OS_configMode & OS_STACKCHECK) != 0U) \
								  ? (os_boolean_t)((os_boolean_t)(x) != OS_FALSE) \
								  : OS_FALSE \
								) \
								!= OS_FALSE \
							)
#endif

#define OS_InsufficientStackForIsr(isr)		OS_DoStackCheck( OS_OptInsufficientStackForIsr(isr) )
#define OS_InsufficientStackForErrHook(app)	OS_DoStackCheck( OS_OptInsufficientStackErrHook(app) )
#define OS_InsufficientStackForStHook(app)	OS_DoStackCheck( OS_OptInsufficientStackStHook(app) )
#define OS_InsufficientStackForShHook(app)	OS_DoStackCheck( OS_OptInsufficientStackShHook(app) )
#define OS_InsufficientKernelStack(len)		OS_DoStackCheck( OS_OptInsufficientKernelStack(len) )
#define OS_KernelStackOverflow()			OS_DoStackCheck( OS_OptKernelStackOverflow() )
#define OS_TaskStackOverflow(tp,td)			OS_DoStackCheck( OS_OptTaskStackOverflow(tp,td) )

/*!
 * os_resource_t and os_resourcedynamic_t
 *
 * These two structures represent the constant and variable parts of a
 * Resource to the kernel.
 *
 * STATIC
 * app         - owner application
 * permissions - which nontrusted applications can access this resource
 * prio        - ceiling priority
 * dynamic	   - Points to the dynamic part of a resource.
 *
 * DYNAMIC
 * etUsed      - remaining exec. time before resource was taken (if timed),
 *               global etUsed is saved here while resource is held
 * etLimit     - global etLimit is saved here while resource is held
 * etbType     - type of timing that was being done, global etType is saved here while resource is held
 * lastPrio    - priority of task/ISR before taking resource
 * takenBy     - which task has taken the resource
 * next        - Identifies the lock taken before the current one. If the value is OS_NULLOBJECT, there is no such lock.
 * 				 This member may be assigned the value OS_CreateObjectId(r, OS_OBJ_RESOURCE) or
 * 				 OS_CreateObjectId(r, OS_OBJ_SPINLOCK), depending on whether the previously taken lock was the resource
 * 				 with ID 'r' or the spinlock with ID 's'.
 *
 * How to determine if a resource is in use:
 * - If the takenBy field is a valid task ID, the resource has been taken
 *   by a task.
 * - If the takenBy field contains OS_NULLTASK, the lastPrio should
 *   be examined. If it contains an interrupt priority, the resource
 *   has been taken by an ISR. Otherwise (normally 0) the resource is free.
 */
#ifndef OS_ASM
typedef struct os_resource_s os_resource_t;
typedef struct os_resourcedynamic_s os_resourcedynamic_t;

/* Deviation MISRAC2012-2 <+2> */
/* Deviation MISRAC2012-4 <START> */
struct os_resource_s
{
	OS_Cfg_app_decl(const os_appcontext_t *app)
	OS_Cfg_app_decl(os_permission_t permissions)
	os_prio_t prio;
	os_resourcedynamic_t *dynamic;
};
/* Deviation MISRAC2012-4 <STOP> */

#define OS_RESOURCES_INIT( app, permissions, prio, dyn )	\
	{														\
		OS_Cfg_app_init(app)								\
		OS_Cfg_app_init(permissions)						\
		prio,												\
		dyn													\
	}

/* Deviation MISRAC2012-2 <+2> */
/* Deviation MISRAC2012-4 <START> */
struct os_resourcedynamic_s
{
	OS_Cfg_rlock_decl(os_tick_t etUsed)
	OS_Cfg_rlock_decl(os_tick_t etLimit)
	os_prio_t lastPrio;
	os_taskorisr_t takenBy;
	os_objectid_t next;
	OS_Cfg_rlock_decl(os_uint8_t etType)
};
/* Deviation MISRAC2012-4 <STOP> */

/* Resource flags */
#define OS_RES_INTERNAL	OS_U(0x01)

/* OS_ResourceGetDynamic - get the dynamic state of a resource.
*/
#define OS_ResourceGetDynamic(r)	(OS_resourceTableBase[(r)].dynamic)

/* OS_resSchedulerMap - Maps a core id to the respective RES_SCHEDULER resource id.
*/
#if (OS_N_CORES > 1)
extern const os_resourceid_t OS_resSchedulerMap[OS_N_CORES_MAX];
#endif

extern const os_resource_t * const OS_resourceTableBase;
extern const os_resourceid_t OS_nResources;

#define OS_ResourceInUse(r)	((r)->takenBy != OS_NULLTASK)

#ifndef OS_IsIsrPrio
#define OS_IsIsrPrio(p)		((p) > OS_MAXPOSSIBLETASKPRIO)
#endif

#ifndef OS_ResourceInvalidForIsr
#define OS_ResourceInvalidForIsr(isrp, rs)	(OS_IsrLevelToPrio(OS_GetIsrResPrio(isrp)) > (rs)->prio)
#endif

#ifndef OS_IsrTookLock
#define OS_IsrTookLock(id, type)	(OS_GetKernelData()->isrLastLock == OS_CreateObjectId((id), (type)))
#endif

#endif /* OS_ASM */

/*!
 * os_function_t
 *
 * Data structure for the trusted functions table.
 */
#ifndef OS_ASM

typedef struct os_function_s os_function_t;
typedef void (*os_functionaddress_t)(os_functionid_t, void *);

/* Deviation MISRAC2012-2 */
struct os_function_s
{
	const os_appcontext_t *app;		/* Owner application */
	os_functionaddress_t function;	/* Function address */
	os_permission_t permissions;	/* Access permissions */
	os_stacklen_t stackLen;			/* Stack needed */
};

extern const os_functionid_t OS_nFunctions;
extern const os_function_t * const OS_functionTableBase;

#endif /* OS_ASM */

/*!
 * os_hookdynamic_t
 *
 * Data structure holding the return context for application-specific
 * hook functions, so that they can be killed after a protection violation.
 *
 * The information stored is identical to that for an ISR, but a different
 * structure is used so that any future non-related extensions to the ISR
 * structure won't affect the hook structure as well.
 *
 * Only two contexts are needed; the startup and shutdown hooks can share
 * a context but the error hook needs a different one because the error
 * hook can be called while in the startup or shutdown hook.
 */
#ifndef OS_ASM

typedef struct os_hookcontext_s os_hookcontext_t;

/* Deviation MISRAC2012-2 */
struct os_hookcontext_s
{
	os_savedcontext_t c;
};

#define OS_HOOKCONTEXT_INIT { OS_SAVEDCONTEXT_INIT }

#endif

/*!
 * OS_iStackLen
 *
 * This constant defines the length (in bytes) of the interrupt/kernel stack.
 *
 * OS_GetKernelData()->iSpInitial contains the initial interrupt/kernel stack pointer. OS_GetKernelData()->iStackBase
 * denotes the base address of the interrupt/kernel stack with the length OS_iStackLen. Hence, the this stack ranges
 * from OS_GetKernelData()->iStackBase to OS_GetKernelData()->iStackBase + OS_iStackLen (exclusive). Depending on
 * the direction of growth, OS_GetKernelData()->iSpInitial starts at one end of this range. The macro OS_STACKGROWS
 * defines the direction of growth.
 *
 * The default type for OS_GetKernelData()->iSpInitial is 'void * const', but if the macro OS_ARCH_DEFINES_ISPINITIAL_T
 * is defined we don't declare the type here. That permits the architecture to have (for example) a non-constant
 * OS_GetKernelData()->iSpInitial. This could be used if the kernel needs to switch to a different stack for ISRs etc.
 */
#ifndef OS_ASM

#ifndef OS_ARCH_DEFINES_ISPINITIAL_T
typedef  os_stackelement_t * const os_ispinitial_t;
#endif

#endif /* OS_ASM */

/*!
 * Execution timing macros.
 */
#ifndef OS_ASM

#ifndef OS_ARCH_STARTTASKEXECTIMING
#define OS_ARCH_STARTTASKEXECTIMING(t,s)
#endif

#ifndef OS_ARCH_STOPTASKEXECTIMING
#define OS_ARCH_STOPTASKEXECTIMING(t)
#endif

#ifndef OS_ARCH_PREEMPTTASKEXECTIMING
#define OS_ARCH_PREEMPTTASKEXECTIMING(t)
#endif

#endif

/*!
 * os_accounting_t etc.
 *
 * Data structure for storing dynamic task/ISR accounting information.
 * Functions that perform the accounting.
 * An array of function pointers for the functions. The array
 * elements can be OS_NULL.
 */
#ifndef OS_ASM
typedef struct os_accounting_s os_accounting_t;
typedef struct os_accountingsave_s os_accountingsave_t;

/* Deviation MISRAC2012-2 */
struct os_accounting_s
{
	os_timervalue_t frc;			/* FRC at last measurement */
	os_tick_t etUsed;				/* Execution budget/lock time used so far */
	os_tick_t etLimit;				/* Execution budget/lock time limit */
	os_tick_t osSaveUsed;			/* etLimit saved here during ISR "OS" interrupt lock timing */
	os_tick_t allSaveUsed;			/* etLimit saved here during ISR "All" interrupt lock timing */
	os_tick_t osSaveLimit;			/* etLimit saved here during ISR "OS" interrupt lock timing */
	os_tick_t allSaveLimit;			/* etLimit saved here during ISR "All" interrupt lock timing */
	os_uint8_t inFunction;			/* What's being timed - TASK or ISR */
	os_uint8_t etType;				/* What's being timed - Budget, ResourceLock or InterruptLock? */
	os_uint8_t osSaveType;			/* etType saved here during ISR "OS" interrupt lock timing */
	os_uint8_t allSaveType;			/* etType saved here during ISR "All" interrupt lock timing */
	os_uint8_t taskTimingNesting;	/* Indicates that task timing is nesting - prevents unwanted recursion. */
};

#define OS_ACCOUNTINGS_INIT			\
	{								\
		0u, /*frc*/					\
		0u, /*etUsed*/				\
		0u, /*etLimit*/				\
		0u, /*osSaveUsed*/			\
		0u, /*allSaveUsed*/			\
		0u, /*osSaveLimit*/			\
		0u, /*allSaveLimit*/		\
		0u, /*inFunction*/			\
		0u, /*etType*/				\
		0u, /*osSaveType*/			\
		0u, /*allSaveType*/			\
		0u /*taskTimingNesting*/	\
	}

/* Deviation MISRAC2012-2 */
struct os_accountingsave_s
{
	os_tick_t etUsed;				/* Execution budget/lock time used so far */
	os_tick_t etLimit;				/* Execution budget/lock time limit */
	os_uint8_t inFunction;			/* What's being timed - TASK or ISR */
	os_uint8_t etType;				/* What's being timed - Budget, ResourceLock or InterruptLock? */
};

/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern const os_tick_t OS_resourceLockTimes[];

#if (!defined(OS_USE_OPTIMIZATION_OPTIONS)) || (!defined(OS_EXCLUDE_TIMINGPROTECTION))
/* We don't really need to eliminate these from here, but it provides some
 * sanity checking of the kernel at compile time if we do.
 */
typedef void (*os_starttaskexectimingfp_t)(const os_task_t *);
typedef void (*os_preempttaskexectimingfp_t)(const os_task_t *);
typedef void (*os_stoptaskexectimingfp_t)(const os_task_t *);
typedef void (*os_starttaskintlocktimingfp_t)(const os_task_t *, os_intlocktype_t);
typedef void (*os_stoptaskintlocktimingfp_t)(const os_task_t *, os_intlocktype_t);
typedef void (*os_startreslocktimingfp_t)(os_resourcedynamic_t *, os_tick_t);
typedef void (*os_stopreslocktimingfp_t)(os_resourcedynamic_t *);
typedef void (*os_startisrexectimingfp_t)(os_tick_t);
typedef void (*os_preemptisrexectimingfp_t)(os_accountingsave_t *);
typedef void (*os_resumeisrexectimingfp_t)(os_accountingsave_t *);
typedef void (*os_stopisrexectimingfp_t)(const os_isr_t *);
typedef void (*os_startisrintlocktimingfp_t)(os_isrid_t,os_intlocktype_t);
typedef void (*os_stopisrintlocktimingfp_t)(os_isrid_t,os_intlocktype_t);

extern const os_starttaskexectimingfp_t		OS_startTaskExecTimingFp;
extern const os_preempttaskexectimingfp_t	OS_preemptTaskExecTimingFp;
extern const os_stoptaskexectimingfp_t		OS_stopTaskExecTimingFp;
extern const os_starttaskintlocktimingfp_t	OS_startTaskIntLockTimingFp;
extern const os_stoptaskintlocktimingfp_t	OS_stopTaskIntLockTimingFp;
extern const os_startreslocktimingfp_t		OS_startResLockTimingFp;
extern const os_stopreslocktimingfp_t		OS_stopResLockTimingFp;
extern const os_startisrexectimingfp_t		OS_startIsrExecTimingFp;
extern const os_preemptisrexectimingfp_t	OS_preemptIsrExecTimingFp;
extern const os_resumeisrexectimingfp_t		OS_resumeIsrExecTimingFp;
extern const os_stopisrexectimingfp_t		OS_stopIsrExecTimingFp;
extern const os_startisrintlocktimingfp_t	OS_startIsrIntLockTimingFp;
extern const os_stopisrintlocktimingfp_t	OS_stopIsrIntLockTimingFp;

#endif /* (!defined(OS_USE_OPTIMIZATION_OPTIONS)) || (!defined(OS_EXCLUDE_TIMINGPROTECTION)) */

#endif /* OS_ASM */

#define OS_UNUSEDRESLOCKTIME	OS_MAXTIMERVALUE

#define OS_ACC_NONE		OS_U(0)
#define OS_ACC_BUDGET	OS_U(1)
#define OS_ACC_RESLOCK	OS_U(2)
#define OS_ACC_INTLOCK	OS_U(3)

#ifndef OS_DoTimingProtection
#define OS_DoTimingProtection(f)	f
#endif

#define OS_STARTTASKEXECTIMING(t)		OS_DoTimingProtection((*OS_startTaskExecTimingFp)(t))
#define OS_STOPTASKEXECTIMING(t)		OS_DoTimingProtection((*OS_stopTaskExecTimingFp)(t))
#define OS_PREEMPTTASKEXECTIMING(t)		OS_DoTimingProtection((*OS_preemptTaskExecTimingFp)(t))
#define OS_STARTTASKINTLOCKTIMING(t,l)	OS_DoTimingProtection((*OS_startTaskIntLockTimingFp)((t),(l)))
#define OS_STOPTASKINTLOCKTIMING(t,l)	OS_DoTimingProtection((*OS_stopTaskIntLockTimingFp)((t),(l)))
#define OS_STARTISRINTLOCKTIMING(i,l)	OS_DoTimingProtection((*OS_startIsrIntLockTimingFp)((i),(l)))
#define OS_STOPISRINTLOCKTIMING(i,l)	OS_DoTimingProtection((*OS_stopIsrIntLockTimingFp)((i),(l)))
#define OS_STARTRESLOCKTIMING(r,l)		OS_DoTimingProtection((*OS_startResLockTimingFp)((r),(l)))
#define OS_STOPRESLOCKTIMING(r)			OS_DoTimingProtection((*OS_stopResLockTimingFp)(r))
#define OS_STARTISREXECTIMING(budget)	OS_DoTimingProtection((*OS_startIsrExecTimingFp)(budget))
#define OS_PREEMPTISREXECTIMING(save)	OS_DoTimingProtection((*OS_preemptIsrExecTimingFp)(save))
#define OS_RESUMEISREXECTIMING(save)	OS_DoTimingProtection((*OS_resumeIsrExecTimingFp)(save))
#define OS_STOPISREXECTIMING(i)			OS_DoTimingProtection((*OS_stopIsrExecTimingFp)(i))

#define OS_SHUTDOWNEXECTIMING()			OS_DoTimingProtection( \
	do {										\
		if ( OS_executionTimer[OS_GetMyCoreId()] != OS_NULL )		\
		{										\
			OS_ResetExecTimingInterrupt();		\
		}										\
	} while(0)	)

#define OS_CAT2PREEMPTEXECTIMING(save)	OS_DoTimingProtection( \
	do {\
		os_accounting_t * const acc = &OS_GetKernelData()->accounting;	\
		(save)->inFunction = OS_INNOTHING;					\
		if ( acc->inFunction == OS_INTASK )					\
		{													\
			OS_PREEMPTTASKEXECTIMING(OS_GetKernelData()->taskCurrent);	\
			(save)->inFunction = acc->inFunction;			\
		}													\
		else												\
		if ( acc->inFunction == OS_INCAT2 )					\
		{													\
			OS_PREEMPTISREXECTIMING(save);					\
		}													\
		else												\
		{													\
			/* MISRA *sigh* */								\
		}													\
	} while(0)												\
)

#define OS_CAT2RESUMEEXECTIMING(save)	OS_DoTimingProtection( \
	do {													\
		if ( (save)->inFunction == OS_INTASK )				\
		{																		\
			if ( (OS_GetKernelData()->taskCurrent != OS_NULL) &&									\
				 (OS_GetKernelData()->taskCurrent->dynamic->state > OS_TS_MAX_TERMINATING) )		\
			{																	\
				OS_STARTTASKEXECTIMING(OS_GetKernelData()->taskCurrent);							\
			}																	\
		}																		\
		else												\
		if ( (save)->inFunction == OS_INCAT2 )				\
		{													\
			OS_RESUMEISREXECTIMING(save);					\
		}													\
		else												\
		{													\
			OS_GetKernelData()->accounting.inFunction = OS_INNOTHING;		\
		}													\
	} while(0)												\
)

#define OS_CAT2PREEMPT_DECL(save)		OS_DoTimingProtection(os_accountingsave_t save;)

#ifndef OS_RATEMONITORCHECK
#define OS_RATEMONITORCHECK(rm)		(((rm) != OS_NULLRATEMONITOR) && ((*OS_rateMonitorFunc)(rm)))
#endif

#ifndef OS_DoMemoryProtection
#if (OS_HASMEMPROT)
#define OS_DoMemoryProtection(x)	(x)
#else
#define OS_DoMemoryProtection(x)
#endif
#endif

#ifndef OS_SETPROTECTION
#define OS_SETPROTECTION(task)					OS_DoMemoryProtection(OS_SetProtection(task))
#endif
#ifndef OS_SETISRPROTECTION
#define OS_SETISRPROTECTION(isr, id)			OS_DoMemoryProtection(OS_SetIsrProtection(isr, id))
#endif
#ifndef OS_SETHOOKPROTECTION
#define OS_SETHOOKPROTECTION(app,ctxt,stklen)	OS_DoMemoryProtection(OS_SetHookProtection(app, ctxt, stklen))
#endif

#if (!defined(OS_DoQueryIsr)) && (!defined(OS_DoMarkIsr))
#define OS_DoQueryIsr(x)			(x)
#define OS_DoMarkIsr(x)				do { x; } while(0)
#endif

#if !defined(OS_QueryRestoreProtection)
#define OS_QueryRestoreProtection(isr)	\
	OS_DoQueryIsr((!OS_AppIsNull((isr)->app)) && (((isr)->app->flags & OS_APP_TRUSTED) == 0U))
#endif

#define OS_KillNextIsr()			OS_DoQueryIsr( (OS_GetKernelData()->inFunction == OS_INCAT2) && \
											   OS_IsIsrKilled(OS_isrTableBase[OS_GetKernelData()->isrCurrent].dynamic) )
#define OS_IsIsrKilled(id)			OS_DoQueryIsr(( ((id)->statusflags & OS_ISRF_KILLED) != 0U ))
#define OS_MarkIsrKilled(id)		OS_DoMarkIsr((id)->statusflags |= OS_ISRF_KILLED)
#define OS_MarkIsrNotKilled(id)		OS_DoMarkIsr((id)->statusflags &= (os_uint8_t) ~OS_ISRF_KILLED)

#ifndef OS_CallIsrDirectly
#define OS_CallIsrDirectly()		((OS_configMode & OS_CALLISRDIRECT) != 0U)
#endif

#ifndef OS_CallAppSHookDirectly
#define OS_CallAppSHookDirectly()	((OS_configMode & OS_CALLAPPSHOOKDIRECT) != 0U)
#endif

#ifndef OS_CallAppEHookDirectly
#define OS_CallAppEHookDirectly()	((OS_configMode & OS_CALLAPPEHOOKDIRECT) != 0U)
#endif

#ifndef OS_LateEndOfInterrupt
#define OS_LateEndOfInterrupt(isr)	do { } while(0)
#endif

/*
 * OS_GetIsrStackBase(const os_isr_t *i)
 *
 * This macro returns the configured stack base for an ISR. The generic OS doesn't
 * define a stack base for ISRs, since their stacks are dynamically carved out of the kernel
 * stack.
 *
 * @return the stack base for ISR i. OS_NULL is returned if the ISR uses the
 *         global kernel/interrupt stack.
 * @param i pointer to the ISR structure to be queried.
 *
 */
#ifndef OS_GetIsrStackBase
#define OS_GetIsrStackBase(i)	OS_NULL
#endif

/*
 * OS_GetCurrentIsrSp(void)
 *
 * This macro returns the current stack pointer for the currently executed
 * ISR. Generally, ISRs run on the kernel stack, so the architecture
 * independent implementation simply returns the current stack pointer.
 * Architectures, that use different stack pointers for ISRs must override
 * this macro.
 *
 * Precondition: An ISR is currently executed.
 * @return current stack pointer referring to the stack of the ISR.
 */
#ifndef OS_GetCurrentIsrSp
#define OS_GetCurrentIsrSp()	OS_GetCurrentSp()
#endif

/*!
 * OS_GetKernelData()->inKernel, OS_GetKernelData()->inFunction, etc.
 *
 * These variables hold various aspects of the kernel's state:
 *   OS_GetKernelData()->inKernel - incremented on entering kernel, decremented on exit
 *   OS_GetKernelData()->inFunction - one of OS_INTASK, OS_INCAT1 etc.
  *  OS_GetKernelData()->isrCurrent - id of current ISR. Saved and set before calling ISR,
 *                       restored afterwards. OS_NULLISR if at task level.
 *   OS_GetKernelData()->isrLastLock - head of a LIFO queue of locks (i.e. resources, spinlocks) taken at ISR level
 *   OS_GetKernelData()->errorHookNesting - contains a bit for each type of error hook
 *                       (protection, error, application-error). Use to prevent
 *                       mutual recursion if errors occur inside hooks.
 */
#define OS_INBOOT			OS_U(0)		/* Initial state - MUST BE ZERO */
#define OS_INTASK			OS_U(1)		/* In a task */
#define OS_INCAT1			OS_U(2)		/* In category 1 ISR */
#define OS_INCAT2			OS_U(3)		/* In category 2 ISR */
#define OS_INACB			OS_U(4)		/* In alarm callback */
#define OS_INSHUTDOWN		OS_U(5)		/* Shut down */
#define OS_ININTERNAL		OS_U(6)		/* In some internal kernel function */

#define OS_INHOOKMIN		OS_U(7)		/* Hook functions lower limit */
#define OS_INSTARTUPHOOK	OS_U(7)		/* In startup hook */
#define OS_INSHUTDOWNHOOK	OS_U(8)		/* In shutdown hook */
#define OS_INERRORHOOK		OS_U(9)		/* In error hook */
#define OS_INPRETASKHOOK	OS_U(10)	/* In pre-task hook */
#define OS_INPOSTTASKHOOK	OS_U(11)	/* In post-task hook */
#define OS_INPREISRHOOK		OS_U(12)	/* In pre-isr hook */
#define OS_INPOSTISRHOOK	OS_U(13)	/* In post-isr hook */
#define OS_INPROTECTIONHOOK	OS_U(14)	/* In protection hook */
#define OS_INHOOKMAX		OS_U(14)	/* Hook functions upper limit */

#ifndef OS_ASM
/* Needs changing if ever INHOOKMAX > 15 */
typedef os_uint16_t	os_callermask_t;
#endif

#define OS_INALLMASK			OS_U(0xffff)
#ifndef OS_ASM
#define OS_INFUNCMASK(n)		((os_callermask_t) ((os_callermask_t) 1U<<(n)))
#else
#define OS_INFUNCMASK(n)		(1<<(n))
#endif
#define OS_INBOOTMASK			OS_INFUNCMASK(OS_INBOOT)
#define OS_INTASKMASK			OS_INFUNCMASK(OS_INTASK)
#define OS_INCAT1MASK			OS_INFUNCMASK(OS_INCAT1)
#define OS_INCAT2MASK			OS_INFUNCMASK(OS_INCAT2)
#define OS_INACBMASK			OS_INFUNCMASK(OS_INACB)
#define OS_INSHUTDOWNMASK		OS_INFUNCMASK(OS_INSHUTDOWN)
#define OS_ININTERNALMASK		OS_INFUNCMASK(OS_ININTERNAL)
#define OS_INSTARTUPHOOKMASK	OS_INFUNCMASK(OS_INSTARTUPHOOK)
#define OS_INSHUTDOWNHOOKMASK	OS_INFUNCMASK(OS_INSHUTDOWNHOOK)
#define OS_INERRORHOOKMASK		OS_INFUNCMASK(OS_INERRORHOOK)
#define OS_INPRETASKHOOKMASK	OS_INFUNCMASK(OS_INPRETASKHOOK)
#define OS_INPOSTTASKHOOKMASK	OS_INFUNCMASK(OS_INPOSTTASKHOOK)
#define OS_INPREISRHOOKMASK		OS_INFUNCMASK(OS_INPREISRHOOK)
#define OS_INPOSTISRHOOKMASK	OS_INFUNCMASK(OS_INPOSTISRHOOK)
#define OS_INPROTECTIONHOOKMASK	OS_INFUNCMASK(OS_INPROTECTIONHOOK)

/* OS_GetKernelData()->inFunction must never take this value! */
#define OS_INNOTHING	OS_U(255)	/* Unknown (for accounting) */

/* Bits in OS_errorHookNesting
 */
#define OS_EHN_APP	OS_U(0x01)		/* Application error hook called */
#define OS_EHN_ERR	OS_U(0x02)		/* Main error hook called */
#define OS_EHN_PROT	OS_U(0x04)		/* Protection hook called */

#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)

#define OS_HOOK_SAVE_INKERNEL_DECL	os_uint8_t OS_hookSaveInKernel;
#define OS_HOOK_SAVE_INKERNEL()		do { OS_hookSaveInKernel = OS_GetKernelData()->inKernel; } while(0)
#define OS_HOOK_RESTORE_INKERNEL()	do { OS_GetKernelData()->inKernel = OS_hookSaveInKernel; } while(0)

#else

#define OS_HOOK_SAVE_INKERNEL_DECL
#define OS_HOOK_SAVE_INKERNEL()
#define OS_HOOK_RESTORE_INKERNEL()

#endif

#ifndef OS_IS_ACTION_HOOK
#define OS_IS_ACTION_HOOK(h)		((h) != 0)
#endif

#ifndef OS_IS_INCAT2
#define OS_IS_INCAT2(x)				((x) == OS_INCAT2)
#endif

#define OS_SUSPENDNESTLIMIT	OS_U(255)

/* os_fastsuspendresumestatus_t
 *
 * Structure for storing suspend/resume status for fast interrupt lock functions
 */
#ifndef OS_ASM

typedef struct os_fastsuspendresumestatus_s os_fastsuspendresumestatus_t;

/* Deviation MISRAC2012-2 */
struct os_fastsuspendresumestatus_s
{
	os_intstatus_t	allOldLevel;
	os_intstatus_t	osOldLevel;
	os_uint8_t		allNestCount;
	os_uint8_t		osNestCount;
};

#define OS_FASTSUSPENDRESUMESTATUS_INIT	\
	{									\
		0u,		/*allOldLevel*/			\
		0u,		/*osOldLevel*/			\
		0u,		/*allNestCount*/		\
		0u		/*osNestCount*/			\
	}

#endif

/*!
 * OS_hookFunctions
 *
 * For the generic kernel library a collection of 8 function pointers holds the addresses of the
 * configured hook functions, or OS_NULL if no function is configured.
 *
 * !LINKSTO Kernel.HookRoutines, 1
 */
#ifndef OS_CALLSTARTUPHOOK
#ifndef OS_ASM
extern const os_startuphook_t		OS_ptrStartupHook;
#endif
#define OS_CALLSTARTUPHOOK()		OS_CALLHOOKVOID_INDIRECT(OS_ptrStartupHook, OS_INSTARTUPHOOK)
#endif

#ifndef OS_CALLSHUTDOWNHOOK
#ifndef OS_ASM
extern const os_shutdownhook_t		OS_ptrShutdownHook;
#endif
#define OS_CALLSHUTDOWNHOOK(e)		OS_CALLHOOKPARAM_INDIRECT(OS_ptrShutdownHook, OS_INSHUTDOWNHOOK, (e))
#endif

#ifndef OS_CALLERRORHOOK
#ifndef OS_ASM
extern const os_errorhook_t			OS_ptrErrorHook;
#endif
#define OS_CALLERRORHOOK(e)			OS_CALLHOOKPARAM_INDIRECT(OS_ptrErrorHook, OS_INERRORHOOK, (e))
#endif

#ifndef OS_CALLPROTECTIONHOOK
#ifndef OS_ASM
extern const os_protectionhook_t	OS_ptrProtectionHook;
#endif
#define OS_CALLPROTECTIONHOOK(e,r)	OS_CALLHOOKPARAMRETURN_INDIRECT(OS_ptrProtectionHook, OS_INPROTECTIONHOOK, (e), (r))
#endif

#ifndef OS_CALLPRETASKHOOK
#ifndef OS_ASM
extern const os_pretaskhook_t		OS_ptrPreTaskHook;
#endif
#define OS_CALLPRETASKHOOK()		OS_CALLHOOKVOID_INDIRECT(OS_ptrPreTaskHook, OS_INPRETASKHOOK)
#endif

#ifndef OS_CALLPOSTTASKHOOK
#ifndef OS_ASM
extern const os_posttaskhook_t		OS_ptrPostTaskHook;
#endif
#define OS_CALLPOSTTASKHOOK()		OS_CALLHOOKVOID_INDIRECT(OS_ptrPostTaskHook, OS_INPOSTTASKHOOK)
#endif

#ifndef OS_CALLPREISRHOOK
#ifndef OS_ASM
extern const os_preisrhook_t		OS_ptrPreIsrHook;
#endif
#define OS_CALLPREISRHOOK(i)		OS_CALLHOOKPARAM_INDIRECT(OS_ptrPreIsrHook, OS_INPREISRHOOK, (i))
#endif

#ifndef OS_CALLPOSTISRHOOK
#ifndef OS_ASM
extern const os_postisrhook_t		OS_ptrPostIsrHook;
#endif
#define OS_CALLPOSTISRHOOK(i)		OS_CALLHOOKPARAM_INDIRECT(OS_ptrPostIsrHook, OS_INPOSTISRHOOK, (i))
#endif

#ifndef OS_IS_INSTARTUPHOOK
#define OS_IS_INSTARTUPHOOK(x)			((x) == OS_INSTARTUPHOOK)
#endif

#ifndef OS_IS_INERRORHOOK
#define OS_IS_INERRORHOOK(x)			((x) == OS_INERRORHOOK)
#endif

#ifndef OS_IS_INSHUTDOWNHOOK
#define OS_IS_INSHUTDOWNHOOK(x)			((x) == OS_INSHUTDOWNHOOK)
#endif

/* These macros call the appropriate error hook.
 * The _DIRECT versions are used by the optimized kernel.
 * The _INDIRECT versions are used by the generic library build.
 */
#define OS_CALLHOOKVOID_DIRECT(hf,h) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(hf)();														\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

#define OS_CALLHOOKPARAM_DIRECT(hf,h,p) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(hf)(p);													\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

#define OS_CALLHOOKPARAMRETURN_DIRECT(hf,h,p,r) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(r) = (hf)(p);												\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

#define OS_CALLHOOKVOID_INDIRECT(hp,h) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(*(hp))();													\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

#define OS_CALLHOOKPARAM_INDIRECT(hp,h,p) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(*(hp))(p);													\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

#define OS_CALLHOOKPARAMRETURN_INDIRECT(hp,h,p,r) \
	do {															\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();	\
		os_uint8_t Os_CallHookTmp = _kernel_data->inFunction;		\
		_kernel_data->inFunction = (h);								\
		OS_TRACE_HOOK_ENTRY(h);										\
		(r) = (*(hp))(p);											\
		OS_TRACE_HOOK_EXIT_P(h);									\
		_kernel_data->inFunction = Os_CallHookTmp;					\
	} while (0)

/* These macros call all the application startup/shutdown hooks. The macros can be
 * overridden for optimization
 * !LINKSTO Kernel.Autosar.OsApplication.CoreAssignment, 1
 */
#ifndef OS_CALLAPPSTARTUPHOOKS
#define OS_CALLAPPSTARTUPHOOKS() \
	do {													\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();\
		os_coreid_t const _myCoreId = OS_GetMyCoreId();		\
		os_unsigned_t casuhI;								\
		const os_appcontext_t *casuhApp;					\
															\
		casuhApp = OS_appTableBase;							\
		for ( casuhI = 0; casuhI < OS_nApps; casuhI++ )		\
		{													\
			if ((casuhApp->startupHook != OS_NULL)			\
				&& (OS_GetAppCoreId(casuhApp) == _myCoreId))\
			{												\
				_kernel_data->inFunction = OS_INSTARTUPHOOK;\
				OS_CallStartupHook(casuhApp);				\
				_kernel_data->inFunction = OS_ININTERNAL;	\
			}												\
			casuhApp++;										\
		}													\
	} while (0)
#endif

#ifndef OS_CALLAPPSHUTDOWNHOOKS
#define OS_CALLAPPSHUTDOWNHOOKS(ec) \
	do {													\
		os_kerneldata_t * const _kernel_data = OS_GetKernelData();\
		os_coreid_t const _myCoreId = OS_GetMyCoreId();		\
		os_unsigned_t casdhI;								\
		const os_appcontext_t *casdhApp;					\
															\
		casdhApp = OS_appTableBase;							\
		for ( casdhI = 0; casdhI < OS_nApps; casdhI++ )		\
		{													\
			if ((casdhApp->shutdownHook != OS_NULL)			\
				&& (OS_GetAppCoreId(casdhApp) == _myCoreId))\
			{												\
				_kernel_data->inFunction = OS_INSHUTDOWNHOOK;\
				OS_CallShutdownHook(casdhApp, (ec));		\
				_kernel_data->inFunction = OS_ININTERNAL;	\
			}												\
			casdhApp++;										\
		}													\
	} while (0)
#endif

/*!
 * OS_ERROR, OS_ERRORACTION
 *
 * These macros provide error handling.
 *
 * OS_ERROR() is called whenever an error is detected. It is responsible for taking the
 * action appropriate for the error and returning the correct OSEK-style error code.
 *
 * The macros OS_SID and OS_SIF must be defined before using this macro, usually
 * at the top of the file. They must be the correct SID and service info structure
 * for the service, eg OS_SID_ActivateTask/OS_svc_ActivateTask.
 *
 * If there's an error wrapper function (some architectures need this to avoid using
 * oodles of space on a task's stack) it will be called. Otherwise the error handler
 * is called directly.
 *
 * OS_ERRORACTION() performs the action dictated by the error, as modified by the various
 * hook functions. Normally it simply calls OS_ErrorAction()
 *
 * In the optimized kernel, both macros can be given reduced functionality. See Os_optimize.h
 *
 * OS_ERROR_NOCHECK() is used when OS_ERROR() must be called with an arbitrary variable. It is used
 * merely to hide the call from the error-db generator/checker, which cannot handle arbitrary variables.
 *
 * OS_ERRORCODE_CHECK() is used by functions that return error codes without reporting the error directly.
 * It simply evaluates to the error code given as parameter.
 *
 * OS_ERRORCODE_NOCHECK() is the same as OS_ERRORCODE_CHECK(), but isn't checked by the error-db generator.
 * It is used mainly for OS_ERROR_NoError.
 */
#if !defined(OS_ERROR)
#if defined(OS_ERRORWRAPPER)
#define OS_ERROR(e,p) OS_ERRORWRAPPER(&OS_SIF, (e), (p))
#else
#define OS_ERROR(e,p) OS_Error(&OS_SIF, (e), (p))
#endif
#endif

#if !defined(OS_ERRORACTION)
#define OS_ERRORACTION(act,res)	OS_ErrorAction(act,res)
#endif

#ifndef OS_ERROR_NOCHECK
#define OS_ERROR_NOCHECK(e, p)	OS_ERROR((os_error_t)(e), (p))
#endif

#ifndef OS_ERRORCODE_CHECK
#define OS_ERRORCODE_CHECK(e)	(e)
#endif

#ifndef OS_ERRORCODE_NOCHECK
#define OS_ERRORCODE_NOCHECK(e)	(e)
#endif

/*
 * OS_ErrorHandlingForVoidApi()
 *
 * This macro determines if error handling for APIs that don't return StatusType is included.
 * In the generic library it tests a constant flag.
 * In the optimized library it is either true or false depending on the configuration mode
 *
 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.ErrorHook.Call.OnlyIfErrorCodeReturned, 1
 */
#ifndef OS_ErrorHandlingForVoidApi
#define OS_ErrorHandlingForVoidApi()	((OS_configMode & OS_VOIDERROR) != 0)
#endif

/*!
 * OS_SHUTDOWN
 *
 * This macro is a shortcut for calling OS_Shutdown.
 *
 * If there's a shutdown wrapper function (some architectures need this to switch to the
 * kernel stack when shutting down from a task) it will be called. Otherwise OS_Shutdown
 * is called directly.
 */
#ifdef OS_SHUTDOWNWRAPPER
#define OS_SHUTDOWN(error,sync)	OS_SHUTDOWNWRAPPER((error), (sync))
#else
#define OS_SHUTDOWN(error,sync)	OS_Shutdown((error), (sync))
#endif

/*!
 * OS_FreeTaskContext
 *
 * This macro frees up any allocated context that the task uses while
 * active. If the architecture does not define this macro, the generic
 * kernel assumes it to be "no action".
 */
#ifndef OS_FreeTaskContext
#define OS_FreeTaskContext(t)
#endif

/*!
 * OS_CallingContextCheck()
 *
 * Returns TRUE (a non-zero value) if the service (identified by the
 * macro OS_SID) is permitted to be called from the calling context.
 *
 * The array pointed to by OS_permittedContext contains a 16-bit word
 * for each system service. Each word contains a bit for each possible calling
 * context (1<<OS_INxxx). The array can be selected by the configurator
 * to allow the rules to be relaxed. The reentrant nature of the
 * kernel safely permits the calling of some system services from contexts
 * that Autosar forbids.
 *
 * If the calling context is the error hook, the context that caused
 * the error must also allow the system service.
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context, 1
 * Each API function that calls this macro must provide a OS_SID macro to define the context.
 *
 * CHECK: SAVE
 * CHECK: RULE 306 OFF (There's really no other way)
 */
#ifndef OS_CallingContextCheck
#define OS_CallingContextCheck()																			\
  (																											\
    (os_boolean_t)( ( OS_GetKernelData()->inFunction == OS_INERRORHOOK )									\
      ? (((OS_permittedContext[OS_SID] & OS_INERRORHOOKMASK) != 0U) &&										\
         ((OS_permittedContext[OS_SID] & OS_INFUNCMASK(OS_GetKernelData()->errorStatus.calledFrom)) != 0U))	\
      : ((OS_permittedContext[OS_SID] & OS_INFUNCMASK(OS_GetKernelData()->inFunction)) != 0U)				\
    ) != OS_FALSE																							\
  )
#endif

#ifndef OS_ASM
extern const os_callermask_t * const OS_permittedContext;
/* Deviation MISRAC2012-1, MISRAC2012-3 <+2> */
extern const os_callermask_t OS_permittedContextOsek[];
extern const os_callermask_t OS_permittedContextAutosar[];
#endif

/* CHECK: RESTORE */

/*!
 * OS_IntRestoreHardLock
 *
 * Restores the interrupt lock flag of the CPU if kernel locking is done by hard interrupt locking.
 * The default implementation is for systems where kernel locking is done by interrupt level locking.
 * For level locking kernels nothing has to be done, because the interrupt level has already been set/restored.
 *
 */
#ifndef OS_IntRestoreHardLock
#define OS_IntRestoreHardLock(p)
#endif

/*!
 * OS_InterruptEnableCheck
 *
 * Checks to see if interrupts are enabled (nesting counts == 0)
 *
 * For use in system services that are not permitted to be called with
 * interrupts disabled.
 *
 * When using InterruptEnableCheck:
 *  use parameter OS_IEC_OSEK if the check should always be made for
 *			correct kernel operation (eg GetResource, ReleaseResource)
 *  use parameter OS_IEC_OSEKEXTRA for compatibility with PO4, when a
 *			task switch is possible
 *  use parameter OS_IEC_AUTOSAR when the check is only necessary for
 *			full Autosar compliance (where task switch isn't possible, eg
 *			GetTaskID)
 *
 * In practice, OS_IEC_OSEK should never be passed as parameter to
 * OS_InterruptEnableCheck() to avoid a compiler warning. Use
 * OS_InterruptEnableCheckAlways() instead.
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.Miscellaneous.InterruptsDisabled, 2
 */
#define OS_IEC_OSEK			OS_U(0)	/* Safe, only what's really necessary */
#define OS_IEC_OSEKEXTRA	OS_U(1)	/* Safe, ProOSEK 4 ExtraCheck compatible */
#define OS_IEC_AUTOSAR		OS_U(2)	/* Strict Autosar checking */

#define OS_InterruptEnableCheckAlways()													\
	(																					\
		(os_boolean_t)(	(OS_GetKernelData()->inFunction == OS_INTASK)					\
			?	(	(OS_GetKernelData()->taskCurrent->dynamic->nestSuspendOs == 0U)		\
				 && (OS_GetKernelData()->taskCurrent->dynamic->nestSuspendAll == 0U)	\
				)																		\
			:	(	(OS_GetKernelData()->nestSuspendOs == 0U) 							\
				 && (OS_GetKernelData()->nestSuspendAll == 0U) 							\
				)																		\
		) != OS_FALSE																	\
	)

#define OS_InterruptEnableCheckAlwaysFromTask(td)	( ((td)->nestSuspendOs == 0U) && ((td)->nestSuspendAll == 0U) )

#if (!defined(OS_InterruptEnableCheck)) && (!defined(OS_InterruptEnableCheckFromTask))

#define OS_InterruptEnableCheck(m)				( (OS_iecMode < (m)) || OS_InterruptEnableCheckAlways() )
#define OS_InterruptEnableCheckFromTask(m,td)	( (OS_iecMode < (m)) || OS_InterruptEnableCheckAlwaysFromTask(td) )

#endif

#ifndef OS_ASM
extern const os_uint8_t OS_iecMode;
#endif

/*!
 * OS_IsValidTaskState()		- Checks to see if taskstate id valid
 * OS_IsValidTaskId()			- Checks to see if TaskId id valid
 * OS_IsValidResourceId()		- Checks to see if Resource id valid
 *
 * These macros are defined as a constant (disable the checking) in STANDARD mode.
 * Otherwise they compare the parameter against its valid range.
 */
#define OS_IsValidTaskState(s)		OS_DoExtendedStatusTrue((s) < OS_TS_NSTATES)
#define OS_IsValidTaskId(t)			OS_DoExtendedStatusTrue((t) < OS_nTasks)
#define OS_IsValidResourceId(r)		OS_DoExtendedStatusTrue((r) < OS_nResources)
#define OS_IsCalledFromTask()		OS_DoExtendedStatusTrue(OS_GetKernelData()->inFunction == OS_INTASK)
#define OS_TaskOccupiesResource(td)	OS_DoExtendedStatusFalse((td)->lastLock != OS_NULLOBJECT)
#define OS_IsrOccupiesResource()	OS_DoExtendedStatusFalse(OS_GetKernelData()->isrLastLock != OS_NULLOBJECT)
#define OS_IsrSuspendNestingError(os, all) 						\
	OS_DoExtendedStatusFalse( 									\
			((os) != OS_GetKernelData()->nestSuspendOs) 		\
			|| ((all) != OS_GetKernelData()->nestSuspendAll))

#define OS_SaveIsrLastLock(old)			OS_DoExtendedStatusVoid( 	\
	do {															\
		os_kerneldata_t * const kd = OS_GetKernelData();			\
		*(old) = kd->isrLastLock;									\
		kd->isrLastLock = OS_NULLOBJECT;							\
	} while(0)														\
)

#define OS_SaveIsrNestSuspend(os, all)	OS_DoExtendedStatusVoid( 	\
	do {															\
		os_kerneldata_t * const kd = OS_GetKernelData();			\
		*(os) = kd->nestSuspendOs;									\
		*(all) = kd->nestSuspendAll;								\
	} while(0)														\
)

#define OS_RestoreIsrLastLock(save)		OS_DoExtendedStatusVoid( 	\
	do {															\
		OS_GetKernelData()->isrLastLock = (save);					\
	} while(0)														\
)

#define OS_RestoreNestSuspend(os, all)	OS_DoExtendedStatusVoid( 	\
	do {															\
		os_kerneldata_t * const kd = OS_GetKernelData();			\
		kd->nestSuspendOs = (os);									\
		kd->nestSuspendAll = (all);									\
	} while(0)														\
)

#define OS_SAVEISRNESTSUSPEND_DECL(os, all)	OS_DoExtendedStatusVoid(os_uint8_t os; os_uint8_t all;)
#define OS_SAVEISRLASTLOCK_DECL(last)		OS_DoExtendedStatusVoid(os_objectid_t last;)


/*!
 * OS_InitExecTimingTimer()
 * OS_SetExecTimingInterrupt()
 * OS_ResetExecTimingInterrupt()
 * OS_ReadExecTimer()
 * OS_ExecTimerSub()
 *
 * The default implementations of the execution-time monitor drivers simply use the configured hardware timer.
 * OS_InitExecTimingTimer() does nothing, because the timer gets initialised along with all other
 * hw-timers. The other functions simply call the matching driver function with appropriate parameters,
 * with the exception of OS_SetExecTimingInterrupt(), which uses OS_CtrStart() macro
 * to provide intermediate interrupts if the remaining time is greater than the timer's range.
 *
 * The macros do not get defined if OS_InitExecTimingTimeris already defined by the architecture.
 */
#ifndef OS_InitExecTimingTimer
#ifndef OS_ASM
extern const os_hwt_t * const OS_executionTimer[OS_N_CORES_MAX];
#endif

#define OS_InitExecTimer()					do { } while(0)
#define OS_ReadExecTimer()					OS_HwtRead(OS_executionTimer[OS_GetMyCoreId()])
#define OS_ExecTimerSub(x,y)				OS_HwtSub(OS_executionTimer[OS_GetMyCoreId()], (x), (y))
#define OS_ResetExecTimingInterrupt()		OS_HwtStop(OS_executionTimer[OS_GetMyCoreId()])

#define OS_SetExecTimingInterrupt(frc,tim)	OS_CtrStart(OS_executionTimer[OS_GetMyCoreId()], (frc), (tim))

#endif /* OS_InitExecTimingTimer */

/*!
 * OS_rateMonitor etc.
 *
 * Each rate monitor is a ROM structure containing the time limit and
 * count limit for the object being monitored, and a pointer to the
 * ring buffer of stored times.
 * For each rate monitor there is an index in RAM which indexes the
 * next slot in the ring buffer.
 *
 * Each object (task/ISR) whose rate is to be monitored has the id of a
 * rate monitor in its data structure. There is a "null" value of the
 * id to permit no rate monitor to be indicated.
 */
#ifndef OS_ASM

typedef os_uint8_t os_rateindex_t;
typedef struct os_ratemonitor_s os_ratemonitor_t;
typedef os_boolean_t (*os_ratemonitorfunc_t)(os_ratemonitorid_t);

/* Deviation MISRAC2012-2 */
struct os_ratemonitor_s
{
	os_timestamp_t *ringBuffer;
	os_tick_t timeLimit;
	os_rateindex_t countLimit;
};

/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern const os_ratemonitor_t OS_rateMonitor[];

#if (OS_N_CORES == 1)

/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern os_rateindex_t OS_rateIndex[];
#define OS_GetRateIndex(rmid)	(&OS_rateIndex[(rmid)])

#else

/* Deviation MISRAC2012-1, MISRAC2012-3 <1> */
extern os_rateindex_t * const OS_rateIndex_ptr[];
#define OS_GetRateIndex(rmid)	(OS_rateIndex_ptr[(rmid)])

#endif /* OS_N_CORES == 1 */

extern const os_ratemonitorfunc_t OS_rateMonitorFunc;
extern const os_ratemonitorid_t OS_nRateMonitors;

os_boolean_t OS_RateMonitor(os_ratemonitorid_t);

#endif /* OS_ASM */

/*!
 * OS startup mode.
 */
#define OS_GetAppMode()	(OS_appMode)
#ifndef OS_ASM
/* CHECK: NOPARSE */
extern os_uint8_t
	OS_SECTION_ATTRIB_PRE_DECL(.data.shared.os.OS_appMode)
	OS_appMode
	OS_SECTION_ATTRIB_POST_DECL(.data.shared.os.OS_appMode);
/* CHECK: PARSE */
#endif

/*! OS_CanWrite, OS_MisalignedData
 *
 * OS_CanWrite() is a generic function that checks if the caller can write to the
 * specified location. The check is twofold:
 * 1 - is the caller permitted to write to this region?
 * 2 - can the processor physically perform the operation (alignment etc.)
 *
 * The default behaviour can be overridden by the architecture:
 * - The generic test can be modified or even excluded by defining OS_ArchCanWrite()
 * - An alignement test can be enforced by implementing OS_ArchMisalignedData()
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.Address, 2
 * !LINKSTO Kernel.Autosar.ServiceErrors.Address.Alignment, 1
 */
#ifndef OS_ArchCanWrite
#define OS_ArchCanWrite(b,l) OS_CanWrite(b,l)
#endif

#ifdef OS_ArchMisalignedData
#define OS_MisalignedData(b, l)		OS_ArchMisalignedData(b, l)
#else
#define OS_MisalignedData(b, l)		OS_FALSE
#endif

/*
 * OS_SetIsrinKernel(void)
 *
 * This macro sets the OS_GetKernelData()->inKernel flag to 1 for an ISR entry function.
 */
#if OS_USE_IRQ_ATOMIC_INKERNEL==1
/* handled in vector entry already */
#define OS_SetIsrinKernel()		do {} while (0)
#else
#define OS_SetIsrinKernel()		do { OS_GetKernelData()->inKernel = 1; } while (0)
#endif /* OS_USE_IRQ_ATOMIC_INKERNEL==1 */

/*
 * OS_RestoreIsrinKernel(uint8_t oldinKernel)
 *
 * This macro restores the OS_GetKernelData()->inKernel flag from the value oldinKernel for
 * ISR entry functions.
 */
#if OS_USE_IRQ_ATOMIC_INKERNEL==1
/* handled by exit wrapper */
#define OS_RestoreIsrinKernel(oldinKernel)	do {} while (0)
#else
#define OS_RestoreIsrinKernel(oldinKernel)	do { OS_GetKernelData()->inKernel = (oldinKernel); } while (0)
#endif /* OS_USE_IRQ_ATOMIC_INKERNEL==1 */

/*!
 * os_intstatus_t OS_IOC_LOCKDATA(uint32_t id);
 *
 * Take a lock for ioc data structure with given id and return the old
 * locking state.
 *
 * @param id identifiction number of IOC data structure member.
 * @return old locking state.
 */
#define OS_IOC_LOCKDATA(id)	OS_IntDisable()



/*!
 * void OS_IOC_LOCKDATA(uint32_t id, os_intstatus_t is);
 *
 * Free the lock for ioc data structure with given id, restoring the old
 * locking state is.
 *
 * @param id identifiction number of IOC data structure member.
 * @param is old locking state.
 */
#define OS_IOC_UNLOCKDATA(id, is)	\
	do {							\
		OS_IntRestore(is);			\
	} while (0)

/*!
 * OS_xxxFunc
 *
 * These are constant pointers to various tables of functions
 * and will be either OS_NULL or the address of the (constant) function.
 *
 * The pointers are used in functions like CheckObjectOwnership(), to
 * avoid linker errors when there are no objects of the given type.
 */
#ifndef OS_ASM
/* CHECK: SAVE
 * CHECK: RULE 306 OFF
 */
typedef void (*os_actitaskfunc_t)(const os_task_t *);
typedef os_result_t (*os_killtaskfunc_t)(const os_task_t *, os_taskstate_t);
typedef os_result_t (*os_killisrfunc_t)(os_isrid_t);
typedef os_result_t (*os_killhookfunc_t)(os_hookcontext_t *);
/* CHECK: RESTORE
 */

extern const os_actitaskfunc_t OS_actiTaskFunc;
extern const os_killtaskfunc_t OS_killTaskFunc;
extern const os_killisrfunc_t OS_killIsrFunc;
extern const os_killhookfunc_t OS_killHookFunc;
#endif /* OS_ASM */

/*
 * Function prototypes etc.
 */
#ifndef OS_ASM

/* Kernel startup functions.
 */
extern void OS_InitKernStack(void);
extern void OS_InitApp(void);
extern void OS_InitApplicationData(void);
extern void OS_InitIsrStacks(void);
extern void OS_InitTaskStacks(void);
extern void OS_InitTasks(void);
extern void OS_InitResources(void);
extern void OS_InitInterrupts(void);
extern void OS_InitRateMonitor(void);
extern void OS_EnterProtectedMode(void);

/* Dummy hook functions (in addition to OS_NullFunction() above)
 */
extern void OS_NullErrorHook(os_result_t);
extern void OS_NullIsrHook(os_isrid_t);
extern os_erroraction_t OS_NullProtectionHook(os_result_t);
extern os_erroraction_t OS_EbProtectionHook(os_result_t);

/* Internal kernel functions
 */
extern void OS_Dequeue(os_tasklink_t);
extern void OS_EnqueueLink(os_tasklink_t, os_prio_t);
extern void OS_RequeueUp(os_tasklink_t, os_prio_t);
extern void OS_RequeueDown(os_tasklink_t, os_prio_t);
extern void OS_DequeueAll(const os_task_t *);
extern os_result_t OS_Error(const os_serviceinfo_t * const, os_error_t, os_paramtype_t *);
extern os_result_t OS_ErrorAction(os_erroraction_t, os_result_t);
extern os_result_t OS_ErrorMinimal(os_result_t);
extern os_result_t OS_ErrorMinimalParam(os_serviceid_t, os_result_t, os_paramtype_t *);
extern void OS_ActiTask(const os_task_t *);
extern os_result_t OS_KillTask(const os_task_t *, os_taskstate_t);
extern os_result_t OS_KillIsr(os_isrid_t);
extern os_result_t OS_KillHook(os_hookcontext_t *);
extern void OS_Shutdown(os_result_t, os_boolean_t);
extern void OS_ShutdownNoHooks(void);
extern void OS_CallStartupHook(const os_appcontext_t *);
extern void OS_CallShutdownHook(const os_appcontext_t *, os_result_t);
extern void OS_CallErrorHook(const os_appcontext_t *, os_result_t);
extern os_result_t OS_GetResourceFromTask(os_resourceid_t, os_resourcedynamic_t *, os_paramtype_t *);
extern os_result_t OS_GetResourceFromIsr(os_resourceid_t, os_resourcedynamic_t *, os_paramtype_t *);
extern void OS_ReleaseLocks(os_objectid_t);

extern void OS_RaiseTaskPriority(os_taskdynamic_t *, os_prio_t);
extern void OS_LowerCurrentTasksPriority(os_prio_t);

extern os_errorresult_t OS_LocalActivateTask(const os_task_t *);
extern os_errorresult_t OS_LocalSetEvent(const os_task_t *, os_eventmask_t);
extern os_errorresult_t OS_LocalGetEvent(const os_task_t *, os_eventmask_t *);
extern void OS_DoTerminateTask(const os_task_t *, os_taskdynamic_t *);

#if (OS_N_CORES != 1)
/* Helper for OS_Shutdown: Releases the spinlocks of the current core. */
extern void OS_ReleaseSpinlocksCurCore(void);
#endif /* OS_N_CORES */

#if (OS_N_CORES == 1)
#define OS_DoActivateTask       OS_LocalActivateTask
#define OS_DoSetEvent(t,ts,ev)  OS_LocalSetEvent((ts),(ev))
#define OS_DoGetEvent(t,ts,ep)  OS_LocalGetEvent((ts),(ep))
#else /* OS_N_CORES */
#define OS_DoActivateTask       OS_MultiActivateTask
#define OS_DoSetEvent(t,ts,ev)  OS_MultiSetEvent((t),(ts),(ev))
#define OS_DoGetEvent(t,ts,ep)  OS_MultiGetEvent((t),(ts),(ep))
extern os_errorresult_t OS_MultiActivateTask(const os_task_t *);
extern os_errorresult_t OS_MultiSetEvent(os_taskid_t, const os_task_t *, os_eventmask_t);
extern os_errorresult_t OS_MultiGetEvent(os_taskid_t, const os_task_t *, os_eventmask_t *);
#endif /* OS_N_CORES */

/* CacheInvalidate - invalidates the cache for the given memory area.
 * CacheFlush - flushes the cache for the given memory area.
 *
 * These operations are not necessarily function calls, so the compiler could optimize
 * across them. Therefore the affected memory areas must be qualified as volatile.
*/
#if (OS_CACHE_MODE == OS_CACHE_MODE_NONE)
#define OS_CacheInvalidate(addr, size) OS_CompilerMemBarrier(addr)
#define OS_CacheFlush(addr, size) OS_CompilerMemBarrier(addr)
#elif (OS_ARCH == OS_PA)
/* PA has a configuration option to disable cache management */
#define OS_CacheInvalidate(addr, size)	do {	\
		if (OS_cacheManagementDisabled != 0)	\
		{										\
			OS_CompilerMemBarrier(addr);		\
		}										\
		else									\
		{										\
			OS_CacheInvalidateFn(addr, size);	\
		}										\
	} while(0)
#define OS_CacheFlush(addr, size)	do {	\
		if (OS_cacheManagementDisabled != 0)\
		{									\
			OS_CompilerMemBarrier(addr);	\
		}									\
		else								\
		{									\
			OS_CacheFlushFn(addr, size);	\
		}									\
	} while(0)
#else
#define OS_CacheInvalidate(addr, size) OS_CacheInvalidateFn(addr, size)
#define OS_CacheFlush(addr, size) OS_CacheFlushFn(addr, size)
#endif
extern void OS_CacheInvalidateFn(volatile void *, os_size_t);
extern void OS_CacheFlushFn(volatile void *, os_size_t);

extern os_uint8_t OS_Cat1Entry(os_isrid_t);
extern os_uint8_t OS_Cat2Entry(os_isrid_t);
extern os_uint8_t OS_CatKEntry(os_isrid_t);
extern void OS_Dispatch(os_taskstate_t);

extern os_unsigned_t OS_CheckForDispatch(os_uint8_t);
#endif /* OS_ASM */

/* returns from CheckForDispatch */
#define OS_NO_DISP				OS_U(0)
#define OS_DISP_WITHOUT_SAVE	OS_U(1)
#define OS_DISP_WITH_SAVE		OS_U(2)

#ifndef OS_ASM
extern os_uint8_t OS_UnknownInterrupt(os_isrid_t iid);
extern void OS_CallNontrustedIsr(const os_isr_t *, os_isrdynamic_t *);
extern void OS_CallAppHook(const os_appcontext_t *, os_savedcontext_t *, void *, os_stacklen_t, os_unsigned_t);
extern void OS_CallHook(const os_appcontext_t *, void *, os_size_t, os_savedcontext_t *, os_unsigned_t);
extern os_result_t OS_KillCalledContext(os_savedcontext_t *);

extern void OS_TriggerInterrupt(os_isrid_t);

/* Memory protection functions
 */
extern os_boolean_t OS_CanWrite(const void *, os_size_t);
extern void OS_SetProtection(const os_task_t *);
extern void OS_SetIsrProtection(const os_isr_t *, os_isrdynamic_t *);

/* os_sethookprot_arg2t: Type of 2nd argument to OS_SetHookProtection.
 * Can be overriden the in the arch-specific files. Define
 * OS_SETHOOKPROT_HAVE_ARCH_ARG2T to 1 in this case.
*/
#ifndef OS_SETHOOKPROT_HAVE_ARCH_ARG2T
typedef os_savedcontext_t *    os_sethookprot_arg2t;
#endif
void OS_SetHookProtection(const os_appcontext_t *, os_sethookprot_arg2t, os_size_t);

/* Timing protection functions
 */
extern void OS_InitExecutionTiming(void);
extern void OS_StartTaskExecTiming(const os_task_t *);
extern void OS_PreemptTaskExecTiming(const os_task_t *);
extern void OS_StopTaskExecTiming(const os_task_t *);
extern void OS_StartTaskIntLockTiming(const os_task_t *, os_intlocktype_t);
extern void OS_StopTaskIntLockTiming(const os_task_t *, os_intlocktype_t);
extern void OS_StartIsrExecTiming(os_tick_t);
extern void OS_PreemptIsrExecTiming(os_accountingsave_t *);
extern void OS_ResumeIsrExecTiming(os_accountingsave_t *);
extern void OS_StopIsrExecTiming(const os_isr_t *);
extern void OS_StartIsrIntLockTiming(os_isrid_t, os_intlocktype_t);
extern void OS_StopIsrIntLockTiming(os_isrid_t, os_intlocktype_t);
extern void OS_StartResLockTiming(os_resourcedynamic_t *, os_tick_t);
extern void OS_StopResLockTiming(os_resourcedynamic_t *);

extern void OS_ExceedExecTime(void);
extern void OS_ExecTimerInterrupt(void);
extern void OS_ExecTimerTimestampInterrupt(void);
extern os_tick_t OS_GetTimeUsed(void);

/* "Optional" prototypes for optimized code
 */
#ifdef	OS_PROTO_ENQUEUE
extern void OS_Enqueue(const os_task_t *);
#endif

/* OS_CountLeadingZeros(), for those architectures that need it
 */
extern os_int_t OS_CountLeadingZeros(os_clzword_t word);

/* handling of task return address for those architectures that need it
 */
#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)
#define OS_UserTaskReturn()			OS_VOIDCALL(OS_KernTaskReturn())
#else
extern void OS_UserTaskReturn(void);
#endif
extern void OS_KernTaskReturn(void);
extern void OS_MissingTerminateTask(void);

#endif /* OS_ASM */

/* HW assisted tracing of ISRs
 * OS_HW_TRACE_ISR(os_isrid_t activeIsr)
 * Trace the entry/exit of activeIsr using HW assisted means (e.g. OTM).
 *
 * Note: This is the default implementation, which does nothing. It may
 * get overwritten in the arch-specific header.
 */
#ifndef OS_HW_TRACE_ISR
#define OS_HW_TRACE_ISR(activeIsr)	do { } while (0)
#endif

/* HW assisted tracing of TASKs.
 * OS_HW_TRACE_TASK(os_taskid_t activeTask)
 * Trace the current task (and hence task changes).
 *
 * Note: Activation of tasks should be called by the hardware-specific
 * OS_StartTask(), OS_ResumeTaskSync(), OS_ResumeTaskAsync() and OS_Idle()
 * macros.
 */
#ifndef OS_HW_TRACE_TASK
#define OS_HW_TRACE_TASK(activeTask)	do { } while (0)
#endif

/* OS_CAT2STACKOVERFLOW checks for an overflow of the kernel stack
 * and the CAT2 ISR stack (which is typically the same).
*/
#ifndef OS_CAT2STACKOVERFLOW
#define OS_CAT2STACKOVERFLOW()	OS_KernelStackOverflow()
#endif

#endif /* OS_KERNEL_TASK_H */
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
