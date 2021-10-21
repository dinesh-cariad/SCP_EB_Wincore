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
#ifndef XCP_H
#define XCP_H

/*==================[includes]===============================================*/
#include <Xcp_Cfg.h>          /* Generated module configuration */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <Xcp_Version.h>      /* This modules' version declarations */
#include <Xcp_Types.h>        /* Xcp types to be published */

#if ((XCP_STORE_DAQ_SUPPORTED == STD_ON) || (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON))
#include <NvM.h>            /* NvM API */
#endif

/*==================[macros]=================================================*/

#if (defined XCP_INSTANCE_ID)
#error XCP_INSTANCE_ID already defined
#endif
/** \brief Id of instance of Xcp */
#define XCP_INSTANCE_ID                       0U

#if (defined XCP_STATE_ACTIVE)
#error XCP_STATE_ACTIVE already defined
#endif
/** \brief Defines XCP active state where the code is generated as usual. */
#define XCP_STATE_ACTIVE          0xAAU

#if (defined XCP_STATE_INACTIVE)
#error XCP_STATE_INACTIVE already defined
#endif
/** \brief Defines XCP inactive state where the code is generated but the suppressed functions perform no actions. */
#define XCP_STATE_INACTIVE        0x55U

#if (XCP_CALIBRATION_MEMORY_ACCESS_AREAS == STD_ON)
#define XCP_USE_MEMORY_PROXY STD_ON
#else
#define XCP_USE_MEMORY_PROXY STD_OFF
#endif

/*------------------[error codes]--------------------------------------------*/

#if (defined XCP_E_INV_POINTER)
#error XCP_E_INV_POINTER already defined
#endif
/** \brief Error Code.
 **
 ** API is invoked with invalid pointer. */
#define XCP_E_INV_POINTER                       0x01U

#if (defined XCP_E_NOT_INITIALIZED)
#error XCP_E_NOT_INITIALIZED already defined
#endif
/** \brief Error Code.
 **
 ** API is called before complete initialization. */
#define XCP_E_NOT_INITIALIZED                   0x02U

#if (defined XCP_E_INVALID_PDUID)
#error XCP_E_INVALID_PDUID already defined
#endif
/** \brief Error Code.
 **
 ** API called with wrong PDU ID. */
#define XCP_E_INVALID_PDUID                     0x03U

#if (defined XCP_E_NULL_POINTER)
#error XCP_E_NULL_POINTER already defined
#endif
/** \brief Error Code.
 **
 ** Null pointer has been passed as an argument. */
#define XCP_E_NULL_POINTER                      0x12U

#if (defined XCP_E_INVALID_NVM_BLOCK_LENGTH)
#error XCP_E_INVALID_NVM_BLOCK_LENGTH already defined
#endif
/** \brief Error Code thrown when the NvM block needed to store the DAQ lists has an invalid length
 **
 ** The storage of DAQ lists in NV memory requires a NvM block to be configured correctly. This error
 ** describes the situation when the block required by XCP to store the DAQ list configuration has
 ** an invalid length. The length of the NvM block must match the one of the runtime structure of
 ** the DAQ lists. In order to determine the correct length, the easiest way is to check with the
 ** debugger the size of the runtime DAQ lists information (hint: breakpoint where this error is
 ** thrown).
 **
 ** */
#define XCP_E_INVALID_NVM_BLOCK_LENGTH          0x20U

#if (defined XCP_E_INVALID_EVENT)
#error XCP_E_INVALID_EVENT already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_SetEvent() if
 ** - The given event ID (parameter) is invalid, i.e. there is no event with that ID or
 ** - The given event ID belongs to a periodic event, for which this function shall not be called. */
#define XCP_E_INVALID_EVENT                     0x40U

#if (defined XCP_E_INVALID_RX_PDU_LENGTH)
#error XCP_E_INVALID_RX_PDU_LENGTH already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_<If>RxIndication() if the length of the received Pdu is greater
 ** than maximum configured buffer size for the incoming PDUs
 **/
#define XCP_E_INVALID_RX_PDU_LENGTH             0x80U

