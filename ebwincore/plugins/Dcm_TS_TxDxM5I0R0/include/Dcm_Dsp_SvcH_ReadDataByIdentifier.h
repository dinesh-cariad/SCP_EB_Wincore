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

#ifndef DCM_DSP_SVC_RDBI_H
#define DCM_DSP_SVC_RDBI_H

/* !LINKSTO Dcm.Dsn.File.RDBI.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service ReadDataByIdentifier. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <Dcm_Hsm.h>

#if(DCM_DSP_USE_SERVICE_0X22 == STD_ON)

#include <Dcm_Dsp_DidServices.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initialize the internal data of the ReadDataByIdentifier service
 **
 ** \param[in]    NONE
 **
 ** \return       NONE
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_Init(void);

/** \brief The generic service handler for UDS service 0x22
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_SVC_RDBI_H */
/*==================[end of file]================================================================*/
