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
#ifndef TCPIP_IPSEC_H
#define TCPIP_IPSEC_H

/*==================[inclusions]============================================*/

#include <TcpIp_IpSec_Api.h>      /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h>         /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_Ip_Int.h>         /* used SW-unit interface file */

/*==================[macros]================================================*/
/** \brief manual SA */
#define TCPIP_IPSEC_MANUAL_SA      1U

/*==================[type definitions]======================================*/

/** \brief Structure containing the data describing an IpSec SA. */
typedef struct
{
  /** \brief Security Parameter Index (SPI). */
  uint32 spi;
  /** \brief SA sequence number lower 32 bits. */
  uint32 SeqNumLow;
  /** \brief SA sequence number higher 32 bits. */
  uint32 SeqNumHigh;
  /** \brief Identifier of the Csm key that the crypto operations are performed with. */
  uint32 keyId;
  /** \brief Identified of the Csm job that the crypto operations are performed with. */
  uint32 jobId;
  /** \brief Length of the Authentication header in bytes. */
  uint8 securityHeadersize;
  /** \brief Length of the Message Authentication Code (MAC) in bytes. */
  uint8 MacLen;
  /** \brief Flag indicating wether this SA uses GMAC (TRUE) or not (FALSE). */
  boolean useGmac;
} TcpIp_IpSecSaDataType;

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initialize the IpSec unit
 *
 * This service initialize the IpSec relevant runtime data.
 *
 *
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSec_init
(
  void
);

/** \brief Handle the reception of an AH within an IP datagram
 *
 * This service handles the processing of the Authentication Header (AH) of a received IP datagram.
 *
 * \param[in,out] dataPtr           Reference to the start of the received IP dagram with an AH
 * \param[in,out] dataLenPtr        Input: Reference to the the length of the whole IP datgram.
 *                                  Output: Reference to the length of the payload following the AH
 * \param[out]    protocolPtr       The protocol ID of the payload following the AH indicated by its
 *                                  "Next Header" field
 * \param[in]     ahOffset          Index of the beginning of the AH from dataPtr
 * \param[in]     localAddrId       Address ID of the local address
 * \param[in]     remoteAddrPtr     Reference to the start of the remote address
 * \param[in]     ipsecSaIdxPtr     Index of the of the corresponding security
 *                                  association (SA)
 *
 * \return Result of operation
 * \retval TCPIP_OK                AH successfully processed AND IP datagram successfully
 *                                 authenticated
 * \retval TCPIP_E_NOT_OK          Request was not successful, i.e. either processing or
 *                                 authentication failed
 *
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpSec_ahRxIndication
(
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) dataLenPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) protocolPtr,
  uint16 ahOffset,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipsecSaIdxPtr
);

/** \brief Mainfunction of the IpSec unit
 *
 *  This function maintains the IpSec unit and executes periodic tasks
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpSec_mainFunction
(
  void
);

/** \brief Build an AH for a outgoing IP datagram
 *
 * This service handles the generation of an Authentication Header (AH) of a Ip datagram that is to
 * be transmitted.
 *
 * \param[in]     ipParameterPtr  Reference to the structure containing information about the IP
 *                                parameters of the outgoing fragment/datagram.
 * \param[in,out] ethTxHandlePtr  Reference to the structure containing information about the
 *                                outgoing Ethernet frame, e.g. the location of the buffer.
 * \param[in]     frgCnt          Number of fragments the outgoing datagram is split into.
 * \param[in]     AhOffset        Offset of the start of the AH from the beginning og the datagram.
 *
 * \return Result of operation
 * \retval TCPIP_OK                AH successfully generated.
 * \retval TCPIP_E_NOT_OK          AH generation failed.
 *
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_buildAh
(
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 frgCnt,
  uint8 AhOffset
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#endif /* ifndef TCPIP_IPSEC_H */
/*==================[end of file]===========================================*/
