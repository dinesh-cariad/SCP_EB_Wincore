/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.SecurityAccess.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service SecurityAccess. */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 *
 */
/*===============================[includes]======================================================*/

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x27 handling. */

#include <Dcm_Dsp_SvcH_SecurityAccess.h>
#include <TSMem.h>
#include <TSAutosar.h>                                             /* EB specific standard types */

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Types.h>
#include <Dcm_DspExternal.h>
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#include <Dcm_SecurityAccess_Cfg.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>

/*===========================[macro definitions]=================================================*/

/** @defgroup Macro_Dcm_SecStateType Macros for Dcm_SecStateType
 ** \brief    These macro definitions are used as values for Dcm_SecStateType
 **  @{
 */

#if (defined DCM_SEC_SEND_KEY)
#error DCM_SEC_SEND_KEY already defined
#endif /* #if (defined DCM_SEC_COMP_KEY) */

/** \brief Request type is SendKey */
#define DCM_SEC_SEND_KEY 0x00U

#if (defined DCM_SEC_REQ_SEED)
#error DCM_SEC_REQ_SEED already defined
#endif /* #if (defined DCM_SEC_REQ_SEED) */

/** \brief Request type is GetSeed */
#define DCM_SEC_REQ_SEED 0x01U

/* @} *//* End of macros definitions for Dcm_SecStateType */

#if (defined DCM_SEC_IDLE)
#error DCM_SEC_IDLE already defined
#endif /* #if (defined DCM_SEC_IDLE) */

/** \brief Value for security state if no seed was requested yet or after key was sent. */
#define DCM_SEC_IDLE 0x00U

#if (defined DCM_SECURITYACCESS_GET_REQUEST_TYPE)
  #error "DCM_SECURITYACCESS_GET_REQUEST_TYPE is already defined"
#endif
#define DCM_SECURITYACCESS_GET_REQUEST_TYPE(SubserviceID) \
  ((Dcm_SecStateType) ((SubserviceID) % 2U))

#if (defined DCM_SECURITYACCESS_GET_SECURITY_LEVEL)
  #error "DCM_SECURITYACCESS_GET_SECURITY_LEVEL is already defined"
#endif
#define DCM_SECURITYACCESS_GET_SECURITY_LEVEL(SubserviceID) \
  ((Dcm_SecLevelType) ((SubserviceID) + 1U) / 2U)

#if (defined DCM_SECURITYACCESS_SEQUENCE_IS_INCORRECT)
  #error "DCM_SECURITYACCESS_SEQUENCE_IS_INCORRECT is already defined"
#endif
#define DCM_SECURITYACCESS_SEQUENCE_IS_INCORRECT(SubserviceID, PreviousRequestedLevel) \
  ((SubserviceID) != ((PreviousRequestedLevel)*2U))

/** \brief In this state an attempt is made to retrieve the security delay counters for levels that
 *  **        have external handling of them enabled */
#if (defined DCM_DSP_SECURITYACCESS_INITSTATE_UNINITIALIZED)
  #error "DCM_DSP_SECURITYACCESS_INITSTATE_UNINITIALIZED is already defined"
#endif
#define DCM_DSP_SECURITYACCESS_INITSTATE_UNINITIALIZED   1U

/** \brief In this state the delay timers are started as necessary */
#if (defined DCM_DSP_SECURITYACCESS_INITSTATE_STARTTIMERS)
  #error "DCM_DSP_SECURITYACCESS_INITSTATE_STARTTIMERS is already defined"
#endif
#define DCM_DSP_SECURITYACCESS_INITSTATE_STARTTIMERS    2U

/** \brief In this state the delay timers are processed if any is active */
#if (defined DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED)
  #error "DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED is already defined"
#endif
#define DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED     3U

/* \brief Take care not increment the attempt counter past this value to avoid an overflow */
#if (defined DCM_DSP_SECURITYACCESS_ATTEMPT_COUNTER_MAX_VALUE)
  #error "DCM_DSP_SECURITYACCESS_ATTEMPT_COUNTER_MAX_VALUE is already defined"
#endif
#define DCM_DSP_SECURITYACCESS_ATTEMPT_COUNTER_MAX_VALUE 0xFFU

/* \brief Type of responses to send the client after the attempt counter was successfully saved as
 *        a result of reset to 0 or increment during a send key request */
#if (defined DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_NONE)
  #error "DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_NONE is already defined"
#endif
#define DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_NONE                        1U

/* \brief Send a positive response after the attempt counter was successfully saved */
#if (defined DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS)
  #error "DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS is already defined"
#endif
#define DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS                         2U

/* \brief Send InvalidKey Nrc after the attempt counter was successfully saved */
#if (defined DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY)
  #error "DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY is already defined"
#endif
#define DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY                 3U

/* \brief Send ExceededNumberOfAttempt Nrc after the attempt counter was successfully saved */
#if (defined DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS)
  #error "DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS is already defined"
#endif
#define DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS 4U

#if (defined DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED)
#error DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED) */

/* \brief Mark the reading of the attempt counter for a given security level as failed */
#define DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED(SecLevel) \
  DCM_UINT32_SET_BIT(Dcm_Dsp_SecurityAccess_AttemptCounterReadFailed,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_CLEAR_ATTEMPTCOUNTER_READ_FAILED)
#error DCM_DSP_SECURITYACCESS_CLEAR_ATTEMPTCOUNTER_READ_FAILED already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_CLEAR_ATTEMPTCOUNTER_READ_FAILED) */

/* \brief Mark the reading of the attempt counter for a given security level as OK */
#define DCM_DSP_SECURITYACCESS_CLEAR_ATTEMPTCOUNTER_READ_FAILED(SecLevel) \
  DCM_UINT32_CLEAR_BIT(Dcm_Dsp_SecurityAccess_AttemptCounterReadFailed,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED)
#error DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED) */

/* \brief Check if read of an attempt counter failed for the given security level */
#define DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED(SecLevel) \
  DCM_UINT32_IS_BIT_SET(Dcm_Dsp_SecurityAccess_AttemptCounterReadFailed,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL)
#error DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL) */

/* \brief Mark a security level as being locked with (infinite) delay timer running */
#define DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevel) \
  DCM_UINT32_SET_BIT(Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL)
#error DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL) */

/* \brief Mark a security level as being unlocked */
#define DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL(SecLevel) \
  DCM_UINT32_CLEAR_BIT(Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED)
#error DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED) */

/* \brief Check if a security level is currently locked */
#define DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED(SecLevel) \
  DCM_UINT32_IS_BIT_SET(Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked,(SecLevel))

#if (defined DCM_DSP_SECURITYACCESS_ENTER_EXCLUSIVE_AREA)
#error DCM_DSP_SECURITYACCESS_ENTER_EXCLUSIVE_AREA already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_ENTER_EXCLUSIVE_AREA) */

/** \brief Defines how SecurityAccess implements protection against concurrent access:
 *         Entering exclusive area*/
#define DCM_DSP_SECURITYACCESS_ENTER_EXCLUSIVE_AREA SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0

#if (defined DCM_DSP_SECURITYACCESS_EXIT_EXCLUSIVE_AREA)
#error DCM_DSP_SECURITYACCESS_EXIT_EXCLUSIVE_AREA already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_EXIT_EXCLUSIVE_AREA) */

/** \brief Defines how SecurityAccess implements protection against concurrent access:
 *         Exiting exclusive area*/
#define DCM_DSP_SECURITYACCESS_EXIT_EXCLUSIVE_AREA SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0

/** \brief Indicates an index is not valid */
#if (defined DCM_DSP_SECURITYACCESS_INVALID_INDEX)
  #error "DCM_DSP_SECURITYACCESS_INVALID_INDEX is already defined"
#endif
#define DCM_DSP_SECURITYACCESS_INVALID_INDEX DCM_NUM_CONFIGURED_SECURITY_LEVELS

#if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY)
#error DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY) */

/* \brief In this state the key is compared */
#define DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY        0U

#if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING)
#error DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING) */

/* \brief In this state saving the attempt counter is initialized */
#define DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING         1U

#if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING)
#error DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING) */

/* \brief In this state the attempt counter is saved */
#define DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING               2U

#if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED)
#error DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED already defined
#endif /* #if (defined DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED) */

/* \brief In this state the actions that follow a successfull compare key are executed */
#define DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED 3U

/*===========================[type definitions]=================================================*/

/* !LINKSTO Dcm.Dsn.Type.SecurityAccess.InitStateType,1 */
/** \brief The states of the attempt counter initialization state machine */
typedef uint8 Dcm_Dsp_SecurityAccess_InitStateType;

/* !LINKSTO Dcm.Dsn.Type.SecurityAccess.SendKey.StateType,1 */
/** \brief The states of the SendKey service request. Comprises CompareKey, SaveAttemptCounterInit
 **        and SaveAttemptCounter */
typedef uint8 Dcm_Dsp_SecurityAccess_SendKey_StateType;

/* !LINKSTO Dcm.Dsn.Type.SecurityAccess.PostSaveAttemptCounterRespType,1 */
/** \brief The possible types of responses to send after the successfull saving of an attempt
 **        counter from the service handler. */
