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


/*==================[inclusions]============================================*/
#include <EthIf.h>                /* EthIf API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ndp_Priv.h>       /* own SW-unit private file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */
#include <SchM_TcpIp.h>           /* SchM API declarations */
/*==================[macros]================================================*/

/* \brief Maximum number of router solicitations that will be
          sent when performing prefix discovery. */
#define TCPIP_NDP_MAX_RTR_SOLICITATIONS                               \
  TcpIp_PBcfgPtr->ndpConfig.ndpMaxRtrSolicitations
/* \brief Interval between retransmission of the router solicitations
          that will be sent when performing prefix discovery. (in seconds)*/
#define TCPIP_NDP_RTR_SOLICITATION_INTERVAL                           \
  TcpIp_PBcfgPtr->ndpConfig.ndpRtrSolicitationInterval
/* \brief Maximum delay between router solicitations that will be sent when
   performing prefix discovery. (in seconds)*/
#define TCPIP_NDP_MAX_RTR_SOLICITATION_DELAY                          \
  TcpIp_PBcfgPtr->ndpConfig.ndpMaxRtrSolicitationDelay

/* \brief States of the Router and Prefix discovery controller state machines */
#define TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED                          0U
#define TCPIP_NDP_RPD_CTRL_STATE_START_DELAY                         1U
#define TCPIP_NDP_RPD_CTRL_STATE_PROBING                             2U
#define TCPIP_NDP_RPD_CTRL_STATE_LISTEN                              3U

/* \brief States of Router assignment state machine */
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE                    0U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED                   1U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED                  2U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE                   3U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC                  4U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED                  5U
#define TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED                  6U

/* \brief States of Router list state machine */
#define TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE                      0U
#define TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED                    1U
#define TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED                      2U

/* \brief States of Destination Cache */
#define TCPIP_NDP_DESTINATION_CACHE_STATE_UNASSIGNED                 0U
#define TCPIP_NDP_DESTINATION_CACHE_STATE_ASSIGNED                   1U

/* \brief Router Advertisement options */
#define TCPIP_NDP_RPD_OPTION_SOURCE_LINK_LAYER_ADDR               0x01U
#define TCPIP_NDP_RPD_OPTION_TARGET_LINK_LAYER_ADDR               0x02U
#define TCPIP_NDP_RPD_OPTION_PREFIX_INFORMATION                   0x03U

/* \brief Minimal size of a router advertisement message */
#define TCPIP_NDP_RPD_ROUTER_ADVERT_SIZE_MIN                        12U

/* \brief Prefix information option length in bytes */
#define TCPIP_NDP_RPD_PREFIX_OPT_LEN                                32U
/* \brief Prefix information option L flag bit */
#define TCPIP_NDP_RPD_PREFIX_OPT_FLAG_L                           0x80U
/* \brief Prefix information option A flag bit */
#define TCPIP_NDP_RPD_PREFIX_OPT_FLAG_A                           0x40U

/* \brief size of source link layer address option */
#define TCPIP_NDP_RPD_SOURCE_LINK_LAYER_OPT_HEADER_LEN_BYTES         8U

/* \brief expected size of a prefix in the prefix information option */
#define TCPIP_NDP_RPD_DEAULT_PREFIX_LEN                              8U

/* \brief 2h valid life time of a prefix */
#define TCPIP_NDP_RPD_2_HOURS_IN_SECONDS                          7200U

/* \brief Get the router lifetime from NDP RPD messages */
#define TCPIP_NDP_RPD_GET_ROUTER_LIFETIME(pdata)                      \
  TCPIP_GET16((pdata), 2)

/* \brief Get the router reachable time from NDP RPD messages */
#define TCPIP_NDP_RPD_GET_REACHABLE_TIME(pdata)                       \
  TCPIP_GET32((pdata), 4)

/* \brief Get the router retrans timer from NDP RPD messages */
#define TCPIP_NDP_RPD_GET_RETRANS_TIMER(pdata)                        \
  TCPIP_GET32((pdata), 8)

/* \brief Get the id of the next option */
#define TCPIP_NDP_RPD_GET_OPTION_TYPE(pdata)                          \
  (pdata)[0U]

/* \brief Get the id of the length of the next option */
#define TCPIP_NDP_RPD_GET_OPTION_LENGTH(pdata)                        \
  (((uint16)(pdata)[1U]) * 8U)

/* \brief Prefix options header */
/* \brief Get the length of the prefix */
#define TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFIX_LENGTH(pdata)          \
  (pdata)[2U]
/* \brief Get the flags of the prefix options */
#define TCPIP_NDP_RPD_PREFIX_OPTION_GET_FLAGS(pdata)                  \
  ((pdata)[3U] & 0xC0U)
/* \brief Get the valid lifetime of the prefix */
#define TCPIP_NDP_RPD_PREFIX_OPTION_GET_VALID_LIFETIME(pdata)         \
  TCPIP_GET32((pdata), 4)
/* \brief Get the valid lifetime of the prefix */
#define TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFERRED_LIFETIME(pdata)     \
  TCPIP_GET32((pdata), 8)
/* \brief Get the valid lifetime of the prefix */
#define TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFIX_PTR(pdata)             \
  (&(pdata)[16U])

/* \brief Get the prefix from ipAddress */
#define TCPIP_NDP_RPD_GET_PREFIX_FROM_IPADDRESS(ipAddress,prefix)      \
  do                                                                   \
  {                                                                    \
    (prefix)[0U] = (ipAddress)[0U];                                    \
    (prefix)[1U] = (ipAddress)[1U];                                    \
  } while (0U)

/** \brief Unassigns generated IPv6 address when valid life time expires
 *
 * \param[in]  cIdx     controller on which router and prefix discovery is performed
 * \param[in]  aIdx     Index to the router assignment data structure
 * \param[in]  lAddrId  Local address id of the router assignment
 * \param[in]  aPtr     points to the router and prefix assignment control data structure
 */
#define TcpIp_Ndp_Rpd_RouterAssign_tr_timeout_PREFERRED_INACTIVE(cIdx, aIdx, lAddrId, aPtr) \
  TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_ASSIGNED_INACTIVE((cIdx), (aIdx), (lAddrId), (aPtr))

/** \brief Macro checks preferred_lifetime and changes the state */
#define TCPIP_NDP_RPD_PREFERRED_DEPRECATED(preferred_lifetime) \
  ((preferred_lifetime) > 0U) ? TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED : TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED

/** \brief Macro checks if timeout has occurred and moves state to deprecated */
#define TcpIp_Ndp_Rpd_RouterAssignTimeout_PREFERRED_DEPRECATED(preferred_lifetime,state) \
    (state) = TCPIP_NDP_RPD_PREFERRED_DEPRECATED((preferred_lifetime))

/*==================[type definitions]======================================*/

/* !LINKSTO TcpIp.Design.TcpIp_Ndp_Rpd_RouterAssign_DataType,1 */
typedef struct
{
  /**< contains the IpV6 address generated from the received prefix */
  uint32 ipAddr[TCPIP_IPV6_ADDR_SIZE];
  /**< valid life time value (in seconds) */
  uint32 valid_lifetime;
  /**< preferred life time value (in seconds) */
  uint32 preferred_lifetime;
  /**< current state of the router assignment state machine */
  uint8 state;
}TcpIp_Ndp_Rpd_RouterAssign_DataType;

typedef struct
{
  /**< state of the Prefix discovery state machine */
  uint8 state;
  /**< timer for the Prefix discovery state machine */
  uint8 timer;
  /**< counter for router solicitation retransmissions */
  uint8 RTR_Counter;
  /**< indicates how many additional router assignments are active for this controller
    if more than one router assignment is configured */
  uint8 additionalAssignCount;
}TcpIp_Ndp_Rpd_Ctrl_DataType;

/* !LINKSTO TcpIp.Design.TcpIp_Ndp_Rpd_Router_List_DataType,1 */
typedef struct
{
  /**< router ipv6 address */
  uint32 ipAddr[TCPIP_IPV6_ADDR_SIZE];
  /**< valid life time value (in seconds) */
  uint32 valid_lifetime;
  /**< state of the Router list state machine */
  uint8 state;
}TcpIp_Ndp_Rpd_RouterList_DataType;

/* !LINKSTO TcpIp.Design.TcpIp_Ndp_Rpd_DestCache_DataType,1 */
typedef struct
{
  /**< IP address of the remote host */
  uint32 remoteIpAddr[TCPIP_IPV6_ADDR_SIZE];
  /**< IP address of the router */
  uint32 nextHopIpAddr[TCPIP_IPV6_ADDR_SIZE];
  /**< lock that indicates if entry is in use (> 0), or not (= 0) */
  uint8 lock;
  /**< state of the address entry */
  boolean state;
}TcpIp_Ndp_DestinationCache_DataType;

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

#if(TCPIP_IPV6_ENABLED == STD_ON)
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if(TCPIP_NDP_RPD_ENABLED == STD_ON)
/** \brief Verify if options of a router advertisement have a correct length
 *
 * \param[in]  dataPtr   points to the beginning of the option
 * \param[in]  lenByte   length of the option field
 *
 *
 * \retval E_OK      options are well formated
 * \retval E_NOT_OK  error in option field encountered
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_validateOpts
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);

/** \brief Retrieve options
 *
 * This function evaluates and processes the options of a received router advertisement message
 *
 * \param[in]  ctrlIdx        controller on which the router advertisement was received on
 * \param[in]  dataPtr        points to the beginning of the option field
 * \param[in]  lenByte        length of the option field
 * \param[out] options        points to source link layer of the router advertisement, if
 *                            none exists in the router advertisement NULL_PTR is returned
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_parseOpts
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2VAR(TcpIp_Ndp_NCache_OptionType, AUTOMATIC, TCPIP_APPL_DATA) options
);

/** \brief Transmits router solicitation to all routers multicast
 *
 * \param[in] localAddrId  local address used for transmission
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission
 */

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_transmitRouterSolicit
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Initializes the router and prefix controller data structure */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_init(void);

