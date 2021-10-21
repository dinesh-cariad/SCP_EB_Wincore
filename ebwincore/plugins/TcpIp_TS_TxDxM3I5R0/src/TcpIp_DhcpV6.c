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
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1)The most efficient way to copy an array of 4 bytes to an uint32 while preserving the byte order
 * without any alignment constraints requires to cast the uint32* to an uint8*.
 * 2)AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 3)Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 *
 * MISRAC2012-3) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 * MISRAC2012-4) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 * Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
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
#include <EthIf.h>              /* EthIf API declarations */
#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */
#include <SchM_TcpIp.h>         /* SchM API declarations */
#include <TcpIp_Generic_Int.h>  /* generic upper layer API */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_IpV6_Int.h>     /* used SW-unit interface file */
#include <TcpIp_Udp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Ndp_Int.h>      /* used SW-unit interface file */

#include <TcpIp_DhcpV6_Int.h>   /* own SW-unit interface file */
#include <TcpIp_DhcpV6_Ext.h>   /* own SW-unit interface file */
#include <TcpIp_Dhcp_Priv.h>    /* own SW-unit private header file */

/*==================[macros]================================================*/

/** \brief TcpIp_DhcpV6ReadOption() service ID */
#define TCPIP_SID_DHCPV6READOPTION                            0x19U

/** \brief TcpIp_DhcpV6WriteOption() service ID */
#define TCPIP_SID_DHCPV6WRITEOPTION                           0x1aU

/** \brief Ethernet hardware type */
#define TCPIP_DHCPV6_HWTYPE_ETHERNET                             1U

/** \brief UDP port to which the client sends DHCP messages */
#define TCPIP_DHCPV6_SERVER_PORT                               547U
/** \brief port on which to receive DHCP messages */
#define TCPIP_DHCPV6_CLIENT_PORT                               546U

/** \brief Secs counter is not running */
#define TCPIP_DHCPV6_ELAPSED_TIMER_STOP                     0xFFFFU

/** \brief Infinite time */
#define TCPIP_DHCPV6_INFINITE_TIME                      0xFFFFFFFFU

/* DHCPv6 Transmission and Retransmission Parameters */
/** \brief Max delay of first solicit */
#define TCPIP_DHCPV6_SOL_MAX_DELAY                               1U
/** \brief Initial solicit timeout */
#define TCPIP_DHCPV6_SOL_TIMEOUT                                 1U
/** \brief Max solicit timeout value */
#define TCPIP_DHCPV6_SOL_MAX_RT                                120U
/** \brief Initial Request timeout */
#define TCPIP_DHCPV6_REQ_TIMEOUT                                 1U
/** \brief Max Request timeout value */
#define TCPIP_DHCPV6_REQ_MAX_RT                                 30U
/** \brief Max Request retry  attempts */
#define TCPIP_DHCPV6_REQ_MAX_RC                                 10U
/** \brief Initial Renew timeout */
#define TCPIP_DHCPV6_REN_TIMEOUT                                10U
/** \brief Max Renew timeout value */
#define TCPIP_DHCPV6_REN_MAX_RT                                600U
/** \brief Initial Rebind timeout */
#define TCPIP_DHCPV6_REB_TIMEOUT                                10U
/** \brief Max Rebind timeout value */
#define TCPIP_DHCPV6_REB_MAX_RT                                600U
/** \brief Initial Decline timeout */
#define TCPIP_DHCPV6_DEC_TIMEOUT                                 1U
/** \brief Max Decline retry attempts */
#define TCPIP_DHCPV6_DEC_MAX_RC                                  5U
/** \brief Max (Infinite) timeout value */
#define TCPIP_DHCPV6_INF_MAX_RT                                  0U


/** \brief DUID Type option */
#define TCPIP_DHCPV6_DUID_LL_TYPE                                3U
#define TCPIP_DHCPV6_DUID_HEADER_LEN                             4U
#define TCPIP_DHCPV6_DUID_LL_LEN                                10U
#define TCPIP_DHCPV6_MIN_SERVER_DUID_LEN                         2U

/** \brief client ID option header len */
#define TCPIP_DHCPV6_CLIENT_HEADER_LEN                           8U

/** \brief DhcpV6 header length in bytes */
#define TCPIP_DHCPV6_HEADER_LEN                                  4U
/** \brief Message Type length in bytes */
#define TCPIP_DHCPV6_MSG_TYPE_LEN                                1U
/** \brief transaction ID length in bytes */
#define TCPIP_DHCPV6_TRANSACTION_ID_LEN                          3U

/** \brief Option code length in bytes */
#define TCPIP_DHCPV6_OPTION_CODE_LEN                             2U
/** \brief Option-data length in bytes */
#define TCPIP_DHCPV6_OPTION_LEN_LEN                              2U

/** \brief Client identifier option length value */
#define TCPIP_DHCPV6_CLIENT_ID_OPTION_LEN                       10U

/** \brief IA_NA SOLICIT option length value */
#define TCPIP_DHCPV6_IA_NA_SOLICIT_OPTION_LEN                   12U
/** \brief IA_NA option length value */
#define TCPIP_DHCPV6_IA_NA_OPTION_LEN                           40U
/** \brief IAADDR option length value */
#define TCPIP_DHCPV6_IAADDR_OPTION_LEN                          24U

/** \brief DHCPv6 options header length in bytes */
#define TCPIP_DHCPV6_OPTION_HEADER_LEN                          4U

/** \brief OPTION_IA_NA IAID Length in bytes */
#define TCPIP_DHCPV6_IAID_LEN                                   4U
/** \brief T1 length in bytes */
#define TCPIP_DHCPV6_T1_LEN                                     4U
/** \brief T2 length in bytes */
#define TCPIP_DHCPV6_T2_LEN                                     4U

/** \brief proffered lifetime length in bytes */
#define TCPIP_DHCPV6_PREFFERED_LIFETIME_LEN                     4U
/** \brief valid lifetime length in bytes */
#define TCPIP_DHCPV6_VALID_LIFETIME_LEN                         4U

/** \brief OPTION ELAPSED TIME option length in bytes */
#define TCPIP_DHCPV6_ELAPSED_TIME_OPTION_LENGTH                 2U

/** \brief OPTION_IA_NA header length in bytes */
#define TCPIP_DHCPV6_IA_NA_LEN                                 12U

/** \brief Elapsed Time Option length in bytes */
#define TCPIP_DHCPV6_ELAPSED_TIME_LENGTH                         \
  TCPIP_DHCPV6_OPTION_HEADER_LEN +                               \
  TCPIP_DHCPV6_ELAPSED_TIME_OPTION_LENGTH

/** \brief Elapsed Time Option length value */
#define TCPIP_DHCPV6_ELAPSED_TIME_OPTION_LEN                    2U

/** \brief empty option REQUEST OPTION length in bytes */
#define TCPIP_DHCPV6_OPTION_ORO_TIME_LENGTH                      \
  TCPIP_DHCPV6_OPTION_HEADER_LEN

/** \brief fixed option length of SOLICIT message */
#define TCPIP_DHCPV6_SOLICIT_FIXED_LENGTH                          \
 TCPIP_DHCPV6_CLIENT_HEADER_LEN +                                  \
 (TCPIP_DHCPV6_CLIENT_ID_OPTION_LEN) +                             \
 (TCPIP_DHCPV6_ELAPSED_TIME_LENGTH) +                              \
  TCPIP_DHCPV6_OPTION_HEADER_LEN + TCPIP_DHCPV6_IA_NA_LEN +        \
  TCPIP_DHCPV6_OPTION_ORO_TIME_LENGTH

/** \brief fixed option length of REQUEST message */
#define TCPIP_DHCPV6_REQUEST_FIXED_LENGTH                          \
  TCPIP_DHCPV6_REBIND_FIXED_LENGTH
/** \brief fixed option length of RENEW message */
#define TCPIP_DHCPV6_RENEW_FIXED_LENGTH                            \
  TCPIP_DHCPV6_REBIND_FIXED_LENGTH
/** \brief fixed option length of REBIND message */
#define TCPIP_DHCPV6_REBIND_FIXED_LENGTH                           \
  (TCPIP_DHCPV6_SOLICIT_FIXED_LENGTH) + TCPIP_DHCPV6_HEADER_LEN +  \
  TCPIP_DHCPV6_IAADDR_OPTION_LEN

/** \brief fixed option length of DECLINE message */
#define TCPIP_DHCPV6_DECLINE_FIXED_LENGTH                          \
  TCPIP_DHCPV6_REBIND_FIXED_LENGTH

/* DHCPv6 message types */
/** \brief Invalid message type */
#define TCPIP_DHCPV6_INVALID_MSG_TYPE                           0U
/** \brief Solicit message type */
#define TCPIP_DHCPV6_SOLICIT                                    1U
/** \brief Advertise message type */
#define TCPIP_DHCPV6_ADVERTISE                                  2U
/** \brief Request message type */
#define TCPIP_DHCPV6_REQUEST                                    3U
/** \brief Confirm message type */
#define TCPIP_DHCPV6_CONFIRM                                    4U
/** \brief Renew message type */
#define TCPIP_DHCPV6_RENEW                                      5U
/** \brief Rebind message type */
#define TCPIP_DHCPV6_REBIND                                     6U
/** \brief Reply message type */
#define TCPIP_DHCPV6_REPLY                                      7U
/** \brief Release message type */
#define TCPIP_DHCPV6_RELEASE                                    8U
/** \brief Decline message type */
#define TCPIP_DHCPV6_DECLINE                                    9U
/** \brief Reconfigure message type */
#define TCPIP_DHCPV6_RECONFIGURE                               10U
/** \brief Information-request message type */
#define TCPIP_DHCPV6_INFORMATION_REQUEST                       11U
/** \brief Relay-forw message type */
#define TCPIP_DHCPV6_RELAY_FORW                                12U
/** \brief Relay-repl message type */
#define TCPIP_DHCPV6_RELAY_REPL                                13U

/** \brief DHCP port closed */
#define TCPIP_DHCPV6_STATE_INACTIVE                             0U
/** \brief Wait for LinkLocal address */
#define TCPIP_DHCPV6_STATE_START_DELAY                          1U
/** \brief Startup state.*/
#define TCPIP_DHCPV6_STATE_INIT                                 2U
/** \brief Wait for DCHPOv6 ADVERTISEs.*/
#define TCPIP_DHCPV6_STATE_SOLICIT                              3U
/** \brief Requested specific lease.*/
#define TCPIP_DHCPV6_STATE_REQUESTING                           4U
/** \brief Lease has been assigned, is in use.*/
#define TCPIP_DHCPV6_STATE_BOUND                                5U
/** \brief Renewing a lease. */
#define TCPIP_DHCPV6_STATE_RENEWING                             6U
/** \brief Requesting lease extension from any server.*/
#define TCPIP_DHCPV6_STATE_REBINDING                            7U
/** \brief Requested specific lease in response to NoBinding status code.*/
#define TCPIP_DHCPV6_STATE_REASSIGNING                          8U
/** \brief Dhcp DAD address available but can't be used.*/
#define TCPIP_DHCPV6_STATE_TENTATIVE                            9U
/** \brief Dhcp DAD address available and can be used.*/
#define TCPIP_DHCPV6_STATE_OPTIMISTIC                          10U
/** \brief Dhcp DAD address is not unique and can't be used.*/
#define TCPIP_DHCPV6_STATE_CONFLICTED                          11U

/* DHCPv6 Option codes */
/** \brief Invalid Option code */
#define TCPIP_DHCPV6_INVALID_OPTION_CODE                        0U
/** \brief Client Identifier Option */
#define TCPIP_DHCPV6_OPTION_CLIENTID                            1U
/** \brief Server Identifier Option */
#define TCPIP_DHCPV6_OPTION_SERVERID                            2U
/** \brief Identity Association for Non-temporary Addresses Option */
#define TCPIP_DHCPV6_OPTION_IA_NA                               3U
/** \brief IA Address Option */
#define TCPIP_DHCPV6_OPTION_IAADDR                              5U
/** \brief Option Request Option */
#define TCPIP_DHCPV6_OPTION_ORO                                 6U
/** \brief Elapsed Time Option */
#define TCPIP_DHCPV6_OPTION_ELAPSED_TIME                        8U
/** \brief Server Unicast Option */
#define TCPIP_DHCPV6_OPTION_STATUS_CODE                        13U
/** \brief length of status code options in  bytes */
#define TCPIP_DHCPV6_OPTION_STATUS_CODE_LEN                     2U

/** \brief Retry transmission in next mainfunction */
#define TCPIP_DHCPV6_FLAG_NXT_MAIN                            0x01U

/** \brief LinkLocal address assigned continue to state INIT */
#define TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED         0x02U

/** \brief Checks if the NXT_MAIN flag is set */
#define TCPIP_DHCPV6_FLAG_NXT_MAIN_IS_SET(flags)                  \
  ((((flags) & TCPIP_DHCPV6_FLAG_NXT_MAIN) == TCPIP_DHCPV6_FLAG_NXT_MAIN))

/** \brief Checks if the INIT flag is set */
#define TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED_IS_SET(flags)                  \
  ((((flags) & TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED) == TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED))

/* DHCPv6 Status Codes */
/** \brief Success Status Code */
#define TCPIP_DHCPV6_STATUS_CODE_SUCCESS                         0U
/** \brief Failure, reason unspecified Status Code */
#define TCPIP_DHCPV6_STATUS_CODE_UNSPECFAIL                      1U
/** \brief Server has no address available Status Code */
#define TCPIP_DHCPV6_STATUS_CODE_NOADDRSAVAIL                    2U
/** \brief Client record (binding) unavailable Status Code */
#define TCPIP_DHCPV6_STATUS_CODE_NOBINDING                       3U
/** \brief The prefix for the address is not appropriate for the link Status Code */
#define TCPIP_DHCPV6_STATUS_CODE_NOTONLINK                       4U
/** \brief Status Code sent by sever to client to force the use of multicast */
#define TCPIP_DHCPV6_STATUS_CODE_USEMULTICAST                    5U

/** \brief length of DHCPV6 FQDN option flags*/
#define TCPIP_DHCPV6_FQDN_FLAGS_SIZE                             1U

/** \brief Length of terminator DHCP FQDN Option */
#define TCPIP_DHCPV6_TERMINATOR_LENGTH                           1U

/** \brief Terminator of the DHCP FQDN Option */
#define TCPIP_DHCPV6_DOMAINNAME_TERMINATOR                       0U


/** \brief Factor to calculate 1s to 100ms steps */
#define TCPIP_DHCPV6_100MS_TO_1S_FACTOR                         10U

/** \brief Factor to calculate 100ms to 10ms steps */
#define TCPIP_DHCPV6_100MS_FACTOR                               10U

/** \brief Sets xid to 0 */
#define TCPIP_DHCPV6_SET_UNUSED_XID(pdata)                        \
  do                                                              \
  {                                                               \
    (pdata)[0U] = 0U;                                             \
    (pdata)[1U] = 0U;                                             \
    (pdata)[2U] = 0U;                                             \
  } while (0U)

/** \brief Sets server_ethAddr to 0 */
#define TCPIP_DHCPV6_SET_UNUSED_ETH_ADDR(pdata)                   \
  do                                                              \
  {                                                               \
    (pdata)[0U] = 0U;                                             \
    (pdata)[1U] = 0U;                                             \
    (pdata)[2U] = 0U;                                             \
    (pdata)[3U] = 0U;                                             \
    (pdata)[4U] = 0U;                                             \
    (pdata)[5U] = 0U;                                             \
  } while (0U)

/** \brief Sets DHCPv6 Multicast address to  FF02::1:2*/
#define TCPIP_DHCPV6_SET_ALL_DHCP_SERVER_MULTICAST(pdata)         \
  do                                                              \
  {                                                               \
    (pdata)[0U] = COMSTACK_CT_HTON_UINT32(0xFF020000U);           \
    (pdata)[1U] = 0x00000000U;                                    \
    (pdata)[2U] = 0x00000000U;                                    \
    (pdata)[3U] = COMSTACK_CT_HTON_UINT32(0x00010002U);           \
  } while (0U)

/** \brief Set the Transaction ID */
#define TCPIP_DHCPV6_SET_XID(dataPtr, value)                      \
  do                                                              \
  {                                                               \
    (dataPtr)[0U] = (uint8)((value) >> 24U);                      \
    (dataPtr)[1U] = (uint8)((value) >> 16U);                      \
    (dataPtr)[2U] = (uint8)((value) >> 8U);                       \
  } while (0)

/** \brief Set message header */
#define TCPIP_DHCPV6_SET_MSG_HEADER(dataPtr, msg_type, xid)       \
  (dataPtr)[0U] = (msg_type);                                     \
  (dataPtr)[1U] = (xid)[0U];                                      \
  (dataPtr)[2U] = (xid)[1U];                                      \
  (dataPtr)[3U] = (xid)[2U];                                      \

