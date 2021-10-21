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

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Usage of concatenation operator improves readability of code.
 *
 *
 * MISRAC2012-2) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 *
 * MISRAC2012-4) Deviated Rule: 19.2 (advisory)
 * The union keyword should not be used.
 *
 * Reason:
 * Union is used to force 32 bit alignment of encapsulated structure.
 * Reuse of memory does not occur, since any memory access is performed via the structure.
 * No reliance on implementation-defined behavior.
 *
 */

#ifndef TCPIP_SOCKET_INT_H
#define TCPIP_SOCKET_INT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Socket_Api.h>       /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>              /* module internal headerfile */
#include <TcpIp_Ip_Int.h>           /* SW-Unit IP interface definitions */
#include <TcpIp_Tcp_IntTypes.h>     /* SW-Unit IP interface definitions */

/*==================[macros]================================================*/


/** \brief Macro extracts protocol information out of socket page */
#define TCPIP_SOCKET_PAGE_PROTOCOL_GET(page) ((uint8)((page)>>1u) & 1u)

/** \brief Macro tests protocol of a socket page */
/* Deviation MISRAC2012-1 <+2> */
#define TCPIP_SOCKET_IS_PAGE_PROTOCOL(page,protocol)\
  (TCPIP_SOCKET_PAGE_PROTOCOL_GET(page) == TCPIP_SOCKET_PAGE_PROTOCOL_##protocol)

/** \brief socket page UDP protocol value */
#define TCPIP_SOCKET_PAGE_PROTOCOL_UDP 0u
/** \brief socket page TCP protocol value */
#define TCPIP_SOCKET_PAGE_PROTOCOL_TCP 1u
/** \brief total number of protocol pages IDs supported */
#define TCPIP_SOCKET_NUMPROT           2u

/** \brief flag, clone the listen socket */
#define TCPIP_SOCKET_CLONE             4u

/** \brief Checks the protocol of a particular socket */
/* Deviation MISRAC2012-2 <+3> */
/* Deviation MISRAC2012-1 <+2> */
#define TcpIp_Socket_checkSocketProtocol(socket,protocol) \
  TcpIp_Socket_checkSocketProtocolFct((socket),TCPIP_SOCKET_PAGE_PROTOCOL_##protocol)

/* Deviation MISRAC2012-1 <START> */
#define TcpIp_Socket_TCP_Template_Type(remoteType,typename) \
  /** \brief data type containing all information for handling Tcp communication (generic from lower layer protocol) */ \
  typedef struct \
  {\
    /** \brief generic socket data - this must be the first member of this structure!!! */\
    TcpIp_Socket_Generic_Type generic;\
    /** \brief TCP control block */\
    TcpIp_TcpCtrlBlockType tcp;\
    /** \brief pointer to the listen socket this socket has been derived from */\
    TcpIp_SocketIdType listenSocketId;\
    /** \brief maximum number of connections derivable from this (listen) socket */\
    uint16 maxConnections;\
    /** \brief maximum number of established connections derivable from this (listen) socket */\
    uint16 numEstablishedConnections;\
    /** \brief current number of connections derived from this (listen) socket */\
    uint16 numConnections;\
    /** \brief remote port and remote address of the socket */\
    remoteType remote;\
  } typename##_Type\
/* Deviation MISRAC2012-1 <STOP> */

/** \brief Checks if infinite passive connections are available (TcpIp_TcpListen called with MaxChannels equal 0) */
#define TCPIP_TCP_SOCKETS_INFINITE_PASSIVE_CONNECTIONS_AVAIL(socketPtr) ((socketPtr)->maxConnections == 0U)

/*==================[type definitions]======================================*/

/** \brief This structure stores local connection information */
typedef struct
{
  /** \brief local port of the socket */
  uint16 port;
  /** \brief local address of the socket */
  TcpIp_LocalAddrIdType addrId;
} TcpIp_SockAddrLocalType;

/** \brief Pointer to structure of type TcpIp_SocketType */
typedef P2VAR(TcpIp_SockAddrLocalType,TYPEDEF,TCPIP_APPL_DATA) TcpIp_SockAddrLocalPtrType;

/** \brief data type containing all generic socket data.
           This type is used as interface only, but not as instance
 */
 /* Deviation MISRAC2012-4 */
typedef union
{
  struct
  {
    /** \brief local port and local address of the socket */
    TcpIp_SockAddrLocalType local;
    /** \brief local address identification this socket is assigned to */
    TcpIp_Ip_IpParameterType ipParameters;
    /** \brief pointer to next socket element (linked list maintenance) */
    TcpIp_SocketIdType next;
    /** \brief socket state (for development error detection) */
    uint8 state;
    /** \brief indicates the upper layer */
    uint8 sockOwnerId;
  } aligned;
  /** \brief dummy member to force 4 byte alignment of the struct
   * (required for gcc-4.9-2015q3-arm-eabi); this member is never to be accessed. **/
  uint32 force_align32_do_not_use;
} TcpIp_Socket_Generic_Type;

/** \brief pointer to data type containing generic socket */
typedef P2VAR(TcpIp_Socket_Generic_Type,TYPEDEF,TCPIP_APPL_DATA) TcpIp_Socket_Generic_PtrType;


/** \brief data type containing all information for handling Tcp communication (generic from lower layer protocol) */
typedef struct
{
  TcpIp_Socket_Generic_Type generic;
  /**< generic socket data - this must be the first member of this structure!!! */
} TcpIp_Socket_UDP_Generic_Type;

/** \brief pointer to data type containing all information for handling Udp */
typedef P2VAR(TcpIp_Socket_UDP_Generic_Type,TYPEDEF,TCPIP_APPL_DATA) TcpIp_Socket_UDP_Generic_PtrType;

/** \brief generic (domain) data type containing all information for handling Tcp communication */
TcpIp_Socket_TCP_Template_Type(TcpIp_SockAddrType,TcpIp_Socket_TCP_Generic);

/** \brief pointer to data type containing all information for handling Tcp */
typedef P2VAR(TcpIp_Socket_TCP_Generic_Type,TYPEDEF,TCPIP_APPL_DATA) TcpIp_Socket_TCP_Generic_PtrType;

/** \brief The IP pointer to \a TcpIp_SocketIdType */
typedef P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_SocketIdPtrType;

/*==================[external data declarations]============================*/

/*==================[external function declarations]========================*/
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initializes socket data structures
 *
 * This function initializes the socket data structures by putting all socket
 * data structures into the FreeSocket list.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_init(void);

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

/** \brief Retrieve Transmission Control Block of a socket connection
 *
 * This function returns the socket of the corresponding socket id.
 *
 * param[in]  socketId      Id of the socket connection
 * return The address of the local socket.
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(TcpIp_Socket_Generic_Type,  TCPIP_APPL_DATA, TCPIP_CODE)
  TcpIp_Socket_getSocketPtr
(
  TcpIp_SocketIdType socketId
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

/** \brief Checks if a given socket is a kind of requested protocol socket
 *
 * \param[in]  socketId  Socket identifier
 * \param[in] protocol  protocol to check socket against
 *
 * \return result of evaluation
 * \retval E_OK socket matches the given protocol
 * \retval E_NOT_OK socket does not match the given protocol
 */
/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkSocketProtocolFct
(
  TcpIp_SocketIdType socketId,
  uint8 protocol
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */


/** \brief Checks if a given socketId is valid
 *
 * \param[in]  socketId  Socket identifier
 *
 * \return result of evaluation
 * \retval E_OK socketId is valid
 * \retval E_NOT_OK socketId is not valid
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkSocketId
(
  TcpIp_SocketIdType socketId
);

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )
#if TCPIP_IP_DUALSTACK == STD_ON
/** \brief Checks if a given domain is valid for the socket
 *
 * \param[in]  socketId  Socket identifier
 * \param[in]  domain    Domain of the remote address
 *
 * \return result of evaluation
 * \retval E_OK     domain is valid
 * \retval E_NOT_OK domain is not valid
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkDomain
(
  TcpIp_SocketIdType socketId,
  TcpIp_DomainType domain
);
#endif /* TCPIP_IP_DUALSTACK == STD_ON */
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#if ( TCPIP_UDP_ENABLED == STD_ON )

/** \brief Automatically binds a socket to an Udp port and a local address
 *
 * If the socket by addressed socketId is not already bound (local_port == 0),
 * then this function automatically binds this socket by setting the following socket structure members:
 * - local_port:   get an unused Udp port in range of 49152 to 65535.
 * - local_addrId: based on the remoteAddrPtr, this functions selects the interface and the localAddrId
 *                 used for transmission.
 * Both are finally returned to the caller via output parameters IpAddrIdPtr and portPtr.
 *
 * If the socket is already bound (local_port != 0) but local address is set to ANY interface,
 * then this function only selects a temporary TxLocalAddrId to return, but does not modify local_addrId.
 *
 * TcpIp_IpAddrM_incLocalAddrRef() is called only if the socket has not been bound before.
 *
 * Note that this function cannot fail, since TcpIp always has at least a single unicast address
 * and the maxmimum number of sockets does not exceed 16383 (65535 - 49152).
 *
 * \param[in]  remoteAddrPtr  IP address and port of the remote host to transmit to.
 * \param[out] unicastLocalAddrIdPtr Pointer to return the unicast localAddrId which was selected during auto bind for transmission.
 * \param[out] portPtr        Pointer to return the port which was selected during auto bind.
 * \param[in] socketId        identifying the local socket resource.
 *
 * \retval TCPIP_OK         Socket has been bound.
 * \retval TCPIP_E_NOT_OK   Socket binding has failed.
 *
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_autoBindUdp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_LocalAddrIdType,AUTOMATIC,TCPIP_APPL_DATA) unicastLocalAddrIdPtr,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) portPtr,
  TcpIp_SocketIdType socketId
);



/** \brief Lookup the bound sockets for matching the local address and the local socket.
 *
 * This function performs a lookup on the bound sockets for matching LocalAddrId and local_port and returns a list of sockets found.
 * Depending on the type of localAddrId the behavior is slightly different:
 * Unicast: traverse the following bound sockets in the following order:  unicast - ANY-IP - ANY
 *          Indicate data to the first matching socket owner and then abort.
 * Multicast: traverse the following bound sockets in the following order:  multicast - ANY-IP - ANY
 *            Indicate data to owner every matching socket.
 *
 * \param[in]  maxSocketInstances Maximum number of sockets which should be found (=length of list pointed to by socketIdPtr).
 * \param[in]  local_port         Destination Udp port the datagram.
 * \param[in]  localAddrId        Local Address (interface) the data has been received on.
 * \param[in]  localIpAddrPtr     Local Address the data has been received on.
 * \param[in]  domain             Domain of the Local Address (interface) the data has been received on.
 * \param[out] socketIdsPtr       Pointer to list of sockets found.
 *
 * \return Number of matching sockets found.
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_Socket_lookupUdp
(
    uint8 maxSocketInstances,
    uint16 local_port,
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
    TcpIp_DomainType domain,
    P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdsPtr
);



#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */



/** \brief Requests closing sockets bound to a particular local address identifier
 *
 * This function requests closing all sockets bound to a particular local address identifier and finally
 * calls/initiates to call TcpIp_IpAddrM_socketsClosed() when done.
 * Note: UDP sockets will be closed immediately.
 *
 * \param[in] localAddrId     local address identifier
 */
TS_MOD_PRIV_DECL FUNC(void,TCPIP_CODE) TcpIp_Socket_closeSockets(TcpIp_LocalAddrIdType localAddrId);



/** \brief Immediately closes sockets bound to a particular local address identifier
 *
 * This function closes all sockets bound to a particular local address identifier immediately
 *
 * \param[in] localAddrId       local address identifier
 * \param[in] informRemoteHost  indicates if the remote host shall be infromed that connection
 *                              is reset, in case of TCP a RST is transmitted
 */
TS_MOD_PRIV_DECL FUNC(void,TCPIP_CODE) TcpIp_Socket_terminateSockets
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean informRemoteHost
);

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Bind the socket to a local address
 *
 * This function binds the socket to a local address and a random port if it was not specified by a
 * previous bind call.
 *
 * \note If an socket already exists for this connection (bi-directional open),
 *       then this function returns E_NOT_OK.
 *
 * param[in]   socketId               Uniqe socket identifier
 * param[in]   remoteAddrPtr          IP address and port of the remote host
 * param[out]  localAddrPtr           IP address and port of the local host
 *
 * \retval TCPIP_OK         Socket has been bound.
 * \retval TCPIP_E_NOT_OK   Socket has not been bound.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_autoBindTcp
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
);

/** \brief Update connection information and attach socket to active list
 *
 * This function updates the socket connection with the information of the remote host,
 * attaches the socket into the active socket list.
 *
 * param[in]  socketId               Uniqe socket identifier
 * param[in]  remoteAddrPtr          IP address and port of the remote host
 * param[in]  localAddrPtr           IP address and port of the local host
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_activateTcp
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
);

/** \brief Retrieves transmission control block of a TCP connection which matches IP addresses and
 *         ports
 *
 * This function iterates over the active and listen socket lists to find a connection which matches
 * with the received IP addresses and the ports.
 *
 * param[in]  localAddrPtr   Local socket the segment has been received on.
 * param[in]  remoteAddrPtr  IP address and port of the remote host
 * param[out] socketPtrPtr   Address the address of the local socket is written to
 * param[out] socketIdPtr    Socket id of the matching connection
 *
 * \retval TCPIP_OK       Connection found.
 * \retval TCPIP_E_NOT_OK   Connection does not exist.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_lookupTcp
(
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr
);


/** \brief Retrieves transmission control block of a cloned TCP connection
 *
 * This function creates a clone of the given listen socket and returns it.
 *
 * \param[in]     localAddrPtr             Local socket the segment has been received on.
 * \param[in]     remoteAddrPtr            IP address and port of the remote host
 * \param[in,out] socketPtrPtr             in: pointer to the listen socket
 *                                         out: pointer to the new (cloned) socket
 * \param[in,out] socketIdPtr              in: socket id of the listen socket
 *                                         out: socket id of the new (cloned) socket
 * \param[in]     alsoCheckNotEstablished  FALSE: only gets free sockets for cloning
 *                                         TRUE: also checks if there are sockets which are
 *                                               currently not established and belong to the
 *                                               same listen socket if there is no free socket
 *                                               left or maximal number of allowed connections
 *                                               is reached
 *
 * \retval TCPIP_OK         Clone was created.
 * \retval TCPIP_E_PENDING  Clone could temporarily not be created, max socket in use
 * \retval TCPIP_E_NOT_OK   Clone wasn't created.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_cloneListenSocket
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  boolean alsoCheckNotEstablished
);

/** \brief Notify socket unit that a connection is established
 *
 * This function is called when a TCP connection is successfully established.
 * Note: When SYN cookies is active and an ACK is received, the TcpIp can retrieve
 * a socket which is already allocated (state SYN_RECEIVED) but not established.
 *
 * param[in]  socketId     Id of the socket connection which has been established
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_establishConnection
(
  TcpIp_SocketIdType socketId
);

#if (TCPIP_TCP_CRYPTO_ENABLED == STD_ON)

/** \brief Setting data in array for hashing to get sequence number
 *
 * This function sets data in array for hashing to get sequence number
 *
 * param[in]   localAddrPtr        Pointer to structure containing the local IP parameters
 * param[in]   remoteAddrPtr       Pointer to structure containing the remote IP parameters
 * param[out]  hashDataPtr         Ponter to the hash array
 * param[out]  lengthDataPtr       Pointer to data length in hash array
 * param[in]   socketId            Socket ID
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_setHashData
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) hashDataPtr,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) lengthDataPtr,
  TcpIp_SocketIdType socketId
);
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

/** \brief Attach socket to listen list
 *
 * This function indicates that a socket is ready to listen for incoming data. It attaches the
 * socket to the listen socket list.
 *
 * param[in]  socketId    id of the socket which should be attached to the listen list.
 * param[in]  maxChannels Maximum number of parallel connections which might be opened for this listen socket.
 *
 * \retval TCPIP_OK         Socket put into listen state.
 * \retval TCPIP_E_NOT_OK   Socket not put into listen state (e.g. socket was not bound before).
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_listenTcp
(
  TcpIp_SocketIdType socketId,
  uint16 maxChannels
);

/** \brief Returns the listen socket this socket has been derived from
 *
 * param[in]  socketId    id of the socket.
 *
 * \return SocketId of socket the requested socket has been derived from (member listenSocketId).
 * \retval TCPIP_SOCKETID_INVALID socket has not been derived from any listen socket
 *                               listen socket does not exist any more
 *                               current socket is not in state ACTIVE
 *
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_Socket_getListenSocket
(
  TcpIp_SocketIdType socketId
);

/** \brief Free socket and remove socket from any search list
 *
 * This function removes the specified socket from any search list and frees it for
 * new TCP connections.
 *
 * param[in]  socketId  id of the socket connection
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_closeTcp
(
  TcpIp_SocketIdType socketId
);


#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

/** \brief returns E_NOT_OK if a bound Tcp socket with the same port and local address already exist */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_isAlreadyListen
(
  TcpIp_SocketIdType SocketId
);

#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief MainFunction for SW-Unit socket
 *
 *  Calls TcpIp_Tcp_processSocket() for each Tcp socket which is in state ACTIVE.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_mainFunction
(
  void
);

/** \brief MainFunction for socket transmission
 *
 *  Calls TcpIp_Socket_processTcpSocketsTx() for each Tcp socket which is in state ACTIVE.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_mainFunctionTx
(
  void
);

/** \brief Binds a TCP/UDP socket to a local address/port pair
 *
 * \param[in] SocketId    identifying the local socket resource.
 * \param[in] LocalAddrId IP address identifier representing the local IP address and EthIf
 *                         controller to bind the socket to.
 * \param[in,out] PortPtr  port to which the socket shall be bound. In case the socket shall
 *                         be used as listen socket TCPIP_PORT_ANY accepts incoming packets regardless
 *                         of the destination port. In case the socket shall be used as client socket,
 *                         TCPIP_PORT_ANY let the TCP/IP stack choose the local port automatically and
 *                         write it back to this parameter.
 * \param[in] checkDet     indicates if DET is checked
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted (e.g. address in use)
 *
 */
TS_MOD_PRIV_DECL FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Socket_bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr,
  boolean checkDet
);


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_SOCKET_INT_H */
/*==================[end of file]===========================================*/
