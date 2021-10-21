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
 * MISRAC2012-2) Deviated Rule: 8.6 (required)
 * An identifier with external linkage shall have exactly one external definition.
 *
 * Reason:
 * False Positive, violation only occurs in unit tests, TcpIp_IPV6_Prefix_Mask is defined
 * in TcpIp_IpV6.c
 *
 */

#ifndef TCPIP_IPV6_INT_H
#define TCPIP_IPV6_INT_H

/*==================[inclusions]============================================*/
#include <TSMem.h>                /* TS_MemCpy(), TS_MemSet() */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_Ip_Int.h>         /* SW-Unit IP interface definitions */
#include <TcpIp_IpAddrM_Int.h>    /* used SW-Unit interface definitions */

/*==================[macros]================================================*/

/** \brief IPV6 address length in units of uint32 */
#define TCPIP_IPV6_ADDR_SIZE                   4U

/** \brief IPV6 address length in units of bytes */
#define TCPIP_IPV6_ADDR_BYTE_SIZE              (4U*TCPIP_IPV6_ADDR_SIZE)

/** \brief IPV6 Multicast identification mask */
#define TCPIP_IPV6_ADDR_MULTICAST_MASK         TS_IF_BE_LE(0xFF000000U,0x000000FFU)

/** \brief IPV6 Multicast identification value */
#define TCPIP_IPV6_ADDR_MULTICAST_VALUE        TS_IF_BE_LE(0xFF000000U,0x000000FFU)

/** \brief IPV6 Link Local identification mask */
#define TCPIP_IPV6_ADDR_LINK_LOCAL_MASK        TS_IF_BE_LE(0xFFFF0000U,0x0000FFFFU)

/** \brief IPV6 Link Local identification value */
#define TCPIP_IPV6_ADDR_LINK_LOCAL_VALUE       TS_IF_BE_LE(0xFE800000U,0x000080FEU)

/** \brief specific value for an NDP handle representing a multicast physical address */
#define TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST 0xFFFEU

/** \brief macro to copy an ipv6 address from a linear buffer to uint32 array */
#define TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD(ipAddrPtr, dataPtr)          \
  TS_MemCpy(ipAddrPtr, dataPtr, TCPIP_IPV6_ADDR_BYTE_SIZE);

/** \brief macro to copy an ipv6 address from a uint32 array to a linear buffer */
#define TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD(dataPtr, ipAddrPtr)            \
  TS_MemCpy(dataPtr, ipAddrPtr, TCPIP_IPV6_ADDR_BYTE_SIZE);

/** \brief macro to copy an ipv6 address */
#define TCPIP_IPV6_ADDR_CPY(dst, src)                                  \
  TS_MemCpy((dst), (src), TCPIP_IPV6_ADDR_BYTE_SIZE)

/** \brief macro to compare two ipv6 addresses */
#define TCPIP_IPV6_ADDR_CMP(dst, src)                                  \
  (TS_MemCmp((dst), (src), TCPIP_IPV6_ADDR_BYTE_SIZE) == E_OK)

/** \brief macro to check if an ipV6 address is a multicast */
#define TCPIP_IPV6_ADDR_IS_MULTICAST(ipAddr)                           \
  (((ipAddr)[0U] & TCPIP_IPV6_ADDR_MULTICAST_MASK) == TCPIP_IPV6_ADDR_MULTICAST_VALUE)

/** \brief macro to check if an ipV6 address is a link local address */
#define TCPIP_IPV6_ADDR_IS_LINK_LOCAL(ipAddr)                          \
  (((ipAddr)[0U] &  TCPIP_IPV6_ADDR_LINK_LOCAL_MASK) == TCPIP_IPV6_ADDR_LINK_LOCAL_VALUE)

/** \brief macro to check if an ipV6 address is unspecified */
#define TCPIP_IPV6_ADDR_IS_UNSPECIFIED(ipAddr)                         \
  (((ipAddr)[0U] | (ipAddr)[1U] | (ipAddr)[2U] | (ipAddr)[3U]) == 0U)

/** \brief macro to set an ipv6 address to unspecified */
#define TCPIP_IPV6_SET_UNSPECIFIED_IP(ipAddr)                          \
  TS_MemSet32_NoCheck((ipAddr), 0U, TCPIP_IPV6_ADDR_BYTE_SIZE)

