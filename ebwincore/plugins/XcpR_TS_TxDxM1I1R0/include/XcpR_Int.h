#ifndef XCPR_INT_H
#define XCPR_INT_H

/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

#include <ComStack_Types.h>
#include <XcpR_Int_Cfg.h>
#include <TSAtomic_Assign.h>
#include <XcpR_DefProg_Cfg.h>    /* Configuration defines related to defensive programming */

/* !LINKSTO XcpR.dsn.DetInclusion, 2 */
#include <Det.h>                 /* Det API*/

/*==[Macros definitions]======================================================*/





/*------------------------[Defensive programming]----------------------------*/

#if (defined XCPR_PRECONDITION_ASSERT)
#error XCPR_PRECONDITION_ASSERT is already defined
#endif

#if (defined XCPR_PRECONDITION_ASSERT_NO_EVAL)
#error XCPR_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (XCPR_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))
#else
#define XCPR_PRECONDITION_ASSERT(Condition, ApiId)
#define XCPR_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCPR_POSTCONDITION_ASSERT)
#error XCPR_POSTCONDITION_ASSERT is already defined
#endif

#if (defined XCPR_POSTCONDITION_ASSERT_NO_EVAL)
#error XCPR_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (XCPR_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))
#else
#define XCPR_POSTCONDITION_ASSERT(Condition, ApiId)
#define XCPR_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCPR_INVARIANT_ASSERT)
#error XCPR_INVARIANT_ASSERT is already defined
#endif

#if (defined XCPR_INVARIANT_ASSERT_NO_EVAL)
#error XCPR_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (XCPR_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCPR_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))
#else
#define XCPR_INVARIANT_ASSERT(Condition, ApiId)
#define XCPR_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCPR_STATIC_ASSERT)
# error XCPR_STATIC_ASSERT is already defined
#endif
#if (XCPR_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define XCPR_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define XCPR_STATIC_ASSERT(expr)
#endif

#if (defined XCPR_UNREACHABLE_CODE_ASSERT)
#error XCPR_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (XCPR_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define XCPR_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId))
#else
#define XCPR_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined XCPR_INTERNAL_API_ID)
#error XCPR_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define XCPR_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[Internal macros]---------------------------------------*/

#if (XCPR_DEV_ERROR_DETECT == STD_ON)

#if (defined XCPR_DET_REPORT_ERROR)
#error XCPR_DET_REPORT_ERROR already defined
#endif
/** \brief Report XcpR development error. */
#define XCPR_DET_REPORT_ERROR(ApiId,ErrorId)                            \
        ((void)Det_ReportError(XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId), (ErrorId)))

#endif /* if (XCPR_DEV_ERROR_DETECT == STD_ON) */

#if (defined XCPR_DET_REPORT_RUNTIME_ERROR)
#error XCPR_DET_REPORT_RUNTIME_ERROR already defined
#endif
/** \brief Report XcpR runtime error. */
#define XCPR_DET_REPORT_RUNTIME_ERROR(ApiId,ErrorId)                            \
        ((void)Det_ReportRuntimeError(XCPR_MODULE_ID, XCPR_INSTANCE_ID, (ApiId), (ErrorId)))

#if (defined XCPR_STATE_CONNECTED)
#error XCPR_STATE_CONNECTED already defined
#endif
/** \brief Defines connection status of the XCPR module for connected. */
#define XCPR_STATE_CONNECTED                                 0U

#if (defined XCPR_STATE_DISCONNECTED)
#error XCPR_STATE_DISCONNECTED already defined
#endif
/** \brief Defines connection status of the XCPR module for disconnected. */
#define XCPR_STATE_DISCONNECTED                              1U

#if (defined XCPR_STATE_DISCONNECT_REQUESTED)
#error XCPR_STATE_DISCONNECT_REQUESTED already defined
#endif
/** \brief Defines connection status of the XCPR module for autonomous disconnect. */
#define XCPR_STATE_DISCONNECT_REQUESTED                      2U

#if (defined XCPR_MODE_LOCAL_ECU)
#error XCPR_MODE_LOCAL_ECU already defined
#endif
/** \brief Defines connection mode of the XCPR module for the local Xcp slave. */
#define XCPR_MODE_LOCAL_ECU                                 0U

#if (defined XCPR_MODE_REMOTE_ECU)
#error XCPR_MODE_REMOTE_ECU already defined
#endif
/** \brief Defines connection mode of the XCPR module for the remote Xcp slave. */
#define XCPR_MODE_REMOTE_ECU                                1U

#if (defined XCPR_INVALID_CONNECTION_MODE)
#error XCPR_INVALID_CONNECTION_MODE already defined
#endif
/** \brief Invalid connection mode. */
#define XCPR_INVALID_CONNECTION_MODE                       0xFFU

