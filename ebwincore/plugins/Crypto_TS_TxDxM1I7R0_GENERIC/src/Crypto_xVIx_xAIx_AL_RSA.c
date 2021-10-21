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

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_RSA.h>
#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <TSMem.h>
#include <SchM_Crypto_xVIx_xAIx.h>

#if (STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED)

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_DEFAULT_CURRENT_MODE is already defined
#endif
/** \brief To set the default current mode. */
#define CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_DEFAULT_CURRENT_MODE 0x00U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_HASH_MAX_LEN)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_HASH_MAX_LEN is already defined
#endif
/** \brief The maximal allowed length for a hash digest in bytes. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_HASH_MAX_LEN 64U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_SIG_MASK_CNT_LEN)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_SIG_MASK_CNT_LEN is already defined
#endif
/** \brief The length of count unsed during MGF. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_SIG_MASK_CNT_LEN 4U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_MAX_DB_LEN)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_MAX_DB_LEN is already defined
#endif
/** \brief The maximum length to allocate of DB buffer. Not the actual size of DB. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_MAX_DB_LEN CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_LEN)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_LEN is already defined
#endif
/** \brief The length to zero padding during the final hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_LEN 8U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_DB_MSB_NULL_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_DB_MSB_NULL_MASK is already defined
#endif
/** \brief The Masking of MSBs of DB. It is fixed in our case as this routine only
 *         supports lengths where the exponent of the RSA Key is exactly a multiple of 8.
 */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_DB_MSB_NULL_MASK 0x80U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_EM_LAST_BYTE)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_EM_LAST_BYTE is already defined
#endif
/** \brief The expected last byte of EM. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_EM_LAST_BYTE 0xBCU

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_STRING_LENGTH)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_STRING_LENGTH is already defined
#endif
/** \brief The length of the "Null String" used to compute a hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_STRING_LENGTH 8U

#if (defined CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE is already defined
#elif (8U > CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE is to small
#endif
/** \brief The maximum length in bytes of the RSA key (modulus + exponent)
 *         The size of two Tags(a 1Byte), two lengths (a 2Byte) and atleast 1 Byte
 *         modulo or exponent can be subtracted.
 */
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE \
(CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE - 7U)

#if (defined CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD is already defined
#endif
/** \brief The maximum length in words of the RSA key. */
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD \
((CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) + 1U)

#if (defined CRYPTO_XVIX_XAIX_RSA_PUB_KEY_TEMP_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_RSA_PUB_KEY_TEMP_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number representation of
 *         the longest possible temporary public RSA key.
 */
#define CRYPTO_XVIX_XAIX_RSA_PUB_KEY_TEMP_LEN_WORDS                        \
(                                                             \
  (Crypto_xVIx_xAIx_LNWordType)                                            \
    (                                                         \
        (CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD * (Crypto_xVIx_xAIx_LNWordType)3UL) + \
    (Crypto_xVIx_xAIx_LNWordType)5UL                                                             \
  )                                                                                        \
)

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

/*--Crypto_xVIx_xAIx_RsaSsaPssVerifyStateType-----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE is already defined
#endif
/** \brief The algorithm is in the idle state. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT is already defined
#endif
/** \brief The initialization of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT 2U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED is already defined
#endif
/** \brief The algorithm is initialized. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED 3U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE is already defined
#endif
/** \brief The update operation of the algorithm is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE 4U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT is already defined
#endif
/** \brief The update operation of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT 5U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH is already defined
#endif
/** \brief The finish operation of the algorithm is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH 6U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT is already defined
#endif
/** \brief The finish operation of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT 7U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP is already defined
#endif
/** \brief Starting the Exp calculation. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP 8U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP is already defined
#endif
/** \brief The expected signature shall be verified via modular exponentiation. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP 9U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP is already defined
#endif
/** \brief Finishing the Exp calculation. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP 10U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK is already defined
#endif
/** \brief Starting the Mask Hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK 11U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT is already defined
#endif
/** \brief Wait for the start Mask Hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT 12U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK is already defined
#endif
/** \brief Update Mask Hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK 13U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT is already defined
#endif
/** \brief Wait for Update Mask Hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT 14U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK is already defined
#endif
/** \brief Update Count for Mask Hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK 15U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT is already defined
#endif
/** \brief Wait for Update Count for Mask Hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT 16U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK is already defined
#endif
/** \brief End the mask hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK 17U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT is already defined
#endif
/** \brief Wait for the mask hash to end. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT 18U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK is already defined
#endif
/** \brief Final hash starting. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK 19U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT is already defined
#endif
/** \brief Wait for Final hash start. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT 20U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE is already defined
#endif
/** \brief Update the zeros in the final hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE 21U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT is already defined
#endif
/** \brief Wait for Update the zeros in the final hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT 22U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE)
#error CRYPTO_RSASSA_PSS_INITCRYPTO_RSASSA_PSS_FINALHASH_HASH_UPDATEis already defined
#endif
/** \brief Update the mask in the final hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE 23U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT already defined
#endif
/** \brief Wait for update to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT 24U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE already defined
#endif
/** \brief Update the Salt. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE 25U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT already defined
#endif
/** \brief Wait for Salt to update. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT 26U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH already defined
#endif
/** \brief Finish the final hash. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH 27U

#if (defined CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT)
#error CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT already defined
#endif
/** \brief Wait for final hash to finish. */
#define CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT 28U

