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

/* !LINKSTO EB_Crypto_CryptAlgos_0401, 1 */
/* !LINKSTO EB_Crypto_00040, 1 */

/*==[Includes]====================================================================================*/

#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <TSAutosar.h>
#include <Std_Types.h>
#include <Csm_Types.h>
/* !LINKSTO EB_Crypto_CryptAlgos_0402, 1 */
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_Signature.h>
#include <Crypto_xVIx_xAIx_AL_ECC_NISTP256.h>
#include <Crypto_xVIx_xAIx_AL_Random.h>
#include <Crypto_xVIx_xAIx_AL_Mac.h>
#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_Version.h>
#include <SchM_Crypto_xVIx_xAIx.h>
#include <TSMem.h>

/* !LINKSTO Crypto.Req.Gen/NvM/01019, 1 */
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
#include <NvM.h>
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
#include <Crypto_xVIx_xAIx_AL_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_DerCVC.h>
#include <Crypto_xVIx_xAIx_AL_Base64.h>
#include <Crypto_xVIx_xAIx_AL_RSA.h>
/* !LINKSTO EB_Crypto_01019, 1, EB_Crypto_01021, 1 */
#if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)

#if(STD_ON == CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT)
#include <Det.h>
#endif

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

/*==[Macros]======================================================================================*/

#if(defined CRYPTO_XVIX_XAIX_KDF_DERIVED_KEY_ELEMENT_ID)
#error CRYPTO_XVIX_XAIX_KDF_DERIVED_KEY_ELEMENT_ID is already defined
#endif
/** \brief The element id of the derived key. */
#define CRYPTO_XVIX_XAIX_KDF_DERIVED_KEY_ELEMENT_ID 1U

#if(defined CRYPTO_XVIX_XAIX_CERT_MAX_TAG_NUM)
#error CRYPTO_XVIX_XAIX_CERT_MAX_TAG_NUM is already defined
#endif
/** \brief The maximum number of tags required by a public key stored in a certificate. */
#define CRYPTO_XVIX_XAIX_CERT_MAX_TAG_NUM 4U

#if(defined CRYPTO_XVIX_XAIX_CERT_DATE_ENABLED)
#error CRYPTO_XVIX_XAIX_CERT_DATE_ENABLED is already defined
#endif

#if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
#if(defined CRYPTO_XVIX_XAIX_KDF_FIXEDINPUT_MAX_SIZE)
#error CRYPTO_XVIX_XAIX_KDF_FIXEDINPUT_MAX_SIZE is already defined
#endif
/** \brief The maximum length of the input data for key derivation. */
#define CRYPTO_XVIX_XAIX_KDF_FIXEDINPUT_MAX_SIZE \
CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE + CRYPTO_XVIX_XAIX_MAX_SALT_SIZE + 4U
#endif

/*----- ECC NIST ---------------------------------------------------------------------------------*/

#if(defined CRYPTO_XVIX_XAIX_KEYEXCHANGE)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE is already defined
#endif
/** \brief Used by intent to give a value different from CRYPTO_SIGNATUREGENRATE / CRYPTO_SIGNATUREVERIFY. */
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE 0xFFU

#if(defined CRYPTO_XVIX_XAIX_KEYEXCHANGE_ALGOFAM_LENGTH)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE_ALGOFAM_LENGTH is already defined
#endif
/** \brief The length of the Algorithm Family. */
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_ALGOFAM_LENGTH 1U

#if(defined CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER is already defined
#endif
/** \brief The maximal length of DER-Encoding. */
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER 16U

/*==[Types]=======================================================================================*/

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Searches for a key element and returns its index if present.
 *
 * \param[in]  CryptoKeyId        The id of the key
 * \param[in]  KeyElementId       The id of the key element
 * \param[out] KeyElementIndexPtr Pointer that contains the index of the key element
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGetIndex
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIndexPtr
);

#if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
/** \brief Sets the needed values for the key derivation depending on the algorithm.
 *
 * \param[in]      Algo                   Pointer containing the id of algorithm to be used for key derive.
 * \param[in]      KeyBased               Pointer to boolean indicating if a key based key derivation is used or not.
 * \param[in]      CtrLen                 Pointer to counter length.
 * \param[in]      PrfOutLength           Pointer to the output length of the PRF.
 * \param[in]      CtrPos                 Pointer to the position of the counter value in the FixedInputData.
 * \param[in]      ContextLength          Pointer to the length of the salt.
 * \param[in,out]  FixedInputDataLength   Pointer to the length of the fixes input for key derivation.
 * \param[in]      PasswordLength         Length of the password.
 *
 * \returns Result of the request.
 * \RetVal E_OK      The algorithm is supported and all values were set.
 * \RetVal E_NOT_OK  The algorithm is not supported.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SetKeyDeriveAlgoValues
(
  P2CONST(uint8,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Algo,
    P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  KeyBased,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtrLen,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  PrfOutLength,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtrPos,
  P2CONST(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ContextLength,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  FixedInputDataLength,
          uint32                                           PasswordLength
);

/** \brief Derives a key from a salt and password with the specified algorithm and output length.
 *
 * \param[in]      Algorithm              Pointer containing the id of algorithm to be used for key derive.
 * \param[in]      Password               Pointer to the password.
 * \param[in]      SaltPtr                Pointer to the salt array.
 * \param[in]      SaltLength             Length of the salt.
 * \param[in,out]  OutputPtr              Pointer to the output array.
 * \param[in]      OutputLength           Length of the output which shall be derived.
 *
 * \returns Result of the request.
 * \RetVal  E_OK      Request successful.
 * \RetVal  E_NOT_OK  Request failed.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DeriveKey
(
  P2CONST(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Algorithm,
    P2VAR(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Password,
  P2CONST(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SaltPtr,
          uint32                                                              SaltLength,
    P2VAR(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputPtr,
          uint32                                                              OutputLength
);
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
/** \brief Adds a prevailing zero byte if the most significant bit of the first
 *         byte of the passed source array is set.
 *
 * \param[in,out]  TargetKeyPtr     The array to which the prevailing zero byte would be added.
 * \param[in]      SrcKeyPtr        Pointer to the source array.
 * \param[in]      SrcKeyLengthPtr  length pointer of the source array.

 * \returns length of the Target array.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AddByte2Neg
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  SourcePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TargetLenPtr
);

/** \brief Constructs the length octetes depending upon the given length
 *
 * \param[in]     Length     length of the data whose length needs to be denoted
 *                           in the octets that will be constructed.
 * \param[in,out] TargetPtr  Pointer to the array where the octets will be constructed.

 * \returns length of the Target array.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MakeLengthOctate
(
        uint8                                          Tag,
        uint32                                         Length,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetPtr
);

/*--Certificate_Management------------------------------------------------------------------------------------------*/

