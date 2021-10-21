[!/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!// multiple inclusion protection
[!IF "not(var:defined('MIRROR_PRECOMPILEMACROS_M'))"!]
[!VAR "MIRROR_PRECOMPILEMACROS_M"="'true'"!][!/*

=== Number of the Mirror Dest Network ===
*/!][!VAR "Mirror_DestNetworkSize" = "num:i(count(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*))"!][!/*

=== Number of the Mirror Source Network ===
*/!][!VAR "Mirror_SourceNetworkSize" = "num:i(count(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*))"!][!/*

=== Number of the Mirror FlexRay Source Network ===
*/!][!VAR "Mirror_FlexRaySourceNetworkSize" = "num:i(count(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*[node:name(.) = 'MirrorSourceNetworkFlexRay']))"!][!/*

=== Number of the Mirror CAN Source Network ===
*/!][!VAR "Mirror_CanSourceNetworkSize" = "num:i(count(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*[node:name(.) = 'MirrorSourceNetworkCan']))"!]][!/*

=== Number of the Mirror LIN Source Network ===
*/!][!VAR "Mirror_LinSourceNetworkSize" = "num:i(count(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*[node:name(.) = 'MirrorSourceNetworkLin']))"!]][!/*

=== Mirror Main Func period ===
*/!][!VAR "MainFuncPeriod" = "node:value(as:modconf('Mirror')[1]/MirrorGeneral/MirrorMainFunctionPeriod) - 0.00000001"!][!/*

=== Mirror Initial Dest Network Index ===
*/!][!MACRO "InitialDestNetworkIndex"!][!NOCODE!]
      [!VAR "InitDestIndex" = "0"!]
      [!VAR "DestIndex" = "0"!]
      [!VAR "InitDestPath" = "node:path(node:ref(as:modconf('Mirror')[1]/MirrorConfigSet/MirrorInitialDestNetworkRef))"!]
      [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!]
        [!IF "node:path(.) = $InitDestPath"!]
          [!VAR "InitDestIndex" = "$DestIndex"!]
        [!ENDIF!]
        [!VAR "DestIndex" = "$DestIndex + 1"!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Mirror Check configuration ===
*/!][!MACRO "CheckConfiguration"!][!NOCODE!]
      [!VAR "TimeStampNeeded" = "0"!]
      [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!]
        [!IF "(node:name(.) = 'MirrorDestNetworkIp') or (node:name(.) = 'MirrorDestNetworkCdd') or (node:name(.) = 'MirrorDestNetworkFlexRay')"!]
          [!VAR "TimeStampNeeded" = "1"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!IF "($TimeStampNeeded = 1) and (node:empty(as:modconf('Mirror')[1]/MirrorGeneral/MirrorStbRef)) and (node:empty(as:modconf('Mirror')[1]/MirrorGeneral/MirrorCustomTimeFileName))"!]
      [!ERROR "Either StbM reference or custom time stamp must be used for the configured destination networks"!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Mirror validate network IDs ===
*/!][!MACRO "ValidateNetworkId", "NetworkType", "NetworkID", "NetworkName"!][!NOCODE!]
      [!VAR "IdFound" = "0"!]
      [!IF "$NetworkType = 'MirrorSourceNetworkCan'"!]
          [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!]
            [!IF "(node:name(.) = 'MirrorSourceNetworkCan') and (num:i($NetworkID) = num:i(./MirrorNetworkId))"!]
              [!VAR "IdFound" = "$IdFound + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!]
            [!IF "(node:name(.) = 'MirrorDestNetworkCan') and (num:i($NetworkID) = num:i(./MirrorNetworkId))"!]
              [!VAR "IdFound" = "$IdFound + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!IF "($IdFound > 1)"!]
              [!ERROR!]
              The network ID of [!"$NetworkName"!] isn't unique for CAN network type
              [!ENDERROR!]
          [!ENDIF!]
      [!ELSEIF "$NetworkType = 'MirrorSourceNetworkLin'"!]
          [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!]
            [!IF "(node:name(.) = 'MirrorSourceNetworkLin') and (num:i($NetworkID) = num:i(./MirrorNetworkId))"!]
              [!VAR "IdFound" = "$IdFound + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!IF "($IdFound > 1)"!]
              [!ERROR!]
              The network ID of [!"$NetworkName"!] isn't unique for LIN network type
              [!ENDERROR!]
          [!ENDIF!]
      [!ELSEIF "$NetworkType = 'MirrorSourceNetworkFlexRay'"!]
          [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!]
            [!IF "(node:name(.) = 'MirrorSourceNetworkFlexRay') and (num:i($NetworkID) = num:i(./MirrorNetworkId))"!]
              [!VAR "IdFound" = "$IdFound + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!IF "($IdFound > 1)"!]
              [!ERROR!]
              The network ID of [!"$NetworkName"!] isn't unique for FlexRay network type
              [!ENDERROR!]
          [!ENDIF!]
      [!ELSEIF "$NetworkType = 'MirrorDestNetworkIp'"!]
          [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!]
            [!IF "(node:name(.) = 'MirrorDestNetworkIp') and (num:i($NetworkID) = num:i(./MirrorNetworkId))"!]
              [!VAR "IdFound" = "$IdFound + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!IF "($IdFound > 1)"!]
              [!ERROR!]
              The network ID of [!"$NetworkName"!] isn't unique for IP network type
              [!ENDERROR!]
          [!ENDIF!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate SrcPdu in pdur for Dest Network PDU transmission path. ===      
*/!][!MACRO "TxIPduToSrcPdu", "value"!][!NOCODE!]
      [!VAR "IsPduFound" = "0"!]
      [!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*"!]
        [!IF "(node:path(node:ref(./PduRSrcPdu/PduRSrcPduRef))) = $value"!]
          [!VAR "SrcPduId" = "num:i(./PduRSrcPdu/PduRSourcePduHandleId)"!]
          [!VAR "IsPduFound" = "1"!]
          [!VAR "ActualTxNPduLength" = "num:i(node:ref(./PduRSrcPdu/PduRSrcPduRef)/PduLength)"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$IsPduFound = 0"!]
        [!ERROR "A destination I-PDU wasn't referenced in the PduR module as a Source PDU"!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to get the max queue and buffer size in bytes. ===      
*/!][!MACRO "MaxQueueAndBuffSize"!][!NOCODE!]
      [!VAR "MaxQueueSize" = "0"!]
      [!VAR "MaxBuffSize" = "0"!]
      [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!]
        [!VAR "PduRef" = "node:path(node:ref(./MirrorDestPdu/*[1]/MirrorDestPduRef))"!]
[!CALL "TxIPduToSrcPdu", "value" = "node:path(node:ref(./MirrorDestPdu/*[1]/MirrorDestPduRef))"!]
        [!VAR "BuffSize" = "(./MirrorDestQueueSize) * $ActualTxNPduLength "!]
        [!IF "num:i(./MirrorDestQueueSize) > $MaxQueueSize"!]
          [!VAR "MaxQueueSize" = "num:i(./MirrorDestQueueSize)"!]
        [!ENDIF!]
        [!IF "$BuffSize > $MaxBuffSize"!]
          [!VAR "MaxBuffSize" = "num:i($BuffSize)"!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to get the total number of controllers, channels or clusters associated with the source networks and their types. ===    
*/!][!MACRO "NumOfSrcControllers"!][!NOCODE!]
      [!VAR "SrcControllerCount" = "0"!]
      [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!]
        [!VAR "ComMChannel" = "node:path(node:ref(./MirrorComMNetworkHandleRef))"!]
        [!IF "node:name(.) = 'MirrorSourceNetworkCan'"!]
            [!LOOP "as:modconf('CanSM')[1]/CanSMConfiguration/*[1]/CanSMManagerNetwork/*"!]
                [!IF "(node:path(node:ref(./CanSMComMNetworkHandleRef))) = $ComMChannel"!]
                    [!VAR "NumOfControllers" = "num:i(count(./CanSMController/*))"!]
                    [!VAR "SrcControllerCount" = "$SrcControllerCount + $NumOfControllers"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSEIF "node:name(.) = 'MirrorSourceNetworkLin'"!]
            [!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*"!]
                [!IF "(node:path(node:ref(./LinIfComMNetworkHandleRef))) = $ComMChannel"!]
                    [!VAR "NumOfControllers" = "1"!]
                    [!VAR "SrcControllerCount" = "$SrcControllerCount + $NumOfControllers"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSEIF "node:name(.) = 'MirrorSourceNetworkFlexRay'"!]
            [!LOOP "as:modconf('FrSM')[1]/FrSMConfig/*[1]/FrSMCluster/*"!]
                [!IF "(node:path(node:ref(./FrSMComMNetworkHandleRef))) = $ComMChannel"!]
                    [!VAR "NumOfControllers" = "num:i(count(as:ref(FrSMFrIfClusterRef)/FrIfController/*))"!]
                    [!VAR "SrcControllerCount" = "$SrcControllerCount + $NumOfControllers"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to get the number of controllers for CAN source networks, Channels for LIN source networks and Clusters for FlexRay networks. ===      
*/!][!MACRO "SrcNetworkControllers", "ComMChannel", "NetworkType"!][!NOCODE!]
      [!VAR "TrcvCfg" = "0"!]
      [!VAR "TrcvID" = "0"!]
      [!VAR "ClusterIdx" = "0"!]
      [!IF "$NetworkType = 'MirrorSourceNetworkCan'"!]
      [!VAR "CanSmNetworkIndex" = "0"!]
        [!LOOP "as:modconf('CanSM')[1]/CanSMConfiguration/*[1]/CanSMManagerNetwork/*"!]
        [!VAR "CanSmNetworkIndex" = "$CanSmNetworkIndex + 1"!]
            [!IF "(node:path(node:ref(./CanSMComMNetworkHandleRef))) = $ComMChannel"!]
                [!VAR "CanSmNetwork" = "$CanSmNetworkIndex"!]
                [!VAR "NumOfControllers" = "num:i(count(./CanSMController/*))"!]
                [!IF "node:exists(./CanSMTransceiverId)"!]
                    [!VAR "TrcvCfg" = "1"!]
                    [!VAR "TrcvID" = "node:value(as:ref(./CanSMTransceiverId)/CanIfTrcvId)"!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
      [!ELSEIF "$NetworkType = 'MirrorSourceNetworkLin'"!]
        [!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*"!]
            [!IF "(node:path(node:ref(./LinIfComMNetworkHandleRef))) = $ComMChannel"!]
                [!VAR "LinChannelId" = "./LinIfChannelId"!]
                [!VAR "NumOfControllers" = "1"!]
                    [!IF "node:exists(./LinIfTransceiverDrvConfig)"!]
                        [!VAR "TrcvCfg" = "1"!]
                        [!VAR "TrcvID" = "node:value(as:ref(./LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/LinTrcvChannelId)"!]
                    [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
      [!ELSE!]
      [!VAR "FrSmNetworkIndex" = "0"!]
        [!LOOP "as:modconf('FrSM')[1]/FrSMConfig/*[1]/FrSMCluster/*"!]
        [!VAR "FrSmNetworkIndex" = "$FrSmNetworkIndex + 1"!]
            [!IF "(node:path(node:ref(./FrSMComMNetworkHandleRef))) = $ComMChannel"!]
                [!VAR "FrSmNetwork" = "$FrSmNetworkIndex"!]
                [!VAR "NumOfControllers" = "num:i(count(as:ref(FrSMFrIfClusterRef)/FrIfController/*))"!]
                [!VAR "ClusterIdx" = "num:i(as:ref(FrSMFrIfClusterRef)/FrIfClstIdx)"!]
            [!ENDIF!]
        [!ENDLOOP!]
      [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to check if a tranceiver is configured for the CanSourceNetworks and LinSourceNetworks. ===      
*/!][!MACRO "SrcNetworkCtrlTrcvCheck""!][!NOCODE!]
      [!VAR "CanTrcvCfg" = "0"!]
      [!VAR "LinTrcvCfg" = "0"!]
      [!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!]
        [!VAR "ComMChannel" = "node:path(node:ref(./MirrorComMNetworkHandleRef))"!]
        [!IF "node:name(.) = 'MirrorSourceNetworkCan'"!]
            [!LOOP "as:modconf('CanSM')[1]/CanSMConfiguration/*[1]/CanSMManagerNetwork/*"!]
                [!IF "(node:path(node:ref(./CanSMComMNetworkHandleRef))) = $ComMChannel"!]
                    [!IF "node:exists(./CanSMTransceiverId)"!]
                        [!VAR "CanTrcvCfg" = "1"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ELSEIF "node:name(.) = 'MirrorSourceNetworkLin'"!]
            [!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*"!]
                [!IF "(node:path(node:ref(./LinIfComMNetworkHandleRef))) = $ComMChannel"!]
                    [!IF "node:exists(./LinIfTransceiverDrvConfig)"!]
                        [!VAR "LinTrcvCfg" = "1"!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*/!][!ENDIF!][!// MIRROR_PRECOMPILEMACROS_M

[!ENDNOCODE!][!//