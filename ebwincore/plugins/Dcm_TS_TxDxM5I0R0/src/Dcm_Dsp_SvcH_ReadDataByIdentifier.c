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

/* !LINKSTO Dcm.Dsn.File.RDBI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service ReadDataByIdentifier. */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
  *     "A conversion should not be performed from pointer to void into pointer to object."
  *
  *     Reason:
  *     It is a pointer to void casted to a specific pointer necessary for a specific DID operation.
  *     In this way we have a single context type for all DID operations instead of different
  *     context types for each operation.
  *
  *  MISRAC2012-2) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * the warning is related to DET asserts. The condition is supposed to be
 * always true, otherwise it would trigger an assert.
 *
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

#include <Dcm_Dsp_SvcH_ReadDataByIdentifier.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x22 handling. */
#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON)
#include <Dcm_Dsp_DidServices.h>
#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

/** \brief Minimum length of a RDBI diagnostic request */
#if (defined DCM_RDBI_REQ_LEN_MIN)
  #error "DCM_RDBI_REQ_LEN_MIN is already defined"
#endif
#define DCM_RDBI_REQ_LEN_MIN (2U)

/* !LINKSTO Dcm.Dsn.RDBI.SM.ReadDidList,1 */
/* Processing step identifiers for function: ReadDidList */
#if (defined DCM_RDBI_READDIDLIST_DIDSELECT)
  #error "DCM_RDBI_READDIDLIST_DIDSELECT is already defined"
#endif
#define DCM_RDBI_READDIDLIST_DIDSELECT 0U

#if (defined DCM_RDBI_READDIDLIST_DIDREAD)
  #error "DCM_RDBI_READDIDLIST_DIDREAD is already defined"
#endif
#define DCM_RDBI_READDIDLIST_DIDREAD   1U

/* Mandatory access macros for context of function: ReadDidList */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_READDIDLIST_CONTEXT_PTR)
  #error "DCM_DSP_READDIDLIST_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-1 */
