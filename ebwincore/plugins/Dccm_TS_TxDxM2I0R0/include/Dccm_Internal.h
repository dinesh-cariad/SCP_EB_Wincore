#ifndef DCCM_INTERNAL_H
#define DCCM_INTERNAL_H
/**
 * \file Dccm_Internal.h
 * \brief Diagnostic Client Communication Manager Internal definitions
 *
 * Copyright 2019 by Elektrobit GmbH
 * All rights exclusively reserved for Elektrobit GmbH,
 * unless expressly agreed to otherwise
 **/

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=============================================*/

#include <Dccm.h>
#include <Dccm_Cfg.h>

#if (DCCM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                          /* Header file of Development Error Tracer */
#endif /* if (DCCM_DEV_ERROR_DETECT == STD_ON) */

#include <SchM_Dccm.h>             /* for the locking of the exclusive areas */
/*==================[macros]=================================================*/
#if (defined DCCM_MODULE_ID)
  #error DCCM_MODULE_ID is already defined
#endif
#define DCCM_MODULE_ID                            0xFFU

#if (defined DCCM_INSTANCE_ID)
  #error DCCM_INSTANCE_ID is already defined
#endif
#define DCCM_INSTANCE_ID                          0x00U

/*======= SIDs for internal handler functions =======*/
#if (defined DCCM_SID_RX)
  #error DCCM_SID_RX is already defined
#endif
#define DCCM_SID_RX                               0x01U

#if (defined DCCM_SID_TX)
  #error DCCM_SID_TX is already defined
#endif
#define DCCM_SID_TX                               0x02U

#if (defined DCCM_SID_SERVICE)
  #error DCCM_SID_SERVICE is already defined
#endif
#define DCCM_SID_SERVICE                          0x03U

/*======= [Dccm Error Codes ] =======*/
#if (defined DCCM_E_STATE)
  #error DCCM_E_STATE is already defined
#endif
#define DCCM_E_STATE                              0x00U

#if (defined DCCM_E_INVALID_RESPONSE_PENDING_FORMAT)
  #error DCCM_E_INVALID_RESPONSE_PENDING_FORMAT is already defined
#endif
#define DCCM_E_INVALID_RESPONSE_PENDING_FORMAT    0x02U

#if (defined DCCM_E_NULL_POINTER)
  #error DCCM_E_NULL_POINTER is already defined
#endif
#define DCCM_E_NULL_POINTER                       0x03U

#if (defined DCCM_E_NULL_POINTER_2)
  #error DCCM_E_NULL_POINTER_2 is already defined
#endif
#define DCCM_E_NULL_POINTER_2                     0x04U

#if (defined DCCM_E_NULL_POINTER_3)
  #error DCCM_E_NULL_POINTER_3 is already defined
#endif
#define DCCM_E_NULL_POINTER_3                     0x05U

#if (defined DCCM_E_INVALID_PARAMETER)
  #error DCCM_E_INVALID_PARAMETER is already defined
#endif
#define DCCM_E_INVALID_PARAMETER                  0x06U

#if (defined DCCM_E_INVALID_PARAMETER_2)
  #error DCCM_E_INVALID_PARAMETER_2 is already defined
#endif
#define DCCM_E_INVALID_PARAMETER_2                0x07U

#if (defined DCCM_E_INVALID_PARAMETER_3)
  #error DCCM_E_INVALID_PARAMETER_3 is already defined
#endif
#define DCCM_E_INVALID_PARAMETER_3                0x08U

#if (defined DCCM_E_INVALID_PARAMETER_4)
  #error DCCM_E_INVALID_PARAMETER_4 is already defined
#endif
#define DCCM_E_INVALID_PARAMETER_4                0x09U

#if (defined DCCM_E_BUFFER_OVERFLOW)
  #error DCCM_E_BUFFER_OVERFLOW is already defined
#endif
#define DCCM_E_BUFFER_OVERFLOW                    0x0AU

#if (defined DCCM_E_FAILED)
  #error DCCM_E_FAILED is already defined
#endif
#define DCCM_E_FAILED                             0x0BU

#if (defined DCCM_E_FAILED_2)
  #error DCCM_E_FAILED_2 is already defined
#endif
#define DCCM_E_FAILED_2                           0x0CU

