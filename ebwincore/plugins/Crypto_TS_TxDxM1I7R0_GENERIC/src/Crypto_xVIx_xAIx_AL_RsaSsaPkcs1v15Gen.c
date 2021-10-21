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

/*==[Includes]================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15Gen.h>
#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_AL_SHA1.h>
#include <Crypto_xVIx_xAIx_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <TSMem.h>
#include <SchM_Crypto_xVIx_xAIx.h>

#if (STD_ON == CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED)

/*==[Macros]==================================================================*/

#if (defined CRYPTO_XVIX_XAIX_RSASSAV15GEN_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_RSASSAV15GEN_DEFAULT_CURRENT_MODE is already defined
#endif
/** \brief To set the default current mode. */
#define CRYPTO_XVIX_XAIX_RSASSAV15GEN_DEFAULT_CURRENT_MODE 0x00U

#if (defined CRYPTO_XVIX_XAIX_RSASSAV15_G_HASH_MAX_LEN)
#error CRYPTO_XVIX_XAIX_RSASSAV15_G_HASH_MAX_LEN is already defined
#endif
/** \brief The maximal allowed length for a hash digest in bytes. */
#define CRYPTO_XVIX_XAIX_RSASSAV15_G_HASH_MAX_LEN 64

#if (defined CRYPTO_XVIX_XAIX_RSASSAV15_G_EM_STATIC_FIELDS_LEN)
#error CRYPTO_XVIX_XAIX_RSASSAV15_G_EM_STATIC_FIELDS_LEN is already defined
#endif
/** \brief The length of the static EM fields, as described in Chapter 9.2 of the standard
 **        PKCS #1 v2.2: RSA Cryptography Standard
 **/
#define CRYPTO_XVIX_XAIX_RSASSAV15_G_EM_STATIC_FIELDS_LEN          11U

#if (defined CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_BYTE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_BYTE is already defined
#elif (8U > CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE is to small
#endif
/** \brief The maximum length in bytes of the RSA key (modulus + exponent)
 *         The size of two Tags(a 1Byte), two lengths (a 2Byte) and atleast 1 Byte
 *         modulo or exponent can be subtracted.
 */
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_BYTE \
(CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE - 7U)

#if (defined CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD is already defined
#endif
/** \brief The maximum length in words of the RSA key. */
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD \
((CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_BYTE / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) + 1U)

#if (defined CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG)
#error CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG is already defined
#endif
/** \brief The DER tag of a Sequence. */
#define CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG 0x30U

#if (defined CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG)
#error CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG is already defined
#endif
/** \brief The DER tag of a Integer. */
#define CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG 0x02U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_IDLE)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_IDLE is already defined
#endif

/* \brief The algorithm is in the idle state. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_IDLE 0x00U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INIT is already defined
#endif
/* \brief The initialization of the algorithm is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INIT 0x01U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT is already defined
#endif
 /* \brief The initialization of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT 0x02U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED is already defined
#endif
/* \brief The algorithm is initialized. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED 0x03U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_UPDATE)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_UPDATE is already defined
#endif
/* \brief The update operation of the algorithm is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_UPDATE 0x04U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE is already defined
#endif
/* \brief The update operation of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE 0x05U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH is already defined
#endif
/* \brief The finish operation of the algorithm is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH 0x06U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH is already defined
#endif
/* \brief The finish operation of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH 0x07U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_EM)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_EM is already defined
#endif
/* \brief The generation of the encoded message is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_EM 0x08U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_S)
#error CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_S is already defined
#endif
/* \brief The exponentiation of the encoded message is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_S 0x09U

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID is already defined
#endif
/** \brief  The X.509 algorithm identifier of the SHA1 hash algorithm. Trailing zeros are added to
            match the size of the Crypto_xVIx_xAIx_AlgorithmId. The required size is 11 only. */
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID        \
{                                                         \
  0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02,         \
  0x1a, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00                \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID_LENGTH)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID_LENGTH is already defined
