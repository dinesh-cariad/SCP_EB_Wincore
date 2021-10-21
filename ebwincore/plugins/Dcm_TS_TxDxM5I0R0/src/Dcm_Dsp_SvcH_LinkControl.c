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

/* !LINKSTO Dcm.Dsn.File.LC.Impl,1 */
/* This file contains the implementation of the Diagnostic Service LinkControl */

/*This file contains the implementation of the Diagnostic Service
  * Processing module in Dcm. */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_LinkControl.h>

#include <Dcm_Trace.h>
#include <TSAutosar.h>
#include <Dcm_Dsl_Supervisor.h>                                /* Dcm Dsl Supervisor header file */


#if(DCM_DSP_USE_SERVICE_0X87 == STD_ON)

/*===========================[macro definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON)
/* Dcm_LinkControlBaudrateTable is used to fetch BaudrateRecord for the service
 * LinkControl(0x87) */
/* Deviation MISRAC2012-1 */
STATIC CONST(Dcm_LinkControlBaudrateType, DCM_CONST) Dcm_LinkControlBaudrateTable[9U] = {
    {0x01, {0x00, 0x25, 0x80}}, {0x02, {0x00, 0x4B, 0x00}}, {0x03, {0x00, 0x96, 0x00}},
    {0x04, {0x00, 0xE1, 0x00}}, {0x05, {0x01, 0xC2, 0x00}}, {0x10, {0x01, 0xE8, 0x48}},
    {0x11, {0x03, 0xD0, 0x90}}, {0x12, {0x07, 0xA1, 0x20}}, {0x13, {0x0F, 0x42, 0x40}}
  };
#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x87
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x87
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if((DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON))
/** \brief UDS Subservice function for service 0x87
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[in]    BaudrateRecord Pointer to the array which holds the baudrate
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again. */

