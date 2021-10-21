/**
 * \file
 *
 * \brief AUTOSAR Csm
 *
 * This file contains the implementation of the AUTOSAR
 * module Csm.
 *
 * \version 3.1.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CSM_CBK_H
#define CSM_CBK_H

/* !LINKSTO CSM.Dsn.SWS_Csm_00727,1 */

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm_Version.h>
#include <Csm_Types.h>
#include <Csm_Cfg.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/** \brief  Notifies the CSM that a job has finished. This function is used by the underlying
 **         layer (CRYIF).
 **
 ** \param[in]  job     Holds a pointer to the job, which has finished.
 ** \param[in]  result  Contains the result of the cryptographic operation.
 **
 ** \ServiceID{::CSM_SID_CALLBACKNOTIFICATION}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(void, CSM_CODE) Csm_CallbackNotification
(
  P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
          Std_ReturnType                            result
);

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CSM_CBK_H */

/*==================[end of file]=================================================================*/

