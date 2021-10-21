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

/* !LINKSTO Dcm.Dsn.File.RU.Impl,1 */
/* This file contains the implementation of the Diagnostic Service RequestUpload */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_RequestUpload.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x35 handling. */
#if (DCM_DSP_USE_SERVICE_0X35 == STD_ON)

#include <Dcm_Dsp_DataTransferServices.h>
#include <Dcm_Dsp_MemoryServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

#if (defined DCM_RU_DFI_ALFID_LEN)
#error DCM_RU_DFI_ALFID_LEN already defined
#endif
/** \brief The first two bytes of the RequestUpload request are dataFormatIdentifier
 ** and addressAndLengthFormatIdentifier */
#define DCM_RU_DFI_ALFID_LEN 2U

#if (defined DCM_TD_REQID_BSC_LEN)
#error DCM_TD_REQID_BSC_LEN already defined
#endif
/** \brief The first two bytes of the response for a subsequent TransferData request are
 ** request ID and BlockSequenceCounter. */
#define DCM_TD_REQID_BSC_LEN 2U

#if (defined DCM_RU_MINIMUM_REQ_LEN)
#error DCM_RU_MINIMUM_REQ_LEN already defined
#endif
/** \brief The minimum length of a RequestUpload request: dataFormatIdentifier +
 ** addressAndLengthFormatIdentifier + minimum memoryAddress + minimum memorySize */
#define DCM_RU_MINIMUM_REQ_LEN 4U

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x35
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x35
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
/** \brief Calculate block length depending on txbuffer size
 **
 ** \param[in]    MemSize Size of memory to be uploaded
 ** \param[in]    TxbuffSize txbuffer size
 **
 ** \retval BlockLength The size of one block */
STATIC FUNC(uint32, DCM_CODE) Dcm_Dsp_RequestUpload_SetBlockLength(
  uint32 MemSize,
  uint32 TxbuffSize);
#endif

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants declarations]============================================*/

/*===================[internal constants declarations]===========================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RU.SvcH,1 */
/* !LINKSTO Dcm499,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_REQUESTUPLOAD_SVCH_ENTRY(OpStatus, pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_RequestUpload_SvcStart(OpStatus, pMsgContext);
      break;

    /* consecutive call after the function retuned PENDING before */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_RequestUpload_SvcCont(OpStatus, pMsgContext);
      break;

    /* Cancel the ongoing service processing */
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_RequestUpload_SvcCont(OpStatus, pMsgContext);
      break;

    default:
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_REQUESTUPLOAD_SVCH_EXIT(ServiceProcessStatus, OpStatus, pMsgContext);
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RU.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* Holds address field from request */
  uint32 MemAddress = 0U;
  /* Holds memory size field from request */
  uint32 MemSize = 0U;
  /* Holds the expected request length */
  Dcm_MsgLenType ExpectedReqLen = 0U;

  /* holds the number of bytes occupied by memorySize[] in the request*/
  uint8 MemSizeBytes = 0U;
  /* holds the number of bytes occupied by memoryAddress[] in the request*/
  uint8 MemAddBytes = 0U;

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_RxPduIdInfoType RxPduIdInfo;
  uint8 InstIdx;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif /*(DCM_NUM_PROTOCOL > 1U)*/

  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

  /* ISO Sequence Check: Minimum Length Check */
  if (pMsgContext->reqDataLen < DCM_RU_MINIMUM_REQ_LEN)
  {
    /* !LINKSTO Dcm.EB.RequestUpload.MinimumLengthCheck,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    ServiceProcessStatus = DCM_E_DONE;
  }

  /* ISO Sequence Check: Verify that the AddressLengthFormatIdentifier is valid or not */
  /* !LINKSTO Dcm.RequestUpload.AddressAndLengthFormatIdentifier.NotEnabled,2 */
#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
  if  (DCM_E_DONE != ServiceProcessStatus)
  {
    /* !LINKSTO Dcm.RequestUpload.AddressAndLengthFormatIdentifier.Enabled,2 */
    if (DCM_E_OK != Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(pMsgContext->reqData[1U]))
    {
      /* Send Nrc 0x31 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */

  /* ISO Sequence Check: Total length check */
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    /* The data received should be dataFormatIdentifier,addressAndLengthFormatIdentifier ->(2bytes)
     * memoryAddress[], memorySize[] -> (MemAddBytes + MemSizeBytes bytes) */

    MemSizeBytes = (uint8)((pMsgContext->reqData[1U]) >> DCM_NIBBLE_BITS);
    MemAddBytes = (pMsgContext->reqData[1U]) & DCM_MASK_NIBBLE;

    ExpectedReqLen =
      (Dcm_MsgLenType)DCM_RU_DFI_ALFID_LEN + MemAddBytes + MemSizeBytes;

    /* !LINKSTO Dcm.EB.RequestUpload.FullLengthCheck,1 */
    if ((pMsgContext->reqDataLen) != ExpectedReqLen)
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      ServiceProcessStatus = DCM_E_DONE;
    }
  }

  /* Get memoryAddress and memorySize */
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    Std_ReturnType RetGetMemoryAddressAndSize = DCM_E_NOT_OK;

    /* Get the Memory address and the size of memory to be read */
    RetGetMemoryAddressAndSize =
      Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(
        &(pMsgContext->reqData[1U]), MemAddBytes, MemSizeBytes, &MemAddress, &MemSize);

    /* !LINKSTO Dcm.EB.RequestUpload.InvalidAddressAndLengthFormatIdentifier,1 */
    /* !LINKSTO Dcm.EB.RequestUpload.MemorySizeZero,1 */
    if ((DCM_E_NOT_OK == RetGetMemoryAddressAndSize) ||
        (0U == MemSize))
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      ServiceProcessStatus = DCM_E_DONE;
    }
  }

