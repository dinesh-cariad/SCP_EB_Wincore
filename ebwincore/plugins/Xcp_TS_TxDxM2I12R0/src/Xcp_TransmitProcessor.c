/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function
 *
 * Reason:
 * Using the variable at function scope would require making the variable static, which
 * would prevent allocation to a predefined memory section.
 *
 */

/*==================[inclusions]============================================*/

#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSMem.h>             /* EB memory functions */
#include <SchM_Xcp.h>          /* Needed for exclusive area definition */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>           /* Internal defines and declarations */
#include <Xcp_Wrapper.h>

#if(XCP_PROD_ERR_HANDLING_RETRY_FAILED!= TS_PROD_ERR_DISABLE)
# include <Dem.h>               /* Dem event IDs */
#endif

/*==================[macros]================================================*/

#if (defined XCP_TX_MAX_QUEUE_ID)
# error XCP_TX_MAX_QUEUE_ID already defined
#endif
/** \brief Definition of available PDU queues */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  #if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  #define XCP_TX_MAX_QUEUE_ID                        3U
  #else
  #define XCP_TX_MAX_QUEUE_ID                        2U
  #endif
#elif(XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  #define XCP_TX_MAX_QUEUE_ID                        2U
#else
  #define XCP_TX_MAX_QUEUE_ID                        1U
#endif

#if (defined XCP_IS_TX_PDU_CHANNEL_ACTIVE)
#error XCP_IS_TX_PDU_CHANNEL_ACTIVE already defined
#endif
/** \brief Checks if the Pdu channel is active.
 **
 ** \param[in] IntBufId    Internal Tx Pdu Buffer Id
 **
 ** \return Transmit status of the selected PDU
 ** \retval XCP_TX_IDLE        no is pending
 ** \retval XCP_TX_IN_PROGRESS transmission is in progress */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
#define XCP_IS_TX_PDU_CHANNEL_ACTIVE(TxPduId) \
  (((XCP_IS_BYTE_FLAG_SET(TxPduId, Xcp_TxLPduData.IsPduWaitingForTriggFlag) == TRUE) || \
    (XCP_IS_BYTE_FLAG_SET(TxPduId, Xcp_TxLPduData.IsPduWaitingForConfFlag) == TRUE) \
   ) ? TRUE : FALSE )
#else
#define XCP_IS_TX_PDU_CHANNEL_ACTIVE(TxPduId) \
  (((XCP_IS_BYTE_FLAG_SET((TxPduId), (Xcp_TxLPduData.IsPduWaitingForConfFlag)) == TRUE) \
   ) ? TRUE : FALSE )
#endif

#if (defined XCP_GET_PDU_TRANSMIT_STATUS)
#error XCP_GET_PDU_TRANSMIT_STATUS already defined
#endif
/** \brief Get Pdu transmit status.
 **
 ** \param[in] IntBufId    Internal Tx Pdu Buffer Id
 **
 ** \return Transmit status of the selected PDU
 ** \retval E_OK        no transmission is pending
 ** \retval E_NOT_OK    transmission is in progress */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
#define XCP_GET_PDU_TRANSMIT_STATUS(IntBufId) \
  (((XCP_IS_BYTE_FLAG_SET(IntBufId, Xcp_TxLPduData.IsBufferFullFlag) == TRUE) && \
    (XCP_IS_BYTE_FLAG_SET(IntBufId, Xcp_TxLPduData.IsPduWaitingForTriggFlag) == FALSE) && \
    (XCP_IS_BYTE_FLAG_SET(IntBufId, Xcp_TxLPduData.IsPduWaitingForConfFlag) == FALSE) \
   ) ? E_OK : E_NOT_OK )
#else
#define XCP_GET_PDU_TRANSMIT_STATUS(IntBufId) \
  (((XCP_IS_BYTE_FLAG_SET(IntBufId, Xcp_TxLPduData.IsBufferFullFlag) == TRUE) && \
    (XCP_IS_BYTE_FLAG_SET(IntBufId, Xcp_TxLPduData.IsPduWaitingForConfFlag) == FALSE) \
   ) ? E_OK : E_NOT_OK )
#endif


#if (defined XCP_GET_RES_TX_PDU_INFO_LENGTH)
#error XCP_GET_RES_TX_PDU_INFO_LENGTH already defined
#endif
/** \brief Get length of Tx Pdu used used when transmission is initiated */
#define XCP_GET_RES_TX_PDU_INFO_LENGTH(IntBufId) \
  (Xcp_TxLPduData.LPduLength[(IntBufId)])

#if (defined XCP_SET_RES_TX_PDU_INFO_LENGTH)
#error XCP_SET_RES_TX_PDU_INFO_LENGTH already defined
#endif
/** \brief Set transmit Pdu length into transmit buffer */
#define XCP_SET_RES_TX_PDU_INFO_LENGTH(IntBufId, PduLength) \
  (Xcp_TxLPduData.LPduLength[(IntBufId)] = (PduLength))  \

/*==================[type definitions]======================================*/

/** \brief Definition of PDU data getter function handling data from selected PDU queue
 * type */
typedef P2FUNC(void, XCP_CONST, TxGetPduDataHandlerFuncPtr)
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) BufferPtr
);

/** \brief Definition of PDU data getter function returning if the selected PDU queue has data
 * type */
typedef P2FUNC(boolean, XCP_CONST, TxIsPduQueueEmptyHandlerFuncPtr)(void);

/** \brief Definition of PDU data getter function returning the PDU data length for selected queue
 * type */
typedef P2FUNC(PduLengthType, XCP_CONST, TxGetPduLengthHandlerFuncPtr)(void);

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
/** \brief Definition of PDU data getter function returning if the selected PDU has a multicast
 * response */
typedef P2FUNC(boolean, XCP_CONST, TxIsPduMulticastHandlerFuncPtr)(void);
#endif

/** \brief Definition of PDU data stucture containing the getter functions for retrieving data from
 **        selected queue
 * type */
typedef struct{
  /* handler used to get the PDU data */
  CONST(TxGetPduDataHandlerFuncPtr, XCP_CONST) GetPduData;
  /* handler used to check if there is any data available */
  CONST(TxIsPduQueueEmptyHandlerFuncPtr, XCP_CONST) IsPduQueueEmpty;
  /* handler used to get the PDU data length */
  CONST(TxGetPduLengthHandlerFuncPtr, XCP_CONST) GetPduLength;
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* handler used to inquire if the PDU contains a multicast response */
  CONST(TxIsPduMulticastHandlerFuncPtr, XCP_CONST) IsPduMulticast;
#endif
  /* the supported package type */
  uint8 PackageType;
}Xcp_GetTxPduDataType;

/** \brief data type containing informations regarding tx LPdu */
typedef struct
{
  /* ------------- BusTimeout mechanism data ------------- */
  /* counter used to count up the number of cycles until BusTimeout mechanism overflows */
  Xcp_BusTimeoutCounterValueType BusTimeoutCntVal[XCP_MAX_TX_PDU_PER_CON];
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  uint16 LPduStartPos[XCP_MAX_TX_PDU_PER_CON]; /* runtime info about where to store the
                                              payload of this PDU. Can be changed by FLX_ASSIGN */
  uint8 LPduMaxLenBuf[XCP_MAX_TX_PDU_PER_CON]; /* MAX_FLX_LEN_BUF_x. Can be changed by FLX_ASSIGN */
#if XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT == STD_ON
  /* counter used to count up the number of cycles until FlexRay Hardware Buffers Timeout mechanism overflows */
  Xcp_FlexRayTimeoutCounterValueType Xcp_FlexTimeoutCnt;
#endif
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  /* Semaphores: bit flag indicating if DAQ is enabled for the selected PDU */
  uint8 LPDUPacketStatusDaq[XCP_TX_PDU_FLAG_SIZE];
#endif
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  /* Semaphores: bit flag indicating if EVSERV is enabled for the selected PDU */
  uint8 LPDUPacketStatusEvServ[XCP_TX_PDU_FLAG_SIZE];
  /* Semaphores: bit flag indicating if EVSERV is enabled for the selected PDU */
  uint8 LPDUPacketActivateEvServ[XCP_TX_PDU_FLAG_SIZE];
#endif
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */
  /* ------------- Tx Pdu buffer data ------------- */
  /* Length of Tx Pdu */
  PduLengthType LPduLength[XCP_MAX_TX_PDU_PER_CON];
  /* Tx LPdu buffer used to contingently store the Tx PDU data, for all available PDUs */
  uint8 LPduBuffer[XCP_TX_BUFFER_SIZE];
  /* ------------- Retry mechanism data ------------- */
#if( (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) )
  /* Semaphores: bit flag indicating if the Tx buffer contains the EvResumeMode package. It is
   * used by retry mechanism to prevent aborting transmission of this EvResumeMode package. */
  uint8 IsEvResumeModePendingFlag[XCP_TX_PDU_FLAG_SIZE];
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
#if (XCP_TX_RETRY_COUNT > 0)
  /* counter used to count down the number of retries until the Tx package is aborted */
  uint8 RetryCntVal[XCP_MAX_TX_PDU_PER_CON];
#endif /* (XCP_TX_RETRY_COUNT > 0)  */
  /* ------------- Tx Pdu channel data ------------- */
  /* Semaphores: bit flag indicating if PDU is waiting for confirmation.
   * It is used to prevent multiple confirmation of the same Tx Pdu data */
  uint8 IsPduWaitingForConfFlag[XCP_TX_PDU_FLAG_SIZE];
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
  /* Semaphores: bit flag indicating if PDU is waiting for trigger.
   * It is used to prevent multiple triggering of the same Tx Pdu data */
  uint8 IsPduWaitingForTriggFlag[XCP_TX_PDU_FLAG_SIZE];
#endif
#if (XCP_TX_BUS_RETRY_COUNT > 0U)
   /* counters used to retry the transmission whenever the bus timeout occurs */
  uint8 BusTxRetryCount[XCP_MAX_TX_PDU_PER_CON];
#endif
#if ((XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) || (XCP_SUPPRESS_TX_SUPPORT == STD_ON))
  /* Semaphores: bit flag indicating if PDU is enabled */
  uint8 IsLPduEnabled[XCP_TX_PDU_FLAG_SIZE];
#endif
  /* Semaphores: bit flag indicating if the buffer has new data in it.
   * It is used to prevent overwrite of the old data of each Tx Pdu buffer */
  uint8 IsBufferFullFlag[XCP_TX_PDU_FLAG_SIZE];
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
  /* counter for the number of changes of Multicast to Unicast retries */
  uint8 MulticastRetryCnt[XCP_MAX_TX_PDU_PER_CON];
#endif
  /* Bit flag indicating if a PDU has a multicast response.
   * It is used to set the multicast address before transmission and then
   * restore the unicast address after transmission is completed */
  uint8 IsMulticastFlag[XCP_TX_PDU_FLAG_SIZE];
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
}Xcp_TxLPduDataType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function used to copy available data to the Tx buffers.
 **
 ** \param[in] IntTxPduId Id of the transmission PDU
 */
STATIC FUNC(void, XCP_CODE) Xcp_UpdateAvailableTxBuffers
(
  PduIdType IntTxPduId
);


#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
/** \brief Function to monitor the FlexRay buffers
 ** The function will check if the FrexRay bus is in sync state and, if is,
 ** will call for the buffers initialization if they are not already initialized
 ** and, if not, will set up a flag to prevent the buffers usage
 */
STATIC FUNC(void, XCP_CODE) Xcp_MonitorFlexRayBuffers(void);

/** \brief Function to calculate the FlexRay HEADER_CRC
 ** \param[in]  FrameId - The Flexray Frame ID
 ** \param[in]  PayloadLength - The Flexray Frame Payload length
 ** \return The calculated HEADER_CRC
 **
 */
STATIC FUNC(uint16, XCP_CODE) Xcp_FlxCalculateHeaderCrc(uint16 FrameId, uint8 PayloadLength);

