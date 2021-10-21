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

/*================================================================================================*/

#ifndef CRYPTO_XVIX_XAIX_AL_KEYMANAGEMENT_H
#define CRYPTO_XVIX_XAIX_AL_KEYMANAGEMENT_H

/*==================[includes]====================================================================*/

#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Base64.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>

/* !LINKSTO EB_Crypto_01019, 1, EB_Crypto_01021, 1, Crypto.Req.Gen/NvM/01019, 1 */
#if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)

/*==================[macros]======================================================================*/

#if (defined CRYPTO_XVIX_XAIX_FORMAT_BIN_CERT_CVC)
#error CRYPTO_XVIX_XAIX_FORMAT_BIN_CERT_CVC is already defined
#endif
/** \brief Tag for encoding format used in certificate */
#define CRYPTO_XVIX_XAIX_FORMAT_BIN_CERT_CVC              0x08U

#if (defined CRYPTO_XVIX_XAIX_CERT_KEY_MOD_TAG)
#error CRYPTO_XVIX_XAIX_CERT_KEY_MOD_TAG is already defined
#endif
/** \brief Tag for modulus of public key used in certificates */
#define CRYPTO_XVIX_XAIX_CERT_KEY_MOD_TAG 0x01U

#if (defined CRYPTO_XVIX_XAIX_CERT_KEY_EXP_TAG)
#error CRYPTO_XVIX_XAIX_CERT_KEY_EXP_TAG is already defined
#endif
/** \brief Tag for exponent of public key used in certificates */
#define CRYPTO_XVIX_XAIX_CERT_KEY_EXP_TAG 0x02U

#if(defined CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
#error CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED is already defined
#endif
/** \brief Key derivation only enabled if corresponding key elements exist at least once. */
#if(                                                                                         \
     (                                                                                       \
       (defined CRYPTO_KE_KEYDERIVATION_SALT && defined CRYPTO_KE_KEYDERIVATION_PASSWORD) && \
       defined CRYPTO_KE_KEYDERIVATION_ALGORITHM                                             \
     ) &&                                                                                    \
     (                                                                                       \
       (STD_ON == CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED) ||                                      \
       (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)                                         \
     )                                                                                       \
   )
#define CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED STD_ON
#else
#define CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED STD_OFF
#endif

/*==================[type definitions]============================================================*/

/** \brief  Key data.
 */
/** \var Crypto_xVIx_xAIx_AL_KeyDataType::RfKeyElemDataSize
 *  \brief Key element data size.
 */
/** \var Crypto_xVIx_xAIx_AL_KeyDataType::RfKeyElemData
 *  \brief Key element data.
 */
/** \var Crypto_xVIx_xAIx_AL_KeyDataType::RfKeyElemDataInit
 *  \brief Key element initial data.
 */
/** \var Crypto_xVIx_xAIx_AL_KeyDataType::RfKeyElemDataNvramBlockId
 *  \brief NVram block ID. If it is 0 than it means data is not persistent
 *         and not saved in NvM block.
 */
typedef struct
{
  uint32                                            RfKeyElemDataSize;
  CONSTP2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    RfKeyElemData;
  CONSTP2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST) RfKeyElemDataInit;
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
  CONST(uint16, CRYPTO_xVIx_xAIx_CONST)                       RfKeyElemDataNvramBlockId;
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
} Crypto_xVIx_xAIx_AL_KeyDataType;

/** \brief  Data for all crypto keys.
 *
 * \var Crypto_xVIx_xAIx_AL_KeysDataType::NumKeyData
 *  \brief Number of key data.
 *
 * \var Crypto_xVIx_xAIx_AL_KeysDataType::KeyData
 *  \brief Pointer to key data.
 */
typedef struct
{
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST)                                            NumKeyData;
  CONSTP2VAR(Crypto_xVIx_xAIx_AL_KeyDataType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) KeyData;
} Crypto_xVIx_xAIx_AL_KeysDataType;