#endif
/** \brief   The X.509 algorithm identifier length of the SHA-224 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID_LENGTH 11U

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-224 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x04U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID_LENGTH)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID_LENGTH is already defined
#endif
/** \brief   The X.509 algorithm identifier length of the SHA-224 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID_LENGTH 15U

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-256 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x01U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID_LENGTH)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID_LENGTH is already defined
#endif
/** \brief   The X.509 algorithm identifier length of the SHA-256 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID_LENGTH 15U

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-384 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x02U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID_LENGTH)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID_LENGTH is already defined
#endif
/** \brief   The X.509 algorithm identifier length of the SHA-384 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID_LENGTH 15U

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-512 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x03U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID_LENGTH)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID_LENGTH is already defined
#endif
/** \brief   The X.509 algorithm identifier length of the SHA-512 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID_LENGTH 15U

/*==[Types]===================================================================*/

/* \brief Enumeration of the internal states of the RSASSA-PKCS1-v1_5 signature verification. */
typedef uint8 Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenStateType;

/** \brief The private RSA key used for generating a signature.
 *
 * \var Crypto_xVIx_xAIx_RsaPrivateKeyType::Length
 * \brief Dummy element to make the type compatible with the Csm key type.
 *
 * \var Crypto_xVIx_xAIx_RsaPrivateKeyType::SecretExponent
 * \brief The private exponent of the RSA key.
 *
 * \var Crypto_xVIx_xAIx_RsaPrivateKeyType::Modulus
 * \brief The modulus of the RSA key.
 *
 * \var Crypto_xVIx_xAIx_RsaPrivateKeyType::Barrett
 * \brief The Barrett value (only used if Barrett reduction is enabled).
 *
 */
