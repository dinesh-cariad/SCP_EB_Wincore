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

/* !LINKSTO Dcm.Dsn.File.RSDBI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service ReadScalingDataByIdentifier. */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

#include <Dcm_Dsp_SvcH_ReadScalingDataByIdentifier.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x24 handling. */
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#include <Dcm_Dsp_DidServices.h>
#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

/*===========================[type definitions]=================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Internal service handler for UDS service 0x24
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcStart(
  Dcm_MsgContextPtrType pMsgContext);

/** \brief Internal service handler for UDS service 0x24
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcContinue(
  Dcm_OpStatusType OpStatus,
  Dcm_MsgContextPtrType pMsgContext);

/** \brief Cancels the read scaling DID operation
 *
 * \param ReadDidListContext [in] Pointer to the context whose execution shall
 *                                be cancelled.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_Cancel(
  P2VAR(Dcm_ReadDidContextType, AUTOMATIC, DCM_VAR) ReadScalingDidContext);

/** \brief This function is used to execute the ReadScalingDataByIdentifier service functionality
 **        after the DidService unit was locked by ReadScalingDataByIdentifier.
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Buffer configuration for ReadScalingDid to write the data to */
STATIC VAR(Dcm_DspMsgBufferCfgType, DCM_VAR) ReadScalingDid_BufferCfg;

/** \brief Structure holding the context of reading one scaling DID */
STATIC VAR(Dcm_ReadDidContextType, DCM_VAR) Dcm_ReadScalingDidContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* This is the generic service handler for UDS service 0x24 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* !LINKSTO Dcm.Dsn.IB.RSDBI.SvcH,1 */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_ENTRY(OpStatus,pMsgContext);

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  if((OpStatus != DCM_CANCEL) &&
     (OpStatus != DCM_CONFIRMED_OK) &&
     (OpStatus != DCM_CONFIRMED_NOK))
  {
    Std_ReturnType LockStatus = Dcm_Dsp_DidServices_LockService();

    if (LockStatus == DCM_E_RESOURCE_LOCKED)
    {
      /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.TYPE2RoeRunning.DIDServicesLocked.Postpone,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.ProtocolPreemption.ReturnControlToECUPending.Postpone,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.S3Timeout.ReturnControlToECUPending.Postpone,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.SessionTransition.ReturnControlToECUPending.Postpone,1 */
      ServiceProcessStatus = DCM_E_PENDING;
    }
    else
    {
      DCM_POSTCONDITION_ASSERT(LockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);

      ServiceProcessStatus = Dcm_Dsp_ReadScalingDataByIdentifier_ServiceExecution(DCM_INITIAL,
                                                                                   pMsgContext);
    }
  }
  else
  {
    ServiceProcessStatus = DCM_E_DONE;
  }
#else
  ServiceProcessStatus = Dcm_Dsp_ReadScalingDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

/* This is the generic service handler for UDS service 0x24 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* !LINKSTO Dcm.Dsn.IB.RSDBI.ServiceExecution,1 */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      ServiceProcessStatus = Dcm_Dsp_ReadScalingDataByIdentifier_SvcStart(pMsgContext);
    }
    break;

    /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
    {
      ServiceProcessStatus = Dcm_Dsp_ReadScalingDataByIdentifier_SvcContinue(OpStatus, pMsgContext);
    }
    break;

    /* cancel any ongoing service processing */
    case DCM_CANCEL:
    {
      /* cancel the service */
      ServiceProcessStatus =
                             Dcm_Dsp_ReadScalingDataByIdentifier_Cancel(&Dcm_ReadScalingDidContext);
    }
    break;

    /* actions performed on sucessful sending of the response */
    case DCM_CONFIRMED_OK:
    /* actions performed on failure in sending of the response */
    case DCM_CONFIRMED_NOK:
    {
      ServiceProcessStatus = DCM_E_DONE;
      /* do nothing */
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READSCALINGDATABYIDENTIFIER);
      ServiceProcessStatus = DCM_E_DONE;
    }
    break;
  }

  DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

/*==================[internal function definitions]==============================================*/

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcStart(
  Dcm_MsgContextPtrType pMsgContext)
{
  /* !LINKSTO Dcm.Dsn.IB.RSDBI.SvcStart,1 */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* Check if the request length is different than the configured minimum */
  if ((pMsgContext->reqDataLen != DCM_DID_ID_SIZE))
  {
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.IncorrectMessageLength.TransmitNRC0x13,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
#endif
  }
  else
  {
    /* calculate the requested DID */
    uint8 HighByte = (uint8)pMsgContext->reqData[0U];
    uint8 LowByte  = (uint8)pMsgContext->reqData[1U];
    uint16 Did = (uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

    /* create buffer context */
    ReadScalingDid_BufferCfg.Size = pMsgContext->resMaxDataLen;
    ReadScalingDid_BufferCfg.Occupied = pMsgContext->resDataLen;
    ReadScalingDid_BufferCfg.BufferPtr = pMsgContext->resData;

    Dcm_Dsp_DidServices_ReadDid_Init(
      /* Deviation MISRAC2012-1 <+7> */
      /* context */ &Dcm_ReadScalingDidContext,
      /* Did */ Did,
      /* DidOp */ DCM_DID_OP_READSCALING,
      /* WriteDidIds */ DCM_WRITE_DID_IDS,
      /* OutputBuffer */ &ReadScalingDid_BufferCfg,
      /* enable session / security / rule checks */ DCM_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS,
      /* RxPudId for protocol checks */ DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId));

    serviceProcessStatus = Dcm_Dsp_ReadScalingDataByIdentifier_SvcContinue(DCM_INITIAL, pMsgContext);
  }

  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcContinue(
  Dcm_OpStatusType      OpStatus,
  Dcm_MsgContextPtrType pMsgContext)
{
  /* !LINKSTO Dcm.Dsn.IB.RSDBI.SvcContinue,1 */
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* the service handler shall not call this function to cancel a request */
  DCM_PRECONDITION_ASSERT(OpStatus != DCM_CANCEL, DCM_INTERNAL_API_ID);

  /* store the requested OpStatus in the context */
  DCM_DSP_CONTEXT_OPSTATUS(DCM_DSP_CONTEXT(&Dcm_ReadScalingDidContext)) = OpStatus;
  /* execute ReadDidList */
  serviceProcessStatus = Dcm_Dsp_DidServices_ReadDid_Execute(&Dcm_ReadScalingDidContext);

  if (DCM_E_OK == serviceProcessStatus)
  {
    CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfg =
      Dcm_ReadScalingDidContext.ReadDidContext.DidBufferCfgPtr;

    /* Update the response length */
    pMsgContext->resDataLen = BufferCfg->Occupied;

    /* service processing successfully finished */
    serviceProcessStatus = DCM_E_DONE;
  }
  else if (DCM_E_NOT_OK == serviceProcessStatus)
  {
    Dcm_ExternalSetNegResponse(
                     pMsgContext, DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&Dcm_ReadScalingDidContext)));
  }
  else
  {
    /* do nothing */
  }

  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_Cancel(
  P2VAR(Dcm_ReadDidContextType, AUTOMATIC, DCM_VAR) ReadScalingDidContext)
{
  /* !LINKSTO Dcm.Dsn.IB.RSDBI.Cancel,1 */
  /* cancel the function */
  return Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(ReadScalingDidContext));
}
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

/*==================[end of file]================================================================*/
