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

[!/* ====================================================================================== */!][!//
[!INCLUDE "KeyM_Macros.m"!][!//
[!AUTOSPACING!][!//
[!/* ====================================================================================== */!][!//
[!//
#ifndef KEYM_INT_CFG_H
#define KEYM_INT_CFG_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM_Int_Cfg.h/0001,1 */

#include <KeyM_Cfg.h>
#include <KeyM_Int_Types.h>

/*==================[macros]======================================================================*/

[!VAR "callbackEnabled"!]STD_OFF[!ENDVAR!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!IF "node:ref(node:ref(./KeyMCertCsmSignatureVerifyJobRef)/CsmJobPrimitiveRef)/*/*/CsmSignatureVerifyProcessing = 'CSM_ASYNCHRONOUS'"!]
    [!VAR "callbackEnabled"!]STD_ON[!ENDVAR!]
  [!ENDIF!]
[!ENDLOOP!]
/* !LINKSTO KeyM.Dsn.Api/Macros/0014,1 */
#if (defined KEYM_API_ENABLED_CSMCALLBACK)
#error KEYM_API_ENABLED_CSMCALLBACK is already defined
#endif
/** \brief This macro is used to enable the csm callback. **/
#define KEYM_API_ENABLED_CSMCALLBACK [!"$callbackEnabled"!]

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

#define KEYM_START_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
/** \brief  Constant part of key [!"node:name(.)"!]. **/
extern CONST(KeyM_KeyCnfgType, AUTOMATIC) KeyM_KeyCnfg_[!"node:name(.)"!];

[!ENDLOOP!]
/** \brief  Array to hold the Crypto keys. **/
extern CONST(KeyM_KeyType, AUTOMATIC) KeyM_Keys[KEYM_KEY_COUNT];

[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
/** \brief  Constant part of certificate [!"node:name(.)"!].  **/
extern CONST(KeyM_CertificateCnfgType, AUTOMATIC) KeyM_CertificateCnfg_[!"node:name(.)"!];

[!ENDLOOP!]
/** \brief  Array to hold the certificates. **/
extern CONST(KeyM_CertificateType, AUTOMATIC) KeyM_Certificates[KEYM_CERT_COUNT];

/** \brief  Configuration to hold all internal data. **/
extern CONST(KeyM_IntConfigType, AUTOMATIC) KeyM_Config;

[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!VAR "RulesCount" = "num:i(count(./KeyMCertCertificateElementRuleRef/*))"!]
  [!IF "$RulesCount > 0"!]
extern CONST(KeyM_CertificateElementRuleType, AUTOMATIC) KeyM_CertificateElementRules_[!"node:name(.)"!][[!"$RulesCount"!]];

  [!ENDIF!]
[!ENDLOOP!]
#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_CONST_8
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
/** \brief  Buffer to store the name of a certificate. **/
extern CONST(uint8, AUTOMATIC) KeyM_CertificateName_[!"node:name(.)"!][[!"num:i(string-length(node:value(KeyMCertificateName)))"!]];

[!ENDLOOP!]
#define KEYM_STOP_SEC_CONST_8
#include <KeyM_MemMap.h>

/*==================[external data]===============================================================*/

#define KEYM_START_SEC_VAR_INIT_8
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
/** \brief  Buffer to store the data of a certificate. **/
extern VAR(uint8, KEYM_VAR) KeyM_KeyBuffer_[!"node:name(.)"!][[!"node:value(./KeyMCryptoKeyMaxLength)"!]];

[!ENDLOOP!]
#define KEYM_STOP_SEC_VAR_INIT_8
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
/** \brief  Variable part of keys. **/
extern VAR(KeyM_KeyDataType, KEYM_VAR) KeyM_KeyData_[!"node:name(.)"!];

[!ENDLOOP!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
/** \brief  Variable part of certificates. **/
extern VAR(KeyM_CertificateDataType, KEYM_VAR) KeyM_CertificateData_[!"node:name(.)"!];

[!ENDLOOP!]
[!MACRO "GenerateMandatoryCertificateElementDataPrototype", "CertElem"!]
  [!INDENT "0"!]
  [!IF "not(node:exists(KeyMCertificateElement/*[    (node:value(KeyMCertificateElementOfStructure) = $CertElem)
                                                 and (./KeyMCertificateElementHasIteration = 'false')
                                                 and not(node:exists(KeyMCertificateElementObjectId))
                                                 and not(node:exists(KeyMCertificateElementObjectType))
                                                ]))"!]
/** \brief  Variable part of an internal only certificate element. **/
extern VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_[!"$CertElem"!];

  [!ENDIF!]
  [!ENDINDENT!]
[!ENDMACRO!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!CALL "GenerateMandatoryCertificateElementDataPrototype", "CertElem" = "'CertificateIssuerName'"!]
  [!CALL "GenerateMandatoryCertificateElementDataPrototype", "CertElem" = "'CertificateSubjectName'"!]
  [!CALL "GenerateMandatoryCertificateElementDataPrototype", "CertElem" = "'CertificateSignature'"!]
  [!CALL "GenerateMandatoryCertificateElementDataPrototype", "CertElem" = "'CertificateSubjectPublicKeyInfo_SubjectPublicKey'"!]
  [!LOOP "KeyMCertificateElement/*"!]
/** \brief  Variable part of certificate elements. **/
extern VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementData_[!"node:name(.)"!][[!CALL "GetCertElementIterations"!]];

  [!ENDLOOP!]
/** \brief  The tbsCertificate of the certificate. **/
extern VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_TbsCertificate;

[!ENDLOOP!]
#define KEYM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

/*==================[external function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

[!LOOP "KeyM:GetCertificateVerificationRuleSet(KeyMCertificate/*/KeyMCertCertificateElementRuleRef/*)"!]
/** \brief  KeyMCertCertificateElementRule for '[!"node:name(./../..)"!]/[!"node:name(.)"!]' **/
extern FUNC(boolean, KEYM_CODE) KeyM_CertificateElementRule_[!"node:name(./../..)"!]_[!"node:name(.)"!]
(
  void
);

[!ENDLOOP!]
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef KEYM_INT_CFG_H */

/*==================[end of file]=================================================================*/