typedef struct
{
  uint32                      Length;
  Crypto_xVIx_xAIx_LNWordType SecretExponent[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_LNWordType Modulus[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD];
}
Crypto_xVIx_xAIx_RsaPrivateKeyType;

/**
 * \struct Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType
 * \brief Structure which contains the context of the RSASSA-PKCS1-v1_5 signature generation
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::DataLength
 * \brief The length of the hash value of the data.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::HashResultLength
 * \brief The length of the hash value of the data.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::RsaKey
 * \brief A pointer to the secret key which should be used for creating the signature.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::Result
 * \brief A pointer to the start of a buffer where the created signature bytes should be stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::ResultLengthPtr
 * \brief A pointer to a variable which contains the maximal allowed space for the signature
 *        and where the length of the created signature should be stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::Data
 * \brief A pointer to the start of an array where the data to be signed is stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::LNCtx
 * \brief The context which has to be used for long number arithmetic.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::Signature
 * \brief An array which is used as a buffer for the long number representation of the signature
 *        during signature generation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::Tmp1Value
 * \brief An array which is used as a buffer during signature generation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::State
 * \brief The current state the RSASSA-PKCS1-v1_5 signature generation is in.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::HashResult
 * \brief A pointer to the start of an array where the hash value of the data will be buffered
 *        during the computation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::EM
 * \brief An array which is used as a buffer for the value "EM" during the signature generation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::ObjId
 * \brief The driver object id.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::Job
 * \brief The currently processed job.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::ResultError
 * \brief The return value which will be used when calling the callback.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::IsBusy
 * \brief Flag which indicates if another MainFunction is needed.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::CurrentMode
 * \brief The currently processed mode.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::CancelFlag
 * \brief Flag that states if the job shall be canceld.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::CallbackFlag
 * \brief Flag that states if the callback shall be called.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType::CallbackResult
 * \brief The result which will be forwarded to the callback.
 *
 */
typedef struct
{
  uint32                                                                    DataLength;
  uint32                                                                    HashResultLength;
  Crypto_xVIx_xAIx_RsaPrivateKeyType                                        RsaKey;
  P2VAR(uint8,                        TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)  Result;
  P2VAR(uint32,                       TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultLengthPtr;
  P2CONST(uint8,                      TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)  Data;
  Crypto_xVIx_xAIx_LNCtxType                                                LNCtx;
  Crypto_xVIx_xAIx_LNWordType  Signature[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_LNWordType  Tmp1Value[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenStateType                               State;
  uint8                                 HashResult[CRYPTO_XVIX_XAIX_RSASSAV15_G_HASH_MAX_LEN];
  uint8                                 EM[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_BYTE];
  uint32                                                                    ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                Job;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)              ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     IsBusy;
  Crypto_OperationModeType                                                  CurrentMode;
  boolean                                                                   CancelFlag;
  boolean                                                                   CallbackFlag;
  Std_ReturnType                                                            CallbackResult;
}
Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType;

/*==[Declaration of functions with internal linkage]==========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
/**  \brief This function resets the Context of RSA Verify.
 *
 * \returns void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset
(
  void
);

/**  \brief This function calls the helper hash start function depending on the secondaryFamily.
 *
 * \param[in] Ctx  A pointer to the context.
 *
 * \returns void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}

 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
);

/**  \brief This function calls the helper hash update function depending on the secondaryFamily.
 *
 * \param[in] Ctx         A pointer to the context.
 * \param[in] DataPtr     A pointer to the data.
 * \param[in] DataLength  The length of the data.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                     DataPtr,
  uint32                                                                                    DataLength
);

/** \brief This function calls the helper hash finish function depending on the secondaryFamily.
 *
 * \param[in] Ctx        A pointer to the context.
 * \param[out] ResPtr    A pointer to where the result is to be stored.
 * \param[in] ResLenPtr  A pointer where the length of the result is to be stored.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                       ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                      ResLenPtr
);

/** \brief This function calls the helper hash finish function depending on the given
 *         AlgorithmFamily.
 *
 * \param[in] AlgorithmFamily  The algorithm family of the hash function.
 *
 * \returns void
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Mainfunction
(
  uint32 AlgorithmFamily
);

/** \brief This is the callback function of the underlying hash primitive.
 *
 * \param[in,out] Job     Reference to the currently processed job.
 * \param[in]     Result  The error value.
 *
 * \returns void
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief RSA verification callback function
 *
 *         This is the RSA verification callback function which is called,
 *         when the primitive is called asynchronously.
 *
 * \param[in,out] Job     Reference to the currently processed job.
 * \param[in]     Result  The error value.
 *
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief Helper function for the Callback and the cancel functionality.
 *
 *         This is a helper function intended to implement the callback and
 *         the job cancel functionality in a single function called in the
 *         main function.
 *
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCallbackCancel
(
  void
);

/** \brief Helper function for the
 *       Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_MainFunction function.
 *
 *         This is a helper function intended to combine the same functionality of different states
 *         together in a helper function to increase readability and imporve the quality metrices.
 *
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_DefaultState
(
  void
);

/** \brief Helper function for the finish state of the state machine.
 *
 *        This is a state machine helper function, which performes the needed tasks for the
 *        finish state.
 *
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_FinishState
(
  void
);

/** \brief Helper function for setting the AlgorithmId and the Algorithm length.
 *
 *        This function sets the algorithm id and the algorithm length to the passed pointers.
 *
 * \param[out] AlgorithmId         Pointer to a pointer which would point to the memory location
 *                                 containing the algorithmId.
 * \param[out] AlgorithmIdLength   pointer used to set the algorithm length.
 *
 * \returns void
 * \returns void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenSetAlgorithmId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA),AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) AlgorithmId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) AlgorithmIdLength
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Array of X.509 algorithm identifiers, which the RSASSA-PKCS1-v1_5 signature verification
  *  supports.
  */
  /* !LINKSTO EB_Crypto_CryptAlgos_1402,1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AlgorithmId[5U][15U] =
                                  {
                                    CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID,
                                    CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID,
                                    CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID,
                                    CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID,
                                    CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID
                                  };

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief RSASSA-PKCS1-v1_5 signature generation context.
 *
 * The context variable which is used to store the internal state of the
 * RSASSA-PKCS1-v1_5 signature generation.
 */
static VAR(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, CRYPTO_xVIx_xAIx_VAR)
       Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_0970, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean IsExponentValid = FALSE;
  boolean IsModuloValid = FALSE;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr;
  uint32 KeyLength = CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE;

  /* Check if configured secondaryFamily is implemented */
  if( ((boolean) FALSE)
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
      || ( Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ==
           CRYPTO_ALGOFAM_SHA2_224
         )
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
      || ( Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ==
           CRYPTO_ALGOFAM_SHA2_256
         )
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
      || ( Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ==
           CRYPTO_ALGOFAM_SHA2_384
         )
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
      || ( Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ==
           CRYPTO_ALGOFAM_SHA2_512
         )
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
      || ( Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ==
           CRYPTO_ALGOFAM_SHA1
         )
#endif
    )
  {
    if(
        E_OK == Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                (
                  Job->cryptoKeyId,
                  CRYPTO_KE_SIGNATURE_KEY,
                  &KeyPtr,
                  &KeyLength
                )
      )
    {
      /*This needs to be done because the Job pointer in the context is references in the
        Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset function to call the
        respective reset function of the secondary primitive.*/
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job = Job;
      /* !LINKSTO SWS_Crypto_00017, 1 */
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset();
      /* convert to rsa key type */
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.SecretExponent[0U] =
      CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD - 1U;
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.Modulus[0U] =
      CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD - 1U;
      if(
          E_OK == Crypto_xVIx_xAIx_AL_DecodeRsaKey
                  (
                    KeyPtr,
                    KeyLength,
                    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.Modulus,
                    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.SecretExponent
                  )
        )
      {
        KeyLength = CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE;
        /* check if key is valid */
        IsExponentValid = Crypto_xVIx_xAIx_LNIsValid
                          (
                            Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.SecretExponent,
                            CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD
                          );
        IsModuloValid = Crypto_xVIx_xAIx_LNIsValid
                        (
                          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.Modulus,
                          CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_RSA_MAX_KEY_SIZE_WORD
                        );
        if( ( TRUE == IsExponentValid ) && ( TRUE == IsModuloValid ) )
        {
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId = ObjectId;
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job = Job;
          RetVal = E_OK;
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INIT;
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
        }
      }
    }
  }
  else
  {
    /* Secondary primitive is not supported */
    RetVal = E_NOT_OK;
  }

  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if(CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED ==
     Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State
    )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Data = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.DataLength = Job->jobPrimitiveInputOutput.inputLength;

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_UPDATE;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if(
      ( CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED ==
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State
      ) &&
      (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode)
    )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Result = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultLengthPtr =
    Job->jobPrimitiveInputOutput.outputLengthPtr;
    /* Set state of the state machine to finish */
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED ==
     Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  uint32 i;
  uint32 NumberLength;
  uint32 AlgorithmIdLength = 0U;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) AlgorithmId = NULL_PTR;
  uint8              DerHeaderDigestBuffer[CRYPTO_XVIX_XAIX_COMMON_DER_HEADER_MAX_LEN];
  uint32             DerHeaderDigestLength;
  uint8              DerHeaderDigestInfoBuffer[CRYPTO_XVIX_XAIX_COMMON_DER_HEADER_MAX_LEN];
  uint32             DerHeaderDigestInfoLength;
  uint32             TLength;

  if(!Crypto_xVIx_xAIx_CheckSkipMain(Result, Busy, Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId))
  {
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError = E_OK;
    }
    switch(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State)
    {
      case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INIT:
      {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
        CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT;
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Start
        (
          &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx
        );
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_UPDATE:
      {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
        CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE;
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
        /* !LINKSTO EB_Crypto_CryptAlgos_1413,1 */
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Update
        (
           &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx,
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Data,
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.DataLength
        );
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH:
      {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
        CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH;
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength =
        sizeof(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResult);

        Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Finish
        (
          &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx,
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResult,
          &(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength)
        );
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_EM:
      {
        /* Get the length of the user-provided modulus (which is equal to the signature length) */
        NumberLength = Crypto_xVIx_xAIx_LNBitLengthOfNumber
                       (Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.Modulus);

        /* Make a byte length from the bit length. */
        NumberLength >>= 3U;

        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenSetAlgorithmId(&AlgorithmId, &AlgorithmIdLength );
        /* !LINKSTO EB_Crypto_CryptAlgos_1401,1 */
        DerHeaderDigestLength = Crypto_xVIx_xAIx_CommonDerHeaderOctetString
                                (
                                  DerHeaderDigestBuffer,
                                  Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength
                                );

        DerHeaderDigestInfoLength = Crypto_xVIx_xAIx_CommonDerHeaderSequence
                                    (
                                      DerHeaderDigestInfoBuffer,
                                      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength
                                      + DerHeaderDigestLength
                                      + AlgorithmIdLength
                                    );

        /* Calculate the length of the T field */
        TLength = Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength +
                  AlgorithmIdLength +
                  DerHeaderDigestLength +
                  DerHeaderDigestInfoLength;

        if ((NumberLength > (TLength + (uint32)CRYPTO_XVIX_XAIX_RSASSAV15_G_EM_STATIC_FIELDS_LEN))
            &&
            (AlgorithmId != NULL_PTR)
           )
        {
          /* Set the first two bytes */
          /* !LINKSTO EB_Crypto_CryptAlgos_1404,1 */
          /* !LINKSTO EB_Crypto_CryptAlgos_1405,1 */
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[0U] = (uint8)0x00U;
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[1U] = (uint8)0x01U;

           /* Generate the PS field */
           /* !LINKSTO EB_Crypto_CryptAlgos_1406,1 */
           for (i = 2U; i < (NumberLength - TLength - 1U); i++)
           {
             Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i] = (uint8)0xFFU;
           }

           /* Add the zero byte after the PS field */
           /* !LINKSTO EB_Crypto_CryptAlgos_1407,1 */
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i] = (uint8)0x00U;
           i++;

           /* !LINKSTO EB_Crypto_CryptAlgos_1408,1 */
           TS_MemCpy
           (
             &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i],
             DerHeaderDigestInfoBuffer,
             DerHeaderDigestInfoLength
           );
           i += DerHeaderDigestInfoLength;

           TS_MemCpy
           (
             &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i],
             AlgorithmId,
             AlgorithmIdLength
           );
           i += AlgorithmIdLength;

           TS_MemCpy
           (
             &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i],
             DerHeaderDigestBuffer,
             DerHeaderDigestLength
           );
           i += DerHeaderDigestLength;

           TS_MemCpy
           (
             &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM[i],
             Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResult,
             Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.HashResultLength
           );


           /* !LINKSTO EB_Crypto_CryptAlgos_1410,1 */
           /*Call can not fail because enough space has been allocated to the buffers in context
             and valid lengths are set.*/
           (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
           (
             Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.EM,
             NumberLength,
             Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Tmp1Value,
             (Crypto_xVIx_xAIx_LNWordType)
             (sizeof(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Tmp1Value) /
                sizeof(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Tmp1Value[0U])
             )
           );
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
           CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_S;
         }
         else
         {
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_IDLE;

           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag = TRUE;
           Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult = E_NOT_OK;
         }
         break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_S:
      {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15_FinishState();
        break;
      }
      default:
      {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15_DefaultState();
        break;
      }
    }
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCallbackCancel();
  }
}

