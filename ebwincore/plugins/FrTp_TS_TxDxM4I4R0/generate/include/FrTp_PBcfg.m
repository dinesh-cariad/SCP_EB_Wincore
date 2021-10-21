[!/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]

[!// Only included by FrTp_PBcfg.c
[!// Also contains all code generator checks



[!IF "not(var:defined('FRTP_PBCFG_M'))"!]
[!VAR "FRTP_PBCFG_M"="'true'"!]

[!INCLUDE "FrTp_Variables.m"!]



[!// EXPORTED VARIABLES ----------------------------------------------------------------------------

[!VAR "SortedTxPduList" = "''"!][!// Contains comma-separated XPath of all FrTpTxPdus
                                [!// Sorted by FrTpTxPduPool (arbitrary order), FrIfSlotId (ascending), FrIfPduOffset (ascending)

[!VAR "MapTxPduPoolToStartIdx" = "''"!][!// Contains key-value pairs
                                       [!// Key: XPath of FrTpTxPduPool (string) 
                                       [!// Value: zero-based index of first FrTpTxPdu from this pool in SortedTxPduList (integer)
                                       [!// Access using GetFromMap/OutputFromMap macro
                                       [!// Format: <path1>,<index1>;<path2>,<index2>;

[!VAR "MapTxPduPoolToEndIdx" = "''"!][!// Contains key-value pairs
                                     [!// Key: XPath of FrTpTxPduPool (string) 
                                     [!// Value: zero-based index of last FrTpTxPdu from this pool in SortedTxPduList (integer)
                                     [!// Access using GetFromMap/OutputFromMap macro
                                     [!// Format: <path1>,<index1>;<path2>,<index2>;



[!// MACROS ----------------------------------------------------------------------------------------

[!// sets variable with name $OutVarName to value associated with parameter Key
[!MACRO "GetFromMap", "OutVarName", "Map", "Key"!][!/*
*/!][!"var:set($OutVarName, substring-before(substring-after($Map,concat($Key,',')),';'))"!][!/*
*/!][!ENDMACRO!]

[!// outputs value associated with parameter Key
[!MACRO "OutputFromMap", "Map", "Key"!][!/*
*/!][!"substring-before(substring-after($Map,concat($Key,',')),';')"!][!/*
*/!][!ENDMACRO!]

[!// Converts timeout [s] to main function ticks (as specified in [ecuc_sws_7000])
[!MACRO "TimeoutToTicks", "Time"!][!/*
*/!][!VAR "TimeoutToTicks_Time_Us" = "$Time * 1000000"!][!/*
*/!][!VAR "TimeoutToTicks_FrTpMainFuncCycle_Us" = "$FrTpMainFuncCycle * 1000000"!][!/*
*/!][!VAR "TimeoutToTicks_Remainder" = "num:i($TimeoutToTicks_Time_Us mod $TimeoutToTicks_FrTpMainFuncCycle_Us)"!][!/*
*/!][!IF "$TimeoutToTicks_Remainder != 0"!][!/*
    */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_FrTpMainFuncCycle_Us)+1)"!][!/*
    */!][!WARNING "A timeout is not a multiple of the main function cycle time. The number of ticks for the timeout will be rounded up."!][!/*    
*/!][!ELSE!][!/*
    */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_FrTpMainFuncCycle_Us))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!]



!// CONSISTENCY CHECKS, PART 1 OF 2 ----------------------------------------------------------------


[!// General consistency checks

[!// Container FrTpConnection checks

[!LOOP "FrTpMultipleConfig/*[1]/FrTpConnection/*"!]

  [!// Container FrTpTxSdu

  [!IF "not (node:exists(FrTpRxSdu)) and not(node:exists(FrTpTxSdu))"!]
    [!ERROR!]Either a FrTpRxSdu or a FrTpTxSdu has to be configured in connection [!"node:path(.)"!].[!ENDERROR!]
  [!ENDIF!]
  
  [!IF "node:exists(FrTpTxSdu)"!]
    [!// If a TxSdu is configured the following checks apply!]
    [!IF "not(node:refvalid(FrTpTxSdu/FrTpTxSduRef))"!]
        [!ERROR!][!"node:path(FrTpTxSdu/FrTpTxSduRef)"!]: Reference is invalid.[!ENDERROR!]
    [!ENDIF!]
  [!ENDIF!]

  [!// Container FrTpRxSdu

  [!IF "node:exists(FrTpRxSdu)"!]
    [!// If a RxSdu is configured the following checks apply!]
    [!IF "not(node:refvalid(FrTpRxSdu/FrTpRxSduRef))"!]
        [!ERROR!][!"node:path(FrTpRxSdu/FrTpRxSduRef)"!]: Reference is invalid.[!ENDERROR!]
    [!ENDIF!]
    
    [!IF "not(node:refvalid(FrTpRxPduPoolRef))"!]
        [!ERROR!][!"node:path(FrTpRxPduPoolRef)"!]: Reference is invalid.[!ENDERROR!]
    [!ENDIF!]
  [!ENDIF!]

  [!// Other FrTpConnection elements
  [!IF "not(node:refvalid(FrTpConCtrlRef))"!]
      [!ERROR!][!"node:path(FrTpConCtrlRef)"!]: Reference is invalid.[!ENDERROR!]
  [!ENDIF!]

[!ENDLOOP!]


[!// Container FrTpTxPduPool checks, part 1

[!LOOP "FrTpMultipleConfig/*[1]/FrTpTxPduPool/*"!]
    [!IF "not (node:containsValue(node:paths(node:refs(as:modconf('FrTp')[1]/FrTpMultipleConfig/*[1]/FrTpConnection/*/FrTpTxPduPoolRef)),node:path(.)))"!]
        [!WARNING!]FrTpTxPduPool [!"node:path(.)"!] is not used by any connection. Consider removing it to reduce the ROM consumption of the module configuration.[!ENDWARNING!]
    [!ENDIF!]

    [!LOOP "FrTpTxPdu/*"!]
        [!IF "not(node:refvalid(FrTpTxPduRef))"!]
            [!ERROR!][!"node:path(FrTpTxPduRef)"!]: Reference is invalid.[!ENDERROR!]
        [!ENDIF!]        
    [!ENDLOOP!]
[!ENDLOOP!]



[!// BUILD INTERNAL FrTpTxPdu MAPS ----------------------------------------------------------------- 

[!// Generate the following maps
[!//   Key: XPath of FrTpTxPduPool (string) 
[!//   Value: configuration parameter which is suffix of map name
[!//   Access using GetFromMap/OutputFromMap macro
[!//   Format: <path1>,<parameter1>;<path2>,<parameter2>;
[!VAR "MapTxPduToFrIfFrameTriggering" = "''"!]
[!VAR "MapTxPduToFrIfPduOffset" = "''"!]


[!LOOP "FrTpMultipleConfig/*[1]/FrTpTxPduPool/*/FrTpTxPdu/*"!]

    [!// Get FrIfFrameTriggering associated with current FrTpTxPdu
    [!// 
    [!// XPath query consists of
    [!// - selection of all FrIfFrameTriggerings
    [!// - predicate selecting the FrIfFrameTriggering of the frame containing the current FrTpTxPdu (enclosed in [], explained next)
    [!//   > starting from the FrIfFrameTriggering, (the dot)
    [!//   > dereference FrIfFrameStructureRef, node:ref(./FrIfFrameStructureRef)
    [!//   > select all references to all PDUs which are contained in the frame, (FrIfPdusInFrame/*)
    [!//   > dereference them, (first node:ref, dereferencing FrIfPduRef)
    [!//   > see if one of the FrIfTxPduRef equals the FrTpTxPdu reference (which comes after equality sign)
    [!VAR "FrIfFrameTriggering" = "node:path(as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfFrameTriggering/*[node:refs(node:ref(./FrIfFrameStructureRef)/FrIfPdusInFrame/*/FrIfPduRef)/FrIfPduDirection/FrIfTxPduRef = node:current()/FrTpTxPduRef])"!]

    [!VAR "MapTxPduToFrIfFrameTriggering" = "concat($MapTxPduToFrIfFrameTriggering, node:path(.), ',', $FrIfFrameTriggering, ';')"!]    
    
    [!// Get FrIfPduOffset associated with current FrTpTxPdu
    [!// 
    [!// XPath query consists of
    [!// - selection of all FrIfPdusInFrame (this container contains FrIfPduOffset)
    [!// - predicate selecting the FrIfPdusInFrame referring to the current FrTpTxPdu (enclosed with [], explained next)
    [!//   > starting from the FrIfPduRef, (the dot)
    [!//   > dereference FrIfPduRef
    [!//   > see if one of the FrIfTxPduRef equals the FrTpTxPdu reference (which comes after equality sign)
    [!// - /FrIfPduOffset
    [!VAR "FrIfPduOffset" = "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfFrameStructure/*/FrIfPdusInFrame/*[node:ref(./FrIfPduRef)/FrIfPduDirection/FrIfTxPduRef = node:current()/FrTpTxPduRef]/FrIfPduOffset"!]
    
    [!VAR "MapTxPduToFrIfPduOffset" = "concat($MapTxPduToFrIfPduOffset, node:path(.), ',', $FrIfPduOffset, ';')"!]    

[!ENDLOOP!]



!// CONSISTENCY CHECKS, PART 2 OF 2 (using FrTpTxPdu maps) -----------------------------------------  


[!// Container FrTpTxPduPool checks, part 2
    
[!LOOP "FrTpMultipleConfig/*[1]/FrTpTxPduPool/*"!]
    [!// Further checks of FrIf configuration associated to FrTpTxPdus of current FrTpTxPduPool

    [!// Get reference values from first container    
    [!CALL "GetFromMap", "OutVarName"="'FrIfFrameTriggeringReference'", "Map"="$MapTxPduToFrIfFrameTriggering", "Key"="node:path(FrTpTxPdu/*[1])"!]
    
    [!VAR "FrIfBaseCycleReference"="node:ref($FrIfFrameTriggeringReference)/FrIfBaseCycle"!]
    [!VAR "FrIfCycleRepetitionReference"="node:ref($FrIfFrameTriggeringReference)/FrIfCycleRepetition"!]
    [!VAR "FrIfChannelReference"="node:ref($FrIfFrameTriggeringReference)/FrIfChannel"!]
    
    [!// Take path of associated cluster
    [!VAR "FrIfClusterReference"="node:path(node:ref($FrIfFrameTriggeringReference)/../../../..)"!]
    
    [!LOOP "FrTpTxPdu/*"!]
        [!CALL "GetFromMap", "OutVarName"="'FrIfFrameTriggering'", "Map"="$MapTxPduToFrIfFrameTriggering", "Key"="node:path(.)"!]
        
        [!IF "node:ref($FrIfFrameTriggering)/FrIfBaseCycle != $FrIfBaseCycleReference"!]
            [!ERROR!]The parameters FrIfBaseCycle associated with [!"node:path(../*[1])"!] and [!"node:path(.)"!] differ. They must be equal to guarantee in-order reception. The associated paths are [!"$FrIfFrameTriggeringReference"!] and [!"$FrIfFrameTriggering"!].[!ENDERROR!]
        [!ENDIF!]
        
        [!IF "node:ref($FrIfFrameTriggering)/FrIfCycleRepetition != $FrIfCycleRepetitionReference"!]
            [!ERROR!]The parameters FrIfCycleRepetition associated with [!"node:path(../*[1])"!] and [!"node:path(.)"!] differ. They must be equal to guarantee in-order reception. The associated paths are [!"$FrIfFrameTriggeringReference"!] and [!"$FrIfFrameTriggering"!].[!ENDERROR!]
        [!ENDIF!]
        
        [!// Compare FrIfSlotId to FrIfGNumberOfStaticSlots from associated cluster
        [!IF "node:ref($FrIfFrameTriggering)/FrIfSlotId > node:ref($FrIfFrameTriggering)/../../../../FrIfGNumberOfStaticSlots"!]
            [!// This frame is in the dynamic segment
            [!IF "node:ref($FrIfFrameTriggering)/FrIfChannel = 'FRIF_CHANNEL_AB'"!]
                [!ERROR!]The parameter FrIfChannel associated with [!"node:path(.)"!] has value 'FRIF_CHANNEL_AB', which is only allowed in the static segment. The associated path is [!"$FrIfFrameTriggering"!].[!ENDERROR!]
            [!ENDIF!] 
        [!ENDIF!]
        
        [!IF "node:ref($FrIfFrameTriggering)/FrIfChannel != $FrIfChannelReference"!]
            [!ERROR!]The parameters FrIfChannel associated with [!"node:path(../*[1])"!] and [!"node:path(.)"!] differ. They must be equal to guarantee in-order reception. The associated paths are [!"$FrIfFrameTriggeringReference"!] and [!"$FrIfFrameTriggering"!].[!ENDERROR!]
        [!ENDIF!]                
        
        [!IF "node:path(node:ref($FrIfFrameTriggering)/../../../..) != $FrIfClusterReference"!]
            [!ERROR!][!"node:path(../*[1])"!] and [!"node:path(.)"!] reference different clusters. The associated paths are [!"$FrIfFrameTriggeringReference"!] and [!"$FrIfFrameTriggering"!].[!ENDERROR!]
        [!ENDIF!]        
    [!ENDLOOP!]
[!ENDLOOP!]



[!// CALCULATION OF EXPORTED VARIABLES -------------------------------------------------------------

[!VAR "TxPduCounter" = "0"!][!// Counts Pdus added to SortedTxPduList

[!// Loop over all TxPduPools
[!LOOP "FrTpMultipleConfig/*[1]/FrTpTxPduPool/*"!]

    [!VAR "MapTxPduPoolToStartIdx" = "concat($MapTxPduPoolToStartIdx, node:path(.), ',', num:i($TxPduCounter), ';')"!]

    [!// Sort current TxPduPool
    [!// using variant of selection sort algorithm, adding smallest element not yet present in SortedTxPduList
    [!FOR "i" = "1" TO "count(FrTpTxPdu/*)"!]
        [!VAR "ElementSelected" = "'false'"!]
        [!LOOP "FrTpTxPdu/*[not(contains($SortedTxPduList, concat(node:path(.), ',')))]"!]
            [!CALL "GetFromMap", "OutVarName"="'FrIfFrameTriggering'", "Map"="$MapTxPduToFrIfFrameTriggering", "Key"="node:path(.)"!]
            [!VAR "FrIfSlotId"="node:ref($FrIfFrameTriggering)/FrIfSlotId"!]
            [!CALL "GetFromMap", "OutVarName"="'FrIfPduOffset'", "Map"="$MapTxPduToFrIfPduOffset", "Key"="node:path(.)"!]
            
            [!IF "$ElementSelected = 'false'"!] 
                [!VAR "SelectElement" = "'true'"!][!// there is no element to compare against, yet
            [!ELSE!]
                [!// is current element the minimal element seen so far?
                [!VAR "SelectElement" = "($FrIfSlotId < $MinFrIfSlotId) or (($FrIfSlotId = $MinFrIfSlotId) and ($FrIfPduOffset < $MinFrIfPduOffset))"!]
            [!ENDIF!]
            
            [!IF "$SelectElement = 'true'"!]
                [!VAR "Element" = "node:path(.)"!]
                [!VAR "MinFrIfSlotId" = "$FrIfSlotId"!]
                [!VAR "MinFrIfPduOffset" = "$FrIfPduOffset"!]
                [!VAR "ElementSelected" = "'true'"!]
            [!ENDIF!]
        [!ENDLOOP!]
        
        [!VAR "SortedTxPduList" = "concat($SortedTxPduList, $Element, ',')"!]
        [!VAR "TxPduCounter" = "$TxPduCounter+1"!]
    [!ENDFOR!]
    
    [!VAR "MapTxPduPoolToEndIdx" = "concat($MapTxPduPoolToEndIdx, node:path(.), ',', num:i($TxPduCounter - 1), ';')"!]
    
[!ENDLOOP!]

[!ENDIF!][!// multiple inclusion protection

[!ENDNOCODE!]
