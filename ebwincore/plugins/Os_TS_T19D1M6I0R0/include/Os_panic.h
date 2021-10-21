/* Os_panic.h
 *
 * This file contains codes and reporting functionality for kernel runtime sanity checks.
 * In normal production these checks should be turned off.
 *
 * To use the checks, encapsulate the expression with OS_DEVSANITYCHECK(), The decision should be such
 * that it evaluates to true in the error case. In the true branch, call OS_PANIC() with the appropriate
 * panic code.
 *
 * Add the panic codes to the enum list as required.
 *
 * OS_PANIC is being used in the new task queue management functions. It can also be used to replace
 * calls to OS_ERROR() for several of the internal error tests in the existing code.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_panic.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_PANIC_H
#define OS_PANIC_H
#ifndef OS_ASM

#ifndef OS_DEVELOPMENT_SANITY_CHECKS
#define OS_DEVELOPMENT_SANITY_CHECKS	0
#endif

#if OS_DEVELOPMENT_SANITY_CHECKS
#define	OS_DEVSANITYCHECK(x)	(x)
#else
#define	OS_DEVSANITYCHECK(x)	0
#endif

#define OS_PANIC(x)				OS_Panic(x)

/* Panic codes
*/
enum os_panic_e
{
	OS_PANIC_NONE = 0,

	/* Problems detected at startup
	*/
	OS_PANIC_InsufficientHeap,
	OS_PANIC_InsufficientPageMaps,
	OS_PANIC_SyncArrayCorrupted,
	OS_PANIC_InconsistentAppModes,

	/* Problems detected at runtime
	*/
	OS_PANIC_SlaveSlotNotOccupied,
	OS_PANIC_TaskIsNotInPriorityQueue,
	OS_PANIC_PriorityQueueAlreadyFull,
	OS_PANIC_InvalidTaskState,
	OS_PANIC_InvalidAlarmState,
	OS_PANIC_IsrCannotBeKilled,
	OS_PANIC_HookCannotBeKilled,
	OS_PANIC_UnhandledNmi,
	OS_PANIC_IncorrectKernelNesting,
	OS_PANIC_InvalidIrqSource,
	OS_PANIC_InvalidResourcePriority,

	/* Problems detected during cross-core communication.
	*/
	OS_PANIC_CrosscoreCommunicationError,
	OS_PANIC_InvalidInternalSpinlockConfig,

	/* Problems detected by atomic functions.
	*/
	OS_PANIC_ATOMICS_InvalidInterruptStatus,

	/* Problems detected during startup consistency checks
	*/
	OS_PANIC_SCHK_NonPowerOfTwoTimerWraparoundNotSupported,
	OS_PANIC_SCHK_TimerMaxDeltaAndDefDeltaInconsistent,
	OS_PANIC_SCHK_CounterRangeLowerThanHardwareTimer,
	OS_PANIC_SCHK_TaskIdIsNotIndexInTaskTable,
	OS_PANIC_SCHK_TaskWithRunPrioLowerThanQueuePrio,
	OS_PANIC_SCHK_NonPremptiveTaskWithLowRunPrio,
	OS_PANIC_SCHK_ExtendedTaskInBasicConformanceClass,
	OS_PANIC_SCHK_ExtendedTaskWithMultipleActivations,
	OS_PANIC_SCHK_MultipleActivationsInConformanceClass1,
	OS_PANIC_SCHK_TaskWithMeasureExecButNoAccountingStructure,
	OS_PANIC_SCHK_TaskWithExecTimeLimitButNoAccountingStructure,
	OS_PANIC_SCHK_TaskWithOsIntLockTimeLessThanAllIntLockTime,
	OS_PANIC_SCHK_IncorrectPriorityQueueCount,
	OS_PANIC_SCHK_ZeroEntriesInPriorityQueue,
	OS_PANIC_SCHK_SingleEntryQueueWithDynamicVariables,
	OS_PANIC_SCHK_ConformanceClass1ButMultipleEntryQueue,
	OS_PANIC_SCHK_NoDynamicVariablesInMultipleEntryQueue,
	OS_PANIC_SCHK_IncorrectNumberOfEntriesInPriorityQueue,
	OS_PANIC_SCHK_OverlappingPriorityQueuesFound,
	OS_PANIC_SCHK_PriorityQueueEntriesOutOfBounds,
	OS_PANIC_SCHK_GapInPriorityQueuesFound,
	OS_PANIC_SCHK_PrioritySlotsArrayHasWrongSize,
	OS_PANIC_SCHK_IncorrectMasterPriorityBitAssignment,
	OS_PANIC_SCHK_IncorrectSlavePriorityBitAssignment,
	OS_PANIC_SCHK_AppIdIsNotIndexInAppTable,
	OS_PANIC_SCHK_AppConfigurationIdMismatch,
	OS_PANIC_SCHK_AppRestartTaskNotMemberOfApp,
	OS_PANIC_SCHK_AppHasNoPermissions,
	OS_PANIC_SCHK_AppPermissionBitNotUnique,
	OS_PANIC_SCHK_OwningApplicationHasNoPermission,
	OS_PANIC_SCHK_NoOwningApplication,
	OS_PANIC_SCHK_IsrWithMeasureExecButNoAccountingStructure,
	OS_PANIC_SCHK_IsrWithExecTimeLimitButNoAccountingStructure,
	OS_PANIC_SCHK_IsrWithOsIntLockTimeLessThanAllIntLockTime,
	OS_PANIC_SCHK_ScheduleTableActivateTaskAfterSetEvent,
	OS_PANIC_SCHK_ScheduleTableEventsNotInOrder,
	OS_PANIC_SCHK_DelayGreaterThanCounterMax,
	OS_PANIC_SCHK_DelayPlusMaxIncreaseGreaterThanCounterMax,
	OS_PANIC_SCHK_MaxDecreaseGreaterThanDelay,
	OS_PANIC_SCHK_SynchronisationRangeGreaterThanScheduleTableLength,
	OS_PANIC_SCHK_InvalidCore,
	OS_PANIC_SCHK_InvalidStructureOffsetValue,
	OS_PANIC_SCHK_InvalidTimer,
	OS_PANIC_SCHK_IntVectorOffset,
	OS_PANIC_SCHK_MpuNotSupported,
	OS_PANIC_SCHK_InvalidMpuSetup,

	OS_PANIC_UNKNOWN		/* Must be last */
};

typedef enum os_panic_e os_panic_t;

os_result_t OS_Panic(os_panic_t);

#endif /* OS_ASM */
#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
