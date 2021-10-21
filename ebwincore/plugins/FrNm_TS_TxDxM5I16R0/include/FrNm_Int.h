/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_INT_H
#define FRNM_INT_H

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions where the number of
 * parentheses matter.
 */

/*==================[inclusions]=============================================*/

#include <NmStack_Types.h>      /* Nm specific types */
#include <FrNm_Api.h>              /* Types and decl. from public API */
#include <FrNm_Hsm.h>
#include <FrNm_Cfg.h>          /* Module configuration */
#include <ComStack_Types.h>     /* Include for PduInfoType */
#include <TSAutosar.h>

#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE FRNM_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig.h>           /* relocatable post-build macros */
#include <TSPBConfig_Access.h>

/*==================[macros]================================================*/

/* Service IDs */
#if (defined FRNM_INIT_ID)
#error FRNM_INIT_ID already defined
#endif
#define FRNM_INIT_ID                        0x00U

#if (defined FRNM_PASSIVESTARTUP_ID)
#error FRNM_PASSIVESTARTUP_ID already defined
#endif
#define FRNM_PASSIVESTARTUP_ID              0x01U

#if (defined FRNM_NETWORKREQUEST_ID)
#error FRNM_NETWORKREQUEST_ID already defined
#endif
#define FRNM_NETWORKREQUEST_ID              0x02U

#if (defined FRNM_NETWORKRELEASE_ID)
#error FRNM_NETWORKRELEASE_ID already defined
#endif
#define FRNM_NETWORKRELEASE_ID              0x03U

#if (defined FRNM_ENABLECOMMUNICATION_ID)
#error FRNM_ENABLECOMMUNICATION_ID already defined
#endif
#define FRNM_ENABLECOMMUNICATION_ID         0x05U

#if (defined FRNM_SETUSERDATA_ID)
#error FRNM_SETUSERDATA_ID already defined
#endif
#define FRNM_SETUSERDATA_ID                 0x06U

#if (defined FRNM_GETUSERDATA_ID)
#error FRNM_GETUSERDATA_ID already defined
#endif
#define FRNM_GETUSERDATA_ID                 0x07U

#if (defined FRNM_GETPDUDATA_ID)
#error FRNM_GETPDUDATA_ID already defined
#endif
#define FRNM_GETPDUDATA_ID                  0x08U

#if (defined FRNM_REPEATMESSAGEREQUEST_ID)
#error FRNM_REPEATMESSAGEREQUEST_ID already defined
#endif
#define FRNM_REPEATMESSAGEREQUEST_ID        0x09U

#if (defined FRNM_GETNODEIDENTIFIER_ID)
#error FRNM_GETNODEIDENTIFIER_ID already defined
#endif
#define FRNM_GETNODEIDENTIFIER_ID           0x0aU

#if (defined FRNM_GETLOCALNODEIDENTIFIER_ID)
#error FRNM_GETLOCALNODEIDENTIFIER_ID already defined
#endif
#define FRNM_GETLOCALNODEIDENTIFIER_ID      0x0bU

#if (defined FRNM_DISABLECOMMUNICATION_ID)
#error FRNM_DISABLECOMMUNICATION_ID already defined
#endif
#define FRNM_DISABLECOMMUNICATION_ID        0x0cU

#if (defined FRNM_REQUESTBUSSYNCHRONIZATION_ID)
#error FRNM_REQUESTBUSSYNCHRONIZATION_ID already defined
#endif
#define FRNM_REQUESTBUSSYNCHRONIZATION_ID   0xc0U

#if (defined FRNM_CHECKREMOTESLEEPINDICATION_ID)
#error FRNM_CHECKREMOTESLEEPINDICATION_ID already defined
#endif
#define FRNM_CHECKREMOTESLEEPINDICATION_ID  0x0dU

#if (defined FRNM_GETSTATE_ID)
#error FRNM_GETSTATE_ID already defined
#endif
#define FRNM_GETSTATE_ID                    0x0eU

#if (defined FRNM_GETVERSIONINFO_ID)
#error FRNM_GETVERSIONINFO_ID already defined
#endif
#define FRNM_GETVERSIONINFO_ID              0x0fU

#if (defined FRNM_RXINDICATION_ID)
#error FRNM_RXINDICATION_ID already defined
#endif
#define FRNM_RXINDICATION_ID                0x42U

#if (defined FRNM_TRIGGERTRANSMIT_ID)
#error FRNM_TRIGGERTRANSMIT_ID already defined
#endif
#define FRNM_TRIGGERTRANSMIT_ID             0x41U

#if (defined FRNM_TXCONFIRMATION_ID)
#error FRNM_TXCONFIRMATION_ID already defined
#endif
#define FRNM_TXCONFIRMATION_ID              0x40U

#if (defined FRNM_MAINFUNCTION_ID)
#error FRNM_MAINFUNCTION_ID already defined
#endif
#define FRNM_MAINFUNCTION_ID                0xf0U

/* Pdu schedule variants */

#if (defined FRNM_PDU_SCHEDULE_VARIANT_1)
#error FRNM_PDU_SCHEDULE_VARIANT_1 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_1  0x01U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_2)
#error FRNM_PDU_SCHEDULE_VARIANT_2 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_2  0x02U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_3)
#error FRNM_PDU_SCHEDULE_VARIANT_3 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_3  0x03U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_4)
#error FRNM_PDU_SCHEDULE_VARIANT_4 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_4  0x04U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_5)
#error FRNM_PDU_SCHEDULE_VARIANT_5 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_5  0x05U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_6)
#error FRNM_PDU_SCHEDULE_VARIANT_6 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_6  0x06U

