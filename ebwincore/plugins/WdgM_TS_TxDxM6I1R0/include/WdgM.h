/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef WDGM_H
#define WDGM_H
/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Cfg.h>                                        /* Pre-compile Configuration macros */
#include <WdgM_Version.h>                                  /* this module's version declarations */

/*==================[macros]=====================================================================*/

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020205,1 */
/* WdgM Generated interface version check */
#if !WDGM_EB_GENIF_VERSION_CHECK(1,1)
#error Version check between WdgM Static and WdgM Generated failed
#endif

/*------------------[AUTOSAR service IDs]--------------------------------------------------------*/

#if (defined WDGM_SID_INIT)
#error WDGM_SID_INIT already defined
#endif
/** \brief Service id of WdgM_Init() */
#define WDGM_SID_INIT                   0x00U

#if (defined WDGM_SID_DEINIT)
#error WDGM_SID_DEINIT already defined
#endif
/** \brief Service id of WdgM_DeInit() */
#define WDGM_SID_DEINIT                 0x01U

#if (defined WDGM_SID_GET_VERSION_INFO)
#error WDGM_SID_GET_VERSION_INFO already defined
#endif
/** \brief Service id of WdgM_GetVersionInfo() */
#define WDGM_SID_GET_VERSION_INFO       0x02U

#if (defined WDGM_SID_SET_MODE)
#error WDGM_SID_SET_MODE already defined
#endif
/** \brief Service id of WdgM_SetMode() */
#define WDGM_SID_SET_MODE               0x03U

#if (defined WDGM_SID_GET_MODE)
#error WDGM_SID_GET_MODE already defined
#endif
/** \brief Service id of WdgM_GetMode() */
#define WDGM_SID_GET_MODE               0x0bU

#if (defined WDGM_SID_CHECKPOINT_REACHED)
#error WDGM_SID_CHECKPOINT_REACHED already defined
#endif
/** \brief Service id of WdgM_CheckpointReached() */
#define WDGM_SID_CHECKPOINT_REACHED     0x0eU

#if (defined WDGM_SID_UPDATE_ALIVE_COUNTER)
#error WDGM_SID_UPDATE_ALIVE_COUNTER already defined
#endif
/** \brief Service id of WdgM_UpdateAliveCounter() */
#define WDGM_SID_UPDATE_ALIVE_COUNTER   0x04U

#if (defined WDGM_SID_GET_LOCAL_STATUS)
#error WDGM_SID_GET_LOCAL_STATUS already defined
#endif
/** \brief Service id of WdgM_GetLocalStatus() */
#define WDGM_SID_GET_LOCAL_STATUS       0x0cU

#if (defined WDGM_SID_GET_GLOBAL_STATUS)
#error WDGM_SID_GET_GLOBAL_STATUS already defined
#endif
/** \brief Service id of WdgM_GetGlobalStatus() */
#define WDGM_SID_GET_GLOBAL_STATUS      0x0dU

#if (defined WDGM_SID_PERFORM_RESET)
#error WDGM_SID_PERFORM_RESET already defined
#endif
/** \brief Service id of WdgM_PerformReset() */
#define WDGM_SID_PERFORM_RESET          0x0fU

#if (defined WDGM_SID_GET_FIRST_EXPIRED_SEID)
#error WDGM_SID_GET_FIRST_EXPIRED_SEID already defined
#endif
/** \brief Service id of WdgM_GetFirstExpiredSEID() */
#define WDGM_SID_GET_FIRST_EXPIRED_SEID 0x10U

#if (defined WDGM_SID_MAIN_FUNCTION)
#error WDGM_SID_MAIN_FUNCTION already defined
#endif
/** \brief Service id of WdgM_MainFunction() */
#define WDGM_SID_MAIN_FUNCTION          0x08U

/*------------------[AUTOSAR development error tracing]------------------------------------------*/

#if (defined WDGM_E_NO_INIT)
#error WDGM_E_NO_INIT already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.1,1,WDGM.ASR40.WDGM004.1,1 */
/** \brief Development error: WdgM not initialized */
#define WDGM_E_NO_INIT                  0x10U

