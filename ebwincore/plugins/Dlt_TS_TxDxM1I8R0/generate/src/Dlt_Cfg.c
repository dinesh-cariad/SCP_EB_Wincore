/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains all definitions of the configuration of the AUTOSAR
 * module Dlt that are target independent. */

/* !LINKSTO Dlt.swdd.DltConfig.Template.Dlt_Cfg_c,1 */
[!INCLUDE "include/Dlt_Vars.m"!][!//
[!AUTOSPACING!]
[!//
/*==================[inclusions]============================================*/

[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
#include <SchM_Dlt.h>
  [!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!]
    [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $Index])"!]
#include <SchM_Dlt_1_Core_[!"num:i($Index)"!].h>
    [!ENDIF!]
  [!ENDFOR!]
[!ENDIF!]

#include <Dlt_Int.h>

#if (DLT_INCLUDE_RTE == STD_ON)
#define RTE_PTR2ARRAYTYPE_PASSING
#include <Rte_Dlt.h>
#endif

/*==================[macros]================================================*/

#if(defined DLT_VFB_START_CTX_ID)
#error DLT_VFB_START_CTX_ID already defined
#endif
/* !LINKSTO Dlt.ASR43.VfbTracing.ContextIdBaseRegistration, 1 */
/** \brief Vfb start context ID */
#define DLT_VFB_START_CTX_ID 0x10000000U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define DLT_START_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>
[!LOOP "as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*"!]
/** \brief Transmit buffer for [!"node:name(.)"!]'s messages */
VAR(uint8, DLT_VAR)  Dlt_MessageBuffer_[!"node:name(.)"!][[!"num:i(./DltLogChannelBufferSize)"!]U];
[!ENDLOOP!]
#define DLT_STOP_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>

#define DLT_START_SEC_CONST_8
#include <Dlt_MemMap.h>

CONST(Dlt_ConfigType, DLT_CONST) Dlt_ConfigSet_0 = 0U;

#define DLT_STOP_SEC_CONST_8
#include <Dlt_MemMap.h>

[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!]
[!IF "num:i(count(DltSwc/*/DltSwcContext/*)) > 0"!]
#define DLT_START_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>

[!VAR "ApplicationIdString" = "num:i(0)"!][!//
[!VAR "ApplicationIdStringToBeSplit" = "num:i(0)"!][!//

[!IF "(node:exists(DltSwc/*)) and (num:i(count(DltSwc/*)) > 0)"!][!//
  [!FOR "SwcCnt1" = "1" TO "num:i(count(DltSwc/*))"!][!//
    [!IF "(node:exists(DltSwc/*[num:i($SwcCnt1)]/DltSwcContext/*))"!][!//
      [!IF "num:i(count(DltSwc/*[num:i($SwcCnt1)]/DltSwcContext/*)) > 0"!][!//
        [!FOR "idx1" = "1" TO "num:i(count(DltSwc/*[num:i($SwcCnt1)]/DltSwcContext/*))"!][!//
          [!VAR "ApplicationIdString" = "concat((DltSwc/*[num:i($SwcCnt1)]/DltSwcContext/*[num:i($idx1)]/DltSwcApplicationId),',')"!][!//
          [!VAR "ApplicationIdStringToBeSplit" = "concat($ApplicationIdStringToBeSplit,$ApplicationIdString)"!][!//
        [!ENDFOR!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDFOR!][!//
[!ENDIF!][!//
VAR(Dlt_CtxtPerApplType, DLT_VAR) Dlt_MaxCtxtPerApplication[DLT_MAX_COUNT_APP_IDS] =
{
  [!VAR "ApplicationIdStringToBeSplit" = "substring-after($ApplicationIdStringToBeSplit, '0')"!]
  [!VAR "ApplIdStringFirstToBeSplit" = "num:i(0)"!]
  [!VAR "ApplIdStringSecondToBeSplit" = "num:i(0)"!]
  [!VAR "StringForAppIdReplacing" = "num:i(0)"!]
  [!VAR "ElemSize" = "num:i(0)"!]
  [!VAR "CountForAppl" = "num:i(0)"!]
  [!VAR "ValOfStringInit1" = "$ApplicationIdStringToBeSplit"!]
  [!VAR "StringForAppIdReplacing" = "concat($StringForAppIdReplacing, ',')"!]
  [!VAR "InitStringOfAppId" = "$ApplicationIdStringToBeSplit"!]
  [!VAR "InitStringOfAppIdToContinue" = "$ApplicationIdStringToBeSplit"!]
  [!FOR "idx1" = "1" TO "num:i(count(DltSwc/*/DltSwcContext/*))"!]
    [!VAR "ApplIdStringFirstToBeSplit" = "substring-before($ValOfStringInit1, ',')"!]
    [!VAR "ValOfStringInit1" = "substring-after($ValOfStringInit1, ',')"!]
    [!VAR "InitStringOfAppIdToContinue" = "$InitStringOfAppId"!]
    [!FOR "idx1" = "1" TO "num:i(count(DltSwc/*/DltSwcContext/*)) + 1"!]
      [!VAR "ApplIdStringSecondToBeSplit" = "substring-before($InitStringOfAppIdToContinue, ',')"!]
      [!VAR "InitStringOfAppIdToContinue" = "substring-after($InitStringOfAppIdToContinue, ',')"!]
      [!VAR "ElemSize" = "string-length($ApplIdStringSecondToBeSplit) + $ElemSize + 1"!]
      [!VAR "InitStringOfAppId" = "substring($ApplicationIdStringToBeSplit, $ElemSize, (string-length($ApplicationIdStringToBeSplit)-2) - $ElemSize)"!]
      [!IF "($ApplIdStringSecondToBeSplit != 0) and ($ApplIdStringSecondToBeSplit = $ApplIdStringFirstToBeSplit)"!]
        [!VAR "APPTOSHOW" = "$ApplIdStringSecondToBeSplit"!]
        [!VAR "CountForAppl" = "$CountForAppl + 1"!]
        [!VAR "StringForAppIdReplacing" = "concat($StringForAppIdReplacing, '0,')"!]
      [!ELSE!][!//
        [!VAR "StringForAppIdReplacing" = "concat($StringForAppIdReplacing, concat($ApplIdStringSecondToBeSplit,','))"!]
      [!ENDIF!][!//
      [!VAR "InitStringOfAppId" = "concat($StringForAppIdReplacing, $InitStringOfAppId)"!]
    [!ENDFOR!][!//
    [!VAR "InitStringOfAppIdToContinue" = "$InitStringOfAppId"!][!//
    [!VAR "StringForAppIdReplacing" = "num:i(0)"!][!//
    [!IF "$CountForAppl != 0"!][!//
  {
    [!"$APPTOSHOW"!]U, /* AppId */
    [!"num:i($CountForAppl)"!]U /* no of contexts per application */
  },
    [!ENDIF!][!//
    [!VAR "CountForAppl" = "0"!]
  [!ENDFOR!][!//
  [!VAR "$ApplicationIdStringToBeSplit" = "0"!]
  [!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!//
  {
    DLT_VFB_APP_ID_MSB, /* AppId for Rte VFB tracing */
    [!"num:i($VfbTraceFunctionCount)"!]U /* Number of Rte contexts (trace functions) */
  },
  [!ENDIF!][!//
};

#define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>
  [!ENDIF!]
[!ENDIF!][!//

#define DLT_START_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

[!IF "(node:exists(as:modconf('Dlt')/DltSwc/*)) and (num:i(count(as:modconf('Dlt')/DltSwc/*)) > 0)"!][!//
  [!FOR "DltSwcIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltSwc/*))"!][!//
 /** \brief Variable which contains all the Contexts registered per one Software Component and their assigned threshold and log level */
STATIC CONST(Dlt_SwCContextType, DLT_CONST) Dlt_SwCContextInfo_[!"node:name(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)])"!][DLT_NO_OF_REG_CTX_[!"node:name(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)])"!]] =
{
    [!FOR "DltSwcContextIdx" = "1" TO "num:i(count(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*))"!]
  {
    {
      [!VAR "IsTraceStatusAssigned" = "0"!]
      [!VAR "IsLogLevelAssigned" = "0"!]
      [!VAR "IsLogChannelAssigned" = "0"!]
      [!IF "node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcApplicationId) != 0 "!]
          /* !LINKSTO Dlt.ASR431.SWS_Dlt_00647, 1 */
        [!IF "not(text:match(node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcApplicationId),'^([1-9]{1,10})'))"!]
      "[!"node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcApplicationId)"!]",
        [!ELSE!]
      [!"node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcApplicationId)"!]U,
        [!ENDIF!]
      [!ELSE!]
      DLT_APP_ID_NULL,
      [!ENDIF!]
      [!IF "node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcContextId) != 0 "!]
          /* !LINKSTO Dlt.ASR431.SWS_Dlt_00647, 1 */
        [!IF "not(text:match(node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcContextId),'^([1-9]{1,10})'))"!]
      "[!"node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcContextId)"!]",
        [!ELSE!]
      [!"node:value(DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]/DltSwcContextId)"!]U,
        [!ENDIF!]
      [!ELSE!]
      DLT_CTX_ID_NULL,
      [!ENDIF!]
    },
      [!FOR "DltLogLevelThresholdIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogLevelSetting/*/DltLogLevelThreshold/*))"!]
        [!IF "(node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogLevelSetting/*/DltLogLevelThreshold/*[num:i($DltLogLevelThresholdIdx)]/DltLogLevelThresholdSwcContextRef)) = node:path(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]))"!][!//
          [!VAR "IsLogLevelAssigned" = "1"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDFOR!]
      [!IF "$IsLogLevelAssigned = 1"!]
    [!"as:modconf('Dlt')/DltConfigSet/*/DltLogLevelSetting/*/DltLogLevelThreshold/*[num:i($DltLogLevelThresholdIdx)]/DltSwCLogLevel"!],
        [!VAR "IsLogLevelAssigned" = "0"!]
      [!ELSE!]
    DLT_DEFAULT_MAX_LOG_LEVEL,
      [!ENDIF!]
    {
      /* !LINKSTO Dlt.IsLogChannelAssigned.DefaultLogChannel,1, Dlt.ASR431.SWS_Dlt_00664,1 */
      [!IF "num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*)) > 0"!]
        [!FOR "DltLogChannelAssignmentIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*))"!]
          [!IF "node:path(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]) = node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*[num:i($DltLogChannelAssignmentIdx)]/DltLogChannelAssignmentSwcContextRef))"!]
            [!VAR "IsLogChannelAssigned" = "1"!]
            [!BREAK!]
          [!ENDIF!]
        [!ENDFOR!]
      [!ENDIF!]
      [!IF "num:i($IsLogChannelAssigned) = 0"!]
        [!FOR "DltLogChannelIdx" = "1" TO "(num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*)))"!]
          [!IF "node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltDefaultLogChannelRef)) = node:path(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*[num:i($DltLogChannelIdx)])"!]
      TRUE,
          [!ELSE!]
      FALSE,
          [!ENDIF!]
        [!ENDFOR!]
      [!ELSE!]
        [!FOR "DltLogChannelIdx" = "1" TO "(num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*)))"!]
          [!VAR "IsLogChannelAssignmentFound" = "0"!]
          [!FOR "DltLogChannelAssignmentIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*))"!]
            [!IF "node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*[num:i($DltLogChannelAssignmentIdx)]/DltLogChannelRef)) = node:path(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*[num:i($DltLogChannelIdx)])"!]
              [!IF "(node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannelAssignment/*[num:i($DltLogChannelAssignmentIdx)]/DltLogChannelAssignmentSwcContextRef)) = node:path(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)]))"!]
      TRUE,
                [!VAR "IsLogChannelAssignmentFound" = "1"!]
              [!BREAK!]
              [!ENDIF!]
            [!ENDIF!]
          [!ENDFOR!]
          [!IF "$IsLogChannelAssignmentFound = 0"!]
      FALSE,
          [!ENDIF!]
        [!ENDFOR!]
      [!ENDIF!]
    },
      [!FOR "DltTraceStatusAssignmentIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltConfigSet/*/DltTraceStatusSetting/*/DltTraceStatusAssignment/*))"!]
        [!IF "node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltTraceStatusSetting/*/DltTraceStatusAssignment/*[num:i($DltTraceStatusAssignmentIdx)]/DltTraceStatusAssignmentSwcContextRef)) = node:path(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*[num:i($DltSwcContextIdx)])"!]
          [!VAR "IsTraceStatusAssigned" = "1"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDFOR!]
      [!IF "$IsTraceStatusAssigned = 1"!]
        [!IF "as:modconf('Dlt')/DltConfigSet/*/DltTraceStatusSetting/*/DltTraceStatusAssignment/*[num:i($DltTraceStatusAssignmentIdx)]/DltTraceStatus = 'true'"!]
    TRUE,
          [!VAR "IsTraceStatusAssigned" = "0"!]
        [!ELSE!]
    FALSE,
          [!VAR "IsTraceStatusAssigned" = "0"!]
        [!ENDIF!]
      [!ELSE!]
    DLT_DEFAULT_TRACE_STATUS,
      [!ENDIF!]
  },
    [!ENDFOR!]
};
  [!ENDFOR!]

[!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!//
/* LINKSTO Dlt.ASR43.VfbTracing.ContextInternalHandling, 1 */
STATIC CONST(Dlt_SwCContextType, DLT_CONST) Dlt_SwCContextInfo_Rte[DLT_VFB_CONTEXT_COUNT] =
{
[!FOR "DltRteVfbContext" = "1" TO "num:i($VfbTraceFunctionCount)"!]
  {
    {
      DLT_VFB_APP_ID_MSB,
      DLT_VFB_START_CTX_ID + [!"num:i(num:i($DltRteVfbContext) - 1)"!]U,
    },
    DLT_DEFAULT_MAX_LOG_LEVEL,
    {
      [!FOR "DltLogChannelIdx" = "1" TO "(num:i(count(DltConfigSet/*/DltLogOutput/*/DltLogChannel/*)))"!]
        [!IF "node:path(as:ref(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltDefaultLogChannelRef)) = node:path(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*[num:i($DltLogChannelIdx)])"!]
      TRUE, /* By default, log/trace messages will be sent on this channel */
        [!ELSE!]
      FALSE, /* By default, log/trace messages will not be sent on this channel */
        [!ENDIF!]
      [!ENDFOR!]
    },
    DLT_DEFAULT_TRACE_STATUS,
  },
[!ENDFOR!]
};
[!ENDIF!]

CONST(uint8, DLT_CONST) Dlt_MaxCtxAppIdPerSwC[DLT_SWC_REGISTERED_NO] =
{
  [!LOOP "DltSwc/*"!]
  DLT_NO_OF_REG_CTX_[!"node:name(.)"!],
  [!ENDLOOP!]
[!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!//
  DLT_VFB_CONTEXT_COUNT,
[!ENDIF!][!//
};

/** \brief Constant structure containing information for each Software Component */
CONST(Dlt_SwCLogType, DLT_CONST) Dlt_SwCLogInfo[DLT_SWC_REGISTERED_NO] =
{
  [!FOR "DltSwcIdx" = "1" TO "num:i(count(DltSwc/*))"!]
  {
    [!"num:i(DltSwc/*[num:i($DltSwcIdx)]/DltSwcSessionId)"!]U,
    [!"num:i(DltSwc/*[num:i($DltSwcIdx)]/MaxSwcLogMessageLength)"!]U,
    [!"num:i(DltSwc/*[num:i($DltSwcIdx)]/MaxSwcTraceMessageLength)"!]U,
    [!IF "DltSwc/*[num:i($DltSwcIdx)]/DltSwcSupportLogLevelChangeNotification = 'true'"!]
    TRUE
    [!ELSE!]
    FALSE
    [!ENDIF!]
  },
  [!ENDFOR!]
[!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!//
  /* LINKSTO Dlt.ASR43.VfbTracing.ContextInternalHandling, 1 */
  {
    DLT_RTE_MODULE_ID,     /* Rte's module Id */
    0U,                    /* No log message for VFB trace */
[!IF "DltGeneral/DltVfbSendHookFunctionParameters = 'true'"!][!//
    [!"DltGeneral/DltVfbTracePayloadMaxSize"!]U, /* Trace message length - payload of the hook function */
[!ELSE!][!//
    4U,
[!ENDIF!][!//
    FALSE                  /* No notification support */
  },
[!ENDIF!]
};
[!ENDIF!]

[!IF "num:i($DltNumSwcsToNotify) > 0"!]
/** \brief Constant structure containing pointers to callback functions for each Software Component */
CONST(Dlt_LogTraceSessionControlCallbacksType, DLT_CONST) Dlt_LogTraceSessionControlCallbacks[DLT_NUM_SWCS_TO_NOTIFY] =
{
  [!LOOP "DltSwc/*"!][!//
    [!IF "./DltSwcSupportLogLevelChangeNotification = 'true'"!][!//
  {
    [!"num:i(./DltSwcSessionId)"!]U,
    Rte_Call_SessionControlCallback_[!"node:name(.)"!]_LogLevelChangedNotification,
    Rte_Call_SessionControlCallback_[!"node:name(.)"!]_TraceStatusChangedNotification
  },
    [!ENDIF!][!//
  [!ENDLOOP!][!//
};
[!ENDIF!][!//

[!IF "count(as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*) > 0"!]
/** \brief Constant structure containing information for each Log Channel */
 CONST(Dlt_LogChannelType, DLT_CONST) Dlt_CfgLogChannel[DLT_TXPDU_NO] =
{
  [!LOOP "as:modconf('Dlt')/DltConfigSet/*/DltLogOutput/*/DltLogChannel/*"!]
  {
    [!"num:i(./DltLogChannelBufferOverflowTimer)"!]U,
    [!"num:i(./DltLogChannelTransmitCycle)"!]U,
    [!"num:i(./DltLogChannelTrafficShapingBandwidth)"!]U,
    [!"num:i(./DltLogChannelBufferSize)"!]U,
    &Dlt_MessageBuffer_[!"node:name(.)"!][0U],
    [!"num:i(./DltLogChannelMaxMessageLength)"!]U,
    [!"num:i(./DltTxPdu/DltITxPduHandleId)"!]U,
    "[!"node:value(./DltLogChannelId)"!]",
    [!"num:i(./DltLogChannelMaxNumOfRetries)"!]U
  },
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "count(DltConfigSet/*/DltLogOutput/*/DltLogChannel/*) > 0"!]
CONST(Dlt_LogChannelThresholdInfoType, DLT_CONST) Dlt_CfgLogChannelThresholdInfo[DLT_TXPDU_NO] =
{
[!LOOP "DltConfigSet/*[1]/DltLogOutput/*[1]/DltLogChannel/*"!]
  {
  [!"node:value(./DltLogChannelThreshold)"!],
  [!IF "./DltLogTraceStatusFlag = 'true'"!]
    TRUE
  [!ELSE!]
    FALSE
  [!ENDIF!]
  },
[!ENDLOOP!]
};
[!ENDIF!]
#define DLT_STOP_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>
[!IF "num:i(count(DltSwc/*)) > 0"!]
#define DLT_START_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>

/* !LINKSTO SWS_Dlt_00021, 1 */
/** \brief Points to each Dlt_SwCContextType for all registered Contexts per one Software component */
P2CONST(Dlt_SwCContextType, AUTOMATIC, DLT_APPL_DATA) DltSwCContextInfoPtr[DLT_SWC_REGISTERED_NO] =
{
  [!IF "(node:exists(as:modconf('Dlt')/DltSwc/*)) and (num:i(count(as:modconf('Dlt')/DltSwc/*)) > 0)"!]
    [!FOR "DltSwcIdx" = "1" TO "num:i(count(as:modconf('Dlt')/DltSwc/*))"!]
      [!IF "num:i(count(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)]/DltSwcContext/*)) > 0"!]
  &Dlt_SwCContextInfo_[!"node:name(as:modconf('Dlt')/DltSwc/*[num:i($DltSwcIdx)])"!][0U],
      [!ELSE!]
  NULL_PTR,
      [!ENDIF!]
    [!ENDFOR!]
  [!ENDIF!]
[!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!//
  &Dlt_SwCContextInfo_Rte[0U],
[!ENDIF!]
};
#define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>
[!ENDIF!]
#define DLT_START_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>
[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
  [!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!]
    [!IF "node:ref(DltMultipleConfigurationContainer/DltBswDistribution/DltMasterCore)/OsCoreId = $Index"!]
CONST(Dlt_BSWMasterContextType, DLT_CONST) Dlt_BSWMasterContext =
  {
    {
      &Dlt_InitStateMaster,
      &Dlt_ContextIdTableIdx,
      &Dlt_DefaultLogLevel,
      &Dlt_DefaultTraceStatus,
      &Dlt_FilterMessages,
      &Dlt_ContextIdTable[0U],
      [!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
      &Dlt_MaxRegisteredAppldTableEntry,
      &Dlt_AppIdTable[0U],
      &Dlt_AppToContextIdTable[0U],
      &Dlt_AppToContextIdTableIdx[0U],
      &Dlt_SessionIdTable[0U],
      [!ENDIF!][!//
    },
    Dlt_EnterExclusiveAreaMaster,
    Dlt_ExitExclusiveAreaMaster,
    {
      [!FOR "SatIndex" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores)"!]
        [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $SatIndex])"!]
      SchM_Send_Dlt_MasterSendSyncTablePort_[!"num:i($SatIndex)"!],
        [!ENDIF!]
      [!ENDFOR!]
    },
    SchM_Receive_Dlt_MasterReceiveMessagePort,
    SchM_Receive_Dlt_MasterReceiveContextMessagePort,
      [!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]
    SchM_Receive_Dlt_MasterReceiveSessionIdPort
      [!ENDIF!]
  };
    [!ENDIF!]
  [!ENDFOR!]
[!ELSE!]
CONST(Dlt_BSWMasterContextType, DLT_CONST) Dlt_BSWMasterContext =
  {
    {
      &Dlt_InitStateMaster,
      &Dlt_ContextIdTableIdx,
      &Dlt_DefaultLogLevel,
      &Dlt_DefaultTraceStatus,
      &Dlt_FilterMessages,
      &Dlt_ContextIdTable[0U],
      [!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
      &Dlt_MaxRegisteredAppldTableEntry,
      &Dlt_AppIdTable[0U],
      &Dlt_AppToContextIdTable[0U],
      &Dlt_AppToContextIdTableIdx[0U],
      &Dlt_SessionIdTable[0U],
      [!ENDIF!][!//
    },
   Dlt_EnterExclusiveAreaMaster,
   Dlt_ExitExclusiveAreaMaster
  };
[!ENDIF!][!//

[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
CONST(Dlt_BSWSatelliteContextType, DLT_CONST) Dlt_BSWSatelliteContextTable[DLT_NUMBER_OF_SATELLITES] =
{
  [!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!]
    [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $Index])"!]
  {
    /* DltSatelliteCoreId[!"num:i($Index)"!] */
    [!"num:i($Index)"!]U,
    {
      &Dlt_InitStateSlave_Core_[!"num:i($Index)"!],
      &Dlt_NumberOfContextIds_Core_[!"num:i($Index)"!],
      &Dlt_DefaultLogLevel_Core_[!"num:i($Index)"!],
      &Dlt_DefaultTraceStatus_Core_[!"num:i($Index)"!],
      &Dlt_FilterMessageStatus_Core_[!"num:i($Index)"!],
      &Dlt_ContextIdTable_Core_[!"num:i($Index)"!][0U],
      [!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
      &Dlt_MaxRegisteredAppldTableEntry_Core_[!"num:i($Index)"!],
      &Dlt_AppIdTable_Core_[!"num:i($Index)"!][0U],
      &Dlt_AppToContextIdTable_Core_[!"num:i($Index)"!][0U],
      &Dlt_AppToContextIdTableIdx_Core_[!"num:i($Index)"!][0U],
      &Dlt_SessionIdTable_Core_[!"num:i($Index)"!][0U],
      [!ENDIF!][!//
    },
    Dlt_EnterExclusiveAreaSatelliteCoreId[!"num:i($Index)"!],
    Dlt_ExitExclusiveAreaSatelliteCoreId[!"num:i($Index)"!],
    SchM_Send_Dlt_1_Core_[!"num:i($Index)"!]_SlaveSendContextMessagePort,
      [!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]
    SchM_Send_Dlt_1_Core_[!"num:i($Index)"!]_SlaveSendSessionIdPort,
      [!ENDIF!]
    SchM_Send_Dlt_1_Core_[!"num:i($Index)"!]_SlaveSendMessagePort,
    SchM_Receive_Dlt_1_Core_[!"num:i($Index)"!]_SlaveReceiveSyncTablePort_[!"num:i($Index)"!]
  },
    [!ENDIF!]
  [!ENDFOR!]
};
[!ENDIF!]

#define DLT_STOP_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
  [!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!][!//
    [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $Index])"!][!//
#if(defined DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_32)
#error DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_32 already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_32
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

VAR(uint32, DLT_VAR) Dlt_NumberOfContextIds_Core_[!"num:i($Index)"!];
[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
VAR(Dlt_AppIdTableIdxType, DLT_VAR) Dlt_MaxRegisteredAppldTableEntry_Core_[!"num:i($Index)"!];
VAR(Dlt_AppToContextIdTableIdxType, DLT_VAR) Dlt_AppToContextIdTableIdx_Core_[!"num:i($Index)"!][DLT_MAX_COUNT_APP_IDS];
[!ENDIF!][!//
#if(defined DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_32)
#error DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_32 already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_32
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

#if(defined DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_INIT_8)
#error DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_INIT_8 already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_INIT_8
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

VAR(uint8, DLT_VAR) Dlt_InitStateSlave_Core_[!"num:i($Index)"!] = DLT_UNINITIALIZED;
VAR(Dlt_MessageLogLevelType, DLT_VAR) Dlt_DefaultLogLevel_Core_[!"num:i($Index)"!] = DLT_DEFAULT_MAX_LOG_LEVEL;
VAR(Dlt_MessageTraceStatusType, DLT_VAR) Dlt_DefaultTraceStatus_Core_[!"num:i($Index)"!] = DLT_DEFAULT_TRACE_STATUS;
VAR(Dlt_FilterMessagesType, DLT_VAR) Dlt_FilterMessageStatus_Core_[!"num:i($Index)"!] = DLT_FILTER_MESSAGES;

#if(defined DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_INIT_8)
#error DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_INIT_8 already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_INIT_8
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

#if(defined DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_UNSPECIFIED)
#error DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_UNSPECIFIED already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

VAR(Dlt_ContextIdTableType, DLT_VAR) Dlt_ContextIdTable_Core_[!"num:i($Index)"!][DLT_MAX_COUNT_CONTEXT_IDS];
[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
VAR(Dlt_AppIdTableType, DLT_VAR) Dlt_AppIdTable_Core_[!"num:i($Index)"!][DLT_MAX_COUNT_APP_IDS];
VAR(Dlt_AppToContextIdTableType, DLT_VAR) Dlt_AppToContextIdTable_Core_[!"num:i($Index)"!][DLT_MAX_COUNT_APP_IDS];
VAR(Dlt_SessionIdTableType, DLT_VAR) Dlt_SessionIdTable_Core_[!"num:i($Index)"!][DLT_SWC_REGISTERED_NO];
[!ENDIF!][!//
#if(defined DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
#error DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_UNSPECIFIED already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>
    [!ENDIF!][!//
  [!ENDFOR!][!//
[!ENDIF!][!//

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
  [!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!]
    [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $Index])"!]
#if(defined DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE)
#error DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>
FUNC(void, DLT_1_Core_[!"num:i($Index)"!]_CODE) Dlt_1_Core_[!"num:i($Index)"!]_ReceiveTablesFromMaster(void)
{
  Dlt_ReceiveTablesFromMaster();
}
#if(defined DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE)
#error DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

    [!ENDIF!]
  [!ENDFOR!]
[!ENDIF!]
/*==================[internal function definitions]=========================*/

[!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!//
[!FOR "Index" = "0" TO "node:value(as:modconf('Os')/OsOS/OsNumberOfCores) - 1"!]
    [!IF "node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltSatelliteCore/*[node:ref(DltSatelliteCoreId)/OsCoreId = $Index])"!]
#if(defined DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE)
#error DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_START_SEC_CODE
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

FUNC(void, DLT_1_Core_[!"num:i($Index)"!]_CODE) Dlt_EnterExclusiveAreaSatelliteCoreId[!"num:i($Index)"!] (void)
{
  SchM_Enter_Dlt_1_Core_[!"num:i($Index)"!]_SCHM_DLT_EXCLUSIVE_AREA_[!"num:i($Index)"!]();
}

FUNC(void, DLT_1_Core_[!"num:i($Index)"!]_CODE) Dlt_ExitExclusiveAreaSatelliteCoreId[!"num:i($Index)"!] (void)
{
  SchM_Exit_Dlt_1_Core_[!"num:i($Index)"!]_SCHM_DLT_EXCLUSIVE_AREA_[!"num:i($Index)"!]();
}

#if(defined DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE)
#error DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE already defined
#endif
#define DLT_1_Core_[!"num:i($Index)"!]_STOP_SEC_CODE
#include <Dlt_1_Core_[!"num:i($Index)"!]_MemMap.h>

    [!ENDIF!]
  [!ENDFOR!]
[!ENDIF!]
/*==================[end of file]===========================================*/