/** \brief Function to calculate an nibble for FlexRay HEADER_CRC
 ** \param[in]  tmpCRC - The HEADER_CRC temp value
 ** \param[in]  Nibble - nibble of the HEADER_CRC
 ** \return The calculated HEADER_CRC
 */
STATIC FUNC(uint16, XCP_CODE) Xcp_FlxCrcCalcNibble(uint16 tmpCRC, uint8 Nibble);


#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/

/** \brief Function used to transmit data for a certain PduId.
 **
 ** \param[in] SidId  Id of the caller function
 ** \param[in] IntTxPduId Id of the transmission PDU
 **
*/
STATIC FUNC(void, XCP_CODE) Xcp_TransmitAvailableTxPdus
(
  uint8 SidId,
  PduIdType IntTxPduId
);

/** \brief Function to pack data into Tx PDU buffer.
 **
 ** \param[in] TxBufferId  Id of the selected Tx buffer
 */
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_PackTxBufferData
(
  PduIdType TxBufferId
);

/** \brief Function to determine if the PDU can accept the given
 ** package type
 **
 ** \param[in] PackageType - the package type to check
 ** \param[in] TxPduId - the PDU Id
 ** \return whether the PDU can accept the given package type
 ** \retval TRUE the PDU can accept the given package type
 ** \retval FALSE the PDU cannot accept the given package type
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsTxPacketAllowed
(
  uint8 PackageType,
  PduIdType TxPduId
);

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
/** \brief Function to initialize the FlexRay frame header.
 **
 ** \param[out] TxBufferId  Id of the selected Tx buffer
 */
STATIC FUNC(void, XCP_CODE) Xcp_InitFlexRayBuffer
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
);
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if ((XCP_ON_ETHERNET_ENABLED == STD_ON) || \
     ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && \
     ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))))
/** \brief Add the frame header and tail to the PDU to be sent
 **
 ** \param[in]    DataLength  Length of the current Pdu data section
 ** \param[in]    HeaderLength  Length of the current Pdu header section
 ** \param[in]    FirstPacket Whether this is the first packet in the frame or not
 ** \param[in]    IncrementCnt Whether to increment the header counter or not
 ** \param[out] TxBufferPtr Pointer to the TxFrame, to which the header and tail
 **                           shall be added.
 */
STATIC FUNC(void, XCP_CODE) Xcp_AddFrameHeader
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr,
  PduLengthType HeaderLength,
  boolean FirstPacket,
  boolean IncrementCnt
);
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) || (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

/** \brief Transmit Retry mechanism.
 **
 ** \param[in] TxPduId     Internal Id of the selected Tx Pdu channel
 ** \param[in] TxStatus    Transmit status for the selected Tx Pdu channel
 ** \param[out] AbortTxPtr Pointer to Abort transmit signal
 **/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessRetry
(
  PduIdType TxPduId,
  Std_ReturnType TxStatus,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) AbortTxPtr
);

/** \brief Transmit Pdu via transport layer bus.
 **
 ** \param[in] TxPduId       Destination PDU id from transport layer
 ** \param[in] PduInfoPtr    Pdu data to transmit
 **
 ** \return Transmit operation status
 ** \retval E_OK transmission was accepted by transport layer
 ** \retval E_NOT_OK transmission was rejected by transport layer
 **/
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, EBSTUBS_APPL_CONST) PduInfoPtr
);

/** \brief Set the status for the selected PDU.
 **
 ** \param[in] IntTxPduId    Internal Tx Pdu Buffer Id
 ** \param[in] TxStatus      The state of the Tx PDU:
 **                          * E_OK, the transmission was accepted
 **                          * !E_OK, the transmission was rejected,
 **                            or there is nothing to transmit.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_SetPduTransmitStatus
(
  PduIdType IntBufId,
  Std_ReturnType TxStatus
);


/** \brief Function to get the PDU start position inside the Tx buffer.
 ** \param[in]   IntPduId  The internal PDU ID.
 ** \return  A pointer to the buffer where this PDU can write data to be transmitted
 */
STATIC FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) XcpGetResTxPduInfoPtr
(
  PduIdType IntPduId
);

#if (XCP_NO_OF_CONNECTIONS > 1U)
/** \brief Function used to release the I'th connection resources lock. */
STATIC FUNC(void, XCP_CODE) Xcp_ProcessResourcesUnlock(void);
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */

#if (XCP_ON_CANFD_ENABLED == STD_ON)
/** \brief Add fill bytes to the CAN-FD frame
 **
 ** \param[out] TxBufferPtr Pointer to the TxFrame, to which the header and tail
 **                           shall be added.
 ** \param[out]    DataLength  Length of the current Pdu data section
 ** \param[in]     AvailableFrameLength  Length of the current Pdu data section available
 */
STATIC FUNC(void, XCP_CODE) Xcp_AddCanFDFill
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLength,
  PduLengthType AvailableFrameLength
);
#endif


#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)

/** \brief Add an empty last message with LEN=0 if connection is on FlexRay
 **  and multiple packages in one frame are configured
 **
 ** \param[in] AvailableTxSpace available space into Tx frame
 ** \param[out] TxBufferPtr Pointer to the TxFrame, to which the tail shall be added.
 ** \return Whether an empty packet was added
 ** \retval TRUE   Empty packet was added
 ** \retval FALSE  Empty packet was not added
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_AddFlexRayMultiPduInFrameLastMessage
(
  PduLengthType AvailableTxSpace,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
);
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)*/

#if(XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Function to prepare the EV_SESSION_TERMINATED event package notification. */
STATIC FUNC(void, XCP_CODE)  Xcp_EmitSessionTerminatedEvent(void);
#endif

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
/** \brief Function to verify if it's ok to transmit a PDU when multicast is enabled. */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsAllowedToTransmitMulticast(PduIdType IntTxPduId);
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
/** \brief Function to monitor Multicast buffers
 ** The function verifies if the change form multicast address to unicast address
 ** has been successfully performed otherwise retry the request. In case of failure
 ** the retry counter is incremented until the configured number of retries is reached
 ** and performs an autonomous disconnect afterwards
 */
STATIC FUNC(void, XCP_CODE) Xcp_MonitorMulticastChange(void);
#endif

#if (XCP_MAX_DAQ != 0U)
/** \brief This function performs a size validation of the packets that are prepared
 **        to be transmitted. If the validation is not successful, the packet is not
 **        transmitted and it is also removed from the queue of the respective data
 **        handler.
 **        Currently it only verifies that DTO packets do not exceed the connection
 **        specific MAX_DTO but it can be extended in the future to also cover other
 **        scenarios.
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsTxPacketSizeOk(uint8 HandlerId);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief encapsulate handlers to get Event CTO data type */
STATIC CONST(Xcp_GetTxPduDataType, XCP_CONST) Xcp_EvCtoQueueHandlers =
{
  &Xcp_DeQueueEventCtoQueue,
  &Xcp_IsEventCtoQueueEmpty,
  &Xcp_GetEventCtoLength,
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  NULL_PTR,
#endif
  XCP_SUPPORT_FOR_EVSERV_CTO_PDU
};
#endif

/** \brief encapsulate handlers to get CTO data type */
STATIC CONST(Xcp_GetTxPduDataType, XCP_CONST) Xcp_CtoQueueHandlers =
{
  &Xcp_DeQueueCtoQueue,
  &Xcp_IsCtoQueueEmpty,
  &Xcp_GetCtoLength,
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  &Xcp_IsMulticastResponse,
#endif
  XCP_SUPPORT_FOR_RESERR_CTO_PDU
};

#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
/** \brief encapsulate handlers to get DTO data type */
STATIC CONST(Xcp_GetTxPduDataType, XCP_CONST) Xcp_DtoQueueHandlers =
{
  &Xcp_GetDtoData,
  &Xcp_IsDaqPrioQueueEmpty,
  &Xcp_GetDtoLength,
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  NULL_PTR,
#endif
  XCP_SUPPORT_FOR_DAQ_DTO_PDU
};
#endif

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
/** \brief Table used for calculating the CRC of the header */
/* Deviation MISRAC2012-1 <START> */
STATIC CONST(uint16, XCP_CONST) Xcp_HeaderCrcTable[] =
{
     0x0000, 0x0385, 0x070A, 0x048F, 0x0591, 0x0614, 0x029B, 0x011E,
     0x00A7, 0x0322, 0x07AD, 0x0428, 0x0536, 0x06B3, 0x023C, 0x01B9
};
/* Deviation MISRAC2012-1 <STOP> */
#endif

/** \brief array of handlers use to get PDU data from the selected queue */
/* Deviation MISRAC2012-1 <START> */
STATIC CONSTP2CONST(Xcp_GetTxPduDataType, XCP_CONST, XCP_APPL_CODE) Xcp_TxPduDataHandlers[XCP_TX_MAX_QUEUE_ID] =
{
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  &Xcp_EvCtoQueueHandlers,        /* handlers used to take data from EventCTO queue,
                                   * has the highest priority */
#endif
  &Xcp_CtoQueueHandlers,          /* handlers used to take data from CTO queue */
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  &Xcp_DtoQueueHandlers           /* handlers used to take data from DTO queue,
                                   * has the lowest priority */
#endif
};
/* Deviation MISRAC2012-1 <STOP> */

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Transmit queue containing transmit data and length information */
STATIC VAR(Xcp_TxLPduDataType, XCP_VAR_CLEARED) Xcp_TxLPduData;

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_NUMBER_OF_FRAME_COUNTERS > 0U)
#define XCP_START_SEC_VAR_CLEARED_16
#include <Xcp_MemMap.h>

/** \brief Header frame transmit counter */
STATIC VAR(uint16, XCP_VAR_CLEARED) Xcp_FrameTransmitCounter[XCP_NUMBER_OF_FRAME_COUNTERS];

#define XCP_STOP_SEC_VAR_CLEARED_16
#include <Xcp_MemMap.h>
#endif /* XCP_NUMBER_OF_FRAME_COUNTERS > 0U */

/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if (XCP_NUMBER_OF_FRAME_COUNTERS > 0U)
/*-----------------------------[Xcp_InitFrameCtr]------------------------*/
FUNC(void, XCP_CODE) Xcp_InitFrameCtr(void)
{
  uint8 Iterator;

  DBG_XCP_INITFRAMECTR_ENTRY();

  for(Iterator = 0U; Iterator < XCP_NUMBER_OF_FRAME_COUNTERS; Iterator++)
  {
    Xcp_FrameTransmitCounter[Iterator] = 0U;
  }

  DBG_XCP_INITFRAMECTR_EXIT();
}
#endif /* (XCP_NUMBER_OF_FRAME_COUNTERS > 0U) */

/*-----------------------------[Xcp_InitTransmitProcessor]------------------*/

