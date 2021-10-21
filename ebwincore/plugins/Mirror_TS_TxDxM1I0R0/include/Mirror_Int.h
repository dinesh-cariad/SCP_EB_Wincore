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

#ifndef MIRROR_INTERNAL_H
#define MIRROR_INTERNAL_H

/*==================[inclusions]============================================*/

#include <Mirror_DefProg_Cfg.h>

#if(MIRROR_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                  /* Module public API */
#endif /* MIRROR_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/*-----------------------[Mirror Invalid Id]---------------------------------------*/

#if(defined MIRROR_INVALID_MIRROR_ID)
#error MIRROR_INVALID_MIRROR_ID already defined!
#endif /* #if(defined MIRROR_INVALID_MIRROR_ID)*/
#define MIRROR_INVALID_MIRROR_ID 0xFF

/*------------------[Mirror Module Status macros]----------------------------------*/
#if(defined MIRROR_NOT_INITIALIZED)
#error MIRROR_NOT_INITIALIZED already defined!
#endif /* #if(defined MIRROR_NOT_INITIALIZED)*/
#define MIRROR_NOT_INITIALIZED FALSE

#if(defined MIRROR_INITIALIZED)
#error MIRROR_INITIALIZED already defined!
#endif /* #if(defined MIRROR_INITIALIZED)*/
#define MIRROR_INITIALIZED TRUE

/*------------------[Source networks Status macros]--------------------------------*/
#if(defined MIRROR_SOURCENETWORK_STOPPED)
#error MIRROR_SOURCENETWORK_STOPPED already defined!
#endif /* #if(defined MIRROR_SOURCENETWORK_STOPPED)*/
#define MIRROR_SOURCENETWORK_STOPPED FALSE

#if(defined MIRROR_SOURCENETWORK_STARTED)
#error MIRROR_SOURCENETWORK_STARTED already defined!
#endif /* #if(defined MIRROR_SOURCENETWORK_STARTED)*/
#define MIRROR_SOURCENETWORK_STARTED TRUE

#if(defined MIRROR_SOURCENETWORK_NO_REQUEST)
#error MIRROR_SOURCENETWORK_NO_REQUEST already defined!
#endif /* #if(defined MIRROR_SOURCENETWORK_NO_REQUEST)*/
#define MIRROR_SOURCENETWORK_NO_REQUEST 0x00U

#if(defined MIRROR_SOURCENETWORK_START_REQUESTED)
#error MIRROR_SOURCENETWORK_START_REQUESTED already defined!
#endif /* #if(defined MIRROR_SOURCENETWORK_START_REQUESTED)*/
#define MIRROR_SOURCENETWORK_START_REQUESTED 0x01U

#if(defined MIRROR_SOURCENETWORK_STOP_REQUESTED)
#error MIRROR_SOURCENETWORK_STOP_REQUESTED already defined!
#endif /* #if(defined MIRROR_SOURCENETWORK_STOP_REQUESTED)*/
#define MIRROR_SOURCENETWORK_STOP_REQUESTED 0x02U

/*-------------------[Can Network Types macros]------------------------------------*/
#if(defined MIRROR_CAN_NETWORK)
#error MIRROR_CAN_NETWORK already defined!
#endif /* #if(defined MIRROR_CAN_NETWORK)*/
#define MIRROR_CAN_NETWORK 0x00U

#if(defined MIRROR_CANFD_NETWORK)
#error MIRROR_CANFD_NETWORK already defined!
#endif /* #if(defined MIRROR_CANFD_NETWORK)*/
#define MIRROR_CANFD_NETWORK 0x01U

/*-------------------[Data element status macros]----------------------------------*/
#if(defined MIRROR_ELEMENT_AVAILABLE)
#error MIRROR_ELEMENT_AVAILABLE already defined!
#endif /* #if(defined MIRROR_ELEMENT_AVAILABLE)*/
#define MIRROR_ELEMENT_AVAILABLE 0x00U

#if(defined MIRROR_ELEMENT_WRITING_IN_PROGRESS)
#error MIRROR_ELEMENT_WRITING_IN_PROGRESS already defined!
#endif /* #if(defined MIRROR_ELEMENT_WRITING_IN_PROGRESS)*/
#define MIRROR_ELEMENT_WRITING_IN_PROGRESS 0x01U

