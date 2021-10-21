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
#ifndef TCPIP_UDP_API_H
#define TCPIP_UDP_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Requests to transmit data to a remote destination via UDP protocol
 *
 * This service transmits UDP data to a remote node. The transmission of the
 * data is immediately performed with this function call by forwarding it to EthIf.
 * The parameter TotalLength indicates the payload size of the UDP datagram which
 * shall be sent by the TCP/IP stack within the context of this transmit
 * function call.
 * If DataPtr is a NULL_PTR the payload must be retrieved via SoAd_CopyTxData().
 * If DataPtr is a valid pointer, it points to a linear buffer of TotalLength bytes
 * containing the payload. If the socket has not been bound to a local resource via
 * a previous call to TcpIp_Bind() the local IP address and port used for transmission
 * is selected by the TcpIp stack.
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
 * \retval E_OK      UDP message transmission was successful.
 * \retval E_NOT_OK  UDP message transmission failed.
 * \retval TCPIP_E_PHYS_ADDR_MISS UDP message could not be sent because of an ARP
 * cache miss, ARP request has been sent and upper layer may retry transmission by
 * calling this function later again.
 *
 * \ServiceID{0x12}
 * \Reentrancy{Reentrant for different SocketId}
 * \Synchronicity{Synchronous}
 */
extern FUNC( TcpIp_ReturnType, TCPIP_CODE ) TcpIp_UdpTransmit
(
  TcpIp_SocketIdType  socketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint16 totalLength
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* ifndef TCPIP_UDP_API_H */
/*==================[end of file]===========================================*/
