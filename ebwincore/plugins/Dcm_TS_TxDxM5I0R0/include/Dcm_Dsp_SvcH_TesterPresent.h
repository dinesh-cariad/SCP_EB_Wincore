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

#ifndef DCM_DSP_SVCH_TESTERPRESENT_H
#define DCM_DSP_SVCH_TESTERPRESENT_H


/* This file contains the public API of the Tester Present */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm.h>                                                      /* Declaration of Dcm APIs */

#include <Dcm_Int.h>                                              /* Module internal definitions */

#if(DCM_DSP_USE_SERVICE_0X3E == STD_ON)
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x3E
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE           Operation completed
 ** \retval DCM_E_PENDNG         Operation did not complete, muss be called again
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Internal sub-service handler ZeroSubFunction(0x00) for UDS service 0x3E
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_ZeroSubFunction_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if(DCM_DSP_USE_SERVICE_0X3E == STD_ON) */
#endif /* ifndef DCM_DSP_SVCH_TESTERPRESENT_H */
/*==================[end of file]================================================================*/
