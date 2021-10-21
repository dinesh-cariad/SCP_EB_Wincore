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

#ifndef CRYPTO_XVIX_XAIX_INT_H
#define CRYPTO_XVIX_XAIX_INT_H

/* !LINKSTO EB_Crypto_00001, 1 */

/*==================[misra deviations]============================================================*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.
 *
 */

/*==================[inclusions]==================================================================*/

#include <Crypto_xVIx_xAIx_Int_Cfg.h>

/*==================[defensive programming macros]================================================*/

#if (defined CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT)
#error CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT is already defined
#endif

#if (defined CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_NO_EVAL)
#error CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT(                                     \
                           (Condition),                        \
                           CRYPTO_XVIX_XAIX_MODULE_ID,         \
                           CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                           (ApiId)                             \
                         )

/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL(                                     \
                                   (Condition),                        \
                                   CRYPTO_XVIX_XAIX_MODULE_ID,         \
                                   CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                                   (ApiId)                             \
                                 )
#else
#define CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT(Condition, ApiId)
#define CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT)
#error CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT is already defined
#endif

#if (defined CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_NO_EVAL)
#error CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT(                                     \
                            (Condition),                        \
                            CRYPTO_XVIX_XAIX_MODULE_ID,         \
                            CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                            (ApiId)                             \
                          )

/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL(                                     \
                                    (Condition),                        \
                                    CRYPTO_XVIX_XAIX_MODULE_ID,         \
                                    CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                                    (ApiId)                             \
                                  )
#else
#define CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT(Condition, ApiId)
#define CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CRYPTO_XVIX_XAIX_INVARIANT_ASSERT)
#error CRYPTO_XVIX_XAIX_INVARIANT_ASSERT is already defined
#endif

#if (defined CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_NO_EVAL)
#error CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT(                                     \
                        (Condition),                        \
                        CRYPTO_XVIX_XAIX_MODULE_ID,         \
                        CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                        (ApiId)                             \
                      )

/** \brief Report an assertion violation to Det
 *  \param[in] Condition The condition which is verified
 *  \param[in] ApiId The service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL(                                     \
                                (Condition),                        \
                                CRYPTO_XVIX_XAIX_MODULE_ID,         \
                                CRYPTO_XVIX_XAIX_INSTANCE_ID,       \
                                (ApiId)                             \
                              )
#else
#define CRYPTO_XVIX_XAIX_INVARIANT_ASSERT(Condition, ApiId)
#define CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CRYPTO_XVIX_XAIX_STATIC_ASSERT)
# error CRYPTO_XVIX_XAIX_STATIC_ASSERT is already defined
#endif
#if (CRYPTO_XVIX_XAIX_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 *  \param[in] Condition Condition which is violated
 */
# define CRYPTO_XVIX_XAIX_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define CRYPTO_XVIX_XAIX_STATIC_ASSERT(expr)
#endif

#if (defined CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT)
#error CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 *  \param[in] ApiId Service ID of the API function
 */
#define CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(CRYPTO_XVIX_XAIX_MODULE_ID, CRYPTO_XVIX_XAIX_INSTANCE_ID, (ApiId))
#else
#define CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined CRYPTO_XVIX_XAIX_INTERNAL_API_ID)
#error CRYPTO_XVIX_XAIX_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define CRYPTO_XVIX_XAIX_INTERNAL_API_ID DET_INTERNAL_API_ID

/*==================[macros]======================================================================*/

#if (defined CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE)
#error CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE is already defined
#endif
/** \brief Macro to indicate that crypto driver object is currently performing a cryptographic
 *         operation
 */
#define CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE               TRUE

#if (defined CRYPTO_XVIX_XAIX_DRIVER_OBJECT_INACTIVE)
#error CRYPTO_XVIX_XAIX_DRIVER_OBJECT_INACTIVE is already defined
#endif
/** \brief Macro to indicate that crypto driver object is currently not performing a cryptographic
 *         operation
 */