/** \brief check if received transaction id is correct*/
#define TCPIP_DHCPV6_EVALUATE_XID(xidtx, xidrx)                   \
  TS_MemCmp((xidtx), (xidrx), (TCPIP_DHCPV6_TRANSACTION_ID_LEN))

/** \brief Set option type*/
#define TCPIP_DHCPV6_SET_OPTION_TYP(dataPtr, option)              \
  TCPIP_SET16((dataPtr), 0, (option))

/** \brief Set IAADDR option type*/
#define TCPIP_DHCPV6_SET_IAADDR_OPTION_TYP(dataPtr, option)       \
  TCPIP_SET16((dataPtr), 0, (option))

/** \brief Set option length type*/
#define TCPIP_DHCPV6_SET_OPTION_LEN(dataPtr, len)                 \
  TCPIP_SET16((dataPtr), 2, (len))

/** \brief Set IAADDR option length type*/
#define TCPIP_DHCPV6_SET_IAADDR_OPTION_LEN(dataPtr, len)          \
  TCPIP_SET16((dataPtr), 2, (len))

/** \brief Set DUID type*/
#define TCPIP_DHCPV6_SET_DUID_TYP(dataPtr, option)                \
  TCPIP_SET16((dataPtr), 4, (option))

/** \brief Set DUID length type*/
#define TCPIP_DHCPV6_SET_DUID_LEN(dataPtr, len)                   \
  TCPIP_SET16((dataPtr), 6, (len))

/** \brief Set elapsed time */
#define TCPIP_DHCPV6_SET_ELAPSED_TIME(dataPtr, eTime)             \
  TCPIP_SET16((dataPtr), 4, (eTime));

/** \brief Get  message type */
#define TCPIP_DHCPV6_GET_MSG_TYPE(dhcphdr)                        \
  ((dhcphdr)[0U])

/** \brief Get DUID type */
#define TCPIP_DHCPV6_GET_DUID_TYPE(opthdr)                        \
  TCPIP_GET16((opthdr),0)

/** \brief Get DUID type */
#define TCPIP_DHCPV6_GET_HWTYPE_TYPE(opthdr)                      \
  TCPIP_GET16((opthdr),2)

/** \brief Clears the reserved MBZ field of Dhcp FQDN Option */
#define TCPIP_DHCPV6_CLEAR_OPT_FQDN_MBZ(dhcphdr)                  \
  (dhcphdr)[4U] &= 0x0FU;

/** \brief Sets the S bit to 0 of Dhcp FQDN Option */
#define TCPIP_DHCPV6_CLEAR_OPT_FQDN_S(dhcphdr)                    \
  (dhcphdr)[4U] &= 0xFEU;

/** \brief Sets the O bit to 0 of Dhcp FQDN Option */
#define TCPIP_DHCPV6_CLEAR_OPT_FQDN_O(dhcphdr)                    \
  (dhcphdr)[4U] &= 0xFDU;

/** \brief Sets the N bit to 1 of Dhcp FQDN Option */
#define TCPIP_DHCPV6_SET_OPT_FQDN_N(dhcphdr)                      \
  (dhcphdr)[4U] |= 0x04U;

/** \brief Sets the domain name of Dhcp FQDN Option */
#define TCPIP_DHCPV6_SET_OPT_FQDN_DOMAIN_NAME(dhcphdr, name, len) \
  TS_MemCpy(&(dhcphdr)[5U], name, len);

#define TCPIP_DHCPV6_GET_IPV6_ADDR_PTR(pdata)                     \
    (&(pdata)[TCPIP_DHCPV6_OPTION_HEADER_LEN])

/** \brief Get option id */
#define TCPIP_DHCPV6_GET_OPTION_ID(opthdr)                        \
  TCPIP_GET16((opthdr),0)

/** \brief Get option size */
#define TCPIP_DHCPV6_GET_OPTION_SIZE(opthdr)                      \
  TCPIP_GET16(opthdr,2)

/** \brief Get IA_NA option */
#define TCPIP_DHCPV6_GET_IA_NA_OPTION(opthdr)                     \
  TCPIP_GET16(opthdr, 16)

/** \brief Get option size */
#define TCPIP_DHCPV6_GET_IA_NA_OPTION_SIZE(opthdr)                \
  TCPIP_GET16((opthdr), 18)

/** \brief Get the IAID option*/
#define TCPIP_DHCPV6_GET_IAID(opthdr)                             \
  TCPIP_GET32((opthdr), 4)

/** \brief set the IAID option*/
#define TCPIP_DHCPV6_SET_IAID(opthdr, iaid)                       \
  TCPIP_SET32((opthdr), 4, (iaid));

/** \brief Get the Renewal (T1) Time Value option */
#define TCPIP_DHCPV6_GET_T1_TIME(opthdr)                          \
  TCPIP_GET32((opthdr), 8)

/** \brief Get the Renewal (T2) Time Value option */
#define TCPIP_DHCPV6_GET_T2_TIME(opthdr)                          \
  TCPIP_GET32((opthdr), 12)

/** \brief set the Renewal (T1) Time Value option */
#define TCPIP_DHCPV6_SET_T1_TIME(opthdr, t1)                      \
  TCPIP_SET32((opthdr), 8, t1);

/** \brief set the Renewal (T2) Time Value option */
#define TCPIP_DHCPV6_SET_T2_TIME(opthdr, t2)                      \
  TCPIP_SET32((opthdr), 12, t2);

/** \brief Get the Preferred Time Value option */
#define TCPIP_DHCPV6_GET_PREFERRED_TIME(opthdr)                   \
  TCPIP_GET32((opthdr), 20)

/** \brief Get the Valit Time Value option */
#define TCPIP_DHCPV6_GET_VALID_TIME(opthdr)                       \
  TCPIP_GET32((opthdr), 24)

/** \brief set the Preferred Time Value option */
#define TCPIP_DHCPV6_SET_PREFERRED_TIME(opthdr, prefTime)         \
  TCPIP_SET32((opthdr), 20, prefTime);

/** \brief set the Valit Time Value option */
#define TCPIP_DHCPV6_SET_VALID_TIME(opthdr, validTime)            \
  TCPIP_SET32((opthdr), 24, validTime);

/** \brief set the Server ID */
#define TCPIP_DHCPV6_SET_SERVER_ID(opthdr, serveridPtr, serveridLen) \
    TS_MemCpy((&(opthdr)[4U]), (serveridPtr), (serveridLen));

/** \brief set the IPv6 address */
#define TCPIP_DHCPV6_SET_ADDR(opthdr, addr)                      \
  TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD((&(opthdr)[4U]), (addr));


/** \brief Get the Status Code */
#define TCPIP_DHCPV6_GET_STATUS_CODE(opthdr)                     \
  TCPIP_GET16((opthdr), 4)

/** \brief Sets DHCPv6 elapsed time*/
#define TCPIP_DHCPV6_SET_ELAPSED_TIME_OPTION(pdata, secs)                         \
  do                                                                              \
  {                                                                               \
    TCPIP_DHCPV6_SET_OPTION_TYP((pdata), TCPIP_DHCPV6_OPTION_ELAPSED_TIME);       \
    TCPIP_DHCPV6_SET_OPTION_LEN((pdata), TCPIP_DHCPV6_ELAPSED_TIME_OPTION_LEN);   \
    TCPIP_DHCPV6_SET_ELAPSED_TIME((pdata), secs);                                 \
  } while (0U)

/** \brief Sets DHCPv6 option request option */
#define TCPIP_DHCPV6_SET_OPTION_ORO_TIME_OPTION(pdata)                       \
  do                                                                         \
  {                                                                          \
    TCPIP_DHCPV6_SET_OPTION_TYP((pdata), TCPIP_DHCPV6_OPTION_ORO);           \
    TCPIP_DHCPV6_SET_OPTION_LEN((pdata), 0U);                                \
  } while (0U)


/** \brief Set Server id option  */
#define TCPIP_DHCPV6_SET_SERVER_ID_OPTION(dataPtr, serveridPtr, serveridLen )  \
  do                                                                           \
  {                                                                            \
    TCPIP_DHCPV6_SET_OPTION_TYP((dataPtr), TCPIP_DHCPV6_OPTION_SERVERID);      \
    TCPIP_DHCPV6_SET_OPTION_LEN((dataPtr), serveridLen);                       \
    TCPIP_DHCPV6_SET_SERVER_ID ((dataPtr), serveridPtr, serveridLen);          \
  } while (0U)


/** \brief Set FQDN option header */
#define TCPIP_DHCPV6_SET_FQDN_OPTION(dataPtr, len)                                \
  do                                                                              \
  {                                                                               \
    TCPIP_DHCPV6_SET_OPTION_TYP((dataPtr), TCPIP_DHCPV6_OPTION_FQDN);             \
    TCPIP_DHCPV6_SET_OPTION_LEN((dataPtr), len);                                  \
    TCPIP_DHCPV6_CLEAR_OPT_FQDN_MBZ(dataPtr)                                      \
    TCPIP_DHCPV6_CLEAR_OPT_FQDN_S(dataPtr)                                        \
    TCPIP_DHCPV6_CLEAR_OPT_FQDN_O(dataPtr)                                        \
    TCPIP_DHCPV6_SET_OPT_FQDN_N(dataPtr)                                          \
  } while(0U)

/** \brief Verifies if requested ip address is unspecified */
#define TCPIP_DHCPV6_REQ_IP_IS_UNSPECIFIED(reqIpAddr)                             \
  (TS_MemCmp((reqIpAddr), TcpIp_DhcpV6_unspecified, TCPIP_IPV6_ADDR_BYTE_SIZE) == E_OK)

/** \brief Verifies if requested ip address is multicast */
#define TCPIP_DHCPV6_REQ_IP_IS_MULTICAST(reqIpAddr)                               \
  ((reqIpAddr)[0] == 0xFFU)

/** \brief Close DHCP socket
 *
 * In State SOLICIT. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_SOLICIT_INACTIVE(dhcpIdx)       \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REQUESTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_REQUESTING_INACTIVE(dhcpIdx)     \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State CONFLICTED. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages. It additionally reinitializes
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_CONFLICTED_INACTIVE(dhcpIdx)     \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State RENEWING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_RENEWING_INACTIVE(dhcpIdx, Addr) \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State REASSIGNING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_REASSIGNING_INACTIVE(dhcpIdx, Addr) \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State REBINDING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_stopAssign_REBINDING_INACTIVE(dhcpIdx, Addr) \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Retransmission of DHCPv6 REQUEST
 *
 * In State REASSIGNING. This function retransmits the DHCPv6 REQUEST message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_retransTimeout_REASSIGNING_REASSIGNING(dhcpIdx) \
  TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_REQUESTING((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State SELECTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_close_SOLICIT_INACTIVE(dhcpIdx)              \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State REQUESTING. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_close_REQUESTING_INACTIVE(dhcpIdx)            \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Close DHCP socket
 *
 * In State RENEWING. This function informs address manager that IP shall no longer
 * be used and closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
#define  TcpIp_DhcpV6_tr_close_RENEWING_INACTIVE(dhcpIdx, Addr)       \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State REBINDING: This function informs address manager that IP shall no longer be
 * used and closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
#define TcpIp_DhcpV6_tr_close_REBINDING_INACTIVE(dhcpIdx, Addr)       \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State REBINDING: This function informs address manager that IP shall no longer be
 * used and closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
#define TcpIp_DhcpV6_tr_close_REASSIGNING_INACTIVE(dhcpIdx, Addr)       \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State BOUND: This function informs address manager that IP shall no longer be
 * used and closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
#define TcpIp_DhcpV6_tr_close_BOUND_INACTIVE(dhcpIdx, Addr)       \
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE((dhcpIdx), (Addr))

/** \brief Close DHCP socket
 *
 * In State INIT. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitialize
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_close_INIT_INACTIVE(dhcpIdx)                  \
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE((dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State REBINDING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_received_REBINDING_INIT(Addr, dhcpIdx)       \
  TcpIp_DhcpV6_tr_received_EXPANDING_INIT((Addr), (dhcpIdx))


/** \brief Restart DHCP assignment process
 *
 * In State CONFLICTED. This function restarts the DHCP assignment process
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_received_CONFLICTED_INIT(dhcpIdx)       \
  TcpIp_DhcpV6_entry_INIT((dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State REASSIGNING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_retransTimeout_REASSIGNING_INIT(Addr, dhcpIdx)   \
  TcpIp_DhcpV6_tr_received_EXPANDING_INIT((Addr), (dhcpIdx))

/** \brief Delay transmission of DHCP DISCOVER message
 *
 * In State REQUESTING. This function reinitialize DHCP variables and delays the
 * transmission of the DHCP DISCOVER message by updating the transmission timeout.
 * (Called if NACK is received during request process.)
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_timeout_REQUESTING_INIT(dhcpIdx)             \
  TcpIp_DhcpV6_tr_received_REQUESTING_INIT((dhcpIdx))


/** \brief Restart DHCP assignment process
 *
 * In State REQUESTING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_INIT(dhcpIdx)       \
  TcpIp_DhcpV6_tr_received_REQUESTING_INIT((dhcpIdx))


/** \brief Restart DHCP assignment process
 *
 * In State CONFLICTED. This function restarts the DHCP assignment process
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_retransTimeout_CONFLICTED_INIT(dhcpIdx)       \
  TcpIp_DhcpV6_entry_INIT((dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State REBINDING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message. (called if NACK is received.)
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_timeout_REBINDING_INIT(Addr, dhcpIdx)        \
  TcpIp_DhcpV6_tr_received_EXPANDING_INIT((Addr), (dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State RENEWING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message. (called if NACK is received.)
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_timeout_RENEWING_INIT(Addr, dhcpIdx)        \
  TcpIp_DhcpV6_tr_received_EXPANDING_INIT((Addr), (dhcpIdx))

/** \brief Restart DHCP assignment process
 *
 * In State REASSIGNING. This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used and delays the transmission of the
 * first DISCOVER message.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
#define TcpIp_DhcpV6_tr_timeout_REASSIGNING_INIT(Addr, dhcpIdx)       \
  TcpIp_DhcpV6_tr_received_EXPANDING_INIT((Addr), (dhcpIdx))

/** \brief check message
 *
 * This function checks if received response for Rebinding message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 * \param[in]  localAddr  Local address id of the DHCP assignment
 *
 */
#define TcpIp_DhcpV6_evaluateRebindingResponse(dhcpIdx, opt, msg_type, localAddr)    \
  TcpIp_DhcpV6_evaluateRenewingResponse((dhcpIdx), (opt), (msg_type), (localAddr))

/** \brief Exit assigned state
 *
 * This function informs the IpAddrM that the IP address shall no longer be used.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
#define TcpIp_DhcpV6_exit_ASSIGNED(Addr)       \
  TcpIp_IpAddrM_AssignSM_unassigned((Addr), TCPIP_IPADDRM_ASSIGN_DHCPV6)
/*==================[type definitions]======================================*/
#if (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)

/** \brief This structure defines the runtime data of a DHCPv6 assignment. */
typedef struct
{
  uint32 timer;
  /**< Timer for the statemachine */
  uint32 rtx_timer;
  /**< Retransmission timer */
  uint32 valid_time;
  /**< valid_time.*/
  uint32 preferred_time;
  /**< preferred Time Option */
  uint32 T1_time;
  /**< Renewal (T1) Time Value Option */
  uint32 T2_time;
  /**< Rebinding (T2) Time Value Option*/
  uint32 rtx_timeout;
  /**< Start value of rtx_timer.*/
  uint32 IaId;
  /**< Identity Association Identifier.*/
  uint32 ip_addr [TCPIP_IPV6_ADDR_SIZE];
  /**< assigned IP address */
  uint16 elapsed_time;
  /**< elapsed time that passed since the beginning of the acquisition or renewal process in
       expressed in hundredths of a second.*/
  TcpIp_SocketIdType socketId;
  /**< id of the socket connection */
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
  uint16 option_length[TCPIP_DHCPV6_MAX_OPTIONS];
  /**< real option lengths */
  uint8 option_data[TCPIP_DHCPV6_MAX_OPTION_DATA];
  /**< option data for all options  */
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */
  uint8 xid [TCPIP_DHCPV6_TRANSACTION_ID_LEN];
  /**< Transaction ID.*/
  uint8 server_id [TCPIP_DHCPV6_SERVER_DUID_SIZE];
  /**< Server Identifier Option */
  uint8 server_id_len;
  /**< Server Identifier Option length */
  uint8 state;
  /**< State of DHCP statemachine */
  uint8 num_retry;
  /**< number of retires */
  uint8 req_dec_num_retry;
  /**< number of requesting/decline retransmissions */
  uint8 flags;
  /**< retry transmission in next mainfunction */
  uint8 expiry_state;
  /**< expiry state of the address */
#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)
  uint8 domain_name[TCPIP_DHCPV6_DOMAINNAME_SIZE];
  /**< hostname of the dhcp client */
  uint8 domain_name_len;
  /**< length of the hostname */
#endif /* TCPIP_DHCPV6_FQDN_ENABLED == STD_ON */
  uint8 msg_type;
  /**< dhcpV6 message type to be transmitted */
} TcpIp_DhcpV6_DataType;

