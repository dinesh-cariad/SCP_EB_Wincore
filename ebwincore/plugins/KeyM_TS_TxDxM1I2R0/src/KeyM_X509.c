/**
 * \file
 *
 * \brief AUTOSAR KeyM
 *
 * This file contains the implementation of the AUTOSAR
 * module KeyM.
 *
 * \version 1.2.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[deviations]==================================================================*/

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
 * MISRAC2012-2) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped to SEC_CODE memory sections.
 * Objects at block scope require a different mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 */

/*==================[includes]====================================================================*/

#include <KeyM_X509.h>
#include <KeyM_Der.h>
#include <TSMem.h>

/*==================[macros]======================================================================*/

#if (defined KEYM_X509_VERSION_TAG_NUMBER)
#error KEYM_X509_VERSION_TAG_NUMBER is already defined
#endif
/** \brief  The tag for the version. **/
#define KEYM_X509_VERSION_TAG_NUMBER 0U

#if (defined KEYM_X509_ISSUERUNIQUEID_TAG_NUMBER)
#error KEYM_X509_ISSUERUNIQUEID_TAG_NUMBER is already defined
#endif
/** \brief  The tag for the issuerUniqueId. **/
#define KEYM_X509_ISSUERUNIQUEID_TAG_NUMBER 1U

#if (defined KEYM_X509_SUBJECTUNIQUEID_TAG_NUMBER)
#error KEYM_X509_SUBJECTUNIQUEID_TAG_NUMBER is already defined
#endif
/** \brief  The tag for the subjectUniqueId. **/
#define KEYM_X509_SUBJECTUNIQUEID_TAG_NUMBER 2U

#if (defined KEYM_X509_EXTENSIONS_TAG_NUMBER)
#error KEYM_X509_EXTENSIONS_TAG_NUMBER is already defined
#endif
/** \brief  The tag for the extension. **/
#define KEYM_X509_EXTENSIONS_TAG_NUMBER 3U

#if (defined KEYM_X509_EXTENSIONS_NUM_MAX)
#error KEYM_X509_EXTENSIONS_NUM_MAX is already defined
#endif
/** \brief  The maximum number of supported extensions. **/
#define KEYM_X509_EXTENSIONS_NUM_MAX 32U

#if (defined KEYM_X509_EXTENSIONS_OID_LENGTH)
#error KEYM_X509_EXTENSIONS_OID_LENGTH is already defined
#endif
/** \brief  The common length of the extension oids which are specified in rfc5280. **/
#define KEYM_X509_EXTENSIONS_OID_LENGTH 3U

#if (defined KEYM_X509_KNOWN_EXTENSIONS_COUNT)
#error KEYM_X509_KNOWN_EXTENSIONS_COUNT is already defined
#endif
/** \brief  The number of known extensions. **/
#define KEYM_X509_KNOWN_EXTENSIONS_COUNT 5U

#if (defined KEYM_X509_EXTENSION_INDEX_AUTH_KEY_ID)
#error KEYM_X509_EXTENSION_INDEX_AUTH_KEY_ID is already defined
#endif
/** \brief  The index of the auth key id extension in the known extension list. **/
#define KEYM_X509_EXTENSION_INDEX_AUTH_KEY_ID 0U

#if (defined KEYM_X509_EXTENSION_INDEX_BASIC_CONSTRAINTS)
#error KEYM_X509_EXTENSION_INDEX_BASIC_CONSTRAINTS is already defined
#endif
/** \brief  The index of the auth key id extension in the known extension list. **/
#define KEYM_X509_EXTENSION_INDEX_BASIC_CONSTRAINTS 1U

#if (defined KEYM_X509_EXTENSION_INDEX_EXT_KEY_USAGE)
#error KEYM_X509_EXTENSION_INDEX_EXT_KEY_USAGE is already defined
#endif
/** \brief  The index of the auth key id extension in the known extension list. **/
#define KEYM_X509_EXTENSION_INDEX_EXT_KEY_USAGE 2U

#if (defined KEYM_X509_EXTENSION_INDEX_KEY_USAGE)
#error KEYM_X509_EXTENSION_INDEX_KEY_USAGE is already defined
#endif
/** \brief  The index of the auth key id extension in the known extension list. **/
#define KEYM_X509_EXTENSION_INDEX_KEY_USAGE 3U

#if (defined KEYM_X509_EXTENSION_INDEX_SUBJECT_KEY_ID)
#error KEYM_X509_EXTENSION_INDEX_SUBJECT_KEY_ID is already defined
#endif
/** \brief  The index of the auth key id extension in the known extension list. **/
#define KEYM_X509_EXTENSION_INDEX_SUBJECT_KEY_ID 4U

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/** \brief  Function to parse the version of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Version
(
  P2VAR(uint8,                AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                           encodingLength,
  P2VAR(KeyM_X509INTEGERType, AUTOMATIC, KEYM_APPL_DATA) version
);

/** \brief  Function to parse the serialNumber of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SerialNumber
(
  P2VAR(uint8,                AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                           encodingLength,
  P2VAR(KeyM_X509INTEGERType, AUTOMATIC, KEYM_APPL_DATA) serialNumber
);

/** \brief  Function to parse the algorithmIdentifier of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509AlgorithmIdentifier
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) algorithmIdentifier
);

/** \brief  Function to parse the signature of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Signature
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) signature
);

/** \brief  Function to parse the attributeType of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509AttributeTypeAndValue
(
  P2VAR(uint8,                              AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                         encodingLength,
  P2VAR(KeyM_X509AttributeTypeAndValueType, AUTOMATIC, KEYM_APPL_DATA) attribute,
  P2VAR(uint32,                             AUTOMATIC, KEYM_APPL_DATA) structureLength
);

/** \brief  Function to parse the name of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Name
(
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
               uint8                                                elementStructure,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
);

/** \brief  Function to parse the issuer of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Issuer
(
               P2VAR(uint8,              AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
);

/** \brief  Function to parse the validity of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Validity
(
  P2VAR(uint8,                 AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                            encodingLength,
  P2VAR(KeyM_X509ValidityType, AUTOMATIC, KEYM_APPL_DATA) validity
);

/** \brief  Function to parse the subject of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Subject
(
               P2VAR(uint8,              AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
);

/** \brief  Function to parse the subjectPublicKeyInfo of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SubjectPublicKeyInfo
(
  P2VAR(uint8,                             AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                        encodingLength,
  P2VAR(KeyM_X509SubjectPublicKeyInfoType, AUTOMATIC, KEYM_APPL_DATA) subjectPublicKeyInfo
);

/** \brief  Function to parse the issuerUniqueId of a x509 certificate. **/
static FUNC(void, KEYM_CODE) KeyM_GetX509IssuerUniqueId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) issuerUniqueId
);

/** \brief  Function to parse the subjectUniqueId of a x509 certificate. **/
static FUNC(void, KEYM_CODE) KeyM_GetX509SubjectUniqueId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) subjectUniqueId
);