#if (defined FRNM_PDU_SCHEDULE_VARIANT_7)
#error FRNM_PDU_SCHEDULE_VARIANT_7 already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_7  0x07U

/* Bit mask for Pdu schedule variants */
#if (defined FRNM_PDU_SCHEDULE_VARIANT_MASK)
#error FRNM_PDU_SCHEDULE_VARIANT_MASK already defined
#endif
#define  FRNM_PDU_SCHEDULE_VARIANT_MASK 0x07U

/* Bit mask for PN info bit (CRI) */
#if (defined FRNM_CBV_PNINFOBIT_MASK)
#error FRNM_CBV_PNINFOBIT_MASK already defined
#endif
#define  FRNM_CBV_PNINFOBIT_MASK    0x40U

/* Bit mask for Sleep Ready bit */
#if (defined FRNM_CBV_SLEEPREADYBIT)
#error FRNM_CBV_SLEEPREADYBIT already defined
#endif
#define  FRNM_CBV_SLEEPREADYBIT     0x03U

/* Bit mask for Active wakeUp bit */
#if (defined FRNM_CBV_ACTIVEWAKEUPBIT)
#error FRNM_CBV_ACTIVEWAKEUPBIT already defined
#endif
#define  FRNM_CBV_ACTIVEWAKEUPBIT   0x04U

/* Bit masks for ChannelProperty */
/* Bit mask for control bit vector */
#if (defined FRNM_CONTROL_BIT_VECTOR)
#error FRNM_CONTROL_BIT_VECTOR already defined
#endif
#define FRNM_CONTROL_BIT_VECTOR        0x08U
/* Bit mask for repeat message bit */
#if (defined FRNM_REPEAT_MESSAGE_BIT)
#error FRNM_REPEAT_MESSAGE_BIT already defined
#endif
#define FRNM_REPEAT_MESSAGE_BIT        0x10U
/* Bit mask for synchronization bit */
#if (defined FRNM_SYNCH_POINT_ENABLED)
#error FRNM_SYNCH_POINT_ENABLED already defined
#endif
#define FRNM_SYNCH_POINT_ENABLED       0x20U
/* Bit mask for vote inhibition bit */
#if (defined FRNM_VOTE_INHIBITION_ENABLED)
#error FRNM_VOTE_INHIBITION_ENABLED already defined
#endif
#define FRNM_VOTE_INHIBITION_ENABLED   0x40U
/* Bit mask for data bit */
#if (defined FRNM_NM_DATA_ENABLED)
#error FRNM_NM_DATA_ENABLED already defined
#endif
#define FRNM_NM_DATA_ENABLED           0x80U

/*Rx Pdu types*/
#if (defined FRNM_VOTE)
#error FRNM_VOTE already defined
#endif
#define FRNM_VOTE  0x00U

#if (defined FRNM_DATA)
#error FRNM_DATA already defined
#endif
#define FRNM_DATA  0x01U

#if (defined FRNM_MIXED)
#error FRNM_MIXED already defined
#endif
#define FRNM_MIXED 0x02U

/* Flex Ray schedule cycles used to define voting cycle or data cycle.*/
#if (defined FRNM_CYCLE_VALUE_1)
#error FRNM_CYCLE_VALUE_1 already defined
#endif
#define FRNM_CYCLE_VALUE_1  1U

#if (defined FRNM_CYCLE_VALUE_2)
#error FRNM_CYCLE_VALUE_2 already defined
#endif
#define FRNM_CYCLE_VALUE_2  2U

#if (defined FRNM_CYCLE_VALUE_4)
#error FRNM_CYCLE_VALUE_4 already defined
#endif
#define FRNM_CYCLE_VALUE_4  4U

#if (defined FRNM_CYCLE_VALUE_8)
#error FRNM_CYCLE_VALUE_8 already defined
#endif
#define FRNM_CYCLE_VALUE_8  8U

#if (defined FRNM_CYCLE_VALUE_16)
#error FRNM_CYCLE_VALUE_16 already defined
#endif
#define FRNM_CYCLE_VALUE_16 16U

#if (defined FRNM_CYCLE_VALUE_32)
#error FRNM_CYCLE_VALUE_32 already defined
#endif
#define FRNM_CYCLE_VALUE_32 32U

#if (defined FRNM_CYCLE_VALUE_64)
#error FRNM_CYCLE_VALUE_64 already defined
#endif
#define FRNM_CYCLE_VALUE_64 64U

/* Value for repeat message bit in the Pdu byte 0 */
#if (defined FRNM_PDU_REPEAT_MSG_BIT)
#error FRNM_PDU_REPEAT_MSG_BIT already defined
#endif
#define FRNM_PDU_REPEAT_MSG_BIT 0x00U

/* Value for vote bit in the Pdu byte 0 */
#if (defined FRNM_PDU_VOTE_BIT)
#error FRNM_PDU_VOTE_BIT already defined
#endif
#define FRNM_PDU_VOTE_BIT 0x07U

/** \brief Macro to create a bitmask with single bit set at specified position
 ** (BitPos)
 **
 ** \param[in] BitPos - Bit position */
