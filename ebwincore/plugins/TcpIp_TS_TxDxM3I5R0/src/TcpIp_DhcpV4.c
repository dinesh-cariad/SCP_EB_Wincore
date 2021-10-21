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

/* Misra-C:2012 Deviations:
 *
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 * MISRAC2012-1) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 */

/*==================[inclusions]============================================*/

#include <EthIf.h>              /* EthIf API declarations */
#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */
#include <SchM_TcpIp.h>         /* SchM API declarations */
#include <TcpIp_Generic_Int.h>  /* generic upper layer API */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_Udp_Int.h>      /* used SW-unit interface file */

#include <TcpIp_DhcpV4_Int.h>   /* own SW-unit interface file */
#include <TcpIp_DhcpV4_Ext.h>   /* own SW-unit interface file */
#include <TcpIp_Dhcp_Priv.h>    /* own SW-unit private header file */

/*==================[macros]================================================*/

/** \brief TcpIp_DhcpReadOption() service ID */
#define TCPIP_SID_DHCPREADOPTION                              0x0dU

/** \brief TcpIp_DhcpWriteOption() service ID */
#define TCPIP_SID_DHCPWRITEOPTION                             0x0eU

/** \brief number of different dhcpv4 client methods */
#define TCPIP_DHCPV4_NUM_CLIENT_METHODS                          2U

/** \brief Size of the hardware address */
#define TCPIP_DHCPV4_HW_ADDR_LENGTH                              6U
/** \brief Ethernet hardware type */
#define TCPIP_DHCPV4_HWTYPE_ETHERNET                             1U
/** \brief length of an IP address in bytes */
#define TCPIP_DHCPV4_IP_ADDR_LENGTH                              4U

/** \brief Initial retransmission timeout */
#define TCPIP_DHCPV4_INITIAL_RETRANS_TIMEOUT                     4U
/** \brief Retransmission timeout for simple dhcp */
#define TCPIP_DHCPV4_SIMPLE_RETRANS_TIMEOUT                      1U
/** \brief Selecting timeout */
#define TCPIP_DHCPV4_SELECTING_TIMEOUT                         124U
/** \brief Maximal retransmission timeout */
#define TCPIP_DHCPV4_MAX_RETRANSMISSION_TIMEOUT                 64U
/** \brief Minimal retransmission timeout */
#define TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT                 60U

/** \brief Length of bootp message */
#define TCPIP_DHCPV4_HEADER_LEN                                236U
/** \brief Length of the magic cookie */
#define TCPIP_DHCPV4_MAGIC_COOKIE_SIZE                           4U
/** \brief Offset of the file field in DHCP header */
#define TCPIP_DHCPV4_FILE_OFFSET                               108U
/** \brief Length of the file field */
#define TCPIP_DHCPV4_FILE_LEN                                  128U
/** \brief Offset of the file field in DHCP header */
#define TCPIP_DHCPV4_SNAME_OFFSET                               44U
/** \brief Length of the sname field */
#define TCPIP_DHCPV4_SNAME_LEN                                  64U
/** \brief Length of the hw address pedding */
#define TCPIP_DHCPV4_PADDING_LEN                                10U

/** \brief Length of remaining chaddr bits, sname and file */
#define TCPIP_DHCPV4_UNUSED_LEN                                   \
  TCPIP_DHCPV4_PADDING_LEN + TCPIP_DHCPV4_SNAME_LEN + TCPIP_DHCPV4_FILE_LEN

/** \brief BOOTP opcode for DHCP requests */
#define TCPIP_DHCPV4_BOOTREQUEST                                 1U
/** \brief BOOTP opcode for DHCP responses */
#define TCPIP_DHCPV4_BOOTREPLY                                   2U

/** \brief Length of the magic cookie */
#define TCPIP_DHCPV4_MAGIC_COOKIE                       0x63825363U

/** \brief UDP port to which the client sends DHCP messages */
#define TCPIP_DHCPV4_SERVER_PORT                                67U
/** \brief UDP port to which the client sends DHCP messages */
#define TCPIP_DHCPV4_SIMPLE_SERVER_PORT                      10067U
/** \brief port on which to receive DHCP messages */
#define TCPIP_DHCPV4_CLIENT_PORT                                68U
/** \brief port on which to receive DHCP messages */
#define TCPIP_DHCPV4_SIMPLE_CLIENT_PORT                      10068U

/** \brief Secs counter is not running */
#define TCPIP_DHCPV4_SECS_COUNTER_STOP                      0xFFFFU

/** \brief Maximum valid lease time */
#define TCPIP_DHCPV4_MAX_VALID_LEASE_TIME               0x24924924U

/** \brief Pad option */
#define TCPIP_DHCPV4_OPTION_PAD                                  0U
#define TCPIP_DHCPV4_OPTION_PAD_LEN                              1U
/** \brief Subnet mask option */
#define TCPIP_DHCPV4_OPTION_SUBNET_MASK                          1U
/** \brief Router option */
#define TCPIP_DHCPV4_OPTION_ROUTER                               3U
/** \brief Requested Ip address option */
#define TCPIP_DHCPV4_OPTION_REQ_IP_ADDR                         50U
#define TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN                      4U
/** \brief Ip address lease time option */
#define TCPIP_DHCPV4_OPTION_LEASE_TIME                          51U
/** \brief Overload option */
#define TCPIP_DHCPV4_OPTION_OVERLOAD                            52U
/** \brief Message Type option */
#define TCPIP_DHCPV4_OPTION_MSG_TYPE                            53U
#define TCPIP_DHCPV4_OPTION_MSG_TYPE_LEN                         1U
/** \brief Server Identifier option */
#define TCPIP_DHCPV4_OPTION_SERVER_ID                           54U
#define TCPIP_DHCPV4_OPTION_SERVER_ID_LEN                        4U
/** \brief Renewal (T1) Time Value option */
#define TCPIP_DHCPV4_OPTION_T1_TIME                             58U
/** \brief Renewal (T2) Time Value option */
#define TCPIP_DHCPV4_OPTION_T2_TIME                             59U
/** \brief Parameter List option */
#define TCPIP_DHCPV4_OPTION_PARAM_LIST                          55U
#define TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN               2U
/** \brief End option */
#define TCPIP_DHCPV4_OPTION_END                                255U
/** \brief Minimal size of an option */
#define TCPIP_DHCPV4_OPTION_HDR_LEN                              2U
/** \brief Fully Qualifed Domain Name option */
#define TCPIP_DHCPV4_OPTION_FQDN                                81U
/** \brief DHCP FQDN Option Code Header Length */
#define TCPIP_DHCPV4_OPTION_FQDN_FIXED_SIZE                      3U
#define TCPIP_DHCPV4_OPTION_LEN_1_BYTE                           1U
#define TCPIP_DHCPV4_OPTION_LEN_4_BYTES                          4U

/* DHCP message types */
/** \brief Invalid message type */
#define TCPIP_DHCPV4_INVALID_MSG_TYPE                         0x00U
/** \brief client looks for DHCP servers */
#define TCPIP_DHCPV4_DISCOVER                                 0x01U
/** \brief server offers an IP address */
#define TCPIP_DHCPV4_OFFER                                    0x02U
/** \brief request use of an IP address */
#define TCPIP_DHCPV4_REQUEST                                  0x03U
/** \brief client declines server's acknowledge */
#define TCPIP_DHCPV4_DECLINE                                  0x04U
/** \brief server acknowledges client's request */
#define TCPIP_DHCPV4_ACK                                      0x05U
/** \brief server declines client's request */
#define TCPIP_DHCPV4_NAK                                      0x06U
/** \brief client releases its assigned IP address */
#define TCPIP_DHCPV4_RELEASE                                  0x07U
/** \brief client informs server about using an IP */
#define TCPIP_DHCPV4_INFORM                                   0x08U
/** \brief client looks for DHCP servers */
#define TCPIP_DHCPV4_SIMPLE_DISCOVER                          0x11U
/** \brief client requests an offered IP address */
#define TCPIP_DHCPV4_INT_REQUEST_OFFER                        0x13U
/** \brief client extends lease time */
#define TCPIP_DHCPV4_INT_REQUEST_EXTEND                       0x23U

/** \brief DHCP port closed */
#define TCPIP_DHCPV4_STATE_INACTIVE                              0U
/** \brief Wait for limited broadcast address */
#define TCPIP_DHCPV4_STATE_START_DELAY                           1U
/** \brief Startup state.*/
#define TCPIP_DHCPV4_STATE_INIT                                  2U
/** \brief Wait for DCHPOFFERs.*/
#define TCPIP_DHCPV4_STATE_SELECTING                             3U
/** \brief Requested specific lease.*/
#define TCPIP_DHCPV4_STATE_REQUESTING                            4U
/** \brief Lease has been assigned, is in use.*/
#define TCPIP_DHCPV4_STATE_BOUND                                 5U
/** \brief Renewing a lease. */
#define TCPIP_DHCPV4_STATE_RENEWING                              6U
/** \brief Requesting lease extension from any server.*/
#define TCPIP_DHCPV4_STATE_REBINDING                             7U
/** \brief Probing the chosen Ip address for uniqueness.*/
#define TCPIP_DHCPV4_STATE_PROBING                               8U

/** \brief option length of discover message */
#define TCPIP_DHCPV4_DISCOVER_LENGTH                             8U
/** \brief option length of simple discover message */
#define TCPIP_DHCPV4_SIMPLE_DISCOVER_LENGTH                      8U
/** \brief fixed option length of request message */
#define TCPIP_DHCPV4_REQUEST_FIXED_LENGTH                        8U

/** \brief Sets the broadcast flag, to indicate server to sent message
 * to limited broadcast */
#define TCPIP_DHCPV4_BROADCAST_FLAG                         0x8000U

/** \brief Terminator of the DHCP FQDN Option */
#define TCPIP_DHCPV4_DOMAINNAME_TERMINATOR                       0U
/** \brief Length of terminator DHCP FQDN Option */
#define TCPIP_DHCPV4_TERMINATOR_LENGTH                           1U

/** \brief Retry transmission in next mainfunction */
#define TCPIP_DHCPV4_FLAG_NXT_MAIN                            0x01U

/** \brief limited broadcast address assigned continue to state INIT */
#define TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED             0x02U

/** \brief Indicates if file field contains options */
#define TCPIP_DHCPV4_FILE_OR_FLAG                             0x01U
/** \brief Indicates of sname field contains options */
#define TCPIP_DHCPV4_SNAME_OR_FLAG                            0x02U

/** \brief delay for DISCOVER messages after conflict */
#define TCPIP_DHCPV4_CONFLICT_DISCOVER_DELAY                    10U

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)

#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
/** \brief maximum probe wait delay in seconds */
#define TCPIP_DHCPV4_PROBE_WAIT                                  1U
/** \brief maximum delay till repeated probe */
#define TCPIP_DHCPV4_PROBE_MAX                                   2U
/** \brief max conflicts before rate limiting*/
#define TCPIP_DHCPV4_MAX_CONFLICTS                              10U
/** \brief delay between successive attempts in seconds */
#define TCPIP_DHCPV4_RATE_LIMIT_INTERVAL                        61U

#if (TCPIP_DHCPV4_DAD_CONFIG_DEFAULT_PROBING_PARAM == STD_ON)
/** \brief number of probe messages */
#define TCPIP_DHCPV4_PROBE_NUM                                   3U
/** \brief random delay between 1 and 2 seconds until probe is repeated */
#define TCPIP_DHCPV4_PROBE_RANDOM                              TRUE
/** \brief delay before announcing (must not be zero) */
#define TCPIP_DHCPV4_ANNOUNCE_WAIT                               2U

#else /* (TCPIP_DHCPV4_DAD_CONFIG_DEFAULT_PROBING_PARAM == STD_ON) */

/** \brief following macros are for DoIp handling of Arp probing */
/** \brief number of probe messages */
#define TCPIP_DHCPV4_PROBE_NUM                                   1U
/** \brief delay of 1 seconds until probe is repeated */
#define TCPIP_DHCPV4_PROBE_RANDOM                             FALSE
/** \brief delay before announcing (must not be zero) */
#define TCPIP_DHCPV4_ANNOUNCE_WAIT                               1U

#endif /* (TCPIP_DHCPV4_DAD_CONFIG_DEFAULT_PROBING_PARAM == STD_ON) */

#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */

#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */

/** \brief Factor to calculate 1s to 100ms steps */
#define TCPIP_DHCPV4_100MS_TO_1S_FACTOR                         10U

/** \brief Checks if the NXT_MAIN flag is set */
#define TCPIP_DHCPV4_FLAG_NXT_MAIN_IS_SET(flags)                  \
  ((((flags) & TCPIP_DHCPV4_FLAG_NXT_MAIN) == TCPIP_DHCPV4_FLAG_NXT_MAIN))

/** \brief Checks if the BROADCAST flag is set */
#define TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_IS_SET(flags)             \
  ((((flags) & TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED) == TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED))

/** \brief Checks if the file overide flag is set */
#define TCPIP_DHCPV4_FLAG_FILE_OVERIDE_IS_SET(flags)              \
  ((((flags) & TCPIP_DHCPV4_FILE_OR_FLAG) == TCPIP_DHCPV4_FILE_OR_FLAG))

/** \brief Checks if the sname overide flag is set */
#define TCPIP_DHCPV4_FLAG_SNAME_OVERIDE_IS_SET(flags)             \
  ((((flags) & TCPIP_DHCPV4_SNAME_OR_FLAG) == TCPIP_DHCPV4_SNAME_OR_FLAG))

/** \brief Checks if simple dhcp client is configured */
#define TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx)                      \
  (TCPIP_CFG_TYPE(boolean,intDhcpV4Assign,(dhcpIdx)) == TRUE)

/** \brief Get message op code / message type */
#define TCPIP_DHCPV4_GET_OP(dhcphdr)                              \
  ((dhcphdr)[0U])

/** \brief Get hardware address type */
#define TCPIP_DHCPV4_GET_HTYPE(dhcphdr)                           \
  ((dhcphdr)[1U])

/** \brief Get hardware address length */
#define TCPIP_DHCPV4_GET_HLEN(dhcphdr)                            \
  ((dhcphdr)[2U])

/** \brief Get Dhcp hops */
#define TCPIP_DHCPV4_GET_HOPS(dhcphdr)                            \
  ((dhcphdr)[3U])

/** \brief Get the Transaction ID */
#define TCPIP_DHCPV4_GET_XID(dhcphdr)                             \
  TCPIP_GET32((dhcphdr), 4)

/** \brief Get the seconds, elapsed since client began address acquisition
           or renewal process */
#define TCPIP_DHCPV4_GET_SECS(dhcphdr)                            \
  TCPIP_GET16((dhcphdr), 8)

/** \brief Get the Dhcp Flags */
#define TCPIP_DHCPV4_GET_FLAGS(dhcphdr)                           \
  TCPIP_GET16((dhcphdr), 10)

/** \brief Get the 'your' (client) IP address */
#define TCPIP_DHCPV4_GET_YIADDR(ipAddr, dhcphdr)                  \
  TCPIP_GETCPY32(dhcphdr, 16, &(ipAddr))

