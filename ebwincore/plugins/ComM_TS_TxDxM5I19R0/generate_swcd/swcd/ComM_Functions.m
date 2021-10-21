[!/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('COMM_FUNCTIONS_M'))"!]
[!VAR "COMM_FUNCTIONS_M" = "'true'"!]

[!NOCODE!][!//

[!MACRO "macroGetPnOffset"!][!/*
  */!][!VAR "macroRetValGetPnOffset"="'none'"!][!/*
    */!][!VAR "macroGetPnOffset_FoundBusNmOffset" = "'none'"!][!/*
    
    */!][!FOR "i" = "1" TO "num:i((count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*)))"!][!/*
        */!][!FOR "j" = "1" TO "num:i((count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[num:i($i)]/ComMChannelPerPnc/*)))"!][!/*
          */!][!IF "node:exists(as:modconf('Nm')[1]/NmChannelConfig/*[NmComMChannelRef = as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[num:i($i)]/ComMChannelPerPnc/*[num:i($j)]]/NmBusType/NmStandardBusType)"!][!/*
              */!][!IF "text:contains(string(as:modconf('Nm')[1]/NmChannelConfig/*[NmComMChannelRef = as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[num:i($i)]/ComMChannelPerPnc/*[num:i($j)]]/NmBusType/NmStandardBusType), 'CANNM')"!][!/*
                  */!][!IF "node:exists( as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo)"!][!/*
                      */!][!VAR "macroGetPnOffset_FoundBusNmOffset" = "num:i(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoOffset)"!][!/*
                      */!][!BREAK!][!/*
                  */!][!ENDIF!][!/*
              */!][!ENDIF!][!/*
              
              */!][!IF "text:contains(string(as:modconf('Nm')[1]/NmChannelConfig/*[NmComMChannelRef = as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[num:i($i)]/ComMChannelPerPnc/*[num:i($j)]]/NmBusType/NmStandardBusType), 'UDPNM')"!][!/*
                  */!][!IF "node:exists( as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo)"!][!/*
                      */!][!VAR "macroGetPnOffset_FoundBusNmOffset" = "num:i(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoOffset)"!][!/*
                      */!][!BREAK!][!/*
                  */!][!ENDIF!][!/*
              */!][!ENDIF!][!/*
              
              */!][!IF "text:contains(string(as:modconf('Nm')[1]/NmChannelConfig/*[NmComMChannelRef = as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[num:i($i)]/ComMChannelPerPnc/*[num:i($j)]]/NmBusType/NmStandardBusType), 'FRNM')"!][!/*
                  */!][!IF "node:exists( as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset)"!][!/*
                      */!][!VAR "macroGetPnOffset_FoundBusNmOffset" = "num:i(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset)"!][!/*
                      */!][!BREAK!][!/*
                  */!][!ENDIF!][!/*
              */!][!ENDIF!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDFOR!][!/*
        */!][!IF "num:isnumber($macroGetPnOffset_FoundBusNmOffset) and ($macroGetPnOffset_FoundBusNmOffset >0) "!][!/*
                */!][!BREAK!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
    
    */!][!IF "node:exists(as:modconf('ComM')[1]/ComMGeneral/ComMPnInfoOffset) ='true'"!][!/*
            */!][!VAR "macroRetValGetPnOffset"= "num:i(as:modconf('ComM')[1]/ComMGeneral/ComMPnInfoOffset)"!][!/*
    */!][!ELSE!][!/*
        */!][!IF "num:isnumber($macroGetPnOffset_FoundBusNmOffset)"!][!/*
            */!][!VAR "macroRetValGetPnOffset"="num:i($macroGetPnOffset_FoundBusNmOffset)"!][!/*
        */!][!ELSE!][!/*
            */!][!WARNING!] "Could not setup the PNC offset. Consider enabling ComMPnInfoOffset or setup one of PnInfo in CAN/UDP/FR NM." [!ENDWARNING!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDIF!][!//
[!ENDMACRO!][!//

[!MACRO "GetMasterCoreId"!][!//
[!VAR "MasterCoreId"="''"!][!//
[!VAR "PartitionRef" = "as:modconf('ComM')[1]/ComMGeneral/ComMMasterCoreEcuCPartitionRef"!][!//
[!LOOP "as:modconf('Os')[1]/OsApplication/*[node:exists(OsAppEcucPartitionRef) and (OsAppEcucPartitionRef = $PartitionRef)]"!][!/*
  */!][!IF "node:exists(OsApplicationCoreAssignment)"!][!/*
    */!][!VAR "MasterOsApplicationCoreAssignmentFound" = "'false'"!][!/*
    */!][!FOR "Idx" = "1" TO "count(text:split($MasterCoreId,'_'))"!][!/*
      */!][!IF "num:i(string(text:split($MasterCoreId,'_')[position() = $Idx])) = num:i(OsApplicationCoreAssignment)"!][!/*
        */!][!VAR "MasterOsApplicationCoreAssignmentFound" = "'true'"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
    */!][!IF "$MasterOsApplicationCoreAssignmentFound = 'false'"!][!/*
      */!][!IF "count(text:split($MasterCoreId,'_')) = 0"!][!/*
        */!][!VAR "MasterCoreId" = "num:i(OsApplicationCoreAssignment)"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "MasterCoreId" = "concat($MasterCoreId,'_')"!][!/*
        */!][!VAR "MasterCoreId" = "concat($MasterCoreId,num:i(OsApplicationCoreAssignment))"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDMACRO!][!//
[!MACRO "GetCore","SignalHandle","Direction"!][!//
[!VAR "SignalRef" = "as:modconf('ComM')[1]/ComMConfigSet/*/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection = $Direction][node:ref(ComMPncComSignalRef)/ComHandleId = $SignalHandle]/ComMPncComSignalRef"!][!//
[!VAR "MainFunctionRef" = "as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[node:exists(.) and (. = $SignalRef)]/../../ComMainFunctionRef"!][!//
[!VAR "PartitionRef" = "node:ref($MainFunctionRef)/ComPartitionRef"!][!//
[!LOOP "as:modconf('Os')[1]/OsApplication/*[node:exists(OsAppEcucPartitionRef) and (OsAppEcucPartitionRef = $PartitionRef)]"!][!/*
  */!][!IF "node:exists(OsApplicationCoreAssignment)"!][!/*
    */!][!VAR "SlaveOsApplicationCoreAssignmentFound" = "'false'"!][!/*
    */!][!FOR "Idx" = "1" TO "count(text:split($FoundCore,'_'))"!][!/*
      */!][!IF "num:i(string(text:split($FoundCore,'_')[position() = $Idx])) = num:i(OsApplicationCoreAssignment)"!][!/*
        */!][!VAR "SlaveOsApplicationCoreAssignmentFound" = "'true'"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
    */!][!IF "$SlaveOsApplicationCoreAssignmentFound = 'false'"!][!/*
      */!][!IF "count(text:split($FoundCore,'_')) = 0"!][!/*
        */!][!VAR "FoundCore" = "num:i(OsApplicationCoreAssignment)"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "FoundCore" = "concat($FoundCore,'_')"!][!/*
        */!][!VAR "FoundCore" = "concat($FoundCore,num:i(OsApplicationCoreAssignment))"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDMACRO!][!//

[!MACRO "GetUsedCores"!][!//
[!CALL "GetMasterCoreId"!][!//
[!VAR "FoundCore"="$MasterCoreId"!][!//
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='RX']/ComMPncComSignalRef),'ComHandleId')"!][!/*
  */!][!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
[!ENDLOOP!][!//
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef),'ComHandleId')"!][!/*
  */!][!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'TX'"!][!//
[!ENDLOOP!][!//
[!VAR "UsedCores"="$FoundCore"!][!//
[!ENDMACRO!][!//

[!MACRO "GetPnInfoLength"!][!//
[!VAR "SigSize1" = "0"!][!//
[!VAR "SigSize2" = "0"!][!//
[!/* Get the length of the largest Com signal for array-type signals */!]
[!IF "count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*/ComMPncComSignalRef)[text:contains(string(ComSignalType), 'UINT8_')]) != 0"!][!//
[!VAR "SigSize1" = "num:max(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*/ComMPncComSignalRef)[text:contains(string(ComSignalType), 'UINT8_')]/ComSignalLength)"!][!//
[!ENDIF!][!//
[!/* Get the length of the largest Com signal for non array-type signals */!]
[!IF "count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*/ComMPncComSignalRef)[not(text:contains(string(ComSignalType), 'UINT8_'))]) != 0"!][!//
[!VAR "SigSize2" = "num:max(node:foreach(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*/ComMPncComSignalRef)[not(text:contains(string(ComSignalType), 'UINT8_'))]/ComSignalType, 'SigType', 'node:when($SigType = "BOOLEAN", "8", substring-after($SigType, "T"))')) div 8 "!][!//
[!ENDIF!][!//
[!IF "$SigSize1 > $SigSize2"!][!//
[!VAR "PnInfoLength" = "num:i($SigSize1)"!][!//
[!ELSE!][!//
[!VAR "PnInfoLength" = "num:i($SigSize2)"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//

[!MACRO "GetNoOfTxBuffers"!][!//
[!VAR "NoOfTxBuffers" = "0"!][!//
[!CALL "GetMasterCoreId"!][!//
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!][!/*
  */!][!VAR "FoundCore" = "''"!][!/*
  */!][!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'TX'"!][!/*
  */!][!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!/*
    */!][!VAR "NoOfTxBuffers" = "$NoOfTxBuffers + 1"!][!/*
  */!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDMACRO!][!//

[!MACRO "GetNoOfRxBuffers"!][!//
[!VAR "NoOfRxBuffers" = "0"!][!//
[!CALL "GetMasterCoreId"!][!//
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!][!/*
  */!][!VAR "FoundCore" = "''"!][!/*
  */!][!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!/*
  */!][!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!/*
    */!][!VAR "NoOfRxBuffers" = "$NoOfRxBuffers + 1"!][!/*
  */!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDMACRO!][!//

[!MACRO "GetRxSignalIndex","SignalHandleId"!][!//
[!CALL "GetMasterCoreId"!][!//
[!VAR "RxSignalIndex" = "65535"!][!//
[!VAR "Cntr" = "0"!][!//
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!][!/*
  */!][!VAR "FoundCore" = "''"!][!/*
  */!][!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!/*
  */!][!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!/*
    */!][!IF "num:i(ComHandleId) = num:i($SignalHandleId)"!][!/*
      */!][!VAR "RxSignalIndex" = "$Cntr"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "Cntr" = "$Cntr + 1"!][!/*
  */!][!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDMACRO!][!//

[!ENDNOCODE!][!//
[!ENDIF!][!//