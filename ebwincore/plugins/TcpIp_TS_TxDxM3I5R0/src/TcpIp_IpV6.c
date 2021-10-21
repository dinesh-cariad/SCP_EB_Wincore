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
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1)Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 * 2)AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 */

/*==================[inclusions]============================================*/
#include <TSMem.h>                /* memory functions */
#include <EthIf.h>                /* EthIf API declarations */
#include <SchM_TcpIp.h>           /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>    /* used SW-unit interface file */
#include <TcpIp_Udp_Int.h>        /* used SW-unit interface file */
#include <TcpIp_Tcp_Int.h>        /* used SW-unit interface file */
#include <TcpIp_Ndp_Int.h>        /* used SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>    /* used SW-unit interface file */
#include <TcpIp_IpSec_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpV6_Priv.h>      /* own SW-unit interface file */


/*==================[macros]================================================*/

/** \brief IPV6 header length in units of bytes */
#define TCPIP_IPV6_HEADER_LENGTH                                      40U

/** \brief the version number in the version field */
#define TCPIP_IPV6_VERSION                                             6U

/** \brief Length of the Fragment Header */
#define TCPIP_IPV6_FRAGMENT_HEADER_LENGTH                              8U

/** \brief maximal number of allowed Routing Header */
#define TCPIP_IPV6_NUM_OF_ROUTEHDR_MAX                                 1U

#define TCPIP_IPV6_HDR_NXT_HDR_OFFSET                                  6U
#define TCPIP_IPV6_EXT_HDR_LENGTH_OFFSET                               1U
#define TCPIP_IPV6_EXT_HDR_OPTION_OFFSET                               2U
#define TCPIP_IPV6_EXT_HDR_OPTION_HDR_LENGTH                           2U
#define TCPIP_IPV6_EXT_HDR_OPTION_CHANGE_EN_ROUTE                   0x20U

/** \brief minimal mtu size */
#define TCPIP_IPV6_MIN_MTU_SIZE                                     1280U

/** \brief padding bytes - length of 1 byte */
#define TCPIP_IPV6_OPTION_TYPE_PAD1                                    0U
/** \brief padding bytes - length of n bytes */
#define TCPIP_IPV6_OPTION_TYPE_PADN                                    1U

/** \brief retrieve the option identifier */
#define TCPIP_IPV6_GET_OPTION_ID(type)                       (type) >> 6U
/** \brief if option is unknown - skip over this option */
#define TCPIP_IPV6_IDENTIFIER_SKIP                                  0x00U
/** \brief if option is unknown - discard packet */
#define TCPIP_IPV6_IDENTIFIER_DISCARD                               0x01U
/** \brief if option is unknown - send parameter problem message */
#define TCPIP_IPV6_IDENTIFIER_DISCARD_PROBLEM                       0x02U
/** \brief if option is unknown - send parameter problem message (only unicast) */
#define TCPIP_IPV6_IDENTIFIER_DISCARD_PROBLEM_MULTI                 0x03U

/* Index for Routing Type in Routing Header */
#define TCPIP_IPV6_EXT_HDR_ROUTING_TYPE                                2U
/* Index for Segments Left in Routing Header */
#define TCPIP_IPV6_EXT_HDR_SEGMENTS_LEFT                               3U

/* unicast address has link local scope */
#define TCPIP_IPV6_SCOPE_LINK_LOCAL                                 0x02U
/* unicast address has global scope */
#define TCPIP_IPV6_SCOPE_GLOBAL                                     0x0EU
/* \brief Indicates the number of headers in order */
#define TCPIP_IPV6_NUM_HEADERS                                         6U

/** \brief get the 4-bit Internet Protocol version number = 6 */
#define TCPIP_IPV6_GET_VERSION(pdata)                                   \
  ((uint8)((pdata)[0U] >> 4U))

/** \brief get the traffic class */
#define TCPIP_IPV6_GET_TRAFFIC_CLASS(headerPtr)                         \
  (((uint8)((headerPtr)[1U] >> 4U)) | (((uint8)(((headerPtr)[0U]) & 0x0FU)) << 4U))

/** \brief get the flow label */
#define TCPIP_IPV6_GET_FLOW_LABEL(headerPtr)                            \
  ((TCPIP_GET32((headerPtr), 0U)) & 0xFFFFFU)

/** \brief get the 16-bit length of IP payloader */
#define TCPIP_IPV6_GET_PAYLOADLENGTH(pdata)                             \
  TCPIP_GET16((pdata), 4)

/** \brief get the 8-bit selector "Next Header" */
#define TCPIP_IPV6_GET_NEXTHEADER(pdata)                                \
  ((pdata)[6U])

/** \brief get the recent 8-bit Hop Limit */
#define TCPIP_IPV6_GET_HOPLIMIT(pdata)                                  \
   ((pdata)[7U])

/** \brief get the 128-bit source address */
#define TCPIP_GET_IPV6_SRCADDR_PTR(pdata)                               \
  (&(pdata)[8U])

/** \brief get the 128-bit destination address */
#define TCPIP_GET_IPV6_DSTADDR_PTR(pdata)                               \
  (&(pdata)[24U])

/** \brief get the 128-bit destination address */
#define TCPIP_GET_IPV6_MULTICAST_SCOPE(ipAddr)                          \
  ((((P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))(ipAddr))[1U] & 0x0FU))

/** \brief set IPv6 header */
#define TCPIP_IPV6_CREATE_HEADER(pdata, payloadLen, nextHdr, hopLimit, flowlabel, dscp) \
  do                                                                              \
  {                                                                               \
    (pdata)[0U] = ((TCPIP_IPV6_VERSION << 4U) | (((dscp) >> 2U) & 0xfU));         \
                  /* Version, Traffic Class */                                    \
    (pdata)[1U] = ((((dscp) << 6U) & 0xc0U) | (uint8)(((flowlabel) & 0x000F0000U) >> 16));\
                  /* Traffic Class, Flow Label */                                 \
    (pdata)[2U] = (uint8)(((flowlabel) & 0x0000FF00U) >> 8); /* Flow Label */     \
    (pdata)[3U] = (uint8)((flowlabel) & 0x000000FFU);      /* Flow Label */       \
    TCPIP_SET16((pdata), 4, (payloadLen)); /* payload Length */                   \
    (pdata)[6U] = (nextHdr);  /* Next Header */                                   \
    (pdata)[7U] = (hopLimit); /* Hop Limit */                                     \
  } while (0U)

 /** \brief Set the IPv6 payloader length */
 #define TCPIP_IPV6_SET_PAYLOADLENGTH(pdata, payloadlength)              \
   TCPIP_SET16((pdata), 4, (payloadlength))

 /** \brief Set the IPv6 Next Header identifier */
 #define TCPIP_IPV6_SET_NEXTHEADER(pdata, nextheader)                    \
   do                                                                    \
   {                                                                     \
    (pdata)[6U] = (nextheader);                                          \
   } while (0U)


 /** \brief set Fragment Header */
#define TCPIP_IPV6_FRAGMENT_CREATE_HEADER(pdata,nextHeader,offset,flag,id) \
  do                                                                       \
  {                                                                        \
    (pdata)[0U] = (nextHeader); /* Next Header */                          \
    (pdata)[1U] = 0U; /* reserved */                                       \
    (pdata)[2U] = (uint8)((offset) >> 5U);  /* Fragment Offset */          \
                  /* Fragment Offset                 M-Flag */             \
    (pdata)[3U] = ((uint8)((offset) << 3U) | ((uint8)(flag) & 3U));\
    TCPIP_SET32((pdata), 4, (id)); /* Identification */                    \
  } while (0U)

/** \brief macro sets mutable fields of IPv6 header to zero */
#define TCPIP_IPSEC_IPV6_ZEROOUT_MUTABLE(headerPtr)                 \
  do                                                                \
  {                                                                 \
      (headerPtr)[0U] = (headerPtr[0U] & 0xf0U); /* traffic class */\
      (headerPtr)[1U] = 0U; /* traffic class and flow label */      \
      (headerPtr)[2U] = 0U; /* flow label */                        \
      (headerPtr)[3U] = 0U; /* flow label */                        \
      (headerPtr)[7U] = 0U; /* hop limit */                         \
  } while (0U)

/** \brief Runs over all options included in Destination Option header
 *
 * \param[in] destIpPtr         ip address of the remote host
 * \param[in] localAddrId       local address on which the packet was received on
 * \param[in] dataPtr           points to the beginning of the ip6 header
 * \param[in] dataLen           length of the received packet
 * \param[in] NextHeaderOffset  offset of the next extension header
 * \param[in] IsBroadcast       indicates if the target MAC address is a broadcast address
 *
 * \retval TCPIP_IP_RX_EXT_HDR_OK:          continue with next header
 * \retval TCPIP_IP_RX_NOT_OK:              discard the packet
 * \retval TCPIP_IP_RX_UNRECOGNIZED_OPTION: unrecognized option encountered, send
 *                                          parameter problem message
 */
#define TcpIp_IpV6_rxExtHeaderDestinationOption(dIpPtr, lAddrId, dPtr, dLen, nHdrOff, IsBroadcast) \
  TcpIp_IpV6_rxExtHeaderHopByHop((dIpPtr), (lAddrId), (dPtr), (dLen), (nHdrOff), (IsBroadcast))

/*==================[type definitions]======================================*/

/** \brief combination of local address id and local ip address */
typedef struct
{
  /** \brief ip address of the local address id */
  TcpIp_IpAddrConstPtr ipAddrPtr;
  /** \brief local address id */
  TcpIp_LocalAddrIdType localAddrId;
} TcpIp_IpV6_LocalAddrType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Check if the next header is known and at the allowed position
 *
 * \param[in]     nextHeader  the next header identification
 * \param[in,out] position    current position in the order list
 *
 * \retval TCPIP_IP_RX_EXT_HDR_OK:          continue with next header
 * \retval TCPIP_IP_RX_NOT_OK:              discard the packet
 * \retval TCPIP_IP_RX_PROTOCOL_UNREACHABLE: protocol unknown, transmits icmpV6 error messages
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpV6_checkHeaderOrder
(
  uint8 nextHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) position
);

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
/** \brief Parse IPv6 specific fragment header and fill the \a fragPtr structure for perusal by
 * \a TcpIp_IpV6_handleFragment and \a TcpIp_Ip_Frag_reassemble.
 *
 * \return generic error codes (from TcpIp_Int.h), that are interpreted IPv6 specific in
 *          \a TcpIp_IpV6_rxIndicationIp.
 */
STATIC FUNC( TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV6_parseFragmentHeader
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) dstAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) fragHeaderPtr,
  uint16 lenByte
);

/** \brief Handle the IPv6 specific part of fragment-handling within the context of
 * \a TcpIp_IpV6_processNextHeader after initially calling the generic handling in
 * \a TcpIp_Ip_Frag_reassemble.
 *
 * \return generic error codes (from TcpIp_Int.h), that are interpreted IPv6 specific in
 *          \a TcpIp_IpV6_rxIndicationIp.
 */
STATIC FUNC( TcpIp_Ip_RxReturnType , TCPIP_CODE ) TcpIp_IpV6_rxExtHeaderFragment
(
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) dstAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 NextHeaderOffset,
  boolean IsBroadcast
);
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