#if (defined XCP_E_PDU_BUFFER_FULL)
#error XCP_E_PDU_BUFFER_FULL already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_<If>RxIndication() if the L-PDU buffer corresponding
 ** to the received PDU is full. This may happen when the Xcp_MainFunction is called
 ** with higher latency then the lowest latency used by the master to send messages.
 **/
#define XCP_E_PDU_BUFFER_FULL                   0x81U

#if (defined XCP_E_PDU_BUFFER_LOCKED)
#error XCP_E_PDU_BUFFER_LOCKED already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_<If>(), in case the PDU doesn't belong
 ** the current connection.
 **/
#define XCP_E_PDU_BUFFER_LOCKED                 0x82U

#if (defined XCP_E_PDU_LOST)
#error XCP_E_PDU_LOST already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_MainFunction()/Xcp_MainFunction_[EventName]/
 ** Xcp_<If>RxIndication() if, when unpacking PDUs, at least one PDU message
 ** is silently lost, i.e. without notification towards the Xcp master.
 **/
#define XCP_E_PDU_LOST                          0x83U

#if (defined XCP_E_INVALID_STATE_COMMAND)
#error XCP_E_INVALID_STATE_COMMAND already defined
#endif
/** \brief Error Code thrown when the XCP Control function receives an invalid command.
 **This error describes the situation when the function is called with a value other then
 ** 0xAA or 0x55.
 ** */
#define XCP_E_INVALID_STATE_COMMAND             0x30U

#if (defined XCP_RESP_CTO_QUEUE_FULL)
#error XCP_RESP_CTO_QUEUE_FULL already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_SendErrorCmdSyntaxPacket() if the response CTO queue, used
 ** to enqueue the response packet, is full. Therefore the negative response XCP_ERR_CMD_SYNTAX
 ** packet is not enqueued and this matter should be signalled. */
#define XCP_RESP_CTO_QUEUE_FULL                    0x84U

#if (defined XCP_EVENT_CTO_QUEUE_FULL)
#error XCP_EVENT_CTO_QUEUE_FULL already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_PrepareStoredDaqListsResume() if the event CTO queue, used to
 ** enqueue the event CTO, is full. Therefore the incoming event CTO shall not be enqueued and this matter
 ** should be signaled. */
#define XCP_EVENT_CTO_QUEUE_FULL                  0x85U

#if (defined XCP_PROGRAMMING_SEGMENT_NOT_ACTIVE)
#error XCP_PROGRAMMING_SEGMENT_NOT_ACTIVE already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_SetAbortProgrammingSession() because the programming segment
    is not active anymore. Therefore the programming sequence shall be aborted and the user can retry
    again programming the whole sequence.*/
#define XCP_PROGRAMMING_SEGMENT_NOT_ACTIVE       0x86U

#if (defined XCP_ERR_STIMULATION_DATA_LOST)
#error XCP_ERR_STIMULATION_DATA_LOST already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_InsertOdtToStimBuffer() because previous stimulation data was used (it exists)
    and therefore a new entry cannot be added. Consequently the new data is lost and a DET notification shall
    be issued to signal this situation. */
#define XCP_ERR_STIMULATION_DATA_LOST       0x87U

#if (defined XCP_ERR_TX_BUFFER_NOT_AVAILABLE)
#error XCP_ERR_TX_BUFFER_NOT_AVAILABLE already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_UpdateAvailableTxBuffers() because the TX buffer is
    not available and thus there is the great possibility to loose Pdu data. */
#define XCP_ERR_TX_BUFFER_NOT_AVAILABLE      0x88U

#if (defined XCP_E_PARAM_CHANNEL_INVALID)
#error XCP_E_PARAM_CHANNEL_INVALID already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_SetTransmissionMode() when
 ** the channel parameter is invalid */
#define XCP_E_PARAM_CHANNEL_INVALID         0x89U


#if (defined XCP_E_SCHM_ERROR)
#error XCP_E_SCHM_ERROR already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by Xcp_MainFunction in the BSW distribution
 ** scenario when a memory access (read,write) requires the usage of a
 ** memory proxy which cannot be triggered due to a SchM problem */
#define XCP_E_SCHM_ERROR 0X90U

#if (defined XCP_E_MEMORY_PROXY_MF_SYNC_ERROR)
#error XCP_E_MEMORY_PROXY_MF_SYNC_ERROR already defined
#endif
/** \brief Error Code thrown when the XCP has to trigger the processing
 ** of an event if the event could not be processed in the previous MF.
 ** */
