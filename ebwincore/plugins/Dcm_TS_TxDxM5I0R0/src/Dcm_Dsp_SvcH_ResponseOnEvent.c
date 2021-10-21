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

/* !LINKSTO Dcm.Dsn.File.ROE.Impl,1 */
/* This file contains the implementation of the Diagnostic Service ResponseOnEvent */


/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_ResponseOnEvent.h>
#include <TSAutosar.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x86 handling. */
#if (DCM_DSP_USE_SERVICE_0X86 == STD_ON)

#include <Dcm_Trace.h>
#include <TSMem.h>                                               /* EB specific memory functions */
#include <Dcm_Dsp_DidServices.h>
#include <Dcm_Dsl_RoeServices.h>
#include <Dcm_HsmDcm.h>
/*===========================[macro definitions]=================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON))
/** \brief Contains the ROE start event logic
 **
 ** \param[in]    OpStatus    Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in]    Persistence ROE persistence flag.
 **
 ** \return Std_ReturnType
 ** \retval DCM_E_OK          This should be returned in all cases.
 **/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence);
#endif /* #if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
               (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence);

#endif /*#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))*/

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON))
/** \brief Contains the ROE stop event logic
 **
 ** \param[in]    OpStatus    Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in]    Persistence ROE persistence flag.
 **
 ** \return Std_ReturnType
 ** \retval DCM_E_OK          This should be returned in all cases.
 **/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence);
#endif /* #if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
               (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=======================[external data declarations]============================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.ROE.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* pointer to the subservice handler */
  Dcm_GenericServiceHandlerType subServiceHandler;
  uint8 SubServiceID;
  boolean ProtocolCheckSucessful = FALSE;
  boolean RoeInitialized = Dcm_Dsl_RoeServices_IsInitialized();

  DBG_DCM_DSP_RESPONSEONEVENT_SVCH_ENTRY(OpStatus, pMsgContext);

  if (FALSE == RoeInitialized)
  {
    /* ROE is not initialized - send negative response! */
    /* !LINKSTO Dcm.EB.ROE.NotInitialized,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    /* Obtain SubService ID from request */
    SubServiceID = pMsgContext->reqData[0];
    SubServiceID &= (uint8)~DCM_ROE_PERSISTENCE_BIT;

    if(((SubServiceID == DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE)||
        (SubServiceID == DCM_ROE_SUBSERVICE_STARTRESPONSEONEVENT)||
        (SubServiceID == DCM_ROE_SUBSERVICE_REPORTACTIVATEDEVENTS))&&
        (OpStatus == DCM_INITIAL))
    {
      Dcm_RxPduIdInfoType RxPduIdInfo;
      PduIdType RxPduId = 0U;
      Std_ReturnType GetRxPduIdResult;

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
      GetRxPduIdResult = Dcm_Dsl_RoeServices_GetRxPduId(&RxPduId);

      /* GetRxPduIdResult == DCM_E_OK     -> at least one RoeEvent is in 'ROE started' state
         GetRxPduIdResult == DCM_E_NOT_OK -> there is no RoeEvent in 'ROE started' state */

      if (GetRxPduIdResult == DCM_E_OK)
      {
        Dcm_RxPduIdInfoType StoredRxPduIdInfo;

        Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduId, &StoredRxPduIdInfo);

        /* !LINKSTO SWS_Dcm_00940,1 */
        if (StoredRxPduIdInfo.ProtocolIndex == RxPduIdInfo.ProtocolIndex)
        {
          ProtocolCheckSucessful = TRUE;
        }
        else
        {
          ProtocolCheckSucessful = FALSE;
        }
      }
      else
      {
        /* No events are in the started state, further check for the protocol ID not needed */
        ProtocolCheckSucessful = TRUE;
      }
    }
    else
    {
      /* The requested sub-service does not require a protocol ID check. */
      /* !LINKSTO SWS_Dcm_00929,1, SWS_Dcm_00930,1 */
      ProtocolCheckSucessful = TRUE;
    }
    if(ProtocolCheckSucessful == TRUE)
    {
#if (DCM_NUM_PROTOCOL > 1U)
      const uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                         DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */
      /* Get the context information current instance */
      CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);
      /* Get configuration for current service table */
      CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
                               = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(instIdx).ServiceTableIndex];
      /* Get corresponding sub-service array configuration */
      CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
                               = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

      /* SWS_Dcm_00764: if a valid pointer to subServiceHandler is returned, pMsgContext will point
       * behind the subFunction identifier */
      subServiceHandler = Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

      DCM_PRECONDITION_ASSERT(subServiceHandler != NULL_PTR, DCM_SVCID_RESPONSEONEVENT);
      /* Call sub-service handler */
      serviceProcessStatus = subServiceHandler(OpStatus, pMsgContext);

      /* Sub-service handlers return E_OK or external configured sub-service handlers return
         E_NOT_OK so we must convert to DCM_E_DONE.*/
      if ( (E_OK == serviceProcessStatus) ||
           ( (E_NOT_OK == serviceProcessStatus) &&
             (NULL_PTR == (curSsConfig[curHsmInfo->subServiceIdIdx].IntSrvHandler)) )
         )
      {
        serviceProcessStatus = DCM_E_DONE;
      }
    }
    else
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }
  DBG_DCM_DSP_RESPONSEONEVENT_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON)
