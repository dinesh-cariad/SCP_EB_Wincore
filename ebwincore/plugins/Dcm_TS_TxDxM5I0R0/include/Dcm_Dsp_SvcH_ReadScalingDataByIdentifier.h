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

#if (!defined DCM_DSP_SVC_RSDBI_H)
#define DCM_DSP_SVC_RSDBI_H

/* !LINKSTO Dcm.Dsn.File.RSDBI.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service ReadScalingDataByIdentifier. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                               /* AUTOSAR standard types */
#include <Dcm_Cfg.h>                                            /* Dcm configuration header file */
#include <Dcm_Int.h>                                              /* Module internal definitions */

#if(DCM_DSP_USE_SERVICE_0X24 == STD_ON)

#include <Dcm_Dsp_DidServices.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x24
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* if !defined( DCM_DSP_SVC_RSDBI_H ) */
/*==================[end of file]================================================================*/