#if (defined DCCM_E_PDUID)
  #error DCCM_E_PDUID is already defined
#endif
#define DCCM_E_PDUID                              0x0DU

#if (defined DCCM_E_BUFFER_LENGTH)
  #error DCCM_E_BUFFER_LENGTH is already defined
#endif
#define DCCM_E_BUFFER_LENGTH                      0x0EU

#if (defined DCCM_E_PROTOCOL_ID)
  #error DCCM_E_PROTOCOL_ID is already defined
#endif
#define DCCM_E_PROTOCOL_ID                        0x0FU

#if (defined DCCM_E_FUNCTIONAL_ADDRESSING)
  #error DCCM_E_FUNCTIONAL_ADDRESSING is already defined
#endif
#define DCCM_E_FUNCTIONAL_ADDRESSING              0x10U

#if (defined DCCM_DET_REPORT_ERROR) /* to prevent double declaration */
#error DCCM_DET_REPORT_ERROR already defined
#endif /* if (defined DCCM_DET_REPORT_ERROR) */

#if (DCCM_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module
 **/
#define DCCM_DET_REPORT_ERROR(SID, ERROR_CODE)\
  do{\
    (void)Det_ReportError(\
      (uint16) (DCCM_MODULE_ID),\
      (uint8)  (DCCM_INSTANCE_ID),\
      (uint8)  (SID),\
      (uint8)  (ERROR_CODE)\
    );\
  }while(0)
#else
#define DCCM_DET_REPORT_ERROR(SID, ERROR_CODE)
#endif /* if (DCCM_DEV_ERROR_DETECT == STD_ON) */

/**
 *
 * \brief Macro indicates a switched of timer.
 *
 */
#if (defined DCCM_TIMEOUT_OFF)
  #error DCCM_TIMEOUT_OFF is already defined
#endif
#define DCCM_TIMEOUT_OFF                           0xFFFFFFFFUL

/**
 *
 * \brief suppress positive response message indication bit
 *
 */
#if (defined DCCM_SUPP_MSG_BIT)
  #error DCCM_SUPP_MSG_BIT is already defined
#endif
#define DCCM_SUPP_MSG_BIT                   0x80U

/* supported services */

/**
 *
 * \brief Id if no service was requested.
 *
 */
#if (defined DCCM_NO_SERVICE)
  #error DCCM_NO_SERVICE is already defined
#endif
#define DCCM_NO_SERVICE                     0x00U

/**
 *
 * \brief Service Id for write data by identifier.
 *
 */
#if (defined DCCM_SID_WRITE_DATA_BY_IDENTIFIER)
  #error DCCM_SID_WRITE_DATA_BY_IDENTIFIER is already defined
#endif
#define DCCM_SID_WRITE_DATA_BY_IDENTIFIER   0x2EU

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_ROUTINE_CONTROL_REQUEST)
  #error DCCM_SID_ROUTINE_CONTROL_REQUEST is already defined
#endif
#define DCCM_SID_ROUTINE_CONTROL_REQUEST    0x31U

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_TESTER_PRESENT)
  #error DCCM_SID_TESTER_PRESENT is already defined
#endif
#define DCCM_SID_TESTER_PRESENT    0x3EU

/**
 *
 * \brief Service Id for routine control request.
 *
 */
 #if (defined DCCM_SID_DIAGNOSTIC_SESSION_CONTROL)
  #error DCCM_SID_DIAGNOSTIC_SESSION_CONTROL is already defined
#endif
#define DCCM_SID_DIAGNOSTIC_SESSION_CONTROL 0x10U

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_ECU_RESET)
  #error DCCM_SID_ECU_RESET is already defined
#endif
#define DCCM_SID_ECU_RESET 0x11

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_SECURITY_ACCESS)
  #error DCCM_SID_SECURITY_ACCESS is already defined
#endif
#define DCCM_SID_SECURITY_ACCESS 0x27

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_COMMUNICATION_CONTROL)
  #error DCCM_SID_COMMUNICATION_CONTROL is already defined
#endif
#define DCCM_SID_COMMUNICATION_CONTROL 0x28

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_ACCESS_TIMING_PARAMETER)
  #error DCCM_SID_ACCESS_TIMING_PARAMETER is already defined
