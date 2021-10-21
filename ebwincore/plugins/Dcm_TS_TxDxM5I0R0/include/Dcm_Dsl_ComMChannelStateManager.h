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

/* !LINKSTO Dcm.Dsn.File.ComMChannelStateManager.PrivateApi,1 */
/* This file contains the Public API of the ComMChannelStateManager software
   unit as well as data types and macro definitions used by this unit. */

#ifndef DCM_DSL_COMMCHANNELSTATEMANAGER_H
#define DCM_DSL_COMMCHANNELSTATEMANAGER_H

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 8.9 (required)
 *     An identifier with external linkage shall have exactly one external definition.
 *
 *     Reason:
 *     The definition of the declared external variable is, for some unit tests, in the test source
 *     code as a dummy.
 *     The test source code is not included in the MISRA analysis and therefore it is
 *     not visible to the MISRA checker.
 */
/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Cfg.h>                                  /* Dcm configuration header file */

#include <ComStack_Types.h>                           /* AUTOSAR standard types */
#include <Dcm_Dsl_Cfg.h>                              /* Dcm Dsl Configuration  */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif

/*==================[macros]=====================================================================*/
#if (defined DCM_COMM_OPERATION_ENABLED)
#error DCM_COMM_OPERATION_ENABLED already defined
#endif /* if (defined DCM_COMM_OPERATION_ENABLED) */

/** \brief Allowed name value ID which means that the operation is enable on ComMChannel.
 *  This is a part of Dcm_ComMChannelOperationStateType */
#define DCM_COMM_OPERATION_ENABLED      TRUE

#if (defined DCM_COMM_OPERATION_DISABLED)
#error DCM_COMM_OPERATION_DISABLED already defined
#endif /* if (defined DCM_COMM_OPERATION_DISABLED) */

/** \brief Allowed name value ID which means that the operation is disable on ComMChannel.
 *  This is a part of Dcm_ComMChannelOperationStateType */
#define DCM_COMM_OPERATION_DISABLED     FALSE

/*==================[type definitions]===========================================================*/

/** \brief Type definition which holds the state of a certain operation on ComMChannel */
typedef boolean Dcm_ComMChannelOperationStateType;

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.RuntimeDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.RuntimeDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.RuntimeDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.RuntimeDataTypes,1 */

/** \brief Generic, invalid communication mode. */
#if (defined DCM_INVALID_COMMUNICATION_MODE)
  #error "DCM_INVALID_COMMUNICATION_MODE is already defined"
#endif
#define DCM_INVALID_COMMUNICATION_MODE 0U

/** \brief Both reception and transmission are disabled. */
#if (defined DCM_NO_COMMUNICATION_MODE)
  #error "DCM_NO_COMMUNICATION_MODE is already defined"
#endif
#define DCM_NO_COMMUNICATION_MODE 1U

/** \brief Reception enabled, transmission disabled. */
#if (defined DCM_SILENT_COMMUNICATION_MODE)
  #error "DCM_SILENT_COMMUNICATION_MODE is already defined"
#endif
#define DCM_SILENT_COMMUNICATION_MODE 2U

/** \brief Both reception and transmission are enabled. */
#if (defined DCM_FULL_COMMUNICATION_MODE)
  #error "DCM_FULL_COMMUNICATION_MODE is already defined"
#endif
#define DCM_FULL_COMMUNICATION_MODE 3U

typedef uint8 Dcm_ComMChannelModeType;

#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
/** \brief The status of allowing or disallowing the call to ComM_DCM_ActiveDiagnostic() function
 */

/** \brief The Dcm shall call ComM_DCM_ActiveDiagnostic() when in default session */
#if (defined DCM_COMM_ACTIVE)
  #error "DCM_COMM_ACTIVE is already defined"
#endif
#define DCM_COMM_ACTIVE     0U

/** \brief The Dcm shall NOT call ComM_DCM_ActiveDiagnostic() when in default session */
#if (defined DCM_COMM_NOT_ACTIVE)
  #error "DCM_COMM_NOT_ACTIVE is already defined"
#endif
#define DCM_COMM_NOT_ACTIVE 1U

typedef uint8 Dcm_ActiveDiagnosticType;
#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON) */

/** \brief This type identifies a ComMChannel.
 */
typedef uint8 Dcm_NetworkIdType;

/** \brief Type to store Network state information */
typedef struct
{
  /** \brief The network id of the array of Network IDs.
   */
  Dcm_NetworkIdType NetworkID;

  /** \brief Flag to indicate that the network is active.
   */
  boolean ActiveFlag;
} Dcm_ComMChannelInfoType;

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Function disable for all the ComMChannels the ComM operations */
extern FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_Init(void);

/** \brief Function checks if the ComMChannelID that is given as a parameter,
 *  it is set to enable a reception operation
 *
 *  \param[in] ComMChannelID            The ID of ComMChannel to get if reception operation is enable
 *
 *  \return DCM_COMM_OPERATION_ENABLED  if ComMChannelID is having active reception
 */
extern FUNC(Dcm_ComMChannelOperationStateType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetRxState(
  NetworkHandleType ComMChannelID
);

/** \brief Function checks if the ComMChannelID that is given as a parameter,
 *  it is set to enable a transmission operation
 *
 *  \param[in] ComMChannelID            The ID ComMChannel to get if transmission operation is enable
 *
 *  \return DCM_COMM_OPERATION_ENABLED  if ComMChannelID is having active transmission
 */
extern FUNC(Dcm_ComMChannelOperationStateType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetTxState(
  NetworkHandleType ComMChannelID
);

/** \brief Notifies the Dcm of a ComMChannel mode transition.
 *
 *  \param NetworkId [in]           ComMChannelId which transitions to a communication mode.
 *                                  is provided.
 *  \param CommunicationMode [in]   The newly-entered mode for the ComMChannel.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_EnterMode(
  Dcm_NetworkIdType NetworkId,
  Dcm_ComMChannelModeType CommunicationMode
);


/** \brief Attempt to notify the ComManager that diagnosis is active on a channel and it should
 *         therefore not be shut down. This will only happen if a SWC didn't disable this
 *         functionality
 *
 *  \param  [in]   NetworkId   Network identifier
 *
 *  \return Returns whether notification was successful or not.
 *  \retval DCM_E_OK     Notification was successful.
 *  \retval DCM_E_NOT_OK Notification failed.
 *
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic(
  NetworkHandleType NetworkId
);
/** \brief Attempt to notify the ComManager that diagnosis is inactive on one or more channels and
 *         should therefore be shut down.
 *
 *  \return Returns whether notification was successful or not.
 *  \retval DCM_E_OK     Notification was successful.
 *  \retval DCM_E_NOT_OK Notification failed.
 *
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic(
  NetworkHandleType NetworkId
);

#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
/** \brief Returns whether a ComM_DCM_Active/InactiveDiagnostic() call failed since
           the last invocation of this API. Also resets this information.
 *
 *  \param none
 *
 *  \return TRUE     a ComM_DCM_Active/InactiveDiagnostic() operation has failed since
 *                   the last call of this API
 *          FALSE    no ComM_DCM_Active/InactiveDiagnostic() operation has failed since
 *                   the last call of this API
 **/
extern FUNC(boolean, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetAndResetOperationFailedState(
  void
);
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMCHANNELSTATEMANAGER_H */
/*==================[end of file]================================================================*/
