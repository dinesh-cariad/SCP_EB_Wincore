/**
 * \file
 *
 * \brief AUTOSAR FrTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTSyn.
 *
 * \version 2.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]
/*==================[inclusions]============================================*/

#include <FrTSyn_Cfg.h> /* Generated pre-compile configuration. */
[!INCLUDE "../include/FrTSyn_PrecompileMacros.m"!][!//

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
[!VAR "DIDIdx" = "num:i(0)"!][!//
#define FRTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrTSyn_MemMap.h>

[!IF "num:i($FrTSyn_TimeDomainSlavesSize) > num:i(0)"!][!//
CONST(FrTSyn_SlaveTimeDomainConfigType, FRTSYN_APPL_CONST) FrTSyn_SlaveConfig[[!"num:i($FrTSyn_TimeDomainSlavesSize)"!]U] =
{
  [!CALL "GetFrTSynTimeDomainsList","Type"="'SLAVE'"!]
  [!FOR "i" = "0" TO "$FrTSyn_TimeDomainSlavesSize - 1"!][!//
    [!VAR "currentTimeDomainCfg"="text:split($sortedTimeDomainsList,',')[position()-1 = $i]"!]
    [!CALL "GetAssocTDIdx","timeDomainPath"="$currentTimeDomainCfg", "Type"="'SLAVE'"!]
    [!SELECT "node:ref($currentTimeDomainCfg)"!]
    /* [!"node:name(.)"!], FrTSynGlobalTimeSlaveHandleId: [!"num:i(node:value(./FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimeSlaveHandleId ))"!] */
    {
      [!"node:ref(FrTSynSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier"!]U, /* TimeBaseId */
      FRTSYN_[!"$tdType"!]_[!"node:value(./FrTSynGlobalTimeSlave/FrTSynRxCrcValidated)"!], /* CRC Validation */
      [!"num:i(node:value(./FrTSynGlobalTimeSlave/FrTSynGlobalTimeSequenceCounterJumpWidth))"!]U, /* SequenceCounterJumpWidth */
      [!"$tdID"!]U, /* TimeDomainId */
      [!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
      [!IF "not(node:empty(node:ref($currentTimeDomainCfg)/FrTSynEnableTimeValidation)) and node:value(node:ref($currentTimeDomainCfg)/FrTSynEnableTimeValidation) = 'true'"!]
      [!"num:i(node:value(node:ref($currentTimeDomainCfg)/FrTSynGlobalTimeNetworkSegmentId))"!]U, /* SegmentId */
      [!ELSE!][!//
      [!"num:i(255)"!]U, /* SegmentId not assigned for this time domain */
      [!ENDIF!][!//
      [!ENDIF!][!//
      [!"$assocIdx"!], /* AssocIdx */
      [!CALL "PreparePduControllerAndClusterIdxList"!][!//
      [!CALL "GetControllerAndClusterIdx", "FrTSynGlobalTimePduRef" = "./FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef"!][!//
      [!"$FrIfCtrlIdx"!]U, /* FrIfCtrlIdx */
      [!"$FrIfClstIdx"!]U, /* FrIfClstIdx */
      [!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
      [!IF "(node:exists(FrTSynEnableTimeValidation)) and (node:value(FrTSynEnableTimeValidation) = 'true')"!]
      FRTSYN_TIME_VALIDATION_IS_ENABLED, /* Time Validation */
      [!ELSE!][!//
      FRTSYN_TIME_VALIDATION_NOT_ENABLED, /* Time Validation */
      [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "$FrTSyn_NumGTDILEs > 0"!][!//
        [!IF "node:exists(./FrTSynGlobalTimeSyncDataIDList) or node:exists(./FrTSynGlobalTimeOfsDataIDList)"!]
      [!"$DIDIdx"!]U /* DataIdList idx */
      [!VAR "DIDIdx" = "num:i($DIDIdx + 16)"!][!//
        [!ELSE!]
      0U /* DataIdList idx */
        [!ENDIF!]
      [!ENDIF!][!//
    },
    [!ENDSELECT!]
  [!ENDFOR!]
};
[!ENDIF!][!//

[!IF "num:i($FrTSyn_TimeDomainMastersSize) > num:i(0)"!][!//
CONST(FrTSyn_MasterTimeDomainConfigType, FRTSYN_APPL_CONST) FrTSyn_MasterConfig[[!"num:i($FrTSyn_TimeDomainMastersSize)"!]U] =
{
  [!CALL "GetFrTSynTimeDomainsList","Type"="'MASTER'"!]
  [!FOR "i" = "0" TO "$FrTSyn_TimeDomainMastersSize - 1"!][!//
    [!VAR "currentTimeDomainCfg"="text:split($sortedTimeDomainsList,',')[position()-1 = $i]"!]
    [!CALL "GetAssocTDIdx","timeDomainPath"="$currentTimeDomainCfg"!]
    [!SELECT "node:ref($currentTimeDomainCfg)"!]
    /* [!"node:name(.)"!], FrTSynGlobalTimeMasterHandleId: [!"num:i(node:value(./FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimeMasterHandleId))"!] */
    {
      [!"node:ref(FrTSynSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier"!]U, /* TimeBaseId */
      [!CALL "TimeoutFloatToMFC", "value" = "./FrTSynGlobalTimeMaster/FrTSynGlobalTimeTxPeriod"!][!//
      [!"$MFCycleCount"!]U, /* TxPeriod */
[!IF "bit:and($usedTxTypes, 6) > 0"!][!//Immediate Transmission enabled
  [!IF "node:exists(./FrTSynGlobalTimeMaster/FrTSynGlobalTimeDebounceTime)"!][!//
      [!CALL "TimeoutFloatToMFC", "value" = "./FrTSynGlobalTimeMaster/FrTSynGlobalTimeDebounceTime"!][!//
      [!"$MFCycleCount"!]U, /* Debounce Counter */
  [!ELSE!][!//
      0U, /* Debounce Counter */
  [!ENDIF!][!//
  [!IF "bit:and($usedTxTypes, $TX_BOTH) > 0"!][!//
    [!IF "node:exists(./FrTSynGlobalTimeMaster/FrTSynCyclicMsgResumeTime)"!][!//
      [!CALL "TimeoutFloatToMFC", "value" = "./FrTSynGlobalTimeMaster/FrTSynCyclicMsgResumeTime"!][!//
      [!"num:i($MFCycleCount)"!]U, /* CyclicMsgResumeCounter */
    [!ELSE!][!//
      0U, /* CyclicMsgResumeCounter */
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
      [!CALL "PreparePduControllerAndClusterIdxList"!][!//
      [!CALL "GetControllerAndClusterIdx", "FrTSynGlobalTimePduRef" = "./FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef"!][!//
      [!"$TxPduId"!]U, /* PduId for FrIf_Transmit() */
      FRTSYN_[!"$tdType"!]_[!"node:value(./FrTSynGlobalTimeMaster/FrTSynGlobalTimeTxCrcSecured)"!], /* CRC Support*/
      [!"$tdID"!]U, /* TimeDomainId */
[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
  [!IF "not(node:empty(./FrTSynEnableTimeValidation)) and node:value(./FrTSynEnableTimeValidation) = 'true'"!]
      [!"num:i(node:value(./FrTSynGlobalTimeNetworkSegmentId))"!]U, /* SegmentId */
  [!ELSE!][!//
      [!"num:i(255)"!]U, /* SegmentId not assigned for this time domain*/
  [!ENDIF!][!//
[!ENDIF!][!//
      [!"$assocIdx"!], /* AssocIdx */
      [!"$FrIfCtrlIdx"!]U, /* FrIfCtrlIdx */
      [!"$FrIfClstIdx"!]U, /* FrIfClstIdx */
      [!CALL "GetTxTypeFuncIdx", "timeMasterPath" = "node:path(.)"!][!//
      [!"$txTypeFuncIdx"!]U, /* txCondFuncIdx */
      /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00096,1 */
      [!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
      [!IF "$FrIfImmediateParam = 'true'"!]
      FRTSYN_FRIF_TX_MODE_IMMEDIATE, /* transmissionMode */
      [!ELSE!][!//
      FRTSYN_FRIF_TX_MODE_DECOUPLED, /* transmissionMode */
      [!ENDIF!][!//
      /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00097,1 */
      [!IF "(node:exists(FrTSynEnableTimeValidation)) and (node:value(FrTSynEnableTimeValidation) = 'true')"!]
      FRTSYN_TIME_VALIDATION_IS_ENABLED, /* Time Validation */
      [!ELSE!][!//
      FRTSYN_TIME_VALIDATION_NOT_ENABLED, /* Time Validation */
      [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "$FrTSyn_NumGTDILEs > 0"!][!//
        [!IF "node:exists(./FrTSynGlobalTimeSyncDataIDList) or node:exists(./FrTSynGlobalTimeOfsDataIDList)"!]
      [!"$DIDIdx"!]U /* DataIdList idx */
      [!VAR "DIDIdx" = "num:i($DIDIdx + 16)"!][!//
        [!ELSE!]
      0U /* DataIdList idx */
        [!ENDIF!]
      [!ENDIF!][!//
    },
    [!ENDSELECT!]
  [!ENDFOR!]
};
[!ENDIF!][!//

#define FRTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrTSyn_MemMap.h>

[!IF "$FrTSyn_NumGTDILEs > 0"!]
#define FRTSYN_START_SEC_CONFIG_DATA_8
#include <FrTSyn_MemMap.h>
CONST(FrTSyn_DataIDType, FRTSYN_APPL_CONST) FrTSyn_GlobalTimeDataIDListElements[[!"$FrTSyn_NumGTDILEs"!]U] =
{
[!/* Looping for a third time */!][!//
  [!CALL "GetFrTSynTimeDomainsList"!]
  [!FOR "i" = "0" TO "$FrTSyn_TimeDomainsSize - 1"!][!//
    [!VAR "currentTimeDomainCfg"="text:split($sortedTimeDomainsList,',')[position()-1 = $i]"!]
    [!SELECT "node:ref($currentTimeDomainCfg)"!]
      [!IF "node:exists(./FrTSynGlobalTimeSyncDataIDList)"!]
      [!WS "2"!]/* [!"node:name(.)"!] */
        [!LOOP "./FrTSynGlobalTimeSyncDataIDList/FrTSynGlobalTimeSyncDataIDListElement/*"!]
          [!WS "2"!][!"num:i(node:value(./FrTSynGlobalTimeSyncDataIDListValue))"!]U, /* [!"node:value(./FrTSynGlobalTimeSyncDataIDListIndex)"!] */
        [!ENDLOOP!]
      [!ELSEIF "node:exists(./FrTSynGlobalTimeOfsDataIDList)"!]
      [!WS "2"!]/* [!"node:name(.)"!] */
        [!LOOP "./FrTSynGlobalTimeOfsDataIDList/FrTSynGlobalTimeOfsDataIDListElement/*"!]
          [!WS "2"!][!"num:i(node:value(./FrTSynGlobalTimeOfsDataIDListValue ))"!]U, /* [!"node:value(./FrTSynGlobalTimeOfsDataIDListValue )"!] */
        [!ENDLOOP!]
      [!ENDIF!]
    [!ENDSELECT!]
  [!ENDFOR!]
};
#define FRTSYN_STOP_SEC_CONFIG_DATA_8
#include <FrTSyn_MemMap.h>
[!ENDIF!][!//$FrTSyn_NumGTDILEs > 0
/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]