#define DCM_DSP_READDIDLIST_CONTEXT_PTR(context) \
  (((Dcm_ReadDidListContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_READDIDLIST_CONTEXT)
  #error "DCM_DSP_READDIDLIST_CONTEXT is already defined"
#endif
#define DCM_DSP_READDIDLIST_CONTEXT(context)     \
  (DCM_DSP_READDIDLIST_CONTEXT_PTR(context)->ReadDidListContext)

/** \brief Macro to get the value of ConditionCheckFailed from ReadSignalContext */
#if (defined DCM_DSP_READDIDLIST_CONDFAILED)
  #error "DCM_DSP_READDIDLIST_CONDFAILED is already defined"
#endif
#define DCM_DSP_READDIDLIST_CONDFAILED(context) \
  (context).ReadDidListContext.ReadDidContext.ReadDidContext.ReadStaticDidContext.\
  ReadStaticDidContext.ReadSingleStaticDidContext.ReadSingleStaticDidContext.ReadSignalContext.\
  SignalReadContext.ConditionCheckFailed

/** \brief Macro to get the value of ModeRuleFailed from DidInfoContext */
#if (defined DCM_DSP_READDIDLIST_MODERULEFAILED)
  #error "DCM_DSP_READDIDLIST_MODERULEFAILED is already defined"
#endif
#define DCM_DSP_READDIDLIST_MODERULEFAILED(context) \
  (context).ReadDidListContext.ReadDidContext.ReadDidContext.DidInfoContext.DidInfoContext.\
  ModeRuleFailed

/** \brief Macros for LockStatus used by ReadDataByIdentifier service */
#define DCM_TRYING_LOCK                            (0x00U)
#define DCM_EXECUTING_EXTERNAL_SERVICE             (0x01U)
#define DCM_EXECUTING_INTERNAL_SERVICE             (0x02U)

/*===========================[type definitions]=================================================*/

/* Context type definitions for function: ReadDidList */
typedef struct
{
  Dcm_ReadDidContextType  ReadDidContext;
  Dcm_DspMsgBufferCfgType BufferCfg;
  P2VAR(uint8, TYPEDEF, DCM_VAR) DidList;
  uint16 DidCount;
  uint16 DidsRead;
  uint16 DidIdx;
  PduIdType RxPduId;
} Dcm_ReadDidListContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType       BaseContext;
  Dcm_ReadDidListContextExtType ReadDidListContext;
} Dcm_ReadDidListContextType;

typedef P2VAR(Dcm_ReadDidListContextType, TYPEDEF, DCM_VAR) Dcm_ReadDidListContextTypePtr;

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Internal service handler for UDS service 0x22
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcStart(
  Dcm_MsgContextPtrType pMsgContext);

/** \brief Internal service handler for UDS service 0x22
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcContinue(
  Dcm_OpStatusType OpStatus,
  Dcm_MsgContextPtrType pMsgContext);

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Init(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)  DidList,
  uint16                            DidCount,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)  OutputBuffer,
  uint32                            BufferSize,
  uint32                            AmountOfBufferOccupied,
  PduIdType                         RxPduId);

/** \brief Executes reading Dids from a list based on the given context
 *
 * \param ReadDidListContext [in] Pointer to the context to be processed.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Execute(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext);

/** \brief Cancels reading Dids from a list based on the given context
 *
 * \param ReadDidListContext [in] Pointer to the context whose execution shall
 *                                be cancelled.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Cancel(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext);

/* !LINKSTO Dcm.Dsn.RDBI.SM.ReadDidList,1 */
/* Processing step functions prototypes for function: ReadDidList  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidSelect(
  Dcm_Dsp_ContextPtr context);

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidRead(
  Dcm_Dsp_ContextPtr context);

/** \brief This function is used to execute the ReadDataByIdentifier service functionality
 **        after the DidService unit was locked by ReadDataByIdentifier.
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.RDBI.SM.ReadDidList,1 */
/** \brief Processing step defintions for function: ReadDidList */
/* Deviation MISRAC2012-2 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_ReadDidListStateConfig[] =
{
  /* DID SELECT */ &Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidSelect,
  /* DID READ   */ &Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidRead,
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/



#if (DCM_COMMON_BUFF_CONFIGURED == STD_ON)
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Used to store the received Dids, if a single buffer is configured as Rx and Tx buffer. */
/* Deviation MISRAC2012-2 */
STATIC VAR(Dcm_MsgItemType, DCM_VAR) Dcm_ReadDIDBuffer[DCM_READ_DID_MAX * DCM_DID_ID_SIZE];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#endif /* #if(DCM_COMMON_BUFF_CONFIGURED == STD_ON) */

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Context for reading multiple Dids (as received by service 0x22) */
STATIC VAR(Dcm_ReadDidListContextType, DCM_VAR) Dcm_ReadDidListContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This variable holds if the ReadDataByIdentifier service has locked
 **        the DidServices unit.
 ** This variable can have the following states:
 ** DCM_TRYING_LOCK                 - The DidService is not locked by ReadDataByIdentifier (0x22)
 **                                   service
 ** DCM_EXECUTING_INTERNAL_SERVICE  - The DidService is locked by ReadDataByIdentifier (0x22)
 **                                   service via an internal request (ROE)
 ** DCM_EXECUTING_EXTERNAL_SERVICE  - The DidService is locked by ReadDataByIdentifier (0x22)
 **                                   service via an external request
 */
STATIC VAR(uint8, DCM_VAR) Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>
#endif
/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_Init(void)
{
  DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_ENTRY();

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_EXIT();
}

/* This is the generic service handler for UDS service 0x22 */
/* !LINKSTO Dcm.Dsn.IB.RDBI.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_ENTRY(OpStatus,pMsgContext);

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  switch(Dcm_ReadDataByIdentifier_LockStatus)
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
          /* !LINKSTO Dcm.EB.DidServices.Locking.ROERequestWhileLockedByWriteDataByIdentifier,2 */
          /* !LINKSTO Dcm.EB.DidServices.Locking.ROERequestWhileLockedByInputOutputControlByIdentifier,2 */
          /* !LINKSTO Dcm.EB.DidServices.Locking.ReadDataByIdentifierWhileLockedByReturnControlToECU,1 */
          /* !LINKSTO Dcm.EB.DidServices.Locking.ROERequestWhileLockedByReturnControlToECU,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.BusyWithRequestVehicleInformation.Postpone,1 */
          ServiceProcessStatus = DCM_E_PENDING;
        }
        else
        {
          DCM_POSTCONDITION_ASSERT(LockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);

          /* Set the message type that has locked the DidService unit */
          if(pMsgContext->msgAddInfo.requestOrigin == DCM_INTERNAL_ROE)
          {
            Dcm_ReadDataByIdentifier_LockStatus = DCM_EXECUTING_INTERNAL_SERVICE;
          }
          else
          {
            DCM_POSTCONDITION_ASSERT(pMsgContext->msgAddInfo.requestOrigin == DCM_EXTERNAL,
                                                                               DCM_INTERNAL_API_ID);
            Dcm_ReadDataByIdentifier_LockStatus = DCM_EXECUTING_EXTERNAL_SERVICE;
          }

          ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(DCM_INITIAL,
                                                                                      pMsgContext);
        }
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    case DCM_EXECUTING_INTERNAL_SERVICE:
    {
      /* If the ROE-generated ReadDataByIdentifier (0x22) request has the DidServices unit lock and
       * in the meantime an external ReadDataByIdentifier (0x22) request is received and dispatched,
       * then the ROE-generated ReadDataByIdentifier (0x22) request will be cancelled. Therefore,
       * an external ReadDataByIdentifier (0x22) request shall never find the ReadDataByIdentifier
       * (0x22) service handler already in execution.
       */
      DCM_POSTCONDITION_ASSERT(pMsgContext->msgAddInfo.requestOrigin == DCM_INTERNAL_ROE, DCM_INTERNAL_API_ID);

      ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
    }
    break;

    case DCM_EXECUTING_EXTERNAL_SERVICE:
    {
      /* If the current request has the locked over DidServices unit then continue processing,
       * otherwise return pending until the locked is removed.
       */
      if(pMsgContext->msgAddInfo.requestOrigin == DCM_EXTERNAL)
      {
        ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
      }
      else
      {
        /* !LINKSTO Dcm.EB.DidServices.Locking.ROERequestWhileLockedByReadDataByIdentifier,2 */
        ServiceProcessStatus = DCM_E_PENDING;
      }
    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      ServiceProcessStatus = DCM_E_DONE;
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READDATABYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }
#else
  ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(OpStatus, pMsgContext);
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.RDBI.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcStart(
  Dcm_MsgContextPtrType pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* calculate the number of Dids in the diagnostic request */
  const uint16 didCount = (uint16)((pMsgContext->reqDataLen) / DCM_DID_ID_SIZE);

  /* Check if the request length is even and bigger than the configured minimum */
  if ( (pMsgContext->reqDataLen < DCM_RDBI_REQ_LEN_MIN) ||
       ((pMsgContext->reqDataLen & 0x01U) != 0x00U))
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif
  }
  /* If DCM_READ_DID_MAX is zero no need to check the request Dids limit.*/
  /* Check if the no.of requested Dids not exceeding the allowed limits.*/
  else if (didCount > DCM_READ_DID_MAX)
  {
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2013)
    /* !LINKSTO SWS_Dcm_01335,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
#else
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
#endif

    serviceProcessStatus = DCM_E_DONE;
#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif
  }
  else
  {
    P2VAR(uint8, AUTOMATIC, DCM_VAR) didList;

#if (DCM_COMMON_BUFF_CONFIGURED == STD_ON)
    /* If RX and TX buffer is common */
    if (pMsgContext->reqData == pMsgContext->resData)
    {
      /* Copy the Dids to the buffer Dcm_ReadDIDBuffer[] */
      TS_MemCpy(Dcm_ReadDIDBuffer, pMsgContext->reqData, (uint16)pMsgContext->reqDataLen);
      didList = Dcm_ReadDIDBuffer;
    }
    else
