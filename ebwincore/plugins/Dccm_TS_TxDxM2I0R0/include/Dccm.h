#ifndef DCCM_H
#define DCCM_H

/***************************************************************************
 *
 * \file Dccm.h
 *
 * \brief Diagnostic Client Communication Manager implementation of DCMM
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=============================================*/
#include <Dccm_Cbk.h>                      /* PduR interface for Dccm       */
#include <Dccm_Types.h>                    /* public Dccm defined types     */
#include <Dccm_Cfg.h>                      /* Dccm configuration            */

/*==================[macros]=================================================*/

/**
 *
 * \brief Dccm is busy at the moment: all available diagnostic protocols
 *        are in use.
 *
 * Dccm-specific Std_ReturnType value used by Dccm APIs
 *
 */

#if (defined DCCM_STD_E_BUSY)
  #error DCCM_STD_E_BUSY is already defined
#endif
#define DCCM_STD_E_BUSY 0x02U

/**
 *
 * \brief Requested service executed without error.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_OK)
  #error DCCM_RSP_OK is already defined
#endif
#define DCCM_RSP_OK             0x00U

/**
 *
 * \brief Transmitting of data failed.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TX_FAILED)
  #error DCCM_RSP_TX_FAILED is already defined
#endif
#define DCCM_RSP_TX_FAILED      0x01U

/**
 *
 * \brief Triggering of data transmit failed.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TX_TRIG_FAILED)
  #error DCCM_RSP_TX_TRIG_FAILED is already defined
#endif
#define DCCM_RSP_TX_TRIG_FAILED 0x02U

/**
 *
 * \brief Data receiving failed.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_RX_FAILED)
  #error DCCM_RSP_RX_FAILED is already defined
#endif
#define DCCM_RSP_RX_FAILED      0x03U

/**
 *
 * \brief Response to indicate that a ResponsePending message was received for another service, not the one for which the request was made.
 *
 * UdsC::UdsC_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_INVALID_RESPONSE_PENDING_FORMAT)
  #error DCCM_RSP_INVALID_RESPONSE_PENDING_FORMAT is already defined
#endif
#define DCCM_RSP_INVALID_RESPONSE_PENDING_FORMAT 0x04U

/**
 *
 * \brief No response from server during the P2Client or P2*Client timeout.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TIMEOUT_P2CLIENT)
  #error DCCM_RSP_TIMEOUT_P2CLIENT is already defined
#endif
#define DCCM_RSP_TIMEOUT_P2CLIENT        0x05U

/**
 *
 * \brief No response from server during the internal timer.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TIMEOUT_INTERNAL)
  #error DCCM_RSP_TIMEOUT_INTERNAL is already defined
#endif
#define DCCM_RSP_TIMEOUT_INTERNAL        0x06U

/**
 *
 * \brief Response code received from the (bottom module) PduR.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TIMEOUT)
  #error DCCM_RSP_TIMEOUT is already defined
#endif
#define DCCM_RSP_TIMEOUT        0x07U

/**
 *
 * \brief Receive buffer size is wrong.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_WRONG_BUFFER_SIZE)
  #error DCCM_RSP_WRONG_BUFFER_SIZE is already defined
#endif
#define DCCM_RSP_WRONG_BUFFER_SIZE 0x08U

/**
 *
 * \brief No response from server during the P6Client or P6*Client timeout.
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_RSP_TIMEOUT_P6CLIENT)
  #error DCCM_RSP_TIMEOUT_P6CLIENT is already defined
#endif
#define DCCM_RSP_TIMEOUT_P6CLIENT        0x09U

/**
 *
 * \brief Response when Tx confirmation was OK and suppress bit is set
 *
 * Dccm::Dccm_ResponseCode value provided to the upper module in the callback
 *
 */
#if (defined DCCM_TX_CONFIRMATION_OK)
  #error DCCM_TX_CONFIRMATION_OK is already defined
#endif
#define DCCM_TX_CONFIRMATION_OK    0x11U

/* Negative response codes */

/**
 *
 * \brief Error code returned from server.
 *
 */
#if (defined DCCM_E_RESPONSE_PENDING)
  #error DCCM_E_RESPONSE_PENDING is already defined
#endif
#define DCCM_E_RESPONSE_PENDING     0x78U