/** \brief Triggers periodic router and prefix discovery functions.
 *
 * This function handles retransmissions of router solicitations
 *
 * \param[in] timerTick  current time in ticks of TcpIp timer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_mainFunction
(
  uint16 timerTick
);

/** \brief Start router and prefix discovery
 *
 * This function starts the transmission of router solicitations on the specified controller
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery shall be started
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_startAssign
(
  uint8 ctrlIdx
);

/** \brief Stop router and prefix discovery
 *
 * This function stops the listing for router advertisements and transmission of router solicitations
 * on the specified controller
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery shall be stopped
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_stopAssign
(
  uint8 ctrlIdx
);

/** \brief Process trigger timeout
 *
 * This function performs transitions/actions in the router and prefix discovery controller state
 * machine when a  timeout occurs. e.g. transmissions of router solicitations.
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery is performed
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_timeout
(
  uint8 ctrlIdx
);

/** \brief Router list timeout
 *
 * This function performs transitions/actions in the router and prefix discovery router list state
 * machine when a  timeout occurs.
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery is performed
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_timeout
(
  uint8 ctrlIdx
);

/** \brief Closing router list entries
 *
 * This function closes all entries that are on the given controller.
 *
 * \param[in] ctrlIdx               Index of the EthIf controller
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_ctrlInit
(
  uint8 ctrlIdx
);

/** \brief Activate the default router list
 *
 * This function enables the processing of routers in the default router list
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery shall be started
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_startAssign
(
  uint8 ctrlIdx
);

/** \brief Deactivate the default router list
 *
 * This function disables the processing of routers in the default router list
 *
 * \param[in]  ctrlIdx   controller on which router and prefix discovery shall be started
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_stopAssign
(
  uint8 ctrlIdx
);

/** \brief Set an entry in the default router list to unused
 *
 * \param[in]  routerListDataPtr  entry in the default router list which should be set to unused
 * \param[in]  ctrlIdx            Index of the EthIf controller
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_entry_UNASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListDataPtr,
  uint8 ctrlIdx
);

/** \brief Process trigger rxRouterAdvert
 *
 * This function performs transitions/actions in the router and prefix discovery controller state
 * machine when a router advertisement is received. e.g. stop transmission of router solicitations.
 *
 * \param[in]  ctrlIdx         controller on which router and prefix discovery is performed
 * \param[in]  routerLifetime  life time of the router in seconds
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_rxRouterAdvert
(
  uint8 ctrlIdx,
  uint16 routerLifetime
);

/** \brief Transmission of router solicitation if a router advertisement has been received but
 *         no solicitation has been sent yet
 *
 * \param[in]  ctrlIdx      controller on which router and prefix discovery is performed
 * \param[in]  ctrlDataPtr  points to the router and prefix controller control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_tr_rxRouterAdvert_START_DELAY_LISTEN
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr
);

/** \brief Retransmission of router solicitation
 *
 * \param[in]  ctrlIdx      controller on which router and prefix discovery is performed
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_PROBING
(
  uint8 ctrlIdx
);

/** \brief Stop transmission of router solicitation if valid router advertisement is received
 *         or maximal number of router solicitations is reached and continue listening for
 *         router advertisements
 *
 * \param[in]  ctrlDataPtr  points to the router and prefix controller control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_LISTEN
(
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr
);

/** \brief Stop transmission of router solicitations
 *
 * \param[in]  ctrlDataPtr  points to the router and prefix controller control data structure
 * \param[in]  ctrlIdx      controller on which router and prefix discovery is performed
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_UNASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr,
  uint8 ctrlIdx
);

/** \brief Triggers periodic router and prefix discovery functions.
 *
 * This function handles timeouts of ipv6 addresses generated from a address prefix received
 * from router advertisements
 *
 * \param[in] timerTick  current time in ticks of TcpIp timer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_mainFunction
(
  uint16 timerTick
);

/** \brief Process trigger processPrefix
 *
 * This function performs transitions/actions in the router and prefix discovery assignment state
 * machine when a prefix information is received. e.g. generate ipv6 address if an unknown prefix is
 * received or update life times of ipv6 addresses.
 *
 * \param[in]  ctrlIdx            controller on which router and prefix discovery is performed
 * \param[in]  prefixAddrPtr      pointer to the received address prefix
 * \param[in]  validLifeTime      valid life time of the address prefix
 * \param[in]  preferredLifeTime  preferred life time of the address prefix
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_processPrefix
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
);

/** \brief Process trigger timeout
 *
 * This function performs transitions/actions in the router and prefix discovery assignment state
 * machine when a timeout occurs. e.g. unassigned ipv6 address if valid lifetime expires
 *
 * \param[in]  assignIdx     Index to the router assignment data structure
 * \param[in]  localAddrId   Local address id of the router assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_timeout
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Check router assignments
 *
 * This function loops over all configured router assignments and checks if the passed prefix matches
 * with an existing router assignment or if a free router assignment is left.
 *
 * \param[in]  ctrlIdx          controller on which router and prefix discovery is performed
 * \param[in]  prefixAddrPtr    pointer to the received address prefix
 *
 * \return - an existing router assignment if matching prefix was found
 *         - a free router assignment
 *         - number of configured router assignments (all entries are in use)
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getEntry
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr
);

/** \brief Sets a router assignment ready to be used and starts the router and prefix controller
 *         state machine (transmissions of router solicitations)
 *
 * \param[in]  ctrlIdx      controller on which router and prefix discovery is performed
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_startAssign_INACTIVE_UNASSIGNED
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Unassigns generated IPv6 address and stops the router and prefix controller
 *         state machine (stop transmissions of router solicitations and listening for advertisements)
 *
 * \param[in]  ctrlIdx       controller on which router and prefix discovery is performed
 * \param[in]  assignIdx     Index to the router assignment data structure
 * \param[in]  localAddrId   Local address id of the router assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_ASSIGNED_INACTIVE
(
  uint8 ctrlIdx,
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Stops the router and prefix controller state machine (stop transmissions of router
 *         solicitations and listening for advertisements)
 *
 * \param[in]  ctrlIdx       controller on which router and prefix discovery is performed
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_UNASSIGNED_INACTIVE
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Updates preferred and valid life times when a matching prefix is received
 *
 * \param[in]  assignPtr          points to the router and prefix assignment control data structure
 * \param[in]  validLifeTime      valid life time of the address prefix
 * \param[in]  preferredLifeTime  preferred life time of the address prefix
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_processPrefix_ASSIGNED_ASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
);

/** \brief Generates IPv6 address, request assignment of solicit multicast and inform IpAddrM that
 *         IPv6 address can be used for transmission/reception
 *
 * \param[in]  ctrlIdx            controller on which router and prefix discovery is performed
 * \param[in]  assignIdx          Index to the router assignment data structure
 * \param[in]  localAddrId        Local address id of the router assignment
 * \param[in]  assignPtr          points to the router and prefix assignment control data structure
 * \param[in]  prefixAddrPtr      pointer to the received address prefix
 * \param[in]  validLifeTime      valid life time of the address prefix
 * \param[in]  preferredLifeTime  preferred life time of the address prefix
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_start_address_assignment
(
  uint8 ctrlIdx,
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
);

/** \brief Request release of solicit multicast and inform IpAddrM that
 *         IPv6 address is not valid anymore and shall not be used any longer
 *
 * \param[in]  assignIdx     Index to the router assignment data structure
 * \param[in]  localAddrId   Local address id of the router assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_exit_ASSIGNED
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Stops the router and prefix controller state machine (stop transmissions of router
 *         solicitations and listening for advertisements)
 *
 * \param[in]  ctrlIdx     controller on which router and prefix discovery is performed
 * \param[in]  assignPtr   points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_INACTIVE
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Function to update or set router list cache entry.
 *
 * \param[in]  ctrlIdx        Index of the EthIf controller
 * \param[in]  remoteAddrPtr  ip address of the remote host
 * \param[in]  dataPtr        points to the beginning of the advertisement
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_rxRouterAdvert
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr
);

/** \brief Returns a index of the router list for the given ndp cache entry.
 *
 * \param[in] ctrlIdx               Index of the EthIf controller
 * \param[in] remoteAddrPtr         Pointer to the router address
 *
 * \return Index of the entry in Router List cache
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_findRouterListEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
/** \brief Get matching destination cache entry
 *
 * This function loops over all destination cache entries and compares the passed remote address
 * with the remote address in the cache. If a matching entries is found the matching entry is saved
 * to matchingEntryPtr. If a free entry is found its saved to freeEntryPtr
 *
 * \param[in]  ctrlIdx       Index of the ethernet controller (EthIfCtrlIdx)
 * \param[in]  ipaddr        IP address of the remote host
 *
 * \return - an existing entry for ipaddr if exists
 *         - a free Destination cache entry
 *         - TCPIP_NDP_DESTINATION_CACHE_SIZE no entry found (all are locked)
 *
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_DestCache_getEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr
);

/** \brief Initializes a destination cache entry.
 *
 * This function initializes the destination cache entry identified by the given index.
 *
 * \param[in] ctrlIdx  Index of the ethernet controller (EthIfCtrlIdx)
 * \param[in] entryIdx Index of the destination cache entry
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_initEntry
(
  uint8 ctrlIdx,
  uint8 entryIdx
);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

/** \brief Request release of solicit multicast
 *
 * \param[in]  assignIdx     Index to the router assignment data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_release_Multicast
(
  uint8 assignIdx
);

#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)

/** \brief Enter state OPTIMISTIC
 *
 * \param[in]  localAddrId        Local address id of the router assignment
 * \param[in]  assignPtr          points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Stops the router and prefix controller and duplicate address detection state machines
 *         from state OPTIMISTIC
 *
 * \param[in] localAddrId    Local address id of the link local assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_OPTIMISTIC_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Stops the router and prefix controller and duplicate address detection state machines
 *         from state OPTIMISTIC due to the DAD result
 *
 * \param[in] localAddrId    Local address id of the link local assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_OPTIMISTIC_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Unassigns generated IPv6 address and stops the router and prefix controller
 *         state machine
 *
 * \param[in]  assignIdx     Index to the router assignment data structure
 * \param[in]  localAddrId   Local address id of the router assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_ASSIGNED_CONFLICTED
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */

/** \brief Enter state TENTATIVE
 *
 * \param[in]  localAddrId        Local address id of the router assignment
 * \param[in]  assignPtr          points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Stops the router and prefix controller and duplicate address detection state machines
 *         from state TENTATIVE
 *
 * \param[in] localAddrId    Local address id of the link local assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_TENTATIVE_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Stops the router and prefix controller and duplicate address detection state machines
 *         from state OPTIMISTIC due to the DAD result
 *
 * \param[in] localAddrId    Local address id of the link local assignment
 * \param[in]  assignPtr     points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

/** \brief Assign router address
 *
 * In state DAD. This function assigns the address to the controller by calling
 * TcpIp_IpAddrM_AssignSM_assigned.
 *
 * \param[in] localAddrId   Local address id of the link local assignment
 * \param[in] assignPtr     Pointer to router assignment data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_PREFERRED_DEPRECATED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */

/** \brief Result of Duplicate address detection
 *
 * \param[in] ctrlIdx  Index of the controller on which duplicate address detection
 *                     was performed
 * \param[in] unique   TRUE: address is not used by any other node in the network
 *                     FALSE: duplicate detected
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_RouterAssign_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
);

/** \brief Move router and prefix assignment control data to state Conflicted
 *
 * \param[in]  assignPtr   points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_CONFLICTED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

#else /* TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON */

/** \brief IpAddrM that IPv6 address can be used for transmission/reception
 *
 * \param[in]  localAddrId        Local address id of the router assignment
 * \param[in]  assignPtr          points to the router and prefix assignment control data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_PREFERRED_DEPRECATED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
);

#endif /* TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

STATIC CONST(uint32, TCPIP_CONST) TcpIp_IpV6_unspecified[TCPIP_IPV6_ADDR_SIZE] = {0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/* \brief router and prefix discovery controller data */
STATIC VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_Rdp_Ctrl_Data[TCPIP_NUM_ETHIFCTRL];

/* \brief router and prefix discovery router assignment data */
STATIC VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_Rdp_RouterAssign_Data[TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX];