/** \brief Holds important received DHCP options */
typedef struct
{
  uint32 valid_time;
  /**< IP Address valid Time Option */
  uint32 preferred_time;
  /**< IP Address preferred Time Option */
  uint32 IaId;
  /**< Identity Association Identifier.*/
  uint32 T1_time;
  /**< Renewal (T1) Time Value Option */
  uint32 T2_time;
  /**< Rebinding (T2) Time Value Option*/
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ip_req;
  /**< request address*/
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) server_id;
  /**< Server Identifier Option */
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) client_id;
   /**< Client Identifier Option */
  uint16 status_code;
  /**< Status Code Option in option field*/
  uint16 status_code_IA_NA;
    /**< Status Code Option in IA_NA field*/
  uint16 status_code_IAADDR;
    /**< Status Code Option in IAADDR field*/
  uint8 server_id_len;
  /**< Server Identifier Option length*/
}TcpIp_DhcpV6_OptionType;
/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Retrieve DHCP options
 *
 * This function extracts all DHCP options from the beginning of the data pointer until the
 * end of option field.
 *
 * \param[in]  dhcpIdx Index to the DHCP data structure
 * \param[in]  optPtr  Points to the beginning of the option field in a received DHCP
 *                     messages
 * \param[in]  optlen  Length of the option field
 * \param[out] opts    Holds values of the DHCP options
 *
 * \retval E_OK        All options successfully extracted
 * \retval E_NOT_OK    Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
);


/** \brief Retrieve IANA options
 *
 * This function extracts all timout values from the option field.
 *
 * \param[in]  optPtr  Points to the beginning of the option field in a received IANA
 *                     messages
 * \param[out] opts    Holds values of the DHCP options
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_parseIaNaHeader
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
);

/** \brief Retrieve IANA options
 *
 * This function extracts all IANA options from the beginning of the data pointer until the
 * end of option field.
 *
 * \param[in]  optPtr  Points to the beginning of the option field in a received IANA
 *                     messages
 * \param[in]  optlen  Length of the option field
 * \param[out] opts    Holds values of the DHCP options
 *
 * \retval E_OK        All options successfully extracted
 * \retval E_NOT_OK    Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseIaNaOptions
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
);

/** \brief Retrieve IAADDR options
 *
 * This function extracts all IAADDR options from the beginning of the data pointer until the
 * end of option field.
 *
 * \param[in]  optPtr  Points to the beginning of the option field in a received IAADDR
 *                     messages
 * \param[in]  optlen  Length of the option field
 * \param[out] opts    Holds values of the DHCP options
 *
 * \retval E_OK        All options successfully extracted
 * \retval E_NOT_OK    Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseIaAddrOption
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
);


/** \brief Retrieve DHCP options from option
 *
 * This function extracts all DHCP options and saves them in a structure.
 *
 * \param[in]  dhcpIdx Index to the DHCP data structure
 * \param[in]  dataPtr Points to the beginning of the  header
 * \param[in]  optlen  Length of the  message
 * \param[out] opts    Holds values of the DHCP options
 *
 * \retval E_OK        All options successfully extracted
 * \retval E_NOT_OK    Encountered a malformed Option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_readOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
);

/** \brief Transmit DHCP message
 *
 * This function retrieves a hardware buffer, constructs the messages specified by
 * msg_type and hands the data over to the IP layer.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message type that shall be transmitted
 *
 * \retval TCPIP_OK        Message successfully transmitted
 * \retval TCPIP_E_DROP    Link down, or out of memory
 * \retval TCPIP_E_NOT_OK  Transmission error occurred
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_transmit
(
  uint8 dhcpIdx,
  uint8 msg_type
);

/** \brief Process retransmission timeout
 *
 * This function decides what to do with a retransmission timeout in a specific state
 * of the DHCPv6 state machine.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_retransTimeout
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Process state timeout
 *
 * This function decides what to do if a lease timeout occurs in a specific state
 * of the DHCPv6 state machine.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_timeout
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Insert Dhcp message fields
 *
 * This function appends the DHCP header at the location specified by buffPtr.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  buffPtr    Pointer to the beginning of the DHCP header
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message type that shall be transmitted
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_buildHeader
(
  TcpIp_LocalAddrIdType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) buffPtr,
  uint8 dhcpIdx,
  uint8 msg_type
);


/** \brief Set message specific options
 * DHCP messages contains different DHCP options, this function adds the messages
 * specific options to the DHCP message.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message type of which the option shall be written
 * \param[in]  opthdr     Points to the beginning of the first specific option
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setMsgSpecificOptions
(
  uint8 dhcpIdx,
  uint8 msg_type,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
);


/** \brief Set IA_NA options
 * this function adds the IA_NA options to the DHCP message, and
 * address option part of IA_NA option for message types that need it
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  opthdr       Points to the beginning of IA_NA option
 * \param[in]  iaadr_option TRUE: set IAADDR option,
 *                          FALSE: don't set IAADDR option
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setIaNaOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr,
  boolean iaadr_option
);


/** \brief Set IAADR option
 * this function adds the IAADR options to the DHCP message
 *
 * \param[in]  dhcpIdx      Index to the DHCP data structure
 * \param[in]  opthdr       Points to the beginning of IA_NA option
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setIaAddrOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
);

/** \brief Set Client ID option
 * this function adds the Client ID options to the DHCP message
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  opthdr       Points to the beginning of Client ID option
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setClientIdOption
(
  TcpIp_LocalAddrIdType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
);

/** \brief Get length of DHCP message
 *
 * This function returns the length of the DHCP messages specified by msg_type. (Called
 * to evaluate buffer size)
 *
 * \param[in]  dhcpIdx   Index to the DHCP data structure
 * \param[in]  msg_type  DHCP message type of which the length shall be evaluated
 *
 * \return length of the DHCP messages
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getMsgLength
(
  uint8 msg_type,
  uint8 dhcpIdx
);

/** \brief Calculate lifetimes
 *
 * This function calculates the lifetimes and the T1 and T2 renewal time.
 *
 * \param[in]  opt      Holds values of the received DHCP options
 *
 * \retval E_OK      operation successful
 * \retval E_NOT_OK  dhcp option contains an error
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateLifeTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Process received DHCPv6 messages
 *
 * This function decides what to do with a received DHCPv6 message in a specific state
 * of the DHCPv6 state machine.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_received
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
);

/** \brief check message
 *
 * This function checks if received response for solicit message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateSolicitResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
);


/** \brief check message
 *
 * This function checks if received response for requesting message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 * \param[in]  localAddr  Local address id of the DHCP assignment
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateRequestingResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
);


/** \brief check message
 *
 * This function checks if received response for Renewing message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 * \param[in]  localAddr  Local address id of the DHCP assignment
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateRenewingResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
);


/** \brief check message
 *
 * This function checks if received response for Reassigning message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 * \param[in]  localAddr  Local address id of the DHCP assignment
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateReassigningResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Open UDP socket
 *
 * This function DHCP assignment process
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_startAssign_INACTIVE_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Open UDP socket
 *
 * This function retrieves a UDP socket and binds it to the limited broadcast IP address.
 * It also sets a timeout for the first DHCP SOLICIT transmission.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Stop DHCP assignment process
 *
 * In State INIT. This function reinitialize DHCP variables and moves
 * the DHCP state to closed.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_START_DELAY_INACTIVE
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
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE
(
  uint8 dhcpIdx
);

/** \brief Close DHCP socket
 *
 * In State BOUND. This function closes the DHCP socket to stop listening for
 * incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE
(
  uint8 dhcpIdx,
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Transmit Solicit message
 *
 * This function transmits a DHCPv6 Solicit message and specifies the retransmission
 * timeout.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_INIT_SOLICIT
(
  uint8 dhcpIdx
);

/** \brief Expand IP address lease
 *
 * This function transmits a DHCPv6 RENEW message to the leasing server to expand the
 * lease time of the IP address.
 *
 * \param[in]  dhcpIdx          Index to the DHCP data structure
 * \param[in]  renewalTimeout   Time left from T2 timer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_BOUND_RENEWING
(
  uint8 dhcpIdx,
  uint32 renewalTimeout
);

/** \brief Expand IP address lease
 *
 * This function transmits a DHCPv6 REBINDING message to expand the lease
 * time of the IP address.
 *
 * \param[in]  dhcpIdx        Index to the DHCP data structure
 * \param[in]  leaseTimeout   Time left from valid/preferred timer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_RENEWING_REBINDING
(
  uint8 dhcpIdx,
  uint32 leaseTimeout
);


/** \brief update IP address Expiry state
 *
 * This function sets Expiry state of the address to DEPRECATED
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
);

/** \brief Request Ip address
 *
 * This function transmits a DHCPv6 REQUEST message to lease an IP address. (called
 * if ADVERTISE is received.)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_SOLICIT_REQUESTING
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Request Ip address
 *
 * This function transmits a DHCPv6 REQUEST message to lease an IP address. (called
 * if NoBinding is received.)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_REASSIGNING
(
  uint8 dhcpIdx
);

#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF)
/** \brief Assign address
 *
 * This function assigns the address to the controller by calling
 * TcpIp_IpAddrM_AssignSM_assigned.
 *
 * \param[in] localAddrId   Local address id of the dhcp assignment
 * \param[in] dhcpIdx       Index to the DHCP data structure
 * \param[in] opt           Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

#else /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */

/** \brief Result of Duplicate address detection
 *
 * \param[in] localAddrId  Index of the address Id on which duplicate address detection
 *                         was performed
 * \param[in] unique       TRUE: address is not used by any other node in the network
 *                         FALSE: duplicate detected
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
);

/** \brief Restart DHCP assignment process
 *
 * This function restarts the DHCP assignment process, informs
 * address manager that IP shall no longer be used, delays the transmission of the
 * first DISCOVER message, stops duplicate address detection and transmits DECLINE message.
 *
 * \param[in]  Addr     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_dadResult_entry_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

/** \brief Enter bound state after DAD state
 *
 * This function enters bound state after tentative or optimistic dad verifies address is unique
 *
 * \param[in]  localAddrId     Local address id of the DHCP assignment
 * \param[in]  dhcpIdx         Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_dadResult_DAD_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
);

/** \brief Assign address
 *
 * This function assigns the address to the controller by calling
 * TcpIp_IpAddrM_AssignSM_assigned and sets the address to be used
 * while uniqueness is being verified in state OPTIMISTIC or
 * sets the address to TENTATIVE where it can't be used while
 * uniqueness is being verified
 *
 * \param[in] localAddrId   Local address id of the dhcp assignment
 * \param[in] dhcpIdx       Index to the DHCP data structure
 * \param[in] opt           Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_DAD
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief check message
 *
 * This function checks if received response for Decline message is correct.
 * and transitions to expected state
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 * \param[in]  msg_type   DHCP message type
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateConflictedResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
);

/** \brief Retransmission of DHCPv6 DECLINE
 *
 * In State CONFLICTED. This function retransmits the DHCPv6 DECLINE message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_CONFLICTED_CONFLICTED
(
  uint8 dhcpIdx
);

#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)

/** \brief Enter optimistic state
 *
 * This function enters the state OPTIMISTIC and starts using the received address.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_OPTIMISTIC
(
  uint8 dhcpIdx
);

/** \brief Close DHCP socket
 *
 * In State OPTIMISTIC. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server and informs the IP address manager
 * that the IP address shall not be used.
 *
 * \param[in]  dhcpIdx     Index to the DHCP data structure
 * \param[in]  localAddr   Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_OPTIMISTIC_INACTIVE
(
 uint8 dhcpIdx,
 TcpIp_LocalAddrIdType localAddr
);

#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

/** \brief Enter tentative state
 *
 * This function enters the state TENTATIVE and waits for DAD to finish in order to start using
 * the received address.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_TENTATIVE
(
  uint8 dhcpIdx
);

/** \brief Close DHCP socket
 *
 * In State TENTATIVE. This function closes the DHCP socket to stop listening
 * for incoming DHCP messages from a DHCP server. It additionally reinitializes
 * DHCP variables and moves the DHCP state to closed.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_TENTATIVE_INACTIVE
(
 uint8 dhcpIdx,
 TcpIp_LocalAddrIdType localAddr
);
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */

/** \brief Restart DHCP assignment process
 *
 * This function restarts the DHCP assignment process and delays the transmission of
 * the first DISCOVER message.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_INIT
(
  uint8 dhcpIdx
);

/** \brief Restart DHCP assignment process
 *
 * This function restarts the DHCP assignment process, informs address manager that
 * IP shall no longer be used and delays the transmission of the first DISCOVER message.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_INIT
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCPv6 SOLICIT
 *
 * In State SOLICIT. This function retransmits the DHCPv6 SOLICIT message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_SOLICIT_SOLICIT
(
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCPv6 REQUEST
 *
 * In State REQUESTING. This function retransmits the DHCPv6 REQUEST message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_REQUESTING
(
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCPv6 RENEW
 *
 * In State RENEWING. This function retransmits the DHCPv6 RENEW message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_RENEWING_RENEWING
(
  uint8 dhcpIdx
);

/** \brief Retransmission of DHCPv6 REBIND
 *
 * In State REBINDING. This function retransmits the DHCPv6 REBIND message and increases
 * the retransmission timeout. (Called if retransmission timer expires)
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
);

/** \brief Retransmission
 *
 * This function retransmits the specified message and  changes the retransmission timeout.
 * If the retransmission timeout reaches Maximum retransmission duration (MRD). The messages
 * will be retransmitted every MRD.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  msg_type   DHCP message type
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_increase
(
  uint8 dhcpIdx,
  uint8 msg_type
);

/** \brief Calculate retransmit timeout
 *
 * This function calculates initial retransmit timeout.
 *
 * \param[in]  initRtxTimeout  default initial retransmit timeout
 *
 * \return     random initial retransmit timeout
 */
STATIC FUNC(uint32, TCPIP_CODE) TcpIp_DhcpV6_calculateRetransTimeout
(
  uint16 initRtxTimeout
);

/** \brief Enter inactive state
 *
 * This function reinitializes the runtime data to its default values.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_INACTIVE
(
  uint8 dhcpIdx
);

/** \brief Enter start_delay state
 *
 * This function starts the DHCP assignment process.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);
/** \brief Entry active state
 *
 * This function retrieves a socket from the UDP module and binds it to the broadcast
 * ip address and DHCP client port.
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 *
 * \retval TCPIP_OK       Socket ready to send DHCP requests
 *         else           Socket could not be created
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_entry_ACTIVE
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
);

/** \brief Entry assigned multicast state
 *
 * This function informs the IpAddrM that the multicast IP address can be used
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_ASSIGNED_MULTICAST
(
  uint8 dhcpIdx
);

/** \brief Exit assigned multicast state
 *
 * This function informs the IpAddrM that the multicast IP address can't be used anymore
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST
(
  uint8 dhcpIdx
);

/** \brief Entry assigned state
 *
 * This function informs the IpAddrM that the IP address can be used
 *
 * \param[in]  localAddr  Local address id of the DHCP assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Enter init state
 *
 * This function restarts the DHCP assignment process.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_INIT
(
  uint8 dhcpIdx
);

/** \brief Save received options
 *
 * This function stops retransmissions and starts the T1 renewal timer.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 * \param[in]  opt     Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_pre_entry_BOUND_timeUpdate
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);


/** \brief Update renewing timeout
 *
 * This function updates the renewing timeout which indicates
 * the time when the DHCP tries to expand the life time of the IP address.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  opt        Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_BOUND
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
);

/** \brief Enter bound state
 *
 * This function sets address to preferred.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_BOUND
(
  uint8 dhcpIdx
);

/** \brief Exit active state
 *
 * This function closes the DHCP socket.
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_exit_ACTIVE
(
  uint8 dhcpIdx
);

/** \brief evaluates server id
 *
 * This function test if received server id is correct
 *
 * \param[in] opts       Holds values of the received DHCP options
 * \param[in] dhcpIdx    Index to the DHCP data structure
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateServerId
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx
);

/** \brief evaluates client id
 *
 * This function test if received client id is correct
 *
 * \param[in] opts       Holds values of the received DHCP options
 * \param[in] localAddr  Local address id of the DHCP assignment
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateClientId
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  TcpIp_LocalAddrIdType localAddr
);


/** \brief evaluates REPLY message
 *
 * This function test if the message type is REPLY and if the
 * server id is correct
 *
 * \param[in] opts        Holds values of the received DHCP options
 * \param[in] dhcpIdx     Index to the DHCP data structure
 * \param[in] msg_type    DHCP message type
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateReplyMsg
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx,
  uint8 msg_type
);


/** \brief evaluates received status code and timeouts
 *
 * This function test if the status code is success and if the
 * received timeouts are correct
 *
 * \param[in] opts        Holds values of the received DHCP options
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateStatusAndTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
);

/** \brief evaluates status code
 *
 * This function evaluates all status codes received
 *
 * \param[in] opt           Holds values of the received DHCP options
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateStatusCode
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
);

/** \brief evaluates received Ip address, IaId, lifetimes and the T1 and T2 renewal time
 *
 * This function test if the address is assigned and if the
 * received IaId is correct
 *
 * \param[in] opts        Holds values of the received DHCP options
 * \param[in] dhcpIdx     Index to the DHCP data structure
 * \param[in] checkTime   Flag if checking time is needed
 */

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx,
  boolean checkTime
);