#if (defined FRNM_BITPOS2MASK)
#error FRNM_BITPOS2MASK already defined
#endif
#define FRNM_BITPOS2MASK(BitPos) ((uint8)(1U << (BitPos)))


/* Internal State */
/* The numbers describe bit positions */
#if (defined FRNM_NETWORK_REQUEST_BIT)
#error FRNM_NETWORK_REQUEST_BIT already defined
#endif
#define FRNM_NETWORK_REQUEST_BIT           1U

#if (defined FRNM_NETWORK_PASSIVE_START_BIT)
#error FRNM_NETWORK_PASSIVE_START_BIT already defined
#endif
#define FRNM_NETWORK_PASSIVE_START_BIT     2U

#if (defined FRNM_STARTUP_ERROR_BIT)
#error FRNM_STARTUP_ERROR_BIT already defined
#endif
#define FRNM_STARTUP_ERROR_BIT             3U

#if (defined FRNM_REPEAT_MSG_REQUEST_BIT)
#error FRNM_REPEAT_MSG_REQUEST_BIT already defined
#endif
#define FRNM_REPEAT_MSG_REQUEST_BIT        4U

#if (defined FRNM_UNI_TIMEOUT_BIT)
#error FRNM_UNI_TIMEOUT_BIT already defined
#endif
#define FRNM_UNI_TIMEOUT_BIT               5U

#if (defined FRNM_REMOTE_SLEEP_IND_BIT)
#error FRNM_REMOTE_SLEEP_IND_BIT already defined
#endif
#define FRNM_REMOTE_SLEEP_IND_BIT          6U

#if (defined FRNM_NODE_DETECTION_LOCK_BIT)
#error FRNM_NODE_DETECTION_LOCK_BIT already defined
#endif
#define FRNM_NODE_DETECTION_LOCK_BIT       7U


/** \brief Bitmasks for internal states upated in ChanStatus */
#if (defined FRNM_NETWORK_REQUESTED)
#error FRNM_NETWORK_REQUESTED already defined
#endif
#define FRNM_NETWORK_REQUESTED            \
   FRNM_BITPOS2MASK(FRNM_NETWORK_REQUEST_BIT)

#if (defined FRNM_NETWORK_PASSIVE_START)
#error FRNM_NETWORK_PASSIVE_START already defined
#endif
#define FRNM_NETWORK_PASSIVE_START            \
   FRNM_BITPOS2MASK(FRNM_NETWORK_PASSIVE_START_BIT)

#if (defined FRNM_NETWORK_STARTED)
#error FRNM_NETWORK_STARTED already defined
#endif
#define FRNM_NETWORK_STARTED  ((uint8)(FRNM_NETWORK_REQUESTED | FRNM_NETWORK_PASSIVE_START))

#if (defined FRNM_STARTUP_ERROR)
#error FRNM_STARTUP_ERROR already defined
#endif
#define FRNM_STARTUP_ERROR            \
   FRNM_BITPOS2MASK(FRNM_STARTUP_ERROR_BIT)

#if (defined FRNM_PDU_REPEAT_MSG_REQUESTED)
#error FRNM_PDU_REPEAT_MSG_REQUESTED already defined
#endif
#define FRNM_PDU_REPEAT_MSG_REQUESTED            \
   FRNM_BITPOS2MASK(FRNM_PDU_REPEAT_MSG_BIT)

#if (defined FRNM_REPEAT_MSG_REQUESTED)
#error FRNM_REPEAT_MSG_REQUESTED already defined
#endif
#define FRNM_REPEAT_MSG_REQUESTED            \
   FRNM_BITPOS2MASK(FRNM_REPEAT_MSG_REQUEST_BIT)

#if (defined FRNM_UNI_TIMEOUT_PASSED)
#error FRNM_UNI_TIMEOUT_PASSED already defined
#endif
#define FRNM_UNI_TIMEOUT_PASSED            \
   FRNM_BITPOS2MASK(FRNM_UNI_TIMEOUT_BIT)

#if (defined FRNM_REMOTE_SLEEP_INDICATION)
#error FRNM_REMOTE_SLEEP_INDICATION already defined
#endif
#define FRNM_REMOTE_SLEEP_INDICATION            \
   FRNM_BITPOS2MASK(FRNM_REMOTE_SLEEP_IND_BIT)

#if (defined FRNM_NODE_DETECTION_LOCK)
#error FRNM_NODE_DETECTION_LOCK already defined
#endif
#define FRNM_NODE_DETECTION_LOCK            \
   FRNM_BITPOS2MASK(FRNM_NODE_DETECTION_LOCK_BIT)

#if (defined FRNM_CBV_SLEEPREADYBIT_MASK)
#error FRNM_CBV_SLEEPREADYBIT_MASK already defined
#endif
#define FRNM_CBV_SLEEPREADYBIT_MASK     \
   FRNM_BITPOS2MASK(FRNM_CBV_SLEEPREADYBIT)

#if (defined FRNM_CBV_ACTIVEWAKEUPBIT_MASK)
#error FRNM_CBV_ACTIVEWAKEUPBIT_MASK already defined
#endif
#define FRNM_CBV_ACTIVEWAKEUPBIT_MASK     \
  FRNM_BITPOS2MASK(FRNM_CBV_ACTIVEWAKEUPBIT)

#if (defined FRNM_PDU_VOTE_MASK)
#error FRNM_PDU_VOTE_MASK already defined
#endif
#define FRNM_PDU_VOTE_MASK     \
   FRNM_BITPOS2MASK(FRNM_PDU_VOTE_BIT)