/* Dccm Diagnostic Protocold Status Values */
/**
 *
 * \brief The status of an unallocated protocol.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED                       0x00U
/**
 *
 * \brief The status of protocol that is currently going through the allocation process.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING                   0x01U
/**
 *
 * \brief The status of protocol that is ready to start communication.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY                        0x02U
/**
 *
 * \brief The status of protocol that is currently going through processing a request.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_RECEIVED)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_RECEIVED is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_RECEIVED                0x03U
/**
 *
 * \brief The status of a protocol that has finished processing a request and is ready to forward it.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY_TO_TRANSMIT)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY_TO_TRANSMIT is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY_TO_TRANSMIT            0x04U
/**
 *
 * \brief The transmission was triggered and the protocol is in the process of forwarding the message.
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT                     0x05U
/**
 *
 * \brief The status of a protocol that is in the process of receiving the response.
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE                      0x06U
/**
 *
 * \brief The status of a protocol that has just finished receiving the response and is in the process
 *  of transmitting the callback to the Dccm client application.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY                  0x07U
/**
 *
 * \brief The status of protocol that is currently going through the release process.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE                      0x08U
/**
 *
 * \brief The status of protocol when BufferStreaming is enabled and the transmission of the next data packet is requested.
 *
 */
#if (defined DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER)
  #error DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER is already defined
#endif
#define DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER 0x09U
/**
 *
 * \brief A Protocol ID that is considered as invalid value.
 *
 */
#if (defined DCCM_INVALID_PROTOCOL_ID)
  #error DCCM_INVALID_PROTOCOL_ID is already defined
#endif
#define DCCM_INVALID_PROTOCOL_ID    0xFFU
/**
 *
 * \brief Macro used to mark that the RetryInfoPtr parameter of the Dccm_CopyTxData() function is null.
 *
 */
#if (defined DCCM_RETRY_INFO_NULL)
  #error DCCM_RETRY_INFO_NULL is already defined
#endif
#define DCCM_RETRY_INFO_NULL                                0x0FU

/**
 *
 * \brief Macro used to define AddressingType of a diagnostic protocol.
 *
 */
#if (defined PHYSICAL_ADDRESSING)
  #error PHYSICAL_ADDRESSING is already defined
#endif
#define PHYSICAL_ADDRESSING      0U
/**
 *
 * \brief Macro used to define AddressingType of a diagnostic protocol.
 *
 */
#if (defined FUNCTIONAL_ADDRESSING)
  #error FUNCTIONAL_ADDRESSING is already defined
#endif
#define FUNCTIONAL_ADDRESSING    1U

