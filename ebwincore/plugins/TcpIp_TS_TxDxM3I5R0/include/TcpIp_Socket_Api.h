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
#ifndef TCPIP_SOCKET_API_H
#define TCPIP_SOCKET_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */



#include <TcpIp_Socket_Api.guards>
/*==================[macros]================================================*/

/** \brief Any port
 *
 * This macro defines a value to select any port (instead of a specific one)
 * used in TcpIp_SockAddrType.
 */
#define TCPIP_PORT_ANY                   0U

/** \brief Any local IP address
 *
 * This macro defines a value to select any local IP address (instead of
 * a specific one) used for API service TcpIp_Bind().
 */
#define TCPIP_LOCALADDRID_ANY            254U

/*==================[type definitions]======================================*/

/** \brief TCP connection reseted, all resources released */
#define TCPIP_TCP_RESET 0x00U
/** \brief TCP connection reached state 'CLOSE', all resources released */
#define TCPIP_TCP_CLOSED 0x01U
/** \brief TCP segment with set FIN flag received */
#define TCPIP_TCP_FIN_RECEIVED 0x02U
/** \brief UDP socket and all related resources have been released */
#define TCPIP_UDP_CLOSED 0x03U

/** \brief Socket state machine events signalized to upper layer */
typedef uint8 TcpIp_EventType;


/** \brief Specifies the maximum TCP receive window for the socket. */
#define TCPIP_PARAMID_TCP_RXWND_MAX 0x00U
/** \brief Specifies the frame priority for outgoing frames on the socket. */
#define TCPIP_PARAMID_FRAMEPRIO 0x01U
/** \brief Specifies if the Nagle Algorithm according to IETF RFC 896 is enabled or not. */
#define TCPIP_PARAMID_TCP_NAGLE  0x02U
/** \brief Specifies if TCP Keep Alive Probes are sent on the socket connection. */
#define TCPIP_PARAMID_TCP_KEEPALIVE 0x03U
/** \brief Specifies the time to live value for outgoing frames on the socket. */
#define TCPIP_PARAMID_TTL 0x04U
/** \brief Specifies the time between the last data packet sent and the first keepalive probe. */
#define TCPIP_PARAMID_TCP_KEEPALIVE_TIME 0x05U
/** \brief Specifies the maximum number of times that a keepalive probe is retransmitted. */
#define TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX 0x06U
/** \brief Specifies the interval between subsequent keepalive probes. */
#define TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL 0x07U
/** \brief Specifies which TCP option filter shall be applied on the related socket.*/
#define TCPIP_PARAMID_TCP_OPTIONFILTER 0x08U
/** \brief Specifies if the Path MTU Discovery shall be performed on the related socket.*/
#define TCPIP_PARAMID_PATHMTU_ENABLE 0x09U
/** \brief The 20-bit Flow Label according to IETF RFC 6437.*/
#define TCPIP_PARAMID_FLOWLABEL 0x0aU
/** \brief The 6-bit Differentiated Service Code Point according to IETF RFC 2474.*/
#define TCPIP_PARAMID_DSCP 0x0bU
/** \brief Specifies if UDP checksum handling shall be enabled (TRUE) or skipped (FALSE) on the related socket. */
#define TCPIP_PARAMID_UDP_CHECKSUM 0x0cU
/** \brief Specifies if unspecified ip address shall be used for transmission as source */
#define TCPIP_PARAMID_UDP_UNSPECIFIED_IP 0x81U

/** \brief Type for the specification of all supported Parameter IDs */
typedef uint8 TcpIp_ParamIdType;


/** \brief Protocol TCP */
#define TCPIP_IPPROTO_TCP 0x06U
/** \brief Protocol UDP */
#define TCPIP_IPPROTO_UDP 0x11U

/** \brief Protocol
 *
 * Type definition of the used protocol
 */
typedef uint8 TcpIp_ProtocolType;


/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Allocates resources for a new TCP/UDP socket
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket.
 * The configuration parameters TcpIpUdpSocketMax and TcpIpTcpSocketMax determines the
 * maximum number of sockets.
 * Note: Each accepted incoming TCP connection also allocates a socket resource.
 *
 * \param[in] Domain IP address family. Possible values are AF_INET (IPv4) and AF_INET6
 * (IPv6) which is not supported at the moment.
 * \param[in] Protocol Socket protocol as sub-family of parameter \a type. Possible values are
 * IPPROTO_TCP and IPPROTO_UDP.
 * \param[out] SocketIdPtr Pointer to TcpIp_SocketIdType representing the requested
 * SocketId. This SocketId must be provided for all further API calls which
 * requires a SocketId.
 * Note: SocketId only contains a valid value if return value is E_OK.
 * \param[in] sockOwnerId Id of the socket owner (specifies call-back functions)
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted: no free socket
 * resources found.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Socket_getSocket
