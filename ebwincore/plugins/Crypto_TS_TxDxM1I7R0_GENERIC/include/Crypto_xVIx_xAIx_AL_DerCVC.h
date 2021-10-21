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

#ifndef CRYPTO_XVIX_XAIX_AL_DERCVC_H
#define CRYPTO_XVIX_XAIX_AL_DERCVC_H

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

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_KEY_FORMAT_NOT_SUPPORTED)
#error CRYPTO_XVIX_XAIX_KEY_FORMAT_NOT_SUPPORTED is already defined
#endif
/** \brief Tag for unsupported key format
 */
#define CRYPTO_XVIX_XAIX_KEY_FORMAT_NOT_SUPPORTED 0x00U

#if (defined CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256)
#error CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256 is already defined
#endif
/** \brief Tag for key format used for Rsa Pss Sha 256
 */
#define CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256 0x01U

#if (defined CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256_LENGTH)
#error CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256_LENGTH is already defined
#endif
/** \brief Length of the Rsa Pss Sha 256 key format identifier
 */
#define CRYPTO_XVIX_XAIX_ID_TA_RSA_PSS_SHA_256_LENGTH 10U

#if (defined CRYPTO_XVIX_XAIX_ID_ECC)
#error CRYPTO_XVIX_XAIX_ID_ECC is already defined
#endif
/** \brief Tag for key format used for Ecc
 */
#define CRYPTO_XVIX_XAIX_ID_ECC 0x02U

#if (defined CRYPTO_XVIX_XAIX_ID_ECC_LENGTH)
#error CRYPTO_XVIX_XAIX_ID_ECC_LENGTH is already defined
#endif
/** \brief Length of the Ecc key format identifier
 */
#define CRYPTO_XVIX_XAIX_ID_ECC_LENGTH 7U

#if (defined CRYPTO_XVIX_XAIX_DATE_LENGTH)
#error CRYPTO_XVIX_XAIX_DATE_LENGTH is already defined
#endif
/** \brief Length of a date
 */
#define CRYPTO_XVIX_XAIX_DATE_LENGTH 6U

/*==[Types]=======================================================================================*/

/** \var Crypto_xVIx_xAIx_CVDateInfoType
 *  \brief Stores the date of expiration and effectiveness
 *         year:  data >> 16
 *         month: data >> 8 & 0xFF
 *         day:   data & 0xFF
 */
 /** \var Crypto_xVIx_xAIx_CVDateInfoType::CurrentDate
 *   \brief The current date when the certificate is being verified.
 */
 /** \var Crypto_xVIx_xAIx_CVDateInfoType::ExpirationDate
 *   \brief The Expiration date of the certificate.
 */
 /** \var Crypto_xVIx_xAIx_CVDateInfoType::EffectiveDate
 *   \brief Starting date from which the certificate is valid.
 */
typedef struct
{
#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  uint8 CurrentDate[CRYPTO_XVIX_XAIX_DATE_LENGTH];
#endif
  uint8 ExpirationDate[CRYPTO_XVIX_XAIX_DATE_LENGTH];
  uint8 EffectiveDate[CRYPTO_XVIX_XAIX_DATE_LENGTH];

} Crypto_xVIx_xAIx_CVDateInfoType;

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Checks the validity of a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CVC certifcate,
 * this function checks whether it is valid and whether its length is exactly
 * the length given as an argument.
 *
 * \param[in]  Encoding A pointer to the start of the DER encoding of
 *                      the certificate to be checked
 * \param[in]  EncodingLength The expected length of the certificate
 *
 * \return Validation result
 *
 * \retval ::E_OK     If the certificate is valid and has the expected
 *                    length
 * \retval ::E_NOT_OK If the certificate is invalid or does not have the
 *                    expected length
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCCheckCertificate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  uint32 EncodingLength
);

/** \brief Returns the signature stored in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate,
 * this function returns a pointer to the start of the signature of the certificate
 * and it also returns the length of the signature.
 *
 * See chapter 4.1.1.3 of RFC 2459.
 *
 * \param[in]  Encoding A pointer to the start of the DER encoding of
 *                      the certificate
 * \param[out] ThingLengthPointer A pointer to a variable where the length of
 *                                the signature will be stored
 *
 * \return A pointer to the start of the contents of the encoding of the
 *         signature
 */
 /* Deviation MISRAC2012-1 */
extern FUNC(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCGetSignatureValue
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Encoding,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ThingLengthPtr
);

/** \brief Returns the signed data in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate,
 * this function returns a pointer to the start of the data of the signature
 * which was signed and it also returns the length of the signed data.
 *
 * See chapter 4.1.2 of RFC 2459.
 *
 * \param[in]  Encoding A pointer to the start of the DER encoding of
 *                      the certificate
 * \param[out] ThingLengthPointer A pointer to a variable where the length of
 *                                the signed data will be stored
 *
 * \return A pointer to the start of signed data
 */
 /* Deviation MISRAC2012-1 */
extern FUNC(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCGetSignedData
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Encoding,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ThingLengthPtr
);

/** \brief Returns the exponent of the public RSA key stored in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate,
 * this function returns a pointer to the start of the exponent of the
 * public RSA key stored in the certificate and it also returns the length of the
 * exponent.
 *
 * See chapter 7.3.1 of RFC 2459.
 *
 * \param[in]  Encoding          A pointer to the start of the DER encoding of
 *                               the certificate
 * \param[out] ExponentLengthPtr A pointer to a variable where the length of
 *                                the exponent will be stored
 *
 * \return A pointer to the start of the contents of the encoding of the
 *         exponent of the public key
 */
 /* Deviation MISRAC2012-1 */
extern FUNC(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetExponent
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Encoding,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ExponentLengthPtr
);

/** \brief Returns the modulus of the public RSA key stored in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate,
 * this function returns a pointer to the start of the modulus of the
 * public RSA key stored in the certificate and it also returns the length of the
 * modulus.
 *
 * See chapter 7.3.1 of RFC 2459.
 *
 * \param[in]  Encoding         A pointer to the start of the DER encoding of
 *                              the certificate
 * \param[out] ModulusLengthPtr A pointer to a variable where the length of
 *                              the modulus will be stored
 *
 * \return A pointer to the start of the contents of the encoding of the
 *         modulus of the public key
 */
 /* Deviation MISRAC2012-1 */
extern FUNC(P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetModulos
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Encoding,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ModulusLengthPtr
);

/** \brief Returns the object identifier of the public key stored in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate,
 * this function returns a pointer to the start of the object identifier of the
 * public key stored in the certificate.
 *
 * \param[in]  Encoding         A pointer to the start of the DER encoding of
 *                              the certificate
 *
 * \return A pointer to the start of the contents of the encoding of the
 *         object identifier of the public key
 */
extern FUNC(uint8, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCRSAPubKeyGetOid
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding
);

/** \brief Returns the expiration and effective date stored in a CV certificate.
 *
 * Given a pointer to the start of the DER encoding of a CV certificate, this
 * function extracts the dates (effective date and expiration date) and returns
 * them in the parameter CVDate.
 *
 * \param[in]  Encoding A pointer to the start of the DER encoding of
 *                      the certificate
 * \param[out] CVDate   A pointer to a structure where the dates will be stored
 *
 * \return void
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCVCGetDate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Encoding,
  P2VAR(Crypto_xVIx_xAIx_CVDateInfoType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CVDate
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* CRYPTO_XVIX_XAIX_AL_DERCVC_H */

/*==[end of file]=================================================================================*/

