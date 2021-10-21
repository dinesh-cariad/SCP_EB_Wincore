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

#ifndef KEYM_INT_TYPES_H
#define KEYM_INT_TYPES_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM_Int_Types.h/0001,1 */

#include <Std_Types.h>
#include <Csm_Types.h>
#include <KeyM_Types.h>
#include <KeyM_Cfg.h>

/*==================[macros]======================================================================*/

/* --- KeyM_CryptoKeyStorageType ---------------------------------------------------------------- */
#if (defined KEYM_STORAGE_IN_NVM)
#error KEYM_STORAGE_IN_NVM is already defined
#endif
/** \brief  Storage class NvM. **/
#define KEYM_STORAGE_IN_NVM 0U

#if (defined KEYM_STORAGE_IN_RAM)
#error KEYM_STORAGE_IN_RAM is already defined
#endif
/** \brief  Storage class ram. **/
#define KEYM_STORAGE_IN_RAM 1U

#if (defined KEYM_STATE_UNINIT)
#error KEYM_STATE_UNINIT is already defined
#endif
/** \brief  Uninitialized state of the KeyM. **/
#define KEYM_STATE_UNINIT 0U

#if (defined KEYM_STATE_INIT)
#error KEYM_STATE_INIT is already defined
#endif
/** \brief  Initialized state of the KeyM. **/
#define KEYM_STATE_INIT 1U

#if (defined KEYM_STATE_DEINIT)
#error KEYM_STATE_DEINIT is already defined
#endif
/** \brief  Deinitialization state of the KeyM. **/
#define KEYM_STATE_DEINIT 2U

#if (defined KEYM_STATE_IDLE)
#error KEYM_STATE_IDLE is already defined
#endif
/** \brief  Idle state of the KeyM. **/
#define KEYM_STATE_IDLE 3U

#if (defined KEYM_STATE_SERVICE_CERTIFICATE)
#error KEYM_STATE_SERVICE_CERTIFICATE is already defined
#endif
/** \brief  Service certificate state of the KeyM. **/
#define KEYM_STATE_SERVICE_CERTIFICATE 4U

#if (defined KEYM_STATE_SET_CERTIFICATE)
#error KEYM_STATE_SET_CERTIFICATE is already defined
#endif
/** \brief  Set certificate state of the KeyM. **/
#define KEYM_STATE_SET_CERTIFICATE 5U

#if (defined KEYM_STATE_GET_CERTIFICATE)
#error KEYM_STATE_GET_CERTIFICATE is already defined
#endif
/** \brief  Get certificate state of the KeyM. **/
#define KEYM_STATE_GET_CERTIFICATE 6U

#if (defined KEYM_STATE_VERIFY_CERTIFICATE)
#error KEYM_STATE_VERIFY_CERTIFICATE is already defined
#endif
/** \brief  Verify certificate state of the KeyM. **/
#define KEYM_STATE_VERIFY_CERTIFICATE 7U

#if (defined KEYM_STATE_CERT_GET_STATUS)
#error KEYM_STATE_CERT_GET_STATUS is already defined
#endif
/** \brief  Get certificate status state of the KeyM. **/
#define KEYM_STATE_CERT_GET_STATUS 8U

#if (defined KEYM_STATE_CERT_ELEMENT_GET)
#error KEYM_STATE_CERT_ELEMENT_GET is already defined
#endif
/** \brief  Get certificate element state of the KeyM. **/
#define KEYM_STATE_CERT_ELEMENT_GET 9U

#if (defined KEYM_STRUCTURE_CERTIFICATE_EXTENSION)
#error KEYM_STRUCTURE_CERTIFICATE_EXTENSION is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateExtension". **/
#define KEYM_STRUCTURE_CERTIFICATE_EXTENSION 0U

#if (defined KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME)
#error KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateIssuerName". **/
#define KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME 1U