typedef uint8 Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterRespType;

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Validates the UDS service 0x27 request received
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP    A "response pending" message shall be sent immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
/** \brief Processes the UDS service 0x27 request
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP    A "response pending" message shall be sent immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

/** \brief Handles the Request seed subfunction of SecurityAccess service
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[out] Nrc Negative reponse code
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier
 **
 ** \retval DCM_E_DONE                  Operation completed
 ** \retval DCM_E_PENDING            Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_RequestSeed_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

/** \brief Handles the GetSeed operation.
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[out] Nrc Negative reponse code
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier
 ** \param[in] NewSecTabEntry pointer to configuration information of requested security level
 ** \retval DCM_E_DONE                  Operation completed
 ** \retval DCM_E_PENDING            Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_GetSeed_Operation(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

/** \brief Handles the Send Key subfunction of SecurityAccess service
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[out] Nrc Negative reponse code
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP    A "response pending" message shall be sent immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SendKey_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

/** \brief Perform the compare key operation
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[out] Nrc Negative reponse code
 ** \param[inout] MsgContextPtr Message related information for one diagnostic protocol identifier
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP    A "response pending" message shall be sent immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_CompareKey_Operation(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);

#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
/** \brief Initialize the attempt counters during boot by reading them or applying a default
 **        value to them.
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_AttemptCounters_Initialize(void);

/** \brief Initiate and/or continue the saving of an attempt counter
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[out] Nrc Negative reponse code
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDING        Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP    A "response pending" message shall be sent immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SendKey_SaveAttemptCounter(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc);

#if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)
/** \brief Save one attempt counter after it was reset because of a delay timer
 **        expiring
 **
 ** \param[in] SecLevelIndex  Index of security level to be saved
 **
 ** \retval DCM_E_OK            Operation completed with success
 ** \retval DCM_E_NOT_OK        Operation completed with failure
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP   Saving will just continue in the next cycle with a
 **                             call of the operation which returned a DCM_E_FORCE_RCRRP
 **                             error with a DCM_FORCE_RCRRP_OK OpStatus, in order to
 **                             simulate a successful RCRRP transmission.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SaveResetAttemptCounter(
  uint8 SecLevelIndex);

/** \brief Find a delay timer that is currently being saved
 **
 ** \retval Index of the found delay timer
 */
STATIC FUNC(uint8_least, DCM_CODE) Dcm_Dsp_SecurityAccess_FindSavingDelayTimer(
  void);
#endif /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

/** \brief Increment the attempt counter, check if the maximal number of attempts has been reached
 **        and set the appropriate error code.
 **
 ** \param[in] SecLevelIndex Index of the security level where to start the delay timer
 ** \param[out] Nrc Negative reponse code
 **
 ** \retval NONE
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_CompareKey_TreatAttemptCounters(
  uint8 SecLevelIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Start delay timers after the attempt counters were read out during system startup
 **
 ** \retval NONE
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_StartDelayTimersOnStartup(
  void);

/** \brief Start an individual security access delay timer
 **
 ** \param[in] SecLevelIndex Index of the security level where to start the delay timer
 ** \param[in] Value         Value to start the delay timer with
 **
 ** \retval NONE
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_StartDelayTimer(
  uint8 SecLevelIndex,
  uint32 Value,
  boolean InfiniteDelay);

/** \brief Decrement the delay timers and handle the attempt counters when delay timers expires
 **
 ** \retval NONE
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_DelayTimers_Decrement(
  void);
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Index of currently requested security level */
STATIC VAR(uint8, DCM_VAR) Dcm_Dsp_SecurityAccess_ReqSecLevelIndex;

/** \brief Last requested security level.
 **        Value in range 1-63 - indicates the last requested security level
 **        Value 0 indicates that no security level was requested previously -security handling idle
 */
STATIC VAR(Dcm_SecLevelType, DCM_VAR) Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel;

#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)

/** \brief Indicate the need to lock a security level upon confirmation of response message */
STATIC VAR(uint8, DCM_VAR)  Dcm_Dsp_SecurityAccess_LockSecurityLevel;

/** \brief The state of FSM that manages the security accesss initialization state  */
STATIC VAR(Dcm_Dsp_SecurityAccess_InitStateType, DCM_VAR) Dcm_Dsp_SecurityAccess_InitState;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
/** \brief OpStatus used when calling the asynchronous variants of the API
 **        Xxx_GetSecurityAttemptCounter() and Xxx_SetSecurityAttemptCounter() during
 *         attempt counter initialization and after resetting them due to expired
 *         delay timers. It is not used when setting attempt counters as a result of
 *         them being incremented or reset in the service handler */
STATIC VAR(Dcm_OpStatusType, DCM_VAR) Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus;

/** \brief The state of the SendKey operation */
STATIC VAR(Dcm_Dsp_SecurityAccess_SendKey_StateType, DCM_VAR) Dcm_Dsp_SecurityAccess_SendKey_State;

/** \brief What response to send after successfully saving the attempt counter from
 **        the service handler after increment or reset */
STATIC VAR(Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterRespType, DCM_VAR)
  Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief The index of the last security level who's attempt counter was gotten or set. */
STATIC VAR(uint8_least, DCM_VAR) Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
#define DCM_START_SEC_VAR_CLEARED_32
#include <Dcm_MemMap.h>
/** \brief Bitwise encoded flags that indicate which security level is locked as a result of
 **        exceeded number of security access attempts */
STATIC VAR(uint32, DCM_VAR) Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
/** \brief An array of flags indicating that a given security level's attempt counter
 *         could not be read at startup time */
STATIC VAR(uint32, DCM_VAR) Dcm_Dsp_SecurityAccess_AttemptCounterReadFailed;

/** \brief Keeps track of the total allowed time to retrieve all or to set one attempt counter*/
STATIC VAR(uint32, DCM_VAR) Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief The delay timers runtime information per secruity level */
STATIC VAR(Dcm_Dsp_SecurityAccess_DelayTimerType, DCM_VAR)
  Dcm_Dsp_SecurityAccess_DelayTimers[DCM_NUM_CONFIGURED_SECURITY_LEVELS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_Reset(void)
{
   Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;
}

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.Initialization,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_Init(void)
{
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
  uint8_least SecLevelIndex;
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  Dcm_Dsp_SecurityAccess_AttemptCounterReadFailed = 0U;
  Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer = 0U;
  Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = 0U;
  Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp
    = DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_NONE;
  Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_INITIAL;
  Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

  Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked = 0U;
  Dcm_Dsp_SecurityAccess_LockSecurityLevel = DCM_DSP_SECURITYACCESS_INVALID_INDEX;
  Dcm_Dsp_SecurityAccess_InitState = DCM_DSP_SECURITYACCESS_INITSTATE_UNINITIALIZED;

  /* Deviation TASKING-1 <+2> */
  for (SecLevelIndex = 0U;
      SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
      SecLevelIndex++
      )
  {
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter = 0U;
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer = 0U;
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay = FALSE;
  }
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
  Dcm_Dsp_SecurityAccess_ReqSecLevelIndex = DCM_DSP_SECURITYACCESS_INVALID_INDEX;
  Dcm_Dsp_SecurityAccess_Reset();
}

/* This is the generic service handler for UDS service 0x27 */
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SvcH,1 */
/* !LINKSTO Dcm252,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_SECURITYACCESS_SVCH_ENTRY(OpStatus,MsgContextPtr);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SvcStart(OpStatus, MsgContextPtr);
      break;

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
    /* Subsequent call to the service function after FORCE_RCRRP */
    case DCM_FORCE_RCRRP_OK:
      ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SvcCont(OpStatus, MsgContextPtr);
      break;

    /* Consecutive call after the function returned PENDING before */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SvcCont(OpStatus, MsgContextPtr);
      break;

    /* cancel any ongoing service processing */
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SvcCont(OpStatus, MsgContextPtr);
      break;
#endif /* DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U */
    /* actions performed on successful sending of the response */
    case DCM_CONFIRMED_OK:
    /* actions performed on failure in sending of the response */
    case DCM_CONFIRMED_NOK:
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
      /* Start single or unified delay timer if necessary */
      if (Dcm_Dsp_SecurityAccess_LockSecurityLevel != DCM_DSP_SECURITYACCESS_INVALID_INDEX)
      {
        const uint8 LockSecurityLevel = Dcm_Dsp_SecurityAccess_LockSecurityLevel;
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        uint8_least SecLevelIndex;
        for(SecLevelIndex = 0U;
            SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
            SecLevelIndex++
           )
        {
          /* DcmDspSecurityDelayTime value for the SecurityLevel */
          Dcm_Dsp_SecurityAccess_StartDelayTimer(
            (uint8)SecLevelIndex,
            Dcm_SecurityLevelEntryConfig[LockSecurityLevel].DelayTime,
            Dcm_SecurityLevelEntryConfig[LockSecurityLevel].DelayTimeInfinite);
        }
#else
        Dcm_Dsp_SecurityAccess_StartDelayTimer(
          LockSecurityLevel,
          Dcm_SecurityLevelEntryConfig[LockSecurityLevel].DelayTime,
          Dcm_SecurityLevelEntryConfig[LockSecurityLevel].DelayTimeInfinite);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* Reset the lock request */
        Dcm_Dsp_SecurityAccess_LockSecurityLevel = DCM_DSP_SECURITYACCESS_INVALID_INDEX;
      }
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
      ServiceProcessStatus = DCM_E_DONE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SECURITYACCESS);
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_SECURITYACCESS_SVCH_EXIT(ServiceProcessStatus,OpStatus,MsgContextPtr);
  return ServiceProcessStatus;
}

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.MainFunction,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_MainFunction(void)
{
  /* Process the delay timers - if necessary */
  if (Dcm_Dsp_SecurityAccess_InitState == DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED)
  {
    Dcm_Dsp_SecurityAccess_DelayTimers_Decrement();
  }
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  else if (Dcm_Dsp_SecurityAccess_InitState == DCM_DSP_SECURITYACCESS_INITSTATE_UNINITIALIZED)
  {
    /* Try to read the attempt counters upon startup */
    /* Only advance to the next state after reading all attempt counters has finished */
    if (Dcm_Dsp_SecurityAccess_AttemptCounters_Initialize() == DCM_E_DONE)
    {
      Dcm_Dsp_SecurityAccess_InitState = DCM_DSP_SECURITYACCESS_INITSTATE_STARTTIMERS;
    }
  }
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
  else /* DCM_DSP_SECURITYACCESS_INITSTATE_STARTTIMERS */
  {
    /* Start the delay timers where necessary */
    Dcm_Dsp_SecurityAccess_StartDelayTimersOnStartup();

    /* Move to the state where the delay timers are handled after attempt counters have been read
     * and the delay timers were started as needed */
    Dcm_Dsp_SecurityAccess_InitState = DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED;
  }
}
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SessionChange ,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  TS_PARAM_UNUSED(OldSession);
  TS_PARAM_UNUSED(NewSession);

  /* Reset security access state handling */
  Dcm_Dsp_SecurityAccess_Reset();

  DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_EXIT();
}

