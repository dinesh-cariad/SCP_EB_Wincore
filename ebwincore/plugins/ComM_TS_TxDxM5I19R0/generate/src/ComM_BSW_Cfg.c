/**
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
 */
[!AUTOSPACING!][!//

/* !LINKSTO ComM503_Refine,1 */

/*==================[inclusions]============================================*/
[!INCLUDE "../../generate_macros/ComM_Functions.m"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!]
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!CALL "GetMasterCoreId"!][!//
[!ENDIF!]
[!ENDIF!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!CALL "macroGetPnOffset"!][!//
[!ENDIF!]

/* prevent RTE symbols defined in ComM.h to be visible for ComM's .c files */
#if (defined COMM_INTERNAL_USAGE)
#error COMM_INTERNAL_USAGE already defined
#endif
#define COMM_INTERNAL_USAGE

#include <ComM_BSW.h>                   /* public API symbols */
#include <ComM_BSW_Cfg.h>               /* internal BSW config dependent header */

#if (COMM_CANSM_ACCESS_NEEDED == STD_ON)
#include <CanSM_ComM.h>                 /* Can state manager */
#endif

#if (COMM_FRSM_ACCESS_NEEDED == STD_ON)
#include <FrSM.h>                       /* Flexray state manager */
#endif

#if (COMM_LINSM_ACCESS_NEEDED == STD_ON)
#include <LinSM.h>                      /* Lin state manager */
#endif

#include <SchM_ComM.h>

#if(COMM_VLAN_SUPPORT)
#include<ComM_Int.h>
#endif
/*==================[macros]================================================*/

[!SELECT "ComMConfigSet/*[1]"!][!//

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]

[!LOOP "ComMPnc/*"!]
[!VAR "count" = "num:i(count(ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef))"!]
/** \brief Number of Tx EIRA Com Signals mapped to ComMPnc_[!"ComMPncId"!] */
#if (defined COMM_NUM_TX_PNC_[!"ComMPncId"!])
#error COMM_NUM_TX_PNC_[!"ComMPncId"!] already defined
#endif
#define COMM_NUM_TX_PNC_[!"ComMPncId"!]  [!"$count"!]

[!ENDLOOP!]

[!ENDIF!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

#define COMM_START_SEC_CONST_8
#include <ComM_MemMap.h>


/*---------------[Partial Network Cluster (PNC)]-----------------*/


[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!//

[!CALL "GetPnInfoLength"!][!//

[!VAR "PncMaskLength" = "(num:i(num:max(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncId) div 8)) - $macroRetValGetPnOffset + 1"!][!//


[!/* Generate a bit mask array for each unique RX EIRA signal */!]
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]

[!/* Save the Signal Handle first */!]
[!VAR "HandleId" = "ComHandleId"!]

/** \brief Array of Rx Eira Com signal masks for Com Signal [!"node:name(.)"!] */
STATIC CONST(uint8, COMM_CONST) ComM_PncRxEiraMask_[!"node:name(.)"!][COMM_PN_INFO_LENGTH] =
{
  /* [!"$PnInfoLength  "!] [!"$macroRetValGetPnOffset  "!] [!"$PncMaskLength  "!] */  
  [!VAR "loopCount" = "num:i('0')"!]
[!/* Do this for all bytes in the EIRA */!][!//
  [!FOR "I" = "$macroRetValGetPnOffset" TO "$macroRetValGetPnOffset + $PncMaskLength -1"!][!//
    [!VAR "loopCount" = "$loopCount +1"!]
    [!VAR "Mask" = "0"!][!/*
    Get all PNCs within the range of this byte
    */!][!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[ComMPncId >= num:i($I * 8) and
            ComMPncId < num:i(($I + 1) * 8)], 'ComMPncId')"!][!/*
          Check if this PNC contain the selected signal reference
          */!][!IF "node:containsValue(node:refs(ComMPncComSignal/*[ComMPncComSignalKind='EIRA']
          [ComMPncComSignalDirection='RX']/ComMPncComSignalRef)/ComHandleId, $HandleId)"!][!/*
          If so, set the respective bit
          */!][!VAR "Mask" = "bit:bitset($Mask, (ComMPncId mod 8))"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!"num:inttohex($Mask)"!]U,
  [!ENDFOR!][!//
  [!/* Initialize the array completely to avoid MISRA warnings */!][!//
  [!FOR "I" = "$loopCount + 1" TO "$PnInfoLength"!][!//
  0x00u,
  [!ENDFOR!][!//
};
[!ENDLOOP!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
[!/* Generate a bit mask array for each unique RX ERA signal */!]
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]

[!/* Save the Signal Handle first */!]
[!VAR "HandleId" = "ComHandleId"!]

/** \brief Array of Rx Era Com signal masks for Com Signal [!"node:name(.)"!] */
STATIC CONST(uint8, COMM_CONST) ComM_PncRxEraMask_[!"node:name(.)"!][COMM_PN_INFO_LENGTH] =
{
   [!VAR "loopCount" = "num:i('0')"!]
[!/* Do this for all bytes in the EIRA */!][!//
  [!FOR "I" = "$macroRetValGetPnOffset" TO "$macroRetValGetPnOffset + $PncMaskLength -1"!][!//
    [!VAR "loopCount" = "$loopCount +1"!]
    [!VAR "Mask" = "0"!][!/*
    Get all PNCs within the range of this byte
    */!][!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[ComMPncId >= num:i($I * 8) and
            ComMPncId < num:i(($I + 1) * 8)], 'ComMPncId')"!][!/*
          Check if this PNC contain the selected signal reference
          */!][!IF "node:containsValue(node:refs(ComMPncComSignal/*[ComMPncComSignalKind='ERA']
          [ComMPncComSignalDirection='RX']/ComMPncComSignalRef)/ComHandleId, $HandleId)"!][!/*
          If so, set the respective bit
          */!][!VAR "Mask" = "bit:bitset($Mask, (ComMPncId mod 8))"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!"num:inttohex($Mask)"!]U,
  [!ENDFOR!][!//
  [!/* Initialize the array completely to avoid MISRA warnings */!][!//
  [!FOR "I" = "$loopCount + 1" TO "$PnInfoLength"!][!//
  0x00u,
  [!ENDFOR!][!//
};
[!ENDLOOP!]
[!ENDIF!]

[!/* Generate a bit mask array for each unique TX EIRA signal */!]
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*
            [ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
[!/* Save the Signal Handle first */!]
[!VAR "HandleId" = "ComHandleId"!]
/** \brief Array of Tx Eira Com signal masks for Com Signal [!"node:name(.)"!] */
STATIC CONST(uint8, COMM_CONST) ComM_PncTxMask_[!"node:name(.)"!][COMM_PN_INFO_LENGTH] =
{
    [!VAR "loopCount" = "num:i('0')"!]
[!/* Do this for all bytes in the EIRA */!][!//
  [!FOR "I" = "$macroRetValGetPnOffset" TO "$macroRetValGetPnOffset + $PncMaskLength -1"!][!//
    [!VAR "loopCount" = "$loopCount +1"!]
    [!VAR "Mask" = "0"!][!/*
    Get all PNCs within the range of this byte
    */!][!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[ComMPncId >= num:i($I * 8) and
            ComMPncId < num:i(($I + 1) * 8)], 'ComMPncId')"!][!/*
          Check if this PNC contain the selected signal reference
          */!][!IF "node:containsValue(node:refs(ComMPncComSignal/*
          [ComMPncComSignalDirection='TX']/ComMPncComSignalRef)/ComHandleId, $HandleId)"!][!/*
          If so, set the respective bit
          */!][!VAR "Mask" = "bit:bitset($Mask, (ComMPncId mod 8))"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!"num:inttohex($Mask)"!]U,
  [!ENDFOR!][!//
  [!/* Initialize the array completely to avoid MISRA warnings */!][!//
  [!FOR "I" = "$loopCount + 1" TO "$PnInfoLength"!][!//
  0x00u,
  [!ENDFOR!][!//
};
[!ENDLOOP!]
[!ENDIF!][!//


[!LOOP "node:order(ComMPnc/*, 'ComMPncId')"!]
[!IF "num:i(count(ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef)) != 0"!]
/** \brief List of Tx Eira Com signals mapped to ComMPnc_[!"ComMPncId"!] */
STATIC CONST(uint8, COMM_CONST) ComM_TxSig_Pnc_[!"ComMPncId"!][COMM_NUM_TX_PNC_[!"ComMPncId"!]] =
{
[!LOOP "node:order(node:refs(ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
[!VAR "HandleId" = "ComHandleId"!]
[!VAR "SigIndex" = "0"!]
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
[!IF "ComHandleId = $HandleId"!]
  [!"num:i($SigIndex)"!], /* [!"node:name(.)"!] */
[!ENDIF!]
[!VAR "SigIndex" = "$SigIndex + 1"!]
[!ENDLOOP!]
[!ENDLOOP!]
};
[!ENDIF!][!//
[!ENDLOOP!][!//

/* !LINKSTO ECUC_ComM_00892,1 */
[!IF "node:exists(as:modconf('ComM')[1]/ComMGeneral/ComM0PncVectorAvoidance) and (as:modconf('ComM')[1]/ComMGeneral/ComM0PncVectorAvoidance = 'true')"!]
/** \brief Pnc Gateway Type Passive */
CONST(boolean, COMM_CONST)
  ComM_PncZeroVectorAvoidanceRelease[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*, 'ComMChannelId')"!]
  /* !LINKSTO ComM842_Conf,1 */
  [!VAR "LocalComMChannelId" = "ComMChannelId"!]
  [!IF "count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*[1]/ComMPncComSignal/*[(ComMPncComSignalDirection = 'TX') and
                                                                                       (ComMPncComSignalKind = 'ERA') and
                                                                                       (node:exists(ComMPncComSignalChannelRef)) and
                                                                                       (node:ref(ComMPncComSignalChannelRef)/ComMChannelId = $LocalComMChannelId) and
                                                                                       (node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and
                                                                                       (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType = 'COMM_GATEWAY_TYPE_PASSIVE')
                                                                                       ]) != 0"!]
    TRUE,
  [!ELSE!]
    FALSE,
  [!ENDIF!]
[!ENDLOOP!]
};
[!ENDIF!]

#define COMM_STOP_SEC_CONST_8
#include <ComM_MemMap.h>

/*==================[external constants]====================================*/
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!]
#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/** \brief List of SchM functions for Requesting ComMode from SMs */
CONST(ComM_BusSM_RequestComMode_FctPtr, COMM_CONST)
  ComM_BusSmRequestComModeMcFpList[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*, 'ComMChannelId')"!]
  [!IF "ComMBusType='COMM_BUS_TYPE_CAN' or
        ComMBusType='COMM_BUS_TYPE_FR' or
        ComMBusType='COMM_BUS_TYPE_ETH' or
        ComMBusType='COMM_BUS_TYPE_LIN'"!]
/* !LINKSTO ComM.EB.SchM_Call_CanSM_RequestComMode,1 */
/* !LINKSTO ComM.EB.SchM_Call_FrSM_RequestComMode,1 */
/* !LINKSTO ComM.EB.SchM_Call_EthSM_RequestComMode,1 */
/* !LINKSTO ComM.EB.SchM_Call_LinSM_RequestComMode,1 */
&SchM_Call_ComM_RequiredCSEntry_BusSM_RequestComMode_[!"node:name(.)"!],
  [!ELSE!]
NULL_PTR,
  [!ENDIF!]
[!ENDLOOP!]
};

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMDcmUsage = 'true'"!]
/** \brief List of SchM functions for Notifying Dcm */
CONST(ComM_Dcm_Notif_Type, COMM_CONST)
  ComM_Dcm_Notif_FpList[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(as:modconf('ComM')[1]/ComMConfigSet/*/ComMChannel/*, 'ComMChannelId')"!]
  {
[!IF "count(node:refs(as:modconf('Dcm')[1]/DcmConfigSet/*/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxComMChannelRef)[node:exists(ComMChannelId) and (ComMChannelId = node:current()/ComMChannelId)]) != 0"!]
    /* !LINKSTO ComM.EB.SchM_Call_Dcm_ComM_FullComModeEntered,1 */
    &SchM_Call_ComM_RequiredCSEntry_Dcm_ComM_FullComModeEntered_[!"node:name(.)"!],
    [!IF "(ComMBusType = 'COMM_BUS_TYPE_CAN' or ComMBusType = 'COMM_BUS_TYPE_ETH') and (ComMNetworkManagement/ComMNmVariant = 'FULL' or ComMNetworkManagement/ComMNmVariant = 'PASSIVE')"!]
    /* !LINKSTO ComM.EB.SchM_Call_Dcm_ComM_SilentComModeEntered,1 */
    &SchM_Call_ComM_RequiredCSEntry_Dcm_ComM_SilentComModeEntered_[!"node:name(.)"!],
    [!ELSE!]
    NULL_PTR,
    [!ENDIF!]
    /* !LINKSTO ComM.EB.SchM_Call_Dcm_ComM_NoComModeEntered,1 */
    &SchM_Call_ComM_RequiredCSEntry_Dcm_ComM_NoComModeEntered_[!"node:name(.)"!],
[!ELSE!]
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
[!ENDIF!]
  },
[!ENDLOOP!]
};
[!ENDIF!]

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>
[!ENDIF!]

#define COMM_START_SEC_CONST_8
#include <ComM_MemMap.h>

[!IF "count(ComMChannel/*) > 1"!][!//

CONST(NetworkHandleType, COMM_CONST)
  ComM_NmChannelOfChannel[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
[!IF "(ComMNetworkManagement/ComMNmVariant = 'FULL') or (ComMNetworkManagement/ComMNmVariant = 'PASSIVE')"!][!//
  [!"num:i(as:modconf('Nm')[1]/NmChannelConfig/*[as:ref(NmComMChannelRef)/ComMChannelId = node:current()/ComMChannelId]/NmChannelId)"!]U,
[!ELSE!][!//
  /* ComM channel [!"name(.)"!], ID=[!"ComMChannelId"!]
   * not associated with NM channel, NM never used */
  0xFFU,
[!ENDIF!][!//
[!ENDLOOP!][!//
};

[!ENDIF!][!//

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMWakeupInhibitionEnabled = 'true'"!]
CONST(uint8, COMM_CONST)
  ComM_EB_NoWakeupNvStorage[COMM_NUM_BYTES_NVM] =
{
[!VAR "VarNoWakeupNvStorage" = "0"!][!//
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  [!IF "ComMGlobalNvmBlockDescriptor = 'true'"!][!//
   [!VAR "VarNoWakeupNvStorage" = "bit:or($VarNoWakeupNvStorage,bit:shl(1,num:i(ComMChannelId mod 8)))"!][!//
  [!ENDIF!][!//
  [!IF "((ComMChannelId + 1) mod 8) = 0 "!][!//
    [!"num:i($VarNoWakeupNvStorage)"!]U,
    [!VAR "VarNoWakeupNvStorage" = "0"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "((node:order(ComMChannel/*, 'ComMChannelId')[last()]/ComMChannelId + 1) mod 8) != 0"!]
  [!"num:i($VarNoWakeupNvStorage)"!]U
[!ENDIF!][!//
};
[!ENDIF!][!//

[!IF "count(ComMPnc/*) > 0"!][!//

/** \brief List of Pnc Ids */
CONST(PNCHandleType, COMM_CONST) ComM_PncID[COMM_NUM_PNC] =
{
[!LOOP "node:order(ComMPnc/*, 'ComMPncId')"!][!//
  [!"ComMPncId"!], /* for PNC [!"name(.)"!] */
[!ENDLOOP!][!//
};

CONST(boolean, COMM_CONST)
  ComM_PncNmRequest[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
[!IF "ComMNetworkManagement/ComMPncNmRequest = 'true'"!][!//
 TRUE,
[!ELSE!][!//
 FALSE,
[!ENDIF!][!//
[!ENDLOOP!][!//
};

[!ENDIF!][!//

#define COMM_STOP_SEC_CONST_8
#include <ComM_MemMap.h>

#define COMM_START_SEC_CONST_16
#include <ComM_MemMap.h>

#if ((COMM_NM_VARIANT_LIGHT_NEEDED == STD_ON)      \
     || (COMM_NM_VARIANT_NONE_NEEDED == STD_ON))
/* Timeout after which state "NetReqNoNm" is left when ComMNmVariant='LIGHT' or
 * 'NONE' */
CONST(uint16, COMM_CONST) ComM_NetReqNoNmTimeoutMs[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  /* for channel [!"name(.)"!] */
[!IF "(ComMNetworkManagement/ComMNmVariant = 'LIGHT') or (ComMNetworkManagement/ComMNmVariant = 'NONE')"!][!//
  [!"num:i((../../../../ComMGeneral/ComMTMinFullComModeDuration * 1000) div (ComMMainFunctionPeriod * 1000))"!]U, /* Nm variant 'LIGHT' or 'NONE' */
[!ELSE!][!//
  0xFFU, /* channel with real bus NM support, value never used */
[!ENDIF!][!//
[!ENDLOOP!][!//
};
#endif

#if (COMM_NM_VARIANT_LIGHT_NEEDED == STD_ON)
/* Timeout after which state "ready sleep" is left in ComMNmVariant=LIGHT */
CONST(uint16, COMM_CONST) ComM_ReadySleepNoNmTimeoutMs[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  /* for channel [!"name(.)"!] */
[!IF "(ComMNetworkManagement/ComMNmVariant = 'LIGHT')"!][!//
  [!"num:i((ComMNetworkManagement/ComMNmLightTimeout * 1000) div (ComMMainFunctionPeriod * 1000))"!]U, /* Nm variant LIGHT */
[!ELSEIF "(ComMNetworkManagement/ComMNmVariant = 'NONE')"!][!//
  0U,    /* Nm variant NONE, "ready sleep" state is instantly left */
[!ELSE!][!//
  0xFFU, /* channel with real bus NM support, value never used */
[!ENDIF!][!//
[!ENDLOOP!][!//
};
#endif

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
[!CALL "GetNoOfRxBuffers"!][!//
[!ENDIF!]

CONST(ComM_RxSignal_Struct_Type, COMM_CONST) ComM_RxComSignalEiraCfg[COMM_NUM_RX_EIRA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]
  {
    [!WS "4"!][!"ComHandleId"!],  /* [!"node:name(.)"!] */
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport='true'"!][!//
    [!IF "num:i($NoOfRxBuffers) != 0"!][!//
      [!VAR "FoundCore" = "''"!][!//
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
      [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
        [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
        [!WS "4"!][!"num:i($RxSignalIndex)"!]U, /* MulticoreRxBufferIndex */
        [!WS "4"!]FALSE /* isLocalCore */
      [!ELSE!][!//
        [!WS "4"!]0U,  /* MulticoreRxBufferIndex not used */
        [!WS "4"!]TRUE /* isLocalCore */
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!]
  },
[!ENDLOOP!][!//
};

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
CONST(ComM_RxSignal_Struct_Type, COMM_CONST) ComM_RxComSignalActiveEraCfg[COMM_NUM_RX_ACTIVE_ERA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*
                             [ComMPncComSignalKind='ERA']
                             [ComMPncComSignalDirection='RX']
                             [(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType='COMM_GATEWAY_TYPE_ACTIVE')]/ComMPncComSignalRef),
                             'ComHandleId')"!]
  {
    [!WS "4"!][!"ComHandleId"!],  /* [!"node:name(.)"!] */
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport='true'"!][!//
    [!IF "num:i($NoOfRxBuffers) != 0"!][!//
      [!VAR "FoundCore" = "''"!][!//
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
      [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
        [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
        [!WS "4"!][!"num:i($RxSignalIndex)"!]U, /* MulticoreRxBufferIndex */
        [!WS "4"!]FALSE /* isLocalCore */
      [!ELSE!] [!//
        [!WS "4"!]0U, /* MulticoreRxBufferIndex not used */
        [!WS "4"!]TRUE /* isLocalCore */
      [!ENDIF!]
    [!ENDIF!][!//
  [!ENDIF!][!//
  },
[!ENDLOOP!][!//
};

CONST(ComM_RxSignal_Struct_Type, COMM_CONST) ComM_RxComSignalUnfilteredEraCfg[COMM_NUM_RX_UNFILTERED_ERA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*
                             [ComMPncComSignalKind='ERA']
                             [ComMPncComSignalDirection='RX']
                             /ComMPncComSignalRef),
                             'ComHandleId')"!]
  {
    [!WS "4"!][!"ComHandleId"!],  /* [!"node:name(.)"!] */
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport='true'"!][!//
    [!IF "num:i($NoOfRxBuffers) != 0"!][!//
      [!VAR "FoundCore" = "''"!][!//
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
      [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
        [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
        [!WS "4"!][!"num:i($RxSignalIndex)"!]U, /* MulticoreRxBufferIndex */
        [!WS "4"!]FALSE /* isLocalCore */
      [!ELSE!] [!//
        [!WS "4"!]0U, /* MulticoreRxBufferIndex not used */
        [!WS "4"!]TRUE /* isLocalCore */
      [!ENDIF!]
    [!ENDIF!][!//
  [!ENDIF!][!//
  },
[!ENDLOOP!][!//
};
[!ENDIF!]
[!ENDIF!][!//

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
[!CALL "GetNoOfTxBuffers"!][!//
[!ENDIF!]
#if (COMM_NUM_TX_SIGNALS > 0U)
CONST(ComM_TxSignal_Struct_Type, COMM_CONST) ComM_TxComSignalCfg[COMM_NUM_TX_SIGNALS] =
{
  [!VAR "TxIdx" = "0"!][!//
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
  {
  [!VAR "Handle" = "ComHandleId"!]
  [!VAR "foundsignal" = "'false'"!]
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
    [!IF "num:i($NoOfTxBuffers) != 0"!][!//
      [!VAR "FoundCore" = "''"!][!//
      [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'TX'"!][!//
      [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
        [!WS "4"!]&SchM_Send_ComM_Master_DataSet_[!"node:name(.)"!],
        [!WS "4"!][!"num:i($TxIdx)"!],  /* MulticoreTxBufferIndex */
        [!VAR "TxIdx" = "num:i($TxIdx) + 1"!][!//
      [!ELSE!][!//
        [!WS "4"!]NULL_PTR,
        [!WS "4"!]0U, /* MulticoreTxBufferIndex not used */
      [!ENDIF!][!//
      [!WS "4"!][!"ComHandleId"!], /* [!"node:name(.)"!] */
      [!WS "4"!][!"num:i($FoundCore)"!],  /* CoreId */
      [!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']"!][!/*
        */!][!IF "$foundsignal = 'false'"!][!/*
          */!][!IF "(node:ref(ComMPncComSignalRef)/ComHandleId) = $Handle"!][!/*
            */!][!IF "ComMPncComSignalKind = 'ERA'"!][!/*
              */!][!IF "((node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType) = 'COMM_GATEWAY_TYPE_ACTIVE')"!][!/*
        */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
              */!][!ELSE!][!/*
        */!][!WS "4"!]COMM_ERA_PASSIVE[!/*
              */!][!ENDIF!][!/*
            */!][!ELSE!][!/*
        */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
            */!][!ENDIF!][!/*
            */!][!VAR "foundsignal" = "'true'"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!//
    [!ELSE!][!//
      [!WS "4"!][!"ComHandleId"!], /* [!"node:name(.)"!] */
      [!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']"!][!/*
        */!][!IF "$foundsignal = 'false'"!][!/*
          */!][!IF "(node:ref(ComMPncComSignalRef)/ComHandleId) = $Handle"!][!/*
            */!][!IF "ComMPncComSignalKind = 'ERA'"!][!/*
              */!][!IF "(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and ((node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType) = 'COMM_GATEWAY_TYPE_ACTIVE')"!][!/*
      */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
              */!][!ELSE!][!/*
      */!][!WS "4"!]COMM_ERA_PASSIVE[!/*
              */!][!ENDIF!][!/*
            */!][!ELSE!][!/*
      */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
            */!][!ENDIF!][!/*
            */!][!VAR "foundsignal" = "'true'"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!WS "4"!][!"ComHandleId"!], /* [!"node:name(.)"!] */
    [!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']"!][!/*
      */!][!IF "$foundsignal = 'false'"!][!/*
        */!][!IF "(node:ref(ComMPncComSignalRef)/ComHandleId) = $Handle"!][!/*
          */!][!IF "ComMPncComSignalKind = 'ERA'"!][!/*
            */!][!IF "(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and ((node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType) = 'COMM_GATEWAY_TYPE_ACTIVE')"!][!/*
    */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
            */!][!ELSE!][!/*
    */!][!WS "4"!]COMM_ERA_PASSIVE[!/*
            */!][!ENDIF!][!/*
          */!][!ELSE!][!/*
    */!][!WS "4"!]COMM_EIRA_ERA_ACTIVE[!/*
          */!][!ENDIF!][!/*
          */!][!VAR "foundsignal" = "'true'"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!//
  [!ENDIF!][!//
  },
[!ENDLOOP!][!//
};
#endif /* (COMM_NUM_TX_SIGNALS > 0U) */
[!ENDIF!]

#define COMM_STOP_SEC_CONST_16
#include <ComM_MemMap.h>

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMModeLimitationEnabled = 'true'"!][!//
CONST(ComM_ASR40_InhibitionStatusType, COMM_CONST)
  ComM_ChannelInhibitionStatusInit[COMM_NUM_BYTES_NVM] =
{
[!VAR "VarComMNoCom" = "0"!][!//
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  [!IF "ComMNoCom = 'true'"!][!//
   [!VAR "VarComMNoCom" = "bit:or($VarComMNoCom,bit:shl(1,num:i(ComMChannelId mod 8)))"!][!//
  [!ENDIF!][!//
  [!IF "((ComMChannelId + 1) mod 8) = 0"!][!//
    [!"num:i($VarComMNoCom)"!]U,
    [!VAR "VarComMNoCom" = "0"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "((node:order(ComMChannel/*, 'ComMChannelId')[last()]/ComMChannelId + 1) mod 8) != 0"!]
  [!"num:i($VarComMNoCom)"!]U
[!ENDIF!][!//
};
[!ENDIF!][!//

[!IF "(as:modconf('ComM')[1]/ComMGeneral/ComMModeLimitationEnabled = 'true') or (as:modconf('ComM')[1]/ComMGeneral/ComMWakeupInhibitionEnabled = 'true')"!][!//
CONST(ComM_NvDataType, COMM_CONST) ComM_EB_NvROM =
{
  0U, /* InhibitionCounter */
  [!"num:i(as:modconf('ComM')[1]/ComMGeneral/ComMEcuGroupClassification)"!]U, /* ECUGroupClassification */
[!IF "(as:modconf('ComM')[1]/ComMGeneral/ComMWakeupInhibitionEnabled = 'true')"!][!//
  { /* ChannelWakeUpInhibition array */
[!VAR "VarComMNoWakeup" = "0"!][!//
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  [!IF "ComMNoWakeup = 'true'"!][!//
    [!VAR "VarComMNoWakeup" = "bit:or($VarComMNoWakeup,bit:shl(1,num:i(ComMChannelId mod 8)))"!][!//
  [!ENDIF!][!//
  [!IF "((ComMChannelId + 1) mod 8) = 0"!][!//
    [!"num:i($VarComMNoWakeup)"!]U,
    [!VAR "VarComMNoWakeup" = "0"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "((node:order(ComMChannel/*, 'ComMChannelId')[last()]/ComMChannelId + 1) mod 8) != 0"!]
  [!"num:i($VarComMNoWakeup)"!]U
[!ENDIF!][!//
  }
[!ENDIF!][!//
};
[!ENDIF!][!//


[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]

#if (COMM_NUM_TX_SIGNALS > 0U)
CONST(ComM_PncTxSigMapType, COMM_CONST) ComM_PncTxSignalMap[COMM_NUM_PNC] =
{
[!LOOP "node:order(ComMPnc/*, 'ComMPncId')"!]
  /* [!"node:name(.)"!] */
  {
    [!IF "num:i(count(ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef)) != 0"!][!//
    ComM_TxSig_Pnc_[!"ComMPncId"!],      /* TxSignalList */
    [!ELSE!][!//
    NULL_PTR,
    [!ENDIF!][!//
    COMM_NUM_TX_PNC_[!"ComMPncId"!]     /* NumTxSignal */
  },
[!ENDLOOP!]
};
#endif /* (COMM_NUM_TX_SIGNALS > 0U) */

CONSTP2CONST(uint8, COMM_CONST, COMM_CONST) ComM_PncRxEiraMask[COMM_NUM_RX_EIRA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]
  /* Com Signal: [!"node:name(.)"!] */
  ComM_PncRxEiraMask_[!"node:name(.)"!],
[!ENDLOOP!]
};

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
CONSTP2CONST(uint8, COMM_CONST, COMM_CONST) ComM_PncRxActiveEraMask[COMM_NUM_RX_ACTIVE_ERA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA']
        [ComMPncComSignalDirection='RX'][(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (as:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType='COMM_GATEWAY_TYPE_ACTIVE')]/ComMPncComSignalRef), 'ComHandleId')"!]
  /* Com Signal: [!"node:name(.)"!] */
  ComM_PncRxEraMask_[!"node:name(.)"!],
[!ENDLOOP!]
};

CONSTP2CONST(uint8, COMM_CONST, COMM_CONST) ComM_PncRxUnfilteredEraMask[COMM_NUM_RX_UNFILTERED_ERA_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]
  /* Com Signal: [!"node:name(.)"!] */
  ComM_PncRxEraMask_[!"node:name(.)"!],
[!ENDLOOP!]
};
[!ENDIF!]

#if (COMM_NUM_TX_SIGNALS > 0U)
CONSTP2CONST(uint8, COMM_CONST, COMM_CONST) ComM_PncTxMask[COMM_NUM_TX_SIGNALS] =
{
[!LOOP "node:order(node:refs(ComMPnc/*/ComMPncComSignal/*
        [ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!]
  /* Com Signal: [!"node:name(.)"!] */
  ComM_PncTxMask_[!"node:name(.)"!],
[!ENDLOOP!]
};
#endif /* (COMM_NUM_TX_SIGNALS > 0U) */

[!ENDIF!]

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

[!ENDSELECT!][!//

[!/*
            */!]

/*==================[internal data]=========================================*/

#if(COMM_VLAN_SUPPORT)

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>
CONST(ComM_VlanTableStructType, COMM_CONST) ComM_VlanConfigTable[COMM_VLAN_MANAGED_CH_NB] =
{
[!LOOP "as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*"!][!//
  [!IF "node:exists(node:current()/ComMManageReference/*[1])"!][!//
   {
    [!"num:i(node:current()/ComMChannelId)"!]U, /* ManagedChId */
    [!"num:i(node:refs(node:current()/ComMManageReference/*[1])/ComMChannelId)"!]U /* ManagingChId */
   },
  [!ENDIF!][!//
[!ENDLOOP!][!//
};
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>
#endif

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