#if (defined WDGM_E_PARAM_CONFIG)
#error WDGM_E_PARAM_CONFIG already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.2,1 */
/** \brief Development error: API service Wdg_Init was called with an erroneous
 * configuration set */
#define WDGM_E_PARAM_CONFIG             0x11U

#if (defined WDGM_E_PARAM_MODE)
#error WDGM_E_PARAM_MODE already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.3,1,WDGM.ASR40.WDGM004.3,1 */
/** \brief Development error: API service called with invalid mode */
#define WDGM_E_PARAM_MODE               0x12U

#if (defined WDGM_E_PARAM_SEID)
#error WDGM_E_PARAM_SEID already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.4,1,WDGM.ASR40.WDGM004.4,1 */
/** \brief Development error: API service called with invalid supervised
 * entity */
#define WDGM_E_PARAM_SEID               0x13U

#if (defined WDGM_E_INV_POINTER)
#error WDGM_E_INV_POINTER already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.5,1,WDGM.ASR40.WDGM004.5,1 */
/** \brief Development error: API service called with NULL pointer */
#define WDGM_E_INV_POINTER             0x14U

#if (defined WDGM_E_DISABLE_NOT_ALLOWED)
#error WDGM_E_DISABLE_NOT_ALLOWED already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.6,1,WDGM.ASR40.WDGM004.6,1 */
/** \brief Development error: Disabling of watchdog not allowed */
#define WDGM_E_DISABLE_NOT_ALLOWED      0x15U

#if (defined WDGM_E_CPID)
#error WDGM_E_CPID already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.7,1,WDGM.ASR40.WDGM004.7,1 */
/** \brief Development error: API service used with an invalid CheckpointId */
#define WDGM_E_CPID                     0x16U

#if (defined WDGM_E_DEPRECATED)
#error WDGM_E_DEPRECATED already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.8,1,WDGM.ASR40.WDGM004.8,1 */
/** \brief Development error: Deprecated API service was used */
#define WDGM_E_DEPRECATED               0x17U

#if (defined WDGM_E_AMBIGIOUS)
#error WDGM_E_AMBIGIOUS already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.9,1,WDGM.ASR40.WDGM004.9,1 */
/** \brief Development error: Function WdgM_UpdateAliveIndication cannot determine the Checkpoint,
 * because there are more than one alive supervisions configured in the current mode for the given
 * Supervised Entity */
#define WDGM_E_AMBIGIOUS                0x18U

#if (defined WDGM_E_SEDEACTIVATED)
#error WDGM_E_SEDEACTIVATED already defined
#endif
/* !LINKSTO WDGM.ASR40.WDGM259.10,1,WDGM.ASR40.WDGM004.10,1 */
/** \brief Development error: API service used with a checkpoint of a Supervised Entity
 * that is deactivated in the current Watchdog Manager mode */
#define WDGM_E_SEDEACTIVATED            0x19U

/*------------------[EB development error tracing]-----------------------------------------------*/

#if (defined WDGM_EB_E_INIT_REQUEST)
#error WDGM_EB_E_INIT_REQUEST already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020441,1 */
/** \brief Development error: WdgM initialization request failed */
#define WDGM_EB_E_INIT_REQUEST          0x80U

#if (defined WDGM_EB_E_DEINIT_REQUEST)
#error WDGM_EB_E_DEINIT_REQUEST already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020444,1 */
/** \brief Development error: WdgM de-initialization request failed */
#define WDGM_EB_E_DEINIT_REQUEST        0x81U

#if (defined WDGM_EB_E_SETMODE_REQUEST)
#error WDGM_EB_E_SETMODE_REQUEST already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020438,1 */
/** \brief Development error: WdgM mode change request failed */
#define WDGM_EB_E_SETMODE_REQUEST       0x82U

#if (defined WDGM_EB_E_REENTRANCY)
#error WDGM_EB_E_REENTRANCY already defined
#endif
/** \brief Development error: non-reentrant WdgM main function is executed in parallel */
#define WDGM_EB_E_REENTRANCY            0x83U

