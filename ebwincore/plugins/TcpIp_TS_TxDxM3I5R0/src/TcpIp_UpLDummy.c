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

/*==================[inclusions]============================================*/

#include <TcpIp_UpLDummy.h>       /* function declerations */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/* Socket owner dummy functions */

FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerRxIndication_dummy
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
)
{
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(RemoteAddrPtr);
  TS_PARAM_UNUSED(BufPtr);
  TS_PARAM_UNUSED(Length);
}

FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_SocketOwnerCopyTxData_dummy
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
)
{
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(BufPtr);
  TS_PARAM_UNUSED(BufLength);

  return BUFREQ_OK;
}

FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTcpIpEvent_dummy
(
  TcpIp_SocketIdType SocketId,
  TcpIp_EventType Event
)
{
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(Event);
}

FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerLocalIpAddrAssignmentChg_dummy
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
)
{
  TS_PARAM_UNUSED(IpAddrId);
  TS_PARAM_UNUSED(State);
}

#if (TCPIP_TCP_ENABLED == STD_ON)

FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTxConfirmation_dummy
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
)
{
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(Length);
}

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SocketOwnerTcpAccepted_dummy
(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(SocketIdConnected);
  TS_PARAM_UNUSED(RemoteAddrPtr);

  return E_OK;
}

FUNC(void, TCPIP_CODE) TcpIp_SocketOwnerTcpConnected_dummy
(
  TcpIp_SocketIdType SocketId
)
{
  TS_PARAM_UNUSED(SocketId);
}

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