/* Defines for initialisation status */
#if (defined FRNM_UNINIT)
#error FRNM_UNINIT already defined
#endif
#define FRNM_UNINIT 0U

#if (defined FRNM_INIT)
#error FRNM_INIT already defined
#endif
#define FRNM_INIT   1U

#if (defined FRNM_SOURCE_NODE_IDENTIFIER_POS)
#error FRNM_SOURCE_NODE_IDENTIFIER_POS already defined
#endif
#define FRNM_SOURCE_NODE_IDENTIFIER_POS  1U


#if (defined FRNM_CHANNEL_STATUS)
#error FRNM_CHANNEL_STATUS already defined
#endif

#if (defined FRNM_CHANNEL_CONFIG)
#error FRNM_CHANNEL_CONFIG already defined
#endif

#if (defined FRNM_CHANNELTIME_CONFIG)
#error FRNM_CHANNELTIME_CONFIG already defined
#endif

#if (defined FRNM_CHANNELID_CONFIG)
#error FRNM_CHANNELID_CONFIG already defined
#endif

#if (defined FRNM_CHANNEL_PNENABLED)
#error FRNM_CHANNEL_PNENABLED already defined
#endif

#if (defined FRNM_CHANNEL_NODEID)
#error FRNM_CHANNEL_NODEID already defined
#endif

#if (defined FRNM_CHANNEL_MSGTIMEOUTTIME)
#error FRNM_CHANNEL_MSGTIMEOUTTIME already defined
#endif

#if (defined FRNM_CHANNEL_COMUSERTXCONFPDUID)
#error FRNM_CHANNEL_COMUSERTXCONFPDUID already defined
#endif

#if (defined FRNM_CHANNEL_USERRXPDUID)
#error FRNM_CHANNEL_USERRXPDUID is already defined
#endif

#if (defined FRNM_CHANNEL_USERDATAPDULENGTH)
#error FRNM_CHANNEL_USERDATAPDULENGTH already defined
#endif

#if (defined FRNM_CHANNEL_PDULENGTH)
#error FRNM_CHANNEL_PDULENGTH already defined
#endif

#if (defined FRNM_CHANNEL_NOOFTXVOTEPDUS)
#error FRNM_CHANNEL_NOOFTXVOTEPDUS already defined
#endif

#if (defined FRNM_CHANNEL_NOOFTXDATAPDUS)
#error FRNM_CHANNEL_NOOFTXDATAPDUS already defined
#endif

#if (defined FRNM_CHANNEL_TXVOTEPDUID)
#error FRNM_CHANNEL_TXVOTEPDUID already defined
#endif

#if (defined FRNM_CHANNEL_TXDATAPDUID)
#error FRNM_CHANNEL_TXDATAPDUID already defined
#endif

#if (defined FRNM_CHANNEL_PROPERTY)
#error FRNM_CHANNEL_PROPERTY already defined
#endif

#if (defined FRNM_CHANNEL_CTRLIDX)
#error FRNM_CHANNEL_CTRLIDX already defined
#endif

#if (defined FRNM_COM_USER_DATA_INFO)
#error FRNM_COM_USER_DATA_INFO already defined
#endif

#if (defined FRNM_RESERVED_BYTES)
#error FRNM_RESERVED_BYTES already defined
#endif