#if (defined XCPR_SRC_PDU_FLAG_SIZE)
#error XCPR_SRC_PDU_FLAG_SIZE already defined
#endif
/** \brief Defines the number of bytes necessary to allocate for the Rx Pdu buffers,
 *         to accommodate the busy status */
#if ((XCPR_NO_OF_SOURCE_PDUS % 8U) == 0U)
#define XCPR_SRC_PDU_FLAG_SIZE  \
  ((XCPR_NO_OF_SOURCE_PDUS) / 8U)
#else
#define XCPR_SRC_PDU_FLAG_SIZE  \
  ((XCPR_NO_OF_SOURCE_PDUS / 8U) + 1U)
#endif

#if (defined XCPR_DEST_PDU_FLAG_SIZE)
#error XCPR_DEST_PDU_FLAG_SIZE already defined
#endif
/** \brief Defines the number of bytes necessary to allocate for the Tx Pdu buffers,
 *         to accommodate the busy status */
#if ((XCPR_NO_OF_DESTINATION_PDUS % 8U) == 0U)
#define XCPR_DEST_PDU_FLAG_SIZE  \
  ((XCPR_NO_OF_DESTINATION_PDUS) / 8U)
#else
#define XCPR_DEST_PDU_FLAG_SIZE  \
  ((XCPR_NO_OF_DESTINATION_PDUS / 8U) + 1U)
#endif

#define XCPR_BYTE_IDX(id) ((uint8)((id)/8U))
#define XCPR_FLAG_IDX(id) ((id)%8U)

#if (defined XCPR_CONNECTION_OVER_CAN)
#error XCPR_CONNECTION_OVER_CAN already defined
#endif
/** \brief The type of connection is over CAN. */
#define XCPR_CONNECTION_OVER_CAN                             0U

#if (defined XCPR_CONNECTION_OVER_FLEXRAY)
#error XCPR_CONNECTION_OVER_FLEXRAY already defined
#endif
/** \brief The type of connection is over FlexRay. */
#define XCPR_CONNECTION_OVER_FLEXRAY                         1U

#if (defined XCPR_CONNECTION_OVER_TCPIP)
#error XCPR_CONNECTION_OVER_TCPIP already defined
#endif
/** \brief The type of connection is over TcpIp. */
#define XCPR_CONNECTION_OVER_TCPIP                           2U

#if (defined XCPR_CONNECTION_OVER_UDPIP)
#error XCPR_CONNECTION_OVER_UDPIP already defined
#endif
/** \brief The type of connection is over UdpIp. */
#define XCPR_CONNECTION_OVER_UDPIP                           3U

#if (defined XCPR_CONNECTION_OVER_CANFD)
#error XCPR_CONNECTION_OVER_CANFD already defined
#endif
/** \brief The type of connection is over CANFD. */
#define XCPR_CONNECTION_OVER_CANFD                           4U

#if (defined XCPR_CONNECTION_OVER_CDD)
#error XCPR_CONNECTION_OVER_CDD already defined
#endif
/** \brief The type of connection is over a CDD. */
#define XCPR_CONNECTION_OVER_CDD                             5U

#if (defined XCPR_RX_PDU_DIRECTION)
#error XCPR_RX_PDU_DIRECTION already defined
#endif
/** \brief Macro defining that the PDU routing direction is Rx. */
#define XCPR_RX_PDU_DIRECTION                               0U

#if (defined XCPR_TX_PDU_DIRECTION)
#error XCPR_TX_PDU_DIRECTION already defined
#endif
/** \brief Macro defining that the PDU routing direction is Tx. */
#define XCPR_TX_PDU_DIRECTION                               1U

#if (defined XCPR_RX_SRC_TYPE)
#error XCPR_RX_SRC_TYPE already defined
#endif
/** \brief Macro defining that the source PDU is Rx. */
#define XCPR_RX_SRC_TYPE                               0U

#if (defined XCPR_TX_SRC_TYPE)
#error XCPR_TX_SRC_TYPE already defined
#endif
/** \brief Macro defining that the source PDU is Tx. */
#define XCPR_TX_SRC_TYPE                               1U

#if (defined XCPR_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX)
#error XCPR_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX already defined
#endif
/** \brief The index in the XcpR frame where the sequence correction byte is. */
#define XCPR_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX            0x1U

#if (defined XCPR_RECEPTION_ON_RX_INDICATION)
#error XCPR_RECEPTION_ON_RX_INDICATION already defined
#endif
/** \brief Macro defining that processing of the reception is done on RxIndication. */
#define XCPR_RECEPTION_ON_RX_INDICATION                     0U

