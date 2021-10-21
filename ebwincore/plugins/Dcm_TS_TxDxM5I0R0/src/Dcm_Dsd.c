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

/*This file contains the implementation of the Diagnostic Service
  * Dispatcher module in Dcm. */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The macro is used in function parameter declarations and definitions
  *     where the number of parentheses matter.
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
  * This warnings is a false positive.
  * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
  * - it prepares in advance the index for the next iteration
  * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
  * - since the index is already equal to '1', it never jumps back to check the exit condition
  */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_Int.h>                                             /* Internal declarations of Dcm */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */
#include <Dcm_Dsl_Supervisor.h>                                /* Dcm Dsl Supervisor header file */

#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#include <Dcm_Dsl_SecurityLevelHandler.h>
#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>   /* Dcm Dsl Supervisor Main Connection Handling header file */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#include <Dcm_Dsp_SvcH_SecurityAccess.h>
#endif

/*===========================[macro definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_UpdateHsmInfoAfterValidSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig,
  Dcm_NegativeResponseCodeType curNrc);

STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_UpdateHsmInfoAfterValidSubSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig,
  Dcm_NegativeResponseCodeType curNrc);

/** \brief Used to validate the input service request
 **
 ** \param[in] instIdx Current HSM instance Id
 ** \param[in] curSidConfig Current ServiceId config
 **
 ** \retval DCM_E_SERVICENOTSUPPORTED Service not supported in the current Protocol
 ** \retval DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION Service not supported in the current Session
 ** \retval DCM_E_SECURITYACCESSDENIED Service not supported in the current Security Level
 ** \retval DCM_E_BUSYREPEATREQUEST Tx buffer is not available in order to process the request
 */
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DsdInternal_ValidateSvcReq(
  uint8 instIdx,
  P2VAR(Dcm_SidTabEntryConfigPtrType, AUTOMATIC, DCM_CONST) curSidConfig);

/** \brief Used to validate the input subservice request
 **
 ** \param[in] instIdx Current HSM instance Id
 ** \param[in] curSidConfig Current ServiceId config
 **
 ** \retval DCM_E_SUBFUNCTIONNOTSUPPORTED Subservice not supported in the current Protocol
 ** \retval DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION Subservice not supported in the current
 ** Session
 ** \retval DCM_E_SECURITYACCESSDENIED Subservice not supported in the current Security Level
 ** \retval DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT Incorrect message length in order to
 ** process the request
 */
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DsdInternal_ValidateSubSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig);

/** \brief Check if sub-service request is allowed in current session
 **
 ** \param[in] subSidIdxExt  The sub-service Id
 ** \param[in] curSidConfig  Current ServiceId config
 **
 ** \retval FALSE    Subservice not supported in the current session
 ** \retval TRUE     Subservice supported in the current
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_DsdInternal_CheckSubServiceSession(
  uint8 subSidIdxExt,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig);

/** \brief Check sub-service request is allowed with current security level
 **
 ** \param[in] subSidIdxExt  The sub-service Id
 ** \param[in] curSidConfig  Current ServiceId config
 **
 ** \retval FALSE    Subservice not supported in the current Security Level
 ** \retval TRUE     Subservice supported in the current Security Level
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_DsdInternal_CheckSubServiceSecurity(
  uint8 subSidIdxExt,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig);

#if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON )
/** \brief Used to verify the permissions from manufactrer notification
 ** functions
 **
 ** \param[in] instIdx Current HSM instance Id
 **
 ** \retval E_OK Permission is granted
 ** \retval E_NOT_OK Permission is not granted.Nrc has been returned from Notification function.
 ** \retval DCM_E_REQUEST_NOT_ACCEPTED Permission is not granted. Reject the diagnostic request.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DsdInternal_ManufacturerNotification(uint8 instIdx);
#endif /* #if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON ) */

#if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON )
/** \brief Used to verify the permissions from Supplier notification
 ** functions
 **
 ** \param[in] instIdx Current HSM instance Id
 **
 ** \retval E_OK Permission is granted
 ** \retval E_NOT_OK Permission is not granted.Nrc has been returned from Notification function.
 ** \retval DCM_E_REQUEST_NOT_ACCEPTED Permission is not granted. Reject the diagnostic request.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DsdInternal_SupplierNotification(uint8 instIdx);
#endif /* #if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON ) */

/** \brief Used to set the correct request and message type
 ** functions
 **
 ** \param[in] RequestType The type of the request that this Protocol is currently processing
 ** \param[out] curMsgContext Current message context
 ** \param[out] TxBufferUse The Tx Buffer type
 */
STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_HandleReqMsgType
(
    Dcm_RequestTypeType RequestType,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) curMsgContext,
    P2VAR(Dcm_BufferUsageType, AUTOMATIC, DCM_VAR) TxBufferUse
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#if (DCM_DEV_ERROR_DETECT == STD_ON)
#define DCM_START_SEC_CONST_32
#include <Dcm_MemMap.h>
/** \brief Global array holding the Negative Response Code information. Each bit position in the
 ** array element represents NRC code. Bit '1' represents valid NRC and Bit '0' represents
 ** ISO reserved NRC. The number '32U' in expression represents number of bits in one
 ** array element.
*/
/* !LINKSTO Dcm228,1 */
/* Deviation MISRAC2012-2 */
STATIC CONST(uint32, DCM_CONST) Dcm_ValidNegativeResponseCodeMask[8] = {
    (1UL << DCM_E_GENERALREJECT)             | (1UL << DCM_E_SERVICENOTSUPPORTED) |
    (1UL << DCM_E_SUBFUNCTIONNOTSUPPORTED)   |
    (1UL << DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT) |
    (1UL << DCM_E_RESPONSETOOLONG),
    (1UL << (DCM_E_BUSYREPEATREQUEST % 32U)) | (1UL << (DCM_E_CONDITIONSNOTCORRECT % 32U)) |
    (1UL << (DCM_E_REQUESTSEQUENCEERROR % 32U)) |
    (1UL << (DCM_E_NORESPONSEFROMSUBNETCOMPONENT % 32U)) |
    (1UL << (DCM_E_FAILUREPREVENTSEXECUTIONOFREQUESTEDACTION % 32U)) |
    (1UL << (DCM_E_REQUESTOUTOFRANGE % 32U)) | (1UL << (DCM_E_SECURITYACCESSDENIED % 32U)) |
    (1UL << (DCM_E_INVALIDKEY % 32U))        | (1UL << (DCM_E_EXCEEDNUMBEROFATTEMPTS % 32U)) |
    (1UL << (DCM_E_REQUIREDTIMEDELAYNOTEXPIRED % 32U)),
    0x00000000UL,
    (1UL << (DCM_E_UPLOADDOWNLOADNOTACCEPTED % 32U)) |
    (1UL << (DCM_E_TRANSFERDATASUSPENDED % 32U)) |
    (1UL << (DCM_E_GENERALPROGRAMMINGFAILURE % 32U)) |
    (1UL << (DCM_E_WRONGBLOCKSEQUENCECOUNTER % 32U)) |
    (1UL << (DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING % 32U)) |
    (1UL << (DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION % 32U))  |
    (1UL << (DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION % 32U)),
    (1UL << (DCM_E_RPMTOOHIGH % 32U))            | (1UL << (DCM_E_RPMTOOLOW % 32U)) |
    (1UL << (DCM_E_ENGINEISRUNNING % 32U))       | (1UL << (DCM_E_ENGINEISNOTRUNNING % 32U))    |
    (1UL << (DCM_E_ENGINERUNTIMETOOLOW % 32U))   | (1UL << (DCM_E_TEMPERATURETOOHIGH % 32U))    |
    (1UL << (DCM_E_TEMPERATURETOOLOW % 32U))     | (1UL << (DCM_E_VEHICLESPEEDTOOHIGH % 32U))   |
    (1UL << (DCM_E_VEHICLESPEEDTOOLOW % 32U))    | (1UL << (DCM_E_THROTTLE_PEDALTOOHIGH % 32U)) |
    (1UL << (DCM_E_THROTTLE_PEDALTOOLOW % 32U))  |
    (1UL << (DCM_E_TRANSMISSIONRANGENOTINNEUTRAL % 32U)) |
    (1UL << (DCM_E_TRANSMISSIONRANGENOTINGEAR % 32U)) |
    (1UL << (DCM_E_BRAKESWITCH_NOTCLOSED % 32U)) | (1UL << (DCM_E_SHIFTERLEVERNOTINPARK % 32U)) |
    (1UL << (DCM_E_TORQUECONVERTERCLUTCHLOCKED % 32U)) | (1UL << (DCM_E_VOLTAGETOOHIGH % 32U))  |
    (1UL << (DCM_E_VOLTAGETOOLOW % 32U)),
    0x00000000UL,
    0x00000000UL,
    0x00000000UL
  };
