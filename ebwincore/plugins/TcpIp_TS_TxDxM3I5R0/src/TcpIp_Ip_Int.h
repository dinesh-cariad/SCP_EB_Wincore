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
#ifndef TCPIP_IP_INT_H
#define TCPIP_IP_INT_H

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-3) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * Parentheses defeat the purpose of a compile-time-if macro: e.g. it could not be used
 * within a parameter list, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/

#include <Eth_GeneralTypes.h> /* hardware handle type */
#include <TcpIp_Ip_Api.h>     /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>        /* module internal headerfile */

/*==================[macros]================================================*/

/** \brief TcpIp_IcmpTransmit() service ID */
#define TCPIP_SID_ICMPTRANSMIT              0x0CU

/** \brief TcpIp_IcmpTransmit() service ID */
#define TCPIP_SID_ICMPTRANSMITERROR         0xF2U

/** \brief TcpIp_RxIndication() service ID */
#define TCPIP_SID_RXINDICATION              0x14U

/** \brief IP domain ID for IPV4 */
#define TCPIP_IP_DOMAIN_IPV4 0u
/** \brief IP domain ID for IPV6 */
#define TCPIP_IP_DOMAIN_IPV6 1u
/** \brief total number of domain pages IDs supported */
#define TCPIP_IP_NUMDOMAINS  TCPIP_IP_IF_V4_V6_BOTH(1u,1u,2u)

/** \brief minumal IP header for IPV4 */
#define TCPIP_IP_MINIMUM_IPV6_HEADER_LENGTH   40U
/** \brief minumal IP header for IPV6 */
#define TCPIP_IP_MINIMUM_IPV4_HEADER_LENGTH   20U

/** \brief Macro maps domain type to protocol page */
#define TCPIP_IP_DOMAIN_GET(domain) \
  ((domain) == TCPIP_AF_INET) ? TCPIP_IP_DOMAIN_IPV4 : TCPIP_IP_DOMAIN_IPV6

/** \brief Macro maps domain type to ip address size */
#define TCPIP_IP_GET_IP_ADDR_LEN(domain) \
  (((domain) == TCPIP_AF_INET) ? TCPIP_IPV4_ADDR_SIZE : TCPIP_IPV6_ADDR_SIZE)

/** \brief Macro maps domain type to ip address size */
#define TCPIP_IP_GET_IP_ADDR_BYTE_LEN(domain) \
  (((domain) == TCPIP_AF_INET) ? TCPIP_IPV4_ADDR_BYTE_SIZE : TCPIP_IPV6_ADDR_BYTE_SIZE)

/** \brief ICMP protocol identifier as assigned by IANA */
#define TCPIP_IP_PROTOCOL_ICMP      1u
/** \brief TCP protocol identifier as assigned by IANA */
#define TCPIP_IP_PROTOCOL_TCP       6u
/** \brief UDP protocol identifier as assigned by IANA */
#define TCPIP_IP_PROTOCOL_UDP       17u
/** \brief ICMPv6 protocol identifier as assigned by IANA */
#define TCPIP_IP_PROTOCOL_IPV6FRAG  44u
/** \brief ICMPv6 protocol identifier as assigned by IANA */
#define TCPIP_IP_PROTOCOL_ICMPV6    58u
/** \brief Extended Header Hop-by-Hop */
#define TCPIP_IP_EXT_HDR_HOPBYHOP   0u
/** \brief Extended Header No next Header (IPv6) */
#define TCPIP_IP_EXT_HDR_NONEXTHDR  59u
/** \brief Extended Header Destination Options (IPv6) */
#define TCPIP_IP_EXT_HDR_DSTOPT     60u
/** \brief Extended Header Fragment */
#define TCPIP_IP_EXT_HDR_FRAGMENT   44u
/** \brief Extended Header Routing */
#define TCPIP_IP_EXT_HDR_ROUTING    43u
/** \brief Extended Encapsuleted security payload header */
#define TCPIP_IP_EXT_HDR_ESP                50u
/** \brief Extended Header Authentication */
#define TCPIP_IP_EXT_HDR_AUTHENTICATION     51u

/** \brief Ethernet frame type containing IPV4 messages as assigned by IANA */
#define TCPIP_IP_FRAMETYPE_IPV4 0x800U
/** \brief Ethernet frame type containing ARP messages as assigned by IANA */
#define TCPIP_IP_FRAMETYPE_ARP  0x806U
/** \brief Ethernet frame type containing IPV6 messages as assigned by IANA */
#define TCPIP_IP_FRAMETYPE_IPV6 0x86ddU

