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

#ifndef TCPIP_DHCPV6_INT_H
#define TCPIP_DHCPV6_INT_H

/*==================[inclusions]============================================*/
#include <TcpIp_DhcpV6_Api.h>       /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>              /* module internal headerfile */
#include <TcpIp_IpAddrM_Int.h>      /* own SW-unit interface file */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)

/** \brief Initialize DHCPv6
 *
 * This function initialize the DHCPv6 data structures.
 *
 * \param[in] LocalIpAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_init
(
  TcpIp_LocalAddrIdType LocalIpAddrId
);

/** \brief Request DHCPv6 assignment.
 *
 * This function decides what to do if the IP address manager wants to assign the
 * DHCPv6 address in a specific state of the DHCPv6 state machine. It checks if
 * there is an assigned LinkLocal address before It opens a DHCPv6 socket
 * and delays the transmission of the first DHCPv6 SOLICIT message.
 * \param[in] LocalAddr  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_startAssign
(
  TcpIp_LocalAddrIdType LocalAddr
);

/** \brief Releases DHCPv6 assignment.
 *
 * This function decides what to do if the IP address manager wants to unassign the
 * DHCPv6 address in a specific state of the DHCPv6 state machine. It closes the
 * socket connection.
 * \param[in] LocalAddr  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_stopAssign
(
  TcpIp_LocalAddrIdType LocalAddr
);

/** \brief Triggers periodic DHCPv6 functions.
 *
 * This function handles retransmissions and lease time expirations. The
 * TcpIp_DhcpV6_mainFunction() shall be called after the TcpIp_IpAddrM_mainFunction()
 * to ensure that IP Address changes are handled in the same TcpIp_MainFunction context.
 *
 * \param[in] timerTick  current time in ticks of TcpIp timer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_mainFunction
(
  uint16 timerTick
);

/** \brief get Ip configuration of the DHCPv6 assignment
 *
 * This API function outputs the IP configuration of the DHCPv6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] assignedIpPtrPtr Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the DHCPv6 assignment
 *
 * This API function outputs the IP address of the DHCPv6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] assignedIpPtrPtr Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) assignedIpPtrPtr
);

/** \brief inform DHCPv6 whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
);

/** \brief dummy function to request assignment of IP address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] LocalIpAddrPtr  Pointer to requested IP
 *
 * \retval E_OK Request is successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TCPIP_IPADDRM_ADDRSTATE_PREFERRED  ip address is preferred
 * \retval TCPIP_IPADDRM_ADDRSTATE_DEPRECATED ip address is deprecated (preferred life time expired)
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV6_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId
);

#if  (TCPIP_DHCPGETSTATUS_API == STD_ON)
/** \brief Read DHCP status
 *
 * This functions reads the state of a specific DHCP address.
 *
 * \param[in]  localAddressId    Address Id for the DHCP address
 * \param[in]  statusPtr         Pointer to a status variable. Contains the state information.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_getStatus
(
  TcpIp_LocalAddrIdType localAddressId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) statusPtr
);
#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */

#endif /* TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_DHCPV6_INT_H */