#define DCM_STOP_SEC_CONST_32
#include <Dcm_MemMap.h>

#endif  /* DCM_DEV_ERROR_DETECT == STD_ON */

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.DiagServiceDispatcher.IB.DispatchSvcReq,1 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_DsdInternal_DispatchSvcReq(
  DCM_PDL_DISPATCH_SERVICE_REQ(uint8 instIdx))
{
  Dcm_BufferUsageType TxBufferUse = DCM_BUFFER_NONE;
  BufReq_ReturnType TxBufferAllocationResult;

  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* get pointer to current message context */
  CONSTP2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) curMsgContext = &curHsmInfo->msgContext;

  /* current ServiceId config */
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig = NULL_PTR;

  DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_ENTRY(DCM_CUR_INST_ID(instIdx));

  Dcm_DsdInternal_HandleReqMsgType(curHsmInfo->RequestType, curMsgContext, &TxBufferUse);

  curHsmInfo->negativeResponseCode = DCM_E_OK;

  /* Reset the page processing function id */
  curHsmInfo->processPageFuncId = DCM_PROCESS_PAGE_ID_INVALID;

  /* Set the service type to DSP implementation */
  curHsmInfo->dspService = TRUE;

  /* setup context information for current Hsm */
  curHsmInfo->serviceId = curHsmInfo->RxBuffer.BufferPtr[0];

  /* Only Normal and ROE processing are allowed to respond with NRCs, therefore assign NRC
     buffers only in these two specific cases */
  DCM_PRECONDITION_ASSERT(
    !(DCM_IS_PERIODIC_REQUEST(curHsmInfo->RequestType)),DCM_INTERNAL_API_ID);

  /* Assign NRC buffer for this protocol. This should ALWAYS succeed (if the TxConnection were
  blocked we would have had no dispatch anyway/we wouldn't have reached this point). Therefore
  the allocation result is ignored. The NRC buffer is assigned giving the TxConnectionId as the
  Requester since there is always one NRC buffer per TxConnection.
  */
  (void)Dcm_Dsl_BufferAllocate(curHsmInfo->TxConnectionId,
                               DCM_BUFFER_TX_NRC,
                               DCM_BUFFER_NRC_BUFFER_SIZE,
                               &(curHsmInfo->NRCBuffer));

  /* For transmissions, the size of the transmission data is unknown yet, therefore the buffer
     allocator shall ignore this parameter for buffers for which the usage is transmission. */
  TxBufferAllocationResult = Dcm_Dsl_BufferAllocate(
      DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)),
      TxBufferUse,
      DCM_BUFFER_IGNORE_BUFFER_SIZE,
      &(curHsmInfo->TxBuffer));

  /* check if Tx buffer is available in order to process this request
   * and mark it as 'in use' */
  if (TxBufferAllocationResult != BUFREQ_OK)
  {

#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)

  curHsmInfo->msgContext.reqDataLen = curHsmInfo->RxBuffer.FilledLength - 1U;
  /*
   * setup part of message context which is required for service AND error handling
   */
  /* verify if request data contains something other than ServiceId */
  if(curHsmInfo->RxBuffer.FilledLength > 1U)
  {
    /* set request data pointer to received data without service Id */
    curHsmInfo->msgContext.reqData =
      &curHsmInfo->RxBuffer.BufferPtr[1U];
  }
  else
  {
    /* place NULL_PTR in reqData pointer as there is no data after ServiceId */
    curHsmInfo->msgContext.reqData = NULL_PTR;
  }
  /* initialize with service id */
  curHsmInfo->msgContext.idContext = curHsmInfo->serviceId;
  /* set request data pointer to received data without service Id (needed in DcmConfirmation()
   * of pos and neg. responses) */
  curHsmInfo->msgContext.dcmRxPduId = curHsmInfo->RxConnectionId;
  /* if all preconditions are ok fulfilled then continue process */

    /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.TxBufferBusy.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.RequestHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TxBufferBusy.BRREnabled.TransmitBRR,1 */
    curHsmInfo->negativeResponseCode = DCM_E_BUSYREPEATREQUEST;

    /* as we don't call any service, we have to send the 'processing done' signal ourselves */
    (void) DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
      DCM_HSM_DCM_EV_PROC_DONE);

#else /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
    if ((DCM_HSM_INFO(instIdx).rpCntr == 0U) ||
        (curHsmInfo->serviceId == DCM_SID_CLEAR_DTC) ||
        (curHsmInfo->serviceId == DCM_SID_REQUEST_VEHICLE_INFORMATION))
#else
    if (DCM_HSM_INFO(instIdx).rpCntr == 0U)