/*==================[external function declarations]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Set a key element
 *
 * This function attempts to set the requested key element within the crypto engine
 *
 * \param[in] CryptoKeyId   Identifier of the crypto key whose key element shall be set
 * \param[in] KeyElementId  Identifier of the key element that shall be set
 * \param[in] KeyPtr        Pointer to the key data that shall be set as the key element
 * \param[in] KeyLength     Length of the key element in bytes
 *
 * \returns Result of the key setting operation
 * \retval E_OK                        Request successful
 * \retval E_NOT_OK                    Request failed
 * \retval CRYPTO_E_BUSY               Request failed, crypto driver object is busy
 * \retval CRYPTO_E_KEY_WRITE          Write access was denied
 * \retval CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementSet
(
  uint32                                                CryptoKeyId,
  uint32                                                KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32                                                KeyLength
);

/** \brief Set a key to valid
 *
 * This function sets a key to the state valid
 *
 * \param[in] CryptoKeyId  Identifier of the crypto key that shall be set to valid
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyValidSet
(
  uint32 CryptoKeyId
);

/** \brief Retrieve the value of a key element
 *
 * This function copies the requested key element to the provied buffer
 *
 * \param[in] CryptoKeyId          Identifier of the crypto key whose element
 *                                 shall be retrieved.
 * \param[in] KeyElementId         Identifier of the key element that shall be
 *                                 retrieved.
 * \param[out] ResultPtr           Pointer to the memory location where the key
 *                                 element data shall be stored.
 * \param[in,out] ResultLengthPtr  Pointer to the location where the length
 *                                 information is stored. Shall contain the
 *                                 amount of bytes that shall be read.
 *                                 After finishing the request, it contains the
 *                                 amount of bytes that has been stored in the result.
 *
 * \returns Result of the request
 * \retval E_OK                        Request successful
 * \retval E_NOT_OK                    Request failed
 * \retval CRYPTO_E_BUSY               Request failed, crypto driver object is busy
 * \retval CRYPTO_E_KEY_READ           Read access was denied
 * \retval CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 * \retval CRYPTO_E_SMALL_BUFFER       The provided buffer is too small
 *                                     to store the result
 *
 * \Reentrancy{ Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGet
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
);

#if (                                                         \
      (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)  || \
      (STD_ON == CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED) || \
      (STD_ON == CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED)     || \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED)   || \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED)   || \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED)     || \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED)    || \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED)        \
    )
/** *\brief Searches for a key element and returns a pointer to its data and corresponding length.
 *
 * \param[in]  CryptoKeyId      The id of the key
 * \param[in]  KeyElementId     The id of the key element
 * \param[out] ResultPtr        Pointer to the key element's data
 * \param[out] ResultLengthPtr  Pointer to the size of the key element's data
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGetPtr
(
  uint32                                                                                  CryptoKeyId,
  uint32                                                                                  KeyElementId,
  P2VAR(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                    ResultLengthPtr
);
#endif

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
/** \brief Write modulus and exponent of an Rsa key to a given array adding tags and lengths.
 *
 * This function writes modulus and exponent to the provided array and adds tag
 * CRYPTO_XVIX_XAIX_CERT_KEY_MOD_TAG and tag CRYPTO_XVIX_XAIX_CERT_KEY_EXP_TAG with the
 * corresponding length (two bytes).
 *
 * \param[in] DerModulosPtr            Pointer to the modulus data
 * \param[in] DerModulosLengthPtr      Pointer to the length of the modulus data
 * \param[in] DerExponentPtr           Pointer to the exponent data
 * \param[in] DerTempExponentLengthPtr Pointer to the length of the exponent data
 * \param[out] TargetKeyPtr            Pointer to the result array, which shall contain modulus and
 *                                 exponent with the corresponding tags and lengths
 * \param[in,out] TargetKeyLengthPtr      Pointer to the length of the result array, contains the
 *                                     length of the written data after computation.
 *
 * \returns Result of the request
 * \retval E_OK                        Request successful
 * \retval E_NOT_OK                    Request failed
 *
 * \Reentrancy{ Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_WriteFormattedKeyRsa
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  DerModulosPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerModulosLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  DerExponentPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerTempExponentLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetKeyPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TargetKeyLengthPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */
/** \brief Copy a key element
 *
 * This function copies the requested key element to the destination key element
 *
 * \param[in] CryptoKeyId         Identifier of the key whose element shall be copied
 * \param[in] KeyElementId        Identifier of the key element that shall be copied
 * \param[in] TargetCryptoKeyId   Identifier of the destination key
 * \param[in] TargetKeyElementId  Identifier of the destination key element
 *
 * \returns Result of the request
 * \retval E_OK                        Request successful
 * \retval E_NOT_OK                    Request failed
 * \retval CRYPTO_E_BUSY               Request failed, crypto driver object is busy
 * \retval CRYPTO_E_KEY_READ           Read access was denied
 * \retval CRYPTO_E_KEY_WRITE          Write access was denied
 * \retval CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 * \retval CRYPTO_E_KEY_SIZE_MISMATCH  Key element sizes are not compatible
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
);

/** \brief Copy all key elements of a key
 *
 * This function copies the key elements of the requested key to the destination key
 *
 * \param[in] CryptoKeyId        Identifier of the key whose elements
 *                               shall be copied
 * \param[in] TargetCryptoKeyId  Identifier of the destination key
 *
 * \returns Result of the request
 * \retval E_OK                        Request successful
 * \retval E_NOT_OK                    Request failed
 * \retval CRYPTO_E_BUSY               Request failed, crypto driver object is busy
 * \retval CRYPTO_E_KEY_READ           Read access was denied
 * \retval CRYPTO_E_KEY_WRITE          Write access was denied
 * \retval CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 * \retval CRYPTO_E_KEY_SIZE_MISMATCH  Key element sizes are not compatible
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief Helper Function for Crypto_xVIx_xAIx_AL_FindKeyElement.
 *
 * This function finds the index of a keyelement at a key
 *
 * \param[in]  CryptoKeyId        KeyId of the key, for which the element index shall be found
 * \param[in]  KeyElementId       ElementId of the key element, for which the element index shall be
 *                                found in the key
 * \param[out] KeyElementIndex    holds the element index in case the element was found for the key
 *
 * \returns whether the elementId was found with the given key.
 * \retval TRUE  the key element was found for the given key. KeyElementIndex holds the index of the
 *               element in the key
 * \retval FALSE the key element is not part of the key
 *
 * \Reentrancy {Reentrant}
 * \Synchronicity {Synchronous}
 *
 */
