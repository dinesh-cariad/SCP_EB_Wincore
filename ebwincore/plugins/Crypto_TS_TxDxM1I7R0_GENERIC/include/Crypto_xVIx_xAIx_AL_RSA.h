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

#ifndef CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_H
#define CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_H

#if(STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED)

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

/** \brief Helper for Sync SingleCall.
 *
 *  This function performes RSA verification with processing type Synchronus and
 *  mode SingleCall.
 *
 * \param[in] InputPtr         Input data
 * \param[in] InputLength      Input data length
 * \param[in] signaturePtr     buffer containing the signature.
 * \param[in] signatureLength  Signature length.
 * \param[in] Key              Key to be used for signature verification.
 * \param[in] VerifyPtr        Pointer containing the verification result.
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 * \retval E_NOT_OK       If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Helper_SyncSingleCall
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32 InputLength,
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
);

/** \brief AL Start Function for RSA verify
 *
 * This Function provides the RSA Start Function to call from the upper Layer.
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
Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for RSA verify
 *
 * This Function provides the RSA Update Function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 * \retval E_NOT_OK       If no RSA computation has been started yet.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for RSA verify
 *
 * This Function provides the RSA Finish Function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 * \retval _E_NOT_OK      If no RSA computation has been started yet.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_APPL_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for RSA verify.
 *
 * This Function provides the RSA Cancel Function to call from the upper layer.
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
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Perform the RSASSA-PSS-Verify tasks.
 *
 * This function performs the actual RSASSA-PSS signature verification
 * tasks which has been requested by the service functions.
 *
 * \param[out] Result  Pointer where to store the Result of the processing.
 * \param[out] Busy    Pointer to where to store if Main Function is busy.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_APPL_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED) */

#endif /* CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_H */

/*==[end of file]=================================================================================*/

