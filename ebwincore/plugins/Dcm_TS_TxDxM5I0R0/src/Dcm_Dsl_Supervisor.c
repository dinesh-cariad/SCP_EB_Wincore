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

/* !LINKSTO Dcm.Dsn.File.Supervisor.Core.Impl,1 */
/* This file contains the implementation of the Supervisor Core software unit. */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <ComStack_Types.h>                                 /* AUTOSAR Communication Stack types */
#include <TSAutosar.h>

#include <Dcm_Dsl_Supervisor.h>                                /* Dcm Dsl Supervisor header file */

#include <Dcm_Dsl_Supervisor_CommunicationSessionHandling.h>                                /* Dcm Dsl Supervisor header file */
#include <Dcm_Dsl_Supervisor_MainConnectionHandling.h>   /* Dcm Dsl Supervisor Main Connection Handling header file */

#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>                                /* Dcm Dsl Supervisor header file */

#include <Dcm_Dsl_ComMChannelStateManager.h>                /* Dcm Dsl CComMChannelStateManager header
                                                             file */

#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_Int.h>                                             /* Internal declarations of Dcm */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */

#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif
/*===========================[macro definitions]=================================================*/

/*===========================[type definitions]==================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC (Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_DetermineRequestType(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
);

STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationPostBootRequest(
  void
);

STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationNormalRequest(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType Length
);

STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE1ROE(
  Dcm_BufferUserType RxConnectionId
);

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE2ROE(
  Dcm_BufferUserType RxConnectionId
);
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.PrearbitrationTYPE1Periodic,1 */
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE1Periodic(
  Dcm_BufferUserType RxConnectionId
);

#if(DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.PrearbitrationTYPE2Periodic,1 */
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE2Periodic(
  Dcm_BufferUserType RxConnectionId
);
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationPostBootRequest(
  void
);

STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON)|| (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessTYPE2Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON)|| (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ResetDiagnosticSession(
  void
);

STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_Supervisor_RequestIsPotentialConcurrentTesterPresent(
  PduIdType RxPduId,
  PduLengthType Length
);

STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetProcessingProtocolIndexForTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType
);

STATIC FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RearbitrationTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType
);

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionNormalRequest(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType Length,
  Dcm_RequestTypeType InitialRequestType
);

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE1ROERequest(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType InitialRequestType
);

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE2ROERequest(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType InitialRequestType
);
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionPostBootRequest(
  Dcm_RequestTypeType InitialRequestType
);

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE1PeriodicRequest(
  Dcm_BufferUserType RxConnectionId
);

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE2PeriodicRequest(
  Dcm_BufferUserType RxConnectionId
);
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_EnqueueTYPE1RequestAsPresumptive(
  Dcm_BufferUserType RxConnectionId,
  Dcm_ProtocolIndexType ProcessingProtocolIndex,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
);

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>
/** \brief Indicates if the reset of diagnostic session is enqueued.
  *        TRUE - Reset of diagnostic session is enqueued for being process in the next
  *               Dcm_Dsl_DiagnosticSessionHandler_S3TimerHandlingMainFunction() call.
  *        FALSE - Reset of diagnostic session is not enqueued.
  */
STATIC VAR(boolean, DCM_VAR) Dcm_ResetDiagnosticSession = FALSE;

/** \brief Inhibits Request Processing
  *        TRUE - Request processing not allowed
  *        FALSE - Request processing allowed
  */
STATIC VAR(boolean, DCM_VAR) Dcm_InhibitRequestProcessing = FALSE;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/** \brief Mark if there is an application transmission enqueued
  *         TRUE -  there is a transmission enqueued for being process in the supervisor main
  *                 function
  *         FALSE - there is no transmissions enqueued for being process in the supervisor main
  *                 function
  */
