/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOAD_CBK_H
#define SOAD_CBK_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>
#include <TcpIp.h>             /* TcpIp types */
#include <SoAd_Types.h>        /* Module public types */
#include <SoAd_Version.h>      /* Module Version Info */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

/** \brief Data Reception of UDP datagram or TCP stream
 **
 ** Indicates the reception of a new TCP segment or UDP datagram and provides all data via BufPtr.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] RemoteAddrPtr IP address and port of the remote host which sent the data
 ** \param[in] BufPtr Pointer to the received data
 ** \param[in] Length Data length of the received TCP segment or UDP datagram
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_RxIndication
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
);

/** \brief Copy transmission data
 **
 ** This service requests to copy data for transmission to the buffer indicated.
 ** This call is triggered by TcpIp_Transmit().
 ** Note: The call to SoAd_CopyTxData() may happen in the context of TcpIp_Transmit().
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource
 ** \param[in] BufPtr Pointer to buffer for transmission data
 ** \param[in] BufLength length of provided data buffer
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 BufLength
);

/** \brief Acknowledges transmitted data
 **
 ** The TCP/IP stack calls this function after the data has been acknowledged by the peer
 ** for TCP or was sent to the lower layer driver using UDP.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] Length Number of transmitted data bytes
 **
 ** \ServiceID{0x14}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_TxConfirmation
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
);

#if(SOAD_TLS_ENABLED == STD_ON)

/** \brief Indicates an incoming TCP connection on a server socket that has
 **  SoAdTlsConnectionRef configured.
 **
 ** \param[in] SocketIdConnected Socket handle for the established connection
 **
 ** \return Result of operation
 ** \retval E_OK      SoAd accepts the established connection
 ** \retval E_NOT_OK  SoAd refuses the established connection, TcpIp stack
 ** shall close the connection.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpTlsAccepted
(
  TcpIp_SocketIdType SocketIdConnected
);

#endif /* SOAD_TLS_ENABLED == STD_ON */

/** \brief Indicates an incoming TCP connection on a server socket
 **
 ** SoAd_TcpAccepted() gets called if the stack put a socket into the listen mode
 ** before (as server) and a peer connected to it (as client). In detail: The TCP/IP
 ** stack calls this function after a socket was set into the listen state with
 ** TcpIp_TcpListen() and a TCP connection is requested by the peer.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] SocketIdConnected Socket handle for the established connection
 ** \param[in] RemoteAddrPtr IP address and port of the remote host.
 **
 ** \return Result of operation
 ** \retval E_OK      SoAd accepts the established connection
 ** \retval E_NOT_OK  SoAd refuses the established connection, TcpIp stack
 ** shall close the connection.
 **
 ** \ServiceID{0x15}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpAccepted
(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);

/** \brief Indicates a successful connection from client sight
 **
 ** SoAd_TcpConnected() gets called if the stack initiated a TCP connection before
 ** (as client) and the peer (the server) acknowledged the connection set up.
 ** In detail: The TCP/IP stack calls this function after a socket was requested to
 ** connect with TcpIp_TcpConnect() and a TCP connection is confirmed by the peer.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 **
 ** \ServiceID{0x16}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_TcpConnected
(
  TcpIp_SocketIdType SocketId
);

/** \brief Indicates an event in the TCP/IP stack
 **
 ** The sercive gets called if the stack encounters a condition described by the
 ** values in TcpIpEvent.
 **
 ** \param[in] SocketId Socket handle identifying the local socket resource.
 ** \param[in] Event This parameter contains a description of the event just encountered.
 **
 ** \ServiceID{0x17}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_TcpIpEvent
(
  TcpIp_SocketIdType SocketId,
  TcpIp_EventType Event
);

/** \brief Indicates an IP address change
 **
 ** This service gets called by the TCP/IP stack if an IP address changes
 ** (i.e. new address assigned or assigned address becomes invalid).
 **
 ** \param[in] IpAddrId IP address Identifier, representing an IP address specified in the TcpIp
 **                     module configuraiton (e.g. static IPv4 address on EthIf controller 0).
 ** \param[in] State    state of IP address assignment (TCPIP_IPADDR_STATE_ASSIGNED,
 **                     TCPIP_IPADDR_STATE_ONHOLD or TCPIP_IPADDR_STATE_UNASSIGNED)
 **
 ** \ServiceID{0x18}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_LocalIpAddrAssignmentChg
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
);

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOAD_CBK_H */
/*==================[end of file]===========================================*/
