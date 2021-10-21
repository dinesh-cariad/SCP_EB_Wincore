/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TLS_H
#define TLS_H

/*===============================[includes]======================================================*/
#include <Compiler.h>
#include <Std_Types.h>
#include <TcpIp.h>
#include <TcpIp_Api.h>

/*==================[Error codes]===========================================*/
#define TLS_E_NO_TLS_SOCKET                 100U
#define TLS_E_BUSY                            2U
#define TLS_E_UNINIT                          3U
#define TLS_E_CSM_KEY_INVALID                 4U
#define TLS_E_NOT_CONNECTED                   5U
#define TLS_E_INIT                            6U
#define TLS_E_INV_ARG                         7U

/*------------------[Service/instance IDs]----------------------------------*/
#define TLS_INTERNAL_SVCID                 0xFFU
#define TLS_INIT_SVCID                     0x01U
#define TLS_ENABLE_SVCID                   0x02U
#define TLS_CLOSE_SVCID                    0x03U
#define TLS_RXINDICATION_SVCID             0x04U
#define TLS_TCPCONNECTED_SVCID             0x05U
#define TLS_TCPTRANSMIT_SVCID              0x06U
#define TLS_UDPTRANSMIT_SVCID              0x07U
#define TLS_MAINFUNCTION_SVCID             0x08U
#define TLS_TXCONFIRMATION_SVCID           0x09U
#define TLS_TCPACCEPTED_SVCID              0x0AU
#define TLS_GETCONNECTIONSTATE_SVCID       0x0BU

/*==================[declaration of types]=======================================================*/
/** \brief Connection ID type for TLS connections */
typedef uint8 Tls_ConnectionIdType;

/** \brief Pointer to a TLS uint8 array */
typedef P2VAR(uint8, AUTOMATIC, TLS_VAR) Tls_PtrUint8;
/** \brief Pointer to a const TLS uint8 array */
typedef P2CONST(uint8, AUTOMATIC, TLS_VAR) Tls_PtrConstUint8;
/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

/*==================[external function definitions]==============================================*/

/* !LINKSTO Tls.API.Tls_Init,1 */
/** \brief Initializes the Tls module
 *
 * This must be called before any other Tls function to initialize the module.
 *
 * \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, TLS_CODE) Tls_Init(void);

/* !LINKSTO Tls.API.Tls_Enable,1 */
/** \brief Assigns a TLS connection context to a socket
 *
 * When enabling TLS on a socket, handshaking will start according to the configuration.
 * On server sockets the handshake will begin when the client sends a ClientHello message.
 * On TCP client sockets the handshake is started when Tls_TcpConnected() is called.
 * On UDP client sockets the handshake is started when Tls_UdpTransmit() is called.
 *
 * \param[in]  ConnectionId     The Tls connection identifier from the configuration.
 * \param[in]  SocketId         Socket identifier of the related local socket resource.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. due to a lack of buffer
 *                   space or the socket is not connected.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_Enable(Tls_ConnectionIdType ConnectionId, TcpIp_SocketIdType SocketId);

/* !LINKSTO Tls.API.Tls_TcpTransmit,1 */
/** \brief Requests to transmit data to remote destination via TLS protocol
 *
 * This service requests transmission of application data via TLS/TCP to a remote node. The transmission
 * of the data is decoupled.
 *
 * \param[in]  SocketId         Socket identifier of the related local socket resource.
 * \param[in]  DataPtr          Pointer to a linear buffer of AvailableLength bytes
 *                              containing the application data to be transmitted. In case DataPtr
 *                              is a NULL_PTR, TcpIp shall retrieve data from SoAd via
 *                              callback SoAd_CopyTxData().
 * \param[in]  AvailableLength  Available data for transmission in bytes.
 * \param[in]  ForceRetrieve    This parameter is only valid if DataPtr is a NULL_PTR.
 *                              Indicates how the TCP/IP stack retrieves data from SoAd
 *                              if DataPtr is a NULL_PTR.
 *                              TRUE: the whole data indicated by availableLength shall
 *                              be retrieved from the upper layer via one or multiple
 *                              SoAd_CopyTxData() calls within the context of this
 *                              transmit function.
 *                              FALSE: The TCP/IP stack may retrieve up to availableLength
 *                              data from the upper layer. It is allowed to retrieve
 *                              less than availableLength bytes.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. due to a lack of buffer
 *                   space or the socket is not connected.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_TcpTransmit(TcpIp_SocketIdType SocketId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr,
    uint32 AvailableLength, boolean ForceRetrieve);

/* !LINKSTO Tls.API.Tls_UdpTransmit,1 */
/** \brief Requests to transmit data to a remote destination via DTLS
 *
 * This service transmits DTLS application data to a remote node.
 * There are some differences to TcpIp_UdpTransmit:
 * - The first call will fail and the data will be lost.
 *   This is because the DTLS connection will be established after the first call.
 * - The connection will be established to the RemoteAddrPtr given in the first call.
 * - All calls must pass the same RemoteAddrPtr
 *
 * The parameter TotalLength indicates the payload size of the DTLS record which
 * shall be sent by the TCP/IP stack within the context of this transmit
 * function call.
 * If DataPtr is a NULL_PTR the payload must be retrieved via SoAd_CopyTxData().
 * If DataPtr is a valid pointer, it points to a linear buffer of TotalLength bytes
 * containing the payload.
 *
 * \param[in] socketId identifying the local socket resource.
 * \param[in] dataPtr In case DataPtr is a NULL_PTR, TcpIp shall retrieve data
 * from SoAd via callback SoAd_CopyTxData(). In case DataPtr is a valid pointer
 * TcpIp must not retrieve data via SoAd_CopyTxData(). Then DataPtr points to a
 * linear buffer of TotalLength bytes. TcpIp shall send the whole data indicated
 * by DataPtr and TotalLength within the context of this transmit function.
 * \param[in] remoteAddrPtr IP address and port of the remote host to transmit to.
 * \param[in] totalLength Total length of the data to be transmitted.
 *
 * \return Result of operation
 * \retval E_OK      DTLS message transmission was successful.
 * \retval E_NOT_OK  DTLS message transmission failed.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_UdpTransmit(TcpIp_SocketIdType SocketId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
      TcpIp_ConstSockAddrPtrType RemoteAddrPtr, uint16 TotalLength);

/** \brief MainFunction of the Tls module
 **
 ** This has to be called periodically in intervalls defined by the Tls configuration.
 **
 ** \ServiceID{0x0B}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, TLS_CODE) Tls_MainFunction(void);

/** \brief Get the state of the TLS connection
 **
 **
 ** \return Result of operation
 ** \retval E_OK                  The connection is established
 ** \retval TLS_E_NO_TLS_SOCKET   There is no TLS context assigned to the socket
 ** \retval TLS_E_NOT_CONNECTED   The connection is not established. There may be a handshake currently running or the connection was closed.
 ** \retval E_NOT_OK              The connection failed with an error.
 **
 ** \ServiceID{0x0B}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_GetConnectionState(TcpIp_SocketIdType SocketId);

/* !LINKSTO Tls.API.Tls_Close,1 */
/** \brief Closes a TLS connection
 **
 ** Sends a TLS close_notify message and notifies the upper layer upon completion.
 **
 ** \return Result of operation
 ** \retval E_OK      The request has been accepted
 ** \retval E_NOT_OK  The request has not been accepted (e.g. the TLS
 ** connection was not established before).
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Reentrant for different SocketId}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_Close(TcpIp_SocketIdType SocketId, boolean Abort);

/* Callback functions for TcpIp */

