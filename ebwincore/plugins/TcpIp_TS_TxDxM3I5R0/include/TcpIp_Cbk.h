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
#ifndef TCPIP_CBK_H
#define TCPIP_CBK_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>     /* AUTOSAR comstack types */
#include <Eth_GeneralTypes.h>   /* Ethernet type definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief TcpIp receive interface
 *
 * This is the receive interface of the TcpIp stack. All received data must be passed to the
 * TcpIp module using this API function.
 *
 * \param[in] CtrlIdx Index of the ethernet controller (EthIfCtrlIdx)
 * \param[in] FrameType Value of ethernet header type-field.
 * \param[in] IsBroadcast Indicates if the target MAC address is a broadcast address
 * \param[in] PhysAddrPtr Points to MAC address of remote host (source MAC)
 * \param[in] DataPtr Points to the received data. The data contains the payload of the
 * Ethernet protocol (which excludes the Ethernet header but includes headers of higher layers).
 * \param[in] LenByte Length of received data in units of bytes.
 *
 * \ServiceID{0x14}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC( void, TCPIP_CODE ) TcpIp_RxIndication
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 LenByte
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* ifndef TCPIP_UDP_API_H */
/*==================[end of file]===========================================*/