/** \brief Get the magic cookie */
#define TCPIP_DHCPV4_GET_MAGIC_COOKIE(dhcphdr)                    \
  TCPIP_GET32((dhcphdr), 236) /* TCPIP_DHCPV4_HEADER_LEN */

/** \brief Get the subnet mask option */
#define TCPIP_DHCPV4_GET_SUBNETMASK(ipAddr, opthdr)               \
  TCPIP_GETCPY32(opthdr, 2, &(ipAddr))

/** \brief Get option id */
#define TCPIP_DHCPV4_GET_OPTION_ID(opthdr)                        \
  (opthdr)[0U]

/** \brief Get option size */
#define TCPIP_DHCPV4_GET_OPTION_SIZE(opthdr)                      \
  (opthdr)[1U]

/** \brief Get the router */
#define TCPIP_DHCPV4_GET_ROUTER(ipAddr, opthdr)                   \
  TCPIP_GETCPY32(opthdr, 2, &(ipAddr))

/** \brief Get the lease time */
#define TCPIP_DHCPV4_GET_LEASE_TIME(opthdr)                       \
  TCPIP_GET32((opthdr), 2)

/** \brief Get the overload */
#define TCPIP_DHCPV4_GET_OVERLOAD(opthdr)                         \
  (opthdr)[2U]

/** \brief Get the DHCP message type */
#define TCPIP_DHCPV4_GET_MSG_TYPE(opthdr)                         \
  (opthdr)[2U]

/** \brief Get the Renewal (T1) Time Value option */
#define TCPIP_DHCPV4_GET_T1_TIME(opthdr)                          \
  TCPIP_GET32((opthdr), 2)

/** \brief Get the Renewal (T2) Time Value option */
#define TCPIP_DHCPV4_GET_T2_TIME(opthdr)                          \
  TCPIP_GET32((opthdr), 2)

/** \brief Get the server identification */
#define TCPIP_DHCPV4_GET_SERVER_ID(ipAddr, opthdr)                \
  TCPIP_GETCPY32(opthdr, 2, &(ipAddr))

/** \brief Set the message op code / message type */
#define TCPIP_DHCPV4_SET_OP(dhcphdr, op)                          \
  (dhcphdr)[0U] = (op)

/** \brief Set the hardware address type */
#define TCPIP_DHCPV4_SET_HTYPE(dhcphdr, htype)                    \
  (dhcphdr)[1U] = (htype)

/** \brief the hardware address length */
#define TCPIP_DHCPV4_SET_HLEN(dhcphdr, hlen)                      \
  (dhcphdr)[2U] = (hlen)

/** \brief Set the Dhcp Hops */
#define TCPIP_DHCPV4_SET_HOPS(dhcphdr, hops)                      \
  (dhcphdr)[3U] = (hops)

/** \brief Set the Transaction ID */
#define TCPIP_DHCPV4_SET_XID(dhcphdr, xid)                        \
  TCPIP_SET32((dhcphdr), 4, (xid))

/** \brief Set the seconds elapsed since client began address acquisition
 *         or renewal process. */
#define TCPIP_DHCPV4_SET_SECS(dhcphdr, secs)                      \
  TCPIP_SET16((dhcphdr), 8, (secs))

/** \brief Set the Dhcp Flags */
#define TCPIP_DHCPV4_SET_FLAGS(dhcphdr, flags)                    \
  TCPIP_SET16((dhcphdr), 10, (flags))

/** \brief Set the client IP address */
#define TCPIP_DHCPV4_SET_CIADDR(dhcphdr, ipAddr)                  \
  TCPIP_SETCPY32(dhcphdr, 12, &(ipAddr))

/** \brief Get the pointer to Client Hardware address */
#define TCPIP_DHCPV4_GET_PTR_CHADDR(dhcphdr)                      \
  &(dhcphdr)[28U]

/** \brief Sets 'your' (client) IP address, IP address of next server
 *         to use in bootstrap, IP address of Relay agent to 0 */
#define TCPIP_DHCPV4_SET_UNUSED_ADDRESSES(dhcphdr)                \
  TS_MemSet(&(dhcphdr)[16U], 0U, 3U * TCPIP_DHCPV4_IP_ADDR_LENGTH)

/** \brief Sets remaining chaddr bits, sname and file to 0 */
#define TCPIP_DHCPV4_SET_UNUSED(dhcphdr)                          \
  TS_MemSet(&(dhcphdr)[34U], 0U, TCPIP_DHCPV4_UNUSED_LEN)

/** \brief Set the magic cookie */
#define TCPIP_DHCPV4_SET_MAGIC_COOKIE(dhcphdr)                    \
  TCPIP_SET32((dhcphdr), 0, TCPIP_DHCPV4_MAGIC_COOKIE)

/** \brief Set option id */
#define TCPIP_DHCPV4_SET_OPTION_ID(opthdr, id)                    \
  (opthdr)[0U] = (id)

/** \brief Set option size */
#define TCPIP_DHCPV4_SET_OPTION_LEN(opthdr, len)                  \
  (opthdr)[1U] = (len)

/** \brief Set the DHCP message type */
#define TCPIP_DHCPV4_SET_MSG_TYPE(opthdr, value)                  \
  (opthdr)[2U] = (value)

/** \brief Set the requested ip address */
#define TCPIP_DHCPV4_SET_REQ_IP_ADDR(opthdr, ip)                  \
  TCPIP_SETCPY32(opthdr, 2, &(ip))

/** \brief Set the server identifier */
#define TCPIP_DHCPV4_SET_SERVER_ID(opthdr, ip)                    \
  TCPIP_SETCPY32(opthdr, 2, &(ip))

/** \brief Set DHCP message type */
#define TCPIP_DHCPV4_SET_OPTION_MSG_TYPE(hdr, value)                      \
  do                                                                      \
  {                                                                       \
    TCPIP_DHCPV4_SET_OPTION_ID((hdr), TCPIP_DHCPV4_OPTION_MSG_TYPE);      \
    TCPIP_DHCPV4_SET_OPTION_LEN((hdr), TCPIP_DHCPV4_OPTION_MSG_TYPE_LEN); \
    TCPIP_DHCPV4_SET_MSG_TYPE((hdr), (value));                            \
  } while (0)

/** \brief Set Requested Ip Address */
#define TCPIP_DHCPV4_SET_OPTION_REQUESTED_IP(hdr, ip)                        \
  do                                                                         \
  {                                                                          \
    TCPIP_DHCPV4_SET_OPTION_ID((hdr), TCPIP_DHCPV4_OPTION_REQ_IP_ADDR);      \
    TCPIP_DHCPV4_SET_OPTION_LEN((hdr), TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN); \
    TCPIP_DHCPV4_SET_REQ_IP_ADDR((hdr), (ip));                               \
  } while (0)

/** \brief Set Server Identifier */
#define TCPIP_DHCPV4_SET_OPTION_SERVER_ID(hdr, ip)                         \
  do                                                                       \
  {                                                                        \
    TCPIP_DHCPV4_SET_OPTION_ID((hdr), TCPIP_DHCPV4_OPTION_SERVER_ID);      \
    TCPIP_DHCPV4_SET_OPTION_LEN((hdr), TCPIP_DHCPV4_OPTION_SERVER_ID_LEN); \
    TCPIP_DHCPV4_SET_SERVER_ID((hdr), (ip));                               \
  } while (0)

/** \brief Set Parameter default List */
  #define TCPIP_DHCPV4_SET_OPTION_PARAM_DEFAULT_LIST(hdr)                  \
  do                                                                       \
  {                                                                        \
    (hdr)[2U] = TCPIP_DHCPV4_OPTION_SUBNET_MASK;                           \
    (hdr)[3U] = TCPIP_DHCPV4_OPTION_ROUTER;                                \
  } while (0)

/** \brief Set the Dhcp FQDN Option Code */
#define TCPIP_DHCPV4_SET_OPT_FQDN_CODE(dhcphdr)                   \
  TCPIP_DHCPV4_SET_OPTION_ID((dhcphdr), TCPIP_DHCPV4_OPTION_FQDN)

/** \brief Set the length of Dhcp FQDN Option */
#define TCPIP_DHCPV4_SET_OPT_FQDN_LEN(dhcphdr, len)               \
  TCPIP_DHCPV4_SET_OPTION_LEN((dhcphdr), len)

/** \brief Clears the reserved MBZ field of Dhcp FQDN Option */
#define TCPIP_DHCPV4_CLEAR_OPT_FQDN_MBZ(dhcphdr)                  \
  (dhcphdr)[2U] &= 0x0FU;

/** \brief Sets the S bit to 0 of Dhcp FQDN Option */
#define TCPIP_DHCPV4_CLEAR_OPT_FQDN_S(dhcphdr)                    \
  (dhcphdr)[2U] &= 0xFEU;

/** \brief Sets the O bit to 0 of Dhcp FQDN Option */
#define TCPIP_DHCPV4_CLEAR_OPT_FQDN_O(dhcphdr)                    \
  (dhcphdr)[2U] &= 0xFDU;

/** \brief Sets the E bit to 1 of Dhcp FQDN Option */
#define TCPIP_DHCPV4_SET_OPT_FQDN_E(dhcphdr)                      \
  (dhcphdr)[2U] |= 0x04U;

/** \brief Sets the N bit to 1 of Dhcp FQDN Option */
#define TCPIP_DHCPV4_SET_OPT_FQDN_N(dhcphdr)                      \
  (dhcphdr)[2U] |= 0x08U;

/** \brief Clears the deprecated RCodes of Dhcp FQDN Option */
#define TCPIP_DHCPV4_CLEAR_OPT_FQDN_RCODE(dhcphdr)                \
  do                                                              \
  {                                                               \
    (dhcphdr)[3U] = 0U;                                           \
    (dhcphdr)[4U] = 0U;                                           \
  } while (0)                                                     \

/** \brief Sets the retransmission timer to the specified value */
#define TcpIp_DhcpV4_setRetransTimer(dhcpIdx, timeout)            \
  do                                                              \
  {                                                               \
    TcpIp_DhcpV4_Data[(dhcpIdx)].rtx_timer = (timeout);           \
    TcpIp_DhcpV4_Data[(dhcpIdx)].rtx_timeout = (timeout);         \
  } while (0)

/** \brief Sets the state timer to the specified value */
#define TcpIp_DhcpV4_setStateTimer(dhcpIdx, timeout)              \
  TcpIp_DhcpV4_Data[(dhcpIdx)].timer = (timeout)


/** \brief Sets the domain name of Dhcp FQDN Option */
#define TCPIP_DHCPV4_SET_OPT_FQDN_DOMAIN_NAME(dhcphdr, name, len) \
  TS_MemCpy(&(dhcphdr)[5U], name, len);

/** \brief Close DHCP socket
 *
 * In State SELECTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_stopAssign_SELECTING_INACTIVE(dhcpIdx)      \
  TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REQUESTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_stopAssign_REQUESTING_INACTIVE(dhcpIdx)     \
  TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State RENEWING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  localAddrId Local address id of the DHCP assignment
 * \param[in]  dhcpIdx     Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_stopAssign_RENEWING_INACTIVE(localAddrId, dhcpIdx) \
  TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE((localAddrId), (dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REBINDING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  localAddrId Local address id of the DHCP assignment
 * \param[in]  dhcpIdx     Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_stopAssign_REBINDING_INACTIVE(localAddrId, dhcpIdx) \
  TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE((localAddrId), (dhcpIdx))

/** \brief Delay transmission of DHCP DISCOVER message
 *
 * In State REQUESTING. This function reinitialize DHCP variables and delays the
 * transmission of the DHCP DISCOVER message by updating the transmission timeout.
 * (Called if NACK is received during request process.)
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_timeout_REQUESTING_INIT(dhcpIdx)          \
  TcpIp_DhcpV4_tr_received_REQUESTING_INIT((dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State REBINDING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message. (called if NACK is received.)
 *
 * \param[in]  localAddrId Local address id of the DHCP assignment
 * \param[in]  dhcpIdx     Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_timeout_REBINDING_INIT(localAddrId, dhcpIdx)     \
  TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_INIT((localAddrId), (dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State SELECTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_close_SELECTING_INACTIVE(dhcpIdx)           \
  TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REQUESTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_close_REQUESTING_INACTIVE(dhcpIdx)          \
  TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State RENEWING: This function informs address manager that IP shall no longer
 * be used and closes the DHCP socket.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
#define  TcpIp_DhcpV4_tr_close_RENEWING_INACTIVE(localAddrId, dhcpIdx)     \
  TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE((localAddrId), (dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REBINDING: This function informs address manager that IP shall no longer be
 * used and closes the DHCP socket.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
#define TcpIp_DhcpV4_tr_close_REBINDING_INACTIVE(localAddrId, dhcpIdx)     \
  TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE((localAddrId), (dhcpIdx))

/*==================[type definitions]======================================*/

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)

/** \brief This structure defines the runtime data of a DHCPv4 assignment. */
typedef struct
{
  uint32 timer;
  /**< Timer for the statemachine */
  uint32 rtx_timer;
  /**< Retransmission timer */
  uint32 lease;
  /**< Lease time.*/
  uint32 T1_time;
  /**< Renewal (T1) Time Value Option */
  uint32 T2_time;
  /**< Rebinding (T2) Time Value Option*/
  uint32 rtx_timeout;
  /**< Start value of rtx_timer.*/
  uint32 xid;
  /**< Transaction ID.*/
  uint32 gateway_addr;
  /**< Default Gateway */
  uint32 ip_server;
  /**< ip of the DHCP server */
  uint32 ip_addr;
  /**< assigned IP address */
  uint32 ip_req;
  /**< requested IP address */
  uint32 netmask;
  /**< Retrieved subnet mask */
  uint16 secs;
  /**< Seconds that passed since the beginning of the acquisition or renewal process.*/
  TcpIp_SocketIdType socketId;
  /**< id of the socket connection */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  uint16 option_length[TCPIP_DHCPV4_MAX_OPTIONS];
  /**< real option lengths */
  uint8 option_data[TCPIP_DHCPV4_MAX_OPTION_DATA];
  /**< option data for all options  */
#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
  uint8 state;
  /**< State of DHCP statemachine */
  uint8 num_retry;
  /**< number of retires */
  uint8 flags;
  /**< retry transmission in next mainfunction */
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
  uint8 domain_name[TCPIP_DHCPV4_DOMAINNAME_SIZE];
  /**< hostname of the dhcp client */
  uint8 domain_name_len;
  /**< length of the hostname */
#endif /* TCPIP_DHCPV4_FQDN_ENABLED == STD_ON */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
  uint8 conflicts;
  /**< counts number of conflicting ARPs */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
} TcpIp_DhcpV4_DataType;

/** \brief Holds important received DHCP options */
typedef struct
{
  uint32 netmask;
  /**< Subnet Mask Option */
  uint32 router;
  /**< Router Option */
  uint32 lease_time;
  /**< IP Address Lease Time Option */
  uint32 server_id;
  /**< Server Identifier Option */
  uint32 T1_time;
  /**< Renewal (T1) Time Value Option */
  uint32 T2_time;
  /**< Rebinding (T2) Time Value Option*/
  uint8 overload;
  /**< Overload option */
  uint8  msg_type;
  /**< DHCP Message Type Option */
}TcpIp_DhcpV4_OptionType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief get Ip address of the DHCP assignment
 *
 * This API function returns the IP address of the DHCP assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetIpAddr
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
);

