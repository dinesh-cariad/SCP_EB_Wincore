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

#ifndef CRYPTO_XVIX_XAIX_AL_MAC_H
#define CRYPTO_XVIX_XAIX_AL_MAC_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if ((CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON) || \
     (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED == STD_ON))

/*==[Macros]======================================================================================*/

/*--Crypto_xVIx_xAIx_HelperEncryptState------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_IDLE)
#error CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_IDLE already defined
#endif
/** \brief Idle state of the ECB Encrypt Helper. */
#define CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_PRIMITIVE)
#error CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_PRIMITIVE already defined
#endif
/** \brief Busy state of the Helper set by a Primitive. */
#define CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_PRIMITIVE 1U

#if (defined CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_KEY_MANAGEMENT)
#error CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_KEY_MANAGEMENT already defined
#endif
/** \brief Busy state of the Helper set by a KeyManagement-Function. */
#define CRYPTO_XVIX_XAIX_HELPER_STATE_HMAC_KEY_MANAGEMENT 2U

#if (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_MACGENERATE_SHA2_256_HMAC_ALGORITHM)
#error CRYPTO_XVIX_XAIX_MACGENERATE_SHA2_256_HMAC_ALGORITHM already defined
#endif
/** \brief The algorithm id of the HMAC SHA2 256 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_MACGENERATE_SHA2_256_HMAC_ALGORITHM 0x02U

#endif /* #if (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON) */

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--CMAC-GENERATE---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED == STD_ON)

/** \brief Start function for AES CMAC Generation
 *
 *  Starts the processing of an AES CMAC Generation job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for AES CMAC Generation
 *
 *  Update function for the processing of an AES CMAC Generation job.
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
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for AES CMAC Generation
 *
 *  Finishes the processing of an AES CMAC Generation job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Synchronous singlecall function for AES CMAC Generation
 *
 *  Performs the AES CMAC Generation synchronously in a single call.
 *
 *  \param[in]  ObjectId    Identifier of the crypto driver object that shall process the request.
 *  \param[in]  Job         Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is
 *                                         not valid.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the
 *                                         wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_SyncSingleCall
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for AES CMAC Generation
 *
 *  This function attempts to cancel the AES CMAC Generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for AES CMAC Generation
 *
 *  This function provides the AES CMAC Generation main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_AES_CMAC_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED == STD_ON) */

/*--CMAC-VERIFY-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON)

/** \brief Start function for AES CMAC Verification
 *
 *  Starts the processing of an AES CMAC Verification job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for AES CMAC Verification
 *
 *  Update function for the processing of an AES CMAC Verification job.
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
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for AES CMAC Verification
 *
 *  Finishes the processing of an AES CMAC Verification job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Synchronous singlecall function for AES CMAC Verification
 *
 *  Performs the AES CMAC Verification synchronously in a single call.
 *
 *  \param[in]  ObjectId    Identifier of the crypto driver object that shall process the request.
 *  \param[in]  Job         Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is
 *                                         not valid.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the
 *                                         wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_SyncSingleCall
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for AES CMAC Verification
 *
 *  This function attempts to cancel the AES CMAC Verification.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for AES CMAC Verification
 *
 *  This function provides the AES CMAC Verification main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_AES_CMAC_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON) */

#if ((CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED == STD_ON) || \
     (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON))
/** \brief Function to precalculate the subkeys K1 and K2 and the expanded AES key from a MAC key
 *         in case these elements are avaialble
 *
 *  \param[in] CryptoKeyId    Identifier of the crypto key. It must be a MAC key with key elements
 *                            CRYPTO_KE_MAC_KEY (mandatory),
 *                            CRYPTO_XVIX_XAIX_KE_AES_EXPANDEDKEY (optional),
 *                            CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY1 (optional) and
 *                            CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY2 (optional)
 *                            If an ecisting key could not be created, its internal length is set to
 *                            0.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AESCMAC_PrecalculateKeys
(
  uint32 CryptoKeyId
);
#endif

/*--HMAC-GENERATE---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON)
/** \brief Helper for calling Hmac Sha2 256 in mode synchronous singlecall.
 *
 * \param[in] InputPtr        Pointer to input data for Hmac Sha2 256
 * \param[in] InputLength     Length of the input data
 * \param[in] OutputPtr       Pointer to the output data of Hmac Sha2 256
 * \param[in] OutputLengthPtr Length of the output data
 * \param[in] KeyPtr          Pointer to the key

 * \return Result of the synchronous call of Hmac Sha2 256
 * \retval E_OK     Hmac Sha2 256 has been successfully processed
 * \retval E_NOT_OK Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_HelperHMacSha2256SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32 InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputLengthPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief Start function for SHA-256 HMAC Generation
 *
 *  Starts the processing of an SHA-256 HMAC Generation job.
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
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SHA2_256_HMAC_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for SHA-256 HMAC Generation
 *
 *  Update function for the processing of an SHA-256 HMAC Generation job.
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
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SHA2_256_HMAC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for SHA-256 HMAC Generation
 *
 *  Finishes the processing of an SHA-256 HMAC Generation job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SHA2_256_HMAC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for SHA-256 HMAC Generation
 *
 *  This function attempts to cancel the SHA-256 HMAC Generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SHA2_256_HMAC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for SHA-256 HMAC Generation
 *
 *  This function provides the SHA-256 HMAC Generation main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SHA2_256_HMAC_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON) */

/*--HMAC-VERIFY-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED == STD_ON)

/** \brief Start function for SHA-256 HMAC Verification
 *
 *  Starts the processing of an SHA-256 HMAC Verification job.
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
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SHA2_256_HMAC_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for SHA-256 HMAC Verification
 *
 *  Update function for the processing of an SHA-256 HMAC Verification job.
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
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SHA2_256_HMAC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for SHA-256 HMAC Verification
 *
 *  Finishes the processing of an SHA-256 HMAC Verification job.
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
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SHA2_256_HMAC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for SHA-256 HMAC Verification
 *
 *  This function attempts to cancel the SHA-256 HMAC Verification.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SHA2_256_HMAC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for SHA-256 HMAC Verification
 *
 *  This function provides the SHA-256 HMAC Verification main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SHA2_256_HMAC_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if ((CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON) || \
               (CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED == STD_ON)) */

#endif /* CRYPTO_XVIX_XAIX_AL_MAC_H */

/*==[end of file]=================================================================================*/

