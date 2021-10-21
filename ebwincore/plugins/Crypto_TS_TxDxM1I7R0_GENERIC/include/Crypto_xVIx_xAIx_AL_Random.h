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

#ifndef CRYPTO_XVIX_XAIX_AL_RANDOM_H
#define CRYPTO_XVIX_XAIX_AL_RANDOM_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if (                                                                                              \
      (CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED == STD_ON) ||                                          \
      (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED == STD_ON)                                         \
    )

/*==[Macros]======================================================================================*/

/*--SSG-GENERATE----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_0610,1 */
#if (defined CRYPTO_XVIX_XAIX_RANDOMGENERATE_RNG_NOT_SET_ALGORITHM)
#error CRYPTO_XVIX_XAIX_RANDOMGENERATE_RNG_NOT_SET_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SSG primitive which can be set in the CRYPTO_KE_RANDOM_ALGORITHM
 *         key element.
 */
#define CRYPTO_XVIX_XAIX_RANDOMGENERATE_RNG_NOT_SET_ALGORITHM 0x00U

#endif /* #if (CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED == STD_ON) */

/*--CTRDRBG---------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_0611,1 */
#if (defined CRYPTO_XVIX_XAIX_RANDOMGENERATE_AES_CTRDRBG_ALGORITHM)
#error CRYPTO_XVIX_XAIX_RANDOMGENERATE_AES_CTRDRBG already defined
#endif
/** \brief The algorithm id of the AES-CTRDRBG primitive which can be set in the
 *         CRYPTO_KE_RANDOM_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_RANDOMGENERATE_AES_CTRDRBG_ALGORITHM 0x01U

#endif /* #if (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED == STD_ON) */

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SSG-GENERATE----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED == STD_ON)

/** \brief Synchronous singlecall function for SSG random number generation
 *
 *  Performs the SSG random number generation synchronously in a single call.
 *
 *  \param[in]      ObjectId  Identifier of the crypto driver object that shall process the request.
 *  \param[in,out]  Job       Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key to be used is
 *                                         not available.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the
 *                                         wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_RNG_NOT_SET_SyncSingleCall
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for SSG random number generation
 *
 *  This function attempts to cancel the SSG random number generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in]      Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_RNG_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for SSG random number generation
 *
 *  This function provides the SSG random number generation main function to the upper layer.
 *
 *  \param[in,out] Result  Pointer to where the result of the processing should be stored.
 *  \param[in,out] Busy    After the function returns, this location indicates whether the
 *                         computation has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_RNG_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief Seed function for SSG random number generation
 *
 *  This function acquires entropy input and creates a seed from which the
 *  internal state key element for the SSG primitive is filled.
 *
 *  \param[in] CryptoKeyId     Holds the identifier of the key for which a new
 *                             seed shall be generated.
 *  \param[in] EntropyPtr      Holds a pointer to the memory location which
 *                             contains the data to feed the Seed (seed value).
 *  \param[in] EntropyLength   Contains the length of the entropy in bytes.
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful.
 *  \retval       E_NOT_OK                 Request failed.
 *
 *  \Reentrancy{Reentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_RNG_NOT_SET_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
);

#endif /* #if (CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED == STD_ON) */

/*--CTRDRBG---------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED == STD_ON)

/** \brief AES-CTRDRBG seed function
 *
 *  This function acquires entropy input and may combine it with
 *  a nonce and a personalization string to create a seed from which the
 *  initial internal state is created as described in NIST SP-800-90A.
 *
 *  \param[in] CryptoKeyId     Holds the identifier of the key for which a new
 *                             seed shall be generated.
 *  \param[in] EntropyPtr      Holds a pointer to the memory location which
 *                             contains the data to feed the Seed (seed value).
 *  \param[in] EntropyLength   Contains the length of the entropy in bytes.
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful.
 *  \retval       E_NOT_OK                 Request failed.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
);

/** \brief Synchronous singlecall function for AES-CTRDRBG random number generation
 *
 *  Performs the AES-CTRDRBG random number generation synchronously in a single call.
 *
 *  \param[in]     ObjectId       Identifier of the crypto driver object that shall
 *                                process the request.
 *  \param[in,out] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid.
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_SyncSingleCall
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for AES-CTRDRBG random number generation
 *
 *  This function provides the AES-CTRDRBG random number generation main function to
 *  the upper layer.
 *
 *  \param[in,out] Result  Pointer to where the result of the processing should be stored.
 *  \param[in,out] Busy    After the function returns, this location indicates whether the
 *                         computation has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief Cancel function for AES-CTRDRBG random number generation
 *
 *  This function attempts to cancel the AES-CTRDRBG random number generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in]      Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Helper SyncSingleCall Function for AES-CTRDRBG
 *
 *  This Function provides the AES-CTRDRBG SyncSingleCall Function to call from another primitive
 *
 * \param[in]     ObjectId        Identifier of the crypto driver object that processes the request.
 * \param[in]     Job             Reference to the calling Job.
 * \param[out]    ResultPtr       A pointer to where the result is to be stored
 * \param[in,out] ResultLengthPtr A pointer to the length of the random number to be generated.
 *
 * \returns Error code
 * \retval E_OK           Processing succeeded.
 * \retval E_NOT_OK       Processing failed.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_HelperSyncSingleCall
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)         ResultLengthPtr
);

#ifdef CRYPTO_RANDOM_CTRDRBG_USE_PREDEFINED
/** \brief Helper callout function for overwriting the generated random number with a predefined one.
 *
 *  This function provides a callout allowing for overwriting the generated random number with a
 *  predefined one. This callout will be implemented in the according test case.
 *
 * \param[in]    RandomNumberPtr       A pointer to the array where the random number is stored.
 * \param[in]    RandomNumberLength    The length of the random number.
 *
 * \returns Error code
 * \retval E_OK           Processing succeeded.
 * \retval E_NOT_OK       Processing failed.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_Predefinition
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          RandomNumberPtr,
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST)                        RandomNumberLength
);
#endif

#endif /* #if (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if ((CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED==STD_ON) || (CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED==STD_ON)) */

#endif /* CRYPTO_XVIX_XAIX_AL_RANDOM_H */

/*==[end of file]=================================================================================*/