/** \brief Processes UDP, TCP and ICMPV6 headers and passes packet to the upper protocol layer
 *
 * \param[in] srcAddrInetPtr  source address of the received packet
 * \param[in] destIpPtr       points to the destination address of the received packet
 * \param[in] localAddrId     local address on which the packet was received on
 * \param[in] currentHeader   protocol of the current header
 * \param[in] nextHeaderPtr   points to the beginning of the next header
 * \param[in] payloadLen      length of the payload
 * \param[in] isFragmented    indicates if the packet is fragmented
 * \param[in] HopLimit        hop limit of the received packet
 *
 * \return generic error codes (from TcpIp_Int.h), that are interpreted IPv6 specific in
 *          \a TcpIp_IpV6_rxIndicationIp.
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_passToUpperLayer
(
  P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  TcpIp_IpAddrConstPtr destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint8 currentHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) nextHeaderPtr,
  uint16 payloadLen,
  boolean isFragmented,
  uint8 HopLimit
);

/** \brief Processes extension headers and passes packet to upper protocol layer
 *
 * \param[in] srcAddrInetPtr  source address of the received packet
 * \param[in] destIpPtr       points to the destination address of the received packet
 * \param[in] localAddrId     local address on which the packet was received on
 * \param[in] HopLimit        hop limit of the received packet
 * \param[in] dataPtIn        points to the beginning of the ip6 header
 * \param[in] dataLenIn       length of the received packet
 * \param[in] IsBroadcast     indicates if the target MAC address is a broadcast address
 * \param[in] PhysAddrPtr     Points to MAC address of remote host (source MAC)
 *
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_processNextHeader
(
  P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  TcpIp_IpAddrConstPtr destIpPtr,
  TcpIp_LocalAddrIdType  localAddrId,
  uint8  HopLimit,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)  dataPtrIn,
  uint16 dataLenIn,
  boolean IsBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

/** \brief transmits icmpV6 error messages
 *
 * \param[in] remoteAddrPtr   ip address of the remote host
 * \param[in] localAddrId     local address on which the packet was received on
 * \param[in] dataPtr         points to the beginning of the ip6 header
 * \param[in] dataLen         length of the received packet
 * \param[in] specificValue   value set before the payload
 * \param[in] type            Icmp message Type
 * \param[in] code            Tcmp Error Code
 * \param[in] IsBroadcast     Indicates if the target MAC address is a broadcast address
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_sendError
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 specificVal,
  uint8 type,
  uint8 code,
  boolean IsBroadcast
);

/** \brief Runs over all options included in Hop-By-Hop Option header
 *
 * \param[in] remoteIpAddrPtr   ip address of the remote host
 * \param[in] localAddrId       local address on which the packet was received on
 * \param[in] dataPtr           points to the beginning of the ip6 header
 * \param[in] dataLen           length of the received packet
 * \param[in] NextHeaderOffset  offset of the next extension header
 * \param[in] IsBroadcast       indicates if the target MAC address is a broadcast address
 *
 * \retval TCPIP_IP_RX_EXT_HDR_OK:          continue with next header
 * \retval TCPIP_IP_RX_NOT_OK:              discard the packet
 * \retval TCPIP_IP_RX_UNRECOGNIZED_OPTION: unrecognized option encountered, send
 *                                          parameter problem message
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_rxExtHeaderHopByHop
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 NextHeaderOffset,
  boolean IsBroadcast
);

/** \brief Performs source address selection on the passed controller
 *
 * \param[in] ctrlIdx          EthIf controller used for transmission
 * \param[in] remoteIpAddrPtr  ip address of the remote host
 * \param[in] preferredAddrIn  currently preferred local address
 *
 * \return localAddrId selected for transmission
 */
STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  TcpIp_IpV6_LocalAddrType preferredAddrIn
);

/** \brief Select local address based on the scope of the remote address
 *
 *  Rule 2: Prefer appropriate scope.
 *
 * \param[in] localIpAddr_1  first local address used for comparison
 * \param[in] localIpAddr_2  second local address used for comparison
 * \param[in] remoteIpAddr   ip address of the remote host
 *
 * \return TCPIP_LOCALADDRID_INVALID  both addresses have the same scope
 * \return localIpAddr_1              prefer local address 1
 * \return localIpAddr_2              prefer local address 2
 */
STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_scopeAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2,
  TcpIp_IpAddrConstPtr remoteIpAddr
);

/** \brief Select local address based on the state of the local address (preferred, deprecated)
 *
 * Rule 3: Avoid deprecated addresses.
 *
 * \param[in] localIpAddr_1  first local address used for comparison
 * \param[in] localIpAddr_2  second local address used for comparison
 *
 * \return TCPIP_LOCALADDRID_INVALID  both addresses have the same state
 * \return localIpAddr_1              prefer local address 1
 * \return localIpAddr_2              prefer local address 2
 */
STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_stateAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2
);

/** \brief Select local address based on the common prefix length
 *
 * Rule 8: Use longest matching prefix.
 *
 * \param[in] localIpAddr_1  first local address used for comparison
 * \param[in] netMask_1      netmask of the first local address used for comparison
 * \param[in] localIpAddr_2  second local address used for comparison
 * \param[in] netMask_2_     netmask of the second local address used for comparison
 * \param[in] remoteIpAddr   ip address of the remote host
 *
 * \return localIpAddr_1     prefer local address 1
 * \return localIpAddr_2     prefer local address 2
 */
STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_commonPrefixAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpAddrConstPtr netMask_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2,
  TcpIp_IpAddrConstPtr netMask_2,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr
);

/** \brief Retrieve scope of the ipv6 address
 *
 * \param[in] ipAddr  extract scope of this ip address
 *
 * \return scope of the ip address
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_getAddrScope
(
  TcpIp_IpAddrConstPtr ipAddr
);

/** \brief This function processes the highest-order two bits of an
 *         unknown type field in an option and decides what is happening
 *         with the packet
 *
 * \param[in] type          type of unknown option
 * \param[in] localAddrId   local address on which the packet was received on
 *
 * \retval TCPIP_IP_RX_EXT_HDR_OK:          continue with next header
 * \retval TCPIP_IP_RX_NOT_OK:              discard the packet
 * \retval TCPIP_IP_RX_UNRECOGNIZED_OPTION: unrecognized option encountered, send
 *                                          parameter problem message
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_processUnrecognizedOption
(
  uint8 type,
  TcpIp_LocalAddrIdType localAddrId
);

#if (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON)
/** \brief This function loop over all Extension Headers in the payload
 *         and checks if they are configured in the filter
 *
 * \param[in] currentHeader current header which is checked in filter
 * \param[in] localAddrId   local address on which the packet was received on
 *
 * \retval TRUE      header passed extension header filter
 * \retval FALSE     header is filtered
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_getExtensionHeaderFilter
(
  uint8 currentHeader,
  TcpIp_LocalAddrIdType localAddrId
);
#endif /* (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON) */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/** \brief Reads and resets dropped datagrams caused by invalid IPv6 address
 *
 * This function reads and resets dropped datagrams caused by invalid IPv6 address.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetInvalidIpAddrCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON) */

STATIC FUNC(uint16, TCPIP_CODE) Tcpip_IpV6_getNextHeaderOffset
(
  uint8 currentHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 currentHeaderDataOffset
);