#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)

/** \brief Set the FQDN Option
 *
 * This functions adds the FQDN option in DHCP REQUEST and SOLICIT messages.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  optptr   pointer to the DHCP option field in the DHCP message
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setOptionFQDN
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
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getOptionFQDNLength
(
  uint8 dhcpIdx
);

#endif /* (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) */

#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)

/** \brief Set the DhcpV6 Option
 *
 * This functions adds the DhcpV6 option in REQUEST and SOLICIT messages.
 *
 * \param[in]  dhcpIdx  Index to the DHCP data structure
 * \param[in]  optptr   pointer to the DHCP option field in the DHCP message
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
);

/** \brief Get total length of the DhcpV6 Option
 *
 * This functions returns the length of the DhcpV6 option
 *
 * \param[in]  dhcpIdx    Index to the DHCP data structure
 *
 * \return length of the FQDN option
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getOptionLength
(
  uint8 dhcpIdx
);

#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/** \brief IP address has not been assigned yet */
STATIC CONST(uint32, TCPIP_CONST) TcpIp_DhcpV6_unspecified[TCPIP_IPV6_ADDR_SIZE] = {0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief state information variable definition */
STATIC VAR(TcpIp_DhcpV6_DataType, TCPIP_VAR_CLEARED)
  TcpIp_DhcpV6_Data[TCPIP_NUM_DHCPV6_CLIENTS];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

/* Deviation MISRAC2012-4 */
STATIC VAR(uint8, TCPIP_VAR) TcpIp_DhcpV6_TimeoutCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6ReadOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint16 Option,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPV6READOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* report Det if TcpIp is not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_NOTINIT);
  }
  else if (LocalIpAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    /* report Det if Local Address exceeds number of configured addresses */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_INV_ARG);
  }
  else if(DataLength == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_PARAM_POINTER);
  }
  else if(DataPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON))
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,LocalIpAddrId));
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

    if(ctrlPtr->localAddrIdAny == LocalIpAddrId)
    {
      /* check if dhcp assignment exists on controller */
      LocalIpAddrId =
        TcpIp_IpAddrM_getLocalAddrOfAssignment(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
    }

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if(TcpIp_IpAddrM_isAssignmentInvalid(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6))
    {
      /* report Det if Local Address is not of type DHCP */
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_INV_ARG);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)

      if(Option == TCPIP_DHCPV6_OPTION_FQDN)
      {
        const uint8 dhcpIdx = TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
        P2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr;
        TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);
        TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        /* report Det if DataLength is not big enough for the fully qualified domain name */
        if((TcpIp_DhcpV6_DataPtr->domain_name_len != 0U) &&
           (*DataLength < ((uint16) TcpIp_DhcpV6_DataPtr->domain_name_len -
                           TCPIP_DHCPV6_TERMINATOR_LENGTH
                          )
           )
          )
        {
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_INV_ARG);
        }
        else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
        {
          TcpIp_Dhcp_readOptionFQDN(
                                     DataLength,
                                     DataPtr,
                                     &TcpIp_DhcpV6_DataPtr->domain_name[0U],
                                     TcpIp_DhcpV6_DataPtr->domain_name_len
                                   );
          retval = E_OK;
        }
      }
      else
#endif /* TCPIP_DHCPV6_FQDN_ENABLED == STD_ON */
      {
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
        CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionPtr =
            &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpV6Option,0));
        P2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr;
        TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);
        TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

        retval =
          TcpIp_Dhcp_readOption
            (Option,
             DataLength,
             DataPtr,
             TcpIp_DhcpV6_DataPtr->option_data,
             TcpIp_DhcpV6_DataPtr->option_length,
             dhcpOptionPtr,
             TcpIp_PBcfgPtr->dhcpv6Config.optionNum
            );

#endif /* TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
        if (retval == E_NOT_OK)
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */
        {
         /* unknown option */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
      }
    }
#else /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */
    TS_PARAM_UNUSED(LocalIpAddrId);
    TS_PARAM_UNUSED(Option);
    TS_PARAM_UNUSED(DataLength);
    TS_PARAM_UNUSED(DataPtr);
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    /* unknown option */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6READOPTION, TCPIP_E_INV_ARG);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
#endif /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */
  }

  DBG_TCPIP_DHCPV6READOPTION_EXIT(retval,LocalIpAddrId,Option,DataLength,DataPtr);

  return retval;
}

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6WriteOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint16 Option,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPV6WRITEOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* report Det if TcpIp is not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_NOTINIT);
  }
  else if(LocalIpAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    /* report Det if Local Address exeeds num of configured addresses */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_INV_ARG);
  }
  else if(DataPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON))
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,LocalIpAddrId));
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

    if(ctrlPtr->localAddrIdAny == LocalIpAddrId)
    {
      /* check if dhcp assignment exists on controller */
      LocalIpAddrId =
        TcpIp_IpAddrM_getLocalAddrOfAssignment(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
    }

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if(TcpIp_IpAddrM_isAssignmentInvalid(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6))
    {
      /* report Det if Local Address is not of type DHCP */
      TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_INV_ARG);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)
      if (Option == TCPIP_DHCPV6_OPTION_FQDN)
      {
        if(DataLength > (TCPIP_DHCPV6_DOMAINNAME_SIZE - 1U))
        {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
          /* report Det if data length is bigger than buffer size */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_INV_ARG);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
        }
        else
        {
          const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
          P2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr;
          TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);
          TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

          TcpIp_Dhcp_writeOptionFQDN(
                                      DataLength,
                                      DataPtr,
                                      &TcpIp_DhcpV6_DataPtr->domain_name[0U],
                                      &TcpIp_DhcpV6_DataPtr->domain_name_len
                                    );
          retval = E_OK;
        }
      }
      else
#endif /* TCPIP_DHCPV6_FQDN_ENABLED == STD_ON */
      {
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
        const uint8 dhcpIdx =
          TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
        CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionPtr =
            &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpV6Option,0));
        P2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr;
        TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);
        TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

        retval =
          TcpIp_Dhcp_writeOption
            (Option,
             DataLength,
             DataPtr,
             TcpIp_DhcpV6_DataPtr->option_data,
             TcpIp_DhcpV6_DataPtr->option_length,
             dhcpOptionPtr,
             TcpIp_PBcfgPtr->dhcpv6Config.optionNum
             );

#endif /* TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
        if (retval == E_NOT_OK)
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */
        {
          /* unknown option or invalid option length */
          TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_INV_ARG);
        }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
      }
    }
#else /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */
    TS_PARAM_UNUSED(LocalIpAddrId);
    TS_PARAM_UNUSED(Option);
    TS_PARAM_UNUSED(DataLength);
    TS_PARAM_UNUSED(DataPtr);
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    /* unknown option */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPV6WRITEOPTION, TCPIP_E_INV_ARG);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
#endif /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */
  }

  DBG_TCPIP_DHCPV6WRITEOPTION_EXIT(retval,LocalIpAddrId,Option,DataLength,DataPtr);

  return retval;
}

#if  ((TCPIP_DHCPGETSTATUS_API == STD_ON) && (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON))
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_getStatus
(
  TcpIp_LocalAddrIdType localAddressId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) statusPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_DHCPV6_GETSTATUS_ENTRY(localAddressId , statusPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TcpIp_IpAddrM_isAssignmentInvalid(localAddressId, TCPIP_IPADDRM_ASSIGN_DHCPV6))
  {
    /* report Det if Local Address is not of type DHCP */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPGETSTATUS, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    const uint8 dhcpIdx =
      TcpIp_IpAddrM_getAssignmentIdx(localAddressId, TCPIP_IPADDRM_ASSIGN_DHCPV6);

    TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, TCPIP_INTERNAL_API_ID);

    TS_AtomicAssign8(*statusPtr, TcpIp_DhcpV6_Data[(dhcpIdx)].state);
    retval = E_OK;
  }

  DBG_TCPIP_DHCPV6_GETSTATUS_EXIT(retval, localAddressId, statusPtr);
  return retval;
}
#endif /* ((TCPIP_DHCPGETSTATUS_API == STD_ON) && (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)) */

#if (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)

FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_LocalIpAddrAssignmentChg
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
)
{
  uint8_least i;

  DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_ENTRY(IpAddrId,State);

  if(!(TcpIp_IpAddrM_isAssignmentInvalid(IpAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6)))
  {
    /* loop over all dhcp assignments */
    for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv6Num; i++)
    {
      const TcpIp_LocalAddrIdType dhcpLocalAddr =
        TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV6);
      const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(dhcpLocalAddr);
      TcpIp_LocalAddrIdType LinkLocalAddrId;
      CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[i];

      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
      LinkLocalAddrId = TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV6.localAddrLinkLocal;

      /* check if link local address was assigned */
      if((IpAddrId ==  LinkLocalAddrId) &&
         (State == TCPIP_IPADDR_STATE_ASSIGNED)
        )
      {
        /* mark assignment of link local address */
        TcpIp_DhcpV6_DataPtr->flags |= TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED;
        if(TcpIp_DhcpV6_DataPtr->state == TCPIP_DHCPV6_STATE_START_DELAY)
        {
          /* transition to init if dhcp assignment was already started */
          TcpIp_DhcpV6_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(dhcpLocalAddr, (uint8)i);
        }
        else if
        (
          (TcpIp_DhcpV6_DataPtr->state != TCPIP_DHCPV6_STATE_INACTIVE) &&
          (TcpIp_DhcpV6_DataPtr->socketId == TCPIP_SOCKETID_INVALID)
        )
        {
          /* if link local was unassigned when dhcp was active, just update the socket when
             assigning link local address */
          /* don't update socket for transferring from ONHOLD to ASSIGNED because it already
             exists */
          (void)TcpIp_DhcpV6_entry_ACTIVE(dhcpLocalAddr, (uint8)i);
        }
        else
        {
          /* mark assignment of link local if dhcp assignment is still inactive */
        }
      }
      else if ((IpAddrId ==  LinkLocalAddrId) &&
               (State == TCPIP_IPADDR_STATE_UNASSIGNED)
              )
      {
        /* stop the dhcp assignment if link local address was unassigned */
        TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED);
        if(TcpIp_DhcpV6_DataPtr->socketId != TCPIP_SOCKETID_INVALID)
        {
          TcpIp_DhcpV6_exit_ACTIVE((uint8)i);
          TcpIp_DhcpV6_DataPtr->socketId = TCPIP_SOCKETID_INVALID;
        }
      }
      else
      {
        /* do nothing */
      }
    }
  }

  DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_EXIT(IpAddrId,State);
}

FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_CopyTxData
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
)
{
  uint8_least i;

  DBG_TCPIP_DHCPV6_COPYTXDATA_ENTRY(SocketId,BufPtr,BufLength);

  TS_PARAM_UNUSED(BufLength);

  for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv6Num; i++)
  {
    CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[i];
    if(TcpIp_DhcpV6_DataPtr->socketId == SocketId)
    {
      const TcpIp_LocalAddrIdType localAddr =
                  TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV6);

      TCPIP_PRECONDITION_ASSERT
        (TcpIp_DhcpV6_getMsgLength(TcpIp_DhcpV6_DataPtr->msg_type,(uint8)i) == BufLength, TCPIP_INTERNAL_API_ID);

      TcpIp_DhcpV6_buildHeader(localAddr,BufPtr, (uint8)i, TcpIp_DhcpV6_DataPtr->msg_type);
    }
  }

  DBG_TCPIP_DHCPV6_COPYTXDATA_EXIT(BUFREQ_OK,SocketId,BufPtr,BufLength);

  return BUFREQ_OK;
}

FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_rxIndication
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) rxBufPtr = BufPtr;
  uint8 msg_type = 0U;

  DBG_TCPIP_DHCPV6_RXINDICATION_ENTRY(SocketId,RemoteAddrPtr,BufPtr,Length);

  TS_PARAM_UNUSED(RemoteAddrPtr);

  /* check if message type option is present */
  if( (Length > TCPIP_DHCPV6_HEADER_LEN) &&
      (TCPIP_DHCPV6_GET_MSG_TYPE(rxBufPtr) != TCPIP_DHCPV6_INVALID_MSG_TYPE))
  {
    TcpIp_DhcpV6_OptionType options;
    uint8 i;
    msg_type = TCPIP_DHCPV6_GET_MSG_TYPE(rxBufPtr);

    TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr->dhcpv6Num <= TCPIP_NUM_DHCPV6_CLIENTS, 0U);
    for(i = 0U; i < TcpIp_PBcfgPtr->dhcpv6Num; i++)
    {
      CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[i];
      if(TcpIp_DhcpV6_DataPtr->socketId == SocketId)
      {
        if(TCPIP_DHCPV6_EVALUATE_XID(TcpIp_DhcpV6_DataPtr->xid, &rxBufPtr[1U]) == E_OK)
        {
          /* offset by DHCPv6 msg type and XID */
          Length = Length - TCPIP_DHCPV6_OPTION_HEADER_LEN;

          /* read options */
          retVal = TcpIp_DhcpV6_readOpts(i,&rxBufPtr[TCPIP_DHCPV6_OPTION_HEADER_LEN], Length, &options);

          if(retVal == E_OK)
          {
            const TcpIp_LocalAddrIdType localAddr =
                    TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV6);

            TcpIp_DhcpV6_received(localAddr, i, &options, msg_type);
            break;
          }
        }
      }
    }
  }

  DBG_TCPIP_DHCPV6_RXINDICATION_EXIT(SocketId,RemoteAddrPtr,BufPtr,Length);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_init
