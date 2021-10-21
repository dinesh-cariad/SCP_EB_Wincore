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

/* !LINKSTO Dcm.Dsn.File.TD.Impl,1 */
/* This file contains the implementation of the Diagnostic Service TransferData */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_TransferData.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x36 handling. */
#if (DCM_DSP_USE_SERVICE_0X36 == STD_ON)

#include <Dcm_Dsp_DataTransferServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

#if (defined DCM_TD_BLOCKSEQCOUNTER_MAX)
#error DCM_TD_BLOCKSEQCOUNTER_MAX already defined
#endif
/** \brief The maximum value allowed for blocksequencecounter until it rolls back to 0 */
#define DCM_TD_BLOCKSEQCOUNTER_MAX 0xFFU


#if (defined DCM_REQ_MIN_MANDATORY_TRANSFER_DATA_LEN)
#error DCM_REQ_MIN_MANDATORY_TRANSFER_DATA_LEN already defined
#endif
/** \brief The minimum lenght of the message (without considering SID parameter) required to be
 ** accepted by transfer data, on all accepted conditions must be 1 according to ISO14229-1 2013
 ** (RequestDownload(0x34), RequestUpload(0x35), RequestFileTransfer(0x38)) */
#define DCM_REQ_MIN_MANDATORY_TRANSFER_DATA_LEN 0x01U

#if (defined DCM_TD_REQ_UPLOAD_LEN)
#error DCM_TD_REQ_UPLOAD_LEN already defined
#endif
/** \brief The maximum length of a TransferData request for upload. According do ISO14229-1 2013
 ** size of transferRequestParameterRecord[] in the request shall be 0 in case of data upload */
#define DCM_TD_REQ_UPLOAD_LEN 0x01U

#if (defined DCM_TD_REQ_DOWNLOAD_MIN_LEN)
#error DCM_TD_REQ_DOWNLOAD_MIN_LEN already defined
#endif
/** \brief The maximum length of a TransferData request for download */
#define DCM_TD_REQ_DOWNLOAD_MIN_LEN 0x02U

#if (defined DCM_TD_REQ_UPLOAD_SID_BSC_LEN)
#error DCM_TD_REQ_UPLOAD_SID_BSC_LEN already defined
#endif
/** \brief The size representing 2 bytes: 1 byte SID + 1 byte blocksequencecounter */
#define DCM_TD_REQ_UPLOAD_SID_BSC_LEN 0x02U

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x36
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x36
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief For handling Download operation in the service 0x36
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[inout] TranContextPtr Context information for transfer service.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_Download(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) TranContextPtr,
  Dcm_OpStatusType OpStatus);

/** \brief For handling Upload operation in the service 0x36
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[inout] TranContextPtr Context information for transfer service.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_Upload(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) TranContextPtr,
  Dcm_OpStatusType OpStatus);

/** \brief Does minimum request length check for service 0x36
 **
 ** \param[in]  ReqDataLen Length of request
 **
 ** \retval Std_ReturnType */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_RequestLengthCheck(
  Dcm_MsgLenType ReqDataLen,
  Dcm_DataTransferStateType TransferState);


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants declarations]============================================*/

