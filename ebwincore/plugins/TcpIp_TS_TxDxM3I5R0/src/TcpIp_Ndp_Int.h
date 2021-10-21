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

#ifndef TCPIP_NDP_INT_H
#define TCPIP_NDP_INT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Ndp_Api.h>        /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_Ip_Int.h>         /* SW-Unit IP interface definitions */
#include <TcpIp_IpAddrM_Int.h>    /* used SW-Unit interface definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

typedef struct
{
  /*  TcpIp_Ip_CopyDataFunctorType polymorphic base class */
  TCPIP_IP_COPYDATA_FIELDS

  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIpPtr;         /* pointer to the target address */
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkLayerAddressPtr;  /* target link layer address pointer */
  uint8 flags;      /* Ndp header flags */
  uint8 type;       /* type of Ndp message */

} TcpIp_Ndp_CopyDataFunctorType;

#if (TCPIP_IPV6_ENABLED == STD_ON)

/** \brief Duplicate address detection result indication - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_Ndp_dadResult_FctPtrType)
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
);

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Retrieve physical address from remote host in NDP cache
 *
 * This service queries the IP/physical address translation table specified by CtrlIdx
 * and returns the physical address related to the IP address  specified by IpAddrPtr.
 *
 * \param[in]  CtrlIdx      EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  IpAddrPtr    specifies the IP address for which the physical address shall be retrieved
 * \param[out] PhysAddrPtr  Pointer to the memory where the physical address (MAC  address)
 * \param[in]  initRes      specifies if the address resolution shall be initiated (TRUE) or not
 *                          (FALSE) in case the physical address related to the specified IP
 *                          address is currently unknown.
 *
 * \return Result of operation
 * \retval TCPIP_E_OK              specified IP address resolved, physical address provided via
 *                                 PhysAddrPtr
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address currently unknown (address resolution initiated
 *                                 if initRes set to TRUE)
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_getRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
);

/** \brief Set physical address of remote host in NDP cache
 *
 * This service adds, updates or removes a physical address from a remote host in the NDP table
 *
 * \param[in]  CtrlIdx            EthIf controller index to identify the related /NDP table.
 * \param[in]  IpRemoteAddrPtr    specifies the IP address for which the physical address shall be
 *                                set in the NDP table
 * \param[in]  PhysAddrPtr        specifies the physical address which shall be set in the NDP table
 * \param[in]  State              TCPIP_PHYS_ADDR_ENTRY_STATE_FREE: remove the specified entry in the
 *                                NDP table
 *                                TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: add/update the specified
 *                                entry in the NDP table
 *                                TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: set the specified entry in
 *                                the NDP table to static (entry will remain in the cache until
 *                                it is deleted with TcpIp_SetRemotePhysAddr or TcpIp is reinitialized)
 *
 * \return Result of operation
 * \retval TCPIP_OK                physical address successfully added/updated/removed
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address currently unknown (in case the entry shall be
 *                                 removed)
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_setRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief Initializes SW-Unit Ndp
 *
 * This API service performs the initialization of the NDP cache related data structures.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_init(void);

/** \brief Reception of NDP message
 *
 * This API service indicates the reception of an NDP message.
 *
 * \param[in] type              Icmp message Type
 * \param[in] code              Icmp Error Code
 * \param[in] remoteAddrPtr     IP address of the remote host.
 * \param[in] localAddrId       Local address the frame has been received on.
 * \param[in] dataPtr           Points to the received data. The data contains the payload of the
 *                              Ip layer (which includes the Icmp header).
 * \param[in] lenByte           Length of received data in bytes.
 * \param[in] HopLimit          hop limit of the received icmp message
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE ) TcpIp_Ndp_rxIndication
(
  uint8 type,
  uint8 code,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  uint8 HopLimit
);

/** \brief Time based scheduling of NDP cache functionality
 *
 * \param[in] timerTick     current time in ticks of TcpIp timer
 *
 * This API service performs the time-based tasks of the NDP cache functionality.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_mainFunction
(
  uint16 timerTick
);

/** \brief Searches for an existing NDP entry, returns an identification handle.
 *
 * This function searches the NDP table for an entry matching the given IpV6-address.
 * A handle identifying the entry is returned.
 *
 * \param[in] ctrlIdx               Index of the EthIf controller
 * \param[in] ipAddr                IpV6 address to lookup
 * \param[in] remoteAddrHandlePtr   Pointer the NDP entry handle is written to
 * \param[in] createNewEntry        TRUE create new entry if none were found
 *                                  FALSE do not create new entry if none were found
 *
 * \retval TCPIP_OK                handle is valid
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address not present in the cache
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_lookupAndLock
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  boolean createNewEntry
);

/** \brief Unlocks an NDP entry.
 *
 * This function unlocks the NDP entry identified by the given handle.
 *
 * \param[in] remoteAddrHandle      ARP entry handle
 * \param[in] ctrlIdx               Index of the EthIf controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE ) TcpIp_Ndp_NCache_unlock
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

/** \brief Returns a pointer to the physical address represented by given remote address handle(NDP).
 *
 * \param[in] remoteAddrHandle      NDP entry handle
 * \param[in] ctrlIdx               Index of the EthIf controller
 *
 * \return Result Pointer to physical address
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Ndp_NCache_getPhysAddrPtr
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

#if (TCPIP_NDP_RPD_ENABLED == STD_ON)

/** \brief Initializes SW-Unit Ndp Rpd
 *
 * This API service performs the initialization of the router and prefix discovery
 * related data structures.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_init
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Request Prefix assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Releases Prefix assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
);



/** \brief get Ip configuration of the NDP RPD assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the NDP RPD assignment
 *
 * \param[in]  localAddrId    Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief inform NDP RPD whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state
);

/** \brief Request assignment of IP address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] LocalIpAddrPtr  Pointer to requested IP
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TCPIP_NDP_RPD_STATE_PREFERRED   static ip address is preferred
 * \retval TCPIP_NDP_RPD_STATE_DEPRECATED  static ip address is deprecated
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpAddrState
(
    TcpIp_LocalAddrIdType localAddrId
);

/** \brief Function to get pointer of the address of the router list entry.
 *
 * This function returns pointer of the address of the router list entry.
 *
 * \param[in]  gatewayIpPtrPtr        Pointer to the ipv6 address in router list cache
 * \param[in]  ctrlIdx                Index of the EthIf controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_getDefaultRouterAddress
(
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  uint8 ctrlIdx
);


#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
/** \brief Unlocks a destination cache entry.
 *
 * This function unlocks the destination cache entry identified by the given IP address.
 *
 * \param[in] destCacheHandlePtr   Idx of the entry in destination cache
 * \param[in] ctrlIdx              Index of the ethernet controller (EthIfCtrlIdx)
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_unlock
(
  uint16 destCacheHandlePtr,
  uint8 ctrlIdx
);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
/** \brief Retrieve address of the next hop
 *
 * This function looks into the destination cache to get the next-hop address. If the address
 * is on-link (prefix match) the next-hop address is the same as the packet's destination address
 * otherwise the packet is sent to one of the default routers.
 *
 * \param[in]  remoteAddrPtr        IP address of the remote host
 * \param[in]  localIpAddrPtr       IP address of the local host
 * \param[in]  ctrlIdx              Index of the ethernet controller (EthIfCtrlIdx)
 * \param[out] destCacheHandlePtr   Pointer to the index in the destination cache
 * \param[out] nextHopIpPtr         Pointer to the next hop address
 * \param[in]  networkMaskPtr       Pointer to the netmask
 *
 * \return Result of operation
 * \retval TCPIP_OK                Next hop could be determined
 * \retval TCPIP_E_NOT_OK          Next hop couldn't be determined
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_DestCache_getNextHopAddrAndLock
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  uint8 ctrlIdx,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) destCacheHandlePtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) nextHopIpPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) networkMaskPtr
);

#if (TCPIP_TCP_ENABLED == STD_ON)
/** \brief Update cache of the remote host
 *
 * This service updates the cache entry and resets reachability timer if a remote host
 * acknowledged the reception of TCP data.
 *
 * \param[in] CtrlIdx               Index of the EthIf controller
 * \param[in] remoteAddrPtr         ip address of the remote host
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);
#endif /* TCPIP_TCP_ENABLED == STD_ON */

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

/** \brief Requests start of probing
 *
 * Delays transmission of first DAD solicitation. Delay is used whether it is enabled and whether
 * DAD is configured as non optimistic.
 *
 * \param[in]  ndpDadResultFctPtr  Pointer to a unit specific function requesting the DAD.
 * \param[in]  ipAddress           Probed address
 * \param[in]  localAddrId         Identifier of the probed address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_start
(
  TcpIp_Ndp_dadResult_FctPtrType ndpDadResultFctPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddress,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Abort duplicate address detection process
 *
 * \param[in] localAddrId           Controller on which DAD was initiated
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_abort
(
  uint8 localAddrId
);

#endif /* TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED */

/** \brief builds Ndp header
 *
 * \param[in] that          virtual Base class that contains data needed for building Ndp header
 * \param[in] bufferPtr     pointer to the location in buffer where Ndp header is located
 * \param[in] copyLength    length to copy
 *
 * \retval BUFREQ_OK        operation successful
 */
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Ndp_copyData
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  uint16 copyLength
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_IPV6_INT_H */
