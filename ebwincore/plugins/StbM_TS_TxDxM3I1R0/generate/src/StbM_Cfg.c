/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W585
 * W585: division by zero
 *
 * Reason:
 * STBM_GPT_CONV_TICKS_TO_NS macro expands into 
 * #define STBM_GPT_CONV_TICKS_TO_NS(x, f)       (f <= 1000000000U)?(x*(1000000000U/f)):(x/(f/1000000000U))
 * where f is equal to the following expression
 * num:i(floor((node:value(./StbMLocalTimeClock/StbMClockFrequency)) div (node:value(./StbMLocalTimeClock/StbMClockPrescaler))))
 * Situation 1: when (f <= 1000000000U) and (x*(1000000000U/f)) is reached
 * f, can never be 0, because StbMClockFrequency and StbMClockPrescaler cannot be configured to 0 and the type of StbMClockFrequency and StbMClockPrescaler is integer,
 * which means that even if the worst case happens and their values are close or even more than that, equal,
 * the div operation will be evaluated to at least 1 and no division by 0 will happen.
 * Situation 2: when (f > 1000000000U) and (x/(f/1000000000U)) is reached
 * f, can never be smaller or equal with 1000000000U (check condition (f <= 1000000000U)),
 * which means that when evaluating (x/(f/1000000000U)), no division by 0 will happen.
 */

[!AUTOSPACING!]
[!INCLUDE "../include/StbM_Vars.m"!]
[!INCLUDE "../include/StbM_TimeBase.m"!]

/** \addtogroup Synchronized Time Base Manager
 ** @{ */

/*==[Includes]================================================================*/

#include <StbM_Version.h>

/* to prevent duplicate declarations of symbols from Rte_StbM.h by our public headers */
#if (defined STBM_INTERNAL_USAGE)
#error STBM_INTERNAL_USAGE already defined
#endif
#define STBM_INTERNAL_USAGE
#include <StbM_Int_Cfg.h>

