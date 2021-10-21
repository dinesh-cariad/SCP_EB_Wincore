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

/* !LINKSTO Dcm.Dsn.File.RTE.Impl,1 */
/* This file contains the implementation of the Diagnostic Service RequestTransferExit */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_RequestTransferExit.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x37 handling. */
#if (DCM_DSP_USE_SERVICE_0X37 == STD_ON)

#include <Dcm_Dsp_DataTransferServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x37
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x37 internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants declarations]============================================*/

/*===================[internal constants declarations]===========================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RTE.SvcH,1 */
/* !LINKSTO Dcm505,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_ENTRY(OpStatus, pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_RequestTransferExit_SvcStart(OpStatus, pMsgContext);
      break;

    /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_RequestTransferExit_SvcCont(OpStatus, pMsgContext);
      break;

    /* Cancel the ongoing service processing */
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_RequestTransferExit_SvcCont(OpStatus, pMsgContext);
      break;

    default:
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_EXIT(ServiceProcessStatus, OpStatus, pMsgContext);
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RTE.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
  )
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_RxPduIdInfoType RxPduIdInfo;
  uint8 InstIdx;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif /*(DCM_NUM_PROTOCOL > 1U)*/

  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

  /* If an upload or download request is not active */
  if (DataTranContextTypePtr->TransState == DCM_TRANSFER_IDLE)
  {
    /* !LINKSTO Dcm.EB.RequestTransferExit.DownloadUploadNotActive,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTSEQUENCEERROR);
    ServiceProcessStatus = DCM_E_DONE;
  }
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
  /* Only the service id is expected in the request. The other parameters are not supported */
  /* !LINKSTO Dcm.EB.RequestTransferExit.WrongReqLength.AUTOSAR403,1 */
  else if ((pMsgContext->reqDataLen) != 0U)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    ServiceProcessStatus = DCM_E_DONE;
  }
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
  else
  {
    ServiceProcessStatus = Dcm_Dsp_RequestTransferExit_SvcCont(OpStatus, pMsgContext);
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.RTE.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
  )
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  Std_ReturnType TransferExitResult = DCM_E_NOT_OK;

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_RxPduIdInfoType RxPduIdInfo;
  uint8 InstIdx;
#endif /*(DCM_NUM_PROTOCOL > 1U)*/

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)

  /* If ASR 4.2.2 or ASR 4.3.0 is used (meaning all callout parameters are mandatory),
   * the parameters need to be calculated from the request (SID is extracted already).
   * The request according to ISO 14229-1 2013 looks like this:
   * #1 RequestTransferExit Request SID
   * #2...#n TransferRequestParameterRecord[] */

  P2VAR(uint8, AUTOMATIC, DCM_VAR) TransferResponseParameterRecord = NULL_PTR;
  uint32 TransferResponseParameterRecordSize = 0U;

  /* According to SWS_Dcm_01395, if TransferRequestParameterRecordSize is 0,
   * TransferRequestParameterRecord is undefined.
   * if TransferRequestParameterRecordSize is 0 a NULL_PTR will be used
   * as TransferRequestParameterRecord in the callout */

  uint32 TransferRequestParameterRecordSize = pMsgContext->reqDataLen;
  P2VAR(uint8, AUTOMATIC, DCM_VAR) TransferRequestParameterRecord;

  if (TransferRequestParameterRecordSize > 0U)
  {
    TransferRequestParameterRecord = pMsgContext->reqData;
  }
  else
  {
    /* !LINKSTO Dcm.EB.RequestTransferExit.transferRequestParameterRecordSizeZero,1 */
    /* !LINKSTO Dcm.EB.RequestTransferExit.transferRequestParameterRecordSizeZero.AUTOSAR422,1 */
    TransferRequestParameterRecord = NULL_PTR;
  }

#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
         (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)*/

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif

  /* holds the context information transfer services */
  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
  /* !LINKSTO Dcm.EB.RequestTransferExit.OptionalParametersNotUsed.ASR403,1 */
  TransferExitResult = Dcm_ProcessRequestTransferExit(OpStatus, NULL_PTR, 0U, &Nrc);
#else

  /* Give the callout the max size of the TxBuffer. The callout will return the actual
   * number of written data bytes in TransferResponseParameterRecord */
  TransferResponseParameterRecordSize = pMsgContext->resMaxDataLen + 1U;

  /* Give the address of response data pointer */
  TransferResponseParameterRecord = pMsgContext->resData;

  TransferExitResult = Dcm_ProcessRequestTransferExit(
    OpStatus,
    TransferRequestParameterRecord,
    TransferRequestParameterRecordSize,
    TransferResponseParameterRecord,
    &TransferResponseParameterRecordSize,
    &Nrc);

#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

  if (OpStatus != DCM_CANCEL)
  {
    switch (TransferExitResult)
    {
      case E_OK:
        /*Registering Transfer Exit Request*/
        DataTranContextTypePtr->TransState = DCM_TRANSFER_IDLE;

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
        /* Set response length. If TransferResponseParameterRecordSize is 0,
         * than only positive response id will be transmitted */
        /* !LINKSTO Dcm.EB.RequestTransferExit.transferResponseParameterRecordSizeZero,1 */
        /* !LINKSTO Dcm.EB.RequestTransferExit.transferResponseParameterRecordSizeZero.AUTOSAR422,1 */
        pMsgContext->resDataLen += TransferResponseParameterRecordSize;
#endif

        ServiceProcessStatus = DCM_E_DONE;
        break;

      case E_NOT_OK:
        /* !LINKSTO Dcm759,1 */
        /* !LINKSTO Dcm.EB.RequestTransferExit.NRC.AUTOSAR430,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
        ServiceProcessStatus = DCM_E_DONE;
        break;

      case DCM_E_PENDING:
        ServiceProcessStatus = DCM_E_PENDING;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.RequestTransferExit.WrongInterfaceReturn.DetError,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_REQUESTTRANSFEREXIT, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        /* !LINKSTO Dcm.EB.RequestTransferExit.WrongInterfaceReturn.NRC0x10,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
        ServiceProcessStatus = DCM_E_DONE;
        break;
    }
  }
  else
  {
    ServiceProcessStatus = DCM_E_DONE;
  }
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X37 == STD_ON) */

/*==================[end of file]================================================================*/