#endif
#define DCCM_SID_ACCESS_TIMING_PARAMETER 0x83

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_CONTROL_DTC_SETTING)
  #error DCCM_SID_CONTROL_DTC_SETTING is already defined
#endif
#define DCCM_SID_CONTROL_DTC_SETTING 0x85

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_RESPONSE_ON_EVENT)
  #error DCCM_SID_RESPONSE_ON_EVENT is already defined
#endif
#define DCCM_SID_RESPONSE_ON_EVENT 0x86

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_LINK_CONTROL)
  #error DCCM_SID_LINK_CONTROL is already defined
#endif
#define DCCM_SID_LINK_CONTROL 0x87

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER)
  #error DCCM_SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER is already defined
#endif
#define DCCM_SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER 0x2C

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined DCCM_SID_READ_DTC_INFORMATION)
  #error DCCM_SID_READ_DTC_INFORMATION is already defined
#endif
#define DCCM_SID_READ_DTC_INFORMATION 0x19

/**
 *
 * \brief Indicates a negative response.
 *
 */
#if (defined DCCM_NEGATIVE_RESPONSE_SID)
  #error DCCM_NEGATIVE_RESPONSE_SID is already defined
#endif
#define DCCM_NEGATIVE_RESPONSE_SID          0x7FU

/**
 *
 * \brief The length of a negative response.
 *
 */
#if (defined DCCM_NEGATIVE_RESPONSE_LENGTH)
  #error DCCM_NEGATIVE_RESPONSE_LENGTH is already defined
#endif
#define DCCM_NEGATIVE_RESPONSE_LENGTH          3U

/**
 * \brief The function Dccm_ProcessRetryInformation() instructs the caller to
 *        ask more data from the client application.
 *        This macro is needed only for streaming use-case.
 */
#if (defined DCCM_ASK_MORE_DATA)
  #error DCCM_ASK_MORE_DATA is already defined
#endif
#define DCCM_ASK_MORE_DATA           0x03U


/*==================[type definitions]=======================================*/

/**
 * \struct Dccm_TesterPresentType
 * \brief Contains data for the TesterPresent message.
 *
 * Struct to save data related with TesterPresent message.
 *
 * \var Dccm_TesterPresentType::isTesterPresentEnabled
 * \brief Is TesterPresent message enabled for the current Dccm protocol?
 *         TRUE for enabled; FALSE for disabled.
 *
 * \var Dccm_TesterPresentType::isMarkedForDisable
 * \brief Is TesterPresent message marked for disable for the current Dccm protocol?
 *         TRUE for marked; FALSE for not marked.
 *
 * \var Dccm_TesterPresentType::DataLengthPtr
 * \brief A Pointer to inform the caller of the service about the
 *        length of a received buffer.
 *
 * \var Dccm_TesterPresentType::Buffer
 * \brief A pointer to the send or receive buffer.
 *
 * \var Dccm_TesterPresentType::Length
 * \brief Length of buffer.
 *
 * \var Dccm_TesterPresentType::mustSendTesterPresentMessage
 * \brief Must or must not send the TesterPresent message.
 */
typedef struct
{
  boolean isTesterPresentEnabled;
  boolean isMarkedForDisable;
  P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr;
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer;
  uint16 Length;
  boolean mustSendTesterPresentMessage;
} Dccm_TesterPresentType;


/**
 * \struct Dccm_RequestInfoType
 * \brief Contains data on the information in the request.
 *
 * Struct to save data related with the request message.
 *
 * \var Dccm_RequestInfoType::ServiceID
 * \brief This variable holds the id of the service for which the request is made.
 *
 * \var Dccm_RequestInfoType::isServiceIDsaved
 * \brief The significance of values:
 * TRUE - SID from the request is saved in ServiceID parameter
 * FALSE - SID from the request is not saved in ServiceID parameter
 *
 */
typedef struct
{
  uint8 ServiceID;
  boolean isServiceIDsaved;
} Dccm_RequestInfoType;

