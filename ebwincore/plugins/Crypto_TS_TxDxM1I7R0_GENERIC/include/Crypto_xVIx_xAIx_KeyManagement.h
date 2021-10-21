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

#ifndef CRYPTO_XVIX_XAIX_KEYMANAGEMENT_H
#define CRYPTO_XVIX_XAIX_KEYMANAGEMENT_H

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <Crypto_xVIx_xAIx.h>

/* !LINKSTO EB_Crypto_01019,1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)

/*==================[macros]======================================================================*/

#if(defined CRYPTO_XVIX_XAIX_DATE_SIZE)
#error CRYPTO_XVIX_XAIX_DATE_SIZE is already defined
#endif
/** \brief The size of a date in certificate*/
#define CRYPTO_XVIX_XAIX_DATE_SIZE 6U

#if(defined CRYPTO_XVIX_XAIX_KE_AES_EXPANDEDKEY)
#error CRYPTO_XVIX_XAIX_KE_AES_EXPANDEDKEY is already defined
#endif
/** \brief Key element which can be used together with the key element CRYPTO_KE_MAC_KEY to
 *         precalculate the expanded AES key
 */
#define CRYPTO_XVIX_XAIX_KE_AES_EXPANDEDKEY 1000U

#if(defined CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY1)
#error CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY1 is already defined
#endif
/** \brief Key element which can be used together with the key element CRYPTO_KE_MAC_KEY to
 *         precalculate the AES-CMAC subkey K1
 */
#define CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY1 1001U

#if(defined CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY2)
#error CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY2 is already defined
#endif
/** \brief Key element which can be used together with the key element CRYPTO_KE_MAC_KEY to
 *         precalculate the AES-CMAC subkey K2
 */
#define CRYPTO_XVIX_XAIX_KE_MAC_AESCMAC_SUBKEY2 1002U

#if(defined CRYPTO_XVIX_XAIX_KE_SIGNATURE_BARRETT)
#error CRYPTO_XVIX_XAIX_KE_SIGNATURE_BARRETT is already defined
#endif
/** \brief Key element which can be used to store the barrett of an RSA key. */
#define CRYPTO_XVIX_XAIX_KE_SIGNATURE_BARRETT 1003U

#if (defined CRYPTO_XVIX_XAIX_KE_RSA_ADDITIONAL_INPUT)
#error CRYPTO_XVIX_XAIX_KE_RSA_ADDITIONAL_INPUT is already defined
#endif
/** \brief Key element which can be used to store the RSA additional input. */
#define CRYPTO_XVIX_XAIX_KE_RSA_ADDITIONAL_INPUT 1004U

#if(defined CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_ECC)
#error CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_ECC is already defined
#endif
/** \brief Macros for the signature algorithm type EDDSA used in a
 *         certificate.
 */
#define CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_ECC 1U

#if(defined CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_RSA)
#error CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_RSA is already defined
#endif
/** \brief Macros for the signature algorithm type RSA used in a
 *         certificate.
 */
#define CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_RSA 2U

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Set a key element
 *
 *  This function checks the provided parameters and forwards the key setting request
 *  to the process layer
 *
 *  \param[in] CryptoKeyId   Identifier of the crypto key whose key element shall be set
 *  \param[in] KeyElementId  Identifier of the key element that shall be set
 *  \param[in] KeyPtr        Pointer to the key data that shall be set as the key element
 *  \param[in] KeyLength     Length of the key element in bytes
 *
 *  \returns  Result of the key setting operation
 *  \retval   E_OK                             Request successful
 *  \retval   E_NOT_OK                         Request failed
 *  \retval   CRYPTO_E_BUSY                    Request failed, crypto driver object is busy
 *  \retval   CRYPTO_E_KEY_WRITE_FAIL          Write access was denied
 *  \retval   CRYPTO_E_KEY_NOT_AVAILABLE       The requested key is not available
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH       Provided data size does not match key element size
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementSet
(
  uint32                                                CryptoKeyId,
  uint32                                                KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32                                                KeyLength
);

