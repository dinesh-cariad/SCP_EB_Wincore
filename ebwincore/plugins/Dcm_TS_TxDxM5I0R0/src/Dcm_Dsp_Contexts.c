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

/* Dcm.Dsn.File.DspContexts.Impl,1 */
/* This file contains the implementation of the implementation of the DidServices software unit. */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

#include <Dcm_Types.h>
#include <Dcm_Dsp_Contexts.h>

/*===========================[macro definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsp_Context_Init(
  Dcm_Dsp_ContextPtr BaseContext,
  P2VAR(void, AUTOMATIC, DCM_VAR) SpecialPtr,
  P2CONST(Dcm_Dsp_ContextFunctionType, AUTOMATIC, DCM_CONST) FunctionConfig)
{
  DBG_DCM_DSP_CONTEXT_INIT_ENTRY(BaseContext, SpecialPtr, FunctionConfig);

  BaseContext->current_state = 0U;
  BaseContext->next_state    = 0U;
  BaseContext->FunctionConfig = FunctionConfig;
  BaseContext->SpecialPtr = SpecialPtr;
  BaseContext->opstatus = DCM_INITIAL;
  BaseContext->result   = DCM_E_ABORT;

  DBG_DCM_DSP_CONTEXT_INIT_EXIT(BaseContext, SpecialPtr, FunctionConfig);
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_Context_Execute(Dcm_Dsp_ContextPtr BaseContext)
{
  DBG_DCM_DSP_CONTEXT_EXECUTE_ENTRY(BaseContext);
  /* step through the configured states */
  do
  {
    /* advance the state */
    BaseContext->current_state = BaseContext->next_state;
    /* call the configured state function */
    BaseContext->FunctionConfig[BaseContext->current_state](BaseContext);

  } while (BaseContext->current_state != BaseContext->next_state);

  DBG_DCM_DSP_CONTEXT_EXECUTE_EXIT(BaseContext);
  return BaseContext->result;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_Context_Cancel(Dcm_Dsp_ContextPtr BaseContext)
{
  Std_ReturnType result = DCM_DSP_CONTEXT_RESULT(BaseContext);

  DBG_DCM_DSP_CONTEXT_CANCEL_ENTRY(BaseContext);

  /* check if an async operation is running */
  if (DCM_E_PENDING == result)
  {
    /* update the OpStatus to CANCEL */
    DCM_DSP_CONTEXT_OPSTATUS(BaseContext) = DCM_CANCEL;
    /* let the current async state cancel the operation */
    result = Dcm_Dsp_Context_Execute(BaseContext);
  }

  DBG_DCM_DSP_CONTEXT_CANCEL_EXIT(result, BaseContext);
  return result;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
