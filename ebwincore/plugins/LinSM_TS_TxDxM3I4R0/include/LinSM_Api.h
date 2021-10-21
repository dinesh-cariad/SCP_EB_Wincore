/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINSM_API_STATIC_H
#define LINSM_API_STATIC_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <LinSM_Version.h>     /* this module's version declaration */
#include <LinSM_Cfg.h>         /* this module's configuration */
#include <LinIf.h>             /* LinIf declarations */ /* !LINKSTO LinSM.ASR40.LINSM012,1 */

#include <ComM.h>              /* ComM declarations */ /* !LINKSTO LinSM.ASR40.LINSM013,1 */

/*==================[macros]================================================*/

#if (defined LINSM_FULL_COM) /* to prevent double declaration */
#error LINSM_FULL_COM already defined
#endif /* if (defined LINSM_FULL_COM) */

#if (defined LINSM_NO_COM) /* to prevent double declaration */
#error LINSM_NO_COM already defined
#endif /* if (defined LINSM_NO_COM) */

#if (defined LINSM_WAKEUP) /* to prevent double declaration */
#error LINSM_WAKEUP already defined
#endif /* if (defined LINSM_WAKEUP) */

#if (defined LINSM_GOTO_SLEEP) /* to prevent double declaration */
#error LINSM_GOTO_SLEEP already defined
#endif /* if (defined LINSM_GOTO_SLEEP) */

#if (defined NOTHING_STORED) /* to prevent double declaration */
#error NOTHING_STORED already defined
#endif /* if (defined NOTHING_STORED) */

#if (defined FULL_COM_STORED) /* to prevent double declaration */
#error FULL_COM_STORED already defined
#endif /* if (defined FULL_COM_STORED) */

#if (defined NO_COM_STORED) /* to prevent double declaration */
#error NO_COM_STORED already defined
#endif /* if (defined NO_COM_STORED) */

#if (defined LINSM_SID_INIT) /* to prevent double declaration */
#error LINSM_SID_INIT already defined
#endif /* if (defined LINSM_SID_INIT) */

#if (defined LINSM_SID_SCHEDULEREQUEST) /* to prevent double declaration */
#error LINSM_SID_SCHEDULEREQUEST already defined
#endif /* if (defined LINSM_SID_SCHEDULEREQUEST) */

#if (defined LINSM_SID_GETVERSIONINFO) /* to prevent double declaration */
#error LINSM_SID_GETVERSIONINFO already defined
#endif /* if (defined LINSM_SID_GETVERSIONINFO) */

#if (defined LINSM_SID_GETCURRENTCOMMODE) /* to prevent double declaration */
#error LINSM_SID_GETCURRENTCOMMODE already defined
#endif /* if (defined LINSM_SID_GETCURRENTCOMMODE) */

#if (defined LINSM_SID_REQUESTCOMMODE) /* to prevent double declaration */
#error LINSM_SID_REQUESTCOMMODE already defined
#endif /* if (defined LINSM_SID_REQUESTCOMMODE) */

#if (defined LINSM_SID_MAINFUNCTION) /* to prevent double declaration */
#error LINSM_SID_MAINFUNCTION already defined
#endif /* if (defined LINSM_SID_MAINFUNCTION) */

#if (defined LINSM_SID_SCHEDULEREQUESTCONF) /* to prevent double declaration */
#error LINSM_SID_SCHEDULEREQUESTCONF already defined
#endif /* if (defined LINSM_SID_SCHEDULEREQUESTCONF) */

#if (defined LINSM_SID_WAKEUPCONFIRMATION) /* to prevent double declaration */
#error LINSM_SID_WAKEUPCONFIRMATION already defined
#endif /* if (defined LINSM_SID_WAKEUPCONFIRMATION) */

#if (defined LINSM_SID_GOTOSLEEPCONF) /* to prevent double declaration */
#error LINSM_SID_GOTOSLEEPCONF already defined
#endif /* if (defined LINSM_SID_GOTOSLEEPCONF) */

#if (defined LINSM_SID_GOTOSLEEPINDICATION) /* to prevent double declaration */
#error LINSM_SID_GOTOSLEEPINDICATION already defined
#endif /* if (defined LINSM_SID_GOTOSLEEPINDICATION) */

#if (defined LINSM_E_UNINIT) /* to prevent double declaration */
#error LINSM_E_UNINIT already defined
#endif /* if (defined LINSM_E_UNINIT) */

#if (defined LINSM_E_ALREADY_INITIALIZED) /* to prevent double declaration */
#error LINSM_E_ALREADY_INITIALIZED already defined
#endif /* if (defined LINSM_E_ALREADY_INITIALIZED) */