/** \brief Get ipv6 solict multicast address
 *
 * This API derives the ipv6 solict multicast address from the ipv6 unciast address
 *
 * \param[out] multIpAddr  Points to the ipv6 solicit multicast address
 * \param[in] uniIpAddr    Points to the ipV6 unciast address
 */
#define TcpIp_IpV6_getSolicitMulti(multIpAddr, uniIpAddr)              \
  do                                                                   \
  {                                                                    \
    (multIpAddr)[0U] = TS_IF_BE_LE(0xFF020000U,0x000002FFU);           \
    (multIpAddr)[1U] = TS_IF_BE_LE(0x00000000U,0x00000000U);           \
    (multIpAddr)[2U] = TS_IF_BE_LE(0x00000001U,0x01000000U);           \
    (multIpAddr)[3U] =(TS_IF_BE_LE(0xFF000000U,0x000000FFU) |          \
                       ((uniIpAddr)[3U] & TS_IF_BE_LE(0x00FFFFFFU,0xFFFFFF00U))); \
  } while (0U)

/** \brief Set all nodes multicast ipV6 address
 *
 * This API returns the all nodes multicast address
 *
 * \param[out] addrPtr  Points to the ipv6 address
 */
#define TcpIp_Ipv6_getAllNodesMulticastIpAddr(addrPtr)                 \
  do                                                                   \
  {                                                                    \
    /* FF02:0:0:0:0:0:0:1 */                                           \
    (addrPtr)[0U] = TS_IF_BE_LE(0xFF020000U,0x000002FFU);              \
    (addrPtr)[1U] = TS_IF_BE_LE(0x00000000U,0x00000000U);              \
    (addrPtr)[2U] = TS_IF_BE_LE(0x00000000U,0x00000000U);              \
    (addrPtr)[3U] = TS_IF_BE_LE(0x00000001U,0x01000000U);              \
  } while (0U)

/** \brief Verify for solicit multicast address
 *
 * This API verifies if an IPv6 address is a solicit multicast address
 *
 * \param[in] addrPtr  Points to the ipv6 address
 *
 * \retval TRUE address is a solicit mutlicast,
 * \retval FALSE address is not a solicit mutlicast
 */
#define TcpIp_IpV6_isSolicitMulti(addrPtr)                             \
  (((addrPtr)[0U] == TS_IF_BE_LE(0xFF020000U,0x000002FFU)) &&          \
   ((addrPtr)[1U] == TS_IF_BE_LE(0x00000000U,0x00000000U)) &&          \
   ((addrPtr)[2U] == TS_IF_BE_LE(0x00000001U,0x01000000U)) &&          \
   (((addrPtr)[3U] & TS_IF_BE_LE(0xFF000000U,0x000000FFU)) ==          \
                     TS_IF_BE_LE(0xFF000000U,0x000000FFU)   )          \
  )

/** \brief Verify for all nodes multicast address
 *
 * This API verifies if an IPv6 address is a all nodes multicast address
 * FF02:0:0:0:0:0:0:1
 *
 * \param[in] addrPtr  Points to the ipv6 address
 *
 * \retval TRUE address is an all nodes mutlicast,
 * \retval FALSE address is not an all nodes mutlicast
 */
#define TcpIp_Ipv6_isAllNodesMulticastIpAddr(addrPtr)                  \
  (((addrPtr)[0U] == TS_IF_BE_LE(0xFF020000U,0x000002FFU)) &&          \
   ((addrPtr)[1U] == TS_IF_BE_LE(0x00000000U,0x00000000U)) &&          \
   ((addrPtr)[2U] == TS_IF_BE_LE(0x00000000U,0x00000000U)) &&          \
   ((addrPtr)[3U] == TS_IF_BE_LE(0x00000001U,0x01000000U))             \
  )

/** \brief Set all routers multicast ipV6 address
 *
 * This API returns the all routers multicast address
 *
 * \param[out] addrPtr  Points to the ipv6 address
 */
#define TcpIp_Ipv6_getAllRoutersMulticastIpAddr(addrPtr)                 \
  do                                                                   \
  {                                                                    \
    /* FF02:0:0:0:0:0:0:2 */                                           \
    (addrPtr)[0U] = TS_IF_BE_LE(0xFF020000U,0x000002FFU);              \
    (addrPtr)[1U] = TS_IF_BE_LE(0x00000000U,0x00000000U);              \
    (addrPtr)[2U] = TS_IF_BE_LE(0x00000000U,0x00000000U);              \
    (addrPtr)[3U] = TS_IF_BE_LE(0x00000002U,0x02000000U);              \
  } while (0U)

