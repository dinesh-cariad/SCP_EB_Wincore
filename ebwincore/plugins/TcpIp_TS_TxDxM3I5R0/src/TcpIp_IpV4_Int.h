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
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 */
#ifndef TCPIP_IPV4_INT_H
#define TCPIP_IPV4_INT_H

/*==================[inclusions]============================================*/
#include <TcpIp_IpV4_Api.h>       /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_Ip_Int.h>         /* SW-Unit IP interface definitions */
#include <TcpIp_IpAddrM_Int.h>    /* own SW-unit interface file */

/*==================[macros]================================================*/

/*
 * see rfc792
 */
/** \brief ICMP echo reply code */
#define TCPIP_IPV4_ICMP_TYPE_ECHO_REPLY 0U
/** \brief ICMP destination unreachable code */
#define TCPIP_IPV4_ICMP_TYPE_DEST_UNREACH 3U
/** \brief ICMP echo code */
#define TCPIP_IPV4_ICMP_TYPE_ECHO 8U
/** \brief Length of ICMP header */
#define TCPIP_IPV4_ICMP_HEADER_LENGTH 4U
/** \brief IPV4 address length in 32 bit units */
#define TCPIP_IPV4_ADDR_SIZE                   1U
/** \brief IPV4 address length in units of bytes */
#define TCPIP_IPV4_ADDR_BYTE_SIZE              (4U*TCPIP_IPV4_ADDR_SIZE)

/* Don't fragment flag 0b 0x00 0000 0000 0000 */
#define TCPIP_IPV4_FLAG_DONT_FRAGMENT           0x4000U

/* Don't fragment flag 0b 00x0 0000 0000 0000 */
#define TCPIP_IPV4_FLAG_MORE_FRAGMENTS          0x2000U

/** \brief specific value for an ARP handle representing a broadcast physical address */
#define TCPIP_IPV4_ARP_REMOTE_HANDLE_BROADCAST 0xFFFFU
/** \brief specific value for an ARP handle representing a multicast physical address */
#define TCPIP_IPV4_ARP_REMOTE_HANDLE_MULTICAST 0xFFFEU

#define TCPIP_IPV4_ARP_HANDLE_IS_LOOKUP(handle) ((handle) < TCPIP_IPV4_ARP_REMOTE_HANDLE_MULTICAST)

/** \brief macro converting a multicast IP address to a multicast ethernet broadcast address */
#define TcpIp_IpV4_GetPhysMulticastAddr(physAddrPtr,remoteAddrPtr) \
    do { \
      /* parameter type check & safe macro */ \
      P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddressPtr= (physAddrPtr); \
      /* parameter type check & safe macro */ \
      P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remoteAddressPtr= (remoteAddrPtr);  \
      physAddressPtr[0]= 0x01U; \
      physAddressPtr[1]= 0x00U; \
      physAddressPtr[2]= 0x5EU; \
      physAddressPtr[3]= (0x7FU&remoteAddressPtr[1U]); \
      physAddressPtr[4]= remoteAddressPtr[2U]; \
      physAddressPtr[5]= remoteAddressPtr[3U]; \
    } while(0) \

/* specific IPV4 address values / masks */
/** \brief limited IPV4 broadcast address */
#define TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE  0xFFFFFFFFU
/** \brief IPV4 Multicast identification mask */
#define TCPIP_IPV4_ADDR_MULTICAST_MASK           0xF0000000U
/** \brief IPV4 Multicast identification value */
#define TCPIP_IPV4_ADDR_MULTICAST_VALUE          0xE0000000U
/** \brief IPV4 ANY-IP value */
#define TCPIP_IPV4_ADDR_ANY_VALUE                0x00000000U
/** \brief IPV4 invalid value (ANY IP may be used only for very rare cases, e.g. DHCP) */
#define TCPIP_IPV4_ADDR_INVALID_VALUE            0x00000000U

/** \brief IPV4 link local mask  */
#define TCPIP_IPV4_LINKLOCAL_MASK                0xFFFF0000U
/** \brief IPV4 link local network Id  */
#define TCPIP_IPV4_NETWORKID_LINKLOCAL_VALUE     0xA9FE0000U

/** \brief Address Conflict Detection is deactivated */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF                        0U
/** \brief Abandon the assigned local address if conflict is detected */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_ABANDON                    1U
/** \brief Defend the assigned local address, if 2 conflicts are detected within
           defend interval, abandon address */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND                     2U
/** \brief Defend the assigned local address endlessly if conflict is detected */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND_INF                 3U