/**
 *
 * \struct Dccm_PduInfoRxTxType
 * \brief Contains data for sending or receiving
 *
 * Struct to save data which should be send or was received.
 *
 *//**
 * \var Dccm_PduInfoRxTxType::PduInfo
 * \brief This variable contains SDU data pointer and SDU length of a
 *        transmit or receive buffer.
 *//**
 * \var Dccm_PduInfoRxTxType::Buffer
 * \brief A pointer to the send or receive buffer.
 *//**
 * \var Dccm_PduInfoRxTxType::Length
 * \brief Length of buffer.
 *//**
 * \var Dccm_PduInfoRxTxType::TpTotalLength
 * \brief Length of transmitted/received data, required internally to fill requested data correctly to transmit/receive buffer.
 *
 * Detail for sending: initially the value will be 0, and after each transmission (Dccm_CopyTxData()),
 * it will be incremented with the number of bytes that were transferred
 * during the current invocation of Dccm_CopyTxData(),
 * in order to reflect the number of bytes that were sent up to that moment.
 * For data retransmission (in case of errors), it is decremented with the number of bytes that
 * are requested for retransmission.
 *
 * Detail for receiving: right from the beginning (Dccm_StartOfReception()),
 * it will contain the total length of the data that will be received;
 * it will not be updated during receiving of data.
 * *//**
 * \var Dccm_PduInfoRxTxType::TxConfirmedLength
 * \brief Length of confirmed transmitted data, required internally to fill requested data correctly to transmit buffer
 */


typedef struct
{
  PduInfoType PduInfo;
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer;
  uint16 Length;
  PduLengthType TpTotalLength;
  PduLengthType TxConfirmedLength;
} Dccm_PduInfoRxTxType;

/**
 *
 * \struct Dccm_TimeoutsType
 * \brief Contains all the timers available on a protocol.
 *
 * Struct to save data related with the timeouts.
 *
 *//**
 * \var Dccm_TimeoutsType::Timeout
 * \brief Communication timeout counter
 * Timer variable used for P2 or P6 timeouts, depending on the configuration.
 * This timeout is triggered (started, reloaded, and stopped), using the
 * configuration values:
 * - P2ClientConfigurationValue and P2StarClientConfigurationValue when P2Client is configured.
 * - P6ClientConfigurationValue and P6StarClientConfigurationValue when P6Client is configured.
 * If the configuration values are not set using the Dccm_SetCommunicationTimeoutParameters
 * API then it will be loaded with the default values: DCCM_TIMEOUT_P2CLIENT and DCCM_TIMEOUT_P2STARCLIENT
 * or DCCM_TIMEOUT_P6CLIENT and DCCM_TIMEOUT_P6STARCLIENT.
 * If this timer expires a timeout response will be send to SW-Manager.
 *//**
 * \var Dccm_TimeoutsType::InternalTimeout
 * \brief Communication timeout counter
 * Timer variable used for InternalTimeout implementation
 * which is triggered (started, reloaded, and stopped) using the
 * configuration value: InternalTimeoutConfigurationValue.
 * If the configuration value is not set using the Dccm_SetCommunicationTimeoutParameters
 * API then it will be loaded with the default values: DCCM_TIMEOUT_INTERNAL.
 * If this timer expires a timeout response will be send to SW-Manager.
 *//**
 * \var Dccm_TimeoutsType::InternalTimeoutConfigurationValue
 * \brief The Internal Timeout configuration value
 * The configurable value to be used for the internal timeout.
 * This value is going to be used instead of DCCM_TIMEOUT_INTERNAL to initialize the InternalTimeout
 * parameter when the internal timeout should start.
 *//**
 * \var Dccm_TimeoutsType::P2ClientConfigurationValue
 * \brief The configuration value for P2Client
 * The configurable value to be used for the Timeout.
 * This value is going to be used instead of DCCM_TIMEOUT_P2CLIENT to initialize the Timeout
 * parameter when the P2 timeout should start.
 *//**
 * \var Dccm_TimeoutsType::P2StarClientConfigurationValue
 * \brief The configuration value for P2*Client.
 * The configurable value to be used for the Timeout.
 * This value is going to be used instead of DCCM_TIMEOUT_P2STARCLIENT to initialize the Timeout
 * parameter when the P2 timeout should start.
 *//**
 * \var Dccm_TimeoutsType::P6ClientConfigurationValue
 * \brief The configuration value for P6Client
 * The configurable value to be used for the Timeout.
 * This value is going to be used instead of DCCM_TIMEOUT_P6CLIENT to initialize the Timeout
 * parameter when the P6 timeout should start.
 *//**
 * \var Dccm_TimeoutsType::P6StarClientConfigurationValue
 * \brief The configuration value for P6*Client.
 * The configurable value to be used for the Timeout.
 * This value is going to be used instead of DCCM_TIMEOUT_P6STARCLIENT to initialize the Timeout
 * parameter when the P6 timeout should start.
 */

