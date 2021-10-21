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

#ifndef UDPNM_INT_H
#define UDPNM_INT_H

/*===============================[includes]=================================*/

#include <TSAutosar.h>          /* EB specific standard types */
/* !LINKSTO EB_SWS_UdpNm_00082_6,1 */
#include <NmStack_Types.h>      /* Nm specific types */

#include <UdpNm_Api.h>              /* Types and decl. from public API */
#include <UdpNm_Cfg.h>          /* Module configuration */
/* !LINKSTO EB_SWS_UdpNm_00082_1,1 */
#include <ComStack_Types.h>     /* Include for PduInfoType */

#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE UDPNM_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig.h>           /* relocatable post-build macros */
#include <TSPBConfig_Access.h>


/*===========================[macro definitions]============================*/
#if (defined UDPNM_DET_REPORT_ERROR)
#error UDPNM_DET_REPORT_ERROR already defined
#endif

/* define function like macro for development error reporting,
 * if development error detection is enabled */
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to DET
 **
 ** \param[in] InstanceId Instance ID of the API function
 ** \param[in] ServiceId Service ID of the API function
 ** \param[in] ErrorCode Error code reported to Det module */
#define UDPNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)    \
  ((void)Det_ReportError(UDPNM_MODULE_ID, (InstanceId), (ServiceId), (ErrorCode)))

#else

#define UDPNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)

#endif

/** \brief Macro to create a bitmask with single bit set at specified position (BitPos)
 **
 ** \param[in] BitPos - Bit position */
#if (defined UDPNM_BITPOS2MASK)
#error UDPNM_BITPOS2MASK already defined
#endif
#define UDPNM_BITPOS2MASK(BitPos) ((uint8)(1U << (BitPos)))

/** \brief The maximum PDU size */
#if (defined UDPNM_MAX_PDU_SIZE)
#error UDPNM_MAX_PDU_SIZE already defined
#endif
#define UDPNM_MAX_PDU_SIZE             10U

/* !LINKSTO SWS_UdpNm_00045,2, UdpNm.ASR431.SWS_UdpNm_00045,1 */
/** \brief Bit position of the Repeat message bit in the TX/RX PDU */
#if (defined UDPNM_CBV_REPEATMESSAGEBIT)
#error UDPNM_CBV_REPEATMESSAGEBIT already defined
#endif
#define UDPNM_CBV_REPEATMESSAGEBIT     0U

/* !LINKSTO SWS_UdpNm_00045,2, UdpNm.ASR431.SWS_UdpNm_00045,1 */
/** \brief Bit position for Active Wake Up Bit in the CBV */
#if (defined UDPNM_CBV_ACTIVEWAKEUPBIT)
#error UDPNM_CBV_ACTIVEWAKEUPBIT already defined
#endif
#define UDPNM_CBV_ACTIVEWAKEUPBIT      4U

/* !LINKSTO SWS_UdpNm_00045,2, UdpNm.ASR431.SWS_UdpNm_00045,1 */
/** \brief Bit position of the Partial Netork Information Bit in the TX/RX PDU
 */
#if (defined UDPNM_CBV_PNINFOBIT)
#error UDPNM_CBV_PNINFOBIT already defined
#endif
#define UDPNM_CBV_PNINFOBIT            6U

/* Internal State */
/* The numbers describe bit positions */
#if (defined UDPNM_COM_DISABLED_BIT)
#error UDPNM_COM_DISABLED_BIT already defined
#endif
#define UDPNM_COM_DISABLED_BIT         0U

#if (defined UDPNM_NETWORK_REQUESTED_BIT)
#error UDPNM_NETWORK_REQUESTED_BIT already defined
#endif
#define UDPNM_NETWORK_REQUESTED_BIT    1U

/* Bit 4 is used for setting Remote sleep indication status */
#if (defined UDPNM_RSI_CALLED_BIT)
#error UDPNM_RSI_CALLED_BIT already defined
#endif
#define UDPNM_RSI_CALLED_BIT           4U

#if (defined UDPNM_RMS_ENTERED_FROM_NM)
#error UDPNM_RMS_ENTERED_FROM_NM already defined
#endif
#define UDPNM_RMS_ENTERED_FROM_NM      1U

#if (defined UDPNM_MESSAGE_SENT_ON_CYCLE)
#error UDPNM_MESSAGE_SENT_ON_CYCLE already defined
#endif
#define UDPNM_MESSAGE_SENT_ON_CYCLE    2U