/*==[Definition of functions with internal linkage]===========================*/

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenSetAlgorithmId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA),AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) AlgorithmId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) AlgorithmIdLength
)
{
  switch(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInfo->primitiveInfo->
               algorithm.secondaryFamily
        )
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      *AlgorithmId = &Crypto_xVIx_xAIx_AlgorithmId[0U][0U];
      *AlgorithmIdLength = CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID_LENGTH;
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      *AlgorithmId = &Crypto_xVIx_xAIx_AlgorithmId[1U][0U];
      *AlgorithmIdLength = CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID_LENGTH;
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      *AlgorithmId = &Crypto_xVIx_xAIx_AlgorithmId[2U][0U];
      *AlgorithmIdLength = CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID_LENGTH;
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      *AlgorithmId = &Crypto_xVIx_xAIx_AlgorithmId[3U][0U];
      *AlgorithmIdLength = CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID_LENGTH;
      break;
    }
#endif
#if (STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      *AlgorithmId = &Crypto_xVIx_xAIx_AlgorithmId[4U][0U];
      *AlgorithmIdLength = CRYPTO_XVIX_XAIX_ALGOFAM_SHA1_ALGORITHM_ID_LENGTH;
      break;
    }
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED) */
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_FinishState
(
  void
)
{
  /* Signature = EM^SecretExponent mod Modulus */
  /* !LINKSTO EB_Crypto_CryptAlgos_1409,1 */
  Crypto_xVIx_xAIx_LNModExp
  (
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Signature,
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Tmp1Value,
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.Modulus,
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.RsaKey.SecretExponent,
    &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.LNCtx
  );
  Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_IDLE;

  if
  (
     /* !LINKSTO EB_Crypto_CryptAlgos_1411,1 */
    Crypto_xVIx_xAIx_LNNumberToBEByteArray
    (
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Signature,
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Result,
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultLengthPtr
    ) != E_OK
  )
  {
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult = CRYPTO_E_SMALL_BUFFER;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult = E_OK;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_DefaultState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State)
  {
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE:
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH:
    {
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Mainfunction
      (
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.
          Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
      );
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED:

    default:
    {
      /* Nothing to do. */
      break;
    }

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCallbackCancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[
                                      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId
                                    ].DriverObjectState =
                                    CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId].CurrentJob =
      NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15_Callback
    (
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job,
      CRYPTO_E_JOB_CANCELED
    );
    /* It needs to be checked if the Job pointer in the context is still set in order to avoid
       calling the Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset twice in case
       of Asynchronous processing type. for the Asynchronous precessing type, the reset is called
       within the Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15_Callback function.
    */
    if(NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job)
    {
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset();
    }
  }
  else if(TRUE == Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError =
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult;
    }
    if(
        (
          CRYPTO_PROCESSING_ASYNC ==
          Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) ||
          (
            (
              ((uint8)Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
          )
        ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode)
      )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Update
            (
              Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId,
              Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job
            );
    }
    else if
    (
      (
        CRYPTO_PROCESSING_ASYNC ==
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInfo->processingType
      ) &&
      (
        (
          (
            ((uint8)Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode)
    )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Finish
            (
              Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId,
              Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job
            );
    }

    else
    {
      /* Nothing to do. */
    }

    Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15_Callback
    (
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job,
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult
    );
    Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag = FALSE;
  }

  else
  {
    /* else required by MISRA-C */
  }

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE)
Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset(void)
{
  switch(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.Job->jobPrimitiveInfo->primitiveInfo->
           algorithm.secondaryFamily
        )
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED) || \
   (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED) || \
   (STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED) || \
   (STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    case CRYPTO_ALGOFAM_SHA2_256:
    case CRYPTO_ALGOFAM_SHA2_384:
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();
      break;
    }
#endif
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }

  TS_MemBZero(
               &Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx,
               sizeof(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx)
             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      Crypto_xVIx_xAIx_SHA2224HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      Crypto_xVIx_xAIx_SHA2256HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      Crypto_xVIx_xAIx_SHA2384HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      Crypto_xVIx_xAIx_SHA2512HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      Crypto_xVIx_xAIx_SHA1HelperStart(
                                        Ctx->ObjId,
                                        &Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback,
                                        Ctx->Job,
                                        Ctx->Job->jobPrimitiveInfo->processingType
                                      );
      break;
    }
