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

#ifndef TCPIP_TCP_INT_H
#define TCPIP_TCP_INT_H

/*==================[inclusions]============================================*/

#include <TcpIp_Tcp_Api.h>      /* API declarations implemented by this SW-unit */
#include <TcpIp_Socket_Api.h>   /* API declarations implemented by this SW-unit */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_Ip_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Socket_Int.h>   /* used SW-unit interface file */

/*==================[macros]================================================*/


/*==================[type definitions]======================================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


/** \brief Reception of TCP segment
 *
 * By this API service the TCP SW-Unit gets an indication of a received TCP segment.
 *
 * \param[in] remoteAddrPtr     Source address and source port of received segment.
 * \param[in] localIpAddrPtr    Destination address of received segment.
 * \param[in] dataPtr           Points to the received data. The data contains the payload
 *                              of the Ethernet protocol (which excludes the Ethernet
 *                              header but includes headers of higher layers).
 * \param[in] localAddrId       Local address the frame has been received on.
 * \param[in] lenByte           Length of received data.
 * \param[in] isFragmented      If true, the packet is fragmented
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxIndication
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
);

/** \brief Initialize the transmission control block
 *
 * This API sets the information of a TCP connection and IP parameters to its default values.
 *
 * \param[out] socketPtr   Points to structure to store the socket default values to.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_changeSocketToDefault
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Periodic TCP socket function
 *
 * This API service is called to perform periodic tasks like retransmissions of segments
 * and transmission of delayed acknowledgements.
 *
 * \param[in] socketId   Socket identifier
 * \param[in] socketPtr  Pointer to Tcp socket structure
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_processTcpSocket
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief TCP socket function transmission
 *
 * This API service is called to perform task like transmission.
 *
 * \param[in] socketId   Socket identifier
 * \param[in] socketPtr  Pointer to Tcp socket structure
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_processTcpSocketTx
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Regular close of a TCP connection
 *
 * This API service is called to perform a regular TCP connection termination.
 * (through 3 way handshake)
 *
 * \param[in]  socketPtr    Pointer to Tcp socket structure
 * \param[in]  detReporting Det reporting on or off
 *                          TRUE: report development errors to Det
 *                          FALSE: do not report development errors to Det
 *
 * \retval E_OK      The request has been accepted.
 * \retval E_NOT_OK  The request has not been accepted e.g. connection already
 *                   closing/closed
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_close
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean detReporting
);

/** \brief Immediate close of a TCP connection
 *
 * This API service is called to perform an immediate TCP connection termination.
 * (through transmitting a reset)
 *
 * \param[in]  socketId     Socket identifier
 * \param[in]  socketPtr    Pointer to Tcp socket structure
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_abort
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Immediate close of a TCP connection
 *
 * This API service is called to perform an immediate TCP connection termination.
 * (without transmitting a reset)
 *
 * \param[in]  socketId     Socket identifier
 * \param[in]  socketPtr    Pointer to Tcp socket structure
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_terminate
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Change parameters of a Tcp connection
 *
 * This API service is called to change certain parameters of a Tcp connection.
 *
 * \param[in]  socketPtr      Pointer to Tcp socket structure
 * \param[in]  paramValuePtr  New value to use for a certain parameter
 * \param[in]  paramId        Identifies the parameter to change
 *
 * \retval E_OK      The new parameter value has been accepted.
 * \retval E_NOT_OK  The new parameter value has not been accepted, e.g. parameter/value
 *                   invalid.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameter
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr,
  TcpIp_ParamIdType paramId
);

/** \brief Copy data
 *
 *  This function copies data from the memory buffer into the pointer provided by
 *  bufferPtr.
 *
 * \param[in] that              Points to the functor data
 * \param[in] bufferPtr         Pointer to the destination memory location, data is
 *                              copied to this location
 * \param[in] copyLength        number of bytes that shall be copied
 *
 * \retval BUFREQ_OK     Data was successfully copied
 */
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Tcp_copyData
(
    P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
    uint16 copyLength
);

/** \brief Sets TCP header fields
 *
 * This function appends the TCP header at the location specified by dataPtr.
 *
 * \param[in] that              The functor's 'this' pointer.
 * \param[in] payloadSum        Checksum of the upper layer payload and ip header
 * \param[in] headerPtr         Points to data buffer where the Tcp header is located.
 * \param[in] calculateChecksum If true calculate the TCP checksum
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
);

/** \brief Increase Initial Sequence Number Generator
 *
 * This function is periodically called to increase the Initial Sequence Number.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_mainFunction
(
  void
);

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and reset dropped datagrams caused by invalid destination TCP-Port
 *
 * This function reads and resets dropped datagrams caused by invalid destination TCP-Port.
 *
 * \param[in] MeasurementResetNeeded  Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_GetAndResetInvalidSocketCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/** \brief Initializes SW-Unit Tcp
 *
 * This API service performs the initialization of the TCP related data structures.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_init
(
  void
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* TCPIP_TCP_INT_H */