/* \brief router list database */
STATIC VAR(TcpIp_Ndp_Rpd_RouterList_DataType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_Rdp_RouterList_Data[TCPIP_NUM_ETHIFCTRL][TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE];

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
/* \brief destination cache database */
STATIC VAR(TcpIp_Ndp_DestinationCache_DataType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_DestianationCache_Data[TCPIP_NUM_ETHIFCTRL][TCPIP_NDP_DESTINATION_CACHE_SIZE];
#endif /* TCPIP_NDP_DESTINATION_CACHE_ENABLED */

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

STATIC VAR(uint8, TCPIP_VAR_CLEARED) TcpIp_Ndp_Rdp_RouterList_LastSelected;

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_init(void)
{
  TcpIp_Ndp_Rpd_Ctrl_init();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_mainFunction
(
  uint16 timerTick
)
{
  TcpIp_Ndp_Rpd_Ctrl_mainFunction(timerTick);
  TcpIp_Ndp_Rpd_RouterAssign_mainFunction(timerTick);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_rxRouterAdvert
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* check that ICMP length is 12 or more octets */
  if(lenByte >= TCPIP_NDP_RPD_ROUTER_ADVERT_SIZE_MIN)
  {
    /* remote address must be a link local address */
    if(TCPIP_IPV6_ADDR_IS_LINK_LOCAL(remoteAddrPtr))
    {
      P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optsPtr =
        &dataPtr[TCPIP_NDP_RPD_ROUTER_ADVERT_SIZE_MIN];
      uint16 optsLen = lenByte - TCPIP_NDP_RPD_ROUTER_ADVERT_SIZE_MIN;

      /* verify if options are valid */
      if(TcpIp_Ndp_Rpd_validateOpts(optsPtr, optsLen) == E_OK)
      {
        TcpIp_Ndp_NCache_OptionType options;
        const uint16 routerLifeTime = TCPIP_NDP_RPD_GET_ROUTER_LIFETIME(dataPtr);

        /* process prefixes and source link layer addresses */
        TcpIp_Ndp_Rpd_parseOpts(ctrlIdx, optsPtr, optsLen, &options);

        /* update router and prefix discovery state */
        TcpIp_Ndp_Rpd_Ctrl_rxRouterAdvert(ctrlIdx, routerLifeTime);

        /* update neighbor cache */
        TcpIp_Ndp_NCache_update(ctrlIdx, remoteAddrPtr, options.source_link_addr, TRUE);

        TcpIp_Ndp_Rpd_RouterList_rxRouterAdvert(ctrlIdx, remoteAddrPtr, dataPtr);
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_init
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 prefixIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr;

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);

  assignPtr = &TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx];

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_ENTRY(localAddrId);

  TCPIP_IPV6_SET_UNSPECIFIED_IP(assignPtr->ipAddr);
  assignPtr->valid_lifetime = 0U;
  assignPtr->preferred_lifetime = 0U;
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE;

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  const uint8 prefixIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr;
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  assignPtr = &TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx];

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_ENTRY(localAddrId);

  TS_AtomicAssign8(state, assignPtr->state);

  switch(state)
  {
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:
      TcpIp_Ndp_Rpd_RouterAssign_tr_startAssign_INACTIVE_UNASSIGNED(ctrlIdx, assignPtr);
      break;
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_EXIT(localAddrId);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  const uint8 prefixIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr;
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  assignPtr = &TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx];

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_ENTRY(localAddrId);

  TS_AtomicAssign8(state, assignPtr->state);

  switch(state)
  {
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
        TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_OPTIMISTIC_INACTIVE(localAddrId, assignPtr);
      break;
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
        TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_TENTATIVE_INACTIVE(localAddrId, assignPtr);
      break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /* TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:
      /* ignore */
      break;
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:
      TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_UNASSIGNED_INACTIVE(ctrlIdx, assignPtr);
      break;
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:
      TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_ASSIGNED_INACTIVE
        (ctrlIdx, prefixIdx, localAddrId, assignPtr);
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  const uint8 prefixIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  TcpIp_IpAddrConstPtr netMaskPtr = TcpIp_IPV6_Prefix_Mask;
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_ENTRY
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  /* get generated address for the address with router assignment */
  *addressIpPtrPtr = TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx].ipAddr;
  /* Get IpV6 address of the router from the router list */
  TcpIp_Ndp_Rpd_getDefaultRouterAddress(gatewayIpPtrPtr,ctrlIdx);

  *netmaskIpPtrPtr = netMaskPtr;

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_EXIT
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  const uint8 prefixIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_ENTRY(localAddrId,addressIpPtrPtr);

  /* get generated address for the address with router assignment */
  *addressIpPtrPtr = TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx].ipAddr;

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_EXIT(localAddrId,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
)
{
  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);
  /* this is a dummy function */
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(state);
  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_ENTRY(localAddrId,LocalIpAddrPtr);
  /* this is a dummy function */
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(LocalIpAddrPtr);
  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_EXIT(E_OK,localAddrId,LocalIpAddrPtr);
  return E_OK;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getIpAddrState
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 state;
  const uint8 prefixIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr;

  TCPIP_PRECONDITION_ASSERT(prefixIdx < TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX, TCPIP_INTERNAL_API_ID);

  assignPtr = &TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx];

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_ENTRY(localAddrId);

  if
  (
    (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE) ||
    (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED)
  )
  {
    state = TCPIP_IPADDRM_ADDRSTATE_INVALID;
  }
  else
  {
    state = assignPtr->state;
  }

  DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_EXIT(state,localAddrId);

  return state;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_remove
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ipAddr
)
{
  uint8 routerListIdx;

  /* Initialize the entry that has the correct ip address */
  for(routerListIdx = 0U; routerListIdx < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; routerListIdx++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListDataPtr =
        &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListIdx];

    if(TCPIP_IPV6_ADDR_CMP(routerListDataPtr->ipAddr, ipAddr))
    {
      TcpIp_Ndp_Rpd_RouterList_entry_UNASSIGNED(routerListDataPtr,ctrlIdx);
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_getDefaultRouterAddress
(
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  uint8 ctrlIdx
)
{
  uint8 i;
  uint8 firstReachableRouter = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;
  uint8 firstUneachableRouter = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;
  uint8 nextUnreachableRouter = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;
  uint8 selectedRouter = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;

  DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_ENTRY(gatewayIpPtrPtr,ctrlIdx);

  /* Return address of the first used entry */
  for(i = 0U; i < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; i++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListDataPtr =
        &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][i];

    if(routerListDataPtr->state == TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED)
    {
      /* first check if router is reachable */
      if(TcpIp_Ndp_NCache_EntryIsReachable(ctrlIdx,routerListDataPtr->ipAddr))
      {
        if(firstReachableRouter == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
        {
          /* select the first reachable router we find */
          firstReachableRouter = i;
          selectedRouter = i;
        }

        if(i > TcpIp_Ndp_Rdp_RouterList_LastSelected)
        {
          /* check if router was already selected last time if so select next router in round robin fashion */
          selectedRouter = i;
          break;
        }
      }

      /* if we haven't found any reachable routers, select an unreachable router */
      if(firstReachableRouter == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
      {
        if(firstUneachableRouter == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
        {
          /* select the first unreachable router we find */
          firstUneachableRouter = i;
          selectedRouter = i;
        }

        if((i > TcpIp_Ndp_Rdp_RouterList_LastSelected) && (nextUnreachableRouter == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE))
        {
          /* check if router was already selected last time if so select next router in round robin fashion */
          nextUnreachableRouter = i;
          selectedRouter = i;
        }
      }
    }
  }

  if(selectedRouter != TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
  {
    *gatewayIpPtrPtr = TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][selectedRouter].ipAddr;
    TcpIp_Ndp_Rdp_RouterList_LastSelected = selectedRouter;
  }
  else
  {
    *gatewayIpPtrPtr = TcpIp_IpV6_unspecified;
  }

  DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_EXIT(gatewayIpPtrPtr,ctrlIdx);
}

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_init(void)
{
  uint8 entryIdx;
  uint8_least ctrlIdx;

  /* go through all interfaces */
  for(ctrlIdx = 0U; ctrlIdx < TCPIP_NUM_ETHIFCTRL; ++ctrlIdx)
  {
#if(TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U)
    for(entryIdx = 0U; entryIdx < TCPIP_NDP_DESTINATION_CACHE_SIZE; entryIdx++)
#else /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
    entryIdx = 0U;
#endif /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
    {
      TcpIp_Ndp_DestCache_initEntry((uint8)ctrlIdx,entryIdx);
      TcpIp_Ndp_DestianationCache_Data[ctrlIdx][entryIdx].lock = 0U;
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_removeNextHopEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
)
{
  uint8 entryIdx;
  /* !LINKSTO TcpIp.Design.Ndp.EA1.mainFunction,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
#if(TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U)
  for(entryIdx = 0U; entryIdx < TCPIP_NDP_DESTINATION_CACHE_SIZE; entryIdx++)
#else /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
  entryIdx = 0U;
#endif /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
  {
    P2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
            &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][entryIdx];
    if(TCPIP_IPV6_ADDR_CMP(destinationCacheDataPtr->nextHopIpAddr, ipAddrPtr))
    {
      TcpIp_Ndp_DestCache_initEntry(ctrlIdx,entryIdx);
    }
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.mainFunction,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
}

#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_DestCache_getNextHopAddrAndLock
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  uint8 ctrlIdx,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) destCacheHandlePtr,
  P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) nextHopIpPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) networkMaskPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(ctrlIdx);
  TS_PARAM_UNUSED(destCacheHandlePtr);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_OFF) */

  DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_ENTRY
    (remoteAddrPtr,localIpAddrPtr,ctrlIdx,destCacheHandlePtr,nextHopIpPtr,networkMaskPtr);

  /* !LINKSTO TcpIp.Design.Ndp.EA1.getNextHopAddrAndLock,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)

  *destCacheHandlePtr = TcpIp_Ndp_DestCache_getEntry(ctrlIdx,remoteAddrPtr);

  if(*destCacheHandlePtr != TCPIP_NDP_DESTINATION_CACHE_SIZE)
  {
    CONSTP2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
      &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][*destCacheHandlePtr];

    if(destinationCacheDataPtr->state == TCPIP_NDP_DESTINATION_CACHE_STATE_ASSIGNED)
    {
      *nextHopIpPtr = destinationCacheDataPtr->nextHopIpAddr;
      destinationCacheDataPtr->lock++;
      retVal = TCPIP_OK;
    }
  }
  if(retVal == TCPIP_E_NOT_OK)
#endif
  {
    boolean match = TRUE;
    uint8 i;

    /* verify if prefixes match */
    for(i = 0; i < TCPIP_IPV6_ADDR_SIZE; i++)
    {
      const uint32 localNetworkId = localIpAddrPtr[i] & networkMaskPtr[i];
      const uint32 destinationNetworkId = remoteAddrPtr[i] & networkMaskPtr[i];

      if(localNetworkId != destinationNetworkId)
      {
        match = FALSE;
        break;
      }
    }

    if(match == TRUE)
    {
      /* prefixes match next hop is the remote host */
      *nextHopIpPtr = remoteAddrPtr;
    }
    else if(TCPIP_IPV6_ADDR_IS_LINK_LOCAL(remoteAddrPtr))
    {
      /* if remote address is link local always sent message to remote host
         (messages containing link local addresses are not forwarded by the router) */
      *nextHopIpPtr = remoteAddrPtr;
    }
    else if(TCPIP_IPV6_ADDR_IS_LINK_LOCAL(localIpAddrPtr))
    {
      /* if local address is link local always sent message to remote host
         (messages containing link local addresses are not forwarded by the router) */
      *nextHopIpPtr = remoteAddrPtr;
    }
    else
    {
      /* remote host not in local network, send to router if router exists or is configured */
    }

    /* fill destination cache with either unicast address or default router */
    /* getting default router was already done in TcpIp_IpAddrM_getIpConfig */
    /* check if next hop is valid, it might be that there is no router configured */
    if(!TCPIP_IPV6_ADDR_IS_UNSPECIFIED((*nextHopIpPtr)))
    {
#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
      /* check if there is still space left in the destination cache */
      if(*destCacheHandlePtr != TCPIP_NDP_DESTINATION_CACHE_SIZE)
      {
        CONSTP2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
          &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][*destCacheHandlePtr];
        destinationCacheDataPtr->state = TCPIP_NDP_DESTINATION_CACHE_STATE_ASSIGNED;
        TCPIP_IPV6_ADDR_CPY(destinationCacheDataPtr->remoteIpAddr,remoteAddrPtr);
        TCPIP_IPV6_ADDR_CPY(destinationCacheDataPtr->nextHopIpAddr,*nextHopIpPtr);
        destinationCacheDataPtr->lock++;
      }
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */
      retVal = TCPIP_OK;
    }
    else
    {
      retVal = TCPIP_E_NOT_OK;
    }
  }

  /* !LINKSTO TcpIp.Design.Ndp.EA1.getNextHopAddrAndLock,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_EXIT
    (retVal,remoteAddrPtr,localIpAddrPtr,ctrlIdx,destCacheHandlePtr,nextHopIpPtr,networkMaskPtr);

  return retVal;
}

#if(TCPIP_NDP_RPD_ENABLED == STD_ON)

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_unlock
(
  uint16 destCacheHandlePtr,
  uint8 ctrlIdx
)
{
  DBG_TCPIP_NDP_DESTCACHE_UNLOCK_ENTRY(destCacheHandlePtr,ctrlIdx);
  /* !LINKSTO TcpIp.Design.Ndp.EA1.destCacheUnlock,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  if(destCacheHandlePtr != TCPIP_NDP_DESTINATION_CACHE_SIZE)
  {
    P2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
        &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][destCacheHandlePtr];
    TCPIP_PRECONDITION_ASSERT((destinationCacheDataPtr->lock != 0U), TCPIP_INTERNAL_API_ID);
    destinationCacheDataPtr->lock--;
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.destCacheUnlock,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  DBG_TCPIP_NDP_DESTCACHE_UNLOCK_EXIT(destCacheHandlePtr,ctrlIdx);
}
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_validateOpts
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  Std_ReturnType result = E_NOT_OK;
  uint8 optionType;
  uint16 optionLength = 0U;
  uint32 i = 0U;

  if(lenByte == 0U)
  {
    result = E_OK;
  }
  else
  {
    /* go through all options and look for known ones */
    do
    {
      const uint16 leftLen = lenByte - (uint16)i;

      if(leftLen >= TCPIP_NDP_OPT_HDR_LEN)
      {
        optionType = TCPIP_NDP_RPD_GET_OPTION_TYPE(&dataPtr[i]);
        optionLength = TCPIP_NDP_RPD_GET_OPTION_LENGTH(&dataPtr[i]);

        /* option must have a length greater zero */
        if(optionLength == 0U)
        {
          result = E_NOT_OK;
        }
        else
        {
          switch(optionType)
          {
            case TCPIP_NDP_RPD_OPTION_PREFIX_INFORMATION:
            {
              if((optionLength != TCPIP_NDP_RPD_PREFIX_OPT_LEN) ||
                 (leftLen < TCPIP_NDP_RPD_PREFIX_OPT_LEN)
                )
              {
                /* if prefix information option is incorrect, discard the solicitation */
                result = E_NOT_OK;
              }
              else
              {
                result = E_OK;
              }
              break;
            }
            case TCPIP_NDP_RPD_OPTION_SOURCE_LINK_LAYER_ADDR:
            {
              if((optionLength != TCPIP_NDP_RPD_SOURCE_LINK_LAYER_OPT_HEADER_LEN_BYTES) ||
                 (leftLen < TCPIP_NDP_RPD_SOURCE_LINK_LAYER_OPT_HEADER_LEN_BYTES))
              {
                /* if source link layer option is incorrect, discard the solicitation */
                result = E_NOT_OK;
              }
              else
              {
                if(TcpIp_CheckMacNotMulticast(TCPIP_NDP_OPT_LL_ADDR_GET_PTR(&dataPtr[i])) == E_OK)
                {
                  result = E_OK;
                }
                else
                {
                  result = E_NOT_OK;
                }
              }
              break;
            }
            default:
            {
              /* unknown option type, ignore */
              result = E_OK;
              break;
            }
          }
        }
      }
      else
      {
        /* invalid option length */
        result = E_NOT_OK;
        break;
      }

      i += optionLength;
    }while ((i < lenByte) && (result == E_OK));
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_parseOpts
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2VAR(TcpIp_Ndp_NCache_OptionType, AUTOMATIC, TCPIP_APPL_DATA) options
)
{
  uint16 optionLength = 0U;
  uint32 i;

  options->source_link_addr = NULL_PTR;

  /* go through all options and look for known ones */
  for(i = 0; i < lenByte; i = i + optionLength)
  {
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optionPtr = &dataPtr[i];
    uint8 optionType = TCPIP_NDP_RPD_GET_OPTION_TYPE(optionPtr);

    optionLength = TCPIP_NDP_RPD_GET_OPTION_LENGTH(optionPtr);

    switch(optionType)
    {
      case TCPIP_NDP_RPD_OPTION_PREFIX_INFORMATION:
      {
        uint8 flags = TCPIP_NDP_RPD_PREFIX_OPTION_GET_FLAGS(optionPtr);

        /* check if autonomous address-configuration flag is set */
        if(((flags & TCPIP_NDP_RPD_PREFIX_OPT_FLAG_A) != 0U) &&
           ((flags & TCPIP_NDP_RPD_PREFIX_OPT_FLAG_L) != 0U)
          )
        {
          uint32 valid_lifetime = TCPIP_NDP_RPD_PREFIX_OPTION_GET_VALID_LIFETIME(optionPtr);
          uint32 preferred_lifetime = TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFERRED_LIFETIME(optionPtr);

          /* Preferred life time has to be smaller or equal to valid life time */
          if(preferred_lifetime <= valid_lifetime)
          {
            uint8 prefix_length = TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFIX_LENGTH(optionPtr);

            /* Interface identifier is 64 bits long therefore prefix should also be 64 bits long */
            if(prefix_length == 64U)
            {
              uint32 prefixAddr[TCPIP_IPV6_ADDR_SIZE];
              TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD
                (prefixAddr, TCPIP_NDP_RPD_PREFIX_OPTION_GET_PREFIX_PTR(optionPtr));

              /* ignore link local prefix, multicasts and unspecified addresses */
              if((!TCPIP_IPV6_ADDR_IS_LINK_LOCAL(prefixAddr)) &&
                 (!TCPIP_IPV6_ADDR_IS_MULTICAST(prefixAddr)) &&
                 (!TCPIP_IPV6_ADDR_IS_UNSPECIFIED(prefixAddr))
                )
              {
                TcpIp_Ndp_Rpd_RouterAssign_processPrefix
                  (ctrlIdx, prefixAddr, valid_lifetime, preferred_lifetime);
              }
            }
          }
        }
        break;
      }
      case TCPIP_NDP_RPD_OPTION_SOURCE_LINK_LAYER_ADDR:
      {
        options->source_link_addr = TCPIP_NDP_OPT_LL_ADDR_GET_PTR(optionPtr);
        break;
      }
      default:
      {
        /* unknown option type, ignore */
        break;
      }
    }
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_Rpd_transmitRouterSolicit
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  boolean useUnspecifiedSrc = TRUE;

  /* only transmit router solicitation if link local assignment is running */
  if(TcpIp_IpAddrM_lockIfStartupOnlineShutdown(localAddrId))
  {
    const TcpIp_StateType ifState = TcpIp_IpAddrM_getIfState(localAddrId);
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    uint8 ipAddrState = TCPIP_IPADDRM_ADDRSTATE_INVALID;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)*/
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)*/
    uint16 msgLength = TCPIP_NDP_RPD_ROUTER_SOLIC_SIZE_MIN;
    TcpIp_SockAddrInet6Type remoteIpAddr = {TCPIP_AF_INET6, 0U, {0U}};

    /* send packet to all routers multicast address */
    TcpIp_Ipv6_getAllRoutersMulticastIpAddr(remoteIpAddr.addr);

    if(ifState == TCPIP_STATE_ONLINE)
    {
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
      ipAddrState = TcpIp_IpAddrM_getIpAddrState(localAddrId);

      if(ipAddrState != TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC)
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)*/
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)*/
    /* append link layer address if link local address is assigned */
      {
        msgLength += TCPIP_NDP_OPT_LINK_ADDR_LEN + TCPIP_NDP_OPT_HDR_LEN;
      }
      useUnspecifiedSrc = FALSE;
    }

    {
      uint16 remoteAddrHandle = TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST;
      TcpIp_Ip_AddrType ipAddr;
      uint8 type = TCPIP_NDP_ICMP_TYPE_ROUTER_SOLICIT;
      boolean addEthAddr = FALSE;

      if(ifState == TCPIP_STATE_ONLINE)
      {
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
        if(ipAddrState != TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC)
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)*/
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)*/
        {
          addEthAddr = TRUE;
        }
      }
      ipAddr.localAddrId = localAddrId;
      /* Deviation MISRAC2012-1 */
      ipAddr.remoteAddrPtr = (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) &remoteIpAddr;

      result = TcpIp_Ndp_IntTransmit( &ipAddr,
                                      remoteAddrHandle,
                                      NULL_PTR,
                                      msgLength,
                                      type,
                                      0U,
                                      addEthAddr,
                                      useUnspecifiedSrc
                                    );
    }

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_init(void)
{
  uint8_least ctrlIdx;

  /* go through all interfaces */
  for(ctrlIdx = 0U; ctrlIdx < TCPIP_NUM_ETHIFCTRL; ++ctrlIdx)
  {
    P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
        &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];

    ctrlDataPtr->state = TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED;
    ctrlDataPtr->timer = 0U;
    ctrlDataPtr->RTR_Counter = 0U;
    ctrlDataPtr->additionalAssignCount = 0U;

    TcpIp_Ndp_Rpd_RouterList_ctrlInit((uint8)ctrlIdx);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_mainFunction
(
  uint16 timerTick
)
{
  uint8_least ctrlIdx;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_1S) == TCPIP_TIMER_OFFSET_1S_NDP_RPD_TABLE)
  {
    /* go through all interfaces */
    for (ctrlIdx = 0U; ctrlIdx < TCPIP_NUM_ETHIFCTRL; ctrlIdx++)
    {
      P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
          &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];

      /* handle router list timeouts */
      TcpIp_Ndp_Rpd_RouterList_timeout((uint8)ctrlIdx);

      if(ctrlDataPtr->timer > 0U)
      {
        ctrlDataPtr->timer--;

        if(ctrlDataPtr->timer == 0U)
        {
          TcpIp_Ndp_Rpd_Ctrl_timeout((uint8)ctrlIdx);
        }
      }
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_startAssign
(
  uint8 ctrlIdx
)
{
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
      &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];
  uint8 state;

  TS_AtomicAssign8(state, ctrlDataPtr->state);

  switch(state)
  {
    case TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED:
    {
      /* If router retransmission solicitation delay is enabled, set it to random number
         between 0 and wanted delay, otherwise set it to wanted delay */
#if(TCPIP_NDP_RND_RTR_SOLICIT_DELAY_ENABLED == STD_ON)
      ctrlDataPtr->timer = (uint8)TcpIp_GetRandomRange(TCPIP_NDP_MAX_RTR_SOLICITATION_DELAY);
#else /* TCPIP_NDP_RPD_RTR_SOLICIT_DELAY_ENABLED == STD_ON */
      ctrlDataPtr->timer = TCPIP_NDP_MAX_RTR_SOLICITATION_DELAY;
#endif /* TCPIP_NDP_RPD_RTR_SOLICIT_DELAY_ENABLED == STD_ON */
      ctrlDataPtr->RTR_Counter = 0U;

      /* enable the default router list */
      TcpIp_Ndp_Rpd_RouterList_startAssign(ctrlIdx);

      if(ctrlDataPtr->timer > 0U)
      {
        /* TcpIp_Ndp_Rpd_Ctrl_tr_startAssign_UNASSIGNED_START_DELAY */
        ctrlDataPtr->state = TCPIP_NDP_RPD_CTRL_STATE_START_DELAY;
      }
      else
      {
        /* TcpIp_Ndp_Rpd_Ctrl_tr_startAssign_UNASSIGNED_PROBING */
        TcpIp_Ndp_Rpd_Ctrl_entry_PROBING(ctrlIdx);
      }
      break;
    }
    case TCPIP_NDP_RPD_CTRL_STATE_START_DELAY:   /* intended fall through */
    case TCPIP_NDP_RPD_CTRL_STATE_PROBING:       /* intended fall through */
    case TCPIP_NDP_RPD_CTRL_STATE_LISTEN:
      /* if router and prefix discovery is already running, increment
         assignment counter */
      ctrlDataPtr->additionalAssignCount++;
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_stopAssign
(
  uint8 ctrlIdx
)
{
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
      &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];
  uint8 state;

  TS_AtomicAssign8(state, ctrlDataPtr->state);

  switch(state)
  {
    case TCPIP_NDP_RPD_CTRL_STATE_START_DELAY:     /* intended fall through */
    case TCPIP_NDP_RPD_CTRL_STATE_PROBING:         /* intended fall through */
    case TCPIP_NDP_RPD_CTRL_STATE_LISTEN:
      if(ctrlDataPtr->additionalAssignCount > 0U)
      {
        /* more than one router assignment is running, decrement assignment counter */
        ctrlDataPtr->additionalAssignCount--;
      }
      else
      {
        /* last router assignment is shut down, shut down router and prefix discovery */
        TcpIp_Ndp_Rpd_Ctrl_entry_UNASSIGNED(ctrlDataPtr,ctrlIdx);
      }
      break;
    /* CHECK: NOPARSE */
    case TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED:
      /* router and prefix discovery is already shut down, function will not be called by
       * TcpIp_Ndp_Rpd_RouterAssign_stopAssign */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_timeout
(
  uint8 ctrlIdx
)
{
  CONSTP2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
    &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];

  /* Timeout for the router and prefix discovery ctrl state machine */
  switch(ctrlDataPtr->state)
  {
    case TCPIP_NDP_RPD_CTRL_STATE_START_DELAY:
      TcpIp_Ndp_Rpd_Ctrl_entry_PROBING(ctrlIdx);
      break;
    case TCPIP_NDP_RPD_CTRL_STATE_PROBING:
      if(ctrlDataPtr->RTR_Counter < TCPIP_NDP_MAX_RTR_SOLICITATIONS)
      {
        /* retransmit solicitation if number of maximal transmission has not been received yet */
        TcpIp_Ndp_Rpd_Ctrl_entry_PROBING(ctrlIdx);
      }
      break;
    /* CHECK: NOPARSE */
    case TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED:      /* intended fall through */
    case TCPIP_NDP_RPD_CTRL_STATE_LISTEN:          /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_rxRouterAdvert
(
  uint8 ctrlIdx,
  uint16 routerLifetime
)
{
  CONSTP2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr =
    &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];

  switch(ctrlDataPtr->state)
  {
    case TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED:
      /* ignore router advertisements, if router and prefix discovery is not running */
      break;
    case TCPIP_NDP_RPD_CTRL_STATE_START_DELAY:
      if(routerLifetime != 0U)
      {
        TcpIp_Ndp_Rpd_Ctrl_tr_rxRouterAdvert_START_DELAY_LISTEN(ctrlIdx, ctrlDataPtr);
      }
      break;
    case TCPIP_NDP_RPD_CTRL_STATE_PROBING:
      if(routerLifetime != 0U)
      {
        TcpIp_Ndp_Rpd_Ctrl_entry_LISTEN(ctrlDataPtr);
      }
      break;
    case TCPIP_NDP_RPD_CTRL_STATE_LISTEN:
      /* ignore */
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_tr_rxRouterAdvert_START_DELAY_LISTEN
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr
)
{
  /* exit code */

  /* transitional code */
  {
    /* Transmit solicitation only if number of solicitation retransmission is larger than 0 */
    if(TCPIP_NDP_MAX_RTR_SOLICITATIONS > 0U)
    {
      const TcpIp_LocalAddrIdType localAddrId =
        TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV6.localAddrLinkLocal;

      (void) TcpIp_Ndp_Rpd_transmitRouterSolicit(localAddrId);
    }
  }

  /* entry code */
  TcpIp_Ndp_Rpd_Ctrl_entry_LISTEN(ctrlDataPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_PROBING
(
  uint8 ctrlIdx
)
{
  CONSTP2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) entry =
    &TcpIp_Ndp_Rdp_Ctrl_Data[ctrlIdx];
  TcpIp_LocalAddrIdType localAddrId = TCPIP_CFG(ctrl,ctrlIdx).ctrlIpV6.localAddrLinkLocal;

  if (TCPIP_NDP_MAX_RTR_SOLICITATIONS > 0U)
  {
    (void) TcpIp_Ndp_Rpd_transmitRouterSolicit(localAddrId);
  }

  entry->RTR_Counter++;
  entry->timer = TCPIP_NDP_RTR_SOLICITATION_INTERVAL;
  entry->state = TCPIP_NDP_RPD_CTRL_STATE_PROBING;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_LISTEN
(
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr
)
{

  ctrlDataPtr->timer = 0U;
  ctrlDataPtr->RTR_Counter = 0U;
  ctrlDataPtr->state = TCPIP_NDP_RPD_CTRL_STATE_LISTEN;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_Ctrl_entry_UNASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_Ctrl_DataType, AUTOMATIC, TCPIP_APPL_DATA) ctrlDataPtr,
  uint8 ctrlIdx
)
{

  ctrlDataPtr->timer = 0U;
  ctrlDataPtr->RTR_Counter = 0U;
  ctrlDataPtr->state = TCPIP_NDP_RPD_CTRL_STATE_UNASSIGNED;
  TcpIp_Ndp_Rpd_RouterList_stopAssign(ctrlIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_mainFunction
(
  uint16 timerTick
)
{
  uint8_least prefixIdx;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_1S) == TCPIP_TIMER_OFFSET_1S_NDP_RPD_TABLE)
  {
    /* go through all entries */
    for(prefixIdx = 0; prefixIdx < TcpIp_PBcfgPtr->ndpRpdNum; ++prefixIdx)
    {
      P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr =
          &TcpIp_Ndp_Rdp_RouterAssign_Data[prefixIdx];

      if
      (
        (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED) ||
        (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED)
      )
      {
        /* decrement preferred lifetime if its not infinite and zero */
        if((assignPtr->preferred_lifetime != 0xFFFFFFFFU) && (assignPtr->preferred_lifetime > 0U))
        {
          assignPtr->preferred_lifetime--;
          /* if preferred life time is 0, the address shall become deprecated */
          TcpIp_Ndp_Rpd_RouterAssignTimeout_PREFERRED_DEPRECATED(assignPtr->preferred_lifetime,assignPtr->state);
        }

        /* decrement valid lifetime if its not infinite and zero */
        if((assignPtr->valid_lifetime != 0xFFFFFFFFU) && (assignPtr->valid_lifetime > 0U))
        {
           assignPtr->valid_lifetime--;

           if(assignPtr->valid_lifetime == 0U)
           {
             const TcpIp_LocalAddrIdType localAddrId =
               TcpIp_IpAddrM_getLocalAddrId(prefixIdx , TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);

             TcpIp_Ndp_Rpd_RouterAssign_timeout((uint8)prefixIdx, localAddrId, assignPtr);
           }
        }
      }
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_processPrefix
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
)
{
  /* check router assignment list for existing prefix */
  uint8 assignIdx = TcpIp_Ndp_Rpd_RouterAssign_getEntry(ctrlIdx, prefixAddrPtr);

  /* no matching prefix found or no place left for a new prefix in the router assignment list */
  if(assignIdx != TcpIp_PBcfgPtr->ndpRpdNum)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr =
      &TcpIp_Ndp_Rdp_RouterAssign_Data[assignIdx];
    const TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpAddrM_getLocalAddrId(assignIdx , TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);

    switch(assignPtr->state)
    {
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:
      {
        /* no matching prefix found, free entry left, add prefix if valid life time is greater 0 */
        if(validLifeTime != 0U)
        {
          TcpIp_Ndp_Rpd_RouterAssign_start_address_assignment
              (ctrlIdx, assignIdx, localAddrId, assignPtr, prefixAddrPtr, validLifeTime, preferredLifeTime);
        }
        break;
      }
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
        /* matching prefix found, update life times */
        TcpIp_Ndp_Rpd_RouterAssign_tr_processPrefix_ASSIGNED_ASSIGNED
          (assignPtr, validLifeTime, preferredLifeTime);
        break;
      /* CHECK: NOPARSE */
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:
      case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:
      default:
       /* should not happen */
       TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
       break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_timeout
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  switch(assignPtr->state)
  {
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:
      TcpIp_Ndp_Rpd_RouterAssign_tr_timeout_PREFERRED_INACTIVE
        (ctrlIdx, assignIdx, localAddrId, assignPtr);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
    default:
     /* should not happen */
     TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
     break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_getEntry
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr
)
{
  uint8_least i;
  uint8_least entryIdx = TcpIp_PBcfgPtr->ndpRpdNum;
  uint8_least entryIdxFree = TcpIp_PBcfgPtr->ndpRpdNum;
  boolean isFound = FALSE;

  /* go through all entries */
  for(i = 0; i < (TcpIp_PBcfgPtr->ndpRpdNum) && (isFound == FALSE); ++i)
  {
    const TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
    const uint8 assignCtrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);

    TCPIP_PRECONDITION_ASSERT(assignCtrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    if(ctrlIdx == assignCtrlIdx)
    {
      P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr =
          &TcpIp_Ndp_Rdp_RouterAssign_Data[i];

      switch(assignPtr->state)
      {
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
#else /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:
          /* verify if prefix match with prefix of entry */
          if(TS_MemCmp(assignPtr->ipAddr, prefixAddrPtr, TCPIP_NDP_RPD_DEAULT_PREFIX_LEN) == E_OK)
          {
            entryIdx = i;
            isFound = TRUE;
          }
          break;
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:
          /* verify if prefix match with prefix of entry */
          if(TS_MemCmp(assignPtr->ipAddr, prefixAddrPtr, TCPIP_NDP_RPD_DEAULT_PREFIX_LEN) == E_OK)
          {
            entryIdx = TcpIp_PBcfgPtr->ndpRpdNum;
            entryIdxFree = TcpIp_PBcfgPtr->ndpRpdNum;
            isFound = TRUE;
          }
          break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:
            /* beside the existing entry, try to find a free entry */
            if(entryIdxFree == TcpIp_PBcfgPtr->ndpRpdNum)
            {
              /* yeah it is available - stop further searching */
              entryIdxFree = i;
            }
            break;
        case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:
            break;
        /* CHECK: NOPARSE */
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }
    }
  }

  if(entryIdx == TcpIp_PBcfgPtr->ndpRpdNum)
  {
    /* if no matching prefix was found, check if free entry exists */
    if(entryIdxFree != TcpIp_PBcfgPtr->ndpRpdNum)
    {
      /* there is an empty entry */
      entryIdx = entryIdxFree;
    }
  }

  return (uint8)entryIdx;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_startAssign_INACTIVE_UNASSIGNED
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED;
  TcpIp_Ndp_Rpd_Ctrl_startAssign(ctrlIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_ASSIGNED_INACTIVE
(
  uint8 ctrlIdx,
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  /* exit code */
  TcpIp_Ndp_Rpd_RouterAssign_exit_ASSIGNED(assignIdx, localAddrId);

  /* transitional code */

  /* entry code */
  TcpIp_Ndp_Rpd_RouterAssign_entry_INACTIVE(ctrlIdx, assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_UNASSIGNED_INACTIVE
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_Ndp_Rpd_RouterAssign_entry_INACTIVE(ctrlIdx, assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_processPrefix_ASSIGNED_ASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
)
{
  /* exit code */

  /* transitional code */
  {
    /* If the advertised prefix is equal to the prefix of an address
      configured by stateless autoconfiguration in the list, the
      preferred lifetime of the address is reset to the Preferred
      Lifetime */
    assignPtr->preferred_lifetime = preferredLifeTime;
#if(TCPIP_NDP_DAD_CONFIG_ENABLED==STD_ON)
    /* only when state is TENTATIVE or OPTIMISTIC */
    if
    (
      (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED) ||
      (assignPtr->state == TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED)
    )
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED==STD_ON) */
    {
      assignPtr->state = TCPIP_NDP_RPD_PREFERRED_DEPRECATED(preferredLifeTime);
    }

    if((validLifeTime > assignPtr->valid_lifetime) ||
       (validLifeTime > TCPIP_NDP_RPD_2_HOURS_IN_SECONDS)
      )
    {
      assignPtr->valid_lifetime = validLifeTime;
    }
    else if(assignPtr->valid_lifetime <= TCPIP_NDP_RPD_2_HOURS_IN_SECONDS)
    {
       /* ignore prefix option */
    }
    else
    {
      assignPtr->valid_lifetime = TCPIP_NDP_RPD_2_HOURS_IN_SECONDS;
    }
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_start_address_assignment
(
  uint8 ctrlIdx,
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) prefixAddrPtr,
  uint32 validLifeTime,
  uint32 preferredLifeTime
)
{

  uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
    TcpIp_SockAddrInet6Type solicitMulticast;
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) solicitMulticastPtr =
    /* Deviation MISRAC2012-1 */
    (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST)) &solicitMulticast;
  CONSTP2CONST(TcpIp_NdpRpdAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_NdpRpdAddrIpV6ConfigType,intIpV6NdpRpdAddr,assignIdx));
  const TcpIp_LocalAddrIdType multicastlocalAddrId = intLocalAddrPtr->solMultilocalAddrId;

  assignPtr->valid_lifetime = validLifeTime;
  assignPtr->preferred_lifetime = preferredLifeTime;

  /* retrieve link layer of controller */
  EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

  /* generates link-local address */
  TcpIp_IpV6_generateIp(assignPtr->ipAddr, ethAddr, prefixAddrPtr);

  /* assign solicit multicast address */
  TcpIp_IpV6_getSolicitMulti(solicitMulticast.addr, assignPtr->ipAddr);
  solicitMulticast.domain = TCPIP_AF_INET6;

  /* assign solicited multicast ip address */
  (void) TcpIp_IpAddrM_requestIpAddrAssignment
           (multicastlocalAddrId, TCPIP_IPADDR_ASSIGNMENT_STATIC, solicitMulticastPtr);

#if(TCPIP_NDP_DAD_CONFIG_ENABLED==STD_OFF)
  TcpIp_Ndp_Rpd_RouterAssign_entry_PREFERRED_DEPRECATED(localAddrId,assignPtr);
#else
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
  TcpIp_Ndp_Rpd_RouterAssign_entry_OPTIMISTIC(localAddrId,assignPtr);
#else/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
  TcpIp_Ndp_Rpd_RouterAssign_entry_TENTATIVE(localAddrId,assignPtr);
#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif/*(TCPIP_NDP_DAD_CONFIG_ENABLED==STD_OFF)*/
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_exit_ASSIGNED
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* unassign solicited multicast ip address */
  TcpIp_Ndp_Rpd_RouterAssign_release_Multicast(assignIdx);

  /* inform IpAddrM that address shall not be used any longer */
  TcpIp_IpAddrM_AssignSM_unassigned(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_INACTIVE
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE;
  assignPtr->preferred_lifetime = 0U;
  assignPtr->valid_lifetime = 0U;

  TCPIP_IPV6_SET_UNSPECIFIED_IP(assignPtr->ipAddr);

  TcpIp_Ndp_Rpd_Ctrl_stopAssign(ctrlIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_ctrlInit
(
  uint8 ctrlIdx
)
{
  uint8 routerListIdx;

  for(routerListIdx = 0U; routerListIdx < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; routerListIdx++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListEntryPtr =
      &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListIdx];

    routerListEntryPtr->valid_lifetime = 0U;
    TCPIP_IPV6_SET_UNSPECIFIED_IP(routerListEntryPtr->ipAddr);
    routerListEntryPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE;
  }

  TcpIp_Ndp_Rdp_RouterList_LastSelected = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_startAssign
(
  uint8 ctrlIdx
)
{
  uint8 routerListIdx;

  for(routerListIdx = 0U; routerListIdx < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; routerListIdx++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListEntryPtr =
      &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListIdx];

    switch(routerListEntryPtr->state)
    {
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE:
        routerListEntryPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED;
        break;
      /* CHECK: NOPARSE */
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED:
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED:
        /* router list already enabled, function will not be called by
         * TcpIp_Ndp_Rpd_RouterAssign_startAssign */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_stopAssign
(
  uint8 ctrlIdx
)
{
  uint8 routerListIdx;

  for(routerListIdx = 0U; routerListIdx < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; routerListIdx++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListEntryPtr =
      &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListIdx];

    switch(routerListEntryPtr->state)
    {
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED:
          /* There is no address so only state should be set to INACTIVE*/
          routerListEntryPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE;
        break;
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED:
#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
        TcpIp_Ndp_DestCache_removeNextHopEntry(ctrlIdx,routerListEntryPtr->ipAddr);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */
        routerListEntryPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE;
        routerListEntryPtr->valid_lifetime = 0U;
        TCPIP_IPV6_SET_UNSPECIFIED_IP(routerListEntryPtr->ipAddr);
        break;
      /* CHECK: NOPARSE */
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE:
        /* router list already disabled, function will not be called by
         * TcpIp_Ndp_Rpd_RouterAssign_stopAssign */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_rxRouterAdvert
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr
)
{
  uint8 routerListEntryIdx = TcpIp_Ndp_Rpd_RouterList_findRouterListEntry(ctrlIdx,remoteAddrPtr);
  uint32 valid_lifetime = TCPIP_NDP_RPD_GET_ROUTER_LIFETIME(dataPtr);

  /* If entry doesn't exist and valid lifetime is greater than 0 then create a new one */
  if(routerListEntryIdx != TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListEntryPtr =
      &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListEntryIdx];

    switch(routerListEntryPtr->state)
    {
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED:
        /* no entry exists at the moment, create a new one if lifetime is greater 0 */
        if(valid_lifetime != 0U)
        {
          routerListEntryPtr->valid_lifetime = TCPIP_NDP_RPD_GET_ROUTER_LIFETIME(dataPtr);
          TCPIP_IPV6_ADDR_CPY(routerListEntryPtr->ipAddr, remoteAddrPtr);
          routerListEntryPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED;
        }
        break;
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED:
        if(valid_lifetime != 0U)
        {
          routerListEntryPtr->valid_lifetime = TCPIP_NDP_RPD_GET_ROUTER_LIFETIME(dataPtr);
        }
        else
        {
          TcpIp_Ndp_Rpd_RouterList_entry_UNASSIGNED(routerListEntryPtr,ctrlIdx);
        }
        break;
      /* CHECK: NOPARSE */
      case TCPIP_NDP_RPD_ROUTERLIST_STATE_INACTIVE:
        /* ignore router if router assignment is not active,
         * TcpIp_Ndp_Rpd_RouterList_findRouterListEntry does not return entries in state
         * INACTIVE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_timeout
(
  uint8 ctrlIdx
)
{
  uint8 routerListIdx;

  /* Timeout for the router and prefix discovery router list state machine */
  for(routerListIdx = 0U; routerListIdx < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; routerListIdx++)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListDataPtr =
        &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][routerListIdx];

    if(routerListDataPtr->valid_lifetime > 0U)
    {
      routerListDataPtr->valid_lifetime--;

      if(routerListDataPtr->valid_lifetime == 0U)
      {
        /* Free the entry */
        TcpIp_Ndp_Rpd_RouterList_entry_UNASSIGNED(routerListDataPtr,ctrlIdx);
      }
    }
  }
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_findRouterListEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  uint8_least i;
  uint8_least routerListIdx = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;
  uint8_least routerListIdxFree = TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE;

  /* go through all entries */
  for(i = 0; i < TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE; ++i)
  {
    P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListEntryPtr =
        &TcpIp_Ndp_Rdp_RouterList_Data[ctrlIdx][i];

    /* only search in entries which are in use */
    if(routerListEntryPtr->state == TCPIP_NDP_RPD_ROUTERLIST_STATE_ASSIGNED)
    {
      /* verify if router address match with address of entry */
      if(TS_MemCmp(routerListEntryPtr->ipAddr, remoteAddrPtr, TCPIP_IPV6_ADDR_BYTE_SIZE) == E_OK)
      {
        routerListIdx = i;
        break;
      }
    }

    /* beside the existing entry, try to find a free entry */
    if(routerListIdxFree == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
    {
      if(routerListEntryPtr->state == TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED)
      {
        /* yeah it is available - stop further searching */
        routerListIdxFree = i;
      }
    }
  }

  if(routerListIdx == TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
  {
    /* if no matching default router was found, check if free entry exists */
    if(routerListIdxFree != TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE)
    {
      /* there is an empty entry */
      routerListIdx = routerListIdxFree;
    }
  }

  return (uint8)routerListIdx;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterList_entry_UNASSIGNED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterList_DataType, AUTOMATIC, TCPIP_APPL_DATA) routerListDataPtr,
  uint8 ctrlIdx
)
{
#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
  TcpIp_Ndp_DestCache_removeNextHopEntry(ctrlIdx,routerListDataPtr->ipAddr);
#else
  TS_PARAM_UNUSED(ctrlIdx);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */
  routerListDataPtr->valid_lifetime = 0U;
  TCPIP_IPV6_SET_UNSPECIFIED_IP(routerListDataPtr->ipAddr);
  routerListDataPtr->state = TCPIP_NDP_RPD_ROUTERLIST_STATE_UNASSIGNED;
}

#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_DestCache_getEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr
)
{
  uint16_least i = 0U;
  uint16_least entryIdx = TCPIP_NDP_DESTINATION_CACHE_SIZE;
  uint16_least entryIdxExisting = TCPIP_NDP_DESTINATION_CACHE_SIZE;
  uint16_least entryIdxFree = TCPIP_NDP_DESTINATION_CACHE_SIZE;

  /* go through all entries */
#if(TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U)
  for(i = 0U; i < TCPIP_NDP_DESTINATION_CACHE_SIZE; i++)
#endif /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
  {
    CONSTP2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
      &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][i];

    /* only search in entries which are in use */
    if(destinationCacheDataPtr->state == TCPIP_NDP_DESTINATION_CACHE_STATE_ASSIGNED)
    {
      /* verify if ip address match with ip address of entry */
      if(TCPIP_IPV6_ADDR_CMP(destinationCacheDataPtr->remoteIpAddr, ipaddr))
      {
        entryIdxExisting = i;
#if(TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U)
        break;
#endif /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
      }
    }
    else
    {
      /* beside the existing entry, try to find a free entry and only considering unlocked entries as free entries */
#if(TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U)
      if(entryIdxFree == TCPIP_NDP_DESTINATION_CACHE_SIZE)
#endif /* TCPIP_NDP_DESTINATION_CACHE_SIZE > 1U */
      {
        if(destinationCacheDataPtr->lock == 0U)
        {
          /* yeah it is available - stop further searching */
          entryIdxFree = i;
        }
      }
    }
  }

  if(entryIdxExisting != TCPIP_NDP_DESTINATION_CACHE_SIZE)
  {
    /* there is an existing matching Destination cache entry */
    entryIdx = entryIdxExisting;
  }
  else if(entryIdxFree != TCPIP_NDP_DESTINATION_CACHE_SIZE)
  {
    /* there is an empty Destination cache entry */
    entryIdx = entryIdxFree;
  }
  else
  {
    /* no (unlocked) existing entry, no free entry, all other entries are locked -
     * quite unrealistic but in this case we can't update the Destination cache */
  }

  return (uint16) entryIdx;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_DestCache_initEntry
(
  uint8 ctrlIdx,
  uint8 entryIdx
)
{
  P2VAR(TcpIp_Ndp_DestinationCache_DataType, AUTOMATIC, TCPIP_APPL_DATA) destinationCacheDataPtr =
          &TcpIp_Ndp_DestianationCache_Data[ctrlIdx][entryIdx];

  destinationCacheDataPtr->state = TCPIP_NDP_DESTINATION_CACHE_STATE_UNASSIGNED;
}
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_release_Multicast
(
  uint8 assignIdx
)
{
  CONSTP2CONST(TcpIp_NdpRpdAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_NdpRpdAddrIpV6ConfigType,intIpV6NdpRpdAddr,assignIdx));

  /* unassign solicited multicast ip address */
  (void) TcpIp_IpAddrM_releaseIpAddrAssignment(intLocalAddrPtr->solMultilocalAddrId);
}

#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  TcpIp_IpAddrM_AssignSM_assigned(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC;
  (void)TcpIp_Ndp_Dad_start(&TcpIp_Ndp_RouterAssign_dadResult, assignPtr->ipAddr, localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_OPTIMISTIC_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  uint8 assignIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  (void) TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_ASSIGNED_INACTIVE
          (ctrlIdx, assignIdx, localAddrId, assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_OPTIMISTIC_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  uint8 assignIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  (void) TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_Ndp_Rpd_RouterAssign_tr_ASSIGNED_CONFLICTED(assignIdx, localAddrId, assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_ASSIGNED_CONFLICTED
(
  uint8 assignIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  /* exit code */
  TcpIp_Ndp_Rpd_RouterAssign_exit_ASSIGNED(assignIdx, localAddrId);

  /* transitional code */

  /* entry code */
  TcpIp_Ndp_Rpd_RouterAssign_entry_CONFLICTED(assignPtr);
}

#else


STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE;
  (void)TcpIp_Ndp_Dad_start(&TcpIp_Ndp_RouterAssign_dadResult, assignPtr->ipAddr, localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_tr_stopAssign_TENTATIVE_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  uint8 assignIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  (void) TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_Ndp_Rpd_RouterAssign_release_Multicast(assignIdx);

  TcpIp_Ndp_Rpd_RouterAssign_entry_INACTIVE(ctrlIdx, assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  uint8 assignIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  (void) TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_Ndp_Rpd_RouterAssign_release_Multicast(assignIdx);
  TcpIp_Ndp_Rpd_RouterAssign_entry_CONFLICTED(assignPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_PREFERRED_DEPRECATED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  assignPtr->state = TCPIP_NDP_RPD_PREFERRED_DEPRECATED(assignPtr->preferred_lifetime);

#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)
  {
    /* Inform IpAddrM that link local IP address can be used */
    TcpIp_IpAddrM_AssignSM_assigned(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  }
#else
  TS_PARAM_UNUSED(localAddrId);
#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
}

#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_RouterAssign_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
)
{
  uint8 assignIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr =
      &TcpIp_Ndp_Rdp_RouterAssign_Data[assignIdx];


  switch(assignPtr->state)
  {
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_OPTIMISTIC:
    {
      if(unique)
      {
        assignPtr->state = TCPIP_NDP_RPD_PREFERRED_DEPRECATED(assignPtr->preferred_lifetime);
      }
      else
      {
        /* address already exists in the network */
        TcpIp_Ndp_Rpd_tr_dadResult_OPTIMISTIC_CONFLICTED(localAddrId, assignPtr);
      }
      break;
    }
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_TENTATIVE:
    {
      if(unique)
      {
        /* the address is unique, assign it to the interface */
        TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_PREFERRED_DEPRECATED(localAddrId, assignPtr);
      }
      else
      {
        /* address already exists in the network */
        TcpIp_Ndp_Rpd_tr_dadResult_TENTATIVE_CONFLICTED(localAddrId, assignPtr);
      }
      break;
    }
#endif /* TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON */
#endif /* TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON */
    /* CHECK: NOPARSE */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_INACTIVE:     /* intended fall through */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_UNASSIGNED:   /* intended fall through */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED:   /* intended fall through */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_PREFERRED:    /* intended fall through */
    case TCPIP_NDP_RPD_ROUTERASSIGN_STATE_DEPRECATED:   /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_CONFLICTED
(
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  assignPtr->state = TCPIP_NDP_RPD_ROUTERASSIGN_STATE_CONFLICTED;
  assignPtr->preferred_lifetime = 0U;
  assignPtr->valid_lifetime = 0U;
}

#else /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Rpd_RouterAssign_entry_PREFERRED_DEPRECATED
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Rpd_RouterAssign_DataType, AUTOMATIC, TCPIP_APPL_DATA) assignPtr
)
{
  assignPtr->state = TCPIP_NDP_RPD_PREFERRED_DEPRECATED(assignPtr->preferred_lifetime);

  TcpIp_IpAddrM_AssignSM_assigned(localAddrId, TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER);
}

#endif /* TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON */

#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