/** \brief get netmask of the DHCP assignment
 *
 * This API function returns the netmask of the DHCP assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetNetmask
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief get gateway address of the DHCP assignment
 *
 * This API function returns the gateway IP address  of the DHCP assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the gateway address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetGateway
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr
);

#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)


/** \brief Set the FQDN Option
 *
 * This functions adds the FQDN option in DHCP REQUEST and DISCOVER messages.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  optptr   pointer to the DHCP option field in the DHCP message
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_setOptionFQDN
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
);

/** \brief Get total length of the DHCP FQDN Option
 *
 * This functions returns the length of the FQDN option
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 *
 * \return length of the FQDN option
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_getOptionFQDNLength
(
  uint8 dhcpIdx
);

#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) */

/** \brief Set the DHCP Option
 *
 * This functions adds the Dhcp request list option
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  optptr   pointer to the DHCP option field in the DHCP message
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_SetOptionRequestList
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_EvaluateRouterOptionLength
(
  uint8 addressLen
);

#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
/** \brief Set the DHCP Option
 *
 * This functions adds the Dhcp option
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  optptr   pointer to the DHCP option field in the DHCP message
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_setOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
);

/** \brief Get length of the DHCP options configured to be transmitted
 *
 * This functions returns the sum of all option lengths configured to be transmitted for the
 * respective DHCP index.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 *
 * \return length of all DHCP options
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getTxOptionsLength
(
  uint8 dhcpIdx
);

#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */

/** \brief Retrieve DHCP options
 *
 * This function extracts all DHCP options from the beginning of the data pointer until the
 * end of option field.
 *
 * \param[in]  dhcpIdx          Index to the DHCP data structure
 * \param[in]  optionsPtr       Points to the beginning of the option field in a received DHCP
 *                              messages
 * \param[in]  totalOptionsLen  Length of the option field
 * \param[out] opts             Holds values of the DHCP options
 *
 * \retval E_OK                 All options successfully extracted
 * \retval E_NOT_OK             Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_parseOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionsPtr,
  uint16 totalOptionsLen,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
);

/** \brief Retrieve DHCP option length
 *
 * This function returns the expected length of a known option and retrieves the length
 * from the option field in case its unknown.
 *
 * \param[in]  optPtr   Points to the beginning of the option field in a received DHCP
 *                      messages
 * \param[in]  leftlen  option length remaining
 *
 * \return length of the option
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getOptionLen
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 leftLen
);

/** \brief Retrieve DHCP options from option, sname and file field
 *
 * This function extracts all DHCP options from the option, sname and file field and
 * saves them in a structure.
 *
 * \param[in]  dhcpIdx Index to the DHCP data structure
 * \param[in]  dataPtr Points to the beginning of the bootp header
 * \param[in]  optlen  Length of the bootp message
 * \param[out] opts    Holds values of the DHCP options
 *
 * \retval E_OK        All options successfully extracted
 * \retval E_NOT_OK    Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_readOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
);

/** \brief Transmit DHCP message
 *
 * This function retrieves a hardware buffer, constructs the messages specified by
 * msg_type and hands the data over to the IP layer.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  remoteAddr   IP address and port of the lease server
 * \param[in]  msg_type     DHCP message that shall be transmitted
 *
 * \retval TCPIP_OK        Message successfully transmitted
 * \retval TCPIP_E_DROP    Link down, or out of memory
 * \retval TCPIP_E_NOT_OK  Transmission error occurred
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_transmit
(
  uint8 dhcpIdx,
  TcpIp_SockAddrInetType remoteAddr,
  uint8 msg_type
);

/** \brief Insert Dhcp message fields
 *
 * This function appends the DHCP header at the location specified by buffPtr.
 *
 * \param[in]  buffPtr      Pointer to the beginning of the DHCP header
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  msg_type     DHCP message that shall be transmitted
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_buildHeader
(
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) buffPtr,
  uint8 dhcpIdx,
  uint8 msg_type
);

/** \brief Get length of DHCP message
 *
 * This function returns the length of the DHCP messages specified by msg_type. (Called
 * to evaluate buffer size)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message of which the length shall be evaluated
 *
 * \return length of the DHCP messages
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getMsgLength
(
  uint8 dhcpIdx,
  uint8 msg_type
);

/** \brief Set message specific options
 *
 * DHCP messages contains different DHCP options, this function adds the messages
 * specific options to the DHCP message.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message of which the option shall be written
 * \param[in]  opthdr     Points to the beginning of the first specific option
 *
 * \return length of all the message specific options
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_setMsgSpecificOptions
(
  uint8 dhcpIdx,
  uint8 msg_type,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
);

/** \brief Calculate lease time
 *
 * This function calculates the lease time and the T1 and T2 renewal time.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  opt      Holds values of the received DHCP options
 *
 * \retval E_OK      operation successful
 * \retval E_NOT_OK  dhcp option contains an error
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_evaluateLeaseTime
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Process received DHCP messages
 *
 * This function decides what to do with a received DHCP message in a specific state
 * of the DHCP state machine.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  yiaddr       'your' (client) IP address of the DHCP message
 * \param[in]  opt          Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_received
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Process retransmission timeout
 *
 * This function decides what to do with a retransmission timeout in a specific state
 * of the DHCP state machine.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_retransTimeout
(
  uint8 dhcpIdx
);

/** \brief Process state timeout
 *
 * This function decides what to do if a lease timeout occurs in a specific state
 * of the DHCP state machine.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_timeout
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

/** \brief Address conflict detected
 *
 * This function is called when an address conflict is detected. It unassigns
 * the local address and tries to retrieve a new IP address from the dhcp server.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_conflict
(
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Open UDP socket
 *
 * This function delays DHCP assignment process until broadcast address is assigned
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_startAssign_INACTIVE_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Open UDP socket
 *
 * This function retrieves a UDP socket and binds it to the limited broadcast IP address.
 * It also sets a timeout for the first DHCP DISCOVER transmission.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);


/** \brief Stop DHCP assignment process
 *
 * In State INIT. This function reinitialize DHCP variables and moves
 * the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_START_DELAY_INACTIVE
(
  uint8 dhcpIdx
);

/** \brief Close DHCP socket
 *
 * In State INIT. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE
(
  uint8 dhcpIdx
);

/** \brief Close DHCP socket
 *
 * In State BOUND. This function closes the DHCP socket to stop listening for
 * incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

/** \brief Transmit Discover message
 *
 * This function transmits a DHCP DISCOVER message and specifies the retransmission
 * timeout.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_INIT_SELECTING
(
  uint8 dhcpIdx
);

/** \brief Expand IP address lease
 *
 * This function transmits a DHCP REQUEST message to the leasing server to expand the
 * lease time of the IP address.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_BOUND_RENEWING
(
  uint8 dhcpIdx
);

/** \brief Expand IP address lease
 *
 * This function transmits a DHCP BROADCAST REQUEST message to expand the lease
 * time of the IP address.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_RENEWING_REBINDING
(
  uint8 dhcpIdx
);

/** \brief Request Ip address
 *
 * This function transmits a DHCP REQUEST message to lease an IP address. (called
 * if OFFER is received.)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  yiaddr     'your' (client) IP address of the DHCP message
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_SELECTING_REQUESTING
(
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF)

/** \brief Assign IP address
 *
 * This function evaluates the lease timeouts and informs the IP address manager that
 * IP address can be used. (called if ACK is received.)
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  yiaddr     'your' (client) IP address of the DHCP message
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

#else /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */

/** \brief Start probing Ip address
 *
 * This function evaluates the lease timeouts and starts the ARP probing procedure
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  yiaddr     'your' (client) IP address of the DHCP message
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);


/** \brief Handle probe results for DhcpV4
 *
 * This function handles the result of ARP probing. In case the IP address is unique
 * it announces the IP, otherwise it generates a new IP and retries probing.
 *
 * \param[in] ctrlIdx         controller on which probing was processed
 * \param[in] unique          TRUE: IP address unique in network
 *                            FALSE: IP address used by other node
 * \param[in] remotePhysAddr  physical address of the remote host
 * \param[in] localPhysAddr   physical address of the local host
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_probeResult
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

/** \brief starts the timer for probing delay
 *
 * This function generates a random probing delay.
 *
 * \param[in] TcpIp_DhcpV4_DataPtr  Pointer to dhcp data structure
 *
 * \return probe_delay time
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_startProbeTimer
(
  P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr
);

/** \brief Close DHCP socket
 *
 * In State PROBING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_PROBING_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */

/** \brief Restart DHCP assignment process
 *
 * This function restarts the DHCP assignment process and delays the transmission of
 * the first DISCOVER message. (called if NACK is received.)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_INIT
(
  uint8 dhcpIdx
);

/** \brief Address conflict detected
 *
 * In State ASSIGNED. This function reinitialize DHCP variables, informs the
 * IP address manager to release the address and delays the
 * transmission of the DHCP DISCOVER message by updating the transmission timeout.
 * (Called if address conflict is detected.)
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_conflict_ASSIGNED_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);


/** \brief Restart DHCP assignment process
 *
 * This function restarts the DHCP assignment process, informs address manager that
 * IP shall no longer be used and delays the transmission of the first DISCOVER message.
 * (called if NACK is received.)
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

/** \brief Update renewing timeout
 *
 * In State RENEWING or REBINDING. This function updates the renewing timeout which indicates
 * the time when the DHCP tries to expand the lease time of the IP address.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_BOUND
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Retransmission of DHCP DISCOVER
 *
 * In State SELECTING. This function retransmits the DHCP DISCOVER message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_SELECTING_SELECTING
(
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCP DISCOVER
 *
 * In State REQUESTING. This function retransmits the DHCP REQUEST message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_REQUESTING_REQUESTING
(
  uint8 dhcpIdx
);

/** \brief Retransmission
 *
 * This function retransmits the specified message and doubles the retransmission timeout.
 * If the retransmission timeout reaches 64 seconds. The messages will be retransmitted
 * every 64 seconds.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message that shall be retransmitted
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_increase
(
  uint8 dhcpIdx,
  uint8 msg_type
);

/** \brief Retransmission of DHCP DISCOVER
 *
 * In State REQUESTING. This function retransmits the DHCP REQUEST message and decreases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_RENEWING_RENEWING
(
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCP REQUEST
 *
 * In State REBINDING. This function retransmits the DHCP REQUEST message and decreases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
);

/** \brief Retransmission
 *
 * This function retransmits the specified message and halfes the retransmission timeout.
 * If the retransmission timeout reaches 60 seconds. The messages will be retransmitted
 * every 64 seconds.
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  server_ip  IP address of the lease server
 * \param[in]  msg_type   DHCP message that shall be retransmitted
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_decrease
(
  uint8 dhcpIdx,
  uint32 server_ip,
  uint8 msg_type
);

#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_timeout_INIT_SELECTING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_retransTimeout_SELECTING_SELECTING
(
  uint8 dhcpIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_received_SELECTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_received_BOUND_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */

/** \brief Enter start_delay state
 *
 * This function starts the DHCP assignment process.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Entry active state
 *
 * This function retrieves a socket from the UDP module and binds it to the broadcast
 * ip address and DHCP client port.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  port         Port on which the client should listen for messages
 *
 * \retval TCPIP_OK       Socket ready to send DHCP requests
 *         else           Socket could not be created
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_entry_ACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint16 port
);

/** \brief Entry assigned state
 *
 * This function informs the IpAddrM that the IP address can be used
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Exit assigned state
 *
 * This function informs the IpAddrM that the IP address shall no longer be used.
 *
 * \param[in]  localAddrId  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_exit_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Exit active state
 *
 * This function closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_exit_ACTIVE
(
  uint8 dhcpIdx
);

/** \brief Enter inactive state
 *
 * This function reinitializes the runtime data to its default values.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_INACTIVE
(
  uint8 dhcpIdx
);

/** \brief Enter init state
 *
 * This function restarts the DHCP assignment process.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_INIT
(
  uint8 dhcpIdx
);

/** \brief Save received options
 *
 * This function stops retransmissions and starts the T1 renewal timer. (called if ACK
 * is received.)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_pre_entry_BOUND_time_update
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Enter bound state
 *
 * This function transitions state machine to state BOUND,
 * address is assigned for the first time, renewed or rebound.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_BOUND
(
  uint8 dhcpIdx
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/** \brief IP address has not been assigned yet */
STATIC CONST(uint32, TCPIP_CONST) TcpIp_DhcpV4_unspecified = 0x0U;

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief state information variable definition */
STATIC VAR(TcpIp_DhcpV4_DataType, TCPIP_VAR_CLEARED)
  TcpIp_DhcpV4_Data[TCPIP_NUM_DHCPV4_CLIENTS];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpReadOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8 Option,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPREADOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* report Det if TcpIp is not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_NOTINIT);
  }
  else if (LocalIpAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    /* report Det if Local Address exceeds number of configured addresses */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)

    if(TcpIp_IpAddrM_isAssignmentInvalid(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4))
    {
      /* report Det if Local Address is not of type DHCP */
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_INV_ARG);
    }
    else if(DataLength == NULL_PTR)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_EB_E_PARAM_POINTER);
    }
    else if(DataPtr == NULL_PTR)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_EB_E_PARAM_POINTER);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
#if ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)

      if(Option == TCPIP_DHCP_OPTION_FQDN)
      {
        uint16 len = *DataLength;
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
        P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr;

        TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

        TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        /* report Det if DataLength is not big enough for the fully qualified domain name */
        if((TcpIp_DhcpV4_DataPtr->domain_name_len != 0U)
           && (*DataLength < (TcpIp_DhcpV4_DataPtr->domain_name_len -
           TCPIP_DHCPV4_TERMINATOR_LENGTH)) )
        {
           TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_INV_ARG);
        }
        else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
        {
          TcpIp_Dhcp_readOptionFQDN(
                                     &len,
                                     DataPtr,
                                     &TcpIp_DhcpV4_DataPtr->domain_name[0U],
                                     TcpIp_DhcpV4_DataPtr->domain_name_len
                                   );

          *DataLength = (uint8)len;
          retval = E_OK;
        }
      }
      else
#endif /* TCPIP_DHCPV4_FQDN_ENABLED == STD_ON */
      {
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)

        uint16 len = *DataLength;
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
        CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionPtr =
            &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,0));
        TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

        retval =
          TcpIp_Dhcp_readOption
            (Option,
             &len,
             DataPtr,
             TcpIp_DhcpV4_Data[dhcpIdx].option_data,
             TcpIp_DhcpV4_Data[dhcpIdx].option_length,
             dhcpOptionPtr,
             TcpIp_PBcfgPtr->dhcpv4Config.optionNum
            );

        *DataLength = (uint8)len;

#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
        if (retval == E_NOT_OK)
#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
        {
         /* unknown option */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
      }
