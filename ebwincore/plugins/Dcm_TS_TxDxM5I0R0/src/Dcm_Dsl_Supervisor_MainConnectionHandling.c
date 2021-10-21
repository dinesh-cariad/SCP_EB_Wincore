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

/* !LINKSTO Dcm.Dsn.File.Supervisor.MainConnectionHandling.Impl,1 */
/* This file contains the implementation of the Supervisor MainConnectionHandling software unit. */

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
 */
/*===============================[includes]======================================================*/

#include <Dcm_Dsl_Supervisor_MainConnectionHandling.h>         /* Dcm Dsl Supervisor header file */
#include <Dcm_Dsl_Supervisor_CommunicationSessionHandling.h>   /* Dcm Dsl Supervisor header file */
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>               /* Dcm Dsl Supervisor header file */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                     /* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#endif
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

/*===========================[macro definitions]=================================================*/

/** @defgroup Macro_Dcm_MainConnectionStateType Macros for Dcm_MainConnectionStateType
 ** \brief   These macros are used as values for Dcm_MainConnectionStateType
 **  @{
 */

/** \brief The connection is in invalid state, e.g., not initialized   */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_INVALID_STATE)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_INVALID_STATE is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_INVALID_STATE 0x00U

/** \brief No transmission and no processing is onging.  */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_IDLE)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_IDLE is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_IDLE 0x01U

/** \brief A reception was received on an RxConnection belonging to this MainConnection, and has
 ** resulted in the associated Protocol being made presumptive. The request has not been
 ** dispatched yet to the Protocol. */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING 0x02U

/** \brief The incoming request on an RxConnection belonging to this MainConnection, has been
 ** dispatched to the associated Protocol. */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_PROCESSING)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_PROCESSING is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_PROCESSING 0x03U

/** \brief A request was received on an RxConnection belonging to this MainConnection, and it
 ** resulted in the Supervisor queuing a final (-non-RCRRP) NRC to be transmitted as a reply
 ** before dispatching it to a Protocol. The NRC transmission will start always from a
 ** Dcm_MainFunction() context. Either in the next MainFunction cycle, if the decision to send
 ** an NRC is taken outside a Dcm_MainFunction() context, or in the same Dcm_MainFunction() */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED 0x04U

/** \brief The MainConnection is busy sending a NRC. This is a final transmission. */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING 0x05U

/** \brief While the MainConnection's request is waiting in the Supervisor to be dispatched, a P2
 ** timeout occurred. As a result, an RCRRP transmission is queued.  */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED 0x06U

/** \brief The MainConnection is busy sending a RCRRP NRC. */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING 0x07U

/** \brief While a RCRRP transmission is going on, something happened and the Supervisor took the
 ** decision to finally respond to this request with a NRC. As soon as the RCRRP finishes the
 ** transmission (successfully), the NRC is sent. */
#if (defined DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED)
  #error "DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED 0x08U

/* @} */
/* End of macros definitions for Dcm_MainConnectionStateType */

/** @defgroup Macro_Dcm_SupervisorApplTransmissionConfirmationType Macros for
 **           Dcm_SupervisorApplTransmissionConfirmationType
 ** \brief    These macros are used as values for Dcm_SupervisorApplTransmissionConfirmationType
 **  @{
 */
#if (defined DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION)
  #error "DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION 0x00U

#if (defined DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION)
  #error "DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION 0x01U

#if (defined DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL)
  #error "DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL 0x02U

#if (defined DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR)
  #error "DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR 0x03U
/* @} */
/* End of macros definitions for Dcm_SupervisorApplTransmissionConfirmationType */

/*===========================[type definitions]==================================================*/
/** \brief Type definition for states of a MainConnection */
typedef uint8 Dcm_MainConnectionStateType;

/** \brief Type definition for Service Identifier */
typedef uint8 Dcm_ServiceIdentifierType;

/** \brief Type definition for Application Transmission Confirmation */
typedef uint8 Dcm_SupervisorApplTransmissionConfirmationType;

 #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
typedef struct
{
  /** \brief **/
  Dcm_MsgContextType MsgContextData;
  /** \brief **/
  Dcm_BufferIdType BufferId;
} Dcm_Dsl_Supervisor_TxConf_ContextType;
#endif