#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
/** \brief parse AH and verify if mac is correct
 *
 * \param[in] localAddrId              specifies the destination IP address on which
 *                                     the frame was received
 * \param[in] remoteAddrPtr            specifies the source IP address on which the
 *                                     frame was transmitted
 * \param[in] dataPtr                  dataPtr refers to the beginning of data
 * \param[in] dataLen                  length of the data
 * \param[in] currentHeaderDataOffset  offset of the AH header in the data
 * \param[Out] ipsecSaIdx              index of the identified security association
 *                                     (used to check security policy)
 *
 * \retval TCPIP_IP_RX_EXT_HDR_OK      continue processing packet
 * \retval TCPIP_IP_RX_NOT_OK          drop packet
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_RxAhHeader
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 currentHeaderDataOffset,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipsecSaIdx
);
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) ) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV6_InvalidIpAddrCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/** \brief IP address has not been assigned yet */
STATIC CONST(uint32, TCPIP_CONST) TcpIp_IpV6_Unspecified[4U] = {0x0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/* Mask for highest prefix, first 64 bytes */
TS_MOD_PRIV_DEFN CONST(uint32, TCPIP_CONST) TcpIp_IPV6_Prefix_Mask[TCPIP_IPV6_ADDR_SIZE] = {0xFFFFFFFFU,0xFFFFFFFFU,0U,0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_CONST_8
#include <TcpIp_MemMap.h>

/* Holds all known header and their allowed order */
STATIC CONST(uint8, TCPIP_CONST) TcpIp_IPV6_HeaderOrderList[] =
  {
    TCPIP_IP_EXT_HDR_HOPBYHOP,
    TCPIP_IP_EXT_HDR_DSTOPT,
    TCPIP_IP_EXT_HDR_ROUTING,
#if(TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
    TCPIP_IP_EXT_HDR_FRAGMENT,
#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) */
#if(TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
    TCPIP_IP_EXT_HDR_AUTHENTICATION,
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
    TCPIP_IP_EXT_HDR_DSTOPT
  };

#define TCPIP_STOP_SEC_CONST_8
#include <TcpIp_MemMap.h>

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC( TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV6_rxIndicationIp
(
  uint8 ctrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  boolean IsBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  TcpIp_Ip_RxReturnType result = TCPIP_IP_RX_NOT_OK;

  DBG_TCPIP_IPV6_RXINDICATIONIP_ENTRY(ctrlIdx,dataPtr,lenByte,IsBroadcast,physAddrPtr);

  if(lenByte <= TCPIP_IPV6_HEADER_LENGTH)
  {
    /* header does not fit into received data */
    result = TCPIP_IP_RX_INVALID_HEADER;
  }
  else if(TCPIP_IPV6_GET_VERSION(dataPtr) != TCPIP_IPV6_VERSION)
  {
    /* ip version is not correct */
    result = TCPIP_IP_RX_INVALID_HEADER;
  }
  else if((lenByte - TCPIP_IPV6_HEADER_LENGTH) < TCPIP_IPV6_GET_PAYLOADLENGTH(dataPtr))
  {
    /* payload field of the IPv6 header is not correct */
    result = TCPIP_IP_RX_INVALID_HEADER;
  }
#if (TCPIP_IPV6_TRAFFIC_CLASS_FILTER_ENABLED == STD_ON)
  else if(TCPIP_IPV6_GET_TRAFFIC_CLASS(dataPtr) != 0U)
  {
    /* traffic class field of the IPv6 header is not 0 */
    result = TCPIP_IP_RX_INVALID_HEADER;
  }
#endif /* (TCPIP_IPV6_TRAFFIC_CLASS_FILTER_ENABLED == STD_ON) */
#if (TCPIP_IPV6_FLOW_LABEL_FILTER_ENABLED == STD_ON)
  else if(TCPIP_IPV6_GET_FLOW_LABEL(dataPtr) != 0U)
  {
    /* flow label field of the IPv6 header is not 0 */
    result = TCPIP_IP_RX_INVALID_HEADER;
  }
#endif /* (TCPIP_IPV6_FLOW_LABEL_FILTER_ENABLED == STD_ON) */
  else
  {
    TcpIp_SockAddrInet6Type srcAddrInet;
    srcAddrInet.domain = TCPIP_AF_INET6;

    /* save source address to data structure */
    TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD(srcAddrInet.addr, TCPIP_GET_IPV6_SRCADDR_PTR(dataPtr));

    /* source address shall not be a multicast address */
    if(!TCPIP_IPV6_ADDR_IS_MULTICAST(srcAddrInet.addr))
    {
      TcpIp_SockAddrInet6Type dstAddrInet;
#if (                                                                    \
      (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON) ||           \
      (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON) ||           \
      (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)                \
    )
      /* Deviation MISRAC2012-2 <+2> */
      TcpIp_SockAddrConstPtrType dstAddrPtr = (TcpIp_SockAddrConstPtrType)&(dstAddrInet);
      TcpIp_SockAddrConstPtrType srcAddrPtr = (TcpIp_SockAddrConstPtrType)&(srcAddrInet);
#endif /* (                                                              \
            (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON) ||     \
            (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON) ||     \
            (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)          \
          ) */
      dstAddrInet.domain = TCPIP_AF_INET6;

      /* get destination Ip address */
      TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD(dstAddrInet.addr, TCPIP_GET_IPV6_DSTADDR_PTR(dataPtr));

#if (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON)
      /* check if packet is allowed to be processed further or needs to be dropped */
      if
      (
        TcpIp_RxPolicyCheckIngressHandlerAPI
          (&result, dstAddrPtr, srcAddrPtr, (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, dataPtr, lenByte, IsBroadcast) == E_OK
      )
#endif /* TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON */
      {
        /* map destination Ip addres to local address Id of this device */
        TcpIp_LocalAddrIdType const localAddrId = TcpIp_IpV6_getLocalAddrId_byIp(dstAddrInet.addr, ctrlIdx);

        /* packet is not addressed to us, discard */
        if(localAddrId != TCPIP_LOCALADDRID_INVALID)
        {
          uint8 const HopLimit = TCPIP_IPV6_GET_HOPLIMIT(dataPtr);
          uint16 const Length = TCPIP_IPV6_HEADER_LENGTH + TCPIP_IPV6_GET_PAYLOADLENGTH(dataPtr);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
          /* check if policy exits for remote host */
          if(TcpIp_IpSecDb_checkSecurityPolicyAddr(localAddrId, srcAddrInet.addr,physAddrPtr) == E_OK)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
          {
            result =
              TcpIp_IpV6_processNextHeader
                (&srcAddrInet, dstAddrInet.addr, localAddrId, HopLimit, dataPtr, Length, IsBroadcast,physAddrPtr);
          }

#if (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)
          if(result == TCPIP_IP_RX_OK)
          {
            TcpIp_RxPacketPostProcessHandlerAPI
              (dstAddrPtr, srcAddrPtr, (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, dataPtr, lenByte, IsBroadcast);
          }
#endif /* TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON */
        }
        else
        {
          result = TCPIP_IP_RX_DEST_UNREACHABLE;
        }

#if (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON)
        /* check if packet needs to be routed */
        if((localAddrId == TCPIP_LOCALADDRID_INVALID) || (TCPIP_IPV6_ADDR_IS_MULTICAST(dstAddrInet.addr)))
        {
          result =
            TcpIp_RxPolicyCheckRoutingHandlerAPI
              (dstAddrPtr, srcAddrPtr, (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, dataPtr, lenByte, IsBroadcast);
        }
#endif /* TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
        if ((localAddrId == TCPIP_LOCALADDRID_INVALID) && (result == TCPIP_IP_RX_DEST_UNREACHABLE))
        {
          /* invalid IPv6 address */
          /* increment counter of dropped PDUs caused by invalid IPv6 address */
          /* !LINKSTO TcpIp.Design.IpV6.EA6.rxIndication,1 */
          SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
          if(TcpIp_IpV6_InvalidIpAddrCounter < 0xFFFFFFFFU)
          {
            /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIpAddrCount,1 */
            TS_AtomicAssign32(TcpIp_IpV6_InvalidIpAddrCounter, (TcpIp_IpV6_InvalidIpAddrCounter+1U));
          }
          /* !LINKSTO TcpIp.Design.IpV6.EA6.rxIndication,1 */
          SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
        }
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
      }
    }
    else
    {
      /* source address is multicast */
      result = TCPIP_IP_RX_SRCADDR_INVALID;
    }
  }
  DBG_TCPIP_IPV6_RXINDICATIONIP_EXIT(result,ctrlIdx,dataPtr,lenByte,IsBroadcast,physAddrPtr);

  return result;
}

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_getFragmentSize
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint16 mtu;
  uint16 result;

  DBG_TCPIP_IPV6_GETFRAGMENTSIZE_ENTRY(localAddrId);
  {
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    mtu = (TCPIP_CFG(ctrl,ctrlIdx)).mtu;
    result = (mtu - TCPIP_IPV6_HEADER_LENGTH);

    /* max fraglength must be multiple times of 8 bytes */
    result = (uint16)((result - TCPIP_IPV6_FRAGMENT_HEADER_LENGTH) & 0xFFF8U);
  }
  DBG_TCPIP_IPV6_GETFRAGMENTSIZE_EXIT(result,localAddrId);

  return result;
}

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
)
{
  Std_ReturnType result = E_NOT_OK;
  uint16 mtu;

  DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_ENTRY
    (ipSecSaCommonIdx,localAddrId,maxPayloadPtr,additionalHdrSizePtr);
  {
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    mtu = (TCPIP_CFG(ctrl,ctrlIdx)).mtu;
    *maxPayloadPtr = (mtu - TCPIP_IPV6_HEADER_LENGTH);

#if(TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
    if(ipSecSaCommonIdx != TCPIP_IPSECDB_INVALID_SA_INDEX)
    {
      TcpIp_IpSecSaDataType saData;

      if(TcpIp_IpSecDb_getSecurityTxAssociation(ipSecSaCommonIdx, &saData, FALSE) == E_OK)
      {
        /* get security header size from security association */
        *additionalHdrSizePtr = saData.securityHeadersize;
        *maxPayloadPtr -= saData.securityHeadersize;
        result = E_OK;
      }
    }
    else
#else /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
    TS_PARAM_UNUSED(ipSecSaCommonIdx);
    TS_PARAM_UNUSED(additionalHdrSizePtr);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
    {
      result = E_OK;
    }
  }
  DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_EXIT
    (result,ipSecSaCommonIdx,localAddrId,maxPayloadPtr,additionalHdrSizePtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destAddrPtr,
  uint16 lenByte,
  uint8 protocol
)
{
  uint8 i;
  uint16 retVal;
  uint32 tmp32;
  uint32 checksum = 0U;
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) srcIpAddrPtr;

  DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_ENTRY(srcAddrPtr,destAddrPtr,lenByte,protocol);

  srcIpAddrPtr = TCPIP_GET_IPV6_FROM_SOCK_ADDR_TYPE(srcAddrPtr);

  for(i = 0U; i < TCPIP_IPV6_ADDR_SIZE; i++)
  {
    tmp32 = COMSTACK_NTOH_UINT32(srcIpAddrPtr[i]);
    checksum += (tmp32 >> 16u) + ((uint16)tmp32);
    tmp32 = COMSTACK_NTOH_UINT32(destAddrPtr[i]);
    checksum += (tmp32 >> 16u) + ((uint16)tmp32);
  }

  checksum += lenByte;
  checksum += protocol;
  checksum = ((uint16)checksum + (checksum>>16U));
  retVal = (uint16)(checksum + (checksum>>16U));

  DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_EXIT(retVal,srcAddrPtr,destAddrPtr,lenByte,protocol);

  return retVal;
}

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV6_getMinimumPayload(void)
{
  return (TCPIP_IPV6_MIN_MTU_SIZE - TCPIP_IPV6_HEADER_LENGTH);
}

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  boolean retVal;
  /* Deviation MISRAC2012-2 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_CONST)) RemoteAddrPtr)->addr;

  DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_ENTRY(RemoteAddrPtr);
  if
  (
    (TCPIP_IPV6_ADDR_IS_UNSPECIFIED(IpAddrPtr)) ||
    (TCPIP_IPV6_ADDR_IS_MULTICAST(IpAddrPtr))
  )
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }
  DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_EXIT(retVal,RemoteAddrPtr);
  return retVal;
}
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_provideTxBuffer
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 lenByte,
  TcpIp_LocalAddrIdType localAddrId,
  boolean ipFragment
)
{
  TcpIp_ReturnType retVal  = TCPIP_E_NOT_OK;

  DBG_TCPIP_IPV6_PROVIDETXBUFFER_ENTRY
    (ipParameterPtr,ethTxHandlePtr,lenByte,localAddrId,ipFragment);
  {
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    uint8 const fragmentLength = ((ipFragment == TRUE) ? TCPIP_IPV6_FRAGMENT_HEADER_LENGTH : 0U);
    uint8 const ipHeaderLength = TCPIP_IPV6_HEADER_LENGTH + fragmentLength;
    uint16 ipLength = lenByte + ipHeaderLength;
    Eth_BufIdxType bufIdx;
    uint8 vlanPriority;
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr;

    /* is there a valid vlan priority passed ? */
    if(TCPIP_IP_VLANPRIO_USED(ipParameterPtr->vlanPriority))
    {
      /* yes: take it */
      /* !LINKSTO TcpIp.Design.Socket.Atomic.getIpParameters,1 */
      TS_AtomicAssign8(vlanPriority,ipParameterPtr->vlanPriority);
    }
    else
    {
      /* no: get default VLAN priority for this interface from config */
      vlanPriority = (TCPIP_CFG(ctrl,ctrlIdx)).defaultVlanPrio;
    }

    /* fetch a tx buffer from ethernet */
    if(EthIf_ProvideTxBuffer(
        (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
        TCPIP_IP_FRAMETYPE_IPV6,
        vlanPriority,
        &bufIdx,
        &DataPtr,
        &ipLength) == BUFREQ_OK)
    {
      /* the return length must be greater or equal to the requested buffer length */
      TCPIP_PRECONDITION_ASSERT(ipLength >= lenByte + ipHeaderLength, TCPIP_INTERNAL_API_ID);

      /* buffer was obtained, subtract IP header space from buffer */
      ethTxHandlePtr->bufferHandle = bufIdx;
      ethTxHandlePtr->bufferPtr = &DataPtr[ipHeaderLength];
      ethTxHandlePtr->bufferLength = lenByte;
      retVal = TCPIP_OK;
    }
  }

  DBG_TCPIP_IPV6_PROVIDETXBUFFER_EXIT
    (retVal,ipParameterPtr,ethTxHandlePtr,lenByte,localAddrId,ipFragment);

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_getRemoteAddrHandle
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType retVal;
  /* Deviation MISRAC2012-2 */
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr =
    (P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) destIpAddrPtr;

  DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_ENTRY(remoteAddrHandlePtr,destIpAddrPtr,localAddrId);
  {
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    *remoteAddrHandlePtr = 0U;

    /* check for special destination addresses */
    if(TCPIP_IPV6_ADDR_IS_MULTICAST(destIpPtr->addr))
    {
      /* multicast: remote physical address = multicast address */
      *remoteAddrHandlePtr = TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST;
      retVal = TCPIP_OK;
    }
    else
    {
      TcpIp_IpAddrConstPtr networkMaskPtr;
      TcpIp_IpAddrConstPtr routerAddrPtr;
      TcpIp_IpAddrConstPtr localIpAddrPtr;
      TcpIp_IpAddrConstPtr nextHopIpPtr;
      uint16 destCacheHandle;

      /* assume default next hop = default gateway */
      TcpIp_IpAddrM_getIpConfig(localAddrId, &localIpAddrPtr, &routerAddrPtr, &networkMaskPtr);

      /* set router as next hop */
      nextHopIpPtr = routerAddrPtr;

      retVal = TcpIp_Ndp_DestCache_getNextHopAddrAndLock
              (
                destIpPtr->addr,
                localIpAddrPtr,
                ctrlIdx,
                &destCacheHandle,
                &nextHopIpPtr,
                networkMaskPtr
              );

      if(retVal == TCPIP_OK)
      {
        boolean createNewEntry = TRUE;
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
        uint8 state = TcpIp_IpAddrM_getIpAddrState(localAddrId);

        if(state == TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC)
        {
          createNewEntry = FALSE;
        }
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)*/
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)*/

        /* no special physical address has been selected yet */
        retVal = TcpIp_Ndp_NCache_lookupAndLock(ctrlIdx, nextHopIpPtr, remoteAddrHandlePtr, createNewEntry);

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
        if((retVal != TCPIP_OK) && (state == TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC))
        {
          /* if no entry can be found for the neighbour, check if router link layer address is available */
          if(nextHopIpPtr != routerAddrPtr)
          {
            /* remote address is not in NDP cache, let's see if a router is. */
            retVal = TcpIp_Ndp_NCache_lookupAndLock(ctrlIdx, routerAddrPtr, remoteAddrHandlePtr, FALSE);
          }
        }
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)*/
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)*/

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
        /* unlock the destination cache entry */
        TcpIp_Ndp_DestCache_unlock(destCacheHandle,ctrlIdx);
#else
        TS_PARAM_UNUSED(destCacheHandle);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */
      }
    }
  }

  DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_EXIT(retVal,remoteAddrHandlePtr,destIpAddrPtr,localAddrId);

   return retVal;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_unlockEntry
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
)
{
  DBG_TCPIP_IPV6_UNLOCKENTRY_ENTRY(remoteAddrHandle, ctrlIdx);

  TcpIp_Ndp_NCache_unlock(remoteAddrHandle,ctrlIdx);

  DBG_TCPIP_IPV6_UNLOCKENTRY_EXIT(remoteAddrHandle, ctrlIdx);
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_transmit
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType retVal;
  /* Deviation MISRAC2012-2 */
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr =
    (P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) destIpAddrPtr;

  DBG_TCPIP_IPV6_TRANSMIT_ENTRY
    (ipParameterPtr,fragHdrPtr,ethTxHandlePtr,remoteAddrHandle,destIpAddrPtr,localAddrId);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF )
  TS_PARAM_UNUSED(fragHdrPtr);
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF ) */
  {
    uint8 protocol= ipParameterPtr->protocol;

    /* get local source IP address for transmission */
    uint8 ttl;
    uint8 dscp;
    uint32 flowLabel;
    TcpIp_IpAddrConstPtr localIpAddrPtr;

    /* add IP header space to passed buffer */
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipHeaderPtr =
      &ethTxHandlePtr->bufferPtr[-1*((sint8)TCPIP_IPV6_HEADER_LENGTH)];
    uint16 ipPayloadLength = ethTxHandlePtr->bufferLength;
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    if(ipParameterPtr->useUnspecifiedSrc)
    {
      /* overrule source ip address selection */
      localIpAddrPtr = TcpIp_IpV6_Unspecified;
    }
    else
    {
      TCPIP_PRECONDITION_ASSERT
        (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddrId)).addrType,IPV6),
         TCPIP_INTERNAL_API_ID);

      /* assume default next hop = default gateway */
      TcpIp_IpAddrM_getIpAddress(localAddrId, &localIpAddrPtr);
    }

    /* !LINKSTO TcpIp.Design.Socket.Atomic.getIpParameters,1 */
    TS_AtomicAssign8(ttl, ipParameterPtr->ttl);

    TS_AtomicAssign8(dscp, ipParameterPtr->dscp);

    TS_AtomicAssign32(flowLabel, ipParameterPtr->flowLabel);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
    if (fragHdrPtr->IPisFragment)
    {
      /* make space for the fragment header */
      ipHeaderPtr = &ipHeaderPtr[-1*((sint8)TCPIP_IPV6_FRAGMENT_HEADER_LENGTH)];
      ipPayloadLength += TCPIP_IPV6_FRAGMENT_HEADER_LENGTH;

      {
        TCPIP_IPV6_FRAGMENT_CREATE_HEADER
          (
           &ipHeaderPtr[TCPIP_IPV6_HEADER_LENGTH],
           protocol,
           (uint16)(fragHdrPtr->IPoffset >> 3U),
           fragHdrPtr->IPmoreFlag,
           ipParameterPtr->identifier
          );
        protocol= TCPIP_IP_EXT_HDR_FRAGMENT;
      }
    }
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF */

    /* construct IP header */
    /* Deviation MISRAC2012-2 */
    TCPIP_IPV6_CREATE_HEADER( ipHeaderPtr,
                              ipPayloadLength,
                              protocol,
                              ttl,
                              flowLabel,
                              dscp
                            );
    TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD(TCPIP_GET_IPV6_DSTADDR_PTR(ipHeaderPtr), destIpPtr->addr);
    TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD(TCPIP_GET_IPV6_SRCADDR_PTR(ipHeaderPtr), localIpAddrPtr);

    {
      /* this array must be in the same scope as physAddrPtr,
         since it lives and dies with it (multicast) */
      uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
      P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr;

      /* get the physical destination address */
      if(remoteAddrHandle == TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST)
      {
        /* multicast address - derived from IP multicast address */
        TcpIp_IpV6_GetPhysMulticastAddr(ethAddr, destIpPtr->addr);
        physAddrPtr = ethAddr;
      }
      else
      {
        /* dedicated physical address from NDP table */
        physAddrPtr = TcpIp_Ndp_NCache_getPhysAddrPtr(remoteAddrHandle,ctrlIdx);
      }

      /* !LINKSTO TcpIp.ASR40.SWS_TCPIP_00131,1 */
      if(EthIf_Transmit(
          (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
           ethTxHandlePtr->bufferHandle,
           TCPIP_IP_FRAMETYPE_IPV6,
           FALSE,
           TCPIP_IPV6_HEADER_LENGTH + ipPayloadLength,
           physAddrPtr) == E_OK)
      {
        retVal = TCPIP_OK;
      }
      else
      {
        retVal = TCPIP_E_NOT_OK;
      }
    }
  }

  DBG_TCPIP_IPV6_TRANSMIT_EXIT
    (retVal,ipParameterPtr,fragHdrPtr,ethTxHandlePtr,remoteAddrHandle,destIpAddrPtr,localAddrId);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 payloadSum,
  uint16 ipPayloadLength,
  uint16 frgCnt
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  uint16 partialChecksum;
  uint32 sumWord;
  TcpIp_IpAddrConstPtr localIpAddrPtr;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr = ethTxHandlePtr[0U].bufferPtr;

  DBG_TCPIP_IPV6_UPDATEHEADER_ENTRY
    (updateHeaderFunctorPtr,ipParameterPtr, destIpPtr, ethTxHandlePtr,
     localAddrId, payloadSum, ipPayloadLength, frgCnt);