#if (defined XCPR_RECEPTION_ON_MAINFUNCTION)
#error XCPR_RECEPTION_ON_MAINFUNCTION already defined
#endif
/** \brief Macro defining that processing of the reception is done on MainFunction. */
#define XCPR_RECEPTION_ON_MAINFUNCTION                      1U

#if (defined XCPR_TRANSMISSION_ON_TX_CONFIRMATION)
#error XCPR_TRANSMISSION_ON_TX_CONFIRMATION already defined
#endif
/** \brief Macro defining that processing of the transmission is done on TxConfirmation. */
#define XCPR_TRANSMISSION_ON_TX_CONFIRMATION                0U

#if (defined XCPR_TRANSMISSION_ON_MAINFUNCTION)
#error XCPR_TRANSMISSION_ON_MAINFUNCTION already defined
#endif
/** \brief Macro defining that processing of the transmission is done on MainFunction. */
#define XCPR_TRANSMISSION_ON_MAINFUNCTION                   1U

#if (defined XCPR_PROCESSING_OF_RX_DEST_NOT_POSSIBLE)
#error XCPR_PROCESSING_OF_RX_DEST_NOT_POSSIBLE already defined
#endif
/** \brief Macro indicating that the destination does not need Tx confirmation */
#define XCPR_PROCESSING_OF_RX_DEST_NOT_POSSIBLE             0xFFU

#if (defined XCPR_MASTER_SRC_ORIGIN)
#error XCPR_MASTER_SRC_ORIGIN already defined
#endif
/** \brief Macro defining that the source origin is master. */
#define XCPR_MASTER_SRC_ORIGIN             0U

#if (defined XCPR_SLAVE_SRC_ORIGIN)
#error XCPR_SLAVE_SRC_ORIGIN already defined
#endif
/** \brief Macro defining that the source origin is slave. */
#define XCPR_SLAVE_SRC_ORIGIN             1U

#if (defined XCPR_UNKNOWN_SRC_ORIGIN)
#error XCPR_UNKNOWN_SRC_ORIGIN already defined
#endif
/** \brief Macro defining that the source origin is unknown. */
#define XCPR_UNKNOWN_SRC_ORIGIN             0xFFU

#if (defined XCPR_INVALID_SOURCE_TYPE)
#error XCPR_INVALID_SOURCE_TYPE already defined
#endif
/** \brief Macro defining the invalid source type. */
#define XCPR_INVALID_SOURCE_TYPE                   255U

#if (defined XCPR_INVALID_DESTINATION_TYPE)
#error XCPR_INVALID_DESTINATION_TYPE already defined
#endif
/** \brief Macro defining the invalid destination type. */
#define XCPR_INVALID_DESTINATION_TYPE              255U

#if (defined XCPR_INVALID_PID)
#error XCPR_INVALID_PID already defined
#endif
/** \brief Macro defining the invalid packet identifier. */
#define XCPR_INVALID_PID                           0U

#if (defined XCPR_NO_OF_MAX_DEST_LINKED_TO_ONE_SRC)
#error XCPR_NO_OF_MAX_DEST_LINKED_TO_ONE_SRC already defined
#endif
/** \brief Defines the maximum number of destinations linked to one source. */
#define XCPR_NO_OF_MAX_DEST_LINKED_TO_ONE_SRC     2U

/* *************** Flags for properties of configured destinations *************** */

#if (defined XCPR_TRANSMISSION_MULTIPLE_PACKING)
#error XCPR_TRANSMISSION_MULTIPLE_PACKING already defined
#endif
/** \brief Flag (Bit 0) indicating that packing of multiple messages in one frame is supported
 **        by the PDU channel. */
#define XCPR_TRANSMISSION_MULTIPLE_PACKING                  0x01U

#if (defined XCPR_TRANSMISSION_MAX_DLC_ENABLED)
#error XCPR_TRANSMISSION_MAX_DLC_ENABLED already defined
#endif
/** \brief Flag (Bit 1) indicating that MaxDlc is supported by the PDU channel. */
#define XCPR_TRANSMISSION_MAX_DLC_ENABLED                   0x02U

#if (defined XCPR_RECEPTION_OPEN_SOCON_FLAG)
#error XCPR_RECEPTION_OPEN_SOCON_FLAG already defined
#endif
/** \brief Flag (Bit 2) indicating that the support for Automatically
 *         opening Socket connection for an XcpR source is supported. */
#define XCPR_RECEPTION_OPEN_SOCON_FLAG                 0x04U

#if (defined XCPR_TRANSMISSION_SEQUENCE_CORRECTION)
#error XCPR_TRANSMISSION_SEQUENCE_CORRECTION already defined
#endif
/** \brief Flag (Bit 6) indicating that sequence correction is supported
 **        by the PDU channel. */