/** \brief  Function to set a certificate element from a parsed value. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_SetCertificateElement
(
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
               uint8                                                elementStructure,
  CONSTP2CONST(uint8,                    AUTOMATIC, KEYM_CONST)     oid,
               uint32                                               oidLength,
               boolean                                              isStructure,
               uint8                                                tag,
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) value,
               uint32                                               length
);

/** \brief  Function to parse the AuthorityKeyIdentifier extension of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsAuthKeyId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Function to parse the BasicConstraints extension of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsBasicConstr
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Function to parse the ExtKeyUsage extension of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsExtKeyUsage
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Function to parse the KeyUsage extension of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsKeyUsage
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Function to parse the SubjectKeyIdentifier extension of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsSubjKeyId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509UnknownExtensions
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) oid,
  uint32                                                                    oidLength
);

/** \brief  Function to parse the extensions of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Extensions
(
  /* Deviation MISRAC2012-1 */
         P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
         P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,                AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) extensions
);

/** \brief  Function to parse the tbsCertificate of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509TbsCertificate
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509FullTBSCertificateType,         AUTOMATIC, KEYM_APPL_DATA) tbsCertificate,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Function to parse the signatureAlgorithm of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SignatureAlgorithm
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) signatureAlgorithm
);

/** \brief  Function to parse the signatureValue of a x509 certificate. **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SignatureValue
(
  P2VAR(uint8,                  AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                             encodingLength,
  P2VAR(KeyM_X509BITSTRINGType, AUTOMATIC, KEYM_APPL_DATA) signatureValue
);

/** \brief  Function to copy the parsed certificate into
 **         the certificate where it will be stored.
 **/
static FUNC(void, KEYM_CODE) KeyM_CopyCertificate
(
               KeyM_X509FullCertificateType                     srcCertificate,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST) certificateCnfg
);

/** \brief  Function to check for duplicate extensions in a x509 certificate
 **         by comparing the oid of the current extension to the list of
 **         previous extension oids.
 **/
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_CheckDuplicateExtensions
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) extOidList,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) extOidListLength,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) oid,
        uint32                                                              oidLength
);

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal constants]==========================================================*/

#define KEYM_START_SEC_CONST_8
#include <KeyM_MemMap.h>

/** \brief The oid of the AuthorityKeyIdentifier extension. **/
/* Deviation MISRAC2012-2 */
static CONST(uint8, AUTOMATIC) KeyM_ExtensionsOidAuthKeyId[KEYM_X509_EXTENSIONS_OID_LENGTH] =
{
  0x55U, 0x1DU, 0x23U
};

/** \brief The oid of the BasicConstraints extension. **/
/* Deviation MISRAC2012-2 */
static CONST(uint8, AUTOMATIC) KeyM_ExtensionsOidBasicConstraints[KEYM_X509_EXTENSIONS_OID_LENGTH] =
{
  0x55U, 0x1DU, 0x13U
};

/** \brief The oid of the ExtKeyUsage extension. **/
/* Deviation MISRAC2012-2 */
static CONST(uint8, AUTOMATIC) KeyM_ExtensionsOidExtKeyUsage[KEYM_X509_EXTENSIONS_OID_LENGTH] =
{
  0x55U, 0x1DU, 0x25U
};

/** \brief The oid of the KeyUsage extension. **/
/* Deviation MISRAC2012-2 */
static CONST(uint8, AUTOMATIC) KeyM_ExtensionsOidKeyUsage[KEYM_X509_EXTENSIONS_OID_LENGTH] =
{
  0x55U, 0x1DU, 0x0FU
};

/** \brief The oid of the SubjectKeyIdentifier extension. **/
/* Deviation MISRAC2012-2 */
static CONST(uint8, AUTOMATIC) KeyM_ExtensionsOidSubjectKeyId[KEYM_X509_EXTENSIONS_OID_LENGTH] =
{
  0x55U, 0x1DU, 0x0EU
};

#define KEYM_STOP_SEC_CONST_8
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

/** \brief The list of known extension. **/
/* Deviation MISRAC2012-2 */
static CONST(KeyM_ExtensionInfoType, AUTOMATIC) KeyM_KnownExtensionsList[KEYM_X509_KNOWN_EXTENSIONS_COUNT] =
{
  {
    KeyM_ExtensionsOidAuthKeyId,
    KEYM_X509_EXTENSIONS_OID_LENGTH,
    KeyM_GetX509ExtensionsAuthKeyId,
    FALSE,
    /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_AuthKeyId_003,1 */
    TRUE /* needsNonCritical */
  },
  {
    KeyM_ExtensionsOidBasicConstraints,
    KEYM_X509_EXTENSIONS_OID_LENGTH,
    KeyM_GetX509ExtensionsBasicConstr,
    /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_BasicConstr_003,1 */
    TRUE, /* needsCritical */
    FALSE
  },
  {
    KeyM_ExtensionsOidExtKeyUsage,
    KEYM_X509_EXTENSIONS_OID_LENGTH,
    KeyM_GetX509ExtensionsExtKeyUsage,
    FALSE,
    FALSE
  },
  {
    KeyM_ExtensionsOidKeyUsage,
    KEYM_X509_EXTENSIONS_OID_LENGTH,
    KeyM_GetX509ExtensionsKeyUsage,
    FALSE,
    FALSE
  },
  {
    KeyM_ExtensionsOidSubjectKeyId,
    KEYM_X509_EXTENSIONS_OID_LENGTH,
    KeyM_GetX509ExtensionsSubjKeyId,
    FALSE,
    FALSE
  }
};

