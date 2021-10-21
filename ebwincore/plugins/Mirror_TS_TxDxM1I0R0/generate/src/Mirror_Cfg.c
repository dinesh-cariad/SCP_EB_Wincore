/**
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
 */

[!CODE!]
/*==================[inclusions]============================================*/

#include <Mirror_Cfg.h> /* Generated pre-compile configuration. */
[!INCLUDE "../include/Mirror_PrecompileMacros.m"!][!//

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define MIRROR_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Mirror_MemMap.h>
[!CALL "CheckConfiguration"!][!//
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00075,1 */
/* !LINKSTO Mirror.dsn.PrecompileTimeConfig,1 */
CONST(Mirror_DestNetworkType, MIRROR_APPL_CONST) Mirror_DestNetworks[MIRROR_NO_OF_DESTNETWORKS] =
{
[!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorDestNetwork/*"!][!//
    {
[!CALL "TxIPduToSrcPdu", "value" = "node:path(node:ref(./MirrorDestPdu/*[1]/MirrorDestPduRef))"!][!//
[!CALL "ValidateNetworkId", "NetworkType" = "node:name(.)" , "NetworkID" = "num:i(./MirrorNetworkId)" , "NetworkName" = "as:name(.)"!][!//
        [!"$SrcPduId"!]U,    /*    DestNetworkSrcPduRId    */
        [!"num:i(./MirrorDestPdu/*[1]/MirrorDestPduId)"!]U,    /* DestNetworkPduId */
        [!"$ActualTxNPduLength"!]U,    /*    DestNetworkPduLength    */
        [!"./MirrorDestQueueSize"!]U,    /*    DestQueueSizeRange    */
        [!IF "node:exists(./MirrorDestTransmissionDeadline)"!]TRUE[!ELSE!]FALSE[!ENDIF!],    /*    DestTxDeadlineEnable    */
        [!IF "node:exists(./MirrorDestTransmissionDeadline)"!][!IF "num:i((./MirrorDestTransmissionDeadline) div $MainFuncPeriod) = 0"!]1U[!ELSE!][!"num:i((./MirrorDestTransmissionDeadline) div $MainFuncPeriod)"!][!ENDIF!][!ELSE!]MIRROR_TRANSMISSIONDEADLINE_DISABLED[!ENDIF!],    /*    DestTxDeadline    */
        [!IF "num:i((./MirrorTxConfirmationTimeout) div $MainFuncPeriod) = 0"!]1U[!ELSE!][!"num:i((./MirrorTxConfirmationTimeout) div $MainFuncPeriod)"!][!ENDIF!],    /*    DestTxConfirmationTimeout    */
        [!"node:value(as:ref(./MirrorComMNetworkHandleRef)/ComMChannelId)"!],    /*    ComMChannelId    */
        [!IF "node:name(.) = 'MirrorDestNetworkIp'"!]MIRROR_NT_ETHERNET,[!ENDIF!]    /*    NetworkType    */
        [!"./MirrorNetworkId"!]U,    /*    MirrorNetworkId    */
        [!IF "./MirrorDestPdu/*[1]/MirrorDestPduUsesTriggerTransmit = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!]    /*    TriggerTxEnable    */
    },
    /* [!"as:name(.)"!] */
[!ENDLOOP!][!//
};

/* !LINKSTO Mirror.ASR44.SWS_Mirror_00075,1 */
CONST(Mirror_SourceNetworkType, MIRROR_APPL_CONST) Mirror_SourceNetworks[MIRROR_NO_OF_SOURCENETWORKS] =
{
[!VAR "ControllerIdx" = "0"!][!//
[!VAR "FlexRayNetworkIdx" = "0"!][!//
[!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!][!//
[!CALL "ValidateNetworkId", "NetworkType" = "node:name(.)" , "NetworkID" = "num:i(./MirrorNetworkId)" , "NetworkName" = "as:name(.)"!][!//
    {
[!CALL "SrcNetworkControllers", "ComMChannel" = "node:path(node:ref(./MirrorComMNetworkHandleRef))" , "NetworkType" = "node:name(.)"!][!//
        [!"node:value(as:ref(./MirrorComMNetworkHandleRef)/ComMChannelId)"!]U,    /*    ComMChannelId    */
        [!IF "node:name(.) = 'MirrorSourceNetworkCan'"!]MIRROR_NT_CAN,[!ELSEIF "node:name(.) = 'MirrorSourceNetworkLin'"!]MIRROR_NT_LIN,[!ELSE!]MIRROR_NT_FLEXRAY,[!ENDIF!]    /*    NetworkType    */
[!IF "node:exists(as:modconf('Mirror')[1]/MirrorGeneral/MirrorFrFramesPerSecond)"!][!//
[!IF "node:name(.) = 'MirrorSourceNetworkFlexRay'"!][!//
        [!"num:i($ClusterIdx)"!]U,    /* ClusterIdx */
        [!"num:i($FlexRayNetworkIdx)"!]U,    /* FlexRayNetworkIdx */
        [!VAR "FlexRayNetworkIdx" = "$FlexRayNetworkIdx + 1"!][!//
[!ELSE!][!//
        0U,    /* SourceNetwork is not FlexRay, No ClusterIdx */
        0U,    /* SourceNetwork is not FlexRay, No FlexRayNetworkIdx */
[!ENDIF!][!//
[!ENDIF!][!//
        [!"num:i($ControllerIdx)"!]U,    /*    SourceNetworkControllerIndex    */
        [!"num:i($NumOfControllers)"!]U,    /*    NumOfSourceNetworkControllers    */
        [!"./MirrorNetworkId"!]U,    /*    MirrorNetworkId    */
        [!IF "$TrcvCfg = 0"!]MIRROR_NO_NETWORKTRCV[!ELSE!][!"num:i($TrcvID)"!]U[!ENDIF!],    /*    TrcvId    */
        [!IF "$TrcvCfg = 0"!]FALSE[!ELSE!]TRUE[!ENDIF!]    /*    TrcvConfigured    */
    },
[!VAR "ControllerIdx" = "$ControllerIdx + $NumOfControllers"!][!//
    /* [!"as:name(.)"!] */
[!ENDLOOP!][!//
};

#define MIRROR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Mirror_MemMap.h>

#define MIRROR_START_SEC_CONFIG_DATA_8
#include <Mirror_MemMap.h>

CONST(uint8, MIRROR_APPL_CONST) Mirror_SourceNetworkControllers[MIRROR_NO_OF_SOURCECONTROLLERS] =
{
[!LOOP "as:modconf('Mirror')[1]/MirrorConfigSet/MirrorSourceNetwork/*"!][!//
[!VAR "NetworkName" = "as:name(.)"!][!//
[!CALL "SrcNetworkControllers", "ComMChannel" = "node:path(node:ref(./MirrorComMNetworkHandleRef))" , "NetworkType" = "node:name(.)"!][!//
[!IF "$NetworkType = 'MirrorSourceNetworkCan'"!][!//
[!LOOP "as:modconf('CanSM')[1]/CanSMConfiguration/*[1]/CanSMManagerNetwork/*[num:i($CanSmNetwork)]/CanSMController/*"!][!//
    [!"node:value(as:ref(./CanSMControllerId)/CanIfCtrlId)"!]U, /*    [!"$NetworkName"!]    */
[!ENDLOOP!][!//
[!ELSEIF "$NetworkType = 'MirrorSourceNetworkLin'"!][!//
    [!"$LinChannelId"!]U, /*    [!"$NetworkName"!]    */
[!ELSE!][!//
[!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*[num:i($ClusterIdx) + 1]/FrIfController/*"!][!//
    [!"node:value(./FrIfCtrlIdx)"!]U, /*    [!"$NetworkName"!]     */
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
};

#define MIRROR_STOP_SEC_CONFIG_DATA_8
#include <Mirror_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
[!IF "node:exists(as:modconf('Mirror')[1]/MirrorGeneral/MirrorLinFramesPerSecond)"!][!//
#define MIRROR_START_SEC_LIN_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>

VAR(Mirror_LinElementType, MIRROR_VAR) Mirror_LinElement[MIRROR_LIN_ELEMENTCOUNTER];

VAR(MIRROR_TIMESTAMP_TYPE, MIRROR_VAR) Mirror_LinTimeStamp[MIRROR_LIN_ELEMENTCOUNTER];

#define MIRROR_STOP_SEC_LIN_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>
[!ENDIF!][!//

[!IF "node:exists(as:modconf('Mirror')[1]/MirrorGeneral/MirrorCanFramesPerSecond)"!][!//
#define MIRROR_START_SEC_CAN_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>

VAR(Mirror_CanElementType, MIRROR_VAR) Mirror_CanElement[MIRROR_CAN_ELEMENTCOUNTER];

VAR(MIRROR_TIMESTAMP_TYPE, MIRROR_VAR) Mirror_CanTimeStamp[MIRROR_CAN_ELEMENTCOUNTER];

[!IF "node:exists(as:modconf('Mirror')[1]/MirrorGeneral/MirrorCanFDFramesPerSecond)"!][!//
VAR(Mirror_CanFDElementType, MIRROR_VAR) Mirror_CanFDElement[MIRROR_CANFD_ELEMENTCOUNTER];

VAR(MIRROR_TIMESTAMP_TYPE, MIRROR_VAR) Mirror_CanFDTimeStamp[MIRROR_CANFD_ELEMENTCOUNTER];
[!ENDIF!][!//

#define MIRROR_STOP_SEC_CAN_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>
[!ENDIF!][!//

[!IF "node:exists(as:modconf('Mirror')[1]/MirrorGeneral/MirrorFrFramesPerSecond)"!][!//
#define MIRROR_START_SEC_FR_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>

VAR(Mirror_FlexRayElementType, MIRROR_VAR) Mirror_FlexRayElement[MIRROR_FLEXRAY_ELEMENTCOUNTER];

VAR(MIRROR_TIMESTAMP_TYPE, MIRROR_VAR) Mirror_FlexRayTimeStamp[MIRROR_FLEXRAY_ELEMENTCOUNTER];

VAR(Mirror_FlexRaySrcAggStatusType, MIRROR_VAR) Mirror_FlexRaySrcAggStatus[MIRROR_NO_OF_FLEXRAYSOURCENETWORKS];

#define MIRROR_STOP_SEC_FR_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include <Mirror_MemMap.h>
[!ENDIF!][!//

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]