/*===================[internal constants declarations]===========================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.TD.SvcH,1 */
/* !LINKSTO Dcm502,1 */
/* This is the generic service handler for UDS service 0x36 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_TRANSFERDATA_SVCH_ENTRY(OpStatus, pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_TransferData_SvcStart(OpStatus, pMsgContext);
      break;

    /* Sub-sequent call to the service function after FORCE_RCRRP */
    /* !LINKSTO Dcm.EB.TransferData.ReadMemory.CallRCRRP,1 */
    /* !LINKSTO Dcm.EB.TransferData.WriteMemory.CallRCRRP,1 */
    case DCM_FORCE_RCRRP_OK:
    /* second call when PENDING is returned */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_TransferData_SvcCont(OpStatus, pMsgContext);
      break;

    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_TransferData_SvcCont(OpStatus, pMsgContext);
      break;

    default:
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_TRANSFERDATA_SVCH_EXIT(ServiceProcessStatus, OpStatus, pMsgContext);
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.TD.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

  Std_ReturnType RequestLengthCheck = DCM_E_NOT_OK;

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_RxPduIdInfoType RxPduIdInfo;
  uint8 InstIdx;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif /*(DCM_NUM_PROTOCOL > 1U)*/

  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

  RequestLengthCheck = Dcm_Dsp_TransferData_RequestLengthCheck(
    pMsgContext->reqDataLen, DataTranContextTypePtr->TransState);

  if (RequestLengthCheck == DCM_E_NOT_OK)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    ServiceProcessStatus = DCM_E_DONE;
  }
  else
  {
    uint8 BlockSeqCounter = pMsgContext->reqData[0U];
    boolean SequenceCheckFailed = FALSE;

    /* If an upload or download request is not registered */
    if (DataTranContextTypePtr->TransState == DCM_TRANSFER_IDLE)
    {
      SequenceCheckFailed = TRUE;
    }

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
    /* If an upload or download request is registered but the data determined by the memorySize
     * parameter from the request 0x34/0x35 is already transferred issue nrc 0x31 */
    /* In ASR over 4.2.2, the check is handled by the callout */
    /* !LINKSTO Dcm.EB.TransferData.DataReceived.NRC0x24.AUTOSAR403,1 */
    if (
        (DataTranContextTypePtr->MemTransfered == DataTranContextTypePtr->MemSize) &&
        (BlockSeqCounter == DataTranContextTypePtr->BlockSequenceCounter)
       )
    {
      SequenceCheckFailed = TRUE;
    }
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

    if (SequenceCheckFailed == TRUE)
    {
      /* !LINKSTO Dcm.EB.TransferData.DownloadUploadNotActive,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTSEQUENCEERROR);
      ServiceProcessStatus = DCM_E_DONE;
    }
    /* For the first transferRequest the block sequence counter should be one */
    /* !LINKSTO Dcm.EB.TransferData.Download.StartBlockSequenceCounter,1 */
    /* !LINKSTO Dcm.EB.TransferData.Upload.StartBlockSequenceCounter,1 */
    else if (((DataTranContextTypePtr->MemTransfered == 0U) && (BlockSeqCounter != 1U)) ||
             /* The request with repeated block sequence count is also supported.
                So block sequence counter shall be equal to the internal count or
                equal to the previous value of internal count */
             ((DataTranContextTypePtr->BlockSequenceCounter > 0U) &&
              (BlockSeqCounter != DataTranContextTypePtr->BlockSequenceCounter) &&
              (BlockSeqCounter != (DataTranContextTypePtr->BlockSequenceCounter - 1U))) ||
             ((DataTranContextTypePtr->BlockSequenceCounter == 0U) &&
              (BlockSeqCounter != DCM_TD_BLOCKSEQCOUNTER_MAX) && (BlockSeqCounter != 0U)))
    {
      /* !LINKSTO Dcm.EB.TransferData.Download.WrongBlockSequenceCounter,1 */
      /* !LINKSTO Dcm.EB.TransferData.Upload.WrongBlockSequenceCounter,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_WRONGBLOCKSEQUENCECOUNTER);
      ServiceProcessStatus = DCM_E_DONE;
    }
    else
    {
      ServiceProcessStatus = Dcm_Dsp_TransferData_SvcCont(OpStatus, pMsgContext);
    }
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.TD.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
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

  if (DataTranContextTypePtr->TransState == DCM_TRANSFER_DOWNLOAD)
  {
    ServiceProcessStatus = Dcm_Dsp_TransferData_Download(
                             pMsgContext, DataTranContextTypePtr, OpStatus);
  }
  else
  {
    ServiceProcessStatus = Dcm_Dsp_TransferData_Upload(
                             pMsgContext, DataTranContextTypePtr, OpStatus);
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.TD.SvcDownload,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_Download(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) TranContextPtr,
  Dcm_OpStatusType OpStatus)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_ReturnWriteMemoryType WriteMemoryResult = DCM_WRITE_FAILED;

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
  Dcm_NegativeResponseCodeType ErrorCode;
#endif

  if (OpStatus == DCM_INITIAL)
  {
    /* The first data will be an echo of block sequence counter value received.*/
    pMsgContext->resData[0U] = pMsgContext->reqData[0U];
    pMsgContext->resDataLen++;
    /* Updating the data to be transferred in the current request. Needs to keep this to
       update 'MemTransfered' after data transferred*/
    TranContextPtr->CurrMemTransfered = ((uint32)pMsgContext->reqDataLen - 1U);

    /*Call write memory only if block sequence count received is equal to the internal
      BlockSequenceCounter. If the request is having block sequence count of previous,
      send positive response without writing to memory.*/
    if (pMsgContext->reqData[0U] == TranContextPtr->BlockSequenceCounter)
    {
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
      /* If the sum of data to be transferred exceeds the memorySize
       * parameter from the requestDownload (0x34) */
      if ((TranContextPtr->CurrMemTransfered +
           TranContextPtr->MemTransfered) > TranContextPtr->MemSize)
      {
       /* !LINKSTO Dcm.EB.TransferData.Download.TotalRequestedMemorySizeExceeded,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        ServiceProcessStatus = DCM_E_DONE;
      }
#else
      /* If the data to be transferred in the current step exceeds BlockLength - 2
       * reported from the requestDownload (0x34) */
      if ((TranContextPtr->CurrMemTransfered) > (TranContextPtr->BlockLength - 2U))
      {
        /* !LINKSTO Dcm.EB.TransferData.Download.InconsistentTransferRequestParameterRecord.0x31,2 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        ServiceProcessStatus = DCM_E_DONE;
      }
#endif /*(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)*/
      else
      {
        /* !LINKSTO Dcm503,1 */
        WriteMemoryResult = Dcm_WriteMemory(
          OpStatus,
          0U,
          TranContextPtr->MemAddres + TranContextPtr->MemTransfered,
          TranContextPtr->CurrMemTransfered,
          &(pMsgContext->reqData[1U])
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
          ,&ErrorCode
#endif
          );
      }
    }
    else
    {
      /* !LINKSTO Dcm.EB.TransferData.Download.RepeatedBlockSequenceCounter,1 */
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
    WriteMemoryResult =
      Dcm_WriteMemory(OpStatus, 0U, 0U, 0U, &(pMsgContext->reqData[1U]));
#else
    WriteMemoryResult = Dcm_WriteMemory(
      OpStatus,
      0U,
      TranContextPtr->MemAddres + TranContextPtr->MemTransfered,
      TranContextPtr->CurrMemTransfered,
      &(pMsgContext->reqData[1U]),
      &ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
  }

  if ((OpStatus != DCM_CANCEL) && (ServiceProcessStatus != DCM_E_DONE))
  {
    switch (WriteMemoryResult)
    {
      case DCM_WRITE_OK:
        /* Data download successful. So update internal record of Data written */
        TranContextPtr->MemTransfered += TranContextPtr->CurrMemTransfered;

        /* Data download successful.Increment block sequence counter to receive the next message */

         if (TranContextPtr->BlockSequenceCounter < DCM_TD_BLOCKSEQCOUNTER_MAX)
         {
           /* !LINKSTO Dcm.EB.TransferData.Download.IncrementBlockSequenceCounter,2 */
           TranContextPtr->BlockSequenceCounter++;
         }
         else
         {
           /* !LINKSTO Dcm.EB.TransferData.ResetBlockSequenceCounter,2 */
           TranContextPtr->BlockSequenceCounter = 0U;
         }

        /* !LINKSTO Dcm.EB.TransferData.Write.ReturnOK.NotNRC0x00.ASR430,1 */
        /* Here if the nrc returned by the callout is different than 0x00 it's ignored */

        ServiceProcessStatus = DCM_E_DONE;
        break;

      case DCM_WRITE_PENDING:
        ServiceProcessStatus = DCM_E_PENDING;
        break;

      case DCM_WRITE_FORCE_RCRRP:
        /* !LINKSTO Dcm.EB.TransferData.DCM_WRITE_FORCE_RCRRP,1 */
        ServiceProcessStatus = DCM_E_FORCE_RCRRP;
        break;

      case DCM_WRITE_FAILED:
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
        /* !LINKSTO Dcm.EB.TransferData.WriteFailed.AUTOSAR403,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALPROGRAMMINGFAILURE);
#else

        /* !LINKSTO Dcm.EB.TransferData.Write.ReturnNOK.NRC0x00.ASR430,1 */
        if (ErrorCode == 0x00U)
        {
          ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        }

        /* !LINKSTO SWS_Dcm_01173,1 */
        /* !LINKSTO Dcm.EB.TransferData.WriteFailed.AUTOSAR430,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
        ServiceProcessStatus = DCM_E_DONE;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.TransferData.Download.WrongInterfaceReturn.DetError,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        /* !LINKSTO Dcm.EB.TransferData.Download.WrongInterfaceReturn.NRC0x10,1 */
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

/* !LINKSTO Dcm.Dsn.IB.TD.SvcUpload,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_Upload(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) TranContextPtr,
  Dcm_OpStatusType OpStatus)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_ReturnReadMemoryType ReadMemoryResult = DCM_READ_FAILED;

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
  Dcm_NegativeResponseCodeType ErrorCode;
#endif

  if (OpStatus == DCM_INITIAL)
  {
    /*size of Tx buffer*/
    uint32 TxbuffSize;

    TxbuffSize = pMsgContext->resMaxDataLen + 1U;

    /* The first data will be an echo of block sequence counter value received.*/
    pMsgContext->resData[0U] = pMsgContext->reqData[0U];
    pMsgContext->resDataLen++;
    /*If request is with the previous block sequence count*/
    /* !LINKSTO Dcm.EB.TransferData.Upload.RepeatedBlockSequenceCounter,1 */
    if (pMsgContext->reqData[0U] != TranContextPtr->BlockSequenceCounter)
    {
      TranContextPtr->BlockSequenceCounter = pMsgContext->reqData[0U];
      /*Subtracting the memory transferred in the previous request to the total transferred.
        Since the same is expected in the current request */
      TranContextPtr->MemTransfered -= TranContextPtr->CurrMemTransfered;
    }
    /* Updating the data to be transferred in the current request. Needs to keep this to
    update 'MemTransfered' after data transferred*/

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
    /* !LINKSTO Dcm.EB.TransferData.maxNumberOfBlockLength.NotFitInTxBuffer.NotAUTOSAR403,1 */
    /* !LINKSTO Dcm.EB.TransferData.maxNumberOfBlockLength.FitInTxBuffer.NotAUTOSAR403,1 */
    if(TxbuffSize >= TranContextPtr->BlockLength)
    {
      if((TranContextPtr->BlockLength - DCM_TD_REQ_UPLOAD_SID_BSC_LEN) <= (TranContextPtr->MemSize - TranContextPtr->MemTransfered))
      {
        TranContextPtr->CurrMemTransfered = TranContextPtr->BlockLength - DCM_TD_REQ_UPLOAD_SID_BSC_LEN;
      }
      else
      {
        TranContextPtr->CurrMemTransfered = (TranContextPtr->MemSize - TranContextPtr->MemTransfered);
      }
    }
    else
#endif
    {
      if((TxbuffSize - DCM_TD_REQ_UPLOAD_SID_BSC_LEN) <= (TranContextPtr->MemSize - TranContextPtr->MemTransfered))
      {
        TranContextPtr->CurrMemTransfered = TxbuffSize - DCM_TD_REQ_UPLOAD_SID_BSC_LEN;
      }
      else
      {
        TranContextPtr->CurrMemTransfered = (TranContextPtr->MemSize - TranContextPtr->MemTransfered);
      }
    }

    /* !LINKSTO Dcm504,1 */
    ReadMemoryResult = Dcm_ReadMemory(OpStatus,
                                      0U,
                                      TranContextPtr->MemAddres + TranContextPtr->MemTransfered,
                                      TranContextPtr->CurrMemTransfered,
                                      &(pMsgContext->resData[1U])
    #if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
        (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
                                      ,&ErrorCode
    #endif
                                      );
  }
  else
  {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
    ReadMemoryResult =
      Dcm_ReadMemory(OpStatus, 0U, 0U, 0U, &(pMsgContext->resData[1U]));
#else
    ReadMemoryResult = Dcm_ReadMemory(
      OpStatus,
      0U,
      TranContextPtr->MemAddres + TranContextPtr->MemTransfered,
      TranContextPtr->CurrMemTransfered,
      &(pMsgContext->resData[1U]),
      &ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
  }

  if ((OpStatus != DCM_CANCEL) && (ServiceProcessStatus != DCM_E_DONE))
  {
    switch (ReadMemoryResult)
    {
      case DCM_READ_OK:
        /*Data uploaded successfully. So update internal record of Data uploaded*/
        TranContextPtr->MemTransfered += TranContextPtr->CurrMemTransfered;

        /*Data uploaded successfully.Increment block sequence
        counter to receive the next message*/

        if (TranContextPtr->BlockSequenceCounter < DCM_TD_BLOCKSEQCOUNTER_MAX)
        {
          /* !LINKSTO Dcm.EB.TransferData.Upload.IncrementBlockSequenceCounter,2 */
          TranContextPtr->BlockSequenceCounter++;
        }
        else
        {
          /* !LINKSTO Dcm.EB.TransferData.ResetBlockSequenceCounter,2 */
          TranContextPtr->BlockSequenceCounter = 0U;
        }

        /*Updating the response data filled*/
        pMsgContext->resDataLen += (Dcm_MsgLenType)TranContextPtr->CurrMemTransfered;

        /* !LINKSTO Dcm.EB.TransferData.Read.ReturnOK.NotNRC0x00.ASR430,1 */
        /* Here if the nrc returned by the callout is different than 0x00 it's ignored */

        ServiceProcessStatus = DCM_E_DONE;
        break;

      case DCM_READ_PENDING:
        ServiceProcessStatus = DCM_E_PENDING;
        break;

      case DCM_READ_FORCE_RCRRP:
        /* !LINKSTO Dcm.EB.TransferData.DCM_READ_FORCE_RCRRP,1 */
        ServiceProcessStatus = DCM_E_FORCE_RCRRP;
        break;

      case DCM_READ_FAILED:
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
       /* !LINKSTO Dcm.EB.TransferData.ReadFailed.AUTOSAR403,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
#else
        /* !LINKSTO Dcm.EB.TransferData.Read.ReturnNOK.NRC0x00.ASR430,1 */
        if (ErrorCode == 0x00U)
        {
          ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        }

        /* !LINKSTO Dcm.EB.TransferData.ReadFailed.AUTOSAR430,1 */
        /* !LINKSTO SWS_Dcm_01173,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);

#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

        ServiceProcessStatus = DCM_E_DONE;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.TransferData.Upload.WrongInterfaceReturn.DetError,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        /* !LINKSTO Dcm.EB.TransferData.Upload.WrongInterfaceReturn.NRC0x10,1 */
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

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_RequestLengthCheck(
  Dcm_MsgLenType ReqDataLen,
  Dcm_DataTransferStateType TransferState)
{
  Std_ReturnType LengthCheckResult = DCM_E_NOT_OK;

  if (TransferState == DCM_TRANSFER_DOWNLOAD)
  {
    /* !LINKSTO Dcm.EB.TransferData.Download.MinimumRequestLength,1 */
    if (ReqDataLen >= DCM_TD_REQ_DOWNLOAD_MIN_LEN)
    {
      LengthCheckResult = DCM_E_OK;
    }
  }
  else if (TransferState == DCM_TRANSFER_UPLOAD)
  {
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2013)
    /* !LINKSTO Dcm.EB.TransferData.Upload.RequestLength.ISO2013,1 */
    if (ReqDataLen == DCM_TD_REQ_UPLOAD_LEN)
#else
    /* !LINKSTO Dcm.EB.TransferData.Upload.RequestLength.ISO2006,1 */
    if (ReqDataLen >= DCM_TD_REQ_UPLOAD_LEN)
#endif /*DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2013*/
    {
      LengthCheckResult = DCM_E_OK;
    }
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
    DCM_TRANSFER_IDLE == TransferState, DCM_INTERNAL_API_ID);

    /* In order to respect the sequence order specified in ISO14229-1:2013, in case there is no
       download or upload request in progress, the request length must be considered OK if it is
       at least 1, since the check will fail the sequence check. The request length is in all cases
       at least 1 since it contains at least the SI. */

    /* !LINKSTO Dcm.EB.TransferData.Mandatory.MinimumRequestLength,1 */
    if (ReqDataLen >= DCM_REQ_MIN_MANDATORY_TRANSFER_DATA_LEN)
    {
      LengthCheckResult = DCM_E_OK;
    }
  }

  return LengthCheckResult;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X36 == STD_ON) */

/*==================[end of file]================================================================*/
