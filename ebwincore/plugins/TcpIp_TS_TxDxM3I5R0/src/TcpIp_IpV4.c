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
 * 1)AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 2)Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 *
 * MISRAC2012-5) Deviated Rule: 12.2 (Required)
 * The right hand operand of a shift operator shall lie in the range zero to one less than the
 * width in bits of the essential type of the left hand operand.
 *
 * Reason:
 * If a cast to the correct range was done before the shift, then this rule is a false
 * positive.
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
#include <TcpIp_IpV4_Int.h>       /* own SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>    /* used SW-unit interface file */
#include <TcpIp_IpSec_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpV4_Priv.h>      /* own SW-unit private header file; must be last */


/*==================[macros]================================================*/

/** \brief macro returns TRUE payload is long enough to contain the minimum IPV4 header */
#define TCPIP_IPV4_HEADER_MINIMUM_CONTAINED(length)   ((length) >= TCPIP_IPV4_HEADER_MINIMUM_LENGTH)

/** \brief minimum IPV4 header length in units of bytes */
#define TCPIP_IPV4_HEADER_MINIMUM_LENGTH   20U

/** \brief minimum IPV4 payload length = ICMP error response length */
#define TCPIP_IPV4_ICMP_ERROR_PAYLOAD_LENGTH 8U

/** \brief Default Maximum Segment Size */
#define TCPIP_IPV4_MIN_MTU_SIZE           576U

/** \brief Default Maximum Segment Size */
#define TCPIP_ETH_BROADCAST_ADDRESS           {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU }

/** \brief macro returns the header length in bytes */
#define TCPIP_IPV4_IHL_BYTES_GET(headerPtr)   ((((headerPtr)[0])&0x0FU)*4U)

/** \brief macro returns IP header TTL field */
#define TCPIP_IPV4_TTL_GET(headerPtr)   ((headerPtr)[8])

/** \brief macro returns protocol number out of Ip header */
#define TCPIP_IPV4_PROTOCOL_GET(headerPtr)   ((headerPtr)[9])

/** \brief macro returns TRUE if this is a IPV4 header */
#define TCPIP_IPV4_VERSION_IS_IPV4(headerPtr)   ((((headerPtr)[0])&0xF0U) == 0x40U)

/** \brief macro returns IPV4 datagram length in units of bytes (from header) */
#define TCPIP_IPV4_TOTAL_LENGTH_BYTES_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 2)

/** \brief macro returns source Ip address (from header) */
#define TCPIP_IPV4_SRC_ADDR_GET(headerPtr,ipAddrPtr) \
  TCPIP_GETCPY32(headerPtr, 12, ipAddrPtr)

/** \brief macro returns destination Ip address (from header) */
#define TCPIP_IPV4_DEST_ADDR_GET(headerPtr,ipAddrPtr) \
  TCPIP_GETCPY32(headerPtr, 16, ipAddrPtr)

/** \brief macro returns datagram fragmentation information (from header), ignoring the DF flag */
#define TCPIP_IPV4_FRAGMENTINFO_GET(headerPtr)\
    (TCPIP_GET16(headerPtr, 6) & 0xBFFFU)

/** \brief macro that checks return flag */
#define TCPIP_IPV4_RESERVED_FLAG_NOT_0(fragmentInfo)\
    (((fragmentInfo >> 15u) & 1u) != 0U)

/** \brief macro that checks if fragmentation is needed */
#define TCPIP_IPV4_IS_FRAGMENTATION_NEEDED(fragmentInfo)\
    (((fragmentInfo) & 0x3fFFu) != 0U)

/** \brief macro returns the IDENTIFICATION field from the IPv4 header. */
#define TCPIP_IPV4_IDENTIFICATION_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 4)

/** \brief macro sets IP header IHL field (pass header length value in units of bytes) */
#define TCPIP_IPV4_IHL_BYTES_SET(headerPtr,length) (((headerPtr)[0]) = (0x40U|(((length)/4U)&0xFU)))

/** \brief macro sets IP header TOS field */
#define TCPIP_IPV4_TOS_SET(headerPtr,tos)   (((headerPtr)[1]) = (tos))

/** \brief macro sets IP header DSCP field */
#define TCPIP_IPV4_DSCP_BYTES_SET(headerPtr, dscp)\
  (headerPtr)[1U] |= ((dscp) << 2U) & 0xFCU

/** \brief macro sets IPv4 header TOTAl LENGTH field (pass \a length value in units of bytes)*/
#define TCPIP_IPV4_TOTAL_LENGTH_BYTES_SET(headerPtr,length) \
  TCPIP_SET16(headerPtr,2,length)

/** \brief macro sets IP header IDENTIFICATION field */
#define TCPIP_IPV4_IDENTIFICATION_SET(headerPtr,ident) \
  TCPIP_SET16((headerPtr), 4, (ident))

/** \brief macro sets the datagram fragmentation information (from header) */
#define TCPIP_IPV4_FRAGMENTINFO_SET(headerPtr, fragmentInfo)\
    TCPIP_SET16(headerPtr, 6, fragmentInfo)

/** \brief macro sets IP header TTL field */
#define TCPIP_IPV4_TTL_SET(headerPtr,ttl)   (((headerPtr)[8]) = (ttl))

/** \brief macro sets IP header PROTOCOL field */
#define TCPIP_IPV4_PROTOCOL_SET(headerPtr,protocol)   (((headerPtr)[9]) = (protocol))

/** \brief macro sets IP header CHECKSUM field */
#define TCPIP_IPV4_CHECKSUM_SET(headerPtr,checksum) \
  TCPIP_SET16(headerPtr,10,checksum)

/** \brief macro sets IP header SRC ADDRESS field */
#define TCPIP_IPV4_SRC_ADDR_SET(headerPtr,ipAddr)  \
  TCPIP_SETCPY32(headerPtr,12,ipAddr)

/** \brief macro sets IP header DESTINATION ADDRESS field */
#define TCPIP_IPV4_DEST_ADDR_SET(headerPtr,ipAddr)  \
  TCPIP_SETCPY32(headerPtr,16,ipAddr)

/** \brief macro sets mutable fields of IPv4 header to zero */
#define TCPIP_IPV4_ZEROOUT_MUTABLE(headerPtr)                       \
  do                                                                \
  {                                                                 \
    TCPIP_IPV4_TOS_SET((headerPtr), 0U);                            \
    TCPIP_IPV4_FRAGMENTINFO_SET((headerPtr), 0U);                   \
    TCPIP_IPV4_TTL_SET((headerPtr), 0U);                            \
    TCPIP_IPV4_CHECKSUM_SET((headerPtr), 0U);                       \
  } while (0U)

/** \brief macro which creates immutable ipv4 header  */
#define TCPIP_IPV4_CREATE_IMMUTABLE_HDR(pdata, payloadLen, id, nextHdr, remoteAddr, srcAddr)\
  do                                                                              \
  {                                                                               \
    TCPIP_IPV4_IHL_BYTES_SET((pdata), TCPIP_IPV4_HEADER_MINIMUM_LENGTH);          \
    TCPIP_IPV4_TOS_SET((pdata), 0U);                                              \
    TCPIP_IPV4_TOTAL_LENGTH_BYTES_SET((pdata), payloadLen);                       \
    TCPIP_IPV4_IDENTIFICATION_SET((pdata), ((uint16)(id)));                       \
    TCPIP_IPV4_FRAGMENTINFO_SET((pdata), 0U);                                     \
    TCPIP_IPV4_TTL_SET((pdata), 0U);                                              \
    TCPIP_IPV4_PROTOCOL_SET((pdata), (nextHdr));                                  \
    TCPIP_IPV4_CHECKSUM_SET((pdata), 0U);                                         \
    TCPIP_IPV4_SRC_ADDR_SET((pdata), srcAddr);                                    \
    TCPIP_IPV4_DEST_ADDR_SET((pdata), remoteAddr);                                \
  } while (0U)

/* IPv4 End of Options List option */
#define TCPIP_IPSEC_IPV4_EOOL                                0U
/* IPv4 No Operation option */
#define TCPIP_IPSEC_IPV4_NOP                                 1U
/* IPv4  Security option */
#define TCPIP_IPSEC_IPV4_SEC                               130U
/* IPv4 Extended Security option */
#define TCPIP_IPSEC_IPV4_E_SEC                             133U
/* IPv4 Commercial Security option */
#define TCPIP_IPSEC_IPV4_CIPSO                             134U
/* IPv4 Router Alert option */
#define TCPIP_IPSEC_IPV4_RA                                148U
/* IPv4 Sender Directed Multi-Destination Delivery option */
#define TCPIP_IPSEC_IPV4_SDBM                              149U

/*==================[type definitions]======================================*/

typedef struct
{
  uint32 destIp;
  P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) srcIp;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) payloadPtr;
  uint16 payloadLen;
  uint8 protocol;
  uint8 ttl;
  uint16 lenByte;
} TcpIp_IpV4_HeaderType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV4_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Checks the received IPV4 header for validity
 *
 * \param[in] dataPtr           pointer to start of IPV4 header
 * \param[in] lenByte           datagram length received (size of buffer starting at dataPtr)
 * \param[in] payloadLengthPtr  effective payload length of
 * \param[in] ctrlIdx           controller index
 *
 * \return Length of header in units of bytes, 0 if header is invalid
 */