#if (defined UDPNM_EXECUTE_IMMEDIATE_RESTART)
#error UDPNM_EXECUTE_IMMEDIATE_RESTART already defined
#endif
#define UDPNM_EXECUTE_IMMEDIATE_RESTART      4U

#if (defined UDPNM_IN_REPEAT_MESSAGE_STATE)
#error UDPNM_IN_REPEAT_MESSAGE_STATE already defined
#endif
#define UDPNM_IN_REPEAT_MESSAGE_STATE      8U
/* Macro when EIRA Timer usage is not used. It is used for mapping
 * PN Index and EIRA timer array */
#if (defined UDPNM_EIRA_TIMER_INVALID)
#error UDPNM_EIRA_TIMER_INVALID already defined
#endif
#define UDPNM_EIRA_TIMER_INVALID    0xFFU

#if (defined UDPNM_COM_DISABLED)
#error UDPNM_COM_DISABLED is already defined
#endif
#define UDPNM_COM_DISABLED                     \
   UDPNM_BITPOS2MASK(UDPNM_COM_DISABLED_BIT)
#if (defined UDPNM_CHANNEL_STATUS)
#error UDPNM_CHANNEL_STATUS already defined
#endif

#if (defined UDPNM_NETWORK_REQUESTED)
#error UDPNM_NETWORK_REQUESTED is already defined
#endif
#define UDPNM_NETWORK_REQUESTED               \
   UDPNM_BITPOS2MASK(UDPNM_NETWORK_REQUESTED_BIT)

#if (defined UDPNM_CHANNEL_CONFIG)
#error UDPNM_CHANNEL_CONFIG already defined
#endif

#if (defined UDPNM_CHANNEL_PNENABLED)
#error UDPNM_CHANNEL_PNENABLED already defined
#endif

#if (defined UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ)
#error UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ already defined
#endif

#if (defined UDPNM_CHANNEL_TXPDUID)
#error UDPNM_CHANNEL_TXPDUID already defined
#endif

#if (defined UDPNM_CHANNEL_USERTXCONFPDUID)
#error UDPNM_CHANNEL_USERTXCONFPDUID already defined
#endif

#if (defined UDPNM_CHANNEL_USERRXPDUID)
#error UDPNM_CHANNEL_USERRXPDUID is already defined
#endif

#if (defined UDPNM_CHANNEL_USERRXPDUENABLED)
#error UDPNM_CHANNEL_USERRXPDUENABLED is already defined
#endif

#if (defined UDPNM_CHANNEL_NODEID)
#error UDPNM_CHANNEL_NODEID is already defined
#endif


#if (defined UDPNM_CHANNEL_PDULENGTH)
#error UDPNM_CHANNEL_PDULENGTH already defined
#endif

#if (defined UDPNM_CHANNEL_MSGCYCLEOFFSET)
#error UDPNM_CHANNEL_MSGCYCLEOFFSET already defined
#endif

#if (defined UDPNM_CHANNEL_BUFFER_OFFSET)
#error UDPNM_CHANNEL_BUFFER_OFFSET already defined
#endif

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)

#define UDPNM_CHANNEL_STATUS(a)           UdpNm_ChanStatus[(a)]
#define UDPNM_CHANNEL_CONFIG(a)           UdpNm_ChanConfig[(a)]
#define UDPNM_CHANNEL_PNENABLED(a)        (UdpNm_RootPtr->UdpNm_ChannelData[(a)].PnEnabled)
#define UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ(a)    \
                                      (UdpNm_RootPtr->UdpNm_ChannelData[(a)].PnHandleMultipleNetworkRequests)
#define UDPNM_CHANNEL_TXPDUID(a)          (UdpNm_RootPtr->UdpNm_ChannelData[(a)].TxPduId)
#define UDPNM_CHANNEL_USERTXCONFPDUID(a)  (UdpNm_RootPtr->UdpNm_ChannelData[(a)].UserTxConfPduId)
#define UDPNM_CHANNEL_USERRXPDUID(a)      (UdpNm_RootPtr->UdpNm_ChannelData[(a)].UserRxPduId)
#define UDPNM_CHANNEL_USERRXPDUENABLED(a) (UdpNm_RootPtr->UdpNm_ChannelData[(a)].UserRxPduEnabled)
#define UDPNM_CHANNEL_NODEID(a)           (UdpNm_RootPtr->UdpNm_ChannelData[(a)].UdpNmNodeId)
#define UDPNM_CHANNEL_PDULENGTH(a)        (UdpNm_RootPtr->UdpNm_ChannelData[(a)].RxPduLength)
#define UDPNM_CHANNEL_MSGCYCLEOFFSET(a)   (UdpNm_RootPtr->UdpNm_ChannelData[(a)].MsgCycleOffset)
#define UDPNM_CHANNEL_BUFFER_OFFSET(a)    (UdpNm_RootPtr->BufferOffsets[(a)])