#endif
    {
      /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.TxBufferBusy.BRRDisabled.Discard,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TxBufferBusy.BRRDisabled.Discard,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.TxBufferBusy.BRRDisabled.OBD0x04.RCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.TxBufferBusy.BRRDisabled.OBD0x09.RCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TxBufferBusy.BRRDisabled.OBD0x04.RCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TxBufferBusy.BRRDisabled.OBD0x09.RCRRPSent.Discard,1 */
      /* Abort processing as no response is required. */
      (void) DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
        DCM_HSM_DCM_EV_PROC_ABORT);
    }
    else
    {
      /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNTFRSLT_OK.TxBufferBusy.BRRDisabled.RCRRPSent.TransmitGR,1 */
      /* !LINKSTO Dcm.EB.RequestHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TxBufferBusy.BRRDisabled.RCRRPSent.TransmitGR,1 */
      curHsmInfo->negativeResponseCode = DCM_E_GENERALREJECT;
      /* Transmit NRC and finish processing */
      (void) DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
        DCM_HSM_DCM_EV_PROC_DONE);
    }
#endif /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) */
  }
  else
  {
#if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON )
    /* If manufacturer notification is enabled, calling these functions for getting permissions*/
    if(Dcm_DsdInternal_ManufacturerNotification(DCM_CUR_INST_ID(instIdx)) == E_OK)
#endif /* #if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON ) */
    {
      Dcm_NegativeResponseCodeType curNrc;
      /* Validate the input service request */
      curNrc = Dcm_DsdInternal_ValidateSvcReq(DCM_CUR_INST_ID(instIdx), &curSidConfig);
      Dcm_DsdInternal_UpdateHsmInfoAfterValidSvcReq(DCM_CUR_INST_ID(instIdx),
                                                    curSidConfig,
                                                    curNrc);
      if (DCM_E_OK == curNrc)
      {
#if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON )
       /*If Supplier notification is enabled, calling these functions for getting permissions*/
        if (Dcm_DsdInternal_SupplierNotification(DCM_CUR_INST_ID(instIdx)) == E_OK)
#endif /* #if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON ) */
        {
          if ( (curSidConfig->ExtSrvHandler == NULL_PTR) &&
               (curSidConfig->NumSubServices > 0U) &&
               (curSidConfig->SubfuncAvail == TRUE) )
          {/* Validate the subservice request */
            curNrc = Dcm_DsdInternal_ValidateSubSvcReq(DCM_CUR_INST_ID(instIdx), curSidConfig);
          }

          if ( (curSidConfig->ExtSrvHandler != NULL_PTR) &&
               (curHsmInfo->serviceId == DCM_SID_SESSION_CONTROL) &&
               (curHsmInfo->RxBuffer.FilledLength < 2U) )
          {
            /* !LINKSTO Dcm.EB.DiagnosticSessionControl.NRC0x13,1 */
            curNrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
          }

          Dcm_DsdInternal_UpdateHsmInfoAfterValidSubSvcReq(DCM_CUR_INST_ID(instIdx),
                                                           curSidConfig,
                                                           curNrc);
        }
      }
      /* if any precondition is not ok set the respective error code */
      if (DCM_E_OK != curNrc)
      {
        curHsmInfo->negativeResponseCode = curNrc;
        /* as we don't call any subservice, we have to send
         * the 'processing done' signal ourselves */
        (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm,
                                            DCM_CUR_INST_ID(instIdx),
                                            DCM_HSM_DCM_EV_PROC_DONE);
      }
    }
  }

  DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_EXIT(DCM_CUR_INST_ID(instIdx));
}

/* !LINKSTO Dcm.ExternalSetNegResponse.API,1 */
FUNC(void, DCM_CODE) Dcm_ExternalSetNegResponse(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_APPL_DATA) pMsgContext,
  Dcm_NegativeResponseCodeType errorCode)
{
  DBG_DCM_EXTERNALSETNEGRESPONSE_ENTRY(pMsgContext,errorCode);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_SETNEGRESPONSE, DCM_E_UNINIT);
  }
  else if (pMsgContext == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_SETNEGRESPONSE, DCM_E_PARAM_POINTER);
  }
  /* Validating the range of errorCode*/
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* Get the current context info used by Rx Pdu Id */
#if (DCM_NUM_PROTOCOL > 1U)
  uint8 ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), pMsgContext->msgAddInfo.requestOrigin);
#endif
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(ProtocolIndex);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* The following check does not reject invalid NRCs anymore but only triggers the Det error.
     * Reason: The NRC can be part of a range, e.g. manufacturerSpecificConditionsNotCorrect.
     *         In this case the NRC is legal and must not be filtered. */
    if ((Dcm_ValidNegativeResponseCodeMask[errorCode / 32U] & (1UL << (errorCode % 32U))) == 0U)
    {
      DCM_DET_REPORT_ERROR(DCM_SERVID_SETNEGRESPONSE, DCM_E_PARAM);
    }
#endif

#if (DCM_NUM_PROTOCOL == 1U)
    TS_PARAM_UNUSED(pMsgContext);
#endif

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* In case of async service execution this function is accessing
     * negativeResponseCode from a different task context other than the HSM */

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

    /* if no error occurred yet set negative response code of current Hsm */
    if (curHsmInfo->negativeResponseCode == DCM_E_OK)
    {
      curHsmInfo->negativeResponseCode = errorCode;
    }

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
  }

  DBG_DCM_EXTERNALSETNEGRESPONSE_EXIT(pMsgContext,errorCode);
}

/* !LINKSTO Dcm.ExternalProcessingDone.API,1 */
FUNC(void, DCM_CODE) Dcm_ExternalProcessingDone(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_APPL_DATA) pMsgContext)
{
  DBG_DCM_EXTERNALPROCESSINGDONE_ENTRY(pMsgContext);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_PROCESSINGDONE, DCM_E_UNINIT);
  }
  else if (pMsgContext == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_PROCESSINGDONE, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
#if (DCM_NUM_PROTOCOL > 1U)
    uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#else
#if (DCM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(pMsgContext);
#endif
#endif

     /* Update the FilledLength of the
        transmission buffer with the amount of data the service handler wrote, plus one
        byte for the service ID. */
     DCM_HSM_INFO(instIdx).TxBuffer.FilledLength =
       (Dcm_BufferSizeType)((Dcm_BufferSizeType)(pMsgContext->resDataLen) + 1U);
  }

  DBG_DCM_EXTERNALPROCESSINGDONE_EXIT(pMsgContext);
}
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