#if(defined MIRROR_ELEMENT_WRITING_FINISHED)
#error MIRROR_ELEMENT_WRITING_FINISHED already defined!
#endif /* #if(defined MIRROR_ELEMENT_WRITING_FINISHED)*/
#define MIRROR_ELEMENT_WRITING_FINISHED 0x02U

/*-----------------------[Data element Type ]--------------------------------------*/
#if(defined MIRROR_CAN_DATA_ELEMENT)
#error MIRROR_CAN_DATA_ELEMENT already defined!
#endif /* #if(defined MIRROR_CAN_DATA_ELEMENT)*/
#define MIRROR_CAN_DATA_ELEMENT 0x00U

#if(defined MIRROR_CANFD_DATA_ELEMENT)
#error MIRROR_CANFD_DATA_ELEMENT already defined!
#endif /* #if(defined MIRROR_CANFD_DATA_ELEMENT)*/
#define MIRROR_CANFD_DATA_ELEMENT 0x01U

#if(defined MIRROR_LIN_DATA_ELEMENT)
#error MIRROR_LIN_DATA_ELEMENT already defined!
#endif /* #if(defined MIRROR_LIN_DATA_ELEMENT)*/
#define MIRROR_LIN_DATA_ELEMENT 0x02U

#if(defined MIRROR_FLEXRAY_DATA_ELEMENT)
#error MIRROR_FLEXRAY_DATA_ELEMENT already defined!
#endif /* #if(defined MIRROR_FLEXRAY_DATA_ELEMENT)*/
#define MIRROR_FLEXRAY_DATA_ELEMENT 0x03U

/*-------------------[Queue buffer status macros]----------------------------------*/
#if(defined MIRROR_QUEUE_BUFFER_NOT_ACTIVE)
#error MIRROR_QUEUE_BUFFER_NOT_ACTIVE already defined!
#endif /* #if(defined MIRROR_QUEUE_BUFFER_NOT_ACTIVE)*/
#define MIRROR_QUEUE_BUFFER_NOT_ACTIVE 0x00U

#if(defined MIRROR_QUEUE_BUFFER_ACTIVE)
#error MIRROR_QUEUE_BUFFER_ACTIVE already defined!
#endif /* #if(defined MIRROR_QUEUE_BUFFER_ACTIVE)*/
#define MIRROR_QUEUE_BUFFER_ACTIVE 0x01U

#if(defined MIRROR_QUEUE_BUFFER_TRANSMIT_REQUESTED)
#error MIRROR_QUEUE_BUFFER_TRANSMIT_REQUESTED already defined!
#endif /* #if(defined MIRROR_QUEUE_BUFFER_TRANSMIT_REQUESTED)*/
#define MIRROR_QUEUE_BUFFER_TRANSMIT_REQUESTED 0x02U

#if(defined MIRROR_QUEUE_BUFFER_TRANSMITTING)
#error MIRROR_QUEUE_BUFFER_TRANSMITTING already defined!
#endif /* #if(defined MIRROR_QUEUE_BUFFER_TRANSMITTING)*/
#define MIRROR_QUEUE_BUFFER_TRANSMITTING 0x03U

/*-----------------------[Operations macros]---------------------------------------*/
#if(defined MIRROR_CANID_FD_MASK)
#error MIRROR_CANID_FD_MASK already defined!
#endif /* #if(defined MIRROR_CANID_FD_MASK)*/
#define MIRROR_CANID_FD_MASK 0x40000000

#if(defined MIRROR_CHECK_CANID_TYPE)
#error MIRROR_CHECK_CANID_TYPE already defined!
#endif /* #if(defined MIRROR_CHECK_CANID_TYPE)*/
#define MIRROR_CHECK_CANID_TYPE(CanId) (((Can_IdType)(CanId & MIRROR_CANID_FD_MASK)) >> 30U)

#if(defined MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_A(dest) ((dest) |= (0x40U))

#if(defined MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_FRAMEID_CHANNEL_B(dest) ((dest) |= (0x80U))

#if(defined MIRROR_FLEXRAY_SET_FRAMEID_SLOT_VALID)
#error MIRROR_FLEXRAY_SET_FRAMEID_SLOT_VALID already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_FRAMEID_SLOT_VALID)*/
#define MIRROR_FLEXRAY_SET_FRAMEID_SLOT_VALID(dest) ((dest) |= (0x08U))

