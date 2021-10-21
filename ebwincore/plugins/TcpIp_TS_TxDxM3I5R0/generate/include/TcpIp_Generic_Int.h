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

#ifndef TCPIP_GENERIC_INT_H
#define TCPIP_GENERIC_INT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Socket_Api.h>   /* used SW-unit interface file */

/*==================[macros]================================================*/

[!//
[!VAR "SockOwnCount"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpSocketOwnerConfig/TcpIpSocketOwner/*)"!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!//

#ifdef TcpIp_DhcpV4_getSocket
  #error already defined: TcpIp_DhcpV4_getSocket
#endif
/** \brief Allocates resources for a new TCP/UDP socket (Dhcp)
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket.
 * Through this API, the Dhcp can be defined as upper layer and the call-back functions
 * of the Dhcp will be called for the allocated socket.
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
#define TcpIp_DhcpV4_getSocket(Domain, Protocol, SocketIdPtr)             \
  TcpIp_Socket_getSocket(Domain, Protocol, SocketIdPtr, [!"num:i($SockOwnCount)"!]U)

[!VAR "SockOwnCount"="$SockOwnCount + 1"!][!//
[!//
[!ENDIF!][!//

[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!//

#ifdef TcpIp_DhcpV6_getSocket
  #error already defined: TcpIp_DhcpV6_getSocket
#endif
/** \brief Allocates resources for a new TCP/UDP socket (DhcpV6)
 *
 * By this API service the TCP/IP stack is requested to allocate a new socket.
 * Through this API, the Dhcp can be defined as upper layer and the call-back functions
 * of the Dhcp will be called for the allocated socket.
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
#define TcpIp_DhcpV6_getSocket(Domain, Protocol, SocketIdPtr)             \
  TcpIp_Socket_getSocket(Domain, Protocol, SocketIdPtr, [!"num:i($SockOwnCount)"!]U)
[!//
[!ENDIF!][!//


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_GENERIC_INT_H */
