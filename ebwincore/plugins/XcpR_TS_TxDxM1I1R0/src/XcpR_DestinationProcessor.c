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

#include <TSMem.h>          /* EB memory functions */
/* !LINKSTO XcpR.dsn.SchMInclusion, 1 */
#include <SchM_XcpR.h>      /* Exclusive area definitions */
#include <XcpR.h>
#include <XcpR_Int.h>

/*==[Macros]==================================================================*/

#if (defined XCPR_IS_TX_PDU_ACTIVE)
#error XCPR_IS_TX_PDU_ACTIVE already defined
#endif
/** \brief Checks if the Pdu channel is active.
 **
 ** \param[in] IntBufId    Internal Tx Pdu Buffer Id
 **
 ** \return Transmit status of the selected PDU
 ** \retval TRUE  transmission can be done
 ** \retval FALSE transmission is in progress */
#if ((XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON))
#define XCPR_IS_TX_PDU_ACTIVE(TxPduId) \
  ((((XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(TxPduId)] & (uint8)(1U << XCPR_FLAG_IDX(TxPduId))) != 0U ) || \
    ((XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(TxPduId)] & (uint8)(1U << XCPR_FLAG_IDX(TxPduId))) != 0U) \
   ) ? TRUE : FALSE )
#else
#define XCPR_IS_TX_PDU_ACTIVE(TxPduId) \
  (((XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(TxPduId)] & (uint8)(1U << XCPR_FLAG_IDX(TxPduId))) != 0U) ? TRUE : FALSE )
#endif

#if (defined XCPR_IS_READ_QUEUE_EMPTY)
#error XCPR_IS_READ_QUEUE_EMPTY already defined
#endif
/** \brief Checks if the the read queue of the XcpR is empty.
 **
 ** \param[in] PackageLength    The length of the current package
 **
 ** \return The status of the read queue
 ** \retval TRUE  queue is empty
 ** \retval FALSE queue is not empty */
#define XCPR_IS_READ_QUEUE_EMPTY(PackageLength)  (0U == (XcpR_MsgQueue.NoOfBytes - (PackageLength)))

/*==[Types]===================================================================*/

typedef struct
{
  /* ------------- Dest Pdu buffer data ------------- */
  /* counter used to count up the number of cycles until BusTimeout mechanism overflows */
  XcpR_BusTimeoutCounterValueType BusTimeoutCntVal[XCPR_NO_OF_DEST_PDU_IDS];
  /* Available space in the current PduBuffer */
  PduLengthType PduAvailableTxSpace[XCPR_NO_OF_DEST_PDU_IDS];
  /* flag used when there is not enough Tx space, to indicate the message can never be sent
   * if the value is the one from configuration. Otherwise, it will be sent at next MainFunction. */
  boolean PduAvailableTxSpaceInitConfigValue[XCPR_NO_OF_DEST_PDU_IDS];
  /* Store last position in the current PduBuffer, if multiple packing is enabled */
  XcpR_MultiPackPduType PduMultiPackLastPosition[XCPR_NO_OF_DEST_PDU_IDS];
  /* Length of each Dest Pdu */
  PduLengthType PduLength[XCPR_NO_OF_DEST_PDU_IDS];
  /* Dest Pdu buffer used to contingently store the Dest PDU data, for all available PDUs */
  uint8 PduBuffer[XCPR_DEST_BUFFER_SIZE];
  /* bit flag indicating if PDU is waiting for confirmation. */
  uint8 IsPduWaitingForConfFlag[XCPR_DEST_PDU_FLAG_SIZE];
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
  /* bit flag indicating if PDU is waiting for trigger transmit. */
  uint8 IsPduWaitingForTriggFlag[XCPR_DEST_PDU_FLAG_SIZE];
#endif
  /* bit flag indicating if the buffer has new data in it. */
  uint8 IsBufferFullFlag[XCPR_DEST_PDU_FLAG_SIZE];
  /* Counter for Sequence correction (FlexRay) */
#if ((XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON) || (XCPR_NO_OF_ETHERNET_DESTINATIONS > 0))
  uint16 FrameTransmitCounter[(XCPR_NO_OF_FLEXRAY_DEST_WITH_SEQ_CORR + XCPR_NO_OF_ETHERNET_DESTINATIONS)];
#endif
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)
   /* counters used to retry the transmission whenever the bus timeout occurs */
  uint8 BusTxRetryCount[XCPR_NO_OF_TX_DESTINATION_PDUS];
#endif
#if (XCPR_TX_RETRY_COUNT > 0)
  /* counter used to count down the number of retries until the Tx package is aborted */
  uint8 RetryCntVal[XCPR_NO_OF_TX_DESTINATION_PDUS];
#endif /* (XCPR_TX_RETRY_COUNT > 0) */
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
  /* flag used to mark if the destination needs to retry the transmission */
  boolean NeedsRetry[XCPR_NO_OF_TX_DESTINATION_PDUS];
#endif
} XcpR_DestPduDataType;

/*==[Declaration of functions with internal linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/** \brief Initial checks for packing extracted data
 **
 ** \param[in]  ProcessedInternalDest           Indicator if the packet's destination has been
 **                                             successfully identified.
 ** \param[in]  DestInternalPduId               Internal ID for the selected PDU channel.
 ** \param[in]  extracted_PackageLength         Package length of the extracted packet
 ** \param[in]  isFirstPacket                   First packet indicator
 ** \param[in]  isLastPacket                    Last packet indicator
 ** \param[out] isBufferFullAtFirstIterationPtr Value indicating when attempting to pack
 **                                             first packet, buffer is already full
 **
 ** \return    E_OK,     if pack Tx buffer data into destination runtime data is successfully
 **            E_NOT_OK, if pack Tx buffer data into destination runtime data failed
 **
 **/
STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_PackBufferDataForExtractedMessage
(
  PduIdType DestInternalPduId,
  PduLengthType extracted_PackageLength,
  boolean isFirstPacket,
  boolean isLastPacket,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) isBufferFullAtFirstIterationPtr
);

/** \brief Function to search the destination corresponding to a source from Master XCP
 **
 ** \param[in] extractedSrcPduId     Extracted source PDU-ID
 ** \param[in] Conf_SrcInternalPduId Configured source PDU-ID
 ** \param[in] ConnectMode           Connection mode (local or remote network)
 ** \param[in] SidId                 Function ID from where the function is called
 **
 ** \return    E_OK,     if the configured destination is successfully linked
 **            E_NOT_OK, if no configured destination according to used parameters
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedMasterXcpSource
(
  PduIdType extractedSrcPduId,
  PduIdType Conf_SrcInternalPduId,
  uint8 ConnectMode,
  uint8 SidId
);

/** \brief Function to search the destination corresponding to a source from Remote XCP
 **
 ** \param[in] Conf_SrcInternalPduId Configured source PDU-ID
 ** \param[in] PackageResponse       Package response data
 ** \param[in] SidId                 Function ID from where the function is called
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedRemoteXcpSource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 PackageResponse,
  uint8 SidId
);

/** \brief Function to search the destination corresponding to a source from Local XCP
 **
 ** \param[in] Conf_SrcInternalPduId Configured source PDU-ID
 ** \param[in] PackageResponse       Package response data
 ** \param[in] SidId                 Function ID from where the function is called
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedLocalXcpSource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 PackageResponse,
  uint8 SidId
);

/** \brief Function to get the next destination PDU-ID
 **
 ** \return Destination PDU-ID
 */
STATIC FUNC(PduIdType, XCPR_CODE) XcpR_CheckNextDestination
(
  void
);

/** \brief Function to process the one destination.
 **
 ** \param[in] Conf_DestInternalPduId Configured destination PDU-ID
 **
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessActiveDestination
(
  PduIdType Conf_DestInternalPduId
);

/** \brief Function to pack data into Tx destination PDU buffer.
 **
 ** \param[out] ReportDetInternalBusPtr  Flag to report an internal bus Det error
 ** \param[out] ReportDetExternalBusPtr  Flag to report an external bus Det error
 ** \param[in]  Conf_DestInternalPduId   Configured destination PDU-ID.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_DisconnectOrSessionTerminated
(
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ReportDetInternalBusPtr,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ReportDetExternalBusPtr,
  PduIdType Conf_DestInternalPduId
);

/** \brief Function to pack data into Tx destination PDU buffer.
 **
 ** \param[in]  DestInternalPduId      Internal ID for the selected PDU channel.
 ** \param[in]  PackageLength          Payload length for the message to be packed.
 ** \param[in]  IsFirstPacket          First packet indicator
 ** \param[in]  SidId                  Function ID from where the function is called
 ** \param[out] IsLastPacketInFramePtr Indicator to mark the current message as the
 **                                    last one in the frame
 ** \param[out] MessagePtr             Pointer to message to be packed
 **/
STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_PackTxBufferData
(
  PduIdType DestInternalPduId,
  PduLengthType PackageLength,
  boolean IsFirstPacket,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) IsLastPacketInFramePtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) MessagePtr,
  uint8 SidId
);

/** \brief Function used to get the address where the next package will be packed
 **
 ** \param[in]  DestInternalPduId           Internal ID for the selected Pdu channel.
 ** \param[in]  Conf_DestPduHeaderLength    Configured destination header length.
 ** \param[in]  PackageLength               Payload length for the message to be packed.
 ** \param[in]  IsFirstPacket               First packet indicator.
 ** \param[in]  MessagePtr                  Pointer to message to be packed.
 ** \param[in]  SidId                       Function ID from where the function is called
 ** \param[out] AvailableSpaceForMessagePtr Indicator for PDU fits in the available space.
 **/
STATIC FUNC_P2VAR(uint8, XCPR_VAR_CLEARED, XCPR_CODE) XcpR_PackNextPdu
(
  PduIdType DestInternalPduId,
  PduLengthType Conf_DestPduHeaderLength,
  PduLengthType PackageLength,
  boolean IsFirstPacket,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) AvailableSpaceForMessagePtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) MessagePtr,
  uint8 SidId
);

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
/** \brief Function to calculate if additional fill bytes are needed due to FlexRay alignment
 **
 ** \param[in]  PackageLength          Payload length for the message to be packed
 ** \param[in]  DataLength             Message length
 ** \param[in]  Conf_DestInternalPduId Configured destination PDU-ID
 ** \param[out] DataLengthPlusFill     Length of the aligned data packet
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_GetFlexRayMultiPduInFrameNrOfFillBytes
(
  PduLengthType PackageLength,
  PduLengthType DataLength,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_APPL_DATA) DataLengthPlusFill,
  PduIdType Conf_DestInternalPduId
);

/** \brief Function to add fill bytes due to FlexRay alignment
 **
 ** \param[in]  PackageLength          Payload length for the message to be packed
 ** \param[in]  DataLength             Message length.
 ** \param[in]  Conf_DestInternalPduId Configured destination PDU-ID.
 ** \param[out] XcpRDestBufferPtr      Pointer to the buffer, to which the header and tail
 **                                    shall be added.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_AddFlexRayMultiPduInFrameFill
(
  PduLengthType PackageLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType DataLength,
  PduIdType Conf_DestInternalPduId
);

/** \brief Function to add an empty last message with LEN=0 if connection is on FlexRay
 **  and multiple packages in one frame are configured
 **
 ** \param[in]  AvailableTxSpace       AvailableTxSpace available space into Tx frame
 ** \param[in]  Conf_DestInternalPduId Configured destination PDU-ID
 ** \param[out] XcpRDestBufferPtr      Pointer to the destination frame, to which the tail shall be added
 **/
STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_AddFlexRayMultiPduInFrameLastMessage
(
  PduLengthType AvailableTxSpace,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduIdType Conf_DestInternalPduId
);
#endif

#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
/** \brief Function to add an empty last message with LEN=0 if connection is on FlexRay
 **  and multiple packages in one frame are configured
 **
 ** \param[in]  DestInternalPduId Internal ID for the selected PDU channel
 ** \param[out] XcpRDestBufferPtr Pointer to the destination frame, to which the tail shall be added
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_AddFlexRayFrameEnding
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduIdType DestInternalPduId
);
#endif

#if (((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && \
     ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))) || \
      (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
/** \brief Function to add the frame header and tail to the PDU to be sent
 **
 ** \param[in]  DataLength             Length of the current Pdu data section
 ** \param[in]  HeaderLength           Length of the current Pdu header section
 ** \param[in]  FirstPacket            Whether this is the first packet in the frame or not
 ** \param[in]  Conf_DestInternalPduId Configured destination PDU-ID
 ** \param[out] XcpRDestBufferPtr      Pointer to the TxFrame, to which the header and tail
 **                                    shall be added
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_AddFrameHeader
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType HeaderLength,
  boolean FirstPacket,
  PduIdType Conf_DestInternalPduId
);
#endif

#if (XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED == STD_ON)
/** \brief Function to add fill bytes to the CAN-FD frame
 **
 ** \param[in]      AvailableFrameLength   Length of the current Pdu data section available
 ** \param[in]      Conf_DestInternalPduId Configured destination PDU-ID
 ** \param[out]     XcpRDestBufferPtr      Pointer to the TxFrame, to which the header and tail
 **                                        shall be added
 ** \param[in, out] DataLength             Length of the current Pdu data section
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_AddCanFDFill
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_APPL_DATA) DataLength,
  PduLengthType AvailableFrameLength,
  PduIdType Conf_DestInternalPduId
);
#endif

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
/** \brief Function used for initializing the FlexRay buffer.
 **
 ** \param[in]  DestPtr       Pointer to the destination configuration
 ** \param[out] DestBufferPtr Pointer to destination buffer
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
STATIC FUNC(void, XCPR_CODE) XcpR_InitFlexRayBuffer
(
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) DestPtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) DestBufferPtr
);
#endif /* (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) */