#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/* --- KeyM_GetX509Certificate ------------------------------------------------------------------ */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Certificate
(
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
        uint32                            i;
        uint8                             j;
        KeyM_X509FullCertificateType      tempCertificate =
        {
          {
            0U, /* length */
            NULL_PTR, /* data */
            {0U, NULL_PTR}, /* version */
            {0U, NULL_PTR}, /* serialNumber */
            {0U, NULL_PTR, {0U, NULL_PTR}}, /* signature */
            {
              {0U, NULL_PTR},
              {0U, NULL_PTR}
            }, /* validity */
            {
              {0U, NULL_PTR, {0U, NULL_PTR}}, {0U, NULL_PTR}
            }, /* subjectPublicKeyInfo */
            {0U, NULL_PTR}, /* issuerUniqueID */
            {0U, NULL_PTR}, /* subjectUniqueID */
            {0U, NULL_PTR} /* extensions */
          }, /* tbsCertificate */
          {0U, NULL_PTR, {0U, NULL_PTR}}, /* signatureAlgorithm */
          {0U, NULL_PTR} /* signatureValue */
        };

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_002,2 */
  retVal = KeyM_GetDERSequence(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_003,2 */
    retVal = KeyM_GetX509TbsCertificate(&tempBuffer,
                                        &tempBufferLength,
                                        &tempCertificate.tbsCertificate,
                                        certificateCnfg
                                       );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_004,2 */
    retVal = KeyM_GetX509SignatureAlgorithm(tempBuffer,
                                            tempBufferLength,
                                            &tempCertificate.signatureAlgorithm
                                           );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tempCertificate.signatureAlgorithm.algorithm.oid - tempBuffer)
                        + tempCertificate.signatureAlgorithm.algorithm.length + 2U;
    tempBuffer        = &tempCertificate.signatureAlgorithm.algorithm.oid[tempCertificate.signatureAlgorithm.algorithm.length + 2U];

    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SignatureAlgorithm_002,2 */
    if (   tempCertificate.signatureAlgorithm.algorithm.length
        != tempCertificate.tbsCertificate.signature.algorithm.length
       )
    {
      retVal = KEYM_PARSERESULT_INVALID_CONTENT;
    }
    else
    {
      if (E_OK ==  TS_MemCmp(tempCertificate.signatureAlgorithm.algorithm.oid,
                             tempCertificate.tbsCertificate.signature.algorithm.oid,
                             tempCertificate.tbsCertificate.signature.algorithm.length
                            )
         )
      {
        retVal = KEYM_PARSERESULT_OK;
      }
      else
      {
        retVal = KEYM_PARSERESULT_INVALID_CONTENT;
      }
    }
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_005,2 */
    retVal = KeyM_GetX509SignatureValue(tempBuffer,
                                        tempBufferLength,
                                        &tempCertificate.signatureValue
                                       );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tempCertificate.signatureValue.bitString - tempBuffer)
                        + tempCertificate.signatureValue.length;
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_006,2 */
    if (0U != tempBufferLength)
    {
      retVal = KEYM_PARSERESULT_INVALID_FORMAT;
    }
    else
    {
      KeyM_CopyCertificate(tempCertificate, certificateCnfg);

      /* Check certificate element lengths */
      /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_007,1 */
      for (i = 0U; ((i < certificateCnfg->certElementsNumber) && (KEYM_PARSERESULT_OK == retVal)); i++)
      {
        for (j = 0U; j < certificateCnfg->certElements[i].cnfg->maxIterations; j++)
        {
          if (certificateCnfg->certElements[i].data[j].dataLength > certificateCnfg->certElements[i].cnfg->maxLength)
          {
            retVal = KEYM_PARSERESULT_INVALID_CONTENT;
            break;
          }
        }
      }

      /* Check mandatory certificate elements */
      /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_008,1 */
      /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_009,1 */
      /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Certificate_010,1 */
      for (i = 0U; i < certificateCnfg->certElementsNumber; i++)
      {
        if (   (TRUE == certificateCnfg->certElements[i].cnfg->isMandatory)
            && (NULL_PTR == certificateCnfg->certElements[i].data[0U].dataPtr)
           )
        {
          retVal = KEYM_PARSERESULT_INVALID_CONTENT;
          break;
        }
      }
    }
  }

  return retVal;
}

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/* --- KeyM_GetX509Version ---------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Version_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Version
(
  P2VAR(uint8,                AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                           encodingLength,
  P2VAR(KeyM_X509INTEGERType, AUTOMATIC, KEYM_APPL_DATA) version
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;

  retVal = KeyM_GetDERExplicit(encoding,
                               encodingLength,
                               KEYM_X509_VERSION_TAG_NUMBER,
                               &tempBuffer,
                               &tempBufferLength
                              );
  if (KEYM_PARSERESULT_OK == retVal)
  {
    retVal = KeyM_GetDERInteger(tempBuffer, tempBufferLength, &version->integer, &version->length);
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Version_002,2 */
    if (2U != version->integer[0U])
    {
      retVal = KEYM_PARSERESULT_INVALID_TYPE;
    }
  }
  return retVal;
}

/* --- KeyM_GetX509SerialNumber ----------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SerialNumber_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SerialNumber
(
  P2VAR(uint8,                AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                           encodingLength,
  P2VAR(KeyM_X509INTEGERType, AUTOMATIC, KEYM_APPL_DATA) serialNumber
)
{
  KeyM_ParseResultType retVal;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SerialNumber_002,1 */
  retVal = KeyM_GetDERInteger(encoding, encodingLength, &serialNumber->integer, &serialNumber->length);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SerialNumber_003,2 */
    if (20U < serialNumber->length)
    {
      retVal = KEYM_PARSERESULT_INVALID_FORMAT;
    }
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SerialNumber_004,2 */
    if (0x80U == (serialNumber->integer[0U] & 0x80U))
    {
      retVal = KEYM_PARSERESULT_INVALID_CONTENT;
    }
  }
  return retVal;
}

/* --- KeyM_GetX509AlgorithmIdentifier ---------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509AlgorithmIdentifier_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509AlgorithmIdentifier
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) algorithmIdentifier
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509AlgorithmIdentifier_002,2 */
  retVal = KeyM_GetDERSequence(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    algorithmIdentifier->length              = tempBufferLength;
    algorithmIdentifier->algorithmIdentifier = tempBuffer;
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509AlgorithmIdentifier_003,2 */
    retVal = KeyM_GetDEROID(tempBuffer,
                            tempBufferLength,
                            &algorithmIdentifier->algorithm.oid,
                            &algorithmIdentifier->algorithm.length
                           );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* AlgorithmIdentifier is only supported without parameters. */
    tempBufferLength -=   (algorithmIdentifier->algorithm.oid - tempBuffer)
                        + algorithmIdentifier->algorithm.length;
    tempBuffer        = &algorithmIdentifier->algorithm.oid[algorithmIdentifier->algorithm.length];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509AlgorithmIdentifier_004,2 */
    retVal = KeyM_CheckDERNULL(tempBuffer, tempBufferLength);
  }
  return retVal;
}

/* --- KeyM_GetX509Signature -------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Signature_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Signature
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) signature
)
{
  return KeyM_GetX509AlgorithmIdentifier(encoding, encodingLength, signature);
}

/* --- KeyM_GetX509AttributeTypeAndValue -------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMAttributeTypeAndValue_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509AttributeTypeAndValue
(
  P2VAR(uint8,                              AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                         encodingLength,
  P2VAR(KeyM_X509AttributeTypeAndValueType, AUTOMATIC, KEYM_APPL_DATA) attribute,
  P2VAR(uint32,                             AUTOMATIC, KEYM_APPL_DATA) structureLength
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMAttributeTypeAndValue_002,2 */
  retVal = KeyM_GetDERSet(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    *structureLength = (tempBuffer - encoding) + tempBufferLength;
    /* !LINKSTO EB_KeyM_ParserX509_KeyMAttributeTypeAndValue_003,2 */
    retVal = KeyM_GetDERSequence(tempBuffer, tempBufferLength, &tempBuffer, &tempBufferLength);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMAttributeTypeAndValue_004,2 */
    retVal = KeyM_GetDEROID(tempBuffer, tempBufferLength, &attribute->oid, &attribute->oidLength);
  }

  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -= (attribute->oid - tempBuffer) + attribute->oidLength;
    tempBuffer        = &attribute->oid[attribute->oidLength];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMAttributeTypeAndValue_006,2 */
    retVal = KeyM_GetDERString(tempBuffer,
                               tempBufferLength,
                               &attribute->value,
                               &attribute->length
                              );
    attribute->tag = tempBuffer[0U];
  }
  return retVal;
}

