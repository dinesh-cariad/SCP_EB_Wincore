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
#ifndef TCPIP_IPSECDB_INT_H
#define TCPIP_IPSECDB_INT_H

/*==================[inclusions]=============================================*/

#include <TcpIp_guards.h>         /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_IpSec_Int.h>      /* module internal headerfile */
#include <TcpIp_Crypto.h>         /* used SW-unit interface file */

/*==================[macros]=================================================*/

/** \brief Internal values of IpSec mechanism */
#define TCPIP_IPSECDB_MECHANISM_DISCARDED                                   1U
#define TCPIP_IPSECDB_MECHANISM_BYPASSED                                    2U
#define TCPIP_IPSECDB_MECHANISM_SECURED                                     3U

#define TCPIP_IPSECDB_INVALID_SA_INDEX                                    255U
#define TCPIP_IPSECDB_INVALID_DATAIDX                                     255U

#define TCPIP_IPSECDB_DIRECTION_INBOUND                                     0U
#define TCPIP_IPSECDB_DIRECTION_OUTBOUND                                    1U

#define TCPIP_IPSECDB_DIAG_E_AUTH_FAILED_LOCAL                           0x80U
#define TCPIP_IPSECDB_DIAG_E_AUTH_FAILED_REMOTE                          0x81U
#define TCPIP_IPSECDB_DIAG_E_NO_PROPOSAL_CHOSEN_LOCAL                    0x82U
#define TCPIP_IPSECDB_DIAG_E_NO_PROPOSAL_CHOSEN_REMOTE                   0x83U

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
/** \brief Number of configured IpSec Connections */
#define TCPIP_IPSEC_NUM_CONNECTIONS              TcpIp_PBcfgPtr->numOfIpSecCon
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/*==================[type definitions]======================================*/

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
/** \brief Initialize the IpSecDb unit
 *
 * This service initialize the IpSecDb relevant runtime data.
 *
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_init
(
  void
);

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
/**  \brief Check the presence of a security policy for inbound packets
  *
  * This service checks if there is an IpSec security policy of an incoming packet based on the
  * local and remote IP address pair.
  *
  * \param[in] localAddr       Identifier of the local IP address
  * \param[in] remoteAddrPtr   Reference to the remote address
 * \param[in] physAddrPtr      Points to MAC address of remote host (source MAC)
  *
  * \return Result of operation
  * \retval E_OK remote   Address match
  * \retval E_NOT_OK      No address match
  *
  * \Reentrancy{Non-Reentrant}
  * \Synchronicity{Synchronous}
  */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityPolicyAddr
(
  TcpIp_LocalAddrIdType localAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

/** \brief Get the security association for an incoming packet
  *
  * This service checks if an incoming packet matches a SA in the database, and if so, returns it.
  *
  * \param[in] spi              SPI to identify the SA on the receiver side
  * \param[in] seqNum           Sequence number of the received frame
  * \param[in] localAddr        Identifier of the local (i.e. receiver's) IP address
  * \param[in] remoteAddrPtr    Reference to the remote (i.e. the transmitter's) IP address
  * \param[out] ipSecSaIdxPtr   Index of the database element describing the SA of the match
  * \param[out] saDataPtr       Reference to the data where the SA information is written to
  *
  * \return Indicates wether a match was found
  * \retval E_OK      Matching security association could be found
  * \retval E_NOT_OK  No security association found
  */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpSecDb_getSecurityRxAssociation
(
  uint32 spi,
  uint32 seqNum,
  TcpIp_LocalAddrIdType localAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaIdxPtr,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr
);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/** \brief Check if an inbound packet should be processed according to the security polices.
  *
  * This service checks wether an inbound packet should be processed i.e. passed to the upper layer
  * (BYPASSED, SECURED with valid SA and IPsec connection, or IPsec disabled) or not (DISCARDED or
  * invalid SA/connection). This is done by checking the security policies in the database.
  *
  * \param[in] localAddrId    Identifier of the local (i.e. the receiver's) IP address
  * \param[in] remoteAddrPtr  Reference to the remote (i.e. the sender's) IP address
  * \param[in] dataPtr        Reference to the start of TCP/UDP/ICMP payload
  * \param[in] dataLength     Length of the payload
  * \param[in] nextHeader     upper layer protocol
  * \param[in] ipSecSaIdx     Index of the respective SA
 *  \param[in] physAddrPtr    Points to MAC address of remote host (source MAC)
  *
  * \return Indicates wether the inbound packet should be processed
  * \retval E_OK              security policy found
  * \retval E_NOT_OK          no security policy exists
  *
  * \Reentrancy{Non-Reentrant}
  * \Synchronicity{Synchronous}
  */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityRxPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLength,
  uint8 nextHeader,
  uint8 ipSecSaIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
/** \brief Start an IpSec connection
 *
 * This service sets the state of an IpSec connection to active and initializes its SA database
 * entry in case of manual configuration (i.e. IKE not being used).
 *
 * \param[in] conId   IpSec connection identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_start
(
  uint8 conId
);

/** \brief Stop an IpSec connection
 *
 * This service sets the state of an IpSec connection to inactive and frees the SA database entry
 * in case of manual configuration (i.e. IKE not being used).
 *
 * \param[in] conId   IpSec connection identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_stop
(
  uint8 conId
);

/** \brief Get the security association for an outgoing packet
  *
  * This service checks if an outgoing packet matches a SA in the database, and if so, returns it.
  *
  * \param[in] connectionIdx      Index of the connection
  * \param[out] saDataPtr         Reference to the SA description where the attributes of the
  *                               matching SA are saved
  * \param[in] getSeqNum          TRUE: increases the sequence number and returns it
  *                               FALSE: ignore sequence number
  *
  * \return Result of operation
  * \retval E_OK      Matching SA was found
  * \retval E_NOT_OK  No match was found
  */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpSecDb_getSecurityTxAssociation
(
  uint8 connectionIdx,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  boolean getSeqNum
);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/** \brief Check the security policy of an outbound packet.
  *
  * This service checks the security policy of an SA to determine wether an outbound packet should
  * be secured, not secured, or discarded.
  *
  * \param[in] localAddrId             Identifier of the local (i.e. the sender's) IP address
  * \param[in] remoteAddrPtr           Reference to the remote (i.e. the receiver's) IP address and port
  * \param[in] nextHeader              Number of the protocol following the AH (e.g. UDP, TCP, ICMP)
  * \param[in] localPort               Port from which the packet will be transmitted
  * \param[in] ipSecConnectionIdxPtr   Index of the connection
  *
  * \return Result of operation
  * \retval TCPIP_IPSECDB_MECHANISM_DISCARDED   Packet is discarded
  * \retval TCPIP_IPSECDB_MECHANISM_BYPASSED    Packet is bypassed
  * \retval TCPIP_IPSECDB_MECHANISM_SECURED     Packet is secured
  *
  * \Reentrancy{Non-Reentrant}
  * \Synchronicity{Synchronous}
  */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityTxPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr
);

#if ((TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON))
/** \brief Read and reset dropped/passed datagrams by security policies
 *
 * This function reads and resets dropped/passed datagrams by security policies.
 *
 * \param[in]  MeasurementIdx          Index to select specific measurement data
 * \param[in]  MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetPolicyCount
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)*/


#if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)

/** \brief Log IpSec, IpSecDb and IkeV2 errors
 *
 * This services handles logging of errors originating form IpSec, IpSecDb and IkeV2 using a callout
 * function.
 *
 * \param[in] localAddrId   Identification of the local IP address/Security Association
 * \param[in] remoteIpPtr   Reference to the remote address
 * \param[in] errorType     Error type
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_LogError
(
  uint8 localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpPtr,
  uint8 errorType
);

#endif /* #if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

#endif /* ifndef TCPIP_IPSECDB_INT_H */
/*==================[end of file]===========================================*/