#if (defined WDGM_E_PARAM_WRONG_CORE_ID)
#error WDGM_E_PARAM_WRONG_CORE_ID already defined
#endif
/** \brief Development error: API service called with wrong core id. */
#define WDGM_E_PARAM_WRONG_CORE_ID               0x84U

#if (defined WDGM_EB_E_SLAVE_FAILED_CHANGEMODE)
#error WDGM_EB_E_SLAVE_FAILED_CHANGEMODE already defined
#endif
/** \brief Development error: Satellite instance failed to change to the new mode in the time given by
  * parameter WdgMMasterWaitSlaveModeSwitch. Initialization is considered a particular mode switch
  * operation to WdgMInitialMode. */
#define WDGM_EB_E_SLAVE_FAILED_CHANGEMODE               0x85U

#if (defined WDGM_E_NOT_AUTHORIZED)
#error WDGM_E_NOT_AUTHORIZED already defined
#endif
/** \brief Development error: API service is not authorized to be executed
 * (e.g. in case of a call to WdgM_PerformReset) */
#define WDGM_E_NOT_AUTHORIZED           0xF1U

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/
#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/* This Service is only provided, if callout concept is not used */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020405,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
/** \brief Initialize the Watchdog Manager
 **
 ** \param[in] ConfigPtr Pointer to configuration data, this parameter is
 ** ignored in the current implementation.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) WdgM_Init
(
  P2CONST(WdgM_ConfigType, AUTOMATIC, WDGM_APPL_CONST) ConfigPtr
);
#elif (WDGM_EB_EXT_INIT_REDIRCALLOUT_ENABLED == STD_ON)
#if (defined WdgM_Init)
#error WdgM_Init already defined
#endif
 /* !LINKSTO WDGM.EB.TIMEPM.WDGM020406,1 */
/** \brief Redirect Init BSW API **/
#define WdgM_Init WDGM_EB_EXT_INIT_REDIRCALLOUT
#endif /* (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF) */

 /* !LINKSTO WDGM.EB.TIMEPM.WDGM020407,1 */
/* This Service is only provided, if callout concept is not used */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
/** \brief De-initialize the Watchdog Manager
 **
 ** \pre The WdgM is initialized or an initialization request is active
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) WdgM_DeInit
(
  void
);
#elif (WDGM_EB_EXT_DEINIT_REDIRCALLOUT_ENABLED == STD_ON)
#if (defined WdgM_DeInit)
#error WdgM_DeInit already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020408,1 */
/** \brief Redirect DeInit BSW API **/
#define WdgM_DeInit WDGM_EB_EXT_DEINIT_REDIRCALLOUT
#endif /* (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF) */

#if (WDGM_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the Watchdog Manager
 **
 ** This service returns the version information of this module. The version
 ** information includes
 ** - Module Id
 ** - Vendor Id
 ** - Vendor specific version numbers
 **
 ** \pre Parameter \a VersionInfo is a valid Pointer
 **
 ** \param[out] VersionInfo  Version information
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) WdgM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, WDGM_APPL_DATA) VersionInfo
);
#endif /* (WDGM_VERSION_INFO_API == STD_ON) */