/** \brief Function used to retrieved the internal destination PDU-ID
 **
 ** \param[out] DestInternalPduIdPtr Internal destination ID which is mapped towards the TL
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_GetDestInternalPduId
(
  P2VAR(PduIdType, AUTOMATIC, XCPR_VAR_CLEARED) DestInternalPduIdPtr
);

/** \brief Transmit Retry mechanism.
 **
 ** \param[in]  TxPduId    Internal Id of the selected Tx Pdu channel
 ** \param[in]  TxStatus   Transmit status for the selected Tx Pdu channel
 ** \param[out] AbortTxPtr Pointer to Abort transmit signal
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessRetry
(
  PduIdType TxPduId,
  Std_ReturnType TxStatus,
  P2VAR(boolean, AUTOMATIC, XCPR_APPL_DATA) AbortTxPtr
);

/** \brief Copy data into Tx.
 **
 ** \param[in]  DestinationId   Internal ID of the Tx destination
 ** \param[out] activeDestPduId Internal ID which is mapped towards the TL
 ** \param[out] TxPduInfo       The address of the buffer in which data needs to be
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_PrepareTxData
(
  PduIdType DestinationId,
  P2VAR(PduIdType, AUTOMATIC, XCPR_APPL_DATA) activeDestPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) TxPduInfo
);

#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
/** \brief Function used for re-trigger transmission of destinations
 ** which have been selected for retry transmission.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessTxRetryDestinations
(
  void
);
#endif

/** \brief Function used to extract values from the XcpR queue
 **
 ** \param[out] ExtractedPackageLengthPtr Package length of the extracted packet.
 ** \param[out] ExtractedSrcPduIdPtr      Source PDU-ID of the  extracted packet.
 ** \param[out] ExtractedSrcPduTypePtr    Source PDU type of the  extracted packet.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_HandleDequeuedMessage
(
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) ExtractedPackageLengthPtr,
  P2VAR(PduIdType, AUTOMATIC, XCPR_VAR) ExtractedSrcPduIdPtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) ExtractedSrcPduTypePtr
);

/** \brief Function used to extract multiple values from the XcpR queue
 **
 ** \param[in] DestInternalPduId                Internal ID for the selected Pdu channel
 ** \param[in] isBufferFullAtFirstIterationPtr  Value indicating when attempting to pack
 **                                             first packet, buffer is already full
 ** \param[out] ExtractedPackageLengthPtr       Package length of the extracted packet.
 ** \param[out] ExtractedSrcPduIdPtr            Source PDU-ID of the  extracted packet.
 ** \param[out] ExtractedSrcPduTypePtr          Source PDU type of the  extracted packet.
 ** \param[out] isFirstPacketPtr                First packet indicator.
 ** \param[out] ContinueProcessingPtr           Value indicating to continue processing messages.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_HandleMultipleDequeuedMessages
(
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) isFirstPacketPtr,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ContinueProcessingPtr,
  PduIdType DestInternalPduId,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) isBufferFullAtFirstIterationPtr,
  PduIdType PreviousDestInternalPduId
);

/** \brief Function used to find the destination linked to the current source, based on
 **        the connect mode.
 **
 ** \param[in]  extracted_SrcPduType    Extracted source PDU type (Rx/Tx)
 ** \param[in]  extracted_SrcPduId      Extracted source PDU-ID
 ** \param[in]  extracted_PackageLength Package length of the extracted packet
 ** \param[out] FindDestPduIdRetValPtr  Pointer where return value of XcpR_FindDestinationPduId
 **                                     shall be stored.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_HandleLinkedDestination
(
  uint8 extracted_SrcPduType,
  PduIdType extracted_SrcPduId,
  PduLengthType extracted_PackageLength
);

/** \brief Function used to determine if the source originates from the Master or a Slave.
 **        and to handle Disconnect flags.
 **
 ** \param[in] Pid_or_PackageResponse Packet identifier or pack response
 ** \param[in] Conf_SrcInternalPduId  Configured source PDU-ID
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_SrcOriginAndDisconnectHandling
(
  uint8 Pid_or_PackageResponse,
  PduIdType Conf_SrcInternalPduId
);

/** \brief Function to get the PDU start position inside the DestPdu buffer
 **
 ** \param[in] DestInternalPduId Internal ID for the selected Pdu channel
 ** \param[in] IsFirstPacket     First packet indicator
 **/
STATIC FUNC_P2VAR(uint8, XCPR_VAR_CLEARED, XCPR_CODE) XcpR_GetDestPduDataPtr
(
  PduIdType DestInternalPduId,
  boolean IsFirstPacket
);

#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
/** \brief Function used to send packed data frames.
 **
 ** \param[in]  PackedFrame                     PDU ID for a packed frame
 ** \param[out] RetryTransmissionThisCyclePtr   Value indicating if transmission shall be retried.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_HandlePackedDataProcessing
(
  PduIdType PackedFrame,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) RetryTransmissionThisCyclePtr
);
#endif

#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
/** \brief Function used to set flag in case a TxRetry is needed.
 **
 ** \param[in]  DestInternalPduId               Internal ID for the selected Pdu channel
 ** \param[out] RetryTransmissionThisCyclePtr   Value indicating if transmission shall be retried.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_SetRetryFlag
(
  PduIdType DestInternalPduId,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) RetryTransmissionThisCyclePtr
);

/** \brief Function used to check if processing can continue.
 **
 ** \param[in]  DestInternalPduId       Internal ID for the selected Pdu channel
 **/
STATIC FUNC(boolean, XCPR_CODE) XcpR_CheckIfProcessingCouldContinue
(
  PduIdType DestInternalPduId
);
#endif

/** \brief Function used to reset flags for connect command processing.
 **
 ** \param[in]  Conf_DestPduHeaderLength   Length of the header, used to get the Pid.
 ** \param[out] XcpRDestBufferPtr   Destination buffer.
 **/
STATIC FUNC(void, XCPR_CODE) XcpR_ResetStatusForConnectCommand
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType Conf_DestPduHeaderLength
);

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

#define XCPR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief Destination queue containing destination (Tx) data and length information */
STATIC VAR(XcpR_DestPduDataType, XCPR_VAR_CLEARED) XcpR_DestPduData;

#define XCPR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_CLEARED_8
#include <XcpR_MemMap.h>

/** \brief Connection mode (XCPR_INVALID_CONNECTION_MODE, XCPR_MODE_REMOTE_ECU or XCPR_MODE_LOCAL_ECU) */
STATIC VAR(uint8, XCPR_VAR_CLEARED) XcpR_ActiveConnectionMode;

/** \brief Variable used to hold the state of disconnect command pending */
STATIC VAR(boolean, XCPR_VAR_CLEARED) XcpR_DisconnectCommandPending;

/** \brief Variable used to hold the state of disconnect pending */
STATIC VAR(boolean, XCPR_VAR_CLEARED) XcpR_DisconnectPending;

/** \brief Variable used to hold the state of autonomous disconnect triggered */
STATIC VAR(boolean, XCPR_VAR) XcpR_AutonomousDisconnectPending;

/** \brief Variable used to hold that XcpR should set its state to disconnected */
STATIC VAR(boolean, XCPR_VAR) XcpR_NoTxConfOnInternalBus;

/** \brief Variable used to break the processing of destinations if there is not
 **  enough space for the transmission buffer */
STATIC VAR(boolean, XCPR_VAR) XcpR_InsufficientSpaceLastMessage;

#define XCPR_STOP_SEC_VAR_CLEARED_8
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_8
#include <XcpR_MemMap.h>

/** \brief Array of counters used for reporting Det errors outside the critical section
 **  protecting the loop inside XcpR_ProcessDestinations().
 **  Corresponding values:
 **  Index 0: Counter for runtime errors of type     XCPR_E_PDU_LOST
 **  Index 1: Counter for runtime errors of type     XCPR_E_UNEXPECTED_MSG
 **  Index 2: Counter for development errors of type XCPR_E_INVALID_CONNECT_MODE
 **  Index 3: Counter for development errors of type XCPR_E_INVALID_LENGTH
 **  Index 4: Counter for runtime errors of type     XCPR_E_INVALID_ACTIVE_DESTINATION
 */
VAR(uint8, XCPR_VAR) XcpR_ReportDetProcessDestCtr[XCPR_NR_DET_ERROR_TYPES_PROCESSDEST] = {0U};

#define XCPR_STOP_SEC_VAR_8
#include <XcpR_MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/*------------------[XcpR_InitTransmitProcessor]----------------------------*/

FUNC(void, XCPR_CODE) XcpR_InitTransmitProcessor(void)
{
  PduIdType Iterator;

  for(Iterator = 0U; Iterator < XCPR_NO_OF_DEST_PDU_IDS; Iterator++)
  {
    /* --- clear Dest Pdu buffer data --- */
    XcpR_DestPduData.PduAvailableTxSpace[Iterator] = 0U;
    XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[Iterator] = FALSE;
    XcpR_DestPduData.PduMultiPackLastPosition[Iterator]= 0U;
    XcpR_DestPduData.PduLength[Iterator] = 0U;
    /* --- clear BusTimeout mechanism data --- */
    XcpR_DestPduData.BusTimeoutCntVal[Iterator] = 0U;
  }

  for(Iterator = 0U; Iterator < XCPR_DEST_PDU_FLAG_SIZE; Iterator++)
  {
    XcpR_DestPduData.IsBufferFullFlag[Iterator]         = 0U;
    XcpR_DestPduData.IsPduWaitingForConfFlag[Iterator]  = 0U;
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
    XcpR_DestPduData.IsPduWaitingForTriggFlag[Iterator] = 0U;
#endif
  }

#if ((XCPR_TX_RETRY_COUNT > 0) || (XCPR_TX_BUS_RETRY_COUNT > 0U))
  for(Iterator = 0U; Iterator < XCPR_NO_OF_TX_DESTINATION_PDUS; Iterator++)
  {
    XcpR_DestPduData.NeedsRetry[Iterator] = FALSE;
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)
    XcpR_DestPduData.BusTxRetryCount[Iterator] = 0U;
#endif
#if (XCPR_TX_RETRY_COUNT > 0)
    XcpR_DestPduData.RetryCntVal[Iterator] = XCPR_TX_RETRY_COUNT;
#endif /* (XCPR_TX_RETRY_COUNT > 0) */
  }
#endif /* (XCPR_TX_RETRY_COUNT > 0) || (XCPR_TX_BUS_RETRY_COUNT > 0U) */
#if ((XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON) || (XCPR_NO_OF_ETHERNET_DESTINATIONS > 0))
  for(Iterator = 0U; Iterator < (XCPR_NO_OF_FLEXRAY_DEST_WITH_SEQ_CORR + XCPR_NO_OF_ETHERNET_DESTINATIONS); Iterator++)
  {
    XcpR_DestPduData.FrameTransmitCounter[Iterator] = 0U;
  }
#endif

  XcpR_AutonomousDisconnectPending = FALSE;
  XcpR_NoTxConfOnInternalBus = FALSE;
  XcpR_DisconnectCommandPending = FALSE;
  XcpR_DisconnectPending = FALSE;

  XcpR_ActiveConnectionMode = XCPR_INVALID_CONNECTION_MODE;
}

/* If TriggerTransmit function will be called by lower layer */
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/*-----------------------------[XcpR_ProvideTxData]--------------------------*/

FUNC(Std_ReturnType, XCPR_CODE) XcpR_ProvideTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr;
  PduLengthType XcpRDestBufferLength;
  PduIdType TxDestIterator;
  PduIdType Conf_DestInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;

  for (TxDestIterator = 0U; TxDestIterator < XCPR_NO_OF_TX_DEST_PDU_IDS; TxDestIterator++)
  {
    if (TxPduId == XcpR_DestConf[TxDestIterator].XcpRDestPduId)
    {
      Conf_DestInternalPduId = XcpR_DestConf[TxDestIterator].IntPduBufId;
    }
  }

  /* Enter critical section.*/
  /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

   /* Check if buffer flag is set */
  if ((XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId))) != 0U )
  {
    /* get PDU start position inside the Pdu buffer */
    XcpRDestBufferPtr =  &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[Conf_DestInternalPduId].PduStartPos]);
    /* get the PDU length */
    XcpRDestBufferLength = XcpR_DestPduData.PduLength[Conf_DestInternalPduId];

    /* Copy data to be transmitted */
    TS_MemCpy(PduInfoPtr->SduDataPtr, XcpRDestBufferPtr, XcpRDestBufferLength);
    /* Update the Pdu length */
    PduInfoPtr->SduLength = XcpRDestBufferLength;

    /* clear the "waiting for trigger" flag to prevent sending the same data twice */
    XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
    /* set flag to indicate that the Pdu is waiting for confirmation */
    XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] |= (uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId));

    /* signal that the data was successfully written */
    RetVal = E_OK;
  }

  /* Exit critical section.*/
  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  return RetVal;
}

#endif /* ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

/*------------------[XcpR_ProcessDestinations]------------------------------*/

FUNC(void, XCPR_CODE) XcpR_ProcessDestinations
(
  void
)
{
  PduIdType Iterator;
#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  uint16 PackedFrame_Iterator;
  PduIdType PackedFrame[XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME];
#endif
  boolean isFirstPacket[XCPR_NO_OF_DESTINATION_PDUS];
  boolean isLastPacket[XCPR_NO_OF_DESTINATION_PDUS];
  /* Internal data */
  PduLengthType extracted_PackageLength = (PduLengthType) XCPR_INVALID_PDU_LENGTH;
  PduIdType extracted_SrcPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  uint8 extracted_SrcPduType = XCPR_INVALID_SOURCE_TYPE;
  PduIdType DestInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  PduIdType PreviousDestInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  /* Packet identifiers and handlers */
  boolean ContinueProcessing = TRUE;
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
  boolean RetryTransmissionThisCycle = TRUE;
#endif
  uint8 DeQueueCounter = 0U;
#if ((((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) \
     && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  uint8 FrameDestIndex = 0;
#endif
  Std_ReturnType PackedDataRetVal = E_NOT_OK;
  boolean isBufferFullAtFirstIteration = FALSE;

  /* Reset internal variables */
  XcpR_InsufficientSpaceLastMessage = FALSE;

  XcpR_InitDetErrorsProcessDest();

  /* clear Packet indicators
   * (isFirstPacket, isLastPacket is used generic for both multiple packing enabled and disabled scenarios) */
  for(Iterator = 0U; Iterator < XCPR_NO_OF_DESTINATION_PDUS; Iterator++)
  {
    isFirstPacket[Iterator] = TRUE;
    isLastPacket[Iterator] = FALSE;

#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
    if (Iterator < XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME)
    {
      PackedFrame[Iterator] = (PduIdType) XCPR_INVALID_PDU_ID;
    }
#endif
  }

  while (XcpR_IsMsgQueueEmpty() == FALSE)
  {
    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    if (DeQueueCounter != 0U)
    {
      XcpR_GetDestInternalPduId(&PreviousDestInternalPduId);
    }

    XcpR_HandleDequeuedMessage(&extracted_PackageLength,
                                    &extracted_SrcPduId, &extracted_SrcPduType);

    XcpR_HandleLinkedDestination(extracted_SrcPduType, extracted_SrcPduId, extracted_PackageLength);
    XcpR_GetDestInternalPduId(&DestInternalPduId);

    if (DeQueueCounter != 0U)
    {
      /* Cannot send outside of while loop, multiple independent messages, because we need to check if TxBuffer is available (get TxConf) before sending with the same PduId */
      XcpR_HandleMultipleDequeuedMessages(&isFirstPacket[DestInternalPduId], &ContinueProcessing, DestInternalPduId, &isBufferFullAtFirstIteration, PreviousDestInternalPduId);
    }

    /* Check if processing continues - unxpected messages might be consumed while searching for a Dest in XcpR_HandleLinkedDestination() -> GetActiveDest() */
    if (XcpR_IsMsgQueueEmpty() == FALSE)
    {
      if (ContinueProcessing == TRUE)
      {
        /* Continue processing only if transmission Buffer is available for the destination PID */
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
        /* !LINKSTO XcpR.dsn.TransmissionOngoing.QueuedMessages,1 */
        if (XcpR_CheckIfProcessingCouldContinue(DestInternalPduId) == TRUE)
#endif
        {
          if (XCPR_IS_READ_QUEUE_EMPTY(extracted_PackageLength))
          {
            isLastPacket[DestInternalPduId] = TRUE;
          }

#if ((((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) \
     && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
          if ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U) /* Multiple packing is enabled for this PDU channel */
          {
            PackedDataRetVal = XcpR_PackBufferDataForExtractedMessage(DestInternalPduId, extracted_PackageLength, isFirstPacket[DestInternalPduId], isLastPacket[DestInternalPduId], &isBufferFullAtFirstIteration);

            /* XcpR_PackTxBufferData() returns E_NOT_OK only if there is not sufficient Tx space left. In that case, it is impossible to return another E_OK. */
            if ((PackedDataRetVal == E_OK) && (isBufferFullAtFirstIteration == FALSE))
            {
              boolean DestPduFound = FALSE;

              for (PackedFrame_Iterator= 0U; PackedFrame_Iterator < XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME; PackedFrame_Iterator++)
              {
                if (PackedFrame[PackedFrame_Iterator] == DestInternalPduId)
                {
                  /* current DestInternalPduId is already present in a PackedFrame index. Use the same index. */
                  PackedFrame[PackedFrame_Iterator] = DestInternalPduId;
                  DestPduFound = TRUE;
                  break;
                }
              }

              if (DestPduFound == FALSE)
              {
                PackedFrame[FrameDestIndex] = DestInternalPduId;
                FrameDestIndex++;
              }
            }

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
          }
          else  /* Multiple packing is disabled for this PDU channel */
#endif /* XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON */
          {
            PackedDataRetVal = XcpR_PackBufferDataForExtractedMessage(DestInternalPduId, extracted_PackageLength, isFirstPacket[DestInternalPduId], isLastPacket[DestInternalPduId], &isBufferFullAtFirstIteration);

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            if (PackedDataRetVal == E_OK)
            {
              XcpR_ProcessActiveDestination(DestInternalPduId);
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
              XcpR_SetRetryFlag(DestInternalPduId, &RetryTransmissionThisCycle);
#endif
            }
          }

          if (XcpR_InsufficientSpaceLastMessage == TRUE)
          {
            break;
          }
        }
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
        else
        {
          /* Processing stopped since Transmission buffer is blocked for this PduId */
          SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
          break;
        }
#endif
      }
      else
      {
        /* Transmit buffer is full and in need of TxConfirmation */
        SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
        break;
      }
    }
    else
    {
      /* Internal buffer is empty after unexpected messages triggering Det errors -> data has been consumed */
      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      break;
    }
    DeQueueCounter++;
  }