#else

#define UDPNM_CHANNEL_STATUS(a)           UdpNm_ChanStatus[(0)]
#define UDPNM_CHANNEL_CONFIG(a)           UdpNm_ChanConfig[(0)]
#define UDPNM_CHANNEL_PNENABLED(a)        (UdpNm_RootPtr->UdpNm_ChannelData[(0)].PnEnabled)
#define UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ(a)    \
                                      (UdpNm_RootPtr->UdpNm_ChannelData[(0)].PnHandleMultipleNetworkRequests)
#define UDPNM_CHANNEL_TXPDUID(a)          (UdpNm_RootPtr->UdpNm_ChannelData[(0)].TxPduId)
#define UDPNM_CHANNEL_USERTXCONFPDUID(a)  (UdpNm_RootPtr->UdpNm_ChannelData[(0)].UserTxConfPduId)
#define UDPNM_CHANNEL_USERRXPDUID(a)      (UdpNm_RootPtr->UdpNm_ChannelData[(0)].UserRxPduId)
#define UDPNM_CHANNEL_USERRXPDUENABLED(a) (UdpNm_RootPtr->UdpNm_ChannelData[(0)].UserRxPduEnabled)
#define UDPNM_CHANNEL_NODEID(a)           (UdpNm_RootPtr->UdpNm_ChannelData[(0)].UdpNmNodeId)
#define UDPNM_CHANNEL_PDULENGTH(a)        (UdpNm_RootPtr->UdpNm_ChannelData[(0)].RxPduLength)
#define UDPNM_CHANNEL_MSGCYCLEOFFSET(a)   (UdpNm_RootPtr->UdpNm_ChannelData[(0)].MsgCycleOffset)
#define UDPNM_CHANNEL_BUFFER_OFFSET(a)    (UdpNm_RootPtr->BufferOffsets[(0)])

#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_NetworkStartIndication,1 */
#if (defined UDPNM_CALL_NM_NETWORKSTARTINDICATION)
#error UDPNM_CALL_NM_NETWORKSTARTINDICATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_NetworkMode,1 */
#if (defined UDPNM_CALL_NM_NETWORKMODE)
#error UDPNM_CALL_NM_NETWORKMODE is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_BusSleepMode,1 */
#if (defined UDPNM_CALL_NM_BUSSLEEPMODE)
#error UDPNM_CALL_NM_BUSSLEEPMODE is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_PrepareBusSleepMode,1 */
#if (defined UDPNM_CALL_NM_PREPAREBUSSLEEPMODE)
#error UDPNM_CALL_NM_PREPAREBUSSLEEPMODE is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_RemoteSleepIndication,1 */
#if (defined UDPNM_CALL_NM_REMOTESLEEPINDICATION)
#error UDPNM_CALL_NM_REMOTESLEEPINDICATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_RemoteSleepCancellation,1 */
#if (defined UDPNM_CALL_NM_REMOTESLEEPCANCELLATION)
#error UDPNM_CALL_NM_REMOTESLEEPCANCELLATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_PduRxIndication,1 */
#if (defined UDPNM_CALL_NM_PDURXINDICATION)
#error UDPNM_CALL_NM_PDURXINDICATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_StateChangeNotification,1 */
#if (defined UDPNM_CALL_NM_STATECHANGENOTIFICATION)
#error UDPNM_CALL_NM_STATECHANGENOTIFICATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_RepeatMessageIndication,1 */
#if (defined UDPNM_CALL_NM_REPEATMESSAGEINDICATION)
#error UDPNM_CALL_NM_REPEATMESSAGEINDICATION is already defined
#endif

/* !LINKSTO UdpNm.EB.SchM_Call_Nm_CarWakeUpIndication,1 */
#if (defined UDPNM_CALL_NM_CARWAKEUPINDICATION)
#error UDPNM_CALL_NM_CARWAKEUPINDICATION is already defined
#endif

