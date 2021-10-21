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

#ifndef DCM_SECURITYACCESS_CFG_H
#define DCM_SECURITYACCESS_CFG_H

/* !LINKSTO Dcm.Dsn.File.SecurityAccess.ConfigH,1 */

/*==================[includes]===================================================================*/

[!AUTOSPACING!]
#include <Std_Types.h>

/*==================[macros]=====================================================================*/

/** @defgroup Macro_Dcm_SecurityAccesType Macros for Dcm_SecurityAccesType
** \brief    These macro definitions are used as values for Dcm_SecurityAccesType
**  @{
*/

#if (defined DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER)
#error DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER already defined
#endif /* if (defined DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER) */

/** \brief The DCM will access the data using an RPort requiring a asynchronous CS Interface
*/
#define DCM_USE_SECURITY_ASYNCH_CLIENT_SERVER 0U

#if (defined DCM_USE_SECURITY_ASYNCH_FNC)
#error DCM_USE_SECURITY_ASYNCH_FNC already defined
#endif /* if (defined DCM_USE_SECURITY_ASYNCH_FNC) */

/** \brief The DCM will access the data using the (async) functions that are defined in the
** parameters DcmDspSecurityGetSeedFnc and DcmDspSecurityCompareKeyFnc.
*/
#define DCM_USE_SECURITY_ASYNCH_FNC 1U

#if (defined DCM_USE_SECURITY_SYNCH_CLIENT_SERVER)
#error DCM_USE_SECURITY_SYNCH_CLIENT_SERVER already defined
#endif /* if (defined DCM_USE_SECURITY_SYNCH_CLIENT_SERVER) */

/** \brief The DCM will access the data using an RPort requiring a synchronous CS Interface
*/
#define DCM_USE_SECURITY_SYNCH_CLIENT_SERVER 2U

#if (defined DCM_USE_SECURITY_SYNCH_FNC)
#error DCM_USE_SECURITY_SYNCH_FNC already defined
#endif /* if (defined DCM_USE_SECURITY_SYNCH_FNC) */

/** \brief The DCM will access the data using the (sync) functions that are defined in the
** parameters DcmDspSecurityGetSeedFnc and DcmDspSecurityCompareKeyFnc.
*/
#define DCM_USE_SECURITY_SYNCH_FNC 3U

/** @defgroup Macro_Dcm_SecurityAccessAutosarVersionType Macros for
** Dcm_SecurityAccessAutosarVersionType
** \brief    These macro definitions are used as values for Dcm_SecurityAccessAutosarVersionType
**  @{
*/

#if (defined DCM_USE_AUTOSAR_40)
#error DCM_USE_AUTOSAR_40 already defined
#endif /* if (defined DCM_USE_AUTOSAR_40) */

/** \brief The DCM will use a CompareKey async interface compatible ASR 40 */
#define DCM_USE_AUTOSAR_40 1U

#if (defined DCM_USE_AUTOSAR_42)
#error DCM_USE_AUTOSAR_42 already defined
#endif /* if (defined DCM_USE_AUTOSAR_42) */

/** \brief The DCM will use a CompareKey async interface compatible ASR 40 */
#define DCM_USE_AUTOSAR_42 2U

/* @} *//* End of macros definitions for Dcm_SecurityAccessAutosarVersionType */

/* @} *//* End of macros definitions for Dcm_SecurityAccesType */

#if (defined DCM_NUM_CONFIGURED_SECURITY_LEVELS) /* To prevent double declaration */
#error DCM_NUM_CONFIGURED_SECURITY_LEVELS already defined
#endif /* if (defined DCM_NUM_CONFIGURED_SECURITY_LEVELS) */

/** \brief Number of configured security levels */
#define DCM_NUM_CONFIGURED_SECURITY_LEVELS                               [!//
[!"num:integer(count(DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*))"!]U

#if (defined DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED already defined
#endif /* if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED) */

/** \brief Is the delay timers feature in security access available? */
/* !LINKSTO Dcm.EB.SecurityAccess.DelayTime.DisableFeature,1 */
#define DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED                      [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[(DcmDspSecurityDelayTime != 0) or (DcmDspSecurityDelayTimeOnBoot != 0)])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED already defined
#endif /* if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED) */

/** \brief Is the external handling of attempt counters available? */
#define DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED             [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[DcmDspSecurityAttemptCounterEnabled = 'true'])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_READOUT_TIME) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_READOUT_TIME already defined
#endif /* if (DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_READOUT_TIME) */

/** \brief Max readout time for all security access attempt counters */
#define DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_READOUT_TIME          [!//
[!"num:integer(ceiling(node:value(DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityMaxAttemptCounterReadoutTime) div node:value(DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)))"!]U

#if (defined DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME already defined
#endif /* if (DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME) */