#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  /* After all data has been packed, transmit available packed frames to configured destinations.*/
  for (PackedFrame_Iterator= 0U; PackedFrame_Iterator < XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME; PackedFrame_Iterator++)
  {
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
    XcpR_HandlePackedDataProcessing(PackedFrame[PackedFrame_Iterator], &RetryTransmissionThisCycle);
#else
    XcpR_HandlePackedDataProcessing(PackedFrame[PackedFrame_Iterator], NULL_PTR);
#endif
  }
#endif

#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
  if (RetryTransmissionThisCycle == TRUE)
  {
    XcpR_ProcessTxRetryDestinations();
  }
#endif

  XcpR_TriggerDetErrorsProcessDest();
}

/*==[Definition of functions with internal linkage]===========================*/

#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
/*------------------[XcpR_HandlePackedDataProcessing]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_HandlePackedDataProcessing
(
  PduIdType PackedFrame,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) RetryTransmissionThisCyclePtr
)
{
  if (PackedFrame != (PduIdType) XCPR_INVALID_PDU_ID)
  {
    XcpR_ProcessActiveDestination(PackedFrame);

#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
    if (XcpR_DestPduData.NeedsRetry[PackedFrame] == TRUE)
    {
      *RetryTransmissionThisCyclePtr = FALSE;
    }
#else
     TS_PARAM_UNUSED(RetryTransmissionThisCyclePtr);
#endif
  }
}
#endif

#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
/*------------------[XcpR_SetRetryFlag]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_SetRetryFlag
(
  PduIdType DestInternalPduId,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) RetryTransmissionThisCyclePtr
)
{
  if ((DestInternalPduId < XCPR_NO_OF_TX_DESTINATION_PDUS) && (*RetryTransmissionThisCyclePtr != FALSE))
  {
    if (XcpR_DestPduData.NeedsRetry[DestInternalPduId] == TRUE)
    {
      *RetryTransmissionThisCyclePtr = FALSE;
    }
  }
}

STATIC FUNC(boolean, XCPR_CODE) XcpR_CheckIfProcessingCouldContinue
(
  PduIdType DestInternalPduId
)
{
  boolean ContinueProcessing = TRUE;

  if (XcpR_DestConf[DestInternalPduId].PduType == XCPR_TX_PDU_DIRECTION)
  {
    if (XcpR_DestPduData.NeedsRetry[DestInternalPduId] == TRUE)
    {
      ContinueProcessing = FALSE;
    }
  }
  return ContinueProcessing;
}
#endif
/*------------------[XcpR_HandleDequeuedMessage]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_HandleDequeuedMessage
(
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) ExtractedPackageLengthPtr,
  P2VAR(PduIdType, AUTOMATIC, XCPR_VAR) ExtractedSrcPduIdPtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) ExtractedSrcPduTypePtr
)
{
  /* Extracted values from the Msg queue */
  *ExtractedPackageLengthPtr = XcpR_ReadMsgLength();
  *ExtractedSrcPduIdPtr = XcpR_ReadMsgSrcPduId();
  *ExtractedSrcPduTypePtr = XcpR_ReadMsgSrcPduType();
}

/*------------------[XcpR_HandleMultipleDequeuedMessages]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_HandleMultipleDequeuedMessages
(
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) isFirstPacketPtr,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ContinueProcessingPtr,
  PduIdType DestInternalPduId,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) isBufferFullAtFirstIterationPtr,
  PduIdType PreviousDestInternalPduId
)
{

#if (((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))

  if ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) == 0U) /* Multiple packing is disabled for this PDU channel */
  {
    /* If there are multiple messages in the internal queue, after the first message is sent,
     * wait for TxConfirmation for the current DestInternalPduId before continuing processing the next message.
     * If Tx buffer is available, continue processing the next message from the XcpR internal queue
     * This action shall be done only if Multiple packing is disabled for this destination PDU channel.
     * I.e. One dequeued message -> one transmission. Otherwise, packing is involved and the packed payload with multiple messages is sent once. */
    if ((XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(DestInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(DestInternalPduId))) == 0U )
    {
#endif
      *isFirstPacketPtr = FALSE;
      *ContinueProcessingPtr = TRUE;

#if (((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
    }
    else
    {
      /* If Multiple packing for transmission is disabled (i.e. a LL_Transmit is triggered for each dequeued message)
       * and there are multiple messages in the internal queue (receive from RxInd)
       * do not continue to Dequeue messages until a TxConfirmation is received */
      *ContinueProcessingPtr = FALSE;
    }
  }
  else /* Multiple packing is enabled for this PDU channel */
#endif
  {
    if (*isBufferFullAtFirstIterationPtr == TRUE)
    {
      *ContinueProcessingPtr = FALSE;
    }
    else if (DestInternalPduId != PreviousDestInternalPduId)
    {
      *isFirstPacketPtr = TRUE;
      *ContinueProcessingPtr = TRUE;
    }
    else
    {
      *isFirstPacketPtr = FALSE;
      *ContinueProcessingPtr = TRUE;
    }
  }

#if (((XCPR_RECEIVE_FROM_INDICATION == STD_OFF) || (XCPR_TRANSMIT_FROM_RXINDICATION == STD_ON)) && (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF))
  TS_PARAM_UNUSED(DestInternalPduId);
#endif
}

/*------------------[XcpR_HandleLinkedDestination]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_HandleLinkedDestination
(
  uint8 extracted_SrcPduType,
  PduIdType extracted_SrcPduId,
  PduLengthType extracted_PackageLength
)
{
  /* Config iterators */
  uint8 RxSrcIterator, TxSrcIterator;
  /* Valid/invalid response and Connect Mode */
  uint8 PidOrPackageResponse = 0U;
  uint8 ConnectMode = XCPR_INVALID_CONNECTION_MODE;
  /* Local variable to hold configured data */
  PduIdType Conf_SrcInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;

  /* Identify the PDU configuration as an entry in the XcpR_SrcConf */
  if (extracted_SrcPduType == XCPR_RX_SRC_TYPE)
  {
    for (RxSrcIterator = 0U; RxSrcIterator < XCPR_NO_OF_RX_SRC_PDU_IDS; RxSrcIterator++)
    {
      if (extracted_SrcPduId == XcpR_SrcConf[RxSrcIterator].XcpRSrcPduId)
      {
        Conf_SrcInternalPduId = XcpR_SrcConf[RxSrcIterator].IntPduBufId;
      }
    }
  }
  else /* (extracted_SrcPduType == XCPR_TX_SRC_TYPE) */
  {
    for (TxSrcIterator = XCPR_NO_OF_RX_SRC_PDU_IDS; TxSrcIterator < XCPR_NO_OF_SRC_PDU_IDS; TxSrcIterator++)
    {
      if (extracted_SrcPduId == XcpR_SrcConf[TxSrcIterator].XcpRSrcPduId)
      {
        Conf_SrcInternalPduId = XcpR_SrcConf[TxSrcIterator].IntPduBufId;
      }
    }
  }

  PidOrPackageResponse = XcpR_ReadMsgFirstByte();

  if ((extracted_PackageLength > 1U) && (extracted_SrcPduType == XCPR_RX_SRC_TYPE) && (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE) )
  {
    if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
    {
      /* !LINKSTO XcpR.Impl.DisconnectedState.ConnectMode, 1 */
      ConnectMode = XcpR_ReadMsgSecondByte();
    }
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
    else
    {
      ConnectMode = XcpR_Status.ConnectionMode;
    }
#endif
  }
  else
  {
    /* Nothing to do */
  }

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_OFF)
  if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
#endif
  {
    XcpR_FindDestinationPduId(PidOrPackageResponse, ConnectMode, Conf_SrcInternalPduId, extracted_SrcPduId, extracted_SrcPduType, XCPR_SID_PROCESS_DESTINATIONS);
  }

  if (XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED)
  {
    XcpR_SrcOriginAndDisconnectHandling(PidOrPackageResponse, Conf_SrcInternalPduId);
  }
}

/*------------------[XcpR_FindDestinationPduId]---------------*/

FUNC(void, XCPR_CODE) XcpR_FindDestinationPduId
(
  uint8 Pid_or_PackageResponse,
  uint8 ConnectMode,
  PduIdType Conf_SrcInternalPduId,
  PduIdType extracted_SrcPduId,
  uint8 extracted_SrcPduType,
  uint8 SidId
)
{
  /* pack + headers */
  if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
  {
    /* Runtime data will be updated only when this internal function is called from the destination processor.
     * When called from XcpR_ReceiveRxData() it is only used to search for a linked destination, before accepting a package */
    if (SidId != XCPR_SID_RECEIVE_RX_DATA)
    {
      XcpR_Status.CurrentSourceOrigin = XCPR_MASTER_SRC_ORIGIN;
    }

    /* source from master Xcp is always of type Rx */
    XcpR_ProcessDestFromLinkedMasterXcpSource(extracted_SrcPduId, Conf_SrcInternalPduId, ConnectMode, SidId);
  }
  else /* (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == FALSE) */
  {
    if (SidId != XCPR_SID_RECEIVE_RX_DATA)
    {
      XcpR_Status.CurrentSourceOrigin = XCPR_SLAVE_SRC_ORIGIN;
    }

    if (extracted_SrcPduType == XCPR_RX_SRC_TYPE) /* source is XcpR_RxIndication() from remote Xcp */
    {
      XcpR_ProcessDestFromLinkedRemoteXcpSource(Conf_SrcInternalPduId, Pid_or_PackageResponse, SidId);
    }
    else /* extracted_SrcPduType == XCPR_TX_SRC_TYPE */ /* source is XcpR_Transmit() */
    {
      XcpR_ProcessDestFromLinkedLocalXcpSource(Conf_SrcInternalPduId, Pid_or_PackageResponse, SidId);
    }
  }

}

/*------------------[XcpR_SrcOriginAndDisconnectHandling]---------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_SrcOriginAndDisconnectHandling
(
  uint8 Pid_or_PackageResponse,
  PduIdType Conf_SrcInternalPduId
)
{
  /* pack + headers */
  if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
  {
    XcpR_Status.CurrentSourceOrigin = XCPR_MASTER_SRC_ORIGIN;

    /* source from master Xcp is always of type Rx */
    if (Pid_or_PackageResponse == 0xFE) /* PID is disconnect */
    {
      XcpR_DisconnectCommandPending = TRUE;
    }
  }
  else /* source is XcpR_Transmit() or source is XcpR_RxIndication() from remote Xcp */
  {
    XcpR_Status.CurrentSourceOrigin = XCPR_SLAVE_SRC_ORIGIN;

    if (XcpR_DisconnectCommandPending == TRUE)
    {
      /* Check if a positive response has been transmitted from Xcp */
      if (Pid_or_PackageResponse == 0xFF)
      {
        XcpR_DisconnectPending = TRUE;
      }
      else
      {
        XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST] += 1U;
        XcpR_DisconnectCommandPending = FALSE;
      }
    }
  }

}

/*------------------[XcpR_PackBufferDataForExtractedMessage]---------------*/

STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_PackBufferDataForExtractedMessage
(
  PduIdType DestInternalPduId,
  PduLengthType extracted_PackageLength,
  boolean isFirstPacket,
  boolean isLastPacket,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) isBufferFullAtFirstIterationPtr
)
{
  Std_ReturnType RetVal;
  boolean IsLastPacketInFrame;
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr;
#else
  TS_PARAM_UNUSED(isLastPacket);
#endif
#if ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  TS_PARAM_UNUSED(isBufferFullAtFirstIterationPtr);
#endif

  IsLastPacketInFrame = FALSE;
  RetVal = E_NOT_OK;

  /* Lock the buffer only if the identified destination is a Tx destination, so a LL_Transmit() call will be made. */
  if (XcpR_DestConf[DestInternalPduId].PduType == XCPR_TX_PDU_DIRECTION)
  {
  /* If transmission of packing of multiple messages is enabled, the check for the availability of the Tx buffer is done here, but only for the first message
   * Otherwise, if there are multiple messages in the internal Queue, but transmission of packing messages is disabled, each message is sent individually
   * and the check for the availability of the tx buffer is done at the beginning of the while loop in ProcessDestinations */
#if ((((((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))) \
      && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) \
      || \
       (((XCPR_RECEIVE_FROM_INDICATION == STD_OFF) || ((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_ON))) \
      && (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF)))
    /* if Tx buffer is available, try to fill it with Pdu data */
    if (isFirstPacket == TRUE)
    {
       /* check shall be done only for the first packet */
      if ((XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(DestInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(DestInternalPduId))) == 0U )
      {
#endif
        /* Pack Tx buffer data into Dest runtime data, for each extracted message.
         Extracted messages holding the same SrcPduid will be copied one after another in the identified PduBuffer location from the Dest runtime data */
        if (E_OK == XcpR_PackTxBufferData(DestInternalPduId, (extracted_PackageLength - XCPR_INTERNAL_DATA_SIZE), isFirstPacket, &IsLastPacketInFrame, NULL_PTR, XCPR_SID_PROCESS_DESTINATIONS))
        {
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
            /* Check if multiple packing is enabled for this Destination Pdu Id */
          if (((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U) && (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY))
          {
            if ((isLastPacket == TRUE) || (IsLastPacketInFrame == TRUE))  /* If it's only one packet (the first and last one) */
            {
              /* Point to the initial address and add the LastPosition value */
              XcpRDestBufferPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestInternalPduId].PduStartPos]);
              XcpRDestBufferPtr = &XcpRDestBufferPtr[XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId]];

              XcpR_AddFlexRayFrameEnding(XcpRDestBufferPtr, DestInternalPduId);
            }
          }
