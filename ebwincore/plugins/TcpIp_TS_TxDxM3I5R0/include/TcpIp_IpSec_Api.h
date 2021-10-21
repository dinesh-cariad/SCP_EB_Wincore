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
#ifndef TCPIP_IPSEC_API_H
#define TCPIP_IPSEC_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */


#include <TcpIp_IpSec_Api.guards>
/*==================[macros]================================================*/

/** \brief IPsec is activated for this connection */
#define TCPIP_IPSEC_ACTIVE                                             0U
/** \brief IPsec is deactivated for this connection */
#define TCPIP_IPSEC_INACTIVE                                           1U

#define TCPIP_IPSEC_INVALID_ICV                                        2U
/*==================[type definitions]======================================*/
/** \brief Specifies the IPsec state for a specific IPsec connection. */
typedef uint8 TcpIp_IPsecStateType;

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Request to change state of communication security
 *
 * By this API service the TCP/IP stack is requested to change the IpSec state of
 * the communication network identified by IpSec connection index.
 *
 * \param[in] conId     IpSec connection index to identify the communication where the TcpIp state
 *                      is requested.
 * \param[in] reqState  Requested TcpIp state.
 *
 * \return Result of operation
 * \retval E_OK      Service accepted
 * \retval E_NOT_OK  Service denied
 *
 * \ServiceID{0xF2}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpSecMode
(
  uint8 conId,
  TcpIp_IPsecStateType reqState
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_IPSEC_API_H */
/*==================[end of file]===========================================*/
