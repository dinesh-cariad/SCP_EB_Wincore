#ifndef ECUM_BSW_H
#define ECUM_BSW_H

/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO EcuM2991,1 */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>     /* AUTOSAR ComStack types (NetworkHandleType) */

#include <EcuM_BSW_Types.h>     /* static module types */
#include <EcuM_Version.h>       /* static module version information */
#include <EcuM_BootTarget.h>    /* Module boot target declarations */

/*==================[macros]================================================*/

/* !LINKSTO EcuM2664,1 */
/*------------------[macros for EcuM_StateType]-----------------------------*/

#ifndef ECUM_STATE_STARTUP
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_STARTUP, indicates EcuM STARTUP state. */
#define ECUM_STATE_STARTUP           0x10U
#endif

#ifndef ECUM_STATE_STARTUP_ONE
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_STARTUP_ONE, substate of EcuM STARTUP. */
#define ECUM_STATE_STARTUP_ONE       0x11U
#endif

#ifndef ECUM_STATE_STARTUP_TWO
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_STARTUP_TWO, substate of EcuM STARTUP. */
#define ECUM_STATE_STARTUP_TWO       0x12U
#endif

#ifndef ECUM_STATE_WAKEUP
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP, indicates EcuM WAKEUP state. */
#define ECUM_STATE_WAKEUP            0x20U
#endif

#ifndef ECUM_STATE_WAKEUP_ONE
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_ONE, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_ONE        0x21U
#endif

#ifndef ECUM_STATE_WAKEUP_VALIDATION
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_VALIDATION, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_VALIDATION 0x22U
#endif

#ifndef ECUM_STATE_WAKEUP_REACTION
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_REACTION, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_REACTION   0x23U
#endif

#ifndef ECUM_STATE_WAKEUP_TWO
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_TWO, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_TWO        0x24U
#endif

#ifndef ECUM_STATE_WAKEUP_WAKESLEEP
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_WAKESLEEP, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_WAKESLEEP  0x25U
#endif

#ifndef ECUM_STATE_WAKEUP_TTII
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_WAKEUP_TTII, substate of EcuM WAKEUP. */
#define ECUM_STATE_WAKEUP_TTII       0x26U
#endif

#ifndef ECUM_STATE_RUN
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_RUN, indicates EcuM RUN state. */
#define ECUM_STATE_RUN               0x30U
#endif

#ifndef ECUM_STATE_APP_RUN
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_APP_RUN, substate of EcuM RUN. */
#define ECUM_STATE_APP_RUN           0x32U
#endif

#ifndef ECUM_STATE_APP_POST_RUN
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_APP_POST_RUN, substate of EcuM RUN */
#define ECUM_STATE_APP_POST_RUN      0x33U
#endif

#ifndef ECUM_STATE_SHUTDOWN
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_SHUTDOWN, indicates EcuM SHUTDOWN state. */
#define ECUM_STATE_SHUTDOWN          0x40U
#endif

#ifndef ECUM_STATE_PREP_SHUTDOWN
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_PREP_SHUTDOWN, substate of EcuM SHUTDOWN */
#define ECUM_STATE_PREP_SHUTDOWN     0x44U
#endif

#ifndef ECUM_STATE_GO_SLEEP
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_GO_SLEEP, substate of EcuM SHUTDOWN */
#define ECUM_STATE_GO_SLEEP          0x49U
#endif

#ifndef ECUM_STATE_GO_OFF_ONE
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_GO_OFF_ONE, substate of EcuM SHUTDOWN */
#define ECUM_STATE_GO_OFF_ONE        0x4dU
#endif

#ifndef ECUM_STATE_GO_OFF_TWO
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_GO_OFF_TWO, substate of EcuM SHUTDOWN */
#define ECUM_STATE_GO_OFF_TWO        0x4eU
#endif

#ifndef ECUM_STATE_SLEEP
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_SLEEP, indicates EcuM SLEEP state. */
#define ECUM_STATE_SLEEP             0x50U
#endif

