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

#ifndef DCM_DSP_SVC_RMBA_H
#define DCM_DSP_SVC_RMBA_H

/* !LINKSTO Dcm.Dsn.File.RMBA.PublicApi,1 */
/* This file contains the public API of the diagnostic service ReadMemoryByAddress */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x23
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in]    OpStatus Operation state of the function.
 **
 ** \retval DCM_PENDING           Operation did not complete, it must be called again
 ** \retval DCM_E_DONE            Operation completed
 */
/* This is the generic service handler for UDS service 0x23 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadMemoryByAddress_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X23 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_SVC_RMBA_H */
/*==================[end of file]================================================================*/