STATIC FUNC(uint8, TCPIP_CODE ) TcpIp_IpV4_checkValidHeader
(
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
    uint16 lenByte,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLengthPtr,
    uint8 ctrlIdx
);

/** \brief Checks the IPV4 address for being a valid unicast address
 *
 * \param[in] srcAddr       IP address to check
 * \param[in] netMask       netmask to check srcAddr against
 *
 * \return result of check
 * \retval TRUE source address is valid
 * \retval FALSE source address is invalid
 */
STATIC FUNC(boolean, TCPIP_CODE ) TcpIp_IpV4_checkUnicastAddr
(
    uint32 srcAddr,
    uint32 netMask
);

/** \brief Checks host fraction of the IPV4 address for being invalid (all bits 0 or 1)
 *
 * \param[in] srcAddr       IP address to check
 * \param[in] netMask       netmask to check srcAddr against
 *
 * \return result of check
 * \retval TRUE host part source address is invalid
 * \retval FALSE source address is valid
 */
STATIC FUNC(boolean, TCPIP_CODE ) TcpIp_IpV4_isHostIdInvalid
(
    uint32 srcAddr,
    uint32 netMask
);

/** \brief Performs IpV4 header construction
 *
 * This function constructs the Ip header.
 *
 * \param[in,out] ipHeaderPtr     Pointer to the IP header construction area.
 * \param[in] ipParameterPtr      Points to a structure containing Ip communication parameters
 * \param[in] fragHdrPtr          All info related to length, IP fragmentation & IP header ID.
 * \param[in] localIpAddrPtr      Pointer to local address used for transmission
 * \param[in] destIp              The remote address information
 * \param[in] ipLength            Ip length to transmit in units of bytes.
 * \param[in] ctrlIdx             controller index.
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_buildHeader
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipHeaderPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  TcpIp_IpAddrConstPtr localIpAddrPtr,
  uint32 destIp,
  uint16 ipLength,
  uint8 ctrlIdx
);

/** \brief Checks whether communication from \p srcAddr to \p destAddr is subnet-local.
 *
 * \param[in] ctrlIdx            controller index
 * \param[in] srcAddr            source Ip address
 * \param[in] destAddr           destination Ip address
 * \param[out] remoteNetMaskPtr  Set to netmask of the source and destination, if communication is
 *                               subnet local; otherwise set to 0U.
 *
 * \retval TRUE if communication from \srcAddr to \p destAddr is subnet-local.
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isSubnetCommunication
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrInetType,AUTOMATIC,TCPIP_APPL_DATA) srcAddr,
  uint32 destAddr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) remoteNetMaskPtrPtr
);

/** \brief If we have a gateway we evaluate the NetId
 *
 * \param[in] nextHopIpPtr        pointer to nextHopIp
 * \param[in] networkMask         networkMask
 * \param[in] localIpAddr         localIpAddr
 * \param[in] destIpPtr           Points to the remote IP address information
 * \param[in] remoteAddrHandle    pointer to remote physical address = multicast address
 *
 * \retval TCPIP_OK               Next hop ip address successfully evaluated
 * \retval TCPIP_E_NOT_OK         if next Hop destination can't be evaluated
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_getNextHopIp
(
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) nextHopIpPtr,
  CONST(uint32, TCPIP_APPL_DATA) networkMask,
  CONST(uint32, TCPIP_APPL_DATA) localIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandle
);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)

/** \brief checks for AH header and consults the policy data base
 *
 * \param[in] localAddrId         specifies the destination IP address on which the frame was received
 * \param[in] remoteAddrPtr       specifies the source IP address on which the frame was transmitted
 * \param[in] dataPtr             dataPtr refers to the beginning of IPv4 header
 * \param[in] headerLength        length of the IPv4 header
 * \param[in,out] payloadPtrPtr   in:  pointer to the beginning of the IPv4 payload
 *                                out: pointer to the beginning of the upper layer protocol (e.g. UDP)
 * \param[in,out] payloadLenPtr   in:  IPv4 payload length
 *                                out: Upper layer data length
 * \param[in,out] protocol        in: next header after the IPv4 header
 *                                out: upper layer protocol
 * \param[in] PhysAddrPtr         Points to MAC address of remote host (source MAC)
 *
 * \retval TRUE               frame is either bypassed or secured and validated,
 *                            continue processing packet
 * \retval FALSE              frame shall be discarded or security could not be validated
 */
STATIC FUNC(boolean, TCPIP_CODE) Tcpip_IpV4_checkRxSecurity
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) payloadPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLen,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) protocol,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

#if  ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
STATIC FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpV4_rxIPv4Options
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionPtr,
  uint8 allOptionsLen
);
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)) */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)*/

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
/** \brief Parse IPv4 specific fragment header and fill the \a fragPtr structure for perusal by
 * \a TcpIp_IpV6_handleFragment and \a TcpIp_Ip_Frag_reassemble.
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_parseFragmentHeader
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  uint32 srcIp,
  uint32 dstIp,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  uint8 headerLen,
  uint16 payloadLen,
  uint16 fragmentInfo,
  uint8 protocol
);

/** \brief Handle the IPv4 specific part of fragment-handling within the context of
 * \a TcpIp_IpV6_processNextHeader after initially calling the generic handling in
 * \a TcpIp_Ip_Frag_reassemble.
 *
 * \return generic error codes (from TcpIp_Int.h), that are interpreted IPv4 specific in
 *          \a TcpIp_IpV4_rxIndicationIp.
 */
STATIC FUNC( TcpIp_Ip_RxReturnType , TCPIP_CODE ) TcpIp_IpV4_handleFragment
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtrPtr,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) dataPtrPtr,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) payloadPtrPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLenPtr
);
#endif

/** \brief Pass message to upper layer (UDP,TCP,ICMP)
 *
 * \param[in] isFragmented    Indicates if a packet is fragmented
 * \param[in] destIp          destination ip address of the received message
 * \param[in] srcIp           source ip address of the received message
 * \param[in] localAddrId     local address on which the message was received
 * \param[in] dataPtr         pointer to ip header
 * \param[in] payloadPtr      pointer to ip payload
 * \param[in] payloadLen      length of the payload
 * \param[in] isBroadcast     Indicates if the target MAC address is a broadcast address
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_passToUpperLayer
(
  P2VAR(TcpIp_IpV4_HeaderType, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  TcpIp_LocalAddrIdType localAddrId,
  boolean isFragmented,
  boolean isBroadcast
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and reset dropped datagrams caused by invalid IPv4 address
 *
 * This function reads and resets dropped datagrams caused by invalid IPv4 address.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_GetAndResetInvalidIpAddrCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if (TCPIP_ICMPV4_ENABLED == STD_ON)

/** \brief Checks if remote host shall be informed by errors through sending ICMP Error
 *         like unreachable port or unexpected next header protocol
 *
 * \param[in] RxRetCode       Return value from upper layer protocol (ICMP, UDP, TCP)
 * \param[in] localAddrId     local address on which the message was received
 * \param[in] srcIp           source ip address of the received message
 * \param[in] dataPtr         pointer to ip header
 * \param[in] payloadLen      length of the payload
 * \param[in] headerLength    IPv4 header length of the received frame
 * \param[in] isBroadcast     Indicates if the target MAC address is a broadcast address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_handleErrors
(
  TcpIp_Ip_RxReturnType RxRetCode,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_SockAddrInetType srcIp,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 payloadLen,
  uint8 headerLength,
  boolean isBroadcast
);

#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

/** \brief Checks fragmentation and IpSec headers and pass message to upper layer (UDP,TCP,ICMP)
 *
 * \param[in] localAddrId             local address on which the message was received
 * \param[in] physAddrPtr             Points to MAC address of remote host (source MAC)
 * \param[in] dataPtr                 pointer to ip header
 * \param[in] headerPtr               pointer to a structure containing ip data and parameters
 * \param[in] headerLength            Length of header in units of bytes, 0 if header is invalid
 * \param[in] isBroadcast             Indicates if the target MAC address is a broadcast address
 * \param[in] subNetCommunication     Indicates if communication from srcAddr to destAddr is subnet-local
 * \param[in] ctrlIdx                 Index of the EthIf controller
 *
 * \return value indicating result of processing incoming IpV4 frame
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_rxIndicationIp_processHeaders
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(TcpIp_IpV4_HeaderType, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  uint8 const headerLength,
  boolean isBroadcast,
  boolean subNetCommunication,
  uint8 ctrlIdx
);
#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

STATIC CONST(uint32,TCPIP_CONST) TcpIp_Ip_IpV4AddressUnspecified = 0x0U;

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV4_InvalidIpAddrCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_init(void)
{
  DBG_TCPIP_IPV4_INIT_ENTRY();

  TcpIp_IpV4_ArpInit();

  TcpIp_IpV4_AddrConflDetect_init();

  DBG_TCPIP_IPV4_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_rxIndicationIp
(
  uint8 ctrlIdx,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtrIn,
  uint16 lenByte
)
{
  TcpIp_Ip_RxReturnType RxRetCode = TCPIP_IP_RX_NOT_OK;

  DBG_TCPIP_IPV4_RXINDICATIONIP_ENTRY(ctrlIdx,isBroadcast,physAddrPtr,dataPtrIn,lenByte);
  {
    TcpIp_IpV4_HeaderType header = {0U,NULL_PTR,NULL_PTR,0U,0U,0U,0U};
    /* Fragment handling must redirect ptr to reassembly buffer */
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr= dataPtrIn;
    /* check if header is valid */
    uint8 const headerLength = TcpIp_IpV4_checkValidHeader(dataPtr, lenByte, &header.payloadLen, ctrlIdx);

    if(headerLength >= TCPIP_IPV4_HEADER_MINIMUM_LENGTH)
    {
      /* header was successfully validated */
      TcpIp_SockAddrInetType srcIp;
      uint32 remoteNetMask;
      TcpIp_IpAddrConstPtr remoteNetMaskPtr = NULL_PTR;
      boolean subNetCommunication;

      /* obtain source address */
      srcIp.domain = TCPIP_AF_INET;
      TCPIP_IPV4_SRC_ADDR_GET(dataPtr,&srcIp.addr[0]);

      /* get destination Ip address and map to local address of this device */
      TCPIP_IPV4_DEST_ADDR_GET(dataPtr,&header.destIp);

      subNetCommunication = TcpIp_IpV4_isSubnetCommunication(ctrlIdx, &srcIp, header.destIp, &remoteNetMaskPtr);

      if(subNetCommunication == TRUE)
      {
        remoteNetMask =  (*remoteNetMaskPtr);
      }
      else
      {
        /* set netMask to 0 since it is further only used to check the remote address.
           If the remote address is not in the same subnet, we cannot perform any checks, since
           we don't know the remote netmask */
        remoteNetMask = 0U;
      }

      /* validate source address */
      if (TcpIp_IpV4_checkUnicastAddr(srcIp.addr[0U],remoteNetMask))
      {
#if (                                                                    \
      (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON) ||           \
      (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON) ||           \
      (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)                \
    )
        TcpIp_SockAddrInetType dstIp;
        TcpIp_SockAddrConstPtrType dstAddrPtr;
        TcpIp_SockAddrConstPtrType srcAddrPtr;

        /* copy destination address */
        dstIp.domain = TCPIP_AF_INET;
        dstIp.addr[0U] = header.destIp;

        /* Deviation MISRAC2012-2 <+2> */
        dstAddrPtr = (TcpIp_SockAddrConstPtrType)&(dstIp);
        srcAddrPtr = (TcpIp_SockAddrConstPtrType)&(srcIp);
#endif /* (                                                              \
            (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON) ||     \
            (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON) ||     \
            (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)          \
          ) */