/*==================[internal function definitions]==============================================*/

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.AttemptCounters.Initialize,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_AttemptCounters_Initialize(void)
{
  uint8_least SecLevelIndex = 0U;
  Std_ReturnType InitStatus;

  /* The max. time allowed to read out all the attempt counters was reached */
  if (Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer >=
      DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_READOUT_TIME
     )
  {
    const uint8_least LastSecLevelIndex = Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex;

    P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter;

    /* pointer to configuration information of last security level */
    P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) SecTabEntry;

    /* The assumption is that currently there is an attempt counter read ongoing which needs to be
     * cancelled */
    DCM_PRECONDITION_ASSERT(
      ((LastSecLevelIndex < DCM_DSP_SECURITYACCESS_INVALID_INDEX) &&
       (Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus == DCM_PENDING)
      ),
      DCM_SVCID_SECURITYACCESS);

    AttemptCounter = &Dcm_Dsp_SecurityAccess_DelayTimers[LastSecLevelIndex].AttemptCounter;

    SecTabEntry = &(Dcm_SecurityLevelEntryConfig[LastSecLevelIndex]);

    /* Because the call previously returned a non-final status it must be asynchronous */
    DCM_PRECONDITION_ASSERT(
      ((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
       (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
      ),
      DCM_SVCID_SECURITYACCESS);

    /* !LINKSTO SWS_Dcm_01352.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO SWS_Dcm_01352.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* Call the handler with CANCEL OpStatus */
    (void)SecTabEntry->SecurityGetAttemptCounterFncAsync(DCM_CANCEL, AttemptCounter);

    /* Set all attempt counters that could not be retrieved to their max configured value */
    for (SecLevelIndex = LastSecLevelIndex;
         SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
         SecLevelIndex++
        )
    {
      /* Only care about the security levels that have the external attempt counter handling
       * enabled  */
      if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE)
      {
        /* !LINKSTO SWS_Dcm_01353,1 */
        Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter
          = Dcm_SecurityLevelEntryConfig[SecLevelIndex].SecurityNumAttDelay;

        /* Indicate that the attempt counter was NOT successfully read */
        DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED(SecLevelIndex);
      }
    }

    /* All attempt counters that could not be read by now are left marked as failed
     * (during initiazation) and will be handled as such when starting the delay timers */
  }
  else
  {
    /* Keep track of how much time was used in trying to read all the attempt counters */
    Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer++;

    for (SecLevelIndex = Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex;
         SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
         SecLevelIndex++
        )
    {
      if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE)
      {
        /* pointer to configuration information of the security level */
        P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) SecTabEntry
          = &(Dcm_SecurityLevelEntryConfig[SecLevelIndex]);

        Std_ReturnType GetSecurityAttemptCounterStatus = E_NOT_OK;

        P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter
          = &(Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter);

        /* Call the suitable function according to the interface type configured */
        if((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
           (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
          )
        {
          /* !LINKSTO SWS_Dcm_01154.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01154.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          GetSecurityAttemptCounterStatus = SecTabEntry->SecurityGetAttemptCounterFncAsync(
               Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus,
               AttemptCounter
              );
        }
        else
        {
          /* If the interface is not asynchronous it must be synchronous */
          DCM_PRECONDITION_ASSERT(
              ((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC) ||
               (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER)
              ),
              DCM_SVCID_SECURITYACCESS);

          /* !LINKSTO SWS_Dcm_01154.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01154.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
          GetSecurityAttemptCounterStatus
            = SecTabEntry->SecurityGetAttemptCounterFncSync(AttemptCounter);
        }

        if(GetSecurityAttemptCounterStatus == E_OK)
        {
          /* Indicate that the attempt counter was successfully read */
          DCM_DSP_SECURITYACCESS_CLEAR_ATTEMPTCOUNTER_READ_FAILED(SecLevelIndex);

          /* Call next attempt counter operation with DCM_INITIAL status */
          Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_INITIAL;
        }
        else if ((GetSecurityAttemptCounterStatus == DCM_E_PENDING)
                  &&
                 ((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
                  (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
                 )
                )
        {
          /* If one of the handler returned DCM_E_PENDING, stop processing at this point and
           * resume the handler in the next main function cycle */

          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01351.Recall.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01351.Recall.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_PENDING;

          /* Keep track where to continue in the next cycle */
          Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = SecLevelIndex;

          /* !LINKSTO SWS_Dcm_01351.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01351.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          break;
        }
        else
        {
          /* E_NOT_OK or unexpected return value */

          /* Indicate that the attempt counter was NOT successfully read */
          DCM_DSP_SECURITYACCESS_SET_ATTEMPTCOUNTER_READ_FAILED(SecLevelIndex);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
          if (GetSecurityAttemptCounterStatus != E_NOT_OK)
          {
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.CSAsync,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.CSSync,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.FncAsync,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.FncSync,1 */

            /* report to DET module: unsupported return value */
            DCM_DET_REPORT_ERROR(
                DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER, DCM_E_INTERFACE_RETURN_VALUE);
          }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.AttemptCounter.CSAsync,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.AttemptCounter.CSSync,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.AttemptCounter.FncAsync,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01154.UnexpectedReturnValue.AttemptCounter.FncSync,1 */

          /* !LINKSTO SWS_Dcm_01156.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01156.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01156.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          /* !LINKSTO SWS_Dcm_01156.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

          /* !LINKSTO SWS_Dcm_01156.UnexpectedReturnValue.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01156.UnexpectedReturnValue.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO SWS_Dcm_01156.UnexpectedReturnValue.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          /* !LINKSTO SWS_Dcm_01156.UnexpectedReturnValue.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

          Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter
            = Dcm_SecurityLevelEntryConfig[SecLevelIndex].SecurityNumAttDelay;

          /* Call next get attempt counter operation with DCM_INITIAL status */
          Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_INITIAL;
        }
      }
    }
  }

  /* When the attempt counters of all security levels were initialized */
  if (SecLevelIndex >= DCM_NUM_CONFIGURED_SECURITY_LEVELS)
  {
    /* Initialize the index and operation status variables */
    Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = 0U;

    /* If no attempt counter reading activity is ongoing the opstatus is set to invalid */
    Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_OPSTATUS_INVALID;

    /* Reset the get/set attempt counter timer */
    Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer = 0U;

    /* Inform the caller that the initialization has finished */
    InitStatus = DCM_E_DONE;
  }
  else
  {
    /* Inform the caller that the initialization has not yet finished */
    InitStatus = DCM_E_PENDING;
  }

  return InitStatus;
}

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.DelayTimers.SaveAttemptCounter,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SendKey_SaveAttemptCounter(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType SetAttemptCounterStatus = E_NOT_OK;
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  uint8 AttemptCounter;

  /* Pointer to configuration information of requested security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry
    = &(Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex]);

  const Dcm_SecLevelType NewSecLevel
    = Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].SecLevel;

  /* The attempt counter needs to be saved */
  AttemptCounter
    = Dcm_Dsp_SecurityAccess_DelayTimers[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].AttemptCounter;

  if((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
     (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
    )
  {
    /* !LINKSTO SWS_Dcm_01155.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO SWS_Dcm_01155.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */

    /* If this is the first call to the save attempt counter interface, it needs to have the OpStatus
     * set to DCM_INITIAL. Subsequent calls will use the OpStatus passed to the handler */
    if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING)
    {
      SetAttemptCounterStatus = NewSecTabEntry->SecuritySetAttemptCounterFncAsync(DCM_INITIAL
                                                                                 ,AttemptCounter
                                                                                  );
    }
    else
    {
      SetAttemptCounterStatus = NewSecTabEntry->SecuritySetAttemptCounterFncAsync(OpStatus
                                                                                 ,AttemptCounter
                                                                                  );
    }
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
        ((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC) ||
         (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER)
        ),
        DCM_SVCID_SECURITYACCESS);

    /* !LINKSTO SWS_Dcm_01155.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO SWS_Dcm_01155.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
    SetAttemptCounterStatus
      = NewSecTabEntry->SecuritySetAttemptCounterFncSync(AttemptCounter);
  }

  /* Advance to next SendKey state after the attempt counter saving initialization was performed */
  if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING)
  {
    Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING;
  }

  if (OpStatus != DCM_CANCEL)
  {
    if(((SetAttemptCounterStatus == DCM_E_PENDING) ||
        (SetAttemptCounterStatus == DCM_E_FORCE_RCRRP)
       ) &&
       ((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
        (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
       )
      )
    {
      /* If one of the handler retruned DCM_E_PENDING or DCM_E_FORCE_RCRRP
       * stop procesing at this point and resume the handler in the next main function cycle */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.Recall.PENDING.ASYNCH_CS,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.Recall.PENDING.ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.Recall.FORCE_RCRRP.ASYNCH_CS,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.Recall.FORCE_RCRRP.ASYNCH_FNC,1 */
      ServiceProcessStatus = SetAttemptCounterStatus;
    }
    else
    {
      /* Saving the attempt counter succeeded */
      if (SetAttemptCounterStatus == E_OK)
      {
        /* Generate a positive or negative response according to the situation that lead to
         * saving the attempt counter */
        if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
            DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS
           )
        {
          /* ENTER critical section */
          SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

          /* !LINKSTO Dcm.EB.SecurityLevelHandling.SecurityAccess.SendKey.AttemptCounterEnabled.ResetToDefaultSession.DiscardSecurityLevel,1 */
          /* only set security level when security state is not reset */
          if (DCM_SEC_IDLE != Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel)
          {
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.SecurityLevel.ASYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.SecurityLevel.SYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.SecurityLevel.ASYNCH_FNC,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.SecurityLevel.SYNCH_FNC,1 */

            /* Set the new security level */
            Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(NewSecLevel);
          }

          /* LEAVE critical section */
          SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.PosRes.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.PosRes.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.PosRes.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_OK.PosRes.SYNCH_FNC,1 */
          *Nrc = DCM_E_POSITIVERESPONSE;
        }
        else if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
                 DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY
                )
        {
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_OK.NRC.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_OK.NRC.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_OK.NRC.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_OK.NRC.SYNCH_FNC,1 */
          *Nrc = DCM_E_INVALIDKEY;
        }
        else if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
                 DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS
                )
        {
          /* !LINKSTO Dcm324_Implicit3,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_OK.NRC.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_OK.NRC.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_OK.NRC.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_OK.NRC.SYNCH_FNC,1 */
          *Nrc = DCM_E_EXCEEDNUMBEROFATTEMPTS;
        }
        /* CHECK: NOPARSE */
        else
        {
          /* Must not be reached */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SECURITYACCESS);
        }
        /* CHECK: PARSE */

        ServiceProcessStatus = DCM_E_DONE;
      }
      else
      {
        if (SetAttemptCounterStatus == E_NOT_OK)
        {
          /* A negative response will be returned but the security level will remain unmodified */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.SecurityLevel.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.SecurityLevel.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.SecurityLevel.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.SecurityLevel.SYNCH_FNC,1 */

          if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
                DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS
            )
          {
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_FNC,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_FNC,1 */
            *Nrc = DCM_E_CONDITIONSNOTCORRECT;
          }
          else if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
                   DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS
                  )
          {
            /* !LINKSTO Dcm324_Implicit3,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_FNC,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_FNC,1 */
            *Nrc = DCM_E_EXCEEDNUMBEROFATTEMPTS;
          }
          else if (Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp ==
                   DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY
                  )
          {
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_CS,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_NOT_OK.NRC.ASYNCH_FNC,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.E_NOT_OK.NRC.SYNCH_FNC,1 */
            *Nrc = DCM_E_INVALIDKEY;
          }
          /* CHECK: NOPARSE */
          else
          {
            /* Defensive branch. Code must not be reached */
            DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SECURITYACCESS);
          }
          /* CHECK: PARSE */
        }
        else /* Unexpected return value */
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
          /* report to DET module: unsupported return value */
          DCM_DET_REPORT_ERROR(
              DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

          /* A negative response will be returned but the security level will remain unmodified */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.SecurityLevel.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.SecurityLevel.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.SecurityLevel.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.SecurityLevel.SYNCH_FNC,1 */

          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_FNC,1 */

          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_00660.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_FNC,1 */

          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.NRC.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.CompareKey.E_OK.SetSecurityAttemptCounter.Unknown.NRC.SYNCH_FNC,1 */

          *Nrc = DCM_E_CONDITIONSNOTCORRECT;
        }

        ServiceProcessStatus = DCM_E_DONE;
      }

      /* Reset security access state handling */
      Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;

      Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp = DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_NONE;

      /* Done saving the attempt counter. Reset the state to 'compare key' */
      Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY;
    }
  }
  else
  {
    /* Cancelled saving the attempt counter. Reset the state to 'compare key' */
    Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY;
    ServiceProcessStatus = DCM_E_DONE;
  }

  return ServiceProcessStatus;
}