STATIC VAR(boolean, DCM_VAR) Dcm_ApplicationTransmisionConfirmationEnqueued = FALSE;
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */
#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsl_Supervisor_Init(void)
{
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_Init();

  Dcm_Dsl_Supervisor_ProtocolHandling_Init();

  Dcm_Dsl_Supervisor_MainConnectionHandling_Init();

  Dcm_Dsl_Supervisor_EnqueueResetOfDiagnosticSession();

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  Dcm_ApplicationTransmisionConfirmationEnqueued = FALSE;
#endif
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_PreProcessingMainFunction(void)
{
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  /* Check if there is a transmission enqueued that need to be processed in this main function. */
  if(Dcm_ApplicationTransmisionConfirmationEnqueued == TRUE)
  {
    /* Iterate over all main connections and check for enqueued transmissions. */
    Dcm_Dsl_Supervisor_MainConnectionHandling_PreProcessinMainFunction();
  }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  /* Try to only call APIs of other modules outside of exclusive areas. */
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_MainFunction();

  /* This function only does something when there are MainConnections which have any operations to execute. This can be used for optimization. Call outside of exclusive areas only. */
  Dcm_Dsl_Supervisor_MainConnectionHandling_MainFunction();

  /* Check if the session needs to be reset as result of requests, etc */

  /*   This doesn't need to be flagged for optimization as it contains its own flag! */
  Dcm_Dsl_Supervisor_ResetDiagnosticSession();

  /* This function only does something when there are presumptive protocols in the stacks, i.e.
     when you have Queued Protocols. This can be used for optimization. */
  Dcm_Dsl_Supervisor_ProtocolHandling_MainFunction();

  /* After this happens, it's possible that preemptions or cancellations
     occured.
     these might carry with them the need to switch the DiagnosticSession.

     This doesn't need to be flagged for optimization as it contains its own flag! */
  Dcm_Dsl_Supervisor_ResetDiagnosticSession();

}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_PostProcessingMainFunction(void)
{
  /* Try to only call APIs of other modules outside of exclusive areas. */
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_MainFunction();

  /* This function only does something when there are MainConnections which have any operations to execute. This can be used for optimization. Call outside of exclusive areas only. */
  Dcm_Dsl_Supervisor_MainConnectionHandling_MainFunction();
}

FUNC(void, DCM_CODE) Dcm_Dsl_Supervisor_InhibitRequestProcessing(void)
{
  Dcm_InhibitRequestProcessing = TRUE;
}

FUNC(boolean, DCM_CODE) Dcm_Dsl_Supervisor_RequestProcessingInhibited(void)
{
  return Dcm_InhibitRequestProcessing;
}

FUNC(void, DCM_CODE) Dcm_Dsl_Supervisor_DisinhibitRequestProcessing(void)
{
  Dcm_InhibitRequestProcessing = FALSE;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.RequestRxPermission.Reject,1 */
FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RequestRxPermission(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType Length,
  Dcm_RequestOriginType RequestOrigin
)
{
  /* Decides whether:
     - The current reception may begin
     - How the received request is to be processed */

  /* Determine, based on the origin and the RxConnection, which type
     of request we have. */

  const Dcm_RequestTypeType InitialRequestType = Dcm_Dsl_Supervisor_DetermineRequestType(
                                                   RxConnectionId,
                                                   RequestOrigin);

  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  /* <id>Dcm.EB.EcuReset.Success.NewStartOfReceptionCall.NoSuppressedResp</id>
  */

  if (
      (TRUE != Dcm_InhibitRequestProcessing) ||
      (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == InitialRequestType) ||
      (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == InitialRequestType)
     )
  {

    /* Based on the determined request type, perform the necessary arbitration routines. */
    switch (InitialRequestType)
    {
      case DCM_NORMAL_PHYSICAL_REQUEST:
      case DCM_NORMAL_FUNCTIONAL_REQUEST:
        AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionNormalRequest(
                                   RxConnectionId,
                                   Length,
                                   InitialRequestType
                                 );
        break;

      case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
      case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
        AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionTYPE1ROERequest(
                                   RxConnectionId,
                                   InitialRequestType
                                 );
        break;

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
      case DCM_ROE_TYPE2_PHYSICAL_REQUEST:
      case DCM_ROE_TYPE2_FUNCTIONAL_REQUEST:
        AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionTYPE2ROERequest(
                                   RxConnectionId,
                                   InitialRequestType
                                 );
        break;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

      case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:
      case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
        AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionPostBootRequest(
                                   InitialRequestType
                                 );

        break;

      case DCM_PERIODIC_TYPE1_REQUEST:
        AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionTYPE1PeriodicRequest(
                                   RxConnectionId
                                 );

        break;

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
      case DCM_PERIODIC_TYPE2_REQUEST:
       AcceptAsRequestType = Dcm_Dsl_Supervisor_RxPermissionTYPE2PeriodicRequest(
                                   RxConnectionId
                                 );
       break;
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TpTxConfirmation.ReturnsE_OK.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TesterSourceAddress.NotConfigured.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.TesterSourceAddress.NotConfigured.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.PduR_DcmTransmit.ReturnsNotE_OK.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TpTxConfirmation.ReturnsNotE_OK.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceOk.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceOk.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceNotOk.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceNotOk.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.TesterSourceAddress.NotConfigured.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.SuppressedResponse.TesterSourceAddress.NotConfigured.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.SuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceOk.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.SuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceOk.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceNotOk.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.SuppressedResponse.DcmComMMultiCoreSupportEnabled.SchMInterfaceNotOk.FullCommunicationDisabled.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.PduR_DcmTransmit.ReturnsNotE_OK.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.TpTxConfirmation.ReturnsNotE_OK.Discard,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.NewTYPE2ROEEvent.UnsuppressedResponse.TpTxConfirmation.ReturnsE_OK.Discard,1 */

    /* !LINKSTO Dcm.EB.UDSServices.Success.NewStartOfReceptionCall.UnsuppressedResponse.RejectRx,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewStartOfReceptionCall.SuppressedResponse.RejectRx,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE1ROEEvent.UnsuppressedResponse.Discard,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE1ROEEvent.SuppressedResponse.Discard,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE2ROEEvent.UnsuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE2ROEEvent.SuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.EcuReset.Success.TYPE2Periodic.UnsuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.EcuReset.Success.TYPE2Periodic.SuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.EcuReset.Success.TYPE1Periodic.UnsuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.EcuReset.Success.TYPE1Periodic.SuppressedResponse.Discard,1 */
    /* Reject this. */
    AcceptAsRequestType = DCM_REJECTED_REQUEST;
  }

  return AcceptAsRequestType;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_IncomingReception(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType)
{
  if ((DCM_CONCURRENT_TESTERPRESENT_REQUEST == RequestType) ||
      (DCM_BUSYREPEATREQUEST_REQUEST == RequestType) ||
      (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
      (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType) ||
      (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType) ||
      (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType)
     )
  {

    Dcm_RxPduIdInfoType RxPduIdInfo;

    Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

    /* ROE and Periodic requests have no impact. */
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionBegin(RxConnectionId);
  }
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_AnnounceIncomingApplicationDiagnosticRequest(
  Dcm_BufferUserType RxConnectionId)
{
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_PrepareApplicationDiagnosticRequest(RxConnectionId);
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_IncomingReceptionFinished(
Dcm_BufferUserType RxConnectionId)
{
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(RxConnectionId);
}

FUNC(Dcm_BufferUserType, DCM_CODE) Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol(
  void
)
{
  Dcm_BufferUserType ProtocolId =
    DCM_PROTOCOL_ID_FROM_INDEX(Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolIndex());

  return ProtocolId;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocolConfig(
  Dcm_ProtocolConfigPtrType CurrentProtocolConfig)
{
  Std_ReturnType GetProtocolConfigResult = DCM_E_NOT_OK;

  /* Just a wrapper for this call. */
  GetProtocolConfigResult =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolConfig(CurrentProtocolConfig);

  return GetProtocolConfigResult;
}

FUNC(Dcm_ProtocolGroupType, DCM_CODE) Dcm_Dsl_Supervisor_GetProtocolGroupTypeForProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* Just a wrapper for this call. */
  Dcm_ProtocolGroupType ProtocolGroup =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(ProtocolIndex);

  return ProtocolGroup;
}

FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
)
{

  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ProtocolIndexType ProcessingProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
  Dcm_MainConnectionIndexType MainConnectionIndex;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);
  MainConnectionIndex = Dcm_RxPduIdConfig[RxConnectionId].MainConnectionIndex;

  switch (RequestOrigin)
  {
    case DCM_EXTERNAL:
    case DCM_INTERNAL_POST_BOOT_NO_ANSWER:
    case DCM_INTERNAL_POST_BOOT_WITH_ANSWER:
      ProcessingProtocolIndex = Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex;
      break;

    case DCM_INTERNAL_ROE:
    {
#if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
      if (DCM_INVALID_ROE_CONNECTION != Dcm_MainConnectionConfig[MainConnectionIndex].RoeConnectionIndex)
      {
        /* store the dedicated ROE protocol */
        ProcessingProtocolIndex =
          Dcm_RoeConnectionConfig[Dcm_MainConnectionConfig[MainConnectionIndex].RoeConnectionIndex].ProtocolIndex;
      }
      else
#endif /* #if (DCM_NUM_ROE_CONNECTIONS > 0U) */
      {
        ProcessingProtocolIndex = Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex;
      }

      break;
    }

    case DCM_INTERNAL_PERIODIC:
    {
#if ( (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
        if (DCM_INVALID_PERIODIC_TRANSMISSION != Dcm_MainConnectionConfig[MainConnectionIndex].PeriodicTransmissionIndex)
        {
          /*If the periodic transmission is processed on another protocol than the one on which
           * it was requested, get the index of that protocol */
          ProcessingProtocolIndex =
            Dcm_PeriodicTransmissionConfig[Dcm_MainConnectionConfig[MainConnectionIndex].PeriodicTransmissionIndex].ProtocolIndex;
        }
        else
#endif /* ( (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */
        {
          ProcessingProtocolIndex = Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex;
        }

      /* Reception permission is given for internal requests if the ComMChannel referenced by the RxConnection is
         allowed to transmit data by ComM. */

      break;
    }

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return ProcessingProtocolIndex;
}

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_IsTYPE2ROEPresent(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  DCM_PRECONDITION_ASSERT(ProtocolIndex <= DCM_NUM_PROTOCOLS, DCM_INTERNAL_API_ID);

  /* Wrapper */
  return Dcm_Dsl_Supervisor_ProtocolHandling_IsTYPE2ROEPresent(ProtocolIndex);
}
#endif

FUNC (Dcm_SupervisorProtocolRequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_GetProtocolRequestType(
  Dcm_BufferUserType ProtocolId
)
{
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);

  Dcm_SupervisorProtocolRequestTypeType ProtocolRequestType =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolRequestType(ProtocolIndex);

  return ProtocolRequestType;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_P2TimerTimeout(
  Dcm_TimerIndexType TimerIndex
)
{
  /* Wrapper */
  Dcm_Dsl_Supervisor_ProtocolHandling_P2TimerTimeout(TimerIndex);
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_TransmissionFinished(
  Dcm_BufferUserType TxConnectionId,
  Dcm_TxResultType TransmissionResult
)
{
  uint16_least MainConnectionIndex = 0;
  boolean TransmissionDone = FALSE;

  while((MainConnectionIndex < DCM_NUM_MAIN_CONNECTIONS) && (TransmissionDone == FALSE))
  {

    if (Dcm_MainConnectionConfig[MainConnectionIndex].ConfirmationTxPduId ==
          DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId))
    {

      /* This just gets forwarded to the MainConnectionHandling subsystem. */
      Dcm_Dsl_Supervisor_MainConnectionHandling_TransmissionFinished(
          (Dcm_MainConnectionIndexType)(MainConnectionIndex),
          TransmissionResult
        );

      /* Mark transmission done. MainConnection found. */
      TransmissionDone = TRUE;

    }
    else
    {
      MainConnectionIndex++;
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.ProcessRequest,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessRequest(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ProtocolIndexType RxProtocolIndex;
  Dcm_RxConnectionStatusType RxConnectionState = Dcm_Dsl_RxConnectionGetState(RxConnectionId);
  Dcm_ComMChannelOperationStateType ComMChannelRxState;
  boolean MainConnectionBusy;
  boolean RxConnectionCancellationEnqueued =
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_IsCancellationEnqueuedForRxConnection(
        DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)
      );
  boolean ProtocolBusyWithType1ROE;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  ComMChannelRxState = Dcm_Dsl_ComMChannelStateManager_GetRxState(RxPduIdInfo.ComMChannelId);

  MainConnectionBusy = Dcm_Dsl_Supervisor_MainConnectionHandling_MainConnectionBusy(RxPduIdInfo.MainConnectionIndex);

  RxProtocolIndex = RxPduIdInfo.ProtocolIndex;
  ProtocolBusyWithType1ROE = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithType1ROE(RxProtocolIndex);
  DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_ENTRY(RxConnectionId, RequestType, RequestBuffer);

  /* These are all the cases in which the request is simply dropped from the get-go.
     This restarts S3.
   */
  if(
     (
      /* Request is a functional TesterPresent */
      (DCM_FUNCTIONAL_ADDRESSING == Dcm_RxPduIdConfig[RxConnectionId].AddressingType) &&
      (DCM_TESTER_PRESENT_REQ_LEN == RequestBuffer.FilledLength) &&
      (DCM_SID_TESTER_PRESENT == RequestBuffer.BufferPtr[DCM_DIAG_REQ_SID_IND]) &&
      (DCM_SUPPOSRES_BITMASK == RequestBuffer.BufferPtr[DCM_DIAG_REQ_SUBFUNC_IND])
     ) ||
     (
      (
       (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
       (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType) ||
       (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType) ||
       (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType)
      ) &&
      (
       (
         /* The RxConnection got cancelled in the meantime, either the
            call to the PduR was made for this, or is pending. */
         (TRUE == RxConnectionCancellationEnqueued) ||
         (DCM_RX_CONNECTION_CANCELLED == RxConnectionState)
       ) ||
       (
         /* The MainConnection became busy in the meantime with not a TYPE1 ROE request*/
         (TRUE == MainConnectionBusy) &&
         (FALSE == ProtocolBusyWithType1ROE)
       )
      )
     ) ||
     (
      /* For external and application-generated requests, if the ComMChannel for Rx went
         to NoComMode in the meantime. */
      (
       (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
       (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType)
      ) &&
      (DCM_COMM_OPERATION_ENABLED != ComMChannelRxState)
     )
    )
  {
    /* Even though previously this request might have been accepted as something else, it has now
       been identified as a TesterPresent request and shall be handled accordingly. */

    /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.ConcurrentTesterPresent.Discard,1 */
    /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.AlreadyCancelled.Discard,1 */
    /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.NoComMode.Discard,1 */
    /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.MainConnectionBusy.NotTYPE1ROE.Discard,1 */

    /* Free RxBuffer */
    (void)Dcm_Dsl_BufferFree(DCM_SUPERVISOR_ID,
                             RequestBuffer.BufferId);

    Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
        DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId)
      );
  }
  else
  {
    switch (RequestType)
    {
      /* Replace with an internal "find out what this is" */
      case DCM_NORMAL_PHYSICAL_REQUEST:
      case DCM_NORMAL_FUNCTIONAL_REQUEST:

        Dcm_Dsl_Supervisor_ProcessTYPE1Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;

      case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
      case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:

        Dcm_Dsl_Supervisor_ProcessTYPE1Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
      case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
      case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
        Dcm_Dsl_Supervisor_ProcessTYPE1Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
      case DCM_ROE_TYPE2_PHYSICAL_REQUEST:
      case DCM_ROE_TYPE2_FUNCTIONAL_REQUEST:

        Dcm_Dsl_Supervisor_ProcessTYPE2Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON)*/
#endif /* #if (DCM_DSP_USE_ROESERVICES == STD_ON) */

#if (STD_ON == DCM_PERIODIC_USED)
      case DCM_PERIODIC_TYPE1_REQUEST:
        Dcm_Dsl_Supervisor_ProcessTYPE1Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
      case DCM_PERIODIC_TYPE2_REQUEST:
        Dcm_Dsl_Supervisor_ProcessTYPE2Request(
          RxConnectionId,
          RequestType,
          RequestBuffer
        );
        break;
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */
#endif /* #if (STD_ON == DCM_PERIODIC_USED) */

      case DCM_BUSYREPEATREQUEST_REQUEST:
      {
        Dcm_TimerValueType  NRCTimeout = Dcm_Dsl_Supervisor_MainConnectionHandling_GetNRCTimeout(RxProtocolIndex);

        /* Communication session begins, we DO NOT request FullCommunication for this type
           of response. */
        Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingBegin(
          RxConnectionId,
          TRUE);

        /* Since this is done in the context of an ISR, the NRC transmission
           must be delayed until the next Dcm_MainFunction() cycle.
         */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForBRRTransmission.NotConcurentTesterPresent.TransmitBRR,1 */
        Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
            RxConnectionId,
            RequestBuffer,
            DCM_E_BUSYREPEATREQUEST,
            NRCTimeout
          );
        break;
      }

      case DCM_CONCURRENT_TESTERPRESENT_REQUEST:
        /* A potential tester present request, but which turned out to be
           some other type of request is ignored. */

        /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.PreviousPotentialConcurrentTesterPresent.Discard,1 */
        Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
          RxConnectionId);
        break;

      /* CHECK: NOPARSE */
      default:
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
      /* CHECK: PARSE */
    }
  }
  DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_EXIT(RxConnectionId, RequestType, RequestBuffer);
}

FUNC (Dcm_SessionSwitchPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_IsSessionClearedToSwitch(
  Dcm_SesCtrlType NewSession
)
{

  Dcm_SessionSwitchPermissionType IsSessionClearedToSwitch =
    Dcm_Dsl_Supervisor_ProtocolHandling_IsSessionClearedToSwitch(NewSession);

  return IsSessionClearedToSwitch;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolFree(Dcm_BufferUserType ProtocolID)
{
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolID);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolFree(ProtocolIndex);

  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessingEnd(Dcm_BufferUserType RxConnectionId)
{
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(RxConnectionId);

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_DefaultSessionEntered(
  void
)
{
  /* This just gets passed on to ReceptionHandling since it's the only
     subunit interested in this. Depending on the state of RxConnections
     it will either request InactiveDiagnostic or not. */
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_DefaultSessionEntered();
}

FUNC(Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetProtocolIndexFromRxPduIdAndRequestType(
  PduIdType RxPduId,
  uint8 MessageType)
{
  Dcm_ProtocolIndexType ProtocolIndex;

#if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
  if ((DCM_ROE == MessageType) &&
      (DCM_INVALID_ROE_CONNECTION != Dcm_MainConnectionConfig[
       Dcm_RxPduIdConfig[RxPduId].MainConnectionIndex].RoeConnectionIndex))
  {
    ProtocolIndex = Dcm_RoeConnectionConfig[Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      RxPduId].MainConnectionIndex].RoeConnectionIndex].ProtocolIndex;
  }
  else
#else
  TS_PARAM_UNUSED(MessageType);
#endif
  {
    ProtocolIndex = Dcm_MainConnectionConfig[
      Dcm_RxPduIdConfig[RxPduId].MainConnectionIndex].ProtocolIndex;
  }

  return ProtocolIndex;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_EnqueueResetOfDiagnosticSession(
  void
)
{
  Dcm_ResetDiagnosticSession = TRUE;
}

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ApplicationTransmisionConfirmationEnqueued_SetFlag(
  const boolean ApplicationTransmisionConfirmationEnqueued
)
{
  Dcm_ApplicationTransmisionConfirmationEnqueued = ApplicationTransmisionConfirmationEnqueued;
}
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC (Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_DetermineRequestType(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
)
{
  /* Determines the final RequestType based on the RequestOrigin and RxPermission. */
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_RequestTypeType RequestType = DCM_INVALID_REQUEST_TYPE;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  switch (RequestOrigin)
  {
    case DCM_EXTERNAL:
      /* For external requests, it's important if they're either Physical or Functional. */
      if (DCM_PHYSICAL_ADDRESSING == RxPduIdInfo.AddressingType)
      {
        RequestType = DCM_NORMAL_PHYSICAL_REQUEST;
      }
      else
      {
        RequestType = DCM_NORMAL_FUNCTIONAL_REQUEST;
      }
      break;

    case DCM_INTERNAL_ROE:
      /* For ROE requests, it's important if they're either Physical or Functional AND
         if they're TYPE1 or TYPE2.  */
    {
      boolean RequestIsTYPE2;

#if ((DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
      const Dcm_MainConnectionIndexType MainConnectionIndex =
        Dcm_RxPduIdConfig[RxConnectionId].MainConnectionIndex;
      const Dcm_RoeConnectionIndexType RoeConnectionIndex =
        Dcm_MainConnectionConfig[MainConnectionIndex].RoeConnectionIndex;

      /* Determine if the request is TYPE1 or TYPE2. */
      if ( (DCM_INVALID_ROE_CONNECTION != RoeConnectionIndex) &&
           (FALSE != Dcm_RoeConnectionConfig[RoeConnectionIndex].Type2PduIdsPresent) )
      {
        /* TYPE2 request */
        RequestIsTYPE2 = TRUE;
      }
      else
      {
        /* TYPE1 request */
        RequestIsTYPE2 = FALSE;
      }
#else /* #if ((DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) */
      RequestIsTYPE2 = FALSE;
#endif /* #if ((DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) */

      if (DCM_PHYSICAL_ADDRESSING == RxPduIdInfo.AddressingType)
      {
        if (TRUE == RequestIsTYPE2)
        {
          RequestType = DCM_ROE_TYPE2_PHYSICAL_REQUEST;
        }
        else
        {
          RequestType = DCM_ROE_TYPE1_PHYSICAL_REQUEST;
        }
      }
      else
      {
        if (TRUE == RequestIsTYPE2)
        {
          RequestType = DCM_ROE_TYPE2_FUNCTIONAL_REQUEST;
        }
        else
        {
          RequestType = DCM_ROE_TYPE1_FUNCTIONAL_REQUEST;
        }
      }
      break;
    }

    case DCM_INTERNAL_POST_BOOT_NO_ANSWER:
      RequestType = DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST;
      break;

    case DCM_INTERNAL_POST_BOOT_WITH_ANSWER:
      RequestType = DCM_JUMP_FROM_BL_RESPONSE_REQUEST;
      break;

    case DCM_INTERNAL_PERIODIC:
    {
      /* For Periodic requests, it's important only if they're TYPE1 or TYPE2.  */

#if ( \
     (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
     (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
     (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) \
    )
      Dcm_MainConnectionIndexType MainConnectionIndex;
      Dcm_PeriodicTransmissionIndexType PeriodicTransmissionIndex;

      MainConnectionIndex
        = Dcm_RxPduIdConfig[RxConnectionId].MainConnectionIndex;

      PeriodicTransmissionIndex =
        Dcm_MainConnectionConfig[MainConnectionIndex].PeriodicTransmissionIndex;

      if (DCM_INVALID_PERIODIC_TRANSMISSION != PeriodicTransmissionIndex)
      {
        const Dcm_PeriodicConnectionIndexType NumberOfPeriodicConnections
          = Dcm_PeriodicTransmissionConfig[PeriodicTransmissionIndex].NumberOfPeriodicConnections;

        if (NumberOfPeriodicConnections != 0U)
        {
          /* TYPE2 request */
          RequestType = DCM_PERIODIC_TYPE2_REQUEST;
        }
        else
        {
          /* TYPE1 request */
          RequestType = DCM_PERIODIC_TYPE1_REQUEST;
        }
      }
      else
      {
          RequestType = DCM_PERIODIC_TYPE1_REQUEST;
      }
#else /* #if ( \
               (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
               (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
               (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) \
              ) */
      RequestType = DCM_PERIODIC_TYPE1_REQUEST;
#endif /* #if ( \
               (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
               (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
               (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) \
              ) */
      break;

    }

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  }

  return RequestType;
}

STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationPostBootRequest(
  void
)
{
  const Dcm_ReceptionArbitrationResultType ReceptionArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationPostBootRequest();
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;

  switch (ReceptionArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      /* Go ahead and accept the request. */
      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      /* PostBoot requests should always be accepted. */

      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.PrearbitrationNormalRequest,1 */
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationNormalRequest(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType Length
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;
  const Dcm_ReceptionArbitrationResultType ReceptionArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_EXTERNAL);

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  switch (ReceptionArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
      /* Can't even receive due to no Rx enabled on the channel. */
      RxPermission = DCM_RX_REJECT;
      break;
    case DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION:
    {
      /* The MainConnection is busy. Either reject or accept as a potential
         concurrent TesterPresent request.
       */
      const boolean PotentialTesterPresent = Dcm_Dsl_Supervisor_RequestIsPotentialConcurrentTesterPresent(
                                 RxConnectionId,
                                 Length);

      if (TRUE == PotentialTesterPresent)
      {
        /* !LINKSTO Dcm.EB.RequestHandling.StartOfReception.MainConnectionBusy.NotTYPE1ROE.PotentialConcurrentTesterPresent.AcceptRx,1 */
        RxPermission = DCM_RX_ACCEPT_TESTERPRESENT;
      }
      else
      {
        /* !LINKSTO Dcm.EB.RequestHandling.StartOfReception.MainConnectionBusy.NotTYPE1ROE.NotConcurrentTesterPresentSize.RejectRx,1 */
        /* !LINKSTO Dcm.EB.RequestHandling.StartOfReception.MainConnectionBusy.NotTYPE1ROE.NotConcurrentTesterPresentFunctional.RejectRx,1 */
        RxPermission = DCM_RX_REJECT;
      }
      break;
    }
    case DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
    {
      /* Either a request of the same type (UDS/OBD) is running on another
         stack or priority assessment doesn't let the request go through.
         Reject or accept as a potential concurrent TesterPresent request.
       */
#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyExternal.BRREnabled.AcceptRx, 1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyApplication.BRREnabled.AcceptRx, 1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE1Periodic.BRREnabled.AcceptRx, 1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE2Periodic.BRREnabled.AcceptRx, 1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OBDRequestOBDAlreadyRunningOnDifferentStack.BRREnabled.AcceptRx,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.UDSRequestUDSAlreadyRunningOnDifferentStack.BRREnabled.AcceptRx,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolIdle.NonDefaultSession.BRREnabled.AcceptRx,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningNotTYPE1ROE.BRREnabled.AcceptRx,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.NonDefaultSession.BRREnabled.AcceptRx,1 */

      RxPermission = DCM_RX_ACCEPT_BUSYREPEATREQUEST;
#else /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) */
      /* Since a Concurrent TesterPresent request shall always be accepted, we need to accept into
         the default buffer any potential such request */

      Dcm_ProtocolIndexType  ProtocolIndex =
                                   Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_EXTERNAL);
      Dcm_ProtocolGroupType ProcessingProtocolGroup =
                                   Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(ProtocolIndex);

      if ((DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE == ReceptionArbitrationResult) &&
          (DCM_PROTOCOL_OBD_GROUP == ProcessingProtocolGroup))
      {
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OBDRequestOBDAlreadyRunningOnDifferentStack.BRRDisabled.RejectRx,1 */
        RxPermission = DCM_RX_REJECT;
      }
      else
      {
        /* Since a Concurrent TesterPresent request shall always be accepted, we need to accept into
           the default buffer any potential such request */
        const boolean PotentialTesterPresent = Dcm_Dsl_Supervisor_RequestIsPotentialConcurrentTesterPresent(
                                   RxConnectionId,
                                   Length);
        if (TRUE == PotentialTesterPresent)
        {
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyExternal.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyApplication.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE1Periodic.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE2Periodic.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.UDSRequestUDSAlreadyRunningOnDifferentStack.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolIdle.NonDefaultSession.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningNotTYPE1ROE.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.NonDefaultSession.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */

          RxPermission = DCM_RX_ACCEPT_TESTERPRESENT;
        }
        else
        {
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyExternal.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyExternal.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyApplication.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyApplication.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE1Periodic.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE1Periodic.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE2Periodic.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.RequestedProtocolBusyTYPE2Periodic.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.UDSRequestUDSAlreadyRunningOnDifferentStack.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.UDSRequestUDSAlreadyRunningOnDifferentStack.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolIdle.NonDefaultSession.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolIdle.NonDefaultSession.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningNotTYPE1ROE.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningNotTYPE1ROE.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.NonDefaultSession.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.NonDefaultSession.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */

          RxPermission = DCM_RX_REJECT;
        }
      }
#endif /*#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)*/
      break;
    }
    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      /* Go ahead and accept the request. */

      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.PrearbitrationTYPE1ROE,1 */
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE1ROE(
  Dcm_BufferUserType RxConnectionId
)
{
  const Dcm_ReceptionArbitrationResultType ArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_ROE);
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;

  switch (ArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.NewTYPE1ROEEvent.Discard,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.NewTYPE1ROEEvent.Discard,1 */

      RxPermission = DCM_RX_REJECT;
      break;

    case DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION:
      /* !LINKSTO Dcm.EB.RequestHandling.TYPE1ROEEvent.MainConnectionBusy.Postpone,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.RequestedProtocolBusyExternal.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.RequestedProtocolBusyExternal.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.RequestedProtocolBusyApplication.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.RequestedProtocolBusyApplication.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.RequestedProtocolBusyTYPE1ROE.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.RequestedProtocolBusyTYPE1ROE.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.RequestedProtocolBusyTYPE1Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.RequestedProtocolBusyTYPE1Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.RequestedProtocolBusyTYPE2Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.RequestedProtocolBusyTYPE2Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolBusy.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolBusy.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyExternal.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyExternal.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyApplication.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyApplication.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.OBDAlreadyRunningOnDifferentStack.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.OBDAlreadyRunningOnDifferentStack.Postpone,1 */

    case DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.UDSAlreadyRunningOnDifferentStack.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.UDSAlreadyRunningOnDifferentStack.Postpone,1 */

      RxPermission = DCM_RX_POSTPONE;
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.Core.PrearbitrationTYPE2ROE,1 */
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE2ROE(
  Dcm_BufferUserType RxConnectionId
)
{
  const Dcm_ReceptionArbitrationResultType ArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(RxConnectionId, DCM_INTERNAL_ROE);
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;

  switch (ArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.NewTYPE2ROEEvent.Discard,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.NewTYPE2ROEEvent.Discard,1 */
      RxPermission = DCM_RX_REJECT;
      break;

    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2ROEEvent.RequestedProtocolBusyExternal.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2ROEEvent.RequestedProtocolBusyApplication.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2ROEEvent.RequestedProtocolBusyTYPE1Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2ROEEvent.RequestedProtocolBusyTYPE2Periodic.Postpone,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2ROEEvent.RequestedProtocolBusyTYPE2ROE.Postpone,1 */

      RxPermission = DCM_RX_POSTPONE;
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.NotOnCurrentProtocol.ProcessRequest,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.OnCurrentProtocol.ProcessRequest,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.UDSAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.UDSAlreadyRunningOnSameStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.OBDAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.OBDAlreadyRunningOnSameStack.ProcessInParallel,1 */

      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE1Periodic(
  Dcm_BufferUserType RxConnectionId
)
{
  const Dcm_ReceptionArbitrationResultType ArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_PERIODIC);
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;

  switch (ArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.TYPE1Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.TYPE1Periodic.Discard,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION:
      /* !LINKSTO Dcm.EB.RequestHandling.TYPE1Periodic.MainConnectionBusy.Discard,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolIdle.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE2ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE2ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE2Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE2Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyExternal.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyExternal.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyApplication.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyApplication.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE1ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE1ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE1Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.RequestedProtocolBusyTYPE1Periodic.Discard,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.UDSAlreadyRunningOnDifferentStack.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.UDSAlreadyRunningOnDifferentStack.Discard,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX:
      RxPermission = DCM_RX_REJECT;
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_ReceptionPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_PrearbitrationTYPE2Periodic(
  Dcm_BufferUserType RxConnectionId
)
{
  const Dcm_ReceptionArbitrationResultType ArbitrationResult =
    Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(RxConnectionId, DCM_INTERNAL_PERIODIC);
  Dcm_ReceptionPermissionType RxPermission = DCM_RX_REJECT;

  switch (ArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.TYPE2Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.TYPE2Periodic.Discard,1 */
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyTYPE2ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyTYPE2Periodic.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyExternal.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyApplication.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyTYPE1ROE.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.RequestedProtocolBusyTYPE1Periodic.Discard,1 */

      RxPermission = DCM_RX_REJECT;
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      RxPermission = DCM_RX_ACCEPT;
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxPermission;
}
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
)
{
  Dcm_ReceptionArbitrationResultType ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX;
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ProtocolIndexType ProcessingProtocolIndex;
  Dcm_ComMChannelOperationStateType ComMChannelTxState = DCM_COMM_OPERATION_DISABLED;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  /* The Protocol fetching for TYPE2s is a bit more tricky */

  ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                              RxConnectionId,
                              RequestOrigin);

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                          DCM_INTERNAL_API_ID);

  ComMChannelTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
  /* Always internal of course. These are by all accounts all on UDS Protocols. */

  /* The conditions for a TYPE2 request to be accepted are simple:
     - The necessary Protocol shall be free, i.e. not processing anything
     - The necessary Protocol is not Presumptive with anything
     - No OBD block for UDS Protocols shall be instated.
     - Transmission shall be enabled on the ComMChannel referenced by the RxConnection used to
       configure this behaviour (which is incidentally the one in the name of which the permission
       is requested) */

  if (DCM_COMM_OPERATION_ENABLED != ComMChannelTxState)
  {
    ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL;
  }
  else
  {
    ArbitrationResult = Dcm_Dsl_Supervisor_ProtocolHandling_TYPE2RequestPriorityAssessment(ProcessingProtocolIndex);
  }

  return ArbitrationResult;
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON)|| (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationPostBootRequest(
  void
)
{
  /* Maybe replace with some uninitialized check?*/

  return DCM_SUPERVISOR_ARBITRATION_ACCEPT;
}

STATIC FUNC(Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestOriginType RequestOrigin
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  /* Note: ProcessingProtocol = the Protocol which would be needed to process the new request. */
  Dcm_ProtocolIndexType ProcessingProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
  Dcm_ReceptionArbitrationResultType ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX;
  Dcm_ComMChannelOperationStateType ComMChannelOperationState = DCM_COMM_OPERATION_DISABLED;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);
  ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, RequestOrigin);

  switch (RequestOrigin)
  {
    case DCM_EXTERNAL:
      /* Reception permission is given for external requests if the ComMChannel referenced by the RxConnection is
         allowed to receive data by ComM. */
      ComMChannelOperationState = Dcm_Dsl_ComMChannelStateManager_GetRxState(RxPduIdInfo.ComMChannelId);
      break;

    case DCM_INTERNAL_ROE:
    case DCM_INTERNAL_PERIODIC:
      /* For ROE and Periodic requests you need the ComMChannel to be in FullComMode. */
      ComMChannelOperationState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
      break;

    case DCM_INTERNAL_POST_BOOT_NO_ANSWER:
    case DCM_INTERNAL_POST_BOOT_WITH_ANSWER:
      /* Reception permission doesn't depend on the state of the ComMChannel. */
      ComMChannelOperationState = DCM_COMM_OPERATION_ENABLED;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                          DCM_INTERNAL_API_ID);

  /* Is the ComMChannel able to receive data. */
  if (DCM_COMM_OPERATION_ENABLED != ComMChannelOperationState)
  {
    /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.NewStartOfReceptionCall.RejectRx,1 */

    ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL;
  }
  else
  {
    /* The corresponding MainConnection has to be idle otherwise no response
       can be given. */
    if (TRUE == Dcm_Dsl_Supervisor_MainConnectionHandling_MainConnectionBusy(RxPduIdInfo.MainConnectionIndex))
    {
      /* The receiving MainConnection is busy. No response may be transmitted on it therefore
         only a potential concurrent TesterPresent request may get accepted if this
         arbitration is made for Dcm_StartOfReception. */
      const boolean ProtocolBusyWithType1ROE =
        Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithType1ROE(ProcessingProtocolIndex);
      ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION;
      /* Accept the request if it is not for an ROE or periodic transmission and the protocol is busy with a TYPE1 ROE request */
      if ((DCM_INTERNAL_ROE != RequestOrigin) &&
          (DCM_INTERNAL_PERIODIC != RequestOrigin) &&
          (TRUE == ProtocolBusyWithType1ROE)
         )
      {
        /* !LINKSTO Dcm.EB.RequestHandling.StartOfReception.MainConnectionBusy.TYPE1ROE.OnCurrentProtocol.External.AcceptRx,1 */
        ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
      }
    }
    else
    {
      ArbitrationResult =
        Dcm_Dsl_Supervisor_ProtocolHandling_TYPE1RequestPriorityAssessment(
          ProcessingProtocolIndex,
          RequestOrigin
        );

    }
  }

  return ArbitrationResult;
}

STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetProcessingProtocolIndexForTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType
)
{
  Dcm_ProtocolIndexType ProcessingProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;

  switch (RequestType)
  {
    case DCM_NORMAL_PHYSICAL_REQUEST:
    case DCM_NORMAL_FUNCTIONAL_REQUEST:
      ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_EXTERNAL);
      break;

    case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:
      ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_POST_BOOT_NO_ANSWER);
      break;

    case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
      ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_POST_BOOT_WITH_ANSWER);
      break;

    case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
      ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_ROE);
      break;

    case DCM_PERIODIC_TYPE1_REQUEST:
      ProcessingProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_PERIODIC);
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return ProcessingProtocolIndex;
}

STATIC FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE) Dcm_Dsl_Supervisor_RearbitrationTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType
)
{
  Dcm_ReceptionArbitrationResultType ReceptionArbitrationResult =
    DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX;

  switch (RequestType)
  {
    case DCM_NORMAL_PHYSICAL_REQUEST:
    case DCM_NORMAL_FUNCTIONAL_REQUEST:
      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_EXTERNAL);
      break;

    case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:
      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_POST_BOOT_NO_ANSWER);
      break;

    case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_POST_BOOT_WITH_ANSWER);
      break;

    case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_ROE);
      /* Note: postponing the request doesn't work anymore at this point because the request has already
               been handed over by the Supervisor, so it's no longer in the ROE subsystem's queue. If anything
               happens here, rejection is the only outcome. */
      break;

    case DCM_PERIODIC_TYPE1_REQUEST:
      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request(RxConnectionId, DCM_INTERNAL_PERIODIC);
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return ReceptionArbitrationResult;
}

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_EnqueueTYPE1RequestAsPresumptive(
  Dcm_BufferUserType RxConnectionId,
  Dcm_ProtocolIndexType ProcessingProtocolIndex,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
)
{
  const Dcm_SesCtrlType CurrentSession =
              Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();
  const Dcm_ProtocolGroupType ProcessingProtocolGroup =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(ProcessingProtocolIndex);
  boolean PreemptionNecessary = FALSE;
  Dcm_TimerValueType  NRCTimeout = 0U;
  /*
  Request can get accepted.
  */

  /*
  1. On the stack on which we've received the request, find all of the PresumptiveProtocols
     which are waiting with:
     - External requests - send a BRR for these if possible.
     - TYPE1 ROE requests
     - TYPE1 periodic requests
     If a TYPE1 request got permission to process, it must replace any TYPE1 Presumptive.

     Note: if an ROE request gets this far, the only presumptive it would cancel would
           be a possible Periodic.
  */
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelTYPE1PresumptiveProtocolsOnSameStack(ProcessingProtocolIndex);

  /*
  2. Handle any CurrentProtocols which might be present on the same stack as the one
     on which we've received the request. Also determine if a protocol preemption would
     be necessary, i.e. if the current Protocol would be replaced by another.
  */
  PreemptionNecessary =
    Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionNecessaryOnStack(ProcessingProtocolIndex);

  if (TRUE == PreemptionNecessary)
  {
    /* Only in the case in which preemption is necessary is the CurrentProtocol
       terminated.
     */
    Dcm_Dsl_Supervisor_ProtocolHandling_TerminateCurrentProtocolOnSameStack(
      ProcessingProtocolIndex);
  }

  /*
  3. Cancel all receptions pending on Protocols which
     have lower priorities than the RxProtocol on the RxStack.

     The reception causing the cancellation isn't itself cancelled since
     the cancelling conenction's ID is given.

     NOTE: don't do this if this is an ROE request, as ROE requests shall always be cancelled
           by incoming requests of any kind. Therefore, leave all receptions to proceed if the
           request-to-process is an ROE request.
  */
  if ((DCM_ROE_TYPE1_PHYSICAL_REQUEST != RequestType) &&
      (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST != RequestType)
     )
  {
    Dcm_Dsl_Supervisor_ProtocolHandling_CancelOngoingReceptionsOnLowerPriorityProtocolsOfSameStack(
      RxConnectionId);
  }

  /*
  4. After this step, on all stacks, any remaining Protocols processing ROE requests are
     cancelled. The cancellation
     is done such that:
     -- Active Protocols running TYPE1 ROE requests cancel and stay Started
     -- Started Protocols running TYPE2 ROE requests cancel and stay Started
     -- Stopped Protocols running TYPE2 ROE requests cancel and stay Stopped
     Cancel both running and presumptive Protocols. Do this only if the request
     is not ROE or Periodic.
  */
  if (
      /* ROE may only work on one Protocol always. Therefore, if an ROE request
         comes in, either it's rejected because of a running higher prio
         or external request, an already-running ROE request, or gets accepted
         but at that point surely no ROE request is running. Therefore no need
         to cancel ROE processing when the request is ROE.
       */
      (DCM_ROE_TYPE1_PHYSICAL_REQUEST != RequestType) &&
      (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST != RequestType) &&
      /* Periodic requests are not external requests. They don't cancel running
         ROE requests.
       */
      (DCM_PERIODIC_TYPE1_REQUEST != RequestType)
     )
  {
    /* Note that the Protocol Preemption function already took care of
       preparing the next Protocol if preemption is needed. Here we also
       stop an eventual ROE processing if it were taking place by any chance.
     */
    Dcm_Dsl_Supervisor_ProtocolHandling_CancelROEProcessingOnAllProtocols();
  }

  /*
  5. If the request is for an OBD Protocol...
     After all this is done, if session is non-default AND the Rx request is for an OBD
     Protocol, seek out all UDS Protocols and cancel them. Do this without changing the
     CurrentProtocol status of the cancelled Protocols. Cancel both Running and
     Presumptive UDS Protocols.
  */

  if (DCM_PROTOCOL_OBD_GROUP == ProcessingProtocolGroup)
  {
  /*
    5.1 If session is non-default, seek out all UDS Protocols and cancel them.
        Do this without changing the CurrentProtocol status of the cancelled Protocols.
        Cancel both Running and Presumptive UDS Protocols.
    */
    if (DCM_DEFAULT_SESSION != CurrentSession)
    {
      Dcm_Dsl_Supervisor_ProtocolHandling_CancelUDSProtocolsInNonDefaultSession();

      /* Enqueue a reset of the diag session only if there will be no protocol preemption.
         Otherwise preemption handles this anyway. */
      if (FALSE == PreemptionNecessary)
      {
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ResetToDefaultSession,1 */
        Dcm_Dsl_Supervisor_EnqueueResetOfDiagnosticSession();
      }
    }
  }

  /*
  6. Remember to also, synchronously with the MainFunction, request FullCommunication for
     the ComMChannel in question if necessary.
  */
  if (
      (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
      (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType) ||
      (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType) ||
      (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType)
     )
  {
    Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingBegin(RxConnectionId,
                                                     FALSE);
  }
  else
  {
    /* Just proceed. Other request types won't ask for FullComMode. Since there is no
       request from a Tester involved in ROE or Periodic requests, no CommunicationSession
       is actually taking place. */
  }

  /*
  7. The RxProtocol now becomes the PresumptiveProtocol and the request is enqueued.
     Anything else which was present in the queue is replaced
     as if the new request is accepted, the request in the queue could have only been an Roe request.
  */
  Dcm_Dsl_Supervisor_ProtocolHandling_EnqueueAsPresumptive(ProcessingProtocolIndex,
                                             RxConnectionId,
                                             RequestType,
                                             RequestBuffer,
                                             PreemptionNecessary);

  /*
  8. And then because the Protocol became Presumptive, start the P2Timer.
  */
  if((DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
     (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType)
    )
  {
    Dcm_TimerValueType P2MaxTime = Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime(
                                   ProcessingProtocolIndex);
    Dcm_TimerValueType ServerAdjust = (Dcm_TimerValueType)
                                  (Dcm_ProtocolConfig[ProcessingProtocolIndex].P2ServerAdjustTime);

    /* In case that P2MaxTime is smaller then P2ServerAdjustTime,
       P2Timer will be started with value 0. */
    if (P2MaxTime > ServerAdjust)
    {
      NRCTimeout = P2MaxTime - ServerAdjust;
    }

    (void)Dcm_Dsl_DiagnosticSessionHandler_AllocateP2Timer(DCM_SUPERVISOR_ID,
                                                           ProcessingProtocolIndex);

    /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.StartP2Server,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.StartP2Server,1 */
    (void)Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(DCM_SUPERVISOR_ID,
                                                        ProcessingProtocolIndex,
                                                        NRCTimeout);
  }
}

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessTYPE1Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ProtocolIndexType ProcessingProtocolIndex =
      Dcm_Dsl_Supervisor_GetProcessingProtocolIndexForTYPE1Request(
           RxConnectionId,
           RequestType
         );
  Dcm_ReceptionArbitrationResultType ReceptionArbitrationResult =
      Dcm_Dsl_Supervisor_RearbitrationTYPE1Request(
           RxConnectionId,
           RequestType
         );

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  /* Note: since RxPermission and ProcessRequest are in the same critical section for
           ROE and Periodic requests, another call to Dcm_Dsl_Supervisor_RxArbitrationTYPE1Request
           is not necessary. */

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                        DCM_INTERNAL_API_ID);

  switch (ReceptionArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION:
      /* Can't even receive due to no Rx enabled on the channel. */
      /* Discard */
      /* Notify the DiagnosticSessionHandler
         that the communication session on the RxConnection has ended.
       */
      if (
          (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
          (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType) ||
          (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType) ||
          (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType)
         )
      {
        /* Note: might always be true since for other requests, the first arbitration
           will always dictate the permission and there is no time (i.e. time outside of
           an exclusive area) where an interrupt might change anything about what the first
           arbitration decided) */

        Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
          RxConnectionId);
      }

      /* And discard the request buffer. */
      (void)Dcm_Dsl_BufferFree(DCM_SUPERVISOR_ID, RequestBuffer.BufferId);
      break;

    case DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:

      if (
          (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) &&
          /* Never generate NRCs for Periodic requests. */
          (DCM_PERIODIC_TYPE1_REQUEST != RequestType) &&
          /* Don't generate the BRR request for ROE requests as
             there's no tester here trying a request to speak of. */
          (DCM_ROE_TYPE1_PHYSICAL_REQUEST != RequestType) &&
          (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST != RequestType)
         )
      {
        /* Send BRR */
        Dcm_TimerValueType  NRCTimeout = 0U;
        Dcm_TimerValueType  P2MaxTime = Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime(ProcessingProtocolIndex);
        Dcm_TimerValueType  ServerAdjust = (Dcm_TimerValueType)(Dcm_ProtocolConfig[ProcessingProtocolIndex].P2ServerAdjustTime);

        /* If P2MaxTime is smaller then P2ServerAdjustTime, P2Timer will be started with 0.*/
        if (P2MaxTime > ServerAdjust)
        {
          NRCTimeout = P2MaxTime - ServerAdjust;
        }

        /* Communication session begins, we DO NOT request FullCommunication for this type
           of response. */
        Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingBegin(RxConnectionId,
                                                         TRUE);

        /* Since this is done in the context of an ISR, the NRC transmission
           must be delayed until the next Dcm_MainFunction() cycle.
         */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.RequestedProtocolBusyTYPE2Periodic.BRREnabled.TransmitBRR,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequestOBDAlreadyRunningOnDifferentStack.BRREnabled.TransmitBRR,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.UDSRequestUDSAlreadyRunningOnDifferentStack.BRREnabled.TransmitBRR,1 */

        Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
            RxConnectionId,
            RequestBuffer,
            DCM_E_BUSYREPEATREQUEST,
            NRCTimeout
          );
      }
      else
      {
        if (
            (DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
            (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType) ||
            (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType) ||
            (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType)
           )
        {
          /* Notify the DiagnosticSessionHandler
             that the communication session on the RxConnection has ended.
           */
          /* Note: might always be true since for other requests, the first arbitration
             will always dictate the permission and there is no time (i.e. time outside of
             an exclusive area) where an interrupt might change anything about what the first
             arbitration decided) */

          Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
            RxConnectionId);

        }
      }

      /* And the request is discarded. */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.RequestedProtocolBusyTYPE2Periodic.BRRDisabled.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequestOBDAlreadyRunningOnDifferentStack.BRRDisabled.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.UDSRequestUDSAlreadyRunningOnDifferentStack.BRRDisabled.Discard,1 */

      /* And discard the request buffer. */
      (void)Dcm_Dsl_BufferFree(DCM_SUPERVISOR_ID, RequestBuffer.BufferId);
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      /* Go ahead and process the request. */
      Dcm_Dsl_Supervisor_EnqueueTYPE1RequestAsPresumptive(
          RxConnectionId,
          ProcessingProtocolIndex,
          RequestType,
          RequestBuffer
        );

      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON)|| (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProcessTYPE2Request(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
)
{
  Dcm_ProtocolIndexType ProcessingProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
  Dcm_ReceptionArbitrationResultType ReceptionArbitrationResult =
    DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX;

  switch (RequestType)
  {
    case DCM_ROE_TYPE2_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE2_FUNCTIONAL_REQUEST:
      ProcessingProtocolIndex =
        Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_ROE);

      DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                              DCM_INTERNAL_API_ID);

      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(RxConnectionId, DCM_INTERNAL_ROE);
      break;

    case DCM_PERIODIC_TYPE2_REQUEST:
      ProcessingProtocolIndex =
        Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(RxConnectionId, DCM_INTERNAL_PERIODIC);

      DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                              DCM_INTERNAL_API_ID);

      ReceptionArbitrationResult =
        Dcm_Dsl_Supervisor_RxArbitrationTYPE2Request(RxConnectionId, DCM_INTERNAL_PERIODIC);
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                          DCM_INTERNAL_API_ID);

  switch (ReceptionArbitrationResult)
  {
    case DCM_SUPERVISOR_ARBITRATION_REJECT_NO_RX:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_COMMCHANNEL:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_MAIN_CONNECTION:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE:
    case DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:
      /* Regardless of error, discard the request buffer. */

      (void)Dcm_Dsl_BufferFree(DCM_SUPERVISOR_ID, RequestBuffer.BufferId);
      break;

    case DCM_SUPERVISOR_ARBITRATION_ACCEPT:
      /* Go ahead and process the request. */
      Dcm_Dsl_Supervisor_ProtocolHandling_EnqueueAsPresumptive(ProcessingProtocolIndex,
                                                 RxConnectionId,
                                                 RequestType,
                                                 RequestBuffer,
                                                 FALSE);

      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON)|| (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ResetDiagnosticSession(
  void
)
{
  /* Resets the session at startup or on protocol cancellation because of OBD request

     Should cancellation have finished, we need to see whether the DiagnosticSession may be changed.
     This is only done once per MainFunction and if the condition for this is present.

     This takes care only of the case in which we reset the session to default because of an OBD
     request or at startup.
   */

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (TRUE == Dcm_ResetDiagnosticSession)
  {
    Dcm_ResetDiagnosticSession = FALSE;

    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    Dcm_Dsl_DiagnosticSessionHandler_ResetToDefaultSession(
                        DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR);

  }
  else
  {
    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
}

STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_Supervisor_RequestIsPotentialConcurrentTesterPresent(
  PduIdType RxPduId,
  PduLengthType Length
)
{
  boolean IsPotentialTesterPresent = FALSE;

  /* An incoming request is a potential TesterPresent request
     if it the incoming RxPduId is a functional RxPduId and
     the length of the incoming request is precisely 2 bytes.*/

  if (2U == Length)
  {
    if (DCM_FUNCTIONAL_ADDRESSING == Dcm_RxPduIdConfig[RxPduId].AddressingType)
    {
      IsPotentialTesterPresent = TRUE;
    }
  }

  return IsPotentialTesterPresent;
}

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionNormalRequest(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType Length,
  Dcm_RequestTypeType InitialRequestType
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationNormalRequest(RxConnectionId, Length);

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  InitialRequestType;
      break;

    case DCM_RX_ACCEPT_TESTERPRESENT:
      /* Replace previously determined request type as the request cannot be processed
         as a normal request but might be a concurrent TesterPresent request. */
      AcceptAsRequestType = DCM_CONCURRENT_TESTERPRESENT_REQUEST;
      break;

    case DCM_RX_ACCEPT_BUSYREPEATREQUEST:
      /* Replace previously determined request type as the request cannot be processed
         as a normal request but might be a concurrent TesterPresent request. */
      AcceptAsRequestType = DCM_BUSYREPEATREQUEST_REQUEST;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */
      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE1ROERequest(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType InitialRequestType
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationTYPE1ROE(RxConnectionId);

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  InitialRequestType;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */
      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    case DCM_RX_POSTPONE:
      /* Postpone the ROE request. */
      AcceptAsRequestType = DCM_POSTPONED_ROE_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE2ROERequest(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType InitialRequestType
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationTYPE2ROE(RxConnectionId);

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  InitialRequestType;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */

      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    case DCM_RX_POSTPONE:
      /* Postpone the ROE request. */
      AcceptAsRequestType = DCM_POSTPONED_ROE_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionPostBootRequest(
  Dcm_RequestTypeType InitialRequestType
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationPostBootRequest();

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  InitialRequestType;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */
      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}

STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE1PeriodicRequest(
  Dcm_BufferUserType RxConnectionId
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationTYPE1Periodic(RxConnectionId);

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  DCM_PERIODIC_TYPE1_REQUEST;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */
      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
STATIC FUNC(Dcm_RequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_RxPermissionTYPE2PeriodicRequest(
  Dcm_BufferUserType RxConnectionId
)
{
  Dcm_RequestTypeType AcceptAsRequestType = DCM_REJECTED_REQUEST;

  Dcm_ReceptionPermissionType ReceptionPermission = Dcm_Dsl_Supervisor_PrearbitrationTYPE2Periodic(RxConnectionId);

  switch (ReceptionPermission)
  {
    case DCM_RX_ACCEPT:
      /* Just accept as the previously determined request type. */
      AcceptAsRequestType =  DCM_PERIODIC_TYPE2_REQUEST;
      break;

    case DCM_RX_REJECT:
      /* Reject this. */
      AcceptAsRequestType = DCM_REJECTED_REQUEST;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return AcceptAsRequestType;
}
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]================================================================*/
