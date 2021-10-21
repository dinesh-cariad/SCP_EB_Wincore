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

/* !LINKSTO Dcm.Dsn.File.DDDI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service DynamicallyDefineDataIdentifier. */


/*===============================[includes]======================================================*/

#include <Dcm_Trace_Internal.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_SvcH_DynamicallyDefineDataIdentifier.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x2C handling. */

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)

#include <Dcm_Dsp_DidServices.h>

#include <Dcm_Dsl_Supervisor.h>                                /* Dcm Dsl Supervisor header file */

#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#include <TSMem.h>                                               /* EB specific memory functions */

/*==================[macro definitions]==========================================================*/

/** \brief Minimum length of a DDDID diagnostic request for sub-function 0x01 */
#if (defined DCM_DDDID_REQ_LEN_MIN_SS01)
  #error "DCM_DDDID_REQ_LEN_MIN_SS01 is already defined"
#endif
#define DCM_DDDID_REQ_LEN_MIN_SS01 (6U)

/** \brief Length of a parameter triplet for DDDID diagnostic request, sub-function 0x01 */
#if (defined DCM_DDDID_REQ_SS01_PARAM_LEN)
  #error "DCM_DDDID_REQ_SS01_PARAM_LEN is already defined"
#endif
#define DCM_DDDID_REQ_SS01_PARAM_LEN (4U)

/* Magic numbers for extracting data source info from 0x2C (DDDID) service request */
#if (defined DCM_HIGH_BYTE)
  #error "DCM_HIGH_BYTE is already defined"
#endif
#define DCM_HIGH_BYTE                           (2U)

#if (defined DCM_LOW_BYTE)
  #error "DCM_LOW_BYTE is already defined"
#endif
#define DCM_LOW_BYTE                            (3U)

#if (defined DCM_INDEX_IN_SOURCE)
  #error "DCM_INDEX_IN_SOURCE is already defined"
#endif
#define DCM_INDEX_IN_SOURCE                     (4U)

#if (defined DCM_SIZE)
  #error "DCM_SIZE is already defined"
#endif
#define DCM_SIZE                                (5U)

#if (defined DCM_BYTES_PER_REQUEST)
  #error "DCM_BYTES_PER_REQUEST is already defined"
#endif
#define DCM_BYTES_PER_REQUEST                   (4U)

/*===========================[type definitions]==================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x2C internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcStart(
  Dcm_OpStatusType OpStatus,
  Dcm_MsgContextPtrType pMsgContext);

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcStart(
  Dcm_MsgContextPtrType pMsgContext);

STATIC FUNC(Std_ReturnType, DCM_CODE)
  Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue(
    Dcm_OpStatusType OpStatus,
    Dcm_MsgContextPtrType pMsgContext);
#endif /* #if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
STATIC FUNC(Std_ReturnType, DCM_CODE)
  Dcm_Dsp_DynamicallyDefineDataIdentifier_CheckMaxLengthPeriodicDDDID(
    P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
    uint32 DDDIDSize);
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* This is the generic service handler for UDS service 0x2C */
/* !LINKSTO Dcm.Dsn.IB.DDDI.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  boolean DynamicDidInitialized = Dcm_Dsp_DidServices_DynamicDid_Initialized();

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_ENTRY(OpStatus, pMsgContext);

  if (FALSE == DynamicDidInitialized)
  {
    /* !LINKSTO Dcm.EB.DDDID.NotInitialized,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    switch (OpStatus)
    {
      /* first call of service function */
      case DCM_INITIAL:
      case DCM_PENDING:
      case DCM_CANCEL:
        serviceProcessStatus =
          Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcStart(OpStatus, pMsgContext);
        break;
      default:
        serviceProcessStatus = DCM_E_DONE;
        break;
    }
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}

