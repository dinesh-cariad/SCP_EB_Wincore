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

#ifndef TCPIP_DHCPV4_EXT_H
#define TCPIP_DHCPV4_EXT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Api.h>            /* general TcpIp type definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Handle Reception of DHCP message
 *
 * This function handles the reception of DHCP offer and acknowledge messages.
 *
 * \param[in] SocketId       Socket identifier of the related local socket resource.
 * \param[in] RemoteAddrPtr  Pointer to memory containing IP address and port of the
 *                           remote host which sent the data.
 * \param[in] BufPtr         Pointer to the received data.
 * \param[in] Length         Data length of the received TCP segment or UDP datagram.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_rxIndication
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
);


/** \brief check if linkLocal address is assigned
 *
 * This function checks if Broadcast address is assigned/unassigned
 * and opens/closes the socket for message exchange with the DhcpV4 server
 *
 * \param[in] IpAddrId   IP address Identifier, representing an IP address specified
 *                       in the TcpIp module configuration (
 * \param[in] State      State of IP address assignment.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_LocalIpAddrAssignmentChg
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
);

/** \brief copy Dhcp message into buffer
 *
 * This function is called during TcpIp_UdpTransmit to copy the DHCP message into
 * the hardware buffer
 *
 * \param[in] SocketId   Socket identifier of the related local socket resource.
 * \param[in] BufPtr     Pointer to the buffer to copy data to
 * \param[in] BufLength  Size of the provided buffer
 */
extern FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_CopyTxData
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_DHCPV4_EXT_H */