/** \brief This functions generates an IPv6 address by adding the EUI-64
 *   identifier to the passed prefix.
 *
 * \param[out]  ipAddr         generated ipv6 address
 * \param[in]   linkLayerAddr  link layer address of interface
 * \param[in]   prefix         prefix for generating ipv6 address
 */
#define TcpIp_IpV6_generateIp(ipAddr,linkLayerAddr,prefix)                 \
  do                                                                       \
  {                                                                        \
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr =                   \
    (P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)) (ipAddr);                   \
    TS_MemCpy((ipAddr), (prefix), 8U);                                     \
    (ipAddrPtr)[8U] =  (linkLayerAddr)[0U];                                \
    (ipAddrPtr)[9U] =  (linkLayerAddr)[1U];                                \
    (ipAddrPtr)[10U] = (linkLayerAddr)[2U];                                \
    (ipAddrPtr)[11U] = (0xFFU);                                            \
    (ipAddrPtr)[12U] = (0xFEU);                                            \
    (ipAddrPtr)[13U] = (linkLayerAddr)[3U];                                \
    (ipAddrPtr)[14U] = (linkLayerAddr)[4U];                                \
    (ipAddrPtr)[15U] = (linkLayerAddr)[5U];                                \
    /* set the set the universal local bit */                              \
    (ipAddrPtr)[8U] |= (0x02U);                                            \
  } while (0U)

/* Deviation MISRAC2012-1 <+5> */
#define TCPIP_GET_IPV6_FROM_SOCK_ADDR_TYPE(sockStruct)                     \
  ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA))sockStruct)->addr

#define TCPIP_IPV6_SOCK_ADDR_TYPE_INIT                                     \
  {TCPIP_AF_INET6, 0U, {0U}}
/*==================[type definitions]======================================*/

/** \brief Pointer to const socket address */
typedef P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_SockAddrConstPtrType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Reception of IpV6 datagram
 *
 * By this API service the IpV6 SW-Unit gets an indication of a received IpV6 frame.
 *
 * \param[in] ctrlIdx     Index of the EthIf controller
 * \param[in] dataPtr     Points to the received data. The data contains the payload of the
 *                        Ethernet protocol (which excludes the Ethernet header but includes headers
                          of higher layers).
 * \param[in] lenByte     Length of received data.
 * \param[in] IsBroadcast Indicates if the target MAC address is a broadcast address
 * \param[in] physAddrPtr Points to MAC address of remote host (source MAC)
 *
 * \return value indicating result of processing incoming IpV6 frame
 */
TS_MOD_PRIV_DECL FUNC( TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV6_rxIndicationIp
(
  uint8 ctrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  boolean IsBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )

/** \brief Returns the maximum fragment size in an IpV6
 *
 * \param[in] localAddrId     local address identifier
 *
 * \return maximum fragment size per IpV6 datagram in units of bytes
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_getFragmentSize
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */

/** \brief Returns the maximum payload possible in an IpV6 frame considering the actual MTU
 *
 * \param[in] ipSecSaCommonIdx      IPsec security assocation information index
 * \param[in] localAddrId           local address identifier
 * \param[out] maxPayloadPtr        The maximum IP payload size
 * \param[out] additionalHdrSizePtr The additional size of the AH header
 *
 * \return Result of operation
 * \retval E_OK      IPsec is disabled or security association is established and AH header
 *                   could be retreived
 * \retval E_NOT_OK  Security association is not established, maximal length cannot be evaluated
 */

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
);