#if(TCPIP_SECURITY_MODE < TCPIP_STATIC_IPSEC)
  TS_PARAM_UNUSED(frgCnt);
#endif

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  if(ipParameterPtr->ipSecSaCommonIdx != TCPIP_IPSECDB_INVALID_SA_INDEX)
  {
    /* AH is included change upper layer header offset */
    bufferPtr = &bufferPtr[((sint8)ipParameterPtr->addHeaderLength)];
  }
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

  if(ipParameterPtr->useUnspecifiedSrc)
  {
    /* overrule source ip address selection */
    localIpAddrPtr = TcpIp_IpV6_Unspecified;
  }
  else
  {
    TCPIP_PRECONDITION_ASSERT
      (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddrId)).addrType,IPV6),
       TCPIP_INTERNAL_API_ID);

    /* assume default next hop = default gateway */
    TcpIp_IpAddrM_getIpAddress(localAddrId, &localIpAddrPtr);
  }


  /* the upper layer wants to add a checksum including the Ip pseudo header + payload checksum.
   * For fragmentation, this is mandatory for the header fragment. */
  if(ipParameterPtr->calculateChecksum)
  {
    const uint16 protoLen = ipPayloadLength
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
          - ipParameterPtr->addHeaderLength
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
          ;
    /* calculate Ip pseudo header checksum fragment and pass to upper layer */
    partialChecksum =
      TcpIp_IpV6_calcPartialChecksum
        (destIpPtr, localIpAddrPtr, protoLen, ipParameterPtr->protocol);

    sumWord = (uint32)partialChecksum + (uint32)payloadSum;
    sumWord = (sumWord&0xFFFFU)+(sumWord>>16U);                   /* may overflow again */
    payloadSum = (uint16)((sumWord&0xFFFFU)+(sumWord>>16U)); /* no more overflow */
  }

  TCPIP_PRECONDITION_ASSERT((updateHeaderFunctorPtr != NULL_PTR), TCPIP_INTERNAL_API_ID);

  /* payloadSum included in partialChecksum -> just the UDP/ICMP header left */
  updateHeaderFunctorPtr->updateHeaderFctPtr
  (
    updateHeaderFunctorPtr,
    payloadSum,
    bufferPtr,
    ipParameterPtr->calculateChecksum
  );

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  /* insert IPsec header */
  if(ipParameterPtr->ipSecSaCommonIdx != TCPIP_IPSECDB_INVALID_SA_INDEX)
  {
    P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)remoteAddrV6Ptr =
    /* Deviation MISRAC2012-2 */
    (P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA))destIpPtr;
    const uint8 ttl = 0U;
    const uint8 dscp = 0U;
    const uint32 flowLabel = 0U;
    const sint8 bufferOffset =
      -1* (sint8)(((sint8) ipParameterPtr->addHeaderLength + (sint8) TCPIP_IPV6_HEADER_LENGTH));

    /* create Immutable header */
    bufferPtr = &bufferPtr[bufferOffset];

    /* Deviation MISRAC2012-2 */
    TCPIP_IPV6_CREATE_HEADER( bufferPtr,
                              ipPayloadLength,
                              TCPIP_IP_EXT_HDR_AUTHENTICATION,
                              ttl,
                              flowLabel,
                              dscp
                            );
    TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD(TCPIP_GET_IPV6_DSTADDR_PTR(bufferPtr), remoteAddrV6Ptr->addr);
    TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD(TCPIP_GET_IPV6_SRCADDR_PTR(bufferPtr), localIpAddrPtr);

    /* attach the AH header */
    if(TcpIp_IpSec_buildAh
       (ipParameterPtr, ethTxHandlePtr, frgCnt, TCPIP_IP_MINIMUM_IPV6_HEADER_LENGTH) == E_OK)
    {
      result = TCPIP_OK;
    }
  }
  else
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  {
    result = TCPIP_OK;
  }

  DBG_TCPIP_IPV6_UPDATEHEADER_EXIT
    (result, updateHeaderFunctorPtr,ipParameterPtr, destIpPtr, ethTxHandlePtr,
     localAddrId, payloadSum, ipPayloadLength, frgCnt);

  return result;
}

