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

#ifndef TCPIP_UPLDUMMY_H
#define TCPIP_UPLDUMMY_H

/*==================[inclusions]============================================*/

#include <TcpIp_Api.h>            /* general TcpIp type definitions */
#include <TcpIp_Socket_Api.h>     /* public API implemented by SW-unit Socket */
#include <TcpIp_IpAddrM_Api.h>    /* public API implemented by SW-unit IpAddrM */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief <Up>_RxIndication Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerRxIndicationName.
 *
 * \param[in] SocketId       Socket identifier of the related local socket resource.
 * \param[in] RemoteAddrPtr  Pointer to memory containing IP address and port of the
 *                           remote host which sent the data.
 * \param[in] BufPtr         Pointer to the received data.
 * \param[in] Length         Data length of the received TCP segment or UDP datagram.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerRxIndication_dummy
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
);

/** \brief <Up>_CopyTxData Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerCopyTxDataName.
 *
 * \param[in] SocketId   Socket identifier of the related local socket resource.
 * \param[in] BufPtr     Pointer to buffer for transmission data.
 * \param[in] BufLength  Length of provided data buffer.
 *
 * \return BUFREQ_OK
 */
extern FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_SocketOwnerCopyTxData_dummy
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
);

/** \brief <Up>_TcpIpEvent Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerTcpIpEventName.
 *
 * \param[in] SocketId   Socket identifier of the related local socket resource.
 * \param[in] Event      This parameter contains a description of the event just
 *                       encountered.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTcpIpEvent_dummy
(
  TcpIp_SocketIdType SocketId,
  TcpIp_EventType Event
);

/** \brief <Up>_LocalIpAddrAssignmentChg Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerLocalIpAddrAssignmentChgName.
 *
 * \param[in] IpAddrId   IP address Identifier, representing an IP address specified
 *                       in the TcpIp module configuration (e.g. static IPv4 address on
 *                       EthIf controller 0).
 * \param[in] State      State of IP address assignment.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerLocalIpAddrAssignmentChg_dummy
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
);

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief <Up>_TxConfirmation Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerTxConfirmationName.
 *
 * \param[in] SocketId  Socket identifier of the related local socket resource.
 * \param[in] Length    Number of transmitted data bytes.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTxConfirmation_dummy
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
);

/** \brief <Up>_TcpAccepted Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerTcpAcceptedName.
 *
 * \param[in] SocketId           Socket identifier of the related local socket resource
 *                               which has been used at TcpIp_Bind()
 * \param[in] SocketIdConnected  Socket identifier of the local socket resource used for
 *                               the established connection.
 * \param[in] RemoteAddrPtr      IP address and port of the remote host.
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SocketOwnerTcpAccepted_dummy
(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief <Up>_TcpConnected Dummy Function
 *
 * This function is called if the upper layer does not specify a
 * TcpIpSocketOwnerTcpConnectedName.
 *
 * \param[in] SocketId  Socket identifier of the related local socket resource.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTcpConnected_dummy
(
  TcpIp_SocketIdType SocketId
);

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_UPLDUMMY_H */
