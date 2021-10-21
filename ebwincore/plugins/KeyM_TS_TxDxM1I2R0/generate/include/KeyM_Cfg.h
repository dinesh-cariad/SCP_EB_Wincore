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
#ifndef KEYM_CFG_H
#define KEYM_CFG_H

/* !LINKSTO KeyM.Dsn.Files/KeyM_Cfg.h/0001,1 */

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

#include <KeyM_Types.h>

/*==================[macros]======================================================================*/

/* --- General ---------------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.Api/Macros/0001,1 */
#if (defined KEYM_API_ENABLED_DEVERRORDETECT)
#error KEYM_API_ENABLED_DEVERRORDETECT is already defined
#endif
/** \brief  Development Error detect enabled/disabled info. **/
#define KEYM_API_ENABLED_DEVERRORDETECT [!WS "1"!][!IF "$KeyM_Api_Enabled_DevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0002,1 */
#if (defined KEYM_API_ENABLED_START)
#error KEYM_API_ENABLED_START is already defined
#endif
/** \brief  KeyM_Start() API enabled/disabled info. **/
#define KEYM_API_ENABLED_START [!WS "1"!][!IF "$KeyM_Api_Enabled_Start = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0003,1 */
#if (defined KEYM_API_ENABLED_PREPARE)
#error KEYM_API_ENABLED_PREPARE is already defined
#endif
/** \brief  KeyM_Prepare() API enabled/disabled info. **/
#define KEYM_API_ENABLED_PREPARE [!WS "1"!][!IF "$KeyM_Api_Enabled_Prepare = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0004,1 */
#if (defined KEYM_API_ENABLED_UPDATE)
#error KEYM_API_ENABLED_UPDATE is already defined
#endif
/** \brief  KeyM_Update() API enabled/disabled info. **/
#define KEYM_API_ENABLED_UPDATE [!WS "1"!][!IF "$KeyM_Api_Enabled_Update = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0005,1 */
#if (defined KEYM_API_ENABLED_FINALIZE)
#error KEYM_API_ENABLED_FINALIZE is already defined
#endif
/** \brief  KeyM_Finalize() API enabled/disabled info. **/
#define KEYM_API_ENABLED_FINALIZE [!WS "1"!][!IF "$KeyM_Api_Enabled_Finalize = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0006,1 */
#if (defined KEYM_API_ENABLED_VERIFY)
#error KEYM_API_ENABLED_VERIFY is already defined
#endif
/** \brief  KeyM_Verify() API enabled/disabled info. **/
#define KEYM_API_ENABLED_VERIFY [!WS "1"!][!IF "$KeyM_Api_Enabled_Verify = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0007,1 */
#if (defined KEYM_API_ENABLED_SERVICECERTIFICATE)
#error KEYM_API_ENABLED_SERVICECERTIFICATE is already defined
#endif
/** \brief  KeyM_ServiceCertificate() API enabled/disabled info. **/
#define KEYM_API_ENABLED_SERVICECERTIFICATE [!WS "1"!][!IF "$KeyM_Api_Enabled_ServiceCertificate = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Api/Macros/0011,1 */
#if (defined KEYM_API_ENABLED_NVM)
#error KEYM_API_ENABLED_NVM is already defined
#endif
/** \brief  NvM API usage enabled/disabled info. **/
#define KEYM_API_ENABLED_NVM [!WS "1"!][!IF "$KeyM_Api_Enabled_NvM = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* !LINKSTO KeyM.Dsn.Alg/KeyM_General/00001,2 */
#if (defined KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION)
#error KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION is already defined
#endif
/** \brief Enables (TRUE) or disables (FALSE) execution of background processing in the KeyM_MainBackgroundFunction instead of the default KeyM_MainFunction. **/
#define KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION [!WS "1"!][!IF "$KeyM_Api_Enabled_Background = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]

/* --- Certificates ----------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.Api/Macros/0010,1 */
#if (defined KEYM_CERT_MAX_DEPTH_CHAIN)
#error KEYM_CERT_MAX_DEPTH_CHAIN is already defined
#endif
/** \brief  The configured max depth of certificate chains. **/
#define KEYM_CERT_MAX_DEPTH_CHAIN [!"./KeyMGeneral/KeyMCertificateChainMaxDepth"!]U

[!VAR "MAXLEN" = "num:i(1)"!]
[!LOOP "./KeyMCertificate/*"!]
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!]
[!ENDLOOP!]
[!LOOP "./KeyMCertificate/*"!]
/* !LINKSTO KeyM.Dsn.Api/Macros/0013,1 */
#if (defined KeyMConf_KeyMCertificate_[!"node:name(.)"!])
#error KeyMConf_KeyMCertificate_[!"node:name(.)"!] is already defined
#endif
/** \brief  The symbolic name identifier for KeyM certificate 'KeyMConf_KeyMCertificate_[!"node:name(.)"!]'. **/
#define KeyMConf_KeyMCertificate_[!"node:name(.)"!] [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!"node:value(KeyMCertificateId)"!]U

[!ENDLOOP!]
/* !LINKSTO KeyM.Dsn.Api/Macros/0009,1 */
#if (defined KEYM_CERT_COUNT)
#error KEYM_CERT_COUNT is already defined
#endif
/** \brief  The number of configured KeyM certificates. **/
#define KEYM_CERT_COUNT [!"num:i(count(KeyMCertificate/*))"!]U

/* --- Certificate elements --------------------------------------------------------------------- */

[!VAR "MAXLEN" = "num:i(1)"!]
[!LOOP "./KeyMCertificate/*/KeyMCertificateElement/*"!]
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!]
[!ENDLOOP!]
[!LOOP "./KeyMCertificate/*/KeyMCertificateElement/*"!]
#if (defined KeyMConf_KeyMCertificateElement_[!"node:name(.)"!])
#error KeyMConf_KeyMCertificateElement_[!"node:name(.)"!] is already defined
#endif
/** \brief  The symbolic name identifier for KeyM certificate element 'KeyMConf_KeyMCertificateElement_[!"node:name(.)"!]'. **/
#define KeyMConf_KeyMCertificateElement_[!"node:name(.)"!] [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!"node:value(KeyMCertificateElementId)"!]U

[!ENDLOOP!]
/* --- Keys ------------------------------------------------------------------------------------- */

[!VAR "MAXLEN" = "num:i(1)"!]
[!LOOP "./KeyMCryptoKey/*"!]
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!]
[!ENDLOOP!]
[!LOOP "./KeyMCryptoKey/*"!]
/* !LINKSTO KeyM.Dsn.Api/Macros/0012,1 */
#if (defined KeyMConf_KeyMCryptoKey_[!"node:name(.)"!])
#error KeyMConf_KeyMCryptoKey_[!"node:name(.)"!] is already defined
#endif
/** \brief  The symbolic name identifier for KeyM crypto key 'KeyMConf_KeyMCryptoKey_[!"node:name(.)"!]'. **/
#define KeyMConf_KeyMCryptoKey_[!"node:name(.)"!] [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!"node:value(KeyMCryptoKeyId)"!]U

[!ENDLOOP!]
/* !LINKSTO KeyM.Dsn.Api/Macros/0008,1 */
#if (defined KEYM_KEY_COUNT)
#error KEYM_KEY_COUNT is already defined
#endif
/** \brief  The number of configured KeyM crypto keys. **/
#define KEYM_KEY_COUNT [!"num:i(count(KeyMCryptoKey/*))"!]U

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

/*================================================================================================*/

#endif /* #ifndef KEYM_CFG_H */

/*==================[end of file]=================================================================*/