/* --- KeyM_GetX509Name ------------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Name_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Name
(
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
               uint8                                                elementStructure,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
)
{
        KeyM_ParseResultType               retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA)  tempBuffer;
        uint32                             tempBufferLength = 0U;
        KeyM_X509AttributeTypeAndValueType attr;
        uint32                             currentLength = 0U;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Name_010,1 */
  retVal = KeyM_GetDERSequence(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  *structureLength = (tempBuffer - encoding) + tempBufferLength;
  if (KEYM_PARSERESULT_OK == retVal)
  {
    if (KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME == elementStructure)
    {
      certificateCnfg->certElementDataIssuer->dataPtr = tempBuffer;
      certificateCnfg->certElementDataIssuer->dataLength = tempBufferLength;
    }
    if (KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME == elementStructure)
    {
      certificateCnfg->certElementDataSubject->dataPtr = tempBuffer;
      certificateCnfg->certElementDataSubject->dataLength = tempBufferLength;
    }

    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Name_003,1 */
    do
    {
      retVal = KeyM_GetX509AttributeTypeAndValue(tempBuffer, tempBufferLength, &attr, &currentLength);
      if (KEYM_PARSERESULT_OK == retVal)
      {
        tempBufferLength -= currentLength;
        tempBuffer        = &tempBuffer[currentLength];
        /* Set certificate elements */
        retVal = KeyM_SetCertificateElement(certificateCnfg,
                                            elementStructure,
                                            attr.oid,
                                            attr.oidLength,
                                            FALSE,
                                            attr.tag,
                                            attr.value,
                                            attr.length
                                           );
      }
    }
    while ((tempBufferLength > 0U) && (KEYM_PARSERESULT_OK == retVal));
  }
  return retVal;
}

/* --- KeyM_GetX509Issuer ----------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Issuer_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Issuer
(
               P2VAR(uint8,              AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
)
{
  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Issuer_002,3 */
  return KeyM_GetX509Name(encoding,
                          encodingLength,
                          KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME,
                          certificateCnfg,
                          structureLength
                         );
}

/* --- KeyM_GetX509Validity --------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Validity_001,1 */
static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Validity
(
  P2VAR(uint8,                 AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                            encodingLength,
  P2VAR(KeyM_X509ValidityType, AUTOMATIC, KEYM_APPL_DATA) validity
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Validity_002,2 */
  retVal = KeyM_GetDERSequence(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    retVal = KeyM_GetDERTime(tempBuffer,
                             tempBufferLength,
                             &validity->notBefore.time,
                             &validity->notBefore.length
                            );
  }
  if(KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -= (validity->notBefore.time - tempBuffer)
                        + validity->notBefore.length;
    tempBuffer        = &validity->notBefore.time[validity->notBefore.length];
    retVal = KeyM_GetDERTime(tempBuffer,
                             tempBufferLength,
                             &validity->notAfter.time,
                             &validity->notAfter.length
                            );
  }
  return retVal;
}

/* --- KeyM_GetX509Subject ---------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Subject_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Subject
(
               P2VAR(uint8,              AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(uint32,                   AUTOMATIC, KEYM_APPL_DATA) structureLength
)
{
  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509Subject_002,2 */
  return KeyM_GetX509Name(encoding,
                          encodingLength,
                          KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME,
                          certificateCnfg,
                          structureLength
                         );
}

/* --- KeyM_GetX509SubjectPublicKeyInfo --------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SubjectPublicKeyInfo_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SubjectPublicKeyInfo
(
  P2VAR(uint8,                             AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                        encodingLength,
  P2VAR(KeyM_X509SubjectPublicKeyInfoType, AUTOMATIC, KEYM_APPL_DATA) subjectPublicKeyInfo
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SubjectPublicKeyInfo_002,1 */
  retVal = KeyM_GetDERSequence(encoding, encodingLength, &tempBuffer, &tempBufferLength);
  if(KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SubjectPublicKeyInfo_003,2 */
    retVal = KeyM_GetX509AlgorithmIdentifier(tempBuffer,
                                             tempBufferLength,
                                             &subjectPublicKeyInfo->algorithm
                                            );
  }
  if(KEYM_PARSERESULT_OK == retVal)
  {
    /* AlgorithmIdentifier does not support parameters */
    tempBufferLength -=   (subjectPublicKeyInfo->algorithm.algorithm.oid - tempBuffer)
                          + subjectPublicKeyInfo->algorithm.algorithm.length + 2U;
    tempBuffer        = &subjectPublicKeyInfo->algorithm.algorithm.oid[subjectPublicKeyInfo->algorithm.algorithm.length + 2U];

    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SubjectPublicKeyInfo_004,2 */
    retVal = KeyM_GetDERBitString(tempBuffer,
                                  tempBufferLength,
                                  &subjectPublicKeyInfo->subjectPublicKey.bitString,
                                  &subjectPublicKeyInfo->subjectPublicKey.length
                                 );
  }
  return retVal;
}

/* --- KeyM_GetX509IssuerUniqueId --------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMIssuerUniqueId_001,1 */

static FUNC(void, KEYM_CODE) KeyM_GetX509IssuerUniqueId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) issuerUniqueId
)
{
  uint32 length;
  uint32 position;

  /* we need atleast tag, length, value */
  /* !LINKSTO EB_KeyM_ParserX509_KeyMIssuerUniqueId_003,1 */
  if (3U <= *encodingLength)
  {
    if ((KEYM_ASN1_DER_TAG_EXPLICIT | KEYM_X509_ISSUERUNIQUEID_TAG_NUMBER) == (*encoding)[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&(*encoding)[1U],
                                                       (*encodingLength - 1U),
                                                      &length,
                                                      &position
                                                     )
         )
      {
        /* !LINKSTO EB_KeyM_ParserX509_KeyMIssuerUniqueId_004,1 */
        if (*encodingLength >= (1U + position + length))
        {
          issuerUniqueId->length    = length;
          issuerUniqueId->bitString = &(*encoding)[(1U + position)];
          *encodingLength -= (1U + position + length);
          *encoding        = &(*encoding)[(1U + position + length)];
        }
      }
    }
  }
}

/* --- KeyM_GetX509SubjectUniqueId -------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMSubjectUniqueId_001,1 */

static FUNC(void, KEYM_CODE) KeyM_GetX509SubjectUniqueId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) subjectUniqueId
)
{
  uint32 length;
  uint32 position;

  /* we need atleast tag, length, value */
  /* !LINKSTO EB_KeyM_ParserX509_KeyMSubjectUniqueId_003,1 */
  if (3U <= *encodingLength)
  {
    if ((KEYM_ASN1_DER_TAG_EXPLICIT | KEYM_X509_SUBJECTUNIQUEID_TAG_NUMBER) == (*encoding)[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&(*encoding)[1U],
                                                      (*encodingLength - 1U),
                                                      &length,
                                                      &position
                                                     )
         )
      {
        /* !LINKSTO EB_KeyM_ParserX509_KeyMSubjectUniqueId_004,1 */
        if (*encodingLength >= (1U + position + length))
        {
          subjectUniqueId->length    = length;
          subjectUniqueId->bitString = &(*encoding)[(1U + position)];
          *encodingLength -= (1U + position + length);
          *encoding        = &(*encoding)[(1U + position + length)];
        }
      }
    }
  }
}

