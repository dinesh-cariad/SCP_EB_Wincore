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
[!IF "not(var:defined('ETHTSYN_MACROS_M'))"!]
[!VAR "ETHTSYN_MACROS_M"="'true'"!][!/*

=== Number of TimeDomains that have switch configured ===
*/!][!VAR "EthTSyn_TimeDomainsSwt" = "num:i(count(as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*[(node:exists(./EthTSynPortConfig)) and (node:refvalid(EthTSynSwitchManagementEthSwitchPortHostRef))]))"!][!/*
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

=== Macro to translate the configured timeout float into number of main function cycles. ===
Note: If value is not a multiple of the main function period, it is round down.
      Rounding down has the advantage for wait for send timeouts because a frame is then sent
      earlier and the receiver will not run into a timeout while waiting for the next frame.
*/!][!MACRO "TimeoutFloatToMFC", "value"!][!/*
  */!][!VAR "mValue" = "num:i($value div as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMainFunctionPeriod)"!][!//
        [!"num:i(floor($mValue))"!][!/* (spaces are relevant for output)
  */!][!/*
*/!][!ENDMACRO!][!/*

=== Macro to translate the configured Debounce float into number of main function cycles. ===
Note: If value is not a multiple of the main function period, it is round up.
*/!][!MACRO "DebounceFloatToMFC", "value"!][!/*
  */!][!VAR "mValue" = "($value div as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMainFunctionPeriod)"!][!//
        [!"num:i(ceiling($mValue))"!][!/* (spaces are relevant for output)
  */!][!/*
*/!][!ENDMACRO!][!/*

=== Macro calculate the message log interval byte (log base 2) ===
Note: The value is limited to 2^(+/-20), because this is enough for the range of the
      config interval parameters using this macro. A round up to the next higher message interval
      if period is not exact a 2^x value. This way it is ensured that the sender Tx period is always
      shorter as the receiver timeout.
*/!][!MACRO "LogMsgInterval", "value"!][!/*
  */!][!VAR "mValue" = "1"!][!/*
  */!][!VAR "Result" = "0"!][!/*
  */!][!IF "num:i($value) > "0.5" "!][!/* Positive exponent 
    */!][!FOR "Idx"="0" TO "20"!][!/* Start with 0 to cover range: 1 > value > 0,5.
      */!][!VAR "Result" = "$Idx"!][!/* LogMsgInterval => 2 ^ Idx
      */!][!IF "$mValue >= $value"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
      */!][!VAR "mValue" = "$mValue * 2"!][!/* mValue = 2 ^ (Idx + 1)
    */!][!ENDFOR!][!/*
  */!][!ELSE!][!/* Negative byte value
    */!][!FOR "Idx"="0" TO "20"!][!/*
      */!][!VAR "Result" = "255 - $Idx"!][!/* LogMsgInterval => 2 ^ -Idx (two's complement)
      */!][!VAR "mValue" = "$mValue div 2"!][!/*
      */!][!IF "$mValue <= $value"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
  */!][!ENDIF!][!//
        [!"num:i($Result)"!][!/* LogMsgInterval (spaces are relevant for output)
*/!][!ENDMACRO!][!/*

 === Macro used to convert from seconds to nanoseconds ===
*/!][!MACRO "PdelayNanoseconds", "value"!][!/*
  */!][!VAR "mValue" = "num:i($value * 1000000000)"!][!//
        [!"num:i(floor($mValue))"!][!/* (spaces are relevant for output)
  */!][!/*
*/!][!ENDMACRO!][!/*
[!/*

=== Number of switch ports configured ===
*/!][!VAR "SwtPortsConfigured"!][!CALL "EthTSyn_MaxSwtPorts"!][!ENDVAR!][!/*

*/!][!ENDIF!][!// ETHTSYN_MACROS_M
