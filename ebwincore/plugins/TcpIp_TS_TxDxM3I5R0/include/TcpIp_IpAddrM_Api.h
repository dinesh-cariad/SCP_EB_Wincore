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
#ifndef TCPIP_IPADDRM_API_H
#define TCPIP_IPADDRM_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */


#include <TcpIp_IpAddrM_Api.guards>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief TCP/IP stack state for a specific EthIf controller is ONLINE, i.e. communication
 * via at least one IP address is possible. */
#define TCPIP_STATE_ONLINE 0U
/** \brief TCP/IP stack state for a specific EthIf controller is ONHOLD, i.e. no communication
 * is currently possible (e.g. link down) */
#define TCPIP_STATE_ONHOLD 1U
/** \brief TCP/IP stack state for a specific EthIf controller is OFFLINE, i.e. no
 * communication is possible. */
#define TCPIP_STATE_OFFLINE 2U
/** \brief TCP/IP stack state for a specific EthIf controller is STARTUP, i.e. IP
 * address assignment in progress or ready for manual start, communication is
 * currently not possible. */
#define TCPIP_STATE_STARTUP 3U
/** \brief TCP/IP stack state for a specific EthIf controller is SHUTDOWN, i.e.
 * release of resources using the EthIf controller, release of IP address
 * assignment. */
#define TCPIP_STATE_SHUTDOWN 4U

/** \brief Specifies the TcpIp state for a specific EthIf controller. */
typedef uint8 TcpIp_StateType;


/** \brief IP address assignment in use */
#define TCPIP_IPADDR_STATE_ASSIGNED 0U
/** \brief IP address assignment in use, but link lost */
#define TCPIP_IPADDR_STATE_ONHOLD 1U
/** \brief IP address assignment unused */
#define TCPIP_IPADDR_STATE_UNASSIGNED 2U

/** \brief Specifies the state for a specific IP address assignment. */
typedef uint8 TcpIp_IpAddrStateType;

/* Autosar assignment types */
/** \brief Static IPv4 address assignment */
#define TCPIP_IPADDR_ASSIGNMENT_STATIC         0U
/** \brief address obtained via LINK-LOCAL */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL      1U
/** \brief address obtained via LINK-LOCAL or DO-IP */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP 2U
/** \brief address obtained via DHCP */
#define TCPIP_IPADDR_ASSIGNMENT_DHCP           3U
/** \brief address obtained via IPV6 ROUTER */
#define TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER    4U
/** \brief all configured methods with manual trigger */
#define TCPIP_IPADDR_ASSIGNMENT_ALL            5U

/** \brief Specification of IP address assignment policy */
typedef uint8 TcpIp_IpAddrAssignmentType;

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


/** \brief Request to change state of communication network
 *
 * By this API service the TCP/IP stack is requested to change the TcpIp state of
 * the communication network identified by EthIf controller index.
 *
 * \param[in] CtrlIdx EthIf controller index to identify the communication network
 *                    where the TcpIp state is requested.
 * \param[in] State   Requested TcpIp state.
 *
 * \return Result of operation
 * \retval E_OK      Service accepted
 * \retval E_NOT_OK  Service denied
 *
 * \ServiceID{0x09}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestComMode
(
  uint8 CtrlIdx,
  TcpIp_StateType State
);

/** \brief Request an IP address assignment
 *
 * By this API service the local IP address assignment type for the IP address
 * specified by LocalAddrId shall be requested.
 *
 * \param[in] LocalAddrId   IP address index specifying the IP address for which an
 *                          assignment shall be requested.
 * \param[in] Type           Type of IP address assignment which shall be initiated.
 * \param[in] LocalIpAddrPtr  Pointer to structure containing the IP address which shall be
 *                           assigned to the EthIf controller indirectly specified via
 *                           LocalAddrId. Note: This parameter is only used in case the
 *                           parameters Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 *
 * \ServiceID{0x0a}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType Type,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Release an IP address assignment
 *
 * By this API service the local IP address assignment for the IP address
 * specified by LocalAddrId shall be released.
 *
 * \param[in] LocalAddrId IP address index specifying the IP address for which an
 *                        assignment shall be release.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 *
 * \ServiceID{0x0b}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId
);

/** \brief Obtain local IP address
 *
 * Obtains the local IP address actually used by LocalAddrId.
 *
 * \param[in] LocalAddrId Local address identifier referring to the local IP
 *                        address which shall be obtained.
 * \param[out] IpAddrPtr Pointer to a struct where the IP address is stored.
 *                       Struct members not related to the IP address are of arbitrary
 *                       value and shall not be used.
 * \param[out] NetmaskPtr Pointer to a struct where the netmask is stored.
 *                        Struct members not related to the IP address are of arbitrary
 *                        value and shall not be used.
 * \param[out] DefaultRouterPtr Pointer to a struct where the Gateway IP address is stored.
 *                              Struct members not related to the IP address are of arbitrary
 *                              value and shall not be used.
 *
 * \return Result of operation
 * \retval E_OK      The request was successful
 * \retval E_NOT_OK  The request was not successful
 *
 * \ServiceID{0x10}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddr
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
);

/** \brief Obtain local physical address
 *
 * Obtains the physical source address used by the EthIf controller
 * implicitly specified via LocalAddrId.
 *
 * \param[in] LocalAddrId Local address identifier implicitly specifying the
 *                        EthIf controller for which the physical address shall be obtained.
 * \param[out] PhysAddrPtr Pointer to the memory where the physical source
 *                         address (MAC address) in network byte order is stored
 *
 * \return Result of operation
 * \retval E_OK      The request was successful
 * \retval E_NOT_OK  The request was not successful, e.g. no unique Ctrl specified via IpAddrId.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetPhysAddr
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr
);

/** \brief Obtain controller index
 *
 * Obtains the index of the controller related to LocalAddrId.
 *
 * \param[in]  LocalAddrId  Local address identifier implicitly specifying the EthIf controller
 *                          that shall be returned
 * \param[out] CtrlIdxPtr   Pointer to the memory where the index of the controller related to
 *                          LocalAddrId is stored
 *
 * \return Result of operation
 * \retval E_OK      the request was successful
 * \retval E_NOT_OK  the request was not successful.
 *
 * \ServiceID{0x17}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetCtrlIdx
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) CtrlIdxPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_IPADDRM_API_H */
/*==================[end of file]===========================================*/