/* !LINKSTO UdpNm.Config.UdpNmMultiCoreSupport,1 */
#if (UDPNM_MULTICORE_SUPPORT == STD_ON)

/** \brief SchM_Call for Nm_NetworkStartIndication */
#define UDPNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).NetworkStartIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_NetworkMode */
#define UDPNM_CALL_NM_NETWORKMODE(idx) \
        UDPNM_CHANNEL_CONFIG(idx).NetworkMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_BusSleepMode */
#define UDPNM_CALL_NM_BUSSLEEPMODE(idx) \
        UDPNM_CHANNEL_CONFIG(idx).BusSleepMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_PrepareBusSleepMode */
#define UDPNM_CALL_NM_PREPAREBUSSLEEPMODE(idx) \
        UDPNM_CHANNEL_CONFIG(idx).PrepareBusSleepMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_RemoteSleepIndication */
#define UDPNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).RemoteSleepIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_RemoteSleepCancellation */
#define UDPNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).RemoteSleepCancellation(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_PduRxIndication */
#define UDPNM_CALL_NM_PDURXINDICATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).PduRxIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_StateChangeNotification */
#define UDPNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
        UDPNM_CHANNEL_CONFIG(idx).StateChangeNotification(UDPNM_CHANNEL_CONFIG(idx).nmChannelId,v0,v1)

/** \brief SchM_Call for Nm_RepeatMessageIndication */
#define UDPNM_CALL_NM_REPEATMESSAGEINDICATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).RepeatMessageIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_CarWakeUpIndication */
#define UDPNM_CALL_NM_CARWAKEUPINDICATION(idx) \
        UDPNM_CHANNEL_CONFIG(idx).CarWakeUpIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#else /* (UDPNM_MULTICORE_SUPPORT == STD_OFF) */
