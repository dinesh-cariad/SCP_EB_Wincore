/* Os_error.h
 *
 * Error handling.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_error.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_ERROR_H
#define OS_ERROR_H

#include <Os_types.h>

/*!
 * OS_SID_xxx
 *
 * Service IDs for all system services and other areas that can generate
 * errors.
 *
 * os_serviceid_t is the data type that holds one of these.
 *
 * These OS_SID_ macros are parsed by asc_Os/tools/errdb-to-c.pl, so their values
 * must be in one of two formats:
 *  - a literal decimal number, without parenthesis or type suffix
 *  - a simple expression of the form (sid+number), where sid is a
 *    previously-defined OS_SID_ macro, and number is a literal
 *    decimal number, without parenthesis or type suffix
*/
/* AUTOSAR-defined service ids */
#define OS_SID_GetApplicationId				OS_U(0)
#define OS_SID_GetIsrId						OS_U(1)
#define OS_SID_CallTrustedFunction			OS_U(2)
#define OS_SID_CheckIsrMemoryAccess			OS_U(3)
#define OS_SID_CheckTaskMemoryAccess		OS_U(4)
#define OS_SID_CheckObjectAccess			OS_U(5)
#define OS_SID_CheckObjectOwnership			OS_U(6)
#define OS_SID_StartScheduleTableRel		OS_U(7)
#define OS_SID_StartScheduleTableAbs		OS_U(8)
#define OS_SID_StopScheduleTable			OS_U(9)
#define OS_SID_ChainScheduleTable			OS_U(10)
#define OS_SID_StartScheduleTableSynchron	OS_U(11)
#define OS_SID_SyncScheduleTable			OS_U(12)
#define OS_SID_SetScheduleTableAsync		OS_U(13)
#define OS_SID_GetScheduleTableStatus		OS_U(14)
#define OS_SID_IncrementCounter				OS_U(15)
#define OS_SID_GetCounterValue				OS_U(16)
#define OS_SID_GetElapsedCounterValue		OS_U(17)
#define OS_SID_TerminateApplication			OS_U(18)
#define OS_SID_AllowAccess					OS_U(19)
#define OS_SID_GetApplicationState			OS_U(20)
/* OSEK API and others */
#define OS_SID_UnknownSyscall				OS_U(21)
#define OS_SID_ActivateTask					OS_U(22)
#define OS_SID_TerminateTask				OS_U(23)
#define OS_SID_ChainTask					OS_U(24)
#define OS_SID_Schedule						OS_U(25)
#define OS_SID_GetTaskId					OS_U(26)
#define OS_SID_GetTaskState					OS_U(27)
#define OS_SID_SuspendInterrupts			OS_U(28)
#define OS_SID_ResumeInterrupts				OS_U(29)
#define OS_SID_GetResource					OS_U(30)
#define OS_SID_ReleaseResource				OS_U(31)
#define OS_SID_SetEvent						OS_U(32)
#define OS_SID_ClearEvent					OS_U(33)
#define OS_SID_GetEvent						OS_U(34)
#define OS_SID_WaitEvent					OS_U(35)
#define OS_SID_GetAlarmBase					OS_U(36)
#define OS_SID_GetAlarm						OS_U(37)
#define OS_SID_SetRelAlarm					OS_U(38)
#define OS_SID_SetAbsAlarm					OS_U(39)
#define OS_SID_CancelAlarm					OS_U(40)
#define OS_SID_GetActiveApplicationMode		OS_U(41)
#define OS_SID_StartOs						OS_U(42)
#define OS_SID_ShutdownOs					OS_U(43)
#define OS_SID_GetStackInfo					OS_U(44)
#define OS_SID_DisableInterruptSource		OS_U(45)
#define OS_SID_EnableInterruptSource		OS_U(46)
#define OS_SID_TryToGetSpinlock				OS_U(47)
#define OS_SID_ReleaseSpinlock				OS_U(48)
#define OS_SID_ShutdownAllCores				OS_U(49)
#define OS_SID_GetCpuLoad					OS_U(50)
#define OS_SID_ResetPeakCpuLoad				OS_U(51)
/* Insert additional system services here and adjust OS_SID_XXX to match */
#define OS_SID_XXX							OS_U(52)
#define OS_SID_Dispatch						(OS_SID_XXX+0)
#define OS_SID_TrapHandler					(OS_SID_XXX+1)
#define OS_SID_IsrHandler					(OS_SID_XXX+2)
#define OS_SID_RunSchedule					(OS_SID_XXX+3)
#define OS_SID_KillAlarm					(OS_SID_XXX+4)
#define OS_SID_TaskReturn					(OS_SID_XXX+5)
#define OS_SID_HookHandler					(OS_SID_XXX+6)
#define OS_SID_ArchTrapHandler				(OS_SID_XXX+7)
#define OS_SID_MemoryManagement				(OS_SID_XXX+8)
/* Insert additional internal functions here and adjust OS_N_SIDS to match */
#define OS_N_SIDS							(OS_SID_XXX+9)