FUNC(void, DCM_CODE) Dcm_DsdInternal_StartPagedProcessing(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_ENTRY(pMsgContext);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_STARTPAGEDPROCESSING, DCM_E_UNINIT);
  }
  else if (pMsgContext == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_STARTPAGEDPROCESSING, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* get length of data already stored in buffer */
    Dcm_MsgLenType curTxBufferLen;

#if (DCM_NUM_PROTOCOL > 1U)
    /* HSM instance */
    uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

#if ((DCM_NUM_PROTOCOL == 1U) && (DCM_DEV_ERROR_DETECT == STD_OFF))
    TS_PARAM_UNUSED(pMsgContext);
#endif

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    curTxBufferLen = DCM_HSM_INFO(instIdx).TxBuffer.FilledLength;

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* init length information for keeping track of response total length */
    DCM_HSM_INFO(instIdx).sentLength = curTxBufferLen;
  }

  DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_EXIT(pMsgContext);
}

FUNC(void, DCM_CODE) Dcm_DsdInternal_ProcessPage(Dcm_MsgLenType FilledPageLen)
{
  DBG_DCM_DSDINTERNAL_PROCESSPAGE_ENTRY(FilledPageLen);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_PROCESSPAGE, DCM_E_UNINIT);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
#if (DCM_NUM_PROTOCOL > 1U)
    /* get HSM instance which corresponds to the Tx Pdu Id used by current protocol
     *
     * Due to the lack of an argument defining the protocol instance in the function signature,
     * it is not possible to derive the current HSM instance.
     * Consequently, we have to derive it from the RxPduID on which the request was received.
     */
    const uint8 instIdx = DCM_PROTOCOL_INDEX_FROM_ID(
      Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

    DCM_INVARIANT_ASSERT(DCM_INVALID_PROTOCOL_INDEX != instIdx, DCM_INTERNAL_API_ID);
#endif

    /* update length information of response total length */
    DCM_HSM_INFO(instIdx).sentLength += FilledPageLen;

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* update length information of transmission buffer */
    DCM_HSM_INFO(instIdx).TxBuffer.FilledLength += FilledPageLen;

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Trigger the HSM to process the page. The dataState will be set to DCM_TX_DATA_READY in the
     * HSM as reaction to this event. */

    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROCESS_PAGE);
  }

  DBG_DCM_DSDINTERNAL_PROCESSPAGE_EXIT(FilledPageLen);
}
#endif

#if (DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION == STD_ON)
/* !LINKSTO Dcm.EB.Behavior.DcmDsdDisableGenericServiceImplementation.API,1 */
/* !LINKSTO Dcm.EB.Behavior.DcmDsdDisableGenericServiceImplementation.API.Function,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_APICaller_GenericSvcHImplementation
(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
)
{
  Dcm_NegativeResponseCodeType curNrc;
  Std_ReturnType returnValueOfGenericSvcH;

#if (DCM_NUM_PROTOCOL > 1U)
  /* get protocol instance index */
  /* This service can be invoked also from ROE, so ROE connection needs to be taken into account */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProtocolIndexFromRxPduIdAndRequestType(
      pMsgContext->dcmRxPduId, (uint8)pMsgContext->msgAddInfo.requestOrigin);
#endif

  /* get HSM context information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &(DCM_HSM_INFO(instIdx));

  /* current ServiceId config */
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig =
          &(Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(instIdx).ServiceTableIndex].
                Entries[curHsmInfo->sidTabEntryInd]);

  DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_ENTRY(OpStatus, pMsgContext);

  if ( (OpStatus == DCM_INITIAL) &&
       (curSidConfig->IntSrvHandler != NULL_PTR) &&
       (curSidConfig->NumSubServices > 0U) &&
       (curSidConfig->SubfuncAvail == TRUE) )
  {
    /* !LINKSTO Dcm.EB.Behavior.DcmDsdDisableGenericServiceImplementation.API.SubServiceValidation,1 */
    /* Validate the subservice request */
    curNrc = Dcm_DsdInternal_ValidateSubSvcReq(DCM_CUR_INST_ID(instIdx), curSidConfig);
  }
  else
  {
    curNrc = DCM_E_OK;
  }

  if ( (DCM_E_OK == curNrc) &&
       (curSidConfig->IntSrvHandler != NULL_PTR) )
  {
    /* !LINKSTO Dcm.EB.Behavior.DcmDsdDisableGenericServiceImplementation.API.CallGenericSvcH,1 */
    returnValueOfGenericSvcH = curSidConfig->IntSrvHandler(OpStatus, pMsgContext);
  }
  else if (DCM_E_OK != curNrc)
  {
    returnValueOfGenericSvcH = curNrc;
  }
  else
  {
    /* !LINKSTO Dcm.EB.Behavior.DcmDsdDisableGenericServiceImplementation.API.ReturnedErrors,1 */
    returnValueOfGenericSvcH = DCM_E_SERVICENOTSUPPORTED;
  }

  DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_EXIT(returnValueOfGenericSvcH, OpStatus, pMsgContext);

  return returnValueOfGenericSvcH;
}
#endif /* #if (DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION == STD_ON) */

/*==================[internal function definitions]==============================================*/

STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_UpdateHsmInfoAfterValidSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig,
  Dcm_NegativeResponseCodeType curNrc)
{
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif

  /*
   * setup part of message context which is required for service AND error handling
   */
  /* verify if request data contains something other than ServiceId */
  if(curHsmInfo->RxBuffer.FilledLength > 1U)
  {
    /* set request data pointer to received data without service Id */
    curHsmInfo->msgContext.reqData =
      &curHsmInfo->RxBuffer.BufferPtr[1];
  }
  else
  {
    /* place NULL_PTR in reqData pointer as there is no data after ServiceId */
    curHsmInfo->msgContext.reqData = NULL_PTR;
  }
  /* initialize with service id */
  curHsmInfo->msgContext.idContext = curHsmInfo->serviceId;
  /* set request data pointer to received data without service Id (needed in DcmConfirmation()
   * of pos and neg. responses) */
  curHsmInfo->msgContext.dcmRxPduId = curHsmInfo->RxConnectionId;
  /* if all preconditions are ok fulfilled then continue process */
  if (DCM_E_OK == curNrc)
  {
    /* setup message context for diagnostic service function */

    /* set response ID in first byte of Tx curHsmInfo */
    curHsmInfo->TxBuffer.BufferPtr[0] = (curHsmInfo->serviceId | DCM_SID_RESPONSE_BIT);
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    /* set reqDataLen of MsgContext to length of data in reception buffer without the
     * service Id (1 Byte) */
    curHsmInfo->msgContext.reqDataLen = (Dcm_MsgLenType)(curHsmInfo->RxBuffer.FilledLength - 1U);
    /* buffer is already filled with the response ID.  So buffer fill is already 1 */
    curHsmInfo->TxBuffer.FilledLength = 1U;
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    /* set response buffer of current message context to allocated transmission buffer increase
     * address by one, as the filled length is already 1 */
    curHsmInfo->msgContext.resData =  &(curHsmInfo->TxBuffer.BufferPtr[1]);
    /* reset length of reponse data */
    curHsmInfo->msgContext.resDataLen = 0U;
    /* reset positive response suppression */
    curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = FALSE;
    /* if service request is configured to have a subfunction parameter handle
     * 'suppress positive response bit' and assign value for subServiceId */
    if ((curSidConfig->SubfuncAvail == TRUE) && (curHsmInfo->msgContext.reqDataLen > 0U))
    {
      if(0U != (curHsmInfo->msgContext.reqData[0] & DCM_SUPPOSRES_BITMASK))
      {
        /* set suppress positive response information */
        curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = TRUE;
        /* clear 'suppress positive message response' bit from subfunction parameter */
        curHsmInfo->msgContext.reqData[0] &= DCM_CLEAR_SUPPOSRES_BIT;
      }
      else
      {
        /* Subfunction supported for the service, but not requested in the current request */
        curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = FALSE;
      }
      /*
       * If a service has subservice support update subservice
       * Id into HsmInfoType irrespective of
       * other checks. The SubServiceId shall contain the ROE persistence bit.
       */
      curHsmInfo->subServiceId = curHsmInfo->msgContext.reqData[0];
    }
    else
    {
      /* Subfunction not supported for the requested service */
      curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = FALSE;
    }
    /* set size of transmission buffer of current message context */
    curHsmInfo->msgContext.resMaxDataLen = (Dcm_MsgLenType)(curHsmInfo->TxBuffer.Size - 1U);
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_UpdateHsmInfoAfterValidSubSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig,
  Dcm_NegativeResponseCodeType curNrc)
{
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif

  if (DCM_E_OK == curNrc)
  {
    /* No failures during initial validation of service request */
    curHsmInfo->requestValidationStatus = TRUE;
    if (curSidConfig->ExtSrvHandler != NULL_PTR)
    {
      /* Set the service type to User implementation */
      curHsmInfo->dspService = FALSE;
    }

    Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured(curHsmInfo->serviceId);

    /* !LINKSTO Dcm527,1, Dcm221,1 */
    /* Call the service processing function for service handlers */
    Dcm_DspInternal_SvcFunc(DCM_CUR_INST_ID(instIdx) , DCM_INITIAL);

  }
}

/* !LINKSTO Dcm.Dsn.Dsd.IB.ValidateSvcReq,1 */
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DsdInternal_ValidateSvcReq(
  uint8 instIdx,
  P2VAR(Dcm_SidTabEntryConfigPtrType, AUTOMATIC, DCM_CONST) curSidConfig)
{
  uint8_least i = 0U;
  uint8 sidIdxExt = 0U;
  uint8 modeRuleNrc;
  Dcm_NegativeResponseCodeType curNrc = DCM_E_NOT_OK;
  boolean serviceEnabled = FALSE;

  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* get configuration for current HSM instance */
  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig
    = &DCM_PROTOCOL_CONFIG(instIdx);

  uint8 ServiceTableIndex = curProtocolConfig->ServiceTableIndex;

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[ServiceTableIndex];

  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) sidConfig = NULL_PTR;

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* !LINKSTO Dcm193,1 */
  /* search list of configured services */
  while ( sidIdxExt < curSidTabConfig->NumEntries )
  {
    if ((curSidTabConfig->Entries[sidIdxExt]).ServiceId == curHsmInfo->serviceId)
    {
      const uint8 serviceEnabledByteIdx = sidIdxExt / 8U;
      const uint8 serviceEnabledBitMask = (1U << (sidIdxExt % 8U));

      /* Check if the corresponding service is enabled */
      if ((Dcm_DsdServiceUsed[ServiceTableIndex][serviceEnabledByteIdx] &
          serviceEnabledBitMask) != 0U)
      {
        serviceEnabled = TRUE;
      }

      curHsmInfo->sidTabEntryInd = sidIdxExt;
      break;
    }

    sidIdxExt++;
  }

  /* If service was not found in table of service functions configured for the current running
   * protocol or it configured as disabled, we have to send an appropriate negative response */
  if (serviceEnabled == FALSE)
  {
   /* !LINKSTO Dcm.EB.OBD.AllSvc.RequestedServiceNotSupported,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x00,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x01,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x02,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x03,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x04,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x06,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x07,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x08,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x09,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x0A,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x0B-0x0F,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x00,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x01,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x02,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x03,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x04,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x06,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x07,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x08,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x09,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x0A,1 */
   /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x0B-0x0F,1 */
    if ( (DCM_IS_OBD_SVCID(curHsmInfo->serviceId)) && (curProtocolConfig->SupressServiceNotSupportedNRC == TRUE) )
    {
      curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = TRUE;
    }
    curNrc = DCM_E_SERVICENOTSUPPORTED;
  }
  else
  {
    curNrc = DCM_E_OK;
  }

  /*
   * check if service request is allowed in current session and security levels
   * and mode rule has not failed
   */
  if (curNrc == DCM_E_OK)
  {
    /* set configuration entry of this service Id */
    sidConfig = &(curSidTabConfig->Entries[sidIdxExt]);
    *curSidConfig = sidConfig;

    /*
     * check if service request is allowed in current session
     */

    /* No diagnostic session and security level checks are performed for OBD service
       requests */
    if (! DCM_IS_OBD_SVCID(curHsmInfo->serviceId) )
    {
      /* If no session control configuration is provided for the service,
        no check of session level shall be done */
      if (sidConfig->NumSesCtrls > 0U)
      {
        /* get current session type */
        const Dcm_SesCtrlType curSesCtrl = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

        /*
        * check if service request is allowed in current session
        */
        for (i = 0U; i < sidConfig->NumSesCtrls; ++i)
        {
          if ((sidConfig->SesCtrls[i] == DCM_ALL_SESSION_LEVEL) ||
              (sidConfig->SesCtrls[i] == curSesCtrl))
          {
            break;
          }
        }
        if (i >= sidConfig->NumSesCtrls)
        {
          curNrc = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
        }
      }

      /* check if service request is allowed in current security level
        If no security level configuration is provided for the service,
        no check of security level shall be done */
      if ((curNrc == DCM_E_OK) && (sidConfig->NumSecLevels > 0U))
      {
        /* get current security level */
        Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

        if (curSecLevel != DCM_SEC_LEV_LOCKED)
        {
          /*
          * check if service request is allowed with current security level
          */
          for (i = 0U; i < sidConfig->NumSecLevels; ++i)
          {
            if (sidConfig->SecLevels[i] == curSecLevel)
            {
              break;
            }
          }
          if (i >= sidConfig->NumSecLevels)
          {
            curNrc = DCM_E_SECURITYACCESSDENIED;
          }
        }
        else
        {
          curNrc = DCM_E_SECURITYACCESSDENIED;
        }
      }
    }

    if ((curNrc == DCM_E_OK) && (sidConfig->ModeRuleSrvFunction != NULL_PTR))
    {
      /* invoke the mode rule function for the service */
      sidConfig->ModeRuleSrvFunction(&modeRuleNrc);

      /* if the mode rule resulted in failure modeRuleNrc will have the calculated nrc,
         else zero which implies there is no failure */
      curNrc = modeRuleNrc;
    }
  }

  return curNrc;
}

STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DsdInternal_ValidateSubSvcReq(
  uint8 instIdx,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig)
{
  uint8 subSidIdxExt = 0U;
  uint8 subServiceId = 0U;
  uint8 modeRuleNrc;
  Dcm_NegativeResponseCodeType curNrc = DCM_E_NOT_OK;
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);
  /* get pointer to current message context */
  CONSTP2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) curMsgContext = &curHsmInfo->msgContext;

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif

  if(curMsgContext->reqDataLen > 0U)
  {
    subServiceId = curMsgContext->reqData[0];

    /* search list of configured subservices */
    while ( subSidIdxExt < curSidConfig->NumSubServices )
    {
      if (curSidConfig->SubServices[subSidIdxExt].SubServiceId == subServiceId)
      {
        break;
      }
      subSidIdxExt++;
    }

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
    /* in order to comply to the ISO NRC order for RoutineControl, do not check nrc 0x12 at this point */
    if (curSidConfig->ServiceId != DCM_SID_ROUTINE_CONTROL)
    {
#endif /*(DCM_DSP_USE_SERVICE_0X31 == STD_ON)*/
      if ( subSidIdxExt >= curSidConfig->NumSubServices )
      {
        /* subservice was not found for the current running service, send NRC */
        curNrc = DCM_E_SUBFUNCTIONNOTSUPPORTED;
      }
      else
      {
        curNrc = DCM_E_OK;
      }
#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
    }
    else
    {
      curNrc = DCM_E_OK;
    }
#endif /*#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)*/
  }
  else
  {
    curNrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
  /* RoutineControl sub-services are always available (session,security,mode-rule) */
  if (curSidConfig->ServiceId != DCM_SID_ROUTINE_CONTROL)
  {
#endif /*#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)*/
    if(curNrc == DCM_E_OK)
    {
      if((Dcm_DsdInternal_CheckSubServiceSession(subSidIdxExt,curSidConfig)) == FALSE)
      {
        /* Set NRC if sub-service request is not allowed in current session */
        curNrc = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
      }
      else if((Dcm_DsdInternal_CheckSubServiceSecurity(subSidIdxExt,curSidConfig)) == FALSE)
      {
        /* Set NRC if sub-service request is not allowed with current security level */
        curNrc = DCM_E_SECURITYACCESSDENIED;
      }
      else
      {
        /* Do nothing, another NRC has been previously set. This branch is for MISRA. */
      }
    }

    if ((curNrc == DCM_E_OK) &&
       (curSidConfig->SubServices[subSidIdxExt].ModeRuleSubSrvFunction != NULL_PTR))
    {
      /* invoke the mode rule function for the sub-service */
      curSidConfig->SubServices[subSidIdxExt].ModeRuleSubSrvFunction(&modeRuleNrc);

      /* if the mode rule resulted in failure modeRuleNrc will have the calculated nrc,
       * else zero which implies there is no failure */
      curNrc = modeRuleNrc;
    }
#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
  }
#endif/*#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)*/

  if (curNrc == DCM_E_OK)
  {
    /* subservice found, store the index to reduce searches in service handlers */
    curHsmInfo->subServiceIdIdx = subSidIdxExt;
  }

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
  /* !LINKSTO Dcm.EB.SecurityAccess.SendKey.InvalidateSeed,2 */
  if ((curSidConfig->ServiceId == 0x27U ) && (curNrc != DCM_E_OK))
  {
#if (DCM_SECURITYACCESS_STRICT == STD_OFF)
    if (curNrc != DCM_E_SUBFUNCTIONNOTSUPPORTED)
#endif
    {
      /* Reset security access state handling */
      Dcm_Dsp_SecurityAccess_Reset();
    }
  }
#endif

  return curNrc;
}

STATIC FUNC(boolean, DCM_CODE) Dcm_DsdInternal_CheckSubServiceSession(
  uint8 subSidIdxExt,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig)
{
  uint8_least i = 0U;
  boolean subServiceSession = FALSE;

  /* If no session control configuration is provided for the subservice of current ,
   * running service , no check of session level shall be done
   * The verification done only if service contains sub services
   * Also if no session control configuration is provided for the subservice, no check of session
   * level shall be done */
  if (curSidConfig->SubServices[subSidIdxExt].NumSubSesCtrls > 0U )
  {
    /* get the current session type */
    const Dcm_SesCtrlType curSesCtrl = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

    /* check if sub-service request is allowed in current session */
    for (i = 0U;
         ((i < curSidConfig->SubServices[subSidIdxExt].NumSubSesCtrls)&&(subServiceSession != TRUE));
         ++i)
    {
      if ( (curSidConfig->SubServices[subSidIdxExt].SubSesCtrls[i] == DCM_ALL_SESSION_LEVEL) ||
           (curSidConfig->SubServices[subSidIdxExt].SubSesCtrls[i] == curSesCtrl) )
      {
        subServiceSession = TRUE;
      }
    }
  }
  else
  {
    subServiceSession = TRUE;
  }

  return subServiceSession;
}

STATIC FUNC(boolean, DCM_CODE) Dcm_DsdInternal_CheckSubServiceSecurity(
  uint8 subSidIdxExt,
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSidConfig)
{
  uint8_least i = 0U;
  boolean subServiceSecurity = FALSE;

  /* Check if subservice request is allowed in current security level
   * If no security level configuration is provided for the service,
   * check for sub-service security level */
  if ((curSidConfig->SubServices[subSidIdxExt].NumSubSecLevels > 0U))
  {
    /* get current security level */
    Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    if (curSecLevel != DCM_SEC_LEV_LOCKED)
    {
      /* check if sub-service request is allowed with current security level */
      for (i = 0U;
           ((i < curSidConfig->SubServices[subSidIdxExt].NumSubSecLevels)&&(subServiceSecurity != TRUE));
           ++i)
      {
        if (curSidConfig->SubServices[subSidIdxExt].SubSecLevels[i] == curSecLevel)
        {
          subServiceSecurity = TRUE;
        }
      }
    }
  }
  else
  {
    subServiceSecurity = TRUE;
  }

  return subServiceSecurity;
}