#if (defined KEYM_STRUCTURE_CERTIFICATE_ISSUER_UNIQUE_NAME)
#error KEYM_STRUCTURE_CERTIFICATE_ISSUER_UNIQUE_NAME is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateIssuerUniqueIdentifier". **/
#define KEYM_STRUCTURE_CERTIFICATE_ISSUER_UNIQUE_NAME 2U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SERIAL_NUMBER)
#error KEYM_STRUCTURE_CERTIFICATE_SERIAL_NUMBER is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSerialNumber". **/
#define KEYM_STRUCTURE_CERTIFICATE_SERIAL_NUMBER 3U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SIGNATURE)
#error KEYM_STRUCTURE_CERTIFICATE_SIGNATURE is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSignature". **/
#define KEYM_STRUCTURE_CERTIFICATE_SIGNATURE 4U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM)
#error KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSignatureAlgorithm". **/
#define KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM 5U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM_ID)
#error KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM_ID is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSignatureAlgorithmID". **/
#define KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM_ID 6U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME)
#error KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSubjectName". **/
#define KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME 7U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM)
#error KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSubjectPublicKeyInfo_PublicKeyAlgorithm". **/
#define KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM 8U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLICKEY_INFO_SUBJECT_PUBLICKEY)
#error KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLICKEY_INFO_SUBJECT_PUBLICKEY is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSubjectPublicKeyInfo_SubjectPublicKey". **/
#define KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLICKEY_INFO_SUBJECT_PUBLICKEY 9U

#if (defined KEYM_STRUCTURE_CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER)
#error KEYM_STRUCTURE_CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateSubjectUniqueIdentifier". **/
#define KEYM_STRUCTURE_CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER 10U

#if (defined KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER)
#error KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateValidityPeriodNotAfter". **/
#define KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER 11U

#if (defined KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE)
#error KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateValidityPeriodNotBefore". **/
#define KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE 12U

#if (defined KEYM_STRUCTURE_CERTIFICATE_VERSION_NUMBER)
#error KEYM_STRUCTURE_CERTIFICATE_VERSION_NUMBER is already defined
#endif
/** \brief  The enum value of the certificate structure "CertificateVersionNumber". **/
#define KEYM_STRUCTURE_CERTIFICATE_VERSION_NUMBER 13U

/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00001,3 */
#if (KEYM_API_ENABLED_NVM == STD_ON)
#if (defined KEYM_CERTIFICATE_NVM_READ_NOTSET)
#error KEYM_CERTIFICATE_NVM_READ_NOTSET is already defined
#endif
/** \brief no NvM block configured, no NvM_ReadBlock needed **/
#define KEYM_CERTIFICATE_NVM_READ_NOTSET 0U

#if (defined KEYM_CERTIFICATE_NVM_READ_REQUESTED)
#error KEYM_CERTIFICATE_NVM_READ_REQUESTED is already defined
#endif
/** \brief NvM_ReadBlock done, NvM feedback outstanding **/
#define KEYM_CERTIFICATE_NVM_READ_REQUESTED 1U

#if (defined KEYM_CERTIFICATE_NVM_READ_SUCCESSFUL)
#error KEYM_CERTIFICATE_NVM_READ_SUCCESSFUL is already defined
#endif
/** \brief NvM_ReadBlock finished successfully (NvM_GetErrorStatus) **/
#define KEYM_CERTIFICATE_NVM_READ_SUCCESSFUL 2U

#if (defined KEYM_CERTIFICATE_NVM_READ_FAILED)
#error KEYM_CERTIFICATE_NVM_READ_FAILED is already defined
#endif
/** \brief NvM_ReadBlock failed (NvM_GetErrorStatus) **/
#define KEYM_CERTIFICATE_NVM_READ_FAILED 3U
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

/* --- KeyM_MainFunction_StateType -------------------------------------------------------------- */
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00000,1 */
#if (KEYM_API_ENABLED_NVM == STD_ON)
#if (defined KEYM_MFUNC_STATE_NVMREAD)
#error KEYM_MFUNC_STATE_NVMREAD is already defined
#endif
/** \brief Call NvM_ReadBlock for all KeyM keys with KEYM_STORAGE_IN_NVM **/
#define KEYM_MFUNC_STATE_NVMREAD 0U

#if (defined KEYM_MFUNC_STATE_NVMREADERRORSTATUS)
#error KEYM_MFUNC_STATE_NVMREADERRORSTATUS is already defined
#endif
/** \brief Wait for NvM read error status and check result **/
#define KEYM_MFUNC_STATE_NVMREADERRORSTATUS 1U

#if (defined KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS)
#error KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS is already defined
#endif
/** \brief Wait for NvM write error status and check result **/
#define KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS 2U
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

