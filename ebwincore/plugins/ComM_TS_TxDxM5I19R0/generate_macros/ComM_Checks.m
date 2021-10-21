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
[!INCLUDE "ComM_Functions.m"!][!//
[!/* These checks were originally present as INVALID checks in ComM.xdm.m4
   * Since these inter-module checks involve parameters from different
   * configuration classes, it's no more possible to keep them in ComM.xdm.m4
   * Refer ASCPROJECT-660 for more details.
   */!]

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('COMM_CHECKS_M'))"!]
[!VAR "COMM_CHECKS_M" = "'true'"!]

[!NOCODE!][!//

[!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*"!]
  [!IF "node:exists(ComMManageReference/*[1])"!]
    [!VAR "managingchannelid" = "node:ref(ComMManageReference/*[1])/ComMChannelId"!]
    [!VAR "managedchannelid" = "ComMChannelId"!]   
    [!VAR "managingchannelname" = "node:name(node:ref(ComMManageReference/*[1]))"!]
    [!VAR "managedchannelname" = "node:name(.)"!]
    [!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMUser/*"!]
      [!VAR "userid" = "ComMUserIdentifier"!]
      [!VAR "username" = "node:name(.)"!]
      [!VAR "manageduserfound" = "'false'"!]
      [!VAR "managinguserfound" = "'false'"!]
      [!IF "num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*[ComMChannelId = $managedchannelid][node:refs(ComMUserPerChannel/*/ComMUserChannel)[ComMUserIdentifier = $userid]])) > 0"!]
        [!VAR "manageduserfound" = "'true'"!]
      [!ENDIF!]
      [!IF "num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[node:refs(ComMChannelPerPnc/*)[ComMChannelId = $managedchannelid]][node:refs(ComMUserPerPnc/*)[ComMUserIdentifier = $userid]])) > 0"!]
        [!VAR "manageduserfound" = "'true'"!]
      [!ENDIF!]
      [!IF "$manageduserfound = 'true'"!]
        [!IF "num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*[ComMChannelId = $managingchannelid][node:refs(ComMUserPerChannel/*/ComMUserChannel)[ComMUserIdentifier = $userid]])) > 0"!]
          [!VAR "managinguserfound" = "'true'"!]
        [!ENDIF!]
        [!IF "num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[node:refs(ComMChannelPerPnc/*)[ComMChannelId = $managingchannelid]][node:refs(ComMUserPerPnc/*)[ComMUserIdentifier = $userid]])) > 0"!]
          [!VAR "managinguserfound" = "'true'"!]
        [!ENDIF!]
        [!IF "$manageduserfound != $managinguserfound"!]
          [!/* !LINKSTO SWS_ComM_CONSTR_VLAN_1,1 */!]        
          [!ERROR!] Managed channel: '[!"$managedchannelname"!]' references user (directly or via PNC): '[!"$username"!]' but managing channel: '[!"$managingchannelname"!]' does not references user (directly or via PNC) [!ENDERROR!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDLOOP!]
[!/* === Inter-module checks between ComM and Com, FrSM === */!]
[!/* === Following checks access configuration parameters with configuration class "PostBuild" === */!]