/* This Service is only provided, if callout concept is not used */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020401,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
#if (WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40)
/** \brief Set the current mode of the Watchdog Manager.
 **
 ** \pre The WdgM is initialized or an initialization request is active
 ** \pre Parameter \a Mode is a valid (the mode ID exists in the configuration)
 ** \pre Parameter \a CallerID is valid (the ID is in the list of configured caller IDs)
 **
 ** \param[in] Mode  One of the configured Watchdog Manager modes
 ** \param[in] CallerID  Module ID of the calling module
 ** \return Success of operation
 ** \retval E_OK     Successfully changed to the new mode
 ** \retval E_NOT_OK Changing to the new mode failed
 **
 ** \ProductionError{WDGM_E_IMPROPER_CALLER, thrown\, if the passed CallerID is not in the list of
 **                                       the configured list of allowed CallerIDs}
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_SetMode
(
  WdgM_ModeType Mode,
  uint16 CallerID
);
#else /* WDGM_EB_BSW_COMPATIBILITY_ASR32 or WDGM_EB_BSW_COMPATIBILITY_ASR43 */
/** \brief Set the current mode of the Watchdog Manager.
 **
 ** \pre The WdgM is initialized or an initialization request is active
 ** \pre Parameter \a Mode is valid (the mode ID exists in the configuration)
 **
 ** \param[in] Mode  One of the configured Watchdog Manager modes
 ** \return Success of operation
 ** \retval E_OK     Successfully changed to the new mode
 ** \retval E_NOT_OK Changing to the new mode failed
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_SetMode
(
  WdgM_ModeType Mode
);
#endif
#elif (WDGM_EB_EXT_SETMODE_REDIRCALLOUT_ENABLED == STD_ON)
#if (defined WdgM_SetMode)
#error WdgM_SetMode already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020402,1 */
/** \brief Redirect SetMode BSW API **/
#define WdgM_SetMode WDGM_EB_EXT_SETMODE_REDIRCALLOUT
#endif /* (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF) */

/** \brief Returns the current mode of the Watchdog Manager.
 **
 ** \pre The WdgM is initialized
 ** \pre Parameter \a Mode is a valid pointer
 **
 ** \param[out] Mode  Current WdgM mode
 ** \return Success of operation
 ** \retval E_OK     Current mode successfully returned
 ** \retval E_NOT_OK Returning current mode failed
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetMode
(
  P2VAR(WdgM_ModeType, AUTOMATIC, WDGM_APPL_DATA) Mode
);

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CODE
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.ASR40.WDGM155,1 */
/** \brief Give alive indications to the Watchdog Manager via AUTOSAR 3.2 API (deprecated)
 **
 ** This function updates the alive counter of a requested supervised entity
 ** of the current mode.
 **
 ** \pre The WdgM is initialized
 ** \pre Parameter \a SEID is valid (the ID exists in the configuration)
 ** \pre The Supervised Entity with ID \a SEID is active
 ** \pre At least one Alive Supervision is configured for \a SEID
 **
 ** \param[in] SEID ID of supervised entity whose alive counter is updated
 ** \return Success of operation
 ** \retval E_OK     Operation successful
 ** \retval E_NOT_OK Operation failed
 **
 ** \ProductionError{WDGM_E_DATA_CORRUPTION, thrown\, if data corruption is detected in the
 **                         internal WdgM data}
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_UpdateAliveCounter
(
  WdgM_SupervisedEntityIdType SEID
);

/* !LINKSTO WDGM.ASR40.WDGM263,1 */
/** \brief Give alive indications to the Watchdog Manager
 **
 ** This function indicates to the Watchdog Manager that a checkpoint
 ** within a supervised entity has been reached.
 **
 ** \pre The WdgM is initialized
 ** \pre Parameter \a SEID is valid (the ID exists in the configuration)
 ** \pre Parameter \a CheckpointID is valid (the ID exists in the configuration of \a SEID)
 ** \pre The Supervised Entity with ID \a SEID is active
 **
 ** \param[in] SEID ID of supervised entity whose alive counter is updated
 ** \param[in] CheckpointID Identifier of the Checkpoint within a Supervised Entity
 **            that has been reached.
 ** \return Success of operation
 ** \retval E_OK     Operation successful
 ** \retval E_NOT_OK Operation failed
 **
 ** \ProductionError{WDGM_E_DATA_CORRUPTION, thrown\, if data corruption is detected in the
 **                         internal WdgM data}
 **
 ** \ServiceID{0x0e}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_CheckpointReached
(
  WdgM_SupervisedEntityIdType SEID,
  WdgM_CheckpointIdType       CheckpointID
);

#define WDGM_STOP_SEC_CODE
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/** \brief Get supervision status of a specific entity
 **
 ** \pre The WdgM is initialized
 ** \pre Parameter \a SEID is valid (the ID exists in the configuration)
 ** \pre Parameter \a Status is valid pointer
 **
 ** \param[in] SEID ID of supervised entity whose status shall be returned
 ** \param[out] Status  Status of the given supervised entity
 ** \return Success of operation
 ** \retval E_OK     Operation successful
 ** \retval E_NOT_OK Operation failed
 **
 ** \ServiceID{0x0c}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetLocalStatus
(
  WdgM_SupervisedEntityIdType                      SEID,
  P2VAR(WdgM_LocalStatusType, AUTOMATIC, WDGM_APPL_DATA) Status
);

/** \brief Get global supervision status of the Watchdog Manager
 **
 ** \pre The WdgM is initialized
 ** \pre Parameter \a Status is valid pointer
 **
 ** \param[out] Status  Global supervision status
 ** \return Success of operation
 ** \retval E_OK     Operation successful
 ** \retval E_NOT_OK Operation failed
 **
 ** \ServiceID{0x0d}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetGlobalStatus
(
  P2VAR(WdgM_GlobalStatusType, AUTOMATIC, WDGM_APPL_DATA) Status
);

/** \brief Get SEID that first reached WDGM_LOCAL_STATUS_EXPIRED
 **
 ** Returns SEID that first reached the state WDGM_LOCAL_STATUS_EXPIRED.
 **
 ** \pre The ECU was reset due to an Expired Supervised Entity
 ** \pre Parameter \a SEID is valid pointer
 **
 ** \param[out] SEID Supervised entity ID that first reached WDGM_LOCAL_STATUS_EXPIRED
 ** \return Success of operation
 ** \retval E_OK     Operation successful
 ** \retval E_NOT_OK Operation failed
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetFirstExpiredSEID
(
  P2VAR(WdgM_SupervisedEntityIdType, AUTOMATIC, WDGM_APPL_DATA) SEID
);

/** \brief Force a watchdog reset
 **
 ** \pre The WdgM is initialized
 **
 ** Instructs the Watchdog Manager to cause a watchdog reset.
 **
 ** \ServiceID{0x0f}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) WdgM_PerformReset
(
  void
);


/*-----------------------------------------------------------------------------------------------
 * Scheduled functions
 *-----------------------------------------------------------------------------------------------*/