/* !LINKSTO SWS_Dcm_00892.onDTCStatusChangeNonPersistent,1 */
/* !LINKSTO Dcm.Dsn.IB.ROE.OnChangeOfDTCNonPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_ENTRY(OpStatus, pMsgContext);

  Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeCommon(OpStatus, pMsgContext, DCM_ROE_DO_NOT_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_EXIT(
                                                             SetupROERetVal, OpStatus, pMsgContext);

  /* this function NEVER fails and it has a return value only as a result of standardized function signatures */
  return DCM_E_OK;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON)
/* !LINKSTO SWS_Dcm_00892.onDTCStatusChangePersistent,1 */
/* !LINKSTO Dcm.Dsn.IB.ROE.OnChangeOfDTCPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangePersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_ENTRY(OpStatus, pMsgContext);

  Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeCommon(OpStatus, pMsgContext, DCM_ROE_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_EXIT(
                                                          SetupROERetVal, OpStatus, pMsgContext);

  /* this function NEVER fails and it has a return value only as a result of standardized function signatures */
  return DCM_E_OK;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_ENTRY(
    OpStatus, pMsgContext);
  TS_PARAM_UNUSED(OpStatus);

  /* NOT IMPLEMENTED - sending sub-functionNotSupported */
  Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);

  DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_EXIT(
    DCM_E_OK, OpStatus, pMsgContext);
  return DCM_E_OK;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_ENTRY(OpStatus, pMsgContext);
  TS_PARAM_UNUSED(OpStatus);

  /* NOT IMPLEMENTED - sending sub-functionNotSupported */
  Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);

  DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_EXIT(
    DCM_E_OK, OpStatus, pMsgContext);
  return DCM_E_OK;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON))
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Std_ReturnType StartRetVal;

  if (OpStatus == DCM_INITIAL)
  {
    if (pMsgContext->reqDataLen == 1U)
    {
      StartRetVal = Dcm_Dsl_RoeServices_StartResponseOnEvent(
        Persistence, pMsgContext->dcmRxPduId);

      if (StartRetVal != DCM_E_OK)
      {
        Dcm_ExternalSetNegResponse(pMsgContext, StartRetVal);
      }
      else
      {
        /* eventWindowTime is at position 2 in the request, but service and subservice ID have been
           already stripped from the request data, so now it is at the position 0 */
        uint8 reqEventWindowTime = pMsgContext->reqData[0U];

        /* Set positive response. */
        pMsgContext->resData[0U] = DCM_ROE_SUBSERVICE_STARTRESPONSEONEVENT; /* Subservice ID */
        if (Persistence == DCM_ROE_STORE_EVENT)
        {
          pMsgContext->resData[0U] |= DCM_ROE_PERSISTENCE_BIT;
        }
        pMsgContext->resData[1U] = 0U;    /* numberOfIdentifiedEvents */
        pMsgContext->resData[2U] = reqEventWindowTime; /* eventWindowTime - echo from req. */
        pMsgContext->resDataLen = 3U;
      }
    }
    else
    {
      /* Request length invalid - send negative response! */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    serviceProcessStatus = DCM_E_OK;
  }
  else
  {
    /* do nothing, if the function was called with other OpStatus */
    serviceProcessStatus = DCM_E_OK;
  }

  return serviceProcessStatus;
}
#endif /* #if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
               (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.RoeServices.OnDTCStatusChange,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence)
{
  Std_ReturnType SetupROERetVal;

  /* Extract relevant data from request */
  /* eventWindowTime is at position 2 in the request, but service and sub-service ID have been
     already stripped from the request data, so now it is at the position 0 */
  uint8 reqEventWindowTime = pMsgContext->reqData[0U];
  /* New DTCMask to be setup */
  uint8 RequestedDTCMask = pMsgContext->reqData[1U];
  /* Service to respond to ID */
  uint8 SvcToRespondToSvcID = pMsgContext->reqData[2U];
  /* Service to respond to Sub-service ID */
  uint8 SvcToRespondToSubSvcID = pMsgContext->reqData[3U];

  if (OpStatus == DCM_INITIAL)
  {
    /* Request Length Verification */
    if (pMsgContext->reqDataLen == 4U)
    {
      /* !LINKSTO SWS_Dcm_00893.OnDTCStatusChange,1 */
      if((SvcToRespondToSvcID != DCM_RDTCI_REQUEST_SID)||
          (SvcToRespondToSubSvcID != DCM_RDTCI_REQUEST_SSID))
      {
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      }
      else
      {
        SetupROERetVal = Dcm_Dsl_RoeServices_SetupEvent_DTCStatusChange(
            Persistence,
            RequestedDTCMask,
            reqEventWindowTime);

        if(SetupROERetVal != DCM_E_OK)
        {
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        }
        else
        {
          /* Set positive response. */
          pMsgContext->resData[0U] = DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE; /* Subservice ID */
          if(Persistence == TRUE)
          {
            pMsgContext->resData[0U] |= DCM_ROE_PERSISTENCE_BIT;
          }
          pMsgContext->resData[1U] = 0U;    /* numberOfIdentifiedEvents */
          pMsgContext->resData[2U] = reqEventWindowTime; /* eventWindowTime - echo from req. */

#if (DCM_ROE_ONDTCSTATUSCHANGE_SHORT_RESPONSE == 0U)
          /* !LINKSTO Dcm.EB.DcmROEonDtcStatusChangeShortResponse_Disabled,1 */
          pMsgContext->resData[3U] = RequestedDTCMask; /* Event Type Record */
          pMsgContext->resData[4U] = SvcToRespondToSvcID;
          pMsgContext->resData[5U] = SvcToRespondToSubSvcID;
          pMsgContext->resDataLen = 6U;
#else
          /* !LINKSTO Dcm.EB.DcmROEonDtcStatusChangeShortResponse_Enabled,1 */
          pMsgContext->resDataLen = 3U;
#endif
        }
      }
    }
    else
    {
      /* Request length invalid - send negative response! */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
  }

  else
  {
    /* do nothing, if the function was called with other OpStatus */
  }
}
#endif /* #if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
               (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))*/

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.StartNonPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY(
    OpStatus, pMsgContext);

  serviceProcessStatus = Dcm_Dsp_ResponseOnEvent_StartResponseOnEventCommon(
    OpStatus, pMsgContext, DCM_ROE_DO_NOT_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.StartPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY(
    OpStatus, pMsgContext);

  serviceProcessStatus = Dcm_Dsp_ResponseOnEvent_StartResponseOnEventCommon(
    OpStatus, pMsgContext, DCM_ROE_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON))
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventCommon(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  boolean Persistence)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Std_ReturnType StopRetVal;

  if (OpStatus == DCM_INITIAL)
  {
    /* Request Length Verification */
    if (pMsgContext->reqDataLen == 1U)
    {
      StopRetVal = Dcm_Dsl_RoeServices_StopResponseOnEvent(Persistence);

      if (StopRetVal != DCM_E_OK)
      {
        Dcm_ExternalSetNegResponse(pMsgContext, StopRetVal);
      }
      else
      {
        /* eventWindowTime is at position 2 in the request, but service and subservice ID have been
           already stripped from the request data, so now it is at the position 0 */
        uint8 reqEventWindowTime = pMsgContext->reqData[0U];

        /* Set positive response */
        pMsgContext->resData[0U] = DCM_ROE_SUBSERVICE_STOPRESPONSEONEVENT;  /* Subservice ID */
        if (Persistence == DCM_ROE_STORE_EVENT)
        {
          pMsgContext->resData[0U] |= DCM_ROE_PERSISTENCE_BIT;
        }
        pMsgContext->resData[1U] = 0U;    /* numberOfIdentifiedEvents */
        pMsgContext->resData[2U] = reqEventWindowTime; /* eventWindowTime - echo from req. */
        pMsgContext->resDataLen = 3U;
      }
    }
    else
    {
      /* Request length invalid - send negative response! */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
  }
  else
  {
    /* do nothing, if the function was called with other OpStatus */
  }

  /* With the current behavior the return value is DCM_E_OK for all branches */
  serviceProcessStatus = DCM_E_OK;

  return serviceProcessStatus;
}
#endif /* #if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
               (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.StopNonPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY(OpStatus,pMsgContext);

  serviceProcessStatus = Dcm_Dsp_ResponseOnEvent_StopResponseOnEventCommon(
    OpStatus, pMsgContext, DCM_ROE_DO_NOT_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.StopPersistent.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY(OpStatus,pMsgContext);

  serviceProcessStatus = Dcm_Dsp_ResponseOnEvent_StopResponseOnEventCommon(
    OpStatus, pMsgContext, DCM_ROE_STORE_EVENT);

  DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON) */


#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.Clear.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_ClearResponseOnEvent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_ENTRY(OpStatus, pMsgContext);
  TS_PARAM_UNUSED(OpStatus);

  if (OpStatus == DCM_INITIAL)
  {
    /* Request Length Verification */
    if (pMsgContext->reqDataLen == 1U)
    {
      /* eventWindowTime is at position 2 in the request, but service and subservice ID have been
         already stripped from the request data, so now it is at the position 0 */
      uint8 reqEventWindowTime = pMsgContext->reqData[0U];

      /* !LINKSTO SWS_Dcm_00887,1 */
      Dcm_Dsl_RoeServices_ClearEvents(TRUE);

      /* Set positive response */
      pMsgContext->resData[0U] = DCM_ROE_SUBSERVICE_CLEARRESPONSEONEVENT;  /* Subservice ID */
      pMsgContext->resData[1U] = 0U;    /* numberOfIdentifiedEvents */
      pMsgContext->resData[2U] = reqEventWindowTime; /* eventWindowTime - echo from req. */
      pMsgContext->resDataLen = 3U;
    }
    else
    {
      /* Request length invalid - send negative response! */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
  }
  else
  {
    /* do nothing, if the function was called with other OpStatus */
  }

  /* With the current behavior the return value is DCM_E_OK for all branches */
  serviceProcessStatus = DCM_E_OK;

  DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_EXIT(
    DCM_E_OK, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 == STD_ON) */


#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.ROE.Report.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_ReportActivatedEvents_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_ENTRY(OpStatus, pMsgContext);
  TS_PARAM_UNUSED(OpStatus);

  if (pMsgContext->reqDataLen == 1U)
  {
    /* Request Length Ok, fill the buffer with contents */
    Dcm_MsgLenType resLength;

    Dcm_Dsl_RoeServices_ReportActivatedEvents(&(pMsgContext->resData[1U]), &resLength);

    pMsgContext->resData[0U] = DCM_ROE_SUBSERVICE_REPORTACTIVATEDEVENTS;  /* Subservice ID */
        pMsgContext->resDataLen = resLength + 1U;
        serviceProcessStatus = DCM_E_OK;
      }
      else
      {
        /* Request length invalid - send negative response! */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        serviceProcessStatus = DCM_E_OK;
      }

  DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);
  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON) */

/*==================[internal function definitions]==============================================*/
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */

/*==================[end of file]================================================================*/
