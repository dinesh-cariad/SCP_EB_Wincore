[!/* ====================================================================================== */!][!//
[!INCLUDE "Csm_Macros.m"!][!//
[!/* ====================================================================================== */!][!//
[!//
/**
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
 */

#ifndef CSM_CFG_H
#define CSM_CFG_H

/* !LINKSTO CSM.Dsn.SWS_Csm_00003,1 */

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm_Types.h>

/*==================[macros]======================================================================*/

/** \brief  Csm instance id
 **/
#if (defined CSM_INSTANCE_ID)
  #error CSM_INSTANCE_ID is already defined
#endif
#define  CSM_INSTANCE_ID  0x00U

/** \brief  General APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00001,1 */
#if (defined CSM_API_ENABLED_VERSIONINFO)
  #error CSM_API_ENABLED_VERSIONINFO is already defined
#endif
#define  CSM_API_ENABLED_VERSIONINFO                [!IF "node:value(as:modconf('Csm')/CsmGeneral/CsmVersionInfoApi) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Development Error detect enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00002,1 */
#if (defined CSM_API_ENABLED_DEVERRORDETECT)
  #error CSM_API_ENABLED_DEVERRORDETECT is already defined
#endif
#define  CSM_API_ENABLED_DEVERRORDETECT             [!IF "node:value(as:modconf('Csm')/CsmGeneral/CsmDevErrorDetect) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Deprecated APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00003,1 */
#if (defined CSM_API_ENABLED_USEDEPRECATED)
  #error CSM_API_ENABLED_USEDEPRECATED is already defined
#endif
#define  CSM_API_ENABLED_USEDEPRECATED              [!IF "node:value(as:modconf('Csm')/CsmGeneral/CsmUseDeprecated)  = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Key management APIs enabled/disabled infos
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00004,1 */
#if (defined CSM_API_ENABLED_KEYMNGMNT)
  #error CSM_API_ENABLED_KEYMNGMNT is already defined
#endif
#define  CSM_API_ENABLED_KEYMNGMNT                  [!IF "$Csm_Api_Enabled_KeyMngmnt         = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  General services interfaces enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00005,1 */
#if (defined CSM_API_ENABLED_SERVICE_GENERAL)
  #error CSM_API_ENABLED_SERVICE_GENERAL is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_GENERAL            [!IF "$Csm_Api_Enabled_Srv               = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Asynchronous service interfaces enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00006,1 */
#if (defined CSM_API_ENABLED_SERVICE_ASYNCHRONOUS)
  #error CSM_API_ENABLED_SERVICE_ASYNCHRONOUS is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_ASYNCHRONOUS       [!IF "$Csm_Flg_Async                     = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Synchronous service interfaces enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00007,1 */
#if (defined CSM_API_ENABLED_SERVICE_SYNCHRONOUS)
  #error CSM_API_ENABLED_SERVICE_SYNCHRONOUS is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_SYNCHRONOUS        [!IF "$Csm_Flg_Sync                      = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service AEADDecrypt APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_AEADDECRYPT)
  #error CSM_API_ENABLED_SERVICE_AEADDECRYPT is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_AEADDECRYPT        [!IF "$Csm_Api_Enabled_AEADDecrypt       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service AEADEncrypt APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_AEADENCRYPT)
  #error CSM_API_ENABLED_SERVICE_AEADENCRYPT is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_AEADENCRYPT        [!IF "$Csm_Api_Enabled_AEADEncrypt       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Decrypt APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_DECRYPT)
  #error CSM_API_ENABLED_SERVICE_DECRYPT is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_DECRYPT            [!IF "$Csm_Api_Enabled_Decrypt           = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Encrypt APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_ENCRYPT)
  #error CSM_API_ENABLED_SERVICE_ENCRYPT is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_ENCRYPT            [!IF "$Csm_Api_Enabled_Encrypt           = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Hash APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_HASH)
  #error CSM_API_ENABLED_SERVICE_HASH is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_HASH               [!IF "$Csm_Api_Enabled_Hash              = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service MacGenerate APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_MACGENERATE)
  #error CSM_API_ENABLED_SERVICE_MACGENERATE is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_MACGENERATE        [!IF "$Csm_Api_Enabled_MacGenerate       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service MacVerify APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_MACVERIFY)
  #error CSM_API_ENABLED_SERVICE_MACVERIFY is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_MACVERIFY          [!IF "$Csm_Api_Enabled_MacVerify         = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service RandomGenerate APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_RANDOMGENERATE)
  #error CSM_API_ENABLED_SERVICE_RANDOMGENERATE is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_RANDOMGENERATE     [!IF "$Csm_Api_Enabled_RandomGenerate    = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service SignatureGenerate APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE)
  #error CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE  [!IF "$Csm_Api_Enabled_SignatureGenerate = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service SignatureVerify APIs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00008,1 */
#if (defined CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY)
  #error CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY is already defined
#endif
#define  CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY    [!IF "$Csm_Api_Enabled_SignatureVerify   = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Key management RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00009,1 */
#if (defined CSM_RTE_ENABLED_KEYMNGMNT)
  #error CSM_RTE_ENABLED_KEYMNGMNT is already defined
#endif
#define  CSM_RTE_ENABLED_KEYMNGMNT                  [!IF "$Csm_Rte_Enabled_KeyMngmnt         = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  General services RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00010,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_GENERAL)
  #error CSM_RTE_ENABLED_SERVICE_GENERAL is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_GENERAL            [!IF "$Csm_Rte_Enabled_Srv               = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service AEADDecrypt RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_AEADDECRYPT)
  #error CSM_RTE_ENABLED_SERVICE_AEADDECRYPT is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_AEADDECRYPT        [!IF "$Csm_Rte_Enabled_AEADDecrypt       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service AEADEncrypt RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_AEADENCRYPT)
  #error CSM_RTE_ENABLED_SERVICE_AEADENCRYPT is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_AEADENCRYPT        [!IF "$Csm_Rte_Enabled_AEADEncrypt       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Decrypt RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_DECRYPT)
  #error CSM_RTE_ENABLED_SERVICE_DECRYPT is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_DECRYPT            [!IF "$Csm_Rte_Enabled_Decrypt           = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Encrypt RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_ENCRYPT)
  #error CSM_RTE_ENABLED_SERVICE_ENCRYPT is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_ENCRYPT            [!IF "$Csm_Rte_Enabled_Encrypt           = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service Hash RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_HASH)
  #error CSM_RTE_ENABLED_SERVICE_HASH is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_HASH               [!IF "$Csm_Rte_Enabled_Hash              = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service MacGenerate RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_MACGENERATE)
  #error CSM_RTE_ENABLED_SERVICE_MACGENERATE is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_MACGENERATE        [!IF "$Csm_Rte_Enabled_MacGenerate       = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service MacVerify RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_MACVERIFY)
  #error CSM_RTE_ENABLED_SERVICE_MACVERIFY is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_MACVERIFY          [!IF "$Csm_Rte_Enabled_MacVerify         = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service RandomGenerate RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_RANDOMGENERATE)
  #error CSM_RTE_ENABLED_SERVICE_RANDOMGENERATE is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_RANDOMGENERATE     [!IF "$Csm_Rte_Enabled_RandomGenerate    = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service SignatureGenerate RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_SIGNATUREGENERATE)
  #error CSM_RTE_ENABLED_SERVICE_SIGNATUREGENERATE is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_SIGNATUREGENERATE  [!IF "$Csm_Rte_Enabled_SignatureGenerate = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  Service SignatureVerify RTEs enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00011,1 */
#if (defined CSM_RTE_ENABLED_SERVICE_SIGNATUREVERIFY)
  #error CSM_RTE_ENABLED_SERVICE_SIGNATUREVERIFY is already defined
#endif
#define  CSM_RTE_ENABLED_SERVICE_SIGNATUREVERIFY    [!IF "$Csm_Rte_Enabled_SignatureVerify   = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  General RTE enabled/disabled info
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00012,1 */
#if (defined CSM_RTE_ENABLED)
  #error CSM_RTE_ENABLED is already defined
#endif
#define  CSM_RTE_ENABLED                            [!IF "$Csm_Rte_Enabled_All               = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* --- Jobs --- */

[!INDENT "0"!][!//
[!VAR "MAXLEN" = "num:i(13)"!][!//
[!LOOP "as:modconf('Csm')/CsmJobs/CsmJob/*"!][!//
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!LOOP "as:modconf('Csm')/CsmJobs/CsmJob/*"!][!//
/** \brief  Csm job 'CsmConf_CsmJob_[!"node:name(.)"!]'
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00013,1 */
#if (defined CsmConf_CsmJob_[!"node:name(.)"!])
  #error CsmConf_CsmJob_[!"node:name(.)"!] is already defined
#endif
#define  CsmConf_CsmJob_[!"node:name(.)"!]  [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!CALL "HEX", "_value" = "node:value(CsmJobId)"!]U

[!ENDLOOP!][!//
[!ENDINDENT!][!//
/** \brief  Number of Csm jobs
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00014,1 */
#if (defined CSM_JOB_COUNT)
  #error CSM_JOB_COUNT is already defined
#endif
#define  CSM_JOB_COUNT  [!"substring($SPACE, 1, $MAXLEN - 13)"!][!CALL "HEX", "_value" = "num:i(count(as:modconf('Csm')/CsmJobs/CsmJob/*))"!]U

/* --- Keys --- */

[!INDENT "0"!][!//
[!VAR "MAXLEN" = "num:i(13)"!][!//
[!LOOP "as:modconf('Csm')/CsmKeys/CsmKey/*"!][!//
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!][!//
[!ENDLOOP!][!//
/** \brief  The value representing an empty key in Crypto_JobPrimitiveInfoType
 **/
#if (defined CSM_KEY_EMPTY)
  #error CSM_KEY_EMPTY is already defined
#endif
#define  CSM_KEY_EMPTY  [!"substring($SPACE, 1, $MAXLEN - 13)"!]0xFFFFFFFFU

[!LOOP "as:modconf('Csm')/CsmKeys/CsmKey/*"!][!//
/** \brief  The Csm key '[!"node:name(.)"!]'
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00015,1 */
#if (defined [!"node:name(.)"!])
  #error [!"node:name(.)"!] is already defined
#endif
#define  [!"node:name(.)"!]  [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!IF "node:refvalid(CsmKeyRef) and not(node:empty(CsmKeyRef))"!][!CALL "HEX", "_value" = "num:i(node:value(CsmKeyId))"!]U [!ELSE!]CSM_KEY_EMPTY[!ENDIF!]

[!ENDLOOP!][!//
[!ENDINDENT!][!//
/** \brief  Number of Csm keys
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00016,1 */
#if (defined CSM_KEY_COUNT)
  #error CSM_KEY_COUNT is already defined
#endif
#define  CSM_KEY_COUNT  [!"substring($SPACE, 1, $MAXLEN - 13)"!][!CALL "HEX", "_value" = "num:i(count(as:modconf('Csm')/CsmKeys/CsmKey/*))"!]U

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

#define CSM_START_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

/* --- PrimitiveInfo --- */

/** \brief  Configured instances of Crypto_PrimitiveInfoType referenced in configured
 **         instances of Crypto_JobPrimitiveInfoType
 **/
[!LOOP "as:modconf('Csm')/CsmJobs/CsmJob/*"!][!//
[!IF "false() = node:empty(./CsmJobPrimitiveRef) and true() = node:refvalid(./CsmJobPrimitiveRef)"!][!//
extern CONST(Crypto_PrimitiveInfoType, CSM_CONST) Csm_PI_[!"node:name(.)"!]_[!"node:name(node:ref(./CsmJobPrimitiveRef))"!];
[!ENDIF!][!//
[!ENDLOOP!][!//

/* --- JobPrimitiveInfo --- */

/** \brief  Configured instances of Crypto_JobPrimitiveInfoType referenced in configured
 **         instances of Crypto_JobType
 **/
[!LOOP "as:modconf('Csm')/CsmJobs/CsmJob/*"!][!//
extern CONST(Crypto_JobPrimitiveInfoType, CSM_CONST) Csm_JPI_[!"node:name(.)"!];
[!ENDLOOP!][!//

/* --- JobInfo --- */

/** \brief  Configured instances of Crypto_JobInfoType referenced in configured
 **         instances of Crypto_JobType
 **/
[!LOOP "as:modconf('Csm')/CsmJobs/CsmJob/*"!][!//
extern CONST(Crypto_JobInfoType, CSM_CONST) Csm_JI_[!"node:name(.)"!];
[!ENDLOOP!][!//

#define CSM_STOP_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external data]===============================================================*/

#define CSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

/* --- Jobs --- */

/** \brief  List of configured Csm jobs
 **/
extern VAR(Crypto_JobType, CSM_APPL_DATA) Csm_JobConfigurations[CSM_JOB_COUNT];

#define CSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external function declarations]==============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/* --- Callbacks --- */

/** \brief  Declarations of configured Csm callbacks
 **/
/* !LINKSTO CSM.Dsn.Api/Funcs/00001,1 */
[!LOOP "as:modconf('Csm')/CsmCallbacks/CsmCallback/*[not(node:empty(./CsmCallbackFunc))]"!][!//
extern FUNC(void, CSM_APPL_CODE) [!"node:value(./CsmCallbackFunc)"!](P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);
[!ENDLOOP!][!//

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CSM_CFG_H */

/*==================[end of file]=================================================================*/