/** \brief Compile time if-conversion:
 *  evaluating to
 *  the first parameter \a v4, if \a TCPIP_IPV4_ENABLED is \a STD_ON,
 *  the second parameter \a v6, if \a TCPIP_IPV6_ENABLED is \a STD_ON,
 *  else to the 3rd parameter \a both if \a TCPIP_IPV4_ENABLED and \a TCPIP_IPV6_ENABLED are both \a STD_ON.
 */
#if (TCPIP_IPV6_ENABLED == STD_ON) && (TCPIP_IPV4_ENABLED == STD_ON)
#  define TCPIP_IP_IF_V4_V6_BOTH(v4, v6, both) both
#elif (TCPIP_IPV6_ENABLED == STD_ON)
#  define TCPIP_IP_IF_V4_V6_BOTH(v4, v6, both) v6
#elif (TCPIP_IPV4_ENABLED == STD_ON)
#  define TCPIP_IP_IF_V4_V6_BOTH(v4, v6, both) v4
#endif

/** \brief Compile time if-conversion:
 *  evaluating to
 *  the first parameter \a udp, if \a TCPIP_UDP_ENABLED is \a STD_ON,
 *  the second parameter \a tcp, if \a TCPIP_TCP_ENABLED is \a STD_ON,
 *  else to the 3rd parameter \a both if \a TCPIP_UDP_ENABLED and \a TCPIP_TCP_ENABLED are both \a STD_ON.
 */
#if (TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON)
#  define TCPIP_IP_IF_UDP_TCP_BOTH(udp, tcp, both) both
#elif (TCPIP_UDP_ENABLED == STD_ON)
#  define TCPIP_IP_IF_UDP_TCP_BOTH(udp, tcp, both) udp
#elif (TCPIP_TCP_ENABLED == STD_ON)
#  define TCPIP_IP_IF_UDP_TCP_BOTH(udp, tcp, both) tcp
#endif

/** \brief Macro that subsumes, that both \a TCPIP_IPV6_ENABLED and \a TCPIP_IPV4_ENABLED
 * are \a STD_ON)*/
#define TCPIP_IP_DUALSTACK TCPIP_IP_IF_V4_V6_BOTH(STD_OFF, STD_OFF, STD_ON)

/** \brief Compile time if-conversion, evaluating to
 * \a then operand, if a DualStack configuration,
 * empty, if a SingleStack configuration
 */
#if (TCPIP_IPV6_ENABLED == STD_ON) && (TCPIP_IPV4_ENABLED == STD_ON)
#  define TCPIP_IP_IF_DUALSTACK(then) then
/** \brief conditional compilation macro to disable a parameter in case of static dispatch. */
#  define TCPIP_IP_DUALSTACK_PARAM(param)  param,
/** \brief conditional compilation macro to clean up unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_IP_DUALSTACK_UNUSED(param) /* empty */
#else
#  define TCPIP_IP_IF_DUALSTACK(then) /* empty */
/** \brief conditional compilation macro to disable a parameter in case of static dispatch. */
#  define TCPIP_IP_DUALSTACK_PARAM(param) /* empty */
/** \brief conditional compilation macro to clean up unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_IP_DUALSTACK_UNUSED(param) TS_PARAM_UNUSED(param)
#endif


/** \brief Configuration setting to use out-of-order transmission of IP fragments (header last). */
#define TCPIP_IP_FRAGMENT_TX_OUTOFORDER (STD_ON+1)

/** \brief Configuration setting to use in-order transmission of IP fragments (header first). */
#define TCPIP_IP_FRAGMENT_TX_INORDER ((TCPIP_IP_FRAGMENT_TX_OUTOFORDER)+1)

/** \brief Compile time if-conversion:
 * evaluating to the first parameter \a true, if
 *          \a TCPIP_IP_FRAGMENT_TX_ENABLED is not \a STD_OFF,
 *          else to the 2nd parameter \a false */
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF )
#  define TCPIP_IP_IF_FRAGMENT_TX(true, false) (false)
#else /* (TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF) */
#  define TCPIP_IP_IF_FRAGMENT_TX(true, false) (true)
#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF) */