#if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SaveResetAttemptCounter(
  uint8 SecLevelIndex)
{
  Std_ReturnType SetAttemptCounterStatus = E_NOT_OK;

  /* pointer to configuration information of the security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) SecTabEntry
    = &(Dcm_SecurityLevelEntryConfig[SecLevelIndex]);

  /* The max. timout value for writing an attempt counter must never be surpassed */
  DCM_PRECONDITION_ASSERT((Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer <=
                           DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME
                           )
                           ,DCM_SVCID_SECURITYACCESS
                         );

  /* Max nr. of calls reached? */
  if (Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer ==
      DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME
     )
  {
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.CANCEL.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.CANCEL.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_CANCEL;
  }

  /* Call the suitable function according to the interface type configured */
  if((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
     (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
    )
  {
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    SetAttemptCounterStatus = SecTabEntry->SecuritySetAttemptCounterFncAsync(
                                             Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus
                                            ,0U
                                            );
  }
  else
  {
    /* If the interface is not asynchronous it must be synchronous */
    DCM_PRECONDITION_ASSERT(
        ((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC) ||
         (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER)
        ),
        DCM_SVCID_SECURITYACCESS);

    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
    SetAttemptCounterStatus = SecTabEntry->SecuritySetAttemptCounterFncSync(0U);
  }

  if (Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus != DCM_CANCEL)
  {
    if (((SetAttemptCounterStatus == DCM_E_PENDING) ||
         (SetAttemptCounterStatus == DCM_E_FORCE_RCRRP)
        ) &&
        ((SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
         (SecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
        )
       )
    {
      /* If the set operation did not finish, try again in next cycle */
      if (SetAttemptCounterStatus == DCM_E_PENDING)
      {
        /* The security level stays locked */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.SecLevelLocked.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.SecLevelLocked.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */

        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.PENDING.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.PENDING.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
        Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_PENDING;
      }
      else if (SetAttemptCounterStatus == DCM_E_FORCE_RCRRP)
      {
        /* The security level stays locked */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.SecLevelLocked.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.SecLevelLocked.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */

        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.FORCE_RCRRP.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.FORCE_RCRRP.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
        Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_FORCE_RCRRP_OK;
      }
      /* CHECK: NOPARSE */
      else
      {
        /* Must not be reached */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SECURITYACCESS);
      }
      /* CHECK: PARSE */

      Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer++;
    }
    else if (SetAttemptCounterStatus == DCM_E_OK)
    {
      Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_OPSTATUS_INVALID;
      Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer = 0U;

      /* Unlock the security level */
      DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL(SecLevelIndex);
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if (SetAttemptCounterStatus != E_NOT_OK)
      {
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.SWS_Dcm_01155.UnexpectedReturnValue.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

          /* report to DET module: unsupported return value */
          DCM_DET_REPORT_ERROR(
            DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_OPSTATUS_INVALID;
      Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer = 0U;

      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Error.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Error.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Error.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Error.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
      /* Unlock the security level */
      DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL(SecLevelIndex);

      /* Operation failed, also return E_NOT_OK if an unexpected value was returned */
      SetAttemptCounterStatus = E_NOT_OK;
    }
  }
  else
  {
    /* Operation was cancelled */
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Cancel.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.ResetAttemptCounterOnTimeout.True.SetSecurityAttemptCounter.UnlockSecLevel.On.Cancel.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* Unlock the security level */
    DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL(SecLevelIndex);
    Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_OPSTATUS_INVALID;
    Dcm_Dsp_SecurityAccess_GetSetAttemptCountersTimer = 0U;
    SetAttemptCounterStatus = E_NOT_OK;
  }

  return SetAttemptCounterStatus;
}
#endif /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.AttemptCounter.StartDelayTimerOnStartup,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_StartDelayTimersOnStartup(void)
{
  uint8_least SecLevelIndex = 0U;
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
  uint32 UnifiedDelayTimerValue = 0U;
  boolean StartUnifiedDelayTimer = FALSE;
  boolean UnifiedDelayTimerInfinite = FALSE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
  uint32 DelayTimerValue = 0U;
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */

  /* Start the delay timers as needed */
  /* Deviation TASKING-1 <+2> */
  for (SecLevelIndex = 0U;
      SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
      SecLevelIndex++
      )
  {
    /* If attempt counter override on boot is enabled, the delay timer will be started anyway */
    if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBootOverride == TRUE)
    {
      /* !LINKSTO Dcm.Dsn.IB.SecurityAccess.AttemptCounter.StartDelayTimerOnStartup.FullOverride,1 */
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      /* See if the attempt counter could be retrieved */
      if (DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED(SecLevelIndex))
      {
#if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON)
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.Infinite.FullOverride.UnifiedTimers,1 */
        UnifiedDelayTimerInfinite = TRUE;
        StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.Infinite.FullOverride.NonUnifiedTimers,1 */
        /* Start the delay timer and mark the security level as locked */
        Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex, 0U, TRUE);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_OFF) */
#else /* #if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON) */
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.NoInfinite.FullOverride.UnifiedTimers,1 */
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot > UnifiedDelayTimerValue)
        {
          UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
        }
        StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.NoInfinite.FullOverride.NonUnifiedTimers,1 */
        /* Start the delay timer and mark the security level as locked */
        Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex,
                                        Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot,
                                        FALSE);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON) */
      }
      else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
      {
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.Success.FullOverride.UnifiedTimers,1 */
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot > UnifiedDelayTimerValue)
        {
          UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
        }
        StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* Start the delay timer with the configured value for DelayTimeOnBoot */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.Success.FullOverride.NonUnifiedTimers,1 */
        /* Start the delay timer and mark the security level as locked */
        Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex,
                                        Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot,
                                        FALSE);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_OFF) */
      }
    }
    else
    {
      /* !LINKSTO Dcm.Dsn.IB.SecurityAccess.AttemptCounter.StartDelayTimerOnStartup.NoOverride,1 */
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      /* See if the attempt counter could be retrieved */
      if (DCM_DSP_SECURITYACCESS_ATTEMPTCOUNTER_READ_FAILED(SecLevelIndex))
      {
#if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON)
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.Infinite.NoOverride.UnifiedTimers,1 */
        UnifiedDelayTimerInfinite = TRUE;
        StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.Infinite.NoOverride.NonUnifiedTimers,1 */
        /* Start the delay timer and mark the security level as locked */
        Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex, 0U, TRUE);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_OFF) */
#else /* #if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON) */
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.NoInfinite.NoOverride.UnifiedTimers,1 */
        /* Use the largest value of DelayTime and DelayTimeOnBoot but do not overwrite a larger
         * unified delay timer value*/
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot > UnifiedDelayTimerValue)
        {
          UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
        }
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime > UnifiedDelayTimerValue)
        {
          UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime;
        }
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeInfinite == TRUE)
        {
          UnifiedDelayTimerInfinite = TRUE;
        }
        StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.NoSuccess.NoInfinite.NoOverride.NonUnifiedTimers,1 */
        /* Use the largest value of DelayTime and DelayTimeOnBoot */
        DelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
        if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime > DelayTimerValue)
        {
          DelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime;
        }

        /* Start the delay timer and mark the security level as locked */
        Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex, DelayTimerValue,
                                     Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeInfinite);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_OFF) */