#define UDPNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
        Nm_NetworkStartIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_NETWORKMODE(idx) \
        Nm_NetworkMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_BUSSLEEPMODE(idx) \
        Nm_BusSleepMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_PREPAREBUSSLEEPMODE(idx) \
        Nm_PrepareBusSleepMode(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
        Nm_RemoteSleepIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
        Nm_RemoteSleepCancellation(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_PDURXINDICATION(idx) \
        Nm_PduRxIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
        Nm_StateChangeNotification(UDPNM_CHANNEL_CONFIG(idx).nmChannelId,v0,v1)

#define UDPNM_CALL_NM_REPEATMESSAGEINDICATION(idx) \
        Nm_RepeatMessageIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)

#define UDPNM_CALL_NM_CARWAKEUPINDICATION(idx) \
        Nm_CarWakeUpIndication(UDPNM_CHANNEL_CONFIG(idx).nmChannelId)
#endif

#if (defined UDPNM_TX_BUFFER_OFFSET)
#error UDPNM_TX_BUFFER_OFFSET already defined
#endif
#define UDPNM_TX_BUFFER_OFFSET            (UdpNm_RootPtr->TxBufferOffset)

#if (defined UDPNM_PBRAM_SIZE)
#error UDPNM_PBRAM_SIZE already defined
#endif
#define UDPNM_PBRAM_SIZE                  (LocalConfigPtr->PbRamSize)

#if (defined UDPNM_NETWORK_GW_ERA_REQUEST_AWB)
#error UDPNM_NETWORK_GW_ERA_REQUEST_AWB already defined
#endif /* if (defined UDPNM_NETWORK_GW_ERA_REQUEST_AWB) */

#define UDPNM_NETWORK_GW_ERA_REQUEST_AWB                   3U

#if (defined UDPNM_PASSIVE_STARTUP_AWB)
#error UDPNM_PASSIVE_STARTUP_AWB already defined
#endif /* if (defined UDPNM_PASSIVE_STARTUP_AWB) */

#define UDPNM_PASSIVE_STARTUP_AWB                          2U

#if (defined UDPNM_NETWORK_REQUEST_AWB)
#error UDPNM_NETWORK_REQUEST_AWB already defined
#endif /* if (defined UDPNM_NETWORK_REQUEST_AWB) */

#define UDPNM_NETWORK_REQUEST_AWB                          1U

/*==================[type definitions]======================================*/

/** \brief Definition of the UdpNm_TimeType */
typedef uint16 UdpNm_TimeType;

/** \brief Definition of the UdpNm_NodeIdType */
typedef uint8 UdpNm_NodeIdType;

#if (UDPNM_MULTICORE_SUPPORT == STD_ON)
typedef Std_ReturnType (*UdpNm_Nm_CallsFctPtrType1)(uint8);
typedef Std_ReturnType (*UdpNm_Nm_CallsFctPtrType2)(uint8,uint8,uint8);
#endif

/** \brief Type for the channel configuration
 **
 ** This type stores the static data of a UDPNM channels */
typedef struct
{
#if (UDPNM_MULTICORE_SUPPORT == STD_ON)
   UdpNm_Nm_CallsFctPtrType1 NetworkStartIndication;
   UdpNm_Nm_CallsFctPtrType1 NetworkMode;
   UdpNm_Nm_CallsFctPtrType1 BusSleepMode;
   UdpNm_Nm_CallsFctPtrType1 PrepareBusSleepMode;
#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
   UdpNm_Nm_CallsFctPtrType1 RemoteSleepIndication;
   UdpNm_Nm_CallsFctPtrType1 RemoteSleepCancellation;
#endif
#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
   UdpNm_Nm_CallsFctPtrType1 PduRxIndication;
#endif
#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
   UdpNm_Nm_CallsFctPtrType2 StateChangeNotification;
#endif
#if (UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON)
   UdpNm_Nm_CallsFctPtrType1 RepeatMessageIndication;
#endif
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
   UdpNm_Nm_CallsFctPtrType1 CarWakeUpIndication;
#endif
#endif
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
   uint32            UdpNmCarWakeUpBytePosition;
#endif
   UdpNm_TimeType    UdpNmTimeoutTime;
   UdpNm_TimeType    RMSTime;
   UdpNm_TimeType    WBSTime;
   UdpNm_TimeType    RSITime;
   /* !LINKSTO SWS_UdpNm_00072,2 */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
   UdpNm_TimeType    ImmediateNmTransmissions;
   UdpNm_TimeType    ImmediateNmCycleTime;
   UdpNm_TimeType    MsgCycleTime;
#endif
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
   PduIdType         TxConfPduId;
#endif
   PduIdType         RxPduId;
#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
   PduIdType         UserTxPduId;
#endif
#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
   PduIdType         EraPduId;
   uint8             EraCalcEnabled;
#endif
   uint8             NidPos;
   uint8             CbvPos;
   uint8             UDFBPos;
   NetworkHandleType nmChannelId;
   boolean           AllNmMessagesKeepAwake;
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
   uint8             UdpNmCarWakeUpBitPosition;
   uint8             UdpNmCarWakeUpFilterNodeId;
   boolean           UdpNmCarWakeUpFilterEnabled;
   boolean           UdpNmCarWakeUpRxEnabled;
#endif
   boolean           UdpNmRepeatMsgIndEnabled;
   boolean           UdpNmNodeDetectionEnabled;
   boolean           UdpNmNodeIdEnabled;
   boolean           UdpNmRetryFirstMessageRequest;
} UdpNm_ChanConfigType;

#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)  || (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
#if (UDPNM_ADAPTIVE == STD_OFF)
typedef struct
{
  /* !LINKSTO SWS_UdpNm_00358,2, UdpNm.ASR431.SWS_UdpNm_00358,1 */
  UdpNm_TimeType    PnTimer[UDPNM_EIRA_MAX_TIMER_SIZE];
  uint8             PnValue[UDPNM_PN_INFO_LENGTH];
  uint8             PnInfo[UDPNM_PN_INFO_LENGTH];
}UdpNm_PnStatusType;
#else
typedef struct
{
  /* !LINKSTO SWS_UdpNm_00358,2, UdpNm.ASR431.SWS_UdpNm_00358,1 */
  UdpNm_TimeType    *PnTimer;
  uint8             *PnValue;
  uint8             *PnInfo;
}UdpNm_PnStatusType;
#endif
#endif

/** \brief Type for the channel state
 **
 ** This type stores the dynamic data of a UdpNm channel; e.g. timers
 ** and states. */
typedef struct
{
   P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) RxPduPtr;
   /* !LINKSTO SWS_UdpNm_00072,2 */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
   P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) TxPduPtr;
#endif
#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
  UdpNm_PnStatusType  Era;