#if (TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON)
        /* check if packet is allowed to be processed further or needs to be dropped */
        if
        (
          TcpIp_RxPolicyCheckIngressHandlerAPI
          (
            &RxRetCode,
            dstAddrPtr,
            srcAddrPtr,
            (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
            dataPtr,
            lenByte,
            isBroadcast
          ) == E_OK
        )
#endif /* TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED == STD_ON */
        {
          TcpIp_LocalAddrIdType localAddrId = TcpIp_IpV4_getLocalAddrId_byIpV4(header.destIp,ctrlIdx);

          if(localAddrId != TCPIP_LOCALADDRID_INVALID)
          {
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
            if (TcpIp_IpSecDb_checkSecurityPolicyAddr(localAddrId, srcIp.addr,physAddrPtr) == E_OK)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
            {
              /* destAddr passed local Ip address filter */
              /* extract Ip header information */
              header.payloadPtr = &dataPtr[headerLength];
              header.protocol = TCPIP_IPV4_PROTOCOL_GET(dataPtr);
              header.srcIp = &srcIp;
              header.ttl = TCPIP_IPV4_TTL_GET(dataPtr);
              header.lenByte = lenByte;

              RxRetCode = TcpIp_IpV4_rxIndicationIp_processHeaders
                          (
                            localAddrId,
                            physAddrPtr,
                            dataPtr,
                            &header,
                            headerLength,
                            isBroadcast,
                            subNetCommunication,
                            ctrlIdx
                          );

#if (TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON)
              if(RxRetCode == TCPIP_IP_RX_OK)
              {
                TcpIp_RxPacketPostProcessHandlerAPI
                  (dstAddrPtr, srcAddrPtr, (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, dataPtr, lenByte, isBroadcast);
              }
#endif /* TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED == STD_ON */
            }
          }
          else
          {
            RxRetCode = TCPIP_IP_RX_DEST_UNREACHABLE;
          }
#if (TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON)
          /* check if packet needs to be routed */
          if((localAddrId == TCPIP_LOCALADDRID_INVALID) || (TCPIP_IPV4_IS_MULTICAST_ADDR(header.destIp)))
          {
            RxRetCode =
              TcpIp_RxPolicyCheckRoutingHandlerAPI
                (dstAddrPtr, srcAddrPtr, (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, dataPtr, lenByte, isBroadcast);
          }
#endif /* TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED == STD_ON */
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
          if ((localAddrId == TCPIP_LOCALADDRID_INVALID) && (RxRetCode == TCPIP_IP_RX_DEST_UNREACHABLE))
          {
            /* increment counter of dropped frames caused by invalid IPv4 address */
            /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
            SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
            if(TcpIp_IpV4_InvalidIpAddrCounter < 0xFFFFFFFFU)
            {
              /* !LINKSTO TcpIp.Design.IpV4.Atomic.InvalidIpAddrCounter,1 */
              TS_AtomicAssign32(TcpIp_IpV4_InvalidIpAddrCounter, (TcpIp_IpV4_InvalidIpAddrCounter+1U));
            }
            /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
            SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
          }
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
        }
      }
      else
      {
        /* source address is multicast */
        RxRetCode = TCPIP_IP_RX_SRCADDR_INVALID;
      }
    }
    else
    {
      /* header check failed */
      RxRetCode = TCPIP_IP_RX_INVALID_HEADER;
    }
  }

  DBG_TCPIP_IPV4_RXINDICATIONIP_EXIT(RxRetCode,ctrlIdx,isBroadcast,physAddrPtr,dataPtrIn,lenByte);

  return RxRetCode;
}

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_getFragmentSize
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint16 result;

  DBG_TCPIP_IPV4_GETFRAGMENTSIZE_ENTRY(localAddrId);
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    uint16 mtu;
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
    mtu = (TCPIP_CFG(ctrl,ctrlIdx)).mtu;
    result = (mtu - TCPIP_IPV4_HEADER_MINIMUM_LENGTH);
  }
  DBG_TCPIP_IPV4_GETFRAGMENTSIZE_EXIT(result, localAddrId);

  return result;
}

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
)
{
  Std_ReturnType result = E_NOT_OK;

  DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_ENTRY
    (ipSecSaCommonIdx, localAddrId, maxPayloadPtr, additionalHdrSizePtr);
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    uint16 mtu;

    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    mtu = (TCPIP_CFG(ctrl,ctrlIdx)).mtu;
    *maxPayloadPtr = (mtu - TCPIP_IPV4_HEADER_MINIMUM_LENGTH);

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
  DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_EXIT
    (result, ipSecSaCommonIdx, localAddrId, maxPayloadPtr, additionalHdrSizePtr);

  return result;
}

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_getMinimumPayload(void)
{
  uint16 result;

  DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_ENTRY();
  {
    result = (TCPIP_IPV4_MIN_MTU_SIZE - TCPIP_IPV4_HEADER_MINIMUM_LENGTH);
  }
  DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_EXIT(result);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  DBG_TCPIP_IPV4_REACHCONFIRMATION_ENTRY(CtrlIdx,remoteAddrPtr);
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(remoteAddrPtr);
  DBG_TCPIP_IPV4_REACHCONFIRMATION_EXIT(CtrlIdx,remoteAddrPtr);
}
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  boolean retVal;
  /* Deviation MISRAC2012-2 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_CONST)) RemoteAddrPtr)->addr;

  DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_ENTRY(RemoteAddrPtr);
  if
  (
    (TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(IpAddrPtr[0U])) ||
    (IpAddrPtr[0U] == TCPIP_IPV4_ADDR_INVALID_VALUE) ||
    (TCPIP_IPV4_IS_MULTICAST_ADDR(IpAddrPtr[0U]))
  )
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }
  DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_EXIT(retVal,RemoteAddrPtr);
  return retVal;
}
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_getRemoteAddrHandle
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType retVal;
  /* Deviation MISRAC2012-2 */
  P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr =
    (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) destIpAddrPtr;

  TCPIP_PBCFG_PRECONDITION_ASSERT(localAddrId, localAddrCfg, TCPIP_INTERNAL_API_ID);
  DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_ENTRY(remoteAddrHandlePtr, destIpAddrPtr, localAddrId);
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
    *remoteAddrHandlePtr = 0U;

    /* check for special destination addresses */
    if(TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(destIpPtr->addr[0U]))
    {
      /* broadcast: remote physical address = broadcast address */
      *remoteAddrHandlePtr = TCPIP_IPV4_ARP_REMOTE_HANDLE_BROADCAST;
      retVal = TCPIP_OK;
    }
    else if(TCPIP_IPV4_IS_MULTICAST_ADDR(destIpPtr->addr[0U]))
    {
      /* multicast: remote physical address = multicast address */
      *remoteAddrHandlePtr = TCPIP_IPV4_ARP_REMOTE_HANDLE_MULTICAST;
      retVal = TCPIP_OK;
    }
    else
    {
      TcpIp_IpAddrConstPtr nextHopIpPtr;
      TcpIp_IpAddrConstPtr networkMaskPtr;
      TcpIp_IpAddrConstPtr localIpAddrPtr;

      /* assume default next hop = default gateway */
      TcpIp_IpAddrM_getIpConfig(localAddrId, &localIpAddrPtr, &nextHopIpPtr, &networkMaskPtr);

      {
        retVal = TcpIp_IpV4_getNextHopIp(&nextHopIpPtr,*networkMaskPtr,*localIpAddrPtr,destIpPtr->addr,remoteAddrHandlePtr);

        /* we have no specific handle until now, thus perform an ARP lookup */
        if((*remoteAddrHandlePtr == 0U) && (retVal == TCPIP_OK))
        {
          retVal = TcpIp_IpV4_ArpLookupAndLock(*nextHopIpPtr,remoteAddrHandlePtr,ctrlIdx,TRUE);

          if(retVal == TCPIP_E_PHYS_ADDR_MISS)
          {
            /* ups ARP lookup failed, we need to send an ARP request */
            TcpIp_IpV4_ArpTransmitRequest(*nextHopIpPtr, *localIpAddrPtr, ctrlIdx);
          }
          else
          {
            if(retVal != TCPIP_OK)
            {
              /* address resolution ongoing */
              retVal = TCPIP_E_PHYS_ADDR_MISS;
            }
          }
        }
      }
    }
  }
  DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_EXIT(retVal, remoteAddrHandlePtr, destIpAddrPtr, localAddrId);
  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_provideTxBuffer
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 lenByte,
  TcpIp_LocalAddrIdType localAddrId,
  boolean ipFragment
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TS_PARAM_UNUSED(ipFragment); /* currently only used in IPv6 to indicate the need of a fragment extension header */
  /* TBD: hand down to ETH for allocation ? */

  DBG_TCPIP_IPV4_PROVIDETXBUFFER_ENTRY
    (ipParameterPtr,ethTxHandlePtr,lenByte,localAddrId,ipFragment);
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    /* so far so good, physical address is obtained */
    uint16 ipLength = lenByte + TCPIP_IPV4_HEADER_MINIMUM_LENGTH;
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
      vlanPriority =
          (TCPIP_CFG(ctrl,ctrlIdx)).defaultVlanPrio;
    }

    /* fetch a tx buffer from ethernet */
    if(EthIf_ProvideTxBuffer(
        (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
        TCPIP_IP_FRAMETYPE_IPV4,
        vlanPriority,
        &bufIdx,
        &DataPtr,
        &ipLength) == BUFREQ_OK)
    {
      /* the return length must be greater or equal to the requested buffer length */
      TCPIP_PRECONDITION_ASSERT
        (ipLength >= lenByte + TCPIP_IPV4_HEADER_MINIMUM_LENGTH, TCPIP_INTERNAL_API_ID);

      /* buffer was obtained, subtract IP header space from buffer */
      ethTxHandlePtr->bufferHandle = bufIdx;
      ethTxHandlePtr->bufferPtr =  &DataPtr[TCPIP_IPV4_HEADER_MINIMUM_LENGTH];
      ethTxHandlePtr->bufferLength = lenByte;
      retVal = TCPIP_OK;
    }

  }
  DBG_TCPIP_IPV4_PROVIDETXBUFFER_EXIT
    (retVal,ipParameterPtr,ethTxHandlePtr,lenByte,localAddrId,ipFragment);

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_transmit
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType retVal;

  DBG_TCPIP_IPV4_TRANSMIT_ENTRY
    (ipParameterPtr, fragHdrPtr, ethTxHandlePtr, remoteAddrHandle, destIpPtr, localAddrId);
  {
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    /* add IP header space to passed buffer */
    uint16 const iplength = TCPIP_IPV4_HEADER_MINIMUM_LENGTH + ethTxHandlePtr->bufferLength;
    /* Deviation MISRAC2012-2 */
    uint32 const destIp =
      (((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))destIpPtr)->addr)[0U];

    /* get local source IP address for transmission */
    TcpIp_IpAddrConstPtr localIpAddrPtr;
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    if(ipParameterPtr->useUnspecifiedSrc)
    {
      /* overrule source ip address selection */
      localIpAddrPtr = &TcpIp_Ip_IpV4AddressUnspecified;
    }
    else
    {
      TCPIP_PRECONDITION_ASSERT
        (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddrId)).addrType,IPV4),
         TCPIP_INTERNAL_API_ID);

      /* assume default next hop = default gateway */
      TcpIp_IpAddrM_getIpAddress(localAddrId, &localIpAddrPtr);
    }

    TcpIp_IpV4_buildHeader(
        &ethTxHandlePtr->bufferPtr[-1*((sint8)TCPIP_IPV4_HEADER_MINIMUM_LENGTH)],
        ipParameterPtr,
        fragHdrPtr,
        localIpAddrPtr,
        destIp,
        iplength,
        ctrlIdx
    );
    {
      /* this array must be in the same scope as physAddrPtr,
         since it lives and dies with it (multicast) */
      uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH] = TCPIP_ETH_BROADCAST_ADDRESS;
      P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr;

      /* get the physical destination address */
      switch(remoteAddrHandle)
      {
        case TCPIP_IPV4_ARP_REMOTE_HANDLE_BROADCAST:
        {
          /* broadcast address */
          physAddrPtr = ethAddr;
          break;
        }
        case TCPIP_IPV4_ARP_REMOTE_HANDLE_MULTICAST:
        {
          /* multicast address - derived from IP multicast address */
          TcpIp_IpV4_GetPhysMulticastAddr(ethAddr, (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)) &destIp);
          physAddrPtr = &ethAddr[0];
          break;
        }
        default:
        {
          /* dedicated physical address from ARP table */
          physAddrPtr = TcpIp_IpV4_Arp_getPhysAddrPtr(remoteAddrHandle,ctrlIdx);
          break;
        }
      }

      /* !LINKSTO TcpIp.ASR40.SWS_TCPIP_00131,1 */
      if(EthIf_Transmit(
          (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
           ethTxHandlePtr->bufferHandle,
           TCPIP_IP_FRAMETYPE_IPV4,
           FALSE,
           iplength,
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

  DBG_TCPIP_IPV4_TRANSMIT_EXIT
    (retVal, ipParameterPtr, fragHdrPtr, ethTxHandlePtr, remoteAddrHandle, destIpPtr, localAddrId);
  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_updateHeader
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

  DBG_TCPIP_IPV4_UPDATEHEADER_ENTRY
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
    localIpAddrPtr = &TcpIp_Ip_IpV4AddressUnspecified;
  }
  else
  {
    TCPIP_PRECONDITION_ASSERT
      (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddrId)).addrType,IPV4),
       TCPIP_INTERNAL_API_ID);

    /* assume default next hop = default gateway */
    TcpIp_IpAddrM_getIpAddress(localAddrId, &localIpAddrPtr);
  }

  /* the upper layer wants to add a checksum including the Ip pseudo header + payload checksum.
   * For fragmentation, this is mandatory for the header fragment. */

  if(ipParameterPtr->calculateChecksum && ipParameterPtr->calculatePseudoChecksum)
  {
    const uint16 protoLen = ipPayloadLength
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
        - ipParameterPtr->addHeaderLength
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
        ;
    /* calculate Ip pseudo header checksum fragment and pass to upper layer to add to its checksum */
      partialChecksum =
        TcpIp_IpV4_calcPartialChecksum
          (destIpPtr, localIpAddrPtr, protoLen, ipParameterPtr->protocol);

    sumWord = (uint32)partialChecksum + (uint32)payloadSum;
    sumWord = (sumWord & (uint32)0xFFFFu)+(sumWord>>16U);                   /* may overflow again */
    payloadSum = (uint16)((sumWord & (uint32)0xFFFFu)+(sumWord>>16U)); /* no more overflow */
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
    uint16 const iplength = TCPIP_IPV4_HEADER_MINIMUM_LENGTH + ipPayloadLength;
    P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)remoteAddrV4Ptr =
      /* Deviation MISRAC2012-2 */
    (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))destIpPtr;

    /* create Immutable header */
    sint8 const bufferOffset =
      -1* (sint8)
      (((sint8) ipParameterPtr->addHeaderLength + (sint8) TCPIP_IPV4_HEADER_MINIMUM_LENGTH));
    bufferPtr = &bufferPtr[bufferOffset];

    /* construct IP header */
    /* Deviation MISRAC2012-2 */
    TCPIP_IPV4_CREATE_IMMUTABLE_HDR(bufferPtr,
                                    iplength,
                                    (uint16)ipParameterPtr->identifier,
                                    TCPIP_IP_EXT_HDR_AUTHENTICATION,
                                    &remoteAddrV4Ptr->addr,
                                    localIpAddrPtr
                                   );

    /* attach the AH header */
    if(TcpIp_IpSec_buildAh
       (ipParameterPtr, ethTxHandlePtr, frgCnt, TCPIP_IP_MINIMUM_IPV4_HEADER_LENGTH) == E_OK)
    {
      result = TCPIP_OK;
    }
  }
  else
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  {
    result = TCPIP_OK;
  }

  DBG_TCPIP_IPV4_UPDATEHEADER_EXIT
    (result, updateHeaderFunctorPtr,ipParameterPtr, destIpPtr, ethTxHandlePtr,
     localAddrId, payloadSum, ipPayloadLength, frgCnt);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
)
{
  /* Deviation MISRAC2012-2 */
  P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) intIpAddrPtr =
    (P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) IpAddrPtr;

  /* Deviation MISRAC2012-2 */
  P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) intRouterIpAddrPtr =
    (P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) DefaultRouterPtr;

  /* pointers for obtaining IP configuration */
  TcpIp_IpAddrConstPtr unicastPtr;
  TcpIp_IpAddrConstPtr gatewayPtr;
  TcpIp_IpAddrConstPtr netmaskPtr;

  /* get the current Ip configuration */
  TcpIp_IpAddrM_getIpConfig(LocalAddrId,&unicastPtr,&gatewayPtr,&netmaskPtr);

  /* copy data to output parameters */
  intIpAddrPtr->addr[0] = *unicastPtr;
  intRouterIpAddrPtr->addr[0] = *gatewayPtr;

  /* write netmask in CIDR notation */
  TCPIP_GET_NUMBER_OF_SET_BITS_32(*netmaskPtr,*NetmaskPtr);
}