#define CRYPTO_XVIX_XAIX_DRIVER_OBJECT_INACTIVE             FALSE

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>


#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief Init function of the Process Layer
 *
 *  This function initializes the process layer of the crypto driver and forwards the initialization
 *  request to the abstraction layer
 *
 * \returns  void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_Init
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON)
/** \brief Main function of the Process Layer
 *
 *  This function is called by the Crypto_xVIx_xAIx_MainFunction and triggers the Main functions of
 *  the different primitives.
 *
 * \returns  void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_MainFunction
(
  void
);
#endif /* CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON */

/* !LINKSTO EB_Crypto_01019, 1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief Set a key element
 *
 *  This function forwards the key setting request to the abstraction layer
 *
 *  \param[in] CryptoKeyId   Identifier of the crypto key whose key element shall be set
 *  \param[in] KeyElementId  Identifier of the key element that shall be set
 *  \param[in] KeyPtr        Pointer to the key data that shall be set as the key element
 *  \param[in] keyLength     Length of the key element in bytes
 *
 *  \returns  Result of the key setting operation
 *  \retval   E_OK                             Request successful
 *  \retval   E_NOT_OK                         Request failed
 *  \retval   CRYPTO_E_KEY_WRITE_FAIL          Write access was denied
 *  \retval   CRYPTO_E_KEY_NOT_AVAILABLE       The requested key is not available
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementSet
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32 KeyLength
);

/** \brief Set the key's state to valid
 *
 *  This function forwards the request to set a key's state to valid to the abstraction layer
 *
 *  \param[in] CryptoKeyId   Identifier of the crypto key whose key element shall be set
 *
 *  \returns  Result of the key setting operation
 *  \retval   E_OK      Request successful
 *  \retval   E_NOT_OK  Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyValidSet
(
  uint32 CryptoKeyId
);

/** \brief   Retrieve the value of a key element
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId       Identifier of the crypto key whose element shall be retrieved
 *  \param[in]  KeyElementId      Identifier of the key element that shall be retrieved
 *  \param[out] ResultPtr         Pointer to the memory location where the key element data shall
 *                                be stored
 *  \param[in,out] ResultLengthPtr Pointer to the location where the length information is stored.
 *                                Shall contain the amount of bytes that shall be read. After
 *                                finishing the request, it contains the amount of bytes that has
 *                                been stored in the result.
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *  \retval     CRYPTO_E_KEY_READ_FAIL      Read access was denied
 *  \retval     CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 *  \retval     CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementGet
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
);

/** \brief      Copy a key element
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId                 Identifier of the key whose element shall be copied
 *  \param[in]  KeyElementId                Identifier of the key element that shall be copied
 *  \param[in]  TargetCryptoKeyId           Identifier of the destination key
 *  \param[in]  TargetKeyElementId          Identifier of the destination key element
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *  \retval     CRYPTO_E_KEY_READ_FAIL      Read access was denied
 *  \retval     CRYPTO_E_KEY_WRITE_FAIL     Write access was denied
 *  \retval     CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 *  \retval     CRYPTO_E_KEY_SIZE_MISMATCH  Key element sizes are not compatible
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
);

/** \brief      Copy all key elements of a key
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId                 Identifier of the key whose elements shall be copied
 *  \param[in]  TargetCryptoKeyId           Identifier of the destination key
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *  \retval     CRYPTO_E_KEY_READ_FAIL      Read access was denied
 *  \retval     CRYPTO_E_KEY_WRITE_FAIL     Write access was denied
 *  \retval     CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 *  \retval     CRYPTO_E_KEY_SIZE_MISMATCH  Key element sizes are not compatible
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief      Generate internal seed state for requested key
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId              Identifier of the crypto key for which a seed shall be
 *                                       generated
 *  \param[in]  EntropyPtr               Pointer to the memory location which contains the entropy
 *                                       data
 *  \param[in]  EntropyLength            Length of the entropy in bytes
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_RandomSeed
(
  uint32 CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32 EntropyLength
);

/** \brief   Generate a new key
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId              Identifier of the crypto key for which key material shall
 *                                       be generated
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyGenerate
(
  uint32 CryptoKeyId
);

/** \brief   Derive a key from another key
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]  CryptoKeyId              Identifier of the crypto key that shall be used for
 *                                       derivation
 *  \param[in]  TargetCryptoKeyId        Identifier of the crypto key where the derived key shall be
 *                                       stored
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief   Calculate the public value for the key exchange
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the crypto key that shall be used for the
 *                                         key exchange
 *  \param[out]   PublicValuePtr           Pointer to the memory location where the public value
 *                                         shall be stored
 *  \param[in,out] PublicValueLengthPtr     Pointer to the memory location where the length
 *                                         information shall be stored. On calling this function,
 *                                         this location shall contain the size of the provided
 *                                         buffer.
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 *  \retval       CRYPTO_E_SMALL_BUFFER    The provided buffer is too small to store the result
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyExchangeCalcPubVal
(
  uint32 CryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
);

/** \brief   Calculate the shared secret for the key exchange
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the crypto key that shall be used for the
 *                                         key exchange
 *  \param[out]   PartnerPublicValuePtr    Pointer to the memory where the public value of the
 *                                         partner is located
 *  \param[in]    PartnerPublicValueLength Length of the partner's public value in bytes
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 *  \retval       CRYPTO_E_SMALL_BUFFER    The provided buffer is too small to store the result
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyExchangeCalcSecret
(
  uint32 CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32 PartnerPublicValueLength
);

/** \brief   Parse a certificate
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the key that contains the certificate
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_CertificateParse
(
  uint32 CryptoKeyId
);

/** \brief   Verify a certificate
 *
 *  This function forwards the request to the abstraction layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the key that shall be used for verification
 *  \param[in]    VerifyCryptoKeyId        Identifier of the key that contains the certificate
 *  \param[out]   VerifyPtr                Pointer to the memory location where the result of the
 *                                         verification shall be stored
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_CertificateVerify
(
  uint32 CryptoKeyId,
  uint32 VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief   Enqueue a job into the queue
 *
 *  This function stores the job within a Queue element and stores the Queue element within the
 *  Queue according to the job priority . If the Queue is full, this function will record an error.
 *
 *  \param[in]  Queue                Queue in which the job shall be inserted
 *  \oaram[in]  Job                  Job that shall be put into the Queue
 *  \param[in]  ProcessFunction      Pointer to the function that shall be called when the job is
 *                                   dequeued
 *
 *  \returns    Result of the queueing attempt
 *  \retval     E_OK                 Job was successfully put into the Queue
 *  \retval     E_NOT_OK             Request failed, Job was not put into the Queue
 *  \retval     CRYPTO_E_QUEUE_FULL  Job could not be enqueued, because the Queue is full
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EnqueueJob
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Crypto_xVIx_xAIx_ProcessFuncPtr_t ProcessFunction
);
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief   Dequeue a job from the queue
 *
 *  This function removes the job with the highest priority (first element) from the Queue
 *
 *  \param[in]  Queue                Queue from which the job shall be dequeued
 *
 *  \returns    Dequeued Element, if the Queue contains at least one element. Otherwise NULL_PTR is
 *              returned.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
/* Deviation MISRAC2012-1*/
extern FUNC(FUNC_P2VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DequeueElement
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue
);
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief   Remove the referenced job from the queue
 *
 *  This function will search the referenced job in the Queue. If it is part of the Queue, the job
 *  is removed.
 *
 *  \param[in]  Queue                Queue from which the job shall be removed
 *  \param[in]  Job                  Job that shall be removed
 *
 *  \returns    Result of the request
 *  \retval     E_OK                 Job was successfully removed
 *  \retval     E_NOT_OK             Job is currently not enqueued within the referenced Queue
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RemoveJob
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CRYPTO_XVIX_XAIX_INT_H */

/*==================[end of file]=================================================================*/

