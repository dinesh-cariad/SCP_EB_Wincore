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

#ifndef CANNM_INT_H
#define CANNM_INT_H

/*===============================[includes]=================================*/

#include <TSAutosar.h>          /* EB specific standard types */
/* !LINKSTO CANNM309,1 */
#include <NmStack_Types.h>      /* Nm specific types */

#include <CanNm_Api.h>              /* Types and decl. from public API */
#include <CanNm_Cfg.h>          /* Module configuration */
/* !LINKSTO CANNM305,1 */
#include <ComStack_Types.h>     /* Include for PduInfoType */

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE CANNM_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig.h>           /* relocatable post-build macros */
#include <TSPBConfig_Access.h>


/*===========================[macro definitions]============================*/




/*------------------------[Defensive programming]----------------------------*/

#if (defined CANNM_PRECONDITION_ASSERT)
#error CANNM_PRECONDITION_ASSERT is already defined
#endif

#if (defined CANNM_PRECONDITION_ASSERT_NO_EVAL)
#error CANNM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANNM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))
#else
#define CANNM_PRECONDITION_ASSERT(Condition, ApiId)
#define CANNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANNM_POSTCONDITION_ASSERT)
#error CANNM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined CANNM_POSTCONDITION_ASSERT_NO_EVAL)
#error CANNM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANNM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))
#else
#define CANNM_POSTCONDITION_ASSERT(Condition, ApiId)
#define CANNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANNM_INVARIANT_ASSERT)
#error CANNM_INVARIANT_ASSERT is already defined
#endif

#if (defined CANNM_INVARIANT_ASSERT_NO_EVAL)
#error CANNM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (CANNM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))
#else
#define CANNM_INVARIANT_ASSERT(Condition, ApiId)
#define CANNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANNM_STATIC_ASSERT)
# error CANNM_STATIC_ASSERT is already defined
#endif
#if (CANNM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define CANNM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define CANNM_STATIC_ASSERT(expr)
#endif

#if (defined CANNM_UNREACHABLE_CODE_ASSERT)
#error CANNM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (CANNM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define CANNM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(CANNM_MODULE_ID, CANNM_INSTANCE_ID, (ApiId))
#else
#define CANNM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined CANNM_INTERNAL_API_ID)
#error CANNM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define CANNM_INTERNAL_API_ID DET_INTERNAL_API_ID



#if (defined CANNM_DET_REPORT_ERROR)
#error CANNM_DET_REPORT_ERROR already defined
#endif

/* define function like macro for development error reporting,
 * if development error detection is enabled */
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO CANNM019,1 */
/** \brief Macro for reporting an error to DET
 **
 ** \param[in] InstanceId Instance ID of the API function
 ** \param[in] ServiceId Service ID of the API function
 ** \param[in] ErrorCode Error code reported to Det module */
#define CANNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)    \
  ((void)Det_ReportError(CANNM_MODULE_ID, (InstanceId), (ServiceId), (ErrorCode)))
#else
#define CANNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)
#endif /* if (COMM_DEV_ERROR_DETECT == STD_ON) */

/** \brief Macro to create a bitmask with single bit set at specified position
 ** (BitPos)
 **
 ** \param[in] BitPos - Bit position */
#if (defined CANNM_BITPOS2MASK)
#error CANNM_BITPOS2MASK is already defined
#endif
#define CANNM_BITPOS2MASK(BitPos) ((uint8)(1U << (BitPos)))

/** \brief Define CANNM_BUSLOADREDACTIVE */
#if (defined CANNM_BUSLOADREDACTIVE)
#error CANNM_BUSLOADREDACTIVE is already defined
#endif
#define CANNM_BUSLOADREDACTIVE          0U

/** \brief The maximum PDU size */
#if (defined CANNM_MAX_PDU_SIZE)
#error CANNM_MAX_PDU_SIZE is already defined
#endif
#define CANNM_MAX_PDU_SIZE             8U

