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

#ifndef TCPIP_NDP_PRIV_H
#define TCPIP_NDP_PRIV_H

/*==================[inclusions]============================================*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ndp_Int.h>      /* module internal header file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/* \brief Minimal size of a neighbor solicitation message */
#define TCPIP_NDP_NEIGH_SOLIC_SIZE_MIN                              20U
/* \brief Minimal size of a neighbor advertisement message */
#define TCPIP_NDP_NEIGH_ADVERT_SIZE_MIN                             20U
/* \brief Minimal size of a router solicitation message */
#define TCPIP_NDP_RPD_ROUTER_SOLIC_SIZE_MIN                          4U

/** \brief ICMP type neighbor advertisement message */
#define TCPIP_NDP_ICMP_TYPE_NEIGHBOR_ADVERT                        136U
/** \brief ICMP type neighbor solicitation message */
#define TCPIP_NDP_ICMP_TYPE_NEIGHBOR_SOLICIT                       135U
/** \brief ICMP type router advertisement message */
#define TCPIP_NDP_ICMP_TYPE_ROUTER_ADVERT                          134U
/** \brief ICMP type neighbor solicitation message */
#define TCPIP_NDP_ICMP_TYPE_ROUTER_SOLICIT                         133U
/** \brief ICMP code neighbor discovery protocol */
#define TCPIP_NDP_ICMP_CODE_NDP                                      0U

/* \brief Default hop limit of ndp messages */
#define TCPIP_NDP_HOP_LIMIT                                        255U

/* \brief Length of the option header */
#define TCPIP_NDP_OPT_HDR_LEN                                        2U
/* \brief Id of the source link layer address option */
#define TCPIP_NDP_OPT_SRC_LINK_ADDR_ID                               1U
/* \brief Length of the source link layer address option in 8 bytes */
#define TCPIP_NDP_OPT_SRC_LINK_ADDR_LEN                              1U
/* \brief Id of the target link layer address option */
#define TCPIP_NDP_OPT_TARGET_LINK_ADDR_ID                            2U
/* \brief Length of the target link layer address option in 8 bytes */
#define TCPIP_NDP_OPT_TARGET_LINK_ADDR_LEN                           1U
/* \brief Length of the link layer address */
#define TCPIP_NDP_OPT_LINK_ADDR_LEN                                  6U

/* \brief Set the type of option */
#define TCPIP_NDP_SET_OPTION_TYPE(pdata, option)                      \
  (pdata)[0U] = (option)

/* \brief Set the length of the options */
#define TCPIP_NDP_SET_OPTION_LENGTH(pdata, length)                    \
  (pdata)[1U] = (length)

/* \brief get the link-layer address from ND messages */
#define TCPIP_NDP_OPT_LL_ADDR_GET_PTR(pdata)                          \
  (&(pdata)[2U])

/* \brief* Sets the three flag bits */
#define TCPIP_NDP_SET_FLAGS(pdata, flags)                             \
  (pdata)[0U] = (flags)

/* \brief Initializes the reserved 29 bits with zero */
#define TCPIP_NDP_NEIGH_ADVERT_SET_RESERVED(pdata)                    \
  do                                                                  \
  {                                                                   \
    (pdata)[0U] &= 0xE0U;                                             \
    (pdata)[1U] = 0U;                                                 \
    (pdata)[2U] = 0U;                                                 \
    (pdata)[3U] = 0U;                                                 \
  } while (0U)

/* \brief Macro for initializing the reserved 32 bits */
#define TCPIP_NDP_NEIGH_SOLIC_SET_RESERVED(pdata)                     \
  do                                                                  \
  {                                                                   \
    (pdata)[0U] = 0U;                                                 \
    (pdata)[1U] = 0U;                                                 \
    (pdata)[2U] = 0U;                                                 \
    (pdata)[3U] = 0U;                                                 \
  } while (0U)

/* \brief Macro for initializing the reserved 32 bits */
#define TCPIP_NDP_RPD_ROUTER_SOLIC_SET_RESERVED(pdata)                \
  do                                                                  \
  {                                                                   \
    (pdata)[0U] = 0U;                                                 \
    (pdata)[1U] = 0U;                                                 \
    (pdata)[2U] = 0U;                                                 \
    (pdata)[3U] = 0U;                                                 \
  } while (0U)

/* \brief Get the target address from ND messages */
#define TCPIP_NDP_TARGET_ADDR_GET_PTR(pdata)                          \
  (&(pdata)[4U])

/*==================[type definitions]======================================*/

typedef struct
{
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) target_link_addr;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) source_link_addr;
}TcpIp_Ndp_NCache_OptionType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initalizes the neighbor cache */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_init (void);