#if (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    Std_ReturnType PermissionsResult = DCM_E_NOT_OK;
    Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

    PermissionsResult = Dcm_Dsp_MemoryServices_RangeAndSecurityCheck(
      MemAddress, MemSize, DCM_READMEMORY, &Nrc);

    if (DCM_E_NOT_OK == PermissionsResult)
    {
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
#endif /*(DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON))*/

  /* ISO Sequence Check: Condition check */
  /* If already registered with an upload or download request */
  if ((DCM_E_DONE != ServiceProcessStatus) &&
      (DataTranContextTypePtr->TransState != DCM_TRANSFER_IDLE))
  {
    /* !LINKSTO Dcm.EB.RequestUpload.DownloadInProgress,1 */
    /* !LINKSTO Dcm.EB.RequestUpload.UploadInProgress,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    ServiceProcessStatus = DCM_E_DONE;
  }

#if (DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK == STD_ON)
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    Std_ReturnType PermissionsResult = DCM_E_NOT_OK;

    const uint8 ReqMemIdValue = DCM_BYTE_FROM_QWORD(MemAddress, MemAddBytes);

    /* This check verifies that the requested MemoryId is configured for a DcmDspMemoryIdInfo
     * container, as described in SWS_Dcm_01055. It doesn't check however that the requested
     * MemoryId is exactly the same as the MemoryId that is configured for the identified
     * requested memory range.
     * This offers both AUTOSAR compliance and flexibility.
     * If the configuration parameter DcmDataTransferServicesMemoryIdCheck is enabled and
     * a match between the requested MemoryId and the MemoryId of the requested memory range
     * is desired, then the MemoryId of a DcmDspMemoryIdInfo should be configured to be equal
     * to the most significant byte of the starting memory address. */

    PermissionsResult = Dcm_Dsp_MemoryServices_MemoryIdCheck(ReqMemIdValue);

    if (DCM_E_NOT_OK == PermissionsResult)
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
#endif /*(DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK == STD_ON)*/

  if (DCM_E_DONE != ServiceProcessStatus)
  {
    /* hold format Identifier from the request.
     * pMsgContext->reqData might be overwritten in case a common buffer is used for Rx/Tx */
    DataTranContextTypePtr->RequestUploadFormatId = pMsgContext->reqData[0U];

    /* Storing the memory address to download in the transfer request */
    DataTranContextTypePtr->MemAddres = MemAddress;
    /* Storing the size of memory to download in the transfer request */
    DataTranContextTypePtr->MemSize = MemSize;

    ServiceProcessStatus = Dcm_Dsp_RequestUpload_SvcCont(OpStatus, pMsgContext);
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.RU.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  Std_ReturnType ReqUploadResult = DCM_E_NOT_OK;
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  Dcm_RxPduIdInfoType RxPduIdInfo;
  uint8 InstIdx;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif /*(DCM_NUM_PROTOCOL > 1U)*/

  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
  if (OpStatus == DCM_INITIAL)
  {
    ReqUploadResult = Dcm_ProcessRequestUpload(OpStatus,
                                               DataTranContextTypePtr->RequestUploadFormatId,
                                               DataTranContextTypePtr->MemAddres,
                                               DataTranContextTypePtr->MemSize,
                                               &Nrc);
  }
  else
  {
    ReqUploadResult = Dcm_ProcessRequestUpload(OpStatus, 0U, 0U, 0U, &Nrc);
  }
#else
  ReqUploadResult = Dcm_ProcessRequestUpload(OpStatus,
                                             DataTranContextTypePtr->RequestUploadFormatId,
                                             DataTranContextTypePtr->MemAddres,
                                             DataTranContextTypePtr->MemSize,
                                             &(DataTranContextTypePtr->BlockLength),
                                             &Nrc);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

  if (OpStatus != DCM_CANCEL)
  {
    switch (ReqUploadResult)
    {
      case E_OK:
      {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
        /* Use size of Tx buffer which is resMaxDataLen + 1 */
        uint32 TxbuffSize = pMsgContext->resMaxDataLen + 1U;

        uint32 BlockLength =
          Dcm_Dsp_RequestUpload_SetBlockLength(DataTranContextTypePtr->MemSize, TxbuffSize);
#else
        /* Use block length returned by Dcm_ProcessRequestUpload */
        /* !LINKSTO Dcm.EB.RequestUpload.maxNumberOfBlockLength.AUTOSAR430,1 */
        uint32 BlockLength = DataTranContextTypePtr->BlockLength;
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

        /* Response data contains 'lengthFormatIdentifier' .Its highest nibble contains
           the number of bytes in the following field (maxNumberOfBlockLength).Lowest
           nibble is zero */
        uint8_least BlockBytes = DCM_MAX_ADDRESS_BYTES;

        pMsgContext->resData[0U] = (uint8)(BlockBytes << DCM_NIBBLE_BITS);
        pMsgContext->resDataLen++;
        /* Assembling maxNumberOfBlockLength[] in the response. */
        while (BlockBytes > 0U)
        {
          pMsgContext->resData[BlockBytes] = (uint8)(BlockLength & DCM_MASK_BYTE);
          pMsgContext->resDataLen++;
          BlockLength = (uint32)BlockLength >> DCM_BYTE_BITS;
          BlockBytes--;
        }

        /* Registering Upload Request */
        DataTranContextTypePtr->TransState = DCM_TRANSFER_UPLOAD;
        /* The blockSequenceCounter shall be initialized to one, when receiving a RequestUpload */
        DataTranContextTypePtr->BlockSequenceCounter = 1U;
        /* Setting the total data transferred to zero */
        DataTranContextTypePtr->MemTransfered = 0U;
        ServiceProcessStatus = DCM_E_DONE;
        break;
      }

      case E_NOT_OK:
        /* !LINKSTO Dcm758,1 */
        /* !LINKSTO SWS_Dcm_01133,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
        ServiceProcessStatus = DCM_E_DONE;
        break;

      case DCM_E_PENDING:
        ServiceProcessStatus = DCM_E_PENDING;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.RequestUpload.WrongInterfaceReturn.DetError,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_REQUESTUPLOAD, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        /* !LINKSTO Dcm.EB.RequestUpload.WrongInterfaceReturn.NRC0x10,1 */
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

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
STATIC FUNC(uint32, DCM_CODE) Dcm_Dsp_RequestUpload_SetBlockLength(
  uint32 MemSize,
  uint32 TxbuffSize)
{
  uint32 BlockLength;

  if ((MemSize + DCM_TD_REQID_BSC_LEN) < TxbuffSize)
  {
    /* !LINKSTO Dcm.EB.RequestUpload.maxNumberOfBlockLength.NotFitInTxBuffer.AUTOSAR403,1 */
    BlockLength = MemSize + DCM_TD_REQID_BSC_LEN;
  }
  else
  {
    /* !LINKSTO Dcm.EB.RequestUpload.maxNumberOfBlockLength.FitInTxBuffer.AUTOSAR403,1 */
    BlockLength = TxbuffSize;
  }

  return BlockLength;
}
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X35 == STD_ON) */

/*==================[end of file]================================================================*/