typedef struct
{
  Dccm_TimeoutType Timeout;
  Dccm_TimeoutType InternalTimeout;

  Dccm_TimeoutType InternalTimeoutConfigurationValue;
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
  Dccm_TimeoutType P2ClientConfigurationValue;
  Dccm_TimeoutType P2StarClientConfigurationValue;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
  Dccm_TimeoutType P6ClientConfigurationValue;
  Dccm_TimeoutType P6StarClientConfigurationValue;
#endif
} Dccm_TimeoutsType;

/**
 * \struct Dccm_DiagnosticProtocolType
 * \brief Contains all important data related to one diagnostic protocol.
 *
 * This structure contains all data which would be needed to handle a
 * diagnostic protocol.
 *
 *//**
 * \var Dccm_DiagnosticProtocolType::TxPduId
 * \brief This variable contains the unique identifier of the TX connection.
 *
 *//**
 * \var Dccm_DiagnosticProtocolType::RxPduId
 * \brief This variable contains the unique identifier of the RX connection.
 *
 *//**
 * \var Dccm_DiagnosticProtocolType::Status
 * \brief The status of the protocol.
 *//**
 * \var Dccm_DiagnosticProtocolType::DataLengthPtr
 * \brief A Pointer to inform the caller of the service about the
 *        length of a received buffer.
 *//**
 * \var Dccm_DiagnosticProtocolType::Callback
 * \brief To inform the caller of a service about the result.
 *//**
 * \var Dccm_PduInfoRxTxType::Dccm_PduInfo
 * \brief Contains data for sending or receiving.
 * Struct to save data which should be send or was received.
 *//**
 * \var Dccm_DiagnosticProtocolType::ResponseCode
 * \brief This variable holds the notification response code
 * This variable holds the response code to notify the SW-Manager about the
 * status of the requested service.
 *//**
 * \var Dccm_DiagnosticProtocolType::BufferStreamingCallback
 * \brief This member is used only when buffer streaming is enabled.
 * This callback is used to request the next data package in the use-case with data streaming.
 *//**
 * \var Dccm_DiagnosticProtocolType::StartLocation
 * \brief This member is used only when buffer streaming is enabled.
 * The current invocation of Dccm_CopyTxData() will transmit data starting from this index.
 * This member will be updated (increased) after each invocation of Dccm_CopyTxData().
 *//**
 * \var Dccm_DiagnosticProtocolType::AvailableData
 * \brief This member is used only when buffer streaming is enabled.
 * The length the data that is sent during the current invocation of Dccm_CopyTxData().
 * Can be the length of the buffer, or less (only at the last invocation of Dccm_CopyTxData()).
 *//**
 * \var Dccm_DiagnosticProtocolType::RetryInformation
 * \brief This member is used only when buffer streaming is enabled.
 * The variable contains the retry information that was received
 * at the last call of the function Dccm_CopyTxData().
 * Can have the values from the enum TpDataStateType: TP_DATARETRY, TP_DATACONF or TP_CONFPENDING.
 * If the parameter RetryInfoPtr of the function Dccm_CopyTxData() is NULL_PTR, this member will have
 * the value Dccm_RETRY_INFO_NULL.
 *//**
 * \var Dccm_DiagnosticProtocolType::TesterPresent
 * \brief TesterPresent related data.
 *//**
 * \var Dccm_DiagnosticProtocolType::AddressingType
 * \brief The protocol was initialized for physical or functional addressing.
 *//**
* \var Dccm_DiagnosticProtocolType::TesterPresentInitiatedByDccm
 * \brief Variable used to mark an request.
 * TRUE: the request was initiated inside Dccm; impact in
 * bufferStreaming: do not forward the request to the client.
 * FALSE: the request is a regular payload initiated by the client of Dccm.
 */
