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

#ifndef DCM_DSP_SVC_DDDI_H
#define DCM_DSP_SVC_DDDI_H

/* !LINKSTO Dcm.Dsn.File.DDDI.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service DynamicallyDefineDataIdentifier. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <Dcm_Hsm.h>

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)

#include <Dcm_Dsp_DidServices.h>

/*==================[macro definitions]==========================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x2C
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
/** \brief UDS service 0x2C-0x01
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcH (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_0X85_0X01_SSVC_ENABLED == STD_ON)*/

#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
/** \brief UDS service 0x2C-0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)*/

#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
/** \brief UDS service 0x2C-0x02
 **
 ** \param[in]  DDDidSrcTab Table of sources of the DDDID.
 ** \param[in]  dddidSize Current data length of the DDDID in bytes.
 ** \param[in]  nrSrcUsed The number of currently configured sources.
 ** \param[in]  pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_OK                         Conditions are ok
 ** \retval DCM_E_REQUESTOUTOFRANGE          Memory Range check failed
 ** \retval DCM_E_SECURITYACCESSDENIED       Security Access denied
 ** \retval NRC returned by DcmModeRule
 */
extern FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierAddMemSrcToDDDID(
  P2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab,
  uint16 dddidSize,
  uint8 nrSrcUsed,
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)*/

#if (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON)
/** \brief UDS service 0x2C-0x03
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifierClear_SSvcH (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_0X2C_0X03_SSVC_ENABLED == STD_ON)*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/
#endif /* #if (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */

#endif /* ifndef DCM_DSP_SVC_DDDI_H */
/*==================[end of file]================================================================*/