#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getTxLocalAddrId_byRemoteIp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  TcpIp_LocalAddrIdType result = TCPIP_LOCALADDRID_INVALID;
  uint8_least i;
  /* Deviation MISRAC2012-2 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_CONST)) remoteAddrPtr)->addr;

  DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(remoteAddrPtr);

  /* loop all controllers */
  for(i = 0U; (i < TcpIp_PBcfgPtr->ctrlNum) && (result == TCPIP_LOCALADDRID_INVALID); i++)
  {
      CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,i)).ctrlIpV4);

      const uint8 state = TcpIp_IpAddrM_getIfState(ctrlPtr->localAddrIdTx);

      switch(state)
      {
        case TCPIP_STATE_ONLINE: /* intended fall through */
        case TCPIP_STATE_SHUTDOWN:
        {
          TcpIp_IpAddrConstPtr assignedIpPtr;
          TcpIp_IpAddrConstPtr gatewayIpPtr;
          TcpIp_IpAddrConstPtr netmaskIpPtr;

          TcpIp_IpAddrM_getIpConfig(ctrlPtr->localAddrIdTx,&assignedIpPtr,&gatewayIpPtr,&netmaskIpPtr);

          /* compare if the assigned address of the controller's unicast address is in the same network as
            the destination address */
          if(((*netmaskIpPtr)&(*IpAddrPtr)) == ((*netmaskIpPtr)&(*assignedIpPtr)))
          {
            /* if yes, choose this interface */
            result = ctrlPtr->localAddrIdTx;
          }
          break;
        }
        default:
          /* default case intended to be empty */
          break;
      }
  }

  /* no interface in the same network found */
  if(result == TCPIP_LOCALADDRID_INVALID)
  {
    /* choose default interface (0) */
    CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,0U)).ctrlIpV4);

    result = ctrlPtr->localAddrIdTx;
  }

  DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_EXIT(result,remoteAddrPtr);

  return result;
}