/* --- KeyM_SetCertificateElement --------------------------------------------------------------- */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_SetCertificateElement
(
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg,
               uint8                                                elementStructure,
  CONSTP2CONST(uint8,                    AUTOMATIC, KEYM_CONST)     oid,
               uint32                                               oidLength,
               boolean                                              isStructure,
               uint8                                                tag,
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) value,
               uint32                                               length
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_OK;
  uint16               i;
  uint8                j;
  boolean              foundElement = FALSE;

  /* Check certificate elements */
  for (i = 0U; (i < certificateCnfg->certElementsNumber); i++)
  {
    if (certificateCnfg->certElements[i].cnfg->structure == elementStructure)
    {
      if (certificateCnfg->certElements[i].cnfg->objectIdLength != 0U)
      {
        if (certificateCnfg->certElements[i].cnfg->objectIdLength == oidLength)
        {
          if (E_OK != TS_MemCmp(certificateCnfg->certElements[i].cnfg->objectId, oid, oidLength))
          {
            continue;
          }
        }
        else
        {
          continue;
        }
      }
      else
      {
        /* Skip extension cert elements without OID, this is handled in KeyM_CopyCertificate. */
        if (KEYM_STRUCTURE_CERTIFICATE_EXTENSION == elementStructure)
        {
          continue;
        }
      }

      /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_009,1 */
      if (   (isStructure    && (!certificateCnfg->certElements[i].cnfg->hasIterations))
          || ((!isStructure) && certificateCnfg->certElements[i].cnfg->hasIterations   )
          || ((!isStructure) && certificateCnfg->certElements[i].cnfg->hasObjectType)
         )
      {
        /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_008,1 */
        /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_010,2 */
        if (   certificateCnfg->certElements[i].cnfg->hasObjectType
            && (tag != certificateCnfg->certElements[i].cnfg->objectType)
           )
        {
          continue;
        }

        j = 0U;
        do
        {
          if (NULL_PTR == certificateCnfg->certElements[i].data[j].dataPtr)
          {
            foundElement = TRUE;
            certificateCnfg->certElements[i].data[j].dataLength = length;
            certificateCnfg->certElements[i].data[j].dataPtr    = value;
          }
          else
          {
            if (   (certificateCnfg->certElements[i].cnfg->hasObjectType)
                && (!certificateCnfg->certElements[i].cnfg->hasIterations)
               )
            {
              /* only get first element and ignore others */
              foundElement = TRUE;
            }
          }
          j++;
        }
        while ((j < certificateCnfg->certElements[i].cnfg->maxIterations) && (!foundElement));
        if (!foundElement)
        {
          /* Iterations exceeded */
          /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_011,1 */
          retVal = KEYM_PARSERESULT_INVALID_FORMAT;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetX509ExtensionsAuthKeyId ---------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_AuthKeyId_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsAuthKeyId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
        boolean                           foundKeyIdentifier = FALSE;
        boolean                           foundAuthorityCertIssuer = FALSE;
        boolean                           foundAuthorityCertSerialNumber = FALSE;
        uint8                             tag;

  retVal = KeyM_GetDERSequence(*encoding, *encodingLength, encoding, encodingLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_AuthKeyId_004,1 */
    do
    {
      retVal = KeyM_GetDERTagLengthValue(*encoding, *encodingLength, &tag, &tempBuffer, &tempBufferLength);
      if (KEYM_PARSERESULT_OK == retVal)
      {
        /* keyIdentifier, optional */
        if ((!foundKeyIdentifier) && (tag == 0x80U))
        {
          foundKeyIdentifier = TRUE;
        }
        /* authorityCertIssuer, optional, needs also authorityCertSerialNumber */
        else
        if ((!foundAuthorityCertIssuer) && (tag == 0xA1U))
        {
          foundAuthorityCertIssuer = TRUE;
        }
        /* authorityCertSerialNumber, optional, needs also authorityCertIssuer */
        else
        if ((!foundAuthorityCertSerialNumber) && (tag == 0x82U))
        {
          foundAuthorityCertSerialNumber = TRUE;
        }
        else
        {
          retVal = KEYM_PARSERESULT_INVALID_FORMAT;
        }

        if (KEYM_PARSERESULT_OK == retVal)
        {
          retVal = KeyM_SetCertificateElement(certificateCnfg,
                                              KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                              KeyM_ExtensionsOidAuthKeyId,
                                              KEYM_X509_EXTENSIONS_OID_LENGTH,
                                              FALSE,
                                              tag,
                                              tempBuffer,
                                              tempBufferLength
                                             );
        }

        *encodingLength -= (tempBuffer - *encoding) + tempBufferLength;
        *encoding        = &tempBuffer[tempBufferLength];
      }
    }
    while ((KEYM_PARSERESULT_OK == retVal) && (0U < *encodingLength));

    /* AuthorityCertIssuer and authorityCertSerialNumber MUST both be present or both be absent */
    if (   (foundAuthorityCertIssuer    && (!foundAuthorityCertSerialNumber))
        || ((!foundAuthorityCertIssuer) && foundAuthorityCertSerialNumber   )
       )
    {
      retVal = KEYM_PARSERESULT_INVALID_FORMAT;
    }
  }

  return retVal;
}

/* --- KeyM_GetX509ExtensionsBasicConstr -------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_BasicConstr_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsBasicConstr
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
        uint8                             tlvTag;
        uint32                            tlvLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tlvValue;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) integer = NULL_PTR;
        uint32                            integerLength = 0U;

  retVal = KeyM_GetDERSequence(*encoding, *encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* Parse cA */
    retVal = KeyM_GetDERTagLengthValue(tempBuffer, tempBufferLength, &tlvTag, &tlvValue, &tlvLength);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    retVal = KeyM_SetCertificateElement(certificateCnfg,
                                        KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                        KeyM_ExtensionsOidBasicConstraints,
                                        KEYM_X509_EXTENSIONS_OID_LENGTH,
                                        FALSE,
                                        tlvTag,
                                        tlvValue,
                                        tlvLength
                                       );
    tempBufferLength -= (tlvValue - tempBuffer) + tlvLength;
    tempBuffer        = &tlvValue[tlvLength];

    if (tempBufferLength > 0U)
    {
      /* Parse pathLenConstraint */
      retVal = KeyM_GetDERInteger(tempBuffer, tempBufferLength, &integer, &integerLength);
      if (KEYM_PARSERESULT_OK == retVal)
      {
        tlvTag = tempBuffer[0U];
        tempBufferLength -= (integer - tempBuffer) + integerLength;
        tempBuffer        = &integer[integerLength];
        retVal = KeyM_SetCertificateElement(certificateCnfg,
                                            KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                            KeyM_ExtensionsOidBasicConstraints,
                                            KEYM_X509_EXTENSIONS_OID_LENGTH,
                                            FALSE,
                                            tlvTag,
                                            integer,
                                            integerLength
                                           );
        if (tempBufferLength > 0U)
        {
          /* Additional elements found */
          retVal = KEYM_PARSERESULT_INVALID_FORMAT;
        }
      }
    }
    *encoding = &((*encoding)[*encodingLength]);
  }

  return retVal;
}

/* --- KeyM_GetX509ExtensionsExtKeyUsage -------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_ExtKeyUsage_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsExtKeyUsage
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) oid;
        uint32                            oidLength;

  retVal = KeyM_GetDERSequence(*encoding, *encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    do
    {
      retVal = KeyM_GetDEROID(tempBuffer, tempBufferLength, &oid, &oidLength);
      if (KEYM_PARSERESULT_OK == retVal)
      {
        retVal = KeyM_SetCertificateElement(certificateCnfg,
                                            KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                            KeyM_ExtensionsOidExtKeyUsage,
                                            KEYM_X509_EXTENSIONS_OID_LENGTH,
                                            FALSE,
                                            tempBuffer[0U],
                                            oid,
                                            oidLength
                                           );
        tempBufferLength -= (oid - tempBuffer) + oidLength;
        tempBuffer        = &oid[oidLength];
      }
    }
    while ((KEYM_PARSERESULT_OK == retVal) && (0U < tempBufferLength));
    *encoding = &((*encoding)[*encodingLength]);
  }

  return retVal;
}

/* --- KeyM_GetX509ExtensionsKeyUsage ----------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_KeyUsage_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsKeyUsage
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal          = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) bitString       = NULL_PTR;
        uint32                            bitStringLength = 0U;
        uint8                             tag;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_KeyUsage_003,1 */
  retVal = KeyM_GetDERTagLengthValue(*encoding, *encodingLength, &tag, &bitString, &bitStringLength);
  if ((KEYM_PARSERESULT_OK == retVal) && (KEYM_ASN1_DER_TAG_BIT_STRING != tag))
  {
    retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    retVal = KeyM_SetCertificateElement(certificateCnfg,
                                        KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                        KeyM_ExtensionsOidKeyUsage,
                                        KEYM_X509_EXTENSIONS_OID_LENGTH,
                                        FALSE,
                                        tag,
                                        bitString,
                                        bitStringLength
                                       );
    *encoding = &((*encoding)[*encodingLength]);
  }
  return retVal;
}

