[!/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHTSYN_PRECOMPILE_M'))"!]
[!VAR "ETHTSYN_PRECOMPILE_M"="'true'"!][!/*

=== Maximum number of EthTSyn controller(number of time domains) ===
*/!][!VAR "EthTSyn_MaxCtrl" = "num:i(count(as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*[node:exists(./EthTSynPortConfig)]))"!][!/*

=== Maximum number of EthTSyn slaves ===
*/!][!VAR "EthTSyn_MaxSlave" = "num:i(count(as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*[(node:exists(./EthTSynPortConfig)) and (node:containsValue(./EthTSynPortRole, 'EthTSynGlobalTimeSlave'))]))"!][!/*

=== Number of TimeDomains that have switch configured ===
*/!][!VAR "EthTSyn_TimeDomainsSwt" = "num:i(count(as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*[(node:exists(./EthTSynPortConfig)) and (node:refvalid(EthTSynSwitchManagementEthSwitchPortHostRef))]))"!][!/*

=== Check if dem is used ===
*/!][!VAR "useDEM"="((as:modconf('EthTSyn')[1]/EthTSynReportError/EthTSynSyncFailedReportError = 'DEM') or (as:modconf('EthTSyn')[1]/EthTSynReportError/EthTSynPdelayFailedReportError = 'DEM') or (as:modconf('EthTSyn')[1]/EthTSynReportError/EthTSynUnexpectedSyncReportError = 'DEM')) "!][!/*
[!/*
****************************************************************************************************
* MACRO to get the information if the CRC is used for the Master feature.
****************************************************************************************************
*/!]
[!MACRO "EthTSyn_MaxSwtPorts",""!][!NOCODE!][!/*
  */!][!VAR "MaxSwtPorts" = "$EthTSyn_TimeDomainsSwt"!][!/*
  */!][!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!][!/*
    */!][!IF "(node:exists(./EthTSynPortConfig))"!][!/*
      */!][!LOOP "./EthTSynPortConfig/*"!][!/*
          */!][!IF "node:refvalid(./EthTSynSwitchManagementEthSwitchPortRef)"!][!/*
             */!][!VAR "MaxSwtPorts" = "$MaxSwtPorts + 1"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDNOCODE!][!"num:i($MaxSwtPorts)"!][!ENDMACRO!][!/*

[!/*
****************************************************************************************************
* MACRO to get the information if the CRC is used for the Master feature.
****************************************************************************************************
*/!]
[!MACRO "IsTxCRCUsed",""!][!NOCODE!]
  [!VAR "isTxCRCUsed" = "'false'"!]
  [!// Iterate over all TimeDomain to check if the CRC is used
  [!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!]
    [!IF "node:containsValue(./EthTSynPortRole, 'EthTSynGlobalTimeMaster')"!]
      [!IF "node:value(./EthTSynPortRole/EthTSynGlobalTimeTxCrcSecured) = 'CRC_SUPPORTED'"!]
        [!VAR "isTxCRCUsed" = "'true'"!]
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
  [!VAR "isRxCRCUsed" = "'false'"!]
  [!// Iterate over all TimeDomain to check if the CRC is used
  [!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!]
    [!IF "node:containsValue(./EthTSynPortRole, 'EthTSynGlobalTimeSlave')"!]
      [!IF "(node:exists(./EthTSynPortRole/EthTSynRxCrcValidated))"!]
        [!IF "(node:value(./EthTSynPortRole/EthTSynRxCrcValidated) = 'CRC_VALIDATED') or (node:value(./EthTSynPortRole/EthTSynRxCrcValidated) = 'CRC_OPTIONAL')"!]
          [!VAR "isRxCRCUsed" = "'true'"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

[!/*
****************************************************************************************************
* MACRO to get the information if the Switch is needed
****************************************************************************************************
*/!]
[!MACRO "IsSwtUsed",""!][!NOCODE!]
  [!VAR "IsSwtUsed" = "'false'"!]
  [!// Iterate over all TimeDomain to check if the Swt is used
  [!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!]
    [!IF "node:refvalid(EthTSynSwitchManagementEthSwitchPortHostRef)"!]
      [!IF "(EthTSynPortRole = 'EthTSynGlobalTimeMaster')"!]
        [!IF "node:refvalid(./EthTSynPortConfig//EthTSynSwitchManagementEthSwitchPortRef)"!]
          [!VAR "IsSwtUsed" = "'true'"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

[!/*
****************************************************************************************************
* MACRO to get the information if the Switch Bridge is used
****************************************************************************************************
*/!]
[!MACRO "IsSwtBridgeUsed",""!][!NOCODE!]
  [!VAR "IsSwtBridgeUsed" = "'false'"!]
  [!// Iterate over all TimeDomain to check if the Swt bridge is used
  [!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!]
    [!IF "((./EthTSynPortRole = 'EthTSynGlobalTimeSlave') and (count(./EthTSynPortConfig/*) > 1))"!]
      [!IF "node:refvalid(./EthTSynPortConfig//EthTSynSwitchManagementEthSwitchPortRef)"!]
        [!VAR "IsSwtBridgeUsed" = "'true'"!]
        [!BREAK!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*/!][!ENDIF!][!// ETHTSYN_PRECOMPILE_M