#define XCP_E_MEMORY_PROXY_MF_SYNC_ERROR        0x91U

#if (defined XCP_E_MEMORY_PROXY_CYCLIC_EVENT_SYNC_ERROR)
#error XCP_E_MEMORY_PROXY_CYCLIC_EVENT_SYNC_ERROR already defined
#endif
/** \brief Error Code thrown when the XCP sets the cyclic event but the processing
 ** of the event could not be processed in the previous MFs.
 ** */
#define XCP_E_MEMORY_PROXY_CYCLIC_EVENT_SYNC_ERROR        0x92U

#if (defined XCP_E_WRONG_CONTEXT)
#error XCP_E_WRONG_CONTEXT already defined
#endif
/** \brief Error Code
 **
 ** The API has been called from an invalid application context. */
#define XCP_E_WRONG_CONTEXT 0x92U

#if (defined XCP_E_DATA_LOST)
#error XCP_E_DATA_LOST already defined
#endif
/** \brief Error Code thrown when data which should be provided from
 ** other partition is lost.
 ** */
#define XCP_E_DATA_LOST        0x93U

/*------------------[macros for service IDs]---------------------------------*/

#if (defined XCP_SID_INIT)
#error XCP_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_Init(). */
#define XCP_SID_INIT                            0x00U

#if (defined XCP_SID_GET_VERSION_INFO)
#error XCP_SID_GET_VERSION_INFO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_GetVersionInfo(). */
#define XCP_SID_GET_VERSION_INFO                0x01U

#if (defined XCP_SID_STATE_CONTROL)
#error XCP_SID_STATE_CONTROL already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_Control(). */
#define XCP_SID_STATE_CONTROL                   0x83U

#if (defined XCP_SID_IF_TX_CONFIRMATION)
#error XCP_SID_IF_TX_CONFIRMATION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of the general service ID for Xcp_<If>TxConfirmation() functions. */
#define XCP_SID_IF_TX_CONFIRMATION              0x02U

#if (defined XCP_SID_IF_RX_INDICATION)
#error XCP_SID_IF_RX_INDICATION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of the general service ID for Xcp_<If>RxIndication() functions. */
#define XCP_SID_IF_RX_INDICATION                0x03U

#if (defined XCP_SID_MAIN_FUNCTION)
#error XCP_SID_MAIN_FUNCTION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_MainFunction(). */
#define XCP_SID_MAIN_FUNCTION                   0x04U

#if (defined XCP_SID_SET_TRANSMISSION_MODE)
#error XCP_SID_SET_TRANSMISSION_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_SetTransmissionMode(). */
#define XCP_SID_SET_TRANSMISSION_MODE           0x05U

#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))

#if (defined XCP_SID_IF_TRIGGER_TRANSMIT)
#error XCP_SID_IF_TRIGGER_TRANSMIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_<If>TriggerTransmit(). */
#define XCP_SID_IF_TRIGGER_TRANSMIT             0x41U

#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */


#if (defined XCP_SID_SET_EVENT)
#error XCP_SID_SET_EVENT already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_SetEvent() API. */
#define XCP_SID_SET_EVENT                      0x80U

#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)

#if (defined XCP_SID_SOAD_SO_CON_MODE_CHG)
#error XCP_SID_SOAD_SO_CON_MODE_CHG already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Xcp_SoAdSoConModeChg(). */
#define XCP_SID_SOAD_SO_CON_MODE_CHG           0x81U

#endif /* (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */

#if (defined XCP_SID_EVENT_MAIN_FUNCTION)
#error XCP_SID_EVENT_MAIN_FUNCTION already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_Event_MainFunction() API. */
#define XCP_SID_EVENT_MAIN_FUNCTION            255U

#if (defined XCP_SID_TX_MAIN_FUNCTION)
#error XCP_SID_TX_MAIN_FUNCTION already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_TxMainFunction() API. */
#define XCP_SID_TX_MAIN_FUNCTION               255U

#if (defined XCP_SID_RX_MAIN_FUNCTION)
#error XCP_SID_RX_MAIN_FUNCTION already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_RxMainFunction() API. */
#define XCP_SID_RX_MAIN_FUNCTION               255U