#endif /* #if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY == STD_ON) */
      }
      else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
      {
        if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter >=
            Dcm_SecurityLevelEntryConfig[SecLevelIndex].SecurityNumAttDelay
           )
        {
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.Success.NoOverride.UnifiedTimers,1 */
          /* Use the largest value of DelayTime and DelayTimeOnBoot but do not overwrite a larger
           * unified delay timer value*/
          if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot > UnifiedDelayTimerValue)
          {
            UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
          }
          if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime > UnifiedDelayTimerValue)
          {
            UnifiedDelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime;
          }
          if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeInfinite == TRUE)
          {
            UnifiedDelayTimerInfinite = TRUE;
          }
          StartUnifiedDelayTimer = TRUE;
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
          /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01355.Success.NoOverride.NonUnifiedTimers,1 */
          /* Use the largest value of DelayTime and DelayTimeOnBoot */
          DelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeOnBoot;
          if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime > DelayTimerValue)
          {
            DelayTimerValue = Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTime;
          }

          /* Start the delay timer and mark the security level as locked */
          Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex, DelayTimerValue,
                                     Dcm_SecurityLevelEntryConfig[SecLevelIndex].DelayTimeInfinite);
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_OFF) */
        }
      }
    }
  }

  /* If unified timer is enabled, start all timers with its value */
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
  /* Only start unified timers and lock the levels if needed */
  if (StartUnifiedDelayTimer == TRUE)
  {
    for (SecLevelIndex = 0U;
        SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
        SecLevelIndex++
        )
    {
      /* Start the delay timer and mark the security level as locked */
      Dcm_Dsp_SecurityAccess_StartDelayTimer((uint8)SecLevelIndex, UnifiedDelayTimerValue,
                                             UnifiedDelayTimerInfinite);
    }
  }
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
}

#if ((DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) &&\
    (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON))
STATIC FUNC(uint8_least, DCM_CODE) Dcm_Dsp_SecurityAccess_FindSavingDelayTimer(void)
{
  uint8_least SecLevelIndex = 0U;

  /* Check if there is a delay timer that is currently being saved */
  SecLevelIndex = 0U;
  while (SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS)
  {
    /* Check if the security level is locked and the delay finite */
    if (DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED(SecLevelIndex) &&
        (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay == FALSE) &&
        (Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE) &&
        (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer == 0U)
       )
    {
      break;
    }
    SecLevelIndex++;
  }
  return SecLevelIndex;
}
#endif /* #if ((DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) &&\
    (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)) */

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.AttemptCounter.StartDelayTimer,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_StartDelayTimer(uint8 SecLevelIndex,
                                                                   uint32 Value,
                                                                   boolean InfiniteDelay)
{
#if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE)
  {
    /* Set the requested delay timer value */
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer = Value;

    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01356_ResetExternal,1 */
    if ((Value == 0U) && (InfiniteDelay == FALSE))
    {
      if (0U != Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter)
      {
        /* A level will only need locking if its AttemptCounter has been reset. AttemptCounters
           which reset from zero to zero are not uselessly saved to non-volatile memory. */

        if (Dcm_Dsp_SecurityAccess_FindSavingDelayTimer() < DCM_NUM_CONFIGURED_SECURITY_LEVELS)
        {
          /* If there is any timer which is currently being saved, the timer to be set
           * receives the value 1 (which means that as soon as the current saving timer
           * is done, the timer which is now being set to 0 is going to expire)
           */
          Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer = 1U;

          /* Mark the security level as locked */
          DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevelIndex);
        }
        else
        {
          Std_ReturnType SetAttemptCounterStatus;

          Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter = 0U;

          /* For the first call, the OpStatus needs to be DCM_INITIAL */
          Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_INITIAL;

          /* Call the function to save the attempt counter */
          SetAttemptCounterStatus = Dcm_Dsp_SecurityAccess_SaveResetAttemptCounter(SecLevelIndex);
          if ((SetAttemptCounterStatus == DCM_E_PENDING) ||
              (SetAttemptCounterStatus == DCM_E_FORCE_RCRRP)
             )
          {
            /* Keep track of the security level from where to continue during next cycle */
            Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = SecLevelIndex;

            /* Mark the security level as locked */
            DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevelIndex);
          }
        }
      }
    }
    else
    {
      /* If the security level was configured for infinite locking, apply it */
      Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay = InfiniteDelay;

      /* Mark the security level as locked */
      DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevelIndex);
    }
  }
  else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
  {
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01356_ResetNoExternal,1 */
    if ((Value > 0U) || (InfiniteDelay == TRUE))
    {
      Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer = Value;

      /* If the security level was configured for infinite locking, apply it */
      Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay = InfiniteDelay;

      /* Mark the security level as locked */
      DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevelIndex);
    }
  }
#else /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */
  /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01356_NoReset,1 */
  if ((Value > 0U) || (InfiniteDelay == TRUE))
  {
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer = Value;

    /* If the security level was configured for infinite locking, apply it */
    Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay = InfiniteDelay;

    /* Mark the security level as locked */
    DCM_DSP_SECURITYACCESS_LOCK_SECURITY_LEVEL(SecLevelIndex);
  }
#endif /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */
}

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.DelayTimers.Decrement,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_DelayTimers_Decrement(void)
{
  uint8_least SecLevelIndex;
  uint8_least StartFromIndex = 0U;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
#if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)
  boolean SaveCounterNeeded = FALSE;
#endif /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */

  StartFromIndex = Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

  /* Do not bother to search for delay timers to be decremented if there are none */
  if (Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked != 0U)
  {
    for (SecLevelIndex = StartFromIndex;
        SecLevelIndex < DCM_NUM_CONFIGURED_SECURITY_LEVELS;
        SecLevelIndex++
        )
    {
      /* Check if the security level is locked and the delay finite */
      if (DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED(SecLevelIndex) &&
          (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].InfiniteDelay == FALSE)
         )
      {
        /* Decrement the delay timer if it didn't yet expire */
        if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer > 0U)
        {
          /* Decrement the delay timer and see if it expires */
          Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer--;
          if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer == 0U)
          {
#if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON)
            /* The attempt counter is only  reset if DcmDspSecurityResetAttemptCounterOnTimeout
             * is configured to TRUE */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01357.ExpiredSecurityDelayTimer.ResetAttemptCounterOnTimeout.False,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01357.ExpiredSecurityDelayTimer.ResetAttemptCounterOnTimeout.True.DcmDspSecurityUnifiedDelayTimer.True,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01357.ExpiredSecurityDelayTimer.ResetAttemptCounterOnTimeout.True.DcmDspSecurityUnifiedDelayTimer.False,1 */
            if (0U != Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter)
            {
              Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter = 0U;
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
              /* Only save AttemptCounters which have changed their value. AttemptCounters
                 which reset from 0 to 0 are not uselessly saved to non-volatile memory. */
              SaveCounterNeeded = TRUE;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
            }

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
            /* Check if external handling of the attempt counter of this security level is enabled and
               the AttemptCounter needs to be saved. */
            if ((Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE) &&
                (TRUE == SaveCounterNeeded)
               )
            {
              /* For the first call, the OpStatus needs to be DCM_INITIAL */
              Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus = DCM_INITIAL;
            }
            else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) */
            {
              /* The delay timer expired and therefore the security level is no longer locked */
              DCM_DSP_SECURITYACCESS_UNLOCK_SECURITY_LEVEL(SecLevelIndex);
            }
          }
        }

#if ((DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) &&\
     (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON))
        /* Initiate saving the attempt counter if the delay timer reached 0 and external handling
           is enabled of the attempt counter is enabled for this security level and
           the counter needs to be saved. A counter needs to be saved only if
           Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus is set to a valid value.
         */
        if ((Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE) &&
            (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].DelayTimer == 0U) &&
            (Dcm_Dsp_SecurityAccess_GetSetAttemptCounter_OpStatus != DCM_OPSTATUS_INVALID)
           )
        {
          /* Call the function to save the attempt counter */
          Std_ReturnType SetAttemptCounterStatus
            = Dcm_Dsp_SecurityAccess_SaveResetAttemptCounter((uint8)SecLevelIndex);

          if ((SetAttemptCounterStatus == DCM_E_PENDING) ||
              (SetAttemptCounterStatus == DCM_E_FORCE_RCRRP)
             )
          {
            /* Keep track of the security level from where to continue during next cycle */
            Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = SecLevelIndex;

            /* Processing will continue in the next cycle */
            break;
          }
          else
          {
            Dcm_Dsp_SecurityAccess_ContinueAtSecurityLevelIndex = 0U;
          }
        }
