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

#ifndef DCM_DSP_SVCH_REQUESTONBOARDMONITORINGTESTRESULT_H
#define DCM_DSP_SVCH_REQUESTONBOARDMONITORINGTESTRESULT_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#if(DCM_DSP_USE_SERVICE_0X06 == STD_ON)

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for OBD service 0x06
 **
 ** \param[in]    opStatus    Operation status of the service handler
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestOnBoardMonitoringTestResults_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X06 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_SVCH_REQUESTONBOARDMONITORINGTESTRESULT_H */
/*==================[end of file]================================================================*/