#if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DsdInternal_ManufacturerNotification(uint8 instIdx)
{
  Std_ReturnType MftNotifStatus;
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) CurHsmInfo= &DCM_HSM_INFO(instIdx);
  uint8 Sid = CurHsmInfo->serviceId;
  const uint16 DataSize = (uint16)(CurHsmInfo->RxBuffer.FilledLength - 1U);
  P2VAR(uint8, AUTOMATIC, DCM_VAR) RequestData;
  uint8 ReqType;
  Dcm_NegativeResponseCodeType curNrc = DCM_E_OK;
  Dcm_NegativeResponseCodeType aggregatedNrc = DCM_E_GENERALREJECT;
                                     /* default value in case no Indication() operation or */
                                     /* Rte_DcmManufacturerNotification() function sets a */
                                     /* proper NRC */
  uint16 SourceAddress = CurHsmInfo->TesterSourceAddress;

  /*Verification of the Manufacturer Application environment/permission*/
#if (DCM_INCLUDE_RTE == STD_ON)
  uint8_least i;
  Std_ReturnType NotificationRetVal = E_OK;
#endif /* #if (DCM_INCLUDE_RTE == STD_ON) */

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  Dcm_MainConnectionIndexType MainConnIdx =
      Dcm_RxPduIdConfig[CurHsmInfo->RxConnectionId].MainConnectionIndex;

  if( TRUE == Dcm_MainConnectionConfig[MainConnIdx].MetaDataIsConfigured )
  {
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.Xxx_Indication.SourceAddress,1 */
    SourceAddress = Dcm_PduInfo[Dcm_MainConnectionConfig[MainConnIdx].GenericConnectionIndex].SourceAddress;
  }
#endif

  if (DCM_IS_PHYSICAL_REQUEST(CurHsmInfo->RequestType))
  {
    ReqType = DCM_PHYSICAL_ADDRESSING;
  }
  else
  {
    ReqType = DCM_FUNCTIONAL_ADDRESSING;
  }

  if (DataSize != 0)
  {
    RequestData = &CurHsmInfo->RxBuffer.BufferPtr[1];
  }
  else
  {
    RequestData = NULL_PTR;
  }

#if (DCM_INCLUDE_RTE == STD_ON)
  MftNotifStatus = E_OK;
  /* Execute the  Xxx_ManufacturerIndication() for all configured ports */

 /* Deviation TASKING-1 */
  for (i = 0U; i < DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE; ++i)
  {
    NotificationRetVal =
    Dcm_RteManufacturerNotificationTable[i].Indication
                                      (Sid, RequestData, DataSize, ReqType,
                                       SourceAddress, &curNrc);

    /* If at least one of the Xxx_ManufacturerIndication() functions returns
        DCM_E_REQUEST_NOT_ACCEPTED, the Request Indication Status shall be
        DCM_E_REQUEST_NOT_ACCEPTED */
    if ( NotificationRetVal == DCM_E_REQUEST_NOT_ACCEPTED )
    {
      MftNotifStatus = DCM_E_REQUEST_NOT_ACCEPTED;
      break;
    }

    else
    {
      if (NotificationRetVal == E_NOT_OK)
      {
        /* check if we have a misbehaving Indication() operation that violates Dcm463 and Dcm694 */
        if (curNrc == DCM_E_OK)
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_MANUFACTURERNOTIFICATION,
            DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
        else
        {
          aggregatedNrc = curNrc;
        }

        MftNotifStatus = E_NOT_OK;
      }
      /* If RequestIndicationRetVal == E_OK, then the previous value of
      RequestIndicationStatus shall not be modified. */
    }
  }
#else /* #if (DCM_INCLUDE_RTE == STD_ON) */

  MftNotifStatus = Rte_DcmManufacturerNotification(Sid, RequestData, DataSize, ReqType,
                    SourceAddress, &curNrc);

  /* check if we have a misbehaving Rte_DcmManufacturerNotification()
   * function that violates Dcm463 and Dcm694 */
  if ((MftNotifStatus == E_NOT_OK) && (curNrc == DCM_E_OK))
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_MANUFACTURERNOTIFICATION,
      DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    aggregatedNrc = curNrc;
  }
#endif /* DCM_INCLUDE_RTE == STD_ON */

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */

  switch (MftNotifStatus)
  {
    case DCM_E_REQUEST_NOT_ACCEPTED:
#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
      if ((DCM_HSM_INFO(instIdx).rpCntr == 0U) ||
          (CurHsmInfo->serviceId == DCM_SID_CLEAR_DTC) ||
          (CurHsmInfo->serviceId == DCM_SID_REQUEST_VEHICLE_INFORMATION))
#else
      if (DCM_HSM_INFO(instIdx).rpCntr == 0U)
#endif
      {
        /* discard request */
        /* !LINKSTO Dcm.EB.RequestVerification.ManufacturerNotificationReturnsRequestNotAccepted.NoRCRRPSent.Discard,1 */
        /* !LINKSTO Dcm.EB.RequestVerification.ManufacturerNotificationReturnsRequestNotAccepted.OBD0x04.RCRRPSent.Discard,1 */
        /* !LINKSTO Dcm.EB.RequestVerification.ManufacturerNotificationReturnsRequestNotAccepted.OBD0x09.RCRRPSent.Discard,1 */
       (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm,
                                     DCM_CUR_INST_ID(instIdx),
                                     DCM_HSM_DCM_EV_PROC_ABORT);
      }
      else
      {
        /*  send negative response */
        /* !LINKSTO Dcm.EB.RequestVerification.ManufacturerNotificationReturnsRequestNotAccepted.RCRRPSent.TransmitCNC,1 */
        CurHsmInfo->negativeResponseCode = DCM_E_CONDITIONSNOTCORRECT;

        (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm,
                                            DCM_CUR_INST_ID(instIdx),
                                            DCM_HSM_DCM_EV_PROC_DONE);
      }
      break;

    case E_NOT_OK:

      CurHsmInfo->negativeResponseCode = aggregatedNrc;

      (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
        DCM_HSM_DCM_EV_PROC_DONE);
      break;

    default:
      /* Do Nothing */
      break;
  }

  return MftNotifStatus;
}
#endif /* #if(DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON )*/

#if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DsdInternal_SupplierNotification(uint8 instIdx)
{
  Std_ReturnType SupplNotifStatus;
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) CurHsmInfo= &DCM_HSM_INFO(instIdx);
  uint8 Sid = CurHsmInfo->serviceId;
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
    = &DCM_HSM_INFO(instIdx).msgContext;
  uint16 DataSize = (uint16)(pMsgContext->reqDataLen);
  P2VAR(uint8, AUTOMATIC, DCM_VAR) RequestData;
  uint8 ReqType = DCM_PHYSICAL_ADDRESSING;
  Dcm_NegativeResponseCodeType curNrc = DCM_E_OK;
  Dcm_NegativeResponseCodeType aggregatedNrc = DCM_E_GENERALREJECT;
                                     /* default value in case no Indication() operation or */
                                     /* Rte_DcmManufacturerNotification() function sets a */
                                     /* proper NRC */
  uint16 SourceAddress = CurHsmInfo->TesterSourceAddress;