#endif
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                     DataPtr,
  uint32                                                                                    DataLength
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      (void)Crypto_xVIx_xAIx_SHA2224HelperUpdate(
                                                  DataPtr,
                                                  DataLength,
                                                  Ctx->Job->jobPrimitiveInfo->processingType
                                                );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      (void)Crypto_xVIx_xAIx_SHA2256HelperUpdate(
                                                  DataPtr,
                                                  DataLength,
                                                  Ctx->Job->jobPrimitiveInfo->processingType
                                                );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      (void)Crypto_xVIx_xAIx_SHA2384HelperUpdate(
                                                  DataPtr,
                                                  DataLength,
                                                  Ctx->Job->jobPrimitiveInfo->processingType
                                                );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                                  DataPtr,
                                                  DataLength,
                                                  Ctx->Job->jobPrimitiveInfo->processingType
                                                );
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      (void)Crypto_xVIx_xAIx_SHA1HelperUpdate(
                                               DataPtr,
                                               DataLength,
                                               Ctx->Job->jobPrimitiveInfo->processingType
                                             );
      break;
    }
#endif
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                       ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                      ResLenPtr
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      (void)Crypto_xVIx_xAIx_SHA2224HelperFinish(
                                                   ResPtr,
                                                   ResLenPtr,
                                                   FALSE,
                                                   Ctx->Job->jobPrimitiveInfo->processingType
                                                 );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      (void)Crypto_xVIx_xAIx_SHA2256HelperFinish(
                                                   ResPtr,
                                                   ResLenPtr,
                                                   FALSE,
                                                   Ctx->Job->jobPrimitiveInfo->processingType
                                                 );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      (void)Crypto_xVIx_xAIx_SHA2384HelperFinish(
                                                   ResPtr,
                                                   ResLenPtr,
                                                   FALSE,
                                                   Ctx->Job->jobPrimitiveInfo->processingType
                                                 );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                                   ResPtr,
                                                   ResLenPtr,
                                                   FALSE,
                                                   Ctx->Job->jobPrimitiveInfo->processingType
                                                 );
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      (void)Crypto_xVIx_xAIx_SHA1HelperFinish(
                                               ResPtr,
                                               ResLenPtr,
                                               Ctx->Job->jobPrimitiveInfo->processingType
                                             );
      break;
    }

