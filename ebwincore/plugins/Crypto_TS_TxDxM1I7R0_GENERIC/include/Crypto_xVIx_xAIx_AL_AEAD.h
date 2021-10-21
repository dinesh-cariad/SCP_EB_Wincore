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

#ifndef CRYPTO_XVIX_XAIX_AL_AEAD_H
#define CRYPTO_XVIX_XAIX_AL_AEAD_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if ((CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED == STD_ON))

/*==[Macros]======================================================================================*/

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--GCM-ENCRYPT---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED == STD_ON)

/** \brief Start function for AES GCM Generation
 *
 *  Starts the processing of an AES GCM Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADENCRYPT_AES_GCM_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for AES GCM Generation
 *
 *  Update function for the processing of an AES GCM Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADENCRYPT_AES_GCM_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for AES GCM Generation
 *
 *  Finishes the processing of an AES GCM Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADENCRYPT_AES_GCM_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for AES GCM Generation
 *
 *  This function attempts to cancel the AES GCM Generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 *  \returns    Result of the cancellation attempt
 *  \retval     E_OK                          Job was successfully cancelled.
 *  \retval     E_NOT_OK                      Job could not be cancelled.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADENCRYPT_AES_GCM_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for AES GCM Generation
 *
 *  This function provides the AES GCM Generation main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADENCRYPT_AES_GCM_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED == STD_ON) */

/*--GCM-DECRYPT-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED == STD_ON)

/** \brief Start function for AES GCM Verification
 *
 *  Starts the processing of an AES GCM Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADDECRYPT_AES_GCM_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for AES GCM Verification
 *
 *  Update function for the processing of an AES GCM Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADDECRYPT_AES_GCM_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for AES GCM Verification
 *
 *  Finishes the processing of an AES GCM Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADDECRYPT_AES_GCM_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for AES GCM Verification
 *
 *  This function attempts to cancel the AES GCM Verification.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 *  \returns    Result of the cancellation attempt
 *  \retval     E_OK                          Job was successfully cancelled.
 *  \retval     E_NOT_OK                      Job could not be cancelled.
 *  \retval     CRYPTO_E_JOB_CANCELED         Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADDECRYPT_AES_GCM_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for AES GCM Verification
 *
 *  This function provides the AES GCM Verification main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AEADDECRYPT_AES_GCM_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if ((CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED == STD_ON)) */

#endif /* CRYPTO_XVIX_XAIX_AL_AEAD_H */

/*==[end of file]=================================================================================*/