/** \brief Extracting BCD encoded date from certificate.
 *
 * Date is in BCD format in the CV and extracting it according to that
 *
 * \param[in] EncodingPtr pointer to the start of the encoding of the value
 * \param[out] date        pointer of the return value where date will be stored
 *
 * \return none
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ParseDate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Date
);

/** \brief Checks the validity dates of the certificates.
 *
 * This function checks if the dates of the verify certificate are the same as that of reference
 * certificate.
 *
 * \param[in] RefDate     pointer to the start of the encoding of the date values of reference
 *                        certificate.
 * \param[in] VerfDate    pointer to the start of the encoding of the date values of verify
 *                        certificate.
 * \returns Std_ReturnType
 * \return E_OK  If the dates are valid
 * \return E_NOT_OK If the dates are invalid
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CheckDates
(
  Crypto_xVIx_xAIx_CVDateInfoType RefDate,
  Crypto_xVIx_xAIx_CVDateInfoType VerfDate
);
#endif

/*--ECDH------------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/** \brief Conditional swap.
 *
 *         This function swaps two numbers with using third variable and bitwise XOR as described
 *         in RFC 7748. The cswap function SHOULD be implemented in constant time.
 *
 * \param[in] Value1  Pointer to the first long number to swap.
 * \param[in] Value2  Pointer to the second long number to swap.
 * \param[in] Dummy   Pointer a buffer which is used as temporary storage during the swapping.
 * \param[in] Swap    Contains swap XOR the current bit of scalar. Is the result 0 the long
 *                    numbers would not be exchanged.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHCSwap
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Dummy,
  Crypto_xVIx_xAIx_LNWordType                                     Swap
);

/** \brief Calculate the public value for the key exchange
 *
 *         This function calculates the public value for the key exchange and stores the
 *         calculated value within the result buffer
 *
 *  \param[in]     CryptoKeyId          Identifier of the crypto key that shall
 *                                      be used for the key exchange.
 *  \param[out]    PublicValuePtr       Pointer to the memory location where the
 *                                      public value shall be stored.
 *  \param[in,out] PublicValueLengthPtr Pointer to the memory location where the
 *                                      length information shall be stored. On calling
 *                                      this function, this location shall contain the size
 *                                      of the provided buffer.
 *  \param[in]    CurveIndex            Index of the Elliptic Curve, which s used for key exchange.
 *
 *  \returns Result of the request
 *  \retval E_OK                   Request successful
 *  \retval E_NOT_OK               Request failed
 *  \retval CRYPTO_E_SMALL_BUFFER  The provided buffer is too small to store the result
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcPubVal
(
  uint32                                               CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr,
  uint8                                      CurveIndex
);

/** \brief Calculate the shared secret for the key exchange
 *
 *         This function calculates the shared secret for the key exchange using the partners
 *         public value. The result is stored in a key element within the referenced key.
 *
 *  \param[in] CryptoKeyId               Identifier of the crypto key that shall
 *                                       be used for the key exchange.
 *  \param[out] PartnerPublicValuePtr    Pointer to the memory where the public
                                         value of the partner is located.
 *  \param[in] PartnerPublicValueLength  Length of the partner's public value in bytes.
 *  \param[in] CurveIndex                Index of the Elliptic Curve, which s used for key exchange.
 *
 *  \returns Result of the request
 *  \retval E_OK                   Request successful
 *  \retval E_NOT_OK               Request failed
 *  \retval CRYPTO_E_SMALL_BUFFER  The provided buffer is too small to store the result
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcSecret
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                                PartnerPublicValueLength,
  uint8                                                 CurveIndex
);

/** \brief Calculate the key.
 *
 *         This function is the recommended Montgomery-X-coordinate DH function and
 *         calculates the public key and the secret shared key.
 *
 *  \param[in] Scalar           Pointer to the memory location where the private
 *                              value is stored:
 *  \param[in] Value            Pointer to the memory location where the public
 *                              value or the u-coordinate is stored.
 *  \param[in,out] Tmp          Pointer to a temporary buffer for the calculation.
 *  \param[out] Result          Pointer to the memory location where the result
 *                              of the calculation shall be stored.
 *  \param[in] CtxPtr           Pointer to the elliptic curve context.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKey
(
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Scalar,
  P2CONST(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Helper Function for Crypto_xVIx_xAIx_ECDH_KeyExchangeCalcKey.
 *
 *         This Function is a Helper Function for Crypto_xVIx_xAIx_ECDH_KeyExchangeCalcKey. It calls
 *         Crypto_xVIx_xAIx_LNMultiplyNumbers until it is Finished.
 *
 *  \param[in,out] Result         Pointer to the memory area which should contain
 *                                the long number representation of the result
 *                                of the multiplication.
 *  \param[in,out] Multiplicand   Pointer to the long number representation of
 *                                the multiplicand.
 *  \param[in,out] Multiplicator  Pointer to the long number representation of
 *                                the multiplicator
 *  \param[in,out] LnCtx          Pointer to a context which holds the current
 *                                state of the multiplication.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    LnCtx
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
/** \brief Helper Function for Crypto_xVIx_xAIx_CertificateVerify.
 *
 *         This Function is a Helper Function for Crypto_xVIx_xAIx_CertificateVerify.
 *         It checks whether the signature algorithm for the reference certificate
 *         matches the signature algorithm of the certificate to be verified.
 *
 *  \param[in] CryptoKeyId         Id of the key in which reference certificate
 *                                 is stored.
 *  \param[in] VerifyCryptoKeyId   Id of the key in which the certificate to be
 *                                 verified is stored.
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CompareSignatureAlgorithm
(
  uint32  CryptoKeyId,
  uint32  VerifyCryptoKeyId
);
#endif

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

/*==[Variables with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
/** \brief The password used for key derivation. */
static VAR(Crypto_xVIx_xAIx_SymKeyType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_KDFPw;
#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
static VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_CertKey =
{
  { 0U },
  CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE
};
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
/** \brief Stores input data for KDF. */
static VAR(uint8, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_FixedInputData[CRYPTO_XVIX_XAIX_KDF_FIXEDINPUT_MAX_SIZE] = { 0U };

/** \brief Internal output buffer for KDF. */
static VAR(uint8, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_OutputBuffer[CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE] = { 0U };

#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
/** \brief Stores the public key of a certificate with its tags and lengths. For each tag we need to
 * store the tag itself (1 byte) and its length (three bytes), i.e. we need three times the maximum
 * number of tags since we do not know which type of key is used and how many tags it requires.
 */
static VAR(uint8, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_PubKey
  [
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE +
    (3U * CRYPTO_XVIX_XAIX_CERT_MAX_TAG_NUM)
  ] = { 0U };

/** \brief Stores the 2'complement of the modulo integer before it is
 * copied back to the target public key buffer.
 */
static VAR(uint8,CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_ModuloInt[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE] = {0U};

/** \brief Stores decoded certificate. */
static VAR(uint8, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_KeyCertDecData[CRYPTO_XVIX_XAIX_CERT_MAX_KEY_LENGTH] = { 0U };

#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_BUSY_KEY_DERIVATION = FALSE;
#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_BUSY_CERT_PARSE = FALSE;
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_BUSY_CERT_VERIFY = FALSE;
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE = FALSE;
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/** \brief Elliptic Curve context. */
static VAR(Crypto_xVIx_xAIx_EllipticCurveCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_EllipticCurveCtx;

/** \brief The private key used for key exchange. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_PrivKey[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS] =
{ 0U };
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

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
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGetPtr
(
  uint32                                                                        CryptoKeyId,
  uint32                                                                        KeyElementId,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                    ResultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 KeyElementIndex;

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetIndex(CryptoKeyId, KeyElementId, &KeyElementIndex);

  if (E_OK == RetVal)
  {
    /*This function is called internally and all the parameters are ensured to be valid before the
      call to this function.*/
    *ResultPtr =
    Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemData;
    *ResultLengthPtr =
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemDataSize;
  }

  return RetVal;
}
#endif

/*--[Crypto_xVIx_xAIx_AL_KeyElementSet]-----------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00075, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementSet
(
  uint32                                                CryptoKeyId,
  uint32                                                KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32                                                KeyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean Found = FALSE;
  uint32 KeyElementIndex;

  /* Find referenced keyElement within the cryptoKey */
  for (
        KeyElementIndex = 0U;
        KeyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
        KeyElementIndex++
      )
  {
    if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Id == KeyElementId)
    {
      Found = TRUE;
      break;
    }
  }

  if (FALSE == Found)
  {
    RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  else if(KeyLength > Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Size)
  {
    /* !LINKSTO EB_Crypto_01092, 1 */
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* write the new value */
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemData,
      KeyPtr,
      KeyLength
    );

    /* assign the new key element data size */
    Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemDataSize = KeyLength;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

#if((STD_ON == CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED) || (STD_ON == CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED))
    if (CRYPTO_KE_MAC_KEY == KeyElementId)
    {
      Crypto_xVIx_xAIx_AL_AESCMAC_PrecalculateKeys(CryptoKeyId);
    }
#endif /* #if((STD_ON == CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED) ||
          (STD_ON == CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED)) */

    RetVal = E_OK;
  }

  return RetVal;
}

/* !LINKSTO EB_Crypto_00080, 1, Crypto.Req.Gen/NvM/01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyValidSet
(
  uint32 CryptoKeyId
)
{
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
  uint32 KeyElementIndex;
  boolean NvMResult = E_NOT_OK;
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
  Std_ReturnType RetVal = E_NOT_OK;

  /* !LINKSTO Crypto.Req.Gen/NvM/01017, 1 */
  Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState = CRYPTO_XVIX_XAIX_KEY_STATE_VALID;
  RetVal = E_OK;

#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
  for (
        KeyElementIndex = 0U;
        KeyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
        KeyElementIndex++
      )
  {

    /* if the Nvram block ID is not 0 that means the data is persistent */
    if(0U !=
        Crypto_xVIx_xAIx_AL_Keys_Data[
                                      CryptoKeyId
                                     ].KeyData[KeyElementIndex].RfKeyElemDataNvramBlockId
      )
    {
      /* !LINKSTO Crypto.Req.Gen/NvM/01001, 2 */
      /* !LINKSTO Crypto.Req.Gen/NvM/01002, 1 */
      NvMResult = NvM_WriteBlock
      (
        Crypto_xVIx_xAIx_AL_Keys_Data[
                                      CryptoKeyId
                                     ].KeyData[KeyElementIndex].RfKeyElemDataNvramBlockId,
        Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemData
      );
      if (E_OK != NvMResult)
      {
        RetVal = E_NOT_OK;
      }
    }
  }
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
  return RetVal;
}
/* !LINKSTO EB_Crypto_00077, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGet
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 KeyElementIndex = 0U;
  uint32 i;

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetIndex(CryptoKeyId, KeyElementId, &KeyElementIndex);

  if (E_OK == RetVal)
  {
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO SWS_Crypto_00092, 1 */
    if (
          *ResultLengthPtr >
          Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemDataSize
       )
    {
      *ResultLengthPtr =
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemDataSize;
    }
    for (i = 0U; i < *ResultLengthPtr; i++)
    {
      ResultPtr[i] =
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[KeyElementIndex].RfKeyElemData[i];
    }
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  }

  return RetVal;
}
/* !LINKSTO EB_Crypto_00078, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 SourceKeyElementIndex;
  uint32 TargetKeyElementIndex;

  /* Find referenced source keyElement within the cryptoKey */

  /* The false case can not be tested because it has already been tested in the IL level function.
     If the element id is not present in the source/target key, this function is not called at all.
     Here we only need to get the internal index of the key element.*/
  /* CHECK: NOPARSE */
  for(
       SourceKeyElementIndex = 0u;
       SourceKeyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
       SourceKeyElementIndex++
     )
  /* CHECK: PARSE */
  {
    if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[SourceKeyElementIndex]->Id == KeyElementId)
    {
      break;
    }
  }

  /* Find referenced target keyElement within the cryptoKey */
  /* The false case can not be tested because it has already been tested in the IL level function.
     If the element id is not present in the source/target key, this function is not called at all.
     Here we only need to get the internal index of the key element.*/
  /* CHECK: NOPARSE */
  for(
       TargetKeyElementIndex = 0u;
       TargetKeyElementIndex < Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements;
       TargetKeyElementIndex++
     )
  /* CHECK: PARSE */
  {
    if(
        Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[TargetKeyElementIndex]->Id ==
        TargetKeyElementId
      )
    {
      break;
    }
  }

  /* Check if the size of the destination is large enough */
  if(
      (
        Crypto_xVIx_xAIx_AL_Keys_Data[
                                       CryptoKeyId
                                     ].KeyData[SourceKeyElementIndex].RfKeyElemDataSize ==
        Crypto_xVIx_xAIx_Keys[
                                       TargetCryptoKeyId
                                     ].KeyType[TargetKeyElementIndex]->Size
      ) ||
      (
        (
          Crypto_xVIx_xAIx_AL_Keys_Data[
                                         CryptoKeyId
                                       ].KeyData[SourceKeyElementIndex].RfKeyElemDataSize <
          Crypto_xVIx_xAIx_Keys[
                                         TargetCryptoKeyId
                                       ].KeyType[TargetKeyElementIndex]->Size
        ) &&
        (
          TRUE ==
          Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[TargetKeyElementIndex]->AllowPartialAccess
        )
      )
    )
  {
    /* !LINKSTO EB_Crypto_01091, 1 */
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
    (
      TargetCryptoKeyId,
      TargetKeyElementId,
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[SourceKeyElementIndex].RfKeyElemData,
      Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[SourceKeyElementIndex].RfKeyElemDataSize
    );
  }
  else
  {
    /* !LINKSTO EB_Crypto_01090, 1 */
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_00079, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 i = 0U;
  uint32 j = 0U;
  /* !LINKSTO EB_Crypto_01060, 1 */
  if(CRYPTO_XVIX_XAIX_KEY_STATE_INVALID != Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState)
  {
    RetVal = E_OK;
    /* !LINKSTO EB_Crypto_01082, 1 */
    /* dry run to test sizes. */
    for(
         i = 0U;
         (i < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements) && (E_OK == RetVal);
         i++
       )
    {
      RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      /* find the referenced source key element. */

      /*It has already been proved in the IL function that the key elements in the source key are
         also found in the target key, and due to this the innner loop can not run till the end.
         The break statement breaks the loop as soos as the key element is found in the target key.
       */
       /* CHECK: NOPARSE */
      for(j = 0U; j < Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements; j++)
       /* CHECK: PARSE */
      {
        if(
            Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->Id ==
            Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Id
          )
        {
          /* Check if the size of the destination is large enough */
          if(
              (
                Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[i].RfKeyElemDataSize ==
                Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Size
              ) ||
              (
                (
                  Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[i].RfKeyElemDataSize <
                  Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Size
                ) &&
                (
                  TRUE ==
                  Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->AllowPartialAccess
                )
              )
            )
          {
            RetVal = E_OK;
          }
          break;
        }
      }
    }
    /* dry run was successful */
    if(E_OK == RetVal)
    {
      /*get the first element id from the source key*/
      for(i = 0U; i < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements; i++)
      {
        /*find the referenced source key element*/
        /*It has already been proved in the IL function that the key elements in the source key are
         also found in the target key, and due to this the innner loop can not run till the end.
         The break statement breaks the loop as soos as the key element is found in the target key.
       */
       /* CHECK: NOPARSE */
        for(j = 0U; j < Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements; j++)
       /* CHECK: PARSE */
        {
          if(
              Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->Id ==
              Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Id
            )
          {
            /* dry run was successful so sizes are correct. Access permissions
               are checked in an upper layer so this function can not fail. */
            (void)Crypto_xVIx_xAIx_AL_KeyElementCopy
                  (
                    CryptoKeyId,
                    Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->Id,
                    TargetCryptoKeyId,
                    Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Id
                  );
             break;
          }
        }
      }
    }
  }
  return RetVal;
}

FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_FindKeyElement
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIndex
)
{
  boolean Found = FALSE;
  uint32  Idx;

  for(
      Idx = 0U;
      Idx < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
      Idx++
      )
  {
    if (KeyElementId == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[Idx]->Id)
    {
      Found            = TRUE;
      *KeyElementIndex = Idx;
      break;
    }
  }
  return Found;
}
/* !LINKSTO EB_Crypto_00081,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (                                                      \
      (STD_ON == CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED) ||  \
      (STD_ON == CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED) \
    )
  uint8 Algorithm[1U] = {0U};
  uint32 AlgorithmLength = sizeof(Algorithm);
  /* !LINKSTO EB_Crypto_01026,1 */
  if (
       E_OK == Crypto_xVIx_xAIx_AL_KeyElementGet
               (
                 CryptoKeyId,
                 CRYPTO_KE_RANDOM_ALGORITHM,
                 Algorithm,
                 &AlgorithmLength
               )
     )
  {
    /* !LINKSTO EB_Crypto_01022,1 */
    switch (Algorithm[0U])
    {
/* !LINKSTO EB_Crypto_01023,1 */
#if (STD_ON == CRYPTO_XVIX_XAIX_SSGGENERATE_ENABLED)
      case CRYPTO_XVIX_XAIX_RANDOMGENERATE_RNG_NOT_SET_ALGORITHM:
      {
        RetVal = Crypto_xVIx_xAIx_AL_RANDOMGENERATE_RNG_NOT_SET_RandomSeed
                 (
                   CryptoKeyId,
                   EntropyPtr,
                   EntropyLength
                 );
        break;
      }
#endif
/* !LINKSTO EB_Crypto_01023,1 */
#if (STD_ON == CRYPTO_XVIX_XAIX_CTRDRBGGENERATE_ENABLED)
      case CRYPTO_XVIX_XAIX_RANDOMGENERATE_AES_CTRDRBG_ALGORITHM:
      {
        RetVal = Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_RandomSeed
                 (
                   CryptoKeyId,
                   EntropyPtr,
                   EntropyLength
                 );
        break;
      }
#endif
      default:
      {
        /* Nothing to do here. */
      }
      break;
    }
  }
#else
  TS_PARAM_UNUSED(CryptoKeyId);
  TS_PARAM_UNUSED(EntropyPtr);
  TS_PARAM_UNUSED(EntropyLength);
#endif
  return RetVal;
}

/* !LINKSTO EB_Crypto_00082, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyGenerate
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(CryptoKeyId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_00083, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  /* !LINKSTO EB_Crypto_CryptAlgos_1001, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1002, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1003, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1004, 1 */
  Std_ReturnType RetVal = E_NOT_OK;

#if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context = NULL_PTR;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Algo    = NULL_PTR;
  uint32 AlgoLength = 0U;
  uint32 KOutLength = 0U;
  uint32 ContextLength = 0U;

  if (CRYPTO_XVIX_XAIX_KEY_STATE_VALID == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState)
  {
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    /* !LINKSTO EB_Crypto_01300, 1 */
    if (FALSE == Crypto_xVIx_xAIx_BUSY_KEY_DERIVATION)
    {
      Crypto_xVIx_xAIx_BUSY_KEY_DERIVATION = TRUE;

      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      Crypto_xVIx_xAIx_KDFPw.length = sizeof(Crypto_xVIx_xAIx_KDFPw.data);
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
               (
                 CryptoKeyId,
                 CRYPTO_KE_KEYDERIVATION_PASSWORD,
                 Crypto_xVIx_xAIx_KDFPw.data,
                 &Crypto_xVIx_xAIx_KDFPw.length
               );
    }
    else
    {
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      RetVal = CRYPTO_E_BUSY;
    }

    if (E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
               (
                 CryptoKeyId,
                 CRYPTO_KE_KEYDERIVATION_SALT,
                 &Context,
                 &ContextLength
               );
    }

    if (E_OK == RetVal)
    {
      /* We need the actual length of the output key element.
         The address stored in the parameter Algo will not be used. */
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
               (
                 TargetCryptoKeyId,
                 CRYPTO_XVIX_XAIX_KDF_DERIVED_KEY_ELEMENT_ID,
                 &Algo,
                 &KOutLength
               );
    }

    if (E_OK == RetVal)
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_1005, 1 */
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
      (
        CryptoKeyId,
        CRYPTO_KE_KEYDERIVATION_ALGORITHM,
        &Algo,
        &AlgoLength
      );
    }

    if (E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_DeriveKey(
                                           Algo,
                                           &Crypto_xVIx_xAIx_KDFPw,
                                           Context,
                                           ContextLength,
                                           Crypto_xVIx_xAIx_OutputBuffer,
                                           KOutLength
                                         );
    }

    if (E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
               (
                 TargetCryptoKeyId,
                 CRYPTO_XVIX_XAIX_KDF_DERIVED_KEY_ELEMENT_ID,
                 Crypto_xVIx_xAIx_OutputBuffer,
                 KOutLength
               );

      if (E_OK == RetVal)
      {
        RetVal = Crypto_xVIx_xAIx_KeyValidSet(TargetCryptoKeyId);
      }
    }
    /* !LINKSTO EB_Crypto_CryptAlgos_1008, 1 */
    if ((RetVal != E_OK) && (RetVal != CRYPTO_E_BUSY))
    {
      RetVal = E_NOT_OK;
    }
  }

  /* !LINKSTO EB_Crypto_01300, 1 */
  Crypto_xVIx_xAIx_BUSY_KEY_DERIVATION = FALSE;

#else /* #if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */
  TS_PARAM_UNUSED(CryptoKeyId);
  TS_PARAM_UNUSED(TargetCryptoKeyId);
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

  return RetVal;
}

/* !LINKSTO EB_Crypto_00084, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyExchangeCalcPubVal
(
  uint32                                     CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
  uint8  AlgoFam;
  uint8  CurveOID[CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER]     = { 0U };

  uint32 CurveOIDLength = CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER;
  uint32 AlgoLength     = CRYPTO_XVIX_XAIX_KEYEXCHANGE_ALGOFAM_LENGTH;
  uint8  CurveIndex     = 0xFFU;

  /* !LINKSTO EB_Crypto_5001, 1 */
  if(CRYPTO_XVIX_XAIX_KEY_STATE_VALID == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState)
  {
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    /* !LINKSTO EB_Crypto_01300, 1 */
    if (FALSE == Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE)
    {
      Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE = TRUE;

      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      if(E_OK != Crypto_xVIx_xAIx_AL_KeyElementGet
                 (
                   CryptoKeyId,
                   CRYPTO_KE_KEYEXCHANGE_ALGORITHM,
                   &AlgoFam,
                   &AlgoLength
                 )
        )
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        if(E_OK != Crypto_xVIx_xAIx_AL_KeyElementGet
                   (
                     CryptoKeyId,
                     CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE,
                     CurveOID,
                     &CurveOIDLength
                   )
          )
        {
          /*  CRYPTO_XVIX_XAIX_KEYEXCHANGE is used by intent here to give a value different from
           *  CRYPTO_SIGNATURE into Crypto_GetEllipticCurve.
           */
          RetVal = Crypto_xVIx_xAIx_GetECIndex(
                                                CRYPTO_XVIX_XAIX_KEYEXCHANGE,
                                                AlgoFam,
                                                NULL_PTR,
                                                0U,
                                                &CurveIndex
                                              );
          if(E_OK == RetVal)
          {
            RetVal = Crypto_xVIx_xAIx_ECDHKeyExchangeCalcPubVal(
                                                                 CryptoKeyId,
                                                                 PublicValuePtr,
                                                                 PublicValueLengthPtr,
                                                                 CurveIndex
                                                               );
          }
        }
        else
        {
          RetVal = Crypto_xVIx_xAIx_GetECIndex(
                                                CRYPTO_XVIX_XAIX_KEYEXCHANGE,
                                                AlgoFam,
                                                CurveOID,
                                                (uint8) CurveOIDLength,
                                                &CurveIndex
                                              );
          if(E_OK == RetVal)
          {
            RetVal = Crypto_xVIx_xAIx_ECDHKeyExchangeCalcPubVal(
                                                                 CryptoKeyId,
                                                                 PublicValuePtr,
                                                                 PublicValueLengthPtr,
                                                                 CurveIndex
                                                               );
          }
        }
      }
      Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE = FALSE;
    }
    else
    {
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      RetVal = CRYPTO_E_BUSY;
    }
  }
#else
  TS_PARAM_UNUSED(CryptoKeyId);
  TS_PARAM_UNUSED(PublicValuePtr);
  TS_PARAM_UNUSED(PublicValueLengthPtr);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

  return RetVal;
}