#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId_byRemoteIp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  TcpIp_IpV6_LocalAddrType preferredAddr;
  uint8_least i;

  preferredAddr.localAddrId = TCPIP_LOCALADDRID_INVALID;
  preferredAddr.ipAddrPtr = NULL_PTR;

  DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(remoteAddrPtr);

  /* loop all controllers */
  for(i = 0U; (i < TcpIp_PBcfgPtr->ctrlNum); i++)
  {
    preferredAddr = TcpIp_IpV6_getTxLocalAddrId((uint8)i, remoteAddrPtr, preferredAddr);
  }

  DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_EXIT(preferredAddr.localAddrId,remoteAddrPtr);

  return preferredAddr.localAddrId;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr
)
{
  TcpIp_IpV6_LocalAddrType preferredAddr;

  DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_ENTRY(ctrlIdx,remoteIpAddrPtr);

  preferredAddr.localAddrId = TCPIP_LOCALADDRID_INVALID;
  preferredAddr.ipAddrPtr = NULL_PTR;

  preferredAddr =
    TcpIp_IpV6_getTxLocalAddrId(ctrlIdx, remoteIpAddrPtr, preferredAddr);

  DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_EXIT(preferredAddr.localAddrId,ctrlIdx,remoteIpAddrPtr);

  return preferredAddr.localAddrId;
}

#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpV6_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_StateType returnState = TCPIP_STATE_OFFLINE;

  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &(TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx));

  DBG_TCPIP_IPV6_GETIFSTATE_ENTRY(localAddrId);

  if(ctrlPtr->localAddrIdAny == localAddrId)
  {
    /* if there is a controller ANY requested, check the controllers
       unicast local address for availability */
    uint8_least const startIndex = ctrlPtr->intLocalAddrIdFirst;
    uint8_least const endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* iterate over all local addresses assigned to this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      TcpIp_LocalAddrIdType lAddrId = TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i);
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) lAddrPtr =
        &(TCPIP_CFG(localAddr,lAddrId));

      /* check for unicast addresses only */
      if(TCPIP_IPADDRM_IS_ADDRTYPE(lAddrPtr->addrType,UNICAST))
      {
        returnState = TcpIp_IpAddrM_getIfState(lAddrId);

        /* return ONLINE if at least one unicast address is in state ONLINE,
           otherwise return OFFLINE */
        if(returnState == TCPIP_STATE_ONLINE)
        {
          break;
        }
      }
    }
  }
  else
  {
    /* otherwise check the concrete local address for availability */
    returnState = TcpIp_IpAddrM_getIfState(localAddrId);
  }

  DBG_TCPIP_IPV6_GETIFSTATE_EXIT(returnState,localAddrId);

  return returnState;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
)
{
  CONSTP2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) intIpAddrPtr =
    /* Deviation MISRAC2012-2 */
    (P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) IpAddrPtr;
  P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) defaultRouterPtr =
    /* Deviation MISRAC2012-2 */
    (P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) DefaultRouterPtr;

  /* pointers for obtaining IP configuration */
  TcpIp_IpAddrConstPtr unicastPtr;
  TcpIp_IpAddrConstPtr gatewayPtr;
  TcpIp_IpAddrConstPtr subNetmasktPr;

  uint8 NetmaskCIDR = 0U;
  uint8 iByte;

  DBG_TCPIP_IPV6_GETIPCONFIG_ENTRY(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);

  /* get the current Ip configuration */
  TcpIp_IpAddrM_getIpConfig(LocalAddrId,&unicastPtr,&gatewayPtr,&subNetmasktPr);

  /* copy data to output parameters */
  TCPIP_IPV6_ADDR_CPY(intIpAddrPtr->addr, unicastPtr);

  /* convert netmask to  CIDR notation */
  for(iByte = 0U; iByte < 4U; iByte++)
  {
    uint8 NetmaskByte = 0U;
    TCPIP_GET_NUMBER_OF_SET_BITS_32(subNetmasktPr[iByte],NetmaskByte);
    NetmaskCIDR += NetmaskByte;
  }

  /* write netmask in CIDR notation */
  *NetmaskPtr = NetmaskCIDR;

  TCPIP_IPV6_ADDR_CPY(defaultRouterPtr->addr, gatewayPtr);

  DBG_TCPIP_IPV6_GETIPCONFIG_EXIT(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV6_getLocalAddrId_byIp
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  uint8 ctrlIdx
)
{
  CONSTP2CONST(TcpIp_ctrlIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
    &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV6);

  uint8_least i;
  TcpIp_LocalAddrIdType result = TCPIP_LOCALADDRID_INVALID;
  uint8_least const startIndex = ctrlPtr->intLocalAddrIdFirst;
  uint8_least const endIndex = startIndex + ctrlPtr->intLocalAddrNum;

  DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_ENTRY(remoteIpAddrPtr,ctrlIdx);

  /* iterate over all local addresses assigned to this controller */
  for(i = startIndex; i < endIndex; i++)
  {
    uint8 const LocalAddrId = TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i);

    TcpIp_IpAddrConstPtr unicastPtr;

    TCPIP_PRECONDITION_ASSERT
      (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,LocalAddrId)).addrType,IPV6),
       TCPIP_INTERNAL_API_ID);

    TcpIp_IpAddrM_getIpAddress(LocalAddrId,&unicastPtr);

    if(TCPIP_IPV6_ADDR_CMP(unicastPtr, remoteIpAddrPtr))
    {
      /* exact match */
      /* this matches: unicast, multicast */
      result = LocalAddrId;
      break;
    }
  }

  DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_EXIT(result,remoteIpAddrPtr,ctrlIdx);

  return result;
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetCounter
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  DBG_TCPIP_IPV6_GETANDRESETCOUNTER_ENTRY(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);

  switch(MeasurementIdx)
  {
    case TCPIP_MEAS_DROP_IPV6:
      TcpIp_IpV6_GetAndResetInvalidIpAddrCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_DROP_ICMPV6:
      TcpIp_IpV6_GetAndResetInvalidIcmpCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_ALL:
      TcpIp_IpV6_GetAndResetInvalidIpAddrCount(TRUE, NULL_PTR);
      TcpIp_IpV6_GetAndResetInvalidIcmpCount(TRUE, NULL_PTR);
      break;
    /* CHECK: NOPARSE */
    default:
      TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
      /* measurement index not supported */
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPV6_GETANDRESETCOUNTER_EXIT(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
}

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_isAddrAllNodesMulticast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr
)
{
  boolean retVal = FALSE;
  DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_ENTRY(ctrlIdx, localAddrId, localIpAddrPtr);
  if(TcpIp_Ipv6_isAllNodesMulticastIpAddr(localIpAddrPtr))
  {
    retVal = TRUE;
  }
  TS_PARAM_UNUSED(ctrlIdx);
  TS_PARAM_UNUSED(localAddrId);
  DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_EXIT(retVal, ctrlIdx, localAddrId, localIpAddrPtr);
  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr
)
{
  CONSTP2CONST(TcpIp_ctrlIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV6);
  DBG_TCPIP_IPV6_GETCTRLADDRRANGE_ENTRY(ctrlIdx, startIndexPtr, endIndexPtr);
  *startIndexPtr = ctrlPtr->intLocalAddrIdFirst;
  *endIndexPtr = *startIndexPtr + ctrlPtr->intLocalAddrNum;
  DBG_TCPIP_IPV6_GETCTRLADDRRANGE_EXIT(ctrlIdx, startIndexPtr, endIndexPtr);
}
#endif /* TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON */


/*==================[internal function definitions]=========================*/

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV6_checkHeaderOrder
(
  uint8 nextHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) position
)
{
  uint8 i;
  TcpIp_Ip_RxReturnType result = TCPIP_IP_RX_NOT_OK;

  /* first check if next header is upper layer protocol (most common case) */
#if(TCPIP_UDP_ENABLED == STD_ON)
  if(nextHeader == TCPIP_IP_PROTOCOL_UDP)
  {
    result = TCPIP_IP_RX_EXT_HDR_OK;
  }
  else
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
#if(TCPIP_TCP_ENABLED == STD_ON)
  if(nextHeader == TCPIP_IP_PROTOCOL_TCP)
  {
    result = TCPIP_IP_RX_EXT_HDR_OK;
  }
  else
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
  if ((nextHeader == TCPIP_IP_PROTOCOL_ICMPV6) ||
      (nextHeader == TCPIP_IP_EXT_HDR_NONEXTHDR)
     )
  {
    result = TCPIP_IP_RX_EXT_HDR_OK;
  }
  else /* check extension header order */
  {
    /* loop over all allowed headers */
    for(i = *position; i < sizeof(TcpIp_IPV6_HeaderOrderList); i++)
    {
      /* if known header is found */
      if(nextHeader == TcpIp_IPV6_HeaderOrderList[i])
      {
        /* check if header is allowed at this position */
        /* if header is allowed mark which headers are allowed afterwards */
        *position = i + 1U;
        result = TCPIP_IP_RX_EXT_HDR_OK;
        break;
      }
    }

    if(result != TCPIP_IP_RX_EXT_HDR_OK)
    {
      /* header is not allowed at this position discard */
      if(nextHeader == TCPIP_IP_EXT_HDR_HOPBYHOP)
      {
        /* invalid hop by hop option header detected, discard packet and send
           unrecognized header error message */
        result = TCPIP_IP_RX_PROTOCOL_UNREACHABLE;
      }
      else /* check if header is known */
      {
        /* loop over all headers that are known */
        for(i = 0U ; i < *position; i++)
        {
          /* if known header is found */
          if(nextHeader == TcpIp_IPV6_HeaderOrderList[i])
          {
            /* header is not allowed at this position discard */
            result = TCPIP_IP_RX_NOT_OK;
            break;
          }
        }

        if(i == *position)
        {
          /* unknown header detected, send unrecognized header error message */
          result = TCPIP_IP_RX_PROTOCOL_UNREACHABLE;
        }
      }
    }
  }

  return result;
}

#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )

/** \brief parse the IPv6 specifics and handle v6 specific errors, but return generic
 *  error codes (from TcpIp_Int.h), that are interpreted in \a TcpIp_IpV6_processNextHeader */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV6_parseFragmentHeader
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) dstAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) fragHeaderPtr,
  uint16 lenByte
)
{
  TcpIp_Ip_RxReturnType result;
  /* too short E_TRUNCATED */
  if (lenByte < TCPIP_IPV6_FRAGMENT_HEADER_LENGTH)
  {
    result= TCPIP_IP_RX_FRAGMENT_HEADER_SHORT;
  }
  else
  {
    uint16 fragmentInfo= TCPIP_GET16(fragHeaderPtr, 2);

    TS_MemSet32_NoCheck(fragPtr, 0U, sizeof(*fragPtr));

    /* check TcpIpIpNumFragments, TcpIpIpV6ReassemblyTimeout, ID, FragmentOff%8==0 || more==false */
    TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD(&fragPtr->channelId.srcAddr, srcAddrPtr);
    TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD(&fragPtr->channelId.dstAddr, dstAddrPtr);
    TS_MemCpy(&fragPtr->channelId.identification, &fragHeaderPtr[4U], 4U);

    /* change view to payload, but leave space for the header */
    fragPtr->dataPtr= &fragHeaderPtr[TCPIP_IPV6_FRAGMENT_HEADER_LENGTH];
    fragPtr->lenDataByte= lenByte-TCPIP_IPV6_FRAGMENT_HEADER_LENGTH;

    /* so we can do the same with the reassembly-buffer later */
    fragPtr->lenV46Header= TCPIP_IPV6_HEADER_LENGTH;
    fragPtr->offset= fragmentInfo & 0xFFF8U;
    fragPtr->moreFrags = (uint8)(fragmentInfo & 1U);

    /* the NextHeader field of the offset0 Fragment is dominant */
    if (fragPtr->offset==0u)
    {
      result = TCPIP_IP_RX_OK;
      fragPtr->nextHeader= fragHeaderPtr[0];
      if (fragPtr->nextHeader==0u)
      {
        /* if NextHeader is zero, send ICMP error */
        result= TCPIP_IP_RX_PROTOCOL_UNREACHABLE_FRAG;
      }
    }
    else
    {
      result = TCPIP_IP_RX_OK;
    }
  }
  return result;
}