#else /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */
        TS_PARAM_UNUSED(LocalIpAddrId);
        TS_PARAM_UNUSED(Option);
        TS_PARAM_UNUSED(DataLength);
        TS_PARAM_UNUSED(DataPtr);
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        {
          /* unknown option */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPREADOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
#endif /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */
    }
  }

  DBG_TCPIP_DHCPREADOPTION_EXIT(retval,LocalIpAddrId,Option,DataLength,DataPtr);

  return retval;
}

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpWriteOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8 Option,
  uint8 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPWRITEOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* report Det if TcpIp is not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_NOTINIT);
  }
  else if(LocalIpAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    /* report Det if Local Address exeeds num of configured addresses */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)

    if(TcpIp_IpAddrM_isAssignmentInvalid(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4))
    {
      /* report Det if Local Address is not of type DHCP */
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_INV_ARG);
    }
    else if(DataPtr == NULL_PTR)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_EB_E_PARAM_POINTER);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
#if ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)

      if(Option == TCPIP_DHCP_OPTION_FQDN)
      {
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
        TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

        if(DataLength > (TCPIP_DHCPV4_DOMAINNAME_SIZE - 1U))
        {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
          /* report Det if data length is bigger than buffer size */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_INV_ARG);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
        }
        else
        {
          TcpIp_Dhcp_writeOptionFQDN(
                                      DataLength,
                                      DataPtr,
                                      &TcpIp_DhcpV4_Data[dhcpIdx].domain_name[0U],
                                      &TcpIp_DhcpV4_Data[dhcpIdx].domain_name_len
                                    );
          retval = E_OK;
        }
      }
      else
#endif /* TCPIP_DHCPV4_FQDN_ENABLED == STD_ON */
      {
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
        CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionPtr =
            &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,0));
        TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

        retval =
          TcpIp_Dhcp_writeOption
            (Option,
             DataLength,
             DataPtr,
             TcpIp_DhcpV4_Data[dhcpIdx].option_data,
             TcpIp_DhcpV4_Data[dhcpIdx].option_length,
             dhcpOptionPtr,
             TcpIp_PBcfgPtr->dhcpv4Config.optionNum
             );

#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
        if (retval == E_NOT_OK)
#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
        {
          /* unknown option or invalid option length */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
      }
#else /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */
        TS_PARAM_UNUSED(LocalIpAddrId);
        TS_PARAM_UNUSED(Option);
        TS_PARAM_UNUSED(DataLength);
        TS_PARAM_UNUSED(DataPtr);
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        {
          /* unknown option */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPWRITEOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
#endif /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */
    }
  }

  DBG_TCPIP_DHCPWRITEOPTION_EXIT(retval,LocalIpAddrId,Option,DataLength,DataPtr);

  return retval;
}

#if ((TCPIP_DHCPGETSTATUS_API == STD_ON) && (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON))
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_getStatus
(
  TcpIp_LocalAddrIdType LocalAddressId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) StatusPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPV4_GETSTATUS_ENTRY(LocalAddressId, StatusPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TcpIp_IpAddrM_isAssignmentInvalid(LocalAddressId, TCPIP_IPADDRM_ASSIGN_DHCPV4))
  {
    /* report Det if Local Address is not of type DHCP */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPGETSTATUS, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    const uint8 dhcpIdx =
      TcpIp_IpAddrM_getAssignmentIdx(LocalAddressId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
    TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

    TS_AtomicAssign8(*StatusPtr, TcpIp_DhcpV4_Data[dhcpIdx].state);

    retval = E_OK;
  }

  DBG_TCPIP_DHCPV4_GETSTATUS_EXIT(retval, LocalAddressId, StatusPtr);

  return retval;
}
#endif /* ((TCPIP_DHCPGETSTATUS_API == STD_ON) && (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)) */

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)

FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_rxIndication
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_DHCPV4_RXINDICATION_ENTRY(SocketId, RemoteAddrPtr, BufPtr, Length);

  TS_PARAM_UNUSED(RemoteAddrPtr);

  /* verify length of DHCP message */
  if((Length > (TCPIP_DHCPV4_HEADER_LEN + TCPIP_DHCPV4_MAGIC_COOKIE_SIZE)) &&
     (TCPIP_DHCPV4_GET_OP(BufPtr) == TCPIP_DHCPV4_BOOTREPLY) &&
     (TCPIP_DHCPV4_GET_HTYPE(BufPtr) == TCPIP_DHCPV4_HWTYPE_ETHERNET) &&
     (TCPIP_DHCPV4_GET_HLEN(BufPtr) == TCPIP_DHCPV4_HW_ADDR_LENGTH)
    )
  {
    TcpIp_DhcpV4_OptionType options;
    uint8_least i;

    TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr->dhcpv4Num <= TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);
    for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv4Num; i++)
    {
      if(TcpIp_DhcpV4_Data[i].socketId == SocketId)
      {
        if(TcpIp_DhcpV4_Data[i].xid == TCPIP_DHCPV4_GET_XID(BufPtr))
        {
          /* read options */
          retVal = TcpIp_DhcpV4_readOpts((uint8)i, BufPtr, Length, &options);

          if(retVal == E_OK)
          {
            uint32 yiaddr;
            const TcpIp_LocalAddrIdType localAddrId =
                    TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV4);

            TCPIP_DHCPV4_GET_YIADDR(yiaddr, BufPtr);

            TcpIp_DhcpV4_received(localAddrId, (uint8)i, yiaddr, &options);
          }

          break;
        }
      }
    }
  }

  DBG_TCPIP_DHCPV4_RXINDICATION_EXIT(SocketId, RemoteAddrPtr, BufPtr, Length);
}

FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_LocalIpAddrAssignmentChg
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
)
{
  uint8_least i;

  DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_ENTRY(IpAddrId,State);

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr->dhcpv4Num <= TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv4Num; i++)
  {
    const TcpIp_LocalAddrIdType dhcpLocalAddrId =
          TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV4);
    const TcpIp_LocalAddrIdType limLocalAddrId =
          TCPIP_CFG(ctrl,(TcpIp_IpAddrM_getCtrlIdx(dhcpLocalAddrId))).ctrlIpV4.localAddrLimdBroad;

    if(IpAddrId == limLocalAddrId)
    {
      if(State == TCPIP_IPADDR_STATE_ASSIGNED)
      {
        /* mark assignment of limited broadcast */
        TcpIp_DhcpV4_Data[i].flags |= TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED;

        if(TcpIp_DhcpV4_Data[i].state == TCPIP_DHCPV4_STATE_START_DELAY)
        {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
          if(TCPIP_DHCPV4_ISSIMPLECLIENT(i))
          {
            /* open DHCP socket, bind socket to 10068 and delay transmission of DISCOVER */
            TcpIp_DhcpV4_Simple_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(dhcpLocalAddrId, (uint8)i);
          }
          else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
          {
            /* transition to init if dhcp assignment was already started */
            TcpIp_DhcpV4_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(dhcpLocalAddrId, (uint8)i);
          }
        }
        /* bind socket if broadcast is transitioning UNASSIGNED -> ASSIGNED,
         * in case of transition ONHOLD -> ASSIGNED socket is not bind */
        else if((TcpIp_DhcpV4_Data[i].state != TCPIP_DHCPV4_STATE_INACTIVE) &&
                (TcpIp_DhcpV4_Data[i].socketId == TCPIP_SOCKETID_INVALID))
        {
          /* bind socket */
          (void)TcpIp_DhcpV4_entry_ACTIVE(dhcpLocalAddrId, (uint8)i, TCPIP_DHCPV4_CLIENT_PORT);
        }
        else
        {
         /* do nothing */
        }
      }
      else if(State == TCPIP_IPADDR_STATE_UNASSIGNED)
      {
        TcpIp_DhcpV4_Data[i].flags &= ~((uint8) TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED);

        if(TcpIp_DhcpV4_Data[i].socketId != TCPIP_SOCKETID_INVALID)
        {
          TcpIp_DhcpV4_exit_ACTIVE((uint8)i);
          TcpIp_DhcpV4_Data[i].socketId = TCPIP_SOCKETID_INVALID;
        }
      }
      else
      {
        /* do nothing */
      }
    }
  }
  DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_EXIT(IpAddrId,State);
}

FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_CopyTxData
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
)
{
  uint8_least i;

  DBG_TCPIP_DHCPV4_COPYTXDATA_ENTRY(SocketId,BufPtr,BufLength);

  TS_PARAM_UNUSED(BufLength);

  for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv4Num; i++)
  {
    if(TcpIp_DhcpV4_Data[i].socketId == SocketId)
    {
      uint8 msgType = TCPIP_DHCPV4_INVALID_MSG_TYPE;
      uint8 state;

      /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
      TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[i].state);

      switch(state)
      {
        case TCPIP_DHCPV4_STATE_SELECTING:
        {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
          if(TCPIP_DHCPV4_ISSIMPLECLIENT(i))
          {
            msgType = TCPIP_DHCPV4_SIMPLE_DISCOVER;
          }
          else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
          {
            msgType = TCPIP_DHCPV4_DISCOVER;
          }
          break;
        }
        case TCPIP_DHCPV4_STATE_REQUESTING:
          msgType = TCPIP_DHCPV4_INT_REQUEST_OFFER;
          break;
        case TCPIP_DHCPV4_STATE_RENEWING:
          if(TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_IS_SET(TcpIp_DhcpV4_Data[i].flags))
          {
            msgType = TCPIP_DHCPV4_INT_REQUEST_EXTEND;
          }
          else
          {
            msgType = TCPIP_DHCPV4_DECLINE;
          }
          break;
        case TCPIP_DHCPV4_STATE_REBINDING:
          if(TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_IS_SET(TcpIp_DhcpV4_Data[i].flags))
          {
            msgType = TCPIP_DHCPV4_INT_REQUEST_EXTEND;
          }
          else
          {
            msgType = TCPIP_DHCPV4_DECLINE;
          }
          break;
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
        case TCPIP_DHCPV4_STATE_PROBING: /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
        case TCPIP_DHCPV4_STATE_BOUND:
          msgType = TCPIP_DHCPV4_DECLINE;
          break;
        /* CHECK: NOPARSE */
        case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
        case TCPIP_DHCPV4_STATE_INACTIVE: /* intended fall through */
        case TCPIP_DHCPV4_STATE_INIT:   /* intended fall through */
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }

      TCPIP_PRECONDITION_ASSERT
        (TcpIp_DhcpV4_getMsgLength( ((uint8)i) , msgType) == BufLength, TCPIP_INTERNAL_API_ID);

      TCPIP_PRECONDITION_ASSERT
      (
        BufLength >=
        (
          (uint16) (TCPIP_DHCPV4_HEADER_LEN + TCPIP_DHCPV4_MAGIC_COOKIE_SIZE + TCPIP_DHCPV4_OPTION_HDR_LEN
          + TCPIP_DHCPV4_OPTION_MSG_TYPE_LEN) + ((msgType == TCPIP_DHCPV4_INT_REQUEST_OFFER) ? (TCPIP_DHCPV4_OPTION_HDR_LEN
          + TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN + TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_SERVER_ID_LEN) : 0U)
          + ((msgType == TCPIP_DHCPV4_DISCOVER)? (TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN) : 0U)
        ),
        TCPIP_INTERNAL_API_ID
      );

      TcpIp_DhcpV4_buildHeader(BufPtr, (uint8)i, msgType);
    }
  }
  DBG_TCPIP_DHCPV4_COPYTXDATA_EXIT(BUFREQ_OK,SocketId,BufPtr,BufLength);

  return BUFREQ_OK;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_init
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr;
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  uint8 i;
#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */
  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);
  TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  DBG_TCPIP_DHCPV4_INIT_ENTRY(localAddrId);

  TcpIp_DhcpV4_DataPtr->T1_time = 0U;
  TcpIp_DhcpV4_DataPtr->T2_time = 0U;
  TcpIp_DhcpV4_DataPtr->gateway_addr = 0U;
  TcpIp_DhcpV4_DataPtr->ip_addr = 0U;
  TcpIp_DhcpV4_DataPtr->ip_server = 0U;
  TcpIp_DhcpV4_DataPtr->ip_req = 0U;
  TcpIp_DhcpV4_DataPtr->lease = 0U;
  TcpIp_DhcpV4_DataPtr->netmask = 0U;
  TcpIp_DhcpV4_DataPtr->secs = TCPIP_DHCPV4_SECS_COUNTER_STOP;
  TcpIp_DhcpV4_DataPtr->socketId = TCPIP_SOCKETID_INVALID;
  TcpIp_DhcpV4_DataPtr->state = TCPIP_DHCPV4_STATE_INACTIVE;
  TcpIp_DhcpV4_DataPtr->xid = 0U;
  TcpIp_DhcpV4_DataPtr->flags = 0U;
  TcpIp_DhcpV4_DataPtr->num_retry = 0U;
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
  TcpIp_DhcpV4_DataPtr->conflicts = 0U;
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
  TcpIp_DhcpV4_setRetransTimer(dhcpIdx, 0U);
  TcpIp_DhcpV4_setStateTimer(dhcpIdx, 0U);
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
  TcpIp_DhcpV4_DataPtr->domain_name_len = 0U;
#endif /* TCPIP_DHCPV4_FQDN_ENABLED == STD_ON */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  for (i = 0U; i<TcpIp_PBcfgPtr->dhcpv4Config.optionNum; i++)
  {
    TcpIp_DhcpV4_DataPtr->option_length[i] = 0U;
  }