/* !LINKSTO CANNM045,1 */
/** \brief Bit position of the Repeat message bit in the TX/RX PDU */
#if (defined CANNM_CBV_REPEATMESSAGEBIT)
#error CANNM_CBV_REPEATMESSAGEBIT is already defined
#endif
#define CANNM_CBV_REPEATMESSAGEBIT      0U

/* !LINKSTO CANNM045,1 */
/** \brief Bit position of the Active Wake Up Bit in the CBV */
#if (defined CANNM_CBV_ACTIVEWAKEUPBIT)
#error CANNM_CBV_ACTIVEWAKEUPBIT is already defined
#endif
#define CANNM_CBV_ACTIVEWAKEUPBIT       4U

/* Internal State */
/* The numbers describe bit positions */
#if (defined CANNM_COM_DISABLED_BIT)
#error CANNM_COM_DISABLED_BIT is already defined
#endif
#define CANNM_COM_DISABLED_BIT               0U

#if (defined CANNM_NET_REQUESTED_BIT)
#error CANNM_NET_REQUESTED_BIT is already defined
#endif
#define CANNM_NET_REQUESTED_BIT             1U

/* bit 3 is used for setting Remote sleep indication status */
#if (defined CANNM_RSI_TRUE_BIT)
#error CANNM_RSI_TRUE_BIT is already defined
#endif
#define CANNM_RSI_TRUE_BIT 3U

#if (defined CANNM_RMS_ENTERED_FROM_NM_BIT)
#error CANNM_RMS_ENTERED_FROM_NM_BIT already defined
#endif
#define CANNM_RMS_ENTERED_FROM_NM_BIT      0U

#if (defined CANNM_MESSAGE_SENT_ON_CYCLE_BIT)
#error CANNM_MESSAGE_SENT_ON_CYCLE_BIT already defined
#endif
#define CANNM_MESSAGE_SENT_ON_CYCLE_BIT    1U

#if (defined CANNM_EXECUTE_IMMEDIATE_RESTART_BIT)
#error CANNM_EXECUTE_IMMEDIATE_RESTART_BIT already defined
#endif
#define CANNM_EXECUTE_IMMEDIATE_RESTART_BIT      2U

#if (defined CANNM_IN_REPEAT_MESSAGE_STATE_BIT)
#error CANNM_IN_REPEAT_MESSAGE_STATE_BIT already defined
#endif
#define CANNM_IN_REPEAT_MESSAGE_STATE_BIT      3U

/* !LINKSTO CANNM045,1 */
/** \brief Bit position of the Partial Netork Information Bit in the TX/RX PDU
 */
#if (defined CANNM_CBV_PNINFOBIT)
#error CANNM_CBV_PNINFOBIT is already defined
#endif
#define CANNM_CBV_PNINFOBIT                                6U

#if (defined CANNM_CBV_PNINFOBITMASK)
#error CANNM_CBV_PNINFOBITMASK is already defined
#endif
#define CANNM_CBV_PNINFOBITMASK                  CANNM_BITPOS2MASK(CANNM_CBV_PNINFOBIT)

/* Macro when EIRA Timer usage is not used. It is used for mapping
 * PN Index and EIRA timer array */
#if (defined CANNM_EIRA_TIMER_INVALID)
#error CANNM_EIRA_TIMER_INVALID is already defined
#endif
#define CANNM_EIRA_TIMER_INVALID    0xFFU

/* bit 3 is used for setting Remote sleep indication status */
#if (defined CANNM_RSI_TRUE)
#error CANNM_RSI_TRUE is already defined
#endif
#define CANNM_RSI_TRUE                     \
   CANNM_BITPOS2MASK(CANNM_RSI_TRUE_BIT)

/* Bitmasks for Internal States */
#if (defined CANNM_COM_DISABLED)
#error CANNM_COM_DISABLED is already defined
#endif
#define CANNM_COM_DISABLED                     \
   CANNM_BITPOS2MASK(CANNM_COM_DISABLED_BIT)

