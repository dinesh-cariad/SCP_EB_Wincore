[!/**
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
 */!][!//
[!NOCODE!]
[!// multiple inclusion protection
[!IF "not(var:defined('CANTSYN_PRECOMPILEMACROS_M'))"!]
[!VAR "CANTSYN_PRECOMPILEMACROS_M"="'true'"!][!/*

=== Size of the CanTSyn Time Domains ===
*/!][!VAR "CanTSyn_TDSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*))"!][!/*

=== Size of The total CanTSyn Master Time Domains ===
*/!][!VAR "CanTSyn_TMSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*/CanTSynGlobalTimeMaster))"!][!/*

=== Size of The total CanTSyn Slave Time Domains ===
*/!][!VAR "CanTSyn_TSSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*/CanTSynGlobalTimeSlave))"!][!/*

=== Size of The CanTSyn SYNC Time Domains with Domain Id value less than 16 and with CRC support on Master or CRC support on Slave ===
*/!][!VAR "CanTSyn_SyncDomains" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(CanTSynGlobalTimeDomainId < 16) and (((node:exists(CanTSynGlobalTimeMaster)) and (node:value(CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED')) or ((node:exists(CanTSynGlobalTimeSlave)) and ((node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL'))))]))"!][!/*

=== Size of The CanTSyn OFFSET Time Domains with Domain Id value greather or equal than 16 with CRC support on Master or CRC support on Slave ===
*/!][!VAR "CanTSyn_OffsetDomains" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(CanTSynGlobalTimeDomainId > 15) and (((node:exists(CanTSynGlobalTimeMaster)) and (node:value(CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED')) or ((node:exists(CanTSynGlobalTimeSlave)) and ((node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL'))))]))"!][!/*