/*==[Types]=======================================================================================*/

/** \brief Internal state of the RSASSA-PSS signature verification. */
typedef uint8 Crypto_xVIx_xAIx_RsaSsaPssVerifyStateType;

/** \brief The public RSA key used for verifying a signature.
 *
 * \var Crypto_xVIx_xAIx_RsaPublicKeyType::length
 * \brief Dummy element to make the type compatible with the Csm key type.
 *
 * \var Crypto_xVIx_xAIx_RsaPublicKeyType::PublicExponent
 * \brief The public exponent of the RSA key.
 *
 * \var Crypto_xVIx_xAIx_RsaPublicKeyType::Modulus
 * \brief The modulus of the RSA key.
 *
 * \var Crypto_xVIx_xAIx_RsaPublicKeyType::Barrett
 * \brief The Barrett value (only used if Barrett reduction is enabled).
 *
 */
typedef struct
{
  uint32                      Length;
  Crypto_xVIx_xAIx_LNWordType PublicExponent[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_LNWordType Modulus[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_LNWordType Barrett[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD];
}
Crypto_xVIx_xAIx_RsaPublicKeyType;

/** \brief Structure which contains the context of the RSASSA-PSS signature verification.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::SignatureLength
 * \brief The length of the signature to verify.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::DataLength
 * \brief The length of the data.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::HashResultLength
 * \brief The length of the hash value of the data.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaskHashResultLength
 * \brief The length of the mask hash value.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::HashLength
 * \brief The length of the hash algorithm.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::SLen
 * \brief The length of the Salt.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::dbLen
 * \brief The length of the DB.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaskCnt
 * \brief The Count value for mask hash calculation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaskItr
 * \brief The value of mask hash iterations.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaxNumberOfSlices
 * \brief Maximum number of time slices used for Pss signature calculation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::RsaKey
 * \brief The public key which should be used for verifying the signature.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::Signature
 * \brief A pointer to the start of a buffer where the signature to verify is stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::ResultPtr
 * \brief A pointer to a variable where the signature verification result has to be stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::Data
 * \brief A pointer to the start of an array where the data whose signature to verify is stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaskHashPtr
 * \brief Array where the masked hash is stored.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::Tmp1Value
 * \brief An array which is used as a buffer during signature verification.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::Tmp2Value
 * \brief An array which is used as a buffer during signature verification.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::LNCtx
 * \brief The context which has to be used for long number arithmetic.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::State
 * \brief The current state the RSASSA-PSS signature verification is in.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::HashResult
 * \brief Array where the hash value of the
 *        data will be buffered during the computation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::HashDash
 * \brief Array where the hash value of the H'
 *        data will be buffered during the computation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::MaskCntStr
 * \brief Temporary Pointer for the hash calculation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::DbMask
 * \brief Array where the dBMask is stored during the computation.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::EM
 * \brief An array which is used as a buffer for the value "EM" during the signature verification.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::UseBarrett
 * \brief Indicates whether the signature verification uses Barrett reduction.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::RestartFlag
 * \brief Flag that states if the restart of the service is active.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::ObjId
 * \brief The driver object id.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::Job
 * \brief The currently processed job.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::ResultError
 * \brief The return value which will be used when calling the callback.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::IsBusy
 * \brief Flag which indicates if another MainFunction is needed.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::CurrentMode
 * \brief The currently processed mode.
 *
  * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::CancelFlag
 * \brief Flag that states if the job shall be canceld.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::CallbackFlag
 * \brief Flag that states if the callback shall be called.
 *
 * \var Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType::CallbackResult
 * \brief The result which will be forwarded to the callback.
 *
 */
typedef struct
{
  uint32 SignatureLength;
  uint32 DataLength;
  uint32 HashResultLength;
  uint32 MaskHashResultLength;
  uint32 HashLength;
  uint32 SLen;
  uint32 DbLen;
  uint32 MaskCnt;
  uint32 MaskItr;
  uint32 MaxNumberOfSlices;
  Crypto_xVIx_xAIx_RsaPublicKeyType                                              RsaKey;
  P2CONST(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                            Signature;
  P2VAR(Crypto_VerifyResultType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)            ResultPtr;
  P2CONST(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                            Data;
  P2VAR(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                              MaskHashPtr;
  Crypto_xVIx_xAIx_LNWordType Tmp1Value[CRYPTO_XVIX_XAIX_RSA_PUB_KEY_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType Tmp2Value[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD];
  Crypto_xVIx_xAIx_LNCtxType  LNCtx;
  Crypto_xVIx_xAIx_RsaSsaPssVerifyStateType State;
  uint8   HashResult[CRYPTO_XVIX_XAIX_RSASSA_PSS_HASH_MAX_LEN];
  uint8   HashDash[CRYPTO_XVIX_XAIX_RSASSA_PSS_HASH_MAX_LEN];
  uint8   MaskCntStr[CRYPTO_XVIX_XAIX_RSASSA_PSS_SIG_MASK_CNT_LEN];
  uint8   DbMask[CRYPTO_XVIX_XAIX_RSASSA_PSS_MAX_DB_LEN];
  uint8   EM[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_BYTE];
  boolean UseBarrett;
  boolean RestartFlag;
  uint32  ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)   Job;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        IsBusy;
  Crypto_OperationModeType                           CurrentMode;
  boolean                                            CancelFlag;
  boolean                                            CallbackFlag;
  Std_ReturnType                                     CallbackResult;
}
Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType;

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdateCntMask
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssEndMask
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashZeroUpdate
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashUpdate
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashFinish
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_COMPARE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssCompare
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdateMask
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishSaltUpdate
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdate
(
  void
);

/** \brief State helper function: This function performes the required processing for the
 *         CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPss_Finish
(
  void
);

/**  \brief This function resets the Context of RSA Verify.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset
(
  void
);

/**  \brief This function calls the helper hash start function depending on the secondaryFamily.
 *
 * \param[in] Ctx  A pointer to the context.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}

 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
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
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                   DataPtr,
  uint32                                                                                  DataLength
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
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                     ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                    ResLenPtr
);

/** \brief This function calls the helper hash main function depending on the secondaryFamily.
 *
 * \param[in] Ctx A pointer to the context.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_MainFunction
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
);

/** \brief This is the callback function of the underlying hash primitive.
 *
 * \param[in,out] Job     Reference to the currently processed job.
 * \param[in]     Result  The error value.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Callback
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
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RSA_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief Helper function for the state CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP.
 *
 *         This helper function starts the exponentiation
 *         when performing the signature verification.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssStartExp
(
  void
);

/** \brief Helper function for the state CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP.
 *
 *         This helper function computes signature via modular exponentiation.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishComputeModularExp
(
  void
);

/** \brief Helper function for the state CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP.
 *
 *         This helper function finishes the exponentiation
 *         when performing the signature verification.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishExp
(
  void
);

/** \brief Helper function for the state CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK.
 *
 *         This helper function computes uses the DBMask to compute DB
 *         when performing the signature verification.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssDoneMask
(
  void
);

/** \brief Helper function for the state Default state.
 *
 *         This function is used to perform the common processing for many state,
 *         which have the same processing.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_RsaSsaPss_DefaultWaitState
(
  void
);

/** \brief Function to perform the callback and job cancellation.
 *
 *     This function is implemented in order to perform the job cancellation
 *     and callback functionality is a single call, instead of having it in the
 *     Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_RsaSsaPss_Callback_Cancel
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief "Null String" constant needed for signature verification.
 *
 *         This constant provides a "Null String" needed during the signature verification.
 *
 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST)
Crypto_xVIx_xAIx_RsaSsaPssVerify_Null_String[CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_STRING_LENGTH] =
{ 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief RSASSA-PSS signature verification context.
 *
 *         The context variable which is used to store the internal state of the
 *         RSASSA-PSS signature verification.
 *
 */
static VAR(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief The PrimitiveInfo for the job used in the RSASSA-PSS signature verification helper. */
static VAR(Crypto_PrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyPrimitiveInfo =
{
  64U,                                                    /* resultLength */
  CRYPTO_SIGNATUREVERIFY,                                 /* service */
  {
    CRYPTO_ALGOFAM_RSA,                                   /* family */
    CRYPTO_ALGOFAM_SHA2_256,                              /* secondaryFamily */
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE, /* keyLength */
    CRYPTO_ALGOMODE_NOT_SET                               /* mode */
  }                                                       /* algorithm */
};

/** \brief The JobPrimitiveInfo for the job used in the RSASSA-PSS signature verification helper. */
static VAR(Crypto_JobPrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyJobPrimitiveInfo =
{
  0U,                                                   /* callbackId */
  &Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyPrimitiveInfo, /* primitiveInfo */
  0U,                                                   /* secureCounterId */
  0U,                                                   /* cryIfKeyId */
  CRYPTO_PROCESSING_SYNC,                               /* processingType */
  FALSE                                                 /* callbackUpdateNotification */
};

/** \brief The JobInfo for the job used in the RSASSA-PSS signature verification helper. */
static VAR(Crypto_JobInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyJobInfo =
{
  0U, /* jobId */
  0U  /* jobPriority*/
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_1141, 1, EB_Crypto_CryptAlgos_0968, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean IsExponentValid = FALSE;
  boolean IsModuloValid = FALSE;
  boolean UseBarrett = FALSE;
  boolean IsBarrettValid = FALSE;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr;
  uint32 KeyLength = CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE;

  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset();

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
      /* convert to rsa key type */
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent[0U] =
      CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus[0U] =
      CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett[0U] =
      CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
      if(
          E_OK == Crypto_xVIx_xAIx_AL_DecodeRsaKey
                  (
                    KeyPtr,
                    KeyLength,
                    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
                    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent
                  )
        )
      {
        /* get barrett if available */
        KeyLength = CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE;
        if(
            E_OK == Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                    (
                      Job->cryptoKeyId,
                      CRYPTO_XVIX_XAIX_KE_SIGNATURE_BARRETT,
                      &KeyPtr,
                      &KeyLength
                    )
          )
        {
          /* write barrett to the rsa key this function can not fail */
          (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
          (
            KeyPtr,
            KeyLength,
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett,
            CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
          );
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.UseBarrett = TRUE;
        }
        else
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.UseBarrett = FALSE;
        }
        /* check if key is valid */
        IsExponentValid = Crypto_xVIx_xAIx_LNIsValid
                          (
                            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent,
                            CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                          );
        IsModuloValid = Crypto_xVIx_xAIx_LNIsValid
                        (
                          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
                          CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                        );
        UseBarrett = Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.UseBarrett;
        IsBarrettValid = Crypto_xVIx_xAIx_LNIsValid
                        (
                          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett,
                          CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                        );
        if(
            (TRUE == IsExponentValid) && (TRUE == IsModuloValid) &&
            (
              (FALSE == UseBarrett) || ((TRUE == UseBarrett) && (TRUE == IsBarrettValid))
            )
          )
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId = ObjectId;
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job = Job;
          RetVal = E_OK;
          Crypto_xVIx_xAIx_RSA_HASH_Start(&Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx);
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
          if(
              CRYPTO_PROCESSING_SYNC ==
              Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
            )
          {
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED;
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
          }
          else
          {
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT;
          }
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

/* !LINKSTO EB_Crypto_CryptAlgos_1162, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if (CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Data = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DataLength = Job->jobPrimitiveInputOutput.inputLength;

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1183, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if (
            CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State &&
            CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode
          )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Signature = Job->jobPrimitiveInputOutput.secondaryInputPtr;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SignatureLength =
    Job->jobPrimitiveInputOutput.secondaryInputLength;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = Job->jobPrimitiveInputOutput.verifyPtr;
    /* Set state of the state machine to finish */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}
/* !LINKSTO EB_Crypto_CryptAlgos_1223, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State)
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Helper_SyncSingleCall
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32 InputLength,
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean        Busy   = FALSE;
  boolean IsExponentValid = FALSE;
  boolean IsModuloValid = FALSE;
  boolean UseBarrett = FALSE;
  boolean IsBarrettValid = FALSE;
  uint32 ObjId = 0U;

  Crypto_JobType Job =
  {
    0U,
    CRYPTO_JOBSTATE_IDLE,
    {
      NULL_PTR,
      0U,
      NULL_PTR,
      0U,
      NULL_PTR,
      0U,
      NULL_PTR,
      0U,
      NULL_PTR,
      NULL_PTR,
      NULL_PTR,
      CRYPTO_OPERATIONMODE_SINGLECALL
    },
    &Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyJobPrimitiveInfo,
    &Crypto_xVIx_xAIx_HelperRsaSsaPssVerifyJobInfo,
    0U
  };
  Job.jobPrimitiveInputOutput.inputPtr = InputPtr;
  Job.jobPrimitiveInputOutput.inputLength = InputLength;
  Job.jobPrimitiveInputOutput.secondaryInputPtr    = signaturePtr;
  Job.jobPrimitiveInputOutput.secondaryInputLength = signatureLength;
  Job.jobPrimitiveInputOutput.verifyPtr            = VerifyPtr;
  /*only process if the DriverObjectState is not busy.*/
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  if(
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE ==
        Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState
    )
  {
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = &Job;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO Crypto.Dsn.JobTransition.Idle2Active, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
    Job.jobState = CRYPTO_JOBSTATE_ACTIVE;

    /* convert to rsa key type */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent[0U] =
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus[0U] =
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett[0U] =
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD - 1U;
    if(
        E_OK == Crypto_xVIx_xAIx_AL_DecodeRsaKey
                (
                  Key->data,
                  Key->length,
                  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
                  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent
                )
      )
    {
      /* check if key is valid */
      IsExponentValid = Crypto_xVIx_xAIx_LNIsValid
                        (
                          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent,
                          CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                        );
      IsModuloValid = Crypto_xVIx_xAIx_LNIsValid
                      (
                        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
                        CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                      );
      UseBarrett = Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.UseBarrett;
      IsBarrettValid = Crypto_xVIx_xAIx_LNIsValid
                      (
                        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett,
                        CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_RSA_MAX_KEY_SIZE_WORD
                      );
      if(
          (TRUE == IsExponentValid) && (TRUE == IsModuloValid) &&
          (
            (FALSE == UseBarrett) || ((TRUE == UseBarrett) && (TRUE == IsBarrettValid))
          )
        )
      {
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId = ObjId;
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job = &Job;
        RetVal = E_OK;
        Crypto_xVIx_xAIx_RSA_HASH_Start(&Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx);
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
        if(
            CRYPTO_PROCESSING_SYNC ==
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
          )
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED;
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
        }
        else
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT;
        }
      }
    }
    else
    {
      RetVal = E_NOT_OK;
    }
    if(E_OK == RetVal)
    {
      /* In this context for the Hash Start the MainFunction has only to be called once */
      Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction(&RetVal, &Busy);

    }
    if(E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Update(ObjId,&Job);
      do
      {
        Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction(&RetVal, &Busy);
      }while((TRUE == Busy) && (E_OK == RetVal));
    }
    if(E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Finish(ObjId,&Job);
      do
      {
        Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction(&RetVal, &Busy);
      }while((TRUE == Busy) && (E_OK == RetVal));
    }

    /*set DriverObjectState back to idle after processing of the function. Also
    set the currentJob to NULL_PTR.*/
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  }
  else
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1244, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError = E_OK;
    }

    switch(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State)
    {
      case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE:
      {
        /*state function for CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE.*/
        Crypto_xVIx_xAIx_RsaSsaPssUpdate();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH:
      {
        /*state function for CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH.*/
        Crypto_xVIx_xAIx_RsaSsaPss_Finish();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP:
      {
        Crypto_xVIx_xAIx_RsaSsaPssStartExp();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP:
      {
        Crypto_xVIx_xAIx_RsaSsaPssFinishComputeModularExp();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP:
      {
        Crypto_xVIx_xAIx_RsaSsaPssFinishExp();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK:
      {
        Crypto_xVIx_xAIx_RSA_HASH_Start(&Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx);
        if(
            CRYPTO_PROCESSING_SYNC ==
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
          )
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK;
        }
        else
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT;
        }
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK:
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_1111,1 */
        /* !LINKSTO EB_Crypto_CryptAlgos_1115,1 */
        /*State helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssUpdateMask();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK:
      {
        /*State helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssUpdateCntMask();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK:
      {
        /*state helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssEndMask();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK:
      {
        Crypto_xVIx_xAIx_RsaSsaPssDoneMask();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE:
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_1123,1 */
        /*State helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssFinalHashZeroUpdate();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE:
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_1123,1 */
        /*State helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssFinalHashUpdate();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE:
      {
        /*State helper function*/
        Crypto_xVIx_xAIx_RsaSsaPssFinishSaltUpdate();
        break;
      }

      case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH:
      {
        /*State helper function.*/
        Crypto_xVIx_xAIx_RsaSsaPssFinalHashFinish();
        break;
      }

      default:
      {
        Crypto_xVIx_xAIx_RSA_RsaSsaPss_DefaultWaitState();
        break;
      }
    }
    Crypto_xVIx_xAIx_RSA_RsaSsaPss_Callback_Cancel();
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdateCntMask
(
  void
)
{
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCntStr[0U] =
  (uint8)((Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt >> 24U) & (uint8)0xFFU);
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCntStr[1U] =
  (uint8)((Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt >> 16U) & (uint8)0xFFU);
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCntStr[2U] =
  (uint8)((Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt >>  8U) & (uint8)0xFFU);
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCntStr[3U] =
  (uint8)((Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt) & (uint8)0xFF);
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt++;
  /* !LINKSTO EB_Crypto_CryptAlgos_1115,1 */
  Crypto_xVIx_xAIx_RSA_HASH_Update
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCntStr[0U],
    CRYPTO_XVIX_XAIX_RSASSA_PSS_SIG_MASK_CNT_LEN
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssEndMask
(
  void
)
{
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashResultLength =
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashLength;
  Crypto_xVIx_xAIx_RSA_HASH_Finish
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashPtr,
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashResultLength
  );
  if(
    CRYPTO_PROCESSING_SYNC ==
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    if(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt < Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskItr)
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK;
    }
    else
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK;
    }
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT;
  }
  /* !LINKSTO EB_Crypto_CryptAlgos_1116,1 */
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashPtr =
  &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashPtr[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashResultLength];
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashZeroUpdate
(
  void
)
{
  Crypto_xVIx_xAIx_RSA_HASH_Update
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    &Crypto_xVIx_xAIx_RsaSsaPssVerify_Null_String[0U],
    CRYPTO_XVIX_XAIX_RSASSA_PSS_NULL_LEN
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashUpdate
(
  void
)
{
  Crypto_xVIx_xAIx_RSA_HASH_Update
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResult[0U],
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinalHashFinish
(
  void
)
{
  Crypto_xVIx_xAIx_RSA_HASH_Finish
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashDash[0U],
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength
  );
  if(
    CRYPTO_PROCESSING_SYNC ==
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    /* Compare the signature */
    /* !LINKSTO EB_Crypto_CryptAlgos_1124,1 */
    /*State helper function*/
    Crypto_xVIx_xAIx_RsaSsaPssCompare();
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT;
  }
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength =
  sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashDash);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssCompare
(
  void
)
{
  if(
      E_OK !=
      Crypto_xVIx_xAIx_MemCmp_Sec
      (
        &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen],
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashDash,
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength
      )
    )
  {
    *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
  }
  else
  {
    *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = CRYPTO_E_VER_OK;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
  }
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdateMask
(
  void
)
{
  Crypto_xVIx_xAIx_RSA_HASH_Update
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen],
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashLength
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishSaltUpdate
(
  void
)
{
  if(0U < Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SLen)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1123,1 */
    Crypto_xVIx_xAIx_RSA_HASH_Update
    (
      &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
      &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[
                                                   Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen -
                                                   Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SLen
                                                 ],
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SLen
    );
    if(
        CRYPTO_PROCESSING_SYNC ==
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
      )
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH;
    }
    else
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT;
    }
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssUpdate
(
  void
)
{
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  /* !LINKSTO EB_Crypto_CryptAlgos_1107,1 */
  Crypto_xVIx_xAIx_RSA_HASH_Update
  (
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Data,
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DataLength
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPss_Finish
(
  void
)
{
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
  /* !LINKSTO EB_Crypto_CryptAlgos_1102,1 */
  if(
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SignatureLength !=
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus[0U] *
      CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES
    )
  {
    *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
     Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
     Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
  }
  else
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength =
    sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResult);
    Crypto_xVIx_xAIx_RSA_HASH_Finish
    (
      &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResult,
      &(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength)
    );
    if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
      )
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP;
    }
    else
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset(void)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx,
               sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx)
             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      Crypto_xVIx_xAIx_SHA2224HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RSA_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      /* set selected hash length in byte */
      Ctx->HashLength = 28U;
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_256:
    {
      Crypto_xVIx_xAIx_SHA2256HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RSA_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      /* set selected hash length in byte */
      Ctx->HashLength = 32U;
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_384_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_384:
    {
      Crypto_xVIx_xAIx_SHA2384HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RSA_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      /* set selected hash length in byte */
      Ctx->HashLength = 48U;
      break;
    }

#endif
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_512_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_512:
    {
      Crypto_xVIx_xAIx_SHA2512HelperStart(
                                           Ctx->ObjId,
                                           &Crypto_xVIx_xAIx_RSA_HASH_Callback,
                                           Ctx->Job,
                                           Ctx->Job->jobPrimitiveInfo->processingType
                                         );
      /* set selected hash length in byte */
      Ctx->HashLength = 64U;
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

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                   DataPtr,
  uint32                                                                                  DataLength
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
      (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
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

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                     ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                    ResLenPtr
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
  {
#if(STD_ON == CRYPTO_XVIX_XAIX_SHA2_224_ENABLED)
    case CRYPTO_ALGOFAM_SHA2_224:
    {
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
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
      /* we are in the correct state when this is called so it can not fail */
      (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                                  ResPtr,
                                                  ResLenPtr,
                                                  FALSE,
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

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_MainFunction
(
  P2CONST(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
)
{
  switch(Ctx->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily)
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

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  TS_PARAM_UNUSED(Job);
  switch(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State)
  {
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED;
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = Result;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_EXP;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT:
    {
      if(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt < Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskItr)
      {
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK;
      }
      else
      {
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK;
      }
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT:
    {
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH;
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT:
    {
      /* Compare the signature */
      /* !LINKSTO EB_Crypto_CryptAlgos_1124,1 */
      /*State helper function*/
      Crypto_xVIx_xAIx_RsaSsaPssCompare();
      break;
    }
    /* CHECK: NOPARSE */
    /*All the valid (wait) states have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RSA_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy) &&
      (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError)
    )
  {
    *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError = Result;
  }
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode =
      CRYPTO_XVIX_XAIX_RSASSAPSSVERIFY_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01030, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId].CurrentJob =
      NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset();
    }
    else if(
             (
               CRYPTO_OPERATIONMODE_UPDATE ==
               (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode)
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
      /*nothing to do. */
    }

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssStartExp
(
  void
)
{

  /* !LINKSTO EB_Crypto_CryptAlgos_1103,1 */
  /* This can be casted to void. If SignatureLength is valid was checked before and the size of
     Tmp2Value is guaranteed by the configuration.*/
  (void) Crypto_xVIx_xAIx_LNBEByteArrayToNumber
         (
           Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Signature,
           Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SignatureLength,
           Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp2Value,
           (Crypto_xVIx_xAIx_LNWordType)
           (
             sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp2Value) /
             sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp2Value[0U])
           )
         );

  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_COMPUTE_MODEXP;

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishComputeModularExp
(
  void
)
{
  /* !LINKSTO EB_Crypto_CryptAlgos_1104,1 */

  if (Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.UseBarrett == TRUE)
  {
    (void) Crypto_xVIx_xAIx_LNModExpBarrett
    (
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp1Value,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp2Value,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Barrett,
      &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.LNCtx
    );
  }
  else
  {
    Crypto_xVIx_xAIx_LNModExp
    (
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp1Value,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp2Value,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.Modulus,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.RsaKey.PublicExponent,
      &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.LNCtx
    );
  }

  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_EXP;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssFinishExp
(
  void
)
{
  uint32  NumberLength = sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM);

  /* !LINKSTO EB_Crypto_CryptAlgos_1105,1 */

    /* EM */

    /* This can be casted to void. The size of EM is guaranteed by the configuration.*/
    (void) Crypto_xVIx_xAIx_LNNumberToBEByteArray
    (
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp1Value,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM,
      &NumberLength
    );
  /* !LINKSTO EB_Crypto_CryptAlgos_1108,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1109,1 */
  if (
       (
         (
           Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM[0U] &
           CRYPTO_XVIX_XAIX_RSASSA_PSS_DB_MSB_NULL_MASK
         ) != 0x00U
       ) ||
       (NumberLength < (Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength + 2U)) ||
       (
         Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM[NumberLength - 1U] !=
         CRYPTO_XVIX_XAIX_RSASSA_PSS_EM_LAST_BYTE
       )
     )
  {
     *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
     Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE;
     Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
     Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
  }
  else
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1110,1 */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen =
    NumberLength - Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength - 1U;

    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskCnt = 0U;
    /* !LINKSTO EB_Crypto_CryptAlgos_1114,1 */
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskItr =
    (
      (
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen +
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength
      ) - 1U
    ) / Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.HashResultLength;
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.MaskHashPtr =
    &Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[0U];
  }

  /* Tmp1Value no longer needed, reset it to 0 */
  TS_MemBZero(
               Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp1Value,
               sizeof(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Tmp1Value)
             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RsaSsaPssDoneMask
(
  void
)
{
  uint32 i;
  boolean finished = FALSE;

  /* !LINKSTO EB_Crypto_CryptAlgos_1117,1 */
  Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[0U] &=
  (uint8)(~(uint8)CRYPTO_XVIX_XAIX_RSASSA_PSS_DB_MSB_NULL_MASK);

  /* !LINKSTO EB_Crypto_CryptAlgos_1118,1 */
  for (i = 0U; i < Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen; i++)
  {
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[i] ^= Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.EM[i];
  }
  for (i = 0U; ((i < Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen) && (FALSE == finished)); i++)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1112,1 */
    /* !LINKSTO EB_Crypto_CryptAlgos_1119,1 */
    /* !LINKSTO EB_Crypto_CryptAlgos_1120,1 */
    if (Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[i] != 0U)
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_1121,1 */
      if (Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbMask[i] == 0x01U)
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_1122,1 */
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.SLen =
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.DbLen - i - 1U;

        Crypto_xVIx_xAIx_RSA_HASH_Start(&Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx);
        if(
            CRYPTO_PROCESSING_SYNC ==
            Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
          )
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE;
        }
        else
        {
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT;
        }
      }
      else
      {
        *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State = CRYPTO_XVIX_XAIX_RSASSA_PSS_IDLE;
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult = E_OK;
      }
      finished = TRUE;
    }
  }

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_RsaSsaPss_DefaultWaitState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.State)
  {
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINISH_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_START_MASK_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_MASK_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_UPDATE_CNT_MASK_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_END_MASK_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_DONE_MASK_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_ZERO_UPDATE_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_HASH_UPDATE_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_SALT_UPDATE_WAIT:
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_FINALHASH_FINISH_WAIT:
    {
      Crypto_xVIx_xAIx_RSA_HASH_MainFunction(&Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx);
      break;
    }
    case CRYPTO_XVIX_XAIX_RSASSA_PSS_INITIALIZED:
    {
      break;
    }

    /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    default:
    {}
    break;

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RSA_RsaSsaPss_Callback_Cancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[
                                      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId
                                    ].DriverObjectState =
                                    CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId].CurrentJob =
      NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_AL_RSA_Callback
    (
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job,
      CRYPTO_E_JOB_CANCELED
    );
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Reset();
  }
  else if(TRUE == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ResultError =
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult;
    }
    if(
        (
          CRYPTO_PROCESSING_ASYNC ==
          Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) ||
          (
            (
              ((uint8)Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
          )
        ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode)
      )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Update
            (
              Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId,
              Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job
            );
    }
    else if
    (
      (
        CRYPTO_PROCESSING_ASYNC ==
        Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInfo->processingType
      ) &&
      (
        (
          (
            ((uint8)Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CurrentMode)
    )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Finish
            (
              Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.ObjId,
              Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job
            );
    }

    else
    {
      /* Nothing to do.  */
    }

    Crypto_xVIx_xAIx_AL_RSA_Callback
    (
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.Job,
      Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackResult
    );
    Crypto_xVIx_xAIx_RsaSsaPssVerifyCtx.CallbackFlag = FALSE;
  }

  else
  {
    /* Nothing to do.  */
  }

}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#else /* #if (STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED) */

/* Dummy type definition to prevent compiler warnings about empty translation units */
typedef uint8 Crypto_xVIx_xAIx_RsaSsaPssVerify_PrvtEmptyTranslationUnitType;

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED) */

/*==[end of file]=================================================================================*/

