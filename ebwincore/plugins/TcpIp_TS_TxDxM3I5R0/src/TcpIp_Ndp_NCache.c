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
 * MISRAC2012-2) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 */


/*==================[inclusions]============================================*/
#include <SchM_TcpIp.h>           /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ndp_Priv.h>       /* own SW-unit private file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief TcpIp_GetNdpCacheEntries() service ID */
#define TCPIP_SID_GETNDPCACHEENTRIES                              0x1cU

/* \brief maximum value of Neighbor Cache Time */
#define TCPIP_NDP_CACHETIME_MAX                                    255U

/* \brief Neighbor Advertisement - Router flag */
#define TCPIP_NDP_FLAG_R                                          0x80U
/* \brief Neighbor Advertisement - Solicited flag */
#define TCPIP_NDP_FLAG_S                                          0x40U
/* \brief Neighbor Advertisement - Override flag */
#define TCPIP_NDP_FLAG_O                                          0x20U

/* \brief Neighbor Advertisement - Router flag offset */
#define TCPIP_NDP_FLAG_R_OFFSET                                      7U

/** \brief Mask to check if state is valid */
#define TCPIP_IPV6_NDP_VALID_STATE_MASK                           0x45U

/** \brief Mask to check if state is invalid */
#define TCPIP_IPV6_NDP_INVALID_STATE_MASK                         0x42U

/** \brief Mask to check if state is complete */
#define TCPIP_IPV6_NDP_COMPLETE_STATE_MASK                        0x3CU

/** \brief Macro to verify if state is valid */
#define TCPIP_IPV6_NDP_STATE_VALID(state)        \
  (((state) & TCPIP_IPV6_NDP_VALID_STATE_MASK) != 0U)

/** \brief Macro to verify if state is invalid */
#define TCPIP_IPV6_NDP_STATE_INVALID(state)        \
  (((state) & TCPIP_IPV6_NDP_INVALID_STATE_MASK) != 0U)

/** \brief Macro to verify if state is complete */
#define TCPIP_IPV6_NDP_STATE_COMPLETE(state)        \
  (((state) & TCPIP_IPV6_NDP_COMPLETE_STATE_MASK) != 0U)

/* \brief Number of retransmission of Neighbor Solicitations */
#define TCPIP_NDP_NUM_MULTICAST_SOLICIT_MAX                           \
  TcpIp_PBcfgPtr->ndpConfig.ndpNumMulticastSolicitations
/* \brief Number of retransmission of Neighbor Solicitations */
#define TCPIP_NDP_NUM_UNICAST_SOLICIT_MAX                             \
  TcpIp_PBcfgPtr->ndpConfig.ndpNumUnicastSolicitations
/* \brief Timer to wait for an neighbor advertisement answer (in seconds) */
#define TCPIP_NDP_SOLICIT_DELAY                                       \
  TcpIp_PBcfgPtr->ndpConfig.ndpDefaultRetransTimer
/* \brief Delay timer for state delay (in seconds) */
#define TCPIP_NDP_DELAY_TIMEOUT                                       \
  TcpIp_PBcfgPtr->ndpConfig.ndpDelayFirstProbeTime
/* \brief Set state from reachable to stale if last reachability
          confirmation reached max timeout (in seconds)*/
#define TCPIP_NDP_REACHABLE_CONFIRM_DELAY                             \
  TcpIp_PBcfgPtr->ndpConfig.ndpDefaultReachableTime

/* \brief Get the status flags */
#define TCPIP_NDP_GET_FLAGS(pdata)                                    \
  ((pdata)[0U] & 0xe0U)

/* \brief get first option from ND messages */
#define TCPIP_NDP_GET_OPTIONS_PTR(pdata)                              \
  (&(pdata)[20U])

  /** \brief Update NDP cache entry
 *
 * This function resets the age of the entry.
 *
 * \param[in] entry           ndp cache entry
 */
#define TcpIp_Ndp_NCache_tr_updateSolicit_All_STALE(entry)                   \
  TcpIp_Ndp_NCache_tr_updateAdvert_STALE_STALE((entry))

/** \brief Transition from INCOMPLETE to STALE
 *
 * This performs the state transition from INCOMPLETE to STALE
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  link layer address from solicitation
 * \param[in] isRouter        indicates if solicitation was sent from a router
 */
#define TcpIp_Ndp_NCache_tr_updateSolicit_INCOMPLETE_STALE(entry, ll_target_addr, isRouter) \
  TcpIp_Ndp_NCache_tr_updateSolicit_All((entry), (ll_target_addr), (isRouter))

  /** \brief Update NDP cache entry
 *
 * In state DELAY. This function checks if the link layer address has changed and
 * updates the cache if the addresses are different.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 */
#define TcpIp_Ndp_NCache_tr_update_DELAY(entry, ll_target_addr, flags)                \
  TcpIp_Ndp_NCache_tr_updateAdvert_STALE((entry), (ll_target_addr), (flags))

/** \brief Update NDP cache entry
 *
 * In state PROBE. This function checks if the link layer address has changed and
 * updates the cache if the addresses are different.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 */
#define TcpIp_Ndp_NCache_tr_update_PROBE(entry, ll_target_addr, flags)          \
  TcpIp_Ndp_NCache_tr_updateAdvert_STALE((entry), (ll_target_addr), (flags))

  /** \brief Update NDP cache entry
 *
 * In state REACHABLE. This function sets the entry to STALE.
 *
 * \param[in] entry           ndp cache entry
 */
#define TcpIp_Ndp_NCache_tr_update_REACHABLE_STALE(entry)                      \
  TcpIp_Ndp_NCache_tr_updateAdvert_STALE_STALE((entry))

/*==================[type definitions]======================================*/

/* !LINKSTO TcpIp.Design.TcpIp_Ndp_EntryType,1 */
typedef struct
{
  /**< contains the ip address, physical address and state of the remote host */
  TcpIp_NdpCacheEntryType neighborAddr;
  /**< entry age timer (in seconds) */
  uint32 life_time;
  /**< delay timer (in seconds) */
  uint16 time;
  /**< transmission counter */
  uint8 transCnt;
  /**< lock that indicates if NDP entry is in use (> 0), or not (= 0) */
  uint8 lock;
  /**< indicates if host is a router */
  boolean isRouter;
}TcpIp_Ndp_NCache_EntryType;

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Transmits neighbor advertisement
 *
 * \param[in] rcvlocalAddrId   local address on which the solicitation was received on
 * \param[in] sendlocalAddrId  local address used to transmit
 * \param[in] remoteIpAddr     ip address of the remote host
 * \param[in] targetIp         ip address of the target
 * \param[in] srcUnspecified   TRUE: remote address is unspecified
 *                             FALSE: remote address is unicast
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_transmitNeighborAdvert
(
  TcpIp_LocalAddrIdType rcvlocalAddrId,
  TcpIp_LocalAddrIdType sendlocalAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  boolean srcUnspecified
);

