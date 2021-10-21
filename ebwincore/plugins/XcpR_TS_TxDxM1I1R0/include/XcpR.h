#ifndef XCPR_H
#define XCPR_H

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

#include <XcpR_Version.h>      /* XCPR modules' version info */
#include <XcpR_Cfg.h>

/*==[Macros definitions]======================================================*/

#if (defined XCPR_INSTANCE_ID)
#error XCPR_INSTANCE_ID already defined
#endif
/** \brief Id of instance of XcpR */
#define XCPR_INSTANCE_ID                       0U

/*------------------[error codes]--------------------------------------------*/

#if (defined XCPR_E_NOT_INITIALIZED)
#error XCPR_E_NOT_INITIALIZED already defined
#endif
/** \brief Error Code.
 **
 ** API is called before complete initialization. */
#define XCPR_E_NOT_INITIALIZED                   0x01U

#if (defined XCPR_E_NULL_POINTER)
#error XCPR_E_NULL_POINTER already defined
#endif
/** \brief Error Code.
 **
 ** Null pointer has been passed as an argument. */
#define XCPR_E_NULL_POINTER                      0x02U

#if (defined XCPR_E_INVALID_PDUID)
#error XCPR_E_INVALID_PDUID already defined
#endif
/** \brief Error Code.
 **
 ** API called with wrong PDU ID. */
#define XCPR_E_INVALID_PDUID                     0x10U

#if (defined XCPR_E_INVALID_LENGTH)
#error XCPR_E_INVALID_LENGTH already defined
#endif
/** \brief Error Code.
 **
 ** Message does not fit in the destination's available length. */
#define XCPR_E_INVALID_LENGTH                    0x11U

#if (defined XCPR_E_INVALID_RX_PDU_LENGTH)
#error XCPR_E_INVALID_RX_PDU_LENGTH already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by XcpR_RxIndication() if the length of the
 ** received Pdu is greater than maximum configured buffer size for the
 ** incoming PDUs.
 **/
#define XCPR_E_INVALID_RX_PDU_LENGTH             0x12U

#if (defined XCPR_E_INVALID_TX_PDU_LENGTH)
#error XCPR_E_INVALID_TX_PDU_LENGTH already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by XcpR_Transmit() if the length of the
 ** transmitted Pdu is greater than maximum configured buffer size for the
 ** outgoing PDUs.
 **/
#define XCPR_E_INVALID_TX_PDU_LENGTH             0x13U

#if (defined XCPR_E_INVALID_CONNECT_MODE)
#error XCPR_E_INVALID_CONNECT_MODE already defined
#endif
/** \brief Error Code.
 **
 ** The error is triggered by XcpR_MainFunction() if, for the currently processed
 ** source with CONNECT mode 0 there is no linked Rx destination
 ** or for a source with CONNECT mode 1 there is no linked Tx destination.
 ** transmission on the internal Bus cannot be done.
 **/
#define XCPR_E_INVALID_CONNECT_MODE              0x14U

#if (defined XCPR_E_PDU_LOST)
#error XCPR_E_PDU_LOST already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered if XcpR is disconnected
 ** and if the PID of the received Pdu is not a CONNECT PID
 ** or if the response after connect/disconnect is not positive.
 **/
#define XCPR_E_PDU_LOST                          0x20U

#if (defined XCPR_E_UNEXPECTED_MSG)
#error XCPR_E_UNEXPECTED_MSG already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_MainFunction() when processing a source
 ** or a destination, if the XcpR is disconnected, and XcpR receives a
 ** response from the local or remote Xcp slave but the previous command
 ** was not a CONNECT command.
 **/
#define XCPR_E_UNEXPECTED_MSG                    0x21U

#if (defined XCPR_E_INTERNAL_BUFFER_OVERFLOW)
#error XCPR_E_INTERNAL_BUFFER_OVERFLOW already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_MainFunction() or XcpR_RxIndication(), when processing an XcpR source,
 ** if there is an attempt to write more data in the circular XcpR internal queue, than its currently
 ** available size. The initial size is determined by the XcpRQueueSize configuration parameter.
 **/
#define XCPR_E_INTERNAL_BUFFER_OVERFLOW          0x22U

#if (defined XCPR_E_INTERNAL_BUS)
#error XCPR_E_INTERNAL_BUS already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_ProcessActiveDestination() if the
 ** transmission on the internal Bus cannot be done.
 **/
#define XCPR_E_INTERNAL_BUS                      0x23U

#if (defined XCPR_E_EXTERNAL_BUS)
#error XCPR_E_EXTERNAL_BUS already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_ProcessActiveDestination() if the
 ** transmission on the external Bus cannot be done.
 **/
#define XCPR_E_EXTERNAL_BUS                     0x24U

#if (defined XCPR_E_NO_TX_INTERNAL_BUS)
#error XCPR_E_NO_TX_INTERNAL_BUS already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_ProcessBusMonitor() if the
 ** Tx confirmation from remote Xcp slave is not received.
 **/
#define XCPR_E_NO_TX_INTERNAL_BUS                0x25U

#if (defined XCPR_E_NO_TX_EXTERNAL_BUS)
#error XCPR_E_NO_TX_EXTERNAL_BUS already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_ProcessBusMonitor() if the
 ** Tx confirmation from Xcp master is not received.
 **/
#define XCPR_E_NO_TX_EXTERNAL_BUS                0x26U

#if (defined XCPR_E_PDU_BUFFER_FULL)
#error XCPR_E_PDU_BUFFER_FULL already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_RxIndication()
 ** if the message is lost because the assigned receive Pdu
 ** buffer is full.
 **/
#define XCPR_E_PDU_BUFFER_FULL                   0x27U

