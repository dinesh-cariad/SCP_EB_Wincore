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

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Usage of concatenation operator improves readability of code.
 */

#ifndef TCPIP_IPADDRM_INT_H
#define TCPIP_IPADDRM_INT_H

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Api.h>   /* API declarations implemented by this SW-unit */
#include <TcpIp_Int.h>            /* module internal headerfile */

/*==================[macros]================================================*/

/** \brief Extract address type (UNICAST, MULTICAST) from address information (TcpIp_IpAddrM_AddrInfoType) */
#define TCPIP_IPADDRM_ADDRTYPE_GET(addrinfo) ((addrinfo)&0x01U)

/** \brief Checks address information for a concrete address type (UNICAST, MULTICAST, BROADCAST) */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPADDRM_IS_ADDRTYPE(addressinfo,type)\
  (TCPIP_IPADDRM_ADDRTYPE_GET(addressinfo) == TCPIP_IPADDRM_ADDRTYPE_##type)

/** \brief Extract address domain (IPV4, IPV6) from address information (TcpIp_IpAddrM_AddrInfoType) */
#define TCPIP_IPADDRM_ADDRDOMAIN_GET(addrinfo) (((addrinfo) >> 7u) & 1u)

/** \brief Checks address information for a concrete address domain (IPV4, IPV6) */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPADDRM_IS_ADDRDOMAIN(addressinfo,domain)\
  (TCPIP_IPADDRM_ADDRDOMAIN_GET(addressinfo) == TCPIP_IPADDRM_ADDRDOMAIN_##domain)

/** \brief Returns the ethernet interface index (EthIfCtrlIdx) the given local address Id is assigned to */
#define TcpIp_IpAddrM_getCtrlIdx(localAddrId) \
  ((TCPIP_CFG(localAddr,localAddrId)).ctrlIdx)

/** \brief Returns the assignment method index (0-based and dense) for the given local address Id /assignment method */
#define TcpIp_IpAddrM_getAssignmentIdx(localAddrId,method) \
  ((TCPIP_CFG(localAddr,localAddrId)).assignmByMethod[(method)].assignIdx)

/** \brief Returns the local address Id for the given assignment method index /assignment method */
#ifdef TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS

#define TcpIp_IpAddrM_getLocalAddrId(assignmentIdx,method) \
  TcpIp_IpAddrM_getLocalAddrId_Tmp(assignmentIdx,method)

#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_0U IpV4StaticIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_1U IpV4LinkLocalIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_2U IpV4LinkLocalIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_3U IpV4DhcpIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_4U IpV6StaticIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_5U IpV6StaticIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_6U IpV6DhcpIndex2LocalAddrId
#define TCPIP_IPADDRM_ASSIGNMENT_METHOD_7U IpV6NdpRpdIndex2LocalAddrId

#define TcpIp_IpAddrM_getLocalAddrId_Tmp(assignmentIdx,method) \
  (TcpIp_ConfigLayout.TCPIP_IPADDRM_ASSIGNMENT_METHOD_##method[assignmentIdx])

#else /* TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS */

#define TcpIp_IpAddrM_getLocalAddrId(assignmentIdx,method) \
  (TS_UNCHECKEDGETCFG(TcpIp_PBcfgPtr,TcpIp_LocalAddrIdType,TCPIP,TcpIp_PBcfgPtr->assignmToLocalAddrIdMapRef[(method)])[(assignmentIdx)])

#endif /* TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS */

/** \brief Returns whether the assignment method is invalid for given local address Id /assignment method */
#define TcpIp_IpAddrM_isAssignmentInvalid(localAddrId,method) \
  (((TCPIP_CFG(localAddr,localAddrId)).assignmByMethod[(method)].priority_autoAssign) == 0xFFU)

/** \brief Invalid local IP address
 *
 * This macro defines a value representing an invalid local IP address.
 */
#define TCPIP_LOCALADDRID_INVALID        255U

/** \brief Invalid EthIfCtrl index
 *
 * This macro defines a value representing an invalid EthIfCtrl index.
 */
#define TCPIP_CTRLIDX_INVALID            255U

/* Internal types assignment types */
/* IPv4 */
#define TCPIP_IPADDRM_ASSIGN_STATICV4         TCPIP_IPADDR_ASSIGNMENT_STATIC
#define TCPIP_IPADDRM_ASSIGN_LINKLOCALV4      TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL
#define TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP   TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP
#define TCPIP_IPADDRM_ASSIGN_DHCPV4           TCPIP_IPADDR_ASSIGNMENT_DHCP
/* IPv6 */
#define TCPIP_IPADDRM_ASSIGN_STATICV6         4U
#define TCPIP_IPADDRM_ASSIGN_LINKLOCALV6      5U
#define TCPIP_IPADDRM_ASSIGN_DHCPV6           6U
#define TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER      7U
/** \brief number of assignment methods */
#define TCPIP_IPADDRM_ASSIGN_NUM              (TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER + 1u)

/** \brief invalid assignment type ID */
#define TCPIP_IPADDRM_ASSIGN_INVALID          255U

/** \brief represents an address of type unicast - to be used for TcpIp_IpAddrM_AddressType */
#define TCPIP_IPADDRM_ADDRTYPE_UNICAST 0x00U
/** \brief represents an address of type multicast - to be used for TcpIp_IpAddrM_AddressType */
#define TCPIP_IPADDRM_ADDRTYPE_MULTICAST 0x01U

/** \brief represents an address of IPV4 domain - to be used for TcpIp_IpAddrM_AddressType */
#define TCPIP_IPADDRM_ADDRDOMAIN_IPV4 0x00U
/** \brief represents an address of IPV6 domain- to be used for TcpIp_IpAddrM_AddressType */
#define TCPIP_IPADDRM_ADDRDOMAIN_IPV6 0x01U

/** \brief indicates that the ip address is in state preferred */
#define TCPIP_IPADDRM_ADDRSTATE_PREFERRED                                 1U
/** \brief indicates that the ip address is in state deprecated */
#define TCPIP_IPADDRM_ADDRSTATE_DEPRECATED                                2U
/** \brief indicates that the ip address is in state tentative  */
#define TCPIP_IPADDRM_ADDRSTATE_TENTATIVE                                 3U
/** \brief indicates that the ip address is in state optimistic */
#define TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC                                4U
/** \brief indicates that the ip address is in state conflicted */
#define TCPIP_IPADDRM_ADDRSTATE_CONFLICTED                                5U
/** \brief indicates that the ip address is not assigned */
#define TCPIP_IPADDRM_ADDRSTATE_INVALID                                 255U

/*==================[type definitions]======================================*/

/** \brief contains the address type/domain information for a localAddress */
typedef uint8 TcpIp_IpAddrM_AddrInfoType;


/** \brief pointer type to constant ip address */
typedef P2CONST(uint32,AUTOMATIC,TCPIP_APPL_DATA) TcpIp_IpAddrConstPtr;

/** \brief assignment method - init - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_init_FctPtrType)(TcpIp_LocalAddrIdType localAddrId);

/** \brief assignment method - startAssign - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_startAssign_FctPtrType)(TcpIp_LocalAddrIdType localAddrId);

/** \brief assignment method - stopAssign - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_stopAssign_FctPtrType)(TcpIp_LocalAddrIdType localAddrId);

/** \brief assignment method - getIpConfig - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_getIpConfig_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief assignment method - getIpAddress - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_getIpAddress_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief assignment method - isAssigned - function pointer type */
typedef P2FUNC(boolean, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_isAssigned_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId
);

/** \brief assignment method - assignmentActive - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_assignmentActive_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state
);

/** \brief assignment method - requestIpAddress - function pointer type */
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_requestIpAddress_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief assignment method - getIpAddrState - function pointer type */
typedef P2FUNC(uint8, TCPIP_CODE, TcpIp_IpAddrM_AssignSM_getIpAddrState_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddrId
);

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/*-------------------[SW-unit interface function declarations]---------------*/


/** \brief Initializes IpAddrM data structures
 *
 * This function initializes the data structures of SW-unit IpaddrM.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_init(void);

/** \brief Performs periodic IpAddrM tasks
 *
 * This function performs periodic tasks of SW-unit IpAddrM (state machine handling).
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction(void);



#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

/** \brief returns whether the local address is a concrete (not wildcard like ANY) one
 *
 * \param[in] localAddrId         local address identifier
 *
 * \return
 * \retval TRUE   this is a concrete local address
 * \retval FALSE  this is not a concrete local address
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_checkConcreteLocalAddr
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Check if ip address is valid
 *
 * \param[in] ipAddrPtr  ip address of remote host
 * \param[in] addrInfo        domain of local address
 *
 * \return
 * \retval E_OK      domain of ip address is valid for local address
 * \retval E_NOT_OK  domain of ip address is invalid for local address
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_checkIpAddr
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  TcpIp_IpAddrM_AddrInfoType addrInfo
);

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */



/** \brief AssignSM trigger function: an address has been unassigned
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_unassigned
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
);


/** \brief AssignSM trigger function: an address has been assigned successfully
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_assigned
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
);



/** \brief Returns the state of the interface of the requested local address
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Interface state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
);

#if (TCPIP_IPV6_ENABLED == STD_ON)

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TCPIP_IPADDRM_ADDRSTATE_PREFERRED  ip address is preferred
 * \retval TCPIP_IPADDRM_ADDRSTATE_DEPRECATED ip address is deprecated
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_IpAddrM_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/** \brief Returns information of the given local address id
 *
 * Returns the type of address of the given local address in bit 0.
 * Possible values are:
 * - TCPIP_IPADDRM_ADDRESSTYPE_UNICAST
 * - TCPIP_IPADDRM_ADDRESSTYPE_MULTICAST
 *
 * Returns the address domain of the given local address in bit 7.
 * Possible values are:
 * - TCPIP_IPADDRM_ADDRESSDOMAIN_IPV4
 * - TCPIP_IPADDRM_ADDRESSDOMAIN_IPV6

 * \param[in] localAddrId     Id of local IP address
 *
 * \return Information of local address Id
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_IpAddrM_AddrInfoType, TCPIP_CODE) TcpIp_IpAddrM_getLocalAddrInfo
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Tests whether the requested interface is online and locks it in the current state
 *
 * This API function tests if the interface (If) associated with the given LocalAddrId is in state
 * TCPIP_STATE_ONLINE.
 * If the If is in state TCPIP_STATE_ONLINE then this function locks the interface in this state and returns TRUE.
 * Otherwise it returns false FALSE.
 *
 * \note
 * The lock is released by calling function TcpIp_IpAddrM_releaseIf().
 * Interleaved locking is supported. Thus locking is performed with the
 * first request, and finally released with the last call to TcpIp_IpAddrM_releaseIf().
 *
 * \see TcpIp_IpAddrM_releaseIf
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TRUE     Interface is in expected state and has been locked against state changes
 * \retval FALSE    Interface is not in expected state
 *
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfOnline
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Tests whether the requested interface is enabled and locks it in the current state
 *
 * This API function tests if the interface (If) associated with the given LocalAddrId is in state
 * TCPIP_STATE_ONLINE, TCPIP_STATE_SHUTDOWN.
 * If the If is in expected state, then this function locks the interface in this state and returns TRUE.
 * Otherwise it returns false FALSE.
 *
 * \note
 * The lock is released by calling function TcpIp_IpAddrM_unlockIf().
 * Interleaved locking is supported. Thus locking is performed with the
 * first request, and finally released with the last call to TcpIp_IpAddrM_unlockIf().
 *
 * \see TcpIp_IpAddrM_unlockIf
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TRUE     Interface is in expected state and has been locked against state changes
 * \retval FALSE    Interface is not in expected state
 *
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfOnlineShutdown
(
  TcpIp_LocalAddrIdType localAddrId
);

#if (TCPIP_IPV6_ENABLED == STD_ON)

/** \brief Tests whether the requested interface is enabled and locks it in the current state
 *
 * This API function tests if the interface (If) associated with the given LocalAddrId is in state
 * TCPIP_STATE_ONLINE or TCPIP_STATE_STARTUP.
 * If the If is in expected state, then this function locks the interface in this state and returns TRUE.
 * Otherwise it returns false FALSE.
 *
 * \note
 * The lock is released by calling function TcpIp_IpAddrM_unlockIf().
 * Interleaved locking is supported. Thus locking is performed with the
 * first request, and finally released with the last call to TcpIp_IpAddrM_unlockIf().
 *
 * \see TcpIp_IpAddrM_unlockIf
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TRUE     Interface is in expected state and has been locked against state changes
 * \retval FALSE    Interface is not in expected state
 *
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfStartupOnline
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) || \
    (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_NDP_RPD_ENABLED == STD_ON)
/** \brief Tests whether the requested interface is enabled and locks it in the current state
 *
 * This API function tests if the interface (If) associated with the given LocalAddrId is in state
 * TCPIP_STATE_ONLINE, TCPIP_STATE_STARTUP or TCPIP_STATE_SHUTDOWN.
 * If the If is in expected state, then this function locks the interface in this state and returns TRUE.
 * Otherwise it returns false FALSE.
 *
 * \note
 * The lock is released by calling function TcpIp_IpAddrM_unlockIf().
 * Interleaved locking is supported. Thus locking is performed with the
 * first request, and finally released with the last call to TcpIp_IpAddrM_unlockIf().
 *
 * \see TcpIp_IpAddrM_unlockIf
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TRUE     Interface is in expected state and has been locked against state changes
 * \retval FALSE    Interface is not in expected state
 *
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfStartupOnlineShutdown
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) ||
          (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_NDP_RPD_ENABLED == STD_ON) */



/** \brief Unlocks the state change lock on the requested interface
 *
 * This API function removes the lock on the interface (If) associated with the given LocalAddrId.
 * If state changes have been requested during the lock, those requests shall be initiated during this call.
 *
 * \param[in] localAddrId     Id of local IP address
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_unlockIf
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief get Ip configuration of local address
 *
 * This API function outputs the IP configuration of a local address
 *
 * \param[in] localAddrId       Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);



/** \brief get Ip address of local address
 *
 * This API function outputs the IP address of a local address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief Request an IP address assignment
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
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_requestIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType Type,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Release an IP address assignment
 *
 * \param[in] LocalAddrId IP address index specifying the IP address for which an
 *                        assignment shall be released.
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 *
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_releaseIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId
);

#if (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)

/** \brief returns the ANY configured local address Id of an interface the requested
 *
 * This API functions looks up a local address Id which is configured to ANY and assigned to the
 * same ethernet interface (EthIfCtrlIdx)
 * and domain(IPV4, IPV6) as the given local address Id. If no such a configured ANY local address
 * is found, the original localAddrId
 * passed is returned.
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return configured ANY local address Id
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpAddrM_getAnyIpLocalAddrId
(
  TcpIp_LocalAddrIdType localAddrId
);


/** \brief LocalAddrSM trigger function: sockets bound to the localAddrId have been closed
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_socketsClosed
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */


#if ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON))
/** \brief loops over all local addresses configured for the controller and
 *          returns the local address on which the requested assignment method is configured
 *
 * \param[in] LocalAddrId     Id of local IP address
 * \param[in] method          assignment method
 *
 * \return if the requested local address assignment is not configured on the
 *         controller the passed local address is returned
 */
TS_MOD_PRIV_DECL FUNC (TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpAddrM_getLocalAddrOfAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType method
);
#endif /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_IPADDRM_INT_H */
/*==================[end of file]===========================================*/