STATIC FUNC( TcpIp_Ip_RxReturnType , TCPIP_CODE ) TcpIp_IpV6_rxExtHeaderFragment
(
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) dstAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 NextHeaderOffset,
  boolean IsBroadcast
)
{
  TcpIp_Ip_Frag_RxHeaderType frag;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) nextHeaderPtr = &dataPtr[NextHeaderOffset];
  uint16 payloadLen = dataLen - NextHeaderOffset;

  TcpIp_Ip_RxReturnType result =
    TcpIp_IpV6_parseFragmentHeader
    (
      &frag,
      srcAddrPtr->addr,
      dstAddrPtr,
      nextHeaderPtr,
      payloadLen
    );

  if (result == TCPIP_IP_RX_OK)
  {
    result = TcpIp_Ip_Frag_reassemble(reAssembleItemPtrPtr, &frag);

    {
      CONSTP2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtr = *reAssembleItemPtrPtr;

      if (reAssembleItemPtr != NULL_PTR) /* not an abnormal situation like OutOfBuffers */
      {
        /* use Offset0 fragment's IPV6 header and NextHeader field */
        if (frag.offset==0u)
        {
          reAssembleItemPtr->headerStart= TCPIP_IP_MAX_HEADERS_LENGTH-frag.lenV46Header;
          TS_MemCpy( &reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart],
                     &dataPtr[0],
                     frag.lenV46Header);
          TCPIP_IPV6_SET_NEXTHEADER(&reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart],
                                    frag.nextHeader);
        }

        switch (result)
        {
          case TCPIP_IP_RX_OK: /* buffer _is_ complete */
            /* Reassembling finished */
            /* Deviation MISRAC2012-2 */
            TCPIP_IPV6_SET_PAYLOADLENGTH(&reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart],
                                         (uint16)reAssembleItemPtr->size);
            /* we got the full message -> we can fall through & continue parsing headers.
             * BUT we're not seeing the first header any more, so a nextHeader of 0 means discard */
          break;
          case TCPIP_IP_RX_FRAGMENT_TIMEOUT:
            /* do not report timeouts, if fragment0 is missing - ignore silently */
            result= TCPIP_IP_RX_NOT_OK;
            TcpIp_Ip_Frag_returnBuffer(reAssembleItemPtr);
            *reAssembleItemPtrPtr= NULL_PTR;
          break;
          case TCPIP_IP_RX_NOT_OK:
            /* unfinished buffer */
            /* don't pass the buffer, or it will be returned */
            *reAssembleItemPtrPtr= NULL_PTR;
          break;
          /* supply these errors upstream */
          case TCPIP_IP_RX_FRAGMENT_OFFSET:
            /* reserved field set to offset of 'offset' field */
            TcpIp_IpV6_sendError
               (
                srcAddrPtr,
                localAddrId,
                &dataPtr[0],
                dataLen,
                (uint32)NextHeaderOffset + (uint32)2U,
                TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
                TCPIP_IPV6_ICMP_CODE_PARAM_PROB_ERROR_HDR,
                IsBroadcast
               );
          break;
          case TCPIP_IP_RX_FRAGMENT_LENGTH:
            /* reserved field set to offset of 'length' field in IPv6 header */
            TcpIp_IpV6_sendError
               (
                srcAddrPtr,
                localAddrId,
                &dataPtr[0],
                dataLen,
                4U,
                TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
                TCPIP_IPV6_ICMP_CODE_PARAM_PROB_ERROR_HDR,
                IsBroadcast
               );
          break;
          case TCPIP_IP_RX_FRAGMENT_OVERLAP:
          break;
          /* CHECK: NOPARSE */
          default:
            TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
          break;
          /* CHECK: PARSE */
        }
      }
    }
  }
  else if(result == TCPIP_IP_RX_PROTOCOL_UNREACHABLE_FRAG)
  {
    /* pointer to the unrecognized Next header, starting from the ipv6 header */
    TcpIp_IpV6_sendError
       (
        srcAddrPtr,
        localAddrId,
        &dataPtr[0],
        dataLen,
        NextHeaderOffset,
        TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
        TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_HDR,
        IsBroadcast
       );
  }
  else
  {
    /* discard fragment */
  }

  return result;
}
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_processNextHeader
(
  P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  TcpIp_IpAddrConstPtr destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint8 HopLimit,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtrIn,
  uint16 dataLenIn,
  boolean IsBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  /* fragmentation needs to redirect to reassembly buffer */
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr = dataPtrIn;
  uint16 dataLen = dataLenIn;
  TcpIp_Ip_RxReturnType RxRetCode = TCPIP_IP_RX_EXT_HDR_OK;
  uint8 position = 0U;
  boolean isFragmented = FALSE;
#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtr = NULL_PTR;
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

  uint16 nextHeaderFieldOffset = TCPIP_IPV6_HDR_NXT_HDR_OFFSET;
  uint16 nextHeaderDataOffset = TCPIP_IPV6_HEADER_LENGTH;
  uint8  nextHeader = TCPIP_IPV6_GET_NEXTHEADER(dataPtr);
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  uint8 ipsecSaIdx = TCPIP_IPSECDB_INVALID_SA_INDEX;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

  /* until we reach the last header */
  do
  {
    uint16 currentHeaderFieldOffset = nextHeaderFieldOffset;
    uint16 currentHeaderDataOffset = nextHeaderDataOffset;
    uint8 currentHeader = nextHeader;

    /* If there is a next header there should be at least two bytes left of data */
    if(
        (currentHeader != TCPIP_IP_EXT_HDR_NONEXTHDR) &&
        ((currentHeaderDataOffset + TCPIP_IPV6_EXT_HDR_LENGTH_OFFSET) >= dataLen)
      )
    {
      RxRetCode = TCPIP_IP_RX_NOT_OK;
    }
    else
    {
      /* check header order */
      RxRetCode = TcpIp_IpV6_checkHeaderOrder(currentHeader, &position);

      if(RxRetCode == TCPIP_IP_RX_EXT_HDR_OK)
      {
        P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) nextHeaderPtr = &dataPtr[currentHeaderDataOffset];
        uint16 payloadLen = dataLen - currentHeaderDataOffset;

#if (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON)
       if(!TcpIp_IpV6_getExtensionHeaderFilter(currentHeader, localAddrId))
       {
         RxRetCode = TCPIP_IP_RX_NOT_OK;
       }
       else
#endif /* (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON) */
       {
         switch(currentHeader)
         {
#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )
           case TCPIP_IP_PROTOCOL_IPV6FRAG:
           {
             /* fragmented header available */
             RxRetCode=
               TcpIp_IpV6_rxExtHeaderFragment
                   (
                    &reAssembleItemPtr,
                    localAddrId,
                    srcAddrPtr,
                    destIpPtr,
                    dataPtr,
                    dataLen,
                    currentHeaderDataOffset,
                    IsBroadcast
                   );

             if (RxRetCode == TCPIP_IP_RX_OK)
             {
               isFragmented = TRUE;
               dataPtr = &reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart];
               dataLen = (uint16) reAssembleItemPtr->size + TCPIP_IPV6_HEADER_LENGTH;
               nextHeaderFieldOffset = TCPIP_IPV6_HDR_NXT_HDR_OFFSET;
               nextHeaderDataOffset = TCPIP_IPV6_HEADER_LENGTH;
               nextHeader = TCPIP_IPV6_GET_NEXTHEADER(dataPtr);
               position = 4U;
               RxRetCode = TCPIP_IP_RX_EXT_HDR_OK;
               continue;
             }
             break;
           }
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */
           case TCPIP_IP_EXT_HDR_HOPBYHOP:
           {
             /* Hop-By-Hop Options only allowed once and only in IPv6 header */
             RxRetCode =
               TcpIp_IpV6_rxExtHeaderHopByHop
                 (srcAddrPtr, localAddrId, dataPtr, dataLen, currentHeaderDataOffset, IsBroadcast);

             break;
           }
           case TCPIP_IP_EXT_HDR_DSTOPT:
           {
             /* Destination Options allowed twice */
             RxRetCode =
               TcpIp_IpV6_rxExtHeaderDestinationOption
                 (srcAddrPtr, localAddrId, dataPtr, dataLen, currentHeaderDataOffset, IsBroadcast);

             break;
           }
           case TCPIP_IP_EXT_HDR_ROUTING:
           {
             if(TCPIP_IPV6_EXT_HDR_SEGMENTS_LEFT + currentHeaderDataOffset >= dataLen)
             {
               RxRetCode = TCPIP_IP_RX_NOT_OK;
             }
             else if(dataPtr[currentHeaderDataOffset + TCPIP_IPV6_EXT_HDR_SEGMENTS_LEFT] == 0U)
             {
               /* Node has ignored Routing Header and proceeded to the next header */
               RxRetCode = TCPIP_IP_RX_EXT_HDR_OK;
             }
             else
             {
               /* Node has discarded packet and sent an ICMP Parameter Problem */
               TcpIp_IpV6_sendError
                 (
                  srcAddrPtr,
                  localAddrId,
                  dataPtr,
                  dataLen,
                  (uint32)currentHeaderDataOffset + (uint32)TCPIP_IPV6_EXT_HDR_ROUTING_TYPE,
                  TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
                  TCPIP_IPV6_ICMP_CODE_PARAM_PROB_ERROR_HDR,
                  IsBroadcast
                 );
               RxRetCode = TCPIP_IP_RX_UNRECOGNIZED_ROUTING_TYPE;
             }
             break;
           }
#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
           case TCPIP_IP_EXT_HDR_AUTHENTICATION:
           {
             RxRetCode = TcpIp_IpV6_RxAhHeader( localAddrId,
                                                srcAddrPtr->addr,
                                                dataPtr,
                                                dataLen,
                                                currentHeaderDataOffset,
                                                &ipsecSaIdx
                                              );
             break;
           }

#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)) */
           /* No Next Header */
           case TCPIP_IP_EXT_HDR_NONEXTHDR:
             RxRetCode = TCPIP_IP_RX_OK;
             break;
           default:
           {
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
             if(TcpIp_IpSecDb_checkSecurityRxPolicy(localAddrId,
                                                    srcAddrPtr->addr,
                                                    nextHeaderPtr,
                                                    payloadLen,
                                                    currentHeader,
                                                    ipsecSaIdx,
                                                    physAddrPtr
                                                   ) == E_OK
               )
#else /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
             TS_PARAM_UNUSED(physAddrPtr);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
             {
               RxRetCode =
                 TcpIp_IpV6_passToUpperLayer
                       (
                         srcAddrPtr,
                         destIpPtr,
                         localAddrId,
                         currentHeader,
                         nextHeaderPtr,
                         payloadLen,
                         isFragmented,
                         HopLimit
                       );

#if (TCPIP_UDP_ENABLED == STD_ON)
               if(RxRetCode == TCPIP_IP_RX_PORT_UNREACHABLE)
               {
                 /* no matching socket connection found */
                 TcpIp_IpV6_sendError
                   (
                     srcAddrPtr,
                     localAddrId,
                     dataPtr,
                     dataLen,
                     0U,
                     TCPIP_IPV6_ICMP_TYPE_DST_UNREACH,
                     TCPIP_IPV6_ICMP_CODE_DST_UNREACH,
                     IsBroadcast
                   );
               }
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
             }
             break;
           }
         }
       }
      }
      else
      {
        if(RxRetCode == TCPIP_IP_RX_PROTOCOL_UNREACHABLE)
        {
          /* unrecognized header detected, discard packet */
          TcpIp_IpV6_sendError
            (
             srcAddrPtr,
             localAddrId,
             dataPtr,
             dataLen,
             currentHeaderFieldOffset,
             TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
             TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_HDR,
             IsBroadcast
            );
        }
      }
    }

    if(RxRetCode == TCPIP_IP_RX_EXT_HDR_OK)
    {
      nextHeader = dataPtr[currentHeaderDataOffset];
      nextHeaderDataOffset +=
        Tcpip_IpV6_getNextHeaderOffset(currentHeader, dataPtr, currentHeaderDataOffset);
      nextHeaderFieldOffset = currentHeaderDataOffset;
    }
  } while(RxRetCode == TCPIP_IP_RX_EXT_HDR_OK);

