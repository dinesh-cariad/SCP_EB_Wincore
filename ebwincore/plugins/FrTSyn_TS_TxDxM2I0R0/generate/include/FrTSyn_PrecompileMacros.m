[!/**
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
 */!][!//
[!NOCODE!]
[!// multiple inclusion protection
[!IF "not(var:defined('FRTSYN_PRECOMPILEMACROS_M'))"!]
[!VAR "FRTSYN_PRECOMPILEMACROS_M"="'true'"!][!/*

=== Size of the FrTSyn Slave Time Domains ===
*/!][!VAR "FrTSyn_TimeDomainSlavesSize" = "num:i(count(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*/FrTSynGlobalTimeSlave))"!][!/*

=== Size of the FrTSyn Master Time Domains ===
*/!][!VAR "FrTSyn_TimeDomainMastersSize" = "num:i(count(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*/FrTSynGlobalTimeMaster))"!][!/*

=== Size of the FrTSyn Time Domains ===
*/!][!VAR "FrTSyn_TimeDomainsSize" = "num:i($FrTSyn_TimeDomainSlavesSize + $FrTSyn_TimeDomainMastersSize)"!][!/*

=== Sum of FrTSynGlobalTimeSyncDataIDListElements and FrTSynGlobalTimeOfsDataIDListElements ===
*/!][!VAR "FrTSyn_NumGTDILEs" = "num:i((count(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*/FrTSynGlobalTimeSyncDataIDList)*16) + (count(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*/FrTSynGlobalTimeOfsDataIDList)*16))"!][!/*
=== Type used for member DIDIdx within FrTSyn_{Slave,Master}TimeDomainConfigType ===
*/!][!IF "$FrTSyn_NumGTDILEs > 256"!][!VAR "DIDIdxType" = "'uint16'"!][!ELSE!][!VAR "DIDIdxType" = "'uint8'"!][!ENDIF!][!/*
=== Used to check if extra struct members are enabled ===
*/!][!VAR "immediateTS" = "node:exists(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*/FrTSynGlobalTimeMaster[FrTSynImmediateTimeSync = 'true'])"!][!/*
=== Used for Time Validation ===
*/!][!VAR "FrTSyn_TimeValidationEnabled" = "node:value(as:modconf('FrTSyn')[1]/FrTSynGeneral/FrTSynTimeValidationSupport)"!][!/*

=== Macro to sort the Time Domain List based on Handle Id ===
=== SYNC timebases precede OFS timebases ===
=== Type parameter is expected to be either "SLAVE","MASTER" or "ALL". ===
=== In case of "ALL" slaves and master lists are concatenated, leading with slaves. ===
=== The result is in: sortedTimeDomainsList ===
*/!][!MACRO "GetFrTSynTimeDomainsList", "Type"="'ALL'"!][!NOCODE!]
  [!IF "not(var:defined('sortedTimeDomainsListMasters'))"!]
    [!VAR "sortedTimeDomainsListMasters"="text:join(node:paths(node:order(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*[FrTSynGlobalTimeMaster],'FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimeMasterHandleId', 'FrTSynGlobalTimeDomainId')),',')"!]
  [!ENDIF!]
  [!IF "not(var:defined('sortedTimeDomainsListSlaves'))"!]
    [!VAR "sortedTimeDomainsListSlaves" = "text:join(node:paths(node:order(as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*[FrTSynGlobalTimeSlave], 'FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimeSlaveHandleId','FrTSynGlobalTimeDomainId')),',')"!]
  [!ENDIF!]
  [!IF "$Type = 'SLAVE'"!]
    [!VAR "sortedTimeDomainsList" = "$sortedTimeDomainsListSlaves"!]
  [!ELSEIF "$Type = 'MASTER'"!]
    [!VAR "sortedTimeDomainsList" = "$sortedTimeDomainsListMasters"!]
  [!ELSE!]
    [!VAR "sortedTimeDomainsList" = "concat($sortedTimeDomainsListSlaves, ',', $sortedTimeDomainsListMasters)"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate the configured timeout float into number of main function cycles. ===
<!-- !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00072,1 -->
*/!][!MACRO "TimeoutFloatToMFC", "value"!][!NOCODE!]
    [!VAR "MFCycleCount" = "num:i(ceiling($value div as:modconf('FrTSyn')[1]/FrTSynGeneral/FrTSynMainFunctionPeriod))"!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*********************************************************************************************************************
* This macro classifies the configured FrTSynGlobalTimeMaster TimeDomains according to their configured
* transmission type, which can be
*   TX_PERIODIC
*   TX_IMMEDIATE
*   TX_BOTH
* The variable
*   "usedTxTypes" is used for the lookup table.
*   "timeMasterTxTypes" contains a semicolon separated list of colon separated tuples consisting of
*      the node path and type.
* The used lookup indices are the values of the above listed symbols divided by 2. Calculating the proper idx
* based on what's enabled using the template generators would produce too much clutter, as such
* FRTSYN_<transmission type>_LUT_NAME macros are used which ensure the funcPtr position stays the same,
* but is replaced by NULL_PTR when deactivated.
*********************************************************************************************************************
*/!][!MACRO "ClassifyGTDs"!][!NOCODE!]
[!VAR "TX_PERIODIC" = "num:i(1)"!]
[!VAR "TX_IMMEDIATE" = "num:i(2)"!]
[!VAR "TX_BOTH" = "num:i(4)"!]
[!VAR "usedTxTypes" = "num:i(0)"!]
[!VAR "timeMasterTxTypes" = "''"!]
[!VAR "txTypeFuncNum" = "num:i(0)"!]

[!LOOP "as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*[FrTSynGlobalTimeMaster]"!]
  [!IF "node:value(FrTSynGlobalTimeMaster/FrTSynGlobalTimeTxPeriod) > 0"!]
    [!IF "FrTSynGlobalTimeMaster/FrTSynImmediateTimeSync = 'false'"!]
      [!VAR "usedTxTypes" = "bit:or($usedTxTypes, $TX_PERIODIC)"!]
      [!VAR "timeMasterTxTypes" = "concat($timeMasterTxTypes, node:path(.), ':', num:i($TX_PERIODIC div 2), ';')"!]
    [!ELSE!]
      [!VAR "usedTxTypes" = "bit:or($usedTxTypes, $TX_BOTH)"!]
      [!VAR "timeMasterTxTypes" = "concat($timeMasterTxTypes, node:path(.), ':', num:i($TX_BOTH div 2), ';')"!]
    [!ENDIF!][!//Immediate
  [!ELSE!]
    [!VAR "usedTxTypes" = "bit:or($usedTxTypes, $TX_IMMEDIATE)"!]
    [!VAR "timeMasterTxTypes" = "concat($timeMasterTxTypes, node:path(.), ':', num:i($TX_IMMEDIATE div 2), ';')"!]
  [!ENDIF!][!//TxPeriod
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]
[!CALL "ClassifyGTDs"!][!/*
*********************************************************************************************************************
* Macro sets the "txTypeFuncIdx" variable as previously calculated by the macro ClassifyGTDs
*  The param "timeMasterPath" is the node path of a FrTSynGlobalTimeMaster
*********************************************************************************************************************
*/!][!MACRO "GetTxTypeFuncIdx", "timeMasterPath"!][!NOCODE!]
[!LOOP "text:split($timeMasterTxTypes, ';')"!]
  [!IF "$timeMasterPath = text:split(., ':')[1]"!]
      [!VAR "txTypeFuncIdx" = "num:i(text:split(., ':')[2])"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*
=== Returns the associated Time Domains index ===
=== This is needed because of multiplexed PDUs ===
=== The reserved value of FRTSYN_RESERVED_TT_DONE is used for Offset Time Bases as well as for Synchronized ===
=== Time Bases if they don't have an associated Offset Time Base. ===
=== timeDomainPath parameter is a Time Domain node path, "Type" is either "MASTER" or "SLAVE"
=== The returned values reside in "assocIdx" (number), tdID and "tdType" {SYNC, OFS} (used in the same loop) ===
=== Calling the MACRO for Slaves is incorrect, but it does return the tdID and tdType correctly ===
*/!][!MACRO "GetAssocTDIdx", "timeDomainPath", "Type"="'MASTER'"!][!NOCODE!]
[!VAR "pathOfAssocTD" = "''"!]
[!VAR "tdID" = "num:i(node:value(node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId))"!]
[!IF "($tdID < 16)"!]
  [!VAR "tdType" = "'SYNC'"!]
    [!IF "$Type = 'MASTER'"!]
      [!IF "node:exists(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId +16) and (FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef)])"!]
        [!VAR "pathOfAssocTD" = "node:path(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId +16) and (FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef)])"!]
      [!ELSE!]
        [!VAR "assocIdx" = "'FRTSYN_RESERVED_TT_DONE'"!]
      [!ENDIF!]
    [!ELSE!]
      [!IF "node:exists(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId +16) and (FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef)])"!]
        [!VAR "pathOfAssocTD" = "node:path(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId +16) and (FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef)])"!]
      [!ELSE!]
        [!VAR "pathOfAssocTD" = "$timeDomainPath"!]
      [!ENDIF!]
    [!ENDIF!]
[!ELSE!]
  [!VAR "tdType" = "'OFS'"!]
  [!IF "$Type = 'MASTER'"!]
    [!VAR "assocIdx" = "'FRTSYN_RESERVED_TT_DONE'"!]
  [!ELSE!]
    [!IF "node:exists(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId - 16) and (FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef)])"!]
      [!VAR "pathOfAssocTD" = "node:path(node:ref($timeDomainPath)/../*[(FrTSynGlobalTimeDomainId = node:ref($timeDomainPath)/FrTSynGlobalTimeDomainId - 16) and (FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef = node:ref($timeDomainPath)/FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef)])"!]
    [!ELSE!]
      [!VAR "pathOfAssocTD" = "$timeDomainPath"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDIF!]
[!IF "$pathOfAssocTD != ''"!]
  [!CALL "GetFrTSynTimeDomainsList", "Type"="$Type"!]
  [!VAR "tdIdx" = "num:i(0)"!]
  [!LOOP "text:split($sortedTimeDomainsList,',')"!]
    [!IF "$pathOfAssocTD = ."!]
      [!BREAK!]
    [!ELSE!]
      [!VAR "tdIdx" = "num:i($tdIdx + 1)"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!VAR "assocIdx" = "concat($tdIdx,'U')"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*
*********************************************************************************************************************
* This MACRO does the following:
  1)Takes every PDU reference in FrTSyn Slaves and searches in FrIf for a PDU with the same ECUC reference.
  2)Then we search in every FrameStructure for the PDU we found in FrIf (which is linked to the one in FrTSyn
  through ECUC). Every PDU-ECUC reference and FrameStructure pair is then saved in a list. In the list there
  will be only FrameStructures related to PDU's that come from FrTSyn.
  3)In case we have a Tx Pdu, the Pdu ID will be saved and added to the pair above. For Rx Pdu's a dummy will be
  added with the value "-1"
  4)After that we loop over all FrameTriggerings, we take the reference to the FrameStructure, and then
  try to find it in the list above, if found, a new entry is added in another list containing the PDU-ECUC
  reference, the controller index and the cluster index. If there are PDU's that belong to multiple FrameStructures,
  or FrameTriggerings which reference multiple FrameStructures, there could be the case that a PDU from FrTSyn
  ends up in different controllers, then an error is thrown.
  5)It will check(only in Master case) if the FrTSynGlobalTimeTxPeriod is smaller than the Cycle Duration of
  the respective Cluster(where the Pdu is linked), if yes, it could cause a violation of SWS_FrTSyn_00018, so
  a warning will be saved in the list, to be later popped.
