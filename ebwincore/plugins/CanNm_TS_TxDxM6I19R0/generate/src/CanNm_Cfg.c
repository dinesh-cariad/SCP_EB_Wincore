/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!][!//

/*==================[inclusions]============================================*/
#define CANNM_NO_CFGCLASSMIX_REQUIRED
#ifndef CANNM_NO_PBCFG_REQUIRED
#define CANNM_NO_PBCFG_REQUIRED
#endif

#include <CanNm_Api.h>        /* CanNm API definitions (own interface) */
#include <CanNm_Int.h>        /* CanNm internal dependent information */
#include <SchM_CanNm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal data]=========================================*/

[!/*  select first of the multiple config containers */!][!//
[!SELECT "CanNmGlobalConfig/*[1]"!][!//

/*==================[internal constants]====================================*/

/*==================[external constants]====================================*/
#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

[!// Channel Configuration
/* !LINKSTO CANNM299,1 */
CONST(CanNm_ChanConfigType, CANNM_CONST)
  CanNm_ChanConfig[CANNM_NUMBER_OF_CHANNELS] =
{
[!LOOP "node:order(CanNmChannelConfig/*,'CanNmRxPdu/*[1]/CanNmRxPduId')"!][!//
  /* Configuration of CAN NM channel [!"name(.)"!] with RxPduId [!"CanNmRxPdu/*[1]/CanNmRxPduId"!] : */
  {
[!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmMultiCoreSupport='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_NetworkStartIndication_[!"node:name(.)"!],
    &SchM_Call_CanNm_RequiredCSEntry_Nm_NetworkMode_[!"node:name(.)"!],
    &SchM_Call_CanNm_RequiredCSEntry_Nm_BusSleepMode_[!"node:name(.)"!],
    &SchM_Call_CanNm_RequiredCSEntry_Nm_PrepareBusSleepMode_[!"node:name(.)"!],
[!IF "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmRemoteSleepIndEnabled='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_RemoteSleepIndication_[!"node:name(.)"!],
    &SchM_Call_CanNm_RequiredCSEntry_Nm_RemoteSleepCancellation_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPduRxIndicationEnabled='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_PduRxIndication_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmStateChangeIndEnabled='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_StateChangeNotification_[!"node:name(.)"!],
[!ENDIF!]
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmRepeatMsgIndEnabled='true']) > 0"!]
[!IF "CanNmRepeatMsgIndEnabled='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_RepeatMessageIndication_[!"node:name(.)"!],
[!ELSE!]
    NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPassiveModeEnabled='false') and
      (as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmImmediateTxconfEnabled='false')"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_TxTimeoutException_[!"node:name(.)"!],
[!ENDIF!]
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmCarWakeUpRxEnabled='true']) > 0"!]
[!IF "CanNmCarWakeUpRxEnabled='true'"!]
    &SchM_Call_CanNm_RequiredCSEntry_Nm_CarWakeUpIndication_[!"node:name(.)"!],
[!ELSE!]
    NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!ENDIF!]

    /* !LINKSTO CANNM246,1 */
    [!"num:i((CanNmTimeoutTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Nm Timeout Time */
    /* !LINKSTO CANNM247,1 */
    [!"num:i((CanNmRepeatMessageTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Repeat Message Time */
    /* !LINKSTO CANNM248,1 */
    [!"num:i((CanNmWaitBusSleepTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Wait Bus Sleep Time */
    /* !LINKSTO CANNM249,1 */
    [!VAR "CanNmRemoteSleepIndTimeVar" = "0"!]
    [!IF "../../CanNmRemoteSleepIndEnabled = 'true'"!]
      [!VAR "CanNmRemoteSleepIndTimeVar" = "CanNmRemoteSleepIndTime"!]
    [!ENDIF!]
    [!"num:i(($CanNmRemoteSleepIndTimeVar * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Remote Sleep Indication Time */
[!IF "../../CanNmPassiveModeEnabled = 'false'"!][!//
    /* !LINKSTO CANNM056_Conf,1 */
[!IF "num:i(count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmImmediateNmTransmissions > 0])) > 0"!][!//
    [!"CanNmImmediateNmTransmissions"!]U, /* Immediate Nm Transmissions count */
[!IF "node:exists(CanNmImmediateNmCycleTime)"!]
    [!"num:i((CanNmImmediateNmCycleTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Immediate Nm Cycle Time */
[!ELSE!]
    0U,
[!ENDIF!]
[!ENDIF!]
    [!"num:i((CanNmMsgCycleTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Msg Cycle Time */
[!IF "../../CanNmImmediateTxconfEnabled = 'false'"!][!//
    [!"num:i((CanNmMsgTimeoutTime * 1000) div (../../CanNmMainFunctionPeriod * 1000))"!]U, /* Msg Timeout Time */
[!ENDIF!][!//
[!IF "../../CanNmBusLoadReductionEnabled = 'true'"!][!//
[!IF "CanNmBusLoadReductionActive = 'true'"!][!//
    /* !LINKSTO CANNM042_Conf,1 */
    [!"num:i((CanNmMsgReducedTime * 1000) div (../../CanNmMainFunctionPeriod * 1000)) "!]U, /* Msg Reduced Time */
[!ELSE!][!//
    0U, /* Msg Reduced Time */
[!ENDIF!][!//
[!ENDIF!][!// Bus Load Reduction enabled
[!ENDIF!][!// Passive mode not enabled
[!IF "../../CanNmPassiveModeEnabled = 'false'"!][!//
    [!"CanNmTxPdu/CanNmTxConfirmationPduId"!]U, /* Tx Confirmation PduId */
[!ENDIF!][!// Passive mode not enabled
#if ((CANNM_REPEAT_MSG_IND_ENABLED == STD_ON) &&  \
     (CANNM_NODE_DETECTION_ENABLED == STD_ON))
    [!"CanNmRxPdu/*[1]/CanNmRxPduId"!]U, /* Rx PduId */
#endif
[!IF "../../CanNmComUserDataSupport = 'true'"!]
[!IF "node:exists(CanNmUserDataTxPdu)"!]
    [!"CanNmUserDataTxPdu/CanNmTxUserDataPduId"!]U, /* User Data Tx PduId */[!/*
    Get User Data TxConf PduId by EcuC reference from PduR;
    existence is already checked in CanNm_Checks.m.
    */!]
[!ELSE!][!//
    0xFF, /* User Data Tx PduId */
[!ENDIF!][!//
[!ENDIF!][!// UserDataSupport enabled
#if (CANNM_PN_ERA_CALC_ENABLED == STD_ON)
[!IF "(node:exists(CanNmPnEraCalcEnabled)) and (CanNmPnEraCalcEnabled='true')"!]
    /* !LINKSTO CANNM079_Conf,1 */
    [!/* Get Rx PduId by EcuC reference from PduR, existence is already checked in CanNm_Checks.m */!][!//
    [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/
       PduRSrcPdu[PduRSrcPduRef=node:current()/CanNmPnEraRxNSduRef]/
       PduRSourcePduHandleId"!]U, /* EraPduId */
    1U, /* EraCalcEnabled */
[!ELSE!]
    0U, /* EraPduId */
    0U, /* EraCalcEnabled */
[!ENDIF!][!//
#endif
[!IF "CanNmBusLoadReductionActive = 'true'"!][!//
    CANNM_BITPOS2MASK(CANNM_BUSLOADREDACTIVE), /* Channel Config Flags */
[!ELSE!][!//
    0U, /* Channel Config Flags */
[!ENDIF!][!//
    [!"CanNmPduNidPosition"!], /* Nid Position */
    [!"CanNmPduCbvPosition"!], /* Cbv Position */
[!IF "(../../CanNmUserDataEnabled = 'false') and (../../CanNmComUserDataSupport = 'false')"!][!//
    CANNM_PDU_OFF, /* UserData first byte position */
[!ELSEIF "(CanNmPduNidPosition = 'CANNM_PDU_BYTE_1') or (CanNmPduCbvPosition = 'CANNM_PDU_BYTE_1')"!][!//
    2U,            /* UserData first byte position */
[!ELSEIF "(CanNmPduNidPosition = 'CANNM_PDU_BYTE_0') or (CanNmPduCbvPosition = 'CANNM_PDU_BYTE_0')"!][!//
    1U,            /* UserData first byte position */
[!ELSE!][!//
    0U,            /* UserData first byte position */
[!ENDIF!][!//
    [!"num:i(as:ref(CanNmComMNetworkHandleRef)/ComMChannelId)"!]U, /* ComMChannelId */
    /* !LINKSTO CANNM068_Conf,1 */
[!IF "node:exists(CanNmAllNmMessagesKeepAwake)"!][!//
  [!IF "CanNmAllNmMessagesKeepAwake = 'true'"!][!//
      TRUE, /* AllNmMessagesKeepAwake */
  [!ELSE!][!//
      FALSE, /* AllNmMessagesKeepAwake */
  [!ENDIF!][!//
[!ELSE!][!//
  FALSE, /* CanNmAllNmMessagesKeepAwake */
[!ENDIF!][!//
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[(node:exists(CanNmCarWakeUpRxEnabled)) and (CanNmCarWakeUpRxEnabled='true')]) > 0"!]
  [!IF "node:exists(CanNmCarWakeUpBitPosition)"!][!//
    [!"num:i(CanNmCarWakeUpBitPosition)"!]U, /* CanNmCarWakeUpBitPosition */
  [!ELSE!][!//
    0U, /* CanNmCarWakeUpBitPosition */
  [!ENDIF!][!//
  [!IF "node:exists(CanNmCarWakeUpBytePosition)"!][!//
    [!"num:i(CanNmCarWakeUpBytePosition)"!]U, /* CanNmCarWakeUpBytePosition */
  [!ELSE!][!//
    0U, /* CanNmCarWakeUpBytePosition */
  [!ENDIF!][!//
  [!IF "node:exists(CanNmCarWakeUpFilterNodeId)"!][!//
    [!"num:i(CanNmCarWakeUpFilterNodeId)"!]U, /* CanNmCarWakeUpFilterNodeId */
  [!ELSE!][!//
    0U, /* CanNmCarWakeUpFilterNodeId */
  [!ENDIF!][!//
  [!IF "node:exists(CanNmCarWakeUpFilterEnabled)"!][!//
    [!IF "CanNmCarWakeUpFilterEnabled = 'true'"!][!//
    TRUE, /* CanNmCarWakeUpFilterEnabled */
    [!ELSE!][!//
    FALSE, /* CanNmCarWakeUpFilterEnabled */
    [!ENDIF!][!//
  [!ELSE!][!//
    FALSE, /* CanNmCarWakeUpFilterEnabled */
  [!ENDIF!][!//
  [!IF "CanNmCarWakeUpRxEnabled = 'true'"!][!//
    TRUE, /* CanNmCarWakeUpRxEnabled */
  [!ELSE!]
    FALSE, /* CanNmCarWakeUpRxEnabled */
  [!ENDIF!]
[!ENDIF!]
  [!IF "CanNmRepeatMsgIndEnabled = 'true'"!]
    TRUE,  /* CanNmRepeatMsgIndEnabled */
  [!ELSE!][!//
    FALSE, /* CanNmRepeatMsgIndEnabled */
  [!ENDIF!][!// CanNmRepeatMsgIndEnabled enabled
  [!IF "CanNmNodeDetectionEnabled = 'true'"!]
    TRUE,  /* CanNmNodeDetectionEnabled */
  [!ELSE!][!//
    FALSE, /* CanNmNodeDetectionEnabled */
  [!ENDIF!][!// CanNmNodeDetectionEnabled enabled
  [!IF "CanNmNodeIdEnabled = 'true'"!]
    TRUE,  /* CanNmNodeIdEnabled */
  [!ELSE!][!//
    FALSE, /* CanNmNodeIdEnabled */
  [!ENDIF!][!// CanNmNodeIdEnabled enabled
},

[!ENDLOOP!][!//
};

#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

#define CANNM_START_SEC_CONST_8
#include <CanNm_MemMap.h>

CONST(NetworkHandleType, CANNM_CONST)
  CanNm_IndexFromNmChannelHandle[CANNM_INDEXFROMNMCHANNELHANDLE_NUM] =
{
[!FOR "ComMChannelId"="0" TO "num:i(num:max(node:refs(CanNmChannelConfig/*/CanNmComMNetworkHandleRef)/ComMChannelId))"!][!//
[!IF "count(CanNmChannelConfig/*[node:ref(CanNmComMNetworkHandleRef)/ComMChannelId = $ComMChannelId]) > 0"!][!//
[!SELECT "CanNmChannelConfig/*[node:ref(CanNmComMNetworkHandleRef)/ComMChannelId = $ComMChannelId]"!][!//
  [!"CanNmRxPdu/*[1]/CanNmRxPduId"!]U, [!//
/* RxPduId of channel "[!"name(.)"!]" with ComMChannelHandle [!"$ComMChannelId"!] */
[!ENDSELECT!][!//
[!ELSE!][!//
  0xFFU, /* no CanNm channel for ComMChannelHandle [!"$ComMChannelId"!] */
[!ENDIF!][!//
[!ENDFOR!]
};

#define CANNM_STOP_SEC_CONST_8
#include <CanNm_MemMap.h>

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

[!ENDSELECT!][!//
/*==================[end of file]===========================================*/
