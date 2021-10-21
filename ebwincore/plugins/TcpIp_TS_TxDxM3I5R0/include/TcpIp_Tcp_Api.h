/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TCPIP_TCP_API_H
#define TCPIP_TCP_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Trigger the TCP connection establishment to a remote host
 *
 * By this API service the TCP/IP stack is requested to establish a TCP connection
 * to the configured peer.
 *
 * \param[in]  SocketId       Socket identifier of the related local socket resource.
 * \param[in]  RemoteAddrPtr  IP address and port of the remote host to connect to.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. connection is already
 *                   established or no route to destination specified by remoteAddrPtr
 *                   found.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_TcpConnect
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief Start to listen for incoming TCP connection requests
 *
 * By this API service the TCP/IP stack is requested to listen on the TCP socket
 * specified by the socket identifier.
 *
 * \param[in]  SocketId     Socket identifier of the related local socket resource.
 * \param[in]  MaxChannels  Maximum number of new parallel connections established on
 *                          this listen connection.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. connection is already
 *                   established or no route to destination specified by remoteAddrPtr
 *                   found.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_TcpListen
(
  TcpIp_SocketIdType SocketId,
  uint16 MaxChannels
);

/** \brief Confirm the reception of data
 *
 * By this API service the reception of socket data is confirmed to the TCP/IP stack.
 *
 * \param[in]  SocketId  Socket identifier of the related local socket resource.
 * \param[in]  Length    Number of bytes finally consumed by the upper layer.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. invalid socket id.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_TcpReceived
(
  TcpIp_SocketIdType SocketId,
  uint32 Length
);

/** \brief Requests to transmit data to remote destination via TCP protocol
 *
 * This service requests transmission of data via TCP to a remote node. The transmission
 * of the data is decoupled.
 * Note: The TCP segment(s) are sent dependent on runtime factors (e.g. receive window)
 * and configuration parameter (e.g. Nagle algorithm).
 *
 * \param[in]  SocketId         Socket identifier of the related local socket resource.
 * \param[in]  DataPtr          Pointer to a linear buffer of AvailableLength bytes
 *                              containing the data to be transmitted. In case DataPtr
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
 *                              less than availableLength bytes. Note: Not retrieved
 *                              data will be provided by SoAd with the next call to
 *                              TcpIp_TcpTransmit (along with new data if available).
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted, e.g. due to a lack of buffer
 *                   space or the socket is not connected.
 *
 * \ServiceID{0x13}
 * \Reentrancy{Reentrant for different SocketIds. Non reentrant for the same SocketId.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_TcpTransmit
(
  TcpIp_SocketIdType SocketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_TCP_API_H */
