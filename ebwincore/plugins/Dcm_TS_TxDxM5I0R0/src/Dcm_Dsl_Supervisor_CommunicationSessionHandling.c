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

/* !LINKSTO Dcm.Dsn.File.Supervisor.CommunicationSessionHandling.Impl,1 */
/* This file contains the implementation of the Supervisor CommunicationSessionHandling software
   unit. */

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

#include <Dcm_Dsl_Supervisor_CommunicationSessionHandling.h>   /* Dcm Dsl Supervisor header file */
#include <Dcm_Dsl_ComMChannelStateManager.h>     /* Dcm Dsl CComMChannelStateManager header file */
#include <Dcm_Int.h>                                             /* Declaration of Dcm internals */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                     /* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#endif
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

/*===========================[macro definitions]=================================================*/

/** @defgroup Macro_Dcm_CommunicationSessionStateType Macros for Dcm_CommunicationSessionStateType
 ** \brief    These macros are used as values for Dcm_CommunicationSessionStateType
 **  @{
 */
#if (defined DCM_COMMUNICATION_SESSION_INVALID)
  #error "DCM_COMMUNICATION_SESSION_INVALID is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_INVALID 0U

/* RxConnection not receiving. */
#if (defined DCM_COMMUNICATION_SESSION_IDLE)
  #error "DCM_COMMUNICATION_SESSION_IDLE is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_IDLE 1U

/* RxConnection receiving but not yet finished receiving. */
#if (defined DCM_COMMUNICATION_SESSION_RECEIVING)
  #error "DCM_COMMUNICATION_SESSION_RECEIVING is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_RECEIVING 2U

/* RxConnection finished receiving, an action such as a response is pending on it. */
#if (defined DCM_COMMUNICATION_SESSION_ACTIVE)
  #error "DCM_COMMUNICATION_SESSION_ACTIVE is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_ACTIVE 3U

/* RxConnection finished receiving, an action such as a response is pending on it but during this
   activity a new request started being received.  */
#if (defined DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING)
  #error "DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING 4U

/* RxConnection finished receiving, a BRR is pending on it. */
#if (defined DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST)
  #error "DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST 5U

/* RxConnection finished receiving, a BRR is pending on it but during this activity a new request
   started being received. */
#if (defined DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING)
  #error "DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING 6U

/* RxConnection receiving an diagnostic application request but not yet finished receiving. */
#if (defined DCM_COMMUNICATION_SESSION_APPLICATION_DIAGNOSTIC_REQUEST_IDLE)
  #error "DCM_COMMUNICATION_SESSION_APPLICATION_DIAGNOSTIC_REQUEST_IDLE is already defined"
#endif
#define DCM_COMMUNICATION_SESSION_APPLICATION_DIAGNOSTIC_REQUEST_IDLE 7U

/* @} */
/* End of macros definitions for Dcm_CommunicationSessionStateType */

/*
POSSIBLE TRANSITIONS:

IDLE -> RECEIVING on StartOfReception
IDLE -> APPLICATION_DIAGNOSTIC_REQUEST_IDLE on InjectDiagnosticRequest
APPLICATION_DIAGNOSTIC_REQUEST_IDLE -> RECEIVING
RECEIVING -> IDLE on RxIndication with nothing to process.
RECEIVING -> ACTIVE on RxIndication with something to process
RECEIVING -> ACTIVE_BUSYREPEATREQUEST on RxIndication to send a BUSYREPEATREQUEST
ACTIVE -> IDLE on end of processing while ACTIVE
ACTIVE_BUSYREPEATREQUEST -> IDLE on end of processing while ACTIVE_BUSYREPEATREQUEST
ACTIVE -> ACTIVE_RECEIVING on StartOfReception while it is ACTIVE (only for potential TesterPresent requests)
ACTIVE_BUSYREPEATREQUEST -> ACTIVE_BUSYREPEATREQUEST_RECEIVING on StartOfReception while it is ACTIVE_BUSYREPEATREQUEST (only for potential TesterPresent requests)
ACTIVE_RECEIVING -> ACTIVE on RxIndication while ACTIVE_RECEIVING (only possible scenario since nothing can be processed)
ACTIVE_BUSYREPEATREQUEST_RECEIVING -> ACTIVE_BUSYREPEATREQUEST on RxIndication while ACTIVE_BUSYREPEATREQUEST_RECEIVING (only possible scenario since nothing can be processed)
ACTIVE_RECEIVING -> RECEIVING on end of processing while ACTIVE_RECEIVING.
ACTIVE_BUSYREPEATREQUEST_RECEIVING -> RECEIVING on end of processing while ACTIVE_BUSYREPEATREQUEST_RECEIVING.

*/