/** \brief Retrieve options
 *
 * This function evaluates and extracts the options of a received NDP message
 *
 * \param[in]  dataPtr   points to the beginning of the option
 * \param[in]  lenByte   length of the option field
 * \param[out] options   points to found options
 *
 * \retval E_OK      options successfully extracted
 * \retval E_NOT_OK  error in option field encountered
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_getOption
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2VAR(TcpIp_Ndp_NCache_OptionType, AUTOMATIC, TCPIP_APPL_DATA) options
);


/** \brief Get index of entry in NDP cache
 *   corresponding to the passed ip address
 *
 * \param[in]  ctrlIdx     controller index
 * \param[in]  ipaddr      remote ip address
 * \param[out] replace     Indicates that the entry was replaced
 *
 * \return - an existing entry for ipaddr if exists
 *         - a free NDP entry
 *         - the oldest NDP entry
 *         - TCPIP_NDP_CACHESIZE_MAX no entry found (all are locked)
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_NCache_getEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_DATA) replace
);

/** \brief Verify if entry exists
 *
 * This function verifies if an entry with a corresponding
 * ip address exists in the NDP cache
 *
 * \param[in]  ctrlIdx     controller index
 * \param[in]  ipaddr      remote ip address
 *
 * \return - an existing entry for ipAddr if exists
 *         - TCPIP_NDP_CACHESIZE_MAX no entry found
 *
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_NCache_getExistingEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr
);

/** \brief Enter FREE state
 *
 *  This function reinitializes a entry to its default values
 *
 * \param[in] entry      ndp cache entry
 * \param[in] ctrlIdx    controller index
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_entry_FREE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  uint8 ctrlIdx
);


/** \brief Initializes an entry in the NDP cache
 *
 * \param[in]  entry     entry in the ndp cache
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_initEntry
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
);

/** \brief Transmits neighbor solicitation to multicast address
 *
 * \param[in]  ctrlIdx   controller index
 * \param[in]  targetIp  pointer to target ip address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_transmitMulticastNeighborSolicit
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp
);

/** \brief Timeout of NDP cache entry
 *
 * This function retransmits neighbor solicitations, removes entries which could not be
 * resolved and triggers reachability confirmations for outdated entries.
 *
 * \param[in]  ctrlIdx     controller index
 * \param[in]  entryIndex  index in the ndp cache
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_timeout
(
  uint8 ctrlIdx,
  uint16 entryIdx
);

/** \brief Set state to STALE and reset timers
 *
 * This function sets the state to STALE and resets transmission counter and
 * delay timer.
 *
 * \param[in]  entry             ndp cache entry
 * \param[in]  ll_target_addr    source link layer address of the solicitation
 * \param[in]  remoteAddrPtr     ip address of the remote host (source address of solicitation)
 * \param[in]  isRouter          indicates if solicitation was sent from a router
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_FREE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  boolean isRouter
);

/** \brief Update NDP cache Advertisement
 *
 * This function updates the NDP cache according to the values received in an advertisement.
 *
 * \param[in] ctrlIdx           controller index
 * \param[in] remoteAddrPtr     ip address of the remote host
 * \param[in] ll_target_addr    target link layer address of the advertisement
 * \param[in] flags             flags of the advertisement
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_updateAdvert
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
);

/** \brief Set physical address of remote host in NDP table
 *
 * \param[in]  ctrlIdx      TcpIp controller index to identify the related NDP table.
 * \param[in]  IpAddrPtr    specifies the IP address for which the physical address shall be
 *                          set in the NDP table
 * \param[in]  PhysAddrPtr  specifies the physical address which shall be set in the NDP table
 * \param[in]  State        TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: add/update the specified
 *                          entry in the ARP table
 *                          TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: set the specified entry in
 *                          the ARP table to static
 *
 * \return Result of operation
 * \retval TCPIP_OK:           physical address successfully added/updated
 * \retval TCPIP_E_NOT_OK:     entry is currently locked
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_Ndp_NCache_setRemoteAddr
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief Remove remote host from NDP table
 *
 *  This function removes the physical address of the specified remote host from
 *  the NDP table
 *
 * \param[in] ipAddr    ip address of the remote host that shall be removed
 * \param[in] ctrlIdx   controller index
 *
 * \return Result of the operation
 * \retval TCPIP_OK                 Remote host successfully removed
 * \retval TCPIP_E_PHYS_ADDR_MISS   Remote host could not be found
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_Ndp_NCache_removeRemoteAddr
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr,
  uint8 ctrlIdx
);

/** \brief Lookup NDP cache for link layer address
 *
 * In state STALE. This function sets the entry state to DELAY and starts the delay timer.
 *
 * \param[in] entry  ndp cache entry
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_lookup_STALE_DELAY
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
);

/** \brief Lookup NDP cache for link layer address
 *
 * In state FREE. This function sets the entry state to INCOMPLETE and starts the retransmit timer.
 *
 * \param[in]  ctrlIdx     controller index
 * \param[in]  entryIndex  index in the ndp cache
 * \param[in]  ipAddr      remote ip address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_lookup_FREE_INCOMPLETE
(
  uint8 ctrlIdx,
  uint16 entryIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr
);

/** \brief Update NDP cache entry
 *
 * In state INCOMPLETE. If S falg is set. This function copies the link layer address
 * of the remote host and sets the ndp cache entry state to REACHABLE.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
);

#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
/** \brief Update NDP cache entry
 *
 * In state INCOMPLETE. If S falg is not set. This function copies the link layer address
 * of the remote host and sets the ndp cache entry state to STALE.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
);

/** \brief Update NDP cache entry
 *
 * In state REACHABLE. This function checks if the link layer address has changed and
 * updates the cache if the addresses are different.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 *
 * \return Result of operation
 * \retval TRUE:      physical address changed
 * \retval FALSE:     physical address stayed the same
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
);
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */

/** \brief Update NDP cache entry
 *
 * In state STALE. This function checks if the link layer address has changed and
 * updates the cache if the addresses are different.
 *
 * \param[in] entry           ndp cache entry
 * \param[in] ll_target_addr  target link layer address from advertisement
 * \param[in] flags           flags of the advertisement
 *
 * \return Result of operation
 * \retval TRUE:      physical address changed
 * \retval FALSE:     physical address stayed the same
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
);

/** \brief Update NDP cache entry
 *
 * In state STALE. This function sets the entry state to REACHABLE.
 *
 * \param[in] entry       ndp cache entry
 * \param[in]  isRouter   indicates if advertisement was sent from a router
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  boolean isRouter
);

/** \brief Update NDP cache entry
 *
 * In state STALE. This function resets the age of the entry.
 *
 * \param[in] entry           ndp cache entry
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
);

/** \brief Update NDP cache entry
 *
 * This function checks if the link layer address has changed and updates the cache
 * if the addresses are different.
 *
 * \param[in] entry     ndp cache entry
 * \param[in] linkAddr  link layer address from solicitation
 * \param[in] isRouter  indicates if advertisement was sent from a router
 *
 * \return Result of operation
 * \retval TRUE:      physical address changed
 * \retval FALSE:     physical address stayed the same
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
);

/** \brief Update NDP cache entry
 *
 * This function checks if the link layer address has changed and updates the cache
 * if the addresses are different.
 *
 * \param[in] entry     ndp cache entry
 * \param[in] linkAddr  link layer address from solicitation
 * \param[in] isRouter  indicates if advertisement was sent from a router
 *
 * \return Result of operation
 * \retval TRUE:      physical address changed
 * \retval FALSE:     physical address stayed the same
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_All
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
);

#if TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON
/** \brief Call NDP cache change callout
 *
 *  This function calls the ndp table entry change callout.
 *
 * \param[in] ctrlIdx       controller index
 * \param[in] ipAddrPtr     Ip address of modified entry
 * \param[in] physAddrPtr   pointer to physical address of modified entry
 * \param[in] valid         entry has been added/updated (TRUE) or removed(FALSE)
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_NdpChangeFct
(
  uint8 ctrlIdx,
  P2CONST(uint32,AUTOMATIC,TCPIP_APPL_DATA) ipAddrPtr,
  P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) physAddrPtr,
  boolean valid
);
#endif /* TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/* the neighbor cache */
STATIC VAR(TcpIp_Ndp_NCache_EntryType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_NCache_Data[TCPIP_NUM_ETHIFCTRL][TCPIP_NDP_CACHESIZE_MAX];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetNdpCacheEntries
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) numberOfElements,
  P2VAR(TcpIp_NdpCacheEntryType, AUTOMATIC, TCPIP_APPL_DATA) entryListPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_GETNDPCACHEENTRIES_ENTRY(ctrlIdx,numberOfElements,entryListPtr);
#if (TCPIP_IPV6_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETNDPCACHEENTRIES, TCPIP_E_NOTINIT);
  }
  else if( (ctrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx) ||
           (TCPIP_CFG_TYPE(uint8,ctrlMapping,ctrlIdx) == TCPIP_CTRLIDX_INVALID)
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETNDPCACHEENTRIES, TCPIP_E_INV_ARG);
  }
  else if(numberOfElements == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETNDPCACHEENTRIES, TCPIP_EB_E_PARAM_POINTER);
  }
  else if((*numberOfElements > 0U) && (entryListPtr == NULL_PTR))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETNDPCACHEENTRIES, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 TcpIp_CtrlIdx = TCPIP_CFG_TYPE(uint8,ctrlMapping,ctrlIdx);
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,TcpIp_CtrlIdx)));

    uint8_least const startIndex = ctrlPtr->intLocalAddrIdFirst;
    uint8_least const endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;
    uint16 numValidElements = 0U;
    TcpIp_StateType returnState = TCPIP_STATE_OFFLINE;
    TcpIp_LocalAddrIdType AddrId = TCPIP_LOCALADDRID_INVALID;

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

        if((returnState == TCPIP_STATE_ONLINE) ||
           (returnState == TCPIP_STATE_SHUTDOWN)
          )
        {
          AddrId = lAddrId;
          break;
        }
      }
    }

   /* if there is a unicast address assigned get entries */
   if(AddrId != TCPIP_LOCALADDRID_INVALID)
   {
     if(TcpIp_IpAddrM_lockIfOnlineShutdown(AddrId))
     {
       /* !LINKSTO TcpIp.Design.Ndp.EA1.GetNdpCacheEntries,1 */
       SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

       /* loop all entries of NDP table for specific interface */
       for(i = 0U; i < TCPIP_NDP_CACHESIZE_MAX; i++)
       {
         P2VAR(TcpIp_Ndp_NCache_EntryType,AUTOMATIC,TCPIP_VAR_CLEARED) ndpEntryPtr =
             &TcpIp_Ndp_NCache_Data[TcpIp_CtrlIdx][i];

         /* check if this entry is valid */
         if (!TCPIP_IPV6_NDP_STATE_INVALID(ndpEntryPtr->neighborAddr.State))
         {
           if(*numberOfElements > 0U)
           {
             entryListPtr[numValidElements] = ndpEntryPtr->neighborAddr;
             numValidElements++;

             if(numValidElements == *numberOfElements)
             {
               /* save up to numberOfElements valid entries */
               break;
             }
           }
           else
           {
             /* if numberOfElements is 0 just count the valid entries */
             numValidElements++;
           }
         }
       }

       /* !LINKSTO TcpIp.Design.Ndp.EA1.GetNdpCacheEntries,1 */
       SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

       TcpIp_IpAddrM_unlockIf(AddrId);
       retVal = E_OK;
     }
   }
    *numberOfElements = numValidElements;
  }