#define XCPR_TRANSMISSION_SEQUENCE_CORRECTION               0x40U

#if (defined XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT)
#error XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT already defined
#endif
/** \brief Flag (Bit 7) indicating that the trigger transmit function is supported
 **        by the PDU channel. */
#define XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT                   0x80U

/* *************** Flags for properties of configured sources ******************** */

#if (defined XCPR_RECEPTION_MULTIPLE_PACKING)
#error XCPR_RECEPTION_MULTIPLE_PACKING already defined
#endif
/** \brief Flag (Bit 0) indicating that unpacking of multiple messages in one frame is supported
 **        by the PDU channel. */
#define XCPR_RECEPTION_MULTIPLE_PACKING                     0x01U

#if (defined XCPR_TRANSMISSION_FROM_RXINDICATION)
#error XCPR_TRANSMISSION_FROM_RXINDICATION already defined
#endif
/** \brief Flag (Bit 1) indicating that transmission from the context of RxIndication is supported
 **        by the PDU channel. */
#define XCPR_TRANSMISSION_FROM_RXINDICATION                 0x02U

#if (defined XCPR_TRANSMISSION_OPEN_SOCON_FLAG)
#error XCPR_TRANSMISSION_OPEN_SOCON_FLAG already defined
#endif
/** \brief Flag (Bit 2) indicating that the support for Automatically
 *         opening Socket connection for an XcpR destination is supported. */
#define XCPR_TRANSMISSION_OPEN_SOCON_FLAG              0x04U

/* ******************************************************************************* */

#if (defined XCPR_IGNORED_SOCON_ID)
#error XCPR_IGNORED_SOCON_ID already defined
#endif
/** \brief Ignored Ethernet Socket connection ID. */
#define XCPR_IGNORED_SOCON_ID                              0xFFFFU

#if (defined XCPR_IGNORED_CNT_ID)
#error XCPR_IGNORED_CNT_ID already defined
#endif
/** \brief Ignored counter ID. */
#define XCPR_IGNORED_CNT_ID                                 0xFFU

#if (defined XCPR_INFINITE_RETRIES)
#error XCPR_INFINITE_RETRIES already defined
#endif
/** \brief Magic number for infinite number of Tx retries. */
#define XCPR_INFINITE_RETRIES                               0xFFU

#if (defined XCPR_SID_TRIGGER_DISCONNECT)
#error XCPR_SID_TRIGGER_DISCONNECT already defined
#endif
/** \brief internal API service ID.
 **
 ** Definition of the internal service ID for XcpR_TriggerDisconnectCommand() function. */
#define XCPR_SID_TRIGGER_DISCONNECT                         0x10U

#if (defined XCPR_SID_SESSION_TERMINATED)
#error XCPR_SID_SESSION_TERMINATED already defined
#endif
/** \brief internal API service ID.
 **
 ** Definition of the internal service ID for XcpR_EmitSessionTerminatedEvent() function. */
#define XCPR_SID_SESSION_TERMINATED                         0x11U

#if (defined XCPR_SID_PROCESS_DESTINATIONS)
#error XCPR_SID_PROCESS_DESTINATIONS already defined
#endif
/** \brief internal API service ID.
 **
 ** Definition of the internal service ID for XcpR_ProcessDestinations() function. */
#define XCPR_SID_PROCESS_DESTINATIONS                       0x12U

#if (defined XCPR_SID_GET_PDU_DATA_LENGTH)
#error XCPR_SID_GET_PDU_DATA_LENGTH already defined
#endif
/** \brief internal API service ID.
 **
 ** Definition of the internal service ID for XcpR_GetPduDataLength() function. */
#define XCPR_SID_GET_PDU_DATA_LENGTH                        0x13U

#if (defined XCPR_SID_RECEIVE_RX_DATA)
#error XCPR_SID_RECEIVE_RX_DATA already defined
#endif
/** \brief internal API service ID.
 **
 ** Definition of the internal service ID for XcpR_ReceiveRxData() function. */
#define XCPR_SID_RECEIVE_RX_DATA                            0x14U

#if (defined XCPR_NR_DET_ERROR_TYPES_PROCESSDEST)
#error XCPR_NR_DET_ERROR_TYPES_PROCESSDEST already defined
#endif
/** \brief Defines the number of Det error types reported in XcpR_ProcessDestinations() */
#define XCPR_NR_DET_ERROR_TYPES_PROCESSDEST                 5U