#if(defined MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_LOWER_VALUE)
#error MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_LOWER_VALUE already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_LOWER_VALUE)*/
#define MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_LOWER_VALUE(dest, slotID) ((dest) = ((uint8)((slotID) & 0x00FFU)))

#if(defined MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_HIGHER_VALUE)
#error MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_HIGHER_VALUE already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_HIGHER_VALUE)*/
#define MIRROR_FLEXRAY_SET_FRAMEID_SOLTID_HIGHER_VALUE(dest, slotID) ((dest) |= ((uint8)(((slotID) & 0x0700U) >> 8U)))

#if(defined MIRROR_DEST_FRAME_DATALENGTH_POSITION)
#error MIRROR_DEST_FRAME_DATALENGTH_POSITION already defined!
#endif /* #if(defined MIRROR_DEST_FRAME_DATALENGTH_POSITION)*/
#define MIRROR_DEST_FRAME_DATALENGTH_POSITION (uint32)(((uint32)Mirror_DestNetworks[Mirror_ActiveDestination].DestNetworkPduLength * (uint32)Mirror_ActiveQueueBuffer) + (uint32)12)

#if(defined MIRROR_SET_NETWORK_STATE_AVAILABLE)
#error MIRROR_SET_NETWORK_STATE_AVAILABLE already defined!
#endif /* #if(defined MIRROR_SET_NETWORK_STATE_AVAILABLE)*/
#define MIRROR_SET_NETWORK_STATE_AVAILABLE(StatusByte) ((StatusByte) |= (0x80U))

#if(defined MIRROR_SET_NETWORK_FRAMEID_AVAILABLE)
#error MIRROR_SET_NETWORK_FRAMEID_AVAILABLE already defined!
#endif /* #if(defined MIRROR_SET_NETWORK_FRAMEID_AVAILABLE)*/
#define MIRROR_SET_NETWORK_FRAMEID_AVAILABLE(StatusByte) ((StatusByte) |= (0x40U))

#if(defined MIRROR_SET_NETWORK_PAYLOAD_AVAILABLE)
#error MIRROR_SET_NETWORK_PAYLOAD_AVAILABLE already defined!
#endif /* #if(defined MIRROR_SET_NETWORK_PAYLOAD_AVAILABLE)*/
#define MIRROR_SET_NETWORK_PAYLOAD_AVAILABLE(StatusByte) ((StatusByte) |= (0x20U))

#if(defined MIRROR_SET_NETWORK_TYPE)
#error MIRROR_SET_NETWORK_TYPE already defined!
#endif /* #if(defined MIRROR_SET_NETWORK_TYPE)*/
#define MIRROR_SET_NETWORK_TYPE(StatusByte, NetworkType) ((StatusByte) |= (uint8)NetworkType)

/*-------------[Source Networks Status Operations macros]--------------------------*/
#if(defined MIRROR_SET_LOST_FRAME_BIT)
#error MIRROR_SET_LOST_FRAME_BIT already defined!
#endif /* #if(defined MIRROR_SET_LOST_FRAME_BIT)*/
#define MIRROR_SET_LOST_FRAME_BIT(StatusByte) ((StatusByte) |= (0x80U))

#if(defined MIRROR_CLEAR_LOST_FRAME_BIT)
#error MIRROR_CLEAR_LOST_FRAME_BIT already defined!
#endif /* #if(defined MIRROR_CLEAR_LOST_FRAME_BIT)*/
#define MIRROR_CLEAR_LOST_FRAME_BIT(StatusByte) ((StatusByte) &= (0x7FU))

#if(defined MIRROR_IS_LOST_FRAME_BIT_SET)
#error MIRROR_IS_LOST_FRAME_BIT_SET already defined!
#endif /* #if(defined MIRROR_IS_LOST_FRAME_BIT_SET)*/
#define MIRROR_IS_LOST_FRAME_BIT_SET(StatusByte) (((StatusByte) & (0x80U)) == 0x80U)

#if(defined MIRROR_SET_BUS_ONLINE_BIT)
#error MIRROR_SET_BUS_ONLINE_BIT already defined!
#endif /* #if(defined MIRROR_SET_BUS_ONLINE_BIT)*/
#define MIRROR_SET_BUS_ONLINE_BIT(StatusByte) ((StatusByte) |= (0x40U))

#if(defined MIRROR_CLEAR_BUS_ONLINE_BIT)
#error MIRROR_CLEAR_BUS_ONLINE_BIT already defined!
#endif /* #if(defined MIRROR_CLEAR_BUS_ONLINE_BIT)*/
#define MIRROR_CLEAR_BUS_ONLINE_BIT(StatusByte) ((StatusByte) &= (0xBFU))

