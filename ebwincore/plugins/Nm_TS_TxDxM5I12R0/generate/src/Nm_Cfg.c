/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Nm247_Refine,1 */

/*==================[inclusions]============================================*/

[!AUTOSPACING!][!//
[!INCLUDE "../include/Nm_Cfg.m"!][!//

#include <Nm_Cfg.h>
#include <Nm.h>                 /* Nm public API (own interface) */
#include <Nm_Int.h>             /* Nm internal symbols (own interface) */

[!IF "not(($BusNmNum > 1)
or (NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'false'))"!][!//
#include <TSCompiler.h>
/* For the given configuration this translation unit is empty.  The following
 * macro prevents compiler errors or warnings in this case */
TS_PREVENTEMPTYTRANSLATIONUNIT
[!ENDIF!][!//

#include <SchM_Nm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define NM_START_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

[!INDENT "0"!][!//
[!IF "($BusNmNum > 1) or
      (NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true') or
      (count(NmChannelConfig/*[NmStateReportEnabled = 'true']) != 0) or
      (num:i(count(NmChannelConfig/*)) > 1)"!][!//
/* Array holding the channel configurations */
CONST(Nm_ChannelConfigType, NM_CONST) Nm_ChannelConfig[NM_CHANNEL_NUM] =
{
  [!LOOP "NmChannelConfig/*"!][!//
    [!WS "2"!]{
      [!IF "as:modconf('Nm')[1]/NmGlobalConfig/NmGlobalFeatures/NmMultiCoreSupport='true'"!]
        [!IF "(node:exists(NmBusType/NmStandardBusType)) and
              ((NmBusType/NmStandardBusType='NM_BUSNM_CANNM') or
               (NmBusType/NmStandardBusType='NM_BUSNM_FRNM') or
               (NmBusType/NmStandardBusType='NM_BUSNM_UDPNM')
              )"!]
          [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_PassiveStartUp_[!"node:name(.)"!],
          [!IF "count(as:modconf('Nm')[1]/NmChannelConfig/*[NmPassiveModeEnabled='false']) != 0"!]
            [!IF "NmPassiveModeEnabled='false'"!]
              [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_NetworkRequest_[!"node:name(.)"!],
              [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_NetworkRelease_[!"node:name(.)"!],
#if (EB_NM_STACK == STD_ON)
              [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_NetworkGwEraRequest_[!"node:name(.)"!],
#endif /* (EB_NM_STACK == STD_ON) */
            [!ELSE!]
              [!WS "4"!]NULL_PTR,
              [!WS "4"!]NULL_PTR,
#if (EB_NM_STACK == STD_ON)
              [!WS "4"!]NULL_PTR,
#endif /* (EB_NM_STACK == STD_ON) */
            [!ENDIF!]
          [!ENDIF!]
        [!ELSE!]
          [!WS "4"!]&[!"NmBusType/NmGenericBusNmPrefix"!]_PassiveStartUp,
          [!IF "count(as:modconf('Nm')[1]/NmChannelConfig/*[NmPassiveModeEnabled='false']) != 0"!]
            [!IF "NmPassiveModeEnabled='false'"!]
              [!WS "4"!]&[!"NmBusType/NmGenericBusNmPrefix"!]_NetworkRequest,
              [!WS "4"!]&[!"NmBusType/NmGenericBusNmPrefix"!]_NetworkRelease,
            [!ELSE!]
              [!WS "4"!]NULL_PTR,
              [!WS "4"!]NULL_PTR,
            [!ENDIF!]
          [!ENDIF!]
        [!ENDIF!]
        [!IF "count(as:modconf('Nm')[1]/NmChannelConfig/*[NmPassiveModeEnabled='false']) != 0"!]
          [!IF "as:modconf('Nm')[1]/NmGlobalConfig/NmGlobalFeatures/NmBusSynchronizationEnabled='true'"!]
            [!IF "NmPassiveModeEnabled='false'"!]
              [!IF "(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType='NM_BUSNM_FRNM')"!]
                 [!IF "(node:exists(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmBusSynchronizationEnabled)) and
                       (as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmBusSynchronizationEnabled='true')"!]
                   [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_BusNm_RequestBusSynchronization_[!"node:name(.)"!],
                 [!ELSE!]
                   [!WS "4"!]NULL_PTR,
                 [!ENDIF!]
              [!ELSE!]
                 [!IF "(node:exists(NmBusType/NmStandardBusType)) and
                       ((NmBusType/NmStandardBusType='NM_BUSNM_CANNM') or
                        (NmBusType/NmStandardBusType='NM_BUSNM_UDPNM')
                       )"!]
                   [!WS "4"!]&SchM_Call_Nm_RequiredCSEntry_Nm_BusNm_RequestBusSynchronization_[!"node:name(.)"!],
                 [!ELSE!]
                   [!WS "4"!]&[!"NmBusType/NmGenericBusNmPrefix"!]_RequestBusSynchronization,
                 [!ENDIF!]
              [!ENDIF!]
            [!ELSE!]
              [!WS "4"!]NULL_PTR,
            [!ENDIF!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDIF!]
      [!IF "../../NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true'"!][!//
        [!VAR "ShutdownTimer" = "num:i((NmShutdownDelayTimer * 1000) div (../../NmGlobalConfig/NmGlobalProperties/NmCycletimeMainFunction * 1000))"!]
        [!IF "$ShutdownTimer != 0"!]
          [!WS "4"!][!"$ShutdownTimer"!]U, /* shutdown time in main func cycles */
        [!ELSE!]
          [!/* If NmShutdownDelayTimer = 0, we set the timer value to 1.
               This is to ensure that BusNm_NetworkRelease is called once
               and only once.*/!]
          [!WS "4"!]1U,  /* shutdown time in main func cycles */
        [!ENDIF!][!//
        [!IF "node:exists(NmWaitForShutdownTime)"!]
          [!WS "4"!][!"num:i((NmWaitForShutdownTime * 1000) div (../../NmGlobalConfig/NmGlobalProperties/NmCycletimeMainFunction * 1000))"!]U, /* WaitForShutdownTime */
        [!ELSE!]
          [!WS "4"!]0U, /* WaitForShutdownTime */
        [!ENDIF!]
      [!ENDIF!][!//
      [!IF "count(../*[NmStateReportEnabled = 'true']) != 0"!]
        [!IF "(NmStateReportEnabled = 'true') and (node:exists(as:ref(NmStateReportSignalRef)/ComHandleId))"!]
          [!WS "4"!][!"num:i(as:ref(NmStateReportSignalRef)/ComHandleId)"!]U,  /* ComHandleId */
        [!ELSE!]
          [!WS "4"!]NM_COM_SIGNAL_INVALID,  /* No Com Signal reference */
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "$BusNmNum > 1"!][!//
        [!IF "node:exists(NmBusType/NmStandardBusType)"!][!//
          [!IF "(NmBusType/NmStandardBusType = 'NM_BUSNM_CANNM')"!][!//
            [!WS "4"!][!"num:i($CanNmId)"!]U, /* CanNm Id */
          [!ELSE!][!//
            [!IF "(NmBusType/NmStandardBusType = 'NM_BUSNM_FRNM')"!][!//
              [!WS "4"!][!"num:i($FrNmId)"!]U, /* FrNm Id */
            [!ELSE!][!//
              [!/* here the condition NmBusType/NmStandardBusType =
                  NM_BUSNM_UDPNM is assured by the xdm check on the
                  NmStandardBusType parameter */!][!//
              [!WS "4"!][!"num:i($UdpNmId)"!]U, /* UdpNm Id */
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
        [!/* generic Nm */!][!//
        [!VAR "GenericBusNmName" = "''"!][!//
        [!FOR "arrIndx" = "1" TO "count(text:split($GenericUniqNm))"!][!//
          [!VAR "GenericBusNmName" = "text:split($GenericUniqNm)[position() = $arrIndx]"!][!//
          [!IF "$GenericBusNmName = node:value(NmBusType/NmGenericBusNmPrefix)"!][!//
            [!WS "4"!][!"num:i(number(text:split($GenericNmId)[position() = $arrIndx]))"!]U, /* [!"$GenericBusNmName"!] Id (Generic Bus)*/
          [!ENDIF!][!//
        [!ENDFOR!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "../../NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true'"!][!//
        [!IF "node:exists(NmCoordClusterIndex)"!][!//
          [!WS "4"!][!"NmCoordClusterIndex"!]U, /* cluster index */
        [!ELSE!][!//
          [!WS "4"!]NM_CC_IDX_INVALID, /* network is not part of cluster */
        [!ENDIF!][!//
        [!IF "NmChannelSleepMaster = 'true'"!][!//
          [!WS "4"!]TRUE,    /* NmChannelSleepMaster State */
        [!ELSE!][!//
          [!WS "4"!]FALSE,   /* NmChannelSleepMaster State */
        [!ENDIF!][!//
        [!IF "NmSynchronizingNetwork = 'true'"!][!//
          [!WS "4"!]TRUE,    /* NmSynchronizingNetwork */
        [!ELSE!][!//
          [!WS "4"!]FALSE,   /* NmSynchronizingNetwork */
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "NmPassiveModeEnabled = 'true'"!][!//
        [!WS "4"!]TRUE,      /* Passive mode */
      [!ELSE!]
        [!WS "4"!]FALSE,     /* Passive mode */
      [!ENDIF!][!//
      [!IF "NmNodeIdEnabled = 'true'"!][!//
        [!WS "4"!]TRUE,      /* NmNodeIdEnabled */
      [!ELSE!]
        [!WS "4"!]FALSE,     /* NmNodeIdEnabled */
      [!ENDIF!][!//
      [!IF "NmNodeDetectionEnabled = 'true'"!][!//
        [!WS "4"!]TRUE,      /* NmNodeDetectionEnabled */
      [!ELSE!]
        [!WS "4"!]FALSE,     /* NmNodeDetectionEnabled */
      [!ENDIF!][!//
    [!WS "2"!]},
  [!ENDLOOP!][!//
};
[!ENDIF!][!//
[!ENDINDENT!][!//

[!IF "NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true'"!][!//

[!// take greatest cluster index for all coordinated Nm channels
[!VAR "HighestClusterIndex"
 = "num:order(NmChannelConfig/*[node:exists(NmCoordClusterIndex)]/NmCoordClusterIndex)[last()]"!][!//
[!FOR "ClusterIndex" = "0" TO "$HighestClusterIndex"!][!//
STATIC CONST(NetworkHandleType, NM_CONST) Nm_CcNmChannels_[!"$ClusterIndex"!][!//
[[!"num:i(count(NmChannelConfig/*[(node:exists(NmCoordClusterIndex)) and (NmCoordClusterIndex = $ClusterIndex)]))"!]] =
{
[!/* ordered list of channel IDs for channels belonging to given cluster
*/!][!//
  [!"text:join(num:order(
NmChannelConfig/*[(node:exists(NmCoordClusterIndex)) and (NmCoordClusterIndex = $ClusterIndex)]/NmChannelId),'U, ')"!]U
};
[!ENDFOR!][!//

CONSTP2CONST(NetworkHandleType, NM_CONST, NM_CONST) Nm_CcNmChannels[NM_CC_NUM]=
{
[!FOR "ClusterIndex" = "0" TO "$HighestClusterIndex"!][!//
  Nm_CcNmChannels_[!"$ClusterIndex"!],
[!ENDFOR!][!//
};

[!ENDIF!][!//
[!//
[!SELECT "NmGlobalConfig/NmGlobalFeatures"!][!//
[!IF "($BusNmNum > 1)"!][!//

/* Array of pointers to BusNm functions */
CONST(Nm_BusNmFpType, NM_CONST) Nm_BusNmFp[NM_BUSNM_NUM] =
{
[!FOR "BusId" = "1" TO "count(text:split($BusNmList))"!][!//
[!VAR "BusNmName" = "text:split($BusNmList)[position() = $BusId]"!][!//
[!VAR "BusNmNameFull" = "text:split($BusNmListFullName)[position() = $BusId]"!][!//
[!VAR "PassiveMode" = "text:split($BusNmPassiveMode)[position() = $BusId]"!][!//
  {
[!IF "as:modconf('Nm')[1]/NmGlobalConfig/NmGlobalFeatures/NmMultiCoreSupport = 'false'"!]
    &[!"$BusNmName"!]_PassiveStartUp,
[!ENDIF!]
    &[!"$BusNmName"!]_GetState,
/* !LINKSTO Nm244,1 */
[!IF "as:modconf('Nm')[1]/NmGlobalConfig/NmGlobalFeatures/NmMultiCoreSupport = 'false'"!]
[!IF "count(../../NmChannelConfig/*[NmPassiveModeEnabled ='false']) != 0"!]
[!IF "$PassiveMode = 'false'"!][!//
    &[!"$BusNmName"!]_NetworkRequest,
    &[!"$BusNmName"!]_NetworkRelease,
#if (EB_NM_STACK == STD_ON)
    &[!"$BusNmName"!]_NetworkGwEraRequest,
#endif /* (EB_NM_STACK == STD_ON) */
[!ELSE!][!//
    NULL_PTR,
    NULL_PTR,
#if (EB_NM_STACK == STD_ON)
    NULL_PTR,
#endif /* (EB_NM_STACK == STD_ON) */
[!ENDIF!][!//
[!ENDIF!]
[!ENDIF!]
[!IF "NmUserDataEnabled = 'true'"!][!//
    &[!"$BusNmName"!]_GetUserData,
[!IF "(count(../../NmChannelConfig/*[NmPassiveModeEnabled ='false']) != 0) and (NmComUserDataSupport = 'false')"!]
[!IF "$PassiveMode = 'false'"!][!//
    &[!"$BusNmName"!]_SetUserData,
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "(count(as:modconf('Nm')[1]/NmChannelConfig/*[NmNodeIdEnabled = 'true']) > 0)"!][!//
[!IF "(count(as:modconf('Nm')[1]/NmChannelConfig/*[((node:exists(NmBusType/NmStandardBusType) and (NmBusType/NmStandardBusType = $BusNmNameFull)) or (node:exists(NmBusType/NmGenericBusNmPrefix) and (NmBusType/NmGenericBusNmPrefix = $BusNmNameFull))) and (NmNodeIdEnabled = 'true')]) > 0)"!]
    &[!"$BusNmName"!]_GetNodeIdentifier,
    &[!"$BusNmName"!]_GetLocalNodeIdentifier,
[!ELSE!]
    NULL_PTR,
    NULL_PTR,
[!ENDIF!]
[!ENDIF!][!//
[!IF "count(as:modconf('Nm')[1]/NmChannelConfig/*[NmNodeDetectionEnabled = 'true']) > 0"!]
[!IF "count(as:modconf('Nm')[1]/NmChannelConfig/*[(((node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = $BusNmNameFull)) or (NmBusType/NmGenericBusNmPrefix = $BusNmNameFull)) and (NmNodeDetectionEnabled = 'true')]) > 0"!]
    &[!"$BusNmName"!]_RepeatMessageRequest,
[!ELSE!]
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!]

[!IF "(count(as:modconf('Nm')[1]/NmChannelConfig/*[NmNodeIdEnabled = 'true']) > 0) or
      (count(as:modconf('Nm')[1]/NmChannelConfig/*[NmNodeDetectionEnabled = 'true']) > 0) or
      (NmUserDataEnabled = 'true')"!]
[!IF "(count(as:modconf('Nm')[1]/NmChannelConfig/*[(((node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = $BusNmNameFull)) or (NmBusType/NmGenericBusNmPrefix = $BusNmNameFull)) and (NmNodeIdEnabled = 'true')]) > 0) or
      (count(as:modconf('Nm')[1]/NmChannelConfig/*[(((node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = $BusNmNameFull)) or (NmBusType/NmGenericBusNmPrefix = $BusNmNameFull)) and (NmNodeDetectionEnabled = 'true')]) > 0) or
      (NmUserDataEnabled = 'true')"!]
    &[!"$BusNmName"!]_GetPduData,
[!ELSE!]
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!]

[!IF "NmBusSynchronizationEnabled = 'true'"!][!//
    &[!"$BusNmName"!]_RequestBusSynchronization,
[!ENDIF!][!//
[!IF "NmRemoteSleepIndEnabled = 'true'"!][!//
    &[!"$BusNmName"!]_CheckRemoteSleepIndication,
[!ENDIF!][!//
[!IF "NmComControlEnabled = 'true'"!][!//
    &[!"$BusNmName"!]_DisableCommunication,
    &[!"$BusNmName"!]_EnableCommunication,
[!ENDIF!][!//
  },
[!ENDFOR!][!//
};

[!ENDIF!][!//
[!ENDSELECT!][!//

#define NM_STOP_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

#define NM_START_SEC_CONST_8
#include <Nm_MemMap.h>

[!IF "($BusNmNum > 1) or
      (NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true') or
      (NmGlobalConfig/NmGlobalProperties/NmDevErrorDetect = 'true') or
      (count(NmChannelConfig/*[NmStateReportEnabled = 'true']) != 0) or
      (num:i(count(NmChannelConfig/*)) > 1)"!][!//
/* Array which translates between the NetworkHandle and the Nm channel index  */
CONST(uint8, NM_CONST) Nm_ChanIdxFromNetworkHandle[NM_MAX_CHANNELID + 1U] =
{
[!FOR "Index" = "0" TO "num:max(NmChannelConfig/*/NmChannelId)"!][!//
  [!IF "count(NmChannelConfig/*[NmChannelId = $Index]) > 0"!]
  [!"NmChannelConfig/*[NmChannelId = $Index]/@index"!]U, /* Nm channel index of NetworkHandle [!"$Index"!] */
  [!ELSE!]
  NM_INVALID_CHANNEL_ID, /* NetworkHandle [!"$Index"!] is not mapped to a Nm channel */
  [!ENDIF!]
[!ENDFOR!]
};
[!ENDIF!][!//

[!IF "NmGlobalConfig/NmGlobalFeatures/NmCoordinatorSupportEnabled = 'true'"!][!//

CONST(uint8, NM_CONST) Nm_CcNmChannelsNum[NM_CC_NUM]=
{
[!FOR "ClusterIndex" = "0" TO "$HighestClusterIndex"!][!//
  [!"num:i(count(NmChannelConfig/*[(node:exists(NmCoordClusterIndex)) and (NmCoordClusterIndex = $ClusterIndex)]))"!]U,
[!ENDFOR!][!//
};

[!ENDIF!][!//

#define NM_STOP_SEC_CONST_8
#include <Nm_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
