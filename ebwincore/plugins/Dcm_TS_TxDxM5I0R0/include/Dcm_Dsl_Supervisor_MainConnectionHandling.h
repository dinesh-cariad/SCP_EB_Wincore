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

/* !LINKSTO Dcm.Dsn.File.Supervisor.MainConnectionHandling.PublicApi,1 */
/* This file contains the public API of the Supervisor MainConnectionHandling software unit. */

#ifndef DCM_DSL_SUPERVISOR_MAINCONNECTIONHANDLING_H
#define DCM_DSL_SUPERVISOR_MAINCONNECTIONHANDLING_H
/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Dsl_CommunicationServices_BufferManager.h>
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>
#include <Dcm_Dsl_CommunicationServices_TxConnections.h>

#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>
/* MainConnectionHandling API */

/** \brief Initializes the Dcm_MainConnectionStatus data.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_Init(
  void
);

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/** \brief Iterates over all main connections and if a transmission is enqueued calls
 **        Dcm_ApplicationTransmissionConfirmation API.
 **
 ** It is called at the beginning of Dcm_Dsl_Supervisor_PreProcessingMainFunction.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_PreProcessinMainFunction(
  void
);

/** \brief Searches for a main connection index based on TxConnectionId.
 **
 */
extern FUNC (uint16_least, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_SearchForMainConnection(
  const Dcm_BufferUserType TxConnectionId
);

/** \brief Stores information in Dcm_MainConnectionStatus for the transmission the next call of
 **        Dcm_Dsl_Supervisor_PreProcessingMainFunction.
 **        Also will set the Dcm_ApplicationTransmisionConfirmationEnqueued flag on TRUE in order
 **        to inform that a transmission is needed.
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_StoreMainConnectionStatusTransmission(
  const Dcm_MainConnectionIndexType MainConnectionIndex,
  const Dcm_MsgContextPtrType MsgContext,
  const Dcm_ResponseConfirmationTypeType ResponseConfirmationType,
  const Dcm_NegativeResponseCodeType NegativeResponseCode,
  const boolean TransmissionSuccessful
);
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

/** \brief Sends queued NRCs.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_MainFunction(
  void
);

/** \brief This function enqueues an NRC.
 **
 ** The NRC will be sent as a response to a request on an RxConnectionID. It will be transmitted
 ** IN THE NEXT AVAILABLE MAIN FUNCTION CYCLE. This is to be used from ISRs so we don't call the
 ** PduR to send NRCs in the context of an ISR.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
  Dcm_BufferUserType RxConnectionId,
  Dcm_BufferType RequestBuffer,
  Dcm_NegativeResponseCodeType NRC,
  Dcm_TimerValueType NRCTimeout
);

/** \brief Signals to the internal implementation that the request was dispatched to the protocol
 **        for execution. The internal state of the MainConnection will be updated.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_RequestDispatchedToProtocol(
  Dcm_MainConnectionIndexType MainConnectionIndex
);

/** \brief Signals to the internal implementation that the indicated MainConnection became free.
 **        The internal state of the MainConnection will be updated.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(
  Dcm_MainConnectionIndexType MainConnectionIndex
);

/** \brief Signals to the internal implementation that the indicated MainConnection became busy
 **        with a request. The internal state of the MainConnection will be updated.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
  Dcm_MainConnectionIndexType MainConnectionIndex,
  Dcm_BufferUserType RxConnectionId
);

/** \brief Returns whether the indicated MainConnection is busy with a request (not idle).
 **
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_MainConnectionBusy(
  Dcm_MainConnectionIndexType MainConnectionIndex
);

/** \brief Takes actions based on the status of the given MainConnection. It can:
 **          - Notify a protocol that an RCRRP transmission has ended.
 **          - Clean up the MainConnection resources to make it free.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_TransmissionFinished(
  Dcm_MainConnectionIndexType MainConnectionIndex,
  Dcm_TxResultType TransmissionResult
);

/** \brief Returns the configured NRC timeout value for the given protocol.
 **
 */
extern FUNC (Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_GetNRCTimeout(
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief This function checks if MainConnection is busy with a NRC transmitting.
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_IsBusyWithNRC(
    Dcm_BufferUserType RxConnectionId
);

/** \brief Signals to the CommunicationSessionHandling subunit that the communication sequence has
 **        finished. Cleans up the resources to be able to process a new request.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_End(
  Dcm_MainConnectionIndexType MainConnectionIndex
);

/** \brief This function is used to obtain the status for a specific Main Connection.
 **
 */
extern FUNC (PduIdType, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_GetRxConnectionId(
    Dcm_MainConnectionIndexType MainConnIdx);

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_SUPERVISOR_MAINCONNECTIONHANDLING_H */
/*==================[end of file]================================================================*/