(
  TcpIp_LocalAddrIdType LocalIpAddrId
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(LocalIpAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
   CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
  uint8 i;
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */
  TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);

  DBG_TCPIP_DHCPV6_INIT_ENTRY(LocalIpAddrId);

  TcpIp_DhcpV6_DataPtr->IaId = TcpIp_GetRandomValue();
  TcpIp_DhcpV6_DataPtr->T1_time = 0U;
  TcpIp_DhcpV6_DataPtr->T2_time = 0U;
  TCPIP_IPV6_SET_UNSPECIFIED_IP(TcpIp_DhcpV6_DataPtr->ip_addr);
  TS_MemSet(TcpIp_DhcpV6_DataPtr->server_id, 0U, TCPIP_DHCPV6_SERVER_DUID_SIZE);
  TcpIp_DhcpV6_DataPtr->server_id_len = 0U;
  TcpIp_DhcpV6_DataPtr->valid_time = 0U;
  TcpIp_DhcpV6_DataPtr->preferred_time = 0U;
  TcpIp_DhcpV6_DataPtr->rtx_timeout = 0U;
  TcpIp_DhcpV6_DataPtr->rtx_timer = 0U;
  TcpIp_DhcpV6_DataPtr->elapsed_time = TCPIP_DHCPV6_ELAPSED_TIMER_STOP;
  TcpIp_DhcpV6_DataPtr->socketId = TCPIP_SOCKETID_INVALID;
  TcpIp_DhcpV6_DataPtr->state = TCPIP_DHCPV6_STATE_INACTIVE;
  TcpIp_DhcpV6_DataPtr->timer = 0U;
  TCPIP_DHCPV6_SET_UNUSED_XID(TcpIp_DhcpV6_DataPtr->xid);
  TcpIp_DhcpV6_DataPtr->flags = 0U;
  TcpIp_DhcpV6_DataPtr->num_retry = 0U;
  TcpIp_DhcpV6_DataPtr->req_dec_num_retry = 0U;
  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_INVALID;
#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)
  TcpIp_DhcpV6_DataPtr->domain_name_len = 0U;
#endif /* TCPIP_DHCPV6_FQDN_ENABLED == STD_ON */
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
  for (i = 0U;i<TcpIp_PBcfgPtr->dhcpv6Config.optionNum; i++)
  {
    TcpIp_DhcpV6_DataPtr->option_length[i] = 0U;
  }
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */

  DBG_TCPIP_DHCPV6_INIT_EXIT(LocalIpAddrId);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_startAssign
(
  TcpIp_LocalAddrIdType LocalAddr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(LocalAddr, TCPIP_IPADDRM_ASSIGN_DHCPV6);
  uint8 state;

  DBG_TCPIP_DHCPV6_STARTASSIGN_ENTRY(LocalAddr);

  TS_AtomicAssign8(state, TcpIp_DhcpV6_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_INACTIVE:
      /* open DHCP socket and delay transmission of SOLICIT */
      TcpIp_DhcpV6_tr_startAssign_INACTIVE_START_DELAY(LocalAddr, dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_START_DELAY:     /* intended fall through */
    case TCPIP_DHCPV6_STATE_INIT:            /* intended fall through */
    case TCPIP_DHCPV6_STATE_SOLICIT:         /* intended fall through */
    case TCPIP_DHCPV6_STATE_REQUESTING:      /* intended fall through */
    case TCPIP_DHCPV6_STATE_BOUND:           /* intended fall through */
    case TCPIP_DHCPV6_STATE_RENEWING:        /* intended fall through */
    case TCPIP_DHCPV6_STATE_REBINDING:       /* intended fall through */
    case TCPIP_DHCPV6_STATE_REASSIGNING:     /* intended fall through */
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:      /* intended fall through */
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:       /* intended fall through */
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_CONFLICTED:      /* intended fall through */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
      /* do nothing */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_DHCPV6_STARTASSIGN_EXIT(LocalAddr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_stopAssign
(
  TcpIp_LocalAddrIdType LocalAddr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(LocalAddr, TCPIP_IPADDRM_ASSIGN_DHCPV6);
  uint8 state;

  DBG_TCPIP_DHCPV6_STOPASSIGN_ENTRY(LocalAddr);

  TS_AtomicAssign8(state, TcpIp_DhcpV6_Data[dhcpIdx].state);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_START_DELAY:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_START_DELAY_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_INIT:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_SOLICIT:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_SOLICIT_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_REQUESTING:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_REQUESTING_INACTIVE(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_BOUND:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE(dhcpIdx, LocalAddr);
      break;
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_OPTIMISTIC_INACTIVE(dhcpIdx, LocalAddr);
      break;
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_TENTATIVE_INACTIVE(dhcpIdx, LocalAddr);
      break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_CONFLICTED:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_CONFLICTED_INACTIVE(dhcpIdx);
      break;
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_RENEWING:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_RENEWING_INACTIVE(dhcpIdx, LocalAddr);
      break;
    case TCPIP_DHCPV6_STATE_REBINDING:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_REBINDING_INACTIVE(dhcpIdx, LocalAddr);
      break;
    case TCPIP_DHCPV6_STATE_REASSIGNING:
      /* close DHCP socket */
      TcpIp_DhcpV6_tr_stopAssign_REASSIGNING_INACTIVE(dhcpIdx, LocalAddr);
      break;
    case TCPIP_DHCPV6_STATE_INACTIVE:  /* intended fall through */
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_DHCPV6_STOPASSIGN_EXIT(LocalAddr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_mainFunction
(
  uint16 timerTick
)
{
  uint8_least i;
  boolean secondElapsed = FALSE;

  DBG_TCPIP_DHCPV6_MAINFUNCTION_ENTRY(timerTick);

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_DHCPV6_SOL_DELAY)
  {
    TcpIp_DhcpV6_TimeoutCounter++;

    if(TcpIp_DhcpV6_TimeoutCounter >= 10U)
    {
       secondElapsed = TRUE;
       TcpIp_DhcpV6_TimeoutCounter = 0U;
    }
  }

  /* cycle through all dhcp interface in our list */
  for (i = 0U; i < TcpIp_PBcfgPtr->dhcpv6Num; ++i)
  {
    boolean retransmit = FALSE;
    boolean timeout = FALSE;
    const TcpIp_LocalAddrIdType localAddr =
            TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_DHCPV6);
    CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[i];


    /* check for retries */
    if(TCPIP_DHCPV6_FLAG_NXT_MAIN_IS_SET(TcpIp_DhcpV6_DataPtr->flags))
    {
      if(TcpIp_DhcpV6_DataPtr->num_retry < TcpIp_PBcfgPtr->transRetryNum)
      {
        /* try to retransmit dhcp message until max number is reached */
        TcpIp_DhcpV6_DataPtr->num_retry++;
        retransmit = TRUE;
      }
      else
      {
        /* stop retransmitting, retry when next retransmission or
           state timeout occurs */
        TcpIp_DhcpV6_DataPtr->num_retry = 0U;
        TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_NXT_MAIN);
      }
    }

    if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_DHCPV6_SOL_DELAY)
    {

      if((TcpIp_DhcpV6_DataPtr->state == TCPIP_DHCPV6_STATE_INIT) ||
         (secondElapsed == TRUE)
        )
      {
        /* verify if state timeout occurred */
        if(TcpIp_DhcpV6_DataPtr->timer > 0U)
        {
          TcpIp_DhcpV6_DataPtr->timer--;

          if(TcpIp_DhcpV6_DataPtr->timer == 0U)
          {
            timeout = TRUE;
          }
        }
      }

      /* verify if message shall be retransmitted */
      if(TcpIp_DhcpV6_DataPtr->rtx_timer > 0U)
      {
        TcpIp_DhcpV6_DataPtr->rtx_timer--;

        if(TcpIp_DhcpV6_DataPtr->rtx_timer == 0U)
        {
          /* timer expired, retransmit */
          retransmit = TRUE;
        }
      }

      if(TcpIp_DhcpV6_DataPtr->elapsed_time != TCPIP_DHCPV6_ELAPSED_TIMER_STOP)
      {
        if(TcpIp_DhcpV6_DataPtr->elapsed_time <= (0xffffU - TCPIP_DHCPV6_100MS_FACTOR))
        {
          /* increment seconds counter */
          TcpIp_DhcpV6_DataPtr->elapsed_time += TCPIP_DHCPV6_100MS_FACTOR;
        }
        else
        {
          TcpIp_DhcpV6_DataPtr->elapsed_time = TCPIP_DHCPV6_ELAPSED_TIMER_STOP;
        }
      }
    }

    if(timeout)
    {
      /* timer expired, retransmit */
      TcpIp_DhcpV6_timeout(localAddr, (uint8) i);
    }
    else if(retransmit)
    {
      /* try to transmit DHCP message again */
      TcpIp_DhcpV6_retransTimeout(localAddr, (uint8) i);
    }
    else
    {
      /* nothing to do here */
    }
  }

  DBG_TCPIP_DHCPV6_MAINFUNCTION_EXIT(timerTick);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  const uint8 dhcpIdx =
      TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
  const uint8 state = TcpIp_DhcpV6_Data[dhcpIdx].state;
  TcpIp_IpAddrConstPtr netMaskPtr = TcpIp_IPV6_Prefix_Mask;

  DBG_TCPIP_DHCPV6_GETIPCONFIG_ENTRY(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_BOUND:
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_RENEWING:
    case TCPIP_DHCPV6_STATE_REBINDING:
    case TCPIP_DHCPV6_STATE_REASSIGNING:
      *assignedIpPtrPtr = TcpIp_DhcpV6_Data[dhcpIdx].ip_addr;
#if(TCPIP_NDP_RPD_ENABLED == STD_ON)
      {
        uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
        TcpIp_Ndp_Rpd_getDefaultRouterAddress(gatewayIpPtrPtr,ctrlIdx);
      }
#else
      *gatewayIpPtrPtr = TcpIp_DhcpV6_unspecified;
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
      *netmaskIpPtrPtr = netMaskPtr;
      break;
    case TCPIP_DHCPV6_STATE_INACTIVE:
    case TCPIP_DHCPV6_STATE_START_DELAY:
    case TCPIP_DHCPV6_STATE_INIT:
    case TCPIP_DHCPV6_STATE_SOLICIT:
    case TCPIP_DHCPV6_STATE_REQUESTING:
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)
    case TCPIP_DHCPV6_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
    case TCPIP_DHCPV6_STATE_CONFLICTED:
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
      *assignedIpPtrPtr = TcpIp_DhcpV6_unspecified;
      *gatewayIpPtrPtr = TcpIp_DhcpV6_unspecified;
      *netmaskIpPtrPtr = TcpIp_DhcpV6_unspecified;
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_DHCPV6_GETIPCONFIG_EXIT(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
  const uint8 state = TcpIp_DhcpV6_Data[dhcpIdx].state;

  DBG_TCPIP_DHCPV6_GETIPADDRESS_ENTRY(localAddrId,assignedIpPtrPtr);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_BOUND:
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_RENEWING:
    case TCPIP_DHCPV6_STATE_REBINDING:
    case TCPIP_DHCPV6_STATE_REASSIGNING:
      *assignedIpPtrPtr = TcpIp_DhcpV6_Data[dhcpIdx].ip_addr;
      break;
    case TCPIP_DHCPV6_STATE_INACTIVE:
    case TCPIP_DHCPV6_STATE_START_DELAY:
    case TCPIP_DHCPV6_STATE_INIT:
    case TCPIP_DHCPV6_STATE_SOLICIT:
    case TCPIP_DHCPV6_STATE_REQUESTING:
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)
    case TCPIP_DHCPV6_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
    case TCPIP_DHCPV6_STATE_CONFLICTED:
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
      *assignedIpPtrPtr = TcpIp_DhcpV6_unspecified;
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

   DBG_TCPIP_DHCPV6_GETIPADDRESS_EXIT(localAddrId,assignedIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
)
{
  DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);

  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(state);

  DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_ENTRY(localAddrId,LocalIpAddrPtr);

  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(LocalIpAddrPtr);

  DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_EXIT(E_OK,localAddrId,LocalIpAddrPtr);

  return E_OK;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV6_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 dhcpIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);

  DBG_TCPIP_DHCPV6_GETIPADDRSTATE_ENTRY(localAddrId);

  DBG_TCPIP_DHCPV6_GETIPADDRSTATE_EXIT(TcpIp_DhcpV6_Data[dhcpIdx].expiry_state, localAddrId);

  return TcpIp_DhcpV6_Data[dhcpIdx].expiry_state;
}

/*==================[internal function definitions]=========================*/

#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setOptionFQDN
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
)
{

  const uint16 TotalOptionLength = TcpIp_DhcpV6_getOptionFQDNLength(dhcpIdx);

  if (TotalOptionLength != 0U)
  {
    const uint16 OptionLength = TotalOptionLength - TCPIP_DHCPV6_HEADER_LEN;

    TCPIP_DHCPV6_SET_FQDN_OPTION(optptr, OptionLength);

    TCPIP_DHCPV6_SET_OPT_FQDN_DOMAIN_NAME
                                    (
                                     optptr,
                                     TcpIp_DhcpV6_Data[dhcpIdx].domain_name,
                                     TcpIp_DhcpV6_Data[dhcpIdx].domain_name_len
                                    );

    /* set terminator */
    optptr[TotalOptionLength - 1U] = TCPIP_DHCPV6_DOMAINNAME_TERMINATOR;
  }
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getOptionFQDNLength
(
  uint8 dhcpIdx
)
{
  uint16 len = 0U;

  if(TcpIp_DhcpV6_Data[dhcpIdx].domain_name_len != 0U)
  {
    len = TCPIP_DHCPV6_HEADER_LEN +
          TCPIP_DHCPV6_FQDN_FLAGS_SIZE +
          TCPIP_DHCPV6_TERMINATOR_LENGTH +
          (uint16)TcpIp_DhcpV6_Data[dhcpIdx].domain_name_len;
  }
  return len;
}

#endif /* TCPIP_DHCPV6_FQDN_ENABLED == STD_ON */

#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optptr
)
{
  uint8 i = 0U;
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];
  TCPIP_PRECONDITION_ASSERT( dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, 0U);

  for(i=0U; i<TcpIp_PBcfgPtr->dhcpv6Config.optionNum; i++)
  {
    if (TcpIp_DhcpV6_DataPtr->option_length[i] != 0U)
    {
      CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOption =
        &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpV6Option,i));

      if(dhcpOption->optionTransmit)
      {
        TCPIP_DHCPV6_SET_OPTION_TYP(optptr, dhcpOption->optionCode);
        /* length of the option */
        TCPIP_DHCPV6_SET_OPTION_LEN(optptr, TcpIp_DhcpV6_DataPtr->option_length[i]);
        TS_MemCpy( &(optptr)[TCPIP_DHCPV6_HEADER_LEN],
                   &TcpIp_DhcpV6_DataPtr->option_data[dhcpOption->optionDataOffset],
                   TcpIp_DhcpV6_DataPtr->option_length[i]
                 );
        optptr = &optptr[TCPIP_DHCPV6_HEADER_LEN + TcpIp_DhcpV6_DataPtr->option_length[i]];
      }
    }
  }
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getOptionLength
(
  uint8 dhcpIdx
)
{
  uint16 len = 0U;
  uint8 i = 0U;

  for(i=0U; i<TcpIp_PBcfgPtr->dhcpv6Config.optionNum; i++)
  {
    CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOption =
      &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpV6Option,i));
    if ((TcpIp_DhcpV6_Data[dhcpIdx].option_length[i] != 0U) && (dhcpOption->optionTransmit))
    {
      len += TCPIP_DHCPV6_HEADER_LEN + TcpIp_DhcpV6_Data[dhcpIdx].option_length[i];
    }
  }
  return len;
}

