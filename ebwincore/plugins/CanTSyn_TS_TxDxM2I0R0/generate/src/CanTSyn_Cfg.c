/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
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

#include <CanTSyn_Cfg.h> /* Generated pre-compile configuration. */
[!INCLUDE "../include/CanTSyn_PrecompileMacros.m"!][!//

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* !LINKSTO CanTSyn.dsn.PrecompileTimeConfig,1 */

[!IF "$CanTSyn_SyncDomains != 0"!]
[!CALL "GetCanTSynCRCSyncDomains"!]
[!FOR "i" = "0" TO "$CanTSyn_SyncDomains - 1"!][!//
[!VAR "currentTimeDomain" = "text:split($sortedDomainsList, ',')[position()-1 = $i]"!]
[!SELECT "node:ref($currentTimeDomain)"!][!//

  [!IF "node:exists(./CanTSynGlobalTimeSyncDataIDList/CanTSynGlobalTimeSyncDataIDListElement)"!]
#define CANTSYN_START_SEC_CONST_8
#include <CanTSyn_MemMap.h>

static CONST(CanTSyn_DataIDListType, CANTSYN_APPL_CONST) CanTSyn_DataIdListSyncValues_[!"node:name(.)"!][16] =
{
    [!FOR "j" = "0" TO "15"!]
      [!INDENT "1"!]
  [!"num:i(node:value(concat(node:paths(node:order(./CanTSynGlobalTimeSyncDataIDList/CanTSynGlobalTimeSyncDataIDListElement/*, 'CanTSynGlobalTimeSyncDataIDListIndex'))[position()-1 = $j],'/CanTSynGlobalTimeSyncDataIDListValue')))"!]U,
      [!ENDINDENT!]
    [!ENDFOR!]
};

#define CANTSYN_STOP_SEC_CONST_8
#include <CanTSyn_MemMap.h>
  [!ENDIF!]

  [!IF "node:exists(./CanTSynGlobalTimeFupDataIDList/CanTSynGlobalTimeFupDataIDListElement)"!]
#define CANTSYN_START_SEC_CONST_8
#include <CanTSyn_MemMap.h>

static CONST(CanTSyn_DataIDListType, CANTSYN_APPL_CONST) CanTSyn_DataIdListFupValues_[!"node:name(.)"!][16] =
{
    [!FOR "j" = "0" TO "15"!]
      [!INDENT "1"!]
  [!"num:i(node:value(concat(node:paths(node:order(./CanTSynGlobalTimeFupDataIDList/CanTSynGlobalTimeFupDataIDListElement/*, 'CanTSynGlobalTimeFupDataIDListIndex'))[position()-1 = $j],'/CanTSynGlobalTimeFupDataIDListValue')))"!]U,
      [!ENDINDENT!]
    [!ENDFOR!]
};

#define CANTSYN_STOP_SEC_CONST_8
#include <CanTSyn_MemMap.h>
  [!ENDIF!]

[!ENDSELECT!]
[!ENDFOR!]
[!ENDIF!]


[!IF "$CanTSyn_OffsetDomains != 0"!]
[!CALL "GetCanTSynCRCOffsetDomains"!]
[!FOR "i" = "0" TO "$CanTSyn_OffsetDomains - 1"!][!//
[!VAR "currentTimeDomain" = "text:split($sortedDomainsList, ',')[position()-1 = $i]"!]
[!SELECT "node:ref($currentTimeDomain)"!][!//

  [!IF "node:exists(./CanTSynGlobalTimeOfsDataIDList/CanTSynGlobalTimeOfsDataIDListElement)"!]
#define CANTSYN_START_SEC_CONST_8
#include <CanTSyn_MemMap.h>

static CONST(CanTSyn_DataIDListType, CANTSYN_APPL_CONST) CanTSyn_DataIdListOfsValues_[!"node:name(.)"!][16] =
{
    [!FOR "j" = "0" TO "15"!]
      [!INDENT "1"!]
  [!"num:i(node:value(concat(node:paths(node:order(./CanTSynGlobalTimeOfsDataIDList/CanTSynGlobalTimeOfsDataIDListElement/*, 'CanTSynGlobalTimeOfsDataIDListIndex'))[position()-1 = $j],'/CanTSynGlobalTimeOfsDataIDListValue')))"!]U,
      [!ENDINDENT!]
    [!ENDFOR!]
};
#define CANTSYN_STOP_SEC_CONST_8
#include <CanTSyn_MemMap.h>
  [!ENDIF!]

  [!IF "node:exists(./CanTSynGlobalTimeOfnsDataIDList/CanTSynGlobalTimeOfnsDataIDListElement)"!]
    [!IF "node:value(./CanTSynUseExtendedMsgFormat) = 'false'"!]
#define CANTSYN_START_SEC_CONST_8
#include <CanTSyn_MemMap.h>

static CONST(CanTSyn_DataIDListType, CANTSYN_APPL_CONST) CanTSyn_DataIdListOfnsValues_[!"node:name(.)"!][16] =
{
    [!FOR "j" = "0" TO "15"!]
      [!INDENT "1"!]
  [!"num:i(node:value(concat(node:paths(node:order(./CanTSynGlobalTimeOfnsDataIDList/CanTSynGlobalTimeOfnsDataIDListElement/*, 'CanTSynGlobalTimeOfnsDataIDListIndex'))[position()-1 = $j],'/CanTSynGlobalTimeOfnsDataIDListValue')))"!]U,
      [!ENDINDENT!]
    [!ENDFOR!]
};

#define CANTSYN_STOP_SEC_CONST_8
#include <CanTSyn_MemMap.h>
    [!ENDIF!]
  [!ENDIF!]

[!ENDSELECT!]
[!ENDFOR!]
[!ENDIF!]

#define CANTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanTSyn_MemMap.h>

[!IF "$CanTSyn_TMSize != 0"!]
[!CALL "GetCanTSynMasterDomainsList"!]
[!CALL "IsMasterTimeValidationSupported"!]
CONST(CanTSyn_TimeMasterDomainConfigType, CANTSYN_APPL_CONST) CanTSyn_TimeMasterConfig[[!"num:i($CanTSyn_TMSize)"!]] =
{
[!FOR "i" = "0" TO "$CanTSyn_TMSize - 1"!][!//
  [!VAR "currentTimeDomainCfg"="text:split($sortedMasterDomainsList,',')[position()-1 = $i]"!]
  [!SELECT "node:ref($currentTimeDomainCfg)"!][!//

  /* [!"node:name(.)"!] */
  {
  [!IF "$CanTSyn_CRCMasterDomains != 0"!]
    [!IF "node:value(./CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED'"!]
      [!IF "(node:value(./CanTSynGlobalTimeDomainId) < 16)"!]
    &CanTSyn_DataIdListSyncValues_[!"node:name(.)"!][0], /* DataID List for the SYNC messages */
    &CanTSyn_DataIdListFupValues_[!"node:name(.)"!][0], /* DataID List for the FUP messages */
      [!ELSE!]
    &CanTSyn_DataIdListOfsValues_[!"node:name(.)"!][0], /* DataID List for the OFS messages */
        [!IF "node:value(./CanTSynUseExtendedMsgFormat) = 'false'"!]
    &CanTSyn_DataIdListOfnsValues_[!"node:name(.)"!][0], /* DataID List for the OFNS messages */
        [!ELSE!]
    NULL_PTR,
        [!ENDIF!]
      [!ENDIF!]
    [!ELSE!]
    NULL_PTR,
    NULL_PTR,
    [!ENDIF!]
  [!ENDIF!]
  [!CALL "TimeoutFloatToMFC", "value"="./CanTSynGlobalTimeMaster/CanTSynGlobalTimeDebounceTime"!]
  [!"num:i($mValue + 1)"!]U, /* Debounce Time (Master) */
  [!CALL "IsTxImmediateTransmissionUsed"!]
  [!IF "$IsTxImmediateTransmissionUsed = 'true'"!]
    [!IF "node:exists(./CanTSynGlobalTimeMaster/CanTSynCyclicMsgResumeTime)"!]
    [!CALL "TimeoutFloatToMFC", "value"="./CanTSynGlobalTimeMaster/CanTSynCyclicMsgResumeTime"!]
    [!"$mValue"!]U, /* CyclicMsgResumeTime (Master) */
    [!ELSE!]
    0U, /* CyclicMsgResumeTime (Master) */
    [!ENDIF!]
  [!ENDIF!]
  [!CALL "TimeoutFloatToMFC", "value"="./CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxPeriod"!]
    [!"$mValue"!]U, /* SyncTxInterval (Master) */
  [!CALL "TimeoutFloatToMFC", "value"="./CanTSynGlobalTimeMaster/CanTSynMasterConfirmationTimeout"!]
    [!"$mValue"!]U, /* Confirmation Timeout */
    [!"num:i(node:value(node:ref(./CanTSynSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier))"!]U, /* StbMSynchronizedTimeBaseIdentifier */
  [!CALL "GetCanIfHandleId", "CanTSynGlobalTimePduRef" = "./CanTSynGlobalTimeMaster/CanTSynGlobalTimeMasterPdu/CanTSynGlobalTimePduRef"!][!//
    [!"$PduId"!]U, /* CanIfTxPduId */
    [!"num:i(node:value(./CanTSynGlobalTimeDomainId))"!]U, /* TimeDomainId */
  [!CALL "GetCanIfCtrlId", "CanTSynGlobalTimePduRef" = "./CanTSynGlobalTimeMaster/CanTSynGlobalTimeMasterPdu/CanTSynGlobalTimePduRef"!][!//
    [!"$CtrlId"!]U, /* CanIfCtrlId */
  [!CALL "GetCanIfCtrlIndex", "ControllerID" = "$CtrlId"!][!//
    [!"$CtrlIndex"!]U, /* CtrlIdx */
    [!"concat("CANTSYN_",node:value(./CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured))"!], /* TxCrcSecured */
  [!CALL "isImmediateTimeSync", "value"="./CanTSynGlobalTimeMaster/CanTSynImmediateTimeSync"!]
    [!"$isImmediateTimeSync"!]U, /* ImmediateTimeSync */
[!IF "$CanTSyn_NumTxExtendedMsgFormat > 0"!]
  [!CALL "isExtendedMessage", "value"="./CanTSynUseExtendedMsgFormat"!]
    [!"$isExtendedMessage"!]U, /* ExtendedMessage */
[!ENDIF!]
[!IF "$isMasterTimeValidationSupported = 'true'"!]
  [!IF "node:value(./CanTSynEnableTimeValidation) = 'true'"!]    
    TRUE,/* IsTimeValidationUsed */
    [!"node:value(./CanTSynGlobalTimeNetworkSegmentId)"!]U /* NetworkSegmentId */
  [!ELSE!]    
    FALSE,/* IsTimeValidationUsed */
    CANTSYN_SEGMENTID_UNUSED /* NetworkSegmentId */
  [!ENDIF!] 
[!ENDIF!]
  },[!//
  [!ENDSELECT!]
[!ENDFOR!]
};
[!ENDIF!]

[!CALL "IsSlaveTimeValidationSupported"!]
[!IF "$CanTSyn_TSSize != 0"!]
[!CALL "GetCanTSynSlaveDomainsList"!]
CONST(CanTSyn_TimeSlaveDomainConfigType, CANTSYN_APPL_CONST) CanTSyn_TimeSlaveConfig[[!"num:i($CanTSyn_TSSize)"!]] =
{
[!FOR "i" = "0" TO "$CanTSyn_TSSize - 1"!][!//
  [!VAR "currentTimeDomainCfg"="text:split($sortedSlaveDomainsList,',')[position()-1 = $i]"!]
  [!SELECT "node:ref($currentTimeDomainCfg)"!][!//

  /* [!"node:name(.)"!] */
  {
  [!IF "$CanTSyn_CRCSlaveDomains != 0"!]
    [!IF "(node:value(./CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(./CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL')"!]
      [!IF "(node:value(./CanTSynGlobalTimeDomainId) < 16)"!]
    &CanTSyn_DataIdListSyncValues_[!"node:name(.)"!][0],
    &CanTSyn_DataIdListFupValues_[!"node:name(.)"!][0],
      [!ELSE!]
    &CanTSyn_DataIdListOfsValues_[!"node:name(.)"!][0],
        [!IF "node:value(./CanTSynUseExtendedMsgFormat) = 'false'"!]
    &CanTSyn_DataIdListOfnsValues_[!"node:name(.)"!][0],
        [!ELSE!]
    NULL_PTR,
        [!ENDIF!]
      [!ENDIF!]
    [!ELSE!]
    NULL_PTR,
    NULL_PTR,
    [!ENDIF!]
  [!ENDIF!]
  [!CALL "TimeoutFloatToMFC", "value"="./CanTSynGlobalTimeSlave/CanTSynGlobalTimeFollowUpTimeout"!]
    [!"$mValue"!]U, /* RxFollowUpOffsetInterval (Slave) */
    [!"num:i(node:value(node:ref(./CanTSynSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier))"!]U, /* StbMSynchronizedTimeBaseIdentifier */
  [!CALL "CheckCanIfRxPdu", "CanTSynGlobalTimePduRef" = "./CanTSynGlobalTimeSlave/CanTSynGlobalTimeSlavePdu/CanTSynGlobalTimePduRef"!][!//
  [!CALL "SlaveTimeDomainId", "value" = "./CanTSynGlobalTimeDomainId"!]
    [!"$mValue"!]U, /* TimeDomainId */
    [!"concat("CANTSYN_",node:value(./CanTSynGlobalTimeSlave/CanTSynRxCrcValidated))"!], /* RxCrcValidated */
    [!"num:i(node:value(./CanTSynGlobalTimeSlave/CanTSynGlobalTimeSequenceCounterJumpWidth))"!]U, /* SequenceCounterJumpWidth */
[!IF "$CanTSyn_NumRxExtendedMsgFormat > 0"!]
  [!CALL "isExtendedMessage", "value"="./CanTSynUseExtendedMsgFormat"!]
    [!"$isExtendedMessage"!]U, /* ExtendedMessage */
[!ENDIF!]
[!IF "$isSlaveTimeValidationSupported = 'true'"!]
  [!IF "node:value(./CanTSynEnableTimeValidation) = 'true'"!]    
    TRUE,/* IsTimeValidationUsed */
    [!"node:value(./CanTSynGlobalTimeNetworkSegmentId)"!]U /* NetworkSegmentId */
  [!ELSE!]    
    FALSE,/* IsTimeValidationUsed */
    CANTSYN_SEGMENTID_UNUSED /* NetworkSegmentId */
  [!ENDIF!] 
[!ENDIF!]
  },[!//
  [!ENDSELECT!]
[!ENDFOR!]

};
[!ENDIF!]

#define CANTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanTSyn_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]


