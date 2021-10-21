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

/* !LINKSTO Dcm.Dsn.File.RD.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service RequestDownload. */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_RequestDownload.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x34 handling. */
#if (DCM_DSP_USE_SERVICE_0X34 == STD_ON)

#include <Dcm_Dsp_DataTransferServices.h>
#include <Dcm_Dsp_MemoryServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

#if (defined DCM_RD_DFI_ALFID_LEN)
#error DCM_RD_DFI_ALFID_LEN already defined
#endif
/** \brief The first two bytes of the RequestDownload request are dataFormatIdentifier
 ** and addressAndLengthFormatIdentifier */
#define DCM_RD_DFI_ALFID_LEN 2U

#if (defined DCM_RD_MINIMUM_REQ_LEN)
#error DCM_RD_MINIMUM_REQ_LEN already defined
#endif
/** \brief The minimum length of a RequestDownload request: dataFormatIdentifier +
 ** addressAndLengthFormatIdentifier + minimum memoryAddress + minimum memorySize */
#define DCM_RD_MINIMUM_REQ_LEN 4U

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x34
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x34
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcCont(
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

/* !LINKSTO Dcm.Dsn.IB.RD.SvcH,1 */
/* !LINKSTO Dcm496,1 */
/* This is the generic service handler for UDS service 0x34 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_ENTRY(OpStatus, pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_RequestDownload_SvcStart(OpStatus, pMsgContext);
      break;

    /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_RequestDownload_SvcCont(OpStatus, pMsgContext);
      break;

    /* Cancel the ongoing service processing */
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_RequestDownload_SvcCont(OpStatus, pMsgContext);
      break;

    default:
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_EXIT(ServiceProcessStatus, OpStatus, pMsgContext);
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RD.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcStart(
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

  /* holds the number of bytes occupied by memorySize[] in the request */
  uint8 MemSizeBytes = 0U;
  /* holds the number of bytes occupied by memoryAddress[] in the request */
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
  if (pMsgContext->reqDataLen < DCM_RD_MINIMUM_REQ_LEN)
  {
    /* !LINKSTO Dcm.EB.RequestDownload.MinimumLengthCheck,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    ServiceProcessStatus = DCM_E_DONE;
  }

  /* ISO Sequence Check: Verify that the AddressLengthFormatIdentifier is valid or not */
  /* !LINKSTO Dcm.RequestDownload.AddressAndLengthFormatIdentifier.NotEnabled,2 */
#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    /* !LINKSTO Dcm.RequestDownload.AddressAndLengthFormatIdentifier.Enabled,2 */
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
    /* The data received shall be : dataFormatIdentifier (1 byte) + addressAndLengthFormatIdentifier (1bytes) +
       number of memoryAddress bytes (MemAddBytes) + number of memorySize bytes (MemSizeBytes bytes) */

    MemSizeBytes = (uint8)((pMsgContext->reqData[1U]) >> DCM_NIBBLE_BITS);
    MemAddBytes = (pMsgContext->reqData[1U]) & DCM_MASK_NIBBLE;

    ExpectedReqLen =
      (Dcm_MsgLenType)DCM_RD_DFI_ALFID_LEN + MemAddBytes + MemSizeBytes;

    /* !LINKSTO Dcm.EB.RequestDownload.FullLengthCheck,1 */
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

    /* !LINKSTO Dcm.EB.RequestDownload.InvalidAddressAndLengthFormatIdentifier,1 */
    /* !LINKSTO Dcm.EB.RequestDownload.MemorySizeZero,1 */
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
      MemAddress, MemSize, DCM_WRITEMEMORY, &Nrc);

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
    /* !LINKSTO Dcm.EB.RequestDownload.DownloadInProgress,1 */
    /* !LINKSTO Dcm.EB.RequestDownload.UploadInProgress,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    ServiceProcessStatus = DCM_E_DONE;
  }

#if (DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK == STD_ON)
  if (DCM_E_DONE != ServiceProcessStatus)
  {
    Std_ReturnType PermissionsResult = DCM_E_NOT_OK;

    const uint8 ReqMemIdValue = DCM_BYTE_FROM_QWORD(MemAddress, MemAddBytes);

    /* This check verifies that the requested MemoryId is configured for a DcmDspMemoryIdInfo
     * container, as described in SWS_Dcm_01057. It doesn't check however that the requested
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
    DataTranContextTypePtr->RequestDownloadFormatId = pMsgContext->reqData[0U];

    /* Storing the memory address to download in the transfer request */
    DataTranContextTypePtr->MemAddres = MemAddress;
    /* Storing the size of memory to download in the transfer request */
    DataTranContextTypePtr->MemSize = MemSize;

    ServiceProcessStatus = Dcm_Dsp_RequestDownload_SvcCont(OpStatus, pMsgContext);
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.RD.SvcContinue,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{

  /* holds the context information transfer services */
  P2VAR(Dcm_DataTransferType, AUTOMATIC, DCM_VAR) DataTranContextTypePtr;

  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  Std_ReturnType ReqDownloadResult = DCM_E_NOT_OK;
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  uint8 InstIdx;
#endif

#if ((DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403) || \
  (DCM_NUM_PROTOCOL > 1U))
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_Dsl_RxConnectionGetRxPduIdInformation(pMsgContext->dcmRxPduId, &RxPduIdInfo);
#endif

#if (DCM_NUM_PROTOCOL > 1U)
  InstIdx = RxPduIdInfo.ProtocolIndex;
#endif

  DataTranContextTypePtr = &DCM_DATA_TRANSFER_CONTEXT(InstIdx);

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
  if (OpStatus == DCM_INITIAL)
  {
    ReqDownloadResult = Dcm_ProcessRequestDownload(OpStatus,
                                                   DataTranContextTypePtr->RequestDownloadFormatId,
                                                   DataTranContextTypePtr->MemAddres,
                                                   DataTranContextTypePtr->MemSize,
                                                   RxPduIdInfo.RxBufferSize,
                                                   &Nrc);
  }
  else
  {
    ReqDownloadResult = Dcm_ProcessRequestDownload(OpStatus, 0U, 0U, 0U, 0U, &Nrc);
  }
#else
  /* !LINKSTO Dcm.EB.RequestDownload.maxNumberOfBlockLength.AUTOSAR430,1 */
  ReqDownloadResult = Dcm_ProcessRequestDownload(OpStatus,
                                                 DataTranContextTypePtr->RequestDownloadFormatId,
                                                 DataTranContextTypePtr->MemAddres,
                                                 DataTranContextTypePtr->MemSize,
                                                 &(DataTranContextTypePtr->BlockLength),
                                                 &Nrc);
#endif /*DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

  if (OpStatus != DCM_CANCEL)
  {
    switch (ReqDownloadResult)
    {
      case E_OK:
      {
        /* Response data contains 'lengthFormatIdentifier' .Its highest nibble contains the
           number of bytes in the following field (maxNumberOfBlockLength).Loweset
           nibble is zero */
        uint8_least BlockBytes = DCM_MAX_ADDRESS_BYTES;
        uint32 BlockLength = 0U;

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
        /* Use size of Rx buffer*/
        /* !LINKSTO Dcm.EB.RequestDownload.maxNumberOfBlockLength.AUTOSAR403,1 */
        DataTranContextTypePtr->BlockLength = RxPduIdInfo.RxBufferSize;
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/

        pMsgContext->resData[0U] = (uint8)(BlockBytes << DCM_NIBBLE_BITS);
        pMsgContext->resDataLen++;

        /* Assembling maxNumberOfBlockLength[] in the response. */
        BlockLength = DataTranContextTypePtr->BlockLength;
        while (BlockBytes > 0U)
        {
          pMsgContext->resData[BlockBytes] = (uint8)(BlockLength & DCM_MASK_BYTE);
          pMsgContext->resDataLen++;
          BlockLength = (uint32)BlockLength >> DCM_BYTE_BITS;
          BlockBytes--;
        }

        /* Registering Download Request */
        DataTranContextTypePtr->TransState = DCM_TRANSFER_DOWNLOAD;
        /* The blockSequenceCounter shall be initialized to one, when receiving a RequestDownload */
        DataTranContextTypePtr->BlockSequenceCounter = 1U;
        /* Setting the total data transferred to zero */
        DataTranContextTypePtr->MemTransfered = 0U;
        ServiceProcessStatus = DCM_E_DONE;
        break;
      }

      case E_NOT_OK:
        /* !LINKSTO Dcm757,1 */
        /* !LINKSTO SWS_Dcm_01132,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
        ServiceProcessStatus = DCM_E_DONE;
        break;

      case DCM_E_PENDING:
        ServiceProcessStatus = DCM_E_PENDING;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.RequestDownload.WrongInterfaceReturn.DetError,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_REQUESTDOWNLOAD, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        /* !LINKSTO Dcm.EB.RequestDownload.WrongInterfaceReturn.NRC0x10,1 */
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

#endif /* #if(DCM_DSP_USE_SERVICE_0X34 == STD_ON) */

/*==================[end of file]================================================================*/