/** \brief Set a key to valid
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in] CryptoKeyId   Identifier of the crypto key that shall be set to valid
 *
 *  \returns  Result of the request
 *  \retval   E_OK                        Request successful
 *  \retval   E_NOT_OK                    Request failed
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyValidSet
(
  uint32 CryptoKeyId
);

/** \brief   Retrieve the value of a key element
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]  CryptoKeyId       Identifier of the crypto key whose element shall be retrieved
 *  \param[in]  KeyElementId      Identifier of the key element that shall be retrieved
 *  \param[out] ResultPtr         Pointer to the memory location where the key element data shall be
 *                                stored
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
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementGet
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
);

/** \brief      Copy a key element
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]  CryptoKeyId                 Identifier of the key whose element shall be copied
 *  \param[in]  KeyElementId                Identifier of the key element that shall be copied
 *  \param[in]  TargetCryptoKeyId           Identifier of the destination key
 *  \param[in]  TargetKeyElementId          Identifier of the destination key element
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *  \retval     CRYPTO_E_BUSY               Request failed, crypto driver object is busy
 *  \retval     CRYPTO_E_KEY_READ_FAIL      Read access was denied
 *  \retval     CRYPTO_E_KEY_WRITE_FAIL     Write access was denied
 *  \retval     CRYPTO_E_KEY_NOT_AVAILABLE  The requested key is not available
 *  \retval     CRYPTO_E_KEY_SIZE_MISMATCH  Key element sizes are not compatible
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
);

/** \brief      Copy all key elements of a key
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
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
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief     Get the Ids of the key elements available within the requested key
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]    CryptoKeyId            Identifier of the crypto key whose key element ids shall be
 *                                       retrieved
 *  \param[out]   KeyElementIdsPtr       Memory location, where the key element ids shall be stored
 *  \param[in,out] KeyElementIdsLengthPtr Memory location, where the number of key elements shall be
 *                                       stored. On calling this function it shall contain the
 *                                       number of element Ids that can be stored within the
 *                                       provided buffer.
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 *  \retval     CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementIdsGet
(
  uint32                                               CryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIdsLengthPtr
);

/** \brief      Generate internal seed state for requested key
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
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
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
);

/** \brief   Generate a new key
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]  CryptoKeyId              Identifier of the crypto key for which key material shall
 *                                       be generated
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyGenerate
(
  uint32 CryptoKeyId
);

/** \brief   Derive a key from another key
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]  CryptoKeyId              Identifier of the crypto key that shall be used for
 *                                       derivation
 *  \param[in]  TargetCryptoKeyId        Identifier of the crypto key where the derived key shall be
 *                                       stored
 *
 *  \returns    Result of the request
 *  \retval     E_OK                        Request successful
 *  \retval     E_NOT_OK                    Request failed
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
);

/** \brief   Calculate the public value for the key exchange
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the crypto key that shall be used for the
 *                                         key exchange
 *  \param[out]   PublicValuePtr           Pointer to the memory location where the public value
 *                                         shall be stored
 *  \param[in,out] PublicValueLengthPtr    Pointer to the memory location where the length
 *                                         information shall be
 *                                         stored. On calling this function, this location shall
 *                                         contain the size of the provided buffer.
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 *  \retval       CRYPTO_E_SMALL_BUFFER    The provided buffer is too small to store the result
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyExchangeCalcPubVal
(
  uint32                                               CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
);

/** \brief   Calculate the shared secret for the key exchange
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
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
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyExchangeCalcSecret
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                                PartnerPublicValueLength
);

/** \brief   Parse a certificate
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the key that contains the certificate
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateParse
(
  uint32 CryptoKeyId
);

/** \brief   Verify a certificate
 *
 *  This function checks the provided parameters and forwards the request to the
 *  process layer
 *
 *  \param[in]    CryptoKeyId              Identifier of the key that shall be used for verification
 *  \param[in]    VerifyCryptoKeyId        Identifier of the key that contains the certificate
 *  \param[out]   VerifyPtr                Pointer to the memory location where the result of the
 *                                         verification shall be stored
 *
 *  \returns      Result of the request
 *  \retval       E_OK                     Request successful
 *  \retval       E_NOT_OK                 Request failed
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateVerify
(
  uint32                                                                CryptoKeyId,
  uint32                                                                VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*================================================================================================*/

#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

#endif /* #ifndef CRYPTO_XVIX_XAIX_KEYMANAGEMENT_H */

/*==================[end of file]=================================================================*/