STATIC FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_Dsp_LinkControl_Execution(
  Dcm_OpStatusType  OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)  baudrateRecord,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if((DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Global variable holding the status information of LinkControl service Request Status */
VAR(boolean, DCM_VAR) Dcm_DspLinkControlRequestStatus;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
  )
{
#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(instIdx).ServiceTableIndex];

  /* get corresponding sub-service array configuration */
  CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
    = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

  /* Holds processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* get index of subservice function */
  const uint8 subSrvIdx = curHsmInfo->subServiceIdIdx;

  TS_PARAM_UNUSED(OpStatus);

  /* To check whether sub-function is available or not */
  DCM_POSTCONDITION_ASSERT(pMsgContext->reqDataLen > 0U, DCM_INTERNAL_API_ID);

  /* copy sub-function value to response buffer */
  pMsgContext->resData[0U] = curHsmInfo->subServiceId;
  pMsgContext->resDataLen = 1U;

  /* assign the data from msgcontext for service to that of sub-function.*/
  /* decrement the reqData length as sub-function is removed */
  pMsgContext->reqDataLen = pMsgContext->reqDataLen - 1U;

  if (pMsgContext->reqDataLen == 0U)
  {
    /* as no parameters exist after the sub-function, point the reqData to NULL */
    pMsgContext->reqData = NULL_PTR;
  }
  else
  {
    /* set request data pointer to received data without service and sub-service Ids */
    pMsgContext->reqData = &pMsgContext->reqData[1U];
  }

  if (curSsConfig[subSrvIdx].IntSrvHandler != NULL_PTR)
  {
    /* Call internal sub-service handler*/
    serviceProcessStatus = curSsConfig[subSrvIdx].IntSrvHandler(DCM_INITIAL, pMsgContext);
  }
  else
  {
    /* Call External sub-service handler*/
    serviceProcessStatus = curSsConfig[subSrvIdx].ExtSrvHandler(DCM_INITIAL, pMsgContext);

    /* Sub-service handlers return E_OK or E_NOT_OK, so we must convert to DCM_E_DONE */
    if (serviceProcessStatus == E_OK)
    {
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType nrc = E_OK;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  Std_ReturnType retVal;

  /* get the context information current instance */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(instIdx).ServiceTableIndex];

  /* get corresponding sub-service array configuration */
  CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
    = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

  /* get index of subservice function */
  const uint8 subSrvIdx = curHsmInfo->subServiceIdIdx;

  TS_PARAM_UNUSED(OpStatus);

  if (curSsConfig[subSrvIdx].IntSrvHandler != NULL_PTR)
  {
    retVal = Dcm_LinkControl(OpStatus, 0U, NULL_PTR, &nrc);

    switch (retVal)
    {
      case E_OK:
      {
        if((curHsmInfo->subServiceId == 1U) || (curHsmInfo->subServiceId == 2U))
        {
          Dcm_DspLinkControlRequestStatus = TRUE;
        }
        else
        {
          Dcm_DspLinkControlRequestStatus = FALSE;
        }
        /* Assemble positive response for LinkControl request */
        pMsgContext->resDataLen = (Dcm_MsgLenType)1U;
        pMsgContext->resData[0U] = curHsmInfo->subServiceId;
        serviceProcessStatus = DCM_E_DONE;
      }
      break;

      case DCM_E_PENDING:
        serviceProcessStatus = DCM_E_PENDING;
        break;

      /* Incase of validation of linkBaudrateRecord nrc will be updated with requestOutOfRange */
      case E_NOT_OK:
        /* nrc updated with DCM_E_CONDITIONSNOTCORRECT from Dcm_LinkControl callout */
        Dcm_ExternalSetNegResponse(pMsgContext, nrc);
        serviceProcessStatus = DCM_E_DONE;
        break;

      default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_LINKCONTROL, DCM_E_INTERFACE_RETURN_VALUE);
#endif
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        serviceProcessStatus = DCM_E_DONE;
        break;
    }
  }
  else
  {
    serviceProcessStatus = curSsConfig[subSrvIdx].ExtSrvHandler(OpStatus, pMsgContext);

    /* Sub-service handlers return E_OK or E_NOT_OK, so we must convert to DCM_E_DONE */
    if (serviceProcessStatus == E_OK)
    {
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  return serviceProcessStatus;
}

#if((DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON))
STATIC FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_Dsp_LinkControl_Execution(
  Dcm_OpStatusType  OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)  baudrateRecord,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* Get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* Sub-function of Link Control Request */
  Dcm_LinkControlRequestType linkControlType = curHsmInfo->subServiceId;
  Dcm_NegativeResponseCodeType nrc = E_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  retVal = Dcm_LinkControl(
                           OpStatus,
                            linkControlType,
                            baudrateRecord,
                            &nrc
                          );
  switch (retVal)
  {
    case E_OK:
      if((linkControlType == 1U) || (linkControlType == 2U))
      {
        Dcm_DspLinkControlRequestStatus =  TRUE;
      }
      else
      {
        Dcm_DspLinkControlRequestStatus =  FALSE;
      }

      /* Assemble positive response for LinkControl request */
      pMsgContext->resDataLen = 1U;
      pMsgContext->resData[0U] = curHsmInfo->subServiceId;
      serviceProcessStatus = DCM_E_DONE;
      break;

    case DCM_E_PENDING:
      serviceProcessStatus = DCM_E_PENDING;
      break;

    /* Incase of validation of linkBaudrateRecord nrc will be updated with requestOutOfRange */
    case E_NOT_OK:
      /* nrc updated with DCM_E_CONDITIONSNOTCORRECT from Dcm_LinkControl callout */
      Dcm_ExternalSetNegResponse(pMsgContext, nrc);
      serviceProcessStatus = DCM_E_DONE;
      break;

    default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      DCM_DET_REPORT_ERROR(DCM_SVCID_LINKCONTROL, DCM_E_INTERFACE_RETURN_VALUE);
#endif
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      serviceProcessStatus = DCM_E_DONE;
      break;
  }

  return serviceProcessStatus;
}
#endif /* #if((DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON)) */

/*==================[external function definitions]==============================================*/

/* This is the generic service handler for UDS service 0x87 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_LINKCONTROL_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      serviceProcessStatus = Dcm_Dsp_LinkControl_SvcStart(OpStatus, pMsgContext);
      break;
   /* second call when PENDING is returned */
    case DCM_PENDING:
      serviceProcessStatus = Dcm_Dsp_LinkControl_SvcCont(OpStatus, pMsgContext);
      break;
    /* communicate to callout of service cancellation */
    case DCM_CANCEL:
      (void)Dcm_LinkControl( OpStatus, 0U, NULL_PTR, NULL_PTR);
      serviceProcessStatus = DCM_E_DONE;
      break;

    default:
      serviceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_LINKCONTROL_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);
  return serviceProcessStatus;
}

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_FixedBaudrateVerification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  uint8 idx = 0U;
  /* Holds the baud rate value to be filled for the callout */
  uint8 baudrateRecord[3U];
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_ENTRY(OpStatus,pMsgContext);

  /* Request Length Verification */
  if (pMsgContext->reqDataLen != 1U)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }
  /* Validation of baudrateIdentifier */
  else if ( (pMsgContext->reqData[0U] < 0x01U) || ((pMsgContext->reqData[0U] > 0x05U) &&
            (pMsgContext->reqData[0U] < 0x10U)) || (pMsgContext->reqData[0U] > 0x13U) )
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    if(pMsgContext->reqData[0U] <= 5U)
    {
      idx = pMsgContext->reqData[0U] - 1U;
    }
    else
    {
      idx = pMsgContext->reqData[0U] - 0x0BU;
    }

    DCM_PRECONDITION_ASSERT(
      (pMsgContext->reqData[0U] == Dcm_LinkControlBaudrateTable[idx].baudrateIdentifier),
      DCM_INTERNAL_API_ID);

    /* Fetch baudrate from Baudrate table with baudrateIdentifier as index */
    baudrateRecord[0U] = Dcm_LinkControlBaudrateTable[idx].linkBaudrateRecord[0U];
    baudrateRecord[1U] = Dcm_LinkControlBaudrateTable[idx].linkBaudrateRecord[1U];
    baudrateRecord[2U] = Dcm_LinkControlBaudrateTable[idx].linkBaudrateRecord[2U];

    serviceProcessStatus = Dcm_Dsp_LinkControl_Execution(OpStatus, baudrateRecord,
                                                                 pMsgContext);
  }

  DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);
  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) */

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SpecificBaudrateVerification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* Holds the baud rate value to be filled for the callout */
  uint8 baudrateRecord[3U];
  Std_ReturnType serviceProcessStatus;

  DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_ENTRY(OpStatus,pMsgContext);

  /* Request Length Verification */
  if (pMsgContext->reqDataLen != 3U)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    baudrateRecord[0U] = pMsgContext->reqData[0U];
    baudrateRecord[1U] = pMsgContext->reqData[1U];
    baudrateRecord[2U] = pMsgContext->reqData[2U];

    serviceProcessStatus = Dcm_Dsp_LinkControl_Execution(OpStatus, baudrateRecord,
                                                               pMsgContext);
  }

  DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);
  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) */

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_TransitionBaudrate_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* Holds the baud rate value to be filled for the callout */
  uint8 baudrateRecord[3U];
  Std_ReturnType serviceProcessStatus;

  DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_ENTRY(OpStatus,pMsgContext);

  /* Request Length Verification */
  if (pMsgContext->reqDataLen > 0U)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }
  /* Validation of Sequence of service Request */
  else if ( Dcm_DspLinkControlRequestStatus == FALSE )
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTSEQUENCEERROR);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    serviceProcessStatus = Dcm_Dsp_LinkControl_Execution(OpStatus, baudrateRecord,
                                                         pMsgContext);
  }

  DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_EXIT(serviceProcessStatus,OpStatus,
    pMsgContext);
  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON) */

/* Initializes the control structures for the LinkControl (0x87) service */
FUNC(void, DCM_CODE) Dcm_Dsp_LinkControl_Init(void)
{
  DBG_DCM_DSP_LINKCONTROL_INIT_ENTRY();
  /* Initialize LinkControl Request status. */
  Dcm_DspLinkControlRequestStatus = FALSE;
  DBG_DCM_DSP_LINKCONTROL_INIT_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X87 == STD_ON) */

/*==================[end of file]================================================================*/