#endif
          /* mark the buffer as full */
          XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(DestInternalPduId)] |= (uint8)(1U << XCPR_FLAG_IDX(DestInternalPduId));
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
          /* Check that the PDU needs trigger transmit or not (immediate or decoupled transmission - in the case of FlexRay) */
          if ((XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT &  XcpR_DestConf[DestInternalPduId].Properties) != 0U)
          {
            /* set flag to indicate that the Pdu is waiting for trigger */
            /* !LINKSTO XcpR.Impl.Communication.FlexRay.DecoupledAccess, 1 */
            /* !LINKSTO XcpR.Impl.Communication.CDD.DecoupledAccess, 1 */
            XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(DestInternalPduId)] |= (uint8)(1U << XCPR_FLAG_IDX(DestInternalPduId));
          }
          else
#endif /* ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))*/
          {
            /* set flag to indicate that the Pdu is waiting for confirmation */
            /* !LINKSTO XcpR.Impl.Communication.FlexRay.ImmediateAccess, 1 */
            /* !LINKSTO XcpR.Impl.Communication.CDD.ImmediateAccess, 1 */
            XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(DestInternalPduId)] |= (uint8)(1U << XCPR_FLAG_IDX(DestInternalPduId));
          }

          RetVal = E_OK;
        }
#if ((((((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)) || (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))) \
      && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) \
      || \
       (((XCPR_RECEIVE_FROM_INDICATION == STD_OFF) || ((XCPR_RECEIVE_FROM_INDICATION == STD_ON) && (XCPR_TRANSMIT_FROM_RXINDICATION == STD_ON))) \
      && (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF)))
      }
      else
      {
        /* When attempting to pack first packet, buffer is already full - set variable, and stop processing at next iteration. */
         *isBufferFullAtFirstIterationPtr = TRUE;
      }
    }
    else /* isFirstPacket == FALSE */
    {
      /* same actions, but without checking if Tx buffer is available */
      /* return value is cast to void, considering isFirstPacket = FALSE, the return value in PackTxBufferData() is always set to E_OK, even if there
      is not enough available space for the message being packed, in order to send the already packed messages until this point. */
      (void)XcpR_PackTxBufferData(DestInternalPduId, (extracted_PackageLength - XCPR_INTERNAL_DATA_SIZE), isFirstPacket, &IsLastPacketInFrame, NULL_PTR, XCPR_SID_PROCESS_DESTINATIONS);

#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
/* Check if multiple packing is enabled for this Destination Pdu Id */
      if (((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U) && (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY))
      {
        if ((isLastPacket == TRUE) || (IsLastPacketInFrame == TRUE)) /* if multiple packing is enabled and this is the last message  */
        {
          /* Point to the initial address and add the LastPosition value */
          XcpRDestBufferPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestInternalPduId].PduStartPos]);
          XcpRDestBufferPtr = &XcpRDestBufferPtr[XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId]];

          XcpR_AddFlexRayFrameEnding(XcpRDestBufferPtr, DestInternalPduId);
        }
      }
#endif
       RetVal = E_OK;
    }
#endif
  }
  else /* XcpR_DestConf[DestInternalPduId].PduType == XCPR_RX_PDU_DIRECTION) */
  {
    if (E_OK == XcpR_PackTxBufferData(DestInternalPduId, (extracted_PackageLength - XCPR_INTERNAL_DATA_SIZE), isFirstPacket, &IsLastPacketInFrame, NULL_PTR, XCPR_SID_PROCESS_DESTINATIONS))
    {
      RetVal = E_OK;
    }
  }

  return RetVal;
}

/*------------------[XcpR_ProcessDestFromLinkedMasterXcpSource]-------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedMasterXcpSource
(
  PduIdType extractedSrcPduId,
  PduIdType Conf_SrcInternalPduId,
  uint8 ConnectMode,
  uint8 SidId
)
{
  /* Runtime data will be updated only when this internal function is called from the destination processor.
   * When called from XcpR_ReceiveRxData() it is only used to search for a linked destination, before accepting a package */
  if (SidId != XCPR_SID_RECEIVE_RX_DATA)
  {
    /* Pid will always be 0xFF, when XcpR is in state DISCONNECTED and this function is reached.
     * The protection check is done in XcpR_ReceiveRxData(). Message is not stored in the queue if Pid is not FF. */
    XcpR_Status.MasterSource.Id = extractedSrcPduId;
    XcpR_Status.MasterSource.Type = XCPR_RX_PDU_DIRECTION;
  }

  if (ConnectMode == XCPR_MODE_REMOTE_ECU)
  {
    (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
  }
  else /* if (ConnectMode == XCPR_MODE_LOCAL_ECU) */
  {
    (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_LOCAL_ECU, SidId);
  }
}

/*------------------[XcpR_ProcessDestFromLinkedRemoteXcpSource]-------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedRemoteXcpSource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 PackageResponse,
  uint8 SidId
)
{
  /* Runtime data will be updated only when this internal function is called from the destination processor.
   * When called from XcpR_ReceiveRxData() it is only used to search for a linked destination, before accepting a package */
  if (SidId != XCPR_SID_RECEIVE_RX_DATA)
  {
    XcpR_Status.SlaveSource.Id = Conf_SrcInternalPduId;
    XcpR_Status.SlaveSource.Type = XCPR_RX_PDU_DIRECTION;

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
    if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
    {
#endif
      if (XcpR_Status.isPreviousCommandConnect == TRUE)
      {
        /* Check if a positive response has been transmitted from Xcp */
        if (PackageResponse == 0xFF)
        {
          /* Temporarily Store connected mode */
          XcpR_ActiveConnectionMode = XCPR_MODE_REMOTE_ECU;
        }
        else /* if (PackageResponse == 0xFE) ERR */
        {
          XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST] += 1U;

          /* XcpR remains in Disconnected state */
          XcpR_Status.isPreviousCommandConnect = FALSE;
        }

        (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
      }
      else
      {
        uint8 XcpR_ExtractedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
        PduLengthType extractedPduLength;

        /* Consume the data from the queue since the message will not be sent */
        extractedPduLength = XcpR_GetMsgLength();
        (void)XcpR_GetMsgSrcPduId();
        (void)XcpR_GetMsgSrcPduType();

        /* Dequeue messages in FIFO order */
        XcpR_DeQueueMsgQueue((extractedPduLength - XCPR_INTERNAL_DATA_SIZE), &XcpR_ExtractedMessage[0U]);

        XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_UNEXPECTED_MSG] += 1U;
      }
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
    }
    else /* XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED */
    {
      (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
    }
#endif
  }
  else
  {
    (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
  }

}

/*------------------[XcpR_ProcessDestFromLinkedLocalXcpSource]-------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessDestFromLinkedLocalXcpSource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 PackageResponse,
  uint8 SidId
)
{
  /* Runtime data will be updated only when this internal function is called from the destination processor.
   * When called from XcpR_ReceiveRxData() it is only used to search for a linked destination, before accepting a package */
  if (SidId != XCPR_SID_RECEIVE_RX_DATA)
  {
    XcpR_Status.SlaveSource.Id = Conf_SrcInternalPduId;
    XcpR_Status.SlaveSource.Type = XCPR_TX_PDU_DIRECTION;

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
    if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
    {
#endif
      if (XcpR_Status.isPreviousCommandConnect == TRUE)
      {
        /* Check if a positive response has been transmitted from Xcp */
        if (PackageResponse == 0xFF)
        {
          /* Temporarily Store connected mode */
          XcpR_ActiveConnectionMode = XCPR_MODE_LOCAL_ECU;
        }
        else /* if (PackageResponse == 0xFE) ERR */
        {
          XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST] += 1U;
          /* XcpR remains in disconnected state */
          XcpR_Status.isPreviousCommandConnect = FALSE;
        }

        /* We use XCPR_MODE_REMOTE_ECU because we are searching for a Tx destination */
        (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
      }
      else
      {
        uint8 XcpR_ExtractedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
        PduLengthType extractedPduLength;

        /* Consume the data from the queue since the message will not be sent */
        extractedPduLength = XcpR_GetMsgLength();
        (void)XcpR_GetMsgSrcPduId();
        (void)XcpR_GetMsgSrcPduType();

        /* Dequeue messages in FIFO order */
        XcpR_DeQueueMsgQueue((extractedPduLength - XCPR_INTERNAL_DATA_SIZE), &XcpR_ExtractedMessage[0U]);

        /* DET runtime error */
        XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_UNEXPECTED_MSG] += 1U;
      }
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
    }
    else /* XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED */
    {
      (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
    }
#endif
  }
  else
  {
    (void)XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, SidId);
  }

}

/*------------------[XcpR_GetActiveDestination]-----------------------------*/

/* !LINKSTO XcpR.Impl.ActiveRoutingPath, 1 */
FUNC(Std_ReturnType, XCPR_CODE) XcpR_GetActiveDestination
(
  PduIdType Conf_SrcInternalPduId,
  boolean ConnectMode,
  uint8 SidId
)
{

  Std_ReturnType retVal = E_NOT_OK;
  uint8 DestIterator, SearchedPduType;
  boolean isMasterSrc;
  boolean LinkedDestFound = FALSE;

  /* Pointers to Src and Dest configuration arrays */
  P2CONST(XcpR_SrcPduType, AUTOMATIC, XCPR_APPL_CONST)  Conf_SrcPduIdPtr;
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_DestPduIdPtr;

  if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
  {
    isMasterSrc = TRUE;
  }
  else
  {
    isMasterSrc = FALSE;
  }

  if (ConnectMode == XCPR_MODE_LOCAL_ECU)
  {
    SearchedPduType = XCPR_RX_PDU_DIRECTION;
  }
  else /* if (ConnectMode == XCPR_MODE_REMOTE_ECU) */
  {
    SearchedPduType = XCPR_TX_PDU_DIRECTION;
  }

  /* Pointer to the Src configuration array */
  Conf_SrcPduIdPtr = &XcpR_SrcConf[Conf_SrcInternalPduId];

  for (DestIterator = 0U; DestIterator < XCPR_NO_OF_MAX_DEST_LINKED_TO_ONE_SRC; DestIterator++)
  {
    if ((Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator] != NULL_PTR) && (Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator]->PduType == SearchedPduType))
    {
      Conf_DestPduIdPtr = Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator];
      LinkedDestFound = TRUE;

      /* Runtime data will be updated only when this internal function is called from the destination processor.
      * When called from XcpR_ReceiveRxData() it is only used to search for a linked destination, before accepting a package */
      if (SidId != XCPR_SID_RECEIVE_RX_DATA)
      {
        /* if internal function is invoked in the context of an XcpR_RxIndication() received from the master Xcp and XcpR is not connected.
         * The source is always the master Xcp if the ConnectMode is XCPR_MODE_LOCAL_ECU */
        if (isMasterSrc == TRUE)
        {
          XcpR_Status.DestinationFromMaster.Id = Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator]->TLDestPduId;
          XcpR_Status.DestinationFromMaster.Type = SearchedPduType; /* Destination from Master Xcp can either be Rx or Tx destination */
          XcpR_Status.DestinationFromMasterInternalId = Conf_DestPduIdPtr->IntPduBufId;
        }
        /* if internal function is invoked in the context of an XcpR_RxIndication() received from the remote Xcp slave and XcpR is not connected */
        else
        {
          XcpR_Status.DestinationFromSlave.Id = Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator]->TLDestPduId;
          XcpR_Status.DestinationFromSlave.Type = XCPR_TX_PDU_DIRECTION; /* Destination from remote Slave Xcp is always a Tx destination. */
          XcpR_Status.DestinationFromSlaveInternalId = Conf_DestPduIdPtr->IntPduBufId;
        }
      }
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      else
      {
        XcpR_Status.CurrentSrcLinkedDestInternalId = Conf_DestPduIdPtr->IntPduBufId;
      }
#endif
      retVal = E_OK;
    }
  }

  if (LinkedDestFound == FALSE)
  {
    /* When triggered from SidId == XCPR_SID_GET_PDU_DATA_LENGTH or XCPR_SID_RECEIVE_RX_DATA, no data is copied yet in the internal queue */
    if (SidId == XCPR_SID_PROCESS_DESTINATIONS)
    {
      /* Consume the data from the queue since the message will not be sent */
      uint8 XcpR_ExtractedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
      PduLengthType extractedPduLength;

      extractedPduLength = XcpR_GetMsgLength();
      (void)XcpR_GetMsgSrcPduId();
      (void)XcpR_GetMsgSrcPduType();

      XcpR_DeQueueMsgQueue((extractedPduLength - XCPR_INTERNAL_DATA_SIZE), &XcpR_ExtractedMessage[0U]);
    }
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    /*  Err: No Rx destinations configured for Connect Mode 0/ No Tx destinations configured for Connect Mode 1. */
    XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_CONNECT_MODE] += 1U;
#endif
  }

  return retVal;
}

/*------------------[XcpR_CheckNextDestination]-----------------------------*/