#if (defined XCPR_NR_DET_ERROR_TYPES_FLAG_SIZE)
#error XCPR_NR_DET_ERROR_TYPES_FLAG_SIZE already defined
#endif
/** \brief Defines the number of bytes necessary to allocate for the Det error types buffer
 ** reported in XcpR_ProcessDestinations() */
#if ((XCPR_NR_DET_ERROR_TYPES_PROCESSDEST % 8U) == 0U)
#define XCPR_NR_DET_ERROR_TYPES_FLAG_SIZE  \
  ((XCPR_NR_DET_ERROR_TYPES_PROCESSDEST) / 8U)
#else
#define XCPR_NR_DET_ERROR_TYPES_FLAG_SIZE  \
  ((XCPR_NR_DET_ERROR_TYPES_PROCESSDEST / 8U) + 1U)
#endif

#if (defined XCPR_DET_REPORT_E_PDU_LOST)
#error XCPR_DET_REPORT_E_PDU_LOST already defined
#endif
/** \brief Flag indicating reporting of a E_PDU_LOST Det error. */
#define XCPR_DET_REPORT_E_PDU_LOST                          0U

#if (defined XCPR_DET_REPORT_E_UNEXPECTED_MSG)
#error XCPR_DET_REPORT_E_UNEXPECTED_MSG already defined
#endif
/** \brief Flag indicating reporting of a E_UNEXPECTED_MSG Det error. */
#define XCPR_DET_REPORT_E_UNEXPECTED_MSG                    1U

#if (defined XCPR_DET_REPORT_E_INVALID_CONNECT_MODE)
#error XCPR_DET_REPORT_E_INVALID_CONNECT_MODE already defined
#endif
/** \brief Flag indicating reporting of a E_INVALID_CONNECT_MODE Det error. */
#define XCPR_DET_REPORT_E_INVALID_CONNECT_MODE              2U

#if (defined XCPR_DET_REPORT_E_INVALID_LENGTH)
#error XCPR_DET_REPORT_E_INVALID_LENGTH already defined
#endif
/** \brief Flag indicating reporting of a E_INVALID_LENGTH Det error. */
#define XCPR_DET_REPORT_E_INVALID_LENGTH                    3U

#if (defined XCPR_DET_REPORT_E_INVALID_ACTIVE_DESTINATION)
#error XCPR_DET_REPORT_E_INVALID_ACTIVE_DESTINATION already defined
#endif
/** \brief Flag indicating reporting of a XCPR_E_INVALID_ACTIVE_DESTINATION Det error. */
#define XCPR_DET_REPORT_E_INVALID_ACTIVE_DESTINATION        4U

/** \brief Macro defining the maximum PDU Id value */
#define XCPR_INVALID_PDU_ID ~0U

/** \brief Macro defining the maximum PDU length value */
#define XCPR_INVALID_PDU_LENGTH ~0U

/** \brief Macro defining the maximum value for a Ethernet domain */
#define XCPR_INVALID_ETHERNET_DOMAIN ~0U

/** \brief Macro defining the maximum value for a Ethernet port */
#define XCPR_INVALID_ETHERNET_PORT ~0U

/** \brief Macro defining the maximum value for a Ethernet address */
#define XCPR_INVALID_ETHERNET_ADDRESS ~0U

/** \brief Macro defining the number of bytes in which the IPv6 address is stored */
#define XCPR_SOAD_IPV6ADDRMEMSIZE 16U

/** \brief Macro defining the number of elements in which the IPv6 address is stored */
#define XCPR_SOAD_IPV6ADDRSIZE 4U

#define XCPR_PDU_TYPE_INTERNAL_DATA_SIZE 1U

#define XCPR_INTERNAL_DATA_SIZE (XCPR_PDU_TYPE_INTERNAL_DATA_SIZE + sizeof(PduLengthType) + sizeof(PduIdType))

#if (defined XCPR_BYTE0_INDEX)
#error XCPR_BYTE0_INDEX already defined
#endif
/** \brief Zeroth Byte position for generic use purpose. */
#define XCPR_BYTE0_INDEX                                   0x00U

#if (defined XCPR_BYTE1_INDEX)
#error XCPR_BYTE1_INDEX already defined
#endif
/** \brief First Byte position for generic use purpose. */
#define XCPR_BYTE1_INDEX                                   0x01U

#if (defined XCPR_BYTE2_INDEX)
#error XCPR_BYTE2_INDEX already defined
#endif
/** \brief Second Byte position for generic use purpose. */
#define XCPR_BYTE2_INDEX                                   0x02U

#if (defined XCPR_BYTE3_INDEX)
#error XCPR_BYTE3_INDEX already defined
#endif
/** \brief Third Byte position for generic use purpose. */
#define XCPR_BYTE3_INDEX                                   0x03U

