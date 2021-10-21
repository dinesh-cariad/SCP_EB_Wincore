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

#ifndef DCM_DSP_SVCH_LINKCONTROL_H
#define DCM_DSP_SVCH_LINKCONTROL_H

/* !LINKSTO Dcm.Dsn.File.LC.PublicApi,1 */
/* This file contains the public API of the Diagnostic Service LinkControl */

/*==================[inclusions]=================================================================*/


#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_Cfg.h>                                            /* Dcm configuration header file */
#include <Dcm_Int.h>                                              /* Module internal definitions */

#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                                                  /* BswM interface for Dcm */
#endif

#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#if(DCM_DSP_USE_SERVICE_0X87 == STD_ON)

/*==================[macros]=====================================================================*/

#if (defined DCM_SVCID_LINKCONTROL)
#error DCM_SVCID_LINKCONTROL already defined
#endif /* if (defined DCM_SVCID_LINKCONTROL) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_LINKCONTROL.
 */
#define DCM_SVCID_LINKCONTROL 0x4CU

/*==================[type definitions]===========================================================*/

/** \brief This type contains the information for Link Control service of the Dcm module.
 */
typedef struct
{
  uint8 baudrateIdentifier; /*< holds Fixed baudrate indentifier */
  uint8 linkBaudrateRecord[3]; /*< holds baudrate mapped to baudrate indentifier */
}
Dcm_LinkControlBaudrateType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the control structures for the LinkControl (0x87) service */
extern FUNC(void, DCM_CODE) Dcm_Dsp_LinkControl_Init(void);

/** \brief The generic service handler for UDS service 0x87
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON)

/** \brief The generic Subservice handler for UDS service 0x87-0x01
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_FixedBaudrateVerification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 == STD_ON) */

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON)

/** \brief The generic Subservice handler for UDS service 0x87-0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SpecificBaudrateVerification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 == STD_ON) */

#if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON)

/** \brief The generic Subservice handler for UDS service 0x87-0x03
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, It must be called again.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_TransitionBaudrate_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 == STD_ON) */


/** \brief Callout for service LinkControl.
 **
 ** This callout allows the user to control the communication baud rate of the ECU.
 **
 ** \param[in]     OpStatus Operation status
 ** \param[in]     LinkControlRequestType Sub-function identifier.
 ** \param[in]     BaudrateRecord Pointer to the array which holds the baudrate
 ** \param[in,out] ErrorCode NRC if the request for baudrate change fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::0x87}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_LinkControl(
  Dcm_OpStatusType  OpStatus,
  Dcm_LinkControlRequestType LinkControlRequestType,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)  BaudrateRecord,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Global variable holding the status information of LinkControl service Request Status */
extern VAR(boolean, DCM_VAR) Dcm_DspLinkControlRequestStatus;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X87 == STD_ON) */

#endif /* ifndef DCM_DSP_SVCH_LINKCONTROL_H */
/*==================[end of file]================================================================*/