#if (defined CANNM_NETWORK_REQUESTED)
#error CANNM_NETWORK_REQUESTED is already defined
#endif
#define CANNM_NETWORK_REQUESTED               \
   CANNM_BITPOS2MASK(CANNM_NET_REQUESTED_BIT)

#if (defined CANNM_BUS_LOAD_RED_ENABLED)
#error CANNM_BUS_LOAD_RED_ENABLED is already defined
#endif
#define CANNM_BUS_LOAD_RED_ENABLED            \
   CANNM_BITPOS2MASK(CANNM_BUS_LOAD_RED)

#if (defined CANNM_CHANNEL_CONFIG_BUSLOADREDACTIVE)
#error CANNM_CHANNEL_CONFIG_BUSLOADREDACTIVE is already defined
#endif
#define CANNM_CHANNEL_CONFIG_BUSLOADREDACTIVE(a) \
   ((CANNM_CHANNEL_CONFIG(a).CCFlags & CANNM_BITPOS2MASK(CANNM_BUSLOADREDACTIVE)) != 0U)

#if (defined CANNM_RMS_ENTERED_FROM_NM)
#error CANNM_RMS_ENTERED_FROM_NM is already defined
#endif
#define CANNM_RMS_ENTERED_FROM_NM \
   CANNM_BITPOS2MASK(CANNM_RMS_ENTERED_FROM_NM_BIT)

#if (defined CANNM_MESSAGE_SENT_ON_CYCLE)
#error CANNM_MESSAGE_SENT_ON_CYCLE is already defined
#endif
#define CANNM_MESSAGE_SENT_ON_CYCLE \
   CANNM_BITPOS2MASK(CANNM_MESSAGE_SENT_ON_CYCLE_BIT)

#if (defined CANNM_EXECUTE_IMMEDIATE_RESTART)
#error CANNM_EXECUTE_IMMEDIATE_RESTART is already defined
#endif
#define CANNM_EXECUTE_IMMEDIATE_RESTART \
   CANNM_BITPOS2MASK(CANNM_EXECUTE_IMMEDIATE_RESTART_BIT)

#if (defined CANNM_IN_REPEAT_MESSAGE_STATE)
#error CANNM_IN_REPEAT_MESSAGE_STATE_BIT is already defined
#endif
#define CANNM_IN_REPEAT_MESSAGE_STATE \
   CANNM_BITPOS2MASK(CANNM_IN_REPEAT_MESSAGE_STATE_BIT)

#if (defined CANNM_CHANNEL_STATUS)
#error CANNM_CHANNEL_STATUS is already defined
#endif

#if (defined CANNM_CHANNEL_CONFIG)
#error CANNM_CHANNEL_CONFIG is already defined
#endif

#if (defined CANNM_CHANNEL_BUFFER_OFFSET)
#error CANNM_CHANNEL_BUFFER_OFFSET already defined
#endif

#if (defined CANNM_CHANNEL_USERDATALENGTH)
#error CANNM_CHANNEL_USERDATALENGTH is already defined
#endif

#if (defined CANNM_CHANNEL_MSGCYCLEOFFSET)
#error CANNM_CHANNEL_MSGCYCLEOFFSET is already defined
#endif

#if (defined CANNM_CHANNEL_PDULENGTH)
#error CANNM_CHANNEL_PDULENGTH is already defined
#endif

#if (defined CANNM_CHANNEL_USERTXCONFPDUID)
#error CANNM_CHANNEL_USERTXCONFPDUID is already defined
#endif

#if (defined CANNM_CHANNEL_USERRXPDUID)
#error CANNM_CHANNEL_USERRXPDUID is already defined
#endif

#if (defined CANNM_CHANNEL_TXPDUID)
#error CANNM_CHANNEL_TXPDUID is already defined
#endif

#if (defined CANNM_CHANNEL_NODEID)
#error CANNM_CHANNEL_NODEID is already defined
#endif

#if (defined CANNM_CHANNEL_PNENABLED)
#error CANNM_CHANNEL_PNENABLED is already defined
#endif