#if(defined MIRROR_CAN_SET_ERROR_PASSIVE_BIT)
#error MIRROR_CAN_SET_ERROR_PASSIVE_BIT already defined!
#endif /* #if(defined MIRROR_CAN_SET_ERROR_PASSIVE_BIT)*/
#define MIRROR_CAN_SET_ERROR_PASSIVE_BIT(StatusByte) ((StatusByte) |= (0x20U))

#if(defined MIRROR_CAN_CLEAR_ERROR_PASSIVE_BIT)
#error MIRROR_CAN_CLEAR_ERROR_PASSIVE_BIT already defined!
#endif /* #if(defined MIRROR_CAN_CLEAR_ERROR_PASSIVE_BIT)*/
#define MIRROR_CAN_CLEAR_ERROR_PASSIVE_BIT(StatusByte) ((StatusByte) &= (0xDFU))

#if(defined MIRROR_CAN_SET_BUS_OFF_BIT)
#error MIRROR_CAN_SET_BUS_OFF_BIT already defined!
#endif /* #if(defined MIRROR_CAN_SET_BUS_OFF_BIT)*/
#define MIRROR_CAN_SET_BUS_OFF_BIT(StatusByte) ((StatusByte) |= (0x10U))

#if(defined MIRROR_CAN_CLEAR_BUS_OFF_BIT)
#error MIRROR_CAN_CLEAR_BUS_OFF_BIT already defined!
#endif /* #if(defined MIRROR_CAN_CLEAR_BUS_OFF_BIT)*/
#define MIRROR_CAN_CLEAR_BUS_OFF_BIT(StatusByte) ((StatusByte) &= (0xEFU))

#if(defined MIRROR_CAN_SET_ERROR_ACTIVE)
#error MIRROR_CAN_SET_ERROR_ACTIVE already defined!
#endif /* #if(defined MIRROR_CAN_SET_ERROR_ACTIVE)*/
#define MIRROR_CAN_SET_ERROR_ACTIVE(StatusByte) ((StatusByte) &= (0xCFU))

#if(defined MIRROR_CAN_TXERROR_COUNTER_CASTING)
#error MIRROR_CAN_TXERROR_COUNTER_CASTING already defined!
#endif /* #if(defined MIRROR_CAN_TXERROR_COUNTER_CASTING)*/
#define MIRROR_CAN_TXERROR_COUNTER_CASTING(ErrorCounter) ((ErrorCounter) = (uint8)((uint8)((ErrorCounter) >> 0x03U) & (uint8)0x0F))

#if(defined MIRROR_CAN_SET_TXERROR_COUNTER)
#error MIRROR_CAN_SET_TXERROR_COUNTER already defined!
#endif /* #if(defined MIRROR_CAN_SET_TXERROR_COUNTER)*/
#define MIRROR_CAN_SET_TXERROR_COUNTER(StatusByte, ErrorCounter) ((StatusByte) |= (ErrorCounter))

#if(defined MIRROR_CAN_CLEAR_TXERROR_COUNTER)
#error MIRROR_CAN_CLEAR_TXERROR_COUNTER already defined!
#endif /* #if(defined MIRROR_CAN_CLEAR_TXERROR_COUNTER)*/
#define MIRROR_CAN_CLEAR_TXERROR_COUNTER(StatusByte) ((StatusByte) &= (0xF0U))

#if(defined MIRROR_CAN_SET_BUS_OFFLINE)
#error MIRROR_CAN_SET_BUS_OFFLINE already defined!
#endif /* #if(defined MIRROR_CAN_SET_BUS_OFFLINE)*/
#define MIRROR_CAN_SET_BUS_OFFLINE(StatusByte) ((StatusByte) &= (0x80U))

#if(defined MIRROR_LIN_STATUS_SET_RX_NO_RESPONSE_BIT)
#error MIRROR_LIN_STATUS_SET_RX_NO_RESPONSE_BIT already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_SET_RX_NO_RESPONSE_BIT)*/
#define MIRROR_LIN_STATUS_SET_RX_NO_RESPONSE_BIT(StatusByte) ((StatusByte) |= (0x01U))