/** \brief Define ticket \a TcpIp_Ip_IdTicketCounterType for a specific protocol layer. */
#define TCPIP_IP_DEFINEFRAGIDCOUNTER(counters) \
  STATIC VAR(TcpIp_Ip_IdTicketCounterType, TCPIP_VAR) \
    counters = { TCPIP_IP_DUALSTACK_PARAM(0u) 0u };

/** \brief magic value indicating VLAN is not used */
#define TCPIP_IP_VLANPRIO_UNUSED ((uint8)255U)
/** \brief function like macro checking for used VLAN */
#define TCPIP_IP_VLANPRIO_USED(vlanPrio) ((vlanPrio) != TCPIP_IP_VLANPRIO_UNUSED)

/** \brief Initializer for the \a TcpIp_Ip_IpParameterType Structure. */
#if (TCPIP_SECURITY_MODE <= TCPIP_FIREWALL)
#define TCPIP_IP_IPPARAMETER_INITIALIZER {0u,0u,0u,0u,0u,0u,0u,FALSE,FALSE,TRUE,TRUE}
#else
#define TCPIP_IP_IPPARAMETER_INITIALIZER {0u,0u,0u,0u,0u,0u,0u,0U,255U,FALSE,FALSE,TRUE,TRUE}
#endif

/** \brief default initialization value for variables of type TcpIp_Ip_EthTxHandleType */
#define TCPIP_IP_ETHTXHANDLE_INITIALIZER { NULL_PTR, 0u, 0u}

/** \brief Maximum header length supported by IPV4 reassembly. */
#define TCPIP_IPV4_MAX_HEADERS_LENGTH 60U
/** \brief Maximum header length supported by IPV6 reassembly. */
#define TCPIP_IPV6_MAX_HEADERS_LENGTH 40U

/** \brief Maximum header length supported by IP reassembly. */
#define TCPIP_IP_MAX_HEADERS_LENGTH   \
  TCPIP_IP_IF_V4_V6_BOTH(TCPIP_IPV4_MAX_HEADERS_LENGTH, \
                         TCPIP_IPV6_MAX_HEADERS_LENGTH, \
                         TCPIP_IPV4_MAX_HEADERS_LENGTH  \
                        )

/** \brief specific value for an NDP/ARP handle representing a multicast physical address */
#define TCPIP_IP_REMOTE_HANDLE_MULTICAST  0xFFFEU

/*==================[type definitions]======================================*/

/** \brief Structure containing Ip related Tx attributes */
typedef struct
{
  uint32 flowLabel;   /** < \brief flow Label */
  uint32 identifier; /* packet id */
  uint16 maxIpPayloadLength;  /**< \brief Indicates maximal IP payload size including AH header. */
  uint8 dscp;         /**< \brief Differentiated Services Code Point */
  uint8 vlanPriority; /**< \brief Frame priority used for Ethernet transmission. */
  uint8 ttl;          /**< \brief Time to live used for IP header transmission. */
  uint8 protocol;     /**< \brief Indicates the upper layer protocol */
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  uint8 addHeaderLength;    /**< \brief Indicates additional header lengths e.g. AH header */
  uint8 ipSecSaCommonIdx;  /**< \brief IPsec SA index */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  boolean mayFragment; /**< \brief inverted DF field of IP header: if true -> needs unique ID */
  boolean useUnspecifiedSrc; /**< \brief if true, the unspecified address is used
                                  as source address in IP packet */
  boolean calculateChecksum; /**< \brief If true, checksum is calculated */
  boolean calculatePseudoChecksum; /**< \brief If true, pseudo checksum is included */
} TcpIp_Ip_IpParameterType;

/** \brief Structure containing data-link-layer buffer information, which must
 * be used between ProvideTxBuffer and Transmit */
typedef struct
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr;    /**< \brief pointer to hardware data-link-layer buffer. */
  uint16 bufferLength;  /**< \brief length of hardware data-link-layer buffer. */
  Eth_BufIdxType bufferHandle; /**< \brief handle identifying the related data-link-layer buffer instance. */
} TcpIp_Ip_EthTxHandleType;    /* \brief This shouldn't exceed 12 bytes, as this info needs to be stored for
                                  the In-Order-Transmission option of Fragmentation */

/** \brief Pointer-type to \a TcpIp_Ip_EthTxHandleType */
typedef P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_EthTxHandlePtrType;

/** \brief Structure containing length information.
 * Indicates fragment packet if \a IPisFragment
 */