#endif /* TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON */


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_retransTimeout
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  uint8 state;
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TS_AtomicAssign8(state, TcpIp_DhcpV6_DataPtr->state);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_SOLICIT:
      TcpIp_DhcpV6_tr_retransTimeout_SOLICIT_SOLICIT(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_REQUESTING:
      if(TcpIp_DhcpV6_DataPtr->req_dec_num_retry < TCPIP_DHCPV6_REQ_MAX_RC)
      {
        TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_REQUESTING(dhcpIdx);
        TcpIp_DhcpV6_DataPtr->req_dec_num_retry++;
      }
      else
      {
        TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_INIT(dhcpIdx);
      }
      break;
    case TCPIP_DHCPV6_STATE_REASSIGNING:
      if(TcpIp_DhcpV6_DataPtr->req_dec_num_retry < TCPIP_DHCPV6_REQ_MAX_RC)
      {
        TcpIp_DhcpV6_tr_retransTimeout_REASSIGNING_REASSIGNING(dhcpIdx);
        TcpIp_DhcpV6_DataPtr->req_dec_num_retry++;
      }
      else
      {
        TcpIp_DhcpV6_tr_retransTimeout_REASSIGNING_INIT(localAddr, dhcpIdx);
      }
      break;
    case TCPIP_DHCPV6_STATE_RENEWING:
      TcpIp_DhcpV6_tr_retransTimeout_RENEWING_RENEWING(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_REBINDING:
      TcpIp_DhcpV6_tr_retransTimeout_REBINDING_REBINDING(dhcpIdx);
      break;
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_CONFLICTED:
      if(TcpIp_DhcpV6_DataPtr->req_dec_num_retry < TCPIP_DHCPV6_DEC_MAX_RC)
      {
        TcpIp_DhcpV6_tr_retransTimeout_CONFLICTED_CONFLICTED(dhcpIdx);
        TcpIp_DhcpV6_DataPtr->req_dec_num_retry++;
      }
      else
      {
        TcpIp_DhcpV6_tr_retransTimeout_CONFLICTED_INIT(dhcpIdx);
      }
      break;
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV6_STATE_INACTIVE:    /* intended fall through */
    case TCPIP_DHCPV6_STATE_START_DELAY: /* intended fall through */
    case TCPIP_DHCPV6_STATE_INIT:        /* intended fall through */
    case TCPIP_DHCPV6_STATE_BOUND:       /* intended fall through */
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:  /* intended fall through */
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:   /* intended fall through */
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_timeout
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  uint8 state;
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TS_AtomicAssign8(state, TcpIp_DhcpV6_DataPtr->state);

  switch(state)
  {
    case TCPIP_DHCPV6_STATE_INIT:
      TcpIp_DhcpV6_tr_timeout_INIT_SOLICIT(dhcpIdx);
      break;
    case TCPIP_DHCPV6_STATE_BOUND:
      TcpIp_DhcpV6_tr_timeout_BOUND_RENEWING
        (dhcpIdx,TcpIp_DhcpV6_DataPtr->T2_time - TcpIp_DhcpV6_DataPtr->T1_time);
      break;
    case TCPIP_DHCPV6_STATE_RENEWING:
      if((TcpIp_DhcpV6_DataPtr->valid_time - TcpIp_DhcpV6_DataPtr->T2_time) != 0U)
      {
        TcpIp_DhcpV6_tr_timeout_RENEWING_REBINDING
          (dhcpIdx,TcpIp_DhcpV6_DataPtr->preferred_time - TcpIp_DhcpV6_DataPtr->T2_time);
      }
      else
      {
        TcpIp_DhcpV6_tr_timeout_RENEWING_INIT(localAddr, dhcpIdx);
      }
      break;
    case TCPIP_DHCPV6_STATE_REBINDING:
      if(((TcpIp_DhcpV6_DataPtr->valid_time - TcpIp_DhcpV6_DataPtr->preferred_time) != 0) &&
         (TcpIp_DhcpV6_DataPtr->expiry_state == TCPIP_IPADDRM_ADDRSTATE_PREFERRED))
      {
        TcpIp_DhcpV6_tr_timeout_REBINDING_REBINDING(dhcpIdx);
      }
      else
      {
        TcpIp_DhcpV6_tr_timeout_REBINDING_INIT(localAddr, dhcpIdx);
      }
      break;
    /* CHECK: NOPARSE */
    case TCPIP_DHCPV6_STATE_REQUESTING:
    case TCPIP_DHCPV6_STATE_REASSIGNING:
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_CONFLICTED:
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_INACTIVE:     /* intended fall through */
    case TCPIP_DHCPV6_STATE_SOLICIT:      /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
)
{
  uint16 idx = 0U;
  Std_ReturnType ret;
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(dhcpIdx);
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_OFF) */

  /* parsing loop */
  do
  {
    uint16 leftLen = optlen - idx;
    ret = E_NOT_OK;

    if(leftLen >= TCPIP_DHCPV6_OPTION_HEADER_LEN)
    {
      uint16 len = TCPIP_DHCPV6_GET_OPTION_SIZE(optPtr);
      if(len <= (leftLen - TCPIP_DHCPV6_OPTION_HEADER_LEN))
      {
        uint16 optionId = TCPIP_DHCPV6_GET_OPTION_ID(optPtr);
        switch(optionId)
        {
          case TCPIP_DHCPV6_OPTION_CLIENTID:
            if((len == TCPIP_DHCPV6_CLIENT_ID_OPTION_LEN) &&
               (TCPIP_DHCPV6_GET_DUID_TYPE(&optPtr[TCPIP_DHCPV6_HEADER_LEN]) ==
                TCPIP_DHCPV6_DUID_LL_TYPE) &&
               (TCPIP_DHCPV6_GET_HWTYPE_TYPE(&optPtr[TCPIP_DHCPV6_HEADER_LEN]) ==
                TCPIP_DHCPV6_HWTYPE_ETHERNET))
            {
              opts->client_id = &optPtr[TCPIP_DHCPV6_CLIENT_HEADER_LEN];
              ret = E_OK;
            }
            break;

          case TCPIP_DHCPV6_OPTION_SERVERID:
            if ((len <= TCPIP_DHCPV6_SERVER_DUID_SIZE) &&(len >= TCPIP_DHCPV6_MIN_SERVER_DUID_LEN))
            {
              opts->server_id_len = (uint8) len;
              opts->server_id = &optPtr[TCPIP_DHCPV6_HEADER_LEN];
              ret = E_OK;
            }
            break;

          case TCPIP_DHCPV6_OPTION_IA_NA:
            if (len > TCPIP_DHCPV6_IA_NA_LEN)
            {
              TcpIp_DhcpV6_parseIaNaHeader(optPtr, opts);
              {
                uint16 IaNaOptLen = len - TCPIP_DHCPV6_IA_NA_LEN;

                /* parse IANA options*/
                ret = TcpIp_DhcpV6_parseIaNaOptions
                (
                  &optPtr[TCPIP_DHCPV6_IA_NA_LEN + TCPIP_DHCPV6_OPTION_HEADER_LEN],
                  IaNaOptLen,
                  opts
                );
              }
            }
            break;

          case TCPIP_DHCPV6_OPTION_STATUS_CODE:
            if (len >= TCPIP_DHCPV6_OPTION_STATUS_CODE_LEN)
            {
              opts->status_code = TCPIP_DHCPV6_GET_STATUS_CODE (optPtr);
              if(opts->status_code <= 4U)
              {
                ret = E_OK;
              }
            }
            break;

          case TCPIP_DHCPV6_OPTION_ORO:
            break;

          default:
            /* skip and do nothing */
            ret = E_OK;
            break;
        }
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
        {
          CONSTP2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionPtr =
              &(TCPIP_CFG_TYPE(TcpIp_DhcpOptionConfigType,dhcpV6Option,0));

          TCPIP_PRECONDITION_ASSERT(dhcpIdx < TCPIP_NUM_DHCPV6_CLIENTS, TCPIP_INTERNAL_API_ID);

            (void)TcpIp_Dhcp_writeOptionRxIndication
              (
                optionId,
                len,
                &optPtr[TCPIP_DHCPV6_OPTION_HEADER_LEN],
                TcpIp_DhcpV6_Data[dhcpIdx].option_data,
                TcpIp_DhcpV6_Data[dhcpIdx].option_length,
                dhcpOptionPtr,
                TcpIp_PBcfgPtr->dhcpv6Config.optionNum
              );
        }
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */

        /* resume with next option */
        idx += len + TCPIP_DHCPV6_OPTION_HEADER_LEN;
        optPtr = &optPtr[len + TCPIP_DHCPV6_OPTION_HEADER_LEN];
      }
    }
  } while ((idx < optlen) && (ret == E_OK));

  return ret;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_parseIaNaHeader
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
)
{
  if ((opts->valid_time == 0U) ||
      (opts->preferred_time == 0U) ||
      (opts->valid_time < opts->preferred_time) ||
      (opts->preferred_time < opts->T2_time) ||
      (opts->T2_time < opts->T1_time)
     )
  {
    opts->IaId = TCPIP_DHCPV6_GET_IAID(optPtr);
    opts->T1_time = TCPIP_DHCPV6_GET_T1_TIME(optPtr);
    opts->T2_time = TCPIP_DHCPV6_GET_T2_TIME(optPtr);
  }
}


STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseIaNaOptions
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA)  opts
)
{
  uint16 idx = 0U;
  Std_ReturnType ret;

  /* parsing loop */
  do
  {
    uint16 leftLen = optlen - idx;
    ret = E_NOT_OK;

    if(leftLen >= TCPIP_DHCPV6_OPTION_HEADER_LEN)
    {
      uint16 len = TCPIP_DHCPV6_GET_OPTION_SIZE(optPtr);
      if(len <= (leftLen - TCPIP_DHCPV6_OPTION_HEADER_LEN))
      {
        switch(TCPIP_DHCPV6_GET_OPTION_ID(optPtr))
        {
          case TCPIP_DHCPV6_OPTION_IAADDR:
            if (len >= (TCPIP_DHCPV6_IAADDR_OPTION_LEN))
            {
              /* copy options*/
              opts->preferred_time = TCPIP_DHCPV6_GET_PREFERRED_TIME(optPtr);
              opts->valid_time = TCPIP_DHCPV6_GET_VALID_TIME(optPtr);

              /*Get the IPv6 address advertised by server */
              opts->ip_req = TCPIP_DHCPV6_GET_IPV6_ADDR_PTR(optPtr);
              ret = E_OK;

              if (len > (TCPIP_DHCPV6_IAADDR_OPTION_LEN))
              {
                uint16 IaaddrOptLen = len - TCPIP_DHCPV6_IAADDR_OPTION_LEN;

                /* parse IANA options*/
                ret = TcpIp_DhcpV6_parseIaAddrOption
                (
                  &optPtr[TCPIP_DHCPV6_IAADDR_OPTION_LEN + TCPIP_DHCPV6_OPTION_HEADER_LEN],
                  IaaddrOptLen,
                  opts
                );
              }
            }
          break;

        case TCPIP_DHCPV6_OPTION_STATUS_CODE:
            if (len >= TCPIP_DHCPV6_OPTION_STATUS_CODE_LEN)
            {
              opts->status_code_IA_NA = TCPIP_DHCPV6_GET_STATUS_CODE(optPtr);
              if((opts->status_code_IA_NA) <= 4U)
              {
                ret = E_OK;
              }
            }
          break;

        default:
          /* skip and do nothing */
          ret = E_OK;
          break;
        }

        /* resume with next option */
        idx += len + TCPIP_DHCPV6_OPTION_HEADER_LEN;
        optPtr = &optPtr[len + TCPIP_DHCPV6_OPTION_HEADER_LEN];
      }
    }
  } while ((idx < optlen) && (ret == E_OK));

  return ret;
}


STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_parseIaAddrOption
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr,
  uint16 optlen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  uint16 idx = 0U;
  Std_ReturnType ret;

  /* parsing loop */
  do
  {
    uint16 leftLen = optlen - idx;
    ret = E_NOT_OK;

    if(leftLen >= TCPIP_DHCPV6_OPTION_HEADER_LEN)
    {
      uint16 len = TCPIP_DHCPV6_GET_OPTION_SIZE(optPtr);
      if(len <= (leftLen - TCPIP_DHCPV6_OPTION_HEADER_LEN))
      {
        switch(TCPIP_DHCPV6_GET_OPTION_ID(optPtr))
        {
          case TCPIP_DHCPV6_OPTION_STATUS_CODE:
            if (len >= TCPIP_DHCPV6_OPTION_STATUS_CODE_LEN)
            {
              opts->status_code_IAADDR = TCPIP_DHCPV6_GET_STATUS_CODE(optPtr);
              if((opts->status_code_IAADDR) <= 4U)
              {
                ret = E_OK;
              }
            }
            break;

          default:
            /* skip and do nothing */
            ret = E_OK;
            break;
        }

        /* resume with next option */
        idx += len + TCPIP_DHCPV6_OPTION_HEADER_LEN;
        optPtr = &optPtr[len + TCPIP_DHCPV6_OPTION_HEADER_LEN];
      }
    }

  } while((idx < optlen) && (ret == E_OK));
  return ret;
}


STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_readOpts
(
  uint8 dhcpIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  Std_ReturnType result = E_NOT_OK;

  /* set options to default parameters */
  opts->server_id = NULL_PTR;
  opts->client_id = NULL_PTR;
  opts->server_id_len = 0U;
  opts->preferred_time = 0U;
  opts->valid_time = 0U;
  opts->status_code = 0U;
  opts->status_code_IA_NA = 0U;
  opts->status_code_IAADDR = 0U;
  opts->T1_time = 0U;
  opts->T2_time = 0U;

  /* parse options in 'option' field */
  result = TcpIp_DhcpV6_parseOpts(dhcpIdx,dataPtr, dataLen, opts);

  TcpIp_DhcpV6_evaluateStatusCode(opts);

  return result;
}


STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_transmit
(
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  TcpIp_ReturnType retVal = TCPIP_E_DROP;
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  if(TcpIp_DhcpV6_DataPtr->socketId != TCPIP_SOCKETID_INVALID)
  {
    {
      TcpIp_SockAddrInet6Type remoteAddr;
      P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_VAR) remoteAddrPtr =
        /* Deviation MISRAC2012-1 */
        (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_VAR)) &remoteAddr;

      const uint16 msglength = TcpIp_DhcpV6_getMsgLength( msg_type, dhcpIdx);

      TcpIp_DhcpV6_DataPtr->msg_type = msg_type;
      remoteAddr.domain = TCPIP_AF_INET6;
      TCPIP_DHCPV6_SET_ALL_DHCP_SERVER_MULTICAST(remoteAddr.addr);
      remoteAddr.port = TCPIP_DHCPV6_SERVER_PORT;

      /* trigger transmission of DHCP message */
      retVal = TcpIp_UdpTransmit(TcpIp_DhcpV6_DataPtr->socketId, NULL_PTR, remoteAddrPtr, msglength);

      if(retVal == TCPIP_OK)
      {
        /* stop further retransmission in next mainfunction */
        TcpIp_DhcpV6_DataPtr->num_retry = 0U;
        TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_NXT_MAIN);
      }
      else
      {
        /* no NDP entry or enough buffer retry in next mainfunction */
        TcpIp_DhcpV6_DataPtr->flags |= TCPIP_DHCPV6_FLAG_NXT_MAIN;
        retVal = TCPIP_E_MEMORY;
      }
    }
  }

  return retVal;
}


STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_getMsgLength
(
  uint8 msg_type,
  uint8 dhcpIdx
)
{
   uint16 length = 0U;

#if ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON))
#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)
      length += TcpIp_DhcpV6_getOptionFQDNLength(dhcpIdx);
#endif /* (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
      length += TcpIp_DhcpV6_getOptionLength(dhcpIdx);
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */
#else /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_OFF) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */
      TS_PARAM_UNUSED(dhcpIdx);
#endif /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) ) */

  switch(msg_type)
  {
    case TCPIP_DHCPV6_SOLICIT:
      length += TCPIP_DHCPV6_SOLICIT_FIXED_LENGTH;
      break;
    case TCPIP_DHCPV6_REQUEST:
    {
      length += TCPIP_DHCPV6_REQUEST_FIXED_LENGTH +
      (uint16)TcpIp_DhcpV6_Data[dhcpIdx].server_id_len + TCPIP_DHCPV6_DUID_HEADER_LEN;
      break;
    }
    case TCPIP_DHCPV6_RENEW:
      length += TCPIP_DHCPV6_RENEW_FIXED_LENGTH +
      (uint16)TcpIp_DhcpV6_Data[dhcpIdx].server_id_len + TCPIP_DHCPV6_DUID_HEADER_LEN;
      break;
    case TCPIP_DHCPV6_DECLINE:
      length += TCPIP_DHCPV6_DECLINE_FIXED_LENGTH +
      (uint16)TcpIp_DhcpV6_Data[dhcpIdx].server_id_len + TCPIP_DHCPV6_DUID_HEADER_LEN;
      break;
    case TCPIP_DHCPV6_REBIND:
      length += TCPIP_DHCPV6_REBIND_FIXED_LENGTH;
      break;
    /* CHECK: NOPARSE */
    /* Commented to prevent Polyspace from reporting excess unreachable code findings
    case TCPIP_DHCPV6_ADVERTISE:
    case TCPIP_DHCPV6_CONFIRM:
    case TCPIP_DHCPV6_REPLY:
    case TCPIP_DHCPV6_RELEASE:
    case TCPIP_DHCPV6_RECONFIGURE:
    case TCPIP_DHCPV6_INFORMATION_REQUEST:
    case TCPIP_DHCPV6_RELAY_FORW:
    case TCPIP_DHCPV6_RELAY_REPL:
    */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
  return length;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_buildHeader
(
  TcpIp_LocalAddrIdType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) buffPtr,
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  uint16 secs;

  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr = buffPtr;

  secs = TcpIp_DhcpV6_Data[dhcpIdx].elapsed_time;

  TCPIP_DHCPV6_SET_MSG_HEADER(buffPtr, msg_type, TcpIp_DhcpV6_Data[dhcpIdx].xid);
  opthdr = &opthdr[TCPIP_DHCPV6_HEADER_LEN];

#if (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)
      {
        /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV6.EA2.mainFunction,1 */
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

        /* append Full Qualified Domain Name option */
        TcpIp_DhcpV6_setOptionFQDN(dhcpIdx, opthdr);
        opthdr = &opthdr[TcpIp_DhcpV6_getOptionFQDNLength(dhcpIdx)];

        /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
        /* !LINKSTO TcpIp.Design.DhcpV6.EA2.mainFunction,1 */
        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
      }
#endif /* (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) */
#if (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)
      /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
      /* !LINKSTO TcpIp.Design.DhcpV6.EA2.mainFunction,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

      TcpIp_DhcpV6_setOption(dhcpIdx, opthdr);
      opthdr = &opthdr[TcpIp_DhcpV6_getOptionLength(dhcpIdx)];

      /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
      /* !LINKSTO TcpIp.Design.DhcpV6.EA2.mainFunction,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
#endif /* (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) */

  /* append elapsed time option */
  TCPIP_DHCPV6_SET_ELAPSED_TIME_OPTION(opthdr, secs);
  opthdr = &opthdr[TCPIP_DHCPV6_ELAPSED_TIME_LENGTH];

  /* append option request option */
  TCPIP_DHCPV6_SET_OPTION_ORO_TIME_OPTION(opthdr);
  opthdr = &opthdr[TCPIP_DHCPV6_OPTION_ORO_TIME_LENGTH];

  /* append client-id option */
  TcpIp_DhcpV6_setClientIdOption(localAddr, opthdr);
  opthdr = &opthdr[TCPIP_DHCPV6_OPTION_HEADER_LEN + TCPIP_DHCPV6_DUID_LL_LEN];

  /* set options which are message specific */
  TcpIp_DhcpV6_setMsgSpecificOptions(dhcpIdx, msg_type, opthdr);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setMsgSpecificOptions
(
  uint8 dhcpIdx,
  uint8 msg_type,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  switch(msg_type)
  {
    case TCPIP_DHCPV6_SOLICIT:
      /* append IA_NA option */
      TcpIp_DhcpV6_setIaNaOption(dhcpIdx, opthdr, FALSE);
      break;

    case TCPIP_DHCPV6_REQUEST:
    case TCPIP_DHCPV6_RENEW:
    case TCPIP_DHCPV6_DECLINE:
      /* append server-id option */
      /* Deviation MISRAC2012-1, MISRAC2012-5 */
      TCPIP_DHCPV6_SET_SERVER_ID_OPTION( opthdr,
                                         TcpIp_DhcpV6_DataPtr->server_id,
                                         TcpIp_DhcpV6_DataPtr->server_id_len
                                       );

      opthdr =
        &opthdr[(uint16)TcpIp_DhcpV6_DataPtr->server_id_len + TCPIP_DHCPV6_DUID_HEADER_LEN];

      /* append IA_NA option */
      TcpIp_DhcpV6_setIaNaOption(dhcpIdx, opthdr, TRUE);
      break;

    case TCPIP_DHCPV6_REBIND:
      TcpIp_DhcpV6_setIaNaOption(dhcpIdx, opthdr, TRUE);
      break;

    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setIaNaOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr,
  boolean iaadr_option
)
{
  uint16 option_len = TCPIP_DHCPV6_IA_NA_SOLICIT_OPTION_LEN;

  if(iaadr_option)
  {
    option_len = TCPIP_DHCPV6_IA_NA_OPTION_LEN;
    /* set IAADDR option */
    TcpIp_DhcpV6_setIaAddrOption(
                                 dhcpIdx,
                                 &opthdr[TCPIP_DHCPV6_IA_NA_SOLICIT_OPTION_LEN + TCPIP_DHCPV6_HEADER_LEN]
                                );
  }
  /* set IA NA option */
  TCPIP_DHCPV6_SET_OPTION_TYP((opthdr), TCPIP_DHCPV6_OPTION_IA_NA);
  TCPIP_DHCPV6_SET_OPTION_LEN((opthdr), option_len);
  TCPIP_DHCPV6_SET_IAID((opthdr), TcpIp_DhcpV6_Data[dhcpIdx].IaId);
  TCPIP_DHCPV6_SET_T1_TIME((opthdr), 0U);
  TCPIP_DHCPV6_SET_T2_TIME((opthdr), 0U);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setIaAddrOption
(
  uint8 dhcpIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
)
{
  TCPIP_DHCPV6_SET_IAADDR_OPTION_TYP((opthdr), TCPIP_DHCPV6_OPTION_IAADDR);
  TCPIP_DHCPV6_SET_IAADDR_OPTION_LEN((opthdr), TCPIP_DHCPV6_IAADDR_OPTION_LEN);
  TCPIP_DHCPV6_SET_ADDR((opthdr), (TcpIp_DhcpV6_Data[dhcpIdx].ip_addr));
  TCPIP_DHCPV6_SET_PREFERRED_TIME((opthdr), 0U);
  TCPIP_DHCPV6_SET_VALID_TIME((opthdr), 0U);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_setClientIdOption
(
  TcpIp_LocalAddrIdType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_VAR) opthdr
)
{
  uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddr);
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  TCPIP_DHCPV6_SET_OPTION_TYP((opthdr), TCPIP_DHCPV6_OPTION_CLIENTID);
  TCPIP_DHCPV6_SET_OPTION_LEN((opthdr), TCPIP_DHCPV6_CLIENT_ID_OPTION_LEN);
  TCPIP_DHCPV6_SET_DUID_TYP((opthdr), TCPIP_DHCPV6_DUID_LL_TYPE);
  TCPIP_DHCPV6_SET_DUID_LEN((opthdr), TCPIP_DHCPV6_HWTYPE_ETHERNET);

  /* set DUID (Link Layer address) */
  EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx).ethIfCtrlIdx),
                     (&(opthdr)[8U])
                   );
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateLifeTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  Std_ReturnType result = E_NOT_OK;

  if((opt->valid_time != 0U) && (opt->valid_time >= opt->preferred_time))
  {
    uint32 T1_time = 0U;
    uint32 T2_time = 0U;
    uint32 lifetime = 0U;

    if((opt->T1_time == 0U) || (opt->T2_time == 0U))
    {
      if(opt->valid_time != TCPIP_DHCPV6_INFINITE_TIME)
      {
        /* preferred time is finite */
        lifetime = (opt->preferred_time);

        /* derive T1 and T2 timeouts from the preferred time */
        T1_time = (lifetime >> 1U);
        T2_time = (lifetime / 5U) << 2U;

        result = E_OK;
      }
      else
      {
        /* infinite preferred time */
        lifetime = 0U;
        T1_time = 0U;
        T2_time = 0U;
        result = E_OK;
      }
    }
    /* when T1 and T2 times were received, use those values instead */
    else
    {
      lifetime = opt->preferred_time;
      /* when T1- or T2-option was received, use those values instead */
      T1_time = opt->T1_time;

      T2_time = opt->T2_time;

      /* check for valid timeout values */
      if((lifetime >= T2_time) && (T2_time > T1_time))
      {
        result = E_OK;
      }
    }

    opt->preferred_time = lifetime;
    opt->T1_time = T1_time;
    opt->T2_time = T2_time;

  }
  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_received
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
)
{
  uint8 state;

  TS_AtomicAssign8(state, TcpIp_DhcpV6_Data[dhcpIdx].state);

  if((TcpIp_DhcpV6_evaluateClientId(opt, localAddr) == E_OK))
  {
    switch(state)
    {
      case TCPIP_DHCPV6_STATE_SOLICIT:
        TcpIp_DhcpV6_evaluateSolicitResponse(dhcpIdx, opt, msg_type);
        break;

      case TCPIP_DHCPV6_STATE_REQUESTING:
        TcpIp_DhcpV6_evaluateRequestingResponse(dhcpIdx, opt, msg_type, localAddr);
        break;

      case TCPIP_DHCPV6_STATE_RENEWING:
        TcpIp_DhcpV6_evaluateRenewingResponse(dhcpIdx, opt, msg_type, localAddr);
        break;

      case TCPIP_DHCPV6_STATE_REBINDING:
        TcpIp_DhcpV6_evaluateRebindingResponse(dhcpIdx, opt, msg_type, localAddr);
        break;

      case TCPIP_DHCPV6_STATE_REASSIGNING:
        TcpIp_DhcpV6_evaluateReassigningResponse(dhcpIdx, opt, msg_type, localAddr);
        break;

#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
      case TCPIP_DHCPV6_STATE_CONFLICTED:
        TcpIp_DhcpV6_evaluateConflictedResponse(dhcpIdx, opt, msg_type);
        break;

#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
      case TCPIP_DHCPV6_STATE_BOUND:
      case TCPIP_DHCPV6_STATE_INIT:
        /* ignore messages */
        break;
      /* CHECK: NOPARSE */
      case TCPIP_DHCPV6_STATE_INACTIVE:       /* intended fall through */
      case TCPIP_DHCPV6_STATE_START_DELAY:    /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateSolicitResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
)
{
  if (msg_type == TCPIP_DHCPV6_ADVERTISE)
  {
    if((opt->valid_time != 0U) &&
       (opt->valid_time >= opt->preferred_time) &&
       (opt->status_code == TCPIP_DHCPV6_STATUS_CODE_SUCCESS) &&
       (opt->server_id_len >= TCPIP_DHCPV6_MIN_SERVER_DUID_LEN) &&
       (opt->IaId == TcpIp_DhcpV6_Data[dhcpIdx].IaId ) &&
       (!TCPIP_DHCPV6_REQ_IP_IS_MULTICAST(opt->ip_req))
      )
    {
      if(!TCPIP_DHCPV6_REQ_IP_IS_UNSPECIFIED(opt->ip_req))
      {
        TcpIp_DhcpV6_tr_received_SOLICIT_REQUESTING(dhcpIdx, opt);
      }
    }
  }
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateRequestingResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
)
{

  if(TcpIp_DhcpV6_evaluateReplyMsg(opt, dhcpIdx, msg_type) == E_OK)
  {
    if(TcpIp_DhcpV6_evaluateStatusAndTime(opt) == E_OK)
    {
      if ((TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime(opt, dhcpIdx, TRUE) == E_OK))
      {
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
        if
        (
          /* TcpIpNdpSlaacDadRetransmissionDelay * TcpIpNdpSlaacDadNumberOfTransmissions */
          (opt->T1_time > ((TCPIP_NDP_DAD_CONFIG_TIMEOUT * TCPIP_NDP_DAD_CONFIG_NUM_OF_SOL) / 10U)) ||
          (opt->valid_time == TCPIP_DHCPV6_INFINITE_TIME) /* infinite preferred time */
        )
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON) */
        {
#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF)
          TcpIp_DhcpV6_tr_received_REQUESTING_BOUND(localAddr, dhcpIdx, opt);
#elif (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
          TcpIp_DhcpV6_tr_received_REQUESTING_DAD(localAddr, dhcpIdx, opt);
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
          TcpIp_DhcpV6_tr_received_REQUESTING_DAD(localAddr, dhcpIdx, opt);
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */
        }
      }
    }
    else
    {
      TcpIp_DhcpV6_tr_received_REQUESTING_INIT(dhcpIdx);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateRenewingResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
)
{
  if(TcpIp_DhcpV6_evaluateReplyMsg(opt, dhcpIdx, msg_type) == E_OK)
  {
    if(TcpIp_DhcpV6_evaluateStatusAndTime(opt) == E_OK)
    {
      if((TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime(opt, dhcpIdx, TRUE) == E_OK))
      {
        TcpIp_DhcpV6_tr_received_EXPANDING_BOUND(dhcpIdx, opt);
      }
    }
    else if (opt->status_code == TCPIP_DHCPV6_STATUS_CODE_NOBINDING)
    {
      TcpIp_DhcpV6_tr_received_EXPANDING_REASSIGNING(dhcpIdx);
    }
    else
    {
      TcpIp_DhcpV6_tr_received_EXPANDING_INIT (localAddr, dhcpIdx);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateReassigningResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type,
  TcpIp_LocalAddrIdType localAddr
)
{
  if((TcpIp_DhcpV6_evaluateReplyMsg(opt, dhcpIdx, msg_type) == E_OK))
  {
    if(TcpIp_DhcpV6_evaluateStatusAndTime(opt) == E_OK)
    {
      if((TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime(opt, dhcpIdx, TRUE) == E_OK))
      {
        TcpIp_DhcpV6_tr_received_EXPANDING_BOUND(dhcpIdx, opt);
      }
    }
    else
    {
      TcpIp_DhcpV6_tr_received_EXPANDING_INIT(localAddr, dhcpIdx);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_startAssign_INACTIVE_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_START_DELAY(localAddr, dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  if(TcpIp_DhcpV6_entry_ACTIVE(localAddr, dhcpIdx) == TCPIP_OK)
  {
    TcpIp_DhcpV6_entry_INIT(dhcpIdx);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_START_DELAY_INACTIVE
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE
(
  uint8 dhcpIdx
)
{
  /* exit code */
  TcpIp_DhcpV6_exit_ACTIVE(dhcpIdx);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE
(
  uint8 dhcpIdx,
  TcpIp_LocalAddrIdType localAddr
)
{
  /* exit code */
  TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST(dhcpIdx);
  TcpIp_DhcpV6_exit_ASSIGNED(localAddr);
  TcpIp_DhcpV6_exit_ACTIVE(dhcpIdx);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_INACTIVE(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_INIT_SOLICIT
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* try to discover DHCP servers until some offers an IP */
  TcpIp_DhcpV6_DataPtr->timer = 0U;

  /* evaluate transaction id */
  TCPIP_DHCPV6_SET_XID(TcpIp_DhcpV6_DataPtr->xid, TcpIp_GetRandomValue());

  TcpIp_DhcpV6_DataPtr->elapsed_time = 0U;

  /* first retransmission */
  TcpIp_DhcpV6_DataPtr->rtx_timer =
    TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_SOL_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
  TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_SOLICIT);

  (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_SOLICIT);

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_BOUND_RENEWING
(
  uint8 dhcpIdx,
  uint32 renewalTimeout
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* transitional code */
  uint32 randXid = TcpIp_GetRandomValue();

  TCPIP_DHCPV6_SET_XID(TcpIp_DhcpV6_DataPtr->xid, randXid);

  /* acquisition process restarts */
  TcpIp_DhcpV6_DataPtr->elapsed_time = 0U;

  /* start timer for T2-timeout */
  TcpIp_DhcpV6_DataPtr->timer = renewalTimeout;

  /* first retransmission */

  TcpIp_DhcpV6_DataPtr->rtx_timer =
    TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_REN_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
  TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_RENEWING);

  (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_RENEW);

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_RENEWING_REBINDING
(
  uint8 dhcpIdx,
  uint32 leaseTimeout
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* transitional code */
  uint32 randXid = TcpIp_GetRandomValue();

  TCPIP_DHCPV6_SET_XID(TcpIp_DhcpV6_DataPtr->xid, randXid);

  if(leaseTimeout > 0U)
  {
    /* start timer for detecting lease expiration */
    TcpIp_DhcpV6_DataPtr->timer = leaseTimeout;
  }
  else
  {
    TcpIp_DhcpV6_DataPtr->timer =
      TcpIp_DhcpV6_DataPtr->valid_time - TcpIp_DhcpV6_DataPtr->preferred_time;
    TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_DEPRECATED;
  }

  /* first retransmission */
  TcpIp_DhcpV6_DataPtr->rtx_timer =
    TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_REB_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
  TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_REBINDING);

  (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_REBIND);

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_timeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* transitional code */
  {
    TcpIp_DhcpV6_DataPtr->timer =
      TcpIp_DhcpV6_DataPtr->valid_time - TcpIp_DhcpV6_DataPtr->preferred_time;
    TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_DEPRECATED;
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_SOLICIT_SOLICIT
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV6_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV6_SOLICIT);
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_REQUESTING_REQUESTING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV6_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV6_REQUEST);
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_RENEWING_RENEWING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV6_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV6_RENEW);
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_REBINDING_REBINDING
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_DhcpV6_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV6_REBIND);
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_increase
(
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  /* calculate new timeout value */
  uint32 maxTimeout = TCPIP_DHCPV6_100MS_TO_1S_FACTOR;
  uint32 timeout = TcpIp_DhcpV6_Data[dhcpIdx].rtx_timeout;
  timeout = timeout + ( (timeout - (timeout/10U)) +
                         TcpIp_GetRandomRange((uint16)(timeout/5U))
                      );

  switch(msg_type)
  {
    case TCPIP_DHCPV6_SOLICIT:
      maxTimeout *= TCPIP_DHCPV6_SOL_MAX_RT;
      break;
    case TCPIP_DHCPV6_REQUEST:
      maxTimeout *= TCPIP_DHCPV6_REQ_MAX_RT;
      break;
    case TCPIP_DHCPV6_RENEW:
      maxTimeout *= TCPIP_DHCPV6_REN_MAX_RT;
      break;
    case TCPIP_DHCPV6_REBIND:
      maxTimeout *= TCPIP_DHCPV6_REB_MAX_RT;
      break;
    case TCPIP_DHCPV6_DECLINE:
      maxTimeout = TCPIP_DHCPV6_INF_MAX_RT;
      break;
      /* CHECK: NOPARSE */
    case TCPIP_DHCPV6_INVALID_MSG_TYPE: /* intended fall through */
    case TCPIP_DHCPV6_ADVERTISE:        /* intended fall through */
    case TCPIP_DHCPV6_REPLY:            /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
      /* CHECK: PARSE */
  }

  if((timeout > maxTimeout) && (maxTimeout != TCPIP_DHCPV6_INF_MAX_RT))
  {
    timeout = maxTimeout;
  }

  TcpIp_DhcpV6_Data[dhcpIdx].rtx_timer = timeout;
  TcpIp_DhcpV6_Data[dhcpIdx].rtx_timeout = timeout;

  /* retransmit DHCP message */
  (void) TcpIp_DhcpV6_transmit(dhcpIdx, msg_type);
}

STATIC FUNC(uint32, TCPIP_CODE) TcpIp_DhcpV6_calculateRetransTimeout
(
  uint16 initRtxTimeout
)
{
  uint32 randIRT = 0U;
  /* get random range from 10% of initial value -1 and add 1 after
     this way RAND will be strictly greater than 0 */
  randIRT = initRtxTimeout + (uint32)TcpIp_GetRandomRange((initRtxTimeout/10U)-1U) + 1U;

  return randIRT;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_SOLICIT_REQUESTING
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* transitional code */
  {
    uint32 randXid = TcpIp_GetRandomValue();
    TCPIP_DHCPV6_SET_XID(TcpIp_DhcpV6_DataPtr->xid, randXid);

    TcpIp_DhcpV6_DataPtr->server_id_len = opt->server_id_len;
    TS_MemCpy(TcpIp_DhcpV6_DataPtr->server_id, opt->server_id, TcpIp_DhcpV6_DataPtr->server_id_len);

    TCPIP_IPV6_ADDR_CPY (TcpIp_DhcpV6_DataPtr->ip_addr, opt->ip_req);

    /* acquisition process starts */
    TcpIp_DhcpV6_DataPtr->elapsed_time = 0U;

    /* set timer and retry attempts */
    TcpIp_DhcpV6_DataPtr->timer = 0U;
    TcpIp_DhcpV6_DataPtr->req_dec_num_retry = 1U;

    /* first retransmission */
    TcpIp_DhcpV6_DataPtr->rtx_timer =
      TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_REQ_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
    TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

    TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_REQUESTING);

    (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_REQUEST);
  }

  /* entry code */
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_REASSIGNING
(
  uint8 dhcpIdx
)
{
  /* exit code */
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* transitional code */
  /* set timers and retry attempts */
  TcpIp_DhcpV6_DataPtr->elapsed_time = 0U;
  TcpIp_DhcpV6_DataPtr->timer = 0U;
  TcpIp_DhcpV6_DataPtr->req_dec_num_retry = 1U;

  /* first retransmission */
  TcpIp_DhcpV6_DataPtr->rtx_timer =
    TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_REQ_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
  TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

  /* transmit DHCP message */
  (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_REQUEST);

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_REASSIGNING);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_INIT
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_INIT(dhcpIdx);
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_INIT
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  /* exit code */
  TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST(dhcpIdx);
  TcpIp_DhcpV6_exit_ASSIGNED(localAddr);

  /* transitional code */

  /* entry code */
  TcpIp_DhcpV6_entry_INIT(dhcpIdx);
}

#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* exit code */

  /* transitional code */
  {
    /* save ip */
    TCPIP_IPV6_ADDR_CPY (TcpIp_DhcpV6_Data[dhcpIdx].ip_addr, opt->ip_req);
    TcpIp_DhcpV6_Data[dhcpIdx].req_dec_num_retry = 0U;
  }

  /* entry code */

  TcpIp_DhcpV6_pre_entry_BOUND_timeUpdate(dhcpIdx,opt);
  TcpIp_DhcpV6_entry_ASSIGNED_MULTICAST(dhcpIdx);
  TcpIp_DhcpV6_entry_BOUND(dhcpIdx);
  TcpIp_DhcpV6_entry_ASSIGNED(localAddrId);
}
#else /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
)
{
  const uint8 dhcpIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_DHCPV6);
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  switch(TcpIp_DhcpV6_DataPtr->state)
  {

#if (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_DHCPV6_STATE_OPTIMISTIC:
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_DHCPV6_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
      if(unique)
      {
        TcpIp_DhcpV6_tr_dadResult_DAD_BOUND(localAddrId, dhcpIdx);
      }
      else
      {
        TcpIp_DhcpV6_tr_dadResult_entry_CONFLICTED(localAddrId,dhcpIdx);
      }
      break;
#endif /*(TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */

    /* CHECK: NOPARSE */
    case TCPIP_DHCPV6_STATE_INACTIVE:     /* intended fall through */
    case TCPIP_DHCPV6_STATE_START_DELAY:  /* intended fall through */
    case TCPIP_DHCPV6_STATE_INIT:         /* intended fall through */
    case TCPIP_DHCPV6_STATE_SOLICIT:      /* intended fall through */
    case TCPIP_DHCPV6_STATE_REQUESTING:   /* intended fall through */
    case TCPIP_DHCPV6_STATE_BOUND:        /* intended fall through */
    case TCPIP_DHCPV6_STATE_RENEWING:     /* intended fall through */
    case TCPIP_DHCPV6_STATE_REBINDING:    /* intended fall through */
    case TCPIP_DHCPV6_STATE_REASSIGNING:  /* intended fall through */
    case TCPIP_DHCPV6_STATE_CONFLICTED:   /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_dadResult_entry_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TcpIp_DhcpV6_DataPtr->rtx_timer =
    TcpIp_DhcpV6_calculateRetransTimeout(TCPIP_DHCPV6_DEC_TIMEOUT * TCPIP_DHCPV6_100MS_TO_1S_FACTOR);
  TcpIp_DhcpV6_DataPtr->rtx_timeout = TcpIp_DhcpV6_DataPtr->rtx_timer;

  TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST(dhcpIdx);
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
  TcpIp_DhcpV6_exit_ASSIGNED(localAddrId);
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

  (void) TcpIp_DhcpV6_transmit(dhcpIdx, TCPIP_DHCPV6_DECLINE);
  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_INVALID;
  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state,TCPIP_DHCPV6_STATE_CONFLICTED);
  TcpIp_DhcpV6_DataPtr->req_dec_num_retry = 1U;
  TcpIp_DhcpV6_DataPtr->timer = 0U;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_dadResult_DAD_BOUND
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx
)
{
  TcpIp_DhcpV6_entry_BOUND(dhcpIdx);
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)
  TcpIp_DhcpV6_entry_ASSIGNED(localAddrId);
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
  TS_PARAM_UNUSED(localAddrId);
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_REQUESTING_DAD
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  /* exit code */

  /* transitional code */
  {
    /* save ip */
    TCPIP_IPV6_ADDR_CPY (TcpIp_DhcpV6_Data[dhcpIdx].ip_addr, opt->ip_req);
    TcpIp_DhcpV6_Data[dhcpIdx].req_dec_num_retry = 0U;
  }

  /* entry code */
  TcpIp_DhcpV6_pre_entry_BOUND_timeUpdate(dhcpIdx,opt);
  TcpIp_DhcpV6_entry_ASSIGNED_MULTICAST(dhcpIdx);
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
  TcpIp_DhcpV6_entry_OPTIMISTIC(dhcpIdx);
  TcpIp_DhcpV6_entry_ASSIGNED(localAddrId);
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
  TcpIp_DhcpV6_entry_TENTATIVE(dhcpIdx);
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

  TcpIp_Ndp_Dad_start(&TcpIp_DhcpV6_dadResult, TcpIp_DhcpV6_Data[dhcpIdx].ip_addr, localAddrId);

}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateConflictedResponse
(
  uint8 dhcpIdx,
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt,
  uint8 msg_type
)
{
  if((TcpIp_DhcpV6_evaluateReplyMsg(opt, dhcpIdx, msg_type) == E_OK))
  {
    if((TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime(opt, dhcpIdx, FALSE) == E_OK))
    {
      TcpIp_DhcpV6_tr_received_CONFLICTED_INIT(dhcpIdx);
    }
  }
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_retransTimeout_CONFLICTED_CONFLICTED
(
  uint8 dhcpIdx
)
{
  /* exit code */

  /* transitional code */
  TcpIp_DhcpV6_tr_retransTimeout_increase(dhcpIdx, TCPIP_DHCPV6_DECLINE);

  /* entry code */
}

#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_OPTIMISTIC
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* start timer for valid timeout */
  TcpIp_DhcpV6_DataPtr->timer = TcpIp_DhcpV6_DataPtr->T1_time;

  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_OPTIMISTIC);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_OPTIMISTIC_INACTIVE
(
  uint8 dhcpIdx,
  TcpIp_LocalAddrIdType localAddr
)
{
  TcpIp_Ndp_Dad_abort(localAddr);
  TcpIp_DhcpV6_tr_stopAssign_BOUND_INACTIVE(dhcpIdx,localAddr);
}

#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_TENTATIVE
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* start timer for valid timeout */
  TcpIp_DhcpV6_DataPtr->timer = TcpIp_DhcpV6_DataPtr->T1_time;

  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_TENTATIVE;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_TENTATIVE);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_stopAssign_TENTATIVE_INACTIVE
(
  uint8 dhcpIdx,
  TcpIp_LocalAddrIdType localAddr
)
{
  TcpIp_Ndp_Dad_abort(localAddr);
  TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST(dhcpIdx);
  TcpIp_DhcpV6_tr_stopAssign_INIT_INACTIVE(dhcpIdx);
}
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_INACTIVE
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TcpIp_DhcpV6_DataPtr->timer = 0U;
  TcpIp_DhcpV6_DataPtr->rtx_timer = 0U;
  TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_NXT_MAIN);
  TcpIp_DhcpV6_DataPtr->num_retry = 0U;
  TcpIp_DhcpV6_DataPtr->socketId = TCPIP_SOCKETID_INVALID;
  TcpIp_DhcpV6_DataPtr->server_id_len = 0U;
  TcpIp_DhcpV6_DataPtr->elapsed_time = TCPIP_DHCPV6_ELAPSED_TIMER_STOP;
  TCPIP_IPV6_SET_UNSPECIFIED_IP(TcpIp_DhcpV6_DataPtr->ip_addr);
  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_INVALID;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_INACTIVE);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_ASSIGNED_MULTICAST
(
  uint8 dhcpIdx
)
{
  TcpIp_SockAddrInet6Type solicitMulticast;
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_DhcpV6_Data[dhcpIdx].ip_addr;
  CONSTP2CONST(TcpIp_DhcpAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_DhcpAddrIpV6ConfigType,intIpV6DhcpAddr,dhcpIdx));
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) solicitMulticastPtr =
    /* Deviation MISRAC2012-1 */
    (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST)) &solicitMulticast;
  const TcpIp_LocalAddrIdType multicastlocalAddrId = intLocalAddrPtr->solMultilocalAddrId;

  TcpIp_IpV6_getSolicitMulti(solicitMulticast.addr, ip);
  solicitMulticast.domain = TCPIP_AF_INET6;

  /* assign solicited multicast ip address */
  (void)TcpIp_IpAddrM_requestIpAddrAssignment
          (multicastlocalAddrId, TCPIP_IPADDR_ASSIGNMENT_STATIC, solicitMulticastPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_exit_ASSIGNED_MULTICAST
(
  uint8 dhcpIdx
)
{
  CONSTP2CONST(TcpIp_DhcpAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_DhcpAddrIpV6ConfigType,intIpV6DhcpAddr,dhcpIdx));

  /* unassign solicited-node multicast ip address */
  (void)TcpIp_IpAddrM_releaseIpAddrAssignment(intLocalAddrPtr->solMultilocalAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddr
)
{
  /* inform IP address manager that IP address can be used */
  TcpIp_IpAddrM_AssignSM_assigned(localAddr, TCPIP_IPADDRM_ASSIGN_DHCPV6);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_pre_entry_BOUND_timeUpdate
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  /* stop retransmission timer */
  TcpIp_DhcpV6_DataPtr->rtx_timer = 0U;
  TcpIp_DhcpV6_DataPtr->num_retry = 0U;
  TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_NXT_MAIN);

  /* record lifetime and set timer values */
  TcpIp_DhcpV6_DataPtr->preferred_time = opt->preferred_time;
  TcpIp_DhcpV6_DataPtr->valid_time = opt->valid_time;
  TcpIp_DhcpV6_DataPtr->T1_time = opt->T1_time;
  TcpIp_DhcpV6_DataPtr->T2_time = opt->T2_time;

  /* acquisition process finished */
  TcpIp_DhcpV6_DataPtr->elapsed_time = TCPIP_DHCPV6_ELAPSED_TIMER_STOP;

  /* start timer for T1-timeout */
  TcpIp_DhcpV6_DataPtr->timer = opt->T1_time;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_tr_received_EXPANDING_BOUND
(
  uint8 dhcpIdx,
  P2CONST(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opt
)
{

  TcpIp_DhcpV6_pre_entry_BOUND_timeUpdate(dhcpIdx,opt);

  TcpIp_DhcpV6_entry_BOUND(dhcpIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_BOUND
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_PREFERRED;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_BOUND);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_START_DELAY
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_START_DELAY);

  if(TCPIP_DHCPV6_FLAG_INIT_LINK_LOCAL_IS_ASSIGNED_IS_SET(TcpIp_DhcpV6_DataPtr->flags))
  {
    TcpIp_DhcpV6_tr_LocalIpAddrAssignmentChg_START_DELAY_INIT(localAddr, dhcpIdx);
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_entry_ACTIVE
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 dhcpIdx
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  TcpIp_SocketIdType socketId;
  uint16 port = TCPIP_DHCPV6_CLIENT_PORT;

  /* Retrieve a socket */
  if(TcpIp_DhcpV6_getSocket(TCPIP_AF_INET6, TCPIP_IPPROTO_UDP, &socketId) == E_OK)
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddr);
    TcpIp_LocalAddrIdType linkLocalAddrId;
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
    linkLocalAddrId = TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV6.localAddrLinkLocal;

    /*  bind socket to link local address */
    if(TcpIp_Socket_bind(socketId, linkLocalAddrId, &port, FALSE) == E_OK)
    {
      TcpIp_DhcpV6_Data[dhcpIdx].socketId = socketId;

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

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_entry_INIT
(
  uint8 dhcpIdx
)
{
  CONSTP2VAR(TcpIp_DhcpV6_DataType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_DhcpV6_DataPtr = &TcpIp_DhcpV6_Data[dhcpIdx];

  TCPIP_IPV6_SET_UNSPECIFIED_IP(TcpIp_DhcpV6_DataPtr->ip_addr);
  TS_MemSet(TcpIp_DhcpV6_DataPtr->server_id, 0U, TCPIP_DHCPV6_SERVER_DUID_SIZE);
  TcpIp_DhcpV6_DataPtr->server_id_len = 0U;
  TcpIp_DhcpV6_DataPtr->rtx_timer = 0U;
  TcpIp_DhcpV6_DataPtr->flags &= ~((uint8) TCPIP_DHCPV6_FLAG_NXT_MAIN);
  TcpIp_DhcpV6_DataPtr->num_retry = 0U;
  TcpIp_DhcpV6_DataPtr->elapsed_time = TCPIP_DHCPV6_ELAPSED_TIMER_STOP;
  TcpIp_DhcpV6_DataPtr->expiry_state = TCPIP_IPADDRM_ADDRSTATE_INVALID;

  /* wait before transmitting SOLICIT */
  /*First SOLICIT message is delayed by a random amount between 0 and TCPIP_DHCPV6_SOL_MAX_DELAY*/
  TcpIp_DhcpV6_DataPtr->timer =
    (uint32)TcpIp_GetRandomRange (TCPIP_DHCPV6_SOL_MAX_DELAY * TCPIP_DHCPV6_100MS_TO_1S_FACTOR) +1U;

  TS_AtomicAssign8(TcpIp_DhcpV6_DataPtr->state, TCPIP_DHCPV6_STATE_INIT);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_exit_ACTIVE
(
  uint8 dhcpIdx
)
{
  if(TcpIp_DhcpV6_Data[dhcpIdx].socketId != TCPIP_SOCKETID_INVALID)
  {
    (void)TcpIp_Close(TcpIp_DhcpV6_Data[dhcpIdx].socketId, FALSE);
  }
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateServerId
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx
)
{
  Std_ReturnType result = E_NOT_OK;

  if(opts->server_id != NULL_PTR)
  {
    if((TS_MemCmp(opts->server_id, TcpIp_DhcpV6_Data[dhcpIdx].server_id,
                   TcpIp_DhcpV6_Data[dhcpIdx].server_id_len ) == E_OK)
      )
    {
      result = E_OK;
    }
  }
  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateClientId
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  TcpIp_LocalAddrIdType localAddr
)
{
  uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
  uint8 ctrlIdx;
  Std_ReturnType result = E_NOT_OK;

  ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddr);

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

  if((opts->client_id != NULL_PTR))
  {
    if(!TcpIp_EthAddrCmp(opts->client_id, ethAddr))
    {
      result = E_OK;
    }
  }
  return result;
}


STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateReplyMsg
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx,
  uint8 msg_type
)
{
  Std_ReturnType result = E_NOT_OK;

  if (msg_type == TCPIP_DHCPV6_REPLY)
  {
    result = TcpIp_DhcpV6_evaluateServerId (opts, dhcpIdx);
  }
  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateStatusAndTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  Std_ReturnType result = E_NOT_OK;

  if
  (
    (opts->status_code == TCPIP_DHCPV6_STATUS_CODE_SUCCESS) &&
    (opts->valid_time != 0U)
  )
  {
    result = E_OK;
  }
  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_evaluateStatusCode
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts
)
{
  /*since we currently only support single IA_NA
   * we check if any status code is not equal to success and save it to primary status code value
   * priority of status codes are
   * DHCPv6 option field > IA_NA option field > IAADDR option field*/
  if ((opts->status_code == TCPIP_DHCPV6_STATUS_CODE_SUCCESS) &&
      (opts->status_code_IA_NA != TCPIP_DHCPV6_STATUS_CODE_SUCCESS)
     )
  {
    opts->status_code = opts->status_code_IA_NA;
  }
  else if (
           (opts->status_code == TCPIP_DHCPV6_STATUS_CODE_SUCCESS) &&
           (opts->status_code_IAADDR != TCPIP_DHCPV6_STATUS_CODE_SUCCESS)
          )
  {
    opts->status_code = opts->status_code_IAADDR;
  }
  else
  {
    /* avoid misra */
  }

}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_evaluateIpAddIaIdAndLifeTime
(
  P2VAR(TcpIp_DhcpV6_OptionType, AUTOMATIC, TCPIP_APPL_DATA) opts,
  uint8 dhcpIdx,
  boolean checkTime
)
{
  Std_ReturnType result = E_NOT_OK;

  if
  (
    ((TcpIp_DhcpV6_evaluateLifeTime(opts) == E_OK) || (checkTime == FALSE)) &&
    (opts->IaId == TcpIp_DhcpV6_Data[dhcpIdx].IaId)
  )
  {
    if(TCPIP_IPV6_ADDR_CMP(opts->ip_req, TcpIp_DhcpV6_Data[dhcpIdx].ip_addr))
    {
      result = E_OK;
    }
  }
  return result;
}

#endif /* (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[end of file]===========================================*/
