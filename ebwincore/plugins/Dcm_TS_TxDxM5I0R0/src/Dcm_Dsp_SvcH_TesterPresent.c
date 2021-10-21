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

/* This file contains the implementation of the Tester Present*/

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_TesterPresent.h>
#include <Dcm_Trace.h>
#include <TSAutosar.h>

#if(DCM_DSP_USE_SERVICE_0X3E == STD_ON)

/*===========================[macro definitions]=================================================*/

/*==================[type definitions]===========================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This global variable hold the type of the requested sub-service of
 **        TesterPresent(0x3E).
 **        This variable is used to determine if on invocation of the sent confirmation, the call
 **        to the internal sub-service is needed.
 **
 ** TRUE  - The requested TesterPresent(0x3E) sub-service is handled internally(by DCM).
 ** FALSE - The requested TesterPresent(0x3E) sub-service is handled externally
 **         (by application).
 */
STATIC VAR(boolean, DCM_VAR) Dcm_TesterPresent_SubFunctionHandlerIsInternal = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief This global variable hold the sub-service handler, extracted from
 **        TesterPresent(0x3E) request in the DCM_INITIAL state.
 **        This variable is used in each call of the TesterPresent handler to call the
 **        sub-service handler whether is external or internal.
 */
STATIC VAR(Dcm_GenericServiceHandlerType, DCM_VAR) Dcm_TesterPresent_SubServiceHandler;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>


/*=======================[internal function declarations]========================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x3E internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal DCM Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again**
 */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x3E internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal DCM Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

/*=======================[external function definitions]=========================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* This is the handler implementation for UDS service 0x3E */
/* !LINKSTO Dcm.Dsn.Comp.TesterPresentSvcH,1*/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_TESTERPRESENT_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_TesterPresent_SvcStart(DCM_INITIAL, pMsgContext);
      break;

    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_TesterPresent_SvcCont(DCM_PENDING, pMsgContext);
      break;

    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_TesterPresent_SvcCont(DCM_CANCEL, pMsgContext);
      Dcm_TesterPresent_SubFunctionHandlerIsInternal = FALSE;
      break;

    case DCM_CONFIRMED_OK:
    case DCM_CONFIRMED_NOK:
      ServiceProcessStatus = DCM_E_DONE;
      Dcm_TesterPresent_SubFunctionHandlerIsInternal = FALSE;
      Dcm_TesterPresent_SubServiceHandler = NULL_PTR;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch mode */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      ServiceProcessStatus = DCM_E_DONE;
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_TESTERPRESENT_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_ZeroSubFunction_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  TS_PARAM_UNUSED(OpStatus);

  DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_ENTRY(OpStatus,pMsgContext);

  Dcm_TesterPresent_SubFunctionHandlerIsInternal = TRUE;

  /* Length has been decreased by one within Dcm_Dsp_ExtractSubServiceHandler */
  if (pMsgContext->reqDataLen != 0U)
  {
    /* !LINKSTO Dcm.EB.TesterPresent.ZeroSubFunction.WrongMessageLength,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
  }
  else
  {
    /* Note: Requests for this service with a suppressed positive response arriving on
     * a functional RxPduId are treated in the Dsl and are not dispatched to the service handler */
    /* assemble response */
    pMsgContext->resData[0U] = 0U;
    pMsgContext->resDataLen = 1U;
  }

  ServiceProcessStatus = DCM_E_DONE;

  DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_EXIT(ServiceProcessStatus,OpStatus,
                                                            pMsgContext);
  return ServiceProcessStatus;
}

/*==================[internal function definitions]==============================================*/


STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_TESTERPRESENT_SVCSTART_ENTRY(pMsgContext);

  Dcm_TesterPresent_SubServiceHandler = Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_TesterPresent_SvcCont(OpStatus, pMsgContext);

  DBG_DCM_DSP_TESTERPRESENT_SVCSTART_EXIT(ServiceProcessStatus,pMsgContext);

  return ServiceProcessStatus;
}


STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_TESTERPRESENT_SVCCONT_ENTRY(pMsgContext);

  DCM_PRECONDITION_ASSERT(Dcm_TesterPresent_SubServiceHandler !=
                                                                      NULL_PTR, DCM_INTERNAL_API_ID);

  /* Call sub-service handler */
  ServiceProcessStatus = Dcm_TesterPresent_SubServiceHandler(OpStatus, pMsgContext);


  /* External Sub-service handlers return E_OK and E_NOT_OK, so must be converted to DCM_E_DONE */
  if( (Dcm_TesterPresent_SubFunctionHandlerIsInternal == FALSE)
      && ((ServiceProcessStatus == E_OK) || (ServiceProcessStatus == E_NOT_OK)))
  {
    ServiceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSP_TESTERPRESENT_SVCCONT_EXIT(ServiceProcessStatus,pMsgContext);

  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X3E == STD_ON) */
/*==================[end of file]================================================================*/