=== Size of The CanTSyn Master Time Domains with CRC support ===
*/!][!VAR "CanTSyn_CRCMasterDomains" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(node:exists(CanTSynGlobalTimeMaster)) and (node:value(CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED')]))"!][!/*

=== Size of The CanTSyn Slave Time Domains with CRC support ===
*/!][!VAR "CanTSyn_CRCSlaveDomains" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(node:exists(CanTSynGlobalTimeSlave)) and ((node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL'))]))"!][!/*

=== Size of The CanTSyn Master Time Domains with Domain less than 16 ===
*/!][!VAR "CanTSyn_SyncTMSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[node:exists(./CanTSynGlobalTimeMaster) and (CanTSynGlobalTimeDomainId<16)]))"!][!/*

=== Size of The CanTSyn Slave Time Domains with Domain less than 16 ===
*/!][!VAR "CanTSyn_SyncTSSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(node:exists(./CanTSynGlobalTimeSlave)) and (CanTSynGlobalTimeDomainId<16)]))"!][!/*

=== Size of The CanTSyn Master Time Domains with Domain greater than 15 ===
*/!][!VAR "CanTSyn_OffsetTMSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(node:exists(./CanTSynGlobalTimeMaster)) and (CanTSynGlobalTimeDomainId>15)]))"!][!/*

=== Size of The CanTSyn Slave Time Domains with Domain greater than 15 ===
*/!][!VAR "CanTSyn_OffsetTSSize" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(node:exists(./CanTSynGlobalTimeSlave)) and (CanTSynGlobalTimeDomainId>15)]))"!][!/*

=== Number of Tx time domains which have CanTSynUseExtendedMsgFormat enabled ===
*/!][!VAR "CanTSyn_NumTxExtendedMsgFormat" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*/CanTSynGlobalTimeMaster/*[../../CanTSynUseExtendedMsgFormat = 'true']))"!][!/*

=== Number of Rx time domains which have CanTSynUseExtendedMsgFormat enabled ===
*/!][!VAR "CanTSyn_NumRxExtendedMsgFormat" = "num:i(count(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*/CanTSynGlobalTimeSlave/*[../../CanTSynUseExtendedMsgFormat = 'true']))"!][!/*


=== Macro to translate the configured timeout float into number of main function cycles. ===
Note: If value is not a multiple of the main function period, it is round down.
      Rounding down has the advantage for wait for send timeouts because a frame is then sent
      earlier and the receiver will not run into a timeout while waiting for the next frame.

*/!][!MACRO "TimeoutFloatToMFC", "value"!][!NOCODE!]
[!VAR "mValue" = "num:i($value div as:modconf('CanTSyn')[1]/CanTSynGeneral/CanTSynMainFunctionPeriod)"!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate the configured Slave Time Domain ID. ===
*/!][!MACRO "SlaveTimeDomainId", "value"!][!NOCODE!]
[!VAR "mValue" = "num:i($value)"!]
[!IF "$mValue>15"!]
  [!VAR "mValue" = "num:i($value - 16)"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

===Macro to sort the Time Domain List based on the DomainId
*/!][!MACRO "GetCanTSynCRCSyncDomains", ""!]
    [!VAR "sortedDomainsList"="text:join(node:paths(node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(CanTSynGlobalTimeDomainId < 16) and (((node:exists(CanTSynGlobalTimeMaster)) and (node:value(CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED')) or ((node:exists(CanTSynGlobalTimeSlave)) and ((node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL'))))],'CanTSynGlobalTimeDomainId ')),',')"!]
[!ENDMACRO!][!/*

===Macro to sort the Time Domain List based on the DomainId
*/!][!MACRO "GetCanTSynCRCOffsetDomains", ""!]
    [!VAR "sortedDomainsList"="text:join(node:paths(node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[(CanTSynGlobalTimeDomainId > 15) and (((node:exists(CanTSynGlobalTimeMaster)) and (node:value(CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED')) or ((node:exists(CanTSynGlobalTimeSlave)) and ((node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL'))))],'CanTSynGlobalTimeDomainId ')),',')"!]
[!ENDMACRO!][!/*

=== Macro to sort the Master Time Domain List based on the CanTSynGlobalTimeMasterConfirmationHandleId. ===
*/!][!MACRO "GetCanTSynMasterDomainsList", ""!]
    [!VAR "sortedMasterDomainsList"="text:join(node:paths(node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[node:exists(./CanTSynGlobalTimeMaster)],'CanTSynGlobalTimeMaster/CanTSynGlobalTimeMasterPdu/CanTSynGlobalTimeMasterConfirmationHandleId')),',')"!]
[!ENDMACRO!][!/*

=== Macro to sort the Slave Time Domain List based on the CanTSynGlobalTimeSlaveHandleId. ===
*/!][!MACRO "GetCanTSynSlaveDomainsList", ""!]
    [!VAR "sortedSlaveDomainsList"="text:join(node:paths(node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[node:exists(./CanTSynGlobalTimeSlave)],'CanTSynGlobalTimeSlave/CanTSynGlobalTimeSlavePdu/CanTSynGlobalTimeSlaveHandleId')),',')"!]
[!ENDMACRO!][!/*

****************************************************************************************************
* Create a list of PDU names and the corresponding handle IDs.
* - CanIfTxRefToHandleIdList will hold all source PDU IDs of all CanIf TxPdus.
* Each list is realized as continuous string. The string contains tuples separated by ";". Each
* tuple contains two elements separated by ":". First element of tuple is the PDU reference. The
* Second element of the tuple is the corresponding handle ID.
* A list, for example, looks like this: Pdu_0:321;Pdu_1:322;Pdu_3:323;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]
[!VAR "CanIfTxRefToHandleIdList"="''"!]
[!// Iterate over all Tx PDUs to collect the PDU-IDs
[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*/CanIfTxPduCfg/*"!]
  [!// Get the source PDU-ID of a Tx PDU
  [!VAR "HandleId" = "./CanIfTxPduId"!]
  [!VAR "PduRef" = "node:path(node:ref(./CanIfTxPduRef))"!]
  [!VAR "CanIfTxRefToHandleIdList"!][!"$CanIfTxRefToHandleIdList"!];[!"$PduRef"!]:[!"$HandleId"!][!ENDVAR!]
  [!// Get the PDU-IDs of a Tx PDU

[!ENDLOOP!]


****************************************************************************************************
* Create a list of PDU names and the corresponding handle IDs.
* - CanIfRxRefToHandleIdList will hold all source PDU IDs of all CanIf RxPdus.
* Each list is realized as continuous string. The string contains tuples separated by ";". Each
* tuple contains two elements separated by ":". First element of tuple is the PDU reference. The
* Second element of the tuple is the corresponding handle ID.
* A list, for example, looks like this: Pdu_0:321;Pdu_1:322;Pdu_3:323;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]
[!VAR "CanIfRxRefToHandleIdList"="''"!]
[!// Iterate over all Rx PDUs to collect the PDU-IDs
[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*/CanIfRxPduCfg/*"!]
  [!// Get the source PDU-ID of a Rx PDU
  [!VAR "HandleId" = "./CanIfRxPduId"!]
  [!VAR "PduRef" = "node:path(node:ref(./CanIfRxPduRef))"!]
  [!VAR "CanIfRxRefToHandleIdList"!][!"$CanIfRxRefToHandleIdList"!];[!"$PduRef"!]:[!"$HandleId"!][!ENDVAR!]
  [!// Get the PDU-IDs of a Rx PDU

[!ENDLOOP!]


****************************************************************************************************
* Create a list of PDU names and the corresponding controller handle IDs.
* - CanIfTxRefToCtrlIdList will hold all source Controller IDs of all CanIf TxPdus.
* Each list is realized as continuous string. The string contains tuples separated by ";". Each
* tuple contains two elements separated by ":". First element of tuple is the PDU path. The
* second element of the tuple is the corresponding handle ID.
* A list, for example, looks like this: Pdu_0:321;Pdu_1:322;Pdu_3:323;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]
[!VAR "CanTSyn_CtrlSize" = "num:i(0)"!]
[!VAR "CanIfTxRefToCtrlIdList"="''"!]
[!// Iterate over all Tx PDUs to collect the Ctrl IDs
[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*/CanIfTxPduCfg/*"!]
  [!VAR "PduRef" = "node:path(node:ref(./CanIfTxPduRef))"!]
  [!VAR "CtrlIdx" = "as:ref(as:ref(as:ref(./CanIfTxPduBufferRef)/CanIfBufferHthRef/*[1])/CanIfHthCanCtrlIdRef)/CanIfCtrlId"!]
  [!VAR "CtrlIdFound" = "false()"!]

  [!LOOP "text:split($CanIfTxRefToCtrlIdList,';')"!]
    [!IF "text:split(.,':')[2] = $CtrlIdx"!]
      [!VAR "CtrlIdFound" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]


  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "node:exists(./CanTSynGlobalTimeMaster)"!]
      [!IF "$PduRef = node:path(node:ref(./CanTSynGlobalTimeMaster/CanTSynGlobalTimeMasterPdu/CanTSynGlobalTimePduRef))"!]
        [!IF "$CtrlIdFound = 'false'"!]
          [!VAR "CanTSyn_CtrlSize" = "$CanTSyn_CtrlSize + 1"!]
        [!ENDIF!]

        [!VAR "CanIfTxRefToCtrlIdList"!][!"$CanIfTxRefToCtrlIdList"!];[!"$PduRef"!]:[!"$CtrlIdx"!][!ENDVAR!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDLOOP!]


****************************************************************************************************
* Create a list of controller handle IDs and indexes the corresponding controllers.
* - CanIfIndexToCtrlIdList will hold all indexes of all Controller IDs.
* Each list is realized as continuous string. The string contains tuples separated by ";". Each
* tuple contains two elements separated by ":". First element of tuple is the Controller ID. The
* second element of the tuple is the corresponding index.
* A list, for example, looks like this: Pdu_0:321;Pdu_1:322;Pdu_3:323;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]
[!VAR "CanTSyn_CtrlIndex" = "num:i(0)"!]
[!VAR "CanIfIndexToCtrlIdList"="''"!]
[!// Iterate over all Tx PDUs to collect the Ctrl IDs
[!LOOP "as:modconf('CanIf')[1]/CanIfCtrlDrvCfg/*[1]/CanIfCtrlCfg/*"!]
  [!VAR "CtrlID" = "./CanIfCtrlId"!]

  [!LOOP "text:split($CanIfTxRefToCtrlIdList,';')"!]
    [!IF "text:split(.,':')[2] = $CtrlID"!]
      [!VAR "CanIfIndexToCtrlIdList"!][!"$CanIfIndexToCtrlIdList"!];[!"$CtrlID"!]:[!"$CanTSyn_CtrlIndex"!][!ENDVAR!]
      [!VAR "CanTSyn_CtrlIndex" = "$CanTSyn_CtrlIndex + 1"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDLOOP!]

[!/*
****************************************************************************************************
* MACRO to get the CanIfTxPduHandleId with which CanIf_Transmit() has to be called. The macro parameter
* "CanTSynGlobalTimePduRef" must reference this PDU. The PDU-ID is stored in the global variable "PduID".
*
* GetCanIfHandleId is called to check whether the PDU referenced by CanTSynGlobalTimePdu is
* also referenced by any of the CanIfTxPdu.
****************************************************************************************************
*/!]
[!MACRO "GetCanIfHandleId","CanTSynGlobalTimePduRef"!][!NOCODE!]
    [!VAR "PduRefPath" = "node:path(node:ref($CanTSynGlobalTimePduRef))"!]
      [!VAR "FoundIds" = "false()"!]
      [!LOOP "text:split($CanIfTxRefToHandleIdList,';')"!]
          [!IF "string(text:split(.,':')[1]) = $PduRefPath"!]
              [!VAR "PduId" = "text:split(.,':')[2]"!]
              [!VAR "FoundIds" = "true()"!]
              [!BREAK!]
          [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$FoundIds = 'false'"!]
        <!-- !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00027,1 -->
        [!ERROR!] The CanIf module does not reference the PDU '[!"$PduRefPath"!]' [!ENDERROR!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the CanIfCtrlId with which CanTSyn_SetTransmissionMode() will be called. The macro parameter
* "CanTSynGlobalTimePduRef" must reference this PDU. The controller ID is stored in the global variable "CtrlId".
****************************************************************************************************
*/!]
[!MACRO "GetCanIfCtrlId","CanTSynGlobalTimePduRef"!][!NOCODE!]
  [!VAR "PduRefPath" = "node:path(node:ref($CanTSynGlobalTimePduRef))"!]
  [!LOOP "text:split($CanIfTxRefToCtrlIdList,';')"!]
    [!IF "string(text:split(.,':')[1]) = $PduRefPath"!]
      [!VAR "CtrlId" = "text:split(.,':')[2]"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the GetCanIfCtrlIndex with which CanTSyn_SetTransmissionMode() will be called. The macro
* parameter "CtrlId" represents the handle ID of this controller. The controller index is stored in the
* global variable "CtrlIndex".
****************************************************************************************************
*/!]
[!MACRO "GetCanIfCtrlIndex","ControllerID"!][!NOCODE!]
  [!LOOP "text:split($CanIfIndexToCtrlIdList,';')"!]
    [!IF "string(text:split(.,':')[1]) = $ControllerID"!]
      [!VAR "CtrlIndex" = "num:i(text:split(.,':')[2])"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the value of CanTSynImmediateTimeSync parameter
*
* isImmediateTimeSync is called to check whether CanTSynImmediateTimeSync is TRUE or FALSE
****************************************************************************************************
*/!]
[!MACRO "isImmediateTimeSync","value"!][!NOCODE!]
  [!VAR "isImmediateTimeSync" = "num:i(0)"!]
  [!IF "$value = 'true'"!]
    [!VAR "isImmediateTimeSync" = "num:i(1)"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the value of CanTSynUseExtendedMsgFormat parameter
*
* isExtendedMessage is called to check whether CanTSynUseExtendedMsgFormat is TRUE or FALSE
****************************************************************************************************
*/!]
[!MACRO "isExtendedMessage","value"!][!NOCODE!]
  [!VAR "isExtendedMessage" = "num:i(0)"!]
  [!IF "$value = 'true'"!]
    [!VAR "isExtendedMessage" = "num:i(1)"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to check if a CanIfRxPdu is referencing the same Pdu as "CanTSynGlobalTimePduRef" must
* reference this PDU.
*
* CheckCanIfRxPdu is called to check whether the PDU referenced by CanTSynGlobalTimePdu is
* also referenced by any of the CanIfRxPdu.
****************************************************************************************************
*/!]
[!MACRO "CheckCanIfRxPdu","CanTSynGlobalTimePduRef"!][!NOCODE!]
    [!VAR "RxPduRefPath" = "node:path(node:ref($CanTSynGlobalTimePduRef))"!]
      [!VAR "FoundRxId" = "false()"!]
      [!LOOP "text:split($CanIfRxRefToHandleIdList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $RxPduRefPath"!]
          [!VAR "FoundRxId" = "true()"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$FoundRxId = 'false'"!]
        <!-- !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00040,1 -->
        [!ERROR!] The CanIf module does not reference the PDU '[!"$RxPduRefPath"!]' [!ENDERROR!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the information if the CRC is used for the Master feature.
****************************************************************************************************
*/!]
[!MACRO "IsTxCRCUsed",""!][!NOCODE!]
  [!VAR "isTxCRCUsed" = "false()"!]
  [!// Iterate over all TimeDomain to check if the CRC is used
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "node:exists(./CanTSynGlobalTimeMaster)"!]
      [!IF "node:value(./CanTSynGlobalTimeMaster/CanTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED'"!]
        [!VAR "isTxCRCUsed" = "true()"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if immediate transmission is used for the Master feature.
****************************************************************************************************
*/!]
[!MACRO "IsTxImmediateTransmissionUsed",""!][!NOCODE!]
  [!VAR "IsTxImmediateTransmissionUsed" = "false()"!]
  [!// Iterate over all TimeDomain to check if the Immediate Transmission is used
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "node:exists(./CanTSynGlobalTimeMaster)"!]
      [!IF "node:value(./CanTSynGlobalTimeMaster/CanTSynImmediateTimeSync) = 'true'"!]
        [!VAR "IsTxImmediateTransmissionUsed" = "true()"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if the CRC is used for the Slave feature.
****************************************************************************************************
*/!]
[!MACRO "IsRxCRCUsed",""!][!NOCODE!]
  [!VAR "isRxCRCUsed" = "false()"!]
  [!// Iterate over all TimeDomain to check if the CRC is used
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "node:exists(./CanTSynGlobalTimeSlave)"!]
      [!IF "(node:value(./CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(./CanTSynGlobalTimeSlave/CanTSynRxCrcValidated) = 'CRC_OPTIONAL')"!]
        [!VAR "isRxCRCUsed" = "true()"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if the CanTSynGlobalTimeFollowUpTimeout  is used
****************************************************************************************************
*/!]
[!MACRO "IsRxFollowUpTimeoutUsed",""!][!NOCODE!]
  [!VAR "IsRxFollowUpTimeoutUsed" = "false()"!]
  [!// Iterate over all TimeDomain to check if the CanTSynGlobalTimeFollowUpTimeout is used
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "node:exists(./CanTSynGlobalTimeSlave)"!]
      [!IF "node:value(./CanTSynGlobalTimeSlave/CanTSynGlobalTimeFollowUpTimeout) > 0"!]
        [!VAR "IsRxFollowUpTimeoutUsed" = "true()"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]



[!/*
****************************************************************************************************
* MACRO to get the information if the Tx Offset is used.
****************************************************************************************************
*/!]
[!MACRO "IsTxOffsetUsed",""!][!NOCODE!]
  [!VAR "isTxOffsetUsed" = "false()"!]
  [!IF "$CanTSyn_OffsetTMSize != 0"!]
    [!VAR "isTxOffsetUsed" = "true()"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if the Rx Offset is used.
****************************************************************************************************
*/!]
[!MACRO "IsRxOffsetUsed",""!][!NOCODE!]
  [!VAR "isRxOffsetUsed" = "false()"!]
  [!IF "$CanTSyn_OffsetTSSize != 0"!]
    [!VAR "isRxOffsetUsed" = "true()"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if the Tx Offset is used.
****************************************************************************************************
*/!]
[!MACRO "IsTxSyncUsed",""!][!NOCODE!]
  [!VAR "isTxSyncUsed" = "false()"!]
  [!IF "$CanTSyn_SyncTMSize != 0"!]
    [!VAR "isTxSyncUsed" = "true()"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* MACRO to get the information if the Rx Offset is used.
****************************************************************************************************
*/!]
[!MACRO "IsRxSyncUsed",""!][!NOCODE!]
  [!VAR "isRxSyncUsed" = "false()"!]
  [!IF "$CanTSyn_SyncTSSize != 0"!]
    [!VAR "isRxSyncUsed" = "true()"!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the information if the Time validation is enabled for any Master time domain.
****************************************************************************************************
*/!]
[!MACRO "IsMasterTimeValidationSupported",""!][!NOCODE!]
  [!VAR "isMasterTimeValidationSupported" = "false()"!]
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "(as:modconf('CanTSyn')[1]/CanTSynGeneral/CanTSynTimeValidationSupport = 'true') and (node:exists(./CanTSynGlobalTimeMaster)) and (./CanTSynEnableTimeValidation = 'true')"!]
      [!VAR "isMasterTimeValidationSupported" = "true()"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the information if the Time validation is enabled for any Slave time domain.
****************************************************************************************************
*/!]
[!MACRO "IsSlaveTimeValidationSupported",""!][!NOCODE!]
  [!VAR "isSlaveTimeValidationSupported" = "false()"!]
  [!LOOP "as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*"!]
    [!IF "(as:modconf('CanTSyn')[1]/CanTSynGeneral/CanTSynTimeValidationSupport = 'true') and (node:exists(./CanTSynGlobalTimeSlave)) and (./CanTSynEnableTimeValidation = 'true')"!]
      [!VAR "isSlaveTimeValidationSupported" = "true()"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

*/!][!ENDIF!][!// CANTSYN_PRECOMPILEMACROS_M

[!ENDNOCODE!][!//