/*==[Types declarations]======================================================*/

typedef struct
{
  PduIdType Id;
  uint8 Type;
} XcpR_ConnectionInfoType;

typedef struct
{
  boolean isPreviousCommandConnect;
  boolean isWaitingForConnectConfirmation;
  uint8 ConnectionStatus;
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
  const XcpR_ConnectionGroupType * ActiveConnectionGroup;
  PduIdType CurrentSrcLinkedDestInternalId;
#endif
  XcpR_ConnectionInfoType MasterSource;
  XcpR_ConnectionInfoType DestinationFromMaster;
  PduIdType DestinationFromMasterInternalId;
  XcpR_ConnectionInfoType SlaveSource;
  XcpR_ConnectionInfoType DestinationFromSlave;
  PduIdType DestinationFromSlaveInternalId;
  uint8 CurrentSourceOrigin;
  uint8 ConnectionMode;
#if (((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)))
  TcpIp_SockAddrInet6Type EthernetActiveIPv6Address;
  TcpIp_SockAddrInetType EthernetActiveIPv4Address;
#endif
} XcpR_StatusType;

/* Msg information used while updating the XcpR internal Msg queue. */
typedef struct
{
  PduLengthType Length;       /* The Length of the Msg */
  PduIdType SrcPduId;         /* The SrcPduId of the Msg */
  uint8 SrcPduType;           /* Rx or Tx source */
  uint8 Msg[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];    /* The Msg */
} XcpR_MsgType;

/* Number of bytes used in MSG Queue. Can hold MAX_CTO + the length information */
#if (XCPR_MSG_QUEUE_SIZE > 0xFF)
  typedef uint16 XcpR_MsgNoOfBytesType;
  typedef uint32 XcpR_MsgNoOfBytesNextType;
#else
  typedef uint8 XcpR_MsgNoOfBytesType;
  typedef uint16 XcpR_MsgNoOfBytesNextType;
#endif

/* MSG queue Type used for the XcpR internal Queue */
typedef struct
{
  XcpR_MsgNoOfBytesType NoOfBytes;     /* Number of bytes in MSG Queue */
  XcpR_MsgNoOfBytesType Head;          /* Msg queue header */
  XcpR_MsgNoOfBytesType Tail;          /* Msg queue tail */
  uint8 Queue[XCPR_MSG_QUEUE_SIZE];    /* Msg queue */
  boolean IsTailRolledOver;            /* Bit to indicate the Tail reached the end of the queue and started again from 0. */
} XcpR_MsgQueueType;

/*==[Declaration of functions with external linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/** \brief Function to copy Rx Pdu information to Rx Pdu buffer
 **
 ** \param[in] XcpRSrcPduId           Source PDU ID
 ** \param[in] Conf_SrcInternalPduId  Configured source PDU ID
 ** \param[in] XcpRRxSrcPduPtr        Pdu information associated to the source PDU ID
 ** \param[in] SrcPduType             Source PDU type (Rx/Tx)
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_ReceiveRxData
(
  PduIdType XcpRSrcPduId,
  PduIdType Conf_SrcInternalPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) XcpRRxSrcPduPtr,
  uint8 SrcPduType
);

/** \brief Function to process the sources.
 **
 ** \param[in] SidId                  The Function Id from where the function is called
 ** \param[in] Conf_SrcInternalPduId  Configured source PDU ID
 ** \param[in] SrcPduType             Source PDU type (Rx/Tx)
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_ProcessSource
(
  uint8 SidId,
  PduIdType Conf_SrcInternalPduId,
  uint8 SrcPduType
);

/** \brief Function to indicate that the Pdu transmission was successfully
 **
 ** Use this function to indicate confirmation via IndicateConfirmation function
 **
 ** \param[in] XcpRTxPduId Internal Id of the Tx Pdu
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_IndicateTxConfirmation
(
  PduIdType XcpRTxPduId
);

/** \brief Function to get the extracted message length from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** \return The extracted message length from XcpR internal queue */
extern FUNC(PduLengthType, XCPR_CODE) XcpR_GetMsgLength
(
  void
);