/* !LINKSTO EB_Crypto_00085, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyExchangeCalcSecret
(
  uint32                                      CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                      PartnerPublicValueLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
  uint8  AlgoFam;
  uint8  CurveOID[CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER]     = { 0U };

  uint32 CurveOIDLength = CRYPTO_XVIX_XAIX_KEYEXCHANGE_CURVE_DER;
  uint32 AlgoLength     = CRYPTO_XVIX_XAIX_KEYEXCHANGE_ALGOFAM_LENGTH;
  uint8  CurveIndex     = 0xFFU;

  /* !LINKSTO EB_Crypto_5000, 1 */
  if(CRYPTO_XVIX_XAIX_KEY_STATE_VALID == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState)
  {
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    /* !LINKSTO EB_Crypto_01300, 1 */
    if (FALSE == Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE)
    {
      Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE = TRUE;

      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      if(E_OK != Crypto_xVIx_xAIx_AL_KeyElementGet
                 (
                   CryptoKeyId,
                   CRYPTO_KE_KEYEXCHANGE_ALGORITHM,
                   &AlgoFam,
                   &AlgoLength
                 )
        )
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        if(E_OK != Crypto_xVIx_xAIx_AL_KeyElementGet
                   (
                     CryptoKeyId,
                     CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE,
                     CurveOID,
                     &CurveOIDLength
                   )
          )
        {
          /*  CRYPTO_XVIX_XAIX_KEYEXCHANGE is used by intent here to give a value different from
           *  CRYPTO_SIGNATURE into Crypto_GetEllipticCurve.
           */
          RetVal = Crypto_xVIx_xAIx_GetECIndex(
                                                CRYPTO_XVIX_XAIX_KEYEXCHANGE,
                                                AlgoFam,
                                                NULL_PTR,
                                                0U,
                                                &CurveIndex
                                              );
          if(E_OK == RetVal)
          {
            RetVal = Crypto_xVIx_xAIx_ECDHKeyExchangeCalcSecret(
                                                                 CryptoKeyId,
                                                                 PartnerPublicValuePtr,
                                                                 PartnerPublicValueLength,
                                                                 CurveIndex
                                                               );
          }
        }
        else
        {
          RetVal = Crypto_xVIx_xAIx_GetECIndex(
                                                CRYPTO_XVIX_XAIX_KEYEXCHANGE,
                                                AlgoFam,
                                                CurveOID,
                                                (uint8) CurveOIDLength,
                                                &CurveIndex
                                              );
          if(E_OK == RetVal)
          {
            RetVal = Crypto_xVIx_xAIx_ECDHKeyExchangeCalcSecret(
                                                                 CryptoKeyId,
                                                                 PartnerPublicValuePtr,
                                                                 PartnerPublicValueLength,
                                                                 CurveIndex
                                                               );
          }
        }
      }
      Crypto_xVIx_xAIx_BUSY_KEY_EXCHANGE = FALSE;
    }
    else
    {
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

      RetVal = CRYPTO_E_BUSY;
    }
  }
#else
  TS_PARAM_UNUSED(CryptoKeyId);
  TS_PARAM_UNUSED(PartnerPublicValuePtr);
  TS_PARAM_UNUSED(PartnerPublicValueLength);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

  return RetVal;
}


/* These Functions are not implemented in this version */
/* !LINKSTO EB_Crypto_00086, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CertificateParse
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

/* Computation relies on existence of CRYPTO_KE_CERTIFICATE_DATA */
/* !LINKSTO EB_Crypto_01100, 1 */
#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
  uint8 SignatureAlgorithm[4U] = {0U, 0U, 0U, 0U};
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SignaturePtr = NULL_PTR;
  uint32 SignatureLength = 0U;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SignedDataPtr = NULL_PTR;
  uint32 SignedDataLength = 0U;
  uint8 OidKeyType = CRYPTO_XVIX_XAIX_KEY_FORMAT_NOT_SUPPORTED;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerModulosPtr = NULL_PTR;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerExponentPtr = NULL_PTR;
  uint32 DerModulosLength = 0U;
  uint32 DerTempExponentLength = 0U;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyCertDataPtr = NULL_PTR;
  uint32 KeyCertDataLength = 0U;
  uint32 KeyCertDecDataLength = CRYPTO_XVIX_XAIX_CERT_MAX_KEY_LENGTH;
  uint32 PubKeyLength = CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE;
  /* As default plaintext is used */
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyCertDecDataPtr = NULL_PTR;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyFormatPtr = NULL_PTR;
  uint32 KeyFormatLength = 0U;
  Crypto_xVIx_xAIx_CVDateInfoType Date;

  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  /* !LINKSTO EB_Crypto_01300, 1 */
  if (FALSE == Crypto_xVIx_xAIx_BUSY_CERT_PARSE)
  {
    Crypto_xVIx_xAIx_BUSY_CERT_PARSE = TRUE;

    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    /* Get the actual data of the certificate from the (mandatory) key element */
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
             (
               CryptoKeyId,
               CRYPTO_KE_CERTIFICATE_DATA,
               &KeyCertDataPtr,
               &KeyCertDataLength
             );
  }
  else
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    RetVal = CRYPTO_E_BUSY;
  }

  /* Only continue if data could be retrieved */
  if (E_OK == RetVal)
  {
    /* Set the retirieved certificate data to KeyCertDecDataPtr.*/
    KeyCertDecDataPtr = KeyCertDataPtr;
    /* Set the retrieved length as maximum length for decoded data */
    KeyCertDecDataLength = KeyCertDataLength;
    /* Parsing format key element is optional */
    /* !LINKSTO EB_Crypto_01101,1 */
    /* Get the parsing format from the key element */
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
    (
      CryptoKeyId,
      CRYPTO_KE_CERTIFICATE_PARSING_FORMAT,
      &KeyFormatPtr,
      &KeyFormatLength
    );
    /* !LINKSTO EB_Crypto_01202,1 */
    if (E_OK == RetVal)
    {
      /* !LINKSTO EB_Crypto_01102,1 */
      if (CRYPTO_XVIX_XAIX_FORMAT_BIN_CERT_CVC == *KeyFormatPtr)
      {
        RetVal = Crypto_xVIx_xAIx_AL_Base64Decode
                 (
                   KeyCertDataPtr,
                   KeyCertDataLength,
                   Crypto_xVIx_xAIx_KeyCertDecData,
                   &KeyCertDecDataLength
                 );

         KeyCertDecDataPtr = Crypto_xVIx_xAIx_KeyCertDecData;
      }
      /* Add here other cases for encoding for ASR 4.3.1 */
    }

    /* Only continue if data could be properly decoded if needed.*/
    if (E_OK == RetVal)
    {
      /* Check the tags and lengths */
      /* !LINKSTO EB_Crypto_01093,1 */
      RetVal = Crypto_xVIx_xAIx_AL_DerCVCCheckCertificate(KeyCertDecDataPtr, KeyCertDecDataLength);

      /* Only continue if format is correct */
      if (E_OK == RetVal)
      {
        /* OID is required for public key, otherwise key type is not known */
        /* !LINKSTO EB_Crypto_01094,1 */
        OidKeyType = Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetOid(KeyCertDecDataPtr);

        /* !LINKSTO EB_Crypto_010945,1 */
        if (CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256 == OidKeyType)
        {
          SignatureAlgorithm[0U] = CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_RSA;

          /* Call KeyElementSet with fixlength of 4U because CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM is of length 4U. */
          /* !LINKSTO EB_Crypto_01200,1 */
          RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                   (
                     CryptoKeyId,
                     CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM,
                     SignatureAlgorithm,
                     4U
                   );
          /* !LINKSTO EB_Crypto_01201,1 */
          if(E_OK == RetVal)
          {
            /* get the root key which consists of an exponent and a modulus and */
            DerModulosPtr =
              Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetModulos(KeyCertDecDataPtr, &DerModulosLength);
            DerExponentPtr =
              Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetExponent(KeyCertDecDataPtr, &DerTempExponentLength);

            /* !LINKSTO EB_Crypto_01096,1 */
            /* This call is ensured not to fail because all the parameters are valid and hence the
               call can not fail. */
            (void)Crypto_xVIx_xAIx_AL_WriteFormattedKeyRsa
                     (
                       DerModulosPtr,
                       &DerModulosLength,
                       DerExponentPtr,
                       &DerTempExponentLength,
                       Crypto_xVIx_xAIx_PubKey,
                       &PubKeyLength
                     );
            RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                     (
                       CryptoKeyId,
                       CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY,
                       Crypto_xVIx_xAIx_PubKey,
                       PubKeyLength
                     );
          }
        }
        /* !LINKSTO EB_Crypto_01094,1 */
        else /* CRYPTO_XVIX_XAIX_KEY_FORMAT_NOT_SUPPORTED */
        {
          RetVal = E_NOT_OK;
        }

        /* Only continue if public key is parsed, since it's mandatory to extract the public key */
        /* !LINKSTO EB_Crypto_01097,1 */
        if (E_OK == RetVal)
        {
          SignaturePtr =
            Crypto_xVIx_xAIx_AL_DerCVCGetSignatureValue(KeyCertDecDataPtr, &SignatureLength);

          /* If the certificate buffer is small or empty Crypto_Base64Decode
           * will return error. And if the certificate is valid (checked with
           * Crypto_xVIx_xAIx_AL_DerCVCCheckCertificate) this function must return valid address
           * (position inside certificate buffer).
           */
          RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                   (
                     CryptoKeyId,
                     CRYPTO_KE_CERTIFICATE_SIGNATURE,
                     SignaturePtr,
                     SignatureLength
                   );


          /* Only continue if signature is parsed, since it is mandatory to extract the signature */
          /* !LINKSTO EB_Crypto_01098,1 */
          if (E_OK == RetVal)
          {
            SignedDataPtr =
              Crypto_xVIx_xAIx_AL_DerCVCGetSignedData(KeyCertDecDataPtr, &SignedDataLength);

            /* If the certificate buffer is small or empty
             * Crypto_Base64Decode will return error. And if the certificate is valid (checked with
             * Crypto_xVIx_xAIx_AL_DerCVCCheckCertificate) this function must return valid address
             * (position inside certificate buffer).
             */
            RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                     (
                       CryptoKeyId,
                       CRYPTO_KE_CERTIFICATE_SUBJECT,
                       SignedDataPtr,
                       SignedDataLength
                     );
            /* Only continue if signed data is parsed, since it is mandatory to extract signed
             * data */
            /* !LINKSTO EB_Crypto_01099,1 */
            if (E_OK == RetVal)
            {
              /* Extract optional key elements */
              /* !LINKSTO EB_Crypto_01208,1 */

              Crypto_xVIx_xAIx_AL_DerCVCGetDate(KeyCertDecDataPtr, &Date);

              RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                       (
                         CryptoKeyId,
                         CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE,
                         Date.EffectiveDate,
                         sizeof(Date.EffectiveDate)
                       );
              if(E_OK == RetVal)
              {

                RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet
                         (
                           CryptoKeyId,
                           CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER,
                           Date.ExpirationDate,
                           sizeof(Date.ExpirationDate)
                         );
                if(E_OK == RetVal)
                {
                  RetVal = Crypto_xVIx_xAIx_KeyValidSet(CryptoKeyId);
                }
              }
            }
          }
        }
      }
    }
  }

  /* !LINKSTO EB_Crypto_01300, 1 */
  Crypto_xVIx_xAIx_BUSY_CERT_PARSE = FALSE;

