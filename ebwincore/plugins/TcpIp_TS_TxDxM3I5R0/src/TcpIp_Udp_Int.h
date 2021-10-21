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
#ifndef TCPIP_UDP_INT_H
#define TCPIP_UDP_INT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Udp_Api.h>        /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>            /* module internal headerfile */
#include <TcpIp_Ip_Int.h>         /* SW-Unit IP interface definitions */
#include <TcpIp_Socket_Int.h>     /* SW-Unit Socket interface definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if ( TCPIP_UDP_ENABLED == STD_ON )

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Udp_copyData
(
    P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)  that,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
    uint16 copyLength
);


/** \brief Performs Udp Checksum calculation
 *
 *  This function gets the partial Ip header Checksum passed via partialCRC and completes the Udp
 *  header Checksum caluclation.
 *  Finally it writes the Udp Checksum into the Udp header.
 *
 * \param[in] that              The functor's 'this' pointer.
 * \param[in] payloadSum        Checksum of the upper layer payload and ip header
 * \param[in] headerPtr         Points to data buffer where the Udp header is located.
 * \param[in] calculateChecksum If true calculate the UDP checksum
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Udp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
);


/** \brief Reception of Udp datagram
 *
 * By this API service the Udp SW-Unit gets an indication of a received Udp frame.
 *
 * \param[in] remoteAddrPtr  IP address and port of the remote host to transmit to.
 * \param[in] localIpAddrPtr IP address of the local host.
 * \param[in] dataPtr        Points to the received data. The data contains the payload of the
 *                           Ethernet protocol (which excludes the Ethernet header but includes
                             headers of higher layers).
 * \param[in] localAddrId    Local address the frame has been received on.
 * \param[in] lenByte        Length of received data.
 * \param[in] isFragmented   Is fragmented?
 *
 * \return Result of operation
 * \retval TCPIP_OK                 Udp datagram has been successfully delivered to the application.
 * \retval TCPIP_E_PORT_UNREACHABLE Udp datagram has not been delivered to application, no socket has been found.
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_Udp_rxIndication
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
);


/** \brief closes a Udp socket
 *
 *  This function closes a Udp socket, thus informs the upper ayer by calling Up_TcpIpEvent().
 *
 * \param[in]  socketId       socket which shall be closed.
 * \param[in]  socketOwnerId  upper layer of the socket.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Udp_close
(
  TcpIp_SocketIdType socketId,
  uint8 socketOwnerId
);


/** \brief Changes socket values (Ip layer runtime parameters for UDP transmission) to default values
 *
 * By this API service the UDP stack is requested to change all the Ip communication
 * parameters to default values according to the current post-build-time configuration.
 * The following parameters are affected:
 * - TCPIP_PARAMID_FRAMEPRIO
 * - TCPIP_PARAMID_TTL
 *
 * \param[out] socketPtr    Points to structure to store the default socket values (Ip-Parameters) to.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Udp_changeSocketToDefault
(
    P2VAR(TcpIp_Socket_UDP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Change parameters of a Udp socket
 *
 * This API service is called to change certain parameters of a Udp socket.
 *
 * \param[in]  socketPtr      Pointer to Udp socket structure
 * \param[in]  paramValuePtr  New value to use for a certain parameter
 * \param[in]  paramId        Identifies the parameter to change
 *
 * \retval E_OK      The new parameter value has been accepted.
 * \retval E_NOT_OK  The new parameter value has not been accepted, e.g. parameter/value
 *                   invalid.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_changeParameter
(
  P2VAR(TcpIp_Socket_UDP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr,
  TcpIp_ParamIdType paramId
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and reset dropped datagrams caused by invalid destination UDP-Port
 *
 * This function reads and resets dropped datagrams caused by invalid destination UDP-Port.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement data
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Udp_GetAndResetInvalidSocketCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#endif /* ifndef TCPIP_UDP_INT_H */
/*==================[end of file]===========================================*/