#endif
  UdpNm_TimeType      UdpNmTimeoutTimer;
  UdpNm_TimeType      UniversalTimer;
  /* Used for: */
  /* UDPNM_REMOTE_SLEEP_IND_TIME if in Normal Operation state */
  /* UDPNM_WAIT_BUS_SLEEP_TIME if in Prepare Bus Sleep state */
  UdpNm_TimeType      RmsTimer;
  /* !LINKSTO SWS_UdpNm_00072,2 */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  UdpNm_TimeType      ImmediateNmTransmissionCounter;
  UdpNm_TimeType      MsgCycleTimer;
#endif
  Nm_StateType        CurState;
  uint8               ChanStatus;
  uint8               MessageFlags;
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  uint8               ActiveWakeUp1;
  uint8               ActiveWakeUp2;
#endif
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  boolean            NmTimerExpired;
  boolean            FirstSoAdIfTransmitOk;
#endif

} UdpNm_ChanStatusType;

/*=====================[external function declarations]=====================*/
#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

extern FUNC(void, UDPNM_CODE) UdpNm_MainFunction
(
  uint8 instIdx
);

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>
/*=====================[internal function declarations]=====================*/
#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

#if ((UDPNM_COM_USER_DATA_SUPPORT == STD_ON) || (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON))

/** \brief Collect the NM User Data from the referenced Nm I-PDU
 **
 ** Collect the NM User Data from the referenced NM I-PDU by calling
 ** PduR_UdpNmTriggerTransmit and combine the user data with the further NM
 ** bytes each time before it requests the transmission of the corresponding NM message.
 ** Aggregate the PnInfo into EiraStatus
 **
 ** \param[in] instIdx the state machine index
 ** \param[in] pduInfo the PDU information
 **
 ** \return void
 */
extern FUNC(void, UDPNM_CODE) UdpNm_GetPduUserData
(
  uint8 instIdx,
  P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) pduInfo
);

#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*=====================[external constants declarations]====================*/
#if (UDPNM_ADAPTIVE == STD_OFF)

#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

extern CONST(UdpNm_ChanConfigType, UDPNM_CONST) UdpNm_ChanConfig[UDPNM_NUMBER_OF_CHANNELS];

#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

#define UDPNM_START_SEC_CONST_8
#include <UdpNm_MemMap.h>

extern CONST(NetworkHandleType, UDPNM_CONST)
   UdpNm_IndexFromNmChannelHandle[UDPNM_INDEXFROMNMCHANNELHANDLE_NUM];

#define UDPNM_STOP_SEC_CONST_8
#include <UdpNm_MemMap.h>

#else /* (UDPNM_ADAPTIVE == STD_OFF) */

#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

extern P2VAR(UdpNm_ChanConfigType, UDPNM_VAR, UDPNM_VAR) UdpNm_ChanConfig;

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

#define UDPNM_START_SEC_VAR_CLEARED_8
#include <UdpNm_MemMap.h>

extern P2VAR(NetworkHandleType, UDPNM_VAR, UDPNM_VAR)
   UdpNm_IndexFromNmChannelHandle;

#define UDPNM_STOP_SEC_VAR_CLEARED_8
#include <UdpNm_MemMap.h>

#endif

/*=====================[internal constants declarations]====================*/

/*=======================[external data declarations]=======================*/

#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Partial netoworking info: EiraValue and EiraTimer */
#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
extern VAR(UdpNm_PnStatusType, UDPNM_VAR) UdpNm_EiraStatus;
#endif

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

#if (UDPNM_ADAPTIVE == STD_OFF)
#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Declaration of the extern variable UdpNm_ChanStatus */
extern VAR(UdpNm_ChanStatusType, UDPNM_VAR) UdpNm_ChanStatus[UDPNM_NUMBER_OF_CHANNELS];

/** \brief Declaration of the extern variable UdpNm_RootPtr */
extern P2CONST(UdpNm_ConfigType, UDPNM_VAR, UDPNM_APPL_DATA) UdpNm_RootPtr;

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>
#else
#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Declaration of the extern variable UdpNm_ChanStatus */
extern P2VAR(UdpNm_ChanStatusType, UDPNM_VAR, UDPNM_APPL_DATA) UdpNm_ChanStatus;

/** \brief Declaration of the extern variable UdpNm_RootPtr */
extern P2CONST(UdpNm_ConfigType, UDPNM_VAR, UDPNM_APPL_DATA) UdpNm_RootPtr;

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>
#endif

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef UDPNM_INT_H */

/*==============================[end of file]===============================*/