#else
  TS_PARAM_UNUSED(CryptoKeyId);
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */

  return RetVal;
}
#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_WriteFormattedKeyRsa
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  DerModulosPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerModulosLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  DerExponentPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DerTempExponentLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetKeyPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TargetKeyLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8  TempExponentInt[10U] = {0U};
  uint8  TempModuloLen[5U] = {0U};
  uint32  TempModuloLenOct = 0U;
  uint32  TempExpoLenOct = 0U;
  uint8  TempExponentLen[5U] = {0U};
  uint32 TempModLen = 0U;
  uint32 TempExpLen = 0U;
  uint32 TempTotalKeyLen = 0U;
  uint32  TempSequenceOctetLen = 0U;

  /*Clearing the needed static variables.*/
  TS_MemBZero(Crypto_xVIx_xAIx_ModuloInt,CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE) ;

  /*Prevailing zero byte addition*/
  TempModLen = Crypto_xVIx_xAIx_AL_AddByte2Neg(Crypto_xVIx_xAIx_ModuloInt, DerModulosPtr, DerModulosLengthPtr);
  TempExpLen = Crypto_xVIx_xAIx_AL_AddByte2Neg(TempExponentInt, DerExponentPtr, DerTempExponentLengthPtr);


  /*modulus length octet*/
  TempModuloLenOct = Crypto_xVIx_xAIx_AL_MakeLengthOctate(0x02U, TempModLen, TempModuloLen);
  /*Exponent length octet*/
  TempExpoLenOct = Crypto_xVIx_xAIx_AL_MakeLengthOctate(0x02U, TempExpLen, TempExponentLen);

  TempTotalKeyLen = (uint32)(TempExpoLenOct + TempModuloLenOct + TempExpLen + TempModLen);


  if (*TargetKeyLengthPtr >= TempTotalKeyLen)
  {
    /*Set the sequence tag. This length is not part of TempTotalKeyLen because it is added to the
    length when declaring.*/
    TempSequenceOctetLen = Crypto_xVIx_xAIx_AL_MakeLengthOctate
                           (
                             0x30,
                             TempTotalKeyLen, TargetKeyPtr
                           );

    TS_MemCpy(&TargetKeyPtr[TempSequenceOctetLen], TempModuloLen, TempModuloLenOct);
    TS_MemCpy(&TargetKeyPtr[TempSequenceOctetLen + TempModuloLenOct], Crypto_xVIx_xAIx_ModuloInt, TempModLen);
    TS_MemCpy(
               &TargetKeyPtr[TempSequenceOctetLen + TempModuloLenOct + TempModLen],
               TempExponentLen,
               TempExpoLenOct
             );
    TS_MemCpy(
               &TargetKeyPtr[TempSequenceOctetLen + TempModuloLenOct + TempModLen + TempExpoLenOct],
               TempExponentInt,
               TempExpLen
             );
    *TargetKeyLengthPtr = TempTotalKeyLen + TempSequenceOctetLen;
    RetVal = E_OK;
  }
  return RetVal;
}

static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AddByte2Neg
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  SourcePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TargetLenPtr
)
{
  uint32 TempModLen = 0U;
  if(SourcePtr[0U] > 127U)
  {
    TS_MemCpy(&TargetPtr[1U], SourcePtr, *TargetLenPtr);
    TempModLen = *TargetLenPtr + 1U;
  }
  else
  {
    TS_MemCpy(TargetPtr, SourcePtr, *TargetLenPtr);
    TempModLen = *TargetLenPtr;
  }
  return TempModLen;
}

static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MakeLengthOctate
(
  uint8 Tag,
  uint32  Length,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  TargetPtr
)
{
  uint32 LengthOctate = 0U;
  /*length the modulus*/

  TargetPtr[0U] = Tag;

  /*Check if the length is not representable with short length representation.*/
  if(Length > 0x7F)
  {
    TargetPtr[1U] = 0x82;
    TargetPtr[2U] = 0xFFU & (Length >> 8U);
    TargetPtr[3U] = 0xFFU & Length;
    LengthOctate = 4U;
  }
  else
  {
    TargetPtr[1U] = 0xFFU & Length;
    LengthOctate = 2U;
  }
  return LengthOctate;
}
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */
/* !LINKSTO EB_Crypto_00087,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CertificateVerify
(
  uint32                                                      CryptoKeyId,
  uint32                                                      VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
  Std_ReturnType RetValRefCertSignature = E_NOT_OK;
  Std_ReturnType RetValCertNotBefore = E_NOT_OK;
  Std_ReturnType RetValCertNotAfter = E_NOT_OK;
  Std_ReturnType RetValRefCertNotBefore = E_NOT_OK;
  Std_ReturnType RetValRefCertNotAfter = E_NOT_OK;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SignedDataPtr = NULL_PTR;
  uint32 SignedDataLength = 0U;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SignaturePtr = NULL_PTR;
  uint32 SignatureLength = 0U;
  uint8 SignatureAlgorithm[4U] = {0U, 0U, 0U, 0U};
  uint32 SignatureAlgorithmLength = 4U;
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  Std_ReturnType RetValCurrentTime = E_NOT_OK;
#endif

  Crypto_xVIx_xAIx_CVDateInfoType RefDate =
  {
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
    {0U, 0U, 0U, 0U, 0U, 0U},
#endif
    {0U, 0U, 0U, 0U, 0U, 0U},
    {0U, 0U, 0U, 0U, 0U, 0U}
  };
  Crypto_xVIx_xAIx_CVDateInfoType VerfDate =
  {
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
    {0U, 0U, 0U, 0U, 0U, 0U},
#endif
    {0U, 0U, 0U, 0U, 0U, 0U},
    {0U, 0U, 0U, 0U, 0U, 0U}
  };

  uint32 DateSize = CRYPTO_XVIX_XAIX_DATE_SIZE;

  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  /* !LINKSTO EB_Crypto_01300, 1 */
  if (FALSE == Crypto_xVIx_xAIx_BUSY_CERT_VERIFY)
  {
    Crypto_xVIx_xAIx_BUSY_CERT_VERIFY = TRUE;

    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    RetVal = E_OK;
  }
  else
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    RetVal = CRYPTO_E_BUSY;
  }

  if (E_OK == RetVal)
  {
    /*set the verify pointer to CRYPTO_E_VER_NOT_OK before verifing the certificate.*/
    *VerifyPtr = CRYPTO_E_VER_NOT_OK;

    /*Clearing the needed static Key.*/
    TS_MemBZero(Crypto_xVIx_xAIx_CertKey.data, CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE);
    Crypto_xVIx_xAIx_CertKey.length = CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE;

    /*set the verifyCryptoKeyId to invalid.*/
    Crypto_xVIx_xAIx_Keys[VerifyCryptoKeyId].KeyState = CRYPTO_XVIX_XAIX_KEY_STATE_INVALID;
    /* !LINKSTO EB_Crypto_01208,1 */
    /*Get keyelements of root and verify certifactes*/
    RetValCertNotBefore = Crypto_xVIx_xAIx_AL_KeyElementGet
                          (
                            CryptoKeyId,
                            CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE,
                            RefDate.EffectiveDate,
                            &DateSize
                          );
    RetValCertNotAfter = Crypto_xVIx_xAIx_AL_KeyElementGet
                         (
                           CryptoKeyId,
                           CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER,
                           RefDate.ExpirationDate,
                           &DateSize
                         );
    RetValRefCertNotBefore = Crypto_xVIx_xAIx_AL_KeyElementGet
                             (
                               VerifyCryptoKeyId,
                               CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE,
                               VerfDate.EffectiveDate,
                               &DateSize
                             );
    RetValRefCertNotAfter = Crypto_xVIx_xAIx_AL_KeyElementGet
                            (
                              VerifyCryptoKeyId,
                              CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER,
                              VerfDate.ExpirationDate,
                              &DateSize
                            );
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
    RetValCurrentTime = Crypto_xVIx_xAIx_AL_KeyElementGet
                        (
                          VerifyCryptoKeyId,
                          CRYPTO_KE_CERTIFICATE_CURRENT_TIME,
                          VerfDate.CurrentDate,
                          &DateSize
                        );
#endif
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
    if(
        (E_OK != RetValCertNotBefore) || (E_OK != RetValCertNotAfter)||
        (E_OK != RetValRefCertNotBefore) || (E_OK != RetValRefCertNotAfter) ||
        (E_OK != RetValCurrentTime) ||
        (CRYPTO_XVIX_XAIX_DATE_SIZE != DateSize)
      )
#else
    if(
        (E_OK != RetValCertNotBefore) || (E_OK != RetValCertNotAfter)||
        (E_OK != RetValRefCertNotBefore) || (E_OK != RetValRefCertNotAfter) ||
        (CRYPTO_XVIX_XAIX_DATE_SIZE != DateSize)
      )
#endif /*#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)*/
    {
      RetVal = E_NOT_OK;
    }
  }

  /* !LINKSTO EB_Crypto_01203, 1 */
  /* !LINKSTO EB_Crypto_01204, 1 */
  /*Retrieving of the date keyelements was successful*/
  if(E_OK == RetVal)
  {
    if (E_OK == Crypto_xVIx_xAIx_CheckDates(RefDate, VerfDate))
    {
    /*Check if the dates are valid and in range and the for root and valid certificates are the same */
      if(E_OK == Crypto_xVIx_xAIx_AL_CompareSignatureAlgorithm(CryptoKeyId, VerifyCryptoKeyId))
      {
        /*get the signature algorithm by calling get element.*/
        RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
                 (
                   CryptoKeyId,
                   CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM,
                   SignatureAlgorithm,
                   &SignatureAlgorithmLength
                 );
        if(E_OK == RetVal)
        {
          /* !LINKSTO EB_Crypto_01205, 1 */
          RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
                   (
                     CryptoKeyId,
                     CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY,
                     Crypto_xVIx_xAIx_CertKey.data,
                     &Crypto_xVIx_xAIx_CertKey.length
                   );
          if (E_OK == RetVal)
          {
            RetValRefCertSignature = Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                                     (
                                        VerifyCryptoKeyId,
                                        CRYPTO_KE_CERTIFICATE_SIGNATURE,
                                        &SignaturePtr,
                                        &SignatureLength
                                     );
            if
            (
              (
                E_OK ==
                Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                (
                  VerifyCryptoKeyId,
                  CRYPTO_KE_CERTIFICATE_SUBJECT,
                  &SignedDataPtr,
                  &SignedDataLength
                )
              ) && (E_OK == RetValRefCertSignature)
            )
            {
              switch (SignatureAlgorithm[0U])
              {
                case CRYPTO_XVIX_XAIX_SIGNATURE_ALGORITHM_RSA:
                {
                  /*If the call is not successful, the VerifyPtr shall be CRYPTO_E_VER_NOT_OK,
                  the return value can be ignored.*/
                  /*Call RSA SHA 256 to verify the certificates.*/
                  RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_RSA_RSASSA_PSS_Helper_SyncSingleCall
                  (
                    SignedDataPtr,
                    SignedDataLength,
                    SignaturePtr,
                    SignatureLength,
                    &Crypto_xVIx_xAIx_CertKey,
                    VerifyPtr
                  );
                  if(CRYPTO_E_VER_OK == *VerifyPtr)
                  {
                    /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00178, 1 */
                    RetVal = Crypto_xVIx_xAIx_KeyValidSet(VerifyCryptoKeyId);
                  }
                }
                break;

                default:
                {
                  /*Algorithm not supported.*/
                  RetVal = E_NOT_OK;
                }
                break;

              }
            }
            else
            {
              /* !LINKSTO EB_Crypto_01206, 1 */
              /* !LINKSTO EB_Crypto_01207, 1 */
              RetVal = E_NOT_OK;
            }
          }
          else
          {
            RetVal = E_NOT_OK;
          }
        }
      }
      else
      {
        RetVal = E_NOT_OK;
      }
    }
  }

  /* !LINKSTO EB_Crypto_01300, 1 */
  Crypto_xVIx_xAIx_BUSY_CERT_VERIFY = FALSE;