typedef struct
{
  uint16  IPoffset;      /**< \brief IP-transmit: start offset of the fragment (offset from IP header) */
  boolean IPmoreFlag;    /**< \brief IP-transmit: more fragments will follow */
  boolean IPisFragment;  /**< \brief IP-transmit: if fragmentation enabled: is this a fragment? */
} TcpIp_Ip_Frag_TxHeaderType;

/** \brief The ID counter type that hides the per-protocol IPv4/IPv6 IDENTIFICATION field counters */
typedef uint32 TcpIp_Ip_IdTicketCounterType[TCPIP_IP_NUMDOMAINS];

/* forward declaration */
struct TcpIp_Ip_StructUpdateHeaderFunctor;

/** \brief Function type of header completion callback function
 *
 * Functions of this type are called from Ip layer with a partial checksum
 * calculation of the pseudo Ip header and a pointer to the Ip payload given.
 * The layer implementing this function (e.g. Udp, Tcp) may finish checksum calculation
 * and enter the result into their protocol specific header.
 * For IP Fragmentation, the data is checksummed during copying and the checksum is already
 * included in the \a partialSum, so the callback is only left with checksumming the header.
 *
 * \param[in] that              contains information of the upper header like ports
 * \param[in] payloadSum        checksum of the payload and of the ip header
 *                              In case of Fragmentation, the copied payload data is also included here.
 * \param[in] headerPtr         Points to Ip payload memory area
 * \param[in] calculateChecksum If true calculate checksum.
 */
typedef P2FUNC(void,TCPIP_CODE,TcpIp_Ip_UpdateHeaderFctPtrType)
(
  /* Deviation MISRAC2012-3 */
  P2VAR(struct TcpIp_Ip_StructUpdateHeaderFunctor, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
);

/** \brief The base members of the \a TcpIp_Ip_UpdateHeaderFunctorType base class. */
#define TCPIP_IP_UPDATEHEADER_FIELDS            \
  TcpIp_Ip_UpdateHeaderFctPtrType       updateHeaderFctPtr;
  /* data fields will be added by derived classes. */

/** \brief pure virtual Base class of protocol-specific updateHeader functors.
 * Each protocol can add its own data to be provided to further upper layers (e.g. UDP) */
typedef struct TcpIp_Ip_StructUpdateHeaderFunctor
{
  TCPIP_IP_UPDATEHEADER_FIELDS
} TcpIp_Ip_UpdateHeaderFunctorType;


/* forward declaration */
struct TcpIp_Ip_StructCopyDataFunctor;

/** \brief functor member that receives the functor struct \a that as C++-like 'this' pointer.
 * \param that the functor struct, whose data member \a this must be casted to the specific derived type */
typedef P2FUNC(BufReq_ReturnType,TCPIP_CODE,TcpIp_Ip_CopyDataFctPtrType)
(
  /* Deviation MISRAC2012-3 */
  P2VAR(struct TcpIp_Ip_StructCopyDataFunctor, AUTOMATIC, TCPIP_APPL_DATA) that,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  uint16 copyLength
);


/** \brief The base members of the \a TcpIp_Ip_CopyDataFunctorType base class. */
#define TCPIP_IP_COPYDATA_FIELDS              \
  TcpIp_Ip_CopyDataFctPtrType   copyDataFctPtr;   \
  TcpIp_Ip_HandleIdType         handleId;
/* more data fields will be added by derived classes. */

/** \brief pure virtual Base class of protocol-specific CopyData functors.
 * Each protocol can add its own data to be provided to further upper layers (e.g. SoAd) */
typedef struct TcpIp_Ip_StructCopyDataFunctor
{
  TCPIP_IP_COPYDATA_FIELDS
} TcpIp_Ip_CopyDataFunctorType;

#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )

/** \brief Data structure to maintain a classic free-list for the not-yet-received fragments
 * within their parts of the reassembly data buffer. */
typedef struct
{
  uint32 iNext;
  uint16 iFirst;
  uint16 iLast;
} TcpIp_Ip_Frag_HoleDescriptorType;

/** \brief Data type to hold either IpV4 or IpV6 adresses, with minimal size, if IpV6 is disabled. */
typedef uint32 TcpIp_Ip_Frag_IpAddrType[TCPIP_IP_IF_V4_V6_BOTH(1,4,4)];

/** \brief abstract generalization of the ChannelId of IPv4 and IPv6. */
typedef struct
{
  uint32 identification;  /* has protocol in high bits for v4 */
  TcpIp_Ip_Frag_IpAddrType srcAddr;
  TcpIp_Ip_Frag_IpAddrType dstAddr;
} TcpIp_Ip_Frag_ChannelIdType;

/** \brief the reallocation buffer */
typedef struct {
  TcpIp_Ip_Frag_ChannelIdType channelId;
  TcpIp_Ip_Frag_HoleDescriptorType first_hole;
  uint32 size;    /* just the payload size */
  uint32 timestamp;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr;
  uint8 headerStart;
  boolean bUsed;
} TcpIp_Ip_Frag_RxBufferType;


typedef P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_CONST) TcpIp_Ip_Frag_RxBufferPtrType;

