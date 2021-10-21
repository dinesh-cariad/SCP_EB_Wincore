/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.7
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.

 * MISRAC2012-2) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears
 * in a single function.
 *
 * Reason:
 * The object is defined in this way because the values that are stored
 * in them are required for the next call of this function and should be
 * hold in the data segment memory.
 */
[!INCLUDE "../include/WdgM_Cfg.m"!][!//
[!INCLUDE "../include/WdgM_Macros.m"!][!//
[!INCLUDE "../include/WdgM_Checks.m"!][!//
[!CODE!][!//

/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Lcfg.h>                               /* Module internal BSW declarations and API */
[!IF "$WdgMMulticoreEnabled = 'true'"!]
    [!IF "$DefaultServiceAPI = 'NONE'"!][!//
#include <WdgM.h>                                   /* For WdgM_MainFunction() */
    [!ENDIF!][!//
[!VAR "MULTICORE_NUM_CORES" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores)"!]
[!ELSE!][!//
[!VAR "MULTICORE_NUM_CORES" = "1"!]
[!ENDIF!][!//
[!IF "node:exists(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterCoreId)"!][!//
[!VAR "WDGM_MASTER_CORE_ID" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterCoreId)"!][!//
[!ELSE!][!//
[!VAR "WDGM_MASTER_CORE_ID" = "'0'"!][!//
[!ENDIF!][!//

/*==================[macros]=====================================================================*/

[!AUTOSPACING!][!//
[!//

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal data]==============================================================*/

/*==================[external data]==============================================================*/

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable3,1,WDGM.EB.DesignDecision.InternalVariable5,1,
 WDGM.EB.DesignDecision.InternalVariable8,1 */
[!LOOP "WdgMGeneral/WdgMSupervisedEntity/*"!][!//
/* Checkpoint individual run-time data for each SE */
VAR(WdgM_EB_CPDataType,WDGM_VAR) WdgM_EB_CPData_[!"@name"!][[!"num:i(count(WdgMCheckpoint/*))"!]];
[!ENDLOOP!][!//



#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
#if (WDGM_EB_DM_NUM > 0)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable4,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable10,2 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable11,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable9,1 */
VAR(WdgM_EB_DMDataType,WDGM_VAR) WdgM_EB_DMData[WDGM_EB_DM_NUM];
#else
VAR(WdgM_EB_DMDataType,WDGM_VAR) WdgM_EB_DMData[1];
#endif
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/*==================[internal constants]=========================================================*/

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_16BIT
#else
#define WDGM_START_SEC_CONST_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

CONST(WdgM_EB_SEIdType, WDGM_CONST) WdgM_EB_SEIntSEId[WDGM_EB_SE_NUM] =
{
  [!LOOP "node:order(WdgMGeneral/WdgMSupervisedEntity/*, 'WdgMSupervisedEntityId')"!][!//
  [!VAR "IntSEId" = "@index"!][!//
  [!VAR "SEName" = "@name"!][!//
  [!"$IntSEId"!]U, /* Internal Id of [!"$SEName"!] */
  [!ENDLOOP!][!//
};

[!IF "node:exists(WdgMGeneral/WdgMCallerIds) and (count(WdgMGeneral/WdgMCallerIds/WdgMCallerId/*) > 0) and $GetExpectedWdgMModeCalloutEnabled = 'false'"!][!//
/*------------------[Allowed Caller IDs]------------------------------------*/
CONST(uint16, WDGM_CONST) WdgM_EB_CallerIds[WDGM_EB_CALLERIDS_NUM] =
{
  [!LOOP "WdgMGeneral/WdgMCallerIds/WdgMCallerId/*"!][!//
  [!"node:value(.)"!]U,
  [!ENDLOOP!][!// WdgMCallerIds /*
};
[!ENDIF!][!//

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_16BIT
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/*------------------[trigger and watchdog configuration]--------------------*/

[!LOOP "node:order(WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
[!IF "count(WdgMTrigger/*) > 0"!][!//
/* trigger configuration for mode [!"name(.)"!] */
static CONST(WdgM_EB_TriggerCfgType,WDGM_CONST) WdgM_EB_TrigCfg_M[!"WdgMModeId"!][[!"num:i(count(WdgMTrigger/*))"!]] =
{
  [!LOOP "WdgMTrigger/*"!][!//
  { /* [!"@name"!] */
    [!"WdgMWatchdogMode"!], /* watchdog trigger mode */
    [!"WdgMTriggerConditionValue"!]U, /* TriggerCondition */
    [!"as:ref(as:ref(WdgMTriggerWatchdogRef)/WdgMWatchdogDeviceRef)/WdgIfDeviceIndex"!]U /* watchdog device index */
  },
  [!ENDLOOP!][!// WdgM triggers /*
};
[!ELSE!][!//
/* no triggers configured for mode [!"name(.)"!] */
[!ENDIF!][!//

[!ENDLOOP!][!// WdgM modes /*

/*------------------[SE configuration]--------------------------------------*/

[!VAR "BaseIntGraphId" = "0"!][!//
[!VAR "DMId" = "0"!][!//
[!LOOP "WdgMGeneral/WdgMSupervisedEntity/*"!][!//
/*------------------Internal configuration data for SE: [!"@name"!] */

[!VAR "SEName" = "@name"!][!//
[!VAR "ExtSEId" = "num:i(WdgMSupervisedEntityId)"!][!//
[!VAR "IntSEId" = "@index"!][!//
[!LOOP "WdgMCheckpoint/*"!][!//
[!VAR "CPName" = "@name"!][!//
[!VAR "IntCPId" = "WdgMCheckpointId"!][!//
[!VAR "BaseExtGraphId" = "0"!][!//
[!LOOP "node:order(../../../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
[!VAR "IntModeId" = "WdgMModeId"!][!//
[!VAR "tempText"!][!ENDVAR!][!//
[!LOOP "WdgMAliveSupervision/*[(num:i(as:ref(WdgMAliveSupervisionCheckpointRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMAliveSupervisionCheckpointRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]"!][!//
/* alive supervision of CP [!"$CPName"!] in mode [!"$IntModeId"!] */
static CONST(WdgM_EB_CPAliveCfgType,WDGM_CONST) WdgM_EB_CPAliveCfg_[!"$IntSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!] =
{
  [!"num:i(WdgMExpectedAliveIndications)"!]U, /* ExpextedAliveIndication */
  [!"num:i(WdgMSupervisionReferenceCycle)"!]U, /* Amount of supervision reference cycles */
  [!"num:i(WdgMMaxMargin)"!]U, /* max margin */
  [!"num:i(WdgMMinMargin)"!]U /* min margin */
};

[!ENDLOOP!][!//
[!/*
Generate Configuration for external supervision graph
=====================================================
*/!][!//
[!/* Check if checkpoint is a destination node of some external transition
*/!][!//
[!VAR "NumConfiguredPredecessors" = "num:i(count(WdgMExternalLogicalSupervision/*/WdgMExternalTransition/*[(num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]))"!][!//
[!IF "$NumConfiguredPredecessors > 0"!][!//
[!VAR "ExtGraphIdOffset" = "num:i(WdgMExternalLogicalSupervision/*/WdgMExternalTransition/*[(num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)][1]/../../@index)"!][!//
/* predecessor points of CP [!"$CPName"!] in mode [!"$IntModeId"!] */
static CONST(WdgM_EB_CPType,WDGM_CONST) WdgM_EB_CPPred_[!"$ExtSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!][[!"$NumConfiguredPredecessors"!]] =
{
  [!LOOP "WdgMExternalLogicalSupervision/*/WdgMExternalTransition/*[(num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]"!][!//
  [!VAR "PredecessorSEID" = "num:i(as:ref(WdgMExternalTransitionSourceRef)/../../@index)"!][!//
  [!VAR "PredecessorCPID" = "num:i(as:ref(WdgMExternalTransitionSourceRef)/WdgMCheckpointId)"!][!//
  {
    [!"$PredecessorSEID"!]U, /* Internal SEId of predecessor 0 */
    [!"$PredecessorCPID"!]U /* CPId of predecessor 0 */
  },
  [!ENDLOOP!][!//
};
[!VAR "tempText"!][!"$tempText"!][!"num:i($ExtSEId)"!]-[!"num:i($IntCPId)"!]:[!"num:i($IntModeId)"!][!ENDVAR!][!//

/* External logical supervision of CP [!"$CPName"!] in mode [!"$IntModeId"!] */
static CONST(WdgM_EB_CPLogicalCfgType,WDGM_CONST) WdgM_EB_CPExtLogicalCfg_[!"$ExtSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!] =
{
  WdgM_EB_CPPred_[!"$ExtSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!], /* pointer to predecessor CPs */
  [!"num:i($BaseExtGraphId+$ExtGraphIdOffset)"!]U, /* graph id */
  [!CALL "GetSEIndexMapped", "SEIdCPIdModeId"="$tempText"!], /* checkpoint sequence */
  [!"$NumConfiguredPredecessors"!]U, /* number of predecessors */
  [!IF "count(WdgMExternalLogicalSupervision/*/WdgMExternalCheckpointFinalRef/*[(num:i(node:ref(.)/WdgMCheckpointId) = $IntCPId) and ((num:i(node:ref(.)/../../WdgMSupervisedEntityId)) = $ExtSEId)]) > 0"!]  TRUE[!ELSE!]  FALSE[!ENDIF!] /* isFinal */
};

[!ELSE!][!//
[!/* Check if the checkpoint is an initial node of some graph and not already treated before
*/!][!//
[!LOOP "WdgMExternalLogicalSupervision/*/WdgMExternalCheckpointInitialRef/*[(num:i(as:ref(.)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(.)/../../WdgMSupervisedEntityId) = $ExtSEId)]"!][!//
[!VAR "ExtGraphIdOffset" = "num:i(../../@index)"!][!//
[!VAR "tempText"!][!"$tempText"!][!"num:i($ExtSEId)"!]-[!"num:i($IntCPId)"!]:[!"num:i($IntModeId)"!][!ENDVAR!][!//
/* External logical supervision of CP [!"$CPName"!] in mode [!"$IntModeId"!] */
static CONST(WdgM_EB_CPLogicalCfgType,WDGM_CONST) WdgM_EB_CPExtLogicalCfg_[!"$ExtSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!] =
{
  NULL_PTR, /* initial node has no predecessor CPs */
  [!"num:i($BaseExtGraphId+$ExtGraphIdOffset)"!]U, /* graph id */
  [!CALL "GetSEIndexMapped", "SEIdCPIdModeId"="$tempText"!], /* checkpoint sequence */
  0U, /* number of predecessors */
  FALSE /* isFinal */
};

[!ENDLOOP!][!//
[!ENDIF!][!//
[!/* continuously increment base graph id
*/!][!//
[!VAR "BaseExtGraphId" = "$BaseExtGraphId + num:i(count(WdgMExternalLogicalSupervision/*))"!][!//
[!ENDLOOP!][!/* WdgM modes
*/!][!//
[!/*
Generate Configuration for internal supervision graph
=====================================================
Check if internal supervision is configured at all for the CPs of actual SE
*/!][!//
[!VAR "IsInternalLogicalSupervisionConfigured" = "count(../../WdgMInternalTransition/*[((num:i(as:ref(WdgMInternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)) or ((num:i(as:ref(WdgMInternalTransitionSourceRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionSourceRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) > 0"!][!//
[!IF "$IsInternalLogicalSupervisionConfigured"!][!//
[!VAR "NumConfiguredPredecessors" = "num:i(count(../../WdgMInternalTransition/*[(num:i(as:ref(WdgMInternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]))"!][!//
[!IF "$NumConfiguredPredecessors > 0"!][!//
/* predecessor points of CP [!"$CPName"!] */
static CONST(WdgM_EB_CPType,WDGM_CONST) WdgM_EB_CPPred_[!"$IntSEId"!]_[!"$IntCPId"!][[!"$NumConfiguredPredecessors"!]] =
{
  [!LOOP "../../WdgMInternalTransition/*[(num:i(as:ref(WdgMInternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]"!][!//
  [!VAR "PredecessorSEID" = "num:i(as:ref(WdgMInternalTransitionSourceRef)/../../@index)"!][!//
  [!VAR "PredecessorCPID" = "num:i(as:ref(WdgMInternalTransitionSourceRef)/WdgMCheckpointId)"!][!//
  {
    [!"$PredecessorSEID"!]U, /* Internal SEId of predecessor 0 */
    [!"$PredecessorCPID"!]U /* CPId of predecessor 0 */
  },
  [!ENDLOOP!][!//
};

/* Internal logical supervision of [!"$CPName"!] */
static CONST(WdgM_EB_CPLogicalCfgType,WDGM_CONST) WdgM_EB_CPIntLogicalCfg_[!"$IntSEId"!]_[!"$IntCPId"!] =
{
  WdgM_EB_CPPred_[!"$IntSEId"!]_[!"$IntCPId"!], /* pointer to predecessor CPs */
  [!"num:i($BaseIntGraphId)"!]U, /* graph id */
  0U, /* checkpoint sequence for internal logical supervision is not used */
  [!"$NumConfiguredPredecessors"!]U, /* number of predecessors */
  [!IF "count(../../WdgMInternallCheckpointFinalRef/*[(num:i(node:ref(.)/WdgMCheckpointId) = $IntCPId) and ((num:i(node:ref(.)/../../WdgMSupervisedEntityId)) = $ExtSEId)]) > 0"!]  TRUE[!ELSE!]
  FALSE[!ENDIF!] /* isFinal */
};

[!ELSEIF "node:refvalid(../../WdgMInternalCheckpointInitialRef) and (num:i(node:ref(../../WdgMInternalCheckpointInitialRef)/WdgMCheckpointId) = $IntCPId) and (num:i(node:ref(../../WdgMInternalCheckpointInitialRef)/../../WdgMSupervisedEntityId) = $ExtSEId)"!][!//
/* predecessor points of CP [!"$CPName"!] */
static CONST(WdgM_EB_CPType,WDGM_CONST) WdgM_EB_CPPred_[!"$IntSEId"!]_[!"$IntCPId"!][1] =
{
  {
    WDGM_EB_DUMMY_SEID, /* SEId of predecessor 0 */
    WDGM_EB_DUMMY_CPID /* CPId of predecessor 0 */
  }
};

/* Internal logical supervision of [!"$CPName"!] */
static CONST(WdgM_EB_CPLogicalCfgType,WDGM_CONST) WdgM_EB_CPIntLogicalCfg_[!"$IntSEId"!]_[!"$IntCPId"!] =
{
  WdgM_EB_CPPred_[!"$IntSEId"!]_[!"$IntCPId"!], /* pointer to predecessor CPs */
  [!"num:i($BaseIntGraphId)"!]U, /* graph id */
  0U, /* checkpoint sequence for internal logical supervision is not used */
  0U, /* number of predecessors */
  FALSE /* isFinal */
};

[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!// Checkpoints
[!/*
Generate Configuration for deadline supervision
===============================================
*/!][!//
[!LOOP "node:order(../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
[!VAR "IntModeId" = "WdgMModeId"!][!//
[!VAR "ListOfGeneratedCPIds" = "''"!][!//
[!FOR "MaxSimpleSortLoop" = "0" TO "count(../../../../WdgMGeneral/WdgMSupervisedEntity/*[WdgMSupervisedEntityId = $ExtSEId]/WdgMCheckpoint/*)"!]
[!VAR "IsContinueSimpleSortLoop" = "'false'"!][!//
[!LOOP "node:order(../../../../WdgMGeneral/WdgMSupervisedEntity/*[WdgMSupervisedEntityId = $ExtSEId]/WdgMCheckpoint/*, 'WdgMCheckpointId')"!][!//
[!VAR "CPName" = "@name"!][!//
[!VAR "IntCPId" = "WdgMCheckpointId"!][!//
[!SELECT "../../../../../WdgMConfigSet/*[1]/WdgMMode/*[WdgMModeId = $IntModeId]"!][!//
[!IF "not(text:contains( text:split($ListOfGeneratedCPIds), $IntCPId))"!][!//
[!VAR "IsDeadlineSupervisionStartConfiguration" = "count(WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStartRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStartRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) = 1"!][!//
[!IF "$IsDeadlineSupervisionStartConfiguration"!][!//
  [!VAR "DeadlineSupervisionStartConfigurationDMMax" = "WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStartRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStartRef)/../../WdgMSupervisedEntityId) = $ExtSEId))][1]/WdgMDeadlineMax"!][!//
  [!VAR "DeadlineSupervisionStartConfigurationDMMin" = "WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStartRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStartRef)/../../WdgMSupervisedEntityId) = $ExtSEId))][1]/WdgMDeadlineMin"!][!//
  [!VAR "DeadlineSupervisionStartConfigurationDMStopRef" = "WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStartRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStartRef)/../../WdgMSupervisedEntityId) = $ExtSEId))][1]/WdgMDeadlineStopRef"!][!//
  [!VAR "IsPrevDeadlineSupervisionConfiguration" = "count(WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStopRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStopRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) > 0"!][!//
  [!VAR "DoGenerateCPId" = "'true'"!][!//
  [!IF "$IsPrevDeadlineSupervisionConfiguration"!][!//
    [!VAR "PrevDeadlineSupervisionConfiguration" = "WdgMDeadlineSupervision/*[((num:i(as:ref(WdgMDeadlineStopRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStopRef)/../../WdgMSupervisedEntityId) = $ExtSEId))][1]/WdgMDeadlineStartRef"!][!//
    [!VAR "PrevDeadlineSupervisionIntCPId" = "num:i(as:ref($PrevDeadlineSupervisionConfiguration)/WdgMCheckpointId)"!][!//
    [!VAR "PrevDeadlineSupervisionIntSEId" = "num:i(as:ref($PrevDeadlineSupervisionConfiguration)/../../@index)"!][!//
    [!VAR "IsPrevDeadlineSupervisionConfigurationNotGenerated" = "not(text:contains( text:split($ListOfGeneratedCPIds), $PrevDeadlineSupervisionIntCPId))"!][!//
    [!IF "$IsPrevDeadlineSupervisionConfigurationNotGenerated"!][!//
      [!VAR "IsContinueSimpleSortLoop" = "'true'"!][!//
      [!VAR "DoGenerateCPId" = "'false'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "$DoGenerateCPId = 'true'"!][!//
    [!VAR "ListOfGeneratedCPIds" = "concat($ListOfGeneratedCPIds, ' ', $IntCPId)"!][!//
/* deadline configuration for start CP [!"$CPName"!] in mode [!"$IntModeId"!] */
static CONST(WdgM_EB_CPDeadlineCfgType,WDGM_CONST) WdgM_EB_CPDeadlineCfg_[!"$IntSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!] =
{
  [!"num:i(round(num:f(num:div($DeadlineSupervisionStartConfigurationDMMax, $TimeGranularity))))"!]U, /* maximum Deadline in granularity of [!"num:i(num:mul($TimeGranularity, 1000000))"!]us */
  [!"num:i(round(num:f(num:div($DeadlineSupervisionStartConfigurationDMMin, $TimeGranularity))))"!]U, /* minimum Deadline in granularity of [!"num:i(num:mul($TimeGranularity, 1000000))"!]us */
  [!"num:i(num:div(num:i(num:i(round(num:f(num:div($DeadlineSupervisionStartConfigurationDMMax, $TimeGranularity)))) + $MainFunctionCycleTime - 1), $MainFunctionCycleTime))"!]U, /* maximum allowed main function calls until deadline violation */
  [!IF "$IsPrevDeadlineSupervisionConfiguration"!][!//
  &(WdgM_EB_CPDeadlineCfg_[!"$PrevDeadlineSupervisionIntSEId"!]_[!"$PrevDeadlineSupervisionIntCPId"!]_[!"$IntModeId"!]),
  [!ELSE!][!//
  NULL_PTR,
  [!ENDIF!][!//
  [!"num:i($DMId)"!]U, /* internal Deadline Monitoring ID for runtime data */
  [!"num:i($IntCPId)"!]U, /* Internal Checkpoint id of start checkpoint */
  [!"num:i(as:ref($DeadlineSupervisionStartConfigurationDMStopRef)/WdgMCheckpointId)"!]U, /* Internal Checkpoint id of stop checkpoint */
};
[!VAR "DMId" = "num:i($DMId + 1)"!][!//

  [!ENDIF!][!// DoGenerateCPId
[!ENDIF!][!// $IsDeadlineSupervisionStartConfiguration
[!ENDIF!][!// not(text:contains( text:split($ListOfGeneratedCPIds), $IntCPId))
[!ENDSELECT!][!//
[!ENDLOOP!][!// Checkpoints
[!IF "$IsContinueSimpleSortLoop = 'false'"!][!//
  [!BREAK!][!//
[!ENDIF!][!//
[!ENDFOR!][!// MaxSimpleSortLoop: Ensures that all Checkpoints are generated in worst-case
[!ENDLOOP!][!// WdgMModes

/* checkpoint configuration of SE [!"$SEName"!] */
static CONST(WdgM_EB_CPCfgType,WDGM_CONST) WdgM_EB_CPCfg_[!"$SEName"!][[!"num:i(count(WdgMCheckpoint/*))"!]] =
{
[!LOOP "WdgMCheckpoint/*"!][!//
[!VAR "IntCPId" = "WdgMCheckpointId"!][!//
[!VAR "IntSupervExists" = "WdgMCheckpointId"!][!//
[!VAR "IsInternalLogicalSupervisionConfigured" = "count(../../WdgMInternalTransition/*[((num:i(as:ref(WdgMInternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)) or ((num:i(as:ref(WdgMInternalTransitionSourceRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMInternalTransitionSourceRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) > 0"!][!//
  {
    /* CP [!"@name"!]*/
    {
      /* alive supervision per WdgM mode */
      [!LOOP "node:order(../../../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
      [!VAR "IntModeId" = "WdgMModeId"!][!//
      [!IF "count(WdgMAliveSupervision/*[(num:i(as:ref(WdgMAliveSupervisionCheckpointRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMAliveSupervisionCheckpointRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]) = 1"!][!//
      &(WdgM_EB_CPAliveCfg_[!"$IntSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!]),
      [!ELSE!][!//
      NULL_PTR,
      [!ENDIF!][!//
      [!ENDLOOP!][!// WdgMMode
    },
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
    /* deadline monitoring per WdgM mode */
    {
      [!LOOP "node:order(../../../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
        [!VAR "IntModeId" = "WdgMModeId"!][!//
        [!VAR "IsDeadlineSupervisionStartConfiguration" = "count(WdgMDeadlineSupervision/*[(num:i(as:ref(WdgMDeadlineStartRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStartRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]) > 0"!][!//
        [!VAR "IsDeadlineSupervisionStopConfiguration" = "count(WdgMDeadlineSupervision/*[(num:i(as:ref(WdgMDeadlineStopRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStopRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]) > 0"!][!//
        [!IF "($IsDeadlineSupervisionStartConfiguration = 'true') or ($IsDeadlineSupervisionStopConfiguration = 'true')"!][!//
          [!IF "$IsDeadlineSupervisionStartConfiguration"!]
            [!VAR "IntStartCPId" = "$IntCPId"!][!//
            [!VAR "IntStartSEId" = "$IntSEId"!][!//
          [!ELSE!][!//
            [!VAR "DeadlineSupervisionConfiguration" = "WdgMDeadlineSupervision/*[(num:i(as:ref(WdgMDeadlineStopRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMDeadlineStopRef)/../../WdgMSupervisedEntityId) = $ExtSEId)]/WdgMDeadlineStartRef"!][!//
            [!VAR "IntStartCPId" = "num:i(as:ref($DeadlineSupervisionConfiguration)/WdgMCheckpointId)"!][!//
            [!VAR "IntStartSEId" = "num:i(as:ref($DeadlineSupervisionConfiguration)/../../@index)"!][!//
          [!ENDIF!][!//
      &(WdgM_EB_CPDeadlineCfg_[!"$IntStartSEId"!]_[!"$IntStartCPId"!]_[!"$IntModeId"!]),
        [!ELSE!][!//
      NULL_PTR,
        [!ENDIF!][!//
      [!ENDLOOP!][!// WdgMMode
    },
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
    /* internal logical supervision per WdgM mode */
    {
      [!LOOP "node:order(../../../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
        [!VAR "IntModeId" = "WdgMModeId"!][!//
        [!VAR "IsExternalLogicalSupervisionConfigured" = "count(WdgMExternalLogicalSupervision/*/WdgMExternalTransition/*[((num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)) or ((num:i(as:ref(WdgMExternalTransitionSourceRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionSourceRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) > 0"!][!//
        [!VAR "SupervisedEntityIsReferencedInMode" = "count(WdgMLocalStatusParams/*[(num:i(as:ref(WdgMLocalStatusSupervisedEntityRef)/WdgMSupervisedEntityId)) = $ExtSEId]) > 0"!][!//
        [!IF "($IsInternalLogicalSupervisionConfigured = 'true') and ($SupervisedEntityIsReferencedInMode = 'true')"!][!//
      &(WdgM_EB_CPIntLogicalCfg_[!"$IntSEId"!]_[!"$IntCPId"!]),
        [!ELSE!][!//
      NULL_PTR,
        [!ENDIF!][!//
      [!ENDLOOP!][!// WdgMMode
    },
    /* external logical supervision per WdgM mode */
    {
      [!LOOP "node:order(../../../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//
        [!VAR "IntModeId" = "WdgMModeId"!][!//
        [!VAR "IsExternalLogicalSupervisionConfigured" = "count(WdgMExternalLogicalSupervision/*/WdgMExternalTransition/*[((num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId) = $ExtSEId)) or ((num:i(as:ref(WdgMExternalTransitionSourceRef)/WdgMCheckpointId) = $IntCPId) and (num:i(as:ref(WdgMExternalTransitionSourceRef)/../../WdgMSupervisedEntityId) = $ExtSEId))]) > 0"!][!//
        [!VAR "SupervisedEntityIsReferencedInMode" = "count(WdgMLocalStatusParams/*[(num:i(as:ref(WdgMLocalStatusSupervisedEntityRef)/WdgMSupervisedEntityId)) = $ExtSEId]) > 0"!][!//
        [!IF "$IsExternalLogicalSupervisionConfigured"!][!//
      &(WdgM_EB_CPExtLogicalCfg_[!"$ExtSEId"!]_[!"$IntCPId"!]_[!"$IntModeId"!]),
        [!ELSE!][!//
      NULL_PTR,
        [!ENDIF!][!//
      [!ENDLOOP!][!// WdgMMode
    },
#endif
  },
[!ENDLOOP!][!// Checkpoints
};

[!/* Supervised Entity contains an internal graph => increment counter for graph id */!][!//
[!IF "count(WdgMInternalTransition/*) > 0"!][!//
[!VAR "BaseIntGraphId" = "num:i($BaseIntGraphId + 1)"!][!//
[!ENDIF!][!//
[!//
[!ENDLOOP!][!// Supervised Entities
#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>


/*==================[external constants]=========================================================*/

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_8BIT
#else
#define WDGM_START_SEC_CONST_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_InitialModeId =
    WdgMConf_WdgMMode_[!"name(as:ref(WdgMConfigSet/*[1]/WdgMInitialMode))"!];

[!IF "$SleepModeEnabled"!][!//
CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_DeInitModeId =
    WdgMConf_WdgMMode_[!"name(as:ref(WdgMConfigSet/*[1]/WdgMSleepMode))"!];

[!ENDIF!][!//

CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_ModeCfgSize = WDGM_EB_MODE_NUM;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_8BIT
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

/*------------------[mode configuration]------------------------------------*/
#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

CONST(WdgM_EB_ModeCfgType, WDGM_CONST) WdgM_EB_ModeCfg[WDGM_EB_MODE_NUM] =
{
  [!LOOP "node:order(WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!]
  { /* mode [!"name(.)"!] (id [!"WdgMModeId"!]) */
    [!CALL "getModeSpecificLCM", "modeRef" = "as:path(.)"!]U, /* least common multiple of all reference cycle values */
    [!IF "count(WdgMTrigger/*) > 0"!]
    WdgM_EB_TrigCfg_M[!"WdgMModeId"!], /* trigger configuration */
    [!ELSE!]
    NULL_PTR, /* no triggers in this mode */
    [!ENDIF!]
    [!"WdgMExpiredSupervisionCycleTol"!]U, /* number of supervision cycles in stats EXPIRED */
    [!"num:i(count(WdgMTrigger/*))"!]U /* number of watchdogs (triggers) */
  },
  [!ENDLOOP!][!// WdgMMode/*
};

/* -----------------[SE configuration]--------------------- */

CONST(WdgM_EB_SECfgType,WDGM_CONST) WdgM_EB_SECfg[WDGM_EB_SE_NUM] =
{
  [!LOOP "WdgMGeneral/WdgMSupervisedEntity/*"!]
  [!VAR "ExtSEId" = "num:i(WdgMSupervisedEntityId)"!]
  { /* SE [!"@name"!] (internal SEId: [!"@index"!]) */
    WdgM_EB_CPCfg_[!"@name"!], /* pointer to the array of checkpoints */
    WdgM_EB_CPData_[!"@name"!], /* pointer to the run-time data of the CPs */
    WdgMConf_WdgMSupervisedEntity_[!"@name"!], /* external SEId */
    [!"num:i(count(WdgMCheckpoint/*))"!]U, /* number of CPs in this SE */
    [!IF "WdgMErrorRecoveryEnabled = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* Support for Error Recovery */
    /* number of tolerated reference cycles which are allowed to fail in each WdgM mode*/
    {
      [!LOOP "node:order(../../../WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!]
      [!IF "count(WdgMLocalStatusParams/*[as:ref(WdgMLocalStatusSupervisedEntityRef)/WdgMSupervisedEntityId = $ExtSEId]) > 0"!]
      [!"WdgMLocalStatusParams/*[as:ref(WdgMLocalStatusSupervisedEntityRef)/WdgMSupervisedEntityId = $ExtSEId]/WdgMFailedAliveSupervisionRefCycleTol"!]U, /* WdgM mode [!"@name"!] */
      [!ELSE!]
      WDGM_EB_DUMMY_FAILEDREFCYCLETOL, /* WdgM mode [!"@name"!] */
      [!ENDIF!]
      [!ENDLOOP!][!// /*
    [!IF "$WdgMMulticoreEnabled"!]
    },
    [!"WdgMSupervisedEntityCoreId"!]U, /* the core id of a SE */
  },
    [!ELSE!]
    }
  },
    [!ENDIF!]
  [!ENDLOOP!][!//
};

/* Dummy module configuration structure*/
CONST(WdgM_ConfigType, WDGM_APPL_CONST) WDGM_CONFIG_NAME = 0;

#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>


[!IF "$WdgMMulticoreEnabled"!]
/*============[WdgM Multi-Core internal data]===============*/
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_32BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* For multicore this variable is also used by the master to announce the satellites of the
 * result of the WdgMGetExpectedInitStateCallout(), so this is not under the
 * WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF compiler switch. */
/** \brief Expected initialization state of the WdgM module. */
VAR(Atomic_t, WDGM_VAR) WdgM_EB_ExpectedInitState = ATOMICS_OBJECT_INITIALIZER(WDGM_EB_INIT_STATUS_DEINIT);

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable2,1 */
/** \brief WdgM global supervision status */
VAR(Atomic_t, WDGM_VAR) WdgM_EB_GlobalStatus = ATOMICS_OBJECT_INITIALIZER(WDGM_GLOBAL_STATUS_DEACTIVATED);

/* !LINKSTO WDGM.EB.MulticoreInit.4,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable15,1 */
/** \brief WdgM global initialization status */
VAR(WdgM_EB_GlobalInitStatusType, WDGM_VAR) WdgM_EB_GlobalInitStatus = ATOMICS_OBJECT_INITIALIZER(WDGM_EB_GLOBAL_STATUS_DEINIT);

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_32BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_32BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/** \brief through this variable, the WdgM master instance announces the WdgM satellite instances that the mode switch has been performed.
  * The WdgM satellite instances read the value of this variable to change and update their mode. */
VAR(Atomic_t, WDGM_VAR) WdgM_EB_CoreSwitchMode[WDGM_EB_NUMBER_OF_CORES];

/* !LINKSTO WDGM.ASR40.WDGM200,1 */
/* !LINKSTO WDGM.ASR40.WDGM085.3,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable1,2 */
/* !LINKSTO WDGM.EB.MulticoreLocalStatus2,1 */
/** \brief defines the local status of all SE for all WdgM core instances */
VAR(Atomic_t, WDGM_VAR) WdgM_EB_SELocalStatus[WDGM_EB_SE_NUM];

/** \brief has the expired SEId been set? */
VAR(Atomic_t, WDGM_VAR) WdgM_EB_IsExpiredSEIdSet;

#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
VAR(Atomic_t, WDGM_VAR) WdgM_EB_ExpectedWdgMModeId;
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_32BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_GLOBAL_UNSPECIFIED
#elif (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable12,1,WDGM.EB.DesignDecision.InternalVariable13,1 */
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[WDGM_EB_EXTERNAL_GRAPH_NUM];
[!FOR "index_i" = "0" TO "num:i($NumberOfExternalGraphs)-1"!][!//
/** \brief run-time data for each external graph to store the sequence
  *  number of a checkpoint for each supervised entity */
static VAR(WdgM_EB_ExtGraphDataSeqArrayType, WDGM_VAR) WdgM_EB_ExtGraphDataSeq_[!"num:i($index_i)"!][WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!]];

[!ENDFOR!][!//
#else
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[1];
#endif

#if (WDGM_EB_INTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable6,2,WDGM.EB.DesignDecision.InternalVariable7,2 */
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[WDGM_EB_INTERNAL_GRAPH_NUM];
#else
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[1];
#endif
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_GLOBAL_UNSPECIFIED
#elif (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_UNSPECIFIED
#else
#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief array of pointers to the run-time data for each external graph and length of the pointed arrays */
CONST(WdgM_EB_ExtGraphDataSeqType,WDGM_CONST) WdgM_EB_ExtGraphDataSeqPtr[WDGM_EB_EXTERNAL_GRAPH_NUM] =
{
[!FOR "index_i" = "0" TO "num:i($NumberOfExternalGraphs)-1"!][!//
 /* pointer to the run-time data for each external graph and length of the array */
 { &WdgM_EB_ExtGraphDataSeq_[!"num:i($index_i)"!][0], WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!] },
[!ENDFOR!][!//
};
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_UNSPECIFIED
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/** \brief the Supervised Entity Id which first expired */
VAR(Atomic_t, WDGM_VAR_POWER_ON_INIT) WdgM_EB_ExpiredSEID = 0U;

#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

[!VAR "SupEntExistPerCore" = "false()"!]
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!]
    [!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!//
      [!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!//
        [!VAR "SupEntExistPerCore" = "true()"!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$SupEntExistPerCore"!]

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_1_CORE_[!"num:i($index_i)"!]_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_1_CORE_[!"num:i($index_i)"!]_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>
[!ELSE!][!//
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>
[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!]

/** \brief Internal RAM data holding a copy of the alive counter values for each checkpoint */
static VAR(WdgM_EB_CPInternalDataType, WDGM_VAR) WdgM_EB_IntRAMData_Core[!"$index_i"!][WDGM_EB_CHECKPOINTS_NUM_CORE[!"$index_i"!]];

/* !LINKSTO WDGM.ASR40.WDGM085.3,1, WDGM.EB.DesignDecision.InternalVariable10,2 */
/** \brief failed reference cycle counter for the SE  */
static VAR(uint16, WDGM_VAR) WdgM_EB_FailedAliveSupervisionRefCycleCnt_Core[!"$index_i"!][WDGM_EB_SE_NUM_CORE[!"$index_i"!]];

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_1_CORE_[!"num:i($index_i)"!]_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_1_CORE_[!"num:i($index_i)"!]_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>

[!ELSE!][!//
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!]
    [!ENDIF!][!/* IF $SupEntExistPerCore */!]
    [!VAR "SupEntExistPerCore" = "false()"!]
[!ENDFOR!]
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!]
    [!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!//
      [!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!//
        [!VAR "SupEntExistPerCore" = "true()"!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$SupEntExistPerCore"!]

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#define WDGM_1_CORE_[!"num:i($index_i)"!]_START_SEC_VAR_INIT_ASIL_D_LOCAL_UNSPECIFIED
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>
[!ELSE!][!//
#define WDGM_START_SEC_VAR_INIT_ASIL_D_LOCAL_UNSPECIFIED
#include <WdgM_MemMap.h>
[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!]
/** \brief The WdgM core specific context data.
 ** This variable holds the core-specific run time data of the WdgM. */
/* Deviation MISRAC2012-2 */
static VAR(WdgM_EB_CoreContextType, AUTOMATIC) WdgM_EB_Core[!"num:i($index_i)"!]_Context =
{
  WdgM_EB_SELocalStatus,
  WdgM_EB_FailedAliveSupervisionRefCycleCnt_Core[!"num:i($index_i)"!],
  WdgM_EB_IntRAMData_Core[!"num:i($index_i)"!],
  0U,
  0U,
  FALSE,
#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
  0U,
#endif
  [!"num:i($index_i)"!]U
};

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#define WDGM_1_CORE_[!"num:i($index_i)"!]_STOP_SEC_VAR_INIT_ASIL_D_LOCAL_UNSPECIFIED
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>

[!ELSE!][!//
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_LOCAL_UNSPECIFIED
#include <WdgM_MemMap.h>

[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!]
    [!ENDIF!][!/* IF $SupEntExistPerCore */!]
    [!VAR "SupEntExistPerCore" = "false()"!]
[!ENDFOR!]

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/* Deviation MISRAC2012-1 */
FUNC(WdgM_EB_CoreContextType *, WDGM_CODE) WdgM_GetCoreInstance(void)
{
  P2VAR(WdgM_EB_CoreContextType, WDGM_VAR, WDGM_APPL_DATA) inst = NULL_PTR;

  const WdgM_EB_CoreIdType coreID = WDGM_EB_GET_CORE_ID();

  switch(coreID)
  {
    [!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!]
      [!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!//
        [!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!//
          [!VAR "SupEntExistPerCore" = "true()"!]
        [!ENDIF!]
      [!ENDLOOP!]
     [!IF "$SupEntExistPerCore"!]
    case [!"num:i($index_i)"!]:
    {
      inst = &WdgM_EB_Core[!"num:i($index_i)"!]_Context;
      break;
    }
     [!ENDIF!][!/* IF $SupEntExistPerCore */!]
     [!VAR "SupEntExistPerCore" = "false()"!]
    [!ENDFOR!]
    default:
    {
      /* No WdgM instance exists for core. */
      break;
    }
  }

  return inst;
}

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

[!IF "$DefaultServiceAPI = 'NONE'"!][!//
  [!FOR "Index" = "0" TO "$MULTICORE_NUM_CORES - 1"!]
    [!IF "$Index != $WDGM_MASTER_CORE_ID"!]
#define WDGM_1_CORE_[!"num:i($Index)"!]_START_SEC_CODE_ASIL_D
#include <WdgM_1_CORE_[!"num:i($Index)"!]_MemMap.h>
FUNC(void, WDGM_1_CORE_[!"num:i($Index)"!]_CODE) WdgM_1_CORE_[!"num:i($Index)"!]_MainFunction(void)
{
   WdgM_MainFunction();
}
#define WDGM_1_CORE_[!"num:i($Index)"!]_STOP_SEC_CODE_ASIL_D
#include <WdgM_1_CORE_[!"num:i($Index)"!]_MemMap.h>

    [!ENDIF!]
  [!ENDFOR!]
[!ENDIF!]
[!ELSE!][!/* IF $WdgMMulticoreEnabled */!][!//

/*============[WdgM internal data]===============*/
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_8BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
/** \brief Expected initialization state of the WdgM module */
VAR(WdgM_EB_InitStatusType, WDGM_VAR) WdgM_EB_ExpectedInitState = WDGM_EB_INIT_STATUS_DEINIT;
#endif

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable2,1 */
/** \brief WdgM global supervision status */
VAR(WdgM_GlobalStatusType, WDGM_VAR) WdgM_EB_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_8BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_8BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.ASR40.WDGM200,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable1,2 */
/** \brief defines the local statuses of all SE*/
VAR(WdgM_LocalStatusType, WDGM_VAR) WdgM_EB_SELocalStatus[WDGM_EB_SE_NUM];

/** \brief has the expired SEId been set? */
VAR(boolean, WDGM_VAR) WdgM_EB_IsExpiredSEIdSet;

#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
VAR(WdgM_EB_ModeType, WDGM_VAR) WdgM_EB_ExpectedWdgMModeId;
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_8BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/** \brief Internal RAM data holding a copy of the alive counter values for each checkpoint */
VAR(WdgM_EB_CPInternalDataType, WDGM_VAR) WdgM_EB_IntRAMData[WDGM_EB_CHECKPOINTS_NUM];

/** \brief failed reference cycle counter for the SE  */
VAR(uint16, WDGM_VAR) WdgM_EB_FailedAliveSupervisionRefCycleCnt[WDGM_EB_SE_NUM];

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_VAR_POWER_ON_INIT_ASIL_D_16
#include <WdgM_MemMap.h>

/** \brief the Supervised Entity Id which first expired */
VAR(WdgM_EB_ExpiredSEIDType, WDGM_VAR_POWER_ON_INIT) WdgM_EB_ExpiredSEID = 0U;

#define WDGM_STOP_SEC_VAR_POWER_ON_INIT_ASIL_D_16
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_32BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_32
#endif
#include <WdgM_MemMap.h>

/** \brief WdgM global initialization status */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable15,1 */
VAR(WdgM_EB_GlobalInitStatusType, WDGM_VAR) WdgM_EB_GlobalInitStatus = WDGM_EB_GLOBAL_STATUS_DEINIT;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_32BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_32
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>
/** \brief The WdgM specific context data.
 ** This variable holds the specific run time data of the WdgM. */
VAR(WdgM_EB_CoreContextType, AUTOMATIC) WdgM_EB_Context =
{
  WdgM_EB_SELocalStatus,
  WdgM_EB_FailedAliveSupervisionRefCycleCnt,
  WdgM_EB_IntRAMData,
  0U,
  0U,
  FALSE,
#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
  0U,
#endif
  0U
};

#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable12,1,WDGM.EB.DesignDecision.InternalVariable13,1 */
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[WDGM_EB_EXTERNAL_GRAPH_NUM];
[!FOR "index_i" = "0" TO "num:i($NumberOfExternalGraphs)-1"!][!//
/** \brief run-time data for each external graph to store the sequence
  *  number of checkpoint for each supervised entity */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable16,1 */
static VAR(WdgM_EB_ExtGraphDataSeqArrayType, WDGM_VAR) WdgM_EB_ExtGraphDataSeq_[!"num:i($index_i)"!][WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!]];

[!ENDFOR!][!//
#else
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[1];
#endif

#if (WDGM_EB_INTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable6,2,WDGM.EB.DesignDecision.InternalVariable7,2 */
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[WDGM_EB_INTERNAL_GRAPH_NUM];
#else
VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[1];
#endif
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_UNSPECIFIED
#else
#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief array of pointers to the run-time data for each external graph and length of the pointed arrays */
CONST(WdgM_EB_ExtGraphDataSeqType,WDGM_CONST) WdgM_EB_ExtGraphDataSeqPtr[WDGM_EB_EXTERNAL_GRAPH_NUM] =
{
[!FOR "index_i" = "0" TO "num:i($NumberOfExternalGraphs)-1"!][!//
 /* pointer to the run-time data for each external graph and length of the array */
 { &WdgM_EB_ExtGraphDataSeq_[!"num:i($index_i)"!][0], WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!] },
[!ENDFOR!][!//
};
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_UNSPECIFIED
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

[!ENDIF!][!/* IF $WdgMMulticoreEnabled */!]

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

/*==================[end of file]================================================================*/
[!ENDCODE!][!//