/* --- KeyM_GetX509ExtensionsSubjKeyId ---------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_SubjKeyId_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509ExtensionsSubjKeyId
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_SubjKeyId_003,1 */
  retVal = KeyM_GetDEROctetString(*encoding, *encodingLength, encoding, encodingLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    retVal = KeyM_SetCertificateElement(certificateCnfg,
                                        KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                        KeyM_ExtensionsOidSubjectKeyId,
                                        KEYM_X509_EXTENSIONS_OID_LENGTH,
                                        FALSE,
                                        (*encoding)[0U],
                                        *encoding,
                                        *encodingLength
                                       );
    *encoding = &((*encoding)[*encodingLength]);
  }

  return retVal;
}

/* --- KeyM_GetX509UnknownExtensions ------------------------------------------------------------ */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_006,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509UnknownExtensions
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) oid,
  uint32                                                                    oidLength
)
{
        KeyM_ParseResultType              retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
        uint8                             tlvTag;
        uint32                            tlvLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tlvValue;
  if (KEYM_PARSERESULT_OK == KeyM_GetDERSequence(*encoding, *encodingLength, &tempBuffer, &tempBufferLength))
  {
    do
    {
      retVal = KeyM_GetDERTagLengthValue(tempBuffer, tempBufferLength, &tlvTag, &tlvValue, &tlvLength);
      if (KEYM_PARSERESULT_OK == retVal)
      {
        retVal = KeyM_SetCertificateElement(certificateCnfg,
                                            KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                            oid,
                                            oidLength,
                                            FALSE,
                                            tlvTag,
                                            tlvValue,
                                            tlvLength
                                           );
        tempBufferLength -= (tlvValue - tempBuffer) + tlvLength;
        tempBuffer        = &tlvValue[tlvLength];
      }
    }
    while ((tempBufferLength > 0U) && (retVal == KEYM_PARSERESULT_OK));
  }
  else
  {
    retVal = KeyM_GetDERTagLengthValue(*encoding, *encodingLength, &tlvTag, &tlvValue, &tlvLength);
    if (KEYM_PARSERESULT_OK == retVal)
    {
      retVal = KeyM_SetCertificateElement(certificateCnfg,
                                          KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                          oid,
                                          oidLength,
                                          FALSE,
                                          tlvTag,
                                          tlvValue,
                                          tlvLength
                                         );
    }
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    *encoding = &((*encoding)[*encodingLength]);
  }

  return retVal;
}

/* --- KeyM_CheckDuplicateExtensions ------------------------------------------------------------ */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_CheckDuplicateExtensions
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) extOidList,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) extOidListLength,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) oid,
        uint32                                                              oidLength
)
{
  KeyM_ParseResultType                    retVal = KEYM_PARSERESULT_OK;
  uint8                                   i;
  uint8                                   tlvTag;
  uint32                                  tlvLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tlvValue;

  /* Maximum number of extensions check */
  if (KEYM_X509_EXTENSIONS_NUM_MAX <= *extOidListLength)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_012,1 */
    retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  }
  else
  {
    /* Keep pointer to OID DER structure for duplicate check */
    extOidList[*extOidListLength] = encoding;
    (*extOidListLength)++;
  }

  /* Duplicate extension check */
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* Check for duplicate extensions */
    for (i = 0U; i < (*extOidListLength - 1U); i++)
    {
      /* Return value ignored as value was already parsed and can only return OK */
      (void) KeyM_GetDERTagLengthValue(extOidList[i], (oid - extOidList[i]), &tlvTag, &tlvValue, &tlvLength);
      if (tlvLength == oidLength)
      {
        if (E_OK == TS_MemCmp(tlvValue, oid, oidLength))
        {
          /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_007,1 */
          retVal = KEYM_PARSERESULT_INVALID_FORMAT;
          break;
        }
      }
    }
  }

  return retVal;
}