(
  TcpIp_DomainType Domain,
  TcpIp_ProtocolType Protocol,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
);

/** \brief Binds a TCP/UDP socket to a local address/port pair
 *
 * By this API service the TCP/IP stack is requested to bind a UDP or TCP socket to a
 * local resource, specifying its IP address (via IpAddrId) and port number.
 * Sockets that shall be switched in a listening state later on must be bound to a
 * local resource.
 * Optionally this API can be used to specify the local IP address and port used by later
 * calls of TcpIp_TcpConnect() or TcpIp_Transmit().
 *
 * \param[in] SocketId identifying the local socket resource.
 * \param[in] LocalAddrId IP address identifier representing the local IP address and EthIf
 * controller to bind the socket to. Note: to listen on all available EthIf controller an
 * additional (specific) IpAddr entry can be configured in the TcpIp module configuration.
 * The remote IP address of an incoming packet has no effect then.
 * In case the socket shall be used as client socket, the IP address and EthIf controller
 * represented by IpAddrId is used for transmission.
 * Note: to use the default route an additional (specific) IpAddr entry has to be
 * configured in the TcpIp module configuration. The IP address given by parameter
 * \p localAddrPtr has not effect then.
 * \param[in,out] PortPtr port to which the socket shall be bound. In case the socket shall
 * be used as listen socket TCPIP_PORT_ANY accepts incoming packets regardless of the
 * destination port. In case the socket shall be used as client socket, TCPIP_PORT_ANY
 * let the TCP/IP stack choose the local port automatically and write it back to this parameter.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted (e.g. address in use)
 *
 * \ServiceID{0x05}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
);

/** \brief Closes a socket
 *
 * By this API service the TCP/IP stack is requested to close connection and frees
 * all resources. All buffers assigned to the socket are released regardless of UDP
 * or TCP sockets.
 * In case of a UDP socket the socket resources are released immediately and are
 * available again for socket allocation via TcpIp_GetSocket().
 * In case of TCP, the 4-way handshake for closing a TCP socket starts. After
 * completion the socket resources are released and the SoAd gets informed via
 * SoAd_TcpIpEvent().
 *
 * \param[in] SocketId identifying the local socket resource.
 * \param[in] Abort Terminate immediately?
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted (e.g. the TCP
 * connection was not established before).
 *
 * \ServiceID{0x04}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Close
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
);

/** \brief Changes socket configuration
 *
 * By this API service the TCP/IP stack is requested to change a connection
 * or socket parameter.
 * Nagle algorithm: the Nagle algorithm may be controlled by this API.
 * memLimit UDP: maximum size of UDP receive data queue in bytes, 0 for unlimited
 * memLimit TCP: size of the maximum TCP receive window in bytes, 0 for default
 * window size value
 *
 * \param[in] SocketId identifying the local socket resource.
 * \param[in] ParameterId Identifier of the parameter to be changed
 * \param[in] ParameterValue Points to memory location where new parameter value is stored.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 *
 * \ServiceID{0x0f}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue
);

/** \brief Checks if physical address is known and IpSec SA is established. If physical address is
 *  not known address resolution shall be triggered.
 *
 * By this API service TcpIp stack shall check if a connection is ready for transmission. This
 * is ensured by:
 * 1. If IPsec is enabled, checking that an IpSec Security Association (SA) is established if the
 *    connection shall be secured.
 * 2. Checking that the physical address corresponding to the remote IP address is known (i.e. it is
 *    present in the ARP cache for IPv4 or in the neighbor cache for IPv6). If this is not the case
 *    an address resolution (i.e. sending an ARP request for IPv4 or a neighbor solicitation
 *    message) request is triggered.
 *
 * \param[in]  SocketId      ID of the socket over which data shall be transmitted
 * \param[in]  RemoteAddrPtr UDP: Pointer to address and port of the remote host to which data shall
 *                                be transmitted.
 *                           TCP: NULL_PTR, for TCP a remote host does not need to be provided
 *                                because address is already known by TcpIp.
 *
 * \return Indicates if data can be transmitted.
 * \retval TCPIP_OK          Physical address known and IpSec SA exists, data can be sent
 * \retval TCPIP_E_PENDING   Physical address not known or IpSec SA does not exist
 * \retval TCPIP_E_NOT_OK    - Transmission not allowed according to policy table OR
 *                           - API called with invalid parameters if development error detection is
 *                             enabled OR
 *                           - An error occurs during processing of the request.
 *
 * \ServiceID{0xF5}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Synchronous}
 */
extern FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IsConnectionReady
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* ifndef TCPIP_SOCKET_API_H */
/*==================[end of file]===========================================*/