#if (defined CANNM_CHANNEL_PN_HANDLE_MULTI_REQ)
#error CANNM_CHANNEL_PN_HANDLE_MULTI_REQ is already defined
#endif

#if (defined CANNM_CHANNEL_USERRXPDUENABLED)
#error CANNM_CHANNEL_USERRXPDUENABLED is already defined
#endif


#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define CANNM_CHANNEL_STATUS(a)            CanNm_ChanStatus[(a)]
#define CANNM_CHANNEL_CONFIG(a)            CanNm_ChanConfig[(a)]
#define CANNM_CHANNEL_BUFFER_OFFSET(a)    (CanNm_RootPtr->BufferOffsets[(a)])
#define CANNM_CHANNEL_USERDATALENGTH(a)   (CanNm_RootPtr->CanNm_ChannelData[(a)].UserDataLength)
#define CANNM_CHANNEL_MSGCYCLEOFFSET(a)   (CanNm_RootPtr->CanNm_ChannelData[(a)].CanNmMsgCycleOffset)
#define CANNM_CHANNEL_PDULENGTH(a)        (CanNm_RootPtr->CanNm_ChannelData[(a)].RxPduLength)
#define CANNM_CHANNEL_USERTXCONFPDUID(a)  (CanNm_RootPtr->CanNm_ChannelData[(a)].UserTxConfPduId)
#define CANNM_CHANNEL_USERRXPDUID(a)      (CanNm_RootPtr->CanNm_ChannelData[(a)].UserRxPduId)
#define CANNM_CHANNEL_TXPDUID(a)          (CanNm_RootPtr->CanNm_ChannelData[(a)].TxPduId)
#define CANNM_CHANNEL_NODEID(a)           (CanNm_RootPtr->CanNm_ChannelData[(a)].CanNmNodeId)
#define CANNM_CHANNEL_PNENABLED(a)        (CanNm_RootPtr->CanNm_ChannelData[(a)].PnEnabled)
#define CANNM_CHANNEL_PN_HANDLE_MULTI_REQ(a)    \
                                          (CanNm_RootPtr->CanNm_ChannelData[(a)].PnHandleMultipleNetworkRequests)
#define CANNM_CHANNEL_USERRXPDUENABLED(a) (CanNm_RootPtr->CanNm_ChannelData[(a)].UserRxPduEnabled)
#else
#define CANNM_CHANNEL_STATUS(a)            CanNm_ChanStatus[0]
#define CANNM_CHANNEL_CONFIG(a)            CanNm_ChanConfig[0]
#define CANNM_CHANNEL_BUFFER_OFFSET(a)    (CanNm_RootPtr->BufferOffsets[(0)])
#define CANNM_CHANNEL_USERDATALENGTH(a)   (CanNm_RootPtr->CanNm_ChannelData[(0)].UserDataLength)
#define CANNM_CHANNEL_MSGCYCLEOFFSET(a)   (CanNm_RootPtr->CanNm_ChannelData[(0)].CanNmMsgCycleOffset)
#define CANNM_CHANNEL_PDULENGTH(a)        (CanNm_RootPtr->CanNm_ChannelData[(0)].RxPduLength)
#define CANNM_CHANNEL_USERTXCONFPDUID(a)  (CanNm_RootPtr->CanNm_ChannelData[(0)].UserTxConfPduId)
#define CANNM_CHANNEL_USERRXPDUID(a)      (CanNm_RootPtr->CanNm_ChannelData[(0)].UserRxPduId)
#define CANNM_CHANNEL_TXPDUID(a)          (CanNm_RootPtr->CanNm_ChannelData[0].TxPduId)
#define CANNM_CHANNEL_NODEID(a)           (CanNm_RootPtr->CanNm_ChannelData[(0)].CanNmNodeId)
#define CANNM_CHANNEL_PNENABLED(a)        (CanNm_RootPtr->CanNm_ChannelData[(0)].PnEnabled)
#define CANNM_CHANNEL_PN_HANDLE_MULTI_REQ(a)    \
                                          (CanNm_RootPtr->CanNm_ChannelData[(0)].PnHandleMultipleNetworkRequests)