#endif
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Mainfunction
(
  uint32 AlgorithmFamily
)
{
  switch(AlgorithmFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      Crypto_xVIx_xAIx_SHA2224HelperMainFunction();
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      Crypto_xVIx_xAIx_SHA2256HelperMainFunction();
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      Crypto_xVIx_xAIx_SHA2384HelperMainFunction();
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
      break;
    }
#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED)
    case CRYPTO_ALGOFAM_SHA1:
    {
      Crypto_xVIx_xAIx_SHA1HelperMainFunction();
      break;
    }
#endif /*#if(STD_ON == CRYPTO_XVIX_XAIX_SHA1_ENABLED) */
    /* CHECK: NOPARSE */
    /* All the possible secondary family algorithms have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPkcs1v15_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  TS_PARAM_UNUSED(Job);
  switch(Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State)
  {
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_INIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_UPDATE:
    {
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
      CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_INITIALIZED;
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CallbackResult = Result;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_WAIT_FOR_FINISH:
    {
        Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.State =
        CRYPTO_XVIX_XAIX_RSASSA_PKCS1V15_G_S_FINISH_EM;
    }
      break;
    /* CHECK: NOPARSE */
    /* All the possible (wait) states have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RsaSsaPkcs1v15_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy) &&
      (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError)
    )
  {
    *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ResultError = Result;
  }
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode =
      CRYPTO_XVIX_XAIX_RSASSAV15GEN_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01030, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId]
      .DriverObjectState = CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId].CurrentJob =
      NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_RSA_RSASSA_PKCS1_v1_5_Reset();
    }
    else if(
             (
               CRYPTO_OPERATIONMODE_UPDATE ==
               (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_RsaSsaPkcs1v15GenCtx.CurrentMode)
             ) &&
             (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
           )
    {
      CryIf_CallbackNotification(Job, Result);
    }

    else
    {
      /* Nothing to do. */
    }

  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#else /* #if (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == TRUE) */

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/* Dummy type definition to prevent compiler warnings about empty translation units */
typedef uint8 Crypto_xVIx_xAIx_RsaSsaPkcs1v15Gen_PrvtEmptyTranslationUnitType;

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == TRUE) #else */

