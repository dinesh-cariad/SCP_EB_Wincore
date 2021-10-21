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

#ifndef DCM_DSP_SVC_CONTROLDTCSETTINGL_H
#define DCM_DSP_SVC_CONTROLDTCSETTINGL_H

/* !LINKSTO Dcm.Dsn.File.ControlDTCSetting.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service ControlDTCSetting. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <Dcm_Dsp_DemFacade.h>

#if(DCM_DSP_USE_SERVICE_0X85 == STD_ON)
/*==================[macros]=====================================================================*/

/** \brief Value definition for 'disable Dtc setting' */
#if (defined DCM_DSP_DTCSETTING_DISABLE)
  #error "DCM_DSP_DTCSETTING_DISABLE is already defined"
#endif
#define DCM_DSP_DTCSETTING_DISABLE ((uint8) 0U)

/** \brief Value definition for 'enable Dtc setting' */
#if (defined DCM_DSP_DTCSETTING_ENABLE)
  #error "DCM_DSP_DTCSETTING_ENABLE is already defined"
#endif
#define DCM_DSP_DTCSETTING_ENABLE  ((uint8) 1U)

/*==================[type definitions]===========================================================*/

/** \brief Abstract data type for 'Dtc setting' control values */
typedef uint8 Dcm_Dsp_DtcSettingControlType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x85
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_0X85_0X01_SSVC_ENABLED == STD_ON)
/** \brief UDS subservice 0x85-0x01/0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_On_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (DCM_0X85_0X01_SSVC_ENABLED == STD_ON) */

#if (DCM_0X85_0X02_SSVC_ENABLED == STD_ON)
/** \brief UDS subservice 0x85-0x01/0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_Off_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (DCM_0X85_0X02_SSVC_ENABLED == STD_ON) */

/** \brief Initializes the ControlDTCSetting unit
 **
 ** \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSettingState_Init(void);

/** \brief This function re-enable the ControlDTCSetting in the DCM module, by calling the static
 **        funtion SetState with ControlDTCSettingState parameter set to DCM_DSP_DTCSETTING_ENABLE
 **        and also call the ModeSwitch function if it is supported
 **
 ** \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_ReEnable(void);

/** \brief This function provides the current state of ControlDTCSetting.
 **
 ** \retval DCM_DSP_DTCSETTING_ENABLE       ContolDTCSetting is enable
 ** \retval DCM_DSP_DTCSETTING_DISABLE      ContorlDTCSetting is disable
 **/
extern FUNC(Dcm_Dsp_DtcSettingControlType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_GetState(void);

/** \brief This function provides the mechanism of recall of ReEnable function in the next
 **        main cycle. Evaluates the DTCSetting ModeRule and re-enables DTCSetting if
 **        it fails, also retry to re-enable DTCSetting if the DemFacade was locked.
 **
 ** \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_MainFunction(void);

/** \brief This function performs requested actions upon session level change.
 **
 **  \param[in] OldSession          Session level before change
 **  \param[in] NewSession          Session level after change
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SessionChange(
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

#endif /* #if(DCM_DSP_USE_SERVICE_0X85 == STD_ON) */
#endif /* ifndef DCM_DSP_SVC_CONTROLDTCSETTINGL_H */
/*==================[end of file]================================================================*/