#if (defined KEYM_MFUNC_STATE_VRFY_INIT)
#error KEYM_MFUNC_STATE_VRFY_INIT is already defined
#endif
/** \brief Initialize next verification processing **/
#define KEYM_MFUNC_STATE_VRFY_INIT 3U

#if (defined KEYM_MFUNC_STATE_VRFY_PARSE)
#error KEYM_MFUNC_STATE_VRFY_PARSE is already defined
#endif
/** \brief Parse an unparsed certificate of a chain with verification request **/
#define KEYM_MFUNC_STATE_VRFY_PARSE 4U

#if (defined KEYM_MFUNC_STATE_VRFY_VERIFY)
#error KEYM_MFUNC_STATE_VRFY_VERIFY is already defined
#endif
/** \brief Verify an unverified certificate of a chain with verification request **/
#define KEYM_MFUNC_STATE_VRFY_VERIFY 5U

#if (defined KEYM_MFUNC_STATE_VRFY_PREPROOT)
#error KEYM_MFUNC_STATE_VRFY_PREPROOT is already defined
#endif
/** \brief Root cert: store parsed key into associated Csm job before verification **/
#define KEYM_MFUNC_STATE_VRFY_PREPROOT 6U

#if (defined KEYM_MFUNC_STATE_VRFY_STORE_NVM)
#error KEYM_MFUNC_STATE_VRFY_STORE_NVM is already defined
#endif
/** \brief Call NvM_WriteBlock for a successfully verified certificate **/
#define KEYM_MFUNC_STATE_VRFY_STORE_NVM 7U

#if (defined KEYM_MFUNC_STATE_VRFY_WAITCSMCALLBACK)
#error KEYM_MFUNC_STATE_VRFY_WAITCSMCALLBACK is already defined
#endif
/** \brief Wait for callback of the called Csm signature verification **/
#define KEYM_MFUNC_STATE_VRFY_WAITCSMCALLBACK 8U

#if (defined KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY)
#error KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY is already defined
#endif
/** \brief Store parsed public key after successful verification into associated Csm job **/
#define KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY 9U

#if (defined KEYM_MFUNC_STATE_VRFY_FINISH)
#error KEYM_MFUNC_STATE_VRFY_FINISH is already defined
#endif
/** \brief Complete and clean up after verification **/
#define KEYM_MFUNC_STATE_VRFY_FINISH 10U

#if (defined KEYM_MFUNC_STATE_PREPARSE_PARSE)
#error KEYM_MFUNC_STATE_PREPARSE_PARSE is already defined
#endif
/** \brief Parse next unparsed certificate, incl. rule validation, without request **/
#define KEYM_MFUNC_STATE_PREPARSE_PARSE 11U

#if (defined KEYM_MFUNC_STATE_VRFY_ERROR)
#error KEYM_MFUNC_STATE_VRFY_ERROR is already defined
#endif
/** \brief Do error handling of verification **/
#define KEYM_MFUNC_STATE_VRFY_ERROR 12U

/* --- KeyM_ParseResultType --------------------------------------------------------------- */
#if (defined KEYM_PARSERESULT_OK)
#error KEYM_PARSERESULT_OK is already defined
#endif
/** \brief Parsing successful **/
#define KEYM_PARSERESULT_OK 0U

#if (defined KEYM_PARSERESULT_NOT_OK)
#error KEYM_PARSERESULT_NOT_OK is already defined
#endif
/** \brief Parsing not successful, undefined error **/
#define KEYM_PARSERESULT_NOT_OK 1U

#if (defined KEYM_PARSERESULT_INVALID_TYPE)
#error KEYM_PARSERESULT_INVALID_TYPE is already defined
#endif
/** \brief Parsing not successful, because certificate type is invalid **/
#define KEYM_PARSERESULT_INVALID_TYPE 2U

#if (defined KEYM_PARSERESULT_INVALID_FORMAT)
#error KEYM_PARSERESULT_INVALID_FORMAT is already defined
#endif
/** \brief Parsing not successful, because the certificate's format is **/
#define KEYM_PARSERESULT_INVALID_FORMAT 3U

#if (defined KEYM_PARSERESULT_INVALID_CONTENT)
#error KEYM_PARSERESULT_INVALID_CONTENT is already defined
#endif
/** \brief Parsing not successful, because the certificate's content is invalid **/
#define KEYM_PARSERESULT_INVALID_CONTENT 4U