#if(XCP_ASYNCH_CRC_SUPPORT == STD_ON)

#if (defined XCP_SID_CRC_MAIN_FUNCTION)
#error XCP_SID_CRC_MAIN_FUNCTION already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_CrcMainFunction() API. */
#define XCP_SID_CRC_MAIN_FUNCTION            255U

#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */
#if(defined XCP_SID_SEND_CMD_SYNTAX_PACKET)
#error XCP_SID_SEND_CMD_SYNTAX_PACKET already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_SendErrorCmdSyntaxPacket(). */
#define XCP_SID_SEND_CMD_SYNTAX_PACKET     0x83U

#if(defined XCP_SID_PREPARE_STORED_DAQ_RESUME)
#error XCP_SID_PREPARE_STORED_DAQ_RESUME already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_PrepareStoredDaqListsResume() API. */
#if(defined XCP_SID_ENQUEUECTOEVENT)
#error XCP_SID_ENQUEUECTOEVENT already defined
#endif
#define XCP_SID_ENQUEUECTOEVENT            0x84U

#if (defined XCP_SID_APPL_COMMAND)
#error XCP_SID_APPL_COMMAND already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_Appl<CommandName>() API. */
#define XCP_SID_APPL_COMMAND               0x85U

#if (defined XCP_SID_SET_ABORT_PROGRAMMING_SESSION)
#error XCP_SID_SET_ABORT_PROGRAMMING_SESSION already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_SetAbortProgrammingSession() API. */
#define XCP_SID_SET_ABORT_PROGRAMMING_SESSION   0x86U

#if (defined XCP_SID_INSERT_ODT_TO_STIM_BUFFER)
#error XCP_SID_INSERT_ODT_TO_STIM_BUFFER already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_InsertOdtToStimBuffer() API. */
#define XCP_SID_INSERT_ODT_TO_STIM_BUFFER     0x87U

#if (defined XCP_SID_UPDATE_AVAILABLE_TX_BUFFERS)
#error XCP_SID_UPDATE_AVAILABLE_TX_BUFFERS already defined
#endif
/** \brief XCP API service ID.
 **
 ** Definition of service ID for Xcp_UpdateAvailableTxBuffers() API. */
#define XCP_SID_UPDATE_AVAILABLE_TX_BUFFERS     0x88U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if (XCP_VERSION_INFO_API == STD_ON)

/** \brief Return the modules version information
 **
 ** This function provides the information to module vendor ID, module ID and
 ** software version major.minor.patch
 **
 ** Precondition: ::XCP_VERSION_INFO_API = ::STD_ON
 **
 ** \param[out]  VersionInfoPtr  Pointer to where to store the version
 **                              information of this module.
 **
 ** \ServiceID{::XCP_SID_GET_VERSION_INFO}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) VersionInfoPtr
);

#endif /* (XCP_VERSION_INFO_API == STD_ON) */

/*---------------[Interface ECU State Manager <--> Xcp]---------------------*/

/** \brief Initializes the XCP.
 **
 ** This function initializes interfaces and variables of the AUTOSAR XCP
 ** module.
 **
 ** Precondition: None.
 **
 ** \param[in]  Xcp_ConfigPtr  Pointer to a selected configuration structure.
 **
 ** \ServiceID{::XCP_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_Init
(
  P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr
);

/*------------------[Scheduled functions]-----------------------------------*/

/** \brief Scheduled function of the XCP module
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None.
 **
 ** \ProductionError{XCP_E_RESP_CTO_QUEUE_FULL, thrown\, if a command response
 ** or event package cannot be sent to the master because the internal queue is full.}
 ** \ProductionError{XCP_E_PDU_LOST, thrown\, if an XCP message is received
 ** but it is discarded by XCP because it is detected as incorrect.}
 ** \ProductionError{XCP_E_PDU_BUFFER_FULL, thrown\, if an XCP message is received
 ** but it is discarded because the reception buffer is already full.}
 ** \ProductionError{XCP_E_STIMULATION_DATA_LOST, thrown\, if new stimulation data is
 ** received before previous data is processed.}
 ** \ProductionError{XCP_E_ACQUISITION_DATA_LOST, thrown\, if the requested acquisition data
 ** exceeds the connection MAX_DTO and cannot fit into a package.}
 ** \ProductionError{XCP_E_ILLEGAL_MEMORY_ACCESS, thrown\, if an illegal memory
 ** access is performed.}
 ** \ProductionError{XCP_E_RETRY_FAILED, thrown\, if an XCP message transmission
 ** (including the configured number of retries) fails.}
 **
 ** \ServiceID{::XCP_SID_MAIN_FUNCTION} */
