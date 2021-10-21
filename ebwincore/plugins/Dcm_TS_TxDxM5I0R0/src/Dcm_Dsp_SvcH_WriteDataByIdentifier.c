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

/* !LINKSTO Dcm.Dsn.File.WDBI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service WriteDataByIdentifier. */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

#include <Dcm_Dsp_SvcH_WriteDataByIdentifier.h>
#include <Dcm_Dsl_Supervisor.h>

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)

#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

/** \brief Minimal length of the data record in the WriteDataByIdentifier request message */
#if (defined DCM_DREC_MIN_LENGTH)
  #error "DCM_DREC_MIN_LENGTH is already defined"
#endif
#define DCM_DREC_MIN_LENGTH 1U

/*===========================[type definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Internal service handler for UDS service 0x2E
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Internal service handler for UDS service 0x2E
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcContinue(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);


/** \brief This function is used to execute the WriteDataByIdentifier service functionality
 **        after the DidService unit was locked by WriteDataByIdentifier.
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* Holds the context information of a WriteDid request */
/* Rationale for mapping Dcm_WriteDIDContext to NOINIT section.
 *
 * Members of Dcm_WriteDIDContextType are initialized and accessed as described below:
 * RequestProcessingSt : Initialized in Dcm_DspInternal_WriteDataByIdentifier_SvcH() before
 *                       being accessed from Dcm_Dsp_WriteDataByIdentifier_SvcStart().
 * DidAsynchSignalSt : Initialized in Dcm_Dsp_WriteDataByIdentifier_SvcStart() before
 *                     being accessed from Dcm_DspInternal_WriteDid().
 * SignalInd  : Initialized in Dcm_Dsp_WriteDataByIdentifier_SvcStart() before being
 *              accessed from the Dcm_DspInternal_WriteDid().
 */
/*
 STATIC VAR(Dcm_WriteDIDContextType, DCM_VAR) Dcm_WriteDIDContext[DCM_NUM_PROTOCOL];
 */
/** \brief Array to hold information regarding the processing of 0x22 Service. */
VAR(Dcm_WriteDidContextType, DCM_VAR) Dcm_WriteDidContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This variable holds if the WriteDataByIdentifier service has locked
 **         the DidServices unit.
 **  DCM_TRYING_LOCK       - WriteDataByIdentifier service has the lock over DidServices unit.
 **  DCM_EXECUTING_SERVICE - WriteDataByIdentifier does not have the lock over DidServices unit.
 */
STATIC VAR(uint8, DCM_VAR) Dcm_WriteDataByIdentifier_LockStatus = DCM_TRYING_LOCK;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>
#endif
/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_Init(void)
{
  DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_ENTRY();

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  Dcm_WriteDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.WDBI.SvcH,1 */
/* The generic service handler for UDS service 0x2E */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_ENTRY(OpStatus, pMsgContext);

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  switch(Dcm_WriteDataByIdentifier_LockStatus)
  {
    case DCM_TRYING_LOCK:
    {
      if((OpStatus != DCM_CANCEL) &&
         (OpStatus != DCM_CONFIRMED_OK) &&
         (OpStatus != DCM_CONFIRMED_NOK))
      {

        Std_ReturnType LockStatus = Dcm_Dsp_DidServices_LockService();

        if (LockStatus == DCM_E_RESOURCE_LOCKED)
        {
          /* !LINKSTO Dcm.EB.DidServices.Locking.WriteDataByIdentifierWhileLockedByReturnControlToECU,1 */
          ServiceProcessStatus = DCM_E_PENDING;
        }
        else
        {
          DCM_POSTCONDITION_ASSERT(LockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);

          Dcm_WriteDataByIdentifier_LockStatus = DCM_EXECUTING_SERVICE;
          ServiceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_ServiceExecution(DCM_INITIAL,
                                                                                       pMsgContext);
        }
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    case DCM_EXECUTING_SERVICE:
    {
      ServiceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      ServiceProcessStatus = DCM_E_DONE;
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_WRITEDATABYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }
#else
  ServiceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.WDBI.SvcStart,1 */
/* The function for implementation of UDS service 0x2E */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the instance id. This service is only called externally. */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* holds the context information of currently processed DID */

  if (pMsgContext->reqDataLen < (DCM_DID_ID_SIZE + DCM_DREC_MIN_LENGTH))
  {
    /* !LINKSTO Dcm.EB.WriteDataByIdentifier.NRC.0x13.DataRecord.Minimal.Length,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_NOT_OK;
  }
  else
  {
    /* calculate the requested DID */
    uint8 HighByte = (uint8)pMsgContext->reqData[0U];
    uint8 LowByte  = (uint8)pMsgContext->reqData[1U];

    uint16 Did = (uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

    Dcm_Dsp_DidServices_WriteDid_Init(
      &Dcm_WriteDidContext,
      Did,
      /* request data      */ &pMsgContext->reqData[DCM_DID_ID_SIZE],
      /* request data len  */ (uint32)pMsgContext->reqDataLen - DCM_DID_ID_SIZE,
      instIdx);

    serviceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_SvcContinue(DCM_INITIAL, pMsgContext);
  }

  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.WDBI.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcContinue(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* the service handler shall not call this function to cancel a request */
  DCM_PRECONDITION_ASSERT(OpStatus != DCM_CANCEL, DCM_INTERNAL_API_ID);

  /* store the requested OpStatus in the context */
  DCM_DSP_CONTEXT_OPSTATUS(DCM_DSP_CONTEXT(&Dcm_WriteDidContext)) = OpStatus;
  /* execute WriteDid */
  serviceProcessStatus = Dcm_Dsp_DidServices_WriteDid_Execute(&Dcm_WriteDidContext);

  if (DCM_E_OK == serviceProcessStatus)
  {
    /* Assemble the Did in response and update the response length.*/
    pMsgContext->resData[0U] = pMsgContext->reqData[0U];
    pMsgContext->resData[1U] = pMsgContext->reqData[1U];
    pMsgContext->resDataLen = DCM_DID_ID_SIZE;

    serviceProcessStatus = DCM_E_DONE;
  }
  else if (DCM_E_NOT_OK == serviceProcessStatus)
  {
    Dcm_ExternalSetNegResponse(
      pMsgContext, DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&Dcm_WriteDidContext)));
  }
  else
  {
    /* do nothing */
  }

  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.WDBI.ServiceExecution,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      ServiceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_SvcStart(pMsgContext);
    }
    break;

    /* consecutive call after the function retuned PENDING before */
    case DCM_PENDING:
    {
      ServiceProcessStatus = Dcm_Dsp_WriteDataByIdentifier_SvcContinue(OpStatus, pMsgContext);
    }
    break;

    /* cancel any ongoing service processing */
    case DCM_CANCEL:
    {
      /* cancel the service */
      ServiceProcessStatus = Dcm_Dsp_DidServices_WriteDid_Cancel(&Dcm_WriteDidContext);
    }
    break;

    /* actions performed on successful sending of the response */
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
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_WRITEDATABYIDENTIFIER);
      ServiceProcessStatus = DCM_E_DONE;
    }
    break;
    /* CHECK: PARSE */
  }

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  if ((  (ServiceProcessStatus == DCM_E_OK)
       ||(ServiceProcessStatus == DCM_E_NOT_OK)
       ||(ServiceProcessStatus == DCM_E_ABORT)
       ||(ServiceProcessStatus == DCM_E_DONE)
      ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK))
  {
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_WriteDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
  }
#endif

  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X2E == STD_ON) */

/*==================[end of file]================================================================*/