#endif /* TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON */

  DBG_TCPIP_DHCPV4_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  uint8 state;

  DBG_TCPIP_DHCPV4_STARTASSIGN_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_INACTIVE:
    {
      TcpIp_DhcpV4_tr_startAssign_INACTIVE_START_DELAY(localAddrId, dhcpIdx);
      break;
    }
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:        /* intended fall through */
    case TCPIP_DHCPV4_STATE_REQUESTING:  /* intended fall through */
    case TCPIP_DHCPV4_STATE_BOUND:       /* intended fall through */
    case TCPIP_DHCPV4_STATE_RENEWING:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_REBINDING:   /* intended fall through */
    case TCPIP_DHCPV4_STATE_SELECTING:   /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      /* do nothing */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_DHCPV4_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_DHCPV4_STOPASSIGN_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_START_DELAY:
      /* close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_START_DELAY_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_INIT:
      /* close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_SELECTING:
      /* close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_SELECTING_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_REQUESTING:
      /* close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_REQUESTING_INACTIVE(dhcpIdx);
      break;
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:
      /* close DHCP socket and stop ARP probing */
      TcpIp_DhcpV4_tr_stopAssign_PROBING_INACTIVE(localAddrId, dhcpIdx);
      break;
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV4_STATE_BOUND:
      /* inform address manager, close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE(localAddrId, dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_RENEWING:
      /* inform address manager, close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_RENEWING_INACTIVE(localAddrId, dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_REBINDING:
      /* inform address manager, close DHCP socket */
      TcpIp_DhcpV4_tr_stopAssign_REBINDING_INACTIVE(localAddrId, dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_INACTIVE:  /* intended fall through */
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_DHCPV4_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_mainFunction
(
  uint16 timerTick
)
{
  uint8_least i;

  DBG_TCPIP_DHCPV4_MAINFUNCTION_ENTRY(timerTick);

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr->dhcpv4Num <= TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);
  /* cycle through all dhcp interface in our list */
  for (i = 0U; i < TcpIp_PBcfgPtr->dhcpv4Num; ++i)
  {
    boolean retransmit = FALSE;
    boolean timeout = FALSE;
    const TcpIp_LocalAddrIdType localAddrId =
            TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV4);
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                            TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[i];

    /* check for retires */
    if(TCPIP_DHCPV4_FLAG_NXT_MAIN_IS_SET(TcpIp_DhcpV4_DataPtr->flags))
    {
      if(TcpIp_DhcpV4_DataPtr->num_retry < TcpIp_PBcfgPtr->transRetryNum)
      {
        /* try to retransmit dhcp message until max number is reached */
        TcpIp_DhcpV4_DataPtr->num_retry++;
        retransmit = TRUE;
      }
      else
      {
        /* stop retransmitting, retry when next retransmission or
           state timeout occurs */
        TcpIp_DhcpV4_DataPtr->num_retry = 0U;
        TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);
      }
    }

    if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_1S) == TCPIP_TIMER_OFFSET_1S_DHCP)
    {
      /* verify if message shall be retransmitted */
      if(TcpIp_DhcpV4_DataPtr->rtx_timer > 0U)
      {
        TcpIp_DhcpV4_DataPtr->rtx_timer--;

        if(TcpIp_DhcpV4_DataPtr->rtx_timer == 0U)
        {
          /* timer expired, retransmit */
          retransmit = TRUE;
        }
      }

      /* verify if state timeout occurred */
      if(TcpIp_DhcpV4_DataPtr->timer > 0U)
      {
        TcpIp_DhcpV4_DataPtr->timer--;

        if(TcpIp_DhcpV4_DataPtr->timer == 0U)
        {
          timeout = TRUE;

          /* if retries are ongoing, stop them */
          TcpIp_DhcpV4_DataPtr->num_retry = 0U;
          TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);
        }
      }

      if(TcpIp_DhcpV4_DataPtr->secs != TCPIP_DHCPV4_SECS_COUNTER_STOP)
      {
        /* increment seconds counter */
        TcpIp_DhcpV4_DataPtr->secs++;
      }
    }

    if(timeout)
    {
      /* timer expired, retransmit */
      TcpIp_DhcpV4_timeout(localAddrId, (uint8) i);
    }
    else if(retransmit)
    {
      /* try to transmit DHCP message again */
      TcpIp_DhcpV4_retransTimeout((uint8) i);
    }
    else
    {
      /* nothing to do here */
    }
  }

  DBG_TCPIP_DHCPV4_MAINFUNCTION_EXIT(timerTick);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_DHCPV4_GETIPCONFIG_ENTRY
    (localAddrId, assignedIpPtrPtr, gatewayIpPtrPtr, netmaskIpPtrPtr);

  TcpIp_DhcpV4_intGetIpAddr(localAddrId, assignedIpPtrPtr);
  TcpIp_DhcpV4_intGetNetmask(localAddrId, netmaskIpPtrPtr);
  TcpIp_DhcpV4_intGetGateway(localAddrId, gatewayIpPtrPtr);

  DBG_TCPIP_DHCPV4_GETIPCONFIG_EXIT
    (localAddrId, assignedIpPtrPtr, gatewayIpPtrPtr, netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
)
{
  DBG_TCPIP_DHCPV4_GETIPADDRESS_ENTRY(localAddrId, assignedIpPtrPtr);

  TcpIp_DhcpV4_intGetIpAddr(localAddrId, assignedIpPtrPtr);

  DBG_TCPIP_DHCPV4_GETIPADDRESS_EXIT(localAddrId, assignedIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4_isAssigned
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 dhcpIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  uint8 state;
  boolean result = FALSE;

  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  DBG_TCPIP_DHCPV4_ISASSIGNED_ENTRY(localAddrId);

  if((state == TCPIP_DHCPV4_STATE_BOUND) ||
     (state == TCPIP_DHCPV4_STATE_RENEWING) ||
     (state == TCPIP_DHCPV4_STATE_REBINDING))
  {
    result = TRUE;
  }

  DBG_TCPIP_DHCPV4_ISASSIGNED_EXIT(result, localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
)
{
  const uint8 numAnnounce = TcpIp_PBcfgPtr->ipV4Config.numGratuitousARP;

  DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);

  if(numAnnounce > 0U)
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    if(state)
    {
      const uint8 dhcpIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId,TCPIP_IPADDRM_ASSIGN_DHCPV4);

      TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

      /* inform others about assigned ip address by transmitting arp announces */
      TcpIp_IpV4_ArpAnnounce(TcpIp_DhcpV4_Data[dhcpIdx].ip_addr, numAnnounce, 1U, localAddrPtr->ctrlIdx, TCPIP_IPADDRM_ASSIGN_DHCPV4);
    }
    else
    {
      /* stop announcing ip address */
      TcpIp_IpV4_ArpAnnounce(TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, localAddrPtr->ctrlIdx, TCPIP_IPADDRM_ASSIGN_DHCPV4);
    }
  }

  {
    if(state)
    {
      const uint8 addrDefend = TcpIp_PBcfgPtr->dhcpv4Config.addrDefendMechanism;

      /* listen for address conflicts */
      TcpIp_IpV4_AddrConflDetect_activate(localAddrId, &TcpIp_DhcpV4_conflict, addrDefend);
    }
    else
    {
      /* stop listen for ARP conflicts */
      TcpIp_IpV4_AddrConflDetect_activate
        (localAddrId, NULL_PTR, TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF);
    }
  }

  DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(LocalIpAddrPtr);

  return E_OK;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetIpAddr
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:        /* intended fall through */
    case TCPIP_DHCPV4_STATE_REQUESTING:  /* intended fall through */
    case TCPIP_DHCPV4_STATE_SELECTING:   /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      *assignedIpPtrPtr = &TcpIp_DhcpV4_unspecified;
      break;
    case TCPIP_DHCPV4_STATE_BOUND:       /* intended fall through */
    case TCPIP_DHCPV4_STATE_RENEWING:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_REBINDING:   /* intended fall through */
      *assignedIpPtrPtr = &TcpIp_DhcpV4_Data[dhcpIdx].ip_addr;
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetNetmask
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);

  P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr;
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_DataPtr->state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:        /* intended fall through */
    case TCPIP_DHCPV4_STATE_REQUESTING:  /* intended fall through */
    case TCPIP_DHCPV4_STATE_SELECTING:   /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      *netmaskIpPtrPtr = &TcpIp_DhcpV4_unspecified;
      break;
    case TCPIP_DHCPV4_STATE_BOUND:       /* intended fall through */
    case TCPIP_DHCPV4_STATE_RENEWING:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_REBINDING:   /* intended fall through */
    {
      if(TcpIp_DhcpV4_DataPtr->netmask != 0U)
      {
        *netmaskIpPtrPtr = &TcpIp_DhcpV4_DataPtr->netmask;
      }
      else
      {
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

        /* netmask from DHCP server not received use static one */
        *netmaskIpPtrPtr =
          &TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV4.netMaskStatic;
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
}
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_intGetGateway
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);

  P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr;
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

  TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_DataPtr->state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:        /* intended fall through */
    case TCPIP_DHCPV4_STATE_REQUESTING:  /* intended fall through */
    case TCPIP_DHCPV4_STATE_SELECTING:   /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      *gatewayIpPtrPtr = &TcpIp_DhcpV4_unspecified;
      break;
    case TCPIP_DHCPV4_STATE_BOUND:       /* intended fall through */
    case TCPIP_DHCPV4_STATE_RENEWING:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_REBINDING:   /* intended fall through */
    {
      if(TcpIp_DhcpV4_DataPtr->gateway_addr != 0U)
      {
        *gatewayIpPtrPtr = &TcpIp_DhcpV4_DataPtr->gateway_addr;
      }
      else
      {
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

        /* gateway address from DHCP server not received use static one */
        *gatewayIpPtrPtr = &TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV4.defaultGWIpStatic;
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
}

#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_setOptionFQDN
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  const uint8 OptionLength =
      TCPIP_DHCPV4_OPTION_FQDN_FIXED_SIZE + TcpIp_DhcpV4_DataPtr->domain_name_len +
      TCPIP_DHCPV4_TERMINATOR_LENGTH;

    if(TcpIp_DhcpV4_DataPtr->domain_name_len != 0U)
    {
      const uint8 DomainNameTerminatorIdx =
        TCPIP_DHCPV4_OPTION_HDR_LEN + OptionLength - 1U;

      TCPIP_DHCPV4_SET_OPT_FQDN_CODE(optptr);
      /* length of the option */
      TCPIP_DHCPV4_SET_OPT_FQDN_LEN(optptr, OptionLength);
      /* must be set to zero */
      TCPIP_DHCPV4_CLEAR_OPT_FQDN_MBZ(optptr);
      /* server should not perform the A RR (FQDN-to-address) DNS updates */
      TCPIP_DHCPV4_CLEAR_OPT_FQDN_S(optptr);
      /* must be set to zero */
      TCPIP_DHCPV4_CLEAR_OPT_FQDN_O(optptr);
      /* server SHOULD NOT perform any DNS updates */
      TCPIP_DHCPV4_SET_OPT_FQDN_N(optptr);
      /* canonical wire format, without compression */
      TCPIP_DHCPV4_SET_OPT_FQDN_E(optptr);
      /* deprecated, must be set to zero */
      TCPIP_DHCPV4_CLEAR_OPT_FQDN_RCODE(optptr);

      TCPIP_DHCPV4_SET_OPT_FQDN_DOMAIN_NAME
                                      (
                                        optptr,
                                        TcpIp_DhcpV4_DataPtr->domain_name,
                                        TcpIp_DhcpV4_DataPtr->domain_name_len
                                      );

      /* set terminator */
      optptr[DomainNameTerminatorIdx] = TCPIP_DHCPV4_DOMAINNAME_TERMINATOR;
    }
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_getOptionFQDNLength
(
  uint8 dhcpIdx
)
{
  uint8 len = 0U;

  if(TcpIp_DhcpV4_Data[dhcpIdx].domain_name_len != 0U)
  {
    len = TCPIP_DHCPV4_OPTION_HDR_LEN +
          TCPIP_DHCPV4_OPTION_FQDN_FIXED_SIZE +
          TcpIp_DhcpV4_Data[dhcpIdx].domain_name_len +
          TCPIP_DHCPV4_TERMINATOR_LENGTH;
  }

  return len;
}

#endif /* TCPIP_DHCPV4_FQDN_ENABLED == STD_ON */


#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_setOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
)
{
  uint8 i = 0U;

  for(i=0U; i<TcpIp_PBcfgPtr->dhcpv4Config.optionNum; i++)
  {
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                            TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
    if ( TcpIp_DhcpV4_DataPtr->option_length[i] != 0U )
    {
      CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOption =
        &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,i));

      if(dhcpOption->optionTransmit)
      {
        if(dhcpOption->optionCode != TCPIP_DHCPV4_OPTION_PARAM_LIST)
        {
          TCPIP_DHCPV4_SET_OPTION_ID(optptr, (uint8)dhcpOption->optionCode);
          /* length of the option */
          TCPIP_DHCPV4_SET_OPTION_LEN(optptr, (uint8)TcpIp_DhcpV4_DataPtr->option_length[i]);
          TS_MemCpy( &(optptr)[TCPIP_DHCPV4_OPTION_HDR_LEN],
                     &TcpIp_DhcpV4_DataPtr->option_data[dhcpOption->optionDataOffset],
                     TcpIp_DhcpV4_DataPtr->option_length[i]
                   );
          optptr = &optptr[TCPIP_DHCPV4_OPTION_HDR_LEN + TcpIp_DhcpV4_DataPtr->option_length[i]];
        }
      }
    }
  }
}


STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getTxOptionsLength
(
  uint8 dhcpIdx
)
{
  uint16 len = 0U;
  uint8 i = 0U;

  for(i=0U; i<TcpIp_PBcfgPtr->dhcpv4Config.optionNum; i++)
  {
    CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOption =
      &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,i));
    if ((TcpIp_DhcpV4_Data[dhcpIdx].option_length[i] != 0U) && (dhcpOption->optionTransmit))
    {
      if(dhcpOption->optionCode != TCPIP_DHCPV4_OPTION_PARAM_LIST)
      {
        len += TCPIP_DHCPV4_OPTION_HDR_LEN + TcpIp_DhcpV4_Data[dhcpIdx].option_length[i];
      }
    }
  }
  return len;
}
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */


STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getParameterOptionListLength
(
  uint8 dhcpIdx
)
{
  uint16 len = 0U;
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  uint8 ParamReqListIndex = TcpIp_PBcfgPtr->dhcpv4Config.ParamReqListOptionIdx;
#else
  TS_PARAM_UNUSED(dhcpIdx);
#endif

  len += TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN;

#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  if(ParamReqListIndex < TcpIp_PBcfgPtr->dhcpv4Config.optionNum)
  {
    len += TcpIp_DhcpV4_Data[dhcpIdx].option_length[ParamReqListIndex];
  }
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */
  return len;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_parseOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionsPtr,
  uint16 totalOptionsLen,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint16 currentOptionIdx = 0U;

#if ((TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_OFF) && (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(dhcpIdx);
#endif /* ((TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_OFF) && (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_OFF)) */

  /* Iterate all options */
  while(currentOptionIdx < totalOptionsLen)
  {
    uint16 const leftOptionsLen = totalOptionsLen - currentOptionIdx;
    CONSTP2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) currentOptionPtr = &optionsPtr[currentOptionIdx];
    uint16 const currentOptionLen = TcpIp_DhcpV4_getOptionLen(currentOptionPtr, leftOptionsLen);

    if((currentOptionLen != 0U) && (currentOptionLen <= leftOptionsLen))
    {
      uint8 optionId = TCPIP_DHCPV4_GET_OPTION_ID(currentOptionPtr);

#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
      if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx) &&
         (optionId != TCPIP_DHCPV4_OPTION_SUBNET_MASK) &&
         (optionId != TCPIP_DHCPV4_OPTION_MSG_TYPE) &&
         (optionId != TCPIP_DHCPV4_OPTION_END)
        )
      {
        /* for simple dhcp netmask, msg type and end option are allowed,
           if any other option is set in the message discard it */
        break;
      }
      else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
      {
        switch(optionId)
        {
          case TCPIP_DHCPV4_OPTION_SUBNET_MASK:
            TCPIP_DHCPV4_GET_SUBNETMASK(opts->netmask, currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_ROUTER:
            TCPIP_DHCPV4_GET_ROUTER(opts->router, currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_LEASE_TIME:
            opts->lease_time = TCPIP_DHCPV4_GET_LEASE_TIME(currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_OVERLOAD:
            opts->overload = TCPIP_DHCPV4_GET_OVERLOAD(currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_MSG_TYPE:
            opts->msg_type = TCPIP_DHCPV4_GET_MSG_TYPE(currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_SERVER_ID:
            TCPIP_DHCPV4_GET_SERVER_ID(opts->server_id, currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_T1_TIME:
            opts->T1_time = TCPIP_DHCPV4_GET_T1_TIME(currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_T2_TIME:
            opts->T2_time = TCPIP_DHCPV4_GET_T2_TIME(currentOptionPtr);
            break;
          case TCPIP_DHCPV4_OPTION_END:
            retVal = E_OK;
            break;
          default:
            /* skip and do nothing */
            break;
        }
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
        {
          CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionConfigPtr =
              &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,0));

          TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

          if(currentOptionLen >= TCPIP_DHCPV4_OPTION_HDR_LEN)
          {
            (void)TcpIp_Dhcp_writeOptionRxIndication
              (
                optionId,
                (currentOptionLen - TCPIP_DHCPV4_OPTION_HDR_LEN),
                &optionsPtr[currentOptionIdx + TCPIP_DHCPV4_OPTION_HDR_LEN],
                TcpIp_DhcpV4_Data[dhcpIdx].option_data,
                TcpIp_DhcpV4_Data[dhcpIdx].option_length,
                dhcpOptionConfigPtr,
                TcpIp_PBcfgPtr->dhcpv4Config.optionNum
              );
          }
        }
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */
      }

      /* resume with next option */
      currentOptionIdx += currentOptionLen;
    }
    else
    {
      /* invalid option length */
      break;
    }
  }

  return retVal;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getOptionLen
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 leftLen
)
{
  uint16 optionLen = 0U;
  uint8 const optionCode = TCPIP_DHCPV4_GET_OPTION_ID(optPtr);

  if(optionCode == TCPIP_DHCPV4_OPTION_PAD)
  {
    optionLen = TCPIP_DHCPV4_OPTION_PAD_LEN;
  }
  else if(optionCode == TCPIP_DHCPV4_OPTION_END)
  {
    /* end of option reached, stop processing */
    optionLen = leftLen;
  }
  else
  {
    /* Remaining options have [Code | Len | Value(s)] structure => at least 3 bytes */
    if(leftLen > TCPIP_DHCPV4_OPTION_HDR_LEN)
    {
      uint8 const optionLenFieldVal = TCPIP_DHCPV4_GET_OPTION_SIZE(optPtr);

      switch(optionCode)
      {
        case TCPIP_DHCPV4_OPTION_SUBNET_MASK: /* intended fall through */
        case TCPIP_DHCPV4_OPTION_LEASE_TIME:  /* intended fall through */
        case TCPIP_DHCPV4_OPTION_SERVER_ID:   /* intended fall through */
        case TCPIP_DHCPV4_OPTION_T1_TIME:     /* intended fall through */
        case TCPIP_DHCPV4_OPTION_T2_TIME:
          /* These options must have a length of 4 bytes */
          if(optionLenFieldVal == TCPIP_DHCPV4_OPTION_LEN_4_BYTES)
          {
            optionLen = TCPIP_DHCPV4_OPTION_HDR_LEN + ((uint16) optionLenFieldVal);
          }
          break;
        case TCPIP_DHCPV4_OPTION_MSG_TYPE:    /* intended fall through */
        case TCPIP_DHCPV4_OPTION_OVERLOAD:
          /* These options must have a length of 1 byte */
          if(optionLenFieldVal == TCPIP_DHCPV4_OPTION_LEN_1_BYTE)
          {
            optionLen = TCPIP_DHCPV4_OPTION_HDR_LEN + ((uint16) optionLenFieldVal);
          }
          break;
        case TCPIP_DHCPV4_OPTION_ROUTER:      /* intended fall through */
        default:
          /* Unknown or router options have a variable length */
          optionLen = TCPIP_DHCPV4_OPTION_HDR_LEN + ((uint16) optionLenFieldVal);

          /* Check length of router option */
          if(optionCode == TCPIP_DHCPV4_OPTION_ROUTER)
          {
            if(TcpIp_DhcpV4_EvaluateRouterOptionLength(optionLenFieldVal) == E_NOT_OK)
            {
              /* Router option is not at least 4 bytes long and a multiple of 4 bytes */
              optionLen = 0U;
            }
          }
          break;
      }
    }
  }

  return optionLen;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_readOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  Std_ReturnType result = E_NOT_OK;

  /* set options to default parameters */
  opts->msg_type = TCPIP_DHCPV4_INVALID_MSG_TYPE;
  opts->router = 0U;
  opts->netmask = 0U;
  opts->server_id = 0U;
  opts->lease_time = 0xFFFFFFFFU;
  opts->overload = 0U;
  opts->T1_time = 0U;
  opts->T2_time = 0U;

  /* check magic cookie */
  if(TCPIP_DHCPV4_GET_MAGIC_COOKIE(dataPtr) == TCPIP_DHCPV4_MAGIC_COOKIE)
  {
    const uint16 offset = (TCPIP_DHCPV4_HEADER_LEN + TCPIP_DHCPV4_MAGIC_COOKIE_SIZE);
    const uint16 len = dataLen - offset;

    /* parse options in 'option' field */
    result = TcpIp_DhcpV4_parseOpts(dhcpIdx, &dataPtr[offset], len, opts);

    if(result == E_OK)
    {
      /* check if message type option is present */
      if(opts->msg_type != TCPIP_DHCPV4_INVALID_MSG_TYPE)
      {
        /* check if "file" field is used to hold options */
        if(TCPIP_DHCPV4_FLAG_FILE_OVERIDE_IS_SET(opts->overload))
        {
          result =
            TcpIp_DhcpV4_parseOpts
              (dhcpIdx, &dataPtr[TCPIP_DHCPV4_FILE_OFFSET], TCPIP_DHCPV4_FILE_LEN, opts);
        }

        if(result == E_OK)
        {
          /* check if "sname" field is used to hold options */
          if(TCPIP_DHCPV4_FLAG_SNAME_OVERIDE_IS_SET(opts->overload))
          {
            result =
              TcpIp_DhcpV4_parseOpts
                (dhcpIdx, &dataPtr[TCPIP_DHCPV4_SNAME_OFFSET], TCPIP_DHCPV4_SNAME_LEN, opts);
          }
        }
      }
    }
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_transmit
(
  uint8 dhcpIdx,
  TcpIp_SockAddrInetType remoteAddr,
  uint8 msg_type
)
{
  TcpIp_ReturnType retVal = TCPIP_E_DROP;
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr =
    &TcpIp_DhcpV4_Data[dhcpIdx];

  if(TcpIp_DhcpV4_DataPtr->socketId != TCPIP_SOCKETID_INVALID)
  {
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_VAR) remoteAddrPtr =
      /* Deviation MISRAC2012-2 */
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_VAR)) &remoteAddr;

    const uint16 msglength = TcpIp_DhcpV4_getMsgLength(dhcpIdx, msg_type);
    uint8 useUnspecifiedSrc;

    if(TcpIp_DhcpV4_DataPtr->ip_addr == 0U)
    {
      /* if ip address is not assigned, use unspecified as source address */
      useUnspecifiedSrc = TRUE;
    }
    else
    {
      /* if ip address is assigned, use ip address from dhcp server */
      useUnspecifiedSrc = FALSE;
    }

    /* set socket parameter for unspecified address */
    (void)
      TcpIp_ChangeParameter
        (TcpIp_DhcpV4_DataPtr->socketId, TCPIP_PARAMID_UDP_UNSPECIFIED_IP, &useUnspecifiedSrc);

    /* trigger transmission of DHCP message */
    retVal = TcpIp_UdpTransmit(TcpIp_DhcpV4_DataPtr->socketId, NULL_PTR, remoteAddrPtr, msglength);

    if(retVal == TCPIP_OK)
    {
      /* stop further retransmission in next mainfunction */
      TcpIp_DhcpV4_DataPtr->num_retry = 0U;
      TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);
    }
    else
    {
      /* no ARP entry or enough buffer retry in next mainfunction */
      TcpIp_DhcpV4_DataPtr->flags |= TCPIP_DHCPV4_FLAG_NXT_MAIN;
      retVal = TCPIP_E_MEMORY;
    }
  }

  return retVal;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_buildHeader
(
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) buffPtr,
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  uint16 optlen;
  uint16 secs;
  uint16 flags = TCPIP_DHCPV4_BROADCAST_FLAG;
  const TcpIp_LocalAddrIdType localAddrId =
    TcpIp_IpAddrM_getLocalAddrId(dhcpIdx , TCPIP_IPADDRM_ASSIGN_DHCPV4);
  uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr = &buffPtr[TCPIP_DHCPV4_HEADER_LEN];
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  secs = (TcpIp_DhcpV4_DataPtr->secs == TCPIP_DHCPV4_SECS_COUNTER_STOP) ?
         (uint16) 0U : TcpIp_DhcpV4_DataPtr->secs;

  if(!(TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_IS_SET(TcpIp_DhcpV4_DataPtr->flags)))
  {
    /* flags should be 0 if DECLINE message is transmitted */
    flags = 0U;
  }

  /* construct DHCP header */
  TCPIP_DHCPV4_SET_OP(buffPtr,TCPIP_DHCPV4_BOOTREQUEST);
  TCPIP_DHCPV4_SET_HTYPE(buffPtr, TCPIP_DHCPV4_HWTYPE_ETHERNET);
  TCPIP_DHCPV4_SET_HLEN(buffPtr, TCPIP_DHCPV4_HW_ADDR_LENGTH);
  TCPIP_DHCPV4_SET_HOPS(buffPtr, 0U);
  TCPIP_DHCPV4_SET_XID(buffPtr, TcpIp_DhcpV4_DataPtr->xid);
  TCPIP_DHCPV4_SET_SECS(buffPtr, secs);
  TCPIP_DHCPV4_SET_FLAGS(buffPtr, flags);
  TCPIP_DHCPV4_SET_CIADDR(buffPtr, TcpIp_DhcpV4_DataPtr->ip_addr);
  TCPIP_DHCPV4_SET_UNUSED_ADDRESSES(buffPtr);
  EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_DHCPV4_GET_PTR_CHADDR(buffPtr));
  TCPIP_DHCPV4_SET_UNUSED(buffPtr);

  /* construct Options, these options are equal for all message types */
  TCPIP_DHCPV4_SET_MAGIC_COOKIE(opthdr);
  opthdr = &opthdr[TCPIP_DHCPV4_MAGIC_COOKIE_SIZE];
  TCPIP_DHCPV4_SET_OPTION_MSG_TYPE(opthdr, msg_type & 0x0fU);
  opthdr = &opthdr[TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_MSG_TYPE_LEN];

  /* set options which are message specific */
  optlen = TcpIp_DhcpV4_setMsgSpecificOptions(dhcpIdx, msg_type, opthdr);

  /* mark end of option */
  opthdr[optlen] = TCPIP_DHCPV4_OPTION_END;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_getMsgLength
(
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  uint16 length = TCPIP_DHCPV4_HEADER_LEN;

#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(dhcpIdx);
#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_OFF) */

  switch(msg_type)
  {
    case TCPIP_DHCPV4_DISCOVER:
      length += TCPIP_DHCPV4_DISCOVER_LENGTH;
      length += TcpIp_DhcpV4_getParameterOptionListLength(dhcpIdx);
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getOptionFQDNLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getTxOptionsLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON ) */
      break;
    case TCPIP_DHCPV4_SIMPLE_DISCOVER:
      length += TCPIP_DHCPV4_SIMPLE_DISCOVER_LENGTH;
      break;
    case TCPIP_DHCPV4_DECLINE: /* intended fall through */
      length += TCPIP_DHCPV4_REQUEST_FIXED_LENGTH +
                TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN +
                TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_SERVER_ID_LEN;
        break;
    case TCPIP_DHCPV4_INT_REQUEST_EXTEND:
      length += TCPIP_DHCPV4_REQUEST_FIXED_LENGTH;
      length += TcpIp_DhcpV4_getParameterOptionListLength(dhcpIdx);
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getOptionFQDNLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getTxOptionsLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON ) */
      break;
    case TCPIP_DHCPV4_INT_REQUEST_OFFER:
      length += TCPIP_DHCPV4_REQUEST_FIXED_LENGTH +
                TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN +
                TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_SERVER_ID_LEN;
      length += TcpIp_DhcpV4_getParameterOptionListLength(dhcpIdx);
