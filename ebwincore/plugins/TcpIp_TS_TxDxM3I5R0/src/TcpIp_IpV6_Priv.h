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

#ifndef TCPIP_IPV6_PRIV_H
#define TCPIP_IPV6_PRIV_H

/*==================[inclusions]============================================*/

#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV6_Int.h>     /* module internal headerfile */
#include <TcpIp_Ndp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/
/** \brief default hop limit of an icmpV6 message */
#define TCPIP_IPV6_ICMP_DEFAULT_HOP_LIMIT                              \
  TcpIp_PBcfgPtr->ipV6Config.icmpV6HopLimit

/** \brief ICMP type destination unreachable message */
#define TCPIP_IPV6_ICMP_TYPE_DST_UNREACH                              1U
/** \brief ICMP type time exceeded message */
#define TCPIP_IPV6_ICMP_TYPE_TIME_EXCEEDED                            3U
/** \brief ICMP type parameter problem message */
#define TCPIP_IPV6_ICMP_TYPE_PARAM_PROBLEM                            4U
/** \brief ICMP type neighbor advertisement message */
#define TCPIP_IPV6_ICMP_TYPE_NEIGHBOR_ADVERT                        136U
/** \brief ICMP type neighbor solicitation message */
#define TCPIP_IPV6_ICMP_TYPE_NEIGHBOR_SOLICIT                       135U
/** \brief ICMP type neighbor advertisement message */
#define TCPIP_IPV6_ICMP_TYPE_ROUTER_ADVERT                          134U
/** \brief ICMP type echo request message */
#define TCPIP_IPV6_ICMP_TYPE_ECHO_REQUEST                           128U
/** \brief ICMP type echo reply message */
#define TCPIP_IPV6_ICMP_TYPE_ECHO_REPLY                             129U
/** \brief ICMP code destination unreachable message */
#define TCPIP_IPV6_ICMP_CODE_DST_UNREACH                              4U
/** \brief Length of ICMP header */
#define TCPIP_IPV6_ICMP_HEADER_LENGTH                                 4U


/** \brief report parameter problem message with unrecognized ipv6 option code */
#define TCPIP_IPV6_ICMP_CODE_TIME_EXCEEDED_FRAGMENT                   1U
/** \brief report parameter problem message with unrecognized next header code*/
#define TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_HDR                1U
/** \brief report parameter problem message with unrecognized ipv6 option code */
#define TCPIP_IPV6_ICMP_CODE_PARAM_PROB_UNRECOGNIZ_OPT                2U
/** \brief report parameter problem message with erroneous header field code */
#define TCPIP_IPV6_ICMP_CODE_PARAM_PROB_ERROR_HDR                     0U

/** \brief Length of ICMP header of an error message */
#define TCPIP_IPV6_ICMP_ERR_MSG_FIXED_LENGTH                          4U
/** \brief Length of ICMPv6 echo request message */
#define TCPIP_IPV6_MIN_ICMP_ECHO_REQ_LENGTH                           4U

/** \brief macro converting a multicast IP address to a multicast ethernet
  broadcast address */
#define TcpIp_IpV6_GetPhysMulticastAddr(physAddrPtr, remoteAddr)       \
  do{                                                                  \
      P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr =         \
        (P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA)) (remoteAddr);       \
                                                                       \
      (physAddrPtr)[0U] = 0x33U;                                       \
      (physAddrPtr)[1U] = 0x33U;                                       \
      (physAddrPtr)[2U] = remoteAddrPtr[12U];                          \
      (physAddrPtr)[3U] = remoteAddrPtr[13U];                          \
      (physAddrPtr)[4U] = remoteAddrPtr[14U];                          \
      (physAddrPtr)[5U] = remoteAddrPtr[15U];                          \
  }while(0) \

#define TCPIP_IPV6_ICMP_ECHO_REPLY_IS_ENABLED                          \
  (TcpIp_PBcfgPtr->ipV6Config.icmpV6EchoReplyEnable == TRUE)

#define TCPIP_IPV6_ICMP_ECHO_REPLY_TO_MULTICAST_IS_ENABLED             \
  ((TcpIp_PBcfgPtr->ipV6Config.icmpV6EchoReplyToMulticastEnabled) == TRUE)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Reception of Icmp datagram
 *
 * This API service indicates the reception of an Icmp frame.
 *
 * \param[in] remoteAddrPtr     IP address and port of the remote host.
 * \param[in] localIpAddrPtr    Local IP address.
 * \param[in] dataPtr           Points to the received data. The data contains the payload of the
 *                              Ip layer (which includes the Icmp header).
 * \param[in] localAddrId       Local address the frame has been received on.
 * \param[in] lenByte           Length of received data.
 * \param[in] hopLimit          hop limit of the received icmp message
 * \param[in] isFragmented      If true, packet is fragmented
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE ) TcpIp_IpV6_Icmp_rxIndication
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  uint8 hopLimit,
  boolean isFragmented
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Reads and resets dropped datagrams caused by invalid ICMv6
 *
 * This function reads and resets dropped datagrams caused by invalid ICMv6.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetInvalidIcmpCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/** \brief Initalize static IpV6
 *
 * This function initialize the static IpV6 data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method       Static IPv6 or Link Local IPv6
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_init
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
);

/** \brief Request static IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method       Static IPv6 or Link Local IPv6
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_startAssign
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

/** \brief Releases static IpV6 assignment.
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method       Static IPv6 or Link Local IPv6
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
);

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

/** \brief Indicate if ip address is unique or a duplicate
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] unique       TRUE: ip address is unique, FALSE: duplicated
 *                         ip address detected
 * \param[in] method       Static IPv6 or Link Local IPv6
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique,
  uint8 method
);

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */

/** \brief get Ip configuration of the static IpV6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayIpPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 * \param[in]  method           Static IPv6 or Link Local IPv6
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr,
  uint8 method
);

/** \brief get Ip address of the static IpV6 assignment
 *
 * \param[in]  localAddrId      Id of local IP address
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[in]  method           Static IPv6 or Link Local IPv6
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  uint8 method
);

/** \brief Request assignment of IP address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] LocalIpAddrPtr  Pointer to requested IP
 * \param[in]  method         Static IPv6 or Link Local IPv6
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_Static_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr,
  uint8 method
);

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in]  method         Static IPv6 or Link Local IPv6
 *
 * \return Result of operation  static ip address state
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_Static_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
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


#endif /* TCPIP_IPV6_PRIV_H */
