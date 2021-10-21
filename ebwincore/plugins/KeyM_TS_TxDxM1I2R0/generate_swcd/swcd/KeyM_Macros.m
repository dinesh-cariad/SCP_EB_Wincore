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
[!//
[!VAR "KeyM_Api_Enabled_DevErrorDetect"     = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMDevErrorDetect)           "!][!//
[!VAR "KeyM_Api_Enabled_KeyManager"         = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyManagerEnabled)  "!][!//
[!VAR "KeyM_Api_Enabled_CertificateManager" = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCertificateManagerEnabled)"!][!//
[!VAR "KeyM_Api_Enabled_Background"         = "node:value(as:modconf('KeyM')/KeyMGeneral/KeyMBackgroundEnabled)        "!][!//
[!//
[!VAR "KeyM_Api_Enabled_Start"              = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyStartFinalizeFunctionEnabled) = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_Prepare"            = "$KeyM_Api_Enabled_KeyManager         = 'true' and node:exists(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyPrepareFunctionEnabled)       = 'true' and node:value(as:modconf('KeyM')/KeyMGeneral/KeyMCryptoKeyPrepareFunctionEnabled)       = 'true'"!][!//
[!VAR "KeyM_Api_Enabled_Update"             = "$KeyM_Api_Enabled_KeyManager         = 'true'                                                                                                  "!][!//
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
[!ENDIF!][!//