STATIC FUNC(PduIdType, XCPR_CODE) XcpR_CheckNextDestination
(
  void
)
{
  PduIdType Conf_SrcInternalPduId, Extracted_SrcPduId, DestPduId;
  uint8 Extracted_SrcPduType, RxSrcIterator, TxSrcIterator;
  uint8 DestIterator, SearchedPduType;
  /* Pointers to Src and Dest configuration arrays */
  P2CONST(XcpR_SrcPduType, AUTOMATIC, XCPR_APPL_CONST)  Conf_SrcPduIdPtr;
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_DestPduIdPtr;

  /* If XcpR is connected to the LOCAL ECU, even if it receives multiple packages
   * they cannot be packed to be sent to the local ECU, since a local connection
   * is always over CDD.
   * So the searched Pdu Type will always be a Tx destination - to the master Xcp
   * or to the remote slave Xcp */
  SearchedPduType = XCPR_TX_PDU_DIRECTION;

  Conf_SrcInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  DestPduId = (PduIdType) XCPR_INVALID_PDU_ID;

  Extracted_SrcPduId = XcpR_ReadMsgSrcPduId();
  Extracted_SrcPduType = XcpR_ReadMsgSrcPduType();

  /* Identify the PDU configuration as an entry in the XcpR_SrcConf */
  if (Extracted_SrcPduType == XCPR_RX_SRC_TYPE)
  {
    for (RxSrcIterator = 0U; RxSrcIterator < XCPR_NO_OF_RX_SRC_PDU_IDS; RxSrcIterator++)
    {
      if (Extracted_SrcPduId == XcpR_SrcConf[RxSrcIterator].XcpRSrcPduId)
      {
        Conf_SrcInternalPduId = XcpR_SrcConf[RxSrcIterator].IntPduBufId;
      }
    }
  }
  else /* (extracted_SrcPduType == XCPR_TX_SRC_TYPE) */
  {
    for (TxSrcIterator = XCPR_NO_OF_RX_SRC_PDU_IDS; TxSrcIterator < XCPR_NO_OF_SRC_PDU_IDS; TxSrcIterator++)
    {
      if (Extracted_SrcPduId == XcpR_SrcConf[TxSrcIterator].XcpRSrcPduId)
      {
        Conf_SrcInternalPduId = XcpR_SrcConf[TxSrcIterator].IntPduBufId;
      }
    }
  }

  /* Pointer to the Src configuration array */
  Conf_SrcPduIdPtr = &XcpR_SrcConf[Conf_SrcInternalPduId];

  for (DestIterator = 0U; DestIterator < XCPR_NO_OF_MAX_DEST_LINKED_TO_ONE_SRC; DestIterator++)
  {
    if ((Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator] != NULL_PTR) && (Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator]->PduType == SearchedPduType))
    {
      Conf_DestPduIdPtr = Conf_SrcPduIdPtr->SrcToDestMapping[DestIterator];

      DestPduId = Conf_DestPduIdPtr->IntPduBufId;
    }
  }

  return DestPduId;
}

/*-----------------------------[XcpR_PackTxBufferData]-----------------*/

STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_PackTxBufferData
(
  PduIdType DestInternalPduId,
  PduLengthType PackageLength,
  boolean IsFirstPacket,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) IsLastPacketInFramePtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) MessagePtr,
  uint8 SidId
)
{
  /* Info about runtime Tx available space */
  boolean AvailableSpaceForMessage;
  /* initialize return value to worst case value */
  Std_ReturnType RetVal = E_NOT_OK;
  /* get the package header */
  PduLengthType Conf_DestPduHeaderLength = XcpR_DestConf[DestInternalPduId].PduAttributes.PduHeaderLength;
#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
  /* get the data pointer for the Dest Pdu buffer */
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr = XcpR_GetDestPduDataPtr(DestInternalPduId, IsFirstPacket);
#endif

  /* calculate and length and available space */
#if (((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  if (((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) ||
      ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP) || (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP)))
   && ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
  {
    if (IsFirstPacket == TRUE)
    {
      XcpR_DestPduData.PduLength[DestInternalPduId] = 0U; /* Reset value only if it's the first package */
      XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] = XcpR_DestConf[DestInternalPduId].PduAttributes.PduLengthMax;
      XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[DestInternalPduId] = TRUE;
    }
  }
  else
#endif
  {
    XcpR_DestPduData.PduLength[DestInternalPduId] = 0U;
    XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] = XcpR_DestConf[DestInternalPduId].PduAttributes.PduLengthMax;
    XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[DestInternalPduId] = TRUE;
  }

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
  {
    if (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
    {
     /* initialize FlexRay header for Tx PDU */
#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      if (IsFirstPacket == TRUE)
#endif
      {
        /* Add the FR header for each DestPdu, at the beginning (only if it is the first packet) */
        /* !LINKSTO XcpR.Impl.AdaptBusInfo, 1 */
        XcpR_InitFlexRayBuffer(&XcpR_DestConf[DestInternalPduId], XcpRDestBufferPtr);
      }
    }
  }
#endif /* (XCPR_ON_FLEXRAY_ENABLED == STD_ON) */

  /* fill up the Tx send buffer */
  if (XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] > Conf_DestPduHeaderLength)
  {
#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
    XcpRDestBufferPtr = XcpR_PackNextPdu(DestInternalPduId, Conf_DestPduHeaderLength, PackageLength, IsFirstPacket, &AvailableSpaceForMessage,
                                         MessagePtr, SidId);

#else
    (void)XcpR_PackNextPdu(DestInternalPduId, Conf_DestPduHeaderLength, PackageLength, IsFirstPacket, &AvailableSpaceForMessage,
                           MessagePtr, SidId);
#endif

    if (AvailableSpaceForMessage == TRUE)
    {
      /* In case of multiple packing, check if the next PduId has the same destination */
      if ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) &&
          ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
      {
        if (XcpR_IsMsgQueueEmpty() == FALSE)
        {
          PduIdType NextDestinationPduId;
          NextDestinationPduId = XcpR_CheckNextDestination();

          /* If the next message from the internal queue does not have the same destination, mark the current message as the last one in the frame. */
          if ((DestInternalPduId != NextDestinationPduId) && (NextDestinationPduId != (PduIdType)XCPR_INVALID_PDU_ID))
          {
            *IsLastPacketInFramePtr = TRUE;
          }
        }
      }

      /* at least one PDU is packed, so set return value to OK */
      RetVal = E_OK;
    }
    else if ((AvailableSpaceForMessage == FALSE) && (IsFirstPacket == TRUE)) /* If there is no PDU packed and also not sufficient space for the message */
    {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
      XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_LENGTH] += 1U;
#endif
    }
    else /* There is not sufficient space but this is not the first message being packed */
    {
     /* Function is called in the context of multiple packing and at least one PDU is packed.
      * There was not sufficient space for this additional message to be packed, but return value is set to OK,
      * in order to send the already packed messages. */
      *IsLastPacketInFramePtr = TRUE;
      RetVal = E_OK;
    }
  }
  /* CHECK: NOPARSE */
  else if (IsFirstPacket == FALSE) /* If there is at least one PDU packed but there is not sufficient space for the header of the message */
  /* CHECK: PARSE */
  {
    /* Function is called in the context of multiple packing and at least one PDU is packed.
     * There was not sufficient space for the header of this additional message to be packed, but return value is set to OK,
     * in order to send the already packed messages. */
     RetVal = E_OK;
     *IsLastPacketInFramePtr = TRUE;
     XcpR_InsufficientSpaceLastMessage = TRUE;
  }
  /* CHECK: NOPARSE */
  else
  {
   /* Unreachable code.
    * There is always sufficient space for the header of the first packet.
    */
    XCPR_UNREACHABLE_CODE_ASSERT(XCPR_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */


  if (RetVal == E_OK)
  {
    /* If there are multiple PDUs packed, then the rounding up is done in XcpR_AddFlexRayFrameEnding() */
#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  if ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) == 0U) /* Multiple packing is disabled for this Pdu */
#endif
  {
    if (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
    {
      /* Check if the full payload needs to be rounded up */
      {
        if( (XcpR_DestPduData.PduLength[DestInternalPduId] & 1U) == 1U)
        {
          /* add tail */
          XcpRDestBufferPtr[0U] = 0x00;
          /* update Tx package size */
          XcpR_DestPduData.PduLength[DestInternalPduId] += 1U;
        }
      }
    }
  }
#endif /* (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) */
  }

  return RetVal;
}

/*-----------------------------[XcpR_PackNextPdu]------------------*/

STATIC FUNC_P2VAR(uint8, XCPR_VAR_CLEARED, XCPR_CODE) XcpR_PackNextPdu
(
  PduIdType DestInternalPduId,
  PduLengthType Conf_DestPduHeaderLength,
  PduLengthType PackageLength,
  boolean IsFirstPacket,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) AvailableSpaceForMessagePtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR) MessagePtr,
  uint8 SidId
)
{
  uint8 XcpR_ExtractedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
  /* Message length for the selected PDU to be packed */
  PduLengthType MessageLength;
  PduLengthType MessageLengthPlusFill;
  /* save configured header length */
  PduLengthType headerLengthLocal;
  /* get the data pointer for the Dest Pdu buffer */
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr = XcpR_GetDestPduDataPtr(DestInternalPduId, IsFirstPacket);

  if (SidId == XCPR_SID_PROCESS_DESTINATIONS)
  {
    TS_PARAM_UNUSED(MessagePtr);
  }

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  if (((IsFirstPacket == FALSE) && (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)) && ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
  {
   /* In the case of FlexRay the subsequent concatenated messages
    * has only the LEN field as header, so one byte only */
    headerLengthLocal = 1U;
  }
  else
#endif
  {
    headerLengthLocal = Conf_DestPduHeaderLength;
  }

  MessageLength = headerLengthLocal + PackageLength;

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  if ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) && ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
  {
    /* !LINKSTO XcpR.Impl.AdaptBusInfo, 1 */
    XcpR_GetFlexRayMultiPduInFrameNrOfFillBytes(PackageLength, MessageLength, &MessageLengthPlusFill, DestInternalPduId);
  }
  else
#endif
  {
    MessageLengthPlusFill = MessageLength;
  }

  /* Check whether next PDU fits in the available space.
   * For FlexRay: It is not necessary to take into consideration the tail because the
   *              frame/PDU size is always even. */
  if (XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] >= MessageLengthPlusFill)
  {

    if (SidId == XCPR_SID_PROCESS_DESTINATIONS)
    {
      /* Extracted values from the Msg queue */
      (void)XcpR_GetMsgLength();
      (void)XcpR_GetMsgSrcPduId();
      (void)XcpR_GetMsgSrcPduType();

      /* Dequeue messages in FIFO order */
      XcpR_DeQueueMsgQueue(PackageLength, &XcpR_ExtractedMessage[0U]);
    }
    else /* (SidId == XCPR_SID_TRIGGER_DISCONNECT) || (SidId == XCPR_SID_SESSION_TERMINATED) */
    {
      TS_MemCpy(&XcpR_ExtractedMessage[0U], MessagePtr, PackageLength);
    }

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
    if ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) && ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
    {
      /* !LINKSTO XcpR.Impl.AdaptBusInfo, 1 */
      XcpR_AddFlexRayMultiPduInFrameFill(PackageLength, XcpRDestBufferPtr, MessageLength, DestInternalPduId);
    }
#endif

    *AvailableSpaceForMessagePtr = TRUE;

#if (((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && \
     ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))) || \
      (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
    /* write the header into the frame */
    /* !LINKSTO XcpR.Impl.AdaptBusInfo, 1 */
    XcpR_AddFrameHeader(PackageLength, XcpRDestBufferPtr, headerLengthLocal, IsFirstPacket, DestInternalPduId);
#endif

    /* select the data position in Pdu package */
    XcpRDestBufferPtr = &XcpRDestBufferPtr[headerLengthLocal];
#if ( ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)) && \
      (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
    if (((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) ||
       ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) || (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))) &&
       ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
    {
        XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId] += headerLengthLocal;
    }
#endif
    /* Add data from extracted (dequeued) message  */
    TS_MemCpy(XcpRDestBufferPtr, &XcpR_ExtractedMessage[0U], PackageLength);

#if (XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED == STD_ON)
    if (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_CANFD)
    {
      /* verify CAN-FD configuration parameters and add required FILL bytes */
      /* !LINKSTO XcpR.Impl.AdaptBusInfo, 1 */
      XcpR_AddCanFDFill(XcpRDestBufferPtr, &MessageLengthPlusFill, XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId], DestInternalPduId);
    }
#endif /* (XCPR_ON_CANFD_ENABLED == STD_ON) */

    /* select the address where the next package will be packed (we already moved the position
     * with the header length, now move it at the end of the message)*/
    XcpRDestBufferPtr = &XcpRDestBufferPtr[MessageLengthPlusFill - headerLengthLocal];

#if ( ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)) && \
      (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
    if (((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) ||
       ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) || (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))) &&
       ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
    {
        XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId] += (MessageLengthPlusFill - headerLengthLocal);
    }
#endif

    /* Update transmit data length */
    XcpR_DestPduData.PduLength[DestInternalPduId] += MessageLengthPlusFill;
    /* Update frame available space */
#if ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) && ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)))
    if (((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) ||
       ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) || (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))) &&
       ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U))
    {
      /* update available space with the remaining free Pdu frame space */
      XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] -= MessageLengthPlusFill;
      XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[DestInternalPduId] = FALSE;
    }
    else
#endif
    {
      /* abort packing because only one PDU in frame is supported */
      XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId] = 0U;
      XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[DestInternalPduId] = FALSE;
    }
  }
  else
  {
    *AvailableSpaceForMessagePtr = FALSE;
    XcpR_InsufficientSpaceLastMessage = TRUE;

    /* If there is not sufficient space caused by the initial value from configuration, consume data,
     * since it will never be sent. */
    if (XcpR_DestPduData.PduAvailableTxSpaceInitConfigValue[DestInternalPduId] == TRUE)
    {
      uint8 XcpR_ExtractedUnusedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
      PduLengthType extractedPduLength;

      /* Consume the data from the queue since the message will not be sent */
      extractedPduLength = XcpR_GetMsgLength();
      (void)XcpR_GetMsgSrcPduId();
      (void)XcpR_GetMsgSrcPduType();

      /* Dequeue messages in FIFO order */
      XcpR_DeQueueMsgQueue((extractedPduLength - XCPR_INTERNAL_DATA_SIZE), &XcpR_ExtractedUnusedMessage[0U]);
    }
  }

  return XcpRDestBufferPtr;
}

/*-----------------------------[XcpR_GetDestPduDataPtr]------------------*/

STATIC FUNC_P2VAR(uint8, XCPR_VAR_CLEARED, XCPR_CODE) XcpR_GetDestPduDataPtr
(
  PduIdType DestInternalPduId,
  boolean IsFirstPacket
)
{
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) retValue;
#if ( ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_OFF) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_OFF)) || \
      ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF) && (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_OFF )))
  TS_PARAM_UNUSED(IsFirstPacket);
#endif

#if ( ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)) && \
      ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON )))
  if (((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) ||
      ((XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) || (XcpR_DestConf[DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP)))
       &&
      (((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestInternalPduId].Properties) != 0U) ||
      ((XCPR_TRANSMISSION_SEQUENCE_CORRECTION & XcpR_DestConf[DestInternalPduId].Properties) != 0U)))
  {
    if (IsFirstPacket == TRUE)
    {
      /* get PDU start position inside the DestPdu buffer */
       retValue = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestInternalPduId].PduStartPos]);
       /* Also initialize the Last Position with 0 */
       XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId] = 0U;
    }
    else
    {
      /* If this is not the first packet for this DestInternalPduId, then point to the initial address and add the LastPosition value */
      retValue = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestInternalPduId].PduStartPos]);
      retValue = &retValue[XcpR_DestPduData.PduMultiPackLastPosition[DestInternalPduId]];
    }
  }
  else