#if (defined FRNM_CHANNEL_BUFFER_OFFSET)
#error FRNM_CHANNEL_BUFFER_OFFSET already defined
#endif

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define FRNM_CHANNEL_STATUS(a)              FrNm_ChanStatus[(a)]
#define FRNM_CHANNEL_CONFIG(a)              FrNm_ChanConfig[(a)]
#define FRNM_CHANNELTIME_CONFIG(a)          FrNm_ChanConfig[(a)].ChannelTiming
#define FRNM_CHANNELID_CONFIG(a)            FrNm_ChanConfig[(a)].ChannelIdentifiers
#define FRNM_CHANNEL_MSGTIMEOUTTIME(a)      (FrNm_RootPtr->FrNm_ChannelData[(a)].MsgTimeoutTime)
#define FRNM_CHANNEL_TXVOTEPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(a)].TxVotePduId)
#define FRNM_CHANNEL_TXDATAPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(a)].TxDataPduId)
#define FRNM_CHANNEL_COMUSERTXCONFPDUID(a)  (FrNm_RootPtr->FrNm_ChannelData[(a)].ComUserTxConfPduId)
#define FRNM_CHANNEL_USERRXPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(a)].ComUserRxPduId)
#define FRNM_CHANNEL_USERDATAPDULENGTH(a)   (FrNm_RootPtr->FrNm_ChannelData[(a)].UserDataPduLength)
#define FRNM_CHANNEL_PDULENGTH(a)           (FrNm_RootPtr->FrNm_ChannelData[(a)].PduLength)
#define FRNM_CHANNEL_NODEID(a)              (FrNm_RootPtr->FrNm_ChannelData[(a)].NodeId)
#define FRNM_CHANNEL_NOOFTXVOTEPDUS(a)      (FrNm_RootPtr->FrNm_ChannelData[(a)].NoOfTxVotePdus)
#define FRNM_CHANNEL_NOOFTXDATAPDUS(a)      (FrNm_RootPtr->FrNm_ChannelData[(a)].NoOfTxDataPdus)
#define FRNM_CHANNEL_PROPERTY(a)            (FrNm_RootPtr->FrNm_ChannelData[(a)].ChannelProperty)
#define FRNM_CHANNEL_CTRLIDX(a)             (FrNm_RootPtr->FrNm_ChannelData[(a)].CtrlIdx)
#define FRNM_CHANNEL_PNENABLED(a)           (FrNm_RootPtr->FrNm_ChannelData[(a)].PnEnabled)
#define FRNM_COM_USER_DATA_INFO(a)          (FrNm_ComUserDataInfo[(a)])
#define FRNM_RESERVED_BYTES(a)              (FRNM_CHANNELID_CONFIG(a).FrNmReservedBytes)
#define FRNM_CHANNEL_BUFFER_OFFSET(a)       (FrNm_RootPtr->BufferOffsets[(a)])
#else
#define FRNM_CHANNEL_STATUS(a)              FrNm_ChanStatus[0]
#define FRNM_CHANNEL_CONFIG(a)              FrNm_ChanConfig[0]
#define FRNM_CHANNELTIME_CONFIG(a)          FrNm_ChanConfig[0].ChannelTiming
#define FRNM_CHANNELID_CONFIG(a)            FrNm_ChanConfig[0].ChannelIdentifiers
#define FRNM_CHANNEL_MSGTIMEOUTTIME(a)      (FrNm_RootPtr->FrNm_ChannelData[(0)].MsgTimeoutTime)
#define FRNM_CHANNEL_TXVOTEPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(0)].TxVotePduId)
#define FRNM_CHANNEL_TXDATAPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(0)].TxDataPduId)
#define FRNM_CHANNEL_COMUSERTXCONFPDUID(a)  (FrNm_RootPtr->FrNm_ChannelData[(0)].ComUserTxConfPduId)
#define FRNM_CHANNEL_USERRXPDUID(a)         (FrNm_RootPtr->FrNm_ChannelData[(0)].ComUserRxPduId)
#define FRNM_CHANNEL_USERDATAPDULENGTH(a)   (FrNm_RootPtr->FrNm_ChannelData[(0)].UserDataPduLength)
#define FRNM_CHANNEL_PDULENGTH(a)           (FrNm_RootPtr->FrNm_ChannelData[(0)].PduLength)
#define FRNM_CHANNEL_NODEID(a)              (FrNm_RootPtr->FrNm_ChannelData[(0)].NodeId)
#define FRNM_CHANNEL_NOOFTXVOTEPDUS(a)      (FrNm_RootPtr->FrNm_ChannelData[(0)].NoOfTxVotePdus)
#define FRNM_CHANNEL_NOOFTXDATAPDUS(a)      (FrNm_RootPtr->FrNm_ChannelData[(0)].NoOfTxDataPdus)
#define FRNM_CHANNEL_PROPERTY(a)            (FrNm_RootPtr->FrNm_ChannelData[(0)].ChannelProperty)
#define FRNM_CHANNEL_CTRLIDX(a)             (FrNm_RootPtr->FrNm_ChannelData[(0)].CtrlIdx)
#define FRNM_CHANNEL_PNENABLED(a)           (FrNm_RootPtr->FrNm_ChannelData[(0)].PnEnabled)
#define FRNM_COM_USER_DATA_INFO(a)          (FrNm_ComUserDataInfo[(0)])
#define FRNM_RESERVED_BYTES(a)              (FRNM_CHANNELID_CONFIG(0).FrNmReservedBytes)
#define FRNM_CHANNEL_BUFFER_OFFSET(a)       (FrNm_RootPtr->BufferOffsets[(0)])
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_NetworkStartIndication,1 */
#if (defined FRNM_CALL_NM_NETWORKSTARTINDICATION)
#error FRNM_CALL_NM_NETWORKSTARTINDICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_NetworkMode,1 */
#if (defined FRNM_CALL_NM_NETWORKMODE)
#error FRNM_CALL_NM_NETWORKMODE is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_BusSleepMode,1 */
#if (defined FRNM_CALL_NM_BUSSLEEPMODE)
#error FRNM_CALL_NM_BUSSLEEPMODE is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_RemoteSleepIndication,1 */
#if (defined FRNM_CALL_NM_REMOTESLEEPINDICATION)
#error FRNM_CALL_NM_REMOTESLEEPINDICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_RemoteSleepCancellation,1 */
#if (defined FRNM_CALL_NM_REMOTESLEEPCANCELLATION)
#error FRNM_CALL_NM_REMOTESLEEPCANCELLATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_SynchronizationPoint,1 */
#if (defined FRNM_CALL_NM_SYNCHRONIZATIONPOINT)
#error FRNM_CALL_NM_SYNCHRONIZATIONPOINT is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_CoordReadyToSleepIndication,1 */
#if (defined FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION)
#error FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_PduRxIndication,1 */
#if (defined FRNM_CALL_NM_PDURXINDICATION)
#error FRNM_CALL_NM_PDURXINDICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_StateChangeNotification,1 */
#if (defined FRNM_CALL_NM_STATECHANGENOTIFICATION)
#error FRNM_CALL_NM_STATECHANGENOTIFICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_CarWakeUpIndication,1 */
#if (defined FRNM_CALL_NM_CARWAKEUPINDICATION)
#error FRNM_CALL_NM_CARWAKEUPINDICATION is already defined
#endif

