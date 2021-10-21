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

#ifndef TCPIP_DHCPV4_INT_H
#define TCPIP_DHCPV4_INT_H

/*==================[inclusions]============================================*/
#include <TcpIp_DhcpV4_Api.h>       /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>              /* module internal headerfile */
#include <TcpIp_IpAddrM_Int.h>      /* own SW-unit interface file */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initalize DHCP
 *
 * This function initialize the DHCP data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_init
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Request DHCP assignment.
 *
 * This function decides what to do if the IP address manager wants to assign the
 * DHCP address in a specific state of the DHCP state machine. It opens a DHCP socket
 * and delays the transmission of the first DHCP DISCOVER message.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Releases DHCP assignment.
 *
 * This function decides what to do if the IP address manager wants to unassign the
 * DHCP address in a specific state of the DHCP state machine. It transmits the DHCP
 * release messages and closese the socket connection.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Triggers periodic DHCP functions.
 *
 * This function handles retransmissions and lease time expirations. The
 * TcpIp_DhcpV4_mainFunction() shall be called after the TcpIp_IpAddrM_mainFunction()
 * to ensure that IP Address changes are handled in the same TcpIp_MainFunction context.
 *
 * \param[in] timerTick  current time in ticks of TcpIp timer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_mainFunction
(
  uint16 timerTick
);

/** \brief get Ip configuration of the DHCP assignment
 *
 * This API function outputs the IP configuration of the DHCP assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] assignedIpPtrPtr Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the DHCP assignment
 *
 * This API function outputs the IP address of the DHCP assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] assignedIpPtrPtr Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) assignedIpPtrPtr
);

/** \brief get assignment status of the DHCP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId     Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4_isAssigned
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief inform dhcp whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_assignmentActive
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
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

#if  (TCPIP_DHCPGETSTATUS_API == STD_ON)
/** \brief Read DHCP status
 *
 * This functions reads the state of a specific DHCP address.
 *
 * \param[in]  LocalAddressId    Address Id for the DHCP address
 * \param[in]  StatusPtr         Pointer to a status variable. Contains the state information.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_getStatus
(
  TcpIp_LocalAddrIdType LocalAddressId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) StatusPtr
);
#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_DHCPV4_INT_H */
