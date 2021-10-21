[!/**
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
 */!][!//
[!//
[!AUTOSPACING!][!//
[!//
[!/* *** multiple inclusion protection *** */!][!//
[!IF "not(var:defined('KEYM_MACROS_M'))"!][!//
[!VAR "KEYM_MACROS_M" = "'true'"!][!//
[!//
[!//
[!//
[!VAR "SPACE"                               = "'                                                                      '"!][!//
[!VAR "MINUS"                               = "'----------------------------------------------------------------------'"!][!//
[!//
[!VAR "KeyM_Api_Enabled_DevErrorDetect"     = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMDevErrorDetect)           "!][!//
[!VAR "KeyM_Api_Enabled_KeyManager"         = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyManagerEnabled)  "!][!//
[!VAR "KeyM_Api_Enabled_CertificateManager" = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCertificateManagerEnabled)"!][!//
[!VAR "KeyM_Api_Enabled_Background"         = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMBackgroundEnabled)        "!][!//
[!//
[!VAR "KeyM_Api_Enabled_Start"              = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_Prepare"            = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyPrepareFunctionEnabled)       = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyPrepareFunctionEnabled)       = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_Update"             = "$KeyM_Api_Enabled_KeyManager         = 'true'                                                                                                                                                                                                     "!][!//
[!VAR "KeyM_Api_Enabled_Finalize"           = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_Verify"             = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyVerifyFunctionEnabled)        = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyVerifyFunctionEnabled)        = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_ServiceCertificate" = "$KeyM_Api_Enabled_CertificateManager = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMServiceCertificateFunctionEnabled)     = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMServiceCertificateFunctionEnabled)     = 'true'"!][!//
[!//
[!VAR "KeyM_Api_Enabled_NvM" = "'false'"!][!//
[!LOOP "KeyMCryptoKey/*"!][!//
[!IF "'KEYM_STORAGE_IN_NVM' = node:value(./KeyMCryptoKeyStorage)"!][!//
[!VAR "KeyM_Api_Enabled_NvM" = "'true'"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!//
[!//
[!INDENT "0"!][!//
[!MACRO "CompareEnumToSymbol", "CompareEnum"!][!//
  [!IF "    $CompareEnum = 'KEYM_EQUALS'          "!]==[!//
  [!ELSEIF "$CompareEnum = 'KEYM_EQUALS_NOT'      "!]!=[!//
  [!ELSEIF "$CompareEnum = 'KEYM_GREATER_OR_EQUAL'"!]>=[!//
  [!ELSEIF "$CompareEnum = 'KEYM_LESS_OR_EQUAL'   "!]<=[!//
  [!ELSEIF "$CompareEnum = 'KEYM_LESS_THAN'       "!]<[!//
  [!ELSE!][!//
    [!ERROR "not supported compare enum"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!ENDINDENT!][!//
[!//
[!MACRO "StructureEnumToDefine", "Structure"!][!//
  [!IF "    $Structure = 'CertificateExtension'                               "!]KEYM_STRUCTURE_CERTIFICATE_EXTENSION[!//
  [!ELSEIF "$Structure = 'CertificateIssuerName'                              "!]KEYM_STRUCTURE_CERTIFICATE_ISSUER_NAME[!//
  [!ELSEIF "$Structure = 'CertificateIssuerUniqueIdentifier'                  "!]KEYM_STRUCTURE_CERTIFICATE_ISSUER_UNIQUE_NAME[!//
  [!ELSEIF "$Structure = 'CertificateSerialNumber'                            "!]KEYM_STRUCTURE_CERTIFICATE_SERIAL_NUMBER[!//
  [!ELSEIF "$Structure = 'CertificateSignature'                               "!]KEYM_STRUCTURE_CERTIFICATE_SIGNATURE[!//
  [!ELSEIF "$Structure = 'CertificateSignatureAlgorithm'                      "!]KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM[!//
  [!ELSEIF "$Structure = 'CertificateSignatureAlgorithmID'                    "!]KEYM_STRUCTURE_CERTIFICATE_SIGNATURE_ALGORITHM_ID[!//
  [!ELSEIF "$Structure = 'CertificateSubjectName'                             "!]KEYM_STRUCTURE_CERTIFICATE_SUBJECT_NAME[!//
  [!ELSEIF "$Structure = 'CertificateSubjectPublicKeyInfo_PublicKeyAlgorithm' "!]KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM[!//
  [!ELSEIF "$Structure = 'CertificateSubjectPublicKeyInfo_SubjectPublicKey'   "!]KEYM_STRUCTURE_CERTIFICATE_SUBJECT_PUBLICKEY_INFO_SUBJECT_PUBLICKEY[!//
  [!ELSEIF "$Structure = 'CertificateSubjectUniqueIdentifier'                 "!]KEYM_STRUCTURE_CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER[!//
  [!ELSEIF "$Structure = 'CertificateValidityPeriodNotAfter'                  "!]KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER[!//
  [!ELSEIF "$Structure = 'CertificateValidityPeriodNotBefore'                 "!]KEYM_STRUCTURE_CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE[!//
  [!ELSEIF "$Structure = 'CertificateVersionNumber'                           "!]KEYM_STRUCTURE_CERTIFICATE_VERSION_NUMBER[!//
  [!ELSE!][!//
    [!ERROR "Unsupported structure"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!MACRO "FormatArray", "Array", "Indentation"!][!//
[!INDENT "$Indentation"!][!//
  [!CR!]{[!CR!][!//
  [!VAR "SplitLine" = "0"!][!//
  [!VAR "i" = "0"!][!//
  [!LOOP "text:split(text:replaceAll($Array, '\s+', ''), ',')"!][!//
    [!IF "num:i($SplitLine) = 0"!][!//
    [!WS "2"!][!//
    [!ENDIF!][!//
    [!VAR "i" = "$i + 1"!][!//
    [!VAR "SplitLine" = "$SplitLine + 1"!][!//
    [!"."!]U[!//
    [!IF "num:i($i) != count(text:split(text:replaceAll($Array, '\s+', ''), ','))"!][!//
      ,[!//
      [!IF "num:i($SplitLine) = 8"!][!//
        [!VAR "SplitLine" = "0"!][!//
        [!CR!][!//
      [!ELSEIF "num:i($SplitLine) != 0"!][!//
        [!WS!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!CR!]}[!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!MACRO "GetCertElementIterations"!][!//
[!INDENT "0"!][!//
  [!IF "node:value(./KeyMCertificateElementHasIteration)"!][!//
[!"node:value(./KeyMCertificateElementMaxIterations)"!][!//
  [!ELSE!][!//
1[!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!//
[!ENDIF!][!//