/*==================[type definitions]============================================================*/

/** \brief  Typedef for the CertificateVerify callback. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_CbkCertificateVerifyType,1 */
typedef P2FUNC(Std_ReturnType, CRYPTO_CODE, KeyM_CbkCertificateVerifyType)(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result);

/** \brief  Typedef for the ServiceCertificate callback. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_CbkServiceCertificateType,1 */
typedef P2FUNC(void, CRYPTO_CODE, KeyM_CbkServiceCertificateType)(KeyM_CertificateIdType CertId, KeyM_ResultType Result, uint16 ResultDataLength, uint8* ResultDataPtr);

/** \brief  Type for a certificate element rule function. **/
typedef P2FUNC(boolean, CRYPTO_CODE, KeyM_CertificateElementRuleType)(void);

/** \brief  Type for the constant part of a certificate element. **/
typedef struct
{
         CONST(uint8,              KEYM_CONST) maxIterations;
         CONST(uint16,             KEYM_CONST) elementId; /* Identifier of a certificate element. */
         CONST(uint16,             KEYM_CONST) maxLength; /* Maximum length in bytes. */
  CONSTP2CONST(uint8,   AUTOMATIC, KEYM_CONST) objectId; /* This is the object identifier (OID) that is used to identify the certificate element within its element structure. */
         CONST(uint8,              KEYM_CONST) objectIdLength;
         CONST(uint8,              KEYM_CONST) objectType; /* Certificate elements are stored in ASN.1 format. In this item the type of ASN.1 TLV can be specified (e.g. INTEGER has the value '2'). This can be used to identify only such certificate elements. If the type is different, the element is not included in the search. */
         CONST(uint8,              KEYM_CONST) structure; /* This defines in which structure the certificate element is located. */
         CONST(boolean,            KEYM_CONST) hasIterations; /* Defines if the certificate element can occur more than one time. If so, the iterator can be used to retrieve the individual data values of this certificate element. */
         CONST(boolean,            KEYM_CONST) hasObjectType;
         CONST(boolean,            KEYM_CONST) isMandatory;
}
KeyM_CertificateElementCnfgType;

/** \brief  Type for the variable part of a certificate element. **/
typedef struct
{
        uint32                              dataLength;
  P2VAR(uint8,   AUTOMATIC, KEYM_APPL_DATA) dataPtr;
}
KeyM_CertificateElementDataType;

/** \brief  Type to combine the constant and variable part of a certificate element. **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirstNext/00001,1 */
typedef struct
{
  CONSTP2CONST(KeyM_CertificateElementCnfgType, AUTOMATIC, KEYM_CONST) cnfg;
    CONSTP2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_CONST) data;
}
KeyM_CertificateElementType;

/** \brief  Type for the constant part of a certificate. **/
#ifndef RTE_TYPE_KeyM_CertificateType
#define RTE_TYPE_KeyM_CertificateType
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_CertificateCnfgType,1 */
typedef struct
{
         CONST(uint32,                                     KEYM_CONST) certMaxLength;
         CONST(uint32,                                     KEYM_CONST) csmSignatureVerifyJobId;
         CONST(uint32,                                     KEYM_CONST) csmSignatureVerifyKeyId;
         CONST(uint16,                                     KEYM_CONST) certUpperCertId;
         CONST(uint16,                                     KEYM_CONST) keymStorageKey;
         CONST(uint16,                                     KEYM_CONST) certNameLength;
         CONST(uint8,                                      KEYM_CONST) csmSignatureVerifyJobProcessingType;
  CONSTP2CONST(uint8,                           AUTOMATIC, KEYM_CONST) certName;
         CONST(KeyM_CbkCertificateVerifyType,              KEYM_CONST) keymCbkCertificateVerify;
         CONST(KeyM_CbkServiceCertificateType,             KEYM_CONST) keymCbkServiceCertificate;
         CONST(boolean,                                    KEYM_CONST) isSelfSigned;

         CONST(uint16,                                     KEYM_CONST) certElementRulesNumber;
  CONSTP2CONST(KeyM_CertificateElementRuleType, AUTOMATIC, KEYM_CONST) certElementRules;

         CONST(uint16,                                     KEYM_CONST) certElementsNumber;
  CONSTP2CONST(KeyM_CertificateElementType,     AUTOMATIC, KEYM_CONST) certElements;

         P2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_APPL_DATA) certElementDataIssuer;
         P2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_APPL_DATA) certElementDataSubject;
         P2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_APPL_DATA) certElementDataSignatureValue;
         P2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_APPL_DATA) certElementDataPublicKey;
         P2VAR(KeyM_CertificateElementDataType, AUTOMATIC, KEYM_APPL_DATA) certTbsCertificate;
}
KeyM_CertificateCnfgType;

