[!AUTOSPACING!]
[!/* ====================================================================================== */!][!//
[!INCLUDE "include/Csm_Macros.m"!]
[!/* ====================================================================================== */!][!//
[!//
[!/*
****************************************************************************************************
* MACRO to get the AlgorithmFamily or AlgorithmMode of CsmPrimitive configurations into
* Csm_PI_<Name> constants
****************************************************************************************************
*/!]
[!MACRO "GETALGORITHM", "_path", "_type", "_srv"!]
  [!VAR "locParNameBase" = "concat($_path, '/', $_srv, 'Algorithm', $_type          )"!]
  [!VAR "locParNameCust" = "concat($_path, '/', $_srv, 'Algorithm', $_type, 'Custom')"!]
  [!VAR "locMcrNameCust" = "concat('CRYPTO_ALGO', text:toupper(text:replace(text:replace($_type, 'Secondary', ''), 'ily', '')), '_CUSTOM')"!]
  [!VAR "locMcrNameNotS" = "concat('CRYPTO_ALGO', text:toupper(text:replace(text:replace($_type, 'Secondary', ''), 'ily', '')), '_NOT_SET')"!]
  [!IF "node:exists($locParNameBase)"!]
    [!IF "node:value($locParNameBase) = $locMcrNameCust"!]
      [!IF "node:exists($locParNameCust)"!]
        [!WS "0"!][!"$locMcrNameCust"!]_[!"node:value($locParNameCust)"!][!//
      [!ELSE!]
        [!WS "0"!][!"$locMcrNameNotS"!][!//
      [!ENDIF!]
    [!ELSE!]
      [!WS "0"!][!"node:value($locParNameBase)"!][!//
    [!ENDIF!]
  [!ELSE!]
    [!WS "0"!][!"$locMcrNameNotS"!][!//
  [!ENDIF!]
[!ENDMACRO!]
[!//
[!//
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

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <CryIf.h>

#include <Csm_Types.h>
#include <Csm_Int_Cfg.h>
#include <Csm_Cfg.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/
[!//
[!IF "num:i(0) != num:i(count(./CsmJobs/CsmJob/*))"!]
[!//

#define CSM_START_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* Job PrimitiveInfos */
/* !LINKSTO CSM.Dsn.Api/Consts/00001,1,
            CSM.Req.Gen/Crypto_PrimitiveInfoType/00001,1,
            CSM.Req.Gen/Crypto_PrimitiveInfoType/00002,1,
            CSM.Req.Gen/Crypto_PrimitiveInfoType/00003,1
 */

[!LOOP "./CsmJobs/CsmJob/*"!]
  [!IF "false() = node:empty(./CsmJobPrimitiveRef) and true() = node:refvalid(./CsmJobPrimitiveRef)"!]
CONST(Crypto_PrimitiveInfoType, CSM_CONST) Csm_PI_[!"node:name(.)"!]_[!"node:name(node:ref(./CsmJobPrimitiveRef))"!] =
{
  [!SELECT "node:ref(./CsmJobPrimitiveRef)/*[1]/*"!]
    [!VAR "vService" = "node:name(./..)"!]
    [!WS "2"!][!/* resultLength      */!][!IF "node:exists(concat($vService,'ResultLength'))"!][!"node:value(concat($vService,'ResultLength'))"!]U[!ELSEIF "node:exists(concat($vService,'ResultMaxLength'))"!][!"node:value(concat($vService,'ResultMaxLength'))"!]U[!ELSEIF "node:exists(concat($vService,'CompareLength'))"!][!"node:value(concat($vService,'CompareLength'))"!]U[!ELSE!]0U[!ENDIF!],
    [!WS "2"!][!/* service           */!][!"text:toupper(text:replace($vService,'Csm','CRYPTO_'))"!],
    [!WS "2"!][!/* algorithm         */!]{
    [!WS "2"!][!/*   family          */!]  [!CALL "GETALGORITHM", "_path"="node:path(node:current())", "_srv"="$vService", "_type"="'Familiy'        "!],
    [!WS "2"!][!/*   secondaryFamily */!]  [!CALL "GETALGORITHM", "_path"="node:path(node:current())", "_srv"="$vService", "_type"="'SecondaryFamily'"!],
    [!WS "2"!][!/*   keyLength       */!]  [!IF "node:exists(concat($vService,'KeyLength'))"!][!"node:value(concat($vService,'KeyLength'))"!]U[!ELSEIF "node:exists(concat($vService,'AlgorithmKeyLength'))"!][!"node:value(concat($vService,'AlgorithmKeyLength'))"!]U[!ELSE!]0U[!ENDIF!],
    [!WS "2"!][!/*   mode            */!]  [!CALL "GETALGORITHM", "_path"="node:path(node:current())", "_srv"="$vService", "_type"="'Mode'           "!][!CR!]
    [!WS "2"!][!/*                   */!]}
  [!ENDSELECT!]
};

[!ENDIF!]
[!ENDLOOP!]
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* Job JobPrimitiveInfos */
/* !LINKSTO CSM.Dsn.Api/Consts/00002,1 */

[!LOOP "./CsmJobs/CsmJob/*"!]
CONST(Crypto_JobPrimitiveInfoType, CSM_CONST) Csm_JPI_[!"node:name(.)"!] =
{
  [!WS "2"!][!/* callbackId                 */!][!IF "node:exists(CsmJobPrimitiveCallbackRef) and node:refvalid(CsmJobPrimitiveCallbackRef)"!]CSM_CBK_[!"text:toupper(node:name(node:ref(CsmJobPrimitiveCallbackRef)))"!][!ELSE!]CSM_CBK_EMPTY[!ENDIF!],
  [!WS "2"!][!/* primitiveInfo              */!][!IF "false() = node:empty(./CsmJobPrimitiveRef) and true() = node:refvalid(./CsmJobPrimitiveRef)"!]&Csm_PI_[!"node:name(.)"!]_[!"node:name(node:ref(./CsmJobPrimitiveRef))"!][!ELSE!]NULL_PTR[!ENDIF!],
  [!WS "2"!][!/* secureCounterId            */!]0U,
  [!IF "node:refvalid(CsmJobKeyRef)"!]
#if defined(CryIfConf_CryIfKey_[!"node:name(node:ref(node:ref(./CsmJobKeyRef)/CsmKeyRef))"!])
    [!WS "2"!][!/* cryIfKeyId                 */!][!IF "node:refvalid(CsmJobKeyRef)"!]CryIfConf_CryIfKey_[!"node:name(node:ref(node:ref(./CsmJobKeyRef)/CsmKeyRef))"!][!ELSE!]CSM_KEY_EMPTY[!ENDIF!],
#else  /* #if defined(CryIfConf_CryIfKey_[!"node:name(node:ref(node:ref(./CsmJobKeyRef)/CsmKeyRef))"!]) */
    [!WS "2"!][!/* cryIfKeyId                 */!][!IF "node:refvalid(CsmJobKeyRef)"!][!"node:value(node:ref(node:ref(./CsmJobKeyRef)/CsmKeyRef)/CryIfKeyId)"!]U[!ELSE!]CSM_KEY_EMPTY[!ENDIF!],
#endif /* #if defined(CryIfConf_CryIfKey_[!"node:name(node:ref(node:ref(./CsmJobKeyRef)/CsmKeyRef))"!]) */
  [!ELSE!]
    [!WS "2"!]CSM_KEY_EMPTY,
  [!ENDIF!]
  [!IF "node:refvalid(./CsmJobPrimitiveRef)"!]
    [!SELECT "node:ref(./CsmJobPrimitiveRef)/*[1]/*"!]
      [!VAR "vService" = "node:name(./..)"!]
      [!WS "2"!][!/* processingType             */!][!IF "node:value(concat($vService,'Processing')) = 'CSM_ASYNCHRONOUS'"!]CRYPTO_PROCESSING_ASYNC[!ELSE!]CRYPTO_PROCESSING_SYNC[!ENDIF!],
    [!ENDSELECT!]
  [!ELSE!]
    [!WS "2"!][!/* processingType             */!]CRYPTO_PROCESSING_SYNC,
  [!ENDIF!]
  [!WS "2"!][!/* callbackUpdateNotification */!][!IF "node:exists(CsmJobPrimitiveCallbackRef) and node:refvalid(CsmJobPrimitiveCallbackRef) and node:exists(CsmJobPrimitiveCallbackUpdateNotification) and node:value(CsmJobPrimitiveCallbackUpdateNotification) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!CR!]
};

[!ENDLOOP!]
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* Job JobInfos */
/* !LINKSTO CSM.Dsn.Api/Consts/00003,1 */

[!LOOP "./CsmJobs/CsmJob/*"!]
CONST(Crypto_JobInfoType, CSM_CONST) Csm_JI_[!"node:name(.)"!] =
{
  [!WS "2"!][!/* jobId       */!][!"node:value(./CsmJobId)      "!]U,
  [!WS "2"!][!/* jobPriority */!][!"node:value(./CsmJobPriority)"!]U
};

[!ENDLOOP!]
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
#define CSM_STOP_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>
[!//
[!ENDIF!]

/*==================[external data]===============================================================*/
[!//
[!IF "num:i(0) != num:i(count(./CsmJobs/CsmJob/*))"!]
[!//

#define CSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

[!IF "num:i(0) != num:i(count(./CsmJobs/CsmJob/*))"!]
[!//
/* Jobs */
/* !LINKSTO CSM.Dsn.Api/Datas/00002,1 */

VAR(Crypto_JobType, CSM_APPL_DATA) Csm_JobConfigurations[CSM_JOB_COUNT] =
{
[!LOOP "node:order(./CsmJobs/CsmJob/*,'node:value(./CsmJobId)')"!][!//
  {
    [!WS "4"!][!/* jobId                                          */!][!"node:value(./CsmJobId)"!],
    [!WS "4"!][!/* state/jobState                                 */!]CRYPTO_JOBSTATE_IDLE,
    [!WS "4"!][!/* PrimitiveInputOutput/jobPrimitiveInputOutput   */!]{
    [!WS "6"!][!/*   inputPtr, inputLength                        */!]  NULL_PTR, 0U,
    [!WS "6"!][!/*   secondaryInputPtr, secondaryInputLength      */!]  NULL_PTR, 0U,
    [!WS "6"!][!/*   tertiaryInputPtr, tertiaryInputLength        */!]  NULL_PTR, 0U,
    [!WS "6"!][!/*   outputPtr, outputLengthPtr                   */!]  NULL_PTR, NULL_PTR,
    [!WS "6"!][!/*   secondaryOutputPtr, secondaryOutputLengthPtr */!]  NULL_PTR, NULL_PTR,
    [!IF "node:value(./../../../CsmEbGeneral/CsmEbMisc/CsmEbAutosarApiVersion) = 'CSM_API_VERSION_431'"!]
      [!WS "6"!][!/*   input64                                    */!]  0U,
    [!ENDIF!]
    [!WS "6"!][!/*   verifyPtr                                    */!]  NULL_PTR,
    [!IF "node:value(./../../../CsmEbGeneral/CsmEbMisc/CsmEbAutosarApiVersion) != 'CSM_API_VERSION_EB'"!]
      [!WS "6"!][!/*   output64Ptr                                */!]  NULL_PTR,
    [!ENDIF!]
    [!WS "6"!][!/*   mode                                         */!]  CRYPTO_OPERATIONMODE_SINGLECALL
    [!WS "4"!][!/*                                                */!]},
    [!WS "4"!][!/* jobPrimitiveInfo                               */!]&Csm_JPI_[!"node:name(.)"!],
    [!WS "4"!][!/* jobInfo                                        */!]&Csm_JI_[!"node:name(.)"!],
    [!WS "4"!][!/* cryptoKeyId                                    */!]CSM_KEY_EMPTY
  },
[!ENDLOOP!]
};
[!//
[!ENDIF!]

#define CSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>
[!//
[!ENDIF!]

/*==================[external function definitions]===============================================*/

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/