#if(defined MIRROR_LIN_STATUS_SET_RX_ERROR_BIT)
#error MIRROR_LIN_STATUS_SET_RX_ERROR_BIT already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_SET_RX_ERROR_BIT)*/
#define MIRROR_LIN_STATUS_SET_RX_ERROR_BIT(StatusByte) ((StatusByte) |= (0x02U))

#if(defined MIRROR_LIN_STATUS_SET_TX_ERROR_BIT)
#error MIRROR_LIN_STATUS_SET_TX_ERROR_BIT already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_SET_TX_ERROR_BIT)*/
#define MIRROR_LIN_STATUS_SET_TX_ERROR_BIT(StatusByte) ((StatusByte) |= (0x04U))

#if(defined MIRROR_LIN_STATUS_SET_TX_HEADER_ERROR_BIT)
#error MIRROR_LIN_STATUS_SET_TX_HEADER_ERROR_BIT already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_SET_TX_HEADER_ERROR_BIT)*/
#define MIRROR_LIN_STATUS_SET_TX_HEADER_ERROR_BIT(StatusByte) ((StatusByte) |= (0x08U))

#if(defined MIRROR_LIN_STATUS_CLEAR_RX_ERRORS)
#error MIRROR_LIN_STATUS_CLEAR_RX_ERRORS already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_CLEAR_RX_ERRORS)*/
#define MIRROR_LIN_STATUS_CLEAR_RX_ERRORS(StatusByte) ((StatusByte) &= (0xFCU))

#if(defined MIRROR_LIN_STATUS_CLEAR_TX_ERRORS)
#error MIRROR_LIN_STATUS_CLEAR_TX_ERRORS already defined!
#endif /* #if(defined MIRROR_LIN_STATUS_CLEAR_TX_ERRORS)*/
#define MIRROR_LIN_STATUS_CLEAR_TX_ERRORS(StatusByte) ((StatusByte) &= (0xF3U))

#if(defined MIRROR_FLEXRAY_STATUS_SET_BUS_SYNCHRONOUS)
#error MIRROR_FLEXRAY_STATUS_SET_BUS_SYNCHRONOUS already defined!
#endif /* #if(defined MIRROR_FLEXRAY_STATUS_SET_BUS_SYNCHRONOUS)*/
#define MIRROR_FLEXRAY_STATUS_SET_BUS_SYNCHRONOUS(StatusByte) ((StatusByte) |= (0x20U))

#if(defined MIRROR_FLEXRAY_STATUS_CLEAR_BUS_SYNCHRONOUS)
#error MIRROR_FLEXRAY_STATUS_CLEAR_BUS_SYNCHRONOUS already defined!
#endif /* #if(defined MIRROR_FLEXRAY_STATUS_CLEAR_BUS_SYNCHRONOUS)*/
#define MIRROR_FLEXRAY_STATUS_CLEAR_BUS_SYNCHRONOUS(StatusByte) ((StatusByte) &= (0xDFU))

#if(defined MIRROR_FLEXRAY_STATUS_SET_NORMAL_ACTIVE)
#error MIRROR_FLEXRAY_STATUS_SET_NORMAL_ACTIVE already defined!
#endif /* #if(defined MIRROR_FLEXRAY_STATUS_SET_NORMAL_ACTIVE)*/
#define MIRROR_FLEXRAY_STATUS_SET_NORMAL_ACTIVE(StatusByte) ((StatusByte) |= (0x10U))

#if(defined MIRROR_FLEXRAY_STATUS_CLEAR_NORMAL_ACTIVE)
#error MIRROR_FLEXRAY_STATUS_CLEAR_NORMAL_ACTIVE already defined!
#endif /* #if(defined MIRROR_FLEXRAY_STATUS_CLEAR_NORMAL_ACTIVE)*/
#define MIRROR_FLEXRAY_STATUS_CLEAR_NORMAL_ACTIVE(StatusByte) ((StatusByte) &= (0xEFU))

#if(defined MIRROR_FLEXRAY_STATUS_SET_TXCONFLICT)
#error MIRROR_FLEXRAY_STATUS_SET_TXCONFLICT already defined!
#endif /* #if(defined MIRROR_FLEXRAY_STATUS_SET_TXCONFLICT)*/
#define MIRROR_FLEXRAY_STATUS_SET_TXCONFLICT(StatusByte) ((StatusByte) |= (0x01U))

