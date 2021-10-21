[!CODE!]
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

[!AUTOSPACING!]

/*================================================================================================*/
#ifndef CRYPTO_XVIX_XAIX_AL_CFG_H
#define CRYPTO_XVIX_XAIX_AL_CFG_H

/*==================[includes]====================================================================*/

/*==================[macros]======================================================================*/

/** \brief Macro indicating scheduling type static */
#if (defined CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC)
#error CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC is already defined
#endif
#define  CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC  1U

/** \brief Macro indicating scheduling type dynamic
 */
#if (defined CRYPTO_XVIX_XAIX_AES_SCHEDULE_DYNAMIC)
#error CRYPTO_XVIX_XAIX_AES_SCHEDULE_DYNAMIC is already defined
#endif
#define  CRYPTO_XVIX_XAIX_AES_SCHEDULE_DYNAMIC 2U

/** \brief Macro indicating the used scheduling type
 */
#if (defined CRYPTO_XVIX_XAIX_AES_SCHEDULE)
#error CRYPTO_XVIX_XAIX_AES_SCHEDULE is already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_SCHEDULE CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC
/* CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC  (completed) ||
   CRYPTO_XVIX_XAIX_AES_SCHEDULE_DYNAMIC (pending) */

[!NOCODE!]
[!VAR "CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_NOT_SET[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]0U[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_NOT_SET[!ENDVAR!]
[!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]0U[!ENDVAR!]
[!LOOP "CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*"!]
  [!IF "($CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'ENCRYPT'             and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_AES') and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_ECB')"!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'DECRYPT'             and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_AES') and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_ECB')"!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                            'ENCRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CBC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'DECRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CBC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_GENERATE' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CMAC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_VERIFY' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CMAC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_GENERATE' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_HMAC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_VERIFY' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_HMAC')"!]
      [!VAR "CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_GENERATE' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_SIPHASH_2_4')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'MAC_VERIFY' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_SIPHASH_2_4')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'AEAD_ENCRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_GCM')"!]
      [!VAR "CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                           'AEAD_DECRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_GCM')"!]
      [!VAR "CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SHA1_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'HASH' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_SHA1')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SHA2_224_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'HASH' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SHA2_256_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'HASH' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SHA2_384_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'HASH' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SHA2_512_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'HASH' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'SIGNATURE_GENERATE' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_ED25519')"!]
      [!VAR "CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'SIGNATURE_VERIFY' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_ED25519')"!]
      [!VAR "CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'SIGNATURE_GENERATE' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_ECCNIST')"!]
      [!VAR "CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'SIGNATURE_VERIFY' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_ECCNIST')"!]
      [!VAR "CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'RANDOM'              and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RNG') and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_NOT_SET')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                              'RANDOM'              and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_AES') and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_CTRDRBG')"!]
      [!VAR "CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "as:ref(.)/CryptoPrimitiveService =                        'SIGNATURE_VERIFY'    and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RSA') and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_RSASSA_PSS')"!]
    [!VAR "CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "as:ref(.)/CryptoPrimitiveService =                        'SIGNATURE_VERIFY'    and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RSA') and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5')"!]
    [!VAR "CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED"!]STD_ON[!ENDVAR!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA1')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "as:ref(.)/CryptoPrimitiveService =                        'SIGNATURE_GENERATE'    and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RSA') and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5')"!]
    [!VAR "CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED"!]STD_ON[!ENDVAR!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA1')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]    
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                            'ENCRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CFB')"!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "($CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED != 'STD_ON')"!]
    [!IF "
    as:ref(.)/CryptoPrimitiveService =                            'DECRYPT' and
    node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*, 'CRYPTO_ALGOMODE_CFB')"!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED"!]STD_ON[!ENDVAR!]
    [!ELSE!]
      [!VAR "CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED"!]STD_OFF[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "as:ref(.)/CryptoPrimitiveService =                        'ENCRYPT'             and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RSA') and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_RSAES_OAEP')"!]
    [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED"!]STD_ON[!ENDVAR!]
    [!VAR "CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED"!]STD_ON[!ENDVAR!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_224[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]28U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_256[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]32U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_384[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]48U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_512[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]64U[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "as:ref(.)/CryptoPrimitiveService =                        'DECRYPT'             and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_RSA') and
  node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_RSAES_OAEP')"!]
    [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED"!]STD_ON[!ENDVAR!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_224')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_224[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]28U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_256')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_256[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]32U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_384')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_384[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]48U[!ENDVAR!]
    [!ENDIF!]
    [!IF "node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmSecondaryFamiliy/*, 'CRYPTO_ALGOFAM_SHA2_512')"!]
      [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]CRYPTO_ALGOFAM_SHA2_512[!ENDVAR!]
      [!VAR "CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]64U[!ENDVAR!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
[!// Check for dependencies
[!IF "($CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!IF "($CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!IF "($CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!IF "($CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
  [!VAR "CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!IF "($CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!IF "($CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED = 'STD_ON') or ($CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED = 'STD_ON')"!]
  [!VAR "CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]STD_ON[!ENDVAR!]
[!ENDIF!]
[!//
[!ENDNOCODE!]
#define CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED [!"$CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED"!]
#define CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED [!"$CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED"!]

#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED [!"$CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED"!]
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED [!"$CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED"!]

#define CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED"!]
#define CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED"!]
#define CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED [!"$CRYPTO_XVIX_XAIX_GCM_ENC_ENABLED"!]
#define CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED [!"$CRYPTO_XVIX_XAIX_GCM_DEC_ENABLED"!]

#define CRYPTO_XVIX_XAIX_SHA1_ENABLED [!"$CRYPTO_XVIX_XAIX_SHA1_ENABLED"!]

#define CRYPTO_XVIX_XAIX_SHA2_224_ENABLED [!"$CRYPTO_XVIX_XAIX_SHA2_224_ENABLED"!]
#define CRYPTO_XVIX_XAIX_SHA2_256_ENABLED [!"$CRYPTO_XVIX_XAIX_SHA2_256_ENABLED"!]
#define CRYPTO_XVIX_XAIX_SHA2_384_ENABLED [!"$CRYPTO_XVIX_XAIX_SHA2_384_ENABLED"!]
#define CRYPTO_XVIX_XAIX_SHA2_512_ENABLED [!"$CRYPTO_XVIX_XAIX_SHA2_512_ENABLED"!]

#define CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED"!]
#define CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED"!]
#define CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED [!"$CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED"!]

#define CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED [!"$CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED"!]

#define CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED"!]
#define CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_HMAC_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED [!"$CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED"!]

#define CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED [!"$CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED"!]

#define CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED [!"$CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED"!]
#define CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED [!"$CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED"!]

#define CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED"!]
#define CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED"!]

[!IF "($CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED = 'STD_ON')"!]
#if (defined CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM)
#error CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM is already defined
#endif
/** /brief The configured algorithm for RSAES-OAEP encryption. */
#define CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_ALGORITHM"!]

#if (defined CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH)
#error CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH is already defined
#endif
/** /brief The configured algorithm's output length for RSAES-OAEP encryption. */
#define CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_HASH_LENGTH"!]
[!ENDIF!]

[!IF "($CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED = 'STD_ON')"!]
#if (defined CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM)
#error CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM is already defined
#endif
/** /brief The configured algorithm for RSAES-OAEP decryption. */
#define CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_ALGORITHM"!]

#if (defined CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH)
#error CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH is already defined
#endif
/** /brief The configured algorithm's output length for RSAES-OAEP decryption. */
#define CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH [!"$CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_HASH_LENGTH"!]
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED is already defined
#endif

/** /brief Macro to disable key exchange functionality if no key exchange private key key element is configured. */
[!VAR "curveIdKeyElemCount" = "num:i(count(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '9 = ./CryptoKeyElementId')))"!][!//
[!IF "num:i(0) < num:i($curveIdKeyElemCount)"!][!//
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED)
#error CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED is already defined
#endif

[!VAR "MaxCertSize" = "num:i(0)"!][!//
[!VAR "keyElemCount" = "num:i(count(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '0 = ./CryptoKeyElementId')))"!][!//
[!IF "(num:i(0) < num:i($keyElemCount) and ('STD_ON' = $CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED) and ('STD_ON' = $CRYPTO_XVIX_XAIX_SHA2_256_ENABLED))"!][!//
[!VAR "MaxCertSize" = "num:max(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '0 = ./CryptoKeyElementId')/CryptoKeyElementSize)"!][!//
#define CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED STD_ON

#if (defined CRYPTO_XVIX_XAIX_CERT_MAX_KEY_LENGTH)
#error CRYPTO_XVIX_XAIX_CERT_MAX_KEY_LENGTH is already defined
#endif

#define CRYPTO_XVIX_XAIX_CERT_MAX_KEY_LENGTH [!"num:i($MaxCertSize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED)
#error CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED is already defined
#endif
[!VAR "certCurrTime" = "num:i(count(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '19 = ./CryptoKeyElementId')))"!][!//
[!IF "num:i(0) < num:i($certCurrTime)"!][!//
#define CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED STD_OFF
[!ENDIF!]

/*==================[type definitions]============================================================*/

/*==================[external function declarations]==============================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*================================================================================================*/

#endif /* #ifndef CRYPTO_XVIX_XAIX_AL_CFG_H */
[!ENDCODE!]