#else
TS_PARAM_UNUSED(CryptoKeyId);
TS_PARAM_UNUSED(VerifyCryptoKeyId);
TS_PARAM_UNUSED(VerifyPtr);
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */
  return RetVal;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_KeyElementGetIndex
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIndexPtr
)
{
Std_ReturnType RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;

  for (
        *KeyElementIndexPtr = 0U;
        *KeyElementIndexPtr < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
        (*KeyElementIndexPtr)++
      )
  {
    if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[*KeyElementIndexPtr]->Id == KeyElementId)
    {
      RetVal = E_OK;
      break;
    }
  }

  return RetVal;
}

/*--ECDH -----------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcPubVal
(
  uint32                                               CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr,
  uint8                                                CurveIndex
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  uint32 PrivateKeyLength = Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PrivateKeyPtr;

  /* Check output buffer size */
  if (*PublicValueLengthPtr < Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength)
  {
    RetVal = CRYPTO_E_SMALL_BUFFER;
  }
  /* Get the private key from key element */
  /* !LINKSTO EB_Crypto_5003, 1 */
  else if(E_OK == Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                  (
                    CryptoKeyId,
                    CRYPTO_KE_KEYEXCHANGE_PRIVKEY,
                    &PrivateKeyPtr,
                    &PrivateKeyLength
                  )
         )
  {
    /* Check private key size */
    if (PrivateKeyLength == Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength)
    {
      /* Reset context */
      TS_MemBZero(&Crypto_xVIx_xAIx_EllipticCurveCtx, sizeof(Crypto_xVIx_xAIx_EllipticCurveCtxType));
      TS_MemBZero(&Crypto_xVIx_xAIx_PrivKey, sizeof(Crypto_xVIx_xAIx_PrivKey));

      /* Set the length of private key */
      Crypto_xVIx_xAIx_PrivKey[0U] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength);

      /* Set the curve index */
      Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx.CurveIndex = CurveIndex;

      if(CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE == CurveIndex)
      {
        (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber(
                                                      PrivateKeyPtr,
                                                      PrivateKeyLength,
                                                      Crypto_xVIx_xAIx_PrivKey,
                                                      sizeof(Crypto_xVIx_xAIx_PrivKey) /
                                                        sizeof(Crypto_xVIx_xAIx_PrivKey[0U])
                                                    );

        /* Set the length of private key again for the bitmask patches */
        Crypto_xVIx_xAIx_PrivKey[0U] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength);

        /* clear the first 3 bits */
        Crypto_xVIx_xAIx_PrivKey[1U] &= (Crypto_xVIx_xAIx_LNWordType)(~((Crypto_xVIx_xAIx_LNWordType)7U));

        /* clear the highest bit of the last octet from the first hash half */
        Crypto_xVIx_xAIx_PrivKey[Crypto_xVIx_xAIx_PrivKey[0U]] &= (Crypto_xVIx_xAIx_LNWordType)(~(CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT));

        /* set the second highest bit of the final octet from the first hash half */
        Crypto_xVIx_xAIx_PrivKey[Crypto_xVIx_xAIx_PrivKey[0U]] |= (Crypto_xVIx_xAIx_LNWordType)((CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT) >> 1U);

        /* !LINKSTO SWS_Crypto_00109, 1 */
        Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKey(
                                                 Crypto_xVIx_xAIx_PrivKey,
                                                 Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].Coeff1, /* UCoordinate*/
                                                 Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                               );

        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNNumberToLEByteArray(Crypto_xVIx_xAIx_EllipticCurveCtx.Result.XValue, PublicValuePtr, PublicValueLengthPtr);
      }
      /* ECC NIST Curves */
      else
      {
        /* Get the private key from Crypto_xVIx_xAIx_KeyElements */
        /* !LINKSTO EB_Crypto_5003, 1 */
        (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber(
                                                      PrivateKeyPtr,
                                                      PrivateKeyLength,
                                                      Crypto_xVIx_xAIx_PrivKey,
                                                      sizeof(Crypto_xVIx_xAIx_PrivKey) /
                                                        sizeof(Crypto_xVIx_xAIx_PrivKey[0U])
                                                    );

        /* Calculate the Public key: Secret key * Base Point */
        Crypto_xVIx_xAIx_ECPointMultiplication(
                                                &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),   /* Public key */
                                                &(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].P),
                                                Crypto_xVIx_xAIx_PrivKey,
                                                Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                              );

          /* Since the calculation returns a point in standard projective
           * coordinates, a transformation to affine coordinates is needed.
           */
          Crypto_xVIx_xAIx_ECPointConvertProj2Aff(
                                                   &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),
                                                   Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                   &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                                 );

        /* The result of point multiplication is a point of the elliptic curve
         * Store the x coordinate in first half of the PublicValuePtr and y-coordinate in second half
         */
        (void)Crypto_xVIx_xAIx_LNNumberToBEByteArrayWithSize
              (
                Crypto_xVIx_xAIx_EllipticCurveCtx.Result.XValue,
                PublicValuePtr,
                ((uint32)Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength >> 1U)
              );

        (void)Crypto_xVIx_xAIx_LNNumberToBEByteArrayWithSize
              (
                Crypto_xVIx_xAIx_EllipticCurveCtx.Result.YValue,
                &PublicValuePtr[(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength >> 1U)],
                ((uint32)Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength >> 1U)
              );
      }

      *PublicValueLengthPtr = Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength;

      /* Store the public key in the key element */
      if(E_OK != Crypto_xVIx_xAIx_AL_KeyElementSet(
                                                    CryptoKeyId,
                                                    CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY,
                                                    PublicValuePtr,
                                                    Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength
                                                  )
        )
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        /* everything was ok  */
        RetVal = E_OK;
      }

      /* Reset context */
      TS_MemBZero(&Crypto_xVIx_xAIx_EllipticCurveCtx, sizeof(Crypto_xVIx_xAIx_EllipticCurveCtxType));
      TS_MemBZero(&Crypto_xVIx_xAIx_PrivKey, sizeof(Crypto_xVIx_xAIx_PrivKey));
    }
  }
  else
  {
    /* This empty else is required by MISRA-C:2012 */
  }

  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcSecret
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                                PartnerPublicValueLength,
  uint8                                                 CurveIndex
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 TmpArray[CRYPTO_XVIX_XAIX_ECDH_ARRAY_LEN] = { 0U };
  uint32 TmpArrayLength = Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PrivateKeyPtr;

  /* Check output buffer size */
  if (PartnerPublicValueLength != Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength)
  {
    /* !LINKSTO EB_Crypto_5004, 1 */
    RetVal = CRYPTO_E_SMALL_BUFFER;
  }
  /* Get the private key from key element */
  /* !LINKSTO EB_Crypto_5002, 1 */
  else if(E_OK == Crypto_xVIx_xAIx_AL_KeyElementGetPtr
                  (
                    CryptoKeyId,
                    CRYPTO_KE_KEYEXCHANGE_PRIVKEY,
                    &PrivateKeyPtr,
                    &TmpArrayLength
                  )
         )
  {
    /* Check private key size */
    if (TmpArrayLength == Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength)
    {
      /* Reset context */
      TS_MemBZero(&Crypto_xVIx_xAIx_EllipticCurveCtx, sizeof(Crypto_xVIx_xAIx_EllipticCurveCtxType));
      TS_MemBZero(&Crypto_xVIx_xAIx_PrivKey, sizeof(Crypto_xVIx_xAIx_PrivKey));

      /* Set the length of private key */
      Crypto_xVIx_xAIx_PrivKey[0U] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength);

      Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx.CurveIndex = CurveIndex;

      if(CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE == CurveIndex)
      {
        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber(
                                                      PrivateKeyPtr,
                                                      Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength,
                                                      Crypto_xVIx_xAIx_PrivKey,
                                                      sizeof(Crypto_xVIx_xAIx_PrivKey) /
                                                        sizeof(Crypto_xVIx_xAIx_PrivKey[0U])
                                                    );

        /* Set the length of private key again for the bitmask patches */
        Crypto_xVIx_xAIx_PrivKey[0U] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength);

        /* Clear the first 3 bits */
        Crypto_xVIx_xAIx_PrivKey[1U] &= (Crypto_xVIx_xAIx_LNWordType)(~((Crypto_xVIx_xAIx_LNWordType)7U));

        /* Clear the highest bit of the last octet from the first hash half */
        Crypto_xVIx_xAIx_PrivKey[Crypto_xVIx_xAIx_PrivKey[0U]] &= (Crypto_xVIx_xAIx_LNWordType)(~(CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT));

        /* Set the second highest bit of the final octet from the first hash half */
        Crypto_xVIx_xAIx_PrivKey[Crypto_xVIx_xAIx_PrivKey[0U]] |= (Crypto_xVIx_xAIx_LNWordType)((CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT) >> 1U);

        /* Set the length of the partner public key */
        Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue[0U] = Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength;

        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber(
                                                      PartnerPublicValuePtr,
                                                      PartnerPublicValueLength,
                                                      Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue,
                                                      sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue) /
                                                        sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue[0U])
                                                    );

        Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKey(
                                                 Crypto_xVIx_xAIx_PrivKey,
                                                 Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue,
                                                 Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                               );

        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNNumberToLEByteArray(Crypto_xVIx_xAIx_EllipticCurveCtx.Result.XValue, TmpArray, &TmpArrayLength);
      }
      /* ECC NIST Curves */
      else
      {
        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber(
                                                      PrivateKeyPtr,
                                                      Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PrivateKeyLength,
                                                      Crypto_xVIx_xAIx_PrivKey,
                                                      sizeof(Crypto_xVIx_xAIx_PrivKey) /
                                                        sizeof(Crypto_xVIx_xAIx_PrivKey[0U])
                                                    );

        /* Store the partner public key as a point in projective coordinate for the multiplication */
        Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.ZValue[0U] = 1U;
        /* Set the length of the public key */
        Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.ZValue[1U] = 1U;
        Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.YValue[0U] = ((Crypto_xVIx_xAIx_LNWordType)Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].PublicKeyLength >> (Crypto_xVIx_xAIx_LNWordType)1U);
        Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue[0U] = Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.YValue[0U];

        /*Set the X-coordinate */
        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
              (
                PartnerPublicValuePtr,
                (PartnerPublicValueLength >> 1U),
                Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue,
                sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue) /
                  sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.XValue[0U])
              );

        /* Set the Y-coordinate */
        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
              (
                &PartnerPublicValuePtr[(PartnerPublicValueLength >> 1U)],
                (PartnerPublicValueLength >> 1U),
                Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.YValue,
                sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.YValue) /
                  sizeof(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint.YValue[0U])
              );

        /* Calculate the shared secret key: secret key * PartnerPublicValuePtr */
        Crypto_xVIx_xAIx_ECPointMultiplication(
                                                &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),   /* shared secret key */
                                                &(Crypto_xVIx_xAIx_EllipticCurveCtx.PubPoint),
                                                Crypto_xVIx_xAIx_PrivKey,
                                                Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                              );

        /* Since the calculation returns a point in standard projective
         * coordinates, a transformation to affine coordinates is needed.
         */
        Crypto_xVIx_xAIx_ECPointConvertProj2Aff(
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.Result),
                                                 Crypto_xVIx_xAIx_EllipticCurveCtx.TmpValue,
                                                 &(Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx)
                                               );

        /* Store X-coordinate */
        /* Return value can be ignored as the length is already checked */
        (void)Crypto_xVIx_xAIx_LNNumberToBEByteArrayWithSize
              (
                Crypto_xVIx_xAIx_EllipticCurveCtx.Result.XValue,
                TmpArray,
                TmpArrayLength
              );
      }

      /* store the secret shared key in key element */
      if(
          E_OK != Crypto_xVIx_xAIx_AL_KeyElementSet(
                                                     CryptoKeyId,
                                                     CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE,
                                                     TmpArray,
                                                     TmpArrayLength
                                                   )
        )
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        /* everything was ok */
        RetVal = E_OK;
      }

      /* Reset context */
      TS_MemBZero(&Crypto_xVIx_xAIx_EllipticCurveCtx, sizeof(Crypto_xVIx_xAIx_EllipticCurveCtxType));
      TS_MemBZero(&Crypto_xVIx_xAIx_PrivKey, sizeof(Crypto_xVIx_xAIx_PrivKey));
    }
  }
  else
  {
    /* This empty else is required by MISRA-C:2012 */
  }

  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKey
