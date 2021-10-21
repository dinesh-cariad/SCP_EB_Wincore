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

#ifndef DCM_DSP_SVC_ROE_H
#define DCM_DSP_SVC_ROE_H

/* !LINKSTO Dcm.Dsn.File.ROE.PublicApi,1 */
/* This file contains the public API of the Diagnostic Service ResponseOnEvent */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

#if (DCM_DSP_USE_SERVICE_0X86 == STD_ON)

#include <Dcm_Dsl_RoeServices.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x86
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON)
/** \brief UDS service 0x86-0x05
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)
/** \brief UDS service 0x86-0x45
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StartResponseOnEventPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON)
/** \brief UDS service 0x86-0x01
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON)
/** \brief UDS service 0x86-0x41
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangePersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 == STD_ON)
/** \brief UDS service 0x86-0x03
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 == STD_ON)
/** \brief UDS service 0x86-0x43
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON)
/** \brief UDS service 0x86-0x04
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_ReportActivatedEvents_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON)
/** \brief UDS service 0x86-0x00
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventNonPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)
/** \brief UDS service 0x86-0x40
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_StopResponseOnEventPersistent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 == STD_ON)
/** \brief UDS service 0x86-0x06
 **
 ** \param[in]    OpStatus Operation state for internal Dcm Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_ClearResponseOnEvent_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#endif /* #if (DCM_DSP_USE_ROESERVICES == STD_ON) */

#endif /* ifndef DCM_DSP_SVC_ROE_H */
/*==================[end of file]================================================================*/
