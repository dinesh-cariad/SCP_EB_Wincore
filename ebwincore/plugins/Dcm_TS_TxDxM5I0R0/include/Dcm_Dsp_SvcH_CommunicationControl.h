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

#ifndef DCM_DSP_SVCH_COMMUNICATIONCONTROL_H
#define DCM_DSP_SVCH_COMMUNICATIONCONTROL_H

/* !LINKSTO Dcm.Dsn.File.CommunicationControl.PublicApi,1 */
/* This file contains the public API of the Diagnostic Service CommunicationControl */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_CommunicationControl_Cfg.h>  /* CommunicationControl configuration header file */
#include <Dcm_Int.h>                                              /* Module internal definitions */

#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                                                  /* BswM interface for Dcm */
#endif

#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief This function set the mode to DCM_ENABLE_RX_TX_NORM_NM in case at least one referenced
 ** arbitrary ModeDeclarationGroupPrototype is not fulfilled any more or the DCM is transitioning to
 ** default session or upon any diagnostic session change where the new session do not support
 ** UDS Service CommunicationControl any more.
 **
 ** \retval void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ResetCommunicationControl(void);

/** \brief The generic service handler for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if(DCM_0X28_0X00_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler EnableRxAndTx(0x00) for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_EnableRxAndTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X00_SSVC_ENABLED == STD_ON) */

#if(DCM_0X28_0X01_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler EnableRxAndDisableTx(0x01) for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 **
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_EnableRxAndDisableTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X01_SSVC_ENABLED == STD_ON) */

#if(DCM_0X28_0X02_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler DisableRxAndEnableTx(0x02) for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_DisableRxAndEnableTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X02_SSVC_ENABLED == STD_ON) */

#if(DCM_0X28_0X03_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler DisableRxAndTx(0x03) for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_DisableRxAndTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X03_SSVC_ENABLED == STD_ON) */

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)

#if(DCM_0X28_0X04_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler EnableRxAndDisableTxWithEnhancedAddressInformation(0x04)
 **        for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE)
              Dcm_Dsp_CommunicationControl_EnableRxAndDisableTxWithEnhancedAddrInfo_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X04_SSVC_ENABLED == STD_ON) */

#if(DCM_0X28_0X05_SSVC_ENABLED == STD_ON)
/** \brief Internal sub-service handler EnableRxAndTxWithEnhancedAddressInformation(0x05)
 **        for UDS service 0x28
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE)
                     Dcm_Dsp_CommunicationControl_EnableRxAndTxWithEnhancedAddrInfo_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_0X28_0X05_SSVC_ENABLED == STD_ON) */

#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */
/** \brief Initializes the mode of all ComM Channels that DCM uses and needed variables
 **
 ** \retval void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_Init(void);

#if (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON)
/** \brief This function determine if at least one referenced arbitrary
 **        ModeDeclarationGroupPrototype is not fulfilled any more and call
 **        Dcm_Dsp_CommunicationControl_ResetCommunicationControl
 **
 ** \retval void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ProcessModeRules(void);
#endif

#if((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON))
/** \brief This function is a setter for Dcm_CommunicationControl_ConfirmedCommunicationMode
 **
 ** \param[in]    CommunicationMode The communication mode that will be store in
 **               Dcm_CommunicationControl_ConfirmedCommunicationMode
 **
 ** \retval void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(
  Dcm_CommunicationModeType CommunicationMode);
#endif /* #if((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON)) */

/** \brief This function performs required actions upon session level change.
 **
 **  \param[in] OldSession          Session level before change
 **  \param[in] NewSession          Session level after change
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if(DCM_DSP_USE_SERVICE_0X28 == STD_ON) */
#endif /* ifndef DCM_DSP_SVCH_COMMUNICATIONCONTROL_H */
/*==================[end of file]================================================================*/