#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr
)
{
  CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4);

  const TcpIp_LocalAddrIdType result = ctrlPtr->localAddrIdTx;

  DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_ENTRY(ctrlIdx,remoteIpAddrPtr);

  TS_PARAM_UNUSED(remoteIpAddrPtr);

  DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_EXIT(result,ctrlIdx,remoteIpAddrPtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpV4_getIfState
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_StateType returnState;
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &(TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx));
  CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlIpV4Ptr =
      &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ctrlIpV4);

  DBG_TCPIP_IPV4_GETIFSTATE_ENTRY(localAddrId);

  if(ctrlPtr->localAddrIdAny == localAddrId)
  {
    /* if there is a controller ANY requested, check the controllers unicast local address for availability */
    returnState = TcpIp_IpAddrM_getIfState(ctrlIpV4Ptr->localAddrIdTx);
  }
  else
  {
    /* otherwise check the concrete local address for availability */
    returnState = TcpIp_IpAddrM_getIfState(localAddrId);
  }

  DBG_TCPIP_IPV4_GETIFSTATE_EXIT(returnState,localAddrId);

  return returnState;
}


TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_mainFunction(uint16 timerTick)
{
  DBG_TCPIP_IPV4_MAINFUNCTION_ENTRY(timerTick);

  TcpIp_IpV4_ArpMainFunction(timerTick);

#if (TCPIP_IPV4_LINK_LOCAL_ENABLED)
  TcpIp_IpV4_LinkLocal_mainFunction(timerTick);
#endif /* (TCPIP_IPV4_LINK_LOCAL_ENABLED) */

  TcpIp_IpV4_AddrConflDetect_mainFunction(timerTick);

  DBG_TCPIP_IPV4_MAINFUNCTION_EXIT(timerTick);
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getLocalAddrId_byIpV4
(
  uint32 ipV4,
  uint8 ctrlIdx
)
{
  CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4);

  uint8_least i;
  TcpIp_LocalAddrIdType result = TCPIP_LOCALADDRID_INVALID;
  const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
  const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;

  /* iterate over all local addresses assigned to this controller */
  for(i = startIndex; i < endIndex; i++)
  {
    TCPIP_PBCFG_PRECONDITION_ASSERT(i, intIpLocalAddrCfg, TCPIP_INTERNAL_API_ID);
    {
      const uint8 LocalAddrId =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrConstPtr unicastPtr;

      TCPIP_PRECONDITION_ASSERT
        (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,LocalAddrId)).addrType,IPV4),
        TCPIP_INTERNAL_API_ID);

      TcpIp_IpAddrM_getIpAddress(LocalAddrId,&unicastPtr);

      if(*unicastPtr == ipV4)
      {
        /* exact match */
        /* this matches: unicast, multicast and limited broadcast addresses */
        result = LocalAddrId;
      }
      else
      {
        ; /* no result yet */
      }

      if(result != TCPIP_LOCALADDRID_INVALID)
      {
        break;
      }
    }
 }
  return result;
}

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destAddrPtr,
  uint16 lenByte,
  uint8 protocol
)
{
  uint16 retVal;
  uint32 checksum = 0U;
  uint32 tmp32;
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) srcIpAddrPtr;

  DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_ENTRY(srcAddrPtr,destAddrPtr,lenByte,protocol);

  srcIpAddrPtr = TCPIP_GET_IPV4_FROM_SOCK_ADDR_TYPE(srcAddrPtr);

  tmp32 = COMSTACK_NTOH_UINT32(srcIpAddrPtr[0U]);
  checksum += (tmp32 >> 16u) + ((uint16)tmp32);
  tmp32 = COMSTACK_NTOH_UINT32(destAddrPtr[0U]);
  checksum += (tmp32 >> 16u) + ((uint16)tmp32);
  checksum += lenByte;
  checksum += protocol;
  checksum = ((uint16)checksum + (checksum>>16U));
  retVal = (uint16)(checksum + (checksum>>16U));

  DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_EXIT(retVal,srcAddrPtr,destAddrPtr,lenByte,protocol);

  return retVal;
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_GetAndResetCounter
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  DBG_TCPIP_IPV4_GETANDRESETCOUNTER_ENTRY(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);

  switch(MeasurementIdx)
  {
    case TCPIP_MEAS_DROP_IPV4:
      TcpIp_IpV4_GetAndResetInvalidIpAddrCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_REPLACED_ARP:
      TcpIp_IpV4_Arp_GetAndResetReplacedArpEntryCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_DROP_ARP:
      TcpIp_IpV4_Arp_GetAndResetDroppedLockedArpEntryCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_ALL:
      TcpIp_IpV4_GetAndResetInvalidIpAddrCount(TRUE, NULL_PTR);
      TcpIp_IpV4_Arp_GetAndResetReplacedArpEntryCount(TRUE, NULL_PTR);
      TcpIp_IpV4_Arp_GetAndResetDroppedLockedArpEntryCount(TRUE, NULL_PTR);
      break;
    /* CHECK: NOPARSE */
    default:
      TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
      /* measurement index not supported */
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPV4_GETANDRESETCOUNTER_EXIT(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isAddrLimBroadcast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr
)
{
  boolean retVal = FALSE;
  CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlV4Ptr =
      &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4);
  DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_ENTRY(ctrlIdx, localAddrId, localIpAddrPtr);
  if((ctrlV4Ptr->localAddrLimdBroad == localAddrId) && TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(*localIpAddrPtr))
  {
    retVal = TRUE;
  }
  DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_EXIT(retVal, ctrlIdx, localAddrId, localIpAddrPtr);
  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr
)
{
  CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4);
  DBG_TCPIP_IPV4_GETCTRLADDRRANGE_ENTRY(ctrlIdx, startIndexPtr, endIndexPtr);
  *startIndexPtr = ctrlPtr->intLocalAddrIdFirst;
  *endIndexPtr = *startIndexPtr + ctrlPtr->intLocalAddrNum;
  DBG_TCPIP_IPV4_GETCTRLADDRRANGE_EXIT(ctrlIdx, startIndexPtr, endIndexPtr);
}
#endif /* TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON */

/*==================[internal function definitions]=========================*/

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)

/** \brief parse the IPv4 specifics and handle v4 specific errors, but return generic
 *  error codes (from TcpIp_Int.h), that are interpreted in \a TcpIp_IpV4_rxIndicationIp */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_parseFragmentHeader
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  uint32 srcIp,
  uint32 dstIp,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  uint8 headerLen,
  uint16 payloadLen,
  uint16 fragmentInfo,
  uint8 protocol
)
{
  TS_MemSet32_NoCheck(fragPtr, 0u, sizeof(*fragPtr));

  fragPtr->channelId.srcAddr[0]= srcIp;
  fragPtr->channelId.dstAddr[0]= dstIp;
  /* for V4, the protocol is part of the ID and still fits into the 32 bit field. */
  fragPtr->channelId.identification=
    (uint32)((((uint32)protocol) << 16u) | (uint32)(uint16)TCPIP_IPV4_IDENTIFICATION_GET(headerPtr));

  fragPtr->lenV46Header= headerLen;
  fragPtr->lenDataByte= payloadLen;

  /* change view to payload, but leave space for the header */
  fragPtr->dataPtr= &headerPtr[fragPtr->lenV46Header];

  fragPtr->offset= fragmentInfo << 3u; /* multiply by 8bytes, eliminate flags */
  fragPtr->moreFrags= (((fragmentInfo >> 13u) & 1u) == TRUE) ? TRUE : FALSE;
}

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_handleFragment
(
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr,
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtrPtr,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) dataPtrPtr,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) payloadPtrPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLenPtr
)
{
  TcpIp_Ip_RxReturnType result= TcpIp_Ip_Frag_reassemble(reAssembleItemPtrPtr, fragPtr);
  TcpIp_Ip_Frag_RxBufferPtrType reAssembleItemPtr= *reAssembleItemPtrPtr;

  /* reAssembleItemPtr == NULL_PTR for OutOfBuffers*/
  if (result != TCPIP_IP_RX_FRAGMENT_OUT_OF_BUFFERS)
  {
    /* use Offset0 fragment's IPV4 header field */
    if (fragPtr->offset==0u)
    {
      reAssembleItemPtr->headerStart= TCPIP_IP_MAX_HEADERS_LENGTH-fragPtr->lenV46Header;
      TS_MemCpy( &reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart],
                 &(*dataPtrPtr)[0],
                 fragPtr->lenV46Header);
      TCPIP_IPV4_FRAGMENTINFO_SET(&reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart], 0);
      /* no need to update checksum - those checks have passed and won't be queried again */
    }
    switch (result)
    {
      case TCPIP_IP_RX_OK: /* buffer _is_ complete */
        /* Reassembling finished -> add payload length to fragment0's headerlength */
        /* this may overflow for 64k payload + 60B header */
        /* Deviation MISRAC2012-2 */
        TCPIP_IPV4_TOTAL_LENGTH_BYTES_SET(&reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart],
                                          (uint16)reAssembleItemPtr->size + TCPIP_IPV4_HEADER_MINIMUM_LENGTH); /* include Minimum length IPv4 header (added for IPsec ICV calculation) */
        *dataPtrPtr= &reAssembleItemPtr->bufferPtr[reAssembleItemPtr->headerStart];
        *payloadPtrPtr= &reAssembleItemPtr->bufferPtr[TCPIP_IP_MAX_HEADERS_LENGTH];
        *payloadLenPtr=(uint16)reAssembleItemPtr->size;
      break;
      case TCPIP_IP_RX_FRAGMENT_TIMEOUT:      /* fall through */
      case TCPIP_IP_RX_FRAGMENT_OFFSET:       /* fall through */
      case TCPIP_IP_RX_FRAGMENT_LENGTH:       /* fall through */
      case TCPIP_IP_RX_FRAGMENT_OVERLAP:      /* fall through */
        /* kill buffer and drop fragment silently */
        TcpIp_Ip_Frag_returnBuffer(reAssembleItemPtr);
        result= TCPIP_IP_RX_NOT_OK;
        /* MISRA forbids a fall-through, here */
        /* keep statements in same order, so at least the compiler can tail-merge the cases */
        *payloadPtrPtr= NULL_PTR;
        *reAssembleItemPtrPtr= NULL_PTR;
      break;
      case TCPIP_IP_RX_NOT_OK:
        /* unfinished buffer */
        /* don't pass the buffer, or it will be returned */
        *payloadPtrPtr= NULL_PTR;
        *reAssembleItemPtrPtr= NULL_PTR;
      break;
      /* CHECK: NOPARSE */
      default: TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
      break;
      /* CHECK: PARSE */
    }
  }
  return result;
}
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_passToUpperLayer
(
  P2VAR(TcpIp_IpV4_HeaderType, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  TcpIp_LocalAddrIdType localAddrId,
  boolean isFragmented,
  boolean isBroadcast
)
{
  TcpIp_Ip_RxReturnType RxRetCode= TCPIP_IP_RX_OK;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) payloadPtr = headerPtr->payloadPtr;
  /* Deviation MISRAC2012-2 */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr =
    (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) headerPtr->srcIp;

#if (TCPIP_ICMPV4_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(isBroadcast);
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

  switch(headerPtr->protocol)
  {
#if (TCPIP_UDP_ENABLED == STD_ON)
    case TCPIP_IP_PROTOCOL_UDP:
    {
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
      RxRetCode =
#else /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
      (void)
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
        TcpIp_Udp_rxIndication
        (
          srcAddrPtr,
          &headerPtr->destIp,
          payloadPtr,
          localAddrId,
          headerPtr->payloadLen,
          isFragmented
        );
      break;
    }
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
    case TCPIP_IP_PROTOCOL_ICMP:
    {
      TcpIp_IpV4_rxIndicationIcmp
        (
         srcAddrPtr,
         payloadPtr,
         localAddrId,
         headerPtr->payloadLen,
         headerPtr->ttl,
         isBroadcast,
         isFragmented
        );
      break;
    }
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
#if (TCPIP_TCP_ENABLED == STD_ON)
    case TCPIP_IP_PROTOCOL_TCP:
    {
      TcpIp_Tcp_rxIndication
        (
         srcAddrPtr,
         &headerPtr->destIp,
         payloadPtr,
         localAddrId,
         headerPtr->payloadLen,
         isFragmented
        );
      break;
    }
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    default:
    {
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
      RxRetCode = TCPIP_IP_RX_PROTOCOL_UNREACHABLE;
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
      break;
    }
  }

  return RxRetCode;
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpV4_checkValidHeader
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLengthPtr,
  uint8 ctrlIdx
)
{
  uint8 headerLength = 0U;

  /* check if a complete minimum IPV4 header is contained in the payload and the header version is correct */
  if(TCPIP_IPV4_HEADER_MINIMUM_CONTAINED(lenByte) && TCPIP_IPV4_VERSION_IS_IPV4(dataPtr))
  {
    const uint8 IHLBytes = TCPIP_IPV4_IHL_BYTES_GET(dataPtr);

    /* check that the complete IP header + 8 byte payload is contained received datagram */
    if(lenByte >= IHLBytes)
    {
      uint16 checksum = 0U;
      const boolean IPv4_hardware_checksum = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_IPV4(ctrl,ctrlIdx);
      if(IPv4_hardware_checksum != TRUE)
      {
        checksum = TcpIp_CalcChecksum16Bit(dataPtr,IHLBytes);
      }
      if(checksum == 0U)
      {
        /* check that total message length is long enough for header */
        const uint16 MessageLength = TCPIP_IPV4_TOTAL_LENGTH_BYTES_GET(dataPtr);
        if((MessageLength >= IHLBytes) && (lenByte >= MessageLength))
        {
          /* header is alright, return length of validated header */
          headerLength = IHLBytes;
          *payloadLengthPtr = MessageLength - headerLength;
        }
      }
    }
  }
  return headerLength;
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isHostIdInvalid
(
    uint32 srcAddr,
    uint32 netMask
)
{
  const uint32 hostMask = ((uint32)~netMask);
  const uint32 SubnetHostAddr = (hostMask&srcAddr);
  const uint32 SubnetBroadcastAddr = (hostMask&0xFFFFFFFFU);
  const uint32 SubnetNetworkAddr = (0U);
  boolean result;

  if((SubnetHostAddr == SubnetBroadcastAddr) || (SubnetHostAddr == SubnetNetworkAddr))
  {
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }

  return result;
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_checkUnicastAddr
(
    uint32 srcAddr,
    uint32 netMask
)
{
  boolean result;

  /* check if host Id is not all 0 (network) and all 1 (broadcast) */
  if (TcpIp_IpV4_isHostIdInvalid(srcAddr,netMask))
  {
    result = FALSE;
  }
  /* source address must not be a multicast address */
  else if (TCPIP_IPV4_IS_MULTICAST_ADDR(srcAddr))
  {
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_buildHeader
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipHeaderPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  TcpIp_IpAddrConstPtr localIpAddrPtr,
  uint32 destIp,
  uint16 ipLength,
  uint8 ctrlIdx
)
{
  uint8 ttl;
  uint8 dscp;
  const boolean IPv4_hardware_checksum = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_IPV4(ctrl,ctrlIdx);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF )
  TS_PARAM_UNUSED(fragHdrPtr);
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF ) */

  /* !LINKSTO TcpIp.Design.Socket.Atomic.getIpParameters,1 */
  TS_AtomicAssign8(ttl, ipParameterPtr->ttl);
  /* construct IP header */
  TCPIP_IPV4_IHL_BYTES_SET(ipHeaderPtr, TCPIP_IPV4_HEADER_MINIMUM_LENGTH);
  TCPIP_IPV4_TOS_SET(ipHeaderPtr, 0U);
  TCPIP_IPV4_TOTAL_LENGTH_BYTES_SET(ipHeaderPtr, ipLength);

  /* DSCP */
  TS_AtomicAssign8(dscp, ipParameterPtr->dscp);
  TCPIP_IPV4_DSCP_BYTES_SET(ipHeaderPtr, dscp);

  /* The ID field should only be used for fragments and (re-)fragmentable (DF=0) packets. */
  /* Identification must come from the upper layers, if they expect/do fragmentation.
   * (may only be left out (0?), if DF==1) */
  TCPIP_IPV4_IDENTIFICATION_SET(ipHeaderPtr, (uint16)ipParameterPtr->identifier);
  {
    /* Fragment offset / DF bit */
    uint16 fragInfo =
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
        (fragHdrPtr->IPisFragment == TRUE) ?
        (((uint16)(fragHdrPtr->IPoffset >> 3u)) | (((uint16)((fragHdrPtr->IPmoreFlag == TRUE) ? 0x1U : 0x0U) << 13u))) :
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF */
        ((uint16) 0u);
    if (!ipParameterPtr->mayFragment)
    {
      fragInfo |= TCPIP_IPV4_FLAG_DONT_FRAGMENT;
    }
    TCPIP_IPV4_FRAGMENTINFO_SET(ipHeaderPtr, fragInfo);
  }
  TCPIP_IPV4_TTL_SET(ipHeaderPtr, ttl);
  TCPIP_IPV4_PROTOCOL_SET(ipHeaderPtr, ipParameterPtr->protocol);
  TCPIP_IPV4_SRC_ADDR_SET(ipHeaderPtr, localIpAddrPtr);
  TCPIP_IPV4_DEST_ADDR_SET(ipHeaderPtr, &destIp);
  TCPIP_IPV4_CHECKSUM_SET(ipHeaderPtr, (uint16)0U);

  if(IPv4_hardware_checksum == FALSE)
  {
    /* Deviation MISRAC2012-2 */
    TCPIP_IPV4_CHECKSUM_SET(ipHeaderPtr,
        TcpIp_CalcChecksum16Bit(ipHeaderPtr, TCPIP_IPV4_HEADER_MINIMUM_LENGTH));
  }
}


STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_isSubnetCommunication
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrInetType,AUTOMATIC,TCPIP_APPL_DATA) srcAddr,
  uint32 destAddr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) remoteNetMaskPtrPtr
)
{
  TcpIp_IpAddrConstPtr unicastLocalIpPtr;
  TcpIp_IpAddrConstPtr defaultGatewayPtr;
  boolean result = FALSE;

  /* get unicast local addr config for receiving interface */
  TcpIp_LocalAddrIdType unicastLocalAddrId;
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  unicastLocalAddrId =
    TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx
    (
      ctrlIdx,
      /* Deviation MISRAC2012-2 */
      (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) srcAddr
    );

  TcpIp_IpAddrM_getIpConfig(unicastLocalAddrId, &unicastLocalIpPtr, &defaultGatewayPtr, remoteNetMaskPtrPtr);

  if((*unicastLocalIpPtr) != TCPIP_IPV4_ADDR_INVALID_VALUE)
  {
    /* if unicast ip address is assigned and netmask is known check whether srcAddr and unicast
       address of receiving interface are within a subnet. */
    if(((**remoteNetMaskPtrPtr) != TCPIP_IPV4_ADDR_INVALID_VALUE) &&
       (TCPIP_IPV4_IN_SAME_SUBNET(srcAddr->addr[0], *unicastLocalIpPtr, **remoteNetMaskPtrPtr))
      )
    {
      result = TRUE;
    }
  }
#if (TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE == STD_ON)
  else
  {
    /* if unicast ip address is not assigned, only allow frames addressed
       to boradcast or multicast */
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
    const boolean isMulticast = TCPIP_IPV4_IS_MULTICAST_ADDR(destAddr);
    /* Deviation MISRAC2012-5 */
    if(TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(destAddr) || isMulticast)
    {
      result = TRUE;
    }
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
    TS_PARAM_UNUSED(destAddr);
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
  }
#else /* (TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE == STD_ON) */
  TS_PARAM_UNUSED(destAddr);
#endif /* (TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE == STD_ON) */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_getNextHopIp
(
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) nextHopIpPtr,
  CONST(uint32, TCPIP_APPL_DATA) networkMask,
  CONST(uint32, TCPIP_APPL_DATA) localIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandle
)
{
  const uint32 destIpAddr = (*destIpPtr);
  const uint32 localNetworkId = (localIpAddr)&(networkMask);
  const uint32 destinationNetworkId = (destIpAddr)&(networkMask);
  TcpIp_ReturnType retVal;

  /* if we have a gateway we evaluate the NetId - if destination is in the same network */
  if(localNetworkId == destinationNetworkId)
  {
    const uint32 hostMask = (uint32)~(networkMask);

    /* this is a directed network in our local network - use broadcast physical address */
    if(((destIpAddr)&hostMask) == (hostMask&COMSTACK_HTON_UINT32(TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE)))
    {
      /* remote physical address = broadcast address */
      *remoteAddrHandle = TCPIP_IPV4_ARP_REMOTE_HANDLE_BROADCAST;
    }
    else
    {
      /* no broadcast, directly send to remote Ip address within the same network */
      *nextHopIpPtr = destIpPtr;
    }
    retVal = TCPIP_OK;
  }
  else if(TcpIp_IpV4_IsLinkLocalAddr(localIpAddr))
  {
    /* link-local address assigned, directly send to remote Ip address within the same network */
    *nextHopIpPtr = destIpPtr;
    retVal = TCPIP_OK;
  }
  else if(TcpIp_IpV4_IsLinkLocalAddr(destIpAddr))
  {
    /* destination address is link local, always send to link local address directly */
    *nextHopIpPtr = destIpPtr;
    retVal = TCPIP_OK;
  }
  else if(*(*nextHopIpPtr) == TCPIP_IPV4_ADDR_INVALID_VALUE)
  {
    /* cannot evaluate next hop destination - return error ... */
    retVal = TCPIP_E_NOT_OK;
  }
  else
  {
    retVal = TCPIP_OK;
  }

  return retVal;
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_GetAndResetInvalidIpAddrCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.InvalidIpAddrCounter,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpV4_InvalidIpAddrCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.InvalidIpAddrCounter,1 */
    TS_AtomicAssign32(TcpIp_IpV4_InvalidIpAddrCounter, 0U);
  }
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */


TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_IsLinkLocalAddr
(
  uint32 ipAddr
)
{
  const uint32 upperBytes =
    ((ipAddr)& COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_LINKLOCAL_MASK));

  return
   (upperBytes == COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_NETWORKID_LINKLOCAL_VALUE));
}

#if (TCPIP_ICMPV4_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_handleErrors
(
  TcpIp_Ip_RxReturnType RxRetCode,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_SockAddrInetType srcIp,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 payloadLen,
  uint8 headerLength,
  boolean isBroadcast
)
{
  switch(RxRetCode)
  {
    case TCPIP_IP_RX_PROTOCOL_UNREACHABLE: /* intended fall through */
    case TCPIP_IP_RX_PORT_UNREACHABLE:
    {
      /* in case of an upper layer error response, transmit ICMP error message */
      TcpIp_IpAddrM_AddrInfoType const addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);

      if(TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,UNICAST) && (!isBroadcast))
      {
        uint16 const icmpPayloadLength =
          headerLength + ((payloadLen > TCPIP_IPV4_ICMP_ERROR_PAYLOAD_LENGTH)
                            ? TCPIP_IPV4_ICMP_ERROR_PAYLOAD_LENGTH
                            : payloadLen);
        uint32 unusedBytes = 0U;
        /* Deviation MISRAC2012-2 */
        P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr =
          (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) &srcIp;
        TcpIp_Ip_Icmp_ParameterType icmpParameter;

        icmpParameter.code = RxRetCode;
        icmpParameter.hopLimit = TcpIp_PBcfgPtr->ipV4Config.icmpTtl;
        icmpParameter.type = TCPIP_IPV4_ICMP_TYPE_DEST_UNREACH;
        icmpParameter.specificValue = &unusedBytes;

        /* but only if destination was not a multicast/unicast */
        (void)TcpIp_IpV4_icmpTransmit
          (
            dataPtr,
            icmpPayloadLength,
            srcAddrPtr,
            localAddrId,
            &icmpParameter,
            TRUE
          );
      }
      break;
    }
    default:
      /* default case intended to be empty */
      break;
  }
}
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
STATIC FUNC(Std_ReturnType, TCPIP_CODE) Tcpip_IpV4_checkRxSecurity
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) payloadPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadLen,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) protocol,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  uint8 ipsecSaIdx = TCPIP_IPSECDB_INVALID_SA_INDEX;
#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))

  if(*protocol == TCPIP_IP_EXT_HDR_AUTHENTICATION)
  {
    /* only UDP, TCP or ICMP allowed as next header for IPv4 AH */
    if( (*payloadLen != 0U) &&
        ((*payloadPtr[0U] == TCPIP_IP_PROTOCOL_ICMP) ||
         (*payloadPtr[0U] == TCPIP_IP_PROTOCOL_TCP) ||
         (*payloadPtr[0U] == TCPIP_IP_PROTOCOL_UDP)))
    {
      *payloadPtr = dataPtr;
      *payloadLen = headerLength + *payloadLen;

      /* zero out mutable fields */
      TCPIP_IPV4_ZEROOUT_MUTABLE(dataPtr);

      /* check IPV4 header options */
      returnValue =
        TcpIp_IpV4_rxIPv4Options
          (&dataPtr[TCPIP_IPV4_HEADER_MINIMUM_LENGTH],
           headerLength - TCPIP_IPV4_HEADER_MINIMUM_LENGTH
          );

      if(returnValue == E_OK)
      {
        returnValue = TcpIp_IpSec_ahRxIndication(payloadPtr,
                                                 payloadLen,
                                                 protocol,
                                                 headerLength,
                                                 localAddrId,
                                                 remoteAddrPtr,
                                                 &ipsecSaIdx
                                                );
      }
    }
  }
  else
  {
    /* frame is not secured */
    returnValue = E_OK;
  }

  if(returnValue == E_OK)