FUNC(void, XCP_CODE) Xcp_InitTransmitProcessor(void)
{
  PduIdType Iterator;

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  uint16 startAddress  = 0;
#endif

  DBG_XCP_INITTRANSMITPROCESSOR_ENTRY();

#if(XCP_NO_OF_CONNECTIONS == 1U)
  /* Initialize runtime flags from configuration: if the PDU is dynamic and which packet is supported.
  In case multiple configurations are configured, this initialization occurs at Rx Indication time, depending
  on the needed configuration  */
  Xcp_InitTxPduFlags(0U);
#endif

  for(Iterator = 0U; Iterator < XCP_MAX_TX_PDU_PER_CON; Iterator++)
  {
    /* --- clear tx pdu buffer data --- */
    XCP_CLEAR_BYTE_FLAG(Iterator,Xcp_TxLPduData.IsBufferFullFlag);
    /* Initialize transmit data pdu (Transmit data and length). */
    Xcp_TxLPduData.LPduLength[Iterator] = 0U;
    /* --- clear retry mechanism data --- */
#if( (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) )
    XCP_CLEAR_BYTE_FLAG(Iterator,Xcp_TxLPduData.IsEvResumeModePendingFlag);
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) */
#if (XCP_TX_RETRY_COUNT > 0)
    Xcp_TxLPduData.RetryCntVal[Iterator] = XCP_TX_RETRY_COUNT;
#endif /* (XCP_TX_RETRY_COUNT > 0) */
    /* --- clear Tx Pdu channel data --- */
    XCP_CLEAR_BYTE_FLAG(Iterator,Xcp_TxLPduData.IsPduWaitingForConfFlag);
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
    XCP_CLEAR_BYTE_FLAG(Iterator,Xcp_TxLPduData.IsPduWaitingForTriggFlag);
#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
    /* --- clear BusTimeout mechanism data --- */
    Xcp_TxLPduData.BusTimeoutCntVal[Iterator] = 0U;
#if (XCP_TX_BUS_RETRY_COUNT > 0U)
    Xcp_TxLPduData.BusTxRetryCount[Iterator] = 0U;
#endif
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    XCP_CLEAR_BYTE_FLAG(Iterator,Xcp_TxLPduData.IsMulticastFlag);
#endif
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
    /* Reset Multicast to Unicast retry counter */
    Xcp_TxLPduData.MulticastRetryCnt[Iterator] = 0U;
#endif /* (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0) */
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
  }

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

  /* initialize max buffer length (MAX_FLX_LEN_BUF_x_init) */
  for (Iterator=0; Iterator<XCP_NO_OF_FLX_BUF_CONFIGURED; Iterator++)
  {
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[Iterator].Flags,uint8) == TRUE)
    {
      Xcp_TxLPduData.LPduStartPos[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] = startAddress;

      /* set the value from configuration */
      Xcp_TxLPduData.LPduMaxLenBuf[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] =
          Xcp_FlxBufConfig[Iterator].FlxBufLengthInitValue;
      startAddress += Xcp_FlxBufConfig[Iterator].FlxBufLengthInitValue;
    }
  }

#if XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT == STD_ON
  /* Reset FlexRay buffers timeout */
  Xcp_TxLPduData.Xcp_FlexTimeoutCnt = 0U;
#endif
#endif /*(XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/

  DBG_XCP_INITTRANSMITPROCESSOR_EXIT();
}

/*-----------------------------[Xcp_ProcessTransmit]------------------------*/

FUNC(void, XCP_CODE) Xcp_ProcessTransmit
(
  uint8 SidId
)
{
  /* Variable used to iterate over all configured TxPdus */
  PduIdType TxPduId;
  /* pointer used to select the locked connection resource */
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_PROCESSTRANSMIT_ENTRY(SidId);

#if (XCP_COM_MAINFUNCTION_SUPPORT == STD_ON)
  if (SidId != XCP_SID_MAIN_FUNCTION)
#endif
  {
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
      /* !LINKSTO Xcp846,1 */
      /* !LINKSTO Xcp847,1 */
      /* Proceed only if the current connection (channel) has Tx enabled */
      if (XCP_IS_BYTE_FLAG_SET(Xcp_ConnectionStatus.ConnectionId,Xcp_TransmissionDisabled) == FALSE)
#endif
      {
        /* select the connection configuration */
        ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
        /* iterate over all configured TxPduIds */
        for(TxPduId = 0U; TxPduId < ConnectionCfgPtr->TxPduMax; TxPduId++)
        {
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
          boolean LocalReturnValue = E_NOT_OK;
          /* if PDU is dynamic and enabled or PDU is not dynamic */
          if ((TRUE == XCP_IS_FLAG_SET(XCP_DYNAMIC_TX_PDU,
              Xcp_TxLPduIdConf[ConnectionCfgPtr->TxPduIdList[TxPduId]].Properties,uint8)) &&
              (XCP_IS_BYTE_FLAG_SET(TxPduId, Xcp_TxLPduData.IsLPduEnabled) == TRUE))
          {
            LocalReturnValue = E_OK;
          }
          else if (FALSE == XCP_IS_FLAG_SET(XCP_DYNAMIC_TX_PDU,
              Xcp_TxLPduIdConf[ConnectionCfgPtr->TxPduIdList[TxPduId]].Properties,uint8))
          {
            LocalReturnValue = E_OK;
          }
          else
          {
            /* nothing to do */
          }
          if (LocalReturnValue == E_OK)
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */
          {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
            /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastMode,1 */
            if (Xcp_IsAllowedToTransmitMulticast(TxPduId) == TRUE)
#endif
            {
              /* fill data into available Tx buffers */
              Xcp_UpdateAvailableTxBuffers(TxPduId);
              /* send available Transmit Pdus */
              Xcp_TransmitAvailableTxPdus(SidId, TxPduId);
            }
          }
        }
      }
    }
  }
#if ((XCP_COM_MAINFUNCTION_SUPPORT == STD_ON)||(XCP_EVENT_TRIGGERED_MAINFUNC == STD_ON))
  if (SidId == XCP_SID_MAIN_FUNCTION)
#endif
  {
#if(XCP_NO_OF_CONNECTIONS > 1U)
#if(XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    if(XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8) == FALSE)
#endif
    {
      /* Execute release mechanism for the I'th level connection resources's lock. */
      Xcp_ProcessResourcesUnlock();
    }
#endif
  }
  DBG_XCP_PROCESSTRANSMIT_EXIT();
}

/*-----------------------------[Xcp_ProcessBusMonitor]------------------*/

FUNC(void, XCP_CODE) Xcp_ProcessBusMonitor
(
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsDisconnectTriggered
)
{
  PduIdType Iterator;
  P2VAR(Xcp_BusTimeoutCounterValueType, AUTOMATIC, XCP_APPL_DATA) BusTimeoutCtrValPtr;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_PROCESSBUSMONITOR_ENTRY();

  *IsDisconnectTriggered = FALSE;

  if (XCP_IS_FLAG_SET(XCP_AUTONOMOUS_DISCONNECT_FLAG,
                      Xcp_ConnectionStatus.Flags,
                      uint8) == TRUE)
  {
#if(XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* Perform the DISCONNECT only when the EV packet EV_SESSION_TERMINATED was sent */
    if (Xcp_IsEventCtoQueueEmpty() == TRUE)
#endif
    {
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
      /* Prevent event main functions to execute and check continuously that no event
         main function is being processed. Only when there are no more event main
         functions running,  disconnect will happen.*/
      /* !LINKSTO Xcp.DISCONNECT.AUTONOMOUS.BusyWhenEventMFIsProcessing.Postpone,1 */
      if (Xcp_IsAtLeastOneEventMainFunctionProcessing() == TRUE)
      {
        /* Set flag to prevent the event main functions to be executed anymore */
        /* !LINKSTO Xcp.DISCONNECT.AUTONOMOUS.BusyWhenEventMFIsProcessing.StopEvents,1 */
        Xcp_EventMainFunctionsExecutionIsAllowed = FALSE;
      }
      else
#endif /* XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U */
      {
        /* Enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        /* A Disconnect event is detected */
        Xcp_Disconnect();
        *IsDisconnectTriggered = TRUE;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
        /* Re-enable event main functions */
        Xcp_EventMainFunctionsExecutionIsAllowed = TRUE;
#endif
        /* Exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
    }
  }
  else
  {
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if ((Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID) &&
        (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY))
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      /* Check if FlexRay buffers are being initialized and, if not, if we can initialize them */
      Xcp_MonitorFlexRayBuffers();
    }
#endif /* ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)) */

#if(XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
#endif
    {
      /* Perform Bus Timeout monitor */
      /* select connection configuration */
      ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
      /* check the status for available Tx buffers. */
      for(Iterator = 0U; (Iterator < ConnectionCfgPtr->TxPduMax); Iterator++)
      {
        /* Enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        /* select the bus timeout counter */
        BusTimeoutCtrValPtr = &Xcp_TxLPduData.BusTimeoutCntVal[Iterator];

        if(TRUE == XCP_IS_TX_PDU_CHANNEL_ACTIVE(Iterator))
        {
          if(*BusTimeoutCtrValPtr < XCP_TX_BUS_TO_COUNTER_MAX_VAL)
          {
            /* increment the Bus Timeout Counter's value until it reaches the maximum configuration value */
            *BusTimeoutCtrValPtr += 1U;
          }
          else
          {
#if (XCP_TX_BUS_RETRY_COUNT > 0U)
            /* A bus timeout occurred, check if we should retry the transmission */
            if (Xcp_TxLPduData.BusTxRetryCount[Iterator] < XCP_TX_BUS_RETRY_COUNT)
            {
              /* !LINKSTO Xcp.TX.Message.BusRetry,1 */
              /* Retry the transmission */
#if (XCP_TX_BUS_RETRY_COUNT < XCP_INFINITE_RETRIES)
              /* !LINKSTO Xcp.TX.Message.BusRetry.Infinite,1 */
              /* Increment the retry count only if we shouldn't repeat indefinitely */
              Xcp_TxLPduData.BusTxRetryCount[Iterator]++;
#endif /* XCP_TX_BUS_RETRY_COUNT < XCP_INFINITE_RETRIES */

              /* Clear the pending flags */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
              XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsPduWaitingForTriggFlag);
#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
              XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsPduWaitingForConfFlag);

              /* Reset the bus timeout counter */
              *BusTimeoutCtrValPtr = 0U;
            }
            else
#endif /* XCP_TX_BUS_RETRY_COUNT > 0U */
            {
              /* !LINKSTO Xcp.TX.Message.BusRetry.Disconnect,1 */
              /* Start the DISCONNECT process, due to event "Bus Timeout" or because the retries have expired */
              Xcp_PrepareForAutonomousDisconnect(TRUE);
            }
          }
        }
        /* Exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
    }
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    if ((XCP_IS_FLAG_SET(XCP_AUTONOMOUS_DISCONNECT_FLAG, Xcp_ConnectionStatus.Flags, uint8) == FALSE) &&
        (XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8) == TRUE))
    {
      Xcp_MonitorMulticastChange();
    }
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
  }

  DBG_XCP_PROCESSBUSMONITOR_EXIT();
}

FUNC(void, XCP_CODE) Xcp_PrepareForAutonomousDisconnect(boolean EmitSessionTerminatedEvent)
{
  /* Function shall be called inside Xcp exclusive area */
  PduIdType Iterator;

  /* Start clearing up all PDUs so that the EV_SESSION_TERMINATED is the only one queued */
  for(Iterator = 0U; (Iterator < Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].TxPduMax); Iterator++)
  {
    /* Clear the pending flags */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
    XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsPduWaitingForTriggFlag);
#endif
    XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsPduWaitingForConfFlag);
    XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsBufferFullFlag);
#if(XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_TxLPduData.IsMulticastFlag);
#endif
  }

#if(XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  if (XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8))
  {
    XCP_CLEAR_FLAG(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8);
    /* do not emit the session terminated event as multicast is active*/
    EmitSessionTerminatedEvent = FALSE;
  }
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

  if (Xcp_ConnectionStatus.State != XCP_STATE_DISCONNECTED)
  {
#if(XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* Send the Event Packet EV_SESSION_TERMINATED */
    if (EmitSessionTerminatedEvent == TRUE)
    {
      Xcp_EmitSessionTerminatedEvent();
    }
#else
    TS_PARAM_UNUSED(EmitSessionTerminatedEvent);
#endif /* XCP_EVENT_PACKET_ENABLED == STD_ON */

    /* Xcp slave decided to autonomously disconnect, schedule the disconnect */
    XCP_SET_FLAG(XCP_AUTONOMOUS_DISCONNECT_FLAG, Xcp_ConnectionStatus.Flags, uint8);
  }
}