**********************************************************************************************************************
*/!][!MACRO "PreparePduControllerAndClusterIdxList",""!][!NOCODE!]
    [!VAR "FrIfFrameStructureWithPduList"="''"!]
    [!LOOP "as:modconf('FrTSyn')[1]/FrTSynGlobalTimeDomain/*"!]
      [!VAR "isMaster" = "false()"!]
      [!IF "node:exists(./FrTSynGlobalTimeSlave)"!]
        [!VAR "PduRefPathFrTSyn" = "node:path(node:ref(./FrTSynGlobalTimeSlave/FrTSynGlobalTimeSlavePdu/FrTSynGlobalTimePduRef))"!]
        [!VAR "FrTSynGlobalTimeTxPeriod" = "num:f(-1)"!]
      [!ELSE!]
        [!VAR "PduRefPathFrTSyn" = "node:path(node:ref(./FrTSynGlobalTimeMaster/FrTSynGlobalTimeMasterPdu/FrTSynGlobalTimePduRef))"!]
        [!VAR "FrTSynGlobalTimeTxPeriod" = "num:f(node:value(./FrTSynGlobalTimeMaster/FrTSynGlobalTimeTxPeriod))"!]
        [!VAR "isMaster" = "true()"!]
      [!ENDIF!]
      [!VAR "FoundIds" = "false()"!]
      [!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfPdu/*"!]
        [!IF "node:exists(./FrIfPduDirection/FrIfRxPduRef)"!]
          [!VAR "PduRef" = "node:path(node:ref(./FrIfPduDirection/FrIfRxPduRef))"!]
          [!VAR "PduId" = "num:i(-1)"!]
          [!VAR "FrIfImmediate" = "false()"!]
        [!ELSE!]
          [!VAR "PduRef" = "node:path(node:ref(./FrIfPduDirection/FrIfTxPduRef))"!]
          [!VAR "PduId" = "node:value(./FrIfPduDirection/FrIfTxPduId)"!]
          [!IF "node:value(./FrIfPduDirection/FrIfImmediate) = 'true'"!]
            [!VAR "FrIfImmediate" = "true()"!]
          [!ELSE!]
            [!VAR "FrIfImmediate" = "false()"!]
          [!ENDIF!]
        [!ENDIF!]
        [!IF "$PduRef = $PduRefPathFrTSyn"!]
          [!VAR "FoundIds" = "true()"!]
          [!VAR "FrIfPdu" = "node:path(.)"!]
          [!IF "($isMaster = 'true') and ($PduId = num:i(-1))"!]
            [!ERROR!] The FrIf PDU '[!"$FrIfPdu"!] references a Master but is an FrIfRxPdu in FrIf' [!ENDERROR!]
          [!ENDIF!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$FoundIds = 'false'"!]
        [!ERROR!] The FrIf module does not reference the PDU '[!"$PduRefPathFrTSyn"!]' [!ENDERROR!]
      [!ENDIF!]
      [!VAR "FoundPdu" = "false()"!]
      [!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfFrameStructure/*/FrIfPdusInFrame/*"!]
          [!VAR "FrameStructurePdu" = "node:path(node:ref(./FrIfPduRef))"!]
          [!IF "$FrameStructurePdu = $FrIfPdu"!]
            [!VAR "FoundPdu" = "true()"!]
            [!VAR "FrIfFrameStructure" = "node:path(./../..)"!]
            [!VAR "FrIfFrameStructureWithPduList"!][!"$FrIfFrameStructureWithPduList"!];[!"$PduRefPathFrTSyn"!]:[!"$FrIfFrameStructure"!]:[!"$PduId"!]:[!"$FrTSynGlobalTimeTxPeriod"!]:[!"$FrIfImmediate"!][!ENDVAR!]
          [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$FoundPdu = 'false'"!]
        [!ERROR!] The FrIf PDU '[!"$FrIfPdu"!] does not belong to any FrameStructure' [!ENDERROR!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!VAR "FrIfPduControllerClusterIdxList"="''"!]
    [!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfFrameTriggering/*"!]
      [!VAR "FrIfControllerIdx" = "num:i(node:value(./../../FrIfCtrlIdx))"!]
      [!VAR "FrIfClusterIdx" = "num:i(node:value(./../../../../FrIfClstIdx))"!]
      [!VAR "FrIfGdCycle" = "num:f(node:value(./../../../../FrIfGdCycle))"!]
      [!VAR "FrameFrameStructureRef" = "node:path(node:ref(./FrIfFrameStructureRef))"!]
      [!LOOP "text:split($FrIfFrameStructureWithPduList,';')"!]
          [!VAR "PduIdTx" = "text:split(.,':')[3]"!]
          [!IF "string(text:split(.,':')[2]) = $FrameFrameStructureRef"!]
              [!VAR "Pdu" = "text:split(.,':')[1]"!]
              [!VAR "FrTSynTxPeriod" = "text:split(.,':')[4]"!]
              [!VAR "FrIfImmediateValue" = "text:split(.,':')[5]"!]
              [!LOOP "text:split($FrIfPduControllerClusterIdxList,';')"!]
                  [!IF "string(text:split(.,':')[1]) = $Pdu"!]
                      [!VAR "ExistingFrIfControllerIdx" = "text:split(.,':')[2]"!]
                      [!IF "$ExistingFrIfControllerIdx != $FrIfControllerIdx"!]
                          [!ERROR!] The FrIf PDU '[!"$Pdu"!] belongs to two or more different Controllers!' [!ENDERROR!]
                      [!ENDIF!]
                  [!ENDIF!]
              [!ENDLOOP!]
              [!VAR "FrIfPduControllerClusterIdxList"!][!"$FrIfPduControllerClusterIdxList"!];[!"$Pdu"!]:[!"$FrIfControllerIdx"!]:[!"$FrIfClusterIdx"!]:[!"$PduIdTx"!]:[!"$FrIfImmediateValue"!][!ENDVAR!]
          [!ENDIF!]
      [!ENDLOOP!]
    [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*********************************************************************************************************************
* This MACRO does the following:
    Takes the FrTSynGlobalTimePduRef from the current TimeDomain, and searches for it in the list created above,
    when found, the Controller index, the Cluster Index and the TxPduId from FrIf are then stored in separate
    variables, to be used after. Also if a TxPeriodWarning was found, it will be popped now.
*********************************************************************************************************************
<!-- !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00018,1 -->
*/!][!MACRO "GetControllerAndClusterIdx","FrTSynGlobalTimePduRef"!][!NOCODE!]
    [!VAR "PduRefPath" = "node:path(node:ref($FrTSynGlobalTimePduRef))"!]
    [!LOOP "text:split($FrIfPduControllerClusterIdxList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $PduRefPath"!]
          [!VAR "PduName" = "text:split(.,':')[1]"!]
          [!VAR "FrIfCtrlIdx" = "num:i(text:split(.,':')[2])"!]
          [!VAR "FrIfClstIdx" = "num:i(text:split(.,':')[3])"!]
          [!VAR "TxPduId" = "num:i(text:split(.,':')[4])"!]
          [!VAR "FrIfImmediateParam" = "text:split(.,':')[5]"!]
          [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*/!][!ENDIF!][!// FRTSYN_PRECOMPILEMACROS_M

[!ENDNOCODE!][!//