#if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_SYNTAXERROR_SET)
#error MIRROR_FLEXRAY_IS_AGGRIGATED_SYNTAXERROR_SET already defined!
#endif /* #if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_SYNTAXERROR_SET)*/
#define MIRROR_FLEXRAY_IS_AGGRIGATED_SYNTAXERROR_SET(Fr_ChannelStatus) (0U != (Fr_ChannelStatus & 0x02) ? TRUE : FALSE)

#if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_CONTENTERROR_SET)
#error MIRROR_FLEXRAY_IS_AGGRIGATED_CONTENTERROR_SET already defined!
#endif /* #if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_CONTENTERROR_SET)*/
#define MIRROR_FLEXRAY_IS_AGGRIGATED_CONTENTERROR_SET(Fr_ChannelStatus) (0U != (Fr_ChannelStatus & 0x04) ? TRUE : FALSE)

#if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_BVIOLATION_SET)
#error MIRROR_FLEXRAY_IS_AGGRIGATED_BVIOLATION_SET already defined!
#endif /* #if(defined MIRROR_FLEXRAY_IS_AGGRIGATED_BVIOLATION_SET)*/
#define MIRROR_FLEXRAY_IS_AGGRIGATED_BVIOLATION_SET(Fr_ChannelStatus) (0U != (Fr_ChannelStatus & 0x10) ? TRUE : FALSE)

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_A(AggStatusByte) ((AggStatusByte) |= (0x02U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_BVIOLATION_CHANNEL_B(AggStatusByte) ((AggStatusByte) |= (0x20U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_A(AggStatusByte) ((AggStatusByte) |= (0x04U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CONTENTERROR_CHANNEL_B(AggStatusByte) ((AggStatusByte) |= (0x40U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_A(AggStatusByte) ((AggStatusByte) |= (0x08U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_SYNTAXERROR_CHANNEL_B(AggStatusByte) ((AggStatusByte) |= (0x80U))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_A(AggStatusByte) ((AggStatusByte) &= (0x0F))

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_B(AggStatusByte) \
{\
  AggStatusByte &= 0xF0;\
  AggStatusByte = (AggStatusByte >> 4U);\
}

#if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_AB)
#error MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_AB already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_AB)*/
#define MIRROR_FLEXRAY_SET_AGGRIGATED_STATUS_CHANNEL_AB(AggStatusByte) \
{\
  uint8 Temp = (AggStatusByte & 0x0F);\
  ((AggStatusByte) &= (0xF0));\
  ((AggStatusByte) = (AggStatusByte >> 4U));\
  ((AggStatusByte) |= (Temp));\
}

#if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_A)
#error MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_A already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_A)*/
#define MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_A(AggStatusByte, OldAggStatusByte) \
{\
  ((OldAggStatusByte) &= (0xF0)); \
  ((OldAggStatusByte) |= ((AggStatusByte) & 0x0F)); \
}

#if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_B)
#error MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_B already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_B)*/
#define MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_B(AggStatusByte, OldAggStatusByte) \
{\
  ((OldAggStatusByte) &= (0x0F)); \
  ((OldAggStatusByte) |= ((AggStatusByte) & 0xF0)); \
}

#if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_AB)
#error MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_AB already defined!
#endif /* #if(defined MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_AB)*/
#define MIRROR_FLEXRAY_SET_OLD_AGGRIGATED_STATUS_CHANNEL_AB(AggStatusByte, OldAggStatusByte) ((OldAggStatusByte) = (AggStatusByte))

/*-----------------------[Frame Size macros]---------------------------------------*/
#if(defined MIRROR_CAN_ELEMENT_MINIMUM_LENGTH)
#error MIRROR_CAN_ELEMENT_MINIMUM_LENGTH already defined!
#endif /* #if(defined MIRROR_CAN_ELEMENT_MINIMUM_LENGTH)*/
#define MIRROR_CAN_ELEMENT_MINIMUM_LENGTH 0x09U

#if(defined MIRROR_LIN_ELEMENT_MINIMUM_LENGTH)
#error MIRROR_LIN_ELEMENT_MINIMUM_LENGTH already defined!
#endif /* #if(defined MIRROR_LIN_ELEMENT_MINIMUM_LENGTH)*/
#define MIRROR_LIN_ELEMENT_MINIMUM_LENGTH 0x05U

#if(defined MIRROR_FLEXRAY_ELEMENT_MINIMUM_LENGTH)
#error MIRROR_FLEXRAY_ELEMENT_MINIMUM_LENGTH already defined!
#endif /* #if(defined MIRROR_FLEXRAY_ELEMENT_MINIMUM_LENGTH)*/
#define MIRROR_FLEXRAY_ELEMENT_MINIMUM_LENGTH 0x07U

#if(defined MIRROR_STATUS_ELEMENT_LENGTH)
#error MIRROR_STATUS_ELEMENT_LENGTH already defined!
#endif /* #if(defined MIRROR_STATUS_ELEMENT_LENGTH)*/
#define MIRROR_STATUS_ELEMENT_LENGTH 0x05U

#if(defined MIRROR_FLEXRAY_AGGREGATED_STATUS_ELEMENT_LENGTH)
#error MIRROR_FLEXRAY_AGGREGATED_STATUS_ELEMENT_LENGTH already defined!
#endif /* #if(defined MIRROR_FLEXRAY_AGGREGATED_STATUS_ELEMENT_LENGTH)*/
#define MIRROR_FLEXRAY_AGGREGATED_STATUS_ELEMENT_LENGTH 0x08U

#if(defined MIRROR_FRAME_HEADER_SIZE)
#error MIRROR_FRAME_HEADER_SIZE already defined!
#endif /* #if(defined MIRROR_FRAME_HEADER_SIZE)*/
#define MIRROR_FRAME_HEADER_SIZE        0x0EU

/*-----------------------[Time Stamp macros]---------------------------------------*/
#if(defined MIRROR_SECOND_TO_NANOSECONDS)
#error MIRROR_SECOND_TO_NANOSECONDS already defined!
#endif /* #if(defined MIRROR_SECOND_TO_NANOSECONDS)*/
#define MIRROR_SECOND_TO_NANOSECONDS 0x3B9ACA00U

#if(defined MIRROR_10_MICROSECONDS_TO_NANOSECONDS)
#error MIRROR_10_MICROSECONDS_TO_NANOSECONDS already defined!
#endif /* #if(defined MIRROR_10_MICROSECONDS_TO_NANOSECONDS)*/
#define MIRROR_10_MICROSECONDS_TO_NANOSECONDS 0x2710U

#if(defined MIRROR_MAX_TIMESTAMP_DIFFERENCE)
#error MIRROR_MAX_TIMESTAMP_DIFFERENCE already defined!
#endif /* #if(defined MIRROR_MAX_TIMESTAMP_DIFFERENCE)*/
#define MIRROR_MAX_TIMESTAMP_DIFFERENCE 0x270FD8F0U

/*==================[defensive programming macros]=========================*/




/*------------------------[Defensive programming]----------------------------*/

#if (defined MIRROR_PRECONDITION_ASSERT)
#error MIRROR_PRECONDITION_ASSERT is already defined
#endif

#if (defined MIRROR_PRECONDITION_ASSERT_NO_EVAL)
#error MIRROR_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (MIRROR_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))
#else
#define MIRROR_PRECONDITION_ASSERT(Condition, ApiId)
#define MIRROR_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined MIRROR_POSTCONDITION_ASSERT)
#error MIRROR_POSTCONDITION_ASSERT is already defined
#endif

#if (defined MIRROR_POSTCONDITION_ASSERT_NO_EVAL)
#error MIRROR_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (MIRROR_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))
#else
#define MIRROR_POSTCONDITION_ASSERT(Condition, ApiId)
#define MIRROR_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined MIRROR_INVARIANT_ASSERT)
#error MIRROR_INVARIANT_ASSERT is already defined
#endif

#if (defined MIRROR_INVARIANT_ASSERT_NO_EVAL)
#error MIRROR_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (MIRROR_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define MIRROR_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))
#else
#define MIRROR_INVARIANT_ASSERT(Condition, ApiId)
#define MIRROR_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined MIRROR_STATIC_ASSERT)
# error MIRROR_STATIC_ASSERT is already defined
#endif
#if (MIRROR_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define MIRROR_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define MIRROR_STATIC_ASSERT(expr)
#endif

#if (defined MIRROR_UNREACHABLE_CODE_ASSERT)
#error MIRROR_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (MIRROR_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define MIRROR_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId))
#else
#define MIRROR_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined MIRROR_INTERNAL_API_ID)
#error MIRROR_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define MIRROR_INTERNAL_API_ID DET_INTERNAL_API_ID


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef MIRROR_INTERNAL_H */
/*==================[end of file]===========================================*/
