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

#ifndef DCM_DSP_SVC_SECURITYACCESS_H
#define DCM_DSP_SVC_SECURITYACCESS_H

/* !LINKSTO Dcm.Dsn.File.SecurityAccess.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service SecurityAccess. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <Dcm_SecurityAccess_Cfg.h>

#if(DCM_DSP_USE_SERVICE_0X27 == STD_ON)

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.Type.SecurityAccess.DelayTimerType,1 */
/** \brief Security access delay timer related variables */
typedef struct
{
  /** \brief The delay timer that, while runnig, inhibts another attempt to access the current
  **         security level */
  VAR(uint32, TYPEDEF) DelayTimer;
  /** \brief Attempt counter keeping track how many consecutive failed attempts were made to access
   **        the current security level */
  VAR(uint8, TYPEDEF) AttemptCounter;
  /** \brief A flag indicating that the currently running delay timer is infinite i.e. it will
   **        never reach zero while the ECU is powered on. */
  VAR(boolean, TYPEDEF) InfiniteDelay;
} Dcm_Dsp_SecurityAccess_DelayTimerType;
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x27
 *
 *  \param OpStatus [in] OpStatus Operation state of the function.
 *  \param MsgContextPtr [inout] Message related information for one diagnostic
 *  protocol identifier.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr
);

/** \brief This function initializes the Security Access Service Handler Unit
 *
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_Init(void);

/** \brief This function resets the Security Access Service Handler state.
 *
 *  This function must be called from within a critical section only.
 *
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_Reset(void);
#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
/** \brief This function is called at each Dcm_MainFunction and handles the
 *         delay timers as well as the initialization of the attempt counters
 *
 *  This function must be called from within a critical section only.
 *
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_MainFunction(void);
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */

/** \brief This function performs requested actions upon session change.
 **
 **  \param[in] OldSession          Session level before change
 **  \param[in] NewSession          Session level after change
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_SecurityAccess_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X27 == STD_ON) */

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_SVC_SECURITYACCESS_H */
/*==================[end of file]================================================================*/