/** \brief abstract generalization of the header formats of IPv4 and IPv6.
 * allocation sizes big enough for V6, the high bytes are zero-padded for V4 */
typedef struct {
  TcpIp_Ip_Frag_ChannelIdType channelId;                /* fragment channel ID */
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr;   /* plain fragment data w/o header */
  uint16 lenDataByte;                                   /* fragment length without header */
  uint8 lenV46Header;                                   /* basic Ip header length*/
  uint16 offset;                                        /* fragment offset */
  uint8  nextHeader;                                    /* nextHeader value */
  boolean moreFrags;                                    /* indicates if there are more fragments */
} TcpIp_Ip_Frag_RxHeaderType;

#endif /* ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON ) */

/** \brief Data type to hold local address ID and remote address */
typedef struct
{
  TcpIp_LocalAddrIdType localAddrId;    /* local address ID */
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr; /* remote address pointer */
}TcpIp_Ip_AddrType;

/** \brief Attributes of a received Icmp message. */
typedef struct
{
  uint8 hopLimit;        /* Hop limit of the received icmp message */
  uint8 type;            /* Icmp message type */
  uint8 code;            /* Icmp error code */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) specificValue;  /* Value set before the payload  */
} TcpIp_Ip_Icmp_ParameterType;

/*==================[inclusions]============================================*/


#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

/* function declarations of specific implementations are included here after
 * the dependent type definitions
 * only include what's enabled.
 * */
#if TCPIP_IPV4_ENABLED == STD_ON
  #include <TcpIp_IpV4_Int.h>
#endif /* TCPIP_IPV4_ENABLED */
#if TCPIP_IPV6_ENABLED == STD_ON
  #include <TcpIp_IpV6_Int.h>
#endif /* TCPIP_IPV6_ENABLED */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON)
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initialises data structures of reassembly and TX fragmentation */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_init(void);

/** \brief Triggers periodic IP functions */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_mainFunction(void);

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)