/*!
 * OS_ERROR_xxx
 *
 * Error conditions. Multiple services can use the same error
 * condition, but any one service must use a unique error condition
 * for each error detected.
 *
 * os_error_t is the data type that holds one of these.
 * os_errorresult_t is the data type that holds one of these but must also be capable of holding the same range of
 *                  values as os_result_t because the code might get translated.
*/
#define OS_ERROR_NoError					OS_U(0)	/* Same as UnknownError */
#define OS_ERROR_UnknownError				OS_U(0)
#define OS_ERROR_UnknownSystemCall			OS_U(1)
#define OS_ERROR_InvalidTaskId				OS_U(2)
#define OS_ERROR_InvalidTaskState			OS_U(3)
#define OS_ERROR_Quarantined				OS_U(4)
#define OS_ERROR_MaxActivations				OS_U(5)
#define OS_ERROR_WriteProtect				OS_U(6)
#define OS_ERROR_ReadProtect				OS_U(7)
#define OS_ERROR_ExecuteProtect				OS_U(8)
#define OS_ERROR_InvalidAlarmId				OS_U(9)
#define OS_ERROR_InvalidAlarmState			OS_U(10)
#define OS_ERROR_AlarmNotInUse				OS_U(11)
#define OS_ERROR_WrongContext				OS_U(12)
#define OS_ERROR_HoldsResource				OS_U(13)
#define OS_ERROR_NoEvents					OS_U(14)
#define OS_ERROR_TaskNotExtended			OS_U(15)
#define OS_ERROR_TaskNotInQueue				OS_U(16)
#define OS_ERROR_InvalidCounterId			OS_U(17)
#define OS_ERROR_CorruptAlarmList			OS_U(18)
#define OS_ERROR_ParameterOutOfRange		OS_U(19)
#define OS_ERROR_AlarmInUse					OS_U(20)
#define OS_ERROR_AlreadyStarted				OS_U(21)
#define OS_ERROR_InvalidStartMode			OS_U(22)
#define OS_ERROR_AlarmNotInQueue			OS_U(23)
#define OS_ERROR_InvalidResourceId			OS_U(24)
#define OS_ERROR_ResourceInUse				OS_U(25)
#define OS_ERROR_ResourcePriorityError		OS_U(26)
#define OS_ERROR_ResourceNestingError		OS_U(27)
#define OS_ERROR_TaskSuspended				OS_U(28)
#define OS_ERROR_NestingUnderflow			OS_U(29)
#define OS_ERROR_NestingOverflow			OS_U(30)
#define OS_ERROR_NonfatalException			OS_U(31)
#define OS_ERROR_FatalException				OS_U(32)
#define OS_ERROR_UnhandledNmi				OS_U(33)
#define OS_ERROR_UnknownInterrupt			OS_U(34)
#define OS_ERROR_TaskTimeBudgetExceeded		OS_U(35)
#define OS_ERROR_IsrTimeBudgetExceeded		OS_U(36)
#define OS_ERROR_UnknownTimeBudgetExceeded	OS_U(37)
#define OS_ERROR_Permission					OS_U(38)
#define OS_ERROR_ImplicitSyncStartRel		OS_U(39)
#define OS_ERROR_CounterIsHw				OS_U(40)
#define OS_ERROR_InvalidScheduleId			OS_U(41)
#define OS_ERROR_NotRunning					OS_U(42)
#define OS_ERROR_NotStopped					OS_U(43)
#define OS_ERROR_AlreadyChained				OS_U(44)
#define OS_ERROR_InvalidObjectType			OS_U(45)
#define OS_ERROR_InvalidObjectId			OS_U(46)
#define OS_ERROR_InvalidApplicationId		OS_U(47)
#define OS_ERROR_InvalidIsrId				OS_U(48)
#define OS_ERROR_InvalidMemoryRegion		OS_U(49)
#define OS_ERROR_NotChained					OS_U(50)
#define OS_ERROR_InvalidFunctionId			OS_U(51)
#define OS_ERROR_NotSyncable				OS_U(52)
#define OS_ERROR_NotImplemented				OS_U(53)
#define OS_ERROR_StackError					OS_U(54)
#define OS_ERROR_RateLimitExceeded			OS_U(55)
#define OS_ERROR_InterruptDisabled			OS_U(56)
#define OS_ERROR_ReturnFromTask				OS_U(57)
#define OS_ERROR_InsufficientStack			OS_U(58)
#define OS_ERROR_WatchdogTimeout			OS_U(59)
#define OS_ERROR_PllLockLost				OS_U(60)
#define OS_ERROR_ArithmeticTrap				OS_U(61)
#define OS_ERROR_MemoryProtection			OS_U(62)
#define OS_ERROR_NotTrusted					OS_U(63)
#define OS_ERROR_TaskResLockTimeExceeded	OS_U(64)
#define OS_ERROR_IsrResLockTimeExceeded		OS_U(65)
#define OS_ERROR_TaskIntLockTimeExceeded	OS_U(66)
#define OS_ERROR_IsrIntLockTimeExceeded		OS_U(67)
#define OS_ERROR_IncrementZero				OS_U(68)
#define OS_ERROR_DifferentCounters			OS_U(69)
#define OS_ERROR_ScheduleTableNotIdle		OS_U(70)
#define OS_ERROR_InvalidRestartOption		OS_U(71)
#define OS_ERROR_TaskAggregateTimeExceeded	OS_U(72)
#define OS_ERROR_IncorrectKernelNesting		OS_U(73)
#define OS_ERROR_KernelStackOverflow		OS_U(74)
#define OS_ERROR_TaskStackOverflow			OS_U(75)
#define OS_ERROR_IntEException				OS_U(76)
#define OS_ERROR_ExceptionInKernel			OS_U(77)
#define OS_ERROR_SysReq						OS_U(78)
#define OS_ERROR_StackOverflow				OS_U(79)
#define OS_ERROR_StackUnderflow				OS_U(80)
#define OS_ERROR_SoftBreak					OS_U(81)
#define OS_ERROR_UndefinedOpcode			OS_U(82)
#define OS_ERROR_AccessError				OS_U(83)
#define OS_ERROR_ProtectionFault			OS_U(84)
#define OS_ERROR_IllegalOperandAccess		OS_U(85)
#define OS_ERROR_UnknownException			OS_U(86)