/** \brief Max write time for one security access attempt counter */
#define DCM_DSP_SECURITYACCESS_MAX_ATTEMPT_COUNTER_WRITE_TIME          [!//
[!"num:integer(ceiling(node:value(DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityMaxAttemptCounterWriteTime) div node:value(DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)))"!]U

#if (defined DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED already defined
#endif /* if (DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED) */

/** \brief This flag signals if a unified delay timer is used */
#define DCM_DSP_SECURITYACCESS_UNIFIED_DELAY_TIMER_ENABLED                [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityUnifiedDelayTimer = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING already defined
#endif /* if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING) */

/** \brief This flag signals if legacy AttemptCounter handling is used */
#define DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING                [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityLegacyAttemptCountersHandling = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT already defined
#endif /* if (DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT) */

/** \brief This flag signals if an attempt counter will be reset upon delay timer expiry */
#define DCM_DSP_SECURITYACCESS_RESET_ATTEMPT_COUNTER_ON_TIMEOUT           [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityResetAttemptCounterOnTimeout = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY already defined
#endif /* if (DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY) */

/** \brief This flag signals if an infinite delay is applied upon attemptcounter readout failures */
#define DCM_DSP_SECURITYACCESS_FAILED_ATTEMPT_COUNTER_READOUT_INFINITE_DELAY         [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityAttemptCounterReadoutFailInfiniteDelay = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT) /* To prevent double declaration */
#error DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT already defined
#endif /* if (DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT) */

/** \brief This flag signals if attempt counters are incremented beyond their limit */
#define DCM_DSP_SECURITYACCESS_INCREMENT_ATTEMPT_COUNTER_PAST_LIMIT        [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityIncrementAttemptCounterPastLimit = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityAccessAutosarVersionType,1 */
/** \brief Security Access ASR version */
typedef uint8 Dcm_SecurityAccessAutosarVersionType;

/** \brief State of an authentication sequence (seed is already requested or not) */
typedef uint8 Dcm_SecStateType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityAccesType,1 */
/** \brief Security access interface type */
typedef uint8 Dcm_SecurityAccesType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetSeedFncADRAsyncType,1 */
/** \brief Type of function to get seed value if ADRSize > 0(Async operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetSeedFncADRAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SecurityAccessDataRecord,
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetSeedFncADRSyncType,1 */
/** \brief Type of function to get seed value if ADRSize > 0(Sync and User operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetSeedFncADRSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SecurityAccessDataRecord,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetSeedFncAsyncType,1 */
/** \brief Type of function to get seed value for ADRSize not > 0 (Async operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetSeedFncAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetSeedFncSyncType,1 */
/** \brief Type of function to get seed value for ADRSize not > 0 (Sync and User operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetSeedFncSyncType) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_ASR40_SecurityCompareKeyFncAsyncType,1 */
/** \brief Type of function to send Compare Key value (Async operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ASR40_SecurityCompareKeyFncAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Key,
  Dcm_OpStatusType OpStatus);

/* !LINKSTO Dcm.Dsn.Type.Dcm_ASR42_SecurityCompareKeyFncAsyncType,1 */
/** \brief Type of function to send Compare Key value (Async operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ASR42_SecurityCompareKeyFncAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Key,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_ASR40_SecurityCompareKeyFncSyncType,1 */
/** \brief Type of function to send Compare key value (Sync and User operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ASR40_SecurityCompareKeyFncSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Key);

/* !LINKSTO Dcm.Dsn.Type.Dcm_ASR42_SecurityCompareKeyFncSyncType,1 */
/** \brief Type of function to send Compare key value (Sync and User operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ASR42_SecurityCompareKeyFncSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Key,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
/* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01152.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetAttemptCounterFncSyncType,1 */
/** \brief Type of synchronous function to get the SecurityAttemptCounter */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetAttemptCounterFncSyncType) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter);

/* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01152.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityGetAttemptCounterFncAsyncType,1 */
/** \brief Type of asynchronous function to get the SecurityAttemptCounter */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecurityGetAttemptCounterFncAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter);

/* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01153.Port.DCM_USE_SECURITY_SYNCH_FNC,1 */
/* !LINKSTO Dcm.Dsn.Type.Dcm_SecuritySetAttemptCounterFncSyncType,1 */
/** \brief Type of synchronous function to set the SecurityAttemptCounter */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecuritySetAttemptCounterFncSyncType) (
  uint8 AttemptCounter);

/* !LINKSTO Dcm.EB.SecurityAccess.SWS_Dcm_01153.Port.DCM_USE_SECURITY_ASYNCH_FNC,1 */
/* !LINKSTO Dcm.Dsn.Type.Dcm_SecuritySetAttemptCounterFncAsyncType,1 */
/** \brief Type of asynchronous function to set the SecurityAttemptCounter */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SecuritySetAttemptCounterFncAsyncType) (
  Dcm_OpStatusType OpStatus,
  uint8 AttemptCounter);
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */

/* !LINKSTO Dcm.Dsn.Type.Dcm_SecurityLevelEntryConfigType,1 */
/** \brief Entry of a security level table */
typedef struct
{
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
  /** \brief Delay time started after max number of failed security access attempts is reached.
   **        Only after this time elapses a 'SecurityAccess' can be requested again.
   */
  uint32 DelayTime;
  /** \brief Delay time started at boot after max number of failed security access attempts is
   **        reached. Only after this time elapses a 'SecurityAccess' can be requested again.
   */
  uint32 DelayTimeOnBoot;
#endif
  /** \brief Size of seed in bytes */
  uint32 SeedSize;
  /** \brief Size of key in bytes */
  uint32 KeySize;
  /** \brief ADR Size */
  uint32 ADRSize;
  /** \brief ptr to DcmDspSecurityGetSeedFnc() for userfnc. and sync operation for ADRSize > 0 */
  Dcm_SecurityGetSeedFncADRSyncType SecurityGetSeedFncADRSync;
   /** \brief ptr to DcmDspSecurityGetSeedFnc() for async operation for ADRSize > 0*/
  Dcm_SecurityGetSeedFncADRAsyncType SecurityGetSeedFncADRAsync;
  /** \brief ptr to DcmDspSecurityGetSeedFnc() for userfnc.
   ** and sync operation for ADRSize not > 0*/
  Dcm_SecurityGetSeedFncSyncType SecurityGetSeedFncSync;
  /** \brief ptr to DcmDspSecurityGetSeedFnc() for async operation for ADRSize not > 0*/
  Dcm_SecurityGetSeedFncAsyncType     SecurityGetSeedFncAsync;
  /** \brief ptr to DcmDspSecurityComapreKeyFnc() for sync operation. AUTOSAR 4.0.3 version */
  Dcm_ASR40_SecurityCompareKeyFncSyncType SecurityCompareKeyFncSync_ASR40;
  /** \brief ptr to DcmDspSecurityComapreKeyFnc() for sync operation. AUTOSAR 4.2.2 version */
  Dcm_ASR42_SecurityCompareKeyFncSyncType SecurityCompareKeyFncSync_ASR42;
  /** \brief ptr to DcmDspSecurityComapreKeyFnc() for async operation. AUTOSAR 4.0.3 version */
  Dcm_ASR40_SecurityCompareKeyFncAsyncType SecurityCompareKeyFncAsync_ASR40;
  /** \brief ptr to DcmDspSecurityComapreKeyFnc() for async operation. AUTOSAR 4.2.1 version */
  Dcm_ASR42_SecurityCompareKeyFncAsyncType SecurityCompareKeyFncAsync_ASR42;
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  /** \brief Sync operation for GetSecurityAttemptCounter */
  Dcm_SecurityGetAttemptCounterFncSyncType SecurityGetAttemptCounterFncSync;
  /** \brief Async operation for GetSecurityAttemptCounter */
  Dcm_SecurityGetAttemptCounterFncAsyncType SecurityGetAttemptCounterFncAsync;
  /** \brief Sync operation for SetSecurityAttemptCounter */
  Dcm_SecuritySetAttemptCounterFncSyncType SecuritySetAttemptCounterFncSync;
  /** \brief Async operation for SetSecurityAttemptCounter */
  Dcm_SecuritySetAttemptCounterFncAsyncType SecuritySetAttemptCounterFncAsync;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
  /** \brief Number of attempts until a delay will be activated */
  uint8 SecurityNumAttDelay;
#if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON)
  /** \brief If true, the attempt counter for this level is handled externally */
  boolean ExternalAttemptCounterHandling;
#endif /* #if (DCM_DSP_SECURITYACCESS_SAVE_ATTEMPT_COUNTERS_ENABLED == STD_ON) */
  /** \brief If true, the delay time on boot is executed regardless of the delay counter value */
  boolean DelayTimeOnBootOverride;
  /** \brief If true, the delay time is infinite upon reached the max. number of attempts */
  boolean DelayTimeInfinite;
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
  /** \brief Security access interface type */
  Dcm_SecurityAccesType SecAccessInterface;
  /** \brief Security access interface, AUTOSAR version dependent */
  Dcm_SecurityAccessAutosarVersionType SecAccessAutosarVersion;
  /** \brief Value of this security level */
  Dcm_SecLevelType SecLevel;
}
Dcm_SecurityLevelEntryConfigType;

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal data]==============================================================*/
#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
/** \brief Table of diagnostic security levels */
extern CONST(Dcm_SecurityLevelEntryConfigType, DCM_CONST)
  Dcm_SecurityLevelEntryConfig[DCM_NUM_CONFIGURED_SECURITY_LEVELS];
#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_SECURITYACCESS_CFG_H */
/*==================[end of file]================================================================*/