#else /* (TCPIP_IPV6_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(ctrlIdx);
  TS_PARAM_UNUSED(numberOfElements);
  TS_PARAM_UNUSED(entryListPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  TCPIP_REPORT_ERROR(TCPIP_SID_GETNDPCACHEENTRIES, TCPIP_E_INV_ARG);
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

  DBG_TCPIP_GETNDPCACHEENTRIES_EXIT(retVal,ctrlIdx,numberOfElements,entryListPtr);

  return retVal;
}

#if (TCPIP_IPV6_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_lookupAndLock
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  boolean createNewEntry
)
{
  TcpIp_ReturnType result = TCPIP_E_PHYS_ADDR_MISS;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  uint8 oldEthAddr[TCPIP_ETH_ADDR_LENGTH];
  uint32 oldIpAddr[TCPIP_IPV6_ADDR_SIZE];
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  boolean replaceEntry = FALSE;
  boolean transmitSolicit = FALSE;

  DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_ENTRY(ctrlIdx,ipAddr,remoteAddrHandlePtr,createNewEntry);

  /* !LINKSTO TcpIp.Design.Ndp.EA1.lookupAndLock,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.getRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  {
    uint16 entryIdx;

    /* search for existing entry */
    entryIdx = TcpIp_Ndp_NCache_getExistingEntry(ctrlIdx, ipAddr);

    if((entryIdx == TCPIP_NDP_CACHESIZE_MAX) && (createNewEntry == TRUE))
    {
      /* add a new entry if none exists */
      entryIdx = TcpIp_Ndp_NCache_getEntry(ctrlIdx, ipAddr, &replaceEntry);
    }

    if(entryIdx != TCPIP_NDP_CACHESIZE_MAX)
    {
      CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
        &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
      if(replaceEntry == TRUE)
      {
        /* copy old NDP entry data to temporary storage for callout */
        TCPIP_IPV6_ADDR_CPY(oldIpAddr, entry->neighborAddr.Inet6Addr);
        TcpIp_EthAddrCpy(oldEthAddr,entry->neighborAddr.PhysAddr);
      }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

      switch(entry->neighborAddr.State)
      {
        case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
          /* address resolution is still ongoing */
          break;
        case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:    /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:     /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:
          *remoteAddrHandlePtr = entryIdx;
          entry->lock++;
          result = TCPIP_OK;
          break;
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
        {
          /* last reachability confirmation is a long time ago
             set state to delay */
          TcpIp_Ndp_NCache_tr_lookup_STALE_DELAY(entry);
          *remoteAddrHandlePtr = entryIdx;
          entry->lock++;
          result = TCPIP_OK;
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
          TcpIp_Ndp_NCache_tr_lookup_FREE_INCOMPLETE(ctrlIdx, entryIdx, ipAddr);
          transmitSolicit = TRUE;
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
  /* !LINKSTO TcpIp.Design.Ndp.EA1.lookupAndLock,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.getRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old NDP entry */
    TcpIp_NdpChangeFct(ctrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  if(transmitSolicit == TRUE)
  {
    /* ups NDP lookup failed, we need to send an NDP solicitation */
    TcpIp_Ndp_NCache_transmitMulticastNeighborSolicit(ctrlIdx, ipAddr);
  }

  DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_EXIT(result,ctrlIdx,ipAddr,remoteAddrHandlePtr,createNewEntry);

  return result;
}

TS_MOD_PRIV_DEFN FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE ) TcpIp_Ndp_NCache_getPhysAddrPtr
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
)
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr =
      TcpIp_Ndp_NCache_Data[ctrlIdx][remoteAddrHandle].neighborAddr.PhysAddr;

  DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_ENTRY(remoteAddrHandle,ctrlIdx);
  DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_EXIT(addr,remoteAddrHandle,ctrlIdx);
  return addr;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_getRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;
  /* get the unicast address of the controller */
  TcpIp_LocalAddrIdType localAddrId;

  DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes);

  localAddrId = TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx(CtrlIdx, IpAddrPtr);

  /* check if this entry is valid */
  if (localAddrId != TCPIP_LOCALADDRID_INVALID)
  {
    if (TcpIp_IpAddrM_lockIfOnline(localAddrId))
    {
      uint16 remoteAddrHandle = 0U;

      retval = TcpIp_Ndp_NCache_lookupAndLock
              (
                CtrlIdx,
                /* Deviation MISRAC2012-1 */
                ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA))IpAddrPtr)->addr,
                &remoteAddrHandle,
                initRes
              );

     if (retval == TCPIP_OK)
     {
       /* retrieve physical address from NDP cache and copy it to PhysAddrPtr */
       P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysRemoteAddrPtr =
         TcpIp_Ndp_NCache_getPhysAddrPtr(remoteAddrHandle, CtrlIdx);

       TcpIp_EthAddrCpy(PhysAddrPtr, PhysRemoteAddrPtr);

       TcpIp_Ndp_NCache_unlock(remoteAddrHandle,CtrlIdx);
     }

      TcpIp_IpAddrM_unlockIf(localAddrId);
    }
  }
  DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_EXIT(retval,CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes);

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_setRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;
  /* Deviation MISRAC2012-1 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_CONST)) IpRemoteAddrPtr)->addr;

  DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,State);


#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(!TCPIP_IPV6_NDP_STATE_VALID(State))
  {
    /* invalid request */
    retval = TCPIP_E_NOT_PERMITTED;
  }
  else if((TCPIP_IPV6_ADDR_IS_MULTICAST(IpAddrPtr)) || (TCPIP_IPV6_ADDR_IS_UNSPECIFIED(IpAddrPtr)))
  {
    /* invalid ip address */
    retval = TCPIP_E_NOT_PERMITTED;
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    switch(State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:     /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
        retval = TcpIp_Ndp_NCache_setRemoteAddr(CtrlIdx, IpAddrPtr, PhysAddrPtr, State);
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        retval = TcpIp_Ndp_NCache_removeRemoteAddr(IpAddrPtr, CtrlIdx);
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        retval = TCPIP_E_NOT_OK;
        break;
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_EXIT(retval,CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,State);

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE ) TcpIp_Ndp_NCache_unlock
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
)
{
  DBG_TCPIP_NDP_NCACHE_UNLOCK_ENTRY(remoteAddrHandle,ctrlIdx);

  TCPIP_PRECONDITION_ASSERT(remoteAddrHandle < TCPIP_NDP_CACHESIZE_MAX, TCPIP_INTERNAL_API_ID);
  /* !LINKSTO TcpIp.Design.Ndp.EA1.getRemotePhysAddr,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.unlock,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  TCPIP_PRECONDITION_ASSERT
    ((TcpIp_Ndp_NCache_Data[ctrlIdx][remoteAddrHandle].lock != 0U), TCPIP_INTERNAL_API_ID);

  /* unlock NDP table entry */
  TcpIp_Ndp_NCache_Data[ctrlIdx][remoteAddrHandle].lock--;

  /* !LINKSTO TcpIp.Design.Ndp.EA1.getRemotePhysAddr,1 */
  /* !LINKSTO TcpIp.Design.Ndp.EA1.unlock,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  DBG_TCPIP_NDP_NCACHE_UNLOCK_EXIT(remoteAddrHandle,ctrlIdx);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_update
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
)
{
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
  uint8 oldEthAddr[TCPIP_ETH_ADDR_LENGTH];
  uint32 oldIpAddr[TCPIP_IPV6_ADDR_SIZE];
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  boolean replaceEntry = FALSE;

  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  TCPIP_PRECONDITION_ASSERT( ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID );
  {
    /* search for existing entries, add a new entry if none exists */
    uint16 const entryIdx = TcpIp_Ndp_NCache_getEntry(ctrlIdx, remoteAddrPtr, &replaceEntry);
    TCPIP_PRECONDITION_ASSERT( entryIdx <= TCPIP_NDP_CACHESIZE_MAX, TCPIP_INTERNAL_API_ID );
    if(entryIdx != TCPIP_NDP_CACHESIZE_MAX)
    {
      CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
        &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
      if(replaceEntry == TRUE)
      {
        /* copy old NDP entry data to temporary storage for callout */
        TCPIP_IPV6_ADDR_CPY(oldIpAddr, entry->neighborAddr.Inet6Addr);
        TcpIp_EthAddrCpy(oldEthAddr,entry->neighborAddr.PhysAddr);
      }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

      switch(entry->neighborAddr.State)
      {
        case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        {
          TCPIP_PRECONDITION_ASSERT((entry->lock == 0U), TCPIP_INTERNAL_API_ID);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
              TcpIp_Ndp_NCache_tr_updateSolicit_INCOMPLETE_STALE(entry, linkAddr, isRouter);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:
        case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
            TcpIp_Ndp_NCache_tr_updateSolicit_All(entry, linkAddr, isRouter);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
            TcpIp_Ndp_NCache_tr_updateSolicit_REACHABLE(entry, linkAddr, isRouter);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
        {
          TCPIP_PRECONDITION_ASSERT((entry->lock == 0U), TCPIP_INTERNAL_API_ID);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
            TcpIp_Ndp_NCache_tr_updateSolicit_All(entry, linkAddr, isRouter);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        {
          if(linkAddr != NULL_PTR)
          {
            TcpIp_Ndp_NCache_tr_updateSolicit_FREE_STALE(entry, linkAddr, remoteAddrPtr, isRouter);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
            ethAddrChanged = TRUE;
#endif/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          }
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
          /* ignore solicitation */
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

  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old NDP entry */
    TcpIp_NdpChangeFct(ctrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }

  if(ethAddrChanged == TRUE)
  {
    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate NDP table change to upper layer */
    TcpIp_NdpChangeFct(ctrlIdx,remoteAddrPtr,linkAddr, TRUE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_init (void)
{
  uint8_least ctrlIdx;
  uint16_least entryIdx;

  /* go through all interfaces */
  for(ctrlIdx = 0U; ctrlIdx < TCPIP_NUM_ETHIFCTRL; ++ctrlIdx)
  {
    /* go through all entries and initialize them */
    for(entryIdx = 0U; entryIdx < TCPIP_NDP_CACHESIZE_MAX; entryIdx++)
    {
      CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
        &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

      TcpIp_Ndp_NCache_initEntry(entry);
      entry->lock = 0U;
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_mainFunction
(
  uint16 timerTick
)
{
  uint8_least ctrlIdx;
  uint16 entryIdx;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_1S) == TCPIP_TIMER_OFFSET_1S_NDP_TABLE)
  {
    /* go through all interfaces */
    for (ctrlIdx = 0; ctrlIdx < TCPIP_NUM_ETHIFCTRL; ctrlIdx++)
    {
      /* go through all entries */
      for(entryIdx = 0U; entryIdx < TCPIP_NDP_CACHESIZE_MAX; entryIdx++)
      {
        TcpIp_Ndp_NCache_timeout((uint8)ctrlIdx, entryIdx);
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_rxNeighborSolicit
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  /* check that ICMP length is 20 or more octets */
  if(lenByte >= TCPIP_NDP_NEIGH_SOLIC_SIZE_MIN)
  {
    uint32 targetIp[TCPIP_IPV6_ADDR_SIZE];

    TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD
      (targetIp, TCPIP_NDP_TARGET_ADDR_GET_PTR(dataPtr));

    /* target address shall not be a multicast address */
    if(!TCPIP_IPV6_ADDR_IS_MULTICAST(targetIp))
    {
      TcpIp_Ndp_NCache_OptionType options;
      const uint16 optionlen = lenByte - TCPIP_NDP_NEIGH_SOLIC_SIZE_MIN;
      P2CONST(uint8, AUTOMATIC, TCPIP_VAR) optionPtr = TCPIP_NDP_GET_OPTIONS_PTR(dataPtr);

      if(TcpIp_Ndp_NCache_getOption(optionPtr, optionlen, &options) == E_OK)
      {
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
        boolean srcUnspecified = FALSE;
        Std_ReturnType result = E_OK;
        TcpIp_LocalAddrIdType targetAddrId = TCPIP_LOCALADDRID_INVALID;
        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

        targetAddrId =
          TcpIp_IpV6_getLocalAddrId_byIp
            /* Deviation MISRAC2012-1 */
            ((P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA))&targetIp, ctrlIdx);

        /* if source address is unspecified address foreign node is performing DAD */
        if(TCPIP_IPV6_ADDR_IS_UNSPECIFIED(remoteAddrPtr->addr))
        {
          TcpIp_IpAddrConstPtr localIpAddrPtr;
          srcUnspecified = TRUE;

          TcpIp_IpAddrM_getIpAddress(localAddrId, &localIpAddrPtr);

          /* if source ip address is unspecified address, the ip
            destination address must be solicited-node multicast address */
          if(!TcpIp_IpV6_isSolicitMulti(localIpAddrPtr))
          {
            result = E_NOT_OK;
          }
          /* if source ip address is unspecified address, packet
             must not contain source link layer address */
          else if(options.source_link_addr != NULL_PTR)
          {
            result = E_NOT_OK;
          }
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
          else if((targetAddrId != TCPIP_LOCALADDRID_INVALID))
          {
            result = TcpIp_Ndp_Dad_checkDuplicate(targetIp);
          }
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
          else
          {
            ; /* avoid misra */
          }
        }

        if(result == E_OK)
        {
          /* check if target address is assigned to receiving interface */
          if(targetAddrId != TCPIP_LOCALADDRID_INVALID)
          {
#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
            /* update entry in ndp cache if source link layer option
               is provided */
            if(options.source_link_addr != NULL_PTR)
            {
              TcpIp_Ndp_NCache_update(ctrlIdx, remoteAddrPtr->addr, options.source_link_addr, FALSE);
            }
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */

            (void)
              TcpIp_Ndp_NCache_transmitNeighborAdvert
                (localAddrId, targetAddrId, remoteAddrPtr, targetIp, srcUnspecified);
          }
        }
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_rxNeighborAdvert
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  /* check that ICMP length is 20 or more octets */
  if(lenByte >= TCPIP_NDP_NEIGH_ADVERT_SIZE_MIN)
  {
    uint32 targetIp[TCPIP_IPV6_ADDR_SIZE];

    TCPIP_IPV6_ADDR_COPY_FROM_PAYLOAD
      (targetIp, TCPIP_NDP_TARGET_ADDR_GET_PTR(dataPtr));

    /* target address shall not be a multicast address */
    if(!TCPIP_IPV6_ADDR_IS_MULTICAST(targetIp))
    {
      const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);
      Std_ReturnType result = E_OK;
      const uint8 flags = TCPIP_NDP_GET_FLAGS(dataPtr);

      if(TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
      {
        /* If the IP Destination Address is a multicast address the
           Solicited flag is zero */
        if((flags & TCPIP_NDP_FLAG_S) != 0U)
        {
          result = E_NOT_OK;
        }
      }

      if(result == E_OK)
      {
        TcpIp_Ndp_NCache_OptionType options;
        const uint16 optionlen = lenByte - TCPIP_NDP_NEIGH_ADVERT_SIZE_MIN;
        P2CONST(uint8, AUTOMATIC, TCPIP_VAR) optionPtr = TCPIP_NDP_GET_OPTIONS_PTR(dataPtr);

        if(TcpIp_Ndp_NCache_getOption(optionPtr, optionlen, &options) == E_OK)
        {
          const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
          result = TcpIp_Ndp_Dad_checkDuplicate(targetIp);
          if(result == E_OK)
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
          {
            TcpIp_Ndp_NCache_updateAdvert(ctrlIdx, targetIp, options.target_link_addr, flags);
          }
        }
      }
    }
  }
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_transmitNeighborSolicit
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  uint16 remoteAddrHandle,
  boolean unspecifiedSource
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;

  if((localAddrId != TCPIP_LOCALADDRID_INVALID))
  {
    if(TcpIp_IpAddrM_lockIfStartupOnline(localAddrId))
    {
      uint16 msgLength = TCPIP_NDP_NEIGH_SOLIC_SIZE_MIN;

      if(!unspecifiedSource)
      {
        msgLength += (TCPIP_NDP_OPT_LINK_ADDR_LEN + TCPIP_NDP_OPT_HDR_LEN);
#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
        if(TcpIp_IpAddrM_getIpAddrState(localAddrId) == TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC)
        {
          result = TCPIP_E_NOT_OK;
        }
        else
        {
          result = TCPIP_OK;
        }
      }
      else
      {
        result = TCPIP_OK;
#endif/* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
      }

#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
      if(result == TCPIP_OK)
#endif/* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
      {
        TcpIp_Ip_AddrType ipAddr;
        uint8 type = TCPIP_NDP_ICMP_TYPE_NEIGHBOR_SOLICIT;

        ipAddr.localAddrId = localAddrId;
        /* Deviation MISRAC2012-1 */
        ipAddr.remoteAddrPtr = (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) remoteIpAddr;

        result = TcpIp_Ndp_IntTransmit( &ipAddr,
                                        remoteAddrHandle,
                                        targetIp,
                                        msgLength,
                                        type,
                                        0U,
                                        !unspecifiedSource,
                                        unspecifiedSource
                                      );
      }

      /* unlock interface */
      TcpIp_IpAddrM_unlockIf(localAddrId);
    }
  }

  return result;
}

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  /* Deviation MISRAC2012-1 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_CONST)) remoteAddrPtr)->addr;

  DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_ENTRY(CtrlIdx,remoteAddrPtr);

  /* !LINKSTO TcpIp.Design.Ndp.EA1.reachConfirmation,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  {
    uint16 const entryIdx = TcpIp_Ndp_NCache_getExistingEntry(CtrlIdx, IpAddrPtr);

    if (entryIdx != TCPIP_NDP_CACHESIZE_MAX)
    {
      CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
              &TcpIp_Ndp_NCache_Data[CtrlIdx][entryIdx];
      switch(entry->neighborAddr.State)
      {
        case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:
        case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
        {
          entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE;
          entry->time = TCPIP_NDP_REACHABLE_CONFIRM_DELAY;
          entry->transCnt = 0U;
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
        {
          /* Ignore these states */
          break;
        }
        /* CHECK: NOPARSE */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }
    }
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.reachConfirmation,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_EXIT(CtrlIdx,remoteAddrPtr);

}
#endif /* TCPIP_TCP_ENABLED == STD_ON */

#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_EntryIsReachable
(
  uint8 CtrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ipAddr
)
{

  boolean result = FALSE;
  uint8 i;
  /* go through all interfaces */

  /* !LINKSTO TcpIp.Design.Ndp.EA1.getDefaultRouterAddress,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  for (i = 0; i < TCPIP_NDP_CACHESIZE_MAX; i++)
  {
    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
      &TcpIp_Ndp_NCache_Data[CtrlIdx][i];
    if
    (
      (entry->neighborAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE) &&
      (entry->neighborAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
    )
    {
      if(TS_MemCmp(entry->neighborAddr.Inet6Addr, ipAddr, TCPIP_IPV6_ADDR_BYTE_SIZE) == E_OK)
      {
        result = TRUE;
      }
    }
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.getDefaultRouterAddress,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  return result;
}
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/


STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_transmitMulticastNeighborSolicit
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp
)
{
  TcpIp_SockAddrInet6Type remoteAddr;
  TcpIp_LocalAddrIdType localAddrId;

  remoteAddr.domain = TCPIP_AF_INET6;

  /* link layer address unknown send packet to solicit multicast address */
  TcpIp_IpV6_getSolicitMulti(remoteAddr.addr, targetIp);
  /* get local ip address through source address selection */
  localAddrId =
    TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx
    (
      ctrlIdx,
      /* Deviation MISRAC2012-1 */
      (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA))&remoteAddr
    );

  (void)
    TcpIp_Ndp_NCache_transmitNeighborSolicit
      (localAddrId, &remoteAddr, targetIp, TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST, FALSE);
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_transmitNeighborAdvert
(
  TcpIp_LocalAddrIdType rcvlocalAddrId,
  TcpIp_LocalAddrIdType sendlocalAddrId,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  boolean srcUnspecified
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;

  if(TcpIp_IpAddrM_lockIfOnline(sendlocalAddrId))
  {
    const TcpIp_IpAddrM_AddrInfoType addrInfo =
      TcpIp_IpAddrM_getLocalAddrInfo(rcvlocalAddrId);
    TcpIp_SockAddrInet6Type allNodesMulticast;
    uint16 msgLength = TCPIP_NDP_NEIGH_ADVERT_SIZE_MIN;
    uint8 flags = 0U;

    allNodesMulticast.domain = TCPIP_AF_INET6;

    if(srcUnspecified)
    {
      /* solicitation received from unspecified address,
         send packet to all node address */
      TcpIp_Ipv6_getAllNodesMulticastIpAddr(allNodesMulticast.addr);
      remoteIpAddr = &allNodesMulticast;
    }
    else
    {
      /* send advertisement to sender */
      flags |= TCPIP_NDP_FLAG_S;
    }

    if(TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
    {
      /* if solicitation was received on multicast append the target link layer
         address option */
      msgLength += TCPIP_NDP_OPT_LINK_ADDR_LEN + TCPIP_NDP_OPT_HDR_LEN;

#if(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if(TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
      if(TcpIp_IpAddrM_getIpAddrState(sendlocalAddrId) != TCPIP_IPADDRM_ADDRSTATE_OPTIMISTIC)
#endif/* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
#endif/* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
      {
        flags |= TCPIP_NDP_FLAG_O;
      }
    }

    {
      TcpIp_Ip_AddrType ipAddr;
      uint8 type = TCPIP_NDP_ICMP_TYPE_NEIGHBOR_ADVERT;
      const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(sendlocalAddrId);
      uint16 remoteAddrHandle;
      boolean addEthAddr = (TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST));
      /* Deviation MISRAC2012-1 <+2> */
      P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr =
        (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) remoteIpAddr;

      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

      ipAddr.localAddrId = sendlocalAddrId;
      ipAddr.remoteAddrPtr = remoteAddr;

      result = TcpIp_IpV6_getRemoteAddrHandle(&remoteAddrHandle, remoteAddr, sendlocalAddrId);

      if(result == TCPIP_OK)
      {
        result = TcpIp_Ndp_IntTransmit( &ipAddr,
                                        remoteAddrHandle,
                                        targetIp,
                                        msgLength,
                                        type,
                                        flags,
                                        addEthAddr,
                                        FALSE
                                      );
        /* if there was an lookup unlock the entry */
        if(remoteAddrHandle != TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST)
        {
          TcpIp_IpV6_unlockEntry(remoteAddrHandle,ctrlIdx);
        }
      }
    }

    TcpIp_IpAddrM_unlockIf(sendlocalAddrId);
  }

  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ndp_NCache_getOption
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2VAR(TcpIp_Ndp_NCache_OptionType, AUTOMATIC, TCPIP_APPL_DATA) options
)
{
  Std_ReturnType result = E_NOT_OK;
  uint8 optionType;
  uint32 i = 0U;
  uint16 optionLength = 0U;

  options->source_link_addr = NULL_PTR;
  options->target_link_addr = NULL_PTR;

  if(lenByte == 0U)
  {
    result = E_OK;
  }
  else
  {
    /* go through all options and look for known ones */
    do
    {
      /* i will not be greater than lenByte, this is verified below (i < lenByte) */
      if( ( (uint16)(lenByte - (uint16) i) ) >= TCPIP_NDP_OPT_HDR_LEN)
      {
        optionType = dataPtr[i];
        optionLength = ((uint16)(dataPtr[i + 1U])) * 8U;

        /* option must have a length greater zero */
        if(optionLength == 0U)
        {
          result = E_NOT_OK;
        }
        else if((i + optionLength) > lenByte)
        {
          result = E_NOT_OK;
        }
        else
        {
          switch(optionType)
          {
            /* option field contains link layer address */
            case TCPIP_NDP_OPT_SRC_LINK_ADDR_ID:
            {
              if(optionLength == TCPIP_NDP_OPT_LINK_ADDR_LEN + TCPIP_NDP_OPT_HDR_LEN)
              {
                if(TcpIp_CheckMacNotMulticast(&dataPtr[i + TCPIP_NDP_OPT_HDR_LEN]) == E_OK)
                {
                  options->source_link_addr = &dataPtr[i + TCPIP_NDP_OPT_HDR_LEN];
                  result = E_OK;
                }
              }
              else
              {
                result = E_NOT_OK;
              }
              break;
            }
            case TCPIP_NDP_OPT_TARGET_LINK_ADDR_ID:
            {
              if(optionLength == TCPIP_NDP_OPT_LINK_ADDR_LEN + TCPIP_NDP_OPT_HDR_LEN)
              {
                if(TcpIp_CheckMacNotMulticast(&dataPtr[i + TCPIP_NDP_OPT_HDR_LEN]) == E_OK)
                {
                  options->target_link_addr = &dataPtr[i + TCPIP_NDP_OPT_HDR_LEN];
                  result = E_OK;
                }
              }
              else
              {
                result = E_NOT_OK;
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

          i += optionLength;
        }
      }
      else
      {
        /* invalid option length */
        result = E_NOT_OK;
      }

    }while ((i < lenByte) && (result == E_OK));
  }

  return result;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_NCache_getEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_DATA) replace
)
{
  uint16_least i;
  uint16_least entryIdx = TCPIP_NDP_CACHESIZE_MAX;
  uint16_least entryIdxExisting = TCPIP_NDP_CACHESIZE_MAX;
  uint16_least entryIdxFree = TCPIP_NDP_CACHESIZE_MAX;
  uint16_least entryIdxOldest = TCPIP_NDP_CACHESIZE_MAX;
  uint32 timeOldest = 0U;

  /* go through all entries */
  for(i = 0; i < TCPIP_NDP_CACHESIZE_MAX; ++i)
  {
    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
      &TcpIp_Ndp_NCache_Data[ctrlIdx][i];

    /* only search in entries which are in use */
    if(entry->neighborAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
    {
      /* verify if ip address match with ip address of entry */
      if(TCPIP_IPV6_ADDR_CMP(entry->neighborAddr.Inet6Addr, ipaddr))
      {
        entryIdxExisting = i;
        break;
      }
    }

    /* beside the existing entry, try to find a free entry */
    if(entryIdxFree == TCPIP_NDP_CACHESIZE_MAX)
    {
      /* only considering unlocked entries as free entries */
      if(entry->lock == 0U)
      {
        if(entry->neighborAddr.State == TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
        {
          /* yeah it is available - stop further searching */
          entryIdxFree = i;
        }
        else if
        (
          ((entry->life_time > timeOldest) || (entryIdxOldest == TCPIP_NDP_CACHESIZE_MAX)) &&
          (entry->neighborAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC)
        )
        {
          /* until we found a free entry, we keep track of the oldest NDP entry */
          entryIdxOldest = i;
          timeOldest = entry->life_time;
        }
        else
        {
          ; /* no matching or older entry found */
        }
      }
    }
  }

  if(entryIdxExisting != TCPIP_NDP_CACHESIZE_MAX)
  {
    /* there is an existing matching NDP entry */
    entryIdx = entryIdxExisting;
  }
  else if(entryIdxFree != TCPIP_NDP_CACHESIZE_MAX)
  {
    /* there is an empty NDP entry */
    entryIdx = entryIdxFree;
  }
  else if(entryIdxOldest != TCPIP_NDP_CACHESIZE_MAX)
  {
    TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdxOldest].neighborAddr.State =
        TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
    entryIdx = entryIdxOldest;
    *replace = TRUE;
  }
  else
  {
    /* no (unlocked) existing entry, no free entry, all other entries are locked -
     * quite unrealistic but in this case we can't update the NDP-table */
  }

  return (uint16) entryIdx;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ndp_NCache_getExistingEntry
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipaddr
)
{
  uint16_least i;
  uint16_least existingEntry = TCPIP_NDP_CACHESIZE_MAX;

  /* go through all entries */
  for(i = 0; i < TCPIP_NDP_CACHESIZE_MAX; ++i)
  {
    if(TcpIp_Ndp_NCache_Data [ctrlIdx][i].neighborAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
    {
       if(TCPIP_IPV6_ADDR_CMP(TcpIp_Ndp_NCache_Data[ctrlIdx][i].neighborAddr.Inet6Addr, ipaddr))
       {
         existingEntry = i;
         break;
       }
    }
  }

  return (uint16) existingEntry;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_entry_FREE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  uint8 ctrlIdx
)
{
  TcpIp_Ndp_NCache_initEntry(entry);
#if (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
  TcpIp_Ndp_DestCache_removeNextHopEntry(ctrlIdx,entry->neighborAddr.Inet6Addr);
#else
  TS_PARAM_UNUSED(ctrlIdx);
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_initEntry
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
)
{
  entry->time = 0U;
  entry->transCnt = 0U;
  entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
  entry->life_time = 0U;
  entry->isRouter = FALSE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_timeout
(
  uint8 ctrlIdx,
  uint16 entryIdx
)
{
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
  uint8 remEthAddr[TCPIP_ETH_ADDR_LENGTH];
  uint32 remIpAddr[TCPIP_IPV6_ADDR_SIZE];
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  boolean transmitSolicit = FALSE;
  TcpIp_SockAddrInet6Type remoteAddr;
  uint32 targetAddr[TCPIP_IPV6_ADDR_SIZE];
  uint16 remoteAddrHandle = entryIdx;

  remoteAddr.domain = TCPIP_AF_INET6;
  /* !LINKSTO TcpIp.Design.Ndp.EA1.mainFunction,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  {
    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
      &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];
    switch(entry->neighborAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE: /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:      /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:      /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
      {
        /* increment life_time with every tick */
        if( entry->life_time < 0xffffffffU)
        {
          entry->life_time++;
        }
        /* decrement time with every tick */
        entry->time--;

        /* when delay time was exceeded execute timeout */
        if(entry->time == 0U)
        {
          switch(entry->neighborAddr.State)
          {
            case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
            {
              /* max wait time for an neighbor advertisement replay reached,
                 retransmit solicitation */
              if(entry->transCnt >= TCPIP_NDP_NUM_MULTICAST_SOLICIT_MAX)
              {
                /* tr_timeout_INCOMPLETE_FREE */
                TcpIp_Ndp_NCache_entry_FREE(entry,ctrlIdx);
              }
              else
              {
                /* tr_timeout_INCOMPLETE_INCOMPLETE */
                entry->time = TCPIP_NDP_SOLICIT_DELAY;
                entry->transCnt++;

                /* link layer address unknown send packet to solicit multicast address */
                TcpIp_IpV6_getSolicitMulti(remoteAddr.addr, entry->neighborAddr.Inet6Addr);
                TCPIP_IPV6_ADDR_CPY(targetAddr,  entry->neighborAddr.Inet6Addr);
                remoteAddrHandle = TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST;
                transmitSolicit = TRUE;
              }
              break;
            }
            case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:
            {
              /* tr_timeout_DELAY_PROBE */
              entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE;
              entry->time = TCPIP_NDP_SOLICIT_DELAY;
              entry->transCnt++;
              entry->lock++;

              /* link layer address known send packet to remote host address */
              TCPIP_IPV6_ADDR_CPY(remoteAddr.addr,  entry->neighborAddr.Inet6Addr);
              TCPIP_IPV6_ADDR_CPY(targetAddr,  entry->neighborAddr.Inet6Addr);
              remoteAddrHandle = entryIdx;
              transmitSolicit = TRUE;
              break;
            }
            case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:
            {
              if(entry->transCnt >= TCPIP_NDP_NUM_UNICAST_SOLICIT_MAX)
              {
                /* tr_timeout_PROBE_FREE */
                TcpIp_Ndp_NCache_entry_FREE(entry,ctrlIdx);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
                TCPIP_IPV6_ADDR_CPY(remIpAddr, entry->neighborAddr.Inet6Addr);
                TcpIp_EthAddrCpy(remEthAddr,entry->neighborAddr.PhysAddr);
                ethAddrChanged = TRUE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
              }
              else
              {
                entry->time = TCPIP_NDP_SOLICIT_DELAY;
                entry->transCnt++;
                entry->lock++;

                /* link layer address known send packet to remote host address */
                TCPIP_IPV6_ADDR_CPY(remoteAddr.addr,  entry->neighborAddr.Inet6Addr);
                TCPIP_IPV6_ADDR_CPY(targetAddr,  entry->neighborAddr.Inet6Addr);
                remoteAddrHandle = entryIdx;
                transmitSolicit = TRUE;
              }
              break;
            }
            case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
            {
              /* tr_timeout_STALE_STALE */
              entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_STALE;
              break;
            }
            /* CHECK: NOPARSE */
            case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:  /* intended fall through */
            case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:   /* intended fall through */
            case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: /* intended fall through */
            default:
              /* should not happen */
              TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
              break;
            /* CHECK: PARSE */
          }
        }
        break;
      }
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
      {
        /* increment life_time with every tick */
        if( entry->life_time < 0xffffffffU)
        {
          entry->life_time++;
        }
        break;
      }
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        /* do nothing entry is unused */
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.mainFunction,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  if(transmitSolicit)
  {
    /* Deviation MISRAC2012-1 */
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr =
     (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA))&remoteAddr;
    /* get local ip address through source address selection */
    TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx(ctrlIdx, remoteAddrPtr);

    (void)TcpIp_Ndp_NCache_transmitNeighborSolicit
                                    (
                                     localAddrId,
                                     &remoteAddr,
                                     targetAddr,
                                     remoteAddrHandle,
                                     FALSE
                                   );
    /* if entry was locked unlock the entry */
    if(remoteAddrHandle != TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST)
    {
      TcpIp_Ndp_NCache_unlock(remoteAddrHandle,ctrlIdx);
    }
  }

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(ethAddrChanged == TRUE)
  {
    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate NDP table change to upper layer */
    TcpIp_NdpChangeFct(ctrlIdx,remIpAddr,remEthAddr, FALSE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_updateAdvert
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
)
{
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)*/
#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
  boolean isRouter = FALSE;
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

  TCPIP_PRECONDITION_ASSERT( ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID );

  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  {
    /* Find the entry in our neighbor cache  */
    const uint16 entryIdx = TcpIp_Ndp_NCache_getExistingEntry(ctrlIdx, remoteAddrPtr);

    TCPIP_PRECONDITION_ASSERT( entryIdx <= TCPIP_NDP_CACHESIZE_MAX, TCPIP_INTERNAL_API_ID );
    if(entryIdx != TCPIP_NDP_CACHESIZE_MAX)
    {
      CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
        &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
      isRouter = entry->isRouter;
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

      switch(entry->neighborAddr.State)
      {
        case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        {
          TCPIP_PRECONDITION_ASSERT((entry->lock == 0U), TCPIP_INTERNAL_API_ID);

          if(ll_target_addr != NULL_PTR)
          {
            if ((flags & TCPIP_NDP_FLAG_S) == TCPIP_NDP_FLAG_S)
            {
              TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_REACHABLE(entry, ll_target_addr, flags);
            }
#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
            else
            {
              TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_STALE(entry, ll_target_addr, flags);
            }
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
            ethAddrChanged = TRUE;
#endif /*(TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)*/
          }
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
              TcpIp_Ndp_NCache_tr_update_PROBE(entry, ll_target_addr, flags);
          break;
        }
#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
        case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif /*(TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)*/
          TcpIp_Ndp_NCache_tr_update_DELAY(entry, ll_target_addr, flags);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          (void)
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          TcpIp_Ndp_NCache_tr_updateAdvert_REACHABLE(entry, ll_target_addr, flags);
          break;
        }
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
        {
          TCPIP_PRECONDITION_ASSERT((entry->lock == 0U), TCPIP_INTERNAL_API_ID);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged =
#else/* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
            (void)
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
              TcpIp_Ndp_NCache_tr_updateAdvert_STALE(entry, ll_target_addr, flags);
          break;
        }
#else /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */
          case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:       /* intended fall through */
          case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:   /* intended fall through */
          case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:       /* intended fall through */
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
          /* ignore advertisement */
          break;
        /* CHECK: NOPARSE */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }

#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
      /* if router flag changed from true to false, remove default router list */
      isRouter = (isRouter && (!entry->isRouter)) ? TRUE : FALSE;
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
    }
  }
  /* !LINKSTO TcpIp.Design.Ndp.EA1.rxIndication,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
  if (isRouter)
  {
    TcpIp_Ndp_Rpd_RouterList_remove(ctrlIdx, remoteAddrPtr);
  }
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(ethAddrChanged == TRUE)
  {
    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate NDP table change to upper layer */
    TcpIp_NdpChangeFct(ctrlIdx,remoteAddrPtr,ll_target_addr, TRUE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_Ndp_NCache_setRemoteAddr
(
  uint8 ctrlIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
)
{
  uint16 entryIdx;
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
  uint8 oldEthAddr[TCPIP_ETH_ADDR_LENGTH];
  uint32 oldIpAddr[TCPIP_IPV6_ADDR_SIZE];
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  boolean replaceEntry = FALSE;

  TCPIP_PRECONDITION_ASSERT( ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID );

  /* !LINKSTO TcpIp.Design.Ndp.EA1.setRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  /* search for existing entries, add a new entry if none exists */
  entryIdx = TcpIp_Ndp_NCache_getEntry(ctrlIdx, IpAddrPtr, &replaceEntry);

  TCPIP_PRECONDITION_ASSERT( entryIdx <= TCPIP_NDP_CACHESIZE_MAX, TCPIP_INTERNAL_API_ID );
  if(entryIdx != TCPIP_NDP_CACHESIZE_MAX)
  {

    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) ndpEntryPtr =
      &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

    if(ndpEntryPtr->lock == 0U)
    {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
      if(replaceEntry == TRUE)
      {
        /* copy old ARP entry data to temporary storage for callout */
        TCPIP_IPV6_ADDR_CPY(oldIpAddr, ndpEntryPtr->neighborAddr.Inet6Addr);
        TcpIp_EthAddrCpy(oldEthAddr,ndpEntryPtr->neighborAddr.PhysAddr);
      }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

      switch(ndpEntryPtr->neighborAddr.State)
      {
        case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:         /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          ethAddrChanged = TRUE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          break;
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:       /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:    /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:        /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:        /* intended fall through */
        case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          /* check if physical address has changed */
          ethAddrChanged = TcpIp_EthAddrCmp(ndpEntryPtr->neighborAddr.PhysAddr,PhysAddrPtr);
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
          break;
        /* CHECK: NOPARSE */
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }

      if(State == TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE)
      {
        ndpEntryPtr->time = TCPIP_NDP_REACHABLE_CONFIRM_DELAY;
      }

      TCPIP_IPV6_ADDR_CPY(ndpEntryPtr->neighborAddr.Inet6Addr, IpAddrPtr);
      TcpIp_EthAddrCpy(ndpEntryPtr->neighborAddr.PhysAddr, PhysAddrPtr);
      ndpEntryPtr->neighborAddr.State = State;
      ndpEntryPtr->transCnt = 0U;
      result = TCPIP_OK;
    }
  }

  /* !LINKSTO TcpIp.Design.Ndp.EA1.setRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old NDP entry */
    TcpIp_NdpChangeFct(ctrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }

  if(ethAddrChanged == TRUE)
  {
    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate NDP table change to upper layer */
    TcpIp_NdpChangeFct(ctrlIdx,IpAddrPtr,PhysAddrPtr, TRUE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_Ndp_NCache_removeRemoteAddr
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr,
  uint8 ctrlIdx
)
{
  uint16 entryIdx;
  TcpIp_ReturnType result = TCPIP_E_PHYS_ADDR_MISS;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
  uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  /* !LINKSTO TcpIp.Design.Ndp.EA1.setRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  entryIdx = TcpIp_Ndp_NCache_getExistingEntry(ctrlIdx, ipAddr);

  if(entryIdx != TCPIP_NDP_CACHESIZE_MAX)
  {
    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) ndpEntryPtr =
      &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

    switch(ndpEntryPtr->neighborAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        TcpIp_Ndp_NCache_entry_FREE(ndpEntryPtr,ctrlIdx);
        result = TCPIP_OK;
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:    /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY:     /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE:     /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STALE:
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
        ethAddrChanged = TRUE;
        TcpIp_EthAddrCpy(ethAddr,ndpEntryPtr->neighborAddr.PhysAddr);
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        TcpIp_Ndp_NCache_entry_FREE(ndpEntryPtr,ctrlIdx);
        result = TCPIP_OK;
        break;
      /* CHECK: NOPARSE */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:      /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.Ndp.EA1.setRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(ethAddrChanged == TRUE)
  {
    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate NDP table change to upper layer */
    TcpIp_NdpChangeFct(ctrlIdx,ipAddr,ethAddr, FALSE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  return result;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_lookup_STALE_DELAY
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
)
{
  /* exit code */

  /* transitional code */
  {
    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY;
    entry->time = TCPIP_NDP_DELAY_TIMEOUT;
    entry->transCnt = 0U;
  }

  /* entry code */
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_lookup_FREE_INCOMPLETE
(
  uint8 ctrlIdx,
  uint16 entryIdx,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddr
)
{
  /* value for unknown ethernet mac address */
  uint8 TcpIp_Ndp_NCache_EthUnknownAddress[TCPIP_ETH_ADDR_LENGTH] =
          {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

  /* exit code */

  TCPIP_PRECONDITION_ASSERT( ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID );
  TCPIP_PRECONDITION_ASSERT( entryIdx < TCPIP_NDP_CACHESIZE_MAX, TCPIP_INTERNAL_API_ID );
  {
    /* transitional code */
    CONSTP2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry =
        &TcpIp_Ndp_NCache_Data[ctrlIdx][entryIdx];

    TCPIP_IPV6_ADDR_CPY(entry->neighborAddr.Inet6Addr, ipAddr);

    TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, TcpIp_Ndp_NCache_EthUnknownAddress);

    entry->time = TCPIP_NDP_SOLICIT_DELAY;
    entry->transCnt = 1U;
    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE;
    entry->life_time = 0U;
    entry->isRouter = FALSE;

    /* entry code */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
)
{
  /* exit code */

  /* transitional code */
  {
    /* record link  layer address */
    TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, ll_target_addr);

    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE;
    entry->time = TCPIP_NDP_REACHABLE_CONFIRM_DELAY;
    entry->transCnt = 0U;
    entry->isRouter = (flags & TCPIP_NDP_FLAG_R) >> TCPIP_NDP_FLAG_R_OFFSET;
  }

  /* entry code */

}
#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_INCOMPLETE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
)
{
  /* exit code */

  /* transitional code */
  {
    /* record link  layer address */
    TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, ll_target_addr);

    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_STALE;
    entry->time = 0U;
    entry->transCnt = 0U;
    entry->isRouter = (flags & TCPIP_NDP_FLAG_R) >> TCPIP_NDP_FLAG_R_OFFSET;
  }

  /* entry code */
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
)
{
  boolean ethAddrChanged = TcpIp_Ndp_NCache_tr_updateAdvert_STALE(entry, ll_target_addr, flags);

  /* Solicitation flag 0 or 1 */
  if ((entry->lock > 0) || ((flags & TCPIP_NDP_FLAG_O) == 0U))
  {
    /* if source link-layer address is not equal to cached link-layer
       address change state to STALE, but do not update the address */
    if(ll_target_addr != NULL_PTR)
    {
      if(TcpIp_EthAddrCmp(ll_target_addr, entry->neighborAddr.PhysAddr))
      {
        TcpIp_Ndp_NCache_tr_update_REACHABLE_STALE(entry);
      }
    }
  }

  return ethAddrChanged;
}
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  uint8 flags
)
{
  boolean ethAddrChanged = FALSE;
  boolean isRouter = (flags & TCPIP_NDP_FLAG_R) >> TCPIP_NDP_FLAG_R_OFFSET;

  if ((flags & TCPIP_NDP_FLAG_S) == TCPIP_NDP_FLAG_S)
  {
    if((flags & TCPIP_NDP_FLAG_O) == TCPIP_NDP_FLAG_O)
    {
      boolean changed = FALSE;

      if(ll_target_addr != NULL_PTR)
      {
        changed = TcpIp_EthAddrCmp(ll_target_addr, entry->neighborAddr.PhysAddr);
      }

      if((entry->lock == 0) && (changed))
      {
        /* if link layer address different and the entry is not locked,
           copy it into cache */
        TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, ll_target_addr);
        ethAddrChanged = TRUE;
      }

      if((entry->lock == 0) || (!changed))
      {
        /* if entry is not locked or link layer address has not changed, set
         * state to reachable */
        TcpIp_Ndp_NCache_tr_updateAdvert_STALE_REACHABLE(entry, isRouter);
      }
    }
    else /* Override flag == 0 */
    {
      if(ll_target_addr != NULL_PTR)
      {
        if(!TcpIp_EthAddrCmp(ll_target_addr, entry->neighborAddr.PhysAddr))
        {
          /* if link layer address are equal set state to reachable */
          TcpIp_Ndp_NCache_tr_updateAdvert_STALE_REACHABLE(entry, isRouter);
        }
      }
      else
      {
        /* if link layer address was not provided set state to reachable */
        TcpIp_Ndp_NCache_tr_updateAdvert_STALE_REACHABLE(entry, isRouter);
      }
    }
  }
#if (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF)
  else  /* Solicitation flag == 0 */
  {
    if ((flags & TCPIP_NDP_FLAG_O) == TCPIP_NDP_FLAG_O)
    {
      if(ll_target_addr != NULL_PTR)
      {
        if(TcpIp_EthAddrCmp(ll_target_addr, entry->neighborAddr.PhysAddr))
        {
          if(entry->lock == 0)
          {
            /* if source link-layer address is not equal to cached link-layer
               address change state to STALE */
            TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, ll_target_addr);
            entry->isRouter = isRouter;
            ethAddrChanged = TRUE;
            TcpIp_Ndp_NCache_tr_updateAdvert_STALE_STALE(entry);
          }
        }
      }
    }
  }
#endif /* (TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED == STD_OFF) */

  return ethAddrChanged;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  boolean isRouter
)
{
  /* exit code */

  /* transitional code */
  {
    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE;
    entry->time = TCPIP_NDP_REACHABLE_CONFIRM_DELAY;
    entry->transCnt = 0U;
    entry->isRouter = isRouter;
  }

  /* entry code */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateAdvert_STALE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry
)
{
  /* exit code */

  /* transitional code */
  {
    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_STALE;
    entry->time = 0U;
    entry->transCnt = 0U;
  }

  /* entry code */
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_FREE_STALE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ll_target_addr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  boolean isRouter
)
{
  /* exit code */

  /* transitional code */
  {
    TCPIP_IPV6_ADDR_CPY(entry->neighborAddr.Inet6Addr, remoteAddrPtr);
    TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, ll_target_addr);
    entry->neighborAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_STALE;
    entry->isRouter = isRouter;
    entry->time = 0U;
    entry->transCnt = 0U;
    entry->life_time = 0U;
  }

  /* entry code */
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_REACHABLE
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
)
{
  boolean ethAddrChanged = FALSE;

  /* associated link layer address is not equal to stored one */
  if(linkAddr != NULL_PTR)
  {
    if(TcpIp_EthAddrCmp(linkAddr, entry->neighborAddr.PhysAddr) == TRUE)
    {
      if(entry->lock == 0U)
      {
        /* copy new link layer address, only if entry is not locked */
        TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, linkAddr);
        ethAddrChanged = TRUE;
      }

      /* reachability is uncertain set state to stale */
      TcpIp_Ndp_NCache_tr_updateSolicit_All_STALE(entry);
    }
  }

  entry->isRouter = isRouter;

  return ethAddrChanged;
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Ndp_NCache_tr_updateSolicit_All
(
  P2VAR(TcpIp_Ndp_NCache_EntryType, AUTOMATIC, TCPIP_APPL_DATA) entry,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) linkAddr,
  boolean isRouter
)
{
  boolean ethAddrChanged = FALSE;

  /* only update entry if it is unlocked */
  if(entry->lock == 0U)
  {
    /* associated link layer address is not equal to stored one
       (maybe unknown in case of incomplete state) */
    if(linkAddr != NULL_PTR)
    {
      if(TcpIp_EthAddrCmp(linkAddr, entry->neighborAddr.PhysAddr) == TRUE)
      {
        /* copy new link layer address */
        TcpIp_EthAddrCpy(entry->neighborAddr.PhysAddr, linkAddr);
        entry->isRouter = isRouter;

        ethAddrChanged = TRUE;

        /* reachability is uncertain set state to stale */
        TcpIp_Ndp_NCache_tr_updateSolicit_All_STALE(entry);
      }
    }
  }

  return ethAddrChanged;
}


#if TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON

STATIC FUNC(void, TCPIP_CODE) TcpIp_NdpChangeFct
(
  uint8 ctrlIdx,
  P2CONST(uint32,AUTOMATIC,TCPIP_APPL_DATA) ipAddrPtr,
  P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) physAddrPtr,
  boolean valid
)
{
  /* indicate NDP table change to upper layer */
  TcpIp_SockAddrInet6Type ipSockAddr;
  TCPIP_IPV6_ADDR_CPY(ipSockAddr.addr, ipAddrPtr);
  ipSockAddr.domain = TCPIP_AF_INET6;

  /* Deviation MISRAC2012-1 */
  TcpIp_PhysAddrTableChgAPI
    ((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,(P2VAR(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA))&ipSockAddr,physAddrPtr,valid);
}

#endif /* TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON */

#endif /* TCPIP_IPV6_ENABLED == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