/** \brief NDP table main function
 *
 *  This function performs the NDP table related timing.
 *
 * \param[in] timerTick  current time in ticks
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_mainFunction
(
  uint16 timerTick
);

/** \brief Update NDP cache Solicitation
 *
 * This function updates the NDP cache according to the values received in a solicitation.
 *
 * \param[in] ctrlIdx           controller index
 * \param[in] remoteAddrPtr     ip address of the remote host
 * \param[in] ll_target_addr    link layer address of the remote host
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_update
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
);

/** \brief Handle received Neighbor Solicitions
 *
 * This function evaluates a received neighbor solicitation. It updates the NDP cache
 * and responses with a neighbor advertisement.
 *
 * \param[in] remoteAddrPtr  ip address of the remote host
 * \param[in] localAddrId    local address on which the solicitation was received on
 * \param[in] dataPtr        points to the beginning of the solicitation
 * \param[in] lenByte        length of the solicitation
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_rxNeighborSolicit
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);

/** \brief Handle received Neighbor Advertisement
 *
 * This function evaluates a received neighbor advertisements. It updates the NDP cache
 * and checks for address duplicates.
 *
 * \param[in] localAddrId    local address on which the advertisement was received on
 * \param[in] dataPtr        points to the beginning of the advertisement
 * \param[in] lenByte        length of the advertisement
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_rxNeighborAdvert
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);


#if (TCPIP_NDP_RPD_ENABLED == STD_ON)

/** \brief Initializes SW-Unit Ndp
 *
 * This API service performs the initialization of router and prefix discovery
 * related data structures.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_init(void);

/** \brief Time based scheduling of NDP router and prefix discovery functionality
 *
 * \param[in] timerTick     current time in ticks of TcpIp timer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_mainFunction
(
  uint16 timerTick
);

/** \brief Handle received Neighbor Advertisement
 *
 * This function evaluates a received neighbor advertisements. It updates the NDP cache
 * and checks for address duplicates.
 *
 * \param[in] localAddrId    local address on which the advertisement was received on
 * \param[in] dataPtr        points to the beginning of the advertisement
 * \param[in] lenByte        length of the advertisement
 */

TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_rxRouterAdvert
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);

/** \brief Closing router list entry.
 *
 * \param[in] ctrlIdx               Index of the EthIf controller
 * \param[in] ipAddr                Address of the entry in router list cache
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_remove
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ipAddr
);

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
/** \brief Initializes a destination cache per controller.
 *
 * This function initializes the destination cache per controller.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_init(void);

/** \brief Removes a destination cache entries.
 *
 * This function removes the destination cache entries that have specific router entry as next hop.
 *
 * \param[in] ctrlIdx            Index of the ethernet controller (EthIfCtrlIdx)
 * \param[in] ipAddrPtr          Ip address of the router entry that was next hop
 */

TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_removeNextHopEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

/** \brief Checking cache entry state.
 *
 * \param[in] CtrlIdx               Index of the EthIf controller
 * \param[in] ipAddr                Address of the entry in router list cache
 *
 * \return Result of operation
 * \retval TRUE                ipAddr is found in state REACHABLE, PROBE, STALE, DELAY or STATIC
 * \retval FALSE               ipAddr isn't found in state REACHABLE, PROBE, STALE, DELAY or STATIC
 */

TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_EntryIsReachable
(
  uint8 CtrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ipAddr
);

#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */


/** \brief Transmits neighbor solicitation to unicast address
 *
 * \param[in] localAddrId       local address used for transmission
 * \param[in] remoteIpAddr      ip address of the remote host
 * \param[in] targetIp          ip address of the target
 * \param[in] remoteAddrHandle  index into the ndp cache
 * \param[in] unspecifiedSource indicates if source address should be set to
 *                              unspecified (used for DAD probing)
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_transmitNeighborSolicit
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  uint16 remoteAddrHandle,
  boolean unspecifiedSource
);

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
/** \brief Initialize all addresses for Duplicate address detection */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_init(void);

/** \brief MainFunction for SW-Unit Ndp duplicate address detection
 *
 *  Goes trough all IpV6 unicast addresses
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_mainFunction
(
  uint16 timerTick
);

/** \brief Check if address is already assigned
 *
 * Takes valid address Id when OPTIMISTIC DAD is disabled. Otherwise takes
 * a valid Ip address.
 *
 * \param[in] targetIp     IPv6 unicast address Id to verify uniqueness of the
 *                         address
 * \param[in] targetIdIn   IPv6 unicast address to verify it's uniqueness
 *
 * \retval TCPIP_OK        address is not duplicate
 * \retval TCPIP_E_NOT_OK  address is duplicate
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_Dad_checkDuplicate
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp
);

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */

/** \brief Performs Ndp transmission
 *
 *  This function sets Ndp parameters and sets the function used for copying
 *  Ndp data and and invokes the transmission of the Icmp frame over IpV6
 *
 * \param[in] ipAddrPtr                  remote and local address
 * \param[in] remoteAddrHandlePtr        remote address handle pointer
 * \param[in] targetIp                   pointer to the target address
 * \param[in] msgLength                  The length of the message.
 * \param[in] type                       type of Ndp message
 * \param[in] flags                      Ndp header flags
 * \param[in] addLocalLinkLayerAddress   specifies if target link layer address option is transmitted
 * \param[in] useUnspecifiedSrc          specifies if Unspecified source address is used
 *
 * \return Result of operation
 * \retval TCPIP_OK               Transmission request has been successfully performed.
 * \retval TCPIP_E_NOT_OK         Transmission request failed.
*/
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_IntTransmit
(
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint16 remoteAddrHandle,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  uint16 msgLength,
  uint8 type,
  uint8 flags,
  boolean addLocalLinkLayerAddress ,
  boolean useUnspecifiedSrc
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