/* If TriggerTransmit function will be called by lower layer */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/*-----------------------------[Xcp_ProvideTxData]--------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_ProvideTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) TxBufferPtr;
  PduLengthType TxBufferLength;
  /* get the ID of Tx LPdu buffer */
  PduIdType IntPduId = Xcp_TxLPduIdConf[TxPduId].IntPduBufId;

  DBG_XCP_PROVIDETXDATA_ENTRY(TxPduId);

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsPduWaitingForTriggFlag) == TRUE)
  {
    /* get PDU start position inside the LPdu buffer */
    TxBufferPtr = XcpGetResTxPduInfoPtr(TxPduId);
    /* get the PDU length */
    TxBufferLength = XCP_GET_RES_TX_PDU_INFO_LENGTH(IntPduId);
    /* Copy data to be transmitted */
    TS_MemCpy(PduInfoPtr->SduDataPtr, TxBufferPtr, TxBufferLength);
    /* Update the Pdu length */
    PduInfoPtr->SduLength = TxBufferLength;
    /* clear the "waiting for trigger" flag to prevent sending the same data twice */
    XCP_CLEAR_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsPduWaitingForTriggFlag);
    /* set flag to indicate that the Pdu is waiting for confirmation */
    XCP_SET_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsPduWaitingForConfFlag);
    /* signal that the data was successfully written */
    RetVal = E_OK;
  }

  /* Exit critical section.*/
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROVIDETXDATA_EXIT(RetVal,PduInfoPtr);

  return RetVal;
}

#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

/*-----------------------------[Xcp_IndicateTxConfirmation]--------------------------*/

FUNC(void, XCP_CODE) Xcp_IndicateTxConfirmation
(
  PduIdType TxPduId
)
{
  /* get the ID of Tx LPdu buffer */
  PduIdType IntPduId = Xcp_TxLPduIdConf[TxPduId].IntPduBufId;
#if (XCP_TRANSMIT_FROM_CONFIRMATION == STD_ON)
  boolean isTxIdle = FALSE;
#endif
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  boolean isUnicastPending = FALSE;
#endif

  DBG_XCP_INDICATETXCONFIRMATION_ENTRY(TxPduId);

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsPduWaitingForConfFlag) == TRUE)
  {
    /* clear the waiting for confirmation flag to prevent Tx indication twice */
    XCP_CLEAR_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsPduWaitingForConfFlag);
    /* clear flag to indicate that the Tx buffer is ready for new data */
    XCP_CLEAR_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsBufferFullFlag);
    /* Clear the bus timeout counter */
    Xcp_TxLPduData.BusTimeoutCntVal[IntPduId] = 0U;
#if (XCP_TX_BUS_RETRY_COUNT > 0U)
    /* Clear the tx bus retry counter */
    Xcp_TxLPduData.BusTxRetryCount[IntPduId] = 0U;
#endif
#if (XCP_TRANSMIT_FROM_CONFIRMATION == STD_ON)
    /* Tx is now idle */
    isTxIdle = TRUE;
#endif
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    if(XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsMulticastFlag) == TRUE)
    {
      /* set flag isUnicastPending - this will trigger request to SoAd switch to
      unicast address and port */
      isUnicastPending = TRUE;
    }
#endif
  }
  /* Exit critical section.*/
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* Verify if a change from multicast to unicast address is required */
  if (isUnicastPending == TRUE)
  {
    /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.UnicastChange,1 */
    if (Xcp_SoAdChangeTxToUnicast() == E_OK)
    {
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      /* clear the Multicast flag in the connection status if the request is successful */
      XCP_CLEAR_FLAG(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8);
      XCP_CLEAR_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsMulticastFlag);
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    }
  }
#endif
#if (XCP_TRANSMIT_FROM_CONFIRMATION == STD_ON)
  /* check if transmission from Tx confirmation is enabled for this PDU */
  if ((TRUE == isTxIdle) &&
      (TRUE == XCP_IS_FLAG_SET(XCP_SUPPORT_TX_FROM_CONFIRMATION,
               Xcp_TxLPduIdConf[TxPduId].Properties,
               uint8)))
  {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastMode,1 */
    if (Xcp_IsAllowedToTransmitMulticast(IntPduId) == TRUE)
#endif
    {
      /* update buffers and trigger transmission for same PDU */
      Xcp_UpdateAvailableTxBuffers(IntPduId);
      /* send available Transmit Pdus */
      Xcp_TransmitAvailableTxPdus(XCP_SID_IF_TX_CONFIRMATION, IntPduId);
    }
  }
#endif

  DBG_XCP_INDICATETXCONFIRMATION_EXIT();
}

#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
/*-----------------------------[Xcp_EnableTxCommunicationChannel]--------------------------*/
FUNC(uint8, XCP_CODE) Xcp_EnableTxCommunicationChannel
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr
)
{
  uint8 RetVal = E_NOT_OK;
  PduIdType TxPduId = ParamPtr[XCP_DYNAMIC_PDU_CMD_PDUID];
  /* pointer used to select the locked connection resource */
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_ENTRY(ParamPtr);
#if (XCP_NO_OF_CONNECTIONS > 1U)
  /* CHECK: NOPARSE */
  if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
  /* CHECK: PARSE */
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
  {
    /* select the connection configuration */
    ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
    /* Check if requested PduId is valid */
    if (TxPduId >  ConnectionCfgPtr->TxPduMax)
    {
      /* return E_NOT_OK */
      RetVal = E_NOT_OK;
    }
    else if (ParamPtr[XCP_DYNAMIC_PDU_CMD_STATE] == XCP_DYNAMIC_PDU_CMD_STATE_ENABLED)
    {
      /* enable the requested PDU */
      XCP_SET_BYTE_FLAG(Xcp_TxLPduIdConf[TxPduId].IntPduBufId, Xcp_TxLPduData.IsLPduEnabled);
      RetVal = E_OK;
    }
    else
    {
      /* disable the requested PDU */
      XCP_CLEAR_BYTE_FLAG(Xcp_TxLPduIdConf[TxPduId].IntPduBufId, Xcp_TxLPduData.IsLPduEnabled);
      RetVal = E_OK;
    }
  }
#if(XCP_NO_OF_CONNECTIONS > 1U)
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable branch.In case of an invalid Xcp_ConnectionStatus.ConnectionId
     * (XCP_INVALID_CONNECTION_ID) the command will not be inserted into the command queue because
     * the described condition is verified before in Xcp_ProcessReception() in case the reception
     * of the command is made from Xcp_MainFunction or, in case the XcpPduSupportRxFromRxIndication
     * is enabled, the Xcp_ConnectionStatus.ConnectionId is updated to the RxLPdu's internal connectionId.
     */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */
#endif
  DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_EXIT(RetVal, ParamPtr);
  return RetVal;
}
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */

/*-----------------------------[Xcp_InitTxPduFlags]------------------*/
FUNC(void, XCP_CODE) Xcp_InitTxPduFlags
(
  uint8 ConnectionId
)
{
  PduIdType Iterator;

  /* CHECK: NOPARSE */
  if(ConnectionId != XCP_INVALID_CONNECTION_ID)
  /* CHECK: PARSE */
  {
    for(Iterator = 0U; Iterator < XCP_NO_OF_TX_PDU_IDS; Iterator++)
    {
      if (Xcp_TxLPduIdConf[Iterator].IntConnectionId == ConnectionId)
      {
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
        /* If PDU is dynamic and default state is enabled update runtime properties of the pdu */
        if ((TRUE == XCP_IS_FLAG_SET
                    (XCP_DYNAMIC_TX_PDU, Xcp_TxLPduIdConf[Iterator].Properties,uint8)) &&
            (TRUE == XCP_IS_FLAG_SET
                    (XCP_ENABLE_STATE_DYNAMIC_TX_PDU, Xcp_TxLPduIdConf[Iterator].Properties,uint8)))
        {
          XCP_SET_BYTE_FLAG(Xcp_TxLPduIdConf[Iterator].IntPduBufId, Xcp_TxLPduData.IsLPduEnabled);
        }
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED = STD_ON) */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

        if (Xcp_ConnectionCfg[ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
        {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
          if (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_DAQ_DTO_PDU, Xcp_TxLPduIdConf[Iterator].Properties, uint8) == TRUE)
          {
            /* Initialize whether DAQ packet type is accepted by this PDU */
            XCP_SET_BYTE_FLAG(Xcp_TxLPduIdConf[Iterator].IntPduBufId, Xcp_TxLPduData.LPDUPacketStatusDaq);
          }
#endif
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
          if (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_EVSERV_CTO_PDU, Xcp_TxLPduIdConf[Iterator].Properties, uint8) == TRUE)
          {
            /* Initialize wether EV_SERV packet type is accepted by this PDU */
            XCP_SET_BYTE_FLAG(Xcp_TxLPduIdConf[Iterator].IntPduBufId, Xcp_TxLPduData.LPDUPacketStatusEvServ);
            /* Initially EV_SERV packet type is deactivated */
            XCP_CLEAR_BYTE_FLAG(Xcp_TxLPduIdConf[Iterator].IntPduBufId, Xcp_TxLPduData.LPDUPacketActivateEvServ);
          }
#endif
        }
#endif /*(XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
      }
    }
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Defensive programming makes it impossible to test when connection id is invalid as
     * the generated connection ID for each PDU is valid. */
  }
  /* CHECK: PARSE */
}

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusDaq
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
)
{
  /* Function must be called within Xcp critical section */
  if (ShouldBeEnabled == TRUE)
  {
    /* enable DAQ */
    XCP_SET_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketStatusDaq);
  }
  else
  {
    /* disable DAQ */
    XCP_CLEAR_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketStatusDaq);
  }
}
#endif /* XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK*/

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusEvServ
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
)
{
  /* Function must be called within Xcp critical section */
  if (ShouldBeEnabled == TRUE)
  {
    /* enable EV_SERV packet type (SERV is currently not implemented) */
    XCP_SET_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketStatusEvServ);
  }
  else
  {
    /* disable EV_SERV packet type (SERV is currently not implemented) */
    XCP_CLEAR_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketStatusEvServ);
  }
}

FUNC(void, XCP_CODE) Xcp_SetPacketActivateStatusEvServ
(
  PduIdType FlagID,
  boolean ShouldBeActivated
)
{
  if (ShouldBeActivated == TRUE)
  {
    /* enable EV_SERV packet type (SERV is currently not implemented) */
    XCP_SET_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketActivateEvServ);
  }
  else
  {
    /* disable EV_SERV packet type (SERV is currently not implemented) */
    XCP_CLEAR_BYTE_FLAG(FlagID, Xcp_TxLPduData.LPDUPacketActivateEvServ);
  }
}

FUNC(boolean, XCP_CODE) Xcp_GetPacketStatusEvServEnabled
(
  uint8 TxPduIdx
)
{
  return XCP_IS_BYTE_FLAG_SET(Xcp_TxLPduIdConf[TxPduIdx].IntPduBufId, Xcp_TxLPduData.LPDUPacketStatusEvServ);
}

#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

FUNC(void, XCP_CODE) Xcp_SetTxFlxBufferLength(uint8 bufferId, uint8 newLength)
{
  /* Function must be called within Xcp critical section */
  uint8 Iterator;
  uint16 startAddress = 0U;

  /* Reset all buffers so that unexpected data is not being sent during reconfiguration */
  TS_MemSet(Xcp_TxLPduData.IsBufferFullFlag, 0U, XCP_TX_PDU_FLAG_SIZE);
  TS_MemSet(Xcp_TxLPduData.IsPduWaitingForConfFlag, 0U, XCP_TX_PDU_FLAG_SIZE);
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
  TS_MemSet(Xcp_TxLPduData.IsPduWaitingForTriggFlag, 0U, XCP_TX_PDU_FLAG_SIZE);
#endif

  /* set new length */
  Xcp_TxLPduData.LPduMaxLenBuf[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[bufferId].FlxPduIdConfIdx].IntPduBufId] = newLength;

  for (Iterator=0; Iterator<XCP_NO_OF_FLX_BUF_CONFIGURED; Iterator++)
  {
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[Iterator].Flags,uint8) == TRUE)
    {
      Xcp_TxLPduData.LPduStartPos[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] = startAddress;

      /* set the value from runtime  */
      startAddress += Xcp_TxLPduData.LPduMaxLenBuf[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId];
    }
  }
}