#endif
  {
    retValue = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestInternalPduId].PduStartPos]);
  }

  return retValue;
}

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)

/*-----------------------------[XcpR_GetFlexRayMultiPduInFrameNrOfFillBytes]------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_GetFlexRayMultiPduInFrameNrOfFillBytes
(
  PduLengthType PackageLength,
  PduLengthType DataLength,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_APPL_DATA) DataLengthPlusFill,
  PduIdType Conf_DestInternalPduId
)
{
  PduLengthType length = DataLength;

  /* get the number of bytes of alignment */
  const PduLengthType NumberOfBytesAllignment = XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.FlexRayPackageAlignment;

  /* Check if the packet (actual data) of the message is aligned to the packet alignment,
   * which is the header length. If not, calculate necessary nr of fill bytes to tail  */
  PduLengthType MessageUnalignedBytes = PackageLength % NumberOfBytesAllignment;

  if (MessageUnalignedBytes > 0U)
  {
    length += NumberOfBytesAllignment - MessageUnalignedBytes;
  }

  *DataLengthPlusFill = length;
}

/*-----------------------------[XcpR_AddFlexRayMultiPduInFrameFill]------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_AddFlexRayMultiPduInFrameFill
(
  PduLengthType PackageLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType DataLength,
  PduIdType Conf_DestInternalPduId
)
{

  /* get the number of bytes of alignment */
  const PduLengthType NumberOfBytesAllignment = XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.FlexRayPackageAlignment;

  /* Check if the packet (actual data) of the message is aligned to the packet alignment,
   *  which is the header length. If not, add fill bytes to tail  */
  PduLengthType MessageUnalignedBytes = PackageLength % NumberOfBytesAllignment;

  if (MessageUnalignedBytes > 0U)
  {
    /* iterator */
    uint8 i;
    for (i = 0U; i<(NumberOfBytesAllignment - MessageUnalignedBytes); i++)
    {
      XcpRDestBufferPtr[DataLength+i] = 0x00;
    }
  }
}

/*-----------------------------[XcpR_AddFlexRayMultiPduInFrameLastMessage]------------------*/

STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_AddFlexRayMultiPduInFrameLastMessage
(
  PduLengthType AvailableTxSpace,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduIdType Conf_DestInternalPduId
)
{
  Std_ReturnType retValue = FALSE;

    if (AvailableTxSpace >=  1U)
    {
      /* add an empty FlexRay header
       * LEN = 0 for detecting the end of the used payload segment */
      XcpR_AddFrameHeader(0U, XcpRDestBufferPtr, 1U, FALSE, Conf_DestInternalPduId);
      retValue = TRUE;
    }

  return retValue;
}
#endif /* #if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */

#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
/*-----------------------------[XcpR_AddFlexRayFrameEnding]------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_AddFlexRayFrameEnding
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduIdType DestInternalPduId
)
{

  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) LocalXcpRDestBufferPtr = XcpRDestBufferPtr;

    /* Add a last message with only a header with LEN = 0 */
    if (XcpR_AddFlexRayMultiPduInFrameLastMessage(XcpR_DestPduData.PduAvailableTxSpace[DestInternalPduId], LocalXcpRDestBufferPtr, DestInternalPduId) == TRUE)
    {
      /* update Tx package size */
      XcpR_DestPduData.PduLength[DestInternalPduId] += 1U;

      /* An empty message was added, move payload pointer to the next byte */
      LocalXcpRDestBufferPtr = &LocalXcpRDestBufferPtr[1U];
    }

    /* Check if the full payload needs to be rounded up */
    {
      if( (XcpR_DestPduData.PduLength[DestInternalPduId] & 1U) == 1U)
      {
        /* add tail */
        LocalXcpRDestBufferPtr[0U] = 0x00;
        /* update Tx package size */
        XcpR_DestPduData.PduLength[DestInternalPduId] += 1U;
      }
    }

}
#endif /* (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */

#if (((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && \
     ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON ))) || \
      (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))

/*-----------------------------[XcpR_AddFrameHeader]------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_AddFrameHeader
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType HeaderLength,
  boolean FirstPacket,
  PduIdType Conf_DestInternalPduId
)
{
#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  if ((XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) && ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[Conf_DestInternalPduId].Properties) != 0U))
  {
    /* add the header only if packing of multiple PDUs
    is enabled for this connection */
    {
      /* write only the data Length into frame header */
      XcpRDestBufferPtr[HeaderLength - 1U] = (uint8)(DataLength);
    }
  }
#endif  /* XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON */

#if (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON)
  if ((XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY) && ((XCPR_TRANSMISSION_SEQUENCE_CORRECTION & XcpR_DestConf[Conf_DestInternalPduId].Properties) != 0U))
  {
  /* add the sequence correction counter */
    {
      /* Add only if it's not an empty length */
      if (DataLength != 0U)
      {
#if (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
        if ((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[Conf_DestInternalPduId].Properties) != 0U)
        {
          /* Write the sequence counter only in the header of the first packet */
          if (FirstPacket == TRUE)
          {
            /* write the frame transmit counter */
            XcpRDestBufferPtr[XCPR_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX] = (uint8)XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId];

            /* Increment header frame transmit counter */
            XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] += 1U;
            /* Since the counter is a 16bit, we need to overflow at 8bit for FlexRay*/
            XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] %= 256U;
          }
        }
        else
#endif
        {
          /* write the frame transmit counter */
          XcpRDestBufferPtr[XCPR_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX] = (uint8)XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId];

          /* Increment header frame transmit counter */
          XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] += 1U;
          /* Since the counter is a 16bit, we need to overflow at 8bit for FlexRay*/
          XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] %= 256U;
        }
      }
    }
  }
#endif  /* XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON */
#endif /*XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON */

#if (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)
  if ((XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) ||
      (XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))
  {
    /* Ethernet header (4 bytes always Intel Format) */
    /* add data length information */
    XcpRDestBufferPtr[XCPR_BYTE0_INDEX] = (uint8)DataLength;
    XcpRDestBufferPtr[XCPR_BYTE1_INDEX] = (uint8)(DataLength >> 8U);
    /* add frame counter information */
    {
      XcpRDestBufferPtr[XCPR_BYTE2_INDEX] = (uint8)XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId];
      XcpRDestBufferPtr[XCPR_BYTE3_INDEX] = (uint8)(XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] >> 8U);
      /* Increment Ethernet header frame transmit counter */
      XcpR_DestPduData.FrameTransmitCounter[XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.IntCounterId] += 1U;
    }
  }
#endif /* (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON) */

#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_OFF) || \
     (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF))
  TS_PARAM_UNUSED(HeaderLength);
#endif

#if ((XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_OFF) || \
     ((XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF)))
  TS_PARAM_UNUSED(FirstPacket);
#endif

}
#endif /* (((XCPR_ON_FLEXRAY_ENABLED == STD_ON) && \
           ((XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) || (XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT == STD_ON )))) */

#if (XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED == STD_ON)
/*-----------------------------[XcpR_AddCanFDFill]------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_AddCanFDFill
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_APPL_DATA) DataLength,
  PduLengthType AvailableFrameLength,
  PduIdType Conf_DestInternalPduId
)
{
  /* array index */
  uint8 index;

  /* array containing CANFD DLC values */
  PduLengthType ValidDlcValues[8U] = {8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};

  /* local variable used to calculate the valid DLC */
  PduLengthType ValidLength = 0U;

  PduLengthType length = *DataLength;

  /*  check configuration of MaxDlcRequired */
#if (XCPR_DEST_MAX_DLC_REQUIRED == STD_ON)
  if ((XCPR_TRANSMISSION_MAX_DLC_ENABLED & XcpR_DestConf[Conf_DestInternalPduId].Properties) != 0U)
  {
    /* check if length equals configured MaxDlcValue */
    if((length != XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.MaxDlcValue) &&
       (AvailableFrameLength >= XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.MaxDlcValue ))
    {
      /* Add fill bytes until the length configured */
      TS_MemSet(&XcpRDestBufferPtr[length], XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.FillValue,
          ((usize)XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.MaxDlcValue - (usize)length));
      /* update the length */
      length = XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.MaxDlcValue;
    }
  }
  else
#endif
  {
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
        TS_MemSet(&XcpRDestBufferPtr[length], XcpR_DestConf[Conf_DestInternalPduId].BusAttributes.FillValue, ((usize)ValidLength - (usize)length));
        length = ValidLength;
      }
    }
  }

  /* update DataLength with the new value */
  *DataLength = length;
}
#endif /* #if (XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED == STD_ON) */

#if (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON)
/*-----------------------------[XcpR_InitFlexRayBuffer]---------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_InitFlexRayBuffer
(
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) DestPtr,
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) DestBufferPtr
)
{
  uint8 Iterator;

  /* add the NAX address */
  DestBufferPtr[0U] = DestPtr->BusAttributes.FlexRayNax;
  /* initialize the FlexRay header */
  for(Iterator = 1U; Iterator < DestPtr->PduAttributes.PduHeaderLength; Iterator++)
  {
    DestBufferPtr[Iterator] = 0U;
  }

}
#endif /* (XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) */

STATIC FUNC(void, XCPR_CODE) XcpR_GetDestInternalPduId
(
  P2VAR(PduIdType, AUTOMATIC, XCPR_VAR_CLEARED) DestInternalPduIdPtr
)
{

  if (XcpR_Status.CurrentSourceOrigin == XCPR_MASTER_SRC_ORIGIN)
  {
    *DestInternalPduIdPtr = XcpR_Status.DestinationFromMasterInternalId;
  }
  else /* if (XcpR_Status.CurrentSourceOrigin == XCPR_SLAVE_SRC_ORIGIN) */
  {
    *DestInternalPduIdPtr = XcpR_Status.DestinationFromSlaveInternalId;
  }

}

/*------------------[XcpR_ProcessActiveDestination]-------------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessActiveDestination
(
  PduIdType Conf_DestInternalPduId
)
{
  /* Pointers to the Dest configuration array */
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_DestPduIdPtr;
  /* Pointer to XcpR Dest runtime data */
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr;
  Std_ReturnType PduTxStatus;
  boolean AbortTransmission = FALSE;
  boolean ReportDetInternalBus = FALSE;
  boolean ReportDetExternalBus = FALSE;

  PduLengthType Conf_DestPduHeaderLength;
  uint8 DestPduType;
  PduIdType activeDestPduId;

  /* PduInfo to be transmitted */
  PduInfoType RxPduInfo, TxPduInfo;

  /* get PDU start position inside the SrcPdu buffer */
  XcpRDestBufferPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[Conf_DestInternalPduId].PduStartPos]);

  /* Pointer to the Dest configuration array */
  Conf_DestPduIdPtr = &XcpR_DestConf[Conf_DestInternalPduId];
  /* get the Destination PDU header length */
  Conf_DestPduHeaderLength = Conf_DestPduIdPtr->PduAttributes.PduHeaderLength;

  DestPduType = Conf_DestPduIdPtr->PduType;
  /* get the Transmit pdu id */

  /* Set CONNECT package flag */
  if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
  {
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* If the PID in the transmitted package is Connect */
    if ((XcpRDestBufferPtr[Conf_DestPduHeaderLength] == 0xFF) && (XcpR_Status.isPreviousCommandConnect == FALSE))
    {
      /* !LINKSTO XcpR.dsn.InternalState.PreviousCommandConnect,1 */
      XcpR_Status.isPreviousCommandConnect = TRUE;
    }
    /* If a CONNECT positive response is received from the Xcp slave */
    else if ((XcpRDestBufferPtr[Conf_DestPduHeaderLength] == 0xFF) &&
             (XcpR_Status.isPreviousCommandConnect == TRUE) &&
             (XcpR_DestConf[Conf_DestInternalPduId].IsMasterConnection == TRUE)
            )
    {
      /* !LINKSTO XcpR.dsn.InternalState.WaitingConnectConfirmation,1 */
      XcpR_Status.isPreviousCommandConnect = FALSE;
      XcpR_Status.isWaitingForConnectConfirmation = TRUE;
    }
    else
    {
      /* Nothing to do */
    }

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* nothing to do */
  }

  /* Store only the connection mode.
   * The Connection Status is saved only after a TxConfirmation.
   * However, XcpR needs to check the connection mode when deciding to forward a TriggerTransmit to the UL.
   * After a LL_Transmit, first XcpR_TriggerTransmit() is invoked, and only afterwards, XcpR_TxConfirmation().
   * As a consequence, the connection mode needs to be set at an earlier point.
   */
  XcpR_Status.ConnectionMode = XcpR_ActiveConnectionMode;

  if (DestPduType == XCPR_RX_PDU_DIRECTION)
  {
    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* Prepare RxPduInfo pointer */
    RxPduInfo.SduDataPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[Conf_DestInternalPduId].PduStartPos]);
    RxPduInfo.SduLength  = XcpR_DestPduData.PduLength[Conf_DestInternalPduId];

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* If the identified destination is of type RX, clearly its corresponding source originates from a master Xcp */
    XcpR_DestConf[Conf_DestInternalPduId].ULRxIndicationPtr(XcpR_DestConf[Conf_DestInternalPduId].TLDestPduId, &RxPduInfo);
  }
  else /* if (DestPduType == XCPR_TX_PDU_DIRECTION) */
  {

    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
    /* If ConnectMode for XcpR is 1, pass ConnectMode 0 to the Xcp slave */
    if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
    {
      if (XcpRDestBufferPtr[Conf_DestPduHeaderLength] == 0xFF)
      {
        if (XcpRDestBufferPtr[Conf_DestPduHeaderLength + 1U] == 1U)
        {
          /* !LINKSTO XcpR.Impl.RemoteConnections.ConnectMode, 1 */
          XcpRDestBufferPtr[Conf_DestPduHeaderLength + 1U] = 0U;
        }
      }
    }
    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    XcpR_PrepareTxData(Conf_DestInternalPduId, &activeDestPduId, &TxPduInfo);

    PduTxStatus = XcpR_DestConf[Conf_DestInternalPduId].LLTransmitPtr(activeDestPduId, &TxPduInfo);

    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    if (PduTxStatus == E_NOT_OK)
    {
      XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
      if ((XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT &  XcpR_DestConf[Conf_DestInternalPduId].Properties) != 0U)
      {
        XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
      }
#endif
      XcpR_ResetStatusForConnectCommand(XcpRDestBufferPtr, Conf_DestPduHeaderLength);
    }

    XcpR_ProcessRetry(Conf_DestInternalPduId, PduTxStatus, &AbortTransmission);

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    if (AbortTransmission == TRUE)
    {
      SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      /* !LINKSTO XcpR.dsn.TxRetry.DiscardMessage,1 */
      /* configured number of retries performed - discard the message
         and make the buffer available for a new Tx Pdu */
      XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &=
        (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));

      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      XcpR_DisconnectOrSessionTerminated(&ReportDetInternalBus, &ReportDetExternalBus, Conf_DestInternalPduId);

      XcpR_ClearStatusFlags();
    }

    if (ReportDetInternalBus == TRUE)
    {
      XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INTERNAL_BUS);
    }
    if (ReportDetExternalBus == TRUE)
    {
      XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_EXTERNAL_BUS);
    }

  }
}