/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.DDDI.SvcStart,1 */
/* The function for implementation of UDS service 0x2C */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus;

  /* SWS_Dcm_00764: if get a valid pointer to subServiceHandler, then pMsgContext will point
   * behind the subFunction identifier */
  Dcm_GenericServiceHandlerType subServiceHandler =
    Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

  DCM_PRECONDITION_ASSERT(subServiceHandler != NULL_PTR, DCM_SVCID_DYNAMICALLYDEFINEDDID);
  /* Call sub-service handler */
  serviceProcessStatus = subServiceHandler(OpStatus, pMsgContext);
  /* Sub-service handlers return E_OK, so we must convert to DCM_E_DONE */
  if (serviceProcessStatus == E_OK)
  {
    serviceProcessStatus = DCM_E_DONE;
  }

  return serviceProcessStatus;
}

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
/* This is the UDS service 0x2C-0x01 handler */
/* !LINKSTO Dcm.Dsn.IB.DDDI.AddSrcByDid.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus;

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_ENTRY(OpStatus, pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      serviceProcessStatus =
        Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcStart(pMsgContext);
    }
    break;

    default:
    {
      serviceProcessStatus =
        Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue(OpStatus, pMsgContext);
    }
    break;
  }

  if (DCM_E_DONE == serviceProcessStatus)
  {
    /* Set positive response. */
    pMsgContext->resData[0U] = 0x01; /* Subservice ID */
    pMsgContext->resData[1U] = pMsgContext->reqData[0U]; /* DID MSB */
    pMsgContext->resData[2U] = pMsgContext->reqData[1U]; /* DID LSB */
    pMsgContext->resDataLen = 3U;

#if (DCM_DDDID_PERSISTENCE == STD_ON)
    /* !LINKSTO SWS_Dcm_00868,1 */
    /* If DDDID persistence is enabled store the DDDID configuration to Nvm during NvM_WriteAll() */
    (void)NvM_SetRamBlockStatus(DCM_DDDID_NVM_BLOCK_ID, TRUE);