#endif
    {
      didList = pMsgContext->reqData;
    }

    Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Init(
      &Dcm_ReadDidListContext,
      didList,
      didCount,
      pMsgContext->resData,
      pMsgContext->resMaxDataLen,
      pMsgContext->resDataLen,
      pMsgContext->dcmRxPduId);

    serviceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_SvcContinue(DCM_INITIAL, pMsgContext);
  }

  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.RDBI.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcContinue(
  Dcm_OpStatusType      OpStatus,
  Dcm_MsgContextPtrType pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* the service handler shall not call this function to cancel a request */
  DCM_PRECONDITION_ASSERT(OpStatus != DCM_CANCEL, DCM_INTERNAL_API_ID);

  /* store the requested OpStatus in the context */
  DCM_DSP_CONTEXT_OPSTATUS(DCM_DSP_CONTEXT(&Dcm_ReadDidListContext)) = OpStatus;
  /* execute ReadDidList */
  serviceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Execute(&Dcm_ReadDidListContext);

  if (DCM_E_OK == serviceProcessStatus)
  {
    CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfg =
      &Dcm_ReadDidListContext.ReadDidListContext.BufferCfg;

    /* Update the response length */
    pMsgContext->resDataLen = BufferCfg->Occupied;

    /* service processing successfully finished */
    serviceProcessStatus = DCM_E_DONE;

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif
  }
  else if (DCM_E_NOT_OK == serviceProcessStatus)
  {
    Dcm_ExternalSetNegResponse(
      pMsgContext, DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&Dcm_ReadDidListContext)));

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif
  }
  else
  {
    /* do nothing */
  }

  return serviceProcessStatus;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Init(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DidList,
  uint16 DidCount,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferSize,
  uint32 AmountOfBufferOccupied,
  PduIdType RxPduId)
{
  P2VAR(Dcm_ReadDidListContextExtType, AUTOMATIC, DCM_VAR) ReadDidList =
    &ReadDidListContext->ReadDidListContext;

  /* intialize base context */
  DCM_DSP_CONTEXT_INIT(ReadDidListContext, Dcm_ReadDidListStateConfig);

  /* initialize ReadDidList */
  ReadDidList->DidList  = DidList;
  ReadDidList->DidCount = DidCount;
  ReadDidList->DidsRead = 0U;
  ReadDidList->DidIdx   = 0U;
  ReadDidList->RxPduId  = RxPduId;
  /* - output buffer */
  ReadDidList->BufferCfg.BufferPtr = OutputBuffer;
  ReadDidList->BufferCfg.Size      = BufferSize;
  ReadDidList->BufferCfg.Occupied  = AmountOfBufferOccupied;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Execute(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext)
{
  Std_ReturnType result =
    Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(ReadDidListContext));

  return result;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Cancel(
  P2VAR(Dcm_ReadDidListContextType, AUTOMATIC, DCM_VAR) ReadDidListContext)
{
  /* cancel the function */
  return Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(ReadDidListContext));
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidSelect (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-1 <+2> */
  if (DCM_DSP_READDIDLIST_CONTEXT(context).DidIdx <
      DCM_DSP_READDIDLIST_CONTEXT(context).DidCount)
  {
    /* Deviation MISRAC2012-1 <+4> */
    const uint16 DidPos =
      DCM_DID_ID_SIZE * DCM_DSP_READDIDLIST_CONTEXT(context).DidIdx;
    uint8 HighByte = (uint8)DCM_DSP_READDIDLIST_CONTEXT(context).DidList[DidPos];
    uint8 LowByte  = (uint8)DCM_DSP_READDIDLIST_CONTEXT(context).DidList[DidPos + 1U];

    /* Getting the requested DID from request buffer */
    const uint16 Did = (uint16)DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

    Dcm_Dsp_DidServices_ReadDid_Init(
      /* Deviation MISRAC2012-1 <+7> */
      /* context */ &DCM_DSP_READDIDLIST_CONTEXT(context).ReadDidContext,
      /* Did */ Did,
      /* DidOp */ DCM_DID_OP_READ,
      /* WriteDidIds */ DCM_WRITE_DID_IDS,
      /* OutputBuffer */ &DCM_DSP_READDIDLIST_CONTEXT(context).BufferCfg,
      /* enable session / security / rule checks */ DCM_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS,
      /* RxPudId for protocol checks */ DCM_DSP_READDIDLIST_CONTEXT(context).RxPduId);

    DCM_DSP_CONTEXT_NEXT(context) = DCM_RDBI_READDIDLIST_DIDREAD;
  }
  else
  {
    /* Deviation MISRAC2012-1 */
    if (0U == DCM_DSP_READDIDLIST_CONTEXT(context).DidsRead)
    {
      DCM_DSP_CONTEXT_NRC(context)    = DCM_E_REQUESTOUTOFRANGE;
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
    else
    {
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
    }
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ReadDidList_DidRead(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-1 */
  Dcm_ReadDidContextTypePtr ReadDidContextPtr =
    &DCM_DSP_READDIDLIST_CONTEXT(context).ReadDidContext;

  Std_ReturnType result;

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadDid_Cancel(ReadDidContextPtr);
  }
  else
  {
    /* execute subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadDid_Execute(ReadDidContextPtr);

    if (DCM_E_OK == result)
    {
      /* Deviation MISRAC2012-1 <+6> */
      /* increase the number of Dids read */
      DCM_DSP_READDIDLIST_CONTEXT(context).DidsRead += 1U;
      /* advance the Did index */
      DCM_DSP_READDIDLIST_CONTEXT(context).DidIdx += 1U;
      /* Update the response length */
      DCM_DSP_READDIDLIST_CONTEXT(context).BufferCfg.Occupied =
        ReadDidContextPtr->ReadDidContext.DidBufferCfgPtr->Occupied;
      /* next state: select the next Did to be read */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_RDBI_READDIDLIST_DIDSELECT;
    }
    else if (DCM_E_NOT_OK == result)
    {
      /* Indicates that either ConditionCheckRead failed with a nrc different than 0
       * or ModeRule check failed */
      boolean ForceStopProcessing = FALSE;

      if ((DCM_DSP_READDIDLIST_CONDFAILED(Dcm_ReadDidListContext) == TRUE) ||
          (DCM_DSP_READDIDLIST_MODERULEFAILED(Dcm_ReadDidListContext) == TRUE))
      {
        ForceStopProcessing = TRUE;
      }
      /* Deviation TASKING-1 */
      if ((ForceStopProcessing == TRUE) ||
          (DCM_E_REQUESTOUTOFRANGE != DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadDidContextPtr))))
      {
        /* inherit the Nrc of the subfunction */
        DCM_DSP_CONTEXT_NRC(context) =
          DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadDidContextPtr));
      }
      else
      {
        /* Deviation TASKING-1 */
        DCM_PRECONDITION_ASSERT(
          DCM_E_REQUESTOUTOFRANGE == DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadDidContextPtr)),
          DCM_INTERNAL_API_ID);

        /* Deviation TASKING-1 */
        DCM_PRECONDITION_ASSERT(FALSE == ForceStopProcessing,DCM_INTERNAL_API_ID);

        /* the service handler shall not call this function to cancel a request */

        /* ROOR is only returned in case the number of Dids read remains 0 */

        /* advance the Did index */
        /* Deviation MISRAC2012-1 */
        DCM_DSP_READDIDLIST_CONTEXT(context).DidIdx += 1U;
        /* next state: select the next Did to be read */
        DCM_DSP_CONTEXT_NEXT(context) = DCM_RDBI_READDIDLIST_DIDSELECT;
      }
    }
    else
    {
      /* result remains DCM_E_PENDING */
    }
  }

  /* inherit the result of the subfunction */
  DCM_DSP_CONTEXT_RESULT(context) = result;

}