/* prevent redundant declarations if used from WdgM files which also include
 * Rte_WdgM.h */
#if ((WDGM_EB_INCLUDE_RTE == STD_OFF) || (!defined(WDGM_INTERNAL_USAGE)))

/** \brief Cyclic main function for WdgM processing.
 *
 *  Performs the processing of the cyclic Watchdog Manager jobs.
 *
 ** \ProductionError{WDGM_E_MONITORING, thrown\,
 **                         if supervision has failed for a supervised entity}
 ** \ProductionError{WDGM_E_SET_MODE, thrown\, if watchdog drivers' mode switch has failed}
 ** \ProductionError{WDGM_E_MF_TIMINGVIOLATION, thrown\, if WdgM_MainFunction period deviates from
 **                         the configured mode-dependent schedule period (<code>WdgMSupervisionCycle</code>)}
 ** \ProductionError{WDGM_E_DATA_CORRUPTION, thrown\, if data corruption is detected in the
 **                         internal WdgM data}
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
/* Deviation MISRA-2 */
extern FUNC(void, WDGM_CODE) WdgM_MainFunction
(
  void
);
#endif /* ((WDGM_EB_INCLUDE_RTE == STD_OFF) || (!defined WDGM_INTERNAL_USAGE )) */

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>
/*==================[external constants]=========================================================*/

#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/** \brief Data structure storing configuration data
 *
 * A pointer to this structure must be given to WdgM_Init().  The name of this
 * structure is defined by the container name of the first entry of the WdgM
 * configuration set list. */
extern CONST(WdgM_ConfigType, WDGM_APPL_CONST) WDGM_CONFIG_NAME;

#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/*==================[external data]==============================================================*/

#endif /* ifndef WDGM_H */
/*==================[end of file]================================================================*/