[!/* Ensure that the length of the referenced ComSignal is sufficient to store at least
     all PN info bits relevant for the PNC it is assigned to */!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport ='true'"!]
    [!CALL "macroGetPnOffset"!]
    [!IF "not(num:isnumber($macroRetValGetPnOffset)) = 'true'"!]
        [!ERROR!] "Could not setup the PNC offset. Consider enabling ComMPnInfoOffset or setup one of PnInfo in CAN/UDP/FR NM." [!ENDERROR!]
    [!ELSE!]
        [!/* !LINKSTO ComM.PartialNetworkOffset_MinComMPncId,1 */!]
        [!IF "not( ($macroRetValGetPnOffset >0) and ($macroRetValGetPnOffset <=31))"!]
                [!VAR "errMessage" = "concat('Retrived PN offset is ',string($macroRetValGetPnOffset))"!]
                [!ERROR!][!"$errMessage"!]: . Allowed range is [1:31]. Review ComMPnInfoOffset and busNm PnOffset.[!ENDERROR!][!//
        [!ENDIF!]
    [!ENDIF!]

    [!VAR "minPnId" = "num:i(8 * $macroRetValGetPnOffset)"!]
    [!IF "count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[ComMPncId < $minPnId ]) > 0"!]
        [!VAR "errMessage" = "concat('Min allowed ComMPncId in this configuration is ',string($minPnId))"!]
        [!ERROR!][!"$errMessage"!]: . Cosider reviewing ComMPncId, ComMPnInfoOffset and BusNmPnInfoOffset(if set).[!ENDERROR!][!//
    [!ENDIF!]

[!ENDIF!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport ='true'"!]
  [!VAR "BusNmPnInfoLength" = "num:i(255)"!]
  [!IF "node:exists(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoLength)"!]
  [!VAR "BusNmPnInfoLength" = "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoLength"!]
  [!ENDIF!]
  [!IF "$BusNmPnInfoLength = num:i(255)"!]
    [!IF "node:exists(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength)"!]
      [!VAR "BusNmPnInfoLength" = "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength"!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "$BusNmPnInfoLength = num:i(255)"!]
    [!IF "node:exists(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoLength)"!]
      [!VAR "BusNmPnInfoLength" = "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoLength"!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "$BusNmPnInfoLength = num:i(255)"!]
    [!CALL "GetPnInfoLength"!]
    [!VAR "BusNmPnInfoLength" = "$PnInfoLength"!]
  [!ENDIF!]
  [!/* !LINKSTO ComM.PartialNetworkOffset_MaxComMPncId,1 */!]
  [!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*"!]
    [!IF "ComMPncId > ((($macroRetValGetPnOffset + $BusNmPnInfoLength) * 8) - 1)"!]
      [!ERROR!]"PncId: [!"ComMPncId"!] is greater than ((PnInfoOffset + PnInfoLength) * 8) - 1"[!ENDERROR!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!SELECT "ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*/ComMPncComSignalRef"!]
  [!IF "(text:contains(string(node:ref(.)/ComSignalType), 'UINT8_') and num:i(as:ref(.)/ComSignalLength) < num:i(num:i(num:i(../../../ComMPncId) div 8) - $macroRetValGetPnOffset +1)) or (not(text:contains(string(as:ref(.)/ComSignalType), 'UINT8_')) and (as:ref(.)/ComBitSize <= num:i(num:i(../../../ComMPncId) - 8*$macroRetValGetPnOffset)))"!]
     [!ERROR!]"The length of the referenced ComSignal ('ComMPncComSignalRef' for the PNC "[!"node:name(../../..)"!]") is not sufficient to send / receive the EIRA!"
     [!ENDERROR!]
  [!ENDIF!]

[!ENDSELECT!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!]
  [!VAR "MasterPartitionRef" = "as:modconf('ComM')[1]/ComMGeneral/ComMMasterCoreEcuCPartitionRef"!]
  [!IF "count(as:modconf('Os')[1]/OsApplication/*[node:exists(OsAppEcucPartitionRef) and (OsAppEcucPartitionRef = $MasterPartitionRef)]/OsApplicationCoreAssignment) = 0"!]
    [!/* !LINKSTO ComM.EB.MasterCoreOsApplicationCoreAssignment,1 */!]
    [!ERROR!]"At least one OsApplication shall reference an EcuPartition configured in ComMMasterCoreEcuCPartitionRef parameter and OsApplicationCoreAssignment shall be enabled"
    [!ENDERROR!]
  [!ENDIF!]

  [!CALL "GetMasterCoreId"!]
  [!IF "num:i(count(text:split($MasterCoreId,'_'))) > 1"!]
    [!/* !LINKSTO ComM.EB.MasterCoreMultipleOsApplicationCoreAssignments,1 */!]
    [!ERROR!]"EcuC partition referenced by ComMMasterCoreEcuCPartitionRef was detected on more than one core"
    [!ENDERROR!]
  [!ENDIF!]
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
    [!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]
      [!VAR "SignalRef" = "as:modconf('ComM')[1]/ComMConfigSet/*/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection = 'RX'][node:ref(ComMPncComSignalRef)/ComHandleId = node:current()/ComHandleId]/ComMPncComSignalRef"!]
      [!IF "num:i(count(as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef])) = 0"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToPDU,1 */!]
        [!ERROR!]"Signal [!"node:name(.)"!] is not maped to an IPDU"
        [!ENDERROR!]
      [!ENDIF!]

      [!IF "not(node:refvalid(as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef]/../../ComMainFunctionRef))"!]
        [!/* !LINKSTO ComM.EB.PDUsMappedToMainfunctions,1 */!]
        [!ERROR!]"IPDU  which contains signal [!"node:name(.)"!] is not maped to a ComMainFunctionRef"
        [!ENDERROR!]
      [!ENDIF!]

      [!VAR "MainFunctionRef" = "as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef]/../../ComMainFunctionRef"!]
      [!VAR "PartitionRef" = "node:ref($MainFunctionRef)/ComPartitionRef"!]
      [!IF "count(as:modconf('Os')[1]/OsApplication/*[node:exists(OsAppEcucPartitionRef) and (OsAppEcucPartitionRef = $PartitionRef)]/OsApplicationCoreAssignment) = 0"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToOsApplicationCoreAssignment,1 */!]
        [!ERROR!]"At least one OsApplication shall reference an EcuPartition configured in IPDU which contains Signal [!"node:name(.)"!] and OsApplicationCoreAssignment shall be enabled"
        [!ENDERROR!]
      [!ENDIF!]

      [!VAR "FoundCore"="''"!]
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!]
      [!IF "num:i(count(text:split($FoundCore,'_'))) > 1"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToMultipleOsApplicationCoreAssignments,1 */!]
        [!ERROR!]"Signal [!"node:name(.)"!] shall be mapped to a single core (OsApplicationCoreAssignment)"
        [!ENDERROR!]
      [!ENDIF!]
    [!ENDLOOP!]

    [!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
      [!VAR "SignalRef" = "as:modconf('ComM')[1]/ComMConfigSet/*/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection = 'TX'][node:ref(ComMPncComSignalRef)/ComHandleId = node:current()/ComHandleId]/ComMPncComSignalRef"!]
      [!IF "num:i(count(as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef])) = 0"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToPDU,1 */!]
        [!ERROR!]"Signal [!"node:name(.)"!] is not maped to an IPDU"
        [!ENDERROR!]
      [!ENDIF!]

      [!IF "not(node:refvalid(as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef]/../../ComMainFunctionRef))"!]
        [!VAR "ErrorFound" = "'true'"!]
        [!/* !LINKSTO ComM.EB.PDUsMappedToMainfunctions,1 */!]
        [!ERROR!]"IPDU  which contains signal [!"node:name(.)"!] is not maped to a ComMainFunctionRef"
        [!ENDERROR!]
      [!ENDIF!]

      [!VAR "MainFunctionRef" = "as:modconf('Com')[1]/ComConfig/*/ComIPdu/*/ComIPduSignalRef/*[. = $SignalRef]/../../ComMainFunctionRef"!]
      [!VAR "PartitionRef" = "node:ref($MainFunctionRef)/ComPartitionRef"!]
      [!IF "count(as:modconf('Os')[1]/OsApplication/*[node:exists(OsAppEcucPartitionRef) and (OsAppEcucPartitionRef = $PartitionRef)]/OsApplicationCoreAssignment) = 0"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToOsApplicationCoreAssignment,1 */!]
        [!ERROR!]"At least one OsApplication shall reference an EcuPartition configured in IPDU which contains Signal [!"node:name(.)"!] and OsApplicationCoreAssignment shall be enabled"
        [!ENDERROR!]
      [!ENDIF!]

      [!VAR "FoundCore"="''"!]
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'TX'"!]
      [!IF "num:i(count(text:split($FoundCore,'_'))) > 1"!]
        [!/* !LINKSTO ComM.EB.SignalMappedToMultipleOsApplicationCoreAssignments,1 */!]
        [!ERROR!]"Signal [!"node:name(.)"!] shall be mapped to a single core (OsApplicationCoreAssignment)"
        [!ENDERROR!]
     [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDIF!]

[!ENDNOCODE!][!//

[!ENDIF!]