FUNC(uint8, XCP_CODE) Xcp_GetTxFlxBufferLength(uint8 bufferId)
{
   return Xcp_TxLPduData.LPduMaxLenBuf[Xcp_TxLPduIdConf[Xcp_FlxBufConfig[bufferId].FlxPduIdConfIdx].IntPduBufId];
}

FUNC(Std_ReturnType, XCP_CODE) Xcp_SetDefaultLpduToBuffer(uint8 bufferId, uint8 bufferLength)
{
  Std_ReturnType RetValue = E_NOT_OK;
  /* initialize only buffers with preconfigured values */
  if(XCP_IS_FLAG_SET(XCP_BUFFER_PRECONFIGURED_MASK, Xcp_FlxBufConfig[bufferId].Flags,uint8) == TRUE)
  {
    /* calculate the HEADER_CRC */
    uint16 HeaderCrc = Xcp_FlxCalculateHeaderCrc
               (
                 /* frame Id */
                 Xcp_FlxBufConfig[bufferId].FlxBufSlotIDInitValue,
                 /* payload length in words */
                 (Xcp_FlxBufConfig[bufferId].FlxBufLengthInitValue / 2U)
               );
    /* Configure the Flexray Hw Buffer with preconfigured values for the LPDU_ID */
    /* !LINKSTO Xcp833.1,1 */
    RetValue =
          FrIf_ReconfigLPdu
          (
            Xcp_FlxBufConfig[bufferId].FlxCtrlIdx,                              /* FrIf_CtrlIdx */
            Xcp_FlxBufConfig[bufferId].FlxLPDU_ID,                              /* FrIf_LPduIdx */
            Xcp_FlxBufConfig[bufferId].FlxBufSlotIDInitValue,                   /* FrIf_FrameId */
            (Fr_ChannelType)Xcp_FlxBufConfig[bufferId].FlxBufChannelInitValue,  /* FrIf_ChnlIdx */
            Xcp_FlxBufConfig[bufferId].FlxBufCycleRepetitionInitValue,          /* FrIf_CycleRepetition */
            Xcp_FlxBufConfig[bufferId].FlxBufOffsetInitValue,                   /* FrIf_CycleOffset */
            bufferLength,                                                       /* FrIf_PayloadLength */
            HeaderCrc                                                           /* FrIf_HeaderCRC */
          );
  }
  else
  {
    RetValue = E_OK;
  }
  return RetValue;
}


#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */


/*==================[internal function definitions]=========================*/

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
/*-----------------------------[Xcp_MonitorFlexRayBuffers]------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_MonitorFlexRayBuffers(void)
{
  uint8 FrIf_Cycle;
  uint8 i;
  uint16 FrIf_MacroTick;

  /* FrIf_GetGlobalTime returns E_NOT_OK it means that the Flexray is in asynchronous state
   * or other issues occurred. What this means to us is that the buffers which are configured with
   * parameter "FrIfReconfigurable" are not implicitly initialized by FrIf and must be initialized
   * by us each time FrIf_GetGlobalTime() returns E_NOT_OK
   */
  if (FrIf_GetGlobalTime(0U, &FrIf_Cycle, &FrIf_MacroTick) == E_OK)
  {
    /* if the FrIf state is OK and the buffers are not yet initialized, initialize them */
    if (XCP_IS_FLAG_SET(XCP_FLEXRAY_BUFFERS_READY_FLAG, Xcp_ConnectionStatus.Flags, uint8) == FALSE)
    {
      boolean AllBuffersWereConfigured = TRUE;

      for (i=0; i<XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
      {
        if ( Xcp_SetDefaultLpduToBuffer(i, Xcp_FlxBufConfig[i].FlxBufLengthInitValue) != E_OK)
        {
          AllBuffersWereConfigured = FALSE;
        }
      }
      /* Check that all buffers were accepted by FrIf to be configured */
      if (AllBuffersWereConfigured == TRUE)
      {
        /* Mark the connection as ready, buffers can be used */
        XCP_SET_FLAG_U8_ATOMIC(XCP_FLEXRAY_BUFFERS_READY_BIT, Xcp_ConnectionStatus.Flags);
#if (XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT == STD_ON)
        Xcp_TxLPduData.Xcp_FlexTimeoutCnt = 0U;
#endif
      }
    }
    else
    {
      /* do nothing - buffers are initialized and FrIf is up and running */
    }
  }
  else
  {
    /* FlexRay state is not OK, we need to clear the flag that the buffers are ready */
    XCP_CLEAR_FLAG_U8_ATOMIC(XCP_FLEXRAY_BUFFERS_READY_BIT, Xcp_ConnectionStatus.Flags);
#if (XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT == STD_ON)
    if (Xcp_TxLPduData.Xcp_FlexTimeoutCnt < XCP_FLEXRAY_SYNC_TO_COUNTER_MAX_VAL)
    {
      /* increment the FlexTimeout Timeout Counter's value until it reaches the maximum configuration value */
      Xcp_TxLPduData.Xcp_FlexTimeoutCnt++;
    }
    else
    {
      Xcp_TxLPduData.Xcp_FlexTimeoutCnt = 0U;
      Xcp_PrepareForAutonomousDisconnect(FALSE);

     /* Do a partial state reset so that further connections can be accepted */
     if (Xcp_ConnectionStatus.State == XCP_STATE_DISCONNECTED)
     {
       /* Clear receive processor's IsBufferFullFlag */
       Xcp_InitReceiveProcessor();
#if (XCP_NO_OF_CONNECTIONS > 1U)
       Xcp_ProcessResourcesUnlock();
#endif
     }
    }
#endif /* (XCP_FLEXRAY_SYNC_TIMEOUT_SUPPORT == STD_ON) */
  }
}

STATIC FUNC(uint16, XCP_CODE) Xcp_FlxCalculateHeaderCrc(uint16 FrameId, uint8 PayloadLength)
{
  uint16 HeaderCRC = 0x1A;
  /* no sync frame */
  uint8 SyncFlag = 0;
  /* no startup frame */
  uint8 SupFlag = 0;

  uint32 Header = (((uint32)SyncFlag)<<19U)|(((uint32)SupFlag)<<18U)|(((uint32)FrameId) << 7U)|((uint32)PayloadLength);

  HeaderCRC = Xcp_FlxCrcCalcNibble(HeaderCRC, (uint8)((Header >> 16U) & 0x0FU));
  HeaderCRC = Xcp_FlxCrcCalcNibble(HeaderCRC, (uint8)((Header >> 12U) & 0x0FU));
  HeaderCRC = Xcp_FlxCrcCalcNibble(HeaderCRC, (uint8)((Header >> 8U) & 0x0FU));
  HeaderCRC = Xcp_FlxCrcCalcNibble(HeaderCRC, (uint8)((Header >> 4U) & 0x0FU));
  HeaderCRC = Xcp_FlxCrcCalcNibble(HeaderCRC, (uint8)((Header >> 0U) & 0x0FU));

  return HeaderCRC;
}


STATIC FUNC(uint16, XCP_CODE) Xcp_FlxCrcCalcNibble(uint16 tmpCRC, uint8 Nibble)
{
  uint16 tempValue = ((uint16)Nibble & 0xFU);

  return (uint16)(((uint16)((uint16)(tmpCRC << 4U) & 0x7FFU)) ^ Xcp_HeaderCrcTable[tempValue ^ ((uint16)(tmpCRC>>7U)&0xFU)]);
}


#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/



/*-----------------------------[Xcp_UpdateAvailableTxBuffers]--------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_UpdateAvailableTxBuffers
(
  PduIdType IntTxPduId
)
{
  DBG_XCP_UPDATEAVAILABLETXBUFFERS_ENTRY(IntTxPduId);

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* if Tx buffer is available, try to fill it with Pdu data */
  /* !LINKSTO Xcp.BswDistribution.Communication.Tx.Buffers.CoreLocal,1 */
  if(XCP_IS_BYTE_FLAG_SET(IntTxPduId, Xcp_TxLPduData.IsBufferFullFlag) == FALSE)
  {
    /* prepare Tx buffer data */
    if(E_OK == Xcp_PackTxBufferData(IntTxPduId))
    {
      /* indicate that new data is available to be transmitted */
      XCP_SET_BYTE_FLAG(IntTxPduId, Xcp_TxLPduData.IsBufferFullFlag);
    }
  }

  /* Exit critical section.*/
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_UPDATEAVAILABLETXBUFFERS_EXIT();
}

/*-----------------------------[Xcp_TransmitAvailableTxPdus]--------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_TransmitAvailableTxPdus
(
  uint8 SidId,
  PduIdType IntTxPduId
)
{
  /* local parameter used to store the PduId of the lower layer */
  PduIdType DestTxPduId;
  /* select the connection configuration */
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  /* Variable representing the transmission status. This variable is used to process
   * the retry mechanism. We consider the following situations:
   * - E_OK: when there is nothing to transmit or the transmission was accepted
   *   by the underlying communication layer(no retry necessary).
   * - any other error code returned by the underlying communication layer when
   *   the transmission was rejected by the underlying communication layer
   *   (process the retry mechanism calculation). */
  Std_ReturnType PduTxStatus;
  boolean AbortTransmission = FALSE;
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  boolean IsUnicastPending = FALSE;
#endif
  PduInfoType TxPduInfo;

  DBG_XCP_TRANSMITAVAILABLETXPDUS_ENTRY(SidId, IntTxPduId);

#if (XCP_PROD_ERR_HANDLING_RETRY_FAILED != TS_PROD_ERR_REP_TO_DET)
  TS_PARAM_UNUSED(SidId);
#endif

  /* Prepare PduInfo pointer */
  TxPduInfo.SduDataPtr = XcpGetResTxPduInfoPtr(ConnectionCfgPtr->TxPduIdList[IntTxPduId]);
  TxPduInfo.SduLength = XCP_GET_RES_TX_PDU_INFO_LENGTH(IntTxPduId);
  /* get the Transmit pdu id */
  DestTxPduId = Xcp_TxLPduIdConf[ConnectionCfgPtr->TxPduIdList[IntTxPduId]].DestPduId;

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* !LINKSTO Xcp.BswDistribution.Communication.Tx.CoreLocal,1 */
  /* !LINKSTO Xcp.BswDistribution.Communication.PduData.CoreLocal,1 */
  PduTxStatus = XCP_GET_PDU_TRANSMIT_STATUS(IntTxPduId);

  /* check the status of the Tx PDU and if ready lock other Tx requests */
  if (PduTxStatus == E_OK)
  {
    Xcp_SetPduTransmitStatus(IntTxPduId, E_OK);
  }

  /* Exit critical section.*/
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if(PduTxStatus == E_OK)
  {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    boolean OkToTransmit = TRUE;
    if((XCP_IS_BYTE_FLAG_SET(IntTxPduId, Xcp_TxLPduData.IsMulticastFlag) == TRUE) &&
       (XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8) ==
          FALSE))
    {
      /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastChange,1 */
      if (Xcp_SoAdChangeTxToMulticast() == E_OK)
      {
        XCP_SET_FLAG_U8_ATOMIC(XCP_MULTICAST_TRANSMIT_MODE_BIT, Xcp_ConnectionStatus.Flags);
      }
      else
      {
        /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastChangeUnsuccessful,1 */
        OkToTransmit = FALSE;
        /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastTransmitFailed,1 */
        /* set transmission failure; regular Tx retry mechanism will be performed */
        PduTxStatus = E_NOT_OK;
      }
    }
    if(OkToTransmit)
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
    {
      /* the call has to be done outside Xcp critical section.
       * Leave <If> interface to implement that mechanism. */
      PduTxStatus = Xcp_Transmit(DestTxPduId, &TxPduInfo);
    }

    /* Enter critical section.*/
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    if (PduTxStatus != E_OK)
    {
      /* update transmission flags (TriggerTx and TxConfirmation) only if
      the status from the <IF>Transmit is not OK. In this way we are avoiding setting the
      waiting for confirmation flags twice. */
      Xcp_SetPduTransmitStatus(IntTxPduId, PduTxStatus);
    }
    /* process retry mechanism */
    Xcp_ProcessRetry(IntTxPduId, PduTxStatus, &AbortTransmission);
    if(AbortTransmission == TRUE)
    {
      /* configured number of retries performed - discard the message
         and make the buffer available for a new Tx Pdu */
      /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.TransmitUnsuccessfulRetry,1 */
      XCP_CLEAR_BYTE_FLAG(IntTxPduId, Xcp_TxLPduData.IsBufferFullFlag);
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
      /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.MulticastChangeUnsuccessfulRetry,1 */
      if(XCP_IS_BYTE_FLAG_SET(IntTxPduId, Xcp_TxLPduData.IsMulticastFlag) == TRUE)
      {
        if(XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8))
        {
          /* set flag to trigger change request to unicast address and port as transmit is aborted */
          IsUnicastPending = TRUE;
        }
        else
        {
          /* clear multicast flag for the PDU*/
          XCP_CLEAR_BYTE_FLAG(IntTxPduId, Xcp_TxLPduData.IsMulticastFlag);
        }
      }
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
    }
    /* Exit critical section.*/
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* if IsUnicastPending flag is set request change of the remote address  */
  if (IsUnicastPending == TRUE)
  {
    if (Xcp_SoAdChangeTxToUnicast() == E_OK)
    {
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      /* clear multicast flag for the connection */
      XCP_CLEAR_FLAG(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8);
      /* clear multicast flag for the PDU*/
      XCP_CLEAR_BYTE_FLAG(IntTxPduId, Xcp_TxLPduData.IsMulticastFlag);
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
      /* Reset multicast to unicast retry counter */
      Xcp_TxLPduData.MulticastRetryCnt[IntTxPduId] = 0U;
#endif
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    }
  }