#define OS_ERROR_UndefinedInstruction		OS_U(87)
#define OS_ERROR_Overflow					OS_U(88)
#define OS_ERROR_BrkInstruction				OS_U(89)
#define OS_ERROR_WdgTimer					OS_U(90)
#define OS_ERROR_NMI						OS_U(91)

#define OS_ERROR_RegisterBank				OS_U(92)	/* register bank overflow */
#define OS_ERROR_DebugInterface				OS_U(93)

#define OS_ERROR_InsufficientPageMaps		OS_U(94)
#define OS_ERROR_InsufficientHeap			OS_U(95)

#define OS_ERROR_TLB_multiple_hit					OS_U(96)
#define OS_ERROR_Userbreak							OS_U(97)
#define OS_ERROR_InstructionAddressError			OS_U(98)
#define OS_ERROR_InstructionTlbMiss					OS_U(99)
#define OS_ERROR_TlbProtectionViolation				OS_U(100)
#define OS_ERROR_GeneralIllegalInstruction			OS_U(101)
#define OS_ERROR_SlotIllegalInstruction				OS_U(102)
#define OS_ERROR_GeneralFPUDisable					OS_U(103)
#define OS_ERROR_SlotFPUDisable						OS_U(104)
#define OS_ERROR_DataAddressErrorRead				OS_U(105)
#define OS_ERROR_DataAddressErrorWrite				OS_U(106)
#define OS_ERROR_DataTlbMissRead					OS_U(107)
#define OS_ERROR_DataTlbMissWrite					OS_U(108)
#define OS_ERROR_DataTlbReadProtViolation			OS_U(109)
#define OS_ERROR_DataTlbWriteProtViolation			OS_U(110)
#define OS_ERROR_FpuException						OS_U(111)
#define OS_ERROR_InitialPageWrite					OS_U(112)
#define OS_ERROR_UnconditionalTrap					OS_U(113)
#define OS_ERROR_PrefetchAbort						OS_U(114)
#define OS_ERROR_DataAbort							OS_U(115)
#define OS_ERROR_IllegalSupervisorCall				OS_U(116)
#define OS_ERROR_IllegalInterrupt					OS_U(117)