/** \brief macro evaluates to true if and only if ipAddr is the IPv4 broadcast
 *  address, 255.255.255.255 */
#define TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(ipAddr)                        \
  ( (ipAddr) == COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE) )

/** \brief macro evaluates to true if and only if ipAddr is an IPv4 multicast
 *  address, i.e., in the range 224.0.0.0 to 239.255.255.255 */
#define TCPIP_IPV4_IS_MULTICAST_ADDR(ipAddr)                                \
  ( ((ipAddr)& COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_MASK)) ==  \
               COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_VALUE)     \
  )

/** \brief macro to check if an ipV4 address is valid unicast address */
#define TCPIP_IPV4_ADDR_IS_NOT_VALID_UNICAST(ipAddr)     \
  (TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR((ipAddr)[0U])) ||\
  (TCPIP_IPV4_IS_MULTICAST_ADDR((ipAddr)[0U]))

/* Deviation MISRAC2012-1 <+1> */
#define TCPIP_GET_IPV4_FROM_SOCK_ADDR_TYPE(sockStruct)                         \
  (((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))sockStruct)->addr)

/** \brief macro evaluates to true if and only if srcIp and destIp are in the
 *  same subnet */
#define TCPIP_IPV4_IN_SAME_SUBNET(srcIp, destIp, netMask)           \
  ( ((srcIp)&(netMask)) == ((destIp)&(netMask)) )

/*==================[type definitions]======================================*/

/** \brief Arp probe result indication - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpV4_ArpProbeResult_FctPtrType)
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

/** \brief Address conflict indication - function pointer type */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_IpV4_AddrConflDetect_FctPtrType)
(
  TcpIp_LocalAddrIdType localAddrId
);

/*==================[external function declarations]========================*/

#if (TCPIP_IPV4_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/*-------------------[SW-unit interface function declarations]---------------*/

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )

/** \brief Returns the maximum fragment size in an IpV4
 *
 * \param[in] localAddrId     local address identifier
 *
 * \return maximum fragment size per IpV4 datagram in units of bytes
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_getFragmentSize
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */

/** \brief Returns the maximum payload possible in an IpV4 frame considering the actual MTU
 *
 * \param[in] ipSecSaCommonIdx      IPsec security association information index
 * \param[in] localAddrId           local address identifier
 * \param[out] maxPayloadPtr        The maximum IP payload size
 * \param[out] additionalHdrSizePtr The additional size of the AH header
 *
 * \return Result of operation
 * \retval E_OK      IPsec is disabled or security association is established and AH header
 *                   could be retrieved
 * \retval E_NOT_OK  Security association is not established, maximal length cannot be evaluated
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
);

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Returns the minimal supported payload in an IpV4 frame
 *
 * \return minimal supported payload length per IpV4 datagram in units of bytes
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_getMinimumPayload(void);

/** \brief This function does not perform any action.
 *
 * \param[in]  CtrlIdx       EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  remoteAddrPtr IP address of the remote host.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
/** \brief Evaluate new socket connection ipV4 address
 *
 * \param[in]  RemoteAddrPtr  IP address and port of the remote host to connect to.
 *
 * \retval TRUE           valid remote address
 * \retval FALSE          invalid remote address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Requests a Tx-buffer from Eth module
 *
 * TcpIp_IpV4_provideTxBuffer  allocates the TxBuffer by calling EthIf_ProvideTxBuffer
 * (LenBytePtr + IP-header), allocates the Ip-header and returns the remaining buffer.
 *
 * \param[in] ipParameterPtr      Points to a structure containing Ip communication parameters
 * \param[in,out] ethTxHandlePtr  Handle represents the data link layer buffer used for transmission.
 * \param[in] lenByte             Payload length
 * \param[in] localAddrId         Local address used for transmission
 * \param[in] ipFragment          Is it a fragment
 *
 * \return Result of operation
 * \retval TCPIP_OK               A buffer has of sufficient size has been provided
 * \retval TCPIP_E_NOT_OK         Requesting the buffer failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_provideTxBuffer
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 lenByte,
  TcpIp_LocalAddrIdType localAddrId,
  boolean ipFragment
);

/** \brief Find the destination address ARP entry handle
 *
 * \param[in,out] remoteAddrHandlePtr   Pointer to the ARP entry handle is written to
 * \param[in] destIpAddrPtr             destination address pointer
 * \param[in] localAddrId               local address id
 *
 *
 * \retval TCPIP_OK                 handle found
 * \retval TCPIP_E_PHYS_ADDR_MISS   address is unknown (no entry exists)
 * \retval TCPIP_E_NOT_OK           address resolution is still ongoing
 */

TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_getRemoteAddrHandle
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
);


/** \brief Performs IpV4 transmission
 *
 * This function constructs the Ip header and invokes the transmission of the Ip frame over EthIf.
 *
 * \param[in] ipParameterPtr          Points to a structure containing the Ip communication parameters.
 * \param[in] fragHdrPtr              All info related to length, IP fragmentation & IP header ID.
 * \param[in] ethTxHandlePtr          Handle represents the data link layer buffer used for transmission.
 * \param[in] remoteAddrHandle        Indicates remote link layer address index
 * \param[in] destIpPtr               Points to the remote IP address information.
 * \param[in] localAddrId             Local address used for transmission.
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_transmit
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Update upper layer protocol header
 *
 * This function calculates the Ip pseudo header checksum and adds it to the partial data checksum,
 * which was accumulated during the data copy operations.
 * It will then call the callback function provided via \a updateHeaderFunctorPtr, which will construct or
 * at least finalize the header of the upper layer protocol using the accumulated checksum and
 * additional data provided with the \a updateHeaderFunctorPtr functor. Additionally it will check if
 * frame shall be secured and adds the authentication header and calculates the ICV.
 *
 * \param[in] updateHeaderFunctorPtr  Points to the function used to build upper layer header.
 * \param[in,out] ipParameterPtr      Points to a structure containing the Ip communication parameters.
 * \param[in] destIpPtr               Points to the remote IP address information.
 * \param[in] ethTxHandlePtr          Points to the buffers containing upper layer data
 * \param[in] localAddrId             Local address used for transmission.
 * \param[in] payloadSum              Checksum of the upper layer payload
 * \param[in] ipPayloadLength         ip payload length
 * \param[in] frgCnt                  number of fragments to be transmitted, needed for ICV calculation
 *
 * \return Result of operation
 * \retval TCPIP_OK               Header successfully added
 * \retval TCPIP_E_NOT_OK         No security association available, mac generate faild
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 payloadSum,
  uint16 ipPayloadLength,
  uint16 frgCnt
);

/** \brief Obtain local IPv4 address
 *
 * Obtains the local IP address actually used by LocalAddrId.
 *
 * \param[in] LocalAddrId Local address identifier referring to the local IP
 *                        address which shall be obtained.
 * \param[out] IpAddrPtr  Pointer to a struct where the IP address is stored.
 *                        Struct members not related to the IP address are of arbitrary
 *                        value and shall not be used.
 * \param[out] NetmaskPtr Pointer to a struct where the netmask is stored.
 *                        Struct members not related to the IP address are of arbitrary
 *                        value and shall not be used.
 * \param[out] DefaultRouterPtr Pointer to a struct where the Gateway IP address is stored.
 *                              Struct members not related to the IP address are of arbitrary
 *                              value and shall not be used.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
);

/** \brief Returns a pointer to the physical address represented by given remote address handle(ARP).
 *
 * \param[in] remoteAddrHandle      ARP entry handle
 * \param[in] ctrlIdx               Index of the EthIf controller
 *
 * \return Result Pointer to physical address
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE ) TcpIp_IpV4_Arp_getPhysAddrPtr
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))

/** \brief Returns the LocalAddrId to use for transmission for a given remote IPv4 address
 *
 * This function returns the localAddrId of a unicast local address matching the the remote IpV4
 * address passed in remoteAddrPtr.
 * This function performs the LocalAddrId selection according to Figure 4: Source Address selection
 * for transmission,
 * columns 'w/o bind' or 'ANY'.
 * Note that at least always the DefaultSrcAddr is returned, even if there is no IpV4 address assigned at all.
 *
 * \param[in] remoteAddrPtr      Points to remote IP address structure
 *
 * \return LocalAddrId selected for transmission
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getTxLocalAddrId_byRemoteIp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */

