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
#ifndef TCPIP_IP_API_H
#define TCPIP_IP_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Cbk.h>            /* TcpIp_RxIndication() function declaration */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */


#include <TcpIp_Ip_Api.guards>
/*==================[macros]================================================*/

/* \brief link layer address is fixed and does not timeout */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC                        0x01U
/* \brief link layer address of host is currently evaluated */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE                    0x02U
/* \brief host is reachable, link layer address is valid */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE                     0x04U
/* \brief reachability confirmation has not been received in a long time,
          validation of link layer address uncertain */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_STALE                         0x08U
/* \brief waiting for reachability confirmation */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_DELAY                         0x10U
/* \brief probe reachability of host */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_PROBE                         0x20U
/* \brief Cache entry is not used */
#define TCPIP_PHYS_ADDR_ENTRY_STATE_FREE                          0x40U

/*==================[type definitions]======================================*/

/** \brief data was accepted */
#define TCPIP_IP_RX_OK                    0x00U
/** \brief data was not accepted */
#define TCPIP_IP_RX_NOT_OK                0x01U
/** \brief data was not delivered to transport protocol - protocol unknown*/
#define TCPIP_IP_RX_PROTOCOL_UNREACHABLE  0x02U
/** \brief data was not accepted by transport protocol - port unknown*/
#define TCPIP_IP_RX_PORT_UNREACHABLE      0x03U
/** \brief data was not delivered to transport protocol - protocol unknown*/
#define TCPIP_IP_RX_PROTOCOL_UNREACHABLE_FRAG  0x04U
/** \brief extension header successfully processed */
#define TCPIP_IP_RX_EXT_HDR_OK            0x05U
/** \brief data was not accepted because of unrecognized ipv6 option code */
#define TCPIP_IP_RX_UNRECOGNIZED_OPTION   0x06U

/* predefined IPv4 ICMP error numbers end here */

/** \brief data was not delivered to transport protocol - no more buffers to handle */
#define TCPIP_IP_RX_FRAGMENT_OUT_OF_BUFFERS 0x07U
/** \brief Fragment header too short */
#define TCPIP_IP_RX_FRAGMENT_HEADER_SHORT 0x08U
/** \brief Timeout during fragment reassembly */
#define TCPIP_IP_RX_FRAGMENT_TIMEOUT      0x09U
/** \brief Length problem during fragment reassembly */
#define TCPIP_IP_RX_FRAGMENT_LENGTH       0x0AU
/** \brief Offset problem during fragment reassembly */
#define TCPIP_IP_RX_FRAGMENT_OFFSET       0x0BU
/** \brief Overlap problem during fragment reassembly */
#define TCPIP_IP_RX_FRAGMENT_OVERLAP      0x0CU
/** \brief Reserved flag usage problem during fragment reassembly */
#define TCPIP_IP_RX_FRAGMENT_RESERVED     0x0DU
/** \brief Reserved flag Segments Left is non-zero with unrecognized Routing Type */
#define TCPIP_IP_RX_UNRECOGNIZED_ROUTING_TYPE 0x0EU

/** \brief Basic header check failed */
#define TCPIP_IP_RX_INVALID_HEADER        0x0FU
/** \brief Destination address not known to host */
#define TCPIP_IP_RX_DEST_UNREACHABLE      0x10U
/** \brief Data not accepted because of invalid source address */
#define TCPIP_IP_RX_SRCADDR_INVALID       0x11U

/** \brief Reserved values for policy check handlers start here */
#define TCPIP_IP_RX_RES_POLICYCHECK       0x60U