[!/* get the header file list of each connected BSW module*/!][!//
[!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
  [!IF "(node:name(.) != 'CanTSyn') and (node:name(.) != 'FrTSyn') and (node:name(.) != 'EthTSyn')"!]
    [!LOOP "asc:getIncludes('StbM', node:name(.), 'ForUpperLayer', 'If')"!]
#include <[!"."!]>
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDLOOP!]

[!IF "($StbMRteUsed = 'true') and (($StbMTimeRecordingUsed = 'true') or ($StbMTimeBaseNotificationStatus = 'true') or ($StbMNotificationOfCustomersUsed = 'true') or ($StbMTimeValidationUsed = 'true'))"!]

/* include Rte header for the RTE calls. */
/* !LINKSTO StbM.dsn.RteInclusion,1 */
[!VAR "EcuCPartitionName" = "text:split($ListOfEcucPartitionNames, ';')[1]"!]
[!IF "$EcuCPartitionName = '_NoPartition'"!]
 [!VAR "EcuCPartitionName" = "''"!]
[!ENDIF!]
#include <Rte_StbM[!"$EcuCPartitionName"!].h>

[!ENDIF!]

[!IF "($isStbMExternalsHeaderFileNeeded = 'true')"!]
/* include StbM_Externals.h header file, which contains the prototypes of all needed callbacks */
/* !LINKSTO StbM.EB.SyncTimeRecording.Used.StbM_Externals.HeaderFile.Included,1 */
/* !LINKSTO StbM.EB.OffsetTimeRecording.Used.StbM_Externals.HeaderFile.Included,1 */
/* !LINKSTO StbM.EB.StbMNotificationInterface.CALLBACK.StbM_Externals.HeaderFile.Included,1 */
/* !LINKSTO StbM.EB.StbMNotificationInterface.CALLBACK_AND_SR_INTERFACE.StbM_Externals.HeaderFile.Included,1 */
/* !LINKSTO StbM.EB.StbMNotificationCustomer.StbM_Externals.HeaderFile.Included,1 */
#include <StbM_Externals.h>
[!ENDIF!]


[!IF "(($StbMTriggeredCustomersNumber > 0) or ($StbMGptUsage = 'true'))"!]
/* include StbM_Int header because STBM_UNREACHABLE_CODE_ASSERT definition is needed. */
#include <StbM_Int.h>
[!ENDIF!]


/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/* !LINKSTO StbM.dsn.PrecompileTimeConfig,1 */

#define STBM_START_SEC_CONFIG_DATA_8
#include <StbM_MemMap.h>

[!/* create the index list for all time bases with connected masters for handling*/!][!//
[!LOOP "text:split($MasterList)"!]
  [!VAR "tmpTimeBaseId" = "text:split( ., ':' )[1]"!]
  [!VAR "tmpTimeBaseName" = "text:split( ., ':' )[2]"!]
  [!VAR "tmpNumberOfMaster" = "text:split( ., ':' )[3]"!]
/* List of connected BSW modules with master to handle for time base with Id [!"$tmpTimeBaseId"!] */
STATIC CONST(uint8, STBM_APPL_CONST) StbM_TimeSetterIndexList_[!"$tmpTimeBaseName"!][[!"$tmpNumberOfMaster"!]] =
{
  [!FOR "i"="4" TO "$tmpNumberOfMaster + 3"!]
  [!"text:split( ., ':' )[number($i)]"!]U,
  [!ENDFOR!]
};
[!ENDLOOP!]

[!IF "($noSetModuleInList = 'true')"!]
[!/* create the index list (always 1 entry) for the internal behaviour if used*/!][!//
/* Index of internal setter function */
STATIC CONST(uint8, STBM_APPL_CONST) StbM_TimeSetterIndexList_Internal[1] =
{
  [!"num:i($StbMBswModulesNumber - 1)"!]U
};
[!ENDIF!]

#define STBM_STOP_SEC_CONFIG_DATA_8
#include <StbM_MemMap.h>

#define STBM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <StbM_MemMap.h>

[!IF "$StbMTriggeredCustomersNumber > 0"!]
/** Triggered customer configuration */
CONST(StbM_TriggerCustomerCfgType, STBM_APPL_CONST) StbM_TriggerCustomerCfg[STBM_NUMBER_OF_TRIGGERED_CUSTOMERS] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMTriggeredCustomer/*, 'node:ref(./StbMSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier')"!]
  {
[!/* express the configured time in number of Mainfunction invocations (round-up)*/!][!//
    /* !LINKSTO StbM.ASR44.SWS_StbM_00093,1 */
    [!"num:i(ceiling(node:value(StbMTriggeredCustomerPeriod) div (as:modconf('StbM')[1]/StbMGeneral/StbMMainFunctionPeriod * 1000000)))"!]U,  /* invocationNumber */
    [!"node:value(node:ref(StbMOSScheduleTableRef)/OsScheduleTableDuration)"!]U,  /* scheduleTblDuration */
    [!"node:name(node:ref(StbMOSScheduleTableRef))"!],  /* scheduleTableIndex */
    [!"node:value(node:ref(StbMSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier)"!]U  /* SyncTimeBaseIndex */
  },
[!ENDLOOP!]
};
[!ENDIF!]



[!IF "$StbMNotificationOfCustomersUsed = 'true'"!]
/** Notification of customers configuration */
CONST(StbM_CustomerNotificationCfgType, STBM_APPL_CONST) StbM_CustomerNotificationCfg[STBM_NUMBER_OF_NOTIFICATION_CUSTOMERS] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(count(StbMNotificationCustomer/*) > 0)], './StbMSynchronizedTimeBaseIdentifier')"!]
  [!VAR "timeBaseId" = "./StbMSynchronizedTimeBaseIdentifier"!]
  [!VAR "timeBaseName" = "node:name(.)"!]
  /* [!"node:name(.)"!] */
  [!LOOP "node:order(StbMNotificationCustomer/*, './StbMNotificationCustomerId')"!]
    [!VAR "customerId" = "./StbMNotificationCustomerId"!]
    [!VAR "customerName" = "node:name(.)"!]
  { /* [!"node:name(.)"!] */
    [!/* get the time base Id */!][!//
    [!"$timeBaseId"!]U,  /* time base Id */
    [!/* get the customer Id */!][!//
    [!"$customerId"!]U,  /* customer Id */
    [!IF "node:exists(StbMTimeNotificationCallback) = 'true'"!]
      [!/* !LINKSTO StbM.ASR19-11.SWS_StbM_00274,1 */!][!//
      [!CALL "GetTimeNotificationCallbackName", "customer" = "node:path(.)"!]
    [!/* generate the TimeNotification Callback */!][!//
    /* !LINKSTO StbM.ASR19-11.SWS_StbM_00273,1 */
    &[!"$timeNotificationCallbackName"!],  /* customerNotificationCbkFctPtr */
    [!ELSE!]
    [!/* generate the TimeNotification C-S Interface */!][!//
    &Rte_Call_StbM[!"$EcuCPartitionName"!]_GlobalTime_TimeEvent_[!"$timeBaseName"!]_[!"$customerName"!]_NotifyTime,  /* customerNotificationRteCbkFctPtr */
    [!ENDIF!]
  },
  [!ENDLOOP!]
[!ENDLOOP!]
};
[!ENDIF!]



[!VAR "nrOfTimeSetter" = "num:i(0)"!]
[!VAR "indexOfGptTimeBases" = "num:i(0)"!]
[!/* create the configuration of all sync time bases*/!][!//
/* Synchronized time base(s) configuration */
CONST(StbM_SyncTimeBaseCfgType, STBM_APPL_CONST) StbM_SyncTimeBaseCfg[STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
  [!/* get the name of the function for the Virtual Local Time source */!][!//
  [!IF "node:exists(./StbMOffsetTimeBase)"!]
  NULL_PTR,  /* virtualLocalTimeGetterFctPtr */
  [!ELSE!]
  [!IF "((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware)))"!]
  [!IF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration)"!]
  &StbM_GetCurrentVirtualLocalTimeGpt,  /* virtualLocalTimeGetterFctPtr */
  [!ELSEIF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../OsCounter)"!]
  &StbM_GetCurrentVirtualLocalTime_BasedOn_OsCounter,  /* virtualLocalTimeGetterFctPtr */
  [!ELSE!]
  &StbM_GetCurrentVirtualLocalTimeEthTSyn,  /* virtualLocalTimeGetterFctPtr */
  [!ENDIF!]
  [!ELSE!]
  &StbM_GetCurrentVirtualLocalTime_Using_OSGetTimeStamp,  /* virtualLocalTimeGetterFctPtr */
  [!ENDIF!]
  [!ENDIF!]
  
  [!/* get the name of the GetCurrentTime function if we have a global time reference
     Error checks are done in StbM_Vars.m.
  */!][!//
  [!VAR "numberOfMaster" = "0"!]
  [!VAR "timeBaseId" = "./StbMSynchronizedTimeBaseIdentifier"!]
  [!VAR "tmpTimeBaseName" = "''"!]
  [!VAR "BswModuleName" = "''"!]
  [!LOOP "text:split($MasterList)"!]
[!/* extract the entry for the actual time base Id from the 'MasterList' variable.*/!][!//
    [!VAR "tmpTimeBaseId" = "text:split( ., ':' )[1]"!]
    [!IF "$tmpTimeBaseId = $timeBaseId"!]
      [!VAR "tmpTimeBaseName" = "text:split( ., ':' )[2]"!]
      [!VAR "numberOfMaster" = "text:split( ., ':' )[3]"!]
      [!VAR "BswModuleName" = "text:split( ., ':' )[5]"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!IF "node:exists(./StbMOffsetTimeBase)"!]
[!/* use the internal getter */!][!//
  &StbM_GetCurrentTimeInternal,  /* timeGetterFctPtr */
  [!ELSE!]
  [!VAR "set" = "concat($BswModuleName, '.AdjacentLayerConfig.ApiName.V1.GetCurrentTime')"!]
  [!IF "ecu:has($set)"!]
  &[!"asc:getApiFunctionName('', $BswModuleName, 'GetCurrentTime')"!], /* timeGetterFctPtr */
  [!ELSE!][!//
[!/* use the internal getter */!][!//
  &StbM_GetCurrentTimeInternal,  /* timeGetterFctPtr */
  [!ENDIF!]
  [!ENDIF!]
[!/* If there are masters --> store the pointer to the index table (and their number)*/!][!//
  [!IF "$numberOfMaster > 0"!]
    &StbM_TimeSetterIndexList_[!"$tmpTimeBaseName"!][0],  /* timeSetterIndexPtr */
    [!VAR "nrOfTimeSetter" = "num:i($numberOfMaster)"!]
  [!ELSE!][!//
[!/* If there are no masters --> store the internal setter */!][!//
    &StbM_TimeSetterIndexList_Internal[0],  /* timeSetterIndexPtr */
    [!VAR "nrOfTimeSetter" = "num:i(1)"!]
  [!ENDIF!]
    [!"$nrOfTimeSetter"!]U, /* numberOfTimeSetter */
  [!IF "node:exists(./StbMOffsetTimeBase)"!]
    STBM_OFFSET_TIME_BASE_NO_ETHTSYN_AND_OS_USED, /* timeBaseLocalTimeIdx */
  [!ELSE!]
  [!IF "((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware)))"!]
  [!IF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../OsCounter)"!][!//
    STBM_TIME_BASE_USES_OS, /* timeBaseLocalTimeIdx */
  [!ELSEIF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration)"!][!//
    [!"$indexOfGptTimeBases"!]U, /* timeBaseLocalTimeIdx */
    [!VAR "indexOfGptTimeBases" = "num:i($indexOfGptTimeBases + 1)"!]
  [!ELSE!]
    [!"node:value(node:ref(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/EthTSynGlobalTimeEthIfRef)/EthIfCtrlIdx)"!]U, /* timeBaseLocalTimeIdx */
  [!ENDIF!]
  [!ELSE!]
    STBM_TIME_BASE_USES_OS, /* timeBaseLocalTimeIdx */
  [!ENDIF!]
  [!ENDIF!]
  
  [!IF "node:exists(./StbMOffsetTimeBase)"!]
    STBM_DUMMY_ROLE_OFFSET_TIMEBASE, /* dummy role used for offset */
  [!ELSE!]
  [!IF "node:value(./StbMSynchronizedTimeBaseIdentifier) < 16"!]
  [!VAR "additionalPassiveMaster" = "num:i(0)"!]
  [!LOOP "text:split($AllMastersList, ':')"!]
[!/* check if there are passive Masters for this timebase.*/!][!//
    [!VAR "tmpTimeBaseId" = "text:split( ., ':' )[1]"!]
    [!IF "$tmpTimeBaseId = $timeBaseId"!]
      [!VAR "additionalPassiveMaster" = "num:i(1)"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!/* store if this time base is connected to a slave (necessary for DET checks only)*/!][!//
  [!VAR "slaveConnected" = "text:split( $SlaveList, ' ' )[number($timeBaseId) + 1]"!]
   /* !LINKSTO StbM.dsn.Roles,1 */
  [!IF "$slaveConnected = 'true'"!][!//
    [!IF "($numberOfMaster + $additionalPassiveMaster) > 0"!][!//
    STBM_ROLE_GATEWAY,  /* role */
    [!ELSE!][!//
    STBM_ROLE_SLAVE,  /* role */
    [!ENDIF!]
  [!ELSE!][!//
    STBM_ROLE_MASTER,  /* role */
  [!ENDIF!]
  [!ELSE!]
    STBM_ROLE_MASTER,  /* only master role is supported */
  [!ENDIF!]
  [!ENDIF!]
  },
[!ENDLOOP!]
};



[!IF "$StbMGptUsage = 'true'"!]
[!/* create array of Ids that use Gpt as virtual local time referance */!][!//
/* Time Bases that use Gpt as source for Virtual Local Time referance */
CONST(StbM_GptTimeBaseCfgType, STBM_APPL_CONST) StbM_GptTimeBases[STBM_NUMBER_OF_GPT_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[not(node:exists(StbMOffsetTimeBase))], './StbMSynchronizedTimeBaseIdentifier')"!]
   [!IF "((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware)))"!]
	[!IF "(node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../OsCounter)) or (node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../EthTSynGlobalTimeDomain))"!][!//
   [!/* nothing to do */!][!//
   [!ELSE!][!//
   { /* [!"node:name(.)"!] */
    [!"num:i(node:value(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/GptChannelTickValueMax))"!]U, /* gptMaxChannelTickValue */
    [!"num:i(node:value(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/GptChannelId))"!], /* gptChannelId */
   },
   [!ENDIF!]
	[!ELSE!]
	[!/* nothing to do */!][!//
	[!ENDIF!]
[!ENDLOOP!]
};
[!ENDIF!]



[!IF "$StbMOffsetTimebaseNumber > 0"!]
[!/* create the configuration of all offset time bases*/!][!//
/* Offset time base(s) configuration */
CONST(StbM_OffsetTimeBaseCfgType, STBM_APPL_CONST) StbM_OffsetTimeBaseCfg[STBM_NUMBER_OF_OFFSET_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)], './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
    [!"node:ref(StbMOffsetTimeBase)/StbMSynchronizedTimeBaseIdentifier"!]U  /* SyncTimeBaseIndex */
  },
[!ENDLOOP!]
};
[!ENDIF!]

#if (STBM_COMMON_FEATURES_ENABLED)
[!VAR "startIndexRcIntervals" = "num:i(0)"!]
[!VAR "endIndexRcIntervals" = "num:i(0)"!]
[!VAR "startIndexSyncBlocks" = "num:i(0)"!]
[!VAR "endIndexSyncBlocks" = "num:i(0)"!]

#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
/* variables for start and stop indexes, for StbM time bases linked to a CANTSYN Master time domain */
[!VAR "startIndexMasterCanTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexMasterCanTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a FRTSYN Master time domain */
[!VAR "startIndexMasterFrTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexMasterFrTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a ETHTSYN Master time domain */
[!VAR "startIndexMasterEthTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexMasterEthTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a ETHTSYN Pdelay Responder time domain */
[!VAR "startIndexPdelayResponderTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexPdelayResponderTimeValidationBlocks" = "num:i(0)"!]

/* variables for start and stop indexes, for StbM time bases linked to a CANTSYN Slave time domain */
[!VAR "startIndexSlaveCanTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexSlaveCanTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a FRTSYN Slave time domain */
[!VAR "startIndexSlaveFrTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexSlaveFrTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a ETHTSYN Slave time domain */
[!VAR "startIndexSlaveEthTSynTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexSlaveEthTSynTimeValidationBlocks" = "num:i(0)"!]
/* variables for start and stop indexes, for StbM time bases linked to a ETHTSYN Pdelay Initiator time domain */
[!VAR "startIndexPdelayInitiatorTimeValidationBlocks" = "num:i(0)"!]
[!VAR "endIndexPdelayInitiatorTimeValidationBlocks" = "num:i(0)"!]
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

[!VAR "startIndexOffsetBlocks" = "num:i(0)"!]
[!VAR "endIndexOffsetBlocks" = "num:i(0)"!]
[!VAR "intervalArrayIndex" = "num:i(0)"!]
[!VAR "masterRateCorrIndex" = "num:i(0)"!]
[!VAR "indexOfShareDataTimeBases" = "num:i(0)"!]
[!/* create the configuration of all common features of time bases*/!][!//
/* Common time base(s) configuration */
CONST(StbM_CommonTimeBaseCfgType, STBM_APPL_CONST) StbM_CommonTimeBaseCfg[STBM_NUMBER_OF_SYNC_TIMEBASES  + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
[!/* convert timeout in Main function invocation, if disabled take 0 as 'invalid value'*/!][!//
#if (STBM_TIMEOUTS_USED == STD_ON)
  [!IF "node:exists(StbMSyncLossTimeout)"!]
    [!"num:i(ceiling(node:value(StbMSyncLossTimeout) div as:modconf('StbM')[1]/StbMGeneral/StbMMainFunctionPeriod))"!]U,  /* timeoutValue */
  [!ELSE!][!//
    0U,  /* timeoutValue */
  [!ENDIF!]
#endif /* STBM_TIMEOUTS_USED == STD_ON */
#if (STBM_STATUS_NOTIFICATIONS_USED == STD_ON)
  /* !LINKSTO StbM.ASR44.SWS_StbM_00279,1 */
  /* !LINKSTO StbM.ASR44.ECUC_StbM_00045_Conf,1 */
  [!IF "node:exists(StbMStatusNotificationMask)"!]
    [!"num:i(node:value(StbMStatusNotificationMask))"!]U,  /* statusNotificationMask */
  [!ELSE!][!//
    0U,  /* statusNotificationMask */
  [!ENDIF!]
#endif /* STBM_STATUS_NOTIFICATIONS_USED == STD_ON */

#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON))
[!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
[!IF "(($isLinkedToSlaveCanTSyn = 'slaveCanTSyn') or ($isLinkedToSlaveFrTSyn = 'slaveFrTSyn') or ($isLinkedToSlaveEthTSyn = 'slaveEthTSyn') or ($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator'))"!]
[!IF "((node:exists(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)) and (node:value(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount) != 0)) or ((node:exists(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) and (node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) != 0))"!]
[!IF "(node:exists(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) and (node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) != 0)"!]
    [!VAR "endIndexSyncBlocks" = "num:i($startIndexSyncBlocks + num:i(node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) - 1)"!]
    [!"$startIndexSyncBlocks"!]U, /* startIndexTrBlocks */
[!VAR "startIndexSyncBlocks" = "num:i($endIndexSyncBlocks + 1)"!]
[!ELSE!]
    [!VAR "endIndexOffsetBlocks" = "num:i($startIndexOffsetBlocks + num:i(node:value(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)) - 1)"!]
    [!"$startIndexOffsetBlocks"!]U, /* startIndexTrBlocks */
[!VAR "startIndexOffsetBlocks" = "num:i($endIndexOffsetBlocks + 1)"!]
[!ENDIF!]
[!ELSE!][!//
    0U, /* startIndexTrBlocks */
[!ENDIF!]
[!ELSE!]
    0U, /* startIndexTrBlocks */
[!ENDIF!]
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON)) */
#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
[!IF "(node:exists(./StbMTimeValidation))"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!IF "(($isLinkedToMasterCanTSyn = 'masterCanTSyn') or ($isLinkedToMasterFrTSyn = 'masterFrTSyn') or ($isLinkedToMasterEthTSyn = 'masterEthTSyn') or ($isLinkedToMasterEthTSynPdelayResponder = 'masterEthTSynPdelayResponder'))"!]
    [!IF "($isLinkedToMasterCanTSyn = 'masterCanTSyn')"!]
      [!VAR "endIndexMasterCanTSynTimeValidationBlocks" = "num:i($startIndexMasterCanTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
      [!"$startIndexMasterCanTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationMASTERPart CanTSyn */
      [!VAR "startIndexMasterCanTSynTimeValidationBlocks" = "num:i($endIndexMasterCanTSynTimeValidationBlocks + 1)"!]
      0U, /* startIndexTrBlocksTimeValidationMASTERPdelayPart CanTSyn */
    [!ELSEIF "($isLinkedToMasterFrTSyn = 'masterFrTSyn')"!]
      [!VAR "endIndexMasterFrTSynTimeValidationBlocks" = "num:i($startIndexMasterFrTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
      [!"$startIndexMasterFrTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationMASTERPart FrTSyn */
      [!VAR "startIndexMasterFrTSynTimeValidationBlocks" = "num:i($endIndexMasterFrTSynTimeValidationBlocks + 1)"!]
      0U, /* startIndexTrBlocksTimeValidationMASTERPdelayPart FrTSyn */
    [!ELSE!]
      [!IF "($isLinkedToMasterEthTSyn = 'masterEthTSyn')"!]
       [!VAR "endIndexMasterEthTSynTimeValidationBlocks" = "num:i($startIndexMasterEthTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
       [!"$startIndexMasterEthTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationMASTERPart EthTSyn */
      [!VAR "startIndexMasterEthTSynTimeValidationBlocks" = "num:i($endIndexMasterEthTSynTimeValidationBlocks + 1)"!]
      
       [!IF "($isLinkedToMasterEthTSynPdelayResponder = 'masterEthTSynPdelayResponder')"!]
         [!VAR "endIndexPdelayResponderTimeValidationBlocks" = "num:i($startIndexPdelayResponderTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
         [!"$startIndexPdelayResponderTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationMASTERPart EthTSynPdelayResponder */
         [!VAR "startIndexPdelayResponderTimeValidationBlocks" = "num:i($endIndexPdelayResponderTimeValidationBlocks + 1)"!]
       [!ELSE!]
         0U, /* startIndexTrBlocksTimeValidationMASTERPdelayPart EthTSynPdelayResponder */
       [!ENDIF!]
      
      [!ENDIF!]
      
    [!ENDIF!]
  [!ELSE!]
    0U, /* startIndexTrBlocksTimeValidationMASTERPart */
    0U, /* startIndexTrBlocksTimeValidationMASTERPdelayPart */
  [!ENDIF!]
  [!IF "(($isLinkedToSlaveCanTSyn = 'slaveCanTSyn') or ($isLinkedToSlaveFrTSyn = 'slaveFrTSyn') or ($isLinkedToSlaveEthTSyn = 'slaveEthTSyn') or ($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator'))"!]
    [!IF "($isLinkedToSlaveCanTSyn = 'slaveCanTSyn')"!]
      [!VAR "endIndexSlaveCanTSynTimeValidationBlocks" = "num:i($startIndexSlaveCanTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
      [!"$startIndexSlaveCanTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationSLAVEPart CanTSyn */
      [!VAR "startIndexSlaveCanTSynTimeValidationBlocks" = "num:i($endIndexSlaveCanTSynTimeValidationBlocks + 1)"!]
      0U, /* startIndexTrBlocksTimeValidationSLAVEPdelayPart CanTSyn */
    [!ELSEIF "($isLinkedToSlaveFrTSyn = 'slaveFrTSyn')"!]
      [!VAR "endIndexSlaveFrTSynTimeValidationBlocks" = "num:i($startIndexSlaveFrTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
      [!"$startIndexSlaveFrTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationSLAVEPart FrTSyn */
      [!VAR "startIndexSlaveFrTSynTimeValidationBlocks" = "num:i($endIndexSlaveFrTSynTimeValidationBlocks + 1)"!]
      0U, /* startIndexTrBlocksTimeValidationSLAVEPdelayPart FrTSyn */
    [!ELSE!]
      [!IF "($isLinkedToSlaveEthTSyn = 'slaveEthTSyn')"!]
       [!VAR "endIndexSlaveEthTSynTimeValidationBlocks" = "num:i($startIndexSlaveEthTSynTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
       [!"$startIndexSlaveEthTSynTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationSLAVEPart EthTSyn */
       [!VAR "startIndexSlaveEthTSynTimeValidationBlocks" = "num:i($endIndexSlaveEthTSynTimeValidationBlocks + 1)"!]
      
       [!IF "($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator')"!]
         [!VAR "endIndexPdelayInitiatorTimeValidationBlocks" = "num:i($startIndexPdelayInitiatorTimeValidationBlocks + num:i(node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)) - 1)"!]
         [!"$startIndexPdelayInitiatorTimeValidationBlocks"!]U, /* startIndexTrBlocksTimeValidationSLAVEPart EthTSynPdelayInitiator */
         [!VAR "startIndexPdelayInitiatorTimeValidationBlocks" = "num:i($endIndexPdelayInitiatorTimeValidationBlocks + 1)"!]
       [!ELSE!]
         0U, /* startIndexTrBlocksTimeValidationSLAVEPdelayPart EthTSynPdelayInitiator */
       [!ENDIF!]
       
      [!ENDIF!]
    [!ENDIF!]
  [!ELSE!]
    0U, /* startIndexTrBlocksTimeValidationSLAVEPart */
    0U, /* startIndexTrBlocksTimeValidationSLAVEPdelayPart */
  [!ENDIF!]
[!ELSE!][!//
    0U, /* startIndexTrBlocksTimeValidationMASTERPart */
    0U, /* startIndexTrBlocksTimeValidationMASTERPdelayPart */
    0U, /* startIndexTrBlocksTimeValidationSLAVEPart */
    0U, /* startIndexTrBlocksTimeValidationSLAVEPdelayPart */
[!ENDIF!]
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */
[!/* convert timeleap value in a timestamp format , if disabled take 0 as 'invalid value'*/!][!//
#if (STBM_TIMELEAP_DETECTION_USED == STD_ON)
  [!IF "node:exists(StbMTimeLeapFutureThreshold)"!]
    [!VAR "maxValueSec1" = "4294967296"!]
    [!VAR "integerPartSec1" = "num:i(node:value(StbMTimeLeapFutureThreshold))"!]
    [!VAR "floatPartSec1" = "node:value(StbMTimeLeapFutureThreshold) - $integerPartSec1"!]
    [!"num:i($floatPartSec1 * 1000000000)"!]U,  /* timeleapFutureValueNs */
    [!"num:i($integerPartSec1 mod $maxValueSec1)"!]U,  /* timeleapFutureValueSec */
  [!ELSE!][!//
    0U,  /* timeleapFutureValueNs */
    0U,  /* timeleapFutureValueSec */
  [!ENDIF!]
  [!IF "node:exists(StbMTimeLeapPastThreshold)"!]
    [!VAR "maxValueSec2" = "4294967296"!]
    [!VAR "integerPartSec2" = "num:i(node:value(StbMTimeLeapPastThreshold))"!]
    [!VAR "floatPartSec2" = "node:value(StbMTimeLeapPastThreshold) - $integerPartSec2"!]
    [!"num:i($floatPartSec2 * 1000000000)"!]U,  /* timeleapPastValueNs */
    [!"num:i($integerPartSec2 mod $maxValueSec2)"!]U,  /* timeleapPastValueSec */
  [!ELSE!][!//
    0U,  /* timeleapPastValueNs */
    0U,  /* timeleapPastValueSec */
  [!ENDIF!]
[!/* clearTimeleapCount is increased by one additional unit, since,
     one value will be decreased from the counter in BusSetGlobalTime
     when a timeleap is detected
  */!][!//
  [!IF "node:exists(StbMClearTimeleapCount)"!]
    [!"num:i(node:value(StbMClearTimeleapCount) + 1)"!]U,  /* clearTimeleapCount */
  [!ELSE!][!//
    0U,  /* clearTimeleapCount */
  [!ENDIF!]
#endif /* STBM_TIMELEAP_DETECTION_USED == STD_ON */
#if (STBM_OFFSET_CORRECTION_USED == STD_ON)
[!CALL "GetTimeIntervalOffset", "timeBaseNode" = "node:path(.)"!]
    [!"$offsetCorrAdaptNs"!]U, /* offsetCorrectionAdaptIntervalNs */
    [!"$offsetCorrAdaptSec"!]U, /* offsetCorrectionAdaptIntervalSec */
    [!"$offsetCorrAdaptNsLo"!]U, /* offsetCorrectionAdaptIntervalNsLo */
    [!"$offsetCorrAdaptNsHi"!]U, /* offsetCorrectionAdaptIntervalNsHi */
    [!"$offsetCorrJumpNs"!]U, /* offsetCorrectionJumpThresholdNs */
    [!"$offsetCorrJumpSec"!]U, /* offsetCorrectionJumpThresholdSec */
#endif /* STBM_OFFSET_CORRECTION_USED == STD_ON */
#if (STBM_SLAVE_RATE_CORRECTION_USED == STD_ON)
[!CALL "GetTimeIntervalRate", "timeBaseNode" = "node:path(.)"!]
    [!"$rateCorrNs"!]U, /* rateCorrectionInterval - nanoseconds part */
    [!"$rateCorrSec"!]U, /* rateCorrectionInterval - seconds part */
    [!"$doubleTheRateCorrNs"!]U, /* doubleTheRateCorrectionInterval - nanoseconds part */
    [!"$doubleTheRateCorrSec"!]U, /* doubleTheRateCorrectionInterval - seconds part */
#endif /* STBM_SLAVE_RATE_CORRECTION_USED == STD_ON */
#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON))
  /* !LINKSTO StbM.ASR44.SWS_StbM_00382,1 */
  /* !LINKSTO StbM.ASR44.SWS_StbM_00383,1 */

[!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
[!IF "(($isLinkedToSlaveCanTSyn = 'slaveCanTSyn') or ($isLinkedToSlaveFrTSyn = 'slaveFrTSyn') or ($isLinkedToSlaveEthTSyn = 'slaveEthTSyn') or ($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator'))"!]
[!IF "node:exists(./StbMTimeRecording)"!]
  [!IF "node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) != 0"!]
    [!"node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)"!]U, /* numberOfBlocks */
  [!ELSE!]
    [!"node:value(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)"!]U, /* numberOfBlocks */
  [!ENDIF!]
[!ELSE!][!//
    0U, /* numberOfBlocks */
[!ENDIF!]
[!ELSE!][!//
    0U, /* numberOfBlocks */
[!ENDIF!]
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON)) */
#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
/* !LINKSTO StbM.ASR20-11.SWS_StbM_00470.Master.ValidationRecordTable,1 */
/* !LINKSTO StbM.ASR20-11.SWS_StbM_00470.Slave.ValidationRecordTable,1 */
/* !LINKSTO StbM.ASR20-11.SWS_StbM_00470.PdelayResponder.ValidationRecordTable,1 */
/* !LINKSTO StbM.ASR20-11.SWS_StbM_00470.PdelayInitiator.ValidationRecordTable,1 */
[!IF "(node:exists(./StbMTimeValidation))"!]
    [!"node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount)"!]U, /* numberOfBlocksTimeValidation */
[!ELSE!][!//
    0U, /* numberOfBlocksTimeValidation */
[!ENDIF!]
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */
#if (STBM_NVM_USED == STD_ON)
  [!IF "((node:exists(./StbMStoreTimebaseNonVolatile)) and (node:value(./StbMStoreTimebaseNonVolatile) != 'NO_STORAGE'))"!]
    [!"num:i(as:ref(./StbMGlobalNvMBlockDescriptor)/NvMNvramBlockIdentifier)"!]U, /* NVRAM Block Id */
  [!ELSE!]
    0U, /* NVRAM Block Id */
  [!ENDIF!]
#endif /* (STBM_NVM_USED == STD_ON) */
#if (STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
  [!IF "node:exists(./StbMTimeCorrection/StbMAllowMasterRateCorrection) and (node:value(./StbMTimeCorrection/StbMAllowMasterRateCorrection) = 'true')"!]
    [!"num:i(node:value(./StbMTimeCorrection/StbMMasterRateDeviationMax))"!]U, /* masterRateDeviationMax */
    [!"$masterRateCorrIndex"!]U, /* masterRcArrayIndex */
    [!VAR "masterRateCorrIndex" = "num:i($masterRateCorrIndex + 1)"!]
  [!ELSE!]
    STBM_RATE_DEVIATION_INVALID_VALUE, /* masterRateDeviationMax */
    STBM_INVALID_INDEX, /* masterRcArrayIndex */
  [!ENDIF!]
#endif /* STBM_MASTER_RATE_CORRECTION_USED == STD_ON */
#if (STBM_SLAVE_RATE_CORRECTION_USED == STD_ON)
[!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) and (node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) != 0)"!]
  [!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration)"!]
    [!VAR "endIndexRcIntervals" = "num:i($startIndexRcIntervals + num:i(node:value(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration)) - 1)"!]
  [!ELSE!]
    [!VAR "endIndexRcIntervals" = "num:i($startIndexRcIntervals)"!]
  [!ENDIF!]
    [!"$startIndexRcIntervals"!]U, /* startIndexRcIntervals */
    [!"$endIndexRcIntervals"!]U, /* stopIndexRcIntervals */
    [!"$intervalArrayIndex"!]U, /* intervalArrayIndex */
[!VAR "intervalArrayIndex" = "num:i($intervalArrayIndex + 1)"!]
[!VAR "startIndexRcIntervals" = "num:i($endIndexRcIntervals + 1)"!]
[!ELSE!][!//
    0U, /* startIndexRcIntervals */
    0U, /* stopIndexRcIntervals */
    0U, /* intervalArrayIndex */
[!ENDIF!]
#endif /* STBM_SLAVE_RATE_CORRECTION_USED == STD_ON */
#if ((STBM_MASTER_RATE_CORRECTION_USED == STD_ON) || (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON))
  [!IF "((node:exists(./StbMAllowSystemWideGlobalTimeMaster) = 'true') and (node:value(./StbMAllowSystemWideGlobalTimeMaster) = 'true'))"!]
    [!IF "(node:value(./StbMIsSystemWideGlobalTimeMaster) = 'true')"!]
      STBM_SYSTEM_WIDE_MASTER_ENABLED, /* systemWideMaster */
    [!ELSE!]
      STBM_SYSTEM_WIDE_MASTER_DISABLED, /* systemWideMaster */
    [!ENDIF!]
  [!ELSE!]
    STBM_SYSTEM_WIDE_MASTER_DISABLED,
  [!ENDIF!]
#endif /* ((STBM_MASTER_RATE_CORRECTION_USED == STD_ON) || (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON)) */
#if (STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON)
  [!IF "(node:value(./StbMProvideDataInSharedMemory) = 'true')"!]
    [!"$indexOfShareDataTimeBases"!]U, /* timeBaseSharesDataIdx */
    [!VAR "indexOfShareDataTimeBases" = "num:i($indexOfShareDataTimeBases + 1)"!]
  [!ELSE!]
    STBM_TIME_BASE_DOES_NOT_USE_SHARED_DATA, /* timeBaseSharesDataIdx */
  [!ENDIF!]
#endif /* STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON */

#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)
   [!IF "num:i(count(./StbMNotificationCustomer/*)) > 0"!]
     [!"num:i(count(./StbMNotificationCustomer/*))"!]U, /* number of customer notifications */
   [!ELSE!]
     0U, /* number of customer notifications */
   [!ENDIF!]
#endif /* (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) */

  },
[!ENDLOOP!]
};
#endif /* STBM_COMMON_FEATURES_ENABLED */

[!/* get the name of the SetGlobalTime function for each connected BSW module and store the
     function pointer*/!][!//
/* List of all "set global time" functions of adjacent layer */
CONST(StbM_TimeSetterFctPtrType, STBM_APPL_CONST) StbM_TimeSetterFctPtrList[STBM_NUMBER_OF_ADJACENT_MODULES] =
{
[!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
  [!VAR "BswModuleName" = "node:name(.)"!]
  [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ModuleConfigName')"!]
  [!IF "ecu:has($key)"!]
  [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ApiName.V1.SetGlobalTime')"!]
  [!IF "ecu:has($key)"!]
  &[!"asc:getApiFunctionName('', $BswModuleName, 'SetGlobalTime')"!],  /* [!"node:name(.)"!]*/
  [!ELSE!]
  NULL_PTR,  /* [!"node:name(.)"!]*/
  [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
[!/* the internal SetGlobalTime function is always the last entry in the list */!][!//
[!IF "($noSetModuleInList = 'true')"!]
  &StbM_SetGlobalTimeOffset  /* internal setter function to adjust time value */
[!ENDIF!]
};

[!IF "($StbMTimeBaseSyncTimeRecord = 'true') or ($StbMTimeBaseOffsetTimeRecord = 'true') or ($StbMTimeBaseNotificationStatus = 'true') or ($StbMTimeValidationUsed = 'true')"!]
/* Function pointer array for all the callback functions of all time bases */
/* !LINKSTO StbM.ASR44.SWS_StbM_00277.ToApplication.SR_INTERFACE,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00277.ToCDD.CALLBACK,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00277.ToCDDAndApplication.CALLBACK_AND_SR_INTERFACE,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00277.NoNotification,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00299,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00285,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00322,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00323,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00328,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00329,1 */
/* !LINKSTO StbM.ASR44.ECUC_StbM_00046_Conf,1 */
CONST(StbM_CallbacksFctPtrType, STBM_APPL_CONST) StbM_CallbacksPtrList[STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
    [!VAR "timeBaseName" = "node:name(.)"!]
#if (STBM_STATUS_NOTIFICATIONS_USED == STD_ON)
    [!IF "node:value(StbMNotificationInterface) = 'CALLBACK_AND_SR_INTERFACE'"!]
    [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMStatusNotificationCallback)"!]
    &[!"$callbackName"!],  /* statusNotificationCbkFctPtr */
    &Rte_Write_StbM[!"$EcuCPartitionName"!]_GlobalTime_StatusEvent_[!"$timeBaseName"!]_eventNotification,  /* statusNotificationRteCbkFctPtr */
    [!ELSEIF "(node:value(StbMNotificationInterface) = 'SR_INTERFACE')"!]
    NULL_PTR, /* statusNotificationCbkFctPtr */
    &Rte_Write_StbM[!"$EcuCPartitionName"!]_GlobalTime_StatusEvent_[!"$timeBaseName"!]_eventNotification,  /* statusNotificationRteCbkFctPtr */
    [!ELSEIF "(node:value(StbMNotificationInterface) = 'CALLBACK')"!]
    [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMStatusNotificationCallback)"!]
    &[!"$callbackName"!],  /* statusNotificationCbkFctPtr */
#if (STBM_RTE_USED == STD_ON)
    NULL_PTR, /* statusNotificationRteCbkFctPtr */
#endif /* (STBM_RTE_USED == STD_ON) */
    [!ELSE!]
    NULL_PTR, /* statusNotificationCbkFctPtr */
#if (STBM_RTE_USED == STD_ON)
    NULL_PTR,  /* statusNotificationRteCbkFctPtr */
#endif /* (STBM_RTE_USED == STD_ON) */
    [!ENDIF!]
#endif /* (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) */

#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and (node:exists(StbMTimeRecording/StbMSyncTimeRecordBlockCallback)))"!]
    [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMTimeRecording/StbMSyncTimeRecordBlockCallback)"!]
    &[!"$callbackName"!],  /* syncTimeRecordBlockCbkFctPtr */
    [!ELSE!]
    NULL_PTR,  /* syncTimeRecordBlockCbkFctPtr */
    [!ENDIF!]
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) */

#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and (node:exists(StbMTimeRecording/StbMOffsetTimeRecordBlockCallback)))"!]
    [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMTimeRecording/StbMOffsetTimeRecordBlockCallback)"!]
    &[!"$callbackName"!],  /* offsetTimeRecordBlockCbkFctPtr */
    [!ELSE!]
    NULL_PTR,  /* offsetTimeRecordBlockCbkFctPtr */
    [!ENDIF!]
#endif /* (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) */


#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON))
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ((node:exists(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) > 0)))"!]
    &Rte_Call_StbM[!"$EcuCPartitionName"!]_MeasurementNotification_[!"$timeBaseName"!]_SetSyncTimeRecordTable,  /* syncTimeRecordBlockRteCbkFctPtr */
    [!ELSE!]
    NULL_PTR,  /* syncTimeRecordBlockRteCbkFctPtr */
    [!ENDIF!]
#endif /* ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON)) */

#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON))
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ((node:exists(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount) > 0)))"!]
    &Rte_Call_StbM[!"$EcuCPartitionName"!]_MeasurementNotification_[!"$timeBaseName"!]_SetOffsetTimeRecordTable,  /* offsetTimeRecordBlockRteCbkFctPtr */
    [!ELSE!]
    NULL_PTR,  /* offsetTimeRecordBlockRteCbkFctPtr */
    [!ENDIF!]
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON)) */


#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
  /* Call GetConnectedBusTSynModule macro, in order to see what modules are connected to this Time Base */
  [!IF "not(node:exists(./StbMOffsetTimeBase))"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!ENDIF!]
  #if (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON)
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToMasterCanTSyn = 'masterCanTSyn') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Can_',$timeBaseName, '_SetMasterTimingData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON) */
  #if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON)
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToMasterFrTSyn = 'masterFrTSyn') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Fr_',$timeBaseName, '_SetMasterTimingData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON) */
  #if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
    [!"node:when((((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToMasterEthTSyn = 'masterEthTSyn') or ($isLinkedToMasterEthTSynPdelayResponder = 'masterEthTSynPdelayResponder')) and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Eth_',$timeBaseName, '_SetMasterTimingData,'), 'NULL_PTR,')"!]
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToMasterEthTSynPdelayResponder = 'masterEthTSynPdelayResponder') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Eth_',$timeBaseName, '_SetPdelayResponderData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */
  #if (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON)
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToSlaveCanTSyn = 'slaveCanTSyn') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Can_',$timeBaseName, '_SetSlaveTimingData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON) */
  #if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON)
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToSlaveFrTSyn = 'slaveFrTSyn') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Fr_',$timeBaseName, '_SetSlaveTimingData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON) */
  #if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
    [!"node:when((((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToSlaveEthTSyn = 'slaveEthTSyn') or ($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator')) and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Eth_',$timeBaseName, '_SetSlaveTimingData,'), 'NULL_PTR,')"!]
    [!"node:when(((not(node:exists(./StbMOffsetTimeBase))) and ($isLinkedToSlaveEthTSynPdelayInitiator = 'slaveEthTSynPdelayInitiator') and ($isTimeValidationEnabledForThisTimeBase = 'true')), concat('&Rte_Call_StbM',$EcuCPartitionName,'_TimeBaseProviderNotification_Eth_',$timeBaseName, '_SetPdelayInitiatorData,'), 'NULL_PTR,')"!]
  #endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

  },
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "$StbMTimeBaseSyncTimeRecord = 'true'"!]
/* !LINKSTO StbM.ASR44.SWS_StbM_00308.Header,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00309,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00310,1 */
/* Array for Sync Time Recording headers */
CONST(StbM_SyncRecordTableHeadType, STBM_APPL_CONST) StbM_SyncRecordTableHeads[STBM_NUMBER_OF_SYNC_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(not(node:exists(StbMOffsetTimeBase))) and (node:value(./StbMSynchronizedTimeBaseIdentifier) < 16)], './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
    [!"node:value(StbMSynchronizedTimeBaseIdentifier)"!]U, /* SynchronizedTimeDomain */
    [!/* get StbMClockFrequency and StbMClockPrescaler values */!][!//
	[!IF "((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware)))"!]
    [!IF "(node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../OsCounter)) or (node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration))"!][!//
    [!"node:value(StbMLocalTimeClock/StbMClockFrequency)"!]U, /* HWfrequency */
    [!"node:value(StbMLocalTimeClock/StbMClockPrescaler)"!]U, /* HWprescaler */
    [!ELSE!]
    [!"num:i(1000000000)"!]U, /* HWfrequency */
    [!"num:i(1)"!]U, /* HWprescaler */
    [!ENDIF!]
	[!ELSE!]
	[!/* For Synchronized Time bases, if StbMLocalTimeHardware does not exist, for sure StbMUseOSGetTimeStamp will be enabled,
	     and StbMClockFrequency and StbMClockPrescaler will be provided */!]
	[!"node:value(StbMLocalTimeClock/StbMClockFrequency)"!]U, /* HWfrequency */
    [!"node:value(StbMLocalTimeClock/StbMClockPrescaler)"!]U, /* HWprescaler */
	[!ENDIF!]
  },
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "$StbMTimeBaseOffsetTimeRecord = 'true'"!]
/* !LINKSTO StbM.ASR44.SWS_StbM_00311.Header,1 */
/* Array for Offset Time Recording headers */
CONST(StbM_OffsetRecordTableHeadType, STBM_APPL_CONST) StbM_OffsetRecordTableHeads[STBM_NUMBER_OF_OFFSET_TIMEBASES] =
{
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)], './StbMSynchronizedTimeBaseIdentifier')"!]
  { /* [!"node:name(.)"!] */
    [!"node:value(StbMSynchronizedTimeBaseIdentifier)"!]U, /* SynchronizedTimeDomain */
  },
[!ENDLOOP!]
};
[!ENDIF!]

#define STBM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <StbM_MemMap.h>

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

[!IF "$StbMTriggeredCustomersNumber > 0"!]
#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>

FUNC(uint32, STBM_CODE) StbM_ConvNsToTicks
(
  uint8 triggeredCustomerIndex,
  uint32 nanoseconds
)
{
  uint32 currentTimeTicks = 0U;

  switch(triggeredCustomerIndex)
  {
  [!VAR "triggeredCustomerIndex" = "num:i(0)"!]
  [!LOOP "node:order(as:modconf('StbM')[1]/StbMTriggeredCustomer/*, 'node:ref(./StbMSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier')"!]
    [!VAR "StbMOsCounterId" = "node:name(node:ref(node:ref(StbMOSScheduleTableRef)/OsScheduleTableCounterRef))"!]
    [!VAR "conversionFactor" = "node:value(./StbMScheduleTicksToNsConvFactor)"!]
    case [!"$triggeredCustomerIndex"!]:
      [!CALL "GetTriggeredCustomerNeededFrequency", "triggeredCustomer" = "node:path(.)"!]
      [!IF "($isConversionMacroDefinedForTriggeredCustomerFreq = 'true')"!]
        currentTimeTicks = StbM_NsToTicks_[!"$neededTriggeredCustomerFrequency"!](nanoseconds);
      [!ELSE!]
        currentTimeTicks = (nanoseconds * [!"$conversionFactor"!]) / OS_TICKS2NS_[!"$StbMOsCounterId"!]([!"$conversionFactor"!]);
      [!ENDIF!]
      break;
    [!VAR "triggeredCustomerIndex" = "num:i($triggeredCustomerIndex + 1)"!]
  [!ENDLOOP!]
    /* defensive programming */
    /* Default case would've been reached in the following situation:
       1. StbM_ConvNsToTicks() called with a timeBase ID which is above the maximum number of Triggered Customers.
       Situation 1 -> Will never happen, because the callee of StbM_ConvNsToTicks() is StbM_SyncTriggeredCustomer().
                      The call looks like this "StbM_SyncTriggeredCustomer(idx, StbM_TriggerCustomerCfg[idx].syncTimeBaseIndex);"
                      where the argument "idx" takes values through a for which loops from 0 to STBM_NUMBER_OF_TRIGGERED_CUSTOMERS.
                      In conclusion, this "triggeredCustomerIndex" from our StbM_ConvNsToTicks() function,
                      will always have a value from 0 to the maximum configured number of Triggered Customers
                      and default case will never be reached and neither be tested. */
    /* CHECK: NOPARSE */
    default: /* default case intended to be empty */
      STBM_UNREACHABLE_CODE_ASSERT(STBM_API_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }

  return currentTimeTicks;
}


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>
[!ENDIF!]




[!IF "$StbMGptUsage = 'true'"!]
#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>


FUNC(uint32, STBM_CODE) StbM_ConvGptTicksToNs
(
  uint32 timeBaseId,
  uint32 ticks
)
{
  uint32 GptTicksInNanoseconds = 0U;

  switch(timeBaseId)
  {
  [!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware))) and (node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration))], './StbMSynchronizedTimeBaseIdentifier')"!]
    [!VAR "timeBaseId" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!]
    case [!"$timeBaseId"!]:
      [!CALL "GetNeededFrequency", "timeBase" = "node:path(.)"!]
      [!IF "($isConversionMacroDefinedForFreq = 'true')"!]
        GptTicksInNanoseconds = StbM_TicksToNs_[!"$neededFrequency"!](ticks);
      [!ELSE!]
        /* Deviation TASKING-1 */
        GptTicksInNanoseconds = STBM_GPT_CONV_TICKS_TO_NS(ticks, [!"$freq"!]U);
      [!ENDIF!]
      break;
  [!ENDLOOP!]
    /* defensive programming */
    /* Default case would've been reached in the following situations:
       1. StbM_ConvGptTicksToNs() called with a timeBase ID which is not configured 
       2. StbM_ConvGptTicksToNs() called with a timeBase ID which doesn't have Gpt configured as a LocalTimeHardware reference
       Situation 1 -> Will never happen, because in the upper callee of StbM_ConvGptTicksToNs() function (which is StbM_GetTime()),
                      it is always assured from above callees that timeBaseId is a configured one.
       Situation 2 -> Will never happen, because virtualTimeGetter pointer is always defined based on which LocalTimeHardware it is configured.
                      In our case it will always be StbM_GetCurrentVirtualLocalTimeGpt() function. */
    /* CHECK: NOPARSE */
    default: /* default case intended to be empty */
      STBM_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return GptTicksInNanoseconds;
}


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>
[!ENDIF!]


/*==[Definition of functions with internal linkage]===========================*/

/** @} doxygen end group definition */
/*==================[end of file]===========================================*/