typedef struct
{
  Dcm_BufferType NRCBuffer;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  Dcm_MsgContextType MsgContext;
#endif

  Dcm_TransmissionTimeoutType NRCTimeout;

  Dcm_BufferUserType RxConnectionId;

  Dcm_MainConnectionStateType State;

  Dcm_NegativeResponseCodeType RespondWithNRC;

  Dcm_ServiceIdentifierType ServiceId;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)

  Dcm_ResponseConfirmationTypeType ResponseConfirmationType;

  boolean TransmissionSuccessful;

  Dcm_SupervisorApplTransmissionConfirmationType TransmissionEnqueued;
#endif

} Dcm_MainConnectionStatusType;

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_SendQueuedNRC(
  Dcm_MainConnectionIndexType MainConnectionIndex
);

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_ProtocolApplicationTransmissionConfirmation(
  Dcm_TxResultType TransmissionResult,
  Dcm_MainConnectionIndexType MainConnectionIndex
);

#endif

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_MainConnectionStatusType, DCM_VAR) Dcm_MainConnectionStatus[DCM_NUM_MAIN_CONNECTIONS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.Init,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_Init(
  void
)
{
  uint8_least MainConnectionIndex;

 /* Deviation TASKING-1 */
  for (MainConnectionIndex = 0U; MainConnectionIndex < DCM_NUM_MAIN_CONNECTIONS; MainConnectionIndex++)
  {
    Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_IDLE;
    Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId =  DCM_INVALID_USER_ID;
    Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC = 0U;
    Dcm_MainConnectionStatus[MainConnectionIndex].ServiceId = 0U;
    Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = 0U;
    Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferId = DCM_BUFFER_INVALID_ID;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
    Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = DCM_FINALRESPONSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = FALSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION;
#endif
  }
}

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.PreProcessinMainFunction,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_PreProcessinMainFunction(
  void
)
{
  uint8_least MainConnectionIndex;
  Dcm_BufferSizeType ResDataLen = 0U;
  Dcm_MsgType BufferPtr = NULL_PTR;

  for (MainConnectionIndex = 0;
       MainConnectionIndex < DCM_NUM_MAIN_CONNECTIONS;
       MainConnectionIndex++)
  {
    if(Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued != DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION)
    {
      if((Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued == DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION) ||
          (Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued == DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR) )
      {
        ResDataLen = Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resDataLen;
        BufferPtr = Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resData;
        Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resDataLen = 0U;
        Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resData = NULL_PTR;
      }

      Dcm_ApplicationTransmissionConfirmation(
          Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.idContext,
          &Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext,
          Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType,
          Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.msgAddInfo.suppressPosResponse,
          Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful);

      if((Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued == DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION) ||
         (Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued == DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR) )
      {
        Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resDataLen = ResDataLen;
        Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resData = BufferPtr;
      }
      Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = DCM_FINALRESPONSE;
      Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = FALSE;
      Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION;
    }
  }
  Dcm_Dsl_Supervisor_ApplicationTransmisionConfirmationEnqueued_SetFlag(FALSE);
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.SearchForMainConnection,1 */
FUNC (uint16_least, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_SearchForMainConnection(
  const Dcm_BufferUserType TxConnectionId
)
{
  uint16_least MainConnectionIndex;

   for (MainConnectionIndex = 0;
        MainConnectionIndex < DCM_NUM_MAIN_CONNECTIONS;
        MainConnectionIndex++)
   {

     if (Dcm_MainConnectionConfig[MainConnectionIndex].ConfirmationTxPduId ==
           DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId))
     {
       break;
     }
   }
   return MainConnectionIndex;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.StoreMainConnectionStatusTransmission,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_StoreMainConnectionStatusTransmission(
  const Dcm_MainConnectionIndexType MainConnectionIndex,
  const Dcm_MsgContextPtrType MsgContext,
  const Dcm_ResponseConfirmationTypeType ResponseConfirmationType,
  const Dcm_NegativeResponseCodeType NegativeResponseCode,
  const boolean TransmissionSuccessful
)
{
  if( DCM_E_BUSYREPEATREQUEST == NegativeResponseCode )
  {
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL_BRR;
  }
  else
  {
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_PROTOCOL;
  }

  if (NULL_PTR == MsgContext)
  {
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION;
  }
  else
  {
    Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext = *MsgContext;
  }
  Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = ResponseConfirmationType;
  Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = TransmissionSuccessful;
}
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.MainFunction,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_MainFunction(
  void
)
{
  uint8_least MainConnectionIndex;

 /* Deviation TASKING-1 */
  for (MainConnectionIndex = 0;
       MainConnectionIndex < DCM_NUM_MAIN_CONNECTIONS;
       MainConnectionIndex++)
  {
    switch (Dcm_MainConnectionStatus[MainConnectionIndex].State)
    {
      case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED:
      case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED:
      {
        /* Protection can be provided in the function below as no state change from these
           two states to another state can happen in another context,
           except from RCRRP_QUEUED to NRC_QUEUED, leading to the same
           treating.
         */

        /* Something's waiting to be sent. Send it. */
        Dcm_Dsl_Supervisor_MainConnectionHandling_SendQueuedNRC((Dcm_MainConnectionIndexType)(MainConnectionIndex));
        break;
      }

      default:
        /* Do nothing. No action is required in the other cases. */
        break;
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.InitiateNRC,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
  Dcm_BufferUserType RxConnectionId,
  Dcm_BufferType RequestBuffer,
  Dcm_NegativeResponseCodeType NRC,
  Dcm_TimerValueType NRCTimeout
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_MainConnectionIndexType MainConnectionIndex;
  Dcm_BufferUserType TxConnectionId;
  PduIdType ConfirmationTxPduId;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  MainConnectionIndex = RxPduIdInfo.MainConnectionIndex;

  /* Get the Tx connection related to the main connection */
  ConfirmationTxPduId = Dcm_MainConnectionConfig[MainConnectionIndex].ConfirmationTxPduId;
  /* Translate the ConfirmationTxPduID into a TxConnectionId */
  TxConnectionId = DCM_TX_CONNECTION_ID_FROM_INDEX(ConfirmationTxPduId);

  Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId = RxConnectionId;
  Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC = NRC;
  Dcm_MainConnectionStatus[MainConnectionIndex].ServiceId = RequestBuffer.BufferPtr[0];
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = NRCTimeout;

#if(DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)

  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.reqData = &RequestBuffer.BufferPtr[1];
  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.reqDataLen = RequestBuffer.FilledLength - 1;

  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resDataLen = 0U;
  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resData = NULL_PTR;
  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.resMaxDataLen = 0U;

  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.idContext = RequestBuffer.BufferPtr[0];
  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.dcmRxPduId = RxConnectionId;

  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.msgAddInfo.reqType = Dcm_RxPduIdConfig[RxConnectionId].AddressingType;

  /* set suppress positive response information */
  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.msgAddInfo.suppressPosResponse = FALSE;

  Dcm_MainConnectionStatus[MainConnectionIndex].MsgContext.msgAddInfo.requestOrigin = DCM_NORMAL;

#endif /* #if(DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  switch (Dcm_MainConnectionStatus[MainConnectionIndex].State)
  {

    case DCM_SUPERVISOR_MAIN_CONNECTION_IDLE:
    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING:

      /* Reserve NRC Buffer of MainConnection. This operation shall be protected by the
         encompassing exclusive area of this function. */
      (void) Dcm_Dsl_BufferAllocate(TxConnectionId,
                                    DCM_BUFFER_TX_NRC,
                                    DCM_BUFFER_NRC_BUFFER_SIZE,
                                    &Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer);

      /* MainConnection is idle or blocked as a result of reception. */
      if (DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING == NRC)
      {

        /* This is the special RCRRP NRC. This also only comes when the MainConnection is busy. */

        DCM_PRECONDITION_ASSERT(
          DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING ==
            Dcm_MainConnectionStatus[MainConnectionIndex].State,
          DCM_INTERNAL_API_ID);

        /* Note: this state actually flows directly into transmission since the treating of the
           MainConnections is done in the same cycle as the treating of the Timers, right after
           the Timers.
         */
        Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED;

        /* This also forces TransmissionTimeout to be 0U. */
        Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = 0U;
      }
      else
      {
        Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED;
      }
      break;

    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING:

      DCM_PRECONDITION_ASSERT(
        DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING != NRC,
        DCM_INTERNAL_API_ID);

      /* MainConnection is busy sending a RCRRP as a result of a P2Timer timeout. */
      Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED;

      break;

    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED:

      DCM_PRECONDITION_ASSERT(
        DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING != NRC,
        DCM_INTERNAL_API_ID);

      /* The MainConnection had an RCRRP enqueued.
         In this case we replace the RCRRP with the NRC. */

      /* This also forces TransmissionTimeout to be 0U. Theoretically, since the timer timed out,
         the NRC initialization would have been invoked with the P2Timer value, which was 0
         anyway. */
      Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = 0U;

      Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.MainConnectionBusy,1 */
FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_MainConnectionBusy(
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{
  boolean MainConnectionBusy = TRUE;

  if (DCM_SUPERVISOR_MAIN_CONNECTION_IDLE == Dcm_MainConnectionStatus[MainConnectionIndex].State)
  {
    MainConnectionBusy = FALSE;
  }

  return MainConnectionBusy;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.TransmissionFinished,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_TransmissionFinished(
  Dcm_MainConnectionIndexType MainConnectionIndex,
  Dcm_TxResultType TransmissionResult
)
{
  const Dcm_ProtocolIndexType ProtocolIndex =
    Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex;

  const Dcm_TimerValueType  NRCTimeout = Dcm_Dsl_Supervisor_MainConnectionHandling_GetNRCTimeout(ProtocolIndex);

  /* The Supervisor first needs to know what the answer was for. This means it needs to know
     whether the MainConnection sending the answer was busy transmitting either:
     - an NRC, case in which this is the end point. Ignore the transmission result and
       clear the MainConnection.
     - a RCRRP, case in which:
       - If it succeeded, set the flag that a RCRRP has already been sent by the MainConnection,
         maybe even increment a counter and restart the P2 timer.
       - If it failed, this is the end point. Clear the MainConnection AND the queue. */

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Do note that the Supervisor only transmits from Presumptive protocols. */
  switch (Dcm_MainConnectionStatus[MainConnectionIndex].State)
  {
    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING:
      /* MainConnection was sending an NRC. With this its actions are now done.
         Don't care about how the transmission went. */

      Dcm_Dsl_Supervisor_MainConnectionHandling_End(MainConnectionIndex);

      /* Clear the presumptive protocol after a successful NRC transmission */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

      break;

    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING:

      /* MainConnection was sending a RCRRP. See whether it succeeded. */
      if (DCM_TX_OK == TransmissionResult)
      {
        /* Go back to a general "busy" state on the MainConnection. */
        Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING;

        /* Notify Protocol that a RCRRP has been transmitted for it. */
        Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPTransmissionFinished(
            ProtocolIndex
          );

        /* Restart P2Timer */
        (void)Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(DCM_SUPERVISOR_ID,
                                                            ProtocolIndex,
                                                            NRCTimeout);
      }
      else
      {
        /*If the RCRRP failed transmission, the Tester's already timed out. Really nothing left
          to do do but clear resources. */

        Dcm_Dsl_Supervisor_MainConnectionHandling_End(MainConnectionIndex);

        /* P2Timer is already elapsed, free it. */
        (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                           ProtocolIndex);

        Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

      }

      break;

    case DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED:

      if (DCM_TX_OK == TransmissionResult)
      {
        /* Just change the state of the MainConnection. The transmission will follow in the next cycle. */
        Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED;
        /* As an RCRRP just got sent, of course set the timeout to maximum. */
        Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = NRCTimeout;
      }
      else
      {
        /* If the RCRRP failed transmission, the Tester's already timed out.
           nothing left to do but clear resources. */
        Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

        Dcm_Dsl_Supervisor_MainConnectionHandling_End(MainConnectionIndex);
      }

      /* P2Timer is already elapsed, free it. */
      (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                         ProtocolIndex);
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  }

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)

  Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_APPL_TRANSMISSION_CONFIRMATION_MAIN_CONNECTION;

  Dcm_Dsl_ProtocolApplicationTransmissionConfirmation(
      TransmissionResult,
      MainConnectionIndex
    );
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  DCM_POSTCONDITION_ASSERT(MainConnectionIndex != DCM_NUM_MAIN_CONNECTIONS, DCM_INTERNAL_API_ID);
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.ClearMainConnection,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{
    Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId = DCM_INVALID_USER_ID;
    Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC = 0U;
    Dcm_MainConnectionStatus[MainConnectionIndex].ServiceId = 0x00U;
    Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = 0U;
    Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_IDLE;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
    Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = DCM_FINALRESPONSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = FALSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION;
#endif
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.BusyWithRequest,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
  Dcm_MainConnectionIndexType MainConnectionIndex,
  Dcm_BufferUserType RxConnectionId
)
{
  /* MainConnection must be idle to make this transition */
  DCM_PRECONDITION_ASSERT(
    DCM_SUPERVISOR_MAIN_CONNECTION_IDLE ==
    Dcm_MainConnectionStatus[MainConnectionIndex].State,
    DCM_INTERNAL_API_ID);

  /* MainConnection becomes busy with waiting for the processing of this request. Clear the rest.  */
  Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING;
  Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId =  RxConnectionId;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.RequestDispatchedToProtocol,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_RequestDispatchedToProtocol(
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{
  /* MainConnection must be in one of the following states:
      BUSY_WAITING, BUSY_RCRRP_QUEUED or BUSY_RCRRP_TRANSMITTING to make this transition */
  DCM_PRECONDITION_ASSERT(
    ((DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_WAITING ==
      Dcm_MainConnectionStatus[MainConnectionIndex].State) ||
     (DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED ==
      Dcm_MainConnectionStatus[MainConnectionIndex].State) ||
     (DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING ==
      Dcm_MainConnectionStatus[MainConnectionIndex].State)),
    DCM_INTERNAL_API_ID);

  /* MainConnection becomes busy with processing this request. Clear the rest.  */
  Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_PROCESSING;
  Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId =  DCM_INVALID_USER_ID;
  Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC = 0U;
  Dcm_MainConnectionStatus[MainConnectionIndex].ServiceId = 0U;
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout = 0U;
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferId = DCM_BUFFER_INVALID_ID;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
    Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = DCM_FINALRESPONSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = FALSE;
    Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = DCM_SUPERVISOR_PROTOCOL_NO_APPL_TRANSMISSION_CONFIRMATION;
#endif
}

FUNC (Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_GetNRCTimeout(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  Dcm_TimerValueType  NRCTimeout = 0U;
  Dcm_TimerValueType  P2StarMaxTime = Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime(
                                      ProtocolIndex);
  Dcm_TimerValueType  ServerAdjust = (Dcm_TimerValueType)
                                     (Dcm_ProtocolConfig[ProtocolIndex].P2StarServerAdjustTime);

  /* In case that P2StarMaxTime is smaller then P2StarServerAdjustTime P2Timer will be started with value 0 */
  if (P2StarMaxTime > ServerAdjust)
  {
    NRCTimeout = P2StarMaxTime - ServerAdjust;
  }

  return NRCTimeout;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.IsBusyWithNRC,1 */
FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_IsBusyWithNRC(
    Dcm_BufferUserType RxConnectionId
)
{
  boolean MainConnIsBusyWithNrc = FALSE;
  Dcm_MainConnectionIndexType MainConnectionIndex = Dcm_RxPduIdConfig[RxConnectionId].MainConnectionIndex;

  if ((Dcm_MainConnectionStatus[MainConnectionIndex].State == DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING_NRC_QUEUED) ||
     (Dcm_MainConnectionStatus[MainConnectionIndex].State ==  DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING))
  {
    MainConnIsBusyWithNrc = TRUE;
  }
  return MainConnIsBusyWithNrc;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_End(
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{
  /* Advertize end of communication sequence to SessionManager. */
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId);

  /* And that all processing has been finished pertaining to this communication session.
     This checks to see whether S3 timers or ComMChannels need any handling. */
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId);

  /* Free NRC Buffer. */
  (void)Dcm_Dsl_BufferFree(
          DCM_SUPERVISOR_ID,
          Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferId
   );

   /* The Protocol will no longer be presumptive after this so clear the MainConnection. */
   Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(MainConnectionIndex);
}

FUNC (PduIdType, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_GetRxConnectionId(
    Dcm_MainConnectionIndexType MainConnIdx)
{
  PduIdType RxConnectionId;

  RxConnectionId = Dcm_MainConnectionStatus[MainConnIdx].RxConnectionId;

  return RxConnectionId;
}

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>
/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.SendQueuedNRC,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_MainConnectionHandling_SendQueuedNRC(
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{
  /* MainConnection is idle, NRC may be enqueued.*/
  /* Get the Tx connection related to the main connection */
  const PduIdType ConfirmationTxPduId = Dcm_MainConnectionConfig[MainConnectionIndex].ConfirmationTxPduId;
  /* Translate the ConfirmationTxPduID into a TxConnectionId */
  const Dcm_BufferUserType TxConnectionId = DCM_TX_CONNECTION_ID_FROM_INDEX(ConfirmationTxPduId);
  Dcm_RxPduIdInfoType RxPduIdInfo;
  NetworkHandleType ComMChannelId;

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StartProtocol.ReturnsNotE_OK.GetMetaDataSourceAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StartProtocol.ReturnsNotE_OK.GetMetaDataTargetAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StopProtocol.ReturnsNotE_OK.GetMetaDataSourceAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StopProtocol.ReturnsNotE_OK.GetMetaDataTargetAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitBRR.GetMetaDataSourceAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitBRR.GetMetaDataTargetAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitRCRRP.GetMetaDataSourceAddr, 1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitRCRRP.GetMetaDataTargetAddr, 1 */
  (void) Dcm_Dsl_Supervisor_ProtocolHandling_MetaDataInfoHandling(
            Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex,
            MainConnectionIndex);
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* An NRC has been queued, either:
       BusyRepeatRequest
       RequestCorrectlyReceivedResponsePending
       ConditionsNotCorrect
       GeneralReject
  */
  DCM_PRECONDITION_ASSERT(((DCM_E_GENERALREJECT ==
                              Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC) ||
                           (DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING ==
                              Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC) ||
                           (DCM_E_CONDITIONSNOTCORRECT ==
                              Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC) ||
                           (DCM_E_BUSYREPEATREQUEST==
                              Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC)
                          ), DCM_INTERNAL_API_ID);

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(
    Dcm_MainConnectionStatus[MainConnectionIndex].RxConnectionId,
    &RxPduIdInfo);

  ComMChannelId = RxPduIdInfo.ComMChannelId;

  /* The NRC Buffer of the MainConnection. is already reserved. */

  /* fill the NRC Buffer with the data of the requested NRC */
    Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferPtr[0] = 0x7F;
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferPtr[1] =
    Dcm_MainConnectionStatus[MainConnectionIndex].ServiceId;
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.BufferPtr[2] =
    Dcm_MainConnectionStatus[MainConnectionIndex].RespondWithNRC;
  Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer.FilledLength = 3U;

  /* The owner of the NRC buffer is always the TxConnection upon allocation. */

  /* Depending on what kind of NRC was enqueued, move MainConnection to necessary state.
     This is done before the transmission as the transmission can fail basically in the first
     try. */
  if (DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_QUEUED ==
        Dcm_MainConnectionStatus[MainConnectionIndex].State)
  {
    /* Normal NRC was queued*/
    Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_NRC_TRANSMITTING;
  }
  else
  {

    DCM_INVARIANT_ASSERT(DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_QUEUED ==
      Dcm_MainConnectionStatus[MainConnectionIndex].State, DCM_INTERNAL_API_ID);

    /* RCRRP was queued */
    Dcm_MainConnectionStatus[MainConnectionIndex].State = DCM_SUPERVISOR_MAIN_CONNECTION_BUSY_RCRRP_TRANSMITTING;

    /* Notify protocol */
    Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPSent(RxPduIdInfo.ProtocolIndex);
  }

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_TxConnectionTransmit(TxConnectionId,
                               ComMChannelId,
                               DCM_SUPERVISOR_ID,
                               FALSE,
                               3U,
                               Dcm_MainConnectionStatus[MainConnectionIndex].NRCTimeout,
                               Dcm_MainConnectionStatus[MainConnectionIndex].NRCBuffer
                              );

}

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.MainConnectionHandling.ProtocolApplicationTransmissionConfirmation,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_ProtocolApplicationTransmissionConfirmation(
  Dcm_TxResultType TransmissionResult,
  Dcm_MainConnectionIndexType MainConnectionIndex
)
{

  boolean TransmissionSuccessful = TRUE;

  if (TransmissionResult != DCM_TX_OK)
  {
     TransmissionSuccessful = FALSE;
  }

  /* Set Dcm_ApplicationTransmisionConfirmationEnqueued flag to TRUE. Mark the transmission for
  * being process in the next supervisor main function. */
  Dcm_Dsl_Supervisor_ApplicationTransmisionConfirmationEnqueued_SetFlag(TRUE);

  /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.PriorityNRC.OK,1 */
  /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.PriorityNRC.Failure,1 */
  Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionEnqueued = TRUE;
  Dcm_MainConnectionStatus[MainConnectionIndex].ResponseConfirmationType = DCM_FINALRESPONSE;
  Dcm_MainConnectionStatus[MainConnectionIndex].TransmissionSuccessful = TransmissionSuccessful;

}
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*==================[end of file]================================================================*/
