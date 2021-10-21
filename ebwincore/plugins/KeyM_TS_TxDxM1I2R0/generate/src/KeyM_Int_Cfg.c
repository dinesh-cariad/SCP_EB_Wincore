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
[!INCLUDE "include/KeyM_Macros.m"!][!//
[!AUTOSPACING!][!//
[!/* ====================================================================================== */!][!//
/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM_Int_Cfg.c/0001,1 */

#include <KeyM_Int_Cfg.h>
#include <KeyM_Externals.h>

#include <TSMem.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

[!LOOP "node:filter(node:refs(KeyM:GetCertificateVerificationRuleSet(KeyMCertificate/*/KeyMCertCertificateElementRuleRef/*)/KeyMArgumentRef/*), 'node:exists(./KeyMCertificateElementConditionValue)')"!]
/* --- KeyM_CertificateElementCondition_[!"node:name(./../..)"!]_[!"node:name(.)"!] [!"substring($MINUS, 1, 100-43-string-length(node:name(./../..))-string-length(node:name(.)))"!]--- */
static FUNC(boolean, KEYM_CODE) KeyM_CertificateElementCondition_[!"node:name(./../..)"!]_[!"node:name(.)"!]
(
  void
);
[!ENDLOOP!]
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal constants]==========================================================*/

#define KEYM_START_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

[!LOOP "KeyMCertificate/*/KeyMCertificateElement/*"!]
  [!IF "node:exists(./KeyMCertificateElementObjectId)"!]
static CONST(uint8, AUTOMATIC) KeyM_CertificateElementObjectId_[!"node:name(.)"!][[!"num:i(count(text:split(text:replaceAll(./KeyMCertificateElementObjectId, '\s+', ''), ',')))"!]] =[!//
  [!CALL "FormatArray", "Array" = "node:value(./KeyMCertificateElementObjectId)", "Indentation" = "0"!];

  [!ENDIF!]
[!ENDLOOP!]
[!LOOP "node:order(KeyMCertificate/*/KeyMCertificateElement/*, 'node:value(./KeyMCertificateElementId)')"!]
static CONST(KeyM_CertificateElementCnfgType, AUTOMATIC) KeyM_CertificateElementCnfg_[!"node:name(.)"!] =
{
  [!IF "node:value(./KeyMCertificateElementHasIteration)"!]
  [!"node:value(./KeyMCertificateElementMaxIterations)"!]U, /* maxIterations */
  [!ELSE!]
  1U, /* maxIterations */
  [!ENDIF!]
  [!"node:value(./KeyMCertificateElementId)"!]U, /* elementId */
  [!"node:value(./KeyMCertificateElementMaxLength)"!]U, /* maxLength */
  [!IF "node:exists(./KeyMCertificateElementObjectId)"!][!//
  KeyM_CertificateElementObjectId_[!"node:name(.)"!], /* objectId */
  [!"num:i(count(text:split(text:replaceAll(./KeyMCertificateElementObjectId, '\s+', ''), ',')))"!]U, /* objectIdLength */
  [!ELSE!]
  NULL_PTR, /* objectId */
  0U, /* objectIdLength */
  [!ENDIF!]
  [!IF "node:exists(./KeyMCertificateElementObjectType)"!]
  [!"node:value(./KeyMCertificateElementObjectType)"!]U, /* objectType */
  [!ELSE!]
  0U, /* objectType */
  [!ENDIF!]
  [!CALL "StructureEnumToDefine", "Structure" = "node:value(./KeyMCertificateElementOfStructure)"!], /* structure */
  [!"text:toupper(./KeyMCertificateElementHasIteration)"!], /* hasIterations */
  [!"text:toupper(node:exists(./KeyMCertificateElementObjectType))"!], /* hasObjectType */
  [!"text:toupper(./KeyMCertificateElementIsMandatory)"!] /* isMandatory */
};

[!ENDLOOP!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!IF "num:i(count(./KeyMCertificateElement/*)) > 0"!]
static CONST(KeyM_CertificateElementType, AUTOMATIC) KeyM_CertificateElements_[!"node:name(.)"!][[!"num:i(count(./KeyMCertificateElement/*))"!]U] =
{
    [!LOOP "node:order(KeyMCertificateElement/*, 'node:value(./KeyMCertificateElementId)')"!][!//
  {
    &KeyM_CertificateElementCnfg_[!"node:name(.)"!], /* cnfg */
    KeyM_CertificateElementData_[!"node:name(.)"!] /* data */
  },
    [!ENDLOOP!]
};

  [!ENDIF!]
[!ENDLOOP!]
#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

#define KEYM_START_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
/* !LINKSTO KeyM.Dsn.Api/Consts/0003,1 */
CONST(KeyM_KeyCnfgType, AUTOMATIC) KeyM_KeyCnfg_[!"node:name(.)"!] =
{
  [!"node:value(KeyMCryptoKeyMaxLength )"!]U, /* keyMaxLength */
  [!IF "(node:exists(./KeyMNvmBlock/KeyMNvmBlockDescriptorRef))"!]
  [!"node:value(node:ref(./KeyMNvmBlock/KeyMNvmBlockDescriptorRef)/NvMNvramBlockIdentifier)"!]U[!//
  [!ELSE!]
  0U[!//
  [!ENDIF!], /* nvmBlockDescriptorId */
  KeyM_KeyBuffer_[!"node:name(.)"!], /* keyData */
  [!"node:value(KeyMCryptoKeyStorage)"!] /* keyStorage */
};

[!ENDLOOP!]
/* !LINKSTO KeyM.Dsn.Api/Consts/0002,1 */
CONST(KeyM_KeyType, AUTOMATIC) KeyM_Keys[KEYM_KEY_COUNT] =
{
[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
  {
    &KeyM_KeyCnfg_[!"node:name(.)"!], /* cnfg */
    &KeyM_KeyData_[!"node:name(.)"!] /* data */
  },
[!ENDLOOP!]
};

[!MACRO "GetCertificateElementDataName", "CertElem"!][!//
  [!IF "    node:exists(KeyMCertificateElement/*[    (node:value(KeyMCertificateElementOfStructure) = $CertElem)
                                                 and (./KeyMCertificateElementHasIteration = 'false')
                                                 and not(node:exists(KeyMCertificateElementObjectId))
                                                 and not(node:exists(KeyMCertificateElementObjectType))
                                                ])"!]
&KeyM_CertificateElementData_[!"node:name(KeyMCertificateElement/*[    (node:value(KeyMCertificateElementOfStructure) = $CertElem)
                                                                   and (./KeyMCertificateElementHasIteration = 'false')
                                                                   and not(node:exists(KeyMCertificateElementObjectId))
                                                                   and not(node:exists(KeyMCertificateElementObjectType))
                                                                  ])"!][0],[!//
  [!ELSE!]
  &KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_[!"$CertElem"!],[!//
  [!ENDIF!][!//
[!ENDMACRO!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
/* !LINKSTO KeyM.Dsn.Api/Consts/0005,1 */
CONST(KeyM_CertificateCnfgType, AUTOMATIC) KeyM_CertificateCnfg_[!"node:name(.)"!] =
{
  [!"node:value(KeyMCertificateMaxLength)"!]U, /* certMaxLength */
  [!"node:value(node:ref(./KeyMCertCsmSignatureVerifyJobRef)/CsmJobId)"!]U, /* csmSignatureVerifyJobId */
  [!IF "(node:exists(./KeyMCertCsmSignatureVerifyKeyRef))"!]
  [!"node:value(node:ref(./KeyMCertCsmSignatureVerifyKeyRef)/CsmKeyId)"!]U[!//
  [!ELSE!]
  [!"node:value(node:ref(node:ref(./KeyMCertCsmSignatureVerifyJobRef)/CsmJobKeyRef)/CsmKeyId) "!]U[!//
  [!ENDIF!], /* csmSignatureVerifyKeyId */
  [!"node:value(node:ref(./KeyMCertUpperHierarchicalCertRef)/KeyMCertificateId)"!]U, /* certUpperCertId */
  [!"node:value(node:ref(./KeyMCertStorageCryptoKeyRef)/KeyMCryptoKeyId)"!]U, /* keymStorageKey */
  [!"num:i(string-length(node:value(KeyMCertificateName)))"!]U, /* certNameLength */
  [!IF "node:ref(node:ref(./KeyMCertCsmSignatureVerifyJobRef)/CsmJobPrimitiveRef)/*/*/CsmSignatureVerifyProcessing = 'CSM_ASYNCHRONOUS'"!]
  CRYPTO_PROCESSING_ASYNC[!//
  [!ELSE!]
  CRYPTO_PROCESSING_SYNC[!//
  [!ENDIF!], /* csmSignatureVerifyJobProcessingType */
  KeyM_CertificateName_[!"node:name(.)"!], /* certName */
  [!IF "(node:exists(./KeyMCertificateVerifyCallbackNotificationFunc))"!]
  &[!"node:value(./KeyMCertificateVerifyCallbackNotificationFunc)"!][!//
  [!ELSE!]
  NULL_PTR[!//
  [!ENDIF!], /* keymCbkCertificateVerify */
  [!IF "(node:exists(./KeyMServiceCertificateCallbackNotificationFunc))"!]
  &[!"node:value(./KeyMServiceCertificateCallbackNotificationFunc)"!][!//
  [!ELSE!]
  NULL_PTR[!//
  [!ENDIF!], /* keymCbkServiceCertificate */
  [!"text:toupper(node:path(node:ref(./KeyMCertUpperHierarchicalCertRef)) = node:path(.))"!], /* isSelfSigned */
  [!"num:i(count(./KeyMCertCertificateElementRuleRef/*))"!]U, /* certElementRulesNumber */
  [!IF "num:i(count(./KeyMCertCertificateElementRuleRef/*)) > 0"!]
  KeyM_CertificateElementRules_[!"node:name(.)"!], /* certElementRules */
  [!ELSE!]
  NULL_PTR, /* certElementRules */
  [!ENDIF!]
  [!"num:i(count(./KeyMCertificateElement/*))"!]U, /* certElementsNumber */
  [!IF "num:i(count(./KeyMCertificateElement/*)) > 0"!]
  KeyM_CertificateElements_[!"node:name(.)"!], /* certElements */
  [!ELSE!]
  NULL_PTR, /* certElements */
  [!ENDIF!]
  [!CALL "GetCertificateElementDataName", "CertElem" = "'CertificateIssuerName'"!][!WS!]/* certElementDataIssuer */
  [!CALL "GetCertificateElementDataName", "CertElem" = "'CertificateSubjectName'"!][!WS!]/* certElementDataSubject */
  [!CALL "GetCertificateElementDataName", "CertElem" = "'CertificateSignature'"!][!WS!]/* certElementDataSignatureValue */
  [!CALL "GetCertificateElementDataName", "CertElem" = "'CertificateSubjectPublicKeyInfo_SubjectPublicKey'"!][!WS!]/* certElementDataPublicKey */
  &KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_TbsCertificate, /* certTbsCertificate */
};

[!ENDLOOP!]
/* !LINKSTO KeyM.Dsn.Api/Consts/0004,1 */
CONST(KeyM_CertificateType, AUTOMATIC) KeyM_Certificates[KEYM_CERT_COUNT] =
{
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  {
    &KeyM_CertificateCnfg_[!"node:name(.)"!], /* cnfg */
    &KeyM_CertificateData_[!"node:name(.)"!] /* data */
  },
[!ENDLOOP!]
};

/* !LINKSTO KeyM.Dsn.Api/Consts/0001,1 */
CONST(KeyM_IntConfigType, AUTOMATIC) KeyM_Config =
{
  KeyM_Keys, /* keys */
  KeyM_Certificates /* certificates */
};

[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!VAR "RulesCount" = "num:i(count(./KeyMCertCertificateElementRuleRef/*))"!]
  [!IF "$RulesCount > 0"!]
CONST(KeyM_CertificateElementRuleType, AUTOMATIC) KeyM_CertificateElementRules_[!"node:name(.)"!][[!"$RulesCount"!]] =
{
    [!LOOP "./KeyMCertCertificateElementRuleRef/*"!]
  &KeyM_CertificateElementRule_[!"node:name(node:ref(.)/../..)"!]_[!"node:name(node:ref(.))"!],
    [!ENDLOOP!]
};

  [!ENDIF!]
[!ENDLOOP!]
#define KEYM_STOP_SEC_CONST_UNSPECIFIED
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_CONST_8
#include <KeyM_MemMap.h>

/* !LINKSTO KeyM.Dsn.Api/Consts/0006,1 */
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
CONST(uint8, AUTOMATIC) KeyM_CertificateName_[!"node:name(.)"!][[!"num:i(string-length(node:value(KeyMCertificateName)))"!]] =
{
  "[!"node:value(KeyMCertificateName)"!]"
};

[!ENDLOOP!]
#define KEYM_STOP_SEC_CONST_8
#include <KeyM_MemMap.h>

/*==================[external data]===============================================================*/

#define KEYM_START_SEC_VAR_INIT_8
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
VAR(uint8, KEYM_VAR) KeyM_KeyBuffer_[!"node:name(.)"!][[!"node:value(./KeyMCryptoKeyMaxLength)"!]] = { 0U };

[!ENDLOOP!]
#define KEYM_STOP_SEC_VAR_INIT_8
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

[!LOOP "node:order(KeyMCryptoKey/*, 'node:value(./KeyMCryptoKeyId)')"!]
/* !LINKSTO KeyM.Dsn.Api/Datas/0002,1 */
VAR(KeyM_KeyDataType, KEYM_VAR) KeyM_KeyData_[!"node:name(.)"!] =
{
  [!"node:value(./KeyMCryptoKeyMaxLength)"!]U /* keyLength */
};

[!ENDLOOP!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
/* !LINKSTO KeyM.Dsn.Api/Datas/0003,1 */
VAR(KeyM_CertificateDataType, KEYM_VAR) KeyM_CertificateData_[!"node:name(.)"!] =
{
  FALSE, /* flgServiceRequested */
  FALSE, /* flgParsed */
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00001,3 */
  KEYM_CERTIFICATE_NVM_READ_NOTSET, /* certNvMReadStatus */
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */
  KEYM_CERTIFICATE_NOT_AVAILABLE /* certStatus */
};

[!ENDLOOP!]
[!MACRO "GenerateMandatoryCertificateElementData", "CertElem"!][!//
[!IF "not(node:exists(KeyMCertificateElement/*[    (node:value(KeyMCertificateElementOfStructure) = $CertElem)
                                               and (./KeyMCertificateElementHasIteration = 'false')
                                               and not(node:exists(KeyMCertificateElementObjectId))
                                               and not(node:exists(KeyMCertificateElementObjectType))
                                              ]))"!]
VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_[!"$CertElem"!] =
{
  0U, /* dataLength */
  NULL_PTR /* dataPtr */
};

[!ENDIF!]
[!ENDMACRO!]
[!LOOP "node:order(KeyMCertificate/*, 'node:value(./KeyMCertificateId)')"!]
  [!CALL "GenerateMandatoryCertificateElementData", "CertElem" = "'CertificateIssuerName'"!]
  [!CALL "GenerateMandatoryCertificateElementData", "CertElem" = "'CertificateSubjectName'"!]
  [!CALL "GenerateMandatoryCertificateElementData", "CertElem" = "'CertificateSignature'"!]
  [!CALL "GenerateMandatoryCertificateElementData", "CertElem" = "'CertificateSubjectPublicKeyInfo_SubjectPublicKey'"!]
  [!LOOP "KeyMCertificateElement/*"!]
VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementData_[!"node:name(.)"!][[!CALL "GetCertElementIterations"!]U] =
{
    [!VAR "CertElemIterations"!][!CALL "GetCertElementIterations"!][!ENDVAR!]
    [!LOOP "text:range(1,$CertElemIterations)"!]
  {
    0U, /* dataLength */
    NULL_PTR /* dataPtr */
  },
    [!ENDLOOP!]
};

  [!ENDLOOP!]
VAR(KeyM_CertificateElementDataType, KEYM_VAR) KeyM_CertificateElementDataInternal_[!"node:name(.)"!]_TbsCertificate =
{
  0U, /* dataLength */
  NULL_PTR /* dataPtr */
};

[!ENDLOOP!]
#define KEYM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

/*==================[external function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

[!LOOP "KeyM:GetCertificateVerificationRuleSet(KeyMCertificate/*/KeyMCertCertificateElementRuleRef/*)"!]
/* --- KeyM_CertificateElementRule_[!"node:name(./../..)"!]_[!"node:name(.)"!] [!"substring($MINUS, 1, 100-43-string-length(node:name(./../..))-string-length(node:name(.)))"!]--- */
FUNC(boolean, KEYM_CODE) KeyM_CertificateElementRule_[!"node:name(./../..)"!]_[!"node:name(.)"!]
(
  void
)
{
  [!IF "node:exists(./KeyMLogicalOperator)"!]
    [!IF "    node:value(./KeyMLogicalOperator) = 'KEYM_AND'"!][!VAR "LogicalSymbol" = "'&&'"!][!VAR "LogicalStartValue" = "'TRUE' "!]
    [!ELSEIF "node:value(./KeyMLogicalOperator) = 'KEYM_OR' "!][!VAR "LogicalSymbol" = "'||'"!][!VAR "LogicalStartValue" = "'FALSE'"!]
    [!ELSE!][!ERROR "not supported logical enum"!]
    [!ENDIF!]
  [!ELSE!]
    [!VAR "LogicalSymbol" = "'&&'"!][!VAR "LogicalStartValue" = "'TRUE'"!]
  [!ENDIF!]
  boolean vrfyResult = [!"$LogicalStartValue"!];
  [!LOOP "./KeyMArgumentRef/*"!]
    [!IF "node:exists(node:ref(.)/KeyMArgumentRef)"!]
  vrfyResult = KeyM_CertificateElementRule_[!"node:name(node:ref(.)/../..)"!]_[!"node:name(node:ref(.))"!]() [!"$LogicalSymbol"!] vrfyResult;
    [!ELSE!][!//
  vrfyResult = KeyM_CertificateElementCondition_[!"node:name(node:ref(.)/../..)"!]_[!"node:name(node:ref(.))"!]() [!"$LogicalSymbol"!] vrfyResult;
    [!ENDIF!]
  [!ENDLOOP!]
  return vrfyResult;
}

[!ENDLOOP!]
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

[!LOOP "node:filter(node:refs(KeyM:GetCertificateVerificationRuleSet(KeyMCertificate/*/KeyMCertCertificateElementRuleRef/*)/KeyMArgumentRef/*), 'node:exists(./KeyMCertificateElementConditionValue)')"!]
  [!VAR "CertElemVar"!]KeyM_CertificateElementData_[!"node:name(node:ref(./KeyMCertificateElementRef))"!][!ENDVAR!]
  [!VAR "LogicalOperator"!][!CALL "CompareEnumToSymbol", "CompareEnum" = "node:value(./KeyMCertElementConditionType)"!][!ENDVAR!]
/* --- KeyM_CertificateElementCondition_[!"node:name(./../..)"!]_[!"node:name(.)"!] [!"substring($MINUS, 1, 100-43-string-length(node:name(./../..))-string-length(node:name(.)))"!]--- */
static FUNC(boolean, KEYM_CODE) KeyM_CertificateElementCondition_[!"node:name(./../..)"!]_[!"node:name(.)"!]
(
  void
)
{
  boolean vrfyResult = TRUE;
  uint8 i;

  /* !LINKSTO KeyM.Req.Xdm/KeyMCertificateElementRule/General/00001,1 */
  for (i = 0U; i < KeyM_CertificateElementCnfg_[!"node:name(node:ref(./KeyMCertificateElementRef))"!].maxIterations; i++)
  {
    if (NULL_PTR != [!"$CertElemVar"!][i].dataPtr)
    {
  [!IF "node:exists(./KeyMCertificateElementConditionValue/KeyMCertificateElementConditionPrimitive)"!]
      /* KeyMCertificateElementConditionPrimitive */
      vrfyResult = vrfyResult && (KeyM_CertificateElementData_[!"node:name(node:ref(./KeyMCertificateElementRef))"!][i].dataPtr[0U] [!CALL "CompareEnumToSymbol", "CompareEnum" = "node:value(./KeyMCertElementConditionType)"!][!WS "1"!][!"node:value(./KeyMCertificateElementConditionValue/KeyMCertificateElementConditionPrimitive/KeyMCertificateElementConditionPrimitiveValue)"!]U);
  [!ELSEIF "node:exists(./KeyMCertificateElementConditionValue/KeyMCertificateElementConditionArray)"!]
      /* KeyMCertificateElementConditionArray */
      vrfyResult = (vrfyResult
                    && ([!"$CertElemVar"!][i].dataLength <= KeyM_CertificateElementCnfg_[!"node:name(node:ref(./KeyMCertificateElementRef))"!].maxLength)
    [!LOOP "./KeyMCertificateElementConditionValue/KeyMCertificateElementConditionArray/KeyMCertificateElementConditionArrayElement/*"!]
                    && ([!"node:value(./KeyMCertificateElementConditionArrayElementIndex)"!]U < [!"$CertElemVar"!][i].dataLength)
                    && ([!"$CertElemVar"!][i].dataPtr[[!"node:value(./KeyMCertificateElementConditionArrayElementIndex)"!]U][!WS "1"!][!"$LogicalOperator"!][!WS "1"!][!"node:value(./KeyMCertificateElementConditionArrayElementValue)"!]U)
    [!ENDLOOP!]
                   );
  [!ELSE!]
    [!ERROR "The condition value is not supported."!]
  [!ENDIF!]
    }
  }

  return vrfyResult;
}

[!ENDLOOP!]
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[end of file]=================================================================*/

