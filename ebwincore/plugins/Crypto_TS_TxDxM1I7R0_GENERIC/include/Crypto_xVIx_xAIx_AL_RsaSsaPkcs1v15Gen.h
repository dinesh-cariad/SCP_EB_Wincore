/**
 * \file
 *
 * \brief AUTOSAR Crypto
 *
 * This file contains the implementation of the AUTOSAR
 * module Crypto.
 *
 * \version 1.7.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CRYPTO_XVIX_XAIX_AL_RSASSAPKCS1V15GEN_H
#define CRYPTO_XVIX_XAIX_AL_RSASSAPKCS1V15GEN_H

#if(STD_ON == CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED)

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Csm_Types.h>

/*==[Macros]======================================================================================*/

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief AL Start Function for RSASSA-PKCS1-v1_5 verify
 *
 * This Function provides the RSASSA-PKCS1-v1_5 Start Function to call from the upper Layer.
 *
 * \param[in] ObjId    The Driver object ID.
 * \param[in,out] Job  A pointer to the processed Job.
 *
 * \returns Error code.
 * \retval E_OK      If start was successfully requested.
 * \retval E_NOT_OK  If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for RSASSA-PKCS1-v1_5 verify
 *
 * This Function provides the RSASSA-PKCS1-v1_5 Update Function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 * \retval E_NOT_OK       If no RSASSA-PKCS1-v1_5 computation has been started yet.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for RSASSA-PKCS1-v1_5 verify
 *
 * This Function provides the RSASSA-PKCS1-v1_5 Finish Function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 * \retval _E_NOT_OK      If no RSASSA-PKCS1-v1_5 computation has been started yet.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for RSASSA-PKCS1-v1_5 verify.
 *
 * This Function provides the RSASSA-PKCS1-v1_5 Cancel Function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Result of the cancellation attempt.
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Perform the RSASSA-PKCS-Verify tasks.
 *
 * This function performs the actual RSASSA-PKCS signature verification
 * tasks which has been requested by the service functions.
 *
 * \param[out] Result  Pointer where to store the Result of the processing.
 * \param[out] Busy    Pointer to where to store if Main Function is busy.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED) */

#endif /* CRYPTO_XVIX_XAIX_AL_RSASSAPKCS1V15GEN_H */

/*==[end of file]=================================================================================*/