/*Verification of the Supplier Application environment/permission */
#if (DCM_INCLUDE_RTE == STD_ON)
  uint8_least i;
  Std_ReturnType NotificationRetVal = E_OK;
#endif /* #if (DCM_INCLUDE_RTE == STD_ON) */

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  Dcm_MainConnectionIndexType MainConnIdx =
      Dcm_RxPduIdConfig[CurHsmInfo->RxConnectionId].MainConnectionIndex;

  if( TRUE == Dcm_MainConnectionConfig[MainConnIdx].MetaDataIsConfigured)
  {
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.Xxx_Indication.SourceAddress,1 */
    SourceAddress = Dcm_PduInfo[Dcm_MainConnectionConfig[MainConnIdx].GenericConnectionIndex].SourceAddress;
  }
#endif

  if (DCM_IS_PHYSICAL_REQUEST(CurHsmInfo->RequestType))
  {
    ReqType = DCM_PHYSICAL_ADDRESSING;
  }
  else
  {
    ReqType = DCM_FUNCTIONAL_ADDRESSING;
  }

  if (DataSize != 0)
  {
    RequestData = pMsgContext->reqData;
  }
  else
  {
    RequestData = NULL_PTR;
  }

#if (DCM_INCLUDE_RTE == STD_ON)
  SupplNotifStatus = E_OK;
  /* Execute the  Xxx_SupplierIndication() for all configured ports */
  /* Deviation TASKING-1 */
  for (i = 0U; i < DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE; ++i)
  {
    NotificationRetVal =
    Dcm_RteSupplierNotificationTable[i].Indication
                                    (Sid, RequestData, DataSize, ReqType, SourceAddress, &curNrc);
    /* If at least one of the Xxx_SupplierIndication() functions returns
     * DCM_E_REQUEST_NOT_ACCEPTED, the Request Indication
     * Status shall be DCM_E_REQUEST_NOT_ACCEPTED */
    if ( NotificationRetVal == DCM_E_REQUEST_NOT_ACCEPTED )
    {
      SupplNotifStatus = DCM_E_REQUEST_NOT_ACCEPTED;
      break;
    }

    else
    {
      if (NotificationRetVal == E_NOT_OK)
      {
        /* check if we have a misbehaving Indication() operation that violates Dcm518 and Dcm693 */
        if (curNrc == DCM_E_OK)
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_SUPPLIERNOTIFICATION,
            DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
        else
        {
          aggregatedNrc = curNrc;
        }
        SupplNotifStatus = E_NOT_OK;
      }
      /* If RequestIndicationRetVal == E_OK, then the previous value of
      NotificationStatus shall not be modified. */
    }
  }
#else /* #if (DCM_INCLUDE_RTE == STD_ON) */
  SupplNotifStatus = Rte_DcmSupplierNotification(Sid, RequestData, DataSize,
                                                 ReqType, SourceAddress, &curNrc);

  /* check if we have a misbehaving Rte_DcmSupplierNotification()
   * function that violates Dcm518 and Dcm693 */
  if ((SupplNotifStatus == E_NOT_OK) && (curNrc == DCM_E_OK))
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_SUPPLIERNOTIFICATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    aggregatedNrc = curNrc;
  }
#endif /* #if (DCM_INCLUDE_RTE == STD_ON) */

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */

  switch (SupplNotifStatus)
  {
    case DCM_E_REQUEST_NOT_ACCEPTED:
#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
      if ((DCM_HSM_INFO(instIdx).rpCntr == 0U) ||
          (CurHsmInfo->serviceId == DCM_SID_CLEAR_DTC) ||
          (CurHsmInfo->serviceId == DCM_SID_REQUEST_VEHICLE_INFORMATION))
#else
      if (DCM_HSM_INFO(instIdx).rpCntr == 0U)
#endif
      {
        /* discard request */
        /* !LINKSTO Dcm.EB.RequestVerification.SupplierNotificationReturnsRequestNotAccepted.NoRCRRPSent.Discard,1 */
        /* !LINKSTO Dcm.EB.RequestVerification.SupplierNotificationReturnsRequestNotAccepted.OBD0x04.RCRRPSent.Discard,1 */
        /* !LINKSTO Dcm.EB.RequestVerification.SupplierNotificationReturnsRequestNotAccepted.OBD0x09.RCRRPSent.Discard,1 */
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm,
                                      DCM_CUR_INST_ID(instIdx),
                                      DCM_HSM_DCM_EV_PROC_ABORT);
      }
      else
      {
        /*  send negative response */
        /* !LINKSTO Dcm.EB.RequestVerification.SupplierNotificationReturnsRequestNotAccepted.RCRRPSent.TransmitCNC,1 */
        CurHsmInfo->negativeResponseCode = DCM_E_CONDITIONSNOTCORRECT;

        (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm,
                                            DCM_CUR_INST_ID(instIdx),
                                            DCM_HSM_DCM_EV_PROC_DONE);
      }
      break;

    case E_NOT_OK:

      CurHsmInfo->negativeResponseCode = aggregatedNrc;

      (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
        DCM_HSM_DCM_EV_PROC_DONE);
      break;

    default:
      /* Do Nothing */
      break;
  }

  return SupplNotifStatus;
}
#endif /* #if(DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON )*/


STATIC FUNC(void, DCM_CODE) Dcm_DsdInternal_HandleReqMsgType
(
    Dcm_RequestTypeType RequestType,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) curMsgContext,
    P2VAR(Dcm_BufferUsageType, AUTOMATIC, DCM_VAR) TxBufferUse
)
{
  if (DCM_IS_PHYSICAL_REQUEST(RequestType))
  {
  /* set addressing type (functional, physical) */
    curMsgContext->msgAddInfo.reqType = DCM_PHYSICAL_ADDRESSING;
  }
  else
  {
    /* set addressing type (functional, physical) */
    curMsgContext->msgAddInfo.reqType = DCM_FUNCTIONAL_ADDRESSING;
  }

  /* Determine the type of processing this protocol has to undertake and
     allocate the TxBuffer for the specific job.

     Note: a TxBuffer will be in a "processing" state until it is actually
           ready to be handed over to a TxConnection for transmission of its
           enclosed data. */

  if ((DCM_NORMAL_PHYSICAL_REQUEST == RequestType) ||
      (DCM_NORMAL_FUNCTIONAL_REQUEST == RequestType))
  {
    *TxBufferUse = DCM_BUFFER_TX_NORMAL;
    curMsgContext->msgAddInfo.requestOrigin = DCM_EXTERNAL;
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
      DCM_IS_ROE_REQUEST(RequestType),DCM_INTERNAL_API_ID);
    *TxBufferUse = DCM_BUFFER_TX_ROE;
    curMsgContext->msgAddInfo.requestOrigin = DCM_INTERNAL_ROE;
  }
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
