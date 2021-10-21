/**
 * \file
 *
 * \brief AUTOSAR CryIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CryIf.
 *
 * \version 1.0.24
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CRYIF_CBK_H
#define CRYIF_CBK_H

/* !LINKSTO SWS_CryIf_00006, 1
 */

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <CryIf.h>

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define CRYIF_START_SEC_CODE
#include <CryIf_MemMap.h>

/**
 * \brief   Notifies the CryIf about the completion of the request with the result of the
 *          cryptographic operation.
 *
 * \param[in]    job    Holds a pointer to the job structure
 * \param[in]    result Contains the result of the cryptographic operation
 *
 * \ServiceID{::CRYIF_SID_CALLBACKNOTIFICATION}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Non Reentrant}
 */
extern FUNC(void, CRYIF_CODE) CryIf_CallbackNotification
(
  P2CONST(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job,
  Std_ReturnType result
);

#define CRYIF_STOP_SEC_CODE
#include <CryIf_MemMap.h>


#endif /* #ifndef CRYIF_CBK_H */

/*==================[end of file]=================================================================*/