#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */
#if (XCP_PROD_ERR_HANDLING_RETRY_FAILED != TS_PROD_ERR_DISABLE)
  if(AbortTransmission == TRUE)
  {
    /* the call has to be done outside Xcp critical section.
     * Leave Error manager to implement that mechanism. */
#if(XCP_PROD_ERR_HANDLING_RETRY_FAILED!= TS_PROD_ERR_DISABLE)
    XCP_REPORT_RETRY_FAILURE(SidId);
#endif
  }
#endif

  DBG_XCP_TRANSMITAVAILABLETXPDUS_EXIT();
}

#if (XCP_MAX_DAQ != 0U)
STATIC FUNC(boolean, XCP_CODE) Xcp_IsTxPacketSizeOk(uint8 HandlerId)
{
  boolean PacketSizeOk = FALSE;
  if (XCP_SUPPORT_FOR_DAQ_DTO_PDU == Xcp_TxPduDataHandlers[HandlerId]->PackageType)
  {
    if (Xcp_TxPduDataHandlers[HandlerId]->GetPduLength() <= Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Dto)
    {
      PacketSizeOk = TRUE;
    }
    else
    {
      Xcp_ClearDTOEntryFromDaqPrioQueue();
#if(XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST!= TS_PROD_ERR_DISABLE)
      /* !LINKSTO Xcp.Acquisition.DataLostError,1 */
      XCP_REPORT_ACQUISITION_DATA_LOST(XCP_SID_UPDATE_AVAILABLE_TX_BUFFERS);
#endif
    }
  }
  else
  {
    PacketSizeOk = TRUE;
  }
  return PacketSizeOk;
}
#endif

/*-----------------------------[Xcp_PackTxBufferData]-----------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_PackTxBufferData
(
  PduIdType TxBufferId
)
{
  /* initialize return value to worst case value */
  Std_ReturnType RetVal = E_NOT_OK;
  /* select the Tx buffer */
  /* !LINKSTO Xcp.BswDistribution.Communication.ConnectionData.CoreLocal,1 */
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  /* get the TxPduId assigned to this Tx PDU buffer */
  const PduIdType TxPduId = ConnectionCfgPtr->TxPduIdList[TxBufferId];
  /* get the  package header */
  const PduLengthType HeaderLength = ConnectionCfgPtr->PduHeaderLength;
  /* get the data pointer for the Tx Pdu buffer */
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) TxBufferPtr =
      XcpGetResTxPduInfoPtr(TxPduId);
  /* length of all packed data */
  PduLengthType TxPayloadSegmentSize = 0U;
#if ((XCP_ON_ETHERNET_ENABLED == STD_ON) || \
     ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && \
     ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))))
  boolean FirstPacket = TRUE;
  /* Used to decide if header counter is to be incremented */
  boolean incrCtr = TRUE;
#endif
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  boolean skipPackage = FALSE;
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

  /* Initialize available space into Tx frame */
  PduLengthType AvailableTxSpace;

  /* handler ID */
  uint8 Xcp_TxPduHandlerId = 0U;
  /* The Payload length for the selected message to be packed */
  PduLengthType PackageLength;
  /* Message length for the selected PDU to be packed */
  PduLengthType MessageLength;

  DBG_XCP_PACKTXBUFFERDATA_ENTRY(TxBufferId);

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
  if (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
  {
    /* initialize FlexRay header for Tx PDU */
    Xcp_InitFlexRayBuffer(TxBufferPtr);
  }
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

  /* calculate the available space */
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  if (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
  {
    AvailableTxSpace = Xcp_TxLPduData.LPduMaxLenBuf[TxBufferId];
  }
  else
#endif
  {
    AvailableTxSpace = ConnectionCfgPtr->PduLengthMax;
  }

  /* fill up the Tx send buffer */
  do
  {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    skipPackage = FALSE;
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
    /* check if the selected package type is supported by the Tx PDU */
    if (Xcp_IsTxPacketAllowed(Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->PackageType, TxPduId) == TRUE)
    {
#if (XCP_MAX_DAQ != 0U)
      boolean IsTxPacketSizeOk = FALSE;
#endif
      /* Check whether any response for the selected queue is available */
      boolean IsPduQueueEmpty = Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->IsPduQueueEmpty();
#if (XCP_MAX_DAQ != 0U)
      if (IsPduQueueEmpty == FALSE)
      {
        IsTxPacketSizeOk = Xcp_IsTxPacketSizeOk(Xcp_TxPduHandlerId);
      }
#endif
#if (XCP_MAX_DAQ != 0U)
      if ((IsPduQueueEmpty == FALSE) && (IsTxPacketSizeOk == TRUE))
#else
      if (IsPduQueueEmpty == FALSE)
#endif
      {
        PduLengthType headerLengthLocal;
        /* Get Length of pdu data */
        PackageLength = Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->GetPduLength();
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
        if ((FirstPacket == FALSE) && (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY))
        {
          /* In the case of FlexRay the subsequent concatenated messages
             has only the LEN field as header, so one byte only */
          headerLengthLocal = 1U;
        }
        else
#endif
        {
          headerLengthLocal = HeaderLength;
        }

        MessageLength = headerLengthLocal + PackageLength;

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
        if((ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY) &&
           (XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG, ConnectionCfgPtr->Properties, uint8) == TRUE)
        )
        {
          PduLengthType NumberOfBytesAllignment = ConnectionCfgPtr->FlexRayPackageAlignment;
          PduLengthType MessageUnalignedBytes = PackageLength % NumberOfBytesAllignment;
          if (MessageUnalignedBytes > 0U)
          {
            if (AvailableTxSpace >= (MessageLength + NumberOfBytesAllignment - MessageUnalignedBytes))
            {
              TS_MemSet(&TxBufferPtr[MessageLength], XCP_RESERVED_BYTE, ((usize)NumberOfBytesAllignment - (usize)MessageUnalignedBytes));
            }
            MessageLength += NumberOfBytesAllignment - MessageUnalignedBytes;
          }
        }
#endif

        /* Check whether next PDU fits in the available space.
         * For FlexRay: It is not necessary to take into consideration the tail because the
         *              frame/PDU size is always even. */
        if (AvailableTxSpace >= MessageLength)
        {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
          if (Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->PackageType == XCP_SUPPORT_FOR_RESERR_CTO_PDU)
          {
            /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.XcpPackMultiMsgInOneEthernetFrame,1 */
            if ((Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->IsPduMulticast()) &&
                (AvailableTxSpace == ConnectionCfgPtr->PduLengthMax))
            {
              /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID005,1 */
              incrCtr = FALSE;
              XCP_SET_BYTE_FLAG(TxBufferId, Xcp_TxLPduData.IsMulticastFlag);
              /* AvailableTxSpace is the total length supported by the connection
                 Pack only the multicast message*/
              AvailableTxSpace = MessageLength;
              RetVal = E_OK;
            }
            else if ((Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->IsPduMulticast())&&
                     (AvailableTxSpace < ConnectionCfgPtr->PduLengthMax))
            {
              /* some packages have already been packed and a multicast message is identified
                 stop packing so the multicast message will be sent independently on the next cycle */
                 skipPackage = TRUE;
            }
            else
            {
              /* Continue normal processing */
            }
          }
          if (skipPackage == FALSE)
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
          {
#if ((XCP_ON_ETHERNET_ENABLED == STD_ON) || \
    ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && \
        ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))))
          /* write the header into the frame */
            Xcp_AddFrameHeader(PackageLength, TxBufferPtr, headerLengthLocal, FirstPacket, incrCtr);
#endif
            /* select the data position in Pdu package */
            TxBufferPtr = &TxBufferPtr[headerLengthLocal];
            /* Dequeue PDU from the selected queue to transmit buffer */
            Xcp_TxPduDataHandlers[Xcp_TxPduHandlerId]->GetPduData(PackageLength, TxBufferPtr);
#if( (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) )
          /* check if the package is EvResumeMode */
            if( (TxBufferPtr[0U] == XCP_EV_PID) &&
                (TxBufferPtr[1U] == XCP_EV_RESUME_MODE) )
            {
              XCP_SET_BYTE_FLAG(TxBufferId,Xcp_TxLPduData.IsEvResumeModePendingFlag);
            }
#endif
#if (XCP_ON_CANFD_ENABLED == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
            if (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_CANFD)
#endif
            {
              /* verify CAN-FD configuration parameters and add required FILL bytes */
              Xcp_AddCanFDFill(TxBufferPtr, &MessageLength, AvailableTxSpace);
            }
#endif /* (XCP_ON_CANFD_ENABLED == STD_ON) */
            /* select the address where the next package will be packed (we already moved the position
             * with the header length, now move it at the end at the message)*/
            TxBufferPtr = &TxBufferPtr[MessageLength - headerLengthLocal];
            /* Update transmit data length */
            TxPayloadSegmentSize += MessageLength;
            /* Update frame available space */
#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
            if (XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
                ConnectionCfgPtr->Properties,
                uint8) != TRUE)
            {
              /* abort packing because only one PDU in frame is supported */
              AvailableTxSpace = 0U;
            }
            else
#endif
            {
              /* update available space with the remaining free Pdu frame space */
              AvailableTxSpace -= MessageLength;
            }
#else
            /* abort packing because only one PDU in frame is supported */
            AvailableTxSpace = 0U;
#endif
            /* at least one PDU is packed, so set return value to OK */
            RetVal = E_OK;
          }
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
          else
          {
            /* select the next PDU handler set */
            Xcp_TxPduHandlerId++;
          }
#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */
        }
        else
        {
          /* select the next PDU handler set */
          Xcp_TxPduHandlerId++;
        }
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
        FirstPacket = FALSE;