#ifndef ECUM_STATE_RESET
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_RESET, indicates EcuM RESET state. */
#define ECUM_STATE_RESET             0x90U
#endif

#ifndef ECUM_STATE_OFF
/** \brief Value for EcuM_StateType
 **
 ** Definition of ECUM_STATE_OFF, indicates EcuM OFF state. */
#define ECUM_STATE_OFF               0x80U
#endif

/*------------------[macros for EcuM_ShutdownCauseType]---------------------*/

#ifndef ECUM_CAUSE_UNKNOWN
/** \brief Value for EcuM_ShutdownCauseType
 **
 **  No cause was set. */
#define ECUM_CAUSE_UNKNOWN                          0U
#endif

#ifndef ECUM_CAUSE_ECU_STATE
/** \brief Value for EcuM_ShutdownCauseType
 **
 **  ECU state machine entered a state for shutdown */
#define ECUM_CAUSE_ECU_STATE                        1U
#endif

#ifndef ECUM_CAUSE_WDGM
/** \brief Value for EcuM_ShutdownCauseType
 **
 **  Watchdog Manager detected a failure */
#define ECUM_CAUSE_WDGM                             2U
#endif

#ifndef ECUM_CAUSE_DCM
/** \brief Value for EcuM_ShutdownCauseType
 **
 **  Diagnostic Communication Manager requests a shutdown due to
 ** a service request */
#define ECUM_CAUSE_DCM                              3U
#endif

/*------------[macros for EcuM Multi-Core synchronization]------------------*/

#ifndef ECUM_CONTINUE_SYNC
/** \brief Value for EcuM_SyncActionType
 **
 **  EcuM should continue synchronizing its cores before
 **  the sleep/shutdown process */
#define ECUM_CONTINUE_SYNC                              0x00
#endif

#ifndef ECUM_ABORT_SYNC
/** \brief Value for EcuM_SyncActionType
 **
 **  EcuM should abort synchronization of its cores */
#define ECUM_ABORT_SYNC                                 0x01
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* !LINKSTO EcuM2676,1 */
#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/*--------------------------------------------------------------------------
 * General
 *--------------------------------------------------------------------------*/

#if (ECUM_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the EcuM.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Module Id
 **  - Vendor Id
 **  - Vendor specific version numbers
 **
 ** \param[out] VersionInfoPtr Pointer to where to store the version
 ** information of this module.
 **
 ** \ServiceID{::ECUM_SID_GET_VERSION_INFO}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_GetVersionInfo(
   P2VAR(Std_VersionInfoType, AUTOMATIC, ECUM_APPL_DATA) VersionInfoPtr);
#endif

/*--------------------------------------------------------------------------
 * Initialization and shutdown
 *--------------------------------------------------------------------------*/
/** \brief Prepare power off or reset.
 **
 ** This function instruct the ECU state manager module to perform a
 ** power off or a reset depending on the selected shutdown target.
 **
 ** \param[in] caller Module ID of the calling module. Only special modules are
 ** allowed to call this function.
 **
 ** \ServiceID{::ECUM_SID_DOWN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDown(uint16 caller);

/** \brief Prepare halt.
 **
 ** Instructs the ECU State Manager module to go into a sleep mode where the
 ** microcontroller is halted, depending on the selected shutdown target.
 **
 ** \ServiceID{::ECUM_SID_HALT}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHalt(void);

/** \brief Prepare Polling sleep mode.
 **
 ** Instructs the ECU State Manager module to go into a polling sleep mode
 ** depending on the selected shutdown target.
 **
 ** \ServiceID{::ECUM_SID_POLL}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoPoll(void);

/** \brief Initialize EcuM Module.
 **
 ** This function initializes the ECU state manager and carries out the
 ** startup procedure. The function will never return (it calls StartOS).
 **
 ** \ServiceID{::ECUM_SID_INIT}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_Init(void);

/** \brief Startup sequence two
 **
 ** This function implements the STARTUP II state.
 **
 ** \ServiceID{::ECUM_SID_STARTUP_TWO}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_StartupTwo(void);

/** \brief Shutdown ECU
 **
 ** Typically called from the shutdown hook, this function takes over
 ** execution control and will carry out GO OFF II activities.
 **
 ** \ServiceID{::ECUM_SID_SHUTDOWN}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_Shutdown(void);

/*--------------------------------------------------------------------------
 * State management
 *--------------------------------------------------------------------------*/

