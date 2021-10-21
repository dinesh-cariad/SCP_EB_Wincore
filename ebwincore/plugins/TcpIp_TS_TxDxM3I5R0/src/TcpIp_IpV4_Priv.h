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

#ifndef TCPIP_IPV4_PRIV_H
#define TCPIP_IPV4_PRIV_H

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV4_Int.h>     /* module internal header file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief Macro to check if an address has a link local address prefix */
#define TCPIP_IPV4_ARP_ADDR_IS_LINKLOCAL(ipAddr)\
  (((ipAddr)[0U] == 169U) && ((ipAddr)[1U] == 254U))

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if (TCPIP_IPV4_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Returns the LocalAddrId for a given IpV4 address
 *
 * This function determines the local Address Id of the IpV4 address passed as parameter.
 *
 * \param[in] ipV4            IpV4 address.
 * \param[in] ctrlIdx Index of the TcpIp controller (TcpIpCtrlIdx)
 *
 * \return localAddrId found, TCPIP_LOCALADDRID_INVALID if lookup failed
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpV4_getLocalAddrId_byIpV4
(
  uint32 ipV4,
  uint8 ctrlIdx
);

#if (TCPIP_ICMPV4_ENABLED == STD_ON)
/** \brief Reception of Icmp datagram
 *
 * This API service indicates the reception of an Icmp frame.
 *
 * \param[in] remoteAddrPtr     IP address and port of the remote host.
 * \param[in] dataPtr           Points to the received data. The data contains the payload
 *                              of the Ip layer (which includes the Icmp header).
 * \param[in] localAddrId       Local address the frame has been received on.
 * \param[in] lenByte           Length of received data.
 * \param[in] ttl               TTL of received IP packet.
 * \param[in] isBroadcast       Indicates if the target MAC address is a broadcast address
 * \param[in] isFragmented      Indicates if a packet is fragmented
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE ) TcpIp_IpV4_rxIndicationIcmp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  uint8 ttl,
  boolean isBroadcast,
  boolean isFragmented
);
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */



/** \brief Initializes ARP functionality
 *
 * This API service performs the initialization of the ARP related data structures.
 */
TS_MOD_PRIV_DECL FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpInit(void);



/** \brief Time based scheduling of ARP functionality
 *
 * \param[in] timerTick     current time in ticks of TcpIp timer
 *
 * This API service performs the time-based tasks of the ARP functionality (ARP entry timeout detection).
 */
 TS_MOD_PRIV_DECL FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpMainFunction(uint16 timerTick);



/** \brief Inserts/Updates an ARP table entry and locks it for further usage
 *
 *  This function updates an existing matching ARP entry or inserts a new ARP entry.
 *  If no existing entry exists, a free ARP entry is used.
 *  If no free ARP entry exists the oldest ARP entry is replaced.
 *  If no existing entry exists and all ARP entries are locked, then the update/insert
 *  is discarded
 *
 * \param[in] ipAddr        IpV4 address to update
 * \param[in] physAddrPtr   Address of the remote physical address
 * \param[in] ctrlIdx       Index of the EthIf controller
 * \param[in] insert        TRUE: if no entry exists create a new entry
 *                          FALSE: do not create a new entry if none can be found
 */
 TS_MOD_PRIV_DECL FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpUpdate
 (
   uint32 ipAddr,
   P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
   uint8 ctrlIdx,
   boolean insert
 );



/** \brief Searches for an existing ARP entry, returns an identification handle
 *         and locks the entry.
 *
 * This function searches the ARP table for a entry matching the given IpV4-address
 * and performs an entry lock to avoid entry modifications (except timeout).
 * A handle identifying the entry is returned.
 *
 * \param[in] ipAddr                IpV4 address to update
 * \param[in] remoteAddrHandlePtr   Pointer the ARP entry handle is written to
 * \param[in] ctrlIdx               Index of the EthIf controller
 * \param[in] insert                TRUE: if no entry exists create a new entry
 *                                  FALSE: do not create a new entry if none can be
 *                                         found
 *
 * \return link layer address of remote host
 * \retval TCPIP_OK                 link layer address is valid
 * \retval TCPIP_E_PHYS_ADDR_MISS   link layer address is unknown (no entry exists)
 * \retval TCPIP_E_NOT_OK           address resolution is still ongoing
 */
TS_MOD_PRIV_DECL FUNC( TcpIp_ReturnType, TCPIP_CODE ) TcpIp_IpV4_ArpLookupAndLock
  (
    uint32 ipAddr,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
    uint8 ctrlIdx,
    boolean insert
  );


/** \brief Transmit an ARP request message.
 *
 * \param[in] remoteIpAddr     Remote IpV4 address
 * \param[in] localIpAddr      Local IpV4 address
 * \param[in] ctrlIdx          Index of the EthIf controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpTransmitRequest
(
  uint32 remoteIpAddr,
  uint32 localIpAddr,
  uint8 ctrlIdx
);

/** \brief Transmit an ARP announce message.
 *
 * \param[in] LocalIpAddr      Local address which shall be announced
 * \param[in] ctrlIdx          Index of the EthIf controller
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpTransmitAnnounce
(
  uint32 LocalIpAddr,
  uint8 ctrlIdx
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and reset the measurement number of replaced oldest Arp entries
 *
 * This function reads and resets dropped datagrams caused by replacing oldest Arp entries.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_GetAndResetReplacedArpEntryCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

/** \brief Read and reset dropped ARP add requests
 *
 * This function reads and resets dropped 'ARP add requests' caused by all ARP entries are locked or
 * are in state STATIC or INCOMPLETE
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_GetAndResetDroppedLockedArpEntryCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/** \brief Time based scheduling of Address conflict detection functionality
 *
 * \param[in] timerTick     current time in ticks of TcpIp timer
 *
 * This API service performs the time-based tasks of the  Address conflict detection
 * functionality (defend intervals).
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_mainFunction
(
  uint16 timerTick
);
/** \brief Initialize Address Conflict Detection and Defense functionality */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_init(void);

/** \brief Check for Address Conflicts
 *
 * \param[in] ctrlIdx      EthIf controller on which the ARP message was received on
 * \param[in] physAddrPtr  Source hardware address of the ARP message
 * \param[in] srcIpAddr    Source IP address of the ARP message
 *
 * This function checks received ARP message for address conflicts
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE ) TcpIp_IpV4_AddrConflDetect_arpDetection
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 srcIpAddr
);


/** \brief Checks if address is link local
 *
 *  address, i.e., in the range 169.254.0.0 through 169.254.255.255
 *
 *  \param[in]  ipAddr        IpV4 address
 *
 * \retval TRUE    address is link local
 * \retval FALSE   address is not link local
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_IsLinkLocalAddr
(
  uint32 ipAddr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_IPV4_PRIV_H */