/** \brief Function to get the extracted message length from XcpR internal queue, but
 **  without increasing the buffer Head counter.
 **
 ** \return The extracted message length from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(PduLengthType, XCPR_CODE) XcpR_ReadMsgLength
(
  void
);

/** \brief Function to get the extracted source PDU ID from XcpR internal queue.
 **
 ** \return The extracted source PDU ID from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(PduIdType, XCPR_CODE) XcpR_GetMsgSrcPduId
(
  void
);

/** \brief Function to get the extracted source PDU ID from XcpR internal queue, but
 **  without increasing the buffer Head counter.
 **
 ** \return The extracted source PDU ID from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(PduIdType, XCPR_CODE) XcpR_ReadMsgSrcPduId
(
  void
);

/** \brief Function to get the extracted source PDU ID type from XcpR internal queue.
 **
 ** \return The extracted source PDU ID type from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(uint8, XCPR_CODE) XcpR_GetMsgSrcPduType
(
  void
);

/** \brief Function to get the extracted source PDU ID type from XcpR internal queue, but
 **  without increasing the buffer Head counter.
 **
 ** \return The extracted source PDU ID type from XcpR internal queue.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(uint8, XCPR_CODE) XcpR_ReadMsgSrcPduType
(
  void
);

/** \brief Function to get the extracted message first byte,
 ** in order to read Pid or Package Response, but
 ** without increasing the buffer Head counter.
 **
 ** \return The extracted message first byte
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(uint8, XCPR_CODE) XcpR_ReadMsgFirstByte
(
  void
);

/** \brief Function to get the extracted message second byte,
 **  in order to read Pid or Package Response, but
 **  without increasing the buffer Head counter.
 **
 ** \return The extracted message second byte.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(uint8, XCPR_CODE) XcpR_ReadMsgSecondByte
(
  void
);

/** \brief Function to Dequeue one message from XcpR internal queue
 **
 ** \param[in] DataLength How much data shall be dequeued
 ** \param[in] BufferPtr  Buffer to which message shall be dequeued
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_DeQueueMsgQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) BufferPtr
);

/** \brief Function to flush all pending messages
 **
 ** Use this function to flush all pending messages.
 ** Queue head, tail and number of elements will be reset
 ** to 0x00U.
 ** The uploaded bytes counter is also reseted.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_FlushMsgQueue
(
  void
);

/** \brief Function to return whether the XcpR queue is full or not
 **
 ** \retval TRUE if XcpR queue is full
 ** \retval FALSE if XcpR queue is not full
 ** \return Whether the XcpR queue is full or not
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(boolean, XCPR_CODE) XcpR_IsMsgQueueEmpty
(
  void
);


/** \brief Function to queue a received message into XcpR queue
 **
 ** \param[in] MsgPtr  Message packet data to be processed
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_InsertMsgInQueue
(
  P2VAR(XcpR_MsgType, AUTOMATIC, XCPR_APPL_DATA) MsgPtr
);

/** \brief Function to queue bytes into XcpR queue.
 **
 ** XcpR Queue is a byte array. Use this function to enqueue one byte from the
 ** into XcpR queue
 **
 ** \param[in] DataByte The data byte to be queued
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_EnqueueByteInMsgQueue
(
  uint8 DataByte
);

/** \brief Function to update total number of bytes in Msg queue
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_UpdateMsgQueueHead
(
  void
);

/** \brief Function to process the destinations.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_ProcessDestinations
(
  void
);

/** \brief Function to initialize receive processor
 **
 ** This function Initializes all variables used by
 ** the XcpR receive processor.
 **/
extern FUNC(void, XCPR_CODE) XcpR_InitReceiveProcessor(void);

/** \brief Function to initialize transmit processor
 **
 ** This function Initializes all variables used by
 ** the XcpR transmit processor. */
extern FUNC(void, XCPR_CODE) XcpR_InitTransmitProcessor(void);

#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/** \brief Function to get the data to be passed through TriggerTransmit
 ** function
 **
 ** Use this function to get the data to be passed through TriggerTransmit
 ** function.
 **
 ** \param[in]  TxPduId id used to link to the buffer
 ** \param[out] PduInfoPtr The address of the buffer in which data needs to be
 ** \return     E_OK,     if the output buffer was successfully written
 **             E_NOT_OK, if no data is available to be written into the output buffer
 **                       copied
 **/
FUNC(Std_ReturnType, XCPR_CODE) XcpR_ProvideTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
);
#endif

/** \brief Function to get the active destination
 **
 ** \param[in] Conf_SrcInternalPduId Configured source PDU ID
 ** \param[in] ConnectMode           Connection mode of the XCPR module(local or remote)
 ** \param[in] SidId                 The Function Id from where the function is called
 **
 ** \return     E_OK,     if the configured destination is successfully linked
 **             E_NOT_OK, if no configured destination  according to used parameters
 **/
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_GetActiveDestination
(
  PduIdType Conf_SrcInternalPduId,
  boolean   ConnectMode,
  uint8     SidId
);

