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

#ifndef TCPIP_GENERIC_API_H
#define TCPIP_GENERIC_API_H

/*==================[macros]================================================*/

[!VAR "SockOwnCount"="0"!][!//
[!LOOP "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpSocketOwnerConfig/TcpIpSocketOwner/*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!//
#ifdef TcpIp_GetSocket
  #error already defined: TcpIp_GetSocket
#endif
/** \brief Allocates resources for a new TCP/UDP socket (Soad)
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket. Through
 * this API, Soad can be defined as upper layer and the call-back functions of the Soad
 * module will be called for the allocated socket. Note: This API exists to maintain
 * backwards compatibility.
 *
 * \param[in]  Domain       IP address family.
 * \param[in]  Protocol     Socket protocol as sub-family of parameter type
 * \param[out] SocketIdPtr  Pointer to TcpIp_SocketIdType representing the requested
 *                          SocketId. This SocketId must be provided for all further API
 *                          calls which requires a SocketId. Note: SocketId only contains
 *                          a valid value if return value is E_OK.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted: no free socket
 */
/* !LINKSTO TcpIp.DesignGetSocket,1 */
#define TcpIp_GetSocket(Domain, Protocol, SocketIdPtr)                    \
  TcpIp_Socket_getSocket(Domain, Protocol, SocketIdPtr, [!"num:i($SockOwnCount)"!]U)

#ifdef TcpIp_SoAdGetSocket
  #error already defined: TcpIp_SoAdGetSocket
#endif
/** \brief Allocates resources for a new TCP/UDP socket (Soad)
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket. Through
 * this API, Soad can be defined as upper layer and the call-back functions of the Soad
 * module will be called for the allocated socket.
 *
 * \param[in]  Domain       IP address family.
 * \param[in]  Protocol     Socket protocol as sub-family of parameter type
 * \param[out] SocketIdPtr  Pointer to TcpIp_SocketIdType representing the requested
 *                          SocketId. This SocketId must be provided for all further API
 *                          calls which requires a SocketId. Note: SocketId only contains
 *                          a valid value if return value is E_OK.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted: no free socket
 */
/* !LINKSTO TcpIp.DesignUpSocket,1 */
#define TcpIp_SoAdGetSocket(Domain, Protocol, SocketIdPtr)                \
  TcpIp_Socket_getSocket(Domain, Protocol, SocketIdPtr, [!"num:i($SockOwnCount)"!]U)

[!//
[!ELSE!][!//
[!//
#ifdef TcpIp_[!"node:name(.)"!]GetSocket
  #error already defined: TcpIp_[!"node:name(.)"!]GetSocket
#endif
/** \brief Allocates resources for a new TCP/UDP socket ([!"node:name(.)"!])
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket.
 * Through this API, the socket owner [!"node:name(.)"!] can be defined as upper layer and
 * the call-back functions of the socket owner [!"node:name(.)"!] will be called for the
 * allocated socket.
 *
 * \param[in]  Domain       IP address family.
 * \param[in]  Protocol     Socket protocol as sub-family of parameter type
 * \param[out] SocketIdPtr  Pointer to TcpIp_SocketIdType representing the requested
 *                          SocketId. This SocketId must be provided for all further API
 *                          calls which requires a SocketId. Note: SocketId only contains
 *                          a valid value if return value is E_OK.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted: no free socket
 */
/* !LINKSTO TcpIp.DesignUpSocket,1 */
#define TcpIp_[!"node:name(.)"!]GetSocket(Domain, Protocol, SocketIdPtr)  \
  TcpIp_Socket_getSocket(Domain, Protocol, SocketIdPtr, [!"num:i($SockOwnCount)"!]U)

[!//
[!ENDIF!][!//
[!VAR "SockOwnCount"="$SockOwnCount + 1"!][!//
[!ENDLOOP!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_GENERIC_API_H */