/* error codes for Cortex-M exceptions */
#define OS_ERROR_NonMaskableInterrupt				OS_U(118)
#define OS_ERROR_HardFault							OS_U(119)
#define OS_ERROR_MemoryManagement					OS_U(120)
#define OS_ERROR_BusFault							OS_U(121)
#define OS_ERROR_UsageFault							OS_U(122)
#define OS_ERROR_ReservedException_1				OS_U(123)
#define OS_ERROR_ReservedException_2				OS_U(124)
#define OS_ERROR_ReservedException_3				OS_U(125)
#define OS_ERROR_ReservedException_4				OS_U(126)
#define OS_ERROR_SupervisorCall						OS_U(127)
#define OS_ERROR_DebugMonitor						OS_U(128)
#define OS_ERROR_ReservedException_5				OS_U(129)
#define OS_ERROR_PendingSupervisorCall				OS_U(130)
#define OS_ERROR_SystemTick							OS_U(131)

/* error codes for DSPIC traps */
#define OS_ERROR_OscillatorFailureTrap				OS_U(132)
#define OS_ERROR_StackErrorTrap						OS_U(133)
#define OS_ERROR_AddressErrorTrap					OS_U(134)
#define OS_ERROR_MathErrorTrap						OS_U(135)
#define OS_ERROR_DMACErrorTrap						OS_U(136)
#define OS_ERROR_GenericHardTrap					OS_U(137)
#define OS_ERROR_GenericSoftTrap					OS_U(138)
#define OS_ERROR_UnknownTrap						OS_U(139)

#define OS_ERROR_SysErr								OS_U(140)
#define OS_ERROR_HVTrap								OS_U(141)
#define OS_ERROR_FETrap								OS_U(142)
#define OS_ERROR_Trap								OS_U(143)
#define OS_ERROR_ReservedInstruction				OS_U(144)
#define OS_ERROR_CoprocessorUnusable				OS_U(145)
#define OS_ERROR_PrivilegedInstruction				OS_U(146)
#define OS_ERROR_MisalignedAccess					OS_U(147)
#define OS_ERROR_FEINT								OS_U(148)

#define OS_ERROR_InvalidSpinlockId					OS_U(149)
#define OS_ERROR_InvalidSpinlockNesting				OS_U(150)
#define OS_ERROR_SpinlockAlreadyHeld				OS_U(151)
#define OS_ERROR_SpinlockInterferenceDeadlock		OS_U(152)

#define OS_ERROR_CoreIsDown							OS_U(153)
#define OS_ERROR_InvalidCoreId						OS_U(154)
#define OS_ERROR_ApplicationNotAccessible			OS_U(155)
#define OS_ERROR_ApplicationNotRestarting			OS_U(156)
#define OS_ERROR_HoldsLock							OS_U(157)

#define OS_ERROR_SpinlockNotOccupied				OS_U(158)
#define OS_ERROR_CallTrustedFunctionCrosscore		OS_U(159)

/* error codes for ARC exceptions */
#define OS_ERROR_MemoryError						OS_U(160)
#define OS_ERROR_InstructionError					OS_U(161)
#define OS_ERROR_EV_MachineCheck					OS_U(162)
#define OS_ERROR_EV_TLBMissI						OS_U(163)
#define OS_ERROR_EV_TLBMissD						OS_U(164)
#define OS_ERROR_EV_ProtV							OS_U(165)
#define OS_ERROR_EV_PrivilegeV						OS_U(166)
#define OS_ERROR_EV_SWI								OS_U(167)
#define OS_ERROR_EV_Trap							OS_U(168)
#define OS_ERROR_EV_Extension						OS_U(169)
#define OS_ERROR_EV_DivZero							OS_U(170)
#define OS_ERROR_EV_DCError							OS_U(171)
#define OS_ERROR_EV_Misaligned						OS_U(172)
#define OS_ERROR_EV_VecUnit							OS_U(173)

#ifndef OS_ASM
typedef os_uint8_t os_error_t;
typedef os_uint8_t os_errorresult_t;
#endif