#define CANNM_CHANNEL_USERRXPDUENABLED(a) (CanNm_RootPtr->CanNm_ChannelData[(0)].UserRxPduEnabled)
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_NetworkStartIndication,1 */
#if (defined CANNM_CALL_NM_NETWORKSTARTINDICATION)
#error CANNM_CALL_NM_NETWORKSTARTINDICATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_NetworkMode,1 */
#if (defined CANNM_CALL_NM_NETWORKMODE)
#error CANNM_CALL_NM_NETWORKMODE already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_BusSleepMode,1 */
#if (defined CANNM_CALL_NM_BUSSLEEPMODE)
#error CANNM_CALL_NM_BUSSLEEPMODE already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_PrepareBusSleepMode,1 */
#if (defined CANNM_CALL_NM_PREPAREBUSSLEEPMODE)
#error CANNM_CALL_NM_PREPAREBUSSLEEPMODE already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_RemoteSleepIndication,1 */
#if (defined CANNM_CALL_NM_REMOTESLEEPINDICATION)
#error CANNM_CALL_NM_REMOTESLEEPINDICATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_RemoteSleepCancellation,1 */
#if (defined CANNM_CALL_NM_REMOTESLEEPCANCELLATION)
#error CANNM_CALL_NM_REMOTESLEEPCANCELLATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_PduRxIndication,1 */
#if (defined CANNM_CALL_NM_PDURXINDICATION)
#error CANNM_CALL_NM_PDURXINDICATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_StateChangeNotification,1 */
#if (defined CANNM_CALL_NM_STATECHANGENOTIFICATION)
#error CANNM_CALL_NM_STATECHANGENOTIFICATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_RepeatMessageIndication,1 */
#if (defined CANNM_CALL_NM_REPEATMESSAGEINDICATION)
#error CANNM_CALL_NM_REPEATMESSAGEINDICATION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_TxTimeoutException,1 */
#if (defined CANNM_CALL_NM_TXTIMEOUTEXCEPTION)
#error CANNM_CALL_NM_TXTIMEOUTEXCEPTION already defined
#endif

/* !LINKSTO CanNm.EB.SchM_Call_Nm_CarWakeUpIndication,1 */
#if (defined CANNM_CALL_NM_CARWAKEUPINDICATION)
#error CANNM_CALL_NM_CARWAKEUPINDICATION already defined
#endif

/* !LINKSTO CanNm.Config.CanNmMultiCoreSupport,1 */
#if(CANNM_MULTICORE_ENABLED == STD_ON)

/** \brief SchM_Call for Nm_NetworkStartIndication */
#define CANNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).NetworkStartIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_NetworkMode */
#define CANNM_CALL_NM_NETWORKMODE(idx) \
   CANNM_CHANNEL_CONFIG(idx).NetworkMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_BusSleepMode */
#define CANNM_CALL_NM_BUSSLEEPMODE(idx) \
   CANNM_CHANNEL_CONFIG(idx).BusSleepMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_PrepareBusSleepMode */
#define CANNM_CALL_NM_PREPAREBUSSLEEPMODE(idx) \
   CANNM_CHANNEL_CONFIG(idx).PrepareBusSleepMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_RemoteSleepIndication */
#define CANNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).RemoteSleepIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_RemoteSleepCancellation */
#define CANNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).RemoteSleepCancellation(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_PduRxIndication */
#define CANNM_CALL_NM_PDURXINDICATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).PduRxIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_StateChangeNotification */
#define CANNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
   CANNM_CHANNEL_CONFIG(idx).StateChangeNotification(CANNM_CHANNEL_CONFIG(idx).nmChannelId,v0,v1)