/* prevent redundant declarations if used from EcuM files which also include
 * Rte_EcuM.h */

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Select target state for shutdown sequence using AUTOSAR 3.2 API.
 **
 ** This function doesn't initiate any shutdown activities but only stores the
 ** target state for later use when performing a shutdown sequence.  During
 ** SHUTDOWN a change of the shutdown target from and to the value
 ** ECUM_STATE_SLEEP is not allowed.
 **
 ** Development Error Detection
 ** - ECUM_E_STATE_PAR_OUT_OF_RANGE: On invalid value for parameter \p
 **   target.
 **
 ** \param[in] target The selected shutdown target. Only the following subset
 ** of the EcuM_StateType value range is accepted:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[in] mode An index like value which can be dereferenced to a sleep
 ** mode (EcuM_SleepModeConfigType). Available sleep modes are defined at
 ** configuration time and are stored in the EcuM_SleepMode list. The mode
 ** parameter is only used if the target parameter equals to
 ** ECUM_STATE_SLEEP. In all other cases, it is ignored.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new shutdown target was not set.
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_SelectShutdownTarget
(
    EcuM_ASR32_StateType target,
    uint8                mode
);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Get target state for shutdown sequence using AUTOSAR 3.2 API.
 **
 ** This function always returns the selected shutdown target as set by
 ** EcuM_SelectShutdownTarget.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] shutdownTarget Is set to one of the following values:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[out] sleepMode If the return parameter is ECUM_STATE_SLEEP, then
 ** this output parameter identifies which of the configured sleep modes was
 ** actually chosen (index into EcuM_SleepMode). If the return parameter is
 ** ECUM_STATE_RESET, this out parameter tells which of the configured reset
 ** modes was actually chosen.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors. (Always).
 ** \retval E_NOT_OK The service has failed (e.g. due to a Null pointer being
 ** passed).
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_GetShutdownTarget
(
    P2VAR(EcuM_ASR32_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Get target state for shutdown sequence using AUTOSAR 3.2 API
 **
 ** This function always returns the same value until the next shutdown.
 ** The return value describes the ECU state from which the last wakeup
 ** or power up occurred.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] shutdownTarget Set to one of the following values:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[out] sleepMode If the return parameter is ECUM_STATE_SLEEP, this
 ** output parameter tells which of the configured sleep modes was actually
 ** chosen (index into EcuM_SleepMode). If the return parameter is
 ** ECUM_STATE_RESET, this out parameter tells which of the configured reset
 ** modes was actually chosen.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new shutdown target was not set.
 **
 ** \ServiceID{::ECUM_SID_GET_LAST_SHUT_TAR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_GetLastShutdownTarget
(
    P2VAR(EcuM_ASR32_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
);
#endif /* ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) ... */

/* If the Rte interface and the multicore functionality are enabled, the prototype for EcuM_SelectShutdownTarget
   is exposed by SchM_EcuM.h since a BswSwcMapping is present in the EcuM BSW module description file */
#if !((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_INCLUDE_RTE == STD_ON))
/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Select target state for shutdown sequence using AUTOSAR 4.0 API.
 **
 ** This function doesn't initiate any shutdown activities but only stores the
 ** target state for later use when performing a shutdown sequence.  During
 ** SHUTDOWN a change of the shutdown target from and to the value
 ** ECUM_STATE_SLEEP is not allowed.
 **
 ** Development Error Detection
 ** - ECUM_E_STATE_PAR_OUT_OF_RANGE: On invalid value for parameter \p
 **   target.
 **
 ** \param[in] target The selected shutdown target. Only the following subset
 ** of the EcuM_StateType value range is accepted:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[in] mode An index like value which can be dereferenced to a sleep
 ** mode (EcuM_SleepModeConfigType). Available sleep modes are defined at
 ** configuration time and are stored in the EcuM_SleepMode list. The mode
 ** parameter is only used if the target parameter equals to
 ** ECUM_STATE_SLEEP. In all other cases, it is ignored.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new shutdown target was not set.
 **
 ** \ServiceID{::ECUM_SID_SEL_SHUT_TAR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_SelectShutdownTarget
(
    EcuM_ASR40_StateType target,
    uint8                mode
);
#endif

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get target state for shutdown sequence using AUTOSAR 4.0 API.
 **
 ** This function always returns the selected shutdown target as set by
 ** EcuM_SelectShutdownTarget.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] shutdownTarget Is set to one of the following values:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[out] sleepMode If the return parameter is ECUM_STATE_SLEEP, then
 ** this output parameter identifies which of the configured sleep modes was
 ** actually chosen (index into EcuM_SleepMode). If the return parameter is
 ** ECUM_STATE_RESET, this out parameter tells which of the configured reset
 ** modes was actually chosen.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors. (Always).
 ** \retval E_NOT_OK The service has failed (e.g. due to a Null pointer being
 ** passed).
 **
 ** \ServiceID{::ECUM_SID_GET_SHUT_TAR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetShutdownTarget
(
    P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get target state for shutdown sequence using AUTOSAR 4.0 API
 **
 ** This function always returns the same value until the next shutdown.
 ** The return value describes the ECU state from which the last wakeup
 ** or power up occurred.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] shutdownTarget Set to one of the following values:
 **   - ECUM_STATE_SLEEP
 **   - ECUM_STATE_RESET
 **   - ECUM_STATE_OFF
 ** \param[out] sleepMode If the return parameter is ECUM_STATE_SLEEP, this
 ** output parameter tells which of the configured sleep modes was actually
 ** chosen (index into EcuM_SleepMode). If the return parameter is
 ** ECUM_STATE_RESET, this out parameter tells which of the configured reset
 ** modes was actually chosen.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new shutdown target was not set.
 **
 ** \ServiceID{::ECUM_SID_GET_LAST_SHUT_TAR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetLastShutdownTarget
(
    P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Select cause of shutdown using AUTOSAR 4.0 API
 **
 ** This function elects the cause for a shutdown.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[in] target The selected shutdown cause
 **
 ** \return Standard Return Code
 ** \retval E_OK The new shutdown cause was set.
 ** \retval E_NOT_OK The new shutdown cause was not set.
 **
 ** \ServiceID{::ECUM_SID_SEL_SHUT_CAUSE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_SelectShutdownCause
(
  EcuM_ASR40_ShutdownCauseType target
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get the cause of shutdown using AUTOSAR 4.0 API
 **
 ** This function returns the selected shutdown cause as set by
 ** EcuM_SelectShutdownCause.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] shutdownCause  The selected cause of the next shutdown.
 **
 ** \return Standard Return Code
 ** \retval E_OK The service has succeeded.
 ** \retval E_NOT_OK The service has failed,
 **         e.g. due to NULL pointer being passed
 **
 ** \ServiceID{::ECUM_SID_GET_SHUT_CAUSE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetShutdownCause
(
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) shutdownCause
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get most recent shutdown operation using AUTOSAR 4.0 API
 **
 ** This function returns information about the most recent
 ** shutdown operation.
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] target  One of these values is returned:
 **                         - ECUM_STATE_SLEEP
 **                         - ECUM_STATE_RESET
 **                         - ECUM_STATE_OFF
 ** \param[out] mode    This parameter tells which of the configured
 **                     sleep modes(target is ECUM_STATE_SLEEP) or which of the
 **                     reset mechanisms(target is ECUM_STATE_RESET) was
 **                     actually chosen.
 ** \param[out] cause   The selected shutdown cause
 ** \param[out] time    Absolute time of the shutdown if supported by hardware.
 **
 ** \return Standard Return Code
 ** \retval E_OK The service has succeeded.
 ** \retval E_NOT_OK The service has failed,
 **         e.g. due to NULL pointer being passed
 **
 ** \ServiceID{::ECUM_SID_GET_RCNT_SHUT_CAUSE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetMostRecentShutdown
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA)         target,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                        mode,
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) cause,
  P2VAR(uint32, AUTOMATIC, ECUM_APPL_DATA)                       time
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get next most recent shutdown operation using AUTOSAR 4.0 API
 **
 ** This function returns information about the next most recent
 ** shutdown operation. All stored shutdown information can be read by
 ** first calling EcuM_GetMostRecentShutdown and then looping over
 **
 ** Development Error Detection
 ** - ECUM_E_NULL_POINTER: If a null pointer was passed as argument.
 **
 ** \param[out] target  One of these values is returned:
 **                         - ECUM_STATE_SLEEP
 **                         - ECUM_STATE_RESET
 **                         - ECUM_STATE_OFF
 ** \param[out] mode    This parameter tells which of the configured
 **                     sleep modes(target is ECUM_STATE_SLEEP) or which
 **                     of the reset mechanisms(target is ECUM_STATE_RESET) was
 **                     actually chosen.
 ** \param[out] cause   The selected shutdown cause
 ** \param[out] time    Absolute time of the shutdown if supported by hardware.
 **
 ** \return Standard Return Code
 ** \retval E_OK The service has succeeded.
 ** \retval E_NOT_OK The service has failed,
 **         e.g. due to NULL pointer being passed
 **
 ** \ServiceID{::ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetNextRecentShutdown
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA)         target,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                        mode,
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) cause,
  P2VAR(uint32, AUTOMATIC, ECUM_APPL_DATA)                       time
);

/*--------------------------------------------------------------------------
 * Wakeup handling
 *--------------------------------------------------------------------------*/

/** \brief Gets pending wakeup events.
 **
 ** This function gets all wakeup events which have been set but not yet
 ** validated.
 **
 ** \return Wakeup sources which have pending events.
 **
 ** \ServiceID{::ECUM_SID_GET_PEND_WK_EV}
 ** \Reentrancy{Non-Reentrant Non-Interruptible}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void);

/** \brief Clear wakeup events.
 **
 ** This function clears the wakeup events for a set of sources.
 **
 ** \param[in] sources Events of wakeup sources to be cleared.
 **
 ** \ServiceID{::ECUM_SID_CLEAR_WK_EV}
 ** \Reentrancy{Non-Reentrant Non-Interruptible}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent
(
    EcuM_WakeupSourceType sources
);

/** \brief Get validated wakeup events.
 **
 ** This function retrieves wakeup events validated using
 ** EcuM_ValidateWakeupEvent.
 **
 ** \return Wakeup sources which have been validated events.
 **
 ** \ServiceID{::ECUM_SID_GET_VALID_WK_EV}
 ** \Reentrancy{Non-Reentrant Non-Interruptible}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_WakeupSourceType, ECUM_CODE)
  EcuM_GetValidatedWakeupEvents(void);

/** \brief Get expired wakeup events.
 **
 ** This function retrieves wakeup events that have been set and for which
 ** validation has failed.  Events which do not need validation are never be
 ** reported by this function.
 **
 ** \return All events that have been set and for which validation has
 ** failed.
 **
 ** \ServiceID{::ECUM_SID_GET_EXP_WK_EV}
 ** \Reentrancy{Non Reentrant Non-Interruptible}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void);

/*--------------------------------------------------------------------------
 * Scheduled functions
 *--------------------------------------------------------------------------*/

#if (RTE_CONFIG_PRESENT == FALSE)
/** \brief EcuM main function
 **
 ** This service implements all activities of the ECU State Manager
 ** while the OS is up and running.
 **
 ** This function has to be called periodically by a task controlled by the
 ** BSW scheduler.
 **
 ** \ProductionError{ECUM_E_RAM_CHECK_FAILED, thrown\, if The RAM check during
 ** wakeup failed.}
 **
 ** \ServiceID{::ECUM_SID_MAIN_FUNCTION}
 ** \Reentrancy{Not-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_MainFunction(void);
#endif /* RTE_CONFIG_PRESENT == TRUE */

/*--------------------------------------------------------------------------
 * EcuM Fixed - Run Request Protocol
 *--------------------------------------------------------------------------*/

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Enter RUN state
 **
 ** When EcuMFixed support is enabled, EcuM_Mainfunction() will only perform
 ** wakeup validation, and will process EcuMFixed specific functionality only
 ** after this callback has been called.
 ** This callback is supposed to be called by BswM after it has finished the
 ** initialization procedure (after both startup or wakeup), including all
 ** deferred initialization tasks.
 **
 ** \ServiceID{::ECUM_SID_GO_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoRun(void);

/** \brief  Releases all RUN and POST_RUN requests
 **
 ** No further RUN or POST_RUN requests are accepted
 ** until shutdown has completed.
 **
 ** The benefit of this function over an ECU reset is that the shutdown
 ** sequence is executed, which e.g. takes care of writing back NV memory
 ** contents.
 **
 ** \ProductionError{ECUM_E_ALL_RUN_REQUESTS_KILLED, thrown\, if the service
 ** EcuM_KillAllRUNRequests was issued.}
 **
 ** \ServiceID{::ECUM_SID_KILL_ALL_RUN_REQUESTS}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_KillAllRUNRequests(void);

/** \brief Get the status of wakeup sources
 **
 ** Returns the sum status of the requested wakeup sources
 **
 ** \param[in] sources Bit mask of the wakeup sources for which the
 **                    status is requested.
 **
 ** \return Sum status of all wakeup sources.
 ** \retval ECUM_WKSTATUS_VALIDATED At least one wakeupt source is validated.
 ** \retval ECUM_WKSTATUS_PENDING   At least one wakeupt source is pending, none
 **                                 is validated
 ** \retval ECUM_WKSTATUS_EXPIRED   At least one wakeupt source is expired, none
 **                                 is validated or pending
 ** \retval ECUM_WKSTATUS_NONE      No wakeup source is validated, pending or
 **                                 expired
 **
 ** \ServiceID{::ECUM_SID_GET_STAT_OF_WK_SRC}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_WakeupStatusType, ECUM_CODE)
          EcuM_GetStatusOfWakeupSource(EcuM_WakeupSourceType sources);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Places a request for the RUN state using AUTOSAR 4.0 API
 **
 ** EcuM enters RUN mode after the EcuM_GoRun() callback has been called
 ** and stays there for as long as RUN requests requested by this API
 ** function are pending.
 **
 ** Requests can be placed by every user made known to the state manager
 ** at configuration time.
 **
 ** \param[in] user ID of the entity requesting the RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The request was accepted by EcuM.
 ** \retval E_NOT_OK The request was not accepted by EcuM, a detailed
 **                  error condition was sent to DET.
 **
 ** \ServiceID{::ECUM_SID_REQUEST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_RequestRUN(EcuM_ASR40_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Releases a RUN request previously done with a call to EcuM_RequestRUN
 **        using AUTOSAR 4.0 API.
 **
 ** When the last RUN request is released, EcuM leaves the RUN state.
 **
 ** The service is intended for implementing AUTOSAR ports.
 **
 ** \param[in] user ID of the entity releasing the RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The release request was accepted by EcuM.
 ** \retval E_NOT_OK The release request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_RELEASE_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_ReleaseRUN(EcuM_ASR40_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Places a request for the POST RUN state using AUTOSAR 4.0 API.
 **
 ** EcuM enters POST_RUN state after all RUN requests have been released
 ** and stays there for as long as POST_RUN requests are pending.
 **
 ** Requests can be placed by every user made known to the state manager
 ** at configuration time. Requests for RUN and POST RUN must be tracked
 ** independently (in other words: two independent variables). The service
 ** is intended for implementing AUTOSAR ports
 **
 ** \param[in] user ID of the entity requesting the POST RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The request was accepted by EcuM.
 ** \retval E_NOT_OK The request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_REQUEST_POST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_RequestPOST_RUN(EcuM_ASR40_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Releases a POST RUN request previously done with a call to
 **        EcuM_RequestPOST_RUN using AUTOSAR 4.0 API.
 **
 ** After the last POST_RUN request has been released, EcuM
 ** indicates the SHUTDOWN mode to Rte. This mode transition
 ** can then be caught by BswM to proceed with the shutdown
 ** or sleep tasks.
 **
 ** The service is intended for implementing AUTOSAR ports.
 **
 ** \param[in] user ID of the entity releasing the POST RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The release request was accepted by EcuM.
 ** \retval E_NOT_OK The release request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_RELEASE_POST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_ReleasePOST_RUN(EcuM_ASR40_UserType user);

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Places a request for the RUN state using AUTOSAR 3.2 API
 **
 ** EcuM enters RUN mode after the EcuM_GoRun() callback has been called
 ** and stays there for as long as RUN requests requested by this API
 ** function are pending.
 **
 ** Requests can be placed by every user made known to the state manager
 ** at configuration time.
 **
 ** \param[in] user ID of the entity requesting the RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The request was accepted by EcuM.
 ** \retval E_NOT_OK The request was not accepted by EcuM, a detailed
 **                  error condition was sent to DET.
 **
 ** \ServiceID{::ECUM_SID_REQUEST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_RequestRUN(EcuM_ASR32_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Releases a RUN request previously done with a call to EcuM_RequestRUN
 **        using AUTOSAR 3.2 API.
 **
 ** When the last RUN request is released, EcuM leaves the RUN state.
 **
 ** The service is intended for implementing AUTOSAR ports.
 **
 ** \param[in] user ID of the entity releasing the RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The release request was accepted by EcuM.
 ** \retval E_NOT_OK The release request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_RELEASE_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_ReleaseRUN(EcuM_ASR32_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Places a request for the POST RUN state using AUTOSAR 3.2 API.
 **
 ** EcuM enters POST_RUN state after all RUN requests have been released
 ** and stays there for as long as POST_RUN requests are pending.
 **
 ** Requests can be placed by every user made known to the state manager
 ** at configuration time. Requests for RUN and POST RUN must be tracked
 ** independently (in other words: two independent variables). The service
 ** is intended for implementing AUTOSAR ports
 **
 ** \param[in] user ID of the entity requesting the POST RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The request was accepted by EcuM.
 ** \retval E_NOT_OK The request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_REQUEST_POST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_RequestPOST_RUN(EcuM_ASR32_UserType user);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Releases a POST RUN request previously done with a call to
 **        EcuM_RequestPOST_RUN using AUTOSAR 3.2 API.
 **
 ** After the last POST_RUN request has been released, EcuM
 ** indicates the SHUTDOWN mode to Rte. This mode transition
 ** can then be caught by BswM to proceed with the shutdown
 ** or sleep tasks.
 **
 ** The service is intended for implementing AUTOSAR ports.
 **
 ** \param[in] user ID of the entity releasing the POST RUN state.
 **
 ** \return Standard Return Code
 ** \retval E_OK The release request was accepted by EcuM.
 ** \retval E_NOT_OK The release request was not accepted by
 **                  EcuM, a detailed error condition was
 **                  sent to DET.
 **
 ** \ServiceID{::ECUM_SID_RELEASE_POST_RUN}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_ReleasePOST_RUN(EcuM_ASR32_UserType user);
#endif /* ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || ... */

#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */
#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_BSW_H */