/** @defgroup Macro_Dcm_ActiveDiagnosticRequestType Macros for Dcm_ActiveDiagnosticRequestType
 ** \brief    These macros are used as values for Dcm_ActiveDiagnosticRequestType
 **  @{
 */
#if (defined DCM_REQUEST_NOTHING)
  #error "DCM_REQUEST_NOTHING is already defined"
#endif
#define DCM_REQUEST_NOTHING 0U

#if (defined DCM_REQUEST_ACTIVE)
  #error "DCM_REQUEST_ACTIVE is already defined"
#endif
#define DCM_REQUEST_ACTIVE 1U

#if (defined DCM_REQUEST_INACTIVE)
  #error "DCM_REQUEST_INACTIVE is already defined"
#endif
#define DCM_REQUEST_INACTIVE 2U
/* @} */
/* End of macros definitions for Dcm_ActiveDiagnosticRequestType */

/*===========================[type definitions]==================================================*/

typedef uint8 Dcm_CommunicationSessionStateType;

typedef struct
{
  boolean RequestActiveDiagnostic;
  boolean RequestInactiveDiagnostic;
} Dcm_ActionsOnComMChannelsType;

typedef uint8 Dcm_ActiveDiagnosticRequestType;

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerStop(
  Dcm_BufferUserType RxConnectionId
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerRestart(
  Dcm_BufferUserType RxConnectionId
);

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_CommunicationSessionStateType, DCM_VAR) Dcm_CommunicationSessionState[DCM_NUM_RX_CONNECTIONS];

STATIC VAR(boolean, DCM_VAR) Dcm_CancelRxConnection[DCM_NUM_RX_CONNECTIONS];