/* !LINKSTO Tls.API.Tls_RxIndication,1 */
/** \brief Data Reception of UDP datagram or TCP stream
 **
 ** Indicates the reception of a new TCP segment or UDP datagram and provides all data via BufPtr.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] RemoteAddrPtr IP address and port of the remote host which sent the data
 ** \param[in] BufPtr Pointer to the received data
 ** \param[in] Length Data length of the received TCP segment or UDP datagram
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(void, TLS_CODE) Tls_RxIndication(TcpIp_SocketIdType SocketId, TcpIp_ConstSockAddrPtrType RemoteAddrPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length);

/* !LINKSTO Tls.API.Tls_TxConfirmation,1 */
/** \brief Acknowledges transmitted data
 **
 ** The TCP/IP stack calls this function after the data has been acknowledged by the peer
 ** for TCP or was sent to the lower layer driver using UDP.
 ** Tls only confirms application data.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] Length Number of transmitted data bytes
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Reentrant for different SocketId}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, TLS_CODE) Tls_TxConfirmation(TcpIp_SocketIdType SocketId, uint16 Length);

/* !LINKSTO Tls.API.Tls_TcpConnected,1 */
/** \brief Indicates a successful connection from client sight
 **
 ** This is the equivalent to SoAd_TcpConnected().
 ** When the client socket is connected, Tls starts the handshake.
 ** On completion, it calls SoAd_TcpConnected() or another upper layer.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] RemoteAddrPtr IP address and port of the remote host.
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant for different SocketId}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_TcpConnected(TcpIp_SocketIdType SocketId, TcpIp_ConstSockAddrPtrType RemoteAddrPtr);

/* !LINKSTO Tls.API.Tls_TcpAccepted,1 */
/** \brief Indicates an incoming TCP connection on a server socket
 **
 ** This is the equivalent to SoAd_TcpAccepted().
 ** It just saves the SocketId of the listen socket to properly call SoAd_TcpAccepted().
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] SocketIdConnected Socket handle for the established connection
 ** \param[in] RemoteAddrPtr IP address and port of the remote host.
 **
 ** \return Result of operation
 ** \retval E_OK      Tls accepts the established connection
 ** \retval E_NOT_OK  Tls refuses the established connection
 **
 ** \ServiceID{0x0A}
 ** \Reentrancy{Reentrant for different SocketId}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TLS_CODE) Tls_TcpAccepted(TcpIp_SocketIdType SocketId, TcpIp_SocketIdType SocketIdConnected, TcpIp_ConstSockAddrPtrType RemoteAddrPtr);


#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#endif