(
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Scalar,
  P2CONST(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  uint32 i = 0U;
  Crypto_xVIx_xAIx_LNWordType Swap = 0U;
  /* Current bit of the scalar */
  Crypto_xVIx_xAIx_LNWordType CurrentBit;
  Crypto_xVIx_xAIx_LNWordType Bit;
  /* buffer which will be used as temporary storage */
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) X2 = &Tmp[0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Z2 = &Tmp[1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp1 = &Tmp[2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp2 = &Tmp[3U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) X3 = Result->XValue;
  CONSTP2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Z3 = Result->ZValue;

  /* x2 = 1 */
  X2[0U] = 1U;
  X2[1U] = 1U;

  /* z2 = 0 */
  Z2[0U] = 1U;
  Z2[1U] = 0U;

  /* z3 = 1 */
  Z3[0U] = 1U;
  Z3[1U] = 1U;

  /* u-coordinate and the public key of partner */
  /* x3 = value */
  X3[0U] = Value[0U];
  for(i = 1U; i <= Value[0U]; i++)
  {
    X3[i] = Value[i];
  }

  /* The ladder-step computation */
  CurrentBit = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Scalar);

  while(0U < CurrentBit)
  {
    CurrentBit--;
    Bit = CRYPTO_XVIX_XAIX_LN_GET_BIT(Scalar, CurrentBit);
    Swap ^= Bit;
    /* conditional swap */
    Crypto_xVIx_xAIx_ECDHCSwap(X2, X3, Tmp1, Swap);
    Crypto_xVIx_xAIx_ECDHCSwap(Z2, Z3, Tmp1, Swap);
    Swap = Bit;

    /* C: Tmp1 = X3 + Z3 */
    Crypto_xVIx_xAIx_LNAdditionModular(Tmp1, X3, Z3, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* D: x3 = x3 - z3 */
    Crypto_xVIx_xAIx_LNSubtractionModular(X3, X3, Z3, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* A: z3 = x2 + z2 */
    Crypto_xVIx_xAIx_LNAdditionModular(Z3, X2, Z2, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* B: x2 = x2 - z2 */
    Crypto_xVIx_xAIx_LNSubtractionModular(X2, X2, Z2, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* CB = C * B: tmp1 = tmp1 * X2 */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(Tmp1, Tmp1, X2, &CtxPtr->LNCtx);

    /* DA = D * A: x3 = x3 * z3 */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(X3, X3, Z3, &CtxPtr->LNCtx);

    /* A^2: z3 = z3 * z3 */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(Z3, Z3, Z3, &CtxPtr->LNCtx);

    /* B^2: x2 = x2 * x2 */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(X2, X2, X2, &CtxPtr->LNCtx);

    /* E = A^2 - B^2: tmp2 = Z3 - X2 : tmp2 */
    Crypto_xVIx_xAIx_LNSubtractionModular(Tmp2, Z3, X2, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* z2 = a24(Coeff2) * E */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper
    (
      Z2,
      Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Coeff2,
      Tmp2,
      &CtxPtr->LNCtx
    );

    /* (A^2 + a24 * E): z2 = z2 + z3 */
    Crypto_xVIx_xAIx_LNAdditionModular(Z2, Z3, Z2, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* E * (A^2 + a24 * E): z2 = z2 * tmp2 --> final result */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(Z2, Tmp2, Z2, &CtxPtr->LNCtx);

    /* A^2 * B^2: x2 = z3 * x2 --> final result */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(X2, Z3, X2, &CtxPtr->LNCtx);

    /* DA - CB: z3 = x3 - tmp1 */
    Crypto_xVIx_xAIx_LNSubtractionModular(Z3, X3, Tmp1, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* (DA - CB)^2: z3 */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(Z3, Z3, Z3, &CtxPtr->LNCtx);

    /* z3: value * (DA - CB)^2 = value * z3 --> final result */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(Z3, Value, Z3, &CtxPtr->LNCtx);

    /* DA + CB: x3 = x3 + tmp1 */
    Crypto_xVIx_xAIx_LNAdditionModular(X3, X3, Tmp1, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

    /* x3: (DA + CB)^2 = x3 * x3 --> final result */
    Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(X3, X3, X3, &CtxPtr->LNCtx);

  }
  /* conditional swap */
  Crypto_xVIx_xAIx_ECDHCSwap(X2, X3, Tmp1, Swap);
  Crypto_xVIx_xAIx_ECDHCSwap(Z2, Z3, Tmp1, Swap);
  Crypto_xVIx_xAIx_LNInversion(Tmp1, Z2, X3, Z3, Tmp2, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime, &CtxPtr->LNCtx);

  /* result = x2 * 1/z2 */
  Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper(X3, X2, Tmp1, &CtxPtr->LNCtx);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHCSwap
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Dummy,
  Crypto_xVIx_xAIx_LNWordType                                               Swap
)
{
  Crypto_xVIx_xAIx_LNWordType i;
  Crypto_xVIx_xAIx_LNWordType Mask = 0U;

  if((Value1[0U] != Value2[0U]) && (1U == Swap))
  {
    Value1[0U] ^= Value2[0U];
    Value2[0U] ^= Value1[0U];
    Value1[0U] ^= Value2[0U];
  }

  Dummy[0U] = 8U;
  /* dummy = value1 XOR value2 dummy[0] stores the length*/
  for(i = 1U; i <= Dummy[0U]; i++)
  {
    Dummy[i] = Value1[i] ^ Value2[i];
  }

  /* mask(swap) = 0 - swap */
  if(1U == Swap)
  {
    Mask = 0xFFFFFFFFUL;
  }

  /* dummy = mask(swap) AND dummy */
  for(i = 1U; i <= Dummy[0U]; i++)
  {
    Dummy[i] &= Mask;
  }

  /* value1 = value1 XOR dummy */
  for(i = 1U; i <= Dummy[0U]; i++)
  {
    Value1[i] ^= Dummy[i];
  }

  /* value2 = value2 XOR dummy */
  for(i = 1U; i <= Dummy[0U]; i++)
  {
    Value2[i] ^= Dummy[i];
  }
}

/* Helper Function for Crypto_xVIx_xAIx_ECDH_KeyExchangeCalcKey */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDHKeyExchangeCalcKeyHelper
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    LnCtx
)
{
  Crypto_xVIx_xAIx_LNMultiplyNumbers(Result, Multiplicand, Multiplicator, LnCtx);
  Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx.CurveIndex].FastReductionFun
  (
    Result,
    &Crypto_xVIx_xAIx_EllipticCurveCtx.ECCtx
  );
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_CompareSignatureAlgorithm
(
  uint32  CryptoKeyId,
  uint32  VerifyCryptoKeyId
)
{
  Std_ReturnType IsSameSigAlgo = E_NOT_OK;
  Std_ReturnType VrfyCertAlgo = E_NOT_OK;
  uint8 RefSigAlgo[4U] =   {0u, 0u, 0u, 0u};
  uint8 VerfySigAlgo[4U] = {0u, 0u, 0u, 0u};
  uint32 RefSigAlgoLength = 4U;
  uint32 VerfySigAlgoLength = 4U;

  VrfyCertAlgo = Crypto_xVIx_xAIx_AL_KeyElementGet
                 (
                   VerifyCryptoKeyId,
                   CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM,
                   VerfySigAlgo,
                   &VerfySigAlgoLength
                 );
  if(
      (
        E_OK ==
        Crypto_xVIx_xAIx_AL_KeyElementGet
        (
          CryptoKeyId,
          CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM,
          RefSigAlgo,
          &RefSigAlgoLength
        )
      ) && (E_OK == VrfyCertAlgo)

    )
  {
    if(RefSigAlgo[0U] == VerfySigAlgo[0U])
    {
      IsSameSigAlgo = E_OK;
    }
  }
  return IsSameSigAlgo;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CheckDates
(
  Crypto_xVIx_xAIx_CVDateInfoType RefDate,
  Crypto_xVIx_xAIx_CVDateInfoType VerfDate
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 RefEffectiveDate = 0U;
  uint32 RefExpireDate = 0U;
  uint32 VerfEffectiveDate = 0U;
  uint32 VerfExpireDate = 0U;
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  uint32 VerfCurrentDate = 0U;
#endif

  /*get the current date for the */


  /*get the reference dates in uint32 format.*/
  Crypto_xVIx_xAIx_ParseDate(RefDate.EffectiveDate, &RefEffectiveDate);
  Crypto_xVIx_xAIx_ParseDate(RefDate.ExpirationDate, &RefExpireDate);

  /*get the verify dates in uint32 format.*/
  Crypto_xVIx_xAIx_ParseDate(VerfDate.EffectiveDate, &VerfEffectiveDate);
  Crypto_xVIx_xAIx_ParseDate(VerfDate.ExpirationDate, &VerfExpireDate);
#if  (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  Crypto_xVIx_xAIx_ParseDate(VerfDate.CurrentDate, &VerfCurrentDate);
#endif


  /*Check the effective date.*/
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  if (
       (VerfEffectiveDate >= RefEffectiveDate) &&
       (VerfEffectiveDate <= RefExpireDate) &&
       (VerfExpireDate <= RefExpireDate)
       && (VerfCurrentDate >= VerfEffectiveDate)
       && (VerfCurrentDate <= VerfExpireDate)
     )
#else
  if (
       (VerfEffectiveDate >= RefEffectiveDate) &&
       (VerfEffectiveDate <= RefExpireDate) &&
       (VerfExpireDate <= RefExpireDate)
     )
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON) */
  {
    RetVal = E_OK;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ParseDate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Date
)
{
  uint32 Year = 0U;
  uint32 Month = 0U;
  uint32 Day = 0U;

  *Date = 0U;

  Year  = (uint32) (uint8) ((Encoding[0U] * 10U) + Encoding[1U]);
  Month = (uint32) (uint8) ((Encoding[2U] * 10U) + Encoding[3U]);
  Day   = (uint32) (uint8) ((Encoding[4U] * 10U) + Encoding[5U]);

  *Date = ((Year << 16U)) | (Month << 8U) | Day;
}
#endif/* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */

#if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SetKeyDeriveAlgoValues
(
  P2CONST(uint8,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Algo,
    P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  KeyBased,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtrLen,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  PrfOutLength,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtrPos,
  P2CONST(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ContextLength,
    P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  FixedInputDataLength,
          uint32                                           PasswordLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  switch (*Algo)
  {
#if (STD_ON == CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED)
    case CRYPTO_XVIX_XAIX_MACGENERATE_SHA2_256_HMAC_ALGORITHM:
    {
      /* A key based key derivation function is used. */
      *KeyBased = TRUE;
      /* Counter size is 4 byte */
      *CtrLen = 4U;
      /* Set the output length of the PRF */
      *PrfOutLength = 32U;
      /* Set the position of the counter value in the FixedInputData */
      *CtrPos = 0U;
      /* The FixedInputData is a concatenation of (ctr || salt) */
      *FixedInputDataLength = *CtrLen + *ContextLength;
      RetVal = E_OK;
    }
    break;
#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED) */

#if (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
    case CRYPTO_XVIX_XAIX_HASH_SHA2_256_ALGORITHM:
    {
      /* A hash based key derivation function is used. */
      *KeyBased = FALSE;
      /* Counter size is 1 byte */
      *CtrLen = 1U;
      /* Set the output length of the PRF */
      *PrfOutLength = 32U;
      /* Set the position of the counter value in the FixedInputData */
      *CtrPos = PasswordLength;
      /* The FixedInputData is a concatenation of (key || ctr || salt) */
      *FixedInputDataLength = PasswordLength + *CtrLen + *ContextLength;
      RetVal = E_OK;
    }
    break;
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED) */

    default:
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_1007, 1 */
      RetVal = E_NOT_OK;
    }
    break;
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DeriveKey
(
  P2CONST(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Algorithm,
    P2VAR(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Password,
  P2CONST(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SaltPtr,
          uint32                                                              SaltLength,
    P2VAR(uint8,                       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputPtr,
          uint32                                                              OutputLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* !LINKSTO EB_Crypto_CryptAlgos_1010, 1 */
  uint32 Ctr = 0U;
  uint32 N = 0U;
  uint32 j = 0U;
  uint32 CtrLen = 0U;
  uint32 KOutLength = OutputLength;
  uint32 KOutLengthSum = 0U;
  uint32 FixedInputDataLength = 0U;
  uint32 PrfOutLength = 0U;
  uint32 CtrPos = 0U;
  boolean KeyBased = FALSE;

  RetVal = Crypto_xVIx_xAIx_SetKeyDeriveAlgoValues(
                                                    Algorithm,
                                                    &KeyBased,
                                                    &CtrLen,
                                                    &PrfOutLength,
                                                    &CtrPos,
                                                    &SaltLength,
                                                    &FixedInputDataLength,
                                                    Password->length
                                                  );

  if (E_OK == RetVal)
  {
    /* Calculate the number of runs, this depends on the length of the PRFs output.
     * This is a ceiling function.
     */
    N = (uint32) (1U + ((KOutLength - 1U) / PrfOutLength));
    /* check for counter overflow */
    if((1U == CtrLen) &&  (0xFFU < N))
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* Copy fixed part of input data */
      if (TRUE == KeyBased)
      {
        /* FixedInputData is (ctr || salt) */
        TS_MemCpy(&Crypto_xVIx_xAIx_FixedInputData[CtrLen], SaltPtr, SaltLength);
      }
      else
      {
        /* FixedInputData is (key || ctr || salt) */
        TS_MemCpy(
                   &Crypto_xVIx_xAIx_FixedInputData[0U],
                   Password->data,
                   Password->length
                 );
        TS_MemCpy(
                   &Crypto_xVIx_xAIx_FixedInputData[CtrLen + Password->length],
                   SaltPtr,
                   SaltLength
                 );
      }
      for (Ctr = 1U; Ctr <= N; Ctr++)
      {
        if (E_OK == RetVal)
        {
          /* !LINKSTO EB_Crypto_CryptAlgos_1009, 1 */
          /* Add loop counter to fixed input data */
          for (j = 0U; j < CtrLen; j++)
          {
            Crypto_xVIx_xAIx_FixedInputData[CtrPos + j] =
              (uint8)((Ctr >> (((CtrLen - 1U) - j) << 3U)) & 0xFFU);
          }
          /* !LINKSTO EB_Crypto_CryptAlgos_1005, 1 */
          switch (*Algorithm)
          {
#if (STD_ON == CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED)
            case CRYPTO_XVIX_XAIX_MACGENERATE_SHA2_256_HMAC_ALGORITHM:
            {
              /* !LINKSTO EB_Crypto_CryptAlgos_1006, 1 */
              RetVal = Crypto_xVIx_xAIx_HelperHMacSha2256SyncSingleCall
                       (
                         Crypto_xVIx_xAIx_FixedInputData,
                         FixedInputDataLength,
                         &OutputPtr[KOutLengthSum],
                         &KOutLength,
                         Password
                       );
            }
            break;
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_HMAC_GEN_ENABLED) */

#if (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED)
           case CRYPTO_XVIX_XAIX_HASH_SHA2_256_ALGORITHM:
           {
             /* !LINKSTO EB_Crypto_CryptAlgos_1006, 1 */
             RetVal = Crypto_xVIx_xAIx_SHA2256HelperSyncSingleCall
                      (
                        Crypto_xVIx_xAIx_FixedInputData,
                        FixedInputDataLength,
                        &OutputPtr[KOutLengthSum],
                        &KOutLength
                      );
           }
            break;
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_SHA2_256_ENABLED) */
             /* CHECK: NOPARSE */
            /*The default case would never be executed because this has already been checked in
              the Crypto_xVIx_xAIx_SetKeyDeriveAlgoValues function.*/
            default:
            {
              /* Caution: this 'default' is required by MISRA-C:2012 */
            }
            break;
             /* CHECK: PARSE */
          }
          KOutLengthSum += KOutLength;
          KOutLength = OutputLength - KOutLengthSum;
        }
        else
        {
          break;
        }
      }
    }
  }

  return RetVal;
}

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_KEY_DERIVATION_ENABLED) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT) */

/*==[end of file]=================================================================================*/