STATIC VAR(Dcm_ActionsOnComMChannelsType, DCM_VAR) Dcm_ActionsOnComMChannels[DCM_NUM_RX_COMM_CHANNELS];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.Init,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_Init(
  void
)
{
  uint16_least RxConnectionIndex;
  uint8_least ComMChannelIndex;

  /* These are 0-based and consecutive. They can be used as indexes. */
  /* Deviation TASKING-1 <+2> */
  for (RxConnectionIndex = 0U;
       RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
       RxConnectionIndex ++)
  {
    Dcm_CancelRxConnection[RxConnectionIndex] = FALSE;
    Dcm_CommunicationSessionState[RxConnectionIndex] = DCM_COMMUNICATION_SESSION_IDLE;
  }

  /* Deviation TASKING-1 <+2> */
  for (ComMChannelIndex = 0U;
       ComMChannelIndex < DCM_NUM_RX_COMM_CHANNELS;
       ComMChannelIndex ++)
  {
    Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic = FALSE;
    Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic = FALSE;
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.Main,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_MainFunction(
  void
)
{
  uint16_least RxConnectionIndex;
  uint8_least ComMChannelIndex;

  /* These are 0-based and consecutive. They can be used as indexes. */
  /* Deviation TASKING-1 <+2> */
  for (RxConnectionIndex = 0U;
       RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
       RxConnectionIndex ++)
  {
    /* Cancel RxConnections which were marked to be cancelled. */
    if (TRUE == Dcm_CancelRxConnection[RxConnectionIndex])
    {
      Dcm_Dsl_RxConnectionCancelReceive(DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex));
      Dcm_CancelRxConnection[RxConnectionIndex] = FALSE;
    }
  }

  /* Deviation TASKING-1 <+2> */
  for (ComMChannelIndex = 0U;
       ComMChannelIndex < DCM_NUM_RX_COMM_CHANNELS;
       ComMChannelIndex ++)
  {
    Dcm_ActiveDiagnosticRequestType ActiveDiagnosticRequest = DCM_REQUEST_NOTHING;

    /* Request FullComMode or NoComMode on channel if marked. */
    if (
        (TRUE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic) ||
        (TRUE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic)
       )
    {

      /* ENTER critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      if (
          (TRUE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic) &&
          (FALSE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic)
         )
      {
        ActiveDiagnosticRequest = DCM_REQUEST_ACTIVE;
        Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic = FALSE;
      }
      else if (
               (TRUE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic) &&
               (FALSE == Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic)
              )
      {
        ActiveDiagnosticRequest = DCM_REQUEST_INACTIVE;
        Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic = FALSE;
      }
      else
      {
        /* The case in which both a delayed activation and a deactivation were requested.
           Reset both flags. */
        Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic = FALSE;
        Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic = FALSE;
      }

      /* EXIT critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      if (DCM_REQUEST_ACTIVE == ActiveDiagnosticRequest)
      {
        (void)Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic(Dcm_RxComMChannelIDs[ComMChannelIndex]);
      }
      else if (DCM_REQUEST_INACTIVE == ActiveDiagnosticRequest)
      {
        (void)Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic(Dcm_RxComMChannelIDs[ComMChannelIndex]);
      }
      else
      {
        /* The case in which both a delayed activation and a deactivation were requested. Do nothing. */
      }
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.DefaultSessionEntered,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_DefaultSessionEntered(
  void
)
{
  /* Disable ComMChannels which have no more active connections on them. */

  /* The Supervisor is allowed to browse all config data. */

  /* Only call this from a MainFunction context!*/

  uint8_least ChannelIndex;

  /* Deviation TASKING-1 <+2> */
  for (ChannelIndex = 0;
      ChannelIndex < DCM_NUM_RX_COMM_CHANNELS;
      ChannelIndex++ )
  {
    boolean ChannelIsActive = FALSE;
    uint16_least RxConnectionIndex;
    const NetworkHandleType CurrentNetworkId = Dcm_RxComMChannelIDs[ChannelIndex];

    /* Search to see if any CommunicationSession is still active for this ComMChannel. If it
       is then InactiveDiagnostic shouldn't be requested for this ComMChannel.

       Don't request InactiveDiagnostic for all channels directly, because when you have default
       session and OBD and UDS on the same channel in parallel, you shut down OBD's
       channel while UDS is still running or vice-versa. */

    /* Deviation TASKING-1 <+2> */
    for (RxConnectionIndex = 0;
         RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
         RxConnectionIndex ++)
    {
      Dcm_RxPduIdInfoType SearchRxPduIdInfo;

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(
        DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex),
        &SearchRxPduIdInfo);

      /* If any RxConnection referencing the same ComMChannel is anything but Idle,
         the Dcm most likely has requested ActiveDiagnostic for it already. */
      if (CurrentNetworkId == SearchRxPduIdInfo.ComMChannelId)
      {
        if ((DCM_COMMUNICATION_SESSION_ACTIVE ==
              Dcm_CommunicationSessionState[RxConnectionIndex]) ||
            (DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING ==
              Dcm_CommunicationSessionState[RxConnectionIndex])
           )
        {
          /* If a channel is still active here, as soon as the communication
             session keeping it active ends, it will go down.
           */
          ChannelIsActive = TRUE;
          break;
        }
      }
    }

    if (FALSE == ChannelIsActive)
    {
      /* All RxConnections of this channel are inactive. If an RxConnection on the channel is active,
        the deactivation of the RxConnection will cause the ComM switch. */

      /* Note: this call is immediate. If any interruption by any of the RxConnections has changed
               the situation of ActiveDiagnostic calls up to now (likely preemption scenario), that
               same condition will set the flag to request ActiveDiagnostic in the next MainFunction
               cycle. */

      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NonDefaultSession.OBDRequestReceived.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ProtocolPreemption.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NonDefaultSession.S3Timeout.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ResetToDefaultSessionAPI.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ResetToDefaultSessionClientServerOperation.InactiveDiagnostic,1 */

      (void)Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic(CurrentNetworkId);
    }
  }
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_PrepareApplicationDiagnosticRequest(
  Dcm_BufferUserType RxConnectionId
)
{
  if(DCM_COMMUNICATION_SESSION_IDLE == Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)])
  {
    Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
      DCM_COMMUNICATION_SESSION_APPLICATION_DIAGNOSTIC_REQUEST_IDLE;
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerStop(RxConnectionId);
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.ReceptionBegin,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionBegin(
  Dcm_BufferUserType RxConnectionId
)
{
  switch (Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)])
  {
    case DCM_COMMUNICATION_SESSION_IDLE:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_RECEIVING;

      Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerStop(RxConnectionId);
      break;

    case DCM_COMMUNICATION_SESSION_APPLICATION_DIAGNOSTIC_REQUEST_IDLE:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_RECEIVING;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.ReceptionEnd,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
  Dcm_BufferUserType RxConnectionId
)
{
  Dcm_SesCtrlType CurrentSession;

  switch (Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)])
  {
    case DCM_COMMUNICATION_SESSION_RECEIVING:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_IDLE;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_ACTIVE;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  /* Since reception just ended, no need to cancel anything anymore, so clear
     the flag just in case. */

  Dcm_CancelRxConnection[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
      FALSE;

  CurrentSession = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

  if (DCM_DEFAULT_SESSION != CurrentSession)
  {
    /* See if the S3Timer needs to be restarted. */
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerRestart(RxConnectionId);
  }

}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.ProcessingBegin,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingBegin(
  Dcm_BufferUserType RxConnectionId,
  boolean SendingBusyRepeatRequest
)
{
  if (TRUE == SendingBusyRepeatRequest)
  {
    /* !LINKSTO SWS_Dcm_01050,1 */
    /* Don't request ActiveDiagnostic for these. InactiveDiagnostic won't be requested without
       ActiveDiagnostic being requested first. */
    Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
      DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST;
  }
  else
  {
    uint8_least ComMChannelIndex = 0U;
    boolean ChannelIsAlreadyActive = FALSE;
    boolean ChannelFound = FALSE;
    Dcm_RxPduIdInfoType RxPduIdInfo;

    Dcm_Dsl_RxConnectionGetRxPduIdInformation(
      RxConnectionId,
      &RxPduIdInfo);

    /* Note: SWS_Dcm_01373 does this in any session now... */

    /* This ComMChannel was not activated. Time to activate it. This function is almost always
       called in an interrupt context of the RxConnection. Sync the action with the MainFunction. */

    while((ComMChannelIndex < DCM_NUM_RX_COMM_CHANNELS) && (ChannelFound == FALSE))
    {
      if (Dcm_RxComMChannelIDs[ComMChannelIndex] == RxPduIdInfo.ComMChannelId)
      {
        Dcm_RxConnectionIndexType RxConnectionIndex;
        ChannelFound = TRUE;

        /* Deviation TASKING-1 <+2> */
        for (RxConnectionIndex = 0;
             RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
             RxConnectionIndex ++)
        {
          Dcm_RxPduIdInfoType SearchRxPduIdInfo;

          Dcm_Dsl_RxConnectionGetRxPduIdInformation(
            DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex),
            &SearchRxPduIdInfo);

          /* If any RxConnection referencing the same ComMChannel is anything but Idle,
             the Dcm has requested ActiveDiagnostic for it already. */
          if (Dcm_RxComMChannelIDs[ComMChannelIndex] == SearchRxPduIdInfo.ComMChannelId)
          {

            if ((DCM_COMMUNICATION_SESSION_ACTIVE ==
                  Dcm_CommunicationSessionState[RxConnectionIndex]) ||
                (DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING ==
                  Dcm_CommunicationSessionState[RxConnectionIndex])
               )
            {
              ChannelIsAlreadyActive = TRUE;
              break;
            }
          }
        }
      }
      /* Channel was not yet found. */
      else
      {
        ComMChannelIndex++;
      }
    }

    if (FALSE == ChannelIsAlreadyActive)
    {
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ImmediateApplicationRequest.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.DelayedApplicationRequest.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ImmediateApplicationRequest.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.DelayedApplicationRequest.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromBootloader.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromECUReset.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromBootloader.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromECUReset.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */

      Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestActiveDiagnostic = TRUE;
    }

    switch (Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)])
    {
      case DCM_COMMUNICATION_SESSION_RECEIVING:
        Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
          DCM_COMMUNICATION_SESSION_ACTIVE;
        break;

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.EndOfCommunicationSession,1 */
/* Just means that the processing on an RxConnection communication session ended, regardless of which
   type of request this was. */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
  Dcm_BufferUserType RxConnectionId
)
{

  switch (Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)])
  {
    case DCM_COMMUNICATION_SESSION_IDLE:
      /* This case handles the situation in which a protocol is started by a ROE TYPE1
       * or a periodic TYPE1 and the Start/Stop protocol callbacks fail, therefore,
       * the protocol is not started and that means the Dcm_CommunicationSessionState
       * has to remain DCM_COMMUNICATION_SESSION_IDLE. */
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_IDLE;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_RECEIVING;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_IDLE;
      break;

    case DCM_COMMUNICATION_SESSION_ACTIVE_BUSYREPEATREQUEST_RECEIVING:
      Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)] =
        DCM_COMMUNICATION_SESSION_RECEIVING;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.ProcessingEnd,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(
  Dcm_BufferUserType RxConnectionId
)
{
  Dcm_SesCtrlType CurrentSession;

  boolean ChannelIsActive = FALSE;
  boolean ChannelFound = FALSE;
  uint16_least RxConnectionIndex;

  CurrentSession = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

  if (DCM_DEFAULT_SESSION != CurrentSession)
  {
    /* See if the S3Timer needs to be restarted. */
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerRestart(RxConnectionId);
  }
  else
  {
    Dcm_RxPduIdInfoType RxPduIdInfo;

    Dcm_Dsl_RxConnectionGetRxPduIdInformation(
      RxConnectionId,
      &RxPduIdInfo);

    /* Search to see if any CommunicationSession is still active for this ComMChannel. If it
       is then InactiveDiagnostic shouldn't be requested for this ComMChannel.

       Don't request InactiveDiagnostic for all channels directly, because when you have default
       session and OBD and UDS on the same channel in parallel, you shut down OBD's
       channel while UDS is still running or vice-versa.
    */

    /* Deviation TASKING-1 <+2> */
    for (RxConnectionIndex = 0;
         RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
         RxConnectionIndex ++)
    {
      Dcm_RxPduIdInfoType SearchRxPduIdInfo;

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(
        DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex),
        &SearchRxPduIdInfo);

      /* If any RxConnection referencing the same ComMChannel is anything but Idle,
         the Dcm most likely has requested ActiveDiagnostic for it already. Communication sessions
         which are active sending a BusyRepeatRequest are ignored since they should not hold
         the channel in full comm mode anyway. */
      if (
          (RxPduIdInfo.ComMChannelId == SearchRxPduIdInfo.ComMChannelId) &&
          ((DCM_COMMUNICATION_SESSION_ACTIVE ==
              Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionIndex)]) ||
           (DCM_COMMUNICATION_SESSION_ACTIVE_RECEIVING ==
              Dcm_CommunicationSessionState[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionIndex)])
          )
         )
      {
        ChannelIsActive = TRUE;
      }
    }

    if (FALSE == ChannelIsActive)
    {
      /* This ComMChannel was activated. Time to deactivate it. Do this in the next MainFunction
         cycle.

         Rationale: Supervisor is scheduled after HSM anyway*/

      uint8_least ComMChannelIndex = 0U;

      while((ComMChannelIndex < DCM_NUM_RX_COMM_CHANNELS) && (ChannelFound == FALSE))
      {

        if (Dcm_RxComMChannelIDs[ComMChannelIndex] == RxPduIdInfo.ComMChannelId)
        {
          ChannelFound = TRUE;
        }
        /* Channel was not yet found. */
        else
        {
          ComMChannelIndex++;
        }
      }
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedFunctionalNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedOBDNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.ProcessingCancelled.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.ManufacturerAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SupplierAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedFunctionalNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedOBDNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.ProcessingCancelled.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.ManufacturerAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SupplierAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.PreemptionTimeoutBBR.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.PreemptionTimeoutBBR.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.ProtocolStopConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.ProtocolStopConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm170,2 */

      Dcm_ActionsOnComMChannels[ComMChannelIndex].RequestInactiveDiagnostic = TRUE;
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.EnqueueCancellationOfRxConnection,1 */
FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_EnqueueCancellationOfRxConnection(
    Dcm_RxConnectionIndexType RxConnectionIndex
)
{
  /* Cancellation shall be requested in sync with the MainFunction. */
  Dcm_CancelRxConnection[RxConnectionIndex] = TRUE;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.IsCancellationEnqueuedForRxConnection,1 */
FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_IsCancellationEnqueuedForRxConnection(
    Dcm_RxConnectionIndexType RxConnectionIndex
)
{
  return Dcm_CancelRxConnection[RxConnectionIndex];
}
#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.CheckS3TimerStop,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerStop(
  Dcm_BufferUserType RxConnectionId
)
{
  /* Stop S3 timer, if
   * - request is a normal diagnostic request (i.e. neither ROE nor PR)
   * - a non-default session is active
   * - request is received on the same MainConnection which changed the session to non-default
   */

  Dcm_SesCtrlType CurrentSession =
    Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

  /* Get the MainConnection which changed the session to non-default */
  Dcm_MainConnectionIndexType MainConnectionIndex =
    Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection();

  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ExternalRequestReception.NotTesterPresent.Stop,3 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ImmediateApplicationRequest.NotTesterPresent.Stop,3 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.DelayedApplicationRequest.NotTesterPresent.Stop,3 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ExternalRequestReception.PotentialTesterPresent.Stop,3 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ImmediateApplicationRequest.PotentialTesterPresent.Stop,3 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.DelayedApplicationRequest.PotentialTesterPresent.Stop,3 */

  if ((DCM_DEFAULT_SESSION != CurrentSession) &&
      (MainConnectionIndex == Dcm_RxPduIdConfig[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)].MainConnectionIndex))
  {
    (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(
            DCM_SUPERVISOR_ID,
            DCM_INVALID_PROTOCOL_INDEX, /* unused for S3TIMER */
            DCM_S3TIMER
          );
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.CommunicationSessionHandling.CheckS3TimerRestart,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_CheckS3TimerRestart(
  Dcm_BufferUserType RxConnectionId
)
{
  /* Start S3 timer, if
   * - processed request is a normal diagnostic request (i.e. neither ROE nor PR)
   * - a non-default session is active
   * - processed request is received on the same MainConnection which changed the session to non-default
   * - no new request is in reception and no request is enqueued on this MainConnection
   */
  boolean CommunicationSessionIsActive = FALSE;
  uint16_least RxConnectionIndex;

  /* Get the MainConnection which changed the session to non-default */
  Dcm_MainConnectionIndexType MainConnectionIndex =
    Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection();

  if (MainConnectionIndex == Dcm_RxPduIdConfig[DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)].MainConnectionIndex)
  {
    /* Only care about communication sessions which end on the same MainConnection
     * which changed the session to non-default
     */
    /* Deviation TASKING-1 <+2> */
    for (RxConnectionIndex = 0;
         RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
         RxConnectionIndex ++)
    {
      if ((DCM_COMMUNICATION_SESSION_IDLE != Dcm_CommunicationSessionState[RxConnectionIndex]) &&
          (MainConnectionIndex == Dcm_RxPduIdConfig[RxConnectionIndex].MainConnectionIndex)
         )
      {
        CommunicationSessionIsActive = TRUE;
        break;
      }
    }

    /* Via SessionManager, reset any running S3 Timer. */
    if (FALSE == CommunicationSessionIsActive)
    {
      /* We're in a non-default session and have just been left with no running
         communication sessions. Reset the S3 timer. */

      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.UnsuccessfulReception.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.External.WrongSizeReception.Restart,1 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ConcurrentTesterPresent.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ImmediateApplicationRequest.ConcurrentTesterPresent.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.DelayedApplicationRequest.ConcurrentTesterPresent.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.PotentialConcurrentTesterPresent.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.CancelledReception.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.NoComMode.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.BusyMainConnection.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.BusyProtocol.NoBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.SuppressedResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProcessingComplete.ManufacturerAbortWithoutResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProcessingComplete.SupplierAbortWithoutResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProcessingComplete.TxBufferBusy.NoBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ApplicationRequest.SuppressedResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ApplicationRequest.ProcessingComplete.ManufacturerAbortWithoutResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ApplicationRequest.ProcessingComplete.SupplierAbortWithoutResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.ApplicationRequest.ProcessingComplete.TxBufferBusy.NoBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProcessingComplete.TxConfirmation.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.ApplicationRequest.ProcessingComplete.TxConfirmation.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.PostBootResponse.TxConfirmation.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.PostBootResponse.SuppressedPositiveResponse.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProtocolPreemption.StartOfReceptionBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.External.RxBufferBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.S3ServerHandling.External.ProtocolPreemption.RxIndicationBRR.Restart,3 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.External.TxBufferBRR.Restart,1 */
      /* !LINKSTO Dcm.EB.SessionStateHandling.ApplicationRequest.TxBufferBRR.Restart,1 */

      (void)Dcm_Dsl_DiagnosticSessionHandler_StartS3Timer(
              DCM_S3SERVER_TICK_DEFAULT
            );
    }
  }
}

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]================================================================*/