#if (defined XCPR_E_INVALID_MESSAGE_LENGTH)
#error XCPR_E_INVALID_MESSAGE_LENGTH already defined
#endif
/** \brief Development Error Code.
 **
 ** The error is triggered by XcpR_MainFunction() or XcpR_RxIndication(), when processing an XcpR source,
 ** if the data length of the unpacked message is bigger than the maximum between MaxCto/MaxDto/MaxCtoPgm
 ** of all configured XcpR sources.
 **
 **/
#define XCPR_E_INVALID_MESSAGE_LENGTH            0x28U

#if (defined XCPR_E_PDU_OUTSIDE_CONNECTION_GROUP)
#error XCPR_E_PDU_OUTSIDE_CONNECTION_GROUP already defined
#endif
/** \brief Development Error Code.
 **
 ** The error is triggered by XcpR_RxIndication()
 ** if the PduId is not accepted because is not part of the
 ** currently active Connection Group, which is set after XcpR
 ** is in state CONNECTED, considering the configuration.
 **/
#define XCPR_E_PDU_OUTSIDE_CONNECTION_GROUP      0x29U

#if (defined XCPR_E_INVALID_ACTIVE_DESTINATION)
#error XCPR_E_INVALID_ACTIVE_DESTINATION already defined
#endif
/** \brief Runtime Error Code.
 **
 ** The error is triggered by XcpR_ProcessSource()
 ** if no configured destination has been successfully linked.
 **/
#define XCPR_E_INVALID_ACTIVE_DESTINATION        0x30U

/*------------------[macros for service IDs]---------------------------------*/

#if (defined XCPR_SID_INIT)
#error XCPR_SID_INIT already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_Init(). */
#define XCPR_SID_INIT                            0x00U

#if (defined XCPR_SID_GET_VERSION_INFO)
#error XCPR_SID_GET_VERSION_INFO already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_GetVersionInfo(). */
#define XCPR_SID_GET_VERSION_INFO                 0x01U

#if (defined XCPR_SID_IF_TX_CONFIRMATION)
#error XCPR_SID_IF_TX_CONFIRMATION already defined
#endif
/** \brief API service ID.
 **
 ** Definition of the general service ID for XcpR_TxConfirmation() function. */
#define XCPR_SID_IF_TX_CONFIRMATION              0x02U

#if (defined XCPR_SID_IF_RX_INDICATION)
#error XCPR_SID_IF_RX_INDICATION already defined
#endif
/** \brief API service ID.
 **
 ** Definition of the general service ID for XcpR_RxIndication() function. */
#define XCPR_SID_IF_RX_INDICATION                0x03U

#if (defined XCPR_SID_IF_TRANSMIT)
#error XCPR_SID_IF_TRANSMIT already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_Transmit(). */
#define XCPR_SID_IF_TRANSMIT                     0x04U

#if (defined XCPR_SID_IF_TRIGGER_TRANSMIT)
#error XCPR_SID_IF_TRIGGER_TRANSMIT already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_TriggerTransmit(). */
#define XCPR_SID_IF_TRIGGER_TRANSMIT             0x05U

#if (defined XCPR_SID_MAIN_FUNCTION)
#error XCPR_SID_MAIN_FUNCTION already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_MainFunction(). */
#define XCPR_SID_MAIN_FUNCTION                   0x06U

#if (defined XCPR_SID_SOAD_SO_CON_MODE_CHG)
#error XCPR_SID_SOAD_SO_CON_MODE_CHG already defined
#endif
/** \brief API service ID.
 **
 ** Definition of service ID for XcpR_SoAdSoConModeChg(). */
#define XCPR_SID_SOAD_SO_CON_MODE_CHG            0x07U

/*==[Types declarations]======================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

#if (XCPR_VERSION_INFO_API == STD_ON)
/* !LINKSTO XcpR.API.GetVersionInfo,1 */

/** \brief Return the modules version information
 **
 ** This function provides the information to module vendor ID, module ID and
 ** software version major.minor.patch
 **
 ** Precondition: ::XCPR_VERSION_INFO_API = ::STD_ON
 **
 ** \param[out]  VersionInfoPtr  Pointer to where to store the version
 **                              information of this module.
 **
 ** \ServiceID{::XCPR_SID_GET_VERSION_INFO}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCPR_APPL_DATA) VersionInfoPtr
);

#endif /* (XCPR_VERSION_INFO_API == STD_ON) */

/*---------------[Interface ECU State Manager <--> XcpR]---------------------*/

/** \brief Initializes the XCPR.
 **
 ** This function initializes interfaces and variables of the XCP Router
 ** module.
 **
 ** Precondition: None.
 **
 ** \ServiceID{::XCPR_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_Init
(
  void
);

/** \brief Scheduled function of the XCP Router module
 **
 ** Scheduled function of the XCP Router module.
 **
 ** Precondition: None.
 **
 ** \ServiceID{::XCPR_SID_MAIN_FUNCTION}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCPR_CODE) XcpR_MainFunction
(
  void
);

#if (XCPR_NO_OF_TX_SOURCE_PDUS > 0)
/** \brief XCPR PDU is received.
 **
 ** This function is called by the upper layer module when an
 ** XCPR PDU has been received.
 **
 ** \param[in]  TxPduId     PDU-ID that has been received.
 ** \param[in]  PduInfoPtr  Pointer to SDU(Buffer of received payload).
 **
 ** \return Transmit operation status
 ** \retval E_OK transmission was accepted by transport layer
 ** \retval E_NOT_OK transmission was rejected by transport layer
 **
 ** \ServiceID{::XCPR_SID_IF_TRANSMIT}
 ** \Reentrancy{Reentrant for different TxPduId and non-Reentrant for the same TxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_CONST) PduInfoPtr
);
#endif

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

#endif /* XCPR_H */

