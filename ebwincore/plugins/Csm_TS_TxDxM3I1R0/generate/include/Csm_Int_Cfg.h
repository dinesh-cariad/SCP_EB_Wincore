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

#ifndef CSM_INT_CFG_H
#define CSM_INT_CFG_H

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm_Types.h>
#include <Csm_Cfg.h>
#include <Csm_Int_Types.h>

/*==================[macros]======================================================================*/

/* --- CryIf channels --- */

/** \brief  The value representing an empty CryIf channel reference in internal Csm_QueueType
 **/
#if (defined CSM_CHN_EMPTY)
  #error CSM_CHN_EMPTY is already defined
#endif
#define  CSM_CHN_EMPTY  0xFFFFFFFFU

/* --- Callbacks --- */

[!INDENT "0"!][!//
[!VAR "MAXLEN" = "num:i(5)"!][!//
[!LOOP "./CsmCallbacks/CsmCallback/*"!][!//
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!][!//
[!ENDLOOP!][!//
/** \brief  The value representing an empty callback in Crypto_JobPrimitiveInfoType
 **/
#if (defined CSM_CBK_EMPTY)
  #error CSM_CBK_EMPTY is already defined
#endif
#define  CSM_CBK_EMPTY  [!"substring($SPACE, 1, $MAXLEN - 5)"!]0xFFFFFFFFU
[!LOOP "./CsmCallbacks/CsmCallback/*"!][!//
/** \brief  Csm callback 'CSM_CBK_[!"text:toupper(node:name(.))"!]'
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00017,1 */
#if (defined CSM_CBK_[!"text:toupper(node:name(.))"!])
  #error CSM_CBK_[!"text:toupper(node:name(.))"!] is already defined
#endif
#define  CSM_CBK_[!"text:toupper(node:name(.))"!]  [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!CALL "HEX", "_value" = "num:i(node:value(./CsmCallbackId))"!]U
[!ENDLOOP!][!//
[!ENDINDENT!][!//
/** \brief  Number of Csm callbacks
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00018,1 */
#if (defined CSM_CBK_COUNT)
  #error CSM_CBK_COUNT is already defined
#endif
#define  CSM_CBK_COUNT  [!"substring($SPACE, 1, $MAXLEN - 5)"!][!CALL "HEX", "_value" = "num:i(count(./CsmCallbacks/CsmCallback/*))"!]U

/* --- Queues --- */

[!INDENT "0"!][!//
[!VAR "MAXLEN" = "num:i(5)"!][!//
[!LOOP "./CsmQueues/CsmQueue/*"!][!//
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!][!//
[!ENDLOOP!][!//
/** \brief  The value representing an empty queue
 **/
#if (defined CSM_QUE_EMPTY)
  #error CSM_QUE_EMPTY is already defined
#endif
#define  CSM_QUE_EMPTY  [!"substring($SPACE, 1, $MAXLEN - 5)"!]0xFFFFFFFFU
[!LOOP "./CsmQueues/CsmQueue/*"!][!//
/** \brief  Csm callback 'CSM_QUE_[!"text:toupper(node:name(.))"!]'
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00019,1 */
#if (defined CSM_QUE_[!"text:toupper(node:name(.))"!])
  #error CSM_QUE_[!"text:toupper(node:name(.))"!] is already defined
#endif
#define  CSM_QUE_[!"text:toupper(node:name(.))"!]  [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!CALL "HEX", "_value" = "num:i(@index)"!]U
[!ENDLOOP!][!//
[!ENDINDENT!][!//
/** \brief  Number of Csm queues
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00020,1 */
#if (defined CSM_QUE_COUNT)
  #error CSM_QUE_COUNT is already defined
#endif
#define  CSM_QUE_COUNT  [!"substring($SPACE, 1, $MAXLEN - 5)"!][!CALL "HEX", "_value" = "num:i(count(./CsmQueues/CsmQueue/*))"!]U

/* --- Queue sizes --- */

[!INDENT "0"!][!//
[!VAR "MAXLEN" = "num:i(5)"!][!//
[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!][!//
  [!IF "$MAXLEN < num:i(string-length(node:name(.)))"!][!VAR "MAXLEN" = "num:i(string-length(node:name(.)))"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!][!//
/** \brief  Size of Csm queue 'CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!]'
 **/
/* !LINKSTO CSM.Dsn.Api/Macros/00021,1 */
#if (defined CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!])
  #error CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!] is already defined
#endif
#define  CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!]  [!"substring($SPACE, 1, $MAXLEN - string-length(node:name(.)))"!][!CALL "HEX", "_value" = "num:i(node:value(./CsmQueueSize))"!]U
[!ENDLOOP!][!//
[!ENDINDENT!][!//

/* --- Misc --- */

#if (defined CSM_API_VERSION)
#error CSM_API_VERSION is already defined
#endif
/** \brief  tbd **/
/* !LINKSTO CSM.Dsn.Api/Macros/00022,1 */
#define CSM_API_VERSION [!"node:value(./CsmEbGeneral/CsmEbMisc/CsmEbAutosarApiVersion)"!]

#if (defined CSM_API_JOBSTATE)
#error CSM_API_JOBSTATE is already defined
#endif
/** \brief  tbd **/
/* !LINKSTO CSM.Dsn.Api/Macros/00023,1 */
[!IF "node:value(./CsmEbGeneral/CsmEbMisc/CsmEbAutosarApiVersion) = 'CSM_API_VERSION_430'"!]
#define CSM_API_JOBSTATE state
[!ELSE!]
#define CSM_API_JOBSTATE jobState
[!ENDIF!]

#if (defined CSM_API_JOBPRIMITIVEINPUTOUTPUT)
#error CSM_API_JOBPRIMITIVEINPUTOUTPUT is already defined
#endif
/** \brief  tbd **/
/* !LINKSTO CSM.Dsn.Api/Macros/00024,1 */
[!IF "node:value(./CsmEbGeneral/CsmEbMisc/CsmEbAutosarApiVersion) = 'CSM_API_VERSION_430'"!]
#define CSM_API_JOBPRIMITIVEINPUTOUTPUT PrimitiveInputOutput
[!ELSE!]
#define CSM_API_JOBPRIMITIVEINPUTOUTPUT jobPrimitiveInputOutput
[!ENDIF!]

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

#define CSM_START_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

/* --- Callbacks --- */

[!IF "num:i(0) != num:i(count(./CsmCallbacks/CsmCallback/*))"!][!//
/** \brief  List of configured Csm callbacks
 **/
extern CONST(Csm_CallbackNotificationType, CSM_CONST) Csm_Callbacks[CSM_CBK_COUNT];
[!ENDIF!][!//

/* !LINKSTO CSM.Req.Gen/CsmKeys/00001,1 */
 #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
/* --- Keys --- */

/** \brief  List of configured Csm keys
 **/
extern CONST(uint32, CSM_CONST) Csm_Keys[CSM_KEY_COUNT];
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Queues --- */

/** \brief  List of configured Csm queues
 **/
extern CONSTP2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) Csm_Queues[CSM_QUE_COUNT];

/* --- Job additional configurations --- */

/** \brief  List of additional internal configuration sets per Csm job
 **/
extern CONST(Crypto_JobExtraCnfgType, CSM_CONST) Csm_JobExtraCnfg[CSM_JOB_COUNT];

#define CSM_STOP_SEC_CONST_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external data]===============================================================*/

#define CSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

/* --- Queue element buffers --- */

/** \brief  Instances of configured Csm module queue data elements
 **/
[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!][!//
extern VAR(Csm_QueueElementType, CSM_APPL_DATA) Csm_QueueElementsBuffer_[!"node:name(.)"!][CSM_QUE_SIZE_[!"text:toupper(node:name(.))"!]];
[!ENDLOOP!][!//

/* --- Queues --- */

/** \brief  Instances of configured Csm module queues
 **/
[!LOOP "node:refs(./CsmJobs/CsmJob/*/CsmJobQueueRef)"!][!//
extern VAR(Csm_QueueType, CSM_APPL_DATA) Csm_Queue_[!"node:name(.)"!];
[!ENDLOOP!][!//

/* --- Job additional datas --- */

/** \brief  List of additional internal data sets per Csm job
 **/
extern VAR(Crypto_JobExtraDataType, CSM_APPL_DATA) Csm_JobExtraData[CSM_JOB_COUNT];

#define CSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Csm_MemMap.h>

/*==================[external function declarations]==============================================*/

/*================================================================================================*/

#endif /* #ifndef CSM_INT_CFG_H */

/*==================[end of file]=================================================================*/