#endif
      }
      else
      {
        /* select the next PDU handler set */
        Xcp_TxPduHandlerId++;
      }
    }
    else
    {
      /* select the next PDU handler set */
      Xcp_TxPduHandlerId++;
    }
  }
  while((Xcp_TxPduHandlerId < XCP_TX_MAX_QUEUE_ID) && (AvailableTxSpace > HeaderLength));

  if(RetVal == E_OK)
  {
#if(XCP_ON_FLEXRAY_ENABLED == STD_ON)

#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)

    /* Add a last message with only a header with LEN = 0 */
    if (Xcp_AddFlexRayMultiPduInFrameLastMessage(AvailableTxSpace, TxBufferPtr) == TRUE)
    {
      /* update Tx package size */
      TxPayloadSegmentSize += 1U;
      /* An empty message was added, move payload pointer to the next byte */
      TxBufferPtr = &TxBufferPtr[1U];
    }

#endif /* (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */

    /* Check if the full payload needs to be rounded up */
#if(XCP_NO_OF_CONNECTIONS > 1U)
    if (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      if((TxPayloadSegmentSize & 1U) == 1U)
      {
        /* add tail */
        TxBufferPtr[0U] = XCP_RESERVED_BYTE;
        /* update Tx package size */
        TxPayloadSegmentSize++;
      }
    }

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */
    /* update size of stored package into Tx package */
    XCP_SET_RES_TX_PDU_INFO_LENGTH(TxBufferId, TxPayloadSegmentSize);
  }

  DBG_XCP_PACKTXBUFFERDATA_EXIT(RetVal);

  return RetVal;
}

STATIC FUNC(boolean, XCP_CODE) Xcp_IsTxPacketAllowed
(
  uint8 PackageType,
  PduIdType TxPduId
)
{
  /* Function must be called within Xcp critical section */
  boolean retValue = FALSE;
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
  {
    if (PackageType == XCP_SUPPORT_FOR_RESERR_CTO_PDU)
    {
      /* the RESERR packet type cannot be changed at runtime so get the allowed one from configuration */
      retValue = XCP_IS_FLAG_SET(PackageType, Xcp_TxLPduIdConf[TxPduId].Properties, uint8);
    }

#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
    /* Get the value from the runtime bitfield */
    if (PackageType == XCP_SUPPORT_FOR_DAQ_DTO_PDU)
    {
      /* the DAQ packet type is changed at runtime so get the allowed one from the bitfield */
      if  (XCP_IS_BYTE_FLAG_SET(Xcp_TxLPduIdConf[TxPduId].IntPduBufId, Xcp_TxLPduData.LPDUPacketStatusDaq) == TRUE)
      {
        retValue = Xcp_IsFlxDaqIdAllowedForTheBuffer(TxPduId, PackageType, XCP_GET_DAQ_ID_FROM_PRIO(0U));
      }
    }
#endif


#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* Get the value from the runtime bitfield */
    if (PackageType == XCP_SUPPORT_FOR_EVSERV_CTO_PDU)
    {
      /* the EV_SERV packet type is changed at runtime so get the allowed one from the bitfield */
      if (XCP_IS_BYTE_FLAG_SET(Xcp_TxLPduIdConf[TxPduId].IntPduBufId, Xcp_TxLPduData.LPDUPacketStatusEvServ) == TRUE)
      {
        retValue = (XCP_IS_BYTE_FLAG_SET(Xcp_TxLPduIdConf[TxPduId].IntPduBufId, Xcp_TxLPduData.LPDUPacketActivateEvServ) == TRUE);
      }
      else
      {
        retValue = FALSE;
      }
    }
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */
  }
  else
#endif /* ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)) */
  {
    /* the packet type cannot be changed at runtime so get the allowed one from configuration */
    retValue = XCP_IS_FLAG_SET(PackageType, Xcp_TxLPduIdConf[TxPduId].Properties, uint8);
  }
  return retValue;
}

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
/*-----------------------------[Xcp_InitFlexRayBuffer]------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_InitFlexRayBuffer
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
)
{
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  uint8 Iterator;

  DBG_XCP_INITFLEXRAYBUFFER_ENTRY();

  /* add the NAX address */
  TxBufferPtr[0U] = ConnectionCfgPtr->FlexRayNax;
  /* initialize the FlexRay header */
  for(Iterator = 1U; Iterator < ConnectionCfgPtr->PduHeaderLength; Iterator++)
  {
    TxBufferPtr[Iterator] = XCP_RESERVED_BYTE;
  }

  DBG_XCP_INITFLEXRAYBUFFER_EXIT(TxBufferPtr);

}
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if ((XCP_ON_ETHERNET_ENABLED == STD_ON) || \
     ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && \
     ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))))

/*-----------------------------[Xcp_AddFrameHeader]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_AddFrameHeader
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr,
  PduLengthType HeaderLength,
  boolean FirstPacket,
  boolean IncrementCnt
)
{
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];

  DBG_XCP_ADDFRAMEHEADER_ENTRY(DataLength,TxBufferPtr,HeaderLength,FirstPacket,IncrementCnt);

  switch(ConnectionCfgPtr->ConnectionType)
  {
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)

  case XCP_CONNECTION_OVER_FLEXRAY:
  {
#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      /* add the header only if packing of multiple PDUs
      is enabled for this connection */
      if(XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
                         ConnectionCfgPtr->Properties,
                         uint8) == TRUE)
      {
        /* write only the data Length into frame header */
        TxBufferPtr[HeaderLength - 1U] = (uint8)(DataLength);
      }

#endif  /* XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON */
#if (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON)
      /* add the sequence correction counter */
      if(XCP_IS_FLAG_SET(XCP_SEQUENCE_CORRECTION_FLAG,
                         ConnectionCfgPtr->Properties,
                         uint8) == TRUE)
      {
        /* Add only if it's not an empty length */
        if (DataLength !=0U)
        {
          /* Write the sequence counter only in the header of the first packet */
          /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderCounter002,1 */
          if (FirstPacket == TRUE)
          {
            /* write the frame transmit counter */
            /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderCounter001,1 */
            /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderTypes004,1 */
            /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderTypes005,1 */
            /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderTypes008,1 */
            /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageHeaderTypes009,1 */
            TxBufferPtr[XCP_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX] =
              (uint8)Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId];
          }
          /* Increment header frame transmit counter */
          Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId] += 1U;
          /* Since the counter is a 16bit, we need to overflow at 8bit for FlexRay*/
          Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId] %= 256U;
        }
      }
#endif  /* XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON */
      break;
  }

#endif /*XCP_ON_FLEXRAY_ENABLED == STD_ON */
#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
  case XCP_CONNECTION_OVER_UDPIP:
  case XCP_CONNECTION_OVER_TCPIP:
    /* Ethernet XCP header (4 bytes always Intel Format) */
    /* add data length information */
    TxBufferPtr[XCP_BYTE0_INDEX] = (uint8)DataLength;
    TxBufferPtr[XCP_BYTE1_INDEX] = (uint8)(DataLength >> 8U);
    /* add frame counter information */
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
    if (IncrementCnt == FALSE)
    {
      TxBufferPtr[XCP_BYTE2_INDEX] = 0U;
      TxBufferPtr[XCP_BYTE3_INDEX] = 0U;
    }
    else
#endif
    {
      TxBufferPtr[XCP_BYTE2_INDEX] = (uint8)Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId];
      TxBufferPtr[XCP_BYTE3_INDEX] = (uint8)(Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId] >> 8U);
      /* Increment Ethernet header frame transmit counter */
      Xcp_FrameTransmitCounter[ConnectionCfgPtr->IntCounterId] += 1U;
    }
    break;
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */
  default:
    /* nothing to do */
    break;
  }

#if ((XCP_ON_FLEXRAY_ENABLED == STD_OFF) || \
     (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF))
  TS_PARAM_UNUSED(HeaderLength);
#endif

#if (XCP_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_OFF)
  TS_PARAM_UNUSED(FirstPacket);
#endif

#if (XCP_ON_ETHERNET_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(IncrementCnt)
#elif (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_OFF)
  TS_PARAM_UNUSED(IncrementCnt)
#endif

  DBG_XCP_ADDFRAMEHEADER_EXIT(DataLength,TxBufferPtr,HeaderLength,FirstPacket,IncrementCnt);
}

#endif /* ((XCP_ON_ETHERNET_ENABLED == STD_ON) || \
        *  ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))) */

/*-----------------------------[Xcp_ProcessRetry]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProcessRetry
(
  PduIdType TxPduId,
  Std_ReturnType TxStatus,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) AbortTxPtr
)
{
  /* variable used to signal that the abort transmission event was reached */
  boolean AbortTransmission = TRUE;
#if (XCP_TX_RETRY_COUNT > 0)
  /* select retry counter */
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) RetryCntValPtr =
      &Xcp_TxLPduData.RetryCntVal[TxPduId];
#endif

  DBG_XCP_PROCESSRETRY_ENTRY(TxPduId, TxStatus);

  if(TxStatus == E_OK)
  {
#if( (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) )
    /* no error detected at transmission, so clear EvResumeModePending flag.*/
    XCP_CLEAR_BYTE_FLAG(TxPduId,Xcp_TxLPduData.IsEvResumeModePendingFlag);
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
#if (XCP_TX_RETRY_COUNT > 0)
    /* no error detected at transmission, so reset the value of retry
     * mechanism to default value */
    *RetryCntValPtr = XCP_TX_RETRY_COUNT;
#endif /* (XCP_TX_RETRY_COUNT > 0) */
    /* no error detected at transmission, so there is nothing to abort */
    AbortTransmission = FALSE;
  }
  else
  {
#if( (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) )
    if(XCP_IS_BYTE_FLAG_SET(TxPduId,Xcp_TxLPduData.IsEvResumeModePendingFlag) == TRUE)
    {
      /* never abort the EvResumeModePending response */
      AbortTransmission = FALSE;
    }
    else
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) && (XCP_EVENT_PACKET_ENABLED == STD_ON) */
    {
#if (XCP_TX_RETRY_COUNT > 0)
      /* compute abort event */
      if(*RetryCntValPtr > 0U)
      {
        /* Decrement the Retry Counter's value until reaches 0 */
        *RetryCntValPtr -= 1U;
        /* the abort event was not yet detected */
        AbortTransmission = FALSE;
      }
      else
      {
        /* aborting, reset the retry counter */
        *RetryCntValPtr = XCP_TX_RETRY_COUNT;
      }
#endif /* (XCP_TX_RETRY_COUNT > 0) */
    }
  }
#if ((XCP_TX_RETRY_COUNT == 0) &&             \
     ((XCP_STORE_DAQ_SUPPORTED == STD_OFF) || (XCP_EVENT_PACKET_ENABLED == STD_OFF)))
  TS_PARAM_UNUSED(TxPduId);
#endif
  /* update abort transmission signal */
  *AbortTxPtr = AbortTransmission;

  DBG_XCP_PROCESSRETRY_EXIT(AbortTxPtr);
}

/*-----------------------------[Xcp_Transmit]------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_CONST) PduInfoPtr
)
{
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  Std_ReturnType RetVal;

  DBG_XCP_TRANSMIT_ENTRY(TxPduId, PduInfoPtr);

  RetVal = ConnectionCfgPtr->TransmitFunctionPtr(TxPduId, PduInfoPtr);

  DBG_XCP_TRANSMIT_EXIT(RetVal);

  return RetVal;
}

/*-----------------------------[Xcp_SetPduTransmitStatus]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_SetPduTransmitStatus
(
  PduIdType IntBufId,
  Std_ReturnType TxStatus
)
{
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
  /* select the connection configuration */
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
#endif

  DBG_XCP_SETPDUTRANSMITSTATUS_ENTRY(IntBufId, TxStatus);

  if(TxStatus == E_OK)
  {
    /* change the state of the transmission to "in progress". */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
    /* Check that the PDU needs trigger transmit or not (immediate or decoupled transmission - in the case of FlexRay) */
    if(XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_TRIGGER_TRANSMIT,
                       Xcp_TxLPduIdConf[ConnectionCfgPtr->TxPduIdList[IntBufId]].Properties,
                       uint8) == TRUE)
    {
      /* !LINKSTO Xcp.Communication.FlexRay.DecoupledAccess,1 */
      /* !LINKSTO Xcp.Communication.CDD.DecoupledAccess,1 */
      XCP_SET_BYTE_FLAG(IntBufId, &(Xcp_TxLPduData.IsPduWaitingForTriggFlag));
    }
    else
