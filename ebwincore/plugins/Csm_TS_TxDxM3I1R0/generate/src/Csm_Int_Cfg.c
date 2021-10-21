[!AUTOSPACING!]
[!/* ====================================================================================== */!][!//
[!INCLUDE "include/Csm_Macros.m"!]
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

/*==================[misra deviations]============================================================*/

/* MISRA-C:2012 Deviations
 *
 * MISRAC2012-1) Deviated Rule: 11.1 (required)
 *   Conversions shall not be performed between a pointer to a function and any other type.
 *
 *   Reason:
 *   The calls to callbacks accessible via Rte must be inserted in the array of API callbacks
 *   returning void. That's why the return value of Rte callbacks has to be casted.
 */

/*==================[includes]====================================================================*/

#include <Csm_Types.h>
#include <Csm_Int_Types.h>
#include <Csm_Int_Cfg.h>
#include <Csm_Cfg.h>

#include <CryIf.h>

[!IF "$Csm_Rte_Enabled_Callback = 'true'"!]
#include <Rte_Csm.h>
[!ENDIF!]

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

#define CSM_START_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Callbacks --- */
/* !LINKSTO CSM.Dsn.Api/Consts/00004,1 */

[!IF "num:i(0) != num:i(count(./CsmCallbacks/CsmCallback/*))"!]
/* Deviation MISRAC2012-1 <START> */
CONST(Csm_CallbackNotificationType, CSM_CONST) Csm_Callbacks[CSM_CBK_COUNT] =
{
  [!LOOP "node:order(./CsmCallbacks/CsmCallback/*, 'node:value(./CsmCallbackId)')"!]
    [!IF "node:empty(./CsmCallbackFunc)"!]
      [!WS "2"!](Csm_CallbackNotificationType) Rte_Call_[!"node:name(.)"!]_CallbackNotification_CallbackNotification,
    [!ELSE!]
      [!WS "2"!][!"node:value(./CsmCallbackFunc)"!],
    [!ENDIF!]
  [!ENDLOOP!]
};
/* Deviation MISRAC2012-1 <STOP> */
[!ENDIF!]

[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Keys --- */
/* !LINKSTO CSM.Dsn.Api/Consts/00005,1 */

/* !LINKSTO CSM.Req.Gen/CsmKeys/00001,1 */
#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(uint32, CSM_CONST) Csm_Keys[CSM_KEY_COUNT] =
{
[!LOOP "node:order(./CsmKeys/CsmKey/*,'node:value(./CsmKeyId)')"!][!//
  [!WS "2"!][!"node:value(node:ref(./CsmKeyRef)/CryIfKeyId)"!]U, /* [!"node:name(node:ref(./CsmKeyRef))"!]" */
[!ENDLOOP!]
};
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Queues --- */
/* !LINKSTO CSM.Dsn.Api/Consts/00006,1 */

CONSTP2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) Csm_Queues[CSM_QUE_COUNT] =
{
[!LOOP "./CsmQueues/CsmQueue/*"!]
  [!IF "true() = node:contains(node:refs(./../../../CsmJobs/CsmJob/*/CsmJobQueueRef), .)"!]
    [!WS "2"!]&(Csm_Queue_[!"node:name(.)"!]),
  [!ELSE!]
    [!WS "2"!]NULL_PTR,
  [!ENDIF!]
[!ENDLOOP!]
};

[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Job additional configurations --- */
/* !LINKSTO CSM.Dsn.Api/Consts/00007,1 */

CONST(Crypto_JobExtraCnfgType, CSM_CONST) Csm_JobExtraCnfg[CSM_JOB_COUNT] =
{
[!LOOP "node:order(./CsmJobs/CsmJob/*,'node:value(./CsmJobId)')"!]
  {
    [!IF "node:refvalid(./CsmJobQueueRef)"!]
      [!WS "4"!][!/* csmQueue     */!]&Csm_Queue_[!"node:name(node:ref(./CsmJobQueueRef))"!]
    [!ELSE!]
      [!WS "4"!]NULL_PTR
      [!ERROR "Job/CsmJobQueueRef not valid."!]
    [!ENDIF!]
  },
[!ENDLOOP!]
};

[!//
#define CSM_STOP_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external data]===============================================================*/

#define CSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* ---- Queue element buffers --- */
/* !LINKSTO CSM.Dsn.Api/Datas/00003,1 */

[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!]
VAR(Csm_QueueElementType, CSM_APPL_DATA) Csm_QueueElementsBuffer_[!"node:name(.)"!][CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!]] =
{
  [!FOR "i" = "num:i(1)" TO "num:i(node:value(./CsmQueueSize))"!]
    [!WS "2"!]{ NULL_PTR, NULL_PTR },
  [!ENDFOR!]
};

[!ENDLOOP!]
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Queues --- */
/* !LINKSTO CSM.Dsn.Api/Datas/00005,1 */

[!VAR "queueID" = "num:i(0)"!]
[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!]
VAR(Csm_QueueType, CSM_APPL_DATA) Csm_Queue_[!"node:name(.)"!] =
{
  [!WS "2"!][!/* queueId             */!][!"$queueID"!]U,
  [!WS "2"!][!/* lockMainQueueDelJob */!]FALSE,
  [!WS "2"!][!/* lockMainProcessJob  */!]FALSE,
  [!IF "node:refvalid(./CsmChannelRef)"!]
#if defined(CryIfConf_CryIfChannel_[!"node:name(node:ref(./CsmChannelRef))"!])
    [!WS "2"!][!/* cryIfChannelId      */!]CryIfConf_CryIfChannel_[!"node:name(node:ref(./CsmChannelRef))"!],
#else
    [!WS "2"!][!/* cryIfChannelId      */!][!"node:value(node:ref(./CsmChannelRef)/CryIfChannelId)"!]U,
#endif
  [!ELSE!]
    [!WS "2"!][!/* cryIfChannelId      */!]CSM_CHN_EMPTY,
    [!ERROR "Job/CsmJobQueueRef/CsmChannelRef not valid."!]
  [!ENDIF!]
  [!WS "2"!][!/* currentSize         */!]0U,
  [!WS "2"!][!/* maxSize             */!]CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!],
  [!WS "2"!][!/* head                */!]NULL_PTR,
  [!WS "2"!][!/* data                */!]&(Csm_QueueElementsBuffer_[!"node:name(.)"!][0U])
  [!VAR "queueID" = "num:i($queueID + num:i(1))"!]
};

[!ENDLOOP!]
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
/* --- Job additional datas --- */
/* !LINKSTO CSM.Dsn.Api/Datas/00004,1 */

VAR(Crypto_JobExtraDataType, CSM_APPL_DATA) Csm_JobExtraData[CSM_JOB_COUNT] =
{
[!LOOP "node:order(./CsmJobs/CsmJob/*,'node:value(./CsmJobId)')"!]
  [!WS "2"!]{ CSM_QL_NONE },
[!ENDLOOP!]
};

[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
#define CSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external function definitions]===============================================*/

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/