typedef struct
{
  PduIdType TxPduId;
  PduIdType RxPduId;
  Dccm_DiagnosticProtocolStatusType Status;
  P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr;
  Dccm_CallbackType Callback;
  Dccm_PduInfoRxTxType Dccm_PduInfo;
  Dccm_DiagProtocolResponseCodeType ResponseCode;
  Dccm_TimeoutsType Timeouts;
#if (DCCM_BUFFER_STREAMING == STD_ON)
  Dccm_BufferStreamingCallbackType BufferStreamingCallback;
  uint16 StartLocation;
  PduLengthType AvailableData;
  uint8 RetryInformation;
#endif
  Dccm_TesterPresentType TesterPresent;
  uint8 AddressingType;
  Dccm_RequestInfoType RequestInfo;
  boolean TesterPresentInitiatedByDccm;
} Dccm_DiagnosticProtocolType;

/** \brief Structure to store the info related to configured ECUs
 */
typedef struct
{
  /** \brief Number of EcuId.
   */
  uint8 EcuId;
  /** \brief Number of associated TxPdu
   */
  uint16 TxPduId;
  /** \brief Number of associated RxPdu
   */
  uint16 RxPduId;
}
Dccm_PduIdsConfigurationType;


/*==================[external function declarations]========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/**
 *
 * \brief Returns the index of the Diagnostic Protocol that use the RxPduId that is provided as parameter
 *
 * \param  RxPduId Pdu Id of the server.
 *
 * \param  ProtocolId the index of the searched protocol
 *
 * \retval E_OK ProtocolId was found
 *
 * \retval E_NOT_OK ProtocolId was not found. The parameter ProtocolId will have the value of DCCM_INVALID_PROTOCOL_ID.
 *
 */
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolByRxPduId
(
  PduIdType RxPduId,
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
);

/**
 *
 * \brief Returns the index of the Diagnostic Protocol that use the TxPduId that is provided as parameter
 *
 * \param  TxPduId Pdu Id of the server.
 *
 * \param  ProtocolId the index of the searched protocol
 *
 * \retval E_OK ProtocolId was found
 *
 * \retval E_NOT_OK ProtocolId was not found. The parameter ProtocolId will have the value of DCCM_INVALID_PROTOCOL_ID.
 *
 */
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolByTxPduId
(
  PduIdType TxPduId,
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]=====================================*/
#define DCCM_START_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

extern CONST(PduIdType, DCCM_CONST) MaxServers;
extern CONST(Dccm_DiagnosticProtocolType, DCCM_CONST) Dccm_DiagnosticProtocolDefault;
extern CONST(Dccm_TesterPresentType, DCCM_CONST) Dccm_TesterPresentDefault;

#if (DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING > 0U)
extern CONST(Dccm_PduIdsConfigurationType, DCCM_CONST) Dccm_PhysicalPduIdsArray[DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING];
#else
extern CONST(Dccm_PduIdsConfigurationType, DCCM_CONST) Dccm_PhysicalPduIdsArray[1U];
#endif

#if (DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING > 0U)
extern CONST(Dccm_PduIdsConfigurationType, DCCM_CONST) Dccm_FunctionalPduIdsArray[DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING];
#else
extern CONST(Dccm_PduIdsConfigurationType, DCCM_CONST) Dccm_FunctionalPduIdsArray[1U];
#endif


#define DCCM_STOP_SEC_CONST_UNSPECIFIED
#include <MemMap.h>
/*==================[external data]==========================================*/

#define DCCM_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

extern VAR(Dccm_DiagnosticProtocolType, DCCM_VAR) Dccm_DiagnosticProtocols[DCCM_MAX_DIAGNOSTIC_PROTOCOLS];
extern VAR(Dccm_TimeoutType, DCCM_VAR) TesterPresentInterval;
extern VAR(Dccm_TimeoutType, DCCM_VAR) TesterPresentIntervalCounter;
extern VAR(uint8, DCCM_VAR) NumberOfProtocolIdsForTesterPresentMessages;
extern VAR(Dccm_ProtocolIdType, DCCM_VAR) ProtocolIdsForTesterPresentMessages[];
extern VAR(uint8, DCCM_VAR) NrOfProtocolsAlreadyUsedForPhysicalCommunication;
extern VAR(boolean, DCCM_VAR) DccmWasInitialized;

#define DCCM_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

#endif /* DCCM_INTERNAL_H */
/*==================[end of file]============================================*/