/** \brief Returns the LocalAddrId to use for transmission for a given LocalAddrId
 *
 * This function returns the localAddrId of a unicast local address matching the the local address passed in LocalAddrId.
 * This function performs the LocalAddrId selection according to Figure 4: Source Address selection for transmission,
 * columns 'unicast', 'multicast', 'broadcast' or 'ANY_IP'.
 * Note that the unicast adress is returned, even if there is no IpV4 address assigned at all.
 *
 * \param[in] ctrlIdx           Ethernet Interface controller index
 * \param[in] remoteIpAddrPtr   Pointer to the remote address
 *
 * \return localAddrId selected for transmission
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr
);

/** \brief Returns the state of the interface of the requested local address
 *
 *         Returs the state of the interface addressed.
 *         Note that TCPIP_LOCALADDRID_ANY is always in state TCPIP_STATE_ONLINE,
 *           thus can be always bound.
 *         Note that the controller specific ANY address state is equivalent to the
 *           unicast address localAddrId state.
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Interface state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpV4_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
);

#if (TCPIP_ICMPV4_ENABLED == STD_ON)
/** \brief Performs Icmp (V4) transmission
 *
 *  This function constructs the Icmp header and payload and and invokes the transmission of the Icmp frame over EthIf.
 *  Please note that only the first 4 bytes of the ICMP header is constructed and the payload is put directly afterwards.
 *
 * \param[in] dataPtr Points to the received data. The data contains the payload of the Icmp message
 * \param[in] lenByte Length of received data.
 * \param[in] destIpPtr           remote IP address
 * \param[in] localAddrId       Local address used for transmission
 * \param[in] icmpParameterPtr  Contains icmp header informations, like ttl, type, code,
 *                              specific value
 * \param[in] truncate          Indicates if truncation of message is allowed if maximal
 *                              ipv4 payload size is exeeded
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
*/
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_icmpTransmit
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean truncate
);

/** \brief Sets ICMP header fields
 *
 * This function appends the ICMP header at the location specified by dataPtr.
 *
 * \param[in] that              The functor's 'this' pointer.
 * \param[in] payloadSum        Checksum of the upper layer payload and ip header
 * \param[in] headerPtr         Points to data buffer where the Udp header is located.
 * \param[in] calculateChecksum If true calculate the ICMP checksum
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_Icmp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
);

#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

/** \brief Reception of IpV4 datagram
 *
 * By this API service the IpV4 SW-Unit gets an indication of a received IpV4 frame.
 *
 * \param[in] ctrlIdx     Index of the EthIf controller
 * \param[in] isBroadcast Indicates if the target MAC address is a broadcast address
 * \param[in] physAddrPtr Points to MAC address of remote host (source MAC)
 * \param[in] dataPtrIn   Points to the received data. The data contains the payload of the
 *                        Ethernet protocol (which excludes the Ethernet header but includes headers of higher layers).
 * \param[in] lenByte     Length of received data.
 *
 * \return value indicating result of processing incoming IpV4 frame
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_rxIndicationIp
(
  uint8 ctrlIdx,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtrIn,
  uint16 lenByte
);



/** \brief Reception of ARP frame
 *
 * By this API service the IpV4 SW-Unit gets an indication of a received ARP frame.
 *
 * \param[in] ctrlIdx     Index of the EthIf controller
 * \param[in] physAddrPtr Points to MAC address of remote host (source MAC)
 * \param[in] dataPtr     Points to the received data. The data contains the payload of the
 *                        Ethernet protocol (which excludes the Ethernet header but includes headers of higher layers).
 * \param[in] lenByte     Length of received data.
 *
 * \return value indicating result of processing incoming ARP frame
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_rxIndicationArp
(
  uint8 ctrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);



/** \brief Time based scheduling of IpV4 functionality
 *
 * \param[in] timerTick     current time in ticks of TcpIp timer
 *
 * This API service performs the time-based tasks of the IpV4 functionality.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_mainFunction(uint16 timerTick);

/** \brief Initializes SW-Unit IpV4
 *
 * This API service performs the initialization of the IpV4 SW-Unit related data structures.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_init(void);

/** \brief Perform ARP announcing for an requested Ip address.
 *
 * \param[in] ipAddress             Ip address to announce in the network
 * \param[in] numAnnounce           number of announcements to transmit
 * \param[in] delay                 delay to transmit between announcements in [s]
 * \param[in] ctrlIdx               Index of the EthIf controller
 * \param[in] method                Assignment method of requested Ip address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE)  TcpIp_IpV4_ArpAnnounce
(
  uint32 ipAddress,
  uint8 numAnnounce,
  uint8 delay,
  uint8 ctrlIdx,
  uint8 method
);

/** \brief Unlocks an ARP entry.
 * This function unlocks the ARP entry identified by the given handle.
 *
 * \param[in] remoteAddrHandle      ARP entry handle
 * \param[in] ctrlIdx               Index of the EthIf controller
 */
TS_MOD_PRIV_DECL FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpUnlock(uint16 remoteAddrHandle, uint8 ctrlIdx);