#if (defined LINSM_E_NONEXISTENT_NETWORK) /* to prevent double declaration */
#error LINSM_E_NONEXISTENT_NETWORK already defined
#endif /* if (defined LINSM_E_NONEXISTENT_NETWORK) */

#if (defined LINSM_E_PARAMETER) /* to prevent double declaration */
#error LINSM_E_PARAMETER already defined
#endif /* if (defined LINSM_E_PARAMETER) */

#if (defined LINSM_E_PARAMETER_POINTER) /* to prevent double declaration */
#error LINSM_E_PARAMETER_POINTER already defined
#endif /* if (defined LINSM_E_PARAMETER_POINTER) */

#if (defined LINSM_E_CONFIRMATION_TIMEOUT) /* to prevent double declaration */
#error LINSM_E_CONFIRMATION_TIMEOUT already defined
#endif /* if (defined LINSM_E_CONFIRMATION_TIMEOUT) */

#if (defined LINSM_E_NOT_IN_RUN_SCHEDULE) /* to prevent double declaration */
#error LINSM_E_NOT_IN_RUN_SCHEDULE already defined
#endif /* if (defined LINSM_E_NOT_IN_RUN_SCHEDULE) */

#if (defined LINSM_E_UNEXPECTED_CALLOUT) /* to prevent double declaration */
#error LINSM_E_UNEXPECTED_CALLOUT already defined
#endif /* if (defined LINSM_E_UNEXPECTED_CALLOUT) */

#if (defined LINSM_E_REPETITION_MAX_REACHED) /* to prevent double declaration */
#error LINSM_E_REPETITION_MAX_REACHED already defined
#endif /* if (defined LINSM_E_REPETITION_MAX_REACHED) */

/*------------------[LinSM Mode/State values]-----------*/

/** \brief full communication (used for LinSM_ModeType and channel state) */
#define LINSM_FULL_COM      1U

/** \brief no communication (used for LinSM_ModeType and channel state) */
#define LINSM_NO_COM        2U

/** \brief wakeup in progress (used for internal channel state) */
#define LINSM_WAKEUP        0U

/** \brief goto sleep in progress (used for internal channel state) */
#define LINSM_GOTO_SLEEP    3U

/*------------------[LinSM stored mode]-----------*/

/** \brief no stored mode */
#define NOTHING_STORED      0U

/** \brief full communication stored */
#define FULL_COM_STORED     1U

/** \brief no communication stored */
#define NO_COM_STORED       2U


/*------------------[Service Ids]-----------*/

/** \brief Service Id of LinSM_Init() */
#define LINSM_SID_INIT 0x01U

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
/** \brief Service Id of LinSM_ScheduleRequest() */
#define LINSM_SID_SCHEDULEREQUEST 0x10U
#endif

/** \brief Service Id of LinSM_GetVersionInfo() */
#define LINSM_SID_GETVERSIONINFO 0x02U

/** \brief Service Id of LinSM_GetCurrentComMode() */
#define LINSM_SID_GETCURRENTCOMMODE 0x11U

/** \brief Service Id of LinSM_RequestComMode() */
#define LINSM_SID_REQUESTCOMMODE 0x12U

/** \brief Service Id of LinSM_MainFunction() */
#define LINSM_SID_MAINFUNCTION 0x30U

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
/** \brief Service Id of LinSM_ScheduleRequestConfirmation() */
#define LINSM_SID_SCHEDULEREQUESTCONF 0x20U
#endif

/** \brief Service Id of LinSM_WakeupConfirmation() */
#define LINSM_SID_WAKEUPCONFIRMATION 0x21U

/** \brief Service Id of LinSM_GotoSleepConfirmation() */
#define LINSM_SID_GOTOSLEEPCONF 0x22U

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
/** \brief Service Id of LinSM_GotoSleepIndication() */
#define LINSM_SID_GOTOSLEEPINDICATION 0x03U
#endif


/*------------------[Error Ids]-----------*/

/** \brief DET Error Code
 *
 * API called without initialization of LinSM */
#define LINSM_E_UNINIT 0x00U

/** \brief DET Error Code
 *
 * Initialization API is used when already initialized
 *
 * - This error is not used as it contradicts LINSM043. */
#define LINSM_E_ALREADY_INITIALIZED 0x10U

/** \brief DET Error Code
 *
 * Referenced channel or network does not exist (identification is out of
 * range) */
#define LINSM_E_NONEXISTENT_NETWORK 0x20U

/** \brief DET Error Code
 *
 * API service called with wrong parameter  */
#define LINSM_E_PARAMETER 0x30U