/* --- KeyM_GetX509Extensions ------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_001,2 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Extensions
(
  /* Deviation MISRAC2012-1 */
         P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
         P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,                AUTOMATIC, KEYM_CONST)     certificateCnfg,
         P2VAR(KeyM_X509BITSTRINGType,                  AUTOMATIC, KEYM_APPL_DATA) extensions
)
{
        KeyM_ParseResultType               retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA)  tempBuffer;
        uint32                             tempBufferLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA)  extensionBuffer;
        uint32                             extensionBufferLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA)  oid;
        uint32                             oidLength;
        uint32                             length;
        uint32                             position;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA)  extOidList[KEYM_X509_EXTENSIONS_NUM_MAX];
        uint8                              extOidListLength = 0U;
        uint8                              foundExtensions = 0U;
        uint8                              i = 0U;
        boolean                            critical = FALSE;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_002,1 */
  if (   (3U <= *encodingLength)
      && ((KEYM_ASN1_DER_TAG_EXPLICIT | KEYM_X509_EXTENSIONS_TAG_NUMBER) == (*encoding)[0U])
     )
  {
    if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&(*encoding)[1U],
                                                    (*encodingLength - 1U),
                                                    &length,
                                                    &position
                                                   )
       )
    {
      /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_003,1 */
      if (*encodingLength >= (1U + position + length))
      {
        extensions->length = length;
        extensions->bitString = &(*encoding)[(1U + position)];
        retVal = KeyM_GetDERSequence(&(*encoding)[(1U + position)], length, &extensionBuffer, &extensionBufferLength);
        if (KEYM_PARSERESULT_OK == retVal)
        {
          /* Parse extensions */
          do
          {
            retVal = KeyM_GetDERSequence(extensionBuffer, extensionBufferLength, &tempBuffer, &tempBufferLength);
            if (KEYM_PARSERESULT_OK == retVal)
            {
              extensionBufferLength -= (tempBuffer - extensionBuffer) + tempBufferLength;
              extensionBuffer        = &tempBuffer[tempBufferLength];
              retVal = KeyM_GetDEROID(tempBuffer, tempBufferLength, &oid, &oidLength);
            }

            /* Check for duplicate extensions */
            if (KEYM_PARSERESULT_OK == retVal)
            {
              retVal = KeyM_CheckDuplicateExtensions(extOidList, &extOidListLength, tempBuffer, oid, oidLength);
            }

            /* Parse critical value */
            if (KEYM_PARSERESULT_OK == retVal)
            {
              tempBufferLength -= (oid - tempBuffer) + oidLength;
              tempBuffer        = &oid[oidLength];

              retVal = KeyM_GetDERBoolean(tempBuffer, tempBufferLength, &critical);

              if (KEYM_PARSERESULT_OK == retVal)
              {
                tempBuffer        = &tempBuffer[3U];
                tempBufferLength -= 3U;
              }
              else
              /* Critical does not exist */
              {
                /* Critical value is optional, default is FALSE */
                /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_004,1 */
                critical = FALSE;
              }
              /* Parse extnValue */
              retVal = KeyM_GetDEROctetString(tempBuffer, tempBufferLength, &tempBuffer, &tempBufferLength);
            }

            /* Set certificate element of extension without iterations to extnValue */
            if (KEYM_PARSERESULT_OK == retVal)
            {
              retVal = KeyM_SetCertificateElement(certificateCnfg,
                                                  KEYM_STRUCTURE_CERTIFICATE_EXTENSION,
                                                  oid,
                                                  oidLength,
                                                  TRUE, /* isStructure */
                                                  0U,
                                                  tempBuffer,
                                                  tempBufferLength
                                                 );
            }

            if (KEYM_PARSERESULT_OK == retVal)
            {
              for (i = 0U; i < KEYM_X509_KNOWN_EXTENSIONS_COUNT; i++)
              {
                if (KeyM_KnownExtensionsList[i].oidLength == oidLength)
                {
                  if (E_OK == TS_MemCmp(oid, KeyM_KnownExtensionsList[i].oid, oidLength))
                  {
                    /* Set a flag for each found extension */
                    foundExtensions |= 1U << i;
                    retVal = KeyM_KnownExtensionsList[i].parseFunc(&tempBuffer, &tempBufferLength, certificateCnfg);
                    if (   (critical  && KeyM_KnownExtensionsList[i].needsNonCritical)
                        || ((!critical) && KeyM_KnownExtensionsList[i].needsCritical)
                       )
                    {
                      retVal = KEYM_PARSERESULT_INVALID_FORMAT;
                    }
                    break;
                  }
                }
              }
            }
            if ((KEYM_PARSERESULT_OK == retVal) && (KEYM_X509_KNOWN_EXTENSIONS_COUNT <= i))
            {
              /* Parse unknown extension */
              if (!critical)
              {
                retVal = KeyM_GetX509UnknownExtensions(&tempBuffer, &tempBufferLength, certificateCnfg, oid, oidLength);
                tempBufferLength = 0U;
              }
              else
              {
                /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_005,1 */
                retVal = KEYM_PARSERESULT_INVALID_FORMAT;
              }
            }
            if ((KEYM_PARSERESULT_OK == retVal) && (extensionBuffer != tempBuffer))
            {
              /* after parsing the current extension no data shall be left in the extension and
                 the pointer shall be set to the beginning of the next extension */
              retVal = KEYM_PARSERESULT_INVALID_FORMAT;
            }
          }
          while ((extensionBufferLength > 0U) && (retVal == KEYM_PARSERESULT_OK));

          if (   (!certificateCnfg->isSelfSigned)
              && (   (!(foundExtensions & (1U << KEYM_X509_EXTENSION_INDEX_AUTH_KEY_ID)))
                  || (!(foundExtensions & (1U << KEYM_X509_EXTENSION_INDEX_SUBJECT_KEY_ID)))
                 )
             )
          {
            /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_AuthKeyId_002,1 */
            /* !LINKSTO EB_KeyM_ParserX509_KeyMExtensions_SubjKeyId_002,1 */
            retVal = KEYM_PARSERESULT_INVALID_FORMAT;
          }
        }
        *encodingLength -= (1U + position + length);
        *encoding        = &(*encoding)[(1U + position + length)];
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetX509TbsCertificate --------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509TbsCertificate
(
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  P2VAR(KeyM_X509FullTBSCertificateType,         AUTOMATIC, KEYM_APPL_DATA) tbsCertificate,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
)
{
        KeyM_ParseResultType              retVal;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) tempBuffer;
        uint32                            tempBufferLength;
        uint32                            structureLength = 0U;

  /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_002,2 */
  retVal = KeyM_GetDERSequence(*encoding, *encodingLength, &tempBuffer, &tempBufferLength);
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tbsCertificate->data   = *encoding;
    tbsCertificate->length = (tempBuffer - *encoding) + tempBufferLength;
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_003,2 */
    retVal = KeyM_GetX509Version(tempBuffer, tempBufferLength, &tbsCertificate->version);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tbsCertificate->version.integer - tempBuffer)
                        + tbsCertificate->version.length;
    tempBuffer        = &tbsCertificate->version.integer[tbsCertificate->version.length];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_004,2 */
    retVal = KeyM_GetX509SerialNumber(tempBuffer, tempBufferLength, &tbsCertificate->serialNumber);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tbsCertificate->serialNumber.integer - tempBuffer)
                        + tbsCertificate->serialNumber.length;
    tempBuffer        = &tbsCertificate->serialNumber.integer[tbsCertificate->serialNumber.length];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_005,2 */
    retVal = KeyM_GetX509Signature(tempBuffer, tempBufferLength, &tbsCertificate->signature);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* AlgorithmIdentifier does not support parameters */
    tempBufferLength -=   (tbsCertificate->signature.algorithm.oid - tempBuffer)
                        + tbsCertificate->signature.algorithm.length + 2U;
    tempBuffer        = &tbsCertificate->signature.algorithm.oid[tbsCertificate->signature.algorithm.length + 2U];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_006,2 */
    retVal = KeyM_GetX509Issuer(tempBuffer,
                                tempBufferLength,
                                certificateCnfg,
                                &structureLength
                               );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -= structureLength;
    tempBuffer = &tempBuffer[structureLength];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_007,2 */
    retVal = KeyM_GetX509Validity(tempBuffer, tempBufferLength, &tbsCertificate->validity);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tbsCertificate->validity.notAfter.time - tempBuffer)
                        + tbsCertificate->validity.notAfter.length;
    tempBuffer = &tbsCertificate->validity.notAfter.time[tbsCertificate->validity.notAfter.length];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_008,2 */
    retVal = KeyM_GetX509Subject(tempBuffer,
                                 tempBufferLength,
                                 certificateCnfg,
                                 &structureLength
                                );
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -= structureLength;
    tempBuffer = &tempBuffer[structureLength];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_009,2 */
    retVal = KeyM_GetX509SubjectPublicKeyInfo(tempBuffer, tempBufferLength, &tbsCertificate->subjectPublicKeyInfo);
  }
  if (KEYM_PARSERESULT_OK == retVal)
  {
    tempBufferLength -=   (tbsCertificate->subjectPublicKeyInfo.subjectPublicKey.bitString - tempBuffer)
                        + tbsCertificate->subjectPublicKeyInfo.subjectPublicKey.length;
    tempBuffer        = &tbsCertificate->subjectPublicKeyInfo.subjectPublicKey.bitString[tbsCertificate->subjectPublicKeyInfo.subjectPublicKey.length];
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_010,1 */
    KeyM_GetX509IssuerUniqueId(&tempBuffer, &tempBufferLength, &tbsCertificate->issuerUniqueID);
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_012,1 */
    KeyM_GetX509SubjectUniqueId(&tempBuffer, &tempBufferLength, &tbsCertificate->subjectUniqueID);
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_014,1 */
    retVal = KeyM_GetX509Extensions(&tempBuffer, &tempBufferLength, certificateCnfg, &tbsCertificate->extensions);
  }

  if (KEYM_PARSERESULT_OK == retVal)
  {
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_011,1 */
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_013,1 */
    /* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509TbsCertificate_015,1 */
    *encodingLength -= (tempBuffer - *encoding) + tempBufferLength;
    *encoding        = tempBuffer;
  }
  return retVal;
}