extern FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_FindKeyElement
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIndex
);

/** \brief Generate internal seed state for requested key
 *
 * This function generates a random seed by using the provided entropy
 *
 * \param[in] CryptoKeyId    Identifier of the crypto key for which
 *                           a seed shall be generated.
 * \param[in] EntropyPtr     Pointer to the memory location which
 *                           contains the entropy data.
 * \param[in] EntropyLength  Length of the entropy in bytes
 *
 * \returns Result of the request
 * \retval E_OK      Request successful
 * \retval E_NOT_OK  Request failed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
);

/** \brief Generate a new key
 *
 * This function generates new key material for the referenced key
 *
 * \param[in] CryptoKeyId  Identifier of the crypto key for which
 *                         key material shall be generated.
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 * \retval CRYPTO_E_BUSY  Request failed, crypto driver object is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyGenerate
(
  uint32 CryptoKeyId
);

/** \brief Derive a key from another key
 *
 * This function generates new key material for the referenced key by deriving it
 * from the key material of the referenced source key
 *
 * \param[in] CryptoKeyId        Identifier of the crypto key that shall
 *                               be used for derivation.
 * \param[in] TargetCryptoKeyId  Identifier of the crypto key where the
 *                               derived key shall be stored.
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 * \retval CRYPTO_E_BUSY  Request failed, crypto driver object is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief Calculate the public value for the key exchange
 *
 * This function calculates the public value for the key exchange and stores the
 * calculated value within the result buffer
 *
 * \param[in] CryptoKeyId               Identifier of the crypto key that shall
 *                                      be used for the key exchange.
 * \param[out] PublicValuePtr           Pointer to the memory location where
                                        the public value shall be stored.
 * \param[in,out] PublicValueLengthPtr  Pointer to the memory location where
 *                                      the length information shall be stored.
 *                                      On calling this function, this location
 *                                      shall contain the size of the provided buffer.
 *
 * \returns Result of the request
 * \retval E_OK                   Request successful
 * \retval E_NOT_OK               Request failed
 * \retval CRYPTO_E_BUSY          Request failed, crypto driver object is busy
 * \retval CRYPTO_E_SMALL_BUFFER  The provided buffer is too small to store the result
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyExchangeCalcPubVal
(
  uint32                                               CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
);

/** \brief Calculate the shared secret for the key exchange
 *
 * This function calculates the shared secret for the key exchange using the partners
 * public value. The result is stored in a key element within the referenced key.
 *
 * \param[in] CryptoKeyId               Identifier of the crypto key that shall
 *                                      be used for the key exchange.
 * \param[out] PartnerPublicValuePtr    Pointer to the memory where the public
 *                                      value of the partner is located.
 * \param[in] PartnerPublicValueLength  Length of the partner's public value in bytes.
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 * \retval CRYPTO_E_BUSY  Request failed, crypto driver object is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyExchangeCalcSecret
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                                PartnerPublicValueLength
);

/** \brief Parse a certificate
 *
 * This function parses the certificate data stored in the key element
 * CRYPTO_KE_CERT_DATA and fills the key elements CRYPTO_KE_CERT_SIGNEDDATA,
 * CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE
 *
 * \param[in] CryptoKeyId  Identifier of the key that contains the certificate
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 * \retval CRYPTO_E_BUSY  Request failed, crypto driver object is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CertificateParse
(
  uint32 CryptoKeyId
);

/** \brief Verify a certificate
 *
 * This function verifies the certificate stored in the key verifyCryptoKeyId with
 * the certificate stored in the key referenced by cryptoKeyId
 *
 * \param[in] CryptoKeyId        Identifier of the key that shall be
 *                               used for verification.
 * \param[in] VerifyCryptoKeyId  Identifier of the key that contains
                                 the certificate.
 * \param[out] VerifyPtr         Pointer to the memory location where
                                 the result of the verification shall be stored.
 *
 * \returns Result of the request
 * \retval E_OK           Request successful
 * \retval E_NOT_OK       Request failed
 * \retval CRYPTO_E_BUSY  Request failed, crypto driver object is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CertificateVerify
(
  uint32                                                                CryptoKeyId,
  uint32                                                                VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Array holding all keys data. */
extern VAR(Crypto_xVIx_xAIx_AL_KeysDataType, CRYPTO_xVIx_xAIx_VAR)
Crypto_xVIx_xAIx_AL_Keys_Data[CRYPTO_XVIX_XAIX_KEY_COUNT];

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*================================================================================================*/

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT) */

#endif /* #ifndef CRYPTO_XVIX_XAIX_AL_KEYMANAGEMENT_H */

/*==================[end of file]=================================================================*/