#else
  TS_PARAM_UNUSED(dataPtr);
  TS_PARAM_UNUSED(headerLength);
  TS_PARAM_UNUSED(payloadLen);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
  {
    returnValue = TcpIp_IpSecDb_checkSecurityRxPolicy(localAddrId,
                                                      remoteAddrPtr,
                                                      *payloadPtr,
                                                      *payloadLen,
                                                      *protocol,
                                                      ipsecSaIdx,
                                                      physAddrPtr
                                                     );
  }

  return returnValue;
}

#if  ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC))
/** \brief Parse and zero mutable IPv4 options according to RFC 4302 Appendix A1 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_rxIPv4Options
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionPtr,
  uint8 allOptionsLen
)
{
  uint8 optionOffset = 0U;
  Std_ReturnType retVal;

  if(allOptionsLen != 0U)
  {
    do
    {
      uint8 const type = optionPtr[optionOffset];
      uint8 currentOptionLen = 0U;
      retVal = E_NOT_OK;

      /* Retrieve the length of the option */
      if((type == TCPIP_IPSEC_IPV4_NOP) || type == (TCPIP_IPSEC_IPV4_EOOL))
      {
        /* NOP and EEOL have fixed length of 1 */
        currentOptionLen = 1U;
        retVal = E_OK;
      }
      else
      {
        /* Other options have length as field after type */
        if((optionOffset + 1U) < allOptionsLen)
        {
          currentOptionLen = optionPtr[optionOffset+1U];
          if(((optionOffset + currentOptionLen) <= allOptionsLen) && (currentOptionLen > 0U))
          {
            retVal = E_OK;
          }
        }
      }

      /* Check whether option type is mutable */
      if(retVal == E_OK)
      {
        switch(type)
        {
          case TCPIP_IPSEC_IPV4_NOP: /* Intended falltrough */
          case TCPIP_IPSEC_IPV4_SEC: /* Intended falltrough */
          case TCPIP_IPSEC_IPV4_E_SEC: /* Intended falltrough */
          case TCPIP_IPSEC_IPV4_CIPSO: /* Intended falltrough */
          case TCPIP_IPSEC_IPV4_RA: /* Intended falltrough */
          case TCPIP_IPSEC_IPV4_SDBM: /* Intended falltrough */
          {
            /* Intended to be empty. Skip immutable options */
            break;
          }
          case TCPIP_IPSEC_IPV4_EOOL:
          {
            /* Exit at end of options */
            optionOffset = allOptionsLen;
            break;
          }
          default:
          {
            /* Zero known mutable and unknown options */
            TS_MemBZero(&optionPtr[optionOffset], currentOptionLen);
            break;
          }
        }
        /* Increase offset for next iteration */
        optionOffset += currentOptionLen;
      }

    } while((retVal == E_OK) && (optionOffset < allOptionsLen));
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_IpV4_rxIndicationIp_processHeaders
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(TcpIp_IpV4_HeaderType, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  uint8 const headerLength,
  boolean isBroadcast,
  boolean subNetCommunication,
  uint8 ctrlIdx
)
{
  TcpIp_Ip_RxReturnType RxRetCode = TCPIP_IP_RX_NOT_OK;
  boolean isFragmented = FALSE;
  uint16 const fragmentInfo = TCPIP_IPV4_FRAGMENTINFO_GET(dataPtr);
#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) reAssembleItemPtr= NULL_PTR;
#endif

  if (TCPIP_IPV4_RESERVED_FLAG_NOT_0(fragmentInfo))  /* reserved flag -> discard */
  {
    headerPtr->payloadPtr= NULL_PTR;                  /* make sure it's discarded */
    RxRetCode= TCPIP_IP_RX_FRAGMENT_RESERVED;
  }
  else if (TCPIP_IPV4_IS_FRAGMENTATION_NEEDED(fragmentInfo))    /* ignoring DF flag: IsFragment? */
  {
#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
    TcpIp_Ip_Frag_RxHeaderType frag;
    isFragmented = TRUE;
    /* most of header already parsed -> pass that data so we keep 'one source of truth' */
    TcpIp_IpV4_parseFragmentHeader(
      &frag,
      headerPtr->srcIp->addr[0U],
      headerPtr->destIp,
      dataPtr,
      headerLength,
      headerPtr->payloadLen,
      fragmentInfo,
      headerPtr->protocol);
    RxRetCode= TcpIp_IpV4_handleFragment(
      &frag,
      &reAssembleItemPtr,
      &dataPtr,
      &headerPtr->payloadPtr,
      &headerPtr->payloadLen);
#else
    /* Ip reassembly not configured: silently discard message */
    headerPtr->payloadPtr = NULL_PTR;
    RxRetCode = TCPIP_IP_RX_OK;
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */
  }
  else
  {
    RxRetCode = TCPIP_IP_RX_OK;
  }

  if(headerPtr->payloadPtr != NULL_PTR)
  {
    /* IpV4 message is accepted */
    if(subNetCommunication)
    {
      boolean ArpInsert = FALSE;
#if (TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE == STD_ON)
      ArpInsert = TcpIp_IpV4ArpPacketFilterAPI(localAddrId,dataPtr,headerPtr->lenByte);
#endif /* TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE == STD_ON */
      {
        /* remote host is from the same network (and valid address) - inform ARP for cache update */
        TcpIp_IpV4_ArpUpdate(headerPtr->srcIp->addr[0U],physAddrPtr,ctrlIdx,ArpInsert);
      }
    }

    {
      if (RxRetCode == TCPIP_IP_RX_OK)
      {
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
        if (Tcpip_IpV4_checkRxSecurity
            (localAddrId,
             headerPtr->srcIp->addr,
             dataPtr,
             headerLength,
             &headerPtr->payloadPtr,
             &headerPtr->payloadLen,
             &headerPtr->protocol,
             physAddrPtr) == E_OK
           )
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
        {

#if (TCPIP_ICMPV4_ENABLED == STD_ON)
          RxRetCode =
#else /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
            (void)
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
            TcpIp_IpV4_passToUpperLayer(headerPtr, localAddrId, isFragmented, isBroadcast);
        }
      }
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
      if(RxRetCode != TCPIP_IP_RX_OK)
      {
        /* check if error shall be reported to remote host */
        TcpIp_IpV4_handleErrors
          (RxRetCode, localAddrId, headerPtr->srcIp[0U], dataPtr, headerPtr->payloadLen, headerLength, isBroadcast);
      }
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */
    }
#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )
    /* if a reassembled datagram has been consumed, free reassembly buffer */
    if (reAssembleItemPtr != NULL_PTR)
    {
      /* rx-indication finished */
      TcpIp_Ip_Frag_returnBuffer(reAssembleItemPtr);
    }
#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */
  }
#if (!((TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) || (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) || (TCPIP_ICMPV4_ENABLED == STD_ON)))
  TS_PARAM_UNUSED(headerLength);
#endif /* (!((TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) || (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) || (TCPIP_ICMPV4_ENABLED == STD_ON))) */

  return RxRetCode;
}
#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
