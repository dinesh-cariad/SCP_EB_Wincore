[!/**
 * \file
 *
 * \brief AUTOSAR Csm
 *
 * This file contains the implementation of the AUTOSAR
 * module Csm.
 *
 * \version 3.1.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!//
[!INDENT "0"!][!//
[!//
[!/* *** multiple inclusion protection *** */!][!//
[!IF "not(var:defined('CSM_MACROS_M'))"!][!//
[!VAR "CSM_MACROS_M" = "'true'"!][!//
[!//
[!//
[!//
[!VAR "SPACE" = "'                                                                          '"!][!//
[!//
[!VAR "Csm_Api_Enabled_KeyMngmnt"         = "string(boolean(num:i(count(as:modconf('Csm')/CsmKeys/CsmKey/*))))"!][!//
[!VAR "Csm_Api_Enabled_Callback"          = "string(boolean(num:i(count(as:modconf('Csm')/CsmCallbacks/CsmCallback/*[not(node:empty(./CsmCallbackFunc))]))))"!][!//
[!//
[!VAR "Csm_Api_Enabled_AEADDecrypt"       = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmAEADDecrypt))      "!][!//
[!VAR "Csm_Api_Enabled_AEADEncrypt"       = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmAEADEncrypt))      "!][!//
[!VAR "Csm_Api_Enabled_Decrypt"           = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmDecrypt))          "!][!//
[!VAR "Csm_Api_Enabled_Encrypt"           = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmEncrypt))          "!][!//
[!VAR "Csm_Api_Enabled_Hash"              = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmHash))             "!][!//
[!VAR "Csm_Api_Enabled_MacGenerate"       = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmMacGenerate))      "!][!//
[!VAR "Csm_Api_Enabled_MacVerify"         = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmMacVerify))        "!][!//
[!VAR "Csm_Api_Enabled_RandomGenerate"    = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmRandomGenerate))   "!][!//
[!VAR "Csm_Api_Enabled_SignatureGenerate" = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureGenerate))"!][!//
[!VAR "Csm_Api_Enabled_SignatureVerify"   = "string(node:exists(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureVerify))  "!][!//
[!//
[!VAR "Csm_Rte_Enabled_KeyMngmnt"         = "string(boolean(num:i(count(as:modconf('Csm')/CsmKeys/CsmKey/*[CsmKeyUsePort = 'true']))))"!][!//
[!VAR "Csm_Rte_Enabled_Callback"          = "string(boolean(num:i(count(as:modconf('Csm')/CsmCallbacks/CsmCallback/*[node:empty(./CsmCallbackFunc)]))))"!][!//
[!//
[!VAR "Csm_Rte_Enabled_AEADDecrypt"       = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmAEADDecrypt']))))      "!][!//
[!VAR "Csm_Rte_Enabled_AEADEncrypt"       = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmAEADEncrypt']))))      "!][!//
[!VAR "Csm_Rte_Enabled_Decrypt"           = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmDecrypt']))))          "!][!//
[!VAR "Csm_Rte_Enabled_Encrypt"           = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmEncrypt']))))          "!][!//
[!VAR "Csm_Rte_Enabled_Hash"              = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmHash']))))             "!][!//
[!VAR "Csm_Rte_Enabled_MacGenerate"       = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmMacGenerate']))))      "!][!//
[!VAR "Csm_Rte_Enabled_MacVerify"         = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmMacVerify']))))        "!][!//
[!VAR "Csm_Rte_Enabled_RandomGenerate"    = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmRandomGenerate']))))   "!][!//
[!VAR "Csm_Rte_Enabled_SignatureGenerate" = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmSignatureGenerate']))))"!][!//
[!VAR "Csm_Rte_Enabled_SignatureVerify"   = "string(boolean(num:i(count(node:refs(as:modconf('Csm')/CsmJobs/CsmJob/*[CsmJobUsePort = 'true']/CsmJobPrimitiveRef)/*[@name = 'CsmSignatureVerify']))))  "!][!//
[!//
[!VAR "Csm_Flg_Sync_AEADDecrypt"          = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmAEADDecrypt/CsmAEADDecryptConfig[CsmAEADDecryptProcessing = 'CSM_SYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Sync_AEADEncrypt"          = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmAEADEncrypt/CsmAEADEncryptConfig[CsmAEADEncryptProcessing = 'CSM_SYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Sync_Decrypt"              = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmDecrypt/CsmDecryptConfig[CsmDecryptProcessing = 'CSM_SYNCHRONOUS']))))                              "!][!//
[!VAR "Csm_Flg_Sync_Encrypt"              = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmEncrypt/CsmEncryptConfig[CsmEncryptProcessing = 'CSM_SYNCHRONOUS']))))                              "!][!//
[!VAR "Csm_Flg_Sync_Hash"                 = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmHash/CsmHashConfig[CsmHashProcessing = 'CSM_SYNCHRONOUS']))))                                       "!][!//
[!VAR "Csm_Flg_Sync_MacGenerate"          = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmMacGenerate/CsmMacGenerateConfig[CsmMacGenerateProcessing = 'CSM_SYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Sync_MacVerify"            = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmMacVerify/CsmMacVerifyConfig[CsmMacVerifyProcessing = 'CSM_SYNCHRONOUS']))))                        "!][!//
[!VAR "Csm_Flg_Sync_RandomGenerate"       = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmRandomGenerate/CsmRandomGenerateConfig[CsmRandomGenerateProcessing = 'CSM_SYNCHRONOUS']))))         "!][!//
[!VAR "Csm_Flg_Sync_SignatureGenerate"    = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureGenerate/CsmSignatureGenerateConfig[CsmSignatureGenerateProcessing = 'CSM_SYNCHRONOUS']))))"!][!//
[!VAR "Csm_Flg_Sync_SignatureVerify"      = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureVerify/CsmSignatureVerifyConfig[CsmSignatureVerifyProcessing = 'CSM_SYNCHRONOUS']))))      "!][!//
[!//
[!VAR "Csm_Flg_Async_AEADDecrypt"         = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmAEADDecrypt/CsmAEADDecryptConfig[CsmAEADDecryptProcessing = 'CSM_ASYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Async_AEADEncrypt"         = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmAEADEncrypt/CsmAEADEncryptConfig[CsmAEADEncryptProcessing = 'CSM_ASYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Async_Decrypt"             = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmDecrypt/CsmDecryptConfig[CsmDecryptProcessing = 'CSM_ASYNCHRONOUS']))))                              "!][!//
[!VAR "Csm_Flg_Async_Encrypt"             = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmEncrypt/CsmEncryptConfig[CsmEncryptProcessing = 'CSM_ASYNCHRONOUS']))))                              "!][!//
[!VAR "Csm_Flg_Async_Hash"                = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmHash/CsmHashConfig[CsmHashProcessing = 'CSM_ASYNCHRONOUS']))))                                       "!][!//
[!VAR "Csm_Flg_Async_MacGenerate"         = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmMacGenerate/CsmMacGenerateConfig[CsmMacGenerateProcessing = 'CSM_ASYNCHRONOUS']))))                  "!][!//
[!VAR "Csm_Flg_Async_MacVerify"           = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmMacVerify/CsmMacVerifyConfig[CsmMacVerifyProcessing = 'CSM_ASYNCHRONOUS']))))                        "!][!//
[!VAR "Csm_Flg_Async_RandomGenerate"      = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmRandomGenerate/CsmRandomGenerateConfig[CsmRandomGenerateProcessing = 'CSM_ASYNCHRONOUS']))))         "!][!//
[!VAR "Csm_Flg_Async_SignatureGenerate"   = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureGenerate/CsmSignatureGenerateConfig[CsmSignatureGenerateProcessing = 'CSM_ASYNCHRONOUS']))))"!][!//
[!VAR "Csm_Flg_Async_SignatureVerify"     = "string(boolean(num:i(count(as:modconf('Csm')/CsmPrimitives/*/CsmSignatureVerify/CsmSignatureVerifyConfig[CsmSignatureVerifyProcessing = 'CSM_ASYNCHRONOUS']))))      "!][!//
[!//
[!//
[!//
[!VAR "Csm_Api_Enabled_All"                   = "'false'"!][!//
[!VAR "Csm_Rte_Enabled_All"                   = "'false'"!][!//
[!VAR "Csm_Api_Enabled_Srv"                   = "'false'"!][!//
[!VAR "Csm_Rte_Enabled_Srv"                   = "'false'"!][!//
[!VAR "Csm_Flg_Sync"                          = "'false'"!][!//
[!VAR "Csm_Flg_Async"                         = "'false'"!][!//
[!//
[!//
[!//
[!IF "$Csm_Api_Enabled_KeyMngmnt         = 'true' or
      $Csm_Api_Enabled_Callback          = 'true' or
      $Csm_Api_Enabled_AEADDecrypt       = 'true' or
      $Csm_Api_Enabled_AEADEncrypt       = 'true' or
      $Csm_Api_Enabled_Decrypt           = 'true' or
      $Csm_Api_Enabled_Encrypt           = 'true' or
      $Csm_Api_Enabled_Hash              = 'true' or
      $Csm_Api_Enabled_MacGenerate       = 'true' or
      $Csm_Api_Enabled_MacVerify         = 'true' or
      $Csm_Api_Enabled_RandomGenerate    = 'true' or
      $Csm_Api_Enabled_SignatureGenerate = 'true' or
      $Csm_Api_Enabled_SignatureVerify   = 'true'    "!][!//
  [!VAR "Csm_Api_Enabled_All" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!IF "$Csm_Rte_Enabled_KeyMngmnt         = 'true' or
      $Csm_Rte_Enabled_Callback          = 'true' or
      $Csm_Rte_Enabled_AEADDecrypt       = 'true' or
      $Csm_Rte_Enabled_AEADEncrypt       = 'true' or
      $Csm_Rte_Enabled_Decrypt           = 'true' or
      $Csm_Rte_Enabled_Encrypt           = 'true' or
      $Csm_Rte_Enabled_Hash              = 'true' or
      $Csm_Rte_Enabled_MacGenerate       = 'true' or
      $Csm_Rte_Enabled_MacVerify         = 'true' or
      $Csm_Rte_Enabled_RandomGenerate    = 'true' or
      $Csm_Rte_Enabled_SignatureGenerate = 'true' or
      $Csm_Rte_Enabled_SignatureVerify   = 'true'    "!][!//
  [!VAR "Csm_Rte_Enabled_All" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!IF "$Csm_Api_Enabled_AEADDecrypt       = 'true' or
      $Csm_Api_Enabled_AEADEncrypt       = 'true' or
      $Csm_Api_Enabled_Decrypt           = 'true' or
      $Csm_Api_Enabled_Encrypt           = 'true' or
      $Csm_Api_Enabled_Hash              = 'true' or
      $Csm_Api_Enabled_MacGenerate       = 'true' or
      $Csm_Api_Enabled_MacVerify         = 'true' or
      $Csm_Api_Enabled_RandomGenerate    = 'true' or
      $Csm_Api_Enabled_SignatureGenerate = 'true' or
      $Csm_Api_Enabled_SignatureVerify   = 'true'    "!][!//
  [!VAR "Csm_Api_Enabled_Srv" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!IF "$Csm_Rte_Enabled_AEADDecrypt       = 'true' or
      $Csm_Rte_Enabled_AEADEncrypt       = 'true' or
      $Csm_Rte_Enabled_Decrypt           = 'true' or
      $Csm_Rte_Enabled_Encrypt           = 'true' or
      $Csm_Rte_Enabled_Hash              = 'true' or
      $Csm_Rte_Enabled_MacGenerate       = 'true' or
      $Csm_Rte_Enabled_MacVerify         = 'true' or
      $Csm_Rte_Enabled_SignatureGenerate = 'true' or
      $Csm_Rte_Enabled_SignatureVerify   = 'true'    "!][!//
[!/* *** Csm_Rte_Enabled_RandomGenerate is not included here because the corresponding 
Client-Server-Interface does not define the operation CancelJob*** */!][!//
  [!VAR "Csm_Rte_Enabled_Srv" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!IF "$Csm_Flg_Sync_AEADDecrypt       = 'true' or
      $Csm_Flg_Sync_AEADEncrypt       = 'true' or
      $Csm_Flg_Sync_Decrypt           = 'true' or
      $Csm_Flg_Sync_Encrypt           = 'true' or
      $Csm_Flg_Sync_Hash              = 'true' or
      $Csm_Flg_Sync_MacGenerate       = 'true' or
      $Csm_Flg_Sync_MacVerify         = 'true' or
      $Csm_Flg_Sync_RandomGenerate    = 'true' or
      $Csm_Flg_Sync_SignatureGenerate = 'true' or
      $Csm_Flg_Sync_SignatureVerify   = 'true'    "!][!//
  [!VAR "Csm_Flg_Sync" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!IF "$Csm_Flg_Async_AEADDecrypt       = 'true' or
      $Csm_Flg_Async_AEADEncrypt       = 'true' or
      $Csm_Flg_Async_Decrypt           = 'true' or
      $Csm_Flg_Async_Encrypt           = 'true' or
      $Csm_Flg_Async_Hash              = 'true' or
      $Csm_Flg_Async_MacGenerate       = 'true' or
      $Csm_Flg_Async_MacVerify         = 'true' or
      $Csm_Flg_Async_RandomGenerate    = 'true' or
      $Csm_Flg_Async_SignatureGenerate = 'true' or
      $Csm_Flg_Async_SignatureVerify   = 'true'    "!][!//
  [!VAR "Csm_Flg_Async" = "'true'"!][!//
[!ENDIF!][!//
[!//
[!//
[!//
[!/*
****************************************************************************************************
* MACRO to get a upper case hexadecimal representation of _value with _digit digits.
****************************************************************************************************
*/!][!//
[!MACRO "HEX", "_value" = "num:i(0)", "_digits" = "num:i(8)"!][!//
[!"text:replace(text:toupper(num:inttohex($_value, $_digits)), '0X', '0x')"!][!//
[!ENDMACRO!][!//
[!//
[!//
[!//
[!ENDIF!][!//
[!//
[!ENDINDENT!][!//
[!//