STATIC FUNC(void, XCPR_CODE) XcpR_DisconnectOrSessionTerminated
(
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ReportDetInternalBusPtr,
  P2VAR(boolean, AUTOMATIC, XCPR_VAR) ReportDetExternalBusPtr,
  PduIdType Conf_DestInternalPduId
)
{
  boolean TriggerDisconnect = FALSE;
#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
  boolean EmitSessionTerminated = FALSE;
#endif
  PduIdType LocalDestFromMasterInternalId = (PduIdType) ~0U;

  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  /* !LINKSTO XcpR.dsn.TxRetry.RuntimeError.MasterToSlave,1 */
  /* XcpR could not transmit to Xcp Slave */
  if (Conf_DestInternalPduId == XcpR_Status.DestinationFromMasterInternalId)
  {
    *ReportDetInternalBusPtr = TRUE;
  }
  else
  {
    /* !LINKSTO XcpR.dsn.TxRetry.RuntimeError.SlaveToMaster,1 */
    /* DET runtime error */
    *ReportDetExternalBusPtr = TRUE;

    /* message is a response from remote Xcp */
    if (XcpR_Status.DestinationFromMaster.Type == XCPR_TX_PDU_DIRECTION)
    {
      if (XcpR_Status.isPreviousCommandConnect == TRUE)
      {
        TriggerDisconnect = TRUE;
      }
      else if (XcpR_DisconnectPending == TRUE)
      {
        /* !LINKSTO XcpR.dsn.TxRetry.SlaveToMaster.DisconnectXcpR,1 */
        XcpR_Status.ConnectionStatus = XCPR_STATE_DISCONNECTED;
#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
        EmitSessionTerminated = TRUE;
#endif
      }
      else
      {
        /* nothing else to do */
      }
    }
    else
    {
      TriggerDisconnect = TRUE;
    }
  }

  LocalDestFromMasterInternalId = XcpR_Status.DestinationFromMasterInternalId;

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  if (TriggerDisconnect == TRUE)
  {
    /* !LINKSTO XcpR.dsn.TxRetry.TriggerAutonomousDisconnect,1 */
    /* trigger autonomous disconnect */
    XcpR_TriggerDisconnectCommand(LocalDestFromMasterInternalId, TRUE);
  }
#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
  if (EmitSessionTerminated == TRUE)
  {
    /* !LINKSTO XcpR.dsn.BusMonitor.SendSessionTerminatedEvent,1 */
    XcpR_EmitSessionTerminatedEvent();
  }
#endif
}

/*------------------[XcpR_IndicateTxConfirmation]---------------------------*/

FUNC(void, XCPR_CODE) XcpR_IndicateTxConfirmation
(
  PduIdType XcpRTxPduId
)
{
  PduIdType Conf_DestInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  PduIdType TxDestIterator;

#if (XCPR_TRANSMIT_FROM_CONFIRMATION == STD_ON)
  boolean isTxIdle = FALSE;
#endif

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
  uint8 ConnGroupIterator, RoutingPathIterator;
#endif

#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  if (XcpRTxPduId >= XCPR_NO_OF_TX_DEST_PDU_IDS)
  {
    XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TX_CONFIRMATION, XCPR_E_INVALID_PDUID);
  }
  else
#endif
  {

    for (TxDestIterator = 0U; TxDestIterator < XCPR_NO_OF_TX_DEST_PDU_IDS; TxDestIterator++)
    {
      if (XcpRTxPduId == XcpR_DestConf[TxDestIterator].XcpRDestPduId)
      {
        Conf_DestInternalPduId = XcpR_DestConf[TxDestIterator].IntPduBufId;
      }
    }

    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
    /* Clear flags */
    if ((XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId))) != 0U)
    {
      /* clear the waiting for confirmation flag to prevent Tx indication twice */
      XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
      XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
#endif
      /* clear flag to indicate that the Tx buffer is ready for new data */
      XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_DestInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_DestInternalPduId)));
      /* Clear the bus timeout counter */
      XcpR_DestPduData.BusTimeoutCntVal[Conf_DestInternalPduId] = 0U;
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)
      /* Clear the tx bus retry counter */
      XcpR_DestPduData.BusTxRetryCount[Conf_DestInternalPduId] = 0U;
#endif
#if (XCPR_TRANSMIT_FROM_CONFIRMATION == STD_ON)
      /* Tx is now idle */
      isTxIdle = TRUE;
#endif
    }

    if (XcpR_DestConf[Conf_DestInternalPduId].IsMasterConnection == TRUE)
    {
      if ((XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED) && (XcpR_Status.isWaitingForConnectConfirmation == TRUE))
      {
        XcpR_Status.isWaitingForConnectConfirmation = FALSE;
        /* !LINKSTO XcpR.Impl.SaveConnectedState, 1 */
        /* Update XcpR connection status */
        XcpR_Status.ConnectionStatus = XCPR_STATE_CONNECTED;
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
        for (ConnGroupIterator = 0U; ConnGroupIterator < XCPR_NO_OF_CONNECTION_GROUPS; ConnGroupIterator++)
        {
          for (RoutingPathIterator = 0U; RoutingPathIterator < XCPR_MAX_ROUTING_PATHS_PER_CONNECTION; RoutingPathIterator++)
          {
            if ((XcpR_Status.MasterSource.Id == (XcpR_ConnectionGroups[ConnGroupIterator].RoutingPath[RoutingPathIterator].XcpRSrcPduId)) &&
                (XcpR_Status.DestinationFromMasterInternalId == (XcpR_ConnectionGroups[ConnGroupIterator].RoutingPath[RoutingPathIterator].RoutedDestinationPtr->IntPduBufId)))
            {
              /* !LINKSTO XcpR.Impl.ConnectionGroups.ActiveGroupId, 1 */
              XcpR_Status.ActiveConnectionGroup = &XcpR_ConnectionGroups[ConnGroupIterator];
            }
          }
        }
#endif
      }
    }

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  if (XcpR_DestConf[Conf_DestInternalPduId].IsMasterConnection == TRUE)
  {

    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    if (XcpR_DisconnectPending == TRUE)
    {
      /* !LINKSTO XcpR.Impl.ResetRoutingPath, 1 */
      /* Flush msg queue */
      XcpR_FlushMsgQueue();

      XcpR_Status.ConnectionStatus = XCPR_STATE_DISCONNECTED;
      XcpR_Status.isPreviousCommandConnect = FALSE;
      XcpR_Status.isWaitingForConnectConfirmation = FALSE;

      /* Initialize source processor */
      XcpR_InitReceiveProcessor();

      /* Initialize destination processor */
      XcpR_InitTransmitProcessor();

      XcpR_Status.MasterSource.Id = (PduIdType) XCPR_INVALID_PDU_ID;
      XcpR_Status.MasterSource.Type = XCPR_INVALID_SOURCE_TYPE;
      XcpR_Status.DestinationFromMaster.Id = (PduIdType) XCPR_INVALID_PDU_ID;
      XcpR_Status.DestinationFromMaster.Type = XCPR_INVALID_DESTINATION_TYPE;
      XcpR_Status.DestinationFromMasterInternalId = (PduIdType) XCPR_INVALID_PDU_ID;

      XcpR_Status.SlaveSource.Id = (PduIdType) XCPR_INVALID_PDU_ID;
      XcpR_Status.SlaveSource.Type = XCPR_INVALID_SOURCE_TYPE;
      XcpR_Status.DestinationFromSlave.Id = (PduIdType) XCPR_INVALID_PDU_ID;
      XcpR_Status.DestinationFromSlave.Type = XCPR_INVALID_DESTINATION_TYPE;
      XcpR_Status.DestinationFromSlaveInternalId = (PduIdType) XCPR_INVALID_PDU_ID;

      XcpR_Status.CurrentSourceOrigin = XCPR_UNKNOWN_SRC_ORIGIN;
      XcpR_Status.ConnectionMode = XCPR_INVALID_CONNECTION_MODE;

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      /* !LINKSTO XcpR.Impl.ConnectionGroups.ActiveGroupInformationReset, 1 */
      XcpR_Status.CurrentSrcLinkedDestInternalId = (PduIdType) XCPR_INVALID_PDU_ID;
      XcpR_Status.ActiveConnectionGroup = NULL_PTR;
#endif
    }

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  }

#if (XCPR_TRANSMIT_FROM_CONFIRMATION == STD_ON)
    /* check if transmission from Tx confirmation is enabled for this PDU */
    if ((TRUE == isTxIdle) && (XcpR_DestConf[XcpRTxPduId].ProcessingType == XCPR_TRANSMISSION_ON_TX_CONFIRMATION))
    {
      /* Trigger next transmit */
      /* !LINKSTO XcpR.Impl.TransmitFromTxConfirmation, 1 */
      XcpR_ProcessDestinations();
    }
#endif
  }
}
#if ((XCPR_TX_BUS_RETRY_COUNT > 0U) || (XCPR_TX_RETRY_COUNT > 0))
/*------------------[XcpR_ProcessTxRetryDestinations]---------------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessTxRetryDestinations
(
  void
)
{
  uint8 Process_Iterator;

  /* TxBuffer is not available for transmission and Retry is performed */
  for (Process_Iterator = 0U; Process_Iterator < XCPR_NO_OF_TX_DESTINATION_PDUS; Process_Iterator++)
  {
    /* check if this destination is marked for retry transmission */
    if (XcpR_DestPduData.NeedsRetry[Process_Iterator] == TRUE)
    {
      SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(Process_Iterator)] |= (uint8)(1U << XCPR_FLAG_IDX(Process_Iterator));
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
      if ((XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT &  XcpR_DestConf[Process_Iterator].Properties) != 0U)
      {
        XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(Process_Iterator)] |= (uint8)(1U << XCPR_FLAG_IDX(Process_Iterator));
      }
#endif
      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      XcpR_ProcessActiveDestination(Process_Iterator);
    }
  }
}
#endif
/*------------------[XcpR_ProcessRetry]---------------------------*/

STATIC FUNC(void, XCPR_CODE) XcpR_ProcessRetry
(
  PduIdType TxPduId,
  Std_ReturnType TxStatus,
  P2VAR(boolean, AUTOMATIC, XCPR_APPL_DATA) AbortTxPtr
)
{
  /* variable used to signal that the abort transmission event was reached */
  boolean AbortTransmission = TRUE;
#if (XCPR_TX_RETRY_COUNT > 0)
  /* select retry counter */
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) RetryCntValPtr =
      &XcpR_DestPduData.RetryCntVal[TxPduId];
#endif

  if (TxStatus == E_OK)
  {
#if (XCPR_TX_RETRY_COUNT > 0)
    /* no error detected at transmission, so reset the value of retry
     * mechanism to default value */
    *RetryCntValPtr = XCPR_TX_RETRY_COUNT;
    XcpR_DestPduData.NeedsRetry[TxPduId] = FALSE;
#endif /* (XCPR_TX_RETRY_COUNT > 0) */
    /* no error detected at transmission, so there is nothing to abort */
    AbortTransmission = FALSE;
  }
  else
  {
#if (XCPR_TX_RETRY_COUNT > 0)
    /* compute abort event */
    if (*RetryCntValPtr > 0U)
    {
      XcpR_DestPduData.NeedsRetry[TxPduId] = TRUE;
      /* Decrement the Retry Counter's value until reaches 0 */
      *RetryCntValPtr -= 1U;
      /* the abort event was not yet detected */
      AbortTransmission = FALSE;
    }
    else
    {
      /* aborting, reset the retry counter */
      *RetryCntValPtr = XCPR_TX_RETRY_COUNT;
      XcpR_DestPduData.NeedsRetry[TxPduId] = FALSE;
    }
#endif /* (XCPR_TX_RETRY_COUNT > 0) */
  }
#if (XCPR_TX_RETRY_COUNT == 0)
  TS_PARAM_UNUSED(TxPduId);
#endif
  /* update abort transmission signal */
  *AbortTxPtr = AbortTransmission;
}

/*------------------[XcpR_ClearStatusFlags]---------------------------*/

FUNC(void, XCPR_CODE) XcpR_ClearStatusFlags
(
  void
)
{
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  XcpR_Status.isPreviousCommandConnect = FALSE;
  XcpR_Status.isWaitingForConnectConfirmation = FALSE;
  XcpR_DisconnectCommandPending = FALSE;
  XcpR_DisconnectPending = FALSE;

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
}

FUNC(void, XCPR_CODE) XcpR_TriggerDisconnectCommand
(
  PduIdType DestinationId,
  boolean EmitSessionTerminated
)
{
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr;
#endif
  /* prepare Pid for disconnect command */
  uint8 XcpR_DisconnectMessage = 0xFEU;
  PduLengthType PackageLength = 1U;
  boolean IsFirstPacket = TRUE;
  boolean IsLastPacketInFrame = FALSE;
  /* PduInfo to be transmitted */
  PduInfoType RxPduInfo, TxPduInfo;
  PduIdType activeDestPduId;
  /* Pointer to the Dest configuration array */
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_DestPduIdPtr
    = &XcpR_DestConf[DestinationId];

#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
  if (TRUE == EmitSessionTerminated)
  {
    /* !LINKSTO XcpR.dsn.BusMonitor.SendSessionTerminatedEvent,1 */
    XcpR_EmitSessionTerminatedEvent();
  }
#else
  TS_PARAM_UNUSED(EmitSessionTerminated);
#endif

  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  /* pack the command with the header */
  /* CHECK: NOPARSE */
  if (E_OK == XcpR_PackTxBufferData(DestinationId, PackageLength, IsFirstPacket, &IsLastPacketInFrame,
                              &XcpR_DisconnectMessage, XCPR_SID_TRIGGER_DISCONNECT))
  /* CHECK: PARSE */
  {
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
    /* Check if multiple packing is enabled for this Destination Pdu Id */
    if (((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[DestinationId].Properties) != 0U) && (XcpR_DestConf[DestinationId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY))
    {
      /* Point to the initial address and add the LastPosition value */
      XcpRDestBufferPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestinationId].PduStartPos]);
      XcpRDestBufferPtr = &XcpRDestBufferPtr[XcpR_DestPduData.PduMultiPackLastPosition[DestinationId]];

      XcpR_AddFlexRayFrameEnding(XcpRDestBufferPtr, DestinationId);
    }
