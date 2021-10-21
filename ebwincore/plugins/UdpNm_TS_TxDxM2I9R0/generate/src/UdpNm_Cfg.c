/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!][!//

/*==================[inclusions]============================================*/
#include <UdpNm_Adaptive.h>

#if (UDPNM_ADAPTIVE == STD_OFF)
#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#ifndef UDPNM_NO_PBCFG_REQUIRED
#define UDPNM_NO_PBCFG_REQUIRED
#endif

#include <UdpNm_Api.h>        /* UdpNm API definitions (own interface) */
#include <UdpNm_Int.h>        /* UdpNm internal dependent information */
#include <SchM_UdpNm.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal data]=========================================*/

[!/*  select first of the multiple config containers */!][!//
[!SELECT "UdpNmGlobalConfig/*[1]"!][!//

/*==================[internal constants]====================================*/

/*==================[external constants]====================================*/
#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

[!// Channel Configuration
CONST(UdpNm_ChanConfigType, UDPNM_CONST) UdpNm_ChanConfig[UDPNM_NUMBER_OF_CHANNELS] =
{
[!LOOP "node:order(UdpNmChannelConfig/*,'UdpNmRxPdu/*[1]/UdpNmRxPduId')"!][!//
/* Configuration of UdpNm channel [!"name(.)"!] with RxPduId [!"UdpNmRxPdu/*[1]/UdpNmRxPduId"!] : */
  {
[!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMultiCoreSupport='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_NetworkStartIndication_[!"node:name(.)"!],
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_NetworkMode_[!"node:name(.)"!],
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_BusSleepMode_[!"node:name(.)"!],
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_PrepareBusSleepMode_[!"node:name(.)"!],
[!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmRemoteSleepIndEnabled='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_RemoteSleepIndication_[!"node:name(.)"!],
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_RemoteSleepCancellation_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPduRxIndicationEnabled='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_PduRxIndication_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmStateChangeIndEnabled='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_StateChangeNotification_[!"node:name(.)"!],
[!ENDIF!]
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmRepeatMsgIndEnabled='true']) > 0"!]
[!IF "UdpNmRepeatMsgIndEnabled='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_RepeatMessageIndication_[!"node:name(.)"!],
[!ELSE!]
    NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmCarWakeUpRxEnabled='true']) > 0"!]
[!IF "UdpNmCarWakeUpRxEnabled='true'"!]
    &SchM_Call_UdpNm_RequiredCSEntry_Nm_CarWakeUpIndication_[!"node:name(.)"!],
[!ELSE!]
    NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!ENDIF!]
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmCarWakeUpRxEnabled='true']) > 0"!]
  [!IF "node:exists(UdpNmCarWakeUpBytePosition)"!][!//
    [!"num:i(UdpNmCarWakeUpBytePosition)"!]U, /* UdpNmCarWakeUpBytePosition */
  [!ELSE!][!//
    0U, /* UdpNmCarWakeUpBytePosition */
  [!ENDIF!][!//
[!ENDIF!]
    /* UdpNmTimeoutTime: Nm-Timeout Time */
    /* !LINKSTO ECUC_UdpNm_00020,1 */
    [!"num:i((UdpNmTimeoutTime * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U,
    /* RmsTimer: Repeat Message Time */
    /* !LINKSTO ECUC_UdpNm_00022,2 */
    [!"num:i((UdpNmRepeatMessageTime * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U,
    /* WBSTime: Wait Bus Sleep Time */
    /* !LINKSTO ECUC_UdpNm_00021,1 */
    [!"num:i((UdpNmWaitBusSleepTime * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U,
    /* RSITime: Remote Sleep Indication Time */
    /* !LINKSTO ECUC_UdpNm_00005,1 */
    [!VAR "UdpNmRemoteSleepIndTimeVar" = "0"!]
    [!IF "../../UdpNmRemoteSleepIndEnabled = 'true'"!]
      [!VAR "UdpNmRemoteSleepIndTimeVar" = "UdpNmRemoteSleepIndTime"!]
    [!ENDIF!]
    [!"num:i(($UdpNmRemoteSleepIndTimeVar * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U,
[!IF "../../UdpNmPassiveModeEnabled = 'false'"!][!//
    [!"UdpNmImmediateNmTransmissions"!]U, /* Immediate Nm Transmissions count */
[!IF "node:exists(UdpNmImmediateNmCycleTime)"!][!//
    [!"num:i((UdpNmImmediateNmCycleTime * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U, /* Immediate Nm Cycle Time */
[!ELSE!][!//
    0U,
[!ENDIF!][!//
    /* MsgCycleTimer: Msg Cycle Time */
    /* !LINKSTO ECUC_UdpNm_00028,1 */
    [!"num:i((UdpNmMsgCycleTime * 1000) div (../../UdpNmMainFunctionPeriod * 1000))"!]U,
[!ENDIF!]
[!IF "../../UdpNmPassiveModeEnabled = 'false'"!][!//
    /* TxConfPduId: Tx Confirmation PduId */
    /* !LINKSTO ECUC_UdpNm_00042,1 */
    [!"UdpNmTxPdu/UdpNmTxConfirmationPduId"!]U,
[!ENDIF!]
    /* RxPduId: Rx PduId */
    /* !LINKSTO ECUC_UdpNm_00043,1 */
    [!"UdpNmRxPdu/*[1]/UdpNmRxPduId"!]U,
[!IF "../../UdpNmComUserDataSupport = 'true'"!]
    /* UserTxPduId: User Data Tx PduId */
    /* !LINKSTO ECUC_UdpNm_00058,1 */
[!IF "node:exists(UdpNmUserDataTxPdu)"!]
    [!"UdpNmUserDataTxPdu/UdpNmTxUserDataPduId"!]U,
[!ELSE!][!//
    0U,
[!ENDIF!][!//
[!ENDIF!][!// UserDataSupport enabled
#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
[!IF "(node:exists(UdpNmPnEraCalcEnabled)) and (UdpNmPnEraCalcEnabled='true')"!][!//
    /* EraPduId: EraPduId */
    /* !LINKSTO ECUC_UdpNm_00073,1 */
    [!/* Get Rx PduId by EcuC reference from PduR, existence is already checked in UdpNm_Checks.m */!][!//
    [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/
       PduRSrcPdu[PduRSrcPduRef=node:current()/UdpNmPnEraRxNSduRef]/PduRSourcePduHandleId"!]U,
    /* EraCalcEnabled: EraCalcEnabled */
    /* !LINKSTO ECUC_UdpNm_00060,1 */
    STD_ON,
[!ELSE!]
    0U,
    STD_OFF,
[!ENDIF!][!//
#endif
    /* NidPos: Nid Position */
    /* !LINKSTO ECUC_UdpNm_00025,1 */
    [!"UdpNmPduNidPosition"!],
    /* CbvPos: Cbv Position */
    /* !LINKSTO ECUC_UdpNm_00026,1 */
    [!"UdpNmPduCbvPosition"!],
    /* UDFBPos: UserData first byte position */
[!IF "(../../UdpNmUserDataEnabled = 'false') and (../../UdpNmComUserDataSupport = 'false')"!][!//
    UDPNM_PDU_OFF,
/* !LINKSTO ECUC_UdpNm_00025,1 */
[!ELSEIF "(UdpNmPduNidPosition = 'UDPNM_PDU_BYTE_1') or (UdpNmPduCbvPosition = 'UDPNM_PDU_BYTE_1')"!][!//
    2U,
[!ELSEIF "(UdpNmPduNidPosition = 'UDPNM_PDU_BYTE_0') or (UdpNmPduCbvPosition = 'UDPNM_PDU_BYTE_0')"!][!//
    1U,
[!ELSE!][!//
    0U,
[!ENDIF!][!//
    /* nmChannelId: ComMChannelId */
    /* !LINKSTO ECUC_UdpNm_00018,1 */
    [!"num:i(as:ref(UdpNmComMNetworkHandleRef)/ComMChannelId)"!]U,
    /* UdpNmAllNmMessagesKeepAwake */
[!IF "UdpNmAllNmMessagesKeepAwake = 'true'"!]
    TRUE,
[!ELSE!][!//
    FALSE,
[!ENDIF!]
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmCarWakeUpRxEnabled='true']) > 0"!]
  [!IF "node:exists(UdpNmCarWakeUpBitPosition)"!][!//
    [!"num:i(UdpNmCarWakeUpBitPosition)"!]U, /* UdpNmCarWakeUpBitPosition */
  [!ELSE!][!//
    0U, /* UdpNmCarWakeUpBitPosition */
  [!ENDIF!][!//
  [!IF "node:exists(UdpNmCarWakeUpFilterNodeId)"!][!//
    [!"num:i(UdpNmCarWakeUpFilterNodeId)"!]U, /* UdpNmCarWakeUpFilterNodeId */
  [!ELSE!][!//
    0U, /* UdpNmCarWakeUpFilterNodeId */
  [!ENDIF!][!//
  [!IF "node:exists(UdpNmCarWakeUpFilterEnabled)"!][!//
    [!IF "UdpNmCarWakeUpFilterEnabled = 'true'"!][!//
    TRUE, /* UdpNmCarWakeUpFilterEnabled */
    [!ELSE!][!//
    FALSE, /* UdpNmCarWakeUpFilterEnabled */
    [!ENDIF!][!//
  [!ELSE!][!//
    FALSE, /* UdpNmCarWakeUpFilterEnabled */
  [!ENDIF!][!//
  [!IF "UdpNmCarWakeUpRxEnabled = 'true'"!][!//
    TRUE, /* UdpNmCarWakeUpRxEnabled */
  [!ELSE!]
    FALSE, /* UdpNmCarWakeUpRxEnabled */
  [!ENDIF!]
[!ENDIF!]
  [!IF "UdpNmRepeatMsgIndEnabled = 'true'"!]
    TRUE, /* UdpNmRepeatMsgIndEnabled */
  [!ELSE!][!//
    FALSE, /* UdpNmRepeatMsgIndEnabled */
  [!ENDIF!][!// UdpNmRepeatMsgIndEnabled enabled
  [!IF "UdpNmNodeDetectionEnabled = 'true'"!]
    TRUE,  /* UdpNmNodeDetectionEnabled */
  [!ELSE!][!//
    FALSE, /* UdpNmNodeDetectionEnabled */
  [!ENDIF!][!// UdpNmNodeDetectionEnabled enabled
  [!IF "UdpNmNodeIdEnabled = 'true'"!]
    TRUE,  /* UdpNmNodeIdEnabled */
  [!ELSE!][!//
    FALSE, /* UdpNmNodeIdEnabled */
  [!ENDIF!][!// UdpNmNodeIdEnabled enabled
  [!IF "(node:exists(UdpNmRetryFirstMessageRequest)) and (UdpNmRetryFirstMessageRequest = 'true')"!]
    TRUE,  /* RetryFirstMessageRequest */
  [!ELSE!][!//
    FALSE, /* RetryFirstMessageRequest */
  [!ENDIF!][!// RetryFirstMessageRequest enabled
  },
[!ENDLOOP!][!//
};

#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

#define UDPNM_START_SEC_CONST_8
#include <UdpNm_MemMap.h>

CONST(NetworkHandleType, UDPNM_CONST)
  UdpNm_IndexFromNmChannelHandle[UDPNM_INDEXFROMNMCHANNELHANDLE_NUM] =
{
[!FOR "ComMChannelId"="0" TO "num:i(num:max(node:refs(UdpNmChannelConfig/*/UdpNmComMNetworkHandleRef)/ComMChannelId))"!][!//
[!IF "count(UdpNmChannelConfig/*[node:ref(UdpNmComMNetworkHandleRef)/ComMChannelId = $ComMChannelId]) > 0"!][!//
[!SELECT "UdpNmChannelConfig/*[node:ref(UdpNmComMNetworkHandleRef)/ComMChannelId = $ComMChannelId]"!][!//
  [!"UdpNmRxPdu/*[1]/UdpNmRxPduId"!]U, [!//
/* RxPduId of channel "[!"name(.)"!]" with ComMChannelHandle [!"$ComMChannelId"!] */
[!ENDSELECT!][!//
[!ELSE!][!//
  0xFFU, /* no UdpNm channel for ComMChannelHandle [!"$ComMChannelId"!] */
[!ENDIF!][!//
[!ENDFOR!]
};

#define UDPNM_STOP_SEC_CONST_8
#include <UdpNm_MemMap.h>

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/
#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

  /* !LINKSTO SWS_UdpNm_00234_Refine,1 */
[!FOR "I"="0" TO "num:i(count(UdpNmChannelConfig/*)) - 1"!]
  FUNC(void, UDPNM_CODE) UdpNm_MainFunction_[!"num:i($I)"!](void)
  {
    UdpNm_MainFunction([!"num:i($I)"!]U);
  }[!//
[!CR!]
[!ENDFOR!]

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function definitions]=========================*/

[!ENDSELECT!][!//
#endif /* #if (UDPNM_ADAPTIVE == STD_OFF) */
/*==================[end of file]===========================================*/