#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getOptionFQDNLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
      length += TcpIp_DhcpV4_getTxOptionsLength(dhcpIdx);
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON ) */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return length;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_setMsgSpecificOptions
(
  uint8 dhcpIdx,
  uint8 msg_type,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
)
{
  uint16 length = 0U;
  const uint8 type = msg_type & 0x0FU;

  TCPIP_PRECONDITION_ASSERT((type == TCPIP_DHCPV4_DISCOVER) || (type == TCPIP_DHCPV4_REQUEST) || (type == TCPIP_DHCPV4_DECLINE),
                            DET_INTERNAL_API_ID);

  if((msg_type == TCPIP_DHCPV4_DISCOVER) || (type == TCPIP_DHCPV4_REQUEST) || (msg_type == TCPIP_DHCPV4_DECLINE))
  {
    if((msg_type == TCPIP_DHCPV4_INT_REQUEST_OFFER) || (msg_type == TCPIP_DHCPV4_DECLINE))
    {
      /* append Requested IP address option */
      TCPIP_DHCPV4_SET_OPTION_REQUESTED_IP(opthdr, TcpIp_DhcpV4_Data[dhcpIdx].ip_req);
      length += TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_REQ_IP_ADDR_LEN;

      /* append Server Identifier option */
      TCPIP_DHCPV4_SET_OPTION_SERVER_ID(&opthdr[length], TcpIp_DhcpV4_Data[dhcpIdx].ip_server);
      length += TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_SERVER_ID_LEN;
    }

    if(msg_type != TCPIP_DHCPV4_DECLINE)
    {
      /* append parameter list option */
      TcpIp_DhcpV4_SetOptionRequestList(dhcpIdx, &opthdr[length]);
      length += TcpIp_DhcpV4_getParameterOptionListLength(dhcpIdx);

#if (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON)
      {
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.mainFunction,1 */
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

        /* append Full Qualified Domain Name option */
        TcpIp_DhcpV4_setOptionFQDN(dhcpIdx, &opthdr[length]);
        length += TcpIp_DhcpV4_getOptionFQDNLength(dhcpIdx);
              /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.mainFunction,1 */
        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
      }
#endif /* (TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
      {
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.mainFunction,1 */
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

        TcpIp_DhcpV4_setOption(dhcpIdx, &opthdr[length]);
        length += TcpIp_DhcpV4_getTxOptionsLength(dhcpIdx);

        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV4.EA2.mainFunction,1 */
        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
      }
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */
    }
  }

  return length;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_evaluateLeaseTime
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  Std_ReturnType result = E_NOT_OK;

  if(opt->lease_time != 0xFFFFFFFFU)
  {
    if(opt->lease_time <= TCPIP_DHCPV4_MAX_VALID_LEASE_TIME)
    {

      /* lease is finite */
      uint32 lease = opt->lease_time + TcpIp_DhcpV4_Data[dhcpIdx].secs;
      uint32 T1_time;
      uint32 T2_time;

      TCPIP_PRECONDITION_ASSERT(opt->lease_time <= (0xFFFFFFFFU - TcpIp_DhcpV4_Data[dhcpIdx].secs), TCPIP_INTERNAL_API_ID);

      /* derive T1 and T2 timeouts from the lease time */
      T1_time = (lease >> 1U);
      T2_time = ((lease * 7U) >> 3U);

      /* when T1- or T2-option was received, use those values instead */
      if(opt->T1_time != 0U)
      {
        T1_time = opt->T1_time;
      }
      if(opt->T2_time != 0U)
      {
        T2_time = opt->T2_time;
      }

      /* check for valid timeout values */
      if((lease > T2_time) && (T2_time > T1_time))
      {
        opt->lease_time = lease;
        opt->T1_time = T1_time;
        opt->T2_time = T2_time;
        result = E_OK;
      }
    }
  }
  else
  {
    /* infinite lease */
    opt->lease_time = 0U;
    opt->T1_time = 0U;
    opt->T2_time = 0U;
    result = E_OK;
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_received
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  uint8 state;

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_SELECTING:
    {
      if(opt->msg_type == TCPIP_DHCPV4_OFFER)
      {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
        if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx))
        {
          if(opt->netmask != 0U)
          {
            TcpIp_DhcpV4_Simple_tr_received_SELECTING_BOUND(localAddrId, dhcpIdx, yiaddr, opt);
          }
        }
        else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
        {
          TcpIp_DhcpV4_tr_received_SELECTING_REQUESTING(dhcpIdx, yiaddr, opt);
        }
      }
      break;
    }
    case TCPIP_DHCPV4_STATE_REQUESTING:
    {
      if(opt->msg_type == TCPIP_DHCPV4_ACK)
      {
        if(TcpIp_DhcpV4_evaluateLeaseTime(dhcpIdx, opt) == E_OK)
        {
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
          if
          (
            (opt->T1_time > (TCPIP_DHCPV4_PROBE_WAIT + (TCPIP_DHCPV4_PROBE_NUM - 1U) * TCPIP_DHCPV4_PROBE_MAX + TCPIP_DHCPV4_ANNOUNCE_WAIT)) ||
            (opt->T1_time == 0U) /* infinite lease time */
          )
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
          {
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF)
          TcpIp_DhcpV4_tr_received_REQUESTING_BOUND(localAddrId, dhcpIdx, yiaddr, opt);
#else /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */
          TcpIp_DhcpV4_tr_received_REQUESTING_PROBING(localAddrId, dhcpIdx, yiaddr, opt);
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */
          }
        }
      }
      else if(opt->msg_type == TCPIP_DHCPV4_NAK)
      {
        TcpIp_DhcpV4_tr_received_REQUESTING_INIT(dhcpIdx);
      }
      else
      {
        /* ignore all other messages */
      }
      break;
    }
    case TCPIP_DHCPV4_STATE_RENEWING:
    case TCPIP_DHCPV4_STATE_REBINDING:
    {
      if(opt->msg_type == TCPIP_DHCPV4_ACK)
      {
        if(TcpIp_DhcpV4_evaluateLeaseTime(dhcpIdx, opt) == E_OK)
        {
          TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_BOUND(dhcpIdx, opt);
        }
      }
      else if(opt->msg_type == TCPIP_DHCPV4_NAK)
      {
        TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_INIT(localAddrId, dhcpIdx);
      }
      else
      {
        /* ignore all other messages */
      }
      break;
    }
    case TCPIP_DHCPV4_STATE_BOUND:
    {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
      if(opt->msg_type == TCPIP_DHCPV4_OFFER)
      {
        if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx))
        {
          TcpIp_DhcpV4_Simple_tr_received_BOUND_BOUND(localAddrId, dhcpIdx, yiaddr, opt);
        }
      }
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
      break;
    }
    case TCPIP_DHCPV4_STATE_INIT:    /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      /* discard the message */
      break;
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV4_STATE_INACTIVE: /* intended fall through */
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_retransTimeout
(
  uint8 dhcpIdx
)
{
  uint8 state;

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_SELECTING:
    {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
      if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx))
      {
        TcpIp_DhcpV4_Simple_tr_retransTimeout_SELECTING_SELECTING(dhcpIdx);
      }
      else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
      {
        TcpIp_DhcpV4_tr_retransTimeout_SELECTING_SELECTING(dhcpIdx);
      }
      break;
    }
    case TCPIP_DHCPV4_STATE_REQUESTING:
      TcpIp_DhcpV4_tr_retransTimeout_REQUESTING_REQUESTING(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_RENEWING:
      TcpIp_DhcpV4_tr_retransTimeout_RENEWING_RENEWING(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_REBINDING:
      TcpIp_DhcpV4_tr_retransTimeout_REBINDING_REBINDING(dhcpIdx);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:   /* intended fall through */
    case TCPIP_DHCPV4_STATE_BOUND:  /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_timeout
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  uint8 state;

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_INIT:
    {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
      if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx))
      {
        TcpIp_DhcpV4_Simple_tr_timeout_INIT_SELECTING(localAddrId, dhcpIdx);
      }
      else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
      {
        TcpIp_DhcpV4_tr_timeout_INIT_SELECTING(dhcpIdx);
      }
      break;
    }
    case TCPIP_DHCPV4_STATE_REQUESTING:
      TcpIp_DhcpV4_tr_timeout_REQUESTING_INIT(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_BOUND:
      TcpIp_DhcpV4_tr_timeout_BOUND_RENEWING(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_RENEWING:
      TcpIp_DhcpV4_tr_timeout_RENEWING_REBINDING(dhcpIdx);
      break;
    case TCPIP_DHCPV4_STATE_REBINDING:
      TcpIp_DhcpV4_tr_timeout_REBINDING_INIT(localAddrId, dhcpIdx);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE:    /* intended fall through */
    case TCPIP_DHCPV4_STATE_SELECTING:   /* intended fall through */
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_conflict
(
  TcpIp_LocalAddrIdType localAddr
)
{
  TCPIP_PBCFG_PRECONDITION_ASSERT(localAddr, localAddrCfg, TCPIP_INTERNAL_API_ID);
  {
    uint8 state;
    const uint8 dhcpIdx =
      TcpIp_IpAddrM_getAssignmentIdx(localAddr, TCPIP_IPADDRM_ASSIGN_DHCPV4);

    TCPIP_PRECONDITION_ASSERT( dhcpIdx <  TCPIP_NUM_DHCPV4_CLIENTS, TCPIP_INTERNAL_API_ID);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(state, TcpIp_DhcpV4_Data[dhcpIdx].state);

    switch(state)
    {
      case TCPIP_DHCPV4_STATE_RENEWING:
      case TCPIP_DHCPV4_STATE_REBINDING:
      case TCPIP_DHCPV4_STATE_BOUND:
        /* conflict detected, abandon address and retrieve a new address from dhcp server */
        TcpIp_DhcpV4_tr_conflict_ASSIGNED_INIT(localAddr, dhcpIdx);
        break;
      /* CHECK: NOPARSE */
      case TCPIP_DHCPV4_STATE_REQUESTING:
      case TCPIP_DHCPV4_STATE_SELECTING:
      case TCPIP_DHCPV4_STATE_INIT:
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
      case TCPIP_DHCPV4_STATE_PROBING:     /* intended fall through */
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
      case TCPIP_DHCPV4_STATE_INACTIVE:  /* intended fall through */
      case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_startAssign_INACTIVE_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_START_DELAY(localAddr, dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  if(TcpIp_DhcpV4_entry_ACTIVE(localAddrId, dhcpIdx, TCPIP_DHCPV4_CLIENT_PORT) == TCPIP_OK)
  {
    TcpIp_DhcpV4_entry_INIT(dhcpIdx);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_START_DELAY_INACTIVE
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_BOUND_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* exit code */
  TcpIp_DhcpV4_exit_ASSIGNED(localAddrId);
  TcpIp_DhcpV4_exit_ACTIVE(dhcpIdx);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE
(
  uint8 dhcpIdx
)
{
  /* exit code */
  TcpIp_DhcpV4_exit_ACTIVE(dhcpIdx);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_INIT_SELECTING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    const TcpIp_SockAddrInetType remoteAddr =
      {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};

    /* try to discover DHCP servers until some offers an IP */
    TcpIp_DhcpV4_setStateTimer(dhcpIdx, 0U);

    /* evaluate transaction id */
    TcpIp_DhcpV4_Data[dhcpIdx].xid = TcpIp_GetRandomValue();

    /* first retransmission */
    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, TCPIP_DHCPV4_INITIAL_RETRANS_TIMEOUT);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(TcpIp_DhcpV4_Data[dhcpIdx].state, TCPIP_DHCPV4_STATE_SELECTING);

    (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_DISCOVER);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_BOUND_RENEWING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_SockAddrInetType remoteAddr = {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {0U}};
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                            TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
    const uint32 renewalTimeout = TcpIp_DhcpV4_DataPtr->T2_time - TcpIp_DhcpV4_DataPtr->T1_time;
    uint32 timeout = TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT;

    /* send message to the dhcp server */
    remoteAddr.addr[0U] = TcpIp_DhcpV4_DataPtr->ip_server;

    /* acquisition process restarts */
    TcpIp_DhcpV4_DataPtr->secs = 0U;

    /* start timer for T2-timeout */
    TcpIp_DhcpV4_setStateTimer(dhcpIdx, renewalTimeout);

    /* first retransmission */
    if((renewalTimeout >> 1U) >= TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT)
    {
      timeout = (renewalTimeout >> 1U);
    }

    /* first retransmission */
    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, timeout);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_RENEWING);

    (void)
      TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_INT_REQUEST_EXTEND);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_timeout_RENEWING_REBINDING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    const TcpIp_SockAddrInetType remoteAddr =
      {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                            TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
    const uint32 leaseTimeout = TcpIp_DhcpV4_DataPtr->lease - TcpIp_DhcpV4_DataPtr->T2_time;
    uint32 timeout = TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT;

    /* start timer for detecting lease expiration */
    TcpIp_DhcpV4_setStateTimer(dhcpIdx, leaseTimeout);

    if((leaseTimeout >> 1U) >= TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT)
    {
      timeout = (leaseTimeout >> 1U);
    }

    /* first retransmission */
    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, timeout);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_REBINDING);

    (void)
      TcpIp_DhcpV4_transmit
        (dhcpIdx, remoteAddr, TCPIP_DHCPV4_INT_REQUEST_EXTEND);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_SELECTING_REQUESTING
(
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* exit code */

  /* transitional code */
  {
    const TcpIp_SockAddrInetType remoteAddr =
      {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                            TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

    TcpIp_DhcpV4_DataPtr->ip_server = opt->server_id;
    TcpIp_DhcpV4_DataPtr->ip_req = yiaddr;

    /* acquisition process starts */
    TcpIp_DhcpV4_DataPtr->secs = 0U;

    /* set timer to prevent waiting infinitely */
    TcpIp_DhcpV4_setStateTimer(dhcpIdx, TCPIP_DHCPV4_SELECTING_TIMEOUT);

    /* first retransmission */
    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, TCPIP_DHCPV4_INITIAL_RETRANS_TIMEOUT);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_REQUESTING);

    (void)
      TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_INT_REQUEST_OFFER);
  }

  /* entry code */
}

#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* transitional code */
  {
    /* save ip, netmask and gateway */
    TcpIp_DhcpV4_DataPtr->gateway_addr = opt->router;
    TcpIp_DhcpV4_DataPtr->netmask = opt->netmask;
    TcpIp_DhcpV4_DataPtr->ip_addr = yiaddr;
  }

  /* entry code */
  TcpIp_DhcpV4_pre_entry_BOUND_time_update(dhcpIdx, opt);
  TcpIp_DhcpV4_entry_BOUND(dhcpIdx);
  TcpIp_DhcpV4_entry_ASSIGNED(localAddrId);
}

#else /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  const uint16 probe_delay = TcpIp_DhcpV4_startProbeTimer(TcpIp_DhcpV4_DataPtr);
  /* exit code */

  /* transitional code */
  /* save ip, netmask and gateway */
  TcpIp_DhcpV4_DataPtr->gateway_addr = opt->router;
  TcpIp_DhcpV4_DataPtr->netmask = opt->netmask;
  TcpIp_DhcpV4_DataPtr->ip_addr = yiaddr;

  /* entry code */
  TcpIp_DhcpV4_pre_entry_BOUND_time_update(dhcpIdx, opt);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_PROBING);

  /* Start ARP Probing procedure */
  TcpIp_IpV4_ArpProbe
    (
        &TcpIp_DhcpV4_probeResult,
        TcpIp_DhcpV4_DataPtr->ip_addr,
        TCPIP_DHCPV4_PROBE_NUM,
        TCPIP_DHCPV4_ANNOUNCE_WAIT,
        TCPIP_DHCPV4_PROBE_RANDOM,
        localAddrId,
        TCPIP_IPADDRM_ASSIGN_DHCPV4,
        probe_delay
    );
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_probeResult
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  uint8 state;
  const TcpIp_LocalAddrIdType localAddrId =
    (TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4.localAddrIdTx;

  const uint8 dhcpIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_DhcpV4_DataPtr->state);

  switch(state)
  {
    case TCPIP_DHCPV4_STATE_PROBING:
      /* Stop ARP Probing procedure */
      TcpIp_IpV4_ArpProbe(NULL_PTR, TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, 0U, localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4, 0U);
      if(unique)
      {
        TcpIp_DhcpV4_entry_BOUND(dhcpIdx);
        TcpIp_DhcpV4_entry_ASSIGNED(localAddrId);
      }
      else
      {
        const uint32 initDelayTimeout =
          (uint32)TcpIp_PBcfgPtr->dhcpv4Config.initDelay + TCPIP_DHCPV4_CONFLICT_DISCOVER_DELAY;
        const TcpIp_SockAddrInetType remoteAddr =
          {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};
#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
        const uint32 ipAddr = TcpIp_DhcpV4_DataPtr->ip_addr;
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */
        TcpIp_DhcpV4_DataPtr->ip_addr = 0U;
        /* Remove transmission of broadcast flag */
        TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED);
        (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_DECLINE);
        TcpIp_DhcpV4_DataPtr->flags |= TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED;
        if(TcpIp_DhcpV4_DataPtr->conflicts <= TCPIP_DHCPV4_MAX_CONFLICTS)
        {
          TcpIp_DhcpV4_DataPtr->conflicts++;
        }
#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
        TcpIp_IpV4_AddrConflDetect_conflictCallout
        (
          localAddrId,
          &ipAddr,
          localPhysAddr,
          remotePhysAddr
        );
#else
  TS_PARAM_UNUSED(localPhysAddr);
  TS_PARAM_UNUSED(remotePhysAddr);
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */
        TcpIp_DhcpV4_entry_INIT(dhcpIdx);
        /* Wait time before transmitting DISCOVER message (10s + configured delay) */
        TcpIp_DhcpV4_setStateTimer(dhcpIdx, initDelayTimeout);
      }
      break;
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV4_STATE_SELECTING: /* intended fall through */
    case TCPIP_DHCPV4_STATE_REQUESTING: /* intended fall through */
    case TCPIP_DHCPV4_STATE_RENEWING: /* intended fall through */
    case TCPIP_DHCPV4_STATE_REBINDING: /* intended fall through */
    case TCPIP_DHCPV4_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INACTIVE: /* intended fall through */
    case TCPIP_DHCPV4_STATE_INIT:   /* intended fall through */
    case TCPIP_DHCPV4_STATE_BOUND:  /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_startProbeTimer