extern FUNC(void, XCP_CODE) Xcp_MainFunction
(
  void
);

/** \brief Function to set a DAQ list sampling event.
 **
 ** This function sets an event for the given event channel to trigger the
 ** sampling of the associated DAQ list(s). This event will then be handled by
 ** one of the following Xcp_MainFunction() calls.
 **
 ** Precondition: The event channel must be a non-cyclic event channel, i.e.
 ** the parameter XcpEventChannelTimeCycle is set to 0 for that event channel.
 **
 ** \param[in]  EventId  XCP event channel ID.
 **
 ** \return Result of the operation.
 ** \retval XCP_OK     Setting the event was successful.
 ** \retval XCP_NOT_OK Setting the event failed (a development error
 **                    happened).
 ** \retval XCP_NOT_INITIALIZED Setting the event failed (module is not
 **                             initialized).
 ** \retval XCP_NOT_CONNECTED   Setting the event failed (slave is in the
 **                             state DISCONNECTED).
 ** \retval XCP_OVERLOAD        Setting the event failed (event is already
 **                             set).
 ** \retval XCP_NO_ACTIVE_LIST  Setting the event failed (no active list is
 **                             assigned to that event).
 **
 ** \ServiceID{::XCP_SID_SET_EVENT}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Xcp_ReturnType, XCP_CODE) Xcp_SetEvent
(
  uint16 EventId
);
#if (XCP_ENABLE_XCP_CONTROL_API == STD_ON)
/**
 ** \brief Function to perform an XCP disable or enable depending on on the
 **  command passed.
 **  On disable state the function will terminate the current XCP session and
 **  reset internal data structures and sub processors. The code will be
 **  generated no action performed form the suppressed functions.
 **  On enable state the Xcp module is reinitialized, the suppressed
 **  functionality in disable state is resumed.
 **  The function shall be called with the values 0xAA for active,
 **  and 0x55 for inactive state.
 **
 ** Precondition: The XcpEnableXcpControlApi parameter has to be set to true,
 **               thus, XCP_ENABLE_XCP_CONTROL_API is set to STD_ON allowing the Xcp_Control() function
 **               to be enabled and called when needed.
 **
 ** \param[in] command gives the command to enable or disable the Xcp module.
 **
 ** \ServiceID{::XCP_SID_STATE_CONTROL}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_Control( uint8 command );
#endif

#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/**
 ** \brief This API is used to turn on and off of the TX capabilities of used communication
 ** bus channel in XCP module.
 ** If Xcp_SetTransmissionMode(Channel, Mode) is called and parameter Mode
 ** equals XCP_TX_OFF, all TxPDUs which are assigned to Channel shall not be
 ** transmitted.
 **
 ** If Xcp_SetTransmissionMode(Channel, Mode) is called and parameter Mode
 ** equals XCP_TX_ON, all TxPDUs which are assigned to Channel shall be able to be
 ** transmitted.
 **
 ** \param[in] Channel The Network channel for the used bus communication
 ** \param[in] Mode Enabled or disabled Transmission mode Parameters
 **
 ** \ServiceID{::XCP_SID_SET_TRANSMISSION_MODE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE)  Xcp_SetTransmissionMode
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
);
#endif /* XCP_SUPPRESS_TX_SUPPORT == STD_ON */

#if ((XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON) || (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON))
/**
 ** \brief This API is used to report that a MemoryProxy instance has finished
 ** its task.
 **
 ** \param[in] EventId  The Id of the Event for which the MemoryProxy was invoked.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(void, XCP_CODE) Xcp_MemoryProxyResponseHandler(P2VAR(uint16, AUTOMATIC, XCP_APPL_DATA) EventId);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/
#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */
#endif /* ifndef XCP_H */
/*==================[end of file]============================================*/