/* --- KeyM_GetX509SignatureAlgorithm ----------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SignatureAlgorithm_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SignatureAlgorithm
(
  P2VAR(uint8,                            AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                       encodingLength,
  P2VAR(KeyM_X509AlgorithmIdentifierType, AUTOMATIC, KEYM_APPL_DATA) signatureAlgorithm
)
{
  return KeyM_GetX509AlgorithmIdentifier(encoding, encodingLength, signatureAlgorithm);
}

/* --- KeyM_GetX509SignatureValue --------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMGetX509SignatureValue_001,1 */

static FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509SignatureValue
(
  P2VAR(uint8,                  AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                             encodingLength,
  P2VAR(KeyM_X509BITSTRINGType, AUTOMATIC, KEYM_APPL_DATA) signatureValue
)
{
  return KeyM_GetDERBitString(encoding,
                              encodingLength,
                              &signatureValue->bitString,
                              &signatureValue->length
                             );
}

/* --- KeyM_CopyCertificate --------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserX509_KeyMCopyCertificate_001,1 */

static FUNC(void, KEYM_CODE) KeyM_CopyCertificate
(
               KeyM_X509FullCertificateType                     srcCertificate,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST) certificateCnfg
)
{
  uint32 i;

  for (i = 0U; i < certificateCnfg->certElementsNumber; i++)
  {
    if (certificateCnfg->certElements[i].cnfg->objectIdLength > 0U)
    {
      /* Skip copying sub elements */
      continue;
    }

    if (certificateCnfg->certElements[i].cnfg->hasIterations)
    {
      /* Skip copying iterations */
      continue;
    }

    switch (certificateCnfg->certElements[i].cnfg->structure)
    {
      case KEYM_STRUCTURE_CERTIFICATE_VERSION_NUMBER:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.version.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.version.integer;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SERIAL_NUMBER:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.serialNumber.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.serialNumber.integer;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.validity.notBefore.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.validity.notBefore.time;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.validity.notAfter.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.validity.notAfter.time;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.subjectPublicKeyInfo.algorithm.algorithm.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.subjectPublicKeyInfo.algorithm.algorithm.oid;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLICKEY_INFO_SUBJECT_PUBLICKEY:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.subjectPublicKeyInfo.subjectPublicKey.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.subjectPublicKeyInfo.subjectPublicKey.bitString;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_ISSUER_UNIQUE_NAME:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.issuerUniqueID.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.issuerUniqueID.bitString;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.subjectUniqueID.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.subjectUniqueID.bitString;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_EXTENSION:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.tbsCertificate.extensions.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.tbsCertificate.extensions.bitString;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.signatureAlgorithm.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.signatureAlgorithm.algorithmIdentifier;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM_ID:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.signatureAlgorithm.algorithm.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.signatureAlgorithm.algorithm.oid;
        break;
      }
      case KEYM_STRUCTURE_CERTIFICATE_SIGNATURE:
      {
        certificateCnfg->certElements[i].data[0U].dataLength = srcCertificate.signatureValue.length;
        certificateCnfg->certElements[i].data[0U].dataPtr    = srcCertificate.signatureValue.bitString;
        break;
      }
      default:
      {
        /* Other structure types are already successfully set */
        break;
      }
    }
  }

  /* Set needed elements */
  /* signature */
  certificateCnfg->certElementDataSignatureValue->dataLength = srcCertificate.signatureValue.length;
  certificateCnfg->certElementDataSignatureValue->dataPtr    = srcCertificate.signatureValue.bitString;

  /* public key */
  certificateCnfg->certElementDataPublicKey->dataLength = srcCertificate.tbsCertificate.subjectPublicKeyInfo.subjectPublicKey.length;
  certificateCnfg->certElementDataPublicKey->dataPtr    = srcCertificate.tbsCertificate.subjectPublicKeyInfo.subjectPublicKey.bitString;

  /* tbsCertificate */
  certificateCnfg->certTbsCertificate->dataLength = srcCertificate.tbsCertificate.length;
  certificateCnfg->certTbsCertificate->dataPtr    = srcCertificate.tbsCertificate.data;
}

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[end of file]=================================================================*/