#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )
  /* if a reassembled datagram has been consumed, free reassembly buffer */
  if(reAssembleItemPtr != NULL_PTR)
  {
    /* rx-indication finished */
    TcpIp_Ip_Frag_returnBuffer(reAssembleItemPtr);
  }
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

  return RxRetCode;
}

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_passToUpperLayer
(
  P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  TcpIp_IpAddrConstPtr destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint8 currentHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) nextHeaderPtr,
  uint16 payloadLen,
  boolean isFragmented,
  uint8 HopLimit
)
{
  TcpIp_Ip_RxReturnType RxRetCode = TCPIP_IP_RX_NOT_OK;
  /* Deviation MISRAC2012-2 */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrSockPtr =
    (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) srcAddrPtr;

  switch(currentHeader)
  {
#if (TCPIP_UDP_ENABLED == STD_ON)
    case TCPIP_IP_PROTOCOL_UDP:
    {

      RxRetCode =
        TcpIp_Udp_rxIndication
          (srcAddrSockPtr, destIpPtr, nextHeaderPtr, localAddrId, payloadLen, isFragmented);

      break;
    }
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
#if (TCPIP_TCP_ENABLED == STD_ON)
    case TCPIP_IP_PROTOCOL_TCP:
    {

      TcpIp_Tcp_rxIndication
      (
        srcAddrSockPtr,
        destIpPtr,
        nextHeaderPtr,
        localAddrId,
        payloadLen,
        isFragmented
      );

      RxRetCode = TCPIP_IP_RX_OK;
      break;
    }
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    case TCPIP_IP_PROTOCOL_ICMPV6:
    {
      TcpIp_IpV6_Icmp_rxIndication
      (
        srcAddrPtr,
        destIpPtr,
        nextHeaderPtr,
        localAddrId,
        payloadLen,
        HopLimit,
        isFragmented
      );

      RxRetCode = TCPIP_IP_RX_OK;
      break;
    }
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RxRetCode;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_sendError
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 specificVal,
  uint8 type,
  uint8 code,
  boolean IsBroadcast
)
{
  if( ((TcpIp_PBcfgPtr->ipV6Config.icmpV6MsgParameterProblemEnabled == TRUE) &&
          (type == TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM)
      ) ||
      ((TcpIp_PBcfgPtr->ipV6Config.icmpV6MsgDestinationUnreachableEnabled == TRUE) &&
          (type == TCPIP_IPV6_ICMP_TYPE_DST_UNREACH)
      ) ||
      (type == TCPIP_IPV6_ICMP_TYPE_TIME_EXCEEDED)
    )
  {

    TcpIp_IpAddrM_AddrInfoType const addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);
    if(((TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,UNICAST)) && (!IsBroadcast)) ||
        (code == TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_OPT)
      )
    {
      uint8 const hopLimit = TCPIP_IPV6_ICMP_DEFAULT_HOP_LIMIT;
      const uint16 maxLen = TCPIP_IPV6_MIN_MTU_SIZE -
                          TCPIP_IPV6_HEADER_LENGTH -
                          TCPIP_IPV6_ICMP_HEADER_LENGTH -
                          TCPIP_IPV6_ICMP_ERR_MSG_FIXED_LENGTH;
      uint16 size = dataLen;
      TcpIp_Ip_Icmp_ParameterType icmpParameter;

      icmpParameter.code = code;
      icmpParameter.hopLimit = hopLimit;
      icmpParameter.type = type;
      icmpParameter.specificValue = &specificVal;

      if (dataLen > maxLen)
      {
        /* error message shall include as much of the original packet
        * as possible, but shall not exceed the minimum mtu */
        size = maxLen;
      }

    (void)
      TcpIp_IpV6_Icmp_transmit
      (
        dataPtr,
        size,
        /* Deviation MISRAC2012-2 */
        (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr,
        localAddrId,
        &icmpParameter,
        TRUE
      );
    }
  }
}

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_rxExtHeaderHopByHop
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 NextHeaderOffset,
  boolean IsBroadcast
)
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) nextHeaderPtr = &dataPtr[NextHeaderOffset];
  uint16 payloadLen = dataLen - NextHeaderOffset;
  TcpIp_Ip_RxReturnType RxRetCode;
  uint16 HeaderLength = 8U + ((uint16)nextHeaderPtr[TCPIP_IPV6_EXT_HDR_LENGTH_OFFSET] * 8U);

  if(payloadLen >= HeaderLength)
  {
    uint16 optionIdOffset = TCPIP_IPV6_EXT_HDR_OPTION_HDR_LENGTH;
    RxRetCode = TCPIP_IP_RX_EXT_HDR_OK;

    while((optionIdOffset < HeaderLength) && (RxRetCode == TCPIP_IP_RX_EXT_HDR_OK))
    {
      uint8 const type = nextHeaderPtr[optionIdOffset];

      if(type == TCPIP_IPV6_OPTION_TYPE_PAD1)
      {
        /* padding byte */
        optionIdOffset += 1U;
      }
      else
      {
        uint16 const remainLen = HeaderLength - optionIdOffset;

        /* option must be at least 2 bytes big */
        if(remainLen >= TCPIP_IPV6_EXT_HDR_OPTION_HDR_LENGTH)
        {
          /* get the option length */
          uint16 Length = (uint16) nextHeaderPtr[optionIdOffset + 1U] +
                           TCPIP_IPV6_EXT_HDR_OPTION_HDR_LENGTH;

          /* option length should not exceed the header length */
          if((optionIdOffset + Length) > HeaderLength)
          {
            /* invalid length */
            RxRetCode = TCPIP_IP_RX_NOT_OK;
          }
          else
          {
            if(type != TCPIP_IPV6_OPTION_TYPE_PADN)
            {
              /* option unknown, find out what should happen in this case */
              RxRetCode = TcpIp_IpV6_processUnrecognizedOption(type, localAddrId);

#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
              if((RxRetCode == TCPIP_IP_RX_EXT_HDR_OK) &&
                 (((uint8)(type & TCPIP_IPV6_EXT_HDR_OPTION_CHANGE_EN_ROUTE) >> 5U) == TRUE)
                )
              {
                const uint16 optionLength = (uint16) nextHeaderPtr[optionIdOffset + 1U];
                P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionPtr =
                    &nextHeaderPtr[optionIdOffset + TCPIP_IPV6_EXT_HDR_OPTION_HDR_LENGTH];

                /* option may have changed en-route => set it to zero for AH ICV calculation */
                /* rfc4302:
                 * For any option for which contents may change en route, the entire "Option Data"
                 * field must be treated as zero-valued octets when computing or verifying the ICV.
                 * The Option Type and Opt Data Len are included in the ICV calculation */
                TS_MemBZero(optionPtr, optionLength);
              }
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)) */

              if(RxRetCode == TCPIP_IP_RX_UNRECOGNIZED_OPTION)
              {
                TcpIp_IpV6_sendError
                   (
                    remoteIpAddrPtr,
                    localAddrId,
                    dataPtr,
                    dataLen,
                    (uint32)NextHeaderOffset + (uint32)optionIdOffset,
                    TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM,
                    TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_OPT,
                    IsBroadcast
                   );
              }
            }

            optionIdOffset += Length;
          }
        }
        else
        {
          /* invalid length */
          RxRetCode = TCPIP_IP_RX_NOT_OK;
          break;
        }
      }
    }
  }
  else
  {
    /* invalid length */
    RxRetCode = TCPIP_IP_RX_NOT_OK;
  }

  return RxRetCode;
}

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_processUnrecognizedOption
(
  uint8 type,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_Ip_RxReturnType result = TCPIP_IP_RX_NOT_OK;

  /* get the two most significant bits from the type */
  uint8 const identifier = TCPIP_IPV6_GET_OPTION_ID(type);

  switch(identifier)
  {
    /* 00 */
    case TCPIP_IPV6_IDENTIFIER_SKIP:
    {
      /* option unknown, skip over this option */
      result = TCPIP_IP_RX_EXT_HDR_OK;
      break;
    }
    /* 01 */
    case TCPIP_IPV6_IDENTIFIER_DISCARD:
    {
      /* option unknown, discard packet */
      result = TCPIP_IP_RX_NOT_OK;
      break;
    }
    /* 10 */
    case TCPIP_IPV6_IDENTIFIER_DISCARD_PROBLEM:
    {
      /* option unknown, discard packet and report Parameter Problem */
      result = TCPIP_IP_RX_UNRECOGNIZED_OPTION;
      break;
    }
    /* 11 */
    case TCPIP_IPV6_IDENTIFIER_DISCARD_PROBLEM_MULTI:
    {
      TcpIp_IpAddrM_AddrInfoType const addrInfo =
        TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);

      result = TCPIP_IP_RX_NOT_OK;

      if(!TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
      {
        /* Option unknown, discard packet and report Parameter Problem
           if source address is unicast */
        result = TCPIP_IP_RX_UNRECOGNIZED_OPTION;
      }
      break;
    }
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return result;
}


STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_getTxLocalAddrId
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr,
  TcpIp_IpV6_LocalAddrType preferredAddrIn
)
{
  TcpIp_IpV6_LocalAddrType preferredAddr = preferredAddrIn;
  TcpIp_IpAddrConstPtr preferredNetMask = TcpIp_IPV6_Prefix_Mask;
  CONSTP2CONST(TcpIp_ctrlIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
    &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV6);
    /* Deviation MISRAC2012-2 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_CONST))remoteIpAddrPtr)->addr;

  uint8_least i;
  uint8_least const startIndex = ctrlPtr->intLocalAddrIdFirst;
  uint8_least const endIndex = startIndex + ctrlPtr->intLocalAddrNum;

  /* iterate over all local addresses assigned to this controller */
  for(i = startIndex; i < endIndex; i++)
  {
    TcpIp_IpV6_LocalAddrType localAddr;

    localAddr.localAddrId = TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i);

    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddr.localAddrId));

      /* check for unicast addresses only */
      if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
      {
        TcpIp_IpAddrConstPtr netMaskPtr, gateWayPtr;
        TCPIP_PRECONDITION_ASSERT
          (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddr.localAddrId)).addrType,IPV6),
           TCPIP_INTERNAL_API_ID);

        TcpIp_IpAddrM_getIpConfig(localAddr.localAddrId, &localAddr.ipAddrPtr, &gateWayPtr, &netMaskPtr);

        if(!TCPIP_IPV6_ADDR_IS_UNSPECIFIED(localAddr.ipAddrPtr))
        {
          /* Rule 1: Prefer same address. */
          if(TCPIP_IPV6_ADDR_CMP(IpAddrPtr, localAddr.ipAddrPtr))
          {
            preferredAddr = localAddr;
            break;
          }
          else if(preferredAddr.localAddrId == TCPIP_LOCALADDRID_INVALID)
          {
            /* first assigned unicast address found */
            preferredAddr = localAddr;
            preferredNetMask = netMaskPtr;
          }
          else
          {
            TcpIp_IpV6_LocalAddrType result = {NULL_PTR, TCPIP_LOCALADDRID_INVALID};
            TCPIP_PRECONDITION_ASSERT
            (
              (preferredAddr.ipAddrPtr != NULL_PTR) &&
              (preferredAddr.localAddrId != TCPIP_LOCALADDRID_INVALID),
              TCPIP_INTERNAL_API_ID
             );
            /* Rule 2: Prefer appropriate scope */
            result =
              TcpIp_IpV6_scopeAddrSelection
              (
                preferredAddr,
                localAddr,
                (TcpIp_IpAddrConstPtr)IpAddrPtr
              );

            if(result.localAddrId == TCPIP_LOCALADDRID_INVALID)
            {
              /* Rule 3: Avoid deprecated addresses. */
              result =
                TcpIp_IpV6_stateAddrSelection(preferredAddr, localAddr);

              if(result.localAddrId == TCPIP_LOCALADDRID_INVALID)
              {
                /* Rule 8: Use longest matching prefix. */
                result =
                  TcpIp_IpV6_commonPrefixAddrSelection
                    (preferredAddr, preferredNetMask, localAddr, netMaskPtr, IpAddrPtr);
              }
            }

            /* save the preferred local address */
            if(preferredAddr.localAddrId != result.localAddrId )
            {
              preferredNetMask = netMaskPtr;
              preferredAddr = result;
            }
          }
        }
      }
    }
  }

  return preferredAddr;
}

STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_scopeAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2,
  TcpIp_IpAddrConstPtr remoteIpAddr
)
{
  TcpIp_IpV6_LocalAddrType result;
  uint8 localAddrScope_1 = TcpIp_IpV6_getAddrScope(localIpAddr_1.ipAddrPtr);
  uint8 localAddrScope_2 = TcpIp_IpV6_getAddrScope(localIpAddr_2.ipAddrPtr);
  uint8 remoteAddrScope = TcpIp_IpV6_getAddrScope(remoteIpAddr);

  result.localAddrId = TCPIP_LOCALADDRID_INVALID;
  result.ipAddrPtr = NULL_PTR;

  /* check if the scope is the same */
  if(localAddrScope_1 != localAddrScope_2)
  {
    if(localAddrScope_1 < localAddrScope_2)
    {
      if(localAddrScope_1 < remoteAddrScope)
      {
        /* use 2 */
        result = localIpAddr_2;
      }
      else
      {
        /* use 1 */
        result = localIpAddr_1;
      }
    }
    else
    {
      if(localAddrScope_2 < remoteAddrScope)
      {
        /* use 1 */
        result = localIpAddr_1;
      }
      else
      {
        /* use 2 */
        result = localIpAddr_2;
      }
    }
  }

  return result;
}

STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_stateAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2
)
{
  TcpIp_IpV6_LocalAddrType result;
  uint8 state_1 = TcpIp_IpAddrM_getIpAddrState(localIpAddr_1.localAddrId);
  uint8 state_2 = TcpIp_IpAddrM_getIpAddrState(localIpAddr_2.localAddrId);

  result.localAddrId = TCPIP_LOCALADDRID_INVALID;
  result.ipAddrPtr = NULL_PTR;

  TCPIP_PRECONDITION_ASSERT
    ((state_1 == TCPIP_IPADDRM_ADDRSTATE_PREFERRED) ||
     (state_1 == TCPIP_IPADDRM_ADDRSTATE_DEPRECATED) ||
     (state_1 == TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC),
     TCPIP_INTERNAL_API_ID
    );
  TCPIP_PRECONDITION_ASSERT
    ((state_2 == TCPIP_IPADDRM_ADDRSTATE_PREFERRED) ||
     (state_2 == TCPIP_IPADDRM_ADDRSTATE_DEPRECATED) ||
     (state_2 == TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC),
     TCPIP_INTERNAL_API_ID
    );

  /* Check if the states are not equal and at least one is preferred */
  if
  (
    (state_1 != state_2) &&
    ((state_1 == TCPIP_IPADDRM_ADDRSTATE_PREFERRED) || (state_2 == TCPIP_IPADDRM_ADDRSTATE_PREFERRED))
  )
  {
    /* state of ip addresses are not equal.
     * States to be considered by priority are:
     * 1. preferred
     * 2. optimistic/deprecated
     * 3. tentative - they haven't ip so they won't be checked
       return preferred one */
    if(state_1 == TCPIP_IPADDRM_ADDRSTATE_PREFERRED)
    {
      result = localIpAddr_1;
    }
    else
    {
      result = localIpAddr_2;
    }
  }

  return result;
}

STATIC FUNC(TcpIp_IpV6_LocalAddrType, TCPIP_CODE) TcpIp_IpV6_commonPrefixAddrSelection
(
  TcpIp_IpV6_LocalAddrType localIpAddr_1,
  TcpIp_IpAddrConstPtr netMask_1,
  TcpIp_IpV6_LocalAddrType localIpAddr_2,
  TcpIp_IpAddrConstPtr netMask_2,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr
)
{
  TcpIp_IpV6_LocalAddrType result = localIpAddr_1;
  uint8 i;

  for(i = 0; i < TCPIP_IPV6_ADDR_SIZE; i++)
  {
    const uint32 localNetworkId_1 = localIpAddr_1.ipAddrPtr[i] & netMask_1[i];
    const uint32 destinationNetworkId_1 = remoteIpAddr[i] & netMask_1[i];
    const uint32 match_1 =
      COMSTACK_HTON_UINT32(localNetworkId_1) ^ COMSTACK_HTON_UINT32(destinationNetworkId_1);
    const uint32 localNetworkId_2 = localIpAddr_2.ipAddrPtr[i] & netMask_2[i];
    const uint32 destinationNetworkId_2 = remoteIpAddr[i] & netMask_2[i];
    const uint32 match_2 =
      COMSTACK_HTON_UINT32(localNetworkId_2) ^ COMSTACK_HTON_UINT32(destinationNetworkId_2);

    /* if prefix of both addresses do not match */
    if((match_1 != 0U) || (match_2 != 0U))
    {
      /* the smallest value of match has the highest prefix match */
      if((match_2) < (match_1))
      {
        result = localIpAddr_2;
      }
      break;
    }
  }

  return result;
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_getAddrScope
(
  TcpIp_IpAddrConstPtr ipAddr
)
{
  uint8 scope;

  if(TCPIP_IPV6_ADDR_IS_MULTICAST(ipAddr))
  {
    /* if address is a multicast address (remote address only), get the scope
     * from the scope field */
    scope = TCPIP_GET_IPV6_MULTICAST_SCOPE(ipAddr);
  }
  else
  {
    if(TCPIP_IPV6_ADDR_IS_LINK_LOCAL(ipAddr))
    {
      scope = TCPIP_IPV6_SCOPE_LINK_LOCAL;
    }
    else
    {
      /* any other addresses are treated as global */
      scope = TCPIP_IPV6_SCOPE_GLOBAL;
    }
  }

  return scope;
}

#if (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON)
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_getExtensionHeaderFilter
(
  uint8 currentHeader,
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean headerIsAllowed = FALSE;

  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
          &(TCPIP_CFG(localAddr,localAddrId));

  if(localAddrPtr->ipV6ExtensionHeaderIdListNum == 0U)
  {
    headerIsAllowed = TRUE;
  }
  else
  {
    uint16 i;

    for(i = localAddrPtr->ipV6ExtensionHeaderIdListOffset;
        i<(localAddrPtr->ipV6ExtensionHeaderIdListOffset+localAddrPtr->ipV6ExtensionHeaderIdListNum);
        i++
       )
    {
      if(currentHeader ==  (TCPIP_CFG_TYPE(uint8,ipV6ExtensionHeaderId,i)))
      {
        headerIsAllowed = TRUE;
        break;
      }
    }
  }
  return headerIsAllowed;
}
#endif /* (TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE == STD_ON) */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetInvalidIpAddrCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIpAddrCount,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpV6_InvalidIpAddrCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIpAddrCount,1 */
    TS_AtomicAssign32(TcpIp_IpV6_InvalidIpAddrCounter, 0U);
  }

}

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

STATIC FUNC(uint16, TCPIP_CODE) Tcpip_IpV6_getNextHeaderOffset
(
  uint8 currentHeader,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 currentHeaderDataOffset
)
{
  uint16 offset =
    (uint16)dataPtr[currentHeaderDataOffset + TCPIP_IPV6_EXT_HDR_LENGTH_OFFSET];

  if(currentHeader == TCPIP_IP_EXT_HDR_AUTHENTICATION)
  {
    /* length is calculated differently for authentication header than for others */
    offset = (offset + 2U) * 4U;
  }
  else
  {
    offset = 8U + offset * 8U;
  }

  return offset;
}

#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV6_RxAhHeader
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint16 currentHeaderDataOffset,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipsecSaIdx
)
{
  TcpIp_Ip_RxReturnType retVal = TCPIP_IP_RX_NOT_OK;
  Std_ReturnType returnValue = E_NOT_OK;
  uint8 protocol = 0U;

  /* zero out mutable fields */
  TCPIP_IPSEC_IPV6_ZEROOUT_MUTABLE(dataPtr);

  returnValue = TcpIp_IpSec_ahRxIndication( &dataPtr,
                                            &dataLen,
                                            &protocol,
                                            currentHeaderDataOffset,
                                            localAddrId,
                                            remoteAddrPtr,
                                            ipsecSaIdx
                                          );

  if (returnValue == E_OK)
  {
    /* integrity is ok, continue with the next header */
    retVal = TCPIP_IP_RX_EXT_HDR_OK;
  }

  return retVal;
}
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else /* (TCPIP_IPV6_ENABLED == STD_ON) */
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