/** \brief DET Error Code
 *
 * API service called with invalid pointer */
#define LINSM_E_PARAMETER_POINTER 0x40U

/** \brief DET Error Code
 *
 * Timeout of the callbacks from LinIf */
#define LINSM_E_CONFIRMATION_TIMEOUT 0x50U

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
/** \brief DET Error Code
 *
 * LinSM_ScheduleRequest called for a channel not in FULL_COM state */
#define LINSM_E_NOT_IN_RUN_SCHEDULE 0x51U
#endif

/** \brief DET Error Code:
 *
 * LinIf signalled an unexpected confirmation */
#define LINSM_E_UNEXPECTED_CALLOUT 0x60U

/** \brief DET Error Code:
 *
 * Repetition max was excedeed */
#define LINSM_E_REPETITION_MAX_REACHED 0x61U

/*==================[type definitions]======================================*/

/** \brief Type to report the current mode to the BswM
 *
 *  Range:
 *  - LINSM_FULL_COM
 *  - LINSM_NO_COM
 */
typedef uint8 LinSM_ModeType;

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief Initializes the LinSM module.
  **
  ** This function initializes the LinSM.
  ** Note that the \p ConfigPtr parameter is ignored by this implementation
  ** as post-build configuration is not supported.
  **
  ** \param[in] ConfigPtr  Pointer to the LinSM configuration (ignored)
  **
  ** \ServiceID{0x01}
  ** \Reentrancy{Non reentrant}
  ** \Synchronicity{Synchronous}
  */
extern FUNC(void, LINSM_CODE) LinSM_Init
(
  P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_APPL_CONST) ConfigPtr
);

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
/** \brief Change schedule table for a LIN channel
 **
 ** The upper layer requests a schedule table to be changed on one LIN
 ** channel.
 **
 ** This services delegates the schedule request to the LinIf.
 **
 ** \param[in] network   Identification of the LIN channel
 ** \param[in] schedule  Index of the new Schedule table
 **
 ** \return Result of operation
 ** \retval E_OK Schedule table request has been accepted.
 ** \retval E_NOT_OK Schedule table switch request has not been
 **      accepted due to one of the following reasons:
 **      * LinSM has not been initialized
 **      * referenced channel does not exist (identification is out of range)
 **      * Referenced schedule table does not exist (identification is out of
 **        range)
 **      * Sub-state is not LINSM_FULL_COM
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant for different LIN channels}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest
(
  NetworkHandleType network,
  LinIf_SchHandleType schedule
);
#endif


/** \brief Get version information of the LinSM module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Vendor Id
 **  - Module Id
 **  - Vendor specific version numbers
 **
 ** \param[out] versioninfo Pointer to where to store the version
 **                         information of this module.
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
#if (LINSM_VERSION_INFO_API == STD_ON)
extern FUNC(void, LINSM_CODE) LinSM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINSM_APPL_DATA) versioninfo
);
#endif /* (LINSM_VERSION_INFO_API == STD_ON) */


/** \brief Function to query the current communication mode.
 **
 ** Returns the current communication mode for the specified channel.
 **
 ** \param[in] network  Identification of the LIN channel
 ** \param[out] mode    Returns the active mode, see ComM_ModeType for
 **                     descriptions of the modes
 **
 ** \return Result of operation
 ** \retval E_OK Ok
 ** \retval E_NOT_OK Not possible to perform the request, e.g. not
 **                  initialized.
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode
(
  NetworkHandleType  network,
  P2VAR(ComM_ModeType, AUTOMATIC, LINSM_APPL_DATA) mode
);


/** \brief Requesting of a communication mode by ComM.
 **
 ** The mode switch will not be made instantly. The LinSM will notify the
 ** ComM when mode transition is made.
 **
 ** \param[in] network  Identification of the LIN channel
 ** \param[in] mode  Requested mode
 **
 ** \return Result of operation
 ** \retval E_OK Request accepted
 ** \retval E_NOT_OK Not possible to perform the request, e.g. not
 **                  initialized.
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Reentrant for different LIN channels}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode
(
  NetworkHandleType  network,
  ComM_ModeType  mode
);


/** \brief Cyclic MainFunction for the LIN State Manager
 **
 ** Periodic function that runs the timers of different request timeouts
 **
 ** This function must be called cyclically using a fixed time period
 ** specified in LinSMMainProcessingPeriod.
 **
 ** \ServiceID{0x30}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINSM_CODE) LinSM_MainFunction
(
  void
);

#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

#endif /* ifndef LINSM_API_STATIC_H */
/*==================[end of file]===========================================*/