/** \brief Return type used for processing of received datagrams inside TcpIp_RxIndication */
typedef uint8 TcpIp_Ip_RxReturnType;

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Set physical address of remote host
 *
 * This service adds, updates or removes a physical address from a remote host in the ARP/NDP table
 *
 * \param[in]  CtrlIdx      EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  IpAddrPtr    specifies the IP address for which the physical address shall be
 *                          set in the ARP/NDP table
 * \param[in]  PhysAddrPtr  specifies the physical address which shall be set in the ARP/NDP table
 * \param[in]  State        TCPIP_PHYS_ADDR_ENTRY_STATE_FREE: remove the specified entry in the
 *                          ARP/NDP table
 *                          TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: add/update the specified
 *                          entry in the ARP/NDP table
 *                          TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: set the specified entry in
 *                          the ARP/NDP table to static (entry will remain in the cache until
 *                          it is deleted with TcpIp_SetRemotePhysAddr or TcpIp is reinitialized)
 *
 * \return Result of operation
 * \retval TCPIP_OK                physical address successfully added/updated/removed
 * \retval TCPIP_E_NOT_OK          The request was not successful, e.g. invalid controller index
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address currently unknown (in case the entry shall be removed)
 *
 * \ServiceID{0xF0}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_SetRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief Retrieve physical address from remote host
 *
 * This service queries the IP/physical address translation table specified by CtrlIdx
 * and returns the physical address related to the IP address  specified by IpAddrPtr.
 *
 * \param[in]  CtrlIdx      EthIf controller index to identify the related ARP/NDP table.
 * \param[in]  IpAddrPtr    specifies the IP address for which the physical address shall be retrieved
 * \param[out] PhysAddrPtr  Pointer to the memory where the physical address (MAC  address)
 * \param[in]  initRes      specifies if the address resolution shall be initiated (TRUE) or not
 *                          (FALSE) in case the physical address related to the specified IP
 *                          address is currently unknown.
 *
 * \return Result of operation
 * \retval TCPIP_E_OK              specified IP address resolved, physical address provided via
 *                                 PhysAddrPtr
 * \retval TCPIP_E_NOT_OK          The request was not successful, e.g. invalid controller index
 * \retval TCPIP_E_PHYS_ADDR_MISS  physical address currently unknown (address resolution initiated
 *                                 if initRes set to TRUE)
 *
 * \ServiceID{0x16}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_GetRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
);

#if (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON)

/** \brief Requests to transmit an ICMP message
 *
 * This service transmits an ICMP message
 *
 * \param[in] LocalIpAddrId Id of Local Address used for transmission.
 * \param[in] RemoteAddrPtr IP address of the remote host to transmit to.
 * \param[in] Ttl   Time-to-Live used for Ip header
 * \param[in] Type  ICMP Type to write into ICMP header
 * \param[in] Code  ICMP Code to write into ICMP header
 * \param[in] DataLength Data length of the data to be transmitted.
 * \param[in] DataPtr DataPtr points to a linear buffer of DataLength bytes.
 *
 * \return Result of operation
 * \retval E_OK      UDP message transmission was successful.
 * \retval E_NOT_OK  UDP message transmission failed (E.g. caused by ARP cache miss).
 *
 * \ServiceID{0x0C}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpTransmit
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  uint8 Ttl,
  uint8 Type,
  uint8 Code,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
);

# if (TCPIP_ICMPTRANSMITERROR_API == STD_ON)
/** \brief Requests to transmit an ICMP message based on a (EthIf) CtrlIdx
 *
 *  This function constructs the ICMP header and payload and and invokes the transmission of the Icmp
 *  frame over EthIf.
 *
 * \param[in] CtrlIdx Index of the ethernet controller (EthIfCtrlIdx).
 * \param[in] RemoteAddrPtr Pointer to remote IP address.
 * \param[in] Ttl   Time-to-Live/Hop Limit used for Ip header
 * \param[in] Type  ICMP Message Type to write into ICMP header
 * \param[in] Code  ICMP Error Code to write into ICMP header
 * \param[in] DataLength Data Length of payload to transmit.
 * \param[in] DataPtr Points to the received data that contains the payload of the Icmp message
 * \param[in] SpecificValue Pointer to value set before the payload.
 *
 *
 * \return Result of operation
 * \retval E_OK      Transmission request has been successfully performed.
 * \retval E_NOT_OK  Transmission request failed.
 *
 * \ServiceID{0xF2}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpTransmitError
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  uint8 Ttl,
  uint8 Type,
  uint8 Code,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) SpecificValue
);
# endif /* (TCPIP_ICMPTRANSMITERROR_API == STD_ON) */

#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_IP_API_H */
/*==================[end of file]===========================================*/