/*!
 * OS_ACTION_*
 *
 * Error actions
 *
 * os_erroraction_t is the data type that holds one of these
*/
#define OS_ACTION_MASK			OS_U(0x0f)	/* Mask for the Action field */
#define OS_ACTION_IGNORE		OS_U(0x00)	/* Ignore totally - return OK */
#define OS_ACTION_RETURN		OS_U(0x01)	/* Return result to caller */
#define OS_ACTION_KILL			OS_U(0x02)	/* Kill task or ISR */
#define OS_ACTION_QUARANTINE	OS_U(0x03)	/* Quarantine task or ISR */
#define OS_ACTION_QUARANTINEAPP	OS_U(0x04)	/* Quarantine Application */
#define OS_ACTION_RESTART		OS_U(0x05)	/* Kill and restart application */
#define OS_ACTION_SHUTDOWN		OS_U(0x06)	/* Shut down OS */
#define OS_ACTION_DONOTHING		OS_U(0xfe)	/* "Do nothing". Only for use as return value from ProtectionHook */
#define OS_ACTION_DEFAULT		OS_U(0xff)	/* Use default action. Only for use as return value from ProtectionHook */

#define OS_ActionIsValid(act)	((act)<=OS_ACTION_SHUTDOWN)

#define OS_ACTION_HOOK				OS_U(0xe0)	/* Mask for all autosar hooks */
#define OS_ACTION_HOOK_EB			OS_U(0xf0)	/* Mask for all hooks, including EB extensions */
#define OS_ACTION_NOHOOK			OS_U(0x00)	/* Use no hooks ;-) */
#define OS_ACTION_PROTECTIONHOOK	OS_U(0x80)	/* Call ProtectionHook */
#define OS_ACTION_ERRORHOOK			OS_U(0x40)	/* Call global ErrorHook */
#define OS_ACTION_ERRORHOOK_APP		OS_U(0x20)	/* Call application's ErrorHook */
#define OS_ACTION_ERRORHOOK_EB		OS_U(0x10)	/* Call global ErrorHook, only when EB extension is configured */

#ifndef OS_ASM
typedef os_uint8_t os_erroraction_t;

/* Configuration constant - which error hooks really get called, etc.
*/
extern const os_erroraction_t OS_hookSelection;
#endif

/*!
 * os_errorentry_t
 *
 * This is the data structure that drives the OS error handling
 * mechanism. The fields are in a strange order to optimise the
 * size of the struct, since we'll have rather a lot of them.
*/
#ifndef OS_ASM
typedef struct os_errorentry_s os_errorentry_t;

struct os_errorentry_s
{
	os_result_t			result;		/* result code for hooks and return */
	os_error_t			condition;	/* error condition */
	os_erroraction_t	action;		/* action to take */
};

#endif

/*!
 * os_errorstatus_t
 *
 * This structure is used by the kernel to store all the information
 * about the most recent error. The error hook function can use
 * OS_GetKernelData()->errorStatus to copy its contents into a user-
 * supplied area.
*/
#define OS_MAXPARAM		3		/* Largest no. of parameters to a service */

#ifndef OS_ASM
typedef struct os_errorstatus_s os_errorstatus_t;

struct os_errorstatus_s
{
	os_paramtype_t parameter[OS_MAXPARAM];		/* Parameters */
	os_result_t result;			/* return value from service */
	os_erroraction_t action;	/* Action to take */
	os_uint8_t calledFrom;		/* INTASK, INCAT1 etc. */
	os_serviceid_t serviceId;	/* OS_SID_xxx */
	os_error_t errorCondition;	/* OS_ERROR_xxx */
};
#define OS_ERRORSTATUS_INIT					\
	{										\
		{ 0u, 0u, 0u },	/*parameter*/		\
		0u,				/*result*/			\
		0u,				/*action*/			\
		0u,				/*calledFrom*/		\
		0u,				/*serviceId*/		\
		0u				/*errorCondition*/	\
	}

#endif

/*!
 * os_serviceinfo_t
 *
 * This structure is used when calling OS_Error to pass the service ID and
 * the associated error table to the error handler.
*/
#ifndef OS_ASM
typedef struct os_serviceinfo_s os_serviceinfo_t;

struct os_serviceinfo_s
{
	os_errorentry_t const *errorTable;
	os_serviceid_t sid;
	os_uint8_t nErrors;
};