/** \brief Calculates partial checksum.
 *
 * Calculates partial checksum for IpV6 header.
 *
 * \param[in] srcAddrPtr   source IpV6 address
 * \param[in] destAddrPtr  destination IpV4 address
 * \param[in] lenByte      length of header
 * \param[in] protocol     protocol used
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destAddrPtr,
  uint16 lenByte,
  uint8 protocol
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Reads and resets IPv6 Unit measurement data
 *
 * This function reads and resets invalid ip address and invalid icmpv6 packet counter.
 *
 * \param[in] MeasurementIdx           Index to select specific measurement data
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetCounter
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
/** \brief Checks if the given address is all nodes multicast
 *
 * Checks if the given address is all nodes multicast
 *
 * \param[in] ctrlIdx             controller index
 * \param[in] localAddrId         local address id
 * \param[in] localIpAddrPtr      pointer to the local address
 *
 * \retval TRUE       ip addresses is all nodes multicast
 * \retval FALSE      ip addresses isn't all nodes multicast
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_isAddrAllNodesMulticast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr
);

/** \brief Gets range of local address ids for a given IpV6 controller
 *
 * Gets range of local address ids for a given IpV6 controller
 *
 * \param[in] ctrlIdx              controller index
 * \param[out] startIndexPtr       Index of first local address
 * \param[out] endIndexPtr         Index of first local address for next controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr
);
#endif /* TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON */

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Returns the minimal supported payload in an IpV6 frame
 *
 * \return minimal supported payload length per IpV6 datagram in units of bytes
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_getMinimumPayload(void);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
/** \brief Evaluate new socket connection ipV6 address
 *
 * \param[in]  RemoteAddrPtr  IP address and port of the remote host to connect to.
 *
 * \retval TRUE           valid remote address
 * \retval FALSE          invalid remote address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Find the destination address Ndp entry handle
 *
 * \param[in,out] remoteAddrHandlePtr   Pointer to the entry handle is written to
 * \param[in] destIpAddrPtr             destination address pointer
 * \param[in] localAddrId               local address id
 *
 *
 * \retval TCPIP_OK                 handle found
 * \retval TCPIP_E_PHYS_ADDR_MISS   address is unknown (no entry exists)
 * \retval TCPIP_E_NOT_OK           address resolution is still ongoing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_getRemoteAddrHandle
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief unlock Ndp entry
 *
 * \param[in] remoteAddrHandle  Entry handle is written to
 * \param[in] ctrlIdx           Controller used
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_unlockEntry
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

/** \brief Requests a Tx-buffer from Eth module
 *
 * TcpIp_IpV6_provideTxBuffer allocates the TxBuffer by calling EthIf_ProvideTxBuffer
 * (LenBytePtr + IP-header), allocates the Ip-header and returns the remaining buffer.
 *
 * \param[in] ipParameterPtr      Points to a structure containing Ip communication parameters
 * \param[in,out] ethTxHandlePtr  Handle represents the data link layer buffer used for transmission.
 * \param[in] lenByte             Payload length
 * \param[in] localAddrId         Local address used for transmission
 * \param[in] ipFragment          Is it a fragment
 *
 * \return Result of operation
 * \retval TCPIP_OK               A buffer of sufficient size has been provided
 * \retval TCPIP_E_NOT_OK         Requesting the buffer failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_provideTxBuffer
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 lenByte,
  TcpIp_LocalAddrIdType localAddrId,
  boolean ipFragment
);

/** \brief Performs IpV6 transmission
 *
 * This function constructs the Ip header and invokes the transmission of the Ip frame over EthIf.
 *
 * \param[in] ipParameterPtr      Points to a structure containing Ip communication parameters
 * \param[in] fragHdrPtr          Points to a structure containing Ip Fragmentation parameters
 * \param[in] ethTxHandlePtr      Handle represents the data link layer buffer used for transmission.
 * \param[in] remoteAddrHandle    Indicates remote link layer address index
 * \param[in] destIpAddrPtr       Points to the remote IP address information
 * \param[in] localAddrId         Local address used for transmission
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_transmit
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
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

TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_updateHeader
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

#if (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Returns the LocalAddrId to use for transmission for a given remote IpV6 address
 *
 * This function returns the localAddrId of a unicast local address matching the the remote IpV6
 * address passed in remoteAddrPtr.
 * This function performs the LocalAddrId selection according to Figure 4: Source Address selection
 * for transmission,
 * columns 'w/o bind' or 'ANY'.
 * Note that at least always the DefaultSrcAddr is returned, even if there is no IpV6 address assigned at all.
 *
 * \param[in] remoteAddrPtr      Points to remote IP address structure
 *
 * \return LocalAddrId selected for transmission
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId_byRemoteIp
(
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief Returns the LocalAddrId to use for transmission for a given controller and emote address
 *
 * This function returns the localAddrId of a unicast local address matching the the local address
 * passed in LocalAddrId.
 * This function performs the LocalAddrId selection according to Source Address Selection Algorithm
 * defined in IETF RFC 6724.
 *
 * \param[in] ctrlIdx          controller used for transmission
 * \param[in] remoteIpAddrPtr  address of the remote host
 *
 * \return localAddrId selected for transmission
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr
);

#endif /* (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Returns the LocalAddrId for a given IpV6 address
 *
 * This function determines the local Address Id of the IpV6 address passed as parameter.
 *
 * \param[in] remoteIpAddrPtr  IpV6 address.
 * \param[in] ctrlIdx          Index of the ethernet controller (EthIfCtrlIdx)
 *
 * \return localAddrId found, TCPIP_LOCALADDRID_INVALID if lookup failed
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getLocalAddrId_byIp
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  uint8 ctrlIdx
);

/** \brief Performs Icmp (V6) transmission
 *
 *  This function constructs the Icmp header and payload and and invokes the transmission of the Icmp
 *  frame over EthIf.
 *
 * \param[in] dataPtr         Points to the received data. The data contains the payload of the Icmp
                              message
 * \param[in] messageLengthIn Length of payload to transmit.
 * \param[in] destIpPtr       remote IP address
 * \param[in] localAddrIdIn   Local address used for transmission
 * \param[in] icmpParameterPtr  Contains icmp header informations, like ttl, type, code,
 *                              specific value
 * \param[in] truncate          Indicates if truncation of message is allowed if maximal
 *                              ipv6 payload size is exeeded
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
*/
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_Icmp_transmit
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 messageLengthIn,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrIdIn,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean truncate
);