(
  P2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr
)
{
  uint16 probe_delay_time;

  if(TcpIp_DhcpV4_DataPtr->conflicts < TCPIP_DHCPV4_MAX_CONFLICTS)
  {
    probe_delay_time =
      (uint16)TcpIp_GetRandomRange
        (
          TCPIP_DHCPV4_PROBE_WAIT*TCPIP_DHCPV4_100MS_TO_1S_FACTOR
        );
  }
  else
  {
    probe_delay_time =
      (TCPIP_DHCPV4_RATE_LIMIT_INTERVAL*TCPIP_DHCPV4_100MS_TO_1S_FACTOR) + 1U;
  }

  return probe_delay_time;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_stopAssign_PROBING_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* Stop ARP Probing procedure */
  TcpIp_IpV4_ArpProbe(NULL_PTR, TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, 0U, localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4, 0U);
  TcpIp_DhcpV4_tr_stopAssign_INIT_INACTIVE(dhcpIdx);
}

#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_REQUESTING_INIT
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_INIT(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_conflict_ASSIGNED_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  const TcpIp_SockAddrInetType remoteAddr =
    {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                      TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  const uint32 initDelayTimeout =
    (uint32)TcpIp_PBcfgPtr->dhcpv4Config.initDelay + TCPIP_DHCPV4_CONFLICT_DISCOVER_DELAY;
  /* exit code */
  TcpIp_DhcpV4_exit_ASSIGNED(localAddrId);

  /* transitional code */
  TcpIp_DhcpV4_DataPtr->ip_addr = 0U;
  /* Remove transmission of broadcast flag */
  TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED);
  TcpIp_DhcpV4_DataPtr->secs = TCPIP_DHCPV4_SECS_COUNTER_STOP;
  (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_DECLINE);
  TcpIp_DhcpV4_DataPtr->flags |= TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_ASSIGNED;

  /* entry code */
  TcpIp_DhcpV4_entry_INIT(dhcpIdx);
  /* Wait time before transmitting DISCOVER message (10s + configured delay) */
  TcpIp_DhcpV4_setStateTimer(dhcpIdx, initDelayTimeout);
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* exit code */
  TcpIp_DhcpV4_exit_ASSIGNED(localAddrId);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV4_entry_INIT(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_received_RENEWING_OR_REBINDING_BOUND
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* entry code */
  TcpIp_DhcpV4_pre_entry_BOUND_time_update(dhcpIdx, opt);
  TcpIp_DhcpV4_entry_BOUND(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_SELECTING_SELECTING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV4_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV4_DISCOVER);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_REQUESTING_REQUESTING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV4_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV4_INT_REQUEST_OFFER);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_increase
(
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  const TcpIp_SockAddrInetType remoteAddr =
    {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};

  /* check for retry in mainfunction */
  if(!TCPIP_DHCPV4_FLAG_NXT_MAIN_IS_SET(TcpIp_DhcpV4_Data[dhcpIdx].flags))
  {
    /* calculate new timeout value */
    uint32 timeout = (TcpIp_DhcpV4_Data[dhcpIdx].rtx_timeout << 1U);

    if(timeout > TCPIP_DHCPV4_MAX_RETRANSMISSION_TIMEOUT)
    {
      timeout = TCPIP_DHCPV4_MAX_RETRANSMISSION_TIMEOUT;
    }

    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, timeout);
  }

  /* retransmit DHCP message */
  (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, msg_type);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_RENEWING_RENEWING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    const uint32 ip = TcpIp_DhcpV4_Data[dhcpIdx].ip_server;

    TcpIp_DhcpV4_tr_retransTimeout_decrease(dhcpIdx, ip, TCPIP_DHCPV4_INT_REQUEST_EXTEND);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    const uint32 ip = TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE;

    TcpIp_DhcpV4_tr_retransTimeout_decrease(dhcpIdx, ip, TCPIP_DHCPV4_INT_REQUEST_EXTEND);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_tr_retransTimeout_decrease
(
  uint8 dhcpIdx,
  uint32 server_ip,
  uint8 msg_type
)
{
  TcpIp_SockAddrInetType remoteAddr = {TCPIP_AF_INET, TCPIP_DHCPV4_SERVER_PORT, {0U}};
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* check for retry in mainfunction */
  if(!TCPIP_DHCPV4_FLAG_NXT_MAIN_IS_SET(TcpIp_DhcpV4_DataPtr->flags))
  {
    /* calculate new timeout value */
    uint32 timeout = TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT;

    if((TcpIp_DhcpV4_DataPtr->rtx_timeout >> 1U) >= TCPIP_DHCPV4_MIN_RETRANSMISSION_TIMEOUT)
    {
      timeout = (TcpIp_DhcpV4_DataPtr->rtx_timeout >> 1U);
    }

    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, timeout);
  }

  remoteAddr.addr[0U] = server_ip;

  /* retransmit DHCP message */
  (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, msg_type);
}

#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  if(TcpIp_DhcpV4_entry_ACTIVE(localAddrId, dhcpIdx, TCPIP_DHCPV4_SIMPLE_CLIENT_PORT) == TCPIP_OK)
  {
    TcpIp_DhcpV4_entry_INIT(dhcpIdx);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_timeout_INIT_SELECTING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
    const TcpIp_SockAddrInetType remoteAddr =
      {TCPIP_AF_INET, TCPIP_DHCPV4_SIMPLE_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};
    uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

    /* try to discover DHCP servers until some offers an IP */
    TcpIp_DhcpV4_setStateTimer(dhcpIdx, 0U);

    /* transaction id is set to the lower 4 bytes of the ethernet address */
    TcpIp_DhcpV4_Data[dhcpIdx].xid = TCPIP_GET32(ethAddr, 2);

    /* first retransmission */
    TcpIp_DhcpV4_setRetransTimer(dhcpIdx, TCPIP_DHCPV4_SIMPLE_RETRANS_TIMEOUT);

    /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
    TS_AtomicAssign8(TcpIp_DhcpV4_Data[dhcpIdx].state, TCPIP_DHCPV4_STATE_SELECTING);

    (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_SIMPLE_DISCOVER);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_retransTimeout_SELECTING_SELECTING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    const TcpIp_SockAddrInetType remoteAddr =
      {TCPIP_AF_INET, TCPIP_DHCPV4_SIMPLE_SERVER_PORT, {TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE}};

    /* check for retry in mainfunction */
    if(!TCPIP_DHCPV4_FLAG_NXT_MAIN_IS_SET(TcpIp_DhcpV4_Data[dhcpIdx].flags))
    {
      TcpIp_DhcpV4_setRetransTimer(dhcpIdx, TCPIP_DHCPV4_SIMPLE_RETRANS_TIMEOUT);
    }

    /* retransmit DHCP message */
    (void) TcpIp_DhcpV4_transmit(dhcpIdx, remoteAddr, TCPIP_DHCPV4_SIMPLE_DISCOVER);
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_received_SELECTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV4_DataPtr->ip_addr = yiaddr;
    TcpIp_DhcpV4_DataPtr->ip_req = yiaddr;
    TcpIp_DhcpV4_DataPtr->gateway_addr = 0U;
    TcpIp_DhcpV4_DataPtr->netmask = opt->netmask;

    /* infinite lease */
    opt->lease_time = 0U;
    opt->T1_time = 0U;
    opt->T2_time = 0U;
  }

  /* entry code */
  TcpIp_DhcpV4_pre_entry_BOUND_time_update(dhcpIdx, opt);
  TcpIp_DhcpV4_entry_BOUND(dhcpIdx);
  TcpIp_DhcpV4_entry_ASSIGNED(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Simple_tr_received_BOUND_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint32 yiaddr,
  P2VAR(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  /* exit code */

  /* transitional code */
  {
    /* inform Local address manager that IP shall not be used anymore */
    TcpIp_IpAddrM_AssignSM_unassigned(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);

    TcpIp_DhcpV4_DataPtr->ip_addr = yiaddr;
    TcpIp_DhcpV4_DataPtr->gateway_addr = 0U;
    TcpIp_DhcpV4_DataPtr->netmask = opt->netmask;

    /* infinite lease */
    opt->lease_time = 0U;
    opt->T1_time = 0U;
    opt->T2_time = 0U;

    /* inform IP address manager that IP address can be used */
    TcpIp_IpAddrM_AssignSM_assigned(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
  }

  /* entry code */
}

#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_START_DELAY);

  if(TCPIP_DHCPV4_FLAG_BROADCAST_ADDR_IS_SET(TcpIp_DhcpV4_DataPtr->flags))
  {
#if (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON)
    if(TCPIP_DHCPV4_ISSIMPLECLIENT(dhcpIdx))
    {
      /* open DHCP socket, bind socket to 10068 and delay transmission of DISCOVER */
      TcpIp_DhcpV4_Simple_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(localAddr, dhcpIdx);
    }
    else
#endif /* (TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED == STD_ON) */
    {
      TcpIp_DhcpV4_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(localAddr, dhcpIdx);
    }
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_entry_ACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  uint16 port
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  TcpIp_SocketIdType socketId = (TcpIp_SocketIdType)0U;

  /* Retrieve a socket */
  if(TcpIp_DhcpV4_getSocket(TCPIP_AF_INET, TCPIP_IPPROTO_UDP, &socketId) == E_OK)
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TcpIp_LocalAddrIdType limLocalAddrId;

    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    limLocalAddrId = TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV4.localAddrLimdBroad;

    /*  bind socket to limited broadcast address */
    if(TcpIp_Socket_bind(socketId, limLocalAddrId, &port, FALSE) == E_OK)
    {
      TcpIp_DhcpV4_Data[dhcpIdx].socketId = socketId;

      result = TCPIP_OK;
    }
    else
    {
      /* in case address is not available close the socket */
      (void) TcpIp_Close(socketId, FALSE);
    }
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_pre_entry_BOUND_time_update
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV4_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  /* stop retransmission timer */
  TcpIp_DhcpV4_setRetransTimer(dhcpIdx, 0U);
  TcpIp_DhcpV4_DataPtr->num_retry = 0U;
  TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);

  /* record lease and set timer values */
  TcpIp_DhcpV4_DataPtr->lease = opt->lease_time;
  TcpIp_DhcpV4_DataPtr->T1_time = opt->T1_time;
  TcpIp_DhcpV4_DataPtr->T2_time = opt->T2_time;

  /* acquisition process finished */
  TcpIp_DhcpV4_DataPtr->secs = TCPIP_DHCPV4_SECS_COUNTER_STOP;

  /* start timer for T1-timeout */
  TcpIp_DhcpV4_setStateTimer(dhcpIdx, TcpIp_DhcpV4_DataPtr->T1_time);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_BOUND
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_BOUND);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* inform IP address manager that IP address can be used */
  TcpIp_IpAddrM_AssignSM_assigned(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_exit_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* inform Local address manager that IP shall not be used anymore */
  TcpIp_IpAddrM_AssignSM_unassigned(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV4);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_exit_ACTIVE
(
  uint8 dhcpIdx
)
{
  if(TcpIp_DhcpV4_Data[dhcpIdx].socketId != TCPIP_SOCKETID_INVALID)
  {
    (void) TcpIp_Close(TcpIp_DhcpV4_Data[dhcpIdx].socketId, FALSE);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_INACTIVE
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  TcpIp_DhcpV4_setStateTimer(dhcpIdx, 0U);
  TcpIp_DhcpV4_setRetransTimer(dhcpIdx, 0U);
  TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);
  TcpIp_DhcpV4_DataPtr->num_retry = 0U;
  TcpIp_DhcpV4_DataPtr->socketId = TCPIP_SOCKETID_INVALID;
  TcpIp_DhcpV4_DataPtr->secs = TCPIP_DHCPV4_SECS_COUNTER_STOP;
  TcpIp_DhcpV4_DataPtr->ip_addr = 0U;
  TcpIp_DhcpV4_DataPtr->ip_req = 0U;
  TcpIp_DhcpV4_DataPtr->gateway_addr = 0U;
  TcpIp_DhcpV4_DataPtr->netmask = 0U;
#if (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON)
  TcpIp_DhcpV4_DataPtr->conflicts = 0U;
#endif /* (TCPIP_DHCPV4_CONFIG_DAD_ENABLED == STD_ON) */
  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_INACTIVE);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_entry_INIT
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
                          TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];
  TcpIp_DhcpV4_DataPtr->gateway_addr = 0U;
  TcpIp_DhcpV4_DataPtr->netmask = 0U;
  TcpIp_DhcpV4_DataPtr->ip_addr = 0U;
  TcpIp_DhcpV4_DataPtr->ip_server = 0U;
  TcpIp_DhcpV4_setRetransTimer(dhcpIdx, 0U);
  TcpIp_DhcpV4_DataPtr->ip_req = 0U;
  TcpIp_DhcpV4_DataPtr->flags &= ~((uint8) TCPIP_DHCPV4_FLAG_NXT_MAIN);
  TcpIp_DhcpV4_DataPtr->num_retry = 0U;
  TcpIp_DhcpV4_DataPtr->secs = TCPIP_DHCPV4_SECS_COUNTER_STOP;

  /* wait configured amount of time before transmitting DISCOVER */
  TcpIp_DhcpV4_setStateTimer(dhcpIdx, TcpIp_PBcfgPtr->dhcpv4Config.initDelay);

  /* !LINKSTO TcpIp.Design.DhcpV4.Atomic.State,1 */
  TS_AtomicAssign8(TcpIp_DhcpV4_DataPtr->state, TCPIP_DHCPV4_STATE_INIT);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_SetOptionRequestList
(
    uint8 dhcpIdx,
    P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
)
{
#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  uint8 ParamReqListIndex = TcpIp_PBcfgPtr->dhcpv4Config.ParamReqListOptionIdx;
  uint16 len=0U;
#else
  TS_PARAM_UNUSED(dhcpIdx);
#endif

  /* id of the option */
  TCPIP_DHCPV4_SET_OPTION_ID(optptr, TCPIP_DHCPV4_OPTION_PARAM_LIST);
  /* length of the option */
  TCPIP_DHCPV4_SET_OPTION_LEN(optptr, TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN);
  /* set default options */
  TCPIP_DHCPV4_SET_OPTION_PARAM_DEFAULT_LIST(optptr);

#if (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)
  {
    /* append any other option */
    CONSTP2VAR(TcpIp_DhcpV4_DataType, AUTOMATIC, TCPIP_APPL_DATA)
               TcpIp_DhcpV4_DataPtr = &TcpIp_DhcpV4_Data[dhcpIdx];

    if(ParamReqListIndex < TcpIp_PBcfgPtr->dhcpv4Config.optionNum)
    {
      CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOption =
        &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpOption,ParamReqListIndex));

      if(TcpIp_DhcpV4_DataPtr->option_length[ParamReqListIndex]!=0u)
      {
        len = (TcpIp_DhcpV4_DataPtr->option_length[ParamReqListIndex]);

        TS_MemCpy( &(optptr)[TCPIP_DHCPV4_OPTION_HDR_LEN + TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN],
                   &TcpIp_DhcpV4_DataPtr->option_data[dhcpOption->optionDataOffset],
                   (uint32)len
                 );

        /* modify option length */
        TCPIP_DHCPV4_SET_OPTION_LEN(optptr, TCPIP_DHCPV4_OPTION_PARAM_LIST_DEFAULT_LEN + len);
      }
    }
  }
#endif /* (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON) */
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_EvaluateRouterOptionLength
(
  uint8 addressLen
)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(addressLen >= TCPIP_IPV4_ADDR_BYTE_SIZE )
  {
    if((addressLen%TCPIP_IPV4_ADDR_BYTE_SIZE) == 0U)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif /* TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