#endif /* #if ((DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT == STD_ON) &&\
               (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)) */
      }
    }
  }
}
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{

  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
  /* state of security access */
  Dcm_SecStateType SecState = DCM_SEC_SEND_KEY;

  /* pointer to configuration information of requested security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry = NULL_PTR;
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */

  /* negative response code */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;

  /* check request length */
  if (MsgContextPtr->reqDataLen < 1U)
  {
    /* !LINKSTO Dcm.EB.SecurityAccess.NRC0x13,1 */
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
    /* loop variable */
    uint8 SecIdx;

    /* Dcm338: get security level by computing the next smallest odd number */
    Dcm_SecLevelType NewSecLevel = DCM_SECURITYACCESS_GET_SECURITY_LEVEL(MsgContextPtr->reqData[0]);

    /* store type of request */
    SecState = DCM_SECURITYACCESS_GET_REQUEST_TYPE(MsgContextPtr->reqData[0]);

    /* Check if requested security level is supported */
    /* Deviation TASKING-1 */
    for (SecIdx = 0U; SecIdx < DCM_NUM_CONFIGURED_SECURITY_LEVELS; ++SecIdx)
    {
      if (Dcm_SecurityLevelEntryConfig[SecIdx].SecLevel == NewSecLevel)
      {
        NewSecTabEntry = &(Dcm_SecurityLevelEntryConfig[SecIdx]);
        Dcm_Dsp_SecurityAccess_ReqSecLevelIndex = SecIdx;
        break;
      }
    }
    /* requested security level not found */
    /* !LINKSTO Dcm321,1 */
    if (SecIdx >= DCM_NUM_CONFIGURED_SECURITY_LEVELS)
#else
    TS_PARAM_UNUSED(OpStatus);
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */
    {
      Nrc = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
    else
    {
      /* Seed was not requested */

      /* !LINKSTO Dcm324_Implicit1,1 */
      if ((SecState == DCM_SEC_SEND_KEY) &&
          DCM_SECURITYACCESS_SEQUENCE_IS_INCORRECT(MsgContextPtr->reqData[0]
                                                  ,Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel)
         )
      {
        /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.SecurityLevelActive,1 */
        Nrc = DCM_E_REQUESTSEQUENCEERROR;
      }
    }
  }

  /* Check failed attempts, delay counter and seed/key size */
  if (Nrc == DCM_E_POSITIVERESPONSE)
  {
    /* Check length of key */
    if ((SecState == DCM_SEC_SEND_KEY) &&
             (MsgContextPtr->reqDataLen !=
             ((Dcm_BufferSizeType)(NewSecTabEntry->KeySize + (Dcm_BufferSizeType)1U))))
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.NRC0x13,1 */
      Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    /* Check length of securityAccessDataRecord in the seed request */
    else
    {
      if ((SecState == DCM_SEC_REQ_SEED) &&
              (MsgContextPtr->reqDataLen !=
              ((Dcm_BufferSizeType)(NewSecTabEntry->ADRSize + (Dcm_BufferSizeType)1U))))
      {
        /* !LINKSTO Dcm.EB.SecurityAccess.NRC0x13,1 */
        Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
      }
    }
  }

  /* Preconditions OK */
  if (Nrc == DCM_E_POSITIVERESPONSE)
  {
    /* setup response data (value of security level) */
    MsgContextPtr->resData[0U] = MsgContextPtr->reqData[0];

    /* assign the data from msgcontext for service to that of sub-function.*/
    /* To avoid MsgContextPtr change when OpStatus is DCM_PENDING */
    /* point the reqData behind the sub-function */
    MsgContextPtr->reqData = &MsgContextPtr->reqData[1U];
    /* decrement the reqData length as sub-function is removed */
    MsgContextPtr->reqDataLen = MsgContextPtr->reqDataLen - 1U;

    ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SvcCont( OpStatus, MsgContextPtr);
  }
  else
#else /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */
  }
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */
  {
    Dcm_ExternalSetNegResponse(MsgContextPtr, Nrc);
    ServiceProcessStatus = DCM_E_DONE;

    /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.InvalidateSeed,2 */
#if (DCM_SECURITYACCESS_STRICT == STD_OFF)
    /* !LINKSTO Dcm.EB.SecurityAccess.SubFunctionNotSupported.AllowOutOfSequenceRequests,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.AllowOutOfSequenceRequests,1 */
    if ((Nrc != DCM_E_SUBFUNCTIONNOTSUPPORTED) && (Nrc != DCM_E_REQUESTSEQUENCEERROR))
#endif /*(DCM_SECURITYACCESS_STRICT == STD_OFF)*/
    {
      Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;
    }
  }

  return ServiceProcessStatus;
}

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 curProtocolId = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      MsgContextPtr->dcmRxPduId].MainConnectionIndex].ProtocolIndex;
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */
  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(curProtocolId);

  /* State of security access */
  Dcm_SecStateType SecState = DCM_SECURITYACCESS_GET_REQUEST_TYPE(curHsmInfo->subServiceId);
  /* negative reponse code */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;

  /* Request Seed */
  if (SecState == DCM_SEC_REQ_SEED)
  {
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
    /* While not all attempt counters are retrieved respond to any "request seed" with Nrc 0x22 */
    if (Dcm_Dsp_SecurityAccess_InitState != DCM_DSP_SECURITYACCESS_INITSTATE_INITIALIZED)
    {
      /* !LINKSTO SWS_Dcm_01354,1 */
      Nrc = DCM_E_CONDITIONSNOTCORRECT;
    }
    /* Check if the security level is locked */
#if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON)
    else if (Dcm_Dsp_SecurityAccess_SecurityLevelsBlocked != 0U)
#else /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
    else if(DCM_DSP_SECURITYACCESS_IS_SECURITY_LEVEL_LOCKED(Dcm_Dsp_SecurityAccess_ReqSecLevelIndex))
#endif /* #if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED == STD_ON) */
    {
      /* !LINKSTO Dcm324_Implicit2,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01350.UnifiedDelayTimer.False,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01350.UnifiedDelayTimer.True,1 */
      Nrc = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
    }
    else
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
    {
      ServiceProcessStatus = Dcm_Dsp_SecurityAccess_RequestSeed_SSvcH(OpStatus, &Nrc, MsgContextPtr);
    }
  }
  /* Authenticate the key */
  else
  {
    ServiceProcessStatus = Dcm_Dsp_SecurityAccess_SendKey_SSvcH(OpStatus, &Nrc, MsgContextPtr);
  }

#if ((DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)&&\
    (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON))
  /* Wait for eventual ongoing save attempt counter operation to finish */
  if ((Dcm_Dsp_SecurityAccess_SendKey_State != DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING) &&
      (Dcm_Dsp_SecurityAccess_SendKey_State != DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING)
     )