/* !LINKSTO FrNm.EB.SchM_Call_Nm_TxTimeoutException,1 */
#if (defined FRNM_CALL_NM_TXTIMEOUTEXCEPTION)
#error FRNM_CALL_NM_TXTIMEOUTEXCEPTION is already defined
#endif

#if (FRNM_MULTICORE_ENABLED == STD_ON)
/** \brief SchM call for Nm_NetworkStartIndication */
#define FRNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).NetworkStartIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_NetworkMode */
#define FRNM_CALL_NM_NETWORKMODE(idx) \
        FRNM_CHANNEL_CONFIG(idx).NetworkMode(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_BusSleepMode */
#define FRNM_CALL_NM_BUSSLEEPMODE(idx) \
        FRNM_CHANNEL_CONFIG(idx).BusSleepMode(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_RemoteSleepIndication */
#define FRNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).RemoteSleepIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_RemoteSleepCancellation */
#define FRNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).RemoteSleepCancellation(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_SynchronizationPoint */
#define FRNM_CALL_NM_SYNCHRONIZATIONPOINT(idx) \
        FRNM_CHANNEL_CONFIG(idx).SynchronizationPoint(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_CoordReadySleepIndication */
#define FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).CoordReadyToSleepIndication(idx)

/** \brief SchM call for Nm_PduRxIndication */
#define FRNM_CALL_NM_PDURXINDICATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).PduRxIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_StateChangeNotification */
#define FRNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
        FRNM_CHANNEL_CONFIG(idx).StateChangeNotification(FRNM_CHANNELID_CONFIG(idx).ChannelId,v0,v1)

/** \brief SchM call for Nm_TxTimeoutException */
#define FRNM_CALL_NM_TXTIMEOUTEXCEPTION(idx) \
        FRNM_CHANNEL_CONFIG(idx).TxTimeoutException(FRNM_CHANNELID_CONFIG(idx).ChannelId)

/** \brief SchM call for Nm_CarWakeupIndication */
#define FRNM_CALL_NM_CARWAKEUPINDICATION(idx) \
        FRNM_CHANNEL_CONFIG(idx).CarWakeUpIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#else /* (FRNM_MULTICORE_ENABLED == STD_OFF) */

#define FRNM_CALL_NM_NETWORKSTARTINDICATION(idx) \
        Nm_NetworkStartIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_NETWORKMODE(idx) \
        Nm_NetworkMode(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_BUSSLEEPMODE(idx) \
        Nm_BusSleepMode(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_REMOTESLEEPINDICATION(idx) \
        Nm_RemoteSleepIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_REMOTESLEEPCANCELLATION(idx) \
        Nm_RemoteSleepCancellation(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_SYNCHRONIZATIONPOINT(idx) \
        Nm_SynchronizationPoint(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION(idx) \
        Nm_CoordReadyToSleepIndication(idx)

#define FRNM_CALL_NM_PDURXINDICATION(idx) \
        Nm_PduRxIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_STATECHANGENOTIFICATION(idx,v0,v1) \
        Nm_StateChangeNotification(FRNM_CHANNELID_CONFIG(idx).ChannelId,v0,v1)

#define FRNM_CALL_NM_TXTIMEOUTEXCEPTION(idx) \
        Nm_TxTimeoutException(FRNM_CHANNELID_CONFIG(idx).ChannelId)

#define FRNM_CALL_NM_CARWAKEUPINDICATION(idx) \
        Nm_CarWakeUpIndication(FRNM_CHANNELID_CONFIG(idx).ChannelId)
#endif

#if (defined FRNM_TX_BUFFER_OFFSET)
#error FRNM_TX_BUFFER_OFFSET already defined
#endif
#define FRNM_TX_BUFFER_OFFSET            (FrNm_RootPtr->TxBufferOffset)

#if (defined FRNM_PB_REQUIRED_CFG_RAM_SIZE)
#error FRNM_PB_REQUIRED_CFG_RAM_SIZE already defined
#endif
#define FRNM_PB_REQUIRED_CFG_RAM_SIZE          (LocalConfigPtr->PbRequiredCfgRamSize)

/* Value for Invalid Index for Eira timer array */
#if (defined FRNM_EIRA_INVALID_INDEX)
#error FRNM_EIRA_INVALID_INDEX already defined
#endif
#define FRNM_EIRA_INVALID_INDEX 0xFFU

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
/** \brief MACRO for DET reporting
 */
#if (defined FRNM_DET_REPORT_ERROR)
#error FRNM_DET_REPORT_ERROR already defined
#endif
#define FRNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)          \
   (void)Det_ReportError(FRNM_MODULE_ID, (InstanceId), (ServiceId), (ErrorCode))
#else
#define FRNM_DET_REPORT_ERROR(InstanceId, ServiceId, ErrorCode)
#endif

#if (defined FRNM_NETWORK_GW_ERA_REQUEST_AWB)
#error FRNM_NETWORK_GW_ERA_REQUEST_AWB already defined
#endif /* if (defined FRNM_NETWORK_GW_ERA_REQUEST_AWB) */
#define FRNM_NETWORK_GW_ERA_REQUEST_AWB                   3U

#if (defined FRNM_PASSIVE_STARTUP_AWB)
#error FRNM_PASSIVE_STARTUP_AWB already defined
#endif /* if (defined FRNM_PASSIVE_STARTUP_AWB) */
#define FRNM_PASSIVE_STARTUP_AWB                          2U

#if (defined FRNM_NETWORK_REQUEST_AWB)
#error FRNM_NETWORK_REQUEST_AWB already defined
#endif /* if (defined FRNM_NETWORK_REQUEST_AWB) */
#define FRNM_NETWORK_REQUEST_AWB                          1U

/*==================[type definitions]=======================================*/

/**
 ** \brief Type to allow declaration of a do-action function pointer.
 **
 ** Replace HSM do-action. This type allow declaration of a function
 ** pointer which can be used to register a function cyclic execution
 ** in a state_entry and stop this cyclic execution by set to NULL on
 ** state_exit.
 **
 ** \param instIdx HSM instance number
 ** \retval void
 **/
typedef void (*FrNm_DoActionType)(uint8 instIdx);

#if (FRNM_MULTICORE_ENABLED == STD_ON)
/** \brief SchM call function pointer which accepts a single parameter */
typedef Std_ReturnType (*FrNm_Nm_CallsFctPtrType1)(uint8);

/** \brief SchM call function pointer which accepts two parameters */
typedef Std_ReturnType (*FrNm_Nm_CallsFctPtrType2)(uint8,uint8,uint8);
#endif

/**
 * \brief FrNm identifier information for one particular FrNm cluster
 */
typedef struct
{
#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
  PduIdType EraPduId;
  uint8 EraCalcEnabled;
#endif
#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
  uint8   FrNmCarWakeUpBitPosition;
  uint8   FrNmCarWakeUpBytePosition;
  uint8   FrNmCarWakeUpFilterNodeId;
#endif
  NetworkHandleType ChannelId; /**< NM-Network ID configured for the respective FlexRay Channel */
  uint8   FrNmReservedBytes;
  boolean FrNmSourceNodeIdentifierEnabled;
  boolean FrNmNodeDetectionEnabled;
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  boolean FrNmActiveWakeupBitEnabled;
#endif
#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
  boolean FrNmCarWakeUpFilterEnabled;
  boolean FrNmCarWakeUpRxEnabled;
#endif
} FrNm_ChnIdentifierConfigType;

/**
 * \brief FrNm timing information for one particular FrNm cluster
 */
typedef struct
{
  /* !LINKSTO FRNM309,1 */
  uint16 ReadySleepCnt;      /**< Number of repetitions in the ready sleep state before switches
                                  into bus sleep state [1..65535] */
  uint16 RemoteSleepIndTime; /**< Number of communication cycles that have to pass with all
                                  participating ECUs signalling ready to sleep, till an indication
                                  is sent to the upper layer [0..65535] */
  uint16 RepeatMessageTime;  /**< Number of communication cycles the state machine reside in repeat
                                  message state [0..65535] */
  uint8 DataCycle;           /**< Number of FlexRay communication cycles required to transmit the
                                  data PDU[ 1, 2, 4, 8, 16, 32, 64] */
  uint8 RepetitionCycle;     /**< Number of FlexRay communication cycles required to change the
                                  vote PDU [ 1, 2, 4, 8, 16, 32, 64] */
  uint8 VotingCycle;         /**< Number of FlexRay communication cycles required to transmit the
                                  vote PDU[ 1, 2, 4, 8, 16, 32, 64] */
} FrNm_ChnTimingConfigType;

/**
 * \brief FrNm configuration for one particular FrNm cluster
 */
typedef struct
{
#if (FRNM_MULTICORE_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType1 NetworkStartIndication;
  FrNm_Nm_CallsFctPtrType1 NetworkMode;
  FrNm_Nm_CallsFctPtrType1 BusSleepMode;
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType1 RemoteSleepIndication;
  FrNm_Nm_CallsFctPtrType1 RemoteSleepCancellation;
#endif
#if (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
  FrNm_Nm_CallsFctPtrType1 CoordReadyToSleepIndication;
#endif
#if (FRNM_SYNCHRONIZATIONPOINT_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType1 SynchronizationPoint;
#endif
#if (FRNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType1 PduRxIndication;
#endif
#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType2 StateChangeNotification;
#endif
  FrNm_Nm_CallsFctPtrType1 TxTimeoutException;
#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
  FrNm_Nm_CallsFctPtrType1 CarWakeUpIndication;
#endif
#endif
  FrNm_ChnIdentifierConfigType ChannelIdentifiers;      /**< cluster specific identifiers */
  FrNm_ChnTimingConfigType ChannelTiming;               /**< cluster specific timing information */
  boolean AllNmMessagesKeepAwake;
} FrNm_ChnConfigType;


/** \brief Definition of the FrNm_TimeType */
typedef uint16 FrNm_TimeType;

/** \brief Definition of the FrNm_NodeIdType */
typedef uint8 FrNm_NodeIdType;

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
/** \brief Definition of FrNm_PnStatusType */
typedef struct
{
  /* !LINKSTO FRNM435,1 */
  FrNm_TimeType     PnTimer[FRNM_EIRA_MAX_TIMER_SIZE];
  uint8             PnValue[FRNM_PN_INFO_LENGTH];
  uint8             PnInfo[FRNM_PN_INFO_LENGTH];
}FrNm_PnStatusType;
#endif

/** \brief Type for the channel state
 **
 ** This type stores the dynamic data of a FrNm channel; e.g. timers
 ** and states. */
typedef struct
{
  P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) RxPduPtr; /**< cluster specific Pdu receive buffer */
  P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) TxPduPtr; /**< cluster specific Pdu transmit buffer */
#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
  /* !LINKSTO FRNM430,1 */
  FrNm_PnStatusType             PnEra;
#endif
  FrNm_TimeType    UniversalTimer;  /**< Timer for repeat message time and remote sleep time */
  FrNm_TimeType    TimeoutTimer;    /**< Timer for detecting transmisson timeout */
  /* !LINKSTO FRNM309,1 */
  FrNm_TimeType    ReadySleepTimer; /**< Timer for ready sleep time */
  Nm_StateType     CurState;        /**< Current state of a cluster */
  uint8            ActCycle;        /**< Current FlexRay cycle read from FrIf */
  uint8            ChanStatus;      /**< See  bitmasks for internal States.
                                         This variable is not protected against concurrent access
                                         and must only be accessed from within the main function
                                         context */
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
  uint8            FrNm_EmulatedCounter;
#endif
  uint8            ControlBitVector;
  boolean          ComEnabled;      /**< Store communication control request from upper layer.
                                         This variable is accessed concurrently between
                                         FrNm_Mainfunction context and other API's.
                                         Set to FALSE in FrNm_Mainfunction context when bus
                                         synchronization is lost. In this case, FrNm will go to
                                         either Bus Sleep State or Synchronous State. But set back
                                         to TRUE while entering 'NetworkMode'.
                                         Disable or Enable API's can set this variable only if
                                         FrNm is already in 'NetworkMode'. We do not expect
                                         synchronization error when transmission is disabled.
                                         Therefore, protection against concurrent access of this
                                         variable is not required.*/
  boolean FrNm_SyncLoss;
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
  boolean FrNm_EmulatedCounterValid;
#endif
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  uint8 ActiveWakeUp1;
  uint8 ActiveWakeUp2;
  boolean FrNm_SetActiveWakeUpFlag;
#endif
}FrNm_ChanStatusType;

/** \brief Definition of the FrNm_UserDataConfigType */
#if (FRNM_COM_USER_DATA_SUPPORT ==STD_ON)
typedef struct
{
  PduIdType      ComUserTxPduId;      /**< Tx PDU for Com user data defined
                                           by EcuC */
  boolean        ComUserTxPduEnabled; /**< Flag indicates whether Tx PDU
                                           for Com user data configured */
  boolean        ComUserRxPduEnabled; /**< Flag indicates whether Rx PDU
                                           for Com user data configured */
}FrNm_UserDataConfigType;
#endif

/*==================[internal function declarations]=========================*/
/*==================[internal constants]=====================================*/
/*==================[external data]==========================================*/

#define FRNM_START_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

/** \brief Intialization information of FrNm module */
extern VAR(uint8, FRNM_VAR) FrNm_InitStatus;

#define FRNM_STOP_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

#define FRNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
extern VAR(FrNm_PnStatusType, FRNM_VAR) FrNm_PnEiraStatus;
#endif

/** \brief Function pointer to routine schedualed from channel main function */
extern VAR(FrNm_DoActionType, FRNM_VAR) FrNm_DoActFkp[FRNM_NUMBER_OF_CHANNELS];

/** \brief Declaration of the extern variable FrNm_ChanStatus */
extern VAR(FrNm_ChanStatusType, FRNM_VAR)
   FrNm_ChanStatus[FRNM_NUMBER_OF_CHANNELS];\

extern P2CONST(FrNm_ConfigType,FRNM_VAR,FRNM_APPL_DATA) FrNm_RootPtr;

#define FRNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[external constants]=====================================*/
#define FRNM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrNm_MemMap.h>

#if (FRNM_COM_USER_DATA_SUPPORT ==STD_ON)
extern CONST(FrNm_UserDataConfigType, FRNM_CONST) FrNm_ComUserDataInfo[FRNM_NUMBER_OF_CHANNELS];
#endif

/** \brief Configuration for FrNm clusters
*/
extern CONST(FrNm_ChnConfigType, FRNM_CONST) FrNm_ChanConfig[FRNM_NUMBER_OF_CHANNELS];

#define FRNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[external function declarations]=========================*/
#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>
extern FUNC(void, FRNM_CODE) FrNm_MainFunction
(
  uint8 instIdx
);

/** \brief Handle transmission of Nm vote with or without Nm data.
 ** \param instIdx Index to the channel clusters.
 */
FUNC(void, FRNM_CODE) FrNm_HandleVoteCycle
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
);
/** \brief Handle transmission of Nm data.
 ** \param instIdx Index to the channel clusters.
 */
FUNC(void, FRNM_CODE) FrNm_HandleDataCycle
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
);

/** \brief Handle start up error and global time error.
 ** \param instIdx Index to the channel clusters.
 ** \param longBussOff FrNm_StartupError has been called.
 */
FUNC(void, FRNM_CODE) FrNm_HandleSynchError
(
  uint8 instIdx,
  boolean longBussOff
);

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON)

/** \brief Process the EIRA/ERA timers
 **
 ** This function decrement the timer for EIRA/ERA
 ** If the timer expires the change is communicate to PduR via the PduR_FrNmRxIndication callback
 **
 ** \param[in] pPnStatus Pointer to EIRA/ERA structure
 ** \param[in] PduId Id of the PDU used in PduR_FrNmRxIndication callback
 */
FUNC(void, FRNM_CODE) FrNm_HandlePnTimers
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
);

#endif

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef FRNM_INT_H */

/*==================[end of file]============================================*/