#endif
  }
  else if (DCM_E_PENDING != serviceProcessStatus)
  {
    /* Set negative response. */
    /* Get the current context info used by Rx Pdu Id */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
      &DCM_HSM_INFO(Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
          pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex);

    /* Because a mode rule can be configured to return any NRC, and
     * Dcm_ExternalSetNegResponse() verifies if the NRC is known, set
     * the NRC directly to avoid having to change the source code
     * along with the configuration */
    curHsmInfo->negativeResponseCode = serviceProcessStatus;
  }
  else
  {
    /* nothing to do */
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.DDDI.AddSrcByDid.SSvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcStart(
  Dcm_MsgContextPtrType pMsgContext)
{
  CONSTP2VAR(Dcm_DDDIDContextType, AUTOMATIC, DCM_VAR) DDDIDContext = &Dcm_DDDIDContext;
  Std_ReturnType serviceProcessStatus;
  const uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_ENTRY(pMsgContext);

  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.IncorrectMessageLength,1 */
  if ((pMsgContext->reqDataLen < DCM_DDDID_REQ_LEN_MIN_SS01) ||
      (((pMsgContext->reqDataLen - DCM_DID_ID_SIZE) % DCM_DDDID_REQ_SS01_PARAM_LEN) != 0U))
  {
    serviceProcessStatus = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
    Dcm_DidInfoContextType DidInfoContext;

    Std_ReturnType ReturnVal = DCM_E_OK;
    /* Get the dynamically defined data identifier ID */
    uint8 HighByte = (uint8)pMsgContext->reqData[0U];
    uint8 LowByte  = (uint8)pMsgContext->reqData[1U];

    const uint16 reqDDDid = (uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);
    /* Check if the request is inside the dynamic range */
    if((reqDDDid < DCM_DID_DMC_LOW_LIMIT) || (reqDDDid > DCM_DID_DMC_UP_LIMIT))
    {
      serviceProcessStatus = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {

      /* Verify if the dynamically defined data identifier can be configured in the current
       * session and security level */
      Dcm_Dsp_DidServices_GetDidInfo_Init(&DidInfoContext, reqDDDid, DCM_DID_OP_DYNAMIC, instIdx);

      ReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Execute(&DidInfoContext);

      if (DCM_E_OK == ReturnVal)
      {
        uint16 dddidSize = 0U;

        CONSTP2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab =
          &Dcm_DDDidSrcTable[Dcm_DidConfig[DidInfoContext.DidInfoContext.ConfigIdx].DDDidSrcTableIdx];

        /* The number of DID sources in this request message. */
        const uint8 nrSrcReq =
          (uint8)((pMsgContext->reqDataLen - DCM_DID_ID_SIZE) / DCM_DDDID_REQ_SS01_PARAM_LEN);
        uint8 nrSrcUsed = 0U;
        uint8 maxSrc    = Dcm_DidInfo[DidInfoContext.DidInfoContext.InfoIdx].DDDidMaxNumSrcElements;

        /* Get the number of currently configured sources and their total length for the DDDID
         * under configuration */
        /* During request processing the DDDidSrcTab is not changed */
        while ((DDDidSrcTab[nrSrcUsed].srcType != DCM_DDDID_SRC_TYPE_NONE)
               && (nrSrcUsed < maxSrc))
        {
          if ((DDDidSrcTab[nrSrcUsed].srcType & DCM_DDDID_SRC_TYPE_DID ) == DCM_DDDID_SRC_TYPE_DID)
          {
            dddidSize += DDDidSrcTab[nrSrcUsed].dataIdDDDid.srcDidSize;
          }
          else
          {
            dddidSize += (uint16)DDDidSrcTab[nrSrcUsed].memAddrDDDid.srcMemSize;
          }
          nrSrcUsed++;
        }
        /* Check that the DDDID has enough space (left) to add the requested number of sources */
        /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.MaxSrcElements,1 */
        if ((nrSrcUsed + nrSrcReq) > maxSrc)
        {
          serviceProcessStatus = DCM_E_REQUESTOUTOFRANGE;
        }
        else
        {
          /* Initialize the context for the SSvcContinue call */
          DDDIDContext->DDDidSrcTab = DDDidSrcTab;
          DDDIDContext->dddidSize = dddidSize;
          DDDIDContext->nrSrcReq  = nrSrcReq;
          DDDIDContext->nrSrcUsed = nrSrcUsed;
          DDDIDContext->ProtocolId = instIdx;
          DDDIDContext->srcDidIdx = 0;

          serviceProcessStatus =
            Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue(DCM_INITIAL, pMsgContext);
        }
      }
      else
      {
        serviceProcessStatus = DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&DidInfoContext));
      }
    }
  }
  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_EXIT(serviceProcessStatus,
    pMsgContext);
  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.DDDI.AddSrcByDid.SSvcContinue,2 */
STATIC FUNC(Std_ReturnType, DCM_CODE)
  Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue(Dcm_OpStatusType OpStatus,
                                                                  Dcm_MsgContextPtrType pMsgContext)
{
  Std_ReturnType serviceProcessStatus;
  uint8 reqIdx;

  CONSTP2VAR(Dcm_DDDIDContextType, AUTOMATIC, DCM_VAR) DDDIDContext = &Dcm_DDDIDContext;
  CONSTP2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab  =
  DDDIDContext->DDDidSrcTab;
  /* Current data length of the DDDID in bytes */
  uint16 dddidSize = DDDIDContext->dddidSize;
  uint8 nrSrcUsed = DDDIDContext->nrSrcUsed;

  /* The DDDID will be defined based on DID sources. */
  uint8 DDDIDType = DCM_DDDID_SRC_TYPE_DID;

  serviceProcessStatus = DCM_E_DONE;
  /* check all requested source DIDs if they are readable in
   * current session, security level and mode */
  for (reqIdx = DDDIDContext->srcDidIdx; reqIdx < DDDIDContext->nrSrcReq; reqIdx++)
  {
    const uint16 didOffset = (uint16)reqIdx * DCM_BYTES_PER_REQUEST;
    const uint16 reqSrcDid = DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(
      (pMsgContext->reqData[DCM_HIGH_BYTE + didOffset]),
      (uint8)(pMsgContext->reqData[DCM_LOW_BYTE + didOffset]));
    /* Position in source. If the value is 0 a NRC(0x31) will be transmitted */
    const uint32 PositionInSource =
      (uint32)pMsgContext->reqData[DCM_INDEX_IN_SOURCE + didOffset] - 1U;
    /* Size of source DID data in bytes */
    const uint32 DataSize = (uint32)pMsgContext->reqData[DCM_SIZE + didOffset];
    Std_ReturnType ReturnVal;
    CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) srcDidInfoContextPtr =
      &(DDDIDContext->srcDidInfoContext);

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
    Std_ReturnType CheckSizeResult =
      Dcm_Dsp_DynamicallyDefineDataIdentifier_CheckMaxLengthPeriodicDDDID(
        pMsgContext, (dddidSize + DataSize));
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */

    /* Check if the source Did can be read in the current conditions as per the configuration */
    if ((DCM_INITIAL == OpStatus) || (reqIdx != DDDIDContext->srcDidIdx))
    {
      Dcm_Dsp_DidServices_GetDidInfo_Init(
        srcDidInfoContextPtr, reqSrcDid, DCM_DID_OP_READ, DDDIDContext->ProtocolId);
    }

    if (DCM_CANCEL != OpStatus)
    {
      ReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Execute(srcDidInfoContextPtr);
    }
    else
    {
      ReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Cancel(srcDidInfoContextPtr);
    }

    if (DCM_E_OK == ReturnVal)
    {
#if (DCM_DID_REF_COUNT > 0U)
      /* DIDs can reference other DIDs which have to be checked too */
      CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) srcDidInfo =
        &srcDidInfoContextPtr->DidInfoContext;

      /* check if the DID is configured by id */
      if ((DCM_DID_TYPE_ID_STATIC == srcDidInfo->ConfigType) ||
          (DCM_DID_TYPE_ID_DYNAMIC == srcDidInfo->ConfigType))
      {
        const uint16 numReferencedDids = Dcm_DidConfig[srcDidInfo->ConfigIdx].NumRefDids;

        /* check if it has referenced DIDs */
        if (0U != numReferencedDids)
        {
          uint16_least referencedDidIdx;
          Std_ReturnType refReturnVal;
          Dcm_DidInfoContextType refDidInfoContext;

          /* check the permissions for the referenced DIDs */
          for (referencedDidIdx = 0U ; referencedDidIdx < numReferencedDids ;referencedDidIdx++)
          {
            const uint16 refDidID =
              Dcm_DidRefers[Dcm_DidConfig[srcDidInfo->ConfigIdx].DidRefersIdx + referencedDidIdx];

            Dcm_Dsp_DidServices_GetDidInfo_Init(
              &refDidInfoContext, refDidID, DCM_DID_OP_READ, DDDIDContext->ProtocolId);
            refReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Execute(&refDidInfoContext);

            /* The referenced DID cannot be range DID, so PENDING response is not expected */
            if (DCM_E_OK != refReturnVal)
            {
              serviceProcessStatus = DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&refDidInfoContext));
              break;
            }
          }
        }
      } /* else: DIDs from ranges cannot reference other DIDs */