#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
/** \brief  Type for the NvMRead status **/
typedef uint8 KeyM_CertificateNvMReadStatusType;
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief  Type for the variable part of a certificate. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_CertificateDataType,1 */
typedef struct
{
  boolean flgServiceRequested;
  boolean flgParsed;

#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00001,3 */
  KeyM_CertificateNvMReadStatusType certNvMReadStatus;
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */

  KeyM_CertificateStatusType certStatus;
}
KeyM_CertificateDataType;

/** \brief  Type to combine the constant and variable part of a certificate. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_CertificateType,1 */
typedef struct
{
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST) cnfg;
    CONSTP2VAR(KeyM_CertificateDataType, AUTOMATIC, KEYM_CONST) data;
}
KeyM_CertificateType;
#endif /* #ifndef RTE_TYPE_KeyM_CertificateType */

/** \brief  Type to define the key storage. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_KeyStorageType,1 */
typedef uint8 KeyM_KeyStorageType;

/** \brief  Type for the constant part of a key. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_KeyCnfgType,1 */
typedef struct
{
  CONST(uint32, KEYM_CONST) keyMaxLength;

  CONST(uint16, KEYM_CONST) nvmBlockDescriptorId;

  CONSTP2VAR(uint8, AUTOMATIC, KEYM_CONST) keyData;

  CONST(KeyM_KeyStorageType, KEYM_CONST) keyStorage;
}
KeyM_KeyCnfgType;

/** \brief  Type for the variable part of a key. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_KeyDataType,1 */
typedef struct
{
  uint32 keyLength;
}
KeyM_KeyDataType;

/** \brief  Type to combine the constant and variable part of a key. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_KeyType,1 */
typedef struct
{
  CONSTP2CONST(KeyM_KeyCnfgType, AUTOMATIC, KEYM_CONST) cnfg;
    CONSTP2VAR(KeyM_KeyDataType, AUTOMATIC, KEYM_CONST) data;
}
KeyM_KeyType;

/** \brief  This structure is the base type to initialize the Key Manager module. A pointer to
 **         an instance of this structure will be used in the initialization of the Key Manager
 **         module. This type is implementation specific.
 **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_IntConfigType,1 */
typedef struct
{
  CONSTP2CONST(KeyM_KeyType,         AUTOMATIC, KEYM_CONST) keys;
  CONSTP2CONST(KeyM_CertificateType, AUTOMATIC, KEYM_CONST) certificates;
}
KeyM_IntConfigType;

/** \brief  Type to store the state. **/
/* !LINKSTO KeyM.Dsn.Api/KeyM_StateType,1 */
typedef uint8 KeyM_StateType;

/** \brief  Type to store the KeyM_MainFunctions state. **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00000,1 */
typedef uint8 KeyM_MainFunction_StateType;

/** \brief  Type indicates the parsing result. **/
/* !LINKSTO KeyM.Dsn.Api/Types/KeyM_ParseResultType,1 */
typedef uint8 KeyM_ParseResultType;

/** \brief  Typedef for the X509 extension parsing functions. **/
typedef P2FUNC(KeyM_ParseResultType, CRYPTO_CODE, KeyM_ExtensionParseFuncType)
(
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) encoding,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType,         AUTOMATIC, KEYM_CONST)     certificateCnfg
);

/** \brief  Type to store the informations of a known extension. **/
typedef struct
{
  CONSTP2CONST(uint8,                       AUTOMATIC, KEYM_CONST) oid;
         CONST(uint32,                      AUTOMATIC)             oidLength;
               KeyM_ExtensionParseFuncType                         parseFunc;
               boolean                                             needsCritical;
               boolean                                             needsNonCritical;
}
KeyM_ExtensionInfoType;

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

/*================================================================================================*/

#endif /* #ifndef KEYM_INT_TYPES_H */

/*==================[end of file]=================================================================*/