/** \brief Function for processing Bus monitoring.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_ProcessBusMonitor
(
  void
);

/** \brief Function for triggering autonomous disconnect.
 **
 ** \param[in] DestinationId         Internal ID of the Tx destination.
 ** \param[in] EmitSessionTerminated Whether to send the EV_SESSION_TERMINATED
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_TriggerDisconnectCommand
(
  PduIdType DestinationId,
  boolean EmitSessionTerminated
);

#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Function for emit event session terminated.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE)  XcpR_EmitSessionTerminatedEvent
(
  void
);
#endif

/** \brief Function for processing the response of a Autonomous Disconnect request.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_ProcessAutonomousDisconnectResponse
(
  void
);

/** \brief Function for clearing status flags.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_ClearStatusFlags
(
  void
);

/** \brief Function to search for linked destination.
 **
 ** \param[in] Pid_or_PackageResponse Packet identifier or pack response
 ** \param[in] ConnectMode            Connection mode (local or remote network)
 ** \param[in] Conf_SrcInternalPduId  Configured source PDU ID
 ** \param[in] extracted_SrcPduId     Extracted source PDU ID
 ** \param[in] extracted_SrcPduType   Extracted source PDU type (Rx/Tx)
 ** \param[in] SidId                  The Function Id from where the function is called
 **
 ** \return    E_OK,     if the configured destination is successfully linked
 **            E_NOT_OK, if no configured destination  according to used parameters
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, XCPR_CODE) XcpR_FindDestinationPduId
(
  uint8     Pid_or_PackageResponse,
  uint8     ConnectMode,
  PduIdType Conf_SrcInternalPduId,
  PduIdType extracted_SrcPduId,
  uint8     extracted_SrcPduType,
  uint8     SidId
);

/** \brief Function to report DET erros
 **
 **\ Development error {XCPR_E_INVALID_CONNECT_MODE}
 **\ Development error {XCPR_E_INVALID_LENGTH}
 **\ Runtime error     {XCPR_E_PDU_LOST}
 **\ Runtime error     {XCPR_E_UNEXPECTED_MSG}
 **/
extern FUNC(void, XCPR_CODE) XcpR_TriggerDetErrorsProcessDest
(
  void
);

/** \brief Function to initializes and resets all error counters
 **
 ** The function resets all error counters.
 **/
extern FUNC(void, XCPR_CODE) XcpR_InitDetErrorsProcessDest
(
  void
);

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
#if (XCPR_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)

/** \brief Function which copies the remote address from the socket having the SourceSocketId to
 **        the socket having the DestSocketId.
 **
 ** The copy of the remote address is done with/without reseting the port value.
 **
 ** \param[in]   SourceSocketId The socket ID which is the source of the remote address.
 ** \param[in]   IsConnectCmd: - TRUE when CONNECT command was called before any other information received on the bus;
 **                            - FALSE when the CONNECT command wasn't called before any other received information.
 */
extern FUNC(void, XCPR_CODE) XcpR_CopySoAdRemoteAddress
(
  SoAd_SoConIdType SourceSocketId,
  boolean IsConnectCmd,
  boolean IsIPv6
);
#endif

/** \brief Function used to delete the most recently added packet from the
 **  XcpR queue if the SoAd socket address is wrong.
 **
 ** */
extern FUNC(void, XCPR_CODE) XcpR_RemoveLastMsgFromQueue(void);
#endif

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

#define XCPR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief The internal XcpR message queue, implemented as a byte array. */
extern VAR(XcpR_MsgQueueType, XCPR_VAR_CLEARED) XcpR_MsgQueue;

#define XCPR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief Internal status of the XcpR module. */
extern VAR(XcpR_StatusType, XCPR_VAR) XcpR_Status;

#define XCPR_STOP_SEC_VAR_UNSPECIFIED
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_8
#include <XcpR_MemMap.h>

/** \brief Initialization status of the XcpR module */
extern VAR(boolean, XCPR_VAR) XcpR_Initialized;

/** \brief Array of counters used for reporting Det errors outside the critical section
 **  protecting the loop inside XcpR_ProcessDestinations().
 **  Corresponding values:
 **  Index 0: Counter for runtime errors of type     XCPR_E_PDU_LOST
 **  Index 1: Counter for runtime errors of type     XCPR_E_UNEXPECTED_MSG
 **  Index 2: Counter for development errors of type XCPR_E_INVALID_CONNECT_MODE
 **  Index 3: Counter for development errors of type XCPR_E_INVALID_LENGTH
 **  Index 4: Counter for runtime errors of type     XCPR_E_INVALID_ACTIVE_DESTINATION
 */
extern VAR(uint8, XCPR_VAR) XcpR_ReportDetProcessDestCtr[XCPR_NR_DET_ERROR_TYPES_PROCESSDEST];

#define XCPR_STOP_SEC_VAR_8
#include <XcpR_MemMap.h>

#endif /* XCPR_INT_H */