/*==================[external function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/** \brief Check the Buffer for Suppress Bit
 **
 ** This function checks if the suppress positive response message indication bit is set
 ** in the input buffer.
 **
 ** Configuration: No configuration is needed for this function
 **
 ** \return boolean
 ** \retval TRUE bit is set
 ** \retval FALSE bit is not set
 **
 ** \ServiceID{::Dccm_CheckBufferSuppressBit}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(boolean, DCCM_CODE) Dccm_CheckBufferSuppressBit
(
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer
);

/** \brief Check the Buffer for Response Pending message
 **
 ** This function is used to check if the Response Pending
 ** message has been set in the input buffer.
 **
 ** Configuration: No configuration is needed for this function
 **
 ** \return Std_ReturnType
 ** \retval TRUE if the response pending is set
 ** \retval FALSE the buffer is not big enough or if the response pending is not set
 **
 ** \ServiceID{::Dccm_CheckResponsePending}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(boolean, DCCM_CODE) Dccm_CheckResponsePending
(
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer
);

/** \brief An interface to allocate a diagnostic protocol.
 **
 ** This function is used to allocate a diagnostic protocol.
 ** It shall be called before sending an Dccm Request.
 ** If BufferStreaming is not activated the parameter BufferStreamingCallback should be null pointer.
 **
 ** The application that is the client of Dccm can not communicate with a sever without first allocating a diagnostic protocol.
 ** The number of available diagnostic protocols is limited by:
 ** - the total number of protocols that is configured for the Dccm module,
 ** - the number of protocols previously allocated by the client application, and
 ** - the number of protocols reserved for functional communication (from the configuration of Dccm).
 **
 ** From the total number of protocols, the client of the Dccm module
 ** can use any number of protocols for functional communication,
 ** but the number of protocols available for physical communication
 ** is just the difference between the total number of protocols
 ** and the number of protocols reserved for functional communication.
 **
 ** A specific TxPduId can be used only once, for a single Dccm communication protocol.
 ** A specific RxPduId can be used only once, for a single Dccm communication protocol.
 **
 ** Configuration: The maximum number of parallel diagnostic protocols
 ** can be configured but is limited to maximum 255.
 **
 ** \param[in] TxPduId The PduId that will be used for sending data.
 **
 ** \param[in] RxPduId The PduId that will be used for receiving data.
 **
 ** \param[out] ProtocolId The ID of the protocol will be returned to the caller of the function.
 **
 ** \param[in] AddressingType The protocol is allocated for physical or functional communication.
 **                    Expected values: PHYSICAL_ADDRESSING or FUNCTIONAL_ADDRESSING.
 **
 ** \param[in] Callback The callback function that will be used by the Dccm
 **           to inform the caller of a service about the result.
 **
 ** \param[in] BufferStreamingCallback The callback function that will be used by the Dccm to ask the next data
 **                  packet from the client application in the use-case with buffer streaming.
 **                  If BufferStreaming is not activated this parameter should be null pointer.
 **
 ** \return Std_ReturnType
 ** \retval E_OK The protocol was allocated
 **
 ** \retval E_NOT_OK There was an error related with the parameters provided to the function.
 **                     The ProtocolId OUT parameter contains a value that is not valid (DCCM_INVALID_PROTOCOL_ID).
 **                     The maximum number of diagnostic protocols used for physical communication has been reached.
 **
 ** \retval DCCM_STD_E_BUSY There are no available protocols. After a protocol will be
 **                    released by the client application, it can be allocated again.
 **
 ** \ServiceID{::Dccm_AllocateDiagnosticProtocol}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
FUNC(Std_ReturnType, DCCM_CODE) Dccm_AllocateDiagnosticProtocol
(
  uint16 TxPduId,
  uint16 RxPduId,
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId,
  uint8 AddressingType,
  Dccm_CallbackType Callback,
  Dccm_BufferStreamingCallbackType BufferStreamingCallback
);

/** \brief An interface to release a Dccm Diagnostic Protocol
 **
 ** This function is used to release a diagnostic protocol
 ** It shall be called when there is no need for communication
 ** over the specific PduId.
 ** If TesterPresent is enabled for the current protocol,
 ** Dccm will take care to disable the TesterPresent sending.
 **
 ** Configuration: The maximum number of parallel diagnostic protocols
 ** can be configured but is limited to maximum 255 (0x00 - 0xFE: 0U - 254U).
 **
 ** \param[in] ProtocolId Release the protocol with this ID.
 **
 ** \return Std_ReturnType
 ** \retval E_OK The protocol was released
 **
 ** \retval E_NOT_OK The ProtocolID is outside of the pre-configured range of values,
 **                    or the protocol identified with this ProtocolID
 **                    is in one of the following states: UNUSED, RELEASE, ALLOCATING.
 **
 ** \ServiceID{::Dccm_ReleaseDiagnosticProtocol}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_ReleaseDiagnosticProtocol
(
  Dccm_ProtocolIdType ProtocolId
);

/** \brief An interface to release all the Dccm Diagnostic Protocols
 **
 ** This function is used to release all allocated diagnostic protocols
 ** It shall be called when there is no need for communication.
 **
 ** Configuration: The maximum number of parallel diagnostic protocols
 ** can be configured but is limited to maximum 255 (0x00 - 0xFE: 0U - 254U).
 **
 ** \return Std_ReturnType
 ** \retval E_OK All the diagnostic protocols were properly released.
 **              Non-releasable statuses: RELEASE or ALLOCATING.
 **
 ** \retval E_NOT_OK No protocol has been released because
 **                  at least one protocol is still in one of the following situations:
 **         - the state is RELEASE or ALLOCATING;
 **         - or the TesterPresent feature is enabled and the sending of the
 **           TesterPresent message is on-going.
 **
 ** \ServiceID{::Dccm_ReleaseAllDiagnosticProtocols}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_ReleaseAllDiagnosticProtocols
(
    void
);

/** \brief Sends an UDS payload over a Diagnostic Protocol
 **
 ** This function is used to initiate the sending of the
 ** UDS payload over a Diagnostic Protocol. The Diagnostic Protocol
 ** should be in the READY state otherwise the call will return with error.
 **
 ** Configuration: The maximum number of parallel diagnostic protocols
 ** can be configured but is limited to maximum 255. The PduIds should be
 ** configured and the function will check if the provided PduId is not over
 ** the limit.
 **
 ** \param[in] ProtocolId The Protocol ID that will be used
 **         in the communication with the server.
 **
 ** \param[in,out] Buffer A pointer to the start of the buffer where the data
 **        received from the server will be stored.
 **
 ** \param[in]  BufferLength Size of the buffer.
 **
 ** \param[in,out]  DataLengthPtr
 **           IN: The number of bytes that will be sent to the server.
 **           OUT: A pointer to return the number of bytes received from the server.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Service accepted
 ** \retval E_NOT_OK ProtocolId not correct, buffer is too short, Buffer is null or DataLengthPtr is null.
 ** \retval DCCM_STD_E_BUSY A request is active.
 **
 ** \ServiceID{::Dccm_SendRequest}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Asynchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_SendRequest
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer,
  uint16 BufferLength,
  P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr
);

/** \brief Returns the status of a Diagnostic Protocol.
 **
 ** This function is used to query the status of a specific
 ** Diagnostic Protocol.
 **
 ** \param[in] ProtocolId The status of this protocol ID is queried.
 **
 ** \param[out] Status This is the status of the protocol that is returned to the user.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Status was retrieved and returned to the user
 ** \retval E_NOT_OK ProtocolID is out of the pre-configured range of values,
 **                  or the Status parameter is null.
 **
 ** \ServiceID{::Dccm_GetDiagnosticProtocolStatus}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolStatus
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(Dccm_DiagnosticProtocolStatusType, AUTOMATIC, DCCM_APPL_DATA) Status
);


/** \brief Set the time interval the periodic sending of the TesterPresent message.
 **
 ** This function is used to set the time interval the periodic sending of the TesterPresent message.
 **
 ** \param[in] Interval The time interval between initiating two consecutive TesterPresent messages.
 **            The value should be long enough to permit the proper sending of the messages:
 **            - Dccm_MainFunction() must be called a number of times
 **            to assure that the protocols switch through a number of states;
 **            - PduR must have enough time to call the call-back functions (provided by Dccm)
 **            involved in the sending of the messages.
 **
 ** \ServiceID{::Dccm_SetTesterPresentInterval}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCCM_CODE) Dccm_SetTesterPresentInterval
(
  uint16 Interval
);

/**  \brief Enable the periodic sending of the TesterPresent message.
 **
 ** This function is used to enable the periodic sending of the TesterPresent message on a specific Dccm protocol.
 ** Dccm_SetTesterPresentInterval() must be called before the call to this function.
 ** In Dccm, the TesterPresent functionality can be used only for functional communication.
 ** Because of this limitation, the Dccm protocol must be allocated specifically for functional communication.
 **
 ** \param[in] ProtocolId The ID of the Dccm protocol.
 ** \param[in] BufferLength - should be at least 3 bytes length, to have enough room for a negative response from the server
 ** \param[out] Buffer - the content of buffer is ignored
 ** \param[out] DataLengthPtr - will contain the answer from the server (if it is received)
 **
 ** \return Std_ReturnType
 ** \retval E_OK Status was retrieved and returned to the user
 ** \retval E_NOT_OK ProtocolId not correct, buffer is too short, Buffer is null, DataLengthPtr is null,
 **                   no functional address is set in the configuration of Dccm,
 **                   BufferLength is smaller than 3,
 **                   the protocol identified with ProtocolId was not allocated for functional communication,
 **                   or the function Dccm_SetTesterPresentInterval() was not yet called.
 **
 ** \ServiceID{::Dccm_EnableTesterPresent}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_EnableTesterPresent
(
    Dccm_ProtocolIdType ProtocolId,
    P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer,
    uint16 BufferLength,
    P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr
);

/** \brief Disable the periodic sending of tester present
 **
 ** This function is used to disable the periodic sending of tester present
 ** for the ProtocolID provided.
 ** Use-case 1: if the sending of the TesterPresent message is not ongoing,
 ** TesterPresent will be disabled during the call of this function.
 ** Use-case 2: if the sending of the TesterPresent message was already triggered by
 ** Dccm, Dccm will disable the periodic sending of TesterPresent message
 ** after the invocation of Dccm_TxConfirmation(), in the next
 ** call of Dccm_MainFunction(). Until then, Dccm_IsTesterPresentEnabled() will return false.
 **
 ** \param[in] ProtocolId The ID of the Dccm protocol.
 **
 ** \return Std_ReturnType
 ** \retval E_OK TesterPresent functional message was disabled for the Dccm protocol ProtocolId.
 ** \retval E_NOT_OK There was an error:
 **            - the sending of the message TesterPresent is not enabled
 **              for the ProtocolId provided as input parameter, or
 **            - the ProtocolId is not valid, or
 **            - the module was not properly initialized.
 **
 ** \ServiceID{::Dccm_DisableTesterPresent}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_DisableTesterPresent
(
    Dccm_ProtocolIdType ProtocolId
);

/** \brief Check if the tester present is enabled or not
 **
 ** This function will return the status of tester present for the selected Dccm protocol.
 **
 ** \param[in] ProtocolId The ID of the Dccm protocol.
  ** \param[out] IsTesterPresentEnabled will be TRUE if TesterPresent notification
 **                 is enabled for the protocol with the ID ProtocolId.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Status was retrieved and returned to the user
 ** \retval E_NOT_OK the ProtocolId is not good,
 **            or IsTesterPresentEnabled is NULL.
 **
 ** \ServiceID{::Dccm_IsTesterPresentEnabled}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_IsTesterPresentEnabled
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(boolean, AUTOMATIC, DCCM_APPL_DATA) IsTesterPresentEnabled
);

#if (DCCM_P2CLIENT_ENABLED == STD_ON)
/** \brief Set the timeout parameters for a specific diagnostic protocol.
 **
 ** This function is used to set the timeout parameters for a specific diagnostic protocol.
 ** Depending on the configuration, the input parameters may be P2ClientConfigurationValue and P2StarClientConfigurationValue
 ** or P6ClientConfigurationValue and P6StarClientConfigurationValue.
 **
 ** \param[in] ProtocolId The ID of the Dccm protocol.
 ** \param[in] P2ClientConfigurationValue The value to be used for the start of P2Client timeout.
 ** \param[in] InternalTimeout The value to be used for the start of internal timeout.
 ** \param[in] P2StarClientConfigurationValue The value to be used for the start of P2StarClient timeout.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Diagnostic Protocol communication parameters were successfully updated
 ** \retval E_NOT_OK the ProtocolId is not valid or not in the DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY
 **
 ** \ServiceID{::Dccm_SetCommunicationTimeoutParameters}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_SetCommunicationTimeoutParameters
(
  Dccm_ProtocolIdType ProtocolId,
  Dccm_TimeoutType P2ClientConfigurationValue,
  Dccm_TimeoutType InternalTimeout,
  Dccm_TimeoutType P2StarClientConfigurationValue
);

#elif (DCCM_P6CLIENT_ENABLED == STD_ON)

/** \brief Set the timeout parameters for a specific diagnostic protocol.
 **
 ** This function is used to set the timeout parameters for a specific diagnostic protocol.
 ** Depending on the configuration, the input parameters may be P2ClientConfigurationValue and P2StarClientConfigurationValue
 ** or P6ClientConfigurationValue and P6StarClientConfigurationValue.
 **
 ** \param[in] ProtocolId The ID of the Dccm protocol.
 ** \param[in] P6ClientConfigurationValue The value to be used for the start of P6Client timeout.
 ** \param[in] InternalTimeout The value to be used for the start of internal timeout.
 ** \param[in] P6StarClientConfigurationValue The value to be used for the start of P6StarClient timeout.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Diagnostic Protocol communication parameters were successfully updated
 ** \retval E_NOT_OK the ProtocolId is not valid or not in the DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY
 **
 ** \ServiceID{::Dccm_SetCommunicationTimeoutParameters}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_SetCommunicationTimeoutParameters
(
  Dccm_ProtocolIdType ProtocolId,
  Dccm_TimeoutType P6ClientConfigurationValue,
  Dccm_TimeoutType InternalTimeout,
  Dccm_TimeoutType P6StarClientConfigurationValue
);

#endif

/** \brief Initializes or reinitializes the Dccm module.
 **
 ** This function resets all relevant variables to the default values.
 **
 ** This function shall be used during the startup phase of the ECU after
 ** the NVRAM Manager has finished the restore of NVRAM data.
 **
 ** SW-Components including Monitor Functions are initialized afterwards.
 **
 ** Caveats: The Dccm is not functional until this function has been called.
 **
 ** \ServiceID{::Dccm_Init}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCCM_CODE) Dccm_Init
(
    void
);

/*------------------[Scheduled functions]-----------------------------------*/

/** \brief Processes the Dccm requests
 **
 ** This function is used to process the Dccm requests.
 ** It shall be called periodically as a cyclic task by the software
 ** system (e.g. by operating system).
 ** If a Main function of a un-initialized module is called from the BSW Scheduler,
 ** then it shall return immediately without performing any functionality and
 ** without raising any errors.
 **
 ** Timing: fixed cyclic
 **
 ** Configuration: The cyclic time for the main function has to be defined as
 ** an operating system task or runnable entity.
 **
 ** \ServiceID{::Dccm_MainFunction}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCCM_CODE) Dccm_MainFunction(void);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/
#endif /* DCCM_H */
/*==================[end of file]============================================*/