#endif /* #if (DCM_DID_REF_COUNT > 0U) */
    }
    else if (DCM_E_PENDING == ReturnVal)
    {
      /* DID could be a rangeDID where IsDidAvailable could return PENDING */
    }
    else
    {
      serviceProcessStatus = DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(srcDidInfoContextPtr));
    }

    /* Calculate DID size only if DID exists and can be read. */
    if (DCM_E_OK == ReturnVal)
    {
      const uint32 DidDataLength =
        Dcm_Dsp_DidServices_GetMaximumDidLength(&srcDidInfoContextPtr->DidInfoContext);

      /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DynamicDIDSrc,1 */
      if ((DCM_DID_TYPE_ID_DYNAMIC == srcDidInfoContextPtr->DidInfoContext.ConfigType) ||
          ((PositionInSource + DataSize) > DidDataLength) ||
          ((dddidSize + DataSize + DCM_DID_ID_SIZE) > DCM_MAX_BUFFER_SIZE) ||
          ((pMsgContext->reqData[DCM_INDEX_IN_SOURCE + didOffset]) == 0U)
         )
      {
        serviceProcessStatus = DCM_E_REQUESTOUTOFRANGE;
      }

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
      if (CheckSizeResult != DCM_E_OK)
      {
        serviceProcessStatus = DCM_E_REQUESTOUTOFRANGE;
      }
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */
    }

    /* Validate that all conditions are fulfilled and that position and size of signal in DID
     * are valid. */
    /* There is no separate check if position is wrong, since if sum of position and size is
     * wrong, correctness of position doesn't matter. */
    if ((DCM_E_NOT_OK == ReturnVal) ||
        (DCM_E_PENDING == ReturnVal) ||
        (DCM_E_DONE != serviceProcessStatus ) ||
        (DCM_CANCEL == OpStatus))
    {
      /* Could not add all requested sources. Roll back changes if it is not DCM_E_PENDING */
      /* Since all searches had been stopped as soon as DCM_DDDID_SRC_TYPE_NONE is found for one
       * source no need to go through all, set just one. */

      if (DCM_E_PENDING == ReturnVal)
      {
        DDDIDContext->srcDidIdx = reqIdx;
        DDDIDContext->nrSrcUsed = nrSrcUsed;
        DDDIDContext->dddidSize = dddidSize;
        serviceProcessStatus = DCM_E_PENDING;
      }
      else
      {
        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        DDDidSrcTab[nrSrcUsed - reqIdx].srcType = DCM_DDDID_SRC_TYPE_NONE;

        /* LEAVE critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      }

      break;
    }

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    DDDidSrcTab[nrSrcUsed].dataIdDDDid.srcDidPos =  (uint8)PositionInSource;
    DDDidSrcTab[nrSrcUsed].dataIdDDDid.srcDidSize = (uint8)DataSize;
    dddidSize += (uint16)DataSize;

    /* Add the source DID to the table of sources of the DDDID */
    DDDidSrcTab[nrSrcUsed].srcType = DDDIDType;
    DDDidSrcTab[nrSrcUsed].dataIdDDDid.srcDid = reqSrcDid;

    nrSrcUsed++;

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCCONTINUE_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);
  return serviceProcessStatus;
}