/** \brief Retrieve physical address from remote host in ARP cache
 *
 * This service queries the IP/physical address translation table specified by CtrlIdx
 * and returns the physical address related to the IP address specified by IpAddrPtr.
 *
 * \param[in]  CtrlIdx            EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  IpRemoteAddrPtr    specifies the IP address for which the physical address shall be retrieved
 * \param[out] PhysAddrPtr        Pointer to the memory where the physical address (MAC  address)
 * \param[in]  initRes            specifies if the address resolution shall be initiated (TRUE) or not
 *                                (FALSE) in case the physical address related to the specified IP
 *                                address is currently unknown.
 *
 * \return Result of operation
 * \retval TCPIP_E_OK              specified IP address resolved, physical address provided via
 *                                 PhysAddrPtr
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address currently unknown (address resolution initiated
 *                                 if initRes set to TRUE)
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_Arp_getRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
);

/** \brief Set physical address of remote host in ARP table
 *
 * This service adds, updates or removes a physical address from a remote host in the ARP table
 *
 * \param[in]  CtrlIdx            EthIf controller index to identify the related ARP table.
 * \param[in]  IpRemoteAddrPtr    specifies the IP address for which the physical address shall be
 *                                set in the ARP table
 * \param[in]  PhysAddrPtr        specifies the physical address which shall be set in the ARP table
 * \param[in]  State              TCPIP_PHYS_ADDR_ENTRY_STATE_FREE: remove the specified entry in the
 *                                ARP table
 *                                TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: add/update the specified
 *                                entry in the ARP table
 *                                TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: set the specified entry in
 *                                the ARP table to static (entry will remain in the cache until
 *                                it is deleted with TcpIp_SetRemotePhysAddr or TcpIp is reinitialized)
 *
 * \return Result of operation
 * \retval TCPIP_OK:               physical address successfully added/updated/removed
 * \retval TCPIP_E_PHYS_ADDR_MISS: physical address currently unknown (in case the entry shall be
 *                                 removed)
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_Arp_setRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief Starts/Stops Address Conflict Detection for the requested local address
 *
 * Required: Single Unicast Local Address Id per controller only
 *
 * \param[in] localAddrId     local address which should be observed for conflicts
 * \param[in] conflictFctPtr  Function pointer to conflict indication function
 * \param[in] mechanism       defend mechanism used to defend address
 *                            TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF: stops
 *                            the current address detection
 *                            TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_ABANDON: conflict
 *                            function is called when conflict is detected
 *                            TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND: if 2
 *                            conflicts are detected within defend interval conflict
 *                            function is called
 *                            TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND_INF:
 *                            ip address is defended indefinitely
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_activate
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr,
  uint8 mechanism
);

/** \brief Initalize static IpV4
 *
 * This function initialize the static IpV4 data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_init(TcpIp_LocalAddrIdType localAddrId);

/** \brief Request static IpV4 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_startAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief Releases static IpV4 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_stopAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief get Ip configuration of the static IpV4 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the static IpV4 assignment
 *
 * \param[in]  localAddrId    Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief get assignment status of the static IpV4 assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId     Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_StaticIp_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId
);

/** \brief inform static IpV4 whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_assignmentActive
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
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_StaticIp_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Calculates partial checksum.
 *
 * Calculates partial checksum for IpV4 header.
 *
 * \param[in] srcAddrPtr   source IpV4 address
 * \param[in] destAddrPtr  destination IpV4 address
 * \param[in] lenByte      length of header
 * \param[in] protocol     protocol used
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destAddrPtr,
  uint16 lenByte,
  uint8 protocol
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and resets IPv4 Unit measurement data
 *
 * This function reads and resets invalid ip address, replaced arp,
 * dropped arp frames counter.
 *
 * \param[in] MeasurementIdx           Index to select specific measurement data
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_GetAndResetCounter
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
/** \brief Checks if the given address is limited broadcast
 *
 * Checks if the given address is limited broadcast
 *
 * \param[in] ctrlIdx             controller index
 * \param[in] localAddrId         local address id
 * \param[in] localIpAddrPtr      pointer to the local address
 *
 * \retval TRUE       ip addresses is limited broadcast
 * \retval FALSE      ip addresses isn't limited broadcast
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isAddrLimBroadcast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr
);

/** \brief Gets range of local address ids for a given IpV4 controller
 *
 * Gets range of local address ids for a given IpV4 controller
 *
 * \param[in] ctrlIdx              controller index
 * \param[out] startIndexPtr       Index of first local address
 * \param[out] endIndexPtr         Index of first local address for next controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr
);
#endif /* TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON */


#if (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON)

/** \brief Initalize Link local address assignment
 *
 * This function initialize the link local data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_init
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Initalize Link local address assignment
 *
 * This function initialize the link local data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_init
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Request AUTOIP address assignment.
 *
 * This function decides what to do if the IP address manager wants to assign the
 * link local address in a specific state of the link local state machine. E.g.
 * generate IP address and start probing
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Request AUTOIP_DOIP address assignment.
 *
 * This function decides what to do if the IP address manager wants to assign the
 * link local address in a specific state of the link local state machine. E.g.
 * generate IP address and start probing
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Release AUTOIP address assignment.
 *
 * This function decides what to do if the IP address manager wants to unassign the
 * link local address in a specific state of the link local state machine. E.g.
 * stop probing, announcing, defending.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Release AUTOIP_DOIP address assignment.
 *
 * This function decides what to do if the IP address manager wants to unassign the
 * link local address in a specific state of the link local state machine. E.g.
 * stop probing, announcing, defending.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief trigger announcement
 *
 * This API function triggers the ARP to announce the link local ip address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
*/
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
);

/** \brief trigger announcement
 *
 * This API function triggers the ARP to announce the link local ip address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
*/
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
);

/** \brief get assignment status of the AUTOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_AutoIp_isAssigned
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief get assignment status of the AUTOIP_DOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_isAssigned
(
  TcpIp_LocalAddrIdType localAddrId
);


/** \brief get Ip configuration of the AUTOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 * \param[out] gatewayIpPtrPtr    Address the gateway Ip address shall be stored to
 * \param[out] netmaskIpPtrPtr    Address the netmask shall be stored to
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
);


/** \brief get Ip configuration of the AUTOIP_DOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 * \param[out] gatewayIpPtrPtr    Address the gateway Ip address shall be stored to
 * \param[out] netmaskIpPtrPtr    Address the netmask shall be stored to
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
);


/** \brief get Ip address of the AUTOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
);


/** \brief get Ip address of the AUTOIP_DOIP assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
);


/** \brief dummy function to request assignment of IP address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] LocalIpAddrPtr  Pointer to requested IP
 *
 * \retval E_OK Request is successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_AutoIp_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);


/** \brief dummy function to request assignment of IP address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] LocalIpAddrPtr  Pointer to requested IP
 *
 * \retval E_OK Request is successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);


/** \brief Triggers periodic local assignment functions.
 *
 * This function handles delay of link local ip address assignment.
 *
 * \param[in] timerTick  current time in ticks of TcpIp timer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_mainFunction
(
  uint16 timerTick
);

#endif /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
/** \brief Perform ARP probing for requested Ip address.
 *
 * \param[in] arpProbeResultFctPtr  Function pointer to result indication function of Arp probing
 * \param[in] ipAddress             Ip address to probe in the network
 * \param[in] numProbes             number of probes to transmit
 * \param[in] lastProbeWait         time to wait after last probe was sent
 * \param[in] randomInterval        TRUE: probe interval random 1-2s, FALSE; probe interval: 1s
 * \param[in] localAddrId           Local address the frame has been received on.
 * \param[in] method                Method of the Local address the frame has been received on.
 * \param[in] probe_delay           Delay time for transmitting first probe
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE)
  TcpIp_IpV4_ArpProbe
    (
      TcpIp_IpV4_ArpProbeResult_FctPtrType arpProbeResultFctPtr,
      uint32 ipAddress,
      uint8 numProbes,
      uint8 lastProbeWait,
      boolean randomInterval,
      TcpIp_LocalAddrIdType localAddrId,
      uint8 method,
      uint16 probe_delay
    );

#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */

#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
/** \brief Reports address conflict to the upper layer
 *
 * This function calls the Duplicate address detection callout function.
 *
 * \param[in]  IpAddrId           Id of the conflicted local address
 * \param[in]  IpAddr             Conflicted IpV4 address
 * \param[in]  LocalPhysAddrPtr   Local physical address mapped to the Ipv4 address
 * \param[in]  RemotePhysAddrPtr  Remote physical address mapped to the Ipv4 address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_conflictCallout
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrConstPtr IpAddr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_CONST) LocalPhysAddrPtr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_CONST) RemotePhysAddrPtr
);
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

#endif /* ifndef TCPIP_IPV4_INT_H */

/*==================[end of file]===========================================*/