/** \brief Return allocated reassembly buffer
 *
 * \param[in] bufferPtr  pointer to the reassembly buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_returnBuffer
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr
);

/** \brief Handle the Fragment \a frag.
 *
 *  \param[out] bufferPtrPtr  The fragment reassembly buffer pointer
 *  \param[in]  frag          Fragment header abstraction
 *
 *  \return TCPIP Standard return code.
 *  \retval TCPIP_IP_RX_OK:    The buffer \a bufferPtrPtr is complete and accessible (!=NULL)
 *                             The caller has to call \a returnBuffer() after consumption.
 *  \retval TCPIP_IP_RX_NOT_OK The buffer \a bufferPtrPtr is not yet complete, but accessble (!=NULL)
 *  \retval TCPIP_IP_RX_OUT_OF_BUFFERS No free reassembly buffer could be found (\a bufferPtrPtr==NULL)
 *  \par DEVIATIONS: According to RFC6946, atomic (complete, offset==more==0) fragments aren't
 *        handled as fragments. Thus they don't collide with partially-assembled buffers.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_Ip_Frag_reassemble
(
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtrPtr,
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) frag
);

#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

/** \brief transmit the IP packet.
 *
 * This function performs the remote address handle lookup (lock and unlock)
 * and invokes the transmission of the packet
 *
 * \param[in] copyDataFunctorPtr      The copyData functor.
 * \param[in] updateHeaderFunctorPtr  The updateHeader functor.
 * \param[in] ipParameterPtr          The IP parameter configuration block.
 * \param[in] localAddrId             Local address identifier
 * \param[in] remoteAddrPtr           Points to a structure containing the remote address information
 * \param[in] dataPtr                 The source data buffer. If NULL_PTR, the copyTxData interface is used.
 * \param[in] headerLength            length of the header
 * \param[in] totalLength             total length to transmit
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_transmit
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  uint16 totalLength
);

/** \brief transmit the IP packet.
 *
 * This function gets the maximal allowed message size and sets the message header and packet sizes
 * before invokes buffer allocation and transmission of packet
 *
 * \param[in] copyDataFunctorPtr      The copyData functor.
 * \param[in] updateHeaderFunctorPtr  The updateHeader functor.
 * \param[in] ipParameterPtr          The IP parameter configuration block.
 * \param[in] addrPtr                 remote and local address
 * \param[in] remoteHandle            remote address handle
 * \param[in] dataPtr                 pointer to data to be transmitted
 * \param[in] headerLength            length of the header
 * \param[in] upperLayerPayloadLength length of the upper layer payload to transmit
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_transmitIp
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) addrPtr,
  uint16 remoteHandle,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  uint16 upperLayerPayloadLength
);

#if ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON))

/** \brief Returns the LocalAddrId to use for transmission for a given remote IPv4/IPv6 address
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getTxLocalAddrId_byRemoteIp
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_Ip_getTxLocalAddrId_byRemoteIp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief Returns the LocalAddrId to use for transmission for a given LocalAddrId
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getTxLocalAddrId_byLocalAddrId
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_Ip_getTxLocalAddrId_byLocalAddrId
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief Returns the maximum payload possible in an IpV4/IpV6 frame considering the
 *         actual MTU and security header
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getMaximumPayload
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ip_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
);


#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Returns the minimal supported payload in an IpV4/IpV6 frame
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getMinimumPayload
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_Ip_getMinimumPayload
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Update cache of the remote host
 *
 * This service updates the cache entry and resets reachability timer if a remote host
 * acknowledged the reception of TCP data.
 *
 * \param[in]  CtrlIdx       EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  remoteAddrPtr IP address and domain of the remote host.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
/** \brief Evaluate new socket connectionaddress
 *
 * \param[in]  RemoteAddrPtr  IP address and port of the remote host to connect to.
 *
 * \retval TCPIP_OK           valid remote address
 * \retval TCPIP_E_NOT_OK     invalid remote address
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Ip_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Returns the state of the interface of the requested local address
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getIfState
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_Ip_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Obtain local IPv4/IPv6 address
 *
 * This API identifies the IP protocol and calls the corresponding TcpIp_Ip_getIpConfig
 * of the IP protocol.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
);

/** \brief Calculates partial checksum.
 *
 * Calculates partial checksum for Ip header.
 *
 * \param[in] remoteAddrPtr   remote Ip address
 * \param[in] localAddrPtr    local Ip address
 * \param[in] lenByte         length of header
 * \param[in] protocol        protocol used
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_Ip_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint16 lenByte,
  uint8 protocol
);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
/** \brief Compares to ip addresses
 *
 * Compares two ip addresses if they match
 *
 * \param[in] domainType   domain of the ip addresses (IPv4 or IPv6)
 * \param[in] AddrPtr1     first ip address to compare
 * \param[in] AddrPtr2     second ip address to compare
 *
 * \retval TCPIP_OK           ip addresses do match
 * \retval TCPIP_E_NOT_OK     ip addresses are not equal
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ip_compareIpAddr
(
  uint8 domainType,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr1,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr2
);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
/** \brief Checks if the given address is limited broadcast or all nodes multicast
 *
 * Checks if the given address is limited broadcast or all nodes multicast depending on the
 * domain of the address
 *
 * \param[in] ctrlIdx             controller index
 * \param[in] localAddrId         local address id
 * \param[in] localIpAddrPtr      pointer to the local address
 * \param[in] domainType          domain of the ip addresses (IPv4 or IPv6)
 *
 * \retval TRUE       ip addresses is limited broadcast / all nodes multicast
 * \retval FALSE      ip addresses isn't limited broadcast / all nodes multicast
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  TcpIp_DomainType domainType
);

/** \brief Gets range of local address ids for a given controller depending on the domain
 *
 * Gets range of local address ids for a given controller depending on the domain
 *
 * \param[in] ctrlIdx              controller index
 * \param[out] startIndexPtr       Index of first local address
 * \param[out] endIndexPtr         Index of first local address for next controller
 * \param[in] domainType           domain of the ip addresses (IPv4 or IPv6)
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr,
  TcpIp_DomainType domainType
);
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON) */

#endif /* ifndef TCPIP_IP_INT_H */
/*==================[end of file]===========================================*/