/* !LINKSTO Dcm.Dsn.IB.RDBI.ServiceExecution,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_SvcStart(pMsgContext);
    }
    break;

    /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
    {
      ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_SvcContinue(OpStatus, pMsgContext);
    }
    break;

    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReadMemory.CallRCRRP,1 */
    /* Subsequent call to the service function after FORCE_RCRRP */
    case DCM_FORCE_RCRRP_OK:
    {
      ServiceProcessStatus = Dcm_Dsp_ReadDataByIdentifier_SvcContinue(OpStatus, pMsgContext);
    }
    break;

    /* cancel any ongoing service processing */
    case DCM_CANCEL:
    {
      /* cancel the service */
      ServiceProcessStatus =
                           Dcm_Dsp_ReadDataByIdentifier_ReadDidList_Cancel(&Dcm_ReadDidListContext);
#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
      /* Unlock DidServices */
      Dcm_Dsp_DidServices_UnlockService();
      Dcm_ReadDataByIdentifier_LockStatus = DCM_TRYING_LOCK;
#endif
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
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READDATABYIDENTIFIER);
      ServiceProcessStatus = DCM_E_DONE;
    }
    break;
    /* CHECK: PARSE */
  }

  /* Map MemServices RESOURCE_LOCKED onto DCM_E_PENDING so the services gets called once more */
  if (ServiceProcessStatus == DCM_E_RESOURCE_LOCKED)
  {
    ServiceProcessStatus = DCM_E_PENDING;
  }

  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X22 == STD_ON) */

/*==================[end of file]================================================================*/