extern const os_serviceinfo_t OS_svc_ActivateTask;
extern const os_serviceinfo_t OS_svc_AllowAccess;
extern const os_serviceinfo_t OS_svc_IncrementCounter;
extern const os_serviceinfo_t OS_svc_CallTrustedFunction;
extern const os_serviceinfo_t OS_svc_CancelAlarm;
extern const os_serviceinfo_t OS_svc_ChainScheduleTable;
extern const os_serviceinfo_t OS_svc_ChainTask;
extern const os_serviceinfo_t OS_svc_CheckIsrMemoryAccess;
extern const os_serviceinfo_t OS_svc_CheckObjectAccess;
extern const os_serviceinfo_t OS_svc_CheckObjectOwnership;
extern const os_serviceinfo_t OS_svc_CheckTaskMemoryAccess;
extern const os_serviceinfo_t OS_svc_ClearEvent;
extern const os_serviceinfo_t OS_svc_DisableInterruptSource;
extern const os_serviceinfo_t OS_svc_Dispatch;
extern const os_serviceinfo_t OS_svc_EnableInterruptSource;
extern const os_serviceinfo_t OS_svc_GetActiveApplicationMode;
extern const os_serviceinfo_t OS_svc_GetAlarm;
extern const os_serviceinfo_t OS_svc_GetAlarmBase;
extern const os_serviceinfo_t OS_svc_GetApplicationId;
extern const os_serviceinfo_t OS_svc_GetApplicationState;
extern const os_serviceinfo_t OS_svc_GetCounterValue;
extern const os_serviceinfo_t OS_svc_GetCpuLoad;
extern const os_serviceinfo_t OS_svc_ResetPeakCpuLoad;
extern const os_serviceinfo_t OS_svc_GetElapsedCounterValue;
extern const os_serviceinfo_t OS_svc_GetEvent;
extern const os_serviceinfo_t OS_svc_GetIsrId;
extern const os_serviceinfo_t OS_svc_GetResource;
extern const os_serviceinfo_t OS_svc_GetScheduleTableStatus;
extern const os_serviceinfo_t OS_svc_GetStackInfo;
extern const os_serviceinfo_t OS_svc_GetTaskId;
extern const os_serviceinfo_t OS_svc_GetTaskState;
extern const os_serviceinfo_t OS_svc_HookHandler;
extern const os_serviceinfo_t OS_svc_IsrHandler;
extern const os_serviceinfo_t OS_svc_KillAlarm;
extern const os_serviceinfo_t OS_svc_MemoryManagement;
extern const os_serviceinfo_t OS_svc_ReleaseResource;
extern const os_serviceinfo_t OS_svc_ReleaseSpinlock;
extern const os_serviceinfo_t OS_svc_ResumeInterrupts;
extern const os_serviceinfo_t OS_svc_RunSchedule;
extern const os_serviceinfo_t OS_svc_Schedule;
extern const os_serviceinfo_t OS_svc_SetAbsAlarm;
extern const os_serviceinfo_t OS_svc_SetEvent;
extern const os_serviceinfo_t OS_svc_SetRelAlarm;
extern const os_serviceinfo_t OS_svc_SetScheduleTableAsync;
extern const os_serviceinfo_t OS_svc_ShutdownAllCores;
extern const os_serviceinfo_t OS_svc_ShutdownOs;
extern const os_serviceinfo_t OS_svc_StartOs;
extern const os_serviceinfo_t OS_svc_StartScheduleTableRel;
extern const os_serviceinfo_t OS_svc_StartScheduleTableAbs;
extern const os_serviceinfo_t OS_svc_StartScheduleTableSynchron;
extern const os_serviceinfo_t OS_svc_StopScheduleTable;
extern const os_serviceinfo_t OS_svc_SuspendInterrupts;
extern const os_serviceinfo_t OS_svc_SyncScheduleTable;
extern const os_serviceinfo_t OS_svc_TaskReturn;
extern const os_serviceinfo_t OS_svc_TerminateApplication;
extern const os_serviceinfo_t OS_svc_TerminateTask;
extern const os_serviceinfo_t OS_svc_TrapHandler;
extern const os_serviceinfo_t OS_svc_TryToGetSpinlock;
extern const os_serviceinfo_t OS_svc_UnknownSID;
extern const os_serviceinfo_t OS_svc_UnknownSyscall;
extern const os_serviceinfo_t OS_svc_WaitEvent;

extern const os_serviceinfo_t OS_svc_ArchTrapHandler;

#endif /* OS_ASM */

#endif /* OS_ERROR_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