/** \brief SchM_Call for Nm_RepeatMessageIndication */
#define CANNM_CALL_NM_REPEATMESSAGEINDICATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).RepeatMessageIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_TxTimeoutException */
#define CANNM_CALL_NM_TXTIMEOUTEXCEPTION(idx) \
   CANNM_CHANNEL_CONFIG(idx).TxTimeoutException(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

/** \brief SchM_Call for Nm_CarWakeUpIndication */
#define CANNM_CALL_NM_CARWAKEUPINDICATION(idx) \
   CANNM_CHANNEL_CONFIG(idx).CarWakeUpIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#else
#define CANNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
   Nm_NetworkStartIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_NETWORKMODE(idx) \
   Nm_NetworkMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_BUSSLEEPMODE(idx) \
   Nm_BusSleepMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_PREPAREBUSSLEEPMODE(idx) \
   Nm_PrepareBusSleepMode(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
   Nm_RemoteSleepIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
   Nm_RemoteSleepCancellation(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_PDURXINDICATION(idx) \
   Nm_PduRxIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
   Nm_StateChangeNotification(CANNM_CHANNEL_CONFIG(idx).nmChannelId,v0,v1)

#define CANNM_CALL_NM_REPEATMESSAGEINDICATION(idx) \
   Nm_RepeatMessageIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_TXTIMEOUTEXCEPTION(idx) \
   Nm_TxTimeoutException(CANNM_CHANNEL_CONFIG(idx).nmChannelId)

#define CANNM_CALL_NM_CARWAKEUPINDICATION(idx) \
   Nm_CarWakeUpIndication(CANNM_CHANNEL_CONFIG(idx).nmChannelId)
#endif

#if (defined CANNM_TX_BUFFER_OFFSET)
#error CANNM_TX_BUFFER_OFFSET already defined
#endif
#define CANNM_TX_BUFFER_OFFSET            (CanNm_RootPtr->TxBufferOffset)

#if (defined PB_REQUIRED_CFG_RAM_SIZE)
#error PB_REQUIRED_CFG_RAM_SIZE already defined
#endif
#define PB_REQUIRED_CFG_RAM_SIZE          (LocalConfigPtr->PbRequiredCfgRamSize)

/** \brief Start Tx Timeout Timer.
 ** \param instIdx index of state machine instance to work on */
#if (defined CANNM_TX_TIMER_START)
#error CANNM_TX_TIMER_START is already defined
#endif
#define CANNM_TX_TIMER_START(instIdx,TimeoutTime)       \
  do                                                    \
  {                                                     \
    CANNM_CHANNEL_STATUS(instIdx).TimeoutTimer          \
      = (TimeoutTime);   \
  } while (0)

/** \brief Stop Tx Timeout Timer.
 ** \param instIdx index of state machine instance to work on */
#if (defined CANNM_TX_TIMER_STOP)
#error CANNM_TX_TIMER_STOP is already defined
#endif
#define CANNM_TX_TIMER_STOP(instIdx)                    \
  do                                                    \
  {                                                     \
    CANNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;    \
  } while (0)

#if (defined CANNM_NETWORK_GW_ERA_REQUEST_AWB)
#error CANNM_NETWORK_GW_ERA_REQUEST_AWB already defined
#endif /* if (defined CANNM_NETWORK_GW_ERA_REQUEST_AWB) */

#define CANNM_NETWORK_GW_ERA_REQUEST_AWB                   3U

#if (defined CANNM_PASSIVE_STARTUP_AWB)
#error CANNM_PASSIVE_STARTUP_AWB already defined
#endif /* if (defined CANNM_PASSIVE_STARTUP_AWB) */

#define CANNM_PASSIVE_STARTUP_AWB                          2U

#if (defined CANNM_NETWORK_REQUEST_AWB)
#error CANNM_NETWORK_REQUEST_AWB already defined
#endif /* if (defined CANNM_NETWORK_REQUEST_AWB) */

#define CANNM_NETWORK_REQUEST_AWB                          1U

/*==================[type definitions]======================================*/

/** \brief Definition of the CanNm_TimeType */
typedef uint16 CanNm_TimeType;

/** \brief Definition of the CanNm_NodeIdType */
typedef uint8 CanNm_NodeIdType;

#if (CANNM_MULTICORE_ENABLED == STD_ON)
typedef Std_ReturnType (*CanNm_Nm_CallsFctPtrType1)(uint8);
typedef Std_ReturnType (*CanNm_Nm_CallsFctPtrType2)(uint8,uint8,uint8);
#endif

/** \brief Type for the channel configuration
 **
 ** This type stores the static data of a CANNM channels */
typedef struct
{
#if (CANNM_MULTICORE_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType1 NetworkStartIndication;
   CanNm_Nm_CallsFctPtrType1 NetworkMode;
   CanNm_Nm_CallsFctPtrType1 BusSleepMode;
   CanNm_Nm_CallsFctPtrType1 PrepareBusSleepMode;
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType1 RemoteSleepIndication;
   CanNm_Nm_CallsFctPtrType1 RemoteSleepCancellation;
#endif
#if (CANNM_PDU_RX_INDICATION_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType1 PduRxIndication;
#endif
#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType2 StateChangeNotification;
#endif
#if (CANNM_REPEAT_MSG_IND_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType1 RepeatMessageIndication;
#endif
#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && \
     (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
   CanNm_Nm_CallsFctPtrType1 TxTimeoutException;
#endif
#if (CANNM_CARWAKEUP_RX_ENABLED == STD_ON)
   CanNm_Nm_CallsFctPtrType1 CarWakeUpIndication;
#endif
#endif
   CanNm_TimeType    CanNmTime;
   CanNm_TimeType    RMSTime;
   CanNm_TimeType    WBSTime;
   CanNm_TimeType    RSITime;
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if (CANNM_IMMEDIATE_TRANSMISSION == STD_ON)
   CanNm_TimeType    ImmediateNmTransmissions;
   CanNm_TimeType    ImmediateNmCycleTime;
#endif /* (CANNM_IMMEDIATE_TRANSMISSION == STD_ON) */
   CanNm_TimeType    MsgCycleTime;
#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
   CanNm_TimeType    MsgTimeoutTime;
#endif
#if (CANNM_BUS_LOAD_REDUCTION_ENABLED == STD_ON)
   CanNm_TimeType    ReducedTime;
#endif
#endif /* passive mode not enabled */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
   PduIdType         TxConfPduId;
#endif
#if ((CANNM_REPEAT_MSG_IND_ENABLED == STD_ON) &&  \
     (CANNM_NODE_DETECTION_ENABLED == STD_ON))
   PduIdType         RxPduId;
#endif
#if (CANNM_COM_USER_DATA_SUPPORT == STD_ON)
   PduIdType         UserTxPduId;
#endif
#if (CANNM_PN_ERA_CALC_ENABLED == STD_ON)
   PduIdType         EraPduId;
   uint8             EraCalcEnabled;
#endif
   uint8             CCFlags;
   uint8             NidPos;
   uint8             CbvPos;
   uint8             UDFBPos;
   NetworkHandleType nmChannelId;
   boolean           AllNmMessagesKeepAwake; /* CANNM068_Conf */
#if (CANNM_CARWAKEUP_RX_ENABLED == STD_ON)
   uint8             CanNmCarWakeUpBitPosition;
   uint8             CanNmCarWakeUpBytePosition;
   uint8             CanNmCarWakeUpFilterNodeId;
   boolean           CanNmCarWakeUpFilterEnabled;
   boolean           CanNmCarWakeUpRxEnabled;
#endif
   boolean           CanNmRepeatMsgIndEnabled;
   boolean           CanNmNodeDetectionEnabled;
   boolean           CanNmNodeIdEnabled;
} CanNm_ChanConfigType;

#if (CANNM_PN_EIRA_CALC_ENABLED == STD_ON)  || (CANNM_PN_ERA_CALC_ENABLED == STD_ON)
typedef struct
{
  /* !LINKSTO CANNM438,1, CANNM428,1 */
  CanNm_TimeType    PnTimer[CANNM_EIRA_MAX_TIMER_SIZE];
  uint8             PnValue[CANNM_PN_INFO_LENGTH];
  uint8             PnInfo[CANNM_PN_INFO_LENGTH];
}CanNm_PnStatusType;
#endif

/** \brief Type for the channel state
 **
 ** This type stores the dynamic data of a CanNm channel; e.g. timers
 ** and states. */
typedef struct
{
   P2VAR(uint8, TYPEDEF, CANNM_VAR_CLEARED) RxPduPtr;
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
   P2VAR(uint8, TYPEDEF, CANNM_VAR_CLEARED) TxPduPtr;
#endif
   CanNm_TimeType    CanNmTimer;
   CanNm_TimeType    UniversalTimer;
   CanNm_TimeType    RmsTimer;
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if (CANNM_IMMEDIATE_TRANSMISSION == STD_ON)
   CanNm_TimeType    ImmediateNmTransmissionCounter;
#endif /* (CANNM_IMMEDIATE_TRANSMISSION == STD_ON) */
   CanNm_TimeType    MsgCycleTimer;
#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
   CanNm_TimeType    TimeoutTimer;
#endif
#endif
#if (CANNM_PN_ERA_CALC_ENABLED == STD_ON)
   CanNm_PnStatusType    Era;
#endif
   Nm_StateType      CurState;
   uint8             ChanStatus;
   uint8             MessageFlags;
#if (CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
   uint8             ActiveWakeUp1;
   uint8             ActiveWakeUp2;
#endif
#if (CANNM_PN_SUPPORTED == STD_ON)
   boolean           PnFilterEnabled;
#endif
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
   boolean           NmTimerExpired;
   boolean           FirstCanIfTransmitOk;
#endif
} CanNm_ChanStatusType;

/*=====================[external function declarations]=====================*/
#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>
#if (CANNM_PN_EIRA_CALC_ENABLED == STD_ON) || (CANNM_PN_ERA_CALC_ENABLED == STD_ON)

/** \brief checks whether the received message is valid or not
 **
 ** This function checks whether the received message is valid or not.
 **
 ** \param[in] PduId Pdu Id of the received PDU.
 **
 ** \return Received message is valid or not.
 */
extern FUNC(boolean, CANNM_CODE) CanNm_IsValidPnMessage
(
  PduIdType PduId
);
#endif
#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>
/*=====================[internal function declarations]=====================*/
#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

#if ((CANNM_COM_USER_DATA_SUPPORT == STD_ON) || (CANNM_PN_EIRA_CALC_ENABLED == STD_ON))

extern FUNC(void, CANNM_CODE) CanNm_GetPduUserData
(
  uint8 instIdx,
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfo
);

#endif

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*=====================[external constants declarations]====================*/
#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

extern CONST(CanNm_ChanConfigType, CANNM_CONST)
   CanNm_ChanConfig[CANNM_NUMBER_OF_CHANNELS];

#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

#define CANNM_START_SEC_CONST_8
#include <CanNm_MemMap.h>

extern CONST(NetworkHandleType, CANNM_CONST)
   CanNm_IndexFromNmChannelHandle[CANNM_INDEXFROMNMCHANNELHANDLE_NUM];

#define CANNM_STOP_SEC_CONST_8
#include <CanNm_MemMap.h>

/*=====================[internal constants declarations]====================*/

/*=======================[external data declarations]=======================*/

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanNm_MemMap.h>

/** \brief Declaration of the extern variable CanNm_ChanStatus */
extern VAR(CanNm_ChanStatusType, CANNM_VAR_CLEARED)
   CanNm_ChanStatus[CANNM_NUMBER_OF_CHANNELS];

extern P2CONST(CanNm_ConfigType, CANNM_VAR_CLEARED, CANNM_APPL_DATA) CanNm_RootPtr;

#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanNm_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef CANNM_INT_H */

/*==============================[end of file]===============================*/