#endif
  }
  /* CHECK: NOPARSE */
  else
  {
   /* Unreachable code.
    * XcpR_PackTxBufferData() can return E_NOT_OK only if there is not sufficient
    * space in the tx buffer, however, this would be the first message in the transmit buffer,
    * of only 2 bytes, so there is no scenario where it would not be sufficient space, considering
    * that everything that has been packed until this point has already been transmitted. */
    XCPR_UNREACHABLE_CODE_ASSERT(XCPR_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  /* !LINKSTO XcpR.dsn.TxRetry.SlaveToMaster.TriggerAutonomousDisconnect,1 */
  if (Conf_DestPduIdPtr->PduType == XCPR_RX_PDU_DIRECTION)
  {
    /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* Prepare RxPduInfo pointer */
    RxPduInfo.SduDataPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestinationId].PduStartPos]);
    RxPduInfo.SduLength  = XcpR_DestPduData.PduLength[DestinationId];

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* If the identified destination is of type RX, clearly its corresponding source originates from a master Xcp */
    XcpR_DestConf[DestinationId].ULRxIndicationPtr(XcpR_Status.DestinationFromMaster.Id, &RxPduInfo);
  }
  else
  {
    XcpR_PrepareTxData(DestinationId, &activeDestPduId, &TxPduInfo);
    /* Transmit Disconnect command */
    (void)XcpR_DestConf[DestinationId].LLTransmitPtr(activeDestPduId, &TxPduInfo);
  }

  /* mark the status as autonomous disconnect triggered */
  TS_AtomicAssignGeneric(XcpR_Status.ConnectionStatus, XCPR_STATE_DISCONNECT_REQUESTED);
}

/*------------------[XcpR_EmitSessionTerminatedEvent]---------------------------*/
#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)

FUNC(void, XCPR_CODE)  XcpR_EmitSessionTerminatedEvent(void)
{
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRDestBufferPtr;
#endif
  uint8 XcpR_SessionTerminatedEventMsg[2];
  boolean IsFirstPacket = TRUE;
  boolean IsLastPacketInFrame = FALSE;
  PduLengthType PackageLength = 2U;
  PduInfoType TxPduInfo;

  XcpR_SessionTerminatedEventMsg[0x00] = 0xFDU;
  XcpR_SessionTerminatedEventMsg[0x01] = 0x07U;

  /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  /* pack the command with the header */
  /* CHECK: NOPARSE */
  if (E_OK == XcpR_PackTxBufferData(XcpR_Status.DestinationFromSlaveInternalId, PackageLength, IsFirstPacket, &IsLastPacketInFrame,
                              &XcpR_SessionTerminatedEventMsg[0U], XCPR_SID_SESSION_TERMINATED))
  /* CHECK: PARSE */
  {
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
    if (((XCPR_TRANSMISSION_MULTIPLE_PACKING & XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].Properties) != 0U) && (XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY))
    {
      /* Point to the initial address and add the LastPosition value */
      XcpRDestBufferPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].PduStartPos]);
      XcpRDestBufferPtr = &XcpRDestBufferPtr[XcpR_DestPduData.PduMultiPackLastPosition[XcpR_Status.DestinationFromSlaveInternalId]];

      XcpR_AddFlexRayFrameEnding(XcpRDestBufferPtr, XcpR_Status.DestinationFromSlaveInternalId);
    }
#endif
  }
  /* CHECK: NOPARSE */
  else
  {
   /* Unreachable code.
    * XcpR_PackTxBufferData() can return E_NOT_OK only if there is not sufficient
    * space in the tx buffer, however, this would be the first message in the transmit buffer,
    * of only 2 bytes, so there is no scenario where it would not be sufficient space, considering
    * that everything that has been packed until this point has already been transmitted. */
    XCPR_UNREACHABLE_CODE_ASSERT(XCPR_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */

  TxPduInfo.SduDataPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].PduStartPos]);
  TxPduInfo.SduLength  = XcpR_DestPduData.PduLength[XcpR_Status.DestinationFromSlaveInternalId];
  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  /* send session terminated event to Master Xcp */
  XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].LLTransmitPtr(XcpR_Status.DestinationFromSlave.Id, &TxPduInfo);
}
#endif

/*------------------[XcpR_ProcessBusMonitor]---------------------------*/

FUNC(void, XCPR_CODE) XcpR_ProcessBusMonitor(void)
{
  PduIdType Iterator;
  P2VAR(XcpR_BusTimeoutCounterValueType, AUTOMATIC, XCPR_APPL_DATA) BusTimeoutCtrValPtr;

  boolean ReportDetNoTxExternalBus = FALSE;
  boolean ReportDetNoTxInternalBus = FALSE;
  /* local variables to read global data inside a critical section */
  boolean LocalTxPduActive = FALSE;
  boolean TriggerDisconnect = FALSE;
  XcpR_BusTimeoutCounterValueType LocalBusTimeoutCtrVal = (XcpR_BusTimeoutCounterValueType) ~0U;
  PduIdType LocalDestFromSlaveInternalId = (PduIdType) ~0U;
  PduIdType LocalDestFromMasterInternalId = (PduIdType) ~0U;
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)
  uint8 LocalBusTxRetryCount = (uint8) ~0U;
#endif

  if (XcpR_AutonomousDisconnectPending == TRUE)
  {
    TS_AtomicAssignGeneric(XcpR_AutonomousDisconnectPending, FALSE);
    TS_AtomicAssignGeneric(XcpR_Status.ConnectionStatus, XCPR_STATE_DISCONNECTED);
  }
  else if (XcpR_NoTxConfOnInternalBus == TRUE)
  {
    TS_AtomicAssignGeneric(XcpR_NoTxConfOnInternalBus, FALSE);
  }
  else
  {
    /* check the status for available Tx buffers. */
    for (Iterator = 0U; (Iterator < XCPR_NO_OF_TX_DESTINATION_PDUS); Iterator++)
    {
      SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      /* select the bus timeout counter */
      BusTimeoutCtrValPtr = &XcpR_DestPduData.BusTimeoutCntVal[Iterator];

      LocalTxPduActive = XCPR_IS_TX_PDU_ACTIVE(XcpR_DestConf[Iterator].IntPduBufId);
      LocalBusTimeoutCtrVal = *BusTimeoutCtrValPtr;

      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      if (TRUE == LocalTxPduActive)
      {
        if (LocalBusTimeoutCtrVal < XCPR_TX_BUS_TO_COUNTER_MAX_VAL)
        {
          SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* increment the Bus Timeout Counter's value until it reaches the maximum configuration value */
          *BusTimeoutCtrValPtr += 1U;

          SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
        }
        else
        {
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)

          SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          LocalBusTxRetryCount = XcpR_DestPduData.BusTxRetryCount[Iterator];

          SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* !LINKSTO XcpR.dsn.BusMonitor.RetryTransmission,1 */
          /* A bus timeout occurred, check if we should retry the transmission */
          if (LocalBusTxRetryCount < XCPR_TX_BUS_RETRY_COUNT)
          {
            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            /* Retry the transmission */
#if (XCPR_TX_BUS_RETRY_COUNT < XCPR_INFINITE_RETRIES)
            /* !LINKSTO XcpR.dsn.BusMonitor.RetryTransmisionWithoutAutonDisconnect,1 */
            /* Increment the retry count only if we shouldn't repeat indefinitely */
            XcpR_DestPduData.BusTxRetryCount[Iterator]++;
#endif
            /* Clear the pending flags */
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
            XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(XcpR_DestConf[Iterator].IntPduBufId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(XcpR_DestConf[Iterator].IntPduBufId)));
#endif /* ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
            XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(XcpR_DestConf[Iterator].IntPduBufId)]  &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(XcpR_DestConf[Iterator].IntPduBufId)));
            /* Reset the bus timeout counter */
            *BusTimeoutCtrValPtr = 0U;
            XcpR_DestPduData.NeedsRetry[Iterator] = TRUE;

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
          }
          else
#endif /* XCPR_TX_BUS_RETRY_COUNT > 0U */
          {
            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            LocalDestFromSlaveInternalId = XcpR_Status.DestinationFromSlaveInternalId;

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            if (Iterator == LocalDestFromSlaveInternalId)
            {
              ReportDetNoTxExternalBus = TRUE;
#if (XCPR_EVENT_PACKET_ENABLED == STD_ON)
              /* !LINKSTO XcpR.dsn.BusMonitor.SendSessionTerminatedEvent,1 */
              XcpR_EmitSessionTerminatedEvent();
#endif

              SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

              /* !LINKSTO XcpR.dsn.BusMonitor.DisconnectAfterFailedRetries,1 */
              /* XcpR connected to local ECU or disconnect is pending */
              if ((XcpR_DestConf[XcpR_Status.DestinationFromMasterInternalId].PduType == XCPR_RX_PDU_DIRECTION) ||
                  (TRUE == XcpR_DisconnectPending))
              {
                XcpR_AutonomousDisconnectPending = TRUE;
              }
              else
              {
                TriggerDisconnect = TRUE;
              }

              LocalDestFromMasterInternalId = XcpR_Status.DestinationFromMasterInternalId;

              SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

              if (TriggerDisconnect == TRUE)
              {
                /* !LINKSTO XcpR.dsn.BusMonitor.TriggerAutonomousDisconnect,1 */
                /* trigger autonomous disconnect from remote Xcp */
                XcpR_TriggerDisconnectCommand(LocalDestFromMasterInternalId, FALSE);
              }
            }
            else
            {
              /* !LINKSTO XcpR.dsn.BusMonitor.RuntimeError.DisconnectAfterFailedRetries,1 */
              /* no TxConfirmation from internal bus */
              ReportDetNoTxInternalBus = TRUE;
              XcpR_NoTxConfOnInternalBus = TRUE;
            }

            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            /* If there was a CONNECT attempt and a positive response was received from the slave Xcp,
             * but there was no TxConfirmation and the timeout counter expired, reset the value of ConnectionMode */
            if ((XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED) && (XcpR_Status.isWaitingForConnectConfirmation == TRUE))
            {
              XcpR_Status.ConnectionMode = XCPR_INVALID_CONNECTION_MODE;
            }

            /* clear status flags */
            XcpR_ClearStatusFlags();
#if (XCPR_TX_BUS_RETRY_COUNT > 0U)
            XcpR_DestPduData.BusTxRetryCount[Iterator] = 0U;
#endif
            *BusTimeoutCtrValPtr = 0U;
            /* Clear the pending flags */
#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
            XcpR_DestPduData.IsPduWaitingForTriggFlag[XCPR_BYTE_IDX(XcpR_DestConf[Iterator].IntPduBufId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(XcpR_DestConf[Iterator].IntPduBufId)));
#endif /* ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */
            XcpR_DestPduData.IsPduWaitingForConfFlag[XCPR_BYTE_IDX(XcpR_DestConf[Iterator].IntPduBufId)]  &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(XcpR_DestConf[Iterator].IntPduBufId)));
            /* discard the message and make the buffer available for a new Tx Pdu */
            XcpR_DestPduData.IsBufferFullFlag[XCPR_BYTE_IDX(XcpR_DestConf[Iterator].IntPduBufId)] &=
              (uint8)~((uint8)(1U << XCPR_FLAG_IDX(XcpR_DestConf[Iterator].IntPduBufId)));

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          }
        }
      }
    }
  }

  if (ReportDetNoTxExternalBus == TRUE)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_NO_TX_EXTERNAL_BUS);
  }
  if (ReportDetNoTxInternalBus == TRUE)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_NO_TX_INTERNAL_BUS);
  }
}

/*------------------[XcpR_ProcessAutonomousDisconnectResponse]---------------------------*/

FUNC(void, XCPR_CODE) XcpR_ProcessAutonomousDisconnectResponse(void)
{
  P2CONST(XcpR_DestPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_DestPduIdPtr =
    &XcpR_DestConf[XcpR_Status.DestinationFromMasterInternalId];

  /* received response from Xcp Slave */
  TS_AtomicAssignGeneric(XcpR_Status.ConnectionStatus, XCPR_STATE_DISCONNECTED);
  /* send Tx confirmation if response was sent from local Xcp */
  if (Conf_DestPduIdPtr->PduType == XCPR_RX_PDU_DIRECTION)
  {
    XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].ULTxConfirmationPtr(XcpR_DestConf[XcpR_Status.DestinationFromSlaveInternalId].TxConfTriggerTransmitPduId);
  }
}

STATIC FUNC(void, XCPR_CODE) XcpR_PrepareTxData
(
  PduIdType DestinationId,
  P2VAR(PduIdType, AUTOMATIC, XCPR_APPL_DATA) activeDestPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) TxPduInfo
)
{
  /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  *activeDestPduId = XcpR_DestConf[DestinationId].TLDestPduId;

  /* Prepare TxPduInfo pointer */
  TxPduInfo->SduDataPtr = &(XcpR_DestPduData.PduBuffer[XcpR_DestConf[DestinationId].PduStartPos]);
  TxPduInfo->SduLength  = XcpR_DestPduData.PduLength[DestinationId];

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, XCPR_CODE) XcpR_ResetStatusForConnectCommand
(
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) XcpRDestBufferPtr,
  PduLengthType Conf_DestPduHeaderLength
)
{
  if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
  {
    /* check if we need to reset the internal status set before the LL Transmit */
    if ((XcpRDestBufferPtr[Conf_DestPduHeaderLength] == 0xFF) &&
        (XcpR_Status.isWaitingForConnectConfirmation == TRUE))
    {
      /* !LINKSTO XcpR.dsn.InternalState.TransmissionFailed.WaitingConnectConfirmation,1 */
      XcpR_Status.isPreviousCommandConnect = TRUE;
      XcpR_Status.isWaitingForConnectConfirmation = FALSE;
    }
    else
    {
      /* !LINKSTO XcpR.dsn.InternalState.TransmissionFailed.PreviousCommandConnect,1 */
      XcpR_Status.isPreviousCommandConnect = FALSE;
    }
  }
}

/*------------------[XcpR_TriggerDetErrorsProcessDest]---------------*/

FUNC(void, XCPR_CODE) XcpR_TriggerDetErrorsProcessDest
(
  void
)
{
  uint8 DetIterator;

  for (DetIterator = 0U; DetIterator < XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST]; DetIterator++)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_PDU_LOST);
  }
  for (DetIterator = 0U; DetIterator < XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_UNEXPECTED_MSG]; DetIterator++)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_UNEXPECTED_MSG);
  }
  for (DetIterator = 0U; DetIterator < XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_ACTIVE_DESTINATION]; DetIterator++)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INVALID_ACTIVE_DESTINATION);
  }
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  for (DetIterator = 0U; DetIterator < XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_CONNECT_MODE]; DetIterator++)
  {
    XCPR_DET_REPORT_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INVALID_CONNECT_MODE);
  }
  for (DetIterator = 0U; DetIterator < XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_LENGTH]; DetIterator++)
  {
    XCPR_DET_REPORT_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INVALID_LENGTH);
  }
#endif

  /* Reset Det internal variables */
  XcpR_InitDetErrorsProcessDest();
}

/*------------------[XcpR_InitDetErrorsProcessDest]---------------*/

FUNC(void, XCPR_CODE) XcpR_InitDetErrorsProcessDest
(
  void
)
{
  uint8 DetIterator;

  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  for (DetIterator = 0U; DetIterator < XCPR_NR_DET_ERROR_TYPES_PROCESSDEST; DetIterator++)
  {
    XcpR_ReportDetProcessDestCtr[DetIterator] = 0U;
  }

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
}

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