#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
    {
      /* !LINKSTO Xcp.Communication.FlexRay.ImmediateAccess,1 */
      /* !LINKSTO Xcp.Communication.CDD.ImmediateAccess,1 */
      XCP_SET_BYTE_FLAG(IntBufId, &(Xcp_TxLPduData.IsPduWaitingForConfFlag));
    }
  }
  else
  {
    /* change the state of the transmission to "in available". */
#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
    /* Check that the PDU needs trigger transmit or not (immediate or decoupled transmission - in the case of FlexRay) */
    if(XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_TRIGGER_TRANSMIT,
                       Xcp_TxLPduIdConf[ConnectionCfgPtr->TxPduIdList[IntBufId]].Properties,
                       uint8) == TRUE)
    {
      /* !LINKSTO Xcp.Communication.FlexRay.DecoupledAccess,1 */
      XCP_CLEAR_BYTE_FLAG(IntBufId, &(Xcp_TxLPduData.IsPduWaitingForTriggFlag));
    }
    else
#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
    {
      /* !LINKSTO Xcp.Communication.FlexRay.ImmediateAccess,1 */
      XCP_CLEAR_BYTE_FLAG(IntBufId, &(Xcp_TxLPduData.IsPduWaitingForConfFlag));
    }
  }
  DBG_XCP_SETPDUTRANSMITSTATUS_EXIT();
}

/*-----------------------------[XcpGetResTxPduInfoPtr]------------------*/
STATIC FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) XcpGetResTxPduInfoPtr
(
  PduIdType IntPduId
)
{
  /* Get the value from configuration */
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) retValue =
    &(Xcp_TxLPduData.LPduBuffer[Xcp_TxLPduIdConf[IntPduId].LPduStartPos]);

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
  {
     /* Get the value at runtime */
     retValue = &(Xcp_TxLPduData.LPduBuffer[Xcp_TxLPduData.LPduStartPos[Xcp_TxLPduIdConf[IntPduId].IntPduBufId]]);
  }
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
  return retValue;
}

#if (XCP_NO_OF_CONNECTIONS > 1U)
/*-----------------------------[Xcp_ProcessResourcesUnlock]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProcessResourcesUnlock(void)
{
  PduIdType Iterator;
  boolean UnlockResources = TRUE;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_PROCESSRESOURCESUNLOCK_ENTRY();

  if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
  {
    /* select connection configuration */
    ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* check the status for available Tx buffers. */
    for(Iterator = 0U; Iterator < ConnectionCfgPtr->TxPduMax; Iterator++)
    {
      /* store pdu channel status */
      if((XCP_IS_TX_PDU_CHANNEL_ACTIVE(Iterator) == TRUE) ||
         XCP_IS_BYTE_FLAG_SET(Iterator, Xcp_TxLPduData.IsBufferFullFlag) == TRUE)
      {
        /* delay unlock, because the transmission is pending */
        UnlockResources = FALSE;
      }
    }
    /* if any response is pending  OR
        the connection state is CONNECTED OR RESUME OR
        OR if any Tx PDU is still active
        DO NOT release first level lock */
    if((Xcp_IsCtoQueueEmpty()) &&
       ((Xcp_ConnectionStatus.State != XCP_STATE_CONNECTED) &&
        (Xcp_ConnectionStatus.State != XCP_STATE_RESUME)) &&
       (UnlockResources == TRUE))
    {
      /* release the I'th level lock of the connection resources */
      Xcp_ConnectionStatus.ConnectionId = XCP_INVALID_CONNECTION_ID;
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
      Atomics_Store(&Xcp_ActiveConnectionId, XCP_NO_ACTIVE_CONNECTION);
#endif
    }
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }

  DBG_XCP_PROCESSRESOURCESUNLOCK_EXIT();
}
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */

#if (XCP_ON_CANFD_ENABLED == STD_ON)
/*-----------------------------[Xcp_AddCanFDFill]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_AddCanFDFill
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLength,
  PduLengthType AvailableFrameLength
)
{
  /* array containing CANFD DLC values */
  PduLengthType ValidDlcValues[8U] = {8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
  /* local variable used to calculate the valid DLC */
  PduLengthType ValidLength = 0U;
  PduLengthType length = *DataLength;
  /* array index */
  uint8 index;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];

  /*  check configuration of MaxDlcRequired */
  if (XCP_IS_FLAG_SET(XCP_MAX_DLC_REQUIRED,
                ConnectionCfgPtr->Properties, uint8) == TRUE)
  {
    /* !LINKSTO Xcp.ASAM.CANFD.MAX_DLC002,1 */
    /* !LINKSTO Xcp.ASAM.CANFDPacket001,1 */
    /* check if length equals configured MaxDlcValue */
    if((length != ConnectionCfgPtr->MaxDlcValue) &&
       (AvailableFrameLength >= (ConnectionCfgPtr->MaxDlcValue)))
    {
      /* !LINKSTO Xcp.ASAM.CANFDPacket002,1 */
      /* Add fill bytes until the length configured */
      TS_MemSet(&TxBufferPtr[length], ConnectionCfgPtr->FillValue,
          ((usize)ConnectionCfgPtr->MaxDlcValue - (usize)length));
      /* update the length */
      length = ConnectionCfgPtr->MaxDlcValue;
    }
  }
  else
  {
    /* !LINKSTO Xcp.ASAM.CANFD.MAX_DLC001,1 */
    /* Get valid DLC length for the message */
    /* lengths from 1 - 7 are valid DLC */
    if (length >= 8U)
    {
      for (index = 1U; index < 8U; index++)
      {
        /* if the the length is in the range ValidDlcValues..[index-1]ValidDlcValues[index],
         use higher value as DLC */
        if ((length > ValidDlcValues[index-1]) && (length <= ValidDlcValues[index]))
        {
          ValidLength = ValidDlcValues[index];
          break;
        }
      }
      /* add FILL if required */
      if ((length < ValidLength) &&
           (AvailableFrameLength >= ValidLength))
      {
        /* !LINKSTO Xcp.ASAM.CANFDPacket003,1 */
        TS_MemSet(&TxBufferPtr[length], ConnectionCfgPtr->FillValue, ((usize)ValidLength - (usize)length));
        length = ValidLength;
      }
    }
  }
  /* update DataLength with the new value */
  *DataLength = length;
}
#endif /* #if (XCP_ON_CANFD_ENABLED == STD_ON) */


#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
STATIC FUNC(boolean, XCP_CODE) Xcp_AddFlexRayMultiPduInFrameLastMessage
(
  PduLengthType AvailableTxSpace,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
)
{
  boolean retValue = FALSE;
  /* select the Tx buffer */
  CONSTP2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageConcatEmpty002,1 */
  if (XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
              ConnectionCfgPtr->Properties, uint8) == TRUE)
  {
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif
    {
      /* CHECK: NOPARSE */
      if (AvailableTxSpace >=  1U)
      /* CHECK: PARSE */
      {
        /* !LINKSTO Xcp.ASAM.FLEXRAY.MessageConcatEmpty001,1 */
        /* add an empty FlexRay header
           LEN = 0 for detecting the end of the used payload segment */
        Xcp_AddFrameHeader(0U, TxBufferPtr, 1U, FALSE, TRUE);
        retValue = TRUE;
      }
      /* CHECK: NOPARSE */
      else
      {
        /* Unreachable branch. When enabling the flexray sequence correction support
         * feature, the sequence correction flag is always set, so the conditions
         * are excluding on each other. */
      }
      /* CHECK: PARSE */
    }
  }
  return retValue;
}
#endif /* #if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */


#if(XCP_EVENT_PACKET_ENABLED == STD_ON)

/*--------------------------[Xcp_EmitSessionTerminatedEvent]-----------------------------*/

STATIC FUNC(void, XCP_CODE)  Xcp_EmitSessionTerminatedEvent(void)
{
  Xcp_EventCTOType EventCTO;

  DBG_XCP_EMITDAQOVERLOADEVENT_ENTRY();

  /* !LINKSTO Xcp.ASAM.EV_SESSION_TERMINATED,1 */
  /* prepare event response CTO for EV_SESSION_TERMINATED */
  EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
  EventCTO.EVENT_CTO[1U] = XCP_EV_SESSION_TERMINATED;
  EventCTO.Length = 2U;

  /* enqueue event package into event CTo queue */
  Xcp_EnqueueCtoEvent(&EventCTO);

  DBG_XCP_EMITDAQOVERLOADEVENT_EXIT();
}
#endif

#if(XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/*--------------------------[Xcp_IsAllowedToTransmitMulticast]-----------------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_IsAllowedToTransmitMulticast
(
  PduIdType IntTxPduId
)
{
  boolean retValue = FALSE;

  DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_ENTRY(IntTxPduId);

  if(XCP_IS_FLAG_SET(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8) == FALSE)
  {
    retValue = TRUE;
  }
  else
  {
    /* check if we are retrying to send this pdu */
    if ((XCP_IS_BYTE_FLAG_SET(IntTxPduId, Xcp_TxLPduData.IsBufferFullFlag) == TRUE) &&
        (XCP_IS_BYTE_FLAG_SET(IntTxPduId, Xcp_TxLPduData.IsMulticastFlag) == TRUE))
    {
      /* try retransmission */
      retValue = TRUE;
    }
  }

  DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_EXIT(retValue,IntTxPduId);

  return retValue;
}

/*--------------------------[Xcp_MonitorMulticastChange]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_MonitorMulticastChange(void)
{
  /* check if there are multicast PDUs that are still waiting for confirmation or in
   * Tx retry transmission mode */
  PduIdType Iterator;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
  boolean isRetryRequired = FALSE;
#endif /* XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0 */

  ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  for(Iterator = 0U; Iterator < ConnectionCfgPtr->TxPduMax; Iterator++)
  {
    /* get the ID of Tx LPdu buffer */
    PduIdType IntPduId = Xcp_TxLPduIdConf[Iterator].IntPduBufId;
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* verify if there is any PDU in multicast mode and PDU data has been sent */
    if ((XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsMulticastFlag) == TRUE)   &&
        ((XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsBufferFullFlag) == FALSE) &&
         (XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_TxLPduData.IsPduWaitingForConfFlag) == FALSE)))
    {
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
      /* check if maximum number of reties is reached */
      if (Xcp_TxLPduData.MulticastRetryCnt[Iterator] < XCP_MULTICAST_TO_UNICAST_RETRY_COUNT)
      {
        /* number of retries not reached yet -> set isRetryRequired flag
           in order to do the retry*/
        isRetryRequired = TRUE;
        /* Increment retry counter */
        Xcp_TxLPduData.MulticastRetryCnt[Iterator]++;
      }
      else
#endif /* (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0) */
      {
        /* maximum number of retries performed - autonomously disconnect */
        Xcp_PrepareForAutonomousDisconnect(FALSE);
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
        Xcp_TxLPduData.MulticastRetryCnt[Iterator] = 0U;
#endif /* XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0 */
      }
    }
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#if (XCP_MULTICAST_TO_UNICAST_RETRY_COUNT > 0)
    /* if it is required to perform retry request to SoAd */
    if (isRetryRequired == TRUE)
    {
      if (Xcp_SoAdChangeTxToUnicast() == E_OK)
      {
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        Xcp_TxLPduData.MulticastRetryCnt[Iterator] = 0U;
        /* if successful, clear multicast flags */
        XCP_CLEAR_FLAG(XCP_MULTICAST_TRANSMIT_MODE_FLAG, Xcp_ConnectionStatus.Flags, uint8);
        XCP_CLEAR_BYTE_FLAG(IntPduId, Xcp_TxLPduData.IsMulticastFlag);
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
      isRetryRequired = FALSE;
    }
#endif
  }
}
#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
#endif /* (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) */

/*==================[end of file]===========================================*/