#endif /* #if ((DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)&&\
              (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)) */
  {
    if (DCM_E_POSITIVERESPONSE != Nrc)
    {
      /* With AUTOSAR 4.2.1 the key compare function can set any Nrc if E_NOT_OK is returned.
       * Dcm_ExternalSetNegResponse() does not allow any Nrc to be set, so set the Nrc directly.
       * The same is done when dealing with failing mode rules. */
      DCM_DSP_SECURITYACCESS_ENTER_EXCLUSIVE_AREA();
      curHsmInfo->negativeResponseCode = Nrc;
      DCM_DSP_SECURITYACCESS_EXIT_EXCLUSIVE_AREA();

      ServiceProcessStatus = DCM_E_DONE;
      Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;
    }
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.RequestSeed,2 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_RequestSeed_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  /* loop variable */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType GetSeedStatus = E_OK;

  /* pointer to configuration information of requested security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry
    = &(Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex]);

  const Dcm_SecLevelType NewSecLevel
    = Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].SecLevel;

  const Dcm_SecLevelType ActiveSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

  /* check if requested security level is already active */
  if ((NewSecLevel != DCM_SEC_LEV_LOCKED) && (NewSecLevel == ActiveSecLevel))
  {
    /* !LINKSTO Dcm323,1 */
    /* setup response data (if seed value is 0U level is already unlocked) */
    TS_MemSet(&(MsgContextPtr->resData[1U]),0U,NewSecTabEntry->SeedSize);
    MsgContextPtr->resDataLen = (Dcm_MsgLenType)(NewSecTabEntry->SeedSize + (Dcm_MsgLenType)1U);

    /* !LINKSTO Dcm.EB.SecurityAccess.RequestSeedForLevelUnlocked.InvalidateSeed,1 */
    /* reset security access state handling */
    Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;

    ServiceProcessStatus = DCM_E_DONE;
  }
  else
  {
#if (DCM_NUM_PROTOCOL > 1U)
    /* Get current protocol id */
    uint8 curProtocolId = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      MsgContextPtr->dcmRxPduId].MainConnectionIndex].ProtocolIndex;
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

    /* get information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo
      = &DCM_HSM_INFO(curProtocolId);

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* !LINKSTO Dcm.EB.SecurityLevelHandling.SecurityAccess.RequestSeed.ResetToDefaultSession.DiscardSecurityLevel,1 */
    /* only update security state when security state is not reset by session switch */
    if (FALSE == Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(curHsmInfo->serviceId))
    {
      /* set security state to requested security level */
      Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel
        = (Dcm_SecLevelType)(DCM_SECURITYACCESS_GET_SECURITY_LEVEL(curHsmInfo->subServiceId));
    }

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* !LINKSTO Dcm.SecurityAccess.GetSeed,1 */
    /* Process getSeed request */
    GetSeedStatus = Dcm_Dsp_SecurityAccess_GetSeed_Operation(OpStatus, Nrc, NewSecTabEntry
                                                             ,MsgContextPtr);
    if (GetSeedStatus == DCM_E_OK)
    {
      /* setup response data */
      MsgContextPtr->resDataLen += (Dcm_MsgLenType)(NewSecTabEntry->SeedSize + (Dcm_MsgLenType)1U);
      ServiceProcessStatus = DCM_E_DONE;
    }
    else if (GetSeedStatus == DCM_E_PENDING)
    {
      /* Pending status should be returned to the service handler. */
      ServiceProcessStatus = DCM_E_PENDING;
    }
    else
    {
      /* No operation shall be done here, since error code is captured
       * in 'Nrc' and negative response is sent from the service handler. */

      /* GetSeedStatus might not be DCM_E_NOT_OK. It might have some unsupported value
       * in which case a Det and GeneralReject were issued.
       * But if it's DCM_E_NOT_OK (but not because of a cancellation) and the operation did not set
       * a Nrc, send ConditionsNotCorrect */

      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeedFunc.NRC0x00,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeedCS.NRC0x00,1 */
      if ((OpStatus != DCM_CANCEL) && (GetSeedStatus == DCM_E_NOT_OK) && (*Nrc == 0x00))
      {
        *Nrc = DCM_E_CONDITIONSNOTCORRECT;
      }

      ServiceProcessStatus = DCM_E_DONE;
    }
  }

  return ServiceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_GetSeed_Operation(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  Std_ReturnType GetSeedStatus = E_NOT_OK;
  Std_ReturnType RetVal = DCM_E_NOT_OK;

  if (NewSecTabEntry->ADRSize > 0U)
  {
    /* Point to the SecurityAccessDataRecord from the request message,
       as sub-function removed before calling sub-service handler */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) SecurityAccessDataRecord = &(MsgContextPtr->reqData[0U]);

    /* For  USE_DATA_SYNCH_FNC, USE_DATA_ASYNCH_FNC,USE_DATA_SYNCH_CLIENT_SERVER,
       USE_DATA_ASYNCH_CLIENT_SERVER call the DcmDspSecurityGetSeedFnc operation when
       DcmDspSecurityADRSize is present */
    if((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
       (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER))
    {
      /* !LINKSTO SWS_Dcm_00659.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO SWS_Dcm_00659.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeed.DcmDspSecurityGetSeedFnc,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.RTE.RequestSeed,1 */
      /* Calling the GetSeed operation  */
      GetSeedStatus = NewSecTabEntry->SecurityGetSeedFncADRAsync(SecurityAccessDataRecord,
                                             OpStatus, &(MsgContextPtr->resData[1U]), Nrc);
      if(OpStatus != DCM_CANCEL)
      {
        if(GetSeedStatus != E_OK)
        {
          if((GetSeedStatus == DCM_E_NO_DATA) || (GetSeedStatus == DCM_E_PENDING))
          {
            RetVal = DCM_E_PENDING;
          }
          else if(GetSeedStatus == E_NOT_OK)
          {
            /* !LINKSTO Dcm659,1 */
            /* In this case the Nrc code is taken from the GetSeed operation */
          }
          else
          {
            *Nrc = DCM_E_GENERALREJECT;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* report to DET module: unsupported return value */
            DCM_DET_REPORT_ERROR(
              DCM_SVCID_SECURITYGETSEEDFNCADRASYN, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          }
        }
        else
        {
          /* If the GetSeed returns DCM_E_OK, set the Nrc to DCM_E_POSITIVERESPONSE */
          *Nrc = DCM_E_POSITIVERESPONSE;
          RetVal = DCM_E_OK;
        }
      }
      else
      {
        RetVal = DCM_E_NOT_OK;
      }
    }
    else
    {
      /* SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC or
         SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER */

      /* !LINKSTO SWS_Dcm_00659.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO SWS_Dcm_00659.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeed.DcmDspSecurityGetSeedFnc,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.RTE.RequestSeed,1 */
      /* Calling the GetSeed operation */
      GetSeedStatus = NewSecTabEntry->SecurityGetSeedFncADRSync(SecurityAccessDataRecord,
                                                  &(MsgContextPtr->resData[1U]), Nrc);
      if(GetSeedStatus != E_OK)
      {
        if(GetSeedStatus != E_NOT_OK)
        {
          /* In case GetSeedStatus == DCM_E_NOT_OK Nrc will be set from the GetSeed operation*/
          *Nrc = DCM_E_GENERALREJECT;
          RetVal = GetSeedStatus;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* report to DET module: unsupported return value */
          DCM_DET_REPORT_ERROR(DCM_SVCID_SECURITYGETSEEDFNCADRSYNC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
      }
      else
      {
        /* If the GetSeed returns E_OK, set the Nrc to DCM_E_POSITIVERESPONSE */
        *Nrc = DCM_E_POSITIVERESPONSE;
        RetVal = DCM_E_OK;
      }
    }
  }
  else
  {
    /* For  USE_DATA_SYNCH_FNC, USE_DATA_ASYNCH_FNC,USE_DATA_SYNCH_CLIENT_SERVER,
       USE_DATA_ASYNCH_CLIENT_SERVER call the DcmDspSecurityGetSeedFnc operation when
       DcmDspSecurityADRSize is not present */
    if((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
       (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER))
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeed.DcmDspSecurityGetSeedFnc,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.RTE.RequestSeed,1 */
      /* Calling the GetSeed operation */
      GetSeedStatus = NewSecTabEntry->SecurityGetSeedFncAsync(OpStatus,
                                                           &(MsgContextPtr->resData[1U]), Nrc);
      if(OpStatus != DCM_CANCEL)
      {
        if(GetSeedStatus != E_OK)
        {
          if((GetSeedStatus == DCM_E_NO_DATA) || (GetSeedStatus == DCM_E_PENDING))
          {
            RetVal = DCM_E_PENDING;
          }
          else if(GetSeedStatus == E_NOT_OK)
          {
            /* !LINKSTO Dcm659,1 */
            /* In this case the Nrc code is taken from the GetSeed operation */
          }
          else
          {
            *Nrc = DCM_E_GENERALREJECT;
            RetVal = GetSeedStatus;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* report to DET module: unsupported return value */
            DCM_DET_REPORT_ERROR(DCM_SVCID_SECURITYGETSEEDFNCASYNC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          }
        }
        else
        {
          /* If the GetSeed returns E_OK , set the Nrc to DCM_E_POSITIVERESPONSE */
          *Nrc = DCM_E_POSITIVERESPONSE;
          RetVal = DCM_E_OK;
        }
      }
      else
      {
        RetVal = DCM_E_NOT_OK;
      }
    }
    else
    {
      /* SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC or
         SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER */

      /* !LINKSTO Dcm.EB.SecurityAccess.GetSeed.DcmDspSecurityGetSeedFnc,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.RTE.RequestSeed,1 */
      /* Calling the GetSeed operation */
      GetSeedStatus = NewSecTabEntry->SecurityGetSeedFncSync(&(MsgContextPtr->resData[1U]), Nrc);
      if(GetSeedStatus != E_OK)
      {
        if(GetSeedStatus != E_NOT_OK)
        {
          /* In case GetSeedStatus == E_NOT_OK Nrc will be set from the GetSeed operation*/
          *Nrc = DCM_E_GENERALREJECT;
          RetVal = GetSeedStatus;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* report to DET module: unsupported return value */
          DCM_DET_REPORT_ERROR(DCM_SVCID_SECURITYGETSEEDFNCSYNC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
      }
      else
      {
        /* If the GetSeed read returns E_OK , set the Nrc to DCM_E_POSITIVERESPONSE */
        *Nrc = DCM_E_POSITIVERESPONSE;
        RetVal = DCM_E_OK;
      }
    }
  }

  return RetVal;
}

#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SendKey.Treat.Attempt.Counters,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_CompareKey_TreatAttemptCounters(
  uint8 SecLevelIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc)
{
  /* pointer to configuration information of requested security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry
    = &(Dcm_SecurityLevelEntryConfig[SecLevelIndex]);

#if (DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT == STD_OFF)
  /* Only increment the counter up to the configured limit */
  if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter <
      NewSecTabEntry->SecurityNumAttDelay
     )
#endif /* #if (DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT == STD_OFF) */
  {
    /* Increment and store the number of failed attempts. Take care to avoid overflow */
    if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter <
        DCM_DSP_SECURITYACCESS_ATTEMPT_COUNTER_MAX_VALUE
       )
    {
      /* !LINKSTO SWS_Dcm_01397.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO SWS_Dcm_01397.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO SWS_Dcm_01397.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO SWS_Dcm_01397.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
      Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter++;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      /* If the security level provides an interface to save the attempt counter, do so */
      if (Dcm_SecurityLevelEntryConfig[SecLevelIndex].ExternalAttemptCounterHandling == TRUE)
      {
        /* If the security level provides an interface to save the attempt counter, do so */
        Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING;
      }
      else
      {
        /* No counter needs to be saved, the operation is finished. */
        Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED;
      }
#else /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON)
      /* !LINKSTO SWS_Dcm_01155.LegacySetAttemptCounter,1 */
      Rte_DcmSecSetNumAtt(Dcm_SecurityLevelEntryConfig[SecLevelIndex].SecLevel,
                          Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter);
#endif /* #if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
    }
  }

  /* Trigger saving the attempt counter. After that the nagtive response will be send and the
   * delay timer will be started as well as the security level locked */
  if (Dcm_Dsp_SecurityAccess_DelayTimers[SecLevelIndex].AttemptCounter >=
      NewSecTabEntry->SecurityNumAttDelay
     )
  {
    /* A negative response will be returned but the security level will remain unmodified */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
    /* Set the response to be sent after storing the attempt counter is finished */
    if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING)
    {
      Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp
        = DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_EXCEEDED_NUMBER_OF_ATTEMPTS;
    }
    else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.Nrc.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.Nrc.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.Nrc.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.Nrc.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
      /* No attempt counter saving, so set the Nrc immediately */
      /* !LINKSTO Dcm324_Implicit3,1 */
      *Nrc = DCM_E_EXCEEDNUMBEROFATTEMPTS;
    }

    /* Signal the necessity to start the delay timer for this security level.
     * This is done upon confirmation of the transmitted Nrc response */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.False.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.False.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.False.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.False.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.True.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.True.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.True.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01349.SecurityDelayTimer.DcmDspSecurityUnifiedDelayTimer.True.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
    Dcm_Dsp_SecurityAccess_LockSecurityLevel = SecLevelIndex;
  }
  else
  {
    /* A negative response will be returned but the security level will remain unmodified */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
    if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING)
    {
      /* The NRC will be set after saving the attempt counter */
      Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp
        = DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_INVALID_KEY;
    }
    else
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.Nrc.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.Nrc.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.Nrc.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_00660.Nrc.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
      /* No attempt counter saving, so set the NRC immediately */
      *Nrc = DCM_E_INVALIDKEY;
    }
  }
}
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_CompareKey_Operation(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType SendKeyStatus = E_NOT_OK;

  /* pointer to configuration information of requested security level */
  P2CONST(Dcm_SecurityLevelEntryConfigType, AUTOMATIC, DCM_CONST) NewSecTabEntry
    = &(Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex]);

  const Dcm_SecLevelType NewSecLevel
    = Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].SecLevel;

  /* For  USE_DATA_SYNCH_FNC, USE_DATA_ASYNCH_FNC,USE_DATA_SYNCH_CLIENT_SERVER,
     USE_DATA_ASYNCH_CLIENT_SERVER call the DcmDspSecurityCompareKeyFnc operation */
  /* !LINKSTO Dcm.SecurityAccess.CompareKey,1 */
  if((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
     (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER))
  {
    /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.DcmDspSecurityCompareKeyFnc,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.RTE.SendKey,1 */
    /* Calling the CompareKey operation  */
    if (NewSecTabEntry->SecAccessAutosarVersion == DCM_USE_AUTOSAR_42)
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_42.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_42.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      SendKeyStatus = NewSecTabEntry->SecurityCompareKeyFncAsync_ASR42(&(MsgContextPtr->reqData[0U]),
                                                                       OpStatus, Nrc);
    }
    else
    {
      SendKeyStatus =NewSecTabEntry->SecurityCompareKeyFncAsync_ASR40(&(MsgContextPtr->reqData[0U]),
                                                                      OpStatus);
    }
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
      ((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_FNC) ||
      (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_SYNCH_CLIENT_SERVER)),
      DCM_SVCID_SECURITYACCESS);

    /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.DcmDspSecurityCompareKeyFnc,1 */
    /* !LINKSTO Dcm.EB.SecurityAccess.RTE.SendKey,1 */
    /* Calling the CompareKey operation */
    if (NewSecTabEntry->SecAccessAutosarVersion == DCM_USE_AUTOSAR_42)
    {
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_42.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_42.Port.DCM_USE_SECURITY_SYNCH_FNC, 1 */
      SendKeyStatus = NewSecTabEntry->SecurityCompareKeyFncSync_ASR42(&(MsgContextPtr->reqData[0U]),
                                                                      Nrc);
    }
    else
    {
      SendKeyStatus = NewSecTabEntry->SecurityCompareKeyFncSync_ASR40(&(MsgContextPtr->reqData[0U]));
    }
  }

  /* Handle the response key result */
  if (OpStatus != DCM_CANCEL)
  {
    /* Authentication succeeded */
    /* !LINKSTO Dcm.EB.SecurityAccess.StrictSequence,1 */
    if (SendKeyStatus == E_OK)
    {
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
      /* Reset number of failed attempts */
      if (Dcm_Dsp_SecurityAccess_DelayTimers[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].AttemptCounter >
          0U
         )
      {
        /* Reset the current attempt counter */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01357.SuccessfulSendKey, 1 */
        Dcm_Dsp_SecurityAccess_DelayTimers[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].AttemptCounter
          = 0U;
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
        /* If needed save the reset attempt counter */
        if (Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].ExternalAttemptCounterHandling ==
            TRUE
           )
        {
          /* Initialize saving of the AttemptCounter. */
          Dcm_Dsp_SecurityAccess_PostSaveAttemptCounterResp
            = DCM_POST_SAVE_ATTEMPT_COUNTER_RESP_POS;

          Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING;
        }
        else
        {
          /* The AttemptCounter doesn't need to be saved, therefore finish the operation. */
          Dcm_Dsp_SecurityAccess_SendKey_State =
            DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED;
        }
#else /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON)
      /* !LINKSTO SWS_Dcm_01155.LegacySetAttemptCounter,1 */
      Rte_DcmSecSetNumAtt(Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].SecLevel, 0);
#endif /* #if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
      }
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      else
      {
        /* The counter is 0 already, nothing to save. Finish the operation. */
        Dcm_Dsp_SecurityAccess_SendKey_State =
          DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED;
      }
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      if (Dcm_Dsp_SecurityAccess_SendKey_State ==
            DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED
         )
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
      {
        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* !LINKSTO Dcm.EB.SecurityLevelHandling.SecurityAccess.SendKey.AttemptCounterDisabled.ResetToDefaultSession.DiscardSecurityLevel,1 */
        /* only set security level when security state is not reset */
        if (DCM_SEC_IDLE != Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel)
        {
          /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.SecurityLevel.ASYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.SecurityLevel.SYNCH_CS,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.SecurityLevel.ASYNCH_FNC,1 */
          /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.SecurityLevel.SYNCH_FNC,1 */

          /* Set the new security level */
          Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(NewSecLevel);
        }

        /* LEAVE critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.PosRes.ASYNCH_CS,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.PosRes.SYNCH_CS,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.PosRes.ASYNCH_FNC,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.CompareKey.E_OK.PosRes.SYNCH_FNC,1 */

        /* If the CompareKey returns DCM_E_OK, set the Nrc to DCM_E_POSITIVERESPONSE */
        *Nrc = DCM_E_POSITIVERESPONSE;

        /* setup response data */
        MsgContextPtr->resDataLen = 1U;

        /* Reset security access state handling */
        Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
        /* Reset the state to CompareKey */
        Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
      }

      ServiceProcessStatus = DCM_E_DONE;
    }
    else if ((SendKeyStatus == E_NOT_OK) || (SendKeyStatus == DCM_E_COMPARE_KEY_FAILED))
    {
      /* Authentication failed */

      /* A negative response will be returned but the security level will remain unmodified */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.SecurityLevel.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
      /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.SecurityLevel.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */

      /* In this case Nrc will be set from the CompareKey operation */
      if (NewSecTabEntry->SecAccessAutosarVersion == DCM_USE_AUTOSAR_42)
      {
        if(SendKeyStatus == E_NOT_OK)
        {
          /* The Nrc has already been set by the CompareKey function */

          /* If Xxx_CompareKey does not return a valid Nrc, issue Nrc 0x22 */
          if (*Nrc == 0x00U)
          {
            /* !LINKSTO Dcm.EB.SecurityAccess.CompareKeyCS.NRC0x00,1 */
            /* !LINKSTO Dcm.EB.SecurityAccess.CompareKeyFunc.NRC0x00,1 */
            *Nrc = DCM_E_CONDITIONSNOTCORRECT;
          }
        }
        else
        {
          /* !LINKSTO Dcm324_Implicit4,1 */
          *Nrc = DCM_E_INVALIDKEY;
        }
      }
      else
      {
        /* Nrc should be InvalidKey if the operation CompareKey() return value
         * DCM_E_COMPARE_KEY_FAILED or DCM_E_NOT_OK. */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_40.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_40.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_40.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.Nrc.ASR_40.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
        *Nrc = DCM_E_INVALIDKEY;
      }

      DCM_PRECONDITION_ASSERT((NewSecTabEntry->SecurityNumAttDelay > 0U)
                              ,DCM_SVCID_SECURITYACCESS);

#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
      /* Attempt counter shall be incremented only upon DCM_E_COMPARE_KEY_FAILED */
      if(SendKeyStatus == DCM_E_COMPARE_KEY_FAILED)
      {
        /* The attempt counter is only incrementd when the CompareKey Operation returns
         *  DCM_E_COMPARE_KEY_FAILED and not upon DCM_E_NOT_OK */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.AttemptCounter.Port.DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.AttemptCounter.Port.DCM_USE_SECURITY_SYNCH_CLIENT_SERVER,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.AttemptCounter.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
        /* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01150.AttemptCounter.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
        Dcm_Dsp_SecurityAccess_CompareKey_TreatAttemptCounters(
           Dcm_Dsp_SecurityAccess_ReqSecLevelIndex
          ,Nrc);
      }

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
      if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY_FINISHED)
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
      {
        /* reset security access state handling */
        Dcm_Dsp_SecurityAccess_StrictCheckingReqSecLevel = DCM_SEC_IDLE;

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
        /* All operations are complete, reset the state to CompareKey. */
        Dcm_Dsp_SecurityAccess_SendKey_State = DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
      }

      ServiceProcessStatus = DCM_E_DONE;
    }
    else if ((SendKeyStatus == DCM_E_PENDING) &&
              ((NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_FNC) ||
               (NewSecTabEntry->SecAccessInterface == DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
              )
            )
    {
      /* Authentication did not finish in this main function cycle */
      ServiceProcessStatus = DCM_E_PENDING;
    }
    else
    {
      *Nrc = DCM_E_GENERALREJECT;
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    ServiceProcessStatus = DCM_E_DONE;
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.SecurityAccess.SendKey,2 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SendKey_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

#if ((DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_OFF) && \
     (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON))
  const Dcm_SecLevelType NewSecLevel
    = Dcm_SecurityLevelEntryConfig[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].SecLevel;

  /* number of failed attempts */
  /* !LINKSTO SWS_Dcm_01154.LegacyGetAttemptCounter,1 */
  Dcm_Dsp_SecurityAccess_DelayTimers[Dcm_Dsp_SecurityAccess_ReqSecLevelIndex].AttemptCounter =
        Rte_DcmSecGetNumAtt(NewSecLevel);
#endif /* #if ((DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_OFF) && \
               (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON)) */

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  /* If no attempt counter saving operation is currently active, call the compare key operation */
  if (Dcm_Dsp_SecurityAccess_SendKey_State == DCM_DSP_SECURITYACCESS_SENDKEY_STATE_COMPARE_KEY)
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
  {
    ServiceProcessStatus = Dcm_Dsp_SecurityAccess_CompareKey_Operation(OpStatus, Nrc, MsgContextPtr);
  }

  /* CompareKey may move the state of SendKey processing to initialize the saving of the attempt
   * counter */

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  /* If saving the attempt counter needs to be initialized or continued */
  if ((DCM_DSP_SECURITYACCESS_SENDKEY_STATE_INIT_SAVING == Dcm_Dsp_SecurityAccess_SendKey_State) ||
      (DCM_DSP_SECURITYACCESS_SENDKEY_STATE_SAVING == Dcm_Dsp_SecurityAccess_SendKey_State)
     )
  {
    ServiceProcessStatus
      = Dcm_Dsp_SecurityAccess_SendKey_SaveAttemptCounter(OpStatus, Nrc);

    if (*Nrc == DCM_E_POSITIVERESPONSE)
    {
      /* setup response data */
      MsgContextPtr->resDataLen = 1U;
    }
  }
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

  return ServiceProcessStatus;
}
#endif /* DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X27 == STD_ON) */

/*==================[end of file]================================================================*/