/** \brief Performs IcmpV6 Checksum calculation
 *
 *  This function gets the partial Ip header Checksum passed via partialCRC and completes the Udp
 *  header Checksum caluclation.
 *  Finally it writes the IcmpV6 Checksum into the Icmp header.
 *
 * \param[in] that              The functor's 'this' pointer.
 * \param[in] payloadSum        Checksum of the upper layer payload and ip header
 * \param[in] headerPtr         Points to data buffer where the IcmpV6 header is located.
 * \param[in] calculateChecksum If true calculate the IcmpV6 checksum
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Icmp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
);

/** \brief Performs Icmp (V6) transmission
 *
 *  This function sets Icmp parameters and sets the function used for constructing
 *  the Icmp header and payload and invokes the transmission of the Icmp frame over IpV6.
 *
 * \param[in] copyDataFunctorPtr     The copyData functor.
 * \param[in] dataPtr                The source data buffer.
 * \param[in] messageLengthIn        The length of the message.
 * \param[in] ipAddrPtr              remote and local address
 * \param[in] remoteAddrHandle       remote address handle
 * \param[in] icmpParameterPtr       pointer to Icmp parameters
 * \param[in] useUnspecifiedSrc      specifies if Unspecified source address is used
 * \param[in] truncate               Indicates if truncation of message is allowed if maximal
 *                                   ipv6 payload size is exceeded
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
*/
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_Icmp_intTransmit
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 messageLengthIn,
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean useUnspecifiedSrc,
  boolean truncate
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
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpV6_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Obtain local IPv6 address
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
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
);

/** \brief Initalize static IpV6
 *
 * This function initialize the static IpV6 data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_init(TcpIp_LocalAddrIdType localAddrId);

/** \brief Request static IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_startAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief Releases static IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_stopAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief get Ip configuration of the static IpV6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the static IpV6 assignment
 *
 * \param[in]  localAddrId    Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief inform static IpV6 whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_assignmentActive
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
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_StaticIp_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation
 * \retval TCPIP_IPADDRM_ADDRSTATE_PREFERRED  static ip address is always preferred
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpAddrState
(
    TcpIp_LocalAddrIdType localAddrId
);

#if (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON)

/** \brief Initalize link local IpV6
 *
 * This function initialize the static IpV6 data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_init(TcpIp_LocalAddrIdType localAddrId);

/** \brief Request link local IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_startAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief Releases link local IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_stopAssign(TcpIp_LocalAddrIdType localAddrId);

/** \brief get Ip configuration of the link local IpV6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get Ip address of the link local IpV6 assignment
 *
 * \param[in]  localAddrId    Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);

/** \brief inform link local IpV6 whether an assignment has gone active or inactive
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_assignmentActive
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
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_LinkLocal_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 *
 * \return Result of operation link local ip address state
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DECL CONST(uint32, TCPIP_CONST) TcpIp_IPV6_Prefix_Mask[TCPIP_IPV6_ADDR_SIZE];

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#endif /* TCPIP_IPV6_INT_H */