#endif /* (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON) */

#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
/* This is the UDS service 0x2C-0x02 handler */
/* !LINKSTO Dcm.Dsn.IB.DDDI.AddSrcByMem.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  P2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab = NULL_PTR;
  Dcm_NegativeResponseCodeType nrc = DCM_E_OK;
  uint16 dddidSize = 0U;
  uint8 memAddLength = 0U;
  uint8 memSizeLength = 0U;
  uint8 nrSrcReq = 0U;
  uint8 nrSrcUsed = 0U;

  TS_PARAM_UNUSED(OpStatus);

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_ENTRY(OpStatus,pMsgContext);

  /* Check if the data length is at least 5 bytes (1 byte addressAndLengthFormatIdentifier
     parameter + 2 bytes dynamicallyDefinedDataIdentifier parameter + at least one memoryAddress
     location  parameter + at least one memorySize parameter. If not, send NRC 0x13.*/
  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.IncorrectMessageLength,1 */
  if (pMsgContext->reqDataLen < 5U)
  {
    nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
    /* Check if address and length format are specified and if they are supported. */
    /* !LINKSTO Dcm.EB.0x2C.AddressAndLengthFormatIdentifier,1 */
    if (Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(pMsgContext->reqData[2U]) ==
      DCM_E_NOT_OK)
    {
      /* Address and length format are not supported. */
      nrc = DCM_E_REQUESTOUTOFRANGE;
    }
    else
#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */
    {
      /* Holds the length (number of bytes) of the memoryAddress parameter */
      memAddLength = (pMsgContext->reqData[2U]) & DCM_MASK_NIBBLE;
      /* Holds the length (number of bytes) of the memorySize parameter */
      memSizeLength = (uint8)((pMsgContext->reqData[2U]) >> DCM_NIBBLE_BITS);

      /* If there memory address or memory size length are outside of ranges (expected number of
       * bytes are between 1 and 4. This check will be later done in function
       * Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize, but if it is checked here, some
       * execution can be saved. */
      if((memAddLength < 1U) ||(memSizeLength < 1U) ||
         (memAddLength > DCM_MAX_ADDRESS_BYTES) || (memSizeLength > DCM_MAX_MEMSIZE_BYTES))
      {
        nrc = DCM_E_REQUESTOUTOFRANGE;
      }
      /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineByMemoryAddress.ConsistencyLengthCheck,1 */
      /* If there are incomplete pairs memory address, memory size, report an error. */
      else if((((uint8)pMsgContext->reqDataLen - 3U) % (uint8)(memAddLength + memSizeLength)) != 0U)
      {
        nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
      }
      else
      {
        Std_ReturnType ReturnVal = DCM_E_OK;

        /* Get the dynamically defined data identifier ID */

        uint8 HighByte = (uint8)pMsgContext->reqData[0U];
        uint8 LowByte = (uint8)pMsgContext->reqData[1U];

        const uint16 reqDDDid = DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

        /* Check if the request is inside the dynamic range */
        if((reqDDDid < DCM_DID_DMC_LOW_LIMIT) || (reqDDDid > DCM_DID_DMC_UP_LIMIT))
        {
          nrc = DCM_E_REQUESTOUTOFRANGE;
        }
        else
        {
          /* Verify if the dynamically defined data identifier can be configured
           * in the current session and security level */
        const uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);

          /* Check if the Did can be read in the current conditions as per the configuration */
          Dcm_DidInfoContextType DidInfoContext;
          Dcm_Dsp_DidServices_GetDidInfo_Init(&DidInfoContext, reqDDDid, DCM_DID_OP_DYNAMIC, instIdx);

          ReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Execute(&DidInfoContext);

          if (ReturnVal == DCM_E_OK)
          {
            uint8 maxSrc = Dcm_DidInfo[DidInfoContext.DidInfoContext.InfoIdx].DDDidMaxNumSrcElements;
            /* Check that the DDDID has enough space (left) to add the requested number of sources */
            DDDidSrcTab = &Dcm_DDDidSrcTable[
              Dcm_DidConfig[DidInfoContext.DidInfoContext.ConfigIdx].DDDidSrcTableIdx];

            /* ENTER critical section */
            SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

            /* Get the number of currently configured sources and their total length
             * for the DDDID under configuration */
            while ((DDDidSrcTab[nrSrcUsed].srcType != DCM_DDDID_SRC_TYPE_NONE)
                &&(nrSrcUsed < maxSrc))
            {
              if ( (DDDidSrcTab[nrSrcUsed].srcType & DCM_DDDID_SRC_TYPE_DID)
                   ==
                   DCM_DDDID_SRC_TYPE_DID
                 )
              {
                dddidSize += DDDidSrcTab[nrSrcUsed].dataIdDDDid.srcDidSize;
              }
              else
              {
                dddidSize += (uint16)DDDidSrcTab[nrSrcUsed].memAddrDDDid.srcMemSize;
              }
              nrSrcUsed++;
            }

            /* LEAVE critical section */
            SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

            /* Compute the number of DID sources in this request message. */
            nrSrcReq = ((uint8)(pMsgContext->reqDataLen - 3U)) / (memAddLength + memSizeLength);

            /* Requested number of DDDID sources cannot be handled */
            if ((nrSrcReq + nrSrcUsed) > maxSrc)
            {
              nrc = DCM_E_REQUESTOUTOFRANGE;
            }
          }
          else
          {
            nrc = DidInfoContext.BaseContext.nrc;
          }
        }
      }
    }
  }
  /* If no error encountered until now try to add all memory sources to the DDDID */
  if (nrc == DCM_E_OK)
  {
    nrc = Dcm_Dsp_DynamicallyDefineDataIdentifierAddMemSrcToDDDID(
                                              DDDidSrcTab, dddidSize, nrSrcUsed, pMsgContext);
  }

  if (nrc == DCM_E_OK)
  {
    /* Get the context information current instance */
    P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
      &DCM_HSM_INFO(Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
          pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex);

    /* Set positive response. */
    pMsgContext->resData[0U] = curHsmInfo->subServiceId;
    pMsgContext->resData[1U] = pMsgContext->reqData[0U];
    pMsgContext->resData[2U] = pMsgContext->reqData[1U];
    pMsgContext->resDataLen = 3U;

#if (DCM_DDDID_PERSISTENCE == STD_ON)
    /* If DDDID persistence is enabled store the DDDID configuration to Nvm during NvM_WriteAll() */
    (void)NvM_SetRamBlockStatus(DCM_DDDID_NVM_BLOCK_ID, TRUE);
#endif
  }
  else
  {
    /* Get the current context info used by Rx Pdu Id */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo
      = &DCM_HSM_INFO(Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
          pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex);

    /* Because a mode rule can be configured to return any NRC, and
     * Dcm_ExternalSetNegResponse() verifies if the NRC is known, set
     * the NRC directly to avoid having to change the source code
     * along with the configuration */
    curHsmInfo->negativeResponseCode = nrc;
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_EXIT(
    nrc, OpStatus, pMsgContext);

  return DCM_E_DONE;
}
#endif /* (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */

#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddMemSrcToDDDID(
  P2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab,
  uint16 dddidSize,
  uint8 nrSrcUsed,
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Dcm_NegativeResponseCodeType nrc = DCM_E_NOT_OK;
  uint16 sizeOfdddid;
  uint16 numberOfSrcUsed;
  uint16 IndexOfMemorySources;
  uint8 memAddLength;
  uint8 memSizeLength;
  uint8 nrSrcReq;

  /* The DDDID will be defined based on memory sources. */
  uint8 DDDIDType = DCM_DDDID_SRC_TYPE_MEM;

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_ENTRY(
    DDDidSrcTab, dddidSize, nrSrcUsed, pMsgContext);

  sizeOfdddid = dddidSize;
  numberOfSrcUsed = nrSrcUsed;

  /* Holds the length (number of bytes) of the memoryAddress parameter */
  memAddLength = (pMsgContext->reqData[DCM_DID_ID_SIZE]) & DCM_MASK_NIBBLE;
  /* Holds the length (number of bytes) of the memorySize parameter */
  memSizeLength = (uint8)((pMsgContext->reqData[DCM_DID_ID_SIZE]) >> DCM_NIBBLE_BITS);
  /* Compute the number of MEM sources in this request message. */
  nrSrcReq = ((uint8)(pMsgContext->reqDataLen - 3U)) / (memAddLength + memSizeLength);

  for (IndexOfMemorySources = 0U; IndexOfMemorySources < nrSrcReq; IndexOfMemorySources++)
  {
    Std_ReturnType retVal = DCM_E_NOT_OK;
    /* Variable holding the memory address to be read */
    uint32 readMemAddress = 0U;
    /* Variable holding the size of the memory (number of bytes) to be read */
    uint32 readMemSize = 0U;
    /* Variable holding the index of MemoryRangeInfo table */
    uint16 memRangeIndex = 0U;
    /* Variable holding the index of MemoryIdInfo table */
    uint16 memIdIndex = 0U;

    const uint16 dataOffset =
      2U + (IndexOfMemorySources * ((uint16)memAddLength + (uint16)memSizeLength));

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
    Std_ReturnType CheckSizeResult = DCM_E_OK;
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */

#if (DCM_PRECONDITION_ASSERT_ENABLED == STD_ON)
    /* Get the Memory address and the size of memory to be read */
    retVal = Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(&(pMsgContext->reqData[dataOffset]),
                                                            memAddLength,
                                                            memSizeLength,
                                                            &readMemAddress,
                                                            &readMemSize);

    DCM_PRECONDITION_ASSERT(retVal==DCM_E_OK,DCM_SVCID_DYNAMICALLYDEFINEDDID);
#else
    (void) Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(&(pMsgContext->reqData[dataOffset]),
                                                            memAddLength,
                                                            memSizeLength,
                                                            &readMemAddress,
                                                            &readMemSize);
#endif

    /* Perform checks to ensure that requested Memory falls within 'configured ranges' */
    retVal = Dcm_Dsp_MemoryServices_ReadMemory_GetRange(&memRangeIndex, &memIdIndex,
        readMemAddress, readMemSize);

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
    CheckSizeResult
      = Dcm_Dsp_DynamicallyDefineDataIdentifier_CheckMaxLengthPeriodicDDDID(pMsgContext,
                                                                         (sizeOfdddid + readMemSize));
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */

    if ((retVal == DCM_E_NOT_OK) || ((sizeOfdddid + readMemSize + 2U) > pMsgContext->resMaxDataLen))
    {
      /* Memory Range Check failed; Set nrc */
      nrc = DCM_E_REQUESTOUTOFRANGE;
    }
#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
    else if (CheckSizeResult != DCM_E_OK)
    {
      /* Memory Range Check failed; Set nrc */
      nrc = DCM_E_REQUESTOUTOFRANGE;
    }
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */
    else
    {
      retVal = Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks(memRangeIndex, memIdIndex, &nrc);

      if(retVal == DCM_E_OK)
      {
        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* Position and size are validated in function Dcm_Dsp_MemoryServices_ReadMemory_GetRange().*/
        DDDidSrcTab[numberOfSrcUsed].memAddrDDDid.srcMemAddr = readMemAddress;
        DDDidSrcTab[numberOfSrcUsed].memAddrDDDid.srcMemSize = readMemSize;
        sizeOfdddid += (uint16)readMemSize;

        /* Add the source memory to the table of sources of the DDDID */
        DDDidSrcTab[numberOfSrcUsed].srcType = DDDIDType;

        numberOfSrcUsed++;

        /* LEAVE critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      }
    }

    if(retVal != DCM_E_OK)
    {
      /* Exit loop if there were some issues. */

      /* Could not add all requested sources. Roll back changes */
      /* Since all searches are stopped as soon as DCM_DDDID_SRC_TYPE_NONE is found for one source
       * no need to go through all, set just one. */

      /* ENTER critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      DDDidSrcTab[numberOfSrcUsed - IndexOfMemorySources].srcType = DCM_DDDID_SRC_TYPE_NONE;

      /* LEAVE critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      break;
    }
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_EXIT(DDDidSrcTab,nrc);

  return nrc;
}
#endif /* (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */

#if (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON)
/* This is the UDS service 0x2C-0x03 handler */
/* !LINKSTO Dcm.Dsn.IB.DDDI.Clear.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierClear_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Dcm_NegativeResponseCodeType nrc = DCM_E_OK;

  TS_PARAM_UNUSED(OpStatus);

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_ENTRY(OpStatus,pMsgContext);

  if (pMsgContext->reqDataLen == 2U)
  {

    Dcm_DidConfigIdxType ConfigIdx = 0;
    /* There is one DDDID given. Delete it. */
    const uint8 HighByte = (uint8)pMsgContext->reqData[0U];
    const uint8 LowByte = (uint8)pMsgContext->reqData[1U];
    const uint16 reqDid = (uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);
    const boolean DIDFound = Dcm_Dsp_DidServices_GetDidIndex(reqDid, &ConfigIdx);

    if ((reqDid >= DCM_DID_DMC_LOW_LIMIT) && (reqDid <= DCM_DID_DMC_UP_LIMIT) && (DIDFound == TRUE))
    {
      Dcm_Dsp_DidServices_DynamicDid_Clear(reqDid);
    }
    else
    {
      /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.ClearDynamicallyDefinedDataIdentifier.InvalidRange,1 */
      /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.ClearDynamicallyDefinedDataIdentifier.NotConfigured,1 */
      nrc = DCM_E_REQUESTOUTOFRANGE;
    }
  }
  else if (pMsgContext->reqDataLen == 0U)
  {
    /* No specific DDDID given. Thereby ALL of them must be cleared, according to the UDS spec. */
    Dcm_Dsp_DidServices_DynamicDid_Clear(DCM_DDDID_CLEAR_ALL);
  }
  else
  {
    /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.IncorrectMessageLength,1 */
    /* Something is wrong (incomplete specification or there are more then one DDDID specified) */
    nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }

  if (nrc != DCM_E_OK)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, nrc);
  }
  else
  {
    /* Get the context information current instance */
    P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
      &DCM_HSM_INFO(Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
          pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex);

    /* This service always returns positive response (if the DDDID isn't out of range
     * or not specified) */
    pMsgContext->resData[0U] = curHsmInfo->subServiceId;
    pMsgContext->resDataLen = 1U;
    /* Only add the DDDID to the response message if it was specified in the request as well */
    if (pMsgContext->reqDataLen == 2U)
    {
      pMsgContext->resData[1U] = pMsgContext->reqData[0U];
      pMsgContext->resData[2U] = pMsgContext->reqData[1U];
      pMsgContext->resDataLen = 3U;
    }

#if (DCM_DDDID_PERSISTENCE == STD_ON)
    /* If DDDID persistence is enabled store the DDDID configuration to Nvm during NvM_WriteAll() */
    (void)NvM_SetRamBlockStatus(DCM_DDDID_NVM_BLOCK_ID, TRUE);
#endif
  }

  DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_EXIT(
    DCM_E_DONE, OpStatus, pMsgContext);

  return DCM_E_DONE;
}
#endif /* (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U))
STATIC FUNC(Std_ReturnType, DCM_CODE)
  Dcm_Dsp_DynamicallyDefineDataIdentifier_CheckMaxLengthPeriodicDDDID(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint32 DDDIDSize)
{
  Std_ReturnType ReturnVal = DCM_E_NOT_OK;

  uint8 HighByte = (uint8)pMsgContext->reqData[0U];
  uint8 LowByte = (uint8)pMsgContext->reqData[1U];

  const uint16 ReqDDDid = DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

  if ((ReqDDDid < 0xF200U) || (ReqDDDid > 0xF2FFU) || (DDDIDSize <= DCM_PERIODIC_DDDID_MAX_LENGTH))
  {
    /* Payload data must not be larger than the configured max length */
    ReturnVal = DCM_E_OK;
  }

  return ReturnVal;
}
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) && (DCM_PERIODIC_DDDID_MAX_LENGTH > 0U)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */

/*==================[end of file]================================================================*/